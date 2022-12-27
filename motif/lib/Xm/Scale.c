#ifdef REV_INFO
#ifndef lint
static char SCCSID[] = "OSF/Motif: @(#)Scale.c	3.36.1.4 91/04/01";
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

#include <X11/Intrinsic.h>
#include <X11/IntrinsicP.h>
#include <Xm/ScaleP.h>
#include <Xm/ScrollBarP.h>
#include <Xm/LabelG.h>


#define MESSAGE1 "The scale minumum value is greater than or equal to the scale maximum value."
#define MESSAGE2 "The specified scale value is less than the minimum scale value."
#define MESSAGE3 "The specified scale value is greater than the maximum scale value."
#define MESSAGE4 "Incorrect orientation."
#define MESSAGE5 "Incorrect processing direction."
#define MESSAGE6 "Invalid highlight thickness."
#define MESSAGE7 "Invalid scaleMultiple; greater than (max - min)"
#define MESSAGE8 "Invalid scaleMultiple; less than zero"


/* Convenience macros and definitions */

#define TotalWidth(w)   (w->core.width + (w->core.border_width * 2))
#define TotalHeight(w)  (w->core.height + (w->core.border_width * 2))

#define BIGSIZE (Dimension) -1
#define SCROLLBAR_MAX	1000000000
#define SCROLLBAR_MIN	0
#define SLIDER_SIZE	30
#define SCALE_VALUE_MARGIN 3
#define SCALE_DEFAULT_MAJOR_SIZE \
	(100 + (2 * sw->scale.highlight_thickness))
#define SCALE_DEFAULT_MINOR_SIZE \
	(15 + (2 * sw->scale.highlight_thickness))


/*  Core class functions  */

static void    ClassPartInitialize();
static void    Initialize();
static void    Redisplay();
static void    Resize();
static Boolean SetValues();
static void    SetValuesAlmost();
static void    Destroy();
static void    Realize();


/*  Composite class functions  */

static void ChangeManaged();
static XtGeometryResult GeometryManager();


/*  Internal functions  */

static void ProcessingDirectionDefault();
static void GetScaleMultiple();
static void LayoutScale();
static void ValueChanged();
static void ShowValue();
static void SliderReleased();
static void GetForegroundGC();
static void CaclcScrollBarData();
static void GetScaleSize ();
static void CalcScrollBarData();
static Dimension ScrollWidth();
static Dimension ScrollHeight();


/* Event procedures referenced in the actions list */

static void Arm();
static void Activate();


/* Default translation table and action list */

/*  Resource definitions for Scale class */

static XtResource resources[] =
{
	{
     XmNshadowThickness, XmCShadowThickness, XmRHorizontalDimension, 
	 sizeof (Dimension),
     XtOffset (XmManagerWidget, manager.shadow_thickness),
     XmRImmediate, (caddr_t) 2
     },
	 {
		XmNwidth, XmCWidth, XmRHorizontalDimension,
		sizeof(Dimension), XtOffset(XmScaleWidget, core.width),
		XmRImmediate, (caddr_t) BIGSIZE
	},
	{
		XmNheight, XmCHeight, XmRVerticalDimension,
		sizeof(Dimension), XtOffset(XmScaleWidget, core.height),
		XmRImmediate, (caddr_t) BIGSIZE
	},
   {
     XmNvalue, XmCValue, XmRInt, sizeof(int),
     XtOffset(XmScaleWidget,scale.value), XmRImmediate, 
     (caddr_t) XmINVALID_DIMENSION     /* what else? */
   },

   {
     XmNmaximum, XmCMaximum, XmRInt, sizeof(int),
     XtOffset(XmScaleWidget,scale.maximum), XmRImmediate, (caddr_t)100
   },

   {
     XmNminimum, XmCMinimum, XmRInt, sizeof(int),
     XtOffset(XmScaleWidget,scale.minimum), XmRImmediate, (caddr_t)0
   },

   {
     XmNorientation, XmCOrientation, XmROrientation, sizeof(unsigned char),
     XtOffset(XmScaleWidget,scale.orientation), 
     XmRImmediate, (caddr_t) XmVERTICAL
   },

   {
     XmNprocessingDirection, XmCProcessingDirection, XmRProcessingDirection,
     sizeof(unsigned char),
     XtOffset(XmScaleWidget,scale.processing_direction), XmRCallProc, 
     (caddr_t) ProcessingDirectionDefault
   },

   {
     XmNtitleString, 
     XmCTitleString, 
     XmRXmString, 
     sizeof(XmString),
     XtOffset(XmScaleWidget,scale.title), 
     XmRImmediate, 
     NULL
   },

   {
     XmNfontList, XmCFontList, XmRFontList, sizeof(XmFontList),
     XtOffset(XmScaleWidget, scale.font_list), XmRImmediate, NULL
   },

   {
     XmNshowValue, XmCShowValue, XmRBoolean, sizeof(Boolean),
     XtOffset(XmScaleWidget,scale.show_value), XmRImmediate, (caddr_t) False
   },
         
   {
     XmNdecimalPoints, XmCDecimalPoints, XmRShort, sizeof(short),
     XtOffset(XmScaleWidget,scale.decimal_points), XmRImmediate, (caddr_t) 0
   },

   {
     XmNscaleWidth, XmCScaleWidth, XmRHorizontalDimension,
	 sizeof (Dimension),
     XtOffset (XmScaleWidget, scale.scale_width),
	 XmRImmediate, (caddr_t) 0
   },

   {
     XmNscaleHeight, XmCScaleHeight, XmRVerticalDimension,
	 sizeof (Dimension),
     XtOffset (XmScaleWidget, scale.scale_height),
	 XmRImmediate, (caddr_t) 0
   },

   {
     XmNhighlightThickness, XmCHighlightThickness,
	 XmRHorizontalDimension, sizeof (Dimension),
     XtOffset (XmScaleWidget, scale.highlight_thickness),
     XmRImmediate, (caddr_t) 2
   },

   {
     XmNhighlightOnEnter, XmCHighlightOnEnter, XmRBoolean, sizeof (Boolean),
     XtOffset (XmScaleWidget, scale.highlight_on_enter),
     XmRImmediate, (caddr_t) False
   },


   {
     XmNvalueChangedCallback, XmCCallback, XmRCallback, sizeof(XtCallbackList),
     XtOffset(XmScaleWidget,scale.value_changed_callback), 
     XmRCallback, NULL
   },

   { 
     XmNdragCallback, XmCCallback, XmRCallback, sizeof(XtCallbackList),
     XtOffset(XmScaleWidget,scale.drag_callback), XmRCallback, NULL
   },
   {
	XmNscaleMultiple, XmCScaleMultiple, XmRInt, sizeof(int),
	XtOffset(XmScaleWidget,scale.scale_multiple), 
	XmRImmediate, (caddr_t) 0
   },
};


/*  Definition for resources that need special processing in get values  */

static XmSyntheticResource syn_resources[] =
{
  {     XmNtitleString, 
	sizeof(XmString),
	XtOffset(XmScaleWidget,scale.title), 
	_XmScaleGetTitleString,
	NULL
   },
	{ XmNscaleWidth,
	  sizeof (Dimension),
	  XtOffset (XmScaleWidget, scale.scale_width), 
	  _XmFromHorizontalPixels,
	  _XmToHorizontalPixels 
	},
	{ XmNscaleHeight,
	  sizeof (Dimension),
	  XtOffset (XmScaleWidget, scale.scale_height), 
	  _XmFromVerticalPixels,
	  _XmToVerticalPixels
	},
	{ XmNscaleMultiple,
	  sizeof(int),
	  XtOffset (XmScaleWidget, scale.scale_multiple), 
	  GetScaleMultiple,
	  NULL
	}
};


/*  Scale class record definition  */

externaldef(xmscaleclassrec) XmScaleClassRec xmScaleClassRec = 
{
   {                                            /* core_class fields    */
      (WidgetClass) &xmManagerClassRec,         /* superclass         */
      "XmScale",                                /* class_name         */
      sizeof(XmScaleRec),                       /* widget_size        */
      NULL,                          /* class_initialize   */
      ClassPartInitialize,                      /* class_part_init    */
      FALSE,                                    /* class_inited       */
      (XtInitProc) Initialize,                  /* initialize         */
      NULL,                                     /* initialize_hook    */
      Realize,                                  /* realize            */
      NULL,                                     /* actions            */
      0,                                        /* num_actions        */
      resources,                                /* resources          */
      XtNumber(resources),                      /* num_resources      */
      NULLQUARK,                                /* xrm_class          */
      TRUE,                                     /* compress_motion    */
      XtExposeCompressMaximal,                  /* compress_exposure  */
      TRUE,                                     /* compress_enterlv   */
      FALSE,                                    /* visible_interest   */
      Destroy,                                  /* destroy            */
      Resize,                                   /* resize             */
      Redisplay,                                /* expose             */
      (XtSetValuesFunc) SetValues,              /* set_values         */
      NULL,                                     /* set_values_hook    */
      (XtAlmostProc) SetValuesAlmost,           /* set_values_almost  */
      NULL,                                     /* get_values_hook    */
      NULL,                                     /* accept_focus       */
      XtVersion,                                /* version            */
      NULL,                                     /* callback_private   */
      XtInheritTranslations,                    /* tm_table           */
      NULL,                                     /* query_geometry     */
      NULL,                                     /* display_accelerator*/
      NULL,                                     /* extension          */
   },

   {                                            /* composite_class fields */
      (XtGeometryHandler) GeometryManager,      /* geometry_manager   */
      ChangeManaged,                            /* change_managed     */
      XtInheritInsertChild,                     /* insert_child       */
      XtInheritDeleteChild,                     /* delete_child       */
      NULL,                                     /* extension          */
   },

   {                                            /* constraint_class fields */
      NULL,                                     /* resource list        */   
      0,                                        /* num resources        */   
      0,                                        /* constraint size      */   
      NULL,                                     /* init proc            */   
      NULL,                                     /* destroy proc         */   
      NULL,                                     /* set values proc      */   
      NULL,                                     /* extension            */
   },


   {		/* manager_class fields */
      XtInheritTranslations,			/* translations           */
      syn_resources,				/* syn_resources      	  */
      XtNumber(syn_resources),			/* num_syn_resources 	  */
      NULL,					/* syn_cont_resources     */
      0,					/* num_syn_cont_resources */
      XmInheritParentProcess,                   /* parent_process         */
      NULL,					/* extension           	  */
   },

   {                                            /* scale class - none */     
      0                                         /* mumble */
   }    
};

