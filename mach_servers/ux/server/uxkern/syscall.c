/* 
 * Mach Operating System
 * Copyright (c) 1988 Carnegie-Mellon University
 * All rights reserved.  The CMU software License Agreement specifies
 * the terms and conditions for use and redistribution.
 */
/*
 * HISTORY
 * $Log:	syscall.c,v $
 * Revision 2.10  91/03/20  14:36:03  dbg
 * 	Include syscalltrace.h control file, else we do a half job.
 * 	[91/02/21  11:47:54  af]
 * 
 * Revision 2.9  90/11/05  16:58:23  rpd
 * 	Make work w/o MAP_UAREA.
 * 	[90/11/01            rwd]
 * 
 * Revision 2.8  90/08/06  15:35:11  rwd
 * 	Added share lock panic.
 * 	[90/07/05            rwd]
 * 
 * Revision 2.7  90/06/19  23:16:10  rpd
 * 	Purged CMUCS conditionals.
 * 	[90/06/08            rpd]
 * 
 * Revision 2.6  90/06/02  15:28:22  rpd
 * 	Revamped uu_reply_msg handling in ux_generic_server.
 * 	[90/04/27            rpd]
 * 	Converted to new IPC.
 * 	[90/03/26  20:23:01  rpd]
 * 
 * Revision 2.5  90/05/29  20:25:19  rwd
 * 	Added panic for attempt to exit while holding the master lock.
 * 	[90/05/09            rwd]
 * 
 * Revision 2.4  90/01/23  00:05:33  af
 * 	Do not initialize syscalltrace here, or else we conflict
 * 	with its other definition in syscall_subr.c
 * 	[90/01/20  23:30:08  af]
 * 
 * Revision 2.3  89/11/29  15:31:35  af
 * 	Added rval2, because some syscalls do not modify it and it
 * 	must be preserved.
 * 	[89/11/20            af]
 * 	Use "extern" for decls of external variables.
 * 	[89/11/16  15:31:02  af]
 * 
 * Revision 2.2  89/10/17  11:27:12  rwd
 * 	Add interrupt return parameter.
 * 	[89/09/21            dbg]
 * 
 */

#include <uxkern/import_mach.h>

#include <sys/param.h>
#include <sys/types.h>
#include <sys/user.h>
#include <sys/proc.h>
#include <sys/systm.h>
#include <sys/parallel.h>

#include <uxkern/bsd_msg.h>
#include <uxkern/syscalltrace.h>

extern struct sysent	cmusysent[];
extern int		ncmusysent;

extern struct sysent	sysent[];
int			nsysent;

#ifdef	SYSCALLTRACE
int		syscalltrace;		/* no processes */
extern char	*syscallnames[];
#endif

/*
 * Generic UX server message handler.
 * Generic system call.
 */
