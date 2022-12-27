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
 * $Log:	nm_init.c,v $
 * Revision 2.1  90/10/27  20:45:26  dpj
 * Moving under MACH3 source control
 * 
 * Revision 1.22.2.2  90/10/07  13:55:36  dpj
 * 	Allow auto-configuration of default transport module, and
 * 	specification from the NETMSG_CONFIG file.
 * 
 * Revision 1.22.2.1  90/08/02  20:23:21  dpj
 * 	Added initialization for global netmsgserver port set,
 * 	and for the netipc module.
 * 	[90/06/03  17:39:15  dpj]
 * 
 * Revision 1.22  89/05/02  11:14:21  dpj
 * 	Fixed all files to conform to standard copyright/log format
 * 
 * Revision 1.21  89/04/24  20:41:07  dpj
 * 	Changes from NeXT as of 88/09/30
 * 	[89/04/19  17:55:49  dpj]
 * 
 * Revision 1.20  88/10/08  22:29:45  dpj
 * 	Removed enabling of NETPORT here. It happens only via the configuration
 * 	file. Added USE_DELTAT.
 * 
 * 05-Sep-88  Avadis Tevanian (avie) at NeXT
 *	Added USE_DATAGRAM and USE_SRR.
 *
 *  3-Sep-88  Daniel Julin (dpj) at Carnegie-Mellon University
 *	Removed enabling of NETPORT here. It happens only via the configuration
 *	file. Added USE_DELTAT.
 *
 * 29-Mar-88  Daniel Julin (dpj) at Carnegie-Mellon University
 *	Modified logstat initialization so that tracing can be
 *	turned on very early.
 *
 * 26-Mar-88  Daniel Julin (dpj) at Carnegie-Mellon University
 *	Added a call to lock_queue_init(), needed with the new
 *	memory management module.
 *
 * 15-Jan-88  Daniel Julin (dpj) at Carnegie-Mellon University
 *	Made TCP the default transport protocol when USE_TCP is true.
 *	Do not enable netport any time the kernel has the capability,
 *	follow the initial param value instead.
 *
 * 06-Dec-87  Daniel Julin (dpj) at Carnegie-Mellon University
 *	Added initialization for TCP module (USE_TCP).
 *
 *  6-Oct-87  Daniel Julin (dpj) at Carnegie-Mellon University
 *	Added auto-configuration of network, netport and VMTP.
 *
 * 25-Sep-87  Daniel Julin (dpj) at Carnegie-Mellon University
 *	Added call to transport_init().
 *
 *  5-Aug-87  Daniel Julin (dpj) and Robert Sansom (rds) at Carnegie-Mellon University
 *	Added Camelot support.
 *
 * 29-Jul-87  Daniel Julin (dpj) at Carnegie-Mellon University
 *	Changed the netport option to depend on NETPORT instead of
 *	USE_VMTP.
 *
 * 12-Jul-87  Daniel Julin (dpj) at Carnegie-Mellon University
 *	Added initialisation of kernel network ports (if USE_VMTP).
 *
 * 22-Jun-87  Robert Sansom (rds) at Carnegie Mellon University
 *	Made initialisation of VMTP conditional on USE_VMTP.
 *
 * 11-Jun-87  Daniel Julin (dpj) at Carnegie-Mellon University
 *	Added initialization of VMTP.
 *
 * 30-Apr-87  Robert Sansom (rds) at Carnegie Mellon University
 *	Added thread_lock conditional on LOCK_THREADS.
 *
 * 15-Apr-87  Daniel Julin (dpj) at Carnegie Mellon University
 *	Modified for two-phase initialisation of logstat.
 *
 * 15-Apr-87  Daniel Julin (dpj) at Carnegie Mellon University
 *	Added ls_init(). Replaced fprintf with panic().
 *	Reorganized so that the log service is available as soon as possible.
 *
 * 12-Apr-87  Robert Sansom (rds) at Carnegie Mellon University
 *	Added cthread_init().
 *
 * 10-Feb-87  Robert Sansom (rds) at Carnegie Mellon University
 *	Started.
 *
 */
