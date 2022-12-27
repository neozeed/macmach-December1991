/*
 * Mach Operating System
 * Copyright (c) 1991,1990,1989,1988,1987 Carnegie Mellon University
 * All Rights Reserved.
 * 
 * Permission to use, copy, modify and distribute this software and its
 * documentation is hereby granted, provided that both the copyright
 * notice and this permission notice appear in all copies of the
 * software, derivative works or modified versions, and any portions
 * thereof, and that both notices appear in supporting documentation.
 * 
 * CARNEGIE MELLON ALLOWS FREE USE OF THIS SOFTWARE IN ITS 
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
 * any improvements or extensions that they make and grant Carnegie the
 * rights to redistribute these changes.
 */
/*
 * HISTORY
 * $Log:	service.c,v $
 * Revision 2.5  91/08/29  15:50:34  rpd
 * 	Added service_waitfor.
 * 	[91/08/23            rpd]
 * 
 * Revision 2.4  91/03/27  17:36:02  mrt
 * 	Changed mach.h include
 * 
 * Revision 2.3  91/03/19  11:27:01  mrt
 * 	Changed to new copyright
 * 
 * Revision 2.2  90/09/12  16:35:09  rpd
 * 	Converted to new IPC.
 * 	[90/08/12            rpd]
 * 
 * Revision 1.3  89/04/29  13:34:43  mrt
 * 	Fixed includes for Mach 2.5
 * 
 *  7-Feb-89  Richard P. Draves (rpd) at Carnegie-Mellon University
 *	Rewritten to use backup ports instead of splitting
 *	receive_rights and ownwership_rights between servers
 *	and the service server.
 *
 */
/*
 *	Program:	Service server
 *
 *	Purpose:
 *		Create ports for globally interesting services,
 *		and hand the receive rights to those ports (i.e.
 *		the ability to serve them) to whoever asks.
 *
 *	Why we need it:
 *		We need to get the service ports into the
 *		very top of the task inheritance structure,
 *		but the currently available system startup
 *		mechanism doesn't allow the actual servers
 *		to be started up from within the initial task
 *		itself.  We start up as soon as we can, and
 *		force the service ports back up the task tree,
 *		and let servers come along later to handle them.
 *
 *		In the event that a server dies, a new instantiation
 *		can grab the same service port.
 *
 *	Can be run before /etc/init instead, if desired.
 */

/*
 * We must define MACH_INIT_SLOTS and include <mach_init.h> first,
 * to make sure SERVICE_SLOT gets defined.
 */
#define MACH_INIT_SLOTS		1
#include <mach_init.h>

#include <stdio.h>
#include <mach.h>
#include <mach/message.h>
#include <mach/notify.h>
#include <mach_error.h>
#include <mach/mig_errors.h>
#include <cthreads.h>

extern boolean_t debug;
extern char *program_name;

static boolean_t service_demux();
extern boolean_t service_server();
extern boolean_t notify_server();

extern mach_port_t service_port;
mach_port_t trusted_notify_port;
mach_port_t untrusted_notify_port;
mach_port_t port_set;

#define MAXSIZE		64	/* max size for requests & replies */

typedef struct service_record {
    mach_port_t service;
    boolean_t taken;
    struct condition waitfor;
} service_record_t;

struct mutex services_lock;		/* lock for services, threads_count */
service_record_t *services;
unsigned int services_count;
unsigned int threads_count;

