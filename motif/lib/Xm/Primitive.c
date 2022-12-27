#ifdef REV_INFO
#ifndef lint
static char SCCSID[] = "OSF/Motif: @(#)Primitive.c	3.21 91/01/10";
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
#include <ctype.h>
#include <Xm/XmP.h>
#include <X11/Shell.h>
#include <X11/ShellP.h>
#include <X11/VendorP.h>
#include <Xm/PushBGP.h>
#include <Xm/LabelP.h>
#include <Xm/PushBP.h>


#define MESSAGE1 "Invalid highlight thickness."
#define MESSAGE2 "The unit type is incorrect."
#define MESSAGE3 "Invalid shadow thickness."
#define MESSAGE4 "Cannot set pixmap resource to unspecified."

#define INVALID_UNIT_TYPE 255
#define ConstraintClassFlag     0x10	/* definition from IntrinsicI.h */

#ifndef NAVIG_IN_GLOBALS
extern void _XmNavigationTypeDefault();
#endif /* NAVIG_IN_GLOBALS */

/************************************************************************
 *
 *   Default translation table
 *	These translations will be compiled at class initialize.  When
 *	a subclass of primitive is created then these translations will
 *	be used to augment the translations of the subclass IFF
 *	traversal is on.  The SetValues routine will also augment
 *	a subclass's translations table IFF traversal goes from off to on.
 *	Since we are augmenting it should not be a problem when
 *	traversal goes from off to on to off and on again.
 *
 ************************************************************************/

#ifndef MCCABE
static char defaultTranslations[] =
    "<Key>osfBeginLine:	PrimitiveTraverseHome()\n\
     <Key>osfHelp:	PrimitiveHelp()\n\
     <Key>osfUp:	PrimitiveTraverseUp()\n\
     <Key>osfDown:	PrimitiveTraverseDown()\n\
     <Key>osfLeft:	PrimitiveTraverseLeft()\n\
     <Key>osfRight:	PrimitiveTraverseRight()\n\
     Shift ~Meta ~Alt <Key>Tab:	PrimitivePrevTabGroup()\n\
     ~Meta ~Alt <Key>Tab:	PrimitiveNextTabGroup()\n\
    <FocusIn>:       PrimitiveFocusIn()\n\
    <FocusOut>:      PrimitiveFocusOut()\n\
    <Unmap>:         PrimitiveUnmap()";
#else
static char defaultTranslations[];
#endif


/************************************************************************
 *
 *   Action list
 *	Used to provide actions for subclasses with traversal on.
 *
 *************************************<->***********************************/

static XtActionsRec actionsList[] =
{
  {"PrimitiveFocusIn",         (XtActionProc) _XmPrimitiveFocusIn},
  {"PrimitiveFocusOut",        (XtActionProc) _XmPrimitiveFocusOut},
  {"PrimitiveUnmap",           (XtActionProc) _XmPrimitiveUnmap},
  {"PrimitiveHelp",            (XtActionProc) _XmPrimitiveHelp},
  {"PrimitiveTraverseLeft",    (XtActionProc) _XmTraverseLeft },
  {"PrimitiveTraverseRight",   (XtActionProc) _XmTraverseRight },
  {"PrimitiveTraverseUp",      (XtActionProc) _XmTraverseUp },
  {"PrimitiveTraverseDown",    (XtActionProc) _XmTraverseDown },
  {"PrimitiveTraverseNext",    (XtActionProc) _XmTraverseNext },
  {"PrimitiveTraversePrev",    (XtActionProc) _XmTraversePrev },
  {"PrimitiveTraverseHome",    (XtActionProc) _XmTraverseHome },
  {"PrimitiveNextTabGroup",    (XtActionProc) _XmTraverseNextTabGroup },
  {"PrimitivePrevTabGroup",    (XtActionProc) _XmTraversePrevTabGroup },
};



extern char * _XmGetBGPixmapName();
extern void * _XmClearBGPixmapName();

extern void _XmForegroundColorDefault();
extern void _XmBackgroundColorDefault();
extern void _XmTopShadowColorDefault();
extern void _XmBottomShadowColorDefault();
extern void _XmPrimitiveTopShadowPixmapDefault();
extern void _XmPrimitiveHighlightPixmapDefault();
extern void _XmRegisterPixmapConverters();


