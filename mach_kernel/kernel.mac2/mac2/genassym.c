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
 * $Log:	genassym.c,v $
 * Revision 2.2  91/09/12  16:40:38  bohman
 * 	Created Mach 3.0 version from 2.5 version.
 * 	[91/09/11  14:44:10  bohman]
 * 
 * Revision 2.2  90/09/04  17:20:25  bohman
 * 	Created.
 * 
 */

/*
 *	Apple Macintosh II Mach (macmach)
 *
 *	File: mac2/genassym.c
 */
#include <mach/mach_types.h>

#include <machine/psl.h>
#include <machine/cpu.h>

#include <kern/thread.h>

main()
{
    regs_t	*rp = (regs_t *)0;
    pcb_t	pcb = (pcb_t)0;
    task_t	task = (task_t)0;
    struct	eml_dispatch *eml = (struct eml_dispatch *)0;
    thread_t	thread = (thread_t)0;
    normal_exception_frame_t *frame = (normal_exception_frame_t *)0;
     
    printf("#define\tPMMU_RP_LIMIT 0x%x\n", PMMU_RP_LIMIT);
    printf("#define\tPMMU_VALID_RP 0x%x\n", PMMU_VALID_RP);
    printf("#define\tPCB_REGS 0x%x\n", &pcb->pcb_user);
    printf("#define\tPCB_FRAME 0x%x\n", &pcb->pcb_frame);
    printf("#define\tPCB_STACK 0x%x\n", &pcb->pcb_stack);
    printf("#define\tPCB_D0 0x%x\n", &pcb->pcb_user.r_dreg[0]);
    printf("#define\tPCB_D1 0x%x\n", &pcb->pcb_user.r_dreg[1]);
    printf("#define\tPCB_D3 0x%x\n", &pcb->pcb_user.r_dreg[3]);
    printf("#define\tPCB_A0 0x%x\n", &pcb->pcb_user.r_areg[0]);
    printf("#define\tPCB_A1 0x%x\n", &pcb->pcb_user.r_areg[1]);
    printf("#define\tPCB_A2 0x%x\n", &pcb->pcb_user.r_areg[2]);
    printf("#define\tPCB_SP 0x%x\n", &pcb->pcb_user.r_sp);
    printf("#define\tR_SP 0x%x\n", &rp->r_sp);
    printf("#define\tF_PC 0x%x\n", &frame->f_pc);
    printf("#define\tF_SR 0x%x\n", &frame->f_sr);
    printf("#define\tF_VOR 0x%x\n", ((long)&frame->f_pc)+sizeof (long));
    printf("#define\tPCB_RETURN_FRAME_SIZE 0x%x\n", &pcb->pcb_return_frame_size);
    printf("#define\tPCB_RETURN_FRAME_DATA 0x%x\n", &pcb->pcb_return_frame_data);
    printf("#define\tPCB_K_REGS 0x%x\n", &pcb->pcb_kernel);
    printf("#define\tPCB_K_SP 0x%x\n", &pcb->pcb_kernel.r_ksp);
    printf("#define\tPCB_K_PC 0x%x\n", &pcb->pcb_kernel.r_pc);
    printf("#define\tPCB_K_SR 0x%x\n", &pcb->pcb_kernel.r_sr);
    printf("#define\tPCB_FP_FRAME 0x%x\n", &pcb->pcb_fp_frame);
    printf("#define\tPCB_FPS_REGS 0x%x\n", pcb->pcb_fp_state.fps_regs);
    printf("#define\tPCB_FPS_CTRL 0x%x\n", &pcb->pcb_fp_state.fps_control);
    printf("#define\tPCB_FLAGS 0x%x\n", &pcb->pcb_flags);
    printf("#define\tPCB_AST 0x%x\n", &pcb->pcb_ast);
    printf("#define\tPCB_THREAD 0x%x\n", &pcb->pcb_thread);
    printf("#define\tRET_FRAME_BIT %d\n", bit(RET_FRAME));
    printf("#define\tMAC_EMULATION_BIT %d\n", bit(MAC_EMULATION));
    printf("#define\tTRACE_USER_BIT %d\n", bit(TRACE_USER));
    printf("#define\tTRACE_AST_BIT %d\n", bit(TRACE_AST));
    printf("#define\tSR_SUPR_BIT %d\n", bit(SR_SUPR));
    printf("#define\tSR_TRACE_BIT %d\n", bit(SR_TRACE));
    printf("#define\tSR_T0_BIT %d\n", bit(SR_T0));
    printf("#define\tFP_RESTORE_BIT %d\n", bit(FP_RESTORE));
    printf("#define\tEML_MIN_SYSCALL 0x%x\n", EML_MIN_SYSCALL);
    printf("#define\tTASK_EMUL 0x%x\n", &task->eml_dispatch);
    printf("#define\tEMUL_DISP_COUNT 0x%x\n", &eml->disp_count);
    printf("#define\tEMUL_DISP_VECTOR 0x%x\n", &eml->disp_vector[0]);
    printf("#define\tTHREAD_PCB 0x%x\n", &thread->pcb);
    printf("#define\tTHREAD_RECOVER 0x%x\n", &thread->recover);
    printf("#define\tTHREAD_TASK 0x%x\n", &thread->task);
    exit(0);
}

bit(mask)
register long mask;
{
    register int i;
    
    for (i = 0; i < 32; i++) {
	if (mask & 1)
	    return (i);
	mask >>= 1;
    }
    exit (1);
}
