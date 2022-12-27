#ifdef REV_INFO
#ifndef lint
static char SCCSID[] = "OSF/Motif: @(#)CascadeBGP.h	3.5 90/07/23";
#endif /* lint */
#endif /* REV_INFO */
/******************************************************************************
*******************************************************************************
*
*  (c) Copyright 1989, 1990, OPEN SOFTWARE FOUNDATION, INC.
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
#ifndef  _XmCascadeBGP_h
#define  _XmCascadeBGP_h

#include <Xm/CascadeBG.h>
#include <Xm/ExtObjectP.h>
#include <Xm/XmP.h>


/*********************************************************************/
/* The CascadeButton Gadget Cache Object's class and instance records*/
/*********************************************************************/


typedef struct _XmCascadeButtonGCacheObjClassPart
{
    int foo;

} XmCascadeButtonGCacheObjClassPart;


typedef struct _XmCascadeButtonGCacheObjClassRec
{
        ObjectClassPart                     object_class;
        XmExtClassPart                      ext_class;
        XmLabelGCacheObjClassPart           label_class_cache;
        XmCascadeButtonGCacheObjClassPart   cascade_button_class_cache;

} XmCascadeButtonGCacheObjClassRec;

externalref XmCascadeButtonGCacheObjClassRec xmCascadeButtonGCacheObjClassRec;


typedef struct _XmCascadeButtonGCacheObjPart
{
    Pixmap              cascade_pixmap;         /* pixmap for the cascade */
    int                 map_delay;              /* time delay for posting */
    Pixmap		armed_pixmap;
} XmCascadeButtonGCacheObjPart;

typedef struct _XmCascadeButtonGCacheObjRec
{
    ObjectPart                   object;
    XmExtPart                    ext;
    XmLabelGCacheObjPart         label_cache;
    XmCascadeButtonGCacheObjPart cascade_button_cache;
} XmCascadeButtonGCacheObjRec;

/* The CascadeButtonGadget instance record */

typedef	struct _XmCascadeButtonGadgetPart
{			/* resources */
    Widget		submenu;		/* the menu to pull down */
    XtCallbackList	activate_callback;	/* widget fired callback */
    XtCallbackList	cascade_callback;	/* optional callback, called */
						/* when the menu is about */
						/* to be pulled down */
			/* internal fields */
    Boolean		armed;			/* armed flag */
    XRectangle		cascade_rect;		/* location of cascade*/
    int 		timer;			/* timeout id */
    XmCascadeButtonGCacheObjPart         *cache;

} XmCascadeButtonGadgetPart;


/* Full instance record declaration */

typedef struct _XmCascadeButtonGadgetRec
{
    ObjectPart		       object;
    RectObjPart                rectangle;
    XmGadgetPart               gadget;
    XmLabelGadgetPart          label;
    XmCascadeButtonGadgetPart  cascade_button;
} XmCascadeButtonGadgetRec;



/* CascadeButton class structure */

typedef struct 
{
    caddr_t	extension;	/* Pointer to extension record */
} XmCascadeButtonGadgetClassPart;


/* Full class record declaration for CascadeButton class */

typedef struct _XmCascadeButtonGadgetClassRec 
{
    RectObjClassPart               rect_class;
    XmGadgetClassPart              gadget_class;
    XmLabelGadgetClassPart         label_class;
    XmCascadeButtonGadgetClassPart cascade_button_class;
} XmCascadeButtonGadgetClassRec;


externalref XmCascadeButtonGadgetClassRec   xmCascadeButtonGadgetClassRec;

/* Access macro definitions  for UNcached fields*/

#define CBG_Submenu(cb)		(((XmCascadeButtonGadget) 		    \
                                  cb)->cascade_button.submenu)
#define CBG_ActivateCall(cb)	(((XmCascadeButtonGadget)                    \
                                  cb)->cascade_button.activate_callback)
#define CBG_CascadeCall(cb)	(((XmCascadeButtonGadget)                    \
                                  cb)->cascade_button.cascade_callback)
#define CBG_Armed(cb)		(((XmCascadeButtonGadget)                    \
                                  cb)->cascade_button.armed)
#define CBG_CascadeRect(cb)	(((XmCascadeButtonGadget)                    \
                                  cb)->cascade_button.cascade_rect)
#define CBG_Timer(cb)           (((XmCascadeButtonGadget)                    \
				  cb)->cascade_button.timer)
#define CBG_Cascade_x(cb)	(((XmCascadeButtonGadget)                    \
                                  cb)->cascade_button.cascade_rect.x)
#define CBG_Cascade_y(cb)	(((XmCascadeButtonGadget)                    \
                                  cb)->cascade_button.cascade_rect.y)
#define CBG_Cascade_width(cb)	(((XmCascadeButtonGadget)                    \
                                  cb)->cascade_button.cascade_rect.width)
#define CBG_Cascade_height(cb)	(((XmCascadeButtonGadget)                    \
                                  cb)->cascade_button.cascade_rect.height)
#define CBG_HasCascade(cb)      (((LabG_MenuType(cb) == XmMENU_PULLDOWN)  || \
			          (LabG_MenuType(cb) == XmMENU_POPUP) ||     \
                                  (LabG_MenuType(cb) == XmMENU_OPTION)) &&   \
			         (CBG_Submenu(cb)))

				  
/* Access macro definitions  for Cached fields*/

#define CBG_CascadePixmap(cb)   (((XmCascadeButtonGadget)                    \
				  cb)->cascade_button.cache->cascade_pixmap)
#define CBG_MapDelay(cb)        (((XmCascadeButtonGadget)                    \
				  cb)->cascade_button.cache->map_delay)
#define CBG_ArmedPixmap(cb)	(((XmCascadeButtonGadget)                    \
				  cb)->cascade_button.cache->armed_pixmap)


/******************************/
/* Convenience Macros         */
/******************************/


#define CBG_Cache(w)                    (((XmCascadeButtonGadget)(w))->\
					   cascade_button.cache)
#define CBG_ClassCachePart(w) \
	(((XmCascadeButtonGadgetClass)xmCascadeButtonGadgetClass)->gadget_class.cache_part)



#endif  /* _XmCascadeBGP_h */
/* DON'T ADD STUFF AFTER THIS #endif */
