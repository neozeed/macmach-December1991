/* 
 * Mach Operating System
 * Copyright (c) 1990 Carnegie-Mellon University
 * All rights reserved.  The CMU software License Agreement specifies
 * the terms and conditions for use and redistribution.
 */
/*
 * HISTORY
 * $Log:	shared_lock.h,v $
 * Revision 2.4  90/11/05  16:56:40  rpd
 * 	Add spin_lock_t.
 * 	[90/10/31            rwd]
 * 
 * Revision 2.3  90/06/19  23:13:51  rpd
 * 	Minor cleanup.
 * 	[90/06/09            rpd]
 * 
 * Revision 2.2  90/05/29  20:24:34  rwd
 * 	Created from sys/ushared.h
 * 	[90/04/30            rwd]
 * 
 */

#ifndef	_SYS_SHARED_LOCK_H_
#define	_SYS_SHARED_LOCK_H_

typedef struct shared_lock {
	spin_lock_t lock;
	int who;
	int need_wakeup;
} shared_lock_t;

#endif	_SYS_SHARED_LOCK_H_
