#ifdef REV_INFO
#ifndef lint
static char SCCSID[] = "OSF/Motif: @(#)ToggleB.c	3.33 91/01/10";
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
/*
 * Include files & Static Routine Definitions
 */


#include <stdio.h>
#include <Xm/XmP.h>
#include <X11/ShellP.h>
#include <Xm/LabelP.h>
#include <Xm/ToggleBP.h>
#include <Xm/ToggleBG.h>
#include <Xm/CascadeB.h>

#define XmINVALID_TYPE  255	/* dynamic default flag for IndicatorType */
#define XmINVALID_BOOLEAN 85    /* dynamic default flag for VisibleWhenOff */

#define PixmapOn(w)            ((w)->toggle.on_pixmap)
#define PixmapOff(w)           ((w)->label.pixmap)
#define Pixmap_Insen_On(w)     ((w)->toggle.insen_pixmap)
#define Pixmap_Insen_Off(w)    ((w)->label.pixmap_insen)
#define IsNull(p)              (p == XmUNSPECIFIED_PIXMAP)
#define IsOn(w)                ((w)->toggle.visual_set)

#define MESSAGE1 "Indicator type should be either XmONE_OF_MANY or XmN_OF_MANY"

static void Redisplay();
static void Resize();
static Boolean SetValues();
static void ClassInitialize();
static void ClassPartInitialize();
static void Initialize();
static void Destroy();
static int  ToggleButtonCallback();
static void Arm();
static void Help();
static void Disarm();
static void Select();
static void Leave();
static void Enter();
static void DrawToggle();
static void GetGC();
static void BtnDown();
static void BtnUp();
static void BorderHighlight();
static void BorderUnhighlight();
static void ArmAndActivate();
static void KeySelect();
static void DrawToggleShadow();
extern  _XmSelectColorDefault();



/*************************************<->*************************************
 *
 *
 *   Description:  default translation table for class: ToggleButton
 *   -----------
 *
 *   Matches events with string descriptors for internal routines.
 *
 *************************************<->***********************************/
static XtTranslations default_parsed;

#ifndef MCCABE
static char defaultTranslations[] =
   "<Btn1Down>:         Arm()\n\
    <Btn1Up>:           Select()\
                        Disarm()\n\
    <Key>osfSelect:	ArmAndActivate()\n\
    <Key>osfActivate:	ArmAndActivate()\n\
    <Key>osfHelp:	Help()\n\
    ~Shift ~Meta ~Alt <Key>Return:	ArmAndActivate()\n\
    ~Shift ~Meta ~Alt <Key>space:	ArmAndActivate()\n\
    <EnterWindow>:      Enter()\n\
    <LeaveWindow>:      Leave()";
#else
static char defaultTranslations[];
#endif /* MCCABE */

static XtTranslations menu_parsed;

#ifndef MCCABE
static char menuTranslations[] =
   "<BtnDown>:	BtnDown()\n\
    <BtnUp>:	BtnUp()\n\
    <Key>osfSelect:	ArmAndActivate()\n\
    <Key>osfActivate:	ArmAndActivate()\n\
    <Key>osfHelp:	Help()\n\
    <Key>osfCancel:	MenuShellPopdownOne()\n\
    ~Shift ~Meta ~Alt <Key>Return:	ArmAndActivate()\n\
    ~Shift ~Meta ~Alt <Key>space:	ArmAndActivate()\n\
    <EnterWindow>:      Enter()\n\
    <LeaveWindow>:      Leave()";
#else
static char menuTranslations[];
#endif

/*************************************<->*************************************
 *
 *
 *   Description:  action list for class: ToggleButton
 *   -----------
 *
 *   Matches string descriptors with internal routines.
 *
 *************************************<->***********************************/

static XtActionsRec actionsList[] =
{
  {"Arm", 	     (XtActionProc) Arm            },
  {"ArmAndActivate", (XtActionProc) ArmAndActivate },
  {"Disarm", 	     (XtActionProc) Disarm         },
  {"Select", 	     (XtActionProc) Select         },
  {"Enter", 	     (XtActionProc) Enter          },
  {"Leave", 	     (XtActionProc) Leave          },
  {"BtnDown",        (XtActionProc) BtnDown        },
  {"BtnUp",          (XtActionProc) BtnUp          },
  {"KeySelect",      (XtActionProc) KeySelect      },
  {"Help",           (XtActionProc) Help},
};




/*************************************<->*************************************
 *
 *
 *   Description:  resource list for class: ToggleButton
 *   -----------
 *
 *   Provides default resource settings for instances of this class.
 *   To get full set of default settings, examine resouce list of super
 *   classes of this class.
 *
 *************************************<->***********************************/

static XtResource resources[] = 
{
   {
     XmNindicatorSize, 
     XmCIndicatorSize, 
     XmRVerticalDimension, 
     sizeof(Dimension),
     XtOffset (XmToggleButtonWidget, toggle.indicator_dim),
     XmRImmediate, (caddr_t) XmINVALID_DIMENSION
   },

   {
     XmNindicatorType, XmCIndicatorType, XmRIndicatorType,sizeof(unsigned char),
     XtOffset (XmToggleButtonWidget, toggle.ind_type),
     XmRImmediate, (caddr_t) XmINVALID_TYPE
   },

   {
     XmNvisibleWhenOff, XmCVisibleWhenOff, XmRBoolean, sizeof(Boolean),
     XtOffset (XmToggleButtonWidget, toggle.visible),
     XmRImmediate, (caddr_t) XmINVALID_BOOLEAN
   },

   {
     XmNspacing, 
     XmCSpacing, 
     XmRHorizontalDimension, 
     sizeof(Dimension),
     XtOffset (XmToggleButtonWidget, toggle.spacing),
     XmRImmediate, (caddr_t) 4
   },

   {
     XmNselectPixmap, XmCSelectPixmap, XmRPrimForegroundPixmap, sizeof(Pixmap),
     XtOffset (XmToggleButtonWidget, toggle.on_pixmap),
     XmRImmediate, (caddr_t) XmUNSPECIFIED_PIXMAP 
   },

   {
     XmNselectInsensitivePixmap, XmCSelectInsensitivePixmap, XmRPixmap,
     sizeof(Pixmap),
     XtOffset (XmToggleButtonWidget, toggle.insen_pixmap),
     XmRImmediate, (caddr_t) XmUNSPECIFIED_PIXMAP
   },

   {
     XmNset, XmCSet, XmRBoolean, sizeof(Boolean),
     XtOffset (XmToggleButtonWidget, toggle.set),
     XmRImmediate, (caddr_t) False
   },

   {
      XmNindicatorOn, XmCIndicatorOn, XmRBoolean, sizeof (Boolean),
      XtOffset (XmToggleButtonWidget, toggle.ind_on),
      XmRImmediate, (caddr_t) True
   },

   {
      XmNfillOnSelect, XmCFillOnSelect, XmRBoolean, sizeof (Boolean),
      XtOffset (XmToggleButtonWidget, toggle.fill_on_select),
      XmRImmediate, (caddr_t) True
   },

   {
      XmNselectColor, XmCSelectColor, XmRPixel, sizeof (Pixel),
      XtOffset (XmToggleButtonWidget, toggle.select_color),
      XmRCallProc, (caddr_t) _XmSelectColorDefault
   },

   {
      XmNvalueChangedCallback, XmCValueChangedCallback, XmRCallback,
      sizeof (XtCallbackList),
      XtOffset (XmToggleButtonWidget, toggle.value_changed_CB),
      XmRPointer, (caddr_t)NULL 
   },

   {
      XmNarmCallback, XmCArmCallback, XmRCallback,
      sizeof (XtCallbackList),
      XtOffset (XmToggleButtonWidget, toggle.arm_CB),
      XmRPointer, (caddr_t)NULL 
   },

   {
      XmNdisarmCallback, XmCDisarmCallback, XmRCallback,
      sizeof (XtCallbackList),
      XtOffset (XmToggleButtonWidget, toggle.disarm_CB),
      XmRPointer, (caddr_t)NULL 
   },

   {
      XmNtraversalOn,
      XmCTraversalOn,
      XmRBoolean,
      sizeof(Boolean),
      XtOffset(XmPrimitiveWidget, primitive.traversal_on),
      XmRImmediate,
      (caddr_t) True
   },
   {
      XmNhighlightThickness,
      XmCHighlightThickness,
      XmRHorizontalDimension,
      sizeof (Dimension),
      XtOffset (XmPrimitiveWidget, primitive.highlight_thickness),
      XmRImmediate,
      (caddr_t) 2
   },
};

/*  Definition for resources that need special processing in get values  */

static XmSyntheticResource syn_resources[] =
{
   { XmNspacing,
     sizeof (Dimension),
     XtOffset (XmToggleButtonWidget, toggle.spacing),
     _XmFromHorizontalPixels,
     _XmToHorizontalPixels },

   { XmNindicatorSize,
     sizeof (Dimension),
     XtOffset (XmToggleButtonWidget, toggle.indicator_dim),
     _XmFromVerticalPixels,
     _XmToVerticalPixels },

};

/*************************************<->*************************************
 *
 *
 *   Description:  global class record for instances of class: ToggleButton
 *   -----------
 *
 *   Defines default field settings for this class record.
 *
 *************************************<->***********************************/

