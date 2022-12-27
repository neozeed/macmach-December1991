/* 
 * Mach Operating System
 * Copyright (c) 1991,1990,1989,1988 Carnegie Mellon University
 * All Rights Reserved.
 * 
 * Permission to use, copy, modify and distribute this software and its
 * documentation is hereby granted, provided that both the copyright
 * notice and this permission notice appear in all copies of the
 * software, derivative works or modified versions, and any portions
 * thereof, and that both notices appear in supporting documentation.
 * 
 * CARNEGIE MELLON ALLOWS FREE USE OF THIS SOFTWARE IN ITS "AS IS"
 * CONDITION.  CARNEGIE MELLON DISCLAIMS ANY LIABILITY OF ANY KIND FOR
 * ANY DAMAGES WHATSOEVER RESULTING FROM THE USE OF THIS SOFTWARE.
 * 
 * Carnegie Mellon requests users of this software to return to
 * 
 *  Software Distribution Coordinator  or  Software.Distribution@CS.CMU.EDU
 *  School of Computer Science
 *  Carnegie Mellon University
 *  Pittsburgh PA 15213-3890
 * 
 * any improvements or extensions that they make and grant Carnegie Mellon
 * the rights to redistribute these changes.
 */
/*
 * HISTORY
 * $Log:	mach_clock.c,v $
 * Revision 2.16  91/08/03  18:18:56  jsb
 * 	NORMA_IPC: added call to netipc_timeout in hardclock.
 * 	[91/07/24  22:30:22  jsb]
 * 
 * Revision 2.15  91/07/31  17:45:57  dbg
 * 	Fixed timeout race.  Implemented host_adjust_time.
 * 	[91/07/30  17:03:54  dbg]
 * 
 * Revision 2.14  91/05/18  14:32:29  rpd
 * 	Fixed timeout/untimeout to use a fixed-size array of timers
 * 	instead of a zone.
 * 	[91/03/31            rpd]
 * 	Fixed host_set_time to update the mapped time value.
 * 	Changed the mapped time value to include a check field.
 * 	[91/03/19            rpd]
 * 
 * Revision 2.13  91/05/14  16:44:06  mrt
 * 	Correcting copyright
 * 
 * Revision 2.12  91/03/16  14:50:45  rpd
 * 	Updated for new kmem_alloc interface.
 * 	[91/03/03            rpd]
 * 	Use counter macros to track thread and stack usage.
 * 	[91/03/01  17:43:15  rpd]
 * 
 * Revision 2.11  91/02/05  17:27:45  mrt
 * 	Changed to new Mach copyright
 * 	[91/02/01  16:14:47  mrt]
 * 
 * Revision 2.10  91/01/08  15:16:22  rpd
 * 	Added continuation argument to thread_block.
 * 	[90/12/08            rpd]
 * 
 * Revision 2.9  90/11/05  14:31:27  rpd
 * 	Unified untimeout and untimeout_try.
 * 	[90/10/29            rpd]
 * 
 * Revision 2.8  90/10/12  18:07:29  rpd
 * 	Fixed calls to thread_bind in host_set_time.
 * 	Fix from Philippe Bernadat.
 * 	[90/10/10            rpd]
 * 
 * Revision 2.7  90/09/09  14:32:18  rpd
 * 	Use decl_simple_lock_data.
 * 	[90/08/30            rpd]
 * 
 * Revision 2.6  90/08/27  22:02:48  dbg
 * 	Add untimeout_try for multiprocessors.  Reduce lint.
 * 	[90/07/17            dbg]
 * 
 * Revision 2.5  90/06/02  14:55:04  rpd
 * 	Converted to new IPC and new host port technology.
 * 	[90/03/26  22:10:04  rpd]
 * 
 * Revision 2.4  90/01/11  11:43:31  dbg
 * 	Switch to master CPU in host_set_time.
 * 	[90/01/03            dbg]
 * 
 * Revision 2.3  89/08/09  14:33:09  rwd
 * 	Include mach/vm_param.h and use PAGE_SIZE instead of NBPG.
 * 	[89/08/08            rwd]
 * 	Removed timemmap to machine/model_dep.c
 * 	[89/08/08            rwd]
 * 
 * Revision 2.2  89/08/05  16:07:11  rwd
 * 	Added mappable time code.
 * 	[89/08/02            rwd]
 * 
 * 14-Jan-89  David Golub (dbg) at Carnegie-Mellon University
 *	Split into two new files: mach_clock (for timing) and priority
 *	(for priority calculation).
 *
 *  8-Dec-88  David Golub (dbg) at Carnegie-Mellon University
 *	Use sentinel for root of timer queue, to speed up search loops.
 *
 * 30-Jun-88  David Golub (dbg) at Carnegie-Mellon University
 *	Created.
 *
 */ 
