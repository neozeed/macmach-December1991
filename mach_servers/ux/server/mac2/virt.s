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
 * $Log:	virt.s,v $
 * Revision 2.2  90/08/30  11:03:38  bohman
 * 	Created.
 * 	[90/08/29  12:01:26  bohman]
 * 
 */

/*
 *	Apple Macintosh II Mach (macmach)
 *
 *	File: mac2/virt.s
 *	Author: David E. Bohman II (CMU macmach)
 */

/*
 * This file contains routines used to
 * copy, zero kernel virtual memory.
 */

#include <mac2/asm.h>

/*
 * bcopy(f, t, n) copies n bytes
 * from address f to address t
 */
ENTRY(bcopy)
	/*
	 * collect arguments
	 */
	movl	sp@(4),a0
	movl	sp@(8),a1
	movl	sp@(12),d1

ENTRY2(copy_mem)
	/*
	 * get count of long
	 * words into d0
	 */
	movl	d1,d0
	lsrl	#2,d0

	/*
	 * check for count == 0
	 */    
	beq	2f
	bra	1f

	/*
	 * loop for moving lower 16 bits
	 * of count
	 */
0:	movl	a0@+,a1@+
1:	dbf	d0,0b

	/*
	 * check upper 16 bits
	 * of count for more to do
	 */    
	swap	d0
	tstw	d0
	beq	2f
	subqw	#1,d0
	swap	d0
	bra	0b

	/*
	 * check for residue bytes
	 */
2:	andl	#3,d1
	bra	1f

	/*
	 * move residue
	 */
0:	movb	a0@+,a1@+
1:	dbf	d1,0b

	rts

/*
 * ovbcopy(f, t, n) copies n bytes
 * from address f to address t even
 * if the regions overlap.
 */
ENTRY(ovbcopy)
	/*
	 * collect arguments
	 */
	movl	sp@(4),a0
	movl	sp@(8),a1
	movl	sp@(12),d1

	/*
	 * get count of long
	 * words into d0
	 */
	movl	d1,d0
	lsrl	#2,d0

	/*
	 * check for direction
	 * of copy
	 */
	cmpl	a0,a1
	bgt	backw

	/*
	 * check for count == 0
	 */    
	beq	2f
	bra	1f

	/*
	 * loop for moving lower 16 bits
	 * of count
	 */
0:	movl	a0@+,a1@+
1:	dbf	d0,0b

	/*
	 * check upper 16 bits
	 * of count for more to do
	 */    
	swap	d0
	tstw	d0
	beq	2f
	subqw	#1,d0
	swap	d0
	bra	0b

	/*
	 * check for residue bytes
	 */
2:	andl	#3,d1
	bra	1f

	/*
	 * move residue
	 */
0:	movb	a0@+,a1@+
1:	dbf	d1,0b

	rts

backw:
	/*
	 * start copy at end
	 */
	addl	d1,a0
	addl	d1,a1

	/*
	 * check for count == 0
	 */    
	beq	2f
	bra	1f

	/*
	 * loop for moving lower 16 bits
	 * of count
	 */
0:	movl	a0@-,a1@-
1:	dbf	d0,0b

	/*
	 * check upper 16 bits
	 * of count for more to do
	 */    
	swap	d0
	tstw	d0
	beq	2f
	subqw	#1,d0
	swap	d0
	bra	0b

	/*
	 * check for residue bytes
	 */
2:	andl	#3,d1
	bra	1f

	/*
	 * move residue
	 */
0:	movb	a0@-,a1@-
1:	dbf	d1,0b

	rts

/*
 * wcopy(f, t, n)
 *
 * wcopy copies 16 bit words padding
 * an extra byte out with 0.  the count
 * argument is specified in bytes.
 */
ENTRY(wcopy)
	/*
	 * collect arguments
	 */
	movl	sp@(4),a0
	movl	sp@(8),a1
	movl	sp@(12),d1

	/*
	 * get count of 16 bit
	 * words into d0
	 */
	movl	d1,d0
	lsrl	#1,d0

	/*
	 * check for count == 0
	 */
	beq	2f
	bra	1f

	/*
	 * loop for copying lower 16 bits
	 * of count
	 */
0:	movw	a0@+,a1@+
1:	dbf	d0,0b

	/*
	 * check upper 16 bits
	 * of count for more to do
	 */
	swap	d0
	tstw	d0
	beq	2f
	subqw	#1,d0
	swap	d0
	bra	0b

	/*
	 * check for residue byte
	 */
2:	btst	#0,d1
	beq	2f
	movw	a0@,d0
	bfclr	d0{#24:#8}
	movw	d0,a1@

2:	rts

/*
 * bzero(b, n) zeros n bytes of
 * memory starting at address b
 */
ENTRY(bzero)
ENTRY2(blkclr)
	/*
	 * collect arguments
	 */
	movl	sp@(4),a0
	movl	sp@(8),d1

ENTRY2(zero_mem)
	/*
	 * get count of long
	 * words into d0
	 */
	movl	d1,d0
	lsrl	#2,d0

	/*
	 * check for count == 0
	 */    
	beq	2f
	bra	1f

	/*
	 * loop for clearing lower 16 bits
	 * of count
	 */
0:	clrl	a0@+
1:	dbf	d0,0b

	/*
	 * check upper 16 bits
	 * of count for more to do
	 */    
	swap	d0
	tstw	d0
	beq	2f
	subqw	#1,d0
	swap	d0
	bra	0b

	/*
	 * check for residue bytes
	 */
2:	andl	#3,d1
	bra	1f

	/*
	 * clear residue
	 */
0:	clrb	a0@+
1:	dbf	d1,0b

	rts
