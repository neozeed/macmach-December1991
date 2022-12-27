/* 
 * Mach Operating System
 * Copyright (c) 1990 Carnegie-Mellon University
 * Copyright (c) 1989 Carnegie-Mellon University
 * Copyright (c) 1988 Carnegie-Mellon University
 * Copyright (c) 1987 Carnegie-Mellon University
 * All rights reserved.  The CMU software License Agreement specifies
 * the terms and conditions for use and redistribution.
 */
/*
 * HISTORY
 * $Log:	bsd_machdep.c,v $
 * Revision 2.9  90/09/09  22:33:04  rpd
 * 	Fixed 407 & 410 images in machine_getxfile().
 * 	[90/09/02            af]
 * 
 * Revision 2.8  90/08/06  15:35:39  rwd
 * 	Fix references to share_lock.
 * 	[90/06/08            rwd]
 * 
 * Revision 2.7  90/06/19  23:16:31  rpd
 * 	Temporarily turned off partial-page vm_write use.
 * 	[90/06/04            rpd]
 * 
 * Revision 2.6  90/06/02  15:29:11  rpd
 * 	Removed u_text_start.
 * 	[90/06/02            rpd]
 * 
 * 	Fixed sendsig to not deallocate if the vm_read fails.
 * 	Also fixed sendsig to use proc_exit if the stack is bad;
 * 	psignal doesn't work because the process doesn't notice the SIGILL.
 * 	[90/03/27            rpd]
 * 	Converted to new IPC.
 * 	[90/03/26  20:29:37  rpd]
 * 
 * Revision 2.5  90/05/29  20:25:29  rwd
 * 	Change EMULATOR_END to remove gap between it and SIGCODE.
 * 	Don't delete shared file region on exec.
 * 	[90/04/16            dbg]
 * 
 * 	Use new vm_write (which does not require page-alignment) to zero
 * 	the part of the BSS that overlaps the area mapped from the text
 * 	file.
 * 	[90/04/11            dbg]
 * 
 * Revision 2.4  90/05/21  14:03:00  dbg
 * 	Use new vm_write (which does not require page-alignment) to zero
 * 	the part of the BSS that overlaps the area mapped from the text
 * 	file.
 * 	[90/04/11            dbg]
 * 
 * Revision 2.3  90/03/14  21:31:22  rwd
 * 	Change shared locks to use share_lock macros.
 * 	[90/02/16            rwd]
 * 	Fix up refernces to fields in shared uarea.
 * 	[90/01/23            rwd]
 * 
 * Revision 2.2  90/01/23  00:05:41  af
 * 	ptrace_set_u_word takes an extra param now.
 * 	[90/01/20  23:31:30  af]
 * 
 * Revision 2.1  89/08/04  15:10:24  rwd
 * Created.
 * 
 *  4-May-89  David Golub (dbg) at Carnegie-Mellon University
 *	Changes for separate emulator stack.  Emulator code now handles
 *	sigreturn, sigcleanup, setting user registers on exec.
 *
 * 10-Apr-89  David Golub (dbg) at Carnegie-Mellon University
 *	Move most of sigreturn/osigcleanup to user side of interface.
 *
 *  7-Apr-89  David Golub (dbg) at Carnegie-Mellon University
 *	Pass new thread state to thread_dup.
 *
 *  3-Apr-89  David Golub (dbg) at Carnegie-Mellon University
 *	Clear user's fp on exec to keep traceback (in uVax emulator)
 *	happy.
 *
 *  9-Mar-89  David Golub (dbg) at Carnegie-Mellon University
 *	In getxfile: switch off master before vm_map call - it will call
 *	inode pager.
 *
 *  3-Jan-89  David Golub (dbg) at Carnegie-Mellon University
 *	Update to 3 Jan 89 and out-of-kernel use.
 *
 * 20-Jul-88  David Golub (dbg) at Carnegie-Mellon University
 *	Created.
 *
 */

