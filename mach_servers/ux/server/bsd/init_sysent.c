/* 
 * Mach Operating System
 * Copyright (c) 1990 Carnegie-Mellon University
 * Copyright (c) 1989 Carnegie-Mellon University
 * Copyright (c) 1988 Carnegie-Mellon University
 * All rights reserved.  The CMU software License Agreement specifies
 * the terms and conditions for use and redistribution.
 */
/*
 * HISTORY
 * $Log:	init_sysent.c,v $
 * Revision 2.6  91/09/03  11:11:51  jsb
 * 	From Intel SSD: Added i860 cases.
 * 	[91/09/02  14:01:48  jsb]
 * 
 * Revision 2.5  91/03/20  14:34:23  dbg
 * 	Added sysmips() to let Messages run [sigh].
 * 	[91/02/04            af]
 * 
 * Revision 2.4  90/05/21  13:49:18  dbg
 * 	Added i386 syscalls.  Cleaned up conditionals.
 * 	sigvec always takes 4 arguments.
 * 	[90/03/20            dbg]
 * 
 * Revision 2.3  89/11/29  15:27:14  af
 * 	Expanded syscall table for mips, since Ultrix uses up to syscall 256!
 * 	Added vice syscalls etc for mips.
 * 	[89/11/26  11:27:21  af]
 * 
 * Revision 2.2  89/08/31  16:28:39  rwd
 * 	Made syscall -7 sctrace (toggle set syscalltrace)
 * 	[89/08/22            rwd]
 * 
 * Revision 2.1  89/08/04  14:05:36  rwd
 * Created.
 * 
 * Revision 2.4  88/08/24  01:16:22  mwyoung
 * 	Corrected include file references.
 * 	[88/08/22  22:04:07  mwyoung]
 * 
 *
 * 12-May-88  Mike Accetta (mja) at Carnegie-Mellon University
 *	Integrated changes originally by Bob Baron to also support the
 *	Vice system calls at the new Ultrix 2.0 positions;
 *	MACH_COMPAT:  enable the [sg]etdomainname() and getdirentries()
 *	calls under this option to replace the SUN_UNIX option since
 *	they are now needed on both the Sun (for SunOS applications)
 *	and Vax (for Andrew Ultrix 2.0 applications).
 *	[ V5.1(XF24) ]
 *
 * 27-Oct-87  Robert Baron (rvb) at Carnegie-Mellon University
 *	read/write and readv/writev are sysp now
 *
 * 08-Oct-87  Mike Accetta (mja) at Carnegie-Mellon University
 *	Extract vendor specific system calls into per-machine tables to
 *	reduce complexity of multiply nested conditionals.
 *	[ V5.1(XF18) ]
 *
 * 29-Sep-87  Jonathan J. Chew (jjc) at Carnegie-Mellon University
 *	Put back SUN_UNIX system calls.
 *
 * 22-Sep-87  Richard Sanzi (sanzi) at Carnegie-Mellon University
 *	To avoid a conflict with ACIS defined syscalls for the RT, I
 *	moved the VMTP syscalls for all archictectures.
 *
 * 31-Aug-87  David Black (dlb) at Carnegie-Mellon University
 *	gettimeofday and settimeofday must be serial; time and related
 *	data have no locks.
 *
 *  1-Jul-87  Daniel Julin (dpj) at Carnegie-Mellon University
 *	Updated from new VMTP sources from Stanford (June 87).
 *
 * 31-May-87  Daniel Julin (dpj) at Carnegie-Mellon University
 *	VMTP_INVOKE: added support for split invoke call.
 *
 * 28-May-87  Daniel Julin (dpj) at Carnegie-Mellon University
 *	Added VMTP.
 *
 * 02-Apr-87  Jonathan J. Chew (jjc) at Carnegie-Mellon University
 *	Added entry in system call table for Sun ptrace which has
 *	five arguments instead of four.
 *
 * 17-Mar-87  Mike Accetta (mja) at Carnegie-Mellon University
 *	Restored xutimes() call.
 *	[ V5.1(F7) ]
 *
 * 16-Mar-87  Mike Accetta (mja) at Carnegie-Mellon University
 *	Fixed compatp() definition for non-MACH case.
 *	[ V5.1(F7) ]
 *
 * 22-Feb-87  Avadis Tevanian (avie) at Carnegie-Mellon University
 *	Purge MACH_ACC.
 *
 * 15-Feb-87  Avadis Tevanian (avie) at Carnegie-Mellon University
 *	Redid VICE system calls again.  They are different for EACH
 *	machine type.  I just love binary compatibility.
 *
 *  7-Feb-87  Avadis Tevanian (avie) at Carnegie-Mellon University
 *	Merge VICE changes -- include vice.h and change to #if VICE.
 *	Deleted SUN_UNIX calls, reconcilled romp/BALANCE/VICE system
 *	calls.  Amazing.
 *
 * 27-Jan-87  Mike Accetta (mja) at Carnegie-Mellon University
 *	CMUCS_RPAUSE: Added rpause() system call.
 *	[ V5.1(F1) ]
 *
 * 08-Jan-87  Robert Beck (beck) at Sequent Computer Systems, Inc.
 *	Add #include cputypes.h for BALANCE definition.
 *	Add #ifdef BALANCE extra sysent entries (getdirentries and tmp_ctl,
 *	plus reserve 10 more slots just in case).
 *	If BALANCE, sigvec() takes 4 arguments.
 *
 * 13-Dec-86  Bill Bolosky (bolosky) at Carnegie-Mellon University
 *	romp: Added syscall 153 as "getfpemulator" to allow floating
 *	point to work from C.
 *
 *  2-Dec-86  Jay Kistler (jjk) at Carnegie-Mellon University
 *	VICE:  added calls to support ITC/Andrew remote file system.
 *	N.B.  I see that these call numbers may have to be
 *	reassigned--I'll do this after debugging; meanwhile, the only
 *	incompatibility I see is with SUN_UNIX.
 *
 * 23-Sep-86  David Golub (dbg) at Carnegie-Mellon University
 *	Added osigcleanup (from 4.2) to normal syscall dispatch
 *	tables (under CMUCS).  Added hooks for SUN-specific
 *	system calls (emulation).
 *
 *  7-Aug-86  David Golub (dbg) at Carnegie-Mellon University
 *	Merged with real 4.3 release.
 *
 * 27-Jul-86  Bill Bolosky (bolosky) at Carnegie-Mellon University
 *	romp: added "exect" call to support debuggers on the RT.
 *
 * 25-Jan-86  Avadis Tevanian (avie) at Carnegie-Mellon University
 *	Upgraded to 4.3.
 *
 *  9-Aug-85	David L. Black at CMU.  Added new getutime syscall.
 *
 * 22-Oct-84  Robert V Baron (rvb) at Carnegie-Mellon University
 *	removed Avie's change for system call #151
 * 	and use the syscall handler in mp/syscall_sw.c instead
 *
 *  1-Apr-84  Avadis Tevanian (avie) at Carnegie-Mellon University
 *	Added IPCAtrium system call (#152) and cmusys system call (153).
 *
 * 31-Mar-84  Avadis Tevanian (avie) at Carnegie-Mellon University
 *	Added mpsys system call (#151).
 *
 */

