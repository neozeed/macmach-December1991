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
 * $Log:	asm.h,v $
 * Revision 2.2  90/08/30  11:00:30  bohman
 * 	Created.
 * 	[90/08/29  10:57:19  bohman]
 * 
 */

/*
 *	Apple Macintosh II Mach (macmach)
 *
 *	File: mac2/asm.h
 *	Author: David E. Bohman II (CMU macmach)
 */

#ifndef _MAC2_ASM_H_
#define _MAC2_ASM_H_

#define	ENTRY(name) \
	.globl _##name; .text; .even; _##name:

#define	ENTRY2(name) \
	.globl _##name; _##name:

#endif	_MAC2_ASM_H_
