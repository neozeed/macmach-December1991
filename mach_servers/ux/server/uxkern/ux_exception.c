/* 
 * Mach Operating System
 * Copyright (c) 1987 Carnegie-Mellon University
 * All rights reserved.  The CMU software License Agreement specifies
 * the terms and conditions for use and redistribution.
 */

/*
 *********************************************************************
 * HISTORY
 * $Log:	ux_exception.c,v $
 * Revision 2.3  90/06/02  15:28:54  rpd
 * 	Converted to new IPC.
 * 	Deallocate task and thread ports.
 * 	Added EXC_UNIX_ABORT.
 * 	[90/03/26  20:27:02  rpd]
 * 
 * Revision 2.2  89/10/17  11:27:35  rwd
 * 	Remove separate ux_exception thread.  Handle exception messages
 * 	by normal server threads.
 * 	[89/09/21            dbg]
 * 
 * 	Out-of-kernel version (NO CONDITIONALS!).  Runs in same task as
 * 	rest of UX kernel.
 * 	[89/01/06            dbg]
 * 
 * Revision 2.1  89/08/04  14:50:25  rwd
 * Created.
 * 
 * Revision 2.6  88/09/25  22:16:55  rpd
 * 	Changed includes to the new style.
 * 	Changed to use object_copyin instead of port_copyin,
 * 	and eliminated use of PORT_INVALID.
 * 	[88/09/24  18:14:52  rpd]
 * 
 * Revision 2.5  88/08/06  19:22:38  rpd
 * Eliminated use of kern/mach_ipc_defs.h.
 * 
 * Revision 2.4  88/07/20  21:08:20  rpd
 * Modified to use a port set to eat notifications; this fixes a message leak.
 * Also will now send error messages in response to bogus requests.
 * 
 * Revision 2.3  88/07/17  19:30:39  mwyoung
 * Change use of kernel_only to new kernel_vm_space.
 *
 * 10-Mar-88  David Black (dlb) at Carnegie-Mellon University
 *	Check error returns from port_copyin().
 *
 * 29-Dec-87  David Golub (dbg) at Carnegie-Mellon University
 *	Delinted.
 *
 *  8-Dec-87  David Black (dlb) at Carnegie-Mellon University
 *	Rewrite to use local port names and internal kernel rpcs.
 *
 *  4-Dec-87  David Black (dlb) at Carnegie-Mellon University
 *	Update to exc interface.  Set ipc_kernel in handler thread.
 *	Deallocate thread/task references now returned from convert
 *	routines.
 *
 * 30-Nov-87  David Black (dlb) at Carnegie-Mellon University
 *	Split unix-dependent stuff into this separate file.
 *
 * 30-Oct-87  David Black (dlb) at Carnegie-Mellon University
 *	Get port references right.
 *
 * 19-Oct-87  David Black (dlb) at Carnegie-Mellon University
 *	Removed port_copyout to kernel_task.  mach_ipc.c has been fixed
 *	to allow kernel to send to any port.
 *
 *  1-Oct-87  David Black (dlb) at Carnegie-Mellon University
 *	Created
 *
 **********************************************************************
 */

#include <sys/param.h>
#include <sys/user.h>

#include <mach/exception.h>
#include <sys/ux_exception.h>

#include <uxkern/import_mach.h>

/*
 *	Unix exception handler.
 */

any_t		ux_handler();	/* forward */
extern void	ux_create_thread();

static mach_port_t	ux_local_port;

/*
 * Returns exception port to map exceptions to signals.
 */
mach_port_t	ux_handler_setup()
{
	register kern_return_t	r;

	/*
	 *	Allocate the exception port.
	 */
	r = mach_port_allocate(mach_task_self(), MACH_PORT_RIGHT_RECEIVE,
			       &ux_local_port);
	if (r != KERN_SUCCESS)
		panic("ux_handler_setup: can't allocate");

	r = mach_port_insert_right(mach_task_self(),
				   ux_local_port, ux_local_port,
				   MACH_MSG_TYPE_MAKE_SEND);
	if (r != KERN_SUCCESS)
		panic("ux_handler_setup: can't acquire send right");

	/*
	 * Add it to the server port set.
	 */
	ux_server_add_port(ux_local_port);

	/*
	 * Return the exception port.
	 */

	return (ux_local_port);
}

void	ux_exception();	/* forward */

kern_return_t
catch_exception_raise(exception_port, thread, task,
	exception, code, subcode)
	mach_port_t	exception_port;
	thread_t	thread;
	task_t		task;
	int		exception, code, subcode;
{
	int	signal = 0, ux_code = 0;
	int	ret = KERN_SUCCESS;

#ifdef	lint
	exception_port++;
#endif	lint

	/*
	 *	Catch bogus ports
	 */
	if (MACH_PORT_VALID(task) && MACH_PORT_VALID(thread)) {

#ifdef mac2 /* catch EFAULT inside emulator */
	    if (exception != EXC_BAD_ACCESS
		|| !machine_emulator_fault(thread)) {
		/*
		 *	Convert exception to unix signal and code.
		 */
		ux_exception(exception, code, subcode, &signal, &ux_code);

		/*
		 *	Send signal.
		 */
		if (signal != 0)
		    thread_psignal(task, thread, signal, ux_code);
	    }
#else
	    /*
	     *	Convert exception to unix signal and code.
	     */
	    ux_exception(exception, code, subcode, &signal, &ux_code);

	    /*
	     *	Send signal.
	     */
	    if (signal != 0) {
		thread_psignal(task, thread, signal, ux_code);
	    }
#endif
	} else {
	    printf("catch_exception_raise: task %x thread %x\n",
		   task, thread);
	    ret = KERN_INVALID_ARGUMENT;
	}

	if (MACH_PORT_VALID(task))
		(void) mach_port_deallocate(mach_task_self(), task);

	if (MACH_PORT_VALID(thread))
		(void) mach_port_deallocate(mach_task_self(), thread);

	return(ret);
}

extern boolean_t	machine_exception();

/*
 *	ux_exception translates a mach exception, code and subcode to
 *	a signal and u.u_code.  Calls machine_exception (machine dependent)
 *	to attempt translation first.
 */

void ux_exception(exception, code, subcode, ux_signal, ux_code)
int	exception, code, subcode;
int	*ux_signal, *ux_code;
{
	/*
	 *	Try machine-dependent translation first.
	 */
	if (machine_exception(exception, code, subcode, ux_signal, 
	    ux_code))
		return;
	
	switch(exception) {

	    case EXC_BAD_ACCESS:
		if (code == KERN_INVALID_ADDRESS)
		    *ux_signal = SIGSEGV;
		else
		    *ux_signal = SIGBUS;
		break;

	    case EXC_BAD_INSTRUCTION:
	        *ux_signal = SIGILL;
		break;

	    case EXC_ARITHMETIC:
	        *ux_signal = SIGFPE;
		break;

	    case EXC_EMULATION:
		*ux_signal = SIGEMT;
		break;

	    case EXC_SOFTWARE:
		switch (code) {
		    case EXC_UNIX_BAD_SYSCALL:
			*ux_signal = SIGSYS;
			break;
		    case EXC_UNIX_BAD_PIPE:
		    	*ux_signal = SIGPIPE;
			break;
		    case EXC_UNIX_ABORT:
			*ux_signal = SIGABRT;
			break;
		}
		break;

	    case EXC_BREAKPOINT:
		*ux_signal = SIGTRAP;
		break;
	}
}
