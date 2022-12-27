#ifdef REV_INFO
#ifndef lint
static char SCCSID[] = "OSF/Motif: @(#)MessageBP.h	3.5 90/06/26";
#endif /* lint */
#endif /* REV_INFO */
/******************************************************************************
*******************************************************************************
*
*  (c) Copyright 1989, 1990, OPEN SOFTWARE FOUNDATION, INC.
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
#ifndef _XmessageP_h
#define _XmessageP_h

#include <Xm/BulletinBP.h>
#include <Xm/MessageB.h>


/*  New fields for the MessageBox widget class record  */

typedef struct
{
   int mumble;   /* No new procedures */
} XmMessageBoxClassPart;


/* Full class record declaration */

typedef struct _XmMessageBoxClassRec
{
   CoreClassPart             core_class;
   CompositeClassPart        composite_class;
   ConstraintClassPart       constraint_class;
   XmManagerClassPart        manager_class;
   XmBulletinBoardClassPart  bulletin_board_class;
   XmMessageBoxClassPart     message_box_class;
} XmMessageBoxClassRec;

externalref XmMessageBoxClassRec xmMessageBoxClassRec;


/* New fields for the MessageBox widget record */

typedef struct
{
    unsigned char           dialog_type;
    unsigned char           default_type;
    Boolean		    internal_pixmap;
    Boolean                 minimize_buttons;

    unsigned char           message_alignment;
    XmString                message_string;
    Widget                  message_wid;

    Pixmap                  symbol_pixmap;
    Widget                  symbol_wid;

    XmString                ok_label_string;
    XtCallbackList          ok_callback;
    Widget                  ok_button;

    XmString                cancel_label_string;
    XtCallbackList          cancel_callback;

    XmString                help_label_string;
    Widget                  help_button;

    Widget                  separator;

} XmMessageBoxPart;


/****************************************************************
 *
 * Full instance record declaration
 *
 ****************************************************************/

typedef struct _XmMessageBoxRec
{
    CorePart	         core;
    CompositePart        composite;
    ConstraintPart       constraint;
    XmManagerPart        manager;
    XmBulletinBoardPart  bulletin_board; 
    XmMessageBoxPart     message_box;
} XmMessageBoxRec;


#ifdef _NO_PROTO

extern Boolean _XmMessageBoxNoGeoRequest() ;
extern XmGeoMatrix _XmMessageBoxGeoMatrixCreate() ;

#else /* _NO_PROTO */

extern Boolean _XmMessageBoxNoGeoRequest( XmGeoMatrix geoSpec) ;
extern XmGeoMatrix _XmMessageBoxGeoMatrixCreate( XmMessageBoxWidget mb,
                                Widget instigator, XtWidgetGeometry *desired) ;

#endif /* _NO_PROTO */

#endif /* _XmMessage_h */
/* DON'T ADD ANYTHING AFTER THIS #endif */
