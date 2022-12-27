#ifdef REV_INFO
#ifndef lint
static char SCCSID[] = "OSF/Motif: @(#)widgets.c	1.8 90/08/14";
#endif /* lint */
#endif /* REV_INFO */
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
 **   File:     @(#)widgets.c	1.8 - 90/08/14
 **
 **   Project:     Motif - widget examination program
 **
 **   Description: Program which shows resources of widgets
 **
 **   Author:      Pete Levine
 **
 **
 **
 **
 ****************************************************************************
 ****************************************************************************/
#define WIDGETS 1

#include "widgetView.h"
#include "widgets.h"
#include "globals.h"
#include "callbacks.h"


/**********************************************************/
SetLabel (label)
char *label;
{
    register int  n;
    Arg           args[MAX_ARGS];
    XmString      tcs;

    n = 0;

    tcs = XmStringLtoRCreate(label, XmSTRING_DEFAULT_CHARSET);
    XtSetArg(args[n], XmNlabelString, tcs);  n++;
    XtSetValues (LabelW, args, n);
    XmStringFree(tcs);

}

/***********************************************************/
Widget CreateCascade (label, mnemonic, submenu, parent)
char       *label,
           mnemonic;
Widget     submenu,
           parent;

{

  static Widget   widget;
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
Widget CreateDialog (parent, addrstr, num_items)
Widget parent;
XmString *addrstr;
Cardinal num_items;
{
  static Widget   widget,
           OKBtn,
           HelpBtn;
  int      n;
  Arg      args[MAX_ARGS];
  XmString tcs, tcs2;


  tcs = XmStringLtoRCreate ("List", XmSTRING_DEFAULT_CHARSET);

  n = 0;
  XtSetArg(args[n], XmNwidth, 350);  n++;
  XtSetArg(args[n], XmNheight, 300);  n++;
  /* the label of the selection box list */
  XtSetArg (args[n], XmNlistLabelString, tcs); n++;
  tcs2 = XmStringLtoRCreate (TESTLABEL, XmSTRING_DEFAULT_CHARSET);

  /* the label of the cancel box */
  XtSetArg (args[n], XmNhelpLabelString, tcs2); n++;

  /* the contents of the selection box list */
  XtSetArg (args[n], XmNlistItems, addrstr); n++;

  /* there are eight items */
  XtSetArg (args[n], XmNlistItemCount, num_items); n++;

  /* eight of which are visible */
  XtSetArg (args[n], XmNlistVisibleItemCount, 8); n++;

  widget = XmCreateSelectionBox (parent, "selection_dialog",
				    args, n);

  XmStringFree (tcs);
  XmStringFree (tcs2);

  return (widget);
}

/***********************************************************/
Widget CreateDialogShell (parent, say, x, y)
Widget parent;
char *say;
Cardinal x, y;
{

  static   Widget   widget;
  int      n;
  Arg      args[MAX_ARGS];

  n = 0;
  XtSetArg(args[n], XmNx, x);  n++;
  XtSetArg(args[n], XmNy, y);  n++;
  XtSetArg(args[n], XmNallowShellResize, True);  n++;
  widget = XmCreateDialogShell (parent, say, args, n);


  return (widget);
}

/***********************************************************/
Widget CreateList (parent, addrstr, num_items)
Widget parent;
XmString *addrstr;
Cardinal num_items;
{
  static Widget   widget,
           OKBtn,
           HelpBtn;
  int      n;
  Arg      args[MAX_ARGS];
  XmString tcs;


  tcs = XmStringLtoRCreate ("Color List", XmSTRING_DEFAULT_CHARSET);

/*  n = 0;
  ScrollBarList = XmCreateScrollBar (parent, "Scroll_bar", args, n);
*/
  n = 0;
  XtSetArg(args[n], XmNheight,  200);  n++;
  XtSetArg(args[n], XmNwidth,  250);  n++;
  XtSetArg(args[n], XmNx, 10);  n++;
  XtSetArg(args[n], XmNy, 10);  n++;
  /* the label of the selection box list */
  XtSetArg (args[n], XmNlistLabelString, tcs); n++;

  /* the contents of the selection box list */
  XtSetArg (args[n], XmNitems, addrstr); n++;

  /* there are eight items */
  XtSetArg (args[n], XmNitemCount, num_items); n++;

  /* eight of which are visible */
  XtSetArg (args[n], XmNvisibleItemCount, 20); n++;

  XtSetArg (args[n], XmNselectionPolicy, XmSINGLE_SELECT); n++;
/*  
  XtSetArg (args[n], XmNverticalScrollBar, ScrollBarList); n++;
*/
  widget = XmCreateScrolledList (parent, "selection_dialog",
				    args, n);
  return (widget);
}
/***********************************************************/
Widget CreateFileSelection (parent, name, filter)
Widget parent;
char *name, *filter;
{

  static   Widget   widget;
  int      n = 0;
  Arg      args[MAX_ARGS];
  XmString     tcs;

  n = 0;
  XtSetArg (args[n], XmNwidth, 550); 	n++;
  XtSetArg (args[n], XmNheight, 450); 	n++;
  widget = XmCreateFileSelectionBox (parent, name, args, n);

  tcs = XmStringLtoRCreate(filter, XmSTRING_DEFAULT_CHARSET);

  if (filter) {
    n = 0;
    XtSetArg (args[n], XmNdirMask, tcs); n++;
    XtSetValues (widget, args, n);
  }

  XmStringFree(tcs);

  return (widget);
}

/***********************************************************/
Widget CreatePushButton (parent, label, x, y, width, height)
Widget    parent;
char      *label;
int       x, y;
int       width, height;

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
/*  XtSetArg(args[n], XmNlabelType, tcs);n++;
*/
  if (width && height) {
    XtSetArg(args[n], XmNwidth, width);n++;
    XtSetArg(args[n], XmNheight, height);n++;
  }
  widget = XmCreatePushButton(parent, label, args, n);
  XtManageChild(widget);
  XmStringFree(tcs);

  return(widget);
}


/***********************************************************/
Widget CreateRadioBox (parent)
Widget     parent;
{
  static Widget       widget;
  int          n;
  Arg          args[MAX_ARGS];

  n = 0;
  /*
   * contains two toggle buttons which by virtue of being contained
   * within a radio box are mutually exclusive
   */
  widget = XmCreateRadioBox (parent, "radioBox", args, n);
  XtManageChild (widget);
  
  return (widget);
}


/***********************************************************/
Widget CreateSeparator (parent)
Widget     parent;
{
  static Widget       widget;
  Arg          args[MAX_ARGS];
  int          n;

  n = 0;
  widget = XmCreateSeparator (parent, "separator", args, n);
  XtManageChild (widget);

  return(widget);
}


/***********************************************************/
Widget CreateToggle (parent, label, mnemonic, x, y, indicator)
Widget     parent;
char       *label,
           mnemonic;
int        x, y;
Boolean    indicator;

{
  static Widget       widget;
  int          n;
  Arg          args[MAX_ARGS];
  XmString     tcs;


  n = 0;

  XtSetArg(args[n], XmNx, x);n++;
  XtSetArg(args[n], XmNy, y);n++;
  XtSetArg(args[n], XmNshadowType, XmSHADOW_OUT);n++;
  ToggleFrame = XmCreateFrame (parent, "Frame", args, n);
  XtManageChild (ToggleFrame);

  n = 0;
  tcs = XmStringLtoRCreate (label, XmSTRING_DEFAULT_CHARSET);
  XtSetArg(args[n], XmNlabelString, tcs);n++;
  XtSetArg(args[n], XmNmnemonic, mnemonic);n++;
  XtSetArg(args[n], XmNset, indicator);n++; 
  widget = XmCreateToggleButton (ToggleFrame, "toggle",args,n);
  XtManageChild (widget);
  XmStringFree(tcs);

  return(widget);

}


/***********************************************************/
Widget CreateLabel (parent, string, posx, posy)
Widget parent;
char *string;
int posx, posy;
{


  static Widget       widget;
  Arg          args[MAX_ARGS];
  XmString tcs;
  XmFontList   fontlist;
  int n=0;
  Dimension width, height;
  

  tcs = XmStringLtoRCreate (string, XmSTRING_DEFAULT_CHARSET);
/*
  XtSetArg(args[n], XmNx, posx);  n++;
  XtSetArg(args[n], XmNy, posy);  n++;
  XtSetArg(args[n], XmNresizePolicy,  XmRESIZE_ANY);  n++;
  XtSetArg(args[n], XmNshadowThickness, 2);  n++;
  LabelBoardW = XmCreateFrame (parent, "Frame", args, n);
  XtManageChild (LabelBoardW);
*/

  n = 0;
  XtSetArg(args[n], XmNx, posx);  n++;
  XtSetArg(args[n], XmNy, posy);  n++;
  XtSetArg(args[n], XmNborderWidth, 2);  n++;
  XtSetArg(args[n], XmNrecomputeSize, True);  n++;
  XtSetArg(args[n], XmNlabelString, tcs); n++;
  widget = XmCreateLabel (parent, "Label", args, n);
  XtManageChild (widget);

  XmStringFree (tcs);

  /* return the bulletin board so a subsequent Xtdestroy
     will kill both the bboard and the label */
  return (LabelBoardW);
}

/***********************************************************/
Widget CreateColorLabel (parent, string, posx, posy, color)
Widget parent;
char *string;
int posx, posy;
long color;
{


  static Widget       widget;
  Arg          args[MAX_ARGS];
  XmString tcs;
  XmFontList   fontlist;
  int n=0;
  Dimension width, height;
  

  tcs = XmStringLtoRCreate (string, XmSTRING_DEFAULT_CHARSET);

  XtSetArg(args[n], XmNx, posx);  n++;
  XtSetArg(args[n], XmNy, posy);  n++;
  LabelBoardW = XmCreateFrame (parent, "Frame", args, n);
  XtManageChild (LabelBoardW);

  n = 0;
  XtSetArg(args[n], XmNx, 0);  n++;
  XtSetArg(args[n], XmNy, 0);  n++;
  XtSetArg(args[n], XmNrecomputeSize, True);  n++;
  XtSetArg(args[n], XmNlabelString, tcs); n++;
  XtSetArg(args[n], XmNforeground, color); n++;
  widget = XmCreateLabel (LabelBoardW, "Label", args, n);
  XtManageChild (widget);

  XmStringFree (tcs);

  /* return the bulletin board so a subsequent Xtdestroy
     will kill both the bboard and the label */
  return (LabelBoardW);
}

/***********************************************************/
Widget CreateText (parent, buffer, x, y, length)
Widget parent;
char *buffer;
int x, y, length;
{

  static Widget widget;
  Arg          args[MAX_ARGS];
  int n=0;

  n = 0;
  XtSetArg(args[n], XmNmaxLength,  MAX_TEXT_INPUT_LEN);  n++;
  XtSetArg(args[n], XmNresizeWidth,  False);  n++;
  XtSetArg(args[n], XmNeditMode,  XmSINGLE_LINE_EDIT);  n++;
  XtSetArg(args[n], XmNresizeHeight,  False);  n++;
  XtSetArg(args[n], XmNcolumns, length);  n++;
  XtSetArg (args[n], XmNx,  x);  n++;
  XtSetArg (args[n], XmNy,  y);  n++;
  widget = XmCreateText (parent, "s_text", args, n);
  XmTextSetString (widget, buffer);
  XtManageChild (widget);

  return (widget);
}
/***********************************************************/
Widget CreateTextInputField (parent, buffer, x, y, length)
Widget parent;
char *buffer;
int x, y, length;
{

  Widget widget, wid1, wid2, Wid;
  Arg          args[MAX_ARGS];
  int n=0;


  widget = CreateText (parent, buffer, x, y, length);

/*
  n=0;
  XtSetArg(args[n], XmNx,  x);  n++;
  XtSetArg(args[n], XmNy,  y);  n++;
  XtSetArg(args[n], XmNshadowThickness, 0);  n++; 
  XtSetArg(args[n], XmNspacing,  0);  n++;
  XtSetArg(args[n], XmNwidth,  137);  n++;
  XtSetArg(args[n], XmNheight, 44);  n++;
  XtSetArg(args[n], XmNscrollingPolicy, XmAUTOMATIC);  n++; 
  wid1 = XmCreateScrolledWindow (parent, "s_text", args, n);


  n=0;
  XtSetArg(args[n], XmNshadowThickness, 0);  n++; 
  XtSetArg(args[n], XmNwidth,  200);  n++;
  XtSetArg(args[n], XmNheight, 10);  n++;
  XtSetArg(args[n], XmNmarginHeight, 1);  n++;
  XtSetArg(args[n], XmNmarginWidth, 1);  n++;
  XtSetArg(args[n], XmNresizePolicy,  XmRESIZE_GROW);  n++;
  wid2 = XmCreateBulletinBoard (wid1, "Form", args, n);


  XmScrolledWindowSetAreas (wid1, NULL, NULL, wid2);

  XtManageChild (wid2);
  XtManageChild (wid1);

  n = 0;
  XtSetArg(args[n], XmNmaxLength,  MAX_TEXT_INPUT_LEN);  n++;
  XtSetArg(args[n], XmNresizeWidth,  True);  n++;
  XtSetArg(args[n], XmNeditMode,  XmSINGLE_LINE_EDIT);  n++;
  XtSetArg(args[n], XmNresizeHeight,  False);  n++;
  XtSetArg(args[n], XmNcolumns, 21);  n++;

  widget = XmCreateText (wid2, "sctext", args, n);

  if (buffer != (char *) NULL)
    XmTextSetString(widget, buffer);


  XtManageChild (widget);
  n = 0;
  XtSetArg(args[n], XmNcolumns, 20);  n++;
  XtSetValues (widget, args, n);
*/

  return (widget);
}

/***********************************************************/
Widget CreateMultilineText (parent, x, y, width, height)
Widget parent;
int x, y, width, height;
{

  static Widget widget;
  Arg    args[MAX_ARGS];
  int n=0;

  n = 0;
  XtSetArg(args[n], XmNeditMode,  XmMULTI_LINE_EDIT);  n++;
  XtSetArg(args[n], XmNeditable, True);  n++;
  XtSetArg (args[n], XmNx,  x);  n++;
  XtSetArg (args[n], XmNy,  y);  n++;
  XtSetArg (args[n], XmNwidth,  width);  n++;
  XtSetArg (args[n], XmNheight,  height);  n++;
  XtSetArg(args[n], XmNresizeWidth,  True);  n++;

  XtSetArg (args[n], XmNscrollingPolicy,  XmAUTOMATIC);  n++;

  widget = XmCreateScrolledText (parent, "s_text", args, n);
  XtManageChild (widget);

  return (widget);
}
	  

/***********************************************************/
Widget CreateScale (parent, value, minimum, maximum, xpos, ypos)
Widget parent;
int value,
    minimum,
    maximum,
    xpos,
    ypos;
{

  
  static Widget widget;
  Arg          args[MAX_ARGS];
  int n=0;

    n= 0;
    XtSetArg(args[n], XmNx,  xpos);  n++;
    XtSetArg(args[n], XmNy,  ypos);  n++;
    XtSetArg(args[n], XmNvalue,  value);  n++;
    XtSetArg(args[n], XmNminimum,  minimum);  n++;
    XtSetArg(args[n], XmNmaximum,  maximum);  n++;
    XtSetArg(args[n], XmNshowValue,  True);  n++;
    XtSetArg(args[n], XmNorientation,  XmHORIZONTAL);  n++;
    XtSetArg(args[n], XmNprocessingDirection,  XmMAX_ON_RIGHT);  n++;
    widget = XmCreateScale (parent, "Scale", args, n);
    XtManageChild (widget);
    
    return (widget);

}
