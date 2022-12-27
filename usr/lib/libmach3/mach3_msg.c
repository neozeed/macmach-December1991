/* 
 * Mach Operating System
 * Copyright (c) 1989 Carnegie-Mellon University
 * All rights reserved.  The CMU software License Agreement specifies
 * the terms and conditions for use and redistribution.
 */
/*
 * HISTORY
 * $Log:	mach3_msg.c,v $
 * Revision 2.2  90/07/26  12:37:30  dpj
 * 	First version
 * 	[90/07/24  14:28:34  dpj]
 * 
 */

#include <mach/kern_return.h>
#include <mach/message.h>

extern msg_return_t msg_send_trap();
extern msg_return_t msg_receive_trap();
extern msg_return_t msg_rpc_trap();

msg_return_t	msg_send(header, option, timeout)
	msg_header_t	*header;
	msg_option_t	option;
	msg_timeout_t	timeout;
{
	register
	msg_return_t	result;


	result = msg_send_trap(header, option, header->msg_size, timeout);
	if (result == SEND_SUCCESS)
		return result;

	if (result == KERN_INVALID_ARGUMENT) {
#if	MACH3
		return result;
#else	MACH3
		while ((result = msg_send_old(header, option, timeout))
		       			== SEND_INTERRUPTED)
			if (option & SEND_INTERRUPT)
				break;
#endif	MACH3
	} else if ((result == SEND_INTERRUPTED) &&
		 !(option & SEND_INTERRUPT))
		do
			result = msg_send_trap(header, option,
					       header->msg_size, timeout);
		while (result == SEND_INTERRUPTED);

	return result;
}

msg_return_t	msg_receive(header, option, timeout)
	msg_header_t	*header;
	msg_option_t	option;
	msg_timeout_t	timeout;
{
	register
	msg_return_t	result;

	result = msg_receive_trap(header, option, header->msg_size,
				  header->msg_local_port, timeout);
	if (result == RCV_SUCCESS)
		return result;

	if (result == KERN_INVALID_ARGUMENT) {
#if	MACH3
		return result;
#else	MACH3
		while ((result = msg_receive_old(header, option, timeout))
					== RCV_INTERRUPTED)
			if (option & RCV_INTERRUPT)
				break;
#endif	MACH3
	} else if ((result == RCV_INTERRUPTED) &&
		 !(option & RCV_INTERRUPT))
		do
			result = msg_receive_trap(header, option,
						  header->msg_size,
						  header->msg_local_port,
						  timeout);
		while (result == RCV_INTERRUPTED);

	return result;
}

msg_return_t	msg_rpc(header, option, rcv_size, send_timeout, rcv_timeout)
	msg_header_t	*header;
	msg_option_t	option;
	msg_size_t	rcv_size;
	msg_timeout_t	send_timeout;
	msg_timeout_t	rcv_timeout;
{
	register
	msg_return_t	result;
	
	result = msg_rpc_trap(header, option, header->msg_size,
			      rcv_size, send_timeout, rcv_timeout);
	if (result == RPC_SUCCESS)
		return result;

	if (result == KERN_INVALID_ARGUMENT) {
#if	MACH3
		return result;
#else	MACH3
		while ((result = msg_rpc_old(header, option, rcv_size,
					      send_timeout, rcv_timeout))
					== SEND_INTERRUPTED)
			if (option & SEND_INTERRUPT)
				return result;
		if ((result == RCV_INTERRUPTED) &&
		    !(option & RCV_INTERRUPT)) {
			header->msg_size = rcv_size;
			do
				result = msg_receive_old(header, option,
							 rcv_timeout);
			while (result == RCV_INTERRUPTED);
		}

		return result;
#endif	MACH3
	} else if ((result == SEND_INTERRUPTED) &&
		   !(option & SEND_INTERRUPT)) {
		do
			result = msg_rpc_trap(header, option,
					      header->msg_size, rcv_size,
					      send_timeout, rcv_timeout);
		while (result == SEND_INTERRUPT);
	}

	if ((result == RCV_INTERRUPTED) &&
	    !(option & RCV_INTERRUPT))
		do
			result = msg_receive_trap(header, option, rcv_size,
						  header->msg_local_port,
						  rcv_timeout);
		while (result == RCV_INTERRUPTED);

	return result;
}
