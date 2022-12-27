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
 * $Log:	portsearch.h,v $
 * Revision 2.1  90/10/27  20:45:57  dpj
 * Moving under MACH3 source control
 * 
 * Revision 1.6  89/05/02  11:16:40  dpj
 * 	Fixed all files to conform to standard copyright/log format
 * 
 * 18-Dec-86  Robert Sansom (rds) at Carnegie Mellon University
 *	Started.
 *
 */
/*
 * portsearch.h
 *
 *
 * $Header: /afs/cs.cmu.edu/project/mach-2/rcs/pkg/netmsg/server/portsearch.h,v 2.1 90/10/27 20:45:57 dpj Exp $
 *
 */

/*
 * External definitions for the port search module
 */


#ifndef	_PORTSEARCH_
#define	_PORTSEARCH_

#include <mach/boolean.h>


extern void ps_do_port_search();
/*
port_rec_ptr_t		port_rec_ptr;
boolean_t		new_information;
network_port_ptr_t	new_nport_ptr;
int			(*retry)();
*/

extern boolean_t ps_init();
/*
*/

#endif	_PORTSEARCH_
