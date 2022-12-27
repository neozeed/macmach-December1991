#ifdef REV_INFO
#ifndef lint
static char SCCSID[] = "OSF/Motif: @(#)List.h	3.8 91/01/10";
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
#ifndef _XmList_h
#define _XmList_h

#include <Xm/Xm.h>

externalref WidgetClass xmListWidgetClass;

#define XmINITIAL 	0
#define XmADDITION	1
#define XmMODIFICATION	2

#ifdef _NO_PROTO
extern void XmListAddItem ();
extern void XmListAddItems ();
extern void XmListAddItemUnselected ();
extern void XmListDeleteItem ();
extern void XmListDeleteItems ();
extern void XmListDeletePos ();
extern void XmListDeleteItemsPos ();
extern void XmListDeleteAllItems ();
extern void XmListReplaceItems ();
extern void XmListReplaceItemsPos ();
extern void XmListSelectItem ();
extern void XmListSelectPos ();
extern void XmListDeselectItem ();
extern void XmListDeselectPos ();
extern void XmListDeselectAllItems ();
extern void XmListSetPos ();
extern void XmListSetBottomPos ();
extern void XmListSetItem ();
extern void XmListSetBottomItem ();
extern void XmListSetAddMode ();
extern Boolean XmListItemExists ();
extern int XmListItemPos ();
extern Boolean XmListGetMatchPos ();
extern Boolean XmListGetSelectedPos ();
extern void XmListSetHorizPos ();
extern Widget XmCreateList ();
extern Widget XmCreateScrolledList ();
#else /* _NO_PROTO */

#if defined(__cplusplus) || defined(c_plusplus)
extern "C" {
#endif

extern void XmListAddItem (Widget w, XmString item, int pos);
extern void XmListAddItems (Widget w, XmString *items, int item_count, int pos);
extern void XmListAddItemUnselected (Widget w, XmString item, int pos);
extern void XmListDeleteItem (Widget w, XmString item);
extern void XmListDeleteItems (Widget w, XmString *items, int item_count);
extern void XmListDeletePos (Widget w, int pos);
extern void XmListDeleteItemsPos (Widget w, int item_count, int pos);
extern void XmListDeleteAllItems (Widget w);
extern void XmListReplaceItems (Widget w, XmString *old_items, int item_count, XmString *new_items);
extern void XmListReplaceItemsPos (Widget w, XmString *new_items, int item_count, int position);
extern void XmListSelectItem (Widget w, XmString item,  
#if NeedWidePrototypes
int notify
#else
Boolean notify
#endif 
);
extern void XmListSelectPos (Widget w, int pos,  
#if NeedWidePrototypes
int notify
#else
Boolean notify
#endif 
);
extern void XmListDeselectItem (Widget w, XmString item);
extern void XmListDeselectPos (Widget w, int pos);
extern void XmListDeselectAllItems (Widget w);
extern void XmListSetPos (Widget w, int pos);
extern void XmListSetBottomPos (Widget w, int pos);
extern void XmListSetItem (Widget w, XmString item);
extern void XmListSetBottomItem (Widget w, XmString item);
extern void XmListSetAddMode (Widget w,    
#if NeedWidePrototypes
int add_mode
#else
Boolean add_mode
#endif 
);
extern Boolean XmListItemExists (Widget w, XmString item);
extern int XmListItemPos (Widget w, XmString item);
extern Boolean XmListGetMatchPos (Widget w, XmString item, int **pos_list, int *pos_count);
extern Boolean XmListGetSelectedPos (Widget w, int **pos_list, int *pos_count);
extern void XmListSetHorizPos (Widget w, int position);
extern Widget XmCreateList (Widget parent, char *name, ArgList args, Cardinal argCount);
extern Widget XmCreateScrolledList (Widget parent, char *name, ArgList args, Cardinal argCount);
#endif /* _NO_PROTO */

#if defined(__cplusplus) || defined(c_plusplus)
}
#endif

#ifndef XmIsList
#define XmIsList(w)	XtIsSubclass(w, xmListWidgetClass)
#endif /* XmIsList */

typedef struct _XmListClassRec * XmListWidgetClass;
typedef struct _XmListRec      * XmListWidget;

#endif /* _XmList_h */
/* DON'T ADD ANYTHING AFTER THIS #endif */
