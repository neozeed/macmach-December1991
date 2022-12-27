/* 
 * Mach Operating System
 * Copyright (c) 1987 Carnegie-Mellon University
 * All rights reserved.  The CMU software License Agreement specifies
 * the terms and conditions for use and redistribution.
 */
/*
 * HISTORY
 * $Log:	kernel.h,v $
 * Revision 2.4  90/06/02  15:25:23  rpd
 * 	Cleaned up conditionals; removed MACH, CMU, CMUCS, MACH_NO_KERNEL.
 * 	[90/04/28            rpd]
 * 
 * Revision 2.3  89/09/15  15:29:10  rwd
 * 	Made mapped time dependant on MAP_TIME
 * 	[89/09/13            rwd]
 * 
 * Revision 2.2  89/08/09  14:45:53  rwd
 * 	Changed get_time to macro
 * 	[89/08/08            rwd]
 * 	Out-of-kernel version.  Removed time variable and replaced by
 * 	subroutine call.
 * 	[89/01/05            dbg]
 * 
 * Revision 2.4  88/08/24  02:32:40  mwyoung
 * 	Adjusted include file references.
 * 	[88/08/17  02:15:39  mwyoung]
 * 
 *
 *  5-Feb-88  Joseph Boykin (boykin) at ENcore Computer Corporation
 *	Added include of 'time.h'.
 *
 * 18-Nov-87  Avadis Tevanian (avie) at Carnegie-Mellon University
 *	Use MACH conditional.
 */
/*
 * Copyright (c) 1982, 1986 Regents of the University of California.
 * All rights reserved.  The Berkeley software License Agreement
 * specifies the terms and conditions for redistribution.
 *
 *	@(#)kernel.h	7.1 (Berkeley) 6/4/86
 */

#ifndef	_SYS_KERNEL_H_
#define	_SYS_KERNEL_H_

#ifdef	KERNEL
#include <map_time.h>
#else	KERNEL
#ifndef	MAP_TIME
#define	MAP_TIME	1
#endif	MAP_TIME
#endif	KERNEL

#include <sys/time.h>

/*
 * Global variables for the kernel
 */

long	rmalloc();

/* 1.1 */
long	hostid;
char	hostname[MAXHOSTNAMELEN];
int	hostnamelen;

/* 1.2 */
struct	timeval boottime;
#if	MAP_TIME
struct timeval *mtime;
#define get_time(x) *(x) = *mtime;
#else	MAP_TIME
void get_time();
#endif	MAP_TIME
struct	timezone tz;			/* XXX */
int	hz;
int	phz;				/* alternate clock's frequency */
int	tick;
int	lbolt;				/* awoken once a second */
int	realitexpire();

#define	LSCALE	1000		/* scaling for "fixed point" arithmetic */
extern	long	avenrun[3];
extern	long	mach_factor[3];

#ifdef	GPROF
extern	int profiling;
extern	char *s_lowpc;
extern	u_long s_textsize;
extern	u_short *kcount;
#endif	GPROF

#endif	_SYS_KERNEL_H_
