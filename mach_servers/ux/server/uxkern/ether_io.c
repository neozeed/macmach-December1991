/* 
 * Mach Operating System
 * Copyright (c) 1987 Carnegie-Mellon University
 * All rights reserved.  The CMU software License Agreement specifies
 * the terms and conditions for use and redistribution.
 */
/*
 * File:	mach_iface.c
 *
 * Abstract:
 *	Getting packets from/to a Mach Network Interface.
 *
 * HISTORY
 * $Log:	ether_io.c,v $
 * Revision 2.7  91/01/08  15:00:56  rpd
 * 	Fixed iface_find to raise the qlimit of the network receive port.
 * 	[91/01/05            rpd]
 * 
 * Revision 2.6  90/09/09  22:32:55  rpd
 * 	Initial support for mapped ether interface.
 * 	[90/08/30  17:48:50  af]
 * 
 * Revision 2.5  90/09/09  14:13:48  rpd
 * 	Deallocate the device port after closing it.
 * 	[90/08/23            rpd]
 * 
 * Revision 2.4  90/06/02  15:27:41  rpd
 * 	Reorganize to have interrupt_enter around mbuf routines since
 * 	they call spl routines.
 * 	[90/05/10            rwd]
 * 	Updated set_thread_priority call for the new priority range.
 * 	[90/04/23            rpd]
 * 	Converted to new IPC.
 * 	[90/03/26  20:15:13  rpd]
 * 
 * Revision 2.3  90/05/29  20:25:09  rwd
 * 	Reorginize to have interrupt_enter around mbuf routines since
 * 	they call spl routines.
 * 	[90/05/10            rwd]
 * 
 * Revision 2.2  89/12/08  20:16:42  rwd
 * 	Call cthread_wire before setting thread priority
 * 	[89/11/01            rwd]
 * 
 * Revision 2.1  89/08/04  14:51:21  rwd
 * Created.
 * 
 * 23-Mar-89  David Golub (dbg) at Carnegie-Mellon University
 *	Changed network input to receive packets asynchronously.
 *
 * 22-Feb-89  David Golub (dbg) at Carnegie-Mellon University
 *	Modified for U*X server use.
 *
 * 29-Sep-88  Alessandro Forin (af) at Carnegie-Mellon University
 *	Created.
 */

#include <map_ether.h>

#include <sys/param.h>
#include <sys/mbuf.h>
#include <sys/socket.h>
#include <sys/errno.h>
#include <sys/ioctl.h>
#include <sys/zalloc.h>
#include <sys/synch.h>

#include <net/if.h>
#include <net/netisr.h>

#include <netinet/in.h>
#include <netinet/in_systm.h>
#include <netinet/in_var.h>
#include <netinet/ip.h>
#include <netinet/if_ether.h>

#ifdef NS
#include <netns/ns.h>
#include <netns/ns_if.h>
#endif

#include <uxkern/device_reply_hdlr.h>
#include <uxkern/device_utils.h>

#include <uxkern/import_mach.h>
#include <device/device_types.h>
#include <device/net_status.h>
#include <sys/user.h>

/*
 * Interface descriptor
 *	NOTE: most of the code outside will believe this to be simply
 *	a "struct ifnet".  That is ok, since it is exactly what ethernets
 *	do in Unix anyway.  The other information is, on the other hand
 *	our own business.
 */
struct ether_softc {
	struct	arpcom es_ac;	/* same as ethernets */
#define es_if	es_ac.ac_if	/* a struct ifnet used outside */
#define es_addr	es_ac.ac_enaddr	/* hardware address, 6 bytes ok for ethers */
#define es_ip	es_ac.ac_ipaddr	/* copy of IP address */
	mach_port_t es_port;	/* where to send requests */
	mach_port_t es_reply_port;	/* where to receive data */
	char	es_name[IFNAMSIZ];
				/* name lives here */
	struct ether_softc *es_link;
				/* link in list of reply ports */
};

struct ether_softc *es_list = 0;
				/* list of all ethernet interfaces,
				   to find by reply port. */

