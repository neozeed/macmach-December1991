#ifdef REV_INFO
#ifndef lint
static char SCCSID[] = "OSF/Motif: @(#)TextFP.h	3.13 90/07/31";
#endif /* lint */
#endif /* REV_INFO */
/******************************************************************************
*******************************************************************************
*
*  (c) Copyright 1989, 1990, OPEN SOFTWARE FOUNDATION, INC.
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
* X Window System is a trademark of the Massachusetts Institute of Technology
*
*******************************************************************************
******************************************************************************/
#ifndef _XmTextFP_h
#define _XmTextFP_h

#include <Xm/XmP.h>
#include <Xm/TextF.h>

/*
 * Defines for different cursors
 */

#define IBEAM_WIDTH 3
#define CARET_WIDTH 9
#define CARET_HEIGHT 5

static unsigned char caretBits[] = {
   0x10, 0x00, 0x38, 0x00, 0x6c, 0x00, 0xc6, 0x00, 0x83, 0x01};

/*
 * This struct is for support of Insert Selection targets.
 */
typedef struct {
    Atom selection;
    Atom target;
} TextInsertPair;

/*
 * These two structs are used in text highlighting.
 */
typedef struct {
    XmTextPosition position;    /* Starting position. */
    XmHighlightMode mode;         /* Highlighting mode for this position. */
} HighlightRec;

typedef struct {
    Cardinal number;            /* Number of different highlight areas. */
    Cardinal maximum;           /* Number we've allocated space for. */
    HighlightRec *list;         /* Pointer to array of highlight data. */
} HighlightData;

/*
 * Here is the Text Field Widget class structure.
 */

typedef struct _XmTextFieldClassPart
  {
    caddr_t extension;		/* Pointer to extension record. */
  }
  XmTextFieldClassPart;

typedef struct _XmTextFieldClassRec
  {
    CoreClassPart core_class;  /* Not RectObjClassPart so I can reference
				  core_class s */
    XmPrimitiveClassPart primitive_class;
    XmTextFieldClassPart text_class;
  }
  XmTextFieldClassRec;

externalref XmTextFieldClassRec xmTextFieldClassRec;

/*
 * Here is the Text Field Widget instance structures.
 */

typedef struct _XmTextFieldPart
  {
    XtCallbackList activate_callback;	   /* Command activate callback */
    XtCallbackList losing_focus_callback;  /* Verify losing focus callback */
    XtCallbackList modify_verify_callback; /* Verify value to change callback */
    XtCallbackList motion_verify_callback; /* Verify insert cursor position to
					      change callback */
    XtCallbackProc gain_primary_callback; /* Gained ownership of Primary
                                             Selection */
    XtCallbackProc lose_primary_callback; /* Lost ownership of Primary
                                             Selection */
    XtCallbackList value_changed_callback; /* Notify that value has change						      callback */
    char * value;		/* The string value in the widget */
    Dimension margin_width;	/* Height between text borders and text */
    Dimension margin_height;	/* Width between text borders and text */
    XmTextPosition cursor_position;/* Character location of the insert cursor */
    short columns;		/* The number of characters in the width */
    int max_length;		/* Maximum number of character that can be
				   inserted into the text field widget */
    int blink_rate;		/* Rate of blinking text cursor in msec */
    XmFontList font_list;	/* Uses only the font portion of fontlist */
    XFontStruct *font;	        /* font retrieved from the fontlist */
    XmTextScanType *selection_array; /* Description of what to cycle
					     through on selections */
    int selection_array_count;  /* Selection array count */
    int threshold;		/* Selection threshold */
    Boolean resize_width;	/* Allows the widget to grow horizontally
				   when borders are reached */
    Boolean pending_delete;	/* Delete primary selection on insert when
				   set to True */
    Boolean editable;		/* Sets editablility of text */
    Boolean verify_bell;          /* Determines if bell is sounded when verify
                                   *  callback returns doit - False
                                   */
    Boolean cursor_position_visible;	/* Sets visibility of insert cursor */

    Boolean traversed;          /* Flag used with losing focus verification to
                                   indicate a traversal key pressed event */
    Boolean add_mode;		/* Add mode for cursor movement */
    Boolean has_focus;		/* Flag that indicates whether the widget
			           has input focus */
    Boolean blink_on;		/* State of Blinking insert cursor */
    Boolean cursor_on;		/* Indicates whether the cursor is visible */
    Boolean dest_on;		/* Indicates whether the destination cursor
				   has been drawn */
    Boolean dest_visible;	/* Indicates whether the destination cursor
				   is visible */
    Boolean has_primary;	/* Indicates that is has the
				   primary selection */
    Boolean has_secondary;	/* Indicates that is has the
				   secondary selection */
    Boolean has_destination;	/* Indicates that is has the
				   destination selection */
    Boolean sec_drag;           /* Indicates a secondary drag was made */ 
    Boolean selection_move;	/* Indicates that the action requires a
				   secondary move (i.e. copy & cut) */
    Boolean pending_off;	/* indicates pending delete state */
    Boolean fontlist_created;   /* Indicates that the text field widget created
				   it's own fontlist */
    Boolean has_rect;		/* currently has clipping rectangle */
    Boolean do_drop;		/* Indicates that the widget the recieved the
				   button release, did not have a previous
                                   button press, so it is o.k. to request
				   the MOTIF_DROP selection. */
    Boolean cancel;		/* Cancels selection actions when true */
    Boolean extending;		/* Indicates extending primary selection */
    Boolean changed_visible;    /* Indicates whether the dest_visible flag
				   is in a temporary changed state */
    GC gc;			/* Normal GC for drawing text and cursor */
    GC image_gc;		/* Image GC for drawing text cursor*/

    XmTextPosition h_offset;  	/* The x position of the first character
                                   (relative to left edge of the widget) */
    int size_allocd;		/* Size allocated for value string */
    int string_length;          /* The length of the string (including the
                                   trailing NULL) */

    Dimension average_char_width;/* Average character width based on font */

    XmTextPosition orig_left;     /* Left primary selection prior to extend */
    XmTextPosition orig_right;    /* Right primary selection prior to extend */
    XmTextPosition prim_pos_left; /* Left primary selection position */
    XmTextPosition prim_pos_right; /* Right primary selection position */
    XmTextPosition prim_anchor;	/* Primary selection pivot point */

    XmTextPosition sec_pos_left; /* Left secondary selection position */
    XmTextPosition sec_pos_right; /* Right secondary selection position */
    XmTextPosition sec_anchor;	/* Secondary selection pivot point */

    XmTextPosition stuff_pos;	/* Position to stuff the primary selection */
    XmTextPosition dest_position; /* Position of the destination selection */
    int cursor_height;		/* Save cursor dimensions */
    int cursor_width;		/* Save cursor dimensions */
    Pixmap dest_cursor;		/* The destination cursor pixmap */
    Pixmap add_mode_cursor;	/* The add mode cursor pixmap */
    Pixmap cursor;		/* The ibeam cursor pixmap */
    Pixmap putback;		/* pixmap used in blinking text cursor */
    Pixmap stipple_tile;	/* The tile pattern for the stippled I-beam */

    Time prim_time;             /* Timestamp of primary selection */
    Time dest_time;             /* Timestamp of destination selection */
    Time sec_time;              /* Timestamp of secondary selection */
    Time last_time;             /* Time of last selection event */
    int sarray_index;		/* Index into selection array */

    HighlightData highlight;    /* Info on the highlighting regions. */
    XtIntervalId timer_id;	/* Blinking cursor timer */

    caddr_t extension;		/* Pointer to extension record. */
  }
  XmTextFieldPart;

