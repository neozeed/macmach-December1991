#ifdef REV_INFO
#ifndef lint
static char SCCSID[] = "OSF/Motif: @(#)ToggleBG.c	3.36.1.1 91/02/21";
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
#include <Xm/LabelGP.h>
#include <Xm/ToggleBGP.h>
#include <Xm/CacheP.h>
#include <Xm/ExtObjectP.h>
#include <Xm/BaseClassI.h>
#include <Xm/CascadeB.h>

#define XmINVALID_TYPE  255	/* dynamic default flag for IndicatorType */
#define XmINVALID_BOOLEAN 85    /* dynamic default flag for VisibleWhenOff */

#define PixmapOn(w)           (TBG_OnPixmap(w))
#define PixmapOff(w)          (LabG_Pixmap(w))
#define Pixmap_Insen_On(w)     (TBG_InsenPixmap(w))
#define Pixmap_Insen_Off(w)    (LabG_PixmapInsensitive(w))
#define IsNull(p)              (p == XmUNSPECIFIED_PIXMAP)
#define IsOn(w)                (TBG_VisualSet(w))

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
static void Disarm();
static void Select();
static void Leave();
static void Enter();
static void Help();
static void DrawToggle();
static void GetGC();
static void BtnDown();
static void BtnUp();
static void BorderHighlight();
static void BorderUnhighlight();
static void ArmAndActivate();
static void KeySelect();
static void DrawToggleShadow();

static void     InputDispatch();
static Boolean  VisualChange();

extern _XmSelectColorDefault();

int _XmToggleBCacheCompare();

/* Class Procedure for creating the cache object */

static void SecondaryObjectCreate();

/* Initialize and SetValues Pre and Post hooks */

static void InitializePosthook();
static Boolean SetValuesPrehook();
static Boolean SetValuesPosthook();
static void GetValuesPrehook();
static void GetValuesPosthook();

/*
 * Procedures for manipulating cached_resources as secondary data.
 */
static Cardinal GetToggleBGClassSecResData();
static XtPointer GetToggleBGClassSecResBase ( );
extern Cardinal _XmSecondaryResourceData ();

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

static XtResource cache_resources[] = 
{
 {
     XmNindicatorSize,
     XmCIndicatorSize,
     XmRVerticalDimension,
     sizeof(Dimension),
     XtOffset (XmToggleButtonGCacheObject, toggle_cache.indicator_dim),
     XmRImmediate, (caddr_t) XmINVALID_DIMENSION
   },

   {
     XmNindicatorType, XmCIndicatorType, XmRIndicatorType,sizeof(unsigned char),
     XtOffset (XmToggleButtonGCacheObject, toggle_cache.ind_type),
     XmRImmediate, (caddr_t) XmINVALID_TYPE
   },

   {
     XmNvisibleWhenOff, XmCVisibleWhenOff, XmRBoolean, sizeof(Boolean),
     XtOffset (XmToggleButtonGCacheObject, toggle_cache.visible),
     XmRImmediate, (caddr_t) XmINVALID_BOOLEAN
   },

   {
     XmNspacing, 
     XmCSpacing, 
     XmRHorizontalDimension,
     sizeof(Dimension),
     XtOffset (XmToggleButtonGCacheObject, toggle_cache.spacing),
     XmRImmediate, (caddr_t) 4
   },

   {
     XmNselectPixmap, XmCSelectPixmap, XmRGadgetPixmap, sizeof(Pixmap),
     XtOffset (XmToggleButtonGCacheObject, toggle_cache.on_pixmap),
     XmRImmediate, (caddr_t) XmUNSPECIFIED_PIXMAP 
   },

   {
     XmNselectInsensitivePixmap, XmCSelectInsensitivePixmap, XmRGadgetPixmap, sizeof(Pixmap),
     XtOffset (XmToggleButtonGCacheObject, toggle_cache.insen_pixmap),
     XmRImmediate, (caddr_t) XmUNSPECIFIED_PIXMAP
   },

   {
      XmNindicatorOn, XmCIndicatorOn, XmRBoolean, sizeof (Boolean),
      XtOffset (XmToggleButtonGCacheObject, toggle_cache.ind_on),
      XmRImmediate, (caddr_t) True
   },

   {
      XmNfillOnSelect, XmCFillOnSelect, XmRBoolean, sizeof (Boolean),
      XtOffset (XmToggleButtonGCacheObject, toggle_cache.fill_on_select),
      XmRImmediate, (caddr_t) True

   },

   {
      XmNselectColor, XmCSelectColor, XmRPixel, sizeof (Pixel),
      XtOffset (XmToggleButtonGCacheObject, toggle_cache.select_color),
      XmRCallProc, (caddr_t) _XmSelectColorDefault

   },

};


/************************************************
The uncached resources for ToggleButton
************************************************/
 

static XtResource resources[] =
{
   {
      XmNset, XmCSet, XmRBoolean, sizeof(Boolean),
      XtOffset (XmToggleButtonGadget, toggle.set),
      XmRImmediate, (caddr_t) False
   },

   {
      XmNvalueChangedCallback, XmCValueChangedCallback, XmRCallback,
      sizeof (XtCallbackList),
      XtOffset (XmToggleButtonGadget, toggle.value_changed_CB),
      XmRPointer, (caddr_t)NULL 
   },

   {
      XmNarmCallback, XmCArmCallback, XmRCallback,
      sizeof (XtCallbackList),
      XtOffset (XmToggleButtonGadget, toggle.arm_CB),
      XmRPointer, (caddr_t)NULL 
   },

   {
      XmNdisarmCallback, XmCDisarmCallback, XmRCallback,
      sizeof (XtCallbackList),
      XtOffset (XmToggleButtonGadget, toggle.disarm_CB),
      XmRPointer, (caddr_t)NULL 
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
   }
};

/*  Definition for resources that need special processing in get values  */

static XmSyntheticResource cache_syn_resources[] =
{
   { XmNspacing,
     sizeof (Dimension),
     XtOffset (XmToggleButtonGCacheObject, toggle_cache.spacing),
     _XmFromHorizontalPixels,
     _XmToHorizontalPixels
   },

   { XmNindicatorSize,
     sizeof (Dimension),
     XtOffset (XmToggleButtonGCacheObject, toggle_cache.indicator_dim),
     _XmFromVerticalPixels,
     _XmToVerticalPixels
   },


};

/*****************************************************************
 * 
 *   Class Record definitions
 *
 ****************************************************************/

static XmCacheClassPart ToggleButtonClassCachePart = {
    {NULL, 0, 0},        /* head of class cache list */
    _XmCacheCopy,       /* Copy routine     */
    _XmCacheDelete,     /* Delete routine   */
    _XmToggleBCacheCompare,    /* Comparison routine   */
};

static XmBaseClassExtRec   ToggleBGClassExtensionRec = {
    NULL,    				/*   next_extension    */
    NULLQUARK, 				/* record_typ  */
    XmBaseClassExtVersion,		/*  version  */
    sizeof(XmBaseClassExtRec),		/* record_size  */
    XmInheritInitializePrehook, 	/*  initializePrehook  */
    SetValuesPrehook, 			/* setValuesPrehoo  */
    InitializePosthook, 		/* initializePosthook  */
    SetValuesPosthook, 			/* setValuesPosthook  */
    (WidgetClass)&xmToggleButtonGCacheObjClassRec,	/* secondaryObjectClass */
    (XtInitProc)SecondaryObjectCreate,  	/* secondaryObjectCreate */
    (XmGetSecResDataFunc) GetToggleBGClassSecResData,   /* getSecResData  */
    {NULL},           			/* Other Flags  */
    GetValuesPrehook, 			/* getValuesPrehoo  */
    GetValuesPosthook, 			/* getValuesPosthoo  */
};


