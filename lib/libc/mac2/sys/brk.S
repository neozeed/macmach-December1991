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
 */

#if defined(SYSLIBC_SCCS) && !defined(lint)
	.asciz "@(#)brk.s MacMach 5.1 (from Berkeley 5.5)"
#endif /* SYSLIBC_SCCS and not lint */

#include "SYS.h"

#define	SYS_brk	17

	.globl	_end
	.globl	curbrk
	.globl  minbrk

	.data

minbrk:	.long	_end		/* won't brk() below this */
curbrk:	.long	_end		/* current brk() address */

	.text

	.globl cerror

err:	jmp cerror

ENTRY(_brk) /* char *_brk(addr) */
	bra	ok		/* skip minbrk test */
ENTRY2(brk) /* char *brk(addr) */
	movl	minbrk,d0
	cmpl	sp@(4),d0	/* check address */
	ble	ok
	movl	minbrk,sp@(4)	/* enforce minimum */
ok:	movl	sp@(4),d0	/* get new address */
	addql	#3,d0
	moveq	#~3,d1
	andl	d1,d0		/* long align */
	movl	d0,sp@(4)
	pea	SYS_brk		/* do it... */
	trap	#0
	jcs	err
	movl	sp@(4),curbrk	/* remember new address */
	movl	sp@(4),d0	/* return new address */
	rts