externaldef(xmscalewidgetclass) WidgetClass
	xmScaleWidgetClass = (WidgetClass)&xmScaleClassRec;



/************************************************************************
 *
 *  ClassPartInitialize
 *     Initialize the fast subclassing.
 *
 ************************************************************************/

static void ClassPartInitialize (wc)
WidgetClass wc;

{
   _XmFastSubclassInit (wc, XmSCALE_BIT);
}




/*********************************************************************
 *
 * ProcessingDirectionDefault
 *    This procedure provides the dynamic default behavior for
 *    the processing direction resource dependent on the orientation.
 *
 *********************************************************************/

static void ProcessingDirectionDefault (widget, offset, value)
XmScaleWidget   widget;
int      offset;
XrmValue *value;
{
	static unsigned char direction;

	value->addr = (caddr_t) &direction;

	if (widget->scale.orientation == XmHORIZONTAL)
		direction = XmMAX_ON_RIGHT;
	else /* XmVERTICAL  -- range checking done during widget
		                   initialization */
		direction = XmMAX_ON_TOP;
}



/*********************************************************************
 *
 *  GetScaleMultiple
 *      Provides the export mechanism for the synthetic resource
 *      XmNscaleMultiple.
 *********************************************************************/
static void GetScaleMultiple(sw, offset, value)
XmScaleWidget sw;
int offset;
XtArgVal *value;
{
	Arg args[1];
	int n;
	float sb_value, f;

	n = 0;
	XtSetArg(args[n], XmNpageIncrement, value); n++;
	XtGetValues(sw->composite.children[1],args,n);

	f = (float) (SCROLLBAR_MAX - sw->scale.slider_size - SCROLLBAR_MIN);
	if (f)	
		sb_value = (float) *value / f ;
	else 
		return;

	sb_value = (sb_value * 
			(float) (sw->scale.maximum - sw->scale.minimum));

	/* Set up the round off correctly */
	if (sb_value < 0.0)
		sb_value -= 0.5;
	else if (sb_value > 0.0)
		sb_value += 0.5;


	*value = (int) sb_value;
}



/*********************************************************************
 *  Initialize
 *      Validate all of the argument data for the widget, create the
 *	title label and scrollbar.
 *
 *********************************************************************/

static void ValidateInitialState(req, new)
XmScaleWidget req, new;
{
    Boolean default_value = False;

	if (new->scale.value == XmINVALID_DIMENSION) {
	    new->scale.value = 0 ;
	    default_value = True ;
	}

        if (new->scale.minimum >= new->scale.maximum)
	{
		new->scale.minimum = 0;
		new->scale.maximum = 100;
		_XmWarning (new, MESSAGE1);
	}

	if (new->scale.value < new->scale.minimum)
	{
		new->scale.value = new->scale.minimum;
		if (!default_value) _XmWarning (new, MESSAGE2);
	}

	if (new->scale.value > new->scale.maximum)
	{
		new->scale.value = new->scale.maximum;
		if (!default_value) _XmWarning (new, MESSAGE3);
	}

	if ((new->scale.orientation != XmHORIZONTAL) &&
		(new->scale.orientation != XmVERTICAL))
	{
		new->scale.orientation = XmVERTICAL;
		_XmWarning (new, MESSAGE4);
	}

	if (new->scale.orientation == XmHORIZONTAL)
	{
		if ((new->scale.processing_direction != XmMAX_ON_RIGHT) &&
			(new->scale.processing_direction != XmMAX_ON_LEFT))

		{
			new->scale.processing_direction = XmMAX_ON_RIGHT;
			_XmWarning (new, MESSAGE5);
		}
	}
	else
	{
		if ((new->scale.processing_direction != XmMAX_ON_TOP) &&
			(new->scale.processing_direction != XmMAX_ON_BOTTOM))
		{
			new->scale.processing_direction = XmMAX_ON_TOP;
			_XmWarning (new, MESSAGE5);
		}
	}

	if (new->scale.highlight_thickness < 0)
	{
		_XmWarning (new, MESSAGE6);
		new->scale.highlight_thickness = 0;
	}

	if (new->scale.scale_multiple > (new->scale.maximum 
		- new->scale.minimum))
	{
		_XmWarning(new, MESSAGE7);
		new->scale.scale_multiple = (new->scale.maximum
			- new->scale.minimum) / 10;
	}
	else if (new->scale.scale_multiple < 0)
	{
		_XmWarning(new, MESSAGE8);
		new->scale.scale_multiple = (new->scale.maximum
			- new->scale.minimum) / 10;
	}
	else if (new->scale.scale_multiple == 0)
		new->scale.scale_multiple = (new->scale.maximum
			- new->scale.minimum) / 10;
}

static Widget CreateScaleTitle(new)
XmScaleWidget new;
{
	Widget title;
	Arg args[5];
	int n;

	/*  Create the title label gadget  */

	n = 0;
	XtSetArg (args[n], XmNlabelString, new->scale.title);	n++;
	XtSetArg (args[n], XmNfontList, new->scale.font_list);	n++;

	title = XmCreateLabelGadget ((Widget)new, "scale_title", args, n);

	if (new->scale.title)
		XtManageChild (title);
}

static Widget CreateScaleScrollBar(new)
XmScaleWidget new;
{
	Widget scrollbar;
	Arg args[25];
	int n;

	/*  Build up an arg list for and create the scrollbar  */

	n = 0;
	XtSetArg (args[n], XmNmaximum, SCROLLBAR_MAX);	n++;
	XtSetArg (args[n], XmNminimum, SCROLLBAR_MIN);	n++;
	XtSetArg (args[n], XmNshowArrows, False);		n++;
	XtSetArg (args[n], XmNorientation, new->scale.orientation);	n++;
	XtSetArg (args[n], XmNprocessingDirection,
		new->scale.processing_direction);       n++;
	XtSetArg (args[n], XmNhighlightColor, 
		new->manager.highlight_color);		n++;
	XtSetArg (args[n], XmNhighlightThickness, 
		new->scale.highlight_thickness);		n++;
	XtSetArg (args[n], XmNhighlightOnEnter, 
		new->scale.highlight_on_enter);			n++;
	XtSetArg (args[n], XmNtraversalOn, 
		new->manager.traversal_on);				n++;
	XtSetArg (args[n], XmNshadowThickness, 
		new->manager.shadow_thickness);			n++;
	XtSetArg (args[n], XmNbackground, new->core.background_pixel);	n++;
	XtSetArg (args[n], XmNbackgroundPixmap, 
		new->core.background_pixmap);	n++;
	XtSetArg (args[n], XmNtopShadowColor,
		new->manager.top_shadow_color); n++;
	XtSetArg (args[n], XmNtopShadowPixmap, 
		new->manager.top_shadow_pixmap);		n++;
	XtSetArg (args[n], XmNbottomShadowColor, 
		new->manager.bottom_shadow_color);		n++;
	XtSetArg (args[n], XmNbottomShadowPixmap,
		new->manager.bottom_shadow_pixmap);		n++;
	XtSetArg (args[n], XmNunitType, XmPIXELS);	n++;
	if (new->scale.scale_width != 0)
	{
		XtSetArg (args[n], XmNwidth, new->scale.scale_width);	n++;
	}
	if (new->scale.scale_height != 0)
	{
		XtSetArg (args[n], XmNheight, new->scale.scale_height);	n++;
	}

	scrollbar = XmCreateScrollBar ((Widget)new, "scale_scrollbar", args, n);
	_XmSetEtchedSlider ((XmScrollBarWidget)scrollbar);
	XtManageChild (scrollbar);

	XtAddCallback(scrollbar, XmNvalueChangedCallback,
		ValueChanged, NULL);
	XtAddCallback(scrollbar, XmNdragCallback,
		ValueChanged, NULL);
	XtAddCallback(scrollbar, XmNtoTopCallback,
		ValueChanged,   NULL);
	XtAddCallback(scrollbar, XmNtoBottomCallback,
		ValueChanged,   NULL);

	return(scrollbar);
}

