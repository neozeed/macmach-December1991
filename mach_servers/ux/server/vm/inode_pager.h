/* 
 * Mach Operating System
 * Copyright (c) 1987 Carnegie-Mellon University
 * All rights reserved.  The CMU software License Agreement specifies
 * the terms and conditions for use and redistribution.
 */
/*
 ****************************************************************
 * HISTORY
 *  5-Jan-89  David Golub (dbg) at Carnegie-Mellon University
 *	Out-of-kernel version.
 *
 * $Log:	inode_pager.h,v $
 * Revision 2.1  89/08/04  15:11:24  rwd
 * Created.
 * 
 * Revision 2.4  88/10/18  03:42:57  mwyoung
 * 	Added inode_uncache_try(), so that all caching knowledge is
 * 	buried in the inode_pager itself.
 * 	[88/09/18            mwyoung]
 * 
 * Revision 2.3  88/08/25  18:25:30  mwyoung
 * 	Corrected include file references.
 * 	[88/08/22            mwyoung]
 * 	
 * 	Add declaration for inode_pager_release().
 * 	[88/08/11  18:52:57  mwyoung]
 * 
 * Revision 2.2  88/07/17  19:31:49  mwyoung
 * *** empty log message ***
 * 
 *
 *  6-Dec-87  Michael Young (mwyoung) at Carnegie-Mellon University
 *	Removed inode_pager_t structure declaration from this file...
 *	it should not be exported from the inode_pager implementation.
 *
 ****************************************************************
 */

#ifndef	_INODE_PAGER_
#define	_INODE_PAGER_	1

#include <uxkern/import_mach.h>

kern_return_t	inode_swap_preference();

void		inode_pager_bootstrap();

memory_object_t	inode_pager_setup();
void		inode_pager_release();
boolean_t	inode_pager_active();
void		inode_uncache();
boolean_t	inode_uncache_try();

#include <mach_no_kernel.h>

#if	MACH_NO_KERNEL
extern
struct mutex		inode_pager_init_lock;
#else	MACH_NO_KERNEL
#include <sys/lock.h>

extern
simple_lock_data_t	inode_pager_init_lock;
#endif	MACH_NO_KERNEL

void		inode_pager();

#endif	_INODE_PAGER_
