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
 * $Log:	bsd_server_side.c,v $
 * Revision 2.13  91/03/20  14:59:07  dbg
 * 	Fixed bsd_write_short, bsd_write_long, bsd_readwrite to be serial.
 * 	[91/02/02            rpd]
 * 
 * Revision 2.12  91/03/12  21:52:23  dbg
 * 	Fix reference to user_request_it.
 * 	[91/03/12            rwd]
 * 
 * Revision 2.11  90/10/25  15:07:06  rwd
 * 	Check out data size in pioctl to avoid overwriting reply message.
 * 	[90/10/10            rwd]
 * 
 * Revision 2.10  90/08/06  15:34:29  rwd
 * 	Change bsd_select to reflect change in interface.
 * 	[90/06/25            rwd]
 * 	Fix share_lock references.
 * 	[90/06/08            rwd]
 * 
 * Revision 2.9  90/06/19  23:15:45  rpd
 * 	Fixed bug in bsd_readwrite.
 * 	[90/06/06            rpd]
 * 
 * Revision 2.8  90/06/02  15:26:55  rpd
 * 	Removed bsd_signals_wakeup.
 * 	[90/05/11            rpd]
 * 	More updates for new IPC.
 * 	[90/05/03            rpd]
 * 	Converted to new IPC.
 * 	[90/03/26  20:08:33  rpd]
 * 
 * Revision 2.7  90/05/29  20:24:47  rwd
 * 	Remove some debugging printouts.
 * 	[90/05/12            rwd]
 * 	Added override parameter to user_request_it call.
 * 	[90/04/14            rwd]
 * 	Pass count argument for each path_name_t parameter.
 * 	[90/04/06            dbg]
 * 
 * 	Move bsd_execve glue routine here.
 * 	[90/04/06            dbg]
 * 
 * 	Added calls used by MACH_NBC && MAP_UAREA.
 * 	[90/03/22            rwd]
 * 
 * Revision 2.6  90/05/21  14:00:52  dbg
 * 	Fix arguments to adjtime.
 * 	[90/04/12            dbg]
 * 
 * 	Pass count argument for each path_name_t parameter.
 * 	[90/04/06            dbg]
 * 
 * 	Move bsd_execve glue routine here.
 * 	[90/04/06            dbg]
 * 
 * Revision 2.5  90/03/14  21:30:36  rwd
 * 	Added bsd_share_wakeup for shared locks.
 * 	[90/02/16            rwd]
 * 	Added bsd_emulator_error and bsd_readwrite.
 * 	[90/01/26            rwd]
 * 
 * Revision 2.4  89/11/29  15:30:16  af
 * 	Revision 2.2.2.1  89/11/16  15:29:42  af
 * 		Sigvec takes one more argument.
 * 	[89/11/26  11:37:49  af]
 * 
 * Revision 2.3  89/11/15  13:27:28  dbg
 * 	Add bsd_pioctl.
 * 	[89/11/07            dbg]
 * 	Add bsd_table_set and bsd_table_get.  Table call is split into
 * 	two interfaces because we can't use a 'dealloc' flag on an
 * 	in-out argument.
 * 	[89/10/25            dbg]
 * 
 * Revision 2.2  89/10/17  11:26:57  rwd
 * 	Added interrupt return parameter to all calls.
 * 	[89/09/21            dbg]
 * 
 */
/*
 * Glue
 */

#include <vice.h>
#include <map_uarea.h>
#include <mach_nbc.h>

#include <uxkern/syscall_subr.h>
#include <uxkern/import_mach.h>
#include <uxkern/bsd_types.h>
#include <sys/stat.h>
#include <sys/errno.h>
#if	VICE
#include <sys/viceioctl.h>
#endif	VICE

#include <uxkern/proc_to_task.h>
#include <uxkern/syscalltrace.h>

/*
 * in sys_generic
 */
int
bsd_write_short(proc_port, interrupt, fileno, data, data_count, amount_written)
	mach_port_t	proc_port;
	boolean_t	*interrupt;
	int		fileno;
	char *		data;
	unsigned int	data_count;
	int		*amount_written;	/* out */
{
	START_SERVER_SERIAL(SYS_write, 3)

	arg[0] = fileno;
	arg[1] = (int)data;
	arg[2] = (int)data_count;
	write();
	*amount_written = uth->uu_r.r_val1;

	END_SERVER_SERIAL
}

int
bsd_write_long(proc_port, interrupt, fileno, data, data_count, amount_written)
	mach_port_t	proc_port;
	boolean_t	*interrupt;
	int		fileno;
	char *		data;
	unsigned int	data_count;
	int		*amount_written;	/* out */
{
	START_SERVER_SERIAL(SYS_write, 3)

	arg[0] = fileno;
	arg[1] = (int)data;
	arg[2] = (int)data_count;
	write();
	*amount_written = uth->uu_r.r_val1;

	END_SERVER_SERIAL_DEALLOC(data, data_count)
}

