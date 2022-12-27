/* 
 * Mach Operating System
 * Copyright (c) 1991,1990,1989 Carnegie Mellon University
 * All Rights Reserved.
 * 
 * Permission to use, copy, modify and distribute this software and its
 * documentation is hereby granted, provided that both the copyright
 * notice and this permission notice appear in all copies of the
 * software, derivative works or modified versions, and any portions
 * thereof, and that both notices appear in supporting documentation.
 * 
 * CARNEGIE MELLON ALLOWS FREE USE OF THIS SOFTWARE IN ITS "AS IS"
 * CONDITION.  CARNEGIE MELLON DISCLAIMS ANY LIABILITY OF ANY KIND FOR
 * ANY DAMAGES WHATSOEVER RESULTING FROM THE USE OF THIS SOFTWARE.
 * 
 * Carnegie Mellon requests users of this software to return to
 * 
 *  Software Distribution Coordinator  or  Software.Distribution@CS.CMU.EDU
 *  School of Computer Science
 *  Carnegie Mellon University
 *  Pittsburgh PA 15213-3890
 * 
 * any improvements or extensions that they make and grant Carnegie Mellon
 * the rights to redistribute these changes.
 */


/*
 * HISTORY
 * $Log:	ast.h,v $
 * Revision 2.2  91/09/12  16:38:39  bohman
 * 	Created Mach 3.0 version from 2.5 version.
 * 	[91/09/11  14:21:19  bohman]
 * 
 * Revision 2.2  90/08/30  11:00:33  bohman
 * 	Created.
 * 	[90/08/29  10:57:27  bohman]
 * 
 */

/*
 *	Apple Macintosh II Mach (macmach)
 *
 *	File: mac2/ast.h
 *	Author: David E. Bohman II (CMU macmach)
 */

#ifndef	_MAC2_AST_H_
#define _MAC2_AST_H_

#include <kern/macro_help.h>

#define MACHINE_AST

#define	aston(n) \
MACRO_BEGIN						\
{							\
    register	s = spl7();				\
\
    current_thread_pcb()->pcb_ast |= TRACE_AST;		\
\
    splx(s);						\
}							\
MACRO_END

#define	astoff(n) \
MACRO_BEGIN						\
{							\
    register	s = spl7();				\
\
    current_thread_pcb()->pcb_ast &= ~TRACE_AST;	\
\
    splx(s);						\
}							\
MACRO_END

#endif _MAC2_AST_H_
