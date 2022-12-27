#ifdef REV_INFO
#ifndef lint
static char SCCSID[] = "OSF/Motif: @(#)GadgetUtil.c	3.10 91/01/10";
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




/************************************************************************
 *
 *  XmInputInGadget
 *	Given a composite widget and an (x, y) coordinate, see if the
 *	(x, y) lies within one of the gadgets contained within the
 *	composite.  Return the gadget if found, otherwise return NULL.
 *
 ************************************************************************/

#ifdef _NO_PROTO
XmGadget _XmInputInGadget (cw, x, y)
CompositeWidget cw;
register int x;
register int y;

#else /* _NO_PROTO */
XmGadget _XmInputInGadget (CompositeWidget cw, int x, int y)
#endif /* _NO_PROTO */
{
   register int i;
   register Widget widget;

   for (i = 0; i < cw->composite.num_children; i++)
   {
      widget = cw->composite.children[i];

      if (XmIsGadget (widget) && XtIsManaged (widget))
      {
         if (x >= widget->core.x && y >= widget->core.y && 
             x < widget->core.x + widget->core.width    && 
             y < widget->core.y + widget->core.height)
            return ((XmGadget) widget);
      }
   }

   return (NULL);
}

/************************************************************************
 *
 *  XmInputForGadget
 *	This routine is a front-end for XmInputInGadget which returns NULL
 *      if the gadget returned from XmInputInGadget is not sensitive.
 *
 ************************************************************************/

#ifdef _NO_PROTO
XmGadget _XmInputForGadget (cw, x, y)
CompositeWidget cw;
int x;
int y;

#else /* _NO_PROTO */
XmGadget _XmInputForGadget (CompositeWidget cw, int x, int y)
#endif /* _NO_PROTO */
{
   XmGadget gadget;

   gadget = _XmInputInGadget ((CompositeWidget)cw, x, y);

   if (!gadget  ||  !XtIsSensitive (gadget))
   {
      return ((XmGadget) NULL);
   } 

   return (gadget);
}




/************************************************************************
 *
 *  XmConfigureGadget
 *	Change the dimensional aspects of a widget or gadget.
 *
 ************************************************************************/

#ifdef _NO_PROTO
void _XmConfigureObject (g, x, y, width, height, border_width)
RectObj g;
int x;
int y;
int width;
int height;
int border_width;

#else /* _NO_PROTO */
void _XmConfigureObject (RectObj g, int x, int y, int width, int height, int border_width)
#endif /* _NO_PROTO */
{
   if (XtIsWidget (g))
   {
      XtConfigureWidget (g, x, y, width, height, border_width);
   }
   else
   {
      if (g->rectangle.x != x ||g->rectangle.y != y ||
          g->rectangle.width != width || g->rectangle.height != height)
      {
         if (XtIsRealized (g) && XtIsManaged (g))
            XClearArea (XtDisplay (g), XtWindow (g),
                        g->rectangle.x, g->rectangle.y,
                        g->rectangle.width, g->rectangle.height, True);
                             
         g->rectangle.x = x;
         g->rectangle.y = y;
         g->rectangle.width = width;
         g->rectangle.height = height;
         g->rectangle.border_width = 0;

         if (g->object.widget_class->core_class.resize)
            (*(g->object.widget_class->core_class.resize))(g);

         if (XtIsRealized (g) && XtIsManaged (g))
         {
            XClearArea (XtDisplay (g), XtWindow (g),
                        g->rectangle.x, g->rectangle.y,
                        g->rectangle.width, g->rectangle.height, False);

            if (g->object.widget_class->core_class.expose)
               (*(g->object.widget_class->core_class.expose))(g, NULL, NULL);

         }
      }
   }
}




/************************************************************************
 *
 *  _XmResizeObject
 *	Change the width or height of a widget or gadget.
 *
 ************************************************************************/

#ifdef _NO_PROTO
void _XmResizeObject (g, width, height, border_width)
RectObj g;
int width;
int height;
int border_width;

#else /* _NO_PROTO */
void _XmResizeObject (RectObj g, int width, int height, int border_width)
#endif /* _NO_PROTO */
{
   if (XtIsWidget (g))
      XtResizeWidget (g, width, height, border_width);
   else
      _XmConfigureObject((RectObj)g, g->rectangle.x, g->rectangle.y, width, height, 0);
}




/************************************************************************
 *
 *  _XmMoveObject
 *	Change the origin of a widget or gadget.
 *
 ************************************************************************/

#ifdef _NO_PROTO
void _XmMoveObject (g, x, y)
RectObj g;
int x;
int y;

