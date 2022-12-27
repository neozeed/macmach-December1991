#ifdef REV_INFO
#ifndef lint
static char SCCSID[] = "OSF/Motif: @(#)Text.c	3.37.1.7 91/04/01";
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
#define TEXT

#include <stdio.h>
#include <X11/Xos.h>

#include <X11/Xatom.h>
#include <X11/keysymdef.h>
#include <Xm/TextP.h>
#include <Xm/TextF.h>
#include <Xm/ScrolledW.h>

/* Resolution independence conversion functions */

#define MESSAGE2 "Invalid source, source ignored."
#define MESSAGE3 "Invalid edit mode."
#define MESSAGE4 "Text widget is editable, Traversal_on must be true."

/*
 * For resource list management. 
 */

static XmTextSourceRec nullsource;
static XmTextSource nullsourceptr = &nullsource;

externalref caddr_t _XmdefaultTextActionsTable;
extern char _XmTextEventBindings1[];
extern char _XmTextEventBindings2[];
extern char _XmTextEventBindings3[];
extern Cardinal _XmdefaultTextActionsTableSize;
extern Boolean _XmTextSetDestinationSelection();

static XtResource resources[] =
{
    {
      XmNsource, XmCSource, XmRPointer, sizeof(caddr_t),
      XtOffset(XmTextWidget, text.source),
      XmRPointer, (caddr_t) &nullsourceptr
    },

    {
      XmNactivateCallback, XmCCallback, XmRCallback, sizeof(XtCallbackList),
      XtOffset (XmTextWidget, text.activate_callback),
      XmRCallback, NULL
    },

    {
      XmNfocusCallback, XmCCallback, XmRCallback, sizeof(XtCallbackList),
      XtOffset (XmTextWidget, text.focus_callback),
      XmRCallback, NULL
    },

    {
      XmNlosingFocusCallback, XmCCallback, XmRCallback, sizeof(XtCallbackList),
      XtOffset (XmTextWidget, text.losing_focus_callback),
      XmRCallback, NULL
    },

    {
      XmNvalueChangedCallback, XmCCallback, XmRCallback, sizeof(XtCallbackList),
      XtOffset (XmTextWidget, text.value_changed_callback),
      XmRCallback, NULL
    },

    {
      XmNmodifyVerifyCallback, XmCCallback, XmRCallback, sizeof(XtCallbackList),
      XtOffset (XmTextWidget, text.modify_verify_callback),
      XmRCallback, NULL
    },

    {
      XmNmotionVerifyCallback, XmCCallback, XmRCallback, sizeof(XtCallbackList),
      XtOffset (XmTextWidget, text.motion_verify_callback),
      XmRCallback, NULL
    },

    {
      XmNgainPrimaryCallback, XmCCallback, XmRCallback, sizeof(XtCallbackList),
      XtOffset (XmTextWidget, text.gain_primary_callback),
      XmRCallback, NULL
    },

    {
      XmNlosePrimaryCallback, XmCCallback, XmRCallback, sizeof(XtCallbackList),
      XtOffset (XmTextWidget, text.lose_primary_callback),
      XmRCallback, NULL
    },

    {
      XmNvalue, XmCValue, XmRString, sizeof(XmString),
      XtOffset(XmTextWidget, text.value),
      XmRString, ""
    },

    {
      XmNmaxLength, XmCMaxLength, XmRInt, sizeof(int),
      XtOffset(XmTextWidget, text.max_length),
      XmRImmediate, (caddr_t) MAXINT
    },

    {
      XmNmarginHeight, XmCMarginHeight, XmRVerticalDimension, sizeof(Dimension),
      XtOffset(XmTextWidget, text.margin_height),
      XmRImmediate, (caddr_t) 5
    },

    {
      XmNmarginWidth, XmCMarginWidth, XmRHorizontalDimension, sizeof(Dimension),
      XtOffset(XmTextWidget, text.margin_width),
      XmRImmediate, (caddr_t) 5
    },

    {
      XmNoutputCreate, XmCOutputCreate,  XmRFunction, sizeof(OutputCreateProc),
      XtOffset(XmTextWidget, text.output_create),
      XmRFunction, (caddr_t) NULL
    },

    {
      XmNinputCreate, XmCInputCreate, XmRFunction, sizeof(InputCreateProc),
      XtOffset(XmTextWidget, text.input_create),
      XmRFunction, (caddr_t) NULL
    },

    {
      XmNtopCharacter, XmCTextPosition, XmRTextPosition, sizeof(XmTextPosition),
      XtOffset(XmTextWidget, text.top_character),
      XmRImmediate, (caddr_t) 0
    },

    {
      XmNcursorPosition, XmCCursorPosition, XmRTextPosition,
      sizeof (XmTextPosition),
      XtOffset (XmTextWidget, text.cursor_position),
      XmRImmediate, (caddr_t) 0
    },

    {
      XmNeditMode, XmCEditMode, XmREditMode, sizeof(int),
      XtOffset(XmTextWidget, text.edit_mode),
      XmRImmediate, (caddr_t) XmSINGLE_LINE_EDIT
    },

    {
      XmNautoShowCursorPosition, XmCAutoShowCursorPosition, XmRBoolean,
      sizeof(Boolean),
      XtOffset(XmTextWidget, text.auto_show_cursor_position),
      XmRImmediate, (caddr_t) True
    },

    {
      XmNeditable, XmCEditable, XmRBoolean, sizeof(Boolean),
      XtOffset(XmTextWidget, text.editable),
      XmRImmediate, (caddr_t) True
    },

    {
      XmNverifyBell, XmCVerifyBell, XmRBoolean, sizeof(Boolean),
      XtOffset(XmTextWidget, text.verify_bell),
      XmRImmediate, (caddr_t) True
    },

   {
     XmNnavigationType, XmCNavigationType, XmRNavigationType,
     sizeof (unsigned char),
     XtOffset (XmPrimitiveWidget, primitive.navigation_type),
     XmRImmediate, (caddr_t) XmTAB_GROUP
   },

};

/* Definition for resources that need special processing in get values */

static XmSyntheticResource get_resources[] =
{
   {
     XmNmarginWidth,
     sizeof(Dimension),
     XtOffset(XmTextWidget, text.margin_width),
     _XmFromHorizontalPixels,
     _XmToHorizontalPixels
   },

   {
     XmNmarginHeight,
     sizeof(Dimension),
     XtOffset(XmTextWidget, text.margin_height),
     _XmFromVerticalPixels,
     _XmToVerticalPixels
   },
};

static void ClassPartInitialize();
static void ClassInitialize();
static void Initialize();
static void InitializeHook();
static void Realize();
static void Destroy();
static void Resize();
static void DoExpose();
static void GetValuesHook();
static Boolean SetValues();
static Boolean SetValuesHook();
static XtGeometryResult QueryGeometry();

externaldef(xmtextclassrec) XmTextClassRec xmTextClassRec = {
  {
/* core_class fields */	
    /* superclass	  */	(WidgetClass) &xmPrimitiveClassRec,
    /* class_name	  */	"XmText",
    /* widget_size	  */	sizeof(XmTextRec),
    /* class_initialize   */    ClassInitialize,
    /* class_part_initiali*/	ClassPartInitialize,
    /* class_inited       */	FALSE,
    /* initialize	  */	Initialize,
    /* initialize_hook    */	InitializeHook,
    /* realize		  */	Realize,
    /* actions		  */    NULL,
    /* num_actions	  */	0,
    /* resources	  */	resources,
    /* num_resources	  */	XtNumber(resources),
    /* xrm_class	  */	NULLQUARK,
    /* compress_motion	  */	TRUE,
    /* compress_exposure  */	XtExposeCompressMaximal,
    /* compress_enterleave*/	TRUE,
    /* visible_interest	  */	FALSE,
    /* destroy		  */	Destroy,
    /* resize		  */	Resize,
    /* expose		  */	DoExpose,
    /* set_values	  */	SetValues,
    /* set_values_hook	  */	SetValuesHook,
    /* set_values_almost  */	XtInheritSetValuesAlmost,
    /* get_values_hook    */	GetValuesHook,
    /* accept_focus	  */	NULL,
    /* version		  */	XtVersion,
    /* callback_private   */	NULL,
    /* tm_table		  */	NULL,
    /* query_geometry     */    QueryGeometry,
    /* display accel	  */	NULL,
    /* extension	  */	NULL,
  },

   {				/* primitive_class fields 	*/
      _XtInherit,   		/* Primitive border_highlight   */
      _XtInherit,   		/* Primitive border_unhighlight */
      NULL,         		/* translations                 */
      NULL,         		/* arm_and_activate           	*/
      get_resources,	    	/* get resources 	        */
      XtNumber(get_resources),	/* num get_resources            */
      NULL,         		/* extension                    */
   },

   {				/* text class fields */
      NULL,             	/* extension         */
   }
};

externaldef(xmtextwidgetclass) WidgetClass xmTextWidgetClass =
					 (WidgetClass) &xmTextClassRec;

/****************************************************************
 *
 * Definitions for the null source.
 *
 ****************************************************************/

/* ARGSUSED */
static void NullAddWidget(source, widget)
XmTextSource source;
XmTextWidget widget;
{}

/* ARGSUSED */
static void NullRemoveWidget(source, widget)
XmTextSource source;
XmTextWidget widget;
{
}

/* ARGSUSED */
static XmTextPosition NullRead(source, position, last_position, block)
XmTextSource source;
XmTextPosition position;
XmTextPosition last_position;
XmTextBlock block;
{
    block->ptr = NULL;
    block->length = 0;
    block->format = FMT8BIT;

    return 0;
}

/* ARGSUSED */
static XmTextStatus NullReplace(source, event, start, end, block)
XmTextSource source;
XEvent * event ;
XmTextPosition start, end;
XmTextBlock block;
{
    return EditError;
}

/* ARGSUSED */
static XmTextPosition NullScan(source, position, sType, dir, n, include)
XmTextSource source;
XmTextPosition position;
XmTextScanType sType;
XmTextScanDirection dir;
int n;
Boolean include;
{
    return 0;
}
 
/* ARGSUSED */
static Boolean NullGetSelection(source, start, end)
XmTextSource source;
XmTextPosition *start, *end;
{
    return FALSE;
}

/* ARGSUSED */
static void NullSetSelection(source, start, end)
XmTextSource source;
XmTextPosition start, end;
{}

