/* 
 * Mach Operating System
 * Copyright (c) 1987 Carnegie-Mellon University
 * All rights reserved.  The CMU software License Agreement specifies
 * the terms and conditions for use and redistribution.
 */
/* 
 * HISTORY
 * $Log:	kern_mman.c,v $
 * Revision 2.7  90/11/05  15:33:26  rpd
 * 	Fixed smmap for new vm_region semantics.
 * 	[90/11/02            rpd]
 * 
 * Revision 2.6  90/06/02  15:21:15  rpd
 * 	Removed MACH, MACH_NO_KERNEL conditionals.
 * 	Removed grow.
 * 	[90/05/13            rpd]
 * 	Removed mips conditionals in smmap.
 * 	[90/04/24            rpd]
 * 
 * 	Fixed smmap to not hold the master lock across vm_map.
 * 	[90/04/05            rpd]
 * 	Converted to new IPC.
 * 	Deallocate object name from vm_region.
 * 	[90/03/26  19:31:29  rpd]
 * 
 * Revision 2.5  90/05/29  20:22:55  rwd
 * 	Added some more debug code.
 * 	[90/05/02            rwd]
 * 
 * Revision 2.4  90/03/14  21:25:32  rwd
 * 	Added MAP_UAREA debug code to obreak.
 * 	[90/01/23            rwd]
 * 
 * Revision 2.3  89/11/29  15:44:11  af
 * 	Do not grant execute permissions in smmap() for mips.
 * 
 * Revision 2.2  89/08/09  14:35:28  rwd
 * 	Out-of-kernel version.
 * 	[89/01/02            dbg]
 * 
 * Revision 2.9  88/12/13  12:02:28  mikeg
 * 	Enclosed modifications to smmap for Suntools in "#ifdef sun"
 * 	conditionals.
 * 
 * Revision 2.8  88/12/08  15:35:24  mikeg
 * 	Modified smmap so that a call to mmap that invokes winmmap
 * 	will have the requested memory marked as MAP_PRIVATE.  This
 * 	allows Suntools to run under Mach.
 * 	[88/11/30  10:18:55  mikeg]
 * 
 * Revision 2.6  88/10/25  03:38:26  mwyoung
 * 	MACH_XP: Get the right offset for objects managed by the
 * 	device_pager.
 * 	[88/10/25            mwyoung]
 * 
 * Revision 2.5  88/10/11  12:07:00  rpd
 * 	Correct mmap() to copy data for MAP_PRIVATE; allow private
 * 	mappings to later increase privileges on the mapped region.
 * 	(From mwyoung.)
 * 	[88/10/11  12:01:46  rpd]
 * 
 * Revision 2.4  88/08/25  18:10:27  mwyoung
 * 	Corrected include file references.
 * 	[88/08/22            mwyoung]
 * 	
 * 	Eliminate old variables in smmap.
 * 	[88/08/21            mwyoung]
 * 	
 * 	Use memory_object_special when not MACH_XP; condense all of
 * 	the mapping operations to a single vm_map call.
 * 	[88/08/21  18:17:24  mwyoung]
 * 	
 * 	Vast simplification of mmap() through use of vm_map().
 * 	Further simplification would be possible if vm_object_special()
 * 	returned a memory object suitable for use in vm_map() instead of
 * 	a vm_object.
 * 	[88/08/20  03:07:54  mwyoung]
 * 
 * Revision 2.3  88/07/17  17:38:39  mwyoung
 * Use new memory object types.
 * 
 * Print the name of the process that can't sbrk.
 * 
 * Revision 2.2.1.2  88/07/04  15:18:40  mwyoung
 * Use new memory object types.
 * 
 * Revision 2.2.1.1  88/06/28  20:14:25  mwyoung
 * Print the name of the process that can't sbrk.
 * 
 *
 *  9-May-88  Michael Young (mwyoung) at Carnegie-Mellon University
 *	Print the name of the process that can't sbrk.
 *
 * 16-Feb-88  David Golub (dbg) at Carnegie-Mellon University
 *	Correct fix below to return proper U*X return code.  Check that
 *	mapfun is not NULL, nulldev, or nodev when mapping devices.
 *
 * 30-Jan-88  Richard Sanzi (sanzi) at Carnegie-Mellon University
 *	Added check to smmap to only work on local inodes.
 *
 * 30-Dec-87  David Golub (dbg) at Carnegie-Mellon University
 *	Delinted.
 *
 *  6-Dec-87  Michael Young (mwyoung) at Carnegie-Mellon University
 *	Changed inode_pager_setup() use to new interface.
 *	Removed meaningless history.
 *
 *  9-Apr-87  William Bolosky (bolosky) at Carnegie-Mellon University
 *	MACH_XP: Turned off device file mmaping for the interim.
 *
 *  2-Mar-87  David Golub (dbg) at Carnegie-Mellon University
 *	Made mmap handle both special devices and files for all machines.
 *
 * 14-Oct-86  David Golub (dbg) at Carnegie-Mellon University
 *	Made mmap work for character devices to support (sun) frame
 *	buffers.
 *
 **********************************************************************
 */
