/* 
 * Mach Operating System
 * Copyright (c) 1987 Carnegie-Mellon University
 * All rights reserved.  The CMU software License Agreement specifies
 * the terms and conditions for use and redistribution.
 */
/*
 **********************************************************************
 * HISTORY
 * $Log:	proc.h,v $
 * Revision 2.7  90/09/09  14:13:26  rpd
 * 	Remove shared port and convert to sinlge object with offset.
 * 	[90/08/24            rwd]
 * 
 * Revision 2.6  90/08/06  15:33:32  rwd
 * 	Changed proc_siglock to call share_lock correctly.
 * 	[90/06/26            rwd]
 * 
 * Revision 2.5  90/06/19  23:13:40  rpd
 * 	Added p_sigref.
 * 	[90/06/12            rpd]
 * 
 * Revision 2.4  90/06/02  15:25:37  rpd
 * 	Moved p_flag into p_shared_ro to emulator can look at it.
 * 	Moved stat, flag codes outside the KERNEL conditional.
 * 	[90/05/13            rpd]
 * 
 * 	Removed p_siglist, p_siglink, p_on_siglist.
 * 	Moved ushared fields closer to the front.
 * 	[90/05/11            rpd]
 * 	More updates for new IPC.
 * 	[90/05/03            rpd]
 * 	Cleaned up conditionals; removed MACH, CMU, CMUCS, MACH_NO_KERNEL.
 * 	[90/04/28            rpd]
 * 	Converted to new IPC.
 * 
 * 	add proc_exec for sun debugger
 * 	[89/06/12            rwd]
 * 
 * 	Rearranged fields, and added lots more.
 * 	[89/04/06            dbg]
 * 
 * 	Out-of-kernel version.  NO CONDITIONALS!
 * 	[89/01/03            dbg]
 * 
 * Revision 2.3  90/05/29  20:24:32  rwd
 * 	Added link for inode "IT" queue.
 * 	[90/03/19            rwd]
 * 
 * Revision 2.2  90/03/14  21:29:12  rwd
 * 	More fields to shared memory.
 * 	[90/02/22            rwd]
 * 	Added fields for share memory region with user task
 * 	[90/01/22            rwd]
 * 
 * 	add proc_exec for sun debugger
 * 	[89/06/12            rwd]
 * 
 * 	Rearranged fields, and added lots more.
 * 	[89/04/06            dbg]
 * 
 * 	Out-of-kernel version.  NO CONDITIONALS!
 * 	[89/01/03            dbg]
 * 
 * Revision 2.1  89/08/04  14:46:06  rwd
 * Created.
 * 
 * Revision 2.4  88/08/24  02:39:19  mwyoung
 * 	Adjusted include file references.
 * 	[88/08/17  02:20:09  mwyoung]
 * 
 *
 *  4-May-88  David Black (dlb) at Carnegie-Mellon University
 *	Document use of p_stat for MACH.
 *
 * 29-Mar-88  Michael Young (mwyoung) at Carnegie-Mellon University
 *	MACH: Removed unused variables (whichqs).
 *
 * 11-Apr-88  Mike Accetta (mja) at Carnegie-Mellon University
 *	Move controlling terminal information to proc structure from
 *	U-area (to provide better handle on disconnecting background
 *	processes from a terminal);  CS_SECURITY => CMUCS.
 *	[ V5.1(XF23) ]
 *
 * 29-Dec-87  David Golub (dbg) at Carnegie-Mellon University
 *	Delinted.
 *
 * 26-Dec-87  David Golub (dbg) at Carnegie-Mellon University
 *	Removed MACH_NOFLOAT.
 *
 * 21-Dec-87  David Golub (dbg) at Carnegie-Mellon University
 *	Check for thread termination condition and return properly
 *	in more places in sig_lock.
 *
 *  9-Dec-87  David Golub (dbg) at Carnegie-Mellon University
 *	Changed forced_exit case of sig_lock to call thread_halt_self
 *	for new thread termination logic.
 *
 *  3-Dec-87  David Black (dlb) at Carnegie-Mellon University
 *	Added second argument to task_dowait.
 *
 * 18-Nov-87  Avadis Tevanian (avie) at Carnegie-Mellon University
 *	Eliminated MACH conditionals.
 *
 * 28-Oct-87  David Golub (dbg) at Carnegie-Mellon University
 *	MACH_TT: restore definition of SWEXIT to keep ps happy.
 *
 * 16-Oct-87  David Black (dlb) at Carnegie-Mellon University
 *	MACH_TT: Incorporate exit_thread logic in sig_lock() macro.
 *		This replaces and extends core_thread.
 *
 * 25-Sep-87  David Black (dlb) at Carnegie-Mellon University
 *	MACH: added core_thread field to deal with network core dumps.
 *
 * 18-Sep-87  Richard Sanzi (sanzi) at Carnegie-Mellon University
 *	Deleted definition of SOWEFPA, as this condition is no longer
 *	associated with processes, but rather with threads.
 *
 *  4-Sep-87  David Black (dlb) at Carnegie-Mellon University
 *	Added sig lock for signals and exit.  This frees proc lock for
 *	other uses.
 *
 * 15-May-87  David Black (dlb) at Carnegie-Mellon University
 *	MACH: Added p_stopsig field to record signal that stopped
 *	process.  Can't use p_cursig for this purpose under MACH.
 *
 * 30-Mar-87  Avadis Tevanian (avie) at Carnegie-Mellon University
 *	Added a lock to the proc structure to synchronize Unix things in
 *	a multiple thread environement.  This is not conditional on
 *	MACH (but on MACH) so that the same version of ps and friends
 *	will work on both kernels.
 *
 * 06-Mar-87  Mike Accetta (mja) at Carnegie-Mellon University
 *	Changed to use shorts instead of ints for these values that
 *	are really signed chars anyway since its more space efficient
 *	and consistent with the prior fix to p_nice.
 *	[ V5.1(F5) ]
 *
 * 05-Mar-87  Avadis Tevanian (avie) at Carnegie-Mellon University
 *	Make usrpri, pri and nice ints for ROMP due to compiler
 *	difference (this doesn't matter under MACH, but till we run
 *	that everywhere...)
 *
 * 04-Mar-87  Jonathan J. Chew (jjc) at Carnegie-Mellon University
 *	Added pointer to proc structure of tracer for Sun.
 *
 * 02-Mar-87  Mike Accetta (mja) at Carnegie-Mellon University
 *	Fixed to make the p_nice field a short on the IBM-RT since its
 *	current compiler doesn't provide signed char types and this is
 *	wreaking havoc with high priority processes never getting any
 *	cycles!  This fix is only temporary until a better compiler
 *	becomes the standard.
 *	[ V5.1(F4) ]
 *
 *  7-Feb-87  Avadis Tevanian (avie) at Carnegie-Mellon University
 *	Merge VICE changes -- include vice.h and change to #if VICE.
 *
 * 31-Jan-87  Avadis Tevanian (avie) at Carnegie-Mellon University
 *	Eliminate p_wchan for MACH as a check for elimination of all
 *	uses of it.
 *
 * 28-Jan-87  Mike Accetta (mja) at Carnegie-Mellon University
 *	CMUCS:  added p_logdev field to record controlling device
 *	of top-level logged-in process.
 *	[ V5.1(F1) ]
 *
 * 08-Jan-87  Robert Beck (beck) at Sequent Computer Systems, Inc.
 *	If MACH, declare p_pctcpu as a long and define PCTCPU_SCALE.
 *
 * 31-Dec-86  David Golub (dbg) at Carnegie-Mellon University
 *	Purged all MACH uses of p0br and friends.  Removed fields
 *	that refer to text structure, and removed segment size fields
 *	(p_tsize, p_dsize, p_ssize) that are unused under MACH.
 *	ROMP_FPA should be the next to go (it belongs with the thread info).
 *
 *  2-Dec-86  Jay Kistler (jjk) at Carnegie-Mellon University
 *	VICE: 1/ added p_rmt_seq field to "proc" struct;
 *	      2/ added SRMT flag;
 *
 * 31-Oct-86  Avadis Tevanian (avie) at Carnegie-Mellon University
 *	Removed include of task/thread header files by using "struct"
 *	instead of typedef.
 *
 * 15-Oct-86  Avadis Tevanian (avie) at Carnegie-Mellon University
 *	Well, as it turns out, the Multimax code actually does want the
 *	Vax versions of p0br and friends for compatibility.  Presumably
 *	these will all go away someday anyway.
 *
 * 14-Oct-86  William Bolosky (bolosky) at Carnegie-Mellon University
 *	Changed #ifdef romp #else romp {vax code here} #endif romp to
 *	the (correct) #ifdef vax {vax code here} #endif vax.  This
 *	should NOT be changed back again; we now have more machines than
 *	just the vax and the RT, and I don't think that the Sun and the
 *	Encore want definitions of p_p0br.  It's time to fix the scripts.
 *
 * 30-Sep-86  Avadis Tevanian (avie) at Carnegie-Mellon University
 *	Added backpointers from proc to task and thread.
 *
 * 24-Sep-86  Michael Young (mwyoung) at Carnegie-Mellon University
 *	Added include of <sys/types.h> to pick up uid_t, etc.
 *
 *  6-Sep-86  Michael Young (mwyoung) at Carnegie-Mellon University
 *	Added include of <sys/time.h> for non-KERNEL compiles.
 *
 * 20-Jul-86  Michael Young (mwyoung) at Carnegie-Mellon University
 *	Added include of "time.h" to satisfy "struct itimerval"
 *	reference.
 *
 *  7-Jul-86  Bill Bolosky (bolosky) at Carnegie-Mellon University
 *	romp: removed p_sid0 and p_sid1 (since this info is stored in
 *	the pmap and is no longer used).  Conditionalized
 *	SPTECHG on vax and added SOWEFPA in same bit on romp w/FPA.
 *	Conditionalized p0br and p1br on vax.
 *
 * 25-Jan-86  Avadis Tevanian (avie) at Carnegie-Mellon University
 *	Upgraded to 4.3.
 *
 * 18-Feb-86  Bill Bolosky (bolosky) at Carnegie-Mellon University
 *	Added definition of p_sid0 and p_sid1 in proc structure for
 *	IBM-RT under switch romp.
 *
 *  3-Sep-85  Avadis Tevanian (avie) at Carnegie-Mellon University
 *	MACH:  Added SACTIVE flag to signify that a process is actually
 *	running on a cpu.
 *
 * 25-Aug-85  Avadis Tevanian (avie) at Carnegie-Mellon University
 *	Allow recursive includes.
 *
 * 25-May-85  Mike Accetta (mja) at Carnegie-Mellon University
 *	Upgraded from 4.1BSD.  Carried over changes below.
 *	[V1(1)]
 *
 * 20-Aug-81  Mike Accetta (mja) at Carnegie-Mellon University
 *	CMUCS:  added SXONLY bit definition to flag execute only
 *	processes;
 *
 **********************************************************************
 */
