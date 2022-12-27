#ifdef REV_INFO
#ifndef lint
static char SCCSID[] = "OSF/Motif: @(#)DrawnB.c	3.28 91/01/10";
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
/*
 * Include files & Static Routine Definitions
 */

#include <stdio.h>
#include <X11/X.h>
#include <Xm/XmP.h>
#include <Xm/LabelP.h>
#include <Xm/DrawnBP.h>
   
#define MESSAGE1 "The shadow type is incorrect."

#define DELAY_DEFAULT 100	

static Boolean SetValues();
static void ClassPartInitialize();
static void Initialize();
static void Arm();
static void MultiArm ();
static void Disarm();
static void Activate();
static void MultiActivate();
static void ActivateCommon();
static void Redisplay();
static void Resize();
static void Realize();
static void Enter();
static void Leave();
static void Destroy();
static void ArmAndActivate();
static XtTimerCallbackProc ArmTimeout();
static void DrawPushButton();

/*************************************<->*************************************
 *
 *
 *   Description:   translation tables for class: DrawnButton
 *   -----------
 *
 *   Matches events with string descriptors for internal routines.
 *
 *************************************<->***********************************/

#ifndef MCCABE
static char defaultTranslations[] =
"<Btn1Down>:					Arm()\n\
 <Btn1Down>,<Btn1Up>:				Activate() Disarm()\n\
 <Btn1Down>(2+):				MultiArm()\n\
 <Btn1Up>(2+):					MultiActivate() \n\
 <Btn1Up>:					Activate() Disarm()\n\
 <Key>osfSelect:	ArmAndActivate()\n\
 <Key>osfActivate:	ArmAndActivate()\n\
 <Key>osfHelp:		Help()\n\
 ~Shift ~Meta ~Alt<Key>Return:	ArmAndActivate()\n\
 ~Shift ~Meta ~Alt<Key>space:	ArmAndActivate()\n\
 <EnterWindow>:					Enter()\n\
 <LeaveWindow>:					Leave()";
#else
static char defaultTranslations[];
#endif


/*************************************<->*************************************
 *
 *
 *   Description:  action list for class: DrawnButton
 *   -----------
 *
 *   Matches string descriptors with internal routines.
 *   Note that Primitive will register additional event handlers
 *   for traversal.
 *
 *************************************<->***********************************/

static XtActionsRec actionsList[] =
{
  {"Arm", 	(XtActionProc) Arm		 },
  {"Activate", 	(XtActionProc) Activate		 },
  {"MultiActivate", 	(XtActionProc) MultiActivate		 },
  {"MultiArm",	(XtActionProc) MultiArm },
  {"ArmAndActivate", 	(XtActionProc) ArmAndActivate },
  {"Disarm", 	(XtActionProc) Disarm		 },
  {"Enter", 	(XtActionProc) Enter		 },
  {"Leave",	(XtActionProc) Leave		 },
  {"Help",	(XtActionProc) _XmPrimitiveHelp },
};


/*  The resource list for Drawn Button  */

