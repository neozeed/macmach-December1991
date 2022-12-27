/* 
 * Mach Operating System
 * Copyright (c) 1991 Carnegie-Mellon University
 * Copyright (c) 1990 Carnegie-Mellon University
 * Copyright (c) 1989 Carnegie-Mellon University
 * All rights reserved.  The CMU software License Agreement specifies
 * the terms and conditions for use and redistribution.
 */
/*
 * HISTORY
 * $Log:	bsd_user_side.c,v $
 * Revision 2.15  91/09/03  11:11:25  jsb
 * 	From Intel SSD: Added i860 cases to parallel mips cases.
 * 	[91/09/02  14:55:05  jsb]
 * 
 * Revision 2.14  91/03/20  15:01:21  dbg
 * 	Fix for ANSI C.
 * 	[91/02/22            dbg]
 * 
 * Revision 2.13  90/11/05  15:31:12  rpd
 * 	Added spin_lock_t.
 * 	[90/10/31            rwd]
 * 
 * Revision 2.12  90/10/25  15:06:18  rwd
 * 	Check for MACH_RCV_TOO_LARGE and check data size in pioctl().
 * 	[90/10/03            rwd]
 * 
 * Revision 2.11  90/08/06  15:29:59  rwd
 * 	Turn all if () EPRINT to if () {EPRINT} since the pmax
 * 	compiler did the wrong thing.
 * 	[90/08/03            rwd]
 * 	Fix for !shared_enabled and not MAP_UAREA and/or MAP_FILE.
 * 	[90/07/17            rwd]
 * 	Turn off debugging.
 * 	[90/06/27            rwd]
 * 	Change to reflect change in bsd_select().
 * 	[90/06/25            rwd]
 * 	Remove extra copying in bsd_select().
 * 	[90/06/11            rwd]
 * 	Fixed readv/writev bug.
 * 	[90/06/05            rwd]
 * 
 * Revision 2.10  90/06/19  23:06:36  rpd
 * 	Fixed argument to e_readwrite in e_readv.
 * 	[90/06/06            rpd]
 * 
 * Revision 2.9  90/06/02  15:20:16  rpd
 * 	Converted new functions to new IPC.
 * 	[90/06/01            rpd]
 * 
 * 	Fixed check for negative numbers in e_emulator_error.
 * 	[90/05/12            rpd]
 * 
 * 	Removed e_check_server_signals.
 * 	[90/05/10            rpd]
 * 	Added missing cast in e_sigvec.
 * 	[90/05/03            rpd]
 * 	Converted to new IPC.
 * 	[90/03/26  19:25:49  rpd]
 * 
 * Revision 2.8  90/05/29  20:22:24  rwd
 * 	Fix references to nocopy case of e_readwrite to release the
 * 	shared buffer themselves when finished.
 * 	[90/05/16            rwd]
 * 	Fix couple of coding problems in e_readv.
 * 	[90/05/14            rwd]
 * 	Make readv and writev use the existing shared interfaces.
 * 	[90/05/08            rwd]
 * 
 * 	Fix non shared_enabled case.
 * 	[90/05/02            rwd]
 * 	Pass count arguments for all path_name_ts.
 * 	[90/04/06            dbg]
 * 
 * 	Do argument shuffling for exec entirely within emulator.
 * 	[90/03/22            dbg]
 * 
 * 	Added MAP_FILE case to e_write.
 * 	[90/04/04            rwd]
 * 
 * Revision 2.7  90/05/21  13:45:30  dbg
 * 	Fix count passed to copystr from copy_args.
 * 	[90/05/11            dbg]
 * 
 * 	Always set u_sigtramp.
 * 	[90/04/23            dbg]
 * 
 * 	Pass count arguments for all path_name_ts.
 * 	[90/04/06            dbg]
 * 
 * 	Do argument shuffling for exec entirely within emulator.
 * 	[90/03/22            dbg]
 * 
 * Revision 2.6  90/03/14  21:22:41  rwd
 * 	Changed shared locks to use share_lock macros.  Added mapped
 * 	sigvec.
 * 	[90/02/16            rwd]
 * 	Fixed typo.  Added e_emulator_error.  Added MAP_UAREA code.
 * 	[90/01/26            rwd]
 * 
 * Revision 2.5  89/11/29  15:26:05  af
 * 	Made sigvec take an extra param, to support those implementations 
 * 	where the signal trampoline code address is passed to the kernel
 * 	dinamically (mips, balance, mmax...).
 * 	[89/11/16  15:15:07  af]
 * 
 * Revision 2.4  89/11/15  13:26:31  dbg
 * 	Add readv, writev, pioctl.
 * 	[89/11/07            dbg]
 * 	Add table call.
 * 	[89/10/25            dbg]
 * 
 * 	In exec, pass entry_count through to bsd_execve.
 * 	[89/10/24            dbg]
 * 
 * Revision 2.3  89/10/17  11:23:52  rwd
 * 	Add interrupt return value to all calls.
 * 	[89/09/21            dbg]
 * 
 * Revision 2.2  89/08/09  14:35:14  rwd
 * 	Set buflen for all out sockarg_t
 * 	[89/08/08            rwd]
 * 
 *
 */
/*
 * Glue routines between traps and MiG interfaces.
 */

#include <mach_init.h>
#include <mach/mig_errors.h>
#include <uxkern/bsd_1.h>
#include <uxkern/bsd_msg.h>

#include <sys/varargs.h>
#include <sys/resource.h>
#include <sys/stat.h>
#include <sys/file.h>
#include <sys/uio.h>
#include <sys/syscall.h>
#include <sys/errno.h>
#include <sys/viceioctl.h>

#ifdef	MAP_UAREA
#include <sys/ushared.h>

extern int shared_enabled;
extern struct ushared_ro *shared_base_ro;
extern struct ushared_rw *shared_base_rw;
extern char *shared_readwrite;
extern int readwrite_inuse;
extern spin_lock_t readwrite_lock;
#endif	MAP_UAREA

extern	mach_port_t	our_bsd_server_port;

#define	DEBUG 0

#if	DEBUG
#ifdef	__STDC__
#define	EPRINT(a) e_emulator_error ## a ##
#else
#define	EPRINT(a) e_emulator_error/**/a/**/
#endif
#else	DEBUG
#define	EPRINT(a)
#endif	DEBUG

/*
 * Copy zero-terminated string and return its length,
 * including the trailing zero.  If longer than max_len,
 * return -1.
 */