static void Initialize (req, new)
XmScaleWidget req, new;
{
	Widget title;
	Widget scrollbar;
	int real_multiple;
	float tmp;

	/* Validate the incoming data  */                      
	ValidateInitialState(req, new);

	if (new->scale.font_list == NULL)
		new->scale.font_list =
			_XmGetDefaultFontList(new, XmLABEL_FONTLIST);

	/*  Set the scale font struct used for interactive value display  */
	/*  to the 0th font in the title font list.  If not font list is  */
	/*  provides, open up fixed and use that.                         */

	if (new->scale.font_list)
	{
             _XmFontListGetDefaultFont(new->scale.font_list,
                                       &new->scale.font_struct);
	}
	else
		new->scale.font_struct =
			XLoadQueryFont (XtDisplay (new), "fixed");

	title = CreateScaleTitle(new);
	scrollbar = CreateScaleScrollBar(new);

	/*  Get the foreground GC and initialize internal variables  */

	GetForegroundGC (new);

	new->scale.show_value_x = 0;
	new->scale.show_value_y = 0;
	new->scale.show_value_width = 0;
	new->scale.show_value_height = 0;
}




/************************************************************************
 *
 *  GetForegroundGC
 *     Get the graphics context used for drawing the slider value.
 *
 ************************************************************************/

static void GetForegroundGC (sw)
XmScaleWidget sw;

{
   XGCValues values;
   XtGCMask  valueMask;

   valueMask = GCForeground | GCBackground | GCFont;
   values.foreground = sw->manager.foreground;
   values.background = sw->core.background_pixel;
   values.font = sw->scale.font_struct->fid;

   sw->scale.foreground_GC = XtGetGC (sw, valueMask, &values);
}




/************************************************************************
 *
 *  Redisplay
 *     General redisplay function called on exposure events.
 *
 ************************************************************************/

static void Redisplay (sw, event, region)
XmScaleWidget sw;
XEvent *event;
Region region;

{
   _XmRedisplayGadgets ((CompositeWidget)sw, (XExposeEvent*)event, region);
   
   ShowValue (sw, sw->scale.value, sw->scale.show_value);
}




/************************************************************************
 *
 *  Resize
 *     Re-layout children.
 *
 ************************************************************************/

static void Resize (sw)
XmScaleWidget sw;

{
   LayoutScale (sw, FALSE);


   /*  If the scale is realized and the value is being displayed,   */
   /*  then ensure the value gets erased before being redisplayed.  */

   if (XtIsRealized (sw) && sw->scale.show_value)
   {
      sw->scale.last_value = sw->scale.value - 1;
      if (sw->scale.show_value)
         ShowValue (sw, sw->scale.value, True);
      else
         ShowValue (sw, sw->scale.value, False);
   }
}




/************************************************************************
 *
 *  SetValues
 *	Process any new data, and adjust the title and scrollbar.
 *
 ************************************************************************/
static XtGeometryResult GeometryManagerHack()
{
	return(XtGeometryYes);
}

static void ValidateInputs(cur, new)
XmScaleWidget cur, new;
{
   /* Validate the incoming data  */                      

   if (new->scale.minimum >= new->scale.maximum)
   {
      new->scale.minimum = cur->scale.minimum;
      new->scale.maximum = cur->scale.maximum;
      _XmWarning (new, MESSAGE1);
   }

   if (new->scale.value < new->scale.minimum)
   {
      new->scale.value = new->scale.minimum;
      _XmWarning (new, MESSAGE2);
   }

   if (new->scale.value > new->scale.maximum)
   {
      new->scale.value = new->scale.maximum;
      _XmWarning (new, MESSAGE3);
   }

   if (new->scale.orientation != XmHORIZONTAL &&
       new->scale.orientation != XmVERTICAL)
   {
      new->scale.orientation = cur->scale.orientation;
      _XmWarning (new, MESSAGE4);
   }

   if (new->scale.orientation == XmHORIZONTAL)
   {
      if (new->scale.processing_direction != XmMAX_ON_LEFT &&
          new->scale.processing_direction != XmMAX_ON_RIGHT)
      {
         new->scale.processing_direction = cur->scale.processing_direction;
         _XmWarning (new, MESSAGE5);
      }
   }
   else
   {
      if (new->scale.processing_direction != XmMAX_ON_TOP &&
          new->scale.processing_direction != XmMAX_ON_BOTTOM)
      {
         new->scale.processing_direction = cur->scale.processing_direction;
         _XmWarning (new, MESSAGE5);
      }
   }


   if (new->scale.highlight_thickness < 0)
   {
      _XmWarning (new, MESSAGE6);
      new->scale.highlight_thickness = cur->scale.highlight_thickness;
   }

	if (new->scale.scale_multiple != cur->scale.scale_multiple)
	{
		if (new->scale.scale_multiple > (new->scale.maximum 
			- new->scale.minimum))
		{
			_XmWarning(new, MESSAGE7);
			new->scale.scale_multiple = (new->scale.maximum
				- new->scale.minimum) / 10;
		}
		else if (new->scale.scale_multiple < 0)
		{
			_XmWarning(new, MESSAGE8);
			new->scale.scale_multiple = (new->scale.maximum
				- new->scale.minimum) / 10;
		}
		else if (new->scale.scale_multiple == 0)
			new->scale.scale_multiple = (new->scale.maximum
			- new->scale.minimum) / 10;
	}
}

static Boolean NeedNewSize(cur, new)
XmScaleWidget cur, new;
{
	Boolean flag = FALSE;
	XmScalePart *nsp, *csp;

	nsp = &(new->scale);
	csp = &(cur->scale);

	if ((nsp->font_list != csp->font_list) ||
		(nsp->highlight_thickness != csp->highlight_thickness) ||
		(nsp->scale_height != csp->scale_height) ||
		(nsp->scale_width != csp->scale_width) ||
		(nsp->show_value != csp->show_value) ||
		(new->manager.shadow_thickness != 
			cur->manager.shadow_thickness) ||
		(new->manager.unit_type != cur->manager.unit_type) ||
		(nsp->orientation != csp->orientation))
		flag = TRUE;

	return(flag);
}

static void HandleTitle(cur, req, new)
XmScaleWidget cur, req, new;
{
	Arg args[5];
	int n = 0;

	n = 0;
	if (new->scale.title != cur->scale.title)
	{
	    XtSetArg (args[n], XmNlabelString, new->scale.title);	n++;
	}

	if (new->scale.font_list != cur->scale.font_list)
	{
		XtSetArg (args[n], XmNfontList, new->scale.font_list);	n++;
	}

	/*
	* The intrin-sicks don't handle geometry management the way we 
	* would like --we don't know if was the parent's idea that the 
	* child changed-- so we have to do a little clever hacking.
	*
	* If a subclass thinks that they can do better, we'll let them.
	*/
	if (new->core.widget_class == xmScaleWidgetClass)
		xmScaleClassRec.composite_class.geometry_manager = 
			(XtGeometryHandler) GeometryManagerHack;
	XtSetValues (new->composite.children[0], args, n);
	if (new->core.widget_class == xmScaleWidgetClass)
		xmScaleClassRec.composite_class.geometry_manager = 
			(XtGeometryHandler) GeometryManager;

	if (new->scale.title != cur->scale.title)
	{
		if ((new->scale.title != NULL) &&
			(!XtIsManaged(new->composite.children[0])))
			XtManageChild(new->composite.children[0]);
		else if ((new->scale.title == NULL) &&
			(XtIsManaged(new->composite.children[0])))
			XtUnmanageChild (new->composite.children[0]);
	}
}

static void HandleScrollBar(cur, req, new)
XmScaleWidget cur, req, new;
{
	int value, increment, page, slider_size;
	Arg args[30];
	int n = 0;

	/* Build up an arg list to reset any attributes of the scrollbar */

	n = 0;
	XtSetArg (args[n], XmNsensitive, new->core.sensitive);	n++;
	XtSetArg (args[n], XmNorientation, new->scale.orientation);	n++;
	XtSetArg (args[n], XmNprocessingDirection, 
		new->scale.processing_direction);		n++;
	XtSetArg (args[n], XmNhighlightColor, 
		new->manager.highlight_color);		n++;
	XtSetArg (args[n], XmNhighlightThickness, 
		new->scale.highlight_thickness);		n++;
	XtSetArg (args[n], XmNshadowThickness, 
		new->manager.shadow_thickness);			n++;
	XtSetArg (args[n], XmNhighlightOnEnter, 
		new->scale.highlight_on_enter);			n++;
	XtSetArg (args[n], XmNtraversalOn, 
		new->manager.traversal_on);				n++;
	XtSetArg (args[n], XmNbackground, new->core.background_pixel);	n++;
	XtSetArg (args[n], XmNbackgroundPixmap,
		new->core.background_pixmap); 			n++;
	XtSetArg (args[n], XmNtopShadowColor,
		new->manager.top_shadow_color); 		n++;
	XtSetArg (args[n], XmNtopShadowPixmap, 
		new->manager.top_shadow_pixmap);		n++;
	XtSetArg (args[n], XmNbottomShadowColor, 
		new->manager.bottom_shadow_color);		n++;
	XtSetArg (args[n], XmNbottomShadowPixmap,
		new->manager.bottom_shadow_pixmap);		n++;
	if (new->scale.scale_width != cur->scale.scale_width)
	{
		XtSetArg (args[n], XmNwidth, new->scale.scale_width);	n++;
	}
	if (new->scale.scale_height != cur->scale.scale_height)
	{
		XtSetArg (args[n], XmNheight, new->scale.scale_height);	n++;
	}

	CalcScrollBarData(new, &value, &slider_size,
		&increment, &page);

	new->scale.slider_size = slider_size;

	XtSetArg (args[n], XmNvalue, value);		n++;
	XtSetArg (args[n], XmNsliderSize, new->scale.slider_size);	n++;
	XtSetArg (args[n], XmNincrement, increment);		n++;
	XtSetArg (args[n], XmNpageIncrement, page);		n++;

	/*
	* See HandleTitle.
	*/
	if (new->core.widget_class == xmScaleWidgetClass)
		xmScaleClassRec.composite_class.geometry_manager = 
			(XtGeometryHandler) GeometryManagerHack;
	XtSetValues (new->composite.children[1], args, n);
	if (new->core.widget_class == xmScaleWidgetClass)
		xmScaleClassRec.composite_class.geometry_manager = 
			(XtGeometryHandler) GeometryManager;
}


