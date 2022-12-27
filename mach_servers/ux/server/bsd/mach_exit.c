/* 
 * Mach Operating System
 * Copyright (c) 1990 Carnegie-Mellon University
 * Copyright (c) 1989 Carnegie-Mellon University
 * Copyright (c) 1988 Carnegie-Mellon University
 * Copyright (c) 1987 Carnegie-Mellon University
 * All rights reserved.  The CMU software License Agreement specifies
 * the terms and conditions for use and redistribution.
 */
/*
 * HISTORY
 * $Log:	mach_exit.c,v $
 * Revision 2.7  90/06/19  23:07:33  rpd
 * 	Added p_sigref check to proc_exit.
 * 	Changed sleep calls in proc_exit to use PSPECL.
 * 	[90/06/12            rpd]
 * 
 * 	Removed debugging code, added uu_proc_exit flag in proc_exit.
 * 	[90/06/05            rpd]
 * 
 * Revision 2.6  90/06/02  15:21:49  rpd
 * 	Added debugging code to proc_exit, looking for longjmps
 * 	from sleep that would prematurely terminate proc_exit.
 * 	[90/05/30            rpd]
 * 
 * 	Made proc_exit more robust: it checks return code from task_threads.
 * 	[90/05/11            rpd]
 * 
 * 	In proc_exit, leave the dead process with a reference count of zero.
 * 	Also, when deregistering self leave uu_procp null.
 * 	[90/05/10            rpd]
 * 	Converted to new IPC.
 * 	Deallocate task and thread ports.
 * 	[90/03/26  19:36:50  rpd]
 * 
 * Revision 2.5  90/05/29  20:23:07  rwd
 * 	In proc_exit, leave the dead process with a reference count of zero.
 * 	Also, when deregistering self leave uu_procp null.
 * 	[90/05/10            rpd]
 * 	Use TASK_THREAD_TIMES_INFO to get times for threads on exit.
 * 	[90/04/03            dbg]
 * 
 * 	Leave resource usage in u.u_ru on exit, instead of copying to an
 * 	mbuf.  The u-area is still visible in wait().
 * 	[90/03/28            dbg]
 * 
 * 	Call close_file instead of closef on exit.
 * 	[90/04/09            rwd]
 * 
 * Revision 2.4  90/05/21  13:50:04  dbg
 * 	Use TASK_THREAD_TIMES_INFO to get times for threads on exit.
 * 	[90/04/03            dbg]
 * 
 * 	Leave resource usage in u.u_ru on exit, instead of copying to an
 * 	mbuf.  The u-area is still visible in wait().
 * 	[90/03/28            dbg]
 * 
 * Revision 2.3  89/11/15  13:26:50  dbg
 * 	If the process being terminated is stopped, restart it so that
 * 	its server threads can finish.
 * 	[89/10/24            dbg]
 * 
 * Revision 2.2  89/10/17  11:25:25  rwd
 * 	Added bsd_init_process.
 * 	[89/09/29            rwd]
 * 
 * Revision 2.1.1.1  89/09/26  14:54:34  dbg
 * 	Remove call to unsignal_proc.
 * 	[89/09/21            dbg]
 * 
 * 	Out-of-kernel version.  NO CONDITIONALS!
 * 	[89/01/02            dbg]
 * 
 * Revision 2.1  89/08/04  14:07:39  rwd
 * Created.
 * 
 * Revision 2.6  88/12/19  02:33:50  mwyoung
 * 	Removed old MACH conditionals.
 * 	[88/12/13            mwyoung]
 * 
 * Revision 2.5  88/08/24  01:18:58  mwyoung
 * 	Corrected include file references.
 * 	[88/08/22            mwyoung]
 * 
 * Revision 2.4  88/08/06  17:59:09  rpd
 * Eliminated use of kern/mach_ipc_defs.h.
 * 
 *  2-Jun-88  David Golub (dbg) at Carnegie-Mellon University
 *	Removed call to fpaclose_thread; it is done by thread_terminate
 *	(via pcb_terminate).
 *
 *  4-May-88  David Black (dlb) at Carnegie-Mellon University
 *	MACH_TIME_NEW is now standard.
 *	Check p_stat when looking for stopped processes.
 * 
 * 29-Mar-88  Michael Young (mwyoung) at Carnegie-Mellon University
 *	MACH: Remove references to multprog.
 *
 *  2-Mar-88  David Black (dlb) at Carnegie-Mellon University
 *	Use thread_read_times to get times.  This replaces and
 *	generalizes the MACH_TIME_NEW code.
 *
 * 17-Feb-88  Jonathan J. Chew (jjc) at Carnegie-Mellon University
 *	Added call to fpaclose_thread() in exit().
 *
 * 12-Feb-88  David Black (dlb) at Carnegie-Mellon University
 *	Update MACH_TIME_NEW interface to use time_value_t's.
 *
 * 29-Dec-87  David Golub (dbg) at Carnegie-Mellon University
 *	Delinted.
 *
 * 23-Dec-87  David Golub (dbg) at Carnegie-Mellon University
 *	Made exit halt all threads in task before cleaning up.
 *
 *  8-Dec-87  David Golub (dbg) at Carnegie-Mellon University
 *	Follow task_terminate with thread_halt_self for new termination
 *	logic.  Check for null p->task for Zombie check in wait.
 *
 * 21-Nov-87  Avadis Tevanian (avie) at Carnegie-Mellon University
 *	Condensed some conditionals, purged previous history.
 *
 * 30-Jan-87  Mike Accetta (mja) at Carnegie-Mellon University
 *	CMUCS:  changed wait() to handle new WLOGINDEV option and
 *	slightly reorganized the ns32000 exit path to make it more
 *	common;  changed exit() to call new ttylogout() routine.
 *	[ V5.1(F1) ]
 *
 *  2-Dec-86  Jay Kistler (jjk) at Carnegie-Mellon University
 *	VICE:  added hooks for ITC/Andrew remote file system.
 *
 * 09-May-85  Glenn Marcy (gm0w) at Carnegie-Mellon University
 *	Upgraded from 4.1BSD.  Carried over changes:
 *	CS_IPC:  Added call to IPCSuicide() in exit().
 *	CS_RFS:  Added call to rfs_exit() in exit().
 *	NILDR:  Added call to ildrrma() in exit().
 *	[V1(1)]
 *
 **********************************************************************
 */
 