/*
 * Copyright (c) 1982, 1986 Regents of the University of California.
 * All rights reserved.  The Berkeley software License Agreement
 * specifies the terms and conditions for redistribution.
 *
 *	@(#)proc.h	7.1 (Berkeley) 6/4/86
 */

#ifndef	_SYS_PROC_H_
#define	_SYS_PROC_H_

#ifdef	KERNEL
#include <vice.h>
#include <map_uarea.h>

#include <uxkern/import_mach.h>
#include <sys/time.h>
#include <sys/types.h>
#include <sys/user.h>
#if	MAP_UAREA
#include <sys/ushared.h>
#endif	MAP_UAREA

/*
 * One structure allocated per active
 * process.
 */
struct	proc {
	struct proc *	p_link;		/* task-to-proc hash link */
	struct proc *	p_rlink;
	struct proc *	p_nxt;		/* list of allocated proc slots */
	struct proc **	p_prev;		/* also zombies, and free procs */

	struct mutex	p_lock;		/* lock for ref count */
	int		p_ref_count;	/* reference count */
	queue_head_t	p_servers;	/* list of server threads active */
	int		p_sigref;	/* signal retries in progress */

#if	MAP_UAREA
#define p_flag		p_shared_ro->us_flag
#else	MAP_UAREA
	int		p_flag;		/* the usual random flags */
#endif	MAP_UAREA
	short		p_stat;		/* Process state */
	short		p_nice;		/* nice for cpu usage */

