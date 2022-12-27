/* 
 * Mach Operating System
 * Copyright (c) 1989 Carnegie-Mellon University
 * All rights reserved.  The CMU software License Agreement specifies
 * the terms and conditions for use and redistribution.
 */
/*
 * HISTORY
 * $Log:	alloc.h,v $
 * Revision 1.1  90/02/19  01:14:29  bww
 * 	Mach Release 2.5
 * 	[90/02/19  01:14:23  bww]
 * 
 * Revision 1.5  89/05/20  22:14:27  mrt
 * 	Extensive revamping.  Added polymorphic arguments.
 * 	Allow multiple variable-sized inline arguments in messages.
 * 	[89/04/07            rpd]
 * 
 *
 * 27-May-87  Richard Draves (rpd) at Carnegie-Mellon University
 *	Created.
 */

#ifndef	_ALLOC_H_
#define	_ALLOC_H_

extern char *malloc();
extern char *calloc();
extern void free();

#endif	_ALLOC_H_
