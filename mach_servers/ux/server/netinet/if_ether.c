/* 
 * Mach Operating System
 * Copyright (c) 1990 Carnegie-Mellon University
 * Copyright (c) 1988 Carnegie-Mellon University
 * All rights reserved.  The CMU software License Agreement specifies
 * the terms and conditions for use and redistribution.
 */
/* 
 * HISTORY
 * $Log:	if_ether.c,v $
 * Revision 2.2  90/06/19  23:12:36  rpd
 * 	Picked up arpioctl/arptnew fix from Paul Parker.
 * 	Purged CMUCS conditionals.
 * 	[90/06/10            rpd]
 * 
 * Revision 2.1  89/08/04  14:21:20  rwd
 * Created.
 * 
 * Revision 2.4  88/09/27  17:30:28  rvb
 * 	Fixed Log
 * 
 * Revision 2.3  88/09/27  16:06:42  rvb
 * 	make arptab and arptab_bsiz and arptab_nb dynamic
 * 	[88/09/23            rvb]
 * 
 * Revision 2.2  88/08/24  01:57:40  mwyoung
 * 	Corrected include file references.
 * 	[88/08/22  18:50:30  mwyoung]
 * 
 *
 * 17-Sep-87  Richard Sanzi (sanzi) at Carnegie-Mellon University
 *	Added ITC mods from Drew Perkins for the 4.3ACIS lan driver,
 *	under switch romp.
 *
 *  1-Jul-87  Daniel Julin (dpj) at Carnegie-Mellon University
 *	Updated from new VMTP sources from Stanford (June 87).
 *
 * 16-May-87  Jay Kistler (jjk) at Carnegie-Mellon University
 *	Merged in IP multicast code.
 *
 * 27-Jan-87  Mike Accetta (mja) at Carnegie-Mellon University
 *	Fixed bug in ioctl() processing which failed to pass new
 *	interface parameter to arptnew() as reported by Bill Bolosky.
 *	[ V5.1(F1) ]
 *
 * 21-Aug-86  Mike Accetta (mja) at Carnegie-Mellon University
 *	Moved return in broadcast check within arpresolve() out of CS_INET
 *	conditional so that it gets executed in both cases.
 *
 * 11-Aug-86  David Golub (dbg) at Carnegie-Mellon University
 *	Fixed to compile without 'vax' or 3Mb ethernet.
 *
 *  1-Aug-86  Bill Bolosky (bolosky) at Carnegie-Mellon University
 *	CS_BUGFIX: Added workaround for romp compiler/processor stupidity 
 *	in in_arpinput.
 *
 * 25-Jan-86  Avadis Tevanian (avie) at Carnegie-Mellon University
 *	Upgraded to 4.3.
 *
 * 13-Aug-85  Mike Accetta (mja) at Carnegie-Mellon University
 *	CS_INET:  Parameterized to support multiple ethernet types and
 *	allow multiple heterogenous interfaces to co-exist.
 *	NEN:  Added support for 3Mb ethernet.
 *	[V1(1)]
 *
 * 27-Jun-85  Mike Accetta (mja) at Carnegie-Mellon University
 *	CS_INET: disabled silly ARP bypass by local host address check
 *	by setting local host upper limit above the maximum legal local
 *	host.
 *	[V1(1)].
 *
 */
/*
 * Copyright (c) 1982, 1986 Regents of the University of California.
 * All rights reserved.  The Berkeley software License Agreement
 * specifies the terms and conditions for redistribution.
 *
 *	@(#)if_ether.c	7.1 (Berkeley) 6/5/86
 */

/*
 * Ethernet address resolution protocol.
 */

#include <multicast.h>

#ifdef	vax
#include <en.h>
#else	vax
#define	NEN 0
#endif	vax

#include <sys/param.h>
#include <sys/systm.h>
#include <sys/mbuf.h>
#include <sys/socket.h>
#include <sys/time.h>
#include <sys/kernel.h>
#include <sys/errno.h>
#include <sys/ioctl.h>
#include <sys/syslog.h>

#include <net/if.h>
#include "in.h"
#include "in_systm.h"
#include "ip.h"
#include "if_ether.h"
#if	NEN > 0
#include <vaxif/if_en.h>
#endif	NEN

#define	ARPTAB_BSIZ	9		/* bucket size */
#define	ARPTAB_NB	19		/* number of buckets */
#define	ARPTAB_SIZE	(ARPTAB_BSIZ * ARPTAB_NB)
struct arptab *arptab;
int	arptab_size = ARPTAB_SIZE;	/* for arp command */
int arptab_bsiz = ARPTAB_BSIZ;
int arptab_nb = ARPTAB_NB;
int arptab_displaced = 0;

/*
 * ARP trailer negotiation.  Trailer protocol is not IP specific,
 * but ARP request/response use IP addresses.
 */
#define ETHERTYPE_IPTRAILERS ETHERTYPE_TRAIL
#if	NEN > 0
#define ENTYPE_IPTRAILERS ENTYPE_TRAIL
#endif	NEN > 0

#define	ARPTAB_HASH(a) \
	((u_long)(a) % arptab_nb)

/*
 *  Change to permit multiple heterogenous interfaces to co-exist.
 */
