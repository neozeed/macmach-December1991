/* 
 * Mach Operating System
 * Copyright (c) 1991 Carnegie Mellon University
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
 * $Log:	ipc_input.c,v $
 * Revision 2.4  91/08/28  11:16:00  jsb
 * 	Mark received pages as dirty and not busy.
 * 	Initialize copy->cpy_cont and copy->cpy_cont_args.
 * 	[91/08/16  10:44:19  jsb]
 * 
 * 	Fixed reference to norma_ipc_kobject_send.
 * 	[91/08/15  08:42:23  jsb]
 * 
 * 	Renamed clport things to norma things.
 * 	[91/08/14  21:34:13  jsb]
 * 
 * 	Fixed norma_ipc_handoff code.
 * 	Added splon/sploff redefinition hack.
 * 	[91/08/14  19:11:07  jsb]
 * 
 * Revision 2.3  91/08/03  18:19:19  jsb
 * 	Use MACH_MSGH_BITS_COMPLEX_DATA instead of null msgid to determine
 * 	whether data follows kmsg.
 * 	[91/08/01  21:57:37  jsb]
 * 
 * 	Eliminated remaining old-style page list code.
 * 	Cleaned up and corrected clport_deliver_page.
 * 	[91/07/27  18:47:08  jsb]
 * 
 * 	Moved MACH_MSGH_BITS_COMPLEX_{PORTS,DATA} to mach/message.h.
 * 	[91/07/04  13:10:48  jsb]
 * 
 * 	Use vm_map_copy_t's instead of old style page_lists.
 * 	Still need to eliminate local conversion between formats.
 * 	[91/07/04  10:18:11  jsb]
 * 
 * Revision 2.2  91/06/17  15:47:41  jsb
 * 	Moved here from ipc/ipc_clinput.c.
 * 	[91/06/17  11:02:28  jsb]
 * 
 * Revision 2.2  91/06/06  17:05:18  jsb
 * 	Fixed copyright.
 * 	[91/05/24  13:18:23  jsb]
 * 
 * 	First checkin.
 * 	[91/05/14  13:29:10  jsb]
 * 
 */
/*
 *	File:	norma/ipc_input.c
 *	Author:	Joseph S. Barrera III
 *	Date:	1991
 *
 *	Functions to support ipc between nodes in a single Mach cluster.
 */

#include <vm/vm_kern.h>
#include <vm/vm_page.h>
#include <mach/vm_param.h>
#include <mach/port.h>
#include <mach/message.h>
#include <kern/assert.h>
#include <kern/host.h>
#include <kern/sched_prim.h>
#include <kern/ipc_sched.h>
#include <kern/ipc_kobject.h>
#include <kern/zalloc.h>
#include <ipc/ipc_mqueue.h>
#include <ipc/ipc_thread.h>
#include <ipc/ipc_kmsg.h>
#include <ipc/ipc_port.h>
#include <ipc/ipc_pset.h>
#include <ipc/ipc_space.h>
#include <ipc/ipc_marequest.h>

#undef	assert
#define assert(ex)\
{if(!(ex))panic("%s:%d assert failed\n", __FILE__, __LINE__);}

#if 1
#define	sploff()	splhigh()
#define	splon(s)	splx(s)
#endif

extern zone_t vm_map_copy_zone;

extern ipc_mqueue_t	norma_ipc_handoff_mqueue;
extern ipc_kmsg_t	norma_ipc_handoff_msg;
extern mach_msg_size_t	norma_ipc_handoff_max_size;
extern mach_msg_size_t	norma_ipc_handoff_msg_size;

ipc_kmsg_t norma_kmsg_complete;
ipc_kmsg_t norma_kmsg_incomplete;

#define	ikm_remote	ikm_marequest
#define	ikm_msgid	ikm_prev

int jc_handoff_fasthits = 0;
int jc_handoff_hits = 0;
int jc_handoff_misses = 0;
int jc_handoff_m2 = 0;		/* XXX very rare (0.1 %) */
int jc_handoff_m3 = 0;
int jc_handoff_m4 = 0;
int jc_netipc_ast = 0;

