/* 
 * Mach Operating System
 * Copyright (c) 1991,1990,1989 Carnegie Mellon University
 * All Rights Reserved.
 * 
 * Permission to use, copy, modify and distribute this software and its
 * documentation is hereby granted, provided that both the copyright
 * notice and this permission notice appear in all copies of the
 * software, derivative works or modified versions, and any portions
 * thereof, and that both notices appear in supporting documentation.
 * 
 * CARNEGIE MELLON ALLOWS FREE USE OF THIS SOFTWARE IN ITS "AS IS"
 * CONDITION.  CARNEGIE MELLON DISCLAIMS ANY LIABILITY OF ANY KIND FOR
 * ANY DAMAGES WHATSOEVER RESULTING FROM THE USE OF THIS SOFTWARE.
 * 
 * Carnegie Mellon requests users of this software to return to
 * 
 *  Software Distribution Coordinator  or  Software.Distribution@CS.CMU.EDU
 *  School of Computer Science
 *  Carnegie Mellon University
 *  Pittsburgh PA 15213-3890
 * 
 * any improvements or extensions that they make and grant Carnegie Mellon
 * the rights to redistribute these changes.
 */


/*
 * HISTORY
 * $Log:	pcb.c,v $
 * Revision 2.2  91/09/12  16:42:10  bohman
 * 	Created Mach 3.0 version from 2.5 version.
 * 	[91/09/11  14:55:26  bohman]
 * 
 * Revision 2.2.1.1  90/09/07  00:56:17  bohman
 * 	Final cleanup.
 * 	[90/09/07            bohman]
 * 
 * Revision 2.2  90/09/04  17:23:02  bohman
 * 	Created.
 * 
 */

/*
 *	Apple Macintosh II Mach (macmach)
 *
 *	File: mac2/pcb.c
 *	Author: David E. Bohman II (CMU macmach)
 */

#include <mach/mach_types.h>

#include <kern/mach_param.h>
#include <kern/thread.h>

zone_t	pcb_zone;

/*
 * Called from MI initialization
 */
void
pcb_module_init()
{
    pcb_zone = zinit(sizeof (struct pcb),
		     THREAD_MAX * sizeof (struct pcb),
		     THREAD_CHUNK * sizeof (struct pcb),
		     FALSE, "pcb");
}

/*
 * Assign a kernel stack to
 * a thread.
 */
void
stack_attach(thread, stack, continuation)
register thread_t	thread;
register vm_offset_t	stack;
void			(*continuation)();
{
    register pcb_t		pcb = thread->pcb;
    register unsigned long	*ksp;
    extern void			stack_attach_continue();

    thread->kernel_stack = stack;

    (vm_offset_t)ksp = pcb->pcb_stack = stack + KERNEL_STACK_SIZE;
    *--ksp = (unsigned long)continuation;
    --ksp;
    pcb->pcb_kernel.r_ksp = (unsigned long)ksp;

    pcb->pcb_kernel.r_pc = (unsigned long)stack_attach_continue;
    pcb->pcb_kernel.r_sr = SR_HIGH;
}

/*
 * Return a thread's kernel stack
 * so that it can be reused by
 * another thread.  If the return
 * frame is located on the stack,
 * copy it into the pcb.
 */
inline
vm_offset_t
stack_detach(thread)
register thread_t		thread;
{
    register vm_offset_t	stack;
    register pcb_t		pcb = thread->pcb;

    stack = thread->kernel_stack;
    pcb->pcb_stack = thread->kernel_stack = 0;

    if ((pcb->pcb_flags & RET_FRAME) == 0) {
	switch (pcb->pcb_frame->f_normal.f_fmt) {
	  case STKFMT_NORMAL:
	    pcb->pcb_return_frame_data.f_normal = pcb->pcb_frame->f_normal;
	    pcb->pcb_return_frame_size = NORMAL_EXCEPTION_FRAME_SIZE;
	    break;

	  case STKFMT_SPECIAL:
	    pcb->pcb_return_frame_data.f_special = pcb->pcb_frame->f_special;
	    pcb->pcb_return_frame_size = SPECIAL_EXCEPTION_FRAME_SIZE;
	    break;

	  case STKFMT_COPROC:
	    pcb->pcb_return_frame_data.f_coproc = pcb->pcb_frame->f_coproc;
	    pcb->pcb_return_frame_size = COPROC_EXCEPTION_FRAME_SIZE;
	    break;

	  case STKFMT_SHORT_BUSERR:
	    pcb->pcb_return_frame_data.f_short_buserr =
		pcb->pcb_frame->f_short_buserr;
	    pcb->pcb_return_frame_size = SHORT_BUSERR_EXCEPTION_FRAME_SIZE;
	    break;

	  case STKFMT_LONG_BUSERR:
	    pcb->pcb_return_frame_data.f_long_buserr =
		pcb->pcb_frame->f_long_buserr;
	    pcb->pcb_return_frame_size = LONG_BUSERR_EXCEPTION_FRAME_SIZE;
	    break;

	  default:
	    panic("stack_detach");
	}

	pcb->pcb_frame = &pcb->pcb_return_frame_data;
	pcb->pcb_flags |= RET_FRAME;
    }

    return (stack);
}

