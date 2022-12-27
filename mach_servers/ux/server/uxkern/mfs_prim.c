/* 
 * Mach Operating System
 * Copyright (c) 1990 Carnegie-Mellon University
 * Copyright (c) 1987 Carnegie-Mellon University
 * All rights reserved.  The CMU software License Agreement specifies
 * the terms and conditions for use and redistribution.
 */
/*
 *	File:	mfs_prim.h
 *	Author:	Avadis Tevanian, Jr.
 *
 *	Copyright (C) 1987, Avadis Tevanian, Jr.
 *
 *	Support for mapped file system implementation.
 *
 * HISTORY
 * $Log:	mfs_prim.c,v $
 * Revision 2.11  91/03/20  15:00:25  dbg
 * 	Change user_request_it to get *file.  Save for later use.  This removes
 * 	race in multi-threaded user programs.
 * 	[91/03/12            rwd]
 * 
 * Revision 2.10  91/01/08  17:44:32  rpd
 * 	Can not have multiple threads doing an override at the same time
 * 	[90/12/20  16:22:21  rvb]
 * 
 * Revision 2.9  90/11/07  13:22:09  rpd
 * 	Postpone distrust of user until last reference to file goes away.
 * 	[90/11/06            rwd]
 * 
 * Revision 2.8  90/10/25  15:07:25  rwd
 * 	Fix PD.  Don't trust users dirty bit.
 * 	[90/10/24            rwd]
 * 
 * Revision 2.7  90/08/06  15:34:47  rwd
 * 	We no longer need to zero on trunc.  Just flush the page that
 * 	needs a partial zero and let the file system handle the rest.
 * 	[90/08/03            rwd]
 * 
 * 	Check return status of memory_object_lock_request in ino_flush.
 * 	[90/07/31            rwd]
 * 	Fix mfs_get so only one server thread is "IT".
 * 	[90/07/27            rwd]
 * 
 * 	Replace pager_wait_lock with ip->pager_wait_lock.
 * 	[90/07/27            rwd]
 * 	Added more debugging messages.  Fix trunc post zeroing calculations.
 * 	Must do pending trunc zero on close of file.
 * 	[90/07/12  13:21:22  rwd]
 * 
 * 	Seems like a good time for some comments.
 * 	[90/07/09            rwd]
 * 
 * 	Fix mfs_trunc code to set trunc_zero so next remapping zeros
 * 	partial page.
 * 	[90/07/05            rwd]
 * 	Mfs_put should call user_unmap_inode not user_map_inode since it
 * 	is already "IT"
 * 	[90/06/28            rwd]
 * 	Fix unmap_inode bug.  Fix share_lock references.
 * 	[90/06/08            rwd]
 * 
 * Revision 2.6  90/06/19  23:15:54  rpd
 * 	Fixed bug in ino_flush: always flush, even when not dirty.
 * 	[90/06/19            rpd]
 * 
 * 	Folded user_trunc into mfs_trunc.  When truncating, flush pages
 * 	past the new length.
 * 	[90/06/12            rpd]
 * 
 * Revision 2.5  90/06/02  15:27:51  rpd
 * 	Fixed up for compilation as a separate file.
 * 	[90/06/01            rpd]
 * 
 * 	Converted Randy's new code to new IPC.
 * 	[90/06/01            rpd]
 * 
 * 	Converted to new IPC.
 * 	[90/03/26  20:17:18  rpd]
 * 
 * Revision 2.4  90/05/29  20:25:12  rwd
 * 	Locking, the bane of my existence.  Fix mfs_put bug.
 * 	Fix logic for waking up someone wanting to be "IT".
 * 	[90/05/15            rwd]
 * 	Remove gross MU_LOCK/UNLOCK macros.  Do the locking in a cleaner way.
 * 	[90/05/15  12:05:57  rwd]
 * 
 * 	Remove panic in mfs_get which only occurs when called from exec.
 * 	We do the correct thing in this case.
 * 	[90/05/14            rwd]
 * 	Fix locking bug in user_map_inode.
 * 	[90/05/14            rwd]
 * 	New NBC code for server to mesh correctly with user NBC code.
 * 	[90/05/10            rwd]
 * 	Change logic to vm_map new area before vm_deallocating old area.
 * 	[90/04/23            rwd]
 * 	Remove cacheing.  Change locking to rid us of the master lock.
 * 	[90/04/18            rwd]
 * 	Add override parameter and caching code.
 * 	[90/04/14            rwd]
 * 	Add new user_ routines for mapping into user space.
 * 	[90/03/21            rwd]
 * 
 * Revision 2.3  90/03/14  21:30:50  rwd
 * 	Bug fix from rfr.
 * 	[90/03/06            rwd]
 * 
 * Revision 2.2  90/01/19  14:38:10  rwd
 * 	First checkin
 * 	[90/01/15            rwd]
 * 
 * Revision 2.3  89/01/15  21:24:23  rpd
 * 	Updated includes to the new style.
 * 	Use decl_simple_lock_data.
 * 
 * 29-Jan-88  David Golub (dbg) at Carnegie-Mellon University
 *	Corrected calls to inode_pager_setup and kmem_alloc.
 *
 * 15-Sep-87  Michael Young (mwyoung) at Carnegie-Mellon University
 *	De-linted.
 *
 * 18-Jun-87  Michael Young (mwyoung) at Carnegie-Mellon University
 *	Make most of this file dependent on MACH_NBC.
 *
 * 30-Apr-87  Avadis Tevanian (avie) at Carnegie-Mellon University
 *	Created.
 */

#include <map_uarea.h>

