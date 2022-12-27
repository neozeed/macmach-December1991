/* 
 * Mach Operating System
 * Copyright (c) 1987 Carnegie-Mellon University
 * All rights reserved.  The CMU software License Agreement specifies
 * the terms and conditions for use and redistribution.
 */
/* 
 **********************************************************************
 * HISTORY
 * $Log:	inode.h,v $
 * Revision 2.9  91/03/20  15:00:11  dbg
 * 	Added which_fp field.
 * 	[91/03/12            rwd]
 * 
 * Revision 2.8  91/01/08  17:44:26  rpd
 * 	over_ride field for "IT" from rwd
 * 	[90/12/20  16:20:51  rvb]
 * 
 * Revision 2.7  90/11/07  13:21:59  rpd
 * 	Add needs_push field.
 * 	[90/11/06            rwd]
 * 
 * Revision 2.6  90/08/06  15:33:24  rwd
 * 	Remove trunc_zero fields.
 * 	[90/08/03            rwd]
 * 	Clean up conditionals.
 * 	[90/07/31            rwd]
 * 	Added pager_wait_lock field.  This replaces equiv global field.
 * 	[90/07/27            rwd]
 * 	Added trunc_zero fields.
 * 	[90/07/03            rwd]
 * 
 * Revision 2.5  90/06/02  15:25:09  rpd
 * 	Cleaned up conditionals; removed MACH, CMU, CMUCS, MACH_NO_KERNEL.
 * 	[90/04/28            rpd]
 * 	Converted to new IPC.
 * 	[90/03/26  20:03:37  rpd]
 * 
 * Revision 2.4  90/05/29  20:24:29  rwd
 * 	Use file_info struct for mapping info.
 * 	[90/04/30            rwd]
 * 	Added master_count to let us release the master when locking mu
 * 	mutex.
 * 	[90/04/18            rwd]
 * 	Add MACH_NBC MAP_UAREA fields.
 * 	[90/03/19            rwd]
 * 
 * Revision 2.3  90/01/19  14:37:48  rwd
 * 	Picked up rfr's NBC fixes.
 * 	[90/01/15            rwd]
 * 
 * Revision 2.2  89/11/29  15:29:40  af
 * 	[89/11/16  17:09:33  af]
 * 
 * Revision 2.1  89/08/04  14:45:49  rwd
 * Created.
 * 
 * Condensed History:
 * 	Taken from Revision 2.3  88/08/24  02:28:52  mwyoung
 * 	Merge VICE changes -- include vice.h and change to #if VICE.
 * 	MULTIMAX: support Encore's fast symbolic links.
 *	Added changes for ns32000 byte ordering (from dlb).
 *	Set the pager_id field in the inode structure.
 *	Upgraded to 4.3.
 *	Upgraded to 4.2BSD.
 *
 **********************************************************************
 */
/*
 * Copyright (c) 1982, 1986 Regents of the University of California.
 * All rights reserved.  The Berkeley software License Agreement
 * specifies the terms and conditions for redistribution.
 *
 *	@(#)inode.h	7.1 (Berkeley) 6/4/86
 */

#ifdef mac2 /* need mach and cthreads definitions */
#ifndef KERNEL
#include <mach.h>
#include <cthreads.h>
#endif
#endif
 
#ifdef	KERNEL
#include <mach_nbc.h>
#include <map_uarea.h>
#include <cmucs_rfs.h>
#include <vice.h>
#else	KERNEL
#ifndef	MACH_NBC
#define	MACH_NBC	1
#endif	MACH_NBC
#ifndef	MAP_UAREA
#define	MAP_UAREA	1
#endif	MAP_UAREA
#ifndef	CMUCS_RFS
#define	CMUCS_RFS	1
#endif	CMUCS_RFS
#ifndef	VICE
#define	VICE		1
#endif	VICE
#endif	KERNEL

#ifdef	KERNEL
#include <uxkern/import_mach.h>
#endif	KERNEL

#if	MACH_NBC
#if	MAP_UAREA
#include <sys/file_info.h>
#endif	MAP_UAREA
#include <sys/queue.h>
#endif	MACH_NBC

/*
 * The I node is the focus of all file activity in UNIX.
 * There is a unique inode allocated for each active file,
 * each current directory, each mounted-on file, text file, and the root.
 * An inode is 'named' by its dev/inumber pair. (iget/iget.c)
 * Data in icommon is read in from permanent inode on volume.
 */

