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
 * $Log:	ipc_ether.c,v $
 * Revision 2.3  91/08/28  11:15:54  jsb
 * 	Generalized netipc_swap_device code.
 * 	[91/08/16  14:30:15  jsb]
 * 
 * 	Added fields to support nrp (node table resolution protocol),
 * 	allowing removal of hardwired table of node/ip/ether addresses.
 * 	[91/08/15  08:58:04  jsb]
 * 
 * Revision 2.2  91/08/03  18:19:14  jsb
 * 	Restructured to work with completely unmodified ethernet driver.
 * 	Added code to automatically find a usable ethernet device.
 * 	Removed NODE_BY_SUBNET case. Perform initialization earlier.
 * 	[91/08/02  14:21:08  jsb]
 * 
 * 	Use IO_LOANED technology.
 * 	[91/07/27  22:56:42  jsb]
 * 
 * 	Added fragmentation.
 * 	[91/07/27  18:54:37  jsb]
 * 
 * 	First checkin.
 * 	[91/07/24  23:38:03  jsb]
 * 
 */
/*
 *	File:	norma/ipc_ether.c
 *	Author:	Joseph S. Barrera III
 *	Date:	1991
 *
 *	Functions for NORMA_IPC over ethernet.
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
#include <device/io_req.h>
#include <device/if_hdr.h>
#include <device/net_status.h>
#include <norma/ipc_net.h>
#include <norma/ipc_ether.h>

#define	MAXFRAG			8
#define	FHP_OVERHEAD		sizeof(struct netipc_fragment_header)

typedef struct netipc_fragment_header	*netipc_fragment_header_t;

struct netipc_fragment_header {
	unsigned short	f_offset;
	unsigned char	f_id;
	unsigned char	f_last;
};

int Noise3 = 0;
int Noise4 = 0;

struct io_req			netipc_ior[MAXFRAG];
char				netipc_ether_buf[MAXFRAG * ETHERMTU];

struct netvec			*recv_netvec = 0;
int				recv_netvec_count;

#define	XSA(a,b,o)	(*(o+(short *)&(a)) = *(o+(short *)&(b)))
#define XLA(a,b)	(XSA((a),(b),0),XSA((a),(b),1))

/*
 * Netipc_recv_fragment_ip_src is 0 if no current frag.
 * If we get packet from ip_src which is not a completing frag,
 * we should ask for missing frags. This is therefor a sender timeout driven
 * mechanism, tied in with higher level timeouts.
 *
 * We should have several pages for reassembly. This should be tied in with
 * the ability to subsitute buffers of same size or smaller.
 * E.g. substituting small kmsgs for pages.
 *
 * For now, if a frag assembly is in progress, we drop everything
 * else aimed at us. An immediate refinement would be to have one
 * page reserved for reassembly, leaving another for recording nacks.
 *
 * Maybe upper level should be allowed to provide several receive buffers
 * and we find best fit. That would seem to minimize our knowledge
 * of what buffers are all about.
 */

int				netipc_recv_fragment_ip_src;
int				netipc_recv_fragment_f_id;

int netipc_ip_id = 1;

#define	NODE_INVALID	-1
int _node_self = NODE_INVALID;
struct node_addr *node_self_addr;

#define	MAX_NODE_TABLE_SIZE	256

struct node_addr	node_table[MAX_NODE_TABLE_SIZE];
int			node_table_size = 0;

#define	NODE_ID(index)	(index)

struct node_addr *
node_addr_by_node(node)
	int node;
{
	return &node_table[node];
}

node_self()
{
	if (_node_self == NODE_INVALID) {
		printf("... premature node_self() call\n");
		netipc_network_init();
	}
	if (_node_self == NODE_INVALID) {
		panic("node_self: I don't know what node I am\n");
	}
	return _node_self;
}

#if 1
/* debugging */
node_id(node_ether_addr)
	unsigned short *node_ether_addr;
{
	int i;

	if (node_table_size == 0) {
		panic("bad node_table_size\n");
	}
	for (i = 0; i < node_table_size; i++) {
		if (node_ether_addr[0] == node_table[i].node_ether_addr[0] &&
		    node_ether_addr[1] == node_table[i].node_ether_addr[1] &&
		    node_ether_addr[2] == node_table[i].node_ether_addr[2]) {
			return NODE_ID(i);
		}
	}
	return NODE_INVALID;
}

