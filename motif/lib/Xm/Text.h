#ifdef REV_INFO
#ifndef lint
static char SCCSID[] = "OSF/Motif: @(#)Text.h	3.11 91/01/10";
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
#ifndef _XmText_h
#define _XmText_h

#include <Xm/Xm.h>

/* -------------- *
 *   type defines *
 * -------------- */
typedef struct _XmTextSourceRec *XmTextSource;
typedef struct _XmTextClassRec *XmTextWidgetClass;
typedef struct _XmTextRec *XmTextWidget;

/* -------------- *
 * extern class   *
 * -------------- */
externalref WidgetClass       xmTextWidgetClass;


/* ----------------------------------- *
 *   text widget public functions      *
 * ----------------------------------- */

#ifdef _NO_PROTO
extern void XmTextSetHighlight ();
extern Widget XmCreateScrolledText ();
extern Widget XmCreateText ();
extern char *XmTextGetString ();
extern XmTextPosition XmTextGetLastPosition ();
extern void XmTextSetString ();
extern void XmTextReplace ();
extern void XmTextInsert ();
extern void XmTextSetAddMode ();
extern Boolean XmTextGetAddMode ();
extern Boolean XmTextGetEditable ();
extern void XmTextSetEditable ();
extern int XmTextGetMaxLength ();
extern void XmTextSetMaxLength ();
extern XmTextPosition XmTextGetTopCharacter ();
extern void XmTextSetTopCharacter ();
extern XmTextPosition XmTextGetCursorPosition ();
extern XmTextPosition XmTextGetInsertionPosition ();
extern void XmTextSetCursorPosition ();
extern void XmTextSetInsertionPosition ();
extern Boolean XmTextRemove ();
extern Boolean XmTextCopy ();
extern Boolean XmTextCut ();
extern Boolean XmTextPaste ();
extern char *XmTextGetSelection ();
extern void XmTextSetSelection ();
extern void XmTextClearSelection ();
extern Boolean XmTextGetSelectionPosition ();
extern XmTextPosition XmTextXYToPos ();
extern Boolean XmTextPosToXY ();
extern XmTextSource XmTextGetSource ();
extern void XmTextSetSource ();
extern void XmTextShowPosition ();
extern void XmTextScroll ();
extern int XmTextGetBaseLine ();
extern int XmTextGetBaseline ();
#else /* _NO_PROTO */

#if defined(__cplusplus) || defined(c_plusplus)
extern "C" {
#endif

extern void XmTextSetHighlight (Widget w, XmTextPosition left, XmTextPosition right, XmHighlightMode mode);
extern Widget XmCreateScrolledText (Widget parent, char *name, ArgList arglist, Cardinal argcount);
extern Widget XmCreateText (Widget parent, char *name, ArgList arglist, Cardinal argcount);
extern char *XmTextGetString (Widget widget);
extern XmTextPosition XmTextGetLastPosition (Widget widget);
extern void XmTextSetString (Widget widget, char *value);
extern void XmTextReplace (Widget widget, XmTextPosition frompos, XmTextPosition topos, char *value);
extern void XmTextInsert (Widget widget, XmTextPosition position, char *value);
extern void XmTextSetAddMode (Widget widget,   
#if NeedWidePrototypes
int state
#else
Boolean state
#endif 
);
extern Boolean XmTextGetAddMode (Widget widget);
extern Boolean XmTextGetEditable (Widget widget);
extern void XmTextSetEditable (Widget widget,    
#if NeedWidePrototypes
int editable
#else
Boolean editable
#endif 
);
extern int XmTextGetMaxLength (Widget widget);
extern void XmTextSetMaxLength (Widget widget, int max_length);
extern XmTextPosition XmTextGetTopCharacter (Widget widget);
extern void XmTextSetTopCharacter (Widget widget, XmTextPosition top_character);
extern XmTextPosition XmTextGetCursorPosition (Widget widget);
extern void XmTextSetCursorPosition (Widget widget, XmTextPosition position);
extern XmTextPosition XmTextGetInsertionPosition (Widget widget);
extern void XmTextSetInsertionPosition (Widget widget, XmTextPosition position);
extern Boolean XmTextRemove (Widget widget);
extern Boolean XmTextCopy (Widget widget, Time copy_time);
extern Boolean XmTextCut (Widget widget, Time cut_time);
extern Boolean XmTextPaste (Widget widget);
extern char *XmTextGetSelection (Widget widget);
extern void XmTextSetSelection (Widget widget, XmTextPosition first, XmTextPosition last, Time set_time);
extern void XmTextClearSelection (Widget widget, Time clear_time);
extern Boolean XmTextGetSelectionPosition (Widget widget, XmTextPosition *left, XmTextPosition *right);
extern XmTextPosition XmTextXYToPos (Widget widget,     
#if NeedWidePrototypes
int x, int y
#else
Position x, Position y
#endif 
);
extern Boolean XmTextPosToXY (Widget widget, XmTextPosition position, Position *x, Position *y);
extern XmTextSource XmTextGetSource (Widget widget);
extern void XmTextSetSource (Widget widget, XmTextSource source, XmTextPosition top_character, XmTextPosition cursor_position);
extern void XmTextShowPosition (Widget widget, XmTextPosition position);
extern void XmTextScroll (Widget widget, int n);
extern int XmTextGetBaseLine (Widget widget);
extern int XmTextGetBaseline (Widget widget);
#endif /* _NO_PROTO */

#if defined(__cplusplus) || defined(c_plusplus)
}
#endif

/* --------------------------------------- *
 *  text widget fast subclassing fallback  *
 * --------------------------------------- */

#ifndef XmIsText
#define XmIsText(w)	XtIsSubclass(w, xmTextWidgetClass)
#endif /* XmIsText */

#endif /* _XmText_h */
/* DON'T ADD STUFF AFTER THIS #endif */
