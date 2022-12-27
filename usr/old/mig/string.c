/* 
 * Mach Operating System
 * Copyright (c) 1989 Carnegie-Mellon University
 * All rights reserved.  The CMU software License Agreement specifies
 * the terms and conditions for use and redistribution.
 */
/*
 * HISTORY
 * $Log:	string.c,v $
 * Revision 1.1  90/02/19  01:15:44  bww
 * 	Mach Release 2.5
 * 	[90/02/19  01:14:23  bww]
 * 
 * Revision 1.5  89/05/20  22:17:50  mrt
 * 	Extensive revamping.  Added polymorphic arguments.
 * 	Allow multiple variable-sized inline arguments in messages.
 * 	[89/04/07            rpd]
 * 
 * 15-Jun-87  David Black (dlb) at Carnegie-Mellon University
 *	Declare and initialize charNULL here for strNull def in string.h
 *
 * 27-May-87  Richard Draves (rpd) at Carnegie-Mellon University
 *	Created.
 */
/*
 * ABSTRACT:
 *   String utility programs to allocate and copy
 *   or concatenate, free and return constant strings.
 */

#define	EXPORT_BOOLEAN
#include <mach/boolean.h>
#include <sys/types.h>
#include "error.h"
#include "alloc.h"
#include "string.h"

char	charNULL = 0;

string_t
strmake(string)
    char *string;
{
    register string_t saved;

    saved = malloc((u_int) (strlen(string) + 1));
    if (saved == strNULL)
	fatal("strmake('%s'): %s", string, unix_error_string(errno));
    return strcpy(saved, string);
}

string_t
strconcat(left, right)
    string_t left, right;
{
    register string_t saved;

    saved = malloc((u_int) (strlen(left) + strlen(right) + 1));
    if (saved == strNULL)
	fatal("strconcat('%s', '%s'): %s",
	      left, right, unix_error_string(errno));
    return strcat(strcpy(saved, left), right);
}

void
strfree(string)
    string_t string;
{
    free(string);
}

char *
strbool(bool)
    boolean_t bool;
{
    if (bool)
	return "TRUE";
    else
	return "FALSE";
}

char *
strstring(string)
    string_t string;
{
    if (string == strNULL)
	return "NULL";
    else
	return string;
}
