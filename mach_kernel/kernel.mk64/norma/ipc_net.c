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
 * $Log:	ipc_net.c,v $
 * Revision 2.4  91/09/04  11:28:40  jsb
 * 	Removed i860 support hacks. Bumped up _netipc_timeout for ipsc to
 * 	work around not-yet-understood bug.
 * 	[91/09/04  09:45:22  jsb]
 * 
 * Revision 2.3  91/08/28  11:16:06  jsb
 * 	Removed shortword count support (BTODEV, etc) since this can be handled
 * 	by each driver. Added a couple of hacks for i860 support.
 * 	[91/08/26  14:02:50  jsb]
 * 
 * 	Added changes in preparation for >1 window sizes.
 * 	Use array-based linked lists in place of full array search.
 * 	Renamed clport things to norma things.
 * 	[91/08/15  09:10:55  jsb]
 * 
 * Revision 2.2  91/08/03  18:19:25  jsb
 * 	Fiddled splon/sploff definitions. Removed another printf.
 * 	[91/08/02  14:30:57  jsb]
 * 
 * 	Many many changes.
 * 	[91/08/01  22:59:24  jsb]
 * 
 * 	Small protocol bug fixes. Removed bogus kv_addr usage.
 * 	[91/07/27  18:56:52  jsb]
 * 
 * 	First checkin.
 * 	[91/07/24  22:55:43  jsb]
 * 
 */ 
/*
 *	File:	norma/ipc_net.c
 *	Author:	Joseph S. Barrera III
 *	Date:	1991
 *
 *	Routines for reliable delivery and flow control for NORMA_IPC.
 */

#include <norma_ether.h>

#include <vm/vm_kern.h>
#include <vm/vm_page.h>
#include <mach/vm_param.h>
#include <kern/assert.h>
#include <ipc/ipc_port.h>
#include <ipc/ipc_kmsg.h>
#include <norma/ipc_net.h>

#if 1
#undef	assert
#define assert(ex)\
{if(!(ex))panic("%s:%d assert failed\n", __FILE__, __LINE__);}
#endif

#if	i386 || i860
#define	sploff()	splhigh()
#define	splon(s)	splx(s)
#else
sploff()		{ return splhigh(); }
splon(s)		{ return splx(s); }
#endif
#if	mips
unsigned long
ntohl(x)
	register unsigned long x;
{
	return (((x & 0x000000ff) << 24)|
		((x & 0x0000ff00) <<  8)|
		((x & 0x00ff0000) >>  8)|
		((x & 0xff000000) >> 24));
}

unsigned long
htonl(x)
	register unsigned long x;
{
	return (((x & 0x000000ff) << 24)|
		((x & 0x0000ff00) <<  8)|
		((x & 0x00ff0000) >>  8)|
		((x & 0xff000000) >> 24));
}

unsigned short
ntohs(x)
	register unsigned short x;
{
	return (((x & 0x00ff) << 8)|
		((x & 0xff00) >> 8));
}

unsigned short
htons(x)
	register unsigned short x;
{
	return (((x & 0x00ff) << 8)|
		((x & 0xff00) >> 8));
}
#endif

/*
 * Not multiprocessor-safe.
 */

#define	NETIPC_TYPE_INVALID	0x00000000
#define	NETIPC_TYPE_KMSG	0xabcdef00
#define	NETIPC_TYPE_PAGE	0xabcdef01
#define	NETIPC_TYPE_CTL		0xabcdef02

struct pginfo {
	unsigned long	pg_msgh_offset;
	unsigned long	pg_copy_index;
	unsigned long	pg_copy_npages;
	unsigned long	pg_copy_last;
};

struct netipc_hdr {
	unsigned long	type;
	unsigned long	seqid;
	struct pginfo	pg;
	unsigned long	remote;
	unsigned long	msgid;
	unsigned long	length;
	unsigned long	data_allign;
	unsigned long	ctl;
	unsigned long	ctl_seqid;
};

#define	MAX_NUM_NODES		256	/* XXX */

#define	CTL_NONE		0
#define	CTL_ACK			1
#define	CTL_NACK		2
#define	CTL_SYNC		3
#define	CTL_QUENCH		4	/* not used yet */

#if	mips
#define	phystokv(phys)	phys_to_k0seg(phys)	/* ??? */
#endif
#define	VPTOKV(m)	phystokv((m)->phys_addr)

/*
 * Some devices want virtual addresses, others want physical addresses.
 *
 * KVTODEV:	Kernel virtual address to device address
 * DEVTOKV:	Device address to kernel virtual address
 * VPTODEV:	vm_page_t to device address
 *
 * XXX These should be defined somewhere else.
 */
#if	NORMA_ETHER	|| i860
/*
 * Device uses virtual addresses.
 */
#define	KVTODEV(addr)	((unsigned long) (addr))
#define	DEVTOKV(addr)	((unsigned long) (addr))
#define	VPTODEV(m)	(VPTOKV(m))
#else	/*NORMA_ETHER*/
/*
 * Device uses physical addresses.
 */
#define	KVTODEV(addr)	((unsigned long) kvtophys(addr))
#define	DEVTOKV(addr)	((unsigned long) phystokv(addr))
#define	VPTODEV(m)	((m)->phys_addr)
#endif	/*NORMA_ETHER*/

/*
 * Counters and such for debugging
 */
int c_netipc_timeout	= 0;
int c_netipc_retry_k	= 0;
int c_netipc_retry_p	= 0;
int c_netipc_unstop	= 0;
int c_netipc_stop	= 0;
int c_netipc_old_recv	= 0;
int c_netipc_frag_drop1	= 0;
int c_netipc_frag_drop2	= 0;
int c_netipc_frag_drop3	= 0;
int c_netipc_frag_drop4	= 0;

struct netipc_hdr		send_hdr_p;
struct netipc_hdr		send_hdr_k;
struct netipc_hdr		send_hdr_a;

#define	MAXVEC	4
struct netvec	netvec_p[MAXVEC];
struct netvec	netvec_k[MAXVEC];
struct netvec	netvec_a[MAXVEC];
struct netvec	netvec_r[MAXVEC];

#define			netipc_send	_netipc_send
boolean_t		netipc_sending;

ipc_kmsg_t		netipc_kmsg_cache;
vm_size_t		netipc_kmsg_first_half;
int			netipc_kmsg_cache_hits;		/* debugging */
int			netipc_kmsg_cache_misses;	/* debugging */
int			netipc_kmsg_splits;		/* debugging */




/*
 * 1. should become netipc_continuation_t
 */