int
bsd_select(proc_port, interrupt, nd, in_set, ou_set, ex_set,
	   in_valid, ou_valid, ex_valid, do_timeout, tv, rval)
	mach_port_t	proc_port;
	boolean_t	*interrupt;
	int		nd;
	fd_set		*in_set;	/* in/out */
	fd_set		*ou_set;	/* in/out */
	fd_set		*ex_set;	/* in/out */
	boolean_t	in_valid;
	boolean_t	ou_valid;
	boolean_t	ex_valid;
	boolean_t	do_timeout;
	timeval_t	tv;
	int		*rval;		/* out */
{
	START_SERVER_SERIAL(SYS_select, 5)

	arg[0] = nd;
	arg[1] = (in_valid) ? (int)in_set : 0;
	arg[2] = (ou_valid) ? (int)ou_set : 0;
	arg[3] = (ex_valid) ? (int)ex_set : 0;
	arg[4] = (do_timeout) ? (int)&tv : 0;
	select();
	*rval = u.u_r.r_val1;

	END_SERVER_SERIAL
}

/*
 * in uipc_syscalls
 */
int
bsd_send_short(proc_port, interrupt,
		fileno, flags, data, data_count, amount_written)
	mach_port_t	proc_port;
	boolean_t	*interrupt;
	int		fileno;
	int		flags;
	char		*data;
	unsigned int	data_count;
	int		*amount_written;	/* OUT */
{
	START_SERVER_SERIAL(SYS_send, 4)

	arg[0] = fileno;
	arg[1] = (int)data;
	arg[2] = (int)data_count;
	arg[3] = flags;
	send();
	*amount_written = u.u_r.r_val1;

	END_SERVER_SERIAL
}

int
bsd_send_long(proc_port, interrupt,
		fileno, flags, data, data_count, amount_written)
	mach_port_t	proc_port;
	boolean_t	*interrupt;
	int		fileno;
	int		flags;
	char		*data;
	unsigned int	data_count;
	int		*amount_written;	/* OUT */
{
	START_SERVER_SERIAL(SYS_send, 4)

	arg[0] = fileno;
	arg[1] = (int)data;
	arg[2] = (int)data_count;
	arg[3] = flags;
	send();
	*amount_written = u.u_r.r_val1;

	END_SERVER_SERIAL_DEALLOC(data, data_count)
}

int
bsd_sendto_short(proc_port, interrupt, fileno, flags,
		to, tolen, data, data_count, amount_written)
	mach_port_t	proc_port;
	boolean_t	*interrupt;
	int		fileno;
	int		flags;
	char		*to;
	int		tolen;
	char		*data;
	unsigned int	data_count;
	int		*amount_written;	/* OUT */
{
	START_SERVER_SERIAL(SYS_send, 6)

	arg[0] = fileno;
	arg[1] = (int)data;
	arg[2] = (int)data_count;
	arg[3] = flags;
	arg[4] = (int)to;
	arg[5] = tolen;
	sendto();
	*amount_written = u.u_r.r_val1;

	END_SERVER_SERIAL
}

int
bsd_sendto_long(proc_port, interrupt, fileno, flags,
		to, tolen, data, data_count, amount_written)
	mach_port_t	proc_port;
	boolean_t	*interrupt;
	int		fileno;
	int		flags;
	char		*to;
	int		tolen;
	char		*data;
	unsigned int	data_count;
	int		*amount_written;	/* OUT */
{
	START_SERVER_SERIAL(SYS_send, 6)

	arg[0] = fileno;
	arg[1] = (int)data;
	arg[2] = (int)data_count;
	arg[3] = flags;
	arg[4] = (int)to;
	arg[5] = tolen;
	sendto();
	*amount_written = u.u_r.r_val1;

	END_SERVER_SERIAL_DEALLOC(data, data_count)
}

int
bsd_recvfrom_short(proc_port, interrupt, fileno, flags, len,
		from, fromlen, data, data_count)
	mach_port_t	proc_port;
	boolean_t	*interrupt;
	int		fileno;
	int		flags;
	int		len;
	char		*from;		/* pointer to OUT array */
	int		*fromlen;	/* in/out */
	char		*data;		/* pointer to OUT array */
	unsigned int	*data_count;	/* out */
{
	START_SERVER_SERIAL(SYS_recvfrom, 6)

	arg[0] = fileno;
	arg[1] = (int)data;
	arg[2] = len;
	arg[3] = flags;
	arg[4] = (int)from;
	arg[5] = (int)fromlen;

	recvfrom();

	*data_count = u.u_r.r_val1;

	END_SERVER_SERIAL
}

