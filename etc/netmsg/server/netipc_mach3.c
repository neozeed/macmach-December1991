/* 
 * Mach Operating System
 * Copyright (c) 1990 Carnegie-Mellon University
 * All rights reserved.  The CMU software License Agreement specifies
 * the terms and conditions for use and redistribution.
 */
/*
 * HISTORY
 * $Log:	netipc_mach3.c,v $
 * Revision 2.1  90/10/27  20:44:52  dpj
 * Moving under MACH3 source control
 * 
 * Revision 2.1.1.1.1.5  90/10/21  21:52:10  dpj
 * 	Cleaned-up computation of the maximum packet size.
 * 	Added definition of struct ether_addr if not available elsewhere.
 * 	Made the ARP entries for self and broadcast be permanent.
 * 
 * Revision 2.1.1.1.1.4  90/10/07  13:46:47  dpj
 * 	Upgraded for new IPC, use of libmach3 and external ARP service.
 * 	Removed hacks for incorrect kernel packet filter -- thery are no
 * 	longer necessary. 
 * 
 * Revision 2.1.1.1.1.3  90/08/15  14:59:07  dpj
 * 	Use mach3_syscall() and mach_device_server_port() from libmach3.
 * 
 * Revision 2.1.1.1.1.2  90/08/02  20:22:56  dpj
 * 	Network access module for Mach 3.0 system.
 * 	Preliminary version.
 * 	[90/06/03  17:54:45  dpj]
 * 
 * Revision 2.1.1.1.1.1  90/07/30  13:57:11  dpj
 * 	Cleaned-up some.
 * 	Added filter priorities.
 * 	Added static ARP entries for a few OSF hosts.
 * 
 */
/*
 * netipc_mach3.c
 *
 *
 */

#ifndef	lint
char                            netipc_mach3_rcsid[] = "$Header: /afs/cs.cmu.edu/project/mach-2/rcs/pkg/netmsg/server/netipc_mach3.c,v 2.1 90/10/27 20:44:52 dpj Exp $";
#endif not lint

/*
 * Functions to send and receive packets
 * using the IPC interface to the network
 * -- MACH 3 packet filter implementation.
 *
 * XXX For now, this is a quick hack. This code only works with Ethernet,
 * XXX and only supports a single network interface.
 */


#include "netmsg.h"
#include "nm_defs.h"

#if	USE_NETIPC_MACH3


#define NETIPC_DEBUG	debug.netipc

#include <mach.h>
#include <mach/message.h>
#include <mach_privileged_ports.h>

#include "debug.h"
#include "netipc.h"
#include "network.h"
#include "nm_extra.h"
#include "mem.h"
#include "transport.h"

#include <sys/socket.h>
#include <net/if.h>
#include <netinet/if_ether.h>
#include <device/net_status.h>


/*
 * Definitions for ethernet addresses.
 */
#ifndef	_ETHER_ADDR_
#define	_ETHER_ADDR_
struct ether_addr {
	char	ether_addr_octet[6];
};
#endif	_ETHER_ADDR_
#define	ETHER_COPY(dst,src)						\
	bcopy(&src,&dst,sizeof(struct ether_addr))
struct ether_addr ether_bcast = { 0xff, 0xff, 0xff, 0xff, 0xff, 0xff };


extern kern_return_t netipc_receive_mach3();
extern kern_return_t netipc_send_mach3();
extern kern_return_t netipc_resend_mach3();


typedef struct {
	netipc_channel_t	common;
	struct mutex		ether_cache_lock;
	struct ether_header	ether_cache_hdr;
	netaddr_t		ether_cache_ip_addr;
	port_t			if_port;
	port_t			reply_port;
} netipc_mach3_channel_t, *netipc_mach3_channel_ptr_t;


typedef struct {
	mach_msg_header_t	msg_header;
	mach_msg_type_t		header_type;
	char			header[NET_HDW_HDR_MAX];
	mach_msg_type_t		packet_type;
	struct packet_header	packet_header;
	struct ip       	ip_header;
	struct udphdr		udp_header;
} netipc_mach3_in_hdr_t, *netipc_mach3_in_hdr_ptr_t;

