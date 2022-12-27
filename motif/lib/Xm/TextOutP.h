#ifdef REV_INFO
#ifndef lint
static char SCCSID[] = "OSF/Motif: @(#)TextOutP.h	3.13 91/01/10";
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
#ifndef _TextOutP_h
#define _TextOutP_h

/****************************************************************
 *
 * Definitions for modules implementing and using text output routines.
 *
 ****************************************************************/

#define ShouldWordWrap(data, widget)	(data->wordwrap && \
       (!(data->scrollhorizontal && \
       (XtClass(widget->core.parent) == xmScrolledWindowWidgetClass))) \
       && (widget->text.edit_mode != XmSINGLE_LINE_EDIT))

typedef struct _LineTableExtraRec {
    Dimension width;
    Boolean wrappedbychar;
} LineTableExtraRec;

/*         
 * output.c  (part of stext)
 */

typedef unsigned int LineNum;
typedef enum {on, off} OnOrOff;	/* For when Booleans aren't obvious enough. */

/*
 * Return the line number containing the given position.  If text currently
 * knows of no line containing that position, returns NOLINE.
 */

#define NOLINE	30000

/*
 * These next define the types of the routines that output is required
 * to export for use by text and by input.
 */

typedef struct _OutputDataRec {
    XmFontList fontlist;	/* Fontlist for text. */
    unsigned int blinkrate;
    Boolean wordwrap;		/* Whether to wordwrap. */
    Boolean cursor_position_visible;
    Boolean autoshowinsertpoint;
    Boolean hasfocus;
    Boolean has_rect;
    Boolean handlingexposures;	/* TRUE if in the midst of expose events. */
    Boolean exposevscroll;	/* Non-zero if we expect expose events to be
				   off vertically. */
    Boolean exposehscroll;	/* Non-zero if we expect expose events to be
				   off horizontally. */
    Boolean resizewidth, resizeheight;
    Boolean scrollvertical, scrollhorizontal;
    Boolean scrollleftside, scrolltopside;
    Boolean ignorevbar;		/* Whether to ignore callbacks from vbar. */
    Boolean ignorehbar;		/* Whether to ignore callbacks from hbar. */
    Boolean dest_visible;	/* Whether destination cursor is visible. */
    Boolean dest_on;		/* Whether destination cursor is on. */
    OnOrOff onoroff;		/* Whether insertion point is being shown. */
    OnOrOff blinkstate;
    Position insertx, inserty;
    int number_lines;		/* Number of lines that fit in the window. */
    int leftmargin, rightmargin;
    int topmargin, bottommargin;
    int scrollwidth;	/* Total width of text we have to display. */
    int vsliderSize;			/* How big the thumb is in the vbar. */
    int hoffset;		/* How much we've scrolled off the left. */
    short columns, rows;
    Dimension lineheight;	/* Number of pixels per line. */
    Dimension minwidth, minheight;
    Dimension averagecharwidth;	/* Number of pixels for an "average" char. */
    Dimension tabwidth;		/* Number of pixels for a tab. */
    Dimension prevW;
    Dimension prevH;
    Dimension cursorwidth, cursorheight;
    XtIntervalId timerid;
    Pixmap cursor;		/* Pixmap to use for insertion cursor. */
    Pixmap add_mode_cursor;	/* Pixmap to use for add mode cursor. */
    Pixmap dest_cursor;		/* Pixmap to use for destination cursor. */
    Pixmap putback;		/* Last pixmap used for cursor. */
    Pixmap stipple_tile;	/* stiiple for add mode cursor. */
    GC gc, imagegc;
    Widget vbar, hbar;
    XmTextWidget widget;	/* Back-pointer to the widget record. */
    XFontStruct *font;		/* font used when NULL font is set. */
} OutputDataRec, *OutputData;


/*
 * Create a new instance of an output object.  This is expected to fill in
 * info about innerwidget and output in the widget record.
 */

#define PASTENDPOS	2147483647  /* Biggest number that can fit in long */

#ifdef _NO_PROTO

typedef void (*OutputCreateProc)(); /* widget, arglist, argcount */
    /* XmTextWidget widget; */
    /* ArgList arglist; */
    /* int argcount; */

/*
 * Given an (x,y) coordinate, return the closest corresponding position. (For
 * use by input; text shouldn't ever need to know.)
 */

typedef XmTextPosition (*XYToPosProc)(); /* ctx, x, y */
    /* XmTextWidget ctx; */
    /* Position x, y; */	/* These are relative to the innerwindow */
				/* returned above. */

/*
 * Return the (x,y) coordinate corresponing to the given position.  If this
 * returns FALSE, then the given position isn't being displayed.
 */

typedef Boolean (*PosToXYProc)(); /* widget, position, x, y */
    /* XmTextWidget widget; */
    /* XmTextPosition position; */
    /* Position *x, *y; */	/* These are relative to the innerwindow */
				/* returned above. */



/*
 * Measures the extent of a line.  Given the line number and starting position
 * of a line, returns the starting position of the next line.  Also returns
 * any extra information that the output module may want to associate with
 * this line for future reference.  (In particular, it will want to associate
 * a vertical coordinate for this line.)  This routine should return FALSE if
 * it decides that this line will not fit in the window.  FALSE should never
 * be returned if the line number is zero.  Output may assume that the line
 * table for all preceeding lines have already been set.  In particular, when
 * this routine will return FALSE, then output knows that the entire linetable
 * has been calculated; that is a good time for it to look over the linetable
 * and decide if it wants to do something obnoxious like resize the window.
 *
 * A possible value to put in nextpos is PASTENDPOS.  This indicates that the
 * current line contains the end of the text in the source.
 *
 * nextpos may be NULL.  If it is, then this indicates that we only want to
 * know if the line will fit on the window.  The caller already has its own
 * idea where the next line will start if it does fit.  (If nextpos is NULL,
 * then no extra information should be generated, and the 'extra' parameter
 * should be ignored.)
 */

