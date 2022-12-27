#ifdef REV_INFO
#ifndef lint
static char SCCSID[] = "OSF/Motif: @(#)DialogSP.h	3.3 90/03/28";
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
#ifndef _XmDialogShellP_h
#define _XmDialogShellP_h

#include <Xm/DialogS.h>
#include <Xm/XmP.h>
#include <X11/ShellP.h>
#include <Xm/VendorEP.h>

/* dialog class name */
#define XmCDialogShell		"XmDialogShell"

/* macros: THESE BELONG IN XmP.h */
#define XtX(w)		 ((w)->core.x)
#define XtY(w)		 ((w)->core.y)
#define XtWidth(w)	 ((w)->core.width)
#define XtHeight(w)	 ((w)->core.height)
#define XtBorderWidth(w) ((w)->core.border_width)
#define XtBackground(w)	 ((w)->core.background_pixel)
#define XtSensitive(w)	 ((w)->core.sensitive && (w)->core.ancestor_sensitive) 

#ifndef XtParent
#define XtParent(w)	 ((w)->core.parent)
#endif


typedef struct _XmDialogShellExtClassPart{
    int			empty;
}XmDialogShellExtClassPart, *XmDialogShellExtClassPartPtr;

typedef struct _XmDialogShellExtClassRec{
    ObjectClassPart		object_class;
    XmExtClassPart		ext_class;
    XmDesktopClassPart		desktop_class;
    XmShellExtClassPart		shell_class;
    XmVendorShellExtClassPart 	vendor_class;
    XmDialogShellExtClassPart 	dialog_class;
}XmDialogShellExtClassRec, *XmDialogShellExtObjectClass;

typedef struct _XmDialogShellExtPart{
    int		      	empty;
} XmDialogShellExtPart;

externalref XmDialogShellExtClassRec 	xmDialogShellExtClassRec;

typedef struct _XmDialogShellExtRec{
    ObjectPart			object;
    XmExtPart			ext;
    XmDesktopPart		desktop;
    XmShellExtPart		shell;
    XmVendorShellExtPart 	vendor;
    XmDialogShellExtPart 	dialog;
}XmDialogShellExtRec, *XmDialogShellExtObject;


/* The DialogShell instance record */

typedef struct 
{
    /* internal fields */
    XtGrabKind 		grab_kind;
    Position		old_x, old_y;
} XmDialogShellPart;


/* Full instance record declaration */

typedef  struct _XmDialogShellWidgetRec 
{	
    CorePart		    core;
    CompositePart	    composite;
    ShellPart		    shell;
    WMShellPart		    wm;
    VendorShellPart	    vendor;
    TransientShellPart	    transient;
    XmDialogShellPart	    dialog;
} XmDialogShellWidgetRec;



/* DialogShell class structure */

typedef struct 
{
    XtPointer			extension;	 /* Pointer to extension record */
} XmDialogShellClassPart;


/* Full class record declaration */

typedef struct _XmDialogShellClassRec 
{
    CoreClassPart 		core_class;
    CompositeClassPart 		composite_class;
    ShellClassPart 		shell_class;
    WMShellClassPart	        wm_shell_class;
    VendorShellClassPart 	vendor_shell_class;
    TransientShellClassPart  	transient_shell_class;
    XmDialogShellClassPart 	dialog_shell_part;
} XmDialogShellClassRec;


externalref XmDialogShellClassRec  xmDialogShellClassRec;

#endif /* _XmDialogShellP_h */
/* DON'T ADD STUFF AFTER THIS #endif */