node_ip_id(ip_addr)
	unsigned long ip_addr;
{
	int i;

	if (node_table_size == 0) {
		panic("bad node_table_size\n");
	}
	for (i = 0; i < node_table_size; i++) {
		if (node_table[i].node_ip_addr == ip_addr) {
			return NODE_ID(i);
		}
	}
	return NODE_INVALID;
}
#endif

struct dev_ops *netipc_device;
int netipc_device_unit = 0;

#define	is_nulldev(func)	((func)==nodev || (func)==nulldev)

unsigned long eaddr_l[2];	/* XXX */

netipc_network_init()
{
	int count;
	int unit = netipc_device_unit;	/* XXX */
	struct dev_ops *dp;

	/*
	 * Return if already initialized.
	 */
	if (netipc_device) {
		return;
	}

	/*
	 * Look for a network device.
	 */
	dev_search(dp) {
		if (is_nulldev(dp->d_write) ||
		    is_nulldev(dp->d_getstat) ||
		    ! is_nulldev(dp->d_read)) {
			continue;
		}
		if ((*dp->d_open)(unit, 0) != 0) {
			/*
			 * Open failed. Try another one.
			 */
			continue;
		}
		if ((*dp->d_getstat)(unit, NET_ADDRESS, eaddr_l, &count) == 0){
			/*
			 * Success!
			 */
			netipc_device = dp;
			printf("netipc: using %s%d\n",
			       netipc_device->d_name,
			       netipc_device_unit);
			break;
		}
		/*
		 * Opened the wrong device. Close and try another one.
		 */
		if (dp->d_close) {
			(*dp->d_close)(unit);
		}
	}

	if (! netipc_device) {
		panic("netipc_network_init: could not find network device\n");
	}

	/*
	 * Set address and calculate node number.
	 */
	eaddr_l[0] = ntohl(eaddr_l[0]);
	eaddr_l[1] = ntohl(eaddr_l[1]);
	printf("netipc: waiting for node table\n");
	while (node_self_addr == 0) {
		assert_wait(&node_self_addr, TRUE);
		nd_request();
		thread_set_timeout(5 * hz);
		thread_block((void (*)()) 0);
	}
	printf("netipc: node %d internet 0x%x ethernet %s\n",
	       node_self(),
	       ntohl(node_self_addr->node_ip_addr),
	       ether_sprintf(node_self_addr->node_ether_addr));
}

boolean_t nd_request_in_progress = FALSE;

request_ior_done()
{
	nd_request_in_progress = FALSE;
}

nd_request()
{
	netipc_ether_header_t ehp;
	netipc_udpip_header_t uhp;
	struct nd_request *ndrq;
	int i, total_length, udp_length;
	unsigned long checksum;

	if (nd_request_in_progress) {
		return;
	}
	nd_request_in_progress = TRUE;

	ehp  = (netipc_ether_header_t) netipc_ether_buf;
	uhp  = (netipc_udpip_header_t) (((char *) ehp) + EHLEN);
	ndrq = (struct nd_request *) (uhp + 1);
	
	udp_length = UDP_OVERHEAD + sizeof(struct nd_request);
	total_length = EHLEN + IP_OVERHEAD + udp_length;

	netipc_ior[0].io_count = total_length;
	netipc_ior[0].io_data = (char *) ehp;
	netipc_ior[0].io_done = request_ior_done;
	netipc_ior[0].io_op = IO_LOANED;
	
	ehp->e_ptype = htons(ETHERTYPE_IP);
	bcopy(eaddr_l, ehp->e_src, 6);
	ehp->e_dest[0] = 0xffff;
	ehp->e_dest[1] = 0xffff;
	ehp->e_dest[2] = 0xffff;
	
	uhp->ip_version = IPVERSION;
	uhp->ip_header_length = 5;
	uhp->ip_type_of_service = 0;
	uhp->ip_total_length = htons(udp_length + IP_OVERHEAD);
	uhp->ip_id = htons(netipc_ip_id++);
	uhp->ip_fragment_offset = htons(0);
	uhp->ip_time_to_live = 0xff;
	uhp->ip_protocol = UDP_PROTOCOL;
	uhp->ip_checksum = 0;

	uhp->ip_src = 0;
	uhp->ip_dst = htonl(0xffffffff);

	for (checksum = i = 0; i < 10; i++) {
		checksum += ((unsigned short *) uhp)[i];
	}
	checksum = (checksum & 0xffff) + (checksum >> 16);
	uhp->ip_checksum = (~checksum & 0xffff);

	uhp->udp_source_port = htons(IPPORT_NDREPLY);
	uhp->udp_dest_port = htons(IPPORT_NDREQUEST);
	uhp->udp_length = htons(udp_length);
	uhp->udp_checksum = 0;

	ndrq->ndrq_magic = htonl(NDRQ_MAGIC);
	ndrq->ndrq_start = htons(0);
	bcopy(eaddr_l, ndrq->ndrq_eaddr, 6);

	/*
	 * Send the packet.
	 */
	(*netipc_device->d_write)(netipc_device_unit, &netipc_ior[0]);
}

