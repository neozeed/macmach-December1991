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
 **   File:     motifShell.c
 **
 **   Project:     Motif - widget examination program
 **
 **   Description: Program which shows resources of widgets
 **
 ****************************************************************************
 ****************************************************************************/

/***************************************************
*                                                  *
*  Revision history:                               *
*                                                  *
*  05/26/89      strong        Initial Version     *
*  06/01/89      strong        1.0                 *
*  06/26/89      pjlevine      complete rewrite    *
*                                                  *
****************************************************/

#include "motifshell.h"

#define STRING_OFFSET 14
#define LABEL0 "Quit"
#define LABEL1 "OSF Membership"
#define LABEL2 "OSF's Research Institute"
#define LABEL3 "OSF's Principles"
#define LABEL4 "OSF/Motif"
#define LABEL5 "Pictures"
#define LABEL6 "Motif Resource Editor"
#define LABEL7 "Periodic Table"
#define LABEL8 "Motif Burger"
#define LABEL10 "Terminal"
#define LABEL11 "File Listing"
#define LABEL12 "Process Status"
#define LABEL12a "Show Source"
#define LABEL13 "Xclock"
#define LABEL14 "Xload"
#define LABEL15 "Kaleidescope"
#define LABEL16 "Load..."
#define LABEL17 "Are you Sure ?"

#define TESTLABEL "Type"
#define GO_AHEAD "This is the selected font...Type something."

int dialog_up = 1;
static XtResourceList resource;
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
XtCallbackProc func1a (w, client_data, call_data)
Widget   w;
caddr_t  client_data,
         call_data;
{

    char *buffer, *GetSource ();
    Arg args[1];

    SetLabel (LABEL1);
    buffer = GetSource ("membership");
    XmTextSetString (TextWin, buffer); 

}

/**********************************************************/
XtCallbackProc func1b (w, client_data, call_data)
Widget   w;
caddr_t  client_data,
         call_data;
{

    char *buffer, *GetSource ();

    SetLabel (LABEL2);
    buffer = GetSource ("research");
    XmTextSetString (TextWin, buffer);

}

/**********************************************************/
XtCallbackProc func1c (w, client_data, call_data)
Widget   w;
caddr_t  client_data,
         call_data;
{

    char *buffer, *GetSource ();

    SetLabel (LABEL3);
    buffer = GetSource ("principles");
    XmTextSetString (TextWin, buffer);


}

/**********************************************************/
XtCallbackProc func1d (w, client_data, call_data)
Widget   w;
caddr_t  client_data,
         call_data;
{

    char *buffer, *GetSource ();

    SetLabel (LABEL4);
    buffer = GetSource ("motif");
    XmTextSetString (TextWin, buffer);

}

/**********************************************************/
XtCallbackProc func2a (w, client_data, call_data)
Widget   w;
caddr_t  client_data,
         call_data;
{

    SetLabel (LABEL5);
    system ("motifgif &");

}

/**********************************************************/
XtCallbackProc func2b (w, client_data, call_data)
Widget   w;
caddr_t  client_data,
         call_data;
{

    SetLabel (LABEL6);
    system ("mre &");

}

/**********************************************************/
XtCallbackProc func2c (w, client_data, call_data)
Widget   w;
caddr_t  client_data,
         call_data;
{

    SetLabel (LABEL7);
    system ("periodic &");

}

/**********************************************************/
XtCallbackProc func2d (w, client_data, call_data)
Widget   w;
caddr_t  client_data,
         call_data;
{

    SetLabel (LABEL8);
    system ("motifbur &");

}

/**********************************************************/
XtCallbackProc func3a (w, client_data, call_data)
Widget   w;
caddr_t  client_data,
         call_data;
{

    system ("xterm &");


}
/**********************************************************/
XtCallbackProc func3b (w, client_data, call_data)
Widget   w;
caddr_t  client_data,
         call_data;
{

    char *buffer, *GetSource ();

    SetLabel (LABEL11);
    system ("ls -al > foo");
    buffer = GetSource ("foo");
    XmTextSetString (TextWin, buffer);
    system ("rm -r foo");

}

/**********************************************************/
XtCallbackProc func3c (w, client_data, call_data)
Widget   w;
caddr_t  client_data,
         call_data;
{

    char *buffer, *GetSource ();

    SetLabel (LABEL12);
    system ("ps -a > foo");
    buffer = GetSource ("foo");
    XmTextSetString (TextWin, buffer);
    system ("rm -r foo");

}

