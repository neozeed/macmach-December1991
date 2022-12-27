/* 
 * Mach Operating System
 * Copyright (c) 1987 Carnegie-Mellon University
 * All rights reserved.  The CMU software License Agreement specifies
 * the terms and conditions for use and redistribution.
 */
/* 
 * HISTORY
 * $Log:	mach_process.c,v $
 * Revision 2.6  90/06/02  15:22:06  rpd
 * 	Converted to new IPC.
 * 	Deallocate unwanted thread ports.
 * 	[90/03/26  19:38:42  rpd]
 * 
 * Revision 2.5  90/01/23  00:03:55  af
 * 	Added cache flushing when writing into I/D space.
 * 	Conditionalized on mips, to avoid penalizing
 * 	other architectures unnecessarily.
 * 	[89/12/09            af]
 * 
 * Revision 2.4  89/12/08  20:14:26  rwd
 * 	Need to include parallel.h
 * 	[89/12/05            rwd]
 * 
 * Revision 2.3  89/10/17  11:25:49  rwd
 * 	If p_thread is null, find a (random) thread in the task.
 * 	[89/09/20            dbg]
 * 
 * Revision 2.2  89/09/26  10:30:06  rwd
 * 	Unlock unix master around touches of user data
 * 	[89/09/19            rwd]
 * 
 * 	Moved get/set U to machine/bsd_machdep.c.
 * 
 * 	Out-of-kernel version.
 * 	[89/01/02            dbg]
 * 
 * Revision 2.1  89/08/04  14:06:35  rwd
 * Created.
 * 
 * Revision 2.4  88/10/18  03:15:08  mwyoung
 * 	Watch out for zero return value from kmem_alloc_wait.
 * 	[88/09/13            mwyoung]
 * 
 *
 *  4-May-88  David Black (dlb) at Carnegie-Mellon University
 *	Make sure p_stat is SSTOP in ptrace().
 *
 * 13-Mar-88  David Golub (dbg) at Carnegie-Mellon University
 *	Use vm_map_copy instead of playing with physical pages.
 *
 *  3-Mar-88  Michael Young (mwyoung) at Carnegie-Mellon University
 *	De-linted.
 *
 * 13-Oct-87  David Black (dlb) at Carnegie-Mellon University
 *	run_state --> user_stop_count.
 *
 * 24-Jul-87  David Black (dlb) at Carnegie-Mellon University
 *	Set modified bit on any pages modified by copy_to_phys.
 *
 * 13-Jul-87  David Black (dlb) at Carnegie-Mellon University
 *	If delivering a thread signal, set thread's u.u_cursig.
 *	Optimize and clean up register references.
 *
 *  2-Jul-87  David Black (dlb) at Carnegie-Mellon University
 *	Derived from sys_process.c via major rewrite to eliminate
 *	ipc structure and procxmt.
 */
 
/*
 * Copyright (c) 1982, 1986 Regents of the University of California.
 * All rights reserved.  The Berkeley software License Agreement
 * specifies the terms and conditions for redistribution.
 *
 *	@(#)sys_process.c	7.1 (Berkeley) 6/5/86
 */

#ifdef	sun
/* Use Sun version of ptrace */
#else	sun

#include <sys/param.h>
#include <sys/systm.h>
#include <sys/dir.h>
#include <sys/user.h>
#include <sys/proc.h>
#include <sys/ptrace.h>
#include <sys/parallel.h>

#include <uxkern/import_mach.h>

extern void start();

/*
 * sys-trace system call.
 */
