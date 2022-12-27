/* 
 * Mach Operating System
 * Copyright (c) 1988 Carnegie-Mellon University
 * All rights reserved.  The CMU software License Agreement specifies
 * the terms and conditions for use and redistribution.
 */
/*
 * HISTORY
 * $Log:	mach_clock.c,v $
 * Revision 2.5  90/08/06  15:31:25  rwd
 * 	Add Statistics code for the callout queue.
 * 	[90/06/08            rwd]
 * 
 * Revision 2.4  90/06/02  15:21:30  rpd
 * 	Updated set_thread_priority call for the new priority range.
 * 	[90/04/23            rpd]
 * 	Converted to new IPC.
 * 	[90/03/26  19:35:09  rpd]
 * 
 * Revision 2.3  89/12/08  20:14:22  rwd
 * 	Fic deallocation in untimeout_special
 * 	[89/11/29            rwd]
 * 	Change callout structures to a zone.  Add special
 * 	timeout/untimeout
 * 	[89/11/01            rwd]
 * 
 * 	Call cthread_wire before setting thread priority.
 * 	[89/11/01            rwd]
 * 
 * Revision 2.2  89/10/17  11:25:04  rwd
 * 	Make hzto return 0 if time has already passed.
 * 	[89/09/22            dbg]
 * 
 */
/*
 * Out-of-kernel replacements for timeout.
 */

#include <uxkern/import_mach.h>

#include <sys/param.h>
#include <sys/types.h>
#include <sys/kernel.h>
#include <sys/parallel.h>
#include <sys/time.h>
#include <sys/synch.h>
#include <sys/zalloc.h>

struct	callout {
	struct callout	*c_next;
	struct callout	*c_prev;
	struct timeval	c_time;		/* absolute time */
	caddr_t		c_arg;		/* argument to routine */
	int		(*c_func)();	/* routine */
	int		c_special;	/* special flag */
};
typedef struct callout	*callout_t;

#define CALLOUT_SIZE sizeof(struct callout)

zone_t		callout_zone;		/* callout zone */
struct mutex	callout_lock = MUTEX_INITIALIZER;
struct callout	calltodo = {&calltodo, &calltodo, 0, 0, 0, 0};

int	hz = 1000;		/* IPC timeout is in milliseconds */
int	tick = 1000;		/* 1000 usecs per tick */

mach_port_t	softclock_port;
cthread_t	softclock_thread;

mach_msg_header_t poke_softclock_msg;

any_t		softclock();	/* forward */
extern any_t	ux_thread_bootstrap();

#define DEBUG
#ifdef DEBUG
boolean_t	softclock_debug = FALSE;
boolean_t	callout_debug = FALSE;
struct callout_info {
	struct mutex	ci_mutex;
	int		ci_size;
	int		ci_inserts;
	int		ci_deletes;
	int		ci_zeros;
	int		ci_depth[16];
	int		ci_max;
} callout_info;
#endif DEBUG

callout_init()
{
	register int	i;

	callout_zone = zinit(CALLOUT_SIZE,1000*CALLOUT_SIZE,10*CALLOUT_SIZE,
			     TRUE,"Callout zone");

	(void) mach_port_allocate(mach_task_self(), MACH_PORT_RIGHT_RECEIVE,
				  &softclock_port);
	(void) mach_port_insert_right(mach_task_self(),
				      softclock_port, softclock_port,
				      MACH_MSG_TYPE_MAKE_SEND);

	softclock_thread = cthread_fork(ux_thread_bootstrap,
					(any_t)((int)softclock));
	cthread_detach(softclock_thread);

	/*
	 * Set up message used to poke softclock thread - it
	 * never changes.
	 */

	poke_softclock_msg.msgh_bits =
		MACH_MSGH_BITS(MACH_MSG_TYPE_COPY_SEND, 0);
	poke_softclock_msg.msgh_size = 0;
	poke_softclock_msg.msgh_remote_port = softclock_port;
	poke_softclock_msg.msgh_local_port = MACH_PORT_NULL;
	poke_softclock_msg.msgh_kind = MACH_MSGH_KIND_NORMAL;
	poke_softclock_msg.msgh_id = 0;
#ifdef	DEBUG
	if (callout_debug) {
	    bzero(&callout_info, sizeof(callout_info));
	    mutex_init(&callout_info.ci_mutex);
	}
#endif	DEBUG
}

timeout(func, arg, t)
	int	(*func)();
	caddr_t	arg;
	int	t;
{
	(void) timeout_special(func, arg, t, 0);
}

/*
 * Arrange that (*func)(arg) is called in t/hz seconds.
 */
