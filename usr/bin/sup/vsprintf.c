/*
 * Copyright (c) 1988 Regents of the University of California.
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms are permitted
 * provided that the above copyright notice and this paragraph are
 * duplicated in all such forms and that any documentation,
 * advertising materials, and other materials related to such
 * distribution and use acknowledge that the software was developed
 * by the University of California, Berkeley.  The name of the
 * University may not be used to endorse or promote products derived
 * from this software without specific prior written permission.
 * THIS SOFTWARE IS PROVIDED ``AS IS'' AND WITHOUT ANY EXPRESS OR
 * IMPLIED WARRANTIES, INCLUDING, WITHOUT LIMITATION, THE IMPLIED
 * WARRANTIES OF MERCHANTIBILITY AND FITNESS FOR A PARTICULAR PURPOSE.
 */

#if defined(LIBC_SCCS) && !defined(lint)
static char sccsid[] = "@(#)vsprintf.c	5.2 (Berkeley) 6/27/88";
#endif /* LIBC_SCCS and not lint */

/*
 **********************************************************************
 * HISTORY
 * $Log:	vsprintf.c,v $
 * Revision 2.3  90/03/23  16:43:55  bww
 * 	Always return the actual number of characters in the string
 * 	(or EOF on overflow) rather than how many _doprnt() thought
 * 	it added.
 * 	[90/03/23  16:43:15  bww]
 * 
 * Revision 2.2  89/08/14  14:19:10  bww
 * 	No new architectures make _doprnt() visible.
 * 	Also added vsnprintf().
 * 	[89/08/14  14:16:49  bww]
 * 
 **********************************************************************
 */
#include <stdio.h>
#include <varargs.h>

#if	!defined(vax) && !defined(sun3) && !defined(ibmrt)
/* 
 *  No new architectures make _doprnt() visible.
 */
#define	_doprnt	_doprnt_va
#endif

int
vsprintf(str, fmt, ap)
	char *str, *fmt;
	va_list ap;
{
	FILE f;

	f._flag = _IOWRT+_IOSTRG;
	f._ptr = str;
	f._cnt = 32767;  /* XXX */
	(void) _doprnt(fmt, ap, &f);
	*f._ptr = 0;
	return (ferror(&f) ? EOF : f._ptr - str);
}

int
vsnprintf(str, n, fmt, ap)
	char *str, *fmt;
	va_list ap;
{
	FILE f;

	f._flag = _IOWRT+_IOSTRG;
	f._ptr = str;
	f._cnt = n-1;
	(void) _doprnt(fmt, ap, &f);
	*f._ptr = 0;
	return (ferror(&f) ? EOF : f._ptr - str);
}
