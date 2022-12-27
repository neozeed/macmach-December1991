/* 
 * Mach Operating System
 * Copyright (c) 1991,1990,1989 Carnegie Mellon University
 * All Rights Reserved.
 * 
 * Permission to use, copy, modify and distribute this software and its
 * documentation is hereby granted, provided that both the copyright
 * notice and this permission notice appear in all copies of the
 * software, derivative works or modified versions, and any portions
 * thereof, and that both notices appear in supporting documentation.
 * 
 * CARNEGIE MELLON ALLOWS FREE USE OF THIS SOFTWARE IN ITS "AS IS"
 * CONDITION.  CARNEGIE MELLON DISCLAIMS ANY LIABILITY OF ANY KIND FOR
 * ANY DAMAGES WHATSOEVER RESULTING FROM THE USE OF THIS SOFTWARE.
 * 
 * Carnegie Mellon requests users of this software to return to
 * 
 *  Software Distribution Coordinator  or  Software.Distribution@CS.CMU.EDU
 *  School of Computer Science
 *  Carnegie Mellon University
 *  Pittsburgh PA 15213-3890
 * 
 * any improvements or extensions that they make and grant Carnegie Mellon
 * the rights to redistribute these changes.
 */
/*
 * HISTORY
 * $Log:	ipc_kobject.c,v $
 * Revision 2.9  91/08/01  14:36:18  dbg
 * 	Call machine-dependent interface routine, under
 * 	MACH_MACHINE_ROUTINES.
 * 	[91/08/01            dbg]
 * 
 * Revision 2.8  91/06/17  15:47:02  jsb
 * 	Renamed NORMA conditionals. Added NORMA_VM support.
 * 	[91/06/17  13:46:55  jsb]
 * 
 * Revision 2.7  91/06/06  17:07:05  jsb
 * 	Added NORMA_TASK support.
 * 	[91/05/14  09:05:48  jsb]
 * 
 * Revision 2.6  91/05/18  14:31:42  rpd
 * 	Added check_simple_locks.
 * 	[91/04/01            rpd]
 * 
 * Revision 2.5  91/05/14  16:42:00  mrt
 * 	Correcting copyright
 * 
 * Revision 2.4  91/03/16  14:50:02  rpd
 * 	Replaced ith_saved with ikm_cache.
 * 	[91/02/16            rpd]
 * 
 * Revision 2.3  91/02/05  17:26:37  mrt
 * 	Changed to new Mach copyright
 * 	[91/02/01  16:12:51  mrt]
 * 
 * Revision 2.2  90/06/02  14:54:08  rpd
 * 	Created for new IPC.
 * 	[90/03/26  23:46:53  rpd]
 * 
 */
/*
 *	File:	kern/ipc_kobject.c
 *	Author:	Rich Draves
 *	Date:	1989
 *
 *	Functions for letting a port represent a kernel object.
 */

#include <mach_debug.h>
#include <mach_ipc_test.h>
#include <mach_machine_routines.h>
#include <norma_task.h>
#include <norma_vm.h>

#include <mach/port.h>
#include <mach/kern_return.h>
#include <mach/message.h>
#include <mach/mig_errors.h>
#include <kern/ipc_kobject.h>
#include <ipc/ipc_object.h>
#include <ipc/ipc_kmsg.h>
#include <ipc/ipc_port.h>
#include <ipc/ipc_thread.h>

#if	MACH_MACHINE_ROUTINES
#include <machine/machine_routines.h>
#endif

/*
 *	Routine:	ipc_kobject_server
 *	Purpose:
 *		Handle a message sent to the kernel.
 *		Generates a reply message.
 *	Conditions:
 *		Nothing locked.
 */