int
copystr(from, to, max_len)
	register char	*from;
	register char	*to;
	register int	max_len;
{
	register int	count;

	count = 0;
	while (count < max_len) {
	    count++;
	    if ((*to++ = *from++) == 0) {
		return (count);
	    }
	}
	return (-1);
}

int
copy_args(argp, arg_count, arg_addr, arg_size, char_count)
	register char	**argp;
	int		*arg_count;	/* OUT */
	vm_offset_t	*arg_addr;	/* IN/OUT */
	vm_size_t	*arg_size;	/* IN/OUT */
	unsigned int	*char_count;	/* IN/OUT */
{
	register char		*ap;
	register int		len;
	register unsigned int	cc = *char_count;
	register char		*cp = (char *)*arg_addr + cc;
	register int		na = 0;

	while ((ap = *argp++) != 0) {
	    na++;
	    while ((len = copystr(ap, cp, *arg_size - cc)) < 0) {
		/*
		 * Allocate more
		 */
		vm_offset_t	new_arg_addr;

		if (vm_allocate(mach_task_self(),
				&new_arg_addr,
				(*arg_size) * 2,
				TRUE) != KERN_SUCCESS)
		    return (E2BIG);
		(void) vm_copy(mach_task_self(),
				*arg_addr,
				*arg_size,
				new_arg_addr);
		(void) vm_deallocate(mach_task_self(),
				*arg_addr,
				*arg_size);
		*arg_addr = new_arg_addr;
		*arg_size *= 2;

		cp = (char *)*arg_addr + cc;
	    }
	    cc += len;
	    cp += len;
	}
	*arg_count = na;
	*char_count = cc;
	return (0);
}

int
e_exec_call(serv_port, interrupt,
		fname, argp, envp, new_arg_addr, entry, entry_count)
	mach_port_t	serv_port;
	boolean_t	*interrupt;	/* OUT */
	char		*fname;
	char		**argp;
	char		**envp;
	vm_offset_t	*new_arg_addr;	/* OUT */
	vm_offset_t	*entry;		/* pointer to OUT array */
	unsigned int	*entry_count;	/* OUT */
{
	vm_offset_t	arg_addr;
	vm_size_t	arg_size;
	int		arg_count, env_count;
	unsigned int	char_count = 0;
	int		error;
	vm_offset_t	arg_start;
	cfname_t	cfname;
	cfname_t	cfarg;
	path_name_t	save_fname;

	/*
	 * Copy the argument and environment strings into
	 * contiguous memory.  Since most argument lists are
	 * small, we allocate a page to start, and add more
	 * if we need it.
	 */
	arg_size = vm_page_size;
	(void) vm_allocate(mach_task_self(),
			   &arg_addr,
			   arg_size,
			   TRUE);

	if (argp) {
	    if (copy_args(argp, &arg_count,
			&arg_addr, &arg_size, &char_count) != 0)
		return (E2BIG);
	}
	else {
	    arg_count = 0;
	}

	if (envp) {
	    if (copy_args(envp, &env_count,
			&arg_addr, &arg_size, &char_count) != 0)
		return (E2BIG);
	}
	else {
	    env_count = 0;
	}

	/*
	 * Save the file name in case a command file needs it.
	 * (The file name is in the old program address space,
	 * and will disappear if the exec is successful.)
	 */
	strcpy(save_fname, fname);

	/*
	 * Exec the program.  Get back the command file name (if any),
	 * and the entry information (machine-dependent).
	 */
	error = bsd_execve(serv_port,
			interrupt,
			save_fname, strlen(save_fname) + 1,
			cfname,
			cfarg,
			entry,
			&entry_count);
	if (error) {
	    (void) vm_deallocate(mach_task_self(), arg_addr, arg_size);
	    return (error);
	}

	/*
	 * Set up new argument list.  If command file name and argument
	 * have been found, use them instead of argv[0].
	 */
	{
	    register char	**ap;
	    register char	*cp;
	    register char	*argstrings = (char *)arg_addr;
	    register int	total_args;
	    register int	len;
	    char		*cmd_args[4];
	    register char	**xargp = 0;

	    total_args = arg_count + env_count;
	    if (cfname[0] != '\0') {
		/*
		 * argv[0] becomes 'cfname'; skip real argv[0].
		 */
		len = strlen(argstrings) + 1;
		argstrings += len;
		char_count -= len;

		xargp = cmd_args;
		*xargp++ = cfname;
		char_count += (strlen(cfname) + 1);

		if (cfarg[0] != '\0') {
		    *xargp++ = cfarg;
		    char_count += (strlen(cfarg) + 1);
		    total_args++;
		}
		*xargp++ = save_fname;
		char_count += (strlen(save_fname) + 1);
		total_args++;

		*xargp = 0;
		xargp = cmd_args;
	    }
	    char_count = (char_count + NBPW - 1) & ~(NBPW - 1);

	    arg_start = set_arg_addr(total_args*NBPW + 3*NBPW + char_count
					 + NBPW);
	    ap = (char **)arg_start;
	    cp = (char *)arg_start + total_args*NBPW + 3*NBPW;

	    *ap++ = (char *)(total_args - env_count);
	    for (;;) {

		if (total_args == env_count)
		    *ap++ = 0;
		if (--total_args < 0)
		    break;
		*ap++ = cp;
		if (xargp && *xargp)
		    len = copystr(*xargp++, cp, (unsigned)char_count);
		else {
		    len = copystr(argstrings, cp, (unsigned)char_count);
		    argstrings += len;
		}
		cp += len;
		char_count -= len;
	    }
	    *ap = 0;
	}

#ifdef	STACK_GROWTH_UP
	*new_arg_addr = ((vm_offset_t) cp + NBPW - 1) & ~(NBPW - 1);
#else	STACK_GROWTH_UP
	*new_arg_addr = arg_start;
#endif	STACK_GROWTH_UP

	(void) vm_deallocate(mach_task_self(), arg_addr, arg_size);
	return (error);
}

