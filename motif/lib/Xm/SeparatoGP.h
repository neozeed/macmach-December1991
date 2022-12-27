#ifdef REV_INFO
#ifndef lint
static char SCCSID[] = "OSF/Motif: @(#)SeparatoGP.h	3.4 90/04/24";
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
#ifndef _XmSeparatorGadgetP_h
#define _XmSeparatorGadgetP_h

#include <Xm/SeparatoG.h>
#include <Xm/ExtObjectP.h>
#include <Xm/XmP.h>

/*****************************************************************/
/* The Separator Gadget Cache Object's class and instance records*/
/*****************************************************************/

typedef struct _XmSeparatorGCacheObjClassPart
{
    int foo;
} XmSeparatorGCacheObjClassPart;


typedef struct _XmSeparatorGCacheObjClassRec  /* separator cache class record */
{
    ObjectClassPart                     object_class;
    XmExtClassPart                      ext_class;
    XmSeparatorGCacheObjClassPart       separator_class_cache;
} XmSeparatorGCacheObjClassRec;

externalref XmSeparatorGCacheObjClassRec xmSeparatorGCacheObjClassRec;


/*  The Separator Gadget Cache instance record  */

typedef struct _XmSeparatorGCacheObjPart
{
   Dimension	  margin;
   unsigned char  orientation;
   unsigned char  separator_type;
   GC             separator_GC;
} XmSeparatorGCacheObjPart;

typedef struct _XmSeparatorGCacheObjRec
{
  ObjectPart		    object;
  XmExtPart		    ext;
  XmSeparatorGCacheObjPart  separator_cache;
} XmSeparatorGCacheObjRec;


/*****************************************************/
/*  The Separator Widget Class and instance records  */
/*****************************************************/

typedef struct _XmSeparatorGadgetClassPart
{
   caddr_t                 extension;
} XmSeparatorGadgetClassPart;


/*  Full class record declaration for Separator class  */

typedef struct _XmSeparatorGadgetClassRec
{
   RectObjClassPart            rect_class;
   XmGadgetClassPart           gadget_class;
   XmSeparatorGadgetClassPart  separator_class;
} XmSeparatorGadgetClassRec;

externalref XmSeparatorGadgetClassRec xmSeparatorGadgetClassRec;

typedef struct _XmSeparatorGadgetPart
{
  XmSeparatorGCacheObjPart  *cache;
} XmSeparatorGadgetPart;

/*  Full instance record declaration  */

typedef struct _XmSeparatorGadgetRec
{
   ObjectPart             object;
   RectObjPart            rectangle;
   XmGadgetPart           gadget;
   XmSeparatorGadgetPart  separator;
} XmSeparatorGadgetRec;

/* MACROS for accessing instance fields*/
#define SEPG_Margin(w)			(((XmSeparatorGadget)(w))->   \
					   separator.cache->margin)
#define SEPG_Orientation(w)		(((XmSeparatorGadget)(w))->   \
					   separator.cache->orientation)
#define SEPG_SeparatorType(w)		(((XmSeparatorGadget)(w))->   \
					   separator.cache->separator_type)
#define SEPG_SeparatorGC(w)		(((XmSeparatorGadget)(w))->   \
					   separator.cache->separator_GC)

/* Convenience Macros */
#define SEPG_Cache(w)                    (((XmSeparatorGadget)(w))->\
					   separator.cache)
#define SEPG_ClassCachePart(w) \
        (((XmSeparatorGadgetClass)xmSeparatorGadgetClass)->gadget_class.cache_part)


#endif /* _XmSeparatorGadgetP_h */
/* DON'T ADD STUFF AFTER THIS #endif */
