#ifdef REV_INFO
#ifndef lint
static char SCCSID[] = "OSF/Motif: @(#)ScrolledW.c	3.31.1.2 91/03/13";
#endif /* lint */
#endif /* REV_INFO */
/******************************************************************************
*******************************************************************************
*
*  (c) Copyright 1989, 1990, 1991 OPEN SOFTWARE FOUNDATION, INC.
*  (c) Copyright 1987, 1988, 1989, 1990, HEWLETT-PACKARD COMPANY
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
#include <X11/Intrinsic.h>
#include <X11/IntrinsicP.h>
#include <X11/Xutil.h>
#include <Xm/Xm.h>
#include <Xm/XmP.h>
#include <Xm/ScrollBar.h>
#include <Xm/ScrollBarP.h>
#include <Xm/DrawingA.h>
#include <Xm/DrawingAP.h>
#include <Xm/ScrolledW.h>
#include <Xm/ScrolledWP.h>
#include <Xm/Traversal.h>

static void Initialize();
static void Realize();
static void Resize();
static void InsertChild();
static Boolean SetValues();
static XtGeometryResult GeometryManager();
static XtGeometryResult QueryProc();
static void Redisplay ();
static void ChangeManaged();
static void ClassPartInitialize();


#define SB_BORDER_WIDTH 0

static Arg vSBArgs[36];
static Arg hSBArgs[36];
static Arg cwArgs[20];
static Boolean InitCW = TRUE;

#define SWMessage1 "Invalid ScrollBar Display policy."
#define SWMessage2 "Invalid Scrolling Policy."
#define SWMessage3 "Invalid Visual Policy."
#define SWMessage4 "Invalid placement policy."
#define SWMessage6 "Cannot change scrolling policy after initialization."
#define SWMessage7 "Cannot change visual policy after initialization."
#define SWMessage8 "Cannot set AS_NEEDED scrollbar policy with a\nvisual policy of VARIABLE."
#define SWMessage9 "Cannot change scrollbar widget in AUTOMATIC mode."
#define SWMessage10 "Cannot change clip window"
#define SWMessage11 "Cannot set visual policy of CONSTANT in APPLICATION_DEFINED mode."


/************************************************************************
 *									*
 * Scrolled Window Resources						*
 *									*
 ************************************************************************/

static XtResource resources[] = 
{
	  
    {
	XmNhorizontalScrollBar, XmCHorizontalScrollBar, XmRWindow, sizeof(Widget),
        XtOffset (XmScrolledWindowWidget, swindow.hScrollBar),
	XmRImmediate, NULL
    },
    {
	XmNverticalScrollBar, XmCVerticalScrollBar, XmRWindow, sizeof(Widget),
        XtOffset (XmScrolledWindowWidget, swindow.vScrollBar),
	XmRImmediate, NULL
    },
    {
	XmNworkWindow, XmCWorkWindow, XmRWindow, sizeof(Widget),
        XtOffset (XmScrolledWindowWidget, swindow.WorkWindow),
	XmRImmediate, NULL
    },
    {
	XmNclipWindow, XmCClipWindow, XmRWindow, sizeof(Widget),
        XtOffset (XmScrolledWindowWidget, swindow.ClipWindow),
	XmRImmediate, NULL
    },
    {
        XmNscrollingPolicy, XmCScrollingPolicy, XmRScrollingPolicy, 
	sizeof(unsigned char),
        XtOffset (XmScrolledWindowWidget, swindow.ScrollPolicy),
        XmRImmediate, (caddr_t)  XmAPPLICATION_DEFINED
    },   
    {
        XmNvisualPolicy, XmCVisualPolicy, XmRVisualPolicy, sizeof (unsigned char),
        XtOffset (XmScrolledWindowWidget, swindow.VisualPolicy),
        XmRImmediate,  (caddr_t) XmVARIABLE
    },   
    {
        XmNscrollBarDisplayPolicy, XmCScrollBarDisplayPolicy, 
	XmRScrollBarDisplayPolicy, sizeof (char),
        XtOffset (XmScrolledWindowWidget, swindow.ScrollBarPolicy),
        XmRImmediate,  (caddr_t) (255)
    },   
    {
        XmNscrollBarPlacement, XmCScrollBarPlacement, XmRScrollBarPlacement, 
	sizeof (unsigned char),
        XtOffset (XmScrolledWindowWidget, swindow.Placement),
        XmRImmediate,  (caddr_t) XmBOTTOM_RIGHT
    },

    {
        XmNscrolledWindowMarginWidth, XmCScrolledWindowMarginWidth,
        XmRHorizontalDimension, sizeof (Dimension), 
        XtOffset (XmScrolledWindowWidget, swindow.WidthPad), 
	XmRImmediate, (caddr_t) 0
    },
    {   
        XmNscrolledWindowMarginHeight, XmCScrolledWindowMarginHeight, 
        XmRVerticalDimension, sizeof (Dimension), 
        XtOffset (XmScrolledWindowWidget, swindow.HeightPad), 
	XmRImmediate, (caddr_t) 0
    },

    {
        XmNspacing, XmCSpacing, XmRHorizontalDimension, sizeof (Dimension),
        XtOffset (XmScrolledWindowWidget, swindow.pad),
        XmRImmediate,  (caddr_t) XmINVALID_DIMENSION
    },

    {
        XmNshadowThickness, XmCShadowThickness, XmRHorizontalDimension, 
        sizeof (Dimension),
        XtOffset (XmScrolledWindowWidget, manager.shadow_thickness),
        XmRImmediate,  (caddr_t) XmINVALID_DIMENSION
    },
};
/****************
 *
 * Resolution independent resources
 *
 ****************/

static XmSyntheticResource get_resources[] =
{
   { XmNscrolledWindowMarginWidth, 
     sizeof (Dimension),
     XtOffset (XmScrolledWindowWidget, swindow.WidthPad), 
     _XmFromHorizontalPixels,
     _XmToHorizontalPixels },

   { XmNscrolledWindowMarginHeight, 
     sizeof (short),
     XtOffset (XmScrolledWindowWidget, swindow.HeightPad),
     _XmFromVerticalPixels,
     _XmToVerticalPixels },

   { XmNspacing, 
     sizeof (Dimension),
     XtOffset (XmScrolledWindowWidget, swindow.pad), 
     _XmFromHorizontalPixels },

};

/**************
 *
 *  Translation tables for Scrolled Window.
 *
 **************/
static void LeftEdge();
static void RightEdge();
static void TopEdge();
static void BottomEdge();
static void PageLeft();
static void PageRight();
static void PageUp();
static void PageDown();
static void LeftEdgeWork();
static void RightEdgeWork();
static void TopEdgeWork();
static void BottomEdgeWork();
static void LeftEdgeGrabbed();
static void RightEdgeGrabbed();
static void TopEdgeGrabbed();
static void BottomEdgeGrabbed();
static void PageLeftGrabbed();
static void PageRightGrabbed();
static void PageUpGrabbed();
static void PageDownGrabbed();
static void CWMapNotify();

static char ScrolledWindowXlations[] =
 "Ctrl <Key>osfBeginLine:	SWTopLine()\n\
  <Key>osfBeginLine:		SWBeginLine()\n\
  Ctrl <Key>osfEndLine:		SWBottomLine()\n\
  <Key>osfEndLine:		SWEndLine()\n\
  Ctrl <Key>osfPageUp:		SWLeftPage()\n\
  Ctrl <Key>osfPageDown:	SWRightPage()\n\
  <Key>osfPageUp:		SWUpPage()\n\
  <Key>osfPageDown:		SWDownPage()\n\
  <Key>osfHelp:			ManagerGadgetHelp()\n\
  <Key>osfUp: 	       		ManagerGadgetTraverseUp()\n\
  <Key>osfDown:			ManagerGadgetTraverseDown()\n\
  <Key>osfLeft:			ManagerGadgetTraverseLeft()\n\
  <Key>osfRight: 		ManagerGadgetTraverseRight()\n\
  Shift ~Meta ~Alt<Key>Tab:  	ManagerGadgetPrevTabGroup()\n\
  ~Meta ~Alt <Key>Tab:       	ManagerGadgetNextTabGroup()\n\
  <EnterWindow>:  		ManagerEnter()\n\
  <FocusOut>:     		ManagerFocusOut()\n\
  <FocusIn>:      		ManagerFocusIn()";

static XtTranslations ClipWindowXlations = NULL;
static XtTranslations WorkWindowXlations = NULL;
/****************
 *
 * These are grab actions used for the keyboard movement of
 * the window. The table consists of three entries: a modifier
 * mask, a virtual key, and an action name.
 * These are converted to  real keysyms and used for the grab actions.
 *
 ****************/
 
 static _XmBuildVirtualKeyStruct ClipWindowKeys[] = {
     {ControlMask, "osfBeginLine", "SWTopLineGrab()\n"},
     {NULL,        "osfBeginLine", "SWBeginLineGrab()\n"},
     {ControlMask, "osfEndLine",   "SWBottomLineGrab()\n"},
     {NULL,        "osfEndLine",   "SWEndLineGrab()\n"},

     {ControlMask, "osfPageUp",    "SWLeftPageGrab()\n"},
     {NULL,        "osfPageUp",    "SWUpPageGrab()\n"},
     {ControlMask, "osfPageDown",  "SWRightPageGrab()\n"},
     {NULL,        "osfPageDown",  "SWDownPageGrab()\n"},
     {NULL,        "osfHelp",       "Help()"}
 };

static char ClipWindowTranslationTable[] = 
" Ctrl <Key>osfBeginLine:	SWTopLineGrab()\n\
  <Key>osfBeginLine:		SWBeginLineGrab()\n\
  Ctrl <Key>osfEndLine:		SWBottomLineGrab()\n\
  <Key>osfEndLine:		SWEndLineGrab()\n\
  Ctrl <Key>osfPageUp:		SWLeftPageGrab()\n\
  Ctrl <Key>osfPageDown:	SWRightPageGrab()\n\
  <Key>osfPageUp:		SWUpPageGrab()\n\
  <Key>osfPageDown:		SWDownPageGrab()";

static char WorkWindowTranslationTable[] = 
" Ctrl ~Shift <Key>osfBeginLine:	SWTopLineWork()\n\
  ~Ctrl ~Shift <Key>osfBeginLine:	SWBeginLineWork()\n\
  Ctrl ~Shift <Key>osfEndLine:		SWBottomLineWork()\n\
  ~Ctrl ~Shift <Key>osfEndLine:		SWEndLineWork()";

static XtActionsRec ScrolledWActions[] =
{
 {"SWBeginLine",        (XtActionProc) LeftEdge},
 {"SWEndLine",          (XtActionProc) RightEdge},
 {"SWTopLine",          (XtActionProc) TopEdge},
 {"SWBottomLine",       (XtActionProc) BottomEdge},
 {"SWLeftPage",         (XtActionProc) PageLeft},
 {"SWRightPage",        (XtActionProc) PageRight},
 {"SWUpPage",           (XtActionProc) PageUp},
 {"SWDownPage",         (XtActionProc) PageDown}, 
 {"SWBeginLineWork",    (XtActionProc) LeftEdgeWork},
 {"SWEndLineWork",      (XtActionProc) RightEdgeWork},
 {"SWTopLineWork",      (XtActionProc) TopEdgeWork},
 {"SWBottomLineWork",   (XtActionProc) BottomEdgeWork},
 {"SWBeginLineGrab",    (XtActionProc) LeftEdgeGrabbed},
 {"SWEndLineGrab",      (XtActionProc) RightEdgeGrabbed},
 {"SWTopLineGrab",      (XtActionProc) TopEdgeGrabbed},
 {"SWBottomLineGrab",   (XtActionProc) BottomEdgeGrabbed},
 {"SWLeftPageGrab",     (XtActionProc) PageLeftGrabbed},
 {"SWRightPageGrab",    (XtActionProc) PageRightGrabbed},
 {"SWUpPageGrab",       (XtActionProc) PageUpGrabbed},
 {"SWDownPageGrab",     (XtActionProc) PageDownGrabbed}, 
 {"SWCWMapNotify",      (XtActionProc) CWMapNotify}, 
 {"Help",		(XtActionProc) _XmManagerHelp}  
};


/****************************************************************
 *
 * Full class record constant
 *
 ****************************************************************/

externaldef(xmscrolledwindowclassrec) XmScrolledWindowClassRec 
        xmScrolledWindowClassRec = {
  {
/* core_class fields      */
    /* superclass         */    (WidgetClass) &xmManagerClassRec,
    /* class_name         */    "XmScrolledWindow",
    /* widget_size        */    sizeof(XmScrolledWindowRec),
    /* class_initialize   */    NULL,
    /* class_partinit     */    ClassPartInitialize,
    /* class_inited       */	FALSE,
    /* initialize         */    Initialize,
    /* Init hook	  */    NULL,
    /* realize            */    Realize,
    /* actions		  */	ScrolledWActions,
    /* num_actions	  */	XtNumber(ScrolledWActions),
    /* resources          */    resources,
    /* num_resources      */    XtNumber(resources),
    /* xrm_class          */    NULLQUARK,
    /* compress_motion	  */	TRUE,
    /* compress_exposure  */	TRUE,
    /* compress_enterleave*/	TRUE,
    /* visible_interest   */    FALSE,
    /* destroy            */    NULL,
    /* resize             */    Resize,
    /* expose             */    (XtExposeProc )Redisplay,
    /* set_values         */    (XtSetValuesFunc )SetValues,
    /* set values hook    */    NULL,
    /* set values almost  */    XtInheritSetValuesAlmost,
    /* get values hook    */    NULL,
    /* accept_focus       */    NULL,
    /* Version            */    XtVersion,
    /* PRIVATE cb list    */    NULL,
    /* tm_table		  */    XtInheritTranslations,
    /* query_geometry     */    QueryProc,
    /* display_accelerator*/    NULL,
    /* extension          */    NULL,
  },
  {
/* composite_class fields */
    /* geometry_manager   */    (XtGeometryHandler )GeometryManager,
    /* change_managed     */    (XtWidgetProc) ChangeManaged,
    /* insert_child	  */	(XtArgsProc) InsertChild,	
    /* delete_child	  */	XtInheritDeleteChild,	/* Inherit from superclass */
    /* Extension          */    NULL,
  },{
/* Constraint class Init */
    NULL,
    0,
    NULL,
    NULL,
    NULL,
    NULL,
    NULL
      
  },
/* Manager Class */
   {		
      ScrolledWindowXlations,	                /* translations        */    
      get_resources,				/* get resources      	  */
      XtNumber(get_resources),			/* num get_resources 	  */
      NULL,					/* get_cont_resources     */
      0,					/* num_get_cont_resources */
      XmInheritParentProcess,                   /* parent_process         */
      NULL,					/* extension           */    
   },

 {
/* Scrolled Window class - none */     
     /* mumble */               0
 }	
};

