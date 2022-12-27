/* 
 * Mach Operating System
 * Copyright (c) 1991 Carnegie-Mellon University
 * Copyright (c) 1990 Carnegie-Mellon University
 * Copyright (c) 1989 Carnegie-Mellon University
 * Copyright (c) 1988 Carnegie-Mellon University
 * Copyright (c) 1987 Carnegie-Mellon University
 * All rights reserved.  The CMU software License Agreement specifies
 * the terms and conditions for use and redistribution.
 */
/* 
 * HISTORY
 * $Log:	user.h,v $
 * Revision 2.12  91/03/20  15:04:32  dbg
 * 	Find 'errno.h' in 'sys' directory.
 * 	[91/02/21            dbg]
 * 
 * Revision 2.11  90/10/25  15:06:56  rwd
 * 	Add STACK_GROWTH_UP case for u.
 * 	[90/10/11            rwd]
 * 
 * Revision 2.10  90/08/06  15:33:50  rwd
 * 	Added uu_share_lock_count for share_lock sanity check.
 * 	[90/07/06            rwd]
 * 
 * Revision 2.9  90/06/19  23:13:59  rpd
 * 	Added uu_proc_exit to uthread.
 * 	[90/06/05            rpd]
 * 
 * Revision 2.8  90/06/02  15:26:07  rpd
 * 	Added uu_xxx field to uthread structure, for debugging.
 * 	[90/05/23            rpd]
 * 
 * 	Removed uu_text_start, uu_stack from utask.
 * 	[90/05/13            rpd]
 * 
 * 	Removed uu_user_thread, uu_arg.
 * 	[90/05/13            rpd]
 * 	Cleaned up conditionals; removed MACH, CMU, CMUCS, MACH_NO_KERNEL.
 * 	[90/04/28            rpd]
 * 
 * 	Revised uu_reply_msg and friends.
 * 	[90/04/27            rpd]
 * 	Converted to new IPC.
 * 	[90/03/26  20:04:37  rpd]
 * 
 * Revision 2.7  90/05/29  20:24:37  rwd
 * 	Added uu_save_master_count for use by mfs_get/put routines.
 * 	[90/05/16            rwd]
 * 	Added uu_fmap and uu_findx to keep track of mapping areas.
 * 	[90/04/10            rwd]
 * 
 * Revision 2.6  90/05/21  13:59:33  dbg
 * 	Always include uu_sigtramp.
 * 	[90/04/23            dbg]
 * 
 * Revision 2.5  90/03/14  21:29:47  rwd
 * 	More fields to shared memory.
 * 	[90/02/22            rwd]
 * 	Started moving fields into shared memory area
 * 	[90/01/23            rwd]
 * 
 * Revision 2.4  90/01/19  14:37:57  rwd
 * 	Change u macro
 * 	[89/12/14            rwd]
 * 
 * Revision 2.3  89/12/08  20:14:13  rwd
 * 	Removed uu_timeout.
 * 	[89/11/29            rwd]
 * 	Removed uu_wait_port.  Added uu_lock, uu_condition and uu_timeout
 * 	for blocking and sleeping so cthreads can free up kernel thread.
 * 	[89/10/30            rwd]
 * 
 * 	Added UU_PENDING to states; documented.
 * 	[89/04/20            dbg]
 * 
 * 	Add extra chain fields to link server threads to processes.
 * 	[89/04/06            dbg]
 * 
 * 	Out-of-kernel version.  NO CONDITIONALS!
 * 	[89/01/02            dbg]
 * 
 * Revision 2.2  89/11/29  15:30:00  af
 * 	Added UU_PENDING to states; documented.
 * 	[89/04/20            dbg]
 * 
 * 	Add extra chain fields to link server threads to processes.
 * 	[89/04/06            dbg]
 * 
 * 	Out-of-kernel version.  NO CONDITIONALS!
 * 	[89/01/02            dbg]
 * 
 * Revision 2.1  89/08/04  14:49:26  rwd
 * Created.
 * 
 * Revision 2.5  88/08/24  02:51:39  mwyoung
 * 	Condensed CS conditionals.
 * 	[88/08/22  22:29:29  mwyoung]
 * 	
 * 	Adjusted include file references.
 * 	[88/08/17  02:27:11  mwyoung]
 * 
 *
 * 16-Jun-88  Michael Young (mwyoung) at Carnegie-Mellon University
 *	Add values for stack start, end, and direction of growth, so we
 *	can handle limit changes.
 *
 * 28-Apr-88  David Golub (dbg) at Carnegie-Mellon University
 *	Move u_rpause and u_rfs from thread to task U-area - they are
 *	both global process state.
 *
 * 20-Apr-88  Mike Accetta (mja) at Carnegie-Mellon University
 *	Added URPW_* resource pause flag bit definitions (renamed and
 *	moved from <sys/fs.h>).
 *	[ V5.1(XF23) ]
 *
 * 11-Apr-88  Mike Accetta (mja) at Carnegie-Mellon University
 *	Move controlling terminal information to proc structure.
 *	[ V5.1(XF23) ]
 *
 *  7-Mar-88  David Kirschen (kirschen) at Encore Computer Corporation
 *      Optimize u-area references for the multimax
 *
 * 26-Feb-88  David Kirschen (kirschen) at Encore Computer Corporation
 *      Add include of param.h for NGROUPS, etc.
 *
 * 25-Jan-88  Robert Baron (rvb) at Carnegie-Mellon University
 *	Rather than have "u" expand to current_thread()->u_address, have
 *	it expand to the constant U_ADDRESS which is updated by load_context
 *	when the thread changes.  If "u" is defined, then user.h won't define
 *	it.
 *
 * 06-Jan-88  Jay Kistler (jjk) at Carnegie Mellon University
 *	VICE: Removed Fid typedef as it is no longer used anywhere and 
 *	it was causing name conflicts outside the kernel.
 *
 * 19-Nov-87  Avadis Tevanian (avie) at Carnegie-Mellon University
 *	Eliminated conditionals, purged history.
 *
 * 10-Oct-87  Mike Accetta (mja) at Carnegie-Mellon University
 *	Move UMODE_* bit definitions to <sys/syscall.h>.
 *	[ V5.1(XF18) ]
 *
 * 03-Feb-87  Mike Accetta (mja) at Carnegie-Mellon University
 *	CS_RFS: Add new RFS_EROOT definition.
 *	[ V5.1(F2) ]
 *
 * 27-Jan-87  Mike Accetta (mja) at Carnegie-Mellon University
 *	CS_GENERIC: Added new u_maxuprc field to allow the maximum
 *	number of processes per user ID to vary for a process tree.
 *	[ V5.1(F1) ]
 *
 * 18-Oct-85  Mike Accetta (mja) at Carnegie-Mellon University
 *	CS_RFS:  added u_rfs definition.
 *	[V1(1)]
 *
 * 07-Sep-85  Mike Accetta (mja) at Carnegie-Mellon University
 *	CS_GENERIC:  added modes field and symbol definitions.
 *	[V1(1)]
 *
 * 03-Aug-85  Mike Accetta (mja) at Carnegie-Mellon University
 *	CS_RPAUSE:  added resource pause field and symbol definitions.
 *	[V1(1)]
 *
 * 22-Jul-85  Mike Accetta (mja) at Carnegie-Mellon University
 *	CS_RFS:  added u_syscode definition.
 *	[V1(1)]
 *
 * 14-May-85  Mike Accetta (mja) at Carnegie-Mellon University
 *	Upgraded to 4.2BSD.  Carried over changes below.
 *	[V1(1)]
 *
 * 28-Jul-82  Mike Accetta (mja) at Carnegie-Mellon University
 *	Changed to include errno.h from include directory (V3.05c).
 *
 * 22-Jun-82  Mike Accetta (mja) at Carnegie-Mellon University
 *	CS_AID:  added u_aid field to record new process account ID
 *	(V3.05a).
 *
 **********************************************************************
 */