void
serv_init(parent)
    task_t parent;
{
    mach_port_t *ports;
    unsigned int portsCnt;
    unsigned int i;
    kern_return_t kr;

    /*
     *	Create the notification ports.
     */

    kr = mach_port_allocate(mach_task_self(),
			    MACH_PORT_RIGHT_RECEIVE, &trusted_notify_port);
    if (kr != KERN_SUCCESS)
	quit(1, "%s: mach_port_allocate: %s\n",
	     program_name, mach_error_string(kr));

    kr = mach_port_allocate(mach_task_self(),
			    MACH_PORT_RIGHT_RECEIVE, &untrusted_notify_port);
    if (kr != KERN_SUCCESS)
	quit(1, "%s: mach_port_allocate: %s\n",
	     program_name, mach_error_string(kr));

    if (debug)
	printf("Trusted notify = %x, untrusted notify = %x.\n",
	       trusted_notify_port, untrusted_notify_port);

    /*
     *	See what our parent had.  We are interested
     *	in the count, not the ports themselves.
     *	However, we save the memory for use below.
     */

    kr = mach_ports_lookup(parent, &ports, &portsCnt);
    if (kr != KERN_SUCCESS)
	quit(1, "%s: mach_ports_lookup: %s",
	     program_name, mach_error_string(kr));

    services_count = portsCnt;

    if (debug)
	printf("Got %u ports from lookup.\n", services_count);

    if (services_count <= SERVICE_SLOT)
	quit(1, "%s: no room for SERVICE_SLOT???", program_name);

    /* deallocate any send rights acquired from mach_ports_lookup */

    for (i = 0; i < services_count; i++)
	if (MACH_PORT_VALID(ports[i])) {
	    kr = mach_port_deallocate(mach_task_self(), ports[i]);
	    if (kr != KERN_SUCCESS)
		quit(1, "%s: mach_port_deallocate: %s\n",
		     program_name, mach_error_string(kr));
	}

    /*
     *	Create ports for the various services.
     *	We will have receive rights for these ports.
     */

    mutex_init(&services_lock);
    services = (service_record_t *)
	malloc(services_count * sizeof *services);
    if (services == NULL)
	quit(1, "%s: can't allocate services array", program_name);

    for (i = 0; i < services_count; i++) {
	mach_port_t service;
	mach_port_t previous;

	/* create the service port */

	kr = mach_port_allocate(mach_task_self(),
				MACH_PORT_RIGHT_RECEIVE, &service);
	if (kr != KERN_SUCCESS)
	    quit(1, "%s: can't allocate service port %d: %s",
		 program_name, i, mach_error_string(kr));

	/* acquire a send right for the service port */

	kr = mach_port_insert_right(mach_task_self(), service,
				    service, MACH_MSG_TYPE_MAKE_SEND);
	if (kr != KERN_SUCCESS)
	    quit(1, "%s: mach_port_insert_right: %s\n",
		 program_name, mach_error_string(kr));

	if (debug)
	    printf("Slot %d: service = %x.\n", i, service);

	/* request a port-destroyed notification */

	kr = mach_port_request_notification(mach_task_self(), service,
			MACH_NOTIFY_PORT_DESTROYED, 0,
			untrusted_notify_port, MACH_MSG_TYPE_MAKE_SEND_ONCE,
			&previous);
	if ((kr != KERN_SUCCESS) || (previous != MACH_PORT_NULL))
	    quit(1, "%s: mach_port_request_notification: %s\n",
		 program_name, mach_error_string(kr));

	/* request a dead-name notification */

	kr = mach_port_request_notification(mach_task_self(), service,
			MACH_NOTIFY_DEAD_NAME, FALSE,
			trusted_notify_port, MACH_MSG_TYPE_MAKE_SEND_ONCE,
			&previous);
	if ((kr != KERN_SUCCESS) || (previous != MACH_PORT_NULL))
	    quit(1, "%s: mach_port_request_notification: %s\n",
		 program_name, mach_error_string(kr));

	ports[i] = service;
	services[i].service = service;
	services[i].taken = FALSE;
	condition_init(&services[i].waitfor);
    }

    /*
     *	Remember our service port, and mark it as taken.
     */

    service_port = services[SERVICE_SLOT].service;
    services[SERVICE_SLOT].taken = TRUE;

    /*
     *	Check the ports into our parent task.
     */

    kr = mach_ports_register(parent, ports, services_count);
    if (kr != KERN_SUCCESS)
	quit(1, "%s: can't register ports in parent", program_name);

    kr = vm_deallocate(mach_task_self(), (vm_address_t) ports,
		       (vm_size_t) (services_count*sizeof(mach_port_t)));
    if (kr != KERN_SUCCESS)
	quit(1, "%s: vm_deallocate: %s\n",
	     program_name, mach_error_string(kr));

    /*
     *	Create a port set and put ports into the set.
     */

    kr = mach_port_allocate(mach_task_self(),
			    MACH_PORT_RIGHT_PORT_SET, &port_set);
    if (kr != KERN_SUCCESS)
	quit(1, "%s: mach_port_allocate: %s\n",
	     program_name, mach_error_string(kr));

    kr = mach_port_move_member(mach_task_self(),
			       trusted_notify_port, port_set);
    if (kr != KERN_SUCCESS)
	quit(1, "%s: mach_port_move_member: %s\n",
	     program_name, mach_error_string(kr));

    kr = mach_port_move_member(mach_task_self(),
			       untrusted_notify_port, port_set);
    if (kr != KERN_SUCCESS)
	quit(1, "%s: mach_port_move_member: %s\n",
	     program_name, mach_error_string(kr));

    for (i = 0; i < services_count; i++) {
	kr = mach_port_move_member(mach_task_self(),
				   services[i].service, port_set);
	if (kr != KERN_SUCCESS)
	    quit(1, "%s: mach_port_move_member: %s\n",
		 program_name, mach_error_string(kr));
    }

    /*
     *	We start off with one thread servicing requests.
     *	More threads are created as needed.
     */

    threads_count = 1;
}

