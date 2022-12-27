#ifdef REV_INFO
#ifndef lint
static char SCCSID[] = "OSF/Motif: @(#)CascadeBG.c	3.29 91/01/10";
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

#include <X11/keysymdef.h>
#include <Xm/XmP.h>
#include <X11/ShellP.h>
#include <Xm/MenuShellP.h>
#include <Xm/RowColumnP.h>
#include <Xm/LabelGP.h>
#include <Xm/CascadeBGP.h>
#include <Xm/LabelP.h>
#include <Xm/CascadeBP.h>
#include <Xm/ExtObjectP.h>
#include <Xm/CacheP.h>
#include <Xm/BaseClassI.h>

#define CASCADE_PIX_SPACE    4	/* pixels between label and bit map */
#define MAP_DELAY_DEFAULT   180
#define EVENTS              ButtonPressMask | ButtonReleaseMask | \
                            EnterWindowMask | LeaveWindowMask

#define WRONGPARENT "XmCascadeButtonGadget must have xmRowColumnWidgetClass \
 parent with rowColumnType XmMENU_PULLDOWN, XmMENU_POPUP, XmMENU_BAR or \
 XmMENU_OPTION."
#define WRONGSUBMENU "Only XmMENU_PULLDOWN XmRowColumnWidgets can be submenus."
#define WRONGMAPDELAY "MapDelay must be >= 0."

extern XContext _XmMenuCursorContext;
extern void _XmCreateArrowPixmaps() ;

/*
 * forward declarations
 */

static Boolean SetValues ();
static void ClassInitialize();
static void ClassPartInitialize();
static void Initialize ();
static void Destroy ();
static void Resize ();
static void Redisplay ();
static void BorderHighlight ();
static void BorderUnhighlight ();
static void ArmAndActivate ();
static void Arm ();
static void ArmAndPost ();
static void Select ();
static void Disarm ();
static void InputDispatch ();
static void DelayedArm ();
static void CheckDisarm ();
static void StartDrag ();
static void DoSelect ();
static void KeySelect ();
static void MenuBarSelect ();
static void MenuBarEnter ();
static void MenuBarLeave ();

static int position_cascade ();

static int _XmCascadeBCacheCompare();

/* Initialize and SetValues Pre and Post hooks */

static void InitializePosthook();
static Boolean SetValuesPrehook();
static Boolean SetValuesPosthook();
static void GetValuesPrehook();
static void GetValuesPosthook();

/*
 * Procedures for manipulating cached_resources as secondary data.
 */
static XtPointer GetCascadeBGClassSecResBase();

