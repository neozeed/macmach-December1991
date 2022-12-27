#ifdef REV_INFO
#ifndef lint
static char SCCSID[] = "OSF/Motif: @(#)xmmenushel.c	1.5 90/08/01";
#endif /* lint */
#endif /* REV_INFO */
/******************************************************************************
*******************************************************************************
*
*  (c) Copyright 1989, 1990 OPEN SOFTWARE FOUNDATION, INC.
*  (c) Copyright 1989 HEWLETT-PACKARD COMPANY
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
* X Window System is a trademark of the Massachusetts Institute of Technology
*
*******************************************************************************
******************************************************************************/

/* Popup Menu and Submenus created with MenuShells */

#include <Xm/Xm.h>
#include <Xm/MenuShell.h>
#include <Xm/PushBG.h>
#include <Xm/CascadeBG.h>
#include <Xm/RowColumn.h>


/*********** Callback for the Pushbuttons ***********************/

void ButtonCB (w, client_data, call_data) 
Widget	w;		/*  widget id		*/
caddr_t	client_data;	/*  data from application   */
caddr_t	call_data;	/*  data from widget class  */
{
   /*  print message and terminate program  */
   printf ("Button %s selected.\n", client_data);
	
}

/************** Event Handler for Popup Menu ************************/

PostIt (w, popup, event)
Widget w;
Widget popup;
XButtonEvent * event;
{
   if (event->button != Button2)
	return;
   XmMenuPosition(popup, event);
   XtManageChild(popup);
}


/***********************Main Logic for Program ********************/

XtAppContext	app_context;
Display*	display;

