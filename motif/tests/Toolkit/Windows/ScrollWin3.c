#ifdef REV_INFO
#ifndef lint
static char SCCSID[] = "OSF/Motif: @(#)ScrollWin3.c	3.5 91/01/11";
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
#include "ScrollWin3.h"
#include "allheaders.h"

#define	SLIDER_SIZE	10

static XtArgVal GetColor(colorstr)
char *colorstr;
{
   XrmValue from, to;

   from.size = strlen(colorstr) +1;
   if (from.size < sizeof(String)) from.size = sizeof(String);
   from.addr = colorstr;
   to.addr = NULL;
   XtConvert(Shell1, XmRString, &from, XmRPixel, &to);

    if (to.addr != NULL)
      return ((Pixel) *((Pixel *) to.addr));
    else
      return ( (XtArgVal) NULL);
}

void CreateScrollBars(parent)
Widget parent;
{
    Arg args[10];
    int n;

    n = 0;
    XtSetArg (args[n], XmNorientation, XmVERTICAL); 		n++;
    XtSetArg (args[n], XmNbackground, GetColor("aquamarine"));	n++;
    XtSetArg (args[n], XmNmaximum, 60);				n++;
    XtSetArg (args[n], XmNminimum, 0);				n++;
    XtSetArg (args[n], XmNtraversalOn, True);			n++;
    XtSetArg (args[n], XmNhighlightOnEnter, True);		n++;
    XtSetArg (args[n], XmNhighlightThickness, 2);		n++;
    XtSetArg (args[n], XmNsliderSize, SLIDER_SIZE);		n++;
    vertSB = XmCreateScrollBar (parent, "vertSB", args, n);
    XtManageChild (vertSB);

    XtAddCallback (vertSB, XmNvalueChangedCallback, ChangeCallback , NULL);

    n = 0;
    XtSetArg (args[n], XmNorientation, XmHORIZONTAL); 		n++;
    XtSetArg (args[n], XmNbackground, GetColor("aquamarine"));	n++;
    XtSetArg (args[n], XmNmaximum, 59);				n++;
    XtSetArg (args[n], XmNminimum, 0);				n++;
    XtSetArg (args[n], XmNtraversalOn, True);			n++;
    XtSetArg (args[n], XmNhighlightOnEnter, True);		n++;
    XtSetArg (args[n], XmNhighlightThickness, 2);		n++;
    XtSetArg (args[n], XmNsliderSize, SLIDER_SIZE);		n++;
    horizSB = XmCreateScrollBar (parent, "horizSB", args, n);

    XtAddCallback (horizSB, XmNvalueChangedCallback, ChangeCallback , NULL);

    XtManageChild (horizSB);
}

static XtCallbackProc ChangeCallback(w, client_data, call_data)
Widget   w;
int 	 client_data;
caddr_t  call_data;
{
    int val_h, val_v, val;
    Arg args[5];
    int n = 0;

    n = 0;
    XtSetArg(args[n], XmNvalue, &val_h);  		n++;
    XtGetValues (horizSB, args, n);

    n = 0;
    XtSetArg(args[n], XmNvalue, &val_v);  		n++;
    XtGetValues (vertSB, args, n);

    val = (51 * (val_v / SLIDER_SIZE)) + val_h;

    n = 0;
    XtSetArg(args[n], XmNcursorPosition, val);		n++;
    XtSetValues (Text1, args, n);
}


