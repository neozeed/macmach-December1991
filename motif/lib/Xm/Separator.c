#ifdef REV_INFO
#ifndef lint
static char SCCSID[] = "OSF/Motif: @(#)Separator.c	3.8 91/01/10";
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
#include <X11/keysymdef.h>   
#include <X11/IntrinsicP.h>
#include <Xm/XmP.h>
#include <Xm/RowColumnP.h>
#include <Xm/SeparatorP.h>

#define MESSAGE1 "Invalid separator type."
#define MESSAGE2 "Invalid orientation."


/* Definition for resources that need special processing in get values */

static XmSyntheticResource syn_resources[] = 
{
   {
      XmNmargin, 
      sizeof (Dimension),
      XtOffset (XmSeparatorWidget, separator.margin),
      _XmFromHorizontalPixels,
      _XmToHorizontalPixels
   },
};


/*  Resource list for Separator  */

static XtResource resources[] = 
{
   {
      XmNseparatorType, XmCSeparatorType, XmRSeparatorType, sizeof (unsigned char),
      XtOffset (XmSeparatorWidget, separator.separator_type),
      XmRImmediate, (caddr_t) XmSHADOW_ETCHED_IN
   },

   {
      XmNmargin, 
      XmCMargin,
      XmRHorizontalDimension, 
      sizeof (Dimension),
      XtOffset (XmSeparatorWidget, separator.margin),
      XmRImmediate, (caddr_t)  0
   },

   {
      XmNorientation, XmCOrientation, XmROrientation, sizeof (unsigned char),
      XtOffset (XmSeparatorWidget, separator.orientation),
      XmRImmediate, (caddr_t) XmHORIZONTAL
   },
   {
     XmNtraversalOn,
     XmCTraversalOn,
     XmRBoolean,
     sizeof (Boolean),
     XtOffset (XmPrimitiveWidget, primitive.traversal_on),
     XmRImmediate, (caddr_t) FALSE
   },
   {
     XmNhighlightThickness,
     XmCHighlightThickness,
     XmRHorizontalDimension,
     sizeof (Dimension),
     XtOffset (XmPrimitiveWidget, primitive.highlight_thickness),
     XmRImmediate, (caddr_t) 0
   },

};


/*  Static routine definitions  */

static void    Initialize();
static void    ClassPartInitialize();
static void    Redisplay();
static void    Destroy();
static void    GetSeparatorGC();
static Boolean SetValues();


static XtActionsRec actionsList[] =
{
     { "Help",	(XtActionProc) _XmPrimitiveHelp },
};


/*  The Separator class record definition  */

externaldef(xmseparatorclassrec) XmSeparatorClassRec xmSeparatorClassRec =
{
   {
      (WidgetClass) &xmPrimitiveClassRec, /* superclass	 	 */
      "XmSeparator",                        /* class_name	         */	
      sizeof(XmSeparatorRec),             /* widget_size         */	
      NULL,				/* class_initialize      */    
      ClassPartInitialize,              /* class_part_initialize */
      FALSE,                            /* class_inited          */	
      (XtInitProc) Initialize,          /* initialize	         */	
      NULL,                             /* initialize_hook       */
      _XtInherit,                       /* realize	         */	
      actionsList,                      /* actions               */	
      XtNumber(actionsList),            /* num_actions    	 */	
      resources,                        /* resources	         */	
      XtNumber (resources),             /* num_resources         */	
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
      NULL,                             /* display_accelerator   */
      NULL,                             /* extension             */
   },

   {
      NULL,                             /* Primitive border_highlight   */
      NULL,                             /* Primitive border_unhighlight */
      NULL,                             /* translations                 */
      NULL,                             /* arm_and_activate             */
      syn_resources,                    /* syn resources                */
      XtNumber(syn_resources),	        /* num syn_resources            */
      NULL,                             /* extension                    */
   }
};

externaldef(xmseparatorwidgetclass) WidgetClass xmSeparatorWidgetClass =
				   (WidgetClass) &xmSeparatorClassRec;


/************************************************************************
 *
 *  ClassPartInitialize
 *     Set up the fast subclassing for the widget
 *
 ************************************************************************/
static void ClassPartInitialize (wc)
WidgetClass wc;

{
   _XmFastSubclassInit (wc, XmSEPARATOR_BIT);
}

      
/************************************************************************
 *
 *  Initialize
 *     The main widget instance initialization routine.
 *
 ************************************************************************/

static void Initialize (request, new)
XmSeparatorWidget request, new;

