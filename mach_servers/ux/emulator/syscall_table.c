/* 
 * Mach Operating System
 * Copyright (c) 1990 Carnegie-Mellon University
 * Copyright (c) 1989 Carnegie-Mellon University
 * All rights reserved.  The CMU software License Agreement specifies
 * the terms and conditions for use and redistribution.
 */
/*
 * HISTORY
 * $Log:	syscall_table.c,v $
 * Revision 2.10  91/09/03  11:11:39  jsb
 * 	From Intel SSD: Added i860 case.
 * 	[91/09/02  15:07:34  jsb]
 * 
 * Revision 2.9  90/06/19  23:07:03  rpd
 * 	Added pid_by_task.
 * 	[90/06/14            rpd]
 * 
 * Revision 2.8  90/05/29  20:22:36  rwd
 * 	Make all path_name calls go through glue routines to pass length
 * 	to MiG interfaces.
 * 	[90/04/06            dbg]
 * 
 * 	Add lseek for MAP_FILE case.
 * 	[90/03/27            rwd]
 * 
 * Revision 2.7  90/05/21  13:48:19  dbg
 * 	sysents at end of file cannot be initialized with array
 * 	constructors (gcc).
 * 	[90/04/20            dbg]
 * 
 * 	Make all path_name calls go through glue routines to pass length
 * 	to MiG interfaces.
 * 	[90/04/06            dbg]
 * 
 * Revision 2.6  90/03/14  21:24:22  rwd
 * 	Add sigsetmask to mapped calls.
 * 	[90/02/23            rwd]
 * 	Add MAP_UAREA version of obreak.
 * 	[90/01/23            rwd]
 * 
 * Revision 2.5  89/12/08  20:14:01  rwd
 * 	Changed mkdir to 2 params
 * 	[89/11/01            rwd]
 * 
 * Revision 2.4  89/11/29  15:26:58  af
 * 	Made sigvec take 4 args.
 * 	Conditionally expand the table (mips) to 256 entries.
 * 	[89/11/16  15:17:06  af]
 * 
 * Revision 2.3  89/11/15  13:26:37  dbg
 * 	Add e_readv, e_writev, e_pioctl.
 * 	[89/11/07            dbg]
 * 	Add table call.  Fix cmusysent to be zero-based.
 * 	[89/10/25            dbg]
 * 
 * 	Correct number of arguments for mkdir.
 * 	[89/10/24            dbg]
 * 
 * Revision 2.2  89/10/17  11:24:18  rwd
 * 	Added init_process syscall at -41.
 * 	[89/09/29            rwd]
 * 
 */
#include <syscall_table.h>

#define	syss(routine, nargs)	{ nargs, routine }
#define	sysg			{ E_GENERIC, emul_generic }
#define	sysr(routine)		{ E_CHANGE_REGS, routine }
#if defined(ECACHE) || defined(MAP_UAREA)
#define syse(routine)		{ E_GENERIC, routine }
#endif defined(ECACHE) || defined(MAP_UAREA)

/*
 * Routines in system call table
 */
int	emul_generic();		/* catchall */

int	e_fork(), e_execv(), e_execve(), e_sigreturn(), e_osigcleanup(),
	e_wait();
int	e_htg_syscall();

int	e_write(), e_stat(), e_lstat(), e_acct(), e_readlink(), e_select();
int	e_send(), e_getrusage(), e_recvfrom(), e_sendto();
int	e_sigvec(), e_sigstack(), e_settimeofday(), e_setitimer();
int	e_accept(), e_setsockopt(), e_getsockopt();
int	e_getsockname(), e_getpeername();
int	e_table();
int	e_readv(), e_writev(), e_pioctl();

#ifdef ECACHE
int	e_getpid(), e_getdtablesize(), e_getpagesize();
#endif ECACHE

