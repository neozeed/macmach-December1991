#ifdef REV_INFO
#ifndef lint
static char SCCSID[] = "OSF/Motif: @(#)ScrollBar.c	3.30.1.2 91/04/01";
#endif /* lint */
#endif /* REV_INFO */
/******************************************************************************
*******************************************************************************
*
*  (c) Copyright 1989, 1990, 1991 OPEN SOFTWARE FOUNDATION, INC.
*  (c) Copyright 1989, DIGITAL EQUIPMENT CORPORATION, MAYNARD, MASS.
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

#include <stdio.h>
#include <X11/keysym.h>
#include <Xm/ScrollBarP.h>



#define MESSAGE1 "The scrollbar minimum value is greater than or equal to\nthe scrollbar maximum value."
#define MESSAGE2 "The specified slider size is less than 1"
#define MESSAGE3 "The specified scrollbar value is less than the minimum\nscrollbar value."
#define MESSAGE4 "The specified scrollbar value is greater than the maximum\nscrollbar value minus the scrollbar slider size."
#define MESSAGE5 "Incorrect orientation."
#define MESSAGE6 "Incorrect processing direction."
#define MESSAGE7 "The scrollbar increment is less than 1."
#define MESSAGE8 "The scrollbar page increment is less than 1."
#define MESSAGE9 "The scrollbar initial delay is less than 1."
#define MESSAGE10 "The scrollbar repeat delay is less than 1."
#define MESSAGE11 "Error in context manager; scrollbar backgrounds cannot be set correctly"
#define MESSAGE12 "Error in context manager; scrollbar foregrounds cannot be set correctly"
#define MESSAGE13 "Specified slider size is greater than the scrollbar maximum\nvalue minus the scrollbar minimum value."

#define FIRST_SCROLL_FLAG (1<<0)
#define VALUE_SET_FLAG    (1<<1)


/*  Class functions routine definitions  */

static void    ClassPartInitialize();
static void    Initialize();
static void    Redisplay();
static void    Resize();
static void    Realize();
static void    Destroy();
static Boolean SetValues();




/*  Internal functions  */
static void ProcessingDirectionDefault();

static void ExportScrollBarValue();
static XmImportOperator ImportScrollBarValue();

static void ForegroundPixelDefault();
static void TroughPixelDefault();

static int  CalcSliderVal();
static void CalcSliderRect();

static void GetForegroundGC();

static void GetSliderPixmap();
static void InitSliding();
static void MoveSlider();
static void DrawSlider();
static void RedrawSlider();

static Boolean ChangeScrollBarValue();
static void TimerEvent();
static void ScrollCallback();
static void ScrollBarUnhighlight();
static void GetUnhighlightGC ();


/*  Event procedures referenced in the actions list  */

static void Select();
static void Release();
static void Moved();
static void TopOrBottom();
static void IncrementUpOrLeft();
static void IncrementDownOrRight();
static void PageUpOrLeft();
static void PageDownOrRight();

/* Internal Structure for TimerEvent client_data */
typedef struct _XmScrollBarTimerRec
{
	XmScrollBarWidget	sbw;
	XButtonPressedEvent	event;
} XmScrollBarTimerRec;


/*  Default translation table and action list  */

#ifndef MCCABE
static char defaultTranslations[] =
"~Shift ~Ctrl ~Meta ~Alt <Btn1Down>:	Select()\n\
 ~Shift ~Ctrl ~Meta ~Alt <Btn1Up>:	Release()\n\
 ~Shift ~Ctrl ~Meta ~Alt Button1<PtrMoved>:	Moved()\n\
 ~Shift ~Ctrl ~Meta ~Alt <Btn2Down>:	Select()\n\
 ~Shift ~Ctrl ~Meta ~Alt <Btn2Up>:	Release()\n\
 ~Shift ~Ctrl ~Meta ~Alt Button2<PtrMoved>:	Moved()\n\
 ~Shift Ctrl ~Meta ~Alt <Btn1Down>:	TopOrBottom()\n\
 ~Shift Ctrl ~Meta ~Alt <Btn1Up>:	Release()\n\
 ~Shift Ctrl <Key>osfPageDown:	PageDownOrRight(1)\n\
 ~Shift Ctrl <Key>osfPageUp:	PageUpOrLeft(1)\n\
 <Key>osfBeginLine:	TopOrBottom()\n\
 <Key>osfEndLine:	TopOrBottom()\n\
 <Key>osfPageUp:	PageUpOrLeft(0)\n\
 <Key>osfPageDown:	PageDownOrRight(0)\n\
 <Key>osfHelp:		PrimitiveHelp()\n\
 ~Shift ~Ctrl <Key>osfUp:	IncrementUpOrLeft(0)\n\
 ~Shift ~Ctrl <Key>osfDown:	IncrementDownOrRight(0)\n\
 ~Shift ~Ctrl <Key>osfLeft:	IncrementUpOrLeft(1)\n\
 ~Shift ~Ctrl <Key>osfRight:	IncrementDownOrRight(1)\n\
 ~Shift Ctrl <Key>osfUp:	PageUpOrLeft(0)\n\
 ~Shift Ctrl <Key>osfDown:	PageDownOrRight(0)\n\
 ~Shift Ctrl <Key>osfLeft:	PageUpOrLeft(1)\n\
 ~Shift Ctrl <Key>osfRight:	PageDownOrRight(1)\n\
 Shift ~Meta ~Alt <Key>Tab:	PrimitivePrevTabGroup()\n\
 ~Meta ~Alt <Key>Tab:		PrimitiveNextTabGroup()\n\
 <FocusIn>:	PrimitiveFocusIn()\n\
 <FocusOut>:	PrimitiveFocusOut()\n\
 <Unmap>:	PrimitiveUnmap()\n\
 <Enter>:	PrimitiveEnter()\n\
 <Leave>:	PrimitiveLeave()";
#else
static char defaultTranslations[];
#endif

static XtActionsRec actionsList[] =
{
	{ "Select",                 (XtActionProc) Select },
	{ "Release",                (XtActionProc) Release },
	{ "Moved",                  (XtActionProc) Moved },
	{ "TopOrBottom",            (XtActionProc) TopOrBottom },
	{ "IncrementUpOrLeft",      (XtActionProc) IncrementUpOrLeft },
	{ "IncrementDownOrRight",   (XtActionProc) IncrementDownOrRight },
	{ "PageUpOrLeft",           (XtActionProc) PageUpOrLeft },
	{ "PageDownOrRight",        (XtActionProc) PageDownOrRight },
	{ "PrimitiveFocusIn",       (XtActionProc) _XmPrimitiveFocusIn },
	{ "PrimitiveFocusOut",      (XtActionProc) _XmPrimitiveFocusOut },
	{ "PrimitiveUnmap",         (XtActionProc) _XmPrimitiveUnmap },
	{ "PrimitiveEnter",         (XtActionProc) _XmPrimitiveEnter },
	{ "PrimitiveLeave",         (XtActionProc) _XmPrimitiveLeave },
	{ "PrimitiveHelp",          (XtActionProc) _XmPrimitiveHelp },
	{ "PrimitiveTraverseLeft",  (XtActionProc) _XmTraverseLeft },
	{ "PrimitiveTraverseRight", (XtActionProc) _XmTraverseRight },
	{ "PrimitiveTraverseUp",    (XtActionProc) _XmTraverseUp },
	{ "PrimitiveTraverseDown",  (XtActionProc) _XmTraverseDown },
	{ "PrimitiveTraverseNext",  (XtActionProc) _XmTraverseNext },
	{ "PrimitiveTraversePrev",  (XtActionProc) _XmTraversePrev },
	{ "PrimitiveTraverseHome",  (XtActionProc) _XmTraverseHome },
	{ "PrimitiveNextTabGroup",  (XtActionProc) _XmTraverseNextTabGroup},
	{ "PrimitivePrevTabGroup",  (XtActionProc) _XmTraversePrevTabGroup},
};


/*  Resource list for ScrollBar  */

static XtResource resources[] = 
{
	{ XmNnavigationType, XmCNavigationType, XmRNavigationType,
	  sizeof(unsigned char),
	  XtOffset(XmScrollBarWidget, primitive.navigation_type),
	  XmRImmediate, (caddr_t) XmSTICKY_TAB_GROUP
	},
	{ XmNforeground, XmCForeground, XmRPixel, sizeof(Pixel),
	  XtOffset(XmScrollBarWidget, primitive.foreground),
	  XmRCallProc, (caddr_t) ForegroundPixelDefault
	},
	{ XmNtroughColor, XmCTroughColor, XmRPixel, sizeof(Pixel),
	  XtOffset(XmScrollBarWidget, scrollBar.trough_color),
	  XmRCallProc, (caddr_t) TroughPixelDefault
	},
	{ XmNvalue, XmCValue, XmRInt, sizeof (int),
	  XtOffset(XmScrollBarWidget, scrollBar.value),
	  XmRImmediate, (caddr_t) XmINVALID_DIMENSION     /* what else? */
	},
	{ XmNminimum, XmCMinimum, XmRInt, sizeof (int),
	  XtOffset(XmScrollBarWidget, scrollBar.minimum),
	  XmRImmediate, (caddr_t) 0
	},
	{ XmNmaximum, XmCMaximum, XmRInt, sizeof (int),
	  XtOffset(XmScrollBarWidget, scrollBar.maximum),
	  XmRImmediate, (caddr_t) 100
	},
	{ XmNsliderSize, XmCSliderSize, XmRInt, sizeof (int),
	  XtOffset(XmScrollBarWidget, scrollBar.slider_size),
	  XmRImmediate, (caddr_t) XmINVALID_DIMENSION
	},
	{ XmNshowArrows, XmCShowArrows, XmRBoolean, sizeof (Boolean),
	  XtOffset(XmScrollBarWidget, scrollBar.show_arrows),
	  XmRImmediate, (caddr_t) True
	},
	{ XmNorientation, XmCOrientation, 
	  XmROrientation, sizeof (unsigned char),
	  XtOffset(XmScrollBarWidget, scrollBar.orientation),
	  XmRImmediate, (caddr_t) XmVERTICAL
	},
	{ XmNprocessingDirection, XmCProcessingDirection, 
	  XmRProcessingDirection, sizeof (unsigned char), 
	  XtOffset(XmScrollBarWidget, scrollBar.processing_direction),
	  XmRCallProc, (caddr_t) ProcessingDirectionDefault
	},
	{ XmNincrement, XmCIncrement, XmRInt, sizeof (int),
	  XtOffset(XmScrollBarWidget, scrollBar.increment),
	  XmRImmediate, (caddr_t) 1
	},
	{ XmNpageIncrement, XmCPageIncrement, XmRInt, sizeof (int),
	  XtOffset(XmScrollBarWidget, scrollBar.page_increment),
	  XmRImmediate, (caddr_t) 10
	},
	{ XmNinitialDelay, XmCInitialDelay, XmRInt, sizeof (int),
	  XtOffset(XmScrollBarWidget, scrollBar.initial_delay),
	  XmRImmediate, (caddr_t) 250
	},
	{ XmNrepeatDelay, XmCRepeatDelay, XmRInt, sizeof (int),
	  XtOffset(XmScrollBarWidget, scrollBar.repeat_delay),
	  XmRImmediate, (caddr_t) 50
	},
	{ XmNvalueChangedCallback, XmCCallback, 
	  XmRCallback, sizeof(XtCallbackList),
	  XtOffset(XmScrollBarWidget, scrollBar.value_changed_callback),
	  XmRPointer, (caddr_t) NULL
	},
	{ XmNincrementCallback, XmCCallback, 
	  XmRCallback, sizeof(XtCallbackList),
	  XtOffset(XmScrollBarWidget, scrollBar.increment_callback),
	  XmRPointer, (caddr_t) NULL
	},
	{ XmNdecrementCallback, XmCCallback, 
	  XmRCallback, sizeof(XtCallbackList),
	  XtOffset(XmScrollBarWidget, scrollBar.decrement_callback),
	  XmRPointer, (caddr_t) NULL
	},
	{ XmNpageIncrementCallback, XmCCallback,
	  XmRCallback, sizeof(XtCallbackList),
	  XtOffset(XmScrollBarWidget, scrollBar.page_increment_callback),
	  XmRPointer, (caddr_t) NULL
	},
	{ XmNpageDecrementCallback, XmCCallback, 
	  XmRCallback, sizeof (XtCallbackList),
	  XtOffset(XmScrollBarWidget, scrollBar.page_decrement_callback),
	  XmRPointer, (caddr_t) NULL
	},
	{ XmNtoTopCallback, XmCCallback,
	  XmRCallback, sizeof(XtCallbackList),
	  XtOffset(XmScrollBarWidget, scrollBar.to_top_callback),
	  XmRPointer, (caddr_t) NULL
	},
	{ XmNtoBottomCallback, XmCCallback,
	  XmRCallback, sizeof(XtCallbackList),
	  XtOffset(XmScrollBarWidget, scrollBar.to_bottom_callback),
	  XmRPointer, (caddr_t) NULL
	},
	{ XmNdragCallback, XmCCallback,
	  XmRCallback, sizeof(XtCallbackList),
	  XtOffset(XmScrollBarWidget, scrollBar.drag_callback),
	  XmRPointer, (caddr_t) NULL
	},
        {
         XmNtraversalOn, XmCTraversalOn, XmRBoolean, sizeof (Boolean),
         XtOffset (XmPrimitiveWidget, primitive.traversal_on),
         XmRImmediate, (caddr_t) FALSE
        },
        {
          XmNhighlightThickness, XmCHighlightThickness, XmRHorizontalDimension,
          sizeof (Dimension),
          XtOffset (XmPrimitiveWidget, primitive.highlight_thickness),
          XmRImmediate, (caddr_t) 0
        },
};


