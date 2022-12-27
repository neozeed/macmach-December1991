/* 
 * Mach Operating System
 * Copyright (c) 1987 Carnegie-Mellon University
 * All rights reserved.  The CMU software License Agreement specifies
 * the terms and conditions for use and redistribution.
 */
/*
 **********************************************************************
 * HISTORY
 * $Log:	mount.h,v $
 * Revision 2.1  89/08/04  14:47:35  rwd
 * Created.
 * 
 * Revision 2.2  88/08/24  02:36:24  mwyoung
 * 	Adjusted include file references.
 * 	[88/08/17  02:17:44  mwyoung]
 * 
 *
 *  7-Feb-87  Avadis Tevanian (avie) at Carnegie-Mellon University
 *	Merge VICE changes -- include vice.h and change to #if VICE.
 *
 *  2-Dec-86  Jay Kistler (jjk) at Carnegie-Mellon University
 *	VICE:  added fields to mount entries to allow determination
 *	of whether file system is handled by Venus or not.
 *
 **********************************************************************
 */
 
#ifdef	KERNEL
#include <vice.h>
#else	KERNEL
#include <sys/features.h>
#endif	KERNEL
/*
 * Copyright (c) 1982, 1986 Regents of the University of California.
 * All rights reserved.  The Berkeley software License Agreement
 * specifies the terms and conditions for redistribution.
 *
 *	@(#)mount.h	7.1 (Berkeley) 6/4/86
 */

/*
 * Mount structure.
 * One allocated on every mount.
 * Used to find the super block.
 */
struct	mount
{
	dev_t	m_dev;		/* device mounted */
	struct	buf *m_bufp;	/* pointer to superblock */
	struct	inode *m_inodp;	/* pointer to mounted on inode */
	struct	inode *m_qinod;	/* QUOTA: pointer to quota file */
#if	VICE
	char	m_remote;	/* remote file system */
	char	m_mounted;	/* TRUE if this structure is in use */
	struct  inode *m_parent;/* The parent directory to the mounted on 
				   inode.  Bogus method of handling chdir .. 
				   out of remote file system */
#endif	VICE
};
#ifdef KERNEL
struct	mount mount[NMOUNT];
#endif