/*
 * Ethernet output routine.
 * Encapsulate a packet of type 'family' for the local net.
 * Never use trailer local net encapsulation.
 */
ether_output(ifp, m0, dst)
	struct ifnet *ifp;
	struct mbuf *m0;
	struct sockaddr *dst;
{
	int type, s, error;
	u_char edst[6];
	struct in_addr idst;
	register struct ether_softc *es = (struct ether_softc *)ifp;
	register struct mbuf *m = m0;
	register struct ether_header *eh;
	int usetrailers;

	if ((ifp->if_flags & IFF_UP) != IFF_UP) {
	    error = ENETDOWN;
	    goto bad;
	}

	switch (dst->sa_family) {
#ifdef INET
	    case AF_INET:
		idst = ((struct sockaddr_in *)dst)->sin_addr;
		if (!arpresolve(&es->es_ac, m, &idst, edst, &usetrailers))
		    return (0);	/* not yet resolved */
		type = ETHERTYPE_IP;
		break;
#endif
#ifdef NS
	    case AF_NS:
		type = ETHERTYPE_NS;
		bcopy((caddr_t)&(((struct sockaddr_ns *)dst)->sns_addr.x_host),
		      (caddr_t)edst,
		      sizeof(edst));
		break;
#endif
#ifdef DLI
	    case AF_DLI:
		if (m->m_len < sizeof(struct ether_header)) {
		    error = EMSGSIZE;
		    goto bad;
		}
		eh = mtod(m, struct ether_header *);
		bcopy(dst->sa_data,
		      (caddr_t)eh->ether_dhost,
		      sizeof(eh->ether_dhost));
		goto gotheader;
#endif
	    case AF_UNSPEC:
		eh = (struct ether_header *)dst->sa_data;
		bcopy((caddr_t)eh->ether_dhost,
		      (caddr_t)edst,
		      sizeof(edst));
		type = eh->ether_type;
		break;

	    default:
		printf("%s%d: can't handle af%d\n", ifp->if_name, ifp->if_unit,
			dst->sa_family);
		error = EAFNOSUPPORT;
		goto bad;
	}

	/*
	 * Add local net header.  If no space in first mbuf,
	 * allocate another.
	 */
	if (m->m_off > MMAXOFF ||
		MMINOFF + sizeof(struct ether_header) > m->m_off) {
	    m = m_get(M_DONTWAIT, MT_HEADER);
	    if (m == 0) {
		error = ENOBUFS;
		goto bad;
	    }
	    m->m_next = m0;
	    m->m_off = MMINOFF;
	    m->m_len = sizeof(struct ether_header);
	}
	else {
	    m->m_off -= sizeof(struct ether_header);
	    m->m_len += sizeof(struct ether_header);
	}
	eh = mtod(m, struct ether_header *);
	eh->ether_type = htons((u_short)type);
	bcopy((caddr_t)edst,
	      (caddr_t)eh->ether_dhost,
	      sizeof(edst));

    gotheader:
	/*
	 * Fill in source address
	 */
	bcopy((caddr_t) es->es_addr,
	      (caddr_t) eh->ether_shost,
	      sizeof(es->es_addr));

	/*
	 * Send message to interface
	 */
	{
	    unsigned int  totlen;
	    io_buf_ptr_t data_addr;
	    register struct mbuf *m1;

	    totlen = 0;
	    for (m1 = m;
		 m1;
		 m1 = m1->m_next)
		totlen += m1->m_len;

	    if (m->m_next == 0) {
		/*
		 * All data in one chunk
		 */
		data_addr = mtod(m, char *);
		(void) device_write_request(es->es_port, MACH_PORT_NULL,
					    0,	/* mode */
					    0,	/* recnum */
					    data_addr,
					    totlen);
	    }
	    else {
		/*
		 * Must allocate contiguous memory and
		 * copy mbuf string into it.
		 */
		(void) vm_allocate(mach_task_self(), (vm_offset_t *)&data_addr,
				   totlen, TRUE);
		(void) m_cpytoc(m, 0, totlen, data_addr);
		(void) device_write_request(es->es_port, MACH_PORT_NULL,
					    0,	/* mode */
					    0,	/* recnum */
					    data_addr,
					    totlen);
		(void) vm_deallocate(mach_task_self(), data_addr, totlen);
	    }
	}
	m_freem(m);	/* sent */
	return (0);

