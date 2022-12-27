/*
 * xconsole
 *
 * $Header:
/afs/athena.mit.edu/astaff/project/xdm/xconsole/RCS/main.c,v 2.0
89/10/31 11:34:37 roman Exp Locker: roman $
 *
 * Copyright 1988 Massachusetts Institute of Technology
 *
 * Permission to use, copy, modify, and distribute this software and its
 * documentation for any purpose and without fee is hereby granted, provided
 * that the above copyright notice appear in all copies and that both that
 * copyright notice and this permission notice appear in supporting
 * documentation, and that the name of M.I.T. not be used in advertising or
 * publicity pertaining to distribution of the software without specific,
 * written prior permission.  M.I.T. makes no representations about the
 * suitability of this software for any purpose.  It is provided "as is"
 * without express or implied warranty.
 *
 * Author:   Roman J. Budzianowski 
 * Created:  July 17 1989
 */

#if ( !defined(lint) && !defined(SABER))
  static char rcs_version[] = "$Header:
/afs/athena.mit.edu/astaff/project/xdm/xconsole/RCS/main.c,v 2.0
89/10/31 11:34:37 roman Exp Locker: roman $";
#endif


#include <stdio.h>
#include <X11/Xos.h>
#include <X11/Xlib.h>
#include <X11/Intrinsic.h>
#include <X11/StringDefs.h>
#include <X11/Xatom.h>
#include <X11/Shell.h>
#include <syslog.h>
#include <signal.h>
#include <sys/file.h>
#include <sys/ioctl.h>
#include <sys/types.h>

#include "console.h"

void SetupTTY();
static void error_status();
static void AbortOtherConsole();
void UnRegisterXcons();
void PrintUsage();
char *getenv();

static XtResource console_resources[] = {
   {"mapped", "Mapped", XtRBoolean, sizeof(Boolean),
      XtOffset(Console,mapped), XtRString, "true"},
   {"debug", "Debug", XtRBoolean, sizeof(Boolean),
      XtOffset(Console,debug), XtRString, "false"},
   {"autoRaise", "AutoRaise", XtRBoolean, sizeof(Boolean),
      XtOffset(Console,autoRaise), XtRString, "true"},
   {"retain", "Retain", XtRBoolean, sizeof(Boolean),
      XtOffset(Console,retain), XtRString, "false"},
   {"doTitle", "DoTitle", XtRBoolean, sizeof(Boolean),
      XtOffset(Console,doTitle), XtRString, "true"},
   {"consoleLog", "ConsoleLog", XtRString, sizeof(String),
      XtOffset(Console,consoleLog), XtRString, "/tmp/console.log"},
};

XrmOptionDescRec command_line_options[] = {
   {"-retain",   "retain",      XrmoptionNoArg,  (caddr_t) "true" },
   {"-log",      "consoleLog",  XrmoptionSepArg,  (caddr_t) NULL },
   {"-debug",    "debug",       XrmoptionNoArg,  (caddr_t) "true" },
   {"-unmapped", "mapped",      XrmoptionNoArg,  (caddr_t) "false" },
};

static struct Xcons_args {
  char * name;			/* name of the argument. */
  char * param;			/* parameter it takes. */
  char * description;		/* description of it. */
} xcons_args[] = {
   {"-retain",    "",                     "retain log of the old console"},
   {"-log",        "<log file>",           "file to log the console output"},
   {"-unmapped",   "",                     "start the console in unmapped state"},
   { "-xrm",       "<resource name>",      "Allows resource specification."},
   { NULL,         NULL,                   NULL },
};

ConsoleRec theConsole;
#define XCONSLOCK "/tmp/xconslock"

Boolean dbg = False;
Boolean debugging = False;
Boolean exiting = False;
Boolean dispatching = False;

/* create a pipe and register one end of it as an input source with the toolkit
   I heard this idea from Ralph Swick - modifying XtNextEvent to call a
function when
   select returns because of signal would be better, but...*/
int signalpipe[2];

/* ARGSUSED */
void HandleSignal(data,src,id)
     caddr_t    data;
     int        *src;
     XtInputId  *id;
{
   Debug("HandleSignal: got signal");
   if(exiting)
     UnRegisterXcons(&theConsole);
}

void KillExit()
{
   char byte = 1;
#ifdef DEBUG
   SystemNotice("xlogin killed by signal");
#endif
   exiting = True;
   write(signalpipe[1],&byte,1);
}