/*  Definition for resources that need special processing in get values  */

static XmSyntheticResource syn_resources[] =
{
	{ XmNvalue,
	  sizeof (int),
	  XtOffset (XmScrollBarWidget, scrollBar.value), 
          ExportScrollBarValue,
          ImportScrollBarValue,
	},
};


/*  The ScrollBar class record definition  */

externaldef(xmscrollbarclassrec) XmScrollBarClassRec xmScrollBarClassRec =
{
   {
      (WidgetClass) &xmPrimitiveClassRec, /* superclass	         */
      "XmScrollBar",                    /* class_name	         */
      sizeof(XmScrollBarRec),           /* widget_size	         */
      NULL,                             /* class_initialize      */
      ClassPartInitialize,              /* class_part_initialize */
      FALSE,                            /* class_inited          */
      (XtInitProc) Initialize,          /* initialize	         */
      NULL,                             /* initialize_hook       */
      Realize,                          /* realize	         */	
      actionsList,                      /* actions               */	
      XtNumber(actionsList),            /* num_actions	         */	
      resources,                        /* resources	         */	
      XtNumber(resources),              /* num_resources         */	
      NULLQUARK,                        /* xrm_class	         */	
      TRUE,                             /* compress_motion       */	
      XtExposeCompressMaximal,          /* compress_exposure     */	
      TRUE,                             /* compress_enterleave   */
      FALSE,                            /* visible_interest      */	
      (XtWidgetProc) Destroy,           /* destroy               */	
      (XtWidgetProc) Resize,            /* resize                */	
      (XtExposeProc) Redisplay,         /* expose                */	
      (XtSetValuesFunc) SetValues,      /* set_values    	 */	
      NULL,                             /* set_values_hook       */
      XtInheritSetValuesAlmost,         /* set_values_almost     */
      NULL,				/* get_values_hook       */
      NULL,                             /* accept_focus	         */	
      XtVersion,                        /* version               */
      NULL,                             /* callback private      */
      defaultTranslations,              /* tm_table              */
      NULL,                             /* query_geometry        */
      NULL,                             /* display_accelerator   */
      NULL,                             /* extension             */
   },

   {
      _XtInherit,			/* border_highlight   */
      ScrollBarUnhighlight,			/* border_unhighlight */
      NULL,				/* translations       */
      NULL,				/* arm_and_activate   */
      syn_resources,   			/* syn_resources      */
      XtNumber(syn_resources),		/* num syn_resources  */
      NULL,				/* extension          */
   },

   {
      NULL,				/* extension          */
   },
};

externaldef(xmscrollbarwidgetclass) WidgetClass xmScrollBarWidgetClass = (WidgetClass) &xmScrollBarClassRec;





/*********************************************************************
 *
 *  ClassPartInitialize
 *     Initialize the fast subclassing.
 *
 *********************************************************************/

static void ClassPartInitialize (wc)
WidgetClass wc;
{
	_XmFastSubclassInit (wc, XmSCROLL_BAR_BIT);
}

static void ScrollBarUnhighlight(sbw)
XmScrollBarWidget sbw;
{
	register int x;
	register int y;
	register int window_width;
	register int window_height;
	register highlight_width;

	x = sbw->core.x;
	y = sbw->core.y;
	window_width = sbw->core.width;
	window_height = sbw->core.height;

	if ((!window_width) || (!window_height))
		return;

	highlight_width = sbw->primitive.highlight_thickness;

	if (!highlight_width)
		return;

	x = 0;
	y = 0;
	sbw->primitive.highlighted = False;
	sbw->primitive.highlight_drawn = False;

	if (XmIsManager (sbw->core.parent))
	{
		_XmDrawBorder ((Widget)sbw, ((XmManagerWidget)(sbw->core.parent))
			->manager.background_GC,
			0, 0, window_width, window_height, highlight_width);
	}
	else
	{
		_XmDrawBorder ((Widget)sbw, sbw->scrollBar.unhighlight_GC,
			0, 0, window_width, window_height, highlight_width);
	}
}



/*********************************************************************
 *
 * ProcessingDirectionDefault
 *    This procedure provides the dynamic default behavior for 
 *    the processing direction resource dependent on the orientation.
 *
 *********************************************************************/

static void ProcessingDirectionDefault (widget, offset, value)
XmScrollBarWidget   widget;
int      offset;
XrmValue *value;
{
	static unsigned char direction;

	value->addr = (caddr_t) &direction;

	if (widget->scrollBar.orientation == XmHORIZONTAL)
		direction = XmMAX_ON_RIGHT;
	else /* XmVERTICAL  -- range checking done during widget
	                       initialization */
		direction = XmMAX_ON_BOTTOM;
}




/*********************************************************************
 *
 * ForegroundPixelDefault
 *    This procedure provides the dynamic default behavior for 
 *    the foreground color.
 *
 *********************************************************************/
static void ForegroundPixelDefault (widget, offset, value)
XmScrollBarWidget   widget;
int      offset;
XrmValue *value;
{
	static Pixel foreground;

	value->addr = (caddr_t) &foreground;

	foreground = widget->core.background_pixel;
}



/*********************************************************************
 *
 * TroughPixelDefault
 *    This procedure provides the dynamic default behavior for 
 *    the trough color.
 *
 *********************************************************************/
static void TroughPixelDefault (widget, offset, value)
XmScrollBarWidget   widget;
int      offset;
XrmValue *value;
{
	static Pixel trough;
	XmColorData *pixel_data;

	value->addr = (caddr_t) &trough;

	pixel_data = _XmGetColors(XtScreen((Widget)widget),
		widget->core.colormap, widget->core.background_pixel);
	
	trough = _XmAccessColorData(pixel_data, XmSELECT);
}





/*********************************************************************
 *
 *  Initialize
 *     The main widget instance initialization routine.
 *
 *********************************************************************/

static void Initialize (request, new)
XmScrollBarWidget request, new;

{
	/* Validate the incoming data  */                      
    Boolean default_value = False;

        if (new->scrollBar.value == XmINVALID_DIMENSION) {
	    new->scrollBar.value = 0 ;
	    default_value = True ;
	}

	if (new->scrollBar.minimum >= new->scrollBar.maximum)
	{
		new->scrollBar.minimum = 0;
		new->scrollBar.maximum = 100;
		_XmWarning (new, MESSAGE1);
	}

	if (new->scrollBar.slider_size == XmINVALID_DIMENSION) {
	    new->scrollBar.slider_size = (new->scrollBar.maximum - 
					new->scrollBar.minimum)/10 ;
	    if (new->scrollBar.slider_size < 1)
		new->scrollBar.slider_size = 1;
	}

	if (new->scrollBar.slider_size < 1)
	{
		new->scrollBar.slider_size = 1;
		_XmWarning (new, MESSAGE2);
	}

	if (new->scrollBar.slider_size > 
		(new->scrollBar.maximum - new->scrollBar.minimum))
	{
		new->scrollBar.slider_size = new->scrollBar.maximum
			- new->scrollBar.minimum;
		_XmWarning (new, MESSAGE13);
	}

	if (new->scrollBar.value < new->scrollBar.minimum)
	{
		new->scrollBar.value = new->scrollBar.minimum;
		if (!default_value) _XmWarning (new, MESSAGE3);
	}

	if (new->scrollBar.value > 
		new->scrollBar.maximum - new->scrollBar.slider_size)
	{
		new->scrollBar.value =
		new->scrollBar.maximum - new->scrollBar.slider_size;
		if (!default_value) _XmWarning (new, MESSAGE4);
	}

	if ((new->scrollBar.orientation != XmHORIZONTAL) &&
		(new->scrollBar.orientation != XmVERTICAL))
	{
		new->scrollBar.orientation = XmVERTICAL;
		_XmWarning (new, MESSAGE5);
	}

	if (new->scrollBar.orientation == XmHORIZONTAL)
	{
		if ((new->scrollBar.processing_direction != XmMAX_ON_RIGHT) &&
			(new->scrollBar.processing_direction != XmMAX_ON_LEFT))

		{
			new->scrollBar.processing_direction = XmMAX_ON_RIGHT;
			_XmWarning (new, MESSAGE6);
		}
	}
	else
	{
		if ((new->scrollBar.processing_direction != XmMAX_ON_TOP) &&
			(new->scrollBar.processing_direction != XmMAX_ON_BOTTOM))
		{
			new->scrollBar.processing_direction = XmMAX_ON_BOTTOM;
			_XmWarning (new, MESSAGE6);
		}
	}

	if (new->scrollBar.increment <= 0)
	{
		new->scrollBar.increment = 1;
		_XmWarning (new, MESSAGE7);
	}

	if (new->scrollBar.page_increment <= 0)
	{
		new->scrollBar.page_increment = 10;
		_XmWarning (new, MESSAGE8);
	}

	if (new->scrollBar.initial_delay <= 0)
	{
		new->scrollBar.initial_delay = 250;
		_XmWarning (new, MESSAGE9);
	}

	if (new->scrollBar.repeat_delay <= 0)
	{
		new->scrollBar.repeat_delay = 50;
		_XmWarning (new, MESSAGE10);
	}

	/*  Set up a geometry for the widget if it is currently 0.  */

	if (request->core.width == 0)
	{
		if (new->scrollBar.orientation == XmHORIZONTAL)
			 new->core.width += 100;
		else
			 new->core.width += 11;
	}
	if (request->core.height == 0)
	{
		if (new->scrollBar.orientation == XmHORIZONTAL)
			 new->core.height += 11;
		else
			 new->core.height += 100;
	}

	/*  Reverse the value for reverse processing.  */

	if ((new->scrollBar.processing_direction == XmMAX_ON_LEFT) ||
		(new->scrollBar.processing_direction == XmMAX_ON_TOP))
		new->scrollBar.value = new->scrollBar.maximum 
			+ new->scrollBar.minimum - new->scrollBar.value
			- new->scrollBar.slider_size;

	/*  Set the internally used variables.  */

	new->scrollBar.pixmap = 0;
	new->scrollBar.sliding_on = False;
	new->scrollBar.timer = 0;
	new->scrollBar.etched_slider = False;

	new->scrollBar.arrow_width = 0;
	new->scrollBar.arrow_height = 0;

	new->scrollBar.arrow1_x = 0;
	new->scrollBar.arrow2_x = 0;
	new->scrollBar.arrow1_y = 0;
	new->scrollBar.arrow2_y = 0;
	new->scrollBar.arrow1_selected = False;
	new->scrollBar.arrow2_selected = False;

	new->scrollBar.arrow1_top_count = 0;
	new->scrollBar.arrow1_cent_count = 0 ;
	new->scrollBar.arrow1_bot_count = 0 ;
	new->scrollBar.arrow1_top = NULL;
	new->scrollBar.arrow1_cent = NULL;
	new->scrollBar.arrow1_bot = NULL;

	new->scrollBar.arrow2_top_count = 0;
	new->scrollBar.arrow2_cent_count = 0 ;
	new->scrollBar.arrow2_bot_count = 0 ;
	new->scrollBar.arrow2_top = NULL;
	new->scrollBar.arrow2_cent = NULL;
	new->scrollBar.arrow2_bot = NULL;

	new->scrollBar.flags = 0;

        /*  Get the drawing graphics contexts.  */

	GetForegroundGC(new);
	GetUnhighlightGC(new);
	Resize(new);
}




/************************************************************************
 *
 *  GetForegroundGC
 *     Get the graphics context used for drawing the slider and arrows.
 *
 ************************************************************************/

static void GetForegroundGC (sbw)
XmScrollBarWidget sbw;

{
    XGCValues values;
    XtGCMask  valueMask;

    valueMask = GCForeground | GCBackground | GCGraphicsExposures;
    values.foreground = sbw->core.background_pixel;
    values.background = sbw->core.background_pixel;
    values.graphics_exposures = False;

    sbw->scrollBar.foreground_GC = XtGetGC (sbw, valueMask, &values);
}


static void GetUnhighlightGC (sbw)
XmScrollBarWidget sbw;

{
    XGCValues values;
    XtGCMask  valueMask;

    valueMask = GCForeground | GCBackground | GCGraphicsExposures;
    values.foreground = sbw->primitive.foreground;
    values.background = sbw->primitive.foreground;
    values.graphics_exposures = False;

    sbw->scrollBar.unhighlight_GC = XtGetGC (sbw, valueMask, &values);
}




