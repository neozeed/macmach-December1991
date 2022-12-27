/* 
 * Mach Operating System
 * Copyright (c) 1991 Carnegie-Mellon University
 * Copyright (c) 1990 Carnegie-Mellon University
 * Copyright (c) 1989 Carnegie-Mellon University
 * All rights reserved.  The CMU software License Agreement specifies
 * the terms and conditions for use and redistribution.
 */
/*
 * HISTORY
 * $Log:	device_utils.h,v $
 * Revision 2.4  91/03/20  15:05:09  dbg
 * 	Distinguish block and character devices.
 * 	[91/02/21            dbg]
 * 
 * Revision 2.3  90/06/02  15:27:32  rpd
 * 	Converted to new IPC.
 * 	[90/03/26  20:13:46  rpd]
 * 
 * Revision 2.2  89/09/15  15:29:30  rwd
 * 	Undefine KERNEL while including device_types.h
 * 	[89/09/11            rwd]
 * 
 */
/*
 * Support routines for device interface in out-of-kernel kernel.
 */
#ifndef	_UXKERN_DEVICE_UTILS_H_
#define	_UXKERN_DEVICE_UTILS_H_

#include <sys/param.h>
#include <sys/types.h>

#include <uxkern/import_mach.h>

#ifdef	KERNEL
#define	KERNEL__
#undef	KERNEL
#endif	KERNEL
#include <device/device_types.h>
#ifdef	KERNEL__
#undef	KERNEL__
#define	KERNEL	1
#endif	KERNEL__

extern mach_port_t	device_server_port;

/*
 * The dev_number_hash table contains both block and character
 * devices.  Distinguish the two.
 */
typedef	int	xdev_t;				/* extended device type */
#define	XDEV_BLOCK(dev)	((xdev_t)(dev) | 0x10000)
#define	XDEV_CHAR(dev)	((xdev_t)(dev))

extern void	dev_utils_init();

extern void	dev_number_hash_enter(/* xdev_t, char * */);
extern void	dev_number_hash_remove(/* xdev_t */);
extern char *	dev_number_hash_lookup(/* xdev_t */);

extern int	dev_error_to_errno(/* int */);

#endif	/* _UXKERN_DEVICE_UTILS_H_ */
