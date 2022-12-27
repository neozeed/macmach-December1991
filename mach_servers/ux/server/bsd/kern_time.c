/* 
 * Mach Operating System
 * Copyright (c) 1988 Carnegie-Mellon University
 * All rights reserved.  The CMU software License Agreement specifies
 * the terms and conditions for use and redistribution.
 */
/* 
 * HISTORY
 * $Log:	kern_time.c,v $
 * Revision 2.2  90/06/19  23:07:24  rpd
 * 	Purged CMUCS, MACH_NO_KERNEL conditionals.
 * 	Changed psignal in realitexpire to psignal3.
 * 	[90/06/08            rpd]
 * 
 * 	Out-of-kernel version.
 * 	[89/01/05            dbg]
 * 
 * Revision 2.1  89/08/04  14:07:46  rwd
 * Created.
 * 
 * Revision 2.3  88/08/24  01:21:39  mwyoung
 * 	Corrected include file references.
 * 	[88/08/22  22:04:32  mwyoung]
 * 
 * Revision 2.2  88/08/22  21:24:32  mja
 * 	More bug fixes from Brad White to allow adjtime() to work
 * 	better on the SUN3.
 * 	[88/08/22  12:59:31  mja]
 * 	
 * 	Eliminate NET_BUGFIX conditional;  consolidate CMUCS conditionals;
 * 	reset TODR to clock when adjust time delta is disabled [BWW].
 * 	[88/08/04  12:31:12  mja]
 * 
 * 17-Sep-87  Jonathan Chew (jjc) at Carnegie-Mellon University
 *	CMUCS: in adjtime(), make sure timedelta is a multiple of
 *	tickdelta (NOT tickadj!).
 *
 * 21-Apr-87  Mike Accetta (mja) at Carnegie-Mellon University
 *	Installed updates from UCB for null pointers in
 *	{s,g}ettimeofday().
 *	[ V5.1(F10) ]
 *
 * 08-Jan-87  Robert Beck (beck) at Sequent Computer Systems, Inc.
 *	Add include cputypes.h for BALANCE definiton.
 *	If BALANCE, gettimeofday() returns max # users in u.u_r.r_val2 (DYNIX
 *	login and init use this as legal max # users allowed to be logged on
 *	the system).
 *
 * 22-Mar-86  Avadis Tevanian (avie) at Carnegie-Mellon University
 *	Made the splhigh changes conditional on CMUCS.
 *
 *  6-Mar-86  Bill Bolosky (bolosky) at Carnegie-Mellon University
 *	Changed spl7() into splhigh().
 *
 ************************************************************************
 */
/*
 * Copyright (c) 1982, 1986 Regents of the University of California.
 * All rights reserved.  The Berkeley software License Agreement
 * specifies the terms and conditions for redistribution.
 *
 *	@(#)kern_time.c	7.4 (Berkeley) 3/23/87
 */

#include <cputypes.h>

#include <sys/param.h>
#include <sys/dir.h>		/* XXX */
#include <sys/user.h>
#include <sys/kernel.h>
#include <sys/inode.h>
#include <sys/proc.h>

/* 
 * Time of day and interval timer support.
 *
 * These routines provide the kernel entry points to get and set
 * the time-of-day and per-process interval timers.  Subroutines
 * here provide support for adding and subtracting timeval structures
 * and decrementing interval timers, optionally reloading the interval
 * timers when they expire.
 */

gettimeofday()
{
	register struct a {
		struct	timeval *tp;
		struct	timezone *tzp;
	} *uap = (struct a *)u.u_ap;
	struct timeval atv;

#if	BALANCE
	/*
	 * Return number of users to programs that limit # of users.
	 * (value in R1 on return from gettimeofday()).
	 *
	 * DYNIX login uses this to tell if too many users logged in
	 * for a binary license.
	 */
	{
		extern unsigned sec0eaddr;
		u.u_r.r_val2 = sec0eaddr;
	}
#endif	BALANCE
	if (uap->tp) {
		get_time(&atv);
		u.u_error = copyout((caddr_t)&atv, (caddr_t)uap->tp,
			sizeof (atv));
		if (u.u_error)
			return;
	}
	if (uap->tzp)
		u.u_error = copyout((caddr_t)&tz, (caddr_t)uap->tzp,
			sizeof (tz));
}

