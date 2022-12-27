/*
 * This an example of how to use the Command widget.
 *
 * User events handled through callback routines.
 *
 * November 14, 1989 - Chris D. Peterson 
 */

/*
 * $XConsortium: xcommand.c,v 1.10 89/12/15 19:36:59 kit Exp $
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
#include <X11/Xaw/Cardinals.h>	

String fallback_resources[] = { 
    "*Command.Label:    Click the left mouse button here",
    NULL,
};

static XrmOptionDescRec options[] = {
{"-label",	"*Command.label",	XrmoptionSepArg,	NULL}
};

main(argc, argv)
int argc;
char **argv;
{
    static void Syntax();
    static void Select();
    XtAppContext app_con;
    Widget toplevel, command;

    toplevel = XtAppInitialize(&app_con, "Xcommand",
			       options, XtNumber(options),
			       &argc, argv, fallback_resources, NULL, ZERO);

    if (argc != 1)		
	Syntax(app_con, argv[0]);

    command = XtCreateManagedWidget("command", commandWidgetClass, toplevel,
				    NULL, ZERO);

    /*
     * Add a callback routine to the Command widget that will print
     * the message "button selected" to stdout.
     */

    XtAddCallback(command, XtNcallback, Select, NULL);

    XtRealizeWidget(toplevel);
    XtAppMainLoop(app_con);
}

/*	Function Name: Select
 *	Description: This function prints a message to stdout.
 *	Arguments: w - ** UNUSED **
 *                 call_data - ** UNUSED **
 *                 client_data - ** UNUSED **
 *	Returns: none
 */

static void
Select(w, client_data, call_data)
Widget w;
XtPointer call_data, client_data;
{
    fprintf(stdout, "Button Selected.\n");
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
    fprintf( stderr, "Usage: %s [-label <label name>]\n", call);
    exit(1);
}
