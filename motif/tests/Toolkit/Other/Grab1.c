#ifdef REV_INFO
#ifndef lint
static char SCCSID[] = "OSF/Motif: @(#)Grab1.c	3.2  91/01/11";
#endif /* lint */
#endif /* REV_INFO */
/******************************************************************************
*******************************************************************************
*
*  (c) Copyright 1989, 1990, 1991 OPEN SOFTWARE FOUNDATION, INC.
*  (c) Copyright 1989, HEWLETT-PACKARD COMPANY
*  ALL RIGHTS RESERVED
*  
*  	THIS SOFTWARE IS FURNISHED UNDER A LICENSE AND MAY BE USED
*  AND COPIED ONLY IN ACCORDANCE WITH THE TERMS OF SUCH LICENSE AND
*  WITH THE INCLUSION OF THE ABOVE COPYRIGHT NOTICE.  THIS SOFTWARE OR
*  ANY OTHER COPIES THEREOF MAY NOT BE PROVIDED OR OTHERWISE MADE
*  AVAILABLE TO ANY OTHER PERSON.  NO TITLE TO AND OWNERSHIP OF THE
*  SOFTWARE IS HEREBY TRANSFERRED.
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
*  Notice:  Notwithstanding any other lease or license that may pertain to,
*  or accompany the delivery of, this computer software, the rights of the
*  Government regarding its use, reproduction and disclosure are as set
*  forth in Section 52.227-19 of the FARS Computer Software-Restricted
*  Rights clause.
*  
*  (c) Copyright 1989, 1990, 1991 Open Software Foundation, Inc.  Unpublished - all
*  rights reserved under the Copyright laws of the United States.
*  
*  RESTRICTED RIGHTS NOTICE:  Use, duplication, or disclosure by the
*  Government is subject to the restrictions as set forth in subparagraph
*  (c)(1)(ii) of the Rights in Technical Data and Computer Software clause
*  at DFARS 52.227-7013.
*  
*  Open Software Foundation, Inc.
*  11 Cambridge Center
*  Cambridge, MA   02142
*  (617)621-8700
*  
*  RESTRICTED RIGHTS LEGEND:  This computer software is submitted with
*  "restricted rights."  Use, duplication or disclosure is subject to the
*  restrictions as set forth in NASA FAR SUP 18-52.227-79 (April 1985)
*  "Commercial Computer Software- Restricted Rights (April 1985)."  Open
*  Software Foundation, Inc., 11 Cambridge Center, Cambridge, MA  02142.  If
*  the contract contains the Clause at 18-52.227-74 "Rights in Data General"
*  then the "Alternate III" clause applies.
*  
*  (c) Copyright 1989, 1990, 1991 Open Software Foundation, Inc.
*  ALL RIGHTS RESERVED 
*  
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
#include <signal.h>
#include <stdio.h>
#include <X11/Xlib.h>
#include <X11/Intrinsic.h>
#include <X11/StringDefs.h>
#include <X11/Shell.h>
#include <X11/Vendor.h>
#include <Xm/Xm.h>
#include <Xm/Label.h>
#include <Xm/LabelG.h>
#include <Xm/MessageB.h>
#include <Xm/RowColumn.h>
#include <Xm/RowColumnP.h>
#include <Xm/PushB.h>
#include <Xm/PushBG.h>
#include <Xm/ToggleB.h>
#include <Xm/ToggleBG.h>
#include <Xm/Frame.h>

#include "Grab1.h"

#define MAX_ARGS  100

/*************Some Globals***********************/

Widget toplevel; 
Widget rc0, rc1, rc2;
Widget frame1, frame2;
Widget arrow1, arrow2, arrow3, arrow5, arrow7, arrow9;
Widget arrow11;
Widget toggle1, toggle2;

Boolean trace = 0;
char automatic = 0;
static void GetOptions();
XtActionProc Quit();
XtActionProc GotKey();

XtActionsRec actions[] = {
   {"Quit", (XtActionProc)Quit},
   {"GotKey", (XtActionProc)GotKey}
};

