/* 
 * Mach Operating System
 * Copyright (c) 1988 Carnegie-Mellon University
 * All rights reserved.  The CMU software License Agreement specifies
 * the terms and conditions for use and redistribution.
 */
/*
 *	File:	kern/macro_help.h
 *
 *	Provide help in making lint-free macro routines
 *
 * HISTORY
 * $Log:	macro_help.h,v $
 * Revision 2.1  89/08/04  14:46:36  rwd
 * Created.
 * 
 * Revision 2.2  88/10/18  03:36:20  mwyoung
 * 	Added a form of return that can be used within macros that
 * 	does not result in "statement not reached" noise.
 * 	[88/10/17            mwyoung]
 * 	
 * 	Add MACRO_BEGIN, MACRO_END.
 * 	[88/10/11            mwyoung]
 * 	
 * 	Created.
 * 	[88/10/08            mwyoung]
 * 
 */

#ifndef	_MACRO_HELP_
#define	_MACRO_HELP_	1

#include <mach/boolean.h>

#ifdef	lint
boolean_t	NEVER;
boolean_t	ALWAYS;
#else	lint
#define		NEVER		FALSE
#define		ALWAYS		TRUE
#endif	lint

#define		MACRO_BEGIN	do {
#define		MACRO_END	} while (NEVER)

#define		MACRO_RETURN	if (ALWAYS) return

#endif	_MACRO_HELP_
