#ifdef REV_INFO
#ifndef lint
static char SCCSID[] = "OSF/Motif: @(#)LabelP.h	3.6 91/01/10";
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
#ifndef _XmLabelP_h
#define _XmLabelP_h

#include <Xm/Label.h>
#include <Xm/XmP.h>

/*  The Label Widget Class and instance records  */

typedef struct _XmLabelClassPart     /* label class record */
{
        XtWidgetProc		setOverrideCallback;
	XmMenuProc		menuProcs;
        String  /* XtTranslations */  translations;
        caddr_t		extension; /* Pointer to extension record */
} XmLabelClassPart;

typedef struct _XmLabelClassRec
{
    CoreClassPart        core_class;
    XmPrimitiveClassPart primitive_class;
    XmLabelClassPart	 label_class;
} XmLabelClassRec;

extern XmLabelClassRec xmLabelClassRec;


/* Inherited  Functions exported by label */

#define XmInheritSetOverrideCallback ((XtProc) _XtInherit)
#define XmInheritResize  ((XtProc) _XtInherit)
#define XmInheritRealize  ((XtProc) _XtInherit)

/* The padding between label text and accelerator text */

# define LABEL_ACC_PAD 		15

/*  The Label instance record  */

typedef struct _XmLabelPart
{
	_XmString	_label;  /* String sent to this widget */
        _XmString	_acc_text;
	 KeySym		mnemonic;
/**        char            mnemonic;	**/
        XmStringCharSet mnemonicCharset;
        char 		*accelerator;
        unsigned char	label_type;
        unsigned char	alignment;
        unsigned char	string_direction;
        XmFontList	font;
	
	Dimension	margin_height;   /* margin around widget */
        Dimension	margin_width;

	Dimension 	margin_left;    /* additional margins on */
        Dimension	margin_right;   /* each side of widget */
        Dimension	margin_top;     /* text (or pixmap) is placed */
        Dimension	margin_bottom;  /* inside the margins */

        Boolean 	recompute_size;

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

	Boolean		skipCallback; /* set by RowColumn when entryCallback
					is provided. */
	unsigned char   menu_type;
} XmLabelPart;


typedef struct _XmLabelRec
{
   CorePart         core;
   XmPrimitivePart  primitive;
   XmLabelPart	    label;
} XmLabelRec;

/* MACROS */

#define Lab_MarginWidth(w)		(((XmLabelWidget)(w)) ->label.margin_width)
#define Lab_MarginHeight(w)		(((XmLabelWidget)(w)) ->label.margin_height)
#define Lab_MarginTop(w)		(((XmLabelWidget)(w)) ->label.margin_top)
#define Lab_MarginBottom(w)		(((XmLabelWidget)(w)) ->label.margin_bottom)
#define Lab_MarginRight(w)		(((XmLabelWidget)(w)) ->label.margin_right)
#define Lab_MarginLeft(w)		(((XmLabelWidget)(w)) ->label.margin_left)
#define Lab_TextRect_x(w)		(((XmLabelWidget)(w)) ->label.TextRect.x)
#define Lab_TextRect_y(w)		(((XmLabelWidget)(w)) ->label.TextRect.y)
#define Lab_TextRect_width(w)		(((XmLabelWidget)(w)) ->label.TextRect.width)
#define Lab_TextRect_height(w)		(((XmLabelWidget)(w)) ->label.TextRect.height)

#define Lab_IsText(w)			(((XmLabelWidget)(w)) ->label.label_type == XmSTRING)
#define Lab_IsPixmap(w)			(((XmLabelWidget)(w)) ->label.label_type == XmPIXMAP)

#define Lab_Font(w)			(((XmLabelWidget)(w)) ->label.font)

#define Lab_Mnemonic(w)			(((XmLabelWidget)(w)) ->label.mnemonic)
#define Lab_Accelerator(w)		(((XmLabelWidget)(w)) ->label.accelerator)
#define Lab_AcceleratorText(w)		(((XmLabelWidget)(w)) ->label.acc_text)
#define Lab_MenuType(w)			(((XmLabelWidget)(w))->label.menu_type)

#endif /* _XmLabelP_h */
/* DON'T ADD ANYTHING AFTER THIS #endif */