typedef struct netipc_packet *netipc_packet_t;
struct netipc_packet {
	int		dp_msgid;
	unsigned long	dp_seqid;
	unsigned long	dp_first_seqid;
	unsigned long	dp_last_seqid;
	ipc_kmsg_t	dp_kmsg;
	int		dp_length;		/* xxx unnesc */
	unsigned long	dp_offset;
	vm_map_copy_t	dp_copy;		/* xxx unnesc */
	unsigned long	dp_copy_index;
	unsigned long	dp_copy_last;
	netipc_packet_t	dp_next;
};

/*
 * XXX Some of these have to be redundant!
 */
#define small_int int
#define	MAX_WINDOW_SIZE	1

#define	WX		(MAX_WINDOW_SIZE + 1)

/*
 * Sender's state
 */
small_int		netipc_timer[MAX_NUM_NODES];
unsigned long		netipc_last_unacked[MAX_NUM_NODES];
unsigned long		netipc_sent_seqid[MAX_NUM_NODES];
netipc_packet_t		netipc_sent_packet[MAX_NUM_NODES][WX];
netipc_packet_t		netipc_packet[MAX_NUM_NODES];
unsigned long		netipc_rcvd_seqid[MAX_NUM_NODES];
boolean_t		netipc_do_send[MAX_NUM_NODES];

/*
 * Receiver's state
 */
unsigned long		netipc_will_ack[MAX_NUM_NODES];
int			netipc_self_stopped;

/*
 * Shared state
 */
small_int		netipc_ctl[MAX_NUM_NODES];
unsigned long		netipc_ctl_seqid[MAX_NUM_NODES];
unsigned long		netipc_usec;

/*
 * Things that can be pending:
 *
 *	netipc_ctl[]
 *	netipc_do_send[]
 *
 * and, differently,
 *
 *	<unacked>
 */

#define	AL_END		-1
#define	AL_FREE		-2

struct a_list {
	int	al_head;
	int	al_next[MAX_NUM_NODES];
};

#define	a_first(al)	((al)->al_head)

#define a_enqueue(al, index)\
{\
	if ((al)->al_next[index] == AL_FREE) {\
		(al)->al_next[index] = (al)->al_head;\
		(al)->al_head = (index);\
	}\
}

#define a_dequeue(al, index)\
{\
	(al)->al_head = (al)->al_next[index];\
	(al)->al_next[index] = AL_FREE;\
}

a_init(al)
	struct a_list *al;
{
	int i;

	al->al_head = AL_END;
	for (i = 0; i < MAX_NUM_NODES; i++) {
		al->al_next[i] = AL_FREE;
	}
}

struct a_list	netipc_pending_send;
struct a_list	netipc_pending_unack;
struct a_list	netipc_pending_timeout;

/* XXX must make sure seqids never wrap to 0 */

#define	DEBUG	0

#if	DEBUG
int			_netipc_timeout = 5000000;	/* 5 sec */
int			Noise = 1;
#else	/*DEBUG*/
#if	NORMA_ETHER
int			_netipc_timeout = 20000;	/* 20 msec */
#else
/*
 * XXX
 * 
 * For the ipsc, things screw up (particularly with remote ethernet
 * driver) with too small a timeout. Furthermore, dropped packets
 * should be much rarer since they only result from lack of buffer space.
 */
int			_netipc_timeout = 1000000;	/* 1 sec */
#endif
int			Noise = 0;
#endif	/*DEBUG*/

#define	NETIPC_TIMEOUT	_netipc_timeout

int Noise2 = 0;
int Noise6 = 0;
int Noise7 = 0;

int netipc_ackdelay = 0;	/* XXX should be based on message type */

/*
 * A timer for a remote node gets set to 1 when a message is sent
 * to that node. Every so many milliseconds, the timer value is
 * incremented. When it reaches a certain value (currently 2),
 * a sync message is sent to see whether we should retransmit.
 *
 * The timer SHOULD be set to 0 when the message is acknowledged.
 */
netipc_timeout(usec)
	unsigned long usec;
{
	register int remote;
	register struct a_list *pending_timeout;
	register int *al_prev;

	if ((netipc_usec += usec) < NETIPC_TIMEOUT) {
		return;
	}
	netipc_usec = 0;
	pending_timeout = &netipc_pending_timeout;
	for (al_prev = &pending_timeout->al_head; (remote = *al_prev) >= 0; ) {
		register int useful = 0;

		if (netipc_will_ack[remote]) {
			unsigned long ctl_seqid = netipc_will_ack[remote];

			useful = 1;
			netipc_will_ack[remote] = 0;
			netipc_send_ctl(remote, CTL_ACK, ctl_seqid);
		}
		if (netipc_timer[remote]) {
			useful = 1;
			if (netipc_timer[remote] == 2) {
				/*
				 * There may or may not be anything left
				 * to be acknowledged.
				 */
				int j, seqid = 0;

				j = netipc_sent_seqid[remote] -
				    MAX_WINDOW_SIZE + 1;
				if (j < 0) {
					j = 0;
				}
				for (; j <= netipc_sent_seqid[remote]; j++) {
					if (netipc_sent_packet[remote]
					    [j % WX]) {
						seqid = j;
						break;
					}
				}
				if (seqid == 0) {
					netipc_timer[remote] = 0;
					continue;
				}
				c_netipc_timeout++;
				if(Noise2)printf("timeout %d\n", remote);
				netipc_send_ctl(remote, CTL_SYNC, seqid);
			} else {
				netipc_timer[remote] = 2;
			}
		}
		if (useful) {
			al_prev = &pending_timeout->al_next[remote];
		} else {
			*al_prev = pending_timeout->al_next[remote];
			pending_timeout->al_next[remote] = AL_FREE;
		}
	}
}

extern netipc_packet_t	netipc_set_seqid();
extern			netipc_recv();
extern vm_page_t	netipc_page_grab();

struct netipc_hdr	netipc_recv_hdr;
vm_page_t		netipc_recv_page;
vm_page_t		netipc_fallback_page = VM_PAGE_NULL;