static XtResource resources[] = 
{     
   {
     XmNmultiClick, XmCMultiClick, XmRMultiClick, sizeof (unsigned char),
     XtOffset (XmDrawnButtonWidget, drawnbutton.multiClick),
     XmRImmediate, (caddr_t) XmMULTICLICK_KEEP
   },

   {
     XmNpushButtonEnabled, XmCPushButtonEnabled, XmRBoolean, sizeof (Boolean),
     XtOffset (XmDrawnButtonWidget, drawnbutton.pushbutton_enabled),
     XmRImmediate, (caddr_t) False
   },

   {
     XmNshadowType, XmCShadowType, XmRShadowType, sizeof(unsigned char),
     XtOffset (XmDrawnButtonWidget, drawnbutton.shadow_type),
     XmRImmediate, (caddr_t) XmSHADOW_ETCHED_IN
   },

   {
     XmNactivateCallback, XmCCallback, XmRCallback, sizeof(XtCallbackList),
     XtOffset (XmDrawnButtonWidget, drawnbutton.activate_callback),
     XmRPointer, (caddr_t) NULL
   },

   {
     XmNarmCallback, XmCCallback, XmRCallback, sizeof(XtCallbackList),
     XtOffset (XmDrawnButtonWidget, drawnbutton.arm_callback),
     XmRPointer, (caddr_t) NULL
   },

   {
     XmNdisarmCallback, XmCCallback, XmRCallback, sizeof(XtCallbackList),
     XtOffset (XmDrawnButtonWidget, drawnbutton.disarm_callback),
     XmRPointer, (caddr_t) NULL
   },
   
   {
     XmNexposeCallback, XmCCallback, XmRCallback, sizeof(XtCallbackList),
     XtOffset (XmDrawnButtonWidget, drawnbutton.expose_callback),
     XmRPointer, (caddr_t) NULL
   },

   {
     XmNresizeCallback, XmCCallback, XmRCallback, sizeof(XtCallbackList),
     XtOffset (XmDrawnButtonWidget, drawnbutton.resize_callback),
     XmRPointer, (caddr_t) NULL
   },
   
   {
     XmNshadowThickness, XmCShadowThickness, XmRHorizontalDimension, 
     sizeof(Dimension),
     XtOffset (XmDrawnButtonWidget, primitive.shadow_thickness),
     XmRImmediate, (caddr_t) 2
   },

   {    
     XmNlabelString, XmCXmString, XmRXmString, sizeof(XmString),
     XtOffset(XmDrawnButtonWidget, label._label),
     XmRString, "\0"
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
   }
};



/*************************************<->*************************************
 *
 *
 *   Description:  global class record for instances of class: DrawnButton
 *   -----------
 *
 *   Defines default field settings for this class record.
 *
 *************************************<->***********************************/

externaldef(xmdrawnbuttonclassrec) XmDrawnButtonClassRec xmDrawnButtonClassRec ={
  {
/* core_class record */	
    /* superclass	  */	(WidgetClass) &xmLabelClassRec,
    /* class_name	  */	"XmDrawnButton",
    /* widget_size	  */	sizeof(XmDrawnButtonRec),
    /* class_initialize   */    NULL,
    /* class_part_init    */    ClassPartInitialize,
    /* class_inited       */	FALSE,
    /* initialize	  */	Initialize,
    /* initialize_hook    */    NULL,
    /* realize		  */	Realize,
    /* actions		  */	actionsList,
    /* num_actions	  */	XtNumber(actionsList),
    /* resources	  */	resources,
    /* num_resources	  */	XtNumber(resources),
    /* xrm_class	  */	NULLQUARK,
    /* compress_motion	  */	TRUE,
    /* compress_exposure  */	FALSE,
    /* compress_enterlv   */    TRUE,
    /* visible_interest	  */	FALSE,
    /* destroy		  */	Destroy,
    /* resize		  */	Resize,
    /* expose		  */	Redisplay,
    /* set_values	  */	SetValues,
    /* set_values_hook    */    NULL,
    /* set_values_almost  */    XtInheritSetValuesAlmost,
    /* get_values_hook    */	NULL,
    /* accept_focus	  */	NULL,
    /* version            */	XtVersion,
    /* callback_private   */    NULL,
    /* tm_table           */    defaultTranslations,
    /* query_geometry     */	NULL, 
    /* display_accelerator */   NULL,
    /* extension          */    NULL,
  },

  { /* primitive_class record       */

    /* Primitive border_highlight   */	_XtInherit,
    /* Primitive border_unhighlight */	_XtInherit,
    /* translations		    */  XtInheritTranslations,
    /* arm_and_activate		    */  ArmAndActivate,
    /* get resources		    */  NULL,
    /* num get_resources	    */  0,
    /* extension		    */  NULL,
  },

  { /* label_class record */
 
    /* setOverrideCallback*/	_XtInherit,
    /* Menu procedures    */    NULL,				
    /* menu trav xlations */	NULL,
    /* extension	  */	NULL,
  }

};
externaldef(xmdrawnbuttonwidgetclass) WidgetClass xmDrawnButtonWidgetClass =
			     (WidgetClass)&xmDrawnButtonClassRec;


/************************************************************************
 *
 *     Arm
 *
 *     This function processes button 1 down occuring on the drawnbutton.
 *     Mark the drawnbutton as armed if XmNpushButtonEnabled is TRUE.
 *     The callbacks for XmNarmCallback are called.
 *
 ************************************************************************/

