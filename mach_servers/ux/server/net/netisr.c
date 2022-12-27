/* 
 * Mach Operating System
 * Copyright (c) 1987 Carnegie-Mellon University
 * All rights reserved.  The CMU software License Agreement specifies
 * the terms and conditions for use and redistribution.
 */
/*
 * HISTORY
 * $Log:	netisr.c,v $
 * Revision 2.2  90/06/19  23:12:30  rpd
 * 	Added debugging code to dosoftnet.
 * 	[90/06/06            rpd]
 * 
 * Revision 2.1  89/08/04  14:17:59  rwd
 * Created.
 * 
 * 24-Feb-89  David Golub (dbg) at Carnegie-Mellon University
 *	Out-of-kernel version - invoke directly from hardware receive
 *	routines.  NO CONDITIONALS!
 *
 *  1-Feb-88  David Golub (dbg) at Carnegie-Mellon University
 *	Goofed... netisr thread must run at splnet, because the routines
 *	it calls expect to be called from the softnet interrupt (at
 *	splnet).
 *
 * 19-Nov-87  David Golub (dbg) at Carnegie-Mellon University
 *	Created.
 *
 */

#include <imp.h>

#include <sys/user.h>
#include <net/netisr.h>

int		netisr = 0;
struct mutex	netisr_mutex = MUTEX_INITIALIZER;

/*
 * Must be called at splnet.
 */
void dosoftnet()
{
	int original_master = u.u_master_lock;
	u.uu_xxx[0]++;	/* for debugging */

	mutex_lock(&netisr_mutex);

#if	NIMP > 0
	if (netisr & (1<<NETISR_IMP)) {
	    netisr &= ~(1<<NETISR_IMP);
	    mutex_unlock(&netisr_mutex);
	    impintr();
	    mutex_lock(&netisr_mutex);
	}
#endif	NIMP > 0
#ifdef	INET
	if (netisr & (1<<NETISR_IP)) {
	    netisr &= ~(1<<NETISR_IP);
	    mutex_unlock(&netisr_mutex);
	    ipintr();
	    mutex_lock(&netisr_mutex);
	}
#endif	INET
#ifdef	NS
	if (netisr & (1<<NETISR_NS)) {
	    netisr &= ~(1<<NETISR_NS);
	    mutex_unlock(&netisr_mutex);
	    nsintr();
	    mutex_lock(&netisr_mutex);
	}
#endif	NS
	if (netisr & (1<<NETISR_RAW)) {
	    netisr &= ~(1<<NETISR_RAW);
	    mutex_unlock(&netisr_mutex);
	    rawintr();
	}
	else {
	    mutex_unlock(&netisr_mutex);
	}

	if (original_master != u.u_master_lock)
		panic("dosoftnet");
}
