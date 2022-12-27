#ifdef REV_INFO
#ifndef lint
static char SCCSID[] = "OSF/Motif: @(#)MainWP.h	3.3 90/04/24";
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
#ifndef _XmMainWindowP_h
#define _XmMainWindowP_h

#include <Xm/MainW.h>
#include <Xm/XmP.h>

#include <Xm/ScrollBar.h>
#include <Xm/ScrollBarP.h>
#include <Xm/DrawingA.h>
#include <Xm/SeparatoG.h>

/* New fields for the MainWindow widget class record */
typedef struct {
     int mumble;   /* No new procedures */
} XmMainWindowClassPart;

/****************
 *
 * Class record declaration
 *
 ****************/
typedef struct _XmMainWindowClassRec {
    CoreClassPart	core_class;
    CompositeClassPart  composite_class;
    ConstraintClassPart constraint_class;
    XmManagerClassPart  manager_class;
    XmScrolledWindowClassPart	swindow_class;
    XmMainWindowClassPart	mwindow_class;
} XmMainWindowClassRec;

externalref XmMainWindowClassRec xmMainWindowClassRec;

/****************
 *
 * Main Window instance structure.
 *
 ****************/
typedef struct {


   Dimension	AreaWidth,AreaHeight;
   Dimension	margin_width,margin_height;
   Widget       CommandWindow;
   Widget       MenuBar;
   Widget       Message;
   unsigned char CommandLoc;
   XmSeparatorGadget       Sep1,Sep2,Sep3;
   Boolean	ManagingSep;
   Boolean	ShowSep;
   
} XmMainWindowPart;


/************************************************************************
 *									*
 * Full instance record declaration					*
 *									*
 ************************************************************************/

typedef struct _XmMainWindowRec {
    CorePart	    core;
    CompositePart   composite;
    ConstraintPart constraint;
    XmManagerPart   manager;
    XmScrolledWindowPart   swindow;
    XmMainWindowPart   mwindow;
} XmMainWindowRec;

#define DEFAULT_HEIGHT 20
#define DEFAULT_WIDTH 20

#endif /* _XmMainWindowP_h */
/* DON'T ADD STUFF AFTER THIS #endif */
