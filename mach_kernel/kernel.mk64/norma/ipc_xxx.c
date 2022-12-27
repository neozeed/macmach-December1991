/* 
 * Mach Operating System
 * Copyright (c) 1991,1990 Carnegie Mellon University
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
 * $Log:	ipc_xxx.c,v $
 * Revision 2.5  91/08/28  11:16:15  jsb
 * 	Renamed clport things to norma things.
 * 	Eliminated NORMA_ETHER special case node/lid uid division.
 * 	[91/08/15  09:14:06  jsb]
 * 
 * 	Renamed ipc_clport_foo things to norma_ipc_foo.
 * 	Added host_port_at_node().
 * 	[91/08/14  19:21:16  jsb]
 * 
 * Revision 2.4  91/08/03  18:19:34  jsb
 * 	Conditionalized printfs.
 * 	[91/08/01  22:55:17  jsb]
 * 
 * 	Eliminated dynamically allocated clport structures; this information
 * 	is now stored in ports directly. This simplifies issues of allocation
 * 	at interrupt time.
 * 
 * 	Added definitions for IP_CLPORT_{NODE,LID} appropriate for using the
 * 	low two bytes of an internet address as a node number.
 * 
 * 	Revised norma_special_port mechanism to better handle port death
 * 	and port replacement. Eliminated redundant special port calls.
 * 	Cleaned up associated startup code.
 * 	[91/07/25  19:09:16  jsb]
 * 
 * Revision 2.3  91/07/01  08:25:52  jsb
 * 	Some locking changes.
 * 	[91/06/29  15:10:46  jsb]
 * 
 * Revision 2.2  91/06/17  15:48:05  jsb
 * 	Moved here from ipc/ipc_clport.c.
 * 	[91/06/17  11:07:25  jsb]
 * 
 * Revision 2.7  91/06/06  17:05:35  jsb
 * 	Added norma_get_special_port, norma_set_special_port.
 * 	[91/05/25  10:28:14  jsb]
 * 
 * 	Much code moved to other norma/ files.
 * 	[91/05/13  17:16:24  jsb]
 * 
 */
/*
 *	File:	norma/ipc_xxx.c
 *	Author:	Joseph S. Barrera III
 *	Date:	1990
 *
 *	Functions to support ipc between nodes in a single Mach cluster.
 */

#include <norma_vm.h>
#include <norma_ether.h>

#include <vm/vm_kern.h>
#include <mach/vm_param.h>
#include <mach/port.h>
#include <mach/message.h>
#include <mach/norma_special_ports.h>
#include <kern/assert.h>
#include <kern/host.h>
#include <kern/sched_prim.h>
#include <kern/ipc_sched.h>
#include <kern/ipc_kobject.h>
#include <kern/zalloc.h>
#include <device/device_port.h>
#include <ipc/ipc_mqueue.h>
#include <ipc/ipc_thread.h>
#include <ipc/ipc_kmsg.h>
#include <ipc/ipc_port.h>
#include <ipc/ipc_pset.h>
#include <ipc/ipc_space.h>
#include <ipc/ipc_marequest.h>

/*
 * A uid (unique id) = node + lid (local id)
 * We embed the node id in the uid because receive rights rarely move.
 */

/*
 * 11 bits for node		-> max 2048 nodes
 * 21 bits for local id		-> max 2097151 exported ports per node
 */
#define	IP_NORMA_NODE(uid)	((unsigned long)(uid) & 0x000007ff)
#define	IP_NORMA_LID(uid)	(((unsigned long)(uid) >> 11) & 0x001fffff)
#define	IP_NORMA_UID(node, lid)	(((lid) << 11) | (node))

#define	MAX_SPECIAL_ID		16

extern ipc_port_t		norma_ipc_get_proxy();
extern void			netipc_thread();

ipc_space_t			ipc_space_remote;
ipc_port_t			norma_port_list = (ipc_port_t) 0;
ipc_port_t			host_special_ports[MAX_SPECIAL_ID];
unsigned long			lid_counter = MAX_SPECIAL_ID;

int				NoiseXXX = 0;

norma_ipc_init()
{
	/*
	 * Register master device, host, and host_priv ports.
	 */
	assert(master_device_port != IP_NULL);
	norma_set_special_port(realhost.host_priv_self,
			       node_self(),
			       NORMA_DEVICE_PORT,
			       master_device_port);

	assert(realhost.host_self != IP_NULL);
	norma_set_special_port(realhost.host_priv_self,
			       node_self(),
			       NORMA_HOST_PORT,
			       realhost.host_self);

	assert(realhost.host_priv_self != IP_NULL);
	norma_set_special_port(realhost.host_priv_self,
			       node_self(),
			       NORMA_HOST_PRIV_PORT,
			       realhost.host_priv_self);

	/*
	 * Initialize network subsystem
	 */
	netipc_init();

	/*
	 * Start up netipc thread and kserver module.
	 * XXX netipc_init should do the former...
	 */
	(void) kernel_thread(kernel_task, netipc_thread, (char *) 0);
	norma_kserver_startup();
}

ip_norma_node(uid)
	unsigned long uid;
{
	return IP_NORMA_NODE(uid);
}

#if 1
ipc_port_node(port)
	ipc_port_t port;
{
	if (!port) return node_self();
	if (!port->ip_norma_uid) return node_self();
	return IP_NORMA_NODE(port->ip_norma_uid);
}

/* XXX acccess check on host port */

#if 1
ipc_port_t
master_device_port_at_node(node)
{
	if (node == node_self()) {
		return host_special_ports[NORMA_DEVICE_PORT];
	} else {
		return norma_ipc_get_proxy(IP_NORMA_UID(node, NORMA_DEVICE_PORT),
					   TRUE);
	}
}

