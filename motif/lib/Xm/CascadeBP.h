#ifdef REV_INFO
#ifndef lint
static char SCCSID[] = "OSF/Motif: @(#)CascadeBP.h	3.8 91/01/10";
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
#ifndef  _XmCascadeBP_h
#define  _XmCascadeBP_h

#include <Xm/CascadeB.h>
#include <Xm/XmP.h>

typedef struct _XmArrowPixmap
{
   Dimension height, width;
   unsigned int depth;
   Pixel top_shadow_color, bottom_shadow_color, foreground_color;
   Display *display;
   Pixmap pixmap;
} XmArrowPixmap;

/* The CascadeButton instance record */

typedef	struct 
{				/* resources */
    XtCallbackList	activate_callback;	/* widget fired callback */
    XtCallbackList	cascade_callback;	/* called when the menu is  */
						/* about to be pulled down */

    Widget		submenu;		/* the menu to pull down */
    Pixmap		cascade_pixmap;		/* pixmap for the cascade */
    int 		map_delay;		/* time delay for posting */

				/* internal fields */

    Boolean		armed;			/* armed flag */
    XRectangle		cascade_rect;		/* location of cascade*/
    XtIntervalId	timer;			/* timeout id */
    Pixmap		armed_pixmap;		/* arm arrow cascade */

} XmCascadeButtonPart;


/* Full instance record declaration */

typedef struct _XmCascadeButtonWidgetRec
{
    CorePart		core;
    XmPrimitivePart	primitive;
    XmLabelPart		label;
    XmCascadeButtonPart	cascade_button;
} XmCascadeButtonWidgetRec;



/* CascadeButton class structure */

typedef struct 
{
    caddr_t	extension;	/* Pointer to extension record */
} XmCascadeButtonClassPart;


/* Full class record declaration for CascadeButton class */

typedef struct _XmCascadeButtonClassRec 
{
    CoreClassPart	    core_class;
    XmPrimitiveClassPart    primitive_class;
    XmLabelClassPart	    label_class;
    XmCascadeButtonClassPart cascade_button_class;
} XmCascadeButtonClassRec;


extern XmCascadeButtonClassRec   xmCascadeButtonClassRec;

/* Access macro definitions */

#define CB_Submenu(cb)		(((XmCascadeButtonWidget)                    \
                                  cb)->cascade_button.submenu)

#define CB_ActivateCall(cb)	(((XmCascadeButtonWidget)                    \
                                  cb)->cascade_button.activate_callback)
#define CB_CascadeCall(cb)	(((XmCascadeButtonWidget)                    \
                                  cb)->cascade_button.cascade_callback)


#define CB_CascadePixmap(cb)	(((XmCascadeButtonWidget)                    \
                                  cb)->cascade_button.cascade_pixmap)
#define CB_ArmedPixmap(cb)	(((XmCascadeButtonWidget)                    \
                                  cb)->cascade_button.armed_pixmap)

#define CB_Cascade_x(cb)	(((XmCascadeButtonWidget)                    \
                                  cb)->cascade_button.cascade_rect.x)
#define CB_Cascade_y(cb)	(((XmCascadeButtonWidget)                    \
                                  cb)->cascade_button.cascade_rect.y)
#define CB_Cascade_width(cb)	(((XmCascadeButtonWidget)                    \
                                  cb)->cascade_button.cascade_rect.width)
#define CB_Cascade_height(cb)	(((XmCascadeButtonWidget)                    \
                                  cb)->cascade_button.cascade_rect.height)

#define CB_HasCascade(cb)       (((Lab_MenuType(cb) == XmMENU_PULLDOWN)  ||  \
				  (Lab_MenuType(cb) == XmMENU_POPUP)) &&     \
			         (CB_Submenu(cb)))


#ifdef _NO_PROTO
void _XmCascadingPopup ();
void _XmCBHelp ();
void _XmButtonPopdownChildren ();
#else /* _NO_PROTO */
void _XmButtonPopdownChildren (XmRowColumnWidget rowcol);
void _XmCascadingPopup (Widget cb, XEvent *event, Boolean doCascade);
void _XmCBHelp (Widget w, XEvent *event);
#endif /* _NO_PROTO */
				  
#endif  /* _XmCascadeBP_h */
/* DON'T ADD STUFF AFTER THIS #endif */
