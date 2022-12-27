/* 
 * Mach Operating System
 * Copyright (c) 1989 Carnegie-Mellon University
 * All rights reserved.  The CMU software License Agreement specifies
 * the terms and conditions for use and redistribution.
 */
/*
 * HISTORY
 * $Log:	proc_to_task.h,v $
 * Revision 2.3  90/06/02  15:28:18  rpd
 * 	Converted to new IPC.
 * 	[90/03/26  20:22:33  rpd]
 * 
 * Revision 2.2  89/12/08  20:16:50  rwd
 * 	Changed port_to_proc_lookup into a macro.
 * 	[89/11/01            rwd]
 * 
 */
/*
 * Task->process and request_port -> process routines.
 */
extern void	task_to_proc_init();
extern mach_port_t task_to_proc_enter();	/* task_t, struct proc * */
extern struct proc *
		task_to_proc_lookup();	/* task_t */
#define port_to_proc_lookup(port) (struct proc *)port
extern void	task_to_proc_remove();	/* task_t */
