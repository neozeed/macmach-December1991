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
 * $Log:	endian.h,v $
 * Revision 2.2  91/09/12  16:40:08  bohman
 * 	Created Mach 3.0 version from 2.5 version, deleting extraneous stuff.
 * 	[91/09/11  14:40:17  bohman]
 * 
 * Revision 2.2  90/08/30  11:01:12  bohman
 * 	Created.
 * 	[90/08/29  11:33:02  bohman]
 * 
 * 16-Jan-89  Zon Williamson (zon) at Carnegie-Mellon University
 *      Created this version for mac2 BSD/TAHOE compatability.
 *	This should be included by "machparam.h".
 */

/*
 *	Apple Macintosh II Mach (macmach)
 *
 *	File: mac2/endian.h
 */

/*
 * Copyright (c) 1987 Regents of the University of California.
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms are permitted
 * provided that the above copyright notice and this paragraph are
 * duplicated in all such forms and that any documentation,
 * advertising materials, and other materials related to such
 * distribution and use acknowledge that the software was developed
 * by the University of California, Berkeley.  The name of the
 * University may not be used to endorse or promote products derived
 * from this software without specific prior written permission.
 * THIS SOFTWARE IS PROVIDED ``AS IS'' AND WITHOUT ANY EXPRESS OR
 * IMPLIED WARRANTIES, INCLUDING, WITHOUT LIMITATION, THE IMPLIED
 * WARRANTIES OF MERCHANTIBILITY AND FITNESS FOR A PARTICULAR PURPOSE.
 *
 *	@(#)endian.h	7.3 (Berkeley) 6/29/88
 */ 

#ifndef _MAC2_ENDIAN_H_
#define _MAC2_ENDIAN_H_

/*
 * Definitions for byte order,
 * according to byte significance from low address to high.
 */
#undef LITTLE_ENDIAN
#undef BIG_ENDIAN
#undef PDP_ENDIAN
#define	LITTLE_ENDIAN	1234	/* least-significant byte first */
#define	BIG_ENDIAN	4321	/* most-significant byte first (IBM, net) */
#define	PDP_ENDIAN	3412	/* LSB first in word, MSW first in long (pdp) */

#define	BYTE_ORDER	BIG_ENDIAN	/* byte order on mac2 */

/*
 * Macros for network/external number representation conversion.
 */
#if BYTE_ORDER == BIG_ENDIAN && !defined(lint)
#define	ntohl(x)	(x)
#define	ntohs(x)	(x)
#define	htonl(x)	(x)
#define	htons(x)	(x)
#else
unsigned short	ntohs(), htons();
unsigned long	ntohl(), htonl();
#endif

#endif	_MAC2_ENDIAN_H_
