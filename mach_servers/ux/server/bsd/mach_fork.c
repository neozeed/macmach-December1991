/* 
 * Mach Operating System
 * Copyright (c) 1987 Carnegie-Mellon University
 * All rights reserved.  The CMU software License Agreement specifies
 * the terms and conditions for use and redistribution.
 */
/*
 * HISTORY
 * $Log:	mach_fork.c,v $
 * Revision 2.8  90/09/09  14:12:46  rpd
 * 	Convert to use new global shared object for MAP_UAREA.
 * 	[90/08/24            rwd]
 * 
 * Revision 2.7  90/08/06  15:31:33  rwd
 * 	Init file_info shared locks on fork.
 * 	[90/07/12            rwd]
 * 
 * Revision 2.6  90/06/19  23:07:43  rpd
 * 	Initialize p_sigref in newproc.
 * 	[90/06/12            rpd]
 * 
 * Revision 2.5  90/06/02  15:21:54  rpd
 * 	Fixed shared memory mapping to avoid execute permission.
 * 	[90/05/31            rpd]
 * 	Initialize us_proc_pointer.
 * 	[90/05/23            rpd]
 * 
 * 	Removed us_sigcheck, us_sigchecklock.
 * 	[90/05/11            rpd]
 * 
 * 	Removed p_siglist, p_on_siglist.
 * 	[90/05/11            rpd]
 * 
 * 	In newproc, check that the new process has reference count zero.
 * 	[90/05/10            rpd]
 * 	More updates for new IPC.
 * 	[90/05/03            rpd]
 * 	Converted to new IPC.
 * 	Deallocate unwanted task ports.
 * 	[90/03/26  19:37:22  rpd]
 * 
 * Revision 2.4  90/05/29  20:23:12  rwd
 * 	Set us_proc_port in shared area on fork.
 * 	[90/05/16            rwd]
 * 	Do not turn on debugging for emulator by default.
 * 	[90/05/02            rwd]
 * 	Initialize us_closefile at fork time.
 * 	[90/04/23            rwd]
 * 	Change mu_lock refernces to new routines.
 * 	[90/04/18            rwd]
 * 	Set us_mach_nbc flag when MACH_NBC.  Update ip mapped
 * 	reference count.
 * 	[90/03/22            rwd]
 * 
 * Revision 2.3  90/03/14  21:26:02  rwd
 * 	Change shared locks to use share_lock macros.
 * 	[90/02/16            rwd]
 * 	Add map_uarea code
 * 	[90/01/22            rwd]
 * 
 * Revision 2.2  89/10/17  11:25:31  rwd
 * 	Add interrupt return parameter to bsd_fork().  Use
 * 	standard system call macros.
 * 	[89/09/21            dbg]
 * 
 * 	Convert fork() to MiG interface.
 * 	[89/04/07            dbg]
 * 
 * 	Initialize more proc structure fields.
 * 	[89/04/06            dbg]
 * 
 * 	Out-of-kernel version - NO CONDITIONALS!
 * 	[89/01/19            dbg]
 * 
 * Revision 2.1  89/08/04  14:05:27  rwd
 * Created.
 * 
 * Revision 2.4  88/08/24  01:19:51  mwyoung
 * 	Corrected include file references.
 * 	[88/08/22            mwyoung]
 * 
 *  4-May-88  David Black (dlb) at Carnegie-Mellon University
 *	Set p_stat to SRUN before resuming child.
 *
 * 29-Mar-88  Michael Young (mwyoung) at Carnegie-Mellon University
 *	Remove references to multprog.
 *
 * 19-Apr-88  Mike Accetta (mja) at Carnegie-Mellon University
 *	Copy controlling tty fields to new proc structures in
 *	newproc().
 *	[ V5.1(XF23) ]
 *
 * 30-Dec-87  David Golub (dbg) at Carnegie-Mellon University
 *	Delinted.
 *
 * 15-Dec-87  Richard Sanzi (sanzi) at Carnegie-Mellon University
 *	Deleted #ifdef romp call to float_fork().
 *
 * 21-Nov-87  Avadis Tevanian (avie) at Carnegie-Mellon University
 *	Cleaned up some conditionals.  Deleted old history.
 *
 * 28-Jan-87  Mike Accetta (mja) at Carnegie-Mellon University
 *	CMUCS:  updated newproc() to avoid propagating new
 *	p_logdev field in child process; made MAXUPRC reference the new
 *	field in the U area rather than the old global variable.
 *	[ V5.1(F1) ]
 *
 * 12-Dec-86  Mike Accetta (mja) at Carnegie-Mellon University
 *	MACH:  nothing mysterious about the reappearance of
 *	pte.h; it is only unnecessary under MACH; back one more
 *	time...
 *
 *  2-Dec-86  Jay Kistler (jjk) at Carnegie-Mellon University
 *	VICE:  added hooks for ITC/Andrew remote file system.
 *
 * 08-May-86  Mike Accetta (mja) at Carnegie-Mellon University
 *	CS_RFS: split RFS hooks into two calls: rfs_newproc() from
 *	parent context in newproc() and rfs_fork from child context in
 *	fork().
 *
 * 26-Jul-85  Mike Accetta (mja) at Carnegie-Mellon University
 *	CS_RFS:  Added hook to inform remote system call handler about
 *	a new process in case the current process is using any remote
 *	objects which must be duplicated.
 *	CMUCS:  Fixed to also replicate execute only bit across
 *	fork().
 *	[V1(1)]
 *
 * 10-May-85  Glenn Marcy (gm0w) at Carnegie-Mellon University
 *	Upgraded to 4.2BSD.  Carried over changes below [V1(1)].
 *
 * 19-Feb-82  Mike Accetta (mja) at Carnegie-Mellon University
 *	CMUCS:  Changed inode reference count modifications to use
 *	incr/decr macros to check for consistency (V3.04c).
 *
 **********************************************************************
 */
 
