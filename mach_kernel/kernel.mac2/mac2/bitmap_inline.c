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
 * $Log:	bitmap_inline.c,v $
 * Revision 2.2  91/09/12  16:38:51  bohman
 * 	Created Mach 3.0 version from 2.5 version.
 * 	[91/09/11  14:22:10  bohman]
 * 
 * Revision 2.2  90/08/30  11:00:44  bohman
 * 	Created.
 * 	[90/08/29  11:28:26  bohman]
 * 
 */

/*
 *	Apple Macintosh II Mach (macmach)
 *
 *	File: mac2/bitmap_inline.c
 *	Author: David E. Bohman II (CMU macmach)
 */

#ifndef	_MACHINE_BITMAP_INLINE_C_
#define	_MACHINE_BITMAP_INLINE_C_

/*
 * Character bitmap allocation routines (inline expansions).
 *
 * The bit offsets are numbered as follows:
 *
 *	7 6 5 4 3 2 1 0
 *	M	      L
 *	S	      S
 *	B	      B
 */

/*
 * Initialize a bitmap.
 */
static inline
void
init_bitmap_byte(bp)
unsigned char *bp;
{
    asm("st	%0@" : : "a" (bp));
}

/*
 * Allocate an element in a bitmap.
 * returns -1 if no free elements,
 * otherwise the bit offset.
 */
static inline
int
alloc_bitmap_byte(bp)
unsigned char *bp;
{
    int bit;

    asm("bfffo	%1@{#0:#8},%0" : "=d" (bit) : "a" (bp));

    if (bit == 8)
	return (-1);

    asm("bfclr	%0@{%1:#1}" : : "a" (bp), "d" (bit));

    return (7 - bit);
}

/*
 * Free an element in a bitmap.
 * NB: the element number is
 * not checked for range errors.
 */
static inline
int
free_bitmap_byte(bp, bit)
unsigned char *bp;
int bit;
{
    bit = 7 - bit;

    asm("bfset	%0@{%1:#1}" : : "a" (bp), "d" (bit));

    return (0);
}

#endif	_MACHINE_BITMAP_INLINE_C_
