#ifdef REV_INFO
#ifndef lint
static char SCCSID[] = "OSF/Motif: @(#)ScrolledWP.h	3.4 90/06/04";
#endif /* lint */
#endif /* REV_INFO */
/******************************************************************************
*******************************************************************************
*
*  (c) Copyright 1989, 1990, OPEN SOFTWARE FOUNDATION, INC.
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
*  (c) Copyright 1989, 1990, Open Software Foundation, Inc.  Unpublished - all
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
*  (c) Copyright 1989, 1990, Open Software Foundation, Inc.
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
#ifndef _XmScrolledWindowP_h
#define _XmScrolledWindowP_h

#include <Xm/ScrolledW.h>
#include <Xm/XmP.h>

#include <Xm/ScrollBar.h>
#include <Xm/DrawingA.h>

/* New fields for the ScrolledWindow widget class record */
typedef struct {
     int mumble;   /* No new procedures */
} XmScrolledWindowClassPart;

/****************
 *
 * Class record declaration
 *
 ****************/
typedef struct _XmScrolledWindowClassRec {
    CoreClassPart	core_class;
    CompositeClassPart  composite_class;
    ConstraintClassPart constraint_class;
    XmManagerClassPart  manager_class;
    XmScrolledWindowClassPart	swindow_class;
} XmScrolledWindowClassRec;

externalref XmScrolledWindowClassRec xmScrolledWindowClassRec;

/****************
 *
 * Scrolled Window instance structure.
 *
 ****************/
typedef struct {

   int vmin;		  /*  slider minimum coordiate position     */
   int vmax;		  /*  slider maximum coordiate position     */
   int vOrigin;		  /*  slider edge location                  */
   int vExtent;		  /*  slider size                           */

   int hmin;		  /*  Same as above for horizontal bar.     */
   int hmax;
   int hOrigin;
   int hExtent;

   Position hsbX,hsbY;
   Dimension hsbWidth,hsbHeight;    /* Dimensions for the horiz bar */

   Position vsbX,vsbY;
   Dimension vsbWidth,vsbHeight;    /* Dimensions for the vertical bar */

   Dimension    GivenHeight, GivenWidth;

   Dimension	AreaWidth,AreaHeight;
   Dimension	WidthPad,HeightPad;
   Position	XOffset, YOffset;

   Dimension	pad;

   Boolean	hasHSB;
   Boolean	hasVSB;
   Boolean	InInit;
   Boolean	FromResize;

   unsigned char	VisualPolicy;
   unsigned char	ScrollPolicy;
   unsigned char	ScrollBarPolicy;
   unsigned char	Placement;
   
   XmScrollBarWidget   	hScrollBar;
   XmScrollBarWidget   	vScrollBar;
   XmDrawingAreaWidget 	ClipWindow;
   Widget              	WorkWindow;
   
} XmScrolledWindowPart;


/************************************************************************
 *									*
 * Full instance record declaration					*
 *									*
 ************************************************************************/

typedef struct _XmScrolledWindowRec {
    CorePart	    core;
    CompositePart   composite;
    ConstraintPart constraint;
    XmManagerPart   manager;
    XmScrolledWindowPart   swindow;
} XmScrolledWindowRec;

#define DEFAULT_HEIGHT 20
#define DEFAULT_WIDTH 20

#endif /* _XmScrolledWindowP_h */
/* DON'T ADD STUFF AFTER THIS #endif */