#include <sys/types.h>
#include <sys/param.h>
#include <sys/buf.h>
#include <sys/conf.h>
#include <sys/dir.h>
#include <sys/file.h>
#include <sys/fs.h>
#include <sys/inode.h>
#include <sys/ioctl.h>
#include <sys/kernel.h>
#include <sys/mfs.h>
#include <sys/mount.h>
#include <sys/parallel.h>
#include <sys/proc.h>
#include <sys/stat.h>
#include <sys/systm.h>
#include <sys/tty.h>
#include <sys/uio.h>
#include <sys/user.h>
#include <uxkern/import_mach.h>
#include <vm/inode_pager.h>

#include <machine/vmparam.h>

int	nbc_debug = 0x0;

/*
 *	Private variables and macros.
 */

#if	MAP_UAREA

boolean_t		mfs_flush = FALSE; /* flush on push? */
boolean_t		mfs_initialized = FALSE;

#define CHUNK_SIZE	(64*1024)	/* XXX */

#define USER_MAP_DEBUG 1
#if	USER_MAP_DEBUG
int	user_map_debug = 0;
#define PD(mask,statement) if (user_map_debug & mask) statement
/*
 * 1	ino_*
 * 2	request,release,free
 * 4	map,unmap,remap
 * 8	mfs_get,put
 * 100	conditions
 * 200	vm_*
 * 400	internal debugging
 */
#else	USER_MAP_DEBUG
#define PD(0xn,x)
#endif	USER_MAP_DEBUG

/*
 * mu_*lock saves the master_lock count and releases it.
 */

mu_lock(ip)
register struct inode *ip;
{
	if (u.uu_master_lock) {
	    register int tmp = u.uu_master_lock;
	    u.uu_master_lock = 0;
	    master_unlock();
	    mutex_lock(&ip->mu_lock);
	    ip->master_count = tmp;
	} else {
	    mutex_lock(&ip->mu_lock);
	    ip->master_count = 0;
	}
}

mu_unlock(ip)
register struct inode *ip;
{
	if (ip->master_count) {
	    register int tmp = ip->master_count;
	    ip->master_count = 0;
	    mutex_unlock(&ip->mu_lock);
	    master_lock();
	    u.uu_master_lock = tmp;
	} else {
	    mutex_unlock(&ip->mu_lock);
	}
}

mfs_init()
{
	register struct inode	*ip;
	int			i;

	if (mfs_initialized) return;
	mfs_initialized = TRUE;

	ip = inode;
	for (i = 0; i < ninode; i++) {
	    ip->dirty = FALSE;
	    ip->mapped = FALSE;
	    ip->user_mapped = FALSE;
	    ip->serv_mapped = FALSE;
	    ip->inode_size = 0;
	    mutex_init(&ip->mu_lock);
	    condition_init(&ip->pager_wait);
	    ip->waiting = FALSE;
	    ip->count = 0;
	    ip->who = 0;
	    ip->wants = 0;
	    ip->temp_ref = FALSE;
	    ip->master_count = 0;
	    ip->ref_count = 0;
	    ip->needs_push = FALSE;
	    ip->over_ride = FALSE;
	    mutex_init(&ip->pager_wait_lock);
	    condition_init(&ip->mu_condition);
	}
}

ino_flush(ip, start, size)
	struct inode		*ip;
	register vm_offset_t	start;
	vm_size_t		size;
{
	vm_offset_t	page_start;
	vm_offset_t	page_end;
	vm_size_t	total_size;
	kern_return_t	status;

	PD(0x1,printf("ino_flush: %x %x %x %x %x\n",ip,start,size,ip->pager_request,ip->dirty));
	mutex_lock(&ip->pager_wait_lock);

	if (ip->pager_request == MACH_PORT_NULL) {
	    mutex_unlock(&ip->pager_wait_lock);
	    return;
	}

	page_start = trunc_page(start);
	page_end = round_page(start+size);
	total_size = page_end-page_start;

	status =  memory_object_lock_request(ip->pager_request, page_start, 
			total_size, FALSE, TRUE, VM_PROT_ALL, ip->pager);
	if (!status) {
	    if (nbc_debug&0x10) 
		printf("Waiting for condition in ino_flush: %x\n", ip->i_id);

	    ip->waiting = TRUE;
	    while (ip->waiting) {
		condition_wait(&ip->pager_wait, &ip->pager_wait_lock);
	    }
	}
	mutex_unlock(&ip->pager_wait_lock);

	if (nbc_debug&0x10) 
		printf("ino_flush done: %x\n", ip->i_id);
}
ino_push(ip, start, size)
	struct inode		*ip;
	vm_offset_t		start;
	vm_size_t		size;
{
	vm_offset_t	page_start;
	vm_offset_t	page_end;
	vm_size_t	total_size;

	PD(0x1,printf("ino_push: %x %x %x %x %x\n",ip,start,size,ip->pager_request,ip->dirty));
	if (!ip->dirty) return;
	ip->dirty = FALSE;

	mutex_lock(&ip->pager_wait_lock);

	if (ip->pager_request == MACH_PORT_NULL) {
	    mutex_unlock(&ip->pager_wait_lock);
	    return;
	}

	page_start = trunc_page(start);
	page_end = round_page(start+size);
	total_size = page_end-page_start;
	
	memory_object_lock_request(ip->pager_request, page_start, total_size, 
				   TRUE, mfs_flush, VM_PROT_NONE, ip->pager);

	if (nbc_debug&0x10) 
		printf("Waiting for condition in ino_push: %x\n", ip->i_id);

	ip->waiting = TRUE;
	while (ip->waiting) {
		condition_wait(&ip->pager_wait, &ip->pager_wait_lock);
	}
	mutex_unlock(&ip->pager_wait_lock);

	if (nbc_debug&0x10) 
		printf("ino_push done: %x\n", ip->i_id);
}

/*
 * Enter holding fi->lock
 */