static Boolean SetValues (cur, req, new)
XmScaleWidget cur, req, new;
{
	Boolean redisplay = False;
	Arg args[30];
	int n, value, increment, page, slider_size;
	Dimension tmp_w, tmp_h;

	/* Make sure that processing direction tracks orientation */

	if ((new->scale.orientation != cur->scale.orientation)
		&&
		((new->scale.scale_width == cur->scale.scale_width) &&
		(new->scale.scale_height == cur->scale.scale_height)))
	{
		new->scale.scale_width = cur->scale.scale_height;
		new->scale.scale_height = cur->scale.scale_width;
	}

	if ((new->scale.orientation != cur->scale.orientation)
		&&
		((new->core.width == cur->core.width) &&
		(new->core.height == cur->core.height)))
	{
		new->core.width = cur->core.height;
		new->core.height = cur->core.width;
	}

	if ((new->scale.orientation != cur->scale.orientation)
		&&
		(new->scale.processing_direction ==
			cur->scale.processing_direction))
	{
		if ((new->scale.orientation == XmHORIZONTAL) &&
			(cur->scale.processing_direction == XmMAX_ON_TOP))
			new->scale.processing_direction = XmMAX_ON_LEFT;
		else if ((new->scale.orientation == XmHORIZONTAL) &&
			(cur->scale.processing_direction ==
				XmMAX_ON_BOTTOM))
			new->scale.processing_direction = XmMAX_ON_RIGHT;
		else if ((new->scale.orientation == XmVERTICAL) &&
			(cur->scale.processing_direction == XmMAX_ON_LEFT))
			new->scale.processing_direction = XmMAX_ON_TOP;
		else if ((new->scale.orientation == XmVERTICAL) &&
			(cur->scale.processing_direction == XmMAX_ON_RIGHT))
			new->scale.processing_direction = XmMAX_ON_BOTTOM;
	}


	ValidateInputs(cur, new);

	HandleTitle(cur, req, new);
	HandleScrollBar(cur, req, new);

	/*  Set the font struct for the value display  */

	if (new->scale.font_list != cur->scale.font_list)
	{

		if ((cur->scale.font_list == NULL) && 
			(cur->scale.font_struct != NULL))
			XFreeFont(XtDisplay (cur), cur->scale.font_struct);

		if (new->scale.font_list != NULL)
                        _XmFontListGetDefaultFont(new->scale.font_list,
                                           &new->scale.font_struct);
		else
			new->scale.font_struct =
				XLoadQueryFont(XtDisplay(new), "fixed");

		new->scale.last_value = new->scale.value - 1;
		XtReleaseGC (new, new->scale.foreground_GC);
		GetForegroundGC (new);
		redisplay = True;
	}

	if (NeedNewSize(cur, new))
	{
		/*
		 * Re-calculate the size of the Scale if a new size was not 
		 * specified.
		 */

		if (new->core.width == cur->core.width)
			new->core.width = BIGSIZE;

		if (new->core.height == cur->core.height)
			new->core.height = BIGSIZE;

		GetScaleSize (new, &(new->core.width), &(new->core.height));
	}

	if ((new->scale.decimal_points != cur->scale.decimal_points) ||
		(new->scale.value != cur->scale.value) ||
		(new->scale.minimum != cur->scale.minimum) ||
 		(new->scale.maximum != cur->scale.maximum) ||
	        (new->scale.processing_direction != 
			cur->scale.processing_direction) ||
		(new->scale.show_value != cur->scale.show_value))
	{
		if (new->scale.value != cur->scale.value)
			new->scale.last_value = new->scale.value - 1;
		ShowValue(new, new->scale.value, new->scale.show_value);
	}


	/*  See if the GC needs to be regenerated  */

	if ((new->manager.foreground != cur->manager.foreground) ||
		(new->core.background_pixel != cur->core.background_pixel))
	{
		XtReleaseGC (new, new->scale.foreground_GC);
		GetForegroundGC (new);
		redisplay = True;
	}

	return (redisplay);
}

static void SetValuesAlmost(old, new, request, reply)
XmScaleWidget old, new;
XtWidgetGeometry *request, *reply;
{
	if (!reply->request_mode) /* A No from our parent */
		LayoutScale(new, False);
	*request = *reply;
}


/************************************************************************
 *
 *  Realize
 *	Can't use the standard Manager class realize procedure,
 *      because it creates a window with NW gravity, and the
 *      scale needs a gravity of None.
 *
 ************************************************************************/

static void Realize (w, p_valueMask, attributes)
register Widget w;
Mask *p_valueMask;
XSetWindowAttributes *attributes;

{
   Mask valueMask = *p_valueMask;

   valueMask |= CWDontPropagate;
   attributes->do_not_propagate_mask =
      ButtonPressMask | ButtonReleaseMask |
      KeyPressMask | KeyReleaseMask | PointerMotionMask;
        
   XtCreateWindow (w, InputOutput, CopyFromParent, valueMask, attributes);
}



/************************************************************************
 *
 *  Destroy
 *	Free the callback lists attached to the scale.
 *
 ************************************************************************/

static void Destroy (sw)
XmScaleWidget sw;

{
   XtRemoveAllCallbacks (sw, XmNvalueChangedCallback);
   XtRemoveAllCallbacks (sw, XmNdragCallback);

   XtReleaseGC (sw, sw->scale.foreground_GC);

   if (sw->scale.font_list == NULL && sw->scale.font_struct != NULL)
      XFreeFont (XtDisplay (sw), sw->scale.font_struct);
}




/************************************************************************
 *
 *  GeometryManager
 *	Force the size of the child and return GeometryNo so that
 *	the child does no further processing.
 *
 ************************************************************************/

static XtGeometryResult GeometryManager (w, request, reply)
Widget w;
XtWidgetGeometry * request;
XtWidgetGeometry * reply;

{
   XmScaleWidget sw = (XmScaleWidget) w->core.parent;
   Dimension width;
   Dimension height;
   Dimension border_width;


   /*  Set the width, height, and border_width variables to the      */
   /*  requested values or to the widget's values if not requested.  */

   if (request->request_mode & CWWidth) 
      width = request->width;
   else 
      width = w->core.width;

   if (request->request_mode & CWHeight) 
      height = request->height;
   else 
      height = w->core.height;

   if (request->request_mode & CWBorderWidth) 
      border_width = request->border_width;
   else 
      border_width = w->core.border_width;


   /*  Resize the widget as is desired, layout the scale using the  */
   /*  new data, and return GeometryNo.                             */

   _XmResizeObject ((RectObj)w, width, height, border_width);

   LayoutScale (sw, True);

   return (XtGeometryNo);
}

static Dimension MaxLabelWidth(sw)
XmScaleWidget sw;
{
	register int i;
	register Widget c;
	register Dimension tmp;
	Dimension max = 0;

	for ( i = 2; i < sw->composite.num_children; i++)
	{
		c = sw->composite.children[i];
		tmp = XtWidth(c) + (2 * XtBorderWidth(c));
		if (tmp > max)
			max = tmp;
	}

	return (max);
}

static Dimension MaxLabelHeight(sw)
XmScaleWidget sw;
{
	register int i;
	register Widget c;
	register Dimension max = 0;
	register Dimension tmp;

	for ( i = 2; i < sw->composite.num_children; i++)
	{
		c = sw->composite.children[i];
		tmp = XtHeight(c) + (2 * XtBorderWidth(c));
		if (tmp > max)
			max = tmp;
	}

	return (max);
}

static Dimension ValueTroughWidth(sw)
XmScaleWidget sw;
{
	char buff[15];
	register Dimension tmp_max, tmp_min, max;
	int direction, ascent, descent;
	XCharStruct overall_return;

	if (sw->scale.show_value)
	{
		if (sw->scale.decimal_points)
			sprintf(buff, "%d.", sw->scale.maximum);
		else
			sprintf(buff, "%d", sw->scale.maximum);
		
		XTextExtents(sw->scale.font_struct, buff, strlen(buff),
			&direction, &ascent, &descent, &overall_return);
		
		tmp_max = overall_return.rbearing - overall_return.lbearing;
		
		if (sw->scale.decimal_points)
			sprintf(buff, "%d.", sw->scale.minimum);
		else
			sprintf(buff, "%d", sw->scale.minimum);
		
		XTextExtents(sw->scale.font_struct, buff, strlen(buff),
			&direction, &ascent, &descent, &overall_return);
		
		tmp_min = overall_return.rbearing - overall_return.lbearing;

		return(MAX(tmp_min, tmp_max));
	}
	else
		return ((Dimension) 0);
}