/* ext rec static initialization */
XmToggleButtonGCacheObjClassRec xmToggleButtonGCacheObjClassRec =
{
  {
      /* superclass         */    (WidgetClass) &xmLabelGCacheObjClassRec,
      /* class_name         */    "XmToggleButtonGadget",
      /* widget_size        */    sizeof(XmToggleButtonGCacheObjRec),
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

/*************************************<->*************************************
 *
 *
 *   Description:  global class record for instances of class: ToggleButton
 *   -----------
 *
 *   Defines default field settings for this class record.
 *
 *************************************<->***********************************/

externaldef(xmtogglebuttongadgetclassrec)
	XmToggleButtonGadgetClassRec xmToggleButtonGadgetClassRec = {
   {
    /* superclass	  */	(WidgetClass) &xmLabelGadgetClassRec,
    /* class_name	  */	"XmToggleButtonGadget",
    /* widget_size	  */	sizeof(XmToggleButtonGadgetRec),
    /* class_initialize   */    ClassInitialize,
    /* class_part_init    */    ClassPartInitialize,				
    /* class_inited       */	FALSE,
    /* INITialize	  */	Initialize,
    /* initialize_hook    */    NULL,
    /* realize		  */	NULL,
    /* actions		  */	NULL,
    /* num_actions	  */	0,
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
    /* extension          */    (XtPointer)&ToggleBGClassExtensionRec,
   },

   {        /* gadget class record */
    /* border_highlight             */	BorderHighlight,
    /* border_unhighlight           */  BorderUnhighlight,
    /* arm_and_activate             */  ArmAndActivate,
    /* input_dispatch               */  InputDispatch,
    /* visual_change                */  VisualChange,
    /* syn resources                */  NULL,         
    /* num syn_resources            */  0,    
    /* class cache part   	    */	&ToggleButtonClassCachePart,
    /* extension                    */  NULL,
   },

   {        /* label class record */
    /* SetOverrideCallback     */    _XtInherit,
    /* menu proc entry 	       */    (XmMenuProc)_XtInherit,
    /* extension               */    NULL,
   },

   {	    /* toggle class record */
    /* extension               */    NULL,
   }
};

externaldef(xmtogglebuttongadgetclass) 
  WidgetClass xmToggleButtonGadgetClass = 
            (WidgetClass)&xmToggleButtonGadgetClassRec;

/***********************************************************
*
*  ClassInitialize
*
************************************************************/

static void ClassInitialize()
{
  ToggleBGClassExtensionRec.record_type = XmQmotif;
}

/************************************************************************
 * 
 * ClassPartInitialize
 *   Set up fast subclassing for the gadget.
 *
 ***********************************************************************/

static void ClassPartInitialize (wc)
WidgetClass wc;

{
  _XmFastSubclassInit (wc, XmTOGGLE_BUTTON_GADGET_BIT);
}

/*******************************************************************
 *
 *  _XmToggleBCacheCompare
 *
 *******************************************************************/
 int _XmToggleBCacheCompare (toggleB_inst, toggleB_cache_inst)
 XmToggleButtonGCacheObjPart *toggleB_inst, *toggleB_cache_inst;
 {
    if((toggleB_inst->ind_type == toggleB_cache_inst->ind_type) &&
       (toggleB_inst->visible == toggleB_cache_inst->visible) &&
       (toggleB_inst->spacing == toggleB_cache_inst->spacing) &&
       (toggleB_inst->indicator_dim == toggleB_cache_inst->indicator_dim) &&
       (toggleB_inst->on_pixmap == toggleB_cache_inst->on_pixmap) &&
       (toggleB_inst->insen_pixmap == toggleB_cache_inst->insen_pixmap) &&
       (toggleB_inst->ind_on == toggleB_cache_inst->ind_on) &&
       (toggleB_inst->fill_on_select == toggleB_cache_inst->fill_on_select) &&
       (toggleB_inst->select_color == toggleB_cache_inst->select_color) &&
       (toggleB_inst->select_GC == toggleB_cache_inst->select_GC) &&
       (toggleB_inst-> background_gc == toggleB_cache_inst->background_gc)) 
       return 1;
    else
       return 0;
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

  TBG_Cache(new) =
	&(((XmToggleButtonGCacheObject)extData->widget)->toggle_cache);
  TBG_Cache(req) =
        &(((XmToggleButtonGCacheObject)extData->reqWidget)->toggle_cache);

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
    XmToggleButtonGadget  tbw = (XmToggleButtonGadget)new;
    XmToggleButtonGadget  rw =  (XmToggleButtonGadget)req;

   /*
    * - register parts in cache.
    * - update cache pointers
    * - and free req
    */

    LabG_Cache(tbw) = (XmLabelGCacheObjPart *)
                      _XmCachePart( LabG_ClassCachePart(tbw),
		                    (caddr_t)LabG_Cache(tbw),
				    sizeof(XmLabelGCacheObjPart));

    TBG_Cache(tbw) = (XmToggleButtonGCacheObjPart *)
	     _XmCachePart( TBG_ClassCachePart(tbw),
                           (caddr_t)TBG_Cache(tbw),
		           sizeof(XmToggleButtonGCacheObjPart));

    /*
     * might want to break up into per-class work that gets explicitly
     * chained. For right now, each class has to replicate all
     * superclass logic in hook routine
     */

     /*
      * free the req subobject used for comparisons
      */
      ext = _XmGetWidgetExtData((Widget)tbw, XmCACHE_EXTENSION);
      XtFree(ext->reqWidget);
      XtDestroyWidget(ext->widget);
      /* extData gets freed at destroy */
}


/************************************************************************
 *
 *  VisualChange
 *      This function is called from XmManagerClass set values when
 *      the managers visuals have changed.  The gadget regenerates any
 *      GC based on the visual changes and returns True indicating a
 *      redraw is needed.  Otherwize, False is returned.
 *
 ************************************************************************/

static Boolean VisualChange (gw, curmw, newmw)
XmGadget gw;
XmManagerWidget curmw;
XmManagerWidget newmw;

{
   XmToggleButtonGadget abg = (XmToggleButtonGadget) gw;

   if (curmw->manager.foreground != newmw->manager.foreground ||
       curmw->core.background_pixel != newmw->core.background_pixel)
   {
      XtReleaseGC (XtParent (abg), TBG_SelectGC(abg));
      XtReleaseGC (XtParent (abg), TBG_BackgroundGC(abg));
      GetGC (abg);
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

static void InputDispatch (tb, event, event_mask)
XmToggleButtonGadget tb;
XEvent * event;
Mask event_mask;

{
   if (event_mask & XmARM_EVENT)
   {
     if (LabG_MenuType(tb) == XmMENU_PULLDOWN ||
	 LabG_MenuType(tb) == XmMENU_POPUP)
      BtnDown(tb, event);
     else
      Arm (tb, event);
   }
   else if (event_mask & XmMULTI_ARM_EVENT)
   {
     if (LabG_MenuType(tb) != XmMENU_PULLDOWN &&
         LabG_MenuType(tb) != XmMENU_POPUP)
     {
      Arm (tb, event);
     }
   }
   else if (event_mask & XmACTIVATE_EVENT)
   {
     if (LabG_MenuType(tb) == XmMENU_PULLDOWN ||
	 LabG_MenuType(tb) == XmMENU_POPUP)
     {
      if (event->type == ButtonRelease)
        BtnUp(tb, event);
      else /* Assume KeyPress or KeyRelease */
        KeySelect (tb, event);
     }
     else
     {
      Select(tb, event);
      Disarm (tb, event);
     }
   }
   else if (event_mask & XmMULTI_ACTIVATE_EVENT)
   {
     if (LabG_MenuType(tb) != XmMENU_PULLDOWN &&
         LabG_MenuType(tb) != XmMENU_POPUP)
     {
      Select(tb, event);
      Disarm (tb, event);
     }
   }
   else if (event_mask & XmHELP_EVENT) Help (tb, event);
   else if (event_mask & XmENTER_EVENT) Enter (tb, event);
   else if (event_mask & XmLEAVE_EVENT) Leave (tb, event);
   else if (event_mask & XmFOCUS_IN_EVENT) BorderHighlight (tb);
   else if (event_mask & XmFOCUS_OUT_EVENT) BorderUnhighlight (tb);
}



/***********************************************************************
 *
 * SetAndDisplayPixmap
 *    Sets the appropriate on, off pixmap in label's pixmap field and
 *    calls label's Redisplay routine.
 *
 ***********************************************************************/

static void SetAndDisplayPixmap(tb)
XmToggleButtonGadget tb;
{
 if (IsOn (tb))
 {
  if ((tb->rectangle.sensitive) && (tb->rectangle.ancestor_sensitive))
  {
   if ( ! IsNull (PixmapOn (tb)))
   {
     Pixmap tempPix;

     tempPix = LabG_Pixmap(tb);
     LabG_Pixmap(tb) = TBG_OnPixmap(tb);
     (* xmLabelGadgetClassRec.rect_class.expose) (tb, NULL, NULL);
     LabG_Pixmap(tb) = tempPix;
   }
   else
     (* xmLabelGadgetClassRec.rect_class.expose) (tb, NULL, NULL);
  }

  else
  {
   if ( ! IsNull (Pixmap_Insen_On (tb)))
   {
     Pixmap tempPix;

     tempPix = LabG_PixmapInsensitive(tb);
     LabG_PixmapInsensitive(tb) = TBG_InsenPixmap(tb);
     (* xmLabelGadgetClassRec.rect_class.expose) (tb, NULL, NULL);
     LabG_PixmapInsensitive(tb)  = tempPix;
   }
   else
     (* xmLabelGadgetClassRec.rect_class.expose) (tb, NULL, NULL);       
  }
 }

 else
     (* xmLabelGadgetClassRec.rect_class.expose) (tb, NULL, NULL);       
}



/*************************************************************************
 *
 *  Help
 *     This routine is called if the user has made a help selection
 *     on the gadget.
 *
 ************************************************************************/

static void Help (tb, event)
XmToggleButtonGadget tb;
XEvent * event;

{
   if (LabG_MenuType(tb) == XmMENU_PULLDOWN ||
       LabG_MenuType(tb) == XmMENU_POPUP)
   {
      (* xmLabelGadgetClassRec.label_class.menuProcs)
	  (XmMENU_POPDOWN, XtParent(tb), NULL, (caddr_t)event, NULL);
   }

   ToggleButtonCallback(tb, XmCR_HELP, TBG_Set(tb), event);
}


/*************************************************************************
 *
 * ToggleButtonCallback
 *    This is the widget's application callback routine
 *
 *************************************************************************/

 static int ToggleButtonCallback(data, reason, value, event)
    XmToggleButtonGadget  data;
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
            XtCallCallbackList ((Widget) data, TBG_ValueChangedCB(data), &temp);
            break;

        case XmCR_ARM          :
            XtCallCallbackList ((Widget) data, TBG_ArmCB(data), &temp);
            break;

        case XmCR_DISARM       :
            XtCallCallbackList ((Widget) data, TBG_DisarmCB(data), &temp);
            break;

        case XmCR_HELP         :
	        _XmSocorro((Widget)data, event);
            break;
       }

}


/**************************************************************************
 *
 *   Leave
 *     This procedure is called when  the mouse button is pressed and  the
 *     cursor moves out of the widget's window. This procedure is used
 *     to change the visuals.
 *
*************************************************************************/

static void Leave(w, event)
     XmToggleButtonGadget w;
     XEvent *event;

{
   int edge, x, y;
   Boolean fill;
   XmManagerWidget mw;

   if (LabG_MenuType(w) == XmMENU_PULLDOWN ||
       LabG_MenuType(w) == XmMENU_POPUP)
 {
    if (! (((XmManagerWidget) XtParent(w))->manager.traversal_on) &&
        (TBG_Armed(w)))
    {
       if (w->gadget.shadow_thickness > 0)
          _XmEraseShadow (XtDisplay (w), XtWindow (w),
                     w-> gadget.shadow_thickness,
                     w-> rectangle.x + w-> gadget.highlight_thickness,
                     w->rectangle .y + w-> gadget.highlight_thickness,
                     w-> rectangle.width - 2 * w->gadget.highlight_thickness,
                     w-> rectangle.height - 2 * w->gadget.highlight_thickness
);
       TBG_Armed(w) = FALSE;
       
       if (TBG_DisarmCB(w))
       {
         XFlush (XtDisplay (w));

         ToggleButtonCallback(w, XmCR_DISARM, TBG_Set(w), event);
       }

    }
 }

 else
 { _XmLeaveGadget ((XmGadget)w, event);

   mw = (XmManagerWidget) XtParent(w);
   if( TBG_IndicatorSet(w) || _XmStringEmpty(LabG__label(w)) ) {
     edge = TBG_IndicatorDim(w);
   } else {
     edge = Min((int)TBG_IndicatorDim(w), 
           Max(0, (int)w->rectangle.height - 2*(w->gadget.highlight_thickness +
                                           w->gadget.shadow_thickness +
                                          (int)LabG_MarginHeight(w)) +
                                           LabG_MarginTop(w) +
                                           LabG_MarginBottom(w)));
   }

   if ((mw->manager.top_shadow_color != TBG_SelectColor(w)) &&
       (mw->manager.bottom_shadow_color != TBG_SelectColor(w)))
         fill = TRUE;
   else
         fill = FALSE;

   
   x = w->rectangle.x +
       w->gadget.highlight_thickness + w->gadget.shadow_thickness +
       LabG_MarginHeight(w);
 
   if ( TBG_IndicatorSet(w) || _XmStringEmpty(LabG__label(w)) )
     y = w->rectangle.y + (w->rectangle.height - TBG_IndicatorDim(w))/2;
   else
     y = LabG_TextRect(w).y;
 

   if ((TBG_IndType(w)) == XmN_OF_MANY)
   {

    if (TBG_Armed(w) == TRUE)
    { 
      if (TBG_VisualSet(w) == TRUE)
         TBG_VisualSet(w) = FALSE;
      else
         TBG_VisualSet(w) = TRUE;
      if (TBG_IndOn(w))
      {
          /* if the toggle indicator is square shaped then adjust the
             indicator width and height, so that it looks proportional
             to a diamond shaped indicator of the same width and height */

          int new_edge;

          new_edge = edge - 3 - ((edge - 10)/10); /* Subtract 3 pixels + 1  */
                                              /* pixel for every 10 pixels, */
                                              /* from the width and height. */

         /* Adjust x,y so that the indicator is centered relative to the label*/
          y = y + ((edge - new_edge) / 2);
          x = x + ((edge - new_edge) / 2);
          edge = new_edge;


          if ((TBG_Visible(w)) ||
              ((!TBG_Visible(w)) && (TBG_VisualSet(w))))
          {
             _XmDrawShadow (XtDisplay (w), XtWindow (w), 
                           ((TBG_VisualSet(w)) ? 
                             XmParentBottomShadowGC(w) :
                             XmParentTopShadowGC(w)),
                           ((TBG_VisualSet(w)) ? 
                             XmParentTopShadowGC(w) :
                             XmParentBottomShadowGC(w)), 
                           2, x, y, edge, edge);

    
             if (TBG_FillOnSelect(w))
                if (edge > 6) XFillRectangle (XtDisplay ((Widget) w), 
                                              XtWindow ((Widget) w),
                                             ((TBG_VisualSet(w)) ?
                                              TBG_SelectGC(w) :
                                              TBG_BackgroundGC(w)),
                                             ((fill) ? x+2 : x+3),
                                             ((fill) ? y+2 : y+3),
                                             ((fill) ? edge-4 : edge-6),
                                             ((fill) ? edge-4 : edge-6));
          }


          if (!TBG_Visible(w))
          {
             if (!TBG_VisualSet(w))
                if (edge > 6) XFillRectangle( XtDisplay ((Widget) w),
                                              XtWindow ((Widget) w),
                                              TBG_BackgroundGC(w),
                                              x, y, edge, edge);
          }

     }     
     else
     {
       if (w->gadget.shadow_thickness> 0)
			  DrawToggleShadow (w );

     }
     if (LabG_IsPixmap(w))
     {
       SetAndDisplayPixmap(w);
     }
    }

   }
   else
   {
    if (TBG_Armed(w) == TRUE)
    { 
      if (TBG_VisualSet(w) == TRUE)
         TBG_VisualSet(w) = FALSE;
      else
         TBG_VisualSet(w) = TRUE;

      if (TBG_IndOn(w))
      {
         if ((TBG_Visible(w)) ||
            ((!TBG_Visible(w)) && (TBG_VisualSet(w))))
                            _XmDrawDiamondButton ((Widget)w, x, y, edge,
                                              ((TBG_VisualSet(w)) ?
                                                XmParentBottomShadowGC(w) :
                                                XmParentTopShadowGC(w)),
                                              ((TBG_VisualSet(w)) ?
                                                XmParentTopShadowGC(w) :
                                                XmParentBottomShadowGC(w)),
                                              (((TBG_VisualSet(w))  &&
                                               (TBG_FillOnSelect(w))) ?
                                                TBG_SelectGC(w) :
                                                TBG_BackgroundGC(w)),
                                              fill);
      
         if (!TBG_Visible(w))
         {
            if (!TBG_VisualSet(w))
               if (edge > 6) XFillRectangle( XtDisplay ((Widget) w),
                                             XtWindow ((Widget) w),
                                             TBG_BackgroundGC(w),
                                             x, y, edge, edge);
         }
      }
      else
      {
       if (w->gadget.shadow_thickness> 0) DrawToggleShadow (w);

      }
      if (LabG_IsPixmap(w))
      {
         SetAndDisplayPixmap(w);
      }
    } 
   }
 }
}


/**************************************************************************
 *
 *  Enter
 *    This procedure is called when the mouse button is pressed and the
 *    cursor reenters the widget's window. This procedure changes the visuals
 *    accordingly.
 *
 **************************************************************************/

static void Enter(w, event)
     XmToggleButtonGadget w;
     XEvent *event;

{
   int edge, x, y;
   Boolean  fill;
   XmManagerWidget mw;

   if (LabG_MenuType(w) == XmMENU_PULLDOWN ||
       LabG_MenuType(w) == XmMENU_POPUP)
   {
      if ((((ShellWidget) XtParent(XtParent(w)))->shell.popped_up) &&
	  (! ((XmManagerWidget) XtParent(w))->manager.traversal_on))
      {
	 if (w->gadget.shadow_thickness > 0)
	     _XmDrawShadow (XtDisplay (w), XtWindow (w),
                   XmParentTopShadowGC(w),
                   XmParentBottomShadowGC(w),
                   w -> gadget.shadow_thickness,
                   w->rectangle.x + w -> gadget.highlight_thickness,
                   w->rectangle.y + w -> gadget.highlight_thickness,
                   w -> rectangle.width - 2 * w->gadget.highlight_thickness,
                   w -> rectangle.height - 2 * w->gadget.highlight_thickness);

	 TBG_Armed(w) = TRUE;

	 if (TBG_ArmCB(w))
	 { 
	    XFlush (XtDisplay (w));
	    ToggleButtonCallback(w, XmCR_ARM, TBG_Set(w), event);
	 }
      }
   }

   else
   {  _XmEnterGadget ((XmGadget)w, event);  
      mw = (XmManagerWidget) XtParent(w);
      
      if( TBG_IndicatorSet(w) || _XmStringEmpty(LabG__label(w)) ) {
        edge = TBG_IndicatorDim(w);
      } else {
	edge = Min((int)TBG_IndicatorDim(w), 
		 Max(0, (int)w->rectangle.height -
		     2*(w->gadget.highlight_thickness +
			w->gadget.shadow_thickness +
			(int)LabG_MarginHeight(w)) +
		     LabG_MarginTop(w) +
		     LabG_MarginBottom(w)));
      }

      if ((mw->manager.top_shadow_color != TBG_SelectColor(w)) &&
	  (mw->manager.bottom_shadow_color != TBG_SelectColor(w)))
	  fill = TRUE;
      else
	  fill = FALSE;


      x = w->rectangle.x +
	  w->gadget.highlight_thickness + w->gadget.shadow_thickness +
	      LabG_MarginHeight(w);
 
      if( TBG_IndicatorSet(w) || _XmStringEmpty(LabG__label(w)) )
        y = w->rectangle.y + (w->rectangle.height - TBG_IndicatorDim(w))/2;
      else
	y = LabG_TextRect(w).y;

      if ((TBG_IndType(w)) == XmN_OF_MANY) 
      {
	 if (TBG_Armed(w) == TRUE)
	 { 
	    if (TBG_VisualSet(w) == TRUE)
		TBG_VisualSet(w) = FALSE;
	    else
		TBG_VisualSet(w) = TRUE;
	    if (TBG_IndOn(w))
	    {
	       /* if the toggle indicator is square shaped then adjust the
		  indicator width and height, so that it looks proportional
		  to a diamond shaped indicator of the same width and height */

	       int new_edge;
	       new_edge = edge - 3 - ((edge - 10)/10);
	                                      /* Subtract 3 pixels + 1  */
                                              /* pixel for every 10 pixels, */
                                              /* from the width and height. */

	       /* Adjust x,y so that the indicator is centered relative to the
		  label*/
	       y = y + ((edge - new_edge) / 2);
	       x = x + ((edge - new_edge) / 2);
	       edge = new_edge;

	       if ((TBG_Visible(w)) ||
		   ((!TBG_Visible(w)) && (TBG_VisualSet(w))))
	       {

		  _XmDrawShadow (XtDisplay (w), XtWindow (w), 
                            ((TBG_VisualSet(w)) ? 
                              XmParentBottomShadowGC(w) :
                              XmParentTopShadowGC(w)),
                            ((TBG_VisualSet(w)) ? 
                              XmParentTopShadowGC(w) :
                              XmParentBottomShadowGC(w)), 
                            2, x, y, edge, edge);

		  if (TBG_FillOnSelect(w))
		      if (edge > 6)
			  XFillRectangle(XtDisplay((Widget) w),
                                             XtWindow ((Widget) w),
                                             ((TBG_VisualSet(w)) ?
                                             TBG_SelectGC(w) :
                                             TBG_BackgroundGC(w)),
                                             ((fill) ? x+2 : x+3),
                                             ((fill) ? y+2 : y+3),
                                             ((fill) ? edge-4 : edge-6),
                                             ((fill) ? edge-4 : edge-6));
	       }
 
	       if (!TBG_Visible(w))
	       {
		  if (!TBG_VisualSet(w))
		      if (edge > 6)
			  XFillRectangle( XtDisplay ((Widget) w),
                                             XtWindow ((Widget) w),
                                             TBG_BackgroundGC(w),
                                             x, y, edge, edge);
	       }
	    }
	    else
	    {
	       if (w->gadget.shadow_thickness> 0)  DrawToggleShadow (w);
	    }
	    if (LabG_IsPixmap(w))
	    {
	       SetAndDisplayPixmap(w);
	    }
	 }
      }
      else 
      {
	 if (TBG_Armed(w) == TRUE) 
	 { 
	    if (TBG_VisualSet(w) == TRUE)
		TBG_VisualSet(w) = FALSE;
	    else
		TBG_VisualSet(w) = TRUE;
	    
	    if (TBG_IndOn(w))
	    {
	       if ((TBG_Visible(w)) ||
		   ((!TBG_Visible(w)) && (TBG_VisualSet(w))))
		   _XmDrawDiamondButton ((Widget)w, x, y, edge,
				      ((TBG_VisualSet(w)) ?
				       XmParentBottomShadowGC(w) :
				       XmParentTopShadowGC(w)),
				      ((TBG_VisualSet(w)) ?
				       XmParentTopShadowGC(w) :
				       XmParentBottomShadowGC(w)),
				      (((TBG_VisualSet(w)) &&
					(TBG_FillOnSelect(w))) ?
				       TBG_SelectGC(w) :
				       TBG_BackgroundGC(w)),
				      fill);

	       if (!TBG_Visible(w))
	       {
		  if (!TBG_VisualSet(w))
		      if (edge > 6)
			  XFillRectangle( XtDisplay ((Widget) w),
					 XtWindow ((Widget) w),
					 TBG_BackgroundGC(w),
					 x, y, edge, edge);
	       }
	    }
	    else
	    {
	       if (w->gadget.shadow_thickness> 0)  DrawToggleShadow (w);
	    }
	    if (LabG_IsPixmap(w))
	    {
	       SetAndDisplayPixmap(w);
	    }
	 }
      }
   }
}


/************************************************************************
 *
 *     Arm
 *        This function processes button 1 down occuring on the togglebutton.
 *        Mark the togglebutton as armed and display it armed.
 *        The callbacks for XmNarmCallback are called.
 *
 ************************************************************************/

static void Arm(w,event)
Widget w;
XEvent *event;
{
  XmToggleButtonGadget tb = (XmToggleButtonGadget)w;

  TBG_VisualSet(tb) = (TBG_Set(tb) == TRUE) ? FALSE : TRUE;
  TBG_Armed(tb) = TRUE;
  if (TBG_IndOn(tb))
  {
    DrawToggle(w);
  }
  else
  {
   if(tb->gadget.shadow_thickness> 0)  DrawToggleShadow (tb);
  }
  if (LabG_IsPixmap(tb))
  {
     SetAndDisplayPixmap(tb);
  }
    
  if (TBG_ArmCB(tb))
  {
     XFlush(XtDisplay(tb));
     ToggleButtonCallback(tb, XmCR_ARM, TBG_Set(tb), event);
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
XmToggleButtonGadget tb;
XEvent *event;

{
   XmToggleButtonCallbackStruct call_value;
   XButtonEvent *buttonEvent = (XButtonEvent *) event;

   TBG_Armed(tb) = FALSE;
   
   if ((buttonEvent->x <= tb->rectangle.x + tb->rectangle.width) &&
       (buttonEvent->y <= tb->rectangle.y + tb->rectangle.height) &&
       (buttonEvent->x >= tb->rectangle.x) &&
       (buttonEvent->y >= tb->rectangle.y))
   {
      TBG_Set(tb) = (TBG_Set(tb) == TRUE) ? FALSE : TRUE;

      /* if the parent is a RowColumn, notify it about the select */
      if (XmIsRowColumn(XtParent(tb)))
      {
	 call_value.reason = XmCR_VALUE_CHANGED;
	 call_value.event = event;
	 call_value.set = TBG_Set(tb);
	 (* xmLabelGadgetClassRec.label_class.menuProcs) (XmMENU_CALLBACK, 
							  XtParent(tb), FALSE,
							  (caddr_t)tb, (caddr_t)&call_value);
      }

      if ((! LabG_SkipCallback(tb)) &&
	  (TBG_ValueChangedCB(tb)))
      {
	 XFlush(XtDisplay(tb));
	 ToggleButtonCallback(tb, XmCR_VALUE_CHANGED, TBG_Set(tb), event);
      }
   }
}


/**********************************************************************
 *
 *    Disarm
 *     The callbacks for XmNdisarmCallback are called..
 *
 ************************************************************************/

static void Disarm(tb,event)
  XmToggleButtonGadget tb;
  XEvent *event;

 { 
   if (TBG_DisarmCB(tb))
     ToggleButtonCallback(tb, XmCR_DISARM, TBG_Set(tb), event);
 }


/************************************************************************
 *
 *     ArmAndActivate
 *       This routine arms and activates a ToggleButton. It is called on
 *       <Key> Return and a <Key> Space, as well as when a mnemonic or
 *       button accelerator has been activated.
 *    Modify: Current implementation does care to draw shadows if indicator
 *	     is set to false; This is being modified.
 ************************************************************************/

static void ArmAndActivate (tb, event)
XmToggleButtonGadget tb;
XEvent *event;

{
   XmToggleButtonCallbackStruct call_value;
   Boolean already_armed = TBG_Armed(tb);

   TBG_Armed(tb) = FALSE;    

   TBG_Set(tb) = (TBG_Set(tb) == TRUE) ? FALSE : TRUE;
   TBG_VisualSet(tb) = TBG_Set(tb);

   if (LabG_MenuType(tb) == XmMENU_PULLDOWN ||
       LabG_MenuType(tb) == XmMENU_POPUP)
       (* xmLabelGadgetClassRec.label_class.menuProcs)
	   (XmMENU_POPDOWN, XtParent(tb), NULL, (caddr_t)event, NULL);

   else
   { if(TBG_IndOn(tb)) DrawToggle(tb);
	  else
	   if(tb->gadget.shadow_thickness> 0)  DrawToggleShadow (tb);
 
      if (LabG_IsPixmap(tb))
	  SetAndDisplayPixmap(tb);
   }

   if (TBG_ArmCB(tb) && !already_armed)
       ToggleButtonCallback(tb, XmCR_ARM, TBG_Set(tb), event);

   /* if the parent is a RowColumn, notify it about the select */
   if (XmIsRowColumn(XtParent(tb)))
   {
      call_value.reason = XmCR_VALUE_CHANGED;
      call_value.event = event;
      call_value.set = TBG_Set(tb);
      (* xmLabelGadgetClassRec.label_class.menuProcs) (XmMENU_CALLBACK,
						       XtParent(tb),  FALSE,
						       (caddr_t)tb, (caddr_t)&call_value);
   }
   
   if ((! LabG_SkipCallback(tb)) &&
       (TBG_ValueChangedCB(tb)))
   {
      XFlush(XtDisplay(tb));
      ToggleButtonCallback(tb, XmCR_VALUE_CHANGED, TBG_Set(tb), event);
   }

   if (TBG_DisarmCB(tb))
   {
      XFlush(XtDisplay(tb));
      ToggleButtonCallback(tb, XmCR_DISARM, TBG_Set(tb), event);
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
XmToggleButtonGadget tb;
XEvent *event;

{
   Boolean already_armed;

   /* Popdown other popups that may be up */
   if (XtParent(XtParent(tb))->core.num_popups != 0)
   {
      ShellWidget popup = (ShellWidget)
	  XtParent(XtParent(tb))->core.popup_list[0];
      
      if (popup->shell.popped_up)
	  (* xmLabelGadgetClassRec.label_class.menuProcs)
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

   (* xmLabelGadgetClassRec.label_class.menuProcs) (XmMENU_TRAVERSAL,
					XtParent(tb), FALSE, NULL, NULL);

   if (tb->gadget.shadow_thickness > 0)
      _XmDrawShadow (XtDisplay (tb), XtWindow (tb),
                 XmParentTopShadowGC(tb),
                 XmParentBottomShadowGC(tb),
                 tb -> gadget.shadow_thickness,
                 tb->rectangle.x + tb -> gadget.highlight_thickness,
                 tb->rectangle.y + tb -> gadget.highlight_thickness,
                 tb -> rectangle.width - 2 * tb->gadget.highlight_thickness,
                 tb -> rectangle.height - 2 * tb->gadget.highlight_thickness);

   already_armed = TBG_Armed(tb);

   TBG_Armed(tb) = TRUE;

   if (TBG_ArmCB(tb) && !already_armed)
   {
     XFlush (XtDisplay (tb));

     ToggleButtonCallback(tb, XmCR_ARM, TBG_Set(tb), event);
   }

   _XmRecordEvent (event);
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
XmToggleButtonGadget tb;
XEvent *event;
{
   XButtonEvent *buttonEvent = (XButtonEvent *) event;
   XmToggleButtonCallbackStruct call_value;

   TBG_Armed(tb) = FALSE;
   
   (* xmLabelGadgetClassRec.label_class.menuProcs)
       (XmMENU_POPDOWN, XtParent(tb), NULL, (caddr_t)event, NULL);

   _XmRecordEvent(event);

   if ((buttonEvent->x <= tb->rectangle.x + tb->rectangle.width) &&
       (buttonEvent->y <= tb->rectangle.y + tb->rectangle.height) &&
       (buttonEvent->x >= tb->rectangle.x) &&
       (buttonEvent->y >= tb->rectangle.y))
   {
      TBG_Set(tb) = (TBG_Set(tb) == TRUE) ? FALSE : TRUE;
      TBG_VisualSet(tb) = TBG_Set(tb);

      /* if the parent is a RowColumn, notify it about the select */
      if (XmIsRowColumn(XtParent(tb)))
      {
	 call_value.reason = XmCR_VALUE_CHANGED;
	 call_value.event = event;
	 call_value.set = TBG_Set(tb);
	 (* xmLabelGadgetClassRec.label_class.menuProcs) (XmMENU_CALLBACK, 
							  XtParent(tb), FALSE,
							  (caddr_t)tb, (caddr_t)&call_value);
      }
      
      if ((! LabG_SkipCallback(tb)) &&
	  (TBG_ValueChangedCB(tb)))
      {
	 XFlush(XtDisplay(tb));
	 ToggleButtonCallback(tb, XmCR_VALUE_CHANGED, TBG_Set(tb), event);
      }
      
      if (TBG_DisarmCB(tb))
	  ToggleButtonCallback(tb, XmCR_DISARM, TBG_Set(tb), event);
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
XmToggleButtonGadget tw;

{
   XGCValues values;
   XtGCMask  valueMask;
   XmManagerWidget mw;

   mw = (XmManagerWidget) XtParent(tw);

   valueMask = GCForeground | GCBackground | GCFillStyle | GCGraphicsExposures;
   values.foreground = TBG_SelectColor(tw);
   values.background =  mw->core.background_pixel;
   values.fill_style = FillSolid;
   values.graphics_exposures = FALSE;

   TBG_SelectGC(tw) = XtGetGC (mw, valueMask, &values); 

   valueMask = GCForeground | GCBackground | GCFillStyle | GCGraphicsExposures;
   values.foreground = mw->core.background_pixel;
   values.background = mw->manager.foreground;
   values.fill_style = FillSolid;
   values.graphics_exposures = FALSE;

   TBG_BackgroundGC(tw) = XtGetGC(mw, valueMask, &values);

               
}


/*************************************<->*************************************
 *
 *  Initialize
 *    If the rectangle height and width fields are set to 0, treat that as a 
 *    flag, and compute the optimum size for this button.  Then using what ever
 *    the rectangle fields are set to, compute the text placement fields.
 *************************************<->***********************************/

static void Initialize (request, new, args, num_args )
 XmToggleButtonGadget request, new;
 ArgList args;
 Cardinal *num_args;

{   int maxIndicatorSize;   /* Max Indicator size permissible */
	int	delta;
	int	boxSize;

    TBG_Armed(new) = FALSE;

    /* if menuProcs is not set up yet, try again */
    if (xmLabelGadgetClassRec.label_class.menuProcs == NULL)
	xmLabelGadgetClassRec.label_class.menuProcs =
	    (XmMenuProc) _XmGetMenuProcContext();


    if (LabG_MenuType(new) == XmMENU_POPUP ||
	LabG_MenuType(new) == XmMENU_PULLDOWN)
    {
       if (new->gadget.shadow_thickness <= 0)
	   new->gadget.shadow_thickness = 2;

       if (TBG_Visible(new) == XmINVALID_BOOLEAN)
	   TBG_Visible(new) = FALSE;

       new->gadget.traversal_on = TRUE;
    }

    else
    {
       if (TBG_Visible(new) == XmINVALID_BOOLEAN)
	   TBG_Visible(new) = TRUE;
    }

    /*
     * if the indicatorType has not been set, then
     * find out if radio behavior is set for RowColumn parents and
     * then set indicatorType.  If radio behavior is true, default to
     * one of many, else default to n of many.
     */
    if ((TBG_IndType(new) != XmN_OF_MANY) &&
       (TBG_IndType(new) != XmONE_OF_MANY))
    {
       if (TBG_IndType(new) != XmINVALID_TYPE)
	   _XmWarning (new, MESSAGE1);

       if  (XmIsRowColumn(XtParent(new)))
       {
	  Arg arg[1];
	  Boolean radio;

	  XtSetArg (arg[0], XmNradioBehavior, &radio);
	  XtGetValues (XtParent(new), arg, 1);

	  if (radio)
	      TBG_IndType(new) = XmONE_OF_MANY;
	  else
	      TBG_IndType(new) = XmN_OF_MANY;
       }
       else
	   TBG_IndType(new) = XmN_OF_MANY;
    }

    if (IsNull (PixmapOff(new)) &&            /* no Off pixmap but do have */
        ! IsNull (PixmapOn(new)))           /* an On, so use that */
    {
       LabG_Pixmap(new) = TBG_OnPixmap(new);
       if (request->rectangle.width == 0)
         new->rectangle.width = 0;
       if (request->rectangle.height == 0)
         new->rectangle.height = 0;
       (* ((RectObjClass)
           ((RectObjClass) new->object.widget_class)->
             rect_class.superclass)->rect_class.resize) (new);
    }

    if (IsNull (Pixmap_Insen_Off(new)) &&        /* no Off pixmap but do have */
        ! IsNull (Pixmap_Insen_On(new)))           /* an On, so use that */
    {
       LabG_PixmapInsensitive(new) = TBG_InsenPixmap(new);
       if (request->rectangle.width == 0)
         new->rectangle.width = 0;
       if (request->rectangle.height == 0)
         new->rectangle.height = 0;
       (* ((RectObjClass)
           ((RectObjClass) new->object.widget_class)->
             rect_class.superclass)->rect_class.resize) (new);
    }

    if (TBG_IndicatorDim(new) == XmINVALID_DIMENSION)  {
      TBG_IndicatorSet(new) = FALSE;
      if (TBG_IndOn(new))
      {
	/* DETERMINE HOW HIGH THE TOGGLE INDICATOR SHOULD BE */

	if LabG_IsPixmap(new) 
	{
	  /*set indicatorSize proportional to size of pixmap*/
	  if (LabG_TextRect(new).height < 13)
	    TBG_IndicatorDim(new) = LabG_TextRect(new).height;
	  else 
	    TBG_IndicatorDim(new) = 13 + (LabG_TextRect(new).height/13);
	}
	else /*set indicatorSize proportional to size of font*/
	{
	  Dimension height;
	  int line_count;

	  height = _XmStringHeight (LabG_Font(new), LabG__label(new));
	  if( (line_count = _XmStringLineCount (LabG__label(new))) < 1)
	    line_count = 1;
	  /* Shiz recommends toggles in menus have smaller indicators 
	  if (LabG_MenuType(new) == XmMENU_POPUP ||          
	      LabG_MenuType(new) == XmMENU_PULLDOWN) {
             TBG_IndicatorDim(new) = Max(DEFAULT_INDICATOR_DIM,
               (height / ((Dimension)line_count))*2/3);
           } else*/
               TBG_IndicatorDim(new) = Max(DEFAULT_INDICATOR_DIM,
                 height / ((Dimension)line_count));

	}
      } else
        TBG_IndicatorDim(new) = 0;
    } else
      TBG_IndicatorSet(new) = TRUE;
 /*
  *
  *   Enlarge the text rectangle if needed to accomodate the size of
  *     indicator button. Adjust the dimenions of superclass Label-Gadget
  *	    so that the toggle-button may be accommodated in it.
  */

     maxIndicatorSize = TBG_IndicatorDim(new) +
                 2 * (new->gadget.shadow_thickness +
                     XM_3D_ENHANCE_PIXEL);
	 boxSize = (int) (LabG_TextRect(new).height) +
			   (int) LabG_MarginTop (new) +(int) LabG_MarginBottom (new);

     if (maxIndicatorSize > boxSize)
	  { delta = maxIndicatorSize - boxSize;
         LabG_MarginTop (new) += delta/2;
         LabG_MarginBottom (new) += delta /2;
	  } 

    /* Make room for toggle indicator and spacing */

    if (TBG_IndOn(new))
    {
        if ((LabG_MarginLeft(new)) < (TBG_IndicatorDim(new) +
                                       TBG_Spacing(new))) {
            LabG_MarginLeft(new) =  TBG_IndicatorDim(new) +
		                    TBG_Spacing(new);
	}
    }

    if (request->rectangle.width == 0)
    {

        new->rectangle.width = LabG_TextRect(new).width +
                          2 * LabG_MarginHeight(new) +   
                          LabG_MarginRight(new) +
                          LabG_MarginLeft(new) +
		          2 * (new->gadget.highlight_thickness +
                               new->gadget.shadow_thickness);

        if (new->rectangle.width == 0)
           new->rectangle.width = 1;
    
        if ((LabG__acceleratorText(new) != NULL) && (TBG_IndOn(new)))
             LabG_AccTextRect(new).x = new->rectangle.width -
                                         new->gadget.highlight_thickness -
                                         new->gadget.shadow_thickness -
                                         LabG_MarginHeight(new) -
                                         LabG_MarginRight(new) +
                                         LABELG_ACC_PAD;


    }

    if (request->rectangle.height == 0) 
        new->rectangle.height = Max(TBG_IndicatorDim(new),
           LabG_TextRect(new).height + 2 * LabG_MarginHeight(new) +
               LabG_MarginTop(new) + LabG_MarginBottom(new)) +
	       2 * (new->gadget.highlight_thickness +
                    new->gadget.shadow_thickness);  

    if (new->rectangle.height == 0)
       new->rectangle.height = 1;


    if (TBG_Set(new))
        TBG_VisualSet(new) = TRUE; /* When toggles first come up, if
                                           XmNset is TRUE, then they are
                                           displayed set */
    else
        TBG_VisualSet(new) = FALSE;

    new->gadget.event_mask = XmARM_EVENT | XmACTIVATE_EVENT | 
	XmMULTI_ARM_EVENT | XmMULTI_ACTIVATE_EVENT | XmHELP_EVENT |
        XmFOCUS_IN_EVENT | XmFOCUS_OUT_EVENT | XmENTER_EVENT | XmLEAVE_EVENT;

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
XmToggleButtonGadget tw;

{
   XtReleaseGC (XtParent(tw), TBG_SelectGC(tw));
   XtReleaseGC (XtParent(tw), TBG_BackgroundGC(tw));

   XtRemoveAllCallbacks (tw, XmNvalueChangedCallback);
   XtRemoveAllCallbacks (tw, XmNarmCallback);
   XtRemoveAllCallbacks (tw, XmNdisarmCallback);
}





/*************************************<->*************************************
 *
 *  DrawToggle
 *     Depending on the state of this widget, draw the ToggleButton.
 *
 *************************************<->***********************************/

static void DrawToggle(w)
   XmToggleButtonGadget w;
{
   int x, y, edge;
   Boolean fill;
   XmManagerWidget mw;

  mw = (XmManagerWidget) XtParent(w);

  if( TBG_IndicatorSet(w) || _XmStringEmpty(LabG__label(w)) ) {
    edge = TBG_IndicatorDim(w);
  } else {
  edge = Min((int)TBG_IndicatorDim(w), 
           Max(0, (int)w->rectangle.height - 2*(w->gadget.highlight_thickness +
                                           w->gadget.shadow_thickness +
                                          (int)LabG_MarginHeight(w)) +
                                           LabG_MarginTop(w) +
                                           LabG_MarginBottom(w)));
  }

  if (DefaultDepthOfScreen (XtScreen (w)) == 1) /* Monochrome Display */
     fill = TRUE;
  else
  {
    if ((mw->manager.top_shadow_color != TBG_SelectColor(w)) &&
        (mw->manager.bottom_shadow_color != TBG_SelectColor(w)))
          fill = TRUE;
    else
          fill = FALSE;
  }

  x = w->rectangle.x +
      w->gadget.highlight_thickness + w->gadget.shadow_thickness +
      LabG_MarginHeight(w);
 
  if( TBG_IndicatorSet(w) || _XmStringEmpty(LabG__label(w)) )
    y = w->rectangle.y + 
      (int)((w->rectangle.height - TBG_IndicatorDim(w)))/2;
  else
    y =  LabG_TextRect(w).y;

  if ((TBG_IndType(w)) == XmN_OF_MANY)
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

     if ((TBG_Visible(w)) ||
	 ((!TBG_Visible(w)) && (TBG_VisualSet(w))))
     {
	_XmDrawSquareButton ((Widget)w, x, y, edge, 
			      ((TBG_VisualSet(w)) ? 
			       XmParentBottomShadowGC(w) :
			       XmParentTopShadowGC(w)),
			      ((TBG_VisualSet(w)) ? 
			       XmParentTopShadowGC(w) :
			       XmParentBottomShadowGC(w)), 
			      (((TBG_VisualSet(w)) &&
				(TBG_FillOnSelect(w))) ?
			       TBG_SelectGC(w) :
			       TBG_BackgroundGC(w)),
			      fill);
     } 
  }

  else
  {
     if ((TBG_Visible(w)) ||
	 ((!TBG_Visible(w)) && (TBG_VisualSet(w))))
	 _XmDrawDiamondButton ((Widget)w, x, y, edge,
			       ((TBG_VisualSet(w)) ? 
				XmParentBottomShadowGC(w) :
				XmParentTopShadowGC(w)),
			       ((TBG_VisualSet(w)) ? 
				XmParentTopShadowGC(w) :
				XmParentBottomShadowGC(w)), 
			       (((TBG_VisualSet(w)) &&
				 (TBG_FillOnSelect(w))) ?
				TBG_SelectGC(w) :
				TBG_BackgroundGC(w)),
			       fill);

  } 

   if ((!TBG_Visible(w)) && (!TBG_VisualSet(w)))
   {
      if (edge > 6)
	  XFillRectangle( XtDisplay ((Widget) w),
			 XtWindow ((Widget) w),
			 TBG_BackgroundGC(w),
			 x, y, edge, edge);
   }
}

/*************************************<->*************************************
 *
 *  BorderHighlight
 *
 *************************************<->***********************************/

static void BorderHighlight (tb)
XmToggleButtonGadget tb;
{
   XEvent * event = NULL;

   if (LabG_MenuType(tb) == XmMENU_PULLDOWN ||
       LabG_MenuType(tb) == XmMENU_POPUP)
   {
      if (tb->gadget.shadow_thickness > 0)
	  _XmDrawShadow (XtDisplay (tb), XtWindow (tb),
                       XmParentTopShadowGC(tb),
                       XmParentBottomShadowGC(tb),
                       tb -> gadget.shadow_thickness,
                       tb->rectangle.x + tb -> gadget.highlight_thickness,
                       tb->rectangle.y + tb -> gadget.highlight_thickness,
                       tb -> rectangle.width -
			 2 * tb->gadget.highlight_thickness,
                       tb -> rectangle.height -
			 2 * tb->gadget.highlight_thickness);
      
      TBG_Armed(tb) = TRUE;
      
      if (TBG_ArmCB(tb))
      {
	 XFlush (XtDisplay (tb));
	 ToggleButtonCallback(tb, XmCR_ARM, TBG_Set(tb), event);
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
XmToggleButtonGadget tb;
{
   XEvent * event = NULL;

   if (LabG_MenuType(tb) == XmMENU_PULLDOWN ||
       LabG_MenuType(tb) == XmMENU_POPUP)
   {
      if (!TBG_Armed(tb))
	  return;
      if (tb->gadget.shadow_thickness > 0)
	  _XmEraseShadow (XtDisplay (tb), XtWindow (tb),
                       tb -> gadget.shadow_thickness,
                       tb->rectangle.x + tb -> gadget.highlight_thickness,
                       tb->rectangle.y + tb -> gadget.highlight_thickness,
		       tb -> rectangle.width -
			  2 * tb->gadget.highlight_thickness,
                       tb -> rectangle.height -
			  2 * tb->gadget.highlight_thickness);
      
      TBG_Armed(tb) = FALSE;

      if (TBG_DisarmCB(tb))
      {
	 XFlush (XtDisplay (tb));
	 ToggleButtonCallback(tb, XmCR_DISARM, TBG_Set(tb), event);
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

static void KeySelect (tb, event)
XmToggleButtonGadget tb;
XEvent *event;
{
   XmToggleButtonCallbackStruct call_value;

   if (!_XmIsEventUnique(event))
      return;

   if (((XmManagerWidget) XtParent(tb)) -> manager.traversal_on)
   {

      TBG_Armed(tb) = FALSE;
      TBG_Set(tb) = (TBG_Set(tb) == TRUE) ? FALSE : TRUE;

      (* xmLabelGadgetClassRec.label_class.menuProcs)
	  (XmMENU_POPDOWN,  XtParent(tb), NULL, (caddr_t)event, NULL);

      _XmRecordEvent(event);

      /* if the parent is a RowColumn, notify it about the select */
      if (XmIsRowColumn(XtParent(tb)))
      {
	 call_value.reason = XmCR_VALUE_CHANGED;
	 call_value.event = event;
	 call_value.set = TBG_Set(tb);
	 (* xmLabelGadgetClassRec.label_class.menuProcs) (XmMENU_CALLBACK, 
							  XtParent(tb), FALSE,
							  (caddr_t)tb, (caddr_t)&call_value);
      }
      
      if ((! LabG_SkipCallback(tb)) &&
	  (TBG_ValueChangedCB(tb)))
      {
	 XFlush(XtDisplay(tb));
	 ToggleButtonCallback(tb, XmCR_VALUE_CHANGED, TBG_Set(tb), event);
      }
   }
}

/************************************************************************
 *
 * Compute Space
 *
 ***********************************************************************/

static void ComputeSpace(tb)
     XmToggleButtonGadget tb;
{

   int needed_width;
   int needed_height;

  /* COMPUTE SPACE FOR DRAWING TOGGLE */

   needed_width = LabG_TextRect(tb).width +
                  LabG_MarginLeft(tb) + LabG_MarginRight(tb) +
                  (2 * (tb->gadget.shadow_thickness +
                        tb->gadget.highlight_thickness +
                        LabG_MarginHeight(tb)));

   needed_height = LabG_TextRect(tb).height +
                   LabG_MarginTop(tb) + LabG_MarginBottom(tb) +
                   (2 * (tb->gadget.shadow_thickness +
                         tb->gadget.highlight_thickness +
                         LabG_MarginHeight(tb)));

   if (needed_height > tb->rectangle.height)
      if (TBG_IndOn(tb))
          LabG_TextRect(tb).y = tb->rectangle.y +
                        tb->gadget.shadow_thickness +
                        tb->gadget.highlight_thickness +
                        LabG_MarginHeight(tb) +
                        LabG_MarginTop(tb) +
                        ((tb->rectangle.height - LabG_MarginTop(tb)
                        - LabG_MarginBottom(tb)
                        - (2 * (LabG_MarginHeight(tb)
                        + tb->gadget.highlight_thickness
                        + tb->gadget.shadow_thickness))
                        - LabG_TextRect(tb).height) / 2);

   if ((needed_width > tb->rectangle.width) ||
     ((LabG_Alignment(tb) == XmALIGNMENT_BEGINNING)
       && (needed_width < tb->rectangle.width)) ||
     ((LabG_Alignment(tb) == XmALIGNMENT_CENTER)
       && (needed_width < tb->rectangle.width)
       && (tb->rectangle.width - needed_width < LabG_MarginLeft(tb))) ||
     (needed_width == tb->rectangle.width))
   {

       if (TBG_IndOn(tb))
          LabG_TextRect(tb).x = tb->rectangle.x +
                                 tb->gadget.shadow_thickness +
                                 tb->gadget.highlight_thickness +
                                 LabG_MarginHeight(tb) +
                                 LabG_MarginLeft(tb);
   } 

} /* ComputeSpace */

/*************************************<->*************************************
 *
 *  Redisplay(w, event) 
 *     Cause the widget, identified by w, to be redisplayed.
 *
 *************************************<->***********************************/

static void Redisplay(w, event)
    Widget w;
    XEvent *event;
{
   register XmToggleButtonGadget tb = (XmToggleButtonGadget) w;

   if (LabG_MenuType(tb) == XmMENU_PULLDOWN ||
       LabG_MenuType(tb) == XmMENU_POPUP) 
   {
      ShellWidget mshell = (ShellWidget) XtParent(XtParent(tb));
      if (! mshell->shell.popped_up)
	  return;
   }
   
   ComputeSpace(tb);

   if (LabG_IsPixmap (tb))
       SetAndDisplayPixmap(tb);
   else
       (* xmLabelGadgetClassRec.rect_class.expose) (tb, NULL, NULL);

   if (TBG_IndOn(tb))
   {
     TBG_VisualSet(tb) = TBG_Set(tb);
     DrawToggle(tb);
   }

   if (LabG_MenuType(tb) == XmMENU_PULLDOWN ||
       LabG_MenuType(tb) == XmMENU_POPUP) 
   {
      if ((TBG_Armed(tb)) && 
	  (tb->gadget.shadow_thickness > 0))
	    _XmDrawShadow (XtDisplay (tb), XtWindow (tb),
                           XmParentTopShadowGC(tb),
                           XmParentBottomShadowGC(tb),
                           tb -> gadget.shadow_thickness,
                           tb->rectangle.x +
                           tb -> gadget.highlight_thickness,
                           tb->rectangle.y +
                           tb -> gadget.highlight_thickness,
                   (int)tb->rectangle.width-2*tb->gadget.highlight_thickness,
                   (int)tb->rectangle.height-2*tb->gadget.highlight_thickness);
   }

   else
   {
      if ((tb->gadget.shadow_thickness) > 0)
			DrawToggleShadow (tb);


      if (tb->gadget.highlighted)
	  _XmHighlightBorder((Widget)tb);
      else if (_XmDifferentBackground ((Widget)tb, XtParent (tb)))
	  _XmUnhighlightBorder((Widget)tb);
   }
}


/**************************************************************************
 * Resize(w, event)
 **************************************************************************/

static void Resize(w)
    Widget w;
{
  register XmToggleButtonGadget tb = (XmToggleButtonGadget) w;

  
  (* ((RectObjClass)
       ((RectObjClass)tb->object.widget_class)->
         rect_class.superclass)->rect_class.resize) (tb);

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
{
    XmWidgetExtData             extData;
    XmBaseClassExt              *cePtr;
    WidgetClass                 ec;
    XmToggleButtonGCacheObject    new;

    cePtr = _XmGetBaseClassExtPtr(XtClass(newParent), XmQmotif);
    ec = (*cePtr)->secondaryObjectClass;

    /* allocate copies and fill from cache */
    new = (XmToggleButtonGCacheObject) XtMalloc(ec->core_class.widget_size);
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

    bcopy((char *)TBG_Cache(newParent),
          (char *)&(new->toggle_cache),
          sizeof(XmToggleButtonGCacheObjPart));

    extData = (XmWidgetExtData) XtCalloc(sizeof(XmWidgetExtDataRec), 1);
    _XmPushWidgetExtData(newParent, extData, XmCACHE_EXTENSION);

    XtSetValues(new, args, *num_args);

    LabG_Cache(newParent) = &(((XmLabelGCacheObject)new)->label_cache);
    LabG_Cache(refParent) = &(((XmLabelGCacheObject)extData->reqWidget)->label_cache);

    /*TBG_Cache(oldParent) = TBG_Cache(newParent);*/
    TBG_Cache(newParent) =
        &(((XmToggleButtonGCacheObject)new)->toggle_cache);
    TBG_Cache(refParent) =
        &(((XmToggleButtonGCacheObject)extData->reqWidget)->toggle_cache);

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
    XmToggleButtonGCacheObject    new;

    cePtr = _XmGetBaseClassExtPtr(XtClass(newParent), XmQmotif);
    ec = (*cePtr)->secondaryObjectClass;

    /* allocate copies and fill from cache */
    new = (XmToggleButtonGCacheObject) XtMalloc(ec->core_class.widget_size);
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

    bcopy((char *)TBG_Cache(newParent),
          (char *)&(new->toggle_cache),
          sizeof(XmToggleButtonGCacheObjPart));

    extData = (XmWidgetExtData) XtCalloc(sizeof(XmWidgetExtDataRec), 1);
    _XmPushWidgetExtData(newParent, extData, XmCACHE_EXTENSION);

    XtGetValues(new, args, *num_args);

}

/************************************************************************
 *
 *  GetValuesPosthook
 *
************************************************************************/
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
static Boolean SetValuesPosthook(current, req, new, args, num_args)
    Widget      current, req, new;
    ArgList     args;
    Cardinal    *num_args;
{
    XmWidgetExtData             ext;
    XmToggleButtonGCacheObjPart        *oldCachePtr, *newCachePtr;

    /*
     * - register parts in cache.
     * - update cache pointers
     * - and free req
     */


  /* assign if changed! */
     if (!_XmLabelCacheCompare(LabG_Cache(new),
                         LabG_Cache(current)))
     {
	_XmCacheDelete((caddr_t)LabG_Cache(current));  /* delete the old one */
	 LabG_Cache(new) = (XmLabelGCacheObjPart *)
	    _XmCachePart(LabG_ClassCachePart(new),
	                 (caddr_t)LabG_Cache(new),
	                 sizeof(XmLabelGCacheObjPart));
     } else
            LabG_Cache(new) = LabG_Cache(current);


    /* assign if changed! */
     if (!_XmToggleBCacheCompare(TBG_Cache(new),
                         TBG_Cache(current)))
     {
       _XmCacheDelete((caddr_t)TBG_Cache(current));  /* delete the old one */
       TBG_Cache(new) = (XmToggleButtonGCacheObjPart *)
	 _XmCachePart(TBG_ClassCachePart(new),
	              (caddr_t)TBG_Cache(new),
		      sizeof(XmToggleButtonGCacheObjPart));
     } else
        TBG_Cache(new) = TBG_Cache(current);


      _XmPopWidgetExtData(new, &ext, XmCACHE_EXTENSION);

      XtFree(ext->widget);
      XtFree(ext->reqWidget);
      XtFree(ext->oldWidget);
      XtFree(ext);
 
      return FALSE;
}




/***************************************************************************
 *
 *
 *  SetValues(current, request, new)
 *     This is the set values procedure for the ToggleButton class.  It is
 *     called last (the set values rtnes for its superclasses are called
 *     first).
 *
 *
 *************************************<->***********************************/


static Boolean SetValues(current, request, new)
    Widget current, request, new;
{
    XmToggleButtonGadget curcbox = (XmToggleButtonGadget) current;
    XmToggleButtonGadget newcbox = (XmToggleButtonGadget) new;
    XmToggleButtonGadget reqcbox = (XmToggleButtonGadget) request;
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


   if (IsNull (PixmapOff(newcbox)) &&     /* no Off pixmap but do have */
       ! IsNull (PixmapOn(newcbox)))     /* an On, so use that */
   {
         LabG_Pixmap(newcbox) = TBG_OnPixmap(newcbox);
         if ((LabG_RecomputeSize(newcbox)) &&
            (request->core.width == current->core.width))
              new->core.width = 0;
         if ((LabG_RecomputeSize(newcbox)) &&
            (request->core.height == current->core.height))
              new->core.height = 0;
         (* ((RectObjClass)
           ((RectObjClass) newcbox->object.widget_class)->
             rect_class.superclass)->rect_class.resize) (newcbox);
   }

   if (IsNull (Pixmap_Insen_Off(newcbox)) &&  /* no Off pixmap but do have */
       ! IsNull (Pixmap_Insen_On(newcbox)))   /* an On, so use that */
   {
         LabG_PixmapInsensitive(newcbox) = TBG_InsenPixmap(newcbox);
         if ((LabG_RecomputeSize(newcbox)) &&
            (request->core.width == current->core.width))
              new->core.width = 0;
         if ((LabG_RecomputeSize(newcbox)) &&
            (request->core.height == current->core.height))
              new->core.height = 0;
         (* ((RectObjClass)
           ((RectObjClass) newcbox->object.widget_class)->
             rect_class.superclass)->rect_class.resize) (newcbox);
   }

   if ((LabG_RecomputeSize(newcbox)) ||
      (LabG__label(newcbox) != LabG__label(curcbox)) ||
      (LabG_Pixmap(newcbox) != LabG_Pixmap(curcbox)) ||
      (LabG_Font(newcbox) != LabG_Font(curcbox)) ||
      (TBG_Spacing(newcbox) != TBG_Spacing(curcbox)) ||
      (TBG_OnPixmap(newcbox) != TBG_OnPixmap(curcbox)) ||
      (TBG_InsenPixmap(newcbox) != TBG_InsenPixmap(curcbox)) ||
      (TBG_IndOn(newcbox) != TBG_IndOn(curcbox)))
   {
       if (LabG_RecomputeSize(newcbox))
       {
         if (reqcbox->rectangle.width == curcbox->rectangle.width)
            newcbox->rectangle.width = 0;
         if (reqcbox->rectangle.height == curcbox->rectangle.height)
            newcbox->rectangle.height = 0;
       }

       if ((TBG_OnPixmap(newcbox) != TBG_OnPixmap(curcbox)) ||
           (TBG_InsenPixmap(newcbox) != TBG_InsenPixmap(curcbox)))
          (* ((RectObjClass)
           ((RectObjClass) newcbox->object.widget_class)->
             rect_class.superclass)->rect_class.resize) (newcbox);


       if ( TBG_IndicatorDim(newcbox) == XmINVALID_DIMENSION )
                 TBG_IndicatorSet(newcbox) = FALSE;

       if (!TBG_IndicatorSet(newcbox))
       {
	 if ((LabG__label(newcbox) != LabG__label(curcbox)) ||
	     (LabG_Pixmap(newcbox) != LabG_Pixmap(curcbox)) ||
	     (LabG_Font(newcbox) != LabG_Font(curcbox)) ||
	     (TBG_IndOn(newcbox) != TBG_IndOn(curcbox)))
	 {
	   if LabG_IsPixmap(new)
	   {
	      if (LabG_TextRect(newcbox).height < 13)
		 TBG_IndicatorDim(newcbox) = LabG_TextRect(newcbox).height;
	      else
		 TBG_IndicatorDim(newcbox) = 13 + (LabG_TextRect(newcbox).height/13);
	   }
	   else
	   {
	      Dimension height;
	      int line_count;

	      height = _XmStringHeight (LabG_Font(newcbox), LabG__label(newcbox));
	      line_count = _XmStringLineCount (LabG__label(newcbox));
	      if (line_count <= 1)
		TBG_IndicatorDim(newcbox) = height;
	      else
		TBG_IndicatorDim(newcbox) = height /((Dimension)line_count);
	   }

	 } 
       } 

       if (LabG_RecomputeSize(newcbox))
       {
         if (reqcbox->rectangle.width == curcbox->rectangle.width)
            newcbox->rectangle.width = 0;
         if (reqcbox->rectangle.height == curcbox->rectangle.height)
            newcbox->rectangle.height = 0;
       }

       if (TBG_IndOn(newcbox))
         {
          if ((LabG_MarginLeft(newcbox)) < (TBG_IndicatorDim(newcbox) +
                                              TBG_Spacing(newcbox)))
              LabG_MarginLeft(newcbox) = TBG_IndicatorDim(newcbox) +
                                            TBG_Spacing(newcbox);
         }

       if (newcbox->rectangle.width == 0)
       {
         newcbox->rectangle.width =
                   LabG_TextRect(newcbox).width +
                   LabG_MarginLeft(newcbox) + LabG_MarginRight(newcbox) +
                   2 * (newcbox->gadget.highlight_thickness +
                        newcbox->gadget.shadow_thickness +
                        LabG_MarginHeight(newcbox));

         if (newcbox->rectangle.width == 0)
            newcbox->rectangle.width = 1;

         flag = TRUE;
       }

       if (newcbox->rectangle.height == 0)
       {
         newcbox->rectangle.height = Max(TBG_IndicatorDim(newcbox),
            LabG_TextRect(newcbox).height + 2*LabG_MarginHeight(newcbox) +
                     LabG_MarginTop(newcbox) + LabG_MarginBottom(newcbox)) +
                    2 * (newcbox->gadget.highlight_thickness +
                         newcbox->gadget.shadow_thickness);

         if (newcbox->rectangle.height == 0)
            newcbox->rectangle.height = 1;

         flag = TRUE;
       }


   }



    if ((TBG_IndType(curcbox) != TBG_IndType(newcbox)) ||
       (TBG_Visible(curcbox) != TBG_Visible(newcbox))) 
    {
      if ((TBG_IndType(newcbox) != XmN_OF_MANY) &&
       (TBG_IndType(newcbox) != XmONE_OF_MANY))
      {
         TBG_IndType(newcbox) = TBG_IndType(curcbox);
         _XmWarning (newcbox, MESSAGE1);
      }
       flag = True;
    }
 /*  Recompute the Top and bottom margins and the height of the text
  *	 rectangle to  accommodate the size of toggle indicator.
  *  if (we are given a new toggleIndicator size)
  *	   { if (user has given new top or bottom margin)
  *			  { compute to accomodate new toggle button size;
  *	          }
  *	      else (user has set new top/bottom margin)
  *	          { Recompute margin to accommodate new toogleButtonIndicatorSize;
  *		      }
  *	   }
  */

   if (TBG_IndicatorDim(newcbox) != TBG_IndicatorDim(curcbox))
   { maxIndicatorSize = (int) (TBG_IndicatorDim(newcbox)) +
                             2 * (newcbox->gadget.shadow_thickness +
                                         XM_3D_ENHANCE_PIXEL);
	 boxSize = (int) (LabG_TextRect(newcbox).height) +
			    (int) (LabG_MarginTop(newcbox)) +
			     (int)(LabG_MarginBottom(newcbox));
	if (maxIndicatorSize != boxSize)
	  { delta = maxIndicatorSize - boxSize;
	    if ( LabG_MarginTop(newcbox) == LabG_MarginTop(curcbox))
		   /* User has not specified new top margin */
		  { LabG_MarginTop(newcbox) = Max ( XM_DEFAULT_TOP_MARGIN,
						(int) LabG_MarginTop(newcbox) + delta/2);
		  }
	    else 
	      /* User has sepcified a top margin  and
			Margin must not be less than user specified amount */
		 { LabG_MarginTop(newcbox) = Max( LabG_MarginTop(newcbox),
							(LabG_MarginTop(newcbox) + delta/2)); 
		 }

        if ( LabG_MarginBottom(newcbox) == LabG_MarginBottom(curcbox))
           /* User has not specified new bottom margin */
          { LabG_MarginBottom(newcbox) = Max ( XM_DEFAULT_BOTTOM_MARGIN,
                        (int) LabG_MarginBottom(newcbox) + delta/2);
          }
        else
          /* User has sepcified a bottom margin  and
            Margin must not be less than user specified amount */
         { LabG_MarginBottom(newcbox) = Max( LabG_MarginBottom(newcbox),
                            (LabG_MarginBottom(newcbox) + delta/2));
         }
       }
    }

    if (TBG_SelectColor(curcbox) != TBG_SelectColor(newcbox))
    {
      XtReleaseGC(XtParent(curcbox), TBG_SelectGC(curcbox));
      XtReleaseGC(XtParent(curcbox), TBG_BackgroundGC(curcbox));
      GetGC(newcbox);
      flag = TRUE;
    }


    if (TBG_Set(curcbox) != TBG_Set(newcbox))
    {
       TBG_VisualSet(newcbox) = TBG_Set(newcbox);
          if (flag == False && TBG_IndOn (newcbox) && XtIsRealized(newcbox))
						DrawToggle (newcbox);
          /**    flag = True;           **/
    }


    newcbox->gadget.event_mask = XmARM_EVENT | XmACTIVATE_EVENT |
	XmMULTI_ARM_EVENT | XmMULTI_ACTIVATE_EVENT | XmHELP_EVENT |
        XmFOCUS_IN_EVENT | XmFOCUS_OUT_EVENT | XmENTER_EVENT | XmLEAVE_EVENT;

    return(flag);
}

/***************************************************************
 *
 * XmToggleButtonGadgetGetState  
 *    This function gets the state of the toggle gadget.
 *
 ***************************************************************/

Boolean XmToggleButtonGadgetGetState(w)
 Widget w;
{
     XmToggleButtonGadget tg = (XmToggleButtonGadget) w;

     return (TBG_Set(tg));
}

/****************************************************************
 *
 * XmToggleButtonGadgetSetState
 *    This function sets the state of the toggle gadget.
 *
 ****************************************************************/

#ifdef _NO_PROTO
void XmToggleButtonGadgetSetState(w, newstate, notify)
Widget w;
Boolean newstate;
Boolean notify;
#else /* _NO_PROTO */
void XmToggleButtonGadgetSetState (Widget w,    
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

    XmToggleButtonGadget tg = (XmToggleButtonGadget) w;

    if (TBG_Set(tg) != newstate)
    {
      TBG_Set(tg) = newstate;
      TBG_VisualSet(tg) = newstate;
      if (XtIsRealized (tg))
      {
        if (TBG_IndOn(tg))
          DrawToggle(tg);
        else
          if (tg->gadget.shadow_thickness> 0) 
					DrawToggleShadow (tg);
        if (LabG_IsPixmap(tg))
          SetAndDisplayPixmap(tg);
      }
      if (notify)
      {
	 /* if the parent is a RowColumn, notify it about the select */
	 if (XmIsRowColumn(XtParent(tg)))
	 {
	    XmToggleButtonCallbackStruct call_value;
	    call_value.reason = XmCR_VALUE_CHANGED;
	    call_value.event = NULL;
	    call_value.set = TBG_Set(tg);
	    (* xmLabelGadgetClassRec.label_class.menuProcs) (XmMENU_CALLBACK,
               XtParent(tg), FALSE, (caddr_t)tg, (caddr_t)&call_value);
	 }

	 if ((! LabG_SkipCallback(tg)) &&
	     (TBG_ValueChangedCB(tg)))
	 {
	    if (XtIsRealized (tg))
	       XFlush (XtDisplay (tg));
	    ToggleButtonCallback(tg, XmCR_VALUE_CHANGED, TBG_Set(tg), NULL);
	 }
      }
    }
} 

/***********************************************************************
 *
 * XmCreateToggleButtonGadget
 *   Creates an instance of a togglebutton and returns the widget id.
 *
 ***********************************************************************/
  
#ifdef _NO_PROTO
Widget XmCreateToggleButtonGadget(parent,name,arglist,argCount)
       Widget parent;
       char   *name;
       Arg    *arglist;
       Cardinal    argCount;
#else /* _NO_PROTO */
Widget XmCreateToggleButtonGadget (Widget parent, char *name, Arg *arglist, Cardinal argCount)
#endif /* _NO_PROTO */
{
    return (XtCreateWidget(name,xmToggleButtonGadgetClass,parent,arglist,argCount));
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
static Cardinal
GetToggleBGClassSecResData  ( class, data_rtn)
WidgetClass class;
XmSecondaryResourceData  **data_rtn;
{   int arrayCount = 0;
    int resNum;
    XmSecondaryResourceData secData, *sd;

    XmBaseClassExt  bcePtr;
    String  resource_class, resource_name;
    XtPointer  client_data;

    bcePtr = &(  ToggleBGClassExtensionRec );
    client_data = NULL;
    resource_class = NULL;
    resource_name = NULL;
    arrayCount =
      _XmSecondaryResourceData ( bcePtr, data_rtn, client_data,
                resource_name, resource_class,
                (XmResourceBaseProc) (GetToggleBGClassSecResBase  ));
    return (arrayCount);

}


/*
 * GetToggleBGClassResBase ()
 *   return the address of the base of resources.
 */
static XtPointer
GetToggleBGClassSecResBase ( widget, client_data)
Widget  widget;
XtPointer client_data;
{   XtPointer  widgetSecdataPtr;
    int  labg_cache_size = sizeof (XmLabelGCacheObjPart);
    int  togglebg_cache_size = sizeof (XmToggleButtonGCacheObjPart);
    char *cp;

    widgetSecdataPtr = (XtPointer)
            (XtMalloc ( labg_cache_size + togglebg_cache_size + 1));

    if (widgetSecdataPtr)
      { cp = (char *) widgetSecdataPtr;
        bcopy ( (char *) ( LabG_Cache(widget)), (char *) cp, labg_cache_size);
        cp += labg_cache_size;
        bcopy ((char *) TBG_Cache(widget), (char *) cp, togglebg_cache_size);
      }
/* else Warning: error cannot allocate Memory */


	return ( widgetSecdataPtr);
}
/*
 * DrawToggleShadow (tb)
 *   - Should be called only if ToggleShadow are to be drawn ;
 *  if the IndicatorOn resource is set to false top and bottom shadows
 *  will be switched depending on whether the Toggle is selected or
 *  unselected.
 */

static void DrawToggleShadow (tb)
XmToggleButtonGadget tb;
{   GC topgc, bottomgc;
	int dx, dy, width, height;
    int hilite_thickness, shadow_thickness;

    if (!(TBG_IndOn(tb)))
	{ if (TBG_VisualSet(tb)) 
		{ topgc = XmParentBottomShadowGC(tb);
		  bottomgc = XmParentTopShadowGC(tb);
		}
	   else
	    { topgc = XmParentTopShadowGC(tb);
		  bottomgc = XmParentBottomShadowGC(tb);
		}
	 }
	 else
        { topgc = XmParentTopShadowGC(tb);
          bottomgc = XmParentBottomShadowGC(tb);
        }

    hilite_thickness = tb->gadget.highlight_thickness;
    shadow_thickness = tb-> gadget.shadow_thickness;
	dx = (int)(tb->rectangle.x + hilite_thickness);
    dy = (int)(tb->rectangle.y + hilite_thickness);
	width = (int) ( tb->rectangle.width - (hilite_thickness << 1));
	height = (int) ( tb->rectangle.height - (hilite_thickness << 1));

    _XmDrawShadow (XtDisplay (tb), XtWindow (tb),
					topgc, bottomgc, shadow_thickness,
					dx, dy, width, height);

}
