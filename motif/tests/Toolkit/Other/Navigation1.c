#ifdef REV_INFO
#ifndef lint
static char SCCSID[] = "OSF/Motif: @(#)Navigation1.c	3.5 91/01/11";
#endif /* lint */
#endif /* REV_INFO */
/******************************************************************************
*******************************************************************************
*
*  (c) Copyright 1989, 1990, 1991 OPEN SOFTWARE FOUNDATION, INC.
*  (c) Copyright 1987, 1988, 1989 HEWLETT-PACKARD COMPANY
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
#include <X11/cursorfont.h>
#include <X11/Intrinsic.h>
#include <X11/StringDefs.h>
#include <X11/Shell.h>
#include <X11/Vendor.h>

#include <Xm/Xm.h>
#include <Xm/XmP.h>
#include <Xm/Label.h>
#include <Xm/LabelG.h>
#include <Xm/RowColumn.h>
#include <Xm/RowColumnP.h>
#include <Xm/MenuShell.h>
#include <Xm/PanedW.h>
#include <Xm/PushB.h>
#include <Xm/PushBG.h>
#include <Xm/ToggleB.h>
#include <Xm/ToggleBG.h>
#include <Xm/MainW.h>
#include <Xm/Frame.h>
#include <Xm/Form.h>
#include <Xm/CascadeB.h>
#include <Xm/CascadeBG.h>
#include <Xm/Separator.h>
#include <Xm/SeparatoG.h>
#include <Xm/Text.h>
#include <Xm/MessageB.h>

#include "Navigation1.h"

/*************Some Globals***********************/

Widget toplevel; 
Widget mainWindow;
Widget manager1, rc2;
Widget button1, button2, button3, button4, button5, button6;
Widget pb1, pb2;

Display	*display;

extern CreateOption();

Quit()
{
    fprintf(stdout,"Begin exiting from SIGINT ... please standby ... \n");
    fflush(stdout);
    exit(0);
}


void PrintName(w)
     Widget w;
{
    fprintf(stdout, "Widget %s was activated\n",
	    XrmQuarkToString(w->core.xrm_name));
}

void PrintName_2(rc, client_data, rc_data)
     XmRowColumnWidget		rc;
     caddr_t			client_data;
     XmRowColumnCallbackStruct	*rc_data;

{
    fprintf(stdout, "Widget %s was activated\n", 
	    XrmQuarkToString(rc_data->widget->core.xrm_name));
}

static XtCallbackRec PrintName_CB[] =
{
    {PrintName_2, NULL},
    {NULL, NULL}
};



void SetInsensitive(w)
     Widget w;
{
    Arg args[1];
    int	n;

    XtSetSensitive(w, False);
}



void main (argc, argv)
     unsigned int	argc;
     char		*argv[];