int timeout_special(func, arg, t, s)
	int	(*func)();
	caddr_t	arg;
	int	t;
	int	s;
{
	register callout_t cp, c;
	boolean_t	reset;
	struct timeval	timeout_time, cur_time;
#ifdef	DEBUG
	int i = 0;
#endif	DEBUG

	if (t <= 0)
		t = 1;

	/*
	 * Convert timeout to absolute time.
	 */
	get_time(&cur_time);
	timeout_time = cur_time;
	timeout_time.tv_sec += t / hz;
	timeout_time.tv_usec += (t % hz) * tick;
	timevalfix(&timeout_time);
#ifdef DEBUG
if (softclock_debug)
  printf("At (%d,%d), timeout %d (msec) becomes time (%d,%d)\n",
    cur_time.tv_sec, cur_time.tv_usec,
    t, timeout_time.tv_sec, timeout_time.tv_usec);
#endif DEBUG

	mutex_lock(&callout_lock);

	c = (callout_t) zalloc(callout_zone);

	c->c_arg = arg;
	c->c_func = func;
	c->c_time = timeout_time;
	c->c_special = s;

	/*
	 * Insert the element at the correct place in the queue.
	 */
	for (cp = calltodo.c_next; cp != &calltodo; cp = cp->c_next) {
#ifdef	DEBUG
	    i++;
#endif	DEBUG

	    if (timercmp(&cp->c_time, &timeout_time, >)) {
		break;
	    }
	}
#ifdef	DEBUG
	if (callout_debug) {
		register int j;
		mutex_lock(&callout_info.ci_mutex);
		callout_info.ci_inserts++;
		callout_info.ci_size++;
		if (callout_info.ci_size > callout_info.ci_max)
		    callout_info.ci_max = callout_info.ci_size;
		j = i/16;
		if (j > 15) j = 15;
		callout_info.ci_depth[j]++;
		mutex_unlock(&callout_info.ci_mutex);
	}
#endif	DEBUG

	/*
	 * Insert new callout item
	 */
	c->c_next = cp;
	c->c_prev = cp->c_prev;
	cp->c_prev = c;
	(c->c_prev)->c_next = c;

	/*
	 * If the new callout item is now the first in the chain,
	 * poke the softclock thread to reset its timeout.
	 */
	reset = (calltodo.c_next == c);
	mutex_unlock(&callout_lock);

	if (reset && cthread_self() != softclock_thread) {
#ifdef DEBUG
if (softclock_debug)
  printf("        Resetting softclock\n");
#endif DEBUG
            (void) mach_msg(&poke_softclock_msg,
			    MACH_SEND_MSG|MACH_SEND_TIMEOUT,
			    sizeof poke_softclock_msg, 0, MACH_PORT_NULL,
			    0, MACH_PORT_NULL);
	}
	return (int) c;
}

untimeout_special(i)
	int i;
{
	register callout_t c = (callout_t)i;

	mutex_lock(&callout_lock);
	if (c->c_next != (callout_t)0) {
		if (calltodo.c_next == c) {
			c->c_func = (int (*)())0;
		} else {
			(c->c_prev)->c_next = c->c_next;
			(c->c_next)->c_prev = c->c_prev;
#ifdef	DEBUG
			if (callout_debug) {
				mutex_lock(&callout_info.ci_mutex);
				callout_info.ci_size--;
				mutex_unlock(&callout_info.ci_mutex);
			}
#endif	DEBUG
			zfree(callout_zone, c);
		}
	} else {
		zfree(callout_zone, c);
	}
	mutex_unlock(&callout_lock);
}

/*
 * untimeout is called to remove a function timeout call
 * from the callout structure.
 */
untimeout(func, arg)
	int	(*func)();
	caddr_t	arg;
{
	register callout_t	c;

	mutex_lock(&callout_lock);

	for (c = calltodo.c_next; c != &calltodo; c = c->c_next) {

	    if (c->c_func == func && c->c_arg == arg) {
		/*
		 * Found callout.
		 */
		if (calltodo.c_next == c) {
		    /*
		     * If we remove the first element on the chain,
		     * the softclock thread will give its timeout to
		     * the next element (firing it too early).
		     * Instead, clear the function pointer so that
		     * it will not be executed.
		     */
		    c->c_func = (int (*)())0;
		}
		else {
		    /*
		     * Not the first element - remove it from the list.
		     */
		    (c->c_prev)->c_next = c->c_next;
		    (c->c_next)->c_prev = c->c_prev;
#ifdef	DEBUG
		    if (callout_debug) {
			mutex_lock(&callout_info.ci_mutex);
			callout_info.ci_size--;
			callout_info.ci_deletes++;
			mutex_unlock(&callout_info.ci_mutex);
		    }
#endif	DEBUG
		    zfree(callout_zone, c);
		}
		break;
	    }
	}

	mutex_unlock(&callout_lock);
}

int
time_to_hz(tv)
	struct timeval *tv;
{
	register long	ticks, sec;