static String translations =
   "~Ctrl ~Shift ~Meta<Key>q: Quit()\n\
    <Key>: GotKey()";

XtTranslations Xlations;

/************************************************/


XtActionProc Quit ()

{
     printf("Begin exiting ... please standby ... \n");
     fflush(stdout);
     exit(0);
}

void PrintName (w)

   Widget w;

{
    printf ("Widget %s was activated\n", XrmQuarkToString(w->core.xrm_name));
}

void PrintName_2 (rc, client_data, rc_data)

   XmRowColumnWidget rc;
   caddr_t client_data;
   XmRowColumnCallbackStruct * rc_data;

{
    printf ("Widget %s was activated\n", 
             XrmQuarkToString(rc_data->widget->core.xrm_name));
}

static XtCallbackRec PrintName_CB[] = {
   {PrintName_2, NULL},
   {NULL, NULL}
};


AddAllTabGroups()
{
   /* Add in correct order */
   XmAddTabGroup(rc1);
   XmAddTabGroup(rc2);
}

XtActionProc GotKey (w, event)

   Widget w;
   XEvent * event;

{
   printf ("Widget %s received a key event\n", w->core.name);
}


/*************End of CALLBACK PROCEDURES****************/


void main (argc, argv)
unsigned int argc;
char **argv;