#define	ARPTAB_LOOK(at,addr,ifp) { \
	register n; \
	at = &arptab[ARPTAB_HASH(addr) * arptab_bsiz]; \
	for (n = 0 ; n < arptab_bsiz ; n++,at++) \
		if (at->at_if == (ifp) && at->at_iaddr.s_addr == addr) \
			break; \
	if (n >= arptab_bsiz) \
		at = 0; \
}

/* timer values */
#define	ARPT_AGE	(60*1)	/* aging timer, 1 min. */
#define	ARPT_KILLC	20	/* kill completed entry in 20 mins. */
#define	ARPT_KILLI	3	/* kill incomplete entry in 3 minutes */

#define arpisen(ac)	\
	(*((short *)(ac)->ac_if.if_name) == ((('n'<<8))+'e'))

struct ether_arp arpethertempl =
{
  
    ARPHRD_ETHER,  ETHERTYPE_IP, 6, 4, ARPOP_REQUEST,
    0x0,  0x0,  0x0,  0x0,  0x0 , 0x0 ,
    0x0,  0x0,  0x0 , 0x0,
    0xff, 0xff, 0xff, 0xff, 0xff, 0xff,
    0x0,  0x0,  0x0 , 0x0,
};
#if	NEN > 0
struct ether_arp arpentempl =
{
    ARPHRD_XETHER, ENTYPE_IP,       1, 4, ARPOP_REQUEST,
    0x0,
    0x0, 0x0, 0x0, 0x0,
    0x0,
    0x0, 0x0, 0x0, 0x0,
};
#endif	NEN > 0
u_char	etherbroadcastaddr[6] = { 0xff, 0xff, 0xff, 0xff, 0xff, 0xff };
extern struct ifnet loif;

/*
 * Timeout routine.  Age arp_tab entries once a minute.
 */
arptimer()
{
	register struct arptab *at;
	register i;

	timeout(arptimer, (caddr_t)0, ARPT_AGE * hz);
	at = &arptab[0];
	for (i = 0; i < arptab_size; i++, at++)
	{
		if (at->at_flags == 0 || (at->at_flags & ATF_PERM))
			continue;
		if (++at->at_timer < ((at->at_flags&ATF_COM) ?
		    ARPT_KILLC : ARPT_KILLI))
			continue;
		/* timer has expired, clear entry */
		arptfree(at);
	}
}

/*
 * Broadcast an ARP packet, asking who has addr on interface ac.
 */
arpwhohas(ac, addr)
	register struct arpcom *ac;
	struct in_addr *addr;
{
	register struct mbuf *m;
	register struct ether_arp *eat;
	register struct ether_arp *ea;
	struct sockaddr sa;

	if ((m = m_get(M_DONTWAIT, MT_DATA)) == NULL)
		return;
	m->m_len = sizeof *ea;
#if	NEN >  0
	if (arpisen(ac))
	{
	    /*
	     *  3Mb ethernet.
	     *
	     *  The ARP packet is smaller by twice the difference in the
	     *  hardware lengths of the 3Mb ethernet from the 10Mb ethernet
	     *  (from the source and target hardware address fields).
	     */
	    eat = &arpentempl;
	    sa.sa_family = ENTYPE_ARP;	/* if_output will swap */
	    m->m_len = sizeof *ea - (2*(6-1));
        }
	else
#endif	NEN >  0
	{
	    /*
	     *  10Mb ethernet.
	     */
	    eat = &arpethertempl;
	    sa.sa_family = ETHERTYPE_ARP;	/* if_output will swap */
	    m->m_len = sizeof *ea;
	}
	/*
	 *  Set packet type and hardware destination in socket address.  For
	 *  both 3Mb and 10Mb ethernet, the type is a short word preceded by a
	 *  destination and source hardware address.  The destination address
	 *  is actually not the first byte of the 3Mb packet header but the
	 *  socket address is interpreted by the output routine and not passed
	 *  directly to the hardware so this doesn't matter.
	 */
	*((u_short *)&sa.sa_data[2*eat->arp_hln]) = sa.sa_family;
	bcopy((caddr_t)&eat->arp_sha[eat->arp_hln+eat->arp_pln],
	      (caddr_t)sa.sa_data, eat->arp_hln);
	/* reate pointer to beginning of ARP packet. */
 	m->m_off = MMAXOFF - m->m_len;
 	ea = mtod(m, struct ether_arp *);
	/* copy in the template packet. */
	bcopy((caddr_t)eat, (caddr_t)ea, (unsigned)m->m_len);
#ifdef	romp	
	if ((!bcmp((caddr_t)ac->ac_if.if_name, (caddr_t)"lan", 3)) &&
	    (ac->ac_if.if_flags & IFF_SNAP))
		ea->arp_hrd = htons(ARPHRD_802);
#endif	romp
	/* copy the source hardware address (from the if) */
	bcopy((caddr_t)&ac->ac_enaddr[0],
	      (caddr_t)&ea->arp_sha[0],
	      eat->arp_hln);
	/* copy the source protocol address (from the if) */
	bcopy((caddr_t)&(ac->ac_ipaddr),
	      (caddr_t)&ea->arp_sha[eat->arp_hln],
	      eat->arp_pln);
	/* keep the target hardware address (from the template) */
	/* copy the target protocol address (parameter) */
	bcopy((caddr_t)addr,
	      (caddr_t)&ea->arp_sha[2*(eat->arp_hln)+eat->arp_pln],
	      eat->arp_pln);
	/* byte swap the appopriate quantities depending on the ethernet type */
	if (eat == &arpethertempl)
	/*
	 *  10Mb ethernet - byte oriented
	 *
	 *  Swap all word quantities.
	 */
	{
	     ea->arp_hrd = htons(ea->arp_hrd);
	     ea->arp_pro = htons(ea->arp_pro);
	     ea->arp_op  = htons(ea->arp_op);
	}
        else
	/*
	 *  3Mb ethernet - word oriented
	 *
	 *  Swap all byte quantities.
	 */
	{
	    register int i;
	
	    *((u_short *)&ea->arp_hln) = htons(*((u_short *)&ea->arp_hln));
	    for (i=2*(eat->arp_pln+eat->arp_hln); i; i-=2)
	        *((u_short *)&ea->arp_sha[i-2]) = htons(*((u_short *)&ea->arp_sha[i-2]));
	}
	sa.sa_family = AF_UNSPEC;
	(*ac->ac_if.if_output)(&ac->ac_if, m, &sa);
}

