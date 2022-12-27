/* Do a 16 bit one's complement sum of a given number of words
 * The word pointer may not be odd
 */
	.globl	_ocsum
_ocsum:	movl	sp@(4), a0	/* get ptr */
	movl	sp@(8), d0	/* get word count */
	movl	d2, sp@-	/* save a reg */
	movl	d0, d2		/* save the count */
	lsrl	#1, d0		/* make d0 longs */
	movl	#0, d1		/* zero accumulator */
	jra	enter		/* into loop */
/* Here we run in 68010 loop mode until we get a carry */
loop:	addl	a0@+, d1	/* add in long */
enter:	dbcs	d0, loop	/* continue until carry or done */
	jcc	exit		/* if no carry, we're done */
	addql	#1, d1		/* add in carry */
	jra	loop		/* and go back to test for carry again */
exit:	btst	#0, d2		/* another short? */
	jeq	ret		/* no, go away */
	movl	#0, d0		/* must do unsigned add */
	movw	a0@, d0		/* get the short */
	addl	d0, d1		/* add it in */
	jcc	ret
	addql	#1, d1
/* Now add in high word to low word as above */
ret:	movl	d1, d0
	lsrl	#8, d0		/* get high word in d0 */
	lsrl	#8, d0		/* and clear high do as well */
	andl	#0xFFFF, d1	/* clear high d1 */
	addw	d1, d0		/* add in just the low word */
	movl	#0, d1		/* doesn't clear x bit */
	addxw	d1, d0		/* add in final carry */
	movl	sp@+, d2	/* restore reg */
	rts			/* all done */
