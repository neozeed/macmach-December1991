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
 * $Log:	ipc_wait.c,v $
 * Revision 2.6  91/09/04  11:28:45  jsb
 * 	Use splhigh/splx instead of sploff/splon for now.
 * 	[91/09/04  09:46:45  jsb]
 * 
 * Revision 2.5  91/08/28  11:16:12  jsb
 * 	Renamed ast_clport things to ast_netipc things.
 * 	[91/08/15  09:12:09  jsb]
 * 
 * 	Fixed, and added counters.
 * 	[91/08/14  19:22:35  jsb]
 * 
 * Revision 2.4  91/08/03  18:19:32  jsb
 * 	Fixed include.
 * 	[91/07/17  14:06:39  jsb]
 * 
 * Revision 2.3  91/06/17  15:48:02  jsb
 * 	Changed norma include.
 * 	[91/06/17  11:01:02  jsb]
 * 
 * Revision 2.2  91/06/06  17:56:02  jsb
 * 	First checkin.
 * 	[91/06/06  17:51:41  jsb]
 * 
 */
/*
 *	File:	norma/ipc_wait.c
 *	Author:	Joseph S. Barrera III
 *	Date:	1991
 */

#include <mach_host.h>

#include <mach/port.h>
#include <mach/message.h>
#include <kern/assert.h>
#include <kern/counters.h>
#include <kern/sched_prim.h>
#include <kern/ipc_sched.h>
#include <kern/ipc_kobject.h>
#include <ipc/ipc_mqueue.h>
#include <ipc/ipc_thread.h>
#include <ipc/ipc_kmsg.h>
#include <ipc/ipc_port.h>
#include <ipc/ipc_pset.h>
#include <ipc/ipc_space.h>
#include <ipc/ipc_marequest.h>

#if 1
#undef	assert
#define assert(ex)\
{if(!(ex))panic("%s:%d assert failed\n", __FILE__, __LINE__);}
#endif

#if 1
#define	sploff()	splhigh()
#define	splon(s)	splx(s)
#endif

/*
 * XXX Needs locking to be multiprocessor safe.
 * XXX We probably might also want per-processor spinning,
 * XXX although this will complicate the sending code.
 *
 * We signal that we are waiting by setting handoff_mqueue nonzero.
 * Our sender specifies that something has changed by setting msg nonzero.
 * We then signal that we are releasing this module by setting msg zero.
 */
ipc_mqueue_t	norma_ipc_handoff_mqueue;
ipc_kmsg_t	norma_ipc_handoff_msg;
mach_msg_size_t	norma_ipc_handoff_max_size;
mach_msg_size_t	norma_ipc_handoff_msg_size;

int c_break_reset = 0;
int c_break_handoff = 0;
int c_break_thread = 0;
int c_break_gcount = 0;
int c_break_lcount = 0;
int c_break_ast = 0;
int c_break_ast_terminate = 0;
int c_break_ast_halt = 0;
int c_break_ast_block = 0;
int c_break_ast_network = 0;
int c_break_ast_netipc = 0;

/*
 * Spin until something else is runnable or until a kmsg shows up.
 */
ipc_kmsg_t
norma_ipc_kmsg_accept(mqueue, max_size, msg_size)
	register volatile ipc_mqueue_t mqueue;
	mach_msg_size_t max_size;
	mach_msg_size_t *msg_size;
{
	register processor_t myprocessor;
	register volatile thread_t *threadp;
	register volatile int *gcount;
	register volatile int *lcount;
	int mycpu;
	int s;

#if	1
	if (c_break_reset) {
		c_break_reset = 0;
		c_break_handoff = 0;
		c_break_thread = 0;
		c_break_gcount = 0;
		c_break_lcount = 0;
		c_break_ast = 0;
		c_break_ast_halt = 0;
		c_break_ast_terminate = 0;
		c_break_ast_block = 0;
		c_break_ast_network = 0;
		c_break_ast_netipc = 0;
	}
#endif

	mycpu = cpu_number();
	myprocessor = current_processor();
	threadp = (volatile thread_t *) &myprocessor->next_thread;
	lcount = (volatile int *) &myprocessor->runq.count;

	/*
	 * Don't mark cpu idle; we still like our pmap.
	 * XXX Will myprocessor->next_thread ever get set?
	 */

#if	MACH_HOST
	gcount = (volatile int *) &myprocessor->processor_set->runq.count;
#else	MACH_HOST
	gcount = (volatile int *) &default_pset.runq.count;
#endif	MACH_HOST

	/*
	 * Indicate that we are spinning on this queue.
	 *
	 * Nonzero norma_ipc_handoff_mqueue keeps other receivers away.
	 * Nonzero norma_ipc_handoff_msg keeps other senders away.
	 */
	assert(norma_ipc_handoff_mqueue == IMQ_NULL);
	norma_ipc_handoff_msg = 0;
	norma_ipc_handoff_max_size = max_size;
	norma_ipc_handoff_msg_size = 0;
	norma_ipc_handoff_mqueue = mqueue;

	/*
	 * Spin until reschedule or kmsg handoff. Do asts in the meantime.
	 */
	for (;;) {
		if (norma_ipc_handoff_msg != IKM_NULL) {
			c_break_handoff++;
			break;
		}
		if (need_ast[mycpu]) {
			if (need_ast[mycpu] & AST_HALT) {
				c_break_ast_halt++;
			}
			if (need_ast[mycpu] & AST_TERMINATE) {
				c_break_ast_terminate++;
			}
			if (need_ast[mycpu] & AST_BLOCK) {
				c_break_ast_block++;
			}
			if (need_ast[mycpu] & AST_NETWORK) {
				c_break_ast_network++;
			}
			if (need_ast[mycpu] & AST_NETIPC) {
				c_break_ast_netipc++;
			}
			c_break_ast++;
			break;
		}
		if (*threadp != (volatile thread_t) THREAD_NULL) {
			c_break_thread++;
			break;
		}
		if (*gcount != 0) {
			c_break_gcount++;
			break;
		}
		if (*lcount != 0) {
			c_break_lcount++;
			break;
		}
	}

	/*
	 * Before we release mqueue, we must check for a delivered message.
	 */
	s = sploff();
	if (norma_ipc_handoff_msg != IKM_NULL) {
		/*
		 * Someone left us a message,
		 * or an indication of a message that was too large.
		 */
		if (norma_ipc_handoff_msg_size) {
			*msg_size = norma_ipc_handoff_msg_size;
			norma_ipc_handoff_mqueue = IMQ_NULL;
			splon(s);
			return IKM_NULL;
		} else {
			register ipc_kmsg_t kmsg;
			kmsg = norma_ipc_handoff_msg;
			norma_ipc_handoff_mqueue = IMQ_NULL;
			splon(s);
			return kmsg;
		}
	}
	norma_ipc_handoff_mqueue = IMQ_NULL;
	splon(s);

	assert(ipc_kmsg_queue_first(&mqueue->imq_messages) == IKM_NULL);
	return IKM_NULL;
}