externaldef(xmtogglebuttonclassrec) 
	XmToggleButtonClassRec xmToggleButtonClassRec = {
   {
    /* superclass	  */	(WidgetClass) &xmLabelClassRec,
    /* class_name	  */	"XmToggleButton",
    /* widget_size	  */	sizeof(XmToggleButtonRec),
    /* class_initialize   */    ClassInitialize,
    /* class_part_init    */    ClassPartInitialize,				
    /* class_inited       */	FALSE,
    /* INITialize	  */	Initialize,
    /* initialize_hook    */    NULL,
    /* realize		  */	XmInheritRealize,
    /* actions		  */	actionsList,
    /* num_actions	  */	XtNumber(actionsList),
    /* resources	  */	resources,
    /* num_resources	  */	XtNumber(resources),
    /* xrm_class	  */	NULLQUARK,
    /* compress_motion	  */	TRUE,
    /* compress_exposure  */	XtExposeCompressMaximal,
    /* compress_enterlv   */    TRUE,
    /* visible_interest	  */	FALSE,
    /* destroy		  */	Destroy,
    /* resize		  */	Resize,
    /* expose		  */	Redisplay,
    /* set_values	  */	SetValues,
    /* set_values_hook    */    NULL,
    /* set_values_almost  */    XtInheritSetValuesAlmost,
    /* get_values_hook    */	NULL,
    /* accept_focus       */    NULL,
    /* version            */	XtVersion,
    /* callback_private   */    NULL,
    /* tm_table           */    NULL,
    /* query_geometry     */	XtInheritQueryGeometry, 
    /* display_accelerator */   NULL,
    /* extension          */    NULL,
   },

   {
    /* Primitive border_highlight   */	BorderHighlight,
    /* Primitive border_unhighlight */  BorderUnhighlight,
    /* translations                 */ 	XtInheritTranslations,
										/* (XtTranslations) _XtInherit, */
    /* arm_and_activate             */  ArmAndActivate,
    /* syn resources                */  syn_resources,         
    /* num syn_resources            */  XtNumber(syn_resources),    
    /* extension                    */  NULL,
   },

   {
    /* SetOverrideCallback     */    _XtInherit,
    /* menu procedures    */	     (XmMenuProc)_XtInherit,
    /* menu traversal xlation  */    XtInheritTranslations,
									  /* (XtTranslations)_XtInherit, */
    /* extension               */    NULL,
   }
};

externaldef(xmtogglebuttonwidgetclass)
   WidgetClass xmToggleButtonWidgetClass = (WidgetClass)&xmToggleButtonClassRec;

/*************************************<->*************************************
 *
 *  ClassInitialize
 *
 *************************************<->***********************************/

static void ClassInitialize ()
{
   /* parse the various translation tables */

   default_parsed       = XtParseTranslationTable(defaultTranslations);
   menu_parsed          = XtParseTranslationTable(menuTranslations);
}

/*****************************************************************************
 *
 * ClassPartInitialize
 *   Set up fast subclassing for the widget.
 *
 ****************************************************************************/

static void ClassPartInitialize (wc)
WidgetClass wc;

{
  _XmFastSubclassInit (wc, XmTOGGLE_BUTTON_BIT);
}



/***********************************************************************
 *
 * SetAndDisplayPixmap
 *   Sets the appropriate on, off pixmap in label's pixmap field and
 *   calls label's Redisplay routine.
 *
 ***********************************************************************/

static void SetAndDisplayPixmap(tb)
XmToggleButtonWidget tb;
{
 if (IsOn (tb))
 {
   if ((tb->core.sensitive) && (tb->core.ancestor_sensitive))
   {
     if ( ! IsNull (PixmapOn (tb)))
     {
       Pixmap tempPix;

       tempPix = tb->label.pixmap;
       tb->label.pixmap = tb->toggle.on_pixmap;
       (* xmLabelClassRec.core_class.expose) (tb, NULL, NULL);
       tb->label.pixmap = tempPix;
     }
     else
     (* xmLabelClassRec.core_class.expose) (tb, NULL, NULL);
   }
   else
   {
     if ( ! IsNull (Pixmap_Insen_On (tb)))
     {
       Pixmap tempPix;

       tempPix = tb->label.pixmap_insen;
       tb->label.pixmap_insen = tb->toggle.insen_pixmap;
       (* xmLabelClassRec.core_class.expose) (tb, NULL, NULL);
       tb->label.pixmap_insen = tempPix;
     }
     else
     (* xmLabelClassRec.core_class.expose) (tb, NULL, NULL);
   }
 }
 else
     (* xmLabelClassRec.core_class.expose) (tb, NULL, NULL);
}


/*************************************************************************
 *
 *  Help
 *     This routine is called if the user has made a help selection
 *     on the widget.
 *
 ************************************************************************/

static void Help (tb, event)
XmToggleButtonWidget tb;
XEvent * event;

{
   if (tb -> label.menu_type == XmMENU_PULLDOWN ||
       tb -> label.menu_type == XmMENU_POPUP)
   {
      (* xmLabelClassRec.label_class.menuProcs)
	  (XmMENU_POPDOWN, XtParent(tb), NULL, (caddr_t)event, NULL);
   }

   _XmPrimitiveHelp((XmPrimitiveWidget)tb, event);
}


/*************************************************************************
 *
 * ToggleButtonCallback
 *   This is the widget's application callback routine
 *
 *************************************************************************/

 static int ToggleButtonCallback(data, reason, value, event)
    XmToggleButtonWidget  data;
    unsigned int        reason;
    unsigned int        value;
    XEvent *            event;
{

    XmToggleButtonCallbackStruct temp;

    temp.reason = reason;
    temp.set= value;
    temp.event  = event;

    switch (reason)
      {
        case XmCR_VALUE_CHANGED:
            XtCallCallbackList ((Widget) data, data->toggle.value_changed_CB, &temp);
            break;

        case XmCR_ARM          :
            XtCallCallbackList ((Widget) data, data->toggle.arm_CB, &temp);
            break;

        case XmCR_DISARM       :
            XtCallCallbackList ((Widget) data, data->toggle.disarm_CB, &temp);
            break;

       }

}


/**************************************************************************
 *
 * Leave
 *  This procedure is called when  the mouse button is pressed and  the
 *  cursor moves out of the widget's window. This procedure is used
 *  to change the visuals.
 *
*************************************************************************/
static void Leave(w, event)
     XmToggleButtonWidget w;
     XEvent *event;

