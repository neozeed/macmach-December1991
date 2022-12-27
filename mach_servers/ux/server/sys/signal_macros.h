/* 
 * Mach Operating System
 * Copyright (c) 1988 Carnegie-Mellon University
 * All rights reserved.  The CMU software License Agreement specifies
 * the terms and conditions for use and redistribution.
 */
/*
 * HISTORY
 * $Log:	signal_macros.h,v $
 * Revision 2.3  90/03/14  21:29:36  rwd
 * 	Added VICE check for SRMT to mask stopsigmask.
 * 	[90/02/28            rwd]
 * 
 * Revision 2.2  89/10/17  11:26:34  rwd
 * 	Change HAVE_SIGNALS to check for signals in p_sig.
 * 	Add EXITING check.
 * 	[89/09/22            dbg]
 * 	Added history message.
 * 	[89/09/21            dbg]
 * 
 */
/*
 * Macros to check and lock signal handling.
 */

/*
 * Check whether a kernel thread should quit.  Conditions are
 * process exiting or signal being delivered to a user thread.
 */
#if	VICE
#define	HAVE_SIGNALS(p)	\
	( (p)->p_sig & ~ \
	    ( (p)->p_sigmask \
	      | ( ((p)->p_flag&STRC) ? 0 : (p)->p_sigignore) \
	      | ( (p->p_flag & SRMT) ? stopsigmask : 0) \
	    ) \
	)
#else	VICE
#define	HAVE_SIGNALS(p)	\
	( (p)->p_sig & ~ \
	    ( (p)->p_sigmask \
	      | ( ((p)->p_flag&STRC) ? 0 : (p)->p_sigignore) \
	    ) \
	)
#endif	VICE

#define	EXITING(p)	(((p)->p_flag & SWEXIT) != 0)
