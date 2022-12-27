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
 * $Log:	emul_mapped.c,v $
 * Revision 2.11  91/08/12  22:36:53  rvb
 * 	Fixed e_maprw to use EPRINT instead of e_emulator_error.
 * 	[91/08/06            rpd]
 * 
 * Revision 2.10  91/07/30  15:35:20  rvb
 * 	From rwd: change default of case share_unlock line to
 * 	rel_it(argp[0],interrupt)
 * 	[91/07/29            rvb]
 * 
 * 	From rfr: gets rid of close piggybacking.
 * 	[91/07/29            rvb]
 * 
 * Revision 2.9  91/03/20  15:01:34  dbg
 * 	Fix for ANSI C.
 * 	[91/02/22            dbg]
 * 
 * Revision 2.8  90/11/05  15:31:37  rpd
 * 	Added spin_lock_t.
 * 	[90/10/31            rwd]
 * 
 * Revision 2.7  90/11/05  15:28:15  rpd
 * 	Fixed vm_region call in emul_mapped_init.
 * 	[90/11/02            rpd]
 * 
 * Revision 2.6  90/08/06  15:30:21  rwd
 * 	Change share_lock to set who field to be proc pointer instead of
 * 	worthless local port name.  Up threshold on share_lock error
 * 	message.
 * 	[90/07/19            rwd]
 * 	Added share_lock and share_unlock routines.
 * 	[90/06/08            rwd]
 * 	Changed debugging e_emulator_error to use EPRINT
 * 	[90/06/05            rwd]
 * 
 * Revision 2.5  90/06/19  23:06:44  rpd
 * 	Minor changes for greater portability.
 * 	[90/06/10            rpd]
 * 
 * Revision 2.4  90/06/02  15:20:35  rpd
 * 	Converted new functions to new IPC.
 * 	[90/06/01            rpd]
 * 
 * 	Fixed e_checksignals to take STRC/SRMT into account.
 * 	[90/05/13            rpd]
 * 
 * 	Removed us_sigcheck, e_check_server_signals.
 * 	Fixed e_checksignals to look at us_sig.
 * 	[90/05/10            rpd]
 * 	Updated to new IPC.
 * 	[90/05/04  16:58:37  rpd]
 * 
 * Revision 2.3  90/05/29  20:22:29  rwd
 * 	Fix multi-threaded problem in get_it().
 * 	[90/05/21            rwd]
 * 	Fix potential multi-threaded program bug in e_readwrite.
 * 	[90/05/16            rwd]
 * 	Added flag to turn off mapped operations.
 * 	[90/05/02            rwd]
 * 	Change close to set flag for close and not send message when
 * 	possible.
 * 	[90/04/23            rwd]
 * 	Simplify locking a bit.
 * 	[90/04/19            rwd]
 * 	Allow for > CHUNK_SIZE read/writes.
 * 	[90/04/13            rwd]
 * 	Add new MAP_FILE code.
 * 	[90/03/26            rwd]
 * 
 * Revision 2.2  90/03/14  21:23:04  rwd
 * 	Sigblock should not allow cantmask to be set.
 * 	[90/03/02            rwd]
 * 	Changed shared locks to use share_lock macros.  Added
 * 	share_unlock_solid.
 * 	[90/02/16            rwd]
 * 	Created
 * 	[90/01/31            rwd]
 * 
 */
/*
 * Routines which use mapped area of uarea instead of sending message
 * to server
 */

#ifdef MAP_UAREA
#include <uxkern/import_mach.h>
#include <uxkern/bsd_msg.h>
#include <mach/machine/vm_param.h>
#include <machine/machparam.h>
#include <sys/types.h>
#include <sys/ushared.h>
#include <sys/errno.h>
#include <sys/proc.h>
#include <machine/vmparam.h>
#ifdef mac2 /* fault jmp_buf */
#include <setjmp.h>
#include "emul_stack.h"
#endif 

int enable_sharing = 1;
int shared_enabled = 0;
struct ushared_ro *shared_base_ro;
struct ushared_rw *shared_base_rw;
char *shared_readwrite;
int readwrite_inuse = 0;
spin_lock_t readwrite_lock = 0;

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
 * Same as bsd/mach_signal.c
 */

extern  mach_port_t	our_bsd_server_port;