{
   new -> primitive.traversal_on = FALSE; 

   /* Force highlightThickness to zero if in a menu. */
   if (XmIsRowColumn(XtParent(new)) && 
       ((RC_Type(XtParent(new)) == XmMENU_PULLDOWN) ||
	(RC_Type(XtParent(new)) == XmMENU_POPUP)))
     new->primitive.highlight_thickness = 0;

   if (new -> separator.separator_type != XmNO_LINE            &&
       new -> separator.separator_type != XmSINGLE_LINE        &&
       new -> separator.separator_type != XmDOUBLE_LINE        &&
       new -> separator.separator_type != XmSINGLE_DASHED_LINE &&
       new -> separator.separator_type != XmDOUBLE_DASHED_LINE &&
       new -> separator.separator_type != XmSHADOW_ETCHED_IN   &&
       new -> separator.separator_type != XmSHADOW_ETCHED_OUT)
   {
      _XmWarning (new, MESSAGE1);
      new -> separator.separator_type = XmSHADOW_ETCHED_IN;
   }

   if (new -> separator.orientation != XmHORIZONTAL &&
       new -> separator.orientation != XmVERTICAL)
   {
      _XmWarning (new, MESSAGE2);
      new -> separator.orientation = XmHORIZONTAL;
   }

   if (new->separator.orientation == XmHORIZONTAL)
   {
      if (request -> core.width == 0)
	 new -> core.width = 2 * new -> primitive.highlight_thickness +2;

      if (request -> core.height == 0)
      {
	 new -> core.height = 2 * new -> primitive.highlight_thickness;

	 if (new -> separator.separator_type == XmSINGLE_LINE ||
	     new -> separator.separator_type == XmSINGLE_DASHED_LINE)
	    new -> core.height += 3;
	 else if (new -> separator.separator_type == XmSHADOW_ETCHED_IN ||
		  new -> separator.separator_type == XmSHADOW_ETCHED_OUT)
	    new -> core.height += new -> primitive.shadow_thickness;
	 else if (new -> separator.separator_type == XmDOUBLE_LINE ||
		  new -> separator.separator_type == XmDOUBLE_DASHED_LINE)
	    new -> core.height += 5;
	 else
	    if (new -> core.height == 0)
	       new -> core.height = 1;
      }
   }
   
   if (new->separator.orientation == XmVERTICAL)
   {
      if (request -> core.height == 0)
	 new -> core.height = 2 * new -> primitive.highlight_thickness +2;

      if (request -> core.width == 0)
      {
	 new -> core.width = 2 * new -> primitive.highlight_thickness;

	 if (new -> separator.separator_type == XmSINGLE_LINE ||
	     new -> separator.separator_type == XmSINGLE_DASHED_LINE)
	    new -> core.width += 3;
	 else if (new -> separator.separator_type == XmSHADOW_ETCHED_IN ||
		  new -> separator.separator_type == XmSHADOW_ETCHED_OUT)
	    new -> core.width += new -> primitive.shadow_thickness;
	 else if (new -> separator.separator_type == XmDOUBLE_LINE ||
		  new -> separator.separator_type == XmDOUBLE_DASHED_LINE)
	    new -> core.width += 5;
	 else
	    if (new -> core.width == 0)
	       new -> core.width = 1;
      }
   }
   
   /*  Get the drawing graphics contexts.  */

   GetSeparatorGC (new);
}




/************************************************************************
 *
 *  GetSeparatorGC
 *     Get the graphics context used for drawing the separator.
 *
 ************************************************************************/

static void GetSeparatorGC (mw)
XmSeparatorWidget mw;

{
   XGCValues values;
   XtGCMask  valueMask;

   valueMask = GCForeground | GCBackground;

   values.foreground = mw -> primitive.foreground;
   values.background = mw -> core.background_pixel;

   if (mw -> separator.separator_type == XmSINGLE_DASHED_LINE ||
       mw -> separator.separator_type == XmDOUBLE_DASHED_LINE)
   {
      valueMask = valueMask | GCLineStyle;
      values.line_style = LineDoubleDash;
   }

   mw -> separator.separator_GC = XtGetGC ((Widget) mw, valueMask, &values);
}




/************************************************************************
 *
 *  Redisplay
 *     Invoke the application exposure callbacks.
 *
 ************************************************************************/
