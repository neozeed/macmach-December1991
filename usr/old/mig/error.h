/* 
 * Mach Operating System
 * Copyright (c) 1989 Carnegie-Mellon University
 * All rights reserved.  The CMU software License Agreement specifies
 * the terms and conditions for use and redistribution.
 */
/*
 * HISTORY
 * $Log:	error.h,v $
 * Revision 1.1  90/02/19  01:14:37  bww
 * 	Mach Release 2.5
 * 	[90/02/19  01:14:23  bww]
 * 
 * Revision 1.5  89/05/20  22:14:39  mrt
 * 	Extensive revamping.  Added polymorphic arguments.
 * 	Allow multiple variable-sized inline arguments in messages.
 * 	[89/04/07            rpd]
 * 
 * 27-May-87  Richard Draves (rpd) at Carnegie-Mellon University
 *	Created.
 */

#ifndef	_ERROR_H_
#define	_ERROR_H_

#include <mach_error.h>

extern void fatal(/* char *format, ... */);
extern void warn(/* char *format, ... */);
extern void error(/* char *format, ... */);

extern int errno;
extern char *unix_error_string();

extern int errors;
extern void set_program_name(/* char *name */);

#endif	_ERROR_H_
