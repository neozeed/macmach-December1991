/* 
 * Mach Operating System
 * Copyright (c) 1987 Carnegie-Mellon University
 * All rights reserved.  The CMU software License Agreement specifies
 * the terms and conditions for use and redistribution.
 */
/*
 * HISTORY
 * $Log:	mach_core.c,v $
 * Revision 2.2  90/06/02  15:21:35  rpd
 * 	No longer necessary to undefine u_arg.
 * 	[90/05/13            rpd]
 * 	Converted to new IPC.
 * 	[90/03/26  19:35:31  rpd]
 * 
 * Revision 2.1  89/08/04  14:06:31  rwd
 * Created.
 * 
 * 20-Jul-88  David Golub (dbg) at Carnegie-Mellon University
 *	Moved machine-dependent routines to machine/bsd_machdep.c
 *
 * 27-Jun-88  David Golub (dbg) at Carnegie-Mellon University
 *	Created to replace kern_sig/core().  Moved fake_u here from old
 *	file.
 *
 */

#include <cmucs.h>
#include <vice.h>

/*
 *	bsd/mach_core.c
 *
 *	Routines to fake the old BSD U-area structure.  Includes
 *	core dump.
 */

#include <sys/types.h>
#include <sys/errno.h>
#include <sys/proc.h>
#include <sys/user.h>
#include <sys/inode.h>
#include <sys/file.h>
#include <sys/acct.h>
#include <sys/uio.h>

#include <sys/parallel.h>

#include <uxkern/import_mach.h>

/*
 * Create a core image for a process.  It does not have
 * to be the current process.
 *
 * Must be called from a U*X process, so that there is
 * a u-area to make point to the proc structure.
 */

int core_dump(p)
	register struct proc *p;
{
	struct proc  *save_proc;
	register struct inode *ip = (struct inode *)0;
	register struct nameidata *ndp;
#if	VICE
	struct file *fp = (struct file *)0;
#endif	VICE

	/*
	 * Pretend we are the victim for the duration of the call.
	 */
	save_proc  = u.u_procp;
	u.u_procp = p;

#if	CMUCS
	if (p->p_flag & SXONLY) {
	    u.u_error = EACCES;
	    goto out;
	}
	p->p_utask.uu_uid = p->p_utask.uu_ruid;
	p->p_utask.uu_gid = p->p_utask.uu_rgid;
	p->p_uid = p->p_utask.uu_uid;
#else	CMUCS
	if (p->p_utask.uu_uid != p->p_utask.uu_ruid ||
	    p->p_utask.uu_gid != p->p_utask.uu_rgid) {
	    u.u_error = EACCES;
	    goto out;
	}
#endif	CMUCS
	if (ctob(UPAGES + p->p_utask.uu_dsize + p->p_utask.uu_ssize)
	    >= p->p_utask.uu_rlimit[RLIMIT_CORE].rlim_cur) {
	    u.u_error = EFBIG;
	    goto out;
	}

#if	VICE
	u.u_rmt_code = -1;
	u.u_rmt_requested = 1;
#endif	VICE
	u.u_error = 0;
	ndp = &u.u_nd;
	ndp->ni_nameiop = CREATE | FOLLOW;
	ndp->ni_segflg = UIO_SYSSPACE;
	ndp->ni_dirp = "core";

	ip = namei(ndp);
#if	VICE
	u.u_rmt_code = 0;
#endif	VICE
	if (ip == (struct inode *)0) {
#if	VICE
	    if (u.u_error != EVICEOP) {
#endif	VICE
		if (u.u_error)
		    goto out;
		ip = maknode(0644, ndp);
		if (ip == (struct inode *)0)
		    goto out;
#if	VICE
	    }
	    else {
		int	fd = u.u_r.r_val1;

		u.u_error = 0;
		if ((unsigned)fd >= NOFILE ||
		    (fp = u.u_ofile[fd]) == (struct file *)0) {
		    u.u_error = EMFILE;
		    goto out;
		}
		ip = (struct inode *)fp->f_data;
		ip->i_count++;
		u.u_ofile[fd] = (struct file *)0;
	    }
#endif	VICE
	}
	if (access(ip, IWRITE) ||
	    (ip->i_mode & IFMT) != IFREG ||
	    ip->i_nlink != 1) {
	    u.u_error = EACCES;
	    goto out;
	}
	itrunc(ip, (u_long)0);

	p->p_utask.uu_acflag |= ACORE;

	/*
	 * Build the core file.
	 */
	u.u_error = machine_core(p, ip);

    out:
	if (ip)
	    iput(ip);
#if	VICE
	if (fp)
	    closef(fp);
#endif	VICE
	u.u_procp = save_proc;

	return (u.u_error == 0);
}

