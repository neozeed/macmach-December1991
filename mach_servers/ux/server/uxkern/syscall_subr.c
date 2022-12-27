/* 
 * Mach Operating System
 * Copyright (c) 1989 Carnegie-Mellon University
 * All rights reserved.  The CMU software License Agreement specifies
 * the terms and conditions for use and redistribution.
 */
/*
 * HISTORY
 * $Log:	syscall_subr.c,v $
 * Revision 2.9  90/11/05  16:59:05  rpd
 * 	Fix to work w/o MAP_UAREA
 * 	[90/11/01            rwd]
 * 
 * Revision 2.8  90/08/06  15:35:20  rwd
 * 	Added share_lock panic.
 * 	[90/07/05            rwd]
 * 
 * Revision 2.7  90/06/19  23:16:15  rpd
 * 	Initialize uu_proc_exit in uarea_init.
 * 	[90/06/05            rpd]
 * 
 * Revision 2.6  90/06/02  15:28:28  rpd
 * 	Improved sanity checking in server_thread_deregister.
 * 	Initialize new uu_xxx field in uarea_init.
 * 	[90/05/23            rpd]
 * 
 * 	Removed uu_arg.
 * 	[90/05/13            rpd]
 * 
 * 	Added sanity checks to server_thread_register/server_thread_deregister.
 * 	Modified end_server_op to handle a null uu_procp.
 * 	[90/05/10            rpd]
 * 	Revised for new reply msg technology.
 * 	[90/04/27            rpd]
 * 	Converted to new IPC.
 * 	[90/03/26  20:23:18  rpd]
 * 
 * Revision 2.5  90/05/29  20:25:21  rwd
 * 	Added sanity checks to server_thread_register/server_thread_deregister.
 * 	Modified end_server_op to handle a null uu_procp.
 * 	[90/05/10            rpd]
 * 	Added panic for attempt to exit while holding the master lock.
 * 	[90/05/09            rwd]
 * 	Added check for us_closefile on all syscalls.
 * 	[90/04/23            rwd]
 * 	Don't check for signals on all syscalls > 1000.  These are all
 * 	special case communication calls.
 * 	[90/03/30            rwd]
 * 
 * Revision 2.4  89/11/15  13:27:44  dbg
 * 	end_server_op must call fspause() with master lock held.
 * 	[89/10/30            dbg]
 * 
 * Revision 2.3  89/10/17  11:27:18  rwd
 * 	Better signal/exit checks.  Remove check for pre-set
 * 	u_procp.
 * 	[89/09/21            dbg]
 * 
 * 	Add interrupt return parameter to all calls.  Set it in
 * 	end_server_op.
 * 	[89/09/21            dbg]
 * 	Add interrupt return parameter to all calls.  Set it in
 * 	end_server_op.
 * 	[89/09/21            dbg]
 * 
 * Revision 2.2  89/08/31  16:29:21  rwd
 * 	Check for signals at the beginning of all syscalls.  If there is
 * 	one, return ERESTART.  Added sctrace().
 * 	[89/08/21            rwd]
 * 
 *
 */
/*
 * Server thread service routines.
 */
#include <cmucs.h>
#include <cmucs_rfs.h>
#include <vice.h>

#include <uxkern/import_mach.h>

#include <sys/param.h>
#include <sys/user.h>
#include <sys/proc.h>
#include <sys/signal_macros.h>
#include <sys/parallel.h>

#include <uxkern/bsd_msg.h>
#include <uxkern/proc_to_task.h>

#include <uxkern/syscalltrace.h>

#ifdef	SYSCALLTRACE
extern int	nsysent;
int		syscalltrace = 0;
extern char	*syscallnames[];
#endif

/*
 * Register a server thread as serving a process.
 */
void
server_thread_register(uth, p)
	register uthread_t	uth;
	register struct proc *	p;
{
	if (uth->uu_procp != 0)
		panic("server_thread_register");

	mutex_lock(&p->p_lock);
	p->p_ref_count++;
	queue_enter(&p->p_servers, uth, uthread_t, uu_server_list);
	mutex_unlock(&p->p_lock);
}

