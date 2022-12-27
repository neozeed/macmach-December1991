#ifdef REV_INFO
#ifndef lint
static char SCCSID[] = "OSF/Motif: @(#)BulletinBP.h	3.7 90/09/04";
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
#ifndef _XmBulletinBoardP_h
#define _XmBulletinBoardP_h

#include <Xm/BulletinB.h>
#include <Xm/XmP.h>


/*  New fields for the BulletinBoard widget class record  */

typedef struct
{
	Boolean			always_install_accelerators;
	XmGeoCreateProc         geo_matrix_create ;
	XmFocusMovedProc	focus_moved_proc ;
	caddr_t			extension;
} XmBulletinBoardClassPart;


/* Full class record declaration */

typedef struct _XmBulletinBoardClassRec
{
	CoreClassPart		core_class;
	CompositeClassPart	composite_class;
	ConstraintClassPart	constraint_class;
	XmManagerClassPart	manager_class;
	XmBulletinBoardClassPart	bulletin_board_class;
} XmBulletinBoardClassRec;

externalref XmBulletinBoardClassRec xmBulletinBoardClassRec;


/* New fields for the BulletinBoard widget record */

typedef struct
{
	Dimension	margin_width;		/*  margins		*/
	Dimension	margin_height;

	Widget		default_button;		/*  widgets		*/
	Widget		dynamic_default_button;	/*  widgets		*/
	Widget		cancel_button;
	Widget		dynamic_cancel_button;

	XtCallbackList	focus_callback;		/*  callback lists	*/
#ifdef BB_HAS_LOSING_FOCUS_CB
	XtCallbackList	losing_focus_callback;	/*  callback lists	*/
#endif
	XtCallbackList	map_callback;
	XtCallbackList	unmap_callback;

	XtTranslations	text_translations;

	XmFontList	button_font_list;	/*  font lists		*/
	XmFontList	label_font_list;
	XmFontList	text_font_list;

	Boolean		allow_overlap;		/*  policies		*/
	Boolean		default_position;
	Boolean		auto_unmanage;
	unsigned char	resize_policy;

	Dimension		old_width;		/*  shadow resources	*/
	Dimension		old_height;
	Dimension		old_shadow_thickness;
	unsigned char	shadow_type;

	Boolean		in_set_values;		/*  internal flag	*/
	Boolean		initial_focus ;

	Boolean		no_resize;		/*  dialog resources	*/
	unsigned char	dialog_style;
	XmString	dialog_title;
	Widget		shell;
	Widget		descendant_focus ;

	XmGeoMatrix     geo_cache ;	    /* Cache for geometry management.*/
} XmBulletinBoardPart;


/****************************************************************
 *
 * Full instance record declaration
 *
 ****************************************************************/

typedef struct _XmBulletinBoardRec
{
	CorePart		core;
	CompositePart		composite;
	ConstraintPart		constraint;
	XmManagerPart		manager;
	XmBulletinBoardPart	bulletin_board;
} XmBulletinBoardRec;


#define BB_CancelButton(w) \
                  (((XmBulletinBoardWidget) w)->bulletin_board.cancel_button)
#define BB_DynamicCancelButton(w) \
          (((XmBulletinBoardWidget) w)->bulletin_board.dynamic_cancel_button)
#define BB_DefaultButton(w) \
                 (((XmBulletinBoardWidget) w)->bulletin_board.default_button)
#define BB_DynamicDefaultButton(w) \
         (((XmBulletinBoardWidget) w)->bulletin_board.dynamic_default_button)
#define BB_MarginHeight(w) \
                  (((XmBulletinBoardWidget) w)->bulletin_board.margin_height)
#define BB_MarginWidth(w) \
                   (((XmBulletinBoardWidget) w)->bulletin_board.margin_width)
#define BB_ButtonFontList(w) \
               (((XmBulletinBoardWidget) w)->bulletin_board.button_font_list)
#define BB_LabelFontList(w) \
                (((XmBulletinBoardWidget) w)->bulletin_board.label_font_list)
#define BB_TextFontList(w) \
                 (((XmBulletinBoardWidget) w)->bulletin_board.text_font_list)
#define BB_StringDirection(w) \
                      (((XmBulletinBoardWidget) w)->manager.string_direction)
#define BB_ResizePolicy(w) \
	    	  (((XmBulletinBoardWidget) w)->bulletin_board.resize_policy)
#define BB_InSetValues(w) \
		  (((XmBulletinBoardWidget) w)->bulletin_board.in_set_values)
#define BB_InitialFocus(w) \
		  (((XmBulletinBoardWidget) w)->bulletin_board.initial_focus)
#define BB_DescendantFocus(w) \
               (((XmBulletinBoardWidget) w)->bulletin_board.descendant_focus)

#ifdef _NO_PROTO

extern void _XmBulletinBoardSizeUpdate() ;
extern void _XmBulletinBoardFocusMoved() ;
extern void _XmBulletinBoardReturn() ;
extern void _XmBulletinBoardCancel() ;
extern void _XmBulletinBoardSetDefaultShadow() ;
extern void _XmBulletinBoardSetDynDefaultButton() ;

#else /* _NO_PROTO */

extern void _XmBulletinBoardSizeUpdate( XmBulletinBoardWidget bbWid) ;
extern void _XmBulletinBoardFocusMoved( Widget wid, XtPointer client_data, 
                                       XmFocusMovedCallbackStruct *call_data) ;
extern void _XmBulletinBoardReturn( XmBulletinBoardWidget bb, XEvent *event,
				         String * params, Cardinal numParams) ;
extern void _XmBulletinBoardCancel( XmBulletinBoardWidget bb, XEvent *event,
				         String * params, Cardinal numParams) ;
extern void _XmBulletinBoardSetDefaultShadow( Widget button) ;
extern void _XmBulletinBoardSetDynDefaultButton( Widget wid,
                                                     Widget newDefaultButton) ;
#endif /* _NO_PROTO */

/* Utility routines:
*/
#ifdef _NO_PROTO

extern Widget _XmBB_CreateButtonG() ;
extern Widget _XmBB_CreateLabelG() ;

extern void _XmBB_GetDialogTitle();
     
#else /* _NO_PROTO */

extern Widget _XmBB_CreateButtonG( Widget bb, XmString l_string, char *name) ;
extern Widget _XmBB_CreateLabelG( Widget bb, XmString l_string, char *name) ;

extern void _XmBB_GetDialogTitle(XmBulletinBoardWidget 	bb,
				 XrmQuark		resource,
				 XtArgVal *		value);

#endif /* _NO_PROTO */


#endif /* _XmBulletinBoardP_h */
/* DON'T ADD ANYTHING AFTER THIS #endif */