void main(argc,argv)
     int argc;
     char **argv;
{
   Display *dpy;
   Arg args[10];
   char *resdir;
   XrmDatabase xconsoleDB = NULL;

   setuid(0);

   signal(SIGTERM, KillExit);
   signal(SIGHUP, KillExit);
   signal(SIGINT, KillExit);

   umask(0);

   /* Start the toolkit */

   XtToolkitInitialize();

   theConsole.appContext = XtCreateApplicationContext();

   dpy =  XtOpenDisplay(theConsole.appContext, NULL, argv[0], "XConsole",
			command_line_options, XtNumber(command_line_options),
			&argc, argv);

   theConsole.shell = XtAppCreateShell(argv[0],"XConsole",
applicationShellWidgetClass,
                               dpy, args, 0);

   XtGetApplicationResources(theConsole.shell, &theConsole, 
			     console_resources, XtNumber(console_resources),
			     (ArgList)NULL, (Cardinal)0);

   if(argc != 1)
     PrintUsage();
			     
   if(theConsole.debug == True)
     dbg = True;

   if(theConsole.mapped == False)
     XtSetMappedWhenManaged(theConsole.shell,False);

   AbortOtherConsole(&theConsole);
     
   CreateConsole(&theConsole);

   SetupTTY(&theConsole);

   InitializeConsole(&theConsole);

   XtRealizeWidget(theConsole.shell);

   if(pipe(signalpipe)==0)
    
XtAppAddInput(theConsole.appContext,signalpipe[0],XtInputReadMask,HandleSignal,NULL);
   else
     SystemError("xconsole: cannot create pipe for signal handling");
     
   if(exiting)
     UnRegisterXcons(&theConsole);
   while(1){
      XEvent ev;
      XtAppNextEvent(theConsole.appContext,&ev);
      XtDispatchEvent(&ev);
   }
}

void UnRegisterXcons(console)
     Console console;
{
   DestroyConsole(console);
   /*need to close pty ? */
   close(console->pty);
   /* tty ? */

   Debug("Xconsole: exiting killed by signal");
   exit(0);
}


static void AbortOtherConsole(console)
     Console console;
{
   int lock = -1;
   int pid;
   int ret;

   /* check if another console is running */
   if(access(XCONSLOCK,R_OK|W_OK) == -1){ /* for whatever reason cannot
stat lock file */
      Debug("Cannot stat lock file");
      goto setlock;
   }

   lock = open(XCONSLOCK,O_RDWR,0);
   if(lock < 0){
      Debug("Cannot open lock file");
      goto setlock;
   }
   
   ret = read(lock,&pid,sizeof(int));
   
   if(ret != sizeof(int)){	/* couldn't get pid of the other console */
      Debug("Cannot get pid");
      goto setlock;
   } 
   
   /* kill the other guy */
   if(kill(pid,SIGTERM) == -1){
      Debug("Cannot kill other console");
      goto setlock;
   }

   /* retrieve the log file name */
   if(console->retain){
      char buf[BUFSIZ];
      ret = read(lock,buf,BUFSIZ);
      if(ret > 0){
	 buf[ret] = '\0';
	 if(strcmp(buf,console->consoleLog)){
	    char copy[BUFSIZ];
	    sprintf(copy, "cp %s %s",buf, console->consoleLog);
	    if(system(copy)){
	       XtWarning("Cannot copy console log");
	       console->retain = False;
	    }
	 }
      } else
	console->retain = False;
      
      Debug("retain: %s - %s",(console->retain)? "Yes" : "No", console->consoleLog);
   }
   
 setlock:
   error_status();
   if(lock >= 0)
     close(lock);

   lock = open(XCONSLOCK,O_RDWR|O_TRUNC|O_CREAT,0666);
   
   if(lock < 0)
     SystemError("XCons: Cannot open lock file, exiting...");

/*   lseek(lock,(off_t)0,L_SET);*/

   pid = getpid();
   write(lock,&pid,sizeof(int));
   write(lock,console->consoleLog,strlen(console->consoleLog));
   close(lock);
   
   return;
}

   
#define DEVSIZE 16
static char ptydev[DEVSIZE] , ttydev[DEVSIZE];

