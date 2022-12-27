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
 * $Log:	mach_signal.c,v $
 * Revision 2.11  91/03/20  14:58:20  dbg
 * 	Fixed issig to use psignal3, so proc_exit isn't called directly.
 * 	[91/02/03            rpd]
 * 
 * Revision 2.10  90/08/06  15:31:44  rwd
 * 	Fix new references to share_lock.
 * 	[90/06/27            rwd]
 * 	Fix references to share_lock.
 * 	[90/06/08            rwd]
 * 	Change sleeps PSPECL instead of PZERO to release a
 * 	server thread.
 * 	[90/05/29            rwd]
 * 
 * Revision 2.9  90/06/19  23:07:58  rpd
 * 	Removed obsolete debugging panic in sigpause.
 * 	[90/06/13            rpd]
 * 
 * 	Added psignal3, gsignal3.
 * 	[90/06/09            rpd]
 * 
 * 	Changed all the non-interruptible sleeps to use PSPECL.
 * 	[90/06/08            rpd]
 * 
 * Revision 2.8  90/06/02  15:22:14  rpd
 * 	Made thread_psignal more robust.  If it doesn't find a process,
 * 	it nukes the task.
 * 	[90/05/11            rpd]
 * 
 * 	Modified thread_psignal for new register/deregister conventions.
 * 	[90/05/10            rpd]
 * 	Fixed killpg1, gsignal loops to allow psignal to nuke processes.
 * 	[90/05/04            rpd]
 * 
 * 	Removed us_sigcheck; it is no longer needed.
 * 	[90/05/03            rpd]
 * 	In psignal, handle SIGKILL by nuking the victim directly.
 * 	[90/04/05            rpd]
 * 
 * 	Fixed thread_signal to not sleep if the signal is masked.
 * 	Instead just save the signal in p_sig, like the mainline does.
 * 	[90/03/27            rpd]
 * 	Converted to new IPC.
 * 	[90/03/26  19:39:03  rpd]
 * 	Fixed killpg1, gsignal loops to allow psignal to nuke processes.
 * 	[90/05/04            rpd]
 * 
 * 	Removed us_sigcheck; it is no longer needed.
 * 	[90/05/03            rpd]
 * 	In psignal, handle SIGKILL by nuking the victim directly.
 * 	[90/04/05            rpd]
 * 
 * 	Fixed thread_signal to not sleep if the signal is masked.
 * 	Instead just save the signal in p_sig, like the mainline does.
 * 	[90/03/27            rpd]
 * 	Converted to new IPC.
 * 	[90/03/26  19:39:03  rpd]
 * 
 * Revision 2.7  90/05/29  20:23:19  rwd
 * 	Took rpd version and undid newipc fixes.
 * 	[90/05/12            rwd]
 * 
 * 	Made thread_psignal more robust.  If it doesn't find a process,
 * 	it nukes the task.
 * 	[90/05/11            rpd]
 * 
 * 	Modified thread_psignal for new register/deregister conventions.
 * 	[90/05/10            rpd]
 * 
 * Revision 2.6  90/05/21  13:50:24  dbg
 * 	Always set sigtramp.
 * 	[90/04/23            dbg]
 * 
 * Revision 2.5  90/03/14  21:26:26  rwd
 * 	Fix typo in issig which masked sigignore at wrong times.
 * 	[90/03/01            rwd]
 * 	Change shared locks to use share_lock macros.
 * 	[90/02/16            rwd]
 * 	Added new MAP_UAREA code
 * 	[90/01/31            rwd]
 * 
 * Revision 2.4  89/11/29  15:27:44  af
 * 	Sigtramp must be set first, as setsigvec might deliver a signal
 * 	right away as a result of a different mask.
 * 	[89/11/26  11:32:18  af]
 * 
 * 	Signal trampoline support for mips et al.
 * 	[89/11/16  16:57:51  af]
 * 
 * Revision 2.3  89/11/15  13:26:57  dbg
 * 	Fix order of arguments to bsd_sigreturn to match MiG interface.
 * 	[89/11/08            dbg]
 * 	Thread_psignal does wakeup on exit if process is exiting.
 * 	[89/10/24            dbg]
 * 
 * Revision 2.2  89/10/17  11:25:56  rwd
 * 	Use p_siglock to protect p_sig*.
 * 
 * 	use p_siglock to protect p_sig* (ONLY!)
 * 	[89/09/25            dbg]
 * 
 * 	Completely rewrite.  Move signal handling back to server threads
 * 	for user process.
 * 	[89/09/21            dbg]
 * 
 * 	Add interrupt return parameter to bsd_take_signal and
 * 	bsd_sigreturn.
 * 	[89/09/21            dbg]
 * 
 * Revision 2.1  89/08/04  14:08:09  rwd
 * Created.
 * 
 *  3-May-89  David Golub (dbg) at Carnegie-Mellon University
 *	Return signal parameters to user.  Signal frame manipulation is
 *	now done in emulator.
 *
 *  6-Apr-89  David Golub (dbg) at Carnegie-Mellon University
 *	Rearrange proc structure fields.
 *
 * 13-Feb-89  David Golub (dbg) at Carnegie-Mellon University
 *	Rewrite... make serial again (the locking is too messy to do
 *	partially)... make psignal deliver signal immediately if
 *	possible!
 *
 * 17-Jan-89  David Golub (dbg) at Carnegie-Mellon University
 *	Moved the surviving routines from kern_sig here.
 *
 *  5-Jan-89  David Golub (dbg) at Carnegie-Mellon University
 *	Moved out of kernel.
 *
 ****	Experimental...
 *	Move most of signal handling here.  Have it done by new
 *	kernel thread, to keep interrupt stuff out.
 *
 *  1-Oct-87  David Black (dlb) at Carnegie-Mellon University
 *	Created by cutting down psignal to only deal with exceptions.
 *
 */