static void Arm (db, event)
XmDrawnButtonWidget db;
XEvent *event;

{
   XButtonEvent *buttonEvent = (XButtonEvent *) event;
   XmDrawnButtonCallbackStruct call_value;
   
   if (event && (event->type != ButtonPress))
      return;
      
   (void) XmProcessTraversal((Widget)db, XmTRAVERSE_CURRENT);

   db -> drawnbutton.armed = TRUE;
   db -> drawnbutton.armTimeStamp = buttonEvent->time;

   if (db->drawnbutton.pushbutton_enabled)
      DrawPushButton(db, db->drawnbutton.armed);

   if (db->drawnbutton.arm_callback)
   {
      XFlush(XtDisplay (db));

      call_value.reason = XmCR_ARM;
      call_value.event = event;
      call_value.window = XtWindow (db);
      XtCallCallbackList ((Widget) db, db->drawnbutton.arm_callback, &call_value);
   }
}


static void MultiArm(db, event)
XmDrawnButtonWidget db;
XEvent *event;

{
    if (db->drawnbutton.multiClick == XmMULTICLICK_KEEP)
			Arm (db, event);
}

/************************************************************************
 *
 *     Activate
 *
 *     Mark the drawnbutton as unarmed (i.e. inactive).
 *     The foreground and background colors will revert to the 
 *     unarmed state if XmNinvertOnArm is set to TRUE.
 *     If the button release occurs inside of the DrawnButton, the 
 *     callbacks for XmNactivateCallback are called.
 *
 ************************************************************************/

static void Activate (db, buttonEvent)
XmDrawnButtonWidget db;
XButtonPressedEvent * buttonEvent;
{
   db->drawnbutton.click_count = 1;
   ActivateCommon (db, buttonEvent);

}

static void MultiActivate (db, buttonEvent)
XmDrawnButtonWidget db;
XButtonPressedEvent * buttonEvent;
{
   /* When a multi click sequence occurs and the user Button Presses and
    * holds for a length of time, the final release should look like a
    * new/separate activate.
    */
  if (db->drawnbutton.multiClick == XmMULTICLICK_KEEP)  
  { if ((buttonEvent->time - db->drawnbutton.armTimeStamp) >
	   XtGetMultiClickTime(XtDisplay(db)))
     db->drawnbutton.click_count = 1;
   else
     db->drawnbutton.click_count++;
   ActivateCommon (db, buttonEvent);	
   Disarm (db, buttonEvent);
 }
}

static void ActivateCommon (db, event)
XmDrawnButtonWidget db;
XEvent * event;

{
   XButtonEvent *buttonEvent = (XButtonEvent *) event;
   XmDrawnButtonCallbackStruct call_value;
   Dimension bw = db->core.border_width ;
      
   if (event && (event->type != ButtonRelease))
       return;
      
   db -> drawnbutton.armed = FALSE;
   if (db->drawnbutton.pushbutton_enabled)
      DrawPushButton(db, db->drawnbutton.armed);


   if ((buttonEvent->x >= (int)-bw) &&
       (buttonEvent->x < (int)(db->core.width + bw)) &&
       (buttonEvent->y >= (int)-bw) &&
       (buttonEvent->y < (int)(db->core.height + bw)) &&
       (db->drawnbutton.activate_callback))
   {
       XFlush(XtDisplay (db));

       call_value.reason = XmCR_ACTIVATE;
       call_value.event = event;
       call_value.window = XtWindow (db);
       call_value.click_count = db->drawnbutton.click_count;

       if ((db->drawnbutton.multiClick == XmMULTICLICK_DISCARD) &&
	   (call_value.click_count > 1)) return;

       if (XmIsRowColumn(XtParent(db)))
	   {
	       (* xmLabelClassRec.label_class.menuProcs) (XmMENU_CALLBACK, 
						       XtParent(db),
						       FALSE, (caddr_t)db,
						       (caddr_t)&call_value);
	   }

       if ((! db->label.skipCallback) &&
	   (db->drawnbutton.activate_callback))
	   {
	       XtCallCallbackList ((Widget) db,
 				   db->drawnbutton.activate_callback,

				   &call_value);
	   }
   }
}

/************************************************************************
 *
 *     ArmAndActivate
 *
 ************************************************************************/

