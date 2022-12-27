/*
 * This an example of how to create nested composite widgets.
 *
 * User events are handled through callback routines.
 *
 * Application resources are also introduced.
 *
 * November 27, 1989 - Chris D. Peterson 
 */

/*
 * $XConsortium: xboxes.c,v 1.20 89/12/15 19:32:48 kit Exp $
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

#include <X11/Xaw/Box.h>	
#include <X11/Xaw/Command.h>	
#include <X11/Xaw/Label.h>	
#include <X11/Xaw/Viewport.h>	
#include <X11/Xaw/Cardinals.h>	

/*
 * All application resources must reside in a structure, the structure
 * contains space where the application resources will be stored.
 */

typedef struct _AppResources {
    Boolean scroll;
    int num_boxes;
} AppResources;

/*
 * The resources specification contains:
 *
 * The name and class of the resource.
 * The representation type of the resource.
 * The location in the structure to keep the value of this resource.
 * The default value for this resource.
 *
 * This information is passed into XtGetApplicationResources().
 */

static XtResource resources[] = {
    {"scroll", "Scroll", XtRBoolean, sizeof(Boolean),
     XtOffset(AppResources *, scroll), XtRImmediate, False},
    {"numBoxes", "NumBoxes", XtRInt, sizeof(int),
     XtOffset(AppResources *, num_boxes), XtRImmediate, (XtPointer) 3},
};

String fallback_resources[] = { 
    "*Command.Label:    Click the left mouse button here to quit",
    "*Viewport*allowVert: True",
    "*Viewport*allowHoriz: True",
    NULL,
};

static XrmOptionDescRec options[] = {
    {"-hspace",	"*Box.hSpace",	XrmoptionSepArg,	NULL},
    {"-vspace",	"*Box.vSpace",	XrmoptionSepArg,	NULL},
    {"-scroll", "scroll",	XrmoptionNoArg,         "True"},
    {"-noscroll", "scroll",	XrmoptionNoArg,         "False"},
    {"-numboxes", "numBoxes",   XrmoptionSepArg,        NULL},
};

main(argc, argv)
int argc;
char **argv;
{
    static void Syntax(), MakeBox();
    static XtCallbackProc Quit();
    int i;
    XtAppContext app_con;
    Widget toplevel, box_parent, box, command, label;
    AppResources app_resources;

    toplevel = XtAppInitialize(&app_con, "Xboxes",
			       options, XtNumber(options),
			       &argc, argv, fallback_resources, NULL, ZERO);

    if (argc != 1)		
	Syntax(app_con, argv[0]);

    /*
     * Actually load the application resources, and store them in the
     * structure pointed to by &app_resources.
     */

    XtGetApplicationResources( toplevel, (XtPointer) &app_resources,
			       resources, XtNumber(resources), NULL, ZERO );

    /*
     * Here we use the app_resources "scroll" to determine whether or not
     * to insert a viewport widget between the outer Box and the Shell.  This 
     * would allow scrolling of the outer Box.
     */

    if (app_resources.scroll)
	box_parent = XtCreateManagedWidget("view",viewportWidgetClass,toplevel,
					   NULL, ZERO);
    else
	box_parent = toplevel;

    box = XtCreateManagedWidget("outerbox", boxWidgetClass, box_parent,
				NULL, ZERO);

    command = XtCreateManagedWidget("command", commandWidgetClass, box,
				    NULL, ZERO);
    XtAddCallback(command, XtNcallback, Quit, NULL);

    /* 
     * Put a bunch of smaller boxes in the outer box.
     */

    for (i = 1; i <= app_resources.num_boxes; i++) {
	char name[BUFSIZ];
	
	sprintf(name, "aLongBoxName%d", i * i * i * i);
	MakeBox(box, (String) name);
    }

    XtRealizeWidget(toplevel);
    XtAppMainLoop(app_con);
}

/*	Function Name: MakeBox
 *	Description: Makes a button box containing three labels.
 *	Arguments: parent - parent of the box.
 *                 name - name of the boxes first label, and the box.
 *	Returns: none.
 */

static void
MakeBox(parent, name)
Widget parent;
String name;
{
    Widget box;
    int i;
    
    box = XtCreateManagedWidget(name, boxWidgetClass, parent, NULL, ZERO);

    (void) XtCreateManagedWidget(name, labelWidgetClass, box, NULL, ZERO);

    for ( i = 0; i < 3; i++)
	(void) XtCreateManagedWidget("label", labelWidgetClass, box,NULL,ZERO);
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
    void exit();

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
    fprintf( stderr, "Usage: %s\n", call);
    fprintf( stderr, "    [ -vspace <value> ] [ -hspace <value> ]\n");
    fprintf( stderr, "    [ -scroll ] [ -noscroll ]\n");
    fprintf( stderr, "    [ -numboxes <value> ]\n");
    exit(1);
}
