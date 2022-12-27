#ifdef REV_INFO
#ifndef lint
static char SCCSID[] = "OSF/Motif: @(#)xmoption.c	1.6 90/08/01";
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
* DEC is a registered trademark of Digital Equipment Corporation
* DIGITAL is a registered trademark of Digital Equipment Corporation
* X Window System is a trademark of the Massachusetts Institute of Technology
*
*******************************************************************************
******************************************************************************/

/* Option Menu Example */

#include <Xm/Xm.h>
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
   printf ("Option %s selected.\n", client_data);
	
}

/***********************Main Logic for Program ********************/


XtAppContext	app_context;
Display*	display;

void main (argc, argv)
int argc;
char **argv;
{
   Widget toplevel, pulldown1, pulldown2, rc;
   Widget option_menus[2], options1[3], options2[3];
   Arg args[6];
   XmString string;

/* Initialize toolkit */
   XtToolkitInitialize();
   app_context = XtCreateApplicationContext();
   display = XtOpenDisplay(app_context, NULL,
   		argv[0], "OptionMenu", NULL, 0, &argc, argv);
   if (!display) { printf("Unable to open display\n"); exit(0); }

   toplevel = XtAppCreateShell(argv[0], NULL,
		applicationShellWidgetClass,
		display, NULL, 0);

/* Create RowColumn in toplevel */

   XtSetArg(args[0], XmNwidth, 405);
   XtSetArg(args[1], XmNheight, 75);
   XtSetArg(args[2], XmNresizeWidth, False);
   XtSetArg(args[3], XmNresizeHeight, False);
   XtSetArg(args[4], XmNnumColumns, 2);
   XtSetArg(args[5], XmNpacking, XmPACK_COLUMN);
   rc = XmCreateRowColumn(toplevel, "rc", args, 6);
   XtManageChild(rc);

/* Create two pulldown menus in rc */

   pulldown1 = (Widget)XmCreatePulldownMenu(rc, "pulldown1", NULL, 0);

   string = XmStringCreate("A-option",XmSTRING_DEFAULT_CHARSET);
   XtSetArg(args[0], XmNlabelString, string); 
   XtSetArg(args[1], XmNmnemonic, 'A'); 
   options1[0] = XmCreatePushButtonGadget(pulldown1, "option1a", args, 2);
   XtAddCallback(options1[0], XmNactivateCallback, ButtonCB, "1A");
   XmStringFree(string);

   string = XmStringCreate("B-option",XmSTRING_DEFAULT_CHARSET);
   XtSetArg(args[0], XmNlabelString, string);
   XtSetArg(args[1], XmNmnemonic, 'B');
   options1[1] = XmCreatePushButtonGadget(pulldown1, "option1b", args, 2);
   XtAddCallback(options1[1], XmNactivateCallback, ButtonCB, "1B");
   XmStringFree(string);

   string = XmStringCreate("C-option",XmSTRING_DEFAULT_CHARSET);
   XtSetArg(args[0], XmNlabelString, string);
   XtSetArg(args[1], XmNmnemonic, 'C');
   options1[2] = XmCreatePushButtonGadget(pulldown1, "option1c", args, 2);
   XtAddCallback(options1[2], XmNactivateCallback, ButtonCB, "1C");
   XtManageChildren(options1, 3);
   XmStringFree(string);


   pulldown2 = (Widget)XmCreatePulldownMenu(rc, "pulldown2", NULL, 0);

   string = XmStringCreate("A-option",XmSTRING_DEFAULT_CHARSET);
   XtSetArg(args[0], XmNlabelString, string); 
   XtSetArg(args[1], XmNmnemonic, 'A'); 
   options2[0] = XmCreatePushButtonGadget(pulldown2, "option2a", args, 2);
   XtAddCallback(options2[0], XmNactivateCallback, ButtonCB, "2A");
   XmStringFree(string);

   string = XmStringCreate("B-option",XmSTRING_DEFAULT_CHARSET);
   XtSetArg(args[0], XmNlabelString, string);
   XtSetArg(args[1], XmNmnemonic, 'B');
   options2[1] = XmCreatePushButtonGadget(pulldown2, "option2b", args, 2);
   XtAddCallback(options2[1], XmNactivateCallback, ButtonCB, "2B");
   XmStringFree(string);

   string = XmStringCreate("C-option",XmSTRING_DEFAULT_CHARSET);
   XtSetArg(args[0], XmNlabelString, string);
   XtSetArg(args[1], XmNmnemonic, 'C');
   options2[2] = XmCreatePushButtonGadget(pulldown2, "option2c", args, 2);
   XtAddCallback(options2[2], XmNactivateCallback, ButtonCB, "2C");
   XtManageChildren(options2, 3);
   XmStringFree(string);


/* Create option menus and attach the two pulldown menus */

   string = XmStringCreate("First Option Set",XmSTRING_DEFAULT_CHARSET);
   XtSetArg(args[0], XmNlabelString, string);
   XtSetArg(args[1], XmNmnemonic, 'F');
   XtSetArg(args[2], XmNsubMenuId, pulldown1);
   XtSetArg(args[3], XmNmenuHistory, options1[2]);
   option_menus[0] = XmCreateOptionMenu(rc, "option_menu1", args, 4);
   XmStringFree(string);

   string = XmStringCreate("Second Option Set",XmSTRING_DEFAULT_CHARSET);
   XtSetArg(args[0], XmNlabelString, string);
   XtSetArg(args[1], XmNmnemonic, 'S');
   XtSetArg(args[2], XmNsubMenuId, pulldown2); 
   XtSetArg(args[3], XmNmenuHistory, options2[0]);
   option_menus[1] = XmCreateOptionMenu(rc, "option_menu2", args, 4);
   XtManageChildren(option_menus, 2);
   XmStringFree(string);

/* Get and dispatch events */

   XtRealizeWidget(toplevel);

   XtAppMainLoop(app_context);
}

