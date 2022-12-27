/* 
 * Mach Operating System
 * Copyright (c) 1991,1990,1989 Carnegie Mellon University
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
 * $Log:	clock.h,v $
 * Revision 2.2  91/09/12  16:39:17  bohman
 * 	Created Mach 3.0 version from 2.5 version.
 * 	[91/09/11  14:25:06  bohman]
 * 
 * Revision 2.2  90/08/30  11:01:00  bohman
 * 	Created.
 * 	[90/08/29  11:30:58  bohman]
 * 
 */

/*
 *	Apple Macintosh II Mach (macmach)
 *
 *	File: mac2/clock.h
 *	Author: David E. Bohman II (CMU macmach)
 */

#ifndef _MAC2_CLOCK_H_
#define _MAC2_CLOCK_H_

/*
 * The delta (in seconds) between MacOS T(0) and U**X T(0)
 */
#define T0_DELTA (((365*(1970-1904))+((1970-1904)/4)+1)*24*60*60)

extern struct actlist	actclock;

#endif _MAC2_CLOCK_H_