ptrace()
{
	register struct proc *p;
	register struct a {
		int	req;
		int	pid;
		int	*addr;
		int	data;
	} *uap;

	vm_offset_t	start_addr, end_addr,
			kern_addr, offset;
	vm_size_t	size;
	unsigned	old_data;

	uap = (struct a *)u.u_ap;

	/*
	 *	Intercept and deal with "please trace me" request.
	 */
	if (uap->req <= 0) {
		u.u_procp->p_flag |= STRC;
		return;
	}

	/*
	 *	Locate victim, and make sure it is traceable.
	 */
	p = pfind(uap->pid);
	if (p == 0 || p->p_stat != SSTOP || p->p_ppid != u.u_procp->p_pid ||
	    !(p->p_flag & STRC)) {
		u.u_error = ESRCH;
		return;
	}

	/*
	 *	If the user thread to trace has not already been set.
	 *	pick one at random.  (We never said that ptrace really
	 *	worked on multi-threaded programs.)
	 */
	while (!MACH_PORT_VALID(p->p_thread)) {
	    thread_array_t	thread_list;
	    unsigned int	i, count;

	    if (task_threads(p->p_task, &thread_list, &count)
		    != KERN_SUCCESS) {
		u.u_error = EIO;
		return;
	    }

	    /*
	     * Use the first thread.
	     */
	    p->p_thread = thread_list[0];

	    /*
	     * Deallocate the rest, and deallocate the memory.
	     */

	    for (i = 1; i < count; i++)
		if (MACH_PORT_VALID(thread_list[i]))
		    (void) mach_port_deallocate(mach_task_self(),
						thread_list[i]);

	    (void) vm_deallocate(mach_task_self(),
				 (vm_offset_t)thread_list,
				 (vm_size_t) (count * sizeof(thread_list[0])));
	}

	/*
	 *	Mach version of ptrace executes request directly here,
	 *	thus simplifying the interaction of ptrace and signals.
	 */
	switch (uap->req) {

	case PT_READ_I:
	case PT_READ_D:
		/*
		 *	Read victim's memory
		 */
		start_addr = trunc_page(uap->addr);
		end_addr = round_page(uap->addr + sizeof(int));
		size = end_addr - start_addr;

		/*
		 * Unlock master lock to touch user data.
		 */
		if (u.u_master_lock)
		    master_unlock();

		if (vm_read(p->p_task, start_addr, size,
			    &kern_addr, &size)
		    != KERN_SUCCESS) {
			u.u_error = EIO;
			if (u.u_master_lock)
			    master_lock();
			break;
		}
		/*
		 *	Read the data from the copy in the kernel map.
		 *	Use bcopy to avoid alignment restrictions.
		 */
		offset = (vm_offset_t) uap->addr - start_addr;
		bcopy((caddr_t)(kern_addr + offset),
			(caddr_t)&u.u_r.r_val1,
			sizeof(int));

		/*
		 *	Discard the kernel's copy.
		 */
		(void)vm_deallocate(mach_task_self(), kern_addr, size);
		if (u.u_master_lock)
		    master_lock();

		break;

	case PT_WRITE_I:
	case PT_WRITE_D:
		/*
		 *	Write victim's memory
		 */
		start_addr = trunc_page(uap->addr);
		end_addr = round_page(uap->addr + sizeof(int));
		size = end_addr - start_addr;

		/*
		 *	Allocate some pageable memory in the kernel map,
		 *	and copy the victim's memory to it.
		 */

		/*
		 * Unlock master lock to touch user data.
		 */
		if (u.u_master_lock)
		    master_unlock();

		if (vm_read(p->p_task, start_addr, size,
			    &kern_addr, &size)
		    != KERN_SUCCESS) {
			u.u_error = EIO;
			if (u.u_master_lock)
			    master_lock();
			break;
		}
		/*
		 *	Change the data in the copy in the kernel map.
		 *	Use bcopy to avoid alignment restrictions.
		 */
		offset = (vm_offset_t)uap->addr - start_addr;
		bcopy((caddr_t)(kern_addr + offset), &old_data, sizeof(int));
		bcopy((caddr_t)&uap->data,
			(caddr_t)(kern_addr + offset),
			sizeof(int));

		/*
		 *	Copy it back to the victim.
		 */
		if (vm_write(p->p_task, start_addr,
			     kern_addr, size)
			!= KERN_SUCCESS) {
		    /*
		     * Area may not be writable.  Try changing
		     * its protection.
		     */
		    if (vm_protect(p->p_task, start_addr, size,
				   FALSE,
				   VM_PROT_READ|VM_PROT_WRITE|VM_PROT_EXECUTE)
			    != KERN_SUCCESS)
			u.u_error = EIO;
		    else {
			if (vm_write(p->p_task, start_addr,
					kern_addr, size)
				!= KERN_SUCCESS)
			    u.u_error = EIO;
			(void) vm_protect(p->p_task, start_addr, size,
			    		FALSE,
					VM_PROT_READ|VM_PROT_EXECUTE);
		    }
		}
		/*
		 *	Discard the kernel's copy.
		 */
		(void)vm_deallocate(mach_task_self(), kern_addr, size);

		if (u.u_master_lock)
		    master_lock();

#ifdef	mips
		{
			vm_machine_attribute_val_t flush = MATTR_VAL_CACHE_FLUSH;
			kern_return_t ret;
			ret = vm_machine_attribute(
					p->p_task, uap->addr, sizeof(int),
					MATTR_CACHE, &flush);
			u.u_r.r_val1 = old_data;
		}
#endif	mips
		break;

	case PT_READ_U:
		/*
		 *	Read victim's U-area or registers.
		 *	Offsets are into BSD kernel stack, and must
		 *	be faked to match MACH.
		 */
		if (ptrace_get_u_word(p, uap->addr, &u.u_r.r_val1))
		    u.u_error = EIO;
		break;

	case PT_WRITE_U:
		/*
		 *	Write victim's registers.
		 *	Offsets are into BSD kernel stack, and must
		 *	be faked to match MACH.
		 */
		if (ptrace_set_u_word(p, uap->addr, uap->data, &u.u_r.r_val1))
		    u.u_error = EIO;
		break;

	case PT_KILL:
		/*
		 *	Force process to exit.
		 */
		proc_exit(p, p->p_stopsig, FALSE);
		break;

	case PT_STEP:
		/*
		 *	Single step the child.
		 */
		if ((unsigned)uap->data > NSIG) {
		    u.u_error = EIO;
		    break;
		}

		ptrace_set_trace(p, (int)uap->addr, TRUE);
		start(p, uap->data);
		break;

	case PT_CONTINUE:
		/*
		 *	Continue the child.
		 */
		if ((unsigned)uap->data > NSIG) {
		    u.u_error = EIO;
		    break;
		}

		if ((int)uap->addr != 1)
		    ptrace_set_trace(p, (int)uap->addr, FALSE);
		start(p, uap->data);
		break;

	default:
		u.u_error = EIO;
	}
}
#endif	sun