int
bsd_recvfrom_long(proc_port, interrupt, fileno, flags, len,
		from, fromlen, data, data_count)
	mach_port_t	proc_port;
	boolean_t	*interrupt;
	int		fileno;
	int		flags;
	int		len;
	char		*from;		/* pointer to OUT array */
	int		*fromlen;	/* in/out */
	char		**data;		/* out */
	unsigned int	*data_count;	/* out */
{
	START_SERVER_SERIAL(SYS_recvfrom, 6)

	if (vm_allocate(mach_task_self(),
			(vm_offset_t *)data,
			len,
			TRUE)
	    != KERN_SUCCESS)
	{
	    uth->uu_error = ENOBUFS;
	}
	else {
	    arg[0] = fileno;
	    arg[1] = (int)*data;
	    arg[2] = len;
	    arg[3] = flags;
	    arg[4] = (int)from;
	    arg[5] = (int)fromlen;

	    recvfrom();

	    *data_count = u.u_r.r_val1;
	}

	/*
	 * Special end code to deallocate data if any error
	 */
	/* { for electric-c */
	}
	unix_release();
	uth->uu_error = end_server_op(uth->uu_error, interrupt);
	if (uth->uu_error) {
	    (void) vm_deallocate(mach_task_self(),
				(vm_offset_t) *data,
				len);
	}
	return (uth->uu_error);
}

/*
 * in ufs_syscalls
 */
int
bsd_chdir(proc_port, interrupt, fname, fname_count)
	mach_port_t	proc_port;
	boolean_t	*interrupt;
	char		*fname;
	unsigned int	fname_count;
{
	START_SERVER_SERIAL(SYS_chdir, 1)

	TRACE(("(%s)",fname));
	arg[0] = (int)fname;
	chdir();

	END_SERVER_SERIAL
}

int
bsd_chroot(proc_port, interrupt, fname, fname_count)
	mach_port_t	proc_port;
	boolean_t	*interrupt;
	char		*fname;
	unsigned int	fname_count;
{
	START_SERVER_SERIAL(SYS_chroot, 1)

	TRACE(("(%s)",fname));
	arg[0] = (int)fname;
	chroot();

	END_SERVER_SERIAL
}

int
bsd_open(proc_port, interrupt, fname, fname_count, mode, crtmode, fileno)
	mach_port_t	proc_port;
	boolean_t	*interrupt;
	char		*fname;
	unsigned int	fname_count;
	int		mode;
	int		crtmode;
	int		*fileno;	/* OUT */
{
	START_SERVER_SERIAL(SYS_open, 3)

	TRACE(("(%s)",fname));
	arg[0] = (int)fname;
	arg[1] = mode;
	arg[2] = crtmode;
	open();
	*fileno = uth->uu_r.r_val1;

	END_SERVER_SERIAL
}

int
bsd_creat(proc_port, interrupt, fname, fname_count, fmode, fileno)
	mach_port_t	proc_port;
	boolean_t	*interrupt;
	char		*fname;
	unsigned int	fname_count;
	int		fmode;
	int		*fileno;	/* OUT */
{
	START_SERVER_SERIAL(SYS_creat, 2)

	TRACE(("(%s)",fname));
	arg[0] = (int)fname;
	arg[1] = fmode;
	creat();
	*fileno = uth->uu_r.r_val1;

	END_SERVER_SERIAL
}

int
bsd_mknod(proc_port, interrupt, fname, fname_count, fmode, dev)
	mach_port_t	proc_port;
	boolean_t	*interrupt;
	char		*fname;
	unsigned int	fname_count;
	int		fmode;
	int		dev;
{
	START_SERVER_SERIAL(SYS_mknod, 3)

	TRACE(("(%s)",fname));
	arg[0] = (int)fname;
	arg[1] = fmode;
	arg[2] = dev;
	mknod();
	END_SERVER_SERIAL
}

int
bsd_link(proc_port, interrupt, target, target_count, linkname, linkname_count)
	mach_port_t	proc_port;
	boolean_t	*interrupt;
	char		*target;
	unsigned int	target_count;
	char		*linkname;
	unsigned int	linkname_count;
{
	START_SERVER_SERIAL(SYS_link, 2)

	TRACE(("(%s,%s)",target,linkname));
	arg[0] = (int)target;
	arg[1] = (int)linkname;
	link();

	END_SERVER_SERIAL
}

int
bsd_symlink(proc_port, interrupt, target, target_count, linkname, linkname_count)
	mach_port_t	proc_port;
	boolean_t	*interrupt;
	char		*target;
	unsigned int	target_count;
	char		*linkname;
	unsigned int	linkname_count;
{
	START_SERVER_SERIAL(SYS_symlink, 2)

	TRACE(("(%s,%s)",target,linkname));
	arg[0] = (int)target;
	arg[1] = (int)linkname;
	symlink();

	END_SERVER_SERIAL
}