#ifdef MAP_UAREA
int	e_obreak(), e_getdtablesize(), e_getgid(), e_getuid();
int	e_getpid(), e_getrlimit(),     e_umask(),  e_getgroups();
int	e_sigblock(), e_read(), e_sigsetmask(), e_close();
#ifdef	MAP_FILE
int	e_lseek();
#endif	MAP_FILE
#endif MAP_UAREA

int	e_open(), e_creat(), e_link(), e_unlink(), e_chdir();
int	e_mknod(), e_chmod(), e_chown(), e_mount(), e_umount();
int	e_access(), e_symlink(), e_chroot(), e_rename();
int	e_truncate(), e_mkdir(), e_rmdir(), e_utimes();
int	e_setquota(), e_xutimes();
int	bsd_task_by_pid(), bsd_pid_by_task(), bsd_init_process();
int	bsd_setgroups(), bsd_setrlimit(), bsd_adjtime(), bsd_sethostname();
int	bsd_bind(), bsd_connect();

/*
 * System call table
 */
struct sysent sysent[] = {
	sysg, sysg,			/* 0 */
	sysr(e_fork),			/* 2 */
#ifdef	MAP_UAREA
	syse(e_read),			/* 3 */
#else	MAP_UAREA
	sysg,				/* 3 */
#endif	MAP_UAREA
	syss(e_write, 3),		/* 4 */
	syss(e_open, 3),		/* 5 */
#ifdef	MAP_UAREA
	syse(e_close),			/* 6 */
#else	MAP_UAREA
	sysg,
#endif	MAP_UAREA
	sysg,
	syss(e_creat, 2),		/* 8 */
	syss(e_link, 2),		/* 9 */
	syss(e_unlink, 1),		/* 10 */
	sysr(e_execv),			/* 11 */
	syss(e_chdir, 1),		/* 12 */
	sysg,
	syss(e_mknod, 3),		/* 14 */
	syss(e_chmod, 2),		/* 15 */
	syss(e_chown, 3),		/* 16 */
#ifdef	MAP_UAREA
	syse(e_obreak),			/* 17 */
#else	MAP_UAREA
	sysg,
#endif	MAP_UAREA
	sysg,
#if	defined(MAP_FILE) && defined(MAP_UAREA)
	syse(e_lseek),			/* 19 */
#else	defined(MAP_FILE) && defined(MAP_UAREA)
	sysg,
#endif	defined(MAP_FILE) && defined(MAP_UAREA)
#if defined(ECACHE) || defined(MAP_UAREA)
	syse(e_getpid),			/* 20 */
#else defined(ECACHE) || defined(MAP_UAREA)
	sysg,
#endif defined(ECACHE) || defined(MAP_UAREA)
	syss(e_mount, 3),		/* 21 */
	syss(e_umount, 1),		/* 22 */
	sysg,
#ifdef	MAP_UAREA
	syse(e_getuid),			/* 24 */
#else	MAP_UAREA
	sysg,
#endif	MAP_UAREA
	sysg, sysg, sysg, sysg, sysg, sysg, sysg, sysg,
	syss(e_access, 2),		/* 33 */
	sysg, sysg, sysg, sysg,
	syss(e_stat, 2),		/* 38 */
	sysg,
	syss(e_lstat, 2),		/* 40 */
	sysg, sysg, sysg, sysg, sysg, sysg,
#ifdef	MAP_UAREA
	syse(e_getgid),			/* 47 */
#else	MAP_UAREA
	sysg,
#endif	MAP_UAREA
	sysg, sysg,
	sysg,
	syss(e_acct, 1),		/* 51 */
	sysg, sysg, sysg, sysg, sysg,
	syss(e_symlink, 2),		/* 57 */
	syss(e_readlink, 3),		/* 58 */
	sysr(e_execve),			/* 59 */
#ifdef	MAP_UAREA
	syse(e_umask),			/* 60 */
#else	MAP_UAREA
	sysg,
#endif	MAP_UAREA
	syss(e_chroot, 1),		/* 61 */
	sysg, sysg,
#ifdef ECACHE
	syse(e_getpagesize),		/* 64 */
#else ECACHE
	sysg,
#endif ECACHE
	sysg,
	sysr(e_fork),			/* 66 (vfork) */
	sysg, sysg, sysg,
	sysg, sysg, sysg, sysg, sysg, sysg, sysg, sysg, sysg,
#ifdef	MAP_UAREA
	syse(e_getgroups),		/* 79 */
#else	MAP_UAREA
	sysg,
#endif	MAP_UAREA
	syss(bsd_setgroups, 2),		/* 80 */
	sysg, sysg,
	syss(e_setitimer, 3),		/* 83 */
	sysr(e_wait),			/* 84 */
	sysg, sysg, sysg,
	syss(bsd_sethostname, 2),	/* 88 */
#if defined(ECACHE) || defined(MAP_UAREA)
	syse(e_getdtablesize),		/* 89 */
#else defined(ECACHE) || defined(MAP_UAREA)
	sysg,
#endif defined(ECACHE) || defined(MAP_UAREA)
	sysg, sysg, sysg,
	syss(e_select, 5),		/* 93 */
	sysg, sysg, sysg, sysg,
	syss(bsd_connect, 3),		/* 98 */
	syss(e_accept, 3),		/* 99 */
	sysg,
	syss(e_send, 4),		/* 101 */
	sysg,
	sysr(e_sigreturn),		/* 103 */
	syss(bsd_bind, 3),		/* 104 */
	syss(e_setsockopt, 5),		/* 105 */
	sysg, sysg,
	/*
	 * Sigvec is defined to take 3 args, the fourth
	 * one is added inside libc on those
	 * machines that use the sigtramp device
	 */
	syss(e_sigvec, 4),		/* 108 */
#ifdef	MAP_UAREA
	syse(e_sigblock),		/* 109 */
	syse(e_sigsetmask),		/* 110 */
#else	MAP_UAREA
	sysg,sysg,
#endif	MAP_UAREA
	sysg,
	syss(e_sigstack, 2),		/* 112 */
	sysg, sysg, sysg, sysg,
	syss(e_getrusage, 2),		/* 117 */
	syss(e_getsockopt, 5),		/* 118 */
	sysg,
	syss(e_readv, 3),		/* 120 */
	syss(e_writev, 3),		/* 121 */
	syss(e_settimeofday, 2),	/* 122 */
	sysg, sysg,
	syss(e_recvfrom, 6),		/* 125 */
	sysg, sysg,
	syss(e_rename, 2),		/* 128 */
	syss(e_truncate, 2),		/* 129 */
	sysg, sysg, sysg,
	syss(e_sendto, 6),		/* 133 */
	sysg, sysg,
	syss(e_mkdir, 2),		/* 136 */
	syss(e_rmdir, 1),		/* 137 */
	syss(e_utimes, 2),		/* 138 */
	sysr(e_osigcleanup),		/* 139 */
	syss(bsd_adjtime, 2),		/* 140 */
	syss(e_getpeername, 3),		/* 141 */
	sysg, sysg,
#ifdef	MAP_UAREA
	syse(e_getrlimit),		/* 144 */
#else	MAP_UAREA
	sysg,
#endif	MAP_UAREA
	syss(bsd_setrlimit, 2),		/* 145 */
	sysg, sysg,
	syss(e_setquota, 2),		/* 148 */
	sysg,
	syss(e_getsockname, 3),		/* 150 */
#ifdef	mac2
	      sysg, sysg, sysg, sysg, sysg, sysg, sysg, sysg, sysg,
	sysg, sysg, sysg, sysg, sysg, sysg, sysg, sysg,
	syss(e_pioctl, 4),		/* 168 */
							      sysg,
	sysg, sysg, sysg, sysg, sysg, sysg, sysg, sysg, sysg, sysg,
	sysg, sysg, sysg, sysg, sysg, sysg, sysg, sysg, sysg, sysg,
	sysg, sysg, sysg, sysg, sysg, sysg, sysg, sysg, sysg, sysg,
#endif	mac2
#ifdef	sun3
	      sysg, sysg, sysg, sysg, sysg, sysg, sysg, sysg, sysg,
	sysg, sysg, sysg, sysg, sysg, sysg, sysg, sysg,
	syss(e_pioctl, 4),		/* 168 */
							      sysg,
	sysg, sysg, sysg, sysg, sysg, sysg, sysg, sysg, sysg, sysg,
	sysg, sysg, sysg, sysg, sysg, sysg, sysg, sysg, sysg, sysg,
	sysg, sysg, sysg, sysg, sysg, sysg, sysg, sysg, sysg, sysg,
#endif	sun3
#if	defined(vax) || defined(i386) || defined(i860)
	      sysg, sysg, sysg, sysg, sysg, sysg,
	syss(e_pioctl, 4),		/* 157 */
							sysg, sysg,
	sysg, sysg, sysg, sysg, sysg, sysg, sysg, sysg, sysg, sysg,
	sysg,
	syss(e_pioctl, 4),		/* 171 */
		    sysg, sysg, sysg, sysg, sysg, sysg, sysg, sysg,
	sysg, sysg, sysg, sysg, sysg, sysg, sysg, sysg, sysg, sysg,
	sysg, sysg, sysg, sysg, sysg, sysg, sysg, sysg, sysg, sysg,
#endif	defined(vax) || defined(i386) || defined(i860)
#ifdef	mips
	sysg, sysg, sysg, sysg, sysg, sysg, sysg, sysg, sysg, sysg,
	sysg, sysg, sysg, sysg, sysg, sysg, sysg,
	syss(e_pioctl, 4),		/* 168 */
	sysg, sysg,
	sysg, sysg, sysg, sysg, sysg, sysg, sysg, sysg, sysg, sysg,
	sysg, sysg, sysg, sysg, sysg, sysg, sysg, sysg, sysg, sysg,
	sysg, sysg, sysg, sysg,
	syss(e_pioctl, 4),		/* 195 */
	sysg, sysg, sysg, sysg, sysg,
	sysg, sysg, sysg, sysg, sysg, sysg, sysg, sysg, sysg, sysg,
	sysg, sysg, sysg, sysg, sysg, sysg, sysg, sysg, sysg, sysg,
	sysg, sysg, sysg, sysg, sysg, sysg, sysg, sysg, sysg, sysg,
	sysg, sysg, sysg, sysg, sysg, sysg, sysg, sysg, sysg, sysg,
	sysg, sysg, sysg, sysg, sysg, sysg, sysg, sysg, sysg, sysg,
	sysg, sysg, sysg, sysg, sysg, sysg, sysg
#endif	mips
};

int	nsysent = sizeof(sysent)/sizeof(struct sysent);

struct sysent	cmusysent[] = {
	sysg,				/*  0 (make it zero-based) */
	sysg,				/* -1 */
	sysg,				/* -2 */
	sysg,				/* -3 */
	syss(e_xutimes, 2),		/* -4 */
	sysg,				/* -5 */
	syss(e_table, 5),		/* -6 */
	sysg,				/* -7 */
	sysg,				/* -8 */
	sysg				/* -9 */
};

int	ncmusysent = sizeof(cmusysent)/sizeof(struct sysent);

struct sysent	sysent_task_by_pid =
	syss(bsd_task_by_pid, 1);

struct sysent	sysent_pid_by_task =
	syss(bsd_pid_by_task, 4);

struct sysent	sysent_htg_ux_syscall =
	sysr(e_htg_syscall);

struct sysent	sysent_init_process =
	syss(bsd_init_process, 0);
