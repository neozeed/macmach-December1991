#ifdef REV_INFO
#ifndef lint
static char SCCSID[] = "OSF/Motif: @(#)ArrowB.c	3.21 91/01/10";
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
#include <Xm/ArrowB.h>
#include <Xm/ArrowBP.h>

#define MESSAGE1 "The arrow direction is incorrect."

#define DELAY_DEFAULT 100	

/*  Event procedures referenced in the actions list  */

static void Activate();
static void MultiActivate();
static void Arm();
static void MultiArm();
static void Disarm();
static void Enter();
static void Leave();
static void ArmAndActivate();
static XtTimerCallbackProc ArmTimeout();

/*  Default translation table and action list  */

#ifndef MCCABE
static char defaultTranslations[] =
    "<Btn1Down>:	Arm()\n\
     <Btn1Down>,<Btn1Up>: Activate()\
			Disarm()\n\
     <Btn1Down>(2+):	MultiArm()\n\
     <Btn1Up>(2+):	MultiActivate()\n\
     <Btn1Up>:		Activate()\
			Disarm()\n\
     <Key>osfSelect:	ArmAndActivate()\n\
     <Key>osfActivate:	ArmAndActivate()\n\
     <Key>osfHelp:	Help()\n\
     ~Shift ~Meta ~Alt <Key>Return:	ArmAndActivate()\n\
     ~Shift ~Meta ~Alt <Key>space:	ArmAndActivate()\n\
     <EnterWindow>:	Enter()\n\
     <LeaveWindow>:	Leave()";
#else
static char defaultTranslations[];
#endif

static XtActionsRec actionsList[] =
{
  { "Activate",    (XtActionProc) Activate	    },
  { "MultiActivate",    (XtActionProc) MultiActivate	    },
  { "Arm",         (XtActionProc) Arm		    },
  { "MultiArm",    (XtActionProc) MultiArm	    },
  { "Disarm",      (XtActionProc) Disarm	    },
  { "ArmAndActivate", (XtActionProc) ArmAndActivate },
  { "Enter",       (XtActionProc) Enter             },
  { "Leave",       (XtActionProc) Leave             },
  { "Help",        (XtActionProc) _XmPrimitiveHelp  }
};


/*  Resource list for ArrowButton  */

static XtResource resources[] = 
{
   {
      XmNmultiClick, XmCMultiClick, XmRMultiClick, 
      sizeof(unsigned char),
      XtOffset(XmArrowButtonWidget, arrowbutton.multiClick), 
      XmRImmediate, (caddr_t) XmMULTICLICK_KEEP
   },

   {
      XmNarrowDirection, XmCArrowDirection, XmRArrowDirection, 
      sizeof(unsigned char),
      XtOffset(XmArrowButtonWidget, arrowbutton.direction), 
      XmRImmediate, (caddr_t) XmARROW_UP
   },

   {
     XmNactivateCallback, XmCCallback, XmRCallback, sizeof(XtCallbackList),
     XtOffset (XmArrowButtonWidget, arrowbutton.activate_callback),
     XmRPointer, (caddr_t) NULL
   },

   {
     XmNarmCallback, XmCCallback, XmRCallback, sizeof(XtCallbackList),
     XtOffset (XmArrowButtonWidget, arrowbutton.arm_callback),
     XmRPointer, (caddr_t) NULL
   },

   {
     XmNdisarmCallback, XmCCallback, XmRCallback, sizeof(XtCallbackList),
     XtOffset (XmArrowButtonWidget, arrowbutton.disarm_callback),
     XmRPointer, (caddr_t) NULL
   }
};


/*  Static routine definitions  */

static void    Initialize();
static void    ClassPartInitialize();
static void    Redisplay();
static void    Resize();
static void    Destroy();
static Boolean SetValues();
static void    GetArrowGC();
static void    ActivateCommon();



/*  The ArrowButton class record definition  */