emul_mapped_init()
{
	kern_return_t	result;
	vm_address_t	address;
	vm_size_t	size;
	vm_prot_t	prot;
	vm_prot_t	max_prot;
	vm_inherit_t	inherit;
	boolean_t	shared;
	mach_port_t	object_name;
	vm_offset_t	offset;
	vm_statistics_data_t	vm_stat;

	if (!enable_sharing)
		return;

	shared_base_ro = (struct ushared_ro *)(EMULATOR_END - vm_page_size);
	shared_base_rw = (struct ushared_rw *)(EMULATOR_END - 2*vm_page_size);
	shared_readwrite = (char *) (EMULATOR_END - 3*vm_page_size);

	address = (vm_address_t) shared_base_ro;
	result = vm_region(mach_task_self(), &address, &size,
			   &prot, &max_prot, &inherit, &shared,
			   &object_name, &offset);
	if (result != KERN_SUCCESS) {
		e_emulator_error("vm_region ret = %x",result);
		return;
	}
	if (!(prot & VM_PROT_READ)) {
		e_emulator_error("shared region not readable");
		return;
	}
	if (shared_base_ro->us_version != USHARED_VERSION) {
		e_emulator_error("shared region mismatch %x/%x",
			shared_base_ro->us_version, USHARED_VERSION);
		return;
	}

	shared_enabled = 1;
	shared_base_rw->us_inuse = 1;
	shared_base_rw->us_debug = 0;
	readwrite_inuse = 0;
	spin_lock_init(&readwrite_lock);
	if (shared_base_ro->us_mach_nbc)
		shared_base_rw->us_map_files = 1;
}

#define E_DECLARE(routine) \
int routine(serv_port, interrupt, syscode, argp, rvalp) \
	mach_port_t	serv_port; \
	boolean_t	*interrupt; \
	int	syscode; \
	int	* argp; \
	int	* rvalp; \
{ \

#define E_IF \
    if (shared_enabled) {

#define E_END(status) \
end:\
	e_checksignals(interrupt); \
	return (status); \
    } else { \
server: \
	return emul_generic(serv_port, interrupt, syscode, argp, rvalp); \
    } \
} \


E_DECLARE(e_obreak)
	vm_offset_t	old, new;
	kern_return_t	ret;
E_IF
	new = round_page(argp[0]);
	if ((int)(new - (vm_offset_t)shared_base_ro->us_data_start) >
	    shared_base_ro->us_rlimit[RLIMIT_DATA].rlim_cur) {
		return ENOMEM;
	}
	old = round_page(shared_base_ro->us_data_start + 
			 ctob(shared_base_rw->us_dsize));
	if (new > old) {
		ret = vm_allocate(mach_task_self(), &old, new - old, FALSE);
		if (ret != KERN_SUCCESS) {
			e_emulator_error("obreak : vm_allocate %x",ret);
			goto server;
		} else
			shared_base_rw->us_dsize += btoc(new - old);
	}
E_END(ESUCCESS)


E_DECLARE(e_getdtablesize)
E_IF
	rvalp[0] = shared_base_ro->us_nofile;
E_END(ESUCCESS)


E_DECLARE(e_getuid)
E_IF
	rvalp[0] = shared_base_ro->us_ruid;
	rvalp[1] = shared_base_ro->us_uid;
E_END(ESUCCESS)


E_DECLARE(e_getpid)
E_IF
	rvalp[0] = shared_base_ro->us_pid;
	rvalp[1] = shared_base_ro->us_ppid;
E_END(ESUCCESS)


E_DECLARE(e_getgid)
E_IF
	rvalp[0] = shared_base_ro->us_rgid;
	rvalp[1] = shared_base_ro->us_gid;
E_END(ESUCCESS)



E_DECLARE(e_getgroups)
	register gid_t *gp;
	int *groups = (int *)argp[1];
	int size,i;
E_IF
	for (gp = &shared_base_ro->us_groups[NGROUPS];
	     gp > shared_base_ro->us_groups; gp--)
		if (gp[-1] != NOGROUP)
			break;
	size = gp - shared_base_ro->us_groups;
	if (argp[0] < size) return EINVAL;
	for (i=0; i < size; i++)
		groups[i]=shared_base_ro->us_groups[i];
	rvalp[0] = size;
E_END(ESUCCESS)


E_DECLARE(e_getrlimit)
	struct rlimit *rlp = (struct rlimit *)argp[1];
E_IF
	if (argp[0] >= RLIM_NLIMITS) return EINVAL;
	*rlp = shared_base_ro->us_rlimit[argp[0]];
E_END(ESUCCESS)


E_DECLARE(e_umask)
E_IF
	rvalp[0] = shared_base_rw->us_cmask;
	shared_base_rw->us_cmask = argp[0];
E_END(ESUCCESS)


E_DECLARE(e_sigblock)
E_IF
	share_lock(&shared_base_rw->us_siglock);
	rvalp[0] = shared_base_rw->us_sigmask;
	shared_base_rw->us_sigmask |= argp[0] &~ cantmask;
	share_unlock(&shared_base_rw->us_siglock);
