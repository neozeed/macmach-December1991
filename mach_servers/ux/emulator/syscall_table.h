/* 
 * Mach Operating System
 * Copyright (c) 1989 Carnegie-Mellon University
 * All rights reserved.  The CMU software License Agreement specifies
 * the terms and conditions for use and redistribution.
 */
/*
 * HISTORY
 * $Log:	syscall_table.h,v $
 * Revision 2.3  90/06/19  23:07:11  rpd
 * 	Added pid_by_task.
 * 	[90/06/14            rpd]
 * 
 * Revision 2.2  89/10/17  11:24:24  rwd
 * 	Added sysent_init_process.
 * 	[89/09/29            rwd]
 * 
 */
/*
 * Definition of system call table.
 */
struct sysent {
	int	nargs;		/* number of arguments, or special code */
	int	(*routine)();
};

/*
 * Special arguments:
 */
#define	E_GENERIC	(-1)
				/* no specialized routine */
#define	E_CHANGE_REGS	(-2)
				/* may change registers */

/*
 * Exported system call table
 */
extern struct sysent	sysent[];	/* normal system calls */
extern int		nsysent;

extern struct sysent	cmusysent[];	/* CMU extensions */
extern int		ncmusysent;

extern struct sysent	sysent_task_by_pid;
extern struct sysent	sysent_pid_by_task;
extern struct sysent	sysent_htg_ux_syscall;
extern struct sysent	sysent_init_process;
