/* 
 * Mach Operating System
 * Copyright (c) 1989 Carnegie-Mellon University
 * All rights reserved.  The CMU software License Agreement specifies
 * the terms and conditions for use and redistribution.
 */
/*
 * HISTORY
 * $Log:	string.h,v $
 * Revision 1.1  90/02/19  01:15:48  bww
 * 	Mach Release 2.5
 * 	[90/02/19  01:14:23  bww]
 * 
 * Revision 1.5  89/05/20  22:17:54  mrt
 * 	Extensive revamping.  Added polymorphic arguments.
 * 	Allow multiple variable-sized inline arguments in messages.
 * 	[89/04/07            rpd]
 * 
 * 15-Jun-87  David Black (dlb) at Carnegie-Mellon University
 *	Fixed strNULL to be the null string instead of the null string
 *	pointer.
 *
 * 27-May-87  Richard Draves (rpd) at Carnegie-Mellon University
 *	Created.
 */
/*
 * ABSTRACT:
 *   Header file for string utility programs (string.c)
 */

#ifndef	_STRING_H_
#define	_STRING_H_

#include <strings.h>

typedef char *string_t;
typedef string_t identifier_t;

extern char	charNULL;
#define	strNULL		&charNULL

extern string_t strmake(/* char *string */);
extern string_t strconcat(/* string_t left, right */);
extern void strfree(/* string_t string */);

#define	streql(a, b)	(strcmp((a), (b)) == 0)

extern char *strbool(/* boolean_t bool */);
extern char *strstring(/* string_t string */);

#endif	_STRING_H_
