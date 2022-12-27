/*
 * $XConsortium: DefErrMsg.c,v 1.5 89/12/13 16:17:04 rws Exp $
 *
 * Copyright 1988 by the Massachusetts Institute of Technology
 *
 * Permission to use, copy, modify, and distribute this software and its
 * documentation for any purpose and without fee is hereby granted, provided
 * that the above copyright notice appear in all copies and that both that
 * copyright notice and this permission notice appear in supporting
 * documentation, and that the name of M.I.T. not be used in advertising
 * or publicity pertaining to distribution of the software without specific,
 * written prior permission. M.I.T. makes no representations about the
 * suitability of this software for any purpose.  It is provided "as is"
 * without express or implied warranty.
 *
 */

#include <stdio.h>
#include <X11/Xlib.h>
#include <X11/Xproto.h>

/*
 * XmuPrintDefaultErrorMessage - print a nice error that looks like the usual 
 * message.  Returns 1 if the caller should consider exitting else 0.
 */
int XmuPrintDefaultErrorMessage (dpy, event, fp)
    Display *dpy;
    XErrorEvent *event;
    FILE *fp;
{
    char buffer[BUFSIZ];
    char mesg[BUFSIZ];
    char number[32];
    char *mtype = "XlibMessage";
    register _XExtension *ext = (_XExtension *)NULL;
    XGetErrorText(dpy, event->error_code, buffer, BUFSIZ);
    XGetErrorDatabaseText(dpy, mtype, "XError", "X Error", mesg, BUFSIZ);
    (void) fprintf(fp, "%s:  %s\n  ", mesg, buffer);
    XGetErrorDatabaseText(dpy, mtype, "MajorCode", "Request Major code %d", 
	mesg, BUFSIZ);
    (void) fprintf(fp, mesg, event->request_code);
    if (event->request_code < 128) {
	sprintf(number, "%d", event->request_code);
	XGetErrorDatabaseText(dpy, "XRequest", number, "", buffer, BUFSIZ);
    } else {
	/* XXX need an Xlib interface for this */
	for (ext = dpy->ext_procs;
	     ext && (ext->codes.major_opcode != event->request_code);
	     ext = ext->next)
	  ;
	if (ext)
	    strcpy(buffer, ext->name);
	else
	    buffer[0] = '\0';
    }
    (void) fprintf(fp, " (%s)", buffer);
    fputs("\n  ", fp);
    XGetErrorDatabaseText(dpy, mtype, "MinorCode", "Request Minor code %d",
	mesg, BUFSIZ);
    (void) fprintf(fp, mesg, event->minor_code);
    fputs("\n  ", fp);
    if (ext) {
	sprintf(mesg, "%s.%d", ext->name, event->minor_code);
	XGetErrorDatabaseText(dpy, "XRequest", mesg, "", buffer, BUFSIZ);
	(void) fprintf(fp, " (%s)", buffer);
    }
    XGetErrorDatabaseText(dpy, mtype, "ResourceID", "ResourceID 0x%x",
	mesg, BUFSIZ);
    (void) fprintf(fp, mesg, event->resourceid);
    fputs("\n  ", fp);
    XGetErrorDatabaseText(dpy, mtype, "ErrorSerial", "Error Serial #%d", 
	mesg, BUFSIZ);
    (void) fprintf(fp, mesg, event->serial);
    fputs("\n  ", fp);
    XGetErrorDatabaseText(dpy, mtype, "CurrentSerial", "Current Serial #%d",
	mesg, BUFSIZ);
    (void) fprintf(fp, mesg, NextRequest(dpy)-1);
    fputs("\n", fp);
    if (event->error_code == BadImplementation) return 0;
    return 1;
}


/*
 * XmuSimpleErrorHandler - ignore errors for XQueryTree, XGetWindowAttributes,
 * and XGetGeometry; print a message for everything else.  In all case, do
 * not exit.
 */
int XmuSimpleErrorHandler (dpy, errorp)
    Display *dpy;
    XErrorEvent *errorp;
{
    switch (errorp->request_code) {
      case X_QueryTree:
      case X_GetWindowAttributes:
        if (errorp->error_code == BadWindow) return 0;
	break;
      case X_GetGeometry:
	if (errorp->error_code == BadDrawable) return 0;
	break;
    }
    /* got a "real" X error */
    return XmuPrintDefaultErrorMessage (dpy, errorp, stderr);
}	