#include <cmucs.h>
#include <map_uarea.h>
#include <mach_nbc.h>

/*
 *	Machine-dependent routines evicted from rest of BSD
 *	files.
 */

#include <mac2/reg.h>

#include <mac2/vmparam.h>

#include <sys/proc.h>
#include <sys/user.h>
#include <sys/wait.h>
#include <sys/inode.h>
#include <sys/uio.h>
#include <sys/exec.h>
#include <sys/systm.h>		/* boothowto */
#include <sys/parallel.h>
#include <sys/syscall.h>

#include <uxkern/import_mach.h>

#define	LOADER_PAGE_SIZE	(MAC2_PGBYTES)
#define loader_round_page(x)	((vm_offset_t)((((vm_offset_t)(x)) \
						+ LOADER_PAGE_SIZE - 1) \
					       & ~(LOADER_PAGE_SIZE-1)))
#define loader_trunc_page(x)	((vm_offset_t)(((vm_offset_t)(x)) \
					       & ~(LOADER_PAGE_SIZE-1)))

/*
 *	A corrupted fileheader can cause getxfile to decide to bail
 *	out without setting up the address space correctly.  It is
 *	essential in this case that control never get back to the
 *	user.  The following error code is used by getxfile to tell
 *	execve that the process must be killed.
 *
 *	XXX - definition MUST be the same as in bsd/kern_exec.c XXX
 */

#define	EGETXFILE	126

int
machine_getxfile(ip, ep, stack_size, r_text_size, r_data_size,
		 normal_load)
