#ifdef REV_INFO
#ifndef lint
static char SCCSID[] = "OSF/Motif: @(#)PushB.c	3.40.1.1 91/02/21";
#endif /* lint */
#endif /* REV_INFO */
/******************************************************************************
*******************************************************************************
*
*  (c) Copyright 1989, 1990, 1991 OPEN SOFTWARE FOUNDATION, INC.
*  (c) Copyright 1989, 1990 DIGITAL EQUIPMENT CORPORATION, MAYNARD, MASS.
*  (c) Copyright 1987, 1988, 1989, 1990, HEWLETT-PACKARD COMPANY
*  (c) Copyright 1988 MASSACHUSETTS INSTITUTE OF TECHNOLOGY 
*  (c) Copyright 1988 MICROSOFT CORPORATION
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
#include <Xm/PushBP.h>
#include <Xm/CascadeB.h>
   
#define DELAY_DEFAULT 100	

static Boolean SetValues();
static void ClassInitialize();
static void ClassPartInitialize();
static void Initialize();
static void GetFillGC();
static void GetBackgroundGC();
static void Arm();
static void MultiArm();
static void Disarm();
static void Activate();
static void MultiActivate();
static void ActivateCommon();
static void Redisplay();
static void Enter();
static void Leave();
static void BtnDown();
static void BtnUp();
static void BorderHighlight();
static void BorderUnhighlight();
static void Destroy();
static void Help();
static void ArmAndActivate();
static XtTimerCallbackProc ArmTimeout();
static void KeySelect();

static void  DrawPushButtonBackground ();
static void  DrawPushButtonLabel();
static void  DrawPushButtonShadows();
static Boolean ComputePBArea ();
static Boolean ComputePBLabelArea();
static GC GetParentBackgroundGC();
static void DrawPBPrimitiveShadows();
static void DrawDefaultButtonShadows();
static void EraseDefaultButtonShadows();
static XmImportOperator ShowAsDef_ToHorizPix() ;
static  void ExportHighlightThickness ();

void _XmClearBCompatibility ();

static int AdjustHighLightThickness ();

static void UnhighlightBorder();
struct  PBTimeOutEvent
		{  XmPushButtonWidget  pushbutton;
	       XEvent			   *xevent;
		} ;

extern _XmSelectColorDefault();


/*************************************<->*************************************
 *
 *
 *   Description:   translation tables for class: PushButton
 *   -----------
 *
 *   Matches events with string descriptors for internal routines.
 *
 *************************************<->***********************************/
static XtTranslations default_parsed;

#ifndef MCCABE
static char defaultTranslations[] =
   "<Btn1Down>:         Arm()\n\
    <Btn1Down>,<Btn1Up>: Activate()\
			Disarm()\n\
    <Btn1Down>(2+):     MultiArm()\n\
    <Btn1Up>(2+):       MultiActivate()\n\
    <Btn1Up>:           Activate()\
		        Disarm()\n\
    <Key>osfSelect:  	ArmAndActivate()\n\
    <Key>osfActivate:   ArmAndActivate()\n\
    <Key>osfHelp:	Help()\n\
    ~Shift ~Meta ~Alt <Key>Return:	ArmAndActivate()\n\
    ~Shift ~Meta ~Alt <Key>space:	ArmAndActivate()\n\
    <EnterWindow>:      Enter()\n\
    <LeaveWindow>:      Leave()";
#else
static char defaultTranslations[];
#endif

static XtTranslations menu_parsed;

#ifndef MCCABE
static char menuTranslations[] =
   "<BtnDown>:          BtnDown()\n\
    <BtnUp>:            BtnUp()\n\
    <Key>osfSelect:	ArmAndActivate()\n\
    <Key>osfActivate:	ArmAndActivate()\n\
    <Key>osfCancel:	MenuShellPopdownOne()\n\
    ~Shift ~Meta ~Alt<Key>Return:	ArmAndActivate()\n\
    ~Shift ~Meta ~Alt <Key>space:	ArmAndActivate()\n\
    <EnterWindow>:      Enter()\n\
    <LeaveWindow>:      Leave()";
#else
static char menuTranslations[];
#endif




/*************************************<->*************************************
 *
 *
 *   Description:  action list for class: PushButton
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
  {"MultiArm", (XtActionProc) MultiArm },
  {"Activate", 	(XtActionProc) Activate	 },
  {"MultiActivate", 	(XtActionProc) MultiActivate	 },
  {"ArmAndActivate", (XtActionProc) ArmAndActivate },
  {"Disarm", 	(XtActionProc) Disarm		 },
  {"BtnDown", 	(XtActionProc) BtnDown		 },
  {"BtnUp", 	(XtActionProc) BtnUp		 },
  {"Enter", 	(XtActionProc) Enter		 },
  {"Leave",	(XtActionProc) Leave		 },
  {"KeySelect",	(XtActionProc) KeySelect	 },
  {"Help",	(XtActionProc) Help		 },
};

/* Definition for resources that need special processing in get values */

static XmSyntheticResource syn_resources[] =
{
  {
     XmNshowAsDefault, 
     sizeof (Dimension),
     XtOffset (XmPushButtonWidget, pushbutton.show_as_default),
     _XmFromHorizontalPixels,
     ShowAsDef_ToHorizPix
  },

  {
     XmNdefaultButtonShadowThickness,
     sizeof (Dimension),
     XtOffset (XmPushButtonWidget, pushbutton.default_button_shadow_thickness ),
     _XmFromHorizontalPixels,  
     _XmToHorizontalPixels
  },

   {
     XmNhighlightThickness,
     sizeof (Dimension),
     XtOffset (XmPrimitiveWidget, primitive.highlight_thickness),
     ExportHighlightThickness,
     _XmToHorizontalPixels
   },

};


/*  The resource list for Push Button  */