#if	CMUCS_RFS
/*
 *  The following macro is used to identify the special CMU file type which is
 *  currently used to implement remote node pointer files.
 */
#define	isesctype(ip)	\
	((ip)->i_gid == 64)
 
/*
 *  The following macro is used to distinguish the special CMU remote node pointer.
 */
#define	isrfslnk(ip)	\
	(isesctype(ip) && (((ip)->i_mode&(IFMT|IEXEC|(IEXEC>>3)|(IEXEC>>6))) == (IFREG|IEXEC)))
 
#endif	CMUCS_RFS
#define	NDADDR	12		/* direct addresses in inode */
#define	NIADDR	3		/* indirect addresses in inode */
#if	MULTIMAX
#define MAX_FASTLINK_SIZE	((NDADDR + NIADDR) * sizeof (daddr_t))
#endif	MULTIMAX

struct inode {
	struct	inode *i_chain[2];	/* must be first */
	mach_port_t	pager;
	mach_port_t	pager_request;
#if	MACH_NBC
#if	MAP_UAREA
	struct utask	*who;		/* who is "IT" */
	int		which;		/* which file descriptor for who */
	struct file	*which_fp;	/* fp since close zeros this in uarea*/
	int		wants;		/* someone wants a to be "IT" */
	struct condition mu_condition;	/* condition to wait upon */
	int		count;		/* ref count */
	struct mutex	mu_lock;	/* lock for above fields */
	int		master_count;	/* for releasing master lock */
	struct file_info file_info;	/* for server mapped info */
	vm_size_t	inode_size;
	struct mutex	pager_wait_lock;
	struct condition pager_wait;	/* waiting for pager to be done */
	int		ref_count;	/* server map ref count */
	int		mapped:1,	/* mapped into KERNEL VM? */
			waiting:1,	/* waiting for pager */
			dirty:1,	/* modified */
     			needs_push:1,	/* was open writeable by user */
			temp_ref:1,	/* a temporary reference exists */
			serv_mapped:1,	/* is it mapped by a user */
			user_mapped:1,	/* is it mapped by a user */
			over_ride:1;	/* IT currently overriden */
#else	MAP_UAREA
	short		map_count;	/* number of times mapped */
	short		use_count;	/* number of times in use */
	vm_offset_t	va;		/* mapped virtual address */
	vm_size_t	size;		/* mapped size */
	vm_offset_t	offset;		/* offset into file at va */
	vm_size_t	inode_size;	/* inode size (not reflected in ip) */
	struct mutex	lock;		/* lock for changing window */
	struct mutex	pager_wait_lock;
	struct condition pager_wait;	/* waiting for pager to be done */
	queue_chain_t	lru_links;	/* lru queue links */
	int		queued:1,	/* on lru queue? */
			mapped:1,	/* mapped into KERNEL VM? */
			waiting:1,	/* waiting for pager */
			dirty:1,	/* modified */
			temporary:1;	/* temporary file */
#endif	MAP_UAREA
#endif	MACH_NBC
	u_short	i_flag;
	u_short	i_count;	/* reference count */
	dev_t	i_dev;		/* device where inode resides */
	u_short	i_shlockc;	/* count of shared locks on inode */
	u_short	i_exlockc;	/* count of exclusive locks on inode */
	ino_t	i_number;	/* i number, 1-to-1 with device address */
	long	i_id;		/* unique identifier */
	struct	fs *i_fs;	/* file sys associated with this inode */
	struct	dquot *i_dquot;	/* quota structure controlling this file */
	struct	text *i_text;	/* text entry, if any (should be region) */
	union {
		daddr_t	if_lastr;	/* last read (read-ahead) */
		struct	socket *is_socket;
		struct	{
			struct inode  *if_freef;	/* free list forward */
			struct inode **if_freeb;	/* free list back */
		} i_fr;
	} i_un;
	struct 	icommon
	{
		u_short	ic_mode;	/*  0: mode and type of file */
		short	ic_nlink;	/*  2: number of links to file */
		uid_t	ic_uid;		/*  4: owner's user id */
		gid_t	ic_gid;		/*  6: owner's group id */
		quad	ic_size;	/*  8: number of bytes in file */
		time_t	ic_atime;	/* 16: time last accessed */
		long	ic_atspare;
		time_t	ic_mtime;	/* 24: time last modified */
		long	ic_mtspare;
		time_t	ic_ctime;	/* 32: last time inode changed */
		long	ic_ctspare;
#if	MULTIMAX
		union {
		    struct { 
			daddr_t	Mb_db[NDADDR]; /* 40: disk block addresses*/
			daddr_t	Mb_ib[NIADDR]; /* 88: indirect blocks */
		    } ic_Mb;
#define	ic_db	ic_Mun.ic_Mb.Mb_db
#define	ic_ib	ic_Mun.ic_Mb.Mb_ib
		    char	ic_Msymlink[MAX_FASTLINK_SIZE];
						/* 40: symbolic link name */
		} ic_Mun;
#define ic_symlink	ic_Mun.ic_Msymlink
#else	MULTIMAX
		daddr_t	ic_db[NDADDR];	/* 40: disk block addresses */
		daddr_t	ic_ib[NIADDR];	/* 88: indirect blocks */
#endif	MULTIMAX
		long	ic_flags;	/* 100: status, currently unused */
#if	MULTIMAX
#define	IC_FASTLINK	0x0001		/* Symbolic link in inode */
#endif	MULTIMAX
		long	ic_blocks;	/* 104: blocks actually held */
		long	ic_spare[5];	/* 108: reserved, currently unused */
	} i_ic;
#if	VICE
	dev_t	i_rmt_dev;	/* device for communications with an agent
				   process maintaining a remote fs */
#endif	VICE
};