/**********************************************************/
XtCallbackProc func3d (w, client_data, call_data)
Widget   w;
caddr_t  client_data,
         call_data;
{

    char *buffer, *GetSource ();
    Arg args[1];

    SetLabel (LABEL12a);
    buffer = GetSource (filename);
    XtSetArg (args[0], XmNvalue, buffer);
    XtSetValues (TextWin, args, 1);

}

/**********************************************************/
XtCallbackProc func4a (w, client_data, call_data)
Widget   w;
caddr_t  client_data,
         call_data;
{

    system ("xclock &");

}

/**********************************************************/
XtCallbackProc func4b (w, client_data, call_data)
Widget   w;
caddr_t  client_data,
         call_data;
{

    system ("xload &");

}

/**********************************************************/
XtCallbackProc func4c (w, client_data, call_data)
Widget   w;
caddr_t  client_data,
         call_data;
{

    system ("kaleid &");

}

/**********************************************************/
XtCallbackProc help (w, client_data, call_data)
Widget   w;
caddr_t  client_data,
         call_data;
{
    char *buffer, *GetSource ();
  
    XtManageChild (TextShell);
    XtManageChild (TextDialog);  
    XtManageChild (Help_BB);
    XtManageChild (dismiss);
    buffer = GetSource ("help");
    XmTextSetString (TextDialog, buffer);
}

/**********************************************************/
XtCallbackProc ListFonts (w, client_data, call_data)
Widget   w;
caddr_t  client_data,
         call_data;
{
  
    XtManageChild (FontShell);
    XtManageChild (FontBulletin);
    XtManageChild (FontDialog);
    XtManageChild (FontTestDialog);

}

/***********************************************************/
/* for cancel font selection                          */
/***********************************************************/
XtCallbackProc FontSelectCancel (w, client_data, callback_data)
Widget   w;
char *client_data;
XmSelectionBoxCallbackStruct *callback_data;

{
  XtUnmanageChild (FontShell);
  XtUnmanageChild (FontBulletin);
  XtUnmanageChild (FontDialog);
  XtUnmanageChild (FontTestDialog);
}

/***********************************************************/
/* displayed on apply font selection                          */
/***********************************************************/
XtCallbackProc FontSelectApply (w, client_data, callback_data)
Widget   w;
char *client_data;
XmSelectionBoxCallbackStruct *callback_data;

{
  char *textstr, *extract_normal_string ();
  int i, n;
  Arg           args[MAX_ARGS];
  char *buffer, *GetSource ();
  XmFontList fontList;
  char *bgbuffer = GO_AHEAD; 

  XmTextSetString (FontTestDialog, bgbuffer);

  if (callback_data->value == (XmString) NULL)
    return (0);

  textstr = extract_normal_string (callback_data->value);

  if ((mfinfo = XLoadQueryFont(display, textstr))==NULL)
      printf ("couldn't open %s font\n", textstr);
  fontList = XmFontListCreate (mfinfo, " ");
  
  n = 0;
  XtSetArg(args[n], XmNfontList,  fontList);  n++;
  XtSetValues (TextWin, args, n);
  n = 0;
  XtSetArg(args[n], XmNfontList,  fontList);  n++;
  XtSetValues (FontTestDialog, args, n);

}

/***********************************************************/
/* displayed on OK font selection                          */
/***********************************************************/
XtCallbackProc FontSelectOK (w, client_data, callback_data)
Widget   w;
char *client_data;
XmSelectionBoxCallbackStruct *callback_data;

{
  char *textstr, *extract_normal_string ();
  int i, n;
  Arg           args[MAX_ARGS];
  char *buffer, *GetSource ();
  XmFontList fontList;

  if (callback_data->value == (XmString) NULL)
    return (0);

  textstr = extract_normal_string (callback_data->value);

  if ((mfinfo = XLoadQueryFont(display, textstr))==NULL)
      printf ("couldn't open %s font\n", textstr);
  fontList = XmFontListCreate (mfinfo, " ");
  
  n = 0;
  XtSetArg(args[n], XmNfontList,  fontList);  n++;
  XtSetValues (TextWin, args, n);
  n = 0;
  XtSetArg(args[n], XmNfontList,  fontList);  n++;
  XtSetValues (FontTestDialog, args, n);

  XtUnmanageChild (FontShell);
  XtUnmanageChild (FontBulletin);
  XtUnmanageChild (FontDialog);
  XtUnmanageChild (FontTestDialog);

}