int
e_getrusage(serv_port, interrupt, which, rusage, rval)
	mach_port_t	serv_port;
	boolean_t	*interrupt;	/* OUT */
	int		which;
	register
	struct rusage	*rusage;
	int		*rval;
{
	register int	error;
	struct thread_basic_info bi;
	unsigned int	bi_count;

	error = bsd_getrusage(serv_port, interrupt, which, rusage);
	if (error || which != RUSAGE_SELF)
	    return (error);

	bi_count = THREAD_BASIC_INFO_COUNT;
	(void) thread_info(mach_thread_self(),
			THREAD_BASIC_INFO,
			(thread_info_t)&bi,
			&bi_count);

	rusage->ru_utime.tv_sec  = bi.user_time.seconds;
	rusage->ru_utime.tv_usec = bi.user_time.microseconds;
	rusage->ru_stime.tv_sec  = bi.system_time.seconds;
	rusage->ru_stime.tv_usec = bi.system_time.microseconds;

	return (0);
}

int
e_write(serv_port, interrupt, fileno, data, count, rval)
	mach_port_t	serv_port;
	boolean_t	*interrupt;	/* out */
	int		fileno;
	char		*data;
	unsigned int	count;
	int		*rval;
{
	extern int	bsd_write_short();
	extern int	bsd_write_long();

#ifdef	MAP_UAREA
#ifdef	MAP_FILE
	int result = 0;
	if (shared_enabled &&
	    e_maprw(serv_port, interrupt, fileno, data,
		    count, rval, 1, &result))
	    return (result);
	else
#endif	MAP_FILE
	if (shared_enabled && count <= vm_page_size) {
	    spin_lock(&readwrite_lock);
	    if (readwrite_inuse) {
		spin_unlock(&readwrite_lock);
		goto server;
	    }
	    readwrite_inuse = 1;
	    spin_unlock(&readwrite_lock);
	    return e_readwrite(serv_port, interrupt, fileno, data, 
			       count, rval, 1, 1);
	}
server:
	if (result != 0) {
	    EPRINT(("e_write(%d, %x, %x)\n",fileno, data, count));
	    EPRINT(("shared = %x, res = %x, inuse = %x\n",shared_enabled, result, readwrite_inuse));
	}
#endif MAP_UAREA
	return (((count <= SMALL_ARRAY_LIMIT) ? bsd_write_short
					      : bsd_write_long
		)(serv_port,
		  interrupt,
		  fileno,
		  data,
		  count,
		  &rval[0])
	       );
}

int
e_send(serv_port, interrupt, fileno, data, count, flags, rval)
	mach_port_t	serv_port;
	boolean_t	*interrupt;
	int		fileno;
	char		*data;
	unsigned int	count;
	int		flags;
	int		*rval;
{
	extern int	bsd_send_short();
	extern int	bsd_send_long();

	return (((count <= SMALL_ARRAY_LIMIT) ? bsd_send_short
					      : bsd_send_long
		)(serv_port,
		  interrupt,
		  fileno,
		  flags,
		  data,
		  count,
		  &rval[0]));
}

int
e_sendto(serv_port, interrupt, fileno, data, count, flags, to, tolen, rval)
	mach_port_t	serv_port;
	boolean_t	*interrupt;
	int		fileno;
	char		*data;
	unsigned int	count;
	int		flags;
	char		*to;
	int		tolen;
	int		*rval;
{
	extern int	bsd_sendto_short();
	extern int	bsd_sendto_long();

	return (((count <= SMALL_ARRAY_LIMIT) ? bsd_sendto_short
					      : bsd_sendto_long
		)(serv_port,
		  interrupt,
		  fileno,
		  flags,
		  to,
		  tolen,
		  data,
		  count,
		  &rval[0]));
}

int
e_recvfrom(serv_port, interrupt,
	   fileno, data, len, flags, from, fromlenaddr, rval)
	mach_port_t	serv_port;
	boolean_t	*interrupt;
	int		fileno;
	char		*data;
	int		len;
	int		flags;
	char		*from;
	int		*fromlenaddr;
	int		*rval;
{
	/*
	 * We receive the address into a temporary buffer,
	 * since the MiG interface always returns the full
	 * amount.
	 */
	register int	error;
	unsigned int	data_count;
	sockarg_t	from_buf;
	int		from_count;

	data_count = len;
	from_count = *fromlenaddr;

	if (len <= SMALL_ARRAY_LIMIT) {
	    error = bsd_recvfrom_short(serv_port,
			interrupt,
			fileno,
			flags,
			len,
			from_buf,
			&from_count,
			data,
			&data_count);
	}
	else {
	    char *		data_addr;

	    error = bsd_recvfrom_long(serv_port,
			interrupt,
			fileno,
			flags,
			len,
			from_buf,
			&from_count,
			&data_addr,
			&data_count);
	    if (error == 0) {
		bcopy(data_addr, data, data_count);
		(void) vm_deallocate(mach_task_self(),
				(vm_offset_t)data_addr,
				data_count);
	    }
	}

	if (error == 0) {
	    rval[0] = data_count;
	    if (from) {
		bcopy(from_buf, from, from_count);
		*fromlenaddr = from_count;
	    }
	}
	return (error);
}

int
e_stat(serv_port, interrupt, fname, ub, rval)
	mach_port_t	serv_port;
	boolean_t	*interrupt;
	char		*fname;
	struct stat	*ub;
	int		*rval;
{
	return (e_stat_call(serv_port, interrupt, fname, ub, TRUE));
}

int
e_lstat(serv_port, interrupt, fname, ub, rval)
	mach_port_t	serv_port;
	boolean_t	*interrupt;
	char		*fname;
	struct stat	*ub;
	int		*rval;
{
	return (e_stat_call(serv_port, interrupt, fname, ub, FALSE));
}

int
e_stat_call(serv_port, interrupt, fname, ub, follow)
	mach_port_t	serv_port;
	boolean_t	*interrupt;
	char		*fname;
	register
	struct stat	*ub;
	boolean_t	follow;
{
	int		error;
	statb_t		statbuf;

	error = bsd_stat(serv_port,
			interrupt,
			follow,
			fname, strlen(fname) + 1,
			&statbuf);
	if (error)
	    return (error);

	/*
	 * Copy out stat fields
	 */
	ub->st_dev	= statbuf.s_dev;
	ub->st_ino	= statbuf.s_ino;
	ub->st_mode	= statbuf.s_mode;
	ub->st_nlink	= statbuf.s_nlink;
	ub->st_uid	= statbuf.s_uid;
	ub->st_gid	= statbuf.s_gid;
	ub->st_rdev	= statbuf.s_rdev;
	ub->st_size	= statbuf.s_size;
	ub->st_atime	= statbuf.s_atime;
	ub->st_mtime	= statbuf.s_mtime;
	ub->st_ctime	= statbuf.s_ctime;
	ub->st_blksize	= statbuf.s_blksize;
	ub->st_blocks	= statbuf.s_blocks;

	return (0);
}

