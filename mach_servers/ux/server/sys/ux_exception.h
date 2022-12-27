/* 
 * Mach Operating System
 * Copyright (c) 1987 Carnegie-Mellon University
 * All rights reserved.  The CMU software License Agreement specifies
 * the terms and conditions for use and redistribution.
 */
/*
 **********************************************************************
 * HISTORY
 * $Log:	ux_exception.h,v $
 * Revision 2.2  90/06/02  15:26:20  rpd
 * 	Cleaned up conditionals; removed MACH, CMU, CMUCS, MACH_NO_KERNEL.
 * 	[90/04/28            rpd]
 * 	Converted to new IPC.
 * 	Added EXC_UNIX_ABORT.
 * 
 * 	Out-of-kernel version.
 * 	[89/01/06            dbg]
 * 
 * Revision 2.1  89/08/04  14:46:33  rwd
 * Created.
 * 
 * Revision 2.2  88/08/24  02:52:12  mwyoung
 * 	Adjusted include file references.
 * 	[88/08/17  02:27:27  mwyoung]
 * 
 *
 * 29-Sep-87  David Black (dlb) at Carnegie-Mellon University
 *	Created.
 *
 **********************************************************************
 */

#ifndef	_SYS_UX_EXCEPTION_H_
#define	_SYS_UX_EXCEPTION_H_

/*
 *	Codes for Unix software exceptions under EXC_SOFTWARE.
 */


#define EXC_UNIX_BAD_SYSCALL	0x10000		/* SIGSYS */

#define EXC_UNIX_BAD_PIPE	0x10001		/* SIGPIPE */

#define EXC_UNIX_ABORT		0x10002		/* SIGABRT */

#ifdef	KERNEL
#include <uxkern/import_mach.h>

/*
 *	Kernel data structures for Unix exception handler.
 */

struct mutex		ux_handler_init_lock;
mach_port_t		ux_exception_port;

#endif	KERNEL
#endif	_SYS_UX_EXCEPTION_H_
