#ifdef REV_INFO
#ifndef lint
static char SCCSID[] = "OSF/Motif: @(#)VendorE.h	3.6 90/06/29";
#endif /* lint */
#endif /* REV_INFO */
/******************************************************************************
*******************************************************************************
*
*  (c) Copyright 1989, 1990  OPEN SOFTWARE FOUNDATION, INC.
*  (c) Copyright 1989, 1990  DIGITAL EQUIPMENT CORPORATION, MAYNARD, MASS.
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
*  (c) Copyright 1989, 1990  Open Software Foundation, Inc.  Unpublished - all
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
*  (c) Copyright 1989, 1990  Open Software Foundation, Inc.
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
#ifndef _VendorE_h
#define _VendorE_h

#include <Xm/ExtObject.h>
#include <X11/Shell.h>
#include <X11/Vendor.h>

/* DeleteResponse values */
#define XmDESTROY 	0
#define XmUNMAP 	1
#define XmDO_NOTHING	2

#ifndef XmNdefaultFontList
#define XmNdefaultFontList 	"defaultFontList"
#define XmCDefaultFontList	"DefaultFontList"
#endif

#define XmNdeleteResponse	"deleteResponse"
#define XmCDeleteResponse	"DeleteResponse"
#define XmRDeleteResponse	"DeleteResponse"

#define XtNkeyboardFocusPolicy   "keyboardFocusPolicy"
#define XtCKeyboardFocusPolicy   "KeyboardFocusPolicy"
#define XtRKeyboardFocusPolicy   "KeyboardFocusPolicy"

#define XtNshellUnitType		"shellUnitType"
#define XtCShellUnitType		"ShellUnitType"
#define XtRShellUnitType		"ShellUnitType"

#define XmRHorizontalInt	"HorizontalInt"
#define XmRVerticalInt		"VerticalInt"
#define XmRShellHorizDim	"ShellHorizDim"
#define XmRShellHorizPos	"ShellHorizPos"
#define XmRShellVertDim		"ShellVertDim"
#define XmRShellVertPos		"ShellVertPos"



#define XmNkeyboardFocusPolicy   "keyboardFocusPolicy"
#define XmCKeyboardFocusPolicy   "KeyboardFocusPolicy"
#define XmRKeyboardFocusPolicy   "KeyboardFocusPolicy"

#define XmNfocusPolicyChanged	 "focusPolicyChanged"

#define XmNshellUnitType		"shellUnitType"
#define XmCShellUnitType		"ShellUnitType"
#define XmRShellUnitType		"ShellUnitType"

#ifndef XmEXPLICIT 
#define XmEXPLICIT 0 
#define XmPOINTER 1 
#endif /* XmEXPLICIT */

#define XmNmwmDecorations       "mwmDecorations"
#define XmCMwmDecorations       "MwmDecorations"
#define XmNmwmFunctions         "mwmFunctions"
#define XmCMwmFunctions         "MwmFunctions"
#define XmNmwmInputMode         "mwmInputMode"
#define XmCMwmInputMode         "MwmInputMode"
#define XmNmwmMenu	        "mwmMenu"
#define XmCMwmMenu	        "MwmMenu"
#define XmNmwmMessages          "mwmMessages"
#define XmCMwmMessages          "MwmMessages"

/* internal callback lists */
#define XmNfocusMovedCallback	 "focusMovedCallback"
#define XmNrealizeCallback	 "realizeCallback"

#define XmNdesktopParent	"desktopParent"
#define XmCDesktopParent	"DesktopParent"

#define XmNuseAsyncGeometry	"useAsyncGeometry"
#define XmCUseAsyncGeometry	"UseAsyncGeometry"

#ifndef XmIsDesktopObject
#define XmIsDesktopObject(w)	XtIsSubclass(w, xmDesktopObjectClass)
#endif /* XmIsDesktopObject */

#ifndef XmIsDisplayObject
#define XmIsDisplayObject(w)	XtIsSubclass(w, xmDisplayObjectClass)
#endif /* XmIsDisplayObject */

#ifndef XmIsScreenObject
#define XmIsScreenObject(w)	XtIsSubclass(w, xmScreenObjectClass)
#endif /* XmIsScreenObject */

#ifndef XmIsWorldObject
#define XmIsWorldObject(w)	XtIsSubclass(w, xmWorldObjectClass)
#endif /* XmIsWorldObject */

#ifndef XmIsShellExt
#define XmIsShellExt(w)	XtIsSubclass(w, xmShellExtObjectClass)
#endif /* XmIsShellExt */

#ifndef XmIsVendorShellExt
#define XmIsVendorShellExt(w)	XtIsSubclass(w, xmVendorShellExtObjectClass)
#endif /* XmIsVendorShellExt */

#ifndef XmIsVendorShell
#define XmIsVendorShell(w)	XtIsSubclass(w, vendorShellWidgetClass)
#endif /* XmIsVendorShell */

/* Class record constants */

typedef struct _XmDesktopRec *XmDesktopObject;
typedef struct _XmDesktopClassRec *XmDesktopObjectClass;
externalref WidgetClass xmDesktopObjectClass;

typedef struct _XmWorldRec *XmWorldObject;
typedef struct _XmWorldClassRec *XmWorldObjectClass;
externalref WidgetClass xmWorldObjectClass;

typedef struct _XmDisplayRec *XmDisplayObject;
typedef struct _XmDisplayClassRec *XmDisplayObjectClass;
externalref WidgetClass xmDisplayObjectClass;

typedef struct _XmScreenRec *XmScreenObject;
typedef struct _XmScreenClassRec *XmScreenObjectClass;
externalref WidgetClass xmScreenObjectClass;

typedef struct _XmShellExtRec *XmShellExtObject;
typedef struct _XmShellExtClassRec *XmShellExtObjectClass;
externalref WidgetClass xmShellExtObjectClass;

typedef struct _XmVendorShellExtRec *XmVendorShellExtObject;
typedef struct _XmVendorShellExtClassRec *XmVendorShellExtObjectClass;
externalref WidgetClass xmVendorShellExtObjectClass;

#ifdef _NO_PROTO
extern Boolean XmIsMotifWMRunning();
#else /* _NO_PROTO */
#if defined(__cplusplus) || defined(c_plusplus)
extern "C" {
#endif
extern Boolean XmIsMotifWMRunning(Widget shell);
#endif /* _NO_PROTO */
#if defined(__cplusplus) || defined(c_plusplus)
}
#endif
#endif /* _VendorE_h */
/* DON'T ADD STUFF AFTER THIS #endif */