{
   int edge, x, y;
   Boolean fill;

   if (w -> label.menu_type == XmMENU_PULLDOWN ||
       w -> label.menu_type == XmMENU_POPUP)
   {
      if (! (((XmManagerWidget) XtParent(w))->manager.traversal_on) &&
	  (w->toggle.Armed))
      {
	 if (w->primitive.shadow_thickness > 0)
	     _XmEraseShadow (XtDisplay (w), XtWindow (w),
                     w-> primitive.shadow_thickness,
                     w-> primitive.highlight_thickness,
                     w-> primitive.highlight_thickness,
                     w-> core.width - 2 * w->primitive.highlight_thickness,
                     w-> core.height - 2 * w->primitive.highlight_thickness);

	 w -> toggle.Armed = FALSE;

	 if (w->toggle.disarm_CB)
	 {
	    XFlush (XtDisplay (w));
	    ToggleButtonCallback(w, XmCR_DISARM, w->toggle.set, event);
	 }
      }
   }

   else
   {  _XmPrimitiveLeave ((XmPrimitiveWidget)w,  event);
      if( w->toggle.indicator_set || _XmStringEmpty(w->label._label) ) {
	edge = w->toggle.indicator_dim;
      } else {
	edge = Min((int)w->toggle.indicator_dim, 
		   Max(0, (int)w->core.height -
		       2*(w->primitive.highlight_thickness +
			  w->primitive.shadow_thickness +
			  (int)w->label.margin_height) +
		       w->label.margin_top +
		       w->label.margin_bottom));
      }

      if ((w->primitive.top_shadow_color != w->toggle.select_color) &&
	  (w->primitive.bottom_shadow_color != w->toggle.select_color))
          fill = TRUE;
      else
          fill = FALSE;

      x = w->primitive.highlight_thickness + w->primitive.shadow_thickness +
	  w->label.margin_width; 

      if ( w->toggle.indicator_set || _XmStringEmpty(w->label._label) )
	y = (w->core.height - w->toggle.indicator_dim)/2;
      else
	y = w->label.TextRect.y;
 
      if ((w->toggle.ind_type) == XmN_OF_MANY)
      {
	 if (w->toggle.Armed == TRUE)
	 { 
	    if (w->toggle.visual_set == TRUE)
		w->toggle.visual_set = FALSE;
	    else
		w->toggle.visual_set = TRUE;
	    if (w->toggle.ind_on)
	    {
	       /* if the toggle indicator is square shaped then adjust the
		  indicator width and height, so that it looks proportional
		  to a diamond shaped indicator of the same width and height */

	       int new_edge;
	       new_edge = edge - 3 - ((edge - 10)/10);
	                                      /* Subtract 3 pixels + 1  */
                                              /* pixel for every 10 pixels, */
                                              /* from the width and height. */

	       /* Adjust x,y so that the indicator is centered relative
		  to the label*/
	       y = y + ((edge - new_edge) / 2);
	       x = x + ((edge - new_edge) / 2);

	       edge = new_edge;

	       if ((w->toggle.visible) ||
		   ((!w->toggle.visible) && (w->toggle.visual_set)))
	       {
		  _XmDrawShadow (XtDisplay (w), XtWindow (w), 
                             ((w->toggle.visual_set) ? 
                               w -> primitive.bottom_shadow_GC :
                               w -> primitive.top_shadow_GC),
                             ((w->toggle.visual_set) ? 
                               w -> primitive.top_shadow_GC :
                               w -> primitive.bottom_shadow_GC), 
                             2, x, y, edge, edge);

    
		  if (w->toggle.fill_on_select)
		      if (edge > 6)
			  XFillRectangle (XtDisplay ((Widget) w), 
					  XtWindow ((Widget) w),
					  ((w->toggle.visual_set) ?
					   w->toggle.select_GC :
					   w -> toggle.background_gc),
					  ((fill) ? x+2 : x+3),
					  ((fill) ? y+2 : y+3),
					  ((fill) ? edge-4 : edge-6),
					  ((fill) ? edge-4 : edge-6));
	       }

	       if (!w->toggle.visible)
	       {
		  if (!w->toggle.visual_set)
		      if (edge > 6)
			  XFillRectangle( XtDisplay ((Widget) w),
                                              XtWindow ((Widget) w),
                                              w->toggle.background_gc,
                                              x, y, edge, edge);
	       }
	    }     
	    else
	    {
	       if (w->primitive.shadow_thickness > 0)
						DrawToggleShadow (w );
	    }
	    if (Lab_IsPixmap(w))
	    {
	       SetAndDisplayPixmap(w);
	    }
	 }
      }
      else
      {
	 if (w->toggle.Armed == TRUE)
	 { 
	    if (w->toggle.visual_set == TRUE)
		w->toggle.visual_set = FALSE;
	    else
		w->toggle.visual_set = TRUE;
  
	    if (w->toggle.ind_on)
	    {
	       if ((w->toggle.visible) ||
		   ((!w->toggle.visible) && (w->toggle.visual_set)))
		   _XmDrawDiamondButton ((Widget)w, x, y, edge,
                                            ((w->toggle.visual_set) ?
                                              w -> primitive.bottom_shadow_GC :
                                              w -> primitive.top_shadow_GC),
                                            ((w->toggle.visual_set) ?
                                              w -> primitive.top_shadow_GC :
                                              w -> primitive.bottom_shadow_GC),
                                            (((w->toggle.visual_set)  &&
                                             (w->toggle.fill_on_select)) ?
                                              w -> toggle.select_GC :
                                              w -> toggle.background_gc),
                                            fill);
      
	       if (!w->toggle.visible)
	       {
		  if (!w->toggle.visual_set)
		      if (edge > 6)
			  XFillRectangle( XtDisplay ((Widget) w),
                                             XtWindow ((Widget) w),
                                             w->toggle.background_gc,
                                             x, y, edge, edge);
	       }
	    }
	    else
	    {
	       if (w->primitive.shadow_thickness > 0) 
					DrawToggleShadow (w);
	    }
	    if (Lab_IsPixmap(w))
	    {
	       SetAndDisplayPixmap(w);
	    }
	 }
      }
   }
}

/**************************************************************************
 *
 * Enter
 *   This procedure is called when the mouse button is pressed and the
 *   cursor reenters the widget's window. This procedure changes the visuals
 *   accordingly.
 *
 **************************************************************************/

static void Enter(w, event)
     XmToggleButtonWidget w;
     XEvent *event;

{
   int edge, x, y;
   Boolean  fill;

   if (w -> label.menu_type == XmMENU_PULLDOWN ||
       w -> label.menu_type == XmMENU_POPUP)
   {
      if ((((ShellWidget) XtParent(XtParent(w)))->shell.popped_up) &&
	  (! ((XmManagerWidget) XtParent(w))->manager.traversal_on))
      {
	 if (w->primitive.shadow_thickness > 0)
	     _XmDrawShadow (XtDisplay (w), XtWindow (w),
                   w -> primitive.top_shadow_GC,
                   w -> primitive.bottom_shadow_GC,
                   w -> primitive.shadow_thickness,
                   w -> primitive.highlight_thickness,
                   w -> primitive.highlight_thickness,
                   w -> core.width - 2 * w->primitive.highlight_thickness,
                   w -> core.height - 2 * w->primitive.highlight_thickness);

	 w -> toggle.Armed = TRUE;

	 if (w->toggle.arm_CB)
	 {
	    XFlush (XtDisplay (w));
	    ToggleButtonCallback(w, XmCR_ARM, w->toggle.set, event);
	 }
      }
   }
   else
   {
      _XmPrimitiveEnter ((XmPrimitiveWidget) w, event);   
      if( w->toggle.indicator_set || _XmStringEmpty(w->label._label) ) {
	edge = w->toggle.indicator_dim;
      } else {
	edge = Min((int)w->toggle.indicator_dim, 
		   Max(0, (int)w->core.height -
		       2*(w->primitive.highlight_thickness +
			  w->primitive.shadow_thickness +
			  (int)w->label.margin_height) +
		       w->label.margin_top +
		       w->label.margin_bottom));
      }

      if ((w->primitive.top_shadow_color != w->toggle.select_color) &&
	  (w->primitive.bottom_shadow_color != w->toggle.select_color))
	  fill = TRUE;
      else
	  fill = FALSE;

      x = w->primitive.highlight_thickness + w->primitive.shadow_thickness +
	  w->label.margin_width;
      
      if( w->toggle.indicator_set || _XmStringEmpty(w->label._label) )
	y = (w->core.height - w->toggle.indicator_dim)/2;
      else
	y = w->label.TextRect.y;

      if ((w->toggle.ind_type) == XmN_OF_MANY) 
      {
	 if (w->toggle.Armed == TRUE)
	 { 
	    if (w->toggle.visual_set == TRUE)
		w->toggle.visual_set = FALSE;
	    else
		w->toggle.visual_set = TRUE;
	    if (w->toggle.ind_on)
	    {
	       /* if the toggle indicator is square shaped then adjust the
		  indicator width and height, so that it looks proportional
		  to a diamond shaped indicator of the same width and height */

	       int new_edge;

	       new_edge = edge - 3 - ((edge - 10)/10);
	                                      /* Subtract 3 pixels+1 */
                                              /* pixel for every 10 pixels, */
                                              /* from the width and height. */

	       /* Adjust x,y so that the indicator is centered
		  relative to the label*/
	       y = y + ((edge - new_edge) / 2);
	       x = x + ((edge - new_edge) / 2);

	       edge = new_edge;

	       if ((w->toggle.visible) ||
		   ((!w->toggle.visible) && (w->toggle.visual_set)))
	       {
		  _XmDrawShadow (XtDisplay (w), XtWindow (w), 
				 ((w->toggle.visual_set) ? 
				  w -> primitive.bottom_shadow_GC :
				  w -> primitive.top_shadow_GC),
				 ((w->toggle.visual_set) ? 
				  w -> primitive.top_shadow_GC :
				  w -> primitive.bottom_shadow_GC), 
				 2, x, y, edge, edge);

		  if (w->toggle.fill_on_select)
		      if (edge > 6)
			  XFillRectangle(XtDisplay((Widget) w),
					 XtWindow ((Widget) w),
					 ((w->toggle.visual_set) ?
					  w->toggle.select_GC :
					  w->toggle.background_gc),
					 ((fill) ? x+2 : x+3),
					 ((fill) ? y+2 : y+3),
					 ((fill) ? edge-4 : edge-6),
					 ((fill) ? edge-4 : edge-6));
	       }

	       if (!w->toggle.visible)
	       {
		  if (!w->toggle.visual_set)
		      if (edge > 6)
			  XFillRectangle (XtDisplay ((Widget) w),
					  XtWindow ((Widget) w),
					  w->toggle.background_gc,
					  x, y, edge, edge);
	       }
	    }
	    else
	    {
	       if (w->primitive.shadow_thickness > 0)
						DrawToggleShadow (w);
	    }

	    if (Lab_IsPixmap(w))
	    {
	       SetAndDisplayPixmap(w);
	    }
	 }
      }
      else 
      {
	 if (w->toggle.Armed == TRUE) 
	 { 
	    if (w->toggle.visual_set == TRUE)
		w->toggle.visual_set = FALSE;
	    else
		w->toggle.visual_set = TRUE;
 
	    if (w->toggle.ind_on)
	    {
	       if ((w->toggle.visible) ||
		   ((!w->toggle.visible) && (w->toggle.visual_set)))
		   _XmDrawDiamondButton ((Widget)w, x, y, edge,
				      ((w->toggle.visual_set) ?
				       w -> primitive.bottom_shadow_GC :
				       w -> primitive.top_shadow_GC),
				      ((w->toggle.visual_set) ?
				       w -> primitive.top_shadow_GC :
				       w -> primitive.bottom_shadow_GC),
				      (((w->toggle.visual_set) &&
					(w->toggle.fill_on_select)) ?
				       w -> toggle.select_GC :
				       w -> toggle.background_gc),
				      fill);
	       if (!w->toggle.visible)
	       {
		  if (!w->toggle.visual_set)
		      if (edge > 6)
			  XFillRectangle( XtDisplay ((Widget) w),
					 XtWindow ((Widget) w),
					 w->toggle.background_gc,
					 x, y, edge, edge);
	       }
	    }
	    else
	    {
	       if (w->primitive.shadow_thickness > 0) DrawToggleShadow (w);
	    }
	    if (Lab_IsPixmap(w))
	    {
	       SetAndDisplayPixmap(w);
	    }
	 }
      }
   }
}