netipc_init_send()
{
	int i;

	netvec_k[0].addr = KVTODEV(&send_hdr_k);
	netvec_k[0].size = sizeof(struct netipc_hdr);

	netvec_a[0].addr = KVTODEV(&send_hdr_a);
	netvec_a[0].size = sizeof(struct netipc_hdr);
	netvec_a[1].addr = KVTODEV(&send_hdr_a);
	netvec_a[1].size = sizeof(unsigned long);

	netvec_p[0].addr = KVTODEV(&send_hdr_p);
	netvec_p[0].size = sizeof(struct netipc_hdr);

	/*
	 * Initialize send_hdr_p
	 */
	send_hdr_p.type = NETIPC_TYPE_PAGE;

	/*
	 * Initialize send_hdr_k
	 */
	send_hdr_k.type = NETIPC_TYPE_KMSG;
	send_hdr_k.remote = node_self();
	send_hdr_k.data_allign = 0;

	/*
	 * Initialize send_hdr_a
	 */
	send_hdr_a.type = NETIPC_TYPE_CTL;
	send_hdr_a.remote = node_self();

	for (i = 0; i < MAX_NUM_NODES; i++) {
		netipc_last_unacked[i] = 1;
	}

	a_init(&netipc_pending_send);
	a_init(&netipc_pending_unack);
	a_init(&netipc_pending_timeout);
}

int norma_ipc_msgid = 1;

/*
 * Called with interrupts NOT blocked (XXX but how do we enforce that?)
 *
 * The only options we have to worry about are
 * MACH_SEND_TIMEOUT and MACH_SEND_ALWAYS.
 */
mach_msg_return_t
norma_ipc_send(kmsg, option)
	ipc_kmsg_t kmsg;
	mach_msg_option_t option;
{
	register mach_msg_header_t *msgh;
	ipc_port_t local_port, remote_port;
	int remote, s;
	register netipc_packet_t dp;

	msgh = (mach_msg_header_t *) &kmsg->ikm_header;
	remote_port = (ipc_port_t) msgh->msgh_remote_port;
	local_port = (ipc_port_t) msgh->msgh_local_port;

	/*
	 * Get the receiver's uid. There had better be one!
	 */
	assert(remote_port->ip_norma_uid != 0);
	msgh->msgh_remote_port = (mach_port_t) remote_port->ip_norma_uid;
	remote = ip_norma_node(msgh->msgh_remote_port);

	/*
	 * If there is a local port, get its uid (creating one if necessary).
	 */
	if (local_port) {
		ip_lock(local_port);
		if (! local_port->ip_norma_uid) {
			norma_ipc_export(local_port);
		}
		msgh->msgh_local_port = (mach_port_t) local_port->ip_norma_uid;
		ip_unlock(local_port);
	}

	/*
	 * Construct netipc_packet_t.
	 */
	dp = (netipc_packet_t) netipc_copy_grab(); /* XXX! */
	dp->dp_msgid = norma_ipc_msgid++;
	dp->dp_kmsg = kmsg;
	dp->dp_offset = 0;
	dp->dp_length =  msgh->msgh_size + IKM_OVERHEAD,
	dp->dp_next = 0;

	/*
	 * Round length to word boundary.
	 * XXX can't we do this somewhere else?
	 */
	if (dp->dp_length & 03) {
		dp->dp_length = (dp->dp_length + 03) & ~03;
	}

	/*
	 * Queue the packet.
	 */
	s = sploff();
	if (netipc_packet[remote]) {
		netipc_packet_t dp0;

		for (dp0 = netipc_packet[remote];
		     dp0->dp_next;
		     dp0 = dp0->dp_next) {
			continue;
		}
		dp0->dp_next = dp;
		dp->dp_offset = 1; /* xxx means uninitialized */
	} else {
		dp->dp_first_seqid = netipc_sent_seqid[remote] + 1;
		if ((dp->dp_kmsg->ikm_header.msgh_bits
		     & MACH_MSGH_BITS_COMPLEX_DATA) == 0) {
			dp->dp_last_seqid = dp->dp_first_seqid;
		} else {
			dp->dp_last_seqid = 0;
		}
		dp->dp_offset = 0;
		netipc_packet[remote] = dp;
	}

	/*
	 * Send it if we can.
	 */
	netipc_send_any(remote);
	splon(s);
	return KERN_SUCCESS;
}

netipc_send_any(remote)
	int remote;
{
	register netipc_packet_t dp;
	int seqid = netipc_sent_seqid[remote] + 1;

	if (seqid < MAX_WINDOW_SIZE + netipc_last_unacked[remote]
	    && (dp = netipc_set_seqid(remote, seqid))){
		if (netipc_sending) {
			netipc_do_send[remote] = TRUE;
			a_enqueue(&netipc_pending_send, remote);
			return;
		}
		netipc_sent_seqid[remote] = seqid;
		netipc_sent_packet[remote][seqid % WX] = dp;
		if (dp->dp_offset == 0) {
			netipc_send_kmsg(remote, dp);
		} else {
			netipc_send_page(remote, dp);
		}
	} else {
		netipc_do_send[remote] = FALSE;
	}
}

/*
 * Called from either norma_ipc_send or netipc_retry.
 *
 * If called from norma_ipc_send, interrupts will and must
 * not be blocked, since we may need to wait for netipc_sending
 * to be false.
 * If called from netipc_retry, interrupts will be blocked, but
 * netipc_sending will already be false.
 */
netipc_send_kmsg(remote, dp)
	int remote;
	register netipc_packet_t dp;
{
	int netvec_count;

	assert(! netipc_sending);
	netipc_sending = TRUE;

	/*
	 * Kmsgs are word alligned.
	 */
	assert(((vm_offset_t) dp->dp_kmsg & 03) == 0);

	/*
	 * Fill in send_hdr_k.
	 */
	send_hdr_k.msgid = dp->dp_msgid;
	send_hdr_k.seqid = dp->dp_seqid;
	send_hdr_k.length = dp->dp_length;
	send_hdr_k.ctl = CTL_NONE;

	/*
	 * Fill in netvec_k.
	 * Cache KVTODEV and page-splitting computations.
	 * (Kmsgs occasionally cross page boundaries, unfortunately.)
	 *
	 * This routine attempts to cache the results of KVTODEV
	 * since it is relatively expensive.
	 * This caching may be less effective now since we've added
	 * flow control, since we don't immediately stuff the kmsg back
	 * into the ikm_cache, which means it might not be the kmsg
	 * we see next. Perhaps we can use the kmsg->ikm_page field
	 * for caching physaddr?
	 */
	if (dp->dp_kmsg != netipc_kmsg_cache) {
		vm_offset_t data = (vm_offset_t) dp->dp_kmsg;
		netipc_kmsg_cache = dp->dp_kmsg;

		netipc_kmsg_first_half = round_page(data) - data;
			
		netvec_k[1].addr = KVTODEV(data);
		netvec_k[2].addr = KVTODEV(data + netipc_kmsg_first_half);
		netipc_kmsg_cache_misses++;
	} else {
		netipc_kmsg_cache_hits++;
	}
	if (dp->dp_length > netipc_kmsg_first_half) {
		netvec_k[1].size = netipc_kmsg_first_half;
		netvec_k[2].size = dp->dp_length - netipc_kmsg_first_half;
		netvec_count = 3;
		netipc_kmsg_splits++;
	} else {
		netvec_k[1].size = dp->dp_length;
		netvec_count = 2;
	}

	/*
	 * Start the send, and the associated timer.
	 */
	netipc_send(remote, netvec_k, netvec_count);
	netipc_timer[remote] = 1;
	a_enqueue(&netipc_pending_timeout, remote);
}