void
serv_loop()
{
    kern_return_t kr;

    cthread_wire();

    kr = mach_msg_server(service_demux, MAXSIZE, port_set);
    quit(1, "%s: mach_msg_server: %s\n",
	 program_name, mach_error_string(kr));
}

static boolean_t
service_demux(request, reply)
    mach_msg_header_t *request, *reply;
{
    if (debug)
	printf("Received request, port %x id %d\n",
	       request->msgh_local_port, request->msgh_id);

    if (request->msgh_local_port == service_port)
	return service_server(request, reply);
    else if ((request->msgh_local_port == trusted_notify_port) ||
	     (request->msgh_local_port == untrusted_notify_port))
	return notify_server(request, reply);
    else {
	/*
	 *	This must be a request directed to a port, probably
	 *	the name server port, which hasn't been taken yet.
	 *	We destroy the request without replying.
	 */

	mach_msg_destroy(request);
	((mig_reply_header_t *) reply)->RetCode = MIG_NO_REPLY;
	return TRUE;
    }
}

kern_return_t
do_mach_notify_port_deleted(notify, name)
    mach_port_t notify;
    mach_port_t name;
{
    if (notify != untrusted_notify_port)
	quit(1, "%s: do_mach_notify_port_deleted\n", program_name);

    return KERN_FAILURE;
}

kern_return_t
do_mach_notify_msg_accepted(notify, name)
    mach_port_t notify;
    mach_port_t name;
{
    if (notify != untrusted_notify_port)
	quit(1, "%s: do_mach_notify_msg_accepted\n", program_name);

    return KERN_FAILURE;
}

kern_return_t
do_mach_notify_port_destroyed(notify, port)
    mach_port_t notify;
    mach_port_t port;
{
    mach_port_t previous;
    unsigned int i;
    kern_return_t kr;

    if (notify != untrusted_notify_port)
	quit(1, "%s: do_mach_notify_port_destroyed\n", program_name);

    /*
     *	We must be cautious in processing this notification.
     *	However, because the message was type-checked
     *	we must have receive rights for the port.  So if it
     *  matches one of our service ports, we can be confident.
     */

    if (!MACH_PORT_VALID(port))
	return KERN_FAILURE;

    mutex_lock(&services_lock);
    for (i = 0; i < services_count; i++)
	if (services[i].service == port) {
	    if (debug)
		printf("Port %x (slot %d) is returned.\n", port, i);

	    if (!services[i].taken)
		quit(1, "%s: port %x not taken!\n", program_name, port);
	    services[i].taken = FALSE;

	    /* start receiving from the port again */

	    kr = mach_port_move_member(mach_task_self(), port, port_set);
	    if (kr != KERN_SUCCESS)
		quit(1, "%s: mach_port_move_member: %s\n",
		     program_name, mach_error_string(kr));

	    /* re-request a port-destroyed notification */

	    kr = mach_port_request_notification(mach_task_self(), port,
			MACH_NOTIFY_PORT_DESTROYED, 0,
			untrusted_notify_port, MACH_MSG_TYPE_MAKE_SEND_ONCE,
			&previous);
	    if (kr != KERN_SUCCESS)
		quit(1, "%s: mach_port_request_notification: %s\n",
		     program_name, mach_error_string(kr));
	    mutex_unlock(&services_lock);

	    if (MACH_PORT_VALID(previous)) {
		kr = mach_port_deallocate(mach_task_self(), previous);
		if (kr != KERN_SUCCESS)
		    quit(1, "%s: mach_port_deallocate: %s\n",
			 program_name, mach_error_string(kr));
	    }

	    /* we assume responsibility for the receive right */

	    return KERN_SUCCESS;
	}

    mutex_unlock(&services_lock);
    return KERN_FAILURE;
}

