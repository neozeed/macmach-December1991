#ifdef REV_INFO
#ifndef lint
static char SCCSID[] = "OSF/Motif: @(#)SelectioBP.h	3.9.1.2 91/03/20";
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
#ifndef _XmSelectionBoxP_h
#define _XmSelectionBoxP_h

#include <Xm/BulletinBP.h>
#include <Xm/SelectioB.h>


/* Defines for use in allocation geometry matrix. */

#define SB_MAX_WIDGETS_VERT	7
#define SB_MAX_NUM_WIDGETS	12

#define UNSPECIFIED		(~0L)

/*  New fields for the SelectionBox widget class record  */

typedef struct
{
        XtCallbackProc  list_callback ;
	caddr_t		extension;      /* Pointer to extension record */
} XmSelectionBoxClassPart;


/* Full class record declaration */

typedef struct _XmSelectionBoxClassRec
{
	CoreClassPart			core_class;
	CompositeClassPart		composite_class;
	ConstraintClassPart		constraint_class;
	XmManagerClassPart		manager_class;
	XmBulletinBoardClassPart	bulletin_board_class;
	XmSelectionBoxClassPart		selection_box_class;
} XmSelectionBoxClassRec;

externalref XmSelectionBoxClassRec xmSelectionBoxClassRec;


/* New fields for the SelectionBox widget record */

typedef struct
{
	Widget		list_label;		/*  list Label  */
	XmString	list_label_string;

	Widget		list;			/*  List  */
	XmString	*list_items;
	int		list_item_count;
	int		list_visible_item_count;
	int		list_selected_item_position;

	Widget		selection_label;	/*  selection Label  */
	XmString	selection_label_string;

	Widget		text;			/*  Text  */
	XmString	text_string;
	short		text_columns;

	Widget		work_area;		/*  other widget  */

	Widget		separator;		/*  separator  */

	Widget		ok_button;		/*  enter button  */
	XmString	ok_label_string;

	Widget		apply_button;		/*  apply button  */
	XmString	apply_label_string;

	XmString	cancel_label_string;	/*  cancel button label  */

	Widget		help_button;		/*  help button  */
	XmString	help_label_string;

	XtCallbackList	ok_callback;		/*  callbacks  */
	XtCallbackList	apply_callback;
	XtCallbackList	cancel_callback;
	XtCallbackList	no_match_callback;

	XtAccelerators	text_accelerators;

	Boolean		must_match;		/*  flags  */
	Boolean		adding_sel_widgets;
	Boolean		minimize_buttons;

	unsigned char	dialog_type;		/*  prompt or selection  */
} XmSelectionBoxPart;


/****************************************************************
 *
 * Full instance record declaration
 *
 ****************************************************************/

typedef struct _XmSelectionBoxRec
{
	CorePart		core;
	CompositePart		composite;
	ConstraintPart		constraint;
	XmManagerPart		manager;
	XmBulletinBoardPart	bulletin_board;
	XmSelectionBoxPart	selection_box;
} XmSelectionBoxRec;



/*  access macros  */

#define SB_ListLabel(w) (((XmSelectionBoxWidget) (w))->selection_box.list_label)
#define SB_List(w) (((XmSelectionBoxWidget) (w))->selection_box.list)
#define SB_SelectionLabel(w) (((XmSelectionBoxWidget) (w))->selection_box.selection_label)
#define SB_Text(w) (((XmSelectionBoxWidget) (w))->selection_box.text)
#define SB_WorkArea(w) (((XmSelectionBoxWidget) (w))->selection_box.work_area)
#define SB_Separator(w) (((XmSelectionBoxWidget) (w))->selection_box.separator)
#define SB_OkButton(w) (((XmSelectionBoxWidget) (w))->selection_box.ok_button)
#define SB_ApplyButton(w) (((XmSelectionBoxWidget) (w))->selection_box.apply_button)
#define SB_CancelButton(w) (((XmSelectionBoxWidget) (w))->bulletin_board.cancel_button)
#define SB_HelpButton(w) (((XmSelectionBoxWidget) (w))->selection_box.help_button)
#define SB_DefaultButton(w) (((XmSelectionBoxWidget) (w))->bulletin_board.default_button)
#define SB_MarginHeight(w) (((XmSelectionBoxWidget) (w))->bulletin_board.margin_height)
#define SB_MarginWidth(w) (((XmSelectionBoxWidget) (w))->bulletin_board.margin_width)
#define SB_ButtonFontList(w) (((XmSelectionBoxWidget) (w))->bulletin_board.button_font_list)
#define SB_LabelFontList(w) (((XmSelectionBoxWidget) (w))->bulletin_board.label_font_list)
#define SB_TextFontList(w) (((XmSelectionBoxWidget) (w))->bulletin_board.text_font_list)
#define SB_StringDirection( w) \
                       (((XmSelectionBoxWidget)( w))->manager.string_direction)
#define SB_AddingSelWidgets( w) \
                 (((XmSelectionBoxWidget) w)->selection_box.adding_sel_widgets)
#define SB_TextAccelerators( w) \
                  (((XmSelectionBoxWidget) w)->selection_box.text_accelerators)
#define SB_ListItemCount( w) \
                    (((XmSelectionBoxWidget) w)->selection_box.list_item_count)
#define SB_ListSelectedItemPosition( w) (((XmSelectionBoxWidget) w) \
                                   ->selection_box.list_selected_item_position)
