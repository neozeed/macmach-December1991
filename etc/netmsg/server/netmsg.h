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
 * $Log:	netmsg.h,v $
 * Revision 2.1  90/10/27  20:45:00  dpj
 * Moving under MACH3 source control
 * 
 * Revision 1.9.1.2  90/10/21  21:53:08  dpj
 * 	Fixed a history entry.
 * 
 * Revision 1.9.1.1  90/07/30  13:59:36  dpj
 * 	Added a dummy definition for mutex_clear(), to allow
 * 	compilation with the new cthreads package.
 * 
 * Revision 1.9  89/05/02  11:13:07  dpj
 * 	Fixed all files to conform to standard copyright/log format
 * 
 * 14-Apr-87  Robert Sansom (rds) at Carnegie Mellon University
 *	Replace the tracing macros by including trace.h.
 *		N.B. trace.h is structured so that it can be used
 *		independently of the network server.
 *
 * 10-Mar-87  Daniel Julin (dpj) at Carnegie-Mellon University
 *	Added include of config.h. Replaced DEBUGOFF with NET_TRACE.
 *
 * 23-Dec-86  Robert Sansom (rds) at Carnegie Mellon University
 *	Made debugging variables be lower case.
 *
 */
/*
 * netmsg.h 
 *
 *
 * $Header: /afs/cs.cmu.edu/project/mach-2/rcs/pkg/netmsg/server/netmsg.h,v 2.1 90/10/27 20:45:00 dpj Exp $
 */

/*
 * This file contains definitions of macros that will be used throughout the
 * network server.  It should be included by every *.c file.  Macros defined
 * here include a standard set of debugging macros, and others. 
 */


#ifndef _NETMSG_
#define _NETMSG_

#include "config.h"
#include "debug.h"

/*
 * Function argument order is IN, INOUT, OUT, REF. These psuedo-keywords are
 * used to label the arguments. For example: 
 *
 * Char FunReturningChar(   IN arg1, arg2, INOUT arg3, OUT arg4, arg5, REF arg6) 
 *
 * IN    indicates arg value is read, and not written. All non-pointer arguments
 * will be IN. 
 *
 * INOUT indicates arg value may be both read and written. 
 *
 * OUT   indicates arg value is only written. INOUT and OUT apply to values
 * referenced by pointers. 
 *
 * REF   indicates arg is a pointer to shared data.  Data may be read or
 * written, but may not meaningfully be copied. 
 *
 * Note: 
 *
 * Functions for which all arguments are IN need not use any of these keywords. 
 *
 */

#define IN
#define INOUT
#define OUT
#define REF

#define EXPORT
#define PUBLIC

#include "debug.h"

#if	NET_TRACE

#define LOCKTRACING	1
#define PRIVATE
#define tracing_on	debug.tracing
#define trace_lock	log_lock
#include "trace.h"

#else	NET_TRACE

#define DEBUGOFF	1
#define LOCKTRACING	1
#define tracing_on	debug.tracing
#define trace_lock	log_lock
#define PRIVATE static
#include "trace.h"

#endif	NET_TRACE

/*
 * Replace incorrect mutex_clear() in some versions of cthreads.
 */
#include	<cthreads.h>
#undef	mutex_clear
#define	mutex_clear(m)		/* NOP */

#endif _NETMSG_
