/*
 **********************************************************************
 * HISTORY
 * $Log:	host.c,v $
 * Revision 2.2  89/10/24  19:02:03  bww
 * 	Fixed arppr() to work with valloc'd arptab [courtesy of
 * 	Robert Baron (rvb)] and changed arppr() to print ethernet
 * 	address in hex format.
 * 	From "[89/01/17            parker]" at CMU.
 * 	Added arppr() routine to display ARP cache.
 * 	From "28-Sep-85  Mike Accetta" at CMU.
 * 	[89/10/24            bww]
 * 
 */
/*
 * Copyright (c) 1983, 1988 Regents of the University of California.
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms are permitted
 * provided that the above copyright notice and this paragraph are
 * duplicated in all such forms and that any documentation,
 * advertising materials, and other materials related to such
 * distribution and use acknowledge that the software was developed
 * by the University of California, Berkeley.  The name of the
 * University may not be used to endorse or promote products derived
 * from this software without specific prior written permission.
 * THIS SOFTWARE IS PROVIDED ``AS IS'' AND WITHOUT ANY EXPRESS OR
 * IMPLIED WARRANTIES, INCLUDING, WITHOUT LIMITATION, THE IMPLIED
 * WARRANTIES OF MERCHANTIBILITY AND FITNESS FOR A PARTICULAR PURPOSE.
 */

#ifndef lint
static char sccsid[] = "@(#)host.c	5.8 (Berkeley) 6/29/88";
#endif /* not lint */

#include <sys/types.h>
#include <sys/mbuf.h>
#include <sys/socket.h>

#include <net/if.h>

#include <netinet/in.h>
#include <netimp/if_imp.h>
#include <netimp/if_imphost.h>

extern	int kmem;
extern 	int nflag;
extern	char *inetname();

#ifdef mac2
hostpr(impsoftcaddr, nimpaddr)
	off_t impsoftcaddr, nimpaddr;
{
	printf("netstat: hostpr() not implemented for mac2.\n");
	return;
}
impstats(impsoftcaddr, nimpaddr)
	off_t impsoftcaddr, nimpaddr;
{
	printf("netstat: impstats() not implemented for mac2.\n");
	return;
}
#else
/*
 * Print the host tables associated with the ARPANET IMP.
 * Symbolic addresses are shown unless the nflag is given.
 */
hostpr(impsoftcaddr, nimpaddr)
	off_t impsoftcaddr, nimpaddr;
{
	struct mbuf *hosts, mb;
	struct imp_softc imp_softc;
	register struct mbuf *m;
	register struct hmbuf *mh;
	register struct host *hp;
	char flagbuf[10], *flags;
	int i, nimp;

	if (impsoftcaddr == 0) {
		printf("imp_softc: symbol not in namelist\n");
		return;
	}
	if (nimpaddr == 0) {
		printf("nimp: symbol not in namelist\n");
		return;
	}
	klseek(kmem, nimpaddr, 0);
	read(kmem, (char *)&nimp, sizeof (nimp));
	klseek(kmem, impsoftcaddr, 0);
	for (i = 0; i < nimp; i++) {
	    read(kmem, (char *)&imp_softc, sizeof (imp_softc));
	    m = imp_softc.imp_hosts;
	    printf("IMP%d Host Table\n", i);
	    printf("%-5.5s %-6.6s %-8.8s %-4.4s %-9.9s %-4.4s %s\n", "Flags",
	        "Host", "Imp", "Qcnt", "Q Address", "RFNM", "Timer");
	    while (m) {
		klseek(kmem, (off_t)m, 0);
		read(kmem, (char *)&mb, sizeof (mb));
		m = &mb;
		mh = mtod(m, struct hmbuf *);
		if (mh->hm_count == 0) {
			m = m->m_next;
			continue;
		}
		for (hp = mh->hm_hosts; hp < mh->hm_hosts + HPMBUF; hp++) {
			if ((hp->h_flags&HF_INUSE) == 0 && hp->h_timer == 0)
				continue;
			flags = flagbuf;
			*flags++ = hp->h_flags&HF_INUSE ? 'A' : 'F';
			if (hp->h_flags&HF_DEAD)
				*flags++ = 'D';
			if (hp->h_flags&HF_UNREACH)
				*flags++ = 'U';
			*flags = '\0';
			printf("%-5.5s %-6d %-8d %-4d %-9x %-4d %d\n",
				flagbuf,
				hp->h_host,
				ntohs(hp->h_imp),
				hp->h_qcnt,
				hp->h_q,
				hp->h_rfnm,
				hp->h_timer);
		}
		m = m->m_next;
	    }
	}
}