/*
 * Called from netipc_retry.
 * See netipc_send_kmsg for more details, since its use
 * and internals are similar.
 */
netipc_send_page(remote, dp)
	int remote;
	register netipc_packet_t dp;
{
	vm_page_t *page_list = &dp->dp_copy->cpy_page_list[dp->dp_copy_index];
	register int allign;
	unsigned long length;
	register vm_offset_t offset;



	assert(! netipc_sending);
	netipc_sending = TRUE;

	/*
	 * Round beginning and end of data to word boundaries.
	 */
#if 1
	if (dp->dp_copy_index >= dp->dp_copy->cpy_npages) {
		printf("dp=0x%x idx=%d npg=%d copy=0x%x\n",
		       dp,
		       dp->dp_copy_index,
		       dp->dp_copy->cpy_npages,
		       dp->dp_copy);
	}
#endif
	assert(dp->dp_copy_index < dp->dp_copy->cpy_npages);
	if (dp->dp_copy_index == dp->dp_copy->cpy_npages - 1
	    && (dp->dp_copy->size & page_mask)) {
		length = dp->dp_copy->size & page_mask;
	} else {
		length = PAGE_SIZE;
	}
	offset = dp->dp_copy->offset & page_mask;
	if (allign = (offset & 03)) {
		length += allign;
		offset &= ~03;
	}
	if (length & 03) {
		length = (length + 03) & ~03;
	}

	send_hdr_p.data_allign = allign;
	send_hdr_p.pg.pg_msgh_offset = dp->dp_offset;
	send_hdr_p.pg.pg_copy_index = dp->dp_copy_index;
	send_hdr_p.pg.pg_copy_npages = dp->dp_copy->cpy_npages;
	send_hdr_p.length = dp->dp_copy->size;
	send_hdr_p.pg.pg_copy_last = dp->dp_copy_last;
	send_hdr_p.msgid = dp->dp_msgid;
	send_hdr_p.seqid = dp->dp_seqid;
	send_hdr_p.remote = node_self();
	send_hdr_p.ctl = CTL_NONE;

	/*
	 * The allignment will be different on the receiving side
	 * then it is here, since data is received into pages starting
	 * at the beginning, whereas it is sent from whereever the
	 * interesting stuff lives, spanning page boundaries in some cases.
	 * In some cases, data will be received in one fewer pages than it
	 * was sent from.
	 */
	if (offset + length > PAGE_SIZE) {
		send_hdr_p.pg.pg_copy_npages--;
	}
	/*
	 * If data crosses a page boundary, we need to point netvec_p
	 * to both physical pages involved.
	 */
	if (offset + length > PAGE_SIZE) {
		vm_offset_t first_half = PAGE_SIZE - offset;

		netvec_p[1].addr = VPTODEV(page_list[0]) + offset;
		netvec_p[1].size = first_half;

		netvec_p[2].addr = VPTODEV(page_list[1]);
		netvec_p[2].size = length - first_half;

		netipc_send(remote, netvec_p, 3);
	} else {
		netvec_p[1].addr = VPTODEV(page_list[0]) + offset;
		netvec_p[1].size = length;

		netipc_send(remote, netvec_p, 2);
	}
	netipc_timer[remote] = 1;
	a_enqueue(&netipc_pending_timeout, remote);
}

/*
 * Called with interrupts blocked, from netipc_recv_intr,
 * when a nack is received.
 * We will be stopped waiting for an ack; resending does not change this.
 *
 * The code below asserts that we are not sending. This is not necessarily
 * the case! (It is the case if we only ever send to one node.) What we
 * should do if we are sending is flip a bit somewhere which netipc_send_intr
 * can examine so that it can do the send. In fact, we should generalize this
 * so that no one has to spin waiting for netipc_sending to become false.
 */
netipc_retry(remote, seqid)
	int remote;
	int seqid;
{
	register netipc_packet_t dp;

	if (netipc_sending) {
		/*
		 * XXX unfortunate but not fatal
		 */
		return;
	}

#if 1
	/*
	 * XXX
	 * Must carefully check validity of requested seqid, and not complain.
	 * We, not set_seqid, are responsible for these checks.
	 */
	if (seqid > netipc_sent_seqid[remote]) {
		printf("%s:%d %d > %d\n",
		       __FILE__, __LINE__, seqid, netipc_sent_seqid[remote]);
	}
	if (seqid <= netipc_sent_seqid[remote] - MAX_WINDOW_SIZE) {
		printf("%s:%d %d <= %d\n",
		       __FILE__, __LINE__, seqid,
		       netipc_sent_seqid[remote] - MAX_WINDOW_SIZE);
	}
	if (netipc_sent_packet[remote][seqid % WX] == 0) {
		printf("%s:%d %d acked!\n",
		       __FILE__, __LINE__, seqid, netipc_sent_seqid[remote]);
	}
#endif
	netipc_timer[remote] = 1;
	a_enqueue(&netipc_pending_timeout, remote);

	dp = netipc_set_seqid(remote, seqid);
	assert(dp);
	if (dp->dp_offset == 0) {
		c_netipc_retry_k++;
		netipc_send_kmsg(remote, dp);
	} else {
		c_netipc_retry_p++;
		netipc_send_page(remote, dp);
	}
}

/*
 * When we receive a positive ack, we can free up the thing we were sending.
 * If it was a kmsg, try to stuff it in the ikm_cache.
 * If it was the last page in a copy object, discard the copy object.
 *
 * We really should be freeing up the pages one at a time.
 * This gets into the issue of how to free pages that are not being stolen
 * but rather borrowed. A borrowed page may still be mapped writable by
 * the sending thread. Someone thus needs to either block the thread
 * until we indicate that we are done with the page, or write-protect
 * the page (using a raw pmap operation) and either block a faulting
 * thread or copy the page upon faulting. In any case, we should call
 * a routine for each page that undoes whatever gets done to prevent
 * the thread from writing the page. (Note, by the way, that other
 * threads are allowed to write to the page while the sending thread
 * is still in the kernel sending, since such writing is indistinguishable
 * from writing just before the sending thread entered the kernel.)
 *
 * This routine is called at interrupt level...
 * XXX Is it okay to call discard from interrupt level? NO!
 */