{
    Arg args[15];
    int n;
    XEvent event;
    KeyCode keycode;
    Display        *display;

    signal (SIGINT, QuitCB);

    toplevel = XtInitialize(
        argv[0],                        /* application name */
        "genericclass",                 /* application class */
        NULL, 0,                        /* options */
        &argc, argv);                   /* command line parameters */

    XtSetArg (args[0], XtNallowShellResize, True) ;
    XtSetValues (toplevel, args, 1) ;


   /****************************************************************/
   /*  Create the of the top-level frame and row-column manager    */
   /****************************************************************/

    n = 0;
    rc0 = XmCreateRowColumn(toplevel, "rc0", args, n);
    XtManageChild(rc0);


   /******************************************************/
   /*  Create the children of the row column manager     */
   /******************************************************/

    n = 0;
    XtSetArg (args[n], XmNshadowThickness, 2);		n++;
    frame1 = (Widget)XmCreateFrame(rc0, "frame1", args, n);
    XtManageChild(frame1);

    n = 0;
    rc1 = XmCreateRowColumn(frame1, "rc1", args, n);
    XtManageChild(rc1);

    n = 0;
    XtSetArg (args[n], XmNshadowThickness, 2);		n++;
    frame2 = (Widget)XmCreateFrame(rc0, "frame2", args, n);
    XtManageChild(frame2);

    n = 0;
    XtSetArg(args[n], XmNradioAlwaysOne, True);		n++;
    rc2 = XmCreateRadioBox(frame2, "rc2", args, n);
    XtManageChild(rc2);

   /****************************************************************/
   /*  Create the widget children of the second row-column manager  */
   /****************************************************************/

   n = 0;
   XtSetArg (args[n], XmNhighlightThickness, 2);	n++;
   XtSetArg (args[n], XmNtraversalOn, True);		n++;
   XtSetArg (args[n], XmNhighlightOnEnter, True);	n++;
   toggle1 = XmCreateToggleButtonGadget(rc2, "toggle1", args, n);
   XtManageChild(toggle1);

   n = 0;
   XtSetArg (args[n], XmNhighlightThickness, 2);	n++;
   XtSetArg (args[n], XmNtraversalOn, True);		n++;
   XtSetArg (args[n], XmNhighlightOnEnter, True);	n++;
   toggle2 = XmCreateToggleButtonGadget(rc2, "toggle2", args, n);
   XtManageChild(toggle2);

   /****************************************************************/
   /*  Create the widget children of the first row-column manager  */
   /****************************************************************/

   n = 0;
   XtSetArg (args[n], XmNhighlightThickness, 2);	n++;
   XtSetArg (args[n], XmNtraversalOn, True);		n++;
   XtSetArg (args[n], XmNhighlightOnEnter, True);	n++;
   arrow1 = XmCreatePushButtonGadget(rc1, "Button1", args, n);
   XtManageChild(arrow1);

   n = 0;
   XtSetArg (args[n], XmNhighlightThickness, 2);	n++;
   XtSetArg (args[n], XmNtraversalOn, True);		n++;
   XtSetArg (args[n], XmNhighlightOnEnter, True);	n++;
   arrow3 = XmCreatePushButtonGadget(rc1, "Button2", args, n);
   XtManageChild(arrow3);

   n = 0;
   XtSetArg (args[n], XmNhighlightThickness, 2);	n++;
   XtSetArg (args[n], XmNtraversalOn, True);		n++;
   XtSetArg (args[n], XmNhighlightOnEnter, True);	n++;
   arrow5 = XmCreatePushButtonGadget(rc1, "Button3", args, n);
   XtManageChild(arrow5);

   n = 0;
   XtSetArg (args[n], XmNhighlightThickness, 2);	n++;
   XtSetArg (args[n], XmNtraversalOn, True);		n++;
   XtSetArg (args[n], XmNhighlightOnEnter, True);	n++;
   arrow7 = XmCreatePushButtonGadget(rc1, "Button4", args, n);
   XtManageChild(arrow7);

   n = 0;
   XtSetArg (args[n], XmNhighlightThickness, 2);	n++;
   XtSetArg (args[n], XmNtraversalOn, True);		n++;
   XtSetArg (args[n], XmNhighlightOnEnter, True);	n++;
   arrow9 = XmCreatePushButtonGadget(rc1, "Button5", args, n);
   XtManageChild(arrow9);

   n = 0;
   XtSetArg (args[n], XmNhighlightThickness, 2);	n++;
   XtSetArg (args[n], XmNtraversalOn, True);		n++;
   XtSetArg (args[n], XmNhighlightOnEnter, True);	n++;
   arrow11 = XmCreatePushButtonGadget(rc1, "Button6", args, n);
   XtManageChild(arrow11);

   AddAllTabGroups(); 

   XtRealizeWidget (toplevel);

   XtAddActions(actions, XtNumber(actions));
   Xlations = XtParseTranslationTable(translations); 

   /* Should generate a warning */
   keycode = XKeysymToKeycode(XtDisplay(arrow11), 'a');
   XtGrabKey(arrow11, keycode, NULL, True, GrabModeAsync, GrabModeAsync);
   XtUngrabKey(arrow11, keycode, NULL);
   XtUngrabKey(toplevel, keycode, NULL);

   /* Grab same key twice */
   XtGrabKey(toplevel, keycode, ShiftMask, False, GrabModeAsync, GrabModeAsync);
   XtGrabKey(toplevel, keycode, ShiftMask, False, GrabModeAsync, GrabModeAsync);

   /* Grab same key with different modifiers */
   keycode = XKeysymToKeycode(XtDisplay(arrow11), 'b');
   XtGrabKey(toplevel, keycode, NULL, False, GrabModeAsync, GrabModeAsync);
   XtGrabKey(toplevel, keycode, ShiftMask, False, GrabModeAsync, GrabModeAsync);

   /* Grab same key with different modifiers */
   keycode = XKeysymToKeycode(XtDisplay(arrow11), 'c');
   XtGrabKey(toplevel, keycode,AnyModifier,False, GrabModeAsync, GrabModeAsync);
   XtGrabKey(toplevel, keycode, ShiftMask, False, GrabModeAsync, GrabModeAsync);

   /* Grab same key with different modifiers */
   keycode = XKeysymToKeycode(XtDisplay(arrow11), 'd');
   XtGrabKey(toplevel, keycode, ShiftMask, False, GrabModeAsync, GrabModeAsync);
   XtGrabKey(toplevel, keycode,AnyModifier,False, GrabModeAsync, GrabModeAsync);
   XtUngrabKey(toplevel, keycode, ShiftMask);
   XtGrabKey(toplevel, keycode, ShiftMask, False, GrabModeAsync, GrabModeAsync);
   XtUngrabKey(toplevel, keycode, ControlMask);

   /* Grab same key with different modifiers */
   keycode = XKeysymToKeycode(XtDisplay(arrow11), 'q');
   XtGrabKey(toplevel, keycode, NULL, False, GrabModeAsync, GrabModeAsync);

   XtOverrideTranslations(toplevel, Xlations); 

   Pause();
   Pause();
   Pause();
   Pause();

   XtMainLoop();
}
    