static void ArmAndActivate (db, event)
XmDrawnButtonWidget db;
XEvent *event;

{
   XmDrawnButtonCallbackStruct call_value;

   db -> drawnbutton.armed = TRUE;
   if (db->drawnbutton.pushbutton_enabled)
      DrawPushButton(db, db->drawnbutton.armed);

   XFlush(XtDisplay (db));

   if (db->drawnbutton.arm_callback)
   {
      call_value.reason = XmCR_ARM;
      call_value.event = event;
      call_value.window = XtWindow (db);
      XtCallCallbackList((Widget)db, db->drawnbutton.arm_callback, &call_value);
   }

   call_value.reason = XmCR_ACTIVATE;
   call_value.event = event;
   call_value.window = XtWindow (db);
   call_value.click_count = 1;		/* always 1 in kselect */

   if (XmIsRowColumn(XtParent(db)))
   {
      (* xmLabelClassRec.label_class.menuProcs) (XmMENU_CALLBACK, 
						 XtParent(db),
						 FALSE, (caddr_t)db,
						 (caddr_t)&call_value);
   }

   if ((! db->label.skipCallback) && (db->drawnbutton.activate_callback))
   {
      XtCallCallbackList ((Widget) db, db->drawnbutton.activate_callback,
			     &call_value);
   }

   db -> drawnbutton.armed = FALSE;

   if (db->drawnbutton.disarm_callback)
   {
      call_value.reason = XmCR_DISARM;
      XtCallCallbackList ((Widget) db, db->drawnbutton.disarm_callback,
			     &call_value);
   }

   /* If the button is still around, show it released, after a short delay */

   if (!db->core.being_destroyed && db->drawnbutton.pushbutton_enabled)
   {
       db->drawnbutton.timer = XtAppAddTimeOut(XtWidgetToApplicationContext(db),
		   				(unsigned long) DELAY_DEFAULT,
		   				ArmTimeout,
						(caddr_t)db);
   }
}


/*ARGSUSED*/
static XtTimerCallbackProc ArmTimeout (db, id)
XmDrawnButtonWidget db;
XtIntervalId *id;
{
   db -> drawnbutton.timer = 0;
   if (db->drawnbutton.pushbutton_enabled &&
       XtIsRealized (db) && XtIsManaged (db))
   {
      DrawPushButton(db, db->drawnbutton.armed);
      XFlush (XtDisplay (db));
   }
   return;
}


/************************************************************************
 *
 *    Disarm
 *
 *     Mark the drawnbutton as unarmed (i.e. active).
 *     The foreground and background colors will revert to the 
 *     unarmed state if XmNinvertOnSelect is set to TRUE and the
 *     drawnbutton is not in a menu.
 *     The callbacks for XmNdisarmCallback are called..
 *
 ************************************************************************/

static void Disarm (db, event)
XmDrawnButtonWidget db;
XEvent *event;

{
   XmDrawnButtonCallbackStruct call_value;

   db -> drawnbutton.armed = FALSE;

   if (db->drawnbutton.disarm_callback)
   {
      XFlush(XtDisplay (db));

      call_value.reason = XmCR_DISARM;
      call_value.event = event;
      call_value.window = XtWindow (db);
      XtCallCallbackList ((Widget) db, db->drawnbutton.disarm_callback, &call_value);
   }
}


/************************************************************************
 *
 *  Enter
 *
 ************************************************************************/

static void Enter (db, event)
XmDrawnButtonWidget db;
XEvent * event;

{
   _XmPrimitiveEnter ((XmPrimitiveWidget)db, event);

   if (db -> drawnbutton.pushbutton_enabled &&
      db -> drawnbutton.armed == TRUE)
      DrawPushButton(db, TRUE);
}


/************************************************************************
 *
 *  Leave
 *
 ************************************************************************/

static void Leave (db, event)
XmDrawnButtonWidget db;
XEvent * event;

{
   _XmPrimitiveLeave ((XmPrimitiveWidget)db, event);

   if (db -> drawnbutton.pushbutton_enabled &&
      db -> drawnbutton.armed == TRUE)
      DrawPushButton(db, FALSE);
}


/************************************************************************
 *
 *  ClassPartInitialize
 *     Set up the fast subclassing for the widget
 *
 ************************************************************************/