/*
 * Resolve an IP address into an ethernet address.  If success, 
 * desten is filled in.  If there is no entry in arptab,
 * set one up and broadcast a request for the IP address.
 * Hold onto this mbuf and resend it once the address
 * is finally resolved.  A return value of 1 indicates
 * that desten has been filled in and the packet should be sent
 * normally; a 0 return indicates that the packet has been
 * taken over here, either now or for later transmission.
 *
 * We do some (conservative) locking here at splimp, since
 * arptab is also altered from input interrupt service (ecintr/ilintr
 * calls arpinput when ETHERTYPE_ARP packets come in).
 */
arpresolve(ac, m, destip, desten, usetrailers)
	register struct arpcom *ac;
	struct mbuf *m;
	register struct in_addr *destip;
	register u_char *desten;
	int *usetrailers;
{
	register struct arptab *at;
	register struct ifnet *ifp;
	struct sockaddr_in sin;
	int s, lna;

	*usetrailers = 0;

#if	MULTICAST
	/* If destination IP address is class D (i.e. multicast), stick */
	/* its low-order 24 bits into the low-order half of an ethernet */
	/* multicast address.                                           */

	if (IN_CLASSD(ntohl(destip->s_addr))) {
		register u_long ipdest = ntohl(destip->s_addr);

		desten[0] = ETHER_IP_MULTICAST0;
		desten[1] = ETHER_IP_MULTICAST1;
		desten[2] = ETHER_IP_MULTICAST2;
		desten[3] = ipdest >> 16;
		desten[4] = ipdest >> 8;
		desten[5] = ipdest;

		return (1);
	}
	else
#endif	MULTICAST
	if (in_broadcast(*destip)) {	/* broadcast address */
		register struct ether_arp *eat;

#if	NEN > 0
		if (arpisen(ac))
			eat = &arpentempl;
		else
#endif	NEN > 0
			eat = &arpethertempl;
		bcopy((caddr_t)&eat->arp_sha[eat->arp_hln+eat->arp_pln],
		      (caddr_t)desten,
		      eat->arp_hln);
		return (1);
	}
	lna = in_lnaof(*destip);
	ifp = &ac->ac_if;
	/* if for us, use software loopback driver if up */
	if (destip->s_addr == ac->ac_ipaddr.s_addr) {
		if (loif.if_flags & IFF_UP) {
			sin.sin_family = AF_INET;
			sin.sin_addr = *destip;
			(void) looutput(&loif, m, (struct sockaddr *)&sin);
			/*
			 * The packet has already been sent and freed.
			 */
			return (0);
		} else {
			bcopy((caddr_t)ac->ac_enaddr, (caddr_t)desten,
			    sizeof(ac->ac_enaddr));
			return (1);
		}
	}
	s = splimp();
	ARPTAB_LOOK(at, destip->s_addr, ifp);
	if (at == 0) {			/* not found */
		if (ifp->if_flags & IFF_NOARP) {
			bcopy((caddr_t)ac->ac_enaddr, (caddr_t)desten, 3);
			desten[3] = (lna >> 16) & 0x7f;
			desten[4] = (lna >> 8) & 0xff;
			desten[5] = lna & 0xff;
			splx(s);
			return (1);
		} else {
			at = arptnew(ifp, destip);
			at->at_hold = m;
			arpwhohas(ac, destip);
			splx(s);
			return (0);
		}
	}
	at->at_timer = 0;		/* restart the timer */
	if (at->at_flags & ATF_COM) {	/* entry IS complete */
		bcopy((caddr_t)at->at_enaddr, (caddr_t)desten,
		    sizeof(at->at_enaddr));
		if (at->at_flags & ATF_USETRAILERS)
			*usetrailers = 1;
		splx(s);
		return (1);
	}
	/*
	 * There is an arptab entry, but no ethernet address
	 * response yet.  Replace the held mbuf with this
	 * latest one.
	 */
	if (at->at_hold)
		m_freem(at->at_hold);
	at->at_hold = m;
	arpwhohas(ac, destip);		/* ask again */
	splx(s);
	return (0);
}

