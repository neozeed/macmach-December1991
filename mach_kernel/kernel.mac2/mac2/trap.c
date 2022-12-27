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
 * $Log:	trap.c,v $
 * Revision 2.2  91/09/12  16:44:48  bohman
 * 	Changed TRAP_BREAKPOINT in user_trap() to not
 * 	decrement pc to trap instruction.
 * 	Created MACH 3.0 version from 2.5 version.
 * 	[91/09/11  15:11:19  bohman]
 * 
 * Revision 2.2.1.1  90/09/07  00:58:02  bohman
 * 	Final cleanup.
 * 	[90/09/07            bohman]
 * 
 * Revision 2.2  90/09/04  17:29:49  bohman
 * 	Created.
 * 
 */

/*
 *	Apple Macintosh II Mach (macmach)
 *
 *	File: mac2/trap.c
 *	Author: David E. Bohman II (CMU macmach)
 */

#include <mach/mach_types.h>
#include <mach/vm_param.h>
#include <mach/exception.h>

#include <kern/thread.h>
#include <kern/syscall_sw.h>

#include <vm/vm_kern.h>

#include <mac2/trap.h>
#include <mac2/call_frame.h>

int trapdebug = 0;

/*
 * Handle traps from user mode.
 */
user_trap(type, frame)
register				type;
register generic_exception_frame_t	*frame;
{
    register pcb_t	pcb = current_thread_pcb();
    register		except, code, subcode = 0;
    void		check_ast();	

    pcb->pcb_frame = frame;

    if (trapdebug)
	printf("user_trap: type %x pc %x sr %x\n",
	       type, frame->f_normal.f_pc, frame->f_normal.f_sr);

    if ((pcb->pcb_flags & TRACE_USER) == 0)
	frame->f_normal.f_sr &= ~SR_TRACE;

    pcb->pcb_flags &= ~RET_FRAME;

    check_ast();

    switch (type) {
      case TRAP_BAD_ACCESS:
	{
	    register unsigned short status;
	    register fault = 0;
	    
	    switch (frame->f_normal.f_fmt) {
	      case STKFMT_SHORT_BUSERR:
#define short_buserr_frame (&frame->f_short_buserr)
		/*
		 * Data fault
		 */
		if (short_buserr_frame->f_dfault) {
		    status =
			pmmu_test_user_data(short_buserr_frame->f_fault);
		    /*
		     * RMW cycle w/o status bits
		     * means that ATC entry is
		     * missing; load it.
		     */
		    if (short_buserr_frame->f_rmw &&
			(status&
			 (PMMU_SR_WRPROT|PMMU_SR_INVALID|PMMU_SR_BUSERR)) == 0)
			pmmu_load_user(short_buserr_frame->f_fault);
		    else
		    if ((status&(PMMU_SR_INVALID|PMMU_SR_WRPROT)) &&
			(status&PMMU_SR_BUSERR) == 0) {
			/*
			 * Call pagein only for
			 * invalid entries or protection
			 * faults; never for physical memory
			 * faults.
			 */
			code = pagefault(short_buserr_frame->f_fault,
					 (short_buserr_frame->f_rmw ||
					  !short_buserr_frame->f_rw)?
					 VM_PROT_READ|VM_PROT_WRITE:
					 VM_PROT_READ,
					 short_buserr_frame->f_fcode);
			if (code == KERN_SUCCESS) {
			    if (short_buserr_frame->f_rmw)
				pmmu_load_user(short_buserr_frame->f_fault);
			}
			else
			    fault = 1;
		    }
		    else
			fault = 1;
		    
		    if (fault) {
			subcode = short_buserr_frame->f_fault;
			break;
		    }
		}
		/*
		 * Instruction fault on StageC
		 */
		if (short_buserr_frame->f_faultc) {
		    status =
			pmmu_test_user_text(short_buserr_frame->f_pc+2);
		    if ((status&(PMMU_SR_INVALID|PMMU_SR_WRPROT)) &&
			(status&PMMU_SR_BUSERR) == 0) {
			code = pagefault(short_buserr_frame->f_pc+2,
					 VM_PROT_READ, FC_UP);
			if (code == KERN_SUCCESS)
			    ;
			else
			    fault = 1;
		    }
		    else
			fault = 1;
		    
		    if (fault) {
			subcode = short_buserr_frame->f_pc+2;
			break;
		    }
		}
		/*
		 * Instruction fault on StageB
		 */
		else if (short_buserr_frame->f_faultb) {
		    status =
			pmmu_test_user_text(short_buserr_frame->f_pc+4);
		    if ((status&(PMMU_SR_INVALID|PMMU_SR_WRPROT)) &&
			(status&PMMU_SR_BUSERR) == 0) {
			code = pagefault(short_buserr_frame->f_pc+4,
					 VM_PROT_READ, FC_UP);
			if (code == KERN_SUCCESS)
			    ;
			else
			    fault = 1;
		    }
		    else
			fault = 1;
		    
		    if (fault) {
			subcode = short_buserr_frame->f_pc+4;
			break;
		    }
		}
#undef short_buserr_frame
		break;
		
	      case STKFMT_LONG_BUSERR:
#define long_buserr_frame (&frame->f_long_buserr)
		if (long_buserr_frame->f_dfault) {
		    status =
			pmmu_test_user_data(long_buserr_frame->f_fault);
		    if (long_buserr_frame->f_rmw &&
			(status&
			 (PMMU_SR_WRPROT|PMMU_SR_INVALID|PMMU_SR_BUSERR)) == 0)
			pmmu_load_user(long_buserr_frame->f_fault);
		    else
		    if ((status&(PMMU_SR_INVALID|PMMU_SR_WRPROT)) &&
			(status&PMMU_SR_BUSERR) == 0) {
			code = pagefault(long_buserr_frame->f_fault,
					 (long_buserr_frame->f_rmw ||
					  !long_buserr_frame->f_rw)?
					 VM_PROT_READ|VM_PROT_WRITE:
					 VM_PROT_READ,
					 long_buserr_frame->f_fcode);
			if (code == KERN_SUCCESS) {
			    if (long_buserr_frame->f_rmw)
				pmmu_load_user(long_buserr_frame->f_fault);
			}
			else
			    fault = 1;
		    }
		    else
			fault = 1;
		    
		    if (fault) {
			subcode = long_buserr_frame->f_fault;
			break;
		    }
		}
		if (long_buserr_frame->f_faultc) {
		    status =
			pmmu_test_user_text(long_buserr_frame->f_stageb-2);
		    if ((status&(PMMU_SR_INVALID|PMMU_SR_WRPROT)) &&
			(status&PMMU_SR_BUSERR) == 0) {
			code = pagefault(long_buserr_frame->f_stageb-2,
					 VM_PROT_READ, FC_UP);
			if (code == KERN_SUCCESS)
			    ;
			else
			    fault = 1;
		    }
		    else
			fault = 1;
		    
		    if (fault) {
			subcode = long_buserr_frame->f_stageb-2;
			break;
		    }
		}
		else if (long_buserr_frame->f_faultb) {
		    status =
			pmmu_test_user_text(long_buserr_frame->f_stageb);
		    if ((status&(PMMU_SR_INVALID|PMMU_SR_WRPROT)) &&
			(status&PMMU_SR_BUSERR) == 0) {
			code = pagefault(long_buserr_frame->f_stageb,
					 VM_PROT_READ, FC_UP);
			if (code == KERN_SUCCESS)
			    ;
			else
			    fault = 1;
		    }
		    else
			fault = 1;
		    
		    if (fault) {
			subcode = long_buserr_frame->f_stageb;
			break;
		    }
		}
#undef long_buserr_frame
		break;
		
	      default:
		user_trap_error(frame, &pcb->pcb_user);
		panic("user_trap: BUSERR unknown frame type");
	    }

	    if (!fault)
		return (0);

	    except = EXC_BAD_ACCESS;
	}
	break;

      case TRAP_TRACE:
	if ((pcb->pcb_flags & TRACE_USER) == 0)
	    return (0);

	except = EXC_BREAKPOINT;
	code = frame->f_normal.f_vector;
	break;

      case TRAP_EMULATION_1010:
      case TRAP_EMULATION_1111:
	except = EXC_EMULATION;
	code = frame->f_normal.f_vector;
	break;

      case TRAP_BAD_INSTRUCTION:
      case TRAP_PRIV_INSTRUCTION:
	except = EXC_BAD_INSTRUCTION;
	code = frame->f_normal.f_vector;
	break;

      case TRAP_BREAKPOINT:
	except = EXC_BREAKPOINT;
	code = frame->f_normal.f_vector;
	break;

      case TRAP_ARITHMETIC:
	code = frame->f_normal.f_vector;
	if (code == EXC_MAC2_FLT_BSUN || code == EXC_MAC2_FLT_OPERAND_ERROR)
	    except = EXC_BAD_INSTRUCTION;
	else
	    except = EXC_ARITHMETIC;
	break;

      case TRAP_SOFTWARE:
	except = EXC_SOFTWARE;
	code = frame->f_normal.f_vector;
	break;
	
      default:
	user_trap_error(frame, &pcb->pcb_user);
	panic("user_trap: unknown trap type");
    }

    exception(except, code, subcode);
    
    if (trapdebug)
	printf("exit user_trap: type %x pc %x sr %x\n",
	       type, frame->f_normal.f_pc, frame->f_normal.f_sr);

    return (0);
}