/****************************************************************************
 *
 *     Arm
 *       This function processes button down occuring on the togglebutton.
 *       Mark the togglebutton as armed and display it armed.
 *       The callbacks for XmNarmCallback are called.
 *
 ***************************************************************************/

static void Arm(w,event)
Widget w;
XEvent *event;
{
  XmToggleButtonWidget tb = (XmToggleButtonWidget)w;


  (void)XmProcessTraversal((Widget)tb, XmTRAVERSE_CURRENT);
  tb->toggle.visual_set = (tb->toggle.set == TRUE) ? FALSE : TRUE;
  tb->toggle.Armed = TRUE;
  if (tb->toggle.ind_on)
  {
    DrawToggle(w);
  }
  else
  {
   if (tb->primitive.shadow_thickness > 0) DrawToggleShadow (tb);
  }
  if (Lab_IsPixmap(tb))
  {
    SetAndDisplayPixmap(tb);
  }

  if (tb->toggle.arm_CB)
  {
   XFlush(XtDisplay(w));

   ToggleButtonCallback(tb, XmCR_ARM, tb->toggle.set, event);
  }
}


/************************************************************************
 *
 *     Select 
 *       Mark the togglebutton as unarmed (i.e. inactive).
 *       If the button release occurs inside of the ToggleButton, the
 *       callbacks for XmNvalueChangedCallback are called.
 *
 ************************************************************************/

static void Select(tb,event)
XmToggleButtonWidget tb;
XEvent *event;

{
  XmToggleButtonCallbackStruct call_value;
  XButtonEvent *buttonEvent = (XButtonEvent *) event;
   Dimension bw = tb->core.border_width ;

  tb->toggle.Armed = FALSE;
    
  /* Check to see if BtnUp is inside the widget */

   if ((buttonEvent->x >= (int)-bw) &&
       (buttonEvent->x < (int)(tb->core.width + bw)) &&
       (buttonEvent->y >= (int)-bw) &&
       (buttonEvent->y < (int)(tb->core.height + bw)))
  {
     tb->toggle.set = (tb->toggle.set == TRUE) ? FALSE : TRUE;

     /* if the parent is a RowColumn, notify it about the select */
     if (XmIsRowColumn(XtParent(tb)))
     {
	call_value.reason = XmCR_VALUE_CHANGED;
	call_value.event = event;
	call_value.set = tb->toggle.set;
	(* xmLabelClassRec.label_class.menuProcs) (XmMENU_CALLBACK, 
						    XtParent(tb), FALSE, (caddr_t)tb,
						    (caddr_t)&call_value);
     }

     if ((! tb->label.skipCallback) &&
	 (tb->toggle.value_changed_CB))
     {
	XFlush(XtDisplay(tb));
	ToggleButtonCallback(tb, XmCR_VALUE_CHANGED, tb->toggle.set, event);
     }
  }
     
}


/**********************************************************************
 *
 *    Disarm
 *      The callbacks for XmNdisarmCallback are called..
 *
 ************************************************************************/

static void Disarm(tb,event)
  XmToggleButtonWidget tb;
  XEvent *event;

 { 
   if (tb->toggle.disarm_CB)
    ToggleButtonCallback(tb, XmCR_DISARM, tb->toggle.set, event);
 }

/************************************************************************
 *
 *     ArmAndActivate
 *       This routine arms and activates a ToggleButton. It is called on
 *       <Key> Return and a <Key> Space, as well as when a mnemonic or
 *       button accelerator has been activated.
 *
 ************************************************************************/

static void ArmAndActivate (tb, event)
XmToggleButtonWidget tb;
XEvent *event;

{
   XmToggleButtonCallbackStruct call_value;
   Boolean already_armed = tb -> toggle.Armed;

   tb -> toggle.Armed = FALSE;

   tb->toggle.set = (tb->toggle.set == TRUE) ? FALSE : TRUE;
   tb->toggle.visual_set = tb->toggle.set;

   if (tb -> label.menu_type == XmMENU_PULLDOWN ||
       tb -> label.menu_type == XmMENU_POPUP)

       (* xmLabelClassRec.label_class.menuProcs)
	   (XmMENU_POPDOWN, XtParent(tb), NULL, (caddr_t)event, NULL);
   
   else
   { if (tb->toggle.ind_on) DrawToggle(tb);
	  else
	   if (tb -> primitive.shadow_thickness > 0) DrawToggleShadow (tb);

      if (Lab_IsPixmap(tb))
      {
         SetAndDisplayPixmap(tb);
      }
   }

   if (tb->toggle.arm_CB && !already_armed)
   {
    XFlush(XtDisplay(tb));
    ToggleButtonCallback(tb, XmCR_ARM, tb->toggle.set, event);
   }

   /* if the parent is a RowColumn, notify it about the select */
   if (XmIsRowColumn(XtParent(tb)))
   {
      call_value.reason = XmCR_VALUE_CHANGED;
      call_value.event = event;
      call_value.set = tb->toggle.set;
      (* xmLabelClassRec.label_class.menuProcs) (XmMENU_CALLBACK,
						 XtParent(tb),  FALSE, (caddr_t)tb,
						 (caddr_t)&call_value);
   }
   
   if ((! tb->label.skipCallback) &&
       (tb->toggle.value_changed_CB))
   {
      XFlush(XtDisplay(tb));
      ToggleButtonCallback(tb, XmCR_VALUE_CHANGED, tb->toggle.set, event);
   }

   if (tb->toggle.disarm_CB)
   {
      XFlush(XtDisplay(tb));
      ToggleButtonCallback(tb, XmCR_DISARM, tb->toggle.set, event);
   }
}


/************************************************************************
 *
 *     BtnDown
 *       This function processes a button down occuring on the togglebutton
 *       when it is in a popup, pulldown, or option menu.
 *       Popdown the posted menu.
 *       Turn parent's traversal off.
 *       Mark the togglebutton as armed (i.e. active).
 *       The callbacks for XmNarmCallback are called.
 *
 ************************************************************************/

static void BtnDown (tb, event)
XmToggleButtonWidget tb;
XEvent *event;

{
   XButtonEvent *buttonEvent = (XButtonEvent *) event;
   Boolean validButton;
   Boolean already_armed;

   if (event && (event->type == ButtonPress))
       (* xmLabelClassRec.label_class.menuProcs) (XmMENU_BUTTON,
						  XtParent(tb), NULL, (caddr_t)event,
						  &validButton);

   if (!validButton)
       return;


   /* Popdown other popups that may be up */
   if (XtParent(XtParent(tb))->core.num_popups != 0)
   {
      ShellWidget popup = (ShellWidget)
	  XtParent(XtParent(tb))->core.popup_list[0];
      
      if (popup->shell.popped_up)
          (* xmLabelClassRec.label_class.menuProcs)
	     (XmMENU_SHELL_POPDOWN, (Widget)popup, NULL, (caddr_t)event, NULL);
      else
      {
	 /* If the shell is not popped up but the cascade button is
	  * highlighted, then unhighlight it.
	  */
	  Widget w = ((XmManagerWidget)XtParent(tb))->manager.active_child;
	  if (w && (XmIsCascadeButton(w) || XmIsCascadeButtonGadget(w)))
	      XmCascadeButtonHighlight (w, FALSE);
      }
      
   }

   /* Set focus to this button.  This must follow the possible
    * unhighlighting of the CascadeButton else it'll screw up active_child.
    */
   (void)XmProcessTraversal((Widget)tb, XmTRAVERSE_CURRENT);
	 /* get the location cursor - get consistent with Gadgets */

   (* xmLabelClassRec.label_class.menuProcs) (XmMENU_TRAVERSAL,
					      XtParent(tb), FALSE, NULL, NULL);

   if (tb->primitive.shadow_thickness > 0)
      _XmDrawShadow (XtDisplay (tb), XtWindow (tb),
                 tb -> primitive.top_shadow_GC,
                 tb -> primitive.bottom_shadow_GC,
                 tb -> primitive.shadow_thickness,
                 tb -> primitive.highlight_thickness,
                 tb -> primitive.highlight_thickness,
                 tb -> core.width - 2 * tb->primitive.highlight_thickness,
                 tb -> core.height - 2 * tb->primitive.highlight_thickness);

   already_armed = tb -> toggle.Armed;

   tb -> toggle.Armed = TRUE;

   if (tb->toggle.arm_CB && !already_armed)
   {
      XFlush (XtDisplay (tb));

      ToggleButtonCallback(tb, XmCR_ARM, tb->toggle.set, event);
   }
   _XmRecordEvent(event);

}