static void _XmCreateCutBuffers(dpy)
Display *dpy;
{
   static XContext context = NULL;
   caddr_t junk;

   if (context == NULL) context = XUniqueContext();

   if (XFindContext(dpy, (Window)0, context, &junk)) {

      XChangeProperty(dpy, RootWindow(dpy, 0), XA_CUT_BUFFER0, XA_STRING, 8,
                      PropModeAppend, NULL, 0 );
      XChangeProperty(dpy, RootWindow(dpy, 0), XA_CUT_BUFFER1, XA_STRING, 8,
                      PropModeAppend, NULL, 0 );
      XChangeProperty(dpy, RootWindow(dpy, 0), XA_CUT_BUFFER2, XA_STRING, 8,
                      PropModeAppend, NULL, 0 );
      XChangeProperty(dpy, RootWindow(dpy, 0), XA_CUT_BUFFER3, XA_STRING, 8,
                      PropModeAppend, NULL, 0 );
      XChangeProperty(dpy, RootWindow(dpy, 0), XA_CUT_BUFFER4, XA_STRING, 8,
                      PropModeAppend, NULL, 0 );
      XChangeProperty(dpy, RootWindow(dpy, 0), XA_CUT_BUFFER5, XA_STRING, 8,
                      PropModeAppend, NULL, 0 );
      XChangeProperty(dpy, RootWindow(dpy, 0), XA_CUT_BUFFER6, XA_STRING, 8,
                      PropModeAppend, NULL, 0 );
      XChangeProperty(dpy, RootWindow(dpy, 0), XA_CUT_BUFFER7, XA_STRING, 8,
                      PropModeAppend, NULL, 0 );

      junk = NULL;   /* this makes saber happy */
      XSaveContext(dpy, (Window)0, context, junk);
   }
}

/****************************************************************
 *
 * Private definitions.
 *
 ****************************************************************/

/****************************************************************
 *
 * ClassPartInitialize
 *     Set up the fast subclassing for the widget.
 *
 ****************************************************************/

static void ClassPartInitialize(wc)
WidgetClass wc;
{
   _XmFastSubclassInit (wc, XmTEXT_BIT);
}

/****************************************************************
 *
 * ClassInitialize
 *   
 *
 ****************************************************************/

static void ClassInitialize()
{
    char * event_bindings;

    xmTextClassRec.core_class.actions =
                                 (XtActionList)_XmdefaultTextActionsTable;
    xmTextClassRec.core_class.num_actions = _XmdefaultTextActionsTableSize;

    event_bindings = (char *)XtMalloc(strlen(_XmTextEventBindings1) +
				      strlen(_XmTextEventBindings2) +
				      strlen(_XmTextEventBindings3) + 1);
    strcpy(event_bindings, _XmTextEventBindings1);
    strcat(event_bindings, _XmTextEventBindings2);
    strcat(event_bindings, _XmTextEventBindings3);
    xmTextClassRec.core_class.tm_table = event_bindings;

    nullsource.AddWidget = NullAddWidget;
    nullsource.RemoveWidget = NullRemoveWidget;
    nullsource.ReadSource = NullRead;
    nullsource.Replace = NullReplace;
    nullsource.Scan = (ScanProc)NullScan;
    nullsource.GetSelection = NullGetSelection;
    nullsource.SetSelection = NullSetSelection;
}


static void EraseInsertionPoint(widget)
XmTextWidget widget;
{
    
    if (widget->text.input->data->has_destination &&
        widget->text.dest_position != widget->text.cursor_position)
       _XmTextClearDestination(widget, False);

    if (widget->text.on_or_off != off) {
	(*widget->text.output->DrawInsertionPoint)(widget,
					 widget->text.cursor_position, off);
	widget->text.on_or_off = off;
    }
}


static void TextDrawInsertionPoint(widget)
XmTextWidget widget;
{
    if (widget->text.on_or_off != on) {
	(*widget->text.output->DrawInsertionPoint)(widget,
					 widget->text.cursor_position, on);
	widget->text.on_or_off = on;
    }

    if (widget->text.input->data->has_destination &&
        (widget->text.dest_position != widget->text.cursor_position ||
         !widget->text.output->data->hasfocus))
       _XmTextDrawDestination(widget);
}


/*
 * Mark the given range of text to be redrawn.
 */

static void AddRedraw(widget, left, right)
XmTextWidget widget;
XmTextPosition left, right;
{
    RangeRec *r = widget->text.repaint.range;
    int i;

/*  fix for PIR1877.  Backspace at the end of a singleLine widget and a 
    multiLine widget displaying one line, was not working properly.   
    Internally, data was being deleted, but this was not being reflected
    in the display.   By checking number_line to be >= 1 (not just > 1) 
    we cover the singleLine widget and one line visible cases.
*/
    if (left == widget->text.last_position 
    &&  widget->text.output->data->number_lines >= 1)

        left = (*widget->text.source->Scan)(widget->text.source, left,
                                       XmSELECT_POSITION, XmsdLeft, 1, TRUE);

    if (left < right) {
       for (i = 0; i < widget->text.repaint.number; i++) {
            if (left <= r[i].to && right >= r[i].from) {
                r[i].from = MIN(left, r[i].from);
                r[i].to = MAX(right, r[i].to);
                return;
            }
        }
        if (widget->text.repaint.number >= widget->text.repaint.maximum) {
            widget->text.repaint.maximum = widget->text.repaint.number + 1;
	    widget->text.repaint.range = r = (RangeRec *)
          XtRealloc((char *)r, widget->text.repaint.maximum * sizeof(RangeRec));
        }
        r[widget->text.repaint.number].from = left;
        r[widget->text.repaint.number].to = right;
        widget->text.repaint.number++;
    }
}


/*
 * Find the highlight record corresponding to the given position.  Returns a
 * pointer to the record.  The third argument indicates whether we are probing
 * the left or right edge of a highlighting range.
 */

static HighlightRec *FindHighlight(widget, position, dir)
XmTextWidget widget;
XmTextPosition position;
XmTextScanDirection dir;
{
    HighlightRec *l = widget->text.highlight.list;
    int i;
    if (dir == XmsdLeft) {
	for (i=widget->text.highlight.number - 1 ; i>=0 ; i--)
	    if (position >= l[i].position) {
		l = l + i;
                break;
            }
    } else {
	for (i=widget->text.highlight.number - 1 ; i>=0 ; i--)
	    if (position > l[i].position) {
		l = l + i;
                break;
            }
    }
    return(l);
}

/*
 * Redraw the specified range of text.  Should only be called by
 * RedrawChanges(), below (as well as calling itself recursively).
 */

static void DisplayText(widget, updateFrom, updateTo)
XmTextWidget widget;
XmTextPosition updateFrom, updateTo;
{
    LineNum i;
    XmTextPosition nextstart;
    HighlightRec *l1, *l2;

    if (updateFrom < widget->text.top_character)
	updateFrom = widget->text.top_character;
    if (updateTo > widget->text.bottom_position)
	updateTo = widget->text.bottom_position;
    if (updateFrom >= updateTo) return;

    l1 = FindHighlight(widget, updateFrom, XmsdLeft);
    l2 = FindHighlight(widget, updateTo, XmsdRight);
    if (l1 != l2) {
	DisplayText(widget, updateFrom, l2->position);
	updateFrom = l2->position;
    }

    /*
     * Once we get here, we need to paint all of the text from updateFrom to
     * updateTo with current highlightmode.  We have to break this into
     * separate lines, and then call the output routine for each line.
     */

    for (i = _XmTextPosToLine(widget, updateFrom);
	 updateFrom <= updateTo && i < widget->text.number_lines;
	 i++) {
	nextstart = widget->text.line[i+1].start;
	(*widget->text.output->Draw)(widget, i, updateFrom,
			     MIN(updateTo, nextstart), l2->mode);
	updateFrom = nextstart;
    }
}





/*
 * Redraw the changed areas of the text.  This should only be called by
 * Redisplay(), below. 
 */

static void RedrawChanges(widget)
XmTextWidget widget;
{
    RangeRec *r = widget->text.repaint.range;
    XmTextPosition updateFrom, updateTo;
    int w, i;

    if (widget->text.repaint.number != 0) {
        EraseInsertionPoint(widget);
    }
    while (widget->text.repaint.number != 0) {
	updateFrom = r[0].from;
	w = 0;
	for (i=1 ; i<widget->text.repaint.number ; i++) {
	    if (r[i].from < updateFrom) {
		updateFrom = r[i].from;
		w = i;
	    }
	}
	updateTo = r[w].to;
	widget->text.repaint.number--;
	r[w].from = r[widget->text.repaint.number].from;
	r[w].to = r[widget->text.repaint.number].to;
	for (i=widget->text.repaint.number-1 ; i>=0 ; i--) {
	    while (r[i].from <= updateTo && i < widget->text.repaint.number) {
		updateTo = MAX(r[i].to, updateTo);
		widget->text.repaint.number--;
		r[i].from = r[widget->text.repaint.number].from;
		r[i].to = r[widget->text.repaint.number].to;
	    }
	}
	DisplayText(widget, updateFrom, updateTo);
    }
    if (widget->text.first_position == widget->text.last_position) {
        EraseInsertionPoint(widget);
	(*widget->text.output->Draw)(widget, (LineNum) 0,
	       			     widget->text.first_position,
		 		     widget->text.last_position,
			             XmHIGHLIGHT_NORMAL);
    }
}
    

static void DoMove(widget, startcopy, endcopy, destcopy)
XmTextWidget widget;
int startcopy, endcopy, destcopy;
{
    Line line = widget->text.line;
    LineNum i;

    EraseInsertionPoint(widget);
    if ((*widget->text.output->MoveLines)(widget, (LineNum) startcopy,
				     (LineNum) endcopy, (LineNum) destcopy))
	return;
    for (i=destcopy ; i <= destcopy + endcopy - startcopy ; i++)
	AddRedraw(widget, line[i].start, line[i+1].start);
}

/*
 * Find the starting position of the line that is delta lines away from the
 * line starting with position start.
 */