/************************************************************************
 *
 *  GetSliderPixmap
 *     Create the pixmap for the slider and render the slider.
 *
 ************************************************************************/

static void GetSliderPixmap (sbw)
XmScrollBarWidget sbw;

{
   short slider_x, slider_y, slider_width, slider_height;

   CalcSliderRect (sbw, &slider_x, &slider_y, &slider_width, &slider_height);

   sbw->scrollBar.pixmap = 
      XCreatePixmap (XtDisplay(sbw), RootWindowOfScreen (XtScreen(sbw)), 
                     slider_width, slider_height, sbw->core.depth);

   sbw->scrollBar.slider_x = slider_x;
   sbw->scrollBar.slider_y = slider_y;
   sbw->scrollBar.slider_width = slider_width;
   sbw->scrollBar.slider_height = slider_height;

   DrawSlider(sbw);
}




/************************************************************************
 *
 *  CalcSliderRect
 *     Calculate the slider location and size in pixels so that
 *     it can be drawn.
 *
 ************************************************************************/

static void CalcSliderRect (sbw, slider_x, slider_y,
	slider_width, slider_height)
XmScrollBarWidget sbw;
short * slider_x;
short * slider_y;
short * slider_width;
short * slider_height;

{
   float userSize;
   float trueSize;
   float factor;
   float slideStart;
   float slideSize;
   int arrowWidth;
   int minSliderWidth;
   int minSliderHeight;

   userSize = sbw->scrollBar.maximum - sbw->scrollBar.minimum;

   if (sbw->scrollBar.orientation == XmHORIZONTAL)
   {
      arrowWidth = sbw->scrollBar.arrow_width;
      trueSize =  sbw->scrollBar.slider_area_width;
   }
   else
   {
      arrowWidth = sbw->scrollBar.arrow_height;
      trueSize = sbw->scrollBar.slider_area_height;
   }


   if (arrowWidth == 0)
      arrowWidth -= 1;

   arrowWidth += sbw->primitive.highlight_thickness + 
                 sbw->primitive.shadow_thickness + 1;


   factor = trueSize / userSize;

   slideStart = (float)(sbw->scrollBar.value - 
                 sbw->scrollBar.minimum) * factor + arrowWidth;

   slideSize = (float) (sbw->scrollBar.slider_size) * factor;

   if (sbw->scrollBar.orientation == XmHORIZONTAL)
   {
      *slider_x = (int) (slideStart + 0.5);
      *slider_y = sbw->scrollBar.slider_area_y;
      *slider_width = (int) (slideSize + 0.5);
      *slider_height = sbw->scrollBar.slider_area_height;
      minSliderWidth = MIN_SLIDER_LENGTH;
      minSliderHeight = MIN_SLIDER_THICKNESS;
   }
   else
   {
      *slider_x = sbw->scrollBar.slider_area_x;
      *slider_y = (int) (slideStart + 0.5);
      *slider_width = sbw->scrollBar.slider_area_width;
      *slider_height = (int) (slideSize + 0.5);
      minSliderWidth = MIN_SLIDER_THICKNESS;
      minSliderHeight = MIN_SLIDER_LENGTH;
   }

   if (*slider_width < minSliderWidth)
   {
      *slider_width = minSliderWidth;
      if (sbw->scrollBar.orientation == XmHORIZONTAL &&
          *slider_x + *slider_width > 
          sbw->scrollBar.slider_area_x + sbw->scrollBar.slider_area_width)
         *slider_x = sbw->scrollBar.slider_area_x + 
                     sbw->scrollBar.slider_area_width - *slider_width;
   }

   if (*slider_height < minSliderHeight)
   {
      *slider_height = minSliderHeight;
      if (sbw->scrollBar.orientation == XmVERTICAL &&
          *slider_y + *slider_height > 
          sbw->scrollBar.slider_area_y + sbw->scrollBar.slider_area_height)
         *slider_y = sbw->scrollBar.slider_area_y + 
                     sbw->scrollBar.slider_area_height - *slider_height;
   }
}




/************************************************************************
 *
 *  DrawSlider
 *     Drawing function for drawing the slider.
 *
 ************************************************************************/

static void DrawSlider (sbw)
XmScrollBarWidget sbw;

{
   register int slider_width = sbw->scrollBar.slider_width;
   register int slider_height = sbw->scrollBar.slider_height;
   register Drawable slider = sbw->scrollBar.pixmap;


   XFillRectangle (XtDisplay ((Widget) sbw), slider,
                   sbw->scrollBar.foreground_GC,
                   0, 0, slider_width, slider_height);
   
   _XmDrawShadow (XtDisplay (sbw), slider,
                  sbw->primitive.top_shadow_GC,
                  sbw->primitive.bottom_shadow_GC, 
                  sbw->primitive.shadow_thickness ? 
		  sbw->primitive.shadow_thickness : (short)1,
                  0, 0, slider_width, slider_height);

   if (sbw->scrollBar.etched_slider)
   {
      if (sbw->scrollBar.orientation == XmHORIZONTAL)
      {
         XDrawLine (XtDisplay (sbw), slider,
                    sbw->primitive.bottom_shadow_GC,
                    slider_width / 2 - 1, 1, 
                    slider_width / 2 - 1, slider_height - 2);
         XDrawLine (XtDisplay (sbw), slider,
                    sbw->primitive.top_shadow_GC,
                    slider_width / 2, 1, 
                    slider_width / 2, slider_height - 2);
      }
      else
      {
         XDrawLine (XtDisplay (sbw), slider,
                    sbw->primitive.bottom_shadow_GC,
                    1, slider_height / 2 - 1,
                    slider_width - 2, slider_height / 2 - 1);
         XDrawLine (XtDisplay (sbw), slider,
                    sbw->primitive.top_shadow_GC,
                    1, slider_height / 2,
                    slider_width - 2, slider_height / 2);
      }
   }
}





/************************************************************************
 *
 *  Redisplay
 *     General redisplay function called on exposure events.
 *
 ************************************************************************/

static void Redisplay (sbw, event, region)
XmScrollBarWidget sbw;
XEvent *event;
Region region;

{
   if (sbw->scrollBar.pixmap == 0) GetSliderPixmap(sbw);

   if (sbw->primitive.shadow_thickness >= 0)
      _XmDrawShadow (XtDisplay (sbw), XtWindow (sbw), 
                     sbw->primitive.bottom_shadow_GC, 
                     sbw->primitive.top_shadow_GC,
                     sbw->primitive.shadow_thickness ?
		     sbw->primitive.shadow_thickness : (short)1,
                     sbw->primitive.highlight_thickness,
                     sbw->primitive.highlight_thickness,
                     sbw->core.width-2 * sbw->primitive.highlight_thickness,
                     sbw->core.height-2 * sbw->primitive.highlight_thickness);

   XClearArea(XtDisplay ((Widget) sbw), XtWindow ((Widget) sbw),
	(int) sbw->scrollBar.slider_area_x,
	(int) sbw->scrollBar.slider_area_y,
	(unsigned int) sbw->scrollBar.slider_area_width,
	(unsigned int) sbw->scrollBar.slider_area_height,
	(Bool) FALSE);

   XCopyArea (XtDisplay ((Widget) sbw),
              sbw->scrollBar.pixmap, XtWindow ((Widget) sbw),
              sbw->scrollBar.foreground_GC,
              0, 0, sbw->scrollBar.slider_width, 
              sbw->scrollBar.slider_height,
              sbw->scrollBar.slider_x, sbw->scrollBar.slider_y);


   if (sbw -> scrollBar.show_arrows)
   {
      if (sbw->scrollBar.arrow1_selected)
      {
         XFillRectangles (XtDisplay ((Widget) sbw), XtWindow ((Widget) sbw),
                          sbw->primitive.bottom_shadow_GC,
                          sbw->scrollBar.arrow1_top,
                          sbw->scrollBar.arrow1_top_count);
         XFillRectangles (XtDisplay ((Widget) sbw), XtWindow ((Widget) sbw),
                          sbw -> primitive.top_shadow_GC,
                          sbw->scrollBar.arrow1_bot,
                          sbw->scrollBar.arrow1_bot_count);
      }
      else
      {
         XFillRectangles (XtDisplay ((Widget) sbw), XtWindow ((Widget) sbw),
                          sbw -> primitive.top_shadow_GC,
                          sbw->scrollBar.arrow1_top,
                          sbw->scrollBar.arrow1_top_count);
         XFillRectangles (XtDisplay ((Widget) sbw), XtWindow ((Widget) sbw),
                          sbw->primitive.bottom_shadow_GC,
                          sbw->scrollBar.arrow1_bot,
                          sbw->scrollBar.arrow1_bot_count);
      }

      if (sbw->scrollBar.arrow2_selected)
      {
         XFillRectangles (XtDisplay ((Widget) sbw), XtWindow ((Widget) sbw),
                          sbw->primitive.bottom_shadow_GC,
                          sbw->scrollBar.arrow2_top,
                          sbw->scrollBar.arrow2_top_count);
         XFillRectangles (XtDisplay ((Widget) sbw), XtWindow ((Widget) sbw),
                          sbw -> primitive.top_shadow_GC,
                          sbw->scrollBar.arrow2_bot,
                          sbw->scrollBar.arrow2_bot_count);
      }
      else
      {
         XFillRectangles (XtDisplay ((Widget) sbw), XtWindow ((Widget) sbw),
                          sbw->primitive.top_shadow_GC,
                          sbw->scrollBar.arrow2_top,
                          sbw->scrollBar.arrow2_top_count);
         XFillRectangles (XtDisplay ((Widget) sbw), XtWindow ((Widget) sbw),
                          sbw->primitive.bottom_shadow_GC,
                          sbw->scrollBar.arrow2_bot,
                          sbw->scrollBar.arrow2_bot_count);
      }

      XFillRectangles (XtDisplay ((Widget) sbw), XtWindow ((Widget) sbw),
                       sbw->scrollBar.foreground_GC,
                       sbw->scrollBar.arrow1_cent,
                       sbw->scrollBar.arrow1_cent_count);

      XFillRectangles (XtDisplay ((Widget) sbw), XtWindow ((Widget) sbw),
                       sbw->scrollBar.foreground_GC,
                       sbw->scrollBar.arrow2_cent,
                       sbw->scrollBar.arrow2_cent_count);
   }


   if (sbw -> primitive.highlighted)
      (*(((XmPrimitiveWidgetClass) XtClass(sbw))
		->primitive_class.border_highlight)) (sbw);
   else
      (*(((XmPrimitiveWidgetClass) XtClass(sbw))
		->primitive_class.border_unhighlight)) (sbw);
}





/************************************************************************
 *
 *  Resize
 *     Process resizes on the widget by destroying and recreating the
 *     slider pixmap.
 *
 ************************************************************************/

static void Resize (sbw)
XmScrollBarWidget sbw;