#ifdef	romp

/*
 * Resolve an IP address into a token ring address. 
 * First do normal arpresolve.  On return, complete
 * routing information if present in arp table.
 *
 * Do same (conservative) locking here at splimp.
 */
tr_arpresolve(ac, m, destip, desten, usetrailers, rcf, rseg)
	register struct arpcom *ac;
	struct mbuf *m;
	register struct in_addr *destip;
	register u_char *desten;
	int *usetrailers;
	u_short *rcf;
	u_short rseg[LAN_MAX_BRIDGE];
{
	register struct arptab *at;
	int arp_return, s;

	if (arp_return = arpresolve(ac,m,destip,desten,usetrailers)) {
		s = splimp();
		ARPTAB_LOOK(at, destip->s_addr,&(ac->ac_if));
		if (at) {
			if (at->at_flags & ATF_COM) {
				if (*rcf = at->at_rcf) {
					bcopy((caddr_t)at->at_rseg,
						(caddr_t)rseg,
						((*rcf & LAN_RCF_LEN_MASK) >> 8)
						- sizeof(*rcf));
				}
				else *rcf = 0;
			}
		}
		splx(s);
	}
	return(arp_return);
}

/*
 * Called from token ring interrupt handlers
 * when ether packet type ETHERTYPE_ARP
 * is received.  Common length and type checks are done here,
 * then the protocol-specific routine is called.
 */
tr_arpinput(ac, m, mac)
	struct arpcom *ac;
	struct mbuf *m;
	register struct mac_hdr *mac;
{
	register struct arphdr *ar;

	if (ac->ac_if.if_flags & IFF_NOARP)
		goto out;
	IF_ADJ(m);
	if (m->m_len < sizeof(struct arphdr))
		goto out;
	ar = mtod(m, struct arphdr *);
	if (ntohs(ar->ar_hrd) != ARPHRD_802)
		goto out;
	if (m->m_len < sizeof(struct arphdr) + 2 * ar->ar_hln + 2 * ar->ar_pln)
		goto out;

	switch (ntohs(ar->ar_pro)) {

	case ETHERTYPE_IP:
	case ETHERTYPE_IPTRAILERS:
		tr_in_arpinput(ac, m, mac);
		return;

	default:
		break;
	}
out:
	m_freem(m);
}

/*
 * ARP for Internet protocols on token ring.
 * Duplicates ethernet arp for the most part,
 * with modifications to handle routing information.
 * Algorithm is that given in RFC 826.
 * In addition, a sanity check is performed on the sender
 * protocol address, to catch impersonators.
 * We also handle negotiations for use of trailer protocol:
 * ARP replies for protocol type ETHERTYPE_TRAIL are sent
 * along with IP replies if we want trailers sent to us,
 * and also send them in response to IP replies.
 * This allows either end to announce the desire to receive
 * trailer packets.
 * We reply to requests for ETHERTYPE_TRAIL protocol as well,
 * but don't normally send requests.
 */
