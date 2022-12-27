#ifdef REV_INFO
#ifndef lint
static char SCCSID[] = "OSF/Motif: @(#)TextP.h	3.15 91/01/10";
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
#ifndef _XmTextP_h
#define _XmTextP_h

#include <Xm/Text.h>
#include <Xm/XmP.h>

typedef struct _InputRec *Input;
typedef struct _OutputRec *Output;
typedef struct _LineTableExtraRec *LineTableExtra;

#include <Xm/TextOutP.h>
#include <Xm/TextSrcP.h>
#include <Xm/TextInP.h>
#include <Xm/StringSrcP.h>
#include <Xm/CutPaste.h>

#define MAXINT	2147483647 /* Biggest number that can fit in long */

#define NODELTA		MAXINT

#define TEXTWIDGETCLASS	"Text"	/* Resource class for the text widget. */

#define GetSrc(widget)  (((XmTextWidget) (widget))->text.source)

#ifndef MIN
#define MIN(x,y)	((x) < (y) ? (x) : (y))
#endif /* MIN */

/*
 * This struct is for support of Insert Selection targets.
 */
typedef struct {
    Atom selection;
    Atom target;
} TextInsertPair;

typedef struct {
	caddr_t		extension;	/* Pointer to extension record */
} XmTextClassPart;

typedef struct _XmTextClassRec {
    CoreClassPart core_class;
    XmPrimitiveClassPart primitive_class;
    XmTextClassPart text_class;
} XmTextClassRec;

externalref XmTextClassRec	xmTextClassRec;

typedef struct {
    XmTextPosition start;	/* First position in this line. */
    Boolean changed;		/* TRUE if something in this line changed. */
    XmTextPosition changed_position; /* First position within the line that
					has changed, if any. */
    Boolean past_end;		/* TRUE if this line is past the end of any */
				/* line actually on the screen. */
    LineTableExtra extra;	/* Extra info the output module keeps. */
} LineRec, *Line;


typedef struct {
    XmTextPosition from, to;	/* Information on one range to repaint. */
} RangeRec;

typedef struct {
    Cardinal number;		/* Number of ranges defined. */
    Cardinal maximum;		/* Number of ranges we have space for. */
    RangeRec *range;		/* Pointer to array of ranges. */
} Ranges;


typedef struct {
    XmTextPosition position;	/* Starting position. */
    XmHighlightMode mode;		/* Highlighting mode for this position. */
} HighlightRec;

typedef struct {
    Cardinal number;		/* Number of different highlight areas. */
    Cardinal maximum;		/* Number we've allocated space for. */
    HighlightRec *list;		/* Pointer to array of highlight data. */
} HighlightData;

/*
 * Structure for main text info. 
 */

typedef struct _XmTextPart {
    XmTextSource source;		   /* The source for this widget. */
    XtCallbackProc activate_callback;      /* command activate callback. */
    XtCallbackProc focus_callback;	   /* Focus callback. */
    XtCallbackProc losing_focus_callback;  /* Losing focus callback. */
    XtCallbackProc value_changed_callback; /* Value changed callback. */
    XtCallbackProc modify_verify_callback; /* Verify value to change callback.*/
    XtCallbackProc motion_verify_callback; /* Insert cursor position 
					      change callback. */
    XtCallbackProc gain_primary_callback; /* Gained ownership of Primary
					     Selection */
    XtCallbackProc lose_primary_callback; /* Lost ownership of Primary
					     Selection */
    char *value;		    /* The sring value in the widget */
    int max_length;		    /* Sets the max. length of string */
    Dimension margin_height;        /* height between text borders and text */
    Dimension margin_width;         /* width between text borders and text */
    OutputCreateProc output_create; /* Routine to create the output portion. */
    InputCreateProc input_create;   /* Routine to create the input portion. */
    /* The naming incongruity amongst the next three items was introduced */
    /* due to a collision with top_position as used as a Form constraint; */
    /* It has no other implications. */
    XmTextPosition top_character;    /* First position to display. */
    XmTextPosition bottom_position; /* Last position to display. */
    XmTextPosition cursor_position; /* Location of the insertion point. */
    int edit_mode;		    /* Sets the line editing mode
				       (i.e. sinlge_line, multi_line, ...) */
    Boolean auto_show_cursor_position; /* If true, automatically try to show
					  the cursor position whenever it
					  changes. */
    Boolean editable;		  /* Determines if text is editable */
    Boolean verify_bell; 	  /* Determines if bell is sounded when verify
				   *  callback returns doit - False
                                   */
    Boolean add_mode;		  /* Determines the state of add moder */
    Boolean traversed;            /* Flag used with losing focus verification to
				     indicate a traversal key pressed event */
    Boolean in_redisplay;	  /* Whether currently in the redisplay proc. */
    Boolean needs_redisplay;	  /* Whether we need to repaint or refigure. */
    Boolean in_refigure_lines;	  /* Whether currently in refigurelines proc. */
    Boolean needs_refigure_lines; /* Whether we need to refigure. */
    Boolean in_resize;		  /* Make sure there are no geometry requsets
				     while we are in resize procedure. */
    Boolean highlight_changed;	  /* Whether highlighting recently changed. */
    Boolean pendingoff;         /* TRUE if we shouldn't do pending delete on
                                   the current selection. */

    OnOrOff on_or_off;		  /* Whether insertion point is currently on. */

    Output output;		   /* The output portion. */
    Input input;		   /* The input portion. */

    XmTextPosition first_position; /* First legal position in the source. */
    XmTextPosition last_position;  /* Last legal position in the source. */
    XmTextPosition forget_past;	   /* Forget all about positions past this. */
    XmTextPosition force_display;  /* Force this position to be displayed. */
    XmTextPosition new_top;	   /* Desired new top position. */
    XmTextPosition last_top_char;  /* Last top position that was displayed. */
    XmTextPosition dest_position;  /* Location of the destination point. */
    int disable_depth;		   /* How many levels of disable we've done. */

    int pending_scroll;		/* Number of lines we want to scroll. */
    int total_lines;		/* Total number of lines in the text widget */
    int top_line;		/* Line number of the top visible line */
    int vsbar_scrolling;	/* scrolling using the vertical scrollbar */

    Cardinal number_lines;	/* Number of line table entries. */
    Cardinal maximum_lines;	/* Maximum number of line table entries. */
    Line line;			/* Pointer to array of line table entries. */

    Ranges repaint;		/* Info on the repaint ranges. */
    HighlightData highlight;	/* Info on the highlighting regions. */
    HighlightData old_highlight;/* Old value of above. */
    Widget inner_widget;	/* Pointer to widget which actually contains
				   text (may be same or different from
				   this record).  */
} XmTextPart;