#define	NETIPC_MACH3_IN_HDR_FILL	(NETIPC_HDR_FILL - sizeof(netipc_mach3_in_hdr_t))

typedef struct {
	netipc_control_t	control;
	char			hdr_fill[NETIPC_MACH3_IN_HDR_FILL];
	netipc_mach3_in_hdr_t	hdr;
	char			data[1];
} netipc_mach3_in_pkt_t, *netipc_mach3_in_pkt_ptr_t;

typedef struct {
	struct ip       	ip_header;
	struct udphdr		udp_header;
} netipc_mach3_out_hdr_t, *netipc_mach3_out_hdr_ptr_t;

#define	NETIPC_MACH3_OUT_HDR_FILL	(NETIPC_HDR_FILL - sizeof(netipc_mach3_out_hdr_t))

typedef struct {
	netipc_control_t	control;
	char			hdr_fill[NETIPC_MACH3_OUT_HDR_FILL];
	netipc_mach3_out_hdr_t	hdr;
	char			data[1];
} *netipc_mach3_out_pkt_ptr_t;

#define	NETIPC_MACH3_MAX_DATA_SIZE	( MEM_TRBUFF.obj_size \
					- sizeof(netipc_mach3_in_pkt_t) + 1 )
#define	NETIPC_MACH3_MAX_PKT_SIZE	( NETIPC_MACH3_MAX_DATA_SIZE \
					+ sizeof(struct ether_header) \
					+ sizeof(struct ip) \
					+ sizeof(struct udphdr))


/*
 * Global variables obtained from the command line.
 */
extern char	*mach3_if_name;
extern char	*mach3_net_server_name;


/*
 * Global variables for this module.
 */
port_t	mach3_if_port = PORT_NULL;
port_t	mach3_net_server_port = PORT_NULL;


/*
 * Local ARP table.
 */

struct ip_addr {
	char			b1;
	char			b2;
	char			b3;
	char			b4;
};

struct netipc_arp_entry {
	struct ip_addr		ip_addr;
	struct ether_addr	ether_addr;
	short			pad;
};

struct netipc_arp_entry		netipc_arp_table [] = {

{{0, 0, 0, 0}, {0, 0, 0, 0, 0, 0}},			/* own */
{{0, 0, 0, 0}, {0, 0, 0, 0, 0, 0}},			/* broadcast */

{{0, 0, 0, 0}, {0, 0, 0, 0, 0, 0}},			/* free */
{{0, 0, 0, 0}, {0, 0, 0, 0, 0, 0}},			/* free */
{{0, 0, 0, 0}, {0, 0, 0, 0, 0, 0}},			/* free */
{{0, 0, 0, 0}, {0, 0, 0, 0, 0, 0}},			/* free */

#if	USE_STATIC_ARP
{{128, 2, 250, 152}, {0x0, 0x0, 0x1c, 0x0, 0x1, 0x74}},	   /* mrbig */
{{128, 2, 250, 252}, {0x8, 0x0, 0x2b, 0xe, 0xf6, 0xb8}},   /* testarossa */
{{128, 2, 209, 82},  {0x8, 0x0, 0x2b, 0x14, 0xac, 0x77}},  /* countach */
{{128, 2, 209, 53},  {0x8, 0x0, 0x2b, 0x10, 0x75, 0x23}},  /* rpd */
{{128, 2, 209, 52},  {0x8, 0x0, 0x2b, 0xf, 0x57, 0xb0}},   /* jsb */
{{128, 2, 250, 194}, {0x8, 0x0, 0x2b, 0x14, 0x9f, 0x5d}},  /* mrt */
{{128, 2, 250, 16},  {0xaa, 0x0, 0x3, 0x1, 0x19, 0x54}},   /* wb1 */
{{128, 2, 250, 236}, {0x8, 0x0, 0x2b, 0xb, 0xb9, 0xc4}},   /* r2d2 */
{{128, 2, 209, 114}, {0x8, 0x0, 0x2b, 0x14, 0x9f, 0x66}},  /* scarecrow */
{{128, 2, 0, 0},     {0xff, 0xff, 0xff, 0xff, 0xff, 0xff}},/* broadcast */
{{128, 2, 255, 255}, {0xff, 0xff, 0xff, 0xff, 0xff, 0xff}},/* broadcast */
{{130, 105, 2, 141}, {0x0, 0xaa, 0x0, 0x3, 0x4, 0x74}},    /* sunset.osf.org */
{{130, 105, 2, 140}, {0x0, 0xaa, 0x0, 0x3, 0x5, 0x1}},     /* ms.osf.org */
{{130, 105, 2, 142}, {0x2, 0x60, 0x8c, 0x69, 0x86, 0x6}},  /* sunshine.osf */
{{130, 105, 2, 144}, {0x2, 0x60, 0x8c, 0x79, 0x19, 0x24}}, /* suntan.osf.org */
{{130, 105, 2, 143}, {0x2, 0x60, 0x8c, 0x69, 0x86, 0x4}},  /* sunburn.osf */
{{130, 105, 2, 121}, {0x0, 0xaa, 0x0, 0x2, 0xe0, 0x7e}},   /* dark.osf.org */
{{130, 105, 2, 175}, {0x0, 0x0, 0xc0, 0xbd, 0x3e, 0x14}},  /* osfri.osf.org */
{{130, 105, 0, 0},   {0xff, 0xff, 0xff, 0xff, 0xff, 0xff}},/* broadcast */
{{130, 105, 255, 255}, {0xff, 0xff, 0xff, 0xff, 0xff, 0xff}},/* broadcast */
#endif	USE_STATIC_ARP

};

