#ifdef REV_INFO
#ifndef lint
static char SCCSID[] = "OSF/Motif: @(#)ArrowBG.c	3.18 91/01/10";
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

#include <stdio.h>
#include <X11/Intrinsic.h>
#include <X11/IntrinsicP.h>
#include <Xm/Xm.h>
#include <Xm/XmP.h>
#include <Xm/ArrowBG.h> 
#include <Xm/ArrowBGP.h>

#define MESSAGE1 "The arrow direction is incorrect."

#define DELAY_DEFAULT 100

/*  Static routine definitions  */

static void    Initialize();
static void    ClassPartInitialize();
static void    Redisplay();
static void    Resize();
static void    Destroy();
static Boolean SetValues();

static void     InputDispatch();
static Boolean  VisualChange();

static void Activate();
static void Arm();
static void Disarm();
static void Enter();
static void Leave();
static void Help();
static void ArmAndActivate();
static XtTimerCallbackProc ArmTimeout();

static void ActivateCommonG();

static void GetArrowGC();


/*  Resource list for Arrow  */

static XtResource resources[] = 
{
   {
     XmNmultiClick,
     XmCMultiClick,
     XmRMultiClick,
     sizeof (unsigned char),
     XtOffset (XmArrowButtonGadget, arrowbutton.multiClick),
     XmRImmediate,
     (caddr_t) XmMULTICLICK_KEEP
   },

   {
      XmNarrowDirection, XmCArrowDirection, XmRArrowDirection, 
      sizeof(unsigned char),
      XtOffset(XmArrowButtonGadget, arrowbutton.direction), 
      XmRImmediate, (caddr_t) XmARROW_UP
   },

   {
     XmNactivateCallback, XmCCallback, XmRCallback, sizeof(XtCallbackList),
     XtOffset (XmArrowButtonGadget, arrowbutton.activate_callback),
     XmRPointer, (caddr_t) NULL
   },

   {
     XmNarmCallback, XmCCallback, XmRCallback, sizeof(XtCallbackList),
     XtOffset (XmArrowButtonGadget, arrowbutton.arm_callback),
     XmRPointer, (caddr_t) NULL
   },

   {
     XmNdisarmCallback, XmCCallback, XmRCallback, sizeof(XtCallbackList),
     XtOffset (XmArrowButtonGadget, arrowbutton.disarm_callback),
     XmRPointer, (caddr_t) NULL
   }
};


/*  The Arrow class record definition  */

externaldef(xmarrowbuttongadgetclassrec) XmArrowButtonGadgetClassRec xmArrowButtonGadgetClassRec =
{
   {
      (WidgetClass) &xmGadgetClassRec,  /* superclass            */
      "XmArrowButtonGadget",            /* class_name	         */
      sizeof(XmArrowButtonGadgetRec),   /* widget_size	         */
      NULL,                             /* class_initialize      */
      ClassPartInitialize,              /* class_part_initialize */
      FALSE,                            /* class_inited          */
      (XtInitProc) Initialize,          /* initialize	         */
      NULL,                             /* initialize_hook       */
      NULL,	                        /* realize	         */
      NULL,                             /* actions               */
      0,			        /* num_actions    	 */
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
      (XtSetValuesFunc) SetValues,      /* set_values	         */	
      NULL,                             /* set_values_hook       */
      XtInheritSetValuesAlmost,         /* set_values_almost     */
      NULL,                             /* get_values_hook       */
      NULL,                             /* accept_focus	         */	
      XtVersion,                        /* version               */
      NULL,                             /* callback private      */
      NULL,                             /* tm_table              */
      NULL,                             /* query_geometry        */
      NULL,				/* display_accelerator   */
      NULL,				/* extension             */
   },

   {
      _XtInherit,					/* border highlight   */
      _XtInherit,					/* border_unhighlight */
      ArmAndActivate,					/* arm_and_activate   */
      InputDispatch,					/* input dispatch     */
      VisualChange,					/* visual_change      */
      NULL,						/* syn resources      */
      0,						/* num syn_resources  */
      NULL,				 	        /* class cache part   */
      NULL,         					/* extension          */
   },

   {
      NULL,			/* extension	      */
   }
};

