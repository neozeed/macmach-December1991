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
 * $Log:	ipc.h,v $
 * Revision 2.1  90/10/27  20:44:06  dpj
 * Moving under MACH3 source control
 * 
 * Revision 1.6  89/05/02  11:08:59  dpj
 * 	Fixed all files to conform to standard copyright/log format
 * 
 *  6-Oct-87  Daniel Julin (dpj) at Carnegie-Mellon University
 *	Added macros for NETPORT option and declarations for RPCMOD.
 *
 *  4-Feb-87  Robert Sansom (rds) at Carnegie Mellon University
 *	Added external definitions of ipc_retry, ipc_port_dead and 
 *	ipc_msg_accepted.
 *
 *  2-Jan-87  Robert Sansom (rds) at Carnegie Mellon University
 *	Started.
 *
 */
/*
 * ipc.h
 *
 *
 * $Header: /afs/cs.cmu.edu/project/mach-2/rcs/pkg/netmsg/server/ipc.h,v 2.1 90/10/27 20:44:06 dpj Exp $
 *
 */

/*
 * External definitions for the IPC module.
 */


#ifndef	_IPC_
#define	_IPC_

#include <mach/boolean.h>

extern boolean_t ipc_init();
/*
*/


extern void ipc_msg_accepted();
/*
port_rec_ptr_t	port_rec_ptr;
*/


extern void ipc_port_dead();
/*
port_rec_ptr_t	port_rec_ptr;
*/


#if	RPCMOD
extern void ipc_port_moved();
/*
port_rec_ptr_t	port_rec_ptr;
*/
#endif	RPCMOD


extern ipc_retry();
/*
port_rec_ptr_t	port_rec_ptr;
*/


extern void ipc_freeze();
/*
port_rec_ptr_t	port_rec_ptr;
*/


#if	NETPORT
/*
 * Macros to manipulate the kernel NETPORT (MACH_NP) tables.
 */

#define	ipc_netport_enter(netport,localport,local) {			\
	if (param.conf_netport) {					\
		netport_enter(task_self(),(netport),(localport),(local)); \
	}								\
}

#define	ipc_netport_remove(netport) {					\
	if (param.conf_netport) {					\
		netport_remove(task_self(),(netport));			\
	}								\
}
#endif	NETPORT

#endif	_IPC_
