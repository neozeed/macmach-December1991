/* 
 * Mach Operating System
 * Copyright (c) 1991 Carnegie-Mellon University
 * Copyright (c) 1987 Carnegie-Mellon University
 * All rights reserved.  The CMU software License Agreement specifies
 * the terms and conditions for use and redistribution.
 */
/*
 * HISTORY
 * $Log:	uipc_domain.c,v $
 * Revision 2.3  91/03/20  15:02:08  dbg
 * 	Fix for ANSI C.
 * 	[91/02/22            dbg]
 * 
 * Revision 2.2  91/01/08  15:00:24  rpd
 * 	Fixed ADDDOMAN for gcc.
 * 	[91/01/05            rpd]
 * 
 * Revision 2.1  89/08/04  14:15:44  rwd
 * Created.
 * 
 * 06-May-87  Mike Accetta (mja) at Carnegie-Mellon University
 *	DLI:  Add data-link interface protocol initialization to
 *	domaininit().
 *	[ V5.1(F10) ]
 *
 */ 
 
#include <dli.h>
/*
 * Copyright (c) 1982, 1986 Regents of the University of California.
 * All rights reserved.  The Berkeley software License Agreement
 * specifies the terms and conditions for redistribution.
 *
 *	@(#)uipc_domain.c	7.1 (Berkeley) 6/5/86
 */

#include <sys/param.h>
#include <sys/socket.h>
#include <sys/protosw.h>
#include <sys/domain.h>
#include <sys/time.h>
#include <sys/kernel.h>

/*
 *	It seems some versions of the preprocessor will expand
 *	arguments before substituting.
 */
#undef	unix

#ifdef	__STDC__
#define	ADDDOMAIN(x)	{ \
	extern struct domain x ## domain; \
	x ## domain.dom_next = domains; \
	domains = &x ## domain; \
}
#else
#define	ADDDOMAIN(x)	{ \
	extern struct domain x/**/domain; \
	x/**/domain.dom_next = domains; \
	domains = &x/**/domain; \
}
#endif

domaininit()
{
	register struct domain *dp;
	register struct protosw *pr;

#ifndef lint
	ADDDOMAIN(unix);
#ifdef INET
	ADDDOMAIN(inet);
#endif
#ifdef NS
	ADDDOMAIN(ns);
#endif
#include "imp.h"
#if NIMP > 0
	ADDDOMAIN(imp);
#endif
#if	DLI
	ADDDOMAIN(dli);
#endif	DLI
#endif

	for (dp = domains; dp; dp = dp->dom_next) {
		if (dp->dom_init)
			(*dp->dom_init)();
		for (pr = dp->dom_protosw; pr < dp->dom_protoswNPROTOSW; pr++)
			if (pr->pr_init)
				(*pr->pr_init)();
	}
	null_init();
	pffasttimo();
	pfslowtimo();
}

struct protosw *
pffindtype(family, type)
	int family, type;
{
	register struct domain *dp;
	register struct protosw *pr;

	for (dp = domains; dp; dp = dp->dom_next)
		if (dp->dom_family == family)
			goto found;
	return (0);
found:
	for (pr = dp->dom_protosw; pr < dp->dom_protoswNPROTOSW; pr++)
		if (pr->pr_type && pr->pr_type == type)
			return (pr);
	return (0);
}

struct protosw *
pffindproto(family, protocol, type)
	int family, protocol, type;
{
	register struct domain *dp;
	register struct protosw *pr;
	struct protosw *maybe = 0;

	if (family == 0)
		return (0);
	for (dp = domains; dp; dp = dp->dom_next)
		if (dp->dom_family == family)
			goto found;
	return (0);
found:
	for (pr = dp->dom_protosw; pr < dp->dom_protoswNPROTOSW; pr++) {
		if ((pr->pr_protocol == protocol) && (pr->pr_type == type))
			return (pr);

		if (type == SOCK_RAW && pr->pr_type == SOCK_RAW &&
		    pr->pr_protocol == 0 && maybe == (struct protosw *)0)
			maybe = pr;
	}
	return (maybe);
}

pfctlinput(cmd, sa)
	int cmd;
	struct sockaddr *sa;
{
	register struct domain *dp;
	register struct protosw *pr;

	for (dp = domains; dp; dp = dp->dom_next)
		for (pr = dp->dom_protosw; pr < dp->dom_protoswNPROTOSW; pr++)
			if (pr->pr_ctlinput)
				(*pr->pr_ctlinput)(cmd, sa);
}

pfslowtimo()
{
	register struct domain *dp;
	register struct protosw *pr;

	for (dp = domains; dp; dp = dp->dom_next)
		for (pr = dp->dom_protosw; pr < dp->dom_protoswNPROTOSW; pr++)
			if (pr->pr_slowtimo)
				(*pr->pr_slowtimo)();
	timeout(pfslowtimo, (caddr_t)0, hz/2);
}

pffasttimo()
{
	register struct domain *dp;
	register struct protosw *pr;

	for (dp = domains; dp; dp = dp->dom_next)
		for (pr = dp->dom_protosw; pr < dp->dom_protoswNPROTOSW; pr++)
			if (pr->pr_fasttimo)
				(*pr->pr_fasttimo)();
	timeout(pffasttimo, (caddr_t)0, hz/5);
}
