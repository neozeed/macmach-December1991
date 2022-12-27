#ifdef REV_INFO
#ifndef lint
static char SCCSID[] = "OSF/Motif: @(#)Gadget.c	3.10 91/01/10";
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
#include <X11/Vendor.h>


#define MESSAGE1 "Invalid highlight thickness."
#define MESSAGE2 "The unit type is incorrect."
#define MESSAGE3 "Invalid shadow thickness."

#define INVALID_UNIT_TYPE 255

#ifndef NAVIG_IN_GLOBALS
extern void _XmNavigationTypeDefault();
#endif /* NAVIG_IN_GLOBALS */

/*  Core class functions  */

static void    ClassPartInit();
static void    ClassInitialize();
static void    Initialize();
static void    Destroy();
static Boolean SetValues();


/*  Resource definitions for Subclasses of Gadget */

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
     XmNtraversalOn, XmCTraversalOn, XmRBoolean, sizeof (Boolean),
     XtOffset (XmGadget, gadget.traversal_on),
     XmRImmediate, (caddr_t) True
   },

   {
     XmNhighlightOnEnter, XmCHighlightOnEnter, XmRBoolean, sizeof (Boolean),
     XtOffset (XmGadget, gadget.highlight_on_enter),
     XmRImmediate, (caddr_t) False
   },

   {
     XmNhighlightThickness, XmCHighlightThickness, XmRHorizontalDimension,
     sizeof (Dimension), XtOffset (XmGadget, gadget.highlight_thickness),
     XmRImmediate, (caddr_t) 2
   },

   {
     XmNshadowThickness, XmCShadowThickness, XmRHorizontalDimension, 
     sizeof (Dimension), XtOffset (XmGadget, gadget.shadow_thickness),
     XmRImmediate, (caddr_t) 2
   },

   {
     XmNunitType, XmCUnitType, XmRUnitType, sizeof (unsigned char),
     XtOffset (XmGadget, gadget.unit_type),
     XmRCallProc, (caddr_t) _XmUnitTypeDefault
   },

   {
     XmNnavigationType, XmCNavigationType, XmRNavigationType, sizeof (unsigned char),
     XtOffset (XmGadget, gadget.navigation_type),
     XmRImmediate, (caddr_t) XmNONE
   },

   {
     XmNhelpCallback, XmCCallback, XmRCallback, sizeof(XtCallbackList),
     XtOffset (XmGadget, gadget.help_callback),
     XmRPointer, (caddr_t) NULL
   },

   {
     XmNuserData, XmCUserData, XmRPointer, sizeof(caddr_t),
     XtOffset (XmGadget, gadget.user_data),
     XmRPointer, (caddr_t) NULL
   },
};


/*  Definition for resources that need special processing in get values  */

static XmSyntheticResource syn_resources[] =
{
   { XmNx,
     sizeof (Position),
     XtOffset (RectObj, rectangle.x),
     _XmFromHorizontalPixels,
     _XmToHorizontalPixels },

   { XmNy,
     sizeof (Position),
     XtOffset (RectObj, rectangle.y),
     _XmFromVerticalPixels,
     _XmToVerticalPixels },

   { XmNwidth,
     sizeof (Dimension),
     XtOffset (RectObj, rectangle.width),
     _XmFromHorizontalPixels,
     _XmToHorizontalPixels },

   { XmNheight,
     sizeof (Dimension),
     XtOffset (RectObj, rectangle.height),
     _XmFromVerticalPixels,
     _XmToVerticalPixels },

   { XmNhighlightThickness,
     sizeof (Dimension),
     XtOffset (XmGadget, gadget.highlight_thickness),
     _XmFromHorizontalPixels,
     _XmToHorizontalPixels },

   { XmNshadowThickness,
     sizeof (Dimension),
     XtOffset (XmGadget, gadget.shadow_thickness),
     _XmFromHorizontalPixels,
     _XmToHorizontalPixels },
};

static void		InitializePrehook();
static Boolean		SetValuesPrehook();
static void		InitializePosthook();
static Boolean		SetValuesPosthook();

static void SecondaryObjectCreate();

static XmBaseClassExtRec baseClassExtRec = {
    NULL,
    NULLQUARK,
    XmBaseClassExtVersion,
    sizeof(XmBaseClassExtRec),
    NULL,                               /* Initialize Prehook  */
    NULL,                               /* SetValues Prehook   */
    NULL,                               /* Initialize Posthook */
    NULL,                               /* SetValues PostHook  */
    (WidgetClass)NULL,                  /* SecondaryObjectClass */
    (XtInitProc)SecondaryObjectCreate,  /* SecondaryObjectCreate */
    (XmGetSecResDataFunc)NULL,		/* getSecRes data	*/
    {NULL},				/* fastSubclass flags	*/
    NULL,                               /* GetValues Prehook   */
    NULL,                               /* GetValues Posthook   */
};

