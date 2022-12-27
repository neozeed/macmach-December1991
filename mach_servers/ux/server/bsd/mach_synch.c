/* 
 * Mach Operating System
 * Copyright (c) 1988 Carnegie-Mellon University
 * All rights reserved.  The CMU software License Agreement specifies
 * the terms and conditions for use and redistribution.
 */
/*
 * HISTORY
 * $Log:	mach_synch.c,v $
 * Revision 2.10  90/08/06  15:31:59  rwd
 * 	Change tsleep_main to use interruptible value as set by
 * 	tsleep_enter to avoid problem of exit occuring between calls.
 * 	[90/06/28            rwd]
 * 	Minor cleanups.  Break tsleep into two pieces.
 * 	[90/06/07            rwd]
 * 
 * Revision 2.9  90/06/19  23:08:13  rpd
 * 	Corrected arguments to longjmp.
 * 	[90/06/06            rpd]
 * 
 * 	Removed debugging code from sleep.
 * 	Added check for uu_proc_exit to tsleep.
 * 	[90/06/05            rpd]
 * 
 * Revision 2.8  90/06/02  15:22:23  rpd
 * 	Added debugging code to sleep, looking for longjmps
 * 	from sleep that would prematurely terminate proc_exit.
 * 	[90/05/30            rpd]
 * 	Fixed tsleep/spl bug.  From rwd:  code in spl_n to check
 * 	for bad value (-1) appearing in u.u_ipl and requested spl.
 * 	[90/05/12            rpd]
 * 	Fixed tsleep to check p_cursig, to prevent it from sleeping
 * 	or calling issig with a non-zero p_cursig.
 * 	[90/04/29            rpd]
 * 
 * Revision 2.7  90/05/29  20:23:23  rwd
 * 	Fixed tsleep/spl bug.  From rwd:  code in spl_n to check
 * 	for bad value (-1) appearing in u.u_ipl and requested spl.
 * 	[90/05/12            rpd]
 * 	Fixed tsleep to check p_cursig, to prevent it from sleeping
 * 	or calling issig with a non-zero p_cursig.
 * 	[90/04/29            rpd]
 * 
 * Revision 2.6  90/03/14  21:26:44  rwd
 * 	Change reference of PINOD+1 to PSPECL.
 * 	[90/02/16            rwd]
 * 
 * Revision 2.5  90/01/19  14:35:03  rwd
 * 	Call ux_server_thread_busy on rmt_* sleep calls (prio=PINOD+1)
 * 	[90/01/12            rwd]
 * 
 * Revision 2.4  89/12/08  20:14:30  rwd
 * 	Change SLEEP_HASH function to be more appropriate for a
 * 	multi-cthreaded server.
 * 	[89/12/04            rwd]
 * 	Fix uthread_wakeup.
 * 	[89/11/29            rwd]
 * 	Returned spl locking to original form.
 * 	[89/11/27            rwd]
 * 	Changed locking on spls and changed while (foo) condition_wait
 * 	to if (foo) condition_wait because of new locking.
 * 	[89/11/20            rwd]
 * 
 * 	Changed master_lock to mutex.  Changed waits to condition waits.
 * 	Timeouts for condition waits are implimented through the timeout
 * 	mechanism.  Use special timeout and untimeout.
 * 	[89/10/30            rwd]
 * 
 * Revision 2.2  89/10/17  11:26:10  rwd
 * 	Added DEBUG code
 * 	[89/09/26            rwd]
 * 
 * Revision 2.1.1.2  89/09/26  14:57:14  dbg
 * 	Add check for exit/stop in tsleep.
 * 	[89/09/21            dbg]
 * 
 * Revision 2.1.1.1  89/09/21  20:37:04  dbg
 * 	Make serialization on master_queue explicit.
 * 	[89/09/21            dbg]
 * 
 */
/*
 * Replacements for sleep, wakeup, tsleep, spl*.
 */

#include <sys/param.h>
#include <sys/types.h>
#include <sys/user.h>
#include <sys/parallel.h>
#include <sys/queue.h>
#include <sys/signal_macros.h>
#include <sys/kernel.h>
#include <sys/synch.h>
#include <sys/time.h>

#include <uxkern/import_mach.h>
#include <sys/message.h>
#include <cthreads.h>