externaldef(xmscrolledwindowwidgetclass) WidgetClass 
    xmScrolledWindowWidgetClass = (WidgetClass)&xmScrolledWindowClassRec;



/************************************************************************
 *									*
 *   Callback Functions							*
 *   These are the callback routines for the scrollbar actions.		*
 *									*
 ************************************************************************/

static XtCallbackProc VertSliderMove();
static XtCallbackProc HorizSliderMove();

static XtCallbackRec VSCallBack[] =
{
   {(XtCallbackProc )VertSliderMove, (caddr_t) NULL},
   {NULL,           (caddr_t) NULL},
};

static XtCallbackRec HSCallBack[] =
{
   {(XtCallbackProc )HorizSliderMove, (caddr_t) NULL},
   {NULL,           (caddr_t) NULL},
};

/************************************************************************
 *									*
 *  VertSliderMove							*
 *  Callback for the sliderMoved resource of the vertical scrollbar.	*
 *									*
 ************************************************************************/
/* ARGSUSED */
static  XtCallbackProc  VertSliderMove(w,closure,call_data)
    Widget w;
    caddr_t  closure; 
    XmScrollBarCallbackStruct *call_data;
{
    XmScrolledWindowWidget sw;

    sw = (XmScrolledWindowWidget )w->core.parent;
    if ((sw->swindow.ScrollPolicy == XmAUTOMATIC) &&
        (!sw->swindow.WorkWindow->core.being_destroyed))
    {
    _XmMoveObject((RectObj)sw->swindow.WorkWindow,
		 (Position ) (sw->swindow.WorkWindow->core.x),
		 (Position ) -((int ) call_data->value));
    sw->swindow.vOrigin = (int ) call_data->value;
    }
}

/************************************************************************
 *									*
 *  HorizSliderMove							*
 *  Callback for the sliderMoved resource of the horizontal scrollbar.	*
 *									*
 ************************************************************************/
/* ARGSUSED */
static XtCallbackProc HorizSliderMove(w,closure,call_data)
    Widget w;
    caddr_t  closure;
    XmScrollBarCallbackStruct *call_data;
{
    XmScrolledWindowWidget sw;

    sw = (XmScrolledWindowWidget )w->core.parent;

    if ((sw->swindow.ScrollPolicy == XmAUTOMATIC) &&
        (!sw->swindow.WorkWindow->core.being_destroyed))
    {
    _XmMoveObject((RectObj)sw->swindow.WorkWindow,
		 (Position ) -((int) call_data->value),
 		 (Position ) (sw->swindow.WorkWindow->core.y));
    sw->swindow.hOrigin = (int ) call_data->value;
    }

}

/************************************************************************
 *									*
 *  KidKilled								*
 *  Destroy callback for the BB child widget.				*
 *									*
 ************************************************************************/
/* ARGSUSED */
 static XtCallbackProc KidKilled(w,closure,call_data)
    Widget w;
    caddr_t  closure, call_data;
{
    XmScrolledWindowWidget sw;
    
    sw = (XmScrolledWindowWidget )(w->core.parent)->core.parent;
    if (sw->swindow.WorkWindow == w)
      sw->swindow.WorkWindow = NULL;
    Resize(sw);
}

/************************************************************************
 *                                                                      *
 * LeftEdge - move the view to the left edge                            *
 *                                                                      *
 ************************************************************************/
/* ARGSUSED */
static void LeftEdge(sw, event, params, num_params)
    XmScrolledWindowWidget sw;
    XKeyPressedEvent *event;
    char   *params;
    int    num_params;
{
    XmScrollBarWidget sb = (XmScrollBarWidget) sw->swindow.hScrollBar;
    int value;
    if (!sw->swindow.WorkWindow) return;

    if ((sw->swindow.ScrollPolicy == XmAUTOMATIC) &&
        (!sw->swindow.WorkWindow->core.being_destroyed))
    {
        value = sw->swindow.hmin;            
	XtSetArg (hSBArgs[0], XmNvalue, (XtArgVal) value);
	XtSetValues(sw->swindow.hScrollBar,hSBArgs,1);
        _XmMoveObject((RectObj)sw->swindow.WorkWindow,
		 (Position ) -(value),
 		 (Position ) (sw->swindow.WorkWindow->core.y));
        sw->swindow.hOrigin = value;
        XmProcessTraversal((Widget)sw->swindow.WorkWindow, XmTRAVERSE_CURRENT);
    }
}

/************************************************************************
 *                                                                      *
 * RightEdge - move the view to the Right edge                          *
 *                                                                      *
 ************************************************************************/
/* ARGSUSED */
static void RightEdge(sw, event, params, num_params)
    XmScrolledWindowWidget sw;
    XKeyPressedEvent *event;
    char   *params;
    int    num_params;
{
    XmScrollBarWidget sb = (XmScrollBarWidget) sw->swindow.hScrollBar;
    int value;
    if (!sw->swindow.WorkWindow) return;

    if ((sw->swindow.ScrollPolicy == XmAUTOMATIC) &&
        (!sw->swindow.WorkWindow->core.being_destroyed))
    {
        value = sw->swindow.hmax - sw->swindow.hExtent;
	XtSetArg (hSBArgs[0], XmNvalue, (XtArgVal) value);
	XtSetValues(sw->swindow.hScrollBar,hSBArgs,1);

       _XmMoveObject((RectObj)sw->swindow.WorkWindow,
                     (Position ) -(value), 
		     (Position )(sw->swindow.WorkWindow->core.y));
        sw->swindow.hOrigin = value;
        XmProcessTraversal((Widget)sw->swindow.WorkWindow, XmTRAVERSE_CURRENT);
    }
}
/************************************************************************
 *                                                                      *
 * TopEdge - move the view to the Top edge                              *
 *                                                                      *
 ************************************************************************/
/* ARGSUSED */
static void TopEdge(sw, event, params, num_params)
    XmScrolledWindowWidget sw;
    XKeyPressedEvent *event;
    char   *params;
    int    num_params;
{
    XmScrollBarWidget sb = (XmScrollBarWidget) sw->swindow.vScrollBar;
    int value;
    if (!sw->swindow.WorkWindow) return;

    if ((sw->swindow.ScrollPolicy == XmAUTOMATIC) &&
        (!sw->swindow.WorkWindow->core.being_destroyed))
    {
       value = sw->swindow.vmin;
 
	XtSetArg (vSBArgs[0], XmNvalue, (XtArgVal) value);
	XtSetValues(sw->swindow.vScrollBar,vSBArgs,1);

       _XmMoveObject((RectObj)sw->swindow.WorkWindow,
                     (Position ) (sw->swindow.WorkWindow->core.x),
		     (Position ) -(value));
        sw->swindow.vOrigin = value;
        XmProcessTraversal((Widget)sw->swindow.WorkWindow, XmTRAVERSE_CURRENT);
    }
}
/************************************************************************
 *                                                                      *
 * BottomEdge - move the view to the Bottom edge                        *
 *                                                                      *
 ************************************************************************/
/* ARGSUSED */
static void BottomEdge(sw, event, params, num_params)
    XmScrolledWindowWidget sw;
    XKeyPressedEvent *event;
    char   *params;
    int    num_params;
{
    XmScrollBarWidget sb = (XmScrollBarWidget) sw->swindow.vScrollBar;
    int value;
    if (!sw->swindow.WorkWindow) return;

    if ((sw->swindow.ScrollPolicy == XmAUTOMATIC) &&
        (!sw->swindow.WorkWindow->core.being_destroyed))
    {
        value = sw->swindow.vmax - sw->swindow.vExtent;

	XtSetArg (vSBArgs[0], XmNvalue, (XtArgVal) value);
	XtSetValues(sw->swindow.vScrollBar,vSBArgs,1);
 
       _XmMoveObject((RectObj)sw->swindow.WorkWindow,
                     (Position ) (sw->swindow.WorkWindow->core.x),
		     (Position ) -(value));
        sw->swindow.vOrigin = value;
        XmProcessTraversal((Widget)sw->swindow.WorkWindow, XmTRAVERSE_CURRENT);
    }
}
/************************************************************************
 *                                                                      *
 * PageLeft - Scroll left a page                                        *
 *                                                                      *
 ************************************************************************/
/* ARGSUSED */
static void PageLeft(sw, event, params, num_params)
    XmScrolledWindowWidget sw;
    XKeyPressedEvent *event;
    char   *params;
    int    num_params;
{
    XmScrollBarWidget sb = (XmScrollBarWidget) sw->swindow.hScrollBar;
    int value;
    if (!sw->swindow.WorkWindow) return;

    if ((sw->swindow.ScrollPolicy == XmAUTOMATIC) &&
        (!sw->swindow.WorkWindow->core.being_destroyed))
    {
        if (sb) 
            value = sw->swindow.hOrigin - sb->scrollBar.page_increment;
        else
            value = sw->swindow.hOrigin - sw->swindow.WorkWindow->core.width;
            
        if (value < sw->swindow.hmin) 
            value = sw->swindow.hmin;
	XtSetArg (hSBArgs[0], XmNvalue, (XtArgVal) value);
	XtSetValues(sw->swindow.hScrollBar,hSBArgs,1);

        _XmMoveObject((RectObj)sw->swindow.WorkWindow,
		 (Position ) -(value),
 		 (Position ) (sw->swindow.WorkWindow->core.y));
        sw->swindow.hOrigin = value;
        XmProcessTraversal((Widget)sw->swindow.WorkWindow, XmTRAVERSE_CURRENT);
    }
}
/************************************************************************
 *                                                                      *
 * PageRight - Scroll Right a page                                      *
 *                                                                      *
 ************************************************************************/
/* ARGSUSED */
static void PageRight(sw, event, params, num_params)
    XmScrolledWindowWidget sw;
    XKeyPressedEvent *event;
    char   *params;
    int    num_params;
{
    XmScrollBarWidget sb = (XmScrollBarWidget) sw->swindow.hScrollBar;
    int value;
    if (!sw->swindow.WorkWindow) return;

    if ((sw->swindow.ScrollPolicy == XmAUTOMATIC) &&
        (!sw->swindow.WorkWindow->core.being_destroyed))
    {
        if (sb) 
            value = sw->swindow.hOrigin + sb->scrollBar.page_increment;
        else
            value = sw->swindow.hOrigin + sw->swindow.WorkWindow->core.width;;
            
        if (value > (sw->swindow.hmax - sw->swindow.hExtent)) 
            value = sw->swindow.hmax - sw->swindow.hExtent;
 
	XtSetArg (hSBArgs[0], XmNvalue, (XtArgVal) value);
	XtSetValues(sw->swindow.hScrollBar,hSBArgs,1);

       _XmMoveObject((RectObj)sw->swindow.WorkWindow,
                     (Position ) -(value), 
		     (Position )(sw->swindow.WorkWindow->core.y));
        sw->swindow.hOrigin = value;
        XmProcessTraversal((Widget)sw->swindow.WorkWindow, XmTRAVERSE_CURRENT);
    }
}
/************************************************************************
 *                                                                      *
 * PageUp - Scroll up a page                                            *
 *                                                                      *
 ************************************************************************/
/* ARGSUSED */
static void PageUp(sw, event, params, num_params)
    XmScrolledWindowWidget sw;
    XKeyPressedEvent *event;
    char   *params;
    int    num_params;
{
    XmScrollBarWidget sb = (XmScrollBarWidget) sw->swindow.vScrollBar;
    int value;
    if (!sw->swindow.WorkWindow) return;

    if ((sw->swindow.ScrollPolicy == XmAUTOMATIC) &&
        (!sw->swindow.WorkWindow->core.being_destroyed))
    {
        if (sb) 
            value = sw->swindow.vOrigin - sb->scrollBar.page_increment;
        else
            value = sw->swindow.vOrigin - sw->swindow.WorkWindow->core.height;
            
        if (value < sw->swindow.vmin) 
            value = sw->swindow.vmin;

	XtSetArg (vSBArgs[0], XmNvalue, (XtArgVal) value);
	XtSetValues(sw->swindow.vScrollBar,vSBArgs,1);
 
       _XmMoveObject((RectObj)sw->swindow.WorkWindow,
                     (Position ) (sw->swindow.WorkWindow->core.x),
		     (Position ) -(value));
        sw->swindow.vOrigin = value;
        XmProcessTraversal((Widget)sw->swindow.WorkWindow, XmTRAVERSE_CURRENT);
    }
}
/************************************************************************
 *                                                                      *
 * PageDown - Scroll Down a page                                        *
 *                                                                      *
 ************************************************************************/
/* ARGSUSED */
static void PageDown(sw, event, params, num_params)
    XmScrolledWindowWidget sw;
    XKeyPressedEvent *event;
    char   *params;
    int    num_params;
{
    XmScrollBarWidget sb = (XmScrollBarWidget) sw->swindow.vScrollBar;
    int value;
    if (!sw->swindow.WorkWindow) return;

    if ((sw->swindow.ScrollPolicy == XmAUTOMATIC) &&
        (!sw->swindow.WorkWindow->core.being_destroyed))
    {
        if (sb) 
            value = sw->swindow.vOrigin + sb->scrollBar.page_increment;
        else
            value = sw->swindow.vOrigin + sw->swindow.WorkWindow->core.height;
            
        if (value > (sw->swindow.vmax - sw->swindow.vExtent)) 
            value = sw->swindow.vmax - sw->swindow.vExtent;

	XtSetArg (vSBArgs[0], XmNvalue, (XtArgVal) value);
	XtSetValues(sw->swindow.vScrollBar,vSBArgs,1);
 
       _XmMoveObject((RectObj)sw->swindow.WorkWindow,
                     (Position ) (sw->swindow.WorkWindow->core.x),
		     (Position ) -(value));
        sw->swindow.vOrigin = value;
        XmProcessTraversal((Widget)sw->swindow.WorkWindow, XmTRAVERSE_CURRENT);
    }

}
/************************************************************************
 *                                                                      *
 * Grab routines - these are shells that set up the right widget and    *
 * call the appropriate "normal" routine.                               *
 *                                                                      *
 ************************************************************************/