XmTextPosition _XmTextFindScroll(widget, start, delta)
XmTextWidget widget;
XmTextPosition start;
int delta;
{
    XmTextPosition position, nextstart;
    register int i;

    if (delta >= 0) {
	for (i=0 ; i<delta ; i++) {
	    if (!(*widget->text.output->MeasureLine)(widget, (LineNum)0, start,
						&nextstart, NULL))
		return start;
	    if (nextstart == PASTENDPOS) return start;
	    start = nextstart;
	}
	return start;
    }

    delta = -delta;
    nextstart = start;


    for (i= 0; i < delta; i++) {
        position = (*widget->text.source->Scan)(widget->text.source, nextstart,
					   XmSELECT_LINE, XmsdLeft, 1, FALSE);
	if (position == nextstart) {
	   position = (*widget->text.source->Scan)(widget->text.source,
		 	 	position, XmSELECT_LINE, XmsdLeft, 1, TRUE);
	   position = (*widget->text.source->Scan)(widget->text.source,
			 	position, XmSELECT_LINE, XmsdLeft, 1, FALSE);
	}

        if (_XmTextShouldWordWrap(widget)) {
            while (position < start) {
		if (!(*widget->text.output->MeasureLine)(widget, (LineNum)0,
					   position, &nextstart, NULL))
                   return position;
	        if (nextstart == PASTENDPOS) return position;
                if (nextstart >= start) {
                   nextstart = position;
                   start = nextstart;
                   break;
                }
	        position = nextstart;
            }
        } else {
	  nextstart = position;
          if (nextstart < 0) {
	      nextstart = 0;
	      break;
	  }
       }
    }

    start = nextstart;
    return start;
}



/* 
 * Refigure the line breaks in this widget.
 */

static void RefigureLines(widget)
XmTextWidget widget;
{
    Line line = widget->text.line;
    LineNum i, j;
    static Line oldline = NULL;
    static Cardinal oldMaxLines = 0;
    static int tell_output_force_display = -1;
    int oldNumLines = widget->text.number_lines;
    int startcopy, endcopy, destcopy, lastcopy; /* %%% Document! */

    if (widget->text.in_refigure_lines || !widget->text.needs_refigure_lines)
        return;
    widget->text.in_refigure_lines = TRUE;
    widget->text.needs_refigure_lines = FALSE;
    EraseInsertionPoint(widget);
    if (oldMaxLines < oldNumLines + 2) {
	oldMaxLines = oldNumLines + 2;
	oldline = (Line) XtRealloc((char *)oldline,
				   oldMaxLines * sizeof(LineRec));
    }
    bcopy((char *)line, (char *)oldline, (oldNumLines + 1) * sizeof(LineRec));
    

    if (widget->text.pending_scroll != 0) {
	widget->text.new_top = _XmTextFindScroll(widget, widget->text.new_top,
					         widget->text.pending_scroll);
	widget->text.pending_scroll = 0;
    }
    if (widget->text.new_top < widget->text.first_position)
        widget->text.new_top = widget->text.first_position;
    widget->text.last_top_char = widget->text.top_character;
    line[0].start = widget->text.top_character = widget->text.new_top;
    line[0].past_end = FALSE;

    widget->text.number_lines = 0;
    j = 0;
    startcopy = endcopy = lastcopy = destcopy = -99;
    for (i = 0 ; i == 0 || !line[i-1].past_end ; i++) {
	if (i+2 >= widget->text.maximum_lines) {
	    widget->text.maximum_lines = i+2;
	    line = widget->text.line = (Line)
	       XtRealloc((char *)line, 
			widget->text.maximum_lines * sizeof(LineRec));
	}
	while (j < oldNumLines && oldline[j].start < line[i].start)
	    j++;
	if (j < oldNumLines && oldline[j].start >= oldline[j+1].start)
	    j = oldNumLines;
	if (j >= oldNumLines)
	    oldline[j].start = -1; /* Make comparisons fail. */
	if (line[i].start >= widget->text.forget_past ||
	      line[i].start != oldline[j].start ||
	      oldline[j].changed ||
	      oldline[j+1].changed) {
	    line[i].past_end =
		!(*widget->text.output->MeasureLine)(widget, i, line[i].start,
					     &line[i+1].start, &line[i].extra);
	} else {
	    line[i] = oldline[j];
	    oldline[j].extra = NULL;
	    line[i].past_end =
		!(*widget->text.output->MeasureLine)(widget, i, line[i].start,
						NULL, NULL);
	    line[i+1].start = oldline[j+1].start;
	}
	if (!line[i].past_end) {
	    if (line[i].start != oldline[j].start ||
	          line[i+1].start != oldline[j+1].start ||
	          line[i].start >= widget->text.forget_past) {
		AddRedraw(widget, line[i].start, line[i+1].start);
	    } else {
		if (i != j && line[i+1].start >= widget->text.last_position)
		  AddRedraw(widget, widget->text.last_position,
					 widget->text.last_position);
		if (oldline[j].changed)
		    AddRedraw(widget, oldline[j].changed_position,
							 line[i+1].start);
		if (i != j && line[i].start != PASTENDPOS) {
		    if (endcopy == j-1) {
			endcopy = j;
			lastcopy++;
		    } else if (lastcopy >= 0 && j <= lastcopy) {
			/* This line was stomped by a previous move. */
			AddRedraw(widget, line[i].start, line[i+1].start);
		    } else {
			if (startcopy >= 0)
			    DoMove(widget, startcopy, endcopy, destcopy);
			startcopy = endcopy = j;
			destcopy = lastcopy = i;
		    }
		}
	    }
	}
	line[i].changed = FALSE;
	if (!line[i].past_end) widget->text.number_lines++;
	else widget->text.bottom_position =
			 MIN(line[i].start, widget->text.last_position);
    }
    if (startcopy >= 0)
	DoMove(widget, startcopy, endcopy, destcopy);
    for (j=0 ; j<oldNumLines ; j++)
	if (oldline[j].extra) {
	    XtFree((char *) oldline[j].extra);
	    oldline[j].extra = NULL;
	};  /* without this semi, the following statement gets executed
               as part of the for.   a set of braces for the for would
               do the same thing */
    widget->text.in_refigure_lines = FALSE;

/*  fix for PIR2460.   End file, on a singleLine widget and a multiLine
    widget displaying one line, was not working properly.   Internally
    we went to the end of the file, but no refresh was done.   The I-beam
    was drawn before the first character, but any new characters would
    be added to the end of the file.    By checking number_line to be
    >= 1 (not just > 1) we cover the singleLine widget and one line
    visible cases.
*/
    if (widget->text.top_character >= widget->text.last_position 
    &&  widget->text.last_position > widget->text.first_position 
    &&  widget->text.output->data->number_lines >= 1 )
    {
       widget->text.pending_scroll = -1; /* Try to not ever display nothing. */
       widget->text.needs_refigure_lines = TRUE;
    }
    if (widget->text.force_display >= 0) {
      if (widget->text.force_display < widget->text.top_character) {
         widget->text.new_top = (*widget->text.source->Scan)
					(widget->text.source,
					 widget->text.force_display,
				         XmSELECT_LINE, XmsdLeft, 1, FALSE);
	 widget->text.needs_refigure_lines = TRUE;
      } else if (widget->text.force_display > widget->text.bottom_position) {
       /* need to add one to account for border condition,
        * i.e. cursor at begginning of line
        */
         widget->text.new_top = widget->text.force_display + 1;
	 widget->text.needs_refigure_lines = TRUE;
	 widget->text.pending_scroll -= widget->text.number_lines;
      } else if (widget->text.force_display ==
			 line[widget->text.number_lines].start) {
         widget->text.new_top = widget->text.force_display;
	 widget->text.pending_scroll -= (widget->text.number_lines - 1);
	 widget->text.needs_refigure_lines = TRUE;
      }
      tell_output_force_display = widget->text.force_display;
      widget->text.force_display = -1;
    }
    if (widget->text.needs_refigure_lines) {
	RefigureLines(widget);
	return;
    }
    AddRedraw(widget, widget->text.forget_past, widget->text.bottom_position);
    widget->text.forget_past = MAXINT;
    if (tell_output_force_display >= 0) {
        (*widget->text.output->MakePositionVisible)((Widget)widget,
                                                   tell_output_force_display);
	tell_output_force_display = -1;
    }
}


/*
 * Compare the old_highlight list and the highlight list, determine what
 * changed, and call AddRedraw with the changed areas.
 */

static void FindHighlightingChanges(widget)
XmTextWidget widget;
{
    int n1 = widget->text.old_highlight.number;
    int n2 = widget->text.highlight.number;
    HighlightRec *l1 = widget->text.old_highlight.list;
    HighlightRec *l2 = widget->text.highlight.list;
    int i1, i2;
    XmTextPosition next1, next2, last_position;

    i1 = i2 = 0;
    last_position = 0;
    while (i1 < n1 && i2 < n2) {
	if (i1 < n1-1) next1 = l1[i1+1].position;
	else next1 = widget->text.last_position;
	if (i2 < n2-1) next2 = l2[i2+1].position;
	else next2 = widget->text.last_position;
	if (l1[i1].mode != l2[i2].mode) {
	    AddRedraw(widget, last_position, MIN(next1, next2));
	}
	last_position = MIN(next1, next2);
	if (next1 <= next2) i1++;
	if (next1 >= next2) i2++;
    }
}




/*
 * Actually do some work.  This routine gets called to actually paint all the
 * stuff that has been pending. Prevent recursive calls and text redisplays
 * during destroys 
 */


static void Redisplay(widget)
XmTextWidget widget;
{
   /* Prevent recursive calls. */
    if (widget->text.in_redisplay || widget->core.being_destroyed) return;

    widget->text.in_redisplay = TRUE;

    if (widget->text.input->data->has_destination &&
        (widget->text.dest_position != widget->text.cursor_position ||
         !widget->text.output->data->hasfocus))
       _XmTextClearDestination(widget, False);

    RefigureLines(widget);
    widget->text.needs_redisplay = FALSE;

    if (widget->text.highlight_changed) {
	FindHighlightingChanges(widget);
	widget->text.highlight_changed = FALSE;
    }

    RedrawChanges(widget);

    TextDrawInsertionPoint(widget);

   /* Can be caused by auto-horiz scrolling... */
    if (widget->text.needs_redisplay) {
	RedrawChanges(widget);
        TextDrawInsertionPoint(widget);
	widget->text.needs_redisplay = FALSE;
    }
    widget->text.in_redisplay = FALSE;
}



/****************************************************************
 *
 * Definitions exported to output.
 *
 ****************************************************************/

/*
 * Mark the given range of text to be redrawn.
 */

