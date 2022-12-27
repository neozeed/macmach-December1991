/* 
 * Mach Operating System
 * Copyright (c) 1987 Carnegie-Mellon University
 * All rights reserved.  The CMU software License Agreement specifies
 * the terms and conditions for use and redistribution.
 */
/*
 * HISTORY
 * $Log:	kern_resource.c,v $
 * Revision 2.3  90/06/02  15:21:25  rpd
 * 	Updated set_thread_priority call for the new priority range.
 * 	[90/04/23            rpd]
 * 	Converted to new IPC.
 * 	Deallocate thread ports from task_threads.
 * 	[90/03/26  19:34:44  rpd]
 * 
 * Revision 2.2  89/10/17  11:24:57  rwd
 * 	Add interrupt return parameter to getrusage.
 * 	[89/09/21            dbg]
 * 
 * Revision 2.1  89/08/04  14:06:25  rwd
 * Created.
 * 
 * 10-Apr-89  David Golub (dbg) at Carnegie-Mellon University
 *	Convert getrusage to MiG interface.
 *
 *  2-Jan-89  David Golub (dbg) at Carnegie-Mellon University
 *	Out-of-kernel version.
 *
 * 16-Jun-88  Michael Young (mwyoung) at Carnegie-Mellon University
 *	Actually grow or shrink stacks when requested in setrlimit().
 *
 *  4-May-88  David Black (dlb) at Carnegie-Mellon University
 *	MACH_TIME_NEW is now standard.
 *
 * 29-Mar-88  Michael Young (mwyoung) at Carnegie-Mellon University
 *	MACH: Removed use of "sys/vm.h".
 *
 *  2-Mar-88  David Black (dlb) at Carnegie-Mellon University
 *	Use thread_read_times to get times for self.  This replaces
 *	and generalized the MACH_TIME_NEW code.
 *
 * 21-Nov-87  Avadis Tevanian (avie) at Carnegie-Mellon University
 *	Reduced conditionals, purged history.
 *
 */

#include "mach.h"
#include <mach_no_kernel.h>

/*
 * Copyright (c) 1982, 1986 Regents of the University of California.
 * All rights reserved.  The Berkeley software License Agreement
 * specifies the terms and conditions for redistribution.
 *
 *	@(#)kern_resource.c	7.1 (Berkeley) 6/5/86
 */

#include "sys/param.h"
#include "sys/systm.h"
#include "sys/dir.h"
#include "sys/user.h"
#include "sys/inode.h"
#include "sys/proc.h"
#if	MACH
#include "sys/fs.h"
#include "sys/uio.h"
#else	MACH
#include "sys/seg.h"
#include "sys/fs.h"
#include "sys/uio.h"
#include "sys/vm.h"
#endif	MACH
#include "sys/kernel.h"

#if	MACH
#if	MACH_NO_KERNEL
#include <sys/syscall.h>
#include <uxkern/import_mach.h>
#else	MACH_NO_KERNEL
#include "sys/thread.h"
#include "sys/time_value.h"
#include "kern/mach.h"
#endif	MACH_NO_KERNEL
#endif	MACH

/*
 * Resource controls and accounting.
 */

getpriority()
{
	register struct a {
		int	which;
		int	who;
	} *uap = (struct a *)u.u_ap;
	register struct proc *p;
	int low = PRIO_MAX + 1;

	switch (uap->which) {

	case PRIO_PROCESS:
		if (uap->who == 0)
			p = u.u_procp;
		else
			p = pfind(uap->who);
		if (p == 0)
			break;
		low = p->p_nice;
		break;

	case PRIO_PGRP:
		if (uap->who == 0)
			uap->who = u.u_procp->p_pgrp;
		for (p = allproc; p != NULL; p = p->p_nxt) {
			if (p->p_pgrp == uap->who &&
			    p->p_nice < low)
				low = p->p_nice;
		}
		break;

	case PRIO_USER:
		if (uap->who == 0)
			uap->who = u.u_uid;
		for (p = allproc; p != NULL; p = p->p_nxt) {
			if (p->p_uid == uap->who &&
			    p->p_nice < low)
				low = p->p_nice;
		}
		break;

	default:
		u.u_error = EINVAL;
		return;
	}
	if (low == PRIO_MAX + 1) {
		u.u_error = ESRCH;
		return;
	}
	u.u_r.r_val1 = low;
}