/*
 * Copyright (c) 1982, 1986 Regents of the University of California.
 * All rights reserved.  The Berkeley software License Agreement
 * specifies the terms and conditions for redistribution.
 *
 *	@(#)user.h	7.1 (Berkeley) 6/4/86
 */

#ifndef	_SYS_USER_H_
#define	_SYS_USER_H_

#ifdef	KERNEL
#include <cmucs_rfs.h>
#include <vice.h>
#include <map_uarea.h>
#else	KERNEL
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

#include <mach/boolean.h>
#ifndef	mac2 /* mac2 does not have pcb structure */
#include <machine/pcb.h>
#endif
#include <sys/namei.h>
#include <sys/param.h>
#include <sys/queue.h>
#include <sys/resource.h>
#include <sys/time.h>
#include <sys/types.h>

/*
 * Per process structure containing data that
 * isn't needed in core when the process is swapped out.
 */

#define	MAXCOMLEN	16		/* <= MAXNAMLEN, >= sizeof(ac_comm) */
 
struct	user {
#ifndef	mac2 /* mac2 does not have pcb structure */
	struct	pcb u_pcb;
#endif
	struct	proc *u_procp;		/* pointer to proc structure */
	int	*u_ar0;			/* address of users saved R0 */
	char	u_comm[MAXCOMLEN + 1];

/* syscall parameters, results and catches */
	int	u_arg[8];		/* arguments to current system call */
	int	*u_ap;			/* pointer to arglist */
	label_t	u_qsave;		/* for non-local gotos on interrupts */
	union {				/* syscall return values */
		struct	{
			int	R_val1;
			int	R_val2;
		} u_rv;
#define	r_val1	u_rv.R_val1
#define	r_val2	u_rv.R_val2
		off_t	r_off;
		time_t	r_time;
	} u_r;
	char	u_error;		/* return error code */
	char	u_eosys;		/* special action on end of syscall */

/* 1.1 - processes and protection */
	uid_t	u_uid;			/* effective user id */
	uid_t	u_ruid;			/* real user id */
	gid_t	u_gid;			/* effective group id */
	gid_t	u_rgid;			/* real group id */
	gid_t	u_groups[NGROUPS];	/* groups, 0 terminated */

/* 1.2 - memory management */
	size_t	u_tsize;		/* text size (clicks) */
	size_t	u_dsize;		/* data size (clicks) */
	size_t	u_ssize;		/* stack size (clicks) */
	caddr_t	u_text_start;		/* text starting address */
	caddr_t	u_data_start;		/* data starting address */
	caddr_t	u_stack_start;		/* stack starting address */
	caddr_t	u_stack_end;		/* stack ending address */
	int	u_stack_grows_up;	/* stack grows at high end? */
	time_t	u_outime;		/* user time at last sample */

/* 1.3 - signal management */
	int	(*u_signal[NSIG+1])();	/* disposition of signals */
	int	u_sigmask[NSIG+1];	/* signals to be blocked */
#if	defined(multimax) || defined(balance) || defined(mips)
	int	(*u_sigtramp)();	/* signal trampoline addr */
#endif	defined(multimax) || defined(balance) || defined(mips)
	int	u_sigonstack;		/* signals to take on sigstack */
	int	u_sigintr;		/* signals that interrupt syscalls */
	int	u_oldmask;		/* saved mask from before sigpause */
	int	u_code;			/* ``code'' to trap */
	struct	sigstack u_sigstack;	/* sp & on stack state variable */
#define	u_onstack	u_sigstack.ss_onstack
#define	u_sigsp		u_sigstack.ss_sp

/* 1.4 - descriptor management */
	struct	file *u_ofile[NOFILE];	/* file structures for open files */
	char	u_pofile[NOFILE];	/* per-process flags of open files */
	int	u_lastfile;		/* high-water mark of u_ofile */
#define	UF_EXCLOSE 	0x1		/* auto-close on exec */
#define	UF_MAPPED 	0x2		/* mapped from device */
	struct	inode *u_cdir;		/* current directory */
	struct	inode *u_rdir;		/* root directory of current process */
#define	u_ttyp	u_procp->p_ttyp		/* controlling tty pointer */
#define	u_ttyd	u_procp->p_ttyd		/* controlling tty dev */
	short	u_cmask;		/* mask for file creation */

/* 1.5 - timing and statistics */
	struct	rusage u_ru;		/* stats for this proc */
	struct	rusage u_cru;		/* sum of stats for reaped children */
	struct	itimerval u_timer[3];
	int	u_XXX[3];
	struct	timeval u_start;
	short	u_acflag;

