#ifdef REV_INFO
#ifndef lint
static char SCCSID[] = "OSF/Motif: @(#)ToggleBGP.h	3.8 91/01/10";
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

#ifndef _XmToggleButtonGP_h
#define _XmToggleButtonGP_h

#include <Xm/ToggleBG.h>
#include <Xm/XmP.h>
#include <Xm/LabelGP.h>
#include <Xm/ExtObjectP.h>

/*************************************************************/
/* The  ToggleButton Gadget Cache Object's class and instance records*/
/*************************************************************/


typedef struct _XmToggleButtonGCacheObjClassPart
{
    int foo;
} XmToggleButtonGCacheObjClassPart;


typedef struct _XmToggleButtonGCacheObjClassRec 
{
	ObjectClassPart                     object_class;
        XmExtClassPart                      ext_class;
	XmLabelGCacheObjClassPart           label_class_cache;
	XmToggleButtonGCacheObjClassPart    toggle_class_cache;
} XmToggleButtonGCacheObjClassRec;

externalref XmToggleButtonGCacheObjClassRec xmToggleButtonGCacheObjClassRec;


typedef struct _XmToggleButtonGCacheObjPart
{ 
   unsigned char	ind_type;
   Boolean		visible;
   Dimension		spacing;
   Dimension		indicator_dim;
   Pixmap		on_pixmap; 
   Pixmap		insen_pixmap; 
   Boolean		ind_on;
   Boolean		fill_on_select;
   Pixel		select_color;
   GC			select_GC;
   GC			background_gc;
} XmToggleButtonGCacheObjPart;

typedef struct _XmToggleButtonGCacheObjRec
{
    ObjectPart   		            object;
    XmExtPart                		    ext;
    XmLabelGCacheObjPart     		    label_cache;
    XmToggleButtonGCacheObjPart             toggle_cache;
} XmToggleButtonGCacheObjRec;


/****************************************************
 *
 * Full class record declaration for Toggle class
 *
 ****************************************************/

typedef struct _XmToggleButtonGadgetClassPart
 {
   caddr_t				   extension;
 } XmToggleButtonGadgetClassPart;


typedef struct _XmToggleButtonGadgetClassRec {
    RectObjClassPart  	 	  	rect_class;
    XmGadgetClassPart  			gadget_class;
    XmLabelGadgetClassPart 	    	label_class;
    XmToggleButtonGadgetClassPart	toggle_class;
} XmToggleButtonGadgetClassRec;


externalref XmToggleButtonGadgetClassRec xmToggleButtonGadgetClassRec;




typedef struct _XmToggleButtonGadgetPart
{ 
   Boolean		indicator_set;
   Boolean		set;
   Boolean      	visual_set; /* used for visuals and does not reflect
   		                            the true state of the button */
   Boolean     		Armed;
   XtCallbackList       value_changed_CB,
			arm_CB,
		        disarm_CB;

   XmToggleButtonGCacheObjPart  *cache; /* Replace cache instance fields */
					/* with a pointer */
} XmToggleButtonGadgetPart;



/****************************************************************
 *
 * Full instance record declaration
 *
 ****************************************************************/

typedef struct _XmToggleButtonGadgetRec {
    ObjectPart			object;
    RectObjPart			rectangle;
    XmGadgetPart		gadget;
    XmLabelGadgetPart		label;
    XmToggleButtonGadgetPart	toggle;
} XmToggleButtonGadgetRec;


/* MACROS */
/**********/
 
/* Macros for cached instance fields */

#define TBG_IndType(w)		(((XmToggleButtonGadget) (w)) -> \
				   toggle.cache->ind_type)
#define TBG_Visible(w)		(((XmToggleButtonGadget) (w)) -> \
				   toggle.cache->visible)
#define TBG_Spacing(w)		(((XmToggleButtonGadget) (w)) -> \
				   toggle.cache->spacing)
#define TBG_IndicatorDim(w)	(((XmToggleButtonGadget) (w)) -> \
				   toggle.cache->indicator_dim)
#define TBG_OnPixmap(w)		(((XmToggleButtonGadget) (w)) -> \
				   toggle.cache->on_pixmap)
#define TBG_InsenPixmap(w)	(((XmToggleButtonGadget) (w)) -> \
				   toggle.cache->insen_pixmap)
#define TBG_IndOn(w)		(((XmToggleButtonGadget) (w)) -> \
				   toggle.cache->ind_on)
#define TBG_FillOnSelect(w)	(((XmToggleButtonGadget) (w)) -> \
				   toggle.cache->fill_on_select)
#define TBG_SelectColor(w)	(((XmToggleButtonGadget) (w)) -> \
				   toggle.cache->select_color)
#define TBG_SelectGC(w)		(((XmToggleButtonGadget) (w)) -> \
				   toggle.cache->select_GC)
#define TBG_BackgroundGC(w)	(((XmToggleButtonGadget) (w)) -> \
				   toggle.cache->background_gc)

/***************************************/
/* Macros for uncached instance fields */

#define TBG_IndicatorSet(w)	(((XmToggleButtonGadget) (w)) -> \
				   toggle.indicator_set)
#define TBG_Set(w)		(((XmToggleButtonGadget) (w)) -> \
                		   toggle.set)
#define TBG_VisualSet(w)	(((XmToggleButtonGadget) (w)) -> \
				   toggle.visual_set)
#define TBG_ValueChangedCB(w)	(((XmToggleButtonGadget) (w)) -> \
				   toggle.value_changed_CB)
#define TBG_ArmCB(w)		(((XmToggleButtonGadget) (w)) -> \
				   toggle.arm_CB)
#define TBG_DisarmCB(w)		(((XmToggleButtonGadget) (w)) -> \
				   toggle.disarm_CB)
#define TBG_Armed(w)		(((XmToggleButtonGadget) (w)) -> \
				   toggle.Armed)

/******************************/
/* Convenience Macros         */
/******************************/

#define TBG_Cache(w)                    (((XmToggleButtonGadget)(w))->\
					   toggle.cache)
#define TBG_ClassCachePart(w) \
        (((XmToggleButtonGadgetClass)xmToggleButtonGadgetClass)->gadget_class.cache_part)

#endif /* _XmToggleButtonGP_h */
/* DON'T ADD ANYTHING AFTER THIS #endif */