/*  Resource definitions for Subclasses of Primitive */

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
     XtOffset (XmPrimitiveWidget, primitive.foreground),
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
     XmNtraversalOn, XmCTraversalOn, XmRBoolean, sizeof (Boolean),
     XtOffset (XmPrimitiveWidget, primitive.traversal_on),
     XmRImmediate, (caddr_t) TRUE
   },

   {
     XmNhighlightOnEnter, XmCHighlightOnEnter, XmRBoolean, sizeof (Boolean),
     XtOffset (XmPrimitiveWidget, primitive.highlight_on_enter),
     XmRImmediate, (caddr_t) False
   },

   {
     XmNunitType, XmCUnitType, XmRUnitType, sizeof (unsigned char),
     XtOffset (XmPrimitiveWidget, primitive.unit_type),
     XmRCallProc, (caddr_t) _XmUnitTypeDefault
   },

   {
     XmNnavigationType, XmCNavigationType, XmRNavigationType, sizeof (unsigned char),
     XtOffset (XmPrimitiveWidget, primitive.navigation_type),
     XmRImmediate, (caddr_t) XmNONE
   },

   {
     XmNhighlightThickness, XmCHighlightThickness, XmRHorizontalDimension,
     sizeof (Dimension),
     XtOffset (XmPrimitiveWidget, primitive.highlight_thickness),
     XmRImmediate, (caddr_t) 2
   },

   {
     XmNhighlightColor, XmCHighlightColor, XmRPixel, sizeof (Pixel),
     XtOffset (XmPrimitiveWidget, primitive.highlight_color),
     XmRString, "Black"
   },

   {
     XmNhighlightPixmap, XmCHighlightPixmap, XmRPrimHighlightPixmap,
     sizeof (Pixmap),
     XtOffset (XmPrimitiveWidget, primitive.highlight_pixmap),
     XmRCallProc, (caddr_t) _XmPrimitiveHighlightPixmapDefault
   },

   {
     XmNshadowThickness, XmCShadowThickness, XmRHorizontalDimension,
     sizeof (Dimension),
     XtOffset (XmPrimitiveWidget, primitive.shadow_thickness),
     XmRImmediate, (caddr_t) 2
   },

   {
     XmNtopShadowColor, XmCTopShadowColor, XmRPixel, sizeof (Pixel),
     XtOffset (XmPrimitiveWidget, primitive.top_shadow_color),
     XmRCallProc, (caddr_t) _XmTopShadowColorDefault
   },

   {
     XmNtopShadowPixmap, XmCTopShadowPixmap, XmRPrimTopShadowPixmap,
     sizeof (Pixmap),
     XtOffset (XmPrimitiveWidget, primitive.top_shadow_pixmap),
     XmRCallProc, (caddr_t) _XmPrimitiveTopShadowPixmapDefault
   },

   {
     XmNbottomShadowColor, XmCBottomShadowColor, XmRPixel, sizeof (Pixel),
     XtOffset (XmPrimitiveWidget, primitive.bottom_shadow_color),
     XmRCallProc, (caddr_t) _XmBottomShadowColorDefault
   },

   {
     XmNbottomShadowPixmap, XmCBottomShadowPixmap, XmRPrimBottomShadowPixmap,
     sizeof (Pixmap),
     XtOffset (XmPrimitiveWidget, primitive.bottom_shadow_pixmap),
     XmRImmediate, (caddr_t) XmUNSPECIFIED_PIXMAP
   },

   {
     XmNhelpCallback, XmCCallback, XmRCallback, sizeof(XtCallbackList),
     XtOffset (XmPrimitiveWidget, primitive.help_callback),
     XmRPointer, (caddr_t) NULL
   },

   {
     XmNuserData, XmCUserData, XmRPointer, sizeof(caddr_t),
     XtOffset (XmPrimitiveWidget, primitive.user_data),
     XmRPointer, (caddr_t) NULL
   },
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

   { XmNhighlightThickness, 
     sizeof (Dimension),
     XtOffset (XmPrimitiveWidget, primitive.highlight_thickness), 
     _XmFromHorizontalPixels,
     _XmToHorizontalPixels },

   { XmNshadowThickness, 
     sizeof (Dimension),
     XtOffset (XmPrimitiveWidget, primitive.shadow_thickness), 
     _XmFromHorizontalPixels,
     _XmToHorizontalPixels }
};


/*  Routine definiton used for initializing the class record.  */

static void    ClassInitialize();
static void    ClassPartInitialize();
static void    Initialize();
static void    Realize ();
static void    Destroy();
static Boolean SetValues();

static void GetHighlightGC();
static void GetTopShadowGC();
static void GetBottomShadowGC();

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

/*  The primitive class record definition  */

