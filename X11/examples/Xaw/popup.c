/*
 * An example of how to create a popup dialog box.
 *
 * August 17, 1989      - Donna Converse
 * November 30, 1989    - Chris D. Peterson
 */

/*
 * $XConsortium: popup.c,v 1.6 89/12/15 19:33:13 kit Exp $
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

#include <stdio.h>			/* For the Syntax message */
#include <X11/Intrinsic.h>
#include <X11/StringDefs.h>
#include <X11/Shell.h>

#include <X11/Xaw/Box.h>
#include <X11/Xaw/Cardinals.h>
#include <X11/Xaw/Command.h>
#include <X11/Xaw/Dialog.h>

static void PopupPrompt();	/* the callback of the main demo button */
static void Quit();		/* the callback of the main quit button */
static void ColorTheButton();	/* the callback of the popup ok button */
static void DestroyPopupPrompt(); /* the callback of the popup cancel button */
static void Ok();		/* an action proc calling ColorTheButton */
static void Syntax();		/* report syntax errors and exit. */
static int ConvertColor();	/* Converts a color name into a pixel value. */

static String fallback_resources[] = {
    "*input: True",
    "*go.label: Press to see Simple Popup Demonstration",
    "*allowShellResize: True",
    "*Dialog*value: ",
    "*Dialog.label: What color should the main button be?",
    "*Dialog*value.translations: #override \\n <Key>Return: Ok()",
    "*Dialog*label.resizable: TRUE",
    NULL,
};

XtActionsRec actionTable[] = {
    {"Ok",	Ok}
};

void 
main(argc, argv)
int argc;
char **argv;
{
    Widget	toplevel, button, box;
    XtAppContext app_con;

    toplevel = XtAppInitialize(&app_con, "Popup", NULL, ZERO,
			       &argc, argv, fallback_resources, NULL, ZERO);

    if (argc != 1) 
	Syntax(app_con, argv[0]);

    XtAppAddActions(app_con, actionTable, XtNumber(actionTable));

    /* Two buttons, with callback routines, in a box. */

    box = XtCreateManagedWidget("box", boxWidgetClass, toplevel, NULL, ZERO);

    button = XtCreateManagedWidget("go", commandWidgetClass, box, NULL, ZERO);
    XtAddCallback(button, XtNcallback, PopupPrompt, NULL);

    button = XtCreateManagedWidget("quit", commandWidgetClass, box, NULL,ZERO);
    XtAddCallback(button, XtNcallback, Quit, NULL);

    XtRealizeWidget(toplevel);
    XtAppMainLoop(app_con);
}

/*	Function Name: PopupPrompt
 *	Description: Creates and pops up the Dialog widget to ask for a new
 *                   button color.
 *	Arguments: button - the command button that activated this function.
 *                 client_data, call_data - *** UNUSED ***
 *	Returns: none.
 */

/*ARGSUSED*/
static void 
PopupPrompt(button, client_data, call_data)
Widget	button;		
XtPointer client_data, call_data;
{
    Arg		args[5];
    Widget	popup, dialog;
    Position	x, y;
    Dimension	width, height;
    Cardinal	n;

    /*
     * This will position the upper left hand corner of the popup at the
     * center of the widget which invoked this callback, which will also
     * become the parent of the popup.  I don't deal with the possibility
     * that the popup will be all or partially off the edge of the screen.
     */

    n = 0;
    XtSetArg(args[0], XtNwidth, &width); n++;
    XtSetArg(args[1], XtNheight, &height); n++;
    XtGetValues(button, args, n);
    XtTranslateCoords(button, (Position) (width / 2), (Position) (height / 2),
		      &x, &y);

    n = 0;
    XtSetArg(args[n], XtNx, x);				n++;
    XtSetArg(args[n], XtNy, y);				n++;

    popup = XtCreatePopupShell("prompt", transientShellWidgetClass, button,
			       args, n);

    /* 
     * The popup will contain a dialog box, prompting the user for input. 
     */

    dialog = XtCreateManagedWidget("dialog", dialogWidgetClass, popup,NULL, 0);

    /*
     * The prompting message's size is dynamic; allow it to request resize. 
     */

    XawDialogAddButton(dialog, "ok", ColorTheButton, (XtPointer) dialog);
    XawDialogAddButton(dialog, "cancel", DestroyPopupPrompt,(XtPointer)dialog);

    /*
     * In this example, it is not necessary to call XtRealizeWidget.
     * There are two situations where you may want to realize the
     * popup before calling XtPopup:
     *
     * If you build your popups upon application startup, the time required
     * to execute XtPopup is reduced if you realize your popups first.
     *
     * If you want to insure that the popup is not clipped by screen
     * boundaries, realize the popup before calling XtGetValues on it.
     *
     * The dialog popup will allow user input to the rest of the application
     * when the grab kind is GrabNone.   Since I allow input to the rest of
     * the application and since I don't destroy the popup before creating a
     * new one, I am allowing multiple identical popups, one for each time 
     * the user clicks on the main application button.
     *
     * Try it with XtGrabExclusive instead of XtGrabNone.  Will the user
     * be able to quit the application while the popup is up?  Does the
     * command button which causes the popup to happen remain highlighted?
     */

    XtPopup(popup, XtGrabNone);
}