netipc_next_copy_object(dp)
	register netipc_packet_t dp;
{
	vm_offset_t saddr, eaddr;
	ipc_kmsg_t kmsg = dp->dp_kmsg;

	assert(dp->dp_offset >= sizeof(dp->dp_kmsg->ikm_header));
	assert(dp->dp_offset < kmsg->ikm_header.msgh_size);

	saddr = (vm_offset_t) &kmsg->ikm_header + dp->dp_offset;
	eaddr = (vm_offset_t) &kmsg->ikm_header + kmsg->ikm_header.msgh_size;

	dp->dp_copy = VM_MAP_COPY_NULL;
	dp->dp_copy_index = 0;
	dp->dp_copy_last = TRUE;
	while (saddr < eaddr) {
		mach_msg_type_long_t *type;
		mach_msg_type_name_t name;
		mach_msg_type_size_t size;	/* xxx */
		mach_msg_type_number_t number;	/* xxx */
		vm_size_t length;

		type = (mach_msg_type_long_t *) saddr;
		if (type->msgtl_header.msgt_longform) {
			name = type->msgtl_name;
			size = type->msgtl_size;
			number = type->msgtl_number;
			saddr += sizeof(mach_msg_type_long_t);
		} else {
			name = type->msgtl_header.msgt_name;
			size = type->msgtl_header.msgt_size;
			number = type->msgtl_header.msgt_number;
			saddr += sizeof(mach_msg_type_t);
		}

		/* calculate length of data in bytes, rounding up */

		length = ((number * size) + 7) >> 3;

		if (type->msgtl_header.msgt_inline) {
			/* inline data sizes round up to int boundaries */
			saddr += (length + 3) &~ 3;
			continue;
		}

		/*
		 * XXX This is required because net_deliver does
		 * XXX funny rounding to msgh_size.
		 * XXX Why doesn't anything in ipc/ipc_kmsg.c need this?
		 */
		if (saddr >= eaddr) {
			break;
		}

		if (MACH_MSG_TYPE_PORT_ANY(name)) {
			panic("out-of-line port");
		}

		if (dp->dp_copy) {
			dp->dp_copy_last = FALSE;
			break;
		}

		dp->dp_copy = * (vm_map_copy_t *) saddr;
		dp->dp_offset = saddr - (vm_offset_t)&kmsg->ikm_header;

		if (dp->dp_copy->type != VM_MAP_COPY_PAGE_LIST) panic("xcdf1");
		if (dp->dp_copy->size != length) panic("xcdf2");

		saddr += sizeof(vm_offset_t);
	}
	assert(dp->dp_copy);
}

netipc_packet_t
netipc_set_seqid(remote, seqid)
	int remote;
	int seqid;
{	
	netipc_packet_t dp, _netipc_set_seqid();
	int s;

	s = splhigh();
	dp = _netipc_set_seqid(remote, seqid);
	splx(s);
	return dp;
}

/*
 * XXX Here, and who knows where else: cpy_npages may be one more
 * XXX than the seqid increment for a copy object!!!
 *
 * Note: dp_last_seqid is set when we first visit it.
 */
netipc_packet_t
_netipc_set_seqid(remote, seqid)
	int remote;
	int seqid;
{
	register netipc_packet_t dp;

	/*
	 * Skip past dp's until we are either in the right
	 * dp, or we are in the dp preceeding the right one
	 * where the right one has not yet been initialized.
	 * Note that we never attempt to set seqid more than
	 * one beyond a previously explored seqid.
	 */
	for (dp = netipc_packet[remote];
	     dp->dp_next
	     && dp->dp_next->dp_offset != 1
	     && dp->dp_next->dp_first_seqid <= seqid;
	     dp = dp->dp_next) {
		continue;
	}

	/*
	 * We may be at the end of one dp and need to enter another.
	 * This next dp will need to be initialized.
	 * The seqid will correspond to a kmsg.
	 */
	if (dp->dp_last_seqid && dp->dp_last_seqid < seqid) {
		assert(dp->dp_last_seqid == seqid - 1);
		dp = dp->dp_next;
		if (! dp) {
			return (netipc_packet_t) 0;
		}
		dp->dp_seqid = seqid;
		dp->dp_first_seqid = seqid;
		if ((dp->dp_kmsg->ikm_header.msgh_bits
		     & MACH_MSGH_BITS_COMPLEX_DATA) == 0) {
			dp->dp_last_seqid = seqid;
		} else {
			dp->dp_last_seqid = 0;
		}
		dp->dp_offset = 0;
		return dp;
	}

	/*
	 * We are now in the correct dp.
	 */
	assert(dp->dp_first_seqid <= seqid);
	assert(dp->dp_last_seqid == 0 || seqid <= dp->dp_last_seqid);

	/*
	 * If we want to be at the kmsg, go there.
	 */
	if (seqid == dp->dp_first_seqid) {
		dp->dp_offset = 0;
		dp->dp_seqid = seqid;
		return dp;
	}
	  
	/*
	 * If we are in the right copy object, just change the index.
	 */
	if (dp->dp_offset) {
		int index = dp->dp_copy_index + (seqid - dp->dp_seqid);
		if (index >= 0 && index < dp->dp_copy->cpy_npages) {
			dp->dp_copy_index = index;
			dp->dp_seqid = seqid;
			return dp;
		}
	}

	/*
	 * We might be too far forward and thus need to back up.
	 * The easiest way of backing up is to start at the kmsg
	 * and walk forward. This isn't necessary the most efficient way!
	 *
	 * Note that this cannot happen if this is a simple message.
	 */
	if (seqid < dp->dp_seqid) {
		dp->dp_seqid = dp->dp_first_seqid;
	}

	/*
	 * If we are currently at the kmsg, advance to the first copy object.
	 */
	if (dp->dp_seqid == dp->dp_first_seqid) {
		dp->dp_seqid++;
		dp->dp_offset = sizeof(dp->dp_kmsg->ikm_header);
	}

	/*
	 * Examine each copy object to see whether it contains seqid.
	 * If it does, set index appropriately and return.
	 */
	for (;;) {
		netipc_next_copy_object(dp);
		if (dp->dp_copy_last && dp->dp_last_seqid == 0) {
			dp->dp_last_seqid =
			    dp->dp_seqid + dp->dp_copy->cpy_npages - 1;
		}
		assert(seqid >= dp->dp_seqid);
		if (seqid < dp->dp_seqid + dp->dp_copy->cpy_npages) {
			dp->dp_copy_index = seqid - dp->dp_seqid;
			dp->dp_seqid = seqid;
			return dp;
		}
		assert(! dp->dp_copy_last);
		dp->dp_seqid += dp->dp_copy->cpy_npages;
	}
}