/************************************************************************
 *
 *     BtnUp
 *       This function processes a button up occuring on the togglebutton
 *       when it is in a popup, pulldown, or option menu.
 *       Mark the togglebutton as unarmed (i.e. inactive).
 *       The callbacks for XmNvalueChangedCallback are called.
 *       The callbacks for XmNdisarmCallback are called.
 *
 ************************************************************************/

static void BtnUp (tb, event)
XmToggleButtonWidget tb;
XEvent *event;

{
   XButtonEvent *buttonEvent = (XButtonEvent *) event;
   XmToggleButtonCallbackStruct call_value;
   Boolean validButton;

   if (event && (event->type == ButtonRelease))
       (* xmLabelClassRec.label_class.menuProcs) (XmMENU_BUTTON,
						  XtParent(tb), NULL, (caddr_t)event,
						  &validButton);

   if (!validButton)
       return;
   
   tb -> toggle.Armed = FALSE;

   (* xmLabelClassRec.label_class.menuProcs)
       (XmMENU_POPDOWN, XtParent(tb), NULL, (caddr_t)event, NULL);

   _XmRecordEvent(event);

   /* Check to see if BtnUp is inside the widget */

   if ((buttonEvent->x <= tb->core.width) &&
       (buttonEvent->y <= tb->core.height)) 
   {
      tb->toggle.set = (tb->toggle.set == TRUE) ? FALSE : TRUE;
      tb->toggle.visual_set = tb->toggle.set;

      /* if the parent is a RowColumn, notify it about the select */
      if (XmIsRowColumn(XtParent(tb)))
      {
	 call_value.reason = XmCR_VALUE_CHANGED;
	 call_value.event = event;
	 call_value.set = tb->toggle.set;
	 (* xmLabelClassRec.label_class.menuProcs) (XmMENU_CALLBACK, 
						    XtParent(tb), FALSE, (caddr_t)tb,
						    (caddr_t)&call_value);
      }
      
      if ((! tb->label.skipCallback) &&
	  (tb->toggle.value_changed_CB))
      {
	 XFlush(XtDisplay(tb));
	 ToggleButtonCallback(tb, XmCR_VALUE_CHANGED, tb->toggle.set, event);
      }
      
      if (tb->toggle.disarm_CB)
	  ToggleButtonCallback(tb, XmCR_DISARM, tb->toggle.set, event);
   }
}


/************************************************************************
 *
 *  GetGC
 *	Get the graphics context to be used to fill the interior of
 *	a square or diamond when selected.
 *
 ************************************************************************/

static void GetGC (tw)
XmToggleButtonWidget tw;

{
   XGCValues values;
   XtGCMask  valueMask;

   valueMask = GCForeground | GCBackground | GCFillStyle | GCGraphicsExposures;
   values.foreground = tw -> toggle.select_color; 
   values.background = tw -> core.background_pixel;
   values.fill_style = FillSolid;
   values.graphics_exposures = FALSE;

   tw -> toggle.select_GC =  XtGetGC (tw, valueMask, &values);

   valueMask = GCForeground | GCBackground | GCFillStyle | GCGraphicsExposures;
   values.foreground = tw->core.background_pixel;
   values.background = tw->primitive.foreground;
   values.fill_style = FillSolid;
   values.graphics_exposures = FALSE;

   tw->toggle.background_gc = XtGetGC(tw, valueMask, &values);
}


/*************************************<->*************************************
 *
 *  Initialize
 *
 *************************************<->***********************************/

static void Initialize (request, new)
 XmToggleButtonWidget request, new;

{   int maxIndicatorSize;   /* Max Indicator size permissible */
    int delta;
    int boxSize;

    new->toggle.Armed = FALSE;

    /* if menuProcs is not set up yet, try again */
    if (xmLabelClassRec.label_class.menuProcs == NULL)
	xmLabelClassRec.label_class.menuProcs =
	    (XmMenuProc) _XmGetMenuProcContext();

    if (new->label.menu_type == XmMENU_POPUP ||
	new->label.menu_type == XmMENU_PULLDOWN)
    {
       if (new->primitive.shadow_thickness <= 0)
	   new->primitive.shadow_thickness = 2;

       if (new->toggle.visible == XmINVALID_BOOLEAN)
	   new->toggle.visible = FALSE;
       
       new->primitive.traversal_on = TRUE;
    }
    else
    {
       if (new->toggle.visible == XmINVALID_BOOLEAN)
	   new->toggle.visible = TRUE;

    }

    /*
     * if the indicatorType has not been set, then
     * find out if radio behavior is set for RowColumn parents and
     * then set indicatorType.  If radio behavior is true, default to
     * one of many, else default to n of many.
     */
    if ((new->toggle.ind_type != XmN_OF_MANY) &&
	(new->toggle.ind_type != XmONE_OF_MANY))
    {
       if (new->toggle.ind_type != XmINVALID_TYPE)
	   _XmWarning (new, MESSAGE1);

       if  (XmIsRowColumn(XtParent(new)))
       {
	  Arg arg[1];
	  Boolean radio;

	  XtSetArg (arg[0], XmNradioBehavior, &radio);
	  XtGetValues (XtParent(new), arg, 1);

	  if (radio)
	      new->toggle.ind_type = XmONE_OF_MANY;
	  else
	      new->toggle.ind_type = XmN_OF_MANY;
       }
       else
	   new->toggle.ind_type = XmN_OF_MANY;
    }
	

    if (IsNull (PixmapOff(new)) &&            /* no Off pixmap but do have */
        ! IsNull (PixmapOn(new)))           /* an On, so use that */
    {
       new->label.pixmap = new->toggle.on_pixmap;
       if (request->core.width == 0)
         new->core.width = 0;
       if (request->core.height == 0)
         new->core.height = 0;

       (* new->core.widget_class->core_class.superclass->core_class.resize)
	   (new);
    }

    if (IsNull (Pixmap_Insen_Off(new)) &&      /* no Off pixmap but do have */
        ! IsNull (Pixmap_Insen_On(new)))       /* an On, so use that */
    {
       new->label.pixmap_insen = new->toggle.insen_pixmap;
       if (request->core.width == 0)
	   new->core.width = 0;
       if (request->core.height == 0)
	   new->core.height = 0;

       (* new->core.widget_class->core_class.superclass->core_class.resize)
	   (new);
    }

    if (new->toggle.indicator_dim == XmINVALID_DIMENSION)  {
      new->toggle.indicator_set = FALSE;
      if (new->toggle.ind_on)
      {

	/* DETERMINE HOW HIGH THE TOGGLE INDICATOR SHOULD BE */

	if Lab_IsPixmap(new) 
	{
	   /*set indicator size proportional to size of pixmap*/	 
	   if (new->label.TextRect.height < 13)
	       new->toggle.indicator_dim = new->label.TextRect.height;
	   else
	       new->toggle.indicator_dim = 13 + (new->label.TextRect.height/13);
	}

	else
	{
	   /*set indicator size proportional to size of font*/	 
	   Dimension height;
	   int line_count;

	   height = _XmStringHeight (new->label.font, new->label._label);
	   if( (line_count = _XmStringLineCount (new->label._label)) < 1)
	     line_count = 1;
	   /* Shiz recommends toggles in menus have smaller indicators 
	   if (new->label.menu_type == XmMENU_POPUP ||
	       new->label.menu_type == XmMENU_PULLDOWN) {
	     new->toggle.indicator_dim = Max(DEFAULT_INDICATOR_DIM,
	       (height / ((Dimension)line_count))*2/3);
	   } else */
	       new->toggle.indicator_dim = Max(DEFAULT_INDICATOR_DIM,
		 height / ((Dimension)line_count));
	}
      } else
	new->toggle.indicator_dim = 0;
    } else
      new->toggle.indicator_set = TRUE;
 /*
  *
  *   Enlarge the text rectangle if needed to accomodate the size of
  *     indicator button. Adjust the dimenions of superclass Label-Gadget
  *     so that the toggle-button may be accommodated in it.
  */

    maxIndicatorSize = new->toggle.indicator_dim +
					    2 * (new->primitive.shadow_thickness +
							 XM_3D_ENHANCE_PIXEL); 
    boxSize = (int)( new->label.TextRect.height)  +
				(int) new->label.margin_top + (int) new->label.margin_bottom; 
 
	if (maxIndicatorSize > boxSize)
	 { delta = maxIndicatorSize - boxSize;
	   new->label.margin_top += delta/2;
	   new->label.margin_bottom += delta /2;
	 }

    /* Make room for toggle indicator and spacing */

    if (new->toggle.ind_on)
    {
       if ((new->label.margin_left) < (new->toggle.indicator_dim +
                                       new->toggle.spacing))
	   new->label.margin_left = (new->toggle.indicator_dim +
				     new->toggle.spacing);
    }

    if (request->core.width == 0)
    {
       new->core.width = new->label.TextRect.width +
                          2 * new->label.margin_width +   
                          new->label.margin_right +
                          new->label.margin_left +
		          2 * (new->primitive.highlight_thickness +
                               new->primitive.shadow_thickness); 

       if (new->core.width == 0)
	   new->core.width = 1; 

       if ((new->label._acc_text != NULL) && (new->toggle.ind_on))
	   new->label.acc_TextRect.x = new->core.width -
                                         new->primitive.highlight_thickness -
                                         new->primitive.shadow_thickness -
                                         new->label.margin_width -
                                         new->label.margin_right +
                                         LABEL_ACC_PAD;
    }

    if (request->core.height == 0)
	new->core.height = Max(new->toggle.indicator_dim,
	    new->label.TextRect.height + 2 * new->label.margin_height + 
	        new->label.margin_top + new->label.margin_bottom)  + 
	    2 * (new->primitive.highlight_thickness +
		new->primitive.shadow_thickness);

    if (new->core.height == 0)
	new->core.height = 1;


    if (new->toggle.set)
        new->toggle.visual_set = TRUE; /* When toggles first come up, if
                                           XmNset is TRUE, then they are
                                           displayed set */
    else
        new->toggle.visual_set = FALSE;


    /* get the right translations established if the user didn't supply them */

    if (new->core.tm.translations == NULL)
    {
       if (new -> label.menu_type == XmMENU_PULLDOWN ||
	   new -> label.menu_type == XmMENU_POPUP)
	   new->core.tm.translations = menu_parsed;
    
       else     
       {
          new->core.tm.translations = default_parsed;
          XtOverrideTranslations(new,
              ((XmPrimitiveClassRec *)XtClass(new))->primitive_class.
                translations);
       }
    }

    if (new -> label.menu_type == XmMENU_PULLDOWN ||
	new -> label.menu_type == XmMENU_POPUP)
	XtOverrideTranslations(new,
			       ((XmLabelClassRec *)
			        XtClass(new))->label_class.translations);

    Resize(new);
    GetGC (new);
}   