/*ARGSUSED*/
static void Redisplay (mw, event, region)
XmSeparatorWidget mw;
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

   if (seg_count == 0)
   {
      segs = (XSegment *) XtMalloc (sizeof (XSegment) * lineCount);
      seg_count = lineCount;
   }

   if (mw -> separator.orientation == XmHORIZONTAL)
   {
      center = mw -> core.height / 2;
      top = mw -> primitive.highlight_thickness;
      bottom = mw -> core.height - 
               mw -> primitive.highlight_thickness;
   }
   else
   {
      center = mw -> core.width / 2;
      top = mw -> primitive.highlight_thickness;
      bottom = mw -> core.width - 
               mw -> primitive.highlight_thickness;
   }

   switch (mw -> separator.separator_type)
   {

      case XmNO_LINE:
         lineCount = 0;
         break;

      case XmSINGLE_LINE:
      case XmSINGLE_DASHED_LINE:
         lineCount = 0;
         if (mw -> separator.orientation == XmHORIZONTAL)
         {
            if (center >= top && center < bottom)
            {
	       segs[0].y1 = segs[0].y2 = center;
	       segs[0].x1 = mw -> primitive.highlight_thickness +
			    mw -> separator.margin;
	       segs[0].x2 = mw -> core.width - 
			    mw -> primitive.highlight_thickness -
			    mw -> separator.margin - 1;
               lineCount ++;
            }
         }
         else
         {
            if (center >= top && center < bottom)
            {
	       segs[0].x1 = segs[0].x2 = center;
	       segs[0].y1 = mw -> primitive.highlight_thickness +
			    mw -> separator.margin;
	       segs[0].y2 = mw -> core.height - 
			    mw -> primitive.highlight_thickness -
			    mw -> separator.margin - 1;
               lineCount ++;
            }
         }
      break;

      case XmDOUBLE_LINE:
      case XmDOUBLE_DASHED_LINE:
         lineCount = 0;
         if (mw -> separator.orientation == XmHORIZONTAL)
         {
            if (center - 1 >= top && center - 1 < bottom)
            {
               segs[0].y1 = segs[0].y2 = center - 1;
               segs[0].x1 = mw -> primitive.highlight_thickness +
                            mw -> separator.margin;
               segs[0].x2 = mw -> core.width -
                            mw -> primitive.highlight_thickness -
			    mw -> separator.margin - 1;
               lineCount ++;
	    }
	       
            if (center + 1 >= top && center + 1 < bottom)
            {
               segs[1].y1 = segs[1].y2 = center + 1;
               segs[1].x1 = mw -> primitive.highlight_thickness +
                            mw -> separator.margin;
               segs[1].x2 = mw -> core.width -
                            mw -> primitive.highlight_thickness -
	                    mw -> separator.margin - 1;
	       lineCount++;
            }
	 }
         else     /* orientation = XmVERTICAL */
         {

            if (center - 1 > top && center - 1 < bottom)
            {
	       segs[0].x1 = segs[0].x2 = center - 1;
               segs[0].y1 = mw -> primitive.highlight_thickness +
			    mw -> separator.margin;
               segs[0].y2 = mw -> core.height -
                            mw -> primitive.highlight_thickness -
			    mw -> separator.margin - 1;
               lineCount ++;
            }

            if (center + 1 > top && center + 1 < bottom)
            {
               segs[1].x1 = segs[1].x2 = center + 1;
               segs[1].y1 = mw -> primitive.highlight_thickness +
	                    mw -> separator.margin;
               segs[1].y2 = mw -> core.height -
                            mw -> primitive.highlight_thickness -
	                    mw -> separator.margin - 1;
               lineCount ++;
            }
	 }
      break;

      case XmSHADOW_ETCHED_IN:
      case XmSHADOW_ETCHED_OUT:

	 if (mw -> separator.orientation == XmHORIZONTAL)
	    if (mw -> primitive.shadow_thickness > mw -> core.height -
	            2 * mw -> primitive.highlight_thickness) 
	       lineCount = (mw -> core.height - 
	                    2 * mw -> primitive.highlight_thickness)/2;
            else 
               lineCount = mw -> primitive.shadow_thickness/2;

	 if (mw -> separator.orientation == XmVERTICAL)
	    if (mw -> primitive.shadow_thickness > mw -> core.width -
	            2 * mw -> primitive.highlight_thickness) 
	       lineCount = (mw -> core.width -
	                    2 * mw -> primitive.highlight_thickness)/2;
            else
               lineCount = mw -> primitive.shadow_thickness/2;

         start = center - lineCount;

	 if (seg_count < mw->primitive.shadow_thickness)
	 {
	    segs = (XSegment *) XtRealloc (segs, sizeof (XSegment) * mw->primitive.shadow_thickness);
	    seg_count = mw->primitive.shadow_thickness;
	 }

	 if (mw -> separator.orientation == XmHORIZONTAL)
	 {
            for (i=0; i < (lineCount*2); i++, start++)
            {
	       segs[i].y1 = segs[i].y2 = start;
	       segs[i].x1 = mw -> primitive.highlight_thickness +
		           mw -> separator.margin;
	       segs[i].x2 = mw -> core.width -
	                   mw -> primitive.highlight_thickness -
		           mw -> separator.margin - 1;
            }
	 }
	 else
	 {
            for (i=0; i < (lineCount*2); i++, start++)
            {
	       segs[i].x1 = segs[i].x2 = start;
	       segs[i].y1 = mw -> primitive.highlight_thickness +
				       mw -> separator.margin;
	       segs[i].y2 = mw -> core.height -
				       mw -> primitive.highlight_thickness -
				       mw -> separator.margin - 1;
	    }
         }
      break;
   }

   if  (mw -> separator.separator_type != XmNO_LINE && lineCount)
   {
      if (mw -> separator.separator_type == XmSHADOW_ETCHED_IN)
      {
         XDrawSegments (XtDisplay ((Widget) mw), XtWindow ((Widget) mw),
                     mw -> primitive.bottom_shadow_GC, segs, lineCount);
         XDrawSegments (XtDisplay ((Widget) mw), XtWindow ((Widget) mw),
                     mw -> primitive.top_shadow_GC, &segs[lineCount], lineCount);
      }	 
      else if (mw -> separator.separator_type == XmSHADOW_ETCHED_OUT)
      {
         XDrawSegments (XtDisplay ((Widget) mw), XtWindow ((Widget) mw),
                     mw -> primitive.top_shadow_GC, segs, lineCount);
         XDrawSegments (XtDisplay ((Widget) mw), XtWindow ((Widget) mw),
                     mw -> primitive.bottom_shadow_GC, &segs[lineCount], lineCount);
      }
      else XDrawSegments (XtDisplay ((Widget) mw), XtWindow ((Widget) mw),
                     mw -> separator.separator_GC, segs, lineCount);
   }

}