#include <cputypes.h>

#include <cmucs.h>
#include <cmucs_rfs.h>
#include <quota.h>
#include <vice.h>
#include <map_uarea.h>
#include <mach_nbc.h>

/*
 * Copyright (c) 1982, 1986 Regents of the University of California.
 * All rights reserved.  The Berkeley software License Agreement
 * specifies the terms and conditions for redistribution.
 *
 *	@(#)kern_fork.c	7.1 (Berkeley) 6/5/86
 */

#include <sys/param.h>
#include <sys/systm.h>
#include <sys/map.h>
#include <sys/dir.h>
#include <sys/user.h>
#include <sys/kernel.h>
#include <sys/proc.h>
#include <sys/inode.h>
#include <sys/file.h>
#include <sys/acct.h>
#include <sys/quota.h>

#include <uxkern/import_mach.h>

#include <uxkern/proc_to_task.h>
#include <sys/parallel.h>
#include <sys/syscall.h>
#include <uxkern/syscall_subr.h>

#if	MAP_UAREA
#include <machine/vmparam.h>
#endif	MAP_UAREA

#if	CMUCS
#undef	MAXUPRC
#define	MAXUPRC		u.u_maxuprc

#endif	CMUCS

int enable_emulator_debugging = 0;

struct proc *newproc();

/*
 * fork system call.
 */
fork()
{
    panic("fork - syscall");
}

vfork()
{
    panic("vfork - syscall");
}