ipc_port_t
host_port_at_node(node)
{
	if (node == node_self()) {
		return host_special_ports[NORMA_HOST_PORT];
	} else {
		return norma_ipc_get_proxy(IP_NORMA_UID(node, NORMA_HOST_PORT),
					   TRUE);
	}
}
#endif

kern_return_t
norma_get_special_port(host, node, which, portp)
	host_t host;
	int node;
	int which;
	ipc_port_t *portp;
{
	ipc_port_t port;

#if 1
	if (which == 0) {
		which = NORMA_NAMESERVER_PORT;
	}
#endif
	if(NoiseXXX)printf("%d: norma_get_special_port\n", node_self());
	switch (which) {
		case NORMA_DEVICE_PORT:
		case NORMA_HOST_PORT:
		case NORMA_HOST_PRIV_PORT:
		case NORMA_NAMESERVER_PORT:
		break;
		
		default:
		return KERN_INVALID_ARGUMENT;
	}
	if (node == node_self()) {
		port = host_special_ports[which];
	} else {
		port = norma_ipc_get_proxy(IP_NORMA_UID(node, which), TRUE);
	}
	*portp = ipc_port_copy_send(port);
	return KERN_SUCCESS;
}

kern_return_t
norma_set_special_port(host, node, which, port)
	host_t host;
	int node;		/* XXX */
	int which;
	ipc_port_t port;
{
#if 1
	if (node == 0 && which == 0) {
		which = NORMA_NAMESERVER_PORT;
	}
#endif
	if(NoiseXXX)printf("%d: norma_set_special_port\n", node_self());
	switch (which) {
		case NORMA_DEVICE_PORT:
		case NORMA_HOST_PORT:
		case NORMA_HOST_PRIV_PORT:
		case NORMA_NAMESERVER_PORT:
		break;
		
		default:
		return KERN_INVALID_ARGUMENT;
	}
	port->ip_srights  = 20100100;		/* XXX */
	port->ip_sorights = 20200100;		/* XXX */
	host_special_ports[which] = port;
	return KERN_SUCCESS;
}

#endif

/*
 * Allocate norma uid for local port.
 * Port must be locked.
 */
norma_ipc_export(port)
	ipc_port_t port;
{
	port->ip_norma_uid = IP_NORMA_UID(node_self(), lid_counter++);
	port->ip_srights  = 10100100;
	port->ip_sorights = 10200100;
	port->ip_norma_next = norma_port_list;
	norma_port_list = port;
}

norma_ipc_destroy(port)
	ipc_port_t port;
{
	ipc_port_t *portp;
	int i;

	/*
	 * If it has a uid, remote it from list
	 * Hey, here's an idea: what about using a queue?
	 */
	if (port->ip_norma_uid) {
		for (portp = &norma_port_list;; portp = &(*portp)->ip_norma_next) {
			assert(portp);
			if (*portp == port) {
				if(NoiseXXX)printf("removed dead port\n");
				*portp = port->ip_norma_next;
				break;
			}
		}
	}

	/*
	 * Remove it from list of special ports.
	 * Note that it may be in more than one slot.
	 */
	for (i = 0; i < MAX_SPECIAL_ID; i++) {
		if (host_special_ports[i] == port) {
			if(NoiseXXX)printf("removed special%d\n", i);
			host_special_ports[i] = IP_NULL;
		}
	}
}

/*
 * Find port for given uid. If no such port exists, create one.
 *
 * xxx should differentiate between uid=local and uid=remote,
 * xxx that is, this should probably be two different routines
 */
ipc_port_t
norma_ipc_get_proxy(uid, create_proxy)
	unsigned long uid;
	boolean_t create_proxy;
{
	ipc_port_t port;
	mach_port_t name;

	if (! uid) {
		return 0;
	}

	/*
	 * XXX
	 * Sure would be nice to have some locking here,
	 * not to mention a more efficient data structure.
	 *
	 * would be trivial to change into hash table
	 */
	for (port = norma_port_list; port; port = port->ip_norma_next) {
		if (port->ip_norma_uid == uid) {
			return port;
		}
	}
	if (! create_proxy && IP_NORMA_NODE(uid) == node_self()) {
		kern_return_t kr;
		kr = norma_get_special_port(0/*xxx*/, node_self(),
					    IP_NORMA_LID(uid), &port);
		if(NoiseXXX)printf("get_proxy uid=0x%x kr=0x%x\n", uid, kr);
		if (port == IP_NULL) {
			if(NoiseXXX)printf("get_proxy: got null port\n");
		}
		if (kr == KERN_SUCCESS) {
			return port;
		}
	}

	if (! create_proxy) {
		if(NoiseXXX)printf("%d: get_proxy failed: 0x%x\n", node_self(), uid);
		return IP_NULL;
	}

	port = (ipc_port_t) io_alloc(IOT_PORT);
	if (port == IP_NULL)
		return IP_NULL;

	io_lock_init(&port->ip_object);
	port->ip_references = 1;
	port->ip_object.io_bits = io_makebits(TRUE, IOT_PORT, 0);

	/*
	 *	The actual values of ip_receiver_name aren't important,
	 *	as long as they are valid (not null/dead).
	 */

	ipc_port_init(port, ipc_space_remote, 1);
	port->ip_srights  = 30100100;
	port->ip_sorights = 30200100;

	port->ip_norma_uid = uid;
	port->ip_norma_next = norma_port_list;
	norma_port_list = port;

	return port;
}