struct inode	*ip;
struct exec	*ep;
vm_offset_t	stack_size;
vm_offset_t	*r_text_size;	/* out */
vm_offset_t	*r_data_size;	/* out */
boolean_t	normal_load;
{
    vm_offset_t		addr;
    vm_size_t		size;
    task_t		my_task = u.u_procp->p_task;
    vm_size_t		text_size, data_size, data_bss_size;
    vm_offset_t		text_start, data_start;
    boolean_t		on_master = TRUE;
    register int	error;
    
    if (u.u_master_lock == 0)
	panic("getxfile: no master lock");
    
#ifdef notdef
    printf("magic %o text %x data %x bss %x\n", ep->a_magic, ep->a_text, ep->a_data, ep->a_bss);
#endif

    /*
     *	Allocate low-memory stuff: text, data, bss, space for brk
     *	calls.
     *	Read text&data into lowest part, then make text read-only.
     */
    
    if (normal_load)
	text_start = USRTEXT;
    else
	text_start = EMULATOR_BASE;
    
    text_size = loader_round_page(ep->a_text);
    if (ep->a_magic != 0413 && ep->a_text == 0)
	data_start = text_start;
    else
	data_start = ((text_start+text_size+SGOFSET)&~SGOFSET);
    data_size = loader_round_page(ep->a_data);
    data_bss_size = loader_round_page(ep->a_data + ep->a_bss);

#ifdef notdef
    printf("text ( %x %x ) data ( %x %x ) data_bss %x\n",
	   text_start, text_size,
	   data_start, data_size,
	   data_bss_size);
#endif

#ifdef notdef
    /*
     * Check that we can fit the file in memory.
     * Check data and bss sizes separately, since they
     * may overflow when added together.
     */
    if (ep->a_data > u.u_rlimit[RLIMIT_DATA].rlim_cur ||
	ep->a_bss  > u.u_rlimit[RLIMIT_DATA].rlim_cur ||
	data_size  > u.u_rlimit[RLIMIT_DATA].rlim_cur ||
	size < 0	/* wraparound */ ||
	size > (VM_MAX_ADDRESS - VM_MIN_ADDRESS) - stack_size) {
	return (ENOMEM);
	}
#endif

    /*
     * Deallocate the entire address space for the task,
     * except for the area holding the emulator and sigtramp.
     */
    {
#if	MAP_UAREA && MACH_NBC
	(void) vm_deallocate(my_task,
			     VM_MIN_ADDRESS,
			     (vm_size_t)((vm_offset_t)MAP_FILE_BASE
					 - VM_MIN_ADDRESS));
#else	MAP_UAREA && MACH_NBC
	(void) vm_deallocate(my_task,
			     VM_MIN_ADDRESS,
			     (vm_size_t)((vm_offset_t)EMULATOR_BASE
					 - VM_MIN_ADDRESS));
#endif	MAP_UAREA && MACH_NBC
    }

    /*
     * Allocate page 0 with no access to
     * catch NIL poointers.
     */
    addr = 0;
    size = MAC2_PGBYTES;

    if (normal_load
	&& vm_map(my_task, &addr, size, 0, FALSE,
		  MEMORY_OBJECT_NULL, 0,
		  FALSE,
		  VM_PROT_NONE,
		  VM_PROT_NONE,
		  VM_INHERIT_COPY) != KERN_SUCCESS) {
	uprintf("Cannot allocate zero page.\n");
	goto suicide;
    }

    /*
     *	Remember data starting point
     */
    u.u_data_start = (caddr_t) data_start;
    
    error = 0;
    
    if (ep->a_magic != 0413) {	/* not demand paged */
	addr = data_start;
	size = data_bss_size;
	
	if (vm_allocate(my_task, &addr, size, FALSE) != KERN_SUCCESS) {
	    uprintf("Cannot find space for data image.\n");
	    goto suicide;
	}
	
	size = data_size;
	if (vm_allocate(mach_task_self(), &addr, size, TRUE))
	    goto suicide;
	
	error = rdwri(UIO_READ, ip,
		      (caddr_t)addr,
		      ep->a_data,
		      (off_t)(sizeof(struct exec) + ep->a_text),
		      0, (int*)0);
	
	if (error == 0)
	    error = vm_write(my_task, data_start, addr,
			     round_page(ep->a_data));
	
	(void) vm_deallocate(mach_task_self(), addr, size);
	
	if (error == 0 && text_size > 0) {
	    addr = text_start;
	    size = text_size;
	    
	    if (vm_allocate(my_task, &addr, size, FALSE) != KERN_SUCCESS) {
		uprintf("Cannot find space for text image.\n");
		goto suicide;
	    }
	    
	    if (vm_allocate(mach_task_self(), &addr, size, TRUE))
		goto suicide;
	    
	    error = rdwri(UIO_READ, ip,
			  (caddr_t)addr,
			  ep->a_text,
			  (off_t)sizeof(struct exec),
			  0, (int*)0);
	    
	    if (error == 0)
		error = vm_write(my_task, text_start, addr,
				 round_page(ep->a_text));
	    
	    if (error == 0)
		(void) vm_protect(my_task,
				  text_start,
				  text_size,
				  FALSE,
				  VM_PROT_READ|VM_PROT_EXECUTE);
	    
	    (void) vm_deallocate(mach_task_self(), addr, size);
	}
    }
    else {
	memory_object_t	pager;
	
	/*
	 *	Allocate a region backed by the exec'ed inode.
	 */
	
	pager = (memory_object_t)inode_pager_setup(ip, TRUE, TRUE);
	iunlock(ip);
	/*
	 * Release the master lock, because vm_map will call the
	 * inode pager, which may block on it.
	 */
	unix_release();
	on_master = FALSE;
	
	if (text_size > 0) {
	    addr = text_start;
	    size = text_size;
	    
	    if (vm_map(my_task, &addr, size, 0, FALSE,
		       pager, (vm_offset_t)0,
		       FALSE,
		       VM_PROT_READ|VM_PROT_EXECUTE,
		       VM_PROT_READ|VM_PROT_EXECUTE,
		       VM_INHERIT_COPY)
		!= KERN_SUCCESS) {
		uprintf("Cannot map text into user address space.\n");
		inode_pager_release(pager);
		goto suicide;
	    }
	}
	
	addr = data_start;
	size = data_size;
	
	if (vm_map(my_task, &addr, size, 0, FALSE,
		   pager, (vm_offset_t)text_size,
		   TRUE,
		   VM_PROT_ALL,
		   VM_PROT_ALL,
		   VM_INHERIT_COPY)
	    != KERN_SUCCESS) {
	    uprintf("Cannot map data into user address space.\n");
	    inode_pager_release(pager);
	    goto suicide;
	}
	inode_pager_release(pager);
	
	addr += size;
	if ((data_bss_size > size)
	    && vm_allocate(my_task,
			   &addr, round_page(data_bss_size - size),
			   FALSE) != KERN_SUCCESS) {
	    uprintf("Cannot allocate BSS in user address space.\n");
	    goto suicide;
	}
	
	if (data_size != ep->a_data) {
	    vm_offset_t	buffer;
	    
	    addr = loader_trunc_page(data_start + ep->a_data);
	    size = LOADER_PAGE_SIZE;
	    
	    error = vm_read(my_task,
			    addr,
			    size,
			    &buffer,
			    &size);
	    
	    if (error == KERN_SUCCESS) {
		bzero((char *)(buffer + size - (data_size - ep->a_data)),
		      data_size - ep->a_data);
		error = vm_write(my_task,
				 addr,
				 buffer,
				 size);
		
		(void) vm_deallocate(mach_task_self(), buffer, size);
	    }
	    
	    if (error != KERN_SUCCESS) {
		uprintf("cannot zero partial data page\n");
		goto suicide;
	    }
	}
    }
    
    /*
     *	Create the stack.
     */
    
    size = round_page(stack_size);
    addr = round_page((vm_offset_t)USRSTACK) - size;
    
    u.u_stack_start = (caddr_t) addr;
    u.u_stack_end = u.u_stack_start + size;
    u.u_stack_grows_up = FALSE;
    
    if (vm_allocate(my_task, &addr, size, FALSE) != KERN_SUCCESS) {
	uprintf("Cannot find space for stack.\n");
	goto suicide;
    }
    
    *r_text_size = text_size;
    *r_data_size = data_bss_size;
    
    if (!on_master) {
	unix_master();
	ilock(ip);
    }
    return (error);
    
  suicide:
    if (!on_master) {
	unix_master();
	ilock(ip);
    }
    return (EGETXFILE);
}