/*
 * Copyright (c) 1982, 1986 Regents of the University of California.
 * All rights reserved.  The Berkeley software License Agreement
 * specifies the terms and conditions for redistribution.
 *
 *	@(#)kern_mman.c	7.1 (Berkeley) 6/5/86
 */

#include <map_uarea.h>

#include <sys/param.h>
#include <sys/systm.h>
#include <sys/map.h>
#include <sys/dir.h>
#include <sys/user.h>
#include <sys/proc.h>
#include <sys/buf.h>
#include <sys/inode.h>
#include <sys/acct.h>
#include <sys/wait.h>
#include <machine/vmparam.h>
#include <sys/file.h>
#include <sys/vadvise.h>
#include <sys/trace.h>
#include <sys/mman.h>
#include <sys/conf.h>
#include <sys/parallel.h>
#include <uxkern/import_mach.h>

sbrk()
{
}

sstk()
{
}

getpagesize()
{
	u.u_r.r_val1 = NBPG * CLSIZE;
}

smmap()
{
	/*
	 *	Map in special device (must be SHARED) or file
	 */
	struct a {
		caddr_t	addr;
		int	len;
		int	prot;
		int	share;
		int	fd;
		off_t	pos;
	} *uap = (struct a *)u.u_ap;
	register struct file *fp;
	register struct inode *ip;
	task_t		user_map = u.u_procp->p_task;
	kern_return_t	result;
	vm_offset_t	user_addr = (vm_offset_t) uap->addr;
	vm_size_t	user_size = uap->len;
	off_t		file_pos = uap->pos;
	memory_object_t	pager;
	vm_offset_t	pager_offset;
	vm_prot_t	user_prot =
				(uap->prot & PROT_WRITE) ?
					VM_PROT_READ|VM_PROT_WRITE :
					VM_PROT_READ;
	int		realshare = uap->share; /* For winmmap kluge */

	extern struct file	*getinode();

	/*
	 *	Only work with local inodes.  Do not use getinode - it will
	 *	try to invoke remote filesystem code, which will panic on smmap.
	 */


	GETF(fp, uap->fd);
	if (fp->f_type != DTYPE_INODE) {
		u.u_error = EINVAL;
		printf("mmap: != DTYPE_INODE\n");
		return;
	}
	ip = (struct inode *)fp->f_data;

	/*
	 *	We bend a little - round the start and end addresses
	 *	to the nearest page boundary.
	 */
	user_addr = trunc_page(user_addr);
	user_size = round_page(user_size);

	/*
	 *	File can be COPIED at an arbitrary offset.
	 *	File can only be SHARED if the offset is at a
	 *	page boundary.
	 *
	if (uap->share == MAP_SHARED &&
	    (trunc_page(file_pos) != (vm_offset_t)(file_pos))) {
		u.u_error = EINVAL;
		printf("mmap: SHARED && !page boundry\n");
		return;
	}

	/*
	 *	File must be writable if mapping will be.
	 */
	if ((uap->prot & PROT_WRITE) && (fp->f_flag&FWRITE) == 0) {
		u.u_error = EINVAL;
		printf("mmap: file not writeable\n");
		return;
	}
	if ((uap->prot & PROT_READ) && (fp->f_flag&FREAD) == 0) {
		u.u_error = EINVAL;
		printf("mmap: file not readable\n");
		return;
	}

	/*
	 *	memory must exist and be writable (even if we're
	 *	just reading)
	 *
	 *	XXX Is this really part of the BSD spec for mmap?
	 *	We'll be replacing the address range anyway.
	 */

	{
	    register vm_offset_t	addr;
	    vm_offset_t			r_addr;
	    vm_size_t			r_size;
	    vm_prot_t			r_protection,
					r_max_protection;
	    vm_inherit_t		r_inheritance;
	    boolean_t			r_is_shared;
	    memory_object_name_t	r_object_name;
	    vm_offset_t			r_offset;

	    addr = user_addr;
	    while (addr < user_addr + user_size) {
		r_addr = addr;
		if (vm_region(user_map,
			      &r_addr,
			      &r_size,
			      &r_protection,
			      &r_max_protection,
			      &r_inheritance,
			      &r_is_shared,
			      &r_object_name,
			      &r_offset) != KERN_SUCCESS) {
		  check_prot_failure:
		    u.u_error = EINVAL;
		    printf("mmap: check protection failure\n");
		    return;
		}

		if (MACH_PORT_VALID(r_object_name))
		    (void) mach_port_deallocate(mach_task_self(),
						r_object_name);

		if ((r_addr > addr) || /* gap */
		    ((r_protection & (VM_PROT_READ|VM_PROT_WRITE))
			!= (VM_PROT_READ|VM_PROT_WRITE))) /* not writable */
		    goto check_prot_failure;

		addr = r_addr + r_size;
	    }
	}

	if ((ip->i_mode & IFMT) == IFCHR) {
		/*
		 *	Map in special device memory.
		 *	Must be shared, since we can't assume that
		 *	this memory can be made copy-on-write.
		 */
		dev_t	dev = ip->i_rdev;

		pager_offset = (vm_offset_t) file_pos;
		pager = device_pager_create(dev,
					    pager_offset,
					    user_size,
					    user_prot);
		if (pager == MACH_PORT_NULL) {
			u.u_error = EINVAL;
			printf("mmap: pager is MACH_PORT_NULL\n");
			return;
		}
	}
	else {
		/*
		 *	Map in a file.  May be PRIVATE (copy-on-write)
		 *	or SHARED (changes go back to file)
		 */

		/*
		 *	Only allow regular files for the moment.
		 */
		if ((ip->i_mode & IFMT) != IFREG) {
			u.u_error = EINVAL;
			return;
		}
		
		if ((pager = inode_pager_setup(ip, FALSE, FALSE))
				== MEMORY_OBJECT_NULL) {
			u.u_error = KERN_INVALID_ARGUMENT;
			return;
		}
		pager_offset = (vm_offset_t) file_pos;
	}

	/*
	 *	Deallocate the existing memory, then map the appropriate
	 *	memory object into the space left.
	 */

	result = vm_deallocate(user_map, user_addr, user_size);
	if (result == KERN_SUCCESS) {
		/*
		 *	We can't hold the master lock across vm_map,
		 *	because the inode pager will need it.
		 */

		if (u.u_master_lock)
			master_unlock();

		result = vm_map(user_map, &user_addr, user_size, 0, FALSE,
				pager, pager_offset,
				(uap->share != MAP_SHARED),
				user_prot,
				(uap->share == MAP_SHARED) ?
					user_prot : VM_PROT_READ|VM_PROT_WRITE,
				((realshare == MAP_SHARED) ?
					VM_INHERIT_SHARE : VM_INHERIT_COPY)
				);

		if (u.u_master_lock)
			master_lock();
	}
	/*
	 *	Throw away references to the appropriate pager
	 */

	if ((ip->i_mode & IFMT) == IFCHR) {
		device_pager_release(pager);
	} else {
		inode_pager_release(pager);
	}

	if (result != KERN_SUCCESS) {
		u.u_error = EINVAL;
		return;
	}

	u.u_pofile[uap->fd] |= UF_MAPPED;
}