{
   register int ht = sbw->primitive.highlight_thickness;
   register int st = sbw->primitive.shadow_thickness;


   /*  Calcualte all of the internal data for slider and arrow placement  */

   if (sbw->scrollBar.show_arrows)
   {
      if (sbw->scrollBar.orientation == XmHORIZONTAL)
      {
         sbw->scrollBar.arrow1_orientation = XmARROW_LEFT;
         sbw->scrollBar.arrow2_orientation = XmARROW_RIGHT;


         /*  left arrow position and size  */

         sbw->scrollBar.arrow1_x = ht + st;
         sbw->scrollBar.arrow1_y = ht + st;
         sbw->scrollBar.arrow_width = 
            sbw->scrollBar.arrow_height = sbw->core.height - 2 * (ht + st);

         if (sbw->core.width < 
             2 * (sbw->scrollBar.arrow_width + ht + st) + MIN_SLIDER_LENGTH + 2)
            sbw->scrollBar.arrow_width = 
               (sbw->core.width - (MIN_SLIDER_LENGTH + 2 + 2 * (ht + st))) / 2;


         /*  slide area position and size  */

         sbw->scrollBar.slider_area_x = 
            ht + st + sbw->scrollBar.arrow_width + 1;
         sbw->scrollBar.slider_area_width =
            sbw->core.width - 2 * (ht + st + sbw->scrollBar.arrow_width + 1);
      
         sbw->scrollBar.slider_area_y = ht + st;
         sbw->scrollBar.slider_area_height = sbw->core.height - 2 * (ht + st);


         /*  right arrow position  */

         sbw->scrollBar.arrow2_x = ht + st + sbw->scrollBar.arrow_width + 1 +
            sbw->scrollBar.slider_area_width + 1;
         sbw->scrollBar.arrow2_y = ht + st;
      }

      else
      {
         sbw->scrollBar.arrow1_orientation = XmARROW_UP;
         sbw->scrollBar.arrow2_orientation = XmARROW_DOWN;


         /*  top arrow position and size  */

         sbw->scrollBar.arrow1_x = ht + st;
         sbw->scrollBar.arrow1_y = ht + st;
         sbw->scrollBar.arrow_width = 
            sbw->scrollBar.arrow_height = sbw->core.width - 2 * (ht + st);
         
         if (sbw->core.height < 
             2 * (sbw->scrollBar.arrow_height + ht + st) + MIN_SLIDER_LENGTH +2)
            sbw->scrollBar.arrow_height = 
               (sbw->core.height - (MIN_SLIDER_LENGTH + 2 + 2 * (ht + st))) / 2;


         /*  slide area position and size  */

         sbw->scrollBar.slider_area_y = 
            ht + st + sbw->scrollBar.arrow_height + 1;
         sbw->scrollBar.slider_area_height =
            sbw->core.height - 2 * (ht + st + sbw->scrollBar.arrow_height +1);
      
         sbw->scrollBar.slider_area_x = ht + st;
         sbw->scrollBar.slider_area_width = sbw->core.width - 2 * (ht + st);


         /*  right arrow position  */

         sbw->scrollBar.arrow2_y = ht + st + sbw->scrollBar.arrow_height + 1 +
            sbw->scrollBar.slider_area_height + 1;
         sbw->scrollBar.arrow2_x = ht + st;
      }
   }
   else
   {
      sbw->scrollBar.arrow_width = 0;
      sbw->scrollBar.arrow_height = 0;

      if (sbw->scrollBar.orientation == XmHORIZONTAL)
      {
         /*  slide area position and size  */

         sbw->scrollBar.slider_area_x = ht + st;
         sbw->scrollBar.slider_area_width = sbw->core.width - 2 * (ht + st);

         sbw->scrollBar.slider_area_y = ht + st;
         sbw->scrollBar.slider_area_height = sbw->core.height - 2 * (ht + st);
      }
      else
      {
         /*  slide area position and size  */

         sbw->scrollBar.slider_area_y = ht + st;
         sbw->scrollBar.slider_area_height = sbw->core.height - 2 * (ht + st);

         sbw->scrollBar.slider_area_x = ht + st;
         sbw->scrollBar.slider_area_width = sbw->core.width - 2 * (ht + st);
      }
   }


   /*  Get the two arrow rectangle draw sets  */

   if (sbw->scrollBar.show_arrows)
   {

      if (sbw->scrollBar.arrow_width > 0 && sbw->scrollBar.arrow_height > 0)
      {
         _XmGetArrowDrawRects (ht, st-1, sbw->scrollBar.arrow1_orientation,
                               sbw->scrollBar.arrow_width + 2 * (ht + st),
                               sbw->scrollBar.arrow_height + 2 * (ht + st),
                               &(sbw->scrollBar.arrow1_top_count),
                               &(sbw->scrollBar.arrow1_cent_count),
                               &(sbw->scrollBar.arrow1_bot_count),
                               &(sbw->scrollBar.arrow1_top),
                               &(sbw->scrollBar.arrow1_cent),
                               &(sbw->scrollBar.arrow1_bot));

         _XmGetArrowDrawRects (ht, st-1, sbw->scrollBar.arrow2_orientation,
                               sbw->scrollBar.arrow_width + 2 * (ht + st),
                               sbw->scrollBar.arrow_height + 2 * (ht + st),
                               &(sbw->scrollBar.arrow2_top_count),
                               &(sbw->scrollBar.arrow2_cent_count),
                               &(sbw->scrollBar.arrow2_bot_count),
                               &(sbw->scrollBar.arrow2_top),
                               &(sbw->scrollBar.arrow2_cent),
                               &(sbw->scrollBar.arrow2_bot));

         _XmOffsetArrow (sbw->scrollBar.arrow2_x - ht - st,
                         sbw->scrollBar.arrow2_y - ht - st,
                         sbw->scrollBar.arrow2_top, sbw->scrollBar.arrow2_cent,
                         sbw->scrollBar.arrow2_bot, sbw->scrollBar.arrow2_top_count,
                         sbw->scrollBar.arrow2_cent_count,
                         sbw->scrollBar.arrow2_bot_count);
      }
      else
      {
         if (sbw->scrollBar.arrow1_top_count != 0)
         {
            XtFree (sbw->scrollBar.arrow1_top);
            sbw->scrollBar.arrow1_top_count = 0;
            sbw->scrollBar.arrow1_top = NULL;
         }

         if (sbw->scrollBar.arrow1_cent_count != 0)
         {
            XtFree (sbw->scrollBar.arrow1_cent);
            sbw->scrollBar.arrow1_cent_count = 0;
            sbw->scrollBar.arrow1_cent = NULL;
         }
         if (sbw->scrollBar.arrow1_bot_count != 0)
         {
            XtFree (sbw->scrollBar.arrow1_bot);
            sbw->scrollBar.arrow1_bot_count = 0;
            sbw->scrollBar.arrow1_bot = NULL;
         }

         if (sbw->scrollBar.arrow2_top_count != 0)
         {
            XtFree (sbw->scrollBar.arrow2_top);
            sbw->scrollBar.arrow2_top_count = 0;
            sbw->scrollBar.arrow2_top = NULL;
         }

         if (sbw->scrollBar.arrow2_cent_count != 0)
         {
            XtFree (sbw->scrollBar.arrow2_cent);
            sbw->scrollBar.arrow2_cent_count = 0;
            sbw->scrollBar.arrow2_cent = NULL;
         }
         if (sbw->scrollBar.arrow2_bot_count != 0)
         {
            XtFree (sbw->scrollBar.arrow2_bot);
            sbw->scrollBar.arrow2_bot_count = 0;
            sbw->scrollBar.arrow2_bot = NULL;
         }
      }
   }


   if (XtIsRealized (sbw))
   {
      if (sbw->scrollBar.pixmap)
      {
         XFreePixmap (XtDisplay (sbw), sbw->scrollBar.pixmap);
         sbw->scrollBar.pixmap = 0;
      }
      GetSliderPixmap (sbw);
   }
   else
	CalcSliderRect (sbw, &(sbw->scrollBar.slider_x),
		&(sbw->scrollBar.slider_y), &(sbw->scrollBar.slider_width),
		&(sbw->scrollBar.slider_height));

   if ((sbw->scrollBar.orientation == XmVERTICAL) &&
	(sbw->scrollBar.slider_area_width <= 0))
   	sbw->scrollBar.slider_area_width = sbw->scrollBar.slider_width;
   if ((sbw->scrollBar.orientation == XmHORIZONTAL) &&
	(sbw->scrollBar.slider_area_height <= 0))
   	sbw->scrollBar.slider_area_height = sbw->scrollBar.slider_height;
}




/*********************************************************************
 *
 * Realize
 *
 ********************************************************************/
static void Realize (sbw, window_mask, window_attributes)
XmScrollBarWidget sbw;
Mask *window_mask;
XSetWindowAttributes *window_attributes;
{
	(*window_mask) |= (CWBackPixel | CWBitGravity);
	window_attributes->background_pixel = sbw->scrollBar.trough_color;
	window_attributes->bit_gravity = ForgetGravity;
	
	XtCreateWindow (sbw, InputOutput, CopyFromParent, *window_mask,
		window_attributes);
}




/************************************************************************
 *
 *  Destroy
 *	Clean up allocated resources when the widget is destroyed.
 *
 ************************************************************************/

static void Destroy (sbw)
XmScrollBarWidget sbw;

{
   if (sbw->scrollBar.arrow1_top != NULL)
   {
      XtFree (sbw->scrollBar.arrow1_top);
      XtFree (sbw->scrollBar.arrow1_cent);
      XtFree (sbw->scrollBar.arrow1_bot);
      XtFree (sbw->scrollBar.arrow2_top);
      XtFree (sbw->scrollBar.arrow2_cent);
      XtFree (sbw->scrollBar.arrow2_bot);
   }

   XtReleaseGC (sbw, sbw->scrollBar.foreground_GC);

   if (sbw->scrollBar.pixmap != 0)
      XFreePixmap (XtDisplay (sbw), sbw->scrollBar.pixmap);

   XtRemoveAllCallbacks (sbw, XmNvalueChangedCallback);
   XtRemoveAllCallbacks (sbw, XmNincrementCallback);
   XtRemoveAllCallbacks (sbw, XmNdecrementCallback);
   XtRemoveAllCallbacks (sbw, XmNpageIncrementCallback);
   XtRemoveAllCallbacks (sbw, XmNpageDecrementCallback);
   XtRemoveAllCallbacks (sbw, XmNdragCallback);
   XtRemoveAllCallbacks (sbw, XmNtoTopCallback);
   XtRemoveAllCallbacks (sbw, XmNtoBottomCallback);

   if (sbw->scrollBar.timer != 0)
   {
      XtRemoveTimeOut (sbw->scrollBar.timer);
      sbw->scrollBar.timer = 0;
   }
}




/************************************************************************
 *
 *  SetValues
 *
 ************************************************************************/
static Boolean ValidateInputs(current, request, new)
XmScrollBarWidget current, request, new;
{
	Boolean returnFlag = TRUE;

	/* Validate the incoming data  */                      

	if (new->scrollBar.minimum >= new->scrollBar.maximum)
	{
		new->scrollBar.minimum = current->scrollBar.minimum;
		new->scrollBar.maximum = current->scrollBar.maximum;
		_XmWarning(new, MESSAGE1);
		returnFlag = FALSE;
	}

	if (new->scrollBar.slider_size < 1)
	{
		if ((new->scrollBar.maximum - new->scrollBar.minimum) <
			current->scrollBar.slider_size)
			new->scrollBar.slider_size = new->scrollBar.maximum
				- new->scrollBar.minimum;
		else
			new->scrollBar.slider_size = current->scrollBar.slider_size;
		_XmWarning(new, MESSAGE2);
		returnFlag = FALSE;
	}
	if (new->scrollBar.slider_size > 
		(new->scrollBar.maximum - new->scrollBar.minimum))
	{
		if ((new->scrollBar.maximum - new->scrollBar.minimum) <
			current->scrollBar.slider_size)
			new->scrollBar.slider_size = new->scrollBar.maximum
				- new->scrollBar.minimum;
		else
			new->scrollBar.slider_size = current->scrollBar.slider_size;
		_XmWarning(new, MESSAGE13);
		returnFlag = FALSE;
	}
	if (new->scrollBar.value < new->scrollBar.minimum)
	{
		new->scrollBar.value = new->scrollBar.minimum;
		_XmWarning(new, MESSAGE3);
		returnFlag = FALSE;
	}

	if (new->scrollBar.value > 
			new->scrollBar.maximum - new->scrollBar.slider_size)
	{
		new->scrollBar.value = 
		new->scrollBar.maximum - new->scrollBar.slider_size;
		_XmWarning(new, MESSAGE4);
		returnFlag = FALSE;
	}

	if ((new->scrollBar.orientation != XmHORIZONTAL) &&
		(new->scrollBar.orientation != XmVERTICAL))
	{
		new->scrollBar.orientation = current->scrollBar.orientation;
		_XmWarning(new, MESSAGE5);
		returnFlag = FALSE;
	}

	if (new->scrollBar.orientation == XmHORIZONTAL)
	{
		if ((new->scrollBar.processing_direction != XmMAX_ON_LEFT) &&
			(new->scrollBar.processing_direction != XmMAX_ON_RIGHT))
		{
			new->scrollBar.processing_direction = 
			current->scrollBar.processing_direction;
			_XmWarning(new, MESSAGE6);
			returnFlag = FALSE;
		}
	}
	else /* new->scrollBar.orientation == XmVERTICAL */
	{
		if ((new->scrollBar.processing_direction != XmMAX_ON_TOP) &&
			(new->scrollBar.processing_direction != XmMAX_ON_BOTTOM))
		{
			new->scrollBar.processing_direction =
			current->scrollBar.processing_direction;
			_XmWarning(new, MESSAGE6);
			returnFlag = FALSE;
		}
	}

	if (new->scrollBar.increment <= 0)
	{
		new->scrollBar.increment = current->scrollBar.increment;
		_XmWarning(new, MESSAGE7);
		returnFlag = FALSE;
	}

	if (new->scrollBar.page_increment <= 0)
	{
		new->scrollBar.page_increment = 
			current->scrollBar.page_increment;
		_XmWarning(new,  MESSAGE8);
		returnFlag = FALSE;
	}

	if (new->scrollBar.initial_delay <= 0)
	{
		new->scrollBar.initial_delay = current->scrollBar.initial_delay;
		_XmWarning(new, MESSAGE9);
		returnFlag = FALSE;
	}

	if (new->scrollBar.repeat_delay <= 0)
	{
		new->scrollBar.repeat_delay = current->scrollBar.repeat_delay;
		_XmWarning(new, MESSAGE10);
		returnFlag = FALSE;
	}

	return(returnFlag);
}

static Boolean SetValues (current, request, new)
XmScrollBarWidget current, request, new;