/*
 *	File:	clock_prim.c
 *	Author:	Avadis Tevanian, Jr.
 *	Date:	1986
 *
 *	Clock primitives.
 */
#include <cpus.h>
#include <stat_time.h>
#include <norma_ipc.h>

#include <mach/boolean.h>
#include <mach/machine.h>
#include <mach/time_value.h>
#include <mach/vm_param.h>
#include <mach/vm_prot.h>
#include <kern/counters.h>
#include <kern/cpu_number.h>
#include <kern/host.h>
#include <kern/lock.h>
#include <kern/mach_param.h>
#include <kern/processor.h>
#include <kern/sched.h>
#include <kern/sched_prim.h>
#include <kern/thread.h>
#include <kern/time_out.h>
#include <kern/time_stamp.h>
#include <vm/vm_kern.h>
#include <sys/time.h>
#include <machine/mach_param.h>	/* HZ */
#include <machine/machparam.h>

extern void	thread_quantum_update();

void softclock();		/* forward */

int		hz = HZ;		/* number of ticks per second */
int		tick = (1000000 / HZ);	/* number of usec per tick */
time_value_t	time = { 0, 0 };	/* time since bootup (uncorrected) */
unsigned long	elapsed_ticks = 0;	/* ticks elapsed since bootup */

int		timedelta = 0;
int		tickdelta = 0;

#if	HZ > 100
int		tickadj = 1;		/* can adjust HZ usecs per second */
#else
int		tickadj = 100 / HZ;	/* can adjust 100 usecs per second */
#endif
int		bigadj = 1000000;	/* adjust 10*tickadj if adjustment
					   > bigadj */

/*
 *	This update protocol, with a check value, allows
 *		do {
 *			secs = mtime->seconds;
 *			usecs = mtime->microseconds;
 *		} while (secs != mtime->check_seconds);
 *	to read the time correctly.  (On a multiprocessor this assumes
 *	that processors see each other's writes in the correct order.
 *	We may have to insert fence operations.)
 */

mapped_time_value_t *mtime = 0;

#define update_mapped_time(time)				\
MACRO_BEGIN							\
	if (mtime != 0) {					\
		mtime->check_seconds = (time)->seconds;		\
		mtime->microseconds = (time)->microseconds;	\
		mtime->seconds = (time)->seconds;		\
	}							\
MACRO_END

decl_simple_lock_data(,	timer_lock)	/* lock for ... */
timer_elt_data_t	timer_head;	/* ordered list of timeouts */
					/* (doubles as end-of-list) */

/*
 *	Handle clock interrupts.
 *
 *	The clock interrupt is assumed to be called at a (more or less)
 *	constant rate.  The rate must be identical on all CPUS (XXX - fix).
 *
 *	Usec is the number of microseconds that have elapsed since the
 *	last clock tick.  It may be constant or computed, depending on
 *	the accuracy of the hardware clock.
 *
 */
void clock_interrupt(usec, usermode, basepri)
	register int	usec;		/* microseconds per tick */
	boolean_t	usermode;	/* executing user code */
	boolean_t	basepri;	/* at base priority */
{
	register int		my_cpu = cpu_number();
	register thread_t	thread = current_thread();

	counter(c_clock_ticks++);
	counter(c_threads_total += c_threads_current);
	counter(c_stacks_total += c_stacks_current);

#if	STAT_TIME
	/*
	 *	Increment the thread time, if using
	 *	statistical timing.
	 */
	if (usermode) {
	    timer_bump(&thread->user_timer, usec);
	}
	else {
	    timer_bump(&thread->system_timer, usec);
	}
#endif	STAT_TIME

	/*
	 *	Increment the CPU time statistics.
	 */
	{
	    register int	state;

	    if (usermode)
		state = CPU_STATE_USER;
	    else if (!cpu_idle(my_cpu))
		state = CPU_STATE_SYSTEM;
	    else
		state = CPU_STATE_IDLE;

	    machine_slot[my_cpu].cpu_ticks[state]++;

	    /*
	     *	Adjust the thread's priority and check for
	     *	quantum expiration.
	     */

	    thread_quantum_update(my_cpu, thread, 1, state);
	}

	/*
	 *	Time-of-day and time-out list are updated only
	 *	on the master CPU.
	 */
	if (my_cpu == master_cpu) {

	    register int s;
	    register timer_elt_t	telt;
	    boolean_t	needsoft = FALSE;

#if	TS_FORMAT == 1
	    /*
	     *	Increment the tick count for the timestamping routine.
	     */
	    ts_tick_count++;
#endif	TS_FORMAT == 1

	    /*
	     *	Update the tick count since bootup, and handle
	     *	timeouts.
	     */

	    s = splsched();
	    simple_lock(&timer_lock);

	    elapsed_ticks++;

	    telt = (timer_elt_t)queue_first(&timer_head.chain);
	    if (telt->ticks <= elapsed_ticks)
		needsoft = TRUE;
	    simple_unlock(&timer_lock);
	    splx(s);

	    /*
	     *	Increment the time-of-day clock.
	     */
	    if (timedelta == 0) {
		time_value_add_usec(&time, usec);
	    }
	    else {
		register int	delta;

		if (timedelta < 0) {
		    delta = usec - tickdelta;
		    timedelta += tickdelta;
		}
		else {
		    delta = usec + tickdelta;
		    timedelta -= tickdelta;
		}
		time_value_add_usec(&time, delta);
	    }
	    update_mapped_time(&time);

#if	NORMA_IPC
	    /*
	     *  Handle net protocol timeouts
	     */
	    netipc_timeout(usec);
#endif	/*NORMA_IPC*/

	    /*
	     *	Schedule soft-interupt for timeout if needed
	     */
	    if (needsoft) {
		if (basepri) {
		    (void) splsoftclock();
		    softclock();
		}
		else {
		    setsoftclock();
		}
	    }
	}
}