    bad:
	m_freem(m0);
	return (error);
}

int net_input_dispose();	/* forward */
extern struct mbuf *mclgetx();

zone_t	net_msg_zone;

mach_port_t	net_reply_port_set;

void
net_input_thread()
{
	register struct ether_softc *es;
	register struct ifnet *ifp;
	struct mbuf *m;
	int	type, len;
	int	s;
	register struct ifqueue *inq;
	char *	addr;
	int	intr;
	struct ether_header eh;
	register net_rcv_msg_t msg = (net_rcv_msg_t)0;

	cthread_set_name(cthread_self(), "network input thread");

	/*
	 * Wire this cthread to a kernel thread so we can
	 * up its priority
	 */
	cthread_wire();

	/*
	 * Make this thread high priority.
	 */
	set_thread_priority(mach_thread_self(), 3);

	u.u_ipl = -1;

	while (TRUE) {
	    if (u.u_ipl != -1)
		panic("net ipl != -1",u.u_ipl);
	    /*
	     * Allocate a message structure, and receive the next
	     * network message.
	     */
	    msg = (net_rcv_msg_t)zalloc(net_msg_zone);

	    if (mach_msg(&msg->msg_hdr, MACH_RCV_MSG,
			 0, 8192, net_reply_port_set,
			 MACH_MSG_TIMEOUT_NONE, MACH_PORT_NULL)
						!= MACH_MSG_SUCCESS)
		goto error;

	    /*
	     * Find the interface that the message was received on.
	     */
	    for (es = es_list; es; es = es->es_link)
		if (msg->msg_hdr.msgh_local_port == es->es_reply_port)
		    break;
	    if (es == 0)
		goto error;

	    ifp = &es->es_if;
	    if ((ifp->if_flags & IFF_UP) == 0)
		goto error;

	    /*
	     * Save the ethernet header.  Do not include it
	     * in the data passed to upper levels.
	     */
	    eh = *(struct ether_header *)&msg->header[0];

	    /*
	     * Get the packet type, address, and length.
	     */
	    type = ((struct packet_header *)&msg->packet[0])->type;
	    addr = &msg->packet[0] + sizeof(struct packet_header);
	    len  = msg->packet_type.msgt_number
			 - sizeof(struct packet_header);

	    if (len == 0)
		goto error;

	    /*
	     * We need this
	     */

	    interrupt_enter(SPLIMP);

	    /*
	     * Wrap an mbuf around the data, excluding the
	     * local header.
	     */
	    m = mclgetx(net_input_dispose,
			(caddr_t)msg,
			addr,
			len,
			TRUE);
	    /*
	     * Prepend the interface pointer.  We know that
	     * we will get enough room by clobbering the
	     * ethernet header.
	     */
	    m->m_off -= sizeof(struct ifnet *);
	    m->m_len += sizeof(struct ifnet *);
	    *(mtod(m, struct ifnet **)) = ifp;

	    /*
	     * Dispatch to protocol.
	     */
	    type = ntohs((u_short)type);
	    switch (type) {
#ifdef INET
		case ETHERTYPE_IP:
		{
		    inq = &ipintrq;
		    intr = NETISR_IP;
		    break;
		}
		case ETHERTYPE_ARP:
		    arpinput(&es->es_ac, m);
		    interrupt_exit(SPLIMP);
		    continue;
#endif
#ifdef NS
		case ETHERTYPE_NS:
		{
		    inq = &nsintrq;
		    intr = NETISR_NS;
		    break;
		}
#endif
		default:
#ifdef	mac2        /* note unexpected ethernet packets */
		    printf("ignoring ethernet packet with type 0x%X",
		      type);
#endif
#ifdef DLI
		    dli_input(m, type, &eh.ether_shost[0],
			  &es->es_dlv, &eh);
#else
		    m_freem(m);
#endif
		    interrupt_exit(SPLIMP);
		    continue;
	    }

	    if (IF_QFULL(inq)) {
		IF_DROP(inq);
		m_freem(m);
		interrupt_exit(SPLIMP);
		continue;
	    }
	    IF_ENQUEUE(inq, m);
	    schednetisr(intr);
	    interrupt_exit(SPLIMP);

	    continue;

	error:
	    zfree(net_msg_zone, (vm_offset_t)msg);
	    continue;
	}
}

