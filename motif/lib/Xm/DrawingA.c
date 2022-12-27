#ifdef REV_INFO
#ifndef lint
static char SCCSID[] = "OSF/Motif: @(#)DrawingA.c	3.19 91/01/10";
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

#include <Xm/DrawingAP.h>


#define GeoMode(r)   ((r)->request_mode)

#define	MARGIN_DEFAULT		10

#define WARN_MARGIN		"Margin width or height cannot be negative."
#define WARN_RESIZE_POLICY	"Incorrect resize policy."


/* Internal procedures:
*/
#ifdef _NO_PROTO

static void             ClassPartInitialize() ;
static void             CalcSize() ;
static Boolean          DoLayout() ;
static void             Initialize() ;
static void             Redisplay() ;
static void             Resize() ;
static XtGeometryResult GeometryManager() ;
static void             ChangeManaged() ;
static Boolean          SetValues() ;
static XtGeometryResult QueryGeometry() ;

#else /* _NO_PROTO */

static void             ClassPartInitialize( WidgetClass class) ;
static void             CalcSize( XmDrawingAreaWidget da,
                             Dimension * replyWidth, Dimension * replyHeight) ;
static Boolean          DoLayout( XmDrawingAreaWidget da) ;
static void             Initialize( XmDrawingAreaWidget request,
                                                     XmDrawingAreaWidget new) ;
static void             Redisplay( XmDrawingAreaWidget da, XEvent * event,
                                                               Region region) ;
static void             Resize( XmDrawingAreaWidget da) ;
static XtGeometryResult GeometryManager( Widget w, XtWidgetGeometry * request,
                                                    XtWidgetGeometry * reply) ;
static void             ChangeManaged( XmDrawingAreaWidget da) ;
static Boolean          SetValues( XmDrawingAreaWidget current,
                                                   XmDrawingAreaWidget request,
                                                     XmDrawingAreaWidget new) ;
static XtGeometryResult QueryGeometry( XmDrawingAreaWidget da,
                                                   XtWidgetGeometry * intended,
                                                  XtWidgetGeometry * desired) ;
#endif /* _NO_PROTO */


/* Default translation table and action list */
#ifndef MCCABE
static char defaultTranslations[] =
"<Key>osfSelect:	DrawingAreaInput() ManagerGadgetSelect()\n\
<Key>osfActivate:	DrawingAreaInput() ManagerGadgetSelect()\n\
<Key>osfHelp:	DrawingAreaInput() ManagerGadgetHelp()\n\
~Shift ~Meta ~Alt <Key>Return:	DrawingAreaInput() ManagerGadgetSelect()\n\
~Shift ~Meta ~Alt <Key>space:	DrawingAreaInput() ManagerGadgetSelect()\n\
<KeyDown>:	DrawingAreaInput() ManagerGadgetKeyInput()\n\
<KeyUp>:	DrawingAreaInput()\n\
<BtnMotion>:	ManagerGadgetButtonMotion()\n\
<Btn1Down>:	DrawingAreaInput() ManagerGadgetArm()\n\
<Btn1Down>,<Btn1Up>:	DrawingAreaInput() ManagerGadgetActivate()\n\
<Btn1Up>:	DrawingAreaInput() ManagerGadgetActivate()\n\
<Btn1Down>(2+):	DrawingAreaInput() ManagerGadgetMultiArm()\n\
<Btn1Up>(2+):	DrawingAreaInput() ManagerGadgetMultiActivate()\n\
<BtnDown>:	DrawingAreaInput()\n\
<BtnUp>:	DrawingAreaInput()";

static char traversalTranslations[] = 
"<Key>osfBeginLine: DrawingAreaInput() ManagerGadgetTraverseHome()\n\
<Key>osfUp:	DrawingAreaInput() ManagerGadgetTraverseUp()\n\
<Key>osfDown:	DrawingAreaInput() ManagerGadgetTraverseDown()\n\
<Key>osfLeft:	DrawingAreaInput() ManagerGadgetTraverseLeft()\n\
<Key>osfRight:	DrawingAreaInput() ManagerGadgetTraverseRight()\n\
Shift ~Meta ~Alt <Key>Tab:	DrawingAreaInput() ManagerGadgetPrevTabGroup()\n\
~Meta ~Alt <Key>Tab:	DrawingAreaInput() ManagerGadgetNextTabGroup()\n\
<EnterWindow>:	ManagerEnter()\n\
<LeaveWindow>:	ManagerLeave()\n\
<FocusOut>:	ManagerFocusOut()\n\
<FocusIn>:	ManagerFocusIn()";