int
bsd_fork(proc_port, interrupt, new_state, new_state_count, child_pid)
	mach_port_t	proc_port;
	boolean_t	*interrupt;
	thread_state_t	new_state;
	unsigned int	new_state_count;
	int		*child_pid;	/* OUT */
{
	register struct proc *p1, *p2;
	register a;
	mach_port_t new_req_port;

	START_SERVER_SERIAL(SYS_fork, 1)

	a = 0;
	if (u.u_uid != 0) {
		for (p1 = allproc; p1; p1 = p1->p_nxt)
			if (p1->p_uid == u.u_uid)
				a++;
		for (p1 = zombproc; p1; p1 = p1->p_nxt)
			if (p1->p_uid == u.u_uid)
				a++;
	}
	/*
	 * Disallow if
	 *  No processes at all;
	 *  not su and too many procs owned; or
	 *  not su and would take last slot.
	 */
	p2 = freeproc;
	if (p2==NULL)
		tablefull("proc");
	if (p2==NULL || (u.u_uid!=0 && (p2->p_nxt == NULL || a>MAXUPRC))) {
		u.u_error = EAGAIN;
		goto out;
	}
	p1 = u.u_procp;

	/*
	 * Create new user process, with task.
	 */
	p2 = newproc(FALSE);
	if (p2 == 0) {
	    /*
	     * No tasks or threads available.
	     */
	    u.u_error = EAGAIN;
	    goto out;
	}

	/*
	 * Clone the parent's registers, but mark it as the child
	 * process.
	 */
	if (!thread_dup(p2->p_thread,
			new_state, new_state_count,
			p1->p_pid, 1))
	{
	    u.u_error = EFAULT;
	    goto out;
	}

	/*
	 * Record its start time.
	 */
	get_time(&p2->p_utask.uu_start);
	p2->p_utask.uu_acflag = AFORK;

	/*
	 * And start it.
	 */
	(void) thread_resume(p2->p_thread);

	*child_pid = p2->p_pid;

out:;
	END_SERVER_SERIAL
}


#if	MAP_UAREA
kern_return_t
mapin_user(p)
	register struct proc *p;
{
	vm_address_t	user_addr = EMULATOR_END - vm_page_size;
	kern_return_t	result;
	extern mach_port_t	shared_memory_port;
	
	result = vm_map(p->p_task, &user_addr, vm_page_size, 0,
			0, shared_memory_port,
			p->p_shared_off + 2*vm_page_size, 0, VM_PROT_READ,
			VM_PROT_READ, VM_INHERIT_NONE);
	if (result != KERN_SUCCESS)
		return result;
	user_addr -= 2*vm_page_size;
	result = vm_map(p->p_task, &user_addr, 2*vm_page_size, 0,
			0, shared_memory_port, p->p_shared_off,
			0, VM_PROT_READ|VM_PROT_WRITE,
			VM_PROT_READ|VM_PROT_WRITE, VM_INHERIT_NONE);
	return result;
}
#endif	MAP_UAREA

/*
 * Create a new process-- the internal version of
 * sys fork.
 * It returns 1 in the new process, 0 in the old.
 */

