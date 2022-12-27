/*
 *  quit  --  print message and exit
 *
 *  Usage:  quit (status,format [,arg]...);
 *	int status;
 *	(... format and arg[s] make up a printf-arglist)
 *
 *  Quit is a way to easily print an arbitrary message and exit.
 *  It is most useful for error exits from a program:
 *	if (open (...) < 0) then quit (1,"Can't open...",file);
 *
 **********************************************************************
 * HISTORY
 * $Log:	quit.c,v $
 * Revision 1.2  88/12/13  13:52:41  gm0w
 * 	Rewritten to use varargs.
 * 	[88/12/13            gm0w]
 * 
 **********************************************************************
 */

#include <stdio.h>
#include <varargs.h>

quit (status, fmt, va_alist)
int status;
char *fmt;
va_dcl
{
	va_list args;

	fflush(stdout);
	va_start(args);
	(void) vfprintf(stderr, fmt, args);
	va_end(args);
	exit(status);
}
