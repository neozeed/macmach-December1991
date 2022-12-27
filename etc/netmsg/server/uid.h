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
 * $Log:	uid.h,v $
 * Revision 2.1  90/10/27  20:47:00  dpj
 * Moving under MACH3 source control
 * 
 * Revision 1.5  89/05/02  11:19:10  dpj
 * 	Fixed all files to conform to standard copyright/log format
 * 
 */
/*
 * uid.h
 *
 * $Header: /afs/cs.cmu.edu/project/mach-2/rcs/pkg/netmsg/server/uid.h,v 2.1 90/10/27 20:47:00 dpj Exp $
 *
 */
/*
 * Public definitions for generator of locally unique identifiers.
 */

#ifndef	_UID_
#define	_UID_

#include <mach/boolean.h>

extern boolean_t uid_init();
/*
*/

extern long uid_get_new_uid();
/*
*/

#endif	_UID_