static Dimension ValueTroughHeight(sw)
XmScaleWidget sw;
{
	char buff[15];
	register Dimension tmp_max, tmp_min, max;
	int direction, ascent, descent;
	XCharStruct overall_return;

	if (sw->scale.show_value)
	{
		if (sw->scale.decimal_points)
			sprintf(buff, "%d.", sw->scale.maximum);
		else
			sprintf(buff, "%d", sw->scale.maximum);
		
		XTextExtents(sw->scale.font_struct, buff, strlen(buff),
			&direction, &ascent, &descent, &overall_return);
		
		tmp_max = ascent + descent;
		
		if (sw->scale.decimal_points)
			sprintf(buff, "%d.", sw->scale.minimum);
		else
			sprintf(buff, "%d", sw->scale.minimum);
		
		XTextExtents(sw->scale.font_struct, buff, strlen(buff),
			&direction, &ascent, &descent, &overall_return);
		
		tmp_min = ascent + descent;

		max = ((tmp_max > tmp_min) ? tmp_max : tmp_min);

		return(max);
	}
	else
		return((Dimension) 0);
}

static Dimension TitleWidth(sw)
XmScaleWidget sw;
{
	register Dimension tmp;
	register Widget title_widget = sw->composite.children[0];

	if (XtIsManaged(title_widget))
	{
		tmp = XtWidth(title_widget)
			+ (2 * XtBorderWidth(title_widget));

		if (sw->scale.orientation == XmVERTICAL)
			tmp += 0.25 * (XtHeight(title_widget)
				+ (2 * XtBorderWidth(title_widget)));
	}
	else
		tmp = 0;
	
	return(tmp);
}

static Dimension TitleHeight(sw)
XmScaleWidget sw;
{
	register Dimension tmp;
	register Widget title_widget = sw->composite.children[0];

	if (XtIsManaged(title_widget))
	{
		tmp = XtHeight(title_widget)
			+ (2 * XtBorderWidth(title_widget));

		if (sw->scale.orientation == XmHORIZONTAL)
			tmp += 0.25 * (XtHeight(title_widget)
				+ (2 * XtBorderWidth(title_widget)));
	}
	else
		tmp = 0;
	
	return(tmp);
}

static Dimension MajorLeadPad(sw)
XmScaleWidget sw;
{
	int diff = 0;
	XmScrollBarWidget sb = (XmScrollBarWidget)
		(sw->composite.children[1]);
	int tic = 0, tmp1 = 0, tmp2 = 0;

	if (sw->composite.num_children > 3)
	{
		tic = sb->primitive.highlight_thickness
			+ sb->primitive.shadow_thickness
			+ (Dimension) (((float) SLIDER_SIZE / 2.0) + 0.5);
		
		if (sw->scale.orientation == XmHORIZONTAL)
			diff = (TotalWidth(sw->composite.children[2]) / 2)
				- tic;
		else
			diff = (TotalHeight(sw->composite.children[2]) / 2)
				- tic;

		if (diff > 0)
			tmp1 += diff;
		
	}
	else if (sw->composite.num_children == 3)
	{
		/*
		 * This is a potential non-terminal recursion.
		 *
		 * Currently MajorScrollSize has knowledge of this potential
		 * problem and has guards around the call to this procedure.
		 * Modify with care.
		 */

		if (sw->scale.orientation == XmHORIZONTAL)
			diff = TotalWidth(sw->composite.children[2])
				- ScrollWidth(sw);
		else
			diff = TotalHeight(sw->composite.children[2])
				- ScrollHeight(sw);
		
		if (diff > 0)
			tmp1 = diff / 2;
		else
			tmp1 = 0;
	}
	tmp1 -= (sb->primitive.highlight_thickness
		+ sb->primitive.shadow_thickness);

	tmp2 = (int) (( ((int) ValueTroughWidth(sw)) - SLIDER_SIZE) / 2);
	tmp2 -= (sb->primitive.highlight_thickness
		+ sb->primitive.shadow_thickness);

	if (tmp1 < 0)
		tmp1 = 0;
	if (tmp2 < 0)
		tmp2 = 0;

	return(MAX(tmp1, tmp2));
}

static Dimension MajorTrailPad(sw)
XmScaleWidget sw;
{
	int diff = 0;
	XmScrollBarWidget sb = (XmScrollBarWidget)
		(sw->composite.children[1]);
	int tic, tmp1 = 0, tmp2 = 0;

	if (sw->composite.num_children > 3)
	{
		tic = sb->primitive.highlight_thickness
			+ sb->primitive.shadow_thickness
			+ (Dimension) (((float) SLIDER_SIZE / 2.0) + 0.5);
		
		if (sw->scale.orientation == XmHORIZONTAL)
			diff = (TotalWidth(sw->composite.children
						[sw->composite.num_children - 1]) / 2)
				- tic;
		else
			diff = (TotalHeight(sw->composite.children
						[sw->composite.num_children - 1]) / 2)
				- tic;

		if (diff > 0)
			tmp1 += diff;
	}
	else if (sw->composite.num_children == 3)
	{
		/*
		 * This is a potential non-terminal recursion.
		 *
		 * Currently MajorScrollSize has knowledge of this potential
		 * problem and has guards around the call to this procedure.
		 * Modify with care.
		 */

		if (sw->scale.orientation == XmHORIZONTAL)
			diff = TotalWidth(sw->composite.children[2])
				- ScrollWidth(sw);
		else
			diff = TotalHeight(sw->composite.children[2]) 
				- ScrollHeight(sw);
		
		if (diff > 0)
			tmp1 = diff / 2;
		else
			tmp1 = 0;
	}
	tmp1 -= (sb->primitive.highlight_thickness
		+ sb->primitive.shadow_thickness);

	tmp2 = (int) (( ((int) ValueTroughWidth(sw)) - SLIDER_SIZE) / 2);
	tmp2 -= (sb->primitive.highlight_thickness
		+ sb->primitive.shadow_thickness);

	if (tmp1 < 0)
		tmp1 = 0;
	if (tmp2 < 0)
		tmp2 = 0;

	return(MAX(tmp1, tmp2));
}

static Dimension ScrollWidth(sw)
XmScaleWidget sw;
{
	Dimension tmp;
	static Boolean in_process;

	if (sw->scale.orientation == XmVERTICAL)
	{
		if (!(tmp = sw->scale.scale_width))
			tmp = SCALE_DEFAULT_MINOR_SIZE;
		else
			tmp = sw->scale.scale_width;
	}
	else
	{
		if (!(tmp = sw->scale.scale_width))
		{
			if (sw->core.width != BIGSIZE)
			{
				/* Have to catch an indirect recursion here */
				if (sw->composite.num_children > 3)
					tmp = sw->core.width 
						- (MajorLeadPad(sw) + MajorTrailPad(sw));
				else
				{
					/* Magic to handle excessively wide values */
					int tmp2 = 0;
					XmScrollBarWidget sb = (XmScrollBarWidget)
						sw->composite.children[1];

					tmp2 = (int) (((int) ValueTroughWidth(sw))
						- SLIDER_SIZE) / 2;
					tmp2 -= (sb->primitive.highlight_thickness
						+ sb->primitive.shadow_thickness);
					if (tmp2 < 0)
						tmp2 = 0;

					tmp = sw->core.width - (2 * tmp2);
				}
			}
		}

		if (tmp <= 0)
		{
			if (sw->composite.num_children > 2)
			{
				/* Have to catch an indirect recursion here */
				if (sw->composite.num_children > 3)
				{
					Dimension tic = 0, diff = 0;
					XmScrollBarWidget sb = (XmScrollBarWidget)
						sw->composite.children[1];

					tmp = (sw->composite.num_children - 2)
						* MaxLabelWidth(sw);
					
					tic = sb->primitive.highlight_thickness
						+ sb->primitive.shadow_thickness
						+ (Dimension) (((float) SLIDER_SIZE / 2.0) 
							+ 0.5);

					diff = tic - (MaxLabelWidth(sw) / 2);

					if (diff > 0)
						tmp+= (2 * diff);
				}
				else
					tmp = MaxLabelWidth(sw);
			}
		}

		if (tmp <= 0)
			tmp = SCALE_DEFAULT_MAJOR_SIZE;
	}

	return(tmp);
}



static Dimension ScrollHeight(sw)
XmScaleWidget sw;
{
	Dimension tmp;
	static Boolean in_process;

	if (sw->scale.orientation == XmHORIZONTAL)
	{
		if (!(tmp = sw->scale.scale_height))
			tmp = SCALE_DEFAULT_MINOR_SIZE;
		else
			tmp = sw->scale.scale_height;
	}
	else
	{
		if (!(tmp = sw->scale.scale_height))
		{
			if (sw->core.height != BIGSIZE)
			{
				/* Have to catch an indirect recursion here */
				if (sw->composite.num_children > 3)
					tmp = sw->core.height 
						- (MajorLeadPad(sw) + MajorTrailPad(sw));
				else
					tmp = sw->core.height;
			}
			else
				tmp = 0;
		}

		if (tmp <= 0)
		{
			if (sw->composite.num_children > 2)
			{
				/* Have to catch an indirect recursion here */
				if (sw->composite.num_children > 3)
				{
					Dimension tic = 0, diff = 0;
					XmScrollBarWidget sb = (XmScrollBarWidget)
						sw->composite.children[1];

					tmp = (sw->composite.num_children - 2)
						* MaxLabelHeight(sw);
					
					tic = sb->primitive.highlight_thickness
						+ sb->primitive.shadow_thickness
						+ (Dimension) (((float) SLIDER_SIZE / 2.0) 
							+ 0.5);

					diff = tic - (MaxLabelHeight(sw) / 2);

					if (diff > 0)
						tmp+= (2 * diff);
				}
				else
					tmp = MaxLabelHeight(sw);
			}
		}

		if (tmp <= 0)
			tmp = SCALE_DEFAULT_MAJOR_SIZE;
	}

	return(tmp);
}


