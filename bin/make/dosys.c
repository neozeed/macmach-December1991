#ifndef lint
static char *RCSid = "$Id: dosys.c,v 1.7 90/05/07 15:58:36 bww Exp $";
#endif

/*
 * Copyright (c) 1989 Carnegie-Mellon University
 * All rights reserved.  The CMU software License Agreement specifies
 * the terms and conditions for use and redistribution.
 ************************************************************************
 * HISTORY
 * $Log:	dosys.c,v $
 * Revision 5.1  90/08/06  18:20:39  mrt
 * 	Picked up bww's changes from Mt Xinu
 * 	[90/08/06  17:23:33  mrt]
 * 
 * Revision 1.7  90/05/07  15:58:36  bww
 * 	fatal() now accepts printf-style arguments.
 * 	[90/05/07  15:50:27  bww]
 * 
 * Revision 1.6  90/05/07  12:40:59  bww
 * 	Disable unlinking of targets upon interrupt for recursive
 * 	makes (as indicated by "exok") -- the sub-make will handle
 * 	it.  Always quit() on receipt of a signal -- "-i" and "-k"
 * 	do not apply.
 * 	[90/05/07  12:32:26  bww]
 * 
 * Revision 1.5  90/04/30  15:20:18  bww
 * 	Added forward declarations of static functions.
 * 	[90/04/30  15:07:17  bww]
 * 
 * Revision 1.4  90/04/25  19:12:31  bww
 * 	Avoid changing any data in forked children.
 * 	[90/04/25  19:12:05  bww]
 * 
 * Revision 1.3  90/02/01  20:47:45  bww
 * 	Delinted.
 * 	[90/02/01  20:34:17  bww]
 * 
 * Revision 1.2  89/05/26  10:06:38  bww
 * 	CMU CS as of 89/05/15
 * 	[89/05/26  09:46:50  bww]
 * 
 * Revision 5.0  89/03/01  01:37:47  bww
 * 	Version 5.
 * 	[89/03/01  01:29:05  bww]
 * 
 */

#ifndef lint
static	char *sccsid = "@(#)dosys.c	4.11 (Berkeley) 11/15/87";
#endif

#include "defs.h"
#include <signal.h>
#include <sys/time.h>
#include <sys/wait.h>
#ifndef sigmask
#define sigmask(m)	(1 << ((m)-1))
#endif

FSTATIC int waitpid = 0;
FSTATIC int waitmsk = 0;
FSTATIC doshell(), doexec(), shexec();
FSTATIC int await();


int
dosys(comm, nohalt, exok)
	register char *comm;
	int nohalt, exok;
{
	register int status;
	int onoexflag;

	if (metas(comm))
		doshell(comm, nohalt);
	else
		doexec(comm);
	waitmsk = sigblock(sigmask(SIGHUP)|sigmask(SIGINT)|sigmask(SIGQUIT));
	status = await(nohalt);
	if (exok) {
		onoexflag = noexflag;
		noexflag = YES;
	}
	(void) sigsetmask(waitmsk);
	if (exok)
		noexflag = onoexflag;
	return status;
}


/*
 * Are there are any Shell meta-characters?
 */
int
metas(s)
	register char *s;
{
	register char c;

	while ((funny[c = *s++] & META) == 0)
		;
	return c;
}


FSTATIC
doshell(comm, nohalt)
	char *comm;
	int nohalt;
{
	(void) fflush(stdout);
	(void) fflush(stderr);
	switch (waitpid = vfork()) {
	case -1:
		fatal("Cannot fork");
	case 0:
		enbint(SIG_DFL);
		shexec(comm, nohalt);
	}
}


#define MAXARGV	1000