	task_t		p_task;		/* corresponding task */
	thread_t	p_thread;	/* corresponding thread */

#if	MAP_UAREA
	vm_offset_t	p_shared_off;	/* offset in shared object */
	struct ushared_rw
			*p_shared_rw;	/* shared memory */
	struct ushared_ro
			*p_shared_ro;	/* shared memory */
	char *		p_readwrite;	/* shared read/write buffer */

	struct proc *	p_mulink;	/* for linking inode "IT" queue */
#define	p_siglock	p_shared_rw->us_siglock
#define	p_sigmask	p_shared_rw->us_sigmask
#define p_sigignore	p_shared_rw->us_sigignore
#define p_sigcatch	p_shared_rw->us_sigcatch
#define p_sig		p_shared_rw->us_sig
#else	MAP_UAREA
	struct mutex	p_siglock;	/* signal lock */
	int		p_sigmask;	/* current signal mask */
	int		p_sigignore;	/* signals being ignored */
	int		p_sigcatch;	/* signals being caught by user */
	int		p_sig;		/* signals pending to this process */
#endif	MAP_UAREA
#if	MAP_UAREA
#define	p_cursig	p_shared_ro->us_cursig
#else	MAP_UAREA
	short		p_cursig;	/* signal pending for take_signal() */
#endif	MAP_UAREA
	short		p_stopsig;	/* signal number at stop */
	int		p_stopcode;	/* signal subcode at stop */
	mach_port_t	p_stop_clear_port;
					/* to resume stopped thread */