int core_file_write(ip, offsetp, task, src_addr, src_size)
	struct inode	*ip;
	off_t		*offsetp;
	task_t		task;
	register vm_offset_t	src_addr;
	register vm_size_t	src_size;
{
	int	error;
	vm_size_t	max_size, size, copy_size;
	vm_offset_t	kern_addr, start_addr, end_addr, offset;
	vm_offset_t	copy_addr;

	error = 0;

	max_size = 65536;				/*XXX*/
	max_size = round_page(max_size);

	(void) vm_allocate(mach_task_self(), &copy_addr, max_size, TRUE);

	while (src_size != 0) {
	    start_addr = trunc_page(src_addr);
	    end_addr = round_page(src_addr + src_size);
	    size = end_addr - start_addr;
	    if (size > max_size)
		size = max_size;

	    if (vm_read(task, start_addr, size,
			&kern_addr, &size)
		!= KERN_SUCCESS) {
		error = EFAULT;
		break;
	    }

	    offset = (vm_offset_t)src_addr - start_addr;
	    copy_size = size - offset;
	    if (copy_size > src_size)
		copy_size = src_size;

	    /*
	     * To avoid deadlocking against the inode pager
	     * (which wants the master lock), we must release
	     * the master lock, copy the user data to a private
	     * area, then re-acquire the master lock to write
	     * the inode.
	     */
	    master_unlock();
	    bcopy((caddr_t)(kern_addr + offset),
		  (caddr_t)copy_addr,
		  copy_size);
	    master_lock();

	    error = rdwri(UIO_WRITE, ip,
			copy_addr,
			copy_size,
			*offsetp, UIO_SYSSPACE, (int *)0);

	    (void) vm_deallocate(mach_task_self(), kern_addr, size);

	    if (error)
		break;

	    *offsetp += copy_size;
	    src_addr += copy_size;
	    src_size -= copy_size;
	}

	(void) vm_deallocate(mach_task_self(), copy_addr, max_size);

	return(error);
}

/*
 *	fake_u:
 *
 *	fake a u-area structure for the specified thread.  Only "interesting"
 *	fields are filled in.
 *
 *	Must be last in the file, since it undefines many u-area access
 *	macros.
 */

fake_u(up, p, thread)
	register struct user	*up;
	struct proc		*p;
	register thread_t	thread;
{
	time_value_t	sys_time, user_time;
	register int	s;

#ifndef	mac2 /* mac2 does not have pcb structure */
	bzero((caddr_t)&up->u_pcb, sizeof(struct pcb));
#endif
	/* for machines with floating point, the fp registers should go here */
#undef	u_comm
	bcopy(p->p_utask.uu_comm, up->u_comm, sizeof(up->u_comm));
	bzero((caddr_t)up->u_arg, sizeof(up->u_arg));
	up->u_arg[0] = p->p_stopsig;	/* the only one used */
#undef	u_uid
	up->u_uid = p->p_utask.uu_uid;
#undef	u_gid
	up->u_gid = p->p_utask.uu_gid;
#undef	u_ruid
	up->u_ruid = p->p_utask.uu_ruid;
#undef	u_rgid
	up->u_rgid = p->p_utask.uu_rgid;
#undef	u_tsize
	up->u_tsize = p->p_utask.uu_tsize;
#undef	u_dsize
	up->u_dsize = p->p_utask.uu_dsize;
#undef	u_ssize
	up->u_ssize = p->p_utask.uu_ssize;
#undef	u_signal
	bcopy((caddr_t)p->p_utask.uu_signal,
	      (caddr_t)up->u_signal,
	      sizeof(up->u_signal));
#undef	u_code
	up->u_code = p->p_stopcode;
#undef	u_ru
	up->u_ru = p->p_utask.uu_ru;
	/*
	 *	Times aren't in uarea any more.
	 */
	thread_read_times(thread, &user_time, &sys_time);
	up->u_ru.ru_stime.tv_sec = sys_time.seconds;
	up->u_ru.ru_stime.tv_usec = sys_time.microseconds;
	up->u_ru.ru_utime.tv_sec = user_time.seconds;
	up->u_ru.ru_utime.tv_usec = user_time.microseconds;
#undef	u_cru
	up->u_cru = p->p_utask.uu_cru;
}
