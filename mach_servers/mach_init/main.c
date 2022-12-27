/*
 * Mach Operating System
 * Copyright (c) 1991,1990,1989,1988,1987 Carnegie Mellon University
 * All Rights Reserved.
 * 
 * Permission to use, copy, modify and distribute this software and its
 * documentation is hereby granted, provided that both the copyright
 * notice and this permission notice appear in all copies of the
 * software, derivative works or modified versions, and any portions
 * thereof, and that both notices appear in supporting documentation.
 * 
 * CARNEGIE MELLON ALLOWS FREE USE OF THIS SOFTWARE IN ITS 
 * CONDITION.  CARNEGIE MELLON DISCLAIMS ANY LIABILITY OF ANY KIND FOR
 * ANY DAMAGES WHATSOEVER RESULTING FROM THE USE OF THIS SOFTWARE.
 * 
 * Carnegie Mellon requests users of this software to return to
 * 
 *  Software Distribution Coordinator  or  Software.Distribution@CS.CMU.EDU
 *  School of Computer Science
 *  Carnegie Mellon University
 *  Pittsburgh PA 15213-3890
 * 
 * any improvements or extensions that they make and grant Carnegie the
 * rights to redistribute these changes.
 */
/*
 * HISTORY
 * $Log:	main.c,v $
 * Revision 2.5  91/08/29  15:50:26  rpd
 * 	Changed -n mode so that mach_init doesn't background itself.
 * 	Added explicit calls to cthread_fork_{prepare,parent,child}.
 * 	[91/08/23            rpd]
 * 
 * Revision 2.4  91/03/27  17:35:57  mrt
 * 	Changed mach.h include
 * 
 * Revision 2.3  91/03/19  11:26:54  mrt
 * 	Changed to new copyright
 * 
 * Revision 2.2  90/09/12  16:35:01  rpd
 * 	Updated for Mach 3.0.
 * 	[90/09/11            rpd]
 * 
 * Revision 1.2  89/04/29  13:34:30  mrt
 * 	24-Apr-87  Mary Thompson
 * 
 *	Added a few more items to the debuging write statements
 *
 * 31-Mar-87  Michael Young (mwyoung) at Carnegie-Mellon University
 *	Make no assumptions about how ports are enabled/disabled when
 *	created... forcibly set them as desired.
 *
 * 10-Feb-87  Mary Thompson, Michael Young
 *	Removed some bogus error statements, finessed call to port_status
 *	to work with old, wrong kernel interface.
 *
 *  4-Jan-87  Michael Young (mwyoung) at Carnegie-Mellon University
 *	Fixed the call to port_status in previous fix.
 *	Added some more sanity checks.
 *	Delay opening of /dev/console until actually needed.
 *	Pass arguments given us on to /etc/init.
 *	Remove conditionals on SERVICE_SLOT.
 *
 *  6-Nov-86  Michael Young (mwyoung) at Carnegie-Mellon University
 *	Modified to alternatively run as process 1; instead of
 *	the parent terminating, it exec's the real /etc/init.
 *
 *  2-Oct-86  Michael Young (mwyoung) at Carnegie-Mellon University
 *	Created.
 */
/*
 *	Program:	Service server
 *
 *	Purpose:
 *		Create ports for globally interesting services,
 *		and hand the receive rights to those ports (i.e.
 *		the ability to serve them) to whoever asks.
 *
 *	Why we need it:
 *		We need to get the service ports into the
 *		very top of the task inheritance structure,
 *		but the currently available system startup
 *		mechanism doesn't allow the actual servers
 *		to be started up from within the initial task
 *		itself.  We start up as soon as we can, and
 *		force the service ports back up the task tree,
 *		and let servers come along later to handle them.
 *
 *		In the event that a server dies, a new instantiation
 *		can grab the same service port.
 *
 *	Can be run before /etc/init instead, if desired.
 */

#include <stdio.h>
#include <strings.h>
#include <ctype.h>
#include <varargs.h>
#include <sys/signal.h>
#include <mach.h>

extern int errno;

extern void serv_init();
extern void serv_loop();

int sig_alarm();
int sig_term();

