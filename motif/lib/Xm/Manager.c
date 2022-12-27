#ifdef REV_INFO
#ifndef lint
static char SCCSID[] = "OSF/Motif: @(#)Manager.c	3.25.1.1 91/02/21";
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

#include <Xm/XmP.h>

#define MESSAGE1 "Invalid shadow thickness."
#define MESSAGE2 "The unit type is incorrect."
#define MESSAGE3 "Cannot set pixmap resource to unspecified."
#define MESSAGE4 "Invalid stringDirection."

#define INVALID_UNIT_TYPE 255

#ifndef NAVIG_IN_GLOBALS
extern void _XmNavigationTypeDefault();
#endif /* NAVIG_IN_GLOBALS */


/*  Core class functions  */

static void    ClassInitialize();
static void    ClassPartInit();
static void    Initialize();
static void    Realize();
static Boolean SetValues();
static void    Destroy();

/* Composite class functions */

static void InsertChild();
static void DeleteChild();

/*  Constraint class functions  */

static void    ConstraintInitialize();
static Boolean ConstraintSetValues();
static void    ConstraintDestroy();

/*  Manager class functions  */

static Boolean
#ifdef _NO_PROTO
ManagerParentProcess();
#else /* _NO_PROTO */
ManagerParentProcess(
            Widget          widget,
            XmParentProcessData data);
#endif /* _NO_PROTO */

/*  GC creation functions  */

static void GetBackgroundGC();
static void GetTopShadowGC();
static void GetBottomShadowGC();
static void GetHighlightGC();


/*  Gadget processing functions  */

static void ManagerEnter();
static void ManagerLeave();
static void ManagerMotion();
static void AddMotionHandlers();
static void CheckRemoveMotionHandlers();
static Boolean GadgetVisualChange();

static void XmDrawEtchedShadow ();
static void get_rects();

/*  Externally referenced functions  */

extern char * _XmGetBGPixmapName();
extern void * _XmClearBGPixmapName();


extern void _XmForegroundColorDefault();
extern void _XmBackgroundColorDefault();
extern void _XmTopShadowColorDefault();
extern void _XmBottomShadowColorDefault();
extern void _XmManagerTopShadowPixmapDefault();
extern void _XmManagerHighlightPixmapDefault();
extern void _XmRegisterPixmapConverters();


/*  Translations for all subclasses of XmManager.               */
/*  Note, if a manager subclass has additional translations it  */
/*  will have to duplicate this set as well.                    */

#ifndef MCCABE
static char managerTraversalTranslations[] = 
	"<Key>osfBeginLine:	ManagerGadgetTraverseHome()\n\
	 <Key>osfUp:        ManagerGadgetTraverseUp()\n\
	 <Key>osfDown:      ManagerGadgetTraverseDown()\n\
	 <Key>osfLeft:      ManagerGadgetTraverseLeft()\n\
	 <Key>osfRight:     ManagerGadgetTraverseRight()\n\
	 Shift ~Meta ~Alt <Key>Tab:	ManagerGadgetPrevTabGroup()\n\
	 ~Meta ~Alt <Key>Tab:		ManagerGadgetNextTabGroup()\n\
	 <EnterWindow>:  ManagerEnter()\n\
	 <LeaveWindow>:  ManagerLeave()\n\
	 <FocusOut>:     ManagerFocusOut()\n\
	 <FocusIn>:      ManagerFocusIn()";

static char defaultTranslations[] = 
	"<Key>osfActivate:	ManagerGadgetSelect() \n\
	 <Key>osfSelect:	ManagerGadgetSelect() \n\
	 <Key>osfHelp:		ManagerGadgetHelp() \n\
	 ~Shift ~Meta ~Alt <Key>Return:	ManagerGadgetSelect() \n\
	 ~Shift ~Meta ~Alt <Key>space:	ManagerGadgetSelect() \n\
	 <Key>:               ManagerGadgetKeyInput() \n\
	 <BtnMotion>:         ManagerGadgetButtonMotion() \n\
	 <Btn1Down>:          ManagerGadgetArm() \n\
	 <Btn1Down>,<Btn1Up>: ManagerGadgetActivate() \n\
	 <Btn1Up>:            ManagerGadgetActivate() \n\
	 <Btn1Down>(2+):      ManagerGadgetMultiArm() \n\
	 <Btn1Up>(2+):        ManagerGadgetMultiActivate()";
#else
static char managerTraversalTranslations[];
static char defaultTranslations[];
#endif

/*  Default actions for all subclasses of XmManager  */

extern void _XmManagerLeave();
static XtActionsRec actionsList[] = {
	{ "ManagerEnter",   (XtActionProc) _XmManagerEnter },
	{ "ManagerLeave",   (XtActionProc) _XmManagerLeave },
	{ "ManagerFocusIn", (XtActionProc) _XmManagerFocusIn },  
	{ "ManagerFocusOut",(XtActionProc) _XmManagerFocusOut },  
	{ "ManagerGadgetPrevTabGroup",
		(XtActionProc) _XmGadgetTraversePrevTabGroup},
	{ "ManagerGadgetNextTabGroup", 
		(XtActionProc) _XmGadgetTraverseNextTabGroup},
	{ "ManagerGadgetTraversePrev",
		(XtActionProc) _XmGadgetTraversePrev },  
	{ "ManagerGadgetTraverseNext",
		(XtActionProc) _XmGadgetTraverseNext },  
	{ "ManagerGadgetTraverseLeft",
		(XtActionProc) _XmGadgetTraverseLeft },  
	{ "ManagerGadgetTraverseRight",
		(XtActionProc) _XmGadgetTraverseRight },  
	{ "ManagerGadgetTraverseUp",
		(XtActionProc) _XmGadgetTraverseUp },  
	{ "ManagerGadgetTraverseDown",
		(XtActionProc) _XmGadgetTraverseDown },  
	{ "ManagerGadgetTraverseHome",
		(XtActionProc) _XmGadgetTraverseHome },
	{ "ManagerGadgetSelect", (XtActionProc) _XmGadgetSelect },
	{ "ManagerGadgetButtonMotion",(XtActionProc)
		_XmGadgetButtonMotion },
	{ "ManagerGadgetKeyInput",(XtActionProc) _XmGadgetKeyInput },
	{ "ManagerGadgetHelp", (XtActionProc) _XmManagerHelp },
	{ "ManagerGadgetArm", (XtActionProc) _XmGadgetArm },
	{ "ManagerGadgetActivate", (XtActionProc) _XmGadgetActivate },
	{ "ManagerGadgetMultiArm", (XtActionProc) _XmGadgetMultiArm },
	{ "ManagerGadgetMultiActivate",
		(XtActionProc) _XmGadgetMultiActivate },
};
 

/*  Resource definitions for Subclasses of XmManager  */