tr_in_arpinput(ac, m, mac)
	register struct arpcom *ac;
	struct mbuf *m;
	register struct mac_hdr *mac;
{
	register struct ether_arp *ea;
	struct ether_header *eh;
	register struct arptab *at;  /* same as "merge" flag */
	struct mbuf *mcopy = 0;
	struct sockaddr_in sin;
	struct sockaddr sa;
	struct in_addr isaddr, itaddr, myaddr;
	int proto, op;
	u_short rcf;
	int ri_len;

	myaddr = ac->ac_ipaddr;
	ea = mtod(m, struct ether_arp *);
	proto = ntohs(ea->arp_pro);
	op = ntohs(ea->arp_op);
	bcopy((caddr_t)(struct in_addr *)ea->arp_spa,
		(caddr_t)&isaddr.s_addr, sizeof (ea->arp_spa));
	bcopy((caddr_t)(struct in_addr *)ea->arp_tpa,
		(caddr_t)&itaddr.s_addr, sizeof (ea->arp_tpa));
	if (!bcmp((caddr_t)ea->arp_sha, (caddr_t)ac->ac_enaddr,
	  sizeof (ea->arp_sha)))
		goto out;	/* it's from me, ignore it. */
	if (!bcmp((caddr_t)ea->arp_sha, (caddr_t)etherbroadcastaddr,
	    sizeof (ea->arp_sha))) {
		log(LOG_ERR,
		    "arp: ether address is broadcast for IP address %x!\n",
		    ntohl(isaddr.s_addr));
		goto out;
	}
	if (isaddr.s_addr == myaddr.s_addr) {
		log(LOG_ERR, "%s: %s\n",
			"duplicate IP address!! sent from ethernet address",
			ether_sprintf(ea->arp_sha));
		itaddr = myaddr;
		if (op == ARPOP_REQUEST)
			goto reply;
		goto out;
	}
	/* check for routing info */
	if (mac->from_addr[0] & LAN_RI_PRESENT) {
		ri_len = ((mac->rcf & LAN_RCF_LEN_MASK) >> 8)
			- sizeof(mac->rcf);
	}
	else ri_len = 0;
	if (ri_len) {
		rcf = mac->rcf;
		if (rcf & LAN_RCF_DIRECTION) {
			rcf &= (~LAN_RCF_DIRECTION);
		}
		else {
			rcf |= LAN_RCF_DIRECTION;
		}
		rcf &= (~LAN_RCF_BROADCAST);
	}
	else rcf = 0;
	ARPTAB_LOOK(at, isaddr.s_addr,&ac->ac_if);
	if (at) {
		bcopy((caddr_t)ea->arp_sha, (caddr_t)at->at_enaddr,
		    sizeof(ea->arp_sha));
		at->at_flags |= ATF_COM;
		at->at_rcf = rcf;
		bcopy((caddr_t)mac->rseg, (caddr_t)at->at_rseg,
			ri_len);
		if (at->at_hold) {
			sin.sin_family = AF_INET;
			sin.sin_addr = isaddr;
			(*ac->ac_if.if_output)(&ac->ac_if, 
			    at->at_hold, (struct sockaddr *)&sin);
			at->at_hold = 0;
		}
	}
	if (at == 0 && itaddr.s_addr == myaddr.s_addr) {
		/* ensure we have a table entry */
		at = arptnew(&isaddr);
		bcopy((caddr_t)ea->arp_sha, (caddr_t)at->at_enaddr,
		    sizeof(ea->arp_sha));
		at->at_flags |= ATF_COM;
		at->at_rcf = rcf;
		bcopy((caddr_t)mac->rseg, (caddr_t)at->at_rseg,
			ri_len);
	}
reply:
	switch (proto) {

	case ETHERTYPE_IPTRAILERS:
		/* partner says trailers are OK */
		if (at)
			at->at_flags |= ATF_USETRAILERS;
		/*
		 * Reply to request iff we want trailers.
		 */
		if (op != ARPOP_REQUEST || ac->ac_if.if_flags & IFF_NOTRAILERS)
			goto out;
		break;

	case ETHERTYPE_IP:
		/*
		 * Reply if this is an IP request, or if we want to send
		 * a trailer response.
		 */
		if (op != ARPOP_REQUEST && ac->ac_if.if_flags & IFF_NOTRAILERS)
			goto out;
	}
	if (itaddr.s_addr == myaddr.s_addr) {
		/* I am the target */
		bcopy((caddr_t)ea->arp_sha, (caddr_t)ea->arp_tha,
		    sizeof(ea->arp_sha));
		bcopy((caddr_t)ac->ac_enaddr, (caddr_t)ea->arp_sha,
		    sizeof(ea->arp_sha));
	} else {
		ARPTAB_LOOK(at, itaddr.s_addr, &ac->ac_if);
		if (at == NULL || (at->at_flags & ATF_PUBL) == 0)
			goto out;
		bcopy((caddr_t)ea->arp_sha, (caddr_t)ea->arp_tha,
		    sizeof(ea->arp_sha));
		bcopy((caddr_t)at->at_enaddr, (caddr_t)ea->arp_sha,
		    sizeof(ea->arp_sha));
	}

	bcopy((caddr_t)ea->arp_spa, (caddr_t)ea->arp_tpa,
	    sizeof(ea->arp_spa));
	bcopy((caddr_t)&itaddr, (caddr_t)ea->arp_spa,
	    sizeof(ea->arp_spa));
	ea->arp_op = htons(ARPOP_REPLY); 
	/*
	 * If incoming packet was an IP reply,
	 * we are sending a reply for type IPTRAILERS.
	 * If we are sending a reply for type IP
	 * and we want to receive trailers,
	 * send a trailer reply as well.
	 */
	if (op == ARPOP_REPLY)
		ea->arp_pro = htons(ETHERTYPE_IPTRAILERS);
	else if (proto == ETHERTYPE_IP &&
	    (ac->ac_if.if_flags & IFF_NOTRAILERS) == 0)
		mcopy = m_copy(m, 0, (int)M_COPYALL);
	eh = (struct ether_header *)sa.sa_data;
	bcopy((caddr_t)ea->arp_tha, (caddr_t)eh->ether_dhost,
	    sizeof(eh->ether_dhost));
	eh->ether_type = ETHERTYPE_ARP;
	sa.sa_family = AF_UNSPEC;
	(*ac->ac_if.if_output)(&ac->ac_if, m, &sa);
	if (mcopy) {
		ea = mtod(mcopy, struct ether_arp *);
		ea->arp_pro = htons(ETHERTYPE_IPTRAILERS);
		(*ac->ac_if.if_output)(&ac->ac_if, mcopy, &sa);
	}
	return;
out:
	m_freem(m);
	return;
}

#endif	romp

/*
 * Called from 10 Mb/s Ethernet interrupt handlers
 * when ether packet type ETHERTYPE_ARP
 * is received.  Common length and type checks are done here,
 * then the protocol-specific routine is called.
 */
arpinput(ac, m)
	struct arpcom *ac;
	struct mbuf *m;
{
	register struct arphdr *ar;
	register struct ether_arp *eat;
	short hrd;
	short pro;