set_arg_addr(arg_size,
	     arg_start_p,
	     arg_size_p)
u_int		arg_size;
vm_offset_t	*arg_start_p;
vm_size_t	*arg_size_p;
{
    /*
     * Round arg size to fullwords
     */
    arg_size = (arg_size + NBPW-1) & ~(NBPW - 1);
    
    /*
     * Put argument list at top of stack.
     */
    *arg_start_p = USRSTACK - arg_size;
    *arg_size_p = arg_size;
}

set_entry_address(ep, entry, entry_count)
struct exec	*ep;
int		*entry;		/* pointer to OUT array */
unsigned int	*entry_count;	/* out */
{
    entry[0] = ep->a_entry;
    *entry_count = 1;
}

/*
 * Set up the process' registers to start the emulator - duplicate
 * what execve() in the emulator would do.
 */
set_emulator_state(ep, arg_addr)
struct exec	*ep;
vm_offset_t	arg_addr;
{
    thread_state_regs_t		regs;
    thread_state_frame_t	frame;
    
    bzero((char *)&regs, sizeof(regs));
    regs.r_sp = (int)arg_addr;
    
    (void) thread_set_state(u.u_procp->p_thread,
			    THREAD_STATE_REGS,
			    &regs,
			    THREAD_STATE_REGS_COUNT);
    
    frame.f_normal.f_pc = (int)ep->a_entry;
    frame.f_normal.f_sr = 0;
    frame.f_normal.f_fmt = STKFMT_NORMAL;
    
    (void) thread_set_state(u.u_procp->p_thread,
			    THREAD_STATE_FRAME,
			    &frame,
			    THREAD_STATE_FRAME_COUNT);
}