static XtResource resources[] = 
{     
   {
     XmNmultiClick,
     XmCMultiClick,
     XmRMultiClick,
     sizeof (unsigned char),
     XtOffset (XmPushButtonWidget, pushbutton.multiClick),
     XmRImmediate, (caddr_t) XmMULTICLICK_KEEP
   },

   {
     XmNfillOnArm,
     XmCFillOnArm,
     XmRBoolean,
     sizeof (Boolean),
     XtOffset (XmPushButtonWidget, pushbutton.fill_on_arm),
     XmRImmediate, (caddr_t) True
   },

   {
     XmNarmColor,
     XmCArmColor,
     XmRPixel,
     sizeof (Pixel),
     XtOffset (XmPushButtonWidget, pushbutton.arm_color),
     XmRCallProc, (caddr_t) _XmSelectColorDefault
   },

   {
     XmNarmPixmap,
     XmCArmPixmap,
     XmRPrimForegroundPixmap,
     sizeof (Pixmap),
     XtOffset (XmPushButtonWidget, pushbutton.arm_pixmap),
     XmRImmediate, (caddr_t) XmUNSPECIFIED_PIXMAP
   },

   {
     XmNshowAsDefault,
     XmCShowAsDefault,
     XmRBooleanDimension,
     sizeof (Dimension),
     XtOffset (XmPushButtonWidget, pushbutton.show_as_default),
     XmRImmediate, (caddr_t) 0
   },

   {
     XmNactivateCallback,
     XmCCallback,
     XmRCallback,
     sizeof(XtCallbackList),
     XtOffset (XmPushButtonWidget, pushbutton.activate_callback),
     XmRPointer, (caddr_t) NULL
   },

   {
     XmNarmCallback,
     XmCCallback,
     XmRCallback,
     sizeof(XtCallbackList),
     XtOffset (XmPushButtonWidget, pushbutton.arm_callback),
     XmRPointer, (caddr_t) NULL
   },

   {
     XmNdisarmCallback,
     XmCCallback,
     XmRCallback,
     sizeof(XtCallbackList),
     XtOffset (XmPushButtonWidget, pushbutton.disarm_callback),
     XmRPointer, (caddr_t) NULL
   },
   
   {
     XmNshadowThickness,
     XmCShadowThickness,
     XmRHorizontalDimension,
     sizeof(Dimension),
     XtOffset (XmPushButtonWidget, primitive.shadow_thickness),
     XmRImmediate, (caddr_t) 2
   },

   {
     XmNdefaultButtonShadowThickness,
     XmCDefaultButtonShadowThickness, 
     XmRHorizontalDimension, 
     sizeof (Dimension),
     XtOffset (XmPushButtonWidget, pushbutton.default_button_shadow_thickness ),
     XmRImmediate,
     (caddr_t) 0
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



/*************************************<->*************************************
 *
 *
 *   Description:  global class record for instances of class: PushButton
 *   -----------
 *
 *   Defines default field settings for this class record.
 *
 *************************************<->***********************************/

externaldef(xmpushbuttonclassrec)  
	XmPushButtonClassRec xmPushButtonClassRec = {
  {
/* core_class record */	
    /* superclass	  */	(WidgetClass) &xmLabelClassRec,
    /* class_name	  */	"XmPushButton",
    /* widget_size	  */	sizeof(XmPushButtonRec),
    /* class_initialize   */    ClassInitialize,
    /* class_part_init    */    ClassPartInitialize,
    /* class_inited       */	FALSE,
    /* initialize	  */	Initialize,
    /* initialize_hook    */    NULL,
    /* realize		  */	_XtInherit,
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
    /* resize		  */	_XtInherit,
    /* expose		  */	Redisplay,
    /* set_values	  */	SetValues,
    /* set_values_hook    */    NULL,
    /* set_values_almost  */    XtInheritSetValuesAlmost,
    /* get_values_hook    */	NULL,
    /* accept_focus	  */	NULL,
    /* version            */	XtVersion,
    /* callback_private   */    NULL,
    /* tm_table           */    NULL,
    /* query_geometry     */	XtInheritQueryGeometry, 
    /* display_accelerator */   NULL,
    /* extension          */    NULL,
  },

  { /* primitive_class record       */

    /* Primitive border_highlight   */	BorderHighlight,
    /* Primitive border_unhighlight */	BorderUnhighlight,
    /* translations		    */  XtInheritTranslations,
									/* (XtTranslations)_XtInherit, */
    /* arm_and_activate		    */  ArmAndActivate,
    /* get resources		    */  syn_resources,
    /* num get_resources	    */  XtNumber(syn_resources),
    /* extension		    */  NULL,
  },

  { /* label_class record */
 
    /* setOverrideCallback*/	_XtInherit,
    /* menu procedures    */	(XmMenuProc)_XtInherit,			
    /* menu traversal xlation */ XtInheritTranslations,
    /* extension	  */	NULL,
  }

};
externaldef(xmpushbuttonwidgetclass)
   WidgetClass xmPushButtonWidgetClass = (WidgetClass)&xmPushButtonClassRec;


/************************************************************************
 *
 *     Arm
 *
 *     This function processes button 1 down occuring on the pushbutton.
 *     Mark the pushbutton as armed (i.e. active).
 *     The callbacks for XmNarmCallback are called.
 *
 ************************************************************************/

static void Arm (pb, event)
XmPushButtonWidget pb;
XEvent *event;

{
   XmPushButtonCallbackStruct call_value;

   (void) XmProcessTraversal((Widget)pb, XmTRAVERSE_CURRENT);

   pb -> pushbutton.armed = TRUE;
   pb -> pushbutton.armTimeStamp = event->xbutton.time; /* see MultiActivate */

   Redisplay(pb, event, FALSE);

   if (pb->pushbutton.arm_callback)
   {
      XFlush(XtDisplay (pb));

      call_value.reason = XmCR_ARM;
      call_value.event = event;
      XtCallCallbackList ((Widget) pb, pb->pushbutton.arm_callback, &call_value);
   }
}

static void MultiArm (pb, event)
XmPushButtonWidget pb;
XEvent *event;

{

    if (pb->pushbutton.multiClick == XmMULTICLICK_KEEP)
            Arm (pb,event);
}



/************************************************************************
 *
 *     Activate
 *
 *     Mark the pushbutton as unarmed (i.e. inactive).
 *     If the button release occurs inside of the PushButton, the 
 *     callbacks for XmNactivateCallback are called.
 *
 ************************************************************************/

static void Activate (pb, buttonEvent)
XmPushButtonWidget pb;
XButtonPressedEvent * buttonEvent;
{
   pb->pushbutton.click_count = 1;
   ActivateCommon (pb, buttonEvent);
}

static void MultiActivate (pb, buttonEvent)
XmPushButtonWidget pb;
XButtonPressedEvent * buttonEvent;
{
   /* When a multi click sequence occurs and the user Button Presses and
    * holds for a length of time, the final release should look like a
    * new/separate activate.
    */
   if (pb->pushbutton.multiClick == XmMULTICLICK_KEEP)
  {
     if ((buttonEvent->time - pb->pushbutton.armTimeStamp) >
        XtGetMultiClickTime(XtDisplay(pb)))
          pb->pushbutton.click_count = 1;
      else
        pb->pushbutton.click_count++;
     ActivateCommon (pb, buttonEvent);
     Disarm (pb, buttonEvent);
  }
}

static void ActivateCommon (pb, event)
XmPushButtonWidget pb;
XEvent * event;

{
   XButtonEvent *buttonEvent = (XButtonEvent *) event;
   XmPushButtonCallbackStruct call_value;
   Dimension bw = pb->core.border_width ;

   pb -> pushbutton.armed = FALSE;

   Redisplay(pb, event, FALSE);

   if ((buttonEvent->x >= (int)-bw) &&
       (buttonEvent->x < (int)(pb->core.width + bw)) &&
       (buttonEvent->y >= (int)-bw) &&
       (buttonEvent->y < (int)(pb->core.height + bw)))
   {
       call_value.reason = XmCR_ACTIVATE;
       call_value.event = event;
       call_value.click_count = pb->pushbutton.click_count;

       if ((pb->pushbutton.multiClick == XmMULTICLICK_DISCARD) &&	
	   (call_value.click_count > 1)) {
           return;
	}

       /* if the parent is a RowColumn, notify it about the select */
       if (XmIsRowColumn(XtParent(pb)))
       {
	  (* xmLabelClassRec.label_class.menuProcs) (XmMENU_CALLBACK, 
						     XtParent(pb), FALSE, 
						     (caddr_t)pb,
						     (caddr_t)&call_value);
       }

       if ((! pb->label.skipCallback) &&
	   (pb->pushbutton.activate_callback))
       {
	  XFlush (XtDisplay (pb));
	  XtCallCallbackList ((Widget) pb, pb->pushbutton.activate_callback,
				 &call_value);
       }
    }
}

/************************************************************************
 *
 *     ArmAndActivate
 *
 ************************************************************************/

static void ArmAndActivate (pb, event)
XmPushButtonWidget pb;
XEvent *event;

{
   Boolean already_armed = pb -> pushbutton.armed;

   XmPushButtonCallbackStruct call_value;

   if (pb ->label.menu_type == XmMENU_PULLDOWN ||
       pb ->label.menu_type == XmMENU_POPUP)
   {
      pb -> pushbutton.armed = FALSE;
      (* xmLabelClassRec.label_class.menuProcs)
	  (XmMENU_POPDOWN, XtParent(pb), NULL, (caddr_t)event, NULL);
   }
   else 
   {
      pb -> pushbutton.armed = TRUE;
      Redisplay (pb, event, FALSE);      
   }

   XFlush (XtDisplay (pb));

   if (pb->pushbutton.arm_callback && !already_armed)
   {
      call_value.reason = XmCR_ARM;
      call_value.event = event;
      XtCallCallbackList ((Widget)pb, pb->pushbutton.arm_callback, &call_value);
   }

   call_value.reason = XmCR_ACTIVATE;
   call_value.event = event;
   call_value.click_count = 1;	           /* always 1 in kselect */

   /* if the parent is a RowColumn, notify it about the select */
   if (XmIsRowColumn(XtParent(pb)))
   {
      (* xmLabelClassRec.label_class.menuProcs) (XmMENU_CALLBACK, 
					XtParent(pb), FALSE, (caddr_t)pb,
					(caddr_t)&call_value);
   }

   if ((! pb->label.skipCallback) &&
       (pb->pushbutton.activate_callback))
   {
      XFlush (XtDisplay (pb));
      XtCallCallbackList ((Widget) pb, pb->pushbutton.activate_callback,
			     &call_value);
   }

   pb -> pushbutton.armed = FALSE;

   if (pb->pushbutton.disarm_callback)
   {
      XFlush (XtDisplay (pb));
      call_value.reason = XmCR_DISARM;
      XtCallCallbackList ((Widget) pb, pb->pushbutton.disarm_callback,
			     &call_value);
   }

   /* If the button is still around, show it released, after a short delay */

   if ((pb->core.being_destroyed == False) &&
       (pb ->label.menu_type != XmMENU_PULLDOWN) &&
       (pb ->label.menu_type != XmMENU_POPUP))  
   {
      pb->pushbutton.timer = XtAppAddTimeOut(XtWidgetToApplicationContext(pb),
						(unsigned long) DELAY_DEFAULT,
                         			ArmTimeout, 
						(caddr_t)(pb));
   }
}

/*ARGSUSED*/
static XtTimerCallbackProc ArmTimeout (pb, id)
XmPushButtonWidget pb;
XtIntervalId *id;
{
   pb -> pushbutton.timer = 0;
   if (XtIsRealized (pb) && XtIsManaged (pb)) {
      Redisplay (pb, NULL, FALSE);
      XFlush (XtDisplay (pb));
   }
   return;
}



/************************************************************************
 *
 *    Disarm
 *
 *     Mark the pushbutton as unarmed (i.e. active).
 *     The callbacks for XmNdisarmCallback are called..
 *
 ************************************************************************/

static void Disarm (pb, event)
XmPushButtonWidget pb;
XEvent *event;

{
   XmPushButtonCallbackStruct call_value;

   pb -> pushbutton.armed = FALSE;

   if (pb->pushbutton.disarm_callback)
   {
      call_value.reason = XmCR_DISARM;
      call_value.event = event;
      XtCallCallbackList ((Widget) pb, pb->pushbutton.disarm_callback, &call_value);
   }
}


/************************************************************************
 *
 *     BtnDown
 *
 *     This function processes a button down occuring on the pushbutton
 *     when it is in a popup, pulldown, or option menu.
 *     Popdown the posted menu.
 *     Turn parent's traversal off.
 *     Mark the pushbutton as armed (i.e. active).
 *     The callbacks for XmNarmCallback are called.
 *
 ************************************************************************/

static void BtnDown (pb, event)
XmPushButtonWidget pb;
XEvent *event;

{
   XButtonEvent *buttonEvent = (XButtonEvent *) event;
   XmPushButtonCallbackStruct call_value;
   Boolean validButton;
   Boolean already_armed;

   if (event && (event->type == ButtonPress))
       (* xmLabelClassRec.label_class.menuProcs) (XmMENU_BUTTON,
						  XtParent(pb), NULL, (caddr_t)event,
						  &validButton);
   
   if (!validButton)
       return;

   /* Popdown other popups that may be up */
   if (XtParent(XtParent(pb))->core.num_popups != 0)
   {
      ShellWidget popup = (ShellWidget)
	                   XtParent(XtParent(pb))->core.popup_list[0];
      
      if (popup->shell.popped_up)
	  (* xmLabelClassRec.label_class.menuProcs)
	      (XmMENU_SHELL_POPDOWN, (Widget)popup, NULL, (caddr_t)event, NULL);
      else
      {
          /* If the shell is not popped up but the cascade button is
           * highlighted, then unhighlight it.
           */
          Widget w = ((XmManagerWidget)XtParent(pb))->manager.active_child;
          if (w && ((XmIsCascadeButton(w) || XmIsCascadeButtonGadget(w))))
              XmCascadeButtonHighlight (w, FALSE);
      }
   }

   /* Set focus to this pushbutton.  This must follow the possible
    * unhighlighting of the CascadeButton else it'll screw up active_child.
    */
   (void)XmProcessTraversal((Widget)pb, XmTRAVERSE_CURRENT);
   /* get the location cursor - get consistent with Gadgets */


   (* xmLabelClassRec.label_class.menuProcs) (XmMENU_TRAVERSAL,
					      XtParent(pb), FALSE, NULL, NULL);

   if (pb->primitive.shadow_thickness > 0)
      _XmDrawShadow (XtDisplay (pb), XtWindow (pb), 
		 pb -> primitive.top_shadow_GC,
		 pb -> primitive.bottom_shadow_GC,
		 pb -> primitive.shadow_thickness,
		 pb -> primitive.highlight_thickness,
		 pb -> primitive.highlight_thickness,
		 pb -> core.width - 2 * pb->primitive.highlight_thickness,
		 pb -> core.height - 2 * pb->primitive.highlight_thickness);

   already_armed = pb -> pushbutton.armed;
   pb -> pushbutton.armed = TRUE;

   if (pb->pushbutton.arm_callback && !already_armed)
   {
      XFlush (XtDisplay (pb));

      call_value.reason = XmCR_ARM;
      call_value.event = event;
      XtCallCallbackList ((Widget) pb, pb->pushbutton.arm_callback, &call_value);
   }
   _XmRecordEvent (event);
}




/************************************************************************
 *
 *     BtnUp
 *
 *     This function processes a button up occuring on the pushbutton
 *     when it is in a popup, pulldown, or option menu.
 *     Mark the pushbutton as unarmed (i.e. inactive).
 *     The callbacks for XmNactivateCallback are called.
 *     The callbacks for XmNdisarmCallback are called.
 *
 ************************************************************************/

static void BtnUp (pb, event)
XmPushButtonWidget pb;
XEvent *event;

{
   XButtonEvent *buttonEvent = (XButtonEvent *) event;
   Widget parent =  XtParent(pb);
   XmPushButtonCallbackStruct call_value;
   Boolean flushDone = False;
   Boolean validButton;

   if (event && (event->type == ButtonRelease))
       (* xmLabelClassRec.label_class.menuProcs) (XmMENU_BUTTON,
						  XtParent(pb), NULL, (caddr_t)event,
						  &validButton);
   
   if (!validButton)
       return;

   pb -> pushbutton.armed = FALSE;

   (* xmLabelClassRec.label_class.menuProcs)
       (XmMENU_POPDOWN, XtParent(pb), NULL, (caddr_t)event, NULL);

   _XmRecordEvent(event);

    call_value.reason = XmCR_ACTIVATE;
    call_value.event = event;

   /* if the parent is a RowColumn, notify it about the select */
   if (XmIsRowColumn(XtParent(pb)))
   {
      (* xmLabelClassRec.label_class.menuProcs) (XmMENU_CALLBACK, 
						 XtParent(pb), FALSE, (caddr_t)pb,
						 (caddr_t)&call_value);
      flushDone = True;
   }
   
   if ((! pb->label.skipCallback) &&
       (pb->pushbutton.activate_callback))
   {
      XFlush (XtDisplay (pb));
      flushDone = True;
      XtCallCallbackList ((Widget) pb, pb->pushbutton.activate_callback,
			     &call_value);
   }
   if (pb->pushbutton.disarm_callback)
   {
      if (!flushDone)
	  XFlush (XtDisplay (pb));
      call_value.reason = XmCR_DISARM;
      call_value.event = event;
      XtCallCallbackList ((Widget) pb, pb->pushbutton.disarm_callback, &call_value);
   }
}




/************************************************************************
 *
 *  Enter
 *
 ************************************************************************/

static void Enter (pb, event)
XmPushButtonWidget pb;
XEvent * event;

{
   XmPushButtonCallbackStruct call_value;


   if (pb ->label.menu_type == XmMENU_PULLDOWN ||
       pb ->label.menu_type == XmMENU_POPUP)
   {
      if ((((ShellWidget) XtParent(XtParent(pb)))->shell.popped_up) &&
          (! ((XmManagerWidget) XtParent(pb))->manager.traversal_on))
      {
	 if (pb->primitive.shadow_thickness > 0)
	    _XmDrawShadow (XtDisplay (pb), XtWindow (pb), 
		       pb -> primitive.top_shadow_GC,
	               pb -> primitive.bottom_shadow_GC,
		       pb -> primitive.shadow_thickness,
		       pb -> primitive.highlight_thickness,
		       pb -> primitive.highlight_thickness,
		       pb -> core.width - 2 * pb->primitive.highlight_thickness,
		       pb -> core.height - 2 * pb->primitive.highlight_thickness);

         pb -> pushbutton.armed = TRUE;

         if (pb->pushbutton.arm_callback)
         {
            XFlush (XtDisplay (pb));

            call_value.reason = XmCR_ARM;
            call_value.event = event;
            XtCallCallbackList ((Widget) pb, pb->pushbutton.arm_callback, &call_value);
         }
      }
   }  
   else 
   {
      _XmPrimitiveEnter ((XmPrimitiveWidget)pb, event);
      if (pb -> pushbutton.armed == TRUE)
         Redisplay (pb,event, FALSE);      
   }
}


/************************************************************************
 *
 *  Leave
 *
 ************************************************************************/

static void Leave (pb, event)
XmPushButtonWidget pb;
XEvent * event;

{
   XmPushButtonCallbackStruct call_value;

   if (pb ->label.menu_type == XmMENU_PULLDOWN ||
       pb ->label.menu_type == XmMENU_POPUP)
   {
      if (! (((XmManagerWidget) XtParent(pb))->manager.traversal_on) &&
         (pb -> pushbutton.armed))
      {
         if (pb->primitive.shadow_thickness > 0)
	    _XmEraseShadow (XtDisplay (pb), XtWindow (pb), 
		       pb -> primitive.shadow_thickness,
		       pb -> primitive.highlight_thickness,
		       pb -> primitive.highlight_thickness,
		       pb -> core.width - 2 * pb->primitive.highlight_thickness,
		       pb -> core.height - 2 * pb->primitive.highlight_thickness);

         pb -> pushbutton.armed = FALSE;

         if (pb->pushbutton.disarm_callback)
         {
	    XFlush (XtDisplay (pb));

	    call_value.reason = XmCR_DISARM;
	    call_value.event = event;
	    XtCallCallbackList ((Widget) pb, pb->pushbutton.disarm_callback, &call_value);
         }
      }
   }
   else 
   {
      _XmPrimitiveLeave ((XmPrimitiveWidget)pb, event);

      if (pb -> pushbutton.armed == TRUE)
      {
	 pb -> pushbutton.armed = FALSE;
         Redisplay (pb,event, FALSE);      
	 pb -> pushbutton.armed = TRUE;
      }
   }
}


/*************************************<->*************************************
 *
 *  BorderHighlight 
 *
 *************************************<->***********************************/
static void BorderHighlight (pb)
XmPushButtonWidget pb;
{
   XmPushButtonCallbackStruct call_value;
   XEvent * event = NULL;

   if (pb ->label.menu_type == XmMENU_PULLDOWN ||
       pb ->label.menu_type == XmMENU_POPUP)
   {
      if (pb->primitive.shadow_thickness > 0)
	 _XmDrawShadow (XtDisplay (pb), XtWindow (pb), 
		    pb -> primitive.top_shadow_GC,
		    pb -> primitive.bottom_shadow_GC,
		    pb -> primitive.shadow_thickness,
		    pb -> primitive.highlight_thickness,
		    pb -> primitive.highlight_thickness,
		    pb -> core.width - 2 * pb->primitive.highlight_thickness,
		    pb -> core.height - 2 * pb->primitive.highlight_thickness);

      pb -> pushbutton.armed = TRUE;

      if (pb->pushbutton.arm_callback)
      {
	 XFlush (XtDisplay (pb));

	 call_value.reason = XmCR_ARM;
	 call_value.event = event;
	 XtCallCallbackList ((Widget) pb, pb->pushbutton.arm_callback, &call_value);
      }
   }
   else _XmHighlightBorder((Widget)pb);
       
}


/*************************************<->*************************************
 *
 *  BorderUnhighlight
 *
 *************************************<->***********************************/
static void BorderUnhighlight (pb)
XmPushButtonWidget pb;
{
   XmPushButtonCallbackStruct call_value;
   XEvent * event = NULL;

   if (pb ->label.menu_type == XmMENU_PULLDOWN ||
       pb ->label.menu_type == XmMENU_POPUP)
   {
      if (!pb -> pushbutton.armed)
         return;
      if (pb->primitive.shadow_thickness > 0)
	 _XmEraseShadow (XtDisplay (pb), XtWindow (pb), 
		    pb -> primitive.shadow_thickness,
		    pb -> primitive.highlight_thickness,
		    pb -> primitive.highlight_thickness,
		    pb -> core.width - 2 * pb->primitive.highlight_thickness,
		    pb -> core.height - 2 * pb->primitive.highlight_thickness);

      pb -> pushbutton.armed = FALSE;

      if (pb->pushbutton.disarm_callback)
      {
	 XFlush (XtDisplay (pb));

	 call_value.reason = XmCR_DISARM;
	 call_value.event = event;
	 XtCallCallbackList ((Widget) pb, pb->pushbutton.disarm_callback, &call_value);
      }
   }
   else 
	/* PushButton is not in a menu - parent may be a shell or manager */
    { if (XmIsManager (pb->core.parent))  _XmUnhighlightBorder((Widget)pb);
       else
	 	/* Has a shell parent */
             UnhighlightBorder(pb);
	}
}

/*************************************<->*************************************
 *
 *  KeySelect
 *
 *  If the menu system traversal is enabled, do an activate and disarm
 *
 *************************************<->***********************************/
static void KeySelect (pb, event)
XmPushButtonWidget pb;
XEvent *event;
{
   XmManagerWidget parent =  (XmManagerWidget) XtParent(pb);
   XmPushButtonCallbackStruct call_value;

   if (!_XmIsEventUnique(event))
      return;

   if (parent -> manager.traversal_on)
   {

      pb -> pushbutton.armed = FALSE;

      (* xmLabelClassRec.label_class.menuProcs)
	  (XmMENU_POPDOWN, XtParent(pb), NULL, (caddr_t)event, NULL);

      _XmRecordEvent(event);
      
      call_value.reason = XmCR_ACTIVATE;
      call_value.event = event;

      /* if the parent is a RowColumn, notify it about the select */
      if (XmIsRowColumn(XtParent(pb)))
      {
	 (* xmLabelClassRec.label_class.menuProcs) (XmMENU_CALLBACK, 
						    XtParent(pb), FALSE, (caddr_t)pb,
						    (caddr_t)&call_value);
      }

      if ((! pb->label.skipCallback) &&
	  (pb->pushbutton.activate_callback))
      {
	 XFlush (XtDisplay (pb));
	 XtCallCallbackList ((Widget) pb, pb->pushbutton.activate_callback,
				&call_value);
      }
   }
}


/*************************************<->*************************************
 *
 *  ClassInitialize 
 *
 *************************************<->***********************************/
static void ClassInitialize ()
{
   /* parse the various translation tables */

   menu_parsed		= XtParseTranslationTable(menuTranslations);
   default_parsed	= XtParseTranslationTable(defaultTranslations);

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
   _XmFastSubclassInit (wc, XmPUSH_BUTTON_BIT);
}

      
/*************************************<->*************************************
 *
 *  Initialize 
 *
 *************************************<->***********************************/
static void Initialize (request, new)
XmPushButtonWidget request, new;

{
   int increase;	
   int adjustment = 0;  
   Widget parent = XtParent(new);

   /* if menuProcs is not set up yet, try again */
   if (xmLabelClassRec.label_class.menuProcs == NULL)
       xmLabelClassRec.label_class.menuProcs =
	   (XmMenuProc) _XmGetMenuProcContext();

/*
 * Fix to introduce Resource XmNdefaultBorderWidth and compatibility
 *  variable.
 *  if defaultBorderWidth > 0, the program knows about this resource
 *  and is therefore a Motif 1.1 program; otherwise it is a Motif 1.0
 *      program and old semantics of XmNshowAsDefault prevails.
 *  - Sankar 2/1/90.
 */
   if (new->pushbutton.default_button_shadow_thickness > 0)
		new->pushbutton.compatible = False;
      else 
                new->pushbutton.compatible = True;

/*
 * showAsDefault as boolean if compatibility is false (Motif 1.1) else
 *  treat it to indicate the thickness of defaultButtonShadow.
 */
   if (new->pushbutton.compatible)
      new->pushbutton.default_button_shadow_thickness 
			= new->pushbutton.show_as_default;

    new->pushbutton.armed = FALSE;
   new->pushbutton.timer = 0;

   /* no unarm_pixmap but do have an arm_pixmap, use that */
   if ((new->label.pixmap == XmUNSPECIFIED_PIXMAP) &&
       (new->pushbutton.arm_pixmap != XmUNSPECIFIED_PIXMAP))
   {
      new->label.pixmap = new->pushbutton.arm_pixmap;
      if (request->core.width == 0)
         new->core.width = 0;
      if (request->core.height == 0)
         new->core.height = 0;
      (* xmLabelClassRec.core_class.resize) (new);
   }

   new->pushbutton.unarm_pixmap = new->label.pixmap;

   if (new->pushbutton.default_button_shadow_thickness)
   {
    /*
     * Special hack for 3d enhancement of location cursor high light.
     *  - Make the box bigger . During drawing of location cursor
     *    make it smaller.  See in Primitive.c
     *  May be we should use the macro: G_HighLightThickness(pbgadget);
     */
     new->primitive.highlight_thickness += XM_3D_ENHANCE_PIXEL;
	 adjustment = XM_3D_ENHANCE_PIXEL;
     increase =  2 * new->pushbutton.default_button_shadow_thickness +
                     new->primitive.shadow_thickness;

	 increase += adjustment;
     if (request->core.width == 0)
     {
        new->core.width += 2 * increase;
        Lab_MarginLeft(new) += increase;
        Lab_MarginRight(new) += increase;
        Lab_TextRect_x(new) += increase;
     }
	  else  
	     /* Add the increase to the core to compensate for extra space */
      if ( increase != 0)
	   { new->core.width += (increase << 1);
	     Lab_MarginLeft(new) += increase;
        Lab_MarginRight(new) += increase;
        Lab_TextRect_x(new) += increase;
	  }

     if (request->core.height == 0)
     {
        new->core.height +=  2 * increase;
        Lab_MarginTop(new) += increase;
        Lab_MarginBottom(new) += increase;
        Lab_TextRect_y(new) += increase;
     }
      else
         /* Add the increase to the core to compensate for extra space */
	   if ( increase != 0)
         {  new->core.height += (increase << 1);
		    Lab_MarginTop(new) += increase;
		    Lab_MarginBottom(new) += increase;
		    Lab_TextRect_y(new) += increase;
		 }
   }
	
   if (new->label.menu_type  == XmMENU_POPUP ||
       new->label.menu_type  == XmMENU_PULLDOWN)
   {
      new->primitive.traversal_on = TRUE;
   }  

   /* get the right translations established if the user didn't supply them */

   if (new->core.tm.translations == NULL)
   {

      if (new ->label.menu_type == XmMENU_PULLDOWN ||
	  new ->label.menu_type == XmMENU_POPUP)
         new->core.tm.translations = menu_parsed;

      else 
      {
         new->core.tm.translations = default_parsed;
	 XtOverrideTranslations(new,
	      ((XmPrimitiveClassRec *)XtClass(new))->primitive_class.
                translations);	   
      }
   }
  
   if (new ->label.menu_type == XmMENU_PULLDOWN ||
       new ->label.menu_type == XmMENU_POPUP)
       XtOverrideTranslations(new, 
         ((XmLabelClassRec *)XtClass(new))->label_class.translations);
/*   else */
  /*
   * Initialize GC's for
   *   fill and background.
   */

   {
      GetFillGC (new);
      GetBackgroundGC (new);
   }
}




/************************************************************************
 *
 *  GetFillGC
 *     Get the graphics context used for filling in background of button.
 *
 ************************************************************************/

static void GetFillGC (pb)
XmPushButtonWidget pb;

{
   XGCValues values;
   XtGCMask  valueMask;

   valueMask = GCForeground | GCFillStyle;

   values.foreground = pb -> pushbutton.arm_color;
   values.fill_style = FillSolid;

   pb -> pushbutton.fill_gc = XtGetGC ((Widget) pb, valueMask, &values);
}



/************************************************************************
 *
 *  GetBackgroundGC
 *     Get the graphics context used for filling in background of 
 *     the pushbutton when not armed.
 *
 ************************************************************************/

static void GetBackgroundGC (pb)
XmPushButtonWidget pb;

{
        XGCValues       values;
        XtGCMask        valueMask;
        short             myindex;
        XFontStruct     *fs;

        valueMask = GCForeground | GCBackground | GCFont | GCGraphicsExposures;
			  
        _XmFontListSearch(pb->label.font,
                          XmSTRING_DEFAULT_CHARSET,
                          &myindex,
                          &fs);
        values.foreground = pb->core.background_pixel;
        values.background = pb->primitive.foreground;
	values.graphics_exposures = False;

        if (fs==NULL)
          valueMask &= ~GCFont;
        else
          values.font     = fs->fid;

        pb->pushbutton.background_gc = XtGetGC(pb,valueMask,&values);
}


/*************************************<->*************************************
 *
 *  SetValues(current, request, new)
 *
 *   Description:
 *   -----------
 *     This is the set values procedure for the pushbutton class.  It is
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

static Boolean SetValues(current, request, new)
XmPushButtonWidget current, request, new;

{
   int increase;
   Boolean  flag = FALSE;    /* our return value */
   int adjustment;

/*
 * Fix to introduce Resource XmNdefaultBorderWidth and compatibility
 *      variable.
 *  if the XmNdefaultBorderWidth resource in the current differ from the
 *  one in "new", then the programmer is setting this resource - so this
 *  is known to the programmer and hence it is a Motif1.1 program.
 *  If they are same then either it is a Motif 1.0 program or there has been
 *  no change in the resource (Motif 1.1 program). If it is a Motif 1.0 
 *  program, then we should copy the value of XmNshowAsDefault to the 
 *  XmNdefaultBorderWidth. If it is  Motif 1.1 program ( Compatible
 *  flag is flase) - then we should not do the copy.
 *  This logic will maintain the semantics of the  XmNshowAsDefault of Motif
 *  1.0. For a full explanation see the Design architecture document.
 *  - Sankar 2/2/90.
 */

   if ( (current->pushbutton.default_button_shadow_thickness) !=
	  (new->pushbutton.default_button_shadow_thickness) )
		new->pushbutton.compatible = False;

   if (new->pushbutton.compatible)
      new->pushbutton.default_button_shadow_thickness 
		= new->pushbutton.show_as_default;

   adjustment = AdjustHighLightThickness (new, current);

	/*
	 * Compute size change.
	 */
   if (new->pushbutton.default_button_shadow_thickness != 
		current->pushbutton.default_button_shadow_thickness)
   {


      if (new->pushbutton.default_button_shadow_thickness > 
		current->pushbutton.default_button_shadow_thickness)
      {
         if (current->pushbutton.default_button_shadow_thickness > 0)
            increase =  (2 * new->pushbutton.default_button_shadow_thickness +
                         new->primitive.shadow_thickness) -
                      (2 * current->pushbutton.default_button_shadow_thickness +
                         current->primitive.shadow_thickness);
         else
            increase =  (2 * new->pushbutton.default_button_shadow_thickness +
                         new->primitive.shadow_thickness);
      }
      else
      {
         if (new->pushbutton.default_button_shadow_thickness > 0)
        increase = - ((2 * current->pushbutton.default_button_shadow_thickness +
                           current->primitive.shadow_thickness) -
                          (2 * new->pushbutton.default_button_shadow_thickness +
                           new->primitive.shadow_thickness));
         else
        increase = - (2 * current->pushbutton.default_button_shadow_thickness +
                          current->primitive.shadow_thickness);
      }

	increase += adjustment;
      
      if (new->label.recompute_size || request->core.width == 0)
      {
	 new->core.width +=  (increase  << 1);
         Lab_MarginLeft(new) += increase;
         Lab_MarginRight(new) += increase;
         flag = TRUE;
      }
	  else
		/* add the change due to default button to the core */
	   if ( increase != 0)
	   {  new->core.width += (increase  << 1);
		  Lab_MarginLeft(new) += increase;
		  Lab_MarginRight(new) += increase;
	      flag = TRUE;
	   }

      if (new->label.recompute_size || request->core.height == 0)
      {
	     new->core.height += (increase << 1); 
         Lab_MarginTop(new) += increase;
         Lab_MarginBottom(new) += increase;
         flag = TRUE;
      }
      else
      /* add the change due to default button to the core */
       if ( increase != 0)
       {  new->core.height += (increase << 1);
		  Lab_MarginTop(new) += increase;
	      Lab_MarginBottom(new) += increase;
          flag = TRUE;
       }

   }

   if ((new->pushbutton.arm_pixmap != current->pushbutton.arm_pixmap) &&
      (new->label.label_type == XmPIXMAP) && (new->pushbutton.armed)) 
      flag = TRUE;
      
   /* no unarm_pixmap but do have an arm_pixmap, use that */
   if ((new->label.pixmap == XmUNSPECIFIED_PIXMAP) &&
       (new->pushbutton.arm_pixmap != XmUNSPECIFIED_PIXMAP))
   {
      new->label.pixmap = new->pushbutton.arm_pixmap;
      if (new->label.recompute_size &&
          request->core.width == current->core.width)
         new->core.width = 0;
      if (new->label.recompute_size &&
          request->core.height == current->core.height)
         new->core.width = 0;
      (* xmLabelClassRec.core_class.resize) (new);
   }

   if (new->label.pixmap != current->label.pixmap)
   {
      new->pushbutton.unarm_pixmap = new->label.pixmap;
      if ((new->label.label_type == XmPIXMAP) && (!new->pushbutton.armed))
	 flag = TRUE;
   }

   if ((new->pushbutton.fill_on_arm != current->pushbutton.fill_on_arm) &&
       (new->pushbutton.armed == TRUE))
	 flag = TRUE;

   if (new ->label.menu_type != XmMENU_PULLDOWN &&
       new ->label.menu_type != XmMENU_POPUP)
   {
      /*  See if the GC need to be regenerated and widget redrawn.  */

      if (new -> pushbutton.arm_color != current -> pushbutton.arm_color)
      {
	 flag = TRUE;
	 XtReleaseGC (new, new -> pushbutton.fill_gc);
	 GetFillGC (new);
      }

      if (new -> core.background_pixel != current -> core.background_pixel) 
      {
	flag = TRUE;
	XtReleaseGC (new, new -> pushbutton.background_gc);
	GetBackgroundGC (new);
      }
   }

	if ( flag == False)
    /* size s unchanged - optimize the shadow drawing  */
    {
      if ( (current->pushbutton.show_as_default != 0) &&
                         (new->pushbutton.show_as_default == 0))
		EraseDefaultButtonShadows (new);

       if ( (current->pushbutton.show_as_default == 0) &&
         (new->pushbutton.show_as_default != 0))
	    DrawDefaultButtonShadows (new);
	}

   
   return(flag);
}




/************************************************************************
 *
 *  Help
 *     This function processes Function Key 1 press occuring on the PushButton.
 *
 ************************************************************************/

static void Help (pb, event)
XmPushButtonWidget pb;
XEvent * event;

{
   XmPushButtonCallbackStruct call_value;

   if (pb ->label.menu_type == XmMENU_PULLDOWN ||
       pb ->label.menu_type == XmMENU_POPUP)
   {
      (* xmLabelClassRec.label_class.menuProcs)
	  (XmMENU_POPDOWN, XtParent(pb), NULL, (caddr_t)event, NULL);
   }

   _XmPrimitiveHelp ((XmPrimitiveWidget)pb, event);
/***
   call_value.reason = XmCR_HELP;
   call_value.event = event;
   XtCallCallbackList ((Widget) pb, pb->primitive.help_callback, &call_value);
****/
}



/************************************************************************
 *
 *  Destroy
 *	Clean up allocated resources when the widget is destroyed.
 *
 ************************************************************************/

static void Destroy (pb)
XmPushButtonWidget pb;

{
   if (pb->pushbutton.timer)
       XtRemoveTimeOut (pb->pushbutton.timer);
  
   if (pb ->label.menu_type != XmMENU_PULLDOWN &&
       pb ->label.menu_type != XmMENU_POPUP)
   {
      XtReleaseGC (pb, pb -> pushbutton.fill_gc);
      XtReleaseGC (pb, pb -> pushbutton.background_gc);
   }

   XtRemoveAllCallbacks (pb, XmNactivateCallback);
   XtRemoveAllCallbacks (pb, XmNarmCallback);
   XtRemoveAllCallbacks (pb, XmNdisarmCallback);
}


/************************************************************************
 *
 *		Application Accessible External Functions
 *
 ************************************************************************/


/************************************************************************
 *
 *  XmCreatePushButton
 *	Create an instance of a pushbutton and return the widget id.
 *
 ************************************************************************/

#ifdef _NO_PROTO
Widget XmCreatePushButton (parent, name, arglist, argcount)
Widget parent;
char * name;
ArgList arglist;
Cardinal argcount;

#else /* _NO_PROTO */
Widget XmCreatePushButton (Widget parent, char *name, ArgList arglist, Cardinal argcount)
#endif /* _NO_PROTO */
{
   return (XtCreateWidget (name, xmPushButtonWidgetClass, 
                           parent, arglist, argcount));
}



/*
 * EraseDefaultButtonShadows (pb)
 *  - Called from SetValues() - effort to optimize shadow drawing.
 */
static void
EraseDefaultButtonShadows (pb)
XmPushButtonWidget pb;
{  int done = False;
   int size, x, y, width, height;
   GC tmp_gc;

/**      if (!(XtIsRealized(pb)) ) done = True;   **/
	 if (!(XtIsManaged(pb)) ) done = True;

      if (!(done))
      {
        if ((pb ->label.menu_type == XmMENU_PULLDOWN) ||
                         (pb ->label.menu_type == XmMENU_POPUP))
        { ShellWidget mshell = (ShellWidget)XtParent(XtParent(pb));
          if (!mshell->shell.popped_up) done = True;
        }
      }

      if (!(done))
      { 
	if (pb->pushbutton.compatible)
          size = (int) (pb->pushbutton.show_as_default);
        else
	  size = (int) (pb -> pushbutton.default_button_shadow_thickness);

	if (size > 0)
	{
           x = y = (int)(pb -> primitive.highlight_thickness);
           width = (int) (pb -> core.width - 2 *
              pb->primitive.highlight_thickness);
           height = (int) (pb -> core.height - 2 *
              pb->primitive.highlight_thickness);

 	   tmp_gc = GetParentBackgroundGC (pb);
	   if (tmp_gc)
	   {  XFillRectangle (XtDisplay(pb), XtWindow(pb), tmp_gc,
	    	  x, y, width, size);
	      XFillRectangle (XtDisplay(pb), XtWindow(pb), tmp_gc,
	    	  x, y + height - size, width, size);
	      XFillRectangle (XtDisplay(pb), XtWindow(pb), tmp_gc,
	    	  x, y + size, size, height - (2 * size));
	      XFillRectangle (XtDisplay(pb), XtWindow(pb), tmp_gc,
	    	  x + width - size, y + size, size, height - (2 * size));

              XtReleaseGC( XtParent(pb), tmp_gc); 
	    }

	 }
      }
 }

/*************************************<->*************************************
 *
 *  Redisplay (pb, event, region)
 *   Completely rewritten to accommodate defaultButtonShadowThickness
 *   Description:
 *   -----------
 *     Cause the widget, identified by pb, to be redisplayed.
 *     If XmNfillOnArm is True and the pushbutton is not in a menu,
 *     the background will be filled with XmNarmColor.
 *     If XmNinvertOnArm is True and XmNLabelType is XmPIXMAP,
 *     XmNarmPixmap will be used in the label.
 *
 *************************************<->***********************************/

struct	PBbox
     { int pbx;
       int pby;
       int pbWidth;
       int pbHeight;
     };

/*ARGSUSED*/
static void Redisplay (pb, event, region)
XmPushButtonWidget pb;
XEvent *event;
Region region;

{
/*
 * Where do we check for dependency on MenyType ??
 */
     if (XtIsRealized(pb))
      { if ( (pb ->label.menu_type == XmMENU_PULLDOWN) ||
				(pb ->label.menu_type == XmMENU_POPUP) )
		    DrawPushButtonLabel (pb);	
	   else
	    { 
            DrawPushButtonBackground (pb);
	        DrawPushButtonLabel (pb);
			DrawPushButtonShadows (pb);
		}
        if (pb -> primitive.highlighted)
             _XmHighlightBorder ((Widget)pb);
          else if (_XmDifferentBackground ((Widget)pb, XtParent (pb)))
             _XmUnhighlightBorder ((Widget)pb);

      }
}

/*
 * DrawPushButtonBackground ()
 *  - Compute the area allocated to the pushbutton and fill it with
 *    parent's background ;
 */
static void  DrawPushButtonBackground (pb)
XmPushButtonWidget pb;
{    struct PBbox box;
     GC  tmp_gc;
     Bool result, releaseGC;
     Widget parent;
	 int	borderwidth;

     result = ComputePBArea (pb, &box);
     if (result)  /* computation is successful */
     {  
	      tmp_gc = GetParentBackgroundGC (pb);
	   if (tmp_gc)
	     XFillRectangle (XtDisplay(pb), XtWindow(pb), tmp_gc,
	    	  box.pbx, box.pby, box.pbWidth, box.pbHeight);
      if (tmp_gc)  XtReleaseGC( XtParent(pb), tmp_gc);
     }

     result = ComputePBLabelArea (pb, &box);
     if (result)  /* computation is successful */
     { 
	if ( (pb -> pushbutton.armed) && (pb->pushbutton.fill_on_arm))
	       tmp_gc = pb->pushbutton.fill_gc;
        else tmp_gc = pb->pushbutton.background_gc;

	   if (tmp_gc)
        XFillRectangle (XtDisplay(pb), XtWindow(pb), tmp_gc,
                  box.pbx, box.pby, box.pbWidth, box.pbHeight);

     }
   }
/*
 * DrawPushButtonLabel (pb)
 * Draw the label contained in the pushbutton.
 */
static void DrawPushButtonLabel (pb)
XmPushButtonWidget pb;
{  GC tmp_gc;
   Boolean   replaceGC = False;

    if (pb ->label.menu_type != XmMENU_PULLDOWN &&
         pb ->label.menu_type != XmMENU_POPUP &&
          pb->pushbutton.fill_on_arm)
   {
    if ((pb->label.label_type == XmSTRING) && (pb->pushbutton.armed) &&
         (pb->pushbutton.arm_color == pb->primitive.foreground))
    {
        tmp_gc = pb->label.normal_GC;
        pb->label.normal_GC = pb->pushbutton.background_gc;
        replaceGC = True;
    }
  }

     if (pb->label.label_type == XmPIXMAP)
      {
     if (pb->pushbutton.armed)
        if (pb->pushbutton.arm_pixmap != XmUNSPECIFIED_PIXMAP)
           pb->label.pixmap = pb->pushbutton.arm_pixmap;
        else
           pb->label.pixmap = pb->pushbutton.unarm_pixmap;

     else   /* pushbutton is unarmed */
        pb->label.pixmap = pb->pushbutton.unarm_pixmap;
      }

      (* xmLabelClassRec.core_class.expose) (pb, NULL, NULL);

      if (replaceGC)
         pb->label.normal_GC = tmp_gc;

}
/*
 * DrawPushButtonShadows()
 *  Note: PushButton has two types of shadows: primitive-shadow and
 *	default-button-shadow.
 *  Following shadows  are drawn:
 *  if pushbutton is in a menu only primitive shadows are drawn;
 *   else
 *    { draw default shadow if needed;
 *	draw primitive shadow ;
 *    }
 */
static void DrawPushButtonShadows (pb)
XmPushButtonWidget pb;
{

	 if  ( (pb->pushbutton.default_button_shadow_thickness))
		 
	    { EraseDefaultButtonShadows (pb);
	      if (pb->pushbutton.show_as_default)
		       DrawDefaultButtonShadows (pb);
	    }
	  
          if (pb->primitive.shadow_thickness > 0)
 	        DrawPBPrimitiveShadows (pb);
    
}
/*
 *  ComputePBArea ()
 *  - compute the area allocated to the pushbutton; fill in the dimensions
 *    in the box; resturn indicates whether the values in box are valid or
 *    not;
 */
static Boolean 
ComputePBArea (pb, box)
XmPushButtonWidget pb;
struct	PBbox *box;
{  int adjust = 0;
   Boolean result = True;

   if (pb == NULL) result = False;
   else
    { 
      if (pb->pushbutton.default_button_shadow_thickness > 0)
	 adjust = XM_3D_ENHANCE_PIXEL;
      box->pbx =  0;  
      box->pby =  0;  
      box->pbWidth = pb->core.width + 2 * adjust ;
      box->pbHeight = pb->core.height + 2 * adjust ;
    }
    return (result);
}
/*
 *  ComputePBLabelArea ()
 *  - compute the area allocated to the label of pushbutton; 
 *    fill in the dimensions
 *    in the box; resturn indicates whether the values in box are valid or
 *    not;
 */
static Boolean
ComputePBLabelArea (pb, box)
XmPushButtonWidget pb;
struct  PBbox *box;
{   Boolean result = True;
    int dx, dy, adjust;

    if (pb == NULL) result = False;
    else
     { 
	   if (pb->pushbutton.compatible)
		  adjust = pb -> pushbutton.show_as_default; 
	    else
	     adjust = pb -> pushbutton.default_button_shadow_thickness; 
	   if (adjust > 0)
	     { adjust = adjust + pb-> primitive.shadow_thickness;
		   adjust = (adjust << 1);
           dx = pb->primitive.highlight_thickness + adjust;
         }
	     else
		   dx = pb->primitive.highlight_thickness +
                       pb-> primitive.shadow_thickness;

       box->pbx = dx;
       box->pby = dx;
	   adjust = (dx << 1);
       box->pbWidth = pb->core.width - adjust;
       box->pbHeight= pb->core.height - adjust;
     }
    return (result);
}
	


static GC GetParentBackgroundGC (pb)
XmPushButtonWidget pb;
{	GC pgc;
	Widget  parent;
        XGCValues       values;
        XtGCMask        valueMask;
        short             myindex;

	parent = XtParent(pb);
	if (parent == NULL) pgc = NULL;

        else
 	 { valueMask =  GCForeground /* | GCBackground */ | GCGraphicsExposures;
	   values.foreground   = parent->core.background_pixel;
           values.graphics_exposures = False;
	   pgc = XtGetGC( parent, valueMask,&values);
	 }
	return (pgc);
}
/*
 * DrawPBPrimitiveShadow (pb)
 *   - Should be called only if PrimitiveShadowThickness > 0 
 */
static void DrawPBPrimitiveShadows (pb)
XmPushButtonWidget pb;
{
      GC topgc, bottomgc;
      int dx, dy, width, height, adjust, shadow_thickness;
      if (pb->pushbutton.armed) 
	  { bottomgc  = pb -> primitive.top_shadow_GC;
	    topgc  = pb -> primitive.bottom_shadow_GC;
	  }
       else 
	  { bottomgc  = pb -> primitive.bottom_shadow_GC;
	    topgc  = pb -> primitive.top_shadow_GC;
	  }

     
       shadow_thickness = pb->primitive.shadow_thickness;
       /*
	* This might have to be modified.
	*  - this is where dependency on compatibility with 1.0
        *    and defaultButtonShadowThickness etc. will showup.
        *  NOTE: defaultButtonShadowThickness is not supported in 
	    *   RowColumn children.
		*  1. Compute (x,y,width,height) for the rectangle within which
	    *	  the shadow is to be drawn.
        */

      if ( (shadow_thickness > 0) && (topgc) && (bottomgc))
       { 
	     if (pb->pushbutton.compatible)
				adjust = pb -> pushbutton.show_as_default;
		    else
				adjust = pb -> pushbutton.default_button_shadow_thickness;
	     if (adjust > 0)
		  { adjust = (adjust << 1);
		    dx = pb->primitive.highlight_thickness + 
				 adjust + pb->primitive.shadow_thickness ;
		  }
	     else
			dx = pb->primitive.highlight_thickness ;

         dy = dx;
         width = pb->core.width - 2 *dx;
         height = pb->core.height - 2 *dx;

         _XmDrawShadow (XtDisplay (pb), XtWindow (pb), topgc,
        bottomgc, shadow_thickness, dx, dy, width, height);

	}
}
/*
 * DrawDefaultButtonShadows()
 *  - get the topShadowColor and bottomShadowColor from the parent;
 *    use those colors to construct top and bottom gc; use these
 *	  GCs to draw the shadows of the button.
 *  - Should not be called if pushbutton is in a row column or in a menu.
 *  - Should be called only if a defaultbuttonshadow is to be drawn.
 */      
      
static void DrawDefaultButtonShadows (pb)
XmPushButtonWidget pb;
{
      GC topgc, bottomgc;
      int dx, dy, width, height, default_button_shadow_thickness;
      GC pgc;
      Widget  parent;
      XGCValues       values;
      XtGCMask        valueMask;
      short             myindex;
      Pixel	mypixel;
      Bool  borrowedGC = False;

      if ( ((pb->pushbutton.compatible) &&
			(pb->pushbutton.show_as_default == 0) ) ||
		 ( (!(pb->pushbutton.compatible)) &&
		    (pb->pushbutton.default_button_shadow_thickness == 0)))
		 return;
	  /*
	   * May need more complex computation for getting the GCs.
	   */

        parent = XtParent(pb);
       if (XmIsManager(parent))
       {  valueMask =  GCForeground  ;
		  values.foreground =   ((XmManagerWidget)(parent))-> 
                                manager.top_shadow_color;

          topgc = XtGetGC( pb, valueMask,&values);

	      values.foreground =  ((XmManagerWidget)(parent))-> 
                                 manager.bottom_shadow_color;
 
		  bottomgc = XtGetGC( pb, valueMask,&values);
		  borrowedGC = True;
	   }
	  else
	   { /* Use your own pixel for drawing */
	    bottomgc  = pb -> primitive.bottom_shadow_GC;
        topgc  = pb -> primitive.top_shadow_GC;
	   }
        if ( (bottomgc == NULL) || (topgc == NULL) )
        {  if(borrowedGC)
	     { if (topgc) XtReleaseGC (pb, topgc);
	       if (bottomgc) XtReleaseGC (pb, bottomgc);
             } 
             return;
	 }

	if (pb->pushbutton.compatible)
	  default_button_shadow_thickness = 
			(int) (pb->pushbutton.show_as_default);
       else    
          default_button_shadow_thickness = 
			(int) pb->pushbutton.default_button_shadow_thickness;
    /*
     *
     * Compute location of bounding box to contain the defaultButtonShadow.
     */

        { dx = pb->primitive.highlight_thickness;

	 	 dy = dx;
         width = pb->core.width - 2 *dx;
	 height = pb->core.height - 2 *dx;
         _XmDrawShadow (XtDisplay (pb), XtWindow (pb), bottomgc, topgc, 
		     /**  topgc,  bottomgc, **/ 
		default_button_shadow_thickness, dx, dy, width, height);
	}

	if(borrowedGC)
	  { if (topgc) XtReleaseGC (pb, topgc);
		if (bottomgc) XtReleaseGC (pb, bottomgc);
	  }

}

static XmImportOperator ShowAsDef_ToHorizPix( widget, offset, value)
            Widget          widget ;
            int             offset ;
            XtArgVal *      value ;

{           XtArgVal        oldValue ;
            XmImportOperator returnVal ;

    oldValue = *value ;
    returnVal = _XmToHorizontalPixels( widget, offset, value) ;

    if(    oldValue  &&  !*value    )
    {   *value = (XtArgVal) 1 ;
        } 
    return( returnVal) ;
    } 

void _XmClearBCompatibility( pb)
XmPushButtonWidget pb;
{
	pb->pushbutton.compatible =  False;
}

/*
 * AdjustHighLightThickness ()
 *  HighlightThickness has a dependency on default_button-shadow-thickness;
 *  This routine (called from SetValues) adjust for that dependency.
 *  Applications should be aware that
 *  if a pushbutton gadget has  with (default_button-shadow-thickness == 0)
 * - then if through a XtSetValue it sets (default_button-shadow-thickness > 0)
 *  the application-specified highlight-thickness is internally increased by
 *  XM_3D_ENHANCE_PIXEL to enhance the 3D-appearance of the defaultButton
 *  Shadow. Similarly if a pushbutton gadget has ( default_button-shadow_
 *  thickness > 0), and it resets the (default_button-shadow-thickness = 0)
 *  through a XtSetValue , then the existing highlight-thickness is decreased
 *  by XM_3D_ENHANCE_PIXEL.
 *  The border-highlight when drawn is however is always of the same
 *  thickness as specified by the application since compensation is done
 *  in the drawing routine (see _XmBorderHighlight / Primitive.c).
 */
static int AdjustHighLightThickness (new, current)
XmPushButtonWidget  new, current;
{  int adjustment = 0; 


	if (new->pushbutton.default_button_shadow_thickness > 0)
	{ if (current->pushbutton.default_button_shadow_thickness == 0)
	    { new->primitive.highlight_thickness += XM_3D_ENHANCE_PIXEL;
		  adjustment = XM_3D_ENHANCE_PIXEL;
		}
	  else
	   if (new->primitive.highlight_thickness !=
			current->primitive.highlight_thickness)
	     { new->primitive.highlight_thickness += XM_3D_ENHANCE_PIXEL;
		   adjustment = XM_3D_ENHANCE_PIXEL;
		 }
	}
    else
     { if (current->pushbutton.default_button_shadow_thickness > 0)
        /* default_button_shadow_thickness was > 0 and is now
         * being set to 0.
         * - so take away the adjustment for enhancement.
         */
	    {if (new->primitive.highlight_thickness ==
				current->primitive.highlight_thickness)
	       {  new->primitive.highlight_thickness -= XM_3D_ENHANCE_PIXEL;
			  adjustment -= XM_3D_ENHANCE_PIXEL;
		   }
        /*
         * This will appear to be a bug if in a XtSetValues the application
         * removes the default_button_shadow_thickness and also
         * sets the high-light-thickness to a value of
         * (old-high-light-thickness (from previous XtSetValue) +
         *  XM_3D_ENHANCE_PIXEL) at the same time.
         * This will be documented.
         */
       }
     }
	return (adjustment);
}


static void 
ExportHighlightThickness ( widget, offset, value)
            Widget          widget ;
            int             offset ;
            XtArgVal *      value ;

{
   XmPushButtonWidget pb;
   
   pb = (XmPushButtonWidget) widget;

             if (pb->pushbutton.show_as_default ||
		pb->pushbutton.default_button_shadow_thickness)
		if ((int)*value >= XM_3D_ENHANCE_PIXEL)
		{ *value -= XM_3D_ENHANCE_PIXEL;
  	       }
            _XmFromHorizontalPixels (widget, offset, value);
}

static void UnhighlightBorder(pb)
XmPushButtonWidget pb;
{
   XmPrimitiveWidget pw;
   register int width;
   register int height;
   register highlight_width;
    GC tmp_gc;

 	   pw = (XmPrimitiveWidget) pb;

   width = pb->core.width;
   height= pb->core.height;
   pw->primitive.highlighted = False;
   pw->primitive.highlight_drawn = False;


    highlight_width = pw->primitive.highlight_thickness;

    if ((width == 0) || (height == 0) || (highlight_width == 0)) return;
	else
     {  tmp_gc = GetParentBackgroundGC (pb);
        if (tmp_gc)
        { _XmDrawBorder ((Widget)pw, tmp_gc, 0, 0, width, height,
                                highlight_width);
           XtReleaseGC( XtParent(pw), tmp_gc);
	    }
	 }
}