externaldef (xmarrowbuttonclassrec) XmArrowButtonClassRec xmArrowButtonClassRec=
{
   {
      (WidgetClass) &xmPrimitiveClassRec, /* superclass            */	
      "XmArrowButton",                  /* class_name	         */	
      sizeof(XmArrowButtonRec),         /* widget_size	         */	
      NULL,                             /* class_initialize      */    
      ClassPartInitialize,              /* class_part_initialize */
      FALSE,                            /* class_inited          */	
      (XtInitProc) Initialize,          /* initialize	         */	
      NULL,                             /* initialize_hook       */
      _XtInherit,                       /* realize	         */	
      actionsList,                      /* actions               */	
      XtNumber(actionsList),            /* num_actions    	 */	
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
      defaultTranslations,              /* tm_table              */
      NULL,                             /* query_geometry        */
      NULL,				/* display_accelerator   */
      NULL,				/* extension             */
   },

   {
      _XtInherit,   			/* Primitive border_highlight   */
      _XtInherit,   			/* Primitive border_unhighlight */
      XtInheritTranslations,            /* translations                 */
      ArmAndActivate,		        /* arm_and_activate             */
      NULL,				/* get resources      		*/
      0,				/* num get_resources  		*/
      NULL,         			/* extension                    */
   }

};

externaldef(xmarrowbuttonwidgetclass) WidgetClass xmArrowButtonWidgetClass =
			  (WidgetClass) &xmArrowButtonClassRec;


/************************************************************************
 *
 *  ClassPartInitialize
 *     Set up the fast subclassing for the widget
 *
 ************************************************************************/
static void ClassPartInitialize (wc)
WidgetClass wc;

{
   _XmFastSubclassInit (wc, XmARROW_BUTTON_BIT);
}

      
/************************************************************************
 *
 *  Initialize
 *     The main widget instance initialization routine.
 *
 ************************************************************************/

static void Initialize (request, new)
XmArrowButtonWidget request, new;

{

   /*
    *  Check the data put into the new widget from .Xdefaults
    *  or through the arg list.
    */

   if (new -> arrowbutton.direction != XmARROW_UP   &&
       new -> arrowbutton.direction != XmARROW_DOWN &&
       new -> arrowbutton.direction != XmARROW_LEFT &&
       new -> arrowbutton.direction != XmARROW_RIGHT)
   {
      _XmWarning (new, MESSAGE1);
      new -> arrowbutton.direction = XmARROW_UP;
   }


   /*  Set up a geometry for the widget if it is currently 0.  */

   if (request -> core.width == 0) new -> core.width += 15;
   if (request -> core.height == 0) new -> core.height += 15;


   /*  Set the internal arrow variables and call resize to generate  */
   /*  the arrow drawing rectangle set.                              */

   new->arrowbutton.timer = 0;
   new->arrowbutton.selected = False;
   new->arrowbutton.top_count = new->arrowbutton.bot_count = new->arrowbutton.cent_count = 0;
   new->arrowbutton.top = new->arrowbutton.bot = new->arrowbutton.cent = NULL;
   Resize (new);


   /*  Get the drawing graphics contexts.  */

   GetArrowGC (new);

}




/************************************************************************
 *
 *  GetArrowGC
 *     Get the graphics context used for drawing the arrowbutton.
 *
 ************************************************************************/

static void GetArrowGC (aw)
XmArrowButtonWidget aw;

{
   XGCValues values;
   XtGCMask  valueMask;

   valueMask = GCForeground | GCBackground | GCFillStyle;

   values.foreground = aw -> primitive.foreground;
   values.background = aw -> core.background_pixel;
   values.fill_style = FillSolid;

   aw -> arrowbutton.arrow_GC = XtGetGC ((Widget) aw, valueMask, &values);
}




/************************************************************************
 *
 *  Redisplay
 *     General redisplay function called on exposure events.
 *
 ************************************************************************/
/* ARGSUSED */
static void Redisplay (aw, event, region)
XmArrowButtonWidget aw;
XEvent * event;
Region region;