	if (ac->ac_if.if_flags & IFF_NOARP)
		goto out;
	IF_ADJ(m);
	if (m->m_len < sizeof(struct arphdr))
		goto out;
	ar = mtod(m, struct arphdr *);
#if	NEN > 0
	if (arpisen(ac))
	{
		eat = &arpentempl;
		hrd = ntohs(ARPHRD_XETHER);
		switch (ar->ar_pro)
		{
		    case ENTYPE_IP:
			pro = ETHERTYPE_IP;
			break;
		    case ENTYPE_IPTRAILERS:
			pro = ETHERTYPE_IPTRAILERS;
			break;
		    default:
			pro = 0;
			break;
		}
	}
	else
#endif	NEN > 0
	{
		eat = &arpethertempl;
		hrd = ARPHRD_ETHER;
		pro = ntohs(ar->ar_pro);
	}
	if (ntohs(ar->ar_hrd) != hrd)
		goto out;
	if (m->m_len < (sizeof (struct arphdr) +
			2*(eat->arp_hln+eat->arp_pln)))
		goto out;

	switch (pro) {

	case ETHERTYPE_IP:
	case ETHERTYPE_IPTRAILERS:
		in_arpinput(ac, m);
		return;

	default:
		break;
	}
out:
	m_freem(m);
}

/*
 * ARP for Internet protocols on 10 Mb/s Ethernet.
 * Algorithm is that given in RFC 826.
 * In addition, a sanity check is performed on the sender
 * protocol address, to catch impersonators.
 * We also handle negotiations for use of trailer protocol:
 * ARP replies for protocol type ETHERTYPE_TRAIL are sent
 * along with IP replies if we want trailers sent to us,
 * and also send them in response to IP replies.
 * This allows either end to announce the desire to receive
 * trailer packets.
 * We reply to requests for ETHERTYPE_TRAIL protocol as well,
 * but don't normally send requests.
 */
in_arpinput(ac, m)
	register struct arpcom *ac;
	struct mbuf *m;
{
	register struct ether_arp *ea;
	register struct ether_arp *eat;
	register struct arptab *at;  /* same as "merge" flag */
	struct mbuf *mcopy = 0;
	struct sockaddr_in sin;
	struct sockaddr sa;
	struct in_addr isaddr, itaddr, myaddr;
	int proto, op;

