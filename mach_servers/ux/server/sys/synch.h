/* 
 * Mach Operating System
 * Copyright (c) 1989 Carnegie-Mellon University
 * All rights reserved.  The CMU software License Agreement specifies
 * the terms and conditions for use and redistribution.
 */
/*
 * HISTORY
 * $Log:	synch.h,v $
 * Revision 2.1  89/08/04  14:44:04  rwd
 * Created.
 * 
 *  8-May-89  David Golub (dbg) at Carnegie-Mellon University
 *	Created.
 *
 */

/*
 * SPL level definitions.
 */
#define	SPL_COUNT	6

#define	SPL0		0
				/* placeholder - not used */
#define	SPLSOFTCLOCK	1
				/* level '0x8' */
#define	SPLNET		2
				/* level '0xc' */
#define	SPLTTY		3
				/* level '0x15' */
#define	SPLBIO		3
				/* level '0x15' */
#define	SPLIMP		4
				/* level '0x16' */
#define	SPLHIGH		5
				/* level '0x18' */


#define	spl0()		(spl_n(SPL0))
#define	splsoftclock()	(spl_n(SPLSOFTCLOCK))
#define	splnet()	(spl_n(SPLNET))
#define	splbio()	(spl_n(SPLBIO))
#define	spltty()	(spl_n(SPLTTY))
#define	splimp()	(spl_n(SPLIMP))
#define	splhigh()	(spl_n(SPLHIGH))
#define	splx(s)		(spl_n(s))
#define splsched()	(spl_n(SPLHIGH))
#define splvm()		(spl_n(SPLIMP))

extern int	spl_n();
extern void	interrupt_enter();
extern void	interrupt_exit();