static XtResource resources[] =
{
   {
     XmNx, XmCPosition, XmRHorizontalPosition, sizeof(Position),
     XtOffset (Widget, core.x), XmRImmediate, (caddr_t) 0
   },

   {
     XmNy, XmCPosition, XmRVerticalPosition, sizeof(Position),
     XtOffset (Widget, core.y), XmRImmediate, (caddr_t) 0
   },

   {
     XmNwidth, XmCDimension, XmRHorizontalDimension, sizeof(Dimension),
     XtOffset (Widget, core.width), XmRImmediate, (caddr_t) 0
   },

   {
     XmNheight, XmCDimension, XmRVerticalDimension, sizeof(Dimension),
     XtOffset (Widget, core.height), XmRImmediate, (caddr_t) 0
   },

   {
     XmNborderWidth, XmCBorderWidth, XmRHorizontalDimension, sizeof(Dimension),
     XtOffset (Widget, core.border_width), XmRImmediate, (caddr_t) 0
   },

   {
     XmNforeground, XmCForeground, XmRPixel, sizeof (Pixel),
     XtOffset (XmManagerWidget, manager.foreground),
     XmRCallProc, (caddr_t) _XmForegroundColorDefault
   },

   {
     XmNbackground, XmCBackground, XmRPixel, sizeof (Pixel),
     XtOffset (Widget, core.background_pixel),
     XmRCallProc, (caddr_t) _XmBackgroundColorDefault
   },

   {
     XmNbackgroundPixmap, XmCPixmap, XmRXmBackgroundPixmap, sizeof (Pixmap),
     XtOffset (Widget, core.background_pixmap),
     XmRImmediate, (caddr_t) XmUNSPECIFIED_PIXMAP
   },

   {
     XmNhighlightColor, XmCHighlightColor, XmRPixel, sizeof (Pixel),
     XtOffset (XmManagerWidget, manager.highlight_color),
     XmRString, "Black"
   },

   {
     XmNhighlightPixmap, XmCHighlightPixmap, XmRManHighlightPixmap,
     sizeof (Pixmap), XtOffset (XmManagerWidget, manager.highlight_pixmap),
     XmRCallProc, (caddr_t) _XmManagerHighlightPixmapDefault
   },

   {
     XmNunitType, XmCUnitType, XmRUnitType, sizeof (unsigned char),
     XtOffset(XmManagerWidget, manager.unit_type),
     XmRCallProc, (caddr_t) _XmUnitTypeDefault
   },

   {
     XmNnavigationType, XmCNavigationType, XmRNavigationType, sizeof (unsigned char),
     XtOffset(XmManagerWidget, manager.navigation_type),
     XmRImmediate, (caddr_t) XmTAB_GROUP,
   },

   {
     XmNshadowThickness, XmCShadowThickness, XmRHorizontalDimension, 
	 sizeof (Dimension),
     XtOffset (XmManagerWidget, manager.shadow_thickness),
     XmRImmediate, (caddr_t) 0
   },

   {
     XmNtopShadowColor, XmCTopShadowColor, XmRPixel, sizeof (Pixel),
     XtOffset (XmManagerWidget, manager.top_shadow_color),
     XmRCallProc, (caddr_t) _XmTopShadowColorDefault
   },

   {
     XmNtopShadowPixmap, XmCTopShadowPixmap, XmRManTopShadowPixmap,
     sizeof (Pixmap),
     XtOffset (XmManagerWidget, manager.top_shadow_pixmap),
     XmRCallProc, (caddr_t) _XmManagerTopShadowPixmapDefault
   },

   {
     XmNbottomShadowColor, XmCBottomShadowColor, XmRPixel, sizeof (Pixel),
     XtOffset (XmManagerWidget, manager.bottom_shadow_color),
     XmRCallProc, (caddr_t) _XmBottomShadowColorDefault
   },

   {
     XmNbottomShadowPixmap, XmCBottomShadowPixmap, XmRManBottomShadowPixmap,
     sizeof (Pixmap),
     XtOffset (XmManagerWidget, manager.bottom_shadow_pixmap),
     XmRImmediate, (caddr_t) XmUNSPECIFIED_PIXMAP
   },

   {
     XmNhelpCallback, XmCCallback, XmRCallback, sizeof(XtCallbackList),
     XtOffset (XmManagerWidget, manager.help_callback),
     XmRPointer, (caddr_t) NULL
   },

   {
     XmNuserData, XmCUserData, XmRPointer, sizeof(caddr_t),
     XtOffset (XmManagerWidget, manager.user_data),
     XmRPointer, (caddr_t) NULL
   },

   {
       XmNtraversalOn, XmCTraversalOn, XmRBoolean, sizeof(Boolean),
       XtOffset (XmManagerWidget, manager.traversal_on),
       XmRImmediate, (caddr_t) TRUE
   },
   {
	XmNstringDirection, XmCStringDirection, XmRStringDirection,
	sizeof(XmStringDirection), 
	XtOffset(XmManagerWidget, manager.string_direction),
	XmRImmediate, (caddr_t) XmSTRING_DIRECTION_DEFAULT
   }
};


/*  Definition for resources that need special processing in get values  */

static XmSyntheticResource syn_resources[] =
{
   { XmNx,
     sizeof (Position),
     XtOffset (Widget, core.x), 
     _XmFromHorizontalPixels,
     _XmToHorizontalPixels },

   { XmNy,
     sizeof (Position),
     XtOffset (Widget, core.y), 
     _XmFromVerticalPixels,
     _XmToVerticalPixels },

   { XmNwidth,
     sizeof (Dimension),
     XtOffset (Widget, core.width),
     _XmFromHorizontalPixels,
     _XmToHorizontalPixels },

   { XmNheight,
     sizeof (Dimension),
     XtOffset (Widget, core.height), 
     _XmFromVerticalPixels,
     _XmToVerticalPixels },

   { XmNborderWidth, 
     sizeof (Dimension),
     XtOffset (Widget, core.border_width), 
     _XmFromHorizontalPixels,
     _XmToHorizontalPixels },

   { XmNshadowThickness, 
     sizeof (Dimension),
     XtOffset (XmManagerWidget, manager.shadow_thickness), 
     _XmFromHorizontalPixels,
     _XmToHorizontalPixels }
};



static CompositeClassExtensionRec CompositeClassExtRec = {
    NULL,
    NULLQUARK,
    XtCompositeExtensionVersion,
    sizeof(CompositeClassExtensionRec),
    TRUE,
};
static void		InitializePrehook();
static Boolean		SetValuesPrehook();
static void		InitializePosthook();
static Boolean		SetValuesPosthook();

static XmBaseClassExtRec baseClassExtRec = {
    NULL,
    NULLQUARK,
    XmBaseClassExtVersion,
    sizeof(XmBaseClassExtRec),
    NULL,				/* InitializePrehook	*/
    NULL,				/* SetValuesPrehook	*/
    NULL,				/* InitializePosthook	*/
    NULL,				/* SetValuesPosthook	*/
    NULL,				/* secondaryObjectClass	*/
    NULL,				/* secondaryCreate	*/
    (XmGetSecResDataFunc)NULL,		/* getSecRes data	*/
    0,					/* fastSubclass flags	*/
    NULL,				/* get_values_prehook	*/
    NULL,				/* get_values_posthook	*/
};

/*  The Manager class record definition.  */

externaldef(xmmanagerclassrec) XmManagerClassRec xmManagerClassRec =
{
   {
      (WidgetClass) &constraintClassRec, /* superclass     */	
      "XmManager",		        /* class_name	     */	
      sizeof(XmManagerRec),             /* widget_size	     */	
      ClassInitialize,                  /* class_initialize  */    
      ClassPartInit,                    /* class part initialize */
      False,                            /* class_inited      */	
      Initialize,                       /* initialize	     */	
      NULL,                             /* initialize hook   */
      Realize,                          /* realize	     */	
      actionsList,                      /* actions           */	
      XtNumber(actionsList),            /* num_actions	     */	
      resources,                        /* resources	     */	
      XtNumber(resources),              /* num_resources     */	
      NULLQUARK,                        /* xrm_class	     */	
      True,                             /* compress_motion   */	
      XtExposeCompressMaximal,          /* compress_exposure */	
      True,                             /* compress enterleave */
      False,                            /* visible_interest  */	
      Destroy,                          /* destroy           */	
      NULL,                             /* resize            */	
      NULL,                             /* expose            */	
      SetValues,                        /* set_values	     */	
      NULL,                             /* set_values_hook   */
      XtInheritSetValuesAlmost,         /* set_values_almost */
      _XmManagerGetValuesHook,          /* get_values_hook   */
      NULL,                             /* accept_focus	     */
      XtVersion,                        /* version           */	
      NULL,                             /* callback private  */
      defaultTranslations,              /* tm_table          */
      NULL,                             /* query geometry    */
      NULL,                             /* display_accelerator   */
      (XtPointer)&baseClassExtRec,      /* extension             */
   },
   {					/* composite class   */
      NULL,                             /* Geometry Manager  */
      NULL,                             /* Change Managed    */
      InsertChild,                      /* Insert Child      */
      DeleteChild,	                /* Delete Child      */
      (XtPointer)&CompositeClassExtRec, /* extension         */
   },

   {						/* constraint class	*/
      NULL,					/* resources		*/
      0,					/* num resources	*/
      sizeof (XmManagerConstraintRec),		/* constraint record	*/
      ConstraintInitialize,			/* initialize		*/
      ConstraintDestroy,			/* destroy		*/
      ConstraintSetValues,			/* set values		*/
      NULL,					/* extension		*/
   },

   {						/* manager class	  */
      managerTraversalTranslations,		/* default translations   */
      syn_resources,				/* syn resources      	  */
      XtNumber(syn_resources),			/* num_syn_resources 	  */
      NULL,					/* syn_cont_resources     */
      0,					/* num_syn_cont_resources */
      ManagerParentProcess,                     /* parent_process         */
      NULL,					/* extension		  */
   },
};

externaldef(xmmanagerwidgetclass) WidgetClass xmManagerWidgetClass = 
                                 (WidgetClass) &xmManagerClassRec;




/************************************************************************
 *
 *  ClassInitialize
 *    Initialize the manager class structure.  This is called only the
 *    first time a manager widget is created.
 *
 *
 * After class init, the "translations" variable will contain the compiled
 * translations to be used to augment a widget's translation
 * table if they wish to have keyboard traversal on.
 *
 ************************************************************************/

static void ClassInitialize()
{
   XmRegisterConverters();   /* Register Library Conversion Rtnes */
   _XmRegisterPixmapConverters();

   _XmInitializeExtensions();
   baseClassExtRec.record_type = XmQmotif;
}




/************************************************************************
 *
 *  ClassPartInit
 *
 *  Used by subclasses of manager to inherit class record procedures.
 *
 ************************************************************************/

static CompositeClassExtension _FindClassExtension(widget_class)
    WidgetClass widget_class;
{
    CompositeClassExtension ext;
    for (ext = (CompositeClassExtension)((CompositeWidgetClass)widget_class)
	       ->composite_class.extension;
	 ext != NULL && ext->record_type != NULLQUARK;
	 ext = (CompositeClassExtension)ext->next_extension);

    if (ext != NULL) {
	if (  ext->version == XtCompositeExtensionVersion
	      && ext->record_size == sizeof(CompositeClassExtensionRec)) {
	    /* continue */
	} else {
	    String params[1];
	    Cardinal num_params = 1;
	    params[0] = widget_class->core_class.class_name;
	    XtErrorMsg( "invalidExtension", "ManagerClassPartInitialize",
		        "XmToolkitError",
		 "widget class %s has invalid CompositeClassExtension record",
		 params, &num_params);
	}
    }
    return ext;
}

