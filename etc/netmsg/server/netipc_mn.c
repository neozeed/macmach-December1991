/* 
 * Mach Operating System
 * Copyright (c) 1990 Carnegie-Mellon University
 * All rights reserved.  The CMU software License Agreement specifies
 * the terms and conditions for use and redistribution.
 */
/*
 * HISTORY
 * $Log:	netipc_mn.c,v $
 * Revision 2.1  90/10/27  20:44:54  dpj
 * Moving under MACH3 source control
 * 
 * Revision 2.1.1.2.1.2  90/08/02  20:23:02  dpj
 * 	Use debug.netipc to control debugging, instead of a
 * 	compile-time constant.
 * 	[90/06/03  17:55:18  dpj]
 * 
 * 	Network access module using MACH_NET.
 * 	[90/06/03  17:38:03  dpj]
 * 
 * Revision 2.1.1.2.1.1  90/07/30  13:57:39  dpj
 * 	Added filter priority in netipc_bind().
 * 
 */
/*
 * netipc_mn.c
 *
 *
 */

#ifndef	lint
char                            netipc_mn_rcsid[] = "$Header: /afs/cs.cmu.edu/project/mach-2/rcs/pkg/netmsg/server/netipc_mn.c,v 2.1 90/10/27 20:44:54 dpj Exp $";
#endif not lint

/*
 * Functions to send and receive packets
 * using the IPC interface to the network
 * -- MACH_NET implementation.
 */


#include "netmsg.h"
#include "nm_defs.h"

#if	USE_NETIPC_MACHNET


#define NETIPC_DEBUG	debug.netipc

#include <mach.h>
#include <mach/message.h>

#include "debug.h"
#include "netipc.h"
#include "network.h"
#include "nm_extra.h"
#include "mem.h"
#include "transport.h"

#define NETIPC_MSG_ID	1959

extern kern_return_t netipc_receive_mn();
extern kern_return_t netipc_send_mn();
extern kern_return_t netipc_resend_mn();

typedef struct {
	netipc_channel_t	common;
	port_t			port;
} netipc_mn_channel_t, *netipc_mn_channel_ptr_t;

typedef struct {
	msg_header_t		msg_header;
	struct ip       	ip_header;
	struct udphdr		udp_header;
} netipc_mn_hdr_t, *netipc_mn_hdr_ptr_t;

#define	NETIPC_MN_HDR_FILL	(NETIPC_HDR_FILL - sizeof(netipc_mn_hdr_t))

typedef struct {
	netipc_control_t	control;
	char			hdr_fill[NETIPC_MN_HDR_FILL];
	netipc_mn_hdr_t		hdr;
	char			data[1];
} *netipc_mn_pkt_ptr_t;


/*
 * netipc_bind_mn
 *	Create a channel for future netipc operations.
 *
 * Parameters:
 *	udp_port : UDP port number (in host byte order).
 *	hdr_size : size of header for protocol above UDP.
 *	priority : priority for filter for incoming packets
 *
 * Results:
 *	channel for future netipc operations.
 *
 */
EXPORT netipc_channel_ptr_t netipc_bind_mn(udp_port, hdr_size, priority)
	unsigned short                  udp_port;
	int                             hdr_size;
	int				priority;
