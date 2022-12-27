#ifdef REV_INFO
#ifndef lint
static char SCCSID[] = "OSF/Motif: @(#)periodic.c	1.13 90/08/09";
#endif /* lint */
#endif /* REV_INFO */
/******************************************************************************
*******************************************************************************
*
*  (c) Copyright 1989, 1990, 1991 OPEN SOFTWARE FOUNDATION, INC.
*  ALL RIGHTS RESERVED
*  
*  	THE INFORMATION IN THIS SOFTWARE IS SUBJECT TO CHANGE WITHOUT
*  NOTICE AND SHOULD NOT BE CONSTRUED AS A COMMITMENT BY OPEN SOFTWARE
*  FOUNDATION, INC. OR ITS THIRD PARTY SUPPLIERS  
*  
*  	OPEN SOFTWARE FOUNDATION, INC. AND ITS THIRD PARTY SUPPLIERS,
*  ASSUME NO RESPONSIBILITY FOR THE USE OR INABILITY TO USE ANY OF ITS
*  SOFTWARE .   OSF SOFTWARE IS PROVIDED "AS IS" WITHOUT WARRANTY OF ANY
*  KIND, AND OSF EXPRESSLY DISCLAIMS ALL IMPLIED WARRANTIES, INCLUDING
*  BUT NOT LIMITED TO THE IMPLIED WARRANTIES OF MERCHANTABILITY AND
*  FITNESS FOR A PARTICULAR PURPOSE.
*  
* Open Software Foundation is a trademark of The Open Software Foundation, Inc.
* OSF is a trademark of Open Software Foundation, Inc.
* OSF/Motif is a trademark of Open Software Foundation, Inc.
* Motif is a trademark of Open Software Foundation, Inc.
* DEC is a registered trademark of Digital Equipment Corporation
* DIGITAL is a registered trademark of Digital Equipment Corporation
* X Window System is a trademark of the Massachusetts Institute of Technology
*
*******************************************************************************
******************************************************************************/

/******************************************************************************
***	defaults:	periodic.c depends on these defaults:
!
periodic*highlightThickness:		1
#ifdef COLOR
periodic*background:			LightGray
#endif
!
***
******************************************************************************/

#include <stdio.h>

#include <Xm/Xm.h>                        /* Motif Toolkit */
#include <Xm/Label.h>
#include <Xm/LabelG.h>
#include <Xm/PushB.h>
#include <Xm/PushBG.h>
#include <Xm/ToggleB.h>
#include <Xm/ToggleBG.h>
#include <Xm/RowColumn.h>
#include <Xm/SeparatoG.h>
#include <Xm/Separator.h>
#include <Mrm/MrmPublic.h>                   /* Mrm Toolkit */

#define MAX_ARGS  100

#define K_scale_widget           1
#define K_drawnbutton_widget     2
#define K_togglebutton_widget    3
#define K_vtext1_widget          4
#define K_vtext2_widget          5
#define K_popup_menu_widget      6

static Widget Scale_1,Drawnbutton_1,Togglebutton_1,VText_1,VText_2,PopMenu_1,PMenu_1;
static Widget Label1, Separator1, PushBtn1, ToggleBtn1;

static MrmHierarchy	s_MrmHierarchy;		/* MRM database hierarch id */
static char		*vec[]={"periodic.uid"};
						/* MRM database file list   */
static MrmCode		class ;

static void quit_button_activate();
static void foo_activate();
static void create_callback();
static void zippy_null();
static void postmenu ();

static MrmCount		regnum = 4 ;
static MrmRegisterArg	regvec[] = {
	{"quit_button_activate",(caddr_t)quit_button_activate},
	{"foo_activate",(caddr_t)foo_activate},
	{"create_callback",(caddr_t)create_callback},
	{"zippy_null",(caddr_t)zippy_null}
	};

