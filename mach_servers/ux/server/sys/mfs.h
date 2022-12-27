/* 
 * Mach Operating System
 * Copyright (c) 1987 Carnegie-Mellon University
 * All rights reserved.  The CMU software License Agreement specifies
 * the terms and conditions for use and redistribution.
 */
/*
 *	File:	mfs.h
 *	Author:	Avadis Tevanian, Jr.
 *
 *	Copyright (C) 1987, Avadis Tevanian, Jr.
 *
 *	Header file for mapped file system support.
 *
 * HISTORY
 * $Log:	mfs.h,v $
 * Revision 2.4  90/06/19  23:13:04  rpd
 * 	Added MAP_UAREA declaration of mfs_inode_size.
 * 	[90/06/11            rpd]
 * 
 * Revision 2.3  90/06/02  15:42:47  rpd
 * 	Cleaned up, placed under KERNEL conditional.
 * 	[90/06/02            rpd]
 * 
 * Revision 2.2  90/01/19  14:37:53  rwd
 * 	New version from rfr
 * 	[90/01/15            rwd]
 * 
 * Revision 2.1  89/08/04  14:47:24  rwd
 * Created.
 * 
 * Revision 2.4  88/10/18  03:38:49  mwyoung
 * 	Add pager_request port.
 * 	[88/09/18            mwyoung]
 * 
 * Revision 2.3  88/08/24  02:35:56  mwyoung
 * 	Adjusted include file references.
 * 	[88/08/17  02:17:29  mwyoung]
 * 
 * Revision 2.2  88/07/17  19:01:33  mwyoung
 * Use new memory object types.
 * 
 *
 * 11-Jun-87  William Bolosky (bolosky) at Carnegie-Mellon University
 *	Changed pager_id to pager.
 *
 * 30-Apr-87  Avadis Tevanian (avie) at Carnegie-Mellon University
 *	Created.
 */

#ifndef	_SYS_MFS_H_
#define	_SYS_MFS_H_

#ifdef	KERNEL
#include <mach_nbc.h>
#include <map_uarea.h>

#if	MACH_NBC
#if	MAP_UAREA
#include <sys/types.h>

extern u_long mfs_inode_size();

#else	MAP_UAREA

#define mfs_inode_size(ip) ((ip)->mapped ? ip->inode_size : ip->i_size)

#endif	MAP_UAREA
#else	MACH_NBC

#define mfs_inode_size(ip) (ip->i_size)

#endif	MACH_NBC

#endif	KERNEL
#endif	_SYS_MFS_H_
