/*
 * $XConsortium: xlogo.c,v 1.11 89/12/09 13:13:19 kit Exp $
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
 *
 */

#include <X11/Intrinsic.h>

#include <X11/Xaw/Logo.h>
#include <X11/Xaw/Cardinals.h>

extern void exit();

String fallback_resources[] = {
    "*iconPixmap:    xlogo32",
    "*iconMask:      xlogo32",
    NULL,
};

/*
 * Report the syntax for calling xlogo.
 */

static void
Syntax(call)
    char *call;
{
    (void) printf ("Usage: %s [-fg <color>] [-bg <color>] [-rv] %s\n", call, 
		   "[-bw <pixels>] [-bd <color>]");
    (void) printf ("             [-d [<host>]:[<vs>]]\n");
    (void) printf ("             [-g [<width>][x<height>]%s", 
		   "[<+-><xoff>[<+-><yoff>]]]\n\n");
    exit(1);
}

void 
main(argc, argv)
int argc;
char **argv;
{
    Widget toplevel;
    XtAppContext app_con;

    toplevel = XtAppInitialize(&app_con, "XLogo", NULL, ZERO, &argc, argv,
			       fallback_resources, NULL, ZERO);

    if (argc != 1) 
	Syntax(argv[0]);

    XtCreateManagedWidget("xlogo", logoWidgetClass, toplevel, NULL, ZERO);
    XtRealizeWidget(toplevel);
    XtAppMainLoop(app_con);
}
