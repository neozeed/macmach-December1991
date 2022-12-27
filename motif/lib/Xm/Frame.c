#ifdef REV_INFO
#ifndef lint
static char SCCSID[] = "OSF/Motif: @(#)Frame.c	3.14 91/01/10";
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
#include <X11/IntrinsicP.h>
#include <Xm/XmP.h>
#include <Xm/FrameP.h>

#define MESSAGE1 "The shadow type is incorrect."
#define MESSAGE2 "Only one child should be inserted in a frame."
#define MESSAGE3 "Invalid margin width."
#define MESSAGE4 "Invalid margin height."
      
/* Event procedures referenced in the actions list */

static void Arm();
static void Activate();
static void Help();

/* Default translation table and action list */

#ifndef MCCABE
static char defaultTranslations[] =
       "<EnterWindow>:		Enter() \n\
	<FocusIn>:		FocusIn() \n\
	<Btn1Down>:		Arm()\n\
	<Btn1Up>:		Activate()";
#else
static char defaultTranslations[];
#endif

static XtActionsRec actionsList[] =
{
	{ "Enter",	(XtActionProc)	_XmManagerEnter },
	{ "FocusIn",	(XtActionProc)	_XmManagerFocusIn },
	{ "Arm",	(XtActionProc)	Arm },
	{ "Activate",	(XtActionProc)	Activate },
	{ "Help",	(XtActionProc)	Help },
};


/* Definition for resources that need special processing in get values */

static XmSyntheticResource syn_resources[] = 
{
   {  XmNmarginWidth, 
      sizeof (Dimension),
      XtOffset (XmFrameWidget, frame.margin_width),
      _XmFromHorizontalPixels,
      _XmToHorizontalPixels },

   {  XmNmarginHeight, 
      sizeof (Dimension),
      XtOffset (XmFrameWidget, frame.margin_height),
      _XmFromVerticalPixels,
      _XmToVerticalPixels, }
};


/*  Resource definitions for Subclasses of Manager */

static XtResource resources[] =
{
   {  XmNmarginWidth, 
      XmCMarginWidth, 
      XmRHorizontalDimension, 
      sizeof (Dimension),
      XtOffset (XmFrameWidget, frame.margin_width),
      XmRImmediate, (caddr_t) 0 },

   {  XmNmarginHeight, 
      XmCMarginHeight, 
      XmRVerticalDimension,
      sizeof (Dimension),
      XtOffset (XmFrameWidget, frame.margin_height),
      XmRImmediate, (caddr_t) 0 },

   {  XmNshadowType, XmCShadowType, XmRShadowType, sizeof (unsigned char),
      XtOffset (XmFrameWidget, frame.shadow_type),
      XmRImmediate, (caddr_t) XmINVALID_DIMENSION }
};


static void    Initialize();
static void    ClassPartInitialize();
static void    Redisplay();
static void    Resize();
static void    InsertChild();
static Boolean SetValues();

static void             ChangeManaged();
static XtGeometryResult GeometryManager();

static XtGeometryResult XmQueryGeometry();

static void XmCalcMgrSize();
static void XmCalcChildSize();


/****************************************************************
 *
 * Full class record constant
 *
 ****************************************************************/