static void LeftEdgeGrabbed(sw, event, params, num_params)
    XmScrolledWindowWidget sw;
    XKeyPressedEvent *event;
    char   *params;
    int    num_params;
{
    LeftEdge(XtParent(sw), event, params, num_params);
}
static void RightEdgeGrabbed(sw, event, params, num_params)
    XmScrolledWindowWidget sw;
    XKeyPressedEvent *event;
    char   *params;
    int    num_params;
{
    RightEdge(XtParent(sw), event, params, num_params);
}
static void TopEdgeGrabbed(sw, event, params, num_params)
    XmScrolledWindowWidget sw;
    XKeyPressedEvent *event;
    char   *params;
    int    num_params;
{
     TopEdge(XtParent(sw), event, params, num_params);
}
static void BottomEdgeGrabbed(sw, event, params, num_params)
    XmScrolledWindowWidget sw;
    XKeyPressedEvent *event;
    char   *params;
    int    num_params;
{
     BottomEdge(XtParent(sw), event, params, num_params);
}
static void PageLeftGrabbed(sw, event, params, num_params)
    XmScrolledWindowWidget sw;
    XKeyPressedEvent *event;
    char   *params;
    int    num_params;
{
     PageLeft(XtParent(sw), event, params, num_params);
}
static void PageRightGrabbed(sw, event, params, num_params)
    XmScrolledWindowWidget sw;
    XKeyPressedEvent *event;
    char   *params;
    int    num_params;
{
     PageRight(XtParent(sw), event, params, num_params);
}
static void PageUpGrabbed(sw, event, params, num_params)
    XmScrolledWindowWidget sw;
    XKeyPressedEvent *event;
    char   *params;
    int    num_params;
{
     PageUp(XtParent(sw), event, params, num_params);
}
static void PageDownGrabbed(sw, event, params, num_params)
    XmScrolledWindowWidget sw;
    XKeyPressedEvent *event;
    char   *params;
    int    num_params;
{
     PageDown(XtParent(sw), event, params, num_params);
}
/************************************************************************
 *                                                                      *
 * Work routines - these are shells that set up the right widget and    *
 * call the appropriate "normal" routine.                               *
 *                                                                      *
 ************************************************************************/
static void LeftEdgeWork(sw, event, params, num_params)
    XmScrolledWindowWidget sw;
    XKeyPressedEvent *event;
    char   *params;
    int    num_params;
{
    LeftEdgeGrabbed(XtParent(sw), event, params, num_params);
}
static void RightEdgeWork(sw, event, params, num_params)
    XmScrolledWindowWidget sw;
    XKeyPressedEvent *event;
    char   *params;
    int    num_params;
{
    RightEdgeGrabbed(XtParent(sw), event, params, num_params);
}
static void TopEdgeWork(sw, event, params, num_params)
    XmScrolledWindowWidget sw;
    XKeyPressedEvent *event;
    char   *params;
    int    num_params;
{
     TopEdgeGrabbed(XtParent(sw), event, params, num_params);
}
static void BottomEdgeWork(sw, event, params, num_params)
    XmScrolledWindowWidget sw;
    XKeyPressedEvent *event;
    char   *params;
    int    num_params;
{
     BottomEdgeGrabbed(XtParent(sw), event, params, num_params);
}

/************************************************************************
 *                                                                      *
 * CWMapNotify - When the clip window is mapped, we need to add the     *
 *               virtual keys as the translation table. Ugh.            *
 *                                                                      *
 ************************************************************************/
/* ARGSUSED */
static void CWMapNotify(w, event, params, num_params)
    Widget w;
    XEvent *event;
    char   *params;
    int    num_params;
{
    ClipWindowXlations = XtParseTranslationTable(ClipWindowTranslationTable);
    XtSetArg (cwArgs[0], XmNtranslations, (XtArgVal) ClipWindowXlations);
    XtSetValues(w,cwArgs,1);
}


/************************************************************************
 *                                                                      *
 * _XmGetRealXlations - Build up a "real" translation table out of      *
 * virtual keysyms.                                                     *
 *                                                                      *
 ************************************************************************/
#ifdef _NO_PROTO
char *_XmGetRealXlations(dpy, keys, num_keys)
    Display                  *dpy;
    _XmBuildVirtualKeyStruct *keys;
    int                      num_keys;
#else /*  _NO_PROTO */
char *_XmGetRealXlations(Display *dpy, 
                        _XmBuildVirtualKeyStruct *keys,
                        int      num_keys)
#endif /* _NO_PROTO */ 
{
    char *result, tmp[1000];     /* Use static array for speed - should be dynamic */
    char *keystring;
    register int i;
    Modifiers mods;
    KeySym   keysym;
    
    tmp[0] = '\0';
    for (i = 0; i < num_keys; i++)
    {
        keysym = XStringToKeysym(keys[i].key);
        if (keysym == NoSymbol) 
            break;
            
        _XmVirtualToActualKeysym(dpy, keysym, &keysym, &mods);
        keystring = XKeysymToString(keysym);
        if (!keystring)
            break;
        mods |= keys[i].mod;

        if (mods & ControlMask)
            strcat(tmp, "Ctrl ");

	if (mods & Mod1Mask)
	    strcat(tmp, "Mod1 ");

        if (mods & ShiftMask)
            strcat(tmp, "Shift ");

        strcat(tmp,"<Key>");
        strcat(tmp, keystring);
        strcat(tmp,": ");
        strcat(tmp,keys[i].action);
    }
    if (tmp[0] != '\0')
        result = XtNewString(tmp);
    else 
        result = NULL;
    return(result);
    
}


/************************************************************************
 *									*
 *  ClassPartInitialize - Set up the fast subclassing.			*
 *									*
 ************************************************************************/
static void ClassPartInitialize(wc)
WidgetClass wc;

{
   _XmFastSubclassInit (wc, XmSCROLLED_WINDOW_BIT);
}


/************************************************************************
 *									*
 *  Initialize								*
 *									*
 ************************************************************************/
/* ARGSUSED */
static void Initialize(request, new, args, num_args)
    XmScrolledWindowWidget request, new;
    ArgList args;
    Cardinal num_args;
{
    Widget bw;
    Dimension ht;
    int    i, j, k;
    char *ClipWindowXlatString;
    unsigned int mask = (unsigned)(KeyPressMask | KeyReleaseMask);

/****************
 *
 *  Bounds check the size.
 *
 ****************/
    if (request->core.width==0 || (request->core.height==0))
    {
        if (request->core.width==0)
	    new->core.width = 100;
	if (request->core.height==0)
	    new->core.height = 100;
    }
    new->swindow.GivenWidth = request->core.width;
    new->swindow.GivenHeight = request->core.height;
  
    i = 0; j = 0; k = 0; 
    bw = NULL;

    if (new->swindow.pad == XmINVALID_DIMENSION) 
        new->swindow.pad = 4;


    if ((new->swindow.ScrollPolicy != XmAUTOMATIC) &&
        (new->swindow.ScrollPolicy != XmAPPLICATION_DEFINED))
    {
	_XmWarning(new, SWMessage2);
	new->swindow.ScrollPolicy = XmAUTOMATIC;
    }

    if ((new->swindow.VisualPolicy != XmCONSTANT) &&
	(new->swindow.VisualPolicy != XmVARIABLE))
    {
	_XmWarning(new, SWMessage3);
        if (new->swindow.ScrollPolicy == XmAUTOMATIC)
            new->swindow.VisualPolicy = XmCONSTANT;
        else
	    new->swindow.VisualPolicy = XmVARIABLE;
    }

    if ((new->swindow.ScrollPolicy == XmAPPLICATION_DEFINED) &&
        (new->swindow.VisualPolicy == XmCONSTANT))
    {
	_XmWarning(new, SWMessage11);
        new->swindow.VisualPolicy = XmVARIABLE;        
    }

    if (new->swindow.ScrollBarPolicy == 255)
    {
        if (new->swindow.ScrollPolicy == XmAUTOMATIC)
           new->swindow.ScrollBarPolicy = XmAS_NEEDED;
        else
           new->swindow.ScrollBarPolicy = XmSTATIC;
    }

    if ((new->swindow.ScrollBarPolicy != XmAS_NEEDED) &&
        (new->swindow.ScrollBarPolicy != XmSTATIC))
    {
	_XmWarning(new, SWMessage1);
        if (new->swindow.ScrollPolicy == XmAUTOMATIC)
           new->swindow.ScrollBarPolicy = XmAS_NEEDED;
        else
           new->swindow.ScrollBarPolicy = XmSTATIC;
    }	

    if (new->swindow.ScrollPolicy == XmAUTOMATIC) 
    	new->swindow.VisualPolicy = XmCONSTANT;


    if ((new->swindow.VisualPolicy == XmVARIABLE) &&
	(request->swindow.ScrollBarPolicy == XmAS_NEEDED))
    {
	_XmWarning(new, SWMessage8);
	new->swindow.ScrollBarPolicy = XmSTATIC;
    }


    if ((new->swindow.Placement != XmTOP_LEFT) &&
        (new->swindow.Placement != XmTOP_RIGHT) &&
        (new->swindow.Placement != XmBOTTOM_LEFT) &&
        (new->swindow.Placement != XmBOTTOM_RIGHT))
    {
	_XmWarning(new, SWMessage4);

	new->swindow.Placement = XmBOTTOM_RIGHT;
    }	


    if (request->manager.shadow_thickness == XmINVALID_DIMENSION)
        if (new->swindow.ScrollPolicy == XmAUTOMATIC)
            new->manager.shadow_thickness = 2;
	else
            new->manager.shadow_thickness = 0;
	
    ht = new->manager.shadow_thickness;    

    new->swindow.AreaWidth = new->core.width - (ht * 2);
    new->swindow.AreaHeight = new->core.height - (ht * 2);
    new->swindow.FromResize = FALSE;

/****************
 *
 * We will set the X and Y offsets to the pad values. That lets us use
 * the four variables as a "margin" to the user, but MainWindow can still
 * dink around with them if it wants.
 *
 ****************/
    new->swindow.XOffset = new->swindow.WidthPad;
    new->swindow.YOffset = new->swindow.HeightPad;

    XtAugmentTranslations(new, ((XmManagerClassRec *)XtClass(new))->
    			       manager_class.translations);

/****************
 *
 * If the policy is constant, create a clip window. Note that if the scroll policy
 * is auto, we force the visual policy to constant.
 *
 ****************/
    
    if (new->swindow.VisualPolicy == XmCONSTANT)
    {
        if (InitCW)
        {
            InitCW = FALSE;

            ClipWindowXlatString = _XmGetRealXlations(XtDisplay(new),
                                                      ClipWindowKeys,
                                                      XtNumber(ClipWindowKeys));
            if (ClipWindowXlatString)
            {
                ClipWindowXlatString = XtRealloc(ClipWindowXlatString,
                                            strlen(ClipWindowXlatString) +
                                            strlen("\n<MapNotify>: SWCWMapNotify()") +
                                            1);
                strcat(ClipWindowXlatString, "\n<MapNotify>: SWCWMapNotify()");
                ClipWindowXlations = XtParseTranslationTable(ClipWindowXlatString);

                XtRegisterGrabAction(LeftEdgeGrabbed,  TRUE, mask, 
                                     GrabModeAsync, GrabModeAsync);
                XtRegisterGrabAction(RightEdgeGrabbed, TRUE, mask, 
                                     GrabModeAsync, GrabModeAsync);
                XtRegisterGrabAction(TopEdgeGrabbed,   TRUE, mask, 
                                     GrabModeAsync, GrabModeAsync);
                XtRegisterGrabAction(BottomEdgeGrabbed,TRUE, mask, 
                                     GrabModeAsync, GrabModeAsync);
                XtRegisterGrabAction(PageLeftGrabbed,  TRUE, mask, 
                                     GrabModeAsync, GrabModeAsync);
                XtRegisterGrabAction(PageRightGrabbed, TRUE, mask, 
                                     GrabModeAsync, GrabModeAsync);
                XtRegisterGrabAction(PageUpGrabbed,    TRUE, mask, 
                                     GrabModeAsync, GrabModeAsync);
                XtRegisterGrabAction(PageDownGrabbed,  TRUE, mask, 
                                     GrabModeAsync, GrabModeAsync);
            }
        }

	XtSetArg (cwArgs[k],XmNx,(XtArgVal) ht); k++;
	XtSetArg (cwArgs[k],XmNy,(XtArgVal) ht); k++;
	XtSetArg (cwArgs[k],XmNshadowThickness,(XtArgVal) 0); k++;
	XtSetArg (cwArgs[k],XmNborderWidth,(XtArgVal) 0); k++;
	XtSetArg (cwArgs[k],XmNmarginWidth,(XtArgVal) 0); k++;
	XtSetArg (cwArgs[k],XmNmarginHeight,(XtArgVal) 0); k++;
    /*	XtSetArg (cwArgs[k],XmNwidth,(XtArgVal) ((int ) (new->swindow.AreaWidth))); k++;
	XtSetArg (cwArgs[k],XmNheight,(XtArgVal) ((int ) (new->swindow.AreaHeight))); k++;*/
	XtSetArg (cwArgs[k],XmNresizePolicy, (XtArgVal) XmRESIZE_SWINDOW); k++;
	new->swindow.ClipWindow = (XmDrawingAreaWidget) XtCreateManagedWidget(
				  "ScrolledWindowClipWindow", 
				  xmDrawingAreaWidgetClass,(Widget) new,cwArgs, k);
	if (ClipWindowXlations)
	    XtOverrideTranslations(new->swindow.ClipWindow, ClipWindowXlations);
/*        XtFree(ClipWindowXlations);*/
        WorkWindowXlations =
          XtParseTranslationTable(WorkWindowTranslationTable);
        bw =(Widget ) new->swindow.ClipWindow;
    }

/****************
 *
 * If the application wants to do all the work, we're done!
 *
 ****************/
    if (new->swindow.ScrollPolicy == XmAPPLICATION_DEFINED)
    {
        new->swindow.InInit = FALSE;
	return;	
    }

/****************
 *
 *  Else we're in autopilot mode - create the scroll bars, and init the internal
 *  fields. The location and size don't matter right now - we'll figure them out
 *  for real at changemanaged time.
 *
 ****************/

    new->swindow.vsbX =  new->core.width;
    new->swindow.vsbY = 0;
    new->swindow.hsbX  = 0;
    new->swindow.hsbY = new->core.height;
    new->swindow.hsbWidth = new->swindow.AreaWidth + (ht * 2);
    new->swindow.vsbHeight = new->swindow.AreaHeight+ (ht * 2);

    new->swindow.AreaWidth = new->swindow.vsbX - ((ht * 2) );
    new->swindow.AreaHeight = new->swindow.hsbY - ((ht * 2) );

    XtSetArg (vSBArgs[i], XmNorientation,(XtArgVal) (XmVERTICAL)); i++;

    new -> swindow.vmin = 0;
    XtSetArg (vSBArgs[i], XmNminimum, (XtArgVal) (new->swindow.vmin)); i++;

    new->swindow.vmax =  new->core.height + ht;
    XtSetArg (vSBArgs[i], XmNmaximum, (XtArgVal) (new->swindow.vmax)); i++;

    new -> swindow.vOrigin = 0;
    XtSetArg (vSBArgs[i], XmNvalue, (XtArgVal) new->swindow.vOrigin); i++;

    if ((int )(bw->core.height - bw->core.y) < 10)
	new->swindow.vExtent = new->swindow.vmax - new->swindow.vOrigin;
    else
        new->swindow.vExtent =  (int ) ((bw->core.height - bw->core.y) / 10);

    XtSetArg (vSBArgs[i], XmNsliderSize, (XtArgVal) (new->swindow.vExtent)); i++;
    
    XtSetArg (hSBArgs[j], XmNorientation,(XtArgVal) (XmHORIZONTAL)); j++;

    new -> swindow.hmin = 0;
    XtSetArg (hSBArgs[j], XmNminimum, (XtArgVal) (new->swindow.hmin)); j++;

    new -> swindow.hmax =  new->core.width + ht; 
    XtSetArg (hSBArgs[j], XmNmaximum, (XtArgVal) (new->swindow.hmax)); j++;

    new -> swindow.hOrigin = 0;
    XtSetArg (hSBArgs[j], XmNvalue, (XtArgVal) 	new -> swindow.hOrigin); j++;

    if ((int )(bw->core.width - bw->core.x) < 10)
	    new->swindow.hExtent = new->swindow.hmax - new->swindow.hOrigin;
    else
           new -> swindow.hExtent =  (int ) ((bw->core.width - bw->core.x) / 10);
    XtSetArg (hSBArgs[j], XmNsliderSize, (XtArgVal) (new->swindow.hExtent)); j++;


    XtSetArg(vSBArgs[i], XmNunitType, XmPIXELS); i++;
    XtSetArg(vSBArgs[i], XmNincrementCallback, (XtArgVal) VSCallBack); i++;
    XtSetArg(vSBArgs[i], XmNdecrementCallback, (XtArgVal) VSCallBack); i++;
    XtSetArg(vSBArgs[i], XmNpageIncrementCallback, (XtArgVal) VSCallBack); i++;
    XtSetArg(vSBArgs[i], XmNpageDecrementCallback, (XtArgVal) VSCallBack); i++;
    XtSetArg(vSBArgs[i], XmNtoTopCallback, (XtArgVal) VSCallBack); i++;
    XtSetArg(vSBArgs[i], XmNtoBottomCallback, (XtArgVal) VSCallBack); i++;
    XtSetArg(vSBArgs[i], XmNdragCallback, (XtArgVal) VSCallBack); i++;
    XtSetArg(vSBArgs[i], XmNvalueChangedCallback, (XtArgVal) VSCallBack); i++;
    XtSetArg(vSBArgs[i], XmNtraversalOn, (XtArgVal) TRUE); i++;
    XtSetArg(vSBArgs[i], XmNbackground, 
                         (XtArgVal) new->core.background_pixel); i++;

    XtSetArg(hSBArgs[j], XmNunitType, XmPIXELS); j++;
    XtSetArg(hSBArgs[j], XmNincrementCallback, (XtArgVal) HSCallBack); j++;
    XtSetArg(hSBArgs[j], XmNdecrementCallback, (XtArgVal) HSCallBack); j++;
    XtSetArg(hSBArgs[j], XmNpageIncrementCallback, (XtArgVal) HSCallBack); j++;
    XtSetArg(hSBArgs[j], XmNpageDecrementCallback, (XtArgVal) HSCallBack); j++;
    XtSetArg(hSBArgs[j], XmNtoTopCallback, (XtArgVal) HSCallBack); j++;
    XtSetArg(hSBArgs[j], XmNtoBottomCallback, (XtArgVal) HSCallBack); j++;
    XtSetArg(hSBArgs[j], XmNdragCallback, (XtArgVal) HSCallBack); j++;
    XtSetArg(hSBArgs[j], XmNvalueChangedCallback, (XtArgVal) HSCallBack); j++;
    XtSetArg(hSBArgs[j], XmNtraversalOn, (XtArgVal) TRUE); j++;
    XtSetArg(hSBArgs[j], XmNbackground, 
                         (XtArgVal) new->core.background_pixel); j++;

    
    new->swindow.vScrollBar =
	(XmScrollBarWidget) XtCreateManagedWidget("vScrollBar", xmScrollBarWidgetClass,
					   (Widget) new,vSBArgs, i); 
    new->swindow.hScrollBar =
	(XmScrollBarWidget) XtCreateManagedWidget("hScrollBar", xmScrollBarWidgetClass,
					   (Widget) new,hSBArgs, j);
/****************
 *
 * Hack to make sure that the scrollbars have a highlight
 *
 ****************/
    XtSetArg(vSBArgs[0], XmNhighlightThickness, (XtArgVal) 2);
    if ((new->swindow.vScrollBar)->primitive.highlight_thickness == 0) 
    {
        XtSetArg(vSBArgs[1], XmNwidth, (XtArgVal) 
                    ((new->swindow.vScrollBar)->core.width) + 4);
        XtSetValues(new->swindow.vScrollBar, vSBArgs, 2);        
    }
    if ((new->swindow.hScrollBar)->primitive.highlight_thickness == 0)
    {
        XtSetArg(vSBArgs[1], XmNheight, (XtArgVal) 
                    ((new->swindow.hScrollBar)->core.height) + 4);
        XtSetValues(new->swindow.hScrollBar, vSBArgs, 2);         
    }


    new->swindow.WorkWindow = NULL;
    new->swindow.InInit = FALSE;    
}