#ifdef _NO_PROTO
void _XmTextMarkRedraw(widget, left, right)
XmTextWidget widget;
XmTextPosition left, right;
#else /* _NO_PROTO */
void _XmTextMarkRedraw (XmTextWidget widget, XmTextPosition left, XmTextPosition right)
#endif /* _NO_PROTO */
{
    if (left < right) {
 	AddRedraw(widget, left, right);
	widget->text.needs_redisplay = TRUE;
	if (widget->text.disable_depth == 0) Redisplay(widget);
    }
}


/*
 * Return the number of lines in the linetable.
 */

#ifdef _NO_PROTO
LineNum _XmTextNumLines(widget)
XmTextWidget widget;
#else /* _NO_PROTO */
LineNum _XmTextNumLines (XmTextWidget widget)
#endif /* _NO_PROTO */
{
    if (widget->text.needs_refigure_lines) RefigureLines(widget);
    return widget->text.number_lines;
}

#ifdef _NO_PROTO
void _XmTextLineInfo(widget, line, startpos, extra)
XmTextWidget widget;
LineNum line;
XmTextPosition *startpos;	/* RETURN */
LineTableExtra *extra;		/* RETURN */
#else /* _NO_PROTO */
void _XmTextLineInfo (XmTextWidget widget, LineNum line, XmTextPosition *startpos, LineTableExtra *extra)
#endif /* _NO_PROTO */
{
    if (widget->text.needs_refigure_lines) RefigureLines(widget);
    if (startpos) *startpos = widget->text.line[line].start;
    if (extra) *extra = widget->text.line[line].extra;
}

/*
 * Return the line number containing the given position.  If text currently
 * knows of no line containing that position, returns NOLINE.
 */

#ifdef _NO_PROTO
LineNum _XmTextPosToLine(widget, position)
XmTextWidget widget;
XmTextPosition position;
#else /* _NO_PROTO */
LineNum _XmTextPosToLine (XmTextWidget widget, XmTextPosition position)
#endif /* _NO_PROTO */
{
    int i;
    if (widget->text.needs_refigure_lines) RefigureLines(widget);
    if (position < widget->text.top_character ||
			 position  > widget->text.bottom_position)
	return NOLINE;
    for (i=0 ; i<widget->text.number_lines ; i++)
	if (widget->text.line[i+1].start > position) return i;
    if (position == widget->text.line[widget->text.number_lines].start)
	return widget->text.number_lines;
    return NOLINE;  /* Couldn't find line with given position */ 
}



/****************************************************************
 *
 * Definitions exported to sources.
 *
 ****************************************************************/

#ifdef _NO_PROTO
void _XmTextInvalidate(widget, position, topos, delta)
XmTextWidget widget;
XmTextPosition position, topos;
long delta;
#else /* _NO_PROTO */
void _XmTextInvalidate (XmTextWidget widget, XmTextPosition position, XmTextPosition topos, long delta)
#endif /* _NO_PROTO */
{
    LineNum l;
    int i;
    XmTextPosition p, endpos;

#define ladjust(p) if ((p > position && p != PASTENDPOS) ||		       \
	(p == position && delta < 0)) {				     	       \
    	  p += delta;						     	       \
	  if (p < widget->text.first_position) p = widget->text.first_position;\
	  if (p > widget->text.last_position) p = widget->text.last_position;  \
	}

#define radjust(p) if ((p > position && p != PASTENDPOS) ||		       \
	(p == position && delta > 0)) {			                       \
    	  p += delta;					       		       \
	  if (p < widget->text.first_position) p = widget->text.first_position;\
	  if (p > widget->text.last_position) p = widget->text.last_position;  \
	}

    widget->text.first_position = (*widget->text.source->Scan)
					(widget->text.source, 0,
					 XmSELECT_ALL, XmsdLeft, 1, FALSE);
    widget->text.last_position = (*widget->text.source->Scan)
					(widget->text.source,  0,
					 XmSELECT_ALL, XmsdRight, 1, FALSE);
    if (widget->text.top_character == topos && position != topos) {
	widget->text.pending_scroll = -1;
	widget->text.forget_past = MIN(widget->text.forget_past, position);
    }
    if (widget->text.top_character >= position &&
            widget->text.bottom_position <= topos) {
	widget->text.new_top = position;
	widget->text.pending_scroll = -1;
	widget->text.forget_past = MIN(widget->text.forget_past, position);
    }
    if (delta == NODELTA) {
	widget->text.forget_past = MIN(widget->text.forget_past, position);
    } else {
	for (i=0 ; i<widget->text.repaint.number ; i++) {
	    radjust(widget->text.repaint.range[i].from);
	    ladjust(widget->text.repaint.range[i].to);
	}
	for (i=0 ; i<widget->text.highlight.number ; i++)
	    ladjust(widget->text.highlight.list[i].position);
	for (i=0 ; i<widget->text.old_highlight.number ; i++)
	    ladjust(widget->text.old_highlight.list[i].position);
	for (i=0 ; i<=widget->text.number_lines ; i++) {
	    radjust(widget->text.line[i].start);
	    if (widget->text.line[i].changed)
		radjust(widget->text.line[i].changed_position);
	}
	if (widget->text.top_character != position)
	    radjust(widget->text.top_character);
	if (widget->text.new_top != position)
	    radjust(widget->text.new_top);
	ladjust(widget->text.bottom_position);
	radjust(widget->text.cursor_position);
	endpos = topos;
	radjust(endpos);

      /* Force _XmTextPosToLine to not bother trying to recalculate. */
	widget->text.needs_refigure_lines = FALSE;
	for (l = _XmTextPosToLine(widget, position), p = position ;
	     l < widget->text.number_lines &&
             widget->text.line[l].start <= endpos ;
	     l++, p = widget->text.line[l].start) {
	    if (l != NOLINE) {
		if (widget->text.line[l].changed) {
		    widget->text.line[l].changed_position = MIN(p,
					 widget->text.line[l].changed_position);
		} else {
		    widget->text.line[l].changed_position = p;
		    widget->text.line[l].changed = TRUE;
		}
	    }
	}
    }
    (*widget->text.output->Invalidate)(widget, position, topos, delta);
    (*widget->text.input->Invalidate)(widget, position, topos, delta);
    widget->text.needs_refigure_lines = widget->text.needs_redisplay = TRUE;
    if (widget->text.disable_depth == 0) Redisplay(widget);
}


static void InsertHighlight(widget, position, mode)
XmTextWidget widget;
XmTextPosition position;
XmHighlightMode mode;
{
    HighlightRec *l1;
    HighlightRec *l = widget->text.highlight.list;
    int i, j;

    l1 = FindHighlight(widget, position, XmsdLeft);
    if (l1->position == position)
	l1->mode = mode;
    else {
	i = (l1 - l) + 1;
	widget->text.highlight.number++;
	if (widget->text.highlight.number > widget->text.highlight.maximum) {
	    widget->text.highlight.maximum = widget->text.highlight.number;
	    l = widget->text.highlight.list = (HighlightRec *)
		XtRealloc((char *) l,
			 widget->text.highlight.maximum * sizeof(HighlightRec));
	}
	for (j=widget->text.highlight.number-1 ; j>i ; j--)
	    l[j] = l[j-1];
	l[i].position = position;
	l[i].mode = mode;
    }
}
	

#ifdef _NO_PROTO
void XmTextSetHighlight(w, left, right, mode)
Widget w;
XmTextPosition left, right;
XmHighlightMode mode;
#else /* _NO_PROTO */
void XmTextSetHighlight (Widget w, XmTextPosition left, XmTextPosition right, XmHighlightMode mode)
#endif /* _NO_PROTO */
{
    XmTextWidget widget = (XmTextWidget)w;
    HighlightRec *l;
    XmHighlightMode endmode;
    int i, j;
    
    if (left >= right || right <= 0 ||
	right > widget->text.last_position) return;

    if (!widget->text.highlight_changed) {
       widget->text.highlight_changed = TRUE;
       if (widget->text.old_highlight.maximum < widget->text.highlight.number) {
           widget->text.old_highlight.maximum = widget->text.highlight.number;
           widget->text.old_highlight.list = (HighlightRec *)
  	 XtRealloc((char *)widget->text.old_highlight.list,
	           widget->text.old_highlight.maximum * sizeof(HighlightRec));
       }
       widget->text.old_highlight.number = widget->text.highlight.number;
       bcopy((char*) widget->text.highlight.list,
             (char*) widget->text.old_highlight.list,
              widget->text.old_highlight.number * sizeof(HighlightRec));
    }
    endmode = FindHighlight(widget, right, XmsdLeft)->mode;
    InsertHighlight(widget, left, mode);
    InsertHighlight(widget, right, endmode);
    l = widget->text.highlight.list;
    i = 1;
    while (i < widget->text.highlight.number) {
	if (l[i].position >= left && l[i].position < right)
	    l[i].mode = mode;
	if (l[i].mode == l[i-1].mode) {
	    widget->text.highlight.number--;
	    for (j=i ; j<widget->text.highlight.number ; j++)
		l[j] = l[j+1];
	} else i++;
    }
    widget->text.needs_redisplay = TRUE;
    if (widget->text.disable_depth == 0)
	Redisplay(widget);
}

/****************************************************************
 *
 * Creation definitions.
 *
 ****************************************************************/

/*
 * Create the text widget.  To handle default condition of the core
 * height and width after primitive has already reset it's height and
 * width, use request values and reset height and width to original
 * height and width state.
 */

static void Initialize(req, new) 
XmTextWidget req, new;
{
    if (req->core.width == 0) new->core.width = req->core.width;
    if (req->core.height == 0) new->core.height = req->core.height;

    if (!new->primitive.traversal_on && new->text.editable) {
        _XmWarning (new, MESSAGE4);
        new->primitive.traversal_on = True;
    }

 /* Flag used in losing focus verification to indicate that a traversal
    key was pressed.  Must be initialized to False */
    new->text.traversed = False;

    new->text.total_lines = 1;
    new->text.top_line = 0;
    new->text.vsbar_scrolling = False;

    if (new->text.output_create == NULL)
	new->text.output_create = (OutputCreateProc)_XmTextOutputCreate;
    if (new->text.input_create == NULL)
	new->text.input_create = _XmTextInputCreate;

   /*  The following resources are defaulted to invalid values to indicate    */
   /*  that it was not set by the application.  If it gets to this point      */
   /*  and they are still invalid then set them to their appropriate default. */

   if (new->text.edit_mode != XmSINGLE_LINE_EDIT &&
       new->text.edit_mode != XmMULTI_LINE_EDIT) {
	_XmWarning (new, MESSAGE3);
	new->text.edit_mode = XmSINGLE_LINE_EDIT;
   }

   /* All 8 buffers must be created to be able to rotate the cut buffers */
   _XmCreateCutBuffers(XtDisplay(new));
}


