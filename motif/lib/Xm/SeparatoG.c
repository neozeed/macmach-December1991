#ifdef REV_INFO
#ifndef lint
static char SCCSID[] = "OSF/Motif: @(#)SeparatoG.c	3.19 91/01/10";
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
#include <Xm/XmP.h>
#include <X11/ShellP.h>
#include <Xm/RowColumn.h>
#include <Xm/RowColumnP.h>
#include <Xm/SeparatoGP.h>
#include <Xm/ExtObjectP.h>
#include <Xm/LabelGP.h>
#include <Xm/CacheP.h>
#include <Xm/BaseClassI.h>

#define MESSAGE1 "Invalid separator type."
#define MESSAGE2 "Invalid orientation."

/*  Static routine definitions  */

static void    ClassInitialize();
static void    ClassPartInitialize();
static void    Initialize();
static void    Redisplay();
static void    Destroy();
static void    Help();
static Boolean SetValues();

/* Initialize and SetValues Pre and Post hooks */

static void InitializePosthook();
static Boolean SetValuesPrehook();
static Boolean SetValuesPosthook();
static void GetValuesPrehook();
static void GetValuesPosthook();

int _XmSeparatorCacheCompare();

/* Gadget Class functions */

static void     InputDispatch();
static Boolean VisualChange();

static void GetSeparatorGC();

/*
 * Procedures for manipulating cached_resources as secondary data.
 */
static Cardinal GetSeparatorGClassSecResData();
static XtPointer GetSeparatorGClassSecResBase ( );
extern Cardinal _XmSecondaryResourceData ();

/*  Resource list for Separator  */

static XtResource resources[] =
{
   {
     XmNtraversalOn, XmCTraversalOn, XmRBoolean, sizeof (Boolean),
     XtOffset (XmGadget, gadget.traversal_on),
     XmRImmediate, (caddr_t) False
   },
   {
     XmNhighlightThickness, XmCHighlightThickness, XmRHorizontalDimension,
     sizeof (Dimension), XtOffset (XmGadget, gadget.highlight_thickness),
     XmRImmediate, (caddr_t) 0
   },
};


static XtResource cache_resources[] = 
{
   {
      XmNseparatorType,
      XmCSeparatorType,
      XmRSeparatorType,
      sizeof (unsigned char),
      XtOffset (XmSeparatorGCacheObject, separator_cache.separator_type),
      XmRImmediate, (caddr_t) XmSHADOW_ETCHED_IN
   },

   {
      XmNmargin, 
      XmCMargin, 
      XmRHorizontalDimension, 
      sizeof (Dimension),
      XtOffset (XmSeparatorGCacheObject, separator_cache.margin),
      XmRImmediate, (caddr_t)  0
   },

   {
      XmNorientation,
      XmCOrientation,
      XmROrientation,
      sizeof (unsigned char),
      XtOffset (XmSeparatorGCacheObject, separator_cache.orientation),
      XmRImmediate, (caddr_t) XmHORIZONTAL
   },
};

static XmSyntheticResource cache_syn_resources[] = 
{
   {
      XmNmargin, 
      sizeof (Dimension),
      XtOffset (XmSeparatorGCacheObject, separator_cache.margin),
      _XmFromHorizontalPixels,
      _XmToHorizontalPixels,
   },
};

