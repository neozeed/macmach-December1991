#ifdef REV_INFO
#ifndef lint
static char SCCSID[] = "OSF/Motif: @(#)TraversalI.h	3.7 90/07/03";
#endif /* lint */
#endif /* REV_INFO */
/******************************************************************************
*******************************************************************************
*
*  (c) Copyright 1990, OPEN SOFTWARE FOUNDATION, INC.
*  (c) Copyright 1990, HEWLETT-PACKARD COMPANY
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
* X Window System is a trademark of the Massachusetts Institute of Technology
*
*******************************************************************************
******************************************************************************/
#ifndef _XmTraversalI_h
#define _XmTraversalI_h

#define XmTAB_ANY	255
#define XmNONE_OR_BC	254

#define NavigFieldCopy(to, from, field)\
  if ((to)->field) *((to)->field) = *((from)->field)

#define NavigPtrsCopy(to, from)\
NavigFieldCopy(to, from, traversal_on); \
   NavigFieldCopy(to, from, ancestor_sensitive); \
     NavigFieldCopy(to, from, sensitive);\
       NavigFieldCopy(to, from, mapped_when_managed)

#define NavigFieldEq(np, field, value) \
  (((np)->field) ? (*(np->field) == value) : False)

#define NavigFieldsEq(to, from, field) \
  (((to)->field) ? ((*((to)->field)) == (*((from)->field))) : True)

#define NavigFieldGet(np, field) \
  (((np)->field) ? (*((np)->field)) : NULL)

#define NavigFieldSet(np, field, value) \
 if ((np)->field) *((np)->field) = value

#define _XmIsTabGroup(navigation_type) \
  ((navigation_type == XmTAB_GROUP) || \
   (navigation_type == XmSTICKY_TAB_GROUP) || \
   (navigation_type == XmEXCLUSIVE_TAB_GROUP))

#define NavigIsTabGroup(np) \
  (NavigFieldEq(np, navigation_type, XmTAB_GROUP) || \
   NavigFieldEq(np, navigation_type, XmSTICKY_TAB_GROUP) || \
   NavigFieldEq(np, navigation_type, XmEXCLUSIVE_TAB_GROUP))

typedef enum {
    HereOnly,
    AboveOnly,
    BelowOnly,
    AboveAndBelow,
} XmNavigTestType;

typedef struct _XmNavigMapEntryRec{
    Widget	widget;
}XmNavigMapEntryRec, *XmNavigMapEntry;

typedef struct _XmNavigMapRec{
    XmNavigMapEntry	entries;
    Cardinal		numEntries;
    unsigned short	*vertList, *horizList;
}XmNavigMapRec, *XmNavigMap;

typedef struct _WidgetNavigPtrsRec{
    Boolean	*traversal_on;
    Boolean	*have_traversal;
    Boolean	*sensitive;
    Boolean	*ancestor_sensitive;
    Boolean	*mapped_when_managed;
    Boolean	*highlighted;
    Boolean	*managed;
    unsigned char *navigation_type;
}WidgetNavigPtrsRec, *WidgetNavigPtrs;

typedef struct _XmNavigIMapEntryRec	*XmNavigIMapEntry;

typedef struct _XmNavigIMapEntryRec{
    Widget		widget;
    XRectangle		visRect;
    Cardinal		level;
    unsigned char 	flags;
}XmNavigIMapEntryRec;

typedef struct _XmNavigIMapRec{
    XmNavigIMapEntry	entries;
    Cardinal		numEntries, maxEntries;
}XmNavigIMapRec, *XmNavigIMap;

#ifdef _NO_PROTO
extern Boolean _XmWidgetIsTraversable ();
extern Boolean _XmPathIsTraversable ();
extern void GetWidgetNavigPtrs();
extern Widget _XmNavigate();
#else /* _NO_PROTO */
extern Boolean _XmWidgetIsTraversable (Widget widget, WidgetNavigPtrs np, XmNavigationType navType, XRectangle *visRect);
extern Boolean _XmPathIsTraversable (Widget widget, XmNavigationType navType, XmNavigTestType testType, XRectangle *visRect);
extern Widget _XmNavigate (Widget w, int direction);
extern void GetWidgetNavigPtrs (Widget widget, WidgetNavigPtrs np);
#endif /* _NO_PROTO */

#endif /* _XmTraversalI_h */
