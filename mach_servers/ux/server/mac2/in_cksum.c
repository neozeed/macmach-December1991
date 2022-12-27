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
 * $Log:	in_cksum.c,v $
 * Revision 2.2  90/08/30  11:01:33  bohman
 * 	Created.
 * 	[90/08/29  11:35:48  bohman]
 * 
 */

/*
 *	Apple Macintosh II Mach (macmach)
 *
 *	File: mac2/in_cksum.c
 *	Author: David E. Bohman II (CMU macmach)
 */

/*
 * Copyright (c) 1982, 1986 Regents of the University of California.
 * All rights reserved.  The Berkeley software License Agreement
 * specifies the terms and conditions for redistribution.
 *
 *	@(#)in_cksum.c	7.1 (Berkeley) 6/5/86
 */

#include <sys/types.h>
#include <sys/mbuf.h>
#include <netinet/in.h>
#include <netinet/in_systm.h>

/*
 * Checksum routine for Internet Protocol family headers.
 *
 * This routine is very heavily used in the network
 * code and should be modified for each CPU to be as fast as possible.
 */
unsigned long
in_cksum(m, len)
register struct mbuf *m;
register int len;
{
    register u_short *w;
    register unsigned long sum = 0;
    register int mlen = 0;
    
    for (;;) {
	/*
	 * Each trip around loop adds in
	 * word from one mbuf segment.
	 */
	w = mtod(m, u_short *);
	if (mlen == -1) {
	    /*
	     * There is a byte left from the last segment;
	     * add it into the checksum.  Don't have to worry
	     * about a carry-out here because we make sure
	     * that high part of (32 bit) sum is small below.
	     */
	    sum += *(u_char *)w;
	    w = (u_short *)((char *)w + 1);
	    mlen = m->m_len - 1;
	    len--;
	} else
	    mlen = m->m_len;
	m = m->m_next;
	if (len < mlen)
	    mlen = len;
	len -= mlen;
	/*
	 * Force to long boundary so we do longword aligned
	 * memory operations.  It is too hard to do byte
	 * adjustment, do only word adjustment.
	 */
	if (((int)w&0x2) && mlen >= 2) {
	    sum += *w++;
	    mlen -= 2;
	}
	/*
	 * Do as much of the checksum as possible 32 bits at at time.
	 *
	 * We can do a 16 bit ones complement sum 32 bits at a time
	 * because the 32 bit register is acting as two 16 bit
	 * registers for adding, with carries from the low added
	 * into the high (by normal carry-chaining) and carries
	 * from the high carried into the low on the next word.
	 *
	 * Here there is the danger of high order carry out, and
	 * we carefully add in the 32 bit carry.
	 */
#define ADD \
    asm("addl %1@+,%0; 			\
	 bcc 1f; addql #1,%0; 1:" :	\
	"=d" (sum), "=a" (w) :		\
	"0" (sum), "1" (w))

	while ((mlen -= 4) >= 0)
	    ADD;
	mlen += 4;

	/*
	 * Now eliminate the possibility of carry-out's by
	 * folding back to a 16 bit number (adding high and
	 * low parts together.)  Then mop up trailing words
	 * and maybe an odd byte.
	 */
#define FOLD \
    asm("movl %0,d0;			\
	 lsrl #8,%0; lsrl #8,%0;	\
	 addw d0,%0;			\
	 bcc 1f; addql #1,%0; 1:" :	\
	"=d" (sum) : "0" (sum) : "d0" )

	FOLD;

	while ((mlen -= 2) >= 0)
	    sum += (unsigned long)*w++;

	if (mlen == -1)
	    sum += *(u_char *)w << 8;

	if (len == 0)
	    break;
	/*
	 * Locate the next block with some data.
	 * If there is a word split across a boundary we
	 * will wrap to the top with mlen == -1 and
	 * then add it in.
	 */
	for (;;) {
	    if (m == 0) {
		printf("cksum: out of data\n");
		goto done;
	    }
	    if (m->m_len)
		break;
	    m = m->m_next;
	}
    }
done:
    /*
     * Add together high and low parts of sum
     * and carry, then generate the ones
     * complement to get cksum.
     */
#define COMP_FOLD \
    FOLD;					\
    asm("notw %0" : "=d" (sum) : "0" (sum))

    COMP_FOLD;

    return (sum);
}
