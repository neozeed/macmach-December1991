#ifndef lint
static char *rcsid_xinit_c = "$XConsortium: xinit.c,v 11.41 89/12/15 18:16:49 rws Exp $";
#endif /* lint */
#include <X11/copyright.h>

/* Copyright    Massachusetts Institute of Technology    1986	*/

#include <X11/Xos.h>
#include <X11/Xlib.h>
#include <X11/Xmu/SysUtil.h>
#include <stdio.h>
#include <ctype.h>
#include <signal.h>
#ifndef SYSV
#include <sys/wait.h>
#endif
#include <errno.h>
extern int sys_nerr;
#include <setjmp.h>

extern char *getenv();
extern char **environ;
char **newenviron = NULL;

#ifndef SHELL
#define SHELL "sh"
#endif

#ifdef macII
#define vfork() fork()
#endif /* macII */

#if defined(SYSV) && !defined(hpux)
#define vfork() fork()
#endif /* SYSV and not hpux */

char *bindir = BINDIR;
char *server_names[] = {
#ifdef vax				/* Digital */
    "Xqvss       Digital monochrome display on Microvax or VAXstation",
    "Xqdss       Digital color display on Microvax of VAXstation",
#endif
#if defined(ultrix) && defined(mips)
    "Xmfbpmax    Digital monochrome display on DECstation 3100",
    "Xcfbpmax    Digital color display on DECstation 3100",
#endif
#ifdef sun				/* Sun */
    "Xsun        Sun monochrome and color displays on Sun 2, 3, or 4 series",
#endif
#ifdef hpux				/* HP */
    "Xhp         HP monochrome and colors displays on 9000/300 series",
#endif
#ifdef apollo				/* Apollo */
    "Xapollo     Apollo monochrome and color displays",
#endif
#ifdef ibm				/* IBM */
    "Xibm        IBM AED, APA, 8514a, megapel, VGA displays on PC/RT",
#endif
#ifdef macII				/* MacII */
    "XmacII      Apple monochrome display on Macintosh II",
#endif
#ifdef mac2
    "Xmacmach    Apple monochrome and color displays on Macintosh II running MacMach",
#endif
#ifdef M4310				/* Tektronix Pegasus */
    "Xpeg        Tektronix Pegasus display on 431x series",
#endif
    NULL};

#ifndef XINITRC
#define XINITRC ".xinitrc"
#endif
char xinitrcbuf[256];

#ifndef XSERVERRC
#define XSERVERRC ".xserverrc"
#endif
char xserverrcbuf[256];

#define	TRUE		1
#define	FALSE		0
#define	OK_EXIT		0
#define	ERR_EXIT	1
char displayname[100] = "unix";
char client_display[100];

char *default_server = "X";
char *default_display = ":0";		/* choose most efficient */
char *default_client[] = {"xterm", "-geometry", "+1+1", "-n", "login", "-display", NULL};
char *serverargv[100];
char *clientargv[100];
char **server = serverargv + 2;		/* make sure room for sh .xserverrc args */
char **client = clientargv + 2;		/* make sure room for sh .xinitrc args */
char *displayNum;
char *program;
Display *xd;			/* server connection */
#ifndef SYSV
union wait	status;
#endif /* SYSV */
int serverpid = -1;
int clientpid = -1;
extern int	errno;

static shutdown();

sigCatch(sig)
	int	sig;
{
	signal(SIGQUIT, SIG_IGN);
	signal(SIGINT, SIG_IGN);
	signal(SIGHUP, SIG_IGN);
	signal(SIGPIPE, SIG_IGN);
	Error("unexpected signal %d\r\n", sig);
	shutdown(serverpid, clientpid);
	exit(1);
}

sigAlarm(sig)
	int sig;
{
#ifdef SYSV
	signal (sig, sigAlarm);
#endif
}

void
sigUsr1(sig)
	int sig;
{
#ifdef SYSV
	signal (sig, sigUsr1);
#endif
}

static Execute (vec)
    char **vec;				/* has room from up above */
{
    execvp (vec[0], vec);
    if (access (vec[0], R_OK) == 0) {
	vec--;				/* back it up to stuff shell in */
	vec[0] = SHELL;
	execvp (vec[0], vec);
    }
    return;
}

