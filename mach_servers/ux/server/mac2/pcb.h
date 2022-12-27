/* 
 * Mach Operating System
 * Copyright (c) 1990 Carnegie-Mellon University
 * Copyright (c) 1989 Carnegie-Mellon University
 * Copyright (c) 1988 Carnegie-Mellon University
 * All rights reserved.  The CMU software License Agreement specifies
 * the terms and conditions for use and redistribution.
 */
/*
 * HISTORY
 * $Log:	pcb.h,v $
 * Revision 2.2.1.1  90/09/07  00:57:30  bohman
 * 	Final cleanup.
 * 	[90/09/07            bohman]
 * 
 * Revision 2.2  90/09/04  17:26:19  bohman
 * 	Created.
 * 
 */

/*
 *	Apple Macintosh II Mach (macmach)
 *
 *	File: mac2/pcb.h
 *	Author: David E. Bohman II (CMU macmach)
 */

#ifndef _MAC2_PCB_H_
#define _MAC2_PCB_H_

#ifndef	ASSEMBLER
#include <mach/mac2/frame.h>
#include <mach/mac2/reg.h>
#endif	ASSEMBLER

/*
 * Define the general registers that are
 * saved per thread during a context switch.
 */
#define NKDREGS		6
#define KDREG(n)	((n) - (8 - NKDREGS))
#define KDMOVEM		d2-d7

#define NKAREGS		6
#define KAREG(n)	((n) - (8 - NKAREGS))
#define KAMOVEM		a2-a7

#ifndef ASSEMBLER
struct pcb {
    regs_t		pcb_user;    		/* saved user registers */
    generic_exception_frame_t
			*pcb_frame;		/* ptr to user exc frame */
    struct {					/* user fpu state: */
	fp_state_t	fp_state;		/* fpu register state */
	fp_frame_t	fp_frame;		/* fpu internal state */
    } pcb_fp;
#define pcb_fp_state	pcb_fp.fp_state
#define pcb_fp_frame	pcb_fp.fp_frame
    struct {					/* saved kernel state: */
	unsigned long r_kdreg[NKDREGS];		/* kernel data registers */
	unsigned long r_kareg[NKAREGS];		/* kernel address registers */
#define	r_kfp r_kareg[KAREG(6)]
#define r_ksp r_kareg[KAREG(7)]
	unsigned short r_sr;			/* kernel status reg */
	unsigned long r_pc;			/* kernel pc */
    } pcb_kernel;
    struct {					/* user exc return frame: */
	unsigned long	frame_size;		/* total size of frame */
	generic_exception_frame_t
	    		frame_data;		/* saved frame data */
    } pcb_return_frame;
#define	pcb_return_frame_size	    pcb_return_frame.frame_size
#define	pcb_return_frame_data	    pcb_return_frame.frame_data
    unsigned short	pcb_ast;		/* thread ast state: */
#define	AST_SCHED	0x8000		/* force a reschedule */
#define	TRACE_USER	0x4000		/* tracing for user */
#define	TRACE_AST	0x2000		/* tracing for ast */
#define	TRACE_PENDING	0x1000		/* trace on user trap inst pending */
    unsigned short	pcb_flags;		/* thread misc state: */
#define	RET_FRAME	0x8000		/* return using frame in pcb */
#define RET_SPECIAL	0x4000		/* special info in return frame */
#define JMP_FAULT	0x2000		/* jump through pcb_label on fault */
#define FP_RESTORE	0x1000		/* force restore of fpu registers */
#define MAC_EMULATION	0x0800		/* thread is emulating MacOS */
#define MAC_24BIT	0x0400		/* thread is in 24 bit mode */
    struct thread	*pcb_thread;	/* back pointer to thread */
};

extern struct pcb *current_thread_pcb();
#endif

#ifdef	KERNEL
#define	aston()		{ current_thread_pcb()->pcb_ast |= AST_SCHED; }
#endif	KERNEL

#endif  _MAC2_PCB_H_