struct dinode {
	union {
		struct	icommon di_icom;
		char	di_size[128];
	} di_un;
};

#define	i_mode		i_ic.ic_mode
#define	i_nlink		i_ic.ic_nlink
#define	i_uid		i_ic.ic_uid
#define	i_gid		i_ic.ic_gid
#if	BYTE_MSF
#define i_size		i_ic.ic_size.val[1]
#else	BYTE_MSF
#define i_size		i_ic.ic_size.val[0]
#endif	BYTE_MSF
#define	i_db		i_ic.ic_db
#define	i_ib		i_ic.ic_ib
#if	MULTIMAX
#define	i_symlink	i_ic.ic_symlink
#define i_icflags	i_ic.ic_flags
#endif	MULTIMAX
#define	i_atime		i_ic.ic_atime
#define	i_mtime		i_ic.ic_mtime
#define	i_ctime		i_ic.ic_ctime
#define i_blocks	i_ic.ic_blocks
#define	i_rdev		i_ic.ic_db[0]
#define	i_lastr		i_un.if_lastr
#define	i_socket	i_un.is_socket
#define	i_forw		i_chain[0]
#define	i_back		i_chain[1]
#define	i_freef		i_un.i_fr.if_freef
#define	i_freeb		i_un.i_fr.if_freeb

#if	VICE
#define i_vicemagic	i_ic.ic_spare[0]
#define VICEMAGIC	0x84fa1cb6
#define i_vicep1	i_ic.ic_spare[1]
#define i_vicep2	i_ic.ic_spare[2]
#define i_vicep3	i_ic.ic_spare[3]
#define i_vicep4	i_ic.ic_spare[4]
#endif	VICE

#define di_ic		di_un.di_icom
#define	di_mode		di_ic.ic_mode
#define	di_nlink	di_ic.ic_nlink
#define	di_uid		di_ic.ic_uid
#define	di_gid		di_ic.ic_gid
#if	BYTE_MSF
#define di_size		di_ic.ic_size.val[1]
#else	BYTE_MSF
#define di_size		di_ic.ic_size.val[0]
#endif	BYTE_MSF
#define	di_db		di_ic.ic_db
#define	di_ib		di_ic.ic_ib
#if	MULTIMAX
#define	di_symlink	di_ic.ic_symlink
#define	di_icflags	di_ic.ic_flags
#endif	MULTIMAX
#define	di_atime	di_ic.ic_atime
#define	di_mtime	di_ic.ic_mtime
#define	di_ctime	di_ic.ic_ctime
#define	di_rdev		di_ic.ic_db[0]
#define	di_blocks	di_ic.ic_blocks
#if	VICE
#define di_vicemagic	di_ic.ic_spare[0]
#define di_vicep1	di_ic.ic_spare[1]
#define di_vicep2	di_ic.ic_spare[2]
#define di_vicep3	di_ic.ic_spare[3]
#define di_vicep4	di_ic.ic_spare[4]
#endif	VICE

#ifdef KERNEL
/*
 * Invalidate an inode. Used by the namei cache to detect stale
 * information. At an absurd rate of 100 calls/second, the inode
 * table invalidation should only occur once every 16 months.
 */