static void GetScaleSize(sw, w, h)
XmScaleWidget sw;
Dimension *w, *h;
{
	int acc = 0;

	if (sw->scale.orientation == XmHORIZONTAL)
	{
		if (w)
			*w = MAX(TitleWidth(sw),
				MajorLeadPad(sw) + ScrollWidth(sw) + MajorTrailPad(sw));

		if (h)
		{
			acc += MaxLabelHeight(sw);
			acc += ValueTroughHeight(sw);
			if (sw->scale.show_value)
				acc += SCALE_VALUE_MARGIN;
			acc += ScrollHeight(sw);
			acc += TitleHeight(sw);

			*h = acc;
		}
	}
	else /* sw->scale.orientation == XmVERTICAL */
	{
		if (w)
		{
			acc += MaxLabelWidth(sw);
			acc += ValueTroughWidth(sw);
			if (sw->scale.show_value)
				acc += SCALE_VALUE_MARGIN;
			acc += ScrollWidth(sw);
			acc += TitleWidth(sw);

			*w = acc;
		}

		if (h)
			*h = MAX(TitleHeight(sw),
				MajorLeadPad(sw) + ScrollHeight(sw) +
					MajorTrailPad(sw));
	}

	/* Don't ever desire 0 dimensions */
	if (!*w)
		*w = 1;

	if (!*h)
		*h = 1;
}

static void LayoutHorizontalLabels(sw, scrollBox, labelBox)
XmScaleWidget sw;
XRectangle *scrollBox, *labelBox;
{
	Dimension first_tic, last_tic, tic_interval;
	XmScrollBarWidget sb = (XmScrollBarWidget)
		(sw->composite.children[1]);
	register Widget w;
	register int i;
	register Dimension tmp;
	register Position x, y, y1;

	y1 = labelBox->y + labelBox->height;

	if (sw->composite.num_children > 3)
	{
		tmp = sb->primitive.highlight_thickness
			+ sb->primitive.shadow_thickness
			+ (Dimension) (((float) SLIDER_SIZE / 2.0) + 0.5);
		
		first_tic = tmp + scrollBox->x;
		last_tic = (scrollBox->x + scrollBox->width) - tmp;
		tic_interval = (last_tic - first_tic)
			/ (sw->composite.num_children - 3);

		for (i = 2, tmp = first_tic;
			i < sw->composite.num_children;
			i++, tmp += tic_interval)
		{
			w = sw->composite.children[i];
			x = tmp - (TotalWidth(w) / 2);
			y = y1 - TotalHeight(w);
			_XmMoveObject((RectObj)w, x, y);
		}
	}
	else if (sw->composite.num_children == 3)
	{
		w = sw->composite.children[2];
		y = y1 - TotalHeight(w);
		tmp = (scrollBox->width - TotalWidth(w)) / 2;
		x = scrollBox->x + tmp;
		_XmMoveObject((RectObj)w, x, y);
	}
}

static void LayoutHorizontalScale(sw, optimum_w, optimum_h)
XmScaleWidget sw;
Dimension optimum_w, optimum_h;
{
	int diff_w, diff_h, tdiff;
	XRectangle labelBox, valueBox, scrollBox, titleBox;

	diff_h = XtHeight(sw) - optimum_h;
	diff_w = XtWidth(sw) - optimum_w;
	tdiff = 0;

	/* Figure out all of the y locations */
	if (diff_h >= 0)
	{
		/* 
		 * We place the title, scrollbar, and value from the right
		 */
		titleBox.height = TitleHeight(sw);
		titleBox.y = XtHeight(sw) - titleBox.height;

		scrollBox.height = ScrollHeight(sw);
		scrollBox.y = titleBox.y - scrollBox.height;

		valueBox.height = ValueTroughWidth(sw);
		valueBox.y = scrollBox.y - valueBox.height;

		/*
		 * Labels are placed all the way to the left, which leaves
		 * the dead space between the value and the labels.  I
		 * don't like it, but it is the 1.0 look.
		 */
		labelBox.y = 0;
		labelBox.height = MaxLabelHeight(sw);
	}
	else if ((tdiff = diff_h + TitleHeight(sw)) >= 0)
	{
		/* Place from the left and let the title get clipped */
		labelBox.y = 0;
		labelBox.height = MaxLabelHeight(sw);

		valueBox.y = labelBox.y + labelBox.height;
		valueBox.height = ValueTroughWidth(sw);

		scrollBox.y = valueBox.y + valueBox.height;
		scrollBox.height = ScrollHeight(sw);

		titleBox.y = scrollBox.y + scrollBox.height;
		titleBox.height = TitleHeight(sw);
	}
	else if ((tdiff += ValueTroughHeight(sw)) >= 0)
	{
		/*
		 * The title is outside the window, and the labels are
		 * allowed overwrite (occlude) the value display region
		 */
		titleBox.height = TitleHeight(sw);
		titleBox.y = XtHeight(sw);

		scrollBox.height = ScrollHeight(sw);
		scrollBox.y = titleBox.y - scrollBox.height;

		valueBox.height = ValueTroughHeight(sw);
		valueBox.y = scrollBox.y - valueBox.height;

		labelBox.y = 0;
		labelBox.height = MaxLabelHeight(sw);
	}
	else if ((tdiff += MaxLabelHeight(sw)) >= 0)
	{
		/*
		 * The title is outside the window, the value trough is 
		 * completely coincident with the label region, and the
		 * labels are clipped from the left
		 */
		titleBox.height = TitleHeight(sw);
		titleBox.y = XtHeight(sw);

		scrollBox.height = ScrollHeight(sw);
		scrollBox.y = titleBox.y - scrollBox.height;

		valueBox.height = ValueTroughHeight(sw);
		valueBox.y = scrollBox.y - valueBox.height;

		labelBox.height = MaxLabelHeight(sw);
		labelBox.y = scrollBox.y - labelBox.height;
	}
	else
	{
		/*
		 * Just center the scrollbar in the available space.
		 */
		titleBox.height = TitleHeight(sw);
		titleBox.y = XtHeight(sw);

		valueBox.height = ValueTroughHeight(sw);
		valueBox.y = titleBox.y;

		labelBox.height = MaxLabelHeight(sw);
		labelBox.y = valueBox.y;

		scrollBox.y = (XtHeight(sw) - ScrollHeight(sw)) / 2;
		scrollBox.height = ScrollHeight(sw);
	}

	if (diff_w >= 0)
	{
		scrollBox.x = MajorLeadPad(sw);
		scrollBox.width = ScrollWidth(sw);
	}
	else
	{
		Dimension sb_min, avail, lp, tp;
		XmScrollBarWidget sb = (XmScrollBarWidget)
			(sw->composite.children[1]);

		sb_min = (2 * sb->primitive.highlight_thickness)
			+ (4 * sb->primitive.shadow_thickness)
			+ SLIDER_SIZE;
		
		lp = MajorLeadPad(sw);
		tp = MajorTrailPad(sw);
		avail = XtWidth(sw) - lp - tp;

		if (avail < sb_min)
		{
			scrollBox.width = sb_min;
			scrollBox.x = (XtWidth(sw) - sb_min) / 2;
		}
		else
		{
			scrollBox.width = avail;
			scrollBox.x = lp;
		}
	}

	_XmMoveObject((RectObj)sw->composite.children[0], 0, titleBox.y);

	_XmConfigureObject((RectObj)sw->composite.children[1],
		scrollBox.x, scrollBox.y,
		scrollBox.width, scrollBox.height, 0);

	{
		int n, value, increment, page, slider_size;
		Arg args[5];

		CalcScrollBarData(sw, &value, &slider_size,
			&increment, &page);

		sw->scale.slider_size = slider_size;

		n = 0;
		XtSetArg (args[n], XmNvalue, value);		n++;
		XtSetArg (args[n], XmNsliderSize, sw->scale.slider_size);	n++;
		XtSetArg (args[n], XmNincrement, increment);		n++;
		XtSetArg (args[n], XmNpageIncrement, page);		n++;

		XtSetValues(sw->composite.children[1], args, n);
	}

	LayoutHorizontalLabels(sw, &scrollBox, &labelBox);
}

static void LayoutVerticalLabels(sw, scrollBox, labelBox)
XmScaleWidget sw;
XRectangle *scrollBox, *labelBox;
{
	Dimension first_tic, last_tic, tic_interval;
	XmScrollBarWidget sb = (XmScrollBarWidget)
		(sw->composite.children[1]);
	register Widget w;
	register int i;
	register Dimension tmp;
	register Position x, x1, y;

	x1 = labelBox->x + labelBox->width;

	if (sw->composite.num_children > 3)
	{
		tmp = sb->primitive.highlight_thickness
			+ sb->primitive.shadow_thickness
			+ (Dimension) (((float) SLIDER_SIZE / 2.0) + 0.5);
		
		first_tic = tmp + scrollBox->y;
		last_tic = (scrollBox->y + scrollBox->height) - tmp;
		tic_interval = (last_tic - first_tic)
			/ (sw->composite.num_children - 3);

		for (i = 2, tmp = first_tic;
			i < sw->composite.num_children;
			i++, tmp += tic_interval)
		{
			w = sw->composite.children[i];
			y = tmp - (TotalHeight(w) / 2);
			x = x1 - TotalWidth(w);
			_XmMoveObject((RectObj)w, x, y);
		}
	}
	else if (sw->composite.num_children == 3)
	{
		w = sw->composite.children[2];
		x = x1 - TotalWidth(w);
		tmp = (scrollBox->height - TotalHeight(w)) / 2;
		y = scrollBox->y + tmp;
		_XmMoveObject((RectObj)w, x, y);
	}
}

