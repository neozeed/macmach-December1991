/* 
 * Mach Operating System
 * Copyright (c) 1989 Carnegie-Mellon University
 * All rights reserved.  The CMU software License Agreement specifies
 * the terms and conditions for use and redistribution.
 */
/*
 * HISTORY
 * $Log:	ux_server_loop.c,v $
 * Revision 2.8  90/10/25  15:07:46  rwd
 * 	Add STACK_GROWTH_UP case for u_offset.
 * 	[90/10/11            rwd]
 * 
 * Revision 2.7  90/08/06  15:35:31  rwd
 * 	Change to reflect change in cthread_msg interface.
 * 	[90/06/06            rwd]
 * 
 * Revision 2.6  90/06/19  23:16:20  rpd
 * 	Modified the server loop to allow simpleroutines.
 * 	[90/06/09            rpd]
 * 
 * Revision 2.5  90/06/02  15:29:01  rpd
 * 	If a receive or rpc fails unexpectedly, panic.
 * 	[90/05/12            rpd]
 * 	Converted to new IPC.
 * 	Fixed bug checking return code in reply message.
 * 	Deallocate reply port if it isn't used.
 * 	[90/03/26  20:28:17  rpd]
 * 
 * Revision 2.4  90/01/19  14:38:21  rwd
 * 	Change to use cthread_msg_rpc which will do an rpc when
 * 	possible.
 * 	[89/12/20            rwd]
 * 	Allocate uarea on stack.
 * 	[89/12/14            rwd]
 * 
 * Revision 2.3  89/12/08  20:17:34  rwd
 * 	Changed to use new cthread_receive call.
 * 	[89/10/23            rwd]
 * 
 * Revision 2.2  89/10/17  11:27:52  rwd
 * 	Undo rfr's single-server-loop changes.
 * 	[89/09/21            dbg]
 * 
 * Revision 2.1  89/08/04  14:50:39  rwd
 * Created.
 * 
 *  6-Apr-89  David Golub (dbg) at Carnegie-Mellon University
 *	Split into different files.
 *
 *  6-Jan-89  David Golub (dbg) at Carnegie-Mellon University
 *	Created.
 *
 */
/*
 * Server thread utilities and main loop.
 */
#include <uxkern/import_mach.h>

#include <sys/param.h>
#include <sys/types.h>
#include <sys/user.h>
#include <sys/proc.h>

mach_port_t ux_server_port_set = MACH_PORT_NULL;
int	u_offset = 0;

/*
 * Number of server threads available to handle user messages.
 */
struct mutex	ux_server_thread_count_lock = MUTEX_INITIALIZER;
int		ux_server_thread_count = 0;
int		ux_server_thread_min = 4;
int		ux_server_receive_min = 2;
int		ux_server_receive_max = 6;
int		ux_server_thread_max = 80;
int		ux_server_stack_size = 4096*16;
/* This must be at least 5 (wired) + 1 (running) + receive_max */
int		ux_server_max_kernel_threads = 12;

void ux_create_single_server_thread(); /* forward */

extern	int cthread_stack_size;
extern	int cthread_stack_mask;

void
ux_server_init()
{
	mach_port_t first_port;
	ur_cthread_t tmp;
	kern_return_t kr;

	kr = mach_port_allocate(mach_task_self(), MACH_PORT_RIGHT_PORT_SET,
				&ux_server_port_set);

	cthread_set_kernel_limit(ux_server_max_kernel_threads);

/*
	tmp = ur_cthread_self();
	cthread_stack_size  = ux_server_stack_size;
	cthread_stack_mask = ~(cthread_stack_size - 1);
	ur_cthread_self() = tmp;
*/
}

void
ux_server_add_port(port)
	mach_port_t	port;
{
	(void) mach_port_move_member(mach_task_self(),
				     port, ux_server_port_set);
}

void
ux_server_remove_port(port)
	mach_port_t	port;
{
	(void) mach_port_move_member(mach_task_self(), port, MACH_PORT_NULL);
}

any_t
ux_thread_bootstrap(arg)
	any_t	arg;
{
	register int	routine_addr = (int)arg;
	cthread_fn_t	real_routine = (cthread_fn_t)routine_addr;
	register int calc;

	struct uthread	uthread;

	bzero((char *)&uthread, sizeof(struct uthread));
#ifdef	STACK_GROWTH_UP
	calc = (int)&uthread - (int)(cthread_sp() & cthread_stack_mask);
#else	STACK_GROWTH_UP
	calc = (int)&uthread - (int)(cthread_sp() | cthread_stack_mask);
#endif	STACK_GROWTH_UP
	if (u_offset) {
	  if (u_offset != calc)
	    panic("different u_offsets");
	} else {
	  u_offset = calc;
	}
/*	cthread_set_data(cthread_self(), (any_t)&uthread);*/

	mutex_init(&uthread.uu_lock);
	condition_init(&uthread.uu_condition);

	return ((*real_routine)((any_t)0));
}