externaldef(xmprimitiveclassrec) XmPrimitiveClassRec xmPrimitiveClassRec =
{
   {
      (WidgetClass) &widgetClassRec,    /* superclass	         */	
      "XmPrimitive",                    /* class_name	         */	
      sizeof(XmPrimitiveRec),           /* widget_size	         */	
      (XtProc) ClassInitialize,         /* class_initialize      */    
      (XtWidgetClassProc) ClassPartInitialize, /* class_part_initialize */
      False,                            /* class_inited          */	
      (XtInitProc) Initialize,          /* initialize	         */	
      NULL,                             /* initialize_hook       */
      Realize,                          /* realize	         */	
      actionsList,                      /* actions               */	
      XtNumber(actionsList),            /* num_actions	         */	
      resources,                        /* resources	         */	
      XtNumber(resources),              /* num_resources         */	
      NULLQUARK,                        /* xrm_class	         */	
      True,                             /* compress_motion       */
      XtExposeCompressMaximal,          /* compress_exposure     */	
      True,                             /* compress_enterleave   */
      False,                            /* visible_interest      */
      (XtWidgetProc) Destroy,           /* destroy               */	
      NULL,                             /* resize                */	
      NULL,                             /* expose                */	
      (XtSetValuesFunc) SetValues,      /* set_values	         */	
      NULL,                             /* set_values_hook       */
      XtInheritSetValuesAlmost,         /* set_values_almost     */
      _XmPrimitiveGetValuesHook,        /* get_values_hook       */
      NULL,                             /* accept_focus	         */	
      XtVersion,                        /* version               */
      NULL,                             /* callback private      */
      NULL,                             /* tm_table              */
      NULL,                             /* query_geometry        */
      NULL,				/* display_accelerator   */
      (XtPointer)&baseClassExtRec,	/* extension             */
   },

   {
      _XmHighlightBorder,		/* border_highlight   */
      _XmUnhighlightBorder,		/* border_unhighlight */
      defaultTranslations,		/* translations       */
      NULL,				/* arm_and_activate   */
      syn_resources,			/* syn resources      */
      XtNumber(syn_resources),		/* num_syn_resources  */
      NULL,				/* extension          */
   }
};

externaldef(xmprimitivewidgetclass) WidgetClass xmPrimitiveWidgetClass = 
                                    (WidgetClass) &xmPrimitiveClassRec;



/************************************************************************
 *
 *  ClassInitialize
 *    Initialize the primitive class structure.  This is called only
 *    the first time a primitive widget is created.  It registers the
 *    resource type converters unique to this class.
 *
 *
 * After class init, the "translations" variable will contain the compiled
 * translations to be used to augment a widget's translation
 * table if they wish to have keyboard traversal on.
 *
 ************************************************************************/

static void ClassInitialize()
{
   XmRegisterConverters();
   _XmRegisterPixmapConverters();

   _XmInitializeExtensions();
   baseClassExtRec.record_type = XmQmotif;

}




/************************************************************************
 *
 *  ClassPartInitialize
 *    Set up the inheritance mechanism for the routines exported by
 *    primitives class part.
 *
 ************************************************************************/

static void ClassPartInitialize (w)
WidgetClass w;

{
   static Boolean first_time = TRUE;
   XmPrimitiveWidgetClass wc = (XmPrimitiveWidgetClass) w;
   XmPrimitiveWidgetClass super =
      (XmPrimitiveWidgetClass) wc->core_class.superclass;

   if (wc->primitive_class.border_highlight == (XtWidgetProc) _XtInherit)
      wc->primitive_class.border_highlight =
         super->primitive_class.border_highlight;

   if (wc->primitive_class.border_unhighlight == (XtWidgetProc) _XtInherit)
      wc->primitive_class.border_unhighlight =
         super->primitive_class.border_unhighlight;

   if (wc->primitive_class.translations == XtInheritTranslations)
      wc->primitive_class.translations = super->primitive_class.translations;
   else if (wc->primitive_class.translations)
      wc->primitive_class.translations = (String)
                     XtParseTranslationTable(wc->primitive_class.translations);

   _XmBaseClassPartInitialize((WidgetClass)wc);
   _XmFastSubclassInit (wc, XmPRIMITIVE_BIT);

    if (first_time)
    {
        _XmSortResourceList((XrmResourceList*)
			    xmPrimitiveClassRec.core_class.resources,
			    xmPrimitiveClassRec.core_class.num_resources);
        first_time = FALSE;
    }

   _XmBuildPrimitiveResources((XmPrimitiveWidgetClass)wc);

}




   
/************************************************************************
 *
 *  GetHighlightGC
 *     Get the graphics context used for drawing the border.
 *
 ************************************************************************/

static void GetHighlightGC (pw)
XmPrimitiveWidget pw;

{
   XGCValues values;
   XtGCMask  valueMask;

   valueMask = GCForeground | GCBackground;
   values.foreground = pw->primitive.highlight_color;
   values.background = pw->core.background_pixel;

   if (pw->primitive.highlight_pixmap != XmUNSPECIFIED_PIXMAP)
   {
      valueMask |= GCFillStyle | GCTile;
      values.fill_style = FillTiled;
      values.tile = pw->primitive.highlight_pixmap;
   }

   pw->primitive.highlight_GC = XtGetGC ((Widget) pw, valueMask, &values);
}



   
/************************************************************************
 *
 *  GetTopShadowGC
 *     Get the graphics context used for drawing the top shadow.
 *
 ************************************************************************/

static void GetTopShadowGC (pw)
XmPrimitiveWidget pw;

{
   XGCValues values;
   XtGCMask  valueMask;

   valueMask = GCForeground | GCBackground;
   values.foreground = pw->primitive.top_shadow_color;
   values.background = pw->primitive.foreground;
    
   if (pw->primitive.top_shadow_pixmap != XmUNSPECIFIED_PIXMAP)
   {
      valueMask |= GCFillStyle | GCTile;
      values.fill_style = FillTiled;
      values.tile = pw->primitive.top_shadow_pixmap;
   }

    pw->primitive.top_shadow_GC = XtGetGC ((Widget) pw, valueMask, &values);
}