/*
 * Create a text widget.  Note that most of the standard stuff is actually
 * to be done by the output create routine called here, since output is in
 * charge of window handling.
 */

static void InitializeHook(widget, args, num_args_ptr)
XmTextWidget widget;
ArgList args;
Cardinal *num_args_ptr;
{
    Cardinal num_args = *num_args_ptr;
    XmTextSource source;

    if (widget->text.source == nullsourceptr)
       source = _XmStringSourceCreate(widget->text.value);
    else
       source = widget->text.source;

    widget->text.disable_depth = 1;
    widget->text.first_position = 0;
    widget->text.last_position = 0;
    widget->text.dest_position = widget->text.cursor_position;

    (*widget->text.output_create)(widget, args, num_args);
    (*widget->text.input_create)(widget, args, num_args);

    widget->text.needs_refigure_lines = widget->text.needs_redisplay = TRUE;
    widget->text.number_lines = 0;
    widget->text.maximum_lines = 1;
    widget->text.line = (Line) XtMalloc(sizeof(LineRec));
    widget->text.repaint.number = widget->text.repaint.maximum = 0;
    widget->text.repaint.range = (RangeRec *) XtMalloc(sizeof(RangeRec));
    widget->text.highlight.number = widget->text.highlight.maximum = 1;
    widget->text.highlight.list = (HighlightRec *)
	XtMalloc(sizeof(HighlightRec));
    widget->text.highlight.list[0].position = 0;
    widget->text.highlight.list[0].mode = XmHIGHLIGHT_NORMAL;
    widget->text.old_highlight.number = 0;
    widget->text.old_highlight.maximum = 1;
    widget->text.old_highlight.list = (HighlightRec *)
	XtMalloc(sizeof(HighlightRec));
    widget->text.highlight_changed = FALSE;
    widget->text.on_or_off = off;
    widget->text.force_display = -1;
    widget->text.in_redisplay = widget->text.in_refigure_lines = FALSE;
    widget->text.in_resize = FALSE;
    widget->text.pending_scroll = 0;
    widget->text.last_top_char = widget->text.top_character;
    widget->text.new_top = widget->text.top_character;
    widget->text.bottom_position = 0;
    widget->text.add_mode = False;
    widget->text.pendingoff = True;
    widget->text.forget_past = 0;

    if (widget->text.value == NULL) widget->text.value = "";

    XmTextSetSource((Widget)widget, source, widget->text.top_character,
		    widget->text.cursor_position);
    XmTextSetEditable((Widget)widget, widget->text.editable);
    XmTextSetMaxLength((Widget)widget, widget->text.max_length);
    widget->text.value = NULL;

    widget->text.first_position = (*widget->text.source->Scan)
					(widget->text.source, 0,
					 XmSELECT_ALL, XmsdLeft, 1, FALSE);
    widget->text.last_position = (*widget->text.source->Scan)
					(widget->text.source, 0,
					 XmSELECT_ALL, XmsdRight, 1, FALSE);
    widget->text.forget_past = widget->text.first_position;
    
    widget->text.disable_depth = 0;
}


static void Realize(w, valueMask, attributes)
Widget w;
Mask *valueMask;
XSetWindowAttributes *attributes;
{
    XmTextWidget widget = (XmTextWidget) w;
    (*widget->text.output->realize)(widget, valueMask, attributes);
}


/****************************************************************
 *
 * Semi-public definitions.
 *
 ****************************************************************/

static void Destroy(w)
Widget w;
{
    XmTextWidget widget = (XmTextWidget) w;
    (*widget->text.source->RemoveWidget)(widget->text.source, widget);
    if (widget->text.input->destroy) (*widget->text.input->destroy)(widget);
    if (widget->text.output->destroy) (*widget->text.output->destroy)(widget);
    XtFree((char *)widget->text.line);
    XtFree((char *)widget->text.repaint.range);
    XtFree((char *)widget->text.highlight.list);
    XtFree((char *)widget->text.old_highlight.list);

    XtRemoveAllCallbacks (w, XmNactivateCallback);
    XtRemoveAllCallbacks (w, XmNfocusCallback);
    XtRemoveAllCallbacks (w, XmNlosingFocusCallback);
    XtRemoveAllCallbacks (w, XmNvalueChangedCallback);
    XtRemoveAllCallbacks (w, XmNmodifyVerifyCallback);
    XtRemoveAllCallbacks (w, XmNmotionVerifyCallback);
}

static void Resize(w)
Widget w;
{
    XmTextWidget widget = (XmTextWidget) w;
    (*widget->text.output->resize)((Widget)widget, FALSE);
}

static void DoExpose(w, event)
Widget w;
XEvent *event;
{
    XmTextWidget widget = (XmTextWidget) w;
    (*widget->text.output->expose)(widget, event, NULL);
}


static void GetValuesHook(w, args, num_args_ptr)
Widget w;
ArgList args;
Cardinal *num_args_ptr;
{
    XmTextWidget widget = (XmTextWidget) w;
    Cardinal num_args = *num_args_ptr;
    char * value;
    int i;

    XtGetSubvalues((caddr_t) widget,
		   resources, XtNumber(resources), args, num_args);

    for (i = 0; i < num_args; i++) {
        if (!strcmp(args[i].name, XmNvalue)) {
           *((XtPointer *)args[i].value) = 
	          (XtPointer)_XmStringSourceGetValue(GetSrc(widget));
        }
    }

    (*widget->text.output->GetValues)(widget, args, num_args);
    (*widget->text.input->GetValues)(widget, args, num_args);
}


/* ARGSUSED */
static Boolean SetValues(oldw, reqw, neww)
Widget oldw, reqw, neww;
{
   XmTextWidget old = (XmTextWidget) oldw;
   XmTextWidget new = (XmTextWidget) neww;
   Boolean cursor_pos_set = FALSE;

   if (new->core.being_destroyed) return False;

   if (new->core.sensitive == False &&
       new->text.input->data->has_destination) {
      _XmTextSetDestinationSelection((Widget)new, new->text.cursor_position,
                                     True, CurrentTime);
   }

   if (!new->primitive.traversal_on && new->text.editable) {
      _XmWarning (new, MESSAGE4);
      new->primitive.traversal_on = True;
   }

   if (new->text.edit_mode != XmSINGLE_LINE_EDIT &&
       new->text.edit_mode != XmMULTI_LINE_EDIT) {
	_XmWarning (new, MESSAGE3);
	new->text.edit_mode = old->text.edit_mode;
   }

    if (old->text.cursor_position != new->text.cursor_position) {
	XmTextSetCursorPosition((Widget)new, new->text.cursor_position);
        cursor_pos_set = TRUE;
    }

    if (old->text.top_character != new->text.top_character) { 
       XmTextPosition new_top = new->text.top_character;
       new->text.top_character = old->text.top_character;
       XmTextSetTopCharacter(neww, new_top);
    }

    if (old->text.source != new->text.source) {
        XmTextSource source = new->text.source;
        new->text.source = old->text.source;
        XmTextSetSource((Widget)new, source, new->text.top_character,
                        new->text.cursor_position);
    }

    XmTextSetEditable(neww, new->text.editable);
    XmTextSetMaxLength(neww, new->text.max_length);

    if (old->text.value != new->text.value &&
	new->text.value != NULL) {
        XmTextPosition cursorPos;
        XmAnyCallbackStruct cb;
 
        new->text.pendingoff = TRUE;
        _XmStringSourceSetValue(GetSrc(new), new->text.value);
         new->text.value = NULL;
        if (!cursor_pos_set) {
           cursorPos = (*new->text.source->Scan)(new->text.source,
                                           XmTextGetCursorPosition((Widget)new),
                                           XmSELECT_ALL, XmsdLeft, 1, TRUE);
           _XmTextSetCursorPosition((Widget)new, cursorPos);
        }
        /* Call ValueChanged Callback to indicate that text has been modified */
        cb.reason = XmCR_VALUE_CHANGED;
        cb.event = NULL;
        XtCallCallbackList ((Widget) new, new->text.value_changed_callback,
			   (Opaque)&cb);
    }

    return FALSE;
}

static Boolean SetValuesHook(w, args, num_args_ptr)
Widget w;
ArgList args;
Cardinal *num_args_ptr;
{
    XmTextWidget widget = (XmTextWidget) w;
    Cardinal num_args = *num_args_ptr;
    Boolean o_redisplay;

    if (widget->core.being_destroyed) return False;

    _XmTextDisableRedisplay(widget, TRUE);
    widget->text.needs_refigure_lines = widget->text.needs_redisplay = TRUE;
    o_redisplay = (*widget->text.output->SetValues)(widget, args, num_args);
    (*widget->text.input->SetValues)(widget, args, num_args);
    widget->text.forget_past = 0;
    widget->text.disable_depth--;
    widget->text.value = NULL;
    if (!o_redisplay) TextDrawInsertionPoint(widget);

    return (o_redisplay);
}

static XtGeometryResult QueryGeometry(w, intended, reply)
Widget w;
XtWidgetGeometry *intended, *reply;
{
    XmTextWidget tw = (XmTextWidget) w;

    if (GMode (intended) & (~(CWWidth | CWHeight)))
       return(XtGeometryNo);

    reply->request_mode = (CWWidth | CWHeight);

    (*tw->text.output->GetPreferredSize)((XmTextWidget)w, 
					 &reply->width, &reply->height);

    if ((reply->width != intended->width) ||
        (reply->height != intended->height) ||
        (GMode(intended) != GMode(reply)))
       return (XtGeometryAlmost);
    else {
        reply->request_mode = 0;
        return (XtGeometryYes);
    }
}

/****************************************************************
 *
 * Public definitions.
 *
 ****************************************************************/