/*
 * Do weird things to the first process' address space before loading
 * the emulator.
 */
void
emul_init_process(p)
struct proc	*p;
{
    /*
     * Clear out the entire address space.
     */
    (void) vm_deallocate(p->p_task,
			 VM_MIN_ADDRESS,
			 (vm_size_t)(VM_MAX_ADDRESS - VM_MIN_ADDRESS));
}

struct call_frame {
    struct call_frame	*f_fp;
    unsigned long	f_pc;
    unsigned long	f_param[0];
};

machine_core(p, ip)
register struct proc	*p;
register struct inode	*ip;
{
    register			i;
    thread_state_regs_t		regs;
    thread_state_frame_t	frame;
    unsigned			count;

    count = THREAD_STATE_REGS_COUNT;
    (void) thread_get_state(p->p_thread,
			    THREAD_STATE_REGS,
			    &regs,
			    &count);

    count = THREAD_STATE_FRAME_COUNT;
    (void) thread_get_state(p->p_thread,
			    THREAD_STATE_FRAME,
			    &frame,
			    &count);

    printf("machine core: %s\n", p->p_utask.uu_comm);
    printf("FMT %x vector %03x PC %08x SR %04x\n",
	   frame.f_normal.f_fmt,
	   frame.f_normal.f_vector,
	   frame.f_normal.f_pc,
	   frame.f_normal.f_sr);

    for (i = 0; i < 4; i++)
	printf("D%x %08x ", i, regs.r_dreg[i]);
    printf("\n");
    for (i = 0; i < 4; i++)
	printf("D%x %08x ", i + 4, regs.r_dreg[i + 4]);
    printf("\n");
    for (i = 0; i < 4; i++)
	printf("A%x %08x ", i, regs.r_areg[i]);
    printf("\n");
    for (i = 0; i < 4; i++)
	printf("A%x %08x ", i + 4, regs.r_areg[i + 4]);
    printf("\n");

    {
	vm_address_t		stack_page, stack_page_end;
	vm_size_t		stack_page_size;
	unsigned		fp = regs.r_areg[6];
	unsigned		user_page_start, user_page_end;
	struct call_frame	*x;
	register		i;

	while (vm_read(p->p_task,
		       trunc_page(fp),
		       vm_page_size,
		       &stack_page,
		       &stack_page_size) == KERN_SUCCESS) {
	    stack_page_end = stack_page + stack_page_size;
	    user_page_start = trunc_page(fp);
	    user_page_end = user_page_start + vm_page_size;
	    while (fp >= user_page_start && fp < user_page_end) {
		x = (struct call_frame *)(fp - user_page_start + stack_page);
		printf("Called from %x, fp %x, args", x->f_pc, x->f_fp);
		for (i = 0; i <= 5; i++) {
		    if ((vm_address_t)&x->f_param[i] < stack_page_end)
			printf(" %x", x->f_param[i]);
		    else
			break;
		}
		printf("\n");

		fp = (unsigned)x->f_fp;
	    }
	    (void) vm_deallocate(mach_task_self(),
				 stack_page,
				 stack_page_size);
	}
    }

    return (0);
}

int ptrace_get_u_word(p, offset, value)
	struct proc *p;
	register int	offset;
	int	*value;	/* out */
{
    panic("ptrace get u word");
}

int ptrace_set_u_word(p, offset, value, old)
	struct proc *p;
	register int offset;
	int	value;
	int	*old;
{
    panic("ptrace set u word");
}

void ptrace_set_trace(p, new_addr, trace_on)
	struct proc *p;
	int	new_addr;
	int	trace_on;
{
    panic("ptrace set trace");
}