typedef struct _XmTextFieldRec
  {
    CorePart core;
    XmPrimitivePart primitive;
    XmTextFieldPart text;
  }
  XmTextFieldRec;

/****************
 *
 * Macros for the uncached data
 *
 ****************/

#define TextF_ActivateCallback(tfg)		(((XmTextFieldWidget)(tfg)) -> \
					   text.activate_callback)
#define TextF_LosingFocusCallback(tfg)		(((XmTextFieldWidget)(tfg)) -> \
					   text.losing_focus_callback)
#define TextF_ModifyVerifyCallback(tfg)	(((XmTextFieldWidget)(tfg)) -> \
					   text.modify_verify_callback)
#define TextF_MotionVerifyCallback(tfg)	(((XmTextFieldWidget)(tfg)) -> \
					   text.motion_verify_callback)
#define TextF_ValueChangedCallback(tfg)	(((XmTextFieldWidget)(tfg)) -> \
					   text.value_changed_callback)
#define TextF_Value(tfg)			(((XmTextFieldWidget)(tfg)) -> \
					   text.value)
#define TextF_MarginHeight(tfg)		(((XmTextFieldWidget)(tfg)) -> \
					   text.margin_height)
#define TextF_MarginWidth(tfg)			(((XmTextFieldWidget)(tfg)) -> \
					   text.margin_width)
#define TextF_CursorPosition(tfg)		(((XmTextFieldWidget)(tfg)) -> \
					   text.cursor_position)
#define TextF_Columns(tfg)			(((XmTextFieldWidget)(tfg)) -> \
					   text.columns)
#define TextF_MaxLength(tfg)			(((XmTextFieldWidget)(tfg)) -> \
					   text.max_length)
#define TextF_BlinkRate(tfg)			(((XmTextFieldWidget)(tfg)) -> \
					   text.blink_rate)
#define TextF_FontList(tfg)			(((XmTextFieldWidget)(tfg)) -> \
					   text.font_list)
#define TextF_Font(tfg)				(((XmTextFieldWidget)(tfg)) -> \
					   text.font)
#define TextF_SelectionArray(tfg)		(((XmTextFieldWidget)(tfg)) -> \
					   text.selection_array)
#define TextF_SelectionArrayCount(tfg)		(((XmTextFieldWidget)(tfg)) -> \
					   text.selection_array_count)
#define TextF_ResizeWidth(tfg)			(((XmTextFieldWidget)(tfg)) -> \
					   text.resize_width)
#define TextF_PendingDelete(tfg)		(((XmTextFieldWidget)(tfg)) -> \
					   text.pending_delete)
#define TextF_Editable(tfg)			(((XmTextFieldWidget)(tfg)) -> \
					   text.editable)
#define TextF_CursorPositionVisible(tfg)	(((XmTextFieldWidget)(tfg)) -> \
					   text.cursor_position_visible)

#define TextF_Threshold(tfg)		   	(((XmTextFieldWidget)(tfg)) -> \
					   text.threshold)

#endif /* _XmTextFieldWidgetP_h */