/************************************************************************
 *
 *  Destroy
 *	Remove the callback lists.
 *
 ************************************************************************/

static void Destroy (mw)
XmSeparatorWidget mw;

{
   XtReleaseGC (mw, mw -> separator.separator_GC);
}




/************************************************************************
 *
 *  SetValues
 *
 ************************************************************************/

static Boolean SetValues (current, request, new)
XmSeparatorWidget current, request, new;

{
   Boolean flag = FALSE;   

   /*
    * We never allow our traversal flags to be changed during SetValues();
    * this is enforced by our superclass.
    */

   /*  Force traversal_on to FALSE */
   new -> primitive.traversal_on = FALSE;
 
   /* Force highlightThickness to zero if in a menu. */
   if (XmIsRowColumn(XtParent(new)) && 
       ((RC_Type(XtParent(new)) == XmMENU_PULLDOWN) ||
	(RC_Type(XtParent(new)) == XmMENU_POPUP)))
     new->primitive.highlight_thickness = 0;

   if (new -> separator.separator_type != XmNO_LINE             &&
       new -> separator.separator_type != XmSINGLE_LINE         &&
       new -> separator.separator_type != XmDOUBLE_LINE         &&
       new -> separator.separator_type != XmSINGLE_DASHED_LINE  &&
       new -> separator.separator_type != XmDOUBLE_DASHED_LINE  &&
       new -> separator.separator_type != XmSHADOW_ETCHED_IN    &&
       new -> separator.separator_type != XmSHADOW_ETCHED_OUT)
   {
      _XmWarning (new, MESSAGE1);
      new -> separator.separator_type = current -> separator.separator_type;
   }

   if (new -> separator.orientation != XmHORIZONTAL &&
       new -> separator.orientation != XmVERTICAL)
   {
      _XmWarning (new, MESSAGE2);
      new -> separator.orientation = current -> separator.orientation;
   }

   if (new -> separator.orientation == XmHORIZONTAL)
   {
      if (request -> core.width == 0)
	 new -> core.width = 2 * new->primitive.highlight_thickness + 2;

      if (request -> core.height == 0)
      {
	 new -> core.height = 2 * new -> primitive.highlight_thickness;

	 if (new -> separator.separator_type == XmSINGLE_LINE ||
	     new -> separator.separator_type == XmSINGLE_DASHED_LINE)
	    new -> core.height += 3;
	 else if (new -> separator.separator_type == XmSHADOW_ETCHED_IN ||
		  new -> separator.separator_type == XmSHADOW_ETCHED_OUT)
	    new -> core.height += new -> primitive.shadow_thickness;
	 else if (new -> separator.separator_type == XmDOUBLE_LINE ||
		  new -> separator.separator_type == XmDOUBLE_DASHED_LINE)
	    new -> core.height += 5;
	 else
	    if (new -> core.height == 0)
	       new -> core.height = 1;
      }

      if (new -> separator.separator_type != current -> separator.separator_type &&
	  request -> core.height == current -> core.height)
      {
	 if (new -> separator.separator_type == XmSINGLE_LINE ||
	     new -> separator.separator_type == XmSINGLE_DASHED_LINE) 
	    new -> core.height = 2 * new -> primitive.highlight_thickness + 3;
	 else if (new -> separator.separator_type == XmSHADOW_ETCHED_IN ||
		  new -> separator.separator_type == XmSHADOW_ETCHED_OUT)
	    new -> core.height = 2 * new -> primitive.highlight_thickness +
				       new -> primitive.shadow_thickness;
	 else if (new -> separator.separator_type == XmDOUBLE_LINE ||
		  new -> separator.separator_type == XmDOUBLE_DASHED_LINE) 
	    new -> core.height = 2 * new -> primitive.highlight_thickness + 5;
      }
   } 

   if (new -> separator.orientation == XmVERTICAL)
   {
      if (request -> core.height == 0)
	 new -> core.height = 2 * new->primitive.highlight_thickness + 2;

      if (request -> core.width == 0)
      {
	 new -> core.width = 2 * new -> primitive.highlight_thickness;

	 if (new -> separator.separator_type == XmSINGLE_LINE ||
	     new -> separator.separator_type == XmSINGLE_DASHED_LINE)
	    new -> core.width += 3;
	 else if (new -> separator.separator_type == XmSHADOW_ETCHED_IN ||
		  new -> separator.separator_type == XmSHADOW_ETCHED_OUT)
	    new -> core.width += new -> primitive.shadow_thickness;
	 else if (new -> separator.separator_type == XmDOUBLE_LINE ||
		  new -> separator.separator_type == XmDOUBLE_DASHED_LINE)
	    new -> core.width += 5;
	 else
	    if (new -> core.width == 0)
	       new -> core.width = 1;
      }

      if (new -> separator.separator_type != current -> separator.separator_type &&
	  request -> core.width == current -> core.width)
      {
	 if (new -> separator.separator_type == XmSINGLE_LINE ||
	     new -> separator.separator_type == XmSINGLE_DASHED_LINE) 
	    new -> core.width = 2 * new -> primitive.highlight_thickness + 3;
	 else if (new -> separator.separator_type == XmSHADOW_ETCHED_IN ||
		  new -> separator.separator_type == XmSHADOW_ETCHED_OUT)
	    new -> core.width = 2 * new -> primitive.highlight_thickness +
				       new -> primitive.shadow_thickness;
	 else if (new -> separator.separator_type == XmDOUBLE_LINE ||
		  new -> separator.separator_type == XmDOUBLE_DASHED_LINE) 
	    new -> core.width = 2 * new -> primitive.highlight_thickness + 5;
      }
   } 

   if (new -> separator.orientation != current -> separator.orientation ||
       new -> separator.margin != current -> separator.margin ||
       new -> primitive.shadow_thickness != current -> primitive.shadow_thickness) 
      flag = TRUE;

   if (new -> separator.separator_type != current -> separator.separator_type  ||
       new -> core.background_pixel != current -> core.background_pixel    ||
       new -> primitive.foreground != current -> primitive.foreground)
   {
      XtReleaseGC (new, new -> separator.separator_GC);
      GetSeparatorGC (new);
      flag = TRUE;
   }

   return (flag);
}

/************************************************************************
 *
 *  XmCreateSeparator
 *	Create an instance of a separator and return the widget id.
 *
 ************************************************************************/

#ifdef _NO_PROTO
Widget XmCreateSeparator (parent, name, arglist, argcount)
Widget parent;
char * name;
ArgList arglist;
Cardinal argcount;

#else /* _NO_PROTO */
Widget XmCreateSeparator (Widget parent, char *name, ArgList arglist, Cardinal argcount)
#endif /* _NO_PROTO */
{
   return (XtCreateWidget (name, xmSeparatorWidgetClass, 
                           parent, arglist, argcount));
}