/*
 * Send an interrupt to process.
 *
 * Stack is set up to allow sigcode stored in u. to call routine,
 * followed by chmk to sigreturn routine below.  After sigreturn
 * resets the signal mask, the stack, the frame pointer, and the
 * argument pointer, it returns to the user-specified pc/psl.
 */
sendsig(p, thread, sig_hdlr, sig, code, mask)
struct proc	*p;
thread_t	thread;
int		(*sig_hdlr)(), sig, code, mask;
{
    thread_state_regs_t		regs;
    thread_state_frame_t	frame;
    unsigned int		regs_size, frame_size;
    register vm_offset_t	usp, old_usp, saved_frame;
    register struct sigframe {
	int	sig;
	int	code;
	int	scp;
    } *sf;
    register struct sigcontext	*sc;
    register int		scflags;
    vm_address_t		user_start_addr, user_end_addr;
    vm_offset_t			buffer_addr;
    vm_size_t			size;

    regs_size = THREAD_STATE_REGS_COUNT;
    (void) thread_get_state(thread,
			    THREAD_STATE_REGS,
			    &regs,
			    &regs_size);

    frame_size = THREAD_STATE_FRAME_COUNT;
    (void) thread_get_state(thread,
			    THREAD_STATE_FRAME,
			    &frame,
			    &frame_size);

#if	MAP_UAREA
    share_lock(&p->p_shared_rw->us_lock, p);
#endif	MAP_UAREA

    scflags = p->p_utask.uu_sigstack.ss_onstack;

    if (!scflags && (p->p_utask.uu_sigonstack & sigmask(sig))) {
	usp = (int)p->p_utask.uu_sigstack.ss_sp;
	p->p_utask.uu_sigstack.ss_onstack = 1;
    }
    else
	usp = regs.r_sp;

#if	MAP_UAREA
    share_unlock(&p->p_shared_rw->us_lock, p);
#endif	MAP_UAREA

    old_usp = usp;
    
    if (frame.f_normal.f_fmt != STKFMT_NORMAL) {
	switch (frame.f_normal.f_fmt) {
	  case STKFMT_SHORT_BUSERR:
	    frame_size = SHORT_BUSERR_EXCEPTION_FRAME_SIZE;
	    break;
	
	  case STKFMT_LONG_BUSERR:
	    frame_size = LONG_BUSERR_EXCEPTION_FRAME_SIZE;
	    break;
	
	  default:
	    frame_size = 0;
	    break;
	}

	if (frame_size > 0) {
	    usp = saved_frame = usp - frame_size;
	    scflags |= SC_FRAMESAVE;
	}
    }

    usp = (vm_offset_t)sc = usp - sizeof (struct sigcontext);
    usp = (vm_offset_t)sf = usp - sizeof (struct sigframe);

    /*
     * Copy the signal frame from the user into the kernel,
     * to modify it.
     */
    user_start_addr = trunc_page(usp);
    user_end_addr   = round_page(old_usp);
    size = user_end_addr - user_start_addr;

    if (vm_read(p->p_task,
		user_start_addr, size,
		&buffer_addr, &size) != KERN_SUCCESS)
	goto error;

    if (scflags & SC_FRAMESAVE) {
	saved_frame = saved_frame - user_start_addr + buffer_addr;
	bcopy(&frame, saved_frame, frame_size);
    }

    (vm_offset_t)sf = (vm_offset_t)sf - user_start_addr + buffer_addr;
    sf->sig = sig;
    if (sig == SIGILL || sig == SIGFPE || sig == SIGEMT)
	sf->code = code;
    else
	sf->code = 0;
    sf->scp = (vm_offset_t)sc;

    (vm_offset_t)sc = (vm_offset_t)sc - user_start_addr + buffer_addr;
    sc->sc_flags = scflags;
    sc->sc_mask = mask;
    sc->sc_sp = old_usp;
    sc->sc_pc = frame.f_normal.f_pc;
    sc->sc_ps = frame.f_normal.f_sr;

    /*
     * Write signal frame and context back to user.
     */
    if (vm_write(p->p_task,
		 user_start_addr,
		 buffer_addr, size) != KERN_SUCCESS) {
	(void) vm_deallocate(mach_task_self(), buffer_addr, size);
	goto error;
    }

    (void) vm_deallocate(mach_task_self(), buffer_addr, size);

    regs.r_sp = usp;
    (void) thread_set_state(thread,
			    THREAD_STATE_REGS,
			    &regs,
			    regs_size);

    frame.f_normal.f_fmt = STKFMT_NORMAL;
    frame.f_normal.f_pc = (unsigned long)sig_hdlr;
    frame.f_normal.f_sr = 0;
    frame_size =
	(NORMAL_EXCEPTION_FRAME_SIZE + sizeof (long) - 1) / sizeof (long);
    (void) thread_set_state(thread,
			    THREAD_STATE_FRAME,
			    &frame,
			    frame_size);

    /*
     * Clear TRACE_USER ast state??
     */
    return;

error:
    /*
     * Process has trashed its stack; kill it and core dump it.
     * The idea is to imitate the default action for a SIGILL.
     */
    proc_exit(p, SIGILL, TRUE);
}

