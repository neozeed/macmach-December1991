/* 
 * Mach Operating System
 * Copyright (c) 1990 Carnegie-Mellon University
 * All rights reserved.  The CMU software License Agreement specifies
 * the terms and conditions for use and redistribution.
 */
/*
 * File: $Source: /afs/cs.cmu.edu/project/mach-2/rcs/lib/mach3/mach3/mach3_setjmp.c,v $
 *
 * HISTORY
 * $Log:	mach3_setjmp.c,v $
 * Revision 2.2  90/07/26  12:37:33  dpj
 * 	First version
 * 	[90/07/24  14:28:39  dpj]
 * 
 *
 */

#ifndef lint
char * mach3_setjmp_rcsid = "$Header: /afs/cs.cmu.edu/project/mach-2/rcs/lib/mach3/mach3/mach3_setjmp.c,v 2.2 90/07/26 12:37:33 dpj Exp $";
#endif	lint

#if	MACH3_US || MACH3_SA

#include	"setjmp.h"

int setjmp(env)
	jmp_buf		env;
{
	return(_setjmp(env));
}

void longjmp(env,val)
	jmp_buf		env;
	int		val;
{
	_longjmp(env,val);
	return;
}

#endif	MACH3_US || MACH3_SA
