#ifdef REV_INFO
#ifndef lint
static char SCCSID[] = "OSF/Motif: @(#)RowColumnP.h	3.11 91/01/10";
#endif /* lint */
#endif /* REV_INFO */
/******************************************************************************
*******************************************************************************
*
*  (c) Copyright 1989, 1990, 1991 OPEN SOFTWARE FOUNDATION, INC.
*  (c) Copyright 1989, DIGITAL EQUIPMENT CORPORATION, MAYNARD, MASS.
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
#ifndef _XmRowColumnP_h
#define _XmRowColumnP_h

#include <Xm/RowColumn.h>
#include <Xm/XmP.h>

/* The RowColumn instance record */

typedef	struct _XmRowColumnPart
{
    Dimension       margin_height;  /* margin around inside of widget */
    Dimension       margin_width;


    Dimension       spacing;        /* pixels between entries */
    Dimension       entry_border;   /* size of entry borders */

                    /* next only used w/ menubars */

    Widget      help_pushbutton;    /* ptr to help pushbutton widget */

    Widget      cascadeBtn;         /* if this menu is pulled down by a */
                                    /* pulldown widget this will point */
                                    /* at the pulldown.  needed to go */
                                    /* up the cascade */

                    /* next two only used w/ option menus */
                    /* they are really only temporary */
                    /* since the data is passed off to */
                    /* the pulldown widget which is */
                    /* automatically built */

    XmString  option_label;           /* label for option menu pulldown */

    Widget      option_submenu;     /* which submenu to pulldown */


    XmKidGeometry   boxes;          /* when doing menu layouts is an */
                                    /* array of geo req's to make it easy */

    WidgetClass     entry_class;    /* if homogeneous, what class */

    XtCallbackList  entry_callback; /* a child fired off */
    XtCallbackList  map_callback;   /* about to be mapped call back */
    XtCallbackList  unmap_callback; /* about to be unmapped call back */

    Widget      memory_subwidget;   /* id of last subwidget that */
                                    /* fired off.  Recorded by the */
                                    /* entry_fired proc, can be set too */
                                    /* this causes mouse/muscle memory */
                                    /* to also be reset */

    short       num_columns;        /* if columnar packing this is how */
                                    /* many columns to use */

    String	    menuPost;	    /* a translation for posting popups */
    unsigned int    postButton;     /* active mouse button */
    unsigned int    postEventType;  /* active mouse event type */
    unsigned int    postModifiers;  /* active mouse modifier */

    String      menu_accelerator;
    KeySym	    mnemonic;
    XmStringCharSet mnemonicCharSet;

    unsigned char   entry_alignment; /* type of label alignment */
                                     /* our children should have */

                    /* next two are layout, Tight is the */
                    /* standard menubar packing.  Columns */
                    /* is radio box style, orientation */
                    /* determines if it is column or row */
                    /* major, Vert = column major */

    unsigned char   packing;    /* entry packing (layout) style */

    unsigned char   type;       /* temporary: diff between menu/bar */

    unsigned char   orientation;    /* horizontal or vertical */

                    /* next two indicate how the widget */
                    /* responds to size changes if there */
                    /* is no geo mgr.  If true then the */
                    /* dimension is never changed.  Set */
                    /* to true if dimension is spec'd */
                    /* at create time */

    Boolean     armed;      /* controls whether pulldowns work */
                            /* or not, button down in any part of */
                            /* the menubar arms it, this is a bit field  */
                            /* used for other internal flags, see macros */ 

                    /* next is only valid for popup menus */

    Boolean     adjust_margin;  /* T/F, indicating if we should force */
                                /* all subwidgets to have similar */
                                /* margins */
    
    Boolean     adjust_last;    /* Indicates whether or not the last row */
                                /* row or column should be stretched to  */
                                /* the edge of the row_column widget.    */

    Boolean     do_alignment;   /* T/F, do we force alignment on all */
                                /* our children */

    Boolean     radio;          /* T/F, do we do the toggle button */
                                /* 'only-one-down' enforcement */

    Boolean     radio_one;      /* T/F, must have one radio button */
                                /* set to on */


    Boolean     homogeneous;    /* T/F, do we only allow a single */
                                /* class of children */

    Boolean     resize_width;
    Boolean     resize_height;

    Boolean     popup_enabled;

    Dimension	old_width;		/* save the old width, etc to use  */
    Dimension	old_height;		/* at resize time since it now has */
    Dimension	old_shadow_thickness;   /* NW gravity                      */

    Widget *	postFromList;		/* list for sharing menupanes */
    int		postFromCount;		/* count of the list */
    int		postFromListSize;	/* size of the malloc'ed list */

    Widget      lastSelectToplevel;     /* returned in XmGetPostedFromWidget*/ 
    Widget	popupPosted;		/* popup submenu currently posted */

} XmRowColumnPart;


