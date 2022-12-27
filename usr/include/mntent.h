/* @(#)mntent.h	1.3 87/01/05 NFSSRC */
/*	@(#)mntent.h 1.1 86/09/24 SMI	*/
/******************************************************
 * HISTORY
 * $Log:	mntent.h,v $
 * Revision 2.1  89/07/19  09:11:58  dimitris
 * 	Changed to accept Ultrix type format for /etc/fstab.
 * 	[89/07/19  09:05:31  dimitris]
 * 
 * Revision 2.0  89/06/15  15:38:15  dimitris
 *   Organized into a misc collection and SSPized
 * 
 * Revision 2.1.1.1  89/06/15  14:14:13  dimitris
 * 	Organized into a misc collection
 * 
 *****************************************************/
/*
 * File system table, see mntent (5)
 *
 * Used by dump, mount, umount, swapon, fsck, df, ...
 *
 * Quota files are always named "quotas", so if type is "rq",
 * then use concatenation of mnt_dir and "quotas" to locate
 * quota file.
 */

#define	MNTTAB		"/etc/fstab"
#define	MOUNTED		"/etc/mtab"

#define	MNTMAXSTR	128
#if CMUCS
#define	MNTTYPE_42	"ufs"	/* unix file system */
#else /* CMUCS */
#define	MNTTYPE_42	"4.2"	/* 4.2 file system */
#endif /* CMUCS */
#define	MNTTYPE_NFS	"nfs"	/* network file system */
#define	MNTTYPE_PC	"pc"	/* IBM PC (MSDOS) file system */

#if CMUCS
#define MNTOPT_SWAP	"sw"
#define MNTOPT_IGNORE   "xx"
#else /* CMUCS */
#define	MNTTYPE_SWAP	"swap"	/* swap file system */
#define	MNTTYPE_IGNORE	"ignore"/* No type specified, ignore this entry */
#endif /* CMUCS */

#define	MNTOPT_RO	"ro"	/* read only */
#define	MNTOPT_RW	"rw"	/* read/write */

#if CMUCS
#define	MNTOPT_QUOTA	"rq"	/* quotas */
#else /* CMUCS */
#define	MNTOPT_QUOTA	"quota"	/* quotas */
#endif /* CMUCS */

#define	MNTOPT_NOQUOTA	"noquota"	/* no quotas */
#define	MNTOPT_NOAUTO	"noauto"	/* hide entry from mount -a */
#define	MNTOPT_SOFT	"soft"	/* soft mount */
#define	MNTOPT_HARD	"hard"	/* hard mount */
#define	MNTOPT_NOSUID	"nosuid"/* no set uid allowed */
#define	MNTOPT_INTR	"intr"	/* allow interrupts on hard mount */


struct	mntent{
	char	*mnt_fsname;		/* name of mounted file system */
	char	*mnt_dir;		/* file system path prefix */
	char	*mnt_type;		/* MNTTYPE_* */
	char	*mnt_opts;		/* MNTOPT* */
#if CMUCS
	char	*mnt_opts2;		/* More MNTOPT* */
#endif /* CMUCS */
	int	mnt_freq;		/* dump frequency, in days */
	int	mnt_passno;		/* pass number on parallel fsck */
};

struct	mntent *getmntent();
char	*hasmntopt();
FILE	*setmntent();
int	endmntent();