ino_clean(ip, file, fi)
    struct inode *ip;
    struct file *file;
    struct file_info *fi;
{
    if (ip->i_nlink == 0) {
	if ((ip->which == -1) || (file->f_flag & (FWRITE|FTRUNC|FAPPEND)))
	    if (fi->map_info.inode_size > ip->inode_size)
		ip->inode_size = fi->map_info.inode_size;
	ip->dirty = FALSE;
	fi->dirty = FALSE;
	return;
    }
    if ((ip->which == -1) || (file->f_flag & (FWRITE|FTRUNC|FAPPEND))) {
	if (fi->map_info.inode_size > ip->inode_size)
	    ip->inode_size = fi->map_info.inode_size;
	if (ip->dirty = fi->dirty) {
	    ip->i_flag |= IUPD|ICHG;
	    ino_push(ip, fi->map_info.offset, fi->map_info.size);
	} else if (ip->which != -1)
	    ip->needs_push = 1;
    }
    fi->dirty = FALSE;
}

/*
 * This is the exit condition of holding mu_lock(ip);
 */

user_free_it(ip, override)
register struct inode *ip;
{
	register struct proc *p = u.u_procp;
	register int master_count;

	PD(0x2,printf("user_free_it: %x %x\n",ip,override));
	mu_lock(ip);
	/*
	 * If noone is "It" then no problem
	 */
	while (ip->who) {
	    register struct file_info *fi;
	    register struct file *file;

	    if (ip->which == -1) {
		fi = &ip->file_info;
		file = NULL;
	    } else {
		fi = &ip->who->u_shared_rw->us_file_info[ip->which];
		file = ip->which_fp;
	    }
	    share_lock(&fi->lock, p);
	    if (fi->inuse && override == 1 && ip->over_ride == FALSE && ip->who == &p->p_utask) {
		ip->over_ride = TRUE;
		fi->inuse = FALSE;
	    }
	    /*
	     * If inuse then wait for wakeup.  "IT" will clean up
	     * for himself
	     */
	    if (fi->inuse) {
		fi->wants = TRUE;
		ip->wants++;
		share_unlock(&fi->lock, p);
		ux_server_thread_busy();
		PD(0x100,printf("user_free_it: condition_wait\n"));
		master_count = ip->master_count;
		ip->master_count = 0;
		condition_wait(&ip->mu_condition, &ip->mu_lock);
		ip->master_count = master_count;
		ip->wants--;
		PD(0x100,printf("user_free_it: after condition_wait\n"));
		ux_server_thread_active();
	    } else {
	    /*
	     * Just take it away.  Remeber to clean up after him.
	     */
		fi->control = FALSE;
		if (ip->which != -1)
		    file->f_offset = fi->offset;
		ino_clean(ip, file, fi);
		share_unlock(&fi->lock, p);
		ip->who = 0;
	    }
	}
}

user_request_it(ip, indx, override, fp)
register struct inode *ip;
int	indx;
int	override;
struct file *fp;
{
	register struct file_info *fi = &u.u_shared_rw->us_file_info[indx];
	register struct proc *p = u.u_procp;

	PD(0x2,printf("user_request_it: %x %x\n",ip,indx));
	if (!ip->mapped)
		panic("Not mapped");
	/*
	 * Make sure noone is "IT" and get mu_lock
	 */
	user_free_it(ip, override);
	ip->who = ((indx == -1)?(struct utask *)mach_task_self():&p->p_utask);
	ip->which = indx;
	ip->which_fp = fp;
	share_lock(&fi->lock, p);
	fi->control = TRUE;
	fi->inuse = TRUE;
	fi->wants = (ip->wants?1:0);
	if (indx != -1)
	    fi->offset = fp->f_offset;
	fi->map_info.inode_size = ip->inode_size;
	fi->dirty = FALSE;
	share_unlock(&fi->lock, p);
	mu_unlock(ip);
}

user_release_it(ip)
register struct inode *ip;
{
	register struct file_info *fi;
	register struct proc *p = u.u_procp;
	register struct file *file;

	PD(0x2,printf("user_release_it: %x\n",ip));
	if (!ip->mapped)
		panic("Not mapped");
	mu_lock(ip);
	if (ip->who == 0)
		panic("user_release_it:ip->who == 0");
	if (ip->which == -1) {
	    fi = &ip->file_info;
	    file = NULL;
	} else {
	    fi = &u.u_shared_rw->us_file_info[ip->which];
	    file  = ip->which_fp;
	}
	share_lock(&fi->lock, p);
	if (ip->which != -1)
	    file->f_offset = fi->offset;
	ino_clean(ip, file, fi);
	fi->control = 0;
	fi->wants = FALSE;
	fi->inuse = 0;
	share_unlock(&fi->lock, p);
	ip->who = 0;
	ip->over_ride = FALSE;
	if (ip->wants)
		condition_signal(&ip->mu_condition);
	mu_unlock(ip);
}

user_map_inode(ip, mode, indx)
register struct inode *ip;
int mode,indx;
{
	register struct file_info *fi;
	register struct map_info *mi;
	register struct proc *p = u.u_procp;

	PD(0x4,printf("user_map_inode: %x %x %x\n",ip,mode,indx));
	if (!mfs_initialized) mfs_init();
	mu_lock(ip);
	if (!ip->mapped)
	    ip->inode_size = ip->i_size;
	if (indx == -1) {
	    if (ip->ref_count++ > 0) {
		mu_unlock(ip);;
		return;
	    }
	    ip->serv_mapped = TRUE;
	    fi = &ip->file_info;
	} else {
	    fi = &u.u_shared_rw->us_file_info[indx];
	    ip->user_mapped = TRUE;
	    ip->count++;
	}
	ip->mapped = TRUE;
	mu_unlock(ip);

	mi = &fi->map_info;
	share_lock(&fi->lock, p);
	fi->open = TRUE;
	fi->read = FALSE;
	fi->write = FALSE;
	fi->append = FALSE;
	if (mode & FREAD) fi->read = TRUE;
	if (mode & (FWRITE|FTRUNC|FAPPEND)) fi->write = TRUE;
	if (mode & FAPPEND) fi->append = TRUE;
	fi->mapped = TRUE;
	fi->control = FALSE;
	fi->inuse = FALSE;
	fi->dirty = FALSE;
	if (indx != -1)
	    fi->offset = u.u_ofile[indx]->f_offset;
	mi->address = 0;
	mi->size = 0;
	mi->offset = 0;
	mi->inode_size = ip->inode_size;
	share_unlock(&fi->lock, p);
	user_request_it(ip, indx, 0, u.u_ofile[indx]);
	if (mode & FAPPEND) {
	    user_remap_inode(ip, ip->inode_size, CHUNK_SIZE);
	} else {
	    user_remap_inode(ip, fi->offset, CHUNK_SIZE);
	}
	share_lock(&fi->lock, p);
	if (fi->wants) {
	    share_unlock(&fi->lock, p);
	    user_release_it(ip);
	} else {
	    fi->inuse=FALSE;
	    share_unlock(&fi->lock, p);
	}
}

