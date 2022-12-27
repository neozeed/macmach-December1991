/* 
 * Mach Operating System
 * Copyright (c) 1989 Carnegie-Mellon University
 * All rights reserved.  The CMU software License Agreement specifies
 * the terms and conditions for use and redistribution.
 */
/*
 * HISTORY
 * $Log:	device.h,v $
 * Revision 2.2  89/09/15  15:29:19  rwd
 * 	Created
 * 	[89/09/11            rwd]
 * 
 */
#ifdef	KERNEL
#define	KERNEL__
#undef	KERNEL
#endif	KERNEL

#include <device/device.h>

#ifdef	KERNEL__
#undef	KERNEL__
#define	KERNEL	1
#endif	KERNEL__

