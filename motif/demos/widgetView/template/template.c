/******************************************************************************
*******************************************************************************
*
*  (c) Copyright 1989, 1990 OPEN SOFTWARE FOUNDATION, INC.
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

/****************************************************************************
 ****************************************************************************
 **
 **   File:     @(#)template.c	1.4 - 90/08/01
 **
 **   Project:     Motif - widget examination program
 **
 **   Description: Program which shows resources of widgets
 **
 **   Author:      Pete Levine
 **
 ****************************************************************************
 ****************************************************************************/

#include <template/template.h>

char filename [256]; 

/***********************************************************/
void  Quit()
{
     printf("exiting...\n\r");
     fflush(stdout);
     exit(0);
}

/**********************************************************/
XtCallbackProc MenuQuit (w, client_data, call_data)
Widget   w;
caddr_t  client_data,
         call_data;
{

  exit (0);

}

/**********************************************************/
XtCallbackProc GenericCallback (w, client_data, call_data)
Widget   w;
caddr_t  client_data,
         call_data;
{

  printf ("callback activated, client data = %s\n", client_data);


}

/***********************************************************/
Widget CreateCascade (label, mnemonic, submenu, parent)
char       *label,
           mnemonic;
Widget     submenu,
           parent;

{

  Widget   widget;
  int      n;
  Arg      args[MAX_ARGS];
  XmString tcs;

  n = 0;
  tcs = XmStringLtoRCreate (label, XmSTRING_DEFAULT_CHARSET);
  XtSetArg(args[n], XmNlabelString, tcs); n++;
  XtSetArg(args[n], XmNmnemonic, mnemonic); n++;
  XtSetArg(args[n], XmNsubMenuId, submenu); n++;
  widget = XmCreateCascadeButton (parent, "cascade", args, n);
  XtManageChild(widget);
  XmStringFree(tcs);

  return (widget);
}

/***********************************************************/
Widget CreatePushButton (parent, label, x, y)
Widget    parent;
char      *label;
int       x, y;

{

  static Widget       widget;
  int          n;
  Arg          args[MAX_ARGS];
  XmString     tcs,
               acc_text;

  n = 0;
  tcs = XmStringLtoRCreate(label, XmSTRING_DEFAULT_CHARSET);
  XtSetArg(args[n], XmNx, x);n++;
  XtSetArg(args[n], XmNy, y);n++;
  widget = XmCreatePushButton(parent, label, args, n);
  XtManageChild(widget);
  XmStringFree(tcs);

  return(widget);
}


/***********************************************************/
void  main(argc, argv)
int     argc;
char    **argv;
{
    register int           n;
    Arg           args[MAX_ARGS];
    int           num_items;
    int           num_widgets;
    Widget        widget, GenericRoutine ();

    signal(SIGHUP,  Quit);
    signal(SIGINT,  Quit);
    signal(SIGQUIT, Quit);

    XtToolkitInitialize();
    if ((display = XtOpenDisplay (NULL, NULL, argv[1], "XMclient",
				  NULL, 0, &argc, argv)) == NULL)
    {
	fprintf (stderr,"\n%s:  Can't open display\n", argv[0]);
        exit(1);
    }


    strcpy (filename, argv[0]);
    strcat (filename, ".c");
    printf ("filename = %s\n", filename);

    n = 0;
    XtSetArg(args [n], XmNgeometry, "+0+0");
    XtSetArg(args[n], XmNallowShellResize, True);  n++;
    Shell = XtAppCreateShell(argv[0], NULL, applicationShellWidgetClass,
                              display, args, n);

    n = 0;
    MainWindow = XmCreateMainWindow(Shell, "MainWindow", args, n);
    XtManageChild(MainWindow);

    n = 0;
    XtSetArg(args[n], XmNresizePolicy,  XmRESIZE_GROW);  n++;
    WorkRegion = XmCreateBulletinBoard (MainWindow, "s_text", args, n);
    XtManageChild (WorkRegion);

    /* widget bulletin board */

    n=0;
    XtSetArg(args[n], XmNx,  0);  n++;
    XtSetArg(args[n], XmNy,  60);  n++;
    XtSetArg(args[n], XmNheight,  400);  n++;
    XtSetArg(args[n], XmNwidth,  275);  n++;
    XtSetArg(args[n], XmNshadowThickness, 2);  n++; 
    XtSetArg(args[n], XmNresizePolicy,  XmRESIZE_GROW);  n++;

    WidgetBoard = XmCreateBulletinBoard (WorkRegion, "Form", args, n);
    XtManageChild (WidgetBoard);

    CreateMenuBar ();

    widget = GenericRoutine (WidgetBoard);

    XtRealizeWidget(Shell);

    XtManageChild (widget);

    XtMainLoop();

}


/***********************************************************/
CreateMenuBar ()
{

    int           n;
    Arg           args[MAX_ARGS];


    /*
    **  the menubar
    */

    n = 0;
    MenuBar = (Widget) XmCreateMenuBar ((Widget) MainWindow, "MenuBar", 
					args, n); 
    if (MenuBar == NULL) {
      printf ("null menu bar\n\r");
      exit (0);
    }
    XtManageChild(MenuBar);

    /*  create the first level pulldowns */

    n = 0;
    PullDown1 = XmCreatePulldownMenu (MenuBar, "File", args, n);

    /*  create cascade buttons for menubar  */

    MenuBtn1 = CreateCascade ("File", 'F', PullDown1, MenuBar);
    /*
    **  Menu1 (FILE menu)
    */

    Label1A = CreatePushButton (PullDown1, "Quit", 0, 0);
    XtAddCallback (Label1A, XmNactivateCallback, MenuQuit, NULL);

}