int
e_readlink(serv_port, interrupt, name, buf, count, rval)
	mach_port_t	serv_port;
	boolean_t	*interrupt;
	char		*name;
	char		*buf;
	int		count;
	int		*rval;
{
	int		buflen;
	register int	error;

	buflen = count;
	error = bsd_readlink(serv_port,
			interrupt,
			name, strlen(name) + 1,
			count,		/* max length */
			buf,
			&buflen);
	if (error == 0)
	    rval[0] = buflen;
	return (error);
}

int
e_acct(serv_port, interrupt, fname, rval)
	mach_port_t	serv_port;
	boolean_t	*interrupt;
	char		*fname;
	int		*rval;
{
	if (fname) {
	    return (bsd_acct(serv_port,
			interrupt,
			TRUE,
			fname, strlen(fname) + 1));
	}
	else {
	    return (bsd_acct(serv_port,
			interrupt,
			FALSE,
			"", 1));
	}
}

struct timeval	zero_time = { 0, 0 };

int
e_select(serv_port, interrupt, nd, in, ou, ex, tv, rval)
	mach_port_t	serv_port;
	boolean_t	*interrupt;
	int		nd;
	fd_set		*in;
	fd_set		*ou;
	fd_set		*ex;
	timeval_t	*tv;
	int		*rval;
{
	register int	ni_size;
	register int	error;
	fd_set		zeros;
	fd_set		in_set, ou_set, ex_set;

	FD_ZERO(&zeros);

	if (nd > NOFILE)
	    nd = NOFILE;	/* 'forgiving, if slightly wrong' */
	ni_size = howmany(nd, NFDBITS) * sizeof(fd_mask);

	if (in)
	    bcopy((char *)in, (char *)&in_set, (unsigned)ni_size);
	if (ou)
	    bcopy((char *)ou, (char *)&ou_set, (unsigned)ni_size);
	if (ex)
	    bcopy((char *)ex, (char *)&ex_set, (unsigned)ni_size);

	error = bsd_select(serv_port,
			interrupt,
			nd,
			(in) ? &in_set : &zeros,
			(ou) ? &ou_set : &zeros,
			(ex) ? &ex_set : &zeros,
			(in != 0),
			(ou != 0),
			(ex != 0),
			(tv != 0),
			(tv) ? *tv : zero_time,
			&rval[0]);

	if (error)
	    return (error);

	if (in)
	    bcopy((char *)&in_set, (char *)in, (unsigned)ni_size);
	if (ou)
	    bcopy((char *)&ou_set, (char *)ou, (unsigned)ni_size);
	if (ex)
	    bcopy((char *)&ex_set, (char *)ex, (unsigned)ni_size);

	return (0);
}

int
e_htg_syscall(argp, rvalp)
{
}

struct sigvec	zero_sv = { 0, 0, 0 };

int
e_sigvec(serv_port, interrupt, sig, nsv, osv, tramp)
	mach_port_t		serv_port;
	boolean_t		*interrupt;
	register int		sig;
	register struct sigvec	*nsv;
	register struct sigvec	*osv;
	unsigned		tramp;
{
	register int	error;
	struct sigvec	old_sig_vec;

#ifdef	MAP_UAREA 
    if (shared_enabled) {
	int bit;

	if (sig <= 0 || sig > NSIG || sig == SIGKILL || sig == SIGSTOP)
		return EINVAL;
	bit = sigmask(sig);
	if (osv) {
		osv->sv_handler = shared_base_rw->us_signal[sig];
		osv->sv_mask = shared_base_rw->us_usigmask[sig];
		bit = sigmask(sig);
		osv->sv_flags = 0;
		if ((shared_base_rw->us_sigonstack & bit) != 0)
			osv->sv_flags |= SV_ONSTACK;
		if ((shared_base_rw->us_sigintr & bit) != 0)
			osv->sv_flags |= SV_INTERRUPT;
	}
	if (nsv) {
		if (sig == SIGCONT && nsv->sv_handler == SIG_IGN)
			return EINVAL;
#if	defined(mips) || defined(i860)
		/*
		 * check for unaligned pc on sighandler
		 */
		if (nsv->sv_handler != SIG_IGN
		    && ((int)nsv->sv_handler & (sizeof(int)-1)))
			return EINVAL;
#endif	defined(mips) || defined(i860)
		/*
		 * Set the signal trampoline code address - the
		 * server does not always know where it is.
		 */
		shared_base_rw->us_sigtramp = (int (*)())tramp;

		share_lock(&shared_base_rw->us_siglock);
		shared_base_rw->us_signal[sig] = nsv->sv_handler;
		shared_base_rw->us_usigmask[sig] = nsv->sv_mask &~ cantmask;

		if (nsv->sv_flags & SV_INTERRUPT)
			shared_base_rw->us_sigintr |= bit;
		else
			shared_base_rw->us_sigintr &= ~bit;
		if (nsv->sv_flags & SV_ONSTACK)
			shared_base_rw->us_sigonstack |= bit;
		else
			shared_base_rw->us_sigonstack &= ~bit;
		if (nsv->sv_handler == SIG_IGN) {
			shared_base_rw->us_sig &= ~bit;
			shared_base_rw->us_sigignore |= bit;
			shared_base_rw->us_sigcatch &= ~bit;
		} else {
			shared_base_rw->us_sigignore &= ~bit;
			if (nsv->sv_handler == SIG_DFL)
				shared_base_rw->us_sigcatch &= ~bit;
			else
				shared_base_rw->us_sigcatch |= bit;
		}
		share_unlock(&shared_base_rw->us_siglock);
	}
#if	defined(mips) || defined(i860)
	{
		extern unsigned sigtramp;
		sigtramp = tramp;
	}
#endif	defined(mips) || defined(i860)
	e_checksignals(interrupt);
	return ESUCCESS;
    } else {
#endif	MAP_UAREA
	error = bsd_sigvec(serv_port,
			interrupt,
			sig,
			nsv != 0,
			(nsv) ? *nsv : zero_sv,
			&old_sig_vec,
			tramp);
	if (error == 0 && osv)
	    *osv = old_sig_vec;
#if	defined(mips) || defined(i860)
	if (error == 0) {
		extern unsigned sigtramp;
		sigtramp = tramp;
	}
#endif	defined(mips) || defined(i860)
	return (error);
#ifdef	MAP_UAREA
    }
#endif	MAP_UAREA
}

