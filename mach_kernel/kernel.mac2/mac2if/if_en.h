/* 
 * Mach Operating System
 * Copyright (c) 1991,1990,1989 Carnegie Mellon University
 * All Rights Reserved.
 * 
 * Permission to use, copy, modify and distribute this software and its
 * documentation is hereby granted, provided that both the copyright
 * notice and this permission notice appear in all copies of the
 * software, derivative works or modified versions, and any portions
 * thereof, and that both notices appear in supporting documentation.
 * 
 * CARNEGIE MELLON ALLOWS FREE USE OF THIS SOFTWARE IN ITS "AS IS"
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
 * any improvements or extensions that they make and grant Carnegie Mellon
 * the rights to redistribute these changes.
 */


/*
 * HISTORY
 * $Log:	if_en.h,v $
 * Revision 2.2  91/09/12  16:49:29  bohman
 * 	Created.
 * 	[91/09/11  16:18:08  bohman]
 * 
 */

/*
 *	Apple Macintosh II Mach (macmach)
 *
 *	File: mac2if/if_en.h
 */

typedef unsigned char	ether_address_t[6];

typedef struct {
    ether_address_t	dest;
    ether_address_t	src;
    unsigned short	type;
} ether_header_t;

#define EN_ENBL_PROTO		1

typedef unsigned	en_enbl_proto_t;

#define EN_ADD_MULTI		2
#define EN_DEL_MULTI		3

typedef unsigned	en_address_status_t[2];

#define EN_ADDRESS_STATUS_COUNT	2

