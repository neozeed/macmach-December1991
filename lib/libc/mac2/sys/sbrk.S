/*
 * Copyright (c) 1983 Regents of the University of California.
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms are permitted
 * provided that the above copyright notice and this paragraph are
 * duplicated in all such forms and that any documentation,
 * advertising materials, and other materials related to such
 * distribution and use acknowledge that the software was developed
 * by the University of California, Berkeley.  The name of the
 * University may not be used to endorse or promote products derived
 * from this software without specific prior written permission.
 * THIS SOFTWARE IS PROVIDED ``AS IS'' AND WITHOUT ANY EXPRESS OR
 * IMPLIED WARRANTIES, INCLUDING, WITHOUT LIMITATION, THE IMPLIED
 * WARRANTIES OF MERCHANTIBILITY AND FITNESS FOR A PARTICULAR PURPOSE.
 **********************************************************************
 * HISTORY
 *
 * 21-Nov-88  Zon Williamson (zon) at Carnegie-Mellon University
 *      Created this version for MacMach.
 *
 **********************************************************************
 */

#if defined(SYSLIBC_SCCS)
	.asciz "@(#)sbrk.s MacMach 5.1 (from Berkeley 5.5)"
	.even
#endif /* SYSLIBC_SCCS and not lint */

#include "SYS.h"

	.globl	curbrk

	.globl	_brk

ENTRY(sbrk) /* char *sbrk(incr) */
	link	a6,#-4		/* space to save curbrk */
	moveq	#~3,d1		/* alignment mask ...111100 */
	movl	curbrk,d0
	addql	#3,d0		/* align curbrk */
	andl	d1,d0
	movl	d0,a6@(-4)	/* save aligned curbrk */
	movl	a6@(8),d0
	addql	#3,d0		/* align incr */
	andl	d1,d0
	addl	a6@(-4),d0	/* add aligned curbrk and incr */
	movl	d0,sp@-
	jbsr	_brk		/* brk(align(curbrk)+align(incr)) */
	moveq	#-1,d1
	cmpl	d0,d1
	jeq	err		/* did brk() fail? */
	movl	a6@(-4),d0	/* return saved curbrk */
err:	unlk	a6
	rts