boolean_t
ux_generic_server(InHeadP, OutHeadP)
	mach_msg_header_t *InHeadP, *OutHeadP;
{
	register struct uthread		*uth = &u;
	register struct bsd_request	*req = (struct bsd_request *)InHeadP;
	register struct bsd_reply	*rep = (struct bsd_reply *)OutHeadP;
	register struct sysent	*callp;
	register int		syscode = req->syscode;
	int			reply_size;
#ifdef	SYSCALLTRACE
	int			pid;
#endif	SYSCALLTRACE

	/*
	 * Fix the reply message.
	 */
	static mach_msg_type_t bsd_rep_int_type = {
	    /* msgt_name */		MACH_MSG_TYPE_INTEGER_32,
	    /* msgt_size */		32,
	    /* msgt_number */		4,
	    /* msgt_inline */		TRUE,
	    /* msgt_longform */		FALSE,
	    /* msgt_deallocate */	FALSE,
	    /* msgt_unused */		0
	};

	/*
	 * Set up standard reply.
	 */
	rep->hdr.msgh_bits =
		MACH_MSGH_BITS(MACH_MSGH_BITS_REMOTE(InHeadP->msgh_bits), 0);
	rep->hdr.msgh_remote_port = InHeadP->msgh_remote_port;
	rep->hdr.msgh_local_port = MACH_PORT_NULL;
	rep->hdr.msgh_kind = InHeadP->msgh_kind;
	rep->hdr.msgh_id = InHeadP->msgh_id + 100;

	if (InHeadP->msgh_id != BSD_REQ_MSG_ID) {
	    static mach_msg_type_t RetCodeType = {
	        /* msgt_name */			MACH_MSG_TYPE_INTEGER_32,
	     	/* msgt_size */			32,
		/* msgt_number */		1,
	    	/* msgt_inline */		TRUE,
	    	/* msgt_longform */		FALSE,
	    	/* msgt_deallocate */		FALSE,
		/* msgt_unused */		0
	    };
	    mig_reply_header_t *OutP = (mig_reply_header_t *)OutHeadP;
	    OutP->RetCodeType = RetCodeType;
	    OutP->RetCode = MIG_BAD_ID;
	    OutP->Head.msgh_size = sizeof *OutP;
	    return (FALSE);
	}

	rep->int_type = bsd_rep_int_type;
	rep->hdr.msgh_size = sizeof(struct bsd_reply);

	/*
	 * Set up server thread to handle process
	 */
	if ((rep->retcode = start_server_op(req->hdr.msgh_local_port,
					    req->syscode))
		!= KERN_SUCCESS)
	{
	    return (TRUE);
	}

	/*
	 * Save the reply msg and initialize current_output.
	 * The user_copy/user_reply_msg code uses them for copyout.
	 */
	uth->uu_reply_msg = &rep->hdr;
	uth->uu_current_size = 0;

	/*
	 * Point at arguments
	 */
	uth->uu_ap = req->arg;

	/*
	 * Init rval[]
	 */
	uth->uu_r.r_val2 = req->rval2;

	/*
	 * Find the system call table descriptor for this call.
	 */
	if (syscode < 0) {
	    if (-syscode > ncmusysent)
		callp = &sysent[0];
	    else
		callp = &cmusysent[syscode + ncmusysent];
	}
	else {
	    if (syscode >= nsysent)
		callp = &sysent[0];
	    else
		callp = &sysent[syscode];
	}

	/*
	 * Switch to master if needed.
	 */
	if (callp->sy_parallel == 0) {
	    unix_master();
	}

	/*
	 * Catch any signals.  If no other error and not restartable,
	 * return EINTR.
	 */
	if (setjmp(&uth->uu_qsave)) {
	    if (uth->uu_error == 0 && uth->uu_eosys != RESTARTSYS)
		uth->uu_error = EINTR;
	}
	else {
#ifdef	SYSCALLTRACE
	    pid = uth->uu_procp->p_pid;
	    if (syscalltrace &&
		    (syscalltrace == pid || syscalltrace < 0)) {

		register int	j;
		char		*cp;

		if (syscode >= nsysent ||
			syscode < 0)
		    printf("[%d]%d", pid, syscode);
		else
		    printf("[%d]%s", pid, syscallnames[syscode]);

		cp = "(";
		for (j = 0; j < callp->sy_narg; j++) {
		    printf("%s%x", cp, uth->uu_ap[j]);
		    cp = ", ";
		}
		if (j)
		    cp = ")";
		else
		    cp = "";
		printf("%s\n", cp);
	    }
#endif	SYSCALLTRACE

	    /*
	     * Do the system call.
	     */
	    (*callp->sy_call)();
	}

	/*
	 * Get off master if it was necessary to be on it.
	 */
	if (callp->sy_parallel == 0) {
	    unix_release();
	}

#if	MAP_UAREA
	if (uth->uu_share_lock_count) {
	    panic("Share lock still held", uth->uu_share_lock_count);
	    uth->uu_share_lock_count = 0;
	}
#endif	MAP_UAREA

	if (uth->uu_master_lock)
	    panic("Master still held", uth->uu_master_lock);

	/*
	 * Put return values back into message, and end the
	 * system call.
	 */
	rep->retcode = end_server_op(uth->uu_error, &rep->interrupt);
	rep->rval[0] = uth->uu_r.r_val1;
	rep->rval[1] = uth->uu_r.r_val2;

#ifdef	SYSCALLTRACE
	if (syscalltrace &&
		(syscalltrace == pid || syscalltrace < 0)) {
	    if (syscode >= nsysent ||
		    syscode < 0)
		printf("    [%d]%d", pid, syscode);
	    else
		printf("    [%d]%s", pid, syscallnames[syscode]);
	    printf(" returns %d", rep->retcode);
	    if (uth->uu_eosys == NORMALRETURN)
		printf(" (%x,%x)", rep->rval[0], rep->rval[1]);
	    printf("%s\n",
		   (rep->interrupt) ? " Interrupt" : "");
	}
#endif	SYSCALLTRACE

	/*
	 * Wrap up any trailing data in the reply message.
	 */
	finish_reply_msg();

	return (TRUE);
}

