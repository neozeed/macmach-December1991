#if defined(LIBC_SCCS) && !defined(lint)
	.asciz "@(#)bzero.S	MacMach 1/9/91"
#endif /* LIBC_SCCS and not lint */

#include "DEFS.h"

/* taken from sys/mac2/virt.s */
/* bzero(unsigned char *b, unsigned long length) */

ENTRY(bzero)
	/*
	 * collect arguments
	 */
	movl	sp@(4),a0
	movl	sp@(8),d1

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