{
	Boolean returnFlag = FALSE;
  Boolean current_backwards =
          ((current->scrollBar.processing_direction == XmMAX_ON_LEFT)
          || (current->scrollBar.processing_direction == XmMAX_ON_TOP));
  Boolean new_backwards =
          ((new->scrollBar.processing_direction == XmMAX_ON_LEFT)
          || (new->scrollBar.processing_direction == XmMAX_ON_TOP));
  int saved_new_value = new->scrollBar.value;

	/* Make sure that processing direction tracks orientation */

	if ((new->scrollBar.orientation != current->scrollBar.orientation)
		&&
		(new->scrollBar.processing_direction ==
			current->scrollBar.processing_direction))
	{
		if ((new->scrollBar.orientation == XmHORIZONTAL) &&
			(current->scrollBar.processing_direction == XmMAX_ON_TOP))
			new->scrollBar.processing_direction = XmMAX_ON_LEFT;
		else if ((new->scrollBar.orientation == XmHORIZONTAL) &&
			(current->scrollBar.processing_direction ==
				XmMAX_ON_BOTTOM))
			new->scrollBar.processing_direction = XmMAX_ON_RIGHT;
		else if ((new->scrollBar.orientation == XmVERTICAL) &&
			(current->scrollBar.processing_direction == XmMAX_ON_LEFT))
			new->scrollBar.processing_direction = XmMAX_ON_TOP;
		else if ((new->scrollBar.orientation == XmVERTICAL) &&
			(current->scrollBar.processing_direction == XmMAX_ON_RIGHT))
			new->scrollBar.processing_direction = XmMAX_ON_BOTTOM;
	}

	while (!ValidateInputs(current, request, new));

  /*
   * Because someone somewhere originally thought that it was clever
   * for the scrollbar widget to do all of its internal processing in
   * just one direction, all of the interface procedures have to go
   * through extreme gymnastics to support reversal.
   */
  if (new->scrollBar.flags & VALUE_SET_FLAG)
	{
     if (current_backwards)
     {
  			new->scrollBar.value = new->scrollBar.maximum
				+ new->scrollBar.minimum - new->scrollBar.value
				- new->scrollBar.slider_size;
     }

     new->scrollBar.flags &= ~VALUE_SET_FLAG;
	}

  if (new->scrollBar.processing_direction !=
                  current->scrollBar.processing_direction)
	{
    if (new_backwards)
            new->scrollBar.value = new->scrollBar.maximum
            				+ new->scrollBar.minimum - new->scrollBar.value
		            		- new->scrollBar.slider_size;
     else
            new->scrollBar.value = saved_new_value;

	}

	/*  See if the GC needs to be regenerated  */

	if ((new->primitive.foreground != current->primitive.foreground) ||
		(new->core.background_pixel != current->core.background_pixel))
	{
		XtReleaseGC(new, new->scrollBar.foreground_GC);
		GetForegroundGC(new);
	}

	/*
	 * See if the trough (a.k.a the window background) needs to be
	 * changed.
	 */
	if ((new->scrollBar.trough_color !=
			current->scrollBar.trough_color)
		&&
		(XtIsRealized(new)))
	{
		XSetWindowBackground(XtDisplay((Widget)new),
			XtWindow((Widget)new), new->scrollBar.trough_color);
		returnFlag = TRUE;
	}
		
	if (new->core.background_pixel !=
		current->core.background_pixel)
	{
		XtReleaseGC(new, new->scrollBar.unhighlight_GC);
		GetUnhighlightGC(new);
	}

	/*
	 * See if the widget needs to be redrawn.  Minimize the amount
	 * of redraw by having specific checks.
	 */

	if ((new->scrollBar.orientation != 
			current->scrollBar.orientation)         ||
		(new->primitive.shadow_thickness !=
			current->primitive.shadow_thickness)    || 
		(new->primitive.highlight_thickness !=
			current->primitive.highlight_thickness) || 
		(new->scrollBar.show_arrows != 
				current->scrollBar.show_arrows))
	{
		returnFlag = TRUE;
		Resize(new);
	}
	else 
	    if ((new->primitive.foreground != current->primitive.foreground)
		||
		(new->core.background_pixel != current->core.background_pixel)
		||
		(new->primitive.top_shadow_color != 
		                     current->primitive.top_shadow_color)
		||
		(new->primitive.bottom_shadow_color != 
		                     current->primitive.bottom_shadow_color))
	{
		returnFlag = TRUE;

		if (new->scrollBar.pixmap != 0)
		{
			XFreePixmap(XtDisplay (new), new->scrollBar.pixmap);
			new->scrollBar.pixmap = 0;
		}
	}
	else if ((new->scrollBar.slider_size != 
			current->scrollBar.slider_size)                    ||
		(new->scrollBar.minimum != current->scrollBar.minimum) ||
		(new->scrollBar.maximum != current->scrollBar.maximum) ||
		(new->scrollBar.processing_direction != 
			current->scrollBar.processing_direction))
	{
		if (new->scrollBar.pixmap != 0)
		{
			XFreePixmap(XtDisplay (new), new->scrollBar.pixmap);
			new->scrollBar.pixmap = 0;
			if (XtIsRealized(new))
				XClearArea(XtDisplay((Widget)new), 
					XtWindow((Widget)new),
					new->scrollBar.slider_x, new->scrollBar.slider_y,
					new->scrollBar.slider_width,
					new->scrollBar.slider_height, False);

			GetSliderPixmap(new);
		}
		RedrawSlider(new);
	}
	else if (new->scrollBar.value != current->scrollBar.value)
		RedrawSlider(new);

	return(returnFlag);
}




/************************************************************************
 *
 *  CalcSliderVal
 *     Calculate the slider val in application coordiates given
 *     the input x and y.
 *
 ************************************************************************/

static int CalcSliderVal (sbw, x, y)
XmScrollBarWidget sbw;
int   x;
int   y;

{
   int   borderWidth;
   float userSize;
   float trueSize;
   float slideStart;
   float slideSize;
   float referencePoint;
   float temp;
   int int_temp;
   int arrow_size;


   borderWidth = sbw->primitive.highlight_thickness +
                 sbw->primitive.shadow_thickness;

   if (sbw->scrollBar.orientation == XmHORIZONTAL)
   {
      referencePoint = (float) x - sbw->scrollBar.separation_x;
      trueSize = sbw->scrollBar.slider_area_width;
      arrow_size = sbw->scrollBar.arrow_width;
   }
   else
   {
      referencePoint = (float) y - sbw->scrollBar.separation_y;
      trueSize = sbw->scrollBar.slider_area_height;
      arrow_size = sbw->scrollBar.arrow_height;
   }
   
   temp = (referencePoint - arrow_size - borderWidth) / trueSize;

   userSize = sbw->scrollBar.maximum - sbw->scrollBar.minimum;
   temp = temp * userSize + sbw->scrollBar.minimum + 0.5;
   int_temp = (int) temp;

   if (int_temp < sbw->scrollBar.minimum)
      int_temp = sbw->scrollBar.minimum;
   else if (int_temp > sbw->scrollBar.maximum - sbw->scrollBar.slider_size)
      int_temp = sbw->scrollBar.maximum - sbw->scrollBar.slider_size;

   return (int_temp);
}




/************************************************************************
 *
 *  Select
 *     This function processes selections occuring on the scrollBar.
 *
 ************************************************************************/

static void Select (sbw, buttonEvent)
XmScrollBarWidget      sbw;
XButtonPressedEvent * buttonEvent;
{
	int borderWidth = sbw->primitive.highlight_thickness + 
	sbw->primitive.shadow_thickness;
	int selectionPos;
	int slider_x = sbw->scrollBar.slider_x;
	int slider_y = sbw->scrollBar.slider_y;
	int slider_width = sbw->scrollBar.slider_width;
	int slider_height = sbw->scrollBar.slider_height;
	Boolean slider_moved;
	XmScrollBarTimerRec	*timer_rec;

	if (buttonEvent->button == Button1)
		(void) XmProcessTraversal((Widget)sbw, XmTRAVERSE_CURRENT);

	sbw->scrollBar.separation_x = 0;
	sbw->scrollBar.separation_y = 0;

	/*  Calculate whether the selection point is in the slider  */
	if ((buttonEvent->x >= slider_x)                 &&
		(buttonEvent->x <= slider_x + slider_width - 1)   &&
		(buttonEvent->y >= slider_y)                      &&
		(buttonEvent->y <= slider_y + slider_height - 1))
	{
		sbw->scrollBar.separation_x = buttonEvent->x - slider_x;
		sbw->scrollBar.separation_y = buttonEvent->y - slider_y;
		sbw->scrollBar.initial_x = slider_x;
		sbw->scrollBar.initial_y = slider_y;
		sbw->scrollBar.sliding_on = True;
		sbw->scrollBar.saved_value = sbw->scrollBar.value;
		sbw->scrollBar.arrow1_selected = FALSE;
		sbw->scrollBar.arrow2_selected = FALSE;

		/* 
		 * This is a little sleazy, but it's either this or setting
		 * a flag to be evaluated outside of this conditional chain.
		 */
		return;
	}

	/* ... in the trough (i.e. slider area)... */
	else if ((buttonEvent->x >= sbw->scrollBar.slider_area_x)   &&
		(buttonEvent->y >= sbw->scrollBar.slider_area_y)        &&
		(buttonEvent->x <= sbw->scrollBar.slider_area_x 
			+ sbw->scrollBar.slider_area_width)                 &&
		(buttonEvent->y <= sbw->scrollBar.slider_area_y 
			+ sbw->scrollBar.slider_area_height))
	{
		sbw->scrollBar.arrow1_selected = FALSE;
		sbw->scrollBar.arrow2_selected = FALSE;
		if (buttonEvent->button == Button1)
		{
			/* Page the slider up or down */

			if (sbw->scrollBar.orientation == XmHORIZONTAL)
			{
				if (buttonEvent->x < sbw->scrollBar.slider_x)
					sbw->scrollBar.change_type = XmCR_PAGE_DECREMENT;
				else
					sbw->scrollBar.change_type = XmCR_PAGE_INCREMENT;
			}
			else
			{
				if (buttonEvent->y < sbw->scrollBar.slider_y)
					sbw->scrollBar.change_type = XmCR_PAGE_DECREMENT;
				else
					sbw->scrollBar.change_type = XmCR_PAGE_INCREMENT;
			}
		}
		else  /* Button2 */
		{
			/* Warp the slider to the cursor, and then drag */

			if (sbw->scrollBar.orientation == XmHORIZONTAL)
				sbw->scrollBar.separation_x = 
					sbw->scrollBar.slider_width / 2;
			else
				sbw->scrollBar.separation_y = 
					sbw->scrollBar.slider_height / 2;

			sbw->scrollBar.initial_x = slider_x;
			sbw->scrollBar.initial_y = slider_y;
			sbw->scrollBar.sliding_on = True;
			sbw->scrollBar.saved_value = sbw->scrollBar.value;

			Moved (sbw, buttonEvent);

			/* 
			 * This is a little sleazy, but it's either this or setting
			 * a flag to be evaluated outside of this conditional chain.
			 */
			return;
		}
	}

	/* ... in arrow 1 */
	else if ((buttonEvent->x >= sbw->scrollBar.arrow1_x)  &&
		(buttonEvent->y >= sbw->scrollBar.arrow1_y)       &&
		(buttonEvent->x <= sbw->scrollBar.arrow1_x 
			+ sbw->scrollBar.arrow_width)                 &&
		(buttonEvent->y <= sbw->scrollBar.arrow1_y 
			+ sbw->scrollBar.arrow_height))
	{
		sbw->scrollBar.change_type = XmCR_DECREMENT;
		sbw->scrollBar.arrow1_selected = True;

		XFillRectangles (XtDisplay((Widget)sbw),
			XtWindow((Widget)sbw),
			sbw->primitive.bottom_shadow_GC,
			sbw->scrollBar.arrow1_top,
			sbw->scrollBar.arrow1_top_count);
		XFillRectangles (XtDisplay((Widget)sbw), 
			XtWindow((Widget)sbw),
			sbw -> primitive.top_shadow_GC,
			sbw->scrollBar.arrow1_bot,
			sbw->scrollBar.arrow1_bot_count);
	}

	/* ... in arrow 2 */
	else if ((buttonEvent->x >= sbw->scrollBar.arrow2_x)      &&
		(buttonEvent->y >= sbw->scrollBar.arrow2_y)           &&
		(buttonEvent->x <= sbw->scrollBar.arrow2_x 
			+ sbw->scrollBar.arrow_width)                     &&
		(buttonEvent->y <= sbw->scrollBar.arrow2_y 
			+ sbw->scrollBar.arrow_height))
	{
		sbw->scrollBar.change_type = XmCR_INCREMENT;
		sbw->scrollBar.arrow2_selected = True;

		XFillRectangles (XtDisplay((Widget)sbw),
			XtWindow((Widget)sbw),
			sbw->primitive.bottom_shadow_GC,
			sbw->scrollBar.arrow2_top,
			sbw->scrollBar.arrow2_top_count);
		XFillRectangles (XtDisplay((Widget)sbw),
			XtWindow((Widget)sbw),
			sbw -> primitive.top_shadow_GC,
			sbw->scrollBar.arrow2_bot,
			sbw->scrollBar.arrow2_bot_count);
	}
	else
		/* ... in the highlight area.  */
		return;


	slider_moved = ChangeScrollBarValue (sbw);

	if (slider_moved)
	{
		ScrollCallback (sbw, sbw->scrollBar.change_type, 
			sbw->scrollBar.value, 0, 0, NULL);

		XSync (XtDisplay((Widget)sbw), False);

    sbw->scrollBar.flags |= FIRST_SCROLL_FLAG;

		timer_rec = (XmScrollBarTimerRec *) 
			XtMalloc(sizeof(XmScrollBarTimerRec));
		timer_rec->sbw = sbw;
		timer_rec->event = *buttonEvent;

		sbw->scrollBar.timer = 
			XtAppAddTimeOut (XtWidgetToApplicationContext(sbw),
				(unsigned long) sbw->scrollBar.initial_delay,
				(XtTimerCallbackProc) TimerEvent, (caddr_t) timer_rec);
	}
}