netipc_acknowledged(remote, seqid)
	int remote;
	int seqid;
{
	register netipc_packet_t dp;

	/*
	 * Ignore obsolete acks.
	 */
	if (seqid < netipc_last_unacked[remote]) {
		return;
	}
	if (! (dp = netipc_sent_packet[remote][seqid % WX])) {
		return;
	}

	/*
	 * Acknowledge the seqid, possibly freeing the kmsg.
	 * XXX Copy objects should only be freed now!
	 *
	 * XXX Actually... at a certain point we are committed,
	 * XXX since we cannot back up beyond page_list boundaries
	 * XXX in copy objects. What a mess, I'll fix it later!
	 *
	 * Reasons for backing up, beyond acknowledgements:
	 *	1. Port moved
	 *	2. Port moved and old port's home node crashes
	 */
	netipc_sent_packet[remote][seqid % WX] = 0;
	if (seqid == netipc_last_unacked[remote]) {
		/*
		 * Update lack_unacked, skipping over already-acked packets.
		 */
		register int u = netipc_last_unacked[remote];
		for (; u <= netipc_sent_seqid[remote]; u++) {
			if (netipc_sent_packet[remote][u % WX]) {
				break;
			}
		}
		assert(u <= netipc_sent_seqid[remote] + 1);
		netipc_last_unacked[remote] = u;
	}
	/*netipc_timer[remote][seqid % WX] = 0; -- unnesc */

	while (dp && dp->dp_last_seqid
	    && dp->dp_last_seqid < netipc_last_unacked[remote]) {
		/*
		 * Free kmsg.
		 */

		if(Noise)printf("-free %d..%d\n",
				dp->dp_first_seqid, dp->dp_last_seqid);
		if (ikm_cache() == IKM_NULL
		    && dp->dp_kmsg->ikm_size == IKM_SAVED_KMSG_SIZE) {
			ikm_cache() = dp->dp_kmsg;
		} else {
			ikm_free(dp->dp_kmsg);
		}

		/*
		 * Free last copy objects.
		 *
		 * XXX actually should just free last page list.
		 * XXX should have freed all previous copy objects.
		 */
		if (dp->dp_kmsg->ikm_header.msgh_bits
		    & MACH_MSGH_BITS_COMPLEX_DATA) {
			if (! dp->dp_copy) {
				netipc_set_seqid(remote, dp->dp_last_seqid);
				assert(dp->dp_copy);
			}
			vm_map_copy_discard(dp->dp_copy);
		}

		/*
		 * Discard the dp; if there are no more, return.
		 */
		netipc_packet[remote] = dp->dp_next;
		netipc_copy_ungrab(dp);
		if (! (dp = netipc_packet[remote])) {
			return;
		}
		if (dp->dp_offset == 1) {
			/* xxx uninitialized */
			dp->dp_first_seqid = netipc_sent_seqid[remote] + 1;
			if ((dp->dp_kmsg->ikm_header.msgh_bits
			     & MACH_MSGH_BITS_COMPLEX_DATA) == 0) {
				dp->dp_last_seqid = dp->dp_first_seqid;
			} else {
				dp->dp_last_seqid = 0;
			}
			dp->dp_offset = 0;
		}
	}

	/*
	 * Send a new packet if there is one to send and if it is within
	 * the window.
	 */
	netipc_send_any(remote);
}

/*
 * Called with interrupts blocked, from netipc_self_unstop and
 * netipc_recv_intr.  Ctl may be either CTL_ACK or CTL_NACK.  If we are
 * currently sending, use netipc_pending_ctl to have netipc_send_intr do
 * the send when the current send completes.
 *
 * The netipc_pending_ctl mechanism should be generalized to allow for
 * arbitrary pending send operations, so that no one needs to spin on
 * netipc_sending.
 */
netipc_send_ctl(remote, ctl, ctl_seqid)
	int remote;
	int ctl;
	int ctl_seqid;
{
	/*
	 * If net is busy sending, let netipc_send_intr send the ctl.
	 */
	if (netipc_sending) {
		/*
		 * We may blow away a preceeding ctl, which is unfortunate
		 * but not fatal.
		 */
		netipc_ctl[remote] = ctl;
		netipc_ctl_seqid[remote] = ctl_seqid;
		a_enqueue(&netipc_pending_send, remote);
		return;
	}
	netipc_sending = TRUE;

	/*
	 * Fill in send_hdr_a.
	 */
	send_hdr_a.ctl = ctl;
	send_hdr_a.ctl_seqid = ctl_seqid;

	/*
	 * Start the send.
	 */
	netipc_send(remote, netvec_a, 2);
}

/*
 * Net send interrupt routine: called when a send completes.
 * If there is something else to send (currently, only ctls),
 * send it; otherwise, set netipc_sending FALSE.
 */
netipc_send_intr()
{
	int s = splhigh();
	_netipc_send_intr();
	splx(s);
}

_netipc_send_intr()
{
	register int remote;

	/*
	 * Scan the pending list, looking for something to send.
	 * If something is on the list but doesn't belong, remove it.
	 */
	while ((remote = a_first(&netipc_pending_send)) >= 0) {
		if (netipc_ctl[remote] != CTL_NONE) {
			/*
			 * There is a ctl to send; send it.
			 */
			send_hdr_a.ctl = netipc_ctl[remote];
			send_hdr_a.ctl_seqid = netipc_ctl_seqid[remote];
			netipc_ctl[remote] = CTL_NONE;
			netipc_send(remote, netvec_a, 2);
			return;
		}
		if (netipc_do_send[remote]) {
			/*
			 * There may be a message to send; try to send it.
			 */
			netipc_send_any(remote);
			if (netipc_sending) {
				return;
			}
		}
		/*
		 * There was nothing to send to this node.
		 * Remove it from the list and continue scanning.
		 */
		a_dequeue(&netipc_pending_send, remote);
	}
	/*
	 * The list is empty; don't send anything.
	 */
	netipc_sending = FALSE;
}

/* ------------------------------ RECV side ------------------------------ */

/*
 * XXX Still need to worry about running out of copy objects
 */

norma_kmsg_put(kmsg)
	ipc_kmsg_t kmsg;
{
	netipc_page_put(kmsg->ikm_page);
}