	struct uprof {			/* profile arguments */
		short	*pr_base;	/* buffer base */
		unsigned pr_size;	/* buffer size */
		unsigned pr_off;	/* pc offset */
		unsigned pr_scale;	/* pc scaling */
	} u_prof;
	short	u_aid;			/* account id */
	u_short	u_maxuprc;		/* max processes per UID (per tree) */
	struct fs *u_rpsfs;		/* resource pause file system */
	char	u_rpswhich;		/* resource pause operation selection */
#define	URPW_FNOSPC	0x01		/* - low on fragments */
#define	URPW_INOSPC	0x02		/* - low on inodes */
#define	URPW_QNOSPC	0x04		/* - out of quota */
#define	URPW_POLL	0x40		/* - poll until available */
#define	URPW_NOTIFY	0x80		/* - pause in progress */
	u_char	u_rpause;		/* resource pause flags: */
#define	URPS_AGAIN	01		/* - no child processes available */
#define	URPS_NOMEM	02		/* - no memory available */
#define	URPS_NFILE	04		/* - file table overflow */
#define	URPS_NOSPC	010		/* - no space on device */
	char	u_modes;		/* process modes: */
#if	CMUCS_RFS
	char	u_rfs;			/* remote syscall state bits: */
#define	URFS_CDIR	01		/* - current directory is remote */
#define	URFS_RDIR	02		/* - root directory is remote */
#define	URFS_EROOT	04		/* - reject accesses above root */
	short	u_rfscode;		/* remote file system call number */
	short	u_rfsncnt;		/* remote file system namei() call count */
#endif	CMUCS_RFS

#if	VICE
	/* ITC remote file system support */
	short	u_rmt_code;		/* remote file system call number */
	short	u_rmt_ncnt;		/* remote file system namei() call count */
	struct  rmtWd {			/* Current directory-new file system */
	    dev_t dev;			/* Mounted file system of the cwd */
	    long fid[3];		/* Vice II file identifier for cwd */
	} u_rmtWd;
	struct  file *u_textfile;	/* file object for a text segment */
	char	u_rmt_requested;	/* For namei, iget, etc: caller
					   can deal with remote files */
	u_char u_rmt_follow1 /*:1*/;	/* Follow symbolic link if it is last
					   component of first pathname of
					   a system call */
	u_char u_rmt_follow2 /*:1*/;	/* ... of second pathname ... */
	dev_t	u_rmt_dev;		/* Venus is listening on this device */
	struct	buf *u_rmt_path_buf;	/* buffer containing the pathname
					   looked up during the namei */
	char	*u_rmt_path;		/* pointer into u_rmt_path_buf to the
					   uninterpreted part of the path */
	long	u_rmt_pag;		/* Process authentication group */
					/* Block for BOGUS namei communication */
	struct	rmt_saved_data *u_rmt_saved_data;
#endif	VICE
/* 1.6 - resource controls */
	struct	rlimit u_rlimit[RLIM_NLIMITS];
	struct	quota *u_quota;		/* user's quota structure */
	int	u_qflags;		/* per process quota flags */

/* namei & co. */
	struct nameicache {		/* last successful directory search */
		int nc_prevoffset;	/* offset at which last entry found */
		ino_t nc_inumber;	/* inum of cached directory */
		dev_t nc_dev;		/* dev of cached directory */
		time_t nc_time;		/* time stamp for cache entry */
	} u_ncache;
	struct	nameidata u_nd;