/*
 * Perform a context switch to
 * a new thread by stealing the
 * kernel stack of the old one.
 * Always returns directly to the
 * calling code, but running as
 * the new thread.
 */
void
stack_handoff(old_th, new_th)
register thread_t	old_th, new_th;
{
    register vm_offset_t	stack;
    register			cpu = cpu_number();

    if (old_th->task != new_th->task) {
	PMAP_DEACTIVATE_USER(vm_map_pmap(old_th->task->map), old_th, cpu);
	PMAP_ACTIVATE_USER(vm_map_pmap(new_th->task->map), new_th, cpu);
    }

    new_th->kernel_stack = stack = stack_detach(old_th);

    new_th->pcb->pcb_stack = stack + KERNEL_STACK_SIZE;

    active_threads[cpu] = new_th;

    switch_context_handoff(old_th, new_th);
}

/*
 * Perform a normal context switch between
 * two threads.  If a continuation is provided
 * for the old thread, then its kernel context
 * is discarded.
 */
thread_t
switch_context(old_th, continuation, new_th)
register thread_t	old_th;
void			(*continuation)();
register thread_t	new_th;
{
    register		cpu = cpu_number();
    extern thread_t	switch_context_discard(), switch_context_save();

    if (old_th->task != new_th->task) {
	PMAP_DEACTIVATE_USER(vm_map_pmap(old_th->task->map), old_th, cpu);
	PMAP_ACTIVATE_USER(vm_map_pmap(new_th->task->map), new_th, cpu);
    }

    active_threads[cpu] = new_th;
    active_stacks[cpu] = new_th->kernel_stack;

    if (old_th->swap_func = continuation)
	return switch_context_discard(old_th, new_th);
    else
	return switch_context_save(old_th, new_th);
}

/*
 * Allocate a pcb for a new thread.
 */
void
pcb_init(thread)
register thread_t	thread;
{
    register pcb_t			pcb;
    register generic_exception_frame_t 	*frame;

    pcb = (pcb_t) zalloc(pcb_zone);
    if (!pcb)
	panic("pcb_init");
    
    thread->pcb = pcb;

    bzero(pcb, sizeof (struct pcb));

    /*
     * Propagate the notion of mac emulation
     * being enabled between threads in a
     * task.  Here I am assuming that 
     * current_task() is the task that is
     * doing the thread_create().
     */
    if (current_task() == thread->task
	&& (current_thread_pcb()->pcb_flags & MAC_EMULATION))
	    pcb->pcb_flags |= MAC_EMULATION;

    pcb->pcb_thread = thread;

    pcb->pcb_frame = frame = &pcb->pcb_return_frame_data;
    pcb->pcb_return_frame_size = NORMAL_EXCEPTION_FRAME_SIZE;
    pcb->pcb_flags |= RET_FRAME;

    frame->f_normal.f_sr = SR_USER;
    frame->f_normal.f_fmt = STKFMT_NORMAL;
    frame->f_normal.f_vector = 0;
}

/*
 * Deallocate a pcb for a thread
 * that is being terminated.
 */
void
pcb_terminate(thread)
register thread_t	thread;
{
    zfree(pcb_zone, (vm_offset_t)thread->pcb);

    thread->pcb = 0;
}

/*
 * Alter the thread`s state so that a following thread_exception_return
 * will make the thread return 'retval' from a syscall.
 */
void
thread_set_syscall_return(thread, retval)
register thread_t	thread;
kern_return_t		retval;
{
    thread->pcb->pcb_user.r_r0 = retval;
}


/*
 *	thread_setstatus:
 *
 *	Set the status of the specified thread.
 */
