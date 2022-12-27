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
 * $Log:	transport.h,v $
 * Revision 2.1  90/10/27  20:46:54  dpj
 * Moving under MACH3 source control
 * 
 * Revision 1.13  89/05/02  11:18:53  dpj
 * 	Fixed all files to conform to standard copyright/log format
 * 
 * 26-Mar-88  Daniel Julin (dpj) at Carnegie-Mellon University
 *	Converted to use the new memory management module.
 *
 *  3-Dec-87  Daniel Julin (dpj) at Carnegie-Mellon University
 *	Added TR_TCP_ENTRY. Fixed TR return codes.
 *
 *  1-Dec-87  Daniel Julin (dpj) at Carnegie-Mellon University
 *	Added TR_OVERLOAD.
 *
 *  2-Oct-87  Daniel Julin (dpj) at Carnegie-Mellon University
 *	Added a no-op entry for operation with no network.
 *	Defined tr_default_entry.
 *
 * 25-Sep-87  Daniel Julin (dpj) at Carnegie-Mellon University
 *	Defined TR_MAX_ENTRY.
 *
 *  3-Sep-87  Daniel Julin (dpj) at Carnegie-Mellon University
 *	Added TR_VMTP2_ENTRY. Modified for RPCMOD. Fixed the TR
 *	return codes.
 *
 *  9-Jun-87  Daniel Julin (dpj) at Carnegie-Mellon University
 *	Added TR_VMTP1_ENTRY.
 *
 * 14-Dec-86  Daniel Julin (dpj) at Carnegie-Mellon University
 *	Defined TRSERV_RPC, removed abort entry point for transport
 *	modules.
 *
 *  2-Nov-86  Daniel Julin (dpj) at Carnegie-Mellon University
 *	Started.
 *
 */
/*
 * transport.h
 *
 *
 * $Header: /afs/cs.cmu.edu/project/mach-2/rcs/pkg/netmsg/server/transport.h,v 2.1 90/10/27 20:46:54 dpj Exp $
 *
 */

/*
 * Generic interface to the transport modules. 
 */


#ifndef	_TRANSPORT_
#define	_TRANSPORT_

#include "mem.h"

/*
 * Type of transport service required by a client.
 */
#define	TRSERV_NORMAL		1
#define	TRSERV_URGENT		2
#define TRSERV_IPC		3
#define	TRSERV_RPC		4

/*
 * Defined entry points to transport protocols.
 */
#define	TR_NOOP_ENTRY		0
#define TR_DELTAT_ENTRY		1
#define TR_VMTP_ENTRY		2
#define TR_DATAGRAM_ENTRY	3
#define TR_SRR_ENTRY		4
#define	TR_VMTP1_ENTRY		5
#define	TR_VMTP2_ENTRY		6
#define	TR_TCP_ENTRY		7
#define	TR_MAX_ENTRY		(TR_TCP_ENTRY + 1)

/*
 * Generic codes returned by a transport module. These codes are in the same
 * space as the DISP_* and IPC_* codes.
 */
#define	TR_SUCCESS		-10
#define	TR_FAILURE		-11
#define TR_REMOTE_ACCEPT	-12
#define	TR_REMOTE_REJECT	-13
#define TR_CRYPT_FAILURE	-14
#define TR_SEND_FAILURE		-15
#define	TR_OVERLOAD		-16


/*
 * Entry points to transport level protocols.
 */
typedef struct {
    int		(*send)();
#if	RPCMOD
    int		(*sendrequest)();
    int		(*sendreply)();
#endif	RPCMOD
} transport_sw_entry_t;

extern transport_sw_entry_t transport_switch[TR_MAX_ENTRY];

extern int	transport_no_function();

#define	tr_default_entry 	param.transport_default

/*
 * UDP Ports used by the transport level protocols.
 */
#define DELTAT_UDP_PORT		7654
#define VMTP_UDP_PORT		7655
#define DATAGRAM_UDP_PORT	7656
#define SRR_UDP_PORT		7657

#if	RPCMOD
/*
 * Macros to call transport modules.
 */
#define	transport_sendrequest(trmod,clid,data,to,crypt,reply)			\
	(transport_switch[(trmod)].sendrequest((clid),(data),(to),(crypt),(reply)))

#define	transport_sendreply(trmod,trid,code,data,crypt)				\
	(transport_switch[(trmod)].sendreply((trid),(code),(data),(crypt)))
#endif	RPCMOD

/*
 * Memory management for MEM_TRBUFF.
 */
extern mem_objrec_t		MEM_TRBUFF;


#endif	_TRANSPORT_