externaldef(xmframeclassrec) XmFrameClassRec xmFrameClassRec = 
{
   {			/* core_class fields      */
      (WidgetClass) &xmManagerClassRec,		/* superclass         */
      "XmFrame",				/* class_name         */
      sizeof(XmFrameRec),			/* widget_size        */
      NULL,					/* class_initialize   */
      ClassPartInitialize,			/* class_part_init    */
      FALSE,					/* class_inited       */
      (XtInitProc) Initialize,			/* initialize         */
      NULL,					/* initialize_hook    */
      _XtInherit,				/* realize            */
      NULL,                  			/* actionsList,	actions	*/
      0,             				/* XtNumber(actionsList), num_actions */
      resources,				/* resources          */
      XtNumber(resources),			/* num_resources      */
      NULLQUARK,				/* xrm_class          */
      TRUE,					/* compress_motion    */
      XtExposeCompressMaximal,	/* compress_exposure  */
      TRUE,					/* compress_enterlv   */
      FALSE,					/* visible_interest   */
      NULL,					/* destroy            */
      (XtWidgetProc) Resize,			/* resize             */
      (XtExposeProc) Redisplay,			/* expose             */
      (XtSetValuesFunc) SetValues,		/* set_values         */
      NULL,					/* set_values_hook    */
      XtInheritSetValuesAlmost,			/* set_values_almost  */
      NULL,					/* get_values_hook    */
      NULL,					/* accept_focus       */
      XtVersion,				/* version            */
      NULL,					/* callback_private   */
      XtInheritTranslations,			/* tm_table           */
      (XtGeometryHandler) XmQueryGeometry,	/* query_geometry     */
      NULL,                             /* display_accelerator   */
      NULL,                             /* extension             */
   },

   {		/* composite_class fields */
      (XtGeometryHandler) GeometryManager,    	/* geometry_manager   */
      (XtWidgetProc) ChangeManaged,		/* change_managed     */
      (XtArgsProc) InsertChild,			/* insert_child       */
      XtInheritDeleteChild,			/* delete_child       */
      NULL,                                     /* extension          */
   },

   {		/* constraint_class fields */
      NULL,					/* resource list        */   
      0,					/* num resources        */   
      sizeof (XmFrameConstraintRec),            /* constraint size      */   
      NULL,					/* init proc            */   
      NULL,					/* destroy proc         */   
      NULL,					/* set values proc      */   
      NULL,                                     /* extension            */
   },

   {						/* manager class     */
      XtInheritTranslations,			/* translations      */
      syn_resources,				/* syn resources      	  */
      XtNumber(syn_resources),			/* num syn_resources 	  */
      NULL,					/* get_cont_resources     */
      0,					/* num_get_cont_resources */
      XmInheritParentProcess,                   /* parent_process         */
      NULL,					/* extension         */    
   },

   {						/* frame class       */
      NULL,					/* extension         */    
   }	
};

externaldef(xmframewidgetclass) WidgetClass xmFrameWidgetClass =
			                        (WidgetClass) &xmFrameClassRec;

/************************************************************************
 *
 *  ClassPartInitialize
 *     Set up the fast subclassing for the widget
 *
 ************************************************************************/
static void ClassPartInitialize (wc)
WidgetClass wc;

{
   _XmFastSubclassInit (wc, XmFRAME_BIT);
}

      

/************************************************************************
 *
 *  Initialize
 *	Ensure that the width and height are not 0.
 *
 *************************************<->***********************************/

static void Initialize (request, new)
XmFrameWidget request, new;

{
   if (new->frame.shadow_type == (unsigned char) XmINVALID_DIMENSION)
      if (XtIsShell (XtParent(new)))
         new->frame.shadow_type = XmSHADOW_OUT;
      else
         new->frame.shadow_type = XmSHADOW_ETCHED_IN;

   if (request->manager.shadow_thickness == 0)
      if (XtIsShell (XtParent(new)))
         new->manager.shadow_thickness = 1;
      else
         new->manager.shadow_thickness = 2;

   if (request -> core.width == 0) 
      new -> core.width += 10 + 2 * new->frame.margin_width;
   else
      new->core.width += 2 * new->frame.margin_width;

   if (request -> core.height == 0) 
      new -> core.height += 10 + 2 * new->frame.margin_height;
   else
      new->core.height += 2 * new->frame.margin_height;

   new -> frame.old_width =  new -> core.width;
   new -> frame.old_height = new -> core.height;
   new -> frame.old_shadow_thickness = new -> manager.shadow_thickness;

   if (new -> frame.shadow_type != XmSHADOW_IN   &&
       new -> frame.shadow_type != XmSHADOW_OUT &&
       new -> frame.shadow_type != XmSHADOW_ETCHED_IN &&
       new -> frame.shadow_type != XmSHADOW_ETCHED_OUT)
   {
      _XmWarning (new, MESSAGE1);
      new -> frame.shadow_type = XmSHADOW_ETCHED_IN;
   }
}




/************************************************************************
 *
 *  InsertChild
 *	Insert the child into the frames children list.  If more
 *	than one child has been inserted, give a warning message.
 *
 ************************************************************************/

static void InsertChild (w)
Widget	  w;

{
   XmManagerWidgetClass	superclass;
   XmFrameWidget	fw;

   if (!XtIsRectObj(w))
	return;

   superclass = (XmManagerWidgetClass) xmManagerWidgetClass;
   (*superclass->composite_class.insert_child)(w);

   fw = (XmFrameWidget) w -> core.parent;

   if (fw -> composite.num_children > 1)
      _XmWarning (w, MESSAGE2);
}