#include <cputypes.h>

#include <cmucs.h>
#include <cmucs_rfs.h>
#include <quota.h>
#include <vice.h>
 
#include <ild.h>
/*
 * Copyright (c) 1982, 1986 Regents of the University of California.
 * All rights reserved.  The Berkeley software License Agreement
 * specifies the terms and conditions for redistribution.
 *
 *	@(#)kern_exit.c	7.1 (Berkeley) 6/5/86
 */

#include <sys/param.h>
#include <sys/systm.h>
#include <sys/map.h>
#include <sys/dir.h>
#include <sys/user.h>
#include <sys/kernel.h>
#include <sys/proc.h>
#include <sys/buf.h>
#include <sys/wait.h>
#include <sys/file.h>
#include <sys/mbuf.h>
#include <sys/inode.h>
#include <sys/syslog.h>
#include <sys/parallel.h>

#include <uxkern/import_mach.h>

/*
 * Exit system call: pass back caller's arg
 */
rexit()
{
	register struct a {
		int	rval;
	} *uap;

	uap = (struct a *)u.u_ap;
	proc_exit(u.u_procp,
		  (uap->rval & 0377) << 8,
		  FALSE);
	u.u_eosys = JUSTRETURN;
}

/*
 * Release resources.
 * Save u. area for parent to look at.
 * Enter zombie state.
 * Wake up parent and init processes,
 * and dispose of children.
 */
