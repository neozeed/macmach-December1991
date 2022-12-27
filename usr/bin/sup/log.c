/*
 * Logging support for SUP
 **********************************************************************
 * HISTORY
 * $Log:	log.c,v $
 * Revision 1.4  89/11/29  17:40:24  bww
 * 	Updated variable argument list usage.
 * 	[89/11/29  17:27:28  bww]
 * 
 * Revision 1.3  89/08/15  15:30:37  bww
 * 	Updated to use v*printf() in place of _doprnt().
 * 	From "[89/04/19            mja]" at CMU.
 * 	[89/08/15            bww]
 * 
 * 27-Dec-87  Glenn Marcy (gm0w) at Carnegie-Mellon University
 *	Added check to allow logopen() to be called multiple times.
 *
 * 20-May-87  Glenn Marcy (gm0w) at Carnegie-Mellon University
 *	Created.
 *
 **********************************************************************
 */

#include <stdio.h>
#include <sys/syslog.h>
#include <c.h>
#if __STDC__
#include <stdarg.h>
#else
#include <varargs.h>
#endif
#include "sup.h"

static int opened = 0;

logopen(program)
char *program;
{
	if (opened)  return;
	openlog(program,LOG_PID,LOG_DAEMON);
	opened++;
}

#if __STDC__
logquit(int retval,char *fmt,...)
#else
/*VARARGS*/
logquit(va_alist)
va_dcl
#endif
{
#if !__STDC__
	int retval;
	char *fmt;
#endif
	char buf[STRINGLENGTH];
	va_list ap;

#if __STDC__
	va_start(ap,fmt);
#else
	va_start(ap);
	retval = va_arg(ap,int);
	fmt = va_arg(ap,char *);
#endif
	vsnprintf(buf, sizeof(buf), fmt, ap);
	va_end(ap);
	if (opened) {
		syslog (LOG_ERR,buf);
		closelog ();
		exit (retval);
	}
	quit (retval,"SUP: %s\n",buf);
}

#if __STDC__
logerr(char *fmt,...)
#else
/*VARARGS*/
logerr(va_alist)
va_dcl
#endif
{
#if !__STDC__
	char *fmt;
#endif
	char buf[STRINGLENGTH];
	va_list ap;

#if __STDC__
	va_start(ap,fmt);
#else
	va_start(ap);
	fmt = va_arg(ap,char *);
#endif
	vsnprintf(buf, sizeof(buf), fmt, ap);
	va_end(ap);
	if (opened) {
		syslog (LOG_ERR,buf);
		return;
	}
	fprintf (stderr,"SUP: %s\n",buf);
	(void) fflush (stderr);
}

#if __STDC__
loginfo(char *fmt,...)
#else
/*VARARGS*/
loginfo(va_alist)
va_dcl
#endif
{
#if !__STDC__
	char *fmt;
#endif
	char buf[STRINGLENGTH];
	va_list ap;

#if __STDC__
	va_start(ap,fmt);
#else
	va_start(ap);
	fmt = va_arg(ap,char *);
#endif
	vsnprintf(buf, sizeof(buf), fmt, ap);
	va_end(ap);
	if (opened) {
		syslog (LOG_INFO,buf);
		return;
	}
	printf ("%s\n",buf);
	(void) fflush (stdout);
}
