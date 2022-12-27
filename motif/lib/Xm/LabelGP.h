#ifdef REV_INFO
#ifndef lint
static char SCCSID[] = "OSF/Motif: @(#)LabelGP.h	3.8 91/01/10";
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
#ifndef _XmLabelGP_h
#define _XmLabelGP_h

#include <Xm/LabelG.h>
#include <Xm/ExtObjectP.h>
#include <Xm/XmP.h>

/*************************************************************/
/* The Label Gadget Cache Object's class and instance records*/
/*************************************************************/


typedef struct _XmLabelGCacheObjClassPart
{
    int foo;
} XmLabelGCacheObjClassPart;


typedef struct _XmLabelGCacheObjClassRec     /* label cache class record */
{
    ObjectClassPart			object_class;
    XmExtClassPart                 	ext_class;
    XmLabelGCacheObjClassPart	 	label_class_cache;
} XmLabelGCacheObjClassRec;

externalref XmLabelGCacheObjClassRec xmLabelGCacheObjClassRec;


/*  The Label Gadget Cache instance record  */

typedef struct _XmLabelGCacheObjPart
{
        unsigned char	label_type;
        unsigned char	alignment;
        unsigned char	string_direction;
	
	Dimension	margin_height;   /* margin around widget */
	Dimension	margin_width;

	Dimension	margin_left;    /* additional margins on */
	Dimension	margin_right;   /* each side of widget */
	Dimension	margin_top;     /* text (or pixmap) is placed */
	Dimension	margin_bottom;  /* inside the margins */

	Boolean recompute_size;

	Boolean		skipCallback; /* set by RowColumn when entryCallback
					is provided. */
	unsigned char   menu_type;
} XmLabelGCacheObjPart;

typedef struct _XmLabelGCacheObjRec
{
    ObjectPart 		     object;
    XmExtPart	             ext;
    XmLabelGCacheObjPart     label_cache;
} XmLabelGCacheObjRec;

/*  The Label Widget Class and instance records  */
/*************************************************/

typedef struct _XmLabelGadgetClassPart     /* label class record */
{
        XtWidgetProc			setOverrideCallback;
	XmMenuProc			menuProcs;
        caddr_t			extension; /* Pointer to extension record */
} XmLabelGadgetClassPart;

typedef struct _XmLabelGadgetClassRec
{
    RectObjClassPart       rect_class;
    XmGadgetClassPart      gadget_class;
    XmLabelGadgetClassPart label_class;
} XmLabelGadgetClassRec;

externalref XmLabelGadgetClassRec xmLabelGadgetClassRec;

typedef struct _XmLabelGadgetPart
{
	_XmString	_label;  /* String sent to this widget */
        _XmString       _acc_text;
    KeySym            mnemonic;
	XmStringCharSet mnemonicCharset;
        char 		*accelerator;
        XmFontList	font;

        Pixmap		pixmap; 
        Pixmap		pixmap_insen; 

        /* PRIVATE members -- values computed by LabelWidgetClass methods */

        GC		normal_GC;   /* GC for text */	
	GC		insensitive_GC;
        XRectangle	TextRect;    /* The bounding box of the text, or clip
                                        rectangle of the window; whichever is
                                        smaller */
        XRectangle	acc_TextRect; /* The bounding box of the text, or clip
                                        rectangle of the window; whichever is
                                        smaller */

	XmLabelGCacheObjPart *cache;
	
} XmLabelGadgetPart;


typedef struct _XmLabelGadgetRec
{
   ObjectPart        object;
   RectObjPart       rectangle;
   XmGadgetPart      gadget;
   XmLabelGadgetPart label;
} XmLabelGadgetRec;

/* Inherited  Functions exported by label */

#define XmInheritSetOverrideCallback ((XtProc) _XtInherit)
#define XmInheritResize  ((XtProc) _XtInherit)

/* Padding between label text and accelerator text */

#define LABELG_ACC_PAD		15

/* MACROS */
/********
 * Macros for cached instance fields
 */
#define LabG_LabelType(w)		(((XmLabelGadget)(w)) -> \
					   label.cache-> label_type)
#define LabG_Alignment(w)		(((XmLabelGadget)(w)) -> \
					   label.cache-> alignment)
#define LabG_StringDirection(w)		(((XmLabelGadget)(w)) -> \
					   label.cache-> string_direction)
#define LabG_MarginHeight(w)		(((XmLabelGadget)(w)) -> \
                                           label.cache-> margin_height)