#define cacheinval(ip)	\
	(ip)->i_id = ++nextinodeid; \
	if (nextinodeid == 0) \
		cacheinvalall();

struct inode *inode;		/* the inode table itself */
struct inode *inodeNINODE;	/* the end of the inode table */
int	ninode;			/* number of slots in the table */
long	nextinodeid;		/* unique id generator */

struct	inode *rootdir;			/* pointer to inode of root directory */

struct	inode *ialloc();
struct	inode *iget();
#ifdef notdef
struct	inode *ifind();
#endif
struct	inode *owner();
struct	inode *maknode();
#if	CMUCS_RFS
struct	inode *cnamei();
#else	CMUCS_RFS
struct	inode *namei();
#endif	CMUCS_RFS

ino_t	dirpref();
#endif

/* flags */
#define	ILOCKED		0x1		/* inode is locked */
#define	IUPD		0x2		/* file has been modified */
#define	IACC		0x4		/* inode access time to be updated */
#define	IMOUNT		0x8		/* inode is mounted on */
#define	IWANT		0x10		/* some process waiting on lock */
#define	ITEXT		0x20		/* inode is pure text prototype */
#define	ICHG		0x40		/* inode has been changed */
#define	ISHLOCK		0x80		/* file has shared lock */
#define	IEXLOCK		0x100		/* file has exclusive lock */
#define	ILWAIT		0x200		/* someone waiting on file lock */
#if	VICE
#define IREMOTE		0x400		/* inode is remote.  Note, this
					   is only used for inodes that are
					   opened without a file descrip-
					   tor, by the kernel */
#endif	VICE
#define	IMOD		0x400		/* inode has been modified */
#define	IRENAME		0x800		/* inode is being renamed */

/* modes */
#define	IFMT		0170000		/* type of file */
#define	IFCHR		0020000		/* character special */
#define	IFDIR		0040000		/* directory */
#define	IFBLK		0060000		/* block special */
#define	IFREG		0100000		/* regular */
#define	IFLNK		0120000		/* symbolic link */
#define	IFSOCK		0140000		/* socket */

#define	ISUID		04000		/* set user id on execution */
#define	ISGID		02000		/* set group id on execution */
#define	ISVTX		01000		/* save swapped text even after use */
#define	IREAD		0400		/* read, write, execute permissions */
#define	IWRITE		0200
#define	IEXEC		0100

#define	ILOCK(ip) { \
	while ((ip)->i_flag & ILOCKED) { \
		(ip)->i_flag |= IWANT; \
		sleep((caddr_t)(ip), PINOD); \
	} \
	(ip)->i_flag |= ILOCKED; \
}

#define	IUNLOCK(ip) { \
	(ip)->i_flag &= ~ILOCKED; \
	if ((ip)->i_flag&IWANT) { \
		(ip)->i_flag &= ~IWANT; \
		wakeup((caddr_t)(ip)); \
	} \
}

#define	IUPDAT(ip, t1, t2, waitfor) { \
	if (ip->i_flag&(IUPD|IACC|ICHG|IMOD)) \
		iupdat(ip, t1, t2, waitfor); \
}

/*
 *  Macros for modifying inode reference counts - used to check for consistency
 *  at each reference.
 *
 *  The idecr_chk() macro is defined such that it doesn't decrement the
 *  reference count when already zero.  This is in order to avoid the nested
 *  iincr panic which would otherwise occur when update() tries to sync the
 *  inode before halting and increments the count back to zero.
 *
 *  The panic message strings are initialized variables rather than constant
 *  strings since they are potentially used in many places.
 */
extern char *PANICMSG_IINCR;
extern char *PANICMSG_IDECR;

#define	iincr_chk(ip)	{ if (++((ip)->i_count) == 0) panic(PANICMSG_IINCR); }
#define	idecr_chk(ip)	{ if (((ip)->i_count) != 0) ((ip)->i_count--); else panic(PANICMSG_IDECR); }

#define	ITIMES(ip, t1, t2) { \
	if ((ip)->i_flag&(IUPD|IACC|ICHG)) { \
		(ip)->i_flag |= IMOD; \
		if ((ip)->i_flag&IACC) \
			(ip)->i_atime = (t1)->tv_sec; \
		if ((ip)->i_flag&IUPD) \
			(ip)->i_mtime = (t2)->tv_sec; \
		if ((ip)->i_flag&ICHG) \
			(ip)->i_ctime = time.tv_sec; \
		(ip)->i_flag &= ~(IACC|IUPD|ICHG); \
	} \
}
