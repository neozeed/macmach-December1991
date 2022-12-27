/*
 * xscroll.c
 *
 * This an example of how to use the Scrollbar widget.
 *
 * This example also shows how to set a float resource in an argument list.
 *
 * User events handled through callback routines.
 *
 * November 14, 1989 - Chris D. Peterson 
 */

/*
 * $XConsortium: xscroll.c,v 1.12 89/12/15 19:37:24 kit Exp $
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

#include <X11/Xaw/Scrollbar.h>
#include <X11/Xaw/Cardinals.h>	

static XtCallbackProc Scrolled(), Jumped();

static XrmOptionDescRec options[] = {
{"-horiz",	"scrollbar.orientation", XrmoptionNoArg,  "horizontal"}
};

void
main(argc, argv)
int argc;
char **argv;
{
    static void Syntax();
    XtAppContext app_con;
    Widget toplevel, scrollbar;
    XtArgVal * l_top;
    float top;
    Arg args[2];
    Cardinal num_args;

    toplevel = XtAppInitialize(&app_con, "Xscroll", options, XtNumber(options),
			       &argc, argv, NULL, NULL, ZERO);

    if (argc != 1)		
	Syntax(app_con, argv[0]);

    
    /*
     * Setting a float resource in an argument list.
     */

    top = 0.5;
    num_args = 0;
    if (sizeof(float) > sizeof(XtArgVal)) {
	/*
	 * If a float is larger than an XtArgVal then pass this 
	 * resource value by reference.
	 */
	XtSetArg(args[num_args], XtNtopOfThumb, &top); num_args++;
    }
    else {
	/*
	 * Convince C not to perform an automatic conversion, which
	 * would truncate 0.5 to 0. 
	 */
	l_top = (XtArgVal *) &top;
	XtSetArg(args[num_args], XtNtopOfThumb, *l_top); num_args++;
    }

    /*
     * Setting other resources is much simpler.
     */

    XtSetArg(args[num_args], XtNlength, 200); num_args++;

    scrollbar = XtCreateManagedWidget("scrollbar", scrollbarWidgetClass,
				      toplevel, args, num_args);

    /*
     * Add a callback routine to the Scrollbar widget that will inform
     * us when the scrollbar has jumped, or scrolled.
     */

    XtAddCallback(scrollbar, XtNjumpProc, Jumped, NULL);
    XtAddCallback(scrollbar, XtNscrollProc, Scrolled, NULL);

    XtRealizeWidget(toplevel);
    XtAppMainLoop(app_con);
}

/*	Function Name: Scrolled
 *	Description: This function prints a message to stdout.
 *	Arguments: w - ** UNUSED **
 *                 call_data - ** UNUSED **
 *                 client_data - the number of pixels scrolled.
 *	Returns: none
 */

/* ARGSUSED */
static XtCallbackProc
Scrolled(w, closure, call_data)
Widget w;
XtPointer closure, call_data;
{
    fprintf(stdout,
	   "scrolled by %d pixels.\n", (int) call_data );
}

/*	Function Name: Jumped
 *	Description: This function prints a message to stdout.
 *	Arguments: w - ** UNUSED **
 *                 call_data - ** UNUSED **
 *                 client_data - a pointer to a float containing 
 *                               the location selected.
 *	Returns: none
 */

/* ARGSUSED */
static XtCallbackProc
Jumped(w, closure, call_data)
Widget w;
XtPointer closure, call_data;
{
    float top = *((float *) call_data);

    fprintf(stdout,
	    "thumbed at %4.2f%% the height of the scrollbar\n", top * 100 );
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
    fprintf( stderr, "Usage: %s [-horiz]\n", call);
    exit(1);
}

