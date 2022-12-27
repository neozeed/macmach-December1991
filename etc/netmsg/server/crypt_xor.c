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
 * $Log:	crypt_xor.c,v $
 * Revision 2.1  90/10/27  20:43:51  dpj
 * Moving under MACH3 source control
 * 
 * Revision 1.6  89/05/02  11:07:27  dpj
 * 	Fixed all files to conform to standard copyright/log format
 * 
 * Revision 1.5  89/04/24  20:38:01  dpj
 * 	Changes from NeXT as of 88/09/30
 * 	[89/04/19  17:51:16  dpj]
 * 
 * 09-Sep-88  Avadis Tevanian (avie) at NeXT
 *	Make conditional.
 *
 * 19-Sep-86  Robert Sansom (rds) at Carnegie Mellon University
 *	Started.
 *
 */
/*
 * crypt_xor.c
 *
 *
 */

#ifndef	lint
char crypt_xor_rcsid[] = "$Header: /afs/cs.cmu.edu/project/mach-2/rcs/pkg/netmsg/server/crypt_xor.c,v 2.1 90/10/27 20:43:51 dpj Exp $";
#endif not lint

/*
 * Packet encryption and decryption used XOR (with cipher block chaining).
 */


#include "config.h"

#if	USE_XOR

#include <mach/mach_types.h>

#include "crypt_defs.h"
#include "key_defs.h"


typedef struct {
    unsigned int high;
    unsigned int low;
} message_t, *message_ptr_t;



/*
 * encrypt_xor
 *	Encrypt data using XOR and cipher block chaining.
 *
 * Parameters:
 *	key		: the key to use for encryption.
 *	data_ptr	: pointer to the data to be encrypted.
 *	data_size	: the number of bytes to be encrypted.
 *
 * Note:
 *	The data_size should be a multiple of eight bytes.
 *
 */
void encrypt_xor (key, data_ptr, data_size)
key_t		key;
pointer_t	data_ptr;
int		data_size;
{
    int				keylong0, keylong1, keylong2, keylong3;
    register int		blocks;
    register int		plong0, plong1, plong2, plong3;
    register message_ptr_t	encryptblock;

    blocks  = data_size / 8;
    encryptblock = (message_ptr_t)data_ptr;
    keylong0 = key.key_longs[0];
    keylong1 = key.key_longs[1];
    keylong2 = key.key_longs[2];
    keylong3 = key.key_longs[3];
    /*
     * Set initial value of cipher block text to be the key.
     */
    plong0 = keylong3;
    plong1 = keylong2;
    plong2 = keylong1;
    plong3 = keylong0;

    while (blocks) {
	plong0 = (encryptblock->high ^= keylong0 ^ plong0);
	plong1 = (encryptblock->low ^= keylong1 ^ plong1);
	encryptblock ++; blocks--;
	if (blocks) {
	    plong2 = (encryptblock->high ^= keylong2 ^ plong2);
	    plong3 = (encryptblock->low ^= keylong3 ^ plong3);
	    encryptblock ++; blocks--;
	}
    }
}



/*
 * decrypt_xor
 *	decrypt data using XOR and cipher block chaining.
 *
 * Parameters:
 *	key		: the key to use for decryption.
 *	data_ptr	: pointer to the data to be decrypted.
 *	data_size	: the number of bytes to be decrypted.
 *
 * Note:
 *	The data_size should be a multiple of eight bytes.
 *
 */
void decrypt_xor(key, data_ptr, data_size)
key_t		key;
pointer_t	data_ptr;
int		data_size;
{
    int				keylong0, keylong1, keylong2, keylong3;
    register int		blocks;
    register message_ptr_t	decryptblock;
    register int		plong0, plong1, plong2, plong3;

    blocks = data_size / 8;
    decryptblock = (message_ptr_t)data_ptr;
    keylong0 = key.key_longs[0];
    keylong1 = key.key_longs[1];
    keylong2 = key.key_longs[2];
    keylong3 = key.key_longs[3];
    /*
     * Set initial value of cipher block text to be the key.
     */
    plong0 = keylong3;
    plong1 = keylong2;
    plong2 = keylong1;
    plong3 = keylong0;

    /*
     ************ WARNING *************
     * This code relies on expression *
     * evaluation being left to right *
     **********************************
     */
    while (blocks) {
	register int temp;
	temp = decryptblock->high;
	decryptblock->high = plong0 ^ keylong0 ^ temp;
	plong0 = temp;
	temp = decryptblock->low;
	decryptblock->low = plong1 ^ keylong1 ^ temp;
	plong1 = temp;
/*
	decryptblock->high = plong0 ^ keylong0 ^ (plong0 = decryptblock->high);
	decryptblock->low = plong1 ^ keylong1 ^ (plong1 = decryptblock->low);
*/
	decryptblock ++; blocks--;
	if (blocks) {
	    temp = decryptblock->high;
	    decryptblock->high = plong2 ^ keylong2 ^ temp;
	    plong2 = temp;
	    temp = decryptblock->low;
	    decryptblock->low = plong3 ^ keylong3 ^ temp;
	    plong3 = temp;
/*
	    decryptblock->high = plong2 ^ keylong2 ^ (plong2 = decryptblock->high);
	    decryptblock->low = plong3 ^ keylong3 ^ (plong3 = decryptblock->low);
*/
	    decryptblock ++; blocks--;
	}
    }
}

#endif	USE_XOR