/*
 * Copyright (c) 1982, 1986 Regents of the University of California.
 * All rights reserved.  The Berkeley software License Agreement
 * specifies the terms and conditions for redistribution.
 *
 *	@(#)kern_sig.c	7.1 (Berkeley) 6/5/86
 */

#include <map_uarea.h>

#include <sys/param.h>
#include <sys/systm.h>
#include <sys/dir.h>
#include <sys/user.h>
#include <sys/acct.h>
#include <sys/inode.h>
#include <sys/proc.h>
#include <sys/kernel.h>

#include <uxkern/import_mach.h>

#include <sys/parallel.h>
#include <sys/syscall.h>
#include <uxkern/syscall_subr.h>
#include <uxkern/proc_to_task.h>
#include <uxkern/bsd_2.h>

/*
 * Generalized interface signal handler.
 */
sigvec()
{
	register struct a {
		int	signo;
		struct	sigvec *nsv;
		struct	sigvec *osv;
		int	(*sigtramp)();		/* signal trampoline code */
	} *uap = (struct a  *)u.u_ap;
	struct sigvec vec;
	register struct sigvec *sv;
	register int sig;
	int bit;

	sig = uap->signo;
#if	MULTIMAX
	/*
	 * On the MMax the u area is not visible to the user tasks,
	 * therefore, the task must declare the address of its
	 * trampoline code before it attempts to catch signals.
	 * This is done by using the special signal SIGCATCHALL
	 */
	/* If this is the intermediate signal catcher then set it. */
	if (sig == SIGCATCHALL) {

		/* Copy the uap->nsv structure from user space to system
		 *	space.  Then set the return value and the signal
		 *	catcher.
		 */
		u.u_error = 
			copyin ((caddr_t)uap->nsv, (caddr_t)&vec, sizeof(vec));
		if (u.u_error)
			return;
		u.u_r.r_val1 = (int)u.u_sigcatch;
		u.u_sigcatch = vec.sv_handler;
		return;
	}
#endif	MULTIMAX
	if (sig <= 0 || sig > NSIG || sig == SIGKILL || sig == SIGSTOP) {
		u.u_error = EINVAL;
		return;
	}
	sv = &vec;
	if (uap->osv) {
		sv->sv_handler = u.u_signal[sig];
		sv->sv_mask = u.u_sigmask[sig];
		bit = sigmask(sig);
		sv->sv_flags = 0;
		if ((u.u_sigonstack & bit) != 0)
			sv->sv_flags |= SV_ONSTACK;
		if ((u.u_sigintr & bit) != 0)
			sv->sv_flags |= SV_INTERRUPT;
		u.u_error =
		    copyout((caddr_t)sv, (caddr_t)uap->osv, sizeof (vec));
		if (u.u_error)
			return;
	}
	if (uap->nsv) {
		u.u_error =
		    copyin((caddr_t)uap->nsv, (caddr_t)sv, sizeof (vec));
		if (u.u_error)
			return;
		if (sig == SIGCONT && sv->sv_handler == SIG_IGN) {
			u.u_error = EINVAL;
			return;
		}
#ifdef	mips
		/*
		 * check for unaligned pc on sighandler
		 */
		if (sv->sv_handler != SIG_IGN
		    && ((int)sv->sv_handler & (sizeof(int)-1))) {
			u.u_error = EINVAL;
			return;
		}
#endif	mips
		/*
		 * Save the signal trampoline code address - the
		 * server does not always know where it is.
		 */
		u.u_sigtramp = uap->sigtramp;

		setsigvec(sig, sv);
	}
}

setsigvec(sig, sv)
	int sig;
	register struct sigvec *sv;
{
	register struct proc *p;
	register int bit;

	bit = sigmask(sig);
	p = u.u_procp;

	/*
	 * Change setting atomically.
	 */

	proc_siglock(p);

	u.u_signal[sig] = sv->sv_handler;
	u.u_sigmask[sig] = sv->sv_mask &~ cantmask;

	if (sv->sv_flags & SV_INTERRUPT)
		u.u_sigintr |= bit;
	else
		u.u_sigintr &= ~bit;
	if (sv->sv_flags & SV_ONSTACK)
		u.u_sigonstack |= bit;
	else
		u.u_sigonstack &= ~bit;
	if (sv->sv_handler == SIG_IGN) {
		p->p_sig &= ~bit;		/* never to be seen again */
		p->p_sigignore |= bit;
		p->p_sigcatch &= ~bit;
	} else {
		p->p_sigignore &= ~bit;
		if (sv->sv_handler == SIG_DFL)
			p->p_sigcatch &= ~bit;
		else
			p->p_sigcatch |= bit;

		/*
		 * Check for new signals that can be handled.
		 */
		check_proc_signals(p);

	}
	proc_sigunlock(p);
}