user_trap_error(frame, regs)
generic_exception_frame_t *frame;
regs_t *regs;
{
    cons_force(); dumptrap("user_trap", frame, regs);
}

int syscalldebug = 0;

/*
 * Handle native system calls.
 */
void
syscall(type, frame)
register				type;
register generic_exception_frame_t	*frame;
{
    register pcb_t		pcb = current_thread_pcb();
    register regs_t		*regs;
    register mach_trap_t	*entry;
    register			nargs;
#define NARGS			11
    int				args[NARGS];

    pcb->pcb_frame = frame;

    if ((pcb->pcb_flags & TRACE_USER) == 0)
	frame->f_normal.f_sr &= ~SR_TRACE;

    pcb->pcb_flags &= ~RET_FRAME;

    regs = &pcb->pcb_user;

    if (type < 0 || type >= mach_trap_count)
	goto bad;

    entry = &mach_trap_table[type];
    nargs = entry->mach_trap_arg_count;
    if (nargs > NARGS)
	goto bad;

    if (nargs > 0
	&& copyin(regs->r_sp + sizeof (int), args, nargs * sizeof (int)))
	goto bad;

    if (syscalldebug) {
	register	i;

	printf("syscall %x ( ", type);
	for (i = 0; i < nargs; i++)
	    printf("%x ", args[i]);
	printf(")");
    }

    switch (nargs) {
      case 0:
	regs->r_r0 = (*entry->mach_trap_function)();
	break;
	
      case 1:
	regs->r_r0 = (*entry->mach_trap_function)(args[0]);
	break;
	
      case 2:
	regs->r_r0 = (*entry->mach_trap_function)(args[0],
						  args[1]);
	break;
	
      case 3:
	regs->r_r0 = (*entry->mach_trap_function)(args[0],
						  args[1],
						  args[2]);
	break;
	
      case 4:
	regs->r_r0 = (*entry->mach_trap_function)(args[0],
						  args[1],
						  args[2],
						  args[3]);
	break;
	
      case 5:
	regs->r_r0 = (*entry->mach_trap_function)(args[0],
						  args[1],
						  args[2],
						  args[3],
						  args[4]);
	break;
	
      case 6:
	regs->r_r0 = (*entry->mach_trap_function)(args[0],
						  args[1],
						  args[2],
						  args[3],
						  args[4],
						  args[5]);
	break;
	
      case 7:
	regs->r_r0 = (*entry->mach_trap_function)(args[0],
						  args[1],
						  args[2],
						  args[3],
						  args[4],
						  args[5],
						  args[6]);
	break;
	
      case 8:
	regs->r_r0 = (*entry->mach_trap_function)(args[0],
						  args[1],
						  args[2],
						  args[3],
						  args[4],
						  args[5],
						  args[6],
						  args[7]);
	break;
	
      case 9:
	regs->r_r0 = (*entry->mach_trap_function)(args[0],
						  args[1],
						  args[2],
						  args[3],
						  args[4],
						  args[5],
						  args[6],
						  args[7],
						  args[8]);
	break;
	
      case 10:
	regs->r_r0 = (*entry->mach_trap_function)(args[0],
						  args[1],
						  args[2],
						  args[3],
						  args[4],
						  args[5],
						  args[6],
						  args[7],
						  args[8],
						  args[9]);
	break;
	
      case 11:
	regs->r_r0 = (*entry->mach_trap_function)(args[0],
						  args[1],
						  args[2],
						  args[3],
						  args[4],
						  args[5],
						  args[6],
						  args[7],
						  args[8],
						  args[9],
						  args[10]);
	break;
    }

    if (syscalldebug)
	printf(" returns %x\n", regs->r_r0);

    return;

bad:
    exception(EXC_BAD_INSTRUCTION, EXC_MAC2_TRAP0, type);
#undef NARGS
}