#else /* _NO_PROTO */
void _XmMoveObject (RectObj g, int x, int y)
#endif /* _NO_PROTO */
{
   if (XtIsWidget (g))
      XtMoveWidget (g, x, y);
   else
      _XmConfigureObject((RectObj)g, x, y, g->rectangle.width, g->rectangle.height, 0);
}




/************************************************************************
 *
 *  _XmRedisplayGadgets
 *	Redisplay any gadgets contained within the manager mw which
 *	are intersected by the region.
 *
 ************************************************************************/

#ifdef _NO_PROTO
void _XmRedisplayGadgets (mw, event, region)
CompositeWidget mw;
register XExposeEvent * event;
Region region;

#else /* _NO_PROTO */
void _XmRedisplayGadgets (CompositeWidget mw, XExposeEvent *event, Region region)
#endif /* _NO_PROTO */
{
   register int i;
   register Widget child;

   for (i = 0; i < mw->composite.num_children; i++)
   {
      child = mw->composite.children[i];
      if (XmIsGadget(child) && XtIsManaged(child))
      {
         if (region == NULL)
         {
            if (child->core.x < event->x + event->width      &&
                child->core.x + child->core.width > event->x &&
                child->core.y < event->y + event->height     &&
                child->core.y + child->core.height > event->y)
            {
               if (child->core.widget_class->core_class.expose)
                  (*(child->core.widget_class->core_class.expose))
                     (child, event, region);
            }
         }
         else
         {
            if (XRectInRegion (region, child->core.x, child->core.y,
                               child->core.width, child->core.height))
            {
               if (child->core.widget_class->core_class.expose)
                  (*(child->core.widget_class->core_class.expose))
                     (child, event, region);
            }
         }
      }
   }
}




/************************************************************************
 *
 *  _XmDispatchGadgetInput
 *	Call the gadgets class function and send the desired data to it.
 *
 ************************************************************************/

#ifdef _NO_PROTO
void _XmDispatchGadgetInput (g, event, mask)
XmGadget g;
XEvent * event;
Mask mask;

#else /* _NO_PROTO */
void _XmDispatchGadgetInput (XmGadget g, XEvent *event, Mask mask)
#endif /* _NO_PROTO */
{
   if (g->gadget.event_mask & mask && XtIsSensitive (g) && XtIsManaged (g))
      if (event != NULL) {
         XEvent synth_event;

#define CopyEvent(source, dest, type) \
    source.type = dest->type

         switch(mask) {
	   case XmENTER_EVENT:
                   CopyEvent(synth_event, event, xcrossing);
		   if (event->type != EnterNotify) {
		      synth_event.type = EnterNotify;
                   }
                   break;
	   case XmLEAVE_EVENT:
                   CopyEvent(synth_event, event, xcrossing);
		   if (event->type != LeaveNotify) {
		      synth_event.type = LeaveNotify;
                   }
                   break;
	   case XmFOCUS_IN_EVENT:
                   CopyEvent(synth_event, event, xfocus);
		   if (event->type != FocusIn) {
		      synth_event.type = FocusIn;
		   }
		   break;
	   case XmFOCUS_OUT_EVENT:
                   CopyEvent(synth_event, event, xfocus);
		   if (event->type != FocusIn) {
		      synth_event.type = FocusOut;
		   }
		   break;
	   case XmMOTION_EVENT:
                   CopyEvent(synth_event, event, xmotion);
		   if (event->type != MotionNotify) {
		      event->type = MotionNotify;
		   }
		   break;
	   case XmARM_EVENT:
                   CopyEvent(synth_event, event, xkey);
		   if (event->type != ButtonPress &&
		       event->type != KeyPress) {
		      synth_event.type = ButtonPress;
		   }
		   break;
	   case XmACTIVATE_EVENT:
                   CopyEvent(synth_event, event, xkey);
		   if (event->type != ButtonRelease &&
		       event->type != KeyPress) {
		      synth_event.type = ButtonRelease;
		   }
		   break;
	   case XmKEY_EVENT:
                   CopyEvent(synth_event, event, xkey);
		   if (event->type != KeyPress &&
		       event->type != ButtonPress) {
		      synth_event.type = KeyPress;
		   }
		   break;
	   case XmHELP_EVENT:
                   CopyEvent(synth_event, event, xkey);
		   if (event->type != KeyPress) {
		      synth_event.type = KeyPress;
		   }
		   break;
           default:
		   bcopy((char*)event, (char*)&synth_event, 
		      (int)sizeof(synth_event));
		   break;
         }
   
         (*(((XmGadgetClass) (g->object.widget_class))->
             gadget_class.input_dispatch)) ((Widget)g, &synth_event, mask);
      } else
         (*(((XmGadgetClass) (g->object.widget_class))->
             gadget_class.input_dispatch)) ((Widget)g, event, mask);
}
