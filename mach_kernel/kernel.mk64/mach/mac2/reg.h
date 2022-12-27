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
 * $Log:	reg.h,v $
 * Revision 2.2  91/09/12  16:53:00  bohman
 * 	Created Mach 3.0 version from 2.5 version.
 * 	[91/09/11  17:14:09  bohman]
 * 
 * Revision 2.2.1.1  90/09/07  00:54:19  bohman
 * 	Final cleanup.
 * 	[90/09/07            bohman]
 * 
 * Revision 2.2  90/09/04  17:13:05  bohman
 * 	Created.
 * 
 */

/*
 *	Apple Macintosh II Mach (macmach)
 *
 *	File: mach/mac2/reg.h
 *	Author: David E. Bohman II (CMU macmach)
 */

#ifndef _MACH_MAC2_REG_H_
#define _MACH_MAC2_REG_H_

/*
 * Format of saved 68020/30
 * processor registers.
 */
typedef struct regs {
    int	r_dreg[8];		/* data registers */
#define r_r0 r_dreg[0]
#define r_r1 r_dreg[1]
    int	r_areg[8];		/* address registers */
#define r_sp	r_areg[7]
} regs_t;

/*
 * Format of 68881/2 floating
 * point register.
 */
typedef	struct {
    unsigned long	fp[3];
} fp_reg_t;

/*
 * Total register state for the FPU.
 */
typedef struct {
    fp_reg_t		fps_regs[8];		/* floating point regs */
    unsigned long	fps_control;		/* control reg */
    unsigned long	fps_status;		/* status reg */
    unsigned long	fps_iaddr;		/* instr address reg */
} fp_state_t;

/*
 * Internal state for the FPU.
 */
#define FPF_NULL_SIZE	0
#define FPF_IDLE_SIZE	0x18
#define FPF_BUSY_SIZE	0xd4

typedef union {
    unsigned long	fpf_format;		/* format word */
    struct {
	unsigned char	fpff_version;		/* version number */
	unsigned char	fpff_size;		/* size of frame */
	unsigned short	fpff_mbz;
	union {
	    struct {
		unsigned short	fpffi_ccr;	/* command/cond register */
		unsigned short	fpffi_mbo;
		unsigned char	fpffi_opnd[12];	/* exceptional operand */
		unsigned long	fpffi_opnd_reg;	/* operand register */
		unsigned long	fpffi_biu_flgs;	/* biu flags */
	    } fpffs_idle_data;
	    unsigned char	fpffs_busy_data[FPF_BUSY_SIZE];
	} fpff_extra;
    } fpf_frame;
} fp_frame_t;
#define fpf_version	fpf_frame.fpff_version
#define fpf_size	fpf_frame.fpff_size
#define fpf_data	fpf_frame.fpff_extra

#endif _MACH_MAC2_REG_H_