FSTATIC
doexec(comm)
	register char *comm;
{
	register char *t;
	register char **p;
	char *argv[MAXARGV];

	while (isspace(*comm))
		++comm;
	if (*comm == 0)
		fatal("no command");

	p = argv;
	for (t = comm; *t; ) {
		if (p >= argv + MAXARGV)
			fatal("%s: Too many arguments", comm);
		*p++ = t;
		while (*t && !isspace(*t))
			++t;
		if (*t)
			for (*t++ = 0; isspace(*t); ++t)
				;
	}
	*p = 0;

	(void) fflush(stdout);
	(void) fflush(stderr);
	switch (waitpid = vfork()) {
	case -1:
		fatal("Cannot fork");
	case 0:
		enbint(SIG_DFL);
		execvp(*argv, argv);
		perror(*argv);
		_exit(1);
	}
}


FSTATIC
shexec(comm, nohalt)
	char *comm;
	int nohalt;
{
#ifdef SHELLENV
	char *shellcom, *shellstr;
	extern char *getenv();

	if ((shellcom = getenv("SHELL")) == 0)
		shellcom = SHELLCOM;
	if ((shellstr = rindex(shellcom, '/')) == 0)
		shellstr = shellcom;
	else
		shellstr += 1;
	execl(shellcom, shellstr, nohalt ? "-c" : "-ce", comm, (char *)0);
	perror(shellcom);
#else
	execl(SHELLCOM, "sh", nohalt ? "-c" : "-ce", comm, (char *)0);
	perror(SHELLCOM);
#endif
	_exit(1);
}


FSTATIC int
await(nohalt)
	int nohalt;
{
	register int pid;
	union wait status;
	unsigned int sig;
	extern char *sys_siglist[];

	while ((pid = wait(&status)) != waitpid)
		if (pid == -1)
			fatal("bad wait code");
	waitpid = 0;
	if (status.w_status) {
		if (sig = status.w_termsig) {
			if (sig < NSIG && sys_siglist[sig] && *sys_siglist[sig])
				printf("*** %s", sys_siglist[sig]);
			else
				printf("*** Signal %u", sig);
			if (status.w_coredump)
				printf(" - core dumped");
			printf("\n");
			(void) fflush(stdout);
			(void) sigsetmask(waitmsk);
			quit((int) sig);
		}
		printf("*** Exit %u", status.w_retcode);
		if (nohalt)
			printf(" (ignored)\n");
		else if (keepgoing)
			printf("\n");
		else
			fatal((char *) 0);
		(void) fflush(stdout);
	}
	return status.w_status;
}


touch(force, name)
	int force;
	char *name;
{
	register int fd;
	struct stat stbuff;
	char junk[1];
	extern long lseek();

	if (stat(name, &stbuff) == -1) {
		if (force)
			goto create;
		perror(name);
		return;
	}
	if (stbuff.st_size == 0)
		goto create;

	if ((fd = open(name, 2)) == -1)
		goto bad;

	if (read(fd, junk, 1) != 1
	|| lseek(fd, 0L, 0) != 0L
	|| write(fd, junk, 1) != 1) {
		(void) close(fd);
		goto bad;
	}
	(void) close(fd);
	return;

bad:
	perror(name);
	return;

create:
	if ((fd = creat(name, 0666)) == -1)
		goto bad;
	(void) close(fd);
}


FILE *
pfopen(comm, nohalt)
	char *comm;
	int nohalt;
{
	FILE *f;
	int fds[2];

	if (pipe(fds) == -1)
		fatal("Cannot make pipe");
	(void) fflush(stdout);
	(void) fflush(stderr);
	switch (waitpid = vfork()) {
	case -1:
		fatal("Cannot fork");
	case 0:
		enbint(SIG_DFL);
		(void) close(fds[0]);
		(void) dup2(fds[1], 1);
		(void) close(fds[1]);
		shexec(comm, nohalt);
	}
	(void) close(fds[1]);
	if ((f = fdopen(fds[0], "r")) == NULL)
		fatal("Cannot allocate file structure");
	waitmsk = sigblock(sigmask(SIGHUP)|sigmask(SIGINT)|sigmask(SIGQUIT));
	return f;
}


int
pfclose(f, nohalt)
	FILE *f;
	int nohalt;
{
	register int status;

	(void) fclose(f);
	status = await(nohalt);
	(void) sigsetmask(waitmsk);
	return status;
}