/************************************************************************
 *
 *  GetBottomShadowGC
 *     Get the graphics context used for drawing the top shadow.
 *
 ************************************************************************/

static void GetBottomShadowGC (pw)
XmPrimitiveWidget pw;

{
   XGCValues values;
   XtGCMask  valueMask;

   valueMask = GCForeground | GCBackground;
   values.foreground = pw->primitive.bottom_shadow_color;
   values.background = pw->primitive.foreground;
    
   if (pw->primitive.bottom_shadow_pixmap != XmUNSPECIFIED_PIXMAP)
   {
      valueMask |= GCFillStyle | GCTile;
      values.fill_style = FillTiled;
      values.tile = pw->primitive.bottom_shadow_pixmap;
   }

   pw->primitive.bottom_shadow_GC = XtGetGC((Widget) pw, valueMask, &values);
}




/************************************************************************
 *
 *  Initialize
 *     The main widget instance initialization routine.
 *
 ************************************************************************/

static void Initialize (request, new, args, num_args)
XmPrimitiveWidget request, new;
ArgList args;
Cardinal * num_args;

{
   XmPrimitiveWidget pw = (XmPrimitiveWidget) new;
   char * name;

   _XmNavigInitialize((Widget)request, (Widget)new, args, num_args);

   if (pw->primitive.unit_type != XmPIXELS             &&
       pw->primitive.unit_type != Xm100TH_MILLIMETERS &&
       pw->primitive.unit_type != Xm1000TH_INCHES     &&
       pw->primitive.unit_type != Xm100TH_POINTS      &&
       pw->primitive.unit_type != Xm100TH_FONT_UNITS)
   {
      _XmWarning (pw, MESSAGE2);
      pw->primitive.unit_type = XmPIXELS;
   }


   /*  Convert the fields from unit values to pixel values  */

   _XmPrimitiveImportArgs((Widget)pw, args, num_args);

   /*  Check the geometry information for the widget  */

   if (request->core.width == 0)
      pw->core.width += pw->primitive.highlight_thickness * 2 +
                          pw->primitive.shadow_thickness * 2;

   if (request->core.height == 0)
      pw->core.height += pw->primitive.highlight_thickness * 2 + 
                           pw->primitive.shadow_thickness * 2;


   /*  See if the background pixmap name was set by the converter.  */
   /*  If so, generate the background pixmap and put into the       */
   /*  associated core field.                                       */

   name = _XmGetBGPixmapName();
   if (name != NULL)
   {
      pw->core.background_pixmap = 
         XmGetPixmap (XtScreen (pw), name,
                      pw->primitive.foreground, pw->core.background_pixel);
      _XmClearBGPixmapName();
   }

   /*  Get the graphics contexts for the border drawing  */

   GetHighlightGC (pw);
   GetTopShadowGC (pw);
   GetBottomShadowGC (pw);


   /*  Set some additional fields of the widget  */

   pw->primitive.highlight_drawn = False;
   pw->primitive.highlighted = False;

}




/************************************************************************
 *
 *  Realize
 *	General realize procedure for primitive widgets.  Lets the bit
 *	gravity default to Forget.
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
 *	Clean up allocated resources when the widget is destroyed.
 *
 ************************************************************************/

static void Destroy (pw)
XmPrimitiveWidget pw;

