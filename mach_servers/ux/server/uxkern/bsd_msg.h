/* 
 * Mach Operating System
 * Copyright (c) 1988 Carnegie-Mellon University
 * All rights reserved.  The CMU software License Agreement specifies
 * the terms and conditions for use and redistribution.
 */
/*
 * HISTORY
 * $Log:	bsd_msg.h,v $
 * Revision 2.4  90/06/02  15:26:48  rpd
 * 	Converted to new IPC.
 * 	[90/03/26  20:08:07  rpd]
 * 
 * Revision 2.3  89/11/29  15:30:13  af
 * 	Added rval2, because some syscalls do not modify it and it
 * 	must be preserved.
 * 	[89/11/20            af]
 * 
 * Revision 2.2  89/10/17  11:26:51  rwd
 * 	Added interrupt return parameter.  Removed INTERRUPT
 * 	and ERROR macros.
 * 	[89/09/21            dbg]
 * 
 */
/*
 * Request and reply message for generic BSD kernel call.
 */

#include <mach/boolean.h>
#include <mach/message.h>

struct	bsd_request {
    mach_msg_header_t	hdr;
    mach_msg_type_t	int_type;	/* int[8] */
    int			rval2;
    int			syscode;
    int			arg[6];
};

struct	bsd_reply {
    mach_msg_header_t	hdr;
    mach_msg_type_t	int_type;	/* int[4] */
    int			retcode;
    int			rval[2];
    boolean_t		interrupt;
};

union bsd_msg {
    struct bsd_request	req;
    struct bsd_reply	rep;
    char		msg[8192];
};

#define	BSD_REQ_MSG_ID		100000