void main (argc, argv)
int argc;
char **argv;
{
   Widget toplevel, rc, buttons[2];
   Widget popupshell, mshell1, mshell2, mshell3;
   Widget popup, submenu1, submenu2, submenu3;
   Widget popupBtn[2], sub1Btn[3], sub2Btn[2], sub3Btn[2];
   Arg args[5];
   XmString string;

/* Initialize toolkit */
   
   XtToolkitInitialize();
   app_context = XtCreateApplicationContext();
   display = XtOpenDisplay(app_context, NULL,
		argv[0], "PopupMenu", NULL, 0, &argc, argv);
    if (!display) { printf("Unable to open display\n"); exit(0); }

    toplevel = XtAppCreateShell(argv[0], NULL,
		applicationShellWidgetClass,
		display, NULL, 0);

/* Create RowColumn in toplevel with two pushbuttons */

   XtSetArg(args[0], XmNwidth, 150);
   XtSetArg(args[1], XmNheight, 50);
   XtSetArg(args[2], XmNresizeWidth, False);
   XtSetArg(args[3], XmNresizeHeight, False);
   XtSetArg(args[4], XmNadjustLast, False);
   rc = XmCreateRowColumn(toplevel, "rc", args, 5);  
   XtManageChild(rc);

   buttons[0] = XmCreatePushButtonGadget(rc, "button1", NULL, 0);
   XtAddCallback(buttons[0], XmNactivateCallback, ButtonCB, "1");

   buttons[1] = XmCreatePushButtonGadget(rc, "button2", NULL, 0);
   XtAddCallback(buttons[1], XmNactivateCallback, ButtonCB, "2");
   XtManageChildren(buttons, 2);

/* Create MenuShell for a Popup MenuPane */

   XtSetArg(args[0], XmNheight, 100);
   XtSetArg(args[1], XmNwidth, 100); 
   popupshell = XmCreateMenuShell(rc, "popupshell", args, 2);

/* Create RowColumn Widget configured as Popup MenuPane */

   XtSetArg(args[0], XmNrowColumnType, XmMENU_POPUP);
   popup = XmCreateRowColumn (popupshell, "popup", args, 1);
   XtAddEventHandler(rc, ButtonPressMask, False, PostIt, popup);

/* Create MenuShells and Pulldown MenuPanes for two submenus */

   XtSetArg(args[0], XmNheight, 100);
   XtSetArg(args[1], XmNwidth, 100);
   mshell1 = XmCreateMenuShell (popupshell, "mshell1", args, 2);

   XtSetArg(args[0], XmNrowColumnType, XmMENU_PULLDOWN);
   submenu1 = XmCreateRowColumn (mshell1, "submenu1", args, 1);

   XtSetArg(args[0], XmNheight, 100);
   XtSetArg(args[1], XmNwidth, 100); 
   mshell2 = XmCreateMenuShell (popupshell, "mshell2", args, 2);

   XtSetArg(args[0], XmNrowColumnType, XmMENU_PULLDOWN);
   submenu2 = XmCreateRowColumn (mshell2, "submenu2", args, 1);

/* Create two Cascade Buttons in the Popup MenuPane */

   string = XmStringCreate("First Submenu", XmSTRING_DEFAULT_CHARSET);
   XtSetArg(args[0], XmNlabelString, string);
   XtSetArg(args[1], XmNsubMenuId, submenu1);
   popupBtn[0] = XmCreateCascadeButtonGadget(popup, "cbutton1", args, 2);
   XmStringFree(string);

   string = XmStringCreate("Second Submenu", XmSTRING_DEFAULT_CHARSET);
   XtSetArg(args[0], XmNlabelString, string);
   XtSetArg(args[1], XmNsubMenuId, submenu2); 
   popupBtn[1] = XmCreateCascadeButtonGadget(popup, "cbutton2", args, 2);
   XmStringFree(string);

   XtManageChildren (popupBtn, 2);

/* Create pushbuttons in MenuPanes submenu1 and submenu2 */

   sub1Btn[0] = XmCreatePushButtonGadget(submenu1, "button1a", NULL, 0);
   XtAddCallback(sub1Btn[0], XmNactivateCallback, ButtonCB, "1a");

   sub1Btn[1] = XmCreatePushButtonGadget(submenu1, "button1b", NULL, 0);
   XtAddCallback(sub1Btn[1], XmNactivateCallback, ButtonCB, "1b");

   sub2Btn[0] = XmCreatePushButtonGadget(submenu2, "button2a", NULL, 0);
   XtAddCallback(sub2Btn[0], XmNactivateCallback, ButtonCB, "2a");

   sub2Btn[1] = XmCreatePushButtonGadget(submenu2, "button2b", NULL, 0);
   XtAddCallback(sub2Btn[1], XmNactivateCallback, ButtonCB, "2b");
   XtManageChildren (sub2Btn, 2);


/* Create a MenuShell for the submenu of submenu1 */

   XtSetArg(args[0], XmNheight, 100);
   XtSetArg(args[1], XmNwidth, 100);
   mshell3 = XmCreateMenuShell (mshell1, "mshell3", args, 2);

/* Create the MenuPane for the submenu of submenu1 */

   XtSetArg(args[0], XmNrowColumnType, XmMENU_PULLDOWN);
   submenu3 = XmCreateRowColumn (mshell3, "submenu3", args, 1);

/* Create the Cascade Button in submenu1 for accessing submenu3 */

   string = XmStringCreate("To Third Submenu", XmSTRING_DEFAULT_CHARSET);
   XtSetArg(args[0], XmNlabelString, string);
   XtSetArg(args[1], XmNsubMenuId, submenu3);
   sub1Btn[2] = XmCreateCascadeButtonGadget(submenu1, "cbutton3", args, 2);
   XmStringFree(string);

   XtManageChildren(sub1Btn, 3);

/* Create pushbuttons in submenu */

   sub3Btn[0] = XmCreatePushButtonGadget(submenu3, "button3a", NULL, 0);
   XtAddCallback(sub3Btn[0], XmNactivateCallback, ButtonCB, "3a");

   sub3Btn[1] = XmCreatePushButtonGadget(submenu3, "button3b", NULL, 0);
   XtAddCallback(sub3Btn[1], XmNactivateCallback, ButtonCB, "3b");
   XtManageChildren (sub3Btn, 2);

/* Get and dispatch events */

   XtRealizeWidget(toplevel);

   XtAppMainLoop(app_context);
}

