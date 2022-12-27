/******************************************************************************
*******************************************************************************
*
*  (c) Copyright 1989, 1990, 1991 OPEN SOFTWARE FOUNDATION, INC.
*  (c) Copyright 1989 HEWLETT-PACKARD COMPANY.
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

/**---------------------------------------------------------------------
***	
***	file:		xmprotocol.c
***
***	project:	Motif Widgets example programs
***
***	description:	This program demonstrates the Motif protocols
***			for communication between the toolkit and the
***			window manager.
***	
***-------------------------------------------------------------------*/


#include <stdio.h>
#include <sys/signal.h>
  
#include <X11/IntrinsicP.h>
#include <X11/StringDefs.h>
  
#include <X11/Shell.h>
#include <Xm/Xm.h>
#include <Xm/XmP.h>
  
#include <Xm/RowColumn.h>
#include <Xm/ScrollBar.h>
#include <Xm/DrawingA.h>
#include <Xm/ScrolledW.h>
#include <Xm/PushB.h>
  
#include <X11/Protocols.h>
#include <X11/AtomMgr.h>
#include <X11/MwmUtil.h>
  
Atom	wm_delete_window, mwm_messages;
  
  
static Widget	ShellAncestor(w)
    Widget	w;
{
    while ((w != NULL) && !XtIsShell(w))
      w = w->core.parent;
    
    return(w);
}


#define SET_ITEMS 0
#define REMOVE_CALLBACKS 1
#define ACTIVATE_ITEMS 2
#define DEACTIVATE_ITEMS 3
#define REMOVE_PROTOCOLS 4

typedef struct _MenuChangeRec{
    String	label;
    Cardinal	start_num, end_num;
    int		operation;
}MenuChangeRec;






static XtCallbackProc	EntrySelected(w, closure, call_data)
    Widget	w;
    caddr_t	closure, call_data;
{
    printf("entry %d selected\n", closure);
}


void MakeMenu(start_num, end_num, str_rtn)
    Cardinal	start_num, end_num;
    String	str_rtn;
{
    Cardinal	i, j;
    String	str = str_rtn;
    
    if (!start_num && !end_num)
      {
	  *str = '\0';
      }
    else 
      {
	  for (i = start_num; i<= end_num ; i++)
	    {
		sprintf(str, " entry_%d f.send_msg %d \n",i,i);
		str += strlen(str);
	    }
	  *(str-1) = '\0';
      }
}

void ChangeMenu(w, menu_data, call_data)
    Widget      w;
    MenuChangeRec *menu_data;
    caddr_t     call_data;
    