externaldef(xmarrowbuttongadgetclass) WidgetClass xmArrowButtonGadgetClass = 
   (WidgetClass) &xmArrowButtonGadgetClassRec;



/************************************************************************
 *
 *  ClassPartInitialize
 *     Set up the fast subclassing for the widget
 *
 ************************************************************************/
static void ClassPartInitialize (wc)
WidgetClass wc;

{
   _XmFastSubclassInit (wc, XmARROW_BUTTON_GADGET_BIT);
}
      

/************************************************************************
 *
 *  Initialize
 *     The main widget instance initialization routine.
 *
 ************************************************************************/

static void Initialize (request, new)
XmArrowButtonGadget request, new;

{

   /*
    *  Check the data put into the new widget from .Xdefaults
    *  or through the arg list.
    */

   if (new->arrowbutton.direction != XmARROW_UP   &&
       new->arrowbutton.direction != XmARROW_DOWN &&
       new->arrowbutton.direction != XmARROW_LEFT &&
       new->arrowbutton.direction != XmARROW_RIGHT)
   {
      _XmWarning (new, MESSAGE1);
      new->arrowbutton.direction = XmARROW_UP;
   }


   /*  Set up a geometry for the widget if it is currently 0.  */

   if (request->rectangle.width == 0) new->rectangle.width += 15;
   if (request->rectangle.height == 0) new->rectangle.height += 15;
   new->arrowbutton.old_x = new->rectangle.x;
   new->arrowbutton.old_y = new->rectangle.y;


   /*  Set the internal arrow variables and call resize to generate  */
   /*  the arrow drawing rectangle set.                              */

   new->arrowbutton.timer = 0;
   new->arrowbutton.selected = False;
   new->arrowbutton.top_count = new->arrowbutton.bot_count = new->arrowbutton.cent_count = 0;
   new->arrowbutton.top = new->arrowbutton.bot = new->arrowbutton.cent = NULL;
   Resize (new);


   /*  Get the drawing graphics contexts.  */

   GetArrowGC (new);


   /*  Initialize the interesting input types.  */

   new->gadget.event_mask |=  XmARM_EVENT | XmACTIVATE_EVENT | XmHELP_EVENT |
        XmFOCUS_IN_EVENT | XmFOCUS_OUT_EVENT | XmENTER_EVENT | XmLEAVE_EVENT
		| XmMULTI_ARM_EVENT|  XmMULTI_ACTIVATE_EVENT;

}




/************************************************************************
 *
 *  GetArrowGC
 *     Get the graphics context used for drawing the arrowbutton.
 *
 ************************************************************************/

static void GetArrowGC (ag)
XmArrowButtonGadget ag;

{
   XGCValues values;
   XtGCMask  valueMask;
   XmManagerWidget mw;

   mw = (XmManagerWidget) XtParent(ag);

   valueMask = GCForeground | GCBackground | GCFillStyle;

   values.foreground = mw->manager.foreground;
   values.background = mw->core.background_pixel;
   values.fill_style = FillSolid;

   ag->arrowbutton.arrow_GC = XtGetGC (mw, valueMask, &values);
}




/************************************************************************
 *
 *  Redisplay
 *     General redisplay function called on exposure events.
 *
 ************************************************************************/
/* ARGSUSED */
static void Redisplay (aw, event, region)
XmArrowButtonGadget aw;
XEvent * event;
Region region;

