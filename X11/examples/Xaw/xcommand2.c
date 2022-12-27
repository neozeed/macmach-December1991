/*
 * This an example of how to use the Command widget.
 *
 * User events handled through application action routines.
 *
 * November 14, 1989 - Chris D. Peterson 
 */

#include <stdio.h>
#include <X11/Intrinsic.h>
#include <X11/StringDefs.h>	/* Get standard string definations. */

#include <X11/Xaw/Command.h>	
#include <X11/Xaw/Cardinals.h>	

static void Syntax();
static XtActionProc SelectAction();

/*
 * NOTE:   \\n is required because the C compiler throws one of them away.
 */

String fallback_resources[] = { 
    "*Command.Label:            Click the any mouse button here",
    "*Command.Translations:     #override \\n\
          <BtnDown>:             set() \\n\
          <BtnDown>,<BtnUp>:     SelectAction() reset()",
    NULL,
};

/*
 * This is a list of string to action bindings, that is used by the
 * Toolkit to bind an action name in a translation table to a function.
 * Use XtAppAddActions() to register this table on your application context.
 */

XtActionsRec actions[] = {
  {"SelectAction",          (XtActionProc) SelectAction},
};

static XrmOptionDescRec options[] = {
{"-label",	"*Command.label",	XrmoptionSepArg,	NULL}
};

main(argc, argv)
int argc;
char **argv;
{
    XtAppContext app_con;
    Widget toplevel, command;

    toplevel = XtAppInitialize(&app_con, "Xcommand2",
			       options, XtNumber(options),
			       &argc, argv, fallback_resources, NULL, ZERO);

    /*
     * Add string to function bindings for out application actions.
     */

    XtAppAddActions(app_con, actions, XtNumber(actions));

    if (argc != 1)		
	Syntax(app_con, argv[0]);

    command = XtCreateManagedWidget("command", commandWidgetClass, toplevel,
				    NULL, ZERO);

    XtRealizeWidget(toplevel);
    XtAppMainLoop(app_con);
}

/*	Function Name: SelectAction
 *	Description: This function prints the button number to stdout.
 *	Arguments: w - ** UNUSED **
 *                 event - the event that caused this action.
 *                 params, num_params - ** NOT USED **
 *	Returns: none
 */

static XtActionProc
SelectAction(w, event, params, num_params)
Widget w;
XEvent * event;
String * params;
Cardinal * num_params;
{
    if (event->type == ButtonRelease) 
	fprintf(stdout, "Button %d Selected.\n", (int) event->xbutton.button);
    else
	fprintf(stdout, "Unknown Event occured.\n");
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