/************************************************************************
 *
 *  Resize 
 *  	Calculate the size of the one child and resize it.
 *
 ************************************************************************/

static void Resize (fw)
XmFrameWidget fw;

{
   Widget child;
   Position childX, childY;
   Dimension childWidth, childHeight;
   int shadowThickness;
   

   _XmClearShadowType ((Widget )fw, 
	fw->frame.old_width, fw->frame.old_height,
	fw->frame.old_shadow_thickness,
	0);

   fw->frame.old_width = fw->core.width;
   fw->frame.old_height = fw->core.height;
   fw->frame.old_shadow_thickness = fw->manager.shadow_thickness;
      
   if (fw -> composite.num_children != 0)
   {
      child = fw -> composite.children[0];
      childX =  child -> core.x;
      childY =  child -> core.y;
      childWidth =  child -> core.width;
      childHeight =  child -> core.height;

      shadowThickness = fw -> manager.shadow_thickness;

      XmCalcChildSize (fw, &childWidth, &childHeight, child->core.border_width,
			   fw -> core.width, fw -> core.height);

      if ((childX != shadowThickness + fw->frame.margin_width || 
	   childY != shadowThickness + fw->frame.margin_height) &&
	  (childWidth != child->core.width || 
	   childHeight != child->core.height))
      {
         _XmConfigureObject((RectObj)child, 
	    shadowThickness + fw->frame.margin_width, 
	    shadowThickness + fw->frame.margin_height, 
	    childWidth,
	    childHeight,
	    child->core.border_width);
      }
      else if (childX != shadowThickness + fw->frame.margin_width || 
	       childY != shadowThickness + fw->frame.margin_height )
      {
	 _XmMoveObject ((RectObj)child, 
	    shadowThickness + fw->frame.margin_width, 
	    shadowThickness + fw->frame.margin_height);
      }
      else if (childWidth!=child->core.width || 
	       childHeight!=child->core.height )
      {
	 _XmResizeObject ((RectObj)child,
	    childWidth,
	    childHeight,
            child->core.border_width);
      }
   }
   if (XtIsRealized(fw))
      _XmDrawShadowType((Widget)fw,fw->frame.shadow_type,
			fw -> core.width,
			fw -> core.height,			   
			fw -> manager.shadow_thickness,
			0,
			fw -> manager.top_shadow_GC,
			fw -> manager.bottom_shadow_GC);
}




/************************************************************************
 *
 *  Redisplay
 *     General redisplay function called on exposure events.
 *
 ************************************************************************/

static void Redisplay (fw, event, region)
XmFrameWidget fw;
XEvent * event;
Region region;

{
   _XmRedisplayGadgets ((CompositeWidget)fw, (XExposeEvent*)event, region);
   
   _XmDrawShadowType((Widget)fw,fw->frame.shadow_type,
			fw -> core.width,
			fw -> core.height,			   
			fw -> manager.shadow_thickness,
			0,
			fw -> manager.top_shadow_GC,
			fw -> manager.bottom_shadow_GC);
}



/************************************************************************
 *
 *  Geometry Manager
 *	Take the requested geometry, calculate the needed size for
 *	the frame and make a request to the frames parent.
 *      Requests to change x, y position are always denied.
 *
 ************************************************************************/

static XtGeometryResult GeometryManager (w, request, reply)
Widget		   w;
XtWidgetGeometry * request;
XtWidgetGeometry * reply;	/* RETURN */