/************************************************************************
 *
 *  Release
 *     This function processes releases occuring on the scrollBar.
 *
 ************************************************************************/

static void Release(sbw, event)
XmScrollBarWidget sbw;
XButtonEvent * event;

{
   if (sbw->scrollBar.arrow1_selected)
   {
      sbw->scrollBar.arrow1_selected = False;

      XFillRectangles (XtDisplay ((Widget) sbw), XtWindow ((Widget) sbw),
                       sbw -> primitive.top_shadow_GC,
                       sbw->scrollBar.arrow1_top,
                       sbw->scrollBar.arrow1_top_count);
      XFillRectangles (XtDisplay ((Widget) sbw), XtWindow ((Widget) sbw),
                       sbw->primitive.bottom_shadow_GC,
                       sbw->scrollBar.arrow1_bot,
                       sbw->scrollBar.arrow1_bot_count);
   }

   if (sbw->scrollBar.arrow2_selected)
   {
      sbw->scrollBar.arrow2_selected = False;

      XFillRectangles (XtDisplay ((Widget) sbw), XtWindow ((Widget) sbw),
                       sbw->primitive.top_shadow_GC,
                       sbw->scrollBar.arrow2_top,
                       sbw->scrollBar.arrow2_top_count);
      XFillRectangles (XtDisplay ((Widget) sbw), XtWindow ((Widget) sbw),
                       sbw->primitive.bottom_shadow_GC,
                       sbw->scrollBar.arrow2_bot,
                       sbw->scrollBar.arrow2_bot_count);
   }

   if (sbw->scrollBar.timer != 0)
   {
      XtRemoveTimeOut (sbw->scrollBar.timer);
      sbw->scrollBar.timer = 0;
   }

   if (sbw->scrollBar.sliding_on == True)
   {
     sbw->scrollBar.sliding_on = False;
	 ScrollCallback (sbw, XmCR_VALUE_CHANGED, sbw->scrollBar.value, 
					 event->x, event->y, event);
   }
}




/************************************************************************
 *
 *  Moved
 *     This function processes mouse moved events during interactive
 *     slider moves.
 *
 ************************************************************************/

static void Moved (sbw, event)
XmScrollBarWidget sbw;
XEvent * event;

{
	XButtonPressedEvent * buttonEvent = (XButtonPressedEvent *) event;
	int newX, newY;
	int realX, realY;
	int slideVal;
	int borderWidth;
	int button_x;
	int button_y;

	borderWidth = sbw->primitive.highlight_thickness
		+ sbw->primitive.shadow_thickness;


	if (sbw->scrollBar.sliding_on != True)
		return;

	button_x = buttonEvent->x;
	button_y = buttonEvent->y;

	/*
	 * Force button_x and button_y to be within the slider_area.
	 */
	if (button_x < sbw->scrollBar.slider_area_x)
		button_x = sbw->scrollBar.slider_area_x;

	if (button_x > 
		sbw->scrollBar.slider_area_x + sbw->scrollBar.slider_area_width)
		button_x = sbw->scrollBar.slider_area_x 
			+ sbw->scrollBar.slider_area_width;

	if (button_y < sbw->scrollBar.slider_area_y)
		button_y = sbw->scrollBar.slider_area_y;

	if (button_y > 
		sbw->scrollBar.slider_area_y 
			+ sbw->scrollBar.slider_area_height)
		button_y = sbw->scrollBar.slider_area_y 
			+ sbw->scrollBar.slider_area_height;


	/*
	 * Calculate the new origin of the slider.  
	 * Bound the values with the slider area.
	 */
	if (sbw->scrollBar.orientation == XmHORIZONTAL)
	{
		newX = realX = button_x - sbw->scrollBar.separation_x;
		newY = realY = sbw->scrollBar.slider_y;

		if (newX < sbw->scrollBar.slider_area_x)
			newX = sbw->scrollBar.slider_area_x;

		if (newX + sbw->scrollBar.slider_width > 
			sbw->scrollBar.slider_area_x 
			+ sbw->scrollBar.slider_area_width)
			newX = sbw->scrollBar.slider_area_x
				+ sbw->scrollBar.slider_area_width
				- sbw->scrollBar.slider_width;
	}
	else
	{
		newX = realX = sbw->scrollBar.slider_x;
		newY = realY = button_y - sbw->scrollBar.separation_y;


		if (newY < sbw->scrollBar.slider_area_y)
			newY = sbw->scrollBar.slider_area_y;

		if (newY + sbw->scrollBar.slider_height > 
			sbw->scrollBar.slider_area_y 
			+ sbw->scrollBar.slider_area_height)
			newY = sbw->scrollBar.slider_area_y 
				+ sbw->scrollBar.slider_area_height
				- sbw->scrollBar.slider_height;
	}


	if (((sbw->scrollBar.orientation == XmHORIZONTAL) && 
		(realX != sbw->scrollBar.initial_x))
		||
		((sbw->scrollBar.orientation == XmVERTICAL)   &&
		(realY != sbw->scrollBar.initial_y)))
	{
		slideVal = CalcSliderVal (sbw, button_x, button_y);

		if ((newX != sbw->scrollBar.initial_x) || 
			(newY != sbw->scrollBar.initial_y))
		{
			MoveSlider (sbw, newX, newY);
			sbw->scrollBar.initial_x = newX;
			sbw->scrollBar.initial_y = newY;
		}

		if (slideVal != sbw->scrollBar.value)
		{
			ScrollCallback(sbw, XmCR_DRAG, 
				sbw->scrollBar.value = slideVal, 
				buttonEvent->x, buttonEvent->y, buttonEvent);
		}
	}
}




/*********************************************************************
 *
 *  TopOrBottom
 *	Issue the to top or bottom callbacks.
 *
 *********************************************************************/

static void TopOrBottom (sbw, event)
XmScrollBarWidget sbw;
XEvent * event;
{
	XmScrollBarPart *sbp = (XmScrollBarPart *) &(sbw->scrollBar);


	if (event->type == KeyPress)
	{
		Modifiers junk;
		KeySym key_sym;
		XKeyPressedEvent *keyEvent = (XKeyPressedEvent *) event;

		/*
		XtTranslateKey(XtDisplay(sbw), keyEvent->keycode,
			keyEvent->state, &junk, &key_sym);
		*/

		key_sym = XtGetActionKeysym(event, &junk);
		
		if (key_sym == osfXK_BeginLine)
		{
			if (sbp->orientation == XmVERTICAL)
			{
				if (sbp->processing_direction == XmMAX_ON_BOTTOM)
					MoveSlider(sbw, sbp->slider_x, sbp->slider_area_y);
				else
					MoveSlider(sbw,
						sbp->slider_x, 
						sbp->slider_area_y + sbp->slider_area_height
							- sbp->slider_height);
			}
			else
			{
				if (sbp->processing_direction == XmMAX_ON_RIGHT)
					MoveSlider(sbw, sbp->slider_area_x, sbp->slider_y);
				else
					MoveSlider(sbw,
						sbp->slider_area_x + sbp->slider_area_width
							- sbp->slider_width,
						sbp->slider_y);
			}
			/*
			 * The following grevious bogosity is due to the fact
			 * that the key behavior was implemented long after 
			 * the rest of this code, and so we have to work around
			 * currently operating nonsense.
			 *
			 * Specifically, since the dawn of time, ScrollBar
			 * processes in just one direction, and does any necessary
			 * reversal just before calling the callback.
			 *
			 * We now proceed to trick that code into doing the right
			 * thing anyway
			 */
			if ((sbp->processing_direction == XmMAX_ON_BOTTOM) ||
				(sbp->processing_direction == XmMAX_ON_RIGHT))
			{
				sbp->value = sbp->minimum;
				ScrollCallback(sbw, XmCR_TO_TOP, sbp->value,
					keyEvent->x, keyEvent->y, keyEvent);
			}
			else
			{
				sbp->value = sbp->maximum - sbp->slider_size;
				ScrollCallback(sbw, XmCR_TO_BOTTOM, sbp->value,
					keyEvent->x, keyEvent->y, keyEvent);
			}
		}
		else /* key_sym == osfXK_EndLine */
		{
			if (sbp->orientation == XmVERTICAL)
			{
				if (sbp->processing_direction == XmMAX_ON_BOTTOM)
					MoveSlider(sbw,
						sbp->slider_x, 
						sbp->slider_area_y + sbp->slider_area_height
							- sbp->slider_height);
				else
					MoveSlider(sbw, sbp->slider_x, sbp->slider_area_y);
			}
			else
			{
				if (sbp->processing_direction == XmMAX_ON_RIGHT)
					MoveSlider(sbw,
						sbp->slider_area_x + sbp->slider_area_width
							- sbp->slider_width,
						sbp->slider_y);
				else
					MoveSlider(sbw, sbp->slider_area_x, sbp->slider_y);
			}

			/* See above for explanation of this nonsense */
			if ((sbp->processing_direction == XmMAX_ON_BOTTOM) ||
				(sbp->processing_direction == XmMAX_ON_RIGHT))
			{
				sbp->value = sbp->maximum - sbp->slider_size;
				ScrollCallback(sbw, XmCR_TO_BOTTOM, sbp->value,
					keyEvent->x, keyEvent->y, keyEvent);
			}
			else
			{
				sbp->value = sbp->minimum;
				ScrollCallback (sbw, XmCR_TO_TOP, sbp->value,
					keyEvent->x, keyEvent->y, keyEvent);
			}
		}
	}
	else  /* event->type == ButtonPress */
	{
		XButtonPressedEvent *buttonEvent =
			(XButtonPressedEvent *) event;

		if /* In arrow1... */
			((buttonEvent->x >= sbp->arrow1_x)                   &&
			(buttonEvent->y >= sbp->arrow1_y)                    &&
			(buttonEvent->x <= sbp->arrow1_x + sbp->arrow_width) &&
			(buttonEvent->y <= sbp->arrow1_y + sbp->arrow_height))
		{
			sbp->change_type = XmCR_DECREMENT;
			sbp->arrow1_selected = True;

			XFillRectangles (XtDisplay((Widget)sbw), 
				XtWindow((Widget)sbw),
				sbw->primitive.bottom_shadow_GC,
				sbp->arrow1_top,
				sbp->arrow1_top_count);
			XFillRectangles (XtDisplay((Widget)sbw),
				XtWindow((Widget)sbw),
				sbw -> primitive.top_shadow_GC,
				sbp->arrow1_bot,
				sbp->arrow1_bot_count);
			if (sbp->orientation == XmVERTICAL)
				MoveSlider(sbw, sbp->slider_x, sbp->slider_area_y);
			else
				MoveSlider(sbw, sbp->slider_area_x, sbp->slider_y);
			sbp->value = sbp->minimum;
			ScrollCallback (sbw, XmCR_TO_TOP, sbp->value,
				buttonEvent->x, buttonEvent->y, buttonEvent);
		}

		else if /* In arrow2... */
			((buttonEvent->x >= sbp->arrow2_x)  &&
			(buttonEvent->y >= sbp->arrow2_y)   &&
			(buttonEvent->x <= sbp->arrow2_x 
				+ sbp->arrow_width)             &&
			(buttonEvent->y <= sbp->arrow2_y 
				+ sbp->arrow_height))
		{
			sbp->change_type = XmCR_INCREMENT;
			sbp->arrow2_selected = True;

			XFillRectangles (XtDisplay((Widget)sbw),
				XtWindow((Widget)sbw),
				sbw->primitive.bottom_shadow_GC,
				sbp->arrow2_top,
				sbp->arrow2_top_count);
			XFillRectangles (XtDisplay((Widget)sbw),
				XtWindow((Widget)sbw),
				sbw -> primitive.top_shadow_GC,
				sbp->arrow2_bot,
				sbp->arrow2_bot_count);
			if (sbp->orientation == XmVERTICAL)
				MoveSlider(sbw,
					sbp->slider_x, 
					sbp->slider_area_y + sbp->slider_area_height
						- sbp->slider_height);
			else
				MoveSlider(sbw,
					sbp->slider_area_x + sbp->slider_area_width
						- sbp->slider_width,
					sbp->slider_y);
			sbp->value = sbp->maximum - sbp->slider_size;
			ScrollCallback (sbw, XmCR_TO_BOTTOM, 
				sbp->value,
				buttonEvent->x, buttonEvent->y, buttonEvent);
		} 
		else if /* in the trough between arrow1 and the slider... */
			(((sbp->orientation == XmHORIZONTAL)       &&
				(buttonEvent->x >= sbp->slider_area_x) &&
				(buttonEvent->x < sbp->slider_x)       &&
				(buttonEvent->y >= sbp->slider_area_y) &&
				(buttonEvent->y <= sbp->slider_area_y
					+ sbp->slider_area_height))
			||
			((sbp->orientation == XmVERTICAL) &&
				(buttonEvent->y >= sbp->slider_area_y)  &&
				(buttonEvent->y < sbp->slider_y)        &&
				(buttonEvent->x >= sbp->slider_area_x)  &&
				(buttonEvent->x < sbp->slider_area_x
					+ sbp->slider_area_width)))
		{
			if (sbp->orientation == XmVERTICAL)
				MoveSlider(sbw, sbp->slider_x, sbp->slider_area_y);
			else
				MoveSlider(sbw, sbp->slider_area_x, sbp->slider_y);
			sbp->value = sbp->minimum;
			ScrollCallback (sbw, XmCR_TO_TOP, sbp->value,
				buttonEvent->x, buttonEvent->y, buttonEvent);
		}
		else if /* in the trough between arrow2 and the slider... */
			(((sbp->orientation == XmHORIZONTAL)                     &&
				(buttonEvent->x > sbp->slider_x + sbp->slider_width) &&
				(buttonEvent->x <= sbp->slider_area_x
					+ sbp->slider_area_width)                        &&
				(buttonEvent->y >= sbp->slider_area_y)               &&
				(buttonEvent->y <= sbp->slider_area_y
					+ sbp->slider_area_height))
			||
				((sbp->orientation == XmVERTICAL)           &&
					(buttonEvent->y > sbp->slider_y 
						+ sbp->slider_height)               &&
					(buttonEvent->y <= sbp->slider_area_y
						+ sbp->slider_area_height)          &&
					(buttonEvent->x >= sbp->slider_area_x)  &&
					(buttonEvent->x <= sbp->slider_area_x 
						+ sbp->slider_area_width)))
		{
			if (sbp->orientation == XmVERTICAL)
				MoveSlider(sbw,
					sbp->slider_x, 
					sbp->slider_area_y + sbp->slider_area_height
						- sbp->slider_height);
			else
				MoveSlider(sbw,
					sbp->slider_area_x + sbp->slider_area_width
						- sbp->slider_width,
					sbp->slider_y);
			sbp->value = sbp->maximum - sbp->slider_size;
			ScrollCallback (sbw, XmCR_TO_BOTTOM, 
				sbp->value,
				buttonEvent->x, buttonEvent->y, buttonEvent);
		}
	}
}