#ifdef _NO_PROTO
Widget XmCreateScrolledText(parent, name, arglist, argcount)
Widget parent;
char *name;
ArgList arglist;
Cardinal argcount;
#else /* _NO_PROTO */
Widget XmCreateScrolledText (Widget parent, char *name, ArgList arglist, Cardinal argcount)
#endif /* _NO_PROTO */
{
  Widget swindow;
  Widget stext;
  ArgList merged_args;
  int n;
  char *s;

  s = XtMalloc(strlen(name) + 3);  /* Name + NULL + "SW" */
  strcpy(s, name);
  strcat(s, "SW");

 /*
  * merge the application arglist with the required preset arglist, for
  * creating the scrolled window portion of the scroll text.
  */
  merged_args = (ArgList)XtCalloc(argcount+4, sizeof(Arg));
  for (n=0; n < argcount; n++) {
      merged_args[n].name = arglist[n].name;
      merged_args[n].value = arglist[n].value;
  }
  XtSetArg(merged_args[n], XmNscrollingPolicy,
			   (XtArgVal) XmAPPLICATION_DEFINED); n++;
  XtSetArg(merged_args[n], XmNvisualPolicy, (XtArgVal) XmVARIABLE); n++;
  XtSetArg(merged_args[n], XmNscrollBarDisplayPolicy, (XtArgVal) XmSTATIC); n++;
  XtSetArg(merged_args[n], XmNshadowThickness, (XtArgVal) 0); n++;
 
  swindow = XtCreateManagedWidget(s, xmScrolledWindowWidgetClass, parent,
							 merged_args, n);
  XtFree(s);
  XtFree(merged_args);

  /* Create Text widget.  */
  stext = XtCreateWidget(name, xmTextWidgetClass, swindow, arglist, argcount);

  /* Add callback to destroy ScrolledWindow parent. */
  XtAddCallback (stext, XmNdestroyCallback, _XmDestroyParentCallback, NULL);

  /* Return Text.*/
  return (stext);
}

#ifdef _NO_PROTO
Widget XmCreateText(parent, name, arglist, argcount)
Widget parent;
char *name;
ArgList arglist;
Cardinal argcount;
#else /* _NO_PROTO */
Widget XmCreateText (Widget parent, char *name, ArgList arglist, Cardinal argcount)
#endif /* _NO_PROTO */
{
    return XtCreateWidget(name, xmTextWidgetClass, parent, arglist, argcount);
}


#ifdef _NO_PROTO
char *XmTextGetString(widget)
Widget widget;
#else /* _NO_PROTO */
char *XmTextGetString(Widget widget)
#endif /* _NO_PROTO */
{
    char *text_copy;

    if (XmIsTextField(widget))
       text_copy = XmTextFieldGetString(widget);
    else
       text_copy = _XmStringSourceGetValue(GetSrc(widget));

    return (text_copy);
}


#ifdef _NO_PROTO
XmTextPosition XmTextGetLastPosition(widget)
Widget widget;
#else /* _NO_PROTO */
XmTextPosition XmTextGetLastPosition (Widget widget)
#endif /* _NO_PROTO */
{
    if (XmIsTextField(widget))
       return(XmTextFieldGetLastPosition(widget));
    else {
       XmTextSource source = GetSrc(widget);
       return (*source->Scan)(source, 0, XmSELECT_ALL, XmsdRight, 1, TRUE);
    }
}


#ifdef _NO_PROTO
void XmTextSetString(widget, value)
Widget   widget;
char     *value;
#else /* _NO_PROTO */
void XmTextSetString (Widget widget, char *value)
#endif /* _NO_PROTO */
{
    if (XmIsTextField(widget))
       XmTextFieldSetString(widget, value);
    else {
       XmTextWidget tw = (XmTextWidget) widget;
       XmTextPosition cursorPos;
       XmAnyCallbackStruct cb;

       tw->text.pendingoff = TRUE;
       if (value == NULL) value = "";
       _XmStringSourceSetValue(GetSrc(tw), value);

       cursorPos = (*tw->text.source->Scan)(tw->text.source,
                                           XmTextGetCursorPosition(widget),
                                           XmSELECT_ALL, XmsdLeft, 1, TRUE);
       XmTextSetCursorPosition(widget, cursorPos);
       tw->text.needs_refigure_lines = True;
       if (tw->text.disable_depth == 0)
	       Redisplay(tw);

       /* Call ValueChanged Callback to indicate that text has been modified */
       cb.reason = XmCR_VALUE_CHANGED;
       cb.event = NULL;
       XtCallCallbackList ((Widget) tw, tw->text.value_changed_callback,
			  (Opaque) &cb);
    }
}



#ifdef _NO_PROTO
void XmTextReplace(widget, frompos, topos, value)
Widget widget;
XmTextPosition frompos, topos;
char   *value;
#else /* _NO_PROTO */
void XmTextReplace (Widget widget, XmTextPosition frompos, XmTextPosition topos, char *value)
#endif /* _NO_PROTO */
{
    if (XmIsTextField(widget))
       XmTextFieldReplace(widget, frompos, topos, value);
    else {
       XmTextWidget tw = (XmTextWidget) widget;
       XmAnyCallbackStruct cb;
       XmTextSource source;
       XmTextBlockRec block;
       Boolean editable;
       int max_length;

       source = GetSrc(tw);
       block.format = FMT8BIT;
       if (value == NULL)
          block.length = 0;
       else
          block.length = strlen(value);
       block.ptr = value;
       editable = _XmStringSourceGetEditable(source);
       max_length = _XmStringSourceGetMaxLength(source);

       _XmStringSourceSetEditable(source, TRUE);
       _XmStringSourceSetMaxLength(source, MAXINT);
       (*source->Replace)(source, NULL, frompos, topos, &block);
       _XmStringSourceSetEditable(source, editable);
       _XmStringSourceSetMaxLength(source, max_length);

       /* Call ValueChanged Callback to indicate that text has been modified */
       cb.reason = XmCR_VALUE_CHANGED;
       cb.event = NULL;
       XtCallCallbackList ((Widget) tw, tw->text.value_changed_callback,
		          (Opaque) &cb);
   }
}

#ifdef _NO_PROTO
void XmTextInsert(widget, position, value)
Widget widget;
XmTextPosition position;
char   *value;
#else /* _NO_PROTO */
void XmTextInsert (Widget widget, XmTextPosition position, char *value)
#endif /* _NO_PROTO */
{
    if (XmIsTextField(widget))
       XmTextFieldInsert(widget, position, value);
    else {
       XmTextReplace(widget, position, position, value);
    }
}

#ifdef _NO_PROTO
void XmTextSetAddMode (widget, state)
Widget widget;
Boolean state;
#else /* _NO_PROTO */
void XmTextSetAddMode (Widget widget,    
#if NeedWidePrototypes
int state
#else
Boolean state
#endif 
)
#endif /* _NO_PROTO */
{
    if (XmIsTextField(widget))
       XmTextFieldSetAddMode(widget, state);
    else {
       XmTextWidget tw = (XmTextWidget) widget;

       (*tw->text.output->DrawInsertionPoint)(tw,
					      tw->text.cursor_position, off);
       tw->text.add_mode = state;

       (*tw->text.output->DrawInsertionPoint)(tw, tw->text.cursor_position, on);
    }
}


#ifdef _NO_PROTO
Boolean XmTextGetAddMode (widget)
Widget widget;
#else /* _NO_PROTO */
Boolean XmTextGetAddMode (Widget widget)
#endif /* _NO_PROTO */
{
    if (XmIsTextField(widget))
       return(XmTextFieldGetAddMode(widget));
    else {
       XmTextWidget tw = (XmTextWidget) widget;

       return (tw->text.add_mode);
    }
}


#ifdef _NO_PROTO
Boolean XmTextGetEditable(widget)
Widget widget;
#else /* _NO_PROTO */
Boolean XmTextGetEditable (Widget widget)
#endif /* _NO_PROTO */
{
    if (XmIsTextField(widget))
       return(XmTextFieldGetEditable(widget));
    else
       return _XmStringSourceGetEditable(GetSrc(widget));
}


#ifdef _NO_PROTO
void XmTextSetEditable(widget, editable)
Widget widget;
Boolean editable;
#else /* _NO_PROTO */
void XmTextSetEditable (Widget widget,     
#if NeedWidePrototypes
int editable
#else
Boolean editable
#endif 
)
#endif /* _NO_PROTO */
{
    if (XmIsTextField(widget))
       XmTextFieldSetEditable(widget, editable);
    else {
       XmTextWidget tw = (XmTextWidget) widget;
       tw->text.editable = editable;
       _XmStringSourceSetEditable(GetSrc(tw), editable);
    }
}


#ifdef _NO_PROTO
int XmTextGetMaxLength(widget)
Widget widget;
#else /* _NO_PROTO */
int XmTextGetMaxLength (Widget widget)
#endif /* _NO_PROTO */
{
    if (XmIsTextField(widget))
       return(XmTextFieldGetMaxLength(widget));
    else
       return _XmStringSourceGetMaxLength(GetSrc(widget));
}


#ifdef _NO_PROTO
void XmTextSetMaxLength(widget, max_length)
Widget widget;
int max_length;
#else /* _NO_PROTO */
void XmTextSetMaxLength (Widget widget, int max_length)
#endif /* _NO_PROTO */
{
    if (XmIsTextField(widget))
       XmTextFieldSetMaxLength(widget, max_length);
    else {
       XmTextWidget tw = (XmTextWidget) widget;

       tw->text.max_length = max_length;
       _XmStringSourceSetMaxLength(GetSrc(tw), max_length);
    }
}



#ifdef _NO_PROTO
XmTextPosition XmTextGetTopCharacter(widget)
Widget widget;
#else /* _NO_PROTO */
XmTextPosition XmTextGetTopCharacter (Widget widget)
#endif /* _NO_PROTO */
{
    XmTextWidget tw = (XmTextWidget) widget;

    if (tw->text.needs_refigure_lines)
	RefigureLines(tw);
    return tw->text.top_character;
}
    

#ifdef _NO_PROTO
void XmTextSetTopCharacter(widget, top_character)
Widget widget;
XmTextPosition top_character;
#else /* _NO_PROTO */
void XmTextSetTopCharacter (Widget widget, XmTextPosition top_character)
#endif /* _NO_PROTO */
{
    XmTextWidget tw = (XmTextWidget) widget;

    if (tw->text.edit_mode != XmSINGLE_LINE_EDIT)
       top_character = (*tw->text.source->Scan)(tw->text.source, top_character,
					   XmSELECT_LINE, XmsdLeft, 1, FALSE);

    if (top_character != tw->text.new_top) {
	tw->text.new_top = top_character;
	tw->text.pending_scroll = 0;
	tw->text.needs_refigure_lines = tw->text.needs_redisplay = TRUE;
	if (tw->text.disable_depth == 0)
	    Redisplay(tw);
    }
}

