/* 
 * Mach Operating System
 * Copyright (c) 1990 Carnegie-Mellon University
 * Copyright (c) 1987 Carnegie-Mellon University
 * All rights reserved.  The CMU software License Agreement specifies
 * the terms and conditions for use and redistribution.
 *
 * HISTORY
 * $Log:	kern_proc.c,v $
 * Revision 2.5  90/09/09  14:12:42  rpd
 * 	Fixed bsd_task_by_pid to return MACH_PORT_NULL after errors.
 * 	[90/08/23            rpd]
 * 
 * Revision 2.4  90/06/19  23:07:17  rpd
 * 	Added bsd_pid_by_task.
 * 	[90/06/14            rpd]
 * 
 * 	Purged MACH, MACH_NO_KERNEL, CMUCS conditionals.
 * 	Added special cases to bsd_task_by_pid to return
 * 	the privileged host port and device server port.
 * 	[90/06/13            rpd]
 * 
 * Revision 2.3  90/06/02  15:21:21  rpd
 * 	Made bsd_task_by_pid more robust: it now kills dead tasks.
 * 	[90/05/11            rpd]
 * 	Converted to new IPC.
 * 	Additional argument to bsd_task_by_pid.
 * 	[90/03/26  19:34:01  rpd]
 * 
 * Revision 2.2  89/10/17  11:24:51  rwd
 * 	Add interrupt return parameter to bsd_task_by_pid.
 * 	[89/09/21            dbg]
 * 
 * Revision 2.1  89/08/04  14:06:41  rwd
 * Created.
 * 
 * 19-Apr-89  David Golub (dbg) at Carnegie-Mellon University
 *	Added bsd_task_by_pid.
 *
 *  3-Jan-89  David Golub (dbg) at Carnegie-Mellon University
 *	Out-of-kernel version.
 *
 * 29-Mar-88  Michael Young (mwyoung) at Carnegie-Mellon University
 *	MACH: Removed use of "sys/vm.h".
 *
 * 17-Dec-86  David Golub (dbg) at Carnegie-Mellon University
 *	Removed include of text.h for MACH.
 *
 * 23-Oct-86  Jonathan J. Chew (jjc) at Carnegie-Mellon Univerity
 *	Moved mysterious reappearance of pte.h under else leg
 *	of CMUCS.
 *
 *  7-Jul-86  Bill Bolosky (bolosky) at Carnegie-Mellon University
 *	Removed unnecessary include of pte.h
 *
 * 14-Feb-86  Bill Bolosky (bolosky) at Carnegie-Mellon University
 *	Added switch ROMP around #includes "../machine/psl.h" so that
 *	the Sailboat doesn't gag on it.  (Question: why does the vax
 *	include it if it still compiles when we cut it out???)
 *
 */
/*
 * Copyright (c) 1982, 1986 Regents of the University of California.
 * All rights reserved.  The Berkeley software License Agreement
 * specifies the terms and conditions for redistribution.
 *
 *	@(#)kern_proc.c	7.1 (Berkeley) 6/5/86
 */

#include <sys/types.h>
#include <sys/param.h>
#include <sys/acct.h>
#include <sys/buf.h>
#include <sys/dir.h>
#include <sys/file.h>
#include <sys/inode.h>
#include <sys/kernel.h>
#include <sys/map.h>
#include <sys/mbuf.h>
#include <sys/parallel.h>
#include <sys/proc.h>
#include <sys/quota.h>
#include <sys/systm.h>
#include <sys/uio.h>
#include <sys/user.h>
#include <sys/wait.h>
#include <uxkern/proc_to_task.h>

/*
 * Clear any pending stops for top and all descendents.
 */
spgrp(top)
	struct proc *top;
{
	register struct proc *p;
	int f = 0;

	p = top;
	for (;;) {
		p->p_sig &=
			  ~(sigmask(SIGTSTP)|sigmask(SIGTTIN)|sigmask(SIGTTOU));
		f++;
		/*
		 * If this process has children, descend to them next,
		 * otherwise do any siblings, and if done with this level,
		 * follow back up the tree (but not past top).
		 */
		if (p->p_cptr)
			p = p->p_cptr;
		else if (p == top)
			return (f);
		else if (p->p_osptr)
			p = p->p_osptr;
		else for (;;) {
			p = p->p_pptr;
			if (p == top)
				return (f);
			if (p->p_osptr) {
				p = p->p_osptr;
				break;
			}
		}
	}
}

/*
 * Is p an inferior of the current process?
 */
inferior(p)
	register struct proc *p;
{

	for (; p != u.u_procp; p = p->p_pptr)
		if (p->p_ppid == 0)
			return (0);
	return (1);
}