/*  The gadget class record definition  */

externaldef(xmgadgetclassrec) XmGadgetClassRec xmGadgetClassRec =
{
   {
      (WidgetClass) &rectObjClassRec,   /* superclass	         */	
      "XmGadget",                       /* class_name	         */	
      sizeof(XmGadgetRec),              /* widget_size	         */	
      ClassInitialize,                  /* class_initialize      */
      ClassPartInit,                    /* class part initialize */
      False,                            /* class_inited          */	
      (XtInitProc) Initialize,          /* initialize	         */	
      NULL,                             /* initialize_hook       */
      NULL,	                        /* realize	         */	
      NULL,				/* actions               */	
      0,				/* num_actions	         */	
      resources,                        /* resources	         */	
      XtNumber(resources),              /* num_resources         */	
      NULLQUARK,                        /* xrm_class	         */	
      True,                             /* compress_motion       */
      True,                             /* compress_exposure     */	
      True,                             /* compress_enterleave   */
      False,                            /* visible_interest      */
      (XtWidgetProc) Destroy,           /* destroy               */	
      NULL,                             /* resize                */	
      NULL,				/* expose                */	
      (XtSetValuesFunc) SetValues,      /* set_values	         */	
      NULL,                             /* set_values_hook       */
      XtInheritSetValuesAlmost,         /* set_values_almost     */
      _XmGadgetGetValuesHook,           /* get_values_hook       */
      NULL,                             /* accept_focus	         */	
      XtVersion,                        /* version               */
      NULL,                             /* callback private      */
      NULL,                             /* tm_table              */
      NULL,                             /* query_geometry        */
      NULL,				/* display_accelerator   */
      (XtPointer)&baseClassExtRec,      /* extension             */
   },

   {
      _XmHighlightBorder,		/* border_highlight   */
      _XmUnhighlightBorder,		/* border_unhighlight */
      NULL,				/* arm_and_activate   */
      NULL,				/* input_dispatch     */
      NULL,				/* visual_change      */
      syn_resources,			/* syn resources      */
      XtNumber(syn_resources),		/* num_syn_resources  */
      NULL,				/* cache_part	      */
      NULL,				/* extension          */
   }
};

externaldef(xmgadgetclass) WidgetClass xmGadgetClass = 
		           (WidgetClass) &xmGadgetClassRec;


/************************************************************************
 *
 *  ClassInitialize
 *
 ************************************************************************/

static void ClassInitialize()
{
   _XmInitializeExtensions();
   baseClassExtRec.record_type = XmQmotif;
}


/************************************************************************
 *
 *  ClassPartInit
 *	Used by subclasses of gadget to inherit class record procedures.
 *
 ************************************************************************/

static void ClassPartInit(g)
WidgetClass g;

{
    static Boolean first_time = TRUE;
    XmGadgetClass wc = (XmGadgetClass) g;
    XmGadgetClass super = (XmGadgetClass) wc->rect_class.superclass;

    if (wc->gadget_class.border_highlight == (XtWidgetProc) _XtInherit)
	wc->gadget_class.border_highlight = 
           super->gadget_class.border_highlight;

    if (wc->gadget_class.border_unhighlight == (XtWidgetProc) _XtInherit)
	wc->gadget_class.border_unhighlight =
	   super->gadget_class.border_unhighlight;

    if (wc->gadget_class.arm_and_activate == (XtProc) _XtInherit)
        wc->gadget_class.arm_and_activate =
           super->gadget_class.arm_and_activate;

    if (wc->gadget_class.input_dispatch == (XtWidgetProc) _XtInherit)
        wc->gadget_class.input_dispatch =
           super->gadget_class.input_dispatch;

    if (wc->gadget_class.visual_change == (XmVisualChangeProc) _XtInherit)
        wc->gadget_class.visual_change =
           super->gadget_class.visual_change;

   _XmBaseClassPartInitialize((WidgetClass)wc);
   _XmFastSubclassInit (g, XmGADGET_BIT);

    if (first_time)
    {
        _XmSortResourceList((XrmResourceList*)
			    xmGadgetClassRec.rect_class.resources,
			    xmGadgetClassRec.rect_class.num_resources);
        first_time = FALSE;
    }

   _XmBuildGadgetResources((XmGadgetClass)wc);
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
    Arg                         myArgs[1];
    ArgList                     mergedArgs;

    XtSetArg(myArgs[0] ,XmNlogicalParent, new);

    if (*num_args)
       mergedArgs = XtMergeArgLists(args, *num_args, myArgs, XtNumber(myArgs));
    else
       mergedArgs = myArgs;


    cePtr = _XmGetBaseClassExtPtr(XtClass(new), XmQmotif);
    (void) XtCreateWidget(XtName(new),
                         (*cePtr)->secondaryObjectClass,
			 XtParent(new) ? XtParent(new) : new,
			 mergedArgs, *num_args + 1);

    if (mergedArgs != myArgs)
      XtFree(mergedArgs);
}