E_END(ESUCCESS)

#ifdef	MAP_FILE

E_DECLARE(e_lseek)
	register struct file_info *fd;
E_IF
	if (argp[0] < 0 || argp[0] > shared_base_ro->us_nofile)
	    return (EBADF);
	fd = &shared_base_rw->us_file_info[argp[0]];

	share_lock(&fd->lock);
	if (fd->mapped && fd->open) {
	    get_it(argp[0], interrupt);
	    switch (argp[2]) {

	    case L_INCR:
		fd->offset += argp[1];
		break;

	    case L_XTND:
		fd->offset = argp[1] + fd->map_info.inode_size;
		break;

	    case L_SET:
		fd->offset = argp[1];
		break;

	    default:
		share_unlock(&fd->lock);
		rel_it(argp[0], interrupt);
		return (EINVAL);
	    }
	    rvalp[0] = fd->offset;
	    rel_it(argp[0], interrupt);
	} else {
	    share_unlock(&fd->lock);
	    goto server;
	}
E_END(ESUCCESS)

#endif	MAP_FILE

E_DECLARE(e_read)
	int result;
E_IF
#ifdef	MAP_FILE
	if (!e_maprw(serv_port, interrupt, argp[0], argp[1], argp[2],
			     rvalp, 0, &result)) {
#endif	MAP_FILE
	if (argp[2] > vm_page_size) goto server;
	spin_lock(&readwrite_lock);
	if (readwrite_inuse) {
		spin_unlock(&readwrite_lock);
		goto server;
	}
	readwrite_inuse = 1;
	spin_unlock(&readwrite_lock);
	result = e_readwrite(serv_port, interrupt, argp[0], argp[1], argp[2],
			     rvalp, 0, 1);
#ifdef	MAP_FILE
	}
#endif	MAP_FILE
E_END(result)

E_DECLARE(e_sigsetmask)

E_IF
	share_lock(&shared_base_rw->us_siglock);

	rvalp[0] = shared_base_rw->us_sigmask;
	shared_base_rw->us_sigmask = argp[0] &~ cantmask;

	share_unlock(&shared_base_rw->us_siglock);

E_END(ESUCCESS)

share_lock(x)
register struct shared_lock *x;
{
	int i=0;
	while(!spin_try_lock(&(x)->lock)) {
	    if (++i % 1024 == 0)
		e_emulator_error("share_lock failure %d", i);
	    swtch_pri(0);
	}
	(x)->who = shared_base_ro->us_proc_pointer & ~KERNEL_USER;
}

share_unlock(x)
register struct shared_lock *x;
{
    (x)->who = 0;
    spin_unlock(&(x)->lock);
    if ((x)->need_wakeup)
	bsd_share_wakeup(our_bsd_server_port, (int)x - (int)shared_base_rw);
}

e_shared_sigreturn(proc_port, interrupt, old_on_stack, old_sigmask)
	mach_port_t	proc_port;
	boolean_t	*interrupt;
	int		old_on_stack;
	int		old_sigmask;
{
	share_lock(&shared_base_rw->us_lock);
	shared_base_rw->us_sigstack.ss_onstack = old_on_stack & 01;
	share_unlock(&shared_base_rw->us_lock);

	share_lock(&shared_base_rw->us_siglock);
	shared_base_rw->us_sigmask = old_sigmask & ~cantmask;
	share_unlock(&shared_base_rw->us_siglock);

	e_checksignals(interrupt);
}

e_checksignals(interrupt)
	boolean_t	*interrupt;
{
	if (shared_enabled) {
		/*
		 *	This is really just a hint; so the lack
		 *	of locking isn't important.
		 */

		if (shared_base_ro->us_cursig ||
		    (shared_base_rw->us_sig &~
		     (shared_base_rw->us_sigmask |
		      ((shared_base_ro->us_flag&STRC) ?
		       0 : shared_base_rw->us_sigignore) |
		      ((shared_base_ro->us_flag&SRMT) ?
		       stopsigmask : 0))))
			*interrupt = TRUE;
	}
}

E_DECLARE(e_close)
	struct file_info *fd;
E_IF
	if (argp[0] < 0 || argp[0] > shared_base_ro->us_lastfile)
	    return (EBADF);

#ifdef	PIGGYBACK
	fd = &shared_base_rw->us_file_info[argp[0]];

	if (shared_base_rw->us_closefile != -1)
	    goto server;
	share_lock(&fd->lock);
	if (fd->mapped && fd->open) {
	    shared_base_rw->us_closefile = argp[0];
	    fd->open = FALSE;
	    share_unlock(&fd->lock);
	    return (ESUCCESS);
	}
	share_unlock(&fd->lock);
