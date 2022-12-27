/* 
 * Mach Operating System
 * Copyright (c) 1989 Carnegie-Mellon University
 * All rights reserved.  The CMU software License Agreement specifies
 * the terms and conditions for use and redistribution.
 */
/*
 * HISTORY
 * $Log:	error.c,v $
 * Revision 1.1  90/02/19  01:14:33  bww
 * 	Mach Release 2.5
 * 	[90/02/19  01:14:23  bww]
 * 
 * Revision 1.6  89/08/14  21:01:45  mrt
 * 	Changed _doprntf to vfprintf
 * 	[89/08/14            mrt]
 * 
 * Revision 1.5  89/05/20  22:14:35  mrt
 * 	Extensive revamping.  Added polymorphic arguments.
 * 	Allow multiple variable-sized inline arguments in messages.
 * 	[89/04/07            rpd]
 * 
 *
 * 28-May-87  Richard Draves (rpd) at Carnegie-Mellon University
 *	Created.
 */
/*
 * ABSTRACT:
 *	Routines to print out error messages.
 *  Exports variables;
 *	errors - the total number of errors encountered
 *  Exports routines:
 *	fatal, warn, error, unix_error_string, set_program_name
 */

#include <stdio.h>
#include <varargs.h>
#include "global.h"
#include "error.h"

extern int yylineno;
extern char *yyinname;

static char *program;
int errors = 0;

/*ARGSUSED*/
/*VARARGS1*/
void
fatal(format, va_alist)
    char *format;
    va_dcl
{
    va_list pvar;
    va_start(pvar);
    fprintf(stderr, "%s: fatal: ", program);
    vfprintf(stderr, format, pvar);
    fprintf(stderr, "\n");
    va_end(pvar);
    exit(1);
}

/*ARGSUSED*/
/*VARARGS1*/
void
warn(format, va_alist)
    char *format;
    va_dcl
{
    va_list pvar;
    va_start(pvar);
    if (!BeQuiet && (errors == 0))
    {
	fprintf(stderr, "\"%s\", line %d: warning: ", yyinname, yylineno-1);
	vfprintf(stderr, format, pvar);
	fprintf(stderr, "\n");
    }
    va_end(pvar);
}

/*ARGSUSED*/
/*VARARGS1*/
void
error(format, va_alist)
    char *format;
    va_dcl
{
    va_list pvar;
    va_start(pvar);
    fprintf(stderr, "\"%s\", line %d: ", yyinname, yylineno-1);
    vfprintf(stderr, format, pvar);
    fprintf(stderr, "\n");
    va_end(pvar);
    errors++;
}

char *
unix_error_string(errno)
    int errno;
{
    extern int sys_nerr;
    extern char *sys_errlist[];
    static char buffer[256];
    char *error_mess;

    if ((0 <= errno) && (errno < sys_nerr))
	error_mess = sys_errlist[errno];
    else
	error_mess = "strange errno";

    sprintf(buffer, "%s (%d)", error_mess, errno);
    return buffer;
}

void
set_program_name(name)
    char *name;
{
    program = name;
}