sigreturn()
{
    panic("sigreturn - MiG interface");
}
osigcleanup()
{
    panic("osigcleanup - MiG interface");
}

/*
 * Clone the parent's registers into the child thread for fork.
 */
boolean_t
thread_dup(child_thread, new_state, new_state_count, parent_pid, rc)
thread_t	child_thread;
thread_state_t	new_state;
unsigned int	new_state_count;
int		parent_pid, rc;
{
    struct child {
	thread_state_regs_t	regs;
	thread_state_frame_t	frame;
    } *child = (struct child *)new_state;
    unsigned		regs_count, frame_count;

    regs_count = THREAD_STATE_REGS_COUNT;
    frame_count = new_state_count - regs_count;

    if ((int)frame_count < 0)
	return (FALSE);

    child->regs.r_r0 = parent_pid;
    child->regs.r_r1 = rc;

    (void) thread_set_state(child_thread,
			    THREAD_STATE_REGS,
			    &child->regs, regs_count);

    (void) thread_set_state(child_thread,
			    THREAD_STATE_FRAME,
			    &child->frame, frame_count);

    /* XXX What about FPU ?? */

    return (TRUE);
}

boolean_t
machine_emulator_fault(thread)
thread_t	thread;
{
    thread_state_frame_t	frame;
    unsigned			count;

    count = THREAD_STATE_FRAME_COUNT;
    if (thread_get_state(thread,
			 THREAD_STATE_FRAME,
			 &frame,
			 &count) != KERN_SUCCESS)
	return (FALSE);

    if (frame.f_normal.f_vector == 8
	&& frame.f_normal.f_pc >= (unsigned)EMULATOR_BASE) {
	frame.f_normal.f_pc = (unsigned)EMULATOR_BASE + sizeof (struct exec);
	frame.f_normal.f_sr = 0;
	frame.f_normal.f_fmt = STKFMT_NORMAL;
	frame.f_normal.f_vector = 0;
	count = NORMAL_EXCEPTION_FRAME_SIZE / sizeof (int);

	if (thread_set_state(thread,
			     THREAD_STATE_FRAME,
			     &frame,
			     count) != KERN_SUCCESS)
	    return (FALSE);
/*
        printf("machine_emulator_fault: vector %x pc %x\n",
	    frame.f_normal.f_vector, frame.f_normal.f_pc);
*/
	return (TRUE);
    }

    return (FALSE);
}

/* TEMP */

int
imin(a, b)
int	a, b;
{
    if (a < b)
	return (a);

    return (b);
}

unsigned
min(a, b)
unsigned	a, b;
{
    if (a < b)
	return (a);

    return (b);
}

/* TEMP */
