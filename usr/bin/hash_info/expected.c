/* 
 * Mach Operating System
 * Copyright (c) 1991,1990 Carnegie Mellon University
 * All Rights Reserved.
 * 
 * Permission to use, copy, modify and distribute this software and its
 * documentation is hereby granted, provided that both the copyright
 * notice and this permission notice appear in all copies of the
 * software, derivative works or modified versions, and any portions
 * thereof, and that both notices appear in supporting documentation.
 * 
 * CARNEGIE MELLON ALLOWS FREE USE OF THIS SOFTWARE IN ITS 
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
 * any improvements or extensions that they make and grant Carnegie the
 * rights to redistribute these changes.
 */
/*
 * HISTORY
 * $Log:	expected.c,v $
 * Revision 2.3  91/03/19  11:33:27  mrt
 * 	Changed to new copyright
 * 
 * Revision 2.2  91/01/08  14:57:16  rpd
 * 	Created.
 * 	[91/01/08            rpd]
 * 
 */

#include <math.h>

/*
 *	If a hash table with a perfectly random (uniform) hash function
 *	has N buckets and K keys are hashed into it, how many buckets
 *	can we expect to contain k keys?
 *
 *	Ans:	N * (K choose k) * (1 / N) ^ k * (N-1 / N) ^ (K - k)
 *
 *	Taking logs and simplifying,
 *
 *	log(K!) - log(k!) - log((K-k)!) + (K-k) log(N-1) - (K-1) log(N)
 *
 *	Note log(X!) = lgamma(X + 1.0)
 */

double
expected(N, K, k)
    double N, K, k;
{
    return exp(lgamma(K + 1.0) - lgamma(k + 1.0) - lgamma(K - k + 1.0) +
	       (K - k) * log(N - 1.0) - (K - 1.0) * log(N));
}
