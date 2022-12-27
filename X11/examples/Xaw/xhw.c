/*
 * This an example of how "Hello, World" could be written using
 * The X Toolkit and the Athena widget set.
 *
 * November 14, 1989 - Chris D. Peterson 
 */

/*
 * $XConsortium: xhw.c,v 1.7 89/12/11 15:31:33 kit Exp $
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

#include <X11/Intrinsic.h>	/* Include standard Toolkit Header file.
				   We do no need "StringDefs.h" */

#include <X11/Xaw/Label.h>	/* Include the Label widget's header file. */
#include <X11/Xaw/Cardinals.h>	/* Definition of ZERO. */

/*
 * These resources will only get loaded if there is no app-defaults
 * file for this application.  Since this is such a simple application
 * I am just loading the resources here.  For more complex applications
 * It is best to install an app-defaults file.
 */

String fallback_resources[] = { "*Label.Label:    Hello, World", NULL };

main(argc, argv)
int argc;
char **argv;
{
    XtAppContext app_con;
    Widget toplevel;

    /*
     * Initialize the Toolkit, set the fallback resources, and get
     * the application context associated with this application.
     */

    toplevel = XtAppInitialize(&app_con, "Xhw", NULL, ZERO, &argc, argv,
			       fallback_resources, NULL, ZERO);

    /*
     * Create a Widget to display the string.  The label is picked up
     * from the resource database.
     */

    (void) XtCreateManagedWidget("label", labelWidgetClass, toplevel, 
				 NULL, ZERO);

    /*
     * Create the windows, and set their attributes according
     * to the Widget data.
     */

    XtRealizeWidget(toplevel);

    /*
     * Now process the events.
     */
    
    XtAppMainLoop(app_con);
}
