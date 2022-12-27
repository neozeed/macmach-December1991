/*
 * This example demostrates how to use the Athena List widget.
 * 
 * November 30, 1989 - Chris D. Peterson
 */

/*
 * $XConsortium: xlist.c,v 1.4 89/12/15 19:37:22 kit Exp $
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
#include <X11/StringDefs.h>

#include <X11/Xaw/List.h>

#include <X11/Xaw/Cardinals.h>

static void Activate(), Syntax();

void 
main(argc, argv)
int argc;
char **argv;
{
    Widget toplevel, list;
    XtAppContext app_con;
    Arg args[1];
    static String items[] = {
	"first list entry",
	"second list entry",
	"third list entry",
	"fourth list entry",
	NULL
    };

    toplevel = XtAppInitialize(&app_con, "Xlist", NULL, ZERO,
			       (Cardinal *) &argc, argv, NULL, NULL, ZERO);
    if (argc != 1) 
	Syntax(app_con, argv[0]);

    XtSetArg(args[0], XtNlist, items);
    list= XtCreateManagedWidget( "list", listWidgetClass, toplevel, args, ONE);
    XtAddCallback(list, XtNcallback, Activate, (XtPointer)NULL);
    
    XtRealizeWidget(toplevel);
    XtAppMainLoop(app_con);
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
    exit(1);
}

/*	Function Name: Activate
 *	Description: Called when a list item has been selected.
 *	Arguments: w, closure - *** UNUSED ***.
 *                 call_data - a pointer to the list info structure.
 *	Returns: 
 */

/* ARGSUSED */
static void 
Activate(w, closure, call_data)
Widget w;
XtPointer closure, call_data;
{
    XawListReturnStruct *item = (XawListReturnStruct*)call_data;
    printf( "selected item %d; \"%s\"\n", item->list_index, item->string );
}

