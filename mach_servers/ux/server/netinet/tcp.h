/* 
 **********************************************************************
 * Mach Operating System
 * Copyright (c) 1987 Carnegie-Mellon University
 * All rights reserved.  The CMU software License Agreement specifies
 * the terms and conditions for use and redistribution.
 **********************************************************************
 * HISTORY
 * $Log:	tcp.h,v $
 * Revision 2.2  89/11/29  15:29:17  af
 * 	Byte-order.
 * 	[89/11/16  17:11:30  af]
 * 
 * Revision 2.1  89/08/04  14:23:25  rwd
 * Created.
 * 
 * 27-Nov-87  Mike Accetta (mja) at Carnegie-Mellon University
 *	Upgraded to version 7.2 (Berkeley).
 *	[ V5.1(XF21) ]
 *
 * 21-Oct-86  Jonathan J. Chew (jjc) at Carnegie-Mellon University
 *	Added definitions of th_x2 and th_off for mc68000
 *
 *  7-Oct-86  David L. Black (dlb) at Carnegie-Mellon University
 *	Added definitions of th_x2 and th_off for ns32000
 *
 * 17-Feb-86  Bill Bolosky (bolosky) at Carnegie-Mellon University
 *	Added definitions of th_x2 and th_off for Sailboat under switch
 *	ROMP.
 *
 **********************************************************************
 */
/*
 * Copyright (c) 1982, 1986 Regents of the University of California.
 * All rights reserved.  The Berkeley software License Agreement
 * specifies the terms and conditions for redistribution.
 *
 *	@(#)tcp.h	7.2 (Berkeley) 10/28/86
 */

typedef	u_long	tcp_seq;
/*
 * TCP header.
 * Per RFC 793, September, 1981.
 */
struct tcphdr {
	u_short	th_sport;		/* source port */
	u_short	th_dport;		/* destination port */
	tcp_seq	th_seq;			/* sequence number */
	tcp_seq	th_ack;			/* acknowledgement number */
#if	BYTE_MSF
	u_char	th_off:4,		/* data offset */
		th_x2:4;		/* (unused) */
#else
	u_char	th_x2:4,		/* (unused) */
		th_off:4;		/* data offset */
#endif
	u_char	th_flags;
#define	TH_FIN	0x01
#define	TH_SYN	0x02
#define	TH_RST	0x04
#define	TH_PUSH	0x08
#define	TH_ACK	0x10
#define	TH_URG	0x20
	u_short	th_win;			/* window */
	u_short	th_sum;			/* checksum */
	u_short	th_urp;			/* urgent pointer */
};

#define	TCPOPT_EOL	0
#define	TCPOPT_NOP	1
#define	TCPOPT_MAXSEG	2

/*
 * Default maximum segment size for TCP.
 * With an IP MSS of 576, this is 536,
 * but 512 is probably more convenient.
 */
#ifdef	lint
#define	TCP_MSS	536
#else
#ifndef IP_MSS
#define	IP_MSS	576
#endif
#define	TCP_MSS	MIN(512, IP_MSS - sizeof (struct tcpiphdr))
#endif

/*
 * User-settable options (used with setsockopt).
 */
#define	TCP_NODELAY	0x01	/* don't delay send to coalesce packets */
#define	TCP_MAXSEG	0x02	/* set maximum segment size */