/*
 * Handle traps from kernel mode.
 */
kernel_trap(type, frame, regs)
register				type;
register generic_exception_frame_t	*frame;
register regs_t				*regs;
{
    register thread_t		thread = current_thread();
    register			code;
    register unsigned char	mode = SwapMMUMode(1);
    register unsigned long	frame_extra;
    
    if (trapdebug)
	printf("kernel_trap: type %x pc %x sr %x\n",
	       type, frame->f_normal.f_pc, frame->f_normal.f_sr);
    
#define ADJUST_SP(f, t) (unsigned long)(((t *)(f))+1)
    /*
     * Adjust the saved ksp to the
     * value it had when the trap
     * occurred; also calculate
     * the size of any extra info
     * in the stack frame.
     */
    switch (frame->f_normal.f_fmt) {
      case STKFMT_NORMAL:
	regs->r_sp = ADJUST_SP(frame, normal_exception_frame_t);
	frame_extra = NORMAL_EXCEPTION_FRAME_SIZE -
	    NORMAL_EXCEPTION_FRAME_SIZE;
	break;
	
      case STKFMT_SPECIAL:
	regs->r_sp = ADJUST_SP(frame, special_exception_frame_t);
	frame_extra = SPECIAL_EXCEPTION_FRAME_SIZE -
	    NORMAL_EXCEPTION_FRAME_SIZE;
	break;
	
      case STKFMT_COPROC:
	regs->r_sp = ADJUST_SP(frame, coproc_exception_frame_t);
	frame_extra = COPROC_EXCEPTION_FRAME_SIZE -
	    NORMAL_EXCEPTION_FRAME_SIZE;
	break;
	
      case STKFMT_SHORT_BUSERR:
	regs->r_sp = ADJUST_SP(frame, short_buserr_exception_frame_t);
	frame_extra = SHORT_BUSERR_EXCEPTION_FRAME_SIZE -
	    NORMAL_EXCEPTION_FRAME_SIZE;
	break;
	
      case STKFMT_LONG_BUSERR:
	regs->r_sp = ADJUST_SP(frame, long_buserr_exception_frame_t);
	frame_extra = LONG_BUSERR_EXCEPTION_FRAME_SIZE -
	    NORMAL_EXCEPTION_FRAME_SIZE;
	break;
    }
#undef ADJUST_SP

    switch (type) {
      case TRAP_BAD_ACCESS:
	/*
	 * Fault code is short and sweet;
	 * only data faults are allowed
	 * but no RMW cycles!
	 */
	switch (frame->f_normal.f_fmt) {
	  case STKFMT_SHORT_BUSERR:
#define short_buserr_frame (&frame->f_short_buserr)
	    if (short_buserr_frame->f_dfault)
		code = pagefault(short_buserr_frame->f_fault,
				 short_buserr_frame->f_rw?VM_PROT_READ:
				 VM_PROT_READ|VM_PROT_WRITE,
				 short_buserr_frame->f_fcode);
	    else
		code = KERN_FAILURE;
#undef short_buserr_frame
	    break;
	    
	  case STKFMT_LONG_BUSERR:
#define long_buserr_frame (&frame->f_long_buserr)
	    if (long_buserr_frame->f_dfault)
		code = pagefault(long_buserr_frame->f_fault,
				 long_buserr_frame->f_rw?VM_PROT_READ:
				 VM_PROT_READ|VM_PROT_WRITE,
				 long_buserr_frame->f_fcode);
	    else
		code = KERN_FAILURE;
#undef long_buserr_frame
	    break;
	    
	  default:
	    kernel_trap_error(frame, regs);
	    panic("kernel_trap: BUSERR unknown frame type");
	}

	if (code == KERN_SUCCESS) {
	    frame_extra = 0;
	    break;
	}

	if (thread == THREAD_NULL) {
	    kernel_trap_error(frame, regs);
	    panic("kernel_trap: no thread");
	}

	if (thread->recover == 0) {
	    kernel_trap_error(frame, regs);
	    panic("kernel_trap: BUSERR no recover");
	}
	
	frame->f_normal.f_pc = thread->recover;
	thread->recover = 0;
	break;

      case TRAP_TRACE:
	/*
	 * A traced, user mode
	 * TRAP #N instruction
	 * causes the trace trap
	 * to occur in kernel mode.
	 */
	frame->f_special.f_sr &= ~SR_TRACE;
	break;
	
      default:
	kernel_trap_error(frame, regs);
	panic("kernel_trap: unknown trap type");
    }

    SwapMMUMode(mode);

    return (frame_extra);
}