/************************************************************************
 *
 *  Destroy
 *	Free toggleButton's graphic context.
 *
 ************************************************************************/

static void Destroy (tw)
XmToggleButtonWidget tw;

{
   XtReleaseGC (tw, tw -> toggle.select_GC);
   XtReleaseGC (tw, tw -> toggle.background_gc);

   XtRemoveAllCallbacks (tw, XmNvalueChangedCallback);
   XtRemoveAllCallbacks (tw, XmNarmCallback);
   XtRemoveAllCallbacks (tw, XmNdisarmCallback);
}





/*************************************<->*************************************
 *
 *  DrawToggle(w)
 *     Depending on the state of this widget, draw the ToggleButton.
 *
 *************************************<->***********************************/

static void DrawToggle(w)
   XmToggleButtonWidget w;
{
   int x, y, edge;
   Boolean   fill;
   

  if( w->toggle.indicator_set || _XmStringEmpty(w->label._label) ) {
    edge = w->toggle.indicator_dim;
  } else {
    edge = Min((int)w->toggle.indicator_dim, 
             Max(0, (int)w->core.height - 2*(w->primitive.highlight_thickness +
		 w->primitive.shadow_thickness +
		(int)w->label.margin_height) +
		 w->label.margin_top +
		 w->label.margin_bottom));

  }


  if (DefaultDepthOfScreen (XtScreen (w)) == 1) /* Monochrome Display */
     fill = TRUE;
  else
  {
    if ((w->primitive.top_shadow_color != w->toggle.select_color) &&
        (w->primitive.bottom_shadow_color != w->toggle.select_color))
          fill = TRUE;
    else
          fill = FALSE;
  }
      
  x = w->primitive.highlight_thickness + w->primitive.shadow_thickness +
      w->label.margin_width;

  if( w->toggle.indicator_set || _XmStringEmpty(w->label._label) )
    y = (int)((w->core.height - w->toggle.indicator_dim))/2;
  else
    y = w->label.TextRect.y;


  if ((w->toggle.ind_type) == XmN_OF_MANY)
  {
      /* if the toggle indicator is square shaped then adjust the
         indicator width and height, so that it looks proportional
         to a diamond shaped indicator of the same width and height */

     int new_edge;
     
     new_edge = edge - 3 - ((edge - 10)/10); /* Subtract 3 pixels + 1 pixel */
                                              /* for every 10 pixels, from   */
                                              /* width and height.           */

     /* Adjust x,y so that the indicator is centered relative to the label */
     y = y + ((edge - new_edge) / 2); 
     x = x + ((edge - new_edge) / 2);
     edge = new_edge;

     if ((w->toggle.visible) ||
	 ((!w->toggle.visible) && (w->toggle.visual_set)))
     {
	_XmDrawSquareButton ((Widget)w, x, y, edge, 
			      ((w->toggle.visual_set) ? 
			       w -> primitive.bottom_shadow_GC :
			       w -> primitive.top_shadow_GC),
			      ((w->toggle.visual_set) ? 
			       w -> primitive.top_shadow_GC :
			       w -> primitive.bottom_shadow_GC), 
			      (((w->toggle.visual_set) &&
				(w->toggle.fill_on_select)) ?
			       w -> toggle.select_GC :
			       w -> toggle.background_gc),
			      fill);
     }
  }

  else
  {
    if ((w->toggle.visible) ||
          ((!w->toggle.visible) && (w->toggle.visual_set)))
                    _XmDrawDiamondButton ((Widget)w, x, y, edge,
                                      ((w->toggle.visual_set) ? 
                                        w -> primitive.bottom_shadow_GC :
                                        w -> primitive.top_shadow_GC),
                                      ((w->toggle.visual_set) ? 
                                        w -> primitive.top_shadow_GC :
                                        w -> primitive.bottom_shadow_GC), 
                                      (((w->toggle.visual_set) &&
                                        (w->toggle.fill_on_select)) ?
                                        w -> toggle.select_GC :
                                        w -> toggle.background_gc),
                                      fill);
  }

   if ((!w->toggle.visible) && (!w->toggle.visual_set))
   {
       if (edge > 6)
	   XFillRectangle( XtDisplay ((Widget) w),
			  XtWindow ((Widget) w),
			  w->toggle.background_gc,
			  x, y, edge, edge);
   } 
}

/*************************************<->*************************************
 *
 *  BorderHighlight
 *
 *************************************<->***********************************/
static void BorderHighlight (tb)
XmToggleButtonWidget tb;
{
   XEvent * event = NULL;

   if (tb -> label.menu_type == XmMENU_PULLDOWN ||
       tb -> label.menu_type == XmMENU_POPUP)
   {
      if (tb->primitive.shadow_thickness > 0)
	  _XmDrawShadow (XtDisplay (tb), XtWindow (tb),
                       tb -> primitive.top_shadow_GC,
                       tb -> primitive.bottom_shadow_GC,
                       tb -> primitive.shadow_thickness,
                       tb -> primitive.highlight_thickness,
                       tb -> primitive.highlight_thickness,
                       tb -> core.width - 2 *
			   tb->primitive.highlight_thickness,
                       tb -> core.height - 2 *
			   tb->primitive.highlight_thickness);

      tb -> toggle.Armed = TRUE;

      if (tb->toggle.arm_CB)
      {
	 XFlush (XtDisplay (tb));
	 ToggleButtonCallback(tb, XmCR_ARM, tb->toggle.set, event);
      }
   }
   else 
       _XmHighlightBorder((Widget)tb);

}


/*************************************<->*************************************
 *
 *  BorderUnhighlight
 *
 *************************************<->***********************************/
static void BorderUnhighlight (tb)
XmToggleButtonWidget tb;
{
   XEvent * event = NULL;

   if (tb -> label.menu_type == XmMENU_PULLDOWN ||
       tb -> label.menu_type == XmMENU_POPUP)
   {
      if (!tb -> toggle.Armed)
	  return;
      if (tb->primitive.shadow_thickness > 0)
	  _XmEraseShadow (XtDisplay (tb), XtWindow (tb),
                       tb -> primitive.shadow_thickness,
                       tb -> primitive.highlight_thickness,
                       tb -> primitive.highlight_thickness,
                       tb -> core.width - 2 *
			    tb->primitive.highlight_thickness,
                       tb -> core.height - 2 *
			    tb->primitive.highlight_thickness);

      tb -> toggle.Armed = FALSE;
      
      if (tb->toggle.disarm_CB)
      {
	 XFlush (XtDisplay (tb));
	 ToggleButtonCallback(tb, XmCR_DISARM, tb->toggle.set, event);
      }
   }
   else 
       _XmUnhighlightBorder((Widget)tb);
}


/*************************************<->*************************************
 *
 *  KeySelect
 *    If the menu system traversal is enabled, do an activate and disarm
 *
 *************************************<->***********************************/
/*ARGUSED*/
static void KeySelect (tb, event, param, num_param)
XmToggleButtonWidget tb;
XEvent *event;
char  **param;
int    *num_param;
{
   XmToggleButtonCallbackStruct call_value;

   if (!_XmIsEventUnique(event))
      return;

   if (((XmManagerWidget) XtParent(tb)) -> manager.traversal_on)
   {
      tb -> toggle.Armed = FALSE;
      tb->toggle.set = (tb->toggle.set == TRUE) ? FALSE : TRUE;

      (* xmLabelClassRec.label_class.menuProcs)
	  (XmMENU_POPDOWN, XtParent(tb), NULL, (caddr_t)event, NULL);

      _XmRecordEvent(event);

      /* if the parent is a RowColumn, notify it about the select */
      if (XmIsRowColumn(XtParent(tb)))
      {
	 call_value.reason = XmCR_VALUE_CHANGED;
	 call_value.event = event;
	 call_value.set = tb->toggle.set;
	 (* xmLabelClassRec.label_class.menuProcs) (XmMENU_CALLBACK, 
						    XtParent(tb), FALSE, (caddr_t)tb,
						    (caddr_t)&call_value);
      }
      
      if ((! tb->label.skipCallback) &&
	  (tb->toggle.value_changed_CB))
      {
	 XFlush(XtDisplay(tb));
	 ToggleButtonCallback(tb, XmCR_VALUE_CHANGED, tb->toggle.set, event);
      }
   }
}