struct proc *
pfind(pid)
	int pid;
{
	register struct proc *p;

	for (p = &proc[pidhash[PIDHASH(pid)]];
	     p != &proc[0];
	     p = &proc[p->p_idhash])
		if (p->p_pid == pid)
			return (p);
	return ((struct proc *)0);
}

/*
 * init the process queues
 */
pqinit()
{
	register struct proc *p;

	/*
	 * most procs are initially on freequeue
	 *	nb: we place them there in their "natural" order.
	 */

	freeproc = NULL;
	for (p = procNPROC; --p > proc; freeproc = p)
		p->p_nxt = freeproc;

	/*
	 * but proc[0] is special ...
	 */

	allproc = p;
	p->p_nxt = NULL;
	p->p_prev = &allproc;

	zombproc = NULL;
}

extern mach_port_t privileged_host_port;
extern mach_port_t device_server_port;

/*
 *	Routine:	bsd_task_by_pid
 *	Purpose:
 *		Get the task port for another "process", named by its
 *		process ID on the same host as "target_task".
 *
 *		Only permitted to privileged processes, or processes
 *		with the same user ID.
 */
int
bsd_task_by_pid(proc_port, interrupt, pid, t, tType)
	mach_port_t	proc_port;
	boolean_t	*interrupt;
	int		pid;
	task_t		*t;
	mach_msg_type_name_t *tType;
{
	register struct proc	*p;
	register int	error;

	if (error = start_server_op(proc_port, 1001))
	    return (error);

	unix_master();

	/* XXX groupmember(2) */

	if (pid == 0) {
		if (groupmember(2) || suser())
			*t = proc[0].p_task;	/* UX server */
		else
			error = EACCES;
	} else if (pid == -1) {
		if (groupmember(2) || suser())
			*t = privileged_host_port;
		else
			error = EACCES;
	} else if (pid == -2) {
		if (groupmember(2) || suser())
			*t = device_server_port;
		else
			error = EACCES;
	} else if (((p = pfind(pid)) != (struct proc *) 0) &&
		   (p->p_stat != SZOMB)) {
		if ((p->p_uid == u.u_uid) ||
		    groupmember(2) ||
		    suser())
			*t = p->p_task;
		else
			error = EACCES;
	} else
		error = ESRCH;

	if (!error) {
		kern_return_t kr;

		/*
		 * Give ourself another send right for the task port,
		 * and specify that the send right should be moved
		 * into the reply message.  This way there is no problem
		 * if the task port should be destroyed before the
		 * the reply message is sent.
		 */

		kr = mach_port_mod_refs(mach_task_self(), *t,
					MACH_PORT_RIGHT_SEND, 1);
		if (kr == KERN_SUCCESS) {
			*tType = MACH_MSG_TYPE_MOVE_SEND;
		} else if (kr == KERN_INVALID_RIGHT) {
			/* Probably a terminated task, so kill the proc. */
			proc_exit(p, SIGKILL, FALSE);
			error = ESRCH;
		} else {
			printf("pid %d -> task 0x%x; mod_refs returned %d\n",
				pid, *t, kr);
			panic("bsd_task_by_pid");
		}
	}

	unix_release();

	if (error) {
		/*
		 * If we can't produce a real send right,
		 * then give our caller MACH_PORT_NULL instead
		 * of a unix error code which he would probably
		 * mistake for a port.
		 */

		*t = MACH_PORT_NULL;
		*tType = MACH_MSG_TYPE_MOVE_SEND;
		error = 0;
	}

	return (end_server_op(error, interrupt));
}

int
bsd_pid_by_task(proc_port, interrupt, task, pidp, comm, commlen, rval)
	mach_port_t	proc_port;
	boolean_t	*interrupt;
	task_t		task;
	int		*pidp;
	char		*comm;
	int		*commlen;
	int		*rval;
{
	register struct proc *p;
	int len;

	unix_master();
	if (task == mach_task_self())
	    p = &proc[0];
	else
	    p = task_to_proc_lookup(task);
	if ((p == 0) || (p->p_flag & SWEXIT)) {
	    *pidp = 0;
	    *commlen = 0;
	    *rval = -1;
	} else {
	    *pidp = p->p_pid;
	    len = strlen(p->p_utask.uu_comm);
	    if (*commlen < len)
		len = *commlen;
	    bcopy(p->p_utask.uu_comm, comm, len);
	    *commlen = len;
	    *rval = 0;
	}
	unix_release();

	/* get rid of the send right */
	(void) mach_port_deallocate(mach_task_self(), task);

	*interrupt = FALSE;
	return ESUCCESS;
}