static void LayoutVerticalScale(sw, optimum_w, optimum_h)
XmScaleWidget sw;
Dimension optimum_w, optimum_h;
{
	int diff_w, diff_h, tdiff;
	XRectangle labelBox, valueBox, scrollBox, titleBox;

	diff_h = XtHeight(sw) - optimum_h;
	diff_w = XtWidth(sw) - optimum_w;
	tdiff = 0;

	/* Figure out all of the x locations */
	if (diff_w >= 0)
	{
		/* 
		 * We place the title, scrollbar, and value from the right
		 */
		titleBox.width = TitleWidth(sw);
		titleBox.x = XtWidth(sw) - titleBox.width;

		scrollBox.width = ScrollWidth(sw);
		scrollBox.x = titleBox.x - scrollBox.width;

		valueBox.width = ValueTroughWidth(sw);
		valueBox.x = scrollBox.x - valueBox.width;

		/*
		 * Labels are placed all the way to the left, which leaves
		 * the dead space between the value and the labels.  I
		 * don't like it, but it is the 1.0 look.
		 */
		labelBox.x = 0;
		labelBox.width = MaxLabelWidth(sw);
	}
	else if ((tdiff = diff_w + TitleWidth(sw)) >= 0)
	{
		/* Place from the left and let the title get clipped */
		labelBox.x = 0;
		labelBox.width = MaxLabelWidth(sw);

		valueBox.x = labelBox.x + labelBox.width;
		valueBox.width = ValueTroughWidth(sw);

		scrollBox.x = valueBox.x + valueBox.width;
		scrollBox.width = ScrollWidth(sw);

		titleBox.x = scrollBox.x + scrollBox.width;
		titleBox.width = TitleWidth(sw);
	}
	else if ((tdiff += ValueTroughWidth(sw)) >= 0)
	{
		/*
		 * The title is outside the window, and the labels are
		 * allowed overwrite (occlude) the value display region
		 */
		titleBox.width = TitleWidth(sw);
		titleBox.x = XtWidth(sw);

		scrollBox.width = ScrollWidth(sw);
		scrollBox.x = titleBox.x - scrollBox.width;

		valueBox.width = ValueTroughWidth(sw);
		valueBox.x = scrollBox.x - valueBox.width;

		labelBox.x = 0;
		labelBox.width = MaxLabelWidth(sw);
	}
	else if ((tdiff += MaxLabelWidth(sw)) >= 0)
	{
		/*
		 * The title is outside the window, the value trough is 
		 * completely coincident with the label region, and the
		 * labels are clipped from the left
		 */
		titleBox.width = TitleWidth(sw);
		titleBox.x = XtWidth(sw);

		scrollBox.width = ScrollWidth(sw);
		scrollBox.x = titleBox.x - scrollBox.width;

		valueBox.width = ValueTroughWidth(sw);
		valueBox.x = scrollBox.x - valueBox.width;

		labelBox.width = MaxLabelWidth(sw);
		labelBox.x = scrollBox.x - labelBox.width;
	}
	else
	{
		/*
		 * Just center the scrollbar in the available space.
		 */
		titleBox.width = TitleWidth(sw);
		titleBox.x = XtWidth(sw);

		valueBox.width = ValueTroughWidth(sw);
		valueBox.x = titleBox.x;

		labelBox.width = MaxLabelWidth(sw);
		labelBox.x = valueBox.x;

		scrollBox.x = (XtWidth(sw) - ScrollWidth(sw)) / 2;
		scrollBox.width = ScrollWidth(sw);
	}

	if (diff_h >= 0)
	{
		scrollBox.y = MajorLeadPad(sw);
		scrollBox.height = ScrollHeight(sw);
	}
	else
	{
		Dimension sb_min, avail, lp, tp;
		XmScrollBarWidget sb = (XmScrollBarWidget)
			(sw->composite.children[1]);

		sb_min = (2 * sb->primitive.highlight_thickness)
			+ (4 * sb->primitive.shadow_thickness)
			+ SLIDER_SIZE;
		
		lp = MajorLeadPad(sw);
		tp = MajorTrailPad(sw);
		avail = XtHeight(sw) - lp - tp;

		if (avail < sb_min)
		{
			scrollBox.height = sb_min;
			scrollBox.y = (XtHeight(sw) - sb_min) / 2;
		}
		else
		{
			scrollBox.height = avail;
			scrollBox.y = lp;
		}
	}

	_XmMoveObject((RectObj)sw->composite.children[0], titleBox.x, 0);

	_XmConfigureObject((RectObj)sw->composite.children[1],
		scrollBox.x, scrollBox.y,
		scrollBox.width, scrollBox.height, 0);
	
	{
		int n, value, increment, page, slider_size;
		Arg args[5];

		CalcScrollBarData(sw, &value, &slider_size,
			&increment, &page);

		sw->scale.slider_size = slider_size;

		n = 0;
		XtSetArg (args[n], XmNvalue, value);		n++;
		XtSetArg (args[n], XmNsliderSize, sw->scale.slider_size);	n++;
		XtSetArg (args[n], XmNincrement, increment);		n++;
		XtSetArg (args[n], XmNpageIncrement, page);		n++;

		XtSetValues(sw->composite.children[1], args, n);
	}

	LayoutVerticalLabels(sw, &scrollBox, &labelBox);
}

static void LayoutScale(sw, resizable)
XmScaleWidget sw;
Boolean resizable;
{
	Dimension tmp_w, tmp_h, sav_w, sav_h;

	/* Save the current values */
	sav_w = XtWidth(sw);
	sav_h = XtHeight(sw);

	/* Mark the scale as anything goes */
	XtWidth(sw) = BIGSIZE;
	XtHeight(sw) = BIGSIZE;

	/* Find out what the best possible answer would be */
	GetScaleSize(sw, &tmp_w, &tmp_h);

	/* Restore the current values */
	XtWidth(sw) = sav_w;
	XtHeight(sw) = sav_h;

	if (resizable)
	{
		if ((tmp_w != XtWidth(sw)) || (tmp_h != XtHeight(sw)))
		{
			Dimension rep_w, rep_h;

			if (XtMakeResizeRequest(sw, tmp_w, tmp_h, &rep_w, &rep_h)
					== XtGeometryAlmost)
				XtMakeResizeRequest(sw, rep_w, rep_h, NULL, NULL);
		}
	}
	
	if (sw->scale.orientation == XmHORIZONTAL)
		LayoutHorizontalScale(sw, tmp_w, tmp_h);
	else /* sw->scale.orientation == XmVERTICAL */
		LayoutVerticalScale(sw, tmp_w, tmp_h);
}



/*********************************************************************
 *  ChangeManaged
 *     Layout children.
 *
 *********************************************************************/

static void ChangeManaged (sw)
XmScaleWidget sw;
{
	Dimension tmp_w, tmp_h;

	if (!XtIsRealized(sw))
	{
		if ((XtWidth(sw) == BIGSIZE) || (XtHeight(sw) == BIGSIZE))
		{
			Dimension rep_w, rep_h;

			GetScaleSize(sw, &tmp_w, &tmp_h);

			if (XtMakeResizeRequest(sw, tmp_w, tmp_h, &rep_w, &rep_h)
				== XtGeometryAlmost)
			{
				XtMakeResizeRequest(sw, rep_w, rep_h, NULL, NULL);
			}
		}
		LayoutScale (sw, FALSE);
	}
	else
	{
		/* Otherwise, Layout and change sizes if needed */
		LayoutScale(sw, TRUE);
	}

	_XmNavigChangeManaged((Widget)sw);
}




/************************************************************************
 *
 *  ShowValue
 *     Display or erase the slider value.
 *
 ************************************************************************/

static void ShowValue (sw, value, show_new)
XmScaleWidget sw;
int value;
Boolean show_new;