extern void	ux_server_thread_busy();
extern void	ux_server_thread_active();
extern int	timeout_special();

/*
 * Sleeping threads are hashed by uu_wchan onto sleep queues.
 */
 
#define	SLEEP_QUEUE_SIZE	64	/* power of 2 */
#define	SLEEP_HASH(x)	(((int)(x)>>16) & (SLEEP_QUEUE_SIZE - 1))

queue_head_t	sleep_queue[SLEEP_QUEUE_SIZE];

struct mutex	master_mutex = MUTEX_INITIALIZER;
struct mutex	sleep_lock = MUTEX_INITIALIZER;

/*
 * Initialize the sleep queues.
 */
rqinit()
{
	register int i;

	for (i = 0; i < SLEEP_QUEUE_SIZE; i++)
	    queue_init(&sleep_queue[i]);
}

uthread_wakeup(uth)
	register uthread_t uth;
{
	register queue_t	q;

	mutex_lock(&sleep_lock);
	mutex_lock(&uth->uu_lock);
	if (uth->uu_sleep_chan) {
		q = &sleep_queue[SLEEP_HASH(uth->uu_sleep_chan)];
		queue_remove(q, uth, uthread_t, uu_sleep_link);
		uth->uu_sleep_chan = (char *)0;
	}
	condition_signal(&uth->uu_condition);
	mutex_unlock(&uth->uu_lock);
	mutex_unlock(&sleep_lock);
}

/*
 * Sleep until a wakeup occurs on chan.  If pri <= PZERO,
 * a signal cannot disturb the sleep; if pri > PZERO
 * signals will be processed.
 *
 * Return in no more than the indicated number of clock ticks.
 * If time_out == 0, no timeout.
 *
 * Return	TS_OK	if chan was awakened normally
 *		TS_TIME	if timeout occured
 *		TS_SIG	if signal occured or process exiting
 *
 * Process must have master_lock.
 */

/*
 * tsleep_enter is the setup routine which will put the thread on the
 * queue.  What ever locking is being used for races should then be
 * released, and tsleep_main called.
 */

tsleep_enter(chan, pri, time_out)
	caddr_t chan;
	int pri, time_out;	/* time_out in hz */
{
	register uthread_t	uth = &u;
	register struct proc	*p = uth->uu_procp;
	register queue_t	q;
	boolean_t		interruptible;

	/*
	 * Zero is a reserved value, used to indicate
	 * that we have been woken up and are no longer on
	 * the sleep queues.
	 */

	if (chan == 0)
	    panic("tsleep");

	/*
	 * We are not interruptible by signals if this thread
	 * is in the middle of proc_exit, which can't be aborted.
	 */
	interruptible = (pri > PZERO) && !uth->uu_proc_exit;

	/*
	 * The sleep_lock protects the sleep queues and
	 * the uu_sleep_chan/uu_interruptible fields in threads.
	 */

	mutex_lock(&sleep_lock);
	uth->uu_sleep_chan = chan;
	uth->uu_interruptible = interruptible;
	q = &sleep_queue[SLEEP_HASH(chan)];
	queue_enter(q, uth, uthread_t, uu_sleep_link);
	mutex_unlock(&sleep_lock);

}