static void ClassPartInit (wc)
WidgetClass wc;

{
   static Boolean first_time = TRUE;
   XmManagerWidgetClass mw = (XmManagerWidgetClass) wc;
   XmManagerWidgetClass super =
	       (XmManagerWidgetClass) mw->core_class.superclass;
    CompositeClassExtension ext = _FindClassExtension(wc);

    if (ext == NULL) {
	XtPointer *extP
	    = &((CompositeWidgetClass)wc)->composite_class.extension;
	ext = XtNew(CompositeClassExtensionRec);
	bcopy((char*)_FindClassExtension(wc->core_class.superclass),
	      (char*)ext,
	      sizeof(CompositeClassExtensionRec));
	ext->next_extension = *extP;
	*extP = (XtPointer)ext;
    }

   if (mw->manager_class.translations == XtInheritTranslations)
       mw->manager_class.translations = super->manager_class.translations;
   else if (mw->manager_class.translations)
       mw->manager_class.translations = (String)
                       XtParseTranslationTable(mw->manager_class.translations);

   if (mw->manager_class.parent_process == XmInheritParentProcess)
       mw->manager_class.parent_process = super->manager_class.parent_process;

   _XmBaseClassPartInitialize(wc);
   _XmFastSubclassInit (wc, XmMANAGER_BIT);

    if (first_time)
    {
        _XmSortResourceList((XrmResourceList*)
			    xmManagerClassRec.core_class.resources,
			    xmManagerClassRec.core_class.num_resources);
        first_time = FALSE;
    }

   _XmBuildManagerResources((XmManagerWidgetClass )wc);

}



   
/*********************************************************************
 *
 * InsertChild
 *
 *********************************************************************/
static void InsertChild(child)
Widget child;
{
	XmManagerWidget mw = (XmManagerWidget) child->core.parent;
	CompositeClassRec *cc = (CompositeClassRec *) compositeWidgetClass;

	if (!XtIsRectObj(child))
		return;
	
	(*(cc->composite_class.insert_child)) (child);
}

/*********************************************************************
 *
 * DeleteChild
 *
 *********************************************************************/
static void DeleteChild(child)
Widget child;
{
	XmManagerWidget mw = (XmManagerWidget) child->core.parent;
	CompositeClassRec *cc = (CompositeClassRec *) compositeWidgetClass;

	if (!XtIsRectObj(child))
		return;
	
	(*(cc->composite_class.delete_child)) (child);
}



/*********************************************************************
 *
 *  GetBackgroundGC
 *	Get the graphics context used by children for erasing there
 *	highlight border.
 *
 *********************************************************************/
static void GetBackgroundGC (mw)
XmManagerWidget mw;

{
   XGCValues values;
   XtGCMask  valueMask;

   valueMask = GCForeground | GCBackground;
   values.foreground = mw->core.background_pixel;
   values.background = mw->manager.foreground;

   if (mw->core.background_pixmap != XmUNSPECIFIED_PIXMAP)
   {
      valueMask |= GCFillStyle | GCTile;
      values.fill_style = FillTiled;
      values.tile = mw->core.background_pixmap;
   }

   mw->manager.background_GC = XtGetGC (mw, valueMask, &values);
}



   
/************************************************************************
 *
 *  GetHighlightGC
 *     Get the graphics context used for drawing the border.
 *
 ************************************************************************/

static void GetHighlightGC (mw)
XmManagerWidget mw;

{
   XGCValues values;
   XtGCMask  valueMask;

   valueMask = GCForeground | GCBackground;
   values.foreground = mw->manager.highlight_color;
   values.background = mw->core.background_pixel;

   if (mw->manager.highlight_pixmap != XmUNSPECIFIED_PIXMAP)
   {
      valueMask |= GCFillStyle | GCTile;
      values.fill_style = FillTiled;
      values.tile = mw->manager.highlight_pixmap;
   }

   mw->manager.highlight_GC = XtGetGC (mw, valueMask, &values);
}



   
/************************************************************************
 *
 *  GetTopShadowGC
 *     Get the graphics context used for drawing the top shadow.
 *
 ************************************************************************/

static void GetTopShadowGC (mw)
XmManagerWidget mw;

{
   XGCValues values;
   XtGCMask  valueMask;

   valueMask = GCForeground | GCBackground;
   values.foreground = mw->manager.top_shadow_color;
   values.background = mw->manager.foreground;
    
   if (mw->manager.top_shadow_pixmap != XmUNSPECIFIED_PIXMAP)
   {
      valueMask |= GCFillStyle | GCTile;
      values.fill_style = FillTiled;
      values.tile = mw->manager.top_shadow_pixmap;
   }

   mw->manager.top_shadow_GC = XtGetGC ((Widget) mw, valueMask, &values);
}




/************************************************************************
 *
 *  GetBottomShadowGC
 *     Get the graphics context used for drawing the top shadow.
 *
 ************************************************************************/

static void GetBottomShadowGC (mw)
XmManagerWidget mw;

{
   XGCValues values;
   XtGCMask  valueMask;

   valueMask = GCForeground | GCBackground;
   values.foreground = mw->manager.bottom_shadow_color;
   values.background = mw->manager.foreground;
    
   if (mw->manager.bottom_shadow_pixmap != XmUNSPECIFIED_PIXMAP)
   {
      valueMask |= GCFillStyle | GCTile;
      values.fill_style = FillTiled;
      values.tile = mw->manager.bottom_shadow_pixmap;
   }

   mw->manager.bottom_shadow_GC = XtGetGC ((Widget) mw, valueMask, &values);
}




/************************************************************************
 *
 *  Initialize
 *     The main widget instance initialization routine.
 *
 ************************************************************************/

static void Initialize (request, new, args, num_args)
Widget request, new;
ArgList args;
Cardinal * num_args;

{
   XmManagerWidget mw = (XmManagerWidget) new;
   char * name;


   /*  Initialize manager and composite instance data  */

   mw->manager.selected_gadget = NULL;
   mw->manager.highlighted_widget = NULL;
   mw->manager.event_handler_added = False;
   mw->manager.active_child = NULL;
   mw->manager.keyboard_list = NULL;
   mw->manager.num_keyboard_entries = 0;
   mw->manager.size_keyboard_list = 0;


    /*  Checking for valid stringDirection */
   if ((mw->manager.string_direction != XmSTRING_DIRECTION_L_TO_R) &&
       (mw->manager.string_direction != XmSTRING_DIRECTION_R_TO_L) &&
       (mw->manager.string_direction != XmSTRING_DIRECTION_DEFAULT)) {
       _XmWarning(mw, MESSAGE4);
       mw->manager.string_direction = XmSTRING_DIRECTION_DEFAULT;
   }

   if (mw->manager.string_direction == XmSTRING_DIRECTION_DEFAULT)
    {
        if (XmIsManager(mw->core.parent))
            mw->manager.string_direction = 
                 ((XmManagerWidget)(mw->core.parent))->manager.string_direction;
        else
            mw->manager.string_direction = XmSTRING_DIRECTION_L_TO_R;

    }


   _XmNavigInitialize(request, new, args, num_args);

   /*  Verify resource data  */

   if (mw->manager.unit_type != XmPIXELS            &&
       mw->manager.unit_type != Xm100TH_MILLIMETERS &&
       mw->manager.unit_type != Xm1000TH_INCHES     &&
       mw->manager.unit_type != Xm100TH_POINTS      &&
       mw->manager.unit_type != Xm100TH_FONT_UNITS)
   {
      _XmWarning (mw, MESSAGE2);
      mw->manager.unit_type = XmPIXELS;
   }


   /*  Convert the fields from unit values to pixel values  */

   _XmManagerImportArgs((Widget)mw,args,num_args);

   /*  See if the background pixmap name was set by the converter.  */
   /*  If so, generate the background pixmap and put into the       */
   /*  associated core field.                                       */

   name = _XmGetBGPixmapName();
   if (name != NULL)
   {
      mw->core.background_pixmap = 
         XmGetPixmap (XtScreen (mw), name,
                      mw->manager.foreground, mw->core.background_pixel);
      _XmClearBGPixmapName();
   }


   /*  Get the shadow drawing GC's  */

   GetBackgroundGC (mw);
   GetTopShadowGC (mw);
   GetBottomShadowGC (mw);
   GetHighlightGC (mw);


   /* Copy accelerator widget from parent or set to NULL.
    */
   {
      XmManagerWidget p = (XmManagerWidget) XtParent(mw);

      if (XmIsManager(p) && p->manager.accelerator_widget)
         mw->manager.accelerator_widget = p->manager.accelerator_widget;
      else
         mw->manager.accelerator_widget = NULL;
   }
}