int
bsd_unlink(proc_port, interrupt, fname, fname_count)
	mach_port_t	proc_port;
	boolean_t	*interrupt;
	char		*fname;
	unsigned int	fname_count;
{
	START_SERVER_SERIAL(SYS_unlink, 1)

	TRACE(("(%s)",fname));
	arg[0] = (int)fname;
	unlink();

	END_SERVER_SERIAL
}

int
bsd_access(proc_port, interrupt, fname, fname_count, fmode)
	mach_port_t	proc_port;
	boolean_t	*interrupt;
	char		*fname;
	unsigned int	fname_count;
	int		fmode;
{
	START_SERVER_SERIAL(SYS_access, 2)

	TRACE(("(%s,%o)",fname,fmode));
	arg[0] = (int)fname;
	arg[1] = fmode;
	saccess();

	END_SERVER_SERIAL
}

int
bsd_stat(proc_port, interrupt, follow, name, name_count, stat)
	mach_port_t	proc_port;
	boolean_t	*interrupt;
	boolean_t	follow;
	char		*name;
	unsigned int	name_count;
	register statb_t *stat;	/* out */
{
	struct stat	sb;

	START_SERVER_SERIAL(follow ? SYS_stat : SYS_lstat, 2);

	TRACE(("(%s)",name));
	arg[0] = (int)name;
	arg[1] = (int)&sb;

	stat1(follow ? FOLLOW : NOFOLLOW);

	/* copy out stat buffer... */
	stat->s_dev	= sb.st_dev;
	stat->s_ino	= sb.st_ino;
	stat->s_mode	= sb.st_mode;
	stat->s_nlink	= sb.st_nlink;
	stat->s_uid	= sb.st_uid;
	stat->s_gid	= sb.st_gid;
	stat->s_rdev	= sb.st_rdev;
	stat->s_size	= sb.st_size;
	stat->s_atime	= sb.st_atime;
	stat->s_mtime	= sb.st_mtime;
	stat->s_ctime	= sb.st_ctime;
	stat->s_blksize	= sb.st_blksize;
	stat->s_blocks	= sb.st_blocks;

	END_SERVER_SERIAL
}

int
bsd_readlink(proc_port, interrupt, name, name_count, count, buf, bufCount)
	mach_port_t	proc_port;
	boolean_t	*interrupt;
	char		*name;
	unsigned int	name_count;
	int		count;
	char		*buf;		/* pointer to OUT array */
	int		*bufCount;	/* out */
{
	START_SERVER_SERIAL(SYS_readlink, 3)

	TRACE(("(%s)",name));
	arg[0] = (int)name;
	arg[1] = (int)buf;
	arg[2] = count;
	readlink();
	*bufCount = uth->uu_r.r_val1;

	END_SERVER_SERIAL
}

int
bsd_chmod(proc_port, interrupt, fname, fname_count, fmode)
	mach_port_t	proc_port;
	boolean_t	*interrupt;
	char		*fname;
	unsigned int	fname_count;
	int		fmode;
{
	START_SERVER_SERIAL(SYS_chmod, 2)

	TRACE(("(%s,%o)",fname,fmode));
	arg[0] = (int)fname;
	arg[1] = fmode;
	chmod();

	END_SERVER_SERIAL
}

int
bsd_chown(proc_port, interrupt, fname, fname_count, uid, gid)
	mach_port_t	proc_port;
	boolean_t	*interrupt;
	char		*fname;
	unsigned int	fname_count;
	int		uid;
	int		gid;
{
	START_SERVER_SERIAL(SYS_chown, 3)

	TRACE(("(%s,%d,%d)",fname,uid,gid));
	arg[0] = (int)fname;
	arg[1] = uid;
	arg[2] = gid;
	chown();

	END_SERVER_SERIAL
}

int
bsd_utimes(proc_port, interrupt, fname, fname_count, times)
	mach_port_t	proc_port;
	boolean_t	*interrupt;
	char		*fname;
	unsigned int	fname_count;
	struct timeval	*times;
{
	START_SERVER_SERIAL(SYS_utimes, 2)

	TRACE(("(%s)",fname));
	arg[0] = (int)fname;
	arg[1] = (int)times;
	utimes();

	END_SERVER_SERIAL
}

int
bsd_truncate(proc_port, interrupt, fname, fname_count, length)
	mach_port_t	proc_port;
	boolean_t	*interrupt;
	char		*fname;
	unsigned int	fname_count;
	int		length;
{
	START_SERVER_SERIAL(SYS_truncate, 2)

	TRACE(("(%s)",fname));
	arg[0] = (int)fname;
	arg[1] = length;
	truncate();

	END_SERVER_SERIAL
}