{
   XmManagerWidget parent;

   _XmNavigDestroy((Widget)pw);
   parent = (XmManagerWidget) XtParent(pw);

   XtReleaseGC (pw, pw->primitive.top_shadow_GC);
   XtReleaseGC (pw, pw->primitive.bottom_shadow_GC);
   XtReleaseGC (pw, pw->primitive.highlight_GC);
   XtRemoveAllCallbacks (pw, XmNhelpCallback);

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
   XmPrimitiveWidget curpw = (XmPrimitiveWidget) current;
   XmPrimitiveWidget newpw = (XmPrimitiveWidget) new;
   Boolean returnFlag = False;

   returnFlag = _XmNavigSetValues (current, request, new, args, num_args);

   /*  Validate changed data.  */

   if (newpw->primitive.unit_type != XmPIXELS             &&
       newpw->primitive.unit_type != Xm100TH_MILLIMETERS &&
       newpw->primitive.unit_type != Xm1000TH_INCHES     &&
       newpw->primitive.unit_type != Xm100TH_POINTS      &&
       newpw->primitive.unit_type != Xm100TH_FONT_UNITS)
   {
      _XmWarning (newpw, MESSAGE2);
      newpw->primitive.unit_type = curpw->primitive.unit_type;
   }


   /*  Convert the necessary fields from unit values to pixel values  */

   _XmPrimitiveImportArgs((Widget)newpw, args, num_args);

   /*  Check for resize conditions  */

   if (curpw->primitive.shadow_thickness !=
       newpw->primitive.shadow_thickness ||
       curpw->primitive.highlight_thickness !=
       newpw->primitive.highlight_thickness)
      returnFlag = True;
   
/*
    if ((curpw->primitive.top_shadow_pixmap != newpw->primitive.top_shadow_pixmap) &&
        (newpw->primitive.top_shadow_pixmap == XmUNSPECIFIED_PIXMAP))
    {
       _XmWarning (newpw, MESSAGE4);
       newpw->primitive.top_shadow_pixmap = curpw->primitive.top_shadow_pixmap;
    }
    if ((curpw->primitive.bottom_shadow_pixmap != newpw->primitive.bottom_shadow_pixmap) &&
        (newpw->primitive.bottom_shadow_pixmap == XmUNSPECIFIED_PIXMAP))
    {
       _XmWarning (newpw, MESSAGE4);
       newpw->primitive.bottom_shadow_pixmap = curpw->primitive.bottom_shadow_pixmap;
    }
    if ((curpw->primitive.highlight_pixmap != newpw->primitive.highlight_pixmap) &&
        (newpw->primitive.highlight_pixmap == XmUNSPECIFIED_PIXMAP))
    {
       _XmWarning (newpw, MESSAGE4);
       newpw->primitive.highlight_pixmap = curpw->primitive.highlight_pixmap;
    }
*/

   /*  Check for GC changes  */

   if (curpw->primitive.highlight_color != newpw->primitive.highlight_color ||
       curpw->primitive.highlight_pixmap != newpw->primitive.highlight_pixmap)
   {
      XtReleaseGC (newpw, newpw->primitive.highlight_GC);
      GetHighlightGC (newpw);
      returnFlag = True;
   }

   if (curpw->primitive.top_shadow_color != newpw->primitive.top_shadow_color ||
       curpw->primitive.top_shadow_pixmap != newpw->primitive.top_shadow_pixmap)
   {
      XtReleaseGC (newpw, newpw->primitive.top_shadow_GC);
      GetTopShadowGC (newpw);
      returnFlag = True;
   }

   if (curpw->primitive.bottom_shadow_color != newpw->primitive.bottom_shadow_color ||
       curpw->primitive.bottom_shadow_pixmap != newpw->primitive.bottom_shadow_pixmap)
   {
      XtReleaseGC (newpw, newpw->primitive.bottom_shadow_GC);
      GetBottomShadowGC (newpw);
      returnFlag = True;
   }


   /*  Return a flag which may indicate that a redraw needs to occur.  */
   
   return (returnFlag);
}



/************************************************************************
 *
 *  The traversal event processing routines.
 *    The following set of routines are the entry points invoked from
 *    each primitive widget when one of the traversal event conditions
 *    occur.  These routines are externed in XmP.h.
 *
 ************************************************************************/

#ifdef _NO_PROTO
void _XmTraverseLeft (w, event)
Widget w;
XEvent * event;

#else /* _NO_PROTO */
void _XmTraverseLeft (Widget w, XEvent *event)
#endif /* _NO_PROTO */
{
   _XmProcessTraversal (w, XmTRAVERSE_LEFT, True);
}

#ifdef _NO_PROTO
void _XmTraverseRight (w, event)
Widget w;
XEvent * event;

#else /* _NO_PROTO */
void _XmTraverseRight (Widget w, XEvent *event)
#endif /* _NO_PROTO */
{
   _XmProcessTraversal (w, XmTRAVERSE_RIGHT, True);
}

#ifdef _NO_PROTO
void _XmTraverseUp (w, event)
Widget w;
XEvent * event;

#else /* _NO_PROTO */
void _XmTraverseUp (Widget w, XEvent *event)
#endif /* _NO_PROTO */
{
   _XmProcessTraversal (w, XmTRAVERSE_UP, True);
}


#ifdef _NO_PROTO
void _XmTraverseDown (w, event)
Widget w;
XEvent * event;

#else /* _NO_PROTO */
void _XmTraverseDown (Widget w, XEvent *event)
#endif /* _NO_PROTO */
{
   _XmProcessTraversal (w, XmTRAVERSE_DOWN, True);
}

#ifdef _NO_PROTO
void _XmTraverseNext (w, event)
Widget w;
XEvent * event;

#else /* _NO_PROTO */
void _XmTraverseNext (Widget w, XEvent *event)
#endif /* _NO_PROTO */
{
   _XmProcessTraversal (w, XmTRAVERSE_NEXT, True);
}


#ifdef _NO_PROTO
void _XmTraversePrev (w, event)
Widget w;
XEvent * event;

#else /* _NO_PROTO */
void _XmTraversePrev (Widget w, XEvent *event)
#endif /* _NO_PROTO */
{
   _XmProcessTraversal (w, XmTRAVERSE_PREV, True);
}


#ifdef _NO_PROTO
void _XmTraverseHome (w, event)
Widget w;
XEvent * event;

