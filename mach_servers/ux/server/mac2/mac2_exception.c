/* 
 * Mach Operating System
 * Copyright (c) 1990 Carnegie-Mellon University
 * Copyright (c) 1989 Carnegie-Mellon University
 * Copyright (c) 1988 Carnegie-Mellon University
 * All rights reserved.  The CMU software License Agreement specifies
 * the terms and conditions for use and redistribution.
 */
/*
 * HISTORY
 * $Log:	mac2_exception.c,v $
 * Revision 2.2  90/08/30  11:01:46  bohman
 * 	Created.
 * 	[90/08/29  11:38:05  bohman]
 * 
 */

/*
 *	Apple Macintosh II Mach (macmach)
 *
 *	File: mac2/mac2_exception.c
 */

#include <sys/signal.h>

#include <mach/boolean.h>
#include <mach/exception.h>
#include <mach/kern_return.h>

/*
 * machine_exception translates a mach exception to a unix exception
 * and code.  unix_exception() handles the machine-independent ones.
 */
boolean_t
machine_exception(exception, code, subcode, unix_signal, unix_code)
register exception, code, subcode;
int *unix_signal, *unix_code;
{
    switch(exception) {
      case EXC_BAD_INSTRUCTION:
	if (code == EXC_MAC2_FLT_BSUN ||
	    code == EXC_MAC2_FLT_OPERAND_ERROR)
	    *unix_signal = SIGFPE;
	else
	    *unix_signal = SIGILL;
	break;
	
      case EXC_ARITHMETIC:
	*unix_signal = SIGFPE;
	break;
	
      case EXC_EMULATION:
	*unix_signal = SIGEMT;
	break;
	
      case EXC_SOFTWARE:
	if (code != EXC_MAC2_CHK && code != EXC_MAC2_TRAPV)
	    return(FALSE);
	*unix_signal = SIGFPE;
	break;
	
      case EXC_BREAKPOINT:
	*unix_signal = SIGTRAP;
	break;
	
      default:
	return(FALSE);
    }

    *unix_code = code;

    return(TRUE);
}