	int	u_stack[1];
};

/* u_eosys values */
#define	NORETURN	0
#define	JUSTRETURN	1
#define	RESTARTSYS	2
#define NORMALRETURN	3

/* u_error codes */
#ifdef	KERNEL
#include <sys/errno.h>
#else	KERNEL
#include <errno.h>
#endif	KERNEL

#ifdef	KERNEL
/*
 *	Per-thread U area.
 */
struct uthread {
	struct proc *uu_procp;		/* pointer to proc structure */
	boolean_t uu_proc_exit;		/* inside proc_exit? */
	queue_chain_t	uu_server_list;	/* chain server thread to process
					   it is serving */

/* syscall parameters, results and catches */
	int	*uu_ap;			/* pointer to arglist */
	int	uu_xxx[4];		/* for debugging */
#if	MAP_UAREA
	int	uu_share_lock_count;	/* for panic test */
#endif	MAP_UAREA
	label_t	uu_qsave;		/* for non-local gotos on interrupts */
	union {				/* syscall return values */
	    struct {
		int	R_val1;
		int	R_val2;
	    } u_rv;
#define	r_val1	u_rv.R_val1
#define	r_val2	u_rv.R_val2
	    off_t	r_off;
	    time_t	r_time;
	} uu_r;
	char	uu_error;		/* return error code */
	char	uu_eosys;		/* special action on end of syscall */

