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
 * $Log:	network.h,v $
 * Revision 2.1  90/10/27  20:45:19  dpj
 * Moving under MACH3 source control
 * 
 * Revision 1.6.1.1  90/07/30  14:02:33  dpj
 * 	Added localhost_id.
 * 
 * Revision 1.6  89/05/02  11:13:57  dpj
 * 	Fixed all files to conform to standard copyright/log format
 * 
 * 22-Dec-86  Robert Sansom (rds) at Carnegie Mellon University
 *	Added udp_checksum and last_ip_id definition.
 *
 *  3-Nov-86  Robert Sansom (rds) at Carnegie Mellon University
 *	Started.
 *
 */
/*
 * network.h
 *
 *
 * $Header: /afs/cs.cmu.edu/project/mach-2/rcs/pkg/netmsg/server/network.h,v 2.1 90/10/27 20:45:19 dpj Exp $
 *
 */

/*
 * Random network-level definitions and externs.
 */


#ifndef	_NETWORK_
#define	_NETWORK_

#include <mach/boolean.h>
#include "nm_defs.h"

#define HOST_NAME_SIZE	40
extern char			my_host_name[HOST_NAME_SIZE];
extern netaddr_t		my_host_id;
extern netaddr_t		broadcast_address;
extern netaddr_t		localhost_id;
extern short			last_ip_id;

extern boolean_t network_init();

extern int udp_checksum();

#endif	_NETWORK_