map_inode(ip)
register struct inode *ip;
{
	user_map_inode(ip, FREAD|FWRITE, -1);
}

/* 
 * you are "IT" when called so
 * strict MU_LOCKing is not necessary
 */
user_unmap_inode(ip)
register struct inode *ip;
{
	register struct file_info *fi;
	register struct proc *p = u.u_procp;
	register who = ip->which;

	PD(0x4,printf("user_unmap_inode: %x\n",ip));
	if (!ip->mapped) {
		/* panic("Not mapped"); */
		printf("Not mapped\n");
		return;
	}

	if (ip->which == -1) {
	    fi = &ip->file_info;
	} else {
	    fi = &u.u_shared_rw->us_file_info[ip->which];
	}
	mu_lock(ip);
	if (ip->which != -1 || ip->ref_count == 1) {
	    share_lock(&fi->lock, p);
	    fi->mapped = FALSE;
	    fi->open = FALSE;
	    share_unlock(&fi->lock, p);
	    mu_unlock(ip);
	    user_remap_inode(ip, 0, 0);
	} else {
	    mu_unlock(ip);
	}

	user_release_it(ip);

	mu_lock(ip);
	if (who == -1) {
	    if (--ip->ref_count == 0) {
		ip->serv_mapped = FALSE;
	    }
	} else {
	    if ((--ip->count) == 0) {
		ip->user_mapped = FALSE;
	    }
	}
	if (!ip->user_mapped && !ip->serv_mapped) {
	    if (ip->i_nlink == 0) {
		ino_flush(ip, 0, ip->inode_size);
	    }
	    else if (ip->needs_push) {
		ip->dirty = TRUE;
		ino_push(ip, 0, ip->inode_size);
	    }
	    ip->mapped = FALSE;
	}
	mu_unlock(ip);
}

unmap_inode(ip)
register struct inode *ip;
{
	user_request_it(ip, -1, 0, 0);
	user_unmap_inode(ip);
}

user_remap_inode(ip, start, size)
register struct inode *ip;
{
	register struct proc *p = u.u_procp;
	register struct file_info *fi;
	register struct file *file;
	register struct map_info *mi;
	vm_prot_t prot;
	register kern_return_t ret;
	int dadd;
	int dsize = 0;
	int dwhich;
	int anywhere;
	task_t atask;
	vm_inherit_t inher;

	PD(0x2,printf("user_remap_inode: %x %x %x\n",ip,start,size));
	if (!ip->mapped)
		panic("Not mapped");

	mu_lock(ip);
	if (!ip->who) { /* This can happen as result of trunc code */
	    mu_unlock(ip);
	    return;
	}

	if (ip->which == -1) {
	    fi = &ip->file_info;
	    file = NULL;
	} else {
	    fi = &ip->who->u_shared_rw->us_file_info[ip->which];
	    file = ip->which_fp;
	}
	mi = &fi->map_info;

	share_lock(&fi->lock, p);
	/*
	 * if currently mapped, clean up
	 */
	if (mi->size > 0) {
	  ino_clean(ip, file, fi);
	  /*
	   * If server mapped, deallocate, else if this is the only reference
	   * to the region then we will deallocate it later else decrement
	   * reference
	   */
	  if (ip->which != -1) {
	    if (ip->who->u_fmap[ip->who->u_findx[ip->which]] == 1) {
		dsize = mi->size;
		dadd = mi->address;
		dwhich = ip->who->u_findx[ip->which];
	    } else {
		ip->who->u_fmap[ip->who->u_findx[ip->which]]--;
	    }
	  } else {
	    PD(0x200,printf("(%d)vm_deallocate %x inode = %x\n",-1,mi->address,(int)ip));
	    vm_deallocate(mach_task_self(), mi->address, mi->size);
	  }
	  mi->address = 0;
	}
	/*
	 * If this was a "get rid of mappings" call just dellocate if
	 * necessary and return.
	 */
	if (size == 0) {
		mi->offset = 0;
		mi->size = 0;
		if (dsize) {
		    ip->who->u_fmap[dwhich]--;
		    PD(0x200,printf("(%d)vm_deallocate %x inode = %x\n",(ip->which==-1?-1:ip->who->u_shared_ro->us_pid),mi->address,(int)ip));
		    vm_deallocate(ip->who->u_procp->p_task, dadd, dsize);
		}
		share_unlock(&fi->lock, p);
		mu_unlock(ip);
		return;
	}
	mi->offset = trunc_page(start);
	mi->size = CHUNK_SIZE;
	(void) inode_pager_setup(ip, FALSE, TRUE);
	if (ip->which != -1) {
	    register int j;
	    mi->address = 0;
	    for(j=ip->which;!mi->address;j++) { /* find free mapping area */
		if (j == NOFILE) j = 0;
		if (ip->who->u_fmap[j] == 0) {
		    mi->address = MAP_FILE_BASE + CHUNK_SIZE * j;
		    ip->who->u_fmap[j]++;
		    ip->who->u_findx[ip->which]=j;
		}
	    }
	    prot = VM_PROT_READ | 
	       (file->f_flag & ((FWRITE|FTRUNC|FAPPEND)?VM_PROT_WRITE:0));
	    anywhere = FALSE;
	    atask = ip->who->u_procp->p_task;
	    inher = VM_INHERIT_SHARE;
	} else {
	    prot = (VM_PROT_READ|VM_PROT_WRITE);
	    anywhere = TRUE;
	    atask = mach_task_self();
	    inher = VM_INHERIT_NONE;
	    mi->address = 0;
	}
	PD(0x200,printf("(%d)vm_map %x inode = %x\n",(ip->which==-1?-1:ip->who->u_shared_ro->us_pid),(anywhere?0:mi->address),(int)ip));
	ret = vm_map(atask, &mi->address, mi->size, 0, anywhere,
		     ip->pager, mi->offset, 0, prot, prot, inher);
	if (ret != KERN_SUCCESS)  {
		printf("vm_map ip=%x addr=%x size=%x off=%x pager=%d ret=%d\n",
			ip,mi->address,mi->size,mi->offset,ip->pager,ret);
		panic("user_remap: vm_map");
	}
	mutex_lock(&ip->pager_wait_lock);
	if (ip->pager_request == MACH_PORT_NULL) {
	    PD(0x100,printf("Waiting for condition in user_remap_inode: %x\n", ip->pager));
	    ip->waiting = TRUE;
	    while(ip->waiting) {
		condition_wait(&ip->pager_wait, &ip->pager_wait_lock);
	    }
	    PD(0x100,printf("Done waiting in user_remap_inode: %x %x\n", ip->pager,ip->pager_request));
	}
	mutex_unlock(&ip->pager_wait_lock);

	(void) inode_pager_release(ip->pager);
	if (dsize) { /* dellocate now */
	    ip->who->u_fmap[dwhich]--;
	    PD(0x200,printf("(%d)vm_deallocate %x inode = %x\n",ip->who->u_shared_ro->us_pid,dadd,(int)ip));
	    vm_deallocate(ip->who->u_procp->p_task, dadd, dsize);
	}
	share_unlock(&fi->lock, p);
	mu_unlock(ip);
}