/************************************************************************
 *									*
 *  Realize								*
 *									*
 ************************************************************************/
static void Realize(sw, p_valueMask, attributes)
    register XmScrolledWindowWidget sw;
    Mask *p_valueMask;
    XSetWindowAttributes *attributes;
{
    Mask valueMask = *p_valueMask;

/****************
 *
 * We don't know if the bboard grew since creation, so we force it back to our 
 * size if needed.
 *
 ****************/ 
    if (sw->swindow.VisualPolicy == XmCONSTANT)
    {    
        if ((sw->swindow.AreaWidth != sw->swindow.ClipWindow->core.width) ||
            (sw->swindow.AreaHeight != sw->swindow.ClipWindow->core.height))
	    _XmResizeObject((RectObj)sw->swindow.ClipWindow,sw->swindow.AreaWidth,sw->swindow.AreaHeight, 
		           sw->swindow.ClipWindow->core.border_width);
    }

   valueMask |= CWDontPropagate;
   attributes->do_not_propagate_mask =  
         ButtonPressMask | ButtonReleaseMask |
         KeyPressMask | KeyReleaseMask | PointerMotionMask;
      
    XtCreateWindow(sw, InputOutput, (Visual *)CopyFromParent,
	valueMask, attributes);

 } /* Realize */



/************************************************************************
 *									*
 *  Redisplay - General redisplay function called on exposure events.	*
 *									*
 ************************************************************************/
static void Redisplay (sw, event, region)
XmScrolledWindowWidget sw;
XEvent * event;
Region region;

{
    Dimension ht, bw;

    if (event)
       _XmRedisplayGadgets ((CompositeWidget)sw, (XExposeEvent*)event, region);
   
   if (XtIsRealized (sw) && (sw->swindow.ScrollPolicy == XmAUTOMATIC))
    {
	ht = sw->manager.shadow_thickness;
	_XmDrawShadow (XtDisplay (sw), XtWindow (sw), 
                  sw -> manager.bottom_shadow_GC,
                  sw -> manager.top_shadow_GC, 
		  sw->manager.shadow_thickness,
                  sw->swindow.ClipWindow->core.x - ht, 
		  sw->swindow.ClipWindow->core.y - ht, 
		  sw -> swindow.AreaWidth + (ht * 2),
		  sw -> swindow.AreaHeight + (ht * 2));
    }
    else
       if (XtIsRealized (sw))
       {
  	   ht = sw->manager.shadow_thickness;
           if (sw->swindow.WorkWindow)
           {
	       bw = sw->swindow.WorkWindow->core.border_width;
	       _XmDrawShadow (XtDisplay (sw), XtWindow (sw), 
                   sw -> manager.bottom_shadow_GC,
                   sw -> manager.top_shadow_GC, 
		   sw->manager.shadow_thickness,
                   sw->swindow.WorkWindow->core.x - ht,
                   sw->swindow.WorkWindow->core.y - ht,
		   sw -> swindow.AreaWidth + ((bw + ht) * 2),
		   sw -> swindow.AreaHeight + ((bw + ht) * 2));
            }
            else
	       _XmDrawShadow (XtDisplay (sw), XtWindow (sw), 
                   sw -> manager.bottom_shadow_GC,
                   sw -> manager.top_shadow_GC, 
		   sw->manager.shadow_thickness,
                   0,
                   0,
		   sw -> swindow.AreaWidth + (ht * 2),
		   sw -> swindow.AreaHeight + (ht * 2));
       }
}

/************************************************************************
 *									*
 *  ClearBorder - Clear the right and bottom border area and save the	*
 *	frame's width and height.					*
 *									*
 ************************************************************************/
static void ClearBorder (sw)
XmScrolledWindowWidget sw;

{
    Dimension ht,oldwidth,oldheight;
    Position x,y;
    Boolean force = FALSE;
    
   if (XtIsRealized (sw) && (sw->swindow.ScrollPolicy == XmAUTOMATIC))
   {

      ht = sw->manager.shadow_thickness;
      oldwidth = sw -> swindow.AreaWidth + ht;
      oldheight = sw ->swindow.AreaHeight + ht;
      x = sw->swindow.ClipWindow->core.x;
      y = sw->swindow.ClipWindow->core.y;
/****************
 *
 * Special case for if the clip window is exactly the size of the
 * scrolled window - gotta clear all the time.
 *
 ****************/
      if ((oldwidth == (sw->core.width - ht)) &&
          (oldheight == (sw->core.height - ht)))
      {
          force = TRUE;
          x = ht; y = ht;
      }

      if ((sw->swindow.AreaWidth != ((sw->swindow.ClipWindow)->core.width)) ||
          force)
          {
	      XClearArea (XtDisplay (sw), XtWindow (sw),
             		  x - ht, 
			  y - ht, 
			  oldwidth, ht, FALSE);
              XClearArea (XtDisplay (sw), XtWindow (sw),
                          x-ht, 
			  oldheight  + sw->swindow.HeightPad, 
			  oldwidth + ht, ht, FALSE);
	  }

      if ((sw->swindow.AreaHeight != ((sw->swindow.ClipWindow)->core.height)) ||
          force)
          {
              XClearArea (XtDisplay (sw), XtWindow (sw),
                          oldwidth + sw->swindow.WidthPad, 
			  y-ht, 
			  ht, oldheight + ht, FALSE);
              XClearArea (XtDisplay (sw), XtWindow (sw),
                          x - ht, 
			  y - ht, 
			  ht, oldheight, FALSE);
	  }
   }

}



/************************************************************************
 *									*
 *  InsertChild								*
 *									*
 ************************************************************************/
static void InsertChild(w)
    Widget	w;
{
    XmManagerWidgetClass     superclass;
    XmScrolledWindowWidget   mom;
    XmScrollBarWidget        sb;
    XmDrawingAreaWidget      da;
    Boolean		     punt;

   if (!XtIsRectObj(w))
	return;

/****************
 *
 * If we are in init, assume it's an internal widget, and the instance vars
 * will be set accordingly.
 *
 ****************/
    mom = (XmScrolledWindowWidget )w->core.parent;

    superclass = (XmManagerWidgetClass)xmManagerWidgetClass;
    
    if (mom->swindow.InInit)
    {
	(*superclass->composite_class.insert_child)(w);
	return;
    }
/****************
 *
 * If variable, look at the class and take a guess as to what it is.
 *
 ****************/
 
    if (mom->swindow.ScrollPolicy == XmAPPLICATION_DEFINED)
    {
	if (XtClass(w) == xmScrollBarWidgetClass)
	{
	    sb =  (XmScrollBarWidget) w;
	    if (sb->scrollBar.orientation == XmHORIZONTAL)
                mom->swindow.hScrollBar = sb;
            else
                mom->swindow.vScrollBar = sb;
	}
        else
    	    if (XtClass(w) == xmDrawingAreaWidgetClass)
    	    {
	        da =  (XmDrawingAreaWidget) w;
	        if (da->drawing_area.resize_policy == XmRESIZE_SWINDOW)
                    mom->swindow.ClipWindow = da;
	    }
            else
                if (mom->swindow.WorkWindow == NULL)
		  {
                    mom->swindow.WorkWindow = w;
                  }
	(*superclass->composite_class.insert_child)(w);
	return;
    }

/****************
 *
 *  Else, we're in constant mode. If the kid is not a scrollbar,
 *  or a DrawingArea with the resizepolicy set to resize_swindow, 
 *  and we have a clip window, reparent it to the clipwindow and 
 *  call it the work window.
 *
 ****************/
    else
    {
        punt = FALSE;
	if (XtClass(w) == xmDrawingAreaWidgetClass)
	{
	    da =  (XmDrawingAreaWidget) w;
	    punt = (da->drawing_area.resize_policy == XmRESIZE_SWINDOW);
	}
        if ((XtClass(w) == xmScrollBarWidgetClass) ||
	    (mom->swindow.ClipWindow == NULL)      ||
	        punt)
  	    (*superclass->composite_class.insert_child)(w);
        else
	{
	    mom->swindow.WorkWindow = w;
	    w->core.parent = (Widget )mom->swindow.ClipWindow;
	    XtAddCallback(w, XmNdestroyCallback,KidKilled,NULL);
	    (*superclass->composite_class.insert_child)(w);
            if (WorkWindowXlations)
                XtOverrideTranslations(w,WorkWindowXlations);
	}
    }
}


