#ifdef REV_INFO
#ifndef lint
static char SCCSID[] = "OSF/Motif: @(#)PushBGP.h	3.6 90/05/29";
#endif /* lint */
#endif /* REV_INFO */
/******************************************************************************
*******************************************************************************
*
*  (c) Copyright 1989, 1990, OPEN SOFTWARE FOUNDATION, INC.
*  (c) Copyright 1989, DIGITAL EQUIPMENT CORPORATION, MAYNARD, MASS.
*  (c) Copyright 1987, 1988, 1989, 1990, HEWLETT-PACKARD COMPANY
*  (c) Copyright 1988 MASSACHUSETTS INSTITUTE OF TECHNOLOGY 
*  (c) Copyright 1988 MICROSOFT CORPORATION
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
#ifndef _XmPButtonGP_h
#define _XmPButtonGP_h

#include <Xm/PushBG.h>
#include <Xm/XmP.h>
#include <Xm/ExtObjectP.h>
#include <Xm/LabelGP.h>


/*************************************************************/
/* The PushButton Gadget Cache Object's class and instance records*/
/*************************************************************/


typedef struct _XmPushButtonGCacheObjClassPart
{
    int foo;
} XmPushButtonGCacheObjClassPart;


					
typedef struct _XmPushButtonGCacheObjClassRec  
{
	ObjectClassPart                     object_class;
        XmExtClassPart                      ext_class;
	XmLabelGCacheObjClassPart           label_class_cache;
	XmPushButtonGCacheObjClassPart      pushbutton_class_cache;
} XmPushButtonGCacheObjClassRec;

externalref XmPushButtonGCacheObjClassRec xmPushButtonGCacheObjClassRec;


typedef struct _XmPushButtonGCacheObjPart
{
   Boolean 	    fill_on_arm;
   Pixel            arm_color;
   Pixmap	    arm_pixmap;
   Pixmap           unarm_pixmap;
   unsigned char    multiClick;     /* KEEP/DISCARD resource */
   Dimension        default_button_shadow_thickness;
		     /* New resource - always add it to gadget's dimension. */

   GC               fill_gc;
   GC               background_gc;
   int              timer;
   
} XmPushButtonGCacheObjPart;

typedef struct _XmPushButtonGCacheObjRec
{
    ObjectPart                   object;
    XmExtPart                    ext;
    XmLabelGCacheObjPart         label_cache;
    XmPushButtonGCacheObjPart    pushbutton_cache;
} XmPushButtonGCacheObjRec;


/* PushButton class structure */

typedef struct _XmPushButtonGadgetClassPart
{
    caddr_t extension;  /* Pointer to extension record */
} XmPushButtonGadgetClassPart;


/* Full class record declaration for PushButton class */

typedef struct _XmPushButtonGadgetClassRec 
{
   RectObjClassPart             rect_class;
   XmGadgetClassPart            gadget_class;
   XmLabelGadgetClassPart       label_class;
   XmPushButtonGadgetClassPart  pushbutton_class;

} XmPushButtonGadgetClassRec;


externalref XmPushButtonGadgetClassRec xmPushButtonGadgetClassRec;


/* PushButton instance record */

typedef struct _XmPushButtonGadgetPart
{
   XtCallbackList   activate_callback;
   XtCallbackList   arm_callback;
   XtCallbackList   disarm_callback;

   Dimension        show_as_default;
   Boolean 	    armed;
   int              click_count;

   Boolean	    compatible;	  /* if false it is Motif 1.1 else Motif 1.0  */
                                  /* not cached for performance reasons */
   
   XmPushButtonGCacheObjPart  *cache; /* Replace cache instance fields */
					/* with a pointer */
} XmPushButtonGadgetPart;

/* Full instance record declaration */

typedef struct _XmPushButtonGadgetRec {
   ObjectPart              object;
   RectObjPart             rectangle;
   XmGadgetPart            gadget;
   XmLabelGadgetPart       label;
   XmPushButtonGadgetPart   pushbutton;
} XmPushButtonGadgetRec;

/* MACROS */
/**********/

/* Macros for cached instance fields */

#define PBG_FillOnArm(w)		(((XmPushButtonGadget) (w)) -> \
					   pushbutton.cache->fill_on_arm)
#define PBG_ArmColor(w)			(((XmPushButtonGadget) (w)) -> \
					   pushbutton.cache->arm_color)
#define PBG_FillGc(w)			(((XmPushButtonGadget) (w)) -> \
					   pushbutton.cache->fill_gc)
#define PBG_BackgroundGc(w)		(((XmPushButtonGadget) (w)) -> \
					   pushbutton.cache->background_gc)
#define PBG_Timer(w)			(((XmPushButtonGadget) (w)) -> \
					   pushbutton.cache->timer)
#define PBG_ArmPixmap(w)		(((XmPushButtonGadget) (w)) -> \
					   pushbutton.cache->arm_pixmap)
#define PBG_UnarmPixmap(w)		(((XmPushButtonGadget) (w)) -> \
					   pushbutton.cache->unarm_pixmap)
#define PBG_MultiClick(w)      		(((XmPushButtonGadget) (w)) -> \
                       			   pushbutton.cache->multiClick)
#define PBG_DefaultButtonShadowThickness(w)     (((XmPushButtonGadget) (w)) -> \
                       pushbutton.cache->default_button_shadow_thickness)

/* Macros for uncached instance fields */

#define PBG_ActivateCallback(w)		(((XmPushButtonGadget) (w)) -> \
					   pushbutton.activate_callback)
#define PBG_ArmCallback(w)		(((XmPushButtonGadget) (w)) -> \
					   pushbutton.arm_callback)
#define PBG_DisarmCallback(w)		(((XmPushButtonGadget) (w)) -> \
					   pushbutton.disarm_callback)
#define PBG_Armed(w)			(((XmPushButtonGadget) (w)) -> \
					   pushbutton.armed)
#define PBG_ClickCount(w)      (((XmPushButtonGadget) (w)) -> \
                       pushbutton.click_count)
#define PBG_Compatible(w)      (((XmPushButtonGadget) (w)) -> \
                       pushbutton.compatible)
#define PBG_ShowAsDefault(w)		(((XmPushButtonGadget) (w)) -> \
					   pushbutton.show_as_default)

/******************************/
/* Convenience Macros         */
/******************************/

#define PBG_Cache(w)			(((XmPushButtonGadget)(w))->\
					   pushbutton.cache)
#define PBG_ClassCachePart(w) \
	(((XmPushButtonGadgetClass)xmPushButtonGadgetClass)->gadget_class.cache_part)


#endif /* _XmPButtonGP_h */
/* DON'T ADD ANYTHING AFTER THIS #endif */