/*
 * Create a thread with a UX u-area.
 */
void
ux_create_thread(routine)
	cthread_fn_t	routine;
{
	int		routine_addr = (int)routine;

	cthread_detach(cthread_fork(ux_thread_bootstrap,
				    (any_t)routine_addr));
}

any_t	ux_server_loop();	/* forward */

void
ux_create_server_thread()
{
	ux_create_thread(ux_server_loop);
}

void
ux_server_thread_busy()
{
	cthread_msg_busy(ux_server_port_set,
			 ux_server_receive_min,
			 ux_server_receive_max);
	mutex_lock(&ux_server_thread_count_lock);
	if (--ux_server_thread_count < ux_server_thread_min) {
	    mutex_unlock(&ux_server_thread_count_lock);
	    ux_create_server_thread();
	}
	else {
	    mutex_unlock(&ux_server_thread_count_lock);
	}
}

void
ux_server_thread_active()
{
	cthread_msg_active(ux_server_port_set,
			 ux_server_receive_min,
			 ux_server_receive_max);
	mutex_lock(&ux_server_thread_count_lock);
	++ux_server_thread_count;
	mutex_unlock(&ux_server_thread_count_lock);
}

#define	ux_server_thread_check() \
	(ux_server_thread_count > ux_server_thread_max)

/*
 * Main loop of UX server.
 */
/*ARGSUSED*/
any_t
ux_server_loop(arg)
	any_t	arg;
{
	register kern_return_t	ret;

	union request_msg {
	    mach_msg_header_t	hdr;
	    mig_reply_header_t	death_pill;
	    char		space[8192];
	} msg_buffer_1, msg_buffer_2;

	mach_msg_header_t * request_ptr;
	mig_reply_header_t * reply_ptr;
	mach_msg_header_t * tmp;

	char	name[64];

	sprintf(name, "ux server thread %x", cthread_self());
	cthread_set_name(cthread_self(), name);

	ux_server_thread_active();

	request_ptr = &msg_buffer_1.hdr;
	reply_ptr = &msg_buffer_2.death_pill;

	do {
	    ret = cthread_mach_msg(request_ptr, MACH_RCV_MSG,
				   0, sizeof msg_buffer_1, ux_server_port_set,
				   MACH_MSG_TIMEOUT_NONE, MACH_PORT_NULL,
				   ux_server_receive_min,
				   ux_server_receive_max);
	    if (ret != MACH_MSG_SUCCESS)
		panic("ux_server_loop: receive", ret);
	    while (ret == MACH_MSG_SUCCESS) {
		if (!bsd_1_server(request_ptr, &reply_ptr->Head))
		if (!ux_generic_server(request_ptr, &reply_ptr->Head))
		if (!exc_server(request_ptr, &reply_ptr->Head))
		if (!bsd_2_server(request_ptr, &reply_ptr->Head))
		{}

		if (reply_ptr->Head.msgh_remote_port == MACH_PORT_NULL) {
		    /* no reply port, just get another request */
		    break;
		}

		if (reply_ptr->RetCode == MIG_NO_REPLY) {
		    /* deallocate reply port right */
		    (void) mach_port_deallocate(mach_task_self(),
					reply_ptr->Head.msgh_remote_port);
		    break;
		}

		ret = cthread_mach_msg(&reply_ptr->Head,
				       MACH_SEND_MSG|MACH_RCV_MSG,
				       reply_ptr->Head.msgh_size,
				       sizeof msg_buffer_2,
				       ux_server_port_set,
				       MACH_MSG_TIMEOUT_NONE,
				       MACH_PORT_NULL,
				       ux_server_receive_min,
				       ux_server_receive_max);
		if (ret != MACH_MSG_SUCCESS) {
		    if (ret == MACH_SEND_INVALID_DEST) {
			/* deallocate reply port right */
			/* XXX should destroy entire reply msg */
			(void) mach_port_deallocate(mach_task_self(),
					reply_ptr->Head.msgh_remote_port);
		    } else
			panic("ux_server_loop: rpc", ret);
		}

		tmp = request_ptr;
		request_ptr = (mach_msg_header_t *) reply_ptr;
		reply_ptr = (mig_reply_header_t *) tmp;
	    }

	} while (!ux_server_thread_check());

	ux_server_thread_busy();

        printf("Server loop done: %s\n",name);

	return ((any_t)0);	/* exit */
}