/*
 * Translate ports. Don't do anything with data.
 */
norma_receive_complex_ports(msgh)
	mach_msg_header_t *msgh;
{
	vm_offset_t saddr = (vm_offset_t) (msgh + 1);
	vm_offset_t eaddr = (vm_offset_t) msgh + msgh->msgh_size;
	boolean_t ool_data = FALSE;

	while (saddr < eaddr) {
		mach_msg_type_long_t *type;
		mach_msg_type_size_t size;
		mach_msg_type_number_t number;
		boolean_t is_inline, longform, is_port;
		vm_size_t length;
		vm_offset_t addr;

		type = (mach_msg_type_long_t *) saddr;
		is_inline = type->msgtl_header.msgt_inline;
		longform = type->msgtl_header.msgt_longform;
		if (longform) {
			is_port = MACH_MSG_TYPE_PORT_ANY(type->msgtl_name);
			size = type->msgtl_size;
			number = type->msgtl_number;
			saddr += sizeof(mach_msg_type_long_t);
		} else {
			is_port = MACH_MSG_TYPE_PORT_ANY(type->msgtl_header.
							 msgt_name);
			size = type->msgtl_header.msgt_size;
			number = type->msgtl_header.msgt_number;
			saddr += sizeof(mach_msg_type_t);
		}

		/* calculate length of data in bytes, rounding up */
		length = ((number * size) + 7) >> 3;

		if (! is_inline && is_port) {
			panic("norma_receive_complex_ports: out-of-line\n");
		}

		if (is_port) {
			ipc_port_t *ports = (ipc_port_t *) saddr;
			mach_msg_type_number_t i;

			for (i = 0; i < number; i++) {
				ports[i] = (ipc_port_t)
				    norma_ipc_get_proxy(ports[i],TRUE);
			}
		}
		if (is_inline) {
			saddr += (length + 3) &~ 3;
		} else {
			saddr += sizeof(vm_offset_t);
		}
	}
}

/*
 * Called in ast-mode, where it is safe to execute ipc code but not to block.
 * (This can actually be in an ast, or from an interrupt handler when the
 * processor was in the idle thread or spinning on norma_ipc_handoff_mqueue.)
 *
 * xxx verify port locking
 */
norma_handoff_kmsg(kmsg)
	ipc_kmsg_t kmsg;
{
	ipc_port_t port;
	ipc_mqueue_t mqueue;
	ipc_pset_t pset;
	ipc_thread_t receiver;
	ipc_thread_queue_t receivers;
	
	jc_handoff_fasthits++;

	if (kmsg->ikm_header.msgh_bits & MACH_MSGH_BITS_COMPLEX_PORTS) {
		norma_receive_complex_ports(&kmsg->ikm_header);
		kmsg->ikm_header.msgh_bits &= ~MACH_MSGH_BITS_COMPLEX_PORTS;
	}
#if 3
	if (kmsg->ikm_header.msgh_bits & MACH_MSGH_BITS_COMPLEX_DATA) {
		kmsg->ikm_header.msgh_bits &= ~MACH_MSGH_BITS_COMPLEX_DATA;
	}
#endif

	port = (ipc_port_t) kmsg->ikm_header.msgh_remote_port;
	assert(IP_VALID(port));

	/*
	 * We must check to see whether this message is destined for a
	 * kernel object. If it is, and if we were to call ipc_mqueue_send,
	 * we would execute the kernel operation, possibly blocking,
	 * which would be bad. Instead, we hand the kmsg off to a thread
	 * which does the delivery.
	 *
	 * XXX reason different, but still need to do it
	 */
	if (port->ip_receiver == ipc_space_kernel) {
		norma_ipc_kobject_send(kmsg);
		return;
	}

	/*
	 * xxx
	 * only try to handoff if someone waiting for us
	 */
	if (norma_ipc_handoff_mqueue == IMQ_NULL) {
		/* XXX queue length */
		ipc_mqueue_send_always(kmsg);
		return;
	}

	ip_lock(port);

	port->ip_msgcount++;
	assert(port->ip_msgcount > 0);

	pset = port->ip_pset;
	if (pset == IPS_NULL) {
		mqueue = &port->ip_messages;
	} else {
		mqueue = &pset->ips_messages;
	}

	/*
	 * If someone is spinning on this queue, we must release them.
	 * However, if the message is too large for them to successfully
	 * receive it, we continue below to find a receiver.
	 */
	if (mqueue == norma_ipc_handoff_mqueue) {
		norma_ipc_handoff_msg = kmsg;
		if (kmsg->ikm_header.msgh_size <= norma_ipc_handoff_max_size) {
			ip_unlock(port);
			return;
		}
		norma_ipc_handoff_msg_size = kmsg->ikm_header.msgh_size;
	}
	
	imq_lock(mqueue);
	receivers = &mqueue->imq_threads;
	ip_unlock(port);
	
	for (;;) {
		receiver = ipc_thread_queue_first(receivers);
		if (receiver == ITH_NULL) {
			/* no receivers; queue kmsg */
			
			ipc_kmsg_enqueue_macro(&mqueue->imq_messages, kmsg);
			imq_unlock(mqueue);
			return;
		}
		
		ipc_thread_rmqueue_first_macro(receivers, receiver);
		assert(ipc_kmsg_queue_empty(&mqueue->imq_messages));
		
		if (kmsg->ikm_header.msgh_size <= receiver->ith_msize) {
			/* got a successful receiver */
			
			receiver->ith_state = MACH_MSG_SUCCESS;
			receiver->ith_kmsg = kmsg;
			imq_unlock(mqueue);
			
			thread_go(receiver);
			return;
		}
		
		receiver->ith_state = MACH_RCV_TOO_LARGE;
		receiver->ith_msize = kmsg->ikm_header.msgh_size;
		thread_go(receiver);
	}
}