/* Full instance record declaration */

typedef struct _XmRowColumnWidgetRec
{
    CorePart		core;
    CompositePart	composite;
    ConstraintPart	constraint;
    XmManagerPart	manager;
    XmRowColumnPart	row_column;
} XmRowColumnWidgetRec;



/* RowColumn class structure */

typedef struct _XmRowColumnClassPart
{
    XmMenuProc      menuProcedures; /* proc to interface with menu widgets */
    XmArmAndActivate  armAndActivate; /* proc triggered by acclerator */
    XmMenuTrav     traversalHandler;/* proc to handle menu traversal */
    caddr_t     extension;      /* Pointer to extension record */
} XmRowColumnClassPart;



typedef struct _XmRowColumnClassRec 
{
    CoreClassPart		core_class;
    CompositeClassPart		composite_class;
    ConstraintClassPart		constraint_class;
    XmManagerClassPart		manager_class;
    XmRowColumnClassPart	row_column_class;
} XmRowColumnClassRec;

externalref XmRowColumnClassRec     xmRowColumnClassRec;




/* Constraint Definition */

/* No constraint resources */

typedef struct _XmRowColumnConstraintPart
{
	Boolean	was_managed;
} XmRowColumnConstraintPart;

typedef struct _XmRowColumnConstraintRec
{
	XmManagerConstraintPart manager;
	XmRowColumnConstraintPart row_column;
} XmRowColumnConstraintRec;


/*  Button Event Status Record for popup verification at manage time */

typedef struct _XmButtonEventStatusRec
{
	Time time;
	Boolean verified;
	Boolean waiting_to_be_managed;
	XButtonEvent event;
} XmButtonEventStatusRec;



/* Access Macros */
#define XmRC_ARMED_BIT	      (1 << 0)	
#define XmRC_BEING_ARMED_BIT  (1 << 1)		/* bits in menu's armed byte */
#define XmRC_EXPOSE_BIT       (1 << 2)		/* used in both menu and */
#define XmRC_WINDOW_MOVED_BIT (1 << 3)		/* popup menu, careful */
#define XmRC_WIDGET_MOVED_BIT (1 << 4)
#define XmRC_POPPING_DOWN_BIT (1 << 5)

#define RC_IsArmed(m)	 (((XmRowColumnWidget)(m))->row_column.armed & XmRC_ARMED_BIT)
#define RC_BeingArmed(m) (((XmRowColumnWidget)(m))->row_column.armed & XmRC_BEING_ARMED_BIT)
#define RC_DoExpose(m)	 (((XmRowColumnWidget)(m))->row_column.armed & XmRC_EXPOSE_BIT)
#define RC_WidgetHasMoved(m) (((XmRowColumnWidget)(m))->row_column.armed & XmRC_WIDGET_MOVED_BIT)
#define RC_WindowHasMoved(m) (((XmRowColumnWidget)(m))->row_column.armed & XmRC_WINDOW_MOVED_BIT)
#define RC_PoppingDown(m) (((XmRowColumnWidget)(m))->row_column.armed & XmRC_POPPING_DOWN_BIT)


#define RC_SetBit(byte,bit,v)  byte = (byte & (~bit)) | (v ? bit : 0)

#define RC_SetArmed(m,v)  RC_SetBit (((XmRowColumnWidget)(m))->row_column.armed, XmRC_ARMED_BIT, v)
#define RC_SetBeingArmed(m,v)  RC_SetBit (((XmRowColumnWidget)(m))->row_column.armed, XmRC_BEING_ARMED_BIT, v)
#define RC_SetExpose(m,v) RC_SetBit (((XmRowColumnWidget)(m))->row_column.armed, XmRC_EXPOSE_BIT, v)
#define RC_SetWidgetMoved(m,v) RC_SetBit(((XmRowColumnWidget)(m))->row_column.armed, XmRC_WIDGET_MOVED_BIT,v)
#define RC_SetWindowMoved(m,v) RC_SetBit(((XmRowColumnWidget)(m))->row_column.armed, XmRC_WINDOW_MOVED_BIT,v)
#define RC_SetPoppingDown(m,v) RC_SetBit(((XmRowColumnWidget)(m))->row_column.armed, XmRC_POPPING_DOWN_BIT,v)

#define RC_MarginW(m)	    (((XmRowColumnWidget)(m))->row_column.margin_width)
#define RC_MarginH(m)	    (((XmRowColumnWidget)(m))->row_column.margin_height)

#define RC_Entry_cb(m)	    (((XmRowColumnWidget)(m))->row_column.entry_callback)
#define RC_Map_cb(m)	    (((XmRowColumnWidget)(m))->row_column.map_callback)
#define RC_Unmap_cb(m)	    (((XmRowColumnWidget)(m))->row_column.unmap_callback)