#else /* _NO_PROTO */
void _XmTraverseHome (Widget w, XEvent *event)
#endif /* _NO_PROTO */
{
   _XmProcessTraversal (w, XmTRAVERSE_HOME, True);
}


#ifdef _NO_PROTO
void _XmTraverseNextTabGroup (w, event)
Widget w;
XEvent * event;

#else /* _NO_PROTO */
void _XmTraverseNextTabGroup (Widget w, XEvent *event)
#endif /* _NO_PROTO */
{
   _XmProcessTraversal (w, XmTRAVERSE_NEXT_TAB_GROUP, True);
}


#ifdef _NO_PROTO
void _XmTraversePrevTabGroup (w, event)
Widget w;
XEvent * event;

#else /* _NO_PROTO */
void _XmTraversePrevTabGroup (Widget w, XEvent *event)
#endif /* _NO_PROTO */
{
   _XmProcessTraversal (w, XmTRAVERSE_PREV_TAB_GROUP, True);
}


/************************************************************************
 *
 *  The border highlighting and unhighlighting routines.
 *
 *  These routines are called through primitive and gadget class fields.
 *  They are also directly referenced by primitives and gadgets through
 *  there redisplay functions.
 *
 ************************************************************************/

#ifdef _NO_PROTO
Boolean _XmDifferentBackground (w, parent)
Widget w;
Widget parent;

#else /* _NO_PROTO */
Boolean _XmDifferentBackground (Widget w, Widget parent)
#endif /* _NO_PROTO */
{
   if (XmIsPrimitive (w) && XmIsManager (parent))
   {
      if (w->core.background_pixel != parent->core.background_pixel ||
          w->core.background_pixmap != parent->core.background_pixmap)
         return (True);
   }

   return (False);
}



#ifdef _NO_PROTO
void _XmDrawBorder (w, gc, x, y, width, height, highlight_width)
Widget w;
GC gc;
Position x;
Position y;
Dimension width;
Dimension height;
Dimension highlight_width;
#else /* _NO_PROTO */
void _XmDrawBorder ( Widget w, GC gc, Position x, Position y, 
	Dimension width, Dimension height, Dimension highlight_width)
#endif /* _NO_PROTO */

{
   XRectangle rect[4];

   rect[0].x = x;
   rect[0].y = y;
   rect[0].width = width;
   rect[0].height = highlight_width;

   rect[1].x = x;
   rect[1].y = y;
   rect[1].width = highlight_width;
   rect[1].height = height;

   rect[2].x = x + width - highlight_width;
   rect[2].y = y;
   rect[2].width = highlight_width;
   rect[2].height = height;

   rect[3].x = x;
   rect[3].y = y + height - highlight_width;
   rect[3].width = width;
   rect[3].height = highlight_width;

   XFillRectangles (XtDisplay (w), XtWindow (w), gc, &rect[0], 4);
}




/************************************************************************
 *
 *  _XmHighlightBorder
 *	Draw the traversal/highlight_on_enter border for the widget.
 *
 ************************************************************************/

#ifdef _NO_PROTO
void _XmHighlightBorder (w)
Widget w;

#else /* _NO_PROTO */
void _XmHighlightBorder (Widget w)
#endif /* _NO_PROTO */
{
   XmPrimitiveWidget pw;
   XmGadget g;
   register int width;
   register int height;
   register int highlight_width;

   g = (XmGadget) w;
   pw = (XmPrimitiveWidget) w;

   width = g->rectangle.width;
   height = g->rectangle.height;

   if (width == 0 || height == 0) return;

   if (XmIsPrimitive(w))
   {
      if (pw->primitive.highlight_thickness == 0) return;

      pw->primitive.highlighted = True;
      pw->primitive.highlight_drawn = True;

	  if (XmIsPushButton(pw))
	   { if ( ((XmPushButtonRec *) pw)->
					pushbutton.default_button_shadow_thickness > 0)
		     highlight_width = pw->primitive.highlight_thickness
								 - XM_3D_ENHANCE_PIXEL;
	     else
		    highlight_width = pw->primitive.highlight_thickness;
	   }
	  else
			highlight_width = pw->primitive.highlight_thickness;


      _XmDrawBorder ((Widget)pw, pw->primitive.highlight_GC,
                  0, 0, width, height, highlight_width);
   }
   else
   {
      if (g->gadget.highlight_thickness == 0) return;

      g->gadget.highlighted = True;
      g ->gadget.highlight_drawn = True;
	  
	  if (g->object.widget_class == xmPushButtonGadgetClass)
       { if (PBG_DefaultButtonShadowThickness (g) > 0)
		  highlight_width = g->gadget.highlight_thickness
								- XM_3D_ENHANCE_PIXEL ;
	     else
		  highlight_width = g->gadget.highlight_thickness;
	   }
	  else
	     highlight_width = g->gadget.highlight_thickness;
      _XmDrawBorder ((Widget)g, 
                ((XmManagerWidget)(g->object.parent))->manager.highlight_GC,
                  g->rectangle.x, g->rectangle.y, width, height,
                   highlight_width );

   }
}