int
tsleep_main(chan, pri, time_out)
	caddr_t chan;
	int pri, time_out;	/* time_out in hz */
{
	register uthread_t	uth = &u;
	register struct proc	*p = uth->uu_procp;
	register queue_t	q;
	boolean_t		timed_out;
	int			timeout_block;

	mutex_lock(&sleep_lock);
	if (uth->uu_sleep_chan == 0) {
	    /*
	     * We have already been woken up.
	     */

	    mutex_unlock(&sleep_lock);

	    if (uth->uu_interruptible) {
		if (EXITING(p) || (p->p_cursig != 0) ||
		    (HAVE_SIGNALS(p) && issig()))
		    return (TS_SIG);
	    }
	    return (TS_OK);
	}

	mutex_lock(&uth->uu_lock);
	if (uth->uu_interruptible) {
	    if (EXITING(p) || (p->p_cursig != 0) || HAVE_SIGNALS(p)) {
		mutex_unlock(&uth->uu_lock);
		/*
		 * Remove self from sleep queue and take signal.
		 */
		q = &sleep_queue[SLEEP_HASH(chan)];
		queue_remove(q, uth, uthread_t, uu_sleep_link);
		mutex_unlock(&sleep_lock);

		if (EXITING(p) || (p->p_cursig != 0) || issig())
		    return (TS_SIG);
		/*
		 * Return to caller, who will notice that sleep
		 * condition has not been satisfied and retry
		 * the sleep.
		 */
		return (TS_OK);
	    }
	}

	if (pri > PZERO || pri == PSPECL) ux_server_thread_busy();

	mutex_unlock(&sleep_lock);

	mutex_unlock(&master_mutex);
	if (time_out) {
		uth->uu_timedout=1;
		timeout_block = timeout_special(uthread_wakeup, uth ,
						time_out, 1);
	}
	condition_wait(&uth->uu_condition, &uth->uu_lock);
	timed_out = uth->uu_timedout;
	if (time_out) untimeout_special(timeout_block);
	mutex_unlock(&uth->uu_lock);

	mutex_lock(&master_mutex);

	if (pri > PZERO || pri == PSPECL) ux_server_thread_active();

	if (uth->uu_interruptible) {
	    if (EXITING(p) || (p->p_cursig != 0) ||
		(HAVE_SIGNALS(p) && issig()))
		return (TS_SIG);
	}
	return ((timed_out) ? TS_TIME : TS_OK);
}

tsleep(chan, pri, time_out)
	caddr_t chan;
	int pri, time_out;	/* time_out in hz */
{
	int			old_spl;
	int			result;

	tsleep_enter(chan, pri, time_out);

	/*
	 * Network interrupt code might be called from spl_n.
	 * This code might try to wake us up.  Hence we can't
	 * hold uth->uu_lock or sleep_lock across spl_n,
	 * or we could deadlock with ourself.
	 */
	old_spl = spl_n(0);

	result = tsleep_main(chan, pri, time_out);

	(void) splx(old_spl);

	return (result);
}

/*
 * Sleep on chan at pri.
 *
 * We make a special check for lbolt (the once-per-second timer)
 * to avoid keeping a separate lbolt thread or overloading the
 * timeout queue.
 */
sleep(chan, pri)
	caddr_t chan;
	int pri;
{
	if (tsleep(chan, pri, (chan == (caddr_t)&lbolt) ? hz : 0)
		== TS_SIG) {
	    /*
	     * Return at spl0 if signal.
	     */
	    (void) spl_n(0);
	    longjmp(&u.u_qsave, 1);
	}
}

wakeup(chan)
	register caddr_t chan;
{
	register queue_t q;
	register uthread_t	uth, next;

	mutex_lock(&sleep_lock);
	q = &sleep_queue[SLEEP_HASH(chan)];

	uth = (uthread_t)queue_first(q);
	while (!queue_end(q, (queue_entry_t)uth)) {
	    next = (uthread_t)queue_next(&uth->uu_sleep_link);
	    if (uth->uu_sleep_chan == chan) {
		queue_remove(q, uth, uthread_t, uu_sleep_link);
		uth->uu_sleep_chan = (char *)0;
		/*
		 * wakeup thread
		 */
		mutex_lock(&uth->uu_lock);
		uth->uu_timedout = 0;
		condition_signal(&uth->uu_condition);
		mutex_unlock(&uth->uu_lock);
	    }
	    uth = next;
	}
	mutex_unlock(&sleep_lock);
}

/*
 * Wakeup server thread to make it handle signals.
 */
unsleep(uth)
	register uthread_t	uth;
{
	register int		old_state;
	register queue_t	q;

	mutex_lock(&sleep_lock);
	if (uth->uu_interruptible && uth->uu_sleep_chan) {
		q = &sleep_queue[SLEEP_HASH(uth->uu_sleep_chan)];
		queue_remove(q, uth, uthread_t, uu_sleep_link);
		uth->uu_sleep_chan = (char *)0;
		mutex_lock(&uth->uu_lock);
		uth->uu_timedout = 0;
		condition_signal(&uth->uu_condition);
		mutex_unlock(&uth->uu_lock);
	}
	mutex_unlock(&sleep_lock);
}

/*
 * Stack of spl locks - one for each priority level.
 */
struct spl_lock {
    cthread_t		holder;
    struct mutex	lock;
    struct condition	condition;
};

struct spl_lock	spl_locks[SPL_COUNT];