net_input_dispose(msg)
	char *	msg;
{
	zfree(net_msg_zone, (vm_offset_t)msg);
}

/*ARGSUSED*/
kern_return_t
ether_write_reply(ifp_p, return_code, count)
	char *		ifp_p;
	kern_return_t	return_code;
	unsigned int	count;
{
}

/*
 * All interface setup is thru IOCTL.
 */
ether_ioctl(ifp, cmd, data)
	register struct ifnet *ifp;
	int	cmd;
	caddr_t	data;
{
	register struct ifaddr *ifa = (struct ifaddr *)data;
	register struct ether_softc *es = (struct ether_softc *)ifp;
	int s = splimp(), error = 0;

	switch (cmd) {
	    /*
	     * Set interface address (and mark interface up).
	     */
	    case SIOCSIFADDR:
		switch (ifa->ifa_addr.sa_family) {
#ifdef INET
		    case AF_INET:
			es->es_ip = IA_SIN(ifa)->sin_addr;
			arpwhohas(&es->es_ac, &IA_SIN(ifa)->sin_addr);
			break;
#endif
#ifdef NS
		    case AF_NS:
		    {
			register struct ns_addr *ina = &(IA_SNS(ifa)->sns_addr);
			if (ns_nullhost(*ina))
			    ina->x_host = *(union ns_host *)es->es_addr;
			else {
			    bcopy(ina->x_host.c_host,
				  es->es_addr,
				  sizeof(es->es_addr));
			    /*
			     * Tell hardware to change address XXX
			     */
			}
			break;
		    }
#endif
		}
		break;

	    default:
		error = EINVAL;
		break;
	}
	splx(s);
	return (error);
}

ether_error()
{
}

/*
 * This function strategically plugs into ifunit(), and it is called
 * on a non-existant interface.  We try to look it up, and if successful
 * initialize a descriptor and call if_attach() with it.
 *
 * Name points to the full device name (including unit number).
 */
