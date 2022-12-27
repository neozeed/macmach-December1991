#ifdef REV_INFO
#ifndef lint
static char SCCSID[] = "OSF/Motif: @(#)ArrowBGP.h	3.7 91/01/10";
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
#ifndef _XmArrowGadgetP_h
#define _XmArrowGadgetP_h

#include <Xm/ArrowBG.h>
#include <Xm/XmP.h>

/*  Arrow class structure  */

typedef struct _XmArrowButtonGadgetClassPart
{
   caddr_t extension;
} XmArrowButtonGadgetClassPart;


/*  Full class record declaration for Arrow class  */

typedef struct _XmArrowButtonGadgetClassRec
{
   RectObjClassPart             rect_class;
   XmGadgetClassPart            gadget_class;
   XmArrowButtonGadgetClassPart arrow_button_class;
} XmArrowButtonGadgetClassRec;

externalref XmArrowButtonGadgetClassRec xmArrowButtonGadgetClassRec;

/* "Gadget caching" is currently under investigation for ArrowBG.  It would
 * be very desirable to cache the XRectangles, requiring immediate reference
 * instead of indirectly through pointers.  ArrowBG will be cached by Beta.
 */
/*  The Arrow instance record  */

typedef struct _XmArrowButtonGadgetPart
{
   XtCallbackList activate_callback;
   XtCallbackList arm_callback;
   XtCallbackList disarm_callback;
   unsigned char direction;	  /*  the direction the arrow is pointing  */

   Boolean selected;

   short        top_count;
   short        cent_count;
   short        bot_count;
   XRectangle * top;
   XRectangle * cent;
   XRectangle * bot;

   Position old_x;
   Position old_y;

   GC      arrow_GC;	  /*  graphics context for arrow drawing   */
   XtIntervalId     timer;	
   unsigned char    multiClick;     /* KEEP/DISCARD resource */
   int          click_count;


} XmArrowButtonGadgetPart;


/*  Full instance record declaration  */

typedef struct _XmArrowButtonGadgetRec
{
   ObjectPart              object;
   RectObjPart             rectangle;
   XmGadgetPart            gadget;
   XmArrowButtonGadgetPart arrowbutton;
} XmArrowButtonGadgetRec;

#endif /* _XmArrowGadgetP_h */
/* DON'T ADD ANYTHING AFTER THIS #endif */
