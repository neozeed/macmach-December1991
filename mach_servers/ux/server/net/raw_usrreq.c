/* 
 * Mach Operating System
 * Copyright (c) 1988 Carnegie-Mellon University
 * All rights reserved.  The CMU software License Agreement specifies
 * the terms and conditions for use and redistribution.
 */
 
/*
 * HISTORY
 *  5-Jan-89  David Golub (dbg) at Carnegie-Mellon University
 *	Out-of-kernel version.
 *
 * $Log:	raw_usrreq.c,v $
 * Revision 2.1  89/08/04  14:18:24  rwd
 * Created.
 * 
 * Revision 2.3  88/11/21  16:57:07  rvb
 * 	Small bit of debugging code for MACH_DEVICE (&DLI) to let you see
 * 	the packet that is queued on the raw socket.
 * 	[88/11/09            rvb]
 * 
 * Revision 2.2  88/08/24  01:55:03  mwyoung
 * 	Corrected include file references.
 * 	[88/08/22  22:09:30  mwyoung]
 * 
 *
 */
#include <mach_no_kernel.h>

#include <dli.h>
#include <mach_device.h>
#include <cmucs.h>
/*
 * Copyright (c) 1980, 1986 Regents of the University of California.
 * All rights reserved.  The Berkeley software License Agreement
 * specifies the terms and conditions for redistribution.
 *
 *	@(#)raw_usrreq.c	7.1 (Berkeley) 6/4/86
 */

#include <sys/param.h>
#include <sys/mbuf.h>
#include <sys/domain.h>
#include <sys/protosw.h>
#include <sys/socket.h>
#include <sys/socketvar.h>
#include <sys/errno.h>

#include "if.h"
#include "route.h"
#include "netisr.h"
#include "raw_cb.h"
#if	DLI
#if	MACH_DEVICE
#include <netinet/in.h>
#include <netinet/if_ether.h>
#endif	MACH_DEVICE
#endif	DLI

#if	MACH_NO_KERNEL
#else	MACH_NO_KERNEL
#if	CMUCS && !defined(vax)
#else	CMUCS && !defined(vax)
#include <vax/mtpr.h>
#endif	CMUCS && !defined(vax)
#endif	MACH_NO_KERNEL

/*
 * Initialize raw connection block q.
 */
raw_init()
{

	rawcb.rcb_next = rawcb.rcb_prev = &rawcb;
	rawintrq.ifq_maxlen = IFQ_MAXLEN;
}

/*
 * Raw protocol interface.
 */
raw_input(m0, proto, src, dst)
	struct mbuf *m0;
	struct sockproto *proto;
	struct sockaddr *src, *dst;
{
	register struct mbuf *m;
	struct raw_header *rh;
	int s;

	/*
	 * Rip off an mbuf for a generic header.
	 */
	m = m_get(M_DONTWAIT, MT_HEADER);
	if (m == 0) {
		m_freem(m0);
		return;
	}
	m->m_next = m0;
	m->m_len = sizeof(struct raw_header);
	rh = mtod(m, struct raw_header *);
	rh->raw_dst = *dst;
	rh->raw_src = *src;
	rh->raw_proto = *proto;

	/*
	 * Header now contains enough info to decide
	 * which socket to place packet in (if any).
	 * Queue it up for the raw protocol process
	 * running at software interrupt level.
	 */
	s = splimp();
	if (IF_QFULL(&rawintrq))
		m_freem(m);
	else
		IF_ENQUEUE(&rawintrq, m);
#if	MACH_NO_KERNEL
	/* call schednetisr under splimp! */
	schednetisr(NETISR_RAW);
	splx(s);
#else	MACH_NO_KERNEL
	splx(s);
	schednetisr(NETISR_RAW);
#endif	MACH_NO_KERNEL
}

#if	DLI
#if	MACH_DEVICE
extern char dli_from[6];
extern	int dli_see;

dli_ok(m)
register struct mbuf *m;
{
register struct ether_header *eh = mtod(m->m_next, struct ether_header *);

	return (dli_see && !bcmp(dli_from, eh->ether_shost, 6));
}

pr_m(n)
register struct mbuf *n;
{
register int i;
register unsigned char *cp;

	for (; n; n = n->m_next) {
		cp = mtod(n, unsigned char *);
		for (i = 0; i < n->m_len; i++)
			printf("x%x ", *cp++);
		printf("|| (%d)\n", n->m_len);
	}
}

pr_sk(str, cp)
register unsigned char *cp;
{
register int i;

	printf("%s", str);
	for (i = 0; i < sizeof (struct sockaddr); i++)
		printf("x%x ", *cp++);
	printf("\n");

}
#endif	MACH_DEVICE
#endif	DLI
/*
 * Raw protocol input routine.  Process packets entered
 * into the queue at interrupt time.  Find the socket
 * associated with the packet(s) and move them over.  If
 * nothing exists for this packet, drop it.
 */