/***********************************************************/
/* displayed on test font selection                          */
/***********************************************************/
XtCallbackProc FontTest (w, client_data, callback_data)
Widget   w;
char *client_data;
XmSelectionBoxCallbackStruct *callback_data;

{

  char *textstr, *extract_normal_string ();
  int i, n;
  Arg           args[MAX_ARGS];
  char *buffer, *GetSource ();
  XmFontList fontList;
  char *bgbuffer = GO_AHEAD; 


  XmTextSetString (FontTestDialog, bgbuffer);

  if (callback_data->value == (XmString) NULL)
    return (0);

  textstr = extract_normal_string (callback_data->value);

  if ((mfinfo = XLoadQueryFont(display, textstr))==NULL)
      printf ("couldn't open %s font\n", textstr);
  fontList = XmFontListCreate (mfinfo, " ");
  
  n = 0;
  XtSetArg(args[n], XmNfontList,  fontList);  n++;
  XtSetValues (FontTestDialog, args, n);

}

/**********************************************************/
SetLabel (label)
char *label;
{
    register int  n;
    Arg           args[MAX_ARGS];
    XmString      tcs;

    n = 0;

    tcs = XmStringLtoRCreate(label, XmSTRING_DEFAULT_CHARSET);
    XtSetArg(args[n], XmNlabelString, tcs);  		n++;
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

  Widget   widget;
  int      n;
  Arg      args[MAX_ARGS];
  XmString tcs;

  tcs = XmStringLtoRCreate (label, XmSTRING_DEFAULT_CHARSET);

  n = 0;
  XtSetArg(args[n], XmNlabelString, tcs); 		n++;
  XtSetArg(args[n], XmNmnemonic, mnemonic); 		n++;
  XtSetArg(args[n], XmNsubMenuId, submenu); 		n++;
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
  Widget   widget,
           OKBtn,
           HelpBtn;
  int      n;
  Arg      args[MAX_ARGS];
  XmString tcs, tcs2;


  tcs = XmStringLtoRCreate ("Font List", XmSTRING_DEFAULT_CHARSET);
  tcs2 = XmStringLtoRCreate (TESTLABEL, XmSTRING_DEFAULT_CHARSET);

  n = 0;
  XtSetArg(args[n], XmNheight,  200);  			n++;
  XtSetArg(args[n], XmNwidth,  200);  			n++;
  XtSetArg(args[n], XmNx, 10);  			n++;
  XtSetArg(args[n], XmNy, 10);  			n++;
  XtSetArg (args[n], XmNlistLabelString, tcs);  	n++;
  XtSetArg (args[n], XmNhelpLabelString, tcs2); 	n++;
  XtSetArg (args[n], XmNlistItems, addrstr); 		n++;
  XtSetArg (args[n], XmNlistItemCount, num_items); 	n++;
  XtSetArg (args[n], XmNlistVisibleItemCount, 8); 	n++;

  widget = XmCreateSelectionBox (parent, "selection_dialog",
				    args, n);

  XmStringFree (tcs);
  XmStringFree (tcs2);

  dialog_up = 0;

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
  XtSetArg(args[n], XmNwidth, 400);  n++;
  XtSetArg(args[n], XmNheight, 400);  n++;
  XtSetArg(args[n], XmNx, x);  n++;
  XtSetArg(args[n], XmNy, y);  n++;
  XtSetArg(args[n], XmNallowShellResize, True);  n++;
  widget = XmCreateDialogShell (parent, say, args, n);

  return (widget);
}

/***********************************************************/
Widget CreateFileSelection (parent, name)
Widget parent;
char *name;
{

  static   Widget   widget;
  int      n;
  Arg      args[MAX_ARGS];

  n = 0;
  widget = XmCreateFileSelectionBox (parent, name, args, n);

  return (widget);
}

/***********************************************************/
Widget CreatePushButton (label, mnemonic, parent)
char      *label,
          mnemonic;
Widget    parent;