/************************************************************************
 *									*
 * ScrollBarVisible - return TRUE if the widget is there and managed.	*
 *									*
 ************************************************************************/
static Boolean ScrollBarVisible(w)
    Widget w;
{
    return(w && XtIsManaged(w));
}

/************************************************************************
 *									*
 * InitializeScrollBars - initialize the scrollbars for auto mode.	*
 *									*
 ************************************************************************/
#ifdef _NO_PROTO
void InitializeScrollBars(w)
    Widget	w;
#else /* _NO_PROTO */
void InitializeScrollBars (Widget w)
#endif /* _NO_PROTO */
{
    XmScrolledWindowWidget	sw = (XmScrolledWindowWidget) w;
    int i, inc;
    Dimension bw;
    
    if (sw->swindow.VisualPolicy == XmVARIABLE)
	return;	
    
    bw = 0;
    if (sw->swindow.WorkWindow)
        bw = sw->swindow.WorkWindow->core.border_width;
    
    sw->swindow.vmin = 0;    
    sw->swindow.vOrigin = 0;
    sw->swindow.hmin = 0;    
    sw->swindow.hOrigin = 0;
    if (ScrollBarVisible(sw->swindow.WorkWindow))
    {
        sw->swindow.vOrigin = abs(sw->swindow.WorkWindow->core.y);
	sw->swindow.vmax = sw->swindow.WorkWindow->core.height + (2 * bw);
	sw->swindow.vExtent = sw->swindow.AreaHeight;
        if (sw->swindow.vOrigin < sw->swindow.vmin)
            sw->swindow.vOrigin = sw->swindow.vmin;

	if ((sw->swindow.vExtent + sw->swindow.vOrigin) > sw->swindow.vmax)
	    sw->swindow.vExtent = sw->swindow.vmax - sw->swindow.vOrigin;
	if (sw->swindow.vExtent < 0)
        {
	    sw->swindow.vExtent = sw->swindow.vmax;
            sw->swindow.vOrigin = sw->swindow.vmin;
        }

	sw->swindow.hmax = sw->swindow.WorkWindow->core.width + (2 * bw);
        sw->swindow.hOrigin = abs(sw->swindow.WorkWindow->core.x);
	sw->swindow.hExtent = sw->swindow.AreaWidth;
        if (sw->swindow.hOrigin < sw->swindow.hmin)
            sw->swindow.hOrigin = sw->swindow.hmin;

	if ((sw->swindow.hExtent + sw->swindow.hOrigin) > sw->swindow.hmax)
	    sw->swindow.hExtent = sw->swindow.hmax - sw->swindow.hOrigin;
	if (sw->swindow.hExtent < 0)
        {
	    sw->swindow.hExtent = sw->swindow.hmax;
            sw->swindow.hOrigin = sw->swindow.hmin;
        }

    }
    else
    {
	sw->swindow.vExtent = sw->swindow.ClipWindow->core.height;
	sw->swindow.hExtent = sw->swindow.ClipWindow->core.width;
	sw->swindow.vmax = sw->swindow.vExtent;
	sw->swindow.hmax = sw->swindow.hExtent;
    }
    if(sw->swindow.vScrollBar)
    {
	i = 0;
        if (sw->swindow.WorkWindow)
        {
            if ((inc = ((sw->swindow.WorkWindow->core.height) / 10)) < 1)
                inc = 1;
            XtSetArg (vSBArgs[i], XmNincrement, (XtArgVal) inc); i++; 
        }
        XtSetArg (vSBArgs[i], XmNpageIncrement, (XtArgVal) (sw->swindow.AreaHeight)); i++;
	XtSetArg (vSBArgs[i], XmNminimum, (XtArgVal) (sw->swindow.vmin)); i++;
	XtSetArg (vSBArgs[i], XmNmaximum, (XtArgVal) (sw->swindow.vmax)); i++;
	XtSetArg (vSBArgs[i], XmNvalue, (XtArgVal) sw->swindow.vOrigin); i++;
	XtSetArg (vSBArgs[i], XmNsliderSize, (XtArgVal) (sw->swindow.vExtent)); i++;
	XtSetValues(sw->swindow.vScrollBar,vSBArgs,i);
    }
    if(sw->swindow.hScrollBar)
    {
	i = 0;
        if (sw->swindow.WorkWindow)
        {
            if ((inc = ((sw->swindow.WorkWindow->core.width) / 10)) < 1)
                inc = 1;
            XtSetArg (hSBArgs[i], XmNincrement, (XtArgVal) inc); i++; 
        }
        XtSetArg (hSBArgs[i], XmNpageIncrement, (XtArgVal) (sw->swindow.AreaWidth)); i++;
	XtSetArg (hSBArgs[i], XmNminimum, (XtArgVal) (sw->swindow.hmin)); i++;
	XtSetArg (hSBArgs[i], XmNmaximum, (XtArgVal) (sw->swindow.hmax)); i++;
	XtSetArg (hSBArgs[i], XmNvalue, (XtArgVal) sw->swindow.hOrigin); i++;
	XtSetArg (hSBArgs[i], XmNsliderSize, (XtArgVal) (sw->swindow.hExtent)); i++;
	XtSetValues(sw->swindow.hScrollBar,hSBArgs,i);
    }
    
}

/************************************************************************
 *									*
 * VariableLayout - Layout the scrolled window for a visual policy of	*
 * XmVARIABLE.								*
 *									*
 ************************************************************************/
static void VariableLayout(sw)
    XmScrolledWindowWidget	sw;
{
    Position	 newx, newy, vsbX, vsbY, hsbX, hsbY;
    Dimension HSBht, VSBht;
    Dimension MyWidth, MyHeight, bbWidth, bbHeight;
    Dimension ht, bw, pad;
    Boolean HasHSB, HasVSB, HSBTrav, VSBTrav;
    XtWidgetGeometry  desired, preferred;
    long tmp;

    MyWidth = sw->core.width - sw->swindow.XOffset - sw->swindow.WidthPad;
    MyHeight = sw->core.height - sw->swindow.YOffset - sw->swindow.HeightPad;
	    	    
    ht = sw->manager.shadow_thickness;

    if ((sw->swindow.WorkWindow == NULL) || 		/* If no kid, just keep the */
        (!XtIsManaged(sw->swindow.WorkWindow)))		/* scrollbars invisible.    */
    {					
	if (sw->swindow.vScrollBar)
	    _XmMoveObject((RectObj)sw->swindow.vScrollBar,sw->core.width,
			 sw->swindow.vScrollBar->core.y);

	if (sw->swindow.hScrollBar)
	    _XmMoveObject((RectObj)sw->swindow.hScrollBar,sw->swindow.hScrollBar->core.x,
			 sw->core.height);

	sw->swindow.AreaWidth = sw->core.width  - sw->swindow.WidthPad - ((ht * 2) );
	sw->swindow.AreaHeight = sw->core.height  - sw->swindow.HeightPad - ((ht * 2) );

	if (sw->swindow.ClipWindow)
	    _XmConfigureObject((RectObj)sw->swindow.ClipWindow, ht + sw->swindow.XOffset, 
	    		       ht + sw->swindow.YOffset, sw->swindow.AreaWidth, 
			       sw->swindow.AreaHeight, 0);

	return;
    }

    pad = sw->swindow.pad;


/****************
 *
 * Need to have some border width variables for the scrollbars here.
 *
 ****************/
 
    bw = sw->swindow.WorkWindow->core.border_width;

    HSBht = 0;
    VSBht = 0;
    
    /* use long tmp. the result of this is a signed value. */

    if ((tmp = MyWidth - ((ht+bw) * 2)) <= 0)
	tmp = 2;
    bbWidth = tmp;

    if ((tmp = MyHeight - ((ht+bw) * 2)) <= 0)
	tmp = 2;
    bbHeight = tmp;

/****************
 *
 * OK, we just figured out the maximum size for the child (with no
 * scrollbars(it's in bbwidth, bbheight). Now query the 
 * kid geometry - tell him that we are going to resize him to bbwidth,
 * bbheight and give him a chance to muck with the scrollbars or whatever.
 * Then, we re-layout according to the current scrollbar config.
 *
 ****************/
    
    newx = ht + sw->swindow.XOffset;
    newy = ht + sw->swindow.YOffset;
    
    desired.x = newx;
    desired.y = newy;
    desired.border_width = bw;
    desired.height = bbHeight;
    desired.width = bbWidth;
    desired.request_mode = (CWWidth | CWHeight);
    XtQueryGeometry(sw->swindow.WorkWindow, &desired, &preferred);
	    
    ht = sw->manager.shadow_thickness;

    pad = sw->swindow.pad;
    bw = preferred.border_width;

    HasHSB = ScrollBarVisible(sw->swindow.hScrollBar);
    HasVSB = ScrollBarVisible(sw->swindow.vScrollBar);

    HSBht = (HasHSB) 
                 ? sw->swindow.hScrollBar->primitive.highlight_thickness : 0;
		 
    VSBht = (HasVSB) 
         	 ? sw->swindow.vScrollBar->primitive.highlight_thickness : 0;
    
    HSBTrav = (HasHSB) 
                 ? sw->swindow.hScrollBar->primitive.traversal_on : TRUE;
		 
    VSBTrav = (HasVSB) 
         	 ? sw->swindow.vScrollBar->primitive.traversal_on : TRUE;

/****************
 *
 * Here's a cool undocumented feature. If the scrollbar is not
 * traversable,but has a highlight thickness, we assume it's something
 * that wants to draw the highlight around the workwindow, and wants to
 * have the scrollbars line up properly. Something like Scrolled Text,
 * for instance :-)
 *
 ****************/
    if (ScrollBarVisible(sw->swindow.WorkWindow) &&
        XmIsPrimitive(sw->swindow.WorkWindow))
    {
        if (HSBht && !HSBTrav) HSBht = 0;
        if (VSBht && !VSBTrav) VSBht = 0;
    }

    /* use long tmp. The result of this is a signed value. */

    tmp = (HasVSB) ? MyWidth - (sw->swindow.vScrollBar->core.width + 
                                    (2 * VSBht) +
		  	            SB_BORDER_WIDTH + ((HSBht + bw + ht)*2) + pad )
		       : MyWidth - ((HSBht + ht+bw) * 2);
    if (tmp <= 0)
	tmp = 2;
    bbWidth = tmp;

    tmp = (HasHSB) ? MyHeight - (sw->swindow.hScrollBar->core.height +
                                      (2 * HSBht) +
    				      SB_BORDER_WIDTH + ((VSBht + ht + bw)*2) + pad ) 
			: MyHeight - ((VSBht + ht+bw) * 2);
    if (tmp <= 0)
	tmp = 2;
    bbHeight = tmp;

    if (HasHSB) newx += HSBht;
    if (HasVSB) newy += VSBht;

/****************
 *
 * Initialize the placement variables - these are correct for
 * bottom-right placement of the scrollbars.
 *
 ****************/
    hsbX = sw->swindow.XOffset;
    vsbY = sw->swindow.YOffset;
    vsbX = (HasVSB) ? (MyWidth - sw->swindow.vScrollBar->core.width 
    		      + sw->swindow.WidthPad) : (MyWidth);

    hsbY = (HasHSB) ? (sw->core.height - sw->swindow.HeightPad - 
                       sw->swindow.hScrollBar->core.height) : (sw->core.height);
/****************
 *
 * Check out the scrollbar placement policy and hack the locations
 * accordingly.
 *
 ****************/
    switch (sw->swindow.Placement)
    {
	case XmTOP_LEFT:
                newx = (HasVSB) ? (sw->swindow.vScrollBar->core.width +
    		                   sw->swindow.XOffset + pad + ht +
				   HSBht) 
				: (sw->swindow.XOffset + ht + HSBht);

                newy = (HasHSB) ? (sw->swindow.hScrollBar->core.height +
    		                   sw->swindow.YOffset + pad + ht +
				   VSBht) 
				: (sw->swindow.YOffset + ht + VSBht);

		hsbX = newx - HSBht - ht;
		hsbY = sw->swindow.YOffset;
		vsbX = sw->swindow.XOffset;
		vsbY = newy - VSBht - ht;
		break;
	case XmTOP_RIGHT:
                newy = (HasHSB) ? (sw->swindow.hScrollBar->core.height +
    		                   sw->swindow.YOffset + pad + ht +
				   VSBht) 
				: (sw->swindow.YOffset + ht + VSBht);

		vsbY = newy - ht - VSBht;
		hsbY = sw->swindow.YOffset;
		break;
	case XmBOTTOM_LEFT:
                newx = (HasVSB) ? (sw->swindow.vScrollBar->core.width +
    		                   sw->swindow.XOffset + pad + ht +
				   HSBht) 
				: (sw->swindow.XOffset + ht + HSBht);
		hsbX = newx - HSBht - ht;
		vsbX = sw->swindow.XOffset;
		break;
	default:
		break;
    }


    _XmConfigureObject((RectObj)sw->swindow.WorkWindow, newx, newy, bbWidth, bbHeight, bw);

    sw->swindow.hsbWidth = bbWidth  + ((HSBht + ht + bw) * 2);

    sw->swindow.vsbHeight = bbHeight  + ((VSBht + ht + bw) * 2);

    if (sw->swindow.ClipWindow)
	_XmConfigureObject((RectObj)sw->swindow.ClipWindow, newx,
			   newy, bbWidth, bbHeight, 0);

    
    sw->swindow.AreaWidth = bbWidth;
    sw->swindow.AreaHeight = bbHeight;

     if (HasVSB)
	 _XmConfigureObject((RectObj)sw->swindow.vScrollBar, vsbX, vsbY,
		     sw->swindow.vScrollBar->core.width, sw->swindow.vsbHeight,
		     SB_BORDER_WIDTH);

     if (HasHSB)
	 _XmConfigureObject((RectObj)sw->swindow.hScrollBar, hsbX, hsbY,
	 	      sw->swindow.hsbWidth, 
		      sw->swindow.hScrollBar->core.height, SB_BORDER_WIDTH);

}