void SetupTTY(console)
     Console console;
{
  int devconsole,ttyconsole;
  int on = 1;
  int status = -1;

  strcpy(ptydev,"/dev/");
  strcpy(ttydev,"/dev/");

  Debug("opening console\n");

#ifdef ultrix
  if (!debugging) {
     if ((devconsole = open("/dev/xcons", O_RDONLY, 0)) < 0) {
	String str = "xconsole:Couldn't open /dev/xcons";
	SystemNotice(str);
	XtError(str);
     }
     console->terminalOutput =
       XtAppAddInput(console->appContext, devconsole, XtInputReadMask,
		     ReadSession, NULL);
     console->ttydev = "/dev/xcons";
     console->pty = devconsole;
  }
#else /*ultrix*/

  Debug("looking for a good pty/tty");
  status = get_pty(&devconsole,&ttyconsole);

  if(status >= 0){
     console->ttydev = ttydev;
     console->pty = devconsole;	/* pty */

     Debug("pty = %d tty= %d",devconsole,ttyconsole);

#ifdef TIOCCONS
     if(debugging == False)
       if (ioctl(devconsole, TIOCCONS, (char *)&on) == -1){
	  SystemWarning("Couldn't get console output");
	  return;
       }
#else
     /* here are implementation for other systems*/
     SystemNotice("xconsole:Couldn't grab console");
     XtError("Couldn't grab console");
#endif

     console->terminalOutput =
XtAppAddInput(console->appContext,devconsole,XtInputReadMask,
						ReadSession,NULL);
  }
  else {
     SystemError("Couldn't find good pty");
     exit(1);
  }
#endif /*ultrix*/

  if(dbg == False){

  }

}


get_pty(pty,tty)
     int *pty,*tty;
{
   int status;
   
   strcpy(ptydev,"/dev/ptyp0");
   strcpy(ttydev,"/dev/ttyp0");

   /*********** we try only once !!!! ***********/
   status = try_last(ptydev,pty);

   if(status == 0){
      Debug("pty: %s ",ptydev);
      return(*tty=do_open(ptydev,ttydev));
   }
   else{
      ptydev[8] = 'q';
      ptydev[9] = '0';
      status = try_last(ptydev,pty);
      if(status == 0){
	 Debug("pty: %s, %s ",ptydev, ttydev);
	 return(*tty=do_open(ptydev,ttydev));
      }
      else
	return -1;
   }
}

do_open(ptydev,ttydev)
     char ptydev[],ttydev[];
{
   int tty;
   ttydev[8] = ptydev[8];
   ttydev[9] = ptydev[9];
   tty = open(ttydev, O_RDWR,0622);
   return tty;
}

try_last(ptydev,pty)
     char ptydev[];
     int *pty;
{
   int i;
   for(i=0;i<16;i++){
      *pty=open(ptydev, O_RDWR,0622);
      if(*pty >= 0)
	return 0;
      (ptydev[9])++;
   }
   return -1;
}

extern int errno;
extern int sys_nerr;
extern *sys_errlist[];

static void error_status()
{
   if( errno > 0 && errno <= sys_nerr){
      syslog(LOG_WARNING, sys_errlist[errno]);
      Debug("System error: %s.\n",sys_errlist[errno]);
   }
}

SystemError(string)
     char *string;
{
   syslog(LOG_NOTICE, string);
   Debug(string);
   error_status();
   exit(1);
}

SystemWarning(string)
     char *string;
{
   error_status();
   syslog(LOG_WARNING, string);
}

SystemNotice(string)
     char *string;
{
   error_status();
   syslog(LOG_NOTICE, string);
}

/*VARARGS1*/
Debug (fmt, arg1, arg2, arg3, arg4, arg5)
char	*fmt;
int	arg1, arg2, arg3, arg4, arg5;
{
	if (dbg) {
	   fprintf(stderr,"Debug: ");
	   fprintf (stderr,fmt, arg1, arg2, arg3, arg4, arg5);
	   fprintf(stderr,"\n");
	   fflush(stderr);
	}
}

void
PrintUsage()
{
  int i;

  fprintf(stderr, "usage: xconsole [options].\n");
  
  for (i = 0 ; xcons_args[i].name != NULL; i++)
    fprintf(stderr, "%-10s %-20s %s\n", xcons_args[i].name, 
	    xcons_args[i].param, xcons_args[i].description);
  exit(1);
}
