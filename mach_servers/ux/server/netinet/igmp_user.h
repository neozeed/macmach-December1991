/* 
 * Mach Operating System
 * Copyright (c) 1991 Carnegie-Mellon University
 * Copyright (c) 1987 Carnegie-Mellon University
 * All rights reserved.  The CMU software License Agreement specifies
 * the terms and conditions for use and redistribution.
 */
/*
 * HISTORY
 * $Log:	igmp_user.h,v $
 * Revision 2.2  91/03/20  15:02:41  dbg
 * 	Fix IOCTLs for ANSI C.
 * 	[91/02/21            dbg]
 * 
 * Revision 2.1  89/08/04  14:22:02  rwd
 * Created.
 * 
 */ 

/*
 *	igmp_user.h
 */

#include "igmproto.h"

#if	IGMPROTO

#define SIOCIGMPREQ _IOWR('i',29,struct igmpreqargs)

struct igmpreqargs {
	char	ifr_name[IFNAMSIZ];	/* Interface name */
	u_char  type;
	u_char  code;
	struct  in_addr groupaddr;
	u_long	key1;
	u_long	key2;
	u_char	loopback;		/* Boolean for Create and Join only */
};   

#endif	IGMPROTO
