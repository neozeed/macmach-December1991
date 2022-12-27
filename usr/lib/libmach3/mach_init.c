/* 
 * Mach Operating System
 * Copyright (c) 1987 Carnegie-Mellon University
 * All rights reserved.  The CMU software License Agreement specifies
 * the terms and conditions for use and redistribution.
 */
/****************************************************************
 * HISTORY
 * $Log:	mach_init.c,v $
 * Revision 2.2  90/07/26  12:37:40  dpj
 * 	First version
 * 	[90/07/24  14:29:11  dpj]
 * 
 * Revision 2.2  90/06/02  15:12:28  rpd
 * 	Converted to new IPC.
 * 	[90/03/26  23:26:00  rpd]
 * 
 * Revision 2.1  89/08/03  17:05:42  rwd
 * Created.
 * 
 * 18-Jan-89  David Golub (dbg) at Carnegie-Mellon University
 *	Altered for stand-alone use:
 *	. Removed registered port list.
 *	. Removed task_data (obsolete), task_notify.
 *	. Removed Accent compatibility calls.
 *
 * 23-Nov-87  Mary Thompson (mrt) at Carnegie Mellon
 *	removed includes of <servers/msgn.h> and <servers/netname.h>
 *	as they are no longer used.
 *
 * 5-Oct-87   Mary Thompson (mrt) at Carnegie Mellon
 *	Added an extern void definition of mig_init to keep
 *	lint happy
 *
 * 30-Jul-87  Mary Thompson (mrt) at Carnegie Mellon
 *	Changed the intialization of the mig_reply_port to be
 *	a call to mig_init instead init_mach.
 *
 * 27-May-87  Richard Draves (rpd) at Carnegie-Mellon University
 *	Changed initialization of mach interface, because the
 *	new mig doesn't export an alloc_port_mach function.
 *
 * 15-May-87  Mary Thompson
 *	Removed include of sys/features.h and conditional
 *	compliations
 *
 *  4-May-87  Mary Thompson
 *	vm_deallocted the init_ports array so that brk might 
 *	have a chance to be correct.
 *
 * 24-Apr-87  Mary Thompson
 *	changed type of mach_init_ports_count to unsigned int
 *
 * 12-Nov-86	Mary Thompson
 *	Added initialization call to init_netname the new
 *	interface for the net name server.
 *
 *  7-Nov-86  Michael Young
 * 	Add "service_port" 
 *
 *  7-Sep-86  Michael Young (mwyoung) at Carnegie-Mellon University
 *	Added initialization for MACH_IPC, using mach_ports_lookup.
 *
 * 29-Aug-86  Michael Young (mwyoung) at Carnegie-Mellon University
 *	Added other interesting global ports.
 */

#define	MACH_INIT_SLOTS		1
#include "mach_init.h"
#include <mach/mach.h>

extern void mig_init();

#if	!defined(MACH_IPC_COMPAT) || MACH_IPC_COMPAT

port_t		task_self_ = PORT_NULL;
port_t		thread_reply_ =  PORT_NULL;

#endif	!defined(MACH_IPC_COMPAT) || MACH_IPC_COMPAT

#if	MACH3 && (! defined(MACH3_UNIX))

#define	MACH_INIT_SLOTS		1

port_t		name_server_port = PORT_NULL;
port_t		environment_port = PORT_NULL;
port_t		service_port = PORT_NULL;


port_array_t	mach_init_ports;
unsigned int	mach_init_ports_count;

#endif	MACH3 && (! defined(MACH3_UNIX))

mach_port_t	mach_task_self_ = MACH_PORT_NULL;

vm_size_t	vm_page_size;

int		mach_init()
{
	vm_statistics_data_t	vm_stat;
	kern_return_t		kr;

#undef	mach_task_self

	/*
	 *	Get the important ports into the cached values,
	 *	as required by "mach_init.h".
	 */
	 
	mach_task_self_ = mach_task_self();

#if	!defined(MACH_IPC_COMPAT) || MACH_IPC_COMPAT
	task_self_ = mach_task_self();
#undef	thread_reply
	thread_reply_ = thread_reply();
#endif	!defined(MACH_IPC_COMPAT) || MACH_IPC_COMPAT

	/*
	 *	Initialize the single mig reply port
	 */

	mig_init(0);

	/*
	 *	Cache some other valuable system constants
	 */

	(void) vm_statistics(mach_task_self_, &vm_stat);
	vm_page_size = vm_stat.pagesize;

#if	MACH3 && (! defined(MACH3_UNIX))
	/*
	 * Get a few other ports from the service server.
	 */
	if (mach_ports_lookup(mach_task_self(), &mach_init_ports, &mach_init_ports_count) != KERN_SUCCESS)
		mach_init_ports_count = 0;

	name_server_port = mach_init_ports_count > (unsigned int)NAME_SERVER_SLOT 
			? mach_init_ports[NAME_SERVER_SLOT] : PORT_NULL;
	environment_port = mach_init_ports_count > ENVIRONMENT_SLOT ? mach_init_ports[ENVIRONMENT_SLOT] : PORT_NULL;
	service_port     = mach_init_ports_count > SERVICE_SLOT ? mach_init_ports[SERVICE_SLOT] : PORT_NULL;

	/* get rid of out-of-line data so brk has a chance of working */
	(void) vm_deallocate(mach_task_self(),(vm_address_t)mach_init_ports,
				vm_page_size);
#endif	MACH3 && (! defined(MACH3_UNIX))

	return(0);
}

int		(*mach_init_routine)() = mach_init;
