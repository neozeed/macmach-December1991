/* 
 * Mach Operating System
 * Copyright (c) 1989 Carnegie-Mellon University
 * Copyright (c) 1988 Carnegie-Mellon University
 * Copyright (c) 1987 Carnegie-Mellon University
 * All rights reserved.  The CMU software License Agreement specifies
 * the terms and conditions for use and redistribution.
 */
/*
 * HISTORY
 * $Log:	netipc.h,v $
 * Revision 2.1  90/10/27  20:44:49  dpj
 * Moving under MACH3 source control
 * 
 * Revision 1.7.1.1.1.2  90/08/02  20:22:50  dpj
 * 	Reorganized for new netipc interface and packet format.
 * 	[90/06/03  17:37:22  dpj]
 * 
 * Revision 1.7.1.1.1.1  90/07/30  13:56:28  dpj
 * 	Added filter priorities.
 * 
 * Revision 1.7  89/05/02  11:13:03  dpj
 * 	Fixed all files to conform to standard copyright/log format
 * 
 * 30-Jan-87  Robert Sansom (rds) at Carnegie Mellon University
 *	Increased NETIPC_MAX_MSG_SIZE by 8 to account for rounding up when sending secure packets.
 *
 * 22-Dec-86  Robert Sansom (rds) at Carnegie Mellon University
 *	Added external definitions of netipc_send and netipc_receive.
 *
 *  3-Nov-86  Robert Sansom (rds) at Carnegie Mellon University
 *	Started.
 *
 */
/*
 * netipc.h
 *
 *
 * $Header: /afs/cs.cmu.edu/project/mach-2/rcs/pkg/netmsg/server/netipc.h,v 2.1 90/10/27 20:44:49 dpj Exp $
 *
 */

/*
 * Definitions for IPC interface to the network.
 */


#ifndef	_NETIPC_
#define	_NETIPC_

#include <sys/types.h>
#include <netinet/in_systm.h>
#include <netinet/in.h>
#include <netinet/ip.h>
#include <netinet/udp.h>

#include "netmsg.h"
#include "nm_defs.h"

#define	NETIPC_HDR_FILL		400

typedef struct {
	netaddr_t		from;
	netaddr_t		to;
	int			data_size;
} netipc_control_t, *netipc_control_ptr_t;

typedef struct {
	netipc_control_t	control;
	char			hdr_fill[NETIPC_HDR_FILL];
} netipc_header_t, *netipc_header_ptr_t;

typedef struct {
	netipc_header_t		hdr;
	char			data[1];
} *netipc_ptr_t;

typedef struct {
	int			buffer_size;
	netipc_ptr_t		template_pkt;
	char			*template_hdr;
	int			template_offset;
	int			template_size;
	int			(*receive)();
	int			(*send)();
	int			(*resend)();
} netipc_channel_t, *netipc_channel_ptr_t;


#define netipc_template(channel)					\
	netipc_data(channel,channel->template_pkt)

#define	netipc_prepare_pkt(_channel,_pkt_ptr,_to,_data_size) {		\
	((netipc_ptr_t)(_pkt_ptr))->hdr.control.to = _to;		\
	((netipc_ptr_t)(_pkt_ptr))->hdr.control.from = 0;		\
	((netipc_ptr_t)(_pkt_ptr))->hdr.control.data_size = _data_size;	\
	bcopy((_channel)->template_hdr,					\
		((char *)(_pkt_ptr)) + (_channel)->template_offset,	\
		(_channel)->template_size);				\
}


#define	netipc_receive(channel,pkt_ptr)					\
	channel->receive(channel,pkt_ptr)

#define	netipc_send(channel,pkt_ptr,do_checksum)			\
	channel->send(channel,pkt_ptr,do_checksum)

#define	netipc_resend(channel,pkt_ptr)					\
	channel->resend(channel,pkt_ptr)


#define NETIPC_MAX_PACKET_SIZE		(1500)	/* Should be ETHERMTU???? */
#define NETIPC_MAX_DATA_SIZE		(NETIPC_MAX_PACKET_SIZE - (sizeof(struct ip) + sizeof(struct udphdr)))


/*
 * Functions exported by netipc.c
 */

extern				netipc_init();

extern netipc_channel_ptr_t	(*netipc_bind_op) ();

#define netipc_bind(udp_port,hdr_size,priority)	\
	(*netipc_bind_op)(udp_port,hdr_size,priority)
/*
unsigned short		udp_port;
int			hdr_size;
int			priority;
*/


/*
 * Priority levels for use in netipc_bind().
 */
#define	NETIPC_PRI_LOW		0
#define	NETIPC_PRI_HI		100


/*
 * Error codes.
 */
#define NETIPC_BAD_UDP_CHECKSUM	-1

#endif	_NETIPC_
