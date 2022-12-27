/*
 * A more complicated Simple Menu Widget example.
 * 
 * November 30, 1989 - Chris D. Peterson
 */

/*
 * $XConsortium: xmenu2.c,v 1.8 89/12/15 19:33:17 kit Exp $
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
#include <X11/bitmaps/xlogo16>

#include <X11/Xaw/MenuButton.h>
#include <X11/Xaw/SimpleMenu.h>
#include <X11/Xaw/Sme.h>
#include <X11/Xaw/SmeBSB.h>
#include <X11/Xaw/SmeLine.h>

#include <X11/Xaw/Cardinals.h>

static void MenuSelect(), Syntax();

String fallback_resources[] = { 
#ifdef COLOR_DISPLAY
    "*SimpleMenu*foreground:		SteelBlue",
    "*SimpleMenu*menuLabel.foreground:	Gold",
    "*SimpleMenu*line.foreground:	Grey",
#endif /* COLOR_DISPLAY */
    "*menuButton.label:		        Click here for menu",
    "*SimpleMenu*menuLabel.vertSpace:	100",
    "*SimpleMenu*menuLabel.leftMargin:	70",
    "*SimpleMenu.label: 		Xmenu2 Menu",
    "*SimpleMenu*quit*label:  	      Select this item to quit the menu demo.",
    "*SimpleMenu*RowHeight:		16",
    "*SimpleMenu*item7*sensitive: 	off",
    "*SimpleMenu*HorizontalMargins:     30",
    "*menuLabel*font: -*-courier-medium-r-normal--34-*-100-100-*-*-iso8859-1",
    "*item1*font: -*-courier-medium-r-normal--11-*-100-100-*-*-iso8859-1",
    "*item2*font: -*-courier-bold-r-normal--11-*-100-100-*-*-iso8859-1",
    "*item3*font: -*-courier-medium-r-normal--14-*-100-100-*-*-iso8859-1",
    "*item4*font: -*-courier-bold-r-normal--14-*-100-100-*-*-iso8859-1",
    "*item5*font: -*-courier-medium-r-normal--17-*-100-100-*-*-iso8859-1",
    "*item6*font: -*-courier-bold-r-normal--17-*-100-100-*-*-iso8859-1",
    "*item7*font: -*-courier-medium-r-normal--20-*-100-100-*-*-iso8859-1",
    "*item8*font: -*-courier-bold-r-normal--20-*-100-100-*-*-iso8859-1",
    NULL,
};

#define NUM_MENU_ITEMS 12

static char * menu_entry_names[] = {
  "quit", 
  "item1", "item2", "item3", "item4", "item5", "item6", "item7", "item8",
  "menu1", "menu2", "menu3",
};

static Boolean status[NUM_MENU_ITEMS];
static Pixmap mark;

void
main(argc, argv)
char ** argv;
int argc;
{
    Widget top, menu, button, entry;
    XtAppContext app_con;
    int i;
    top = XtAppInitialize(&app_con, "Xmenu2", NULL, ZERO,
			  &argc, argv, fallback_resources, NULL, ZERO);
    
    if (argc != 1)		
	Syntax(app_con, argv[0]);
    
    button = XtCreateManagedWidget("menuButton", menuButtonWidgetClass, top,
				   NULL, ZERO);
    
    menu = XtCreatePopupShell("menu", simpleMenuWidgetClass, button, 
			      NULL, ZERO);
  
    for (i = 0; i < NUM_MENU_ITEMS ; i++) {
	char * item = menu_entry_names[i];
	
	entry = XtCreateManagedWidget(item, smeBSBObjectClass, menu, 
				      NULL, ZERO);
	XtAddCallback(entry, XtNcallback, MenuSelect, (XtPointer) i);

	/* 
	 * Put a couple of line seperators in at the apropriate place. 
	 */

	if (i == 4) 
	    entry = XtCreateManagedWidget("line", smeLineObjectClass, menu,
					  NULL, ZERO);
	if (i == 8) 
	    entry = XtCreateManagedWidget("blank", smeObjectClass, menu,
					  NULL, ZERO);
    }

    /*
     * Create the bitmap for marking selected items. 
     */

    mark = XCreateBitmapFromData(XtDisplay(top),
				 RootWindowOfScreen(XtScreen(top)),
				 xlogo16_bits, xlogo16_width, xlogo16_height);

    XtRealizeWidget(top);
    XtAppMainLoop(app_con);
}

/*	Function Name: MenuSelect
 *	Description: called whenever a menu item is selected.
 *	Arguments: w - the menu item that was selected.
 *                 number - the number of the menu item selected.
 *                 garbage - *** unused ***.
 *	Returns: 
 */

/* ARGSUSED */
static void
MenuSelect(w, number, garbage)
Widget w;
XtPointer number, garbage;
{
    Arg arglist[1];
    Cardinal num_args = 0;
    int num = (int) number;
    
    printf("Menu item `%s' has been selected.\n", XtName(w));
    
    if (num == 0) {			/* quit selected. */
	XtDestroyApplicationContext(XtWidgetToApplicationContext(w));
	exit(0);
    }
    if (status[num]) 
	XtSetArg(arglist[num_args], XtNleftBitmap, None); 
    else 
	XtSetArg(arglist[num_args], XtNleftBitmap, mark);
    num_args++;
    XtSetValues(w, arglist, num_args);
    
    status[num] = !status[num];
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
