/* 
 * Mach Operating System
 * Copyright (c) 1989 Carnegie-Mellon University
 * All rights reserved.  The CMU software License Agreement specifies
 * the terms and conditions for use and redistribution.
 */
/*
 * HISTORY
 * $Log:	statement.c,v $
 * Revision 1.1  90/02/19  01:15:36  bww
 * 	Mach Release 2.5
 * 	[90/02/19  01:14:23  bww]
 * 
 * Revision 1.5  89/05/20  22:17:41  mrt
 * 	Extensive revamping.  Added polymorphic arguments.
 * 	Allow multiple variable-sized inline arguments in messages.
 * 	[89/04/07            rpd]
 * 
 * 27-May-87  Richard Draves (rpd) at Carnegie-Mellon University
 *	Created.
 */
/*
 *  ABSTRACT:
 *	Called by the parser to allocate a new
 *	statement structure and thread into the
 *	statement list:
 *   Exports:
 *	stats - pointer to head of statement list
 *
 */

#include "error.h"
#include "alloc.h"
#include "statement.h"

statement_t *stats = stNULL;
static statement_t **last = &stats;

statement_t *
stAlloc()
{
    register statement_t *new;

    new = (statement_t *) malloc(sizeof *new);
    if (new == stNULL)
	fatal("stAlloc(): %s", unix_error_string(errno));
    *last = new;
    last = &new->stNext;
    new->stNext = stNULL;
    return new;
}