settimeofday()
{
	register struct a {
		struct	timeval *tv;
		struct	timezone *tzp;
	} *uap = (struct a *)u.u_ap;
	struct timeval atv;
	struct timezone atz;

	if (uap->tv) {
		u.u_error = copyin((caddr_t)uap->tv, (caddr_t)&atv,
			sizeof (struct timeval));
		if (u.u_error)
			return;
		setthetime(&atv);
	}
	if (uap->tzp && suser()) {
		u.u_error = copyin((caddr_t)uap->tzp, (caddr_t)&atz,
			sizeof (atz));
		if (u.u_error == 0)
			tz = atz;
	}
}

setthetime(tv)
	struct timeval *tv;
{
	int s;

	if (!suser())
		return;
/* WHAT DO WE DO ABOUT PENDING REAL-TIME TIMEOUTS??? */
	{
	    struct timeval time;
	    get_time(&time);
	    boottime.tv_sec += tv->tv_sec - time.tv_sec;
	    set_time(tv);
	}
}

adjtime()
{
	/* XXX */
}

/*
 * Get value of an interval timer.  The process virtual and
 * profiling virtual time timers are kept in the u. area, since
 * they can be swapped out.  These are kept internally in the
 * way they are specified externally: in time until they expire.
 *
 * The real time interval timer is kept in the process table slot
 * for the process, and its value (it_value) is kept as an
 * absolute time rather than as a delta, so that it is easy to keep
 * periodic real-time signals from drifting.
 *
 * Virtual time timers are processed in the hardclock() routine of
 * kern_clock.c.  The real time timer is processed by a timeout
 * routine, called from the softclock() routine.  Since a callout
 * may be delayed in real time due to interrupt processing in the system,
 * it is possible for the real time timeout routine (realitexpire, given below),
 * to be delayed in real time past when it is supposed to occur.  It
 * does not suffice, therefore, to reload the real timer .it_value from the
 * real time timers .it_interval.  Rather, we compute the next time in
 * absolute time the timer should go off.
 */
getitimer()
{
	register struct a {
		u_int	which;
		struct	itimerval *itv;
	} *uap = (struct a *)u.u_ap;
	struct itimerval aitv;
	int s;

	if (uap->which > 2) {
		u.u_error = EINVAL;
		return;
	}
	if (uap->which == ITIMER_REAL) {
		struct timeval time;
		get_time(&time);
		/*
		 * Convert from absoulte to relative time in .it_value
		 * part of real time timer.  If time for real time timer
		 * has passed return 0, else return difference between
		 * current time and time for the timer to go off.
		 */
		aitv = u.u_procp->p_realtimer;
		if (timerisset(&aitv.it_value))
			if (timercmp(&aitv.it_value, &time, <))
				timerclear(&aitv.it_value);
			else
				timevalsub(&aitv.it_value, &time);
	} else
		aitv = u.u_timer[uap->which];
	u.u_error = copyout((caddr_t)&aitv, (caddr_t)uap->itv,
	    sizeof (struct itimerval));
}

setitimer()
{
	register struct a {
		u_int	which;
		struct	itimerval *itv, *oitv;
	} *uap = (struct a *)u.u_ap;
	struct itimerval aitv, *aitvp;
	int s;
	register struct proc *p = u.u_procp;

	if (uap->which > 2) {
		u.u_error = EINVAL;
		return;
	}
	aitvp = uap->itv;
	if (uap->oitv) {
		uap->itv = uap->oitv;
		getitimer();
	}
	if (aitvp == 0)
		return;
	u.u_error = copyin((caddr_t)aitvp, (caddr_t)&aitv,
	    sizeof (struct itimerval));
	if (u.u_error)
		return;
	if (itimerfix(&aitv.it_value) || itimerfix(&aitv.it_interval)) {
		u.u_error = EINVAL;
		return;
	}
	if (uap->which == ITIMER_REAL) {
		struct timeval time;
		get_time(&time);
		untimeout(realitexpire, (caddr_t)p);
		if (timerisset(&aitv.it_value)) {
			timevaladd(&aitv.it_value, &time);
			timeout(realitexpire, (caddr_t)p, hzto(&aitv.it_value));
		}
		p->p_realtimer = aitv;
	} else
		u.u_timer[uap->which] = aitv;
}