static XtActionsRec actionsList[] =
{
   { "Arm",      (XtActionProc) _XmGadgetArm },         /* Motif 1.0 */
   { "Activate", (XtActionProc) _XmGadgetActivate },    /* Motif 1.0 */
   { "Enter",    (XtActionProc) _XmManagerEnter },      /* Motif 1.0 */
   { "FocusIn",  (XtActionProc) _XmManagerFocusIn },    /* Motif 1.0 */
   { "Help",     (XtActionProc) _XmManagerHelp },       /* Motif 1.0 */
   { "DrawingAreaInput", (XtActionProc) _XmDrawingAreaInput },
};
#else
static char defaultTranslations[];
static char traversalTranslations[];
static XtActionsRec actionsList[];
#endif



/*  Resource definitions for DrawingArea
 */

static XmSyntheticResource syn_resources[] =
{
	{	XmNmarginWidth,
		sizeof (Dimension),
		XtOffset (XmDrawingAreaWidget, drawing_area.margin_width),
		_XmFromHorizontalPixels,
		_XmToHorizontalPixels
	},

	{	XmNmarginHeight,
		sizeof (Dimension),
		XtOffset (XmDrawingAreaWidget, drawing_area.margin_height),
		_XmFromVerticalPixels,
		_XmToVerticalPixels
	},
};


static XtResource resources[] =
{
	{	XmNmarginWidth,
		XmCMarginWidth, XmRHorizontalDimension, sizeof (Dimension),
		XtOffset (XmDrawingAreaWidget, drawing_area.margin_width),
		XmRImmediate, (caddr_t) MARGIN_DEFAULT
	},

	{	XmNmarginHeight,
		XmCMarginHeight, XmRVerticalDimension, sizeof (Dimension),
		XtOffset (XmDrawingAreaWidget, drawing_area.margin_height),
		XmRImmediate, (caddr_t) MARGIN_DEFAULT
	},

	{	XmNresizeCallback,
		XmCCallback, XmRCallback, sizeof (XtCallbackList),
		XtOffset (XmDrawingAreaWidget, drawing_area.resize_callback),
		XmRImmediate, (caddr_t) NULL
	},

	{	XmNexposeCallback,
		XmCCallback, XmRCallback, sizeof (XtCallbackList),
		XtOffset (XmDrawingAreaWidget, drawing_area.expose_callback),
		XmRImmediate, (caddr_t) NULL
	},

	{	XmNinputCallback,
		XmCCallback, XmRCallback, sizeof (XtCallbackList),
		XtOffset (XmDrawingAreaWidget, drawing_area.input_callback),
		XmRImmediate, (caddr_t) NULL
	},

	{	XmNresizePolicy,
		XmCResizePolicy, XmRResizePolicy, sizeof (unsigned char),
		XtOffset (XmDrawingAreaWidget, drawing_area.resize_policy),
		XmRImmediate, (caddr_t) XmRESIZE_ANY
	},

};



/****************************************************************
 *
 * Full class record constant
 *
 ****************************************************************/

