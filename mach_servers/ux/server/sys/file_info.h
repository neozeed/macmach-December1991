/* 
 * Mach Operating System
 * Copyright (c) 1990 Carnegie-Mellon University
 * All rights reserved.  The CMU software License Agreement specifies
 * the terms and conditions for use and redistribution.
 */
/*
 * HISTORY
 * $Log:	file_info.h,v $
 * Revision 2.2  90/05/29  20:24:27  rwd
 * 	Created from sys/ushared.h
 * 	[90/04/30            rwd]
 * 
 *
 */

#ifndef	_FILE_INFO_
#define	_FILE_INFO_

#include <sys/shared_lock.h>

typedef	struct file_info {
	    shared_lock_t
			lock;
	    int		open:1,
			read:1,
			write:1,
			append:1,
			mapped:1,
	    		control:1,
	    		inuse:1,
			wants:1,
			dirty:1;
	    int		offset;
	    struct map_info {
		int	address;
		int	size;
		int	offset;
		int	inode_size;
	    } map_info;
	} file_info_t;
#endif	_FILE_INFO_