sigblock()
{
	struct a {
		int	mask;
	} *uap = (struct a *)u.u_ap;
	register struct proc *p = u.u_procp;

	proc_siglock(p);
	u.u_r.r_val1 = p->p_sigmask;
	p->p_sigmask |= uap->mask &~ cantmask;
	proc_sigunlock(p);

	/*
	 * Pending signals can only be blocked.
	 * No need to check for new ones.
	 */
}

sigsetmask()
{
	struct a {
		int	mask;
	} *uap = (struct a *)u.u_ap;
	register struct proc *p = u.u_procp;

	proc_siglock(p);

	u.u_r.r_val1 = p->p_sigmask;
	p->p_sigmask = uap->mask &~ cantmask;

	/*
	 * Check for pending signals that are now visible.
	 */
	check_proc_signals(p);

	proc_sigunlock(p);
}

sigpause()
{
	struct a {
		int	mask;
	} *uap = (struct a *)u.u_ap;
	register struct proc *p = u.u_procp;

	/*
	 * When returning from sigpause, we want
	 * the old mask to be restored after the
	 * signal handler has finished.  Thus, we
	 * save it here and mark the proc structure
	 * to indicate this (should be in u.).
	 */

	proc_siglock(p);

	u.u_oldmask = p->p_sigmask;
	p->p_flag |= SOMASK;
	p->p_sigmask = uap->mask &~ cantmask;

	check_proc_signals(p);
	proc_sigunlock(p);

	for (;;)
	    sleep((caddr_t)&u, PSLEP);
	/*NOTREACHED*/
}

sigstack()
{
	register struct a {
		struct	sigstack *nss;
		struct	sigstack *oss;
	} *uap = (struct a *)u.u_ap;
	struct sigstack ss;

#if	MAP_UAREA
	register struct proc *p = u.u_procp;

	share_lock(&u.u_shared_rw->us_lock, p);
#endif	MAP_UAREA
	if (uap->oss) {
		u.u_error = copyout((caddr_t)&u.u_sigstack, (caddr_t)uap->oss, 
		    sizeof (struct sigstack));
		if (u.u_error) {
#if	MAP_UAREA
			share_unlock(&u.u_shared_rw->us_lock, p);
#endif	MAP_UAREA
			return;
		}
	}
	if (uap->nss) {
		u.u_error =
		    copyin((caddr_t)uap->nss, (caddr_t)&ss, sizeof (ss));
		if (u.u_error == 0)
			u.u_sigstack = ss;
	}
#if	MAP_UAREA
	share_unlock(&u.u_shared_rw->us_lock, p);
#endif	MAP_UAREA
}

kill()
{
	register struct a {
		int	pid;
		int	signo;
	} *uap = (struct a *)u.u_ap;
	register struct proc *p;

	if (uap->signo < 0 || uap->signo > NSIG) {
#if	BALANCE
		/*
		 * Special case of init doing `kill(1, LEGAL_USER|nusers)' is
		 * a hook telling system "/etc/init" is the *REAL* one
		 * (and passing back number of users).  This is only
		 * necessary if running DYNIX /etc/init.
		 */
#define	LEGAL_MAGIC (0x12210000)
		if ((uap->signo & 0xffff0000) == LEGAL_MAGIC) {
			if (uap->pid == 1 && u.u_procp == &proc[1]) {
				extern unsigned sec0eaddr;
				sec0eaddr = uap->signo & 0xffff;
			}
		}
#endif	BALANCE
		u.u_error = EINVAL;
		return;
	}
	if (uap->pid > 0) {
		/* kill single process */
		p = pfind(uap->pid);
		if (p == 0) {
			u.u_error = ESRCH;
			return;
		}
		if (u.u_uid && u.u_uid != p->p_uid)
			u.u_error = EPERM;
		else if (uap->signo)
			psignal(p, uap->signo);
		return;
	}
	switch (uap->pid) {
	case -1:		/* broadcast signal */
		u.u_error = killpg1(uap->signo, 0, 1);
		break;
	case 0:			/* signal own process group */
		u.u_error = killpg1(uap->signo, 0, 0);
		break;
	default:		/* negative explicit process group */
		u.u_error = killpg1(uap->signo, -uap->pid, 0);
		break;
	}
	return;
}

killpg()
{
	register struct a {
		int	pgrp;
		int	signo;
	} *uap = (struct a *)u.u_ap;

	if (uap->signo < 0 || uap->signo > NSIG) {
		u.u_error = EINVAL;
		return;
	}
	u.u_error = killpg1(uap->signo, uap->pgrp, 0);
}

/* KILL CODE SHOULDNT KNOW ABOUT PROCESS INTERNALS !?! */

killpg1(signo, pgrp, all)
	int signo, pgrp, all;
{
	register struct proc *p, *np;
	int f, error = 0;

	if (!all && pgrp == 0) {
		/*
		 * Zero process id means send to my process group.
		 */
		pgrp = u.u_procp->p_pgrp;
		if (pgrp == 0)
			return (ESRCH);
	}
	for (f = 0, p = allproc; p != NULL; p = np) {
		/* psignal might remove the process from allproc list */
		np = p->p_nxt;
		if ((p->p_pgrp != pgrp && !all) || p->p_ppid == 0 ||
		    (p->p_flag&SSYS) || (all && p == u.u_procp))
			continue;
		if (u.u_uid != 0 && u.u_uid != p->p_uid &&
		    (signo != SIGCONT || !inferior(p))) {
			if (!all)
				error = EPERM;
			continue;
		}
		f++;
		if (signo)
			psignal(p, signo);
	}
	return (error ? error : (f == 0 ? ESRCH : 0));
}

