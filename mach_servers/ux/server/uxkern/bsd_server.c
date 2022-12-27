/* 
 * Mach Operating System
 * Copyright (c) 1991 Carnegie-Mellon University
 * Copyright (c) 1990 Carnegie-Mellon University
 * All rights reserved.  The CMU software License Agreement specifies
 * the terms and conditions for use and redistribution.
 */
/*
 * HISTORY
 * $Log:	bsd_server.c,v $
 * Revision 2.3  91/03/20  15:04:47  dbg
 * 	Include sys/time.h before undefining KERNEL to avoid search for
 * 	<time.h>.
 * 	[91/02/22            dbg]
 * 
 * Revision 2.2  90/06/19  23:15:37  rpd
 * 	Added uxkern/bsd_2_user.c, uxkern/bsd_2_server.c.
 * 
 */
/*
 * Wrapper to compile bsd servers without KERNEL defined.
 */
#include <sys/time.h>

#undef	KERNEL
#include <uxkern/bsd_1_server.c>
#undef	msgh_request_port
#undef	msgh_reply_port
#include <uxkern/bsd_2_user.c>
#undef	msgh_request_port
#undef	msgh_reply_port
#include <uxkern/bsd_2_server.c>