/*
 * Unregister a server thread.
 */
void
server_thread_deregister(uth, p)
	register uthread_t	uth;
	register struct proc *	p;
{
	if (uth->uu_procp != p)
		panic("server_thread_deregister");

	mutex_lock(&p->p_lock);
	if ((p->p_ref_count <= 1) || queue_empty(&p->p_servers))
		panic("server_thread_deregister");
	queue_remove(&p->p_servers, uth, uthread_t, uu_server_list);
	if (--p->p_ref_count <= 0)
		panic("server_thread_deregister");
	mutex_unlock(&p->p_lock);

	uth->uu_procp = 0;
}

/*
 * Find a server thread currently serving a process.
 */
uthread_t
server_thread_find(p)
	register struct proc *p;
{
	register uthread_t	uth;

	mutex_lock(&p->p_lock);
	if (queue_empty(&p->p_servers))
	    uth = 0;
	else
	    uth = (uthread_t)queue_first(&p->p_servers);
	mutex_unlock(&p->p_lock);
	return (uth);
}

/*
 * Set up per-thread U area for the current thread.
 */
uarea_init(uth, p)
	register struct uthread *uth;
	struct proc *p;
{
	uth->uu_procp = p;
	uth->uu_proc_exit = FALSE;

	uth->uu_ap = 0;

	bzero((char *) uth->uu_xxx, sizeof uth->uu_xxx);
#if	MAP_UAREA
	uth->uu_share_lock_count = 0;
#endif	MAP_UAREA

	uth->uu_r.r_val1 = 0;
	uth->uu_r.r_val2 = 0;
	uth->uu_error = 0;
	uth->uu_eosys = NORMALRETURN;
	uth->uu_reply_msg = 0;

#if	CMUCS
	uth->uu_rpsfs = 0;
	uth->uu_rpswhich = 0;
#endif	CMUCS

#if	CMUCS_RFS
	uth->uu_rfsncnt = 0;
#endif	CMUCS_RFS

#if	VICE
	uth->uu_rmt_ncnt = 0;
#endif	VICE

	uth->uu_nd.ni_iov = &uth->uu_nd.ni_iovec;

	uth->uu_sb = 0;
	uth->uu_master_lock = 0;
}

/*
 * Start UX server call: map port to process, set up
 * U area.
 */
int
start_server_op(port, syscode)
	mach_port_t	port;
	int		syscode;
{
	register struct proc	*p;
	register struct uthread	*uth = &u;

	if ((p = port_to_proc_lookup(port)) == (struct proc *)0)
		return (ESRCH);
	if (p->p_flag & SWEXIT) return (MIG_NO_REPLY);
	if (p->p_cursig && syscode < 1000)
		return (ERESTART);

	server_thread_register(uth, p);
	uarea_init(uth, p);

#if	CMUCS_RFS
	uth->uu_rfscode = syscode;
#endif
#if	VICE
	uth->uu_rmt_code = syscode;
#endif
#if	CMUCS
	uth->uu_rpswhich &= URPW_NOTIFY;
#endif
#if	MAP_UAREA
	if (p->p_shared_rw->us_closefile != -1) {
	    int filenumber = p->p_shared_rw->us_closefile;
	    uth->uu_ap = &filenumber;
#ifdef	SYSCALLTRACE
	    if (syscalltrace &&
		(syscalltrace == p->p_pid || syscalltrace < 0)) {

		printf("[%d]piggyback close", p->p_pid);
	    }
#endif	SYSCALLTRACE
	    unix_master();
	    if (setjmp(&uth->uu_qsave)) {
		if (uth->uu_error == 0 && uth->uu_eosys != RESTARTSYS)
		    uth->uu_error = EINTR;
		unix_release();
		return(uth->uu_error);
	    }
	    close();
	    unix_release();
	    p->p_shared_rw->us_closefile = -1;
	    uth->uu_error = 0;
	}
#endif	MAP_UAREA
#ifdef	SYSCALLTRACE
	if (syscalltrace &&
		(syscalltrace == p->p_pid || syscalltrace < 0)) {

	    char *s;
	    char num[10];

	    if (syscode >= nsysent || syscode < 0) {
		sprintf(num, "%d", syscode);
		s = num;
	    }
	    else
		s = syscallnames[syscode];
	    printf("[%d]%s", p->p_pid, s);
	}
#endif	SYSCALLTRACE
	return (0);
}

