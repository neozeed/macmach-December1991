/* 
 * Mach Operating System
 * Copyright (c) 1991,1990,1989,1988,1987 Carnegie Mellon University
 * All Rights Reserved.
 * 
 * Permission to use, copy, modify and distribute this software and its
 * documentation is hereby granted, provided that both the copyright
 * notice and this permission notice appear in all copies of the
 * software, derivative works or modified versions, and any portions
 * thereof, and that both notices appear in supporting documentation.
 * 
 * CARNEGIE MELLON ALLOWS FREE USE OF THIS SOFTWARE IN ITS "AS IS"
 * CONDITION.  CARNEGIE MELLON DISCLAIMS ANY LIABILITY OF ANY KIND FOR
 * ANY DAMAGES WHATSOEVER RESULTING FROM THE USE OF THIS SOFTWARE.
 * 
 * Carnegie Mellon requests users of this software to return to
 * 
 *  Software Distribution Coordinator  or  Software.Distribution@CS.CMU.EDU
 *  School of Computer Science
 *  Carnegie Mellon University
 *  Pittsburgh PA 15213-3890
 * 
 * any improvements or extensions that they make and grant Carnegie Mellon
 * the rights to redistribute these changes.
 */
/*
 * HISTORY
 * $Log:	sched_prim.h,v $
 * Revision 2.7  91/05/18  14:33:14  rpd
 * 	Added recompute_priorities.
 * 	[91/03/31            rpd]
 * 
 * Revision 2.6  91/05/14  16:46:40  mrt
 * 	Correcting copyright
 * 
 * Revision 2.5  91/03/16  14:51:26  rpd
 * 	Added declarations of new functions.
 * 	[91/02/24            rpd]
 * 
 * Revision 2.4  91/02/05  17:29:09  mrt
 * 	Changed to new Mach copyright
 * 	[91/02/01  16:17:24  mrt]
 * 
 * Revision 2.3  90/06/02  14:56:03  rpd
 * 	Updated to new scheduling technology.
 * 	[90/03/26  22:17:10  rpd]
 * 
 * Revision 2.2  90/01/11  11:44:04  dbg
 * 	Export thread_bind.
 * 	[89/12/06            dbg]
 * 
 * Revision 2.1  89/08/03  15:53:39  rwd
 * Created.
 * 
 * 29-Jun-88  David Golub (dbg) at Carnegie-Mellon University
 *	Removed THREAD_SHOULD_TERMINATE.
 *
 * 16-May-88  Michael Young (mwyoung) at Carnegie-Mellon University
 *	Added thread_wakeup_with_result routine; thread_wakeup
 *	is a special case.
 *
 * 16-Apr-88  Michael Young (mwyoung) at Carnegie-Mellon University
 *	Added THREAD_RESTART wait result value.
 *
 * 29-Feb-88  David Black (dlb) at Carnegie-Mellon University
 *	thread_setrun is now a real routine.
 *
 * 13-Oct-87  David Golub (dbg) at Carnegie-Mellon University
 *	Moved thread_will_wait and thread_go to sched_prim_macros.h,
 *	to avoid including thread.h everywhere.
 *
 *  5-Oct-87  David Golub (dbg) at Carnegie-Mellon University
 *	Created.  Moved thread_will_wait and thread_go here from
 *	mach_ipc.
 */
/*
 *	File:	sched_prim.h
 *	Author:	David Golub
 *
 *	Scheduling primitive definitions file
 *
 */

#ifndef	_KERN_SCHED_PRIM_H_
#define _KERN_SCHED_PRIM_H_

#include <mach/boolean.h>

/*
 *	Possible results of assert_wait - returned in
 *	current_thread()->wait_result.
 */
#define THREAD_AWAKENED		0		/* normal wakeup */
#define THREAD_TIMED_OUT	1		/* timeout expired */
#define THREAD_INTERRUPTED	2		/* interrupted by clear_wait */
#define THREAD_SHOULD_TERMINATE	3		/* thread should terminate */
#define THREAD_RESTART		4		/* restart operation entirely */

/*
 *	Exported interface to sched_prim.c.
 *	A few of these functions are actually defined in
 *	ipc_sched.c, for historical reasons.
 */

extern void	sched_init();
extern void	assert_wait();
extern void	clear_wait();
extern void	thread_sleep();
extern void	thread_wakeup();		/* for function pointers */
extern void	thread_wakeup_prim();
extern boolean_t thread_invoke();
extern void	thread_block();
extern void	thread_run();
extern void	thread_set_timeout();
extern void	thread_setrun();
extern void	thread_dispatch();
extern void	thread_continue();
extern void	thread_go();
extern void	thread_will_wait();
extern void	thread_will_wait_with_timeout();
extern boolean_t thread_handoff();
extern void	recompute_priorities();

/*
 *	Routines defined as macros
 */

#define thread_wakeup(x)						\
		thread_wakeup_prim((x), FALSE, THREAD_AWAKENED)
#define thread_wakeup_with_result(x, z)					\
		thread_wakeup_prim((x), FALSE, (z))
#define thread_wakeup_one(x)						\
		thread_wakeup_prim((x), TRUE, THREAD_AWAKENED)

/*
 *	Machine-dependent code must define these functions.
 */

extern void	thread_bootstrap_return();
extern void	thread_exception_return();
extern void	thread_syscall_return();
extern struct thread *switch_context();
extern void	stack_handoff();

/*
 *	These functions are either defined in kern/thread.c
 *	via machine-dependent stack_attach and stack_detach functions,
 *	or are defined directly by machine-dependent code.
 */

extern void	stack_alloc();
extern boolean_t stack_alloc_try();
extern void	stack_free();

#endif	_KERN_SCHED_PRIM_H_