/************************************************************************
 *
 *  _XmUnhighlightBorder
 *	Clear the traversal/highlight_on_enter border for the widget.
 *
 ************************************************************************/

#ifdef _NO_PROTO
void _XmUnhighlightBorder (w)
Widget w;

#else /* _NO_PROTO */
void _XmUnhighlightBorder (Widget w)
#endif /* _NO_PROTO */
{
   XmPrimitiveWidget pw;
   XmGadget g;
   register int x;
   register int y;
   register int window_width;
   register int window_height;
   register highlight_width;


   g = (XmGadget) w;
   pw = (XmPrimitiveWidget) w;

   x = g->rectangle.x;
   y = g->rectangle.y;
   window_width = g->rectangle.width;
   window_height = g->rectangle.height;

   if (window_width == 0 || window_height == 0) return;

   if (XmIsPrimitive(w))
   {
      highlight_width = pw->primitive.highlight_thickness;
      if (highlight_width == 0) return;

      x = 0;
      y = 0;
      pw->primitive.highlighted = False;
      pw->primitive.highlight_drawn = False;

      if (XmIsManager (pw->core.parent))
      {
         _XmDrawBorder ((Widget)pw, 
                     ((XmManagerWidget)(pw->core.parent))->manager.background_GC,
                     0, 0, window_width, window_height, highlight_width);
         return;
      }
   }
   else
   {
      highlight_width = g->gadget.highlight_thickness;
      if (highlight_width == 0) return;

      g->gadget.highlighted = False;
      g->gadget.highlight_drawn = False;
   }

   XClearArea (XtDisplay (pw), XtWindow (pw),
               x, y, window_width, highlight_width, False);

   XClearArea (XtDisplay (pw), XtWindow (pw),
               x, y, highlight_width, window_height, False);

   XClearArea (XtDisplay (pw), XtWindow (pw),
               x + window_width - highlight_width, y, 
               highlight_width, window_height, False);

   XClearArea (XtDisplay (pw), XtWindow (pw),
               x, y + window_height - highlight_width,
               window_width, highlight_width, False);
}





/************************************************************************
 *
 *  _XmDrawShadow
 *	Draw an n segment wide bordering shadow on the drawable
 *	d, using the provided GC's and rectangle.
 *
 ************************************************************************/

#ifdef _NO_PROTO
void _XmDrawShadow (display, d, top_GC, bottom_GC, size, x, y, width, height)
Display * display;
Drawable d;
GC top_GC;
GC bottom_GC;
register int size;
register int x;
register int y;
register int width;
register int height;

#else /* _NO_PROTO */
void _XmDrawShadow (Display *display, Drawable d, 
                    GC top_GC, GC bottom_GC, int size, int x, int y, 
                    int width, int height)
#endif /* _NO_PROTO */
{
   static XRectangle * rects = NULL;
   static int rect_count = 0;
   register int i;
   register int size2;
   register int size3;

   if (size <= 0) return;
   if (size > width / 2) size = width / 2;
   if (size > height / 2) size = height / 2;
   if (size <= 0) return;

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

   size2 = size + size;
   size3 = size2 + size;

   for (i = 0; i < size; i++)
   {
      /*  Top segments  */

      rects[i].x = x;
      rects[i].y = y + i;
      rects[i].width = width - i;
      rects[i].height = 1;


      /*  Left segments  */

      rects[i + size].x = x + i;
      rects[i + size].y = y;
      rects[i + size].width = 1;
      rects[i + size].height = height - i;


      /*  Bottom segments  */

      rects[i + size2].x = x + i + 1;
      rects[i + size2].y = y + height - i - 1;
      rects[i + size2].width = width - i - 1;
      rects[i + size2].height = 1;


      /*  Right segments  */

      rects[i + size3].x = x + width - i - 1;
      rects[i + size3].y = y + i + 1;
      rects[i + size3].width = 1;
      rects[i + size3].height = height - i - 1;
   }

   XFillRectangles (display, d, top_GC, &rects[0], size2);
   XFillRectangles (display, d, bottom_GC, &rects[size2], size2);
}



/************************************************************************
 *
 *  _XmEraseShadow
 *	Erase an n segment wide bordering shadow on the drawable
 *	d, using the provided  rectangle.
 *
 ************************************************************************/

#ifdef _NO_PROTO
void _XmEraseShadow (display, d, size, x, y, width, height)
Display * display;
Drawable d;
register int size;
register int x;
register int y;
register int width;
register int height;
#else /* _NO_PROTO */
void _XmEraseShadow (Display *display, Drawable d, int size, 
                     int x, int y, int width, int height)
#endif /* _NO_PROTO */
{
   if (width > 0 && size > 0)
   {
      XClearArea (display, d, x, y, width, size, FALSE);
      XClearArea (display, d, x, y + height - size, width, size, FALSE);
   }

   if (size > 0 && height - (2 * size) > 0)
   {
      XClearArea (display, d, x, y + size, size, height - (2 * size), FALSE);
      XClearArea (display, d, x + width - size, y + size, size, 
                  height - (2 * size), FALSE);
   }
}