externaldef( xmdrawingareaclassrec) XmDrawingAreaClassRec
                                                        xmDrawingAreaClassRec =
{
   {			/* core_class fields      */
      (WidgetClass) &xmManagerClassRec,		/* superclass         */
      "XmDrawingArea",				/* class_name         */
      sizeof(XmDrawingAreaRec),			/* widget_size        */
      NULL,				/* class_initialize   */
      ClassPartInitialize,			/* class_part_init    */
      FALSE,					/* class_inited       */
      (XtInitProc) Initialize,			/* initialize         */
      NULL,					/* initialize_hook    */
      _XtInherit,				/* realize            */
      actionsList,				/* actions	      */
      XtNumber(actionsList),			/* num_actions	      */
      resources,				/* resources          */
      XtNumber(resources),			/* num_resources      */
      NULLQUARK,				/* xrm_class          */
      TRUE,					/* compress_motion    */
      FALSE,					/* compress_exposure  */
      TRUE,					/* compress_enterlv   */
      FALSE,					/* visible_interest   */
      NULL,			                /* destroy            */
      (XtWidgetProc) Resize,			/* resize             */
      (XtExposeProc) Redisplay,			/* expose             */
      (XtSetValuesFunc) SetValues,		/* set_values         */
      NULL,					/* set_values_hook    */
      _XtInherit,				/* set_values_almost  */
      NULL,					/* get_values_hook    */
      NULL,					/* accept_focus       */
      XtVersion,				/* version            */
      NULL,					/* callback_private   */
      defaultTranslations,			/* tm_table           */
      (XtGeometryHandler) QueryGeometry,	/* query_geometry     */
      NULL,             	                /* display_accelerator   */
      NULL,		                        /* extension             */
   },

   {		/* composite_class fields */
      (XtGeometryHandler) GeometryManager,    	/* geometry_manager   */
      (XtWidgetProc) ChangeManaged,		/* change_managed     */
      _XtInherit,				/* insert_child       */
      _XtInherit,				/* delete_child       */
      NULL,                                     /* extension          */
   },

   {		/* constraint_class fields */
      NULL,					/* resource list        */   
      0,					/* num resources        */   
      0,					/* constraint size      */   
      NULL,					/* init proc            */   
      NULL,					/* destroy proc         */   
      NULL,					/* set values proc      */   
      NULL,                                     /* extension            */
   },

   {		/* manager_class fields */
      traversalTranslations,			/* translations           */
      syn_resources,				/* syn_resources      	  */
      XtNumber (syn_resources),			/* num_get_resources 	  */
      NULL,					/* syn_cont_resources     */
      0,					/* num_get_cont_resources */
      XmInheritParentProcess,                   /* parent_process         */
      NULL,					/* extension           */    
   },

   {		/* drawingArea class - none */     
      0						/* mumble */
   }	
};

externaldef( xmdrawingareawidgetclass) WidgetClass xmDrawingAreaWidgetClass
                                       = (WidgetClass) &xmDrawingAreaClassRec ;



/****************************************************************/
static void
#ifdef _NO_PROTO
ClassPartInitialize (class)
            WidgetClass	class;
#else
ClassPartInitialize(
            WidgetClass	class)
#endif
/****************
 * 
 ****************/
{   
/****************/

    _XmFastSubclassInit( class, XmDRAWING_AREA_BIT) ;

    return ;
    }

/****************************************************************/
static void
#ifdef _NO_PROTO
CalcSize(da, replyWidth, replyHeight)
            XmDrawingAreaWidget	da;
            Dimension *     replyWidth ;
            Dimension *     replyHeight ;
#else
CalcSize(
            XmDrawingAreaWidget	da,
            Dimension *     replyWidth,
            Dimension *     replyHeight)
#endif
/****************
 * Figure out how much size we need.
 ****************/
{
    Cardinal  i, numItems;
    Dimension minWidth, minHeight;
    Widget box;   /* Current item */
/****************/
    
  /*
   * If there are no items then any space will work 
   */

   if ((numItems=da->composite.num_children) <= 0)
      {
        if ((replyWidth != NULL) && (replyHeight != NULL))
	{
	   *replyWidth = da->drawing_area.margin_width << 1 ;
	   if (! *replyWidth)
           {   *replyWidth = 10;
               } 
	   *replyHeight = da->drawing_area.margin_height << 1 ;
	   if (! *replyHeight)
           {   *replyHeight = 10;
               } 
	}
	return;
     }

  /*
   * Compute the minimum width & height for this box
   * by summing the rectangles of all children.
   */

   minWidth = minHeight = 0;

   for (i=0; i < numItems; i++)
      {
        box = (Widget) da->composite.children[i];
        if (XtIsManaged (box))
        {
           minWidth = Max (minWidth, (box->core.x + 2*box->core.border_width +
                                      box->core.width));
           minHeight = Max (minHeight, (box->core.y + 2*box->core.border_width +
                                        box->core.height));
        }
     }

   /*
    *  add in margins on right and bottom
    */

   minWidth += da->drawing_area.margin_width;
   minHeight += da->drawing_area.margin_height;
   if (!minWidth)
      minWidth = 10;
   if (!minHeight)
      minHeight = 10;
  
   *replyWidth = minWidth;
   *replyHeight = minHeight;

   return;
}
   