/*
 * Send the specified signal to
 * all processes with 'pgrp' as
 * process group.
 */
gsignal3(pgrp, sig, can_block)
	register int pgrp;
	int sig;
	boolean_t can_block;
{
	register struct proc *p, *np;

	if (pgrp == 0)
		return;
	for (p = allproc; p != NULL; p = np) {
		/* psignal might remove the process from allproc list */
		np = p->p_nxt;
		if (p->p_pgrp == pgrp)
			psignal3(p, sig, can_block);
	}
}

gsignal(pgrp, sig)
	int pgrp;
	int sig;
{
	gsignal3(pgrp, sig, TRUE);
}

#if	MAP_UAREA
#define	proc_sigtrylock(p)	share_try_lock(p, &(p)->p_siglock)
#else	MAP_UAREA
#define	proc_sigtrylock(p)	mutex_try_lock(&(p)->p_siglock)
#endif	MAP_UAREA

/*
 * Used to complete a psignal3 that couldn't block.
 * The p_sigref count keeps the process from disappearing
 * while the psignal_retry message is in transit.
 */

kern_return_t
sbsd_psignal_retry(proc_port, sig)
	mach_port_t proc_port;
	int sig;
{
	register struct proc *p = (struct proc *) proc_port;
	register struct uthread *uth = &u;

	server_thread_register(uth, p);
	uarea_init(uth, p);
	unix_master();

	mutex_lock(&p->p_lock);
	if (--p->p_sigref == 0)
		wakeup((caddr_t) &p->p_sigref);
	mutex_unlock(&p->p_lock);

	psignal3(p, sig, TRUE);

	unix_release();

	/*
	 * If psignal3 killed the process, then proc_exit
	 * might have already deregistered us.
	 */
	if (uth->uu_procp == 0)
		return;

	server_thread_deregister(uth, p);

	if (p->p_flag & SWEXIT)
	    wakeup((caddr_t)&p->p_flag);

	return KERN_SUCCESS;
}

/*
 * Send the specified signal to the specified process.
 * Only allowed to block/sleep if can_block is TRUE.
 * When called from interrupt handlers, can_block must be FALSE.
 */
psignal3(p, sig, can_block)
	register struct proc *p;
	register int	sig;
	boolean_t can_block;
{
	register int	(*action)();
	int		mask;

	/* we must hold the master lock to block */
	if (can_block && (u.u_master_lock == 0))
		panic("psignal3");

	if (p->p_flag & SWEXIT)		/* XXX what locks this field??? */
	    return;

	if (can_block && (sig == SIGKILL)) {
	    /*
	     *	No need to wait for the victim to notice the signal.
	     */

	    proc_exit(p, SIGKILL, FALSE);
	    return;
	}

	if (sig <= 0 || sig > NSIG)
	    return;
	mask = sigmask(sig);

	if (can_block)
	    proc_siglock(p);
	else if (!proc_sigtrylock(p)) {
	    /* send a message to a server thread, which can block */

	    mutex_lock(&p->p_lock);
	    p->p_sigref++;
	    mutex_unlock(&p->p_lock);

	    (void) ubsd_psignal_retry((mach_port_t) p, sig);
	    return;
	}

	if (p->p_sigmask & mask)
	    action = SIG_HOLD;
	else if (p->p_flag & STRC)
	    action = SIG_DFL;
	else if (p->p_sigignore & mask) {
	    proc_sigunlock(p);
	    return;
	}
	else if (p->p_sigcatch & mask)
	    action = SIG_CATCH;
	else
	    action = SIG_DFL;

	/*
	 * Save the signal in p_sig.
	 */
	p->p_sig |= mask;

	if (sig == SIGCONT)
	    p->p_sig &= ~stopsigmask;
	else if (mask & stopsigmask)
	    p->p_sig &= ~sigmask(SIGCONT);

	if (action == SIG_HOLD) {
	    proc_sigunlock(p);
	    return;
	}

	if (p->p_stat == SSTOP) {
	    /*
	     * If traced process is already stopped, no further
	     * action is necessary.
	     */
	    if ((p->p_flag & STRC) == 0) {
		switch (sig) {
		    case SIGKILL:
			/*
			 * Kill signal always sets processes running.
			 */
			start(p, 0);
			unsleep_proc(p);
			break;

		    case SIGCONT:
			/*
			 * If the process catches SIGCONT, let it handle
			 * the signal itself.  Otherwise, we un-stop it.
			 */
			start(p, 0);
			if (action != SIG_DFL)
			    unsleep_proc(p);
			break;

		    case SIGSTOP:
		    case SIGTSTP:
		    case SIGTTIN:
		    case SIGTTOU:
			/*
			 * Already stopped.  Do not need to stop again.
			 */
			p->p_sig &= ~mask;
			break;

		    default:
			/*
			 * If process is sleeping interruptibly, then
			 * unstick it so that when it is continued it
			 * can look at the signal.  But do not unstop
			 * as it is not to be resumed by the signal
			 * alone.
			 */
			unsleep_proc(p);
			break;
		}
	    }
	}
	else {
	    /*
	     * Awaken server threads for process if
	     * sleeping interruptibly.
	     * (wake them up, possibly just to take their
	     *  sleeping pills...)
	     */
	    unsleep_proc(p);
	}

	proc_sigunlock(p);
}