/************************************************************************
 *
 * Compute Space
 *
 ***********************************************************************/

static void ComputeSpace(tb)
     XmToggleButtonWidget tb;
{

   int needed_width;
   int needed_height;

  /* COMPUTE SPACE FOR DRAWING TOGGLE */

   needed_width = tb->label.TextRect.width +
                  tb->label.margin_left + tb->label.margin_right +
                  (2 * (tb->primitive.shadow_thickness +
                        tb->primitive.highlight_thickness +
                        tb->label.margin_width));

   needed_height = tb->label.TextRect.height +
                   tb->label.margin_top + tb->label.margin_bottom +
                   (2 * (tb->primitive.shadow_thickness +
                         tb->primitive.highlight_thickness +
                         tb->label.margin_height));

   if (needed_height > tb->core.height)
       if (tb->toggle.ind_on)
          tb->label.TextRect.y = tb->primitive.shadow_thickness +
                                 tb->primitive.highlight_thickness +
                                 tb->label.margin_height +
                                 tb->label.margin_top +
                                 ((tb->core.height - tb->label.margin_top
                                 - tb->label.margin_bottom
                                 - (2 * (tb->label.margin_height
                                 + tb->primitive.highlight_thickness
                                 + tb->primitive.shadow_thickness))
                                 - tb->label.TextRect.height) / 2);

  if ((needed_width > tb->core.width) ||
     ((tb->label.alignment == XmALIGNMENT_BEGINNING) 
       && (needed_width < tb->core.width)) ||
     ((tb->label.alignment == XmALIGNMENT_CENTER)
       && (needed_width < tb->core.width) 
       && (tb->core.width - needed_width < tb->label.margin_left)) ||
     (needed_width == tb->core.width))
  {

    if (tb->toggle.ind_on)
      tb->label.TextRect.x = tb->primitive.shadow_thickness +
                             tb->primitive.highlight_thickness +
                             tb->label.margin_width +
                             tb->label.margin_left;
  }

} /* ComputeSpace */

/*************************************<->*************************************
 *
 *  Redisplay (w, event)
 *     Cause the widget, identified by w, to be redisplayed.
 *
 *************************************<->***********************************/
/*ARGUSED*/
static void Redisplay(w, event)
    Widget w;
    XEvent *event;
{
   register XmToggleButtonWidget tb = (XmToggleButtonWidget) w;

   ComputeSpace (tb);

   if (Lab_IsPixmap(tb))
       SetAndDisplayPixmap(tb);
   else
       (* xmLabelClassRec.core_class.expose) (tb, NULL, NULL);

   if (tb->toggle.ind_on)
   {
      tb->toggle.visual_set = tb->toggle.set;
      DrawToggle(tb);
   }

   if (tb -> label.menu_type == XmMENU_PULLDOWN ||
       tb -> label.menu_type == XmMENU_POPUP) 
   {
      if ((tb->toggle.Armed) && 
	  (tb->primitive.shadow_thickness > 0))
          _XmDrawShadow (XtDisplay (tb), XtWindow (tb),
                        tb -> primitive.top_shadow_GC,
                        tb -> primitive.bottom_shadow_GC,
                        tb -> primitive.shadow_thickness,
                        tb ->primitive.highlight_thickness,
                        tb ->primitive.highlight_thickness,
                (int)tb->core.width-2*tb->primitive.highlight_thickness,
                (int)tb->core.height-2*tb->primitive.highlight_thickness);
   }

   else
   {
      if ((tb-> primitive.shadow_thickness) > 0)
				DrawToggleShadow (tb);

      if (tb->primitive.highlighted)
	  _XmHighlightBorder((Widget)tb);
      else if (_XmDifferentBackground ((Widget)tb, XtParent (tb)))
	  _XmUnhighlightBorder((Widget)tb);
   }
}


/**************************************************************************
 *
 * Resize(w, event)
 *
 **************************************************************************/

static void Resize(w)
    Widget w;
{
  register XmToggleButtonWidget tb = (XmToggleButtonWidget) w;

  
  (* tb->core.widget_class->core_class.superclass->core_class.resize) (tb);

}

/***************************************************************************
 *
 *
 *  SetValues(current, request, new)
 *     This is the set values procedure for the ToggleButton class.  It is
 *     called last (the set values rtnes for its superclasses are called
 *     first).
 *
 *************************************<->***********************************/