int				netipc_arp_table_size
		= sizeof netipc_arp_table / sizeof(struct netipc_arp_entry);

struct mutex			netipc_arp_table_lock;

/*
 * First few entries in ARP table reserved for static info.
 */
int				netipc_arp_reserved;

/*
 * Next candidate for recycling in arp table.
 */
int				netipc_arp_sweeper;

/*
 * netipc_find_ether_addr
 *	Find the Ethernet address for a given IP address
 */
PRIVATE boolean_t netipc_find_ether_addr(ipaddr,etheraddr)
	netaddr_t			ipaddr;
	struct ether_addr		*etheraddr;	/* OUT */
BEGIN("netipc_find_ether_addr")
	int				i;
	kern_return_t			ret;
	char				local_host_name[10];

	DEBUG1(NETIPC_DEBUG,4,1035,ipaddr);

	mutex_lock(&netipc_arp_table_lock);

	for (i = 0; i < netipc_arp_table_size; i++) {
		if((*(netaddr_t *)&netipc_arp_table[i].ip_addr) == ipaddr) {
			*etheraddr = netipc_arp_table[i].ether_addr;
			mutex_unlock(&netipc_arp_table_lock);
			DEBUG6(NETIPC_DEBUG,4,1036,
				(char) etheraddr->ether_addr_octet[0],
				(char) etheraddr->ether_addr_octet[1],
				(char) etheraddr->ether_addr_octet[2],
				(char) etheraddr->ether_addr_octet[3],
				(char) etheraddr->ether_addr_octet[4],
				(char) etheraddr->ether_addr_octet[5]);
			RETURN(TRUE);
		}
	}

	if (mach3_net_server_port == PORT_NULL) {
	        /*
		 * Note: use the internal version of netname_look_up().
		 */
		local_host_name[0] = '\0';
		ret = _netname_look_up(name_server_port,local_host_name,
				mach3_net_server_name,&mach3_net_server_port);
		if (ret != KERN_SUCCESS) {
			mutex_unlock(&netipc_arp_table_lock);
			ERROR((msg,
			"netipc_mach3: cannot find the network server: %s",
						mach_error_string(ret)));
			ERROR((msg,
			"netipc_mach3: cannot find ethernet address for 0x%x",
								ipaddr));
			RETURN(FALSE);
		}
	}

	ret = mig_net_arp(mach3_net_server_port,ipaddr,etheraddr);
	if (ret != KERN_SUCCESS) {
		mutex_unlock(&netipc_arp_table_lock);
		ERROR((msg,"netipc_mach3: mig_net_arp() failed: %s",
						mach_error_string(ret)));
		ERROR((msg,
			"netipc_mach3: cannot find ethernet address for 0x%x",
								ipaddr));
		RETURN(FALSE);
	}
	for (i = netipc_arp_reserved; i < netipc_arp_table_size; i++) {
		if((*(netaddr_t *)&netipc_arp_table[i].ip_addr) == 0) {
			break;
		}
	}
	if (i == netipc_arp_table_size) {
		i = netipc_arp_sweeper;
		if (++netipc_arp_sweeper == netipc_arp_table_size) {
			netipc_arp_sweeper = netipc_arp_reserved;
		}
	}
	netipc_arp_table[i].ip_addr = * (struct ip_addr *) & ipaddr;
	ETHER_COPY(netipc_arp_table[i].ether_addr, *etheraddr);

	mutex_unlock(&netipc_arp_table_lock);
	RETURN(TRUE);
