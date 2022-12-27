/* 
 * Mach Operating System
 * Copyright (c) 1990 Carnegie-Mellon University
 * Copyright (c) 1989 Carnegie-Mellon University
 * Copyright (c) 1988 Carnegie-Mellon University
 * All rights reserved.  The CMU software License Agreement specifies
 * the terms and conditions for use and redistribution.
 */
/*
 * HISTORY
 * $Log:	vmparam.h,v $
 * Revision 2.2  90/08/30  18:23:48  bohman
 * 	Created.
 * 
 */

/*
 *	Apple Macintosh II Mach (macmach)
 *
 *	File: mac2/vmparam.h
 *	Author: David E. Bohman II (CMU macmach)
 */

/*
 * Copyright (c) 1982, 1986 Regents of the University of California.
 * All rights reserved.  The Berkeley software License Agreement
 * specifies the terms and conditions for redistribution.
 *
 *	@(#)vmparam.h	7.1 (Berkeley) 6/5/86
 */

/*
 * Machine dependent constants for mac2
 */

#ifndef _MAC2_VMPARAM_H_
#define _MAC2_VMPARAM_H_
/*
 * Base address for U*X system call emulator.
 */
#define	EMULATOR_END	(0-NBPG)
#define	EMULATOR_BASE	(EMULATOR_END - (256*1024))

#define MAP_FILE_BASE	(0 - (16*1024*1024))

#define EMULATOR_ABOVE_STACK	1

/*
 * USRTEXT is the start of the user text/data space, while USRSTACK
 * is the top (end) of the user stack.  The user stack can't start
 * at the top page (i.e. 0) due to the way that MACH handles ranges.
 */
#define	USRTEXT		NBPG
#define	USRSTACK	MAP_FILE_BASE

#include <mach/mac2/vm_param.h>

/*
 * Virtual memory related constants, all in bytes
 */
#ifndef DFLDSIZ
#define	DFLDSIZ		(6*1024*1024)		/* initial data size limit */
#endif
#ifndef MAXDSIZ
#define	MAXDSIZ		(16*1024*1024)		/* max data size */
#endif
#ifndef	DFLSSIZ
#define	DFLSSIZ		(512*1024)		/* initial stack size limit */
#endif
#ifndef	MAXSSIZ
#define	MAXSSIZ		MAXDSIZ			/* max stack size */
#endif

#endif	_MAC2_VMPARAM_H_