#include <mach_no_kernel.h>
#include <cputypes.h>

#include <mach_compat.h>
#include <mach_time.h>
#include <mach_vmtp.h>
#include <vice.h>

#define	VMTP_INVOKE	1

/*
 * Copyright (c) 1982, 1986 Regents of the University of California.
 * All rights reserved.  The Berkeley software License Agreement
 * specifies the terms and conditions for redistribution.
 *
 *	@(#)init_sysent.c	7.1 (Berkeley) 6/5/86
 */

/*
 * System call switch table.
 */

#include <sys/param.h>
#include <sys/systm.h>

	/* serial or parallel system call */
#define syss(fn,no) {no, 0, fn}
#define sysp(fn,no) {no, 1, fn}

int	nosys();

/* 1.1 processes and protection */
int	sethostid(),gethostid(),sethostname(),gethostname(),getpid();
int	fork(),rexit(),execv(),execve(),wait();
#ifdef	ibmrt
int	exect(); /* Temporary hack to support RT adb. */
int	getfpemulator();	/* Silliness for RT binary compatibility */
int	getfloatstate(), setfloatstate();
#endif	ibmrt
int	getuid(),setreuid(),getgid(),getgroups(),setregid(),setgroups();
int	getpgrp(),setpgrp();

/* 1.2 memory management */
int	sbrk(),sstk();
int	getpagesize(),smmap(),mremap(),munmap(),mprotect(),madvise(),mincore();

/* 1.3 signals */
int	sigvec(),sigblock(),sigsetmask(),sigpause(),sigstack(),sigreturn();
int	kill(), killpg();
int	osigcleanup();		/* XXX 4.2 sigcleanup call, used by longjmp */

/* 1.4 timing and statistics */
int	gettimeofday(),settimeofday();
int	getitimer(),setitimer();
int 	adjtime();

/* 1.5 descriptors */
int	getdtablesize(),dup(),dup2(),close();
int	select(),getdopt(),setdopt(),fcntl(),flock();

/* 1.6 resource controls */
int	getpriority(),setpriority(),getrusage(),getrlimit(),setrlimit();
int	setquota(),qquota();

/* 1.7 system operation support */
int	umount(),smount(),swapon();
int	sync(),reboot(),sysacct();

/* 2.1 generic operations */
int	read(),write(),readv(),writev(),ioctl();

/* 2.2 file system */
int	chdir(),chroot();
int	mkdir(),rmdir();
int	creat(),open(),mknod(),unlink(),stat(),fstat(),lstat();
int	chown(),fchown(),chmod(),fchmod(),utimes();
int	link(),symlink(),readlink(),rename();
int	lseek(),truncate(),ftruncate(),saccess(),fsync();