/************************************************************************
 *									*
 * ConstantLayout - Layout the scrolled window for a visual policy of	*
 * XmCONSTANT.	This routine assumes a clipping window that surrounds	*
 * the workspace. 							*
 *									*
 ************************************************************************/
static void ConstantLayout(sw)
    XmScrolledWindowWidget	sw;
{
    int i, inc;
    Position newx,newy;
    Position hsbX, hsbY, vsbX, vsbY, clipX, clipY;
    Dimension bbWidth,bbHeight;
    Dimension viswidth,visheight;
    Dimension kidwidth,kidheight;
    Dimension MyWidth, MyHeight;
    Dimension  HSBht, VSBht, ht, bw, pad;
    Boolean HasHSB, HasVSB, HSBExists, VSBExists;

    MyWidth = sw->core.width - sw->swindow.XOffset  - sw->swindow.WidthPad;
    MyHeight = sw->core.height - sw->swindow.YOffset  - sw->swindow.HeightPad;

    ht = sw->manager.shadow_thickness;

    HSBExists = ScrollBarVisible(sw->swindow.hScrollBar);
    VSBExists = ScrollBarVisible(sw->swindow.vScrollBar);

/****************
 *
 * If there's no visible kid, keep the scrollbars invisible.
 *
 ****************/
    if (sw->swindow.WorkWindow == NULL || 
        !(XtIsManaged(sw->swindow.WorkWindow)))	/* If no kid, just keep the */
    {						/* scrollbars invisible.    */
        if (VSBExists)
        {
            _XmMoveObject((RectObj)sw->swindow.vScrollBar,sw->core.width,
	                  sw->swindow.vScrollBar->core.y);
	    sw->swindow.vsbX = sw->swindow.vScrollBar->core.x;
        }
        if (HSBExists)
        {
	    _XmMoveObject((RectObj)sw->swindow.hScrollBar,sw->swindow.hScrollBar->core.x,
		     sw->core.height);
	    sw->swindow.hsbY = sw->swindow.hScrollBar->core.y;
        }
	sw->swindow.AreaWidth = sw->core.width  - sw->swindow.WidthPad - ((ht * 2) );
	sw->swindow.AreaHeight = sw->core.height - sw->swindow.HeightPad - ((ht * 2) );

        _XmConfigureObject((RectObj)sw->swindow.ClipWindow, ht + sw->swindow.XOffset, 
			  ht + sw->swindow.YOffset,
		          sw->swindow.AreaWidth, sw->swindow.AreaHeight, 0);

        Redisplay(sw,NULL,NULL);
	return;
    }

    pad = sw->swindow.pad;
    bw = sw->swindow.WorkWindow->core.border_width;
    HSBht = (HSBExists) ? sw->swindow.hScrollBar->primitive.highlight_thickness : 0;
    VSBht = (VSBExists) ? sw->swindow.vScrollBar->primitive.highlight_thickness : 0;

    newx = sw->swindow.WorkWindow->core.x;
    newy = sw->swindow.WorkWindow->core.y;

    if (abs(newx) < sw->swindow.hmin)
	newx = -(sw->swindow.hmin);
    if (abs(newy) < sw->swindow.vmin)
	newy = -(sw->swindow.vmin);

    kidwidth = sw->swindow.WorkWindow->core.width + (bw * 2);
    kidheight = sw->swindow.WorkWindow->core.height + (bw * 2);

    bbHeight = (HSBExists) ? MyHeight - (sw->swindow.hScrollBar->core.height + 
                             SB_BORDER_WIDTH + ((HSBht + ht) * 2 + pad))
                           : MyHeight - ((HSBht + ht) * 2 + pad);
    bbWidth = (VSBExists) ? MyWidth - (sw->swindow.vScrollBar->core.width + 
	                    SB_BORDER_WIDTH + ((VSBht + ht) * 2 + pad))
                          : MyWidth - ((VSBht + ht) * 2 + pad);
/****************
 *
 * Look at my size and set the bb dimensions accordingly. If the kid 
 * fits easily into the space, and we can ditch the scrollbars, set
 * the bb dimension to the size of the window and flag the scrollbars
 * as false. If the kid won't fit in either direction, or if the 
 * scrollbars are constantly in the way, set the bb dimensions to the
 * minimum area and flag both scrollbars as true. Otherwise, look at 
 * the dimensions, and see if either one needs a scrollbar.
 *
 ****************/
    if (((MyHeight - (ht*2)) >= kidheight) &&
	((MyWidth - (ht*2)) >= kidwidth) &&
  	 (sw->swindow.ScrollBarPolicy == XmAS_NEEDED)) 
    {
	bbWidth = MyWidth - (ht * 2);		
	bbHeight = MyHeight - (ht * 2);
	HasVSB = HasHSB = FALSE;
    }
    else
        if ((((MyHeight - (ht*2)) < kidheight) &&
	     ((MyWidth - (ht*2)) < kidwidth))  ||
     	    (sw->swindow.ScrollBarPolicy == XmSTATIC)) 
	{	
            HasVSB = HasHSB = TRUE;
	}
	else
	{
	    HasVSB = HasHSB = TRUE;
	    if (kidheight <= bbHeight)
	    {
		bbWidth = MyWidth - ((HSBht + ht) * 2);
                bbHeight += VSBht;
		HasVSB = FALSE;
	    }
	    if (kidwidth <= bbWidth)
	    {
		bbHeight = MyHeight - ((VSBht + ht) * 2);
                bbWidth += HSBht;
		HasHSB = FALSE;
	    }

	}

    HasVSB = (HasVSB && VSBExists);
    HasHSB = (HasHSB && HSBExists);

/****************
 *
 * Look at the amount visible: the workwindow dimension - position.
 * If the bb dimensions are bigger, that means the workwindow is scrolled
 * off in the bigger direction, and needs to be dragged back into the
 * visible space.
 *
 ****************/
    viswidth = kidwidth - abs(newx);

    visheight = kidheight - abs(newy);

    if (bbWidth > viswidth)
         newx =  sw->swindow.WorkWindow->core.x + (bbWidth - viswidth);
    if (newx > 0) newx = 0;
    if (abs(newx) >= kidwidth) newx = -(sw->swindow.hmin);
    
    if (bbHeight > visheight)
        newy =  sw->swindow.WorkWindow->core.y + (bbHeight - visheight);
    if (newy > 0) newy = 0;
    if (abs(newy) >= kidheight) newy = -(sw->swindow.vmin);


/****************
 *
 * Now set the size and value for each scrollbar.
 *
 ****************/
    
    if (((sw->swindow.hmax - sw->swindow.hmin) < bbWidth) &&
          sw->swindow.ScrollBarPolicy == XmSTATIC)
        sw->swindow.hExtent = sw->swindow.hmax - sw->swindow.hmin;
    else
	sw->swindow.hExtent = bbWidth;

    if (((sw->swindow.vmax - sw->swindow.vmin) < bbHeight) &&
	(sw->swindow.ScrollBarPolicy == XmSTATIC))
	sw->swindow.vExtent = sw->swindow.vmax - sw->swindow.vmin;
    else
        sw->swindow.vExtent = bbHeight;

    sw->swindow.vOrigin = abs(newy);

    if (sw->swindow.vOrigin < sw->swindow.vmin)
        sw->swindow.vOrigin = sw->swindow.vmin;

    if (sw->swindow.vOrigin > (sw->swindow.vmax - sw->swindow.vExtent))
    	sw->swindow.vExtent = sw->swindow.vmax - sw->swindow.vOrigin;

    if (VSBExists)
    {
        i = 0;
        XtSetArg(vSBArgs[i],XmNsliderSize, (XtArgVal) sw->swindow.vExtent); i++;
        XtSetArg(vSBArgs[i],XmNvalue, (XtArgVal) sw->swindow.vOrigin); i++;
        XtSetArg(vSBArgs[i], XmNpageIncrement, (XtArgVal) bbHeight); i++;
        if (sw->swindow.WorkWindow)
        {
            if ((inc = ((sw->swindow.WorkWindow->core.height) / 10)) < 1)
                inc = 1;
            XtSetArg (vSBArgs[i], XmNincrement, (XtArgVal) inc); i++; 
        }
        XtSetValues(sw->swindow.vScrollBar,vSBArgs,i);
    }
    sw->swindow.hOrigin = abs(newx);

    if (sw->swindow.hOrigin < sw->swindow.hmin)
        sw->swindow.hOrigin = sw->swindow.hmin;

    if (sw->swindow.hOrigin > (sw->swindow.hmax - sw->swindow.hExtent))
    	sw->swindow.hExtent = sw->swindow.hmax - sw->swindow.hOrigin;

    if (HSBExists)
    {
        i = 0;
        XtSetArg(hSBArgs[i],XmNsliderSize, (XtArgVal) sw->swindow.hExtent); i++;
        XtSetArg(hSBArgs[i],XmNvalue, (XtArgVal) sw->swindow.hOrigin); i++;
        XtSetArg(hSBArgs[i], XmNpageIncrement, (XtArgVal) bbWidth); i++;
        if (sw->swindow.WorkWindow)
        {
            if ((inc = ((sw->swindow.WorkWindow->core.width) / 10)) < 1)
                inc = 1;
            XtSetArg (hSBArgs[i], XmNincrement, (XtArgVal) inc); i++; 
        }
        XtSetValues(sw->swindow.hScrollBar,hSBArgs,i);
    }

    if (HasHSB)
        if (!HasVSB) 
	    sw->swindow.hsbWidth = MyWidth;
	else
	    sw->swindow.hsbWidth = bbWidth  + ((HSBht + ht) * 2);

    if (HasVSB)
        if (!HasHSB) 
	    sw->swindow.vsbHeight = MyHeight;
	else 
	    sw->swindow.vsbHeight = bbHeight  + ((VSBht + ht) * 2);	
	    
    HSBht = (HasHSB) 
                 ? sw->swindow.hScrollBar->primitive.highlight_thickness : 0;
		 
    VSBht = (HasVSB) 
         	 ? sw->swindow.vScrollBar->primitive.highlight_thickness : 0;

/****************
 *
 * Initialize the location of the scrollbars and clip window. Assume
 * bottom-right placement for the default, cause it's easy. We figure 
 * these out assuming both sb's are present, then adjust later.
 *
 ****************/
    clipX = ht + sw->swindow.XOffset + HSBht;
    clipY = ht + sw->swindow.YOffset + VSBht;
    hsbX = sw->swindow.XOffset;
    vsbY = sw->swindow.YOffset;
    vsbX = (HasVSB) ? (MyWidth - sw->swindow.vScrollBar->core.width 
    		      + sw->swindow.WidthPad) : (MyWidth);

    hsbY = (HasHSB) ? (sw->core.height - sw->swindow.HeightPad - 
                       sw->swindow.hScrollBar->core.height) : (sw->core.height);
/****************
 *
 * Check out the scrollbar placement policy and hack the locations
 * accordingly.
 *
 ****************/
    switch (sw->swindow.Placement)
    {
	case XmTOP_LEFT:
                clipX = (HasVSB) ? (sw->swindow.vScrollBar->core.width +
    		                   sw->swindow.XOffset + pad + ht +
				   HSBht) 
				: (sw->swindow.XOffset + ht + HSBht);

                clipY = (HasHSB) ? (sw->swindow.hScrollBar->core.height +
    		                   sw->swindow.YOffset + pad + ht +
				   VSBht) 
				: (sw->swindow.YOffset + ht + VSBht);

		hsbX = clipX - HSBht - ht;
		hsbY = sw->swindow.YOffset;
		vsbX = sw->swindow.XOffset;
		vsbY = clipY - VSBht - ht;
		break;
	case XmTOP_RIGHT:
                clipY = (HasHSB) ? (sw->swindow.hScrollBar->core.height +
    		                   sw->swindow.YOffset + pad + ht +
				   VSBht) 
				: (sw->swindow.YOffset + ht + VSBht);

		vsbY = clipY - ht - VSBht;
		hsbY = sw->swindow.YOffset;
		break;
	case XmBOTTOM_LEFT:
                clipX = (HasVSB) ? (sw->swindow.vScrollBar->core.width +
    		                   sw->swindow.XOffset + pad + ht +
				   HSBht) 
				: (sw->swindow.XOffset + ht + HSBht);
		hsbX = clipX - HSBht - ht;
		vsbX = sw->swindow.XOffset;
		break;
	default:
		break;
    }

/****************
 *
 * Finally - move the widgets.
 *
 ****************/
    _XmConfigureObject((RectObj)sw->swindow.ClipWindow, clipX,clipY, bbWidth, bbHeight, 0);
    ClearBorder(sw);

    sw->swindow.AreaWidth = bbWidth;
    sw->swindow.AreaHeight = bbHeight;


    if (HasVSB)
        _XmConfigureObject((RectObj)sw->swindow.vScrollBar, vsbX, vsbY,
		     sw->swindow.vScrollBar->core.width, sw->swindow.vsbHeight,
		     SB_BORDER_WIDTH);
    else
        if (VSBExists)
        {
            _XmConfigureObject((RectObj)sw->swindow.vScrollBar, sw->core.width, 
	                   sw->swindow.YOffset, sw->swindow.vScrollBar->core.width, 
			   sw->swindow.vsbHeight, SB_BORDER_WIDTH);
            if (_XmFocusIsHere((Widget)sw->swindow.vScrollBar))
                XmProcessTraversal((Widget)sw, XmTRAVERSE_NEXT_TAB_GROUP);
        }
    if (HasHSB)
        _XmConfigureObject((RectObj)sw->swindow.hScrollBar, hsbX, hsbY,
		      sw->swindow.hsbWidth, 
		      sw->swindow.hScrollBar->core.height, SB_BORDER_WIDTH);
    else
        if (HSBExists)
        {
            _XmConfigureObject((RectObj)sw->swindow.hScrollBar, sw->swindow.XOffset, 
		      sw->core.height,
		      sw->swindow.hsbWidth, 
		      sw->swindow.hScrollBar->core.height, SB_BORDER_WIDTH);
            if (_XmFocusIsHere((Widget)sw->swindow.hScrollBar))
                XmProcessTraversal((Widget)sw, XmTRAVERSE_NEXT_TAB_GROUP);
        }
    
    if (VSBExists) sw->swindow.vsbX = sw->swindow.vScrollBar->core.x;
    if (HSBExists) sw->swindow.hsbY = sw->swindow.hScrollBar->core.y;


    if (newx != (sw->swindow.WorkWindow->core.x + bw)  ||
	newy != (sw->swindow.WorkWindow->core.y + bw))

         _XmMoveObject((RectObj)sw->swindow.WorkWindow,newx, newy );

}