/*************************************************************************
 *
 *  Realize
 *	Realize a composite widget.  This function sets the bit
 *	gravity to north west.
 *
 *************************************************************************/

static void Realize (w, p_valueMask, attributes)
Widget w;
Mask * p_valueMask;
XSetWindowAttributes *attributes;

{
   Mask valueMask = *p_valueMask;

   valueMask |= CWBitGravity | CWDontPropagate;
   attributes->bit_gravity = NorthWestGravity;
   attributes->do_not_propagate_mask =
      ButtonPressMask | ButtonReleaseMask |
      KeyPressMask | KeyReleaseMask | PointerMotionMask;

   XtCreateWindow (w, InputOutput, CopyFromParent, valueMask, attributes);
}




/************************************************************************
 *
 *  SetValues
 *     Perform and updating necessary for a set values call.
 *
 ************************************************************************/

static Boolean SetValues (current, request, new, args, num_args)
Widget current, request, new;
ArgList args;
Cardinal * num_args;

{
   Boolean returnFlag = False;
   Boolean visualFlag = False;
   XmManagerWidget curmw = (XmManagerWidget) current;
   XmManagerWidget newmw = (XmManagerWidget) new;
   XmManagerWidget parent;

   /*  Process the change in values */
   
   returnFlag = _XmNavigSetValues(current, request, new, args, num_args);

   /*  Validate changed data.  */

   if (newmw->manager.unit_type != XmPIXELS            &&
       newmw->manager.unit_type != Xm100TH_MILLIMETERS &&
       newmw->manager.unit_type != Xm1000TH_INCHES     &&
       newmw->manager.unit_type != Xm100TH_POINTS      &&
       newmw->manager.unit_type != Xm100TH_FONT_UNITS)
   {
      _XmWarning (newmw, MESSAGE2);
      newmw->manager.unit_type = curmw->manager.unit_type;
   }


   /*  Convert the necessary fields from unit values to pixel values  */

   _XmManagerImportArgs((Widget)newmw, args, num_args);
/****************
 *
 * Don't allow the pixmap resources to be set to XmUNSPECIFIED_PIXMAP.
 *
 ****************/
    if ((curmw->core.background_pixmap != newmw->core.background_pixmap) &&
        (newmw->core.background_pixmap == XmUNSPECIFIED_PIXMAP))
    {
       _XmWarning (newmw, MESSAGE3);
        newmw->core.background_pixmap = curmw->core.background_pixmap;
    }
    if ((curmw->core.border_pixmap != newmw->core.border_pixmap) &&
        (newmw->core.border_pixmap == XmUNSPECIFIED_PIXMAP))
    {
       _XmWarning (newmw, MESSAGE3);
        newmw->core.border_pixmap = curmw->core.border_pixmap;
    }

    if ((curmw->manager.top_shadow_pixmap != newmw->manager.top_shadow_pixmap) &&
        (newmw->manager.top_shadow_pixmap == XmUNSPECIFIED_PIXMAP))
    {
       _XmWarning (newmw, MESSAGE3);
       newmw->manager.top_shadow_pixmap = curmw->manager.top_shadow_pixmap;
    }
    if ((curmw->manager.bottom_shadow_pixmap != newmw->manager.bottom_shadow_pixmap) &&
        (newmw->manager.bottom_shadow_pixmap == XmUNSPECIFIED_PIXMAP))
    {
       _XmWarning (newmw, MESSAGE3);
       newmw->manager.bottom_shadow_pixmap = curmw->manager.bottom_shadow_pixmap;
    }
    if ((curmw->manager.highlight_pixmap != newmw->manager.highlight_pixmap) &&
        (newmw->manager.highlight_pixmap == XmUNSPECIFIED_PIXMAP))
    {
       _XmWarning (newmw, MESSAGE3);
       newmw->manager.highlight_pixmap = curmw->manager.highlight_pixmap;
    }

   /*  If either of the background, shadow, or highlight colors or  */
   /*  pixmaps have changed, destroy and recreate the gc's.         */

   if (curmw->core.background_pixel != newmw->core.background_pixel ||
       curmw->core.background_pixmap != newmw->core.background_pixmap)
   {
      XtReleaseGC (newmw, newmw->manager.background_GC);
      GetBackgroundGC (newmw);
      returnFlag = True;
      visualFlag = True;
   }

   if (curmw->manager.top_shadow_color != newmw->manager.top_shadow_color ||
       curmw->manager.top_shadow_pixmap != newmw->manager.top_shadow_pixmap)
   {
      XtReleaseGC (newmw, newmw->manager.top_shadow_GC);
      GetTopShadowGC (newmw);
      returnFlag = True;
      visualFlag = True;
   }

   if (curmw->manager.bottom_shadow_color != 
          newmw->manager.bottom_shadow_color ||
       curmw->manager.bottom_shadow_pixmap != 
          newmw->manager.bottom_shadow_pixmap)
   {
      XtReleaseGC (newmw, newmw->manager.bottom_shadow_GC);
      GetBottomShadowGC (newmw);
      returnFlag = True;
      visualFlag = True;
   }

   if (curmw->manager.highlight_color != newmw->manager.highlight_color ||
       curmw->manager.highlight_pixmap != newmw->manager.highlight_pixmap)
   {
      XtReleaseGC (newmw, newmw->manager.highlight_GC);
      GetHighlightGC (newmw);
      returnFlag = True;
      visualFlag = True;
   }

   if (curmw->manager.foreground != newmw->manager.foreground       ||
       curmw->core.background_pixel != newmw->core.background_pixel ||
       curmw->core.background_pixmap != newmw->core.background_pixmap)
      visualFlag = True;


   /*  Inform gadget children of possible visual changes  */

   if (visualFlag)
      if (GadgetVisualChange (curmw, newmw))
         returnFlag = True;


   /*  Check the geometry in relationship to the shadow thickness  */

   if (newmw->core.width == 0)
      newmw->core.width += newmw->manager.shadow_thickness * 2;

   if (newmw->core.height == 0)
      newmw->core.height += newmw->manager.shadow_thickness * 2;

    /*  Checking for valid stringDirection */
   if ((newmw->manager.string_direction != XmSTRING_DIRECTION_L_TO_R) &&
       (newmw->manager.string_direction != XmSTRING_DIRECTION_R_TO_L)) {
       _XmWarning(newmw, MESSAGE4);
       newmw->manager.string_direction = curmw->manager.string_direction;
   }

   /*  Return flag to indicate if redraw is needed.  */

   return (returnFlag);
}




/************************************************************************
 *
 *  Destroy
 *     Free up space allocated to track managed children.
 *
 ************************************************************************/

static void Destroy (w)
XmManagerWidget	w;

{
   _XmNavigDestroy((Widget)w);

   XtReleaseGC (w, w->manager.background_GC);
   XtReleaseGC (w, w->manager.top_shadow_GC);
   XtReleaseGC (w, w->manager.bottom_shadow_GC);
   XtReleaseGC (w, w->manager.highlight_GC);
   if( w->manager.selected_gadget == (XmGadget)w)    
     w->manager.selected_gadget = NULL;
   XtRemoveAllCallbacks (w, XmNhelpCallback);
}





/************************************************************************
 *
 *  ConstraintInitialize
 *	The constraint destroy procedure checks to see if a gadget
 *	child is being destroyed.  If so, the managers motion processing
 *	event handlers are checked to see if they need to be removed.
 *
 ************************************************************************/

static void ConstraintInitialize (request, new)
Widget request;
Widget new;

{
   XmGadget g;
   XmManagerWidget parent;

   if (!XtIsRectObj(new)) return;

   parent = (XmManagerWidget) new->core.parent;

   if (XmIsGadget (new))
   {
      g = (XmGadget) new;


      if ((g->gadget.event_mask & 
           (XmENTER_EVENT | XmLEAVE_EVENT | XmMOTION_EVENT)) &&
           parent->manager.event_handler_added == False)
         AddMotionHandlers (parent);
   }
   else if (parent->manager.accelerator_widget
                && !XmIsText( new) && !XmIsTextField( new)  && !XmIsList( new))
      XtInstallAccelerators (new, parent->manager.accelerator_widget);
}




/************************************************************************
 *
 *  ConstraintSetValues
 *	Make sure the managers event handler is set appropriately for
 *	gadget event handling.
 *
 ************************************************************************/

static Boolean ConstraintSetValues (current, request, new)
Widget current, request, new;

{
   XmGadget currentg, newg;
   XmManagerWidget parent;
   unsigned int motion_events;

   if (!XtIsRectObj(new)) return (False);

   /*  If the child is a gadget and its event mask has changed with  */
   /*  respect to the event types which need motion events on the    */
   /*  parent.                                                       */

   if (XmIsGadget (new))
   {
      currentg = (XmGadget) current;
      newg = (XmGadget) new;
      parent = (XmManagerWidget) new->core.parent;

      motion_events = XmENTER_EVENT | XmLEAVE_EVENT | XmMOTION_EVENT;

      if ((newg->gadget.event_mask & motion_events) !=
          (currentg->gadget.event_mask & motion_events))
      {
         if ((newg->gadget.event_mask & motion_events) &&
             parent->manager.event_handler_added == False)
            AddMotionHandlers (parent);

         if ((~(newg->gadget.event_mask & motion_events)) &&
             parent->manager.event_handler_added == True)
            CheckRemoveMotionHandlers (parent);
      }
   }

   return (False);
}