{

  static Widget       widget;
  int          n;
  Arg          args[MAX_ARGS];
  XmString     tcs,
               acc_text;

  n = 0;
  tcs = XmStringLtoRCreate(label, XmSTRING_DEFAULT_CHARSET);
  acc_text = XmStringLtoRCreate("^A", XmSTRING_DEFAULT_CHARSET);
  XtSetArg(args[n], XmNmnemonic, mnemonic); n++;

  /* if default, extra border */

  if (bit_flag & DEFAULT)
  {
      XtSetArg(args[n], XmNshowAsDefault, 1); n++;
  }

  /* if there is an accelerator associated with the button */

  if (bit_flag & ACCEL)
  {
      XtSetArg(args[n], XmNaccelerator, "Ctrl<Key>A"); 		n++;
      XtSetArg(args[n], XmNacceleratorText, acc_text);		n++;
  }

  /* if the button is inactive; greyed out */

  if (bit_flag & INACTIVE)
  {
      XtSetArg(args[n], XmNsensitive, False); n++;
  }

  widget = XmCreatePushButton(parent, label, args, n);
  XtManageChild(widget);

  XmStringFree(tcs);

  if (bit_flag & ACCEL)
    XmStringFree(acc_text);
  bit_flag = BITSOFF;  

  return(widget);
}

/***********************************************************/
Widget CreateRadioBox (parent)
Widget     parent;
{
  Widget       widget;
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
  Widget       widget;
  Arg          args[MAX_ARGS];
  int          n;

  n = 0;
  widget = XmCreateSeparator (parent, "separator", args, n);
  XtManageChild (widget);

  return(widget);
}


/***********************************************************/
static Widget CreateToggle (label, mnemonic, parent)
char       *label,
           mnemonic;
Widget     parent;

{
  Widget       widget;
  int          n;
  Arg          args[MAX_ARGS];
  XmString     tcs;

  n = 0;
  tcs = XmStringLtoRCreate (label, XmSTRING_DEFAULT_CHARSET);
  XtSetArg(args[n], XmNlabelString, tcs);		n++;
  XtSetArg(args[n], XmNmnemonic, mnemonic);		n++;
  widget = XmCreateToggleButton (parent, "toggle",args,n);
  XtManageChild (widget);
  XmStringFree(tcs);

  return(widget);

}

/***********************************************************/
/* posts the popup menu when button 2 is pressed in the frame widget */
XtCallbackProc PostIt (w, popup, event)
Widget       w,
             popup;
XButtonEvent *event;

{
  if (event->button != Button2)
    return;
  XmMenuPosition (popup, event);
  XtManageChild (popup);
}


/***********************************************************/
Widget CreateLabel (parent, string, posx, posy)
Widget parent;
char *string;
int posx, posy;
{

  Widget       widget;
  Arg          args[MAX_ARGS];
  XmString tcs;
  XmFontList   fontlist;
  int n=0;
  Dimension width, height;

  tcs = XmStringLtoRCreate (string, XmSTRING_DEFAULT_CHARSET);

  n = 0;
  XtSetArg(args[n], XmNx, posx);  n++;
  XtSetArg(args[n], XmNy, posy);  n++;
  XtSetArg(args[n], XmNresizePolicy,  XmRESIZE_ANY);  n++;
  XtSetArg(args[n], XmNshadowThickness, 2);  n++;
  LabelBoardW = XmCreateBulletinBoard (parent, "Bulletin", args, n);
  XtManageChild (LabelBoardW);

  n = 0;
  XtSetArg(args[n], XmNx, 0);  n++;
  XtSetArg(args[n], XmNy, 0);  n++;
  XtSetArg(args[n], XmNrecomputeSize, True);  n++;
  XtSetArg(args[n], XmNlabelString, tcs); n++;
  widget = XmCreateLabel (LabelBoardW, "Label", args, n);
  XtManageChild (widget);

  XmStringFree (tcs);
  return (widget);
}

/***********************************************************/
Widget CreateText (parent, x, y, width, height)
Widget parent;
int x, y, width, height;
{

  Widget widget;
  Arg          args[MAX_ARGS];
  int n=0;

    XtSetArg(args[n], XmNeditMode,  XmMULTI_LINE_EDIT);  n++;
    XtSetArg(args[n], XmNeditable,  False);  n++;
    XtSetArg(args[n], XmNheight,  height);  n++;
    XtSetArg(args[n], XmNwidth,  width);  n++;
    XtSetArg(args[n], XmNx,  x);  n++;
    XtSetArg(args[n], XmNy,  y);  n++;
    XtSetArg(args[n], XmNresizeWidth,  True);  n++;
    XtSetArg(args[n], XmNresizeHeight,  False);  n++;

    widget = XmCreateText (parent, "s_text", args, n);

    return (widget);
}


