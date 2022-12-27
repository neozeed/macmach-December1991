/* 
 * Mach Operating System
 * Copyright (c) 1989 Carnegie-Mellon University
 * All rights reserved.  The CMU software License Agreement specifies
 * the terms and conditions for use and redistribution.
 */
/*
 * HISTORY
 * $Log:	misc.c,v $
 * Revision 2.13  91/09/03  11:13:54  jsb
 * 	Fixed non-MAP_TIME code: use host_get_time instead of obsolete
 * 	kern_timestamp call.
 * 	[91/09/03  10:57:26  jsb]
 * 
 * Revision 2.12  91/08/12  22:38:21  rvb
 * 	Fixed share_unlock to not take unix_master.
 * 	[91/04/08            rpd]
 * 
 * Revision 2.11  90/09/09  14:13:54  rpd
 * 	In init_mapped_time, map the time device with VM_INHERIT_NONE
 * 	instead of VM_INHERIT_SHARE.
 * 	[90/09/09            rpd]
 * 
 * Revision 2.10  90/08/06  15:35:03  rwd
 * 	When using ptrace() there appears to be a way to get garbled
 * 	share locks.  Allow for this by cleaning when noticed.
 * 	[90/07/24            rwd]
 * 	Added share lock panic.
 * 	[90/07/05            rwd]
 * 	Added share_lock and share_unlock routines.
 * 	[90/06/08            rwd]
 * 
 * Revision 2.9  90/06/19  23:16:02  rpd
 * 	Added share_try_lock.
 * 	[90/06/09            rpd]
 * 
 * Revision 2.8  90/06/02  15:28:01  rpd
 * 	Must be on master before tsleep.
 * 	[90/04/04            rwd]
 * 	Removed mips conditionals in init_mapped_time.
 * 	[90/04/24            rpd]
 * 
 * 	Rewrote set_thread_priority using new priority calls.
 * 	[90/04/23            rpd]
 * 	Converted to new IPC.
 * 	Fixed bug in boot: don't try to use root if it isn't mounted.
 * 	[90/03/26  20:19:52  rpd]
 * 
 * Revision 2.7  90/05/29  20:25:16  rwd
 * 	Remove debugging printfs.
 * 	[90/05/10            rwd]
 * 	Must be on master before tsleep.
 * 	[90/04/04            rwd]
 * 
 * Revision 2.6  90/03/14  21:30:58  rwd
 * 	Add share_lock_solid and share_unlock_solid.
 * 	[90/02/16            rwd]
 * 	Include default_pager_object_user here.
 * 	[90/01/22            rwd]
 * 
 * Revision 2.5  89/11/29  15:31:32  af
 * 	No execute permission for mips, if one can avoid it.
 * 	[89/11/26  11:36:05  af]
 * 
 * Revision 2.4  89/09/26  10:30:27  rwd
 * 	Added Debugger
 * 	[89/09/20            rwd]
 * 
 * Revision 2.3  89/09/15  15:29:39  rwd
 * 	Make mapped time dependant on MAP_TIME
 * 	[89/09/13            rwd]
 * 	Change includes
 * 	[89/09/11            rwd]
 * 
 * Revision 2.2  89/08/09  14:46:09  rwd
 * 	Use sizeof(time_value_t) instead of NBPG since device_map and
 * 	vm_map are rounding up for us.
 * 	[89/08/09            rwd]
 * 	Fixed microtime to get time from mtime.
 * 	[89/08/08            rwd]
 * 	Added copyright to file by dbg.  Changed get_time to use mapped
 * 	time and be macro in sys/kernel.h.
 * 	[89/08/03            rwd]
 * 
 */
/*
 * Miscellaneous routines.
 */
#include <map_time.h>
#include <map_uarea.h>

#include <sys/param.h>
#include <sys/types.h>
#include <sys/buf.h>
#include <sys/mount.h>
#include <sys/fs.h>
#include <sys/time.h>
#include <sys/reboot.h>

#include <uxkern/device.h>
#include <mach/port.h>
#include <mach/vm_prot.h>
#include <mach/vm_inherit.h>

#include <uxkern/device_utils.h>

extern mach_port_t	privileged_host_port;
extern mach_port_t	default_processor_set;

/*
 * Missing system calls
 */
profil()
{
}

swapon()
{
}

resuba()
{
}

/*
 * New kernel interfaces.
 */

#if MAP_TIME
time_value_t *mtime = NULL;

init_mapped_time()
{
	kern_return_t rc;
	mach_port_t device_port, pager = MACH_PORT_NULL;

	rc = device_open(device_server_port,0,"time",&device_port);
	if (rc != D_SUCCESS) panic("unable to open device time");

	rc = device_map(device_port, VM_PROT_READ,
			0, sizeof(time_value_t), &pager, 0);
	if (rc != D_SUCCESS) panic("unable to map device time");
	if (pager == MACH_PORT_NULL) panic("unable to map device time");
	
	rc = vm_map(mach_task_self(), &mtime, sizeof(time_value_t), 0, TRUE,
		    pager, 0, 0, VM_PROT_READ, 
		    VM_PROT_READ, VM_INHERIT_NONE);
	if (rc != D_SUCCESS) panic("unable to vm_map device time");

	rc = mach_port_deallocate(mach_task_self(), pager);
	if (rc != KERN_SUCCESS) panic("unable to deallocate pager");
}

microtime(tvp)
	struct timeval *tvp;
{
	*tvp = *(struct timeval *)mtime;
}