struct sigstack	zero_ss = { 0, 0 };

int
e_sigstack(serv_port, interrupt, nss, oss)
	mach_port_t	serv_port;
	boolean_t	*interrupt;
	struct sigstack	*nss;
	struct sigstack	*oss;
{
	register int	error;
	struct sigstack	old_sig_stack;

#ifdef	MAP_UAREA 
    if (shared_enabled) {
	share_lock(&shared_base_rw->us_lock);
	if (oss)
	    *oss = shared_base_rw->us_sigstack;
	if (nss)
	    shared_base_rw->us_sigstack = *nss;
	share_unlock(&shared_base_rw->us_lock);
	e_checksignals(interrupt);
	return ESUCCESS;
    } else {
#endif	MAP_UAREA
	error = bsd_sigstack(serv_port,
			interrupt,
			(nss != 0),
			(nss) ? *nss : zero_ss,
			&old_sig_stack);
	if (error == 0 && oss)
	    *oss = old_sig_stack;
	return (error);
#ifdef	MAP_UAREA
    }
#endif	MAP_UAREA
}

struct timeval	zero_tv = { 0, 0 };
struct timezone	zero_tz = { 0, 0 };

int
e_settimeofday(serv_port, interrupt, tv, tz)
	mach_port_t	serv_port;
	boolean_t	*interrupt;
	struct timeval	*tv;
	struct timezone	*tz;
{
	return (bsd_settimeofday(serv_port,
			interrupt,
			(tv != 0),
			(tv) ? *tv : zero_tv,
			(tz != 0),
			(tz) ? *tz : zero_tz));
}

struct itimerval zero_itv = { 0, 0, 0, 0 };

int e_setitimer(serv_port, interrupt, which, itv, oitv)
	mach_port_t	serv_port;
	boolean_t	*interrupt;
	int		which;
	struct itimerval *itv;
	struct itimerval *oitv;
{
	register int	error;
	struct itimerval old_itimer_val;

	error = bsd_setitimer(serv_port,
			interrupt,
			which,
			(itv != 0),
			(itv) ? *itv : zero_itv,
			&old_itimer_val);
	if (error == 0 && oitv)
	    *oitv = old_itimer_val;
	return (error);
}

int
e_accept(serv_port, interrupt, s, name, anamelen, rvalp)
	mach_port_t	serv_port;
	boolean_t	*interrupt;
	int		s;
	char *		name;
	int		*anamelen;
	int		*rvalp;
{
	register int	error;
	int		namelen = sizeof(sockarg_t);
	int		new_s;

	sockarg_t	out_name;

	error = bsd_accept(serv_port,
			interrupt,
			s,
			out_name,
			&namelen,
			&new_s);
	if (error)
	    return (error);

	if (name) {
	    if (namelen > *anamelen)
		namelen = *anamelen;
	    bcopy(out_name, name, namelen);
	    *anamelen = namelen;
	}
	*rvalp = new_s;
	return (error);
}

int
e_setsockopt(serv_port, interrupt, s, level, name, val, valsize)
	mach_port_t	serv_port;
	boolean_t	*interrupt;
	int		s;
	int		level;
	int		name;
	char *		val;
	int		valsize;
{
	return (bsd_setsockopt(serv_port,
			interrupt,
			s,
			level,
			name,
			val,
			(val) ? valsize : 0));
}

int
e_getsockopt(serv_port, interrupt, s, level, name, val, avalsize)
	mach_port_t	serv_port;
	boolean_t	*interrupt;
	int		s;
	int		level;
	int		name;
	char *		val;
	int		*avalsize;
{
	register int	error;
	int		valsize = sizeof(sockarg_t);
	sockarg_t	val_buf;

	error = bsd_getsockopt(serv_port,
			interrupt,
			s,
			level,
			name,
			val_buf,
			&valsize);
	if (error)
	    return (error);

	if (val) {
	    if (valsize > *avalsize)
		valsize = *avalsize;
	    bcopy(val_buf, val, valsize);
	    *avalsize = valsize;
	}
	return (error);
}

int
e_getsockname(serv_port, interrupt, fdes, asa, alen)
	mach_port_t	serv_port;
	boolean_t	*interrupt;
	int		fdes;
	char *		asa;
	int		*alen;
{
	register int	error;
	int		buflen = sizeof(sockarg_t);
	sockarg_t	asa_buf;

	error = bsd_getsockname(serv_port,
			interrupt,
			fdes,
			asa_buf,
			&buflen);
	if (error)
	    return (error);

	if (buflen > *alen)
	    buflen = *alen;
	bcopy(asa_buf, asa, buflen);
	*alen = buflen;

	return (error);
}

int
e_getpeername(serv_port, interrupt, fdes, asa, alen)
	mach_port_t	serv_port;
	boolean_t	*interrupt;
	int		fdes;
	char *		asa;
	int		*alen;
{
	register int	error;
	int		buflen = sizeof(sockarg_t);
	sockarg_t	asa_buf;

	error = bsd_getpeername(serv_port,
			interrupt,
			fdes,
			asa_buf,
			&buflen);
	if (error)
	    return (error);

	if (buflen > *alen)
	    buflen = *alen;
	bcopy(asa_buf, asa, buflen);
	*alen = buflen;

	return (error);
}

int
e_table(serv_port, interrupt, id, index, addr, nel, lel, rval)
	mach_port_t	serv_port;
	boolean_t	*interrupt;
	int		id;
	int		index;
	char *		addr;
	int		nel;	/* >0 ==> get, <0 ==> set */
	unsigned int	lel;
	int		*rval;
{
	register int	error;
	int		nel_done;

	if (nel < 0) {
	    /*
	     * Set.
	     */
	    error = bsd_table_set(serv_port, interrupt,
			id, index, lel, nel,
			addr, -nel*lel,
			&nel_done);
	}
	else {
	    char *		out_addr;
	    unsigned int	out_count;

	    error = bsd_table_get(serv_port, interrupt,
			id, index, lel, nel,
			&out_addr, &out_count,
			&nel_done);

	    if (error == 0) {
		/*
		 * Copy table to addr
		 */
		bcopy(out_addr, addr, lel * nel_done);
		(void) vm_deallocate(mach_task_self(),
				     (vm_offset_t)out_addr,
				     (vm_size_t) out_count);
	    }
	}
	if (error == 0)
	    *rval = nel_done;
	return (error);
}