/***********************************************************/
void  main(argc, argv)
int     argc;
char    **argv;
{
    register int           n;
    Arg           args[MAX_ARGS];
    int           num_items;
    XmString *addrstr, *GetWidgetHierarchy ();
    XmString      *DoTheFont (), *font_addrstr;

    XtAppContext	app;

    MenuBar = (Widget) NULL;

    signal(SIGHUP,  Quit);
    signal(SIGINT,  Quit);
    signal(SIGQUIT, Quit);

    system ("touch foo");
    system ("rm -r foo");

    XtToolkitInitialize();
	app = XtCreateApplicationContext();
    if ((display = XtOpenDisplay (app, NULL, argv[1], "XMdemo",
				  NULL, 0, &argc, argv)) == NULL)
    {
	fprintf (stderr,"\n%s:  Can't open display\n", argv[0]);
        exit(1);
    }

    strcpy (filename, argv[0]);
    strcat (filename, ".c");
    printf ("filename = %s\n", filename);
    n = 0;
    XtSetArg(args[n], XmNallowShellResize, True);  n++;
    Shell = XtAppCreateShell(argv[0], NULL, applicationShellWidgetClass,
                              display, args, n);

    n = 0;
    XtSetArg (args[n], XmNmarginWidth, 2);			n++;
    XtSetArg (args[n], XmNmarginHeight, 2);			n++;
    MainWindow = XmCreateForm(Shell, "MainWindow", args, n);
    XtManageChild(MainWindow);

    n = 0;
    XtSetArg (args[n], XmNtopAttachment, XmATTACH_FORM);	n++;
    XtSetArg (args[n], XmNtopOffset, 40);			n++;
    XtSetArg (args[n], XmNrightAttachment, XmATTACH_FORM);	n++;
    XtSetArg (args[n], XmNleftAttachment, XmATTACH_FORM);	n++;
    XtSetArg (args[n], XmNbottomAttachment, XmATTACH_FORM);	n++;
    WorkRegion = XmCreateBulletinBoard (MainWindow, "s_text", args, n);
    XtManageChild (WorkRegion);

    CreateMenuBar ();

    CreateTextWin ();

    FontDialog = CreateDialogShell (Shell, "List O' Fonts", 450, 450);

    font_addrstr = DoTheFont (&num_items);

    n = 0;
    XtSetArg(args[n], XmNwidth,  450);  n++;
    XtSetArg(args[n], XmNheight, 450);  n++;
    FontBulletin = XmCreateBulletinBoard (FontDialog, "f_bulletin", args, n);
    FontShell = CreateDialog (FontBulletin, font_addrstr, num_items); 

    XtAddCallback (FontShell, XmNcancelCallback, FontSelectCancel, NULL);
    XtAddCallback (FontShell, XmNokCallback, FontSelectOK, NULL);
    XtAddCallback (FontShell, XmNapplyCallback, FontSelectApply, NULL);
    XtAddCallback (FontShell, XmNhelpCallback, FontTest, NULL);

    FontTestDialog = CreateText (FontBulletin, 30, 325, 250, 150);
    n = 0;
    XtSetArg(args[n], XmNeditable,  True);  n++;
    XtSetValues (FontTestDialog, args, n);

    TextShell = CreateDialogShell (Shell, "Help Window", 200, 200);
    Help_BB = XmCreateBulletinBoard (TextShell, "help", NULL, 0);
    TextDialog = CreateText (Help_BB, 0, 0, 350, 350);

    n = 0;
    XtSetArg (args[n], XmNy, 375);		n++;
    XtSetArg (args[n], XmNwidth, 350);		n++;
    dismiss = XmCreatePushButton (Help_BB, "dismiss", args, n);

    XtRealizeWidget(Shell);

    XtAppMainLoop(app);
}