	mach_msg_header_t *uu_reply_msg;/* pointer to reply msg */
	vm_size_t uu_current_size;	/* size of current data area */

	struct fs *uu_rpsfs;		/* resource pause file system */
	char	uu_rpswhich;		/* resource pause operation selection */
#if	CMUCS_RFS
	short	uu_rfscode;		/* remote file system call number */
	short	uu_rfsncnt;		/* remote file system namei() call count */
#endif	CMUCS_RFS

#if	VICE
	/* ITC remote file system support */
	short	uu_rmt_code;		/* remote file system call number */
	short	uu_rmt_ncnt;		/* remote file system namei() call count */
	char	uu_rmt_requested;	/* For namei, iget, etc: caller
					   can deal with remote files */
	u_char  uu_rmt_follow1 /*:1*/;	/* Follow symbolic link if it is last
					   component of first pathname of
					   a system call */
	u_char  uu_rmt_follow2 /*:1*/;	/* ... of second pathname ... */
	struct	buf *uu_rmt_path_buf;	/* buffer containing the pathname
					   looked up during the namei */
	char	*uu_rmt_path;		/* pointer into uu_rmt_path_buf to the
					   uninterpreted part of the path */
					/* Block for BOGUS namei communication */
	struct	rmt_saved_data *uu_rmt_saved_data;
#endif	VICE
/* namei & co. */
	struct	nameidata uu_nd;

	struct selbuf *uu_sb;		/* pointer to select structure */
	int	uu_master_lock;		/* synchronization */
	int	uu_save_master_count;	/* for use by mfs_get/put */

	short	uu_ipl;			/* interrupt priority level */
	boolean_t	uu_interruptible;
	struct mutex	uu_lock;	/* for locking codntion */
	struct condition		/* for sleeping and blocking */
			uu_condition;
	int		uu_timedout;	/* did we timeout? */
	queue_chain_t	uu_sleep_link;
	caddr_t		uu_sleep_chan;
};
typedef	struct uthread *	uthread_t;

/*
 *	Per-task U area - global process state.
 */
