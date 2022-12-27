/* 
 * Mach Operating System
 * Copyright (c) 1987 Carnegie-Mellon University
 * All rights reserved.  The CMU software License Agreement specifies
 * the terms and conditions for use and redistribution.
 */
/*
 **********************************************************************
 * HISTORY
 * $Log:	time.h,v $
 * Revision 2.1  89/08/04  14:48:50  rwd
 * Created.
 * 
 * Revision 2.2  88/08/24  02:48:22  mwyoung
 * 	Adjusted include file references.
 * 	[88/08/17  02:25:13  mwyoung]
 * 
 *
 * 06-Jan-88  Jay Kistler (jjk) at Carnegie Mellon University
 *	Added declarations for __STDC__.
 *
 * 20-Jul-86  Michael Young (mwyoung) at Carnegie-Mellon University
 *	Added _TIME_ definition check to prevent multiple inclusion.
 *
 * 27-Jan-86  Avadis Tevanian (avie) at Carnegie-Mellon University
 *	Restored the "struct tm" definition, we need it in kern_clock.
 *
 ************************************************************************
 */

#ifdef	KERNEL
#include <cmucs.h>
#endif	KERNEL
/*
 * Copyright (c) 1982, 1986 Regents of the University of California.
 * All rights reserved.  The Berkeley software License Agreement
 * specifies the terms and conditions for redistribution.
 *
 *	@(#)time.h	7.1 (Berkeley) 6/4/86
 */
#ifndef	_TIME_
#define	_TIME_	1

/*
 * Structure returned by gettimeofday(2) system call,
 * and used in other calls.
 */
struct timeval {
	long	tv_sec;		/* seconds */
	long	tv_usec;	/* and microseconds */
};

struct timezone {
	int	tz_minuteswest;	/* minutes west of Greenwich */
	int	tz_dsttime;	/* type of dst correction */
};
#define	DST_NONE	0	/* not on dst */
#define	DST_USA		1	/* USA style dst */
#define	DST_AUST	2	/* Australian style dst */
#define	DST_WET		3	/* Western European dst */
#define	DST_MET		4	/* Middle European dst */
#define	DST_EET		5	/* Eastern European dst */
#define	DST_CAN		6	/* Canada */

/*
 * Operations on timevals.
 *
 * NB: timercmp does not work for >= or <=.
 */
#define	timerisset(tvp)		((tvp)->tv_sec || (tvp)->tv_usec)
#define	timercmp(tvp, uvp, cmp)	\
	((tvp)->tv_sec cmp (uvp)->tv_sec || \
	 (tvp)->tv_sec == (uvp)->tv_sec && (tvp)->tv_usec cmp (uvp)->tv_usec)
#define	timerclear(tvp)		(tvp)->tv_sec = (tvp)->tv_usec = 0

/*
 * Names of the interval timers, and structure
 * defining a timer setting.
 */
#define	ITIMER_REAL	0
#define	ITIMER_VIRTUAL	1
#define	ITIMER_PROF	2

struct	itimerval {
	struct	timeval it_interval;	/* timer interval */
	struct	timeval it_value;	/* current value */
};

#ifndef KERNEL
#include <time.h>
#endif
#if	CMUCS && defined(KERNEL)
/*
 * Structure returned by gmtime and localtime calls (see ctime(3)).
 */
struct tm {
	int	tm_sec;
	int	tm_min;
	int	tm_hour;
	int	tm_mday;
	int	tm_mon;
	int	tm_year;
	int	tm_wday;
	int	tm_yday;
	int	tm_isdst;
};
#endif	CMUCS && defined(KERNEL)

#if	defined(CMUCS) && defined(__STDC__) && !defined(KERNEL)
extern int adjtime(struct timeval *, struct timeval *);
extern int getitimer(int, struct itimerval *);
extern int setitimer(int, struct itimerval *, struct itimerval *);
extern int gettimeofday(struct timeval *, struct timezone *);
extern int settimeofday(struct timeval *, struct timezone *);
extern int utimes(const char *, struct timeval *);
#endif	defined(CMUCS) && defined(__STDC__) && !defined(KERNEL)
#endif	_TIME_