	myaddr = ac->ac_ipaddr;
	ea = mtod(m, struct ether_arp *);
#if	NEN > 0
	if (arpisen(ac))
	{
		register int i;

		eat = &arpentempl;
		sa.sa_family = ENTYPE_ARP;
		*((u_short *)&ea->arp_hln) = htons(*((u_short *)&ea->arp_hln));
		for (i=2*(eat->arp_hln+eat->arp_pln); i; i-=2)
			*((u_short *)(&ea->arp_sha[i-2])) = ntohs(*((u_short *)(&ea->arp_sha[i-2])));
		proto = (ea->arp_pro == ENTYPE_IP)?ETHERTYPE_IP:0;
		op    = ea->arp_op;
	}
	else
#endif	NEN > 0
	{
		eat = &arpethertempl;
		sa.sa_family = ETHERTYPE_ARP;
		proto = ntohs(ea->arp_pro);
		op    = ntohs(ea->arp_op);
	}
	if (ea->arp_hln != eat->arp_hln ||
	    ea->arp_pln != eat->arp_pln)
		goto out;
/*
 * These bcopys are really necessary on the RT because the brain-damaged
 * compiler generates load instructions for fetching the ip addresses, when
 * they aren't aligned on a full-word boundary.  The processor then blithely
 * proceeds to force alignment (without notifying anyone, of course).  So,
 * I recommend not removing these stupid bcopies.
 */
	bcopy(&ea->arp_sha[eat->arp_hln],
		(caddr_t)&(isaddr.s_addr),sizeof(ea->arp_spa));
	bcopy(&ea->arp_sha[eat->arp_hln*2 + eat->arp_pln],
		(caddr_t)&(itaddr.s_addr),sizeof(ea->arp_tpa));
	if (!bcmp((caddr_t)ea->arp_sha, (caddr_t)ac->ac_enaddr,
	  eat->arp_hln))
		goto out;	/* it's from me, ignore it. */
	if (!bcmp((caddr_t)ea->arp_sha, (caddr_t)etherbroadcastaddr,
	    sizeof (ea->arp_sha))) {
		log(LOG_ERR,
		    "arp: ether address is broadcast for IP address %x!\n",
		    ntohl(isaddr.s_addr));
		goto out;
	}
	if (isaddr.s_addr == myaddr.s_addr) {
		log(LOG_ERR, "%s: %s\n",
			"duplicate IP address!! sent from ethernet address",
			ether_sprintf(ea->arp_sha));
#if	0	/* DPJ DEBUG */
		printf("ac = 0x%x m = 0x%x\n",ac,m);
		vmtp_dumpmbuf(m,"m","in_arpinput");
#endif	0	/* DPJ DEBUG */
		itaddr = myaddr;
		if (op == ARPOP_REQUEST)
			goto reply;
		goto out;
	}
	ARPTAB_LOOK(at, isaddr.s_addr, &ac->ac_if);
	if (at) {
		bcopy((caddr_t)ea->arp_sha, (caddr_t)at->at_enaddr,
		   eat->arp_hln);
		if (eat->arp_hln < sizeof at->at_enaddr)
			bzero((caddr_t)&at->at_enaddr[eat->arp_hln],
			      sizeof(at->at_enaddr)-eat->arp_hln);
		at->at_flags |= ATF_COM;
		if (at->at_hold) {
			sin.sin_family = AF_INET;
			sin.sin_addr = isaddr;
			(*ac->ac_if.if_output)(&ac->ac_if, 
			    at->at_hold, (struct sockaddr *)&sin);
			at->at_hold = 0;
		}
	}
	if (at == 0 && itaddr.s_addr == myaddr.s_addr) {
		/* ensure we have a table entry */
		at = arptnew(&ac->ac_if, &isaddr);
		bcopy((caddr_t)ea->arp_sha, (caddr_t)at->at_enaddr,
		   eat->arp_hln);
		if (eat->arp_hln < sizeof at->at_enaddr)
			bzero((caddr_t)&at->at_enaddr[eat->arp_hln],
			      sizeof(at->at_enaddr)-eat->arp_hln);
		at->at_flags |= ATF_COM;
	}
reply:
	switch (proto) {

	case ETHERTYPE_IPTRAILERS:
		/* partner says trailers are OK */
		if (at)
			at->at_flags |= ATF_USETRAILERS;
		/*
		 * Reply to request iff we want trailers.
		 */
		if (op != ARPOP_REQUEST || ac->ac_if.if_flags & IFF_NOTRAILERS)
			goto out;
		break;

	case ETHERTYPE_IP:
		/*
		 * Reply if this is an IP request, or if we want to send
		 * a trailer response.
		 */
		if (op != ARPOP_REQUEST && ac->ac_if.if_flags & IFF_NOTRAILERS)
			goto out;
	}
	if (itaddr.s_addr == myaddr.s_addr) {
		/* I am the target */
		bcopy((caddr_t)&ea->arp_sha[0],
		      (caddr_t)&ea->arp_sha[eat->arp_hln+eat->arp_pln],
		      eat->arp_hln);
		bcopy((caddr_t)&ac->ac_enaddr[0],
		      (caddr_t)&ea->arp_sha[0],
		      eat->arp_hln);
	} else {
		ARPTAB_LOOK(at, itaddr.s_addr, &ac->ac_if);
		if (at == NULL || (at->at_flags & ATF_PUBL) == 0)
			goto out;
		bcopy((caddr_t)&ea->arp_sha[0],
		      (caddr_t)&ea->arp_sha[eat->arp_hln+eat->arp_pln],
		      eat->arp_hln);
		bcopy((caddr_t)&ac->ac_enaddr[0],
		      (caddr_t)&ea->arp_sha[0],
		      eat->arp_hln);
	}
	bcopy((caddr_t)&ea->arp_sha[eat->arp_hln],
	      (caddr_t)&ea->arp_sha[eat->arp_hln*2+eat->arp_pln],
	      eat->arp_pln);
	bcopy((caddr_t)&itaddr,
	      (caddr_t)&ea->arp_sha[eat->arp_hln],
	      eat->arp_pln);
	ea->arp_op = ARPOP_REPLY;
	bcopy((caddr_t)&ea->arp_sha[eat->arp_hln+eat->arp_pln],
	      (caddr_t)sa.sa_data,
	      eat->arp_hln);
	*((u_short *)&sa.sa_data[2*eat->arp_hln]) = sa.sa_family;
	/*
	 * If incoming packet was an IP reply,
	 * we are sending a reply for type IPTRAILERS.
	 * If we are sending a reply for type IP
	 * and we want to receive trailers,
	 * send a trailer reply as well.
	 */
	if (op == ARPOP_REPLY)
#if	NEN > 0
		ea->arp_pro = (eat == &arpethertempl)?htons(ETHERTYPE_IPTRAILERS):ENTYPE_IPTRAILERS;
#else	NEN > 0
		ea->arp_pro = htons(ETHERTYPE_IPTRAILERS);
#endif	NEN > 0
	else if (proto == ETHERTYPE_IP &&
	    (ac->ac_if.if_flags & IFF_NOTRAILERS) == 0)
		mcopy = m_copy(m, 0, M_COPYALL);
	if (eat == &arpethertempl)
	/*
	 *  10Mb ethernet - byte oriented
	 *
	 *  Swap all word quantities.
	 */
	{
	     /* protocol remains as originally supplied */
	     ea->arp_op  = htons(ea->arp_op);
	}
        else
	/*
	 *  3Mb ethernet - word oriented
	 *
	 *  Swap all byte quantities.
	 */
	{
	    register int i;
	
	    *((u_short *)&ea->arp_hln) = htons(*((u_short *)&ea->arp_hln));
	    for (i=2*(eat->arp_pln+eat->arp_hln); i; i-=2)
	        *((u_short *)&ea->arp_sha[i-2]) = htons(*((u_short *)&ea->arp_sha[i-2]));
	}
	sa.sa_family = AF_UNSPEC;
	(*ac->ac_if.if_output)(&ac->ac_if, m, &sa);
	if (mcopy) {
		ea = mtod(mcopy, struct ether_arp *);
#if	NEN > 0
		ea->arp_pro = (eat == &arpethertempl)?htons(ETHERTYPE_IPTRAILERS):ENTYPE_IPTRAILERS;
#else	NEN > 0
		ea->arp_pro = htons(ETHERTYPE_IPTRAILERS);
#endif	NEN > 0
		(*ac->ac_if.if_output)(&ac->ac_if, mcopy, &sa);
	}
	return;
out:
	m_freem(m);
	return;
}