/* ext rec static initialization */
XmSeparatorGCacheObjClassRec xmSeparatorGCacheObjClassRec =
{
  {
    /* superclass         */    (WidgetClass) &xmExtClassRec,
    /* class_name         */    "XmSeparatorGadget",
    /* widget_size        */    sizeof(XmSeparatorGCacheObjRec),
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

static void SecondaryObjectCreate();

static XmBaseClassExtRec   separatorBaseClassExtRec = {
    NULL,    					/* next_extension        */
    NULLQUARK,					/* record_typ            */
    XmBaseClassExtVersion,			/* version               */
    sizeof(XmBaseClassExtRec),			/* record_size           */
    XmInheritInitializePrehook,			/* initializePrehook     */
    SetValuesPrehook,				/* setValuesPrehook      */
    InitializePosthook,				/* initializePosthook    */
    SetValuesPosthook,				/* setValuesPosthook     */
    (WidgetClass)&xmSeparatorGCacheObjClassRec,	/* secondaryObjectClass  */
    (XtInitProc)SecondaryObjectCreate,		/* secondaryObjectCreate */
    (XmGetSecResDataFunc) GetSeparatorGClassSecResData,	/* getSecResData */
    {NULL},			                /* Other Flags           */
    GetValuesPrehook,				/* getValuesPrehook      */
    GetValuesPosthook,				/* getValuesPosthook     */
};

static XmCacheClassPart SeparatorClassCachePart = {
    {NULL, 0, 0},        /* head of class cache list */
    _XmCacheCopy,       /* Copy routine     */
    _XmCacheDelete,     /* Delete routine   */
    _XmSeparatorCacheCompare,    /* Comparison routine   */
};


/*  The Separator class record definition  */

externaldef(xmseparatorgadgetclassrec) XmSeparatorGadgetClassRec xmSeparatorGadgetClassRec =

{
   {
      (WidgetClass) &xmGadgetClassRec,  /* superclass            */
      "XmSeparatorGadget",              /* class_name	         */
      sizeof(XmSeparatorGadgetRec),     /* widget_size	         */
      ClassInitialize,         		/* class_initialize      */
      ClassPartInitialize,              /* class_part_initialize */
      FALSE,                            /* class_inited          */
      (XtInitProc) Initialize,          /* initialize	         */
      NULL,                             /* initialize_hook       */
      NULL,	                        /* realize	         */
      NULL,                             /* actions               */
      0,			        /* num_actions    	 */
      resources,                        /* resources	         */
      XtNumber(resources),		/* num_resources         */
      NULLQUARK,                        /* xrm_class	         */
      TRUE,                             /* compress_motion       */
      TRUE,                             /* compress_exposure     */
      TRUE,                             /* compress_enterleave   */
      FALSE,                            /* visible_interest      */	
      (XtWidgetProc) Destroy,           /* destroy               */	
      (XtWidgetProc) NULL,              /* resize                */
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
      (XtPointer)&separatorBaseClassExtRec, /* extension         */
   },

   {
      NULL, 			/* border highlight   */
      NULL,      		/* border_unhighlight */
      NULL,			/* arm_and_activate   */
      InputDispatch,		/* input dispatch     */
      VisualChange,		/* visual_change      */
      NULL,			/* syn_resources      */
      0,  			/* num_syn_resources  */
      &SeparatorClassCachePart, /* class cache part   */
      NULL,         		/* extension          */
   },

   {
      NULL,         		/* extension */
   }
};

externaldef(xmseparatorgadgetclass) WidgetClass xmSeparatorGadgetClass = 
   (WidgetClass) &xmSeparatorGadgetClassRec;



/************************************************************************
 *
 *  InputDispatch
 *     This function catches input sent by a manager and dispatches it
 *     to the individual routines.
 *
 ************************************************************************/

static void InputDispatch (sg, event, event_mask)
XmSeparatorGadget sg;
XEvent * event;
Mask event_mask;

{
   if (event_mask & XmHELP_EVENT) Help (sg, event);
}

/*******************************************************************
 *
 *  _XmSeparatorCacheCompare
 *
 *******************************************************************/
 int _XmSeparatorCacheCompare (separator_inst, separator_cache_inst)
 XmSeparatorGCacheObjPart *separator_inst, *separator_cache_inst;
 {
    if((separator_inst->margin == separator_cache_inst->margin) &&
       (separator_inst->orientation == separator_cache_inst->orientation) &&
       (separator_inst->separator_type == separator_cache_inst->separator_type) &&
       (separator_inst-> separator_GC == separator_cache_inst->separator_GC))
       return 1;
    else
       return 0;
 }

/***********************************************************
*
*  ClassInitialize
*
************************************************************/

static void ClassInitialize()
{
  separatorBaseClassExtRec.record_type = XmQmotif;
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
   _XmFastSubclassInit (wc, XmSEPARATOR_GADGET_BIT);
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
  SEPG_Cache(new) =
              &(((XmSeparatorGCacheObject)extData->widget)->separator_cache);
  SEPG_Cache(req) =
              &(((XmSeparatorGCacheObject)extData->reqWidget)->separator_cache);

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
    XmSeparatorGadget lw = (XmSeparatorGadget)new;
    XmSeparatorGadget rw = (XmSeparatorGadget)req;

    /*
     * - register parts in cache.
     * - update cache pointers
     * - and free req
     */

    SEPG_Cache(lw) = (XmSeparatorGCacheObjPart *)
      _XmCachePart( SEPG_ClassCachePart(lw),
                    (caddr_t)SEPG_Cache(lw),
                    sizeof(XmSeparatorGCacheObjPart));

    /*
     * might want to break up into per-class work that gets explicitly
     * chained. For right now, each class has to replicate all
     * superclass logic in hook routine
     */

    /*
     * free the req subobject used for comparisons
     */
    ext = _XmGetWidgetExtData((Widget)lw, XmCACHE_EXTENSION);
    XtFree(ext->reqWidget);
    XtDestroyWidget(ext->widget);
    /* extData gets freed at destroy */
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
    XmSeparatorGCacheObject         new;

    cePtr = _XmGetBaseClassExtPtr(XtClass(newParent), XmQmotif);
    ec = (*cePtr)->secondaryObjectClass;

    /* allocate copies and fill from cache */
    new = (XmSeparatorGCacheObject) XtMalloc(ec->core_class.widget_size);

    new->object.self = (Widget)new;
    new->object.widget_class = ec;
    new->object.parent = XtParent(newParent);
    new->object.xrm_name = newParent->core.xrm_name;
    new->object.being_destroyed = False;
    new->object.destroy_callbacks = NULL;
    new->object.constraints = NULL;

    new->ext.logicalParent = newParent;
    new->ext.extensionType = XmCACHE_EXTENSION;

    bcopy((char *)SEPG_Cache(newParent),
          (char *)&(new->separator_cache),
          sizeof(XmSeparatorGCacheObjPart));

    extData = (XmWidgetExtData) XtCalloc(sizeof(XmWidgetExtDataRec), 1);
    _XmPushWidgetExtData(newParent, extData, XmCACHE_EXTENSION);


    XtSetValues(new, args, *num_args);

    /*SEPG_Cache(oldParent) = SEPG_Cache(newParent);*/
    SEPG_Cache(newParent) = &(((XmSeparatorGCacheObject)new)->separator_cache);
    SEPG_Cache(refParent) =
	      &(((XmSeparatorGCacheObject)extData->reqWidget)->separator_cache);

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
    XmSeparatorGCacheObject     new;

    cePtr = _XmGetBaseClassExtPtr(XtClass(newParent), XmQmotif);
    ec = (*cePtr)->secondaryObjectClass;

    /* allocate copies and fill from cache */
    new = (XmSeparatorGCacheObject) XtMalloc(ec->core_class.widget_size);
    new->object.self = (Widget)new;
    new->object.widget_class = ec;
    new->object.parent = XtParent(newParent);
    new->object.xrm_name = newParent->core.xrm_name;
    new->object.being_destroyed = False;
    new->object.destroy_callbacks = NULL;
    new->object.constraints = NULL;

    new->ext.logicalParent = newParent;
    new->ext.extensionType = XmCACHE_EXTENSION;

    bcopy((char *)SEPG_Cache(newParent),
          (char *)&(new->separator_cache),
          sizeof(XmSeparatorGCacheObjPart));

    extData = (XmWidgetExtData) XtCalloc(sizeof(XmWidgetExtDataRec), 1);
    _XmPushWidgetExtData(newParent, extData, XmCACHE_EXTENSION);

    XtGetValues(new, args, *num_args);

}

/************************************************************************
 *
 *  GetValuesPosthook
 *
 ************************************************************************/
/* ARGSUSED */
static void GetValuesPosthook(new, args, num_args)
    Widget      new;
    ArgList     args;
    Cardinal    *num_args;
{
    XmWidgetExtData ext;

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
{
    XmWidgetExtData             ext;
    XmSeparatorGCacheObjPart        *oldCachePtr, *newCachePtr;

    /*
     * - register parts in cache.
     * - update cache pointers
     * - and free req
     */

    /* assign if changed! */
    if (!_XmSeparatorCacheCompare(SEPG_Cache(new),
                        SEPG_Cache(current)))
    {
          _XmCacheDelete((caddr_t)SEPG_Cache(current));  /* delete the old one */
          SEPG_Cache(new) = (XmSeparatorGCacheObjPart *)
            _XmCachePart(SEPG_ClassCachePart(new),
                         (caddr_t)SEPG_Cache(new),
                         sizeof(XmSeparatorGCacheObjPart));
     } else
           SEPG_Cache(new) = SEPG_Cache(current);

    _XmPopWidgetExtData(new, &ext, XmCACHE_EXTENSION);

    XtFree(ext->widget);
    XtFree(ext->reqWidget);
    XtFree(ext->oldWidget);
    XtFree(ext);

    return FALSE;
}


      
/************************************************************************
 *
 *  Initialize
 *     The main widget instance initialization routine.
 *
 ************************************************************************/
/*ARGSUSED*/
static void Initialize (request, new, args, num_args)
XmSeparatorGadget request, new;
ArgList args;
Cardinal *num_args;

{
   new -> gadget.traversal_on = FALSE;

   /* Force highlightThickness to zero if in a menu. */
   if (XmIsRowColumn(XtParent(new)) && 
       ((RC_Type(XtParent(new)) == XmMENU_PULLDOWN) ||
	(RC_Type(XtParent(new)) == XmMENU_POPUP)))
     new->gadget.highlight_thickness = 0;

   if (SEPG_SeparatorType(new) != XmNO_LINE            &&
       SEPG_SeparatorType(new) != XmSINGLE_LINE        &&
       SEPG_SeparatorType(new) != XmDOUBLE_LINE        &&
       SEPG_SeparatorType(new) != XmSINGLE_DASHED_LINE &&
       SEPG_SeparatorType(new) != XmDOUBLE_DASHED_LINE &&
       SEPG_SeparatorType(new) != XmSHADOW_ETCHED_IN   &&
       SEPG_SeparatorType(new) != XmSHADOW_ETCHED_OUT)
   {
      _XmWarning (new, MESSAGE1);
      SEPG_SeparatorType(new) = XmSHADOW_ETCHED_IN;
   }

   if (SEPG_Orientation(new) != XmHORIZONTAL &&
       SEPG_Orientation(new) != XmVERTICAL)
   {
      _XmWarning (new, MESSAGE2);
      SEPG_Orientation(new) = XmHORIZONTAL;
   }

   if (SEPG_Orientation(new) == XmHORIZONTAL)
   {
      if (request -> rectangle.width == 0)
	 new -> rectangle.width = 2 * new -> gadget.highlight_thickness +2;

      if (request -> rectangle.height == 0)
      {
	 new -> rectangle.height = 2 * new -> gadget.highlight_thickness;

	 if (SEPG_SeparatorType(new) == XmSINGLE_LINE ||
	     SEPG_SeparatorType(new) == XmSINGLE_DASHED_LINE)
	    new -> rectangle.height += 3;
	 else if (SEPG_SeparatorType(new) == XmSHADOW_ETCHED_IN ||
		  SEPG_SeparatorType(new) == XmSHADOW_ETCHED_OUT)
	    new -> rectangle.height += new -> gadget.shadow_thickness;
	 else if (SEPG_SeparatorType(new) == XmDOUBLE_LINE ||
		  SEPG_SeparatorType(new) == XmDOUBLE_DASHED_LINE)
	    new -> rectangle.height += 5;
	 else
	    if (new -> rectangle.height == 0)
	       new -> rectangle.height = 1;
      }
   }

   if (SEPG_Orientation(new) == XmVERTICAL)
   {
      if (request -> rectangle.height == 0)
	 new -> rectangle.height = 2 * new -> gadget.highlight_thickness +2;

      if (request -> rectangle.width == 0)
      {
	 new -> rectangle.width = 2 * new -> gadget.highlight_thickness;

	 if (SEPG_SeparatorType(new) == XmSINGLE_LINE ||
	     SEPG_SeparatorType(new) == XmSINGLE_DASHED_LINE)
	    new -> rectangle.width += 3;
	 else if (SEPG_SeparatorType(new) == XmSHADOW_ETCHED_IN ||
		  SEPG_SeparatorType(new) == XmSHADOW_ETCHED_OUT)
	    new -> rectangle.width += new -> gadget.shadow_thickness;
	 else if (SEPG_SeparatorType(new) == XmDOUBLE_LINE ||
		  SEPG_SeparatorType(new) == XmDOUBLE_DASHED_LINE)
	    new -> rectangle.width += 5;
	 else
	    if (new -> rectangle.width == 0)
	       new -> rectangle.width = 1;
      }
   }



   /*  Get the drawing graphics contexts.  */

   GetSeparatorGC (new);
  
   /* only want help input events */

   new->gadget.event_mask = XmHELP_EVENT;

}




/************************************************************************
 *
 *  GetSeparatorGC
 *     Get the graphics context used for drawing the separator.
 *
 ************************************************************************/

static void GetSeparatorGC (sg)
XmSeparatorGadget sg;

{
   XGCValues values;
   XtGCMask  valueMask;
   XmManagerWidget mw;
   
   mw = (XmManagerWidget) XtParent(sg);

   valueMask = GCForeground | GCBackground;

   values.foreground = mw -> manager.foreground;
   values.background = mw -> core.background_pixel;

   if (SEPG_SeparatorType(sg) == XmSINGLE_DASHED_LINE ||
       SEPG_SeparatorType(sg) == XmDOUBLE_DASHED_LINE)
   {
      valueMask = valueMask | GCLineStyle;
      values.line_style = LineDoubleDash;
   }

   SEPG_SeparatorGC(sg) = XtGetGC ((Widget) mw, valueMask, &values);
}




/************************************************************************
 *
 *  Redisplay
 *     Invoke the application exposure callbacks.
 *
 ************************************************************************/
/*ARGSUSED*/
static void Redisplay (mw, event, region)
XmSeparatorGadget mw;
XEvent * event;
Region region;

{
   int lineCount = 2;
   int center;
   int start;
   int top, bottom;
   int i;
   static XSegment *segs = NULL;
   static int seg_count = 0;

   if (XmIsRowColumn(XtParent(mw)))
   {
      Widget rowcol = XtParent(mw);

      if ((RC_Type(rowcol) == XmMENU_PULLDOWN ||
	   RC_Type(rowcol) == XmMENU_POPUP)    &&
	  (! ((ShellWidget)XtParent(rowcol))->shell.popped_up))
      {
	 /* in a menu system that is not yet popped up, ignore */
	 return;
      }
   }
       
   if (seg_count == 0)
   {
      segs = (XSegment *) XtMalloc (sizeof (XSegment) * lineCount);
      seg_count = lineCount;
   }

   if (SEPG_Orientation(mw) == XmHORIZONTAL)
   {
      center = mw -> rectangle.y + mw -> rectangle.height / 2;
      top = mw -> rectangle.y + mw -> gadget.highlight_thickness;
      bottom = mw -> rectangle.y + mw -> rectangle.height - 
               mw -> gadget.highlight_thickness;
   }
   else
   {
      center = mw -> rectangle.x + mw -> rectangle.width / 2;
      top = mw -> rectangle.x + mw -> gadget.highlight_thickness;
      bottom = mw -> rectangle.x + mw -> rectangle.width - 
               mw -> gadget.highlight_thickness;
   }

   switch (SEPG_SeparatorType(mw))
   {

      case XmNO_LINE:
         lineCount = 0;
         break;

      case XmSINGLE_LINE:
      case XmSINGLE_DASHED_LINE:
         lineCount = 0;
         if (SEPG_Orientation(mw) == XmHORIZONTAL)
         {
            if (center >= top && center < bottom)
            {
	       segs[0].y1 = segs[0].y2 = center;
	       segs[0].x1 = mw -> rectangle.x + 
			   mw -> gadget.highlight_thickness +
			   SEPG_Margin(mw);
	       segs[0].x2 = mw -> rectangle.x + 
			   mw -> rectangle.width - 
			   mw -> gadget.highlight_thickness -
			   SEPG_Margin(mw) - 1;
               lineCount ++;
            }
         }
         else
         {
            if (center >= top && center < bottom)
            {
	       segs[0].x1 = segs[0].x2 = center;
	       segs[0].y1 = mw -> rectangle.y + 
			   mw -> gadget.highlight_thickness +
			   SEPG_Margin(mw);
	       segs[0].y2 = mw -> rectangle.y + 
			   mw -> rectangle.height - 
			   mw -> gadget.highlight_thickness -
			   SEPG_Margin(mw) - 1;
               lineCount ++;
	    }
         }
      break;

      case XmDOUBLE_LINE:
      case XmDOUBLE_DASHED_LINE:
         lineCount = 0;
         if (SEPG_Orientation(mw) == XmHORIZONTAL)
         {
            if (center - 1 >= top && center - 1 < bottom)
            {
               segs[0].y1 = segs[0].y2 = center - 1;
               segs[0].x1 = mw -> rectangle.x + 
	                    mw -> gadget.highlight_thickness +
			    SEPG_Margin(mw);
               segs[0].x2 = mw -> rectangle.x + 
	                    mw -> rectangle.width -
                            mw -> gadget.highlight_thickness -
			    SEPG_Margin(mw) - 1;
               lineCount ++;	      
	    }

            if (center + 1 >= top && center + 1 < bottom)
            {
               segs[1].y1 = segs[1].y2 = center +1;
               segs[1].x1 = mw -> rectangle.x + 
	                    mw -> gadget.highlight_thickness +
	         	    SEPG_Margin(mw);
               segs[1].x2 = mw -> rectangle.x + 
	                    mw -> rectangle.width -
                            mw -> gadget.highlight_thickness -
			    SEPG_Margin(mw) - 1;
	       lineCount++;
	    }
	 }
         else     /* orientation = XmVERTICAL */
         {

            if (center - 1 > top && center - 1 < bottom)
            {
               segs[0].x1 = segs[0].x2 = center - 1;
               segs[0].y1 = mw -> rectangle.y + 
	                    mw -> gadget.highlight_thickness +
			    SEPG_Margin(mw);
               segs[0].y2 = mw -> rectangle.y + 
	                    mw -> rectangle.height -
                            mw -> gadget.highlight_thickness -
			    SEPG_Margin(mw) - 1;
               lineCount ++;
	    }

            if (center + 1 > top && center + 1 < bottom)
            {
               segs[1].x1 = segs[1].x2 = center + 1;
               segs[1].y1 = mw -> rectangle.y + 
	                    mw -> gadget.highlight_thickness +
	        	    SEPG_Margin(mw);
               segs[1].y2 = mw -> rectangle.y + 
	                    mw -> rectangle.height -
                            mw -> gadget.highlight_thickness -
			    SEPG_Margin(mw) - 1;
               lineCount ++;
	    }
	 }
      break;

      case XmSHADOW_ETCHED_IN:
      case XmSHADOW_ETCHED_OUT:

	 if (SEPG_Orientation(mw) == XmHORIZONTAL)
	    if (mw -> gadget.shadow_thickness > mw -> rectangle.height -
	                 2 * mw -> gadget.highlight_thickness) 
	       lineCount = (mw -> rectangle.height - 
	                    2 * mw -> gadget.highlight_thickness)/2;
            else 
               lineCount = mw -> gadget.shadow_thickness/2;

	 if (SEPG_Orientation(mw) == XmVERTICAL)
	    if (mw -> gadget.shadow_thickness > mw -> rectangle.width -
	                 2 * mw -> gadget.highlight_thickness) 
	       lineCount = (mw -> rectangle.width -
	                    2 * mw -> gadget.highlight_thickness)/2;
            else
               lineCount = mw -> gadget.shadow_thickness/2;

         start = center - lineCount;

	 if (seg_count < mw -> gadget.shadow_thickness)
	 {
	    segs = (XSegment *) XtRealloc (segs, sizeof (XSegment) * mw->gadget.shadow_thickness);
	    seg_count = mw -> gadget.shadow_thickness;
	 }

         if (SEPG_Orientation(mw) == XmHORIZONTAL)
         {
            for (i=0; i < (lineCount*2); i++, start++)
	    {
	       segs[i].y1 = segs[i].y2 = start;
	       segs[i].x1 = mw -> rectangle.x + 
			    mw -> gadget.highlight_thickness +
			    SEPG_Margin(mw);
	       segs[i].x2 = mw -> rectangle.x + 
			    mw -> rectangle.width -
			    mw -> gadget.highlight_thickness -
			    SEPG_Margin(mw) - 1;
	    }
	 }
         else
         {
            for (i=0; i < (lineCount*2); i++, start++)
	    {
	       segs[i].x1 = segs[i].x2 = start;
	       segs[i].y1 = mw -> rectangle.y + 
			    mw -> gadget.highlight_thickness +
			    SEPG_Margin(mw);
	       segs[i].y2 = mw -> rectangle.y + 
			    mw -> rectangle.height -
			    mw -> gadget.highlight_thickness -
			    SEPG_Margin(mw) - 1;
            }
	 }
      break;
   }

   if  (SEPG_SeparatorType(mw) != XmNO_LINE && lineCount)
   {
      if (SEPG_SeparatorType(mw) == XmSHADOW_ETCHED_IN)
      {
         XDrawSegments (XtDisplay ((Widget) mw), XtWindow ((Widget) mw),
                     XmParentBottomShadowGC (mw), segs, lineCount);
         XDrawSegments (XtDisplay ((Widget) mw), XtWindow ((Widget) mw),
                     XmParentTopShadowGC (mw), &segs[lineCount], lineCount);
      }
      else if (SEPG_SeparatorType(mw) == XmSHADOW_ETCHED_OUT)
      {
         XDrawSegments (XtDisplay ((Widget) mw), XtWindow ((Widget) mw),
                     XmParentTopShadowGC (mw), segs, lineCount);
         XDrawSegments (XtDisplay ((Widget) mw), XtWindow ((Widget) mw),
                     XmParentBottomShadowGC (mw), &segs[lineCount], lineCount);
      }

      else 
      {
         XDrawSegments (XtDisplay ((Widget) mw), XtWindow ((Widget) mw),
                     SEPG_SeparatorGC(mw), segs, lineCount);
      }
   }

}




/************************************************************************
 *
 *  Destroy
 *	Remove the callback lists.
 *
 ************************************************************************/

static void Destroy (sg)
XmSeparatorGadget sg;

{
   XmManagerWidget mw = (XmManagerWidget) XtParent(sg);

   XtReleaseGC (mw, SEPG_SeparatorGC(sg));
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
   XmSeparatorGadget sg = (XmSeparatorGadget) gw;
   XmManagerWidget mw = (XmManagerWidget) XtParent(gw);


   if (curmw->manager.foreground != newmw->manager.foreground ||
       curmw->core.background_pixel != newmw->core.background_pixel)
   {
      XtReleaseGC (mw, SEPG_SeparatorGC(sg));
      GetSeparatorGC (sg);
      return (True);
   }

   return (False);
}




/************************************************************************
 *
 *  SetValues
 *
 ************************************************************************/

static Boolean SetValues (current, request, new)
XmSeparatorGadget current, request, new;

{
   Boolean flag = FALSE;   
   XmManagerWidget new_mw = (XmManagerWidget) XtParent(new);
   XmManagerWidget curr_mw = (XmManagerWidget) XtParent(current);

   /*
    * We never allow our traversal flags to be changed during SetValues();
    * this is enforced by our superclass.
    */
   /*  Force traversal_on to FALSE */
   new -> gadget.traversal_on = FALSE;
 
   /* Force highlightThickness to zero if in a menu. */
   if (XmIsRowColumn(XtParent(new)) && 
       ((RC_Type(XtParent(new)) == XmMENU_PULLDOWN) ||
	(RC_Type(XtParent(new)) == XmMENU_POPUP)))
     new->gadget.highlight_thickness = 0;

   if (SEPG_SeparatorType(new) != XmNO_LINE             &&
       SEPG_SeparatorType(new) != XmSINGLE_LINE         &&
       SEPG_SeparatorType(new) != XmDOUBLE_LINE         &&
       SEPG_SeparatorType(new) != XmSINGLE_DASHED_LINE  &&
       SEPG_SeparatorType(new) != XmDOUBLE_DASHED_LINE  &&
       SEPG_SeparatorType(new) != XmSHADOW_ETCHED_IN    &&
       SEPG_SeparatorType(new) != XmSHADOW_ETCHED_OUT)
   {
      _XmWarning (new, MESSAGE1);
      SEPG_SeparatorType(new) = SEPG_SeparatorType(current);
   }

   if (SEPG_Orientation(new) != XmHORIZONTAL &&
       SEPG_Orientation(new) != XmVERTICAL)
   {
      _XmWarning (new, MESSAGE2);
      SEPG_Orientation(new) = SEPG_Orientation(current);
   }

   if (SEPG_Orientation(new) == XmHORIZONTAL)
   {
      if (request -> rectangle.width == 0)
	 new -> rectangle.width = 2 * new -> gadget.highlight_thickness + 2;

      if (request -> rectangle.height == 0)
      {
	 new -> rectangle.height = 2 * new -> gadget.highlight_thickness;

	 if (SEPG_SeparatorType(new) == XmSINGLE_LINE ||
	     SEPG_SeparatorType(new) == XmSINGLE_DASHED_LINE)
	    new -> rectangle.height += 3;
	 else if (SEPG_SeparatorType(new) == XmSHADOW_ETCHED_IN ||
		  SEPG_SeparatorType(new) == XmSHADOW_ETCHED_OUT)
	    new -> rectangle.height += new -> gadget.shadow_thickness;
	 else if (SEPG_SeparatorType(new) == XmDOUBLE_LINE ||
		  SEPG_SeparatorType(new) == XmDOUBLE_DASHED_LINE)
	    new -> rectangle.height += 5;
	 else
	    if (new -> rectangle.height == 0)
	       new -> rectangle.height = 1;
      }

      if (SEPG_SeparatorType(new) != SEPG_SeparatorType(current) &&
	  request -> rectangle.height == current -> rectangle.height)
      {
	 if (SEPG_SeparatorType(new) == XmSINGLE_LINE ||
	     SEPG_SeparatorType(new) == XmSINGLE_DASHED_LINE)
	    new -> rectangle.height = 2 * new -> gadget.highlight_thickness + 3;
	 else if (SEPG_SeparatorType(new) == XmSHADOW_ETCHED_IN ||
		  SEPG_SeparatorType(new) == XmSHADOW_ETCHED_OUT) 
	    new -> rectangle.height = 2 * new -> gadget.highlight_thickness +
				       new -> gadget.shadow_thickness;
	 else if (SEPG_SeparatorType(new) == XmDOUBLE_LINE ||
		  SEPG_SeparatorType(new) == XmDOUBLE_DASHED_LINE) 
	    new -> rectangle.height = 2 * new -> gadget.highlight_thickness + 5;
         } 
   }

   if (SEPG_Orientation(new) == XmVERTICAL)
   {
      if (request -> rectangle.height == 0)
	 new -> rectangle.height = 2 * new -> gadget.highlight_thickness + 2;

      if (request -> rectangle.width == 0)
      {
	 new -> rectangle.width = 2 * new -> gadget.highlight_thickness;

	 if (SEPG_SeparatorType(new) == XmSINGLE_LINE ||
	     SEPG_SeparatorType(new) == XmSINGLE_DASHED_LINE)
	    new -> rectangle.width += 3;
	 else if (SEPG_SeparatorType(new) == XmSHADOW_ETCHED_IN ||
		  SEPG_SeparatorType(new) == XmSHADOW_ETCHED_OUT)
	    new -> rectangle.width += new -> gadget.shadow_thickness;
	 else if (SEPG_SeparatorType(new) == XmDOUBLE_LINE ||
		  SEPG_SeparatorType(new) == XmDOUBLE_DASHED_LINE)
	    new -> rectangle.width += 5;
	 else
	    if (new -> rectangle.width == 0)
	       new -> rectangle.width = 1;
      }

      if (SEPG_SeparatorType(new) != SEPG_SeparatorType(current) &&
	  request -> rectangle.width == current -> rectangle.width)
      {
	 if (SEPG_SeparatorType(new) == XmSINGLE_LINE ||
	     SEPG_SeparatorType(new) == XmSINGLE_DASHED_LINE)
	    new -> rectangle.width = 2 * new -> gadget.highlight_thickness + 3;
	 else if (SEPG_SeparatorType(new) == XmSHADOW_ETCHED_IN ||
		  SEPG_SeparatorType(new) == XmSHADOW_ETCHED_OUT) 
	    new -> rectangle.width = 2 * new -> gadget.highlight_thickness +
				       new -> gadget.shadow_thickness;
	 else if (SEPG_SeparatorType(new) == XmDOUBLE_LINE ||
		  SEPG_SeparatorType(new) == XmDOUBLE_DASHED_LINE) 
	    new -> rectangle.width = 2 * new -> gadget.highlight_thickness + 5;
         } 
   }
  
   if (SEPG_Orientation(new) != SEPG_Orientation(current) ||
       SEPG_Margin(new) != SEPG_Margin(current) ||
       new -> gadget.shadow_thickness != current -> gadget.shadow_thickness)
      flag = TRUE;

   if (SEPG_SeparatorType(new) != SEPG_SeparatorType(current) ||
       new_mw -> core.background_pixel != curr_mw -> core.background_pixel ||
       new_mw -> manager.foreground != curr_mw -> manager.foreground)
   {
      XtReleaseGC (new_mw, SEPG_SeparatorGC(new));
      GetSeparatorGC (new);
      flag = TRUE;
   }
  
   /* Initialize the interesting input types */
   new->gadget.event_mask = XmHELP_EVENT;

   return (flag);
}


/************************************************************************
 *
 *  Help
 *
 ************************************************************************/

static void Help (sg, event)
XmSeparatorGadget sg;
XEvent * event;

{
   XmRowColumnWidget parent = (XmRowColumnWidget) XtParent(sg);
   XmAnyCallbackStruct call_value;

   if (XmIsRowColumn(parent))
   {
      if (RC_Type(parent) == XmMENU_POPUP ||
	  RC_Type(parent) == XmMENU_PULLDOWN)
      {
	 (* ((XmRowColumnWidgetClass) parent->core.widget_class)->
	  row_column_class.menuProcedures)
	     (XmMENU_POPDOWN, XtParent(sg), NULL, (caddr_t)event, NULL);
      }
   }

   call_value.reason = XmCR_HELP;
   call_value.event = event;
   _XmSocorro((Widget)sg, event);
}


/************************************************************************
 *
 *  XmCreateSeparatorGadget
 *	Create an instance of a separator and return the widget id.
 *
 ************************************************************************/

#ifdef _NO_PROTO
Widget XmCreateSeparatorGadget (parent, name, arglist, argcount)
Widget parent;
char * name;
ArgList arglist;
Cardinal argcount;

#else /* _NO_PROTO */
Widget XmCreateSeparatorGadget (Widget parent, char *name, ArgList arglist, Cardinal argcount)
#endif /* _NO_PROTO */
{
   return (XtCreateWidget (name, xmSeparatorGadgetClass, 
                           parent, arglist, argcount));
}


/****************************************************
 *   Functions for manipulating Secondary Resources.
 *********************************************************/
/*
 * GetSeparatorGClassSecResData()
 *    Create a XmSecondaryResourceDataRec for each secondary resource;
 *    Put the pointers to these records in an array of pointers;
 *    Return the pointer to the array of pointers.
 *	client_data = Address of the structure in the class record which
 *	  represents the (template of ) the secondary data.
 */
static Cardinal
GetSeparatorGClassSecResData  ( class, data_rtn)
WidgetClass class;
XmSecondaryResourceData  **data_rtn;
{   int arrayCount = 0;
    int resNum;
    XmSecondaryResourceData secData, *sd;


    XmBaseClassExt  bcePtr;
    String  resource_class, resource_name;
    XtPointer  client_data;

    bcePtr = &(separatorBaseClassExtRec );
    client_data = NULL;
    resource_class = NULL;
    resource_name = NULL;
    arrayCount =
      _XmSecondaryResourceData ( bcePtr, data_rtn, client_data,
                resource_name, resource_class,
                (XmResourceBaseProc) (GetSeparatorGClassSecResBase)); 
    return (arrayCount);

}

/*
 * GetSeparatorGClassResBase ()
 *   retrun the address of the base of resources.
 *  If client data is the same as the address of the secndary data in the
 *	class record then send the base address of the cache-resources for this
 *	instance of the widget. 
 * Right now we  do not try to get the address of the cached_data from
 *  the Gadget component of this instance - since Gadget class does not
 *	have any cached_resources defined. If later secondary resources are
 *	defined for Gadget class then this routine will have to change.
 */

static XtPointer
GetSeparatorGClassSecResBase ( widget, client_data)
Widget  widget;
XtPointer client_data;
{	XtPointer  widgetSecdataPtr; 
  
	widgetSecdataPtr = (XtPointer) (SEPG_Cache(widget));


    return (widgetSecdataPtr);
}