struct proc *
newproc(is_sys_proc)
	boolean_t	is_sys_proc;
{
	register struct proc *rpp, *rip;
	register struct utask *up;
	register int n;
	register struct file *fp;
	static int pidchecked = 0;

	/*
	 * First, just locate a slot for a process
	 * and copy the useful info from this process into it.
	 * The panic "cannot happen" because fork has already
	 * checked for the existence of a slot.
	 */
	mpid++;
retry:
	if (mpid >= 30000) {
		mpid = 100;
		pidchecked = 0;
	}
	if (mpid >= pidchecked) {
		int doingzomb = 0;

		pidchecked = 30000;
		/*
		 * Scan the proc table to check whether this pid
		 * is in use.  Remember the lowest pid that's greater
		 * than mpid, so we can avoid checking for a while.
		 */
		rpp = allproc;
again:
		for (; rpp != NULL; rpp = rpp->p_nxt) {
			if (rpp->p_pid == mpid || rpp->p_pgrp == mpid) {
				mpid++;
				if (mpid >= pidchecked)
					goto retry;
			}
			if (rpp->p_pid > mpid && pidchecked > rpp->p_pid)
				pidchecked = rpp->p_pid;
			if (rpp->p_pgrp > mpid && pidchecked > rpp->p_pgrp)
				pidchecked = rpp->p_pgrp;
		}
		if (!doingzomb) {
			doingzomb = 1;
			rpp = zombproc;
			goto again;
		}
	}
	if ((rpp = freeproc) == NULL)
		panic("no procs");

	freeproc = rpp->p_nxt;			/* off freeproc */

	/*
	 * Make a proc table entry for the new process.
	 */
	rip = u.u_procp;
	up = &rip->p_utask;

	if (rpp->p_ref_count != 0)
		panic("newproc");

	mutex_init(&rpp->p_lock);
	rpp->p_ref_count = 1;
	queue_init(&rpp->p_servers);
	rpp->p_sigref = 0;

	/*
	 * We need to create the task now so we have someplace to create
	 * the shared region
	 */

	{
	    kern_return_t	result;
	    mach_port_t		new_req_port;

#ifdef mac2 /* do it the old way */
	    result = task_create(rip->p_task,
				 is_sys_proc? FALSE: TRUE,
				 &rpp->p_task);
#else
	    result = task_create(rip->p_task, TRUE, &rpp->p_task);
#endif
	    if (result != KERN_SUCCESS) {
		printf("mach_fork:task create failure %d\n",result);
		goto out;
	    }
	    result = thread_create(rpp->p_task, &rpp->p_thread);
	    if (result != KERN_SUCCESS) {
		(void) task_terminate(rpp->p_task);
		(void) mach_port_deallocate(mach_task_self(), rpp->p_task);
		printf("mach_fork:thread create failure %d\n",result);
		goto out;
	    }
	    new_req_port = task_to_proc_enter(rpp->p_task, rpp);

	    /*
	     * Insert the BSD request port for the task as
	     * its bootstrap port.
	     */
	    result = task_set_bootstrap_port(rpp->p_task,
					     new_req_port);
	    if (result != KERN_SUCCESS)
		panic("set bootstrap port on fork");
#if	MAP_UAREA
	    if ((result=mapin_user(rpp)) != KERN_SUCCESS) {
		printf("mach_fork:mapin_user %d\n",result);
		goto out;
	    }
	    bcopy(rip->p_shared_ro,rpp->p_shared_ro,sizeof(struct ushared_ro));
	    bcopy(rip->p_shared_rw,rpp->p_shared_rw,sizeof(struct ushared_rw));
	    rpp->p_shared_rw->us_inuse = 0;
	    rpp->p_shared_rw->us_debug = enable_emulator_debugging ;
	    share_lock_init(&rpp->p_shared_rw->us_lock);
	    rpp->p_shared_ro->us_version = USHARED_VERSION;
	    rpp->p_shared_ro->us_nofile = NOFILE;
	    rpp->p_shared_ro->us_proc_pointer = (int)rpp;
	    bcopy(u.u_rlimit, rpp->p_shared_ro->us_rlimit, sizeof(u.u_rlimit));
	    rpp->p_shared_rw->us_sigstack = rip->p_shared_rw->us_sigstack;
	    rpp->p_shared_rw->us_closefile = -1;
#if	MACH_NBC
	    rpp->p_shared_ro->us_mach_nbc = 1;
	    rpp->p_shared_rw->us_map_files = 0;
	    for (n = 0; n <= up->uu_lastfile; n++) {
		register struct file_info *fi = 
			&rpp->p_shared_rw->us_file_info[n];
		fi->inuse = fi->control = FALSE;
		share_lock_init(&fi->lock);
	    }
#endif	MACH_NBC
#endif	MAP_UAREA
	}

#if	CMUCS
	rpp->p_flag = SLOAD | (rip->p_flag & (SPAGI|SOUSIG
					      |SXONLY
					     ));
#else	CMUCS
	rpp->p_flag = SLOAD | (rip->p_flag & (SPAGI|SOUSIG));
#endif	CMUCS
	if (is_sys_proc)
	    rpp->p_flag |= SSYS;
	rpp->p_stat = SIDL;
	rpp->p_nice = rip->p_nice;

#if	MAP_UAREA
	share_lock_init(&rpp->p_siglock);
#else	MAP_UAREA
	mutex_init(&rpp->p_siglock);
#endif	MAP_UAREA
	rpp->p_sigmask = rip->p_sigmask;
	rpp->p_sigignore = rip->p_sigignore;
	rpp->p_sigcatch = rip->p_sigcatch;
	/* take along any pending signals like stops? */
	rpp->p_sig = 0;
	rpp->p_cursig = 0;
	rpp->p_stopsig = 0;
	rpp->p_stopcode = 0;
	rpp->p_stop_clear_port = MACH_PORT_NULL;

	rpp->p_uid = rip->p_uid;
	rpp->p_pgrp = rip->p_pgrp;
	rpp->p_pid = mpid;
	rpp->p_ppid = rip->p_pid;

	rpp->p_pptr = rip;
	rpp->p_cptr = NULL;

	rpp->p_osptr = rip->p_cptr;
	if (rip->p_cptr)
		rip->p_cptr->p_ysptr = rpp;
	rpp->p_ysptr = NULL;
	rip->p_cptr = rpp;

	rpp->p_tptr = NULL;

	timerclear(&rpp->p_realtimer.it_value);
#if	QUOTA
	rpp->p_quota = rip->p_quota;
	rpp->p_quota->q_cnt++;
#endif	QUOTA

#if	CMUCS
	rpp->p_logdev = NODEV;
	rpp->p_ttyd = rip->p_ttyd;
	rpp->p_ttyp = rip->p_ttyp;
#endif	CMUCS

	/*
	 * Increase reference counts on shared objects.
	 */
	for (n = 0; n <= up->uu_lastfile; n++) {
		fp = up->uu_ofile[n];
		if (fp == NULL)
			continue;
		fp->f_count++;
#if	MACH_NBC && MAP_UAREA
		if (u.u_shared_rw->us_file_info[n].mapped) {
		    struct inode *ip = (struct inode *)fp->f_data;
		    mu_lock(ip);
		    ip->count++;
		    mu_unlock(ip);
		}
#endif	MACH_NBC && MAP_UAREA
	}
	if (up->uu_cdir)
#if	CMUCS
	    iincr_chk(up->uu_cdir);
#else	CMUCS
	    up->uu_cdir->i_count++;
#endif	CMUCS
	if (up->uu_rdir)
#if	CMUCS
	    iincr_chk(up->uu_rdir);
#else	CMUCS
	    up->uu_rdir->i_count++;
#endif	CMUCS
#if	VICE
	if (up->uu_textfile)
	    up->uu_textfile->f_count++;
#endif	VICE

	/*
	 * Copy task u-area from parent and zero the timing fields.
	 * Must not be blocked between incrementing file reference
	 * counts (above) and copying u-area; otherwise, another
	 * thread could open a file while we were blocked, leaving
	 * an entry in the file table that only has one reference
	 * but will be copied to the new task.
	 */
	rpp->p_utask = *up;
	bzero((caddr_t) &rpp->p_utask.uu_ru,  sizeof(struct rusage));
	bzero((caddr_t) &rpp->p_utask.uu_cru, sizeof(struct rusage));
	/*
	 * and fix uu_procp
	 */
	rpp->p_utask.uu_procp = rpp;
#if	CMUCS_RFS
	/*
	 *  Notify the remote file system module that a new process is being
	 *  created.
	 */
	rfs_newproc(rpp, 0);
#endif	CMUCS_RFS

	/*
	 *	It is now safe to link onto allproc
	 */
	n = PIDHASH(rpp->p_pid);
	rpp->p_idhash = pidhash[n];
	pidhash[n] = rpp - proc;

	rpp->p_nxt = allproc;			/* onto allproc */
	rpp->p_nxt->p_prev = &rpp->p_nxt;	/*   (allproc is never NULL) */
	rpp->p_prev = &allproc;
	allproc = rpp;
	rpp->p_stat = SRUN;			/* XXX */

#if	CMUCS_RFS
	{
	    /*
	     * Temporarily become the child process to execute
	     * rfs_fork(). (hack hack choke cough...)
	     */
	    struct proc *proc_save;

	    proc_save = u.u_procp;
	    u.u_procp = rpp;
	    rfs_fork();
	    u.u_procp = proc_save;
	}
#endif	CMUCS_RFS
	return (rpp);

	/*
	 * If we cannot get a task or thread, undo the
	 * process creation.
	 */
    out:
	/*
	 * XXX deal with this later XXX
	 */
	panic("newproc: no thread or task");
}