main(argc, argv)
int argc;
register char **argv;
{
	register char **sptr = server;
	register char **cptr = client;
	register char **ptr;
	int pid;
	int client_given = 0, server_given = 0;
	int client_args_given = 0, server_args_given = 0;
	int start_of_client_args, start_of_server_args;

	program = *argv++;
	argc--;

#ifndef UNIXCONN
	(void) XmuGetHostname (displayname, sizeof(displayname));
#endif /* UNIXCONN */
	/*
	 * copy the client args.
	 */
	if (argc == 0 ||
	    (**argv != '/' && **argv != '.')) {
		for (ptr = default_client; *ptr; )
			*cptr++ = *ptr++;
		strcpy(client_display, displayname);
		strcat(client_display, default_display);
		*cptr++ = client_display;
#ifdef sun
		/* 
		 * If running on a sun, and if WINDOW_PARENT isn't defined, 
		 * that means SunWindows isn't running, so we should pass 
		 * the -C flag to xterm so that it sets up a console.
		 */
		if ( getenv("WINDOW_PARENT") == NULL )
		    *cptr++ = "-C";
#endif /* sun */
	} else {
		client_given = 1;
	}
	start_of_client_args = (cptr - client);
	while (argc && strcmp(*argv, "--")) {
		client_args_given++;
		*cptr++ = *argv++;
		argc--;
	}
	*cptr = NULL;
	if (argc) {
		argv++;
		argc--;
	}

	/*
	 * Copy the server args.
	 */
	if (argc == 0 ||
	    (**argv != '/' && **argv != '.')) {
		*sptr++ = default_server;
	} else {
		server_given = 1;
		*sptr++ = *argv++;
		argc--;
	}
	if (argc > 0 && (argv[0][0] == ':' && isdigit(argv[0][1])))
		displayNum = *argv;
	else
		displayNum = *sptr++ = default_display;

	start_of_server_args = (sptr - server);
	while (--argc >= 0) {
		server_args_given++;
		*sptr++ = *argv++;
	}
	*sptr = NULL;


	strcat(displayname, displayNum);

	/*
	 * if no client arguments given, check for a startup file and copy
	 * that into the argument list
	 */
	if (!client_given) {
	    char *cp;
	    Bool required = False;

	    xinitrcbuf[0] = '\0';
	    if ((cp = getenv ("XINITRC")) != NULL) {
		strcpy (xinitrcbuf, cp);
		required = True;
	    } else if ((cp = getenv ("HOME")) != NULL) {
		(void) sprintf (xinitrcbuf, "%s/%s", cp, XINITRC);
	    }
	    if (xinitrcbuf[0]) {
		if (access (xinitrcbuf, F_OK) == 0) {
		    client += start_of_client_args - 1;
		    client[0] = xinitrcbuf;
		} else if (required) {
		    fprintf (stderr,
			     "%s:  warning, no client init file \"%s\"\n",
			     program, xinitrcbuf);
		}
	    }
	}

	/*
	 * if no server arguments given, check for a startup file and copy
	 * that into the argument list
	 */
	if (!server_given) {
	    char *cp;
	    Bool required = False;

	    xserverrcbuf[0] = '\0';
	    if ((cp = getenv ("XSERVERRC")) != NULL) {
		strcpy (xserverrcbuf, cp);
		required = True;
	    } else if ((cp = getenv ("HOME")) != NULL) {
		(void) sprintf (xserverrcbuf, "%s/%s", cp, XSERVERRC);
	    }
	    if (xserverrcbuf[0]) {
		if (access (xserverrcbuf, F_OK) == 0) {
		    server += start_of_server_args - 1;
		    server[0] = xserverrcbuf;
		} else if (required) {
		    fprintf (stderr,
			     "%s:  warning, no server init file \"%s\"\n",
			     program, xserverrcbuf);
		}
	    }
	}


	/*
	 * put the display name into the environment
	 */
	set_environment ();

	/*
	 * Start the server and client.
	 */
	signal(SIGQUIT, sigCatch);
	signal(SIGINT, sigCatch);
	signal(SIGHUP, sigCatch);
	signal(SIGPIPE, sigCatch);
	signal(SIGALRM, sigAlarm);
	signal(SIGUSR1, sigUsr1);
	if ((serverpid = startServer(server)) > 0
	 && (clientpid = startClient(client)) > 0) {
		pid = -1;
		while (pid != clientpid && pid != serverpid)
			pid = wait(NULL);
	}
	signal(SIGQUIT, SIG_IGN);
	signal(SIGINT, SIG_IGN);
	signal(SIGHUP, SIG_IGN);
	signal(SIGPIPE, SIG_IGN);

	shutdown(serverpid, clientpid);

	if (serverpid < 0 || clientpid < 0)
		exit(ERR_EXIT);
	exit(OK_EXIT);
}