/*
 * For now this should be correct but slow.  The way this is written,
 * any dirty pages will be written even if past end of new trunc
 * value.  Clean up in future.
 */

mfs_trunc(ip, length)
register struct inode *ip;
int length;
{
	PD(0x4,printf("mfs_trunc: %x %x\n",ip,length));

	if (!ip->mapped)
		return;

	mu_lock(ip);
	while (ip->who) {
	    mu_unlock(ip);
	    user_remap_inode(ip, 0, 0);
	    user_free_it(ip, 0);
	}

	if (ip->inode_size < length) {
	    mu_unlock(ip);
	    return;
	}

	if (round_page(ip->inode_size) > trunc_page(length))
		ino_flush(ip, trunc_page(length),
			  round_page(ip->inode_size) - trunc_page(length));

	ip->inode_size = length;
	mu_unlock(ip);
}

mfs_get(ip)
register struct inode *ip;
{
	register struct file_info *fi = &ip->file_info;
	register struct proc *p = u.u_procp;
	int settemp = 0;

	PD(0x8,printf("mfs_get: %x\n",ip));
	if (!ip->mapped)
		panic("Not mapped");
	if (!ip->serv_mapped) {
#ifdef	NOT_NEEDED
	    panic("mfs_get non server mapped file");
#endif	NOT_NEEDED
	    map_inode(ip);
	    settemp = 1;
	}
	share_lock(&fi->lock, p);
	if (fi->control && !fi->inuse) {
	    fi->inuse = TRUE;
	    share_unlock(&fi->lock, p);
	    mu_lock(ip);
	} else {
	    share_unlock(&fi->lock, p);
	    user_free_it(ip, 0);
	    ip->who = (struct utask *)mach_task_self();
	    ip->which = -1;
	    share_lock(&fi->lock, p);
	    fi->control = TRUE;
	    fi->inuse = TRUE;
	    fi->wants = (ip->wants?1:0);
	    fi->map_info.inode_size = ip->inode_size;
	    fi->dirty = FALSE;
	    share_unlock(&fi->lock, p);
	}
	if (settemp) ip->temp_ref = TRUE;
	u.uu_save_master_count = ip->master_count;
	ip->master_count = 0;
	mu_unlock(ip);
}

mfs_put(ip)
register struct inode *ip;
{
	register struct file_info *fi = &ip->file_info;
	register struct proc *p = u.u_procp;
	int unmap = 0;

	PD(0x8,printf("mfs_put: %x\n",ip));
	mu_lock(ip);
	if (ip->temp_ref) {
	    ip->temp_ref = 0;
	    unmap = 1;
	}
	ip->master_count = u.uu_save_master_count;
	u.uu_save_master_count = 0;
	mu_unlock(ip);;
	if (unmap) {
		user_unmap_inode(ip);
	} else {
	    share_lock(&fi->lock, p);
	    if (fi->wants) {
		share_unlock(&fi->lock, p);
		user_release_it(ip);
	    } else {
		fi->inuse = FALSE;
		share_unlock(&fi->lock, p);
	    }
	}
}

/* no longer necessary since we deallocate when ref == 0. */
mfs_uncache(ip)
register struct inode *ip;
{
}

mfs_cache_clear()
{
}

mfs_cache_sync()
{
}

u_long
mfs_inode_size(ip)
	register struct inode *ip;
{
	if (ip->mapped) {
	    if (ip->who) {
		if (ip->which == -1)
		    return ip->file_info.map_info.inode_size;
		else if (ip->which_fp->f_flag &
				(FTRUNC|FWRITE|FAPPEND))
		    return ip->who->u_shared_rw->
			us_file_info[ip->which].map_info.inode_size;
		else
		    return ip->inode_size;
	    } else
		return ip->inode_size;
	} else
	    return ip->i_size;
}