{
    Widget	child[7];
    XEvent	event;
    Arg		args[15];
    int		n;

    signal (SIGINT, Quit);

    XtToolkitInitialize();
    app_context = XtCreateApplicationContext();

    if((display = XtOpenDisplay(app_context, NULL, argv[0], "XMclient", NULL, 0,
				&argc, argv)) == NULL)
    {
        fprintf(stdout, "%s:  Can't open display\n", argv[0]);
        exit(1);
    }

/*
 * Top level shell
 */

    n = 0;
    XtSetArg(args[n], XmNallowShellResize, True);  n++;
    toplevel = XtAppCreateShell(argv[0], NULL, applicationShellWidgetClass,
				display, args, n);

/*
 * case 1: 1.1 Paned Window parent, child of shell
 */

    n = 0;
    manager1 = XmCreatePanedWindow(toplevel, "manager1", (ArgList) args, n);
    XtManageChild(manager1);

    CreateButtons(manager1);
    
    XtRealizeWidget (toplevel);

    Pause();


/*
 *  case 2:         shell
 *             mainWindow (RC)
 *    manager1 (1.0 PW) -- rc2 (1.0 RC)
 */

    XtDestroyWidget(manager1);
    XtUnrealizeWidget (toplevel);

    n = 0;
    XtSetArg (args[n], XmNwidth, 300); n++;
    mainWindow = XmCreateRowColumn(toplevel, "mainWindow", args, n);

    n = 0;
    manager1 = XmCreatePanedWindow(mainWindow, "manager1", (ArgList) args, n);
   
    XmAddTabGroup (manager1);

    CreateButtons(manager1);

    n = 0;
    rc2 = XmCreateRowColumn(mainWindow, "rc2", (ArgList) args, n);
    
    n = 0;
    pb1 = XmCreatePushButton(rc2, "pb1", args, n);
    pb2 = XmCreatePushButton(rc2, "pb2", args, n);

    XmAddTabGroup(rc2);

    XtManageChild(pb1);
    XtManageChild(pb2);
    XtManageChild(rc2);
    XtManageChild(manager1);
    XtManageChild(mainWindow);

    XtRealizeWidget (toplevel);

    Pause();

/*
 *  case 3:      shell
 *             mainWindow (RC)
 *      manager1 (1.1 PW)    rc2 (1.1 RC)
 */
    
    XtDestroyWidget (mainWindow);
    XtUnrealizeWidget (toplevel);

    n = 0;
    XtSetArg (args[n], XmNwidth, 300); n++;
    mainWindow = XmCreateRowColumn(toplevel, "mainWindow", args, n);

    n = 0;
    manager1 = XmCreatePanedWindow(mainWindow, "manager1", (ArgList) args, n);
    XtManageChild(manager1);
   
    CreateButtons(manager1);

    n = 0;
    rc2 = XmCreateRowColumn(mainWindow, "rc2", (ArgList) args, n);
    
    n = 0;
    pb1 = XmCreatePushButton(rc2, "pb1", args, n);
    pb2 = XmCreatePushButton(rc2, "pb2", args, n);
    XtManageChild(pb1);
    XtManageChild(pb2);
    XtManageChild(rc2);
    XtManageChild(mainWindow);

    XtRealizeWidget (toplevel);

    Pause();

/*
 *  case 4:      shell
 *           mainWindow (Form)
 *     manager1 (1.0 PW) rc2 (RC)
 */

    XtDestroyWidget(mainWindow);
    XtUnrealizeWidget (toplevel);

    n = 0;
    mainWindow = XmCreateForm(toplevel, "mainWindow", args, n);

    n = 0;
    XtSetArg(args[n], XmNtopAttachment, XmATTACH_FORM); n++;
    manager1 = XmCreatePanedWindow(mainWindow, "manager1", (ArgList) args, n);
    XtManageChild(manager1);
   
    XmAddTabGroup (manager1);

    CreateButtons(manager1);

    n = 0;
    XtSetArg(args[n], XmNtopWidget, manager1); n++;
    XtSetArg(args[n], XmNtopAttachment, XmATTACH_WIDGET); n++;
    rc2 = XmCreateRowColumn(mainWindow, "rc2", (ArgList) args, n);
    
    n = 0;
    pb1 = XmCreatePushButton(rc2, "pb1", args, n);
    pb2 = XmCreatePushButton(rc2, "pb2", args, n);
    XtManageChild(pb1);
    XtManageChild(pb2);
    XtManageChild(rc2);
    XmAddTabGroup(rc2);
    XtManageChild(mainWindow);

    XtRealizeWidget (toplevel);

    Pause();

/*  
 *  case 5:  Note, we are keeping mainWindow this time.
 *           shell
 *         mainWindow (Form)
 *           manager1 (RC)
 */

    XmRemoveTabGroup(manager1);
    XtDestroyWidget(manager1);
    XmRemoveTabGroup(rc2);
    XtDestroyWidget(rc2);

    n = 0;
    XtSetArg(args[n], XmNpacking, XmPACK_COLUMN); n++;
    XtSetArg(args[n], XmNnumColumns, 3); n++;
    manager1 = XmCreateRowColumn(mainWindow, "manager1", (ArgList) args, n);
    XtManageChild(manager1);

    CreateButtons(manager1);
    
    Pause();
    Pause();

    XtAppMainLoop(app_context);
}

CreateButtons(parent)
{
    button1 = XmCreatePushButtonGadget(parent, "button1", NULL, 0);
    XtAddCallback(button1, XmNactivateCallback, SetInsensitive, NULL);
    XtManageChild(button1);

    button2 = XmCreatePushButtonGadget(parent, "button2", NULL, 0);
    XtAddCallback(button2, XmNactivateCallback, SetInsensitive, NULL);
    XtManageChild(button2);

    button3 = XmCreatePushButtonGadget(parent, "button3", NULL, 0);
    XtAddCallback(button3, XmNactivateCallback, SetInsensitive, NULL);
    XtManageChild(button3);

    button4 = XmCreatePushButtonGadget(parent, "button4", NULL, 0);
    XtAddCallback(button4, XmNactivateCallback, SetInsensitive, NULL);
    XtManageChild(button4);

    button5 = XmCreatePushButtonGadget(parent, "button5", NULL, 0);
    XtAddCallback(button5, XmNactivateCallback, SetInsensitive, NULL);
    XtManageChild(button5);

    button6 = XmCreatePushButtonGadget(parent, "button6", NULL, 0);
    XtAddCallback(button6, XmNactivateCallback, SetInsensitive, NULL);
    XtManageChild(button6);
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
    Widget        exit_button, continue_button, help_button;
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
    message = XmCreateMessageBox(rowcol, "msgbox", args, n);
    XtManageChild(message);
    continue_button = XmMessageBoxGetChild(message, XmDIALOG_OK_BUTTON);
    exit_button = XmMessageBoxGetChild(message, XmDIALOG_CANCEL_BUTTON);
    help_button = XmMessageBoxGetChild(message, XmDIALOG_HELP_BUTTON);
    XmStringFree(continue_field);
    XmStringFree(message_field);
    XmStringFree(exit_field);
    XtAddCallback(continue_button, XmNactivateCallback, ContinueCB, popup);
    XtAddCallback(exit_button, XmNactivateCallback, QuitCB, popup);
    XtAddCallback(help_button, XmNactivateCallback, SimpleMessage, "help button");

    return(popup);

}


static void  FlushEvents()
{
    XEvent event;

    while (XtAppPending(app_context)) {
        XtAppNextEvent(app_context, &event);
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
  	printf("Exiting from QuitButton...Please Standby...\n");
	exit(0);
}

static XtCallbackProc  SimpleMessage (w, client_data, call_data)
    Widget   w;
    caddr_t  client_data;
    caddr_t  call_data;

{
    printf ("Activate callback from %s\n", (char *) client_data);
    fflush (stdout);
}