/*
 * Send the specified signal to the specified process.
 */
psignal(p, sig)
	register struct proc *p;
	register int	sig;
{
	psignal3(p, sig, TRUE);
}

/*
 * Wake up all server threads in a process.
 */
unsleep_proc(p)
	register struct proc *p;
{
	register uthread_t	uth;

	mutex_lock(&p->p_lock);
	for (uth = (uthread_t)queue_first(&p->p_servers);
	     !queue_end(&p->p_servers, (queue_entry_t)uth);
	     uth = (uthread_t)queue_next(&uth->uu_server_list))
	    unsleep(uth);
	mutex_unlock(&p->p_lock);
}

/*
 * Check for signals, returning TRUE if there are any to handle.
 * The pending signal is left in p->p_cursig.
 *
 * The thread is not on any sleep queues.
 */
boolean_t
issig()
{
	register struct proc *p = u.u_procp;

	register int	sig;
	int		sigbits, mask;

	/*
	 * If process is stopped, sleep.  If process is
	 * exiting, bail out.
	 */
	while (p->p_stat == SSTOP || (p->p_flag & SWEXIT)) {
	    if (p->p_flag & SWEXIT) {
		/*
		 * Bail out
		 */
#if	SIG_DEBUG
 printf("issig([%d]): exiting\n", p->p_pid);
#endif	SIG_DEBUG
		return (TRUE);
	    }
#if	SIG_DEBUG
 printf("issig([%d]): stopping for other thread\n", p->p_pid);
#endif	SIG_DEBUG
	    sleep((caddr_t)&p->p_stat, PSPECL);
	}

	for (;;) {
	    proc_siglock(p);

	    sigbits = p->p_sig &~ p->p_sigmask;
	    if ((p->p_flag & STRC) == 0)
		sigbits &= ~p->p_sigignore;
#if	VICE
	    if (p->p_flag & SRMT)
		sigbits &= ~stopsigmask;
#endif	VICE
	    if (sigbits == 0) {
		proc_sigunlock(p);
		break;
	    }

	    sig = ffs((long)sigbits);
	    mask = sigmask(sig);

	    p->p_sig &= ~mask;
	    proc_sigunlock(p);

	    p->p_cursig = sig;	/* take the signal */

	    if ((p->p_flag & STRC) != 0
#if	VICE
		&& (p->p_flag & SRMT) == 0
#endif	VICE
	    ) {
		/*
		 * If traced, always stop, and stay stopped until
		 * released by the parent.
		 */
#if	SIG_DEBUG
 printf("issig([%d]): stopping for trace\n", p->p_pid);
#endif	SIG_DEBUG
		stop(p, MACH_PORT_NULL, sig, 0);
		sleep((caddr_t)&p->p_stat, PSPECL);

		if (p->p_flag & SWEXIT) {
#if	SIG_DEBUG
 printf("issig([%d]): exiting after trace\n", p->p_pid);
#endif	SIG_DEBUG
		    return (TRUE);
		}

		/*
		 * If the traced bit was turned off, then put the signal
		 * taken above back into p_sig and rescan signals.  This
		 * ensures that p_sig* and u_signal are consistent.
		 */
		if ((p->p_flag & STRC) == 0) {
		    proc_siglock(p);
		    p->p_sig |= mask;
		    proc_sigunlock(p);
		    continue;
		}

		/*
		 * If parent wants us to take the signal, then it
		 * will leave it in p->p_cursig;  otherwise we
		 * just look for signals again.
		 */
		sig = p->p_cursig;
		if (sig == 0)
		    continue;

		/*
		 * If signal is masked put it back into p_sig and look
		 * for other signals.
		 */
		mask = sigmask(sig);
		if (p->p_sigmask & mask) {
		    proc_siglock(p);
		    p->p_sig |= mask;
		    proc_sigunlock(p);
		    continue;
		}
	    }

	    switch ((int)u.u_signal[sig]) {
		case SIG_DFL:
		    /*
		     * Do not take default actions on system
		     * processes.
		     */
		    if (p->p_ppid == 0)
			break;
		    switch (sig) {
			case SIGTSTP:
			case SIGTTIN:
			case SIGTTOU:
			    /*
			     * Children of init are not allowed to stop
			     * on signals from the keyboard.
			     */
			    if (p->p_pptr == &proc[1]) {
				psignal3(p, SIGKILL, FALSE);
				continue;
			    }
			    /* fall into ... */
			case SIGSTOP:
			    if (p->p_flag & STRC)
				continue;	/* already stopped for trace */

			    stop(p, MACH_PORT_NULL, sig, 0);
#if	SIG_DEBUG
 printf("issig([%d]): stopping for STOP signal\n", p->p_pid);
#endif	SIG_DEBUG
			    sleep((caddr_t)&p->p_stat, PSPECL);

			    if (p->p_flag & SWEXIT) {
#if	SIG_DEBUG
 printf("issig([%d]): exiting after STOP signal\n", p->p_pid);
#endif	SIG_DEBUG
				return (TRUE);
			    }
			    continue;

			case SIGCONT:
			case SIGCHLD:
			case SIGURG:
			case SIGIO:
			case SIGWINCH:
			    continue;

			default:
			    goto send;
		    }
		    /*NOTREACHED*/

		case SIG_HOLD:
		case SIG_IGN:
		    /*
		     * Masking above should prevent us ever trying
		     * to take action on a held or ignored signal,
		     * unless the process is traced.
		     */
		    if ((p->p_flag & STRC) == 0)
			panic("issig");
		    continue;

		default:
		    /*
		     * This signal has an action.  Let psig process it.
		     */
		    goto send;
	    }
	    /*NOTREACHED*/
	}

	/*
	 * No signal to send.
	 */
	p->p_cursig = 0;
#if	SIG_DEBUG
 printf("issig([%d]): no signals\n", p->p_pid);
#endif	SIG_DEBUG
	return (FALSE);

    send:
	/*
	 * Let psig process the signal.
	 */
#if	SIG_DEBUG
 printf("issig([%d]): have signal %d\n", p->p_pid, p->p_cursig);
#endif	SIG_DEBUG
	return (TRUE);
}

