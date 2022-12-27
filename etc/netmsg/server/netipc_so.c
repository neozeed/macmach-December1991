/* 
 * Mach Operating System
 * Copyright (c) 1990 Carnegie-Mellon University
 * All rights reserved.  The CMU software License Agreement specifies
 * the terms and conditions for use and redistribution.
 */
/*
 * HISTORY
 * $Log:	netipc_so.c,v $
 * Revision 2.1  90/10/27  20:44:57  dpj
 * Moving under MACH3 source control
 * 
 * Revision 2.1.1.1.1.3  90/10/21  21:52:48  dpj
 * 	Enabled socket broadcasting.
 * 
 * Revision 2.1.1.1.1.2  90/08/02  20:23:06  dpj
 * 	Network access module using plain UNIX UDP sockets.
 * 	[90/06/03  17:55:52  dpj]
 * 
 * Revision 2.1.1.1.1.1  90/07/30  13:58:39  dpj
 * 	Added filter priority in netipc_bind().
 * 
 */
/*
 * netipc_so.c
 *
 *
 */

#ifndef	lint
char                            netipc_so_rcsid[] = "$Header: /afs/cs.cmu.edu/project/mach-2/rcs/pkg/netmsg/server/netipc_so.c,v 2.1 90/10/27 20:44:57 dpj Exp $";
#endif not lint

/*
 * Functions to send and receive packets
 * using sockets.
 */


#include "netmsg.h"
#include "nm_defs.h"

#if	USE_NETIPC_SOCKET


#define NETIPC_DEBUG	debug.netipc

#include <mach.h>
#include <mach/message.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>

#include "debug.h"
#include "netipc.h"
#include "network.h"
#include "nm_extra.h"
#include "mem.h"
#include "transport.h"

extern int	errno;

extern kern_return_t netipc_receive_so();
extern kern_return_t netipc_send_so();
extern kern_return_t netipc_resend_so();

typedef struct {
	netipc_channel_t	common;
	int			socket;
	short			udp_port;
} netipc_so_channel_t, *netipc_so_channel_ptr_t;

typedef struct {
	int			dummy;
} netipc_so_hdr_t, *netipc_so_hdr_ptr_t;

#define	NETIPC_SO_HDR_FILL	(NETIPC_HDR_FILL - sizeof(netipc_so_hdr_t))

typedef struct {
	netipc_control_t	control;
	char			hdr_fill[NETIPC_SO_HDR_FILL];
	netipc_so_hdr_t		hdr;
	char			data[1];
} *netipc_so_pkt_ptr_t;


/*
 * netipc_bind_so
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
EXPORT netipc_channel_ptr_t netipc_bind_so(udp_port, hdr_size, priority)
	unsigned short                  udp_port;
	int                             hdr_size;
	int				priority;
BEGIN("netipc_bind_so")
	netipc_so_channel_ptr_t         channel;
	netipc_so_pkt_ptr_t		pkt_ptr;
	netipc_so_hdr_ptr_t		hdr_ptr;
	int				on = 1;
	int				rv;
	struct sockaddr_in		sin;

	/*
	 * Initialize the channel data.
	 */
	MEM_ALLOC(channel,netipc_so_channel_ptr_t,
				sizeof(netipc_so_channel_t),FALSE);
	MEM_ALLOCOBJ(pkt_ptr,netipc_so_pkt_ptr_t,MEM_TRBUFF);
	hdr_ptr = &pkt_ptr->hdr;
	channel->common.buffer_size = 1600 /* XXX */;
	channel->common.template_pkt = (netipc_ptr_t) pkt_ptr;
	channel->common.template_hdr = (char *) hdr_ptr;
	channel->common.template_offset = ((char *) hdr_ptr) -
							((char *) pkt_ptr);
	channel->common.template_size = sizeof(netipc_so_hdr_t) + hdr_size;
	channel->common.receive = netipc_receive_so;
	channel->common.send = netipc_send_so;
	channel->common.resend = netipc_resend_so;
	channel->udp_port = udp_port;
	
	 /*
	  * Initialize the template
	  */
	hdr_ptr->dummy = 0;
	bzero(channel->common.template_pkt->data,hdr_size);

	/*
	 *  Create and bind a socket.
	 */
	channel->socket = socket(AF_INET, SOCK_DGRAM, 0);
	if (channel->socket < 0) {
		ERROR((msg,"netipc_bind_so.socket failed, errno=%d",errno));
		RETURN(NULL);
	}
	rv = setsockopt(channel->socket,SOL_SOCKET,SO_BROADCAST,
							&on,sizeof(int));
	if (rv < 0) {
		ERROR((msg,
"netipc_bind_so.setsockopt(SO_BROADCAST) failed, errno=%d -- continuing",
								errno));
	}

	sin.sin_family = AF_INET;
	sin.sin_port = htons(udp_port);
	sin.sin_addr.s_addr = 0;	/* XXX */
	bzero(sin.sin_zero, sizeof(sin.sin_zero));

	rv = bind(channel->socket, &sin, sizeof(sin));
	if (rv < 0) {
		ERROR((msg,"netipc_bind_so.bind failed, errno=%d",errno));
		close(channel->socket);
		RETURN(NULL);
	}

	RETURN(&channel->common);
