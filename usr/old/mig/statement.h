/* 
 * Mach Operating System
 * Copyright (c) 1989 Carnegie-Mellon University
 * All rights reserved.  The CMU software License Agreement specifies
 * the terms and conditions for use and redistribution.
 */
/*
 * HISTORY
 * $Log:	statement.h,v $
 * Revision 1.1  90/02/19  01:15:40  bww
 * 	Mach Release 2.5
 * 	[90/02/19  01:14:23  bww]
 * 
 * Revision 1.5  89/05/20  22:17:44  mrt
 * 	Extensive revamping.  Added polymorphic arguments.
 * 	Allow multiple variable-sized inline arguments in messages.
 * 	[89/04/07            rpd]
 * 
 * 27-May-87  Richard Draves (rpd) at Carnegie-Mellon University
 *	Created.
 */

#ifndef	_STATEMENT_H_
#define	_STATEMENT_H_

#include "routine.h"

typedef enum statement_kind
{
    skRoutine,
    skImport,
    skUImport,
    skSImport,
    skRCSDecl,
} statement_kind_t;

typedef struct statement
{
    statement_kind_t stKind;
    struct statement *stNext;
    union
    {
	/* when stKind == skRoutine */
	routine_t *_stRoutine;
	/* when stKind == skImport, skUImport, skSImport */
	string_t _stFileName;
    } data;
} statement_t;

#define	stRoutine	data._stRoutine
#define	stFileName	data._stFileName

#define stNULL		((statement_t *) 0)

/* stNext will be initialized to put the statement in the list */
extern statement_t *stAlloc();

/* list of statements, in order they occur in the .defs file */
extern statement_t *stats;

#endif	_STATEMENT_H_
