/* 
 * Mach Operating System
 * Copyright (c) 1987 Carnegie-Mellon University
 * All rights reserved.  The CMU software License Agreement specifies
 * the terms and conditions for use and redistribution.
 */
/*
 * Include file for xpr circular buffer silent tracing.  
 *
 * HISTORY
 * $Log:	xpr.h,v $
 * Revision 2.2  90/06/02  15:26:34  rpd
 * 	Cleaned up conditionals; removed MACH, CMU, CMUCS, MACH_NO_KERNEL.
 * 	[90/04/28            rpd]
 * 
 * Revision 2.1  89/08/04  14:50:21  rwd
 * Created.
 * 
 * Revision 2.5  88/12/19  02:51:59  mwyoung
 * 	Added VM system tags.
 * 	[88/11/22            mwyoung]
 * 
 * Revision 2.4  88/08/24  02:55:54  mwyoung
 * 	Adjusted include file references.
 * 	[88/08/17  02:29:56  mwyoung]
 * 
 *
 *  9-Apr-88  Daniel Julin (dpj) at Carnegie-Mellon University
 *	Added flags for TCP and MACH_NP debugging.
 *
 *  6-Jan-88  Michael Young (mwyoung) at Carnegie-Mellon University
 *	Make the event structure smaller to make it easier to read from
 *	kernel debuggers.
 *
 * 16-Mar-87  Mike Accetta (mja) at Carnegie-Mellon University
 *	MACH:  made XPR_DEBUG definition conditional on MACH
 *	since the routines invoked under it won't link without MACH.
 *	[ V5.1(F7) ]
 */

#ifndef	_SYS_XPR_H_
#define	_SYS_XPR_H_

/*
 * If the kernel flag XPRDEBUG is set, the XPR macro is enabled.  The 
 * macro should be invoked something like the following:
 *	XPR(XPR_SYSCALLS, ("syscall: %d, 0x%x\n", syscallno, arg1);
 * which will expand into the following code:
 *	if (xprflags & XPR_SYSCALLS)
 *		xpr("syscall: %d, 0x%x\n", syscallno, arg1);
 * Xpr will log the pointer to the printf string and up to 6 arguements,
 * along with a timestamp and cpuinfo (for multi-processor systems), into
 * a circular buffer.  The actual printf processing is delayed until after
 * the buffer has been collected.  It is assumed that the text/data segments
 * of the kernel can easily be reconstructed in a post-processor which
 * performs the printf processing.
 *
 * If the XPRDEBUG compilation switch is not set, the XPR macro expands 
 * to nothing.
 */

#ifdef	KERNEL
#include <xpr_debug.h>
#else	KERNEL
#ifndef	XPR_DEBUG
#define	XPR_DEBUG	1
#endif	XPR_DEBUG
#endif	KERNEL

#include <machine/xpr.h>

#if	XPR_DEBUG

#define XPR(flags,xprargs) if(xprflags&flags) xpr xprargs

extern int xprflags;
/*
 * flags for message types.
 */
#define XPR_SYSCALLS	0x00000001
#define XPR_TRAPS	0x00000002
#define XPR_SCHED	0x00000004
#define XPR_NPTCP	0x00000008
#define XPR_NP		0x00000010
#define XPR_TCP		0x00000020

#define	XPR_VM_OBJECT		(1 << 8)
#define	XPR_VM_OBJECT_CACHE	(1 << 9)
#define	XPR_VM_PAGE		(1 << 10)
#define	XPR_VM_PAGEOUT		(1 << 11)
#define	XPR_MEMORY_OBJECT	(1 << 12)
#define	XPR_VM_FAULT		(1 << 13)
#define	XPR_INODE_PAGER		(1 << 14)
#define	XPR_INODE_PAGER_DATA	(1 << 15)

#else	XPR_DEBUG
#define XPR(flags,xprargs)
#endif	XPR_DEBUG

struct xprbuf {
	char 	*msg;
	int	arg1,arg2,arg3,arg4,arg5;
	int	timestamp;
	int	cpuinfo;
};

#endif	_SYS_XPR_H_