proc_exit(p, rv, dump)
	register struct proc	*p;
	int			rv;
	boolean_t		dump;
{
	register int i;
	register struct proc *q, *nq;
	register int x;

	struct proc *save_proc   = u.u_procp;

#ifdef PGINPROF
	vmsizmon();
#endif
	/*
	 * If the task is already exiting, quit.
	 */
	if (p->p_flag & SWEXIT)
	    return;

	/*
	 * Mark it as exiting.
	 */
	p->p_flag |= SWEXIT;
	p->p_flag &= ~STRC;

	/*
	 * Suspend the user task.
	 */
	(void) task_suspend(p->p_task);

	/*
	 * Wait until all outstanding signal retries have been received.
	 * The locking of p_sigref is a bit dubious.  While the process
	 * is active, increments and decrements are locked by p_lock.
	 * While the process is exiting, no increments should happen;
	 * decrements and this loop are protected by the master lock.
	 */
	while (p->p_sigref > 0)
	    sleep((caddr_t) &p->p_sigref, PSPECL);

	/*
	 * If the process is stopped, wake up the server threads.
	 * Do not use start() because that will change the stop signal.
	 */
	if (p->p_stat == SSTOP) {
	    p->p_stat = SRUN;
	    wakeup((caddr_t)&p->p_stat);
	}

	/*
	 * Terminate all server threads working for that task,
	 * except for the current thread.
	 */
	{
	    register uthread_t	cth;
	    extern   uthread_t	server_thread_find();
	    extern   void	server_thread_deregister();

	    while ((cth = server_thread_find(p)) != 0) {
		if (cth == &u) {
		    server_thread_deregister(cth, p);
		    save_proc = 0;
		}
		else {
		    unsleep(cth);
		    sleep((caddr_t)&p->p_flag, PSPECL);
		}
	    }
	}

	/*
	 * Get a core dump if requested.
	 */
	if (dump) {
	    if (core_dump(p) == 0)
		rv += 0200;	/* indicate that process dumped core */
	}

	/*
	 * Get the times for all threads in the process.
	 */
	{
	    struct task_thread_times_info
				thread_times;
	    struct task_basic_info
				bi;
	    unsigned int	count;

	    /*
	     * Get times for dead threads
	     */
	    count = TASK_BASIC_INFO_COUNT;
	    (void) task_info(p->p_task,
			     TASK_BASIC_INFO,
			     (task_info_t)&bi,
			     &count);

	    /*
	     * Get times for live threads
	     */
	    count = TASK_THREAD_TIMES_INFO_COUNT;
	    (void) task_info(p->p_task,
			     TASK_THREAD_TIMES_INFO,
			     (task_info_t)&thread_times,
			     &count);

	    /*
	     * Add user run times.
	     */
	    time_value_add(&bi.user_time,&thread_times.user_time);
	    p->p_utask.uu_ru.ru_utime.tv_sec  = bi.user_time.seconds;
	    p->p_utask.uu_ru.ru_utime.tv_usec = bi.user_time.microseconds;

	    /*
	     * Add system run times.
	     */
	    time_value_add(&bi.system_time,&thread_times.system_time);
	    p->p_utask.uu_ru.ru_stime.tv_sec  = bi.system_time.seconds;
	    p->p_utask.uu_ru.ru_stime.tv_usec = bi.system_time.microseconds;

	}

	/*
	 * Pretend we are that process, for the duration of the call.
	 */
	u.u_procp = p;
	u.uu_proc_exit = TRUE;

	p->p_sigignore = ~0;
	for (i = 0; i < NSIG; i++)
		p->p_utask.uu_signal[i] = SIG_IGN;
	untimeout(realitexpire, (caddr_t)p);
	for (i = 0; i <= p->p_utask.uu_lastfile; i++) {
		register struct file *f;

		f = p->p_utask.uu_ofile[i];
		if (f) {
			close_file(i);
			p->p_utask.uu_ofile[i] = NULL;
			p->p_utask.uu_pofile[i] = 0;
		}
	}
#if	CMUCS_RFS
	/*  after closing all files ... */
	rfs_exit();
#endif	CMUCS_RFS
#if	CMUCS
	ttylogout(p->p_logdev);
#endif	CMUCS
	{
	    register struct inode *ip;

	    if (ip = p->p_utask.uu_cdir) {
		ilock(ip);
		iput(ip);
	    }
	    if (ip = p->p_utask.uu_rdir) {
		ilock(ip);
		iput(ip);
	    }
	}
#if	VICE
	{
	    register struct file *f;

	    if (f = p->p_utask.uu_textfile) {
		closef(f);
		p->p_utask.uu_textfile = NULL;
	    }
	}
#endif	VICE
#if	NILD > 0
	/*
	 * remove outstanding ingres locks for
	 * the dying process
	 */
	ildrma(p->p_pid);
#endif	NILD
	p->p_utask.uu_rlimit[RLIMIT_FSIZE].rlim_cur = RLIM_INFINITY;
	acct();
#if	QUOTA
	qclean();
#endif	QUOTA
	if (p->p_ref_count != 1)
		panic("proc_exit");
	p->p_ref_count = 0;
	if (*p->p_prev = p->p_nxt)		/* off allproc queue */
		p->p_nxt->p_prev = p->p_prev;
	if (p->p_nxt = zombproc)		/* onto zombproc */
		p->p_nxt->p_prev = &p->p_nxt;
	p->p_prev = &zombproc;
	zombproc = p;
	task_to_proc_remove(p->p_task);
	p->p_stat = SZOMB;
	i = PIDHASH(p->p_pid);
	x = p - proc;
	if (pidhash[i] == x)
		pidhash[i] = p->p_idhash;
	else {
		for (i = pidhash[i]; i != 0; i = proc[i].p_idhash)
			if (proc[i].p_idhash == x) {
				proc[i].p_idhash = p->p_idhash;
				goto done;
			}
		panic("exit");
	}
	if (p->p_pid == 1) {
#if	CMUCS
		printf("init exited with %d\n",
			rv>>8);
#endif	CMUCS
		if (p->p_utask.uu_data_start == 0) {
			printf("Can't exec /etc/init\n");
			for (;;)
				;
		} else
			panic("init died");
	}
done:
#ifdef mac2
	/* terminate the task before wait1() can put this proc back
	   on the freeproc list, otherwise newproc() could grab it
	   and start it up before we are done with it here */

	(void) task_terminate(p->p_task);

	/*
	 * Yes, it can happen.  The user-reference counts on
	 * the task/thread port names might be greater than one
	 * because other threads in the Unix server were doing
	 * things with the ports.
	 */

	(void) mach_port_deallocate(mach_task_self(), p->p_task);
	p->p_task = MACH_PORT_NULL;
	if (MACH_PORT_VALID(p->p_thread))
	    (void) mach_port_deallocate(mach_task_self(), p->p_thread);
	p->p_thread = MACH_PORT_NULL;
#endif
	p->p_xstat = rv;

	ruadd(&p->p_utask.uu_ru, &p->p_utask.uu_cru);
	if (p->p_cptr)		/* only need this if any child is S_ZOMB */
		wakeup((caddr_t)&proc[1]);
	for (q = p->p_cptr; q != NULL; q = nq) {
		nq = q->p_osptr;
		if (nq != NULL)
			nq->p_ysptr = NULL;
		if (proc[1].p_cptr)
			proc[1].p_cptr->p_ysptr = q;
		q->p_osptr = proc[1].p_cptr;
		q->p_ysptr = NULL;
		proc[1].p_cptr = q;

		q->p_pptr = &proc[1];
		q->p_ppid = 1;
		/*
		 * Traced processes are killed
		 * since their existence means someone is screwing up.
		 * Stopped processes are sent a hangup and a continue.
		 * This is designed to be ``safe'' for setuid
		 * processes since they must be willing to tolerate
		 * hangups anyways.
		 */
		if (q->p_flag&STRC) {
			q->p_flag &= ~STRC;
			psignal(q, SIGKILL);
		} else if (q->p_stat == SSTOP) {
			psignal(q, SIGHUP);
			psignal(q, SIGCONT);
		}
		/*
		 * Protect this process from future
		 * tty signals, clear TSTP/TTIN/TTOU if pending.
		 */
		(void) spgrp(q);
	}
	p->p_cptr = NULL;
	psignal(p->p_pptr, SIGCHLD);
	wakeup((caddr_t)p->p_pptr);
#ifndef mac2
	(void) task_terminate(p->p_task);

	/*
	 * Yes, it can happen.  The user-reference counts on
	 * the task/thread port names might be greater than one
	 * because other threads in the Unix server were doing
	 * things with the ports.
	 */

	(void) mach_port_deallocate(mach_task_self(), p->p_task);
	p->p_task = MACH_PORT_NULL;
	if (MACH_PORT_VALID(p->p_thread))
	    (void) mach_port_deallocate(mach_task_self(), p->p_thread);
	p->p_thread = MACH_PORT_NULL;
#endif
	/*
	 * Return to our former identity.
	 */
	u.uu_proc_exit = FALSE;
	u.u_procp = save_proc;
}