{
    Widget		shell;
    Arg			arg;
    static char		menu_string[256];
    Cardinal		i;
    Atom		atoms[16];
    Boolean		doUnmap = False;
    
    shell = ShellAncestor(w);
    
    switch (menu_data->operation)
      {
	case SET_ITEMS:
	  

	  /*
	   * Generate the string that is passed to mwm for appending
	   * (after interpretation) to the system menu. 
	   */
	  MakeMenu(menu_data->start_num,
		   menu_data->end_num,
		   menu_string);
	  
	  XtSetArg(arg, XmNmwmMenu, menu_string);
	  XtSetValues(shell, &arg, 1);
	  
	  for (i = menu_data->start_num; i<= menu_data->end_num ; i++)
	    /* 
	     * XmAddProtocolCallback will check to see if the protocol
	     * already exists, and add it (by calling XmAddProtocols)
	     * if it doesn't. It doesn't check for duplicate callbacks
	     * or change the state of the activation. 
	     */
	    XmAddProtocolCallback(shell, mwm_messages,
				  (Atom)i,
				  EntrySelected, (caddr_t)i);
          doUnmap = True;
	  break;
	  
	case REMOVE_CALLBACKS:
	  for (i = menu_data->start_num; i<= menu_data->end_num ; i++)
	    /*
	     * RemoveProtocolCallback will remove the first callback
	     * with this signature from the list. It does not check
	     * for duplicates or change the activation (gray-out). 
	     */
	    XmRemoveProtocolCallback(shell, mwm_messages,
				     (Atom)i,
				     EntrySelected, (caddr_t)i);
	  break;

	case REMOVE_PROTOCOLS:
	  for (i = menu_data->start_num; i<= menu_data->end_num ; i++)
	    atoms[i - menu_data->start_num] = i;
	  XmRemoveProtocols(shell, mwm_messages,
			    atoms, 
			    menu_data->end_num - menu_data->start_num +1);
          doUnmap = True;
	  break;
	  
	case ACTIVATE_ITEMS:
	  for (i = menu_data->start_num; i<= menu_data->end_num ; i++)
	    /* 
	     * Tell Mwm these protocols (menu items) are now active
	     */
	    XmActivateProtocol(shell, mwm_messages, (Atom)i);
	  break;
	case DEACTIVATE_ITEMS:
	  for (i = menu_data->start_num; i<= menu_data->end_num ; i++)
	    /* 
	     * Tell Mwm these protocols (menu items) are now inactive
	     */
	    XmDeactivateProtocol(shell, mwm_messages, (Atom)i);
	  break;
      }
    
    if (XtIsRealized(shell) && doUnmap) {
	XtUnmapWidget(shell);
	XtMapWidget(shell);
    }
}

static void Quit()
{
    exit(0);
}


MenuChangeRec	menuops[] = {
    { "add 0-4", 0, 4, SET_ITEMS},
    { "add 1-3", 1, 3, SET_ITEMS},
    { "add 2-2", 2, 2, SET_ITEMS},
    { "activate 0-2" , 0, 2, ACTIVATE_ITEMS},
    { "deactivate 0-2", 0, 2, DEACTIVATE_ITEMS},
    { "activate 2-4" , 2, 4, ACTIVATE_ITEMS},
    { "deactivate 2-4", 2, 4, DEACTIVATE_ITEMS},
};


XtAppContext	app_context;
Display*	display;