#else	MAP_UAREA
queue_head_t		vm_info_queue;		/* lru list of structures */
struct mutex		vm_info_lock_data;
struct condition	mfs_space_available;

#define	vm_info_lock()		mutex_lock(&vm_info_lock_data)
#define	vm_info_unlock()	mutex_unlock(&vm_info_lock_data)

struct mutex			mfs_alloc_lock_data;
boolean_t			mfs_alloc_wanted;
long				mfs_alloc_blocks = 0;
boolean_t			mfs_flush = FALSE;

#define mfs_alloc_lock()	mutex_lock(&mfs_alloc_lock_data)
#define mfs_alloc_unlock()	mutex_unlock(&mfs_alloc_lock_data)

#define	unlock_master_if_held() {			\
	if (u.uu_master_lock > 0) master_unlock();	\
}

#define	lock_master_if_held() {				\
	if (u.uu_master_lock > 0) master_lock();	\
}

#define mfs_lock(ip) 			\
{					\
	unlock_master_if_held();	\
	mutex_lock(&ip->lock);		\
	lock_master_if_held();		\
}

#define mfs_unlock(ip) 			\
{					\
	mutex_unlock(&ip->lock);	\
}
	
/*
 *	mfs_map_size is the number of bytes of VM to use for file mapping.
 *	It should be set by machine dependent code (before the call to
 *	mfs_init) if the default is inappropriate.
 *
 *	mfs_max_window is the largest window size that will be given to
 *	a file mapping.  A default value is computed in mfs_init based on
 *	mfs_map_size.  This too may be set by machine dependent code
 *	if the default is not appropriate.
 */

vm_size_t	mfs_max_window = 0;		/* largest window to use */
int		mfs_max_files = 64;
int		mfs_file_cnt = 0;
boolean_t	mfs_initialized = FALSE;

#define CHUNK_SIZE	(64*1024)	/* XXX */


/*
 *	mfs_init:
 *
 *	Initialize the mfs module.
 */

mfs_init()
{
	register struct inode	*ip;
	int			i;

	if (mfs_initialized) return;
	mfs_initialized = TRUE;

	queue_init(&vm_info_queue);
	mutex_init(&vm_info_lock_data);
	mutex_init(&mfs_alloc_lock_data);

	condition_init(&mfs_space_available);
	mfs_file_cnt = 0;
	mfs_alloc_wanted = FALSE;

	if (mfs_max_window == 0) 	 mfs_max_window = CHUNK_SIZE;
	if (mfs_max_window < CHUNK_SIZE) mfs_max_window = CHUNK_SIZE;

	ip = inode;
	for (i = 0; i < ninode; i++) vm_info_init(ip++);
}

/*
 *	vm_info_init:
 *
 *	Initialize a vm_info structure for an inode.
 */
vm_info_init(ip)
	struct inode *ip;
{
	ip->map_count = 0;
	ip->use_count = 0;
	ip->va = 0;
	ip->size = 0;
	ip->offset = 0;
	ip->queued = FALSE;
	ip->dirty = FALSE;
	ip->temporary = FALSE;
	ip->mapped = FALSE;
	ip->inode_size = 0;
	mutex_init(&ip->lock);
	mutex_init(&ip->pager_wait_lock);
	condition_init(&ip->pager_wait);
	ip->waiting = FALSE;
}

/*
 *	map_inode:
 *
 *	Indicate that the specified inode should be mapped into VM.
 *	A reference count is maintained for each mapped file.
 */
map_inode(ip)
	register struct inode	*ip;
{
	if (!mfs_initialized) mfs_init();
	mfs_lock(ip);
	if ((ip->map_count++ > 0) || (ip->mapped)) {
		mfs_unlock(ip);
		return;
	}

	ip->va = 0;
	ip->size = 0;
	ip->offset = 0;
	ip->mapped = TRUE;

	ip->inode_size = ip->i_size;

	/*
	 *	If the file is less that the maximum window size then
	 *	just map the whole file now.
	 */

	if (ip->inode_size > 0 && ip->inode_size < mfs_max_window)
		remap_inode(ip, 0, ip->inode_size);

	mfs_unlock(ip);

	vm_info_lock();
	queue_enter(&vm_info_queue, ip, struct inode *, lru_links);
	ip->queued = TRUE;
	vm_info_unlock();
}

/*
 *	unmap_inode:
 *
 *	Called when an inode is closed.
 *	If there are no links left to the file then release
 *	the resources held.  If there are links left, then keep
 *	the file mapped under the assumption that someone else
 *	will soon map the same file.
 */
unmap_inode(ip)
	register struct inode	*ip;
{
	mfs_lock(ip);
	if ((!ip->mapped) || (--ip->map_count > 0)) {
		mfs_unlock(ip);
		return;
	}

	if (ip->i_nlink == 0) {
		mfs_unlock(ip);
		mfs_memfree(ip);
		return;
	} else {
		ino_push(ip, ip->offset, ip->size);
	}

	mfs_unlock(ip);
}

/*
 *	remap_inode:
 *
 *	Remap the specified inode (due to extension of the file perhaps).
 *	Upon return, it should be possible to access data in the file
 *	starting at the "start" address for "size" bytes.
 */