/****************************************************************/
static Boolean
#ifdef _NO_PROTO
DoLayout(da)
            XmDrawingAreaWidget da ;
#else
DoLayout(
            XmDrawingAreaWidget da)
#endif
/****************
 * Calculate the size needed for the DrawingArea.
 * Request a change in size if needed from parent.
 ****************/
{
	Dimension		width, height;
/****************/

	CalcSize(da, &width, &height);

	if (((da->core.width == width) && (da->core.height == height) &&
            da->drawing_area.resize_policy != XmRESIZE_SWINDOW)) 
           	return (True);   /* Same size & NOT special case */

	if ( (da->drawing_area.resize_policy == XmRESIZE_GROW ||
		da->drawing_area.resize_policy == XmRESIZE_NONE) &&
	    ((da->core.width >= width) && (da->core.height >= height)) )
		return (True);

	if ( da->drawing_area.resize_policy == XmRESIZE_NONE )
		return (False);

	switch (XtMakeResizeRequest ((Widget) da, width, height,
			&width, &height))
	{
		case XtGeometryYes:
			return (True);


		case XtGeometryAlmost:  /* TAKE WHAT YOU CAN GET! */
			(void) XtMakeResizeRequest((Widget) da, width, height, 
				NULL, NULL);
			return (True);

		case XtGeometryNo:
		default:
			break ;
	}
        return( False) ;
}

/****************************************************************/
static void
#ifdef _NO_PROTO
Initialize (request, new)
            XmDrawingAreaWidget request ;
            XmDrawingAreaWidget new ;
#else
Initialize(
            XmDrawingAreaWidget request,
            XmDrawingAreaWidget new)
#endif
/****************
 * Ensure that the width and height are not 0.
 ****************/
{
/****************/

	/*	Make sure height and width are not zero.
	*/
	if (new -> core.width == 0)
	{   new -> core.width = new->drawing_area.margin_width << 1 ;
            } 
	if (new -> core.width == 0)
	{   new -> core.width = 10 ;
            } 
	if (new -> core.height == 0)
	{   new -> core.height = new->drawing_area.margin_height << 1 ;
            } 
	if (new -> core.height == 0)
	{   new -> core.height = 10 ;
            } 

	/*	Validate resize policy.
	*/
	if ( request->drawing_area.resize_policy != XmRESIZE_SWINDOW &&
	     request->drawing_area.resize_policy != XmRESIZE_NONE &&
	     request->drawing_area.resize_policy != XmRESIZE_ANY &&
	     request->drawing_area.resize_policy != XmRESIZE_GROW ) 
	{
		_XmWarning (new, WARN_RESIZE_POLICY);		
		new->drawing_area.resize_policy = XmRESIZE_ANY;
	}
        return ;
}

/****************************************************************/
static void
#ifdef _NO_PROTO
Redisplay (da, event, region)
            XmDrawingAreaWidget da;
            XEvent * event;
            Region region;
#else
Redisplay(
            XmDrawingAreaWidget da,
            XEvent * event,
            Region region)
#endif
/****************
 * General redisplay function called on exposure events.
 ****************/
{
   XmDrawingAreaCallbackStruct cb;
/****************/
   
   cb.reason = XmCR_EXPOSE;
   cb.event = event;
   cb.window = XtWindow (da);

   if (da->manager.shadow_thickness)
		_XmDrawShadowType((Widget)da, XmSHADOW_OUT,
			XtWidth(da), XtHeight(da),
			da->manager.shadow_thickness, 0,
			da->manager.top_shadow_GC,
			da->manager.bottom_shadow_GC);

   XtCallCallbackList ((Widget) da, da->drawing_area.expose_callback, &cb);

   _XmRedisplayGadgets ((CompositeWidget)da, (XExposeEvent*)event, region);
   return ;
}
/****************************************************************/
static void
#ifdef _NO_PROTO
Resize (da)
            XmDrawingAreaWidget da;