wait()
{
#if	CMUCS
	/*
	 *  In order to address the "logged-in" terminal problem for window
	 *  managers, telnet servers, etc. which are not created initially by
	 *  init but which we want init to clean up after, we add the concept
	 *  of the "login device" associated with a process.  The login
	 *  program executes a special ioctl() call to distinguish the top
	 *  process in the tree and record the controlling device with which
	 *  it is associated.  The new wait3() option, WLOGINDEV, (used by
	 *  init) indicates that the returned resource usage record should
	 *  include the device number of the controlling terminal for the
	 *  top level process (this will be NODEV for any other process).
	 *  This way, init need not create terminal listeners for all
	 *  potential login terminals but can still know enough to clean
	 *  things up after such processes (which it has inherited) do exit.
	 */
#define	rusage			rusage_dev
#endif	CMUCS
	int options;
	struct rusage ru, *rup;

	/*
	 * Wait always takes 2 arguments and returns 2 values.
	 */
	struct a {
		int		options;
		struct rusage *	rup;
	};
	register struct a *uap = (struct a *)u.u_ap;

	options = uap->options;
	rup = uap->rup;
	u.u_error = wait1(options, &ru);

	if (u.u_error)
		return;
	if (rup != (struct rusage *)0)
#if	CMUCS
		u.u_error = copyout((caddr_t)&ru, (caddr_t)rup, 
			(options&WLOGINDEV)?sizeof(ru):sizeof(ru.ru_rusage));
#else	CMUCS
		u.u_error = copyout((caddr_t)&ru, (caddr_t)rup,
		    sizeof (struct rusage));
#endif	CMUCS
}