nd_reply(uhp, count)
	netipc_udpip_header_t uhp;
	int count;
{
	struct nd_reply *ndrp = (struct nd_reply *) (uhp + 1);
	int i;

	if (node_self_addr) {
		/*
		 * Table already initialized.
		 */
		return;
	}
	if (ndrp->ndrp_magic != htonl(NDRP_MAGIC)) {
		printf("nd_reply: bad magic\n");
		return;
	}
	_node_self = ntohs(ndrp->ndrp_node_self);
	node_table_size = ntohs(ndrp->ndrp_table_size);
	if (node_table_size < 0 || node_table_size > MAX_NODE_TABLE_SIZE) {
		panic("netipc_nd_reply: bad table size %d\n", node_table_size);
	}
	for (i = 0; i < node_table_size; i++) {
		struct nd *nd = &ndrp->ndrp_table[i];
		struct node_addr *na = &node_table[i];

		if (! nd->nd_valid) {
			node_table[i].node_ip_addr = 0;
			bzero(node_table[i].node_ether_addr, 6);
			continue;
		} else {
			na->node_ip_addr = nd->nd_iaddr;
			bcopy(nd->nd_eaddr, na->node_ether_addr, 6);
		}
	}
	node_self_addr = &node_table[_node_self];
	thread_wakeup(&node_self_addr);
}

char netipc_swap_device[3] = "hd";
int netipc_swap_node_1 = 1;
int netipc_swap_node_2 = 1;
int netipc_swap_xor = 1;

char *
dev_forward_name(name, namebuf, namelen)
	char *name;
	char *namebuf;
	int namelen;
{
	if (netipc_swap_node_1 == netipc_swap_node_2) {
		return name;
	}
	if (name[0] == netipc_swap_device[0] &&
	    name[1] == netipc_swap_device[1]) {
		namebuf[0] = '<';
		if (node_self() == netipc_swap_node_1) {
			namebuf[1] = '0' + netipc_swap_node_2;
		} else {
			namebuf[1] = '0' + netipc_swap_node_1;
		}
		namebuf[2] = '>';
		namebuf[3] = name[0];			/* e.g. 'h' */
		namebuf[4] = name[1];			/* e.g. 'd' */
		namebuf[5] = name[2] ^ netipc_swap_xor;	/* major */
		namebuf[6] = name[3];			/* minor */
		namebuf[7] = '\0';
		printf("[ %s -> %s ]\n", name, namebuf);
		return namebuf;
	}
	return name;
}

netipc_net_packet(ehp, uhp, count)
	register struct netipc_ether_header *ehp;
	register struct netipc_udpip_header *uhp;
	unsigned int count;
{
	if (ehp->e_ptype != htons(ETHERTYPE_IP) ||
#if 0
	    node_ip_id(uhp->ip_src) == -1 ||
#endif
	    uhp->ip_protocol != UDP_PROTOCOL) {
		return FALSE;
	}
	if (uhp->udp_dest_port == NETIPC_UDPPORT) {
		if (Noise4) {
			printf("R s=%d.%x, d=%d.%x %d\n",
			       node_ip_id(uhp->ip_src),
			       ntohl(uhp->ip_src),
			       node_ip_id(uhp->ip_dst),
			       ntohl(uhp->ip_dst),
			       ntohs(uhp->udp_length) + EHLEN + IP_OVERHEAD);
		}
		netipc_recv_copy(uhp, count);
		return TRUE;
	} else if (ntohs(uhp->udp_dest_port) == IPPORT_NDREPLY) {
		nd_reply(uhp, count);
		return TRUE;
	} else {
		return FALSE;
	}
}

extern int c_netipc_frag_drop1;
extern int c_netipc_frag_drop2;
extern int c_netipc_frag_drop3;
extern int c_netipc_frag_drop4;

