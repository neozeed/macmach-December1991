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
static char sccsid[] = "@(#)opendir.c	5.3 (Berkeley) 6/18/88";
#endif /* LIBC_SCCS and not lint */

#include <sys/param.h>
#include <sys/dir.h>
#include <sys/stat.h>
#include <sys/file.h>
#include <errno.h>
extern int errno;

/*
 * open a directory.
 */
#if __STDC__
DIR *opendir(const char *name)
#else
DIR *
opendir(name)
	char *name;
#endif
{
	register DIR *dirp;
	register int fd;
	struct stat st;
	int pid;

	if ((fd = open(name, 0)) == -1)
		return NULL;
	(void) fcntl(fd, F_GETOWN, &pid);
	errno = 0;
	if (fstat(fd, &st) < 0 || (st.st_mode&S_IFMT) != S_IFDIR) {
		close (fd);
		errno = ENOTDIR;
		return NULL;
	}
	if ((dirp = (DIR *)malloc(sizeof(DIR))) == NULL) {
		close (fd);
		return NULL;
	}
	dirp->dd_bufsiz = MAX(st.st_blksize, MAXBSIZE);
	if ((dirp->dd_buf = (char *)malloc(dirp->dd_bufsiz)) == NULL) {
		(void) free((char *)dirp);
		close (fd);
		return NULL;
	}
	dirp->dd_fd = fd;
	dirp->dd_loc = 0;
	return dirp;
}