struct utask {
	struct	proc *uu_procp;		/* pointer to proc structure */
	char	uu_comm[MAXCOMLEN + 1];

/* 1.1 - processes and protection */
#if	MAP_UAREA
#else	MAP_UAREA
	uid_t	uu_uid;			/* effective user id */
	uid_t	uu_ruid;		/* real user id */
	gid_t	uu_gid;			/* effective group id */
	gid_t	uu_rgid;		/* real group id */
	gid_t	uu_groups[NGROUPS];	/* groups, 0 terminated */
#endif	MAP_UAREA

/* 1.2 - memory management */
	size_t	uu_tsize;		/* text size (clicks) */
#if	MAP_UAREA
#else	MAP_UAREA
	size_t	uu_dsize;		/* data size (clicks) */
#endif	MAP_UAREA
	size_t	uu_ssize;		/* stack size (clicks) */
#if	MAP_UAREA
#else	MAP_UAREA
	caddr_t	uu_data_start;		/* data starting address */
#endif	MAP_UAREA
	caddr_t	uu_stack_start;		/* stack starting address */
	caddr_t	uu_stack_end;		/* stack ending address */
	boolean_t uu_stack_grows_up;	/* stack grows at high end? */
	time_t	uu_outime;		/* user time at last sample */

/* 1.3 - signal management */
#if	MAP_UAREA
#else	MAP_UAREA
	int	(*uu_signal[NSIG+1])();	/* disposition of signals */
	int	uu_sigmask[NSIG+1];	/* signals to be blocked */
	int	(*uu_sigtramp)();	/* signal trampoline/return addr */
	int	uu_sigonstack;		/* signals to take on sigstack */
	int	uu_sigintr;		/* signals that interrupt syscalls */
#endif	MAP_UAREA
	int	uu_oldmask;		/* saved mask from before sigpause */
#if	MAP_UAREA
#else	MAP_UAREA
	struct	sigstack uu_sigstack;	/* sp & on stack state variable */
#endif	MAP_UAREA

/* 1.4 - descriptor management */
	struct	file *uu_ofile[NOFILE];	/* file structures for open files */
	char	uu_pofile[NOFILE];	/* per-process flags of open files */
#if	MAP_UAREA
	int	uu_findx[NOFILE];	/* file -> map */
	int	uu_fmap[NOFILE];	/* map entry ref counts */
#else	MAP_UAREA
	int	uu_lastfile;		/* high-water mark of uu_ofile */
#endif	MAP_UAREA
#define	UF_EXCLOSE 	0x1		/* auto-close on exec */
#define	UF_MAPPED 	0x2		/* mapped from device */
	struct	inode *uu_cdir;		/* current directory */
	struct	inode *uu_rdir;		/* root directory of current process */
#if	MAP_UAREA
#else	MAP_UAREA
	short	uu_cmask;		/* mask for file creation */
#endif	MAP_UAREA

/* 1.5 - timing and statistics */
	struct	rusage uu_ru;		/* stats for this proc */
	struct	rusage uu_cru;		/* sum of stats for reaped children */
	struct	itimerval uu_timer[3];
	int	uu_XXX[3];
	struct	timeval uu_start;
	short	uu_acflag;

	struct uuprof {			/* profile arguments */
		int	pr_lock;
		short	*pr_base;	/* buffer base */
		unsigned pr_size;	/* buffer size */
		unsigned pr_off;	/* pc offset */
		unsigned pr_scale;	/* pc scaling */
	} uu_prof;
	short	uu_aid;			/* account id */
	u_short	uu_maxuprc;		/* max processes per UID (per tree) */
	u_char	uu_rpause;		/* resource pause flags: */
#if	CMUCS_RFS
	char	uu_rfs;			/* remote syscall state bits: */
#endif	CMUCS_RFS

/* 1.6 - resource controls */
#if	MAP_UAREA
#else	MAP_UAREA
	struct	rlimit uu_rlimit[RLIM_NLIMITS];
#endif	MAP_UAREA
	struct	quota *uu_quota;	/* user's quota structure */
	int	uu_qflags;		/* per process quota flags */

	struct unameicache {		/* last successful directory search */
		int nc_prevoffset;	/* offset at which last entry found */
		ino_t nc_inumber;	/* inum of cached directory */
		dev_t nc_dev;		/* dev of cached directory */
		time_t nc_time;		/* time stamp for cache entry */
	} uu_ncache;
	char	uu_modes;		/* process modes: */
#if	VICE
	struct  urmtWd {		/* Current directory-new file system */
	    dev_t dev;			/* Mounted file system of the cwd */
	    long fid[3];		/* Vice II file identifier for cwd */
	} uu_rmtWd;
	struct  file *uu_textfile;	/* file object for a text segment */
	dev_t	uu_rmt_dev;		/* Venus is listening on this device */
	long	uu_rmt_pag;		/* Process authentication group */
#endif	VICE
};

/*
 *	To get p_utask, we include sys/proc.h.  But this introduces
 *	circular includes.  Hence we must do this include after
 *	defining struct utask, which is the part of sys/user.h that
 *	sys/proc.h needs.
 */

#include <sys/proc.h>

extern int u_offset;
#ifdef	STACK_GROWTH_UP
#define	u (*(struct uthread *)((cthread_sp() & cthread_stack_mask) + u_offset))
#else	STACK_GROWTH_UP
#define u (*(struct uthread *)((cthread_sp() | cthread_stack_mask) + u_offset))
#endif	STACK_GROWTH_UP