/*
 *	waitforserver - wait for X server to start up
 */

waitforserver(serverpid)
	int	serverpid;
{
	int	ncycles	 = 120;		/* # of cycles to wait */
	int	cycles;			/* Wait cycle count */

	for (cycles = 0; cycles < ncycles; cycles++) {
		if (xd = XOpenDisplay(displayname)) {
			return(TRUE);
		}
		else {
#define MSG "X server to begin accepting connections"
		    if (!processTimeout (serverpid, 1, MSG)) 
		      break;
#undef MSG
		}
	}

	fprintf (stderr, "giving up.\r\n");
	return(FALSE);
}

/*
 * return TRUE if we timeout waiting for pid to exit, FALSE otherwise.
 */
processTimeout(pid, timeout, string)
	int	pid, timeout;
	char	*string;
{
	int	i = 0, pidfound = -1;
	static char	*laststring;

	for (;;) {
#ifdef SYSV
		alarm(1);
		if ((pidfound = wait(NULL)) == pid)
			break;
		alarm(0);
#else /* SYSV */
		if ((pidfound = wait3(&status, WNOHANG, NULL)) == pid)
			break;
#endif /* SYSV */
		if (timeout) {
			if (i == 0 && string != laststring)
				fprintf(stderr, "\r\nwaiting for %s ", string);
			else
				fprintf(stderr, ".");
			fflush(stderr);
		}
		if (timeout)
			sleep (1);
		if (++i > timeout)
			break;
	}
	if ( i > 0 ) fputc( '\n', stderr );     /* tidy up after message */
	laststring = string;
	return( pid != pidfound );
}

startServer(server)
	char *server[];
{
	int	serverpid;

	serverpid = vfork();
	switch(serverpid) {
	case 0:
		close(0);
		close(1);

		/*
		 * don't hang on read/write to control tty
		 */
#ifdef SIGTTIN
		(void) signal(SIGTTIN, SIG_IGN);
#endif
#ifdef SIGTTOU
		(void) signal(SIGTTOU, SIG_IGN);
#endif
		/*
		 * ignore SIGUSR1 in child.  The server
		 * will notice this and send SIGUSR1 back
		 * at xinit when ready to accept connections
		 */
		(void) signal(SIGUSR1, SIG_IGN);
		/*
		 * prevent server from getting sighup from vhangup()
		 * if client is xterm -L
		 */
		setpgrp(0,getpid());

		Execute (server);
		Error ("no server \"%s\" in PATH\n", server[0]);
		{
		    char **cpp;

		    fprintf (stderr,
"\nUse the -- option, or make sure that %s is in your path and\n",
			     bindir);
		    fprintf (stderr,
"that \"%s\" is a program or a link to the right type of server\n",
			     server[0]);
		    fprintf (stderr,
"for your display.  Possible server names include:\n\n");
		    for (cpp = server_names; *cpp; cpp++) {
			fprintf (stderr, "    %s\n", *cpp);
		    }
		    fprintf (stderr, "\n");
		}
		exit (ERR_EXIT);

		break;
	case -1:
		break;
	default:
		/*
		 * don't nice server
		 */
#ifdef PRIO_PROCESS
		setpriority( PRIO_PROCESS, serverpid, -1 );
#endif

		errno = 0;
		if (! processTimeout(serverpid, 0, "")) {
			serverpid = -1;
			break;
		}
		/*
		 * kludge to avoid race with TCP, giving server time to
		 * set his socket options before we try to open it,
		 * either use the 15 second timeout, or await SIGUSR1.
		 *
		 * If your machine is substantially slower than 15 seconds,
		 * you can easily adjust this value.
		 */
		alarm (15);
		pause ();
		alarm (0);

		if (waitforserver(serverpid) == 0) {
			Error("unable to connect to X server\r\n");
			shutdown(serverpid, -1);
			serverpid = -1;
		}
		break;
	}

	return(serverpid);
}