netipc_recv_copy(uhp, count)
	netipc_udpip_header_t uhp;
	int count;
{
	netipc_fragment_header_t fhp = (netipc_fragment_header_t) (uhp + 1);
	char *buf = (char *) (fhp + 1);
	int i, offset = ntohs(fhp->f_offset);

	/* assert count >= 0 */

	if (recv_netvec == 0) {
		c_netipc_frag_drop1++;
		return;
	}

	if (netipc_recv_fragment_ip_src) {
		if (netipc_recv_fragment_ip_src != uhp->ip_src) {
			/*
			 * Someone has a frag in progress. Drop this packet.
			 */
			c_netipc_frag_drop2++;
			return;
		}
		if (fhp->f_last == 0) {
			/*
			 * New nonfragged message replaces old frag.
			 * XXX clearly suboptimal, see discussion above
			 * XXX assumes only one fragged message in transit
			 */
			c_netipc_frag_drop3++;
			netipc_recv_fragment_ip_src = 0;
		}
	}

	/*
	 * First check for non fragmented packets.
	 */
	if (fhp->f_last == 0) {
		for (i = 0; i < recv_netvec_count; i++) {
			if (count < recv_netvec[i].size) {
				bcopy(buf, recv_netvec[i].addr, count);
				netipc_recv_intr();
				return;
			}
			bcopy(buf, recv_netvec[i].addr, recv_netvec[i].size);
			buf += recv_netvec[i].size;
			count -= recv_netvec[i].size;
		}
		netipc_recv_intr();
		return;
	}

	/*
	 * The packet was fragmented.
	 * If this is the first frag of a message, set up frag info.
	 */
	if (netipc_recv_fragment_ip_src == 0) {
		netipc_recv_fragment_ip_src = uhp->ip_src;
		netipc_recv_fragment_f_id = 0;
	}

	/*
	 * If we missed a frag, drop the whole thing!
	 */
	if (netipc_recv_fragment_f_id != fhp->f_id) {
		netipc_recv_fragment_ip_src = 0;
		c_netipc_frag_drop4++;
		return;
	}

	/*
	 * Skip to the right offset in the receive buffer,
	 * and copy fragment into it.
	 */
	for (i = 0; i < recv_netvec_count; i++) {
		if (offset >= recv_netvec[i].size) {
			offset -= recv_netvec[i].size;
			continue;
		}
		if (count < recv_netvec[i].size - offset) {
			bcopy(buf, recv_netvec[i].addr + offset,
			      count);
			goto did_copy;
		}
		bcopy(buf, recv_netvec[i].addr + offset,
		      recv_netvec[i].size - offset);
		buf += (recv_netvec[i].size - offset);
		count -= (recv_netvec[i].size - offset);
		break;
	}
	for (i++; i < recv_netvec_count; i++) {
		if (count < recv_netvec[i].size) {
			bcopy(buf, recv_netvec[i].addr, count);
			goto did_copy;
		}
		bcopy(buf, recv_netvec[i].addr, recv_netvec[i].size);
		buf += recv_netvec[i].size;
		count -= recv_netvec[i].size;
	}

did_copy:
	/*
	 * If this is the last frag, hand it up.
	 * Otherwise, simply note that we received this frag.
	 */
	if (fhp->f_id == fhp->f_last) {
		netipc_recv_fragment_ip_src = 0;
		netipc_recv_intr();
	} else {
		netipc_recv_fragment_f_id++;
	}
}

netipc_recv(vec, count)
	register struct netvec *vec;
	int count;
{
	recv_netvec = vec;
	recv_netvec_count = count;
}

/* (net as in usable, not as in network) */
#define	NETMTU	(ETHERMTU - EHLEN - IP_OVERHEAD - UDP_OVERHEAD - FHP_OVERHEAD)

netipc_ether_send_complete(ior)
	register io_req_t ior;
{
	register netipc_udpip_header_t uhp;
	register netipc_fragment_header_t fhp;

	uhp = (netipc_udpip_header_t) (((char *) ior->io_data) + EHLEN);
	fhp = (netipc_fragment_header_t) (uhp + 1);
	if (fhp->f_id == fhp->f_last) {
		netipc_send_intr();
	}
}