netipc_init_recv()
{
	netvec_r[0].addr = KVTODEV(&netipc_recv_hdr);
	netvec_r[0].size = sizeof(struct netipc_hdr);
	netvec_r[1].size = PAGE_SIZE;


	/*
	 * Initialize netipc_recv_hdr
	 */
	netipc_recv_hdr.type = NETIPC_TYPE_INVALID;

	/*
	 * Start receiving.
	 */
	netipc_recv_page = netipc_fallback_page = vm_page_grab();
	assert(netipc_recv_page != VM_PAGE_NULL);
	netipc_self_stopped = TRUE;
	netvec_r[1].addr = VPTODEV(netipc_recv_page);
	netipc_recv(netvec_r, 2);
}

/*
 * Called with interrupts blocked when a page becomes available.
 * Replaces current dummy page with new page, so that
 * any incoming page will be valid.
 * Note that with dma, a receiving may currently be happening.
 *
 * This has the bonus of saving a retransmit if we find a page
 * quickly enough. I don't know how often this will happen.
 *----
 * up to date comment:
 *	shouldn't transfer all at once since we might stop again
 */
netipc_self_unstop()
{
	register int remote;
	register struct a_list *pending_unack = &netipc_pending_unack;

	c_netipc_unstop++;
	assert(netipc_self_stopped);
	netipc_fallback_page = netipc_page_grab();
	assert(netipc_fallback_page != VM_PAGE_NULL);
	netipc_self_stopped = FALSE;

	while ((remote = a_first(pending_unack)) > 0) {
		a_dequeue(pending_unack, remote);
		netipc_send_ctl(remote, CTL_NACK, netipc_rcvd_seqid[remote]);
	}
}

int netipc_drop_freq = 0;
int netipc_drop_counter = 0;

netipc_recv_intr()
{
	int s = splhigh();
	_netipc_recv_intr();
	splx(s);
}