/*	Function Name: Quit
 *	Description: exits the application.
 *	Arguments: widget, client_data, call_data - *** UNUSED ***.
 *	Returns: none
 */

/*ARGSUSED*/
static void 
Quit(widget, client_data, call_data)
Widget	widget;		
XtPointer client_data, call_data;
{
    XtDestroyApplicationContext(XtWidgetToApplicationContext(widget));
    exit(0);
}

/*	Function Name: ColorTheButton
 *	Description: This is a callback function that performs the
 *                   actual task of coloring in the command button.
 *	Arguments: w - *** UNUSED ***.
 *                 client_data - a pointer to the dialog widget.
 *                 call_data - *** UNUSED ***.
 *	Returns: none
 */

/*ARGSUSED*/
static void 
ColorTheButton(w, client_data, call_data)
Widget	w;		
XtPointer client_data, call_data;	
{
    Widget dialog = (Widget) client_data;
    Widget button  = XtParent(XtParent(dialog)); 
    int pixel;
    String cname = XawDialogGetValueString(dialog);
    Arg	args[3];

    pixel = ConvertColor(button, cname);

    if (pixel > 0) {
	XtSetArg(args[0], XtNbackground, (Pixel) pixel);
	XtSetValues(button, args, ONE);
	DestroyPopupPrompt(NULL, (XtPointer) dialog, (XtPointer)NULL);
    }
    else {			/* pixel < 0  ==> error. */
	char str[BUFSIZ];
	(void) sprintf(str, "Can't get color \"%s\".  Try again.",  cname);
	   
	XtSetArg(args[0], XtNlabel, str);
	XtSetArg(args[1], XtNvalue, "");
	XtSetValues(dialog, args, TWO);
    }
}

/*	Function Name: DestroyPopupPrompt
 *	Description: Destroys the popup dialog widget.
 *	Arguments: w - *** UNUSED ***.
 *                 client_data - the dialog widget.  This widget is a direct
 *                               child of the popup shell to destroy.
 *                 call_data - *** UNUSED **.
 *	Returns: none.
 */

/*ARGSUSED*/
static void 
DestroyPopupPrompt(widget, client_data, call_data)
Widget	widget;		
XtPointer client_data, call_data;	
{
    Widget popup = XtParent( (Widget) client_data);
    XtDestroyWidget(popup);
}

/*	Function Name: Ok
 *	Description: An action routine that is invoked from any child of 
 *                   the dialog widget.  This routine will also color the
 *                   command button that activated the popup.
 *	Arguments: widget - the child of the dialog that caused this action.
 *                 event, params, num_params - *** UNUSED ***.
 *	Returns: none
 */

/*ARGSUSED*/
static void 
Ok(widget, event, params, num_params)
Widget widget;		
XEvent *event;		
String *params;	
Cardinal *num_params;
{
    Widget dialog = XtParent(widget);

    ColorTheButton(widget, (XtPointer) dialog, (XtPointer) NULL);
}

/*	Function Name: ConvertColor
 *	Description: This converts a string into a color.
 *	Arguments: color_name - name of the color.
 *	Returns: a pixel value for that color.
 */

static int
ConvertColor(w, color_name)
Widget w;
char * color_name;
{
  XrmValue from, to;

  from.size = strlen(color_name) + 1;  
  from.addr = color_name;

/*
 * This conversion accepts a colorname from rgb.txt, or a #rrrgggbbb 
 * rgb color definition, or the special toolkit strings "XtDefaultForeground" 
 * and "XtDefaultBackground".
 */

  XtConvert(w, XtRString, (XrmValuePtr) &from, XtRPixel, (XrmValuePtr) &to);
  if (to.addr == NULL) {
      return(-1);
  }

  return( (int) *((Pixel *) to.addr) );
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
    fprintf(stderr, "Usage: %s\n", call);
    exit(1);
}