/*********************************************************************
 *
 *  IncrementUpOrLeft
 *	The up or left key was pressed, decrease the value by 
 *	one increment.
 *
 *********************************************************************/

static void IncrementUpOrLeft (sbw, event, argv, argc)
XmScrollBarWidget sbw;
XButtonEvent * event;
char ** argv;
int argc;
{
	int new_value;
	int key_pressed;

	/*
	 * arg value passed in will either be 0 -> up key or 1 -> left key 
	 * the key needs to be compared with the scrollbar orientation to
	 * ensure only the proper directional key presses work.
	 */

	key_pressed = atoi(*argv);

	if (((key_pressed == 0) && 
		(sbw->scrollBar.orientation == XmHORIZONTAL)) 
		||
		((key_pressed == 1) && 
		(sbw->scrollBar.orientation == XmVERTICAL)))
		return;

	new_value = sbw->scrollBar.value - sbw->scrollBar.increment;

	if (new_value < sbw->scrollBar.minimum)
		new_value = sbw->scrollBar.minimum;

	if (new_value != sbw->scrollBar.value)
	{
		sbw->scrollBar.value = new_value;
		RedrawSlider (sbw);
		ScrollCallback (sbw, XmCR_DECREMENT, sbw->scrollBar.value,
			event->x, event->y, event);
	}
}




/*********************************************************************
 *
 *  IncrementDownOrRight
 *	The down or right key was pressed, increase the value by 
 *	one increment.
 *
 *********************************************************************/

static void IncrementDownOrRight (sbw, event, argv, argc)
XmScrollBarWidget sbw;
XButtonEvent * event;
char ** argv;
int argc;
{
	int new_value;
	int key_pressed;

	/* 
 	 * arg value passed in will either be 0 -> down key or 1 -> right 
	 * key the key needs to be compared with the scrollbar orientation 
	 * to ensure only the proper directional key presses work.
	 */

	key_pressed = atoi(*argv);

	if (((key_pressed == 0) && 
		(sbw->scrollBar.orientation == XmHORIZONTAL))
		||
		((key_pressed == 1) && 
		(sbw->scrollBar.orientation == XmVERTICAL)))
		return;

	new_value = sbw->scrollBar.value + sbw->scrollBar.increment;

	if (new_value > sbw->scrollBar.maximum - sbw->scrollBar.slider_size)
		new_value = sbw->scrollBar.maximum - sbw->scrollBar.slider_size;

	if (new_value != sbw->scrollBar.value)
	{
		sbw->scrollBar.value = new_value;
		RedrawSlider (sbw);
		ScrollCallback (sbw, XmCR_INCREMENT, sbw->scrollBar.value,
			event->x, event->y, event);
	}
}




/*********************************************************************
 *
 *  PageUpOrLeft
 *	The up or left key was pressed, decrease the value by 
 *	one increment.
 *
 *********************************************************************/

static void PageUpOrLeft (sbw, event, argv, argc)
XmScrollBarWidget sbw;
XButtonEvent * event;
char ** argv;
int argc;
{
	int new_value;
	int key_pressed;

	/*
	 * arg value passed in will either be 0 -> up key or 1 -> left key 
	 * the key needs to be compared with the scrollbar orientation to
	 * ensure only the proper directional key presses work.
	 */

	key_pressed = atoi(*argv);

	if (((key_pressed == 0) && 
		(sbw->scrollBar.orientation == XmHORIZONTAL)) 
		||
		((key_pressed == 1) && 
		(sbw->scrollBar.orientation == XmVERTICAL)))
		return;

	new_value = sbw->scrollBar.value - sbw->scrollBar.page_increment;

	if (new_value < sbw->scrollBar.minimum)
		new_value = sbw->scrollBar.minimum;

	if (new_value != sbw->scrollBar.value)
	{
		sbw->scrollBar.value = new_value;
		RedrawSlider (sbw);
		ScrollCallback (sbw, XmCR_PAGE_DECREMENT, sbw->scrollBar.value,
			event->x, event->y, event);
	}
}




/*********************************************************************
 *
 *  PageDownOrRight
 *	The down or right key was pressed, increase the value by 
 *	one increment.
 *
 *********************************************************************/

static void PageDownOrRight (sbw, event, argv, argc)
XmScrollBarWidget sbw;
XButtonEvent * event;
char ** argv;
int argc;
{
	int new_value;
	int key_pressed;

	/* 
 	 * arg value passed in will either be 0 -> down key or 1 -> right 
	 * key the key needs to be compared with the scrollbar orientation 
	 * to ensure only the proper directional key presses work.
	 */

	key_pressed = atoi(*argv);

	if (((key_pressed == 0) && 
		(sbw->scrollBar.orientation == XmHORIZONTAL))
		||
		((key_pressed == 1) && 
		(sbw->scrollBar.orientation == XmVERTICAL)))
		return;

	new_value = sbw->scrollBar.value + sbw->scrollBar.page_increment;

	if (new_value > sbw->scrollBar.maximum - sbw->scrollBar.slider_size)
		new_value = sbw->scrollBar.maximum - sbw->scrollBar.slider_size;

	if (new_value != sbw->scrollBar.value)
	{
		sbw->scrollBar.value = new_value;
		RedrawSlider (sbw);
		ScrollCallback (sbw, XmCR_PAGE_INCREMENT, sbw->scrollBar.value,
			event->x, event->y, event);
	}
}



/*********************************************************************
 *
 *  MoveSlider
 *	Given x and y positions, move the slider and clear the area
 *	moved out of.
 *
 *********************************************************************/

static void MoveSlider (sbw, currentX, currentY)
XmScrollBarWidget sbw;
int currentX, currentY;
{
	int oldX = sbw->scrollBar.slider_x;
	int oldY = sbw->scrollBar.slider_y;
	int width = sbw->scrollBar.slider_width;
	int height = sbw->scrollBar.slider_height;

	XSegment seg[2];


	if ((currentX == oldX) && (currentY == oldY))
		return;

	if (sbw->scrollBar.orientation == XmHORIZONTAL)
	{
		sbw->scrollBar.slider_x = currentX;

		seg[0].y1 = seg[0].y2 = oldY + 2;
		seg[1].y1 = seg[1].y2 = oldY + height - 3;

		if (oldX < currentX)
		{
			seg[0].x1 = seg[1].x1 = oldX;
			seg[0].x2 = seg[1].x2 = oldX + currentX - oldX - 1;
		}
		else
		{
			seg[0].x1 = seg[1].x1 = currentX + width;
			seg[0].x2 = seg[1].x2 = seg[0].x1 + oldX - currentX - 1;
		}


		if (sbw->scrollBar.pixmap != 0)
		{
			XCopyArea (XtDisplay((Widget) sbw),
				sbw->scrollBar.pixmap, XtWindow((Widget) sbw),
				sbw->scrollBar.foreground_GC,
				0, 0, width, height, currentX, currentY);

			XClearArea (XtDisplay((Widget)sbw), XtWindow((Widget)sbw),
				seg[0].x1, oldY, seg[0].x2 - seg[0].x1 + 1, 
				height, False);
		}
	} 
	else /* sbw->scrollBar.orientation == XmVERTICAL */
	{
		sbw->scrollBar.slider_y = currentY;

		seg[0].x1 = seg[0].x2 = oldX + 2;
		seg[1].x1 = seg[1].x2 = oldX + width - 3;

		if (oldY < currentY)
		{
			seg[0].y1 = seg[1].y1 = oldY;
			seg[0].y2 = seg[1].y2 = oldY + currentY - oldY - 1;
		}
		else
		{
			seg[0].y1 = seg[1].y1 = currentY + height;
			seg[0].y2 = seg[1].y2 = seg[0].y1 + oldY - currentY - 1;
		}

		if (sbw->scrollBar.pixmap != 0)
		{
			XCopyArea (XtDisplay ((Widget) sbw),
				sbw->scrollBar.pixmap, XtWindow ((Widget) sbw),
				sbw->scrollBar.foreground_GC,
				0, 0, width, height, currentX, currentY);

			XClearArea (XtDisplay((Widget) sbw), XtWindow((Widget) sbw),
				oldX, seg[0].y1, width,
				seg[0].y2 - seg[0].y1 + 1, False);
		}
	}
}




/************************************************************************
 *
 *  RedrawSlider
 *	Clear, calculate, and redraw the slider.
 *
 ************************************************************************/

static void RedrawSlider (sbw)
XmScrollBarWidget sbw;

{
	if(XtIsRealized(sbw))
	{
		if(sbw->scrollBar.pixmap!=0)
		{
			XClearArea(XtDisplay((Widget)sbw),XtWindow((Widget)sbw),
				sbw->scrollBar.slider_x,sbw->scrollBar.slider_y,
				sbw->scrollBar.slider_width,
				sbw->scrollBar.slider_height,False);
		}
	}

	CalcSliderRect(sbw,&(sbw->scrollBar.slider_x),
		&(sbw->scrollBar.slider_y), &(sbw->scrollBar.slider_width),
		&(sbw->scrollBar.slider_height));

	if(XtIsRealized(sbw))
	{
		if(sbw->scrollBar.pixmap!=0)
			XCopyArea(XtDisplay((Widget)sbw),
				sbw->scrollBar.pixmap, XtWindow((Widget)sbw),
				sbw->scrollBar.foreground_GC,
				0, 0, sbw->scrollBar.slider_width,
				sbw->scrollBar.slider_height,
				sbw->scrollBar.slider_x, sbw->scrollBar.slider_y);
	}
}




/************************************************************************
 *
 *  ChangeScrollBarValue
 *	Change the scrollbar value by the indicated change type.  Return
 *	True if the value changes, False otherwise.
 *
 ************************************************************************/

static Boolean ChangeScrollBarValue (sbw)
XmScrollBarWidget sbw;