#else
Resize(
            XmDrawingAreaWidget da)
#endif
/****************
 * Invoke the application resize callbacks.
 ****************/
{
   XmDrawingAreaCallbackStruct cb;
/****************/

   if (da->manager.shadow_thickness)
		_XmDrawShadowType((Widget)da, XmSHADOW_OUT,
			XtWidth(da), XtHeight(da),
			da->manager.shadow_thickness, 0,
			da->manager.top_shadow_GC,
			da->manager.bottom_shadow_GC);

   cb.reason = XmCR_RESIZE;
   cb.event = NULL;
   cb.window = XtWindow (da);

   XtCallCallbackList ((Widget) da, da->drawing_area.resize_callback, &cb);
   return ;
}

/****************************************************************/
void
#ifdef _NO_PROTO
_XmDrawingAreaInput( da, event)
            XmDrawingAreaWidget da ;
            XEvent *  event ;
#else
_XmDrawingAreaInput(
            XmDrawingAreaWidget da,
            XEvent *  event)
#endif
/****************
 * This function processes key and button presses and releases
 *   belonging to the DrawingArea.
 ****************/
{   
            XmDrawingAreaCallbackStruct cb ;
/****************/

    int x, y ;
    Boolean button_event, input_on_gadget, focus_explicit ;

    if ((event->type == ButtonPress) || 
	(event->type == ButtonRelease)) {
	x = event->xbutton.x ;
	y = event->xbutton.y ;
	button_event = True ;
    } else 
    if (event->type == MotionNotify) {
	x = event->xmotion.x ;
	y = event->xmotion.y ;
	button_event = True ;
    } else
    if ((event->type == KeyPress) || 
	(event->type == KeyRelease)) {
	x = event->xkey.x ;
	y = event->xkey.y ;
	button_event = False ;
    }
	
    input_on_gadget = (_XmInputForGadget((CompositeWidget)da, x, y) != NULL);
	    
    focus_explicit = ((_XmGetFocusPolicy((Widget)da) == XmEXPLICIT) &&
		      (da->composite.num_children != 0));

    if (!input_on_gadget) {
	if ((!focus_explicit) || (button_event)) {
	    cb.reason = XmCR_INPUT ;
	    cb.event = event ;
	    cb.window = XtWindow( da) ;
	    XtCallCallbackList ((Widget) da,
 				da->drawing_area.input_callback, &cb) ;

	}
    }

    return ;
}




/****************************************************************/
static XtGeometryResult
#ifdef _NO_PROTO
GeometryManager( w, request, reply)
            Widget		w;
            XtWidgetGeometry	*request;
            XtWidgetGeometry	*reply;
#else
GeometryManager(
            Widget		w,
            XtWidgetGeometry	*request,
            XtWidgetGeometry	*reply)
