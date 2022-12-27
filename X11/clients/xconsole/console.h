/*
 * xconsole
 *
 * $Header:
/afs/athena.mit.edu/astaff/project/xdm/xconsole/RCS/console.h,v 2.0
89/10/31 11:35:33 roman Exp Locker: roman $
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
 * Author:    Roman J. Budzianowski
 * Created:   March 27, 1989
 */

#ifndef _CONSOLE
#define _CONSOLE

typedef struct _console {
   XtAppContext appContext;
   Widget    shell;
   Boolean   mapped;
   Widget   title;
   Widget   text;
   Widget   close;
   Boolean  retain;		/* retain existing log ? */
   Boolean  doTitle;		/* display title ?  */
   char     *consoleLog;
   XtInputId terminalOutput;
   Boolean  autoRaise;
   char     *ttydev;
   int      pty;
   Boolean  debug;
} ConsoleRec, *Console;

void WriteToConsole(), CreateConsole();
void ClearConsole();
void DestroyConsole();
void ReadSession();
void ScrollUp();
void ScrollTo();
void ConsoleUp();
void ConsoleDown();
void HandleAllEvents();

#include <X11/Xaw/Text.h>
XtTextPosition GetPosition();
#endif _CONSOLE
