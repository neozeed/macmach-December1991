/* 
 * Mach Operating System
 * Copyright (c) 1989 Carnegie-Mellon University
 * All rights reserved.  The CMU software License Agreement specifies
 * the terms and conditions for use and redistribution.
 */
/*
 * HISTORY
 * $Log:	device_reply_hdlr.h,v $
 * Revision 2.2  90/06/02  15:27:28  rpd
 * 	Converted to new IPC.
 * 	[90/03/26  20:13:26  rpd]
 * 
 */

/*
 * Handler for device read and write replies.
 */
#ifndef	DEVICE_REPLY_HDLR_H
#define	DEVICE_REPLY_HDLR_H

#include <mach/kern_return.h>

typedef kern_return_t	(*kr_fn_t)();

extern void	reply_hash_enter();	/* mach_port_t, char *, kr_fn_t, kr_fn_t */
extern void	reply_hash_remove();	/* mach_port_t */

extern void	device_reply_hdlr();

#endif	DEVICE_REPLY_HDLR_H