END


/*
 * netipc_init_mach3
 *	Initialize the netipc_mach3 module.
 *
 * Parameters:
 *	none.
 *
 * Results:
 *	TRUE or FALSE for success or failure.
 *
 */
EXPORT boolean_t netipc_init_mach3()
BEGIN("netipc_init_mach3")
	kern_return_t		kr;
	struct net_status	if_stat;
	unsigned int		if_stat_count;
	char			if_addr[sizeof(struct ether_addr) + 4];
					/* padding */
	unsigned int		if_addr_count;

	/*
	 * Open the device.
	 */
	kr = device_open(mach_device_server_port(),0,
						mach3_if_name,&mach3_if_port);
	if (kr != KERN_SUCCESS) {
		ERROR((msg,
		"netipc_init_mach3.device_open failed, kr=%d",kr));
		RETURN(FALSE);
	}

	/*
	 * Get status and address from interface.
	 */
	if_stat_count = NET_STATUS_COUNT;
	kr = device_get_status(mach3_if_port,NET_STATUS,
						&if_stat,&if_stat_count);
	if (kr != D_SUCCESS) {
		(void) device_close(mach3_if_port);
		ERROR((msg,
	"netipc_init_mach3.device_get_status(NET_STATUS) failed: %d\n",kr));
		RETURN(FALSE);
	}
	if (if_stat.header_format != HDR_ETHERNET) {
		(void) device_close(mach3_if_port);
		ERROR((msg,
		"netipc_init_mach3: unsupported device header format: %d\n",
						if_stat.header_format));
		RETURN(FALSE);
	}
	if (if_stat.max_packet_size > NETIPC_MACH3_MAX_PKT_SIZE) {
		(void) device_close(mach3_if_port);
		ERROR((msg,
	"netipc_init_mach3: invalid device max packet size: %d (max %d)\n",
			if_stat.max_packet_size,NETIPC_MACH3_MAX_PKT_SIZE));
		RETURN(FALSE);
	}
	if (if_stat.header_size != sizeof(struct ether_header)) {
		(void) device_close(mach3_if_port);
		ERROR((msg,
			"netipc_init_mach3: invalid device header size: %d\n",
							if_stat.header_size));
		RETURN(FALSE);
	}
	if (if_stat.address_size != sizeof(struct ether_addr)) {
		(void) device_close(mach3_if_port);
		ERROR((msg,
			"netipc_init_mach3: invalid device address size: %d\n",
							if_stat.address_size));
		RETURN(FALSE);
	}
	if_addr_count = (sizeof(struct ether_addr)
						+ sizeof(int) - 1)/sizeof(int);
	kr = device_get_status(mach3_if_port,NET_ADDRESS,
					(dev_status_t)if_addr,&if_addr_count);
	if (kr != D_SUCCESS) {
		(void) device_close(mach3_if_port);
		ERROR((msg,
		"mach3_ether.device_get_status(NET_ADDRESS) failed: %d\n",kr));
		RETURN(FALSE);
	}

	/*
	 * Byte-swap address into host format.
	 */
	{
	    register int 	i;
	    register int	*ip;

	    for (i = 0, ip = (int *)if_addr;
		 i < if_addr_count;
		 i++,ip++) {
		*ip = ntohl(*ip);
	    }
	}

	/*
	 * Initialize the ARP table.
	 */
	mutex_init(&netipc_arp_table_lock);
	netipc_arp_table[0].ip_addr = * (struct ip_addr *) & my_host_id;
	ETHER_COPY(netipc_arp_table[0].ether_addr,
					* (struct ether_addr *) if_addr);
	netipc_arp_table[1].ip_addr = * (struct ip_addr *) & broadcast_address;
	ETHER_COPY(netipc_arp_table[1].ether_addr,ether_bcast);
	netipc_arp_reserved = 2;
	netipc_arp_sweeper = netipc_arp_reserved;

	RETURN(TRUE);
