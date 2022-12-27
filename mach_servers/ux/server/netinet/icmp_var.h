#if	CMU
/* 
 * Mach Operating System
 * Copyright (c) 1987 Carnegie-Mellon University
 * All rights reserved.  The CMU software License Agreement specifies
 * the terms and conditions for use and redistribution.
 */
 
/*
 **********************************************************************
 * HISTORY
 * $Log:	icmp_var.h,v $
 * Revision 2.1  89/08/04  14:21:05  rwd
 * Created.
 * 
 * Revision 2.2  88/08/24  01:56:45  mwyoung
 * 	Corrected include file references.
 * 	[88/08/22  18:49:18  mwyoung]
 * 
 *
 * 03-Sep-87  Mike Accetta (mja) at Carnegie-Mellon University
 *	CS_BUGFIX:  Install bug fix from Dan Julin to correct
 *	statistics array dimensions in icmp_stat.
 *	[ V5.1(XF16) ]
 *
 **********************************************************************
 */

#ifdef	KERNEL
#include <cmucs.h>
#endif	KERNEL
#endif	CMU
/*
 * Copyright (c) 1982, 1986 Regents of the University of California.
 * All rights reserved.  The Berkeley software License Agreement
 * specifies the terms and conditions for redistribution.
 *
 *	@(#)icmp_var.h	7.1 (Berkeley) 6/5/86
 */

/*
 * Variables related to this implementation
 * of the internet control message protocol.
 */
struct	icmpstat {
/* statistics related to icmp packets generated */
	int	icps_error;		/* # of calls to icmp_error */
	int	icps_oldshort;		/* no error 'cuz old ip too short */
	int	icps_oldicmp;		/* no error 'cuz old was icmp */
#if	CMUCS
	int	icps_outhist[ICMP_MAXTYPE + 1];
#else	CMUCS
	int	icps_outhist[ICMP_IREQREPLY + 1];
#endif	CMUCS
/* statistics related to input messages processed */
 	int	icps_badcode;		/* icmp_code out of range */
	int	icps_tooshort;		/* packet < ICMP_MINLEN */
	int	icps_checksum;		/* bad checksum */
	int	icps_badlen;		/* calculated bound mismatch */
	int	icps_reflect;		/* number of responses */
#if	CMUCS
	int	icps_inhist[ICMP_MAXTYPE + 1];
#else	CMUCS
	int	icps_inhist[ICMP_IREQREPLY + 1];
#endif	CMUCS
};

#ifdef KERNEL
struct	icmpstat icmpstat;
#endif