	/*
	 * If number of milliseconds will fit in 32 bit arithmetic,
	 * then compute number of milliseconds to time and scale to
	 * ticks.  Otherwise just compute number of hz in time, rounding
	 * times greater than representible to maximum value.
	 *
	 * Delta times less than 25 days can be computed ``exactly''.
	 * Maximum value for any timeout in 10ms ticks is 250 days.
	 */
	sec = tv->tv_sec;
	if (sec <= 0x7fffffff / 1000 - 1000)
		ticks = (tv->tv_sec * 1000 +
			 tv->tv_usec / 1000) / (tick / 1000);
	else if (sec <= 0x7fffffff / hz)
		ticks = sec * hz;
	else
		ticks = 0x7fffffff;

	return (ticks);
}

/*
 * Compute number of hz until specified time.
 * Used to compute third argument to timeout() from an
 * absolute time.
 */
hzto(tv)
	struct timeval *tv;
{
	struct timeval time, desired_time;

	desired_time = *tv;
	get_time(&time);
	if (!timercmp(&time, &desired_time, <))
	    return (0);
	timevalsub(&desired_time, &time);
	return (time_to_hz(&desired_time));
}

/*
 * Softclock thread.
 */
/*ARGSUSED*/
any_t
softclock(th_arg)
	any_t	th_arg;
{
	caddr_t	arg;
	int	t;
	int	(*func)();
	register callout_t c;
	struct timeval timeout_time, cur_time;
	mach_msg_header_t m;

	cthread_set_name(cthread_self(), "softclock");

	/*
	 * Wire this cthread to a kernel thread so we can
	 * up its priority
	 */
	cthread_wire();

	/*
	 * Make this thread high priority.
	 */
	set_thread_priority(mach_thread_self(), 1);

	while (TRUE) {

	    /*
	     * Set the timeout from the first element in the
	     * callout list that has not been removed.
	     */
	    mutex_lock(&callout_lock);
	    while ((c = calltodo.c_next) == &calltodo || c->c_func == 0) {
		if (c == &calltodo) {
		    /*
		     * No timeout - wait to be poked by timeout().
		     */
		    mutex_unlock(&callout_lock);

		    (void) mach_msg(&m, MACH_RCV_MSG,
				    0, sizeof m, softclock_port,
				    MACH_MSG_TIMEOUT_NONE, MACH_PORT_NULL);

		    mutex_lock(&callout_lock);
		}
		else {
		    /*
		     * Cleared timeout element.
		     */
		    calltodo.c_next = c->c_next;
		    (c->c_next)->c_prev = &calltodo;
#ifdef	DEBUG
		    if (callout_debug) {
			mutex_lock(&callout_info.ci_mutex);
			callout_info.ci_zeros++;
			callout_info.ci_size--;
			mutex_unlock(&callout_info.ci_mutex);
		    }
#endif	DEBUG
		    zfree(callout_zone, c);
		}
	    }
	    timeout_time = c->c_time;
	    mutex_unlock(&callout_lock);

	    /*
	     * Compute interval to wait.
	     */
	    get_time(&cur_time);
#ifdef DEBUG
if (softclock_debug)
  printf("    At (%d,%d), timeout time (%d,%d)",
    cur_time.tv_sec, cur_time.tv_usec,
    timeout_time.tv_sec, timeout_time.tv_usec);
#endif DEBUG
	    if (!timercmp(&cur_time, &timeout_time, <))
		t = 0;
	    else {
		timevalsub(&timeout_time, &cur_time);
		t = time_to_hz(&timeout_time);
#ifdef DEBUG
if (softclock_debug)
  printf(" becomes interval %d msec\n", t);
#endif DEBUG
	    }

	    if (t > 0) {
		/*
		 * Wait for timeout to go off.  If we receive
		 * a message instead, the timeout queue has
		 * been altered and we must reset the timeout.
		 */

		if (mach_msg(&m, MACH_RCV_MSG|MACH_RCV_TIMEOUT,
			     0, sizeof m, softclock_port,
			     t, MACH_PORT_NULL) != MACH_RCV_TIMED_OUT)
		    continue;
	    }

	    /*
	     * Remove the timeout from the queue and call the
	     * function (unless it has been reset).
	     */
#ifdef DEBUG
if (softclock_debug)
  printf("........Softclock timed out\n");
#endif DEBUG
	    mutex_lock(&callout_lock);

	    c = calltodo.c_next;

	    calltodo.c_next = c->c_next;
	    (c->c_next)->c_prev = &calltodo;

	    arg  = c->c_arg;
	    func = c->c_func;

#ifdef	DEBUG
	    if (callout_debug) {
		mutex_lock(&callout_info.ci_mutex);
		callout_info.ci_size--;
		mutex_unlock(&callout_info.ci_mutex);
	    }
#endif	DEBUG
	    if (c->c_special && c->c_func) {
		c->c_next = (callout_t)0;
	    } else {
		zfree(callout_zone, c);
	    }

	    mutex_unlock(&callout_lock);

	    /*
	     * Skip this item if it was cleared before we had
	     * a chance to notice.
	     */
	    if (func != (int (*)())0) {
		interrupt_enter(SPLSOFTCLOCK);
		(*func)(arg);
		interrupt_exit(SPLSOFTCLOCK);
	    }
	}
}