static Widget  CreateLabel(label, parent)
    char   *label;
    Widget  parent;
{
    Widget        widget;
    register int  n;
    Arg           args[MAX_ARGS];
    XmString      tcs;


    n = 0;
    tcs = XmStringLtoRCreate(label, XmSTRING_DEFAULT_CHARSET);
    XtSetArg(args[n], XmNlabelString, tcs);  n++;
    widget = XmCreateLabel(parent, "label", args, n);
    XtManageChild(widget);
    XmStringFree(tcs);

    return(widget);
}


static Widget  CreatePushButton(label, parent)
    char   *label;
    Widget  parent;
{
    Widget        widget;
    register int  n;
    Arg           args[MAX_ARGS];
    XmString      tcs;


    n = 0;
    tcs = XmStringLtoRCreate(label, XmSTRING_DEFAULT_CHARSET);
    XtSetArg(args[n], XmNlabelString, tcs);  n++;
    widget = XmCreatePushButton(parent, "pushButton", args, n);
    XtManageChild(widget);
    XmStringFree(tcs);

    return(widget);
}

static Widget  CreateToggle(label, parent)
    char   *label;
    Widget  parent;
{
    Widget        widget;
    register int  n;
    Arg           args[MAX_ARGS];
    XmString      tcs;


    n = 0;
    tcs = XmStringLtoRCreate(label, XmSTRING_DEFAULT_CHARSET);
    XtSetArg(args[n], XmNlabelString, tcs);  n++;
    widget = XmCreateToggleButton(parent, "toggle", args, n);
    XtManageChild(widget);
    XmStringFree(tcs);

    return(widget);
}

XtAppContext	app_context;
Display*	display;

/*
 *  Main program
 */
int main(argc, argv)
unsigned int argc;
char **argv;
{
     /*
     *  Declare the variables to contain the two widget ids
     */
    Widget toplevel, periodic_table_main;
    Arg arglist[1] ;
    int         status;

    /*
     *  Initialize the MRM
     */

    MrmInitialize ();

    /*
     *  Initialize the toolkit.  This call returns the id of the "toplevel"
     *  widget.  The applications "main" widget must be the only child
     *  of this widget.
     */

    XtToolkitInitialize();
    app_context = XtCreateApplicationContext();
    display = XtOpenDisplay(app_context, NULL,
	"periodic",    		/* application name */
	"periodic",              /* application class */
	NULL, 0,                /* options */
	&argc, argv);           /* command line parameters */
    if (!display) { printf("Unable to open display\n"); exit(0); }


    XtSetArg (arglist[0], XtNallowShellResize, FALSE) ;
    toplevel = XtAppCreateShell("periodic", NULL,
		    applicationShellWidgetClass,
		    display, arglist, 1);

    /*
     *  Define the Mrm.hierarchy (only 1 file)
     */

    if (MrmOpenHierarchy (1,			    /* number of files	    */
			vec, 			    /* files     	    */
			NULL,			    /* os_ext_list (null)   */
			&s_MrmHierarchy)	    /* ptr to returned id   */
			!= MrmSUCCESS) {
	printf ("can't open hierarchy\n");
     }

    /*
     * 	Register our callback routines so that the resource manager can 
     * 	resolve them at widget-creation time.
     */

    if (MrmRegisterNames (regvec, regnum)
			!= MrmSUCCESS)
			    printf("can't register names\n");

    /*
     *  Call MRM to fetch and create the pushbutton and its container
     */

    if (MrmFetchWidget (s_MrmHierarchy,
			"periodic_table_main",
			toplevel,
			&periodic_table_main,
			&class)
			!= MrmSUCCESS)
			    printf("can't fetch interface\n");

    /*
     *  Make the toplevel widget "manage" the main window (or whatever the
     *  the uil defines as the topmost widget).  This will
     *  cause it to be "realized" when the toplevel widget is "realized"
     */

    XtManageChild(periodic_table_main);
    
    /*
     *  Realize the toplevel widget.  This will cause the entire "managed"
     *  widget hierarchy to be displayed
     */

    XtRealizeWidget(toplevel);

    /*
    **  PopupMenu1
    */
    PMenu_1 = XmCreatePopupMenu(PopMenu_1, "popMenu", NULL, 0);
    XtAddEventHandler(PopMenu_1, ButtonPressMask, False, postmenu, PMenu_1);

    /*
    **  Menu1
    */
    Label1       = CreateLabel("POPUP MENU", PMenu_1);

    XtSetArg(arglist[0], XmNseparatorType, XmDOUBLE_LINE);
    Separator1   = XmCreateSeparator(PMenu_1, "separator", arglist, 1);

    PushBtn1     = CreatePushButton("pushbutton", PMenu_1);

    ToggleBtn1   = CreateToggle("togglebutton", PMenu_1);


    /*
     *  Loop and process events
     */

    XtAppMainLoop(app_context);

    /* UNREACHABLE */
    return (0);
}