#if	CMUCS
#undef	rusage
#endif	CMUCS

/*
 * Wait system call.
 * Search for a terminated (zombie) child,
 * finally lay it to rest, and collect its status.
 * Look also for stopped (traced) children,
 * and pass back status from them.
 */
wait1(options, ru)
	register int options;
#if	CMUCS
	struct rusage_dev *ru;
#else	CMUCS
	struct rusage *ru;
#endif	CMUCS
{
	register f;
	register struct proc *p, *q;

	f = 0;
loop:
	q = u.u_procp;
	for (p = q->p_cptr; p; p = p->p_osptr) {
		f++;
		if (p->p_stat == SZOMB) {
			u.u_r.r_val1 = p->p_pid;
			u.u_r.r_val2 = p->p_xstat;
			p->p_xstat = 0;
#if	CMUCS
			if (ru)
			{
			    ru->ru_rusage = p->p_utask.uu_ru;
			    ru->ru_dev = p->p_logdev;
			}
#else	CMUCS
			if (ru)
				*ru = p->p_utask.uu_ru;
#endif	CMUCS
			ruadd(&u.u_cru, &p->p_utask.uu_ru);

			p->p_stat = NULL;
			p->p_pid = 0;
			p->p_ppid = 0;
			if (*p->p_prev = p->p_nxt)	/* off zombproc */
				p->p_nxt->p_prev = p->p_prev;
			p->p_nxt = freeproc;		/* onto freeproc */
			freeproc = p;
			if (q = p->p_ysptr)
				q->p_osptr = p->p_osptr;
			if (q = p->p_osptr)
				q->p_ysptr = p->p_ysptr;
			if ((q = p->p_pptr)->p_cptr == p)
				q->p_cptr = p->p_osptr;
			p->p_pptr = 0;
			p->p_ysptr = 0;
			p->p_osptr = 0;
			p->p_cptr = 0;
			p->p_sig = 0;
			p->p_sigcatch = 0;
			p->p_sigignore = 0;
			p->p_sigmask = 0;
			p->p_pgrp = 0;
			p->p_flag = 0;
			p->p_stopsig = 0;
			p->p_cursig = 0;
			return (0);
		}
		if (p->p_stat == SSTOP && (p->p_flag&SWTED)==0 &&
		    (p->p_flag&STRC || options&WUNTRACED)) {
			p->p_flag |= SWTED;
			u.u_r.r_val1 = p->p_pid;
			u.u_r.r_val2 = (p->p_stopsig<<8) | WSTOPPED;
			return (0);
		}
	}
	if (f == 0)
		return (ECHILD);
	if (options&WNOHANG) {
		u.u_r.r_val1 = 0;
		return (0);
	}
	if (setjmp(&u.u_qsave)) {
		p = u.u_procp;
		if ((u.u_sigintr & sigmask(p->p_cursig)) != 0)
			return(EINTR);
		u.u_eosys = RESTARTSYS;
		return (0);
	}
	sleep((caddr_t)u.u_procp, PWAIT);
	goto loop;
}

kern_return_t	init_process()
/*
 *	Make the current process an "init" process, meaning
 *	that it doesn't have a parent, and that it won't be
 *	gunned down by kill(-1, 0).
 */
{
	register struct proc *p;


	if (!suser())
		return(KERN_NO_ACCESS);

	unix_master();
	p = u.u_procp;

	/*
	 *	Take us out of the sibling chain, and
	 *	out of our parent's child chain.
	 */

	if (p->p_osptr)
		p->p_osptr->p_ysptr = p->p_ysptr;
	if (p->p_ysptr)
		p->p_ysptr->p_osptr = p->p_osptr;
	if (p->p_pptr->p_cptr == p)
		p->p_pptr->p_cptr = p->p_osptr;
	p->p_pptr = p;
	p->p_ysptr = p->p_osptr = 0;
	p->p_pgrp = p->p_pid;
	p->p_ppid = 0;

	unix_release();
	return(KERN_SUCCESS);
}

int
bsd_init_process(proc_port, interrupt)
	mach_port_t	proc_port;
	boolean_t	*interrupt;
{
	register int	error;

	if (error = start_server_op(proc_port, 1002))
	    return (error);

	unix_master();

	init_process();

	unix_release();

	return (end_server_op(error, interrupt));
}