kernel_trap_error(frame, regs)
generic_exception_frame_t *frame;
regs_t *regs;
{
    cons_force(); dumptrap("kernel_trap", frame, regs);
    dumpstack(regs->r_areg[6], regs->r_sp);
}

int pagefaultdebug = 0;

/*
 * Handle page fault.
 */
kern_return_t
pagefault(addr, type, fcode)
register vm_offset_t	addr;
vm_prot_t		type;
register		fcode;
{
    register vm_map_t		map;
    register kern_return_t	result;
    register thread_t		thread;

    if (pagefaultdebug)
	printf("pagefault: addr %x type %x fcode %x ",
	       addr, type, fcode);

    thread = current_thread();

    /*
     * Determine which map to "fault" on.
     */
    switch (fcode) {
      case FC_UP:
      case FC_UD:
	map = thread->task->map;
	break;
	
      case FC_SD:
	/*
	 * If this is a kernel space
	 * task, try to fault on the
	 * task's own map, if any.  Holding
	 * locks on the kernel_map is risky and
	 * can lead to deadlocks.
	 */
	if (thread == THREAD_NULL)
	    map = kernel_map;
	else {
	    map = thread->task->map;
	    if (vm_map_pmap(map) != kernel_pmap)
		map = kernel_map;	/* XXX */
	}
	break;
	
      default:
	panic("pagefault: illegal function code");
    }
    
    if (pagefaultdebug)
	printf("thread %x map %x (%s): ",
	       thread,
	       map,
	       (map == kernel_map)? "kernel": "task");
    
    result = vm_fault(map,
		      trunc_page(addr),
		      type,
		      FALSE,
		      FALSE,
		      (void (*)()) 0);
    
    if (pagefaultdebug)
	printf("returns %d\n", result);
    
    return (result);
}