/*
 *	There is a nasty race between softclock and reset_timeout.
 *	For example, scheduling code looks at timer_set and calls
 *	reset_timeout, thinking the timer is set.  However, softclock
 *	has already removed the timer but hasn't called thread_timeout
 *	yet.
 *
 *	Interim solution:  We initialize timers after pulling
 *	them out of the queue, so a race with reset_timeout won't
 *	hurt.  The timeout functions (eg, thread_timeout,
 *	thread_depress_timeout) check timer_set/depress_priority
 *	to see if the timer has been cancelled and if so do nothing.
 *
 *	This still isn't correct.  For example, softclock pulls a
 *	timer off the queue, then thread_go resets timer_set (but
 *	reset_timeout does nothing), then thread_set_timeout puts the
 *	timer back on the queue and sets timer_set, then
 *	thread_timeout finally runs and clears timer_set, then
 *	thread_set_timeout tries to put the timer on the queue again
 *	and corrupts it.
 */

void softclock()
{
	/*
	 *	Handle timeouts.
	 */
	int	s;
	register timer_elt_t	telt;
	register int	(*fcn)();
	register char	*param;

	while (TRUE) {
	    s = splsched();
	    simple_lock(&timer_lock);
	    telt = (timer_elt_t) queue_first(&timer_head.chain);
	    if (telt->ticks > elapsed_ticks) {
		simple_unlock(&timer_lock);
		splx(s);
		break;
	    }
	    fcn = telt->fcn;
	    param = telt->param;

	    remqueue(&timer_head.chain, (queue_entry_t)telt);
	    telt->set = TELT_UNSET;
	    simple_unlock(&timer_lock);
	    splx(s);

	    assert(fcn != 0);
	    (*fcn)(param);
	}
}

/*
 *	Set timeout.
 *
 *	Parameters:
 *		telt	 timer element.  Function and param are already set.
 *		interval time-out interval, in hz.
 */
void set_timeout(telt, interval)
	register timer_elt_t	telt;	/* already loaded */
	register unsigned int	interval;
{
	int	s;
	register timer_elt_t	next;

	s = splsched();
	simple_lock(&timer_lock);

	interval += elapsed_ticks;

	for (next = (timer_elt_t)queue_first(&timer_head.chain);
	     ;
	     next = (timer_elt_t)queue_next((queue_entry_t)next)) {

	    if (next->ticks > interval)
		break;
	}
	telt->ticks = interval;
	/*
	 * Insert new timer element before 'next'
	 * (after 'next'->prev)
	 */
	insque((queue_entry_t) telt, ((queue_entry_t)next)->prev);
	telt->set = TELT_SET;
	simple_unlock(&timer_lock);
	splx(s);
}

boolean_t reset_timeout(telt)
	register timer_elt_t	telt;
{
	int	s;

	s = splsched();
	simple_lock(&timer_lock);
	if (telt->set) {
	    remqueue(&timer_head.chain, (queue_entry_t)telt);
	    telt->set = TELT_UNSET;
	    simple_unlock(&timer_lock);
	    splx(s);
	    return TRUE;
	}
	else {
	    simple_unlock(&timer_lock);
	    splx(s);
	    return FALSE;
	}
}

void init_timeout()
{
	simple_lock_init(&timer_lock);
	queue_init(&timer_head.chain);
	timer_head.ticks = ~0;	/* MAXUINT - sentinel */

	elapsed_ticks = 0;
}

/*
 * Read the time.
 */
