#if defined(LIBC_SCCS) && !defined(lint)
	.asciz "@(#)bcopy.S	MacMach 1/9/91"
#endif /* LIBC_SCCS and not lint */

#include "DEFS.h"

/* taken from sys/mac2/virt.s */
/* bcopy(unsigned char *src, unsigned char *dst, unsigned long length) */

ENTRY(bcopy)
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