static void zippy_null( widget, tag, callback_data )
	Widget	widget;
	char    *tag;
	XmAnyCallbackStruct *callback_data;
{
}

#define fatal 1
static void  postmenu (w, popup, event)
    Widget         w;
    Widget         popup;
    XButtonEvent  *event;
{
    int         status;
    if (event->button != Button3)
        return;

    XmMenuPosition(PMenu_1, event);
    XtManageChild (PMenu_1);
}

/*
 * All widgets that are created call back to this procedure.
 */

static void create_callback(w, tag, reason)
    Widget w;
    int *tag;
    unsigned long *reason;
{

    /*  For internationalization ease, we capture a few strings from the
     *  widgets themselves.  We could go out and fetch them as needed but
     *  since we use these all the time, this method if more efficient.
     */
    switch (*tag) {
        case K_scale_widget: 
	    Scale_1 = w;
            break;
        case K_drawnbutton_widget: 
	    Drawnbutton_1 = w;
            break;
        case K_togglebutton_widget: 
	    Togglebutton_1 = w;
            break;
        case K_vtext1_widget: 
	    VText_1 = w;
            break;
        case K_vtext2_widget: 
	    VText_2 = w;
            break;
        case K_popup_menu_widget: 
	    PopMenu_1 = w;
            break;
        default: 
            break;
    }
}

static void foo_activate( widget, tag, callback_data )
	Widget	widget;
	char    *tag;
	XmAnyCallbackStruct *callback_data;
{
    Arg arglist[2];
    int scale_value;
    Boolean toggle_state;

    XtSetArg( arglist[0], XmNvalue, &scale_value);
    XtGetValues( Scale_1, arglist, 1 );
    XtSetArg( arglist[0], XmNset, &toggle_state);
    XtGetValues( Togglebutton_1, arglist, 1 );

    if ((scale_value == 13) && (toggle_state == True)) {
	XtSetArg( arglist[0], XmNvalue,
"OSF Motif Credits");
	XtSetValues( VText_1, arglist, 1 );
	XtSetArg( arglist[0], XmNvalue,
"DEV: Vania, Ellis, Mike,\n\
..Alka, Al, Scott,\n\
..Daniel, and Paul\n\
REL: Ron and Martha\n\
Q/A: Libby, Tom, Ken,\n\
..Carl, and Dany\n\
DOC: Ken and Bob\n\
Moral Support:\n\
Mary Chung's Restaurant\n");
	XtSetValues( VText_2, arglist, 1 );
    }

}

static void quit_button_activate( widget, tag, callback_data )
	Widget	widget;
	char    *tag;
	XmAnyCallbackStruct *callback_data;
{
    Arg arglist[2];

    static int call_count = 1;

    call_count += 1 ;
    switch ( call_count )
        {
        case 1:
/*            XtSetArg( arglist[0], XmNlabelString,
                XmStringLtoRCreate("Goodbye\nWorld!",""));
		XtSetValues( Text_1, arglist, 1 );          */
            break ;
        case 2:
            exit(1);
            break ;
        }
}
