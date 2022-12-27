/*
 *	$Source: /afs/athena.mit.edu/astaff/project/xdm/xconsole/RCS/console.c,v $
 *	$Header:
/afs/athena.mit.edu/astaff/project/xdm/xconsole/RCS/console.c,v 2.0
89/10/31 11:35:25 roman Exp Locker: roman $
 *      $Author: roman $
 *      $Locker: roman $
 *
 * 	Copyright (C) 1988 Massachusetts Institute of Technology	
 *	
 */


/*

   Permission to use, copy, modify, and distribute this
   software and its documentation for any purpose and without
   fee is hereby granted, provided that the above copyright
   notice appear in all copies and that both that copyright
   notice and this permission notice appear in supporting
   documentation, and that the name of M.I.T. not be used in
   advertising or publicity pertaining to distribution of the
   software without specific, written prior permission.
   M.I.T. makes no representations about the suitability of
   this software for any purpose.  It is provided "as is"
   without express or implied warranty.

*/

/** Author:  Roman J. Budzianowski - Project Athena, MIT **/

#if ( !defined(lint) && !defined(SABER))
static char *rcsid_console_c = "$Header: /afs/athena.mit.edu/astaff/project/xdm/xconsole/RCS/console.c,v 2.0 89/10/31 11:35:25 roman Exp Locker: roman $";
#endif lint

#include <stdio.h>
#include <X11/Xos.h>
#include <X11/Xlib.h>
#include <X11/Intrinsic.h>
#include <X11/StringDefs.h>
#include <X11/Xaw/Form.h>
#include <X11/Xaw/Command.h>
#include <X11/Xaw/Label.h>
#include <X11/Shell.h>
#include <X11/Xaw/AsciiText.h>

#include "console.h"

extern ConsoleRec theConsole;

static XtCallbackRec callbacks[] = {
   {NULL,NULL},
   {NULL,NULL}
};

/* ARGSUSED */
void CloseWindow(w,closure,call_data)
     Widget w;
     caddr_t closure;
     caddr_t call_data;
{
   Console console = (Console)closure;

   XtPopdown(console->shell);
   console->mapped = False;
   
}

void CreateLogFile(console)
     Console console;
{
   int f;
   int open_flag = O_CREAT|O_TRUNC;

   if(console->retain)
     open_flag = O_RDWR;

   f = open(console->consoleLog,open_flag,0666);
   if(f>=0)
     close(f);
   else
     SystemError("Cannot open console log file.\n");
}

void DestroyConsole(console)
     Console console;
{
   if(console->terminalOutput)
      XtRemoveInput(console->terminalOutput);
   XtDiskSourceDestroy(XtTextGetSource(console->text));
}

void ClearConsole(console)
     Console console;
{
   Arg          args[2];
   extern Boolean dbg;

   Debug("clearing console");
   XtDiskSourceDestroy(XtTextGetSource(console->text));
   
   CreateLogFile(console);
   XtSetArg(args[0],XtNfile,console->consoleLog);
   XtSetArg(args[1],XtNeditType,XttextAppend);
   XtTextSetSource(console->text,
		   XtDiskSourceCreate(console->text, args, 2),(XtTextPosition)0);

   if(dbg)
     return;

   ConsoleDown();

}

static XEvent lastEvent;

/* ARGSUSED */
Bool CheckEvent(dsp,event,arg)
     Display *dsp;
     XEvent *event;
     char *arg;
{
   Debug("CheckEvent: event.type = %d",event->type);

   if(event->type == MapNotify || event->type == UnmapNotify){
      Window win = (Window)arg;
      if(event->xmap.window == win){
	 Debug("                 CheckEvent: Found !!!");
	 return True;
      }
      else
	return False;
   }
   else
     return False;
}

/* ARGSUSED */
void HandleMap(w, data, event)
     Widget w;
     caddr_t data;
     XEvent *event;
{
   if(event->type == MapNotify || event->type == UnmapNotify){
      Window win = event->xmap.window;

      bcopy((char *)event,(char *)&lastEvent,sizeof(XEvent));

      XSync(XtDisplay(w),0);

#ifdef DEBUG
      Debug("\nHandleMap: event.type = %d win = %d \n",event->type, win);


      if(dbg){
	 int nevents;
	 nevents = XEventsQueued(XtDisplay(w),QueuedAfterReading);
	 Debug("HandleMap: Events in the queue = %d",nevents);
      }
#endif

      while(XCheckIfEvent(XtDisplay(w), &lastEvent, CheckEvent, (char *)win))
	;

      Debug("HandleMap: lastEvent  event.type = %d\n",lastEvent.type);
      if(lastEvent.type == MapNotify){
	 ConsoleUp();
      } else
	if(lastEvent.type == UnmapNotify){
	   ConsoleDown();
	}
   }
}