setpriority()
{
	register struct a {
		int	which;
		int	who;
		int	prio;
	} *uap = (struct a *)u.u_ap;
	register struct proc *p;
	int found = 0;

	switch (uap->which) {

	case PRIO_PROCESS:
		if (uap->who == 0)
			p = u.u_procp;
		else
			p = pfind(uap->who);
		if (p == 0)
			break;
		donice(p, uap->prio);
		found++;
		break;

	case PRIO_PGRP:
		if (uap->who == 0)
			uap->who = u.u_procp->p_pgrp;
		for (p = allproc; p != NULL; p = p->p_nxt)
			if (p->p_pgrp == uap->who) {
				donice(p, uap->prio);
				found++;
			}
		break;

	case PRIO_USER:
		if (uap->who == 0)
			uap->who = u.u_uid;
		for (p = allproc; p != NULL; p = p->p_nxt)
			if (p->p_uid == uap->who) {
				donice(p, uap->prio);
				found++;
			}
		break;

	default:
		u.u_error = EINVAL;
		return;
	}
	if (found == 0)
		u.u_error = ESRCH;
}

donice(p, n)
	register struct proc *p;
	register int n;
{
#if	MACH
	register thread_t th;
	register int	s;
#endif	MACH

	if (u.u_uid && u.u_ruid &&
	    u.u_uid != p->p_uid && u.u_ruid != p->p_uid) {
		u.u_error = EPERM;
		return;
	}
	if (n > PRIO_MAX)
		n = PRIO_MAX;
	if (n < PRIO_MIN)
		n = PRIO_MIN;
	if (n < p->p_nice && !suser()) {
		u.u_error = EACCES;
		return;
	}
	p->p_nice = n;
#if	MACH_NO_KERNEL
	{
	    thread_array_t	thread_list;
	    unsigned int	thread_count;
	    register int	i, newpri;

	    newpri = 12 + n/2;

	    (void) task_threads(p->p_task, &thread_list, &thread_count);
	    for (i = 0; i < thread_count; i++)
		if (MACH_PORT_VALID(thread_list[i])) {
		    set_thread_priority(thread_list[i], newpri);
		    (void) mach_port_deallocate(mach_task_self(),
						thread_list[i]);
		}
	    (void) vm_deallocate(mach_task_self(),
				 (vm_offset_t)thread_list,
				 (vm_size_t)thread_count * sizeof(thread_t));
	}
#else	MACH_NO_KERNEL
#if	MACH
	th = p->thread;
	th->priority = BASEPRI_USER + 2*n;
	s = splsched();
	thread_lock(th);
	compute_priority(th);
	thread_unlock(th);
	splx(s);
#else	MACH
	(void) setpri(p);
#endif	MACH
#endif	MACH_NO_KERNEL
}