{
   unsigned char change_type = sbw->scrollBar.change_type;
   int change_amount;
   int temp_value;


   /*  Get the amount to change the scroll bar value based on  */
   /*  the type of change occuring.                            */

   if (change_type == XmCR_INCREMENT)
      change_amount = sbw->scrollBar.increment;
   else if (change_type == XmCR_PAGE_INCREMENT)
      change_amount = sbw->scrollBar.page_increment;
   else if (change_type == XmCR_DECREMENT)
      change_amount = -sbw->scrollBar.increment;
   else if (change_type == XmCR_PAGE_DECREMENT)
      change_amount = -sbw->scrollBar.page_increment;


   /*  temp_value contains the possible new scroll bar value  */
   /*  Depending on the change type, adjust this new value    */
   /*  if it extends beyound the bounds of the value range.   */

   temp_value = sbw->scrollBar.value + change_amount;

   if (change_type == XmCR_INCREMENT || change_type == XmCR_PAGE_INCREMENT)
   {
      if (temp_value > sbw->scrollBar.maximum - sbw->scrollBar.slider_size)
         temp_value =  sbw->scrollBar.maximum - sbw->scrollBar.slider_size;

      if (temp_value <= sbw->scrollBar.value)
         return (False);
   }
   else
   {
      if (temp_value < sbw->scrollBar.minimum)
         temp_value = sbw->scrollBar.minimum;

      if (temp_value >= sbw->scrollBar.value)
         return (False);
   }


   sbw->scrollBar.value = temp_value;
   RedrawSlider (sbw);

   return (True);
}




/*********************************************************************
 *
 *  TimerEvent
 *	This is an event processing function which handles timer
 *	event evoked because of arrow selection.
 *
 *  WARNING: The record pointed to by rec will be freed in this
 *           procedure.  Callers of this procedure should consider
 *           themselves warned.
 *
 *********************************************************************/

static void TimerEvent (rec, id)
XmScrollBarTimerRec *rec;
XtIntervalId * id;
{
	XmScrollBarWidget sbw = rec->sbw;
	Boolean flag;


	sbw->scrollBar.timer = 0;

  if (sbw->scrollBar.flags & FIRST_SCROLL_FLAG)
	{
		XSync (XtDisplay (sbw), False);

    sbw->scrollBar.flags &= ~FIRST_SCROLL_FLAG;

		sbw->scrollBar.timer = 
		XtAppAddTimeOut (XtWidgetToApplicationContext(sbw),
			(unsigned long) sbw->scrollBar.repeat_delay,
			(XtTimerCallbackProc)  TimerEvent, (caddr_t) rec);
		return;
	}


	/*  Change the scrollbar slider value  */

	flag = ChangeScrollBarValue (sbw);

	/*  If the orgin was changed invoke the application supplied  */
	/*  slider moved callbacks                                    */

	if (flag)
		ScrollCallback (sbw, sbw->scrollBar.change_type, 
			sbw->scrollBar.value, 0, 0, NULL);

	/*
	 * If the callback does alot of processing, and XSync is needed
	 * to flush the output and input buffers.  If this is not done,
	 * the entry back to MainLoop will cause the flush.  The server
	 * will then perform it work which may take longer than the timer
	 * interval which will cause the scrollbar to be stuck in a loop.
	 */

	XSync (XtDisplay (sbw), False);

	/*  Add the repeat timer */

	if (flag)
	{
		sbw->scrollBar.timer = 
			XtAppAddTimeOut (XtWidgetToApplicationContext(sbw),
				(unsigned long) sbw->scrollBar.repeat_delay,
				(XtTimerCallbackProc)  TimerEvent, (caddr_t) rec);
	}
	else
	{
		/* Free the Timer Rec */

		XtFree(rec);
	}
}




/************************************************************************
 *
 *  ScrollCallback
 *	This routine services the widget's callbacks.  It calls the
 *	specific callback if it is not empty.  If it is empty then the 
 *	main callback is called.
 *
 ************************************************************************/

static void ScrollCallback (sbw, reason, value, xpixel, ypixel, event)
XmScrollBarWidget sbw;
int    reason;
int    value;
int    xpixel;
int    ypixel;
XEvent *event;

{
   XmScrollBarCallbackStruct call_value;

   call_value.reason = reason;
   call_value.event  = event;
    
   if (sbw->scrollBar.processing_direction == XmMAX_ON_LEFT ||
       sbw->scrollBar.processing_direction == XmMAX_ON_TOP)
	{
		switch (reason)
		{
			case XmCR_INCREMENT:
				call_value.reason = reason = XmCR_DECREMENT;
			break;
			case XmCR_DECREMENT:
				call_value.reason = reason = XmCR_INCREMENT;
			break;
			case XmCR_PAGE_INCREMENT:
				call_value.reason = reason = XmCR_PAGE_DECREMENT;
			break;
			case XmCR_PAGE_DECREMENT:
				call_value.reason = reason = XmCR_PAGE_INCREMENT;
			break;
			case XmCR_TO_TOP:
				call_value.reason = reason = XmCR_TO_BOTTOM;
			break;
			case XmCR_TO_BOTTOM:
				call_value.reason = reason = XmCR_TO_TOP;
			break;
		}
      call_value.value = sbw->scrollBar.maximum
		+ sbw->scrollBar.minimum - value - sbw->scrollBar.slider_size;
	}
   else
      call_value.value = value;


   if (sbw->scrollBar.orientation == XmHORIZONTAL)
      call_value.pixel = xpixel;
   else
      call_value.pixel = ypixel;

   switch (reason)
   {

      case XmCR_VALUE_CHANGED:
         XtCallCallbackList ((Widget) sbw, sbw->scrollBar.value_changed_callback,
                                &call_value);
      break;

      case XmCR_INCREMENT:
         if  (sbw->scrollBar.increment_callback)
            XtCallCallbackList ((Widget) sbw, sbw->scrollBar.increment_callback, &call_value);
         else
         {
            call_value.reason = XmCR_VALUE_CHANGED;
            XtCallCallbackList ((Widget) sbw, sbw->scrollBar.value_changed_callback, &call_value);
         }
      break;

      case XmCR_DECREMENT:
         if  (sbw->scrollBar.decrement_callback)
            XtCallCallbackList ((Widget) sbw, sbw->scrollBar.decrement_callback, &call_value);
         else
         {
            call_value.reason = XmCR_VALUE_CHANGED;
            XtCallCallbackList ((Widget) sbw, sbw->scrollBar.value_changed_callback, &call_value);
         }
      break;

      case XmCR_PAGE_INCREMENT:
         if  (sbw->scrollBar.page_increment_callback)
            XtCallCallbackList ((Widget) sbw, sbw->scrollBar.page_increment_callback, &call_value);
         else 
         {
            call_value.reason = XmCR_VALUE_CHANGED;
            XtCallCallbackList ((Widget) sbw, sbw->scrollBar.value_changed_callback, &call_value);
         }
      break;

      case XmCR_PAGE_DECREMENT:
         if  (sbw->scrollBar.page_decrement_callback)
            XtCallCallbackList ((Widget) sbw, sbw->scrollBar.page_decrement_callback, &call_value);
         else
         {
            call_value.reason = XmCR_VALUE_CHANGED;
            XtCallCallbackList ((Widget) sbw, sbw->scrollBar.value_changed_callback, &call_value);
         }
      break;

      case XmCR_TO_TOP:
         if (sbw->scrollBar.to_top_callback)
            XtCallCallbackList ((Widget) sbw, sbw->scrollBar.to_top_callback, &call_value);
         else 
         {
            call_value.reason = XmCR_VALUE_CHANGED;
            XtCallCallbackList ((Widget) sbw, sbw->scrollBar.value_changed_callback, &call_value);
         }
      break;

      case XmCR_TO_BOTTOM:
         if (sbw->scrollBar.to_bottom_callback)
            XtCallCallbackList ((Widget) sbw, sbw->scrollBar.to_bottom_callback, &call_value);
         else
         {
            call_value.reason = XmCR_VALUE_CHANGED;
            XtCallCallbackList ((Widget) sbw, sbw->scrollBar.value_changed_callback, &call_value);
         }
      break;

      case XmCR_DRAG:
         if (sbw->scrollBar.drag_callback)
            XtCallCallbackList ((Widget) sbw, sbw->scrollBar.drag_callback, &call_value);
      break;

   }
}




/*********************************************************************
 *
 *  ExportScrollBarValue
 *	Convert the scrollbar value from the normal processing direction
 *	to reverse processing if needed.
 *
 *********************************************************************/
static void ExportScrollBarValue (sbw, offset, value)
XmScrollBarWidget sbw;
int offset;
XtArgVal * value;
{
	if ((sbw->scrollBar.processing_direction == XmMAX_ON_LEFT) ||
		(sbw->scrollBar.processing_direction == XmMAX_ON_TOP))
		*value = (XtArgVal)sbw->scrollBar.maximum + sbw->scrollBar.minimum
			- sbw->scrollBar.value - sbw->scrollBar.slider_size;
	else
		*value = (XtArgVal)sbw->scrollBar.value;
}



static XmImportOperator ImportScrollBarValue (sbw, offset, value)
XmScrollBarWidget sbw;
int offset;
XtArgVal * value;
{
      sbw->scrollBar.flags |= VALUE_SET_FLAG;
      *value = (XtArgVal)sbw->scrollBar.value;
      return(XmSYNTHETIC_LOAD);
}



/************************************************************************
 *
 *  _XmSetEtchedSlider
 *	Set the scrollbar variable which causes the slider pixmap
 *	to be etched.
 *
 ************************************************************************/

#ifdef _NO_PROTO
void _XmSetEtchedSlider (sbw)
XmScrollBarWidget sbw;

#else /* _NO_PROTO */
void _XmSetEtchedSlider (XmScrollBarWidget sbw)
#endif /* _NO_PROTO */
{
   sbw->scrollBar.etched_slider = True;
}




/************************************************************************
 *
 *		Application Accessible External Functions
 *
 ************************************************************************/


/************************************************************************
 *
 *  XmCreateScrollBar
 *	Create an instance of a scrollbar and return the widget id.
 *
 ************************************************************************/

#ifdef _NO_PROTO
Widget XmCreateScrollBar (parent, name, arglist, argcount)
Widget parent;
char * name;
ArgList arglist;
Cardinal argcount;

#else /* _NO_PROTO */
Widget XmCreateScrollBar (Widget parent, char *name, ArgList arglist, Cardinal argcount)
#endif /* _NO_PROTO */
{
   return (XtCreateWidget (name, xmScrollBarWidgetClass, 
                           parent, arglist, argcount));
}




/************************************************************************
 *
 *  XmScrollBarGetValues
 *	Return some scrollbar values.
 *
 ************************************************************************/

#ifdef _NO_PROTO
void XmScrollBarGetValues (w, value, slider_size, increment, page_increment)
Widget w;
int * value;
int * slider_size;
int * increment;
int * page_increment;

#else /* _NO_PROTO */
void XmScrollBarGetValues (Widget w, int *value, int *slider_size, int *increment, int *page_increment)
#endif /* _NO_PROTO */
{
   XmScrollBarWidget sbw = (XmScrollBarWidget) w;

   if (sbw->scrollBar.processing_direction == XmMAX_ON_LEFT ||
       sbw->scrollBar.processing_direction == XmMAX_ON_TOP)
      *value = sbw->scrollBar.maximum  + sbw->scrollBar.minimum 
	  	- sbw->scrollBar.value - sbw->scrollBar.slider_size;
   else
      *value = sbw->scrollBar.value;

   *slider_size = sbw->scrollBar.slider_size;
   *increment = sbw->scrollBar.increment;
   *page_increment = sbw->scrollBar.page_increment;
}




/************************************************************************
 *
 *  XmScrollBarSetValues
 *	Set some scrollbar values.
 *
 ************************************************************************/

#ifdef _NO_PROTO
void XmScrollBarSetValues (w, value, slider_size, 
                           increment, page_increment, notify)
Widget w;
int value;
int slider_size;
int increment;
int page_increment;
Boolean notify;

#else /* _NO_PROTO */
void XmScrollBarSetValues (Widget w, int value, int slider_size, int increment, int page_increment, 
#if NeedWidePrototypes
int notify
#else
Boolean notify
#endif 
)
#endif /* _NO_PROTO */
{
   XmScrollBarWidget sbw = (XmScrollBarWidget) w;
   int save_value;
   Arg arglist[4];
   int n;


   save_value = sbw->scrollBar.value;

   n = 0;
   XtSetArg (arglist[n], XmNvalue, value);			n++;
   XtSetArg (arglist[n], XmNsliderSize, slider_size);		n++;
   if (increment != 0)
   {
      XtSetArg (arglist[n], XmNincrement, increment);		n++;
   }
   if (page_increment != 0)
   {
      XtSetArg (arglist[n], XmNpageIncrement, page_increment);	n++;
   }
   XtSetValues (sbw, arglist, n);

   if (notify && sbw->scrollBar.value != save_value)
      ScrollCallback (sbw, XmCR_VALUE_CHANGED, 
                      sbw->scrollBar.value, 0, 0, NULL); 
}