void CreateConsole(console)
     Console console;
{
   Arg       args[7];
   Widget    frame,text;
   Cardinal  n;

   Debug("creating console.");

  
XtAddEventHandler(console->shell,StructureNotifyMask,False,HandleMap,(caddr_t)console);

   frame = XtCreateWidget("consoleFrame", formWidgetClass, console->shell, args, 0);


    /** create title **/
   console->title = XtCreateManagedWidget("title", labelWidgetClass,
frame, args, 0);

   callbacks[0].callback = CloseWindow;
   callbacks[0].closure  = (caddr_t)console;
   XtSetArg(args[0],XtNcallback,callbacks);

   console->close = XtCreateManagedWidget("closeWindow",
commandWidgetClass, frame, args, 1);

   CreateLogFile(console);

   n = 0;
   XtSetArg(args[n],XtNeditType,XttextAppend);n++;
   XtSetArg(args[n],XtNtextOptions, scrollOnOverflow|scrollVertical|wordBreak);n++;
   XtSetArg(args[n],XtNfile,console->consoleLog);n++;
   text = XtCreateManagedWidget("text", asciiDiskWidgetClass, frame, args, n);
   console->text = text;

   XtManageChild(frame); 
   
   console->mapped = False;

}

void DisableRedisplay()
{
   XtTextDisableRedisplay(theConsole.text);
}

void EnableRedisplay()
{
   XtTextEnableRedisplay(theConsole.text);
}

void InitializeConsole(console)
     Console console;
{
   char name[BUFSIZ], hostname[BUFSIZ];
   Arg args[1];

   if(console->doTitle == False)
     return;

   strcpy(name,"Console  ");
   gethostname(hostname,BUFSIZ);
   strcat(name,hostname);
   strcat(name,":");
   strcat(name,rindex(console->ttydev,'/') + 1);

   XtSetArg(args[0],XtNlabel,name);
   XtSetValues(console->title,args,1);
}

void ConsoleUp()
{
   if(theConsole.mapped == False){
     XtPopup(theConsole.shell ,XtGrabNone);
     theConsole.mapped = True;
  }
}

void ConsoleDown()
{
   if(theConsole.mapped == True){
      XtPopdown(theConsole.shell);
      theConsole.mapped = False;
   }
}

void WriteToConsole(text)
     char *text;
{
   static XtTextBlock block = { 0, 0, NULL, };
   int status;
   Console console = &theConsole;

   block.length = strlen(text);
   block.ptr = text;

   status = XtTextReplace(console->text, XtTextGetInsertionPoint(console->text),
			XtTextGetInsertionPoint(console->text), &block);
#ifdef DEBUG
   if(status != XawEditDone){
      Debug("Internal error in WriteToConsole.");
      if(status == XawPositionError)
	Debug("   Position error.");
      else
	Debug("   Edit error.");
   }
#endif

   if(!console->mapped){
      XtPopup(console->shell ,XtGrabNone);
      console->mapped = True;
   }
   else if(console->autoRaise)
     XRaiseWindow(XtDisplay(console->shell),XtWindow(console->shell));

/*   Debug("DONE with writing to console.");*/
}

void HandleAllEvents(w)
     Widget w;
{
   XEvent ev;

   while(XtAppPending(theConsole.appContext)){
      XtAppNextEvent(theConsole.appContext,&ev);
      XtDispatchEvent(&ev);
   }
   XFlush(XtDisplay(w));
}


/* ARGSUSED */
void ReadSession(data,src,id)
     caddr_t    data;
     int        *src;
     XtInputId  *id;
{
   static char buffer[BUFSIZ+1];
   int nbytes;
   int i=0;

   nbytes = read(*src,buffer,BUFSIZ);
   switch(nbytes){
    case -1:
      SystemWarning("Error when reading pty.");
      XtRemoveInput(*id);
      break;
    case 0:
      Debug("End of file .");
      XtRemoveInput(*id);
      break;
    default:
      buffer[nbytes] = '\0';
#ifdef notdef
      for(; i<nbytes; i++) {
	if((buffer[i] & 0x7f) == 7) {
	  XBell(XtDisplay(theConsole->shell), 0);
	  bcopy(&buffer[i+1], &buffer[i], nbytes-i);
	  nbytes--; i--;
	}
      }
#endif notdef
      if(nbytes == 0) {
	XtRemoveInput(*id);
	break;
      }
      WriteToConsole(buffer);
      break;
   }
}