#if	MAP_UAREA
#include <sys/ushared.h>

#define u_shared_ro	uu_procp->p_shared_ro
#define u_shared_rw	uu_procp->p_shared_rw
#define us_shared_lock(x)	spin_lock(&x->u_shared_rw->us_lock);
#define us_shared_unlock(x)	spin_unlock(&x->u_shared_rw->us_lock);
#define u_rlimit	u_shared_ro->us_rlimit
#define uu_rlimit	u_shared_ro->us_rlimit
#define	u_data_start	u_shared_ro->us_data_start
#define	uu_data_start	u_shared_ro->us_data_start
#define u_dsize		u_shared_rw->us_dsize
#define uu_dsize	u_shared_rw->us_dsize
#define u_uid		u_shared_ro->us_uid
#define uu_uid		u_shared_ro->us_uid
#define u_ruid		u_shared_ro->us_ruid
#define uu_ruid		u_shared_ro->us_ruid
#define u_gid		u_shared_ro->us_gid
#define uu_gid		u_shared_ro->us_gid
#define u_rgid		u_shared_ro->us_rgid
#define uu_rgid		u_shared_ro->us_rgid
#define u_groups	u_shared_ro->us_groups
#define uu_groups	u_shared_ro->us_groups
#define	u_cmask		u_shared_rw->us_cmask
#define	uu_cmask	u_shared_rw->us_cmask
#define u_sigstack	u_shared_rw->us_sigstack
#define uu_sigstack	u_shared_rw->us_sigstack
#define u_sigonstack	u_shared_rw->us_sigonstack
#define uu_sigonstack	u_shared_rw->us_sigonstack
#define u_sigintr	u_shared_rw->us_sigintr
#define uu_sigintr	u_shared_rw->us_sigintr
#define u_sigmask	u_shared_rw->us_usigmask
#define uu_sigmask	u_shared_rw->us_usigmask
#define u_signal	u_shared_rw->us_signal
#define uu_signal	u_shared_rw->us_signal
#define	u_sigtramp	u_shared_rw->us_sigtramp
#define	uu_sigtramp	u_shared_rw->us_sigtramp
#define u_findx		uu_procp->p_utask.uu_findx
#define u_fmap		uu_procp->p_utask.uu_fmap
#define u_lastfile	u_shared_ro->us_lastfile
#define uu_lastfile	u_shared_ro->us_lastfile
#endif	MAP_UAREA

#define u_procp		uu_procp
#define u_comm		uu_procp->p_utask.uu_comm

#define	u_ap		uu_ap
#define	u_qsave		uu_qsave
#define	u_r		uu_r
#define	u_error		uu_error
#define	u_eosys		uu_eosys

#define	u_reply_msg	uu_reply_msg
#define	u_current_output uu_current_output
#define	u_current_size	uu_current_size

#if	MAP_UAREA
#else	MAP_UAREA
#define u_uid		uu_procp->p_utask.uu_uid
#define u_ruid		uu_procp->p_utask.uu_ruid
#define u_gid		uu_procp->p_utask.uu_gid
#define u_rgid		uu_procp->p_utask.uu_rgid
#define u_groups	uu_procp->p_utask.uu_groups
#endif	MAP_UAREA

#define u_tsize		uu_procp->p_utask.uu_tsize
#if	MAP_UAREA
#else	MAP_UAREA
#define u_dsize		uu_procp->p_utask.uu_dsize
#endif	MAP_UAREA
#define u_ssize		uu_procp->p_utask.uu_ssize
#if	MAP_UAREA
#else	MAP_UAREA
#define	u_data_start	uu_procp->p_utask.uu_data_start
#endif	MAP_UAREA
#define	u_stack_start	uu_procp->p_utask.uu_stack_start
#define	u_stack_end	uu_procp->p_utask.uu_stack_end
#define	u_stack_grows_up uu_procp->p_utask.uu_stack_grows_up
#define u_outime	uu_procp->p_utask.uu_outime

