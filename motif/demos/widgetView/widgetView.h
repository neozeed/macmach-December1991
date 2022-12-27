#ifdef REV_INFO
#ifndef lint
static char SCCSID[] = "OSF/Motif: @(#)widgetView.h	1.9 91/01/10";
#endif /* lint */
#endif /* REV_INFO */
/******************************************************************************
*******************************************************************************
*
*  (c) Copyright 1989, 1990, 1991 OPEN SOFTWARE FOUNDATION, INC.
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
 **   File:     @(#)widgetView.h	1.9 - 91/01/10
 **
 **   Project:     Motif - widget examination program
 **
 **   Description: Program which shows resources of widgets
 **
 **   Author:      Pete Levine
 **
 **
 **
 ****************************************************************************
 ****************************************************************************/

/*  Standard C headers  */
#include <stdio.h>
#include <sys/signal.h>

/*  X headers  */
#include <X11/IntrinsicP.h>
/*#include <X11/Shell.h>
#include <X11/AtomMgr.h>
#include <X11/Protocols.h>
*/
/*  Xm headers  */
#include <Xm/Xm.h>
#include <Xm/ArrowB.h>
#include <Xm/ArrowBG.h>
#include <Xm/BulletinB.h>
#include <Xm/CascadeB.h>
#include <Xm/CascadeBG.h>
#include <Xm/Command.h>
#include <Xm/CutPaste.h>
#include <Xm/DialogS.h>
#include <Xm/DrawingA.h>
#include <Xm/DrawnB.h>
#include <Xm/FileSB.h>
#include <Xm/Form.h>
#include <Xm/Frame.h>
#include <Xm/Label.h>
#include <Xm/LabelG.h>
#include <Xm/List.h>
#include <Xm/MainW.h>
#include <Xm/MenuShell.h>
#include <Xm/MessageB.h>
#include <Xm/PanedW.h>
#include <Xm/PushB.h>
#include <Xm/PushBG.h>
#include <Xm/RowColumn.h>
#include <Xm/Scale.h>
#include <Xm/ScrollBar.h>
#include <Xm/ScrolledW.h>
#include <Xm/SelectioB.h>
#include <Xm/SeparatoG.h>
#include <Xm/Separator.h>
#include <Xm/Text.h>
#include <Xm/ToggleB.h>
#include <Xm/ToggleBG.h>


#define WIDTH        850
#define HEIGHT       750
#define MAX_ARGS     100
#define NOT_FOUND    -1
#define FUNC_ERROR   -1
#define FUNC_OK      0

/*  Global Variables  */

#ifdef MAIN

int     screen;

Widget    Dialog;
Widget    Shell;
Widget    MainWindow;
Widget    MenuBar;
Widget    PullDown1;
Widget    PullDown1A;
Widget    PullDown2;
Widget    PullDown3;
Widget    PopUp;
Widget    popup_label;
Widget    MenuBtn1;
Widget    MenuBtn2;
Widget    MenuBtn3;
Widget    Cascade1;
Widget    SourcePullDown;
Widget    SourceCascade;
Widget    Ccode;
Widget    UILcode;
Widget    SourceToggle;

/* contents of menu 1 */
Widget    Label1A;
Widget    Label1B;
Widget    Label1C;
Widget    Label1D;
Widget    Label1E;

/* contents of menu 2 */
Widget    Label2A;
Widget    Label2B;
Widget    Label2C;
Widget    Label2D;
Widget    Label2E;

/* contents of menu 3 */
Widget    Label3A;
Widget    Label3B;
Widget    Label3C;


Widget    HorizScrollBar;
Widget    VertScrollBar;
Widget    WorkRegion;
Widget    TextWin = (Widget) NULL;
Widget    ScrollWin;
Widget    ScrollBar;
Widget    FormWin;
Widget    LabelW;
Widget    TBoardW = (Widget) NULL;

Widget    LabelBoardW;
Widget    SWin;
Widget    ScrollBarText;
Widget    DialogSh;
Widget    FileDialog;
Widget    FileSelection;
Widget    ScrollBarList;
Widget    ResourceList;
Widget    ResourceDialog;
Widget    WidgetShell;
Widget    WidgetDialog;
Widget    FileOK;
Widget    WidgetBoard;
Widget    LabelHierarchy;
Widget    TextLabel;
Widget    WidgetHierarchy;
Widget    ToFileLabel;
Widget    ToPrinterLabel;
Widget    RowColumn = (Widget) NULL;
Widget    ResourceShell = (Widget) NULL;
Widget    PrintFileDialog;
Widget    PrintFileSelection;
Widget    ResourceButton;
Widget    ResourceButton1;
Widget    ToggleFrame;
Widget    ScaleFrame;
Widget    ScaleInputWidget;

