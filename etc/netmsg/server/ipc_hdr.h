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
 * $Log:	ipc_hdr.h,v $
 * Revision 2.1  90/10/27  20:44:11  dpj
 * Moving under MACH3 source control
 * 
 * Revision 1.7  89/05/02  11:09:16  dpj
 * 	Fixed all files to conform to standard copyright/log format
 * 
 * 28-Jan-87  Robert Sansom (rds) at Carnegie Mellon University
 *	Added ipc_seq_no.
 *
 * 12-Dec-86  Daniel Julin (dpj) at Carnegie-Mellon University
 *	Added IPC_INFO_RPC.
 *
 * 15-Nov-86  Daniel Julin (dpj) at Carnegie-Mellon University
 *	Created.
 *
 *
 */
/*
 * ipc_hdr.h 
 *
 *
 * $Header: /afs/cs.cmu.edu/project/mach-2/rcs/pkg/netmsg/server/ipc_hdr.h,v 2.1 90/10/27 20:44:11 dpj Exp $ 
 *
 */

/*
 * Definitions for the network headers used by the IPC module. 
 */


#ifndef	_IPC_HDR_
#define	_IPC_HDR_

#include	"disp_hdr.h"
#include	"port_defs.h"

/*
 * Header for network IPC messages 
 */
typedef struct {
	disp_hdr_t      disp_hdr;	/* dispatcher header */
	network_port_t  local_port;
	network_port_t  remote_port;
	unsigned long   info;		/* info bits */
	unsigned long   npd_size;	/* size of Network Port Dictionary */
	unsigned long   inline_size;	/* size of inline part of message */
	unsigned long   ool_size;	/* size of ool part of message */
	unsigned long   ool_num;	/* number of ool sections (for assembly) */
	unsigned long	ipc_seq_no;	/* the IPC sequence number of this message */
}               ipc_netmsg_hdr_t;

/*
 * Bits for info field 
 */
#define IPC_INFO_SIMPLE		0x1
#define	IPC_INFO_RPC		0x2

#endif	_IPC_HDR_