int
e_writev(proc_port, interrupt, fdes, iov, iovcnt, rval)
	mach_port_t	proc_port;
	boolean_t	*interrupt;	/* out */
	int		fdes;
	struct iovec	*iov;
	unsigned	iovcnt;
	int		*rval;		/* out */
{
	register int	i;
	register int	len;
	register char	*cp;
	register struct iovec *iovp;
	unsigned int	count;
	int		result;
	char *		bufptr;
	struct iovec	aiov[16];
	char		buf[SMALL_ARRAY_LIMIT];
#ifdef	MAP_UAREA
	int		shared = 0;
#ifdef	MAP_FILE
	int		nocopy = 0;
	register struct file_info *fd = &shared_base_rw->us_file_info[fdes];

	if (fdes < 0 ||
	   (shared_enabled && fdes > shared_base_ro->us_lastfile)) {
	    EPRINT(("e_writev badfile"));
	    rval[0] = -1;
	    return EBADF;
	}
#endif	MAP_FILE
#endif	MAP_UAREA

	rval[0] = -1;
	if (iovcnt > sizeof(aiov)/sizeof(aiov[0])) {
	    EPRINT(("e_writev too many vectors"));
	    return (EINVAL);
	}

	bcopy((char *)iov, (char *)aiov, iovcnt * sizeof(struct iovec));

	count = 0;
	for (i = 0, iovp = aiov; i < iovcnt; i++, iovp++) {
	    len = iovp->iov_len;
	    if (len < 0) {
		EPRINT(("e_writev invalid vector length %x", len));
		return (EINVAL);
	    }
	    count += len;
	    if (count < 0) {
		EPRINT(("e_writev count < 0"));
		return (EINVAL);
	    }
	}
#ifdef	MAP_UAREA
#ifdef	MAP_FILE
	share_lock(&fd->lock);
	if (fd->mapped && fd->open) {
	    share_unlock(&fd->lock);
	    nocopy = 1;
	} else {
	    share_unlock(&fd->lock);
#endif	MAP_FILE
	    spin_lock(&readwrite_lock);
	    if (count <= vm_page_size && readwrite_inuse == 0) {
		bufptr = shared_readwrite;
		readwrite_inuse = shared = 1;
		spin_unlock(&readwrite_lock);
	    } else {
		spin_unlock(&readwrite_lock);
#endif	MAP_UAREA
		if (count <= SMALL_ARRAY_LIMIT) {
		    /*
		     * Short write.  Copy into buffer.
		     */
		    bufptr = buf;
		}
		else {
		    /*
		     * Long write.  Allocate memory to fill.
		     * (Hope that no one uses this to write large
		     *  amounts of data; we`ll lose on the copying.)
		     */
		    (void) vm_allocate(mach_task_self(),
				(vm_offset_t *)&bufptr,
				count,
				TRUE);
		}
#ifdef	MAP_UAREA
	    }
#ifdef	MAP_FILE
	}
#endif	MAP_FILE
#endif	MAP_UAREA
	cp = bufptr;
	for (i = 0, iovp = aiov; i < iovcnt; i++, iovp++) {
	    len = iovp->iov_len;
#if	defined(MAP_UAREA) && defined(MAP_FILE)
	    if (nocopy) {
		if (!e_maprw(proc_port, interrupt, fdes, iovp->iov_base, len,
		             rval, 1, &result) || result != ESUCCESS) {
		    EPRINT(("e_writev e_maprw %d",result));
		    return (result);
		}
	    } else
#endif	defined(MAP_UAREA) && defined(MAP_FILE)
	    bcopy(iovp->iov_base, cp, len);
	    cp += len;
	}

#ifdef	MAP_UAREA
#ifdef	MAP_FILE
	if (nocopy) {
	    rval[0] = count;
	    return ESUCCESS;
	}
#endif	MAP_FILE
	if (shared) {
	    result = e_readwrite(proc_port, interrupt, fdes, 0, count, 
				 rval, 1, 0);
	    spin_lock(&readwrite_lock);
	    readwrite_inuse = 0;
	    spin_unlock(&readwrite_lock);
	} else {
#endif	MAP_UAREA
	    result = ((count <= SMALL_ARRAY_LIMIT) ? bsd_write_short
						   : bsd_write_long)
			(proc_port,
			interrupt,
			fdes,
			bufptr,
			count,
			rval);

	    if (result != 0) {
		EPRINT(("e_writev bsd_write_* %d",result));
	    }
	    if (count > SMALL_ARRAY_LIMIT)
		(void) vm_deallocate(mach_task_self(), (vm_offset_t)bufptr, count);

#ifdef	MAP_UAREA
	}
#endif	MAP_UAREA
	return (result);
}

