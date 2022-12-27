/* 
 * Mach Operating System
 * Copyright (c) 1987 Carnegie-Mellon University
 * All rights reserved.  The CMU software License Agreement specifies
 * the terms and conditions for use and redistribution.
 */
/*
 **********************************************************************
 * HISTORY
 * $Log:	stat.h,v $
 * Revision 2.1  89/08/04  14:48:27  rwd
 * Created.
 * 
 * 06-Jan-88  Jay Kistler (jjk) at Carnegie Mellon University
 *	Made file reentrant.  Added declarations for __STDC__.
 *
 * 11-Dec-86  Jonathan J. Chew (jjc) at Carnegie-Mellon University
 *	Added a variation of the stat structure that explicitly puts
 *	in padding where the Vax and RT compilers implicitly put it.
 *	This is for the Sun which does not align long fields on a long
 *	boundary.
 *
 * 25-Jan-86  Avadis Tevanian (avie) at Carnegie-Mellon University
 *	Upgraded to 4.3.
 *
 **********************************************************************
 */
 
/*
 * Copyright (c) 1982, 1986 Regents of the University of California.
 * All rights reserved.  The Berkeley software License Agreement
 * specifies the terms and conditions for redistribution.
 *
 *	@(#)stat.h	7.1 (Berkeley) 6/4/86
 */

#ifndef	_STAT_
#define	_STAT_	1

struct	stat
{
	dev_t	st_dev;
	ino_t	st_ino;
	unsigned short st_mode;
	short	st_nlink;
	uid_t	st_uid;
	gid_t	st_gid;
	dev_t	st_rdev;
	off_t	st_size;
	time_t	st_atime;
	int	st_spare1;
	time_t	st_mtime;
	int	st_spare2;
	time_t	st_ctime;
	int	st_spare3;
	long	st_blksize;
	long	st_blocks;
	long	st_spare4[2];
};

#ifdef	sun
/*
 *	Explicitly padded stat structure
 */
struct	padded_stat
{
	dev_t	st_dev;
	short	st_shortpad1;	/* pad so next field starts on long bound. */
	ino_t	st_ino;
	unsigned short st_mode;
	short	st_nlink;
	uid_t	st_uid;
	gid_t	st_gid;
	dev_t	st_rdev;
	short	st_shortpad2;	/* pad so next field starts on long bound. */
	off_t	st_size;
	time_t	st_atime;
	int	st_spare1;
	time_t	st_mtime;
	int	st_spare2;
	time_t	st_ctime;
	int	st_spare3;
	long	st_blksize;
	long	st_blocks;
	long	st_spare4[2];
};
#endif	sun

#define	S_IFMT	0170000		/* type of file */
#define		S_IFDIR	0040000	/* directory */
#define		S_IFCHR	0020000	/* character special */
#define		S_IFBLK	0060000	/* block special */
#define		S_IFREG	0100000	/* regular */
#define		S_IFLNK	0120000	/* symbolic link */
#define		S_IFSOCK 0140000/* socket */
#define	S_ISUID	0004000		/* set user id on execution */
#define	S_ISGID	0002000		/* set group id on execution */
#define	S_ISVTX	0001000		/* save swapped text even after use */
#define	S_IREAD	0000400		/* read permission, owner */
#define	S_IWRITE 0000200	/* write permission, owner */
#define	S_IEXEC	0000100		/* execute/search permission, owner */

#if	defined(CMUCS) && defined(__STDC__) && !defined(KERNEL)
extern int stat(const char *, struct stat *);
extern int lstat(const char *, struct stat *);
extern int fstat(int, struct stat *);
#endif	defined(CMUCS) && defined(__STDC__) && !defined(KERNEL)
#endif	_STAT_
