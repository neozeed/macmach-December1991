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
 * $Log:	msg.c,v $
 * Revision 2.4  91/05/14  17:54:38  mrt
 * 	Correcting copyright
 * 
 * Revision 2.3  91/02/14  14:18:33  mrt
 * 	Added new Mach copyright
 * 	[91/02/13  12:44:42  mrt]
 * 
 * Revision 2.2  90/06/02  15:13:01  rpd
 * 	Converted to new IPC.
 * 	[90/03/26  23:28:31  rpd]
 * 
 * Revision 2.1  89/08/03  17:05:52  rwd
 * Created.
 * 
 * 21-Oct-88  Richard Draves (rpd) at Carnegie-Mellon University
 *	Added msg_send wrapper, which handles SEND_INTERRUPT.
 *	Fixed bug in msg_rpc wrapper; it gave the wrong size to	msg_receive_.
 *	Converted to first try the new *_trap calls and fall back on
 *	the (renamed) *_old calls if they don't work.
 *
 * 19-May-87  Mary Thompson (mrt) at Carnegie-Mellon University
 *	Fixed the test for interupts in msg_rpc_.
 *	(Copied from mwyoung's version.)
 */

#include <mach/port.h>
#include <mach/message.h>

mach_msg_return_t
mach_msg(msg, option, send_size, rcv_size, rcv_name, timeout, notify)
	mach_msg_header_t *msg;
	mach_msg_option_t option;
	mach_msg_size_t send_size;
	mach_msg_size_t rcv_size;
	mach_port_t rcv_name;
	mach_msg_timeout_t timeout;
	mach_port_t notify;
{
	mach_msg_return_t mr;

	/*
	 * Consider the following cases:
	 *	1) Errors in pseudo-receive (eg, MACH_SEND_INTERRUPTED
	 *	plus special bits).
	 *	2) Use of MACH_SEND_INTERRUPT/MACH_RCV_INTERRUPT options.
	 *	3) RPC calls with interruptions in one/both halves.
	 */

	mr = mach_msg_trap(msg, option, send_size, rcv_size, rcv_name,
			   timeout, notify);
	if (mr == MACH_MSG_SUCCESS)
		return MACH_MSG_SUCCESS;

	if ((option & MACH_SEND_INTERRUPT) == 0)
		while (mr == MACH_SEND_INTERRUPTED)
			mr = mach_msg_trap(msg, option,
					   send_size, rcv_size, rcv_name,
					   timeout, notify);

	if ((option & MACH_RCV_INTERRUPT) == 0)
		while (mr == MACH_RCV_INTERRUPTED)
			mr = mach_msg_trap(msg, option &~ MACH_SEND_MSG,
					   0, rcv_size, rcv_name,
					   timeout, notify);

	return mr;
}

mach_msg_return_t
mach_msg_send(msg)
	mach_msg_header_t *msg;
{
	return mach_msg(msg, MACH_SEND_MSG,
			msg->msgh_size, 0, MACH_PORT_NULL,
			MACH_MSG_TIMEOUT_NONE, MACH_PORT_NULL);
}

mach_msg_return_t
mach_msg_receive(msg)
	mach_msg_header_t *msg;
{
	return mach_msg(msg, MACH_RCV_MSG,
			0, msg->msgh_size, msg->msgh_local_port,
			MACH_MSG_TIMEOUT_NONE, MACH_PORT_NULL);
}