#endif
/****************
 * 
 ****************/
{
            Dimension	width, height, borderWidth;
            Position    x, y;
            XmDrawingAreaWidget da;
            Boolean geoFlag = False;
            XtGeometryHandler manager;
            XtGeometryResult returnCode = XtGeometryNo;
            XtWidgetGeometry localReply ;
/****************/

    da = (XmDrawingAreaWidget) w->core.parent;

  /*
   * If resize policy is xwresize_swindow then simply pass this request to
   * my parent and return its response to the requesting widget.
   */
    if(    (da->drawing_area.resize_policy == XmRESIZE_SWINDOW)
        && XmIsScrolledWindow( da->core.parent)    )
       {
         manager = ((CompositeWidgetClass)(da->core.parent->core.widget_class))
                                            ->composite_class.geometry_manager;
         returnCode = (*manager)(w, request, reply);
         return (returnCode);
       }

    /* 
     * Since this is a DrawingArea, most position requests are accepted.
     * If requested position is in top or left margin, then return almost.
     */
    /* Fill out entire reply record
    */
    localReply = *request ;
    localReply.request_mode = CWX | CWY | CWWidth | CWHeight | CWBorderWidth ;

    if(    !(request->request_mode & CWX)    )
    {   localReply.x = w->core.x ;
        } 
    if(    !(request->request_mode & CWY)    )
    {   localReply.y = w->core.y ;
        } 
    if(    !(request->request_mode & CWWidth)    )
    {   localReply.width = w->core.width ;
        } 
    if(    !(request->request_mode & CWHeight)    )
    {   localReply.height = w->core.height ;
        } 
    if(    !(request->request_mode & CWBorderWidth)    )
    {   localReply.border_width = w->core.border_width ;
        } 

    /*  if margin != 0, then check for x less than margin width 
    */
    if(    (request->request_mode & CWX)
        && (da->drawing_area.margin_width != 0)
        && (request->x < da->drawing_area.margin_width)    )
    {   
        localReply.x = (Position) da->drawing_area.margin_width ;
        returnCode = XtGeometryAlmost ;
        }
    /*  if margin != 0, then check for y less than margin height  */
    if(    (request->request_mode & CWY)
        && (da->drawing_area.margin_height != 0)
        && (request->y < da->drawing_area.margin_height)    )
    {   
        localReply.y = (Position) da->drawing_area.margin_height ;
        returnCode = XtGeometryAlmost ;
        }
    /* Save current size and set to new size
    */
    x = w->core.x ;
    y = w->core.y ;
    width = w->core.width ;
    height = w->core.height ;
    borderWidth = w->core.border_width ;

    w->core.x = localReply.x ;
    w->core.y = localReply.y ;
    w->core.width = localReply.width ;
    w->core.height = localReply.height ;
    w->core.border_width = localReply.border_width ;

    geoFlag = DoLayout( da) ;
    if(    !geoFlag
        && ((localReply.x + localReply.width + (localReply.border_width << 1))
                         <= (da->core.width - da->drawing_area.margin_width))
        && ((localReply.y + localReply.height + (localReply.border_width << 1))
                  <= (da->core.height - da->drawing_area.margin_height))    )
    {   geoFlag = True ;
        } 
    if(    geoFlag    )
    {   
        if(    returnCode != XtGeometryAlmost    )
        {   returnCode = XtGeometryYes ;
            } 
        }
    else
    {   returnCode = XtGeometryNo ;
        } 

    if(    returnCode == XtGeometryAlmost    )
    {   if(    reply    )
        {   *reply = localReply ;
            } 
        else
        {   returnCode = XtGeometryNo ;
            } 
        }
    if(    returnCode != XtGeometryYes    )
    {   /* Can't oblige, so restore previous values.
        */
        w->core.x = x ;
        w->core.y = y ;
        w->core.width = width ;
        w->core.height = height ;
        w->core.border_width = borderWidth ;
        } 
    return( returnCode) ;
    }

/****************************************************************/
static void
#ifdef _NO_PROTO
ChangeManaged(da)
            XmDrawingAreaWidget da;
#else
ChangeManaged(
            XmDrawingAreaWidget da)
#endif
/****************
 * Re-layout children.
 ****************/
{
    Cardinal  i, numItems;
    Widget box;   /* Current item */
    void (*manager)();
    Position    newx, newy;
    Boolean	do_move;
/****************/

    if (da->drawing_area.resize_policy == XmRESIZE_SWINDOW)
       {
         manager = ((CompositeWidgetClass) (da->core.parent->core.widget_class))
                      ->composite_class.change_managed;
         (*manager)(da->core.parent);
         _XmNavigChangeManaged( (Widget) da) ;
         return;
       }

	/*	If margin != 0, then enforce margins for children
	*/
	numItems = da->composite.num_children;
	for (i=0; i < numItems; i++)
	{
		do_move = False;
		box = (Widget) da->composite.children[i];
		if ( XtIsManaged (box) )
		{
			if ((da->drawing_area.margin_width != 0)
			    && (box->core.x < da->drawing_area.margin_width))
			{
				do_move = True;
				newx = da->drawing_area.margin_width;
			}
			else newx = box->core.x;
			if ((da->drawing_area.margin_height != 0)
			    && (box->core.y < da->drawing_area.margin_height))
			{
				do_move = True;
				newy = da->drawing_area.margin_height;
			}
			else newy = box->core.y;

		}
		if (do_move) _XmMoveObject ((RectObj)box, newx, newy);
	}


    /* Reconfigure the drawingArea */
    
    DoLayout(da);

    _XmNavigChangeManaged( (Widget) da) ;

    return ;
}
/****************************************************************/
static Boolean
#ifdef _NO_PROTO
SetValues (current, request, new)
            XmDrawingAreaWidget current ;
            XmDrawingAreaWidget request ;
            XmDrawingAreaWidget new ;
