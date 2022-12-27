#if	CMU
/*
 * HISTORY
 * $Log:	vcmd.h,v $
 * Revision 2.1  89/08/04  14:49:32  rwd
 * Created.
 * 
 * Revision 2.2  88/08/24  02:52:39  mwyoung
 * 	Adjusted include file references.
 * 	[88/08/17  02:27:43  mwyoung]
 * 
 */
#endif	CMU
/*
 * Copyright (c) 1982, 1986 Regents of the University of California.
 * All rights reserved.  The Berkeley software License Agreement
 * specifies the terms and conditions for redistribution.
 *
 *	@(#)vcmd.h	7.1 (Berkeley) 6/4/86
 */

#ifndef _IOCTL_
#include <sys/ioctl.h>
#endif

#define	VPRINT		0100
#define	VPLOT		0200
#define	VPRINTPLOT	0400

#ifdef mac2 /* tahoe compatability */
#define	VGETSTATE	_IOR('v', 0, int)
#define	VSETSTATE	_IOW('v', 1, int)
#else
#define	VGETSTATE	_IOR(v, 0, int)
#define	VSETSTATE	_IOW(v, 1, int)
#endif