/************************************************************************
 *
 *  ConstraintDestroy
 *	The constraint destroy procedure checks to see if a gadget
 *	child is being destroyed.  If so, the managers motion processing
 *	event handlers are checked to see if they need to be removed.
 *
 ************************************************************************/

static void ConstraintDestroy (w)
Widget w;

{
   XmGadget g;
   XmManagerWidget parent;

   if (!XtIsRectObj(w)) return;

   if (XmIsGadget (w))
   {
      g = (XmGadget) w;
      parent = (XmManagerWidget) w->core.parent;

      if (g->gadget.event_mask & 
          (XmENTER_EVENT | XmLEAVE_EVENT | XmMOTION_EVENT))
         CheckRemoveMotionHandlers (parent);

      if (parent->manager.highlighted_widget == w)
         parent->manager.highlighted_widget = NULL;

      if (parent->manager.selected_gadget == g)
	parent->manager.selected_gadget = NULL;
   }
}

/****************************************************************/
Boolean
#ifdef _NO_PROTO
_XmParentProcess( widget, data)
            Widget          widget ;
            XmParentProcessData data ;
#else /* _NO_PROTO */
_XmParentProcess(
            Widget          widget,
            XmParentProcessData data)
#endif /* _NO_PROTO */
/****************
 * This is the entry point for parent processing.
 ****************/
{
            XmManagerWidgetClass manClass ;
/****************/

    manClass = (XmManagerWidgetClass) widget->core.widget_class ;

    if(    XmIsManager( widget)
        && manClass->manager_class.parent_process    )
    {   
        return( (*manClass->manager_class.parent_process)( widget, data)) ;
        } 

    return( FALSE) ;
    }
/****************************************************************/
static Boolean
#ifdef _NO_PROTO
ManagerParentProcess( widget, data)
            Widget          widget ;
            XmParentProcessData data ;
#else /* _NO_PROTO */
ManagerParentProcess(
            Widget          widget,
            XmParentProcessData data)
#endif /* _NO_PROTO */
/****************
 * 
 ****************/
{
/****************/

    return( _XmParentProcess( XtParent( widget), data)) ;
    }


/************************************************************************
 *
 *  CheckRemoveMotionHandlers
 *	This function loops through the child set checking each gadget 
 *	to see if the need motion events or not.  If no gadget's need
 *	motion events and the motion event handlers have been added,
 *	then remove the event handlers.
 *
 ************************************************************************/

static void CheckRemoveMotionHandlers (mw)
XmManagerWidget mw;

{
   register int i;
   register Widget child;


   /*  If there are any gadgets which need motion events, return.  */

   if (!mw->core.being_destroyed)
   {
      for (i = 0; i < mw->composite.num_children; i++)
      {
         child = mw->composite.children[i];
   
         if (XmIsGadget(child))
         {
            if (((XmGadget) child)->gadget.event_mask & 
                (XmENTER_EVENT | XmLEAVE_EVENT | XmMOTION_EVENT))
            return;
         }
      }
   }


   /*  Remove the motion event handlers  */

   mw->manager.event_handler_added = False;

   XtRemoveEventHandler (mw, PointerMotionMask, False, ManagerMotion, NULL);
   XtRemoveEventHandler (mw, EnterWindowMask, False, ManagerEnter, NULL);
   XtRemoveEventHandler (mw, LeaveWindowMask, False, ManagerLeave, NULL);
}




/************************************************************************
 *
 *  AddMotionHandlers
 *	Add the event handlers necessary to synthisize motion events
 *	for gadgets.
 *
 ************************************************************************/

static void AddMotionHandlers (mw)
XmManagerWidget mw;

{
   mw->manager.event_handler_added = True;

   XtAddEventHandler (mw, PointerMotionMask, False, ManagerMotion, NULL);
   XtAddEventHandler (mw, EnterWindowMask, False, ManagerEnter, NULL);
   XtAddEventHandler (mw, LeaveWindowMask, False, ManagerLeave, NULL);
}




/************************************************************************
 *
 *  ManagerMotion
 *	This function handles the generation of motion, enter, and leave
 *	window events for gadgets and the dispatching of these events to
 *	the gadgets.
 *
 ************************************************************************/

static void ManagerMotion (mw, closure, event)
XmManagerWidget mw;
caddr_t closure;
XPointerMovedEvent * event;

{
   XmGadget gadget;
   XmGadget oldGadget;


   /*  Event on the managers window and not propagated from a child  */

   /* ManagerMotion() creates misleading Enter/Leave events.  A race condition
    * exists such that it's possible that when ManagerMotion() is called, the
    * manager does not yet have the focus.  Dropping the Enter on the floor
    * caused ManagerMotion() to translate the first subsequent motion event
    * into an enter to dispatch to the gadget.  Subsequently button gadget 
    * (un)highlighting on enter/leave was unreliable.  This problem requires 
    * additional investigation.  The quick fix, currently, is for ManagerEnter()
    * and ManagerLeave() to use the event whether or not the manager has the 
    * focus.  In addition, in dispatching enter/leaves to gadgets here in this 
    * routine, ManagerMotion(), bear in mind that we are passing a 
    * XPointerMovedEvent and should probably be creating a synthethic 
    * XCrossingEvent instead.
    *
    * if ((event->subwindow != 0) || !mw->manager.has_focus)
    */
   if (event->subwindow != 0) 
      return;


   gadget = _XmInputForGadget ((CompositeWidget)mw, event->x, event->y);
   oldGadget = (XmGadget) mw->manager.highlighted_widget;


   /*  Dispatch motion events to the child  */

   if (gadget != NULL)
   {
      if (gadget->gadget.event_mask & XmMOTION_EVENT)
         _XmDispatchGadgetInput (gadget, (XEvent*)event, XmMOTION_EVENT);
   }


   /*  Check for and process a leave window condition  */

   if (oldGadget != NULL && gadget != oldGadget)
   {
      if (oldGadget->gadget.event_mask & XmLEAVE_EVENT)
         _XmDispatchGadgetInput (oldGadget, (XEvent*)event, XmLEAVE_EVENT);

      mw->manager.highlighted_widget = NULL;
   }


   /*  Check for and process an enter window condition  */

   if (gadget != NULL && gadget != oldGadget)
   {
      if (gadget->gadget.event_mask & XmENTER_EVENT)
      {
         _XmDispatchGadgetInput (gadget, (XEvent*)event, XmENTER_EVENT);
         mw->manager.highlighted_widget = (Widget) gadget;
      }
      else
         mw->manager.highlighted_widget = NULL;
   }
}




/************************************************************************
 *
 *  ManagerEnter
 *	This function handles the generation of motion and enter window
 *	events for gadgets and the dispatching of these events to the
 *	gadgets.
 *
 ************************************************************************/

static void ManagerEnter (mw, closure, event)
XmManagerWidget mw;
caddr_t closure;
XCrossingEvent * event;

{
   XmGadget gadget;

   /* See ManagerMotion()
    * if (!(mw->manager.has_focus = (Boolean)event->focus)) 
    *    return;
    */

   /*
    * call the traversal action in order to synch things up. This
    * should be cleaned up into a single module |||
    */
   _XmManagerEnter(mw, event);

   gadget = _XmInputForGadget ((CompositeWidget)mw, event->x, event->y);

   /*  Dispatch motion and enter events to the child  */

   if (gadget != NULL)
   {
      if (gadget->gadget.event_mask & XmMOTION_EVENT)
         _XmDispatchGadgetInput (gadget, (XEvent*)event, XmMOTION_EVENT);

      if (gadget->gadget.event_mask & XmENTER_EVENT)
      {
         _XmDispatchGadgetInput (gadget, (XEvent*)event, XmENTER_EVENT);
         mw->manager.highlighted_widget = (Widget) gadget;
      }
      else
         mw->manager.highlighted_widget = NULL;
   }
}




/************************************************************************
 *
 *  ManagerLeave
 *	This function handles the generation of leave window events for
 *	gadgets and the dispatching of these events to the gadgets.
 *
 ************************************************************************/

static void ManagerLeave (mw, closure, event)
XmManagerWidget mw;
caddr_t closure;
XCrossingEvent * event;

{
   XmGadget oldGadget;

   /* See ManagerMotion()
    * if (!(mw->manager.has_focus = (Boolean)event->focus)) 
    *    return;
    */

   oldGadget = (XmGadget) mw->manager.highlighted_widget;

   if (oldGadget != NULL)
   {
      if (oldGadget->gadget.event_mask & XmLEAVE_EVENT)
         _XmDispatchGadgetInput (oldGadget, (XEvent*)event, XmLEAVE_EVENT);
      mw->manager.highlighted_widget = NULL;
   }
   /*
    * call the traversal action in order to synch things up. This
    * should be cleaned up into a single module |||
    */
   _XmManagerLeave(mw, event);

}