/*
 * nm_init.c
 *
 *
 */

#ifndef	lint
char nm_init_rcsid[] = "$Header: /afs/cs.cmu.edu/project/mach-2/rcs/pkg/netmsg/server/nm_init.c,v 2.1 90/10/27 20:45:26 dpj Exp $";
#endif not lint

/*
 * Initialisation for the network server.
 */


#define NM_INIT_DEBUG	0

#include <cthreads.h>
#include <mach/boolean.h>

#include "datagram.h"
#include "deltat.h"
#include "dispatcher.h"
#include "ipc.h"
#include "keyman.h"
#include "mem.h"
#include "netmsg.h"
#include "network.h"
#include "nm_init.h"
#include "nn.h"
#include "portcheck.h"
#include "portops.h"
#include "portrec.h"
#include "portsearch.h"
#include "srr.h"
#include "timer.h"
#include "uid.h"

#if	LOCK_THREADS
mutex_t		thread_lock;
struct mutex	thread_lock_data;
#endif	LOCK_THREADS



/*
 * nm_init
 *	initialises all the modules of the network server.
 *
 * Returns:
 *	TRUE or FALSE depending on whether initialisation was successful or not.
 *
 */
EXPORT boolean_t nm_init()
{
    boolean_t	success = TRUE;

#if	LOCK_THREADS
    thread_lock = &thread_lock_data;
    mutex_init(thread_lock);
    mutex_set_name(thread_lock, "NM_thread_lock");
    mutex_lock(thread_lock);
#endif	LOCK_THREADS
    /*
     * Initialise the utility modules.
     */
    if (!(mem_init())) {
	panic("mem_init failed.");
	success = FALSE;
    }
    DEBUG_STRING(NM_INIT_DEBUG, 5, 2, "mem_init completed");
    if (!(port_set_init())) {
	panic("port_set_init failed.");
	success = FALSE;
    }
    DEBUG_STRING(NM_INIT_DEBUG, 5, 2, "port_set_init completed");
    if (!(lock_queue_init())) {
	panic("lock_queue_init failed.");
	success = FALSE;
    }
    DEBUG_STRING(NM_INIT_DEBUG, 5, 2, "lock_queue_init completed");
    if (!(uid_init())) {
	panic("uid_init failed.");
	success = FALSE;
    }
    DEBUG_STRING(NM_INIT_DEBUG, 5, 2, "uid_init completed");
    if (!(timer_init())) {
	panic("timer_init failed.");
	success = FALSE;
    }
    DEBUG_STRING(NM_INIT_DEBUG, 5, 2, "timer_init completed");
    if (!(network_init())) {
	ERROR((msg,"Autoconf: network_init failed: no network"));
	param.conf_network = FALSE;
    }
    DEBUG_STRING(NM_INIT_DEBUG, 5, 2, "network_init completed");
    if (!(disp_init())) {
	panic("disp_init failed.");
	success = FALSE;
    }
    DEBUG_STRING(NM_INIT_DEBUG, 5, 2, "disp_init completed");
    if (!(transport_init())) {
	panic("transport_init failed.");
	success = FALSE;
    }
    DEBUG_STRING(NM_INIT_DEBUG, 5, 2, "transport_init completed");
    if (!(pr_init())) {
	panic("pr_init fails.");
	success = FALSE;
    }
    DEBUG_STRING(NM_INIT_DEBUG, 5, 2, "pr_init completed");
    if (!(netipc_init())) {
	panic("netipc_init fails.");
	success = FALSE;
    }
    DEBUG_STRING(NM_INIT_DEBUG, 5, 2, "netipc_init completed");

    if (param.conf_network) {
	    /*
	     * Initialise the transport protocols about which we know.
	     */
#if	USE_DATAGRAM
	    if (!(datagram_init())) {
		panic("datagram_init failed.");
		success = FALSE;
	    }
	    DEBUG_STRING(NM_INIT_DEBUG, 5, 2, "datagram_init completed");
#endif	USE_DATAGRAM
#if	USE_SRR
	    if (!(srr_init())) {
		panic("srr_init failed.");
		success = FALSE;
	    }
	    DEBUG_STRING(NM_INIT_DEBUG, 5, 2, "srr_init completed");
#endif	USE_SRR

	    /*
	     * Start the IPC protocol(s).
	     */

#if	USE_VMTP
#if	RPCMOD
	    if (!(vmtp2_init())) {
		ERROR((msg,"Autoconf: vmtp2_init failed."));
		if (tr_default_entry == TR_VMTP2_ENTRY) {
			tr_default_entry = TR_NOOP_ENTRY;
		}
	    }
	    DEBUG_STRING(NM_INIT_DEBUG, 5, 2, "vmtp2_init completed");
#else	RPCMOD
	    if (!(vmtp1_init())) {
		ERROR((msg,"Autoconf: vmtp1_init failed."));
		if (tr_default_entry == TR_VMTP1_ENTRY) {
			tr_default_entry = TR_NOOP_ENTRY;
		}
	    }
	    DEBUG_STRING(NM_INIT_DEBUG, 5, 2, "vmtp2_init completed");
#endif	RPCMOD
#endif	USE_VMTP

#if	USE_TCP
	    if (!(tcp_init())) {
		ERROR((msg,"Autoconf: tcp_init failed."));
		if (tr_default_entry == TR_TCP_ENTRY) {
			tr_default_entry = TR_NOOP_ENTRY;
		}
	    } else {
		if (tr_default_entry == TR_NOOP_ENTRY) {
			tr_default_entry = TR_TCP_ENTRY;
		}
	    }
	    DEBUG_STRING(NM_INIT_DEBUG, 5, 2, "tcp_init completed");
#endif	USE_TCP

#if	USE_DELTAT
	    if (!(deltat_init())) {
		ERROR((msg,"Autoconf: deltat_init failed."));
		if (tr_default_entry == TR_DELTAT_ENTRY) {
			tr_default_entry = TR_NOOP_ENTRY;
		}
	    } else {
		if (tr_default_entry == TR_NOOP_ENTRY) {
			tr_default_entry = TR_DELTAT_ENTRY;
		}
	    }
	    DEBUG_STRING(NM_INIT_DEBUG, 5, 2, "deltat_init completed");
#endif	USE_DELTAT

	    /*
	     * Make sure there is at least one valid IPC protocol.
	     */
	    if (tr_default_entry == TR_NOOP_ENTRY) {
		    panic("Network enabled, but no valid IPC transport protocol.");
	    }
	    ERROR((msg,"Network server using IPC transport module %d",
							tr_default_entry));

    } else {
	    tr_default_entry = TR_NOOP_ENTRY;
    }


    /*
     * Initialise the higher level modules.
     */
    if (!(netname_init())) {
	panic("netname_init fails.");
	success = FALSE;
    }
    DEBUG_STRING(NM_INIT_DEBUG, 5, 2, "netname_init completed");
    if (!(ipc_init())) {
	panic("ipc_init failed.");
	success = FALSE;
    }
#if	USE_KEYMAN
    if (!(km_init())) {
	panic("km_init failed.");
	success = FALSE;
    }
#endif	USE_KEYMAN
    if (!(pc_init())) {
	panic("pc_init fails.");
	success = FALSE;
    }
    if (!(po_init())) {
	panic("po_init fails.");
	success = FALSE;
    }
    if (!(ps_init())) {
	panic("po_init fails.");
	success = FALSE;
    }
    if (!(ls_init_2())) {
	panic("ls_init_2 failed.");
	success = FALSE;
    }

#if	NETPORT
    if (netport_init(task_self()) != KERN_SUCCESS) {
	ERROR((msg,"Autoconf: netport_init failed."));
	param.conf_netport = FALSE;
    }
#endif	NETPORT

#if	CAMELOT
    if (!cam_stuff_init()) {
	panic("cam_stuff_init failed.");
	success = FALSE;
    }
#endif	CAMELOT

#if	LOCK_THREADS
    mutex_unlock(thread_lock);
#endif	LOCK_THREADS

    return success;

}