int
e_readv(proc_port, interrupt, fdes, iov, iovcnt, rval)
	mach_port_t	proc_port;
	boolean_t	*interrupt;	/* out */
	int		fdes;
	struct iovec	*iov;
	unsigned	iovcnt;
	int		*rval;		/* out */
{
	register int	i;
	register int	len;
	register struct iovec *iovp;
	unsigned int	count;
	int		result;
	int		arg[3];
	struct iovec	aiov[16];
	char		buf[SMALL_ARRAY_LIMIT];
#ifdef	MAP_UAREA
	int		shared = 0;
#ifdef	MAP_FILE
	int		nocopy = 0;
	register struct file_info *fd = &shared_base_rw->us_file_info[fdes];

	if (fdes < 0 ||
	   (shared_enabled && fdes > shared_base_ro->us_lastfile)) {
	    EPRINT(("e_writev badfile"));
	    rval[0] = -1;
	    return EBADF;
	}
#endif	MAP_FILE
#endif	MAP_UAREA

	rval[0] = -1;
	if (iovcnt > sizeof(aiov)/sizeof(aiov[0])) {
	    EPRINT(("e_readv too many vectors"));
	    return (EINVAL);
	}

	bcopy((char *)iov, (char *)aiov, iovcnt * sizeof(struct iovec));

	count = 0;
	for (i = 0, iovp = aiov; i < iovcnt; i++, iovp++) {
	    len = iovp->iov_len;
	    if (len < 0) {
		EPRINT(("e_readv invalid vector length %x", len));
		EPRINT(("        count = %d  vector =%d", count, i));
		EPRINT(("        iovcnt = %d", iovcnt));
		return (EINVAL);
	    }
	    count += len;
	    if (count < 0) {
		EPRINT(("e_readv count < 0"));
		return (EINVAL);
	    }
	}

	arg[0] = fdes;
	arg[2] = (int)count;

#ifdef	MAP_UAREA
#ifdef	MAP_FILE
	share_lock(&fd->lock);
	if (fd->mapped && fd->open) {
	    share_unlock(&fd->lock);
	    nocopy = 1;
	} else {
	    share_unlock(&fd->lock);
#endif	MAP_FILE
	    spin_lock(&readwrite_lock);
	    if (count <= vm_page_size && readwrite_inuse == 0) {
		arg[1] = (int) shared_readwrite;
		readwrite_inuse = shared = 1;
		spin_unlock(&readwrite_lock);
	    } else {
		spin_unlock(&readwrite_lock);
#endif	MAP_UAREA
		if (count <= SMALL_ARRAY_LIMIT) {
		    /*
		     * Short read.  Copy into buffer.
		     */
		    arg[1] = (int)buf;
		}
		else {
		    /*
		     * Long read.  Allocate memory to fill.
		     * (Hope that no one uses this to read large
		     *  amounts of data; we`ll lose on the copying.)
		     */
		    (void) vm_allocate(mach_task_self(),
				(vm_offset_t *)&arg[1],
				count,
				TRUE);
		}
#ifdef	MAP_UAREA
	    }
#ifdef	MAP_FILE
	}

	if (nocopy) {
	    result = 0;
	} else
#endif	MAP_FILE
	    if (shared) {
		result = e_readwrite(proc_port, interrupt, fdes, 0,
				     count, rval, 0, 0);
		if (rval[0]>count) {
		   EPRINT(("e_readv e_readwrite ask %d got %d",count,rval[0]));
		}
		count = rval[0];
	    } else
#endif	MAP_UAREA
	    {
		result = emul_generic(proc_port, interrupt,
				      SYS_read, arg, rval);
		if (rval[0]>count) {
		    EPRINT(("e_readv SYS_read ask %d got %d",count,rval[0]));
		}
		count = rval[0];
		if (result != 0) {
			EPRINT(("e_readv SYS_read %d",result));
		}
	    }

	if (result == 0) {
	    register char	*cp;

	    cp = (char *)arg[1];

	    for (i = 0, iovp = aiov; i < iovcnt; i++, iovp++) {
		len = iovp->iov_len;
#if	defined(MAP_UAREA) && defined(MAP_FILE)
		if (nocopy) {
		    if (!e_maprw(proc_port, interrupt, fdes, iovp->iov_base,
				 len, rval, 0, &result) || result != ESUCCESS){
			EPRINT(("e_readv e_maprw %d",result));
			rval[0] = -1;
			return (result);
		    }
		    if (rval[0] == 0) {
			rval[0] = (int)(cp - arg[1]);
			if (rval[0] > arg[2]) {
			    EPRINT(("r_readv ask %d got %d",arg[2],rval[0]));
			}
			return (ESUCCESS);
		    }
		} else
#endif	defined(MAP_UAREA) && defined(MAP_FILE)
		    bcopy(cp, iovp->iov_base, len);
		cp += len;
	    }
	}

#ifdef	MAP_UAREA
	if (shared) {
	    spin_lock(&readwrite_lock);
	    readwrite_inuse = 0;
	    spin_unlock(&readwrite_lock);
	}
#endif	MAP_UAREA

	if (count > SMALL_ARRAY_LIMIT)
#ifdef	MAP_UAREA
	    if (!shared)
#ifdef	MAP_FILE
		if(!nocopy)
#endif	MAP_FILE
#endif	MAP_UAREA
		    (void) vm_deallocate(mach_task_self(), (vm_offset_t)arg[1],
					 count);

	rval[0] = count;
	if (rval[0] > arg[2]) {
	    EPRINT(("r_readv(2) ask %d got %d",arg[2],rval[0]));
	}
	return (result);

}

int
e_pioctl(proc_port, interrupt, path, com, data, follow)
	mach_port_t	proc_port;
	boolean_t	*interrupt;
	char		*path;
	int		com;
	struct ViceIoctl *data;
	int		follow;
{
	register int	result;
	unsigned int	in_count;
	unsigned int	out_count;

	if (data->in == 0)
	    in_count = 0;
	else
	    in_count = data->in_size;
	if (in_count > SMALL_ARRAY_LIMIT)
	    in_count = SMALL_ARRAY_LIMIT;

	if (data->out == 0)
	    out_count = 0;
	else    
	    out_count = data->out_size;
	if (out_count > SMALL_ARRAY_LIMIT)
	    out_count = SMALL_ARRAY_LIMIT;

	result = bsd_pioctl(proc_port,
			interrupt,
			path, strlen(path) + 1,
			com,
			follow,
			data->in,
			in_count,
			out_count,	/* amount wanted */
			data->out,
			&out_count);	/* amount returned */
	if (result == MIG_ARRAY_TOO_LARGE)
	    result = EINVAL;
	if (result == MACH_RCV_TOO_LARGE)
	    result = EINVAL;
	return (result);
}

#define MAXPRINT 128
#define putchar(x) if (pos<MAXPRINT) error[pos++] = (x); else goto done
int
e_emulator_error(fmt, va_alist)
	char *fmt;
	va_dcl
{
	va_list adx;
	register int b, c, i;
	char *s;
	u_long	value;
	char prbuf[11];
	register char *cp;
	char error[MAXPRINT];
	int pos = 0;

	va_start(adx);
loop:
	while ((c = *fmt++) != '%') {
	    if (c == '\0') {
		goto done;
	    }
	    putchar(c);
	}
	c = *fmt++;
	switch (c) {
	    case 'x': case 'X':
		b = 16;
		goto number;
	    case 'd': case 'D':
	    case 'u':
		b = 10;
		goto number;
	    case 'o': case 'O':
		b = 8;
number:
		value = va_arg(adx, u_long);
		if (b == 10 && (long)value < 0) {
		    putchar('-');
		    value = -value;
		}
		cp = prbuf;
		do {
		    *cp++ = "0123456789abcdef"[value%b];
		    value /= b;
		} while (value);
		do {
		    putchar(*--cp);
		} while (cp > prbuf);
		break;
	    case 'c':
		value = va_arg(adx, u_long);
		for (i = 24; i >= 0; i -= 8)
			if (c = (value >> i) & 0x7f)
				putchar(c);
		break;
	    case 's':
		s = va_arg(adx, char *);
		i = 0;
		while (c = *s++) {
		    putchar(c);
		}
		break;
	    case '%':
		putchar('%');
		break;
	}
	goto loop;
done:
	error[pos]='\0';
	va_end(adx);
	return bsd_emulator_error(our_bsd_server_port, error, pos+1);
}
int
e_chdir(proc_port, interrupt, fname)
	mach_port_t	proc_port;
	boolean_t	*interrupt;
	char		*fname;
{
	return bsd_chdir(proc_port,
			interrupt,
			fname, strlen(fname) + 1);
}

