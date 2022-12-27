/*
 * This an example of how to use the Label widget.
 *
 * November 14, 1989 - Chris D. Peterson 
 */

/*
 * $XConsortium: xlabel.c,v 1.7 89/12/15 19:37:20 kit Exp $
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

#include <X11/Xaw/Label.h>	
#include <X11/Xaw/Cardinals.h>	

String fallback_resources[] = { "*Label.Label:    This is xlabel", NULL };

/* This is an example of a command line option table.
 * 
 * "-label"          - The command line option to look for.
 * "*Label.label"    - The resource to which the value will be assigned.
 *                     application name is automatically prepended to this
 *                     string.
 * XrmoptionSepArg   - The next arguemnt will be the value for 
 *                     the *Label.label resource.
 * NULL              - UNUSED.
 */
 
static XrmOptionDescRec options[] = {
{"-label",	"*Label.label",	XrmoptionSepArg,	NULL}
};

main(argc, argv)
int argc;
char **argv;
{
    static void Syntax();
    XtAppContext app_con;
    Widget toplevel;

    toplevel = XtAppInitialize(&app_con, "Xlabel", options, XtNumber(options),
			       &argc, argv, fallback_resources, NULL, ZERO);

    /*
     * Check to see that all arguments were processed, and if not then
     * report an error and exit.
     */

    if (argc != 1)		
	Syntax(app_con, argv[0]);


    (void) XtCreateManagedWidget("label", labelWidgetClass, toplevel, 
				 NULL, ZERO);
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
    fprintf( stderr, "Usage: %s [-label <label name>]\n", call);
    exit(1);
}