kern_return_t
do_mach_notify_no_senders(notify, mscount)
    mach_port_t notify;
    mach_port_mscount_t mscount;
{
    if (notify != untrusted_notify_port)
	quit(1, "%s: do_mach_notify_no_senders\n", program_name);

    return KERN_FAILURE;
}

kern_return_t
do_mach_notify_send_once(notify)
    mach_port_t notify;
{
    if (notify != untrusted_notify_port)
	quit(1, "%s: do_mach_notify_send_once\n", program_name);

    return KERN_SUCCESS;
}

kern_return_t
do_mach_notify_dead_name(notify, name)
    mach_port_t notify;
    mach_port_t name;
{
    unsigned int i;
    kern_return_t kr;

    if (notify != trusted_notify_port)
	return KERN_FAILURE;

    /*
     *	We can trust this notification.
     */

    mutex_lock(&services_lock);
    for (i = 0; i < services_count; i++)
	if (services[i].service == name)
	    break;

    if (debug)
	printf("Port %x (slot %d) died.\n", name, i);

    /* clean up the dead name */

    kr = mach_port_mod_refs(mach_task_self(), name,
			    MACH_PORT_RIGHT_DEAD_NAME, -2);
    if (kr != KERN_SUCCESS)
	quit(1, "%s: mach_port_mod_refs: %s\n",
	     program_name, mach_error_string(kr));

    services[i].service = MACH_PORT_DEAD;
    services[i].taken = FALSE;

    mutex_unlock(&services_lock);
    return KERN_SUCCESS;
}

/*
 *	Routine:	service_checkin
 *	Function:
 *		This is the routine the MiG interface calls...
 *		it merely returns the requested port (as granted).
 */
kern_return_t
do_service_checkin(request, requested, granted)
    mach_port_t request;
    mach_port_t requested;
    mach_port_t *granted;
{
    unsigned int i;
    kern_return_t kr;

    if (!MACH_PORT_VALID(requested))
	return KERN_FAILURE;

    mutex_lock(&services_lock);
    for (i = 0; i < services_count; i++)
	if ((services[i].service == requested) && !services[i].taken) {
	    if (debug)
		printf("Port %x (slot %d) is taken.\n", requested, i);

	    services[i].taken = TRUE;
	    mutex_unlock(&services_lock);
	    condition_broadcast(&services[i].waitfor);

	    /* deallocate the send right we acquired in the request */

	    kr = mach_port_deallocate(mach_task_self(), requested);
	    if (kr != KERN_SUCCESS)
		quit(1, "%s: mach_port_deallocate: %s\n",
		     program_name, mach_error_string(kr));

	    *granted = requested;
	    return KERN_SUCCESS;
	}
    mutex_unlock(&services_lock);

    if (debug)
	printf("Request for port %x refused.\n", requested);

    /* mach_msg_server will deallocate requested for us */
    return KERN_FAILURE;
}

kern_return_t
do_service_waitfor(request, requested)
    mach_port_t request;
    mach_port_t requested;
{
    unsigned int i;
    kern_return_t kr;

    if (!MACH_PORT_VALID(requested))
	return KERN_FAILURE;

    mutex_lock(&services_lock);
    for (i = 0; i < services_count; i++)
	if (services[i].service == requested) {
	    /* wait for the port to be taken */

	    while ((services[i].service == requested) && !services[i].taken) {
		/* we want at least one thread listening to requests */

		if (--threads_count == 0) {
		    threads_count++;	/* for the new thread */
		    cthread_detach(cthread_fork((cthread_fn_t) serv_loop,
						(any_t) 0));
		}

		condition_wait(&services[i].waitfor, &services_lock);
		threads_count++;
	    }
	    mutex_unlock(&services_lock);

	    /* deallocate the send right we acquired in the request */

	    kr = mach_port_deallocate(mach_task_self(), requested);
	    if (kr != KERN_SUCCESS)
		quit(1, "%s: mach_port_deallocate: %s\n",
		     program_name, mach_error_string(kr));

	    return KERN_SUCCESS;
	}
    mutex_unlock(&services_lock);

    /* mach_msg_server will deallocate requested for us */
    return KERN_FAILURE;
}