END


/*
 * netipc_bind_mach3
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
EXPORT netipc_channel_ptr_t netipc_bind_mach3(udp_port, hdr_size, priority)
	unsigned short                  udp_port;
	int                             hdr_size;
	int				priority;
BEGIN("netipc_bind_mach3")
	netipc_mach3_channel_ptr_t	channel;
	netipc_mach3_out_pkt_ptr_t	pkt_ptr;
	netipc_mach3_out_hdr_ptr_t	hdr_ptr;
	kern_return_t                   kr;

	/*
	 * Initialize the channel data.
	 */
	MEM_ALLOC(channel,netipc_mach3_channel_ptr_t,
				sizeof(netipc_mach3_channel_t),FALSE);
	MEM_ALLOCOBJ(pkt_ptr,netipc_mach3_out_pkt_ptr_t,MEM_TRBUFF);
	hdr_ptr = &pkt_ptr->hdr;
	channel->common.buffer_size = NETIPC_MACH3_MAX_DATA_SIZE
					+ sizeof(netipc_mach3_in_hdr_t);
	channel->common.template_pkt = (netipc_ptr_t) pkt_ptr;
	channel->common.template_hdr = (char *) hdr_ptr;
	channel->common.template_offset = ((char *) hdr_ptr) -
							((char *) pkt_ptr);
	channel->common.template_size = sizeof(netipc_mach3_out_hdr_t) +
							hdr_size;
	channel->common.receive = netipc_receive_mach3;
	channel->common.send = netipc_send_mach3;
	channel->common.resend = netipc_resend_mach3;
	mutex_init(&channel->ether_cache_lock);
	netipc_find_ether_addr(my_host_id,
				channel->ether_cache_hdr.ether_shost);
	channel->ether_cache_hdr.ether_type = htons(ETHERTYPE_IP);
	channel->ether_cache_ip_addr = 0;
	
	/*
	 * Get the interface port. XXX
	 */
	channel->if_port = mach3_if_port;

	 /*
	  * Initialize the template
	  */
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
	 * Set up a filter to receive packets.
	 */
	kr = mach_port_allocate(mach_task_self(),MACH_PORT_RIGHT_RECEIVE,
							&channel->reply_port);
	if (kr != KERN_SUCCESS) {
		ERROR((msg,
		"netipc_bind_mach3.port_allocate fails, kr = %d.", kr));
		RETURN(NULL);
	}
	{
		filter_t	filter[40];
		int		i = 0;

#ifdef	notdef	/* good filter */

		filter[i++] = NETF_PUSHLIT;
/* NOHACK	filter[i++] = (filter_t) htons(ETHERTYPE_IP);		*/
/* HACK */	filter[i++] = (filter_t) (htons(ETHERTYPE_IP) + 16);
/* HACK */	filter[i++] = NETF_PUSHLIT | NETF_SUB;
/* HACK */	filter[i++] = (filter_t) 16;
		filter[i++] = (NETF_PUSHWORD + 1) | NETF_EQ;
		filter[i++] = NETF_PUSHLIT;
/* NOHACK	filter[i++] = (filter_t) htons(0xff);			*/
/* HACK */	if (htons(0xff) == 0xff) {
/* HACK	*/		filter[i++] = (filter_t) 0xff;
/* HACK */	} else {
/* HACK	*/		filter[i++] = (filter_t) 0xff;
/* HACK */		filter[i++] = NETF_PUSHLIT;
/* HACK */		filter[i++] = (filter_t) 24;
/* HACK */		filter[i++] = NETF_PUSHLIT | NETF_SUB;
/* HACK */		filter[i++] = (filter_t) 16;
/* HACK */		filter[i++] = NETF_LSH;
/* HACK */	}
		filter[i++] = (NETF_PUSHWORD + 6) | NETF_AND;
		filter[i++] = NETF_PUSHLIT;
		filter[i++] = (filter_t) htons(IPPROTO_UDP);
		filter[i++] = NETF_EQ;
		filter[i++] = NETF_AND;
		filter[i++] = NETF_PUSHLIT;
/* NOHACK	filter[i++] = (filter_t) htons(udp_port);		*/
/* HACK */	filter[i++] = (filter_t) ((htons(udp_port) >> 8) & 0xff);
/* HACK */	filter[i++] = NETF_PUSHLIT;
/* HACK */	filter[i++] = (filter_t) 24;
/* HACK */	filter[i++] = NETF_PUSHLIT | NETF_SUB;
/* HACK */	filter[i++] = (filter_t) 16;
/* HACK */	filter[i++] = NETF_LSH;
/* HACK */	filter[i++] = NETF_PUSHLIT | NETF_OR;
/* HACK */	filter[i++] = (filter_t) (htons(udp_port) & 0xff);
		filter[i++] = (NETF_PUSHWORD + 13) | NETF_EQ;
		filter[i++] = NETF_AND;

#else	/* good filter */

		filter[i++] = NETF_PUSHLIT;
		filter[i++] = (filter_t) htons(ETHERTYPE_IP);
		filter[i++] = (NETF_PUSHWORD + 1) | NETF_EQ;
		filter[i++] = NETF_PUSHLIT;
		filter[i++] = (filter_t) htons(0xff);
		filter[i++] = (NETF_PUSHWORD + 6) | NETF_AND;
		filter[i++] = NETF_PUSHLIT;
		filter[i++] = (filter_t) htons(IPPROTO_UDP);
		filter[i++] = NETF_EQ;
		filter[i++] = NETF_AND;
		filter[i++] = NETF_PUSHLIT;
		filter[i++] = (filter_t) htons(udp_port);
		filter[i++] = (NETF_PUSHWORD + 13) | NETF_EQ;
		filter[i++] = NETF_AND;

#endif	/* good filter */

		(void) device_set_filter(channel->if_port,
						channel->reply_port,
						MACH_MSG_TYPE_MAKE_SEND,
						priority,
						filter,
						i);
	}

	RETURN(&channel->common);
