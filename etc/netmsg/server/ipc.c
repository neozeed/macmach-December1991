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
 * $Log:	ipc.c,v $
 * Revision 2.1  90/10/27  20:44:05  dpj
 * Moving under MACH3 source control
 * 
 * Revision 1.9  89/05/02  11:08:54  dpj
 * 	Fixed all files to conform to standard copyright/log format
 * 
 * 26-Mar-88  Daniel Julin (dpj) at Carnegie-Mellon University
 *	Converted to use the new memory management module.
 *
 *  6-Oct-87  Daniel Julin (dpj) at Carnegie-Mellon University
 *	Modified fro RPCMOD.
 *
 *  2-Jan-87  Robert Sansom (rds) at Carnegie Mellon University
 *	Started.
 *
 */
/*
 * ipc.c
 *
 *
 */

#ifndef	lint
char ipc_rcsid[] = "$Header: /afs/cs.cmu.edu/project/mach-2/rcs/pkg/netmsg/server/ipc.c,v 2.1 90/10/27 20:44:05 dpj Exp $";
#endif not lint

/*
 * Main routines of the IPC module.
 */


#include <mach.h>
#include <mach/boolean.h>
#include <mach/message.h>

#include "ipc.h"
#include "ipc_rec.h"
#include "netmsg.h"
#include "ipc_internal.h"

/*
 * Memory management definitions.
 */
PUBLIC mem_objrec_t	MEM_IPCBUFF;
PUBLIC mem_objrec_t	MEM_ASSEMBUFF;
PUBLIC mem_objrec_t	MEM_IPCBLOCK;
#if	RPCMOD
PUBLIC mem_objrec_t	MEM_IPCREC;
PUBLIC mem_objrec_t	MEM_IPCABORT;
#endif	RPCMOD



/*
 * ipc_init
 *
 * Results:
 *	TRUE or FALSE.
 *
 * Design:
 *
 */
EXPORT boolean_t ipc_init()
BEGIN("ipc_init")

	/*
	 * Initialize the memory management facilities.
	 */
	mem_initobj(&MEM_IPCBUFF,"IPC receive buffer",MSG_SIZE_MAX,TRUE,7,1);
	mem_initobj(&MEM_ASSEMBUFF,"IPC assembly buffer",IPC_ASSEM_SIZE,
								TRUE,3,1);
	mem_initobj(&MEM_IPCBLOCK,"IPC block record",sizeof(ipc_block_t),
								FALSE,500,5);
#if	RPCMOD
	mem_initobj(&MEM_IPCREC,"IPC record",sizeof(ipc_rec_t),FALSE,12,2);
	mem_initobj(&MEM_IPCABORT,"IPC abort record",sizeof(ipc_abort_rec_t),
								FALSE,50,2);
#endif	RPCMOD

	/*
	 * Start the IPC module.
	 */
#if	RPCMOD
	RETURN(ipc_rpc_init());
#else 	RPCMOD
    ipc_in_init();

    ipc_out_init();

    RETURN(TRUE);
#endif	RPCMOD

END