#define SB_ListVisibleItemCount( w) (((XmSelectionBoxWidget) w) \
                                       ->selection_box.list_visible_item_count)
#define SB_TextColumns( w) \
                       (((XmSelectionBoxWidget) w)->selection_box.text_columns)
#define SB_MinimizeButtons( w) \
                   (((XmSelectionBoxWidget) w)->selection_box.minimize_buttons)
#define SB_MustMatch( w) (((XmSelectionBoxWidget) w)->selection_box.must_match)

/* Private external functions:
*/
#ifdef _NO_PROTO

extern void _XmSelectionBoxCreateListLabel() ;
extern void _XmSelectionBoxCreateSelectionLabel() ;
extern void _XmSelectionBoxCreateList() ;
extern void _XmSelectionBoxCreateText() ;
extern void _XmSelectionBoxCreateSeparator() ;
extern void _XmSelectionBoxCreateOkButton() ;
extern void _XmSelectionBoxCreateApplyButton() ;
extern void _XmSelectionBoxCreateCancelButton() ;
extern void _XmSelectionBoxCreateHelpButton() ;
extern XmGeoMatrix _XmSelectionBoxGeoMatrixCreate() ;
extern Boolean _XmSelectionBoxNoGeoRequest() ;
extern void _XmSelectionBoxGetSelectionLabelString() ;
extern void _XmSelectionBoxGetListLabelString() ;
extern void _XmSelectionBoxGetTextColumns() ;
extern void _XmSelectionBoxGetTextString() ;
extern void _XmSelectionBoxGetListItems() ;
extern void _XmSelectionBoxGetListItemCount() ;
extern void _XmSelectionBoxGetListVisibleItemCount() ;
extern void _XmSelectionBoxGetOkLabelString() ;
extern void _XmSelectionBoxGetApplyLabelString() ;
extern void _XmSelectionBoxGetCancelLabelString() ;
extern void _XmSelectionBoxGetHelpLabelString() ;
extern void _XmSelectionBoxUpOrDown() ;
extern void _XmSelectionBoxRestore() ;

#else /* _NO_PROTO */

extern void _XmSelectionBoxCreateListLabel( XmSelectionBoxWidget sel) ;
extern void _XmSelectionBoxCreateSelectionLabel( XmSelectionBoxWidget sel) ;
extern void _XmSelectionBoxCreateList( XmSelectionBoxWidget sel) ;
extern void _XmSelectionBoxCreateText( XmSelectionBoxWidget sel) ;
extern void _XmSelectionBoxCreateSeparator( XmSelectionBoxWidget sel) ;
extern void _XmSelectionBoxCreateOkButton( XmSelectionBoxWidget sel) ;
extern void _XmSelectionBoxCreateApplyButton( XmSelectionBoxWidget sel) ;
extern void _XmSelectionBoxCreateCancelButton( XmSelectionBoxWidget sel) ;
extern void _XmSelectionBoxCreateHelpButton( XmSelectionBoxWidget sel) ;
extern XmGeoMatrix _XmSelectionBoxGeoMatrixCreate( XmSelectionBoxWidget sb,
                                Widget instigator, XtWidgetGeometry *desired) ;
extern Boolean _XmSelectionBoxNoGeoRequest( XmGeoMatrix geoSpec) ;
extern void _XmSelectionBoxGetSelectionLabelString( XmSelectionBoxWidget sel,
                                          XrmQuark resource, XtArgVal *value) ;
extern void _XmSelectionBoxGetListLabelString( XmSelectionBoxWidget sel,
                                          XrmQuark resource, XtArgVal *value) ;
extern void _XmSelectionBoxGetTextColumns( XmSelectionBoxWidget sel,
                                          XrmQuark resource, XtArgVal *value) ;
extern void _XmSelectionBoxGetTextString( XmSelectionBoxWidget sel,
                                          XrmQuark resource, XtArgVal *value) ;
extern void _XmSelectionBoxGetListItems( XmSelectionBoxWidget sel,
                                          XrmQuark resource, XtArgVal *value) ;
extern void _XmSelectionBoxGetListItemCount( XmSelectionBoxWidget sel, 
                                          XrmQuark resource, XtArgVal *value) ;
extern void _XmSelectionBoxGetListVisibleItemCount( XmSelectionBoxWidget sel,
                                          XrmQuark resource, XtArgVal *value) ;
extern void _XmSelectionBoxGetOkLabelString( XmSelectionBoxWidget sel,
                                          XrmQuark resource, XtArgVal *value) ;
extern void _XmSelectionBoxGetApplyLabelString( XmSelectionBoxWidget sel,
                                          XrmQuark resource, XtArgVal *value) ;
extern void _XmSelectionBoxGetCancelLabelString( XmSelectionBoxWidget sel,
                                          XrmQuark resource, XtArgVal *value) ;
extern void _XmSelectionBoxGetHelpLabelString( XmSelectionBoxWidget sel,
                                          XrmQuark resource, XtArgVal *value) ;
extern void _XmSelectionBoxUpOrDown( XmSelectionBoxWidget sel, XEvent *event,
                                                       char **argv, int argc) ;
extern void _XmSelectionBoxRestore( XmSelectionBoxWidget sel, XEvent *event,
                                                       char **argv, int argc) ;
#endif /* _NO_PROTO */

#endif /* _XmSelectionBoxP_h */
/* DON'T ADD ANYTHING AFTER THIS #endif */