{


   /*  Draw the arrow  */

   if (aw->gadget.shadow_thickness > 0)
      _XmDrawShadow (XtDisplay (aw), XtWindow (aw), 
                     XmParentBottomShadowGC (aw),
                     XmParentTopShadowGC (aw),
                     aw->gadget.shadow_thickness,
                     aw->rectangle.x + aw->gadget.highlight_thickness,
                     aw->rectangle.y + aw->gadget.highlight_thickness,
                     aw->rectangle.width - 2 * aw->gadget.highlight_thickness,
                     aw->rectangle.height-2 * aw->gadget.highlight_thickness);


   _XmOffsetArrow (aw->rectangle.x - aw->arrowbutton.old_x,
                   aw->rectangle.y - aw->arrowbutton.old_y,
                   aw->arrowbutton.top, 
                   aw->arrowbutton.cent,
                   aw->arrowbutton.bot,
                   aw->arrowbutton.top_count,
                   aw->arrowbutton.cent_count, 
                   aw->arrowbutton.bot_count);

   aw->arrowbutton.old_x = aw->rectangle.x;
   aw->arrowbutton.old_y = aw->rectangle.y;


   if (aw->arrowbutton.selected && aw->rectangle.sensitive)
   {
      XFillRectangles (XtDisplay ((Widget) aw), XtWindow ((Widget) aw),
                       XmParentBottomShadowGC (aw),
                       aw->arrowbutton.top, aw->arrowbutton.top_count);
      XFillRectangles (XtDisplay ((Widget) aw), XtWindow ((Widget) aw),
                       XmParentTopShadowGC (aw),
                       aw->arrowbutton.bot, aw->arrowbutton.bot_count);
   }
   else
   {
      XFillRectangles (XtDisplay ((Widget) aw), XtWindow ((Widget) aw),
                       XmParentTopShadowGC (aw),
                       aw->arrowbutton.top, aw->arrowbutton.top_count);
      XFillRectangles (XtDisplay ((Widget) aw), XtWindow ((Widget) aw),
                       XmParentBottomShadowGC (aw),
                       aw->arrowbutton.bot, aw->arrowbutton.bot_count);
   }

   XFillRectangles (XtDisplay ((Widget) aw), XtWindow ((Widget) aw),
                    aw->arrowbutton.arrow_GC,
                    aw->arrowbutton.cent, aw->arrowbutton.cent_count);

   if (aw->gadget.highlighted)
      _XmHighlightBorder ((Widget)aw);
   else if (_XmDifferentBackground ((Widget)aw, XtParent (aw)))
      _XmUnhighlightBorder ((Widget)aw);

}



/************************************************************************
 *
 *  Resize
 *	Calculate the drawing rectangles and draw rectangles.
 *
 ************************************************************************/

static void Resize (aw)
XmArrowButtonGadget aw;
{
   _XmGetArrowDrawRects (aw->gadget.highlight_thickness,
   		         aw->gadget.shadow_thickness,
		         aw->arrowbutton.direction,
		         aw->rectangle.width,
		         aw->rectangle.height,
		         &aw->arrowbutton.top_count,
		         &aw->arrowbutton.cent_count,
		         &aw->arrowbutton.bot_count,
		         &aw->arrowbutton.top,
		         &aw->arrowbutton.cent,
		         &aw->arrowbutton.bot);

   _XmOffsetArrow (aw->rectangle.x,
                   aw->rectangle.y,
                   aw->arrowbutton.top, 
                   aw->arrowbutton.cent,
                   aw->arrowbutton.bot,
                   aw->arrowbutton.top_count,
                   aw->arrowbutton.cent_count, 
                   aw->arrowbutton.bot_count);

   aw->arrowbutton.old_x = aw->rectangle.x;
   aw->arrowbutton.old_y = aw->rectangle.y;

}




/************************************************************************
 *
 *  Destroy
 *	Clean up allocated resources when the widget is destroyed.
 *
 ************************************************************************/

static void Destroy (aw)
XmArrowButtonGadget aw;

{
   XmManagerWidget mw = (XmManagerWidget) XtParent(aw);
   
   if (aw->arrowbutton.timer)
      XtRemoveTimeOut (aw->arrowbutton.timer);

   XtReleaseGC (mw, aw->arrowbutton.arrow_GC);
   XtFree (aw->arrowbutton.top);
   XtFree (aw->arrowbutton.cent);
   XtFree (aw->arrowbutton.bot);

   XtRemoveAllCallbacks (aw, XmNactivateCallback);

   XtRemoveAllCallbacks (aw, XmNarmCallback);

   XtRemoveAllCallbacks (aw, XmNdisarmCallback);

}