remap_inode(ip, start, size)
	register struct inode	*ip;
	vm_offset_t		start;
	register vm_size_t	size;
{
	vm_offset_t		addr, offset;
	kern_return_t		ret;

	/*
	 *	Remove old mapping (making its space available).
	 */
	if (ip->size > 0) {
		ino_push(ip, ip->offset, ip->size);
		mfs_map_remove(ip->va, ip->va + ip->size);
	}

	offset = trunc_page(start);
	size = round_page(start + size) - offset;
	if (size < CHUNK_SIZE) size = CHUNK_SIZE;
	do {
		addr = 0;
		mfs_alloc_lock();
		/*
		 *	If there was no space, see if we can free up mappings
		 *	on the LRU list.  If not, just wait for someone else
		 *	to free their memory.
		 */

		if (mfs_file_cnt > mfs_max_files)  {
			register struct inode	*ip1;
			ret = KERN_NO_SPACE;

			vm_info_lock();

			ip1 = (struct inode *)0;
			if (!queue_empty(&vm_info_queue)) {
				ip1 = (struct inode *)
						queue_first(&vm_info_queue);
				queue_remove(&vm_info_queue, ip1,
						struct inode *, lru_links);
				ip1->queued = FALSE;
			}
			vm_info_unlock();

			/*
			 *	If we found someone, free up its memory.
			 */
			if (ip1 != (struct inode *)0) {
				mfs_alloc_unlock();
				mfs_memfree(ip1);
				mfs_alloc_lock();
			}
			else {
				mfs_alloc_wanted = TRUE;
				mfs_alloc_blocks++;	/* statistic only */
				unlock_master_if_held();
				condition_wait(&mfs_space_available,
					       &mfs_alloc_lock_data);
				lock_master_if_held();
			}
		}
		else {
			mfs_file_cnt++;
			mfs_alloc_unlock();

			/* not a TEXT file, can cache */
			(void) inode_pager_setup(ip, FALSE, TRUE);

			unlock_master_if_held();
			ret = vm_map(mach_task_self(), &addr, size,
				     0, TRUE, ip->pager, offset,
				     FALSE, VM_PROT_DEFAULT, VM_PROT_ALL,
				     VM_INHERIT_DEFAULT);
			mutex_lock(&ip->pager_wait_lock);
			if (ip->pager_request == MACH_PORT_NULL) {
				ip->waiting = TRUE;
				while(ip->waiting) {
					condition_wait(&ip->pager_wait, 
						       &ip->pager_wait_lock);
				}
			}
			mutex_unlock(&ip->pager_wait_lock);

			lock_master_if_held();
			(void) inode_pager_release(ip->pager);

			if (ret == KERN_SUCCESS) break;

			mfs_alloc_lock();
			mfs_file_cnt--;
		}
		mfs_alloc_unlock();
	} while (ret != KERN_SUCCESS);
	/*
	 *	Fill in variables corresponding to new mapping.
	 */
	ip->va = addr;
	ip->size = size;
	ip->offset = offset;
}

/*
 *	mfs_trunc:
 *
 *	The specified inode is truncated to the specified size.
 */
mfs_trunc(ip, length)
	register struct inode	*ip;
	register int		length;
{
	register vm_size_t	size, rsize;

	mfs_lock(ip);
	if ((!ip->mapped) || (length > ip->i_size) ) {
		mfs_unlock(ip);
		return;
	}

	/*
	 *	Unmap everything past the new end page.
	 *	Also flush any pages that may be left in the object using
	 *	ino_flush (is this necessary?).
	 */
	size = round_page(length);
	rsize = size - ip->offset;	/* size relative to mapped offset */

	unlock_master_if_held();

	if (ip->size > 0 && rsize < ip->size) {
		mfs_map_remove(ip->va + rsize, ip->va + ip->size);
		/* ino_flush(ip, size, ip->size - rsize); */
		ip->size = rsize;		/* mapped size */
		if (ip->size == 0) {
			ip->offset = 0;
			ip->dirty = FALSE;
		}
	}

	if (size < ip->inode_size) 
		ino_flush(ip, size, size - ip->inode_size);

	/*
	 *	If the new length isn't page aligned, zero the extra
	 *	bytes in the last page.
	 */
	if (length != size) {
		bzero(ip->va + ip->offset + length, (size - length));
	}

	ip->inode_size = length;	/* file size */
	mfs_unlock(ip);
	lock_master_if_held();
}

/*
 *	mfs_get:
 *
 *	Get locked access to the specified file.  The start and size describe
 *	the address range that will be accessed in the near future and
 *	serves as a hint of where to map the file if it is not already
 *	mapped.  Upon return, it is guaranteed that there is enough VM
 *	available for remapping operations within that range (each window
 *	no larger than the chunk size).
 */
mfs_get(ip, start, size)
	register struct inode	*ip;
	vm_offset_t		start;
	register vm_size_t	size;
{
	/*
	 *	Remove from LRU list (if its there).
	 */
	vm_info_lock();
	if (ip->queued)
		queue_remove(&vm_info_queue, ip, struct inode *, lru_links);
	ip->queued = FALSE;
	ip->use_count++;	/* to protect requeueing in mfs_put */
	vm_info_unlock();

	/*
	 *	Lock out others using this file.
	 */
	mfs_lock(ip);

	/*
	 *	If the requested size is larger than the size we have
	 *	mapped, be sure we can get enough VM now.  This size
	 *	is bounded by the maximum window size.
	 */
	if (size > mfs_max_window) size = mfs_max_window;
	if (size > ip->size) remap_inode(ip, start, size);
}

/*
 *	mfs_put:
 *
 *	Indicate that locked access is no longer desired of a file.
 */
mfs_put(ip)
	register struct inode	*ip;
{
	/*
	 *	Place back on LRU list if noone else using it.
	 */
	vm_info_lock();
	if (--ip->use_count == 0) {
		queue_enter(&vm_info_queue, ip, struct inode *, lru_links);
		ip->queued = TRUE;
	}
	vm_info_unlock();
	/*
	 *	Let others at file.
	 */
	mfs_unlock(ip);
}

/*
 *	mfs_uncache:
 *
 *	Make sure there are no cached mappings for the specified inode.
 *	Return TRUE if we have potentially called ino_push.
 */