/*
 * Handle pending AST trace traps.
 */
void
check_ast()
{
    register pcb_t	pcb = current_thread_pcb();

    spl7();
    if (pcb->pcb_ast & TRACE_AST) {
	pcb->pcb_ast &= ~TRACE_AST;
	ast_taken();
    }
    else
	spl0();
}

/*
 * Dump out a stack trace.  Assumes that
 * the bottom of stack is at the end of the
 * same page where the top of stack resides.
 */
dumpstack(fp, sp)
register struct call_frame *fp;
unsigned long sp;
{
    register vm_offset_t stack_page = trunc_page(sp);
    register i;
    static boolean_t	already_done = FALSE;

    if (already_done == TRUE)
	return;

    already_done = TRUE;

    printf("Begin traceback...fp = %x, sp = %x\n", fp, sp);
    while (trunc_page(fp) == stack_page) {
	if (fp == fp->f_fp) {
	    printf("FP loop at %x\n", fp);
	    break;
	}
	printf("Called from %x, fp %x, args", fp->f_pc, fp->f_fp);
	for (i = 0; i <= 5; i++) {
	    if (trunc_page(&fp->f_param[i]) == stack_page)
		printf(" %x", fp->f_param[i]);
	    else
		break;
	}
	printf("\n");

	fp = fp->f_fp;
    }
    printf("End traceback...\n");
}