/************************************************************************
 *									*
 *  ReSize								*
 *  Recompute the size of the scrolled window.				* 
 *									*
 ************************************************************************/
static void Resize(sw)
    XmScrolledWindowWidget	sw;
{
    sw->swindow.FromResize = TRUE;
    if (sw->swindow.VisualPolicy == XmVARIABLE)
        VariableLayout(sw);
    else
        ConstantLayout(sw);
	
    Redisplay(sw,NULL,NULL);
    sw->swindow.FromResize = FALSE;
}



/************************************************************************
 *									*
 * SetBoxSize - set the size of the scrolled window to enclose all the	*
 * visible widgets.							*
 *									*
 ************************************************************************/
static void SetBoxSize(sw)
    XmScrolledWindowWidget sw;
{
    Dimension	    newWidth,newHeight;
    XmScrollBarWidget	    hsb, vsb;
    Widget 	    w;
    Dimension	    hsheight,vswidth;
    Dimension       hsbht, vsbht,  ht;

    hsbht = vsbht = 0;
    ht = sw->manager.shadow_thickness * 2;
    hsb = sw->swindow.hScrollBar;
    vsb = sw->swindow.vScrollBar;
    w = sw->swindow.WorkWindow;
    
    if (ScrollBarVisible(vsb)) 
    {
	vsbht = 2 * vsb->primitive.highlight_thickness;
	vswidth = vsb->core.width + sw->swindow.pad + vsbht;
    }
    else
	vswidth = 0;

    if (ScrollBarVisible(hsb)) 
    {
	hsbht = 2 * hsb->primitive.highlight_thickness;
	hsheight = hsb->core.height + sw->swindow.pad + hsbht;
    }
    else
	hsheight = 0;
	
    if (ScrollBarVisible(w)) 
    {
        newWidth = w->core.width + (2 * w->core.border_width) + vswidth + 
	           ht + hsbht + sw->swindow.XOffset + sw->swindow.WidthPad;
        newHeight = w->core.height  + (2 * w->core.border_width) + hsheight + 
	            ht + vsbht + sw->swindow.YOffset + sw->swindow.HeightPad;
    }
    else
    {
	newWidth = sw->core.width;	
        newHeight = sw->core.height;
    }
/****************
 *
 * If we're not realized, and we have a width and height, use it.
 *
 *****************/
     if (!XtIsRealized(sw))
     {
         if (sw->swindow.GivenWidth)
             newWidth = sw->swindow.GivenWidth;
         if (sw->swindow.GivenHeight)
             newHeight = sw->swindow.GivenHeight;
     }    
    if (XtMakeResizeRequest(sw,newWidth,newHeight,NULL,NULL) == XtGeometryYes)
    	    Resize(sw);	    
}

/************************************************************************
 *									*
 *  GeometryManager							*
 *									*
 ************************************************************************/
static XtGeometryResult GeometryManager (w, request, reply)
Widget w;
XtWidgetGeometry * request;
XtWidgetGeometry * reply;

{
    XmScrolledWindowWidget sw;
    XmScrollBarWidget hsb,vsb;
    XtGeometryResult  retval;
    Dimension	    newWidth,newHeight;
    Dimension       hsheight,vswidth;
    Dimension       hsbht, vsbht, bw, ht;
    int             i, hsmax, vsmax;
    Boolean         Fake = FALSE;

    hsbht = vsbht = 0;
    reply->request_mode = 0;

    sw = (XmScrolledWindowWidget ) w->core.parent;
    while (!XmIsScrolledWindow(sw))
        sw = (XmScrolledWindowWidget )sw->core.parent;

    if (w == (Widget)sw->swindow.ClipWindow) 		/* No Growth for internal widgets. */
       return(XtGeometryNo);
/****************
 *
 * Disallow any X or Y changes - we need to manage the child locations.
 *
 ****************/
    if (request -> request_mode & CWX || request -> request_mode & CWY) 

	if (request->request_mode & CWWidth || 
	    request->request_mode & CWHeight)
	{
	    reply->x = w->core.x;
	    reply->y = w->core.y;
	    reply->request_mode = request->request_mode & ~(CWX | CWY);
	    if (request->request_mode & CWWidth)
	        reply->width = request->width;
	    if (request->request_mode & CWHeight)
	        reply->height = request->height;
	    return( XtGeometryAlmost);
	}
	else
	    return( XtGeometryNo);
	    
    ht = sw->manager.shadow_thickness * 2;
    hsb = sw->swindow.hScrollBar;
    vsb = sw->swindow.vScrollBar;
/****************
 *
 * Carry forward that ugly wart for scrolled primitives...
 *
 ****************/
    
    if (ScrollBarVisible(vsb))
    {
	vsbht = 2 * vsb->primitive.highlight_thickness;
        if (ScrollBarVisible(sw->swindow.WorkWindow)        &&
            XmIsPrimitive(sw->swindow.WorkWindow)           &&
	    !(sw->swindow.vScrollBar->primitive.traversal_on))
	    vsbht = 0;
	vswidth = vsb->core.width + sw->swindow.pad + vsbht;
    }
    else
	vswidth = 0;

    if (ScrollBarVisible(hsb))
    {
	hsbht = 2 * hsb->primitive.highlight_thickness;
        if (ScrollBarVisible(sw->swindow.WorkWindow)        &&
            XmIsPrimitive(sw->swindow.WorkWindow)           &&
	    !(sw->swindow.hScrollBar->primitive.traversal_on))
	    hsbht = 0;
	hsheight = hsb->core.height + sw->swindow.pad + hsbht;
    }
    else
	hsheight = 0;
	
/****************
 *
 * First, see if its from a visible scrollbar. If so, look for a height
 * req from the vsb, or a width from the hsb, and say no, or almost if 
 * they asked for both dimensions. Else update our height & 
 * width and relayout. If the requesting scrollbar is invisible,
 * grant its request and take care of the layout when a later operation
 * forces it to become visible.
 *
 ****************/
 
    if (w == (Widget) hsb || w == (Widget) vsb)
    {
	if ((w  == (Widget) vsb) && (request->request_mode & CWHeight))
        {
            if (request->request_mode & CWWidth)
            {
	        reply->request_mode |= (CWHeight);
	        reply->height = w->core.height;
	        return( XtGeometryAlmost);
            }
            else
                return( XtGeometryNo);
        }

	if ((w  == (Widget) hsb) && (request->request_mode & CWWidth))
        {
            if (request->request_mode & CWHeight)
            {
	        reply->request_mode |= (CWWidth);
	        reply->width = w->core.width;
	        return(XtGeometryAlmost);
            }
            else
                return(XtGeometryNo);
        }

        if (((w == (Widget) hsb) && (w->core.y < sw->core.height)) || 
            ((w == (Widget) vsb) && (w->core.x < sw->core.width)))
        {
	    if(request->request_mode & CWWidth)
	        newWidth = sw->core.width - w->core.width + request->width;
            else
                newWidth = sw->core.width ;
	    if(request->request_mode & CWHeight)
	        newHeight = sw->core.height - w->core.height + request->height;
	    else
                newHeight = sw->core.height ;
            if (request->request_mode & CWBorderWidth)
            {
                newHeight = newHeight - (w->core.border_width * 2) 
                                      + (request->border_width * 2);
                newWidth  =  newWidth - (w->core.border_width * 2) 
                                      + (request->border_width * 2);
            }

            retval = XtMakeResizeRequest(sw,newWidth,newHeight,NULL,NULL);
        }
        else
        {
            Fake = TRUE;
            retval = XtGeometryYes;
        }

        if (retval == XtGeometryYes)
	{
	    if (request->request_mode & CWBorderWidth)
	        w->core.border_width = request->border_width;
	    if(request->request_mode & CWWidth)
	        w->core.width = request->width;
	    if(request->request_mode & CWHeight)
	        w->core.height = request->height;

    	    if (!Fake) Resize(sw);	    
	}

	return(retval);
    }

    if (sw->swindow.VisualPolicy == XmVARIABLE)
    {
	if (request->request_mode & CWBorderWidth)
	    bw = (request->border_width * 2);
	else
	    bw = (w->core.border_width * 2);

	if(request->request_mode & CWWidth)
	    newWidth = request->width + vswidth + ht + bw + hsbht +
	    	       sw->swindow.XOffset + sw->swindow.WidthPad;
	else
            newWidth = sw->core.width ;

	if(request->request_mode & CWHeight)
	    newHeight = request->height + hsheight + ht + bw + vsbht +
	    		sw->swindow.YOffset + sw->swindow.HeightPad;
	else
            newHeight = sw->core.height ;

        retval = XtMakeResizeRequest(sw,newWidth,newHeight,NULL,NULL);
	if (retval == XtGeometryYes)
	{
	    if (request->request_mode & CWBorderWidth)
	        w->core.border_width = request->border_width;
	    if(request->request_mode & CWWidth)
	        w->core.width = request->width;
	    if(request->request_mode & CWHeight)
	        w->core.height = request->height;
            if (!XmIsMainWindow(sw))
       	        Resize(sw);	    
	}
	return(retval);
    }
    else				/* Constant - let the workarea grow and */
					/* update the scrollbar resources.	*/
    {
	if (request->request_mode & CWBorderWidth)
	{
	    bw = (request->border_width * 2);
    	    vsmax =  w->core.height + bw;
    	    hsmax =  w->core.width + bw;
	    w->core.border_width = request->border_width;
	}

	bw = (w->core.border_width * 2);

	if (request->request_mode & CWWidth)
	{
 	     w->core.width = request->width;
	     hsmax = request->width + bw;
	}
	else
	     hsmax = w->core.width + bw;

	if (request->request_mode & CWHeight)
	{
	    w->core.height = request->height;
	    vsmax = request->height + bw;
	}
	else
	     vsmax = w->core.height + bw;

	if (request->request_mode & (CWWidth | CWBorderWidth))
	{
	    i = 0;
	    sw->swindow.hmax = hsmax;
	    if (sw->swindow.hExtent > (sw->swindow.hmax - sw->swindow.hmin))
	    	sw->swindow.hExtent = sw->swindow.hmax - sw->swindow.hmin;

	    if (sw->swindow.hOrigin > (sw->swindow.hmax - sw->swindow.hExtent))
	    	sw->swindow.hOrigin = sw->swindow.hmax - sw->swindow.hExtent;

	    XtSetArg (hSBArgs[i],XmNmaximum, (XtArgVal) hsmax); i++;
            XtSetArg (hSBArgs[i],XmNsliderSize, (XtArgVal) sw->swindow.hExtent); i++;
            XtSetArg (hSBArgs[i],XmNvalue, (XtArgVal) sw->swindow.hOrigin); i++;
            if (ScrollBarVisible(sw->swindow.hScrollBar))
	        XtSetValues((Widget ) sw->swindow.hScrollBar,hSBArgs,i);
	}

	if (request->request_mode & (CWHeight | CWBorderWidth))
	{
	    sw->swindow.vmax = vsmax;
	    i = 0;
	    if (sw->swindow.vExtent > (sw->swindow.vmax - sw->swindow.vmin))
	    	sw->swindow.vExtent = sw->swindow.vmax - sw->swindow.vmin;

	    if (sw->swindow.vOrigin > (sw->swindow.vmax - sw->swindow.vExtent))
	    	sw->swindow.vOrigin = sw->swindow.vmax - sw->swindow.vExtent;

	    XtSetArg (vSBArgs[i],XmNmaximum,(XtArgVal) vsmax); i++;
            XtSetArg (vSBArgs[i],XmNsliderSize, (XtArgVal) sw->swindow.vExtent); i++;
            XtSetArg (vSBArgs[i],XmNvalue, (XtArgVal) sw->swindow.vOrigin); i++;
            if (ScrollBarVisible(sw->swindow.vScrollBar))
	        XtSetValues((Widget ) sw->swindow.vScrollBar,vSBArgs,i);
	}
	Resize(sw);		
	return (XtGeometryYes);
    }
}



/************************************************************************
 *									*
 *  ChangeManaged - called whenever there is a change in the managed	*
 *		    set.						*
 *									*
 ************************************************************************/
static void ChangeManaged(sw)
    XmScrolledWindowWidget sw;
{
    if (sw->swindow.FromResize)
        return;

    if (sw->swindow.VisualPolicy == XmVARIABLE)
	SetBoxSize(sw);
    InitializeScrollBars((Widget)sw);
    if (sw->swindow.VisualPolicy == XmVARIABLE)
        VariableLayout(sw);
    else
	ConstantLayout(sw);
	
    Redisplay(sw,NULL,NULL);
    _XmNavigChangeManaged((Widget)sw);
}
     

