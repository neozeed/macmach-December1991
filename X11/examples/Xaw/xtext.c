/*
 * xtext.c
 *
 * This an example of how to use the Text and Paned widgets.
 *
 * November 14, 1989 - Chris D. Peterson 
 */

/*
 * $XConsortium: xtext.c,v 1.13 89/12/15 19:37:25 kit Exp $
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

#include <X11/Xaw/AsciiText.h>
#include <X11/Xaw/Command.h>
#include <X11/Xaw/Paned.h>

#include <X11/Xaw/Cardinals.h>

static void ClearText(), PrintText(), Syntax();

String fallback_resources[] = { 
    "*input:                  True",
    "*showGrip:               off",
    "*Text*preferredPaneSize: 200", 
    "*Text*editType:          edit",
    "*Text*scrollVertical:    whenNeeded",
    "*Text*scrollHorizontal:  whenNeeded",
    "*Text*autoFill:          on",
    "*clear*label:            Click here to clear the text widget.",
    "*print*label:            Click here to print the text to `stdout'.",
    NULL,
};
 
main(argc, argv)
int argc;
char **argv;
{
    static void Syntax();
    XtAppContext app_con;
    Widget toplevel, paned, clear, print, text;
    Arg args[1];

    toplevel = XtAppInitialize(&app_con, "Xtext", NULL, ZERO,
			       &argc, argv, fallback_resources, NULL, ZERO);

    /*
     * Check to see that all arguments were processed, and if not then
     * report an error and exit.
     */

    if (argc != 1)		
	Syntax(app_con, argv[0]);

    paned = XtCreateManagedWidget("paned", panedWidgetClass, toplevel, 
				    NULL, ZERO);

    clear = XtCreateManagedWidget("clear", commandWidgetClass, paned, 
				  NULL, ZERO);

    print = XtCreateManagedWidget("print", commandWidgetClass, paned, 
				  NULL, ZERO);

    XtSetArg(args[0], XtNstring, 
	     "This is a\ntest.  If this\nhad been an actual\nemergency...");

    text = XtCreateManagedWidget("text", asciiTextWidgetClass, paned, 
				 args, ONE);

    XtAddCallback(clear, XtNcallback, ClearText, (XtPointer) text);
    XtAddCallback(print, XtNcallback, PrintText, (XtPointer) text);

    XtRealizeWidget(toplevel);
    XtAppMainLoop(app_con);
}

/*	Function Name: ClearText
 *	Description: This function clears all text out of the text widget.
 *	Arguments: w - *** UNUSED ***
 *                 text_ptr - a pointer to the text widget.
 *                 call_data - *** UNUSED ***.
 *	Returns: none.
 */

/* ARGSUSED */
static void
ClearText(w, text_ptr, call_data)
Widget w;
XtPointer text_ptr, call_data;
{
    Widget text = (Widget) text_ptr;
    Arg args[1];

    XtSetArg(args[0], XtNstring, "");
    XtSetValues(text, args, ONE);
}

/*	Function Name: PrintText
 *	Description: This function clears all text out of the text widget.
 *	Arguments: w - *** UNUSED ***
 *                 text_ptr - a pointer to the text widget.
 *                 call_data - *** UNUSED ***.
 *	Returns: none.
 */

/* ARGSUSED */
static void
PrintText(w, text_ptr, call_data)
Widget w;
XtPointer text_ptr, call_data;
{
    Widget text = (Widget) text_ptr;
    Arg args[1];
    String str;

    XtSetArg(args[0], XtNstring, &str);
    XtGetValues(text, args, ONE);

    fprintf(stdout, "Text String is:\n--------\n%s\n--------\n", str);
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
    fprintf( stderr, "Usage: %s \n", call);
    exit(1);
}