{
   XmFrameWidget fw = (XmFrameWidget) w -> core.parent;
   Dimension width, height, borderWidth;
   Dimension fwWidth, fwHeight;
   Dimension childWidth, childHeight;
   Dimension replyWidth, replyHeight;
   Boolean almostFlag = False;


   /*  Set up the calculation variables according to the  */
   /*  contents of the requested geometry.                */

   if ((request -> request_mode & CWX) || (request -> request_mode & CWY))
      almostFlag = True;

   if (request -> request_mode & CWWidth) width = request -> width;
   else width = w -> core.width;

   if (request -> request_mode & CWHeight) height = request -> height;
   else height = w -> core.height;

   if (request -> request_mode & CWBorderWidth)
       borderWidth = request -> border_width;
   else borderWidth = w -> core.border_width;


   /*  Calculate the required manager size and only allow  */
   /*  changes of width and height.                        */

   XmCalcMgrSize (fw, width, height,
                      borderWidth, &fwWidth, &fwHeight);

   switch (XtMakeResizeRequest ((Widget) fw, fwWidth, fwHeight,
                                &replyWidth, &replyHeight))
   {
      case XtGeometryYes:
         if (!almostFlag)
         {

            XmCalcChildSize (fw, &childWidth, &childHeight,
                    borderWidth, fw->core.width, fw->core.height);

            _XmResizeObject ((RectObj)w, childWidth, childHeight,
                 	 borderWidth);

            _XmClearShadowType ((Widget )fw,
	       fw->frame.old_width, fw->frame.old_height,
               fw->frame.old_shadow_thickness,
               0);

               fw->frame.old_width = fw->core.width;
               fw->frame.old_height = fw->core.height;
               fw->frame.old_shadow_thickness = fw->manager.shadow_thickness;

            return (XtGeometryDone);
         }
      break;

      case XtGeometryNo:
         return (XtGeometryNo);
      break;

      case XtGeometryAlmost:
      break;
   }


   /*  Fallen through to an almost condition.  Clear the x and y  */
   /*  and set the width, height, and border.                     */


   if (reply != NULL)
   {
      XmCalcChildSize (fw, &childWidth, &childHeight,
                        borderWidth, replyWidth, replyHeight);

      reply -> request_mode = request -> request_mode & ~(CWX | CWY);
      reply -> width = childWidth;
      reply -> height = childHeight;
      reply -> border_width = borderWidth;
      if (request -> request_mode & CWSibling)
               reply -> sibling = request -> sibling;
      if (request -> request_mode & CWStackMode)
               reply -> stack_mode = request -> stack_mode;
      return (XtGeometryAlmost);
   }

   return (XtGeometryNo);
}





/************************************************************************
 *
 *  ChangeManaged
 *	Process a changed in managed state of the child.  If its
 *	size is out of sync with the frame, make a resize request
 *	to change the size of the frame.
 *
 *
 ************************************************************************/

static void ChangeManaged (fw)
XmFrameWidget fw;

{
   Widget child;
   Dimension fwWidth, fwHeight;
   Dimension replyWidth, replyHeight;
   Position childX, childY;
   Dimension childWidth, childHeight;
   int shadowThickness;

   if (fw -> composite.num_children == 0)
      return;


   child = fw -> composite.children[0];
   childX =  child -> core.x;
   childY =  child -> core.y;
   childWidth = child -> core.width;
   childHeight = child -> core.height;

   shadowThickness = fw -> manager.shadow_thickness;

   XmCalcMgrSize (fw, childWidth, childHeight,
                      child -> core.border_width, &fwWidth, &fwHeight);

   if (fwWidth != fw -> core.width || fwHeight != fw -> core.height)
   {
      switch (XtMakeResizeRequest ((Widget) fw, fwWidth, fwHeight,
				                &replyWidth, &replyHeight))
      {
         case XtGeometryYes:
            _XmClearShadowType ((Widget )fw, 
	       fw->frame.old_width, fw->frame.old_height,
               fw->frame.old_shadow_thickness,
               0);

            fw->frame.old_width = fw->core.width;
            fw->frame.old_height = fw->core.height;
            fw->frame.old_shadow_thickness = fw->manager.shadow_thickness;

            XmCalcChildSize (fw, &childWidth, &childHeight,
                  child -> core.border_width, fw->core.width, fw->core.height);
         break;

         case XtGeometryNo:
            XmCalcChildSize (fw, &childWidth, &childHeight,
                  child -> core.border_width, fw->core.width, fw->core.height);
         break;

         case XtGeometryAlmost:
            XtMakeResizeRequest (fw, replyWidth, replyHeight, NULL, NULL);
            XmCalcChildSize (fw, &childWidth, &childHeight,
                  child->core.border_width,replyWidth,replyHeight);
         break;
      }
   }

   if ((childX != shadowThickness + fw->frame.margin_width || 
        childY != shadowThickness + fw->frame.margin_height) &&
       (childWidth != child->core.width || 
        childHeight != child->core.height))
   {
      _XmConfigureObject((RectObj)child, 
         shadowThickness + fw->frame.margin_width, 
         shadowThickness + fw->frame.margin_height, 
         childWidth,
         childHeight,
	 child->core.border_width);
   }
   else if (childX != shadowThickness + fw->frame.margin_width || 
            childY != shadowThickness + fw->frame.margin_height )
   {
      _XmMoveObject ((RectObj)child, 
         shadowThickness + fw->frame.margin_width, 
         shadowThickness + fw->frame.margin_height);
   }
   else if (childWidth!=child->core.width || 
            childHeight!=child->core.height )
   {
      _XmResizeObject ((RectObj)child,
         childWidth,
         childHeight,
	 child->core.border_width);
   }
    _XmNavigChangeManaged((Widget)fw);
}