/************************************************************************
 *
 *  CallVisualChange
 *	Call the VisualChange class functions of a widget in super class
 *	to sub class order.
 *
 ************************************************************************/

static Boolean CallVisualChange (child, class, cur, new)
XmGadget child;
XmGadgetClass class;
Widget cur;
Widget new;

{
   Boolean redisplay = False;

   if (class->rect_class.superclass != (WidgetClass) &xmGadgetClassRec)
      redisplay = 
         CallVisualChange (child, class->rect_class.superclass, cur, new);

   if (class->gadget_class.visual_change != NULL)
      redisplay = (*(class->gadget_class.visual_change)) (child, cur, new);
               
   return (redisplay);
}




/************************************************************************
 *
 *  GadgetVisualChange
 *	Loop through the child set of new and for any gadget that has
 *	a non-NULL visual_change class function, call the function.
 *      The class function will return True if the gadget needs to
 *	redraw.
 *
 ************************************************************************/

static Boolean GadgetVisualChange (cur, new)
XmManagerWidget new;
XmManagerWidget cur;

{
   register int i;
   XmGadget child;
   WidgetClass class;
   Boolean redisplay = False;

   for (i = 0; i < new->composite.num_children; i++)
   {
      child = (XmGadget) new->composite.children[i];

      if (XmIsGadget(child))
      {
         class = XtClass (child);

         if (CallVisualChange (child, class, cur, new))
            redisplay = True;
      }
   }

   return (redisplay);
}




/************************************************************************
 *
 *	The following are the gadget traversal action routines
 *
 ************************************************************************/

#ifdef _NO_PROTO
void _XmDoGadgetTraversal (mw, event, direction)

   XmManagerWidget mw;
   XEvent * event;
   int direction;

#else /* _NO_PROTO */
void _XmDoGadgetTraversal (XmManagerWidget mw, XEvent *event, int direction)
#endif /* _NO_PROTO */
{
   Widget child;

   if ((child = mw->manager.active_child) && XmIsGadget(child))
   {
      _XmProcessTraversal((Widget)child, direction, True);

      /*
       * Moving between gadgets will not generate focus events, so we
       * must handle highlighting ourselves.
       */
      if (child != mw->manager.active_child)
      {
         _XmDispatchGadgetInput((XmGadget)child, event, XmFOCUS_OUT_EVENT);
         ((XmGadget)child)->gadget.have_traversal = False;

         if ((child = mw->manager.active_child) && XmIsGadget(child))
         {
            _XmDispatchGadgetInput((XmGadget)child, event, XmFOCUS_IN_EVENT);
            ((XmGadget)child)->gadget.have_traversal = True;
         }
      }
   }
}


#ifdef _NO_PROTO
void _XmGadgetTraversePrevTabGroup (mw, event)

   XmManagerWidget mw;
   XEvent * event;

#else /* _NO_PROTO */
void _XmGadgetTraversePrevTabGroup (XmManagerWidget mw, XEvent *event)
#endif /* _NO_PROTO */
{
   Widget child;

   if (child = mw->manager.active_child) {
         _XmDoGadgetTraversal(mw, event, XmTRAVERSE_PREV_TAB_GROUP);
   } else
      if (mw->composite.num_children == 0) {
	 _XmProcessTraversal((Widget)mw, XmTRAVERSE_PREV_TAB_GROUP, TRUE);
      }
}


#ifdef _NO_PROTO
void _XmGadgetTraverseNextTabGroup (mw, event)

   XmManagerWidget mw;
   XEvent * event;

#else /* _NO_PROTO */
void _XmGadgetTraverseNextTabGroup (XmManagerWidget mw, XEvent *event)
#endif /* _NO_PROTO */
{
   Widget child;

   if (child = mw->manager.active_child) {
         _XmDoGadgetTraversal(mw, event, XmTRAVERSE_NEXT_TAB_GROUP);
   } else
      if (mw->composite.num_children == 0) {
	 _XmProcessTraversal((Widget)mw, XmTRAVERSE_NEXT_TAB_GROUP, TRUE);
      }

}


#ifdef _NO_PROTO
void _XmGadgetTraverseLeft (mw, event)

   XmManagerWidget mw;
   XEvent * event;

#else /* _NO_PROTO */
void _XmGadgetTraverseLeft (XmManagerWidget mw, XEvent *event)
#endif /* _NO_PROTO */
{
   Widget child;

   if (child = mw->manager.active_child) {
         _XmDoGadgetTraversal(mw, event, XmTRAVERSE_LEFT);
   }
}

#ifdef _NO_PROTO
void _XmGadgetTraverseRight (mw, event)

   XmManagerWidget mw;
   XEvent * event;

#else /* _NO_PROTO */
void _XmGadgetTraverseRight (XmManagerWidget mw, XEvent *event)
#endif /* _NO_PROTO */
{
   Widget child;

   if (child = mw->manager.active_child) {
         _XmDoGadgetTraversal(mw, event, XmTRAVERSE_RIGHT);
   }
}

#ifdef _NO_PROTO
void _XmGadgetTraverseUp (mw, event)

   XmManagerWidget mw;
   XEvent * event;

#else /* _NO_PROTO */
void _XmGadgetTraverseUp (XmManagerWidget mw, XEvent *event)
#endif /* _NO_PROTO */
{
   Widget child;

   if (child = mw->manager.active_child) {
         _XmDoGadgetTraversal(mw, event, XmTRAVERSE_UP);
   }
}

#ifdef _NO_PROTO
void _XmGadgetTraverseDown (mw, event)

   XmManagerWidget mw;
   XEvent * event;

#else /* _NO_PROTO */
void _XmGadgetTraverseDown (XmManagerWidget mw, XEvent *event)
#endif /* _NO_PROTO */
{
   Widget child;

   if (child = mw->manager.active_child) {
         _XmDoGadgetTraversal(mw, event, XmTRAVERSE_DOWN);
   }
}

#ifdef _NO_PROTO
void _XmGadgetTraverseNext (mw, event)

   XmManagerWidget mw;
   XEvent * event;

#else /* _NO_PROTO */
void _XmGadgetTraverseNext (XmManagerWidget mw, XEvent *event)
#endif /* _NO_PROTO */
{
   Widget child;

   if (child = mw->manager.active_child) {
         _XmDoGadgetTraversal(mw, event, XmTRAVERSE_NEXT);
   }
}


#ifdef _NO_PROTO
void _XmGadgetTraversePrev (mw, event)

   XmManagerWidget mw;
   XEvent * event;

#else /* _NO_PROTO */
void _XmGadgetTraversePrev (XmManagerWidget mw, XEvent *event)
#endif /* _NO_PROTO */
{
   Widget child;

   if (child = mw->manager.active_child) {
         _XmDoGadgetTraversal(mw, event, XmTRAVERSE_PREV);
   }
}


#ifdef _NO_PROTO
void _XmGadgetTraverseHome (mw, event)

   XmManagerWidget mw;
   XEvent * event;

#else /* _NO_PROTO */
void _XmGadgetTraverseHome (XmManagerWidget mw, XEvent *event)
#endif /* _NO_PROTO */
{
   Widget child;

   if (child = mw->manager.active_child) {
         _XmDoGadgetTraversal(mw, event, XmTRAVERSE_HOME);
   }
}

#ifdef _NO_PROTO
void _XmGadgetSelect (mw, event)

   XmManagerWidget mw;
   XEvent * event;

#else /* _NO_PROTO */
void _XmGadgetSelect (XmManagerWidget mw, XEvent *event)
#endif /* _NO_PROTO */
{   
            Widget child ;

    if(    _XmGetFocusPolicy((Widget) mw) == XmEXPLICIT    )
    {   
        child = mw->manager.active_child ;
        if(    child  &&  !XmIsGadget( child)    )
        {   child = NULL ;
            } 
        }
    else /* FocusPolicy == XmPOINTER */
    {   child = (Widget) _XmInputForGadget((CompositeWidget) mw, event->xkey.x, event->xkey.y) ;
        } 
    if(    child
        && (((XmGadgetClass)XtClass( child))->gadget_class.arm_and_activate)  )
    {   
        (*(((XmGadgetClass)XtClass( child))->gadget_class.arm_and_activate))(
                                                                child, event) ;
        }
    return ;
    }

#ifdef _NO_PROTO
void _XmGadgetButtonMotion (mw, event)

   XmManagerWidget mw;
   XEvent * event;

#else /* _NO_PROTO */
void _XmGadgetButtonMotion (XmManagerWidget mw, XEvent *event)
#endif /* _NO_PROTO */
{
            Widget child ;

    if(    _XmGetFocusPolicy((Widget) mw) == XmEXPLICIT    )
    {   
        child = mw->manager.active_child ;
        if(    child  &&  !XmIsGadget( child)    )
        {   child = NULL ;
            } 
        }
    else /* FocusPolicy == XmPOINTER */
    {   child = (Widget) _XmInputForGadget((CompositeWidget)mw, event->xmotion.x,
                                                            event->xmotion.y) ;
        } 
    if(    child    )
    {   _XmDispatchGadgetInput((XmGadget)child, event, XmMOTION_EVENT);
        }
    return ;
    }

