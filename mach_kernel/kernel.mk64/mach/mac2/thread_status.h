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
 * $Log:	thread_status.h,v $
 * Revision 2.2  91/09/12  16:53:20  bohman
 * 	Created Mach 3.0 version from 2.5 version.
 * 	[91/09/11  17:15:42  bohman]
 * 
 * Revision 2.2.1.1  90/09/07  00:53:12  bohman
 * 	Final cleanup.
 * 	[90/09/07            bohman]
 * 
 * Revision 2.2  90/08/30  17:51:48  bohman
 * 	Created.
 * 
 */

/*
 *	Apple Macintosh II Mach (macmach)
 *
 *	File: mach/mac2/thread_status.h
 *	Author: David E. Bohman II (CMU macmach)
 */

/*
 * This file contains the structure definitions for the thread
 * state as applied to Macintosh II computers 68020/30 with 68881/2.
 */

#ifndef	_MACH_MAC2_THREAD_STATUS_H_
#define	_MACH_MAC2_THREAD_STATUS_H_

#include <mach/mac2/reg.h>
#include <mach/mac2/frame.h>

/*
 * Thread processor registers.
 */
#define	THREAD_STATE_REGS	1

typedef regs_t	thread_state_regs_t;
#define	THREAD_STATE_REGS_COUNT		\
    (sizeof(thread_state_regs_t)/sizeof (long))

/*
 * Thread fpu registers.
 */
#define THREAD_STATE_FPREGS	2

typedef fp_state_t	thread_state_fpregs_t;
#define THREAD_STATE_FPREGS_COUNT	\
    (sizeof (thread_state_fpregs_t)/sizeof (long))

/*
 * Thread processor state frame.
 */
#define THREAD_STATE_FRAME	3

typedef generic_exception_frame_t	thread_state_frame_t;
#define THREAD_STATE_FRAME_COUNT	\
    (sizeof (thread_state_frame_t)/sizeof (long))

/*
 * Thread fpu state frame.
 */
#define THREAD_STATE_FPFRAME	4

typedef fp_frame_t	thread_state_fpframe_t;
#define THREAD_STATE_FPFRAME_COUNT	\
    (sizeof (thread_state_fpframe_t)/sizeof (long))

#endif	_MACH_MAC2_THREAD_STATUS_H_