#endif	PIGGYBACK
	goto server;
E_END(ESUCCESS)

int
e_readwrite(serv_port, interrupt, fileno, data, count, rval, which, copy)
	mach_port_t	serv_port;
	boolean_t	*interrupt;
	int		fileno;
	char		*data;
	unsigned int	count;
	int		*rval;
	int		which;
	int		copy;
{
	extern int bsd_readwrite();
	int result;

	rval[0] = -1;
	if (fileno < 0 || fileno > shared_base_ro->us_lastfile)
	    return (EBADF);
	if (which && copy)
	    bcopy(data, shared_readwrite, count);
	result = bsd_readwrite(serv_port, interrupt, which, fileno,
			       count, rval);
	if (!which && rval[0] > 0 && copy)
	    bcopy(shared_readwrite, data, rval[0]);
	if (copy) {
	    spin_lock(&readwrite_lock);
	    readwrite_inuse = 0;
	    spin_unlock(&readwrite_lock);
	}
	return (result);
}

#ifdef	MAP_FILE

int
e_maprw(serv_port, interrupt, fileno, data, count, rval, which, result)
	mach_port_t	serv_port;
	boolean_t	*interrupt;
	int		fileno;
	char		*data;
	unsigned int	count;
	int		*rval;
	int		which;
	int		*result;
{
	register struct file_info *fd;
	register struct map_info *mi;
	char *from,*to;	
	char *wdata;
	int size,tsize;

	if (fileno < 0 || fileno > shared_base_ro->us_lastfile) {
	    EPRINT(("e_maprw:%d badfile",which));
	    *result = EBADF;
	    return TRUE;
	}
	fd = &shared_base_rw->us_file_info[fileno];
	mi = &fd->map_info;

	share_lock(&fd->lock);
	if (fd->mapped && fd->open) {
	  if(which?fd->write:fd->read) {
	    get_it(fileno, interrupt);
#ifdef mac2 /* fault jmp_buf */
	    if (_setjmp(emul_stack_fault())) {
	      rel_it(fileno, interrupt);
	      *result = EFAULT;
	      return TRUE;
	    }
#endif
	    wdata = data;
	    tsize = size = count;
	    while (tsize > 0 && size > 0) {
		size = tsize;
		if (which & fd->append)
		    fd->offset = mi->inode_size;
		if (fd->offset < mi->offset ||
		    fd->offset >= mi->offset + mi->size)
		    bsd_maprw_remap(serv_port, interrupt, fileno, fd->offset, 
				    size);
		if (fd->offset + size > mi->offset + mi->size)
		    size = mi->offset + mi->size - fd->offset;
		from =(char * )( mi->address + fd->offset - mi->offset);
		if (which) {
		    if (fd->offset + size > mi->inode_size)
			mi->inode_size = fd->offset + size;
		    fd->dirty = TRUE;
		    to = from;
		    from = wdata;
		} else {
		    if (fd->offset + size > mi->inode_size)
			size = mi->inode_size - fd->offset;
		    if (size <= 0) size = 0;
		    to = wdata;
		}
		if (!fd->inuse) goto done;
		if (size > 0) bcopy(from, to, size);
		fd->offset += size;
		tsize-=size;
		wdata += size;
	    }
	    rel_it(fileno, interrupt);
done:
	    rval[0] = count - tsize;
	    *result = ESUCCESS;
	    return TRUE;
	  }
	  share_unlock(&fd->lock);
	  *result = EBADF;
	  EPRINT(("e_maprw:%d (r/w) %d/%d",which,fd->read,fd->write));
	  return TRUE;
	}
	share_unlock(&fd->lock);
	return FALSE;
}

/* called with share_lock(fd->lock) held */
get_it(fileno, interrupt)
	int fileno;
	int *interrupt;
{
	register struct file_info *fd = &shared_base_rw->us_file_info[fileno];

	if (!fd->control || fd->inuse ) {
	    share_unlock(&fd->lock);
	    bsd_maprw_request_it(our_bsd_server_port, interrupt, fileno);
	    return;
	}
	fd->inuse = TRUE;
	share_unlock(&fd->lock);
}

rel_it(fileno, interrupt)
	int fileno;
	int *interrupt;
{
	register struct file_info *fd = &shared_base_rw->us_file_info[fileno];

	share_lock(&fd->lock);
	if (fd->wants) {
		share_unlock(&fd->lock);
		bsd_maprw_release_it(our_bsd_server_port, interrupt, fileno);
	} else {
		fd->inuse = FALSE;
		share_unlock(&fd->lock);
	}
}

#endif	MAP_FILE
#endif	MAP_UAREA
