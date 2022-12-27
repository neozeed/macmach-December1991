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
 * $Log:	crypt_des.c,v $
 * Revision 2.1  90/10/27  20:43:48  dpj
 * Moving under MACH3 source control
 * 
 * Revision 1.6  89/05/02  11:07:05  dpj
 * 	Fixed all files to conform to standard copyright/log format
 * 
 * Revision 1.5  89/04/24  20:37:48  dpj
 * 	Changes from NeXT as of 88/09/30
 * 	[89/04/19  17:50:51  dpj]
 * 
 * 21-Jul-87  Robert Sansom (rds) at Carnegie Mellon University
 *	Started.
 *
 */
/*
 * crypt_des.c
 *
 *
 */

#ifndef	lint
char crypt_des_rcsid[] = "$Header: /afs/cs.cmu.edu/project/mach-2/rcs/pkg/netmsg/server/crypt_des.c,v 2.1 90/10/27 20:43:48 dpj Exp $";
#endif not lint

/*
 * Data encryption and decryption using software DES encryption algorithm
 *	and cipher block chaining.
 *
 */



#include <config.h>

#if	USE_DES

#include <mach/mach_types.h>

#include <crypt_defs.h>
#include <key_defs.h>



/*
 * encrypt_newdes
 *	encrypt data using the DES algorithm with cipher block chaining.
 *
 * Parameters:
 *	key		: the key to use for encryption.
 *	data_ptr	: pointer to data to be encrypted.
 *	data_size	: number of bytes to be encrypted.
 *
 * Note:
 *	The data_size should be a multiple of eight bytes.
 *	The key should be a correct DES key.
 *
 */
void encrypt_des(key, data_ptr, data_size)
key_t		key;
pointer_t	data_ptr;
int		data_size;
{
    int			blocks, i;
    char		ekey[8], iv[8];
    register char	*block_ptr;

#if	1
    for (i = 0; i < 8; i++) ekey[i] = key.key_bytes[i];
    setkey(ekey);
#endif

    blocks = data_size / 8;
    block_ptr = (char *)data_ptr;

    /*
     * Set IV to be the last 8 bytes of the key.
     */
    for (i = 8; i < 16; i++) iv[i-8] = key.key_bytes[i];

    while (blocks--) {
	/*
	 * XOR in the previous encrypted block from the IV.
	 */
	block_ptr[0] ^= iv[0];
	block_ptr[1] ^= iv[1];
	block_ptr[2] ^= iv[2];
	block_ptr[3] ^= iv[3];
	block_ptr[4] ^= iv[4];
	block_ptr[5] ^= iv[5];
	block_ptr[6] ^= iv[6];
	block_ptr[7] ^= iv[7];

	/*
	 * Encrypt the block.
	 */
	endes(block_ptr);

	/*
	 * Save outgoing cipher text for chaining.
	 */
	iv[0] = block_ptr[0];
	iv[1] = block_ptr[1];
	iv[2] = block_ptr[2];
	iv[3] = block_ptr[3];
	iv[4] = block_ptr[4];
	iv[5] = block_ptr[5];
	iv[6] = block_ptr[6];
	iv[7] = block_ptr[7];

	block_ptr += 8;
    }
}



/*
 * decrypt_des
 *	decrypt data using the DES algorithm and cipher block chaining.
 *
 * Parameters:
 *	key		: the key to use for decryption.
 *	data_ptr	: pointer to the data to be decrypted.
 *	data_size	: the number of bytes to be decrypted.
 *
 * Note:
 *	The data_size should be a multiple of eight bytes.
 *	The key should be a correct DES key.
 *
 */
void decrypt_des(key, data_ptr, data_size)
key_t		key;
pointer_t	data_ptr;
int		data_size;
{
    int			blocks, i, cycle;
    char		dkey[8], iv0[8], iv1[8];
    register char	*block_ptr;
    
#if	1
    for (i = 0; i < 8; i++) dkey[i] = key.key_bytes[i];
    setkey(dkey);
#endif

    block_ptr = (char *)data_ptr;
    blocks = data_size / 8;

    /*
     * Set up the IV for cipher block chaining.
     */
    for (i = 8; i < 16; i++) iv0[i-8] = key.key_bytes[i];

    cycle = 1;
    while (blocks--) {
	/*
	 * Remember the current crypt block for cipher block chaining.
	 */
	if (cycle) {
	    iv1[0] = block_ptr[0];
	    iv1[1] = block_ptr[1];
	    iv1[2] = block_ptr[2];
	    iv1[3] = block_ptr[3];
	    iv1[4] = block_ptr[4];
	    iv1[5] = block_ptr[5];
	    iv1[6] = block_ptr[6];
	    iv1[7] = block_ptr[7];
	}
	else {
	    iv0[0] = block_ptr[0];
	    iv0[1] = block_ptr[1];
	    iv0[2] = block_ptr[2];
	    iv0[3] = block_ptr[3];
	    iv0[4] = block_ptr[4];
	    iv0[5] = block_ptr[5];
	    iv0[6] = block_ptr[6];
	    iv0[7] = block_ptr[7];
	}

	/*
	 * Do the decryption.
	 */
	dedes(block_ptr);

	/*
	 * Or in the chaining block.
	 */
	if (cycle) {
	    block_ptr[0] ^= iv0[0];
	    block_ptr[1] ^= iv0[1];
	    block_ptr[2] ^= iv0[2];
	    block_ptr[3] ^= iv0[3];
	    block_ptr[4] ^= iv0[4];
	    block_ptr[5] ^= iv0[5];
	    block_ptr[6] ^= iv0[6];
	    block_ptr[7] ^= iv0[7];
	    cycle = 0;
	}
	else {
	    block_ptr[0] ^= iv1[0];
	    block_ptr[1] ^= iv1[1];
	    block_ptr[2] ^= iv1[2];
	    block_ptr[3] ^= iv1[3];
	    block_ptr[4] ^= iv1[4];
	    block_ptr[5] ^= iv1[5];
	    block_ptr[6] ^= iv1[6];
	    block_ptr[7] ^= iv1[7];
	    cycle = 1;
	}
	block_ptr += 8;
    }
}

#endif	USE_DES
