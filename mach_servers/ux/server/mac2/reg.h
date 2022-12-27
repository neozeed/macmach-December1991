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
 * $Log:	reg.h,v $
 * Revision 2.2  90/09/04  17:30:34  bohman
 * 	Created.
 * 
 */

/*
 *	Apple Macintosh II Mach (macmach)
 *
 *	File: mac2/reg.h
 */

#ifndef _MAC2_REG_H_
#define _MAC2_REG_H_

/*
 * Location of the users' stored
 * registers relative to R0.
 * Usage is u.u_ar0[XX].
 */
#define	R0	(0)
#define	R1	(1)
#define	R2	(2)
#define	R3	(3)
#define	R4	(4)
#define	R5	(5)
#define	R6	(6)
#define	R7	(7)
#define	AR0	(8)
#define	AR1	(9)
#define	AR2	(10)
#define	AR3	(11)
#define	AR4	(12)
#define	AR5	(13)
#define	AR6	(14)
#define	AR7	(15)
#define	SP	(15)

#include <mach/mac2/reg.h>

#endif	_MAC2_REG_H_
