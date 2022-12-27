/*
 * Copyright (c) 1983 Regents of the University of California.
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
static char sccsid[] = "@(#)seekdir.c	5.3 (Berkeley) 6/18/88";
#endif /* LIBC_SCCS and not lint */

/*
 **********************************************************************
 * HISTORY
 * $Log:	seekdir.c,v $
 * Revision 2.2  89/07/22  12:24:21  bww
 * 	Added code to make seekdir difficult to load and impossible
 * 	to use.  From "[89/05/14            gm0w]" at CMU.
 * 	[89/07/21            bww]
 * 
 **********************************************************************
 */
#include <sys/param.h>
#include <sys/dir.h>
#include <stdio.h>
#include <sys/signal.h>

/*
 * seek to an entry in a directory.
 * Only values returned by "telldir" should be passed to seekdir.
 */
void
seekdir(dirp, loc)
	register DIR *dirp;
	long loc;
{
	extern int DONT_USE_SEEKDIR_UNDER_MACH();

	fprintf(stderr, "Don't use seekdir under MACH !!!\n");
	sigblock(~0);
	signal(SIGKILL, SIG_DFL);
	sigsetmask(~sigmask(SIGKILL));
	kill(getpid(), SIGKILL);
	abort();
	DONT_USE_SEEKDIR_UNDER_MACH();
}