FILE *error_stream();

char *program_name;

boolean_t signalled = FALSE;

#ifdef	DEBUG
boolean_t debug = TRUE;
boolean_t verbose = TRUE;
#else	DEBUG
boolean_t debug = FALSE;
boolean_t verbose = FALSE;
#endif	DEBUG

boolean_t exec_init = TRUE;

int
main(argc, argv, envp)
	int argc;
	char *argv[];
	char *envp[];
{
	int i;

	setbuf(stdout, NULL);

	program_name = rindex(argv[0], '/');
	if (program_name == NULL)
		program_name = argv[0];
	else
		program_name++;

	for (i = 1; i < argc; i++) {
		char *pp;

		for (pp = argv[i]; *pp != '\0'; pp++)
			switch (*pp) {
				case 'n':
				case 'N':
					exec_init = isupper(*pp);
					break;

				case 'd':
				case 'D':
					debug = islower(*pp);
					break;

				case 'v':
				case 'V':
					verbose = islower(*pp);
					break;
			}
	}

	if (exec_init) {
		task_t parent_task;
		int parent_pid;
		int wakeup_pid;

		wakeup_pid = getpid();
		parent_pid = (wakeup_pid == 1) ? 1 : getppid();

		(void) signal(SIGTERM, sig_term);
		(void) signal(SIGALRM, sig_alarm);

		if (debug)
			(void) error_stream();

		/*
		 *	Create a child process which will actually
		 *	do the servicing; the parent will exit (to
		 *	indicate that the registration is done), or
		 *	run /etc/init (if done as part of system startup).
		 *
		 *	The parent must be the task that exec's /etc/init,
		 *	because /etc/init must have pid 1.  We don't want
		 *	the parent to exec /etc/init until the ports are
		 *	registered.  It would be easiest to allocate and
		 *	register the ports before forking, but this would
		 *	leave the parent with receive rights and it's awkward
		 *	to get the receive rights to the child.  So:
		 *	the parent pauses, waiting (up to 60 secs) for a signal
		 *	from the child that it OK to go ahead.  The child
		 *	initializes and then signals the parent.
		 */

		cthread_fork_prepare();
		if (fork()) {
			cthread_fork_parent();

			alarm(60);
			if (!signalled)
				pause();
			if (debug)
				printf("Parent terminating.\n");

			fflush(stdout);
			fflush(stderr);

			if (argv[0] != NULL)
				argv[0] = "init";
			execve("/etc/init", argv, envp);

			if (verbose)
				fprintf(error_stream(),
					"%s: exec /etc/init failed\n");
			return(errno);
		}
		cthread_fork_child();

		/*
		 *	Make ourselves an "init" process, so we
		 *	won't be known to (or killed by) another init process.
		 */

		(void) init_process();

		/*
		 *	Get our original parent's task port.  If we
		 *	were spawned directly as an init process, so
		 *	we don't have an original parent, this will be
		 *	our current parent (init).
		 */

		parent_task = task_by_pid(parent_pid);
		serv_init(parent_task);

		/*
		 *	Wake up the parent.
		 */

		if (debug)
			printf("Registered\n");
		kill(wakeup_pid, SIGTERM);

		if (verbose)
			printf("%s: child alive, pid %d\n",
			       program_name, getpid());

		serv_loop();
	} else {
		/*
		 *	Running as a normal program.
		 */

		serv_init(task_by_pid(getppid()));
		serv_loop();
	}
}

int
sig_alarm()
{
	signalled = TRUE;
}

int
sig_term()
{
	signalled = TRUE;
}

FILE *
error_stream()
{
	static boolean_t io_init = FALSE;

	/*
	 *	If we're the initial process, we must first open
	 *	somewhere to write error messages.
	 */

	if (exec_init && !io_init) {
	    	FILE *f;

		freopen("/.mach_init.log", "w", stdin);
		setbuf(stdin, NULL);
		freopen("/.mach_init.log", "w", stdout);
		setbuf(stdout, NULL);
		freopen("/.mach_init.log", "w", stderr);
		setbuf(stdout, NULL);
		io_init = TRUE;
	}
	return(stderr);
}
