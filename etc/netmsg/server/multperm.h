/* 
 * Mach Operating System
 * Copyright (c) 1989 Carnegie-Mellon University
 * Copyright (c) 1988 Carnegie-Mellon University
 * Copyright (c) 1987 Carnegie-Mellon University
 * All rights reserved.  The CMU software License Agreement specifies
 * the terms and conditions for use and redistribution.
 */
/*
 * HISTORY
 * $Log:	multperm.h,v $
 * Revision 2.1  90/10/27  20:44:47  dpj
 * Moving under MACH3 source control
 * 
 * Revision 1.4  89/05/02  11:12:52  dpj
 * 	Fixed all files to conform to standard copyright/log format
 * 
 * 13-Jan-87  Robert Sansom (rds) at Carnegie Mellon University
 *	Fixed up for the new network server.
 *
 * 15-Jan-86  Robert Sansom (rds) at Carnegie Mellon University
 *	Started.
 *
 */
/*
 * multperm.h
 *
 *
 * $Header: /afs/cs.cmu.edu/project/mach-2/rcs/pkg/netmsg/server/multperm.h,v 2.1 90/10/27 20:44:47 dpj Exp $
 *
 */

/*
 * External definitions for multperm.c
 */


#ifndef	_MULTPERM_
#define	_MULTPERM_


typedef struct mp_block {
    unsigned int	mp_block_high;
    unsigned int	mp_block_low;
} mp_block_t, *mp_block_ptr_t;


extern unsigned int mult();
/*
unsigned int	a,b;
*/

extern void permute();
/*
unsigned int	*out, *out1, in0, in1;
*/


extern void multcrypt();
/*
key_t		key;
mp_block_ptr_t	text;
*/

extern void multdecrypt();
/*
key_t		key;
mp_block_ptr_t	text;
*/

extern unsigned int minverse();
/*
unsigned int	test_key;
*/

extern void invert_key();
/*
key_t		*key_ptr;
*/

#endif	_MULTPERM_