#else MAP_TIME
init_mapped_time(){}

get_time(tvp)
	struct timeval *tvp;
{
	time_value_t time_value;
	kern_return_t rc;

	rc = host_get_time(privileged_host_port, &time_value);
	if (rc != KERN_SUCCESS) panic("get_time: host_get_time");
	tvp->tv_sec = time_value.seconds;
	tvp->tv_usec = time_value.microseconds;
}

microtime(tvp)
	struct timeval *tvp;
{
	get_time(tvp);
}

#endif MAP_TIME

set_time(tvp)
	struct timeval *tvp;
{
	time_value_t	time_value;

	time_value.seconds = tvp->tv_sec;
	time_value.microseconds = tvp->tv_usec;

	(void) host_set_time(privileged_host_port, time_value);
}

int	waittime = -1;

Debugger()
{
	boot(0, RB_NOSYNC | RB_DEBUGGER);
}

boot(paniced, flags)
	int	paniced;
	int	flags;
{
	if (((flags & RB_NOSYNC) == 0) &&
	    (waittime < 0) &&
	    (bfreelist[0].b_forw != 0) &&
	    (mount[0].m_bufp != 0)) {
	    /*
	     * Force an accurate time into the root file system superblock.
	     */
	    mount[0].m_bufp->b_un.b_fs->fs_fmod = 1;

	    waittime = 0;
	    (void) splnet();
	    printf("syncing disks... ");

	    update();

	    {
		register struct buf *bp;
		int	iter, nbusy, obusy;

		obusy = 0;

		for (iter = 0; iter < 20; iter++) {
		    nbusy = 0;
		    for (bp = &buf[nbuf]; --bp >= buf; )
			if ((bp->b_flags & (B_BUSY | B_INVAL)) == B_BUSY)
			    nbusy++;
		    if (nbusy == 0)
			break;
		    printf("%d ", nbusy);

		    if (nbusy != obusy)
			iter = 0;
		    obusy = nbusy;

		    DELAY(40000 * iter);
		}
	    }
	    printf("done\n");

	    /*
	     * resettodr()?
	     */
	}

	(void) host_reboot(privileged_host_port, flags);
}

thread_read_times(thread, utv, stv)
	thread_t	thread;
	time_value_t	*utv;
	time_value_t	*stv;
{
	struct thread_basic_info	bi;
	unsigned int			bi_count;

	bi_count = THREAD_BASIC_INFO_COUNT;
	(void) thread_info(thread,
			   THREAD_BASIC_INFO,
			   (thread_info_t)&bi,
			   &bi_count);

	*utv = bi.user_time;
	*stv = bi.system_time;
}

/*
 *	Priorities run from 0 (high) to 31 (low).
 *	The user base priority is 12.
 *	priority = 12 + nice / 2.
 */

set_thread_priority(thread, pri)
	thread_t	thread;
	int		pri;
{
	(void) thread_max_priority(thread, default_processor_set, pri);
	(void) thread_priority(thread, pri, FALSE);
}

#if MAP_UAREA
#include <sys/user.h>
#include <sys/parallel.h>
#define BACKOFF_SECS 5
#define SHARED_PRIORITY PSPECL

extern int hz;

boolean_t share_try_lock(p, lock)
	register struct proc *p;
	register struct shared_lock *lock;
{
	if (spin_try_lock(&lock->lock)) {
		if (p->p_shared_rw->us_inuse)
			lock->who = (int)p | KERNEL_USER;
		u.uu_share_lock_count++;
		return TRUE;
	} else
		return FALSE;
}

void share_lock(x, p)
register struct shared_lock *x;
register struct proc *p;
{
    if (p->p_shared_rw->us_inuse) {
      while (!spin_try_lock(&(x)->lock) && !share_lock_solid((x)));
      (x)->who = (int)(p) | KERNEL_USER;
    } else {
      spin_lock(&(x)->lock);
    }
    u.uu_share_lock_count++;
}

void share_unlock(x, p)
register struct shared_lock *x;
register struct proc *p;
{
    if (p->p_shared_rw->us_inuse) {
      (x)->who = 0;
      spin_unlock(&(x)->lock);
      if ((x)->need_wakeup)
	wakeup(x);
    } else {
      spin_unlock(&(x)->lock);
    }
    if (u.uu_share_lock_count-- == 0) {
	panic("share_unlock < 0");
	u.uu_share_lock_count = 0;
    }
}

int share_lock_solid(x)
	register struct shared_lock *x;
{
	unix_master();
	x->need_wakeup++;
	if (spin_try_lock(&x->lock)) {
		x->need_wakeup--;
		unix_release();
		return 1;
	}
/*	printf("[%x]Share_lock_solid %x\n",(int)x, x->who);*/
	if (tsleep(x, SHARED_PRIORITY, BACKOFF_SECS * hz) == TS_TIME) {
		if ((x->who & ~KERNEL_USER >= (int)proc) &&
		    (x->who & ~KERNEL_USER <= (int)procNPROC)) {
		    printf("[%x]Unable to get lock from %x\n",(int)x, x->who);
		    panic("share_lock_solid");
		} else {
		    printf("[%x]Taking scribbled share_lock\n",(int)x);
		    share_lock_init(x);
		    x->need_wakeup++;
		}
	}
	(x)->need_wakeup--; /* This protected by the master lock */
/*	printf("[%x]Share_lock_solid\n",(int)x);*/
	unix_release();
	return 0;
}

#endif MAP_UAREA

#include <mach/default_pager_object_user.c>
