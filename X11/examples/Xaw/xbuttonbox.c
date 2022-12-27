/*
 * This an example of how to create a composite widget with children
 * The composite used is a box widget.
 *
 * User events are handled through callback routines.
 *
 * November 27, 1989 - Chris D. Peterson 
 */

/*
 * $XConsortium: xbuttonbox.c,v 1.16 89/12/15 19:33:10 kit Exp $
 *
 * Copyright 1989 Massachusetts Institute of Technology
 *
 * Permission to use, copy, modify, distribute, and sell this software and its
 * documentation for any purpose is hereby granted without fee, provided that
 * the above copyright notice appear in all copies and that both that
 * copyright notice and this permission notice appear in supporting
 * documentation, and that the name of M.I.T. not be used in advertising or
 * publicity pertaining to distribution of the software without specific,
 * written prior permission.  M.I.T. makes no representations about the
 * suitability of this software for any purpose.  It is provided "as is"
 * without express or implied warranty.
 *
 * M.I.T. DISCLAIMS ALL WARRANTIES WITH REGARD TO THIS SOFTWARE, INCLUDING ALL
 * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS, IN NO EVENT SHALL M.I.T.
 * BE LIABLE FOR ANY SPECIAL, INDIRECT OR CONSEQUENTIAL DAMAGES OR ANY DAMAGES
 * WHATSOEVER RESULTING FROM LOSS OF USE, DATA OR PROFITS, WHETHER IN AN ACTION
 * OF CONTRACT, NEGLIGENCE OR OTHER TORTIOUS ACTION, ARISING OUT OF OR IN 
 * CONNECTION WITH THE USE OR PERFORMANCE OF THIS SOFTWARE.
 */

#include <stdio.h>
#include <X11/Intrinsic.h>
#include <X11/StringDefs.h>	/* Get standard string definations. */

#include <X11/Xaw/Command.h>	
#include <X11/Xaw/Label.h>	
#include <X11/Xaw/Box.h>	
#include <X11/Xaw/Cardinals.h>	

extern void exit();

String fallback_resources[] = { 
    "*Command.Label:    Click the left mouse button here to quit",
    NULL,
};

static XrmOptionDescRec options[] = {
    {"-hspace",	"*Box.hSpace",	XrmoptionSepArg,	NULL},
    {"-vspace",	"*Box.vSpace",	XrmoptionSepArg,	NULL},
};

main(argc, argv)
int argc;
char **argv;
{
    static void Syntax();
    static XtCallbackProc Quit();
    int i;
    XtAppContext app_con;
    Widget toplevel, box, command, label;

    toplevel = XtAppInitialize(&app_con, "Xbuttonbox",
			       options, XtNumber(options),
			       &argc, argv, fallback_resources, NULL, ZERO);

    if (argc != 1)		
	Syntax(app_con, argv[0]);

    /* 
     * Create a Box widget and put all children in that box widget.
     * 
     * Unlike the Shell widget a Box widget can accept more than one child.
     * By using boxes and other composites it is possible to create an
     * arbitrarily complex application with hundreds or event thousands of
     * widgets.
     */
    
    box = XtCreateManagedWidget("box", boxWidgetClass, toplevel,
				NULL, ZERO);
    
    /*
     * Put a command widget in the box.
     */

    command = XtCreateManagedWidget("command", commandWidgetClass, box,
				    NULL, ZERO);
    XtAddCallback(command, XtNcallback, Quit, NULL);

    /* 
     * Put 10 Label widget children in the Box.
     */

    for (i = 1; i <= 10; i++) {
	char buf[BUFSIZ];
	
	sprintf(buf, "labelWidgetName%d", i * i * i * i);
	command = XtCreateManagedWidget(buf, labelWidgetClass, box,
					NULL, ZERO);
    }

    XtRealizeWidget(toplevel);
    XtAppMainLoop(app_con);
}

/*	Function Name: Quit
 *	Description: This function prints a message to stdout.
 *	Arguments: w - ** UNUSED **
 *                 call_data - ** UNUSED **
 *                 client_data - ** UNUSED **
 *	Returns: none
 */

static XtCallbackProc
Quit(w, call_data, client_data)
Widget w;
XtPointer call_data, client_data;
{
    XtDestroyApplicationContext(XtWidgetToApplicationContext(w));
    exit(0);
}

/*	Function Name: Syntax
 *	Description: Prints a the calling syntax for this function to stdout.
 *	Arguments: app_con - the application context.
 *                 call - the name of the application.
 *	Returns: none - exits tho.
 */

static void 
Syntax(app_con, call)
XtAppContext app_con;
char *call;
{
    XtDestroyApplicationContext(app_con);
    fprintf( stderr, "Usage: %s [ -vspace <value> ] [ -hspace <value> ]\n",
	    call);
    exit(1);
}