{
	int x, y, width, height;
	XCharStruct width_return;
	static char buffer[256];
	register int i;
	int  diff;
	Boolean neg = FALSE;
	int direction, descent;
	XmScrollBarWidget scrollbar;
	Region value_region;
	XRectangle value_rect;

	if (!XtIsRealized(sw))
		return;

	x = sw->scale.show_value_x;
	y = sw->scale.show_value_y;
	width = sw->scale.show_value_width;
	height = sw->scale.show_value_height;

	if (!show_new)  /* turn off the value display */
	{
		if (width) /* We were displaying, so we must clear it */
		{
			XClearArea (XtDisplay (sw), XtWindow (sw), x, y, width, 
				height, FALSE);
			value_rect.x = x;
			value_rect.y = y;
			value_rect.width = width;
			value_rect.height = height;
			value_region = XCreateRegion();
			XUnionRectWithRegion(&value_rect, value_region,
				value_region);
			_XmRedisplayGadgets ((CompositeWidget)sw, NULL, value_region);
			XDestroyRegion(value_region);
		}
		sw->scale.show_value_width = 0;
		return;
	}

	/*
	 * Time for the real work.
	 */

	if (width)
	{
		/* Clear the old one */
		value_rect.x = x;
		value_rect.y = y;
		value_rect.width = width;
		value_rect.height = height;
		value_region = XCreateRegion();
		XClearArea (XtDisplay (sw), XtWindow (sw), x, y, width, 
			height, FALSE);
		XUnionRectWithRegion(&value_rect, value_region,
			value_region);
		_XmRedisplayGadgets ((CompositeWidget)sw, NULL, value_region);
		XDestroyRegion(value_region);
	}

	/*  Get a string representation of the new value  */

	sprintf (buffer,"%d",value);

	if (value < 0) 
	{
		neg = TRUE;
		for (i = 0; i < strlen(buffer); i++)
			buffer[i] = buffer[i+1];
	}

	if (sw->scale.decimal_points > 0)
	{
		if (strlen(buffer) < sw->scale.decimal_points) 
		{
			diff = sw->scale.decimal_points - strlen(buffer);

			for (i = strlen(buffer); i >= 0; i--)
				buffer[i+diff] = buffer[i];
			for (i = 0; i < diff; i++)
			buffer[i] = '0';
		}

		diff = strlen(buffer) - sw->scale.decimal_points;

		for (i = strlen(buffer); i >= diff; i--)
			buffer[i+1] = buffer[i];

		buffer[diff] ='.';
	}

	if (neg)
	{
		for (i = strlen(buffer); i >= 0; i--)
			buffer[i+1] = buffer[i];

		buffer[0] ='-';
	}


	/*  Calculate the x, y, width, and height of the string to display  */

	XTextExtents (sw->scale.font_struct, buffer, strlen(buffer),
		&direction, &height, &descent, &width_return);
	width = width_return.rbearing - width_return.lbearing ;
	sw->scale.show_value_width = width;
	sw->scale.show_value_height = height + descent;

	scrollbar = (XmScrollBarWidget) sw->composite.children[1];

	if (sw->scale.orientation == XmHORIZONTAL)
	{
		x = scrollbar->core.x
			+ scrollbar->scrollBar.slider_x 
			- (width_return.rbearing - SLIDER_SIZE) / 2;
		y = scrollbar->core.y - 3;
	}
	else
	{
		x = scrollbar->core.x - width_return.rbearing;
		y = scrollbar->core.y + scrollbar->scrollBar.slider_y 
			+ SLIDER_SIZE + ((height - SLIDER_SIZE) / 2) - 3;
	}

	sw->scale.show_value_x = x + width_return.lbearing;
	sw->scale.show_value_y = y - height;


	/*  Display the string  */

	XDrawImageString (XtDisplay(sw), XtWindow(sw),
	sw->scale.foreground_GC, x, y, buffer, strlen(buffer));

	sw->scale.last_value = value;
}




/*********************************************************************
 *
 * CalcScrollBarData
 * Figure out the scale derived attributes of the scrollbar child.
 *
 *********************************************************************/
static void CalcScrollBarData(sw, value, slider_size, increment, page)
XmScaleWidget sw;
int *value, *slider_size, *increment, *page;
{
	Dimension scrollbar_size;
	int size ;
	float sb_value;
	float tmp;

	/*  Adjust the slider size to take SLIDER_SIZE area.    */
	/*  Adjust value to be in the bounds of the scrollbar.  */

	if (sw->scale.orientation == XmHORIZONTAL)
		scrollbar_size = sw->composite.children[1]->core.width;
	else
		scrollbar_size = sw->composite.children[1]->core.height;

	size = scrollbar_size - 2 * (sw->scale.highlight_thickness
			+ sw->manager.shadow_thickness);

	/* prevents divide by zero error later */
	if (size <= 0)
	    scrollbar_size = 1;
	else
	    scrollbar_size -= 2 * (sw->scale.highlight_thickness
			+ sw->manager.shadow_thickness);

	*slider_size = ((SCROLLBAR_MAX - SCROLLBAR_MIN) / scrollbar_size)
		* SLIDER_SIZE;

	/*
	 * Now error check our arithmetic
	 */
	if (*slider_size < 0)
	{
		/* We just overflowed the integer size  */
		/* probably due to ratio between        */
		/* SLIDER_SIZE and scrollbar_size being */
		/* greater than approx. 4.3             */
		*slider_size = SCROLLBAR_MAX;
	}
	else if (*slider_size < 1)
	{
		/* ScrollBar will puke, so don't allow this */
		*slider_size = 1;
	}
	else if (*slider_size < SCROLLBAR_MIN)
	{
		/* ScrollBar will puke, so don't allow this */
		*slider_size = SCROLLBAR_MIN;
	}
	else if (*slider_size > SCROLLBAR_MAX)
	{
		/* ScrollBar will puke, so don't allow this */
		*slider_size = SCROLLBAR_MAX;
	}


	sb_value = (float) (sw->scale.value - sw->scale.minimum) / 
		(float) (sw->scale.maximum - sw->scale.minimum);
	sb_value = sb_value * 
		(float) (SCROLLBAR_MAX - *slider_size - SCROLLBAR_MIN);

	*value = (int) sb_value;

	if (*value > SCROLLBAR_MAX - *slider_size)
		*value = SCROLLBAR_MAX - *slider_size;
	if (*value < SCROLLBAR_MIN)
		*value = SCROLLBAR_MIN;

	/* Set up the increment processing correctly */

	tmp = (float) (SCROLLBAR_MAX - SCROLLBAR_MIN)
		- (float) *slider_size;

	*increment = (int) 
		((tmp / (float) (sw->scale.maximum - sw->scale.minimum))
		+ 0.5);
	if (*increment < 1)
		*increment = 1;

	*page = sw->scale.scale_multiple * (*increment);
	if (*page < 1)
		*page = 1;
}



/*********************************************************************
 *
 *  ValueChanged
 *	Callback procedure invoked from the scrollbars value being changed.
 *
 *********************************************************************/

static void ValueChanged (sbw, closure, call_data)
XmScrollBarWidget sbw;
caddr_t closure;
caddr_t call_data;

{
	XmScaleWidget sw = (XmScaleWidget) XtParent (sbw);
	XmScrollBarCallbackStruct * scroll_callback;
	XmScaleCallbackStruct scale_callback;
	int value;
	float sb_value;

	scroll_callback = (XmScrollBarCallbackStruct *) call_data;
	value = scroll_callback->value;

	sb_value = (float) value 
		/ (float) (SCROLLBAR_MAX - sw->scale.slider_size 
			- SCROLLBAR_MIN);
	sb_value = (sb_value * 
			(float) (sw->scale.maximum - sw->scale.minimum))
		+ (float) sw->scale.minimum;

	/* Set up the round off correctly */
	if (sb_value < 0.0)
		sb_value -= 0.5;
	else if (sb_value > 0.0)
		sb_value += 0.5;


	value = (int) sb_value;

	if (sw->scale.show_value)
	{
		sw->scale.last_value = value - 1;
		ShowValue (sw, value, True);
	}

	sw->scale.value = value;
	scale_callback.reason = scroll_callback->reason;
	scale_callback.value = value;

	if (scale_callback.reason == XmCR_DRAG)
		XtCallCallbackList ((Widget) sw, sw->scale.drag_callback,
			&scale_callback);
	else /* value changed and to_top and to_bottom */
	{
		scale_callback.reason = XmCR_VALUE_CHANGED;
		XtCallCallbackList ((Widget) sw,
			sw->scale.value_changed_callback, &scale_callback);

	}
}





/************************************************************************
 *
 *	External API functions.
 *
 ************************************************************************/


/************************************************************************
 *
 *  XmScaleSetValue
 *
 ************************************************************************/

#ifdef _NO_PROTO
void XmScaleSetValue (w, value)
Widget w;
int    value;

#else /* _NO_PROTO */
void XmScaleSetValue (Widget w, int value)
#endif /* _NO_PROTO */
{
   Arg args[1];

   XtSetArg (args[0], XmNvalue, value);
   XtSetValues (w, args, 1);
}




/************************************************************************
 *
 *  XmScaleGetValue
 *
 ************************************************************************/

#ifdef _NO_PROTO
void XmScaleGetValue (w, value)
Widget w;
int * value;

#else /* _NO_PROTO */
void XmScaleGetValue (Widget w, int *value)
#endif /* _NO_PROTO */
{
   XmScaleWidget sw = (XmScaleWidget) w;

   *value = sw->scale.value;
}




/************************************************************************
 *
 *  XmCreateScale
 *
 ************************************************************************/

#ifdef _NO_PROTO
Widget XmCreateScale (parent, name, arglist, argcount)
Widget parent;
char * name;
ArgList arglist;
Cardinal argcount;

#else /* _NO_PROTO */
Widget XmCreateScale (Widget parent, char *name, ArgList arglist, Cardinal argcount)
#endif /* _NO_PROTO */
{
   return (XtCreateWidget(name, xmScaleWidgetClass, parent, arglist, argcount));
}

/*
 * Copy the XmString in XmNtitleString before returning it to the user.
 */
void
#ifdef _NO_PROTO
_XmScaleGetTitleString( scale, resource, value)
            XmScaleWidget scale ;
            XrmQuark        resource ;
            XtArgVal *      value ;
#else
_XmScaleGetTitleString(
            XmScaleWidget scale,
            XrmQuark        resource,
            XtArgVal *      value)
#endif
/****************           ARGSUSED  ****************/
{
  Arg           al[1] ;
/****************/
  if (scale->scale.title == NULL) {
      *value = NULL ;
  } else {
      XtSetArg (al[0], XmNlabelString, value);	/* make a copy */
      XtGetValues (scale->composite.children[0], al, 1);
  }

  return ;
}