typedef Boolean (*MeasureLineProc)(); /* ctx, line, pos, nextpos, extra */
    /* XmTextWidget ctx; */
    /* LineNum line; */
    /* XmTextPosition pos; */
    /* XmTextPosition *nextpos; */	/* RETURN */
    /* LineTableExtra *extra; */	/* RETURN */



/*
 * Draw some text within a line.  The output finds out information about the
 * line by calling the line table routines.
 *
 * %%% Document special cases (like lines containing PASTENDPOS).
 */

typedef void (*DrawProc)();	/* ctx, line, start, end, highlight */
    /* XmTextWidget ctx; */
    /* LineNum line; */
    /* XmTextPosition start, end; */
    /* XmHighlightMode highlight; */



/*
 * Output should draw or erase an insertion point at the given position.
 */

typedef void (*DrawInsertionPointProc)(); /* ctx, position, onoroff */
    /* XmTextWidget ctx; */
    /* XmTextPosition position; */
    /* OnOrOff onoroff; */


/*
 * Output should ensure that the given position is visible (e.g., not scrolled
 * off horizontally).
 */

typedef void (*MakePositionVisibleProc)();	/* widget, position */
    /* Widget widget; */
    /* XmTextPosition position; */


/* Text would like to move some lines around on the screen.  It would like to
 * move lines fromline through toline (inclusive) to now start at line
 * destline.  If output can perform this move by means of a XCopyArea or
 * similar simple call, it does so and returns TRUE; otherwise, it will return
 * FALSE.  If TRUE, output should modify affected values in the
 * "extra" entries in the linetable, because after returning text will go ahead
 * and move linetable entries around.
 */

typedef Boolean (*MoveLinesProc)(); /* ctx, fromline, toline, destline */
    /* XmTextWidget ctx; */
    /* LineNum fromline, toline, destline; */

/*
 * Inform output of invalidated positions.
 */

typedef void (*InvalidateProc)(); /* ctx, position, topos, delta */
    /* XmTextWidget ctx; */
    /* XmTextPosition position, topos; */
    /* int delta; */

/*
 * Get preferred size of text widget based on the row and column
 * resources multiplied by font attributes as well as adding the
 * margin resource values to the computed size.
 */

typedef void (*GetPreferredSizeProc)(); /* widget, width, height */
    /* XmTextWidget widget; */
    /* Dimension * width, height; */

/*
 * Get values out of the output object.
 */

typedef void (*GetValuesProc)(); /* widget, args, num_args */
    /* XmTextWidget widget; */
    /* ArgList args; */
    /* int num_args; */

/*
 * Set values in the output object.
 */

typedef Boolean (*SetValuesProc)(); /* widget, args, num_args */
    /* XmTextWidget widget; */
    /* ArgList args; */
    /* int num_args; */

#else /* _NO_PROTO */
#if defined(__cplusplus) || defined(c_plusplus)
extern "C" {
#endif
typedef void (*OutputCreateProc)(XmTextWidget widget, ArgList arglist,
				 int argcount);
typedef XmTextPosition (*XYToPosProc)(XmTextWidget ctx,
				Position x, Position y);
typedef Boolean (*PosToXYProc)(XmTextWidget widget, XmTextPosition position,
				Position *x, Position *y);
typedef Boolean (*MeasureLineProc)(XmTextWidget ctx, LineNum line,
				XmTextPosition pos, XmTextPosition *nextpos,
				LineTableExtra *extra);
typedef void (*DrawProc)(XmTextWidget ctx, LineNum line, XmTextPosition start,
				XmTextPosition end, XmHighlightMode highlight);
typedef void (*DrawInsertionPointProc)(XmTextWidget ctx,
				XmTextPosition position, OnOrOff onoroff);
typedef void (*MakePositionVisibleProc)(Widget widget, XmTextPosition position);
typedef Boolean (*MoveLinesProc)(XmTextWidget ctx, LineNum fromline,
				LineNum toline, LineNum destline);
typedef void (*InvalidateProc)(XmTextWidget ctx, XmTextPosition position, 
				XmTextPosition topos, long delta);
typedef void (*GetPreferredSizeProc)(XmTextWidget widget, Dimension *width,
				Dimension *height);
typedef void (*GetValuesProc)(XmTextWidget widget, ArgList args, int num_args);
typedef Boolean (*SetValuesProc)(XmTextWidget widget, ArgList args,
				int num_args);

#if defined(__cplusplus) || defined(c_plusplus)
};
#endif
#endif /* else _NO_PROTO */
 
typedef void (*XmTextResizeProc)(
#ifndef _NO_PROTO
Widget,
Boolean
#endif
);

typedef struct _OutputRec {
    struct _OutputDataRec	*data; /* Output-specific data; opaque type. */
    XYToPosProc			XYToPos;
    PosToXYProc			PosToXY;
    MeasureLineProc		MeasureLine;
    DrawProc			Draw;
    DrawInsertionPointProc	DrawInsertionPoint;
    MakePositionVisibleProc	MakePositionVisible;
    MoveLinesProc		MoveLines;
    InvalidateProc		Invalidate;
    GetPreferredSizeProc	GetPreferredSize;
    GetValuesProc		GetValues;
    SetValuesProc		SetValues;
    XtRealizeProc		realize;
    XtWidgetProc		destroy;
    XmTextResizeProc		resize;
    XtExposeProc		expose;
} OutputRec;

#endif /* _TextOutP_h */
/* DON'T ADD ANYTHING AFTER THIS #endif */