#ifdef _NO_PROTO
void _XmPrimitiveHelp (pw, event)
XmPrimitiveWidget pw;
XKeyPressedEvent *event;
#else /*  _NO_PROTO */
void _XmPrimitiveHelp (XmPrimitiveWidget pw, XEvent *event)
#endif /* _NO_PROTO */
{
    _XmSocorro((Widget)pw, event);
}

/************************************************************************
 *
 *   XmResolvePartOffsets
 *
 ************************************************************************/

#ifdef _NO_PROTO
void XmResolvePartOffsets (class, offset)
WidgetClass class;
XmOffsetPtr *offset;

#else /* _NO_PROTO */
void XmResolvePartOffsets (WidgetClass class, XmOffsetPtr *offset)
#endif /* _NO_PROTO */
{
   WidgetClass c, super = class->core_class.superclass;
   int i, classcount = 0;
   XmPartResource *pr;

   class->core_class.widget_size += super->core_class.widget_size;

   for (c = class; c != NULL; c = c->core_class.superclass) classcount++;

   *offset = (XmOffsetPtr) XtMalloc(classcount * sizeof(XmOffset));

   for (i = classcount-1, c = super; i > 0; c = c->core_class.superclass, i--)
      (*offset)[i] = c->core_class.widget_size;

   (*offset)[0] = 0;

   for (i = 0; i < class->core_class.num_resources; i++) 
   {
      pr = (XmPartResource *) &class->core_class.resources[i];

      /* The next line updates this in place--be careful */

      class->core_class.resources[i].resource_offset =
         XmGetPartOffset(pr, offset);
   }
}

/************************************************************************
 *
 *   XmResolveAllPartOffsets
 *
 ************************************************************************/

/*
 *  Include IntrinsicI.h to get the fastsubclass bit assignments
 */

#define XtIsConstraintClass(wc) \
    (((WidgetClass)(wc))->core_class.class_inited & ConstraintClassFlag)

#ifdef _NO_PROTO
void XmResolveAllPartOffsets (class, offset, constraint_offset)
WidgetClass class;
XmOffsetPtr *offset;
XmOffsetPtr *constraint_offset;

#else /* _NO_PROTO */
void XmResolveAllPartOffsets (WidgetClass class, XmOffsetPtr *offset,
	XmOffsetPtr *constraint_offset)
#endif /* _NO_PROTO */
{
   WidgetClass c, super = class->core_class.superclass;
   ConstraintWidgetClass cc = NULL, scc = NULL;
   int i, classcount = 0;
   XmPartResource *pr;

   /*
    *  Set up constraint class pointers
    */
   if (XtIsConstraintClass(super))
   {
        cc = (ConstraintWidgetClass)class;
	scc = (ConstraintWidgetClass)super;
   }

   /*
    *  Update the part size value (initially, it is the size of this part)
    */
   class->core_class.widget_size += super->core_class.widget_size;
   if (cc && scc)
       cc->constraint_class.constraint_size +=
scc->constraint_class.constraint_size;

   /*
    *  Count the number of superclasses and allocate the offset record(s)
    */
   for (c = class; c != NULL; c = c->core_class.superclass) classcount++;

   *offset = (XmOffsetPtr) XtMalloc(classcount * sizeof(XmOffset));
   if (cc)
       *constraint_offset = (XmOffsetPtr) XtMalloc(classcount * sizeof(XmOffset));
   else 
       if(constraint_offset != NULL) *constraint_offset = NULL;

   /*
    *  Fill in the offset table(s) with the offset of all parts
    */
   for (i = classcount-1, c = super; i > 0; c = c->core_class.superclass, i--)
      (*offset)[i] = c->core_class.widget_size;

   (*offset)[0] = 0;

   if (constraint_offset != NULL && *constraint_offset != NULL)
   {
       for (i = classcount-1, scc = (ConstraintWidgetClass) super; i > 0; 
		scc = (ConstraintWidgetClass)(scc->core_class.superclass), i--)
	  if (XtIsConstraintClass(scc))
              (*constraint_offset)[i] = scc->constraint_class.constraint_size;
	  else
	      (*constraint_offset)[i] = 0;
	
       (*constraint_offset)[0] = 0;
   }

   /*
    *  Update the resource list(s) offsets in place
    */
   for (i = 0; i < class->core_class.num_resources; i++) 
   {
      pr = (XmPartResource *) &class->core_class.resources[i];

      /* The next line updates this in place--be careful */

      class->core_class.resources[i].resource_offset =
         XmGetPartOffset(pr, offset);
   }

   if (cc)
       for (i = 0; i < cc->constraint_class.num_resources; i++) 
       {
          pr = (XmPartResource *) &cc->constraint_class.resources[i];

          /* The next line updates this in place--be careful */

          cc->constraint_class.resources[i].resource_offset =
             XmGetPartOffset(pr, constraint_offset);
       }
}