mremap()
{
}

munmap()
{
	register struct a {
		caddr_t	addr;
		int	len;
	} *uap = (struct a *)u.u_ap;
	vm_offset_t	user_addr;
	vm_size_t	user_size;
	kern_return_t	result;

	user_addr = (vm_offset_t) uap->addr;
	user_size = (vm_size_t) uap->len;
	if (trunc_page(user_addr) != user_addr ||
	    trunc_page(user_size) != user_size) {
		u.u_error = EINVAL;
		return;
	}
	result = vm_deallocate(u.u_procp->p_task, user_addr, user_size);
	if (result != KERN_SUCCESS) {
		u.u_error = EINVAL;
		return;
	}
}

munmapfd(fd)
{
	u.u_pofile[fd] &= ~UF_MAPPED;
}

mprotect()
{
}

madvise()
{
}

mincore()
{
}

/* BEGIN DEFUNCT */
obreak()
{
	struct a {
		char	*nsiz;
	};
	vm_offset_t	old, new;
	kern_return_t	ret;

#if	MAP_UAREA
	if (u.u_shared_rw->us_debug)
		printf("[debug]%s (%d)called obreak(%x)\n", u.u_comm, u.u_procp->p_pid,((struct a *)u.u_ap)->nsiz);
	if (u.u_shared_rw->us_inuse)
		printf("[inuse]%s (%d)called obreak(%x)\n", u.u_comm, u.u_procp->p_pid,((struct a *)u.u_ap)->nsiz);
#endif	MAP_UAREA

	new = round_page(((struct a *)u.u_ap)->nsiz);
	if ((int)(new - (vm_offset_t)u.u_data_start) > u.u_rlimit[RLIMIT_DATA].rlim_cur) {
		u.u_error = ENOMEM;
		return;
	}
	old = round_page(u.u_data_start + ctob(u.u_dsize));
	if (new > old) {
		ret = vm_allocate(u.u_procp->p_task, &old, new - old, FALSE);
		if (ret != KERN_SUCCESS) {
			uprintf("%s: could not sbrk, return = %d\n", u.u_comm, ret);
		} else
			u.u_dsize += btoc(new - old);
	}
}

ovadvise()
{
}
/* END DEFUNCT */
