#ifdef REV_INFO
#ifndef lint
static char SCCSID[] = "OSF/Motif: @(#)PushBG.c	3.36.1.1 91/02/21";
#endif /* lint */
#endif /* REV_INFO */
/******************************************************************************
*******************************************************************************
*
*  (c) Copyright 1989, 1990, 1991 OPEN SOFTWARE FOUNDATION, INC.
*  (c) Copyright 1989, DIGITAL EQUIPMENT CORPORATION, MAYNARD, MASS.
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
#include <Xm/LabelGP.h>
#include <Xm/PushBGP.h>
#include <Xm/CacheP.h>
#include <Xm/ExtObjectP.h>
#include <Xm/BaseClassI.h>
#include <Xm/CascadeB.h>
   
#define DELAY_DEFAULT 100

static Boolean SetValues();
static void ClassPartInitialize();
static void ClassInitialize();
static void Initialize();
static void GetFillGC();
static void GetBackgroundGC();
static void Arm();
static void Disarm();
static void Activate();
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
static void ActivateCommonG();
static void     InputDispatch();
static Boolean  VisualChange();
static void DrawDefaultButtonShadow();
static void EraseDefaultButtonShadow();
static XmImportOperator ShowAsDef_ToHorizPix() ;
static  void ExportHighlightThickness ();

extern _XmSelectColorDefault();

static int _XmPushBCacheCompare();

static void SecondaryObjectCreate();

/* Initialize and SetValues Pre and Post hooks */

static void InitializePosthook();
static Boolean SetValuesPrehook();
static Boolean SetValuesPosthook();
static void GetValuesPrehook();
static void GetValuesPosthook();

static int AdjustHighLightThickness ();
static void DrawPushButtonLabelGadget();
static void DrawDefaultButtonShadows();
static void EraseDefaultButtonShadows();
static void DrawPBGadgetShadows();
static void DrawPushButtonGadgetShadows();
static void DrawPushBGBackground();
static Boolean  ComputePBLabelArea();
struct  PBTimeOutEvent
        {  XmPushButtonGadget  pushbutton;
           XEvent              *xevent;
        } ;

/*
 * Procedures for manipulating cached_resources as secondary data.
 */
static Cardinal GetPushBGClassSecResData();
static XtPointer GetPushBGClassSecResBase ( );
extern Cardinal _XmSecondaryResourceData ();

/************************************************
 The uncached resources for Push Button  
 ************************************************/

static XtResource resources[] = 
{     
   {
     XmNactivateCallback, XmCCallback, XmRCallback, sizeof(XtCallbackList),
     XtOffset (XmPushButtonGadget, pushbutton.activate_callback),
     XmRPointer, (caddr_t) NULL
   },

   {
     XmNarmCallback, XmCCallback, XmRCallback, sizeof(XtCallbackList),
     XtOffset (XmPushButtonGadget, pushbutton.arm_callback),
     XmRPointer, (caddr_t) NULL
   },

   {
     XmNdisarmCallback, XmCCallback, XmRCallback, sizeof(XtCallbackList),
     XtOffset (XmPushButtonGadget, pushbutton.disarm_callback),
     XmRPointer, (caddr_t) NULL
   },

   {
     XmNshadowThickness, XmCShadowThickness, XmRHorizontalDimension, 
     sizeof(Dimension),
     XtOffset (XmPushButtonGadget, gadget.shadow_thickness),
     XmRImmediate, (caddr_t) 2
   },

   {
     XmNtraversalOn,
     XmCTraversalOn,
     XmRBoolean,
     sizeof (Boolean),
     XtOffset (XmGadget, gadget.traversal_on),
     XmRImmediate, 
     (caddr_t) True
   },

   {
     XmNhighlightThickness,
     XmCHighlightThickness,
     XmRHorizontalDimension,
     sizeof (Dimension),
     XtOffset (XmGadget, gadget.highlight_thickness),
     XmRImmediate, 
     (caddr_t) 2
   },

   {
     XmNshowAsDefault,
     XmCShowAsDefault,
     XmRBooleanDimension,
     sizeof (Dimension),
     XtOffset (XmPushButtonGadget, pushbutton.show_as_default),
     XmRImmediate, (caddr_t) 0
   },

};

static XmSyntheticResource syn_resources[] =
{
   {
     XmNshowAsDefault,
     sizeof (Dimension),
     XtOffset (XmPushButtonGadget, pushbutton.show_as_default),
     _XmFromHorizontalPixels,
     ShowAsDef_ToHorizPix
   },
   {
     XmNhighlightThickness,
     sizeof (Dimension),
     XtOffset (XmGadget, gadget.highlight_thickness),
     ExportHighlightThickness,
     _XmToHorizontalPixels
   },

};

/**********************************************
 Cached resources for PushButton Gadget
 **********************************************/

static XtResource cache_resources[] =
{

   {
     XmNmultiClick,
     XmCMultiClick,
     XmRMultiClick,
     sizeof (unsigned char),
     XtOffset (XmPushButtonGCacheObject, pushbutton_cache.multiClick ),
     XmRImmediate,
     (caddr_t) XmMULTICLICK_KEEP
   },

   {
     XmNdefaultButtonShadowThickness,
     XmCDefaultButtonShadowThickness,
     XmRHorizontalDimension, 
     sizeof (Dimension),
     XtOffset (XmPushButtonGCacheObject, pushbutton_cache.default_button_shadow_thickness ),
     XmRImmediate,
     (caddr_t) 0
   },

   {
     XmNfillOnArm, XmCFillOnArm, XmRBoolean, sizeof (Boolean),
     XtOffset (XmPushButtonGCacheObject, pushbutton_cache.fill_on_arm),
     XmRImmediate, (caddr_t) True
   },

   {
     XmNarmColor, XmCArmColor, XmRPixel, sizeof (Pixel),
     XtOffset (XmPushButtonGCacheObject, pushbutton_cache.arm_color),
     XmRCallProc, (caddr_t) _XmSelectColorDefault
   },

   {
     XmNarmPixmap, XmCArmPixmap, XmRGadgetPixmap, sizeof (Pixmap),
     XtOffset (XmPushButtonGCacheObject, pushbutton_cache.arm_pixmap),
     XmRImmediate, (caddr_t) XmUNSPECIFIED_PIXMAP
   },
};

/*  Definition for resources that need special processing in get values  */