END


/*
 * netipc_receive_mach3
 *	Receive a packet over the network.
 *	Reject a packet from ourself otherwise check its UDP checksum.
 *
 * Parameters:
 *
 * Results:
 *	NETIPC_BAD_UDP_CHECKSUM or the return code from msg_receive.
 *	pkt_ptr	: pointer to a buffer to hold the packet
 *
 */
EXPORT kern_return_t netipc_receive_mach3(in_channel,in_pkt_ptr)
	netipc_channel_ptr_t		in_channel;
	netipc_ptr_t			in_pkt_ptr;
BEGIN("netipc_receive_mach3")
	netipc_mach3_channel_ptr_t	channel =
				(netipc_mach3_channel_ptr_t) in_channel;
	netipc_mach3_in_pkt_ptr_t	pkt_ptr =
				(netipc_mach3_in_pkt_ptr_t) in_pkt_ptr;
	netipc_mach3_in_hdr_ptr_t	hdr_ptr;
	kern_return_t			kr;
	int				type;

	hdr_ptr = &pkt_ptr->hdr;

	while (1) {
		hdr_ptr->msg_header.msgh_size = channel->common.buffer_size;
		hdr_ptr->msg_header.msgh_local_port = channel->reply_port;
		kr = mach_msg(&hdr_ptr->msg_header,MACH_RCV_MSG|MACH_RCV_LARGE,
				0,channel->common.buffer_size,
				channel->reply_port,
				MACH_MSG_TIMEOUT_NONE,MACH_PORT_NULL);
		if (kr != MACH_MSG_SUCCESS) {
			DEBUG1(NETIPC_DEBUG, 3, 1034, kr);
			if (kr == MACH_RCV_TOO_LARGE) {
				/*
				 * Special case for when the incoming
				 * message is actually bigger than the
				 * buffer. How can this happen? XXX
				 */
				int	size = hdr_ptr->msg_header.msgh_size;

				ERROR((msg,
				"netipc_receive_mach3: huge packet: %d",size));
				MEM_ALLOC(hdr_ptr,netipc_mach3_in_hdr_ptr_t,
								size,FALSE);
				hdr_ptr->msg_header.msgh_size = size;
				hdr_ptr->msg_header.msgh_local_port =
							channel->reply_port;
				kr = mach_msg_receive(&hdr_ptr->msg_header);
				MEM_DEALLOC(hdr_ptr,size);
			}
			RETURN(kr);
		}
		DEBUG4(NETIPC_DEBUG,3,1037,
					hdr_ptr->msg_header.msgh_bits,
					hdr_ptr->msg_header.msgh_size,
					hdr_ptr->msg_header.msgh_id,
					hdr_ptr->msg_header.msgh_kind);
		DEBUG2(NETIPC_DEBUG,3,1038,
					hdr_ptr->packet_header.length,
					hdr_ptr->packet_header.type);
		DEBUG5(NETIPC_DEBUG,3,1039,
					hdr_ptr->ip_header.ip_len,
					hdr_ptr->ip_header.ip_p,
					hdr_ptr->ip_header.ip_sum,
					hdr_ptr->ip_header.ip_src.s_addr,
					hdr_ptr->ip_header.ip_dst.s_addr);
		DEBUG4(NETIPC_DEBUG,3,1041,
					hdr_ptr->udp_header.uh_sport,
					hdr_ptr->udp_header.uh_dport,
					hdr_ptr->udp_header.uh_ulen,
					hdr_ptr->udp_header.uh_sum);
		{
			int		*data = (int *)pkt_ptr->data;
			DEBUG6(NETIPC_DEBUG,3,1042,
						data[ 0],data[ 1],data[ 2],
						data[ 3],data[ 4],data[ 5]);
			DEBUG6(NETIPC_DEBUG,3,1042,
						data[ 6],data[ 7],data[ 8],
						data[ 9],data[10],data[11]);
			DEBUG6(NETIPC_DEBUG,3,1042,
						data[12],data[13],data[14],
						data[15],data[16],data[17]);
			DEBUG6(NETIPC_DEBUG,3,1042,
						data[18],data[19],data[20],
						data[21],data[22],data[23]);
			DEBUG6(NETIPC_DEBUG,3,1042,
						data[24],data[25],data[26],
						data[27],data[28],data[29]);
		}


		type = ntohs(hdr_ptr->packet_header.type);
		if (type != ETHERTYPE_IP) {
			ERROR((msg,
			"netipc_receive_mach3: wrong ethernet type:%d",type));
			continue;
		}

		pkt_ptr->control.from = hdr_ptr->ip_header.ip_src.s_addr;
		pkt_ptr->control.to = 0;
		if (pkt_ptr->control.from == my_host_id) {
			continue;
		}

		hdr_ptr->ip_header.ip_len = ntohs(hdr_ptr->ip_header.ip_len);

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
 * netipc_send_mach3
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
EXPORT kern_return_t netipc_send_mach3(in_channel,in_pkt_ptr,do_checksum)
	netipc_channel_ptr_t		in_channel;
	netipc_ptr_t	 		in_pkt_ptr;
	int				do_checksum;
BEGIN("netipc_send_mach3")
	netipc_mach3_channel_ptr_t	channel =
				(netipc_mach3_channel_ptr_t) in_channel;
	netipc_mach3_out_pkt_ptr_t	pkt_ptr =
				(netipc_mach3_out_pkt_ptr_t) in_pkt_ptr;
	netipc_mach3_out_hdr_ptr_t	hdr_ptr;
	struct ether_header		*ether_ptr;
	short				saved_ttl;
	msg_return_t			kr;

	hdr_ptr = &pkt_ptr->hdr;
	ether_ptr = ((struct ether_header *)hdr_ptr) - 1;

	mutex_lock(&channel->ether_cache_lock);
	if (channel->ether_cache_ip_addr != pkt_ptr->control.to) {
		boolean_t	found;
		found = netipc_find_ether_addr(pkt_ptr->control.to,
				channel->ether_cache_hdr.ether_dhost);
		if (found) {
			channel->ether_cache_ip_addr = pkt_ptr->control.to;
		} else {
			mutex_unlock(&channel->ether_cache_lock);
			RETURN(KERN_FAILURE);
		}
	}
	bcopy(&channel->ether_cache_hdr,ether_ptr,sizeof(struct ether_header));
	mutex_unlock(&channel->ether_cache_lock);

	hdr_ptr->ip_header.ip_dst.s_addr = pkt_ptr->control.to;
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
	hdr_ptr->ip_header.ip_len = htons(hdr_ptr->ip_header.ip_len);
	hdr_ptr->ip_header.ip_sum = 0;
	hdr_ptr->ip_header.ip_sum = udp_checksum(&hdr_ptr->ip_header,
							sizeof(struct ip));
	kr = device_write_request(channel->if_port,PORT_NULL,0,0,ether_ptr,
		pkt_ptr->control.data_size +
		sizeof(netipc_mach3_out_hdr_t) + sizeof(struct ether_header));

	DEBUG1(NETIPC_DEBUG, 0, 1031, kr);
	if (kr != KERN_SUCCESS) {
		ERROR((msg,
		"netipc_send_mach3.device_write_request failed, kr=%d\n",kr));
	}

	RETURN(kr);

END


/*
 * netipc_resend_mach3
 *	Resend a packet that was previously sent (and checksummed).
 *
 * Parameters:
 *	pkt_ptr	: pointer to the packet to be sent.
 *
 * Results:
 *	value returned by msg_send.
 *
 */
EXPORT kern_return_t netipc_resend_mach3(in_channel,in_pkt_ptr)
	netipc_channel_ptr_t		in_channel;
	netipc_ptr_t	 		in_pkt_ptr;
BEGIN("netipc_resend_mach3")
	netipc_mach3_channel_ptr_t	channel =
				(netipc_mach3_channel_ptr_t) in_channel;
	netipc_mach3_out_pkt_ptr_t	pkt_ptr =
				(netipc_mach3_out_pkt_ptr_t) in_pkt_ptr;
	netipc_mach3_out_hdr_ptr_t	hdr_ptr;
	struct ether_header		*ether_ptr;
	short				saved_ttl;
	msg_return_t			kr;

	hdr_ptr = &pkt_ptr->hdr;
	ether_ptr = ((struct ether_header *)hdr_ptr) - 1;

	/*
	 * Insert a new ip id into the header and send the packet.
	 */
	hdr_ptr->ip_header.ip_id = last_ip_id ++;
	hdr_ptr->ip_header.ip_sum = 0;
	hdr_ptr->ip_header.ip_sum = udp_checksum(&hdr_ptr->ip_header,
							sizeof(struct ip));
	kr = device_write_request(channel->if_port,PORT_NULL,0,0,ether_ptr,
		pkt_ptr->control.data_size +
		sizeof(netipc_mach3_out_hdr_t) + sizeof(struct ether_header));
	DEBUG1(NETIPC_DEBUG, 0, 1031, kr);
	if (kr != KERN_SUCCESS) {
		ERROR((msg,
		"netipc_resend_mach3.device_write_request failed, kr=%d\n",
		kr));
	}

	RETURN(kr);

END

#endif	USE_NETIPC_MACH3
