/* 
 * Mach Operating System
 * Copyright (c) 1989 Carnegie-Mellon University
 * All rights reserved.  The CMU software License Agreement specifies
 * the terms and conditions for use and redistribution.
 */
/*
 * HISTORY
 * $Log:	thread.c,v $
 * Revision 1.2  89/05/05  19:00:52  mrt
 * 	Cleanup for Mach 2.5
 * 
 */
/*
 * mac2/thread.c
 *
 * Cproc startup for mac2 MTHREAD implementation.
 */

#ifndef	lint
static char rcs_id[] = "$Header: thread.c,v 1.2 89/05/05 19:00:52 mrt Exp $";
#endif	not lint


#include <cthreads.h>
#include "cthread_internals.h"

#include <mach.h>

/*
 * C library imports:
 */
extern bzero();

int
cthread_sp()
{
	int x;

	return (int) &x;
}

/*
 * Set up the initial state of a MACH thread
 * so that it will invoke cthread_body(child)
 * when it is resumed.
 */
void
cproc_setup(child, thread, routine)
	register cproc_t child;
	int thread;
	int routine;
{
	register int *top = (int *) (child->stack_base + child->stack_size);
	thread_state_regs_t state;
	thread_state_frame_t frame;
	kern_return_t r;

	/*
	 * Set up mac2 registers.
	 */
	bzero((char *) &state, sizeof(thread_state_regs_t));

	/*
	 * Inner cast needed since too many C compilers choke on the type void (*)().
	 */
        frame.f_normal.f_fmt = STKFMT_NORMAL;
	frame.f_normal.f_vector = 0;
 	frame.f_normal.f_pc = routine;
	frame.f_normal.f_sr = 0;
	*--top = (int) child;	/* argument to function */
	*--top = 0;
	state.r_sp = (int) top;

	MACH_CALL(thread_set_state(thread, \
				   THREAD_STATE_REGS, \
				   (thread_state_t) &state, \
				   THREAD_STATE_REGS_COUNT),
		  r);
	MACH_CALL(thread_set_state(thread, \
				   THREAD_STATE_FRAME, \
				   (thread_state_t) &frame, \
				   THREAD_STATE_FRAME_COUNT),
		  r);
}