setrlimit()
{
	register struct a {
		u_int	which;
		struct	rlimit *lim;
	} *uap = (struct a *)u.u_ap;
	struct rlimit alim;
	register struct rlimit *alimp;
#if	MACH
#else	MACH
	extern unsigned maxdmap;
#endif	MACH

	if (uap->which >= RLIM_NLIMITS) {
		u.u_error = EINVAL;
		return;
	}
	alimp = &u.u_rlimit[uap->which];
	u.u_error = copyin((caddr_t)uap->lim, (caddr_t)&alim,
		sizeof (struct rlimit));
	if (u.u_error)
		return;
	if (alim.rlim_cur > alimp->rlim_max || alim.rlim_max > alimp->rlim_max)
		if (!suser())
			return;
#if	MACH
	if (uap->which == RLIMIT_STACK) {
		vm_offset_t 	new_stack;
		vm_offset_t	old_stack;
		kern_return_t	ret;

		if (u.u_stack_grows_up) {
			new_stack = trunc_page(u.u_stack_start) +
					round_page(alim.rlim_cur);
			old_stack = (vm_offset_t) u.u_stack_end;
			if (new_stack > old_stack) {
#if	MACH_NO_KERNEL
				ret = vm_allocate(u.u_procp->p_task,
#else	MACH_NO_KERNEL
				ret = vm_allocate(current_task()->map,
#endif	MACH_NO_KERNEL
					&old_stack,
					(new_stack - old_stack),
					FALSE);
			} else if (new_stack < old_stack) {
#if	MACH_NO_KERNEL
				ret = vm_deallocate(u.u_procp->p_task,
#else	MACH_NO_KERNEL
				ret = vm_deallocate(current_task()->map,
#endif	MACH_NO_KERNEL
					new_stack,
					(old_stack - new_stack));
			} else
				ret = KERN_SUCCESS;

			if (ret == KERN_SUCCESS)
				u.u_stack_end = (caddr_t) new_stack;
		} else {
			new_stack = round_page(u.u_stack_end) -
					round_page(alim.rlim_cur);
			old_stack = (vm_offset_t) u.u_stack_start;
			if (new_stack < old_stack) {
#if	MACH_NO_KERNEL
				ret = vm_allocate(u.u_procp->p_task,
#else	MACH_NO_KERNEL
				ret = vm_allocate(current_task()->map,
#endif	MACH_NO_KERNEL
					&new_stack,
					(old_stack - new_stack),
					FALSE);
			} else if (new_stack > old_stack) {
#if	MACH_NO_KERNEL
				ret = vm_deallocate(u.u_procp->p_task,
#else	MACH_NO_KERNEL
				ret = vm_deallocate(current_task()->map,
#endif	MACH_NO_KERNEL
					old_stack,
					(new_stack - old_stack));
			} else
				ret = KERN_SUCCESS;

			if (ret == KERN_SUCCESS)
				u.u_stack_start = (caddr_t) new_stack;
		}
		if (ret != KERN_SUCCESS)
			u.u_error = EINVAL;
	}
#else	MACH
	switch (uap->which) {

	case RLIMIT_DATA:
		if (alim.rlim_cur > maxdmap)
			alim.rlim_cur = maxdmap;
		if (alim.rlim_max > maxdmap)
			alim.rlim_max = maxdmap;
		break;

	case RLIMIT_STACK:
		if (alim.rlim_cur > maxdmap)
			alim.rlim_cur = maxdmap;
		if (alim.rlim_max > maxdmap)
			alim.rlim_max = maxdmap;
		break;
	}
#endif	MACH
	*alimp = alim;
#if	MACH_NO_KERNEL
#else	MACH_NO_KERNEL
	if (uap->which == RLIMIT_RSS)
		u.u_procp->p_maxrss = alim.rlim_cur/NBPG;
#endif	MACH_NO_KERNEL
}

getrlimit()
{
	register struct a {
		u_int	which;
		struct	rlimit *rlp;
	} *uap = (struct a *)u.u_ap;

	if (uap->which >= RLIM_NLIMITS) {
		u.u_error = EINVAL;
		return;
	}
	u.u_error = copyout((caddr_t)&u.u_rlimit[uap->which], (caddr_t)uap->rlp,
	    sizeof (struct rlimit));
}

#if	MACH_NO_KERNEL
int
bsd_getrusage(proc_port, interrupt, which, rusage)
	mach_port_t	proc_port;
	boolean_t	*interrupt;
	int		which;
	struct rusage	*rusage;	/* OUT */
{
	register int	error;
	register struct rusage *rup;

	error = start_server_op(proc_port, SYS_getrusage);
	if (error)
	    return (error);
	switch (which) {
	    case RUSAGE_SELF:
		rup = &u.u_ru;
		break;
	    case RUSAGE_CHILDREN:
		rup = &u.u_cru;
		break;
	    default:
		return (end_server_op(EINVAL, interrupt));
	}
	*rusage = *rup;
	return (end_server_op(0, interrupt));
}

getrusage()
{
    panic("getrusage - MiG");
}
#else	MACH_NO_KERNEL
getrusage()
{
	register struct a {
		int	who;
		struct	rusage *rusage;
	} *uap = (struct a *)u.u_ap;
	register struct rusage *rup;
#if	MACH
	time_value_t		sys_time, user_time;
	register struct timeval	*tvp;
#endif	MACH

	switch (uap->who) {

	case RUSAGE_SELF:
#if	MACH
		/*
		 *	This is the current_thread.  Don't need to lock it.
		 */
		thread_read_times(current_thread(), &user_time, &sys_time);
		tvp = &u.u_ru.ru_utime;
		tvp->tv_sec = user_time.seconds;
		tvp->tv_usec = user_time.microseconds;
		tvp = &u.u_ru.ru_stime;
		tvp->tv_sec = sys_time.seconds;
		tvp->tv_usec = sys_time.microseconds;
#endif	MACH
		rup = &u.u_ru;
		break;

	case RUSAGE_CHILDREN:
		rup = &u.u_cru;
		break;

	default:
		u.u_error = EINVAL;
		return;
	}
	u.u_error = copyout((caddr_t)rup, (caddr_t)uap->rusage,
	    sizeof (struct rusage));
}
#endif	MACH_NO_KERNEL

ruadd(ru, ru2)
	register struct rusage *ru, *ru2;
{
	register long *ip, *ip2;
	register int i;

	timevaladd(&ru->ru_utime, &ru2->ru_utime);
	timevaladd(&ru->ru_stime, &ru2->ru_stime);
	if (ru->ru_maxrss < ru2->ru_maxrss)
		ru->ru_maxrss = ru2->ru_maxrss;
	ip = &ru->ru_first; ip2 = &ru2->ru_first;
	for (i = &ru->ru_last - &ru->ru_first; i > 0; i--)
		*ip++ += *ip2++;
}