int
bsd_rename(proc_port, interrupt, from, from_count, to, to_count)
	mach_port_t	proc_port;
	boolean_t	*interrupt;
	char		*from;
	unsigned int	from_count;
	char		*to;
	unsigned int	to_count;
{
	START_SERVER_SERIAL(SYS_rename, 2)

	TRACE(("(%s,%s)",from,to));
	arg[0] = (int)from;
	arg[1] = (int)to;
	rename();

	END_SERVER_SERIAL
}

int
bsd_mkdir(proc_port, interrupt, name, name_count, dmode)
	mach_port_t	proc_port;
	boolean_t	*interrupt;
	char		*name;
	unsigned int	name_count;
	int		dmode;
{
	START_SERVER_SERIAL(SYS_mkdir, 2)

	TRACE(("(%s,%o)",name,dmode));
	arg[0] = (int)name;
	arg[1] = dmode;
	mkdir();

	END_SERVER_SERIAL
}

int
bsd_rmdir(proc_port, interrupt, name, name_count)
	mach_port_t	proc_port;
	boolean_t	*interrupt;
	char		*name;
	unsigned int	name_count;
{
	START_SERVER_SERIAL(SYS_rmdir, 1)

	TRACE(("(%s)",name));
	arg[0] = (int)name;
	rmdir();

	END_SERVER_SERIAL
}

/*
 * In cmu_syscalls
 */
int
bsd_xutimes(proc_port, interrupt, fname, fname_count, times)
	mach_port_t	proc_port;
	boolean_t	*interrupt;
	char		*fname;
	unsigned int	fname_count;
	struct timeval	*times;
{
	START_SERVER_SERIAL(SYS_xutimes, 2)

	TRACE(("(%s)",fname));
	arg[0] = (int)fname;
	arg[1] = (int)times;
	xutimes();

	END_SERVER_SERIAL
}

/*
 * in ufs_mount
 */
int
bsd_mount(proc_port, interrupt, fspec, fspec_count, freg, freg_count, ronly)
	mach_port_t	proc_port;
	boolean_t	*interrupt;
	char		*fspec;
	unsigned int	fspec_count;
	char		*freg;
	unsigned int	freg_count;
	int		ronly;
{
	START_SERVER_SERIAL(SYS_mount, 3)

	TRACE(("(%s,%s)",fspec,freg));
	arg[0] = (int)fspec;
	arg[1] = (int)freg;
	arg[2] = ronly;
	smount();

	END_SERVER_SERIAL
}

int
bsd_umount(proc_port, interrupt, fspec, fspec_count)
	mach_port_t	proc_port;
	boolean_t	*interrupt;
	char		*fspec;
	unsigned int	fspec_count;
{
	START_SERVER_SERIAL(SYS_umount, 1)

	TRACE(("(%s)",fspec));
	arg[0] = (int)fspec;
	umount();

	END_SERVER_SERIAL
}

/*
 * in kern_acct
 */

int
bsd_acct(proc_port, interrupt, acct_on, fname, fname_count)
	mach_port_t	proc_port;
	boolean_t	*interrupt;
	boolean_t	acct_on;
	char		*fname;
	unsigned int	fname_count;
{
	START_SERVER_SERIAL(SYS_acct, 1)

	arg[0] = (acct_on) ? (int)fname : 0;
	sysacct();

	END_SERVER_SERIAL
}

/*
 * in quota_sys
 */
int
bsd_setquota(proc_port, interrupt, fblk, fblk_count, fname, fname_count)
	mach_port_t	proc_port;
	boolean_t	*interrupt;
	char		*fblk;
	unsigned int	fblk_count;
	char		*fname;
	unsigned int	fname_count;
{
	START_SERVER_SERIAL(SYS_setquota, 2)

	arg[0] = (int)fblk;
	arg[1] = (int)fname;
	setquota();

	END_SERVER_SERIAL
}

/*
 * More glue
 */
int
bsd_setgroups(proc_port, interrupt, gidsetsize, gidset)
	mach_port_t	proc_port;
	boolean_t	*interrupt;
	unsigned int	gidsetsize;
	int		*gidset;
{
	START_SERVER_SERIAL(SYS_setgroups, 2)

	arg[0] = gidsetsize;
	arg[1] = (int)gidset;

	setgroups();

	END_SERVER_SERIAL
}

int
bsd_setrlimit(proc_port, interrupt, which, lim)
	mach_port_t	proc_port;
	boolean_t	*interrupt;
	int		which;
	struct rlimit	*lim;
{
	START_SERVER_SERIAL(SYS_setrlimit, 2)

	arg[0] = which;
	arg[1] = (int)lim;

	setrlimit();

	END_SERVER_SERIAL
}

