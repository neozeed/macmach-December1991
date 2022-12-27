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
 * $Log:	crypt.h,v $
 * Revision 2.1  90/10/27  20:43:47  dpj
 * Moving under MACH3 source control
 * 
 * Revision 1.11.2.1  90/08/02  20:21:12  dpj
 * 	Added netipc_crypt_pkt_ptr_t and CRYPT_NEED_CKSUM()
 * 	for new netipc interface and packet formats.
 * 	[90/06/03  17:27:48  dpj]
 * 
 * Revision 1.11  89/05/02  11:06:55  dpj
 * 	Fixed all files to conform to standard copyright/log format
 * 
 *  8-Jun-87  Robert Sansom (rds) at Carnegie Mellon University
 *	Added crypt_functions - array of encryption/decryption functions.
 *	Moved encryption algorithm definitions to here from crypt_defs.h.
 *
 * 18-May-87  Robert Sansom (rds) at Carnegie Mellon University
 *	Made fields in crypt header unsigned quantities.
 *	crypt_algorithm is now obtained from the param record.
 *
 * 22-Jan-87  Robert Sansom (rds) at Carnegie Mellon University
 *	Added CRYPT_ENCRYPT and CRYPT_CHECKSUM_FAILURE.
 *
 * 22-Dec-86  Robert Sansom (rds) at Carnegie Mellon University
 *	Added ch_data_size to crypt_header_t: this is the actual amount
 *	of data following the crypt header, the size in the UDP header
 *	will be rounded to multiples of 8 bytes if the packet is encrypted.
 *
 * 13-Nov-86  Robert Sansom (rds) at Carnegie Mellon University
 *	Started.
 *
 */
/*
 * crypt.h
 *
 *
 * $Header: /afs/cs.cmu.edu/project/mach-2/rcs/pkg/netmsg/server/crypt.h,v 2.1 90/10/27 20:43:47 dpj Exp $
 *
 */

/*
 * External definitions for network server link-level encryption.
 */


#ifndef	_CRYPT_
#define	_CRYPT_

#include	"netipc.h"

/*
 * Security header for packets.
 */
typedef struct crypt_header {
    unsigned long	ch_crypt_level;	/* This must not be encrypted. */
    unsigned short	ch_checksum;	/* Everything hereon can be encrypted. */
    unsigned short	ch_data_size;
} crypt_header_t, *crypt_header_ptr_t;

#define CRYPT_HEADER_SIZE	(sizeof(short) + sizeof(short) + sizeof(long))

/*
 * Encryption levels for data sent over the network.
 */
#define CRYPT_DONT_ENCRYPT		0
#define CRYPT_ENCRYPT			1

/*
 * Encryption return codes.
 */
#define CRYPT_SUCCESS			0
#define CRYPT_FAILURE			-1
#define CRYPT_REMOTE_FAILURE		-2
#define CRYPT_CHECKSUM_FAILURE		-3

/*
 * Encryption algorithms.
 */
#define CRYPT_NULL	0
#define	CRYPT_XOR	1
#define	CRYPT_NEWDES	2
#define	CRYPT_MULTPERM	3
#define CRYPT_DES	4
#define CRYPT_MAX	4

typedef struct {
    int		(*encrypt)();
    int		(*decrypt)();
} crypt_function_t;

extern crypt_function_t		crypt_functions[];

#define CHECK_ENCRYPT_ALGORITHM(algorithm)				\
	(((algorithm) >= CRYPT_NULL) && ((algorithm) <= CRYPT_MAX)	\
		&& (crypt_functions[(algorithm)].encrypt))

#define CHECK_DECRYPT_ALGORITHM(algorithm)				\
	(((algorithm) >= CRYPT_NULL) && ((algorithm) <= CRYPT_MAX)	\
		&& (crypt_functions[(algorithm)].decrypt))

/*
 * Generic packet format.
 */
typedef struct {
	netipc_header_t		netipc_header;
	crypt_header_t		crypt_header;
	char			data[1];
} *netipc_crypt_pkt_ptr_t;


/*
 * Macro to determine if a packet needs to be checksummed or not.
 */
#define	CRYPT_NEED_CKSUM(pkt_ptr)					\
	(ntohl(((netipc_crypt_pkt_ptr_t)(pkt_ptr))->			\
		crypt_header.ch_crypt_level) == CRYPT_DONT_ENCRYPT)

#endif	_CRYPT_