#define RC_Orientation(m)   (((XmRowColumnWidget)(m))->row_column.orientation)
#define RC_Spacing(m)	    (((XmRowColumnWidget)(m))->row_column.spacing)
#define RC_EntryBorder(m)   (((XmRowColumnWidget)(m))->row_column.entry_border)
#define RC_HelpPb(m)	    (((XmRowColumnWidget)(m))->row_column.help_pushbutton)
#define RC_DoMarginAdjust(m)  (((XmRowColumnWidget)(m))->row_column.adjust_margin)
#define RC_EntryAlignment(m)  (((XmRowColumnWidget)(m))->row_column.entry_alignment)
#define RC_Packing(m)	   (((XmRowColumnWidget)(m))->row_column.packing)
#define RC_NCol(m)	   (((XmRowColumnWidget)(m))->row_column.num_columns)
#define RC_AdjLast(m)	   (((XmRowColumnWidget)(m))->row_column.adjust_last)
#define RC_AdjMargin(m)	   (((XmRowColumnWidget)(m))->row_column.adjust_margin)
#define RC_MemWidget(m)	   (((XmRowColumnWidget)(m))->row_column.memory_subwidget)
#define RC_CascadeBtn(m)   (((XmRowColumnWidget)(m))->row_column.cascadeBtn)
#define RC_OptionLabel(m)  (((XmRowColumnWidget)(m))->row_column.option_label)
#define RC_OptionSubMenu(m)  (((XmRowColumnWidget)(m))->row_column.option_submenu)
#define RC_RadioBehavior(m)  (((XmRowColumnWidget)(m))->row_column.radio)
#define RC_RadioAlwaysOne(m) (((XmRowColumnWidget)(m))->row_column.radio_one)
#define RC_PopupPosted(m)    (((XmRowColumnWidget)(m))->row_column.popupPosted)
#define RC_ResizeHeight(m)    (((XmRowColumnWidget)(m))->row_column.resize_height)
#define RC_ResizeWidth(m)     (((XmRowColumnWidget)(m))->row_column.resize_width)
#define RC_Type(m)           (((XmRowColumnWidget)(m))->row_column.type)
#define RC_EntryClass(m)     (((XmRowColumnWidget)(m))->row_column.entry_class)
#define RC_IsHomogeneous(m)  (((XmRowColumnWidget)(m))->row_column.homogeneous)
#define RC_Boxes(m)          (((XmRowColumnWidget)(m))->row_column.boxes)
#define RC_PopupEnabled(m)   (((XmRowColumnWidget)(m))->row_column.popup_enabled)
#define RC_MenuAccelerator(m)  (((XmRowColumnWidget)(m))->row_column.menu_accelerator)
#define RC_Mnemonic(m)   (((XmRowColumnWidget)(m))->row_column.mnemonic)
#define RC_MnemonicCharSet(m)   (((XmRowColumnWidget)(m))->row_column.mnemonicCharSet)
#define RC_MenuPost(m) (((XmRowColumnWidget) m)->row_column.menuPost)
#define RC_PostButton(m) (((XmRowColumnWidget) m)->row_column.postButton)
#define RC_PostModifiers(m) (((XmRowColumnWidget) m)->row_column.postModifiers)
#define RC_PostEventType(m) (((XmRowColumnWidget) m)->row_column.postEventType)


/* Defines used when calling _XmProcessMenuTree() */

#define XmADD     0
#define XmDELETE  1
#define XmREPLACE 2


/* Defines used when calling _XmMenuIsAccessible() */

#define XmWEAK_CHECK 1
#define XmMEDIUM_CHECK 2
#define XmSTRONG_CHECK 3

#define XmMENU_BEGIN 0
#define XmMENU_MIDDLE 1
#define XmMENU_END 2

/* Defines used when calling find_first_managed_child() */
#define ANY_CHILD 0
#define FIRST_BUTTON 1

#ifdef _NO_PROTO
extern void _XmMenuFocus();
extern void _XmRC_GetMnemonicCharSet();
extern void _XmRC_GetLabelString();
extern void _XmRC_GetMenuAccelerator();
#else /* _NO_PROTO */
extern void _XmMenuFocus (Widget w, int operation, Time time);
extern void _XmRC_GetMenuAccelerator(
            XmRowColumnWidget Rc,
            XrmQuark        resource,
            XtArgVal *      value );
extern void _XmRC_GetMnemonicCharSet(
	    XmRowColumnWidget Rc,
            XrmQuark        resource,
            XtArgVal *      value );
extern void _XmRC_GetLabelString(
	    XmRowColumnWidget Rc,
            XrmQuark        resource,
            XtArgVal *      value );
#endif /* _NO_PROTO */

#endif  /* _XmRowColumnP_h */
/* DON'T ADD STUFF AFTER THIS #endif */