#if	MAP_UAREA
#else	MAP_UAREA
#define u_signal	uu_procp->p_utask.uu_signal
#define u_sigmask	uu_procp->p_utask.uu_sigmask
#ifdef	multimax
#define	u_sigcatch	uu_procp->p_utask.uu_sigtramp
#endif	multimax
#define	u_sigtramp	uu_procp->p_utask.uu_sigtramp
#define u_sigonstack	uu_procp->p_utask.uu_sigonstack
#define u_sigintr	uu_procp->p_utask.uu_sigintr
#endif	MAP_UAREA
#define u_oldmask	uu_procp->p_utask.uu_oldmask
#if	MAP_UAREA
#else	MAP_UAREA
#define u_sigstack	uu_procp->p_utask.uu_sigstack
#endif	MAP_UAREA

#define	u_onstack	u_sigstack.ss_onstack
#define	u_sigsp		u_sigstack.ss_sp

#define u_ofile		uu_procp->p_utask.uu_ofile
#define u_pofile	uu_procp->p_utask.uu_pofile
#if	MAP_UAREA
#else	MAP_UAREA
#define u_lastfile	uu_procp->p_utask.uu_lastfile
#endif	MAP_UAREA
#define u_cdir		uu_procp->p_utask.uu_cdir
#define u_rdir		uu_procp->p_utask.uu_rdir
#undef	u_ttyp
#undef	u_ttyd
#define u_ttyp		uu_procp->p_ttyp
#define u_ttyd		uu_procp->p_ttyd
#if	MAP_UAREA
#else	MAP_UAREA
#define u_cmask		uu_procp->p_utask.uu_cmask
#endif	MAP_UAREA

#define u_ru		uu_procp->p_utask.uu_ru
#define u_cru		uu_procp->p_utask.uu_cru
#define u_timer		uu_procp->p_utask.uu_timer
#define u_XXX		uu_procp->p_utask.uu_XXX
#define u_start		uu_procp->p_utask.uu_start
#define u_acflag	uu_procp->p_utask.uu_acflag

#define u_prof		uu_procp->p_utask.uu_prof
#define u_aid		uu_procp->p_utask.uu_aid
#define u_maxuprc	uu_procp->p_utask.uu_maxuprc
#define	u_rpsfs		uu_rpsfs
#define	u_rpswhich	uu_rpswhich
#define u_rpause	uu_procp->p_utask.uu_rpause
#define u_modes		uu_procp->p_utask.uu_modes
#if	CMUCS_RFS
#define u_rfs		uu_procp->p_utask.uu_rfs
#define u_rfscode	uu_rfscode
#define u_rfsncnt	uu_rfsncnt
#endif	CMUCS_RFS
#if	VICE
#define	u_rmt_code	uu_rmt_code
#define	u_rmt_ncnt	uu_rmt_ncnt
#define u_rmtWd		uu_procp->p_utask.uu_rmtWd
#define u_textfile	uu_procp->p_utask.uu_textfile
#define	u_rmt_requested	uu_rmt_requested
#define	u_rmt_follow1	uu_rmt_follow1
#define	u_rmt_follow2	uu_rmt_follow2
#define u_rmt_dev	uu_procp->p_utask.uu_rmt_dev
#define	u_rmt_path_buf	uu_rmt_path_buf
#define	u_rmt_path	uu_rmt_path
#define u_rmt_pag	uu_procp->p_utask.uu_rmt_pag
#define	u_rmt_saved_data uu_rmt_saved_data
#endif	VICE

#if	MAP_UAREA
#else	MAP_UAREA
#define u_rlimit	uu_procp->p_utask.uu_rlimit
#endif	MAP_UAREA
#define u_quota		uu_procp->p_utask.uu_quota
#define u_qflags	uu_procp->p_utask.uu_qflags

#define u_ncache	uu_procp->p_utask.uu_ncache
#define u_nd		uu_nd

#define	u_sb		uu_sb
#define	u_master_lock	uu_master_lock

#define	u_ipl		uu_ipl
#define	u_state		uu_state
#define	u_interruptible	uu_interruptible
#define	u_wait_port	uu_wait_port
#define	u_sleep_link	uu_sleep_link
#define	u_sleep_chan	uu_sleep_chan

#endif	KERNEL
#endif	_SYS_USER_H_