startClient(client)
	char *client[];
{
	int	clientpid;

	if ((clientpid = vfork()) == 0) {
		setuid(getuid());
		setpgrp(0, getpid());
		environ = newenviron;
		Execute (client);
		Error ("no program named \"%s\" in PATH\r\n", client[0]);
		fprintf (stderr,
"\nSpecify a program on the command line or make sure that %s\r\n", bindir);
		fprintf (stderr,
"is in your path.\r\n");
		fprintf (stderr, "\n");
		exit (ERR_EXIT);
	}
	return (clientpid);
}

#ifdef SYSV
#define killpg(pgrp, sig) kill(-(pgrp), sig)
#endif /* SYSV */

static jmp_buf close_env;

static int ignorexio (dpy)
    Display *dpy;
{
    fprintf (stderr, "%s:  connection to X server lost.\r\n", program);
    longjmp (close_env, 1);
    /*NOTREACHED*/
}

static
shutdown(serverpid, clientpid)
	int	serverpid, clientpid;
{
	/* have kept display opened, so close it now */
	if (clientpid > 0) {
		XSetIOErrorHandler (ignorexio);
		if (! setjmp(close_env)) {
		    XCloseDisplay(xd);
		}

		/* HUP all local clients to allow them to clean up */
		errno = 0;
		if ((killpg(clientpid, SIGHUP) != 0) &&
		    (errno != ESRCH))
			Error("can't send HUP to process group %d\r\n",
				clientpid);
	}

	if (serverpid < 0)
		return;
	errno = 0;
	if (killpg(serverpid, SIGTERM) < 0) {
		if (errno == EPERM)
			Fatal("Can't kill X server\r\n");
		if (errno == ESRCH)
			return;
	}
	if (! processTimeout(serverpid, 10, "X server to shut down")) {
	    fprintf (stderr, "\r\n");
	    return;
	}

	fprintf(stderr, 
	"\r\n%s:  X server slow to shut down, sending KILL signal.\r\n",
		program);
	fflush(stderr);
	errno = 0;
	if (killpg(serverpid, SIGKILL) < 0) {
		if (errno == ESRCH)
			return;
	}
	if (processTimeout(serverpid, 3, "server to die")) {
		fprintf (stderr, "\r\n");
		Fatal("Can't kill server\r\n");
	}
	fprintf (stderr, "\r\n");
	return;
}


/*
 * make a new copy of environment that has room for DISPLAY
 */

set_environment ()
{
    int nenvvars;
    char **newPtr, **oldPtr;
    static char displaybuf[256];

    /* count number of environment variables */
    for (oldPtr = environ; *oldPtr; oldPtr++) ;

    nenvvars = (oldPtr - environ);
    newenviron = (char **) malloc ((nenvvars + 2) * sizeof(char **));
    if (!newenviron) {
	fprintf (stderr,
		 "%s:  unable to allocate %d pointers for environment\n",
		 program, nenvvars + 2);
	exit (1);
    }

    /* put DISPLAY=displayname as first element */
    strcpy (displaybuf, "DISPLAY=");
    strcpy (displaybuf + 8, displayname);
    newPtr = newenviron;
    *newPtr++ = displaybuf;

    /* copy pointers to other variables */
    for (oldPtr = environ; *oldPtr; oldPtr++) {
	if (strncmp (*oldPtr, "DISPLAY=", 8) != 0) {
	    *newPtr++ = *oldPtr;
	}
    }
    *newPtr = NULL;
    return;
}

Fatal(fmt, x0,x1,x2,x3,x4,x5,x6,x7,x8,x9)
	char	*fmt;
{
	Error(fmt, x0,x1,x2,x3,x4,x5,x6,x7,x8,x9);
	exit(ERR_EXIT);
}

Error(fmt, x0,x1,x2,x3,x4,x5,x6,x7,x8,x9)
	char	*fmt;
{
	extern char	*sys_errlist[];

	fprintf(stderr, "%s:  ", program);
	if (errno > 0 && errno < sys_nerr)
	  fprintf (stderr, "%s (errno %d):  ", sys_errlist[errno], errno);
	fprintf(stderr, fmt, x0,x1,x2,x3,x4,x5,x6,x7,x8,x9);
}