/***********************************************************/
CreateTextWin ()
{
    int           n;
    Arg           args[MAX_ARGS];
    char          *buffer, *GetSource ();

    n = 0;

    XtSetArg(args[n], XmNshadowThickness, 0);  n++; 
    XtSetArg(args[n], XmNresizePolicy, XmRESIZE_ANY);  n++; 
    SWin = XmCreateBulletinBoard (WorkRegion, "s_win", args, n);
    XtManageChild (SWin);


    n=0;
    XtSetArg(args[n], XmNx,  0);  n++;
    XtSetArg(args[n], XmNy,  60);  n++;
    XtSetArg(args[n], XmNshadowThickness, 0);  n++; 
    XtSetArg(args[n], XmNresizePolicy,  XmRESIZE_ANY);  n++;
    TBoardW = XmCreateBulletinBoard (SWin, "Form", args, n);
    XtManageChild (TBoardW);

    n=0;
    XtSetArg(args[n], XmNeditMode,  XmMULTI_LINE_EDIT);  n++;
    XtSetArg(args[n], XmNeditable,  False);  n++;
    XtSetArg(args[n], XmNx,  0);  n++;
    XtSetArg(args[n], XmNy,  0);  n++;
    XtSetArg(args[n], XmNheight,  HEIGHT - 200);  n++; 
    XtSetArg(args[n], XmNwidth,  WIDTH - 350);  n++; 
    XtSetArg(args[n], XmNresizeWidth,  True);  n++;
    XtSetArg(args[n], XmNresizeHeight,  False);  n++;

    TextWin = XmCreateScrolledText (TBoardW, "s_text", args, n);
    XtManageChild (TextWin);

    buffer = GetSource ("welcome");
    XmTextSetString (TextWin, buffer);

    LabelW = CreateLabel (SWin, "Welcome to Motif", 0, 0);

}
/***********************************************************/
XmString *DoTheFont (count)
int *count;
{

    char **fontlist;
    int i;
    static XmString *addrstr;

    fontlist = XListFonts (display, "*", 100, count);
    addrstr = (XmString *) calloc (*count, sizeof (XmString));
    for (i = 0; i < *count; i++) {
      addrstr[i] = XmStringLtoRCreate (fontlist[i], 
				       XmSTRING_DEFAULT_CHARSET);
    }

    return (addrstr);
}
/***********************************************************/
char *GetSource (fileptr)
char *fileptr;
{

  static char *retbuff;
  int fd, flen;

  if ((fd = open (fileptr, O_RDONLY)) < 0) {
    printf ("Cannot open file %s\n", fileptr);
    return ((char *) NULL);
  }
  flen = GetFileLen(fd);
  retbuff = (char*) calloc (1, flen + 1);
  if (read (fd, retbuff, flen) <= 0) {
    printf ("Error reading file %s\n", fileptr);
    return ((char *) NULL);
  }
  close (fd);
  return (retbuff);
}
/***********************************************************/
GetFileLen (fd)
int fd;
{
  static int retval;

  lseek (fd, 0, L_SET);  
  retval = lseek (fd, 0, L_XTND);
  lseek (fd, 0, L_SET);  

  return (retval);

}
/***********************************************************/
CreateMenuBar ()
{

    int           n;
    Arg           args[MAX_ARGS];
    Dimension	  width;


    /*
    **  the menubar
    */
    n = 0;
    XtSetArg (args[n], XmNrightAttachment, XmATTACH_FORM);	n++;
    XtSetArg (args[n], XmNleftAttachment, XmATTACH_FORM);	n++;
    MenuBar = (Widget) XmCreateMenuBar ((Widget) MainWindow, "MenuBar", 
					args, n); 
    if (MenuBar == NULL) {
      printf ("null menu bar\n\r");
      exit (0);
    }
    XtManageChild(MenuBar);

    /*  create the first level pulldowns */

    n = 0;
    PullDown0 = XmCreatePulldownMenu (MenuBar, "OSF Happenings", args, n);
    PullDown1 = XmCreatePulldownMenu (MenuBar, "OSF Happenings", args, n);
    PullDown2 = XmCreatePulldownMenu (MenuBar, "DEMOS", args, n);
    PullDown3 = XmCreatePulldownMenu (MenuBar, "Unix Commands", args, n);
    PullDown4 = XmCreatePulldownMenu (MenuBar, "X Programs", args, n);
    PullDown5 = XmCreatePulldownMenu (MenuBar, "Font", args, n);
    PullDown6 = XmCreatePulldownMenu (MenuBar, "Help", args, n);
    n = 0;
    XtSetArg(args[n], XmNmenuHelpWidget,  PullDown6);  n++;
    XtSetValues (MenuBar, args, n);

    /*  create cascade buttons for menubar  */

    MenuBtn0 = CreateCascade ("File", 'F', PullDown0, MenuBar);
    MenuBtn1 = CreateCascade ("OSF Happenings", 'O', PullDown1, MenuBar);
    MenuBtn2 = CreateCascade ("Demos", 'D', PullDown2, MenuBar);
    MenuBtn3 = CreateCascade ("Unix Commands", 'U', PullDown3, MenuBar);
    MenuBtn4 = CreateCascade ("X Programs", 'X', PullDown4, MenuBar);
    MenuBtn5 = CreateCascade ("Font", 'F', PullDown5, MenuBar);
    MenuBtn6 = CreateCascade ("Help", 'H', PullDown6, MenuBar);

    Label0A = CreatePushButton (LABEL0,'1',PullDown0);
    XtAddCallback (Label0A, XmNactivateCallback, MenuQuit, NULL);

    /*
    **  Menu1
    */

    Separator1A = CreateSeparator(PullDown1);

    Label1A = CreatePushButton (LABEL1,'1',PullDown1);
    XtAddCallback (Label1A, XmNactivateCallback, func1a, NULL);

    Label1B = CreatePushButton (LABEL2, '2', PullDown1);
    XtAddCallback (Label1B, XmNactivateCallback, func1b, NULL);

    Label1C = CreatePushButton (LABEL3, '3', PullDown1);
    XtAddCallback (Label1C, XmNactivateCallback, func1c, NULL);

    Label1D = CreatePushButton (LABEL4, '4', PullDown1);
    XtAddCallback (Label1D, XmNactivateCallback, func1d, NULL);

    /*
    **  Menu2
    */

    Label2A = CreatePushButton (LABEL5,'1',PullDown2);
    XtAddCallback (Label2A, XmNactivateCallback, func2a, NULL);

    Label2B = CreatePushButton (LABEL6, '2', PullDown2);
    XtAddCallback (Label2B, XmNactivateCallback, func2b, NULL);

    Label2C = CreatePushButton (LABEL7, '3', PullDown2);
    XtAddCallback (Label2C, XmNactivateCallback, func2c, NULL);

    Label2D = CreatePushButton (LABEL8, '4', PullDown2);
    XtAddCallback (Label2D, XmNactivateCallback, func2d, NULL);

    /* menu 3 */

    Label3A = CreatePushButton (LABEL10,'5',PullDown3);
    XtAddCallback (Label3A, XmNactivateCallback, func3a, NULL);

    Label3B = CreatePushButton (LABEL11,'5',PullDown3);
    XtAddCallback (Label3B, XmNactivateCallback, func3b, NULL);

    Label3C = CreatePushButton (LABEL12,'5',PullDown3);
    XtAddCallback (Label3C, XmNactivateCallback, func3c, NULL);

    Label3D = CreatePushButton (LABEL12a,'5',PullDown3);
    XtAddCallback (Label3D, XmNactivateCallback, func3d, NULL);

    /* menu 4 */

    Label4A = CreatePushButton (LABEL13,'1',PullDown4);
    XtAddCallback (Label4A, XmNactivateCallback, func4a, NULL);

    Label4B = CreatePushButton (LABEL14,'2',PullDown4);
    XtAddCallback (Label4B, XmNactivateCallback, func4b, NULL);

    Label4C = CreatePushButton (LABEL15,'3',PullDown4);
    XtAddCallback (Label4C, XmNactivateCallback, func4c, NULL);

    /* menu 4 */

    Label5A = CreatePushButton (LABEL16,'1',PullDown5);
    XtAddCallback (Label5A, XmNactivateCallback, ListFonts, NULL);

    /* menu 5 */

    Label6A = CreatePushButton (LABEL17,'1',PullDown6);
    XtAddCallback (Label6A, XmNactivateCallback, help, NULL);

}

/* support routine to get normal string from XmString */

char *extract_normal_string(cs)
XmString cs;
{

  XmStringContext context;
  XmStringCharSet charset;
  XmStringDirection direction;
  Boolean separator;
  static char *primitive_string;

  XmStringInitContext (&context,cs);
  XmStringGetNextSegment (context,&primitive_string,
			  &charset,&direction,&separator);
  XmStringFreeContext (context);
  return ((char *) primitive_string);
}