void  main(argc, argv)
    int     argc;
    char  **argv;
{
    Boolean       trace = False;
    register int  n;
    Arg           args[MAX_ARGS];
    XmString      tcs;
    Pixel         colorVal;
    Dimension	  height, width;
    char	  *value = "This is a long string which will test the         \nscrolling Features of an Application created      \nwith XmNscrollingPolicy set to XmAPPLICTION_      \nDEFINED. It should be contained on four lines     \nwith very long text horizontally. It will not     \nbe editable so dont try to modify any text!!!     ";

    signal(SIGHUP,  Quit);
    signal(SIGINT,  Quit);
    signal(SIGQUIT, Quit);

    XtToolkitInitialize();
    app_context = XtCreateApplicationContext();
    display = XtOpenDisplay(app_context, NULL, argv[0], "XMclient",
                            NULL, 0, &argc, argv);
    if (display == NULL) {
        fprintf(stderr, "%s:  Can't open display\n", argv[0]);
        exit(1);
    }

    n = 0;
    XtSetArg(args[n], XmNallowShellResize, True);  n++;
    Shell1 = XtAppCreateShell(argv[0], NULL, applicationShellWidgetClass,
                              display, args, n);

    n = 0;
    XtSetArg(args[n], XmNscrollingPolicy, XmAPPLICATION_DEFINED);  	n++;
    XtSetArg(args[n], XmNscrollBarPlacement, XmBOTTOM_RIGHT);		n++;
    XtSetArg(args[n], XmNbackground, GetColor("aquamarine"));		n++;
    XtSetArg(args[n], XmNspacing, 10);					n++;
    XtSetArg(args[n], XmNscrolledWindowMarginHeight, 20);		n++;
    XtSetArg(args[n], XmNscrolledWindowMarginWidth, 20);		n++;
    ScrolledWindow2 = XmCreateScrolledWindow(Shell1, "ScrolledWindow2", 
	args, n);

    n=0;
    XtSetArg (args[n], XmNbackground, GetColor("green")); 		n++;
    DrawingArea1 = XmCreateDrawingArea
		(ScrolledWindow2, "DrawingArea1", args , n);
    XtManageChild (DrawingArea1);

    n = 0;
    XtSetArg (args[n], XmNeditMode, XmMULTI_LINE_EDIT);			n++;
    XtSetArg (args[n], XmNeditable, False);				n++;
    XtSetArg (args[n], XmNrows, 4);					n++;
    XtSetArg (args[n], XmNcolumns, 30);					n++;
    XtSetArg (args[n], XmNwordWrap, False);				n++;
    XtSetArg (args[n], XmNvalue, value);				n++;
    Text1 = XmCreateText(DrawingArea1, "Text1", args, n);
    XtManageChild (Text1);

    CreateScrollBars (ScrolledWindow2);
    XmScrolledWindowSetAreas (ScrolledWindow2, horizSB, vertSB, DrawingArea1);
    XtManageChild(ScrolledWindow2);

    XtRealizeWidget(Shell1);

    Pause();
    Pause();

    XtDestroyWidget(ScrolledWindow2);

    n = 0;
    XtSetArg(args[n], XmNscrollingPolicy, XmAPPLICATION_DEFINED);  	n++;
    XtSetArg(args[n], XmNvisualPolicy, XmCONSTANT);			n++;
    XtSetArg(args[n], XmNscrollBarDisplayPolicy, XmAS_NEEDED);		n++;
    XtSetArg(args[n], XmNscrollBarPlacement, XmTOP_RIGHT);		n++;
    XtSetArg(args[n], XmNbackground, GetColor("aquamarine"));		n++;
    XtSetArg(args[n], XmNspacing, 10);					n++;
    XtSetArg(args[n], XmNscrolledWindowMarginHeight, 20);		n++;
    XtSetArg(args[n], XmNscrolledWindowMarginWidth, 20);		n++;
    ScrolledWindow2 = XmCreateScrolledWindow(Shell1, "ScrolledWindow2", 
	args, n);

    n=0;
    XtSetArg (args[n], XmNbackground, GetColor("green")); 		n++;
    DrawingArea1 = XmCreateDrawingArea
		(ScrolledWindow2, "DrawingArea1", args , n);
    XtManageChild (DrawingArea1);

    n = 0;
    XtSetArg (args[n], XmNeditMode, XmMULTI_LINE_EDIT);			n++;
    XtSetArg (args[n], XmNeditable, False);				n++;
    XtSetArg (args[n], XmNrows, 4);					n++;
    XtSetArg (args[n], XmNcolumns, 30);					n++;
    XtSetArg (args[n], XmNwordWrap, False);				n++;
    XtSetArg (args[n], XmNvalue, value);				n++;
    Text1 = XmCreateText(DrawingArea1, "Text1", args, n);
    XtManageChild (Text1);

    CreateScrollBars (ScrolledWindow2);
    XmScrolledWindowSetAreas (ScrolledWindow2, horizSB, vertSB, DrawingArea1);
    XtManageChild(ScrolledWindow2);

    Pause();

    XtDestroyWidget(ScrolledWindow2);

    n = 0;
    XtSetArg(args[n], XmNscrollingPolicy, XmAPPLICATION_DEFINED);  	n++;
    XtSetArg(args[n], XmNscrollBarDisplayPolicy, XmSTATIC);		n++;
    XtSetArg(args[n], XmNscrollBarPlacement, XmBOTTOM_LEFT);		n++; 
    XtSetArg(args[n], XmNbackground, GetColor("aquamarine"));		n++;
    XtSetArg(args[n], XmNspacing, 10);					n++;
    XtSetArg(args[n], XmNscrolledWindowMarginHeight, 20);		n++;
    XtSetArg(args[n], XmNscrolledWindowMarginWidth, 20);		n++;
    ScrolledWindow2 = XmCreateScrolledWindow(Shell1, "ScrolledWindow2", 
	args, n);

    n=0;
    XtSetArg (args[n], XmNbackground, GetColor("green")); 		n++;
    DrawingArea1 = XmCreateDrawingArea
		(ScrolledWindow2, "DrawingArea1", args , n);
    XtManageChild (DrawingArea1);

    n = 0;
    XtSetArg (args[n], XmNeditMode, XmMULTI_LINE_EDIT);			n++;
    XtSetArg (args[n], XmNeditable, False);				n++;
    XtSetArg (args[n], XmNrows, 4);					n++;
    XtSetArg (args[n], XmNcolumns, 30);					n++;
    XtSetArg (args[n], XmNwordWrap, False);				n++;
    XtSetArg (args[n], XmNvalue, value);				n++;
    Text1 = XmCreateText(DrawingArea1, "Text1", args, n);
    XtManageChild (Text1);

    CreateScrollBars (ScrolledWindow2);
    XmScrolledWindowSetAreas (ScrolledWindow2, horizSB, vertSB, DrawingArea1);
    XtManageChild(ScrolledWindow2);

    Pause();

    XtDestroyWidget(ScrolledWindow2);

    n = 0;
    XtSetArg(args[n], XmNscrollingPolicy, XmAUTOMATIC);  	n++; 
    XtSetArg(args[n], XmNscrollBarDisplayPolicy, XmSTATIC);	n++; 
    XtSetArg(args[n], XmNscrollBarPlacement, XmBOTTOM_LEFT);		n++;
    XtSetArg(args[n], XmNbackground, GetColor("aquamarine"));		n++;
    XtSetArg(args[n], XmNspacing, 10);					n++;
    XtSetArg(args[n], XmNscrolledWindowMarginHeight, 20);		n++;
    XtSetArg(args[n], XmNscrolledWindowMarginWidth, 20);		n++;
    ScrolledWindow2 = XmCreateScrolledWindow(Shell1, "ScrolledWindow2", 
	args, n);

    n=0;
    XtSetArg (args[n], XmNbackground, GetColor("green")); 		n++;
    DrawingArea1 = XmCreateDrawingArea
		(ScrolledWindow2, "DrawingArea1", args , n);
    XtManageChild (DrawingArea1);

    n = 0;
    XtSetArg (args[n], XmNeditMode, XmMULTI_LINE_EDIT);			n++;
    XtSetArg (args[n], XmNeditable, False);				n++;
    XtSetArg (args[n], XmNrows, 4);					n++;
    XtSetArg (args[n], XmNcolumns, 30);					n++;
    XtSetArg (args[n], XmNwordWrap, False);				n++;
    XtSetArg (args[n], XmNvalue, value);				n++;
    Text1 = XmCreateText(DrawingArea1, "Text1", args, n);
    XtManageChild (Text1);

/*    CreateScrollBars (ScrolledWindow2); */
    XmScrolledWindowSetAreas (ScrolledWindow2, NULL, NULL, DrawingArea1);
    XtManageChild(ScrolledWindow2);


    Pause();
    XtAppMainLoop(app_context);
}


static void  Quit()
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
                               Shell1, args, n);

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