{
   /*  Draw the arrow  */
   if (aw -> primitive.shadow_thickness > 0)
      _XmDrawShadow (XtDisplay (aw), XtWindow (aw), 
		     aw -> primitive.bottom_shadow_GC,
		     aw -> primitive.top_shadow_GC,
                     aw -> primitive.shadow_thickness,
                     aw -> primitive.highlight_thickness,
                     aw -> primitive.highlight_thickness,
                     aw->core.width - 2 * aw->primitive.highlight_thickness,
                     aw->core.height-2 * aw->primitive.highlight_thickness);

   if (aw->arrowbutton.selected && aw->core.sensitive)
   {
      XFillRectangles (XtDisplay ((Widget) aw), XtWindow ((Widget) aw),
                       aw -> primitive.bottom_shadow_GC,
                       aw -> arrowbutton.top, aw -> arrowbutton.top_count);
      XFillRectangles (XtDisplay ((Widget) aw), XtWindow ((Widget) aw),
                       aw -> primitive.top_shadow_GC,
                       aw -> arrowbutton.bot, aw -> arrowbutton.bot_count);
   }
   else
   {
      XFillRectangles (XtDisplay ((Widget) aw), XtWindow ((Widget) aw),
                       aw -> primitive.top_shadow_GC,
                       aw -> arrowbutton.top, aw -> arrowbutton.top_count);
      XFillRectangles (XtDisplay ((Widget) aw), XtWindow ((Widget) aw),
                       aw -> primitive.bottom_shadow_GC,
                       aw -> arrowbutton.bot, aw -> arrowbutton.bot_count);
   }

   XFillRectangles (XtDisplay ((Widget) aw), XtWindow ((Widget) aw),
                    aw -> arrowbutton.arrow_GC,
                    aw -> arrowbutton.cent, aw -> arrowbutton.cent_count);

   if (aw -> primitive.highlighted)
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
XmArrowButtonWidget aw;
{
_XmGetArrowDrawRects (aw -> primitive.highlight_thickness,
		      aw -> primitive.shadow_thickness,
		      aw -> arrowbutton.direction,
		      aw -> core.width,
		      aw -> core.height,
		      &aw -> arrowbutton.top_count,
		      &aw -> arrowbutton.cent_count,
		      &aw -> arrowbutton.bot_count,
		      &aw -> arrowbutton.top,
		      &aw -> arrowbutton.cent,
		      &aw -> arrowbutton.bot);
}



/************************************************************************
 *
 *  Destroy
 *	Clean up allocated resources when the widget is destroyed.
 *
 ************************************************************************/

static void Destroy (aw)
XmArrowButtonWidget aw;

{
   if (aw->arrowbutton.timer)
      XtRemoveTimeOut (aw->arrowbutton.timer);

   XtReleaseGC (aw, aw -> arrowbutton.arrow_GC);
   XtFree (aw -> arrowbutton.top);
   XtFree (aw -> arrowbutton.cent);
   XtFree (aw -> arrowbutton.bot);

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
XmArrowButtonWidget current, request, new;

{
   Boolean returnFlag = FALSE;
    

    /*  Check the data put into the new widget.  */

   if (new -> arrowbutton.direction != XmARROW_UP   &&
       new -> arrowbutton.direction != XmARROW_DOWN &&
       new -> arrowbutton.direction != XmARROW_LEFT &&
       new -> arrowbutton.direction != XmARROW_RIGHT)
   {
      _XmWarning (new, MESSAGE1);
      new -> arrowbutton.direction = current -> arrowbutton.direction;
   }


   /*  See if the GC's need to be regenerated and widget redrawn.  */

   if (new -> core.background_pixel != current -> core.background_pixel ||
       new -> primitive.foreground != current -> primitive.foreground)
   {
      returnFlag = TRUE;
      XtReleaseGC (new, new -> arrowbutton.arrow_GC);
      GetArrowGC (new);
   }

   if (new -> arrowbutton.direction != current-> arrowbutton.direction ||
       new -> primitive.highlight_thickness != 
          current -> primitive.highlight_thickness                     ||
       new -> primitive.shadow_thickness !=
          current -> primitive.shadow_thickness)
   {
      returnFlag = TRUE;
      Resize (new);
   }

   return (returnFlag);
}




/************************************************************************
 *
 *  arm
 *     This function processes button 1 down occuring on the arrowButton.
 *
 ************************************************************************/

static void Arm (aw, event)
XmArrowButtonWidget aw;
XEvent  * event;

{
   XmArrowButtonCallbackStruct call_value;

   (void) XmProcessTraversal((Widget)aw, XmTRAVERSE_CURRENT);

   aw -> arrowbutton.selected = True;
   aw -> arrowbutton.armTimeStamp = event->xbutton.time; /* see MultiActivate */

   XFillRectangles (XtDisplay ((Widget) aw), XtWindow ((Widget) aw),
                    aw -> primitive.bottom_shadow_GC,
                    aw -> arrowbutton.top, aw -> arrowbutton.top_count);
   XFillRectangles (XtDisplay ((Widget) aw), XtWindow ((Widget) aw),
                    aw -> primitive.top_shadow_GC,
                    aw -> arrowbutton.bot, aw -> arrowbutton.bot_count);

   if (aw->arrowbutton.arm_callback)
   {
      XFlush(XtDisplay(aw));
      
      call_value.reason = XmCR_ARM;
      call_value.event = event;
      XtCallCallbackList ((Widget) aw, aw->arrowbutton.arm_callback, &call_value);
   }
}


static void MultiArm (aw, event)
XmArrowButtonWidget aw;
XEvent  * event;

{
	if (aw->arrowbutton.multiClick == XmMULTICLICK_KEEP)
			Arm (aw, event);
}



/************************************************************************
 *
 *  activate
 *     This function processes button 1 up occuring on the arrowButton.
 *     If the button 1 up occurred inside the button the activate
 *     callbacks are called.
 *
 ************************************************************************/

static void Activate (aw, buttonEvent)
XmArrowButtonWidget aw;
XButtonPressedEvent * buttonEvent;
{
   aw->arrowbutton.click_count = 1;
   ActivateCommon(aw, buttonEvent);
}

static void MultiActivate (aw, buttonEvent)
XmArrowButtonWidget aw;
XButtonPressedEvent * buttonEvent;
{
   /* When a multi click sequence occurs and the user Button Presses and
    * holds for a length of time, the final release should look like a
    * new/separate activate.
    */

  if (aw->arrowbutton.multiClick == XmMULTICLICK_KEEP)
  {
   if ((buttonEvent->time - aw->arrowbutton.armTimeStamp) > 
        XtGetMultiClickTime(XtDisplay(aw)))
     aw->arrowbutton.click_count = 1;
   else
     aw->arrowbutton.click_count++;

   ActivateCommon(aw, buttonEvent);
   Disarm (aw, buttonEvent);
  }
}

static void ActivateCommon (aw, buttonEvent)
XmArrowButtonWidget aw;
XButtonPressedEvent * buttonEvent;

{
   XmArrowButtonCallbackStruct call_value;
   Dimension bw = aw->core.border_width ;

   aw -> arrowbutton.selected = False;

   XFillRectangles (XtDisplay ((Widget) aw), XtWindow ((Widget) aw),
		    aw -> primitive.top_shadow_GC,
		    aw -> arrowbutton.top, aw -> arrowbutton.top_count);
   XFillRectangles (XtDisplay ((Widget) aw), XtWindow ((Widget) aw),
		    aw -> primitive.bottom_shadow_GC,
		    aw -> arrowbutton.bot, aw -> arrowbutton.bot_count);

   if ((buttonEvent->x >= (int)-bw) &&
       (buttonEvent->x < (int)(aw->core.width + bw)) &&
       (buttonEvent->y >= (int)-bw) &&
       (buttonEvent->y < (int)(aw->core.height + bw)) &&
       (aw->arrowbutton.activate_callback))
   {
      XFlush(XtDisplay(aw));

      call_value.reason = XmCR_ACTIVATE;
      call_value.event = (XEvent *) buttonEvent;
      call_value.click_count = aw->arrowbutton.click_count;

      if ((aw->arrowbutton.multiClick == XmMULTICLICK_DISCARD) &&
	  (call_value.click_count > 1)) { 
	  return;
      }

      XtCallCallbackList ((Widget) aw, aw->arrowbutton.activate_callback, &call_value);
   }
}




/************************************************************************
 *
 *     ArmAndActivate
 *
 ************************************************************************/

static void ArmAndActivate (ab, event)
XmArrowButtonWidget ab;
XEvent *event;

{
   XmArrowButtonCallbackStruct call_value;

   ab -> arrowbutton.selected = TRUE;
   Redisplay ( ab, event, FALSE);

   XFlush (XtDisplay (ab));

   if (ab->arrowbutton.arm_callback)
   {
      call_value.reason = XmCR_ARM;
      call_value.event = event;
      XtCallCallbackList((Widget)ab, ab->arrowbutton.arm_callback, &call_value);
   }

   call_value.reason = XmCR_ACTIVATE;
   call_value.event = event;
   call_value.click_count = 1;	/* always 1 in kselect */

   if (ab->arrowbutton.activate_callback)
   {
      XFlush (XtDisplay (ab));
      XtCallCallbackList((Widget)ab,ab->arrowbutton.activate_callback,
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

   if (ab->core.being_destroyed == False)
   {
      ab->arrowbutton.timer = XtAppAddTimeOut(XtWidgetToApplicationContext(ab),
						(unsigned long) DELAY_DEFAULT,
						ArmTimeout,
						(caddr_t)ab);
   }
}

/* ARGSUSED */
static XtTimerCallbackProc ArmTimeout (ab, id)
XmArrowButtonWidget ab;
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
 *     This function processes button 1 up occuring on the arrowButton.
 *
 ************************************************************************/

static void Disarm (aw, event)
XmArrowButtonWidget aw;
XEvent * event;

{
   XmArrowButtonCallbackStruct call_value;

   aw -> arrowbutton.selected = False;

   XFillRectangles (XtDisplay ((Widget) aw), XtWindow ((Widget) aw),
		    aw -> primitive.top_shadow_GC,
		    aw -> arrowbutton.top, aw -> arrowbutton.top_count);
   XFillRectangles (XtDisplay ((Widget) aw), XtWindow ((Widget) aw),
		    aw -> primitive.bottom_shadow_GC,
		    aw -> arrowbutton.bot, aw -> arrowbutton.bot_count);

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
XmArrowButtonWidget aw;
XEvent * event;

{
   _XmPrimitiveEnter ((XmPrimitiveWidget)aw, event);

   if (aw->arrowbutton.selected && aw->core.sensitive)
   {
      XFillRectangles (XtDisplay ((Widget) aw), XtWindow ((Widget) aw),
                       aw -> primitive.bottom_shadow_GC,
                       aw -> arrowbutton.top, aw -> arrowbutton.top_count);
      XFillRectangles (XtDisplay ((Widget) aw), XtWindow ((Widget) aw),
                       aw -> primitive.top_shadow_GC,
                       aw -> arrowbutton.bot, aw -> arrowbutton.bot_count);
   }

}




/************************************************************************
 *
 *  Leave
 *
 ************************************************************************/

static void Leave (aw, event)
XmArrowButtonWidget aw;
XEvent * event;

{
   _XmPrimitiveLeave ((XmPrimitiveWidget)aw, event);

   if (aw->arrowbutton.selected && aw->core.sensitive)
   {
      XFillRectangles (XtDisplay ((Widget) aw), XtWindow ((Widget) aw),
                       aw -> primitive.top_shadow_GC,
                       aw -> arrowbutton.top, aw -> arrowbutton.top_count);
      XFillRectangles (XtDisplay ((Widget) aw), XtWindow ((Widget) aw),
                       aw -> primitive.bottom_shadow_GC,
                       aw -> arrowbutton.bot, aw -> arrowbutton.bot_count);
   }
}


/************************************************************************
 *
 *  XmCreateArrowButton
 *	Create an instance of an arrowbutton and return the widget id.
 *
 ************************************************************************/

#ifdef _NO_PROTO
Widget XmCreateArrowButton (parent, name, arglist, argcount)
Widget parent;
char * name;
ArgList arglist;
Cardinal argcount;

#else /* _NO_PROTO */
Widget XmCreateArrowButton (Widget parent, char *name, ArgList arglist, Cardinal argcount)
#endif /* _NO_PROTO */
{
   return (XtCreateWidget (name, xmArrowButtonWidgetClass, 
                           parent, arglist, argcount));
}