netipc_send(remote, vec, count)
	int remote;
	register struct netvec *vec;
	int count;
{
	int i, f, frag_count, data_len;
	extern struct node_addr *node_self_addr, *node_addr_by_node();
	struct node_addr *node_dest_addr;
	char *buf;
	netipc_ether_header_t ehp;
	netipc_udpip_header_t uhp;
	netipc_fragment_header_t fhp;
	int total_length, udp_length;

	/*
	 * Compute total size and number of fragments
	 */
	data_len = 0;
	for (i = 0; i < count; i++) {
		data_len += vec[i].size;
	}
	frag_count = (data_len + NETMTU - 1) / NETMTU;
	if (frag_count > MAXFRAG) {
		panic("netipc_send: size %d: too many (%d) fragments\n",
		       data_len, frag_count);
	}

	/*
	 * Get destination address
	 */
	node_dest_addr = node_addr_by_node(remote);

	/*
	 * Construct headers for each fragment; copy data into a contiguous
	 * chunk (yuck). Use loaned ior's.
	 */
	for (f = 0; f < frag_count; f++) {
		ehp = (netipc_ether_header_t) &netipc_ether_buf[f * ETHERMTU];
		uhp = (netipc_udpip_header_t) (((char *) ehp) + EHLEN);
		fhp = (netipc_fragment_header_t) (uhp + 1);
		
		if (data_len > NETMTU) {
			udp_length = ETHERMTU - EHLEN - IP_OVERHEAD;
			total_length = ETHERMTU;
			data_len -= NETMTU;
		} else {
			udp_length = UDP_OVERHEAD + FHP_OVERHEAD + data_len;
			total_length = EHLEN + IP_OVERHEAD + udp_length;
			data_len = 0;
		}
		netipc_ior[f].io_count = total_length;
		netipc_ior[f].io_data = (char *) ehp;
		netipc_ior[f].io_done = netipc_ether_send_complete;
		netipc_ior[f].io_op = IO_LOANED;

		ehp->e_ptype = htons(ETHERTYPE_IP);
		ehp->e_src[0] = node_self_addr->node_ether_addr[0];
		ehp->e_src[1] = node_self_addr->node_ether_addr[1];
		ehp->e_src[2] = node_self_addr->node_ether_addr[2];
		ehp->e_dest[0] = node_dest_addr->node_ether_addr[0];
		ehp->e_dest[1] = node_dest_addr->node_ether_addr[1];
		ehp->e_dest[2] = node_dest_addr->node_ether_addr[2];
		
		uhp->ip_version = IPVERSION;
		uhp->ip_header_length = 5;
		uhp->ip_type_of_service = 0;
		uhp->ip_total_length = htons(total_length);
		uhp->ip_id = htons(netipc_ip_id++);
		uhp->ip_fragment_offset = htons(0);
		uhp->ip_time_to_live = 0xff;
		uhp->ip_protocol = UDP_PROTOCOL;
		uhp->ip_checksum = -1;
#if 0
		uhp->ip_src = node_self_addr->node_ip_addr;
		uhp->ip_dst = node_dest_addr->node_ip_addr;
#else
		XLA(uhp->ip_src, node_self_addr->node_ip_addr);
		XLA(uhp->ip_dst, node_dest_addr->node_ip_addr);
#endif		
		uhp->udp_source_port = htons(NETIPC_UDPPORT);
		uhp->udp_dest_port = htons(NETIPC_UDPPORT);
		uhp->udp_length = htons(udp_length);
		uhp->udp_checksum = -1;

		buf = (char *) (fhp + 1);

		if (frag_count == 0) {
			fhp->f_offset = 0;
			fhp->f_id = 0;
			fhp->f_last = 0;

			for (i = 0; i < count; i++) {
				bcopy(vec[i].addr, buf, vec[i].size);
				buf += vec[i].size;
			}
		} else {
			unsigned short offset = f * NETMTU;
			unsigned short remain = NETMTU;

			fhp->f_offset = htons(offset);
			fhp->f_id = f;
			fhp->f_last = frag_count - 1;

			for (i = 0; i < count; i++) {
				if (offset >= vec[i].size) {
					offset -= vec[i].size;
					continue;
				}
				if (vec[i].size - offset >= remain) {
					bcopy(vec[i].addr + offset,
					      buf, remain);
					break;
				}
				bcopy(vec[i].addr + offset, buf,
				      vec[i].size - offset);
				buf += vec[i].size;
				remain -= vec[i].size;
				offset = 0;
			}
		}

		if (Noise4) {
			printf("S s=%d.%x, d=%d.%x %d\n",
			       node_ip_id(uhp->ip_src),
			       ntohl(node_self_addr->node_ip_addr),
			       node_ip_id(uhp->ip_dst),
			       ntohl(node_dest_addr->node_ip_addr),
			       udp_length + EHLEN + IP_OVERHEAD);
		}

		/*
		 * Send the packet.
		 */
		(*netipc_device->d_write)(netipc_device_unit, &netipc_ior[f]);
	}
}