#ifdef _NO_PROTO
XmTextPosition XmTextGetInsertionPosition(widget)
Widget widget;
#else /* _NO_PROTO */
XmTextPosition XmTextGetInsertionPosition (Widget widget)
#endif /* _NO_PROTO */
{
    if (XmIsTextField(widget))
       return(XmTextFieldGetCursorPosition(widget));
    else
       return(XmTextGetCursorPosition(widget));
}

#ifdef _NO_PROTO
void XmTextSetInsertionPosition(widget, position)
Widget widget;
XmTextPosition position;
#else /* _NO_PROTO */
void XmTextSetInsertionPosition (Widget widget, XmTextPosition position)
#endif /* _NO_PROTO */
{
    if (XmIsTextField(widget))
       XmTextFieldSetCursorPosition(widget, position);
    else
       XmTextSetCursorPosition(widget, position);
}

#ifdef _NO_PROTO
XmTextPosition XmTextGetCursorPosition(widget)
Widget widget;
#else /* _NO_PROTO */
XmTextPosition XmTextGetCursorPosition (Widget widget)
#endif /* _NO_PROTO */
{
    if (XmIsTextField(widget))
       return(XmTextFieldGetCursorPosition(widget));
    else {
       XmTextWidget tw = (XmTextWidget) widget;

       return tw->text.cursor_position;
    }
}


#ifdef _NO_PROTO
void _XmTextSetCursorPosition(widget, position)
Widget widget;
XmTextPosition position;
#else /* _NO_PROTO */
void _XmTextSetCursorPosition (Widget widget, XmTextPosition position)
#endif /* _NO_PROTO */
{
    if (XmIsTextField(widget))
       XmTextFieldSetCursorPosition(widget, position);
    else {
       XmTextWidget tw = (XmTextWidget) widget;
       XmTextSource source;
       XmTextVerifyCallbackStruct cb;

       if (position < 0 || position > tw->text.last_position) return;

       source = GetSrc(tw);

      /* if position hasn't changed, don't call the modify verify callback */
       if (position != tw->text.cursor_position) {
         /* Call Motion Verify Callback before Cursor Changes Positon */
          cb.reason = XmCR_MOVING_INSERT_CURSOR;
          cb.event  = NULL;
          cb.currInsert = tw->text.cursor_position;
          cb.newInsert = position;
          cb.doit = True;
          XtCallCallbackList ((Widget) tw, tw->text.motion_verify_callback,
			      (Opaque) &cb);
   
         /* Cancel action upon application request */
          if (!cb.doit) {
             if (tw->text.verify_bell) XBell(XtDisplay(widget), 0);
             return;
          }
       }

      /* Erase insert cursor prior to move */
       EraseInsertionPoint(tw);
       tw->text.cursor_position = position;

      /*
       * If not in add_mode and pending delete state is on reset
       * the selection.
       */
       if (!tw->text.add_mode && tw->text.pendingoff &&
           _XmStringSourceHasSelection(source))
          (*source->SetSelection)(source, position, position, CurrentTime);

       if (tw->text.input->data->has_destination) {
          if (tw->text.dest_position == tw->text.cursor_position)
             _XmTextClearDestination(tw, False);
          else
             _XmTextDrawDestination(tw);
       }

       tw->text.needs_redisplay = TRUE;
       if (tw->text.auto_show_cursor_position)
   	   XmTextShowPosition((Widget)tw, position);
       if (tw->text.needs_redisplay && tw->text.disable_depth == 0)
      	   Redisplay(tw);
    }
}


#ifdef _NO_PROTO
void XmTextSetCursorPosition(widget, position)
Widget widget;
XmTextPosition position;
#else /* _NO_PROTO */
void XmTextSetCursorPosition (Widget widget, XmTextPosition position)
#endif /* _NO_PROTO */
{
    XmTextWidget tw = (XmTextWidget) widget;

    _XmTextSetCursorPosition(widget, position);

    _XmTextSetDestinationSelection(widget, position, False, CurrentTime);

    if (tw->text.input->data->has_destination &&
        (tw->text.dest_position != tw->text.cursor_position ||
         !tw->text.output->data->hasfocus))
       _XmTextDrawDestination(tw);
}



#ifdef _NO_PROTO
Boolean XmTextRemove(widget)
Widget widget;
#else /* _NO_PROTO */
Boolean XmTextRemove (Widget widget)
#endif /* _NO_PROTO */
{
   if (XmIsTextField(widget))
      return(XmTextFieldRemove(widget));
   else {
      XmTextWidget tw = (XmTextWidget) widget;
      XmTextSource source = tw->text.source;
      XmTextPosition left, right;

      if (!(*source->GetSelection)(source, &left, &right) ||
         left == right) {
         tw->text.input->data->anchor = tw->text.cursor_position;
         return False;
      }

      XmTextReplace(widget, left, right, NULL);

      (*source->SetSelection)(source, tw->text.cursor_position,
				      tw->text.cursor_position, CurrentTime);

      tw->text.input->data->anchor = tw->text.cursor_position;

      return True;
   }
}

#ifdef _NO_PROTO
Boolean XmTextCopy(widget, copy_time)
Widget widget;
Time copy_time;
#else /* _NO_PROTO */
Boolean XmTextCopy (Widget widget, Time copy_time)
#endif /* _NO_PROTO */
{
   if (XmIsTextField(widget))
      return(XmTextFieldCopy(widget, copy_time));
   else {
      char *selected_string = XmTextGetSelection (widget); /* text selection */
      long item_id = 0;                         /* clipboard item id */
      int data_id = 0;                                   /* clipboard data id */
      int status = 0;                                    /* clipboard status  */
      XmString clip_label;

      /*
       * Using the Xm clipboard facilities,
       * copy the selected text to the clipboard
       */
      if (selected_string != NULL) {
         clip_label = XmStringCreateLtoR ("XM_TEXT", XmSTRING_DEFAULT_CHARSET);
        /* start copy to clipboard */
         status = XmClipboardStartCopy (XtDisplay(widget), XtWindow(widget),
                                        clip_label, copy_time, widget,
				        NULL, &item_id);

         if (status != ClipboardSuccess) return False;
   
        /* move the data to the clipboard */
         status = XmClipboardCopy (XtDisplay(widget), XtWindow(widget),
                                   item_id, "STRING", selected_string,
                                   (unsigned long)strlen(selected_string) + 1,
				   0, &data_id);

         if (status != ClipboardSuccess) return False;

        /* end the copy to the clipboard */
         status = XmClipboardEndCopy (XtDisplay(widget), XtWindow(widget),
                                      item_id);

         if (status != ClipboardSuccess) return False;
      } else
         return False;
      return True;
   }
}

#ifdef _NO_PROTO
Boolean XmTextCut(widget, cut_time)
Widget widget;
Time cut_time;
#else /* _NO_PROTO */
Boolean XmTextCut (Widget widget, Time cut_time)
#endif /* _NO_PROTO */
{
  if (XmIsTextField(widget))
     return(XmTextFieldCut(widget, cut_time));
  else {
     XmTextWidget tw = (XmTextWidget) widget;
     Boolean changed_dest_visible = False;

     if ( tw->text.editable == False )           /* can't cut if you can't edit */    /* PIR1870 */
        return False;                            /* assume this means not possible */ /* PIR1870 */
                                                 /* (NO ASSUMPTIONS - ie, NO COPY) */ /* PIR1870 */

     if (tw->text.output->data->dest_visible) {
        _XmTextDestinationVisible(widget, False);
        changed_dest_visible = True;
     }

     if (XmTextCopy(widget, cut_time))
        if (XmTextRemove(widget)) {
           if (tw->text.input->data->has_destination)
              _XmTextSetDestinationSelection(widget, tw->text.cursor_position,
					     False, cut_time);
           else
              XmTextSetAddMode(widget, False);
           _XmTextDestinationVisible(widget, True);
           changed_dest_visible = True;
           return True;
        }

     if (changed_dest_visible)
        _XmTextDestinationVisible(widget, True);

     return False;
  }
}



/*
 * Retrieves the current data from the clipboard
 * and paste it at the current cursor position
 */

#ifdef _NO_PROTO
Boolean XmTextPaste(widget)
Widget widget;
#else /* _NO_PROTO */
Boolean XmTextPaste (Widget widget)
#endif /* _NO_PROTO */
{
   if (XmIsTextField(widget))
      return(XmTextFieldPaste(widget));
   else {
      XmTextWidget tw = (XmTextWidget) widget;
      XmTextSource source = tw->text.source;
      XmTextPosition sel_left = 0;
      XmTextPosition sel_right = 0;
      XmTextPosition paste_pos_left, paste_pos_right;
      XmAnyCallbackStruct cb;
      int status = 0;                            /* clipboard status        */
      char *buffer;                              /* temporary text buffer   */
      unsigned long length;                                /* length of buffer        */
      unsigned long outlength = 0;                         /* length of bytes copied  */
      int private_id = 0;                        /* id of item on clipboard */
      Boolean dest_disjoint = True;
      XmTextBlockRec block;
      Boolean changed_dest_visible = False;

      if (tw->text.output->data->dest_visible) {
         _XmTextDestinationVisible(widget, False);
         changed_dest_visible = True;
      }

      if (tw->text.input->data->has_destination)
         paste_pos_left = paste_pos_right= tw->text.dest_position;
      else
         paste_pos_left = paste_pos_right= tw->text.cursor_position;

      status = XmClipboardInquireLength(XtDisplay(widget), XtWindow(widget),
                                        "STRING", &length);

      if (status == ClipboardNoData || length == 0) return False;

      /* malloc length of clipboard data */
      buffer = XtMalloc(length);

      status = XmClipboardRetrieve (XtDisplay(widget), XtWindow(widget),
				   "STRING", buffer, length,
				   &outlength, &private_id);

      if (status != ClipboardSuccess) return False;

      if (XmTextGetSelectionPosition(widget, &sel_left, &sel_right)) {
         if (tw->text.input->data->pendingdelete &&
             paste_pos_left >= sel_left && paste_pos_right <= sel_right) {
            paste_pos_left = sel_left;
            paste_pos_right = sel_right;
            dest_disjoint = False;
         }
      }

      block.ptr = buffer;
      block.length = outlength - 1;
      block.format = FMT8BIT;

      /* add new text */
      if ((*source->Replace)(source, NULL, paste_pos_left,
                             paste_pos_right, &block) != EditDone) {
        if (tw->text.verify_bell) XBell(XtDisplay(tw), 0);
      } else {
       /*
        * Call ValueChanged Callback to indicate that
        * text has been modified.
        */
        cb.reason = XmCR_VALUE_CHANGED;
        cb.event = NULL;
        XtCallCallbackList ((Widget) tw, tw->text.value_changed_callback,
                           (Opaque) &cb);

        _XmTextSetDestinationSelection((Widget)tw, tw->text.cursor_position,
                                       False, CurrentTime);

        if (sel_left != sel_right) {
           if (!dest_disjoint) {
              tw->text.input->data->anchor = tw->text.dest_position;
              (*source->SetSelection)(source, tw->text.dest_position,
				      tw->text.dest_position, CurrentTime);
           } else {
              if (!tw->text.add_mode) {
                 tw->text.input->data->anchor = tw->text.dest_position;
                 (*source->SetSelection)(source, tw->text.dest_position,
					 tw->text.dest_position, CurrentTime);
              }
           }
        } else
           tw->text.input->data->anchor = tw->text.dest_position;
     }
     if (changed_dest_visible)
        _XmTextDestinationVisible(widget, True);
   }

   return True;
}