/*
 * Real interval timer expired:
 * send process whose timer expired an alarm signal.
 * If time is not set up to reload, then just return.
 * Else compute next time timer should go off which is > current time.
 * This is where delay in processing this timeout causes multiple
 * SIGALRM calls to be compressed into one.
 */
realitexpire(p)
	register struct proc *p;
{
	int s;

	psignal3(p, SIGALRM, FALSE);
	if (!timerisset(&p->p_realtimer.it_interval)) {
		timerclear(&p->p_realtimer.it_value);
		return;
	}
	for (;;) {
		struct timeval time;
		get_time(&time);
		timevaladd(&p->p_realtimer.it_value,
		    &p->p_realtimer.it_interval);
		if (timercmp(&p->p_realtimer.it_value, &time, >)) {
			timeout(realitexpire, (caddr_t)p,
			    hzto(&p->p_realtimer.it_value));
			return;
		}
	}
}

/*
 * Check that a proposed value to load into the .it_value or
 * .it_interval part of an interval timer is acceptable, and
 * fix it to have at least minimal value (i.e. if it is less
 * than the resolution of the clock, round it up.)
 */
itimerfix(tv)
	struct timeval *tv;
{

	if (tv->tv_sec < 0 || tv->tv_sec > 100000000 ||
	    tv->tv_usec < 0 || tv->tv_usec >= 1000000)
		return (EINVAL);
	if (tv->tv_sec == 0 && tv->tv_usec != 0 && tv->tv_usec < tick)
		tv->tv_usec = tick;
	return (0);
}

/*
 * Decrement an interval timer by a specified number
 * of microseconds, which must be less than a second,
 * i.e. < 1000000.  If the timer expires, then reload
 * it.  In this case, carry over (usec - old value) to
 * reducint the value reloaded into the timer so that
 * the timer does not drift.  This routine assumes
 * that it is called in a context where the timers
 * on which it is operating cannot change in value.
 */
itimerdecr(itp, usec)
	register struct itimerval *itp;
	int usec;
{

	if (itp->it_value.tv_usec < usec) {
		if (itp->it_value.tv_sec == 0) {
			/* expired, and already in next interval */
			usec -= itp->it_value.tv_usec;
			goto expire;
		}
		itp->it_value.tv_usec += 1000000;
		itp->it_value.tv_sec--;
	}
	itp->it_value.tv_usec -= usec;
	usec = 0;
	if (timerisset(&itp->it_value))
		return (1);
	/* expired, exactly at end of interval */
expire:
	if (timerisset(&itp->it_interval)) {
		itp->it_value = itp->it_interval;
		itp->it_value.tv_usec -= usec;
		if (itp->it_value.tv_usec < 0) {
			itp->it_value.tv_usec += 1000000;
			itp->it_value.tv_sec--;
		}
	} else
		itp->it_value.tv_usec = 0;		/* sec is already 0 */
	return (0);
}

/*
 * Add and subtract routines for timevals.
 * N.B.: subtract routine doesn't deal with
 * results which are before the beginning,
 * it just gets very confused in this case.
 * Caveat emptor.
 */
timevaladd(t1, t2)
	struct timeval *t1, *t2;
{

	t1->tv_sec += t2->tv_sec;
	t1->tv_usec += t2->tv_usec;
	timevalfix(t1);
}

timevalsub(t1, t2)
	struct timeval *t1, *t2;
{

	t1->tv_sec -= t2->tv_sec;
	t1->tv_usec -= t2->tv_usec;
	timevalfix(t1);
}

timevalfix(t1)
	struct timeval *t1;
{

	if (t1->tv_usec < 0) {
		t1->tv_sec--;
		t1->tv_usec += 1000000;
	}
	if (t1->tv_usec >= 1000000) {
		t1->tv_sec++;
		t1->tv_usec -= 1000000;
	}
}
