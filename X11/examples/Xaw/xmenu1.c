/*
 * A simple Simple Menu Widget example.
 * 
 * November 30, 1989 - Chris D. Peterson
 */

/*
 * $XConsortium: xmenu1.c,v 1.8 89/12/15 19:33:15 kit Exp $
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

#include <X11/Xaw/MenuButton.h>
#include <X11/Xaw/SimpleMenu.h>
#include <X11/Xaw/Sme.h>
#include <X11/Xaw/SmeBSB.h>

#include <X11/Xaw/Cardinals.h>

#define streq(a, b)        ( strcmp((a), (b)) == 0 )

static void MenuSelect(), Syntax();

String fallback_resources[] = { 
#ifdef COLOR_DISPLAY
    /* For Color workstations. */
    "*menu.menuLabel.foreground:    Blue",
    "*menu*quit.foreground:	    Green",
    "*menu*item1.foreground:        Red",
    "xmenu1*menu*item2.foreground:  White",
    "*menu*item3.foreground:        Blue",
    "*menu*item4.foreground:	    Red",
#endif /* COLOR_DISPLAY */
    "*menuButton.label:             Click here for a pulldown menu",
    "*menu.label:		    This is xmenu1",
    "*menuLabel.vertSpace:	    100",
    "*blank.height:		    20",
    NULL,
};

void
main(argc, argv)
char ** argv;
int argc;
{
    Widget top, command, menu, entry;
    XtAppContext app_con;
    int i;
    
    static char * menu_item_names[] = {
	"quit",	"item1", "item2", "item3", "item4",
    };

    top = XtAppInitialize(&app_con, "Xmenu1", NULL, ZERO,
			  &argc, argv, fallback_resources, NULL, ZERO);
    
    if (argc != 1)		
	Syntax(app_con, argv[0]);

    command = XtCreateManagedWidget("menuButton", menuButtonWidgetClass, top,
				    NULL, ZERO);
    
    menu = XtCreatePopupShell("menu", simpleMenuWidgetClass, command, 
			      NULL, ZERO);
    
    for (i = 0; i < (int) XtNumber(menu_item_names) ; i++) {
	char * item = menu_item_names[i];
	
	entry = XtCreateManagedWidget(item, smeBSBObjectClass, menu,
				      NULL, ZERO);
	XtAddCallback(entry, XtNcallback, MenuSelect, NULL);
	
	/*
	 * Create an extra blank entry after the third menu entry.
	 */

	if (i == 2)
	    entry = XtCreateWidget("blank", smeObjectClass, menu,
				   NULL, ZERO);
    }
    
    XtRealizeWidget(top);
    XtAppMainLoop(app_con);
}

/*	Function Name: MenuSelect
 *	Description: called whenever a menu item is selected.
 *	Arguments: w - the menu item that was selected.
 *                 junk, garbage - *** unused ***.
 *	Returns: 
 */

/* ARGSUSED */
static void
MenuSelect(w, junk, garbage)
Widget w;
XtPointer junk, garbage;
{
    printf("Menu item `%s' has been selected.\n", XtName(w));
    if (streq(XtName(w), "quit")) {
	XtDestroyApplicationContext(XtWidgetToApplicationContext(w));
	exit(0);
    }
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