/************************************************************************
 *
 *  SetValues
 *
 ************************************************************************/
/* ARGSUSED */
static Boolean SetValues (current, request, new)
XmArrowButtonGadget current, request, new;

{
   Boolean returnFlag = FALSE;
    

    /*  Check the data put into the new widget.  */

   if (new->arrowbutton.direction != XmARROW_UP   &&
       new->arrowbutton.direction != XmARROW_DOWN &&
       new->arrowbutton.direction != XmARROW_LEFT &&
       new->arrowbutton.direction != XmARROW_RIGHT)
   {
      _XmWarning (new, MESSAGE1);
      new->arrowbutton.direction = current->arrowbutton.direction;
   }


   /*  ReInitialize the interesting input types.  */

   new->gadget.event_mask |=  XmARM_EVENT | XmACTIVATE_EVENT | XmHELP_EVENT |
        XmFOCUS_IN_EVENT | XmFOCUS_OUT_EVENT | XmENTER_EVENT | XmLEAVE_EVENT
	    | XmMULTI_ARM_EVENT | XmMULTI_ACTIVATE_EVENT;

   if (new->arrowbutton.direction != current->arrowbutton.direction ||
       new->gadget.highlight_thickness != current->gadget.highlight_thickness ||
       new->gadget.shadow_thickness != current->gadget.shadow_thickness)
   {
      Resize (new);
      returnFlag = TRUE;
   }

   return (returnFlag);
}




/************************************************************************
 *
 *  VisualChange
 *	This function is called from XmManagerClass set values when
 *	the managers visuals have changed.  The gadget regenerates any
 *	GC based on the visual changes and returns True indicating a
 *	redraw is needed.  Otherwize, False is returned.
 *
 ************************************************************************/

static Boolean VisualChange (gw, curmw, newmw)
XmGadget gw;
XmManagerWidget curmw;
XmManagerWidget newmw;

{
   XmArrowButtonGadget abg = (XmArrowButtonGadget) gw;
   XmManagerWidget mw = (XmManagerWidget) XtParent(gw);

   if (curmw->manager.foreground != newmw->manager.foreground ||
       curmw->core.background_pixel != newmw->core.background_pixel)
   {
      XtReleaseGC (mw, abg->arrowbutton.arrow_GC);
      GetArrowGC (abg);
      return (True);
   }

   return (False);
}




/************************************************************************
 *
 *  InputDispatch
 *     This function catches input sent by a manager and dispatches it
 *     to the individual routines.
 *
 ************************************************************************/

static void InputDispatch (ag, event, event_mask)
XmArrowButtonGadget ag;
XEvent * event;
Mask event_mask;

{
   if ( (event_mask & XmARM_EVENT)  || 
			 ((ag->arrowbutton.multiClick == XmMULTICLICK_KEEP) &&
					(event_mask & XmMULTI_ARM_EVENT) ) )
			Arm (ag, event);
   else if (event_mask & XmACTIVATE_EVENT)
         { ag->arrowbutton.click_count = 1;  
		   ActivateCommonG (ag, event, event_mask);
		 }
    else if (event_mask & XmMULTI_ACTIVATE_EVENT)
        { /* if XmNMultiClick resource is set to DISCARD - do nothing
             else call ActivateCommon() and increment clickCount;
           */
           if (ag->arrowbutton.multiClick == XmMULTICLICK_KEEP)
            {  ag->arrowbutton.click_count++;
               ActivateCommonG ( ag, event, event_mask);
		    }
        }

   else if (event_mask & XmHELP_EVENT) Help (ag, event);
   else if (event_mask & XmENTER_EVENT) Enter (ag, event);
   else if (event_mask & XmLEAVE_EVENT) Leave (ag, event);
   else if (event_mask & XmFOCUS_IN_EVENT) _XmFocusInGadget ((XmGadget)ag, event);
   else if (event_mask & XmFOCUS_OUT_EVENT) _XmFocusOutGadget ((XmGadget)ag, event);
}