/*
 * Stop a process.
 */
stop(p, thread, sig, code)
	register struct proc *p;
	thread_t	thread;
	int		sig, code;
{
	(void) task_suspend(p->p_task);
	p->p_stat = SSTOP;
	p->p_flag &= ~SWTED;
	if (thread != MACH_PORT_NULL)
	    p->p_thread = thread;
	p->p_stopsig = sig;
	p->p_stopcode = code;
	psignal(p->p_pptr, SIGCHLD);
	wakeup((caddr_t)p->p_pptr);
}

/*
 * Restart a process, optionally giving it a signal to take.
 * This is called from psignal, and thus should be protected...
 * but psignal calls it only for SIGCONT or SIGKILL, which
 * are never sent from interrupt level.
 */
start(p, sig)
	register struct proc *p;
	int	sig;
{
#if	SIG_DEBUG
  printf("Starting [%d] signal %d\n", p->p_pid, sig);
#endif	SIG_DEBUG
	p->p_stat = SRUN;
	p->p_stopsig = 0;
	p->p_stopcode = 0;
	p->p_cursig = sig;
	wakeup((caddr_t)&p->p_stat);
	(void) task_resume(p->p_task);
}

/*
 * Perform the action specified by
 * the current signal.
 * The usual sequence is:
 *	if (issig())
 *		psig();
 * The signal bit has already been cleared by issig,
 * and the current signal number stored in p->p_cursig.
 */
psig()
{
	register struct proc *p = u.u_procp;
	register int	sig;
	register int	(*action)();

	while (p->p_stat == SSTOP || (p->p_flag & SWEXIT)) {
	    if (p->p_flag & SWEXIT) {
#if	SIG_DEBUG
  printf("psig([%d]) exiting\n", p->p_pid);
#endif	SIG_DEBUG
		return;
	    }
#if	SIG_DEBUG
  printf("psig([%d]) sleeping for other thread\n", p->p_pid);
#endif	SIG_DEBUG
	    sleep((caddr_t)&p->p_stat, PSPECL);
	}

	sig = p->p_cursig;

	/*
	 *	If another thread got here first (sig == 0) or this is
	 *	a thread signal for another thread, bail out.
	 */
	if (sig == 0) {
#if	SIG_DEBUG
  printf("psig([%d]) no signal\n", p->p_pid);
#endif	SIG_DEBUG
	    return;
	}

	/*
	 *  A polled resource pause condition which is being retried from the
	 *  system call level may be interrupted on the way back out to user
	 *  mode to be retried with a pause diagnostic message pending.  Always
	 *  clear the condition here before processing an interrupting signal
	 *  to keep the pause/continue diagnostic messages paired.
	 */
	if (u.u_rpswhich&URPW_NOTIFY)
	    rpcont();

	action = u.u_signal[sig];
	if (action != SIG_DFL) {

	    /*
	     * User handles sending himself signals.
	     */
#if	SIG_DEBUG
  printf("psig([%d]) CATCH signal %d\n", p->p_pid, sig);
#endif	SIG_DEBUG
	    return;
	}

	p->p_cursig = 0;
#if	SIG_DEBUG
  printf("psig([%d]) taking signal %d\n", p->p_pid, sig);
#endif	SIG_DEBUG
	sig_default(sig);
}

/*
 * Take default action on signal.
 */
sig_default(sig)
	register int	sig;
{
	register struct proc *p = u.u_procp;
	boolean_t	dump;

	switch (sig) {
	    /*
	     *	The new signal code for multiple threads makes it possible
	     *	for a multi-threaded task to get here (a thread that didn`t
	     *	originally process a "stop" signal notices that cursig is
	     *	set), therefore, we must handle this.
	     */
	    case SIGIO:
	    case SIGURG:
	    case SIGCHLD:
	    case SIGCONT:
	    case SIGWINCH:
	    case SIGTSTP:
	    case SIGTTIN:
	    case SIGTTOU:
	    case SIGSTOP:
		return;

	    case SIGILL:
	    case SIGIOT:
	    case SIGBUS:
	    case SIGQUIT:
	    case SIGTRAP:
	    case SIGEMT:
	    case SIGFPE:
	    case SIGSEGV:
	    case SIGSYS:
		/*
		 * Kill the process with a core dump.
		 */
		dump = TRUE;
		break;
	
	    default:
		dump = FALSE;
		break;
	}

	p->p_stopsig = sig;
	p->p_stopcode = 0;
	p->p_utask.uu_acflag |= AXSIG;	/* exited w/signal */
	proc_exit(p, sig, dump);
}