void
spl_init()
{
	register struct spl_lock *sp;

	for (sp = &spl_locks[SPL_COUNT-1]; sp >= &spl_locks[0]; sp--) {
	    sp->holder = NO_CTHREAD;
	    mutex_init(&sp->lock);
	    condition_init(&sp->condition);
	}
}

int
spl_n(new_level)
	int	new_level;
{
	int	old_level;
	register int	i;
	register struct spl_lock *sp;
	register cthread_t	self = cthread_self();

	if (u.u_ipl < 0) {
	    panic("current ipl < 0",u.u_ipl);
	    u.u_ipl = 0;
	}

	if (new_level < 0) {
	    panic("new_level < 0");
	    new_level = 0;
	}

	old_level = u.u_ipl;

	if (new_level > old_level) {
	    /*
	     * Raising priority
	     */
	    for (i = old_level + 1; i <= new_level; i++) {
		sp = &spl_locks[i];

		mutex_lock(&sp->lock);
		while (sp->holder != self && sp->holder != NO_CTHREAD)
		    condition_wait(&sp->condition, &sp->lock);
		sp->holder = self;
		mutex_unlock(&sp->lock);
	    }
	}
	else if (new_level < old_level) {
	    /*
	     * Lowering priority
	     */
	    for (i = old_level; i > new_level; i--) {
		sp = &spl_locks[i];

		mutex_lock(&sp->lock);
		sp->holder = NO_CTHREAD;
		mutex_unlock(&sp->lock);
		condition_signal(&sp->condition);
	    }
	}
	u.u_ipl = new_level;

	/*
	 * Simulate software interrupts for network.
	 */
	{
	    extern int	netisr;
	    extern void	dosoftnet();

	    if (new_level < SPLNET && netisr) {
		register int s = splnet();
		dosoftnet();
		splx(s);
	    }
	}

	return (old_level);
}

#undef spl0
int spl0()
{
    return (spl_n(0));
}

#undef splsoftclock
int splsoftclock()
{
    return (spl_n(SPLSOFTCLOCK));
}

#undef splnet
int splnet()
{
    return (spl_n(SPLNET));
}

#undef splbio
int splbio()
{
    return (spl_n(SPLBIO));
}

#undef spltty
int spltty()
{
    return (spl_n(SPLTTY));
}

#undef splimp
int splimp()
{
    return (spl_n(SPLIMP));
}

#undef splhigh
int splhigh()
{
    return (spl_n(SPLHIGH));
}

#undef splx
int splx(s)
    int s;
{
    return(spl_n(s));
}

/*
 * Interrupt routines start at a given priority.  They may interrupt other
 * threads with a lower priority (unlike non-interrupt threads, which must
 * wait).
 */
void
interrupt_enter(level)
	int level;
{
	register cthread_t	self = cthread_self();
	register struct spl_lock *sp = &spl_locks[level];

	/*
	 * Grab the lock for the interrupt priority.
	 */

	mutex_lock(&sp->lock);
	while (sp->holder != self && sp->holder != NO_CTHREAD)
	    condition_wait(&sp->condition, &sp->lock);
	sp->holder = self;
	mutex_unlock(&sp->lock);

	u.u_ipl = level;
}

void
interrupt_exit(level)
	int level;
{
	register cthread_t	self = cthread_self();
	register struct spl_lock *sp;
	register int		i;

	/*
	 * Release the lock for the interrupt priority.
	 */

	for (i = u.u_ipl; i >= level; i--) {
	    sp = &spl_locks[i];
	    mutex_lock(&sp->lock);
	    sp->holder = NO_CTHREAD;
	    mutex_unlock(&sp->lock);
	    condition_signal(&sp->condition);
	}

	/*
	 * Simulate software interrupts for network.
	 */
	{
	    extern int	netisr;
	    extern void	dosoftnet();

	    if (netisr && level >= SPLNET) {
		/*
		 * Check SPL levels down to splnet.  If none held,
		 * take a softnet interrupt.
		 */
		for (i = level; i >= SPLNET; i--)
		    if (spl_locks[i].holder != NO_CTHREAD)
			goto exit;

		interrupt_enter(SPLNET);
		dosoftnet();
		interrupt_exit(SPLNET);

	      exit:;
	    }
	}

	u.u_ipl = -1;
}