static XmSyntheticResource cache_syn_resources[] =
{
   {
     XmNdefaultButtonShadowThickness,
     sizeof (Dimension),
     XtOffset (XmPushButtonGCacheObject, pushbutton_cache.default_button_shadow_thickness ),
     _XmFromHorizontalPixels,
     _XmToHorizontalPixels
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

static XmCacheClassPart PushButtonClassCachePart = {
    {NULL, 0, 0},            /* head of class cache list */
    _XmCacheCopy,            /* Copy routine     */
    _XmCacheDelete,          /* Delete routine   */
    _XmPushBCacheCompare,    /* Comparison routine   */
};

static XmBaseClassExtRec   PushBGClassExtensionRec = {
    NULL,   		 			/*   next_extension    */
    NULLQUARK,    				/* record_typ  */
    XmBaseClassExtVersion,			/*  version  */
    sizeof(XmBaseClassExtRec), 			/* record_size  */
    XmInheritInitializePrehook,			/*  initializePrehook  */
    SetValuesPrehook,   			/* setValuesPrehook  */
    InitializePosthook,				/* initializePosthook  */
    SetValuesPosthook, 				/* setValuesPosthook  */
    (WidgetClass)&xmPushButtonGCacheObjClassRec,/* secondaryObjectClass */
    (XtInitProc)SecondaryObjectCreate, 		/* secondaryObjectCreate */
    (XmGetSecResDataFunc) GetPushBGClassSecResData,   /* getSecResData  */
    {NULL},           		                /* fast subclass  */
    GetValuesPrehook,   			/* getValuesPrehook  */
    GetValuesPosthook,   			/* getValuesPosthook  */
};

/* ext rec static initialization */
XmPushButtonGCacheObjClassRec xmPushButtonGCacheObjClassRec =
{
  {
      /* superclass         */    (WidgetClass) &xmLabelGCacheObjClassRec,
      /* class_name         */    "XmPushButtonGadget",
      /* widget_size        */    sizeof(XmPushButtonGCacheObjRec),
      /* class_initialize   */    NULL,
      /* chained class init */    NULL,
      /* class_inited       */    False,
      /* initialize         */    NULL,
      /* initialize hook    */    NULL,
      /* realize            */    NULL,
      /* actions            */    NULL,
      /* num_actions        */    0,
      /* resources          */    cache_resources,
      /* num_resources      */    XtNumber(cache_resources),
      /* xrm_class          */    NULLQUARK,
      /* compress_motion    */    False,
      /* compress_exposure  */    False,
      /* compress enter/exit*/    False,
      /* visible_interest   */    False,
      /* destroy            */    NULL,
      /* resize             */    NULL,
      /* expose             */    NULL,
      /* set_values         */    NULL,
      /* set values hook    */    NULL,
      /* set values almost  */    NULL,
      /* get values hook    */    NULL,
      /* accept_focus       */    NULL,
      /* version            */    XtVersion,
      /* callback offsetlst */    NULL,
      /* default trans      */    NULL,
      /* query geo proc     */    NULL,
      /* display accelerator*/    NULL,
      /* extension record   */    NULL,
   },

   {
      /* synthetic resources */   cache_syn_resources,
      /* num_syn_resources   */   XtNumber(cache_syn_resources), 
      /* extension           */   NULL,
   }
};

/*  The PushButton class record definition  */

externaldef(xmpushbuttongadgetclassrec)
	XmPushButtonGadgetClassRec xmPushButtonGadgetClassRec = 
{
  {
      (WidgetClass) &xmLabelGadgetClassRec,  	/* superclass            */
      "XmPushButtonGadget",             	/* class_name	         */
      sizeof(XmPushButtonGadgetRec),    	/* widget_size	         */
      ClassInitialize,         			/* class_initialize      */
      ClassPartInitialize,              	/* class_part_initialize */
      FALSE,                            	/* class_inited          */
      (XtInitProc) Initialize,          	/* initialize	         */
      NULL,                             	/* initialize_hook       */
      NULL,	                        	/* realize	         */
      NULL,                             	/* actions               */
      0,			        	/* num_actions    	 */
      resources,                        	/* resources	         */
      XtNumber(resources),              	/* num_resources         */
      NULLQUARK,                        	/* xrm_class	         */
      TRUE,                             	/* compress_motion       */
      XtExposeCompressMaximal,          	/* compress_exposure     */
      TRUE,                             	/* compress_enterleave   */
      FALSE,                            	/* visible_interest      */	
      (XtWidgetProc) Destroy,           	/* destroy               */	
      _XtInherit,	                	/* resize                */
      (XtExposeProc) Redisplay,        		/* expose                */	
      (XtSetValuesFunc) SetValues,      	/* set_values	         */	
      NULL,                             	/* set_values_hook       */
      XtInheritSetValuesAlmost,         	/* set_values_almost     */
      NULL,                             	/* get_values_hook       */
      NULL,                             	/* accept_focus	         */	
      XtVersion,                        	/* version               */
      NULL,                             	/* callback private      */
      NULL,                             	/* tm_table              */
      XtInheritQueryGeometry,           	/* query_geometry        */
      NULL,					/* display_accelerator   */
      (XtPointer)&PushBGClassExtensionRec,	/* extension             */
   },

   {          /* gadget class record */
      BorderHighlight,		/* border highlight   */
      BorderUnhighlight,	/* border_unhighlight */
      ArmAndActivate,		/* arm_and_activate   */
      InputDispatch,		/* input dispatch     */
      VisualChange,    		/* visual_change      */
      syn_resources,   		/* syn resources      */
      XtNumber(syn_resources),  /* num syn_resources  */
      &PushButtonClassCachePart,	/* class cache part   */
      NULL,         		/* extension          */
   },

   { 	/* label_class record */
 
      _XtInherit,		/* setOverrideCallback */
      (XmMenuProc)_XtInherit,		/* menu proc's entry   */
      NULL,			/* extension	       */   
   },

   {    /* pushbutton class record */
      NULL,			   /* extension 	 */
   }
};

externaldef(xmpushbuttongadgetclass) WidgetClass xmPushButtonGadgetClass = 
   (WidgetClass) &xmPushButtonGadgetClassRec;

/*****************************************************************
 *  SPECIAL PROPERTIES OF PUSHBUTTON GADGET INSIDE A MENU:
 *   When a PushButton (widget/gadget) is incorporated in a Menu
 *   (Pulldownor Popup) - its properties get modified in these ways:
 *   (1) The redisplay routine should not draw its background nor draw
 *	     its shadows.  It should draw only the label. To comply with 
 *	     this means that the arm-color and background color are not
 *	     of any use. As a result the values in the FillGC and BackgroundGC
 *	     are not initialized and are likely to be bogus. This causes
 *	     special casing of Initialize and SetValues routines.
 *   (2) PushButton does not show its depressed appearance in the
 *       menu. This will cause Arm(), DisArm(), ArmAndActivate routines
 *	     to have special cases.
 *   In short the properties of Pushbutton in a menu are so different
 *	  that practically all major routines in this widget will have to
 *	  special-cased to accommodate this difference as if two different
 *	  classes are being glued to one class.
 *******************************************************************/

/*******************************************************************
 *
 *  _XmPushBCacheCompare
 *
 *******************************************************************/

static int _XmPushBCacheCompare (pushB_inst, pushB_cache_inst)
XmPushButtonGCacheObjPart *pushB_inst, *pushB_cache_inst;
{
    if((pushB_inst->fill_on_arm == pushB_cache_inst->fill_on_arm) &&
       (pushB_inst->arm_color == pushB_cache_inst->arm_color) &&
       (pushB_inst->arm_pixmap == pushB_cache_inst->arm_pixmap) &&
       (pushB_inst->unarm_pixmap == pushB_cache_inst->unarm_pixmap) &&
       (pushB_inst->fill_gc == pushB_cache_inst->fill_gc) &&
       (pushB_inst->background_gc == pushB_cache_inst->background_gc) &&
       (pushB_inst->multiClick == pushB_cache_inst->multiClick) &&
       (pushB_inst->default_button_shadow_thickness ==
                    pushB_cache_inst->default_button_shadow_thickness) &&
       (pushB_inst->timer == pushB_cache_inst->timer))
       return 1;
    else
       return 0;
}
/************************************************************************
 *
 *  InputDispatch
 *     This function catches input sent by a manager and dispatches it
 *     to the individual routines.
 *
 ************************************************************************/

static void InputDispatch (pb, event, event_mask)
XmPushButtonGadget pb;
XEvent * event;
Mask event_mask;

{
   if ( (event_mask & XmARM_EVENT) ||
	     ((PBG_MultiClick(pb) == XmMULTICLICK_KEEP) &&
				 (event_mask & XmMULTI_ARM_EVENT)) )
   {
      if (LabG_MenuType(pb) == XmMENU_PULLDOWN ||
	  LabG_MenuType(pb) == XmMENU_POPUP)
         BtnDown (pb, event);
      else
         Arm (pb, event);
   }

   else if (event_mask & XmACTIVATE_EVENT)
        {  PBG_ClickCount (pb) = 1; 
					/* pb->pushbutton.click_count = 1; */
           ActivateCommonG ( pb, event, event_mask);
        }
    else if (event_mask & XmMULTI_ACTIVATE_EVENT)
        { /* if XmNMultiClick resource is set to DISCARD - do nothing
             else increment clickCount and Call ActivateCommonG .
           */
           if (PBG_MultiClick(pb) == XmMULTICLICK_KEEP) 
			{ ( PBG_ClickCount (pb))++;
              ActivateCommonG ( pb, event, event_mask);
		    }
        }

   else if (event_mask & XmHELP_EVENT) Help (pb, event);
   else if (event_mask & XmENTER_EVENT) Enter (pb, event);
   else if (event_mask & XmLEAVE_EVENT) Leave (pb, event);
   else if (event_mask & XmFOCUS_IN_EVENT)  _XmFocusInGadget ((XmGadget)pb, event);
   else if (event_mask & XmFOCUS_OUT_EVENT)  _XmFocusOutGadget ((XmGadget)pb, event);
}




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
XmPushButtonGadget pb;
XEvent *event;

{
   XmPushButtonCallbackStruct call_value;

   PBG_Armed(pb) = TRUE;

   Redisplay(pb, event, FALSE);

   if (PBG_ArmCallback(pb))
   {
      XFlush(XtDisplay (pb));

      call_value.reason = XmCR_ARM;
      call_value.event = event;
      XtCallCallbackList ((Widget) pb, PBG_ArmCallback(pb), &call_value);
   }
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

static void Activate (pb, event)
XmPushButtonGadget pb;
XEvent * event;

{
   XButtonEvent *buttonEvent = (XButtonEvent *) event;
    XmPushButtonCallbackStruct call_value;   

   PBG_Armed(pb) = FALSE;

   Redisplay(pb, event, FALSE);

   if ((buttonEvent->x <= pb->rectangle.x + pb->rectangle.width) &&
       (buttonEvent->y <= pb->rectangle.y + pb->rectangle.height) &&
       (buttonEvent->x >= pb->rectangle.x) &&
       (buttonEvent->y >= pb->rectangle.y))
    {

       call_value.reason = XmCR_ACTIVATE;
       call_value.event = event;
      call_value.click_count = PBG_ClickCount(pb);
					/*   pb->pushbutton.click_count;   */

       /* if the parent is a RowColumn, notify it about the select */
       if (XmIsRowColumn(XtParent(pb)))
       {
	  (* xmLabelGadgetClassRec.label_class.menuProcs) (XmMENU_CALLBACK, 
							   XtParent(pb),
							   FALSE, (caddr_t)pb,
							   (caddr_t)&call_value);
       }

       if ((! LabG_SkipCallback(pb)) &&
	   (PBG_ActivateCallback(pb)))
       {
	  XFlush (XtDisplay (pb));
	  XtCallCallbackList ((Widget) pb, PBG_ActivateCallback(pb),
				 &call_value);
       }
    }
}

/************************************************************************
 *
 *     ArmAndActivate
 *  - Called only if the PushButtonGadget is being selected via keyboard
 *    i.e. by pressing <return> or <space-bar>.
 ************************************************************************/

static void ArmAndActivate (pb, event)
XmPushButtonGadget pb;
XEvent *event;

{
   XmPushButtonCallbackStruct call_value;
   Boolean already_armed = PBG_Armed(pb);

   if (LabG_MenuType(pb) == XmMENU_PULLDOWN ||
       LabG_MenuType(pb) == XmMENU_POPUP)
   {
      PBG_Armed(pb) = FALSE;
      (* xmLabelGadgetClassRec.label_class.menuProcs)
	  (XmMENU_POPDOWN, XtParent(pb), NULL, (caddr_t)event, NULL);
   }
   else
   {
      PBG_Armed(pb) = TRUE;
      Redisplay ( pb, event, FALSE); 
   }

   XFlush (XtDisplay (pb));

   if (event)
   {
      if (event->type == KeyPress)  PBG_ClickCount (pb) = 1;
   }

   if (PBG_ArmCallback(pb) && !already_armed)
   {
      call_value.reason = XmCR_ARM;
      call_value.event = event;
      call_value.click_count = PBG_ClickCount (pb);
      XtCallCallbackList ((Widget) pb, PBG_ArmCallback(pb), &call_value);
   }

   call_value.reason = XmCR_ACTIVATE;
   call_value.event = event;
   call_value.click_count = PBG_ClickCount (pb);

   /* if the parent is a RowColumn, notify it about the select */
   if (XmIsRowColumn(XtParent(pb)))
   {
      (* xmLabelGadgetClassRec.label_class.menuProcs) (XmMENU_CALLBACK, 
						XtParent(pb), FALSE,
						(caddr_t)pb,
						(caddr_t)&call_value);
   }

   if ((! LabG_SkipCallback(pb)) && (PBG_ActivateCallback(pb)))
   {
      XFlush (XtDisplay (pb));
      XtCallCallbackList ((Widget) pb, PBG_ActivateCallback(pb), &call_value);
   }

   PBG_Armed(pb) = FALSE;

   if (PBG_DisarmCallback(pb))
   {
      XFlush (XtDisplay (pb));
      call_value.reason = XmCR_DISARM;
      XtCallCallbackList ((Widget) pb, PBG_DisarmCallback(pb), &call_value);
   }

   /* If the button is still around, show it released, after a short delay */

   if ((pb->object.being_destroyed == False) &&
        LabG_MenuType(pb) != XmMENU_PULLDOWN &&
        LabG_MenuType(pb) != XmMENU_POPUP)
   {
      PBG_Timer(pb) = XtAppAddTimeOut(XtWidgetToApplicationContext(pb),
				(unsigned long) DELAY_DEFAULT,
				(XtTimerCallbackProc) ArmTimeout,
				(caddr_t)(pb));
   }
}

/*ARGSUSED*/
static XtTimerCallbackProc ArmTimeout (pb, id)
XmPushButtonGadget pb;
XtIntervalId *id;
{
   PBG_Timer(pb) = 0;
   if (XtIsRealized (pb) && XtIsManaged (pb)) {
	Redisplay ( pb, NULL, FALSE); 
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
XmPushButtonGadget pb;
XEvent *event;

{
   XmPushButtonCallbackStruct call_value;

   PBG_Armed(pb) = FALSE;

   if (PBG_DisarmCallback(pb))
   {
      call_value.reason = XmCR_DISARM;
      call_value.event = event;
      XtCallCallbackList ((Widget) pb, PBG_DisarmCallback(pb), &call_value);
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
XmPushButtonGadget pb;
XEvent *event;

{
   XmPushButtonCallbackStruct call_value;
   Boolean already_armed;

   /* Popdown other popups that may be up */
   if (XtParent(XtParent(pb))->core.num_popups != 0)
   {
      ShellWidget popup = (ShellWidget)
	                   XtParent(XtParent(pb))->core.popup_list[0];
      
      if (popup->shell.popped_up)
	  (* xmLabelGadgetClassRec.label_class.menuProcs)
	      (XmMENU_SHELL_POPDOWN, (Widget)popup, NULL, (caddr_t)event, NULL);
      else
      {
         /* If the shell is not popped up but the cascade button is
          * highlighted, then unhighlight it.
          */
          Widget w = ((XmManagerWidget)XtParent(pb))->manager.active_child;
          if (w && (XmIsCascadeButton(w) || XmIsCascadeButtonGadget(w)))
              XmCascadeButtonHighlight (w, FALSE);
      }

   }

   /* Set focus to this button.  This must follow the possible
    * unhighlighting of the CascadeButton else it'll screw up active_child.
    */
   (void)XmProcessTraversal((Widget)pb, XmTRAVERSE_CURRENT);
         /* get the location cursor - get consistent with Gadgets */

   (* xmLabelGadgetClassRec.label_class.menuProcs) (XmMENU_TRAVERSAL,
					XtParent(pb), FALSE, NULL, NULL);

   if (pb->gadget.shadow_thickness > 0)
      _XmDrawShadow (XtDisplay (pb), XtWindow (pb), 
		       XmParentTopShadowGC(pb),
		       XmParentBottomShadowGC(pb),
		       pb -> gadget.shadow_thickness,
		       pb -> rectangle.x + pb -> gadget.highlight_thickness,
		       pb -> rectangle.y + pb -> gadget.highlight_thickness,
		       pb -> rectangle.width - 2 * pb->gadget.highlight_thickness,
		       pb -> rectangle.height - 2 * pb->gadget.highlight_thickness);

   already_armed = PBG_Armed(pb);
   PBG_Armed(pb) = TRUE;

   if (PBG_ArmCallback(pb) && !already_armed)
   {
      XFlush (XtDisplay (pb));

      call_value.reason = XmCR_ARM;
      call_value.event = event;
      XtCallCallbackList ((Widget) pb, PBG_ArmCallback(pb), &call_value);
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
XmPushButtonGadget pb;
XEvent *event;

{
   XmPushButtonCallbackStruct call_value;
   Boolean flushDone = False;

   PBG_Armed(pb) = FALSE;

   (* xmLabelGadgetClassRec.label_class.menuProcs)
       (XmMENU_POPDOWN, XtParent(pb), NULL, (caddr_t)event, NULL);

   _XmRecordEvent(event);
   
   call_value.reason = XmCR_ACTIVATE;
   call_value.event = event;

   /* if the parent is a RowColumn, notify it about the select */
   if (XmIsRowColumn(XtParent(pb)))
   {
      (* xmLabelGadgetClassRec.label_class.menuProcs) (XmMENU_CALLBACK, 
						       XtParent(pb), FALSE, (caddr_t)pb,
						       (caddr_t)&call_value);
      flushDone = True;
   }

   if ((! LabG_SkipCallback(pb)) &&
       (PBG_ActivateCallback(pb)))
   {
      XFlush (XtDisplay (pb));
      flushDone = True;
      XtCallCallbackList ((Widget) pb, PBG_ActivateCallback(pb),
			     &call_value);
   }

   if (PBG_DisarmCallback(pb))
   {
         if (!flushDone)
	     XFlush (XtDisplay (pb));
         call_value.reason = XmCR_DISARM;
         call_value.event = event;
         XtCallCallbackList ((Widget) pb, PBG_DisarmCallback(pb), &call_value);
   }
}




/************************************************************************
 *
 *  Enter
 *
 ************************************************************************/

static void Enter (pb, event)
XmPushButtonGadget pb;
XEvent * event;

{
   XmPushButtonCallbackStruct call_value;

   if (LabG_MenuType(pb) == XmMENU_PULLDOWN ||
       LabG_MenuType(pb) == XmMENU_POPUP)
   {
      if ((((ShellWidget) XtParent(XtParent(pb)))->shell.popped_up) &&
          (! ((XmManagerWidget) XtParent(pb))->manager.traversal_on))
      {
	 if (pb->gadget.shadow_thickness > 0)
	    _XmDrawShadow (XtDisplay (pb), XtWindow (pb), 
		       XmParentTopShadowGC(pb),
		       XmParentBottomShadowGC(pb),
		       pb -> gadget.shadow_thickness,
		       pb -> rectangle.x + pb -> gadget.highlight_thickness,
		       pb -> rectangle.y + pb -> gadget.highlight_thickness,
		       pb -> rectangle.width - 2 * pb->gadget.highlight_thickness,
		       pb -> rectangle.height - 2 * pb->gadget.highlight_thickness);

         PBG_Armed(pb) = TRUE;

         if (PBG_ArmCallback(pb))
         {
            XFlush (XtDisplay (pb));

            call_value.reason = XmCR_ARM;
            call_value.event = event;
            XtCallCallbackList ((Widget) pb, PBG_ArmCallback(pb), &call_value);
         }
      }
   }  
   else 
   {
      _XmEnterGadget ((XmGadget)pb, event);
      if (PBG_Armed(pb) == TRUE)
         Redisplay (pb,event, FALSE);      
   }
}


/************************************************************************
 *
 *  Leave
 *
 ************************************************************************/

static void Leave (pb, event)
XmPushButtonGadget pb;
XEvent * event;

{
   XmPushButtonCallbackStruct call_value;

   if (LabG_MenuType(pb) == XmMENU_PULLDOWN ||
       LabG_MenuType(pb) == XmMENU_POPUP)
   {
      if (! (((XmManagerWidget) XtParent(pb))->manager.traversal_on) &&
            (PBG_Armed(pb)))
      {
         if (pb->gadget.shadow_thickness > 0)
	    _XmEraseShadow (XtDisplay (pb), XtWindow (pb), 
		       pb -> gadget.shadow_thickness,
  	               pb -> rectangle.x + pb -> gadget.highlight_thickness,
		       pb -> rectangle.y + pb -> gadget.highlight_thickness,
		       pb -> rectangle.width - 2 * pb->gadget.highlight_thickness,
		       pb -> rectangle.height - 2 * pb->gadget.highlight_thickness);

	 PBG_Armed(pb) = FALSE;

         if (PBG_DisarmCallback(pb))
         {
	    XFlush (XtDisplay (pb));

	    call_value.reason = XmCR_DISARM;
	    call_value.event = event;
	    XtCallCallbackList ((Widget) pb, PBG_DisarmCallback(pb), &call_value);
         }
      }
   }
   else 
   {
      _XmLeaveGadget ((XmGadget)pb, event);

      if (PBG_Armed(pb) == TRUE)
      {
	 PBG_Armed(pb) = FALSE;
         Redisplay (pb,event, FALSE);      
	 PBG_Armed(pb) = TRUE;
      }
   }
}


/*************************************<->*************************************
 *
 *  BorderHighlight 
 *
 *************************************<->***********************************/
static void BorderHighlight (pb)
XmPushButtonGadget pb;
{
   XmPushButtonCallbackStruct call_value;
   XEvent * event = NULL;

   if (LabG_MenuType(pb) == XmMENU_PULLDOWN ||
       LabG_MenuType(pb) == XmMENU_POPUP)
   {
      if (pb->gadget.shadow_thickness > 0)
	  _XmDrawShadow (XtDisplay (pb), XtWindow (pb), 
		       XmParentTopShadowGC(pb),
		       XmParentBottomShadowGC(pb),
		       pb -> gadget.shadow_thickness,
		       pb -> rectangle.x + pb -> gadget.highlight_thickness,
		       pb -> rectangle.y + pb -> gadget.highlight_thickness,
		       pb -> rectangle.width -
			 2 * pb->gadget.highlight_thickness,
		       pb -> rectangle.height -
			 2 * pb->gadget.highlight_thickness);

      PBG_Armed(pb) = TRUE;

      if (PBG_ArmCallback(pb))
      {
	 XFlush (XtDisplay (pb));

	 call_value.reason = XmCR_ARM;
	 call_value.event = event;
	 XtCallCallbackList ((Widget) pb, PBG_ArmCallback(pb), &call_value);
      }
   }
   else
       _XmHighlightBorder((Widget)pb);
}


/*************************************<->*************************************
 *
 *  BorderUnhighlight
 *
 *************************************<->***********************************/
static void BorderUnhighlight (pb)
XmPushButtonGadget pb;
{
   XmPushButtonCallbackStruct call_value;
   XEvent * event = NULL;

   if (LabG_MenuType(pb) == XmMENU_PULLDOWN ||
       LabG_MenuType(pb) == XmMENU_POPUP)
   {
      if (!PBG_Armed(pb))
	  return;
      if (pb->gadget.shadow_thickness > 0)
	  _XmEraseShadow (XtDisplay (pb), XtWindow (pb), 
		       pb -> gadget.shadow_thickness,
  	               pb -> rectangle.x + pb -> gadget.highlight_thickness,
		       pb -> rectangle.y + pb -> gadget.highlight_thickness,
		       pb -> rectangle.width -
			  2 * pb->gadget.highlight_thickness,
		       pb -> rectangle.height -
			  2 * pb->gadget.highlight_thickness);

      PBG_Armed(pb) = FALSE;

      if (PBG_DisarmCallback(pb))
      {
	 XFlush (XtDisplay (pb));

	 call_value.reason = XmCR_DISARM;
	 call_value.event = event;
	 XtCallCallbackList ((Widget) pb, PBG_DisarmCallback(pb), &call_value);
      }
   }
   else
       _XmUnhighlightBorder((Widget)pb);
}


/*************************************<->*************************************
 *
 *  KeySelect
 *
 *  If the menu system traversal is enabled, do an activate and disarm
 *
 *************************************<->***********************************/
static void KeySelect (pb, event)
XmPushButtonGadget pb;
XEvent *event;
{
   XmPushButtonCallbackStruct call_value;

   if (!_XmIsEventUnique(event))
      return;

   if (((XmManagerWidget) XtParent(pb)) -> manager.traversal_on)
   {

      PBG_Armed(pb) = FALSE;

      (* xmLabelGadgetClassRec.label_class.menuProcs)
	  (XmMENU_POPDOWN, XtParent(pb), NULL, (caddr_t)event, NULL);

      _XmRecordEvent(event);

      call_value.reason = XmCR_ACTIVATE;
      call_value.event = event;

      /* if the parent is a RowColumn, notify it about the select */
      if (XmIsRowColumn(XtParent(pb)))
      {
	 (* xmLabelGadgetClassRec.label_class.menuProcs) (XmMENU_CALLBACK, 
							  XtParent(pb),
							  FALSE, (caddr_t)pb,
							  (caddr_t)&call_value);
      }

      if ((! LabG_SkipCallback(pb)) &&
	  (PBG_ActivateCallback(pb)))
      {
	 XFlush (XtDisplay (pb));
	 XtCallCallbackList ((Widget) pb, PBG_ActivateCallback(pb),
				&call_value);
      }
   }
}

/***********************************************************
*
*  ClassInitialize
*
************************************************************/

static void ClassInitialize()
{
  PushBGClassExtensionRec.record_type = XmQmotif;
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
   _XmFastSubclassInit (wc, XmPUSH_BUTTON_GADGET_BIT);
}

/************************************************************************
*
*  SecondaryObjectCreate
*
************************************************************************/
/* ARGSUSED */
static void SecondaryObjectCreate(req, new, args, num_args)
Widget      req, new;
ArgList     args;
Cardinal    *num_args;
{
  XmBaseClassExt              *cePtr;
  Arg                         myArgs[2];
  ArgList                     mergedArgs;
  XmWidgetExtData             extData;

  XtSetArg(myArgs[0] ,XmNlogicalParent, new);
  XtSetArg(myArgs[1] ,XmNextensionType, XmCACHE_EXTENSION);

  if (*num_args)
     mergedArgs = XtMergeArgLists(args, *num_args, myArgs, XtNumber(myArgs));
  else
     mergedArgs = myArgs;

  cePtr = _XmGetBaseClassExtPtr(XtClass(new), XmQmotif);

  (void) XtCreateWidget(XtName(new),
                       (*cePtr)->secondaryObjectClass,
		       XtParent(new),
		       mergedArgs, *num_args + 2);

  extData = _XmGetWidgetExtData(new, XmCACHE_EXTENSION);

  if (mergedArgs != myArgs)
    XtFree (mergedArgs);

  /*
   * fill out cache pointers
  */
   LabG_Cache(new) = &(((XmLabelGCacheObject)extData->widget)->label_cache);
   LabG_Cache(req) = &(((XmLabelGCacheObject)extData->reqWidget)->label_cache);

   PBG_Cache(new) =
	  &(((XmPushButtonGCacheObject)extData->widget)->pushbutton_cache);
   PBG_Cache(req) =
	  &(((XmPushButtonGCacheObject)extData->reqWidget)->pushbutton_cache);

}

/************************************************************************
 *
 *  InitializePosthook
 *
 ************************************************************************/
/* ARGSUSED */
static void InitializePosthook(req, new, args, num_args)
Widget      req, new;
ArgList     args;
Cardinal    *num_args;
{
    XmWidgetExtData     ext;
    XmPushButtonGadget  pbw = (XmPushButtonGadget)new;
    XmPushButtonGadget  rw =  (XmPushButtonGadget)req;

    /*
    * - register parts in cache.
    * - update cache pointers
    * - and free req
    */

    LabG_Cache(pbw) = (XmLabelGCacheObjPart *)
      _XmCachePart( LabG_ClassCachePart(pbw),
                    (caddr_t)LabG_Cache(pbw),
                    sizeof(XmLabelGCacheObjPart));

    PBG_Cache(pbw) = (XmPushButtonGCacheObjPart *)
	   _XmCachePart( PBG_ClassCachePart(pbw),
			 (caddr_t)PBG_Cache(pbw),
			 sizeof(XmPushButtonGCacheObjPart));

   /*
    * might want to break up into per-class work that gets explicitly
    * chained. For right now, each class has to replicate all
    * superclass logic in hook routine
    */
   
   /*
    * free the req subobject used for comparisons
    */
    ext = _XmGetWidgetExtData((Widget)pbw, XmCACHE_EXTENSION);
    XtFree(ext->reqWidget);
    XtDestroyWidget(ext->widget);
    /* extData gets freed at destroy */
}

      
/*************************************<->*************************************
 *
 *  Initialize 
 *
 *************************************<->***********************************/
/*ARGSUSED*/
static void Initialize (request, new, args, num_args)
XmPushButtonGadget request, new;
ArgList args;
Cardinal *num_args;

{
   XmGadgetPart            *pbgadget;
   int increase;
	int adjustment = 0;

    /* if menuProcs is not set up yet, try again */
    if (xmLabelGadgetClassRec.label_class.menuProcs == NULL)
	xmLabelGadgetClassRec.label_class.menuProcs =
	    (XmMenuProc) _XmGetMenuProcContext();


   PBG_Armed(new) = FALSE;
   PBG_Timer(new) = 0;


/*
 * Fix to introduce Resource XmNdefaultBorderWidth and compatibility
 *	variable.
 *  if defaultBorderWidth > 0, the program knows about this resource
 *	and is therefore a Motif 1.1 program; otherwise it is a Motif 1.0
 *      program and old semantics of XmNshowAsDefault prevails.
 *  - Sankar 2/1/90.
 */


   if (PBG_DefaultButtonShadowThickness(new) > 0)
      PBG_Compatible (new) = False;
   else
      PBG_Compatible (new) = True; 

   if ( PBG_Compatible (new)) 
	 PBG_DefaultButtonShadowThickness(new) = PBG_ShowAsDefault(new);

   /* no unarm_pixmap but do have an arm_pixmap, use that */
   if ((LabG_Pixmap(new) == XmUNSPECIFIED_PIXMAP) &&
       (PBG_ArmPixmap(new) != XmUNSPECIFIED_PIXMAP))
   {
      LabG_Pixmap(new) = PBG_ArmPixmap(new);
      if (request->rectangle.width == 0)
         new->rectangle.width = 0;
      if (request->rectangle.height == 0)
         new->rectangle.height = 0;
      (* xmLabelGadgetClassRec.rect_class.resize) (new);
   }
   PBG_UnarmPixmap(new) = LabG_Pixmap(new);


   if (PBG_DefaultButtonShadowThickness(new))
   { 
	/*
     * Special hack for 3d enhancement of location cursor high light.
     *  - Make the box bigger . During drawing of location cursor
     *    make it smaller.  See in Primitive.c
     *  May be we should use the macro: G_HighLightThickness(pbgadget);
     */

	 pbgadget = (XmGadgetPart *) (&(new->gadget));
	 pbgadget->highlight_thickness += XM_3D_ENHANCE_PIXEL;
	 adjustment += XM_3D_ENHANCE_PIXEL;

     increase =  2 * PBG_DefaultButtonShadowThickness(new) +
                     new->gadget.shadow_thickness;

	 increase += adjustment;

     if (request->rectangle.width == 0)
     {
        new->rectangle.width += 2 * increase;
        LabG_MarginLeft(new) =  LabG_MarginLeft(new) + increase;
        LabG_MarginRight(new) = LabG_MarginRight(new) + increase;
        LabG_TextRect_x(new) += increase;
     }
    else
      /* Add the increase to the rectangle to compensate for extra space */
     if (increase != 0)
      { new->rectangle.width += (increase << 1);
        LabG_MarginLeft(new) += increase;
        LabG_MarginRight(new) += increase;
        LabG_TextRect_x(new) += increase;
      }


     if (request->rectangle.height == 0)
     {
        new->rectangle.height +=  2 * increase;
        LabG_MarginTop(new) =  LabG_MarginTop(new) + increase;
        LabG_MarginBottom(new) = LabG_MarginBottom(new) + increase;
        LabG_TextRect_y(new) += increase;
     }
     else
    /* Add the increase to the rectangle to compensate for extra space */
      if (increase != 0)
      {new->rectangle.height += (increase << 1);
       LabG_MarginTop(new)   += increase;
       LabG_MarginBottom(new) += increase;
       LabG_TextRect_y(new) += increase;
     }

   }

   if (LabG_MenuType(new)  == XmMENU_POPUP ||
       LabG_MenuType(new)  == XmMENU_PULLDOWN)
   {
      new->gadget.traversal_on = TRUE;
   }

   /* Get the background fill GC */

   if (LabG_MenuType(new) != XmMENU_PULLDOWN &&
       LabG_MenuType(new) != XmMENU_POPUP)
   {
      GetFillGC (new);
      GetBackgroundGC (new);
   }
   
   /*  Initialize the interesting input types.  */

   new->gadget.event_mask = XmARM_EVENT | XmACTIVATE_EVENT | XmHELP_EVENT |
        XmFOCUS_IN_EVENT | XmFOCUS_OUT_EVENT | XmENTER_EVENT | XmLEAVE_EVENT
        | XmMULTI_ARM_EVENT |  XmMULTI_ACTIVATE_EVENT;


}



/************************************************************************
 *
 *  GetFillGC
 *     Get the graphics context used for filling in background of button.
 *
 ************************************************************************/

static void GetFillGC (pb)
XmPushButtonGadget pb;

{
   XGCValues values;
   XtGCMask  valueMask;
   XmManagerWidget mw = (XmManagerWidget) XtParent(pb);
   
   valueMask = GCForeground | GCBackground | GCFillStyle;

   values.foreground = PBG_ArmColor(pb);
   values.background = mw -> core.background_pixel;
   values.fill_style = FillSolid;

   PBG_FillGc(pb) = XtGetGC (mw, valueMask, &values);
}



/************************************************************************
 *
 *  GetBackgroundGC
 *     Get the graphics context used for filling in background of 
 *     the pushbutton when not armed.
 *
 ************************************************************************/

static void GetBackgroundGC (pb)
XmPushButtonGadget pb;

{
   XGCValues       values;
   XtGCMask        valueMask;
   short             myindex;
   XFontStruct     *fs;
   XmManagerWidget mw = (XmManagerWidget) XtParent(pb);


   valueMask = GCForeground | GCBackground | GCFont | GCGraphicsExposures;

   _XmFontListSearch(LabG_Font(pb),
		     XmSTRING_DEFAULT_CHARSET,
		     &myindex ,
		     &fs); 
   values.foreground = mw->core.background_pixel;
   values.background = mw->manager.foreground;
   values.graphics_exposures = False;
   if (fs==NULL)
     valueMask &= ~GCFont;
   else
     values.font     = fs->fid;

   PBG_BackgroundGc(pb) = XtGetGC(mw,valueMask,&values);
}


/************************************************************************
 *
 *  VisualChange
 *	This function is called from XmManagerClass set values when
 *	the managers visuals have changed.  The gadget regenerates any
 *	GC based on the visual changes and returns True indicating a
 *	redraw is needed.  Otherwise, False is returned.
 *
 ************************************************************************/

static Boolean VisualChange (gw, curmw, newmw)
XmGadget gw;
XmManagerWidget curmw;
XmManagerWidget newmw;

{
   XmPushButtonGadget pbg = (XmPushButtonGadget) gw;
   XmManagerWidget mw = (XmManagerWidget) XtParent(gw);

   /*  See if the GC need to be regenerated and widget redrawn.  */

   if (curmw -> core.background_pixel != newmw -> core.background_pixel &&
	LabG_MenuType(pbg) != XmMENU_PULLDOWN &&
	LabG_MenuType(pbg) != XmMENU_POPUP)
   {
      XtReleaseGC (mw, PBG_FillGc(pbg));
      GetFillGC (pbg);	
      GetBackgroundGC(pbg);
      return (True);
   }
   return (False);
}





/************************************************************************
 *
 *  SetValuesPrehook
 *
 ************************************************************************/
/* ARGSUSED */
static Boolean SetValuesPrehook(oldParent,refParent,newParent, args, num_args)
    Widget      oldParent, refParent, newParent;
    ArgList     args;
    Cardinal    *num_args;
{   XmWidgetExtData             extData;
    XmBaseClassExt              *cePtr;
    WidgetClass                 ec;
    XmPushButtonGCacheObject    new;

    cePtr = _XmGetBaseClassExtPtr(XtClass(newParent), XmQmotif);
    ec = (*cePtr)->secondaryObjectClass;

    /* allocate copies and fill from cache */
     new = (XmPushButtonGCacheObject) XtMalloc(ec->core_class.widget_size);
     new->object.self = (Widget)new;
     new->object.widget_class = ec;
     new->object.parent = XtParent(newParent);
     new->object.xrm_name = newParent->core.xrm_name;
     new->object.being_destroyed = False;
     new->object.destroy_callbacks = NULL;
     new->object.constraints = NULL;

     new->ext.logicalParent = newParent;
     new->ext.extensionType = XmCACHE_EXTENSION;

     bcopy((char *)LabG_Cache(newParent),
           (char *)&(new->label_cache),
	    sizeof(XmLabelGCacheObjPart));

     bcopy((char *)PBG_Cache(newParent),
	   (char *)&(new->pushbutton_cache),
           sizeof(XmPushButtonGCacheObjPart));

     extData = (XmWidgetExtData) XtCalloc(sizeof(XmWidgetExtDataRec), 1);
    _XmPushWidgetExtData(newParent, extData, XmCACHE_EXTENSION);

     XtSetValues(new, args, *num_args);

     LabG_Cache(newParent) = &(((XmLabelGCacheObject)new)->label_cache);
     LabG_Cache(refParent) = &(((XmLabelGCacheObject)extData->reqWidget)->label_cache);

     PBG_Cache(newParent) =
	 &(((XmPushButtonGCacheObject)new)->pushbutton_cache);
     PBG_Cache(refParent) =
	 &(((XmPushButtonGCacheObject)extData->reqWidget)->pushbutton_cache);

     return FALSE;
}

/************************************************************************
 *
 *  GetValuesPrehook
 *
 ************************************************************************/
/* ARGSUSED */
static void GetValuesPrehook(newParent, args, num_args)
    Widget      newParent;
    ArgList     args;
    Cardinal    *num_args;
{   
    XmWidgetExtData             extData;
    XmBaseClassExt              *cePtr;
    WidgetClass                 ec;
    XmPushButtonGCacheObject    new;

    cePtr = _XmGetBaseClassExtPtr(XtClass(newParent), XmQmotif);
    ec = (*cePtr)->secondaryObjectClass;

    /* allocate copies and fill from cache */
     new = (XmPushButtonGCacheObject) XtMalloc(ec->core_class.widget_size);
     new->object.self = (Widget)new;
     new->object.widget_class = ec;
     new->object.parent = XtParent(newParent);
     new->object.xrm_name = newParent->core.xrm_name;
     new->object.being_destroyed = False;
     new->object.destroy_callbacks = NULL;
     new->object.constraints = NULL;

     new->ext.logicalParent = newParent;
     new->ext.extensionType = XmCACHE_EXTENSION;

     bcopy((char *)LabG_Cache(newParent),
           (char *)&(new->label_cache),
	    sizeof(XmLabelGCacheObjPart));

     bcopy((char *)PBG_Cache(newParent),
	   (char *)&(new->pushbutton_cache),
           sizeof(XmPushButtonGCacheObjPart));

     extData = (XmWidgetExtData) XtCalloc(sizeof(XmWidgetExtDataRec), 1);
     _XmPushWidgetExtData(newParent, extData, XmCACHE_EXTENSION);

     XtGetValues(new, args, *num_args);


}

/************************************************************************
 *
 *  GetValuesPosthook
 *
 ************************************************************************/
/*ARGSUSED*/
static void GetValuesPosthook(new, args, num_args)
Widget      new;
ArgList     args;
Cardinal    *num_args;
{
     XmWidgetExtData             ext;

     _XmPopWidgetExtData(new, &ext, XmCACHE_EXTENSION);

     XtFree(ext->widget);
     XtFree(ext);
}


/************************************************************************
 *
 *  SetValuesPosthook
 *
 ************************************************************************/
/*ARGSUSED*/
static Boolean SetValuesPosthook(current, req, new, args, num_args)
   Widget      current, req, new;
   ArgList     args;
   Cardinal    *num_args;
{  XmWidgetExtData                  ext;
   XmPushButtonGCacheObjPart        *oldCachePtr, *newCachePtr;

  /*
   * - register parts in cache.
   * - update cache pointers
   * - and free req
   */
  /* assign if changed! */
  if (!_XmLabelCacheCompare(LabG_Cache(new), LabG_Cache(current)))
  {
       _XmCacheDelete((caddr_t)LabG_Cache(current));  /* delete the old one */
       LabG_Cache(new) = (XmLabelGCacheObjPart *)
                         _XmCachePart(LabG_ClassCachePart(new), (caddr_t)LabG_Cache(new),
                              sizeof(XmLabelGCacheObjPart));
  }
  else
       LabG_Cache(new) = LabG_Cache(current);


  /* assign if changed! */
  if (!_XmPushBCacheCompare(PBG_Cache(new),
		      PBG_Cache(current)))
  {
      _XmCacheDelete((caddr_t)PBG_Cache(current));  /* delete the old one */
      PBG_Cache(new) = (XmPushButtonGCacheObjPart *)
           	     _XmCachePart(PBG_ClassCachePart(new),
	                         (caddr_t)PBG_Cache(new),
				 sizeof(XmPushButtonGCacheObjPart));
  }
  else
       PBG_Cache(new) = PBG_Cache(current);

  _XmPopWidgetExtData(new, &ext, XmCACHE_EXTENSION);

  XtFree(ext->widget);
  XtFree(ext->reqWidget);
  XtFree(ext->oldWidget);
  XtFree(ext);

  return FALSE;
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
XmPushButtonGadget current, request, new;
{
   XmGadgetPart  *pbgadget;
   int increase;
   Boolean  flag = FALSE;    /* our return value */
   XmManagerWidget curmw = (XmManagerWidget) XtParent(current);
   XmManagerWidget newmw = (XmManagerWidget) XtParent(new);
	int adjustment;

/*
 * Fix to introduce Resource XmNdefaultBorderWidth and compatibility
 *      variable.
 *  if  defaultBorderWidth of the current and new are different, then
 *  the programmer is setting the resource XmNdefaultBorderWidth; i.e. it
 *  defaultBorderWidth > 0, the program knows about this resource
 *  a Motif 1.1 program; otherwise it is a Motif 1.0
 *      program and old semantics of XmNshowAsDefault prevails.
 *     Note if (PBG_ShowAsDefault(gadget) == 0 ) then we are NOT currently 
 *      drawing defaultBorderWidth; if it is > 0, we should be drawing
 *	    the shadow in defaultorderWidth; 
 *  - Sankar 2/1/90.
 */


   if ( PBG_DefaultButtonShadowThickness(new) !=
                     PBG_DefaultButtonShadowThickness(current))

     PBG_Compatible (new) = False;

   if ( PBG_Compatible (new))
         PBG_DefaultButtonShadowThickness(new) = PBG_ShowAsDefault(new);

   adjustment = AdjustHighLightThickness (new, current);

   if (PBG_DefaultButtonShadowThickness(new) !=
	        PBG_DefaultButtonShadowThickness(current))
   {

     pbgadget = (XmGadgetPart *) (&(new->gadget));
      if (PBG_DefaultButtonShadowThickness(new) >
		PBG_DefaultButtonShadowThickness(current))
      {
         if (PBG_DefaultButtonShadowThickness(current) > 0)
            increase =  (2 * PBG_DefaultButtonShadowThickness(new) +
                         new->gadget.shadow_thickness) -
                        (2 * PBG_DefaultButtonShadowThickness(current) +
                         current->gadget.shadow_thickness);
         else
            increase =  (2 * PBG_DefaultButtonShadowThickness(new) +
                         new->gadget.shadow_thickness);
      }
      else
      {
         if (PBG_DefaultButtonShadowThickness(new) > 0)
            increase = - ((2 * PBG_DefaultButtonShadowThickness(current) +
                           current->gadget.shadow_thickness) -
                          (2 * PBG_DefaultButtonShadowThickness(new) +
                           new->gadget.shadow_thickness));
         else
            increase = - (2 * PBG_DefaultButtonShadowThickness(current) +
                          current->gadget.shadow_thickness);
      }
     
	increase += adjustment;
 
      if (LabG_RecomputeSize(new) || request->rectangle.width == 0)
      {
	 new->rectangle.width += (increase << 1) ;
         LabG_MarginLeft(new) = LabG_MarginLeft(new) + increase;
         LabG_MarginRight(new) = LabG_MarginRight(new) + increase;
         flag = TRUE;
      }
      else
      /* add the change to the rectangle */
      if (increase != 0)
       {  new->rectangle.width += (increase << 1);
          LabG_MarginLeft(new) += increase;
          LabG_MarginRight(new) += increase;
          flag = TRUE;
       }

      if (LabG_RecomputeSize(new) || request->rectangle.height == 0)
      {
	 new->rectangle.height +=  (increase << 1);
         LabG_MarginTop(new) = LabG_MarginTop(new) + increase;
         LabG_MarginBottom(new) = LabG_MarginBottom(new) + increase;
         flag = TRUE;
      }
    else
      /* add the change to the rectangle */
      if (increase != 0)
      { new->rectangle.height += (increase << 1);
        LabG_MarginTop(new)  += increase;
        LabG_MarginBottom(new) += increase;
        flag = TRUE;
      }

      _XmReCacheLabG((XmLabelGadget)new);
   }

   if ((PBG_ArmPixmap(new) != PBG_ArmPixmap(current)) &&
      (LabG_LabelType(new) == XmPIXMAP) && (PBG_Armed(new))) 
      flag = TRUE;
      
   /* no unarm_pixmap but do have an arm_pixmap, use that */
   if ((LabG_Pixmap(new) == XmUNSPECIFIED_PIXMAP) &&
       (PBG_ArmPixmap(new) != XmUNSPECIFIED_PIXMAP))
   {
      LabG_Pixmap(new) = PBG_ArmPixmap(new);
      if (LabG_RecomputeSize(new) &&
          request->rectangle.width == current->rectangle.width)
         new->rectangle.width = 0;
      if (LabG_RecomputeSize(new) &&
          request->rectangle.height == current->rectangle.height)
         new->rectangle.width = 0;
      (* xmLabelGadgetClassRec.rect_class.resize) (new);
   }

   if (LabG_Pixmap(new) != LabG_Pixmap(current))
   {
      PBG_UnarmPixmap(new) = LabG_Pixmap(new);
      if ((LabG_LabelType(new) == XmPIXMAP) && (!PBG_Armed(new)))
	 flag = TRUE;
   }

   if ((PBG_FillOnArm(new) != PBG_FillOnArm(current)) &&
       (PBG_Armed(new) == TRUE))
	 flag = TRUE;

   if (LabG_MenuType(new) != XmMENU_PULLDOWN &&
       LabG_MenuType(new) != XmMENU_POPUP)
   {
      /*  See if the GC need to be regenerated and widget redrawn.  */

      if (PBG_ArmColor(new) != PBG_ArmColor(current))
      {
	 flag = TRUE;
	 XtReleaseGC (newmw, PBG_FillGc(new));
	 GetFillGC (new);
      }

      if (newmw -> core.background_pixel != curmw -> core.background_pixel) 
      {
	flag = TRUE;
	XtReleaseGC (newmw, PBG_BackgroundGc(new));
	GetBackgroundGC (new);
      }
   }
   
   /*  Initialize the interesting input types.  */

   new->gadget.event_mask = XmARM_EVENT | XmACTIVATE_EVENT | XmHELP_EVENT |
        XmFOCUS_IN_EVENT | XmFOCUS_OUT_EVENT | XmENTER_EVENT | XmLEAVE_EVENT
         | XmMULTI_ARM_EVENT | XmMULTI_ACTIVATE_EVENT;

    if (flag == False)
        /* No size change has taken place. */
    {

          if ( (PBG_ShowAsDefault(current) != 0) &&
                    (PBG_ShowAsDefault(new) == 0) )
              EraseDefaultButtonShadow (new);

      if  ( (PBG_ShowAsDefault(current) == 0) &&
                     (PBG_ShowAsDefault(new) != 0) )
              DrawDefaultButtonShadow (new);
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
XmPushButtonGadget pb;
XEvent * event;

{
   XmPushButtonCallbackStruct call_value;

   if (LabG_MenuType(pb) == XmMENU_PULLDOWN ||
       LabG_MenuType(pb) == XmMENU_POPUP)
   {
      (* xmLabelGadgetClassRec.label_class.menuProcs)
	  (XmMENU_POPDOWN, XtParent(pb), NULL, (caddr_t)event, NULL);
   }

   call_value.reason = XmCR_HELP;
   call_value.event = event;
   _XmSocorro((Widget)pb, event);
}



/************************************************************************
 *
 *  Destroy
 *	Clean up allocated resources when the widget is destroyed.
 *
 ************************************************************************/

static void Destroy (pb)
XmPushButtonGadget pb;

{
   XmManagerWidget mw = (XmManagerWidget) XtParent(pb);

   if (PBG_Timer(pb))
      XtRemoveTimeOut (PBG_Timer(pb));

   if (LabG_MenuType(pb) != XmMENU_PULLDOWN &&
       LabG_MenuType(pb) != XmMENU_POPUP)
      XtReleaseGC (mw, PBG_FillGc(pb));

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
Widget XmCreatePushButtonGadget (parent, name, arglist, argcount)
Widget parent;
char * name;
ArgList arglist;
Cardinal argcount;

#else /* _NO_PROTO */
Widget XmCreatePushButtonGadget (Widget parent, char *name, ArgList arglist, Cardinal argcount)
#endif /* _NO_PROTO */
{
   return (XtCreateWidget (name, xmPushButtonGadgetClass, 
                           parent, arglist, argcount));
}

/*ARGSUSED*/
static void ActivateCommonG (pb, event, event_mask)
XmPushButtonGadget pb;
XEvent * event;
Mask event_mask;
{
      if ( ( LabG_MenuType(pb) == XmMENU_PULLDOWN) ||
              ( LabG_MenuType(pb) == XmMENU_POPUP))
         if (event->type == ButtonRelease)
            BtnUp (pb, event);
         else  /* assume KeyRelease */
            KeySelect (pb, event);
      else
      {
         if (event->type == ButtonRelease)
         {
            Activate (pb, event);
            Disarm (pb, event);
         }
         else  /* assume KeyPress or KeyRelease */
        ArmAndActivate (pb, event);
      }
}
/****************************************************
 *   Functions for manipulating Secondary Resources.
 *********************************************************/
/*
 * GetPushBGSecResData()
 *    Create a XmSecondaryResourceDataRec for each secondary resource;
 *    Put the pointers to these records in an array of pointers;
 *    Return the pointer to the array of pointers.
 */
/*ARGSUSED*/
static Cardinal
GetPushBGClassSecResData  ( class, data_rtn)
WidgetClass class;
XmSecondaryResourceData  **data_rtn;
{   int arrayCount = 0;
    int resNum;
    XmSecondaryResourceData secData, *sd;
    XmBaseClassExt  bcePtr;
    String  resource_class, resource_name;
    XtPointer  client_data;

    bcePtr = &(PushBGClassExtensionRec );
    client_data = NULL;
    resource_class = NULL;
    resource_name = NULL;
    arrayCount =
      _XmSecondaryResourceData ( bcePtr, data_rtn, client_data,  
				resource_name, resource_class,
				(XmResourceBaseProc) (GetPushBGClassSecResBase));

    return (arrayCount);
}

/*
 * GetPushBGClassResBase ()
 *   retrun the address of the base of resources.
 */
static XtPointer
GetPushBGClassSecResBase ( widget, client_data)
Widget  widget;
XtPointer client_data;
{	XtPointer  widgetSecdataPtr; 
    int  labg_cache_size = sizeof (XmLabelGCacheObjPart);
    int  pushbg_cache_size = sizeof (XmPushButtonGCacheObjPart);
	char *cp;

    widgetSecdataPtr = (XtPointer) 
			(XtMalloc ( labg_cache_size + pushbg_cache_size + 1));

    if (widgetSecdataPtr)
	  { cp = (char *) widgetSecdataPtr;
        bcopy ( (char *) ( LabG_Cache(widget)), (char *) cp, labg_cache_size);
	    cp += labg_cache_size;
	    bcopy ((char *) PBG_Cache(widget), (char *) cp, pushbg_cache_size);
	  }
/* else Warning: error cannot allocate Memory */
/*     widgetSecdataPtr = (XtPointer) ( LabG_Cache(widget)); */

	return (widgetSecdataPtr);
}


/*
 * EraseDefaultButtonShadow (pb)
 *  - Called from SetValues() - effort to optimize shadow drawing.
 */

static void
EraseDefaultButtonShadow (pb)
XmPushButtonGadget pb;
{  int done = False;
   int size, x, y, width, height;
/**      if (!(XtIsRealized(pb)) ) done = True;   **/
	  if (!(XtIsManaged (pb)) ) done = True;
      if (!(done))
      {
         if (LabG_MenuType(pb) == XmMENU_PULLDOWN ||
                     LabG_MenuType(pb) == XmMENU_POPUP)
          {
             ShellWidget mshell = (ShellWidget)XtParent(XtParent(pb));
             if (!mshell->shell.popped_up) done = True;
          }
      }

      if (!(done))
        {   size = (int) (PBG_DefaultButtonShadowThickness(pb));
            x = (int) (pb -> rectangle.x ) + 
						(int) (pb -> gadget.highlight_thickness) ;
        y =  (int) (pb -> rectangle.y) + 
					(int) (pb -> gadget.highlight_thickness);
        width = (int) ( pb -> rectangle.width) - 2 *
                           ( (int) pb->gadget.highlight_thickness);
            height = (int) (pb -> rectangle.height) - 
						2 * ( (int) (pb->gadget.highlight_thickness));

           _XmEraseShadow (XtDisplay (pb), XtWindow (pb),
                           size, x, y, width, height);
        }
}
/*
 * DrawDefaultButtonShadow (pb)
 *  - Called from SetValues() - effort to optimize shadow drawing.
 */

static void
DrawDefaultButtonShadow (pb)
XmPushButtonGadget pb;
{  int done = False;
	int size, x, y, width, height;

/*      if (!(XtIsRealized(pb))) done = True;  **/
	  if (!(XtIsManaged(pb))) done = True;
      if (!(done))
      {
         if (LabG_MenuType(pb) == XmMENU_PULLDOWN ||
                     LabG_MenuType(pb) == XmMENU_POPUP)
          {
             ShellWidget mshell = (ShellWidget)XtParent(XtParent(pb));
             if (!mshell->shell.popped_up) done = True;
          }
      }

      if (!(done))
        {   size = (int) (PBG_DefaultButtonShadowThickness(pb));
		    x = (int) (pb -> rectangle.x + pb -> gadget.highlight_thickness);
	        y = (int) (pb -> rectangle.y + pb -> gadget.highlight_thickness);
		    width =  (int)( pb -> rectangle.width -
							 ( pb->gadget.highlight_thickness << 1));
		    height = (int) (pb -> rectangle.height -
				             (pb->gadget.highlight_thickness << 1));

		    _XmDrawShadow (XtDisplay (pb), XtWindow (pb),
				XmParentBottomShadowGC(pb),
			    XmParentTopShadowGC(pb),
			    size, x, y, width, height);

        }
}


void _XmClearBGCompatibility( pbg)
XmPushButtonGadget pbg ;
{
	 PBG_Compatible (pbg) = False;
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
XmPushButtonGadget  new, current;
{	XmGadgetPart  *pbnew, *pbcurrent;
	Dimension oldhilite;
	int adjustment = 0;

	pbnew = (XmGadgetPart *) (&(new->gadget));
	pbcurrent = (XmGadgetPart *) (&(current->gadget));

	if (PBG_DefaultButtonShadowThickness(new) )
	{  if ( !(PBG_DefaultButtonShadowThickness(current)))
		 { pbnew->highlight_thickness += XM_3D_ENHANCE_PIXEL;
	       adjustment += XM_3D_ENHANCE_PIXEL;
		 }
	   else
	      if (pbnew->highlight_thickness !=
			pbcurrent->highlight_thickness)
		     {  pbnew->highlight_thickness += XM_3D_ENHANCE_PIXEL;
	     	    adjustment += XM_3D_ENHANCE_PIXEL;
			 }
	}
	else
	 { if (PBG_DefaultButtonShadowThickness(current))
		/* default_button_shadow_thickness was > 0 and is now
		 * being set to 0. 
		 * - so take away the adjustment for enhancement.
		 */
	     { if ( pbnew->highlight_thickness ==
			pbcurrent->highlight_thickness)
		       { pbnew->highlight_thickness -= XM_3D_ENHANCE_PIXEL;
				 adjustment -= XM_3D_ENHANCE_PIXEL;
			   }
	     }
		/*
	         * This will have a bug if in a XtSetValues the application
		 * removes the default_button_shadow_thickness and also
		 * sets the high-light-thickness to a value of
 		 * (old-high-light-thickness (from previous XtSetValue) +
		 *  XM_3D_ENHANCE_PIXEL).
		 * This will be documented.
		 */
	   }
    return (adjustment);
}

struct  PBbox
     { int pbx;
       int pby;
       int pbWidth;
       int pbHeight;
     };

/*ARGSUSED*/
static void Redisplay (pb, event, region)
XmPushButtonGadget pb;
XEvent *event;
Region region;

{
   if (XtIsRealized(pb))
   {
      if (LabG_MenuType(pb) == XmMENU_PULLDOWN ||
      LabG_MenuType(pb) == XmMENU_POPUP)
      {
         ShellWidget mshell = (ShellWidget)XtParent(XtParent(pb));

         if (!mshell->shell.popped_up)
        	 return;
      }
      if ((LabG_MenuType(pb) == XmMENU_PULLDOWN ) ||
          (LabG_MenuType(pb) == XmMENU_POPUP) )
		   DrawPushButtonLabelGadget (pb);
	 else
	  {
  	      DrawPushBGBackground (pb);
	      DrawPushButtonLabelGadget (pb);
 		  DrawPushButtonGadgetShadows (pb);
	  }
     if (pb -> gadget.highlighted)
            _XmHighlightBorder ((Widget)pb);
         else if (_XmDifferentBackground ((Widget)pb, XtParent (pb)))
            _XmUnhighlightBorder ((Widget)pb);
    }
}
/*
 * DrawPushButtonLabelGadget()
 */

static void DrawPushButtonLabelGadget (pb)
XmPushButtonGadget pb;
{	GC tmp_gc;
    Boolean   replaceGC = False;
    XmManagerWidget mw = (XmManagerWidget) XtParent(pb);
    Boolean saved_gadgetshadow = False;
    int gadget_shadow_thickness  = 0;

      if ((LabG_MenuType(pb) != XmMENU_PULLDOWN &&
       LabG_MenuType(pb) != XmMENU_POPUP) &&
         PBG_FillOnArm(pb))
	  {
       if ((LabG_LabelType(pb) == XmSTRING) &&
             (PBG_Armed(pb)) &&
         (PBG_ArmColor(pb) == mw->manager.foreground))
         {
            tmp_gc = LabG_NormalGC(pb);
            LabG_NormalGC(pb) = PBG_BackgroundGc(pb);
            replaceGC = True;
         }
       }
     if (LabG_LabelType(pb) == XmPIXMAP)
     {
        if (PBG_Armed(pb))
           if (PBG_ArmPixmap(pb) != XmUNSPECIFIED_PIXMAP)
          LabG_Pixmap(pb) = PBG_ArmPixmap(pb);
           else
          LabG_Pixmap(pb) = PBG_UnarmPixmap(pb);

        else   /* pushbutton is unarmed */
           LabG_Pixmap(pb) = PBG_UnarmPixmap(pb);
      }

     if( !(pb->pushbutton.compatible) )
	{ saved_gadgetshadow = True;
	  gadget_shadow_thickness = pb -> gadget.shadow_thickness;
          pb -> gadget.shadow_thickness = 
                     pb -> gadget.shadow_thickness
                       + PBG_DefaultButtonShadowThickness(pb);
          pb -> gadget.shadow_thickness =
                     (pb -> gadget.shadow_thickness << 1);
        }

     (* xmLabelGadgetClassRec.rect_class.expose) (pb, NULL, NULL);

      if (saved_gadgetshadow)
        pb -> gadget.shadow_thickness = gadget_shadow_thickness;

      if (replaceGC)
         LabG_NormalGC(pb) = tmp_gc;
}
/*
 * DrawPushButtonGadgetShadows()
 *  Note: PushButton has two types of shadows: primitive-shadow and
 *  default-button-shadow.
 *  Following shadows  are drawn:
 *  if pushbutton is in a menu only primitive shadows are drawn;
 *   else
 *    { draw default shadow if needed;
 *  draw primitive shadow ;
 *    }
 */
static void DrawPushButtonGadgetShadows (pb)
XmPushButtonGadget pb;
{

    if (PBG_DefaultButtonShadowThickness(pb))
	{ EraseDefaultButtonShadows (pb);
	  if (PBG_ShowAsDefault(pb))
		DrawDefaultButtonShadows (pb);
	}

    if (pb->gadget.shadow_thickness > 0)
	   DrawPBGadgetShadows(pb);
}
/*
 * DrawPBGadgetShadows (pb)
 *   - Should be called only if PrimitiveShadowThickness > 0
 */
static void DrawPBGadgetShadows (pb)
XmPushButtonGadget pb;
{
      GC topgc, bottomgc;
      int dx, dy, width, height, adjust, shadow_thickness;

	  if (PBG_Armed(pb))
	  { bottomgc  = XmParentTopShadowGC(pb); 
	    topgc = XmParentBottomShadowGC(pb);
	  }
	  else
	  { bottomgc  =  XmParentBottomShadowGC(pb);
	    topgc = XmParentTopShadowGC(pb);
	  }

	  shadow_thickness = pb -> gadget.shadow_thickness;

     if ( (shadow_thickness > 0) && (topgc) && (bottomgc))
	 { if (pb->pushbutton.compatible)
			 adjust = PBG_ShowAsDefault(pb);
	     else
			adjust = PBG_DefaultButtonShadowThickness(pb);
	  if (adjust > 0)
             {   adjust = (adjust << 1);
                 dx =  pb -> gadget.highlight_thickness +
                             adjust +  pb -> gadget.shadow_thickness;
             }
           else
            dx = pb->gadget.highlight_thickness;
	    dy = dx;
	    width = pb -> rectangle.width - (dx << 1);
	    height = pb -> rectangle.height - (dy << 1);

	   
	   if ( (width > 0) && (height > 0))	
       { dx += pb->rectangle.x;
	     dy += pb->rectangle.y;
	     _XmDrawShadow (XtDisplay (pb), XtWindow (pb), topgc,
		   bottomgc, shadow_thickness, dx, dy, width, height);
	   }
    }
}

static void EraseDefaultButtonShadows (pb)
XmPushButtonGadget pb;
{
    int dx, dy, width, height, default_button_shadow;
    int disp;

    if (pb->pushbutton.compatible)
      default_button_shadow = (int) (PBG_ShowAsDefault(pb));
	else
	  default_button_shadow = (int) (PBG_DefaultButtonShadowThickness(pb));

	if (default_button_shadow > 0)
	{ dx = pb->gadget.highlight_thickness;
	 disp = (dx << 1);
	  width = pb->rectangle.width -disp;
	  height = pb->rectangle.height - disp;

	  if ( (width > 0) && (height > 0))	
	  { dy = dx;
	    dx += pb->rectangle.x;
	    dy += pb->rectangle.y;
	    _XmEraseShadow (XtDisplay (pb), XtWindow (XtParent(pb)),
		  default_button_shadow, dx, dy, width, height);
	  }
	}
}


/*
 * DrawDefaultButtonShadows()
 *  - get the topShadowColor and bottomShadowColor from the parent;
 *    use those colors to construct top and bottom gc; use these
 *    GCs to draw the shadows of the button.
 *  - Should not be called if pushbutton is in a row column or in a menu.
 *  - Should be called only if a defaultbuttonshadow is to be drawn.
 */

static void DrawDefaultButtonShadows (pb)
XmPushButtonGadget pb;
{
      GC topgc, bottomgc;
      int dx, dy, width, height, default_button_shadow_thickness;


	  topgc  = XmParentBottomShadowGC(pb);
	  bottomgc = XmParentTopShadowGC(pb);

	  if ( (bottomgc == NULL) || (topgc == NULL) ) return;
	
      if (pb->pushbutton.compatible)
	      default_button_shadow_thickness = (int) (PBG_ShowAsDefault(pb));
	   else
	      default_button_shadow_thickness = 
				(int) (PBG_DefaultButtonShadowThickness(pb));

    /*
     *
     * Compute location of bounding box to contain the defaultButtonShadow.
     */
      if (default_button_shadow_thickness > 0)
        { dx = pb->gadget.highlight_thickness;
	      dy = dx;
		  width = pb->rectangle.width - (dx << 1);
		  height= pb->rectangle.height - (dx << 1);
		 
	      if ( (width > 0) && (height > 0))	
	      { dx += pb->rectangle.x;
		    dy += pb->rectangle.y;

		    _XmDrawShadow (XtDisplay (pb), XtWindow (pb), topgc, bottomgc,
					default_button_shadow_thickness, dx, dy, width, height);
		  }
	    }
}
/*
 *
 *  DrawPushBGBackground (pb)
 *  Note: This routine should never be called if the PushButtonGadget is
 *	  in a menu (Pull-down or Popup) since in these cases the background
 *	  GC in the PushButtonGadget will contain uncertain (uninitialized)
 *	  values. This will result in a core-dump when the XFillRectangle()
 *	  is called - since Xlib will try to do XFlushGC. 
 * 
 */

static void DrawPushBGBackground (pb)
XmPushButtonGadget pb;
{  GC  tmp_gc;
	struct PBbox box;
     Boolean result;

	if ((LabG_MenuType(pb) == XmMENU_PULLDOWN ) ||
	      (LabG_MenuType(pb) == XmMENU_POPUP) )
	{  tmp_gc = NULL; 
	}
    else
	{ if ((PBG_Armed(pb)) && (PBG_FillOnArm(pb))) tmp_gc = PBG_FillGc(pb);
		 else tmp_gc =  PBG_BackgroundGc(pb);
	  if ( tmp_gc)
	  { 
	    result = ComputePBLabelArea (pb, &box);
    	if ( (result) &&  (box.pbWidth > 0) && (box.pbHeight > 0))
           XFillRectangle (XtDisplay(pb), XtWindow(pb), tmp_gc,
 	                 box.pbx, box.pby, box.pbWidth, box.pbHeight);
	  }
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


static Boolean
ComputePBLabelArea (pb, box)
XmPushButtonGadget pb;
struct  PBbox *box;
{   Boolean result = True;
    int dx, dy, adjust;

    if (pb == NULL) result = False;
    else
    { if (PBG_DefaultButtonShadowThickness(pb) > 0)
         { adjust = PBG_DefaultButtonShadowThickness(pb) +
                    pb -> gadget.shadow_thickness;
           adjust = (adjust << 1);
           dx = pb -> gadget.highlight_thickness + adjust;
         }
        else
           dx = pb -> gadget.highlight_thickness +
                    pb -> gadget.shadow_thickness;

       box->pbx = dx + pb->rectangle.x;
       box->pby = dx +pb->rectangle.y;
       adjust = (dx << 1);
       box->pbWidth = pb->rectangle.width - adjust;
       box->pbHeight= pb->rectangle.height - adjust;
     }
    return (result);
}

static void
ExportHighlightThickness ( widget, offset, value)
            Widget          widget ;
            int             offset ;
            XtArgVal *      value ;

{
XmPushButtonGadget pb;

        pb = (XmPushButtonGadget) widget;

        if (PBG_DefaultButtonShadowThickness(pb) ||
        	PBG_ShowAsDefault(pb)) {
           if ((int)*value >= XM_3D_ENHANCE_PIXEL)
             *value -= XM_3D_ENHANCE_PIXEL;
	}

        _XmFromHorizontalPixels (widget, offset, value);
}