/*
 * New thread_psignal.  Runs as part of the normal service - thus
 * we have a thread per user exception, so it can wait.
 */
thread_psignal(task, thread, sig, code)
	task_t		task;
	thread_t	thread;
	register int	sig;
	int		code;
{
	register struct proc *p = task_to_proc_lookup(task);
	register uthread_t	uth = &u;

	if (p == 0) {
	    (void) task_terminate(task);
	    return;
	}
	if (sig < 0 || sig > NSIG)
	    return;

	/*
	 * Register thread as service thread
	 */
	server_thread_register(uth, p);
	uarea_init(uth, p);
	unix_master();

	thread_signal(p, thread, sig, code);

	unix_release();

	/*
	 * If thread_signal killed the process, then proc_exit
	 * might have already deregistered us.
	 */
	if (uth->uu_procp == 0)
		return;

	server_thread_deregister(uth, p);

	if (p->p_flag & SWEXIT)
	    wakeup((caddr_t)&p->p_flag);
}

thread_signal(p, thread, sig, code)
	register struct proc *p;
	thread_t	thread;
	register int	sig;
	int		code;
{
	register int	mask;

	mask = sigmask(sig);

	for (;;) {
	    if (p->p_sigmask & mask) {
		/*
		 * Save the signal in p_sig.
		 */
		p->p_sig |= mask;
		return;
	    }

	    while (p->p_stat == SSTOP || (p->p_flag & SWEXIT)) {
		if (p->p_flag & SWEXIT) {
		    return;
		}
		sleep((caddr_t)&p->p_stat, PSPECL);
	    }
	    if (p->p_sigmask & mask) {
		/* could have changed */
		continue;
	    }

	    if (p->p_flag & STRC) {
		/*
		 * Stop for trace.
		 */
		stop(p, thread, sig, code);
		sleep((caddr_t)&p->p_stat, PSPECL);

		if (p->p_flag & SWEXIT) {
		    return;
		}

		if ((p->p_flag & STRC) == 0) {
		    continue;
		}

		sig = p->p_cursig;
		if (sig == 0) {
		    return;
		}

		mask = sigmask(sig);
		if (p->p_sigmask & mask) {
		    continue;
		}
	    }
	    break;
	}

	switch ((int)u.u_signal[sig]) {
	    case SIG_IGN:
	    case SIG_HOLD:
		/*
		 * Should not get here unless traced.
		 */
		break;

	    case SIG_DFL:
		sig_default(sig);
		break;

	    default:
		/*
		 * Send signal to user thread.
		 */
		send_signal(p, thread, sig, code);
		break;
	}
}

/*
 * Unblock thread signal if it was masked off.
 */
check_proc_signals(p)
	register struct proc *p;
{
	wakeup((caddr_t)&p->p_sigmask);
}

/*
 * Send the signal to a thread.  The thread must not be executing
 * any kernel calls.
 */
send_signal(p, thread, sig, code)
	register struct proc *p;
	thread_t	thread;
	register int	sig;
	int		code;
{
	register int	mask;
	int	returnmask;
	int	(*action)();

	action = p->p_utask.uu_signal[sig];
	mask = sigmask(sig);

	/*
	 * At this point, thread is in either exception_raise,
	 * or emulator::take_signal, waiting for a reply message.
	 * There should be no need to suspend it.
	 */

	/*
	 * Signal action may have changed while we blocked for
	 * task_suspend or thread_abort.  Check it again.
	 */
	proc_siglock(p);

	if ((p->p_sigcatch & mask) == 0) {
	    /*
	     * No longer catching signal.  Put it back if
	     * not ignoring it, and start up the task again.
	     */
	    proc_sigunlock(p);
	    psignal(p, sig);
	    (void) task_resume(p->p_task);
	    return;
	}
	if (p->p_flag & SOUSIG) {
	    if (sig != SIGILL && sig != SIGTRAP) {
		p->p_utask.uu_signal[sig] = SIG_DFL;
		p->p_sigcatch &= ~mask;
	    }
	    mask = 0;
	}
	if (p->p_flag & SOMASK) {
	    returnmask = p->p_utask.uu_oldmask;
	    p->p_flag &= ~SOMASK;
	}
	else
	    returnmask = p->p_sigmask;
	p->p_sigmask |= p->p_utask.uu_sigmask[sig] | mask;
	proc_sigunlock(p);

	sendsig(p, thread, action, sig, code, returnmask);

	/*
	 * At this point, thread is in either exception_raise,
	 * or emulator::take_signal, waiting for a reply message.
	 * The reply message should resume it.
	 */
}

/*
 * Called by user to take a pending signal.
 */