int
bsd_sigvec(proc_port, interrupt, signo, have_nsv, nsv, osv, tramp)
	mach_port_t	proc_port;
	boolean_t	*interrupt;	/* OUT */
	int		signo;
	boolean_t	have_nsv;
	struct sigvec	nsv;
	struct sigvec	*osv;	/* OUT */
	int		tramp;
{
	START_SERVER_SERIAL(SYS_sigvec, 4)

	arg[0] = signo;
	arg[1] = (have_nsv) ? (int)&nsv : 0;
	arg[2] = (int)osv;
	arg[3] = tramp;

	sigvec();

	END_SERVER_SERIAL
}

int
bsd_sigstack(proc_port, interrupt, have_nss, nss, oss)
	mach_port_t	proc_port;
	boolean_t	*interrupt;
	boolean_t	have_nss;
	struct sigstack	nss;
	struct sigstack	*oss;		/* OUT */
{
	START_SERVER_SERIAL(SYS_sigstack, 2)

	arg[0] = (have_nss) ? (int)&nss : 0;
	arg[1] = (int)oss;

	sigstack();

	END_SERVER_SERIAL
}

int
bsd_settimeofday(proc_port, interrupt, have_tv, tv, have_tz, tz)
	mach_port_t	proc_port;
	boolean_t	*interrupt;
	boolean_t	have_tv;
	struct timeval	tv;
	boolean_t	have_tz;
	struct timezone	tz;
{
	START_SERVER_SERIAL(SYS_settimeofday, 2)

	arg[0] = (have_tv) ? (int)&tv : 0;
	arg[1] = (have_tz) ? (int)&tz : 0;

	settimeofday();

	END_SERVER_SERIAL
}

int
bsd_adjtime(proc_port, interrupt, delta, olddelta)
	mach_port_t	proc_port;
	boolean_t	*interrupt;
	struct timeval	delta;
	struct timeval	*olddelta;
{
	START_SERVER_SERIAL(SYS_adjtime, 2)

	arg[0] = (int)&delta;
	arg[1] = (int)olddelta;

	adjtime();

	END_SERVER_SERIAL
}

int
bsd_setitimer(proc_port, interrupt, which, have_itv, itv, oitv)
	mach_port_t	proc_port;
	boolean_t	*interrupt;
	int		which;
	boolean_t	have_itv;
	struct itimerval itv;
	struct itimerval *oitv;		/* OUT */
{
	START_SERVER_SERIAL(SYS_setitimer, 3)

	arg[0] = which;
	arg[1] = (have_itv) ? (int)&itv : 0;
	arg[2] = (int)oitv;

	setitimer();

	END_SERVER_SERIAL
}

int
bsd_sethostname(proc_port, interrupt, hostname, len)
	mach_port_t	proc_port;
	boolean_t	*interrupt;
	char		*hostname;
	int		len;
{
	START_SERVER_SERIAL(SYS_sethostname, 2)

	arg[0] = (int)hostname;
	arg[1] = len;

	sethostname();

	END_SERVER_SERIAL
}

int
bsd_bind(proc_port, interrupt, s, name, namelen)
	mach_port_t	proc_port;
	boolean_t	*interrupt;
	int		s;
	char		*name;
	int		namelen;
{
	START_SERVER_SERIAL(SYS_bind, 3)

	arg[0] = s;
	arg[1] = (int) name;
	arg[2] = namelen;

	bind();

	END_SERVER_SERIAL
}

int
bsd_accept(proc_port, interrupt, s, name, namelen, new_s)
	mach_port_t	proc_port;
	boolean_t	*interrupt;
	int		s;
	char		*name;		/* OUT */
	int		*namelen;	/* OUT */
	int		*new_s;		/* OUT */
{
	int	maxnamelen;

	START_SERVER_SERIAL(SYS_accept, 3)

	maxnamelen = sizeof(sockarg_t);

	arg[0] = s;
	arg[1] = (int) name;
	arg[2] = (int) &maxnamelen;

	accept();

	*namelen = maxnamelen;
	*new_s = u.u_r.r_val1;

	END_SERVER_SERIAL
}

int
bsd_connect(proc_port, interrupt, s, name, namelen)
	mach_port_t	proc_port;
	boolean_t	*interrupt;
	int		s;
	char		*name;
	int		namelen;
{
	START_SERVER_SERIAL(SYS_connect, 3)

	arg[0] = s;
	arg[1] = (int) name;
	arg[2] = namelen;

	connect();

	END_SERVER_SERIAL
}

int
bsd_setsockopt(proc_port, interrupt, s, level, name, val, valsize)
	mach_port_t	proc_port;
	boolean_t	*interrupt;
	int		s;
	int		level;
	int		name;
	char		*val;
	int		valsize;
{
	START_SERVER_SERIAL(SYS_setsockopt, 5)

	arg[0] = s;
	arg[1] = level;
	arg[2] = name;
	arg[3] = (valsize > 0) ? (int)val : 0;
	arg[4] = valsize;

	setsockopt();

	END_SERVER_SERIAL
}

