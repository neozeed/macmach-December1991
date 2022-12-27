/* 
 * Mach Operating System
 * Copyright (c) 1989 Carnegie-Mellon University
 * Copyright (c) 1988 Carnegie-Mellon University
 * Copyright (c) 1987 Carnegie-Mellon University
 * All rights reserved.  The CMU software License Agreement specifies
 * the terms and conditions for use and redistribution.
 */
/*
 * HISTORY
 * $Log:	trace.h,v $
 * Revision 2.1  90/10/27  20:46:50  dpj
 * Moving under MACH3 source control
 * 
 * Revision 1.5  89/05/02  11:18:37  dpj
 * 	Fixed all files to conform to standard copyright/log format
 * 
 * 27-Apr-87  Robert Sansom (rds) at Carnegie Mellon University
 *	Allow tracing_on to be defined as a macro.
 *	Added locking for printf's.
 *
 * 19-Mar-87  Robert Sansom (rds) at Carnegie Mellon University
 *	Copied from network server tracing macros.
 *
 */
/*
 * trace.h 
 *
 *
 * $Header: /afs/cs.cmu.edu/project/mach-2/rcs/pkg/netmsg/server/trace.h,v 2.1 90/10/27 20:46:50 dpj Exp $
 */

/*
 * General purpose tracing macros - particularly useful when using cthreads.
 */


#ifndef _TRACE_
#define _TRACE_

#ifdef DEBUGOFF

#define BEGIN(_ROUTINE_NAME_)	{
#define END			}
#define RET			return
#define RETURN(_VALUE_)		return(_VALUE_)

#else DEBUGOFF

#include <stdio.h>

extern int	trace_recursion_level;
#ifndef	tracing_on
extern int	tracing_on;
#endif	tracing_on

#ifdef	LOCKTRACING

#ifndef	trace_lock
#include <cthreads.h>
extern mutex_t	trace_lock
#endif	trace_lock

#define BEGIN(_ROUTINE_NAME_)						\
{									\
	char *_routine_name = _ROUTINE_NAME_;				\
	trace_recursion_level++;					\
	if (tracing_on) {						\
		int i;							\
		mutex_lock(trace_lock);					\
	    	for (i=1; i<=trace_recursion_level; i++)		\
			fprintf(stdout, " ");				\
	        fprintf(stdout, "Enter %s\n", _routine_name);		\
		(void)fflush(stdout);					\
		mutex_unlock(trace_lock);				\
	}								\
	{


#define END								\
	}								\
}

#define RET								\
{									\
	if (tracing_on) {						\
		int i;							\
		mutex_lock(trace_lock);					\
		for (i=1; i<=trace_recursion_level; i++)		\
			fprintf(stdout, " ");				\
		fprintf(stdout, "Exit  %s\n", _routine_name);		\
		(void)fflush(stdout);					\
		mutex_unlock(trace_lock);				\
	}								\
	trace_recursion_level--;					\
	return;								\
}

#define RETURN(_value_)							\
{									\
	if (tracing_on) {						\
		int i;							\
		mutex_lock(trace_lock);					\
		for (i=1; i<=trace_recursion_level; i++)		\
			fprintf(stdout, " ");				\
		fprintf(stdout, "Exit  %s\n", _routine_name);		\
		(void)fflush(stdout);					\
		mutex_unlock(trace_lock);				\
	}								\
	trace_recursion_level--;					\
	return(_value_);						\
}

#else	LOCKTRACING

#define BEGIN(_ROUTINE_NAME_)						\
{									\
	char *_routine_name = _ROUTINE_NAME_;				\
	trace_recursion_level++;					\
	if (tracing_on) {						\
		int i;							\
	    	for (i=1; i<=trace_recursion_level; i++)		\
			fprintf(stdout, " ");				\
	        fprintf(stdout, "Enter %s\n", _routine_name);		\
		(void)fflush(stdout);					\
	}								\
	{


#define END								\
	}								\
}

#define RET								\
{									\
	if (tracing_on) {						\
		int i;							\
		for (i=1; i<=trace_recursion_level; i++)		\
			fprintf(stdout, " ");				\
		fprintf(stdout, "Exit  %s\n", _routine_name);		\
		(void)fflush(stdout);					\
	}								\
	trace_recursion_level--;					\
	return;								\
}

#define RETURN(_value_)							\
{									\
	if (tracing_on) {						\
		int i;							\
		for (i=1; i<=trace_recursion_level; i++)		\
			fprintf(stdout, " ");				\
		fprintf(stdout, "Exit  %s\n", _routine_name);		\
		(void)fflush(stdout);					\
	}								\
	trace_recursion_level--;					\
	return(_value_);						\
}

#endif	LOCKTRACING
#endif	DEBUGOFF


#endif	_TRACE_
