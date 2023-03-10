/* 
 * Mach Operating System
 * Copyright (c) 1988 Carnegie-Mellon University
 * All rights reserved.  The CMU software License Agreement specifies
 * the terms and conditions for use and redistribution.
 */
/*
 **********************************************************************
 * HISTORY
 * $Log:	in_pcb.h,v $
 * Revision 2.1  89/08/04  14:22:31  rwd
 * Created.
 * 
 * Revision 2.2  88/08/24  02:00:34  mwyoung
 * 	Corrected include file references.
 * 	[88/08/22  18:53:29  mwyoung]
 * 
 *
 * 16-May-87  Jay Kistler (jjk) at Carnegie-Mellon University
 *	Merged in code for IP multicast.
 **********************************************************************
 */ 
 
/*
 * Copyright (c) 1982, 1986 Regents of the University of California.
 * All rights reserved.  The Berkeley software License Agreement
 * specifies the terms and conditions for redistribution.
 *
 *	@(#)in_pcb.h	7.1 (Berkeley) 6/5/86
 */

#ifdef	KERNEL
#include <ttlcontrol.h>
#else	KERNEL
#if	CMUCS
#define	TTLCONTROL	1
#endif	CMUCS
#endif	KERNEL

/*
 * Common structure pcb for internet protocol implementation.
 * Here are stored pointers to local and foreign host table
 * entries, local and foreign socket numbers, and pointers
 * up (to a socket structure) and down (to a protocol-specific)
 * control block.
 */
struct inpcb {
	struct	inpcb *inp_next,*inp_prev;
					/* pointers to other pcb's */
	struct	inpcb *inp_head;	/* pointer back to chain of inpcb's
					   for this protocol */
	struct	in_addr inp_faddr;	/* foreign host table entry */
	u_short	inp_fport;		/* foreign port */
	struct	in_addr inp_laddr;	/* local host table entry */
	u_short	inp_lport;		/* local port */
	struct	socket *inp_socket;	/* back pointer to socket */
	caddr_t	inp_ppcb;		/* pointer to per-protocol pcb */
	struct	route inp_route;	/* placeholder for routing entry */
	struct	mbuf *inp_options;	/* IP options */
#if	TTLCONTROL
	u_char	inp_ttl;		/* IP time-to-live */
#endif	TTLCONTROL
};

#define	INPLOOKUP_WILDCARD	1
#define	INPLOOKUP_SETLOCAL	2

#define	sotoinpcb(so)	((struct inpcb *)(so)->so_pcb)

#ifdef	KERNEL
struct	inpcb *in_pcblookup();
#endif	KERNEL