#ifdef _NO_PROTO
char *XmTextGetSelection(widget)
Widget widget;
#else /* _NO_PROTO */
char *XmTextGetSelection(Widget widget)
#endif /* _NO_PROTO */
{
   if (XmIsTextField(widget))
      return(XmTextFieldGetSelection(widget));
   else {
      XmTextSource source;
      XmTextPosition left, right;

      source = GetSrc(widget);
      if (!(*source->GetSelection)(source, &left, &right))
	  return NULL;

      return(_XmStringSourceGetString((XmTextWidget)widget, left, right));
   }
}


#ifdef _NO_PROTO
void XmTextSetSelection(widget, first, last, set_time)
Widget widget;
XmTextPosition first, last;
Time set_time;
#else /* _NO_PROTO */
void XmTextSetSelection (Widget widget, XmTextPosition first, XmTextPosition last, Time set_time)
#endif /* _NO_PROTO */
{
   if (XmIsTextField(widget))
      XmTextFieldSetSelection(widget, first, last, set_time);
   else {
      XmTextSource source;
      XmTextWidget tw = (XmTextWidget) widget;

      if (first < 0 || last > tw->text.last_position) return;

      source = GetSrc(widget);
      (*source->SetSelection)(source, first, last, set_time);
      if (tw->text.input->data->pendingdelete) {
         tw->text.pendingoff = FALSE;
      }
      XmTextSetCursorPosition(widget, last);
   }
}


#ifdef _NO_PROTO
void XmTextClearSelection(widget, clear_time)
Widget widget;
Time clear_time;
#else /* _NO_PROTO */
void XmTextClearSelection (Widget widget, Time clear_time)
#endif /* _NO_PROTO */
{
   if (XmIsTextField(widget))
      XmTextFieldClearSelection(widget, clear_time);
   else {
      XmTextWidget tw = (XmTextWidget) widget;
      XmTextSource source = GetSrc(widget);

      (*source->SetSelection)(tw->text.source, 1, 0, clear_time);
      if (tw->text.input->data->pendingdelete) {
         tw->text.pendingoff = FALSE;
      }
   }
}



#ifdef _NO_PROTO
Boolean XmTextGetSelectionPosition(widget, left, right)
Widget widget;
XmTextPosition *left, *right;
#else /* _NO_PROTO */
Boolean XmTextGetSelectionPosition (Widget widget, XmTextPosition *left, XmTextPosition *right)
#endif /* _NO_PROTO */
{
   if (XmIsTextField(widget))
      return(XmTextFieldGetSelectionPosition(widget, left, right));
   else {
      XmTextWidget tw = (XmTextWidget) widget;

      return (*tw->text.source->GetSelection)(tw->text.source, left, right);
   }
}

#ifdef _NO_PROTO
XmTextPosition XmTextXYToPos(widget, x, y)
Widget widget;
Position x, y;
#else /* _NO_PROTO */
XmTextPosition XmTextXYToPos (Widget widget,      
#if NeedWidePrototypes
int x, int y
#else
Position x, Position y
#endif 
)
#endif /* _NO_PROTO */
{
   if (XmIsTextField(widget))
      return(XmTextFieldXYToPos(widget, x, y));
   else {
      XmTextWidget tw = (XmTextWidget) widget;

      return (*tw->text.output->XYToPos)(tw, x, y);
   }
}


#ifdef _NO_PROTO
Boolean XmTextPosToXY(widget, position, x, y)
Widget widget;
XmTextPosition position;
Position *x, *y;
#else /* _NO_PROTO */
Boolean XmTextPosToXY (Widget widget, XmTextPosition position, Position *x, Position *y)
#endif /* _NO_PROTO */
{
   if (XmIsTextField(widget))
      return(XmTextFieldPosToXY(widget, position, x, y));
   else {
      XmTextWidget tw = (XmTextWidget) widget;

      return (*tw->text.output->PosToXY)(tw, position, x, y);
   }
}

#ifdef _NO_PROTO
XmTextSource XmTextGetSource(widget)
Widget widget;
#else /* _NO_PROTO */
XmTextSource XmTextGetSource (Widget widget)
#endif /* _NO_PROTO */
{
    XmTextWidget tw = (XmTextWidget) widget;

    return tw->text.source;
}


#ifdef _NO_PROTO
void XmTextSetSource(widget, source, top_character, cursor_position)
Widget widget;
XmTextSource source;
XmTextPosition top_character;
XmTextPosition cursor_position;
#else /* _NO_PROTO */
void XmTextSetSource (Widget widget, XmTextSource source, XmTextPosition top_character, XmTextPosition cursor_position)
#endif /* _NO_PROTO */
{
    XmTextWidget tw = (XmTextWidget) widget;
    Boolean changed_dest_visible = False;

    EraseInsertionPoint(tw);
    if (source == NULL) {
       _XmWarning(tw, MESSAGE2);
       return;
    }

    if (tw->text.output->data->dest_visible) {
       _XmTextDestinationVisible(widget, False);
       changed_dest_visible = True;
    }

    (*tw->text.source->RemoveWidget)(tw->text.source, tw);
    tw->text.source = source;
    tw->text.cursor_position = cursor_position;
    top_character = (*tw->text.source->Scan)(tw->text.source, top_character,
					   XmSELECT_LINE, XmsdLeft, 1, FALSE);
    tw->text.new_top = top_character;
    tw->text.top_character = 0;
    _XmTextInvalidate(tw, top_character, top_character, NODELTA);
    XmTextSetHighlight(widget, 0, MAXINT, XmHIGHLIGHT_NORMAL);
    (*tw->text.source->AddWidget)(tw->text.source, tw);
    _XmStringSourceSetGappedBuffer(source->data, cursor_position);
    if (tw->text.disable_depth == 0)
	Redisplay(tw);

    if (changed_dest_visible)
       _XmTextDestinationVisible(widget, True);
}


/*
 * Force the given position to be displayed.  If position < 0, then don't force
 * any position to be displayed.
 */

/* ARGSUSED */
#ifdef _NO_PROTO
void XmTextShowPosition(widget, position)
Widget widget;
XmTextPosition position;
#else /* _NO_PROTO */
void XmTextShowPosition (Widget widget, XmTextPosition position)
#endif /* _NO_PROTO */
{
   if (XmIsTextField(widget))
      XmTextFieldShowPosition(widget, position);
   else {
      XmTextWidget tw = (XmTextWidget) widget;
    
      if (!tw->text.needs_refigure_lines && (position < 0 ||
		  (position >= tw->text.top_character &&
			   position < tw->text.bottom_position))) {
          if (tw->text.input->data->has_destination &&
              tw->text.dest_position != tw->text.cursor_position)
             _XmTextClearDestination(tw, False);
	  (*tw->text.output->MakePositionVisible)((Widget)tw, position);
          if (tw->text.input->data->has_destination &&
              tw->text.dest_position != tw->text.cursor_position)
             _XmTextDrawDestination(tw);
	  return;
      }
      tw->text.force_display = position;
      tw->text.needs_refigure_lines = tw->text.needs_redisplay = TRUE;
      if (tw->text.disable_depth == 0)
	  Redisplay(tw);
   }
}


#ifdef _NO_PROTO
void XmTextScroll(widget, n)
Widget widget;
int n;			/* Positive moves text upward; negative, downward. */
#else /* _NO_PROTO */
void XmTextScroll (Widget widget, int n)
#endif /* _NO_PROTO */
{
    XmTextWidget tw = (XmTextWidget) widget;

    tw->text.pending_scroll += n;
    tw->text.needs_refigure_lines = tw->text.needs_redisplay = TRUE;

    if (tw->text.disable_depth == 0)
	Redisplay(tw);
}

#ifdef _NO_PROTO
int XmTextGetBaseLine(widget)
Widget widget;
#else /* _NO_PROTO */
int XmTextGetBaseLine (Widget widget)
#endif /* _NO_PROTO */
{
   if (XmIsTextField(widget))
      return(XmTextFieldGetBaseLine(widget));
   else
      return _XmTextGetBaseLine((XmTextWidget)widget);
   
}

#ifdef _NO_PROTO
int XmTextGetBaseline(widget)
Widget widget;
#else /* _NO_PROTO */
int XmTextGetBaseline (Widget widget)
#endif /* _NO_PROTO */
{
   return XmTextGetBaseLine(widget);
}

#ifdef _NO_PROTO
void _XmTextDisableRedisplay(widget, losesbackingstore)
XmTextWidget widget;
Boolean losesbackingstore;
#else /* _NO_PROTO */
void _XmTextDisableRedisplay (XmTextWidget widget, Boolean losesbackingstore)
#endif /* _NO_PROTO */
{
    widget->text.disable_depth++;
    if (losesbackingstore) {
	EraseInsertionPoint(widget);
    }
}

#ifdef _NO_PROTO
void _XmTextEnableRedisplay(widget)
XmTextWidget widget;
#else /* _NO_PROTO */
void _XmTextEnableRedisplay (XmTextWidget widget)
#endif /* _NO_PROTO */
{
    if (widget->text.disable_depth) widget->text.disable_depth--;
    if (widget->text.disable_depth == 0 && widget->text.needs_redisplay)
	Redisplay(widget);
}