static void ClassPartInitialize (wc)
WidgetClass wc;

{
   _XmFastSubclassInit (wc, XmDRAWN_BUTTON_BIT);
}

      
/*************************************<->*************************************
 *
 *  Initialize 
 *
 *************************************<->***********************************/
/*ARGSUSED*/
static void Initialize (request, new)
XmDrawnButtonWidget request, new;

{
   new->drawnbutton.armed = FALSE;
   new->drawnbutton.timer = 0;

    /* if menuProcs is not set up yet, try again */
    if (xmLabelClassRec.label_class.menuProcs == NULL)
	xmLabelClassRec.label_class.menuProcs =
	    (XmMenuProc) _XmGetMenuProcContext();

   if (new -> drawnbutton.shadow_type != XmSHADOW_IN   &&
       new -> drawnbutton.shadow_type != XmSHADOW_OUT &&
       new -> drawnbutton.shadow_type != XmSHADOW_ETCHED_IN &&
       new -> drawnbutton.shadow_type != XmSHADOW_ETCHED_OUT)
   {
      _XmWarning (new, MESSAGE1);
      new -> drawnbutton.shadow_type = XmSHADOW_ETCHED_IN;
   }

/* not used now, maybe later...
   new -> drawnbutton.old_width = new -> core.width;
   new -> drawnbutton.old_height = new -> core.height;
   new -> drawnbutton.old_highlight_thickness = new -> primitive.highlight_thickness;
   new -> drawnbutton.old_shadow_thickness = new -> primitive.shadow_thickness;
*/
			 
}

/*************************************<->*************************************
 *
 *  Resize (db)
 *
 *************************************<->***********************************/
static void Resize (db)
XmDrawnButtonWidget db;
{
   XmDrawnButtonCallbackStruct call_value;

   if (XtIsRealized(db)) 
   {
      (* xmLabelClassRec.core_class.resize) (db);
   }

/* not used now, maybe later....
   db->drawnbutton.old_width = db->core.width;
   db->drawnbutton.old_height = db->core.height;
   db->drawnbutton.old_shadow_thickness = db->primitive.shadow_thickness;
   db->drawnbutton.old_highlight_thickness = db->primitive.highlight_thickness;
*/

   if (db->drawnbutton.resize_callback)
   {
      XFlush(XtDisplay (db));
      call_value.reason = XmCR_RESIZE;
      call_value.event = NULL;
      call_value.window = XtWindow (db);
      XtCallCallbackList ((Widget) db, db->drawnbutton.resize_callback, &call_value);
   }
}


/*************************************<->*************************************
 *
 *  Redisplay (db, event, region)
 *
 *************************************<->***********************************/
/*ARGSUSED*/
static void Redisplay (db, event, region)
XmDrawnButtonWidget db;
XEvent *event;
Region region;

{
   XmDrawnButtonCallbackStruct call_value;


   if (XtIsRealized(db)) 
   {
      if (db->drawnbutton.pushbutton_enabled)
	 DrawPushButton(db, db->drawnbutton.armed);

      else
	 _XmDrawShadowType((Widget)db, db->drawnbutton.shadow_type,
			   db -> core.width,
			   db -> core.height,			   
			   db -> primitive.shadow_thickness,
			   db -> primitive.highlight_thickness,
			   db -> primitive.top_shadow_GC,
			   db -> primitive.bottom_shadow_GC);


      if (event && db->drawnbutton.expose_callback)
      {
         XFlush(XtDisplay (db));

	 call_value.reason = XmCR_EXPOSE;
	 call_value.event = event;
	 call_value.window = XtWindow (db);
	 XtCallCallbackList ((Widget) db, db->drawnbutton.expose_callback, &call_value);
      }

      if (event) (* xmLabelClassRec.core_class.expose) (db, NULL, NULL);

   }
}




static void DrawPushButton(db, armed)
XmDrawnButtonWidget db;
Boolean             armed;
{

   if (db->primitive.shadow_thickness > 0)
      _XmDrawShadow (XtDisplay (db), XtWindow (db), 
	    (armed
		? db -> primitive.bottom_shadow_GC
		: db -> primitive.top_shadow_GC),
	    (armed 
		? db -> primitive.top_shadow_GC 
		: db -> primitive.bottom_shadow_GC), 
	      db -> primitive.shadow_thickness,
	      db -> primitive.highlight_thickness,
	      db -> primitive.highlight_thickness,
	      db -> core.width - 2 * db->primitive.highlight_thickness,
	      db -> core.height - 2 * db->primitive.highlight_thickness);
}