/*
 * Dump out diagnostic info
 * following a trap.
 */
dumptrap(title, frame, regs)
char *title;
register generic_exception_frame_t *frame;
register regs_t *regs;
{
    register s, *r;
    
    s = spl7();
    printf("kernel base 0x%x\n", *(unsigned *)0);
    printf("trap vector 0x%x, pc = %x, sr = %x, frame fmt %x\n",
	   frame->f_normal.f_vector,
	   frame->f_normal.f_pc, frame->f_normal.f_sr, frame->f_normal.f_fmt);
    switch (frame->f_normal.f_fmt) {
      case STKFMT_SHORT_BUSERR:
#define short_buserr_frame (&frame->f_short_buserr)
	printf("dfault %d rw %d size %d fcode %d faultc %x faultb %x\n",
	       short_buserr_frame->f_dfault, short_buserr_frame->f_rw,
	       short_buserr_frame->f_size, short_buserr_frame->f_fcode,
	       short_buserr_frame->f_faultc, short_buserr_frame->f_faultb);
	if (short_buserr_frame->f_dfault)
	    printf("data fault address %x\n", short_buserr_frame->f_fault);

	if (short_buserr_frame->f_faultc)
	    printf("text fault address %x\n", short_buserr_frame->f_pc+2);

	if (short_buserr_frame->f_faultb)
	    printf("text fault address %x\n", short_buserr_frame->f_pc+4);

#undef short_buserr_frame
	break;
	
      case STKFMT_LONG_BUSERR:
#define long_buserr_frame (&frame->f_long_buserr)
	printf("dfault %d rw %d size %d fcode %d faultc %x faultb %x\n",
	       long_buserr_frame->f_dfault, long_buserr_frame->f_rw,
	       long_buserr_frame->f_size, long_buserr_frame->f_fcode,
	       long_buserr_frame->f_faultc, long_buserr_frame->f_faultb);
	if (long_buserr_frame->f_dfault)
	    printf("data fault address %x\n", long_buserr_frame->f_fault);

	if (long_buserr_frame->f_faultc)
	    printf("text fault address %x\n", long_buserr_frame->f_stageb-2);

	if (long_buserr_frame->f_faultb)
	    printf("text fault address %x\n", long_buserr_frame->f_stageb);

#undef long_buserr_frame
	break;
	
    }
    r = &regs->r_dreg[0];
    printf("D0-D7  %x %x %x %x %x %x %x %x\n",
	   r[0], r[1], r[2], r[3], r[4], r[5], r[6], r[7]);
    r = &regs->r_areg[0];
    printf("A0-A7  %x %x %x %x %x %x %x %x\n",
	   r[0], r[1], r[2], r[3], r[4], r[5], r[6], r[7]);
    (void) splx(s);
}
