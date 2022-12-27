/* 
 * Mach Operating System
 * Copyright (c) 1989 Carnegie-Mellon University
 * All rights reserved.  The CMU software License Agreement specifies
 * the terms and conditions for use and redistribution.
 */
/*
 * HISTORY
 * $Log:	import_mach.h,v $
 * Revision 2.2  90/06/02  15:27:47  rpd
 * 	Added <mach/notify.h>.
 * 	[90/03/28            rpd]
 * 	Removed mach/host.h.
 * 	[90/03/26  20:16:50  rpd]
 * 
 */

/*
 * MACH interface definitions and data for UX out-of-kernel kernel.
 */

/*
 * <mach/mach.h> must be included with 'KERNEL' off
 */
#ifdef	KERNEL
#define	KERNEL__
#undef	KERNEL
#endif	KERNEL

#include <mach/mach.h>
#include <mach/message.h>
#include <mach/notify.h>
#include <mach/mig_errors.h>
#include <cthreads.h>

#ifdef	KERNEL__
#undef	KERNEL__
#define	KERNEL	1
#endif	KERNEL__