void netipc_ast()
{
	int s;
	ipc_kmsg_t kmsg;

	s = sploff();
	while (kmsg = norma_kmsg_complete) {
		norma_kmsg_complete = kmsg->ikm_next;
		norma_handoff_kmsg(kmsg);
	}
	ast_off(cpu_number(), AST_NETIPC);
	splon(s);
}

/*
 * XXX need to make get_proxy(, FALSE) safe to call from interrupt level
 */
norma_deliver_kmsg(kmsg, remote, msgid)
	ipc_kmsg_t kmsg;
	int remote;
	int msgid;
{
	register mach_msg_header_t *msgh;
	ipc_port_t remote_port;

	/*
	 * Translate remote_port
	 */
	msgh = (mach_msg_header_t *) &kmsg->ikm_header;
	remote_port = (ipc_port_t)
	    norma_ipc_get_proxy(msgh->msgh_remote_port, FALSE);
	if (remote_port == IP_NULL) {
		printf("%d: null remote port msgh_id=%d uid=0x%x\n",
		       node_self(),
		       msgh->msgh_id,
		       msgh->msgh_remote_port);
		return;
	}
	if (remote_port->ip_receiver == ipc_space_remote) {
		printf("%d proxy remote port msgh_id=%d uid=0x%x\n",
		       node_self(),
		       msgh->msgh_id,
		       msgh->msgh_remote_port);
		return;
	}
	msgh->msgh_remote_port = (mach_port_t) remote_port;
	kmsg->ikm_size = IKM_SIZE_NORMA;

#if 1
	/*
	 * Translate local port. This really should be done by receiver,
	 * since get_proxy(, TRUE) is not likely to be safe at interrupt
	 * level.
	 */
	msgh->msgh_local_port = (mach_port_t)
	    norma_ipc_get_proxy(msgh->msgh_local_port, TRUE);
#endif

	/*
	 * If the message is incomplete, put it on the incomplete list.
	 */
	if (msgh->msgh_bits & MACH_MSGH_BITS_COMPLEX_DATA) {
		* (long *) &kmsg->ikm_remote = remote;
		* (long *) &kmsg->ikm_msgid = msgid;
		kmsg->ikm_next = norma_kmsg_incomplete;
		norma_kmsg_incomplete = kmsg;
		return;
	}
	/*
	 * The message is complete.
	 * If it safe to process it now, do so.
	 */
	if (norma_ipc_handoff_mqueue) {
		norma_handoff_kmsg(kmsg);
		return;
	}
	/*
	 * It is not safe now to process the complete message,
	 * so place it on the list of completed messages,
	 * and post an ast.
	 * XXX
	 * 1. should be conditionalized on whether we really
	 *	are called at interrupt level
	 * 2. should check flag set by *all* idle loops
	 * 3. this comment applies as well to deliver_page
	 */
{
	register ipc_kmsg_t *kmsgp;
	
	kmsg->ikm_next = IKM_NULL;
	if (norma_kmsg_complete) {
		for (kmsgp = &norma_kmsg_complete;
		     (*kmsgp)->ikm_next;
		     kmsgp = &(*kmsgp)->ikm_next) {
			continue;
		}
		(*kmsgp)->ikm_next = kmsg;
	} else {
		norma_kmsg_complete = kmsg;
	}
}
{
	register int s = sploff();
	jc_handoff_misses++;
	ast_on(cpu_number(), AST_NETIPC);
	splon(s);
}
}

