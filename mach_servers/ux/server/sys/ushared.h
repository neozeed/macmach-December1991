/* 
 * Mach Operating System
 * Copyright (c) 1990 Carnegie-Mellon University
 * All rights reserved.  The CMU software License Agreement specifies
 * the terms and conditions for use and redistribution.
 */
/* 
 * HISTORY
 * $Log:	ushared.h,v $
 * Revision 2.7  90/11/05  16:56:50  rpd
 * 	Added spin_lock_init.
 * 	[90/10/31            rwd]
 * 
 * Revision 2.6  90/08/06  15:34:03  rwd
 * 	Remove share_lock and share_unlock macros.  These are now
 * 	routines.
 * 	[90/06/08            rwd]
 * 
 * Revision 2.5  90/06/02  15:26:15  rpd
 * 	Added pointer to proc structure as first field of readonly area
 * 	for ease in debugging.
 * 	[90/05/23            rpd]
 * 
 * 	Added us_flag to ushared_ro, to make p_flag shared.
 * 	[90/05/13            rpd]
 * 
 * 	Removed us_sigcheck, us_sigchecklock.
 * 	[90/05/11            rpd]
 * 	Updated for new IPC.
 * 	[90/05/03            rpd]
 * 
 * Revision 2.4  90/05/29  20:24:40  rwd
 * 	Added pointer to proc structure as first field of readonly area
 * 	for ease in debugging.
 * 	[90/05/16            rwd]
 * 	Moved file_info structure to its own file and include it here.
 * 	Moved shared_lock structure to its own file and include it here.
 * 	[90/04/30            rwd]
 * 	Added us_closefile field.
 * 	[90/04/23            rwd]
 * 	Added file info.
 * 	[90/03/16            rwd]
 * 
 * Revision 2.3  90/05/21  13:59:51  dbg
 * 	Always include sigtramp.
 * 	[90/04/23            dbg]
 * 
 * Revision 2.2  90/03/14  21:29:59  rwd
 * 	Added shared memory locks.  Added more sig fields.
 * 	[90/02/15            rwd]
 * 	Created.
 * 	[90/01/23            rwd]
 * 
 */

#ifndef	_SYS_USHARED_H_
#define	_SYS_USHARED_H_

#ifdef	KERNEL
#include <uxkern/import_mach.h>
#else	KERNEL
#include <cthreads.h>
#endif	KERNEL
#include <sys/param.h>
#include <sys/resource.h>
#include <sys/signal.h>
#include <sys/file.h>
#include <sys/shared_lock.h>
#include <sys/file_info.h>

#define USHARED_VERSION 2

#define KERNEL_USER 0x80000000

#define share_lock_init(x)\
    do {\
      spin_lock_init(&(x)->lock);\
      (x)->who = 0;\
      (x)->need_wakeup = 0;\
    } while (0)

struct ushared_ro {
	int		us_proc_pointer;
	int		us_version;
	int		us_mach_nbc;
	int		us_nofile;		/* NOFILE */
/* user fields */
	struct		rlimit us_rlimit[RLIM_NLIMITS];
						/* uu_rlimit */
	caddr_t		us_data_start;		/* uu_data_start */
	uid_t		us_uid;			/* uu_uid */
	uid_t		us_ruid;		/* uu_ruid */
	gid_t		us_gid;			/* uu_gid */
	gid_t		us_rgid;		/* uu_rgid */
	gid_t		us_groups[NGROUPS];	/* uu_groups */
	int		us_lastfile;		/* uu_lastfile */
/* proc fields */
	int		us_flag;		/* p_flag */
	short		us_pid;			/* p_pid */
	short		us_ppid;		/* p_ppid */
	short		us_cursig;		/* p_cursig */
};

struct ushared_rw {
	int		us_inuse;
	int		us_map_files;
	int		us_debug;
	shared_lock_t	us_lock;
/* file fields */
	struct file_info us_file_info[NOFILE];
	int		us_closefile;
/* user fields */
	size_t		us_dsize;		/* uu_dsize */
	short		us_cmask;		/* uu_cmask */
	struct		sigstack us_sigstack;	/* uu_sigstack */
	int		(*us_signal[NSIG+1])();	/* uu_signal */
	int		us_usigmask[NSIG+1];	/* uu_sigmask */
	int		us_sigonstack;		/* uu_sigonstack */
	int		us_sigintr;		/* uu_sigintr */
	int		(*us_sigtramp)();	/* uu_sigtramp */
/* proc fields */
	shared_lock_t	us_siglock;		/* p_siglock */
	int		us_sigmask;		/* p_sigmask */
	int		us_sig;			/* p_sig */
	int		us_sigignore;		/* p_sigignore */
	int		us_sigcatch;		/* p_sigcatch */
};

#endif	_SYS_USHARED_H_