impstats(impsoftcaddr, nimpaddr)
	off_t impsoftcaddr, nimpaddr;
{
	struct imp_softc imp_softc;
	int i, nimp;
	extern char *plural();

	if (impsoftcaddr == 0 || nimpaddr == 0)
		return;
	klseek(kmem, nimpaddr, 0);
	read(kmem, (char *)&nimp, sizeof (nimp));
	klseek(kmem, impsoftcaddr, 0);
	for (i = 0; i < nimp; i++) {
		read(kmem, (char *)&imp_softc, sizeof (imp_softc));
		printf("imp%d statistics:\n", i);
#define	p(f, m)		printf(m, imp_softc.f, plural(imp_softc.f))
		p(imp_if.if_ipackets, "\t%u input message%s\n");
		p(imp_if.if_opackets, "\t%u output message%s\n");
		printf("\t%u times output blocked at least %d sec.\n",
		    imp_softc.imp_block, IMP_OTIMER);
		p(imp_incomplete, "\t%u \"incomplete\" message%s\n");
		p(imp_lostrfnm, "\t%u lost RFNM message%s\n");
		p(imp_badrfnm, "\t%u late/bogus RFNM/incomplete message%s\n");
		p(imp_garbage, "\t%u unknown message type%s\n");
	}
}

#endif

#if	CMU
#include <sys/socket.h>
#include <netinet/if_ether.h>

/*
 * Print the ARP cache protocol/hardware address cache.
 * Symbolic addresses are shown unless the nflag is given.
 */
arppr(arptabaddr, bsizaddr, nbaddr)
	off_t arptabaddr;
	off_t bsizaddr;
	off_t nbaddr;
{
	long bsize = 5;
	long nb = 19;
	caddr_t arptabptr;
	register struct arptab *arptab;
	register int i, j = 0, m;
	char flagbuf[10], *flags;
	extern int Aflag;

	if (arptabaddr == 0) {
		printf("arptab: symbol(s) not in namelist\n");
		return;
	}
	if (arptabaddr != 0)
	{
		klseek(kmem, arptabaddr, 0);
		read(kmem, &arptabptr, sizeof (arptabptr));
	}
	if (bsizaddr != 0)
	{
		klseek(kmem, bsizaddr, 0);
		read(kmem, &bsize, sizeof (bsize));
	}
	if (nbaddr != 0)
	{
		klseek(kmem, nbaddr, 0);
		read(kmem, &nb, sizeof (nb));
	}
	m = bsize*nb;
        arptab = (struct arptab *)malloc(sizeof(struct arptab)*m);
	if (arptab == 0)
	{
		printf("arptab: could not allocate %d table entries\n", m);
		return;
	}
	klseek(kmem, arptabptr, 0);
	read(kmem, arptab, sizeof (struct arptab)*m);
	printf("Address Resolution Mapping Table: %d Buckets %d Deep %d Bytes\n",
		nb, bsize, sizeof (struct arptab));
	printf("Cnt  ");
	if (Aflag)
		printf("%-8.8s ", "ADDR");
	printf("%5.5s %-5.5s %-24.24s %s\n",
		"Flags", "Timer", "Host", "Phys Addr");
	for (i=0; i<m; i++) {
		register struct arptab *at = &arptab[i];

		if (at->at_flags == 0)
			continue;
		flags = flagbuf;
		if (at->at_flags&ATF_INUSE)
			*flags++ = 'U';
		if (at->at_flags&ATF_COM)
			*flags++ = 'C';
		if (at->at_flags&ATF_PERM)
			*flags++ = 'P';
		if (at->at_flags&ATF_PUBL)
			*flags++ = 'B';
		if (at->at_flags&ATF_USETRAILERS)
			*flags++ = 'T';
		*flags = '\0';
		printf("%3d: ", ++j);
		if (Aflag)
			printf("%8x ",arptabaddr+((caddr_t)at-(caddr_t)arptab));
		printf("%-5.5s  %3d  %-24.24s ", flagbuf, at->at_timer,
						inetname(at->at_iaddr));
		if (at->at_enaddr[5] == 0 && at->at_enaddr[4] == 0
		&& at->at_enaddr[3] == 0 && at->at_enaddr[2] == 0
		&& at->at_enaddr[1] == 0)
			printf("#%o", at->at_enaddr[0]);
		else
			printf("%02x:%02x:%02x:%02x:%02x:%02x",
			       at->at_enaddr[0], at->at_enaddr[1],
			       at->at_enaddr[2], at->at_enaddr[3],
			       at->at_enaddr[4], at->at_enaddr[5]);
		printf("\n");
	}
}
#endif	/* CMU */