#define LabG_MarginWidth(w)		(((XmLabelGadget)(w)) -> \
                                           label.cache-> margin_width)
#define LabG_MarginLeft(w)		(((XmLabelGadget)(w)) -> \
                                           label.cache-> margin_left)
#define LabG_MarginRight(w)		(((XmLabelGadget)(w)) -> \
                                           label.cache-> margin_right)
#define LabG_MarginTop(w)		(((XmLabelGadget)(w)) -> \
                                           label.cache-> margin_top)
#define LabG_MarginBottom(w)		(((XmLabelGadget)(w)) -> \
                                           label.cache-> margin_bottom)
#define LabG_RecomputeSize(w)		(((XmLabelGadget)(w)) -> \
                                           label.cache-> recompute_size)
#define LabG_SkipCallback(w)		(((XmLabelGadget)(w)) -> \
                                           label.cache-> skipCallback)
#define LabG_MenuType(w)		(((XmLabelGadget)(w)) -> \
                                           label.cache-> menu_type)
/********
 * Macros for UNcached instance fields
 */
#define LabG__label(w)			(((XmLabelGadget)(w)) -> \
					   label._label)
#define LabG__acceleratorText(w)	(((XmLabelGadget)(w)) -> \
                                           label._acc_text)
#define LabG_Font(w)			(((XmLabelGadget)(w)) -> \
					   label.font)
#define LabG_Mnemonic(w)		(((XmLabelGadget)(w)) -> \
					   label.mnemonic)
#define LabG_MnemonicCharset(w)         (((XmLabelGadget)(w)) -> \
                                           label.mnemonicCharset)
#define LabG_Accelerator(w)		(((XmLabelGadget)(w)) -> \
                                           label.accelerator)
#define LabG_Pixmap(w)			(((XmLabelGadget)(w)) -> \
                                           label.pixmap)
#define LabG_PixmapInsensitive(w)	(((XmLabelGadget)(w)) -> \
                                           label.pixmap_insen)
#define LabG_NormalGC(w)		(((XmLabelGadget)(w)) -> \
                                           label.normal_GC)
#define LabG_InsensitiveGC(w)		(((XmLabelGadget)(w)) -> \
                                           label.insensitive_GC)
#define LabG_TextRect(w)		(((XmLabelGadget)(w)) -> \
                                           label.TextRect)
#define LabG_AccTextRect(w)		(((XmLabelGadget)(w)) -> \
                                           label.acc_TextRect)


/********
 * Convenience Macros 
 */
#define LabG_TextRect_x(w)		(LabG_TextRect(w).x)
                                           
#define LabG_TextRect_y(w)		(LabG_TextRect(w).y)
                                           
#define LabG_TextRect_width(w)		(LabG_TextRect(w).width)
                                           
#define LabG_TextRect_height(w)		(LabG_TextRect(w).height)
                                           
#define LabG_IsText(w)			(LabG_LabelType(w) == XmSTRING)

#define LabG_IsPixmap(w)		(LabG_LabelType(w) == XmPIXMAP)

#define LabG_Cache(w)			(((XmLabelGadget)(w))-> \
					   label.cache)

#define LabG_ClassCachePart(w) \
	(((XmLabelGadgetClass)xmLabelGadgetClass)->gadget_class.cache_part)

#ifdef _NO_PROTO
void _XmReCacheLabG();
#else /* _NO_PROTO */
void _XmReCacheLabG (XmLabelGadget lw);
#endif /* _NO_PROTO */

#ifdef _NO_PROTO
void _XmAssignLabG_MarginHeight();
void _XmAssignLabG_MarginWidth();
void _XmAssignLabG_MarginTop();
void _XmAssignLabG_MarginLeft();
void _XmAssignLabG_MarginRight();
void _XmAssignLabG_MarginBottom();
#else /* _NO_PROTO */
void _XmAssignLabG_MarginWidth (XmLabelGadget lw, Dimension value);
void _XmAssignLabG_MarginHeight (XmLabelGadget lw, Dimension value);
void _XmAssignLabG_MarginRight (XmLabelGadget lw, Dimension value);
void _XmAssignLabG_MarginTop (XmLabelGadget lw, Dimension value);
void _XmAssignLabG_MarginLeft (XmLabelGadget lw, Dimension value);
void _XmAssignLabG_MarginBottom (XmLabelGadget lw, Dimension value);
#endif /* _NO_PROTO */

#endif /* _XmLabelGP_h */
/* DON'T ADD ANYTHING AFTER THIS #endif */