int
bsd_getsockopt(proc_port, interrupt, s, level, name, val, avalsize)
	mach_port_t	proc_port;
	boolean_t	*interrupt;
	int		s;
	int		level;
	int		name;
	char		*val;		/* OUT */
	int		*avalsize;	/* IN-OUT */
{
	int		maxvalsize;

	START_SERVER_SERIAL(SYS_getsockopt, 5)

	maxvalsize = sizeof(sockarg_t);

	arg[0] = s;
	arg[1] = level;
	arg[2] = name;
	arg[3] = (int)val;
	arg[4] = (int)&maxvalsize;

	getsockopt();

	*avalsize = maxvalsize;

	END_SERVER_SERIAL
}

int
bsd_getsockname(proc_port, interrupt, fdes, asa, alen)
	mach_port_t	proc_port;
	boolean_t	*interrupt;
	int		fdes;
	char		*asa;		/* OUT */
	int		*alen;		/* OUT */
{
	int	maxlen;

	START_SERVER_SERIAL(SYS_getsockname, 3)

	maxlen = sizeof(sockarg_t);

	arg[0] = fdes;
	arg[1] = (int)asa;
	arg[2] = (int)&maxlen;

	getsockname();

	*alen = maxlen;

	END_SERVER_SERIAL
}

int
bsd_getpeername(proc_port, interrupt, fdes, asa, alen)
	mach_port_t	proc_port;
	boolean_t	*interrupt;
	int		fdes;
	char		*asa;		/* OUT */
	int		*alen;		/* OUT */
{
	int	maxlen;

	START_SERVER_SERIAL(SYS_getpeername, 3)

	maxlen = sizeof(sockarg_t);

	arg[0] = fdes;
	arg[1] = (int)asa;
	arg[2] = (int)&maxlen;

	getpeername();

	*alen = maxlen;

	END_SERVER_SERIAL
}

int
bsd_table_set(proc_port, interrupt, id, index, lel, nel,
		addr, count, nel_done)
	mach_port_t	proc_port;
	boolean_t	*interrupt;	/* out */
	int		id;
	int		index;
	int		lel;
	int		nel;
	char		*addr;
	unsigned int	count;
	int		*nel_done;	/* out */
{
	START_SERVER_SERIAL(SYS_table, 5)

	arg[0] = id;
	arg[1] = index;
	arg[2] = (int)addr;
	arg[3] = nel;
	arg[4] = lel;

	table();

	*nel_done = u.u_r.r_val1;

	END_SERVER_SERIAL
}

int
bsd_table_get(proc_port, interrupt, id, index, lel, nel,
		addr, count, nel_done)
	mach_port_t	proc_port;
	boolean_t	*interrupt;	/* out */
	int		id;
	int		index;
	int		lel;
	int		nel;
	char		**addr;		/* out */
	unsigned int	*count;		/* out */
	int		*nel_done;	/* out */
{
	START_SERVER_SERIAL(SYS_table, 5)

	*count = nel * lel;
	if (vm_allocate(mach_task_self(),
			(vm_offset_t *)addr,
			*count,
			TRUE)
	    != KERN_SUCCESS)
	{
	    uth->uu_error = ENOBUFS;
	}
	else {
	    arg[0] = id;
	    arg[1] = index;
	    arg[2] = (int)*addr;
	    arg[3] = nel;
	    arg[4] = lel;

	    table();

	    *nel_done = u.u_r.r_val1;
	}

	/*
	 * Special end code to deallocate data if any error
	 */
	/* { for electric-c */
	}
	unix_release();
	uth->uu_error = end_server_op(uth->uu_error, interrupt);
	if (uth->uu_error) {
	    (void) vm_deallocate(mach_task_self(),
				 (vm_offset_t) *addr,
				 *count);
	    *count = 0;
	}

	return (uth->uu_error);
}

int
bsd_pioctl(proc_port, interrupt, path, path_count, com, follow, in_data, in_data_count,
		out_data_wanted, out_data, out_data_count)
	mach_port_t	proc_port;
	boolean_t	*interrupt;
	char *		path;
	unsigned int	path_count;
	int		com;
	boolean_t	follow;
	char *		in_data;
	unsigned int	in_data_count;
	int		out_data_wanted;
	char *		out_data;	 /* pointer to OUT array */
	unsigned int	*out_data_count; /* out */
{
#if	VICE
	struct ViceIoctl	param;

	START_SERVER_SERIAL(SYS_pioctl, 4)

	TRACE(("(%s,%x)",path,com));
	param.in	= in_data;
	param.in_size	= in_data_count;
	param.out	= out_data;
	if (out_data_wanted > *out_data_count)
	    out_data_wanted = *out_data_count;
	param.out_size	= out_data_wanted;

	arg[0] = (int) path;
	arg[1] = com;
	arg[2] = (int) &param;
	arg[3] = follow;

	pioctl();

	*out_data_count = out_data_wanted;

	END_SERVER_SERIAL
#else	VICE
	return (EINVAL);
#endif	VICE

}