kern_return_t
thread_setstatus(thread, flavor, tstate, count)
thread_t		thread;
int			flavor;
thread_state_t		tstate;
unsigned int		count;
{
    register pcb_t	pcb = thread->pcb;
    
    switch (flavor) {
      case THREAD_STATE_REGS:
	{
	    register thread_state_regs_t	*state;
	    
	    if (count < THREAD_STATE_REGS_COUNT)
		return (KERN_INVALID_ARGUMENT);
	    
	    state = (thread_state_regs_t *)tstate;
	    
	    /*
	     * copy machine registers
	     */
	    pcb->pcb_user = *state;
	}
	break;
	
      case THREAD_STATE_FPREGS:
	{
	    register thread_state_fpregs_t	*state;
	    
	    if (count < THREAD_STATE_FPREGS_COUNT)
		return (KERN_INVALID_ARGUMENT);
	    
	    state = (thread_state_fpregs_t *)tstate;

	    pcb->pcb_fp_state = *state;
	    pcb->pcb_flags |= FP_RESTORE;
	    pcb->pcb_fp_frame.fpf_format = 0;
	}
	break;
	
      case THREAD_STATE_FRAME:
	{
	    register thread_state_frame_t *state;
	    
	    state = (thread_state_frame_t *)tstate;

	    /*
	     * We know that all frame types
	     * are an integral number of long
	     * words in length.
	     */
	    switch (state->f_normal.f_fmt) {
	      case STKFMT_NORMAL:
		if (count < (NORMAL_EXCEPTION_FRAME_SIZE / sizeof (int)))
		    return (KERN_INVALID_ARGUMENT);

		pcb->pcb_return_frame_data.f_normal = state->f_normal;
		pcb->pcb_return_frame_size = NORMAL_EXCEPTION_FRAME_SIZE;
		break;
		
	      case STKFMT_SPECIAL:
		if (count < (SPECIAL_EXCEPTION_FRAME_SIZE / sizeof (int)))
		    return (KERN_INVALID_ARGUMENT);

		pcb->pcb_return_frame_data.f_special = state->f_special;
		pcb->pcb_return_frame_size = SPECIAL_EXCEPTION_FRAME_SIZE;
		break;
		
	      case STKFMT_COPROC:
		if (count < (COPROC_EXCEPTION_FRAME_SIZE / sizeof (int)))
		    return (KERN_INVALID_ARGUMENT);

		pcb->pcb_return_frame_data.f_coproc = state->f_coproc;
		pcb->pcb_return_frame_size = COPROC_EXCEPTION_FRAME_SIZE;
		break;
		
	      case STKFMT_SHORT_BUSERR:
		if (count < (SHORT_BUSERR_EXCEPTION_FRAME_SIZE / sizeof (int)))
		    return (KERN_INVALID_ARGUMENT);

		pcb->pcb_return_frame_data.f_short_buserr =
		    state->f_short_buserr;
		pcb->pcb_return_frame_size = SHORT_BUSERR_EXCEPTION_FRAME_SIZE;
		break;
		
	      case STKFMT_LONG_BUSERR:
		if (count < (LONG_BUSERR_EXCEPTION_FRAME_SIZE / sizeof (int)))
		    return (KERN_INVALID_ARGUMENT);

		pcb->pcb_return_frame_data.f_long_buserr =
		    state->f_long_buserr;
		pcb->pcb_return_frame_size = LONG_BUSERR_EXCEPTION_FRAME_SIZE;
		break;
		
	      default:
		return (KERN_INVALID_ARGUMENT);
	    }

	    pcb->pcb_frame = &pcb->pcb_return_frame_data;
	    pcb->pcb_flags |= RET_FRAME;
	    
	    /*
	     *	Enforce user mode status register:
	     *	must have user mode, user stack, interrupt priority 0.
	     *	User may set trace single bit.
	     */
	    pcb->pcb_frame->f_normal.f_sr &= ~(SR_T0|SR_SUPR|SR_MASTER|SR_IPL);

	    /*
	     * Check for user trace.
	     */
	    if (pcb->pcb_frame->f_normal.f_sr & SR_TRACE)
		pcb->pcb_flags |= TRACE_USER;
	    else
		pcb->pcb_flags &= ~TRACE_USER;
	}
	break;

      case THREAD_STATE_FPFRAME:
	{
	    register thread_state_fpframe_t *state;

	    state = (thread_state_fpframe_t *)tstate;

	    if (count < ((sizeof (state->fpf_format) + state->fpf_size)
			 / sizeof (int)))
		return (KERN_INVALID_ARGUMENT);

	    pcb->pcb_fp_frame.fpf_format = state->fpf_format;
	    if (state->fpf_size > 0)
		bcopy(&state->fpf_data,
		      &pcb->pcb_fp_frame.fpf_data, state->fpf_size);

	    pcb->pcb_flags &= ~FP_RESTORE;
	}
	break;
	
      default:
	return (KERN_INVALID_ARGUMENT);
    }
    
    return (KERN_SUCCESS);
}