/*
 * Free an arptab entry.
 */
arptfree(at)
	register struct arptab *at;
{
	int s = splimp();

	if (at->at_hold)
		m_freem(at->at_hold);
	at->at_hold = 0;
	at->at_timer = at->at_flags = 0;
	at->at_iaddr.s_addr = 0;
	splx(s);
}

/*
 * Enter a new address in arptab, pushing out the oldest entry 
 * from the bucket if there is no room.
 * This always succeeds since no bucket can be completely filled
 * with permanent entries (except from arpioctl when testing whether
 * another permanent entry will fit).
 */
struct arptab * 
arptnew(ifp, addr)
	struct ifnet *ifp;
	struct in_addr *addr;
{
	register n;
	int oldest = -1;
	register struct arptab *at, *ato = NULL;
	static int first = 1;

	if (first) {
		first = 0;
		timeout(arptimer, (caddr_t)0, hz);
	}
	at = &arptab[ARPTAB_HASH(addr->s_addr) * arptab_bsiz];
	for (n = 0; n < arptab_bsiz; n++,at++)
	{
		if (at->at_flags == 0)
			goto out;	 /* found an empty entry */
		if (at->at_flags & ATF_PERM)
			continue;
		if ((int)at->at_timer > oldest) {
			oldest = at->at_timer;
			ato = at;
		}
	}
	if (ato == NULL)
		return (NULL);
	arptab_displaced++;
	at = ato;
	arptfree(at);
out:
	at->at_iaddr = *addr;
	at->at_flags = ATF_INUSE;
	at->at_if = ifp;
	return (at);
}

arpioctl(cmd, data)
	int cmd;
	caddr_t data;
{
	register struct arpreq *ar = (struct arpreq *)data;
	register struct arptab *at;
	register struct sockaddr_in *sin;
	struct ifaddr *ifa;
	int s;

	if (ar->arp_pa.sa_family != AF_INET ||
	    ar->arp_ha.sa_family != AF_UNSPEC)
		return (EAFNOSUPPORT);
	sin = (struct sockaddr_in *)&ar->arp_pa;
	s = splimp();
	ARPTAB_LOOK(at, sin->sin_addr.s_addr, at->at_if);	/* XXX */
	if (at == NULL) {		/* not found */
		if (cmd != SIOCSARP) {
			splx(s);
			return (ENXIO);
		}
		if ((ifa = ifa_ifwithnet(&ar->arp_pa)) == NULL) {
			splx(s);
			return (ENETUNREACH);
		}
	}
	switch (cmd) {

	case SIOCSARP:		/* set entry */
		if (at == NULL) {
			at = arptnew(ifa->ifa_ifp, &sin->sin_addr);
			if (ar->arp_flags & ATF_PERM) {
			/* never make all entries in a bucket permanent */
				register struct arptab *tat;
				
				/* try to re-allocate */
				tat = arptnew(at->at_if, &sin->sin_addr);
				if (tat == NULL) {
					arptfree(at);
					splx(s);
					return (EADDRNOTAVAIL);
				}
				arptfree(tat);
			}
		}
		bcopy((caddr_t)ar->arp_ha.sa_data, (caddr_t)at->at_enaddr,
		    sizeof(at->at_enaddr));
		at->at_flags = ATF_COM | ATF_INUSE |
			(ar->arp_flags & (ATF_PERM|ATF_PUBL));
		at->at_timer = 0;
		break;

	case SIOCDARP:		/* delete entry */
		arptfree(at);
		break;

	case SIOCGARP:		/* get entry */
		bcopy((caddr_t)at->at_enaddr, (caddr_t)ar->arp_ha.sa_data,
		    sizeof(at->at_enaddr));
		ar->arp_flags = at->at_flags;
		break;
	}
	splx(s);
	return (0);
}

/*
 * Convert Ethernet address to printable (loggable) representation.
 */
char *
ether_sprintf(ap)
	register u_char *ap;
{
	register i;
	static char etherbuf[18];
	register char *cp = etherbuf;
	static char digits[] = "0123456789abcdef";

	for (i = 0; i < 6; i++) {
		*cp++ = digits[*ap >> 4];
		*cp++ = digits[*ap++ & 0xf];
		*cp++ = ':';
	}
	*--cp = 0;
	return (etherbuf);
}

#ifdef VMTP_ETHER
/*
 * Called from vmtp_route
 */
struct arptab *arptab_look(dst)
	u_long dst;
{
	struct arptab *at;

	ARPTAB_LOOK(at, dst);
	return at;
} /* arptab_look */

#endif VMTP_ETHER