int
bsd_emulator_error(proc_port, error_message, size)
	mach_port_t	proc_port;
	char *		error_message;
	int		size;
{
	printf("emulator [%x] %s\n",proc_port, error_message);
	return KERN_SUCCESS;
}

int
bsd_readwrite(proc_port, interrupt, which, fileno, size, amount)
	mach_port_t	proc_port;
	boolean_t	*interrupt;
	boolean_t	which;
	int		fileno;
	int		size;
	int		*amount;
{
#if	MAP_UAREA
	if (which != 0 && which != 1)
		return (EINVAL);
	else {
		START_SERVER_SERIAL(which?SYS_write:SYS_read, 3)

		arg[0] = fileno;
		arg[1] = (int)uth->uu_procp->p_readwrite;
		arg[2] = (int)size;
		if (which)
			write();
		else
			read();
		*amount = uth->uu_r.r_val1;

		END_SERVER_SERIAL
	}
#else	MAP_UAREA
	return (EINVAL);
#endif	MAP_UAREA
}

int
bsd_share_wakeup(proc_port, offset)
	mach_port_t	proc_port;
	int		offset;
{
#if	MAP_UAREA
	register struct proc	*p;

	if ((p = port_to_proc_lookup(proc_port)) == (struct proc *)0)
		return (ESRCH);
#ifdef	SYSCALLTRACE
	if (syscalltrace &&
		(syscalltrace == p->p_pid || syscalltrace < 0)) {
	    printf("[%d]bsd_share_wakeup(%x, %x)", p->p_pid, (int)p, offset);
	}
#endif
	mutex_lock(&master_mutex);
	wakeup((int)(p->p_shared_rw) + offset);
	mutex_unlock(&master_mutex);
	return (0);
#else MAP_UAREA
	return (EINVAL);
#endif MAP_UAREA
}

int bsd_maprw_request_it(proc_port, interrupt, fileno)
	mach_port_t	proc_port;
	boolean_t	*interrupt;
	register int	fileno;
{
#if	MACH_NBC && MAP_UAREA
	register struct inode *ip;

	START_SERVER_PARALLEL(1005, 1)
	ip = (struct inode *)u.u_ofile[fileno]->f_data;
	user_request_it(ip, fileno, 0, u.u_ofile[fileno]);
	END_SERVER_PARALLEL
#else	MACH_NBC && MAP_UAREA
	return (EINVAL);
#endif	MACH_NBC && MAP_UAREA
}

int bsd_maprw_release_it(proc_port, interrupt, fileno)
	mach_port_t	proc_port;
	boolean_t	*interrupt;
	register int	fileno;
{
#if	MACH_NBC && MAP_UAREA
	register struct inode *ip;
	START_SERVER_PARALLEL(1006, 1)
	ip = (struct inode *)u.u_ofile[fileno]->f_data;
	user_release_it(ip);
	END_SERVER_PARALLEL
#else	MACH_NBC && MAP_UAREA
	return (EINVAL);
#endif	MACH_NBC && MAP_UAREA
}

int bsd_maprw_remap(proc_port, interrupt, fileno, offset, size)
	mach_port_t	proc_port;
	boolean_t	*interrupt;
	register int	fileno;
	int	offset;
	int	size;
{
#if	MACH_NBC && MAP_UAREA
	register struct inode *ip;
	START_SERVER_PARALLEL(1007, 3)
	ip = (struct inode *)u.u_ofile[fileno]->f_data;
	user_remap_inode(ip, offset, size);
	END_SERVER_PARALLEL
#else	MACH_NBC && MAP_UAREA
	return (EINVAL);
#endif	MACH_NBC && MAP_UAREA
}

int
bsd_execve(proc_port, interrupt,
	   fname, fname_count,
	   cfname,
	   cfarg,
	   entry,
	   entry_count)
	mach_port_t	proc_port;
	boolean_t	*interrupt;
	char		*fname;
	unsigned int	fname_count;
	char		*cfname;	/* OUT */
	char		*cfarg;		/* OUT */
	int		*entry;		/* pointer to OUT array */
	unsigned int	*entry_count;	/* OUT */
{
	START_SERVER_SERIAL(SYS_execve, 1)

	TRACE(("(%s)",fname));

	uth->uu_error = exec_file(fname, cfname, cfarg,
				  entry, entry_count);

	END_SERVER_SERIAL
}