static XtResource resources[] = 
{
    {   XmNactivateCallback,
        XmCCallback,
        XmRCallback,
        sizeof (XtCallbackList),
        XtOffset (XmCascadeButtonGadget, cascade_button.activate_callback),
        XmRCallback,
        NULL
    },

    {   XmNcascadingCallback,
        XmCCallback,
        XmRCallback,
        sizeof (XtCallbackList),
        XtOffset (XmCascadeButtonGadget, cascade_button.cascade_callback),
        XmRCallback,
        NULL
    },

    {	XmNsubMenuId, 
	XmCMenuWidget,				/* submenu */
	XmRMenuWidget, 
	sizeof (Widget),
	XtOffset (XmCascadeButtonGadget, cascade_button.submenu), 
	XmRMenuWidget, 
	(caddr_t) 0
    },
    {
        XmNshadowThickness,
        XmCShadowThickness,
        XmRHorizontalDimension,
        sizeof (Dimension),
        XtOffset (XmCascadeButtonGadget, gadget.shadow_thickness),
        XmRImmediate,
        (caddr_t) 2
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
};       


static XtResource cache_resources[] =
{

   {   XmNcascadePixmap, 
       XmCPixmap, 
       XmRGadgetPixmap,
       sizeof(Pixmap),
       XtOffset (XmCascadeButtonGCacheObject,
                 cascade_button_cache.cascade_pixmap), 
       XmRImmediate,
       (caddr_t) XmUNSPECIFIED_PIXMAP
   },

   {   XmNmappingDelay,
       XmCMappingDelay,
       XmRInt,
       sizeof (int),
       XtOffset (XmCascadeButtonGCacheObject,
	         cascade_button_cache.map_delay),
       XmRImmediate,
       (caddr_t) MAP_DELAY_DEFAULT
   },
};


static XmCacheClassPart CascadeButtonClassCachePart = {
    {NULL, 0, 0},        /* head of class cache list */
    _XmCacheCopy,       /* Copy routine     */
    _XmCacheDelete,     /* Delete routine   */
    _XmCascadeBCacheCompare,    /* Comparison routine   */
};

static void SecondaryObjectCreate();

static XmBaseClassExtRec   CascadeBGClassExtensionRec = {
    NULL,    					    /* next_extension        */
    NULLQUARK,    				    /* record_typ  	     */
    XmBaseClassExtVersion,      		    /* version  	     */
    sizeof(XmBaseClassExtRec),  		    /* record_size  	     */
    XmInheritInitializePrehook,   		    /* initializePrehook     */
    SetValuesPrehook,   			    /* setValuesPrehook	     */
    InitializePosthook,   			    /* initializePosthook    */
    SetValuesPosthook,   			    /* setValuesPosthook     */
    (WidgetClass)&xmCascadeButtonGCacheObjClassRec, /* secondaryObjectClass  */
    (XtInitProc)SecondaryObjectCreate,   	    /* secondaryObjectCreate */
    (XmGetSecResDataFunc) GetCascadeBGClassSecResBase, /*  GetCascadeBGClassSecResData, getSecResData       */
    {NULL},           				    /* fast subclass         */
    GetValuesPrehook,				    /* getValuesPrehook      */
    GetValuesPosthook,				    /* getValuesPosthook      */
};

/* ext rec static initialization */
XmCascadeButtonGCacheObjClassRec xmCascadeButtonGCacheObjClassRec =
{
  {
      /* superclass         */    (WidgetClass) &xmLabelGCacheObjClassRec,
      /* class_name         */    "XmCascadeButtonGadget",
      /* widget_size        */    sizeof(XmCascadeButtonGCacheObjRec),
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
      /* synthetic resources */   NULL,
      /* num_syn_resources   */   0,
      /* extension           */   NULL,
   }
};


/*
 * static initialization of the cascade button widget class record, 
 * must do each field
 */

externaldef(xmcascadebuttongadgetclassrec) XmCascadeButtonGadgetClassRec xmCascadeButtonGadgetClassRec = 
{
    {
	(WidgetClass) &xmLabelGadgetClassRec,	/* superclass ptr	*/
	"XmCascadeButtonGadget",		/* class_name	*/
	sizeof (XmCascadeButtonGadgetRec),	/* size of Pulldown widget */
	ClassInitialize,			/* class init proc */
	ClassPartInitialize,			/* chained class init */
	FALSE,					/* class is not init'ed */
	Initialize,				/* widget init proc */
	NULL,					/* init_hook proc */
    	NULL,					/* widget realize proc */
    	NULL,					/* class action table */
	0,
	resources,				/* this class's resource list */
	XtNumber (resources),			/*  "	  " resource_count */
    	NULLQUARK,				/* xrm_class	        */
    	TRUE,					/* do compress motion */
    	XtExposeCompressMaximal,		/* do compress exposure */
	TRUE,					/* don't compress enter-leave */
   	FALSE,					/* no VisibilityNotify */
	Destroy,				/* class destroy proc */
	Resize,					/* class resize proc */
	Redisplay,				/* expose proc */
	SetValues,				/* set_value proc */
	NULL,					/* set_value_hook proc */
	XtInheritSetValuesAlmost,		/* set_value_almost proc */
	NULL,					/* get_values_hook */
	NULL,					/* class accept focus proc */
	XtVersion,				/* current version */
    	NULL,					/* callback offset list */
    	NULL,					/* default translation table */
	XtInheritQueryGeometry,			/* query geo proc */
	NULL,				        /* display accelerator*/
	(XtPointer)&CascadeBGClassExtensionRec,	/* extension */
    },
    {
			/* Gadget Class record */
	BorderHighlight,			/* border_highlight */
	BorderUnhighlight,			/* border_uhighlight */
	ArmAndActivate,				/* arm & activate */
	InputDispatch,				/* input dispatch routine */
	NULL,					/* visual change routine */
	NULL,					/* syn resources */
	0,					/* num syn_resources */
	&CascadeButtonClassCachePart,		/* class cache part   */
	NULL					/* extension */
    },
    {			/* Label Class record */
	_XtInherit,				/* set override callback */
	(XmMenuProc)_XtInherit,			/* menu procedures       */
	NULL,					/* extension */
    },
    {			/* cascade_button class record */
        NULL,  				        /* extension          */
    }
};

/*
 * now make a public symbol that points to this class record
 */

externaldef(xmcascadebuttongadgetclass) WidgetClass xmCascadeButtonGadgetClass = 
                          (WidgetClass) &xmCascadeButtonGadgetClassRec;




static void ClassInitialize()
{
  CascadeBGClassExtensionRec.record_type = XmQmotif;
}

/*
 * set up fast subclassing
 */
static void ClassPartInitialize (wc)
WidgetClass wc;
{
   _XmFastSubclassInit (wc, XmCASCADE_BUTTON_GADGET_BIT);
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

   CBG_Cache(new) =
     &(((XmCascadeButtonGCacheObject)extData->widget)->cascade_button_cache);
   CBG_Cache(req) =
     &(((XmCascadeButtonGCacheObject)extData->reqWidget)->cascade_button_cache);

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
    XmCascadeButtonGadget  cbw = (XmCascadeButtonGadget)new;
    XmCascadeButtonGadget  rw =  (XmCascadeButtonGadget)req;

    /*
    * - register parts in cache.
    * - update cache pointers
    * - and free req
    */

    LabG_Cache(cbw) = (XmLabelGCacheObjPart *)
      _XmCachePart( LabG_ClassCachePart(cbw),
                    (caddr_t)LabG_Cache(cbw),
                    sizeof(XmLabelGCacheObjPart));

    CBG_Cache(cbw) = (XmCascadeButtonGCacheObjPart *)
           _XmCachePart( CBG_ClassCachePart(cbw),
                         (caddr_t)CBG_Cache(cbw),
                         sizeof(XmCascadeButtonGCacheObjPart));

   /*
    * might want to break up into per-class work that gets explicitly
    * chained. For right now, each class has to replicate all
    * superclass logic in hook routine
    */

   /*
    * free the req subobject used for comparisons
    */
    ext = _XmGetWidgetExtData((Widget)cbw, XmCACHE_EXTENSION);
    XtFree(ext->reqWidget);
    XtDestroyWidget(ext->widget);
    /* extData gets freed at destroy */
}

/*******************************************************************
 *
 *  _XmCascadeBCacheCompare
 *
 *******************************************************************/

static int _XmCascadeBCacheCompare (cascadeB_inst, cascadeB_cache_inst)
XmCascadeButtonGCacheObjPart *cascadeB_inst, *cascadeB_cache_inst;
{
   if((cascadeB_inst->cascade_pixmap == cascadeB_cache_inst->cascade_pixmap) &&
      (cascadeB_inst->map_delay == cascadeB_cache_inst->map_delay) &&
      (cascadeB_inst->armed_pixmap == cascadeB_cache_inst->armed_pixmap)) 
        return 1;
   else
        return 0;

}



/*
 * Border highlighting is only allowed for option menus.  Otherwise
 * the button is armed (does not pop up submenus).
 */
static void BorderHighlight (cb)
XmCascadeButtonGadget cb;
{
   if (LabG_MenuType(cb) == XmMENU_OPTION)
      (* ((XmGadgetClass) xmGadgetClass)->
             gadget_class.border_highlight) (cb);

   else
      Arm (cb);
}



/*
 * Border unhighlighting only done in option menus.  Otherwise the button
 * is disarmed (does not pop down submenus).
 */
static void BorderUnhighlight (cb)
XmCascadeButtonGadget cb;
{
   if (LabG_MenuType(cb) == XmMENU_OPTION)
      (* ((XmGadgetClass) xmGadgetClass)->
             gadget_class.border_unhighlight) (cb);

   else
      Disarm (cb, False);
}


/*
 * Draw the 3D shadow around the widget if its in an option menu or if the
 * widget is armed.
 */
static void DrawShadow (cb)
XmCascadeButtonGadget cb;
{
   if (cb->gadget.shadow_thickness > 0 &&
      (CBG_Armed(cb) ||
      (LabG_MenuType(cb) == XmMENU_OPTION)))
   {
      if (XtIsRealized(XtParent(cb)))
      {
	 _XmDrawShadow (XtDisplay (cb), XtWindow (cb),
			XmParentTopShadowGC(cb),
			XmParentBottomShadowGC(cb),
			cb->gadget.shadow_thickness,
			cb->gadget.highlight_thickness + cb->rectangle.x,
			cb->gadget.highlight_thickness + cb->rectangle.y,
			 cb->rectangle.width - 2 * 
			cb->gadget.highlight_thickness,
			 cb->rectangle.height - 2 * 
			cb->gadget.highlight_thickness);
      }
   }
}

static void DrawCascade(cb)
register XmCascadeButtonGadget cb;
{
   if ((CBG_HasCascade(cb)) && (CBG_Cascade_width(cb) != 0))
   {
      /* draw the casacade */
      if ((LabG_MenuType(cb) == XmMENU_OPTION) &&
	  (CBG_CascadePixmap(cb) == XmUNSPECIFIED_PIXMAP))
      {
	 Dimension height, width;
	 Dimension offset_y;

	 switch(CBG_Cascade_height(cb) - 2*G_ShadowThickness(cb)) {
	    case 5:
	    case 6:
	       height = 1;
	       width =  CBG_Cascade_width(cb) - 3;
	       break;
	    case 7:
	    case 8:
	    case 9:
	       height = 2;
	       width =  CBG_Cascade_width(cb) - 4;
	       break;
	    case 10:
	    case 11:
	    case 12:
	    case 13:
	       height = 3;
	       width =  CBG_Cascade_width(cb) - 5;
	       break;
	    default:
	       height = 4;
	       width =  CBG_Cascade_width(cb) - 6;
	       break;
	 }
	 width -= 2*G_ShadowThickness(cb);
	 offset_y =  (CBG_Cascade_height(cb)- height)/2;

	 XFillRectangle(XtDisplay(cb), XtWindow(XtParent(cb)),
	    XmParentBackgroundGC(cb),
	    cb->rectangle.x + CBG_Cascade_x(cb) + G_ShadowThickness(cb),
	    cb->rectangle.y + CBG_Cascade_y(cb) + offset_y,
	    width, height);
	    
	 _XmDrawShadow(XtDisplay(cb), XtWindow(XtParent(cb)),
	    XmParentTopShadowGC(cb), XmParentBottomShadowGC(cb),
	    G_ShadowThickness(cb),
	    cb->rectangle.x + CBG_Cascade_x(cb),
	    cb->rectangle.y + CBG_Cascade_y(cb) + offset_y -
	       G_ShadowThickness(cb),
	    width + (2* G_ShadowThickness(cb)),
	    height +  (2* G_ShadowThickness(cb)));
      }
      else
	 XCopyArea (XtDisplay(cb), 
	    CBG_Armed(cb) && (CBG_ArmedPixmap(cb) != XmUNSPECIFIED_PIXMAP) ?
	       CBG_ArmedPixmap(cb) : CBG_CascadePixmap(cb), 
	    XtWindow(cb),
	    LabG_NormalGC(cb), 0, 0, 
	    CBG_Cascade_width(cb), CBG_Cascade_height(cb),
	    cb->rectangle.x + CBG_Cascade_x(cb), 
	    cb->rectangle.y + CBG_Cascade_y(cb));
   }
}

/*
 * redisplay the widget
 */
static void Redisplay (cb)
XmCascadeButtonGadget cb;
{
    if (XtIsRealized (cb))
    {
       if ((LabG_MenuType(cb) == XmMENU_POPUP) ||
	   (LabG_MenuType(cb) == XmMENU_PULLDOWN))
       {
	  XmMenuShellWidget mshell = (XmMenuShellWidget)XtParent(XtParent(cb));

	  if (!mshell->shell.popped_up)
	      return;
       }

       /*
	* This might be necessary in an option menu to keep the glyph from
	* moving when it's items vary in size.
	*/
       if (LabG_MenuType(cb) == XmMENU_OPTION)
          position_cascade(cb);

        /* Label class does most of the work */
	(* xmLabelGadgetClassRec.rect_class.expose)(cb, NULL, NULL);

	DrawCascade(cb);
	DrawShadow (cb);
    }
}


/*
 * Input sent by a manger and dispatched here.  The gadget handles Arm,
 * Activate, Enter, Leave, FocusIn, FocusOut and Help events.
 */
static void InputDispatch (cb, event, event_mask)
XmCascadeButtonGadget cb;
XEvent * event;
Mask event_mask;
{
   if (event_mask & XmARM_EVENT)
   {
      if (LabG_MenuType(cb) == XmMENU_OPTION)
         ArmAndPost (cb, event);

      else if (LabG_MenuType(cb) == XmMENU_BAR)
	  MenuBarSelect (cb, event);
      
      else 
         StartDrag (cb, event);
   }

   else if (event_mask & XmACTIVATE_EVENT)
   {
      if ((LabG_MenuType(cb) == XmMENU_PULLDOWN) ||
          (LabG_MenuType(cb) == XmMENU_POPUP) ||
	  (LabG_MenuType(cb) == XmMENU_BAR))
      {
          if (event->type == ButtonRelease)
             DoSelect (cb, event);

          else if (event->type == KeyPress)
             KeySelect (cb, event);
      }
      else
          /* option menu */
          ArmAndPost (cb, event);
   }

   else if (event_mask & XmENTER_EVENT)
   {
      if (LabG_MenuType(cb) == XmMENU_BAR)
	  MenuBarEnter (cb, event);

      else if (LabG_MenuType(cb) != XmMENU_OPTION)
	  DelayedArm (cb, event);
   }

   else if (event_mask & XmLEAVE_EVENT)
   {
      if (LabG_MenuType(cb) == XmMENU_BAR)
	  MenuBarLeave (cb);
      else
	  CheckDisarm (cb, event);
   }

   else if (event_mask & XmFOCUS_IN_EVENT)
      BorderHighlight (cb);

   else if (event_mask & XmFOCUS_OUT_EVENT)
   {
      if (((LabG_MenuType(cb) == XmMENU_POPUP) || 
           (LabG_MenuType(cb) == XmMENU_PULLDOWN)) &&
          ((XmManagerWidget)XtParent(cb))->manager.active_child == (Widget)cb)
       return;

       BorderUnhighlight (cb); 
    } 
    else if (event_mask & XmHELP_EVENT)
       _XmCBHelp((Widget)cb, event); 
}


/*
 * Arming the CascadeButtonGadget consists of setting the armed bit
 * and drawing the 3D shadow.  CascadeButtonGadgets in
 * option menus are never armed since they will never get the event
 * to cause it to unarm.
 */
static void Arm (cb)
XmCascadeButtonGadget cb;
{
   if ((LabG_MenuType(cb) != XmMENU_OPTION) &&
       (! CBG_Armed(cb)))
   {
      CBG_Armed(cb) = TRUE;
      DrawCascade(cb);
      DrawShadow (cb);
   }
   if (LabG_MenuType (cb) != XmMENU_BAR)
      XmProcessTraversal((Widget)cb, XmTRAVERSE_CURRENT);
}



/*
 * Post any submenus and then arm the gadget.  The order is important for
 * performance.
 */
static void ArmAndPost (cb, event)
XmCascadeButtonGadget cb;
XEvent	*event;
{
   XmRowColumnWidget parent = (XmRowColumnWidget) XtParent(cb);

   if (!CBG_Armed(cb))
   {
      _XmCascadingPopup ((Widget)cb, event, TRUE);
      Arm(cb);
   }
}

/*
 * class function to cause the cascade button to be armed and selected
 */
static void ArmAndActivate (cb, event)
XmCascadeButtonGadget cb;
XEvent * event;
{
   XmAnyCallbackStruct cback;
   XmRowColumnWidget parent = (XmRowColumnWidget) XtParent(cb);

   switch (LabG_MenuType(cb))
   {
    case XmMENU_OPTION:
    {
       ArmAndPost (cb, event);
       if (CBG_Submenu(cb))
       {
	  (* xmLabelGadgetClassRec.label_class.menuProcs) (XmMENU_TRAVERSAL,
							   (Widget)parent, TRUE,
							   NULL, NULL);
       }
       break;
    }

    case XmMENU_PULLDOWN:
    case XmMENU_POPUP:
    {
       Select (cb, event, TRUE);
       if (CBG_Submenu(cb))
       {
          (* xmLabelGadgetClassRec.label_class.menuProcs) (XmMENU_TRAVERSAL,
							   (Widget)parent, TRUE,
							   NULL, NULL);
       }
       break;
    }

    case XmMENU_BAR:
    {
       ShellWidget myShell = NULL;

       /* Shared menupanes require some additional checks */
       if (CBG_Submenu(cb))
	   myShell = (ShellWidget)XtParent(CBG_Submenu(cb));

       if (myShell && (myShell->shell.popped_up) &&
	   (myShell->composite.children[0] == CBG_Submenu(cb)))
       {
	  (* xmLabelGadgetClassRec.label_class.menuProcs)
	      (XmMENU_POPDOWN, (Widget)parent, NULL, (caddr_t)event, NULL);

	  Disarm (cb, FALSE);
       }

       else 
       {
	  /* call the cascading callbacks first thing */
	  cback.reason = XmCR_CASCADING;
	  cback.event = event;
	  XtCallCallbackList ((Widget) cb, CBG_CascadeCall(cb), &cback);

	  if (! RC_IsArmed (parent))
	  {
	     _XmMenuFocus((Widget)parent, XmMENU_BEGIN, CurrentTime);

	     if (CBG_Submenu (cb))
		 (* xmLabelGadgetClassRec.label_class.menuProcs)
		     (XmMENU_ARM, (Widget)parent, NULL, NULL, NULL);
	  }

	  else
	      (* xmLabelGadgetClassRec.label_class.menuProcs)
		  (XmMENU_BAR_CLEANUP, (Widget)parent, NULL, NULL, NULL);

	  /* do the select without calling the cascading callbacks again */
	  Select (cb, event, FALSE);

	  if (CBG_Submenu(cb))
	  {
	     (* xmLabelGadgetClassRec.label_class.menuProcs) (XmMENU_TRAVERSAL,
							      (Widget)parent, TRUE,
							      NULL, NULL);
	  }

	  else
	  {
	     (* xmLabelGadgetClassRec.label_class.menuProcs)
		 (XmMENU_TRAVERSAL, (Widget)parent, FALSE, NULL, NULL);
	     (* xmLabelGadgetClassRec.label_class.menuProcs)
		 (XmMENU_DISARM, (Widget)parent, NULL, NULL, NULL);

	     _XmSetTransientFlag((Widget)cb, False);
	     _XmMenuFocus(XtParent(cb), XmMENU_END, CurrentTime);
	     XtUngrabPointer(XtParent(cb), CurrentTime);
	  }
	     /*
 	      * The previous call to Select() may set the active_child.  So
 	      * we'll just force it in case there is no submenu (PM mode).
 	      */
 	     parent->manager.active_child = 
	       CBG_Submenu(cb) ? NULL : (Widget)cb;
       }
       
       break;
    }
 }   
}


/*
 * Disarm the menu.  This may include popping down any submenu that is up
 * and removing the timeout to post a submenu.
 */
static void Disarm (cb, unpost)
XmCascadeButtonGadget cb;
Boolean unpost;
{
   if (CBG_Armed(cb))
   {
      CBG_Armed(cb) = FALSE;

      /* popdown any posted submenus */
      if (unpost)
         _XmButtonPopdownChildren ((XmRowColumnWidget)XtParent(cb));

      /* if a delayed arm is pending, remove it */
      if (CBG_Timer(cb))
      {
         XtRemoveTimeOut (CBG_Timer(cb));
         CBG_Timer(cb) = 0; 
      }

      /* if the shadow is drawn and the menupane is not going down, erase it */
      if ((cb->gadget.shadow_thickness != 0) &&
	  (! RC_PoppingDown(XtParent(cb))))
      {
	 if (XtIsRealized(XtParent(cb)))
	 {
	    _XmEraseShadow (XtDisplay (cb), XtWindow (cb),
			    cb->gadget.shadow_thickness,
			    cb->gadget.highlight_thickness + cb->rectangle.x,
			    cb->gadget.highlight_thickness + cb->rectangle.y,
			    cb->rectangle.width - 2 * 
			     cb->gadget.highlight_thickness,
			    cb->rectangle.height - 2 * 
			     cb->gadget.highlight_thickness);
	 }
      }
      DrawCascade(cb);
   }
}


/*
 * called when the post delay timeout occurs.
 */
static XtTimerCallbackProc PostTimeout (cb)
XmCascadeButtonGadget cb;
{
   if (CBG_Timer(cb))
   {
      CBG_Timer(cb) = 0;
    
      _XmCascadingPopup ((Widget)cb, NULL, TRUE);

   }
}



/*
 * set the timer to post the submenu if a leave event does
 * not occur first.
 */
static void DelayedArm (cb, event)
XmCascadeButtonGadget cb;
XEvent	*event;
{
   if ((! CBG_Armed(cb)) &&
       (((XmMenuShellWidget) XtParent(XtParent(cb)))->shell.popped_up) &&
       (! ((XmRowColumnWidget) XtParent(cb))->manager.traversal_on))

   {
      if (CBG_MapDelay(cb) <= 0)
      {
	 /* don't delay, just do it */
	 ArmAndPost (cb, event);
      }
      else
      {
         CBG_Timer(cb) = 
               XtAppAddTimeOut(XtWidgetToApplicationContext(cb), 
			       (unsigned long) CBG_MapDelay(cb),
			       (XtTimerCallbackProc) PostTimeout, (caddr_t)cb);
         Arm(cb);
      }
   }
}


/*
 * if traversal is not on and the mouse
 * has not entered its cascading submenu, disarm the
 * CascadeButtonGadget.
 */
static void CheckDisarm (cb, event)
XmCascadeButtonGadget cb;
XEvent	*event;
{
   XmMenuShellWidget submenushell;
   XMotionEvent * entEvent = (XMotionEvent *) event;

   if (! ((XmRowColumnWidget) XtParent(cb))->manager.traversal_on)
   {
      if ((CBG_Armed(cb)) && 
          (CBG_Submenu(cb)))
      {
         submenushell = (XmMenuShellWidget) XtParent (CBG_Submenu(cb));
   
         if (submenushell->shell.popped_up)
         {
            if ((entEvent->x_root >= submenushell->core.x) &&
                (entEvent->x_root <  submenushell->core.x + 
                                     submenushell->core.width +
                                     (submenushell->core.border_width << 1)) &&
                (entEvent->y_root >= submenushell->core.y) &&
                (entEvent->y_root <  submenushell->core.y + 
                                     submenushell->core.height +
	   			     (submenushell->core.border_width << 1)))

  	        /* then we are in the cascading submenu, don't disarm */
 	        return;
         }
      }
      Disarm (cb, TRUE);
   }
}


/*
 * post submenu and disable traversal.  These functions must be called
 * in this order.
 */
static void StartDrag (cb, event)
XmCascadeButtonGadget cb;
XEvent	*event;
{
   XButtonEvent * btnEvent = (XButtonEvent *)  event;
   XmMenuShellWidget mshell = (XmMenuShellWidget) XtParent(XtParent(cb));

   if (((LabG_MenuType(cb) == XmMENU_PULLDOWN) ||
	(LabG_MenuType(cb) == XmMENU_POPUP)) &&
       ! mshell->shell.popped_up)
   {
      return;
   }
   
   _XmCascadingPopup ((Widget)cb, event, TRUE);
   Arm (cb);
   
   (* xmLabelGadgetClassRec.label_class.menuProcs) (XmMENU_TRAVERSAL,
						    XtParent(cb), FALSE, 
						    NULL, NULL);
   
   /* record event so MenuShell does not process it */
   _XmRecordEvent (event);
}


/*
 * do the popup and if there is not a submenu, bring down the menu system.
 */
static void Select (cb, event, doCascade)
XmCascadeButtonGadget cb;
XEvent	*event;
Boolean doCascade;
{
   XmAnyCallbackStruct cback;

   _XmCascadingPopup ((Widget)cb, event, doCascade);

   /*
    * check if there is a submenu here in case this changed during 
    * the cascadeing callbacks
    */
   if (CBG_Submenu(cb) == NULL)
   {
      (* xmLabelGadgetClassRec.label_class.menuProcs)
	  (XmMENU_POPDOWN, XtParent(cb), NULL, (caddr_t)event, NULL);

      Disarm (cb, FALSE);
      
      cback.event = event;
      cback.reason = XmCR_ACTIVATE;
      
      if (XmIsRowColumn(XtParent(cb)))
      {
	 (* xmLabelGadgetClassRec.label_class.menuProcs) (XmMENU_CALLBACK, 
							  XtParent(cb),
							  FALSE, (caddr_t)cb,
							  (caddr_t)&cback);
      }

      if ((! LabG_SkipCallback(cb)) &&
	  (CBG_ActivateCall(cb)))
      {
	 XtCallCallbackList ((Widget) cb, CBG_ActivateCall(cb), &cback);
      }
   }
   else 
   { 
      Arm (cb); 
   }
}



/*
 * if there is a submenu, enable traversal.
 * call select to do the work
 */
static void DoSelect (cb, event)
XmCascadeButtonGadget cb;
XEvent	*event;
{
   XButtonEvent * btnEvent = (XButtonEvent *)  event;

   if ((LabG_MenuType(cb) == XmMENU_BAR) &&
       ! RC_IsArmed(XtParent(cb)))
       return;
       
   /*
    * make sure the shell is popped up, this takes care of a corner case
    * that can occur with rapid pressing of the mouse button
    */
   if (((LabG_MenuType(cb) == XmMENU_PULLDOWN) ||
	(LabG_MenuType(cb) == XmMENU_POPUP)) &&
       (!((XmMenuShellWidget) XtParent(XtParent(cb)))->shell.popped_up))
   {
      return;
   }

   Select (cb, event, (Boolean) CBG_Submenu(cb) != 0);

   /*
    * don't let the menu shell widget process this event
    */
   _XmRecordEvent (event);
   
   if (CBG_Submenu(cb))
       (* xmLabelGadgetClassRec.label_class.menuProcs) (XmMENU_TRAVERSAL,
							XtParent(cb), TRUE,
							NULL, NULL);
   else
   {
      (* xmLabelGadgetClassRec.label_class.menuProcs)
	  (XmMENU_DISARM, XtParent(cb), NULL, NULL, NULL);

      if (LabG_MenuType(cb) == XmMENU_BAR)
      {
	 _XmSetTransientFlag((Widget)cb, False);
	 _XmMenuFocus(XtParent(cb), XmMENU_END, CurrentTime);
	 XtUngrabPointer(XtParent(cb), CurrentTime);
      }
   }
}


/*
 * if the menu system traversal is enabled, do a select
 */
static void KeySelect (cb, event)
XmCascadeButtonGadget cb;
XEvent	*event;
{
   XmRowColumnWidget parent = (XmRowColumnWidget) XtParent(cb);

   /* check if event has been processed */
   if (!_XmIsEventUnique(event))
      return;

   if (parent->manager.traversal_on)
   {
      if (CBG_Submenu (cb) && (LabG_MenuType(cb) == XmMENU_OPTION))
      {
         XmManagerWidget pane = (XmManagerWidget) CBG_Submenu(cb);
         pane->manager.traversal_on = TRUE;
      }

      if (LabG_MenuType(cb) == XmMENU_BAR)
	  (* xmLabelGadgetClassRec.label_class.menuProcs)
	      (XmMENU_BAR_CLEANUP, (Widget)parent, NULL, NULL, NULL);
	  
      Select(cb, event, TRUE);

      if (CBG_Submenu(cb)) 
      {
	 (* xmLabelGadgetClassRec.label_class.menuProcs)
	     (XmMENU_TRAVERSAL, CBG_Submenu(cb), TRUE, NULL, NULL);
      }
   }

   /* record so that menuShell does not process this event */
   _XmRecordEvent(event);
}


/*
 * If the menu system is not active, arm it and arm this cascadebutton
 * else start the drag mode
 */
static void MenuBarSelect (cb, event)
XmCascadeButtonWidget cb;
XEvent	*event;
{
   Boolean validButton;
   XButtonEvent * btnEvent = (XButtonEvent *) event;

   if (RC_IsArmed ((XmRowColumnWidget) XtParent(cb)))
   {
      /* Cleanup the PM menubar mode, if enabled */
      (* xmLabelGadgetClassRec.label_class.menuProcs)
	  (XmMENU_BAR_CLEANUP, XtParent(cb), NULL, NULL, NULL);

      if (!CBG_Submenu(cb))
      {
	 _XmMenuFocus(XtParent(XtParent(cb)), XmMENU_MIDDLE, CurrentTime);
      }

      StartDrag (cb, event);
   }

   else
   {
      (* xmLabelGadgetClassRec.label_class.menuProcs) (XmMENU_BUTTON,
						       XtParent(cb), NULL,
						       (caddr_t)event, &validButton);
   
      if (validButton)
      {
	 (* xmLabelGadgetClassRec.label_class.menuProcs)
	     (XmMENU_ARM, XtParent(cb), NULL, NULL, NULL);
         
         _XmMenuFocus(XtParent(cb), XmMENU_BEGIN, CurrentTime);

	 ArmAndPost (cb, event);
	 
	 if (!CBG_Submenu(cb))
	 {  
	    /*
	     * since no submenu is posted, check if the grab has occured
	     * and if not, do the pointer grab now.
	     */
	    if (RC_BeingArmed(XtParent(cb)))
	    {
	       Cursor cursor;

	       XFindContext(XtDisplay(cb), NULL, _XmMenuCursorContext,
			    (caddr_t*)&cursor);
	       XtGrabPointer(XtParent(cb), True,
			     EVENTS, GrabModeAsync, GrabModeAsync, None,
			     cursor, CurrentTime);
	       RC_SetBeingArmed(XtParent(cb), False);
	    }
	 }
	 
	 /* record so that menuShell doesn't process this event */
	 _XmRecordEvent (event);
      }
   }
}


/* 
 * If the menu is active, post submenu and arm.
 */
static void MenuBarEnter (cb, event)
register XmCascadeButtonWidget cb;
XEvent	*event;
{
   if ((RC_IsArmed (XtParent(cb))) &&
       (! CBG_Armed(cb)) && 
       (! ((XmRowColumnWidget) XtParent(cb))->manager.traversal_on))
   {
      if (!CBG_Submenu(cb))
      {
	 _XmMenuFocus(XtParent(cb), XmMENU_MIDDLE, CurrentTime);
      }

      _XmCascadingPopup ((Widget)cb, event, TRUE);
      Arm(cb);
   }
}


/*
 * unless our submenu is posted or traversal is on, disarm
 */
static void MenuBarLeave (cb)
register XmCascadeButtonWidget cb;
{
   XmMenuShellWidget submenuShell;

   if (RC_IsArmed (XtParent (cb)))
   {
      if (CBG_Submenu(cb))
      {
         submenuShell = (XmMenuShellWidget) XtParent(CBG_Submenu(cb));

         if (submenuShell->shell.popped_up)
            return;
      }  
   
      if (! ((XmRowColumnWidget) XtParent(cb))->manager.traversal_on)
         Disarm (cb, TRUE);   
   }
}


/*
 * get the cascade size set up
 */
static int size_cascade (cascadebtn)
XmCascadeButtonGadget cascadebtn;
{
    Window rootwin;
    int x,y;					/* must be int */
    unsigned int width, height, border, depth;  /* must be unsigned int */

    if (CBG_CascadePixmap(cascadebtn) != XmUNSPECIFIED_PIXMAP)
    {
       XGetGeometry(XtDisplay(cascadebtn), CBG_CascadePixmap(cascadebtn),
		    &rootwin, &x, &y, &width, &height,
		    &border, &depth);

       CBG_Cascade_width(cascadebtn) = (Dimension) width;
       CBG_Cascade_height(cascadebtn) = (Dimension) height;
    }
    else
    {
       if (LabG_MenuType(cascadebtn) == XmMENU_OPTION)
       {
	  CBG_Cascade_width(cascadebtn) = 
	     CBG_Cascade_height(cascadebtn) = 
		Max(LabG_TextRect(cascadebtn).height,
		    LabG_AccTextRect(cascadebtn).height) + 
	        2 * cascadebtn->gadget.shadow_thickness;     /* glyph shadow */

       }
       else
       {
	  CBG_Cascade_width(cascadebtn) = 0;
	  CBG_Cascade_height(cascadebtn) = 0;
       }
    }
}


/*
 * set up the cascade position.  
 */

static int position_cascade (cascadebtn)
XmCascadeButtonGadget cascadebtn;
{
   Dimension buffer;

   if (CBG_HasCascade(cascadebtn))
   { 
      CBG_Cascade_x(cascadebtn) = cascadebtn->rectangle.width -
                               cascadebtn->gadget.highlight_thickness -
                               cascadebtn->gadget.shadow_thickness -
			       LabG_MarginWidth(cascadebtn) -
                               CBG_Cascade_width(cascadebtn);

      buffer = cascadebtn->gadget.highlight_thickness +
             cascadebtn->gadget.shadow_thickness +
             LabG_MarginHeight(cascadebtn);

      CBG_Cascade_y(cascadebtn) = buffer +
                               ((cascadebtn->rectangle.height -  2*buffer) -
                                CBG_Cascade_height(cascadebtn)) / 2;
   }
   else
   {
      CBG_Cascade_y(cascadebtn) = 0;
      CBG_Cascade_x(cascadebtn) = 0;
   }
}


/*
 * set up the cascade size and location
 */
static void setup_cascade (cascadebtn, adjustWidth, adjustHeight)
XmCascadeButtonGadget cascadebtn;
Boolean adjustWidth;
Boolean adjustHeight;
{
   Dimension delta;

   if (CBG_HasCascade(cascadebtn))
   {
      /*
       *  modify the size of the CascadeButtonGadget to acommadate the
       *  cascade, if needed.  The cascade should fit inside MarginRight.
       */
      if ((CBG_Cascade_width(cascadebtn) + CASCADE_PIX_SPACE) >
	  LabG_MarginRight(cascadebtn))
      {
	 delta = CBG_Cascade_width(cascadebtn) + CASCADE_PIX_SPACE -
	     LabG_MarginRight(cascadebtn);
	 LabG_MarginRight(cascadebtn) = LabG_MarginRight(cascadebtn) +
	   delta;

	 if (adjustWidth)
	     cascadebtn->rectangle.width += delta;

	 else
	 {
	    if (LabG_Alignment(cascadebtn) == XmALIGNMENT_END)
		LabG_TextRect_x(cascadebtn) -= delta;
	    else if (LabG_Alignment(cascadebtn) == XmALIGNMENT_CENTER)
		LabG_TextRect_x(cascadebtn) -= delta/2;
          
	 }
      }
	
      /*
       * the cascade height should fit inside of 
       * TextRect + marginTop + marginBottom
       */
      delta = CBG_Cascade_height(cascadebtn) +
                   2 * (LabG_MarginHeight(cascadebtn) +
		         cascadebtn->gadget.shadow_thickness +
		         cascadebtn->gadget.highlight_thickness);
      
      if (delta > cascadebtn->rectangle.height)
      {
	 delta -= cascadebtn->rectangle.height;
	 LabG_MarginTop(cascadebtn) = LabG_MarginTop(cascadebtn) + 
	   (delta/2);
	 LabG_TextRect_y(cascadebtn) += delta/2;
	 LabG_MarginBottom(cascadebtn) = LabG_MarginBottom(cascadebtn) + 
	   delta - (delta/2);
	 
	 if (adjustHeight)
	     cascadebtn->rectangle.height += delta;
      }
   }

   position_cascade(cascadebtn);
}


/*
 * Destroy the widget
 */

static void Destroy (cb)
XmCascadeButtonGadget cb;
{
    XmRowColumnWidget submenu = (XmRowColumnWidget) CBG_Submenu(cb);

    /*
     * break the submenu link
     */
    if (submenu != NULL)
	(* xmLabelGadgetClassRec.label_class.menuProcs) (XmMENU_SUBMENU,
							 (Widget)submenu, FALSE, (caddr_t)cb,
							 NULL);

    if (CBG_Timer(cb))
         XtRemoveTimeOut (CBG_Timer(cb));
    
    XtRemoveAllCallbacks (cb, XmNactivateCallback);
    XtRemoveAllCallbacks (cb, XmNcascadingCallback);
}
                         

/*
 * routine to resize a cascade button, called by the parent
 * geometery manager
 */

static void Resize (cb)
XmCascadeButtonGadget cb;
{
   if (cb)
   {
      /* Label class does it's work */
      (* xmLabelGadgetClassRec.rect_class.resize) (cb);

      /* move the cascade too */
      position_cascade (cb);
   }
}


/*
 * Attach the submenu to the cascadebtn.  If it is already attached to
 * something, first break that link
 */
static void AttachSubmenu (cb)
XmCascadeButtonGadget cb;
{
   XmRowColumnWidget submenu = (XmRowColumnWidget) CBG_Submenu(cb);
   Arg args[1];

   /* if its already attached somewhere, break that link */
   if (RC_CascadeBtn(submenu))
   {
      XtSetArg (args[0], XmNsubMenuId, NULL);
      XtSetValues (RC_CascadeBtn(submenu), args, 1);
   }
       
   (* xmLabelGadgetClassRec.label_class.menuProcs) (XmMENU_SUBMENU,
						    CBG_Submenu(cb), TRUE,
						    (caddr_t)cb, NULL);
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
    XmCascadeButtonGCacheObject    new;

    cePtr = _XmGetBaseClassExtPtr(XtClass(newParent), XmQmotif);
    ec = (*cePtr)->secondaryObjectClass;

    /* allocate copies and fill from cache */
     new = (XmCascadeButtonGCacheObject) XtMalloc(ec->core_class.widget_size);
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

     bcopy((char *)CBG_Cache(newParent),
           (char *)&(new->cascade_button_cache),
           sizeof(XmCascadeButtonGCacheObjPart));

     extData = (XmWidgetExtData) XtCalloc(sizeof(XmWidgetExtDataRec), 1);
     _XmPushWidgetExtData(newParent, extData, XmCACHE_EXTENSION);

     XtSetValues(new, args, *num_args);

     LabG_Cache(newParent) = &(((XmLabelGCacheObject)new)->label_cache);
     LabG_Cache(refParent) = &(((XmLabelGCacheObject)extData->reqWidget)->label_cache);

     /*CBG_Cache(oldParent) = CBG_Cache(newParent);*/
     CBG_Cache(newParent) =
         &(((XmCascadeButtonGCacheObject)new)->cascade_button_cache);
     CBG_Cache(refParent) =
         &(((XmCascadeButtonGCacheObject)extData->reqWidget)->cascade_button_cache);

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
    XmCascadeButtonGCacheObject    new;

    cePtr = _XmGetBaseClassExtPtr(XtClass(newParent), XmQmotif);
    ec = (*cePtr)->secondaryObjectClass;

    /* allocate copies and fill from cache */
     new = (XmCascadeButtonGCacheObject) XtMalloc(ec->core_class.widget_size);
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

     bcopy((char *)CBG_Cache(newParent),
           (char *)&(new->cascade_button_cache),
           sizeof(XmCascadeButtonGCacheObjPart));

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
/* ARGSUSED */
static Boolean SetValuesPosthook(current, req, new, args, num_args)
   Widget      current, req, new;
   ArgList     args;
   Cardinal    *num_args;
{
   XmWidgetExtData                  ext;
   XmCascadeButtonGCacheObjPart        *oldCachePtr, *newCachePtr;

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
                     _XmCachePart(LabG_ClassCachePart(new),
                                 (caddr_t)LabG_Cache(new),
                                 sizeof(XmLabelGCacheObjPart));
  }
  else
       LabG_Cache(new) = LabG_Cache(current);

  /* assign if changed! */
  if (!_XmCascadeBCacheCompare(CBG_Cache(new),
                              CBG_Cache(current)))
  {
      _XmCacheDelete((caddr_t)CBG_Cache(current));  /* delete the old one */
      CBG_Cache(new) = (XmCascadeButtonGCacheObjPart *)
                     _XmCachePart(CBG_ClassCachePart(new),
                                 (caddr_t)CBG_Cache(new),
                                 sizeof(XmCascadeButtonGCacheObjPart));
  }
  else
       CBG_Cache(new) = CBG_Cache(current);

  _XmPopWidgetExtData(new, &ext, XmCACHE_EXTENSION);

  XtFree(ext->widget);
  XtFree(ext->reqWidget);
  XtFree(ext->oldWidget);
  XtFree(ext);

  return FALSE;
}



/*
 * Set Values
 */

static Boolean SetValues (old, requested, new)
XmCascadeButtonGadget  old, requested, new;
{
    XmRowColumnWidget parent = (XmRowColumnWidget) XtParent (new);
    Boolean flag = FALSE;
    Boolean adjustWidth = FALSE;
    Boolean adjustHeight = FALSE;

    if ((CBG_Submenu(new)) &&
	((! XmIsRowColumn(CBG_Submenu(new))) ||
	 (RC_Type(CBG_Submenu(new)) != XmMENU_PULLDOWN)))
    {
       CBG_Submenu(new) = NULL;
       _XmWarning (new, WRONGSUBMENU);
    }

    if (CBG_MapDelay(new) < 0) 
    {
       CBG_MapDelay(new) = CBG_MapDelay(old);
       _XmWarning (new, WRONGMAPDELAY);
    }

    /* if there is a change to submenu, notify menu system */
    if (CBG_Submenu(old) != CBG_Submenu(new))
    {
       if (CBG_Submenu(old))
	   (* xmLabelGadgetClassRec.label_class.menuProcs) (XmMENU_SUBMENU,
							    CBG_Submenu(old),
							    FALSE, (caddr_t)old, NULL);

       if (CBG_Submenu(new))
	   AttachSubmenu (new);
    }

    if (LabG_MenuType(new) == XmMENU_BAR)
	new->gadget.traversal_on = TRUE;
    
    /* handle the cascade pixmap indicator */
    else if (LabG_MenuType(new) == XmMENU_PULLDOWN ||
	     LabG_MenuType(new) == XmMENU_POPUP ||
	     LabG_MenuType(new) == XmMENU_OPTION)
    {
       /* don't let traversal change */
       if (LabG_MenuType(new) != XmMENU_OPTION)
          new->gadget.traversal_on = TRUE;

       if (LabG_RecomputeSize(new) || (requested->rectangle.width <= 0))
	  adjustWidth = TRUE;
       
       if (LabG_RecomputeSize(new) || (requested->rectangle.height <= 0))
	  adjustHeight = TRUE;

       /* get new pixmap size */
       if (CBG_CascadePixmap (old) != CBG_CascadePixmap (new))
       {
          if (CBG_ArmedPixmap(old) != XmUNSPECIFIED_PIXMAP)
          {
             _XmArrowPixmapCacheDelete(CBG_ArmedPixmap(old));
             _XmArrowPixmapCacheDelete(CBG_CascadePixmap(old));
          }
          CBG_ArmedPixmap(new) = XmUNSPECIFIED_PIXMAP;
	  size_cascade (new);
       } else
          if ( ((CBG_CascadePixmap(new) ==  XmUNSPECIFIED_PIXMAP) &&
		  (!CBG_Submenu(old) && CBG_Submenu(new))) ||
	       ((CBG_ArmedPixmap(old) != XmUNSPECIFIED_PIXMAP) &&
	          (LabG_TextRect_height(old) != LabG_TextRect_height(new))) )
          {
                _XmArrowPixmapCacheDelete(CBG_ArmedPixmap(old));
                _XmArrowPixmapCacheDelete(CBG_CascadePixmap(old));
                CBG_ArmedPixmap(new) = XmUNSPECIFIED_PIXMAP;
                CBG_CascadePixmap(new) = XmUNSPECIFIED_PIXMAP;
                _XmCreateArrowPixmaps(new);
                size_cascade (new);
          }
	  
       /*
        * resize gadget if cascade appeared or disappeared, or if the
	* cascade pixmap changed size.
	*/
       if ((CBG_CascadePixmap (old) != CBG_CascadePixmap (new))  ||
	    (CBG_Submenu(old) != CBG_Submenu(new)))
       {
	  setup_cascade (new, adjustWidth, adjustHeight);

	  /* if there wasn't a cascade, and still isn't, don't redraw */
	  if (CBG_Submenu(old) || CBG_Submenu(new))
	      flag = TRUE;
       }

       /* make sure that other changes did not scrunch our pixmap */
       else if (CBG_Submenu(new))
       {
	  if ((new->gadget.highlight_thickness !=
	       old->gadget.highlight_thickness)                     ||
	      (new->gadget.shadow_thickness !=
	       old->gadget.shadow_thickness)                        ||
	      (LabG_MarginRight (new) != LabG_MarginRight (old))    ||
	      (LabG_MarginHeight (new) != LabG_MarginHeight (old))  ||
	      (LabG_MarginTop (new) != LabG_MarginTop (old))	    ||
	      (LabG_MarginBottom (new) != LabG_MarginBottom (old)))
	  {

             setup_cascade (new, adjustWidth, adjustHeight);
	     flag = TRUE;
	  }

	  else if ((LabG_MarginWidth(new) != LabG_MarginWidth(old)) ||
		   (new->rectangle.width != old->rectangle.width)   ||
		   (new->rectangle.height != old->rectangle.height))
	      
	  {
	     position_cascade (new);
	     flag = TRUE;
	  }
       }
    }
    
    /* don't allow this to change */
    new->gadget.event_mask = XmARM_EVENT | XmACTIVATE_EVENT | 
                             XmFOCUS_IN_EVENT | XmFOCUS_OUT_EVENT |
                             XmENTER_EVENT | XmLEAVE_EVENT | XmHELP_EVENT;

    return (flag);
}


/*
 * Initialize
 */
static void Initialize (req, new, args, num_args)
XmCascadeButtonGadget  req;
XmCascadeButtonGadget  new;
ArgList args;
Cardinal * num_args;
{
   Boolean adjustWidth = FALSE;
   Boolean adjustHeight = FALSE;


   XmRowColumnWidget    submenu = (XmRowColumnWidget) CBG_Submenu (new);
   XmRowColumnWidget    parent = (XmRowColumnWidget) XtParent(new);

   if (! (LabG_MenuType(new) == XmMENU_BAR ||
	  LabG_MenuType(new) == XmMENU_PULLDOWN ||
	  LabG_MenuType(new) == XmMENU_POPUP    ||
	  LabG_MenuType(new) == XmMENU_OPTION))
   {
      _XmWarning (new, WRONGPARENT);
   }

   /* if menuProcs is not set up yet, try again */
   if (xmLabelGadgetClassRec.label_class.menuProcs == NULL)
       xmLabelGadgetClassRec.label_class.menuProcs =
	   (XmMenuProc) _XmGetMenuProcContext();

   CBG_ArmedPixmap(new) =  XmUNSPECIFIED_PIXMAP;

   /*
    * if the user did not specify a margin width, and we are
    * in a menuBar, set up the default.  First, find out what was the
    * request value (not in request since this is in the cached data)
    */
   if (LabG_MenuType(new) == XmMENU_BAR)
   {
      Dimension requestedMarginWidth;
      XtResource request_resources;

      request_resources.resource_name = XmNmarginWidth;
      request_resources.resource_class = XmCMarginWidth;
      request_resources.resource_type = XmRHorizontalDimension;
      request_resources.resource_size = sizeof (Dimension);
      request_resources.default_type = XmRImmediate;
      request_resources.resource_offset = 0;
      request_resources.default_addr = (caddr_t) XmINVALID_DIMENSION;
   
      XtGetSubresources(XtParent(new), &requestedMarginWidth, XtName(new),
			new->object.widget_class->core_class.class_name,
			&request_resources, 1, args, *num_args);

      if (requestedMarginWidth == XmINVALID_DIMENSION)
      {
	 LabG_MarginWidth(new) = 6;
      }
   }

   /* for other menu types, we may need to initialize the cascade pixmap */
   else
   if ((LabG_MenuType(new) != XmMENU_OPTION))
   {
      if (submenu && CBG_CascadePixmap(new) == XmUNSPECIFIED_PIXMAP)
	 _XmCreateArrowPixmaps(new);
   }

   CBG_Armed(new) = FALSE;
   CBG_Timer(new) = 0;

   if ((submenu) &&
       ((! XmIsRowColumn(submenu)) ||
	(RC_Type(submenu) != XmMENU_PULLDOWN)))
   {
      submenu = NULL;
      _XmWarning (new, WRONGSUBMENU);
   }

   if (CBG_MapDelay(new) < 0) 
   {
      CBG_MapDelay(new) = MAP_DELAY_DEFAULT;
      _XmWarning (new, WRONGMAPDELAY);
   }
       
   /* call submenu's class function to set the link  */
   if (submenu != NULL)
       AttachSubmenu (new);

   if (LabG_MenuType(new) == XmMENU_PULLDOWN	||
       LabG_MenuType(new) == XmMENU_POPUP ||
       LabG_MenuType(new) == XmMENU_OPTION)
   {
      if (req->rectangle.width <= 0)
	  adjustWidth = TRUE;
      
      if (req->rectangle.height <= 0)
	  adjustHeight = TRUE;
      
      /* get pixmap size and set up gadget to allow room for it */
      size_cascade (new);
      setup_cascade (new, adjustWidth, adjustHeight);
   }

   if (LabG_MenuType(new) == XmMENU_BAR ||
       LabG_MenuType(new) == XmMENU_PULLDOWN	||
       LabG_MenuType(new) == XmMENU_POPUP)
   {
      new->gadget.traversal_on = TRUE;
   }
	

   /* 
    * initialize the input types 
    */
   new->gadget.event_mask = XmARM_EVENT | XmACTIVATE_EVENT | 
       XmFOCUS_IN_EVENT | XmFOCUS_OUT_EVENT |
	   XmENTER_EVENT | XmLEAVE_EVENT | XmHELP_EVENT;

}


/*
 *************************************************************************
 *
 * Public Routines                                                        
 *
 *************************************************************************
 */

#ifdef _NO_PROTO
Widget XmCreateCascadeButtonGadget (parent, name, al, ac)
Widget   parent;
char    *name;
ArgList  al;
Cardinal ac;
#else /* _NO_PROTO */
Widget XmCreateCascadeButtonGadget (Widget parent, char *name, ArgList al, Cardinal ac)
#endif /* _NO_PROTO */
{
    Widget cb;
 
    cb = XtCreateWidget(name, xmCascadeButtonGadgetClass, parent, al, ac);

    return (cb);
}


/*
 * Arm or disarm the gadget.  This routine does not pop up or down submenus
 */
#ifdef _NO_PROTO
void XmCascadeButtonGadgetHighlight (cb, highlight)
Widget cb;
Boolean highlight;
#else /* _NO_PROTO */
void XmCascadeButtonGadgetHighlight (Widget cb,  
#if NeedWidePrototypes
int highlight
#else
Boolean highlight
#endif 
)
#endif /* _NO_PROTO */
{
   if ((cb) && XmIsCascadeButtonGadget(cb))
   {
      if (highlight)
         Arm (cb);

      else
         Disarm (cb, FALSE);
   }
}

/*
 * GetCascadeBGClassResBase ()
 *   retrun the address of the base of resources.
 */
static XtPointer
GetCascadeBGClassSecResBase ( widget, client_data)
Widget  widget;
XtPointer client_data;

{   XtPointer  widgetSecdataPtr;


    widgetSecdataPtr = (XtPointer) CBG_Cache(widget);

    return(widgetSecdataPtr);
}