rawintr()
{
	int s;
	struct mbuf *m;
	register struct rawcb *rp;
	register struct raw_header *rh;
	struct socket *last;

next:
	s = splimp();
	IF_DEQUEUE(&rawintrq, m);
	splx(s);
	if (m == 0)
		return;
#if	DLI
#if	MACH_DEVICE
	if (dli_ok(m)) pr_m(m);
#endif	MACH_DEVICE
#endif	DLI
	rh = mtod(m, struct raw_header *);
	last = 0;
	for (rp = rawcb.rcb_next; rp != &rawcb; rp = rp->rcb_next) {
		if (rp->rcb_proto.sp_family != rh->raw_proto.sp_family)
			continue;
		if (rp->rcb_proto.sp_protocol  &&
		    rp->rcb_proto.sp_protocol != rh->raw_proto.sp_protocol)
			continue;
		/*
		 * We assume the lower level routines have
		 * placed the address in a canonical format
		 * suitable for a structure comparison.
		 */
#define equal(a1, a2) \
	(bcmp((caddr_t)&(a1), (caddr_t)&(a2), sizeof (struct sockaddr)) == 0)
		if ((rp->rcb_flags & RAW_LADDR) &&
		    !equal(rp->rcb_laddr, rh->raw_dst))
			continue;
		if ((rp->rcb_flags & RAW_FADDR) &&
		    !equal(rp->rcb_faddr, rh->raw_src))
			continue;
		if (last) {
			struct mbuf *n;
			if (n = m_copy(m->m_next, 0, (int)M_COPYALL)) {
				if (sbappendaddr(&last->so_rcv, &rh->raw_src,
				    n, (struct mbuf *)0) == 0)
					/* should notify about lost packet */
					m_freem(n);
				else
					sorwakeup(last);
			}
		}
		last = rp->rcb_socket;
	}
	if (last) {
		if (sbappendaddr(&last->so_rcv, &rh->raw_src,
		    m->m_next, (struct mbuf *)0) == 0)
			m_freem(m->m_next);
		else
			sorwakeup(last);
		(void) m_free(m);		/* header */
	} else
		m_freem(m);
	goto next;
}

/*ARGSUSED*/
raw_ctlinput(cmd, arg)
	int cmd;
	struct sockaddr *arg;
{

	if (cmd < 0 || cmd > PRC_NCMDS)
		return;
	/* INCOMPLETE */
}

/*ARGSUSED*/
raw_usrreq(so, req, m, nam, rights)
	struct socket *so;
	int req;
	struct mbuf *m, *nam, *rights;
{
	register struct rawcb *rp = sotorawcb(so);
	register int error = 0;

	if (req == PRU_CONTROL)
		return (EOPNOTSUPP);
	if (rights && rights->m_len) {
		error = EOPNOTSUPP;
		goto release;
	}
	if (rp == 0 && req != PRU_ATTACH) {
		error = EINVAL;
		goto release;
	}
	switch (req) {

	/*
	 * Allocate a raw control block and fill in the
	 * necessary info to allow packets to be routed to
	 * the appropriate raw interface routine.
	 */
	case PRU_ATTACH:
		if ((so->so_state & SS_PRIV) == 0) {
			error = EACCES;
			break;
		}
		if (rp) {
			error = EINVAL;
			break;
		}
		error = raw_attach(so, (int)nam);
		break;

	/*
	 * Destroy state just before socket deallocation.
	 * Flush data or not depending on the options.
	 */
	case PRU_DETACH:
		if (rp == 0) {
			error = ENOTCONN;
			break;
		}
		raw_detach(rp);
		break;

	/*
	 * If a socket isn't bound to a single address,
	 * the raw input routine will hand it anything
	 * within that protocol family (assuming there's
	 * nothing else around it should go to). 
	 */
	case PRU_CONNECT:
		if (rp->rcb_flags & RAW_FADDR) {
			error = EISCONN;
			break;
		}
		raw_connaddr(rp, nam);
		soisconnected(so);
		break;

	case PRU_CONNECT2:
		error = EOPNOTSUPP;
		goto release;

	case PRU_BIND:
		if (rp->rcb_flags & RAW_LADDR) {
			error = EINVAL;			/* XXX */
			break;
		}
		error = raw_bind(so, nam);
		break;

	case PRU_DISCONNECT:
		if ((rp->rcb_flags & RAW_FADDR) == 0) {
			error = ENOTCONN;
			break;
		}
		raw_disconnect(rp);
		soisdisconnected(so);
		break;

	/*
	 * Mark the connection as being incapable of further input.
	 */
	case PRU_SHUTDOWN:
		socantsendmore(so);
		break;

	/*
	 * Ship a packet out.  The appropriate raw output
	 * routine handles any massaging necessary.
	 */
	case PRU_SEND:
		if (nam) {
			if (rp->rcb_flags & RAW_FADDR) {
				error = EISCONN;
				break;
			}
			raw_connaddr(rp, nam);
		} else if ((rp->rcb_flags & RAW_FADDR) == 0) {
			error = ENOTCONN;
			break;
		}
		error = (*so->so_proto->pr_output)(m, so);
		m = NULL;
		if (nam)
			rp->rcb_flags &= ~RAW_FADDR;
		break;

	case PRU_ABORT:
		raw_disconnect(rp);
		sofree(so);
		soisdisconnected(so);
		break;

	case PRU_SENSE:
		/*
		 * stat: don't bother with a blocksize.
		 */
		return (0);

	/*
	 * Not supported.
	 */
	case PRU_RCVOOB:
	case PRU_RCVD:
		return(EOPNOTSUPP);

	case PRU_LISTEN:
	case PRU_ACCEPT:
	case PRU_SENDOOB:
		error = EOPNOTSUPP;
		break;

	case PRU_SOCKADDR:
		bcopy((caddr_t)&rp->rcb_laddr, mtod(nam, caddr_t),
		    sizeof (struct sockaddr));
		nam->m_len = sizeof (struct sockaddr);
		break;

	case PRU_PEERADDR:
		bcopy((caddr_t)&rp->rcb_faddr, mtod(nam, caddr_t),
		    sizeof (struct sockaddr));
		nam->m_len = sizeof (struct sockaddr);
		break;

	default:
		panic("raw_usrreq");
	}
release:
	if (m != NULL)
		m_freem(m);
	return (error);
}