norma_deliver_page(page, copy_msgh_offset, remote, copy_index, copy_npages,
		    size, offset, msgid)
	vm_page_t page;
	vm_offset_t copy_msgh_offset;
	int remote;
	unsigned int copy_index;
	unsigned int copy_npages;
	int size;
	int offset;
	int msgid;
{
	ipc_kmsg_t kmsg, *kmsgp;
	vm_map_copy_t copy;

	/*
	 * Find appropriate kmsg.
	 * XXX consider making this an array?
	 */
	for (kmsgp = &norma_kmsg_incomplete; ; kmsgp = &kmsg->ikm_next) {
		kmsg = *kmsgp;
		assert(kmsg);
		if ((long)kmsg->ikm_remote == remote) {
			break;
		}
	}

	/*
	 * Mark page dirty (why?) and not busy.
	 */
	page->busy = FALSE;
	page->dirty = TRUE;

	/*
	 * Create copy object if this is its first page.
	 */
	if (copy_index == 0) {
		copy = (vm_map_copy_t) netipc_copy_grab();
		copy->type = VM_MAP_COPY_PAGE_LIST;
		copy->cpy_npages = copy_npages;
		copy->offset = offset;
		copy->size = size;
		copy->cpy_page_list[0] = page;
		copy->cpy_cont = ((kern_return_t (*)()) 0);
		copy->cpy_cont_args = (char *) VM_MAP_COPYIN_ARGS_NULL;
		* ((vm_map_copy_t *) (((vm_offset_t) &kmsg->ikm_header)
				     + copy_msgh_offset)) = copy;
	} else {
		copy = * ((vm_map_copy_t *) (((vm_offset_t) &kmsg->ikm_header)
				     + copy_msgh_offset));
		copy->cpy_page_list[copy_index] = page;
	}
	/*
	 * If the message is complete, take it off the list and process it.
	 * If it unsafe to process it now, post an ast.
	 */
	if (copy_index == copy_npages - 1) {
		*kmsgp = kmsg->ikm_next;
		* (long *) &kmsg->ikm_remote = 0;
		/*
		 * XXX
		 * 1. should be conditionalized on whether we really
		 *	are called at interrupt level
		 * 2. should check flag set by *all* idle loops
		 * 3. this comment applies as well to deliver_kmsg
		 */
		if (norma_ipc_handoff_mqueue) {
			norma_handoff_kmsg(kmsg);
		} else {
			int s;

			kmsg->ikm_next = IKM_NULL;
			if (norma_kmsg_complete) {
				for (kmsgp = &norma_kmsg_complete;
				     (*kmsgp)->ikm_next;
				     kmsgp = &(*kmsgp)->ikm_next) {
					continue;
				}
				(*kmsgp)->ikm_next = kmsg;
			} else {
				norma_kmsg_complete = kmsg;
			}
			s = sploff();
			jc_handoff_misses++;
			ast_on(cpu_number(), AST_NETIPC);
			splon(s);
		}
	}
}