/************************************************************************
 *
 *  Set Values
 *	Adjust the size of the manager based on shadow thickness
 *	changes.
 *
 ************************************************************************/

static Boolean SetValues (current, request, new)
XmFrameWidget current, request, new;

{
    Boolean redisplay = False;

   /* convert from unit values to pixel values */

   if (new->frame.margin_width != current->frame.margin_width ||
       new->frame.margin_height != current->frame.margin_height ||
       new->manager.shadow_thickness != current->manager.shadow_thickness)
   {
      new->core.width += 2 * ((new -> manager.shadow_thickness -
                               current -> manager.shadow_thickness) +
                              (new -> frame.margin_width -
                               current -> frame.margin_width));
      new->core.height += 2 * ((new -> manager.shadow_thickness -
                                current -> manager.shadow_thickness) +
                               (new -> frame.margin_height -
                                current -> frame.margin_height));
   }

   if (new -> frame.shadow_type != XmSHADOW_IN   &&
       new -> frame.shadow_type != XmSHADOW_OUT &&
       new -> frame.shadow_type != XmSHADOW_ETCHED_IN &&
       new -> frame.shadow_type != XmSHADOW_ETCHED_OUT)
   {
      _XmWarning (new, MESSAGE1);
      new -> frame.shadow_type = current -> frame.shadow_type;
   }

   if (new -> frame.shadow_type != current -> frame.shadow_type ||
       new->frame.margin_width != current->frame.margin_width ||
       new->frame.margin_height != current->frame.margin_height ||
       new->manager.shadow_thickness != current->manager.shadow_thickness)
   {
      redisplay = True;
   }
   return (redisplay);
}




/************************************************************************
 *
 *  QueryGeometry
 *  	If the frame has a child which has a query proc, pass through
 *      geometry queries and return the results.  Otherwise, pass back
 *      geometry yes unless the size is less than 2 * highlight thickness.
 *
 ************************************************************************/

static XtGeometryResult XmQueryGeometry (fw, request, ret)
XmFrameWidget fw;
XtWidgetGeometry * request;
XtWidgetGeometry * ret;

{
   XtWidgetGeometry newRequest;
   XtGeometryResult result = XtGeometryYes;
   int shadowThickness;
   int increase_width, increase_height;

   ret -> request_mode = 0;
   ret -> x = request->x;
   ret -> y = request->y;
   ret -> width = request->width;
   ret -> height = request->height;

   newRequest.request_mode = 0;

   shadowThickness = fw -> manager.shadow_thickness;


   increase_width = 2 * (shadowThickness +
                         fw -> frame.margin_width +
			 fw -> core.border_width);

   increase_height = 2 * (shadowThickness +
                         fw -> frame.margin_height +
			 fw -> core.border_width);

   /*  See if there is a child and call its query geometry routine.  */

   if (fw -> composite.num_children != 0)
   {
      if (request -> request_mode & CWWidth)
      {
         newRequest.request_mode |= CWWidth;
         newRequest.width = request -> width - increase_width;
      }

      if (request -> request_mode & CWHeight)
      {
         newRequest.request_mode |= CWHeight;
         newRequest.height = request -> height - increase_height;
      }

      result = XtQueryGeometry (fw->composite.children[0], &newRequest, ret);

      if (ret -> request_mode & CWWidth)
         ret -> width += increase_width;
      else
         ret->width = fw->composite.children[0]->core.width + increase_width;

      ret->request_mode |= CWWidth;

      if (ret -> request_mode & CWHeight)
         ret -> height += increase_height;
      else
         ret->height = fw->composite.children[0]->core.height + increase_height;

      ret->request_mode |= CWHeight;

      if ((ret -> width != request -> width) ||
	  (ret -> height != request -> height))
         result = XtGeometryAlmost;
   }
   else
   {
      if (!increase_width)
        increase_width = 10;

      if (!increase_height)
        increase_height = 10;

      if (request -> request_mode & CWWidth && 
          request -> width < increase_width)
      {
         ret -> request_mode |= CWWidth;
         ret -> width = increase_width;
         result = XtGeometryAlmost;
      }

      if (request -> request_mode & CWHeight && 
          request -> height < increase_height)
      {
         ret -> request_mode |= CWHeight;
         ret -> height = increase_height;
         result = XtGeometryAlmost;
      }
   }      

   return (result);
}