_netipc_recv_intr()
{
	register int remote = netipc_recv_hdr.remote;
	register int seqid = netipc_recv_hdr.seqid;
	register int type = netipc_recv_hdr.type;
	register int ctl = netipc_recv_hdr.ctl;
	register int ctl_seqid = netipc_recv_hdr.ctl_seqid;

#if 1
	if (netipc_drop_freq) {
		if (++netipc_drop_counter >= netipc_drop_freq) {
			/*
			 * Reset counter, drop packet, and rearm.
			 */
			netipc_drop_counter = 0;
			netipc_recv(netvec_r, 2);
			return;
		}
	}
#endif
	/*
	 * Send ctls immediately.
	 * Eventually, we should wait and try to piggyback these ctls.
	 * This could lead to deadlock if we aren't tricky.
	 * We should for example send ctls as soon as we are idle.
	 */
	netipc_print('r', type, remote, ctl, seqid, ctl_seqid);
	if (type == NETIPC_TYPE_KMSG || type == NETIPC_TYPE_PAGE) {
#if	MACH_ETHER
		if (seqid <= netipc_rcvd_seqid[remote]) {
#else
		/* XXXX */
		if (seqid < netipc_rcvd_seqid[remote]) {
#endif
			c_netipc_old_recv++;
			netipc_recv(netvec_r, 2);
			return;
		}
		netipc_rcvd_seqid[remote] = seqid;
		if (netipc_self_stopped) {
			a_enqueue(&netipc_pending_unack, remote);
		} else if (netipc_ackdelay) {
			a_enqueue(&netipc_pending_timeout, remote);
			netipc_will_ack[remote] = seqid;
		} else {
			netipc_send_ctl(remote, CTL_ACK, seqid);
		}
	} else if (type != NETIPC_TYPE_CTL) {
		printf("netipc_recv_intr: bad type 0x%x\n", type);
		netipc_recv(netvec_r, 2);
		return;
	}

	/*
	 * Process incoming ctl, if any.
	 */
	if (ctl == CTL_NONE) {
		/* nothing to do */
	} else if (ctl == CTL_SYNC) {
		if(Noise2)printf("synch %d\n", ctl_seqid);
		if (ctl_seqid != netipc_rcvd_seqid[remote]) {
			netipc_send_ctl(remote, CTL_NACK, ctl_seqid);
		} else if (netipc_pending_unack.al_next[remote] == AL_FREE) {
			if (netipc_ackdelay) {
				/* XXX should ack if is at end of window? */
				a_enqueue(&netipc_pending_timeout, remote);
				netipc_will_ack[remote] = ctl_seqid;
			} else {
				netipc_send_ctl(remote, CTL_ACK, ctl_seqid);
			}
		}
		/* else quench? */
	} else if (ctl == CTL_ACK) {
		netipc_acknowledged(remote, ctl_seqid);
	} else if (ctl == CTL_NACK) {
		netipc_retry(remote, ctl_seqid);
	} else {
		printf("%d: ctl?%d %d\n", node_self(), ctl, remote);
	}

	/*
	 * If we are stopped, or if this was just an ctl,
	 * then restart receive with same buffer.
	 */
	if (netipc_self_stopped || type == NETIPC_TYPE_CTL) {
		netipc_recv(netvec_r, 2);
		return;
	}

	/*
	 * Deliver message according to its type.
	 */
	if (type == NETIPC_TYPE_KMSG) {
		register ipc_kmsg_t kmsg;
		/*
		 * This is a kmsg. Kmsgs are word alligned,
		 * and contain their own length.
		 */
		kmsg = (ipc_kmsg_t) VPTOKV(netipc_recv_page);
		kmsg->ikm_size = IKM_SIZE_NORMA;
		kmsg->ikm_marequest = IMAR_NULL;
		kmsg->ikm_page = netipc_recv_page;
if(Noise6)printf("norma_deliver_kmsg remote=%d msgid=%d\n",
       netipc_recv_hdr.remote,
       netipc_recv_hdr.msgid);
if(Noise7)ipc_kmsg_print(kmsg);
		norma_deliver_kmsg(kmsg,
				   netipc_recv_hdr.remote,
				   netipc_recv_hdr.msgid);
	} else {
		/*
		 * This is out-of-line data.
		 */
/*printf("norma_deliver_page\n");*/
		norma_deliver_page(netipc_recv_page,
				   netipc_recv_hdr.pg.pg_msgh_offset,
				   netipc_recv_hdr.remote,
				   netipc_recv_hdr.pg.pg_copy_index,
				   netipc_recv_hdr.pg.pg_copy_npages,
				   netipc_recv_hdr.length,
				   netipc_recv_hdr.data_allign,
				   netipc_recv_hdr.msgid);
	}

	/*
	 * Start a new receive.
	 * XXX should we have done this before the deliver calls above?
	 */
	netipc_recv_hdr.type = NETIPC_TYPE_INVALID;
	netipc_recv_page = netipc_page_grab();
	if (netipc_recv_page == VM_PAGE_NULL) {
		c_netipc_stop++;
		netipc_self_stopped = TRUE;
		netipc_recv_page = netipc_fallback_page;
	}
	netvec_r[1].addr = VPTODEV(netipc_recv_page);
	netipc_recv(netvec_r, 2);
}

/* ------------------------------ Allocation ------------------------------ */

boolean_t	netipc_thread_awake = FALSE;
int		netipc_thread_awaken = 0;
vm_page_t	netipc_page_list = VM_PAGE_NULL;
int		netipc_page_list_count = 0;
int		netipc_page_list_low = 20;
int		netipc_page_list_high = 30;

/* XXX Should just zalloc from one zone and zfree to the other? */
typedef struct q *q_t;
struct q {
	q_t	next;
};

q_t	vm_map_copy_q;
int	vm_map_copy_q_count;

extern zone_t vm_map_copy_zone;

/*q_t*/
netipc_copy_grab()
{
	int s;
	q_t q;

	s = sploff();
	q = vm_map_copy_q;
	if (! q) {
		splon(s);
		panic("netipc_copy_grab");
	}
	vm_map_copy_q = q->next;
	vm_map_copy_q_count--;
	splon(s);
	return (int) q; /* xxx */
}

netipc_copy_ungrab(q)
	q_t q;
{
	int s;

	s = sploff();
	q->next = vm_map_copy_q;
	vm_map_copy_q = q;
	vm_map_copy_q_count++;
	splon(s);
}

netipc_page_put(m)
	vm_page_t m;
{
	int s;

	s = sploff();
	* (vm_page_t *) &m->pageq.next = netipc_page_list;
	netipc_page_list = m;
	netipc_page_list_count++;
	if (netipc_self_stopped) {
		netipc_self_unstop();
	}
	splon(s);
}

netipc_replenish(always)
	boolean_t always;
{
	vm_page_t m;
	int s;

	while (vm_map_copy_q_count < 300) {
		q_t q;

		q = (q_t) zalloc(vm_map_copy_zone);
		s = sploff();
		q->next = vm_map_copy_q;
		vm_map_copy_q = q;
		vm_map_copy_q_count++;
		splon(s);
	}
	while (netipc_page_list_count < netipc_page_list_high) {
		m = vm_page_grab();
		if (m == VM_PAGE_NULL) {
			break;
		}
		m->tabled = FALSE;
		vm_page_init(m, m->phys_addr);

		s = sploff();
		* (vm_page_t *) &m->pageq.next = netipc_page_list;
		netipc_page_list = m;
		netipc_page_list_count++;
		if (netipc_self_stopped) {
			netipc_self_unstop();
		}
		splon(s);
	}
	while (always && netipc_page_list_count < netipc_page_list_low) {
		while ((m = vm_page_grab()) == VM_PAGE_NULL) {
			vm_page_wait(0);
		}
		m->tabled = FALSE;
		vm_page_init(m, m->phys_addr);

		s = sploff();
		* (vm_page_t *) &m->pageq.next = netipc_page_list;
		netipc_page_list = m;
		netipc_page_list_count++;
		if (netipc_self_stopped) {
			netipc_self_unstop();
		}
		splon(s);
	}
}

/*
 * Grab a vm_page_t at interrupt level. May return VM_PAGE_NULL.
 */
vm_page_t
netipc_page_grab()
{
	vm_page_t m;

	if ((m = netipc_page_list) != VM_PAGE_NULL) {
		netipc_page_list = (vm_page_t) m->pageq.next;
		netipc_page_list_count--;
	} else {
		thread_wakeup((int) &netipc_thread_awake);
	}
	return m;
}

void
netipc_thread_continue()
{
	for (;;) {
		int s;

		netipc_thread_awaken++;
		netipc_replenish(TRUE);
		s = sploff();
		if (netipc_page_list_count < netipc_page_list_high) {
			(void) splon(s);
			continue;
		}
		netipc_thread_awake = FALSE;
		assert_wait((int) &netipc_thread_awake, FALSE);
		(void) splon(s);
		thread_block(netipc_thread_continue);
	}
}

void
netipc_thread()
{
	thread_set_own_priority(0);	/* high priority */
	netipc_thread_continue();
}

netipc_init()
{
	netipc_network_init();
	netipc_init_send();
	netipc_init_recv();
}

#ifdef	netipc_send
netipc_send(remote, vec, count)
	int remote;
	register struct netvec *vec;
	int count;
{
	struct netipc_hdr *hdr = (struct netipc_hdr *) DEVTOKV(vec[0].addr);
	register int seqid = hdr->seqid;
	register int type = hdr->type;
	register int ctl = hdr->ctl;
	register int ctl_seqid = hdr->ctl_seqid;

#if 1
	if (ctl == CTL_NONE && netipc_will_ack[remote]) {
		ctl = hdr->ctl = CTL_ACK;
		ctl_seqid = hdr->ctl_seqid = netipc_will_ack[remote];
		netipc_will_ack[remote] = 0;
	}
#endif
	netipc_print('s', type, remote, ctl, seqid, ctl_seqid);
#undef	netipc_send
	return netipc_send(remote, vec, count);
}
#endif	/*netipc_send*/

netipc_print(c, type, remote, ctl, seqid, ctl_seqid)
	char c;
	int type;
	int remote;
	int ctl;
	int seqid;
	int ctl_seqid;
{
	char *ts;
	char *cs;

	if (Noise == 2) {
		printf("%c", c);
		return;
	}
	if (! Noise) {
		return;
	}
	if (type == NETIPC_TYPE_KMSG) {
		ts = "kmsg";
	} else if (type == NETIPC_TYPE_PAGE) {
		ts = "page";
	} else if (type == NETIPC_TYPE_CTL) {
		ts = "ctrl";
		seqid = 0;
	} else {
		ts = "????";
	}
	if (ctl == CTL_NONE) {
		cs = "none";
		ctl_seqid = 0;
	} else if (ctl == CTL_ACK) {
		cs = "ack ";
	} else if (ctl == CTL_NACK) {
		cs = "nack";
	} else if (ctl == CTL_SYNC) {
		cs = "sync";
	} else if (ctl == CTL_QUENCH) {
		cs = "qnch";
	} else {
		cs = "????";
	}
	printf("%c remote=%d type=%s.%04d ctl=%s.%04d\n",
	       c, remote, ts, seqid, cs, ctl_seqid);
}
