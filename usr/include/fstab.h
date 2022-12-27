/*
 * Copyright (c) 1980 Regents of the University of California.
 * All rights reserved.  The Berkeley software License Agreement
 * specifies the terms and conditions for redistribution.
 *
 *	@(#)fstab.h	5.1 (Berkeley) 5/30/85
 *
 **********************************************************************
 * HISTORY
 * $Log:	fstab.h,v $
 * Revision 2.4  90/01/23  17:24:58  berman
 * 	Added fs_name and fs_opts for new fstab formats.
 * 	[90/01/18            berman]
 * 
 * Revision 2.3  89/01/07  14:00:24  gm0w
 * 	Changed STDC conditional from CMUCS to CMU.  Fixed incorrect
 * 	STDC declaration for getfsent().
 * 	[88/12/17            gm0w]
 * 
 * Revision 2.2  88/12/14  23:30:48  mja
 * 	Added ANSI-C (and C++) compatible argument declarations.
 * 	[88/01/18            dld@cs.cmu.edu]
 * 
 **********************************************************************
 */

/*
 * File system table, see fstab (5)
 *
 * Used by dump, mount, umount, swapon, fsck, df, ...
 *
 * The fs_spec field is the block special name.  Programs
 * that want to use the character special name must create
 * that name by prepending a 'r' after the right most slash.
 * Quota files are always named "quotas", so if type is "rq",
 * then use concatenation of fs_file and "quotas" to locate
 * quota file.
 */
#define	FSTAB		"/etc/fstab"

#define	FSTAB_RW	"rw"	/* read/write device */
#define	FSTAB_RQ	"rq"	/* read/write with quotas */
#define	FSTAB_RO	"ro"	/* read-only device */
#define	FSTAB_SW	"sw"	/* swap device */
#define	FSTAB_XX	"xx"	/* ignore totally */

struct	fstab{
	char	*fs_spec;		/* block special device name */
	char	*fs_file;		/* file system path prefix */
	char	*fs_type;		/* FSTAB_* */
	int	fs_freq;		/* dump frequency, in days */
	int	fs_passno;		/* pass number on parallel dump */
#if CMUCS
	char	*fs_name;		/* file system name (ufs,nfs,ods) */
	char	*fs_opts;		/* options for funny file systems */
#endif /* CMUCS */
};

#if defined(CMU) && defined(__STDC__)
extern struct fstab *getfsent(void);
extern struct fstab *getfsspec(const char*);
extern struct fstab *getfsfile(const char*);
extern struct fstab *getfstype(const char*);
extern int setfsent(void);
extern int endfsent(void);
#else
struct	fstab *getfsent();
struct	fstab *getfsspec();
struct	fstab *getfsfile();
struct	fstab *getfstype();
int	setfsent();
int	endfsent();
#endif
