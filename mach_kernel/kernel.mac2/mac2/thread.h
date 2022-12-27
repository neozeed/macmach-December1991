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
 * $Log:	thread.h,v $
 * Revision 2.2  91/09/12  16:44:29  bohman
 * 	Created Mach 3.0 version from mac2/pcb.h of 2.5 version.
 * 	[91/09/11  15:21:01  bohman]
 * 
 */

/*
 *	Apple Macintosh II Mach (macmach)
 *
 *	File: mac2/thread.h
 */

#ifndef	_MAC2_THREAD_H_
#define	_MAC2_THREAD_H_

#include <mach/mac2/frame.h>
#include <mach/mac2/reg.h>

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

typedef struct pcb {
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
#define	TRACE_AST	0x8000		/* tracing for ast */
    unsigned short	pcb_flags;		/* thread misc state: */
#define	RET_FRAME	0x8000		/* return using frame in pcb */
#define	TRACE_USER	0x4000		/* tracing for user */
#define FP_RESTORE	0x1000		/* force restore of fpu registers */
#define MAC_EMULATION	0x0800		/* thread is emulating MacOS */
    vm_offset_t		pcb_stack;	/* bottom of thread kernel stack */
    struct thread	*pcb_thread;	/* back pointer to thread */
} *pcb_t;

extern pcb_t	current_thread_pcb();

#endif	_MAC2_THREAD_H_