#ifdef _NO_PROTO
void _XmGadgetKeyInput (mw, event)

   XmManagerWidget mw;
   XEvent * event;

#else /* _NO_PROTO */
void _XmGadgetKeyInput (XmManagerWidget mw, XEvent *event)
#endif /* _NO_PROTO */
{
            Widget child ;

    if(    _XmGetFocusPolicy((Widget) mw) == XmEXPLICIT    )
    {   
        child = mw->manager.active_child ;
        if(    child  &&  !XmIsGadget( child)    )
        {   child = NULL ;
            } 
        }
    else /* FocusPolicy == XmPOINTER */
    {   child = (Widget) _XmInputForGadget((CompositeWidget)mw, event->xkey.x, event->xkey.y) ;
        } 
    if(    child    )
    {   _XmDispatchGadgetInput((XmGadget)child, event, XmKEY_EVENT);
        }
    return ;
    }

#ifdef _NO_PROTO
void _XmGadgetArm (mw, event)
XmManagerWidget mw;
XButtonPressedEvent *event;
#else /* _NO_PROTO */
void _XmGadgetArm (XmManagerWidget mw, XButtonPressedEvent *event)
#endif /* _NO_PROTO */
{
	
    XmGadget gadget;

    if ((gadget = _XmInputForGadget((CompositeWidget)mw, event->x, event->y)) != NULL)
    {
		(void)XmProcessTraversal((Widget)gadget, XmTRAVERSE_CURRENT);
        _XmDispatchGadgetInput (gadget, (XEvent*)event, XmARM_EVENT);
        mw->manager.selected_gadget = gadget;
    }

    mw->manager.eligible_for_multi_button_event = NULL;
}

#ifdef _NO_PROTO
void _XmGadgetActivate (mw, event)
XmManagerWidget mw;
XButtonPressedEvent *event; 
#else /* _NO_PROTO */
void _XmGadgetActivate (XmManagerWidget mw, XButtonPressedEvent *event)
#endif /* _NO_PROTO */
{
	
    XmGadget gadget;

    /*
     * we emulate automatic grab with owner_events = false by sending
     * the button up to the button down gadget
     */
    if (gadget = mw->manager.selected_gadget)
    {
        _XmDispatchGadgetInput (gadget, (XEvent*)event, XmACTIVATE_EVENT);
        mw->manager.selected_gadget = NULL;
        mw->manager.eligible_for_multi_button_event = gadget;
    }
}

#ifdef _NO_PROTO
void _XmManagerHelp (mw, event)
XmManagerWidget mw;
XEvent *event;
#else /* _NO_PROTO */
void _XmManagerHelp (XmManagerWidget mw, XEvent *event)
#endif /* _NO_PROTO */
{
	XmGadget gadget;

        if (_XmGetFocusPolicy((Widget)mw) == XmEXPLICIT)
        {
          if (gadget = (XmGadget) mw->manager.active_child)
             _XmDispatchGadgetInput (gadget, event, XmHELP_EVENT);
          else
             _XmSocorro((Widget)mw, event);
        }
        else
        {
          if (gadget = (XmGadget) _XmInputInGadget((CompositeWidget)mw, event->xkey.x,
                                                       event->xkey.y))
               _XmDispatchGadgetInput (gadget, event, XmHELP_EVENT);
          else
               _XmSocorro((Widget)mw, event);
        }
}

/************************************************************************
 *                                                                      *
 * _XmSocorro - Help dispatch function.  Start at the widget help was   *
 * invoked on, find the first non-null help callback list, and call it. *
 *                                                                      *
 * Ah, to be at Tech at 49ers...                                        *
 *                                                                      *
 ************************************************************************/
#ifdef _NO_PROTO
void _XmSocorro (w, event)
Widget w;
XKeyPressedEvent *event;
#else /* _NO_PROTO */
void _XmSocorro (Widget w, XEvent *event)
#endif /* _NO_PROTO */
{
    Widget   parent;
    XmAnyCallbackStruct cb;    

    if (w == NULL) return;

    cb.reason = XmCR_HELP;
    cb.event = (XEvent *) event;

    do
    {
        if ((XtHasCallbacks(w, XmNhelpCallback) == XtCallbackHasSome))
        {
            XtCallCallbacks (w, XmNhelpCallback, &cb);
            return;
        }
        else
            w = XtParent(w);
    }    
    while (w != NULL);
}


#ifdef _NO_PROTO
void _XmGadgetMultiArm (mw, event)
XmManagerWidget mw;
XButtonPressedEvent *event;
#else /* _NO_PROTO */
void _XmGadgetMultiArm (XmManagerWidget mw, XButtonPressedEvent *event)
#endif /* _NO_PROTO */
{
	
    XmGadget gadget;

    gadget = _XmInputForGadget((CompositeWidget)mw, event->x, event->y);

    /*
     * If we're not set up for multi_button events, check to see if the
     * input gadget has changed from the active_child.  This means that the
     * user is quickly clicking between gadgets of this manager widget.  If so,
     * arm the gadget as if it were the first button press.
     */
    if (mw->manager.eligible_for_multi_button_event &&
	((gadget = _XmInputForGadget((CompositeWidget)mw, event->x, event->y)) ==
	  mw->manager.eligible_for_multi_button_event))
    {
        _XmDispatchGadgetInput (gadget, (XEvent*)event, XmMULTI_ARM_EVENT);
	mw->manager.selected_gadget = gadget;
    }
    else
       if (gadget && (gadget != (XmGadget)mw->manager.active_child))
	   _XmGadgetArm(mw, event);
       else
	   mw->manager.eligible_for_multi_button_event = NULL;
}

#ifdef _NO_PROTO
void _XmGadgetMultiActivate (mw, event)
XmManagerWidget mw;
XButtonPressedEvent *event;
#else /* _NO_PROTO */
void _XmGadgetMultiActivate (XmManagerWidget mw, XButtonPressedEvent *event)
#endif /* _NO_PROTO */
{
	
    XmGadget gadget;

    /*
     * If we're not set up for multi_button events, call _XmGadgetActivate
     * in case we're quickly selecting a new gadget in which it should
     * be activated as if it were the first button press.
     */
    if (mw->manager.eligible_for_multi_button_event &&
	((gadget = mw->manager.selected_gadget) ==
	  mw->manager.eligible_for_multi_button_event))
    {

        _XmDispatchGadgetInput (gadget, (XEvent*)event, XmMULTI_ACTIVATE_EVENT);
	mw->manager.selected_gadget = NULL;
    }
    else
       _XmGadgetActivate(mw, event);
}



/*
 * The following functions are used by the widgets to query or modify one
 * of the display dependent global variabled used by traversal mechanism.
 */

typedef struct {
   Display * display;
   Boolean   flag;
} _XmBooleanEntry;


static _XmBooleanEntry * transientFlagList = NULL;
static int transientListSize = 0;

static _XmBooleanEntry * resetFocusFlagList = NULL;
static int resetFocusListSize = 0;


/*
 * Get the state of the 'TransientOperationHappening' flag, based upon the
 * display to which the widget is tied.
 */

#ifdef _NO_PROTO
Boolean _XmGetTransientFlag (w)

   Widget w;

#else /* _NO_PROTO */
Boolean _XmGetTransientFlag (Widget w)
#endif /* _NO_PROTO */
{
   register int i;

   for (i = 0; i < transientListSize; i++)
   {
      if (transientFlagList[i].display == XtDisplay(w))
         return (transientFlagList[i].flag);
   }

   return(False);
}


/*
 * Set the state of the 'TransientOperationHappening' flag.
 */

#ifdef _NO_PROTO
void _XmSetTransientFlag (w, value)

   Widget w;
   Boolean value;

#else /* _NO_PROTO */
void _XmSetTransientFlag (Widget w, Boolean value)
#endif /* _NO_PROTO */
{
   register int i;

   /* See if an entry already exists */
   for (i = 0; i < transientListSize; i++)
   {
      if (transientFlagList[i].display == XtDisplay(w))
      {
         transientFlagList[i].flag = value;
         return;
      }
   }

   /* Allocate a new entry */
   transientListSize++;
   transientFlagList = (_XmBooleanEntry *)XtRealloc(transientFlagList,
                         (sizeof(_XmBooleanEntry) * transientListSize));
   transientFlagList[i].display = XtDisplay(w);
   transientFlagList[i].flag = value;
}


/*
 * Get the state of the 'ResettingFocus' flag, based upon the
 * display to which the widget is tied.
 */

#ifdef _NO_PROTO
Boolean _XmGetFocusResetFlag (w)

   Widget w;

#else /* _NO_PROTO */
Boolean _XmGetFocusResetFlag (Widget w)
#endif /* _NO_PROTO */
{
   register int i;

   for (i = 0; i < resetFocusListSize; i++)
   {
      if (resetFocusFlagList[i].display == XtDisplay(w))
         return (resetFocusFlagList[i].flag);
   }

   return(False);
}