static void  GetOptions(argc, argv, trace)
    int          argc;
    char       **argv;
    Boolean     *trace;
{
    register int i;

    for (i = 1; i < argc; i++) {
        if (argv[i][0] == '-') {
            switch (argv[i][1]) {
                case 'v':
                        *trace = True;
                        break;
                case 'a':
                        automatic = True;
                        break;
                default:
                        fprintf(stderr, "\nusage:  Grab4_Xt [-va]\n");
                        fprintf(stderr, "        The -v option enables trace output.\n\n");
                        fprintf(stderr, "        The -a option enables automatic execution.\n\n");
                        exit(1);
            }
        }
    }
}


static void  Pause()
{
    ContinuePopup = CreateContinuePopup();

    XtPopup(ContinuePopup, XtGrabNone);

    wait = True;
    while (wait)
        FlushEvents();
    Pause_cnt++;
}


static Widget CreateContinuePopup()
{
    Widget        popup;
    Widget        rowcol;
    Widget        message;
    Widget        exit_button, continue_button;
    Arg           args[MAX_ARGS];
    register int  n;
    XmString	  message_field, continue_field, exit_field;

    /*  create popup shell  */
    n = 0;
    XtSetArg(args[n], XtNgeometry, "+500+0");  n++;

    popup = XtCreatePopupShell("Next Event", topLevelShellWidgetClass,
                               toplevel, args, n);

    /*  create a frame for the pushbutton  */
    n = 0;
    XtSetArg(args[n], XmNmarginWidth,  12);  n++;
    XtSetArg(args[n], XmNmarginHeight, 12);  n++;
    rowcol = XmCreateRowColumn(popup, "frame", args, n);
    XtManageChild(rowcol);

    /*  create a Message Box and Control Continue and Exit */
    n = 0;
    message_field = XmStringCreateLtoR(Pause_events[Pause_cnt], XmSTRING_DEFAULT_CHARSET);
    continue_field = XmStringCreateLtoR("Continue", XmSTRING_DEFAULT_CHARSET);
    exit_field = XmStringCreateLtoR("Exit", XmSTRING_DEFAULT_CHARSET);
    XtSetArg(args[n], XmNmessageString, message_field); n++;
    XtSetArg(args[n], XmNokLabelString, continue_field); n++;
    XtSetArg(args[n], XmNcancelLabelString, exit_field); n++;
    message = (Widget) XmCreateMessageBox(rowcol, "msgbox", args, n);
    XtManageChild(message);
    continue_button = (Widget) XmMessageBoxGetChild(message, XmDIALOG_OK_BUTTON);
    exit_button = (Widget) XmMessageBoxGetChild(message, XmDIALOG_CANCEL_BUTTON);
    XmStringFree(continue_field);
    XmStringFree(message_field);
    XmStringFree(exit_field);
    XtAddCallback(continue_button, XmNactivateCallback, ContinueCB, popup);
    XtAddCallback(exit_button, XmNactivateCallback, QuitCB, popup);

    return(popup);

}


static void  FlushEvents()
{
    XEvent event;

    while (XtPending()) {
        XtNextEvent(&event);
        XtDispatchEvent(&event);
    }
}


static XtCallbackProc  ContinueCB(w, client_data, call_data)
    Widget   w;
    caddr_t  client_data;
    caddr_t  call_data;
{
    wait = False;
    XtPopdown((Widget)client_data);
}


static XtCallbackProc  QuitCB(w, client_data, call_data)
    Widget   w;
    caddr_t  client_data;
    caddr_t  call_data;

{
	printf("Exiting...Please Standby...\n");
	exit(0);
}