BEGIN("netipc_bind_mn")
	netipc_mn_channel_ptr_t         channel;
	netipc_mn_pkt_ptr_t		pkt_ptr;
	netipc_mn_hdr_ptr_t		hdr_ptr;
	kern_return_t                   kr;

	/*
	 * Initialize the channel data.
	 */
	MEM_ALLOC(channel,netipc_mn_channel_ptr_t,
				sizeof(netipc_mn_channel_t),FALSE);
	MEM_ALLOCOBJ(pkt_ptr,netipc_mn_pkt_ptr_t,MEM_TRBUFF);
	hdr_ptr = &pkt_ptr->hdr;
	channel->common.buffer_size = 1600 /* XXX */;
	channel->common.template_pkt = (netipc_ptr_t) pkt_ptr;
	channel->common.template_hdr = (char *) hdr_ptr;
	channel->common.template_offset = ((char *) hdr_ptr) -
							((char *) pkt_ptr);
	channel->common.template_size = sizeof(netipc_mn_hdr_t) + hdr_size;
	channel->common.receive = netipc_receive_mn;
	channel->common.send = netipc_send_mn;
	channel->common.resend = netipc_resend_mn;
	
	 /*
	  * Initialize the template
	  */
	hdr_ptr->msg_header.msg_simple = TRUE;
	hdr_ptr->msg_header.msg_type = MSG_TYPE_NORMAL;
	hdr_ptr->msg_header.msg_size = 0;
	hdr_ptr->msg_header.msg_id = NETIPC_MSG_ID;
	hdr_ptr->msg_header.msg_local_port = PORT_NULL;
	hdr_ptr->msg_header.msg_remote_port = task_self();
	hdr_ptr->ip_header.ip_hl = sizeof(struct ip) >> 2;/* 32 bit words */
	hdr_ptr->ip_header.ip_v = IPVERSION;
	hdr_ptr->ip_header.ip_tos = 0;
	hdr_ptr->ip_header.ip_len = 0;
	hdr_ptr->ip_header.ip_id = 0;
	hdr_ptr->ip_header.ip_off = 0;
	hdr_ptr->ip_header.ip_ttl = 30;	/* UDP_TTL */
	hdr_ptr->ip_header.ip_p = IPPROTO_UDP;
	hdr_ptr->ip_header.ip_sum = 0;
	hdr_ptr->ip_header.ip_src.s_addr = my_host_id;
	hdr_ptr->udp_header.uh_sport = htons(udp_port);
	hdr_ptr->udp_header.uh_dport = htons(udp_port);
	hdr_ptr->udp_header.uh_sum = 0;
	hdr_ptr->udp_header.uh_ulen = 0;
	bzero(channel->common.template_pkt->data,hdr_size);

	 /*
	  * Initialise the IPC interface to the network. 
	  */
	kr = port_allocate(task_self(), &channel->port);
	if (kr != KERN_SUCCESS) {
		ERROR((msg,
		"netipc_bind_mn.port_allocate fails, kr = %d.", kr));
		RETURN(NULL);
	}
	kr = port_set_backlog(task_self(), channel->port, 16);
	if (kr != KERN_SUCCESS) {
		ERROR((msg,
		"netipc_bind_mn.port_set_backlog fails, kr = %d.", kr));
		RETURN(NULL);
	}
	kr = netipc_listen(task_self(), 0, 0, 0, (int) (htons(udp_port)),
					IPPROTO_UDP, channel->port);
	if (kr != KERN_SUCCESS) {
		ERROR((msg,
		"netipc_bind_mn.netipc_listen fails, kr = %d.", kr));
		RETURN(NULL);
	}

	RETURN(&channel->common);
END


/*
 * netipc_receive_mn
 *	Receive a packet over the network using netmsg_receive.
 *	Reject a packet from ourself otherwise check its UDP checksum.
 *
 * Parameters:
 *
 * Results:
 *	NETIPC_BAD_UDP_CHECKSUM or the return code from netmsg_receive.
 *	pkt_ptr	: pointer to a buffer to hold the packet
 *
 */
EXPORT kern_return_t netipc_receive_mn(in_channel,in_pkt_ptr)
	netipc_channel_ptr_t	in_channel;
	netipc_ptr_t		in_pkt_ptr;
BEGIN("netipc_receive_mn")
	netipc_mn_channel_ptr_t	channel = (netipc_mn_channel_ptr_t) in_channel;
	netipc_mn_pkt_ptr_t	pkt_ptr = (netipc_mn_pkt_ptr_t) in_pkt_ptr;
	netipc_mn_hdr_ptr_t	hdr_ptr;
	kern_return_t		kr;

	hdr_ptr = &pkt_ptr->hdr;

	while (1) {
		hdr_ptr->msg_header.msg_size = channel->common.buffer_size;
		hdr_ptr->msg_header.msg_local_port = channel->port;
		kr = netmsg_receive(&hdr_ptr->msg_header);
		if (kr != RCV_SUCCESS) {
			DEBUG1(NETIPC_DEBUG, 3, 1034, kr);
			if (kr == RCV_TOO_LARGE) {
				/*
				 * Special case for when the incoming
				 * message is actually bigger than the
				 * buffer. How can this happen? XXX
				 */
				MEM_ALLOC(hdr_ptr,netipc_mn_hdr_ptr_t,
								20000,FALSE);
				hdr_ptr->msg_header.msg_size = 20000;
				hdr_ptr->msg_header.msg_local_port =
						channel->port;
				kr = netmsg_receive(&hdr_ptr->msg_header);
				ERROR((msg,
				"netipc_receive_mn(): huge message (%d)",kr));
				MEM_DEALLOC(hdr_ptr,20000);
			}
			RETURN(kr);
		}

		pkt_ptr->control.from = hdr_ptr->ip_header.ip_src.s_addr;
		pkt_ptr->control.to = 0;
		if (pkt_ptr->control.from == my_host_id) {
			continue;
		}

		if (hdr_ptr->udp_header.uh_sum != 0) {
			/*
			 * Not from ourself - check the UDP Checksum. 
			 */
			hdr_ptr->ip_header.ip_ttl = 0;
			hdr_ptr->ip_header.ip_sum = 
						hdr_ptr->udp_header.uh_ulen;
			hdr_ptr->udp_header.uh_sum = udp_checksum(
					(unsigned short *)
					&(hdr_ptr->ip_header.ip_ttl),
					(hdr_ptr->ip_header.ip_len - 8));
			if (hdr_ptr->udp_header.uh_sum != 0) {
				LOG0(TRUE, 5, 1032);
				RETURN(NETIPC_BAD_UDP_CHECKSUM);
			}
		}

		pkt_ptr->control.data_size = 
					ntohs(hdr_ptr->udp_header.uh_ulen) -
					sizeof(struct udphdr);

		RETURN(RCV_SUCCESS);
	}