/************************************************************************
 *
 *  arm
 *     This function processes button 1 down occuring on the arrowbutton.
 *
 ************************************************************************/

static void Arm (aw, event)
XmArrowButtonGadget aw;
XEvent  * event;

{
   XmArrowButtonCallbackStruct call_value;


   aw->arrowbutton.selected = True;

   XFillRectangles (XtDisplay ((Widget) aw), XtWindow ((Widget) aw),
                    XmParentBottomShadowGC (aw),
                    aw->arrowbutton.top, aw->arrowbutton.top_count);
   XFillRectangles (XtDisplay ((Widget) aw), XtWindow ((Widget) aw),
                    XmParentTopShadowGC (aw),
                    aw->arrowbutton.bot, aw->arrowbutton.bot_count);

   if (aw->arrowbutton.arm_callback)
   {
      XFlush(XtDisplay(aw));
      
      call_value.reason = XmCR_ARM;
      call_value.event = event;
      XtCallCallbackList ((Widget) aw, aw->arrowbutton.arm_callback, &call_value);
   }
}




/************************************************************************
 *
 *  activate
 *     This function processes button 1 up occuring on the arrowbutton.
 *     If the button 1 up occurred inside the button the activate
 *     callbacks are called.
 *
 ************************************************************************/

static void Activate (aw, buttonEvent)
XmArrowButtonGadget aw;
XButtonPressedEvent * buttonEvent;

{
   XmPushButtonCallbackStruct call_value;

   aw->arrowbutton.selected = False;

   XFillRectangles (XtDisplay ((Widget) aw), XtWindow ((Widget) aw),
                    XmParentTopShadowGC (aw),
                    aw->arrowbutton.top, aw->arrowbutton.top_count);
   XFillRectangles (XtDisplay ((Widget) aw), XtWindow ((Widget) aw),
                    XmParentBottomShadowGC (aw),
                    aw->arrowbutton.bot, aw->arrowbutton.bot_count);

   if ((buttonEvent->x <= aw->rectangle.x + aw->rectangle.width) && 
       (buttonEvent->y <= aw->rectangle.y + aw->rectangle.height) &&
       (aw->arrowbutton.activate_callback))
   {
      XFlush(XtDisplay(aw));
      
      call_value.reason = XmCR_ACTIVATE;
      call_value.event = (XEvent *) buttonEvent;
	  call_value.click_count = aw->arrowbutton.click_count;
      XtCallCallbackList ((Widget) aw, aw->arrowbutton.activate_callback, &call_value);
   }
}




/************************************************************************
 *
 *     ArmAndActivate
 *
 ************************************************************************/

static void ArmAndActivate (ab, event)
XmArrowButtonGadget ab;
XEvent *event;

{
   XmPushButtonCallbackStruct call_value;

   ab -> arrowbutton.selected = TRUE;
   ab->arrowbutton.click_count = 1;
   Redisplay ( ab, event, FALSE);

   XFlush (XtDisplay (ab));

   if (ab->arrowbutton.arm_callback)
   {
      call_value.reason = XmCR_ARM;
      call_value.event = event;
      call_value.click_count = ab->arrowbutton.click_count;
      XtCallCallbackList ((Widget) ab, ab->arrowbutton.arm_callback,
				&call_value);
   }

   call_value.reason = XmCR_ACTIVATE;
   call_value.event = event;
   call_value.click_count = 1;		/* always 1 in kselect */

   if (ab->arrowbutton.activate_callback)
    {
       XFlush (XtDisplay (ab));
       XtCallCallbackList ((Widget) ab, ab->arrowbutton.activate_callback,
				&call_value);
    }

    ab -> arrowbutton.selected = FALSE;

    if (ab->arrowbutton.disarm_callback)
    {
       XFlush (XtDisplay (ab));
       call_value.reason = XmCR_DISARM;
       XtCallCallbackList ((Widget) ab, ab->arrowbutton.disarm_callback,
				&call_value);
    }

   /* If the button is still around, show it released, after a short delay */

   if (ab->object.being_destroyed == False)
   {
      ab->arrowbutton.timer = XtAppAddTimeOut(XtWidgetToApplicationContext(ab),
						(unsigned long) DELAY_DEFAULT,
						ArmTimeout,
						(caddr_t)ab);
   }
}