/* 2.3 communications */
int	socket(),bind(),listen(),accept(),connect();
int	socketpair(),sendto(),send(),recvfrom(),recv();
int	sendmsg(),recvmsg(),shutdown(),setsockopt(),getsockopt();
int	getsockname(),getpeername(),pipe();

int	umask();		/* XXX */

/* 2.4 processes */
int	ptrace();

/* 2.5 terminals */

#if	MACH_COMPAT
/* 2.? emulation for vendor-specific system calls */
int	getdirentries(), getdomainname(), setdomainname();

#define	syss_getdirentries()	syss(getdirentries,4)
#define	syss_getdomainname()	syss(getdomainname,2)	
#define	syss_setdomainname()	syss(setdomainname,2)	
#else	MACH_COMPAT
#define	syss_getdirentries()	syss(nosys,0)
#define	syss_getdomainname()	syss(nosys,0)	
#define	syss_setdomainname()	syss(nosys,0)	
#endif	MACH_COMPAT

#ifdef	balance
/* emulation for BALANCE-specific system calls */
int	tmp_ctl();
int	universe();
#endif	balance

#ifdef	mips
int	sysmips();
#endif	mips

#ifdef COMPAT
/* emulations for backwards compatibility */
#define	compat(name,n)	{n, 0, name}
#define	compatp(name,n)	{n, 1, name}

int	owait();		/* now receive message on channel */
int	otime();		/* now use gettimeofday */
int	ostime();		/* now use settimeofday */
int	oalarm();		/* now use setitimer */
int	outime();		/* now use utimes */
int	opause();		/* now use sigpause */
int	onice();		/* now use setpriority,getpriority */
int	oftime();		/* now use gettimeofday */
int	osetpgrp();		/* ??? */
int	otimes();		/* now use getrusage */
int	ossig();		/* now use sigvec, etc */
int	ovlimit();		/* now use setrlimit,getrlimit */
int	ovtimes();		/* now use getrusage */
int	osetuid();		/* now use setreuid */
int	osetgid();		/* now use setregid */
int	ostat();		/* now use stat */
int	ofstat();		/* now use fstat */
#else
#define	compat(n, name)		{0, 0, nosys}
#define	compatp(n, name)	{0, 1, nosys}
#endif

/* BEGIN JUNK */
#ifdef vax
int	resuba();
#ifdef TRACE
int	vtrace();
#endif
#endif
int	profil();		/* 'cuz sys calls are interruptible */
int	vhangup();		/* should just do in exit() */
int	vfork();		/* awaiting fork w/ copy on write */
int	obreak();		/* awaiting new sbrk */
int	ovadvise();		/* awaiting new madvise */
/* END JUNK */

#if	VICE
int	iopen();
int	iread();
int	iwrite();
int	iinc();
int	idec();
int	pioctl();
int	icreate();
int     setpag();
#define	syss_iopen()	syss(iopen,3)
#define	syss_iread()	syss(iread,6)
#define	syss_iwrite()	syss(iwrite,6)
#define	syss_iinc()	syss(iinc,3)
#define	syss_idec()	syss(idec,3)
#define	syss_pioctl()	syss(pioctl,4)
#define	syss_setpag()	syss(setpag,0)
#define	syss_icreate()	syss(icreate,6)
#else	VICE
#define	syss_iopen()	syss(nosys,0)
#define	syss_iread()	syss(nosys,0)
#define	syss_iwrite()	syss(nosys,0)
#define	syss_iinc()	syss(nosys,0)
#define	syss_idec()	syss(nosys,0)
#define	syss_pioctl()	syss(nosys,0)
#define	syss_setpag()	syss(nosys,0)
#define	syss_icreate()	syss(nosys,0)
#endif	VICE
#if MACH_VMTP

/* vmtp - request/response communication */
int	invoke();		/* invoke a message transaction*/
int	recvreq();		/* receive a request */
int	sendreply();		/* send a reply */
int	forward();		/* forward a request */
int	probeentity();		/* probe a communicating entity */
int	getreply();		/* get the next reply (multicast) */
#endif	MACH_VMTP

extern int rpause();
extern int setmodes();
extern int getmodes();
extern int setaid();
extern int getaid();
extern int table();
extern int xutimes();
extern int nulldev();			/* to test 4.1/4.2 kernel */
#if MACH_NO_KERNEL
extern int sctrace();
#endif MACH_NO_KERNEL

struct sysent cmusysent[] =
{
	syss(getmodes, 0),		/* -9 = get process modes */
	syss(setmodes, 1),		/* -8 = set process modes */
#if MACH_NO_KERNEL
	syss(sctrace,1),
#else MACH_NO_KERNEL
	syss(nosys, 2),			/* -7 = old CMU IPC */
#endif MACH_NO_KERNEL
	syss(table, 5),			/* -6 = table lookup */
	syss(rpause, 3),		/* -5 = resource pause */
	syss(xutimes, 2),		/* -4 = extended utimes() */
	syss(nulldev, 0),		/* -3 = old chacct */
	syss(getaid, 0),		/* -2 = get account ID */
	syss(setaid, 1),		/* -1 = set account ID */
}; 
/* 
 *  The preceding table is effectively a negative extension of the following
 *  table.  No other definitions should intervene here.  The system call
 *  handler assumes that these two tables are contiguous.
 */
