#ifdef REV_INFO
#ifndef lint
static char SCCSID[] = "OSF/Motif: @(#)TextF.h	3.11 91/01/10";
#endif /* lint */
#endif /* REV_INFO */
/******************************************************************************
*******************************************************************************
*
*  (c) Copyright 1989, 1990, 1991 OPEN SOFTWARE FOUNDATION, INC.
*  (c) Copyright 1989, 1990, HEWLETT-PACKARD COMPANY
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
* X Window System is a trademark of the Massachusetts Institute of Technology
*
*******************************************************************************
******************************************************************************/
#ifndef _XmTextF_h
#define _XmTextF_h

#include <Xm/Xm.h>

/******************
 * type defines   *
 ******************/
typedef struct _XmTextFieldClassRec *XmTextFieldWidgetClass;
typedef struct _XmTextFieldRec *XmTextFieldWidget;

/******************
 * extern class   *
 ******************/
externalref WidgetClass       xmTextFieldWidgetClass;

/********************
 * public functions *
 ********************/

#ifdef _NO_PROTO
extern char *XmTextFieldGetString ();
extern XmTextPosition XmTextFieldGetLastPosition ();
extern void XmTextFieldSetString ();
extern void XmTextFieldReplace ();
extern void XmTextFieldInsert ();
extern void XmTextFieldSetAddMode ();
extern Boolean XmTextFieldGetAddMode ();
extern Boolean XmTextFieldGetEditable ();
extern void XmTextFieldSetEditable ();
extern int XmTextFieldGetMaxLength ();
extern void XmTextFieldSetMaxLength ();
extern XmTextPosition XmTextFieldGetCursorPosition ();
extern XmTextPosition XmTextFieldGetInsertionPosition ();
extern void XmTextFieldSetCursorPosition ();
extern void XmTextFieldSetInsertionPosition ();
extern Boolean XmTextFieldGetSelectionPosition ();
extern char *XmTextFieldGetSelection ();
extern Boolean XmTextFieldRemove ();
extern Boolean XmTextFieldCopy ();
extern Boolean XmTextFieldCut ();
extern Boolean XmTextFieldPaste ();
extern void XmTextFieldClearSelection ();
extern void XmTextFieldSetSelection ();
extern XmTextPosition XmTextFieldXYToPos ();
extern Boolean XmTextFieldPosToXY ();
extern void XmTextFieldShowPosition ();
extern void XmTextFieldSetHighlight ();
extern int XmTextFieldGetBaseLine ();
extern int XmTextFieldGetBaseline ();
extern Widget XmCreateTextField ();
#else /* _NO_PROTO */
#if defined(__cplusplus) || defined(c_plusplus)
extern "C" {
#endif

extern char *XmTextFieldGetString (Widget widget);
extern XmTextPosition XmTextFieldGetLastPosition (Widget widget);
extern void XmTextFieldSetString (Widget widget, char *value);
extern void XmTextFieldReplace (Widget widget, XmTextPosition from_pos, XmTextPosition to_pos, char *value);
extern void XmTextFieldInsert (Widget widget, XmTextPosition position, char *value);
extern void XmTextFieldSetAddMode (Widget widget,    
#if NeedWidePrototypes
int state
#else
Boolean state
#endif 
);
extern Boolean XmTextFieldGetAddMode (Widget widget);
extern Boolean XmTextFieldGetEditable (Widget widget);
extern void XmTextFieldSetEditable (Widget widget,     
#if NeedWidePrototypes
int editable
#else
Boolean editable
#endif 
);
extern int XmTextFieldGetMaxLength (Widget widget);
extern void XmTextFieldSetMaxLength (Widget widget, int max_length);
extern XmTextPosition XmTextFieldGetCursorPosition (Widget widget);
extern void XmTextFieldSetCursorPosition (Widget widget, XmTextPosition position);
extern XmTextPosition XmTextFieldGetInsertionPosition (Widget widget);
extern void XmTextFieldSetInsertionPosition (Widget widget, XmTextPosition position);
extern Boolean XmTextFieldGetSelectionPosition (Widget widget, XmTextPosition *left, XmTextPosition *right);
extern char *XmTextFieldGetSelection (Widget widget);
extern Boolean XmTextFieldRemove (Widget widget);
extern Boolean XmTextFieldCopy (Widget widget, Time clip_time);
extern Boolean XmTextFieldCut (Widget widget, Time clip_time);
extern Boolean XmTextFieldPaste (Widget widget);
extern void XmTextFieldClearSelection (Widget widget, Time sel_time);
extern void XmTextFieldSetSelection (Widget widget, XmTextPosition first, XmTextPosition last, Time sel_time);
extern XmTextPosition XmTextFieldXYToPos (Widget widget,      
#if NeedWidePrototypes
int x, int y
#else
Position x, Position y
#endif 
);
extern Boolean XmTextFieldPosToXY (Widget widget, XmTextPosition position, Position *x, Position *y);
extern void XmTextFieldShowPosition (Widget widget, XmTextPosition position);
extern void XmTextFieldSetHighlight (Widget widget, XmTextPosition left, XmTextPosition right, XmHighlightMode mode);
extern int XmTextFieldGetBaseLine (Widget widget);
extern int XmTextFieldGetBaseline (Widget widget);
extern Widget XmCreateTextField (Widget parent, char *name, ArgList arglist, Cardinal argcount);
#endif /* _NO_PROTO */

#if defined(__cplusplus) || defined(c_plusplus)
}
#endif

/************************
 * fast subclass define *
 ************************/
#ifndef XmIsTextField
#define XmIsTextField(w)     XtIsSubclass(w, xmTextFieldWidgetClass)
#endif /* XmIsTextField */

#endif /* _XmTextF_h */
/* DON'T ADD STUFF AFTER THIS #endif */