/************************************************************************
 *									*
 *  QueryProc - Query proc for the scrolled window.			*
 *									*
 *  This routine will examine the geometry passed in an recalculate our	*
 *  width/height as follows:  if either the width or height is set, we	*
 *  take that as our new size, and figure out the other dimension to be	*
 *  the minimum size we need to be to display the entire child.  Note	*
 *  that this will only happen in auto mode, with as_needed display 	*
 *  policy.								*
 *									*
 ************************************************************************/
static XtGeometryResult QueryProc(sw, request, ret)
XmScrolledWindowWidget sw;
XtWidgetGeometry * request;
XtWidgetGeometry * ret;

{
    Dimension	       MyWidth, MyHeight, KidHeight, KidWidth;
    Widget 	       w;
    XmScrollBarWidget  hsb, vsb;
    Dimension          hsheight,vswidth;
    Dimension	       hsbht, vsbht, ht;
    XtWidgetGeometry   desired, preferred;

    XtGeometryResult retval = XtGeometryYes;

    ret -> request_mode = 0;
    w = sw->swindow.WorkWindow;
    hsb = sw->swindow.hScrollBar;
    vsb = sw->swindow.vScrollBar;

/****************
 *
 * If the request mode is zero, fill out out default height & width.
 *
 ****************/
    if (request->request_mode == 0)
    {
        if ((sw->swindow.VisualPolicy == XmCONSTANT) ||
            (!sw->swindow.WorkWindow))
        {
	    ret->width = sw->core.width;	
            ret->height = sw->core.height;
    	    ret->request_mode = (CWWidth | CWHeight);
	    return (XtGeometryAlmost);
        }
        hsbht = vsbht = 0;
        ht = sw->manager.shadow_thickness * 2;

        desired.request_mode = 0;
        XtQueryGeometry(sw->swindow.WorkWindow, &desired, &preferred);

        KidWidth = preferred.width;
        KidHeight = preferred.height;
        if (ScrollBarVisible(vsb)) 
        {
            vsbht = 2 * vsb->primitive.highlight_thickness;
	    vswidth = vsb->core.width + sw->swindow.pad + vsbht;
        }
        else
	    vswidth = 0;

        if (ScrollBarVisible(hsb)) 
        {
	    hsbht = 2 * hsb->primitive.highlight_thickness;
	    hsheight = hsb->core.height + sw->swindow.pad + hsbht;
        }
        else
	    hsheight = 0;
	
        if (ScrollBarVisible(w)) 
        {
            ret->width = KidWidth + (2 * w->core.border_width) + vswidth + 
	           ht + hsbht + sw->swindow.XOffset + sw->swindow.WidthPad;
            ret->height = KidHeight  + (2 * w->core.border_width) + hsheight + 
	            ht + vsbht + sw->swindow.YOffset + sw->swindow.HeightPad;
        }
        else
        {
	    ret->width = sw->core.width;	
            ret->height = sw->core.height;
        }
        ret->request_mode = (CWWidth | CWHeight);
	return (XtGeometryAlmost);
    }

/****************
 *
 * If app mode, or static scrollbars, or no visible kid, 
 * accept the new size, and return our current size for any 
 * missing dimension.
 *
 ****************/
    if ((sw->swindow.ScrollPolicy == XmAPPLICATION_DEFINED) ||
	(!ScrollBarVisible(w)))
    {
        if (!(request -> request_mode & CWWidth))
        {
    	    ret->request_mode |= CWWidth;
            ret->width = sw->core.width;
	    retval = XtGeometryAlmost;
	}
        if (!(request -> request_mode & CWHeight))
        {
            ret->request_mode |= CWHeight;
            ret->height = sw->core.height;
            retval = XtGeometryAlmost;
        }
        return(retval);
    }

/****************
 *
 * Else look for the specified dimension, and set the other size so that we
 * just enclose the child. 
 * If the new size would cause us to lose the scrollbar, figure
 * out the other dimension as well and return that, too.
 *
 ****************/

    hsbht = vsbht = 0;
    ht = sw->manager.shadow_thickness * 2;
    hsb = sw->swindow.hScrollBar;
    vsb = sw->swindow.vScrollBar;

    if ((request -> request_mode & CWWidth) &&
        (request -> request_mode & CWHeight)&&
        (sw->swindow.ScrollBarPolicy == XmAS_NEEDED))
    {
        ret->height = w->core.height + (2 * w->core.border_width) +
	              ht + sw->swindow.YOffset + sw->swindow.HeightPad;
        ret->width = w->core.width + (2 * w->core.border_width) +
                     ht + sw->swindow.XOffset + sw->swindow.WidthPad;
        ret->request_mode |= (CWWidth | CWHeight);
        return(XtGeometryAlmost);
    }

    if (request -> request_mode & CWHeight)
    {

        MyHeight = request->height - sw->swindow.YOffset - 
	           sw->swindow.HeightPad - ht;

        if (((w->core.height + (2 * w->core.border_width)) > MyHeight) ||
            (sw->swindow.ScrollBarPolicy == XmSTATIC))
        {
   	    vsbht = 2 * vsb->primitive.highlight_thickness;
	    vswidth = vsb->core.width + sw->swindow.pad;
        }
        else
        {
	    vswidth = 0;
            ret->request_mode |= CWHeight;
            ret->height = w->core.height + (2 * w->core.border_width) +
	             ht + sw->swindow.YOffset + sw->swindow.HeightPad;
        }

        ret->request_mode |= CWWidth;
        ret->width = w->core.width + (2 * w->core.border_width) + vswidth + 
	             ht + vsbht + sw->swindow.XOffset + sw->swindow.WidthPad;
	retval = XtGeometryAlmost;
    }

    if (request -> request_mode & CWWidth)
    {
        MyWidth = request->width - sw->swindow.XOffset - 
	           sw->swindow.WidthPad - ht;

        if (((w->core.width + (2 * w->core.border_width)) > MyWidth) ||
            (sw->swindow.ScrollBarPolicy == XmSTATIC))
        {
   	    hsbht = 2 * hsb->primitive.highlight_thickness;
	    hsheight = hsb->core.height + sw->swindow.pad;
        }
        else
        {
	    hsheight = 0;
            ret->request_mode |= CWWidth;
            ret->width = w->core.width + (2 * w->core.border_width) +
	                 ht + sw->swindow.XOffset + sw->swindow.WidthPad;
        }

        ret->request_mode |= CWHeight;
        ret->height = w->core.height + (2 * w->core.border_width) + hsheight + 
	             ht + hsbht + sw->swindow.YOffset + sw->swindow.HeightPad;
	retval = XtGeometryAlmost;
    }

    return(retval);
}


/************************************************************************
 *									*
 *  SetValues								*
 *									*
 ************************************************************************/
static Boolean SetValues (current, request, new)
    XmScrolledWindowWidget current, request, new;
{
    Boolean Flag = FALSE;

    if ((new->swindow.WidthPad != current->swindow.WidthPad) ||
        (new->swindow.HeightPad != current->swindow.HeightPad) ||
        (new->swindow.pad != current->swindow.pad)) {
	new->swindow.XOffset = new->swindow.WidthPad;
	new->swindow.YOffset = new->swindow.HeightPad;
        Flag = TRUE;
    }

    if ((new->swindow.ScrollBarPolicy != XmAS_NEEDED) &&
        (new->swindow.ScrollBarPolicy != XmSTATIC))
    {
	_XmWarning(new, SWMessage1);
	new->swindow.ScrollBarPolicy = current->swindow.ScrollBarPolicy;
    }	

    if (new->swindow.ScrollBarPolicy != current->swindow.ScrollBarPolicy)
       Flag = TRUE;

    if (request->swindow.ScrollPolicy != current->swindow.ScrollPolicy)
    {
	_XmWarning(new, SWMessage6);
	new->swindow.ScrollPolicy = current->swindow.ScrollPolicy;
    }
    if (request->swindow.VisualPolicy != current->swindow.VisualPolicy)
    {
	_XmWarning(new, SWMessage7);
	new->swindow.VisualPolicy = current->swindow.VisualPolicy;
    }
    if ((new->swindow.VisualPolicy == XmVARIABLE) &&
	(request->swindow.ScrollBarPolicy == XmAS_NEEDED))
    {
	_XmWarning(new, SWMessage8);
	new->swindow.ScrollBarPolicy = XmSTATIC;
    }
   if (new->swindow.ScrollPolicy == XmAUTOMATIC)
    {
    	if (new->swindow.hScrollBar != current->swindow.hScrollBar)
	{
	    _XmWarning(new, SWMessage9);
	    new->swindow.hScrollBar = current->swindow.hScrollBar;	
	}
    	if (new->swindow.vScrollBar != current->swindow.vScrollBar)
	{
	    _XmWarning(new, SWMessage9);
	    new->swindow.vScrollBar = current->swindow.vScrollBar;	
	}

    }

   if (new->swindow.ScrollPolicy == XmAS_NEEDED)
    {
    	if (new->swindow.hScrollBar != current->swindow.hScrollBar)
            if (new->swindow.hScrollBar != NULL)
              Flag = TRUE;
            else
	       new->swindow.hScrollBar = current->swindow.hScrollBar;	

    	if (new->swindow.vScrollBar != current->swindow.vScrollBar)
            if (new->swindow.vScrollBar != NULL)
              Flag = TRUE;
            else
	       new->swindow.vScrollBar = current->swindow.vScrollBar;	
    }

    if (new->swindow.ClipWindow != current->swindow.ClipWindow)
    {
	_XmWarning(new, SWMessage10);
	new->swindow.ClipWindow = current->swindow.ClipWindow;
    }
    
    if ((new->swindow.Placement != XmTOP_LEFT) &&
        (new->swindow.Placement != XmTOP_RIGHT) &&
        (new->swindow.Placement != XmBOTTOM_LEFT) &&
        (new->swindow.Placement != XmBOTTOM_RIGHT))
    {
	_XmWarning(new, SWMessage4);
	new->swindow.Placement = current->swindow.Placement;
    }	
    if ((new->swindow.Placement != current->swindow.Placement)  ||
	(new->swindow.hScrollBar != current->swindow.hScrollBar)||
	(new->swindow.vScrollBar != current->swindow.vScrollBar)||
	(new->swindow.WorkWindow != current->swindow.WorkWindow)||
	(new->swindow.pad != current->swindow.pad))
        {
            if ((new->swindow.hScrollBar != current->swindow.hScrollBar) &&
                (current->swindow.hScrollBar != NULL))
                    if (XtIsRealized(current->swindow.hScrollBar))
                        XtUnmapWidget(current->swindow.hScrollBar);
                    else
                        XtSetMappedWhenManaged(current->swindow.hScrollBar,
                                               FALSE);
            if ((new->swindow.vScrollBar != current->swindow.vScrollBar) &&
                (current->swindow.vScrollBar != NULL))
                    if (XtIsRealized(current->swindow.vScrollBar))
                        XtUnmapWidget(current->swindow.vScrollBar);
                    else
                        XtSetMappedWhenManaged(current->swindow.vScrollBar,
                                               FALSE);
	    if ((new->swindow.hScrollBar != current->swindow.hScrollBar)||
	        (new->swindow.vScrollBar != current->swindow.vScrollBar)||
	        (new->swindow.WorkWindow != current->swindow.WorkWindow))
                InitializeScrollBars((Widget)new);
           SetBoxSize(new);
           Flag = TRUE;
        }
      
    return (Flag);
 }

/************************************************************************
 *									*
 * Spiffy API Functions							*
 *									*
 ************************************************************************/

/************************************************************************
 *									*
 * XmScrolledWindowSetAreas - set a new widget set.			*
 *									*
 ************************************************************************/
#ifdef _NO_PROTO
void XmScrolledWindowSetAreas(w, hscroll, vscroll, wregion)
    Widget w;
    Widget hscroll;
    Widget vscroll;
    Widget wregion;
#else /* _NO_PROTO */
void XmScrolledWindowSetAreas (Widget w, Widget hscroll, Widget vscroll, Widget wregion)
#endif /* _NO_PROTO */
{
    XmScrolledWindowWidget sw = (XmScrolledWindowWidget) w;    
    sw->swindow.WorkWindow = wregion;
    if (sw->swindow.ScrollPolicy != XmAUTOMATIC)
    {
        if ((sw->swindow.hScrollBar) && 
            (hscroll != (Widget )sw->swindow.hScrollBar))
            if (XtIsRealized(sw->swindow.hScrollBar))
                XtUnmapWidget(sw->swindow.hScrollBar);
            else
                XtSetMappedWhenManaged(sw->swindow.hScrollBar, FALSE);
            
        if ((sw->swindow.vScrollBar) && 
            (vscroll != (Widget )sw->swindow.vScrollBar))
            if (XtIsRealized(sw->swindow.vScrollBar))
                XtUnmapWidget(sw->swindow.vScrollBar);
            else
                XtSetMappedWhenManaged(sw->swindow.vScrollBar, FALSE);
          
	sw->swindow.hScrollBar = (XmScrollBarWidget) hscroll;
	sw->swindow.vScrollBar = (XmScrollBarWidget) vscroll;
        InitializeScrollBars((Widget)sw);
        SetBoxSize(sw);
    }
    if (XtIsRealized(sw)) Resize(sw);
}


/************************************************************************
 *									*
 * XmCreateScrolledWindow - hokey interface to XtCreateWidget.		*
 *									*
 ************************************************************************/
#ifdef _NO_PROTO
Widget XmCreateScrolledWindow(parent, name, args, argCount)
Widget   parent;
char     *name;
ArgList  args;
Cardinal argCount;
#else /* _NO_PROTO */
Widget XmCreateScrolledWindow (Widget parent, char *name, ArgList args, Cardinal argCount)
#endif /* _NO_PROTO */
{

    return ( XtCreateWidget( name, 
			     xmScrolledWindowWidgetClass, 
			     parent, 
			     args, 
			     argCount ) );
}