ipc_kmsg_t
ipc_kobject_server(request)
	ipc_kmsg_t request;
{
	mach_msg_size_t reply_size = ikm_less_overhead(8192);
	ipc_kmsg_t reply;
	kern_return_t kr;

	reply = ikm_alloc(reply_size);
	if (reply == IKM_NULL) {
		printf("ipc_kobject_server: dropping request\n");
		ipc_kmsg_destroy(request);
		return IKM_NULL;
	}
	ikm_init(reply, reply_size);

	check_simple_locks();
	if (!mach_server(&request->ikm_header, &reply->ikm_header))
	if (!mach_port_server(&request->ikm_header, &reply->ikm_header))
	if (!mach_host_server(&request->ikm_header, &reply->ikm_header))
	if (!device_server(&request->ikm_header, &reply->ikm_header))
	if (!device_pager_server(&request->ikm_header, &reply->ikm_header))
#if	MACH_DEBUG
	if (!mach_debug_server(&request->ikm_header, &reply->ikm_header))
#endif	MACH_DEBUG
#if	NORMA_TASK
	if (!mach_norma_server(&request->ikm_header, &reply->ikm_header))
#endif	NORMA_TASK
#if	NORMA_VM
	if (!proxy_server(&request->ikm_header, &reply->ikm_header))
#endif	NORMA_VM
#if	MACH_MACHINE_ROUTINES
	if (!MACHINE_SERVER(&request->ikm_header, &reply->ikm_header))
#endif	MACH_MACHINE_ROUTINES
#if	MACH_IPC_TEST
		printf("ipc_kobject_server: bogus kernel message, id=%d\n",
		       request->ikm_header.msgh_id);
#else	MACH_IPC_TEST
		;
#endif	MACH_IPC_TEST
	check_simple_locks();

	kr = ((mig_reply_header_t *) &reply->ikm_header)->RetCode;
	if ((kr == KERN_SUCCESS) || (kr == MIG_NO_REPLY)) {
		mach_msg_bits_t mbits = request->ikm_header.msgh_bits;
		ipc_object_t dest =
			(ipc_object_t) request->ikm_header.msgh_remote_port;
		mach_msg_type_name_t dest_type = MACH_MSGH_BITS_REMOTE(mbits);
		ipc_thread_t self;

		/*
		 *	The server function is responsible for the contents
		 *	of the message.  The reply port right is moved
		 *	to the reply message, so we only have to deallocate
		 *	the destination port right.
		 */

		ipc_object_destroy(dest, dest_type);

		/* like ipc_kmsg_put, but without the copyout */

		ikm_check_initialized(request, request->ikm_size);
		if ((request->ikm_size == IKM_SAVED_KMSG_SIZE) &&
		    (ikm_cache() == IKM_NULL))
			ikm_cache() = request;
		else
			ikm_free(request);
	} else {
		/*
		 *	The message contents of the request are intact.
		 *	Destroy everthing except the reply port right,
		 *	which is needed in the reply message.
		 */

		request->ikm_header.msgh_local_port = MACH_PORT_NULL;
		ipc_kmsg_destroy(request);
	}

	if (kr == MIG_NO_REPLY) {
		/*
		 *	The server function will send a reply message
		 *	using the reply port right, which it has saved.
		 */

		ikm_free(reply);
		return IKM_NULL;
	} else if (!IP_VALID((ipc_port_t)reply->ikm_header.msgh_remote_port)) {
		/*
		 *	Can't queue the reply message if the destination
		 *	(the reply port) isn't valid.
		 */

		ipc_kmsg_destroy(reply);
		return IKM_NULL;
	}

	return reply;
}

/*
 *	Routine:	ipc_kobject_set
 *	Purpose:
 *		Make a port represent a kernel object of the given type.
 *		The caller is responsible for handling refs for the
 *		kernel object, if necessary.
 *	Conditions:
 *		Nothing locked.  The port must be active.
 */

void
ipc_kobject_set(port, kobject, type)
	ipc_port_t port;
	ipc_kobject_t kobject;
	ipc_kobject_type_t type;
{
	ip_lock(port);
	assert(ip_active(port));
	port->ip_bits = (port->ip_bits &~ IO_BITS_KOTYPE) | type;
	port->ip_kobject = kobject;
	ip_unlock(port);
}

/*
 *	Routine:	ipc_kobject_destroy
 *	Purpose:
 *		Release any kernel object resources associated
 *		with the port, which is being destroyed.
 *
 *		This should only be needed when resources are
 *		associated with a user's port.  In the normal case,
 *		when the kernel is the receiver, the code calling
 *		ipc_port_dealloc_kernel should clean up the resources.
 *	Conditions:
 *		The port is not locked, but it is dead.
 */

void
ipc_kobject_destroy(port)
	ipc_port_t port;
{
	switch (ip_kotype(port)) {
	    case IKOT_PAGER:
		vm_object_destroy(port);
		break;

	    default:
#if	MACH_ASSERT
		printf("ipc_kobject_destroy: port 0x%x, kobj 0x%x, type %d\n",
		       port, port->ip_kobject, ip_kotype(port));
#endif	MACH_ASSERT
		break;
	}
}