/* ARGSUSED */
static XtTimerCallbackProc ArmTimeout (ab, id)
XmArrowButtonGadget ab;
XtIntervalId *id;
{
   ab -> arrowbutton.timer = 0;
   if (XtIsRealized (ab) && XtIsManaged (ab)) {
      Redisplay ( ab, NULL, FALSE); 
      XFlush (XtDisplay (ab));
   }
   return;
}

        

/************************************************************************
 *
 *  disarm
 *     This function processes button 1 up occuring on the arrowbutton.
 *
 ************************************************************************/

static void Disarm (aw, event)
XmArrowButtonGadget aw;
XEvent * event;

{
   XmArrowButtonCallbackStruct call_value;

   call_value.reason = XmCR_DISARM;
   call_value.event = event;
   XtCallCallbackList ((Widget) aw, aw->arrowbutton.disarm_callback, &call_value);
}




/************************************************************************
 *
 *  Enter
 *
 ************************************************************************/

static void Enter (aw, event)
XmArrowButtonGadget aw;
XEvent * event;

{
   _XmEnterGadget ((XmGadget)aw, event);

   if (aw->arrowbutton.selected && aw->rectangle.sensitive)
   {
      XFillRectangles (XtDisplay ((Widget) aw), XtWindow ((Widget) aw),
                       XmParentBottomShadowGC (aw),
                       aw->arrowbutton.top, aw->arrowbutton.top_count);
      XFillRectangles (XtDisplay ((Widget) aw), XtWindow ((Widget) aw),
                       XmParentTopShadowGC (aw),
                       aw->arrowbutton.bot, aw->arrowbutton.bot_count);
   }
}


/************************************************************************
 *
 *  Leave
 *
 ************************************************************************/

static void Leave (aw, event)
XmArrowButtonGadget aw;
XEvent * event;

{
   _XmLeaveGadget ((XmGadget)aw, event);

   if (aw->arrowbutton.selected && aw->rectangle.sensitive)
   {
      XFillRectangles (XtDisplay ((Widget) aw), XtWindow ((Widget) aw),
                       XmParentTopShadowGC (aw),
                       aw->arrowbutton.top, aw->arrowbutton.top_count);
      XFillRectangles (XtDisplay ((Widget) aw), XtWindow ((Widget) aw),
                       XmParentBottomShadowGC (aw),
                       aw->arrowbutton.bot, aw->arrowbutton.bot_count);
   }
}


/************************************************************************
 *
 *  Help
 *     This function processes Function Key 1 press occuring on 
 *     the arrowbutton.
 *
 ************************************************************************/

static void Help (aw, event)
XmArrowButtonGadget aw;
XEvent * event;

{
   XmArrowButtonCallbackStruct call_value;

   call_value.reason = XmCR_HELP;
   call_value.event = event;
   _XmSocorro((Widget)aw, event);
}




/************************************************************************
 *
 *  XmCreateArrowButtonGadget
 *	Create an instance of an arrowbutton and return the widget id.
 *
 ************************************************************************/

#ifdef _NO_PROTO
Widget XmCreateArrowButtonGadget (parent, name, arglist, argcount)
Widget parent;
char * name;
ArgList arglist;
Cardinal argcount;

#else /* _NO_PROTO */
Widget XmCreateArrowButtonGadget (Widget parent, char *name, ArgList arglist, Cardinal argcount)
#endif /* _NO_PROTO */
{
   return (XtCreateWidget (name, xmArrowButtonGadgetClass, 
                           parent, arglist, argcount));
}

/* ARGSUSED */
static void ActivateCommonG (ag, event, event_mask)
XmArrowButtonGadget ag;
XEvent * event;
Mask event_mask;

{
      if (event->type == ButtonRelease)
         {
            Activate (ag, event);
            Disarm (ag, event);
         }
         else  /* assume KeyPress or KeyRelease */
        ArmAndActivate (ag, event);
}