/*
 * Set the state of the 'ResettingFocus' flag.
 */

#ifdef _NO_PROTO
void _XmSetFocusResetFlag (w, value)

   Widget w;
   Boolean value;

#else /* _NO_PROTO */
void _XmSetFocusResetFlag (Widget w, Boolean value)
#endif /* _NO_PROTO */
{
   register int i;

   /* See if an entry already exists */
   for (i = 0; i < resetFocusListSize; i++)
   {
      if (resetFocusFlagList[i].display == XtDisplay(w))
      {
         resetFocusFlagList[i].flag = value;
         return;
      }
   }

   /* Allocate a new entry */
   resetFocusListSize++;
   resetFocusFlagList = (_XmBooleanEntry *)XtRealloc(resetFocusFlagList,
                         (sizeof(_XmBooleanEntry) * resetFocusListSize));
   resetFocusFlagList[i].display = XtDisplay(w);
   resetFocusFlagList[i].flag = value;
}


/*
 * Certain widgets, such as those in a menu, would like the application
 * to look non-scraged (i.e. all exposure events have been processed)
 * before invoking a callback which takes a long time to do its thing.
 * This function grabs all exposure events off the queue, and processes
 * them.
 */

#ifdef _NO_PROTO
void XmUpdateDisplay (w)

   Widget w;

#else /* _NO_PROTO */
void XmUpdateDisplay (Widget w)
#endif /* _NO_PROTO */
{
   XEvent event;
   Display * display = XtDisplay(w);

   XSync (display, 0);

   while (XCheckMaskEvent(display, ExposureMask, &event))
      XtDispatchEvent(&event);
}



#ifdef _NO_PROTO
void _XmDrawShadowType(w, shadow_type, core_width, core_height,
	shadow_thickness, highlight_thickness, top_shadow_GC,
	bottom_shadow_GC)
Widget w;
unsigned char shadow_type;
Dimension core_width;
Dimension core_height;
Dimension shadow_thickness;
Dimension highlight_thickness;
GC   top_shadow_GC;
GC   bottom_shadow_GC;

#else /* _NO_PROTO */
void _XmDrawShadowType (Widget w, unsigned int shadow_type, 
                        Dimension core_width, Dimension core_height, 
                        Dimension shadow_thickness, Dimension highlight_thickness, 
                        GC top_shadow_GC, GC bottom_shadow_GC)
#endif /* _NO_PROTO */
{
   if (!XtIsRealized(w)) 
     return;
   switch (shadow_type)
   {
      case XmSHADOW_IN:
      case XmSHADOW_OUT:
	 if (shadow_thickness > 0)
	    _XmDrawShadow (XtDisplay (w), XtWindow (w), 
	     (shadow_type == XmSHADOW_IN) ? bottom_shadow_GC : top_shadow_GC,
	     (shadow_type == XmSHADOW_IN) ? top_shadow_GC : bottom_shadow_GC,
	      shadow_thickness,
	      highlight_thickness,
	      highlight_thickness,
	      core_width - 2 * highlight_thickness,
	      core_height - 2 * highlight_thickness);
      break;

      case XmSHADOW_ETCHED_IN:
      case XmSHADOW_ETCHED_OUT:
           XmDrawEtchedShadow (XtDisplay(w), XtWindow(w), 
	         (shadow_type == XmSHADOW_ETCHED_IN) ?
		    	 bottom_shadow_GC : top_shadow_GC,
	         (shadow_type == XmSHADOW_ETCHED_IN) ?
			     top_shadow_GC : bottom_shadow_GC,
	         shadow_thickness, 
	         highlight_thickness,
		     highlight_thickness, 
	         core_width - 2 * highlight_thickness,
	         core_height - 2 * highlight_thickness);
	    
	    break;

   }
}

/************************************************************************
 *
 *  XmDrawEtchedShadow
 *	Draw an n segment wide etched shadow on the drawable
 *	d, using the provided GC's and rectangle.
 *
 ************************************************************************/

static XRectangle *rects = NULL;
static int rect_count = 0;

static void XmDrawEtchedShadow (display, d, top_GC, bottom_GC, 
                         size, x, y, width, height)
Display * display;
Drawable d;
GC top_GC;
GC bottom_GC;
register int size;
register int x;
register int y;
register int width;
register int height;

{
   int half_size;
   int size2;
   int size3;
   int pos_top, pos_left, pos_bottom, pos_right;

   if (size <= 0) return;
   if (size == 1) 
	{
      _XmDrawShadow (display, d,
         top_GC,  bottom_GC, size, x, y, width, height);
          return;
	} 

   if (size > width / 2) size = width / 2;
   if (size > height / 2) size = height / 2;
   if (size <= 0) return;

   size = (size % 2) ? (size-1) : (size);

   half_size = size / 2;
   size2 = size + size;
   size3 = size2 + size;
   
   if (rect_count == 0)
   {
      rects = (XRectangle *) XtMalloc (sizeof (XRectangle) * size * 4);
      rect_count = size;
   }

   if (rect_count < size)
   {
      rects = (XRectangle *) XtRealloc(rects, sizeof (XRectangle) * size * 4);
      rect_count = size;
   }

   pos_top = 0;
   pos_left = half_size;
   pos_bottom = size2;
   pos_right = size2 + half_size;

   get_rects(half_size, 0, x, y, width, height, 
             pos_top, pos_left, pos_bottom, pos_right);

   pos_top = size3;
   pos_left = size3 + half_size;
   pos_bottom = size;
   pos_right = size + half_size;

   get_rects(half_size, half_size, x, y, width, height, 
                pos_top, pos_left, pos_bottom, pos_right);

   XFillRectangles (display, d, bottom_GC, &rects[size2], size2);
   XFillRectangles (display, d, top_GC, &rects[0], size2);
}


static void get_rects(max_i, offset, x, y, width, height, 
               pos_top, pos_left, pos_bottom, pos_right)

int max_i;
register int offset;
register int x;
register int y;
register int width;
register int height;
register int pos_top, pos_left, pos_bottom, pos_right;

{
   register int i;
   register int offsetX2;
   
   for (i = 0; i < max_i; i++, offset++)
   {
      offsetX2 = offset + offset;

      /*  Top segments  */

      rects[pos_top + i].x = x + offset;
      rects[pos_top + i].y = y + offset;
      rects[pos_top + i].width = width - offsetX2 -1;
      rects[pos_top + i].height = 1;


      /*  Left segments  */

      rects[pos_left + i].x = x + offset;
      rects[pos_left + i].y = y + offset;
      rects[pos_left + i].width = 1;
      rects[pos_left + i].height = height - offsetX2 - 1;


      /*  Bottom segments  */

      rects[pos_bottom + i].x = x + offset;
      rects[pos_bottom + i].y = y + height - offset - 1;
      rects[pos_bottom + i].width = width - offsetX2;
      rects[pos_bottom + i].height = 1;


      /*  Right segments  */

      rects[pos_right + i].x = x + width - offset - 1;
      rects[pos_right + i].y = y + offset;
      rects[pos_right + i].width = 1;
      rects[pos_right + i].height = height - offsetX2;
   }
}


/************************************************************************
 *
 *  _XmClearShadowType
 *	Clear the right and bottom border area and save 
 *	the old width, height and shadow type.
 *
 ************************************************************************/

#ifdef _NO_PROTO
void _XmClearShadowType (w, old_width, old_height, old_shadow_thickness,
                         old_highlight_thickness)

Widget w;
Dimension old_width;
Dimension old_height;
Dimension old_shadow_thickness;
Dimension old_highlight_thickness;

#else /* _NO_PROTO */
void _XmClearShadowType (Widget w, Dimension old_width, 
                         Dimension old_height, Dimension old_shadow_thickness, 
                         Dimension old_highlight_thickness)
#endif /* _NO_PROTO */
{
   if (XtIsRealized(w))
   {
      if (old_width <= w->core.width)
	 XClearArea (XtDisplay (w), XtWindow (w),
	    old_width - old_shadow_thickness - old_highlight_thickness, 0,
	    old_shadow_thickness, old_height - old_highlight_thickness, 
	    False);

      if (old_height <= w->core.height)
	 XClearArea (XtDisplay (w), XtWindow (w),
	    0, old_height - old_shadow_thickness - old_highlight_thickness, 
	    old_width - old_highlight_thickness, old_shadow_thickness, 
	    False);
   }
}




/************************************************************************
 *
 *  _XmDestroyParentCallback
 *     Destroy parent.
 *
 ************************************************************************/
#ifdef _NO_PROTO
void _XmDestroyParentCallback (w, client_data, call_data)
Widget w;
caddr_t client_data;
caddr_t call_data;
#else /* _NO_PROTO */
void _XmDestroyParentCallback (Widget w, caddr_t client_data, caddr_t call_data)
#endif /* _NO_PROTO */
{
   XtDestroyWidget (XtParent (w));
}