static Boolean SetValues(current, request, new)
    Widget current, request, new;
{
    XmToggleButtonWidget curcbox = (XmToggleButtonWidget) current;
    XmToggleButtonWidget newcbox = (XmToggleButtonWidget) new;
    Boolean  flag = FALSE;    /* our return value */
    
    int maxIndicatorSize;   /* Max Indicator size permissible */
    int delta;
    int boxSize;
 
    /**********************************************************************
     * Calculate the window size:  The assumption here is that if
     * the width and height are the same in the new and current instance
     * record that those fields were not changed with set values.  Therefore
     * its okay to recompute the necessary width and height.  However, if
     * the new and current do have different width/heights then leave them
     * alone because that's what the user wants.
     *********************************************************************/

     if (IsNull (PixmapOff(newcbox)) &&       /* no On pixmap but do have */
          ! IsNull (PixmapOn(newcbox)))           /* an Off, so use that */
     {
         newcbox->label.pixmap = newcbox->toggle.on_pixmap;
         if ((newcbox->label.recompute_size) &&
            (request->core.width == current->core.width))
              new->core.width = 0;
         if ((newcbox->label.recompute_size) &&
            (request->core.height == current->core.height))
              new->core.height = 0;
         (* new->core.widget_class->core_class.superclass->core_class.resize)
	     (new);
     }

     if (IsNull (Pixmap_Insen_Off(newcbox)) &&   /* no On pixmap but do have */
         ! IsNull (Pixmap_Insen_On(newcbox)))      /* an Off, so use that */
     {
         newcbox->label.pixmap_insen = newcbox->toggle.insen_pixmap;
         if ((newcbox->label.recompute_size) &&
            (request->core.width == current->core.width))
              new->core.width = 0;
         if ((newcbox->label.recompute_size) &&
            (request->core.height == current->core.height))
              new->core.height = 0;
         (* new->core.widget_class->core_class.superclass->core_class.resize)
	     (new);
    }

     if ((newcbox->label.recompute_size) ||
         (newcbox->label._label != curcbox->label._label) ||
         (newcbox->label.pixmap != curcbox->label.pixmap) ||
         (newcbox->label.font != curcbox->label.font) ||
         (newcbox->toggle.spacing != curcbox->toggle.spacing) ||
         (newcbox->toggle.on_pixmap != curcbox->toggle.on_pixmap) ||
         (newcbox->toggle.insen_pixmap != curcbox->toggle.insen_pixmap) ||
         (newcbox->toggle.ind_on != curcbox->toggle.ind_on))
     {
       if (newcbox->label.recompute_size)
       {
         if (request->core.width == current->core.width)
            new->core.width = 0;
         if (request->core.height == current->core.height)
            new->core.height = 0;
       }

       if ((newcbox->toggle.on_pixmap != curcbox->toggle.on_pixmap) ||
           (newcbox->toggle.insen_pixmap != curcbox->toggle.insen_pixmap))
        (* newcbox->core.widget_class->core_class.superclass->core_class.resize)
 (newcbox);


       if ( newcbox->toggle.indicator_dim == XmINVALID_DIMENSION)
                 newcbox->toggle.indicator_set = FALSE;

       if (!(newcbox->toggle.indicator_set))
       {

	 if ((newcbox->label._label != curcbox->label._label) ||
	      (newcbox->label.pixmap != curcbox->label.pixmap) ||
	      (newcbox->label.font != curcbox->label.font) ||
	      (newcbox->toggle.ind_on != curcbox->toggle.ind_on)) 
	 {
	   if Lab_IsPixmap(new)
	   {
	      if (newcbox->label.TextRect.height < 13)
		 newcbox->toggle.indicator_dim = newcbox->label.TextRect.height;
	      else
		 newcbox->toggle.indicator_dim = 13 +
		     (newcbox->label.TextRect.height/13);
	   }
	   else
	   {
	    Dimension height;
	    int line_count;

	    height = _XmStringHeight (newcbox->label.font,
				      newcbox->label._label);
	    line_count = _XmStringLineCount (newcbox->label._label);
	    if (line_count <= 1)
	      newcbox->toggle.indicator_dim = height;
	    else
	      newcbox->toggle.indicator_dim = height /((Dimension)line_count);
	   }

	 }
       } 

       if (newcbox->toggle.ind_on)
       {
          if ((newcbox->label.margin_left) < (newcbox->toggle.indicator_dim +
                                              newcbox->toggle.spacing))
              newcbox->label.margin_left = newcbox->toggle.indicator_dim +
                                           newcbox->toggle.spacing;
       }


       if (newcbox->label.recompute_size)
       {
         if (request->core.width == current->core.width)
            new->core.width = 0;
         if (request->core.height == current->core.height)
            new->core.height = 0;
       }

       if (new->core.width == 0)
       {

         newcbox->core.width =
                   newcbox->label.TextRect.width + 
                   newcbox->label.margin_left + newcbox->label.margin_right +
                   2 * (newcbox->primitive.highlight_thickness +
                        newcbox->primitive.shadow_thickness +
                        newcbox->label.margin_width);

         if (newcbox->core.width == 0)
           newcbox->core.width = 1;

         flag = TRUE;
       }

       if (new->core.height == 0)
       {
         newcbox->core.height = Max(newcbox->toggle.indicator_dim,
	     newcbox->label.TextRect.height + 2*newcbox->label.margin_height +
	         newcbox->label.margin_top + newcbox->label.margin_bottom) +
	     2 * (newcbox->primitive.highlight_thickness +
                 newcbox->primitive.shadow_thickness);

         if (newcbox->core.height == 0)
           newcbox->core.height = 1;

         flag = TRUE;
       }


     }

    if ((newcbox->primitive.foreground !=
         curcbox->primitive.foreground) ||
        (newcbox->core.background_pixel !=
         curcbox->core.background_pixel))
    {
        XtReleaseGC(curcbox, curcbox->toggle.select_GC);
        XtReleaseGC(curcbox, curcbox->toggle.background_gc);
        GetGC(newcbox);
        flag = TRUE;
    }


    if ((curcbox -> toggle.ind_type != newcbox -> toggle.ind_type) ||
       (curcbox -> toggle.visible != newcbox -> toggle.visible)) 
    {
      if ((newcbox->toggle.ind_type != XmN_OF_MANY) &&
       (newcbox->toggle.ind_type != XmONE_OF_MANY))
      {
         newcbox->toggle.ind_type = curcbox->toggle.ind_type;
         _XmWarning (newcbox, MESSAGE1);
      }
       flag = True;
    }
 /*  Recompute the Top and bottom margins and the height of the text
  *  rectangle to  accommodate the size of toggle indicator.
  *  if (we are given a new toggleIndicator size)
  *    { if (user has given new top or bottom margin)
  *           { compute to accomodate new toggle button size;
  *           }
  *       else (user has set new top/bottom margin)
  *           { Recompute margin to accommodate new toogleButtonIndicatorSize;
  *           }
  *    }
  */

   if (newcbox->toggle.indicator_dim != curcbox->toggle.indicator_dim)
   { maxIndicatorSize = (int) (newcbox->toggle.indicator_dim) +
                             2 * (newcbox->primitive.shadow_thickness +
                                         XM_3D_ENHANCE_PIXEL);
     boxSize = (int) (newcbox->label.TextRect.height) +
                (int) (newcbox->label.margin_top) +
                 (int)(newcbox->label.margin_bottom);
    if (maxIndicatorSize != boxSize)
      { delta = maxIndicatorSize - boxSize;
        if ( newcbox->label.margin_top == curcbox->label.margin_top)
           /* User has not specified new top margin */
          { newcbox->label.margin_top = Max ( XM_DEFAULT_TOP_MARGIN,
                        (int) newcbox->label.margin_top + delta/2);
          }
        else
          /* User has sepcified a top margin  and
            Margin must not be less than user specified amount */
         { newcbox->label.margin_top = Max( newcbox->label.margin_top,
                            (newcbox->label.margin_top + delta/2));
         }

        if ( newcbox->label.margin_bottom == curcbox->label.margin_bottom)
           /* User has not specified new bottom margin */
          { newcbox->label.margin_bottom = Max ( XM_DEFAULT_BOTTOM_MARGIN,
                        (int) newcbox->label.margin_bottom + delta/2);
          }
        else
          /* User has sepcified a bottom margin  and
            Margin must not be less than user specified amount */
         { newcbox->label.margin_bottom = Max( newcbox->label.margin_bottom,
                            (newcbox->label.margin_bottom + delta/2));
         }
       }
    }

    if (curcbox -> toggle.select_color != newcbox -> toggle.select_color)
    {
      XtReleaseGC(curcbox, curcbox->toggle.select_GC);
      XtReleaseGC(curcbox, curcbox->toggle.background_gc);
      GetGC(newcbox);
      flag = TRUE;
    }


    if (curcbox -> toggle.set != newcbox -> toggle.set) 
    {
      newcbox->toggle.visual_set = newcbox-> toggle.set;	
      if (flag == False && newcbox->toggle.ind_on && XtIsRealized(newcbox))
			 DrawToggle (newcbox);
		  
     /**  flag = True;		**/
    }

    return(flag);
}

/***************************************************************
 *
 * XmToggleButtonGetState
 *   This function gets the state of the toggle widget.
 *
 ***************************************************************/

Boolean XmToggleButtonGetState(w)
 Widget  w;
{
    XmToggleButtonWidget tw = (XmToggleButtonWidget) w;

    if( XmIsGadget(w) ) {
      return XmToggleButtonGadgetGetState(w);
    }

    return (tw->toggle.set);
}

/****************************************************************
 *
 * XmTogglebuttonSetState
 *   This function sets the state of the toggle widget.
 *
 ****************************************************************/

#ifdef _NO_PROTO
void XmToggleButtonSetState(w, newstate, notify)
 Widget w;
 Boolean newstate;
 Boolean notify;
#else /* _NO_PROTO */
void XmToggleButtonSetState (Widget w,    
#if NeedWidePrototypes
int newstate,
int notify
#else
Boolean newstate,
Boolean notify
#endif 
)
#endif /* _NO_PROTO */
{
   XmToggleButtonWidget tw = (XmToggleButtonWidget) w;

   if( XmIsGadget(w) ) {
      XmToggleButtonGadgetSetState(w, newstate, notify);
      return;
   }

   if (tw->toggle.set != newstate)
   {
      tw->toggle.set = newstate;
      tw->toggle.visual_set = newstate;
      if (XtIsRealized (tw))
      {
         if (tw->toggle.ind_on)
            DrawToggle(tw);
         else
            if (tw->primitive.shadow_thickness > 0)
	       DrawToggleShadow (tw);

         if (Lab_IsPixmap(tw))
            SetAndDisplayPixmap(tw);
         
      }
      if (notify)
      {
          /* if the parent is a RowColumn, notify it about the select */
          if (XmIsRowColumn(XtParent(tw)))
          {
             XmToggleButtonCallbackStruct call_value;
             call_value.reason = XmCR_VALUE_CHANGED;
             call_value.event = NULL;
             call_value.set = tw->toggle.set;
             (* xmLabelClassRec.label_class.menuProcs) (XmMENU_CALLBACK,
                XtParent(tw), FALSE, (caddr_t)tw, (caddr_t)&call_value);
          }

          if ((! tw->label.skipCallback) &&
              (tw->toggle.value_changed_CB))
          {
             XFlush(XtDisplay(tw));
             ToggleButtonCallback(tw, XmCR_VALUE_CHANGED, tw->toggle.set, NULL);
          }
      }
   }
} 
  
/***********************************************************************
 *
 * XmCreateToggleButton
 *   Creates an instance of a togglebutton and returns the widget id.
 *
 ************************************************************************/

#ifdef _NO_PROTO
Widget XmCreateToggleButton(parent,name,arglist,argCount)
       Widget parent;
       char   *name;
       Arg    *arglist;
       Cardinal    argCount;
#else /* _NO_PROTO */
Widget XmCreateToggleButton (Widget parent, char *name, Arg *arglist, Cardinal argCount)
#endif /* _NO_PROTO */
{
    return (XtCreateWidget(name,xmToggleButtonWidgetClass,parent,arglist,argCount));
}
/*
 * DrawToggleShadow (tb)
 *   - Should be called only if ToggleShadow are to be drawn ;
 *	if the IndicatorOn resource is set to false top and bottom shadows
 *	will be switched depending on whether the Toggle is selected or
 *  unselected.
 *   No need to call the routine if shadow_thickness is 0.
 */

static void DrawToggleShadow (tb)
XmToggleButtonWidget tb;
{   GC topgc, bottomgc;
    int dx, dy, width, height;
    int hilite_thickness, shadow_thickness;

	if (!tb->toggle.ind_on)
	{ if (tb->toggle.visual_set)
	  { topgc = tb -> primitive.bottom_shadow_GC;
	    bottomgc = tb -> primitive.top_shadow_GC;
	  }
	  else
	   {topgc = tb -> primitive.top_shadow_GC;
	    bottomgc = tb -> primitive.bottom_shadow_GC;
	   }
	}
	else
	 {topgc = tb -> primitive.top_shadow_GC;
        bottomgc = tb -> primitive.bottom_shadow_GC;
     }

    hilite_thickness =  tb->primitive.highlight_thickness;
	shadow_thickness =  tb->primitive.shadow_thickness;
	width  =  (int) (tb->core.width - (hilite_thickness << 1));
    height = (int) (tb->core.height - (hilite_thickness << 1));
	
	 _XmDrawShadow (XtDisplay (tb), XtWindow (tb),
                    topgc, bottomgc, shadow_thickness,
				    hilite_thickness, hilite_thickness, width, height);

}

