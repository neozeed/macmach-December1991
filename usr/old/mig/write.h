/* 
 * Mach Operating System
 * Copyright (c) 1989 Carnegie-Mellon University
 * All rights reserved.  The CMU software License Agreement specifies
 * the terms and conditions for use and redistribution.
 */
/*
 * HISTORY
 * $Log:	write.h,v $
 * Revision 1.1  90/02/19  01:16:16  bww
 * 	Mach Release 2.5
 * 	[90/02/19  01:14:23  bww]
 * 
 * Revision 1.5  89/05/20  22:18:31  mrt
 * 	Extensive revamping.  Added polymorphic arguments.
 * 	Allow multiple variable-sized inline arguments in messages.
 * 	[89/04/07            rpd]
 * 
 * 27-May-87  Richard Draves (rpd) at Carnegie-Mellon University
 *	Created.
 */

#ifndef	_WRITE_H_
#define	_WRITE_H_

#include <stdio.h>
#include "statement.h"

extern void WriteHeader(/* FILE *file, statement_t *stats */);
extern void WriteUser(/* FILE *file, statement_t *stats */);
extern void WriteUserIndividual(/* statement_t *stats */);
extern void WriteServer(/* FILE *file, statement_t *stats */);

#endif	_WRITE_H_