END


/*
 * netipc_receive_so
 *	Receive a packet over the network using recvfrom..
 *	Reject a packet from ourself.
 *
 * Parameters:
 *
 * Results:
 *	RCV_SUCCESS or KERN_FAILURE.
 *	pkt_ptr	: pointer to a buffer to hold the packet
 *
 */
EXPORT kern_return_t netipc_receive_so(in_channel,in_pkt_ptr)
	netipc_channel_ptr_t	in_channel;
	netipc_ptr_t		in_pkt_ptr;
BEGIN("netipc_receive_so")
	netipc_so_channel_ptr_t	channel = (netipc_so_channel_ptr_t) in_channel;
	netipc_so_pkt_ptr_t	pkt_ptr = (netipc_so_pkt_ptr_t) in_pkt_ptr;
	int			rv, fromlen;
	struct sockaddr_in	from;

	while (1) {
		fromlen = sizeof(from);
		rv = recvfrom(channel->socket,pkt_ptr->data,
				channel->common.buffer_size,0,&from,&fromlen);
		if (rv < 0) {
			ERROR((msg,
			"netipc_receive_so.recvfrom failed, errno=%d",errno));
			RETURN(KERN_FAILURE);	/* XXX */
		}
		pkt_ptr->control.from = from.sin_addr.s_addr;
		pkt_ptr->control.to = 0;
		if (pkt_ptr->control.from == my_host_id) {
			continue;
		}
		pkt_ptr->control.data_size = rv;

		RETURN(RCV_SUCCESS);
	}
END



/*
 * netipc_send_so
 *	Send the packet over the network using sendto.
 *
 * Parameters:
 *	pkt_ptr	: pointer to the packet to be sent.
 *	do_checksum : whether or not to compute a UDP checksum (ignored).
 *
 * Results:
 *	SEND_SUCCESS or KERN_FAILURE.
 *
 */
EXPORT kern_return_t netipc_send_so(in_channel,in_pkt_ptr,do_checksum)
	netipc_channel_ptr_t	in_channel;
	netipc_ptr_t	 	in_pkt_ptr;
	int			do_checksum;
BEGIN("netipc_send_so")
	netipc_so_channel_ptr_t	channel = (netipc_so_channel_ptr_t) in_channel;
	netipc_so_pkt_ptr_t	pkt_ptr = (netipc_so_pkt_ptr_t) in_pkt_ptr;
	int			len, rv;
	struct sockaddr_in	sin;
	static boolean_t	did_warning = FALSE;

	sin.sin_family = AF_INET;
	sin.sin_port = htons(channel->udp_port);
	sin.sin_addr.s_addr = pkt_ptr->control.to;
	bzero(sin.sin_zero, sizeof(sin.sin_zero));

	rv = sendto(channel->socket,pkt_ptr->data,
				pkt_ptr->control.data_size,0,&sin,sizeof(sin));
	if (rv < 0) {
		if (sin.sin_addr.s_addr == broadcast_address) {
			if (! did_warning) {
				ERROR((msg,
					"netipc_send_so: broadcast failure"));
				did_warning = TRUE;
			}
		} else {
			ERROR((msg,
			"netipc_send_so.sendto failed, errno=%d",errno));
			RETURN(KERN_FAILURE);
		}
	}

	RETURN(SEND_SUCCESS);

END


/*
 * netipc_resend_so
 *	Resend a packet that was previously sent (and checksummed).
 *
 * Parameters:
 *	pkt_ptr	: pointer to the packet to be sent.
 *
 * Results:
 *	value returned by msg_send.
 *
 */
EXPORT kern_return_t netipc_resend_so(in_channel,in_pkt_ptr)
	netipc_channel_ptr_t	in_channel;
	netipc_ptr_t	 	in_pkt_ptr;
BEGIN("netipc_resend_so")
	RETURN(netipc_send_so(in_channel,in_pkt_ptr,FALSE));
END

#endif	USE_NETIPC_SOCKET
