/* 
 * Mach Operating System
 * Copyright (c) 1987 Carnegie-Mellon University
 * All rights reserved.  The CMU software License Agreement specifies
 * the terms and conditions for use and redistribution.
 */
/*
 **********************************************************************
 * HISTORY
 * $Log:	file.h,v $
 * Revision 2.2  90/05/29  20:24:25  rwd
 * 	Change to allow its inclusion more than once.
 * 	[90/03/27            rwd]
 * 
 * Revision 2.1  89/08/04  14:45:22  rwd
 * Created.
 * 
 * Revision 2.3  88/08/24  02:27:48  mwyoung
 * 	Adjusted include file references.
 * 	[88/08/17  02:12:42  mwyoung]
 * 
 *
 * 30-May-87  Mike Accetta (mja) at Carnegie-Mellon University
 *	CMUCS:  Renamed CS_BUGFIX conditionals so that KERNEL_FILE
 *	change will be visible to applications without special
 *	features.
 *	[ V5.1(XF11) ]
 *
 *  7-Feb-87  Avadis Tevanian (avie) at Carnegie-Mellon University
 *	Merge VICE changes -- include vice.h and change to #if VICE.
 *
 * 27-Jan-87  Mike Accetta (mja) at Carnegie-Mellon University
 *	Added FNOSPC definition (and renumbered FDIROK pending its
 *	permanent removal).
 *	[ V5.1(F1) ]
 *
 *  2-Dec-86  Jay Kistler (jjk) at Carnegie-Mellon University
 *	VICE: 1/ added f_rmt_fileid field to "file" struct;
 *	      2/ added DTYPE_VICEINO for Vice remote files;
 *
 * 26-Mar-86  Mike Accetta (mja) at Carnegie-Mellon University
 *	CS_BUGFIX: Fixed KERNEL_FILE symbol to include all
 *	definitions.
 *
 * 25-Jan-86  Avadis Tevanian (avie) at Carnegie-Mellon University
 *	Upgraded to 4.3.
 *
 * 25-Nov-85  Mike Accetta (mja) at Carnegie-Mellon University
 *	CS_BUGFIX: Added new KERNEL_FILE symbol to enable struct
 *	file definitions when desired outside the kernel.
 *
 * 20-Jul-85  Mike Accetta (mja) at Carnegie-Mellon University
 *	CS_RFS:  Added DTYPE_RFSINO definition.
 *	[V1(1)]
 *
 **********************************************************************
 */
 
#ifndef	_SYS_FILE_H_
#define _SYS_FILE_H_

#ifdef	KERNEL
#include <cmucs.h>
#include <cmucs_rfs.h>
#include <vice.h>
#else	KERNEL
#include <sys/features.h>
#endif	KERNEL
/*
 * Copyright (c) 1982, 1986 Regents of the University of California.
 * All rights reserved.  The Berkeley software License Agreement
 * specifies the terms and conditions for redistribution.
 *
 *	@(#)file.h	7.1 (Berkeley) 6/4/86
 */

#if	CMUCS
#ifdef	KERNEL_FILE
#define	KERNEL
#endif	KERNEL_FILE
#endif	CMUCS
#ifdef KERNEL
/*
 * Descriptor table entry.
 * One for each kernel object.
 */
struct	file {
	int	f_flag;		/* see below */
	short	f_type;		/* descriptor type */
	short	f_count;	/* reference count */
	short	f_msgcount;	/* references from message queue */
	struct	fileops {
		int	(*fo_rw)();
		int	(*fo_ioctl)();
		int	(*fo_select)();
		int	(*fo_close)();
	} *f_ops;
	caddr_t	f_data;		/* inode */
	off_t	f_offset;
#if	VICE
	int	f_rmt_fileid;
#endif	VICE
};

struct	file *file, *fileNFILE;
int	nfile;
struct	file *getf();
struct	file *falloc();
#endif

/*
 * flags- also for fcntl call.
 */
#define	FOPEN		(-1)
#define	FREAD		00001		/* descriptor read/receive'able */
#define	FWRITE		00002		/* descriptor write/send'able */
#ifndef	F_DUPFD
#define	FNDELAY		00004		/* no delay */
#define	FAPPEND		00010		/* append on each write */
#endif
#define	FMARK		00020		/* mark during gc() */
#define	FDEFER		00040		/* defer for next gc pass */
#ifndef	F_DUPFD
#define	FASYNC		00100		/* signal pgrp when data ready */
#endif
#define	FSHLOCK		00200		/* shared lock present */
#define	FEXLOCK		00400		/* exclusive lock present */

/* bits to save after open */
#define	FMASK		00113
#define	FCNTLCANT	(FREAD|FWRITE|FMARK|FDEFER|FSHLOCK|FEXLOCK)

/* open only modes */
#define	FCREAT		01000		/* create if nonexistant */
#define	FTRUNC		02000		/* truncate to zero length */
#define	FEXCL		04000		/* error if already created */
#if	CMUCS
#define	FNOSPC		010000		/* disable resource pause for file */
#endif	CMUCS

#ifndef	F_DUPFD
/* fcntl(2) requests--from <fcntl.h> */
#define	F_DUPFD	0	/* Duplicate fildes */
#define	F_GETFD	1	/* Get fildes flags */
#define	F_SETFD	2	/* Set fildes flags */
#define	F_GETFL	3	/* Get file flags */
#define	F_SETFL	4	/* Set file flags */
#define	F_GETOWN 5	/* Get owner */
#define F_SETOWN 6	/* Set owner */
#endif

/*
 * User definitions.
 */

/*
 * Open call.
 */
#define	O_RDONLY	000		/* open for reading */
#define	O_WRONLY	001		/* open for writing */
#define	O_RDWR		002		/* open for read & write */
#define	O_NDELAY	FNDELAY		/* non-blocking open */
#define	O_APPEND	FAPPEND		/* append on each write */
#define	O_CREAT		FCREAT		/* open with file create */
#define	O_TRUNC		FTRUNC		/* open with truncation */
#define	O_EXCL		FEXCL		/* error on create if file exists */

/*
 * Flock call.
 */
#define	LOCK_SH		1	/* shared lock */
#define	LOCK_EX		2	/* exclusive lock */
#define	LOCK_NB		4	/* don't block when locking */
#define	LOCK_UN		8	/* unlock */

/*
 * Access call.
 */
#define	F_OK		0	/* does file exist */
#define	X_OK		1	/* is it executable by caller */
#define	W_OK		2	/* writable by caller */
#define	R_OK		4	/* readable by caller */

/*
 * Lseek call.
 */
#define	L_SET		0	/* absolute offset */
#define	L_INCR		1	/* relative to current offset */
#define	L_XTND		2	/* relative to end of file */

#ifdef KERNEL
#define	GETF(fp, fd) { \
	if ((unsigned)(fd) >= NOFILE || ((fp) = u.u_ofile[fd]) == NULL) { \
		u.u_error = EBADF; \
		return; \
	} \
}
#define	DTYPE_INODE	1	/* file */
#define	DTYPE_SOCKET	2	/* communications endpoint */
#if	CMUCS_RFS
#define	DTYPE_RFSINO	3	/* remote file system inode */
#define	DTYPE_RFSCTL	4	/* remote file system control */
#endif	CMUCS_RFS
#if	VICE
#define DTYPE_VICEINO	5	/* Vice remote file system inode */
#endif	VICE
#endif
#if	CMUCS
#ifdef	KERNEL_FILE
#undef	KERNEL
#endif	KERNEL_FILE
#endif	CMUCS

#endif	_SYS_FILE_H_