	uid_t		p_uid;		/* user id,
					   used to direct tty signals */
	short		p_pgrp;		/* name of process group leader */
#if	MAP_UAREA
#define p_pid	p_shared_ro->us_pid
#define p_ppid	p_shared_ro->us_ppid
#else	MAP_UAREA
	short		p_pid;		/* unique process id */
	short		p_ppid;		/* process id of parent */
#endif	MAP_UAREA
	struct rusage *	p_ru;		/* mbuf holding exit information */
	u_short		p_xstat;	/* Exit status for wait */
	short		p_idhash;	/* hashed based on p_pid for
					   kill+exit+... */
	struct proc *	p_pptr;		/* pointer to parent process */
	struct proc *	p_cptr;		/* pointer to youngest living child */
	struct proc *	p_osptr;	/* pointer to older sibling processes */
	struct proc *	p_ysptr;	/* pointer to younger siblings */
	struct proc *	p_tptr;		/* pointer to tracing process */

	struct itimerval p_realtimer;	/* real-time interval timer */
	struct quota *	p_quota;	/* quotas for this process */
	dev_t	    	p_logdev;	/* logged-in controlling device */
	dev_t       	p_ttyd;		/* controlling tty dev */
	struct tty *	p_ttyp;		/* controlling tty pointer */
#if	VICE
	int		p_rmt_seq;	/* This process is waiting
					   for a remote file system
					   reply message containing this
					   sequence number */
#endif	VICE

	struct utask	p_utask;	/* task U-area */
#ifdef sun
	struct exec	p_exec;	/* for sun debugger */
#endif sun
};

#if	MAP_UAREA
#define	proc_siglock(p)		share_lock(&(p)->p_siglock, p)
#define proc_sigunlock(p)	share_unlock(&(p)->p_siglock, p)
#else	MAP_UAREA
#define	proc_siglock(p)		mutex_lock(&(p)->p_siglock)
#define proc_sigunlock(p)	mutex_unlock(&(p)->p_siglock)
#endif	MAP_UAREA

#define	PIDHSZ		64
#define	PIDHASH(pid)	((pid) & (PIDHSZ - 1))

short	pidhash[PIDHSZ];
struct	proc *pfind();
struct	proc *proc, *procNPROC;	/* the proc table itself */
struct	proc *freeproc, *zombproc, *allproc;
			/* lists of procs in various states */
int	nproc;
#endif	KERNEL

/* stat codes */
#define	SRUN	3		/* running */
#define	SIDL	4		/* intermediate state in process creation */
#define	SZOMB	5		/* intermediate state in process termination */
#define	SSTOP	6		/* process being traced */

/* flag codes */
#define	SLOAD	0x0000001	/* in core */
#define	SSYS	0x0000002	/* swapper or pager process */
#define	STRC	0x0000010	/* process is being traced */
#define	SWTED	0x0000020	/* another tracing flag */
#define	SOMASK	0x0000200	/* restore old mask after taking signal */
#define	SWEXIT	0x0000400	/* working on exiting */
#define	SVFORK	0x0001000	/* process resulted from vfork() */
#define	SPAGI	0x0008000	/* init data space on demand, from inode */
#define	SOUSIG	0x0100000	/* using old signal mechanism */
#define	SXONLY	0x2000000	/* process image read protected	*/
#define SRMT	0x10000000	/* remote file system access--don't stop job */

#endif	_SYS_PROC_H_