typedef struct _XmTextRec {
    CorePart	core;
    XmPrimitivePart primitive;
    XmTextPart text;
} XmTextRec;

/* Semi-Public functions for internal text use only. */

#ifdef _NO_PROTO
extern int _XmTextGetBaseLine();
extern void _XmTextMarkRedraw ();
extern LineNum _XmTextNumLines ();
extern void _XmTextLineInfo ();
extern LineNum _XmTextPosToLine ();
extern void _XmTextInvalidate ();
extern void _XmTextDisableRedisplay ();
extern void _XmTextEnableRedisplay ();
extern Boolean _XmTextShouldWordWrap ();
extern Boolean _XmTextScrollable ();
extern Boolean _XmTextSetDestination ();
extern XmTextPosition _XmTextGetAnchor ();
extern void _XmTextOutputCreate ();
extern void _XmTextInputCreate ();
extern void _XmTextChangeHOffset ();
extern void _XmTextDrawDestination();
extern void _XmTextClearDestination();
extern void _XmTextDestinationVisible();
extern void _XmTextSetCursorPosition();
extern XmTextPosition _XmTextFindScroll();
extern void _XmTextChangeBlinkBehavior();
#else /* _NO_PROTO */
extern int _XmTextGetBaseLine(XmTextWidget widget);
extern void _XmTextMarkRedraw (XmTextWidget widget, XmTextPosition left, XmTextPosition right);
extern LineNum _XmTextNumLines (XmTextWidget widget);
extern void _XmTextLineInfo (XmTextWidget widget, LineNum line, XmTextPosition *startpos, LineTableExtra *extra);
extern LineNum _XmTextPosToLine (XmTextWidget widget, XmTextPosition position);
extern void _XmTextInvalidate (XmTextWidget widget, XmTextPosition position, XmTextPosition topos, long delta);
extern void _XmTextDisableRedisplay (XmTextWidget widget, Boolean losesbackingstore);
extern void _XmTextEnableRedisplay (XmTextWidget widget);
extern Boolean _XmTextShouldWordWrap (XmTextWidget widget);
extern Boolean _XmTextScrollable (XmTextWidget widget);
extern Boolean _XmTextSetDestination (Widget widget, XmTextPosition position, Boolean disown, Time set_time);
extern XmTextPosition _XmTextGetAnchor (XmTextWidget widget);
extern void _XmTextOutputCreate (XmTextWidget widget, ArgList args, Cardinal num_args);
extern void _XmTextInputCreate (XmTextWidget widget, ArgList args, Cardinal num_args);
extern void _XmTextChangeHOffset (XmTextWidget widget, int length);
extern void _XmTextDrawDestination(XmTextWidget widget);
extern void _XmTextClearDestination(XmTextWidget widget, Boolean ignore_sens);
extern void _XmTextDestinationVisible(Widget w, Boolean turn_on);
extern void _XmTextSetCursorPosition(Widget w, XmTextPosition position);
extern XmTextPosition _XmTextFindScroll (XmTextWidget widget, XmTextPosition start, int delta);
extern void _XmTextChangeBlinkBehavior(XmTextWidget widget, Boolean newvalue);
#endif /* _NO_PROTO */

#endif /* _XmnextP_h */
/* DON't ADD STUFF AFTER THIS #endif */
