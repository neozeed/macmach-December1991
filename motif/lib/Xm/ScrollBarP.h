#ifdef REV_INFO
#ifndef lint
static char SCCSID[] = "OSF/Motif: @(#)ScrollBarP.h	3.10.1.1 91/02/21";
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
#ifndef _XmScrollBarP_h
#define _XmScrollBarP_h


#include <Xm/ScrollBar.h>
#include <Xm/XmP.h>

/*  Minimum slider width or height  */

#define MIN_SLIDER_THICKNESS 1
#define MIN_SLIDER_LENGTH 6

/*  ScrollBar class structure  */

typedef struct _XmScrollBarClassPart
{
   int foo;	/*  No new fields needed  */
} XmScrollBarClassPart;


/*  Full class record declaration for CheckBox class  */

typedef struct _XmScrollBarClassRec
{
   CoreClassPart        core_class;
   XmPrimitiveClassPart primitive_class;
   XmScrollBarClassPart scrollBar_class;
} XmScrollBarClassRec;

externalref XmScrollBarClassRec xmScrollBarClassRec;


/*  The ScrollBar instance record  */

typedef struct _XmScrollBarPart
{
   int value;
   int minimum;
   int maximum;
   int slider_size;

   unsigned char orientation;
   unsigned char processing_direction;
   Boolean show_arrows;

   int increment;
   int page_increment;

   int initial_delay;
   int repeat_delay;

   XtCallbackList value_changed_callback;
   XtCallbackList increment_callback;
   XtCallbackList decrement_callback;
   XtCallbackList page_increment_callback;
   XtCallbackList page_decrement_callback;
   XtCallbackList to_top_callback;
   XtCallbackList to_bottom_callback;
   XtCallbackList drag_callback;

   GC unhighlight_GC;
   GC foreground_GC;
   Pixel trough_color;

   Drawable pixmap;
   Boolean  sliding_on;
   Boolean  etched_slider;
   int saved_value;

   unsigned char flags;

   unsigned char change_type;
   Boolean first_scroll;
   XtIntervalId timer;

   short initial_x;
   short initial_y;
   short separation_x;
   short separation_y;

   short slider_x;
   short slider_y;
   short slider_width;
   short slider_height;

   short slider_area_x;
   short slider_area_y;
   short slider_area_width;
   short slider_area_height;

   short arrow1_x;
   short arrow1_y;
   unsigned char arrow1_orientation;
   Boolean arrow1_selected;

   short arrow2_x;
   short arrow2_y;
   unsigned char arrow2_orientation;
   Boolean arrow2_selected;

   short arrow_width;
   short arrow_height;

   short arrow1_top_count;
   short arrow1_cent_count;
   short arrow1_bot_count;

   XRectangle * arrow1_top;
   XRectangle * arrow1_cent;
   XRectangle * arrow1_bot;

   short arrow2_top_count;
   short arrow2_cent_count;
   short arrow2_bot_count;

   XRectangle * arrow2_top;
   XRectangle * arrow2_cent;
   XRectangle * arrow2_bot;
} XmScrollBarPart;


/*  Full instance record declaration  */

typedef struct _XmScrollBarRec
{
   CorePart	   core;
   XmPrimitivePart primitive;
   XmScrollBarPart scrollBar;
} XmScrollBarRec;


#endif /* _XmScrollBarP_h */
/* DON'T ADD ANYTHING AFTER THIS #endif */
