#ifdef REV_INFO
#ifndef lint
static char SCCSID[] = "OSF/Motif: @(#)ArrowBtn4.c	3.5 91/01/11";
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

/**************************************************************************
 *
 *  Create an arrow using inches and setting the width and height. 
 *  Reset the size, change the unit type, set the size again.
 *
 **************************************************************************/

#include <stdio.h>
#include <signal.h>
#include <X11/Intrinsic.h>
#include <X11/StringDefs.h>
#include <X11/Shell.h>
#include <Xm/RowColumn.h>
#include <Xm/MessageB.h>
#include <Xm/ArrowB.h>

#include "ArrowBtn4.h"

/*  Forward declarations  */

static void flushEvents();
Widget toplevel;

unsigned char UnitType[4] = { Xm1000TH_INCHES, 
			      Xm100TH_POINTS,
  			      Xm100TH_MILLIMETERS,
			      Xm100TH_FONT_UNITS };

void main (argc, argv)
int argc;
char **argv;
{
   Widget 	  arrow;
   Arg 		  args[20];
   int 		  n;
   Display        *display;
   Dimension	  width, height;
   unsigned char  unitType;
   int		  i;

   extern void myQuit();
   signal (SIGINT, myQuit);


   /*  initialize toolkit  */
   XtToolkitInitialize();
   app_context = XtCreateApplicationContext();
   display = XtOpenDisplay(app_context, NULL, argv[0], "XMclient", NULL, 0,
                            &argc, argv);
   if (!display) { printf("Unable to open display\n"); exit(0); }


   /* create message shell */
   n = 0;
   XtSetArg (args[n], XmNallowShellResize, True);        n++;
   toplevel = XtAppCreateShell(argv[0], NULL, applicationShellWidgetClass,
                            display, args, n);

   n = 0;
   XtSetArg(args[n], XmNwidth, 100);		n++;
   XtSetArg(args[n], XmNheight, 100);		n++;
   arrow = XmCreateArrowButton (toplevel, "arrow", args, n);
   XtManageChild (arrow);

   XtRealizeWidget (toplevel);
   flushEvents (XtDisplay(toplevel), 2);

  	/*  Get the size of the widget in unit values  */
	n = 0;
	XtSetArg (args[n], XmNunitType, &unitType);	n++;
	XtSetArg (args[n], XmNwidth, &width);		n++;
	XtSetArg (args[n], XmNheight, &height);		n++;
	XtGetValues (arrow, args, n);
	PrintUnitType (unitType);
	printf("Width = %d , Height = %d\n\n", width, height);

	Pause();

   for (i = 0 ; i < 4; i++)
	{

   	n = 0;
   	XtSetArg (args[n], XmNunitType, UnitType[i]); n++;
   	XtSetValues (arrow, args, n);

  	/*  Get the size of the widget in unit values  */
	n = 0;
  	XtSetArg (args[n], XmNunitType, &unitType);	n++;
	XtSetArg (args[n], XmNwidth, &width);		n++;
   	XtSetArg (args[n], XmNheight, &height);		n++;
   	XtGetValues (arrow, args, n);
   	PrintUnitType (unitType);
   	printf("Width = %d , Height = %d\n\n", width, height);

   	Pause();
	}

   Pause();

   XtAppMainLoop(app_context);
}


PrintUnitType(UnitType)
int UnitType;
{

switch (UnitType) {
	case 0:	
   		printf ("unitType = %s\n", "XmPIXELS");
		break;
	case 1:
   		printf ("unitType = %s\n", "Xm100TH_MILLIMETERS");
		break;
	case 2:
   		printf ("unitType = %s\n", "Xm1000TH_INCHES");
		break;
	case 3:
   		printf ("unitType = %s\n", "Xm100TH_POINTS");
		break;
	case 4: 
		printf ("unitType = %s\n", "Xm100TH_FONT_UNITS");
		break;
	default:
		printf ("Not a valid Unit Type\n");
	};
}


static void flushEvents(display, delay)
Display * display;
int delay;
{
   XEvent event;
   while (XPending(display)) {
      XNextEvent(display, &event);
      XtDispatchEvent(&event);
   }
   sleep(delay);
}

void myQuit()
{
     printf("Begin exiting ... please standby ... \n");
     fflush(stdout);
     exit(0);
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
    message = XmCreateMessageBox(rowcol, "msgbox", args, n);
    XtManageChild(message);
    continue_button = XmMessageBoxGetChild(message, XmDIALOG_OK_BUTTON);
    exit_button = XmMessageBoxGetChild(message, XmDIALOG_CANCEL_BUTTON);
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
	printf("Exiting...Please Standby...\n");
	exit(0);
}