mfs_uncache(ip)
	register struct inode	*ip;
{
	/*
	 *	If the file is mapped but there is noone actively using
	 *	it then remove its mappings.
	 */
	if (!ip->mapped) return;

	mfs_lock(ip);
	if (ip->mapped && ip->map_count == 0) {
		mfs_unlock(ip);
		mfs_memfree(ip);
		return;
	} else {
		mfs_unlock(ip);
		return;
	}
}

mfs_memfree(ip)
	register struct inode * ip;
{
	vm_info_lock();
	if (ip->queued) {
		queue_remove(&vm_info_queue, ip, struct inode *, lru_links);
		ip->queued = FALSE;
	}
	vm_info_unlock();
	mfs_lock(ip);
	if (ip->i_nlink == 0) {
		ino_flush(ip, ip->offset, ip->size);
	} else {
		ino_push(ip, ip->offset, ip->size);
	}
	if ((ip->va != 0) && (ip->size != 0))
		mfs_map_remove(ip->va, ip->va + ip->size);
	ip->size = 0;
	ip->va = 0;
	if (ip->map_count == 0) {	/* cached only */
		ip->mapped = FALSE;
		ip->temporary = FALSE;
	}
	mfs_unlock(ip);
}

#define MFS_CACHE_CLEAR	0
#define MFS_CACHE_SYNC	1

/*
 *	mfs_cache_clear:
 *
 *	Clear the mapped file cache.  Note that the map_count is implicitly
 *	locked by the Unix file system code that calls this routine.
 */
mfs_cache_clear()
{
	mfs_cache_scan(MFS_CACHE_CLEAR);
}

mfs_cache_sync()
{
	/* mfs_cache_scan(MFS_CACHE_SYNC); */
}

/*
 *	mfs_cache_clear:
 *
 *	Clear the mapped file cache.  Note that the map_count is implicitly
 *	locked by the Unix file system code that calls this routine.
 */
mfs_cache_scan(op)
int op;
{
	register struct inode	*ip;
	vm_info_lock();
	ip = (struct inode *) queue_first(&vm_info_queue);
	while (!queue_end(&vm_info_queue, (queue_entry_t) ip)) {
		mfs_lock(ip);
		switch(op) {
			case MFS_CACHE_CLEAR:
				if (ip->map_count == 0) {
					vm_info_unlock();
					mfs_unlock(ip);
					mfs_memfree(ip);
					vm_info_lock();
					/* Must restart after unlock */
					ip = (struct inode *) 
					      queue_first(&vm_info_queue);
					continue;
				}
				break;

			case MFS_CACHE_SYNC:
				if ((ip->dirty) && 
				    (!ip->temporary) &&
				    ((ip->i_flag & ILOCKED) == 0)) {
					vm_info_unlock();
					ino_push(ip, ip->offset, ip->size);
					mfs_unlock(ip);
					vm_info_lock();
					/* Must restart after unlock */
					ip = (struct inode *) 
					      queue_first(&vm_info_queue);
					continue;
				}
				break;
		}
		mfs_unlock(ip);
		ip = (struct inode *) queue_next(&ip->lru_links);
	}
	vm_info_unlock();
}

/*
 *	mfs_map_remove:
 *
 *	Remove specified address range from the mfs map and wake up anyone
 *	waiting for map space.
 */
mfs_map_remove(start, end)
	vm_offset_t	start;
	vm_size_t	end;
{
	(void) vm_deallocate(mach_task_self(), start, end-start);

	mfs_alloc_lock();
	mfs_file_cnt--;
	if (mfs_alloc_wanted) {
		mfs_alloc_wanted = FALSE;
		condition_signal(&mfs_space_available);
	}
	mfs_alloc_unlock();
}


/*
 *	Search for and flush pages in the specified range.  For now, it is
 *	unnecessary to flush to disk since I do that synchronously.
 */
ino_flush(ip, start, size)
	struct inode		*ip;
	register vm_offset_t	start;
	vm_size_t		size;
{
	vm_offset_t	page_start;
	vm_offset_t	page_end;
	vm_size_t	total_size;
	if (ip->pager_request == MACH_PORT_NULL) return;
	if (!ip->dirty) return;

	page_start = trunc_page(start);
	page_end = round_page(start+size);
	total_size = page_end-page_start;

	(void) memory_object_lock_request(ip->pager_request, page_start, 
			total_size, FALSE, TRUE, VM_PROT_ALL, MACH_PORT_NULL);
}

/*
 *	Search for and push (to disk) pages in the specified range.
 */
ino_push(ip, start, size)
	struct inode		*ip;
	vm_offset_t		start;
	vm_size_t		size;
{
	vm_offset_t	page_start;
	vm_offset_t	page_end;
	vm_size_t	total_size;

	if (!ip->dirty) return;
	ip->dirty = FALSE;

	if (ip->pager_request == MACH_PORT_NULL) return;

	unlock_master_if_held();
	mutex_lock(&ip->pager_wait_lock);

	page_start = trunc_page(start);
	page_end = round_page(start+size);
	total_size = page_end-page_start;

	memory_object_lock_request(ip->pager_request, page_start, total_size, 
				   TRUE, mfs_flush, VM_PROT_NONE, ip->pager);

	if (nbc_debug&0x10) 
		printf("Waiting for condition in ino_push: %x\n", ip->i_id);

	ip->waiting = TRUE;
	while (ip->waiting) {
		condition_wait(&ip->pager_wait, &ip->pager_wait_lock);
	}
	mutex_unlock(&ip->pager_wait_lock);

	if (nbc_debug&0x10) 
		printf("ino_push done: %x\n", ip->i_id);

/*
	if (ip->i_size != ip->inode_size) {
		printf("Inode size mismatch: %d, %d\n",
			ip->i_size, ip->inode_size);
	}
*/
	lock_master_if_held();
}

#endif	MAP_UAREA
