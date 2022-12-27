/* 
 * Mach Operating System
 * Copyright (c) 1988 Carnegie-Mellon University
 * All rights reserved.  The CMU software License Agreement specifies
 * the terms and conditions for use and redistribution.
 */
/*
 * HISTORY
 * $Log:	syscall_subr.h,v $
 * Revision 2.3  90/06/02  15:28:33  rpd
 * 	Revised for new reply msg technology.
 * 	[90/04/27            rpd]
 * 	Converted to new IPC.
 * 	[90/03/26  20:23:33  rpd]
 * 
 * Revision 2.2  89/10/17  11:27:22  rwd
 * 	Added interrupt return parameter to all calls.
 * 	[89/09/21            dbg]
 * 
 */
/*
 * Glue to make MiG procedure interfaces look like old UX system calls.
 */
#include <sys/parallel.h>
#include <sys/user.h>
#include <sys/syscall.h>
#include <sys/errno.h>

#define	START_SERVER(syscode, nargs, serial)		\
	register uthread_t uth = &u;			\
	register int	error;				\
							\
	error = start_server_op(proc_port, syscode);	\
	if (error)					\
	    return (error);				\
	uth->uu_reply_msg = 0;				\
	if (serial)					\
	    unix_master();				\
	if (setjmp(&uth->uu_qsave)) {			\
	    if (uth->uu_error == 0 &&			\
		    uth->uu_eosys != RESTARTSYS)	\
		uth->uu_error = EINTR;			\
	}						\
	else {						\
		int	arg[nargs];			\
		uth->uu_ap = arg;

#define	END_SERVER(serial)				\
	}						\
	if (serial)					\
	    unix_release();				\
	return (end_server_op(uth->uu_error, interrupt));

#define	END_SERVER_DEALLOC(data, size, serial)		\
	}						\
	if (serial)					\
	    unix_release();				\
	(void) vm_deallocate(mach_task_self(), data, size); \
	return (end_server_op(uth->uu_error, interrupt));

#define	START_SERVER_SERIAL(syscode, nargs)	\
		START_SERVER(syscode, nargs, TRUE)
#define	START_SERVER_PARALLEL(syscode, nargs)	\
		START_SERVER(syscode, nargs, FALSE)

#define	END_SERVER_SERIAL				\
		END_SERVER(TRUE)
#define	END_SERVER_PARALLEL				\
		END_SERVER(FALSE)

#define	END_SERVER_SERIAL_DEALLOC(data, size)		\
		END_SERVER_DEALLOC(data, size, TRUE)
#define	END_SERVER_PARALLEL_DEALLOC(data, size)		\
		END_SERVER_DEALLOC(data, size, FALSE)