END



/*
 * netipc_send_mn
 *	Calculate a UDP checksum for a packet if appropriate.
 *	Insert a new ip_id into the IP header.
 *	Send the packet over the network using msg_send.
 *
 * Parameters:
 *	pkt_ptr	: pointer to the packet to be sent.
 *	do_checksum : whether or not to compute a UDP checksum.
 *
 * Results:
 *	value returned by msg_send.
 *
 */
EXPORT kern_return_t netipc_send_mn(in_channel,in_pkt_ptr,do_checksum)
	netipc_channel_ptr_t	in_channel;
	netipc_ptr_t	 	in_pkt_ptr;
	int			do_checksum;
BEGIN("netipc_send_mn")
	netipc_mn_channel_ptr_t	channel = (netipc_mn_channel_ptr_t) in_channel;
	netipc_mn_pkt_ptr_t	pkt_ptr = (netipc_mn_pkt_ptr_t) in_pkt_ptr;
	netipc_mn_hdr_ptr_t	hdr_ptr;
	short			saved_ttl;
	msg_return_t		kr;

	hdr_ptr = &pkt_ptr->hdr;
	hdr_ptr->ip_header.ip_dst.s_addr = pkt_ptr->control.to;
	hdr_ptr->msg_header.msg_size = sizeof(netipc_mn_hdr_t) +
						pkt_ptr->control.data_size;
	hdr_ptr->ip_header.ip_len = sizeof(struct ip) +
						sizeof(struct udphdr) +
						pkt_ptr->control.data_size;
	hdr_ptr->udp_header.uh_ulen = htons(
			sizeof(struct udphdr) + pkt_ptr->control.data_size);

	if (do_checksum) {  
		/*
		 * Stuff the ip header with the right values
		 * for the UDP checksum. 
		 */
		saved_ttl = hdr_ptr->ip_header.ip_ttl;
		hdr_ptr->ip_header.ip_ttl = 0;
		hdr_ptr->ip_header.ip_sum = hdr_ptr->udp_header.uh_ulen;
		hdr_ptr->udp_header.uh_sum = 0;

		/*
		 * Calculate the checksum and restore the values in the
		 * ip header. 
		 */
		hdr_ptr->udp_header.uh_sum = udp_checksum(
			(unsigned short *) &(hdr_ptr->ip_header.ip_ttl),
			hdr_ptr->ip_header.ip_len - 8);
		hdr_ptr->ip_header.ip_ttl = saved_ttl;
		hdr_ptr->ip_header.ip_sum = 0;
	} else {
		/*
		 * Tell the destination to ignore the UDP checksum.
		 */
		hdr_ptr->udp_header.uh_sum = 0;
	}

	/*
	 * Insert a new ip id into the header and send the packet.
	 */
	hdr_ptr->ip_header.ip_id = last_ip_id ++;
	DEBUG4(NETIPC_DEBUG, 0, 1030, hdr_ptr->msg_header.msg_simple,
			hdr_ptr->msg_header.msg_size,
			hdr_ptr->msg_header.msg_id,
			hdr_ptr->msg_header.msg_type);
	kr = msg_send(&hdr_ptr->msg_header, SEND_TIMEOUT, 0);
	if (kr != KERN_SUCCESS) {
		DEBUG1(NETIPC_DEBUG, 0, 1031, kr);
	}

	RETURN(kr);

END


/*
 * netipc_resend_mn
 *	Resend a packet that was previously sent (and checksummed).
 *
 * Parameters:
 *	pkt_ptr	: pointer to the packet to be sent.
 *
 * Results:
 *	value returned by msg_send.
 *
 */
EXPORT kern_return_t netipc_resend_mn(in_channel,in_pkt_ptr)
	netipc_channel_ptr_t	in_channel;
	netipc_ptr_t	 	in_pkt_ptr;
BEGIN("netipc_resend_mn")
	netipc_mn_channel_ptr_t	channel = (netipc_mn_channel_ptr_t) in_channel;
	netipc_mn_pkt_ptr_t	pkt_ptr = (netipc_mn_pkt_ptr_t) in_pkt_ptr;
	netipc_mn_hdr_ptr_t	hdr_ptr;
	short			saved_ttl;
	msg_return_t		kr;

	hdr_ptr = &pkt_ptr->hdr;

	/*
	 * Insert a new ip id into the header and send the packet.
	 */
	hdr_ptr->ip_header.ip_id = last_ip_id ++;
	DEBUG4(NETIPC_DEBUG, 0, 1030, hdr_ptr->msg_header.msg_simple,
			hdr_ptr->msg_header.msg_size,
			hdr_ptr->msg_header.msg_id,
			hdr_ptr->msg_header.msg_type);
	kr = msg_send(&hdr_ptr->msg_header, SEND_TIMEOUT, 0);
	if (kr != KERN_SUCCESS) {
		DEBUG1(NETIPC_DEBUG, 0, 1031, kr);
	}

	RETURN(kr);

END

#endif	USE_NETIPC_MACHNET