static void InsertChild(widget)
    Widget widget;
{
    if (! XtIsWidget(widget) && XtIsRectObj(widget)) {
	XtAppWarningMsg(XtWidgetToApplicationContext(widget),
	       "invalidClass", "shellInsertChild", "XtToolkitError",
	       "Shell does not accept RectObj children; ignored",
	       (String*)NULL, (Cardinal*)NULL);
    }
    else {
	(*((CompositeWidgetClass)vendorShellClassRec.core_class.
	   superclass)->composite_class.insert_child) (widget);
    }
}



/************************************************************************
 *
 *  Initialize
 *     The main widget instance initialization routine.
 *
 ************************************************************************/

static void Initialize (request, new, args, num_args)
XmGadget request, new;
ArgList args;
Cardinal * num_args;

{
    XmGadget 			gw = (XmGadget) new;
    XmBaseClassExt              *cePtr;
    XtInitProc                  secondaryCreate;

    cePtr = _XmGetBaseClassExtPtr(XtClass(new), XmQmotif);

    if ((*cePtr) &&
	(*cePtr)->secondaryObjectClass &&
	(secondaryCreate = (*cePtr)->secondaryObjectCreate))
      (*secondaryCreate)(request, new, args, num_args);

    
   _XmNavigInitialize ((Widget)request,(Widget) new, args, num_args);

   if (gw->gadget.unit_type != XmPIXELS             &&
       gw->gadget.unit_type != Xm100TH_MILLIMETERS &&
       gw->gadget.unit_type != Xm1000TH_INCHES     &&
       gw->gadget.unit_type != Xm100TH_POINTS      &&
       gw->gadget.unit_type != Xm100TH_FONT_UNITS)
   {
      _XmWarning (gw, MESSAGE2);
      gw->gadget.unit_type = XmPIXELS;
   }


   /*  Convert the fields from unit values to pixel values  */

   _XmGadgetImportArgs((Widget)new, args, num_args);

   /*  Check the geometry information for the widget  */

   if (request->rectangle.width == 0)
      gw->rectangle.width += gw->gadget.highlight_thickness * 2 +
                             gw->gadget.shadow_thickness * 2;

   if (request->rectangle.height == 0)
      gw->rectangle.height += gw->gadget.highlight_thickness * 2 + 
                              gw->gadget.shadow_thickness * 2;


   /*  Force the border width to 0  */

   gw->rectangle.border_width = 0;


   /*  Set some additional fields of the widget  */

   gw->gadget.highlight_drawn = False;
   gw->gadget.highlighted = False;
}




/************************************************************************
 *
 *  Destroy
 *	Clean up allocated resources when the widget is destroyed.
 *
 ************************************************************************/

static void Destroy (gw)
XmGadget gw;

{
   _XmNavigDestroy((Widget)gw);
   XtRemoveAllCallbacks (gw, XmNhelpCallback);

}




/************************************************************************
 *
 *  SetValues
 *     Perform and updating necessary for a set values call.
 *
 ************************************************************************/

static Boolean SetValues (cur, req, new, args, num_args)
XmGadget cur, req, new;
ArgList args;
Cardinal * num_args;

{
   Boolean returnFlag = False;

   returnFlag = _XmNavigSetValues ((Widget)cur, (Widget)req, (Widget)new, args, num_args);

   /*  Validate changed data.  */

   if (new->gadget.unit_type != XmPIXELS             &&
       new->gadget.unit_type != Xm100TH_MILLIMETERS &&
       new->gadget.unit_type != Xm1000TH_INCHES     &&
       new->gadget.unit_type != Xm100TH_POINTS      &&
       new->gadget.unit_type != Xm100TH_FONT_UNITS)
   {
      _XmWarning (new, MESSAGE2);
      new->gadget.unit_type = cur->gadget.unit_type;
   }


   /*  Convert the necessary fields from unit values to pixel values  */

   _XmGadgetImportArgs((Widget)new, args, num_args);

   /*  Check for resize conditions  */

   if (cur->gadget.shadow_thickness != new->gadget.shadow_thickness ||
       cur->gadget.highlight_thickness != new->gadget.highlight_thickness)
      returnFlag = True;
   

   /*  Force the border width to 0  */

   new->rectangle.border_width = 0;


   /*  Return a flag which may indicate that a redraw needs to occur.  */
   
   return (returnFlag);
}