/* 
 *  rpsleep - perform a resource pause sleep
 *
 *  rsleep = function to perform resource specific sleep
 *  arg1   = first function parameter
 *  arg2   = second function parameter
 *  mesg1  = first component of user pause message
 *  mesg2  = second component of user pause message
 *
 *  Display the appropriate pause message on the user's controlling terminal.
 *  Save the current non-local goto information and establish a new return
 *  environment to transfer here.  Invoke the supplied function to sleep
 *  (possibly interruptably) until the resource becomes available.  When the
 *  sleep finishes (either normally or abnormally via a non-local goto caused
 *  by a signal), restore the old return environment and display a resume
 *  message on the terminal.  The notify flag bit is set when the pause message
 *  is first printed.  If it is cleared on return from the function, the
 *  continue message is printed here.  If not, this bit will remain set for the
 *  duration of the polling process and the rpcont() routine will be called
 *  directly from the poller when the resource pause condition is no longer
 *  pending.
 *
 *  Return: true if the resource has now become available, or false if the wait
 *  was interrupted by a signal.
 */

boolean_t
rpsleep(rsleep, arg1, arg2, mesg1, mesg2)
int (*rsleep)();
int arg1;
int arg2;
char *mesg1;
char *mesg2;
{
    label_t lsave;
    boolean_t ret = TRUE;

    if ((u.u_rpswhich&URPW_NOTIFY) == 0)
    {
        u.u_rpswhich |= URPW_NOTIFY;
	uprintf("[%s: %s%s, pausing ...]\r\n", u.u_comm, mesg1, mesg2);
    }

    bcopy((caddr_t)&u.u_qsave, (caddr_t)&lsave, sizeof(lsave));
    if (setjmp(&u.u_qsave) == 0)
	(*rsleep)(arg1, arg2);
    else
	ret = FALSE;
    bcopy((caddr_t)&lsave, (caddr_t)&u.u_qsave, sizeof(lsave));

    if ((u.u_rpswhich&URPW_NOTIFY) == 0)
	rpcont();
    return(ret);
}


/* 
 *  rpcont - continue from resource pause sleep
 *
 *  Clear the notify flag and print the continuation message on the controlling
 *  terminal.  When this routine is called, the resource pause condition is no
 *  longer pending and we can afford to clear all bits since only the notify
 *  bit should be set to begin with.
 */

rpcont()
{
    u.u_rpswhich = 0;
    uprintf("[%s: ... continuing]\r\n", u.u_comm);
}


nosys()
{
	if (u.u_signal[SIGSYS] == SIG_IGN || u.u_signal[SIGSYS] == SIG_HOLD)
	    u.u_error = EINVAL;
	psignal(u.u_procp, SIGSYS);
}
