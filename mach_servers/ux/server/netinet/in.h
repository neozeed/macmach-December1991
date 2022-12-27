/* 
 **********************************************************************
 * Mach Operating System
 * Copyright (c) 1987 Carnegie-Mellon University
 * All rights reserved.  The CMU software License Agreement specifies
 * the terms and conditions for use and redistribution.
 */
/*
 **********************************************************************
 * HISTORY
 * $Log:	in.h,v $
 * Revision 2.2  89/11/29  15:29:07  af
 * 	Byte order (carried over from X115).
 * 	[89/11/22            af]
 * 
 * Revision 2.1  89/08/04  14:22:20  rwd
 * Created.
 * 
 *  1-Jul-87  Daniel Julin (dpj) at Carnegie-Mellon University
 *	Updated from new VMTP sources from Stanford (June 87).
 *
 *  4-Jun-87  Avadis Tevanian (avie) at Carnegie-Mellon University
 *	Put multicast features in #ifdef KERNEL.
 *
 * 28-May-87  Daniel Julin (dpj) at Carnegie-Mellon University
 *	Added IPPROTO_VMTP.
 *
 * 16-May-87  Jay Kistler (jjk) at Carnegie-Mellon University
 *	Merged in code for IP multicast.
 *
 *  8-Oct-86  David L. Black (dlb) at Carnegie-Mellon University
 *	Defined byte order macros for ns32000.
 *
 **********************************************************************
 */ 
 
#ifndef	_IN_
#define	_IN_

/*
 * Copyright (c) 1982, 1986 Regents of the University of California.
 * All rights reserved.  The Berkeley software License Agreement
 * specifies the terms and conditions for redistribution.
 *
 *	@(#)in.h	7.1 (Berkeley) 6/5/86
 */

#ifdef	KERNEL
#include "igmproto.h"
#include "multicast.h"
#include "ttlcontrol.h"
#include "mach_vmtp.h"
#ifdef mac2 /* define struct protosw */
#include <sys/protosw.h>
#endif
#else	KERNEL
#include <sys/features.h>
#endif	KERNEL

/*
 * Constants and structures defined by the internet system,
 * Per RFC 790, September 1981.
 */

/*
 * Protocols
 */
#define	IPPROTO_IP		0		/* dummy for IP */
#define	IPPROTO_ICMP		1		/* control message protocol */
#if	IGMPROTO
#define	IPPROTO_IGMP		2		/* group management protocol */
#endif	IGMPROTO
#define	IPPROTO_GGP		2		/* gateway^2 (deprecated) */
#define	IPPROTO_TCP		6		/* tcp */
#define	IPPROTO_EGP		8		/* exterior gateway protocol */
#define	IPPROTO_PUP		12		/* pup */
#define	IPPROTO_UDP		17		/* user datagram protocol */
#define	IPPROTO_IDP		22		/* xns idp */
#if	MACH_VMTP
#define IPPROTO_VMTP		80		/* vmtp */
#define IPPROTO_VMTP2		81		/* internal:  big-endians */
#define IPPROTO_VMTPS		82		/* internal? - secure vmtp */
#define IPPROTO_VMTPS2		83		/* internal: secure for big-endians */
#endif	MACH_VMTP

#define	IPPROTO_RAW		255		/* raw IP packet */
#define	IPPROTO_MAX		256


/*
 * Ports < IPPORT_RESERVED are reserved for
 * privileged processes (e.g. root).
 * Ports > IPPORT_USERRESERVED are reserved
 * for servers, not necessarily privileged.
 */
#define	IPPORT_RESERVED		1024
#define	IPPORT_USERRESERVED	5000

/*
 * Link numbers
 */
#define	IMPLINK_IP		155
#define	IMPLINK_LOWEXPER	156
#define	IMPLINK_HIGHEXPER	158

/*
 * Internet address (a structure for historical reasons)
 */
struct in_addr {
	u_long s_addr;
};

/*
 * Definitions of bits in internet address integers.
 * On subnets, the decomposition of addresses to host and net parts
 * is done according to subnet mask, not the masks here.
 */
#define	IN_CLASSA(i)		(((long)(i) & 0x80000000) == 0)
#define	IN_CLASSA_NET		0xff000000
#define	IN_CLASSA_NSHIFT	24
#define	IN_CLASSA_HOST		0x00ffffff
#define	IN_CLASSA_MAX		128

#define	IN_CLASSB(i)		(((long)(i) & 0xc0000000) == 0x80000000)
#define	IN_CLASSB_NET		0xffff0000
#define	IN_CLASSB_NSHIFT	16
#define	IN_CLASSB_HOST		0x0000ffff
#define	IN_CLASSB_MAX		65536

#if	MULTICAST
#define	IN_CLASSC(i)		(((long)(i) & 0xe0000000) == 0xc0000000)
#define	IN_CLASSC_NET		0xffffff00
#define	IN_CLASSC_NSHIFT	8
#define	IN_CLASSC_HOST		0x000000ff

#define	IN_CLASSD(i)		(((long)(i) & 0xf0000000) == 0xe0000000)
#define	IN_CLASSD_NET		0xf0000000
#define	IN_CLASSD_NSHIFT	28
#define	IN_CLASSD_HOST		0x0fffffff
#define	IN_CLASSD_MASK		0xefffffff

#else	MULTICAST

#define	IN_CLASSC(i)		(((long)(i) & 0xc0000000) == 0xc0000000)
#define	IN_CLASSC_NET		0xffffff00
#define	IN_CLASSC_NSHIFT	8
#define	IN_CLASSC_HOST		0x000000ff
#endif	MULTICAST

#define	INADDR_ANY		(u_long)0x00000000
#define	INADDR_BROADCAST	(u_long)0xffffffff	/* must be masked */
#if	IGMPROTO
#define INADDR_MA_GROUP		(u_long)0xe0000001
#endif	IGMPROTO

#ifndef KERNEL
#define	INADDR_NONE		0xffffffff		/* -1 return */
#endif

/*
 * Socket address, internet style.
 */
struct sockaddr_in {
	short	sin_family;
	u_short	sin_port;
	struct	in_addr sin_addr;
	char	sin_zero[8];
};

/*
 * Options for use with [gs]etsockopt at the IP level.
 */
#define	IP_OPTIONS	1		/* set/get IP per-packet options */
#if	TTLCONTROL
#define	IP_TIMETOLIVE	2		/* set/get IP time-to-live value */
#endif	TTLCONTROL

/*
 * Macros for number representation conversion.
 */
#if	BYTE_MSF
#define ntohl(x)	(x)
#define ntohs(x)	(x)
#define htonl(x)	(x)
#define htons(x)	(x)
#endif	BYTE_MSF

#if	!defined(ntohl)
u_short	ntohs(), htons();
u_long	ntohl(), htonl();
#endif

#ifdef	KERNEL
extern	struct domain inetdomain;
extern	struct protosw inetsw[];
struct	in_addr in_makeaddr();
u_long	in_netof(), in_lnaof();
#endif	KERNEL
#endif	_IN_
