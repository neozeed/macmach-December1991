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
 * $Log:	setjmp.s,v $
 * Revision 2.2  90/08/30  11:03:00  bohman
 * 	Created.
 * 	[90/08/29  11:45:42  bohman]
 * 
 */

/*
 *	Apple Macintosh II Mach (macmach)
 *
 *	File: mac2/setjmp.s
 *	Author: David E. Bohman II (CMU macmach)
 */

#include <mac2/asm.h>

ENTRY(setjmp)
	movl	sp@(4),a0
	moveml	d2-d7/a2-a7,a0@(4)	| save regs
	movl	sp@,a0@			| save pc
	moveq	#0,d0			| return 0
	rts

ENTRY(longjmp)
	movl	sp@(4),a0
	moveml	a0@(4),d2-d7/a2-a7	| restore regs
	movl	a0@,sp@			| restore pc
	moveq	#1,d0			| return non-zero
	rts