struct sysent sysent[] = {
	syss(nosys,0),			/*   0 = indir */
	syss(rexit,1),			/*   1 = exit */
	syss(fork,0),			/*   2 = fork */
	sysp(read,3),			/*   3 = read */
	sysp(write,3),			/*   4 = write */
	syss(open,3),			/*   5 = open */
	syss(close,1),			/*   6 = close */
		compat(owait,0),	/*   7 = old wait */
	syss(creat,2),			/*   8 = creat */
	syss(link,2),			/*   9 = link */
	syss(unlink,1),			/*  10 = unlink */
	syss(execv,2),			/*  11 = execv */
	syss(chdir,1),			/*  12 = chdir */
		compat(otime,0),	/*  13 = old time */
	syss(mknod,3),			/*  14 = mknod */
	syss(chmod,2),			/*  15 = chmod */
	syss(chown,3),			/*  16 = chown; now 3 args */
	syss(obreak,1),			/*  17 = old break */
		compat(ostat,2),	/*  18 = old stat */
	syss(lseek,3),			/*  19 = lseek */
	sysp(getpid,0),			/*  20 = getpid */
	syss(smount,3),			/*  21 = mount */
	syss(umount,1),			/*  22 = umount */
		compat(osetuid,1),	/*  23 = old setuid */
	sysp(getuid,0),			/*  24 = getuid */
		compat(ostime,1),	/*  25 = old stime */
#ifdef	sun
	syss(ptrace,5),			/*  26 = ptrace */
#else	sun
	syss(ptrace,4),			/*  26 = ptrace */
#endif	sun
		compat(oalarm,1),	/*  27 = old alarm */
		compat(ofstat,2),	/*  28 = old fstat */
		compat(opause,0),	/*  29 = opause */
		compat(outime,2),	/*  30 = old utime */
	syss(nosys,0),			/*  31 = was stty */
	syss(nosys,0),			/*  32 = was gtty */
	syss(saccess,2),		/*  33 = access */
		compat(onice,1),	/*  34 = old nice */
		compat(oftime,1),	/*  35 = old ftime */
	syss(sync,0),			/*  36 = sync */
	syss(kill,2),			/*  37 = kill */
	syss(stat,2),			/*  38 = stat */
		compat(osetpgrp,2),	/*  39 = old setpgrp */
	syss(lstat,2),			/*  40 = lstat */
	syss(dup,2),			/*  41 = dup */
	syss(pipe,0),			/*  42 = pipe */
		compat(otimes,1),	/*  43 = old times */
	syss(profil,4),			/*  44 = profil */
	syss(nosys,0),			/*  45 = nosys */
		compatp(osetgid,1),	/*  46 = old setgid */
	sysp(getgid,0),			/*  47 = getgid */
		compat(ossig,2),	/*  48 = old sig */
	syss(nosys,0),			/*  49 = reserved for USG */
	syss(nosys,0),			/*  50 = reserved for USG */
	syss(sysacct,1),		/*  51 = turn acct off/on */
	syss(nosys,0),			/*  52 = old set phys addr */
	syss(nosys,0),			/*  53 = old lock in core */
	syss(ioctl,3),			/*  54 = ioctl */
	syss(reboot,1),			/*  55 = reboot */
	syss(nosys,0),			/*  56 = old mpxchan */
	syss(symlink,2),		/*  57 = symlink */
	syss(readlink,3),		/*  58 = readlink */
	syss(execve,3),			/*  59 = execve */
	syss(umask,1),			/*  60 = umask */
	syss(chroot,1),			/*  61 = chroot */
	syss(fstat,2),			/*  62 = fstat */
	syss(nosys,0),			/*  63 = used internally */
	sysp(getpagesize,1),		/*  64 = getpagesize */
	syss(mremap,5),			/*  65 = mremap */
	syss(vfork,0),			/*  66 = vfork */
	syss(read,3),			/*  67 = old vread */
	syss(write,3),			/*  68 = old vwrite */
	syss(sbrk,1),			/*  69 = sbrk */
	syss(sstk,1),			/*  70 = sstk */
	syss(smmap,6),			/*  71 = mmap */
	syss(ovadvise,1),		/*  72 = old vadvise */
	syss(munmap,2),			/*  73 = munmap */
	syss(mprotect,3),		/*  74 = mprotect */
	syss(madvise,3),		/*  75 = madvise */
	syss(vhangup,1),		/*  76 = vhangup */
		compat(ovlimit,2),	/*  77 = old vlimit */
	syss(mincore,3),		/*  78 = mincore */
	sysp(getgroups,2),		/*  79 = getgroups */
	sysp(setgroups,2),		/*  80 = setgroups */
	sysp(getpgrp,1),		/*  81 = getpgrp */
	sysp(setpgrp,2),		/*  82 = setpgrp */
	syss(setitimer,3),		/*  83 = setitimer */
	syss(wait,0),			/*  84 = wait */
	syss(swapon,1),			/*  85 = swapon */
	syss(getitimer,2),		/*  86 = getitimer */
	sysp(gethostname,2),		/*  87 = gethostname */
	sysp(sethostname,2),		/*  88 = sethostname */
	sysp(getdtablesize,0),		/*  89 = getdtablesize */
	syss(dup2,2),			/*  90 = dup2 */
	sysp(getdopt,2),		/*  91 = getdopt */
	syss(fcntl,3),			/*  92 = fcntl */
	syss(select,5),			/*  93 = select */
	syss(setdopt,2),		/*  94 = setdopt */
	syss(fsync,1),			/*  95 = fsync */
	sysp(setpriority,3),		/*  96 = setpriority */
	syss(socket,3),			/*  97 = socket */
	syss(connect,3),		/*  98 = connect */
	syss(accept,3),			/*  99 = accept */
	sysp(getpriority,2),		/* 100 = getpriority */
	syss(send,4),			/* 101 = send */
	syss(recv,4),			/* 102 = recv */
	syss(sigreturn,1),		/* 103 = sigreturn */
	syss(bind,3),			/* 104 = bind */
	syss(setsockopt,5),		/* 105 = setsockopt */
	syss(listen,2),			/* 106 = listen */
		compat(ovtimes,2),	/* 107 = old vtimes */
#ifdef	MACH_KERNEL
	syss(sigvec,4),			/* 108 = sigvec */
#else	MACH_KERNEL
	syss(sigvec,3),			/* 108 = sigvec */
#endif	MACH_KERNEL
	syss(sigblock,1),		/* 109 = sigblock */
	syss(sigsetmask,1),		/* 110 = sigsetmask */
	syss(sigpause,1),		/* 111 = sigpause */
	syss(sigstack,2),		/* 112 = sigstack */
	syss(recvmsg,3),		/* 113 = recvmsg */
	syss(sendmsg,3),		/* 114 = sendmsg */
#ifdef	TRACE
	syss(vtrace,2),			/* 115 = vtrace */
#else	TRACE
	syss(nosys,0),			/* 115 = nosys */
#endif	TRACE
	syss(gettimeofday,2),		/* 116 = gettimeofday */
	sysp(getrusage,2),		/* 117 = getrusage */
	syss(getsockopt,5),		/* 118 = getsockopt */
#ifdef	vax
	syss(resuba,1),			/* 119 = resuba */
#else	vax
	syss(nosys,0),			/* 119 = nosys */
#endif	vax
	sysp(readv,3),			/* 120 = readv */
	sysp(writev,3),			/* 121 = writev */
	syss(settimeofday,2),		/* 122 = settimeofday */
	syss(fchown,3),			/* 123 = fchown */
	syss(fchmod,2),			/* 124 = fchmod */
	syss(recvfrom,6),		/* 125 = recvfrom */
	sysp(setreuid,2),		/* 126 = setreuid */
	sysp(setregid,2),		/* 127 = setregid */
	syss(rename,2),			/* 128 = rename */
	syss(truncate,2),		/* 129 = truncate */
	syss(ftruncate,2),		/* 130 = ftruncate */
	syss(flock,2),			/* 131 = flock */
	syss(nosys,0),			/* 132 = nosys */
	syss(sendto,6),			/* 133 = sendto */
	syss(shutdown,2),		/* 134 = shutdown */
	syss(socketpair,5),		/* 135 = socketpair */
	syss(mkdir,2),			/* 136 = mkdir */
	syss(rmdir,1),			/* 137 = rmdir */
	syss(utimes,2),			/* 138 = utimes */
	syss(osigcleanup,0),		/* 139 = 4.2 signal cleanup */
	syss(adjtime,2),		/* 140 = adjtime */
	syss(getpeername,3),		/* 141 = getpeername */
	sysp(gethostid,0),		/* 142 = gethostid */
	sysp(sethostid,1),		/* 143 = sethostid */
	sysp(getrlimit,2),		/* 144 = getrlimit */
	sysp(setrlimit,2),		/* 145 = setrlimit */
	syss(killpg,2),			/* 146 = killpg */
	syss(nosys,0),			/* 147 = nosys */
	syss(setquota,2),		/* 148 = quota */
	syss(qquota,4),			/* 149 = qquota */
	syss(getsockname,3),		/* 150 = getsockname */
	/*
	 * Syscalls 151-180 inclusive are reserved for vendor-specific
	 * system calls.  (This includes various calls added for compatibity
	 * with other Unix variants.)
	 */
#define	GENERIC	1
#ifdef	ibmrt
#undef	GENERIC
	syss(exect,3),			/* 151 = exect */
	syss(nosys,0),			/* 152 */
	syss(getfpemulator,0),		/* 153 */
	syss_iopen(),			/* 154 = iopen */
	syss_iread(),			/* 155 = iread */
	syss_iwrite(),			/* 156 = iwrite */
	syss_iinc(),			/* 157 = iinc */
	syss_idec(),			/* 158 = idec */
	syss_pioctl(),			/* 159 = pioctl */
	syss_setpag(),			/* 160 = setpag */
	syss_icreate(),			/* 161 = icreate */
	syss(nosys,0),			/* 162 */
	syss(nosys,0),			/* 163 */
	syss(nosys,0),			/* 164 */
	syss(nosys,0),			/* 165 */
	syss(nosys,0),			/* 166 */
	syss(getfloatstate,2),		/* 167 = getfloatstate */
	syss(setfloatstate,3),		/* 168 = setfloatstate */
	syss(nosys,0),			/* 169 */
	syss(nosys,0),			/* 170 */
	syss(nosys,0),			/* 171 */
	syss(nosys,0),			/* 172 */
	syss(nosys,0),			/* 173 */
	syss(nosys,0),			/* 174 */
	syss(nosys,0),			/* 175 */
	syss(nosys,0),			/* 176 */
	syss(nosys,0),			/* 177 */
	syss(nosys,0),			/* 178 */
	syss(nosys,0),			/* 179 */
	syss(nosys,0),			/* 180 */
#endif	ibmrt
#ifdef	mac2 /* mac2 system calls */
#undef	GENERIC
	syss(nosys,0),			/* 151 */
	syss(nosys,0),			/* 152 */
	syss(nosys,0),			/* 153 */
	syss(nosys,0),			/* 154 */
	syss(nosys,0),			/* 155 */
	syss_getdirentries(),		/* 156 = getdirentries */
	syss(nosys,0),			/* 157 */
	syss(nosys,0),			/* 158 */
	syss(nosys,0),			/* 159 */
	syss(nosys,0),			/* 160 */
	syss(nosys,0),			/* 161 */
	syss_getdomainname(),		/* 162 = getdomainname */
	syss_setdomainname(),		/* 163 = setdomainname */
	syss(nosys,0),			/* 164 */
	syss(nosys,0),			/* 165 */
	syss(nosys,0),			/* 166 */
	syss(nosys,0),			/* 167 */
        syss_pioctl(),			/* 168 = pioctl */
        syss_setpag(),			/* 169 = setpag */
        syss_icreate(),			/* 170 = icreate */
        syss_iopen(),			/* 171 = iopen */
        syss_iread(),			/* 172 = iread */
        syss_iwrite(),			/* 173 = iwrite */
        syss_iinc(),			/* 174 = iinc */
        syss_idec(),			/* 175 = idec */
	syss(nosys,0),			/* 176 */
	syss(nosys,0),			/* 177 */
	syss(nosys,0),			/* 178 */
	syss(nosys,0),			/* 179 */
	syss(nosys,0),			/* 180 */
#endif	mac2
#ifdef	sun3
#undef	GENERIC
	syss(nosys,0),			/* 151 */
	syss(nosys,0),			/* 152 */
	syss(nosys,0),			/* 153 */
	syss(nosys,0),			/* 154 */
	syss(nosys,0),			/* 155 */
	syss_getdirentries(),		/* 156 = getdirentries */
	syss(nosys,0),			/* 157 */
	syss(nosys,0),			/* 158 */
	syss(nosys,0),			/* 159 */
	syss(nosys,0),			/* 160 */
	syss(nosys,0),			/* 161 */
	syss_getdomainname(),		/* 162 = getdomainname */
	syss_setdomainname(),		/* 163 = setdomainname */
	syss(nosys,0),			/* 164 */
	syss(nosys,0),			/* 165 */
	syss(nosys,0),			/* 166 */
	syss(nosys,0),			/* 167 */
        syss_pioctl(),			/* 168 = pioctl */
        syss_setpag(),			/* 169 = setpag */
        syss_icreate(),			/* 170 = icreate */
        syss_iopen(),			/* 171 = iopen */
        syss_iread(),			/* 172 = iread */
        syss_iwrite(),			/* 173 = iwrite */
        syss_iinc(),			/* 174 = iinc */
        syss_idec(),			/* 175 = idec */
	syss(nosys,0),			/* 176 */
	syss(nosys,0),			/* 177 */
	syss(nosys,0),			/* 178 */
	syss(nosys,0),			/* 179 */
	syss(nosys,0),			/* 180 */
#endif	sun3
#if	defined(vax) || defined(i386) || defined(i860)
#undef	GENERIC
        syss_icreate(),			/* 151 = icreate */
        syss_iopen(),			/* 152 = iopen */
        syss_iread(),			/* 153 = iread */
        syss_iwrite(),			/* 154 = iwrite */
        syss_iinc(),			/* 155 = iinc */
        syss_idec(),			/* 156 = idec */
        syss_pioctl(),			/* 157 = pioctl */
        syss_setpag(),			/* 158 = setpag */
	syss(nosys,0),			/* 159 */
	syss(nosys,0),			/* 160 */
	syss(nosys,0),			/* 161 */
	syss(nosys,0),			/* 162 */
	syss(nosys,0),			/* 163 */
	syss_getdirentries(),		/* 164 = getdirentries */
	syss(nosys,0),			/* 165 */
	syss(nosys,0),			/* 166 */
	syss(nosys,0),			/* 167 */
	syss(nosys,0),			/* 168 */
	syss_getdomainname(),		/* 169 = getdomainname */
	syss_setdomainname(),		/* 170 = setdomainname */
        syss_pioctl(),			/* 171 = pioctl */
        syss_setpag(),			/* 172 = setpag */
        syss_icreate(),			/* 173 = icreate */
        syss_iopen(),			/* 174 = iopen */
        syss_iread(),			/* 175 = iread */
        syss_iwrite(),			/* 176 = iwrite */
        syss_iinc(),			/* 177 = iinc */
        syss_idec(),			/* 178 = idec */
	syss(nosys,0),			/* 179 */
	syss(nosys,0),			/* 180 */
#endif	defined(vax) || defined(i386) || defined(i860)
#ifdef	mips
#undef	GENERIC
	syss(sysmips,5),		/* 151 = sysmips */
	syss(nosys,0),			/* 152 = cacheflush */
	syss(nosys,0),			/* 153 = cachectl */
	syss(nosys,0),                  /* 154 = debug */
	syss(nosys,0),			/* 155 = nosys */
	syss(nosys,0),			/* 156 = nosys */
	syss(nosys,0),			/* 157 = old nfs_mount */
	syss(nosys,0),			/* 158 = nfs_svc */
	syss_getdirentries(),		/* 159 = getdirentries */
	syss(nosys,0),			/* 160 = statfs */
	syss(nosys,0),			/* 161 = fstatfs */
	syss(nosys,0),			/* 162 = unmount */
	syss(nosys,0),			/* 163 = async_daemon */
	syss(nosys,0),			/* 164 = get file handle */
	syss_getdomainname(),		/* 165 = getdomainname */
	syss_setdomainname(),		/* 166 = setdomainname */
	syss(nosys,0),			/* 167 = old pcfs_mount */
	syss(nosys,0),			/* 168 = quotactl */
	syss(nosys,0),			/* 169 = exportfs */
	syss(nosys,0),			/* 170 = mount */
	syss(nosys,0),			/* 171 = mipshwconf */
	syss(nosys,0),			/* 172 */
	syss(nosys,0),			/* 173 */
	syss(nosys,0),			/* 174 */
	syss(nosys,0),			/* 175 */
	syss(nosys,0),			/* 176 */
	syss(nosys,0),			/* 177 */
	syss(nosys,0),			/* 178 */
	syss(nosys,0),			/* 179 */
	syss(nosys,0),			/* 180 */
#endif	mips
#ifdef	balance
#undef	GENERIC
	syss_getdirentries(),		/* 151 = getdirentries */
	syss(tmp_ctl,2),		/* 152 = tmp_ctl */
	syss(universe,0),		/* 153 = universe */
	syss(nosys,0),			/* 154 */
	syss(nosys,0),			/* 155 */
	syss(nosys,0),			/* 156 */
	syss(nosys,0),			/* 157 */
	syss(nosys,0),			/* 158 */
	syss(nosys,0),			/* 159 */
	syss(nosys,0),			/* 160 */
	syss(nosys,0),			/* 161 */
	syss(nosys,0),			/* 162 */
	syss(nosys,0),			/* 163 */
	syss(nosys,0),			/* 164 */
	syss(nosys,0),			/* 165 */
	syss(nosys,0),			/* 166 */
	syss(nosys,0),			/* 167 */
	syss(nosys,0),			/* 168 */
	syss(nosys,0),			/* 169 */
	syss(nosys,0),			/* 170 */
	syss(nosys,0),			/* 171 */
	syss(nosys,0),			/* 172 */
	syss(nosys,0),			/* 173 */
	syss(nosys,0),			/* 174 */
	syss(nosys,0),			/* 175 */
	syss(nosys,0),			/* 176 */
	syss(nosys,0),			/* 177 */
	syss(nosys,0),			/* 178 */
	syss(nosys,0),			/* 179 */
	syss(nosys,0),			/* 180 */
#endif	BALANCE
#ifdef	GENERIC
	syss(nosys,0),			/* 151 */
	syss(nosys,0),			/* 152 */
	syss(nosys,0),			/* 153 */
	syss(nosys,0),			/* 154 */
	syss(nosys,0),			/* 155 */
	syss_getdirentries(),		/* 156 = getdirentries */
	syss(nosys,0),			/* 157 */
	syss(nosys,0),			/* 158 */
	syss(nosys,0),			/* 159 */
	syss(nosys,0),			/* 160 */
	syss(nosys,0),			/* 161 */
	syss_getdomainname(),		/* 162 = getdomainname */
	syss_setdomainname(),		/* 163 = getdomainname */
	syss(nosys,0),			/* 164 */
	syss(nosys,0),			/* 165 */
	syss(nosys,0),			/* 166 */
	syss(nosys,0),			/* 167 */
	syss(nosys,0),			/* 168 */
	syss(nosys,0),			/* 169 */
	syss(nosys,0),			/* 170 */
	syss(nosys,0),			/* 171 */
	syss(nosys,0),			/* 172 */
	syss(nosys,0),			/* 173 */
	syss(nosys,0),			/* 174 */
	syss(nosys,0),			/* 175 */
	syss(nosys,0),			/* 176 */
	syss(nosys,0),			/* 177 */
	syss(nosys,0),			/* 178 */
	syss(nosys,0),			/* 179 */
	syss(nosys,0),			/* 180 */
#endif	GENERIC
#ifdef	mips
	syss(nosys,0),			/* 181 = plock */
	syss(nosys,0),			/* 182 = lockf (future) */
	syss(nosys,0),			/* 183 = ustat */
	syss(nosys,0),			/* 184 = getmnt */
	syss(nosys,0),			/* 185 = mount */
	syss(nosys,0),			/* 186 = umount */
	syss(nosys,0),			/* 187 = sigpending */
	syss(nosys,0),			/* 188 */
	syss(nosys,0),			/* 189 */
	syss_iopen(),			/* 190 = iopen */
	syss_iread(),			/* 191 = iread */
	syss_iwrite(),			/* 192 = iwrite */
	syss_iinc(),			/* 193 = iinc */
	syss_idec(),			/* 194 = idec */
	syss_pioctl(),			/* 195 = pioctl */
	syss_setpag(),			/* 196 = setpag */
	syss_icreate(),			/* 197 = icreate */
	syss(nosys,0),			/* 198 = afs_call */
	syss(nosys,0),			/* 199 */
	syss(nosys,0),			/* 200 */
	syss(nosys,0),			/* 201 */
	syss(nosys,0),			/* 202 */
	syss(nosys,0),			/* 203 */
	syss(nosys,0),			/* 204 */
	syss(nosys,0),			/* 205 */
	syss(nosys,0),			/* 206 */
	syss(nosys,0),			/* 207 */
	syss(nosys,0),			/* 208 */
	syss(nosys,0),			/* 209 */
	syss(nosys,0),			/* 210 */
	syss(nosys,0),			/* 211 */
	syss(nosys,0),			/* 212 */
	syss(nosys,0),			/* 213 */
	syss(nosys,0),			/* 214 */
	syss(nosys,0),			/* 215 */
	syss(nosys,0),			/* 216 */
	syss(nosys,0),			/* 217 */
	syss(nosys,0),			/* 218 */
	syss(nosys,0),			/* 219 */
	syss(nosys,0),			/* 220 */
	syss(nosys,0),			/* 221 */
	syss(nosys,0),			/* 222 */
	syss(nosys,0),			/* 223 */
	/* standardized afs system call numbers */
	syss_pioctl(),			/* 224 = pioctl */
	syss_setpag(),			/* 225 = setpag */
	syss_iopen(),			/* 226 = iopen */
	syss_icreate(),			/* 227 = icreate */
	syss_iread(),			/* 228 = iread */
	syss_iwrite(),			/* 229 = iwrite */
	syss_iinc(),			/* 230 = iinc */
	syss_idec(),			/* 231 = idec */
	syss(nosys,0),			/* 232 = afs_call */
#ifdef	PMAX
	syss(nosys,0),			/* 233 */
	syss(nosys,0),			/* 234 */
	syss(nosys,0),			/* 235 */
	syss(nosys,0),			/* 236 */
	syss(nosys,0),			/* 237 */
	syss(nosys,0),			/* 238 */
	syss(nosys,0),			/* 239 */
	syss(nosys,0),			/* 240 */
	syss(nosys,0),			/* 241 */
	syss(nosys,0),			/* 242 */
	syss(nosys,0),			/* 243 */
	syss(nosys,0),			/* 244 */
	syss(nosys,0),			/* 245 */
	syss(nosys,0),			/* 246 */
	syss(nosys,0),			/* 247 */
	syss(nosys,0),			/* 248 */
	syss(nosys,0),			/* 249 */
	syss(nosys,0),			/* 250 */
	syss(nosys,0),			/* 251 */
	syss(nosys,0),			/* 252 */
	syss(nosys,0),			/* 253 */
	syss(nosys,0),			/* 254 */
	syss(nosys,0),			/* 255 */
	syss(nosys,0),			/* 256 = getsysinfo */
	syss(nosys,0),			/* 257 = setsysinfo */
#endif	PMAX
#else	mips
	syss(nosys,0),			/* 181 */
#endif	mips
};
int	nsysent = sizeof (sysent) / sizeof (sysent[0]);
int	ncmusysent = sizeof (cmusysent) / sizeof (cmusysent[0]);
int	nallsysent = (sizeof (cmusysent)+sizeof(sysent)) / sizeof (sysent[0]);