int
e_chroot(proc_port, interrupt, fname)
	mach_port_t	proc_port;
	boolean_t	*interrupt;
	char		*fname;
{
	return bsd_chroot(proc_port,
			interrupt,
			fname, strlen(fname) + 1);
}

int
e_open(proc_port, interrupt, fname, mode, crtmode, rval)
	mach_port_t	proc_port;
	boolean_t	*interrupt;
	char		*fname;
	int		mode;
	int		crtmode;
	int		*rval;
{
	return bsd_open(proc_port,
			interrupt,
			fname, strlen(fname) + 1,
			mode,
			crtmode,
			rval);
}

int
e_creat(proc_port, interrupt, fname, fmode, rval)
	mach_port_t	proc_port;
	boolean_t	*interrupt;
	char		*fname;
	int		fmode;
	int		*rval;
{
	return bsd_creat(proc_port,
			interrupt,
			fname, strlen(fname) + 1,
			fmode,
			rval);
}

int
e_mknod(proc_port, interrupt, fname, fmode, dev)
	mach_port_t	proc_port;
	boolean_t	*interrupt;
	char		*fname;
	int		fmode;
	int		dev;
{
	return bsd_mknod(proc_port,
			interrupt,
			fname, strlen(fname) + 1,
			fmode,
			dev);
}

int
e_link(proc_port, interrupt, target, linkname)
	mach_port_t	proc_port;
	boolean_t	*interrupt;
	char		*target;
	char		*linkname;
{
	return bsd_link(proc_port,
			interrupt,
			target, strlen(target) + 1,
			linkname, strlen(linkname) + 1);
}

int
e_symlink(proc_port, interrupt, target, linkname)
	mach_port_t	proc_port;
	boolean_t	*interrupt;
	char		*target;
	char		*linkname;
{
	return bsd_symlink(proc_port,
			interrupt,
			target, strlen(target) + 1,
			linkname, strlen(linkname) + 1);
}

int
e_unlink(proc_port, interrupt, fname)
	mach_port_t	proc_port;
	boolean_t	*interrupt;
	char		*fname;
{
	return bsd_unlink(proc_port,
			interrupt,
			fname, strlen(fname) + 1);
}

int
e_access(proc_port, interrupt, fname, fmode)
	mach_port_t	proc_port;
	boolean_t	*interrupt;
	char		*fname;
	int		fmode;
{
	return bsd_access(proc_port,
			interrupt,
			fname, strlen(fname) + 1,
			fmode);
}

int
e_chmod(proc_port, interrupt, fname, fmode)
	mach_port_t	proc_port;
	boolean_t	*interrupt;
	char		*fname;
	int		fmode;
{
	return bsd_chmod(proc_port,
			interrupt,
			fname, strlen(fname) + 1,
			fmode);
}

int
e_chown(proc_port, interrupt, fname, uid, gid)
	mach_port_t	proc_port;
	boolean_t	*interrupt;
	char		*fname;
	int		uid;
	int		gid;
{
	return bsd_chown(proc_port,
			interrupt,
			fname, strlen(fname) + 1,
			uid,
			gid);
}

int
e_utimes(proc_port, interrupt, fname, times)
	mach_port_t	proc_port;
	boolean_t	*interrupt;
	char		*fname;
	timeval_2_t	times;
{
	return bsd_utimes(proc_port,
			interrupt,
			fname, strlen(fname) + 1,
			times);
}

int
e_truncate(proc_port, interrupt, fname, length)
	mach_port_t	proc_port;
	boolean_t	*interrupt;
	char		*fname;
	int		length;
{
	return bsd_truncate(proc_port,
			interrupt,
			fname, strlen(fname) + 1,
			length);
}

int
e_rename(proc_port, interrupt, from_name, to_name)
	mach_port_t	proc_port;
	boolean_t	*interrupt;
	char		*from_name;
	char		*to_name;
{
	return bsd_rename(proc_port,
			interrupt,
			from_name, strlen(from_name) + 1,
			to_name, strlen(to_name) + 1);
}

int
e_mkdir(proc_port, interrupt, fname, dmode)
	mach_port_t	proc_port;
	boolean_t	*interrupt;
	char		*fname;
	int		dmode;
{
	return bsd_mkdir(proc_port,
			interrupt,
			fname, strlen(fname) + 1,
			dmode);
}

int
e_rmdir(proc_port, interrupt, fname)
	mach_port_t	proc_port;
	boolean_t	*interrupt;
	char		*fname;
{
	return bsd_rmdir(proc_port,
			interrupt,
			fname, strlen(fname) + 1);
}

int
e_xutimes(proc_port, interrupt, fname, times)
	mach_port_t	proc_port;
	boolean_t	*interrupt;
	char		*fname;
	timeval_3_t	times;
{
	return bsd_xutimes(proc_port,
			interrupt,
			fname, strlen(fname) + 1,
			times);
}

int
e_mount(proc_port, interrupt, fspec, freg, ronly)
	mach_port_t	proc_port;
	boolean_t	*interrupt;
	char		*fspec;
	char		*freg;
	int		ronly;
{
	return bsd_mount(proc_port,
			interrupt,
			fspec, strlen(fspec) + 1,
			freg, strlen(freg) + 1,
			ronly);
}

int
e_umount(proc_port, interrupt, fspec)
	mach_port_t	proc_port;
	boolean_t	*interrupt;
	char		*fspec;
{
	return bsd_umount(proc_port,
			interrupt,
			fspec, strlen(fspec) + 1);
}

int
e_setquota(proc_port, interrupt, fblk, fname)
	mach_port_t	proc_port;
	boolean_t	*interrupt;
	char		*fblk;
	char		*fname;
{
	return bsd_setquota(proc_port,
			interrupt,
			fblk, strlen(fblk) + 1,
			fname, strlen(fname) + 1);
}
