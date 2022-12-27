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
static char sccsid[] = "@(#)readdir.c	5.4 (Berkeley) 6/18/88";
#endif /* LIBC_SCCS and not lint */

/*
 **********************************************************************
 * HISTORY
 * $Log:	readdir.c,v $
 * Revision 2.2  89/07/22  12:23:42  bww
 * 	Added code to use a dynamically allocated buffer for
 * 	entries.  Changed to use getdirentries instead of read.
 * 	From "[89/05/14            gm0w]" at CMU.
 * 	[89/07/21            bww]
 * 
 **********************************************************************
 */
#include <sys/param.h>
#include <sys/dir.h>

/*
 * get next entry in a directory.
 */
struct direct *
readdir(dirp)
	register DIR *dirp;
{
	register struct direct *dp;
	long offset;

	if (dirp == NULL || dirp->dd_buf == NULL)
		return NULL;
	for (;;) {
		if (dirp->dd_loc == 0) {
			dirp->dd_size = getdirentries(dirp->dd_fd,
						      dirp->dd_buf, 
						      dirp->dd_bufsiz,
						      &offset);
			if (dirp->dd_size <= 0)
				return NULL;
		}
		if (dirp->dd_loc >= dirp->dd_size) {
			dirp->dd_loc = 0;
			continue;
		}
		dp = (struct direct *)(dirp->dd_buf + dirp->dd_loc);
		if ((int)dp & 03)	/* bogus pointer check */
			return NULL;
		if (dp->d_reclen <= 0 ||
		    dp->d_reclen > dirp->dd_bufsiz + 1 - dirp->dd_loc)
			return NULL;
		dirp->dd_loc += dp->d_reclen;
		if (dp->d_ino == 0)
			continue;
		return (dp);
	}
}
