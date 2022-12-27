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
 * $Log:	crypt_defs.h,v $
 * Revision 2.1  90/10/27  20:43:48  dpj
 * Moving under MACH3 source control
 * 
 * Revision 1.6  89/05/02  11:07:01  dpj
 * 	Fixed all files to conform to standard copyright/log format
 * 
 * 21-Jul-87  Robert Sansom (rds) at Carnegie Mellon University
 *	Added DES encryption/decryption functions.
 *
 *  8-Jun-87  Robert Sansom (rds) at Carnegie Mellon University
 *	Moved encryption algorithm definitions to crypt.h.
 *
 * 22-Jan-87  Robert Sansom (rds) at Carnegie Mellon University
 *	Started.
 *
 */
/*
 * crypt_defs.h
 *
 *
 * $Header: /afs/cs.cmu.edu/project/mach-2/rcs/pkg/netmsg/server/crypt_defs.h,v 2.1 90/10/27 20:43:48 dpj Exp $
 *
 */

/*
 * Internal definitions for crypt module.
 */


#ifndef	_CRYPT_DEFS_
#define	_CRYPT_DEFS_


/*
 * External definitions of functions.
 */

extern void encrypt_des();
/*
key_t		key;
pointer_t	data_ptr;
int		data_size;
*/

extern void decrypt_des();
/*
key_t		key;
pointer_t	data_ptr;
int		data_size;
*/


extern void encrypt_multperm();
/*
key_t		key;
pointer_t	data_ptr;
int		data_size;
*/

extern void decrypt_multperm();
/*
key_t		key;
pointer_t	data_ptr;
int		data_size;
*/


extern void encrypt_newdes();
/*
key_t		key;
pointer_t	data_ptr;
int		data_size;
*/

extern void decrypt_newdes();
/*
key_t		key;
pointer_t	data_ptr;
int		data_size;
*/


extern void encrypt_xor();
/*
key_t		key;
pointer_t	data_ptr;
int		data_size;
*/

extern void decrypt_xor();
/*
key_t		key;
pointer_t	data_ptr;
int		data_size;
*/

#endif	_CRYPT_DEFS_
