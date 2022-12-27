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
static char sccsid[] = "@(#)perror.c	5.7 (Berkeley) 12/16/88";
#endif /* LIBC_SCCS and not lint */

#include <sys/types.h>
#include <sys/uio.h>

int errno;
extern int sys_nerr;
extern char *sys_errlist[];

static char ebuf[20];

perror(s)
	char *s;
{
	struct iovec iov[4];
	register struct iovec *v = iov;

	if (s && *s) {
		v->iov_base = s;
		v->iov_len = strlen(s);
		v++;
		v->iov_base = ": ";
		v->iov_len = 2;
		v++;
	}
	if ((u_int)errno < sys_nerr)
		v->iov_base = sys_errlist[errno];
	else {
		(void)sprintf(ebuf, "Unknown error: %d", errno);
		v->iov_base = ebuf;
	}
	v->iov_len = strlen(v->iov_base);
	v++;
	v->iov_base = "\n";
	v->iov_len = 1;
	(void)writev(2, iov, (v - iov) + 1);
}

char *
strerror(errnum)
	int errnum;
{
	if ((u_int)errnum < sys_nerr)
		return(sys_errlist[errnum]);
	(void)sprintf(ebuf, "Unknown error: %d", errnum);
	return(ebuf);
}