Widget    ColorShell;
Widget    ColorList;
Widget    ColorBoard;
Widget    ColorScale;
Widget    ColorLabel;
Widget    ColorFrame;
Widget    ColorConfirm;

Widget    TextShell;
Widget    TextDialog;
Widget    TextBulletin;
Widget    TextOKButton;
Widget    TextCancelButton;
Widget    TextClearButton;
Widget    TextLabelButton;

Widget    ObjectShell;
Widget    ObjectDialog;

Widget    MethodShell;
Widget    MethodDialog;

Widget    ErrorDialog;

Widget    QuestionDialog;

static XColor color; 
#else

extern int     screen;

extern Widget    Dialog;
extern Widget    Shell;
extern Widget    MainWindow;
extern Widget    MenuBar;
extern Widget    PullDown1;
extern Widget    PullDown1A;
extern Widget    PullDown2;
extern Widget    PullDown3;
extern Widget    PopUp;
extern Widget    popup_label;
extern Widget    MenuBtn1;
extern Widget    MenuBtn2;
extern Widget    MenuBtn3;
extern Widget    Cascade1;
extern Widget    SourcePullDown;
extern Widget    SourceCascade;
extern Widget    Ccode;
extern Widget    UILcode;
extern Widget    SourceToggle;

/* contents of menu 1 */
extern Widget    Label1A;
extern Widget    Label1B;
extern Widget    Label1C;
extern Widget    Label1D;
extern Widget    Label1E;

/* contents of menu 2 */
extern Widget    Label2A;
extern Widget    Label2B;
extern Widget    Label2C;
extern Widget    Label2D;
extern Widget    Label2E;

/* contents of menu 3 */
extern Widget    Label3A;
extern Widget    Label3B;
extern Widget    Label3C;


extern Widget    HorizScrollBar;
extern Widget    VertScrollBar;
extern Widget    WorkRegion;
extern Widget    TextWin;
extern Widget    ScrollWin;
extern Widget    ScrollBar;
extern Widget    FormWin;
extern Widget    LabelW;
extern Widget    TBoardW;

extern Widget    LabelBoardW;
extern Widget    SWin;
extern Widget    ScrollBarText;
extern Widget    DialogSh;
extern Widget    FileDialog;
extern Widget    FileSelection;
extern Widget    ScrollBarList;
extern Widget    ResourceList;
extern Widget    ResourceDialog;
extern Widget    WidgetShell;
extern Widget    WidgetDialog;
extern Widget    FileOK;
extern Widget    WidgetBoard;
extern Widget    LabelHierarchy;
extern Widget    TextLabel;
extern Widget    WidgetHierarchy;
extern Widget    ToFileLabel;
extern Widget    ToPrinterLabel;
extern Widget    RowColumn;
extern Widget    ResourceShell;
extern Widget    PrintFileDialog;
extern Widget    PrintFileSelection;
extern Widget    ResourceButton;
extern Widget    ResourceButton1;
extern Widget    ToggleFrame;
extern Widget    ScaleFrame;
extern Widget    ScaleInputWidget;

extern Widget    ColorShell;
extern Widget    ColorList;
extern Widget    ColorBoard;
extern Widget    ColorScale;
extern Widget    ColorLabel;
extern Widget    ColorFrame;
extern Widget    ColorConfirm;

extern Widget    TextShell;
extern Widget    TextDialog;
extern Widget    TextBulletin;
extern Widget    TextOKButton;
extern Widget    TextCancelButton;
extern Widget    TextClearButton;
extern Widget    TextLabelButton;

extern Widget    ObjectShell;
extern Widget    ObjectDialog;

extern Widget    MethodShell;
extern Widget    MethodDialog;

extern Widget    ErrorDialog;

extern Widget    QuestionDialog;

extern XColor color; 
#endif /* MAIN */

#ifdef MAIN
Colormap cmap;
Display *display;
Widget main_selection_box_id;
Widget toplevel;        /* shell returned by XtInitialize */
Widget mre_main;        /* main window of application */
#else
extern Colormap cmap;
extern Display *display;
extern Widget main_selection_box_id;
extern Widget toplevel;        /* shell returned by XtInitialize */
extern Widget mre_main;        /* main window of application */
/* extern ApplicationData data;
extern MRMHierarchy    s_MRMHierarchy;
*/
#endif


#ifdef  PMAX
/* these definitions should not be necessary; they should be in headers */
XFontStruct *XLoadQueryFont();

typedef struct {
  unsigned short offset;
  unsigned short length;
  XmStringCharSet charset;
  unsigned short charset_length;
  XmStringDirection direction;
  Boolean error;
} _XmStringContextRec, *_XmStringContext;

typedef struct {
  XmString string;
  _XmStringContext internal_context;
} XmStringContextRec, *XmStringContext;
#endif
