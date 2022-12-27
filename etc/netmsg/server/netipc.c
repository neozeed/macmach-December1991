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
 * $Log:	netipc.c,v $
 * Revision 2.1  90/10/27  20:44:48  dpj
 * Moving under MACH3 source control
 * 
 * Revision 1.9.2.2  90/10/07  13:51:04  dpj
 * 	Added initialization of mach3 module.
 * 
 * Revision 1.9.2.1  90/08/02  20:22:44  dpj
 * 	Reorganized for new netipc interface and packet format.
 * 	Use debug.netipc for debugging control, instead of a
 * 	compile-time constant.
 * 	Delegate most of the work to specialized modules
 * 	(MACH_NET, SOCKETS, MACH3).
 * 	[90/06/03  17:36:54  dpj]
 * 
 * Revision 1.9  89/05/02  11:12:57  dpj
 * 	Fixed all files to conform to standard copyright/log format
 * 
 * 17-Aug-87  Robert Sansom (rds) at Carnegie Mellon University
 *	Ignore the UDP checksum if a packet is (to be) encrypted.
 *
 * 19-May-87  Robert Sansom (rds) at Carnegie Mellon University
 *	Added include of nm_extra.h.
 *
 * 23-Apr-87  Robert Sansom (rds) at Carnegie Mellon University
 *	Changed netipc_receive to reject any packets from ourself.
 *	Replaced fprintf by LOG/DEBUG macros.
 *
 * 22-Dec-86  Robert Sansom (rds) at Carnegie Mellon University
 *	Started.
 *
 */
/*
 * netipc.c
 *
 *
 */

#ifndef	lint
char netipc_rcsid[] = "$Header: /afs/cs.cmu.edu/project/mach-2/rcs/pkg/netmsg/server/netipc.c,v 2.1 90/10/27 20:44:48 dpj Exp $";
#endif not lint

/*
 * Functions to send and receive packets
 * using the IPC interface to the network.
 */


#include "netmsg.h"
#include "netipc.h"


/*
 * Global pointer for the netipc system to use.
 */
netipc_channel_ptr_t		(*netipc_bind_op)();


#if	USE_NETIPC_MACHNET
extern netipc_channel_ptr_t	netipc_bind_mn();
#endif	USE_NETIPC_MACHNET

#if	USE_NETIPC_SOCKET
extern netipc_channel_ptr_t	netipc_bind_so();
#endif	USE_NETIPC_SOCKET

#if	USE_NETIPC_MACH3
extern netipc_channel_ptr_t	netipc_bind_mach3();
extern boolean_t		netipc_init_mach3();
#endif	USE_NETIPC_MACH3



/*
 * netipc_init
 *	Find an appropriate implementation of netipc operations to use.
 *
 * Parameters: none
 *
 * Results: TRUE on successful initialization.
 *
 */
EXPORT netipc_init()
BEGIN("netipc_init")

	netipc_bind_op = NULL;

#if	USE_NETIPC_MACHNET
	if (netipc_bind_op != NULL) {
		ERROR((msg,
			"netipc_init: more than one netipc system enabled."));
		RETURN(FALSE);
	}
	netipc_bind_op = netipc_bind_mn;
#endif	USE_NETIPC_MACHNET

#if	USE_NETIPC_SOCKET
	if (netipc_bind_op != NULL) {
		ERROR((msg,
			"netipc_init: more than one netipc system enabled."));
		RETURN(FALSE);
	}
	netipc_bind_op = netipc_bind_so;
#endif	USE_NETIPC_SOCKET

#if	USE_NETIPC_MACH3
	if (netipc_bind_op != NULL) {
		ERROR((msg,
			"netipc_init: more than one netipc system enabled."));
		RETURN(FALSE);
	}
	if (! netipc_init_mach3()) {
		ERROR((msg,"netipc_init.netipc_init_mach3 failed."));
		RETURN(FALSE);
	}
	netipc_bind_op = netipc_bind_mach3;
#endif	USE_NETIPC_MACH3

	if (netipc_bind_op == NULL) {
		ERROR((msg,
			"netipc_init: no netipc system enabled."));
		RETURN(FALSE);
	}

	RETURN(TRUE);
END