int
bsd_take_signal(proc_port, interrupt, old_mask, old_onstack, o_sig, o_code,
		o_handler, new_sp)
	mach_port_t	proc_port;
	boolean_t	*interrupt;	/* out */
	int		*old_mask;	/* out */
	int		*old_onstack;	/* out */
	int		*o_sig;		/* out */
	int		*o_code;	/* out */
	int		*o_handler;	/* out */
	int		*new_sp;	/* out */
{
	register struct proc *	p;
	register int	error;
	register int	sig, mask, sigbits;
	register int	(*action)();
	int		returnmask;
	int		oonstack;

	error = start_server_op(proc_port, 1000);
		/* XXX code for take-signal */
	if (error)
	    return (error);

	unix_master();

	p = u.u_procp;
	/*
	 * Process should be running.
	 * Should not get here if process is stopped.
	 */
	if (p->p_stat != SRUN || (p->p_flag & SWEXIT)) {
#if	SIG_DEBUG
  printf("Handle_signals([%d]): not SRUN (%d)\n", p->p_pid, p->p_stat);
#endif	SIG_DEBUG
	    unix_release();
	    return (end_server_op(ESRCH, interrupt));
	}

	/*
	 * Set up return values in case no signals pending.
	 */
	*old_mask = 0;
	*old_onstack = 0;
	*o_sig = 0;
	*o_code = 0;
	*o_handler = 0;
	*new_sp = 0;

	/*
	 * Get pending signal.
	 */
	sig = p->p_cursig;
	if (sig != 0) {
	    p->p_cursig = 0;
	}
	else {
	    /*
	     * If no pending signal, get from signal masks.
	     * Yes, this can happen.
	     */
	    proc_siglock(p);

	    sigbits = p->p_sig &~ p->p_sigmask;
	    if ((p->p_flag & STRC) == 0)
		sigbits &= ~p->p_sigignore;
#if	VICE
	    if (p->p_flag & SRMT)
		sigbits &= ~stopsigmask;
#endif	VICE
	    if (sigbits != 0) {
		sig = ffs((long)sigbits);
		p->p_sig &= ~sigmask(sig);

		/*
		 * Do not deal with per-thread signals -
		 * they should not get here.
		 */
	    }
	    proc_sigunlock(p);
	}
	if (sig == 0) {
	    /*
	     * No signals - return to user.
	     */
	    unix_release();
	    return (end_server_op(0, interrupt));
	}

	action = p->p_utask.uu_signal[sig];
	switch ((int)action) {
	    case SIG_IGN:
	    case SIG_HOLD:
		/*
		 * Should not get here.
		 */
		sig = 0;
		break;

	    case SIG_DFL:
		/*
		 * take default action
		 */
		sig_default(sig);
		sig = 0;
		break;

	    default:
		/*
		 * user gets signal
		 */
		mask = sigmask(sig);

		proc_siglock(p);

		if ((p->p_sigcatch & mask) == 0) {
		    proc_sigunlock(p);
		    psignal(p, sig);
		    sig = 0;
		    break;
		}
		if (p->p_flag & SOUSIG) {
		    if (sig != SIGILL && sig != SIGTRAP) {
			p->p_utask.uu_signal[sig] = SIG_DFL;
			p->p_sigcatch &= ~mask;
		    }
		    mask = 0;
		}
		if (p->p_flag & SOMASK) {
		    returnmask = p->p_utask.uu_oldmask;
		    p->p_flag &= ~SOMASK;
		}
		else
		    returnmask = p->p_sigmask;
		p->p_sigmask |= p->p_utask.uu_sigmask[sig] | mask;
		proc_sigunlock(p);
#if	MAP_UAREA
		share_lock(&p->p_shared_rw->us_lock, p);
#endif	MAP_UAREA
		oonstack = p->p_utask.u_sigstack.ss_onstack;
		if (!oonstack && (p->p_utask.uu_sigonstack & mask)) {
		    *new_sp = (int) p->p_utask.u_sigstack.ss_sp;
		    p->p_utask.u_sigstack.ss_onstack = 1;
		}
		else
		    *new_sp = 0;	/* use existing stack */

#if	MAP_UAREA
		share_unlock(&p->p_shared_rw->us_lock, p);
#endif	MAP_UAREA
		*old_mask = returnmask;
		*old_onstack = oonstack;
		*o_sig = sig;
		*o_code = 0;
		*o_handler = (int)action;
		break;
	}

	unix_release();
	return (end_server_op(0, interrupt));
}

int
bsd_sigreturn(proc_port, interrupt, old_on_stack, old_sigmask)
	mach_port_t	proc_port;
	boolean_t	*interrupt;
	int		old_on_stack;
	int		old_sigmask;
{
	register int error;
	register struct proc *p;

	error = start_server_op(proc_port, SYS_sigreturn);
	if (error)
	    return (error);

	unix_master();

	p = u.u_procp;

#if	MAP_UAREA
	share_lock(&p->p_shared_rw->us_lock, p);
#endif	MAP_UAREA
	p->p_utask.u_sigstack.ss_onstack = old_on_stack & 01;
#if	MAP_UAREA
	share_unlock(&p->p_shared_rw->us_lock, p);
#endif	MAP_UAREA

	proc_siglock(p);
	p->p_sigmask = old_sigmask & ~cantmask;
	proc_sigunlock(p);

	check_proc_signals(p);

	unix_release();

	return (end_server_op(0, interrupt));
}
