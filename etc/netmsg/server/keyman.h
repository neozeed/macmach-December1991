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
 * $Log:	keyman.h,v $
 * Revision 2.1  90/10/27  20:44:25  dpj
 * Moving under MACH3 source control
 * 
 * Revision 1.5  89/05/02  11:10:41  dpj
 * 	Fixed all files to conform to standard copyright/log format
 * 
 *  5-Jun-87  Robert Sansom (rds) at Carnegie Mellon University
 *	Replaced km_get_ikey by km_get_dkey.
 *
 *  9-Jan-87  Robert Sansom (rds) at Carnegie Mellon University
 *	Started.
 *
 */
/*
 * keyman.h
 *
 *
 * $Header: /afs/cs.cmu.edu/project/mach-2/rcs/pkg/netmsg/server/keyman.h,v 2.1 90/10/27 20:44:25 dpj Exp $
 *
 */

/*
 * External definitions for Key Management module.
 */


#ifndef	_KEYMAN_
#define	_KEYMAN_

#include <mach/boolean.h>

#include "nm_defs.h"

#define KM_SERVICE_NAME	"NETWORK_SERVER_KEY_MANAGER"

/*
 * Error Codes.
 */
#define KM_SUCCESS	0
#define KM_FAILURE	-1

extern boolean_t km_init();
/*
*/

extern boolean_t km_get_key();
/*
netaddr_t	host_id;
key_ptr_t	key_ptr;
*/

extern boolean_t km_get_dkey();
/*
netaddr_t	host_id;
key_ptr_t	ikey_ptr;
*/

extern void km_do_key_exchange();
/*
int		client_id;
int		(*client_retry)();
netaddr_t	host_id;
*/

#endif	_KEYMAN_