/*************************************<->*************************************
 *
 *  SetValues(current, request, new)
 *
 *   Description:
 *   -----------
 *     This is the set values procedure for the drawnbutton class.  It is
 *     called last (the set values rtnes for its superclasses are called
 *     first).
 *
 *
 *   Inputs:
 *   ------
 *    current = original widget;
 *    request = original copy of request;
 *    new = copy of request which reflects changes made to it by
 *          set values procedures of its superclasses;
 *    last = TRUE if this is the last set values procedure to be called.
 * 
 *   Outputs:
 *   -------
 *
 *   Procedures Called
 *   -----------------
 *
 *************************************<->***********************************/
/*ARGSUSED*/
static Boolean SetValues(current, request, new)
XmDrawnButtonWidget current, request, new;
{

   Boolean  flag = FALSE;    /* our return value */

    /*  Check the data put into the new widget.  */

   if (new -> drawnbutton.shadow_type != XmSHADOW_IN   &&
       new -> drawnbutton.shadow_type != XmSHADOW_OUT &&
       new -> drawnbutton.shadow_type != XmSHADOW_ETCHED_IN &&
       new -> drawnbutton.shadow_type != XmSHADOW_ETCHED_OUT)
   {
      _XmWarning (new, MESSAGE1);
      new -> drawnbutton.shadow_type = XmSHADOW_ETCHED_IN;
   }

   if (new -> drawnbutton.shadow_type != current-> drawnbutton.shadow_type ||
       new -> primitive.foreground != current -> primitive.foreground    ||
       new -> core.background_pixel != current -> core.background_pixel  ||
       new -> primitive.highlight_thickness != 
       current -> primitive.highlight_thickness                          ||
       new -> primitive.shadow_thickness !=
       current -> primitive.shadow_thickness)
   {
      flag = TRUE;
   }

   return(flag);
}




/*************************************************************************
 *
 *  Realize
 *	This function sets the bit gravity to forget.
 *
 *************************************************************************/

static void Realize (w, p_valueMask, attributes)
Widget w;
Mask * p_valueMask;
XSetWindowAttributes *attributes;

{
   Mask valueMask = *p_valueMask;

   valueMask |= CWBitGravity | CWDontPropagate;
   attributes->bit_gravity = ForgetGravity;
   attributes->do_not_propagate_mask =
      ButtonPressMask | ButtonReleaseMask |
      KeyPressMask | KeyReleaseMask | PointerMotionMask;

   XtCreateWindow (w, InputOutput, CopyFromParent, valueMask, attributes);
}



/************************************************************************
 *
 *  Destroy
 *	Clean up allocated resources when the widget is destroyed.
 *
 ************************************************************************/

static void Destroy (db)
XmDrawnButtonWidget db;

{
   if (db->drawnbutton.timer)
       XtRemoveTimeOut (db->drawnbutton.timer);

   XtRemoveAllCallbacks (db, XmNactivateCallback);
   XtRemoveAllCallbacks (db, XmNarmCallback);
   XtRemoveAllCallbacks (db, XmNdisarmCallback);
   XtRemoveAllCallbacks (db, XmNresizeCallback);
   XtRemoveAllCallbacks (db, XmNexposeCallback);
}


/************************************************************************
 *
 *		Application Accessible External Functions
 *
 ************************************************************************/


/************************************************************************
 *
 *  XmCreateDrawnButton
 *	Create an instance of a drawnbutton and return the widget id.
 *
 ************************************************************************/

#ifdef _NO_PROTO
Widget XmCreateDrawnButton (parent, name, arglist, argcount)
Widget parent;
char * name;
ArgList arglist;
Cardinal argcount;

#else /* _NO_PROTO */
Widget XmCreateDrawnButton (Widget parent, char *name, ArgList arglist, Cardinal argcount)
#endif /* _NO_PROTO */
{
   return (XtCreateWidget (name, xmDrawnButtonWidgetClass, 
                           parent, arglist, argcount));
}


