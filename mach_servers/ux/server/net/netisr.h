/* 
 * Mach Operating System
 * Copyright (c) 1987 Carnegie-Mellon University
 * All rights reserved.  The CMU software License Agreement specifies
 * the terms and conditions for use and redistribution.
 */
/* 
 **********************************************************************
 * HISTORY
 * $Log:	netisr.h,v $
 * Revision 2.1  89/08/04  14:18:03  rwd
 * Created.
 * 
 * 24-Feb-89  David Golub (dbg) at Carnegie-Mellon University
 *	Out-of-kernel version - NO CONDITIONALS!
 *
 *  5-Nov-87  David Golub (dbg) at Carnegie-Mellon University
 *	Use kernel-thread for networking code.
 *
 **********************************************************************
 */ 
#include <uxkern/import_mach.h>

/*
 * Copyright (c) 1980, 1986 Regents of the University of California.
 * All rights reserved.  The Berkeley software License Agreement
 * specifies the terms and conditions for redistribution.
 *
 *	@(#)netisr.h	7.1 (Berkeley) 6/4/86
 */

/*
 * The networking code runs off software interrupts.
 *
 * You can switch into the network by doing splnet() and return by splx().
 * The software interrupt level for the network is higher than the software
 * level for the clock (so you can enter the network in routines called
 * at timeout time).
 */

/*
 * Each ``pup-level-1'' input queue has a bit in a ``netisr'' status
 * word which is used to de-multiplex a single software
 * interrupt used for scheduling the network code to calls
 * on the lowest level routine of each protocol.
 */
#define	NETISR_RAW	0		/* same as AF_UNSPEC */
#define	NETISR_IP	2		/* same as AF_INET */
#define	NETISR_IMP	3		/* same as AF_IMPLINK */
#define	NETISR_NS	6		/* same as AF_NS */

#ifndef LOCORE
#ifdef KERNEL
extern int		netisr;		/* scheduling bits for network */
extern struct mutex	netisr_mutex;

#define	schednetisr(anisr)			\
	{					\
	    mutex_lock(&netisr_mutex);		\
	    netisr |= 1<<(anisr);		\
	    mutex_unlock(&netisr_mutex);	\
	}

#endif
#endif