int
end_server_op(error, interrupt)
	register int	error;
	boolean_t	*interrupt;
{
	register struct uthread *uth = &u;
	register struct proc	*p = uth->uu_procp;

	if (p == 0) {
	    if (uth->uu_master_lock)
		panic("Master still held", uth->uu_master_lock);

#if	MAP_UAREA
	    if (uth->uu_share_lock_count) {
		panic("Share lock still held", uth->uu_share_lock_count);
		uth->uu_share_lock_count = 0;
	    }
#endif	MAP_UAREA

	    return (MIG_NO_REPLY);
	}

#if	CMUCS || CMUCS_RFS || VICE
	if (error) {
	    switch (error) {
#if	CMUCS
		case EDQUOT:
		case ENOSPC:
		    uth->uu_error = error;
		    unix_master();
		    if (fspause(0))
			uth->uu_eosys = RESTARTSYS;
		    unix_release();
		    error = uth->uu_error;
		    break;
#endif	CMUCS
#if	CMUCS_RFS
		case EREMOTE:
		    error = 0;
		    break;
#endif	CMUCS_RFS
#if	VICE
		case EVICEOP:
		    error = 0;
		    break;
#endif	VICE
	    }
	}
#endif	CMUCS || CMUCS_RFS || VICE

	if (uth->uu_eosys == RESTARTSYS)
	    error = ERESTART;
	else if (uth->uu_eosys == JUSTRETURN)
	    error = MIG_NO_REPLY;

	*interrupt = FALSE;
	if (!EXITING(p)) {
	    if (p->p_cursig != 0 || HAVE_SIGNALS(p)) {
		unix_master();
		if (p->p_cursig != 0 || issig())
		    psig();
		if (p->p_cursig)	/* user should take signal */
		    *interrupt = TRUE;
		unix_release();

		/* psig might have killed us */
		if (uth->uu_procp == 0) {
		    if (uth->uu_master_lock)
			panic("Master still held", uth->uu_master_lock);

#if	MAP_UAREA
		    if (uth->uu_share_lock_count) {
			 panic("Share lock still held", uth->uu_share_lock_count);
			uth->uu_share_lock_count = 0;
		    }
#endif	MAP_UAREA


		    return (MIG_NO_REPLY);
		}
	    }
	}

#ifdef	SYSCALLTRACE
	if (syscalltrace &&
		(syscalltrace == p->p_pid || syscalltrace < 0)) {

	    printf("    [%d] returns %d%s\n",
		p->p_pid,
		error,
		(*interrupt) ? " Interrupt" : "");
	}
#endif

	server_thread_deregister(uth, p);

	if (p->p_flag & SWEXIT)
	    wakeup((caddr_t)&p->p_flag);

	if (uth->uu_master_lock)
	    panic("Master still held", uth->uu_master_lock);

#if	MAP_UAREA
	if (uth->uu_share_lock_count) {
	    panic("Share lock still held", uth->uu_share_lock_count);
	    uth->uu_share_lock_count = 0;
	}
#endif	MAP_UAREA

	return (error);
}

int sctrace()
{
#ifdef	SYSCALLTRACE
	struct a {
		int	value;
	} *uap = (struct a*)u.u_ap;

	syscalltrace = uap->value;
#endif SYSCALLTRACE
}