kern_return_t
host_get_time(host, current_time)
	host_t		host;
	time_value_t	*current_time;	/* OUT */
{
	if (host == HOST_NULL)
		return(KERN_INVALID_HOST);

	do {
		current_time->seconds = mtime->seconds;
		current_time->microseconds = mtime->microseconds;
	} while (current_time->seconds != mtime->check_seconds);

	return (KERN_SUCCESS);
}

/*
 * Set the time.  Only available to privileged users.
 */
kern_return_t
host_set_time(host, new_time)
	host_t		host;
	time_value_t	new_time;
{
	int	s;

	if (host == HOST_NULL)
		return(KERN_INVALID_HOST);

#if	NCPUS > 1
	/*
	 * Switch to the master CPU to synchronize correctly.
	 */
	thread_bind(current_thread(), master_processor);
	if (current_processor() != master_processor)
	    thread_block((void (*)) 0);
#endif	NCPUS > 1

	s = splhigh();
	time = new_time;
	update_mapped_time(&time);
	resettodr();
	splx(s);

#if	NCPUS > 1
	/*
	 * Switch off the master CPU.
	 */
	thread_bind(current_thread(), PROCESSOR_NULL);
#endif	NCPUS > 1

	return (KERN_SUCCESS);
}

/*
 * Adjust the time gradually.
 */
kern_return_t
host_adjust_time(host, new_adjustment, old_adjustment)
	host_t		host;
	time_value_t	new_adjustment;
	time_value_t	*old_adjustment;	/* OUT */
{
	time_value_t	oadj;
	unsigned int	ndelta;
	int		s;

	if (host == HOST_NULL)
		return (KERN_INVALID_HOST);

	ndelta = new_adjustment.seconds * 1000000
		+ new_adjustment.microseconds;

#if	NCPUS > 1
	thread_bind(current_thread(), master_processor);
	if (current_processor() != master_processor)
	    thread_block((void (*)) 0);
#endif	NCPUS > 1

	s = splclock();

	oadj.seconds = timedelta / 1000000;
	oadj.microseconds = timedelta % 1000000;

	if (timedelta == 0) {
	    if (ndelta > bigadj)
		tickdelta = 10 * tickadj;
	    else
		tickdelta = tickadj;
	}
	if (ndelta % tickdelta)
	    ndelta = ndelta / tickdelta * tickdelta;

	timedelta = ndelta;

	splx(s);
#if	NCPUS > 1
	thread_bind(current_thread(), PROCESSOR_NULL);
#endif	NCPUS > 1

	*old_adjustment = oadj;

	return (KERN_SUCCESS);
}

mapable_time_init()
{
	if (kmem_alloc_wired(kernel_map, (vm_offset_t *) &mtime, PAGE_SIZE)
						!= KERN_SUCCESS)
		panic("mapable_time_init");
	bzero((char *)mtime, PAGE_SIZE);
	update_mapped_time(&time);
}

timeopen()
{
	return(0);
}
timeclose()
{
	return(0);
}

/*
 *	Compatibility for device drivers.
 *	New code should use set_timeout/reset_timeout and private timers.
 *	These code can't use a zone to allocate timers, because
 *	it can be called from interrupt handlers.
 */

#define NTIMERS		20

timer_elt_data_t timeout_timers[NTIMERS];

/*
 *	Set timeout.
 *
 *	fcn:		function to call
 *	param:		parameter to pass to function
 *	interval:	timeout interval, in hz.
 */
void timeout(fcn, param, interval)
	int	(*fcn)(/* char * param */);
	char *	param;
	int	interval;
{
	int	s;
	register timer_elt_t elt;

	s = splsched();
	simple_lock(&timer_lock);
	for (elt = &timeout_timers[0]; elt < &timeout_timers[NTIMERS]; elt++)
	    if (elt->set == TELT_UNSET)
		break;
	if (elt == &timeout_timers[NTIMERS])
	    panic("timeout");
	elt->fcn = fcn;
	elt->param = param;
	elt->set = TELT_ALLOC;
	simple_unlock(&timer_lock);
	splx(s);

	set_timeout(elt, (unsigned int)interval);
}

/*
 * Returns a boolean indicating whether the timeout element was found
 * and removed.
 */
boolean_t untimeout(fcn, param)
	register int	(*fcn)();
	register char *	param;
{
	int	s;
	register timer_elt_t elt;

	s = splsched();
	simple_lock(&timer_lock);
	queue_iterate(&timer_head.chain, elt, timer_elt_t, chain) {

	    if ((fcn == elt->fcn) && (param == elt->param)) {
		/*
		 *	Found it.
		 */
		remqueue(&timer_head.chain, (queue_entry_t)elt);
		elt->set = TELT_UNSET;

		simple_unlock(&timer_lock);
		splx(s);
		return (TRUE);
	    }
	}
	simple_unlock(&timer_lock);
	splx(s);
	return (FALSE);
}