struct ifnet *
iface_find(name)
	char		*name;
{
	register struct ifnet	*ifp;
	register struct ether_softc *es;
	mach_port_t		if_port;
	mach_port_t		reply_port;
	struct net_status	if_stat;
	unsigned int		if_stat_count;
	char			if_addr[16];	/* XXX */
	unsigned int		if_addr_count;
	kern_return_t		rc;
	int			unit;

	rc = device_open(device_server_port,
			 0,		/* mode */
			 name,
			 &if_port);
	if (rc != D_SUCCESS)
	    return (0);

	/*
	 * Get status and address from interface.
	 */
	if_stat_count = NET_STATUS_COUNT;
	rc = device_get_status(if_port,
			NET_STATUS,
			(dev_status_t)&if_stat,
			&if_stat_count);
	if (rc != D_SUCCESS) {
	    (void) device_close(if_port);
	    (void) mach_port_deallocate(mach_task_self(), if_port);
	    return (0);
	}

#if	MAP_ETHER
	if (if_stat.mapped_size) {
		extern	struct ifnet *ether_map();	/* machdep */

		return ether_map(name, if_port, &if_stat);
	}
#endif	MAP_ETHER

	if_addr_count = sizeof(if_addr)/sizeof(int);
	rc = device_get_status(if_port,
			NET_ADDRESS,
			(dev_status_t)if_addr,
			&if_addr_count);
	if (rc != D_SUCCESS) {
	    (void) device_close(if_port);
	    (void) mach_port_deallocate(mach_task_self(), if_port);
	    return (0);
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
	 * Allocate a receive port for the interface.
	 */
	reply_port = mach_reply_port();
	(void) mach_port_move_member(mach_task_self(), reply_port,
				     net_reply_port_set);
	(void) mach_port_set_qlimit(mach_task_self(), reply_port,
				    MACH_PORT_QLIMIT_MAX);

	/*
	 * Allocate an Ethernet interface structure.
	 */
	es = (struct ether_softc *)malloc(sizeof(struct ether_softc));
	bzero((caddr_t) es, sizeof(struct ether_softc));

	ifp = &es->es_if;

	/*
	 * Save interface name in a safe place.  Caller
	 * has ensured that it is shorter than IFNAMSIZ
	 * and ends in a digit.
	 */
	{
	    register char *dst, *src;
	    register char c;

	    dst = es->es_name;
	    for (src = name; ; src++) {
		c = *src;
		if (c >= '0' && c <= '9')
		    break;
		*dst++ = c;
	    }
	    unit = c - '0';
	}
	ifp->if_name =		es->es_name;
	ifp->if_unit =		unit;
	ifp->if_mtu =		if_stat.max_packet_size
					- if_stat.header_size;
	ifp->if_flags =		if_stat.flags;
	ifp->if_output =	ether_output;
	ifp->if_ioctl =		ether_ioctl;

	bcopy(if_addr, es->es_addr, sizeof(es->es_addr));

	es->es_port = if_port;
	es->es_reply_port = reply_port;
#ifdef mac2 /* filter IP and ARP packets only */

/* The following device filter accepts IP and ARP packets only.
 * Note that the type is ((u_short *)packet)[1].
 */
	{
	    filter_t filter[6];

	    filter[0] = NETF_NOP | NETF_PUSHWORD + 1;
	    filter[1] = NETF_COR | NETF_PUSHLIT;
	    filter[2] = htons(ETHERTYPE_IP);
	    filter[3] = NETF_NOP | NETF_PUSHWORD + 1;
	    filter[4] = NETF_EQ | NETF_PUSHLIT;
	    filter[5] = htons(ETHERTYPE_ARP);
	    rc = device_set_filter(if_port,
				   reply_port,
				   MACH_MSG_TYPE_MAKE_SEND,
				   1,		/* priority */
				   filter,
				   sizeof(filter)/sizeof(filter[0]));
	    if (rc != KERN_SUCCESS) {
		printf("device_set_filter: %d\n", rc);
		panic("iface_find: can't set filter");
	    }
	}

#else

	/*
	 * Set up a filter to receive packets.  Take all,
	 * for the moment.
	 */
	{
	    filter_t	filter[2];

	    filter[0] = NETF_PUSHLIT;
	    filter[1] = (filter_t)TRUE;

	    rc = device_set_filter(if_port,
				   reply_port,
				   MACH_MSG_TYPE_MAKE_SEND,
				   1,		/* priority */
				   filter,
				   sizeof(filter)/sizeof(filter[0]));
	    if (rc != KERN_SUCCESS)
		printf("device_set_filter: %d\n", rc);
	}

#endif

	/*
	 * Attach ether_softc structure to reply_port list.
	 */
	es->es_link = es_list;
	es_list = es;

	if_attach(ifp);

	return (struct ifnet *)ifp;
}

void
netisr_init()
{
	kern_return_t	rc;

	net_msg_zone = zinit(8192,
			1000*8192,
			10*8192,
			TRUE,
			"incoming network messages");

	rc = mach_port_allocate(mach_task_self(), MACH_PORT_RIGHT_PORT_SET,
				&net_reply_port_set);
	if (rc != KERN_SUCCESS)
	    panic("Allocating net reply port set returns %d", rc);

	ux_create_thread(net_input_thread);

}
