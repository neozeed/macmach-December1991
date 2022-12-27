/* 
 * Mach Operating System
 * Copyright (c) 1989 Carnegie-Mellon University
 * All rights reserved.  The CMU software License Agreement specifies
 * the terms and conditions for use and redistribution.
 */
/*
 * HISTORY
 * $Log:	parallel.h,v $
 * Revision 2.2  89/12/08  20:14:56  rwd
 * 	Changed master_lock to mutex
 * 	[89/10/30            rwd]
 * 
 */
/*
 * Serialization for threads within the kernel.
 */

extern struct mutex master_mutex;

#define master_lock() mutex_lock(&master_mutex)

#define master_unlock() mutex_unlock(&master_mutex)

#define	unix_master()	{			\
	if (u.uu_master_lock++ == 0)		\
	    master_lock();			\
}

#define	unix_release()	{			\
	if (--u.uu_master_lock == 0)		\
	    master_unlock();			\
}

#define	unix_reset()	{			\
	if (u.uu_master_lock > 0)		\
	    u.uu_master_lock = 1;		\
}