/*
 *	thread_getstatus:
 *
 *	Get the status of the specified thread.
 */
kern_return_t
thread_getstatus(thread, flavor, tstate, count)
thread_t	thread;
int		flavor;
thread_state_t	tstate;		/* pointer to OUT array */
unsigned int	*count;		/* IN/OUT */
{
    register pcb_t	pcb = thread->pcb;
    
    switch (flavor) {
      case THREAD_STATE_REGS:
	{
	    register thread_state_regs_t	*state;
	    
	    if (*count < THREAD_STATE_REGS_COUNT)
		return (KERN_INVALID_ARGUMENT);
	    
	    state = (thread_state_regs_t *)tstate;
	    
	    /*
	     * copy machine registers
	     */
	    *state = pcb->pcb_user;

	    *count = THREAD_STATE_REGS_COUNT;
	}
	break;
	
      case THREAD_STATE_FPREGS:
	{
	    register thread_state_fpregs_t	*state;
	    
	    if (*count < THREAD_STATE_FPREGS_COUNT)
		return (KERN_INVALID_ARGUMENT);
	    
	    state = (thread_state_fpregs_t *)tstate;

	    *state = pcb->pcb_fp_state;

	    *count = THREAD_STATE_FPREGS_COUNT;
	}
	break;
	
      case THREAD_STATE_FRAME:
	{
	    register thread_state_frame_t	*state;

	    state = (thread_state_frame_t *)tstate;

	    /*
	     * We know that all frame types
	     * are an integral number of long
	     * words in length.
	     */
	    switch (pcb->pcb_frame->f_normal.f_fmt) {
	      case STKFMT_NORMAL:
		if (*count < (NORMAL_EXCEPTION_FRAME_SIZE / sizeof (int)))
		    return (KERN_INVALID_ARGUMENT);

		state->f_normal = pcb->pcb_return_frame_data.f_normal;
		*count = NORMAL_EXCEPTION_FRAME_SIZE / sizeof (int);
		break;
		
	      case STKFMT_SPECIAL:
		if (*count < (SPECIAL_EXCEPTION_FRAME_SIZE / sizeof (int)))
		    return (KERN_INVALID_ARGUMENT);

		state->f_special = pcb->pcb_return_frame_data.f_special;
		*count = SPECIAL_EXCEPTION_FRAME_SIZE / sizeof (int);
		break;
		
	      case STKFMT_COPROC:
		if (*count < (COPROC_EXCEPTION_FRAME_SIZE / sizeof (int)))
		    return (KERN_INVALID_ARGUMENT);

		state->f_coproc = pcb->pcb_return_frame_data.f_coproc;
		*count = COPROC_EXCEPTION_FRAME_SIZE / sizeof (int);
		break;
		
	      case STKFMT_SHORT_BUSERR:
		if (*count < (SHORT_BUSERR_EXCEPTION_FRAME_SIZE
			      / sizeof (int)))
		    return (KERN_INVALID_ARGUMENT);

		state->f_short_buserr =
		    pcb->pcb_return_frame_data.f_short_buserr;
		*count = SHORT_BUSERR_EXCEPTION_FRAME_SIZE / sizeof (int);
		break;
		
	      case STKFMT_LONG_BUSERR:
		if (*count < (LONG_BUSERR_EXCEPTION_FRAME_SIZE
			      / sizeof (int)))
		    return (KERN_INVALID_ARGUMENT);

		state->f_long_buserr =
		    pcb->pcb_return_frame_data.f_long_buserr;
		*count = LONG_BUSERR_EXCEPTION_FRAME_SIZE / sizeof (int);
		break;
		
	      default:
		return (KERN_INVALID_ARGUMENT);
	    }
	}
	break;

      case THREAD_STATE_FPFRAME:
	{
	    register thread_state_fpframe_t	*state;
	    register fp_frame_t			*fp_frame;

	    state = (thread_state_fpframe_t *)tstate;

	    fp_frame = &pcb->pcb_fp_frame;
	    if (*count < ((sizeof (fp_frame->fpf_format) + fp_frame->fpf_size)
			  / sizeof (int)))
		return (KERN_INVALID_ARGUMENT);

	    state->fpf_format = fp_frame->fpf_format;
	    if (state->fpf_size > 0)
		bcopy(&fp_frame->fpf_data, &state->fpf_data, state->fpf_size);

	    *count = ((sizeof (state->fpf_format) + state->fpf_size)
		      / sizeof (int));
	}
	break;
	
      default:
	return (KERN_INVALID_ARGUMENT);
    }
    
    return (KERN_SUCCESS);
}