#else
SetValues(
            XmDrawingAreaWidget current,
            XmDrawingAreaWidget request,
            XmDrawingAreaWidget new)
#endif
/****************
 * 
 ****************/
{
/****************/

	/*	Validate resize policy.
	*/
	if ( request->drawing_area.resize_policy != XmRESIZE_SWINDOW &&
	     request->drawing_area.resize_policy != XmRESIZE_NONE &&
	     request->drawing_area.resize_policy != XmRESIZE_ANY &&
	     request->drawing_area.resize_policy != XmRESIZE_GROW ) 
	{
		_XmWarning (new, WARN_RESIZE_POLICY);		
		new->drawing_area.resize_policy =
			current->drawing_area.resize_policy;
	}
	return( False) ;
}

   
/****************************************************************/
static XtGeometryResult
#ifdef _NO_PROTO
QueryGeometry (da, intended, desired)
            XmDrawingAreaWidget da ;
            XtWidgetGeometry * intended ;
            XtWidgetGeometry * desired ;
#else
QueryGeometry(
            XmDrawingAreaWidget da,
            XtWidgetGeometry * intended,
            XtWidgetGeometry * desired)
#endif
/****************
 * Handle query geometry requests
 ****************/
{
	Dimension	a_w = 0, a_h = 0;
/****************/

	if (GeoMode (intended) & ~ (CWWidth | CWHeight))
		return (XtGeometryYes);

	if ( (GeoMode (intended) & CWWidth) &&
		 (GeoMode (intended) & ~CWHeight))
		a_w = intended->width;

	if ( (GeoMode (intended) & CWHeight) &&
		 (GeoMode (intended) & ~CWWidth))
		a_h = intended->height;

	CalcSize (da, &a_w, &a_h);

	if ( (a_w == XtWidth (da)) && (a_h == XtHeight (da)))
		return (XtGeometryNo);

	GeoMode (desired) = 0;
	desired->width = a_w;
	desired->height = a_h;

	if (!GeoMode (intended))
	{
		GeoMode (desired) |= CWWidth;
		GeoMode (desired) |= CWHeight;
		return (XtGeometryAlmost);
	}
	
	if ( (GeoMode (intended) & (CWWidth | CWHeight))
		&&
		 ( (a_w != intended->width) || (a_h != intended->height)))
	{
		GeoMode (desired) |= CWWidth;
		GeoMode (desired) |= CWHeight;
		return (XtGeometryAlmost);
	}

	if ( (GeoMode (intended) & CWWidth) && 
		 (GeoMode (intended) & ~CWHeight))
	{
		GeoMode (desired) |= CWHeight;
		return (XtGeometryAlmost);
	}

	if ( (GeoMode (intended) & CWHeight) &&
		 (GeoMode (intended) & ~CWWidth))
	{
		GeoMode (desired) |= CWWidth;
		return (XtGeometryAlmost);
	}

	return (XtGeometryYes);
}



/****************************************************************/
Widget
#ifdef _NO_PROTO
XmCreateDrawingArea( p, name, args, n)
            Widget          p;          /*  parent widget	*/
            String          name;	/*  widget name		*/
            ArgList         args;	/*  arg list	*/
            Cardinal        n;          /*  arg count	*/
#else
XmCreateDrawingArea(
            Widget          p,          /*  parent widget	*/
            String          name,	/*  widget name		*/
            ArgList         args,	/*  arg list	*/
            Cardinal        n)          /*  arg count	*/
#endif
/****************
 * This function creates and returns a DrawingArea widget.
 ****************/
{
/****************/

    return( XtCreateWidget( name, xmDrawingAreaWidgetClass, p, args, n)) ;
    }
