#ifdef REV_INFO
#ifndef lint
static char SCCSID[] = "OSF/Motif: @(#)ToggleBP.h	3.6 91/01/10";
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
/********************************************
 *
 *   No new fields need to be defined
 *   for the Toggle widget class record
 *
 ********************************************/

#ifndef _XmToggleButtonP_h
#define _XmToggleButtonP_h

#include <Xm/ToggleB.h>
#include <Xm/XmP.h>

typedef struct _XmToggleButtonClassPart
 {
   int foo; /* No new fields needed */
 } XmToggleButtonClassPart;


/****************************************************
 *
 * Full class record declaration for Toggle class
 *
 ****************************************************/
typedef struct _XmToggleButtonClassRec {
    CoreClassPart	  	core_class;
    XmPrimitiveClassPart  	primitive_class;
    XmLabelClassPart      	label_class;
    XmToggleButtonClassPart	toggle_class;
} XmToggleButtonClassRec;


externalref XmToggleButtonClassRec xmToggleButtonClassRec;


/********************************************
 *
 * No new fields needed for instance record
 *
 ********************************************/

typedef struct _XmToggleButtonPart
{ 
   unsigned char	ind_type;
   Boolean		visible;
   Dimension		spacing;
   Dimension		indicator_dim;
   Boolean		indicator_set;
   Pixmap		on_pixmap; 
   Pixmap		insen_pixmap; 
   Boolean		set;
   Boolean     		visual_set; /* used for visuals and does not reflect
                                        the true state of the button */
   Boolean		ind_on;
   Boolean		fill_on_select;
   Pixel		select_color;
   GC			select_GC;
   GC			background_gc;
   XtCallbackList 	value_changed_CB,
                        arm_CB,
                        disarm_CB;
   Boolean      	Armed;
} XmToggleButtonPart;



/****************************************************************
 *
 * Full instance record declaration
 *
 ****************************************************************/

typedef struct _XmToggleButtonRec {
    CorePart	        core;
    XmPrimitivePart     primitive;
    XmLabelPart		label;
    XmToggleButtonPart  toggle;
} XmToggleButtonRec;

#endif /* _XmToggleButtonP_h */
/* DON'T ADD ANYTHING AFTER THIS #endif */