void main(argc, argv)
    unsigned int argc;
    char **argv;
{
    int i,items, selected;
    unsigned char policy;
    XEvent	event;
    Widget  	inner_box, toplevel,fw,bb;
    Widget 		buttons[16];
    XmScrolledWindowWidget  sw;
    Widget  	hsb,vsb;
    
    
    
    static Boolean 	BailOut;
    Arg 		myArgs[20];
    
    XmString 	text;
    XmStringCharSet cs = "ISOLatin1";
    
    XFontStruct 	*fnt;
    XmFontList 	font1, font2, font3, font4, font5, curfont;
    int    		NewPbutton;
    


    signal(SIGINT, Quit);
    
    XtToolkitInitialize();
    app_context = XtCreateApplicationContext();
    display = XtOpenDisplay(app_context, NULL,
                argv[0], "XmDemos", NULL, 0, &argc, argv);
    if (!display) { printf("Unable to open display\n"); exit(0); }

    toplevel = XtAppCreateShell(argv[0], NULL,
		applicationShellWidgetClass,
		display, NULL, 0);
    
    /*
     * intern the atoms and add mwm_messages to wm_protocols so that
     * mwm will look for mwm_messages property changes. Also add a
     * callback for handling WM_DELETE_WINDOW for when the user
     * selects the close button.
     */
    mwm_messages = XmInternAtom(XtDisplay(toplevel),
				_XA_MWM_MESSAGES,
				FALSE);
    
    wm_delete_window = XmInternAtom(XtDisplay(toplevel),
				"WM_DELETE_WINDOW",
				FALSE);
    
    XmAddWMProtocolCallback(toplevel, wm_delete_window, Quit, 0);

    XmAddWMProtocols(toplevel, &mwm_messages, 1);
			    
    fnt = XLoadQueryFont(XtDisplay(toplevel), "vr-20");
    font1 = XmFontListCreate(fnt, cs);
    fnt = XLoadQueryFont(XtDisplay(toplevel), "vri-25");
    font2 = XmFontListCreate(fnt, cs);
    fnt = XLoadQueryFont(XtDisplay(toplevel), "vbee-36");
    font3 = XmFontListCreate(fnt, cs);

    /*
     * Added to test PIRs #655 and #874
     *
     *						04/23/90
     */

    fnt = XLoadQueryFont(XtDisplay(toplevel), NULL);
    font4 = XmFontListCreate(fnt, cs);
    font5 = XmFontListAdd(font3, fnt, cs);
    curfont = font1;
    
    i = 0; items = 0; selected = 0;
    XtSetArg(myArgs[i], XmNscrollingPolicy, XmAUTOMATIC); i++;    
    XtSetArg(myArgs[i], XmNwidth, 110); i++;    
    XtSetArg(myArgs[i], XmNheight, 150); i++;    
    sw = (XmScrolledWindowWidget )
      XtCreateManagedWidget("ScrolledWindow", 
			    xmScrolledWindowWidgetClass, toplevel, 
			    (ArgList)myArgs, i);
    
    i = 0;
    XtSetArg (myArgs[i], XmNscrollingPolicy,(XtArgVal) &policy); i++;
    XtGetValues(sw,myArgs,1);
    
    if (policy == XmAPPLICATION_DEFINED)
      {
	  i = 0;
	  XtSetArg (myArgs[i], XmNorientation,(XtArgVal) (XmVERTICAL)); i++;
	  vsb = XtCreateManagedWidget("vScrollBar", xmScrollBarWidgetClass,
				      (Widget) sw, myArgs, i); 
	  i = 0;
	  XtSetArg (myArgs[i], XmNorientation,(XtArgVal) (XmHORIZONTAL)); i++;
	  hsb = XtCreateManagedWidget("hScrollBar", xmScrollBarWidgetClass,
				      (Widget) sw, myArgs, i);
      }
    
    
    i = 0;
    XtSetArg(myArgs[i], XmNborderWidth, (XtArgVal) 2);    i++;
    XtSetArg(myArgs[i], XmNshadowThickness, (XtArgVal) 0);    i++;
    inner_box = XtCreateManagedWidget ("RowCol", xmRowColumnWidgetClass, sw,
				       (ArgList)myArgs, i);
    

    /*
     * Create the button list from the item table. All buttons call
     * the ChangeMenu routine.
     */
    for (items = 0; items < XtNumber(menuops); items++)
      {
	  i = 0;
	  text = XmStringLtoRCreate(menuops[items].label, cs);
	  XtSetArg(myArgs[i], XmNfontList, (XtArgVal) curfont);i++;
	  XtSetArg(myArgs[i], XmNlabelString, (XtArgVal) text);i++;
	  XtSetArg(myArgs[i], XmNlabelType, (XtArgVal) XmSTRING);i++;
	  buttons[items] = 
	    XtCreateManagedWidget(argv[0], xmPushButtonWidgetClass,
				  inner_box, (ArgList)myArgs, i);
	  XtAddCallback(buttons[items], XmNactivateCallback, 
			ChangeMenu, (caddr_t)&menuops[items]);
      }

    i = 0;
    if (policy == XmAPPLICATION_DEFINED)
      {
	  XtSetArg(myArgs[i], XmNhorizontalScrollBar, (XtArgVal) hsb); i++;
	  XtSetArg(myArgs[i], XmNverticalScrollBar, (XtArgVal) vsb); i++;
      }
    XtSetArg(myArgs[i], XmNworkWindow, (XtArgVal)inner_box); i++;
    
    XtSetValues(sw, myArgs, i);
    XtRealizeWidget(toplevel);
    
    XtAppMainLoop(app_context);
  }


