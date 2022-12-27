/* 
 * Mach Operating System
 * Copyright (c) 1988 Carnegie-Mellon University
 * All rights reserved.  The CMU software License Agreement specifies
 * the terms and conditions for use and redistribution.
 */
/*
 **********************************************************************
 * HISTORY
 * $Log:	clist.h,v $
 * Revision 2.1  89/08/04  14:44:14  rwd
 * Created.
 * 
 * Revision 2.2  88/10/11  10:23:32  rpd
 * 	Made declarations be extern.
 * 	[88/10/06  07:55:03  rpd]
 * 
 **********************************************************************
 */
/*
 * Copyright (c) 1982, 1986 Regents of the University of California.
 * All rights reserved.  The Berkeley software License Agreement
 * specifies the terms and conditions for redistribution.
 *
 *	@(#)clist.h	7.1 (Berkeley) 6/4/86
 */

#include <cmucs.h>

/*
 * Raw structures for the character list routines.
 */
struct cblock {
	struct cblock *c_next;
	char	c_info[CBSIZE];
};
#ifdef KERNEL
#if	CMUCS
extern struct cblock *cfree;
extern int nclist;
extern struct cblock *cfreelist;
extern int cfreecount;
#else	CMUCS
struct	cblock *cfree;
int	nclist;
struct	cblock *cfreelist;
int	cfreecount;
#endif	CMUCS
#endif