/************************************************************************
 *
 *  XmCalcMgrSize
 *	Calculate the manager size based on the supplied width 
 *	and height.
 *
 ************************************************************************/

static void XmCalcMgrSize (fw, childWidth, childHeight, childBorder,
                           fwWidth, fwHeight)
XmFrameWidget fw;
Dimension childWidth;
Dimension childHeight;
Dimension childBorder;
Dimension * fwWidth;
Dimension * fwHeight;

{
   *fwWidth = 
      childWidth + 2 * (childBorder + 
                        fw -> manager.shadow_thickness +
                        fw -> frame.margin_width);
   *fwHeight = 
      childHeight + 2 * (childBorder +
                         fw -> manager.shadow_thickness +
                         fw -> frame.margin_height);
}




/************************************************************************
 *
 *  XmCalcChildSize
 *	Calculate the child size based on the supplied width 
 *	and height.
 *
 ************************************************************************/

static void XmCalcChildSize (fw, childWidth, childHeight, childBorder,
                             fwWidth, fwHeight)
XmFrameWidget fw;
Dimension * childWidth;
Dimension * childHeight;
Dimension childBorder;
Dimension fwWidth;
Dimension fwHeight;

{
   int temp;

   temp = (int) fwWidth - 
          (int) (2 * (childBorder +
                      fw->manager.shadow_thickness +
		      fw->frame.margin_width));

   if (temp <= 0) *childWidth = 1;
   else *childWidth = (Dimension) temp;

   temp = (int) fwHeight - 
          (int) (2 * (childBorder +
                      fw->manager.shadow_thickness +
		      fw->frame.margin_height));

   if (temp <= 0) *childHeight = 1;
   else *childHeight = (Dimension) temp;
}


/************************************************************************
 *
 *  Arm
 *      This function processes selection actions occuring on the
 *      row column manager that may need to be sent to a gadget.
 *
 ************************************************************************/

static void Arm (fw, event)
XmFrameWidget fw;
XButtonPressedEvent * event;

{
   XmGadget gadget;

   if ((gadget =  _XmInputInGadget((CompositeWidget)fw, event->x, event->y)) != NULL)
   {
      _XmDispatchGadgetInput ((XmGadget)gadget, (XEvent*)event, XmARM_EVENT);
      fw->manager.selected_gadget = gadget;
   }
}




/************************************************************************
 *
 *  Activate
 *      This function processes release actions occuring on the
 *      row column manager that may need to be sent to a gadget.
 *
 ************************************************************************/
static void Activate (fw, event)
XmFrameWidget fw;
XButtonPressedEvent * event;

{
   if (fw->manager.selected_gadget != NULL)
   {
      _XmDispatchGadgetInput ((XmGadget)fw->manager.selected_gadget, (XEvent*)event, 
           XmACTIVATE_EVENT);
      fw->manager.selected_gadget = NULL;
   }
}



/************************************************************************
 *
 *  Help
 *
 ************************************************************************/

static void Help (fw, event)
XmFrameWidget fw;
XEvent *event;

{
   XmGadget gadget;
   XKeyEvent *keyevent = (XKeyEvent *) event;
   XmAnyCallbackStruct call_value;

   if ((gadget = _XmInputInGadget ((CompositeWidget)fw, keyevent->x, keyevent->y)) != NULL)
   {
      _XmDispatchGadgetInput (gadget, event, XmHELP_EVENT);
   }
   else
   {	/* Invoke the help system. */
     _XmManagerHelp((XmManagerWidget)fw, event);
   }
}



/************************************************************************
 *
 *  XmCreateFrame
 *	Create an instance of a frame widget and return the widget id.
 *
 ************************************************************************/

#ifdef _NO_PROTO
Widget XmCreateFrame (parent, name, arglist, argcount)
Widget parent;
char * name;
ArgList arglist;
Cardinal argcount;

#else /* _NO_PROTO */
Widget XmCreateFrame (Widget parent, char *name, ArgList arglist, Cardinal argcount)
#endif /* _NO_PROTO */
{
   return (XtCreateWidget (name, xmFrameWidgetClass, 
                           parent, arglist, argcount));
}

