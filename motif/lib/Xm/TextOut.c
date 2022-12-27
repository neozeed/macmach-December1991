#ifdef REV_INFO
#ifndef lint
static char SCCSID[] = "OSF/Motif: @(#)TextOut.c	3.36.1.3 91/03/19";
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
*	THIS SOFTWARE IS FURNISHED UNDER A LICENSE AND MAY BE USED
*  AND COPIED ONLY IN ACCORDANCE WITH THE TERMS OF SUCH LICENSE AND
*  WITH THE INCLUSION OF THE ABOVE COPYRIGHT NOTICE.  THIS SOFTWARE OR
*  ANY OTHER COPIES THEREOF MAY NOT BE PROVIDED OR OTHERWISE MADE
*  AVAILABLE TO ANY OTHER PERSON.  NO TITLE TO AND OWNERSHIP OF THE
*  SOFTWARE IS HEREBY TRANSFERRED.
*
*	THE INFORMATION IN THIS SOFTWARE IS SUBJECT TO CHANGE WITHOUT
*  NOTICE AND SHOULD NOT BE CONSTRUED AS A COMMITMENT BY OPEN SOFTWARE
*  FOUNDATION, INC. OR ITS THIRD PARTY SUPPLIERS
*
*	OPEN SOFTWARE FOUNDATION, INC. AND ITS THIRD PARTY SUPPLIERS,
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
/* Private definitions. */

#include <X11/Xatom.h>
#include <Xm/TextP.h>
#include <X11/Shell.h>
#include <X11/Vendor.h>

#include <Xm/ScrolledWP.h>
#include <Xm/ScrollBar.h>


/*
 * Defines for bit map cursors
 */
#define CARET_WIDTH 9
#define CARET_HEIGHT 5

static unsigned char caretBits[] = {
   0x10, 0x00, 0x38, 0x00, 0x6c, 0x00, 0xc6, 0x00, 0x83, 0x01};

static unsigned char fillCaretBits[] = {
   0x10, 0x00, 0x38, 0x00, 0x7c, 0x00, 0xfe, 0x00, 0xff, 0x01};

typedef struct {
   XmTextWidget tw;
} TextGCDataRec, *TextGCData;

static XContext _XmTextGCContext = NULL;

static void NotifyResized();
static XtTimerCallbackProc HandleTimer();
static void RedrawRegion();
static void ChangeHOffset();
static void DrawInsertionPoint();

static XtResource output_resources[] =
{
    {
      XmNfontList, XmCFontList, XmRFontList, sizeof(XmFontList),
      XtOffset(OutputData, fontlist),
      XmRImmediate, NULL
    },

    {
      XmNwordWrap, XmCWordWrap, XmRBoolean, sizeof(Boolean),
      XtOffset(OutputData, wordwrap),
      XmRImmediate, (caddr_t) False
    },

    {
      XmNblinkRate, XmCBlinkRate, XmRInt, sizeof(int),
      XtOffset(OutputData, blinkrate),
      XmRImmediate, (caddr_t) 500
    },

    {
      XmNcolumns, XmCColumns, XmRShort, sizeof(short),
      XtOffset(OutputData, columns),
      XmRImmediate, (caddr_t) 20
    },

    {
      XmNrows, XmCRows, XmRShort, sizeof(short),
      XtOffset(OutputData, rows),
      XmRImmediate, (caddr_t) 1
    },

    {
      XmNresizeWidth, XmCResizeWidth, XmRBoolean, sizeof(Boolean),
      XtOffset(OutputData, resizewidth),
      XmRImmediate, (caddr_t) False
    },

    {
      XmNresizeHeight, XmCResizeHeight, XmRBoolean, sizeof(Boolean),
      XtOffset(OutputData, resizeheight),
      XmRImmediate, (caddr_t) False
    },

    {
      XmNscrollVertical, XmCScroll, XmRBoolean, sizeof(Boolean),
      XtOffset(OutputData, scrollvertical),
      XmRImmediate,(caddr_t) True
    },

    {
      XmNscrollHorizontal, XmCScroll, XmRBoolean, sizeof(Boolean),
      XtOffset(OutputData, scrollhorizontal),
      XmRImmediate, (caddr_t) True
    },

    {
      XmNscrollLeftSide, XmCScrollSide, XmRBoolean, sizeof(Boolean),
      XtOffset(OutputData, scrollleftside),
      XmRImmediate,(caddr_t) False
    },

    {
      XmNscrollTopSide, XmCScrollSide, XmRBoolean, sizeof(Boolean),
      XtOffset(OutputData, scrolltopside),
      XmRImmediate, (caddr_t) False
    },

    {
      XmNcursorPositionVisible, XmCCursorPositionVisible, XmRBoolean,
      sizeof(Boolean),
      XtOffset(OutputData, cursor_position_visible),
      XmRImmediate, (caddr_t) True
    },

};

static TextGCData GetTextGCData(w)
Widget w;
{
   TextGCData gc_data;

   if (_XmTextGCContext == NULL)
      _XmTextGCContext = XUniqueContext();

   if (XFindContext(XtDisplay(w), (Window) 0, _XmTextGCContext, 
		    (caddr_t*)&gc_data)) {
       gc_data = (TextGCData)XtCalloc(sizeof(TextGCDataRec), 1);

       XSaveContext(XtDisplay(w), (Window) 0, _XmTextGCContext, gc_data);
       gc_data->tw = (XmTextWidget) w;
   }

   if (gc_data->tw == NULL) gc_data->tw = (XmTextWidget) w;

   return gc_data;

}
static void _XmTextDrawShadow(tw)
XmTextWidget tw;
{
   if (XtIsRealized(tw)) {
      if (tw->primitive.shadow_thickness > 0)
         _XmDrawShadow (XtDisplay (tw), XtWindow (tw),
                        tw->primitive.bottom_shadow_GC,
                        tw->primitive.top_shadow_GC,
                        tw->primitive.shadow_thickness,
                        tw->primitive.highlight_thickness,
                        tw->primitive.highlight_thickness,
                        tw->core.width - 2 * tw->primitive.highlight_thickness,
                        tw->core.height-2 * tw->primitive.highlight_thickness);


      if (tw -> primitive.highlighted)
         _XmHighlightBorder ((Widget)tw);
      else if (_XmDifferentBackground ((Widget)tw, XtParent (tw)))
         _XmUnhighlightBorder ((Widget)tw);
   }
}

static void XmSetFullGC(tw, gc)
XmTextWidget tw;
GC gc;
{
    OutputData data = tw->text.output->data;
    XRectangle ClipRect;

    ClipRect.width = tw->core.width - (2 *(tw->primitive.highlight_thickness +
                                             tw->primitive.shadow_thickness));
    ClipRect.height = tw->core.height - (2 *(tw->primitive.highlight_thickness +
                                             tw->primitive.shadow_thickness));
    ClipRect.x = tw->primitive.highlight_thickness +
                 tw->primitive.shadow_thickness;
    ClipRect.y = tw->primitive.highlight_thickness +
                 tw->primitive.shadow_thickness;

    XSetClipRectangles(XtDisplay(tw), gc, 0, 0, 
                        &ClipRect, 1, Unsorted);

   if (!data->has_rect) {
     OutputData old_data;
     TextGCData gc_data = GetTextGCData((Widget)tw);
     old_data = gc_data->tw->text.output->data; 
     old_data->has_rect = False;
     gc_data->tw = tw;
     data->has_rect = True;
   }
}

static void GetRect(tw, rect)
XmTextWidget tw;
XRectangle *rect;
{
  Dimension margin_width = tw->text.margin_width +
                           tw->primitive.shadow_thickness +
                           tw->primitive.highlight_thickness;
  Dimension margin_height = tw->text.margin_height +
                           tw->primitive.shadow_thickness +
                           tw->primitive.highlight_thickness;

  if (margin_width < tw->core.width)
     rect->x = margin_width;
  else
     rect->x = tw->core.width;

  if (margin_height < tw->core.height)
     rect->y = margin_height;
  else
     rect->y = tw->core.height;

  if ((2 * margin_width) < tw->core.width)
     rect->width = (int) tw->core.width - (2 * margin_width);
  else
     rect->width = 0;

  if ((2 * margin_height) < tw->core.height)
     rect->height = (int) tw->core.height - (2 * margin_height);
  else
     rect->height = 0;
}


static void XmSetMarginGC (tw, gc)
XmTextWidget tw;
GC gc;
{
  OutputData data = tw->text.output->data;
  XRectangle ClipRect;

  GetRect(tw, &ClipRect);
  XSetClipRectangles(XtDisplay(tw), gc, 0, 0, &ClipRect, 1,
                     Unsorted);
 /*
  * Make sure the cached GC has the clipping rectangle
  * set to the current widget.
  */
   if (!data->has_rect) {
     OutputData old_data;
     TextGCData gc_data = GetTextGCData((Widget)tw);
     old_data = gc_data->tw->text.output->data; 
     old_data->has_rect = False;
     gc_data->tw = tw;
     data->has_rect = True;
   }
}

/*
 * Set new clipping rectangle for text widget.  This is
 * done on each focus in event since the text widgets
 * share the same GC.
 */

#ifdef _NO_PROTO
/*--------------------------------------------------------------------------+*/
void _XmTextAdjustGC(w)
/*--------------------------------------------------------------------------+*/
XmTextWidget w;
#else /* _NO_PROTO */
void _XmTextAdjustGC(XmTextWidget w)
#endif /* _NO_PROTO */
{
  OutputData data = w->text.output->data;

  XmSetMarginGC(w, data->gc);
  XmSetFullGC(w, data->imagegc);
}


static void XmSetNormGC (tw, gc)
XmTextWidget tw;
GC gc;
{
    unsigned long valueMask = (GCForeground | GCBackground | GCFunction |
                               GCGraphicsExposures);
    XGCValues values;

    values.foreground = tw->primitive.foreground;
    values.background = tw->core.background_pixel;
    values.function = GXcopy;
    values.graphics_exposures = (Bool) TRUE;

    XChangeGC(XtDisplay(tw), gc, valueMask, &values);
}

static void XmSetInvGC (tw, gc)
XmTextWidget tw;
GC gc;
{
    unsigned long valueMask = (GCForeground | GCBackground | GCFunction |
                               GCGraphicsExposures);
    XGCValues values;

    values.foreground = tw->core.background_pixel;
    values.background = tw->primitive.foreground;
    values.function = GXcopy;
    values.graphics_exposures = (Bool) TRUE;

    XChangeGC(XtDisplay(tw), gc, valueMask, &values);
}

static void XmSetXorGC (tw, gc)
XmTextWidget tw;
GC gc;
{
    unsigned long valueMask = (GCForeground | GCBackground | GCFunction |
                               GCGraphicsExposures);
    XGCValues values;

    values.foreground = tw->primitive.foreground ^ tw->core.background_pixel;
    values.background = 0;
    values.function = GXxor;
    values.graphics_exposures = (Bool) FALSE;

    XChangeGC(XtDisplay(tw), gc, valueMask, &values);
}


static int FindWidth(data, x, block, from, to)
OutputData data;
int x;                          /* Starting position (needed for tabs) */
XmTextBlock block;
int from;                       /* How many positions in to start measuring */
int to;                         /* How many positions in to stop measuring */
{
    XFontStruct *font = data->font;
    char *ptr;
    unsigned char c;
    int i, result;

    if (block->format == FMT16BIT) {
        int direction, ascent, descent;
        XCharStruct overall;
        (void) XTextExtents16(font, (XChar2b *)(block->ptr + from * 2),
                              to - from, &direction, &ascent, &descent,
                              &overall);
        return overall.width;
    }
    result = 0;

    for (i=from, ptr = block->ptr + from; i<to ; i++, ptr++) {
        c = (unsigned char) *ptr;
        if (c == '\t')
            result += (data->tabwidth -
                       ((x + result - data->leftmargin) % data->tabwidth));
        /* %%% Do something for non-printing? */
        else {
            if (font->per_char &&
                (c >= font->min_char_or_byte2 && c <= font->max_char_or_byte2))
                result += font->per_char[c - font->min_char_or_byte2].width;
            else
        result += font->min_bounds.width;
        }
    }
    return result;
}



/* Semi-public routines. */

static XmTextPosition XYToPos(widget, x, y)
XmTextWidget widget;
Position x, y;
{
    OutputData data = widget->text.output->data;
    LineTableExtra extra;
    int i, width, lastwidth, length;
    LineNum line;
    XmTextPosition start, end, laststart;
    XmTextBlockRec block;
    XmTextPosition position;
    int delta = 0;

    x += data->hoffset;
    y -= data->topmargin;
   /* take care of negative y case */
    if (y < 0) {
       delta = ((int)(y + 1)/ (int) data->lineheight) - 1;
       y = 0;
    }
    line = y / data->lineheight;
    if (line > _XmTextNumLines(widget)) line = _XmTextNumLines(widget);
    _XmTextLineInfo(widget, line, &start, &extra);
    if (start == PASTENDPOS)
        return (*widget->text.source->Scan)(widget->text.source, 0, XmSELECT_ALL,
                                      XmsdRight, 1, FALSE);
    _XmTextLineInfo(widget, line+1, &end, &extra);
    end = (*widget->text.source->Scan)(widget->text.source, end, XmSELECT_POSITION,
                               XmsdLeft, 1, TRUE);
    width = lastwidth = data->leftmargin;
    if (start >= end) return start;
    do {
        laststart = start;
        start = (*widget->text.source->ReadSource)(widget->text.source, start,
						   end, &block);
        length = block.format == FMT16BIT ? block.length / 2 : block.length;
        for (i=0 ; i<length && width < x; i++) {
            lastwidth = width;
            width += FindWidth(data, width, &block, i, i+1);
        }
    } while (width < x && start < end);
    if (abs(lastwidth - x) < abs(width - x)) i--;
   /* if original y was negative, we need to find new laststart */
    if (delta && laststart > 0)
       laststart = _XmTextFindScroll(widget, laststart, delta);
    return (*widget->text.source->Scan)(widget->text.source, laststart,
				        XmSELECT_POSITION, (i < 0) ?
					XmsdLeft : XmsdRight, abs(i), TRUE);
}


#ifdef _NO_PROTO
Boolean _XmTextShouldWordWrap(widget)
XmTextWidget widget;
#else /* _NO_PROTO */
Boolean _XmTextShouldWordWrap (XmTextWidget widget)
#endif /* _NO_PROTO */
{
    OutputData data = widget->text.output->data;
    return (ShouldWordWrap(data, widget));
}


#ifdef _NO_PROTO
Boolean _XmTextScrollable(widget)
XmTextWidget widget;
#else /* _NO_PROTO */
Boolean _XmTextScrollable (XmTextWidget widget)
#endif /* _NO_PROTO */
{
    OutputData data = widget->text.output->data;
    return (data->scrollvertical && 
            widget->text.edit_mode != XmSINGLE_LINE_EDIT &&
            XtClass(widget->core.parent) == xmScrolledWindowWidgetClass);
}

static XmTextWidget posToXYCachedWidget = NULL;
static XmTextPosition posToXYCachedPosition;
static Position posToXYCachedX;
static Position posToXYCachedY;

static Boolean PosToXY(widget, position, x, y)
XmTextWidget widget;
XmTextPosition position;
Position *x, *y;
{
    OutputData data = widget->text.output->data;
    LineNum line;
    XmTextPosition linestart;
    LineTableExtra extra;
    XmTextBlockRec block;
    XFontStruct *font = data->font;

    if (widget == posToXYCachedWidget && position == posToXYCachedPosition) {
        *x = posToXYCachedX;
        *y = posToXYCachedY;
        return TRUE;
    }
    line = _XmTextPosToLine(widget, position);
    if (line == NOLINE || line >= data->number_lines) return FALSE;
    *y = data->topmargin + line * data->lineheight +
        font->ascent;
    *x = data->leftmargin;
    _XmTextLineInfo(widget, line, &linestart, &extra);
    while (linestart < position) {
        linestart = (*widget->text.source->ReadSource)(widget->text.source,
						 linestart, position, &block);
        *x += FindWidth(data, *x, &block, 0, block.length);
    }
    *x -= data->hoffset;
    posToXYCachedWidget = widget;
    posToXYCachedPosition = position;
    posToXYCachedX = *x;
    posToXYCachedY = *y;
    return TRUE;
}


static XmTextPosition FindLineEnd(widget, position, extra)
XmTextWidget widget;
XmTextPosition position;
LineTableExtra *extra;
{
    OutputData data = widget->text.output->data;
    XmTextPosition lastChar, lineEnd, nextLeft, nextBreak, lastBreak, oldpos;
    XmTextPosition startpos;
    XmTextBlockRec block;
    int x, lastX, goalwidth, length, i;

    lastChar = (*widget->text.source->Scan)(widget->text.source, position,
                                 XmSELECT_LINE, XmsdRight, 1, FALSE);
    lastBreak = startpos = position;
    x = lastX = data->leftmargin;
    goalwidth = widget->text.inner_widget->core.width - data->rightmargin;
    while (position < lastChar) {
        nextLeft = (*widget->text.source->Scan)(widget->text.source, position,
                                            XmSELECT_WHITESPACE, XmsdRight,
                                            1, FALSE);
        nextBreak = (*widget->text.source->Scan)(widget->text.source, nextLeft,
                                            XmSELECT_WHITESPACE, XmsdRight,
                                            1, TRUE);
        while (position < nextLeft) {
            position = (*widget->text.source->ReadSource)(widget->text.source,
                                                   position, nextLeft, &block);
            length = block.format == FMT16BIT ? block.length/2 : block.length;
            x += FindWidth(data, x, &block, 0, block.length);
            if (x > goalwidth) {
                if (lastBreak > startpos) {
                    if (lastX <= goalwidth) /* word wrap is being performed */
                    {
                        return lastBreak;
                    }
                    x = lastX;
                    oldpos = position = lastBreak;
                    while (x > goalwidth && position > startpos) {
                        oldpos = position;
                        position = (*widget->text.source->Scan)
                            (widget->text.source, position, XmSELECT_POSITION,
                             XmsdLeft, 1, TRUE);
                        (void) (*widget->text.source->ReadSource)
                            (widget->text.source, position, oldpos, &block);
                        x -= FindWidth(data, x, &block, 0, 1);
                    }
                    if (extra) {
                        *extra = (LineTableExtra)
                            XtMalloc((unsigned) sizeof(LineTableExtraRec));
                        (*extra)->wrappedbychar = TRUE;
                    }
                    return oldpos; /* Allows one whitespace char to appear */
                                   /* partially off the edge. */
                }
                if (extra) {
                    *extra = (LineTableExtra)
                        XtMalloc((unsigned) sizeof(LineTableExtraRec));
                    (*extra)->wrappedbychar = TRUE;
                }
                for (i=length - 1 ; i>=0 && x > goalwidth ; i--) {
                    x -= FindWidth(data, x, &block, i, i+1);
                    position =
                        (*widget->text.source->Scan)(widget->text.source,
                                                position, XmSELECT_POSITION,
                                                XmsdLeft, 1, TRUE);
                }
                return position;
            }
        }
        while (position < nextBreak) {
            position = (*widget->text.source->ReadSource)(widget->text.source,
                                                position, nextBreak, &block);
            length = block.format == FMT16BIT ? block.length/2 : block.length;
            x += FindWidth(data, x, &block, 0, block.length);
        }
        lastBreak = nextBreak;
        lastX = x;
    }
    lineEnd = (*widget->text.source->Scan)(widget->text.source, lastChar,
                                 XmSELECT_LINE, XmsdRight, 1, TRUE);
    if (lineEnd != lastChar) return lineEnd;
    else return PASTENDPOS;
}

static XtGeometryResult TryResize(widget, width, height)
XmTextWidget widget;
Dimension width, height;
{
    XtGeometryResult result;
    Dimension origwidth = widget->text.inner_widget->core.width;
    Dimension origheight = widget->text.inner_widget->core.height;
    XtWidgetGeometry request, reply;

    if (origwidth != width) {
       request.request_mode = CWWidth;
       request.width = width;
    } else
       request.request_mode = NULL;

    if (origheight != height) {
       request.request_mode |= CWHeight;
       request.height = height;
    }

    /* requesting current size */
    if (request.request_mode == NULL) return XtGeometryNo;                        /* PIR2693 */

    result = XtMakeGeometryRequest(widget->text.inner_widget, &request, &reply);

    if (result == XtGeometryAlmost) {
	if (request.request_mode & CWWidth)
	   request.width = reply.width;
	if (request.request_mode & CWHeight)
	   request.height = reply.height;
        result = XtMakeGeometryRequest(widget->text.inner_widget, &request,
                                       &reply);

        if (result == XtGeometryYes) {                                            /* PIR2693 */
            result = XtGeometryNo;                                                /* PIR2693 */
	    if ((request.request_mode & CWWidth && reply.width != origwidth) ||   /* PIR2693 */
	        (request.request_mode & CWHeight && reply.height != origheight))  /* PIR2693 */
                result = XtGeometryYes;                                           /* PIR2693 */
	}                                                                         /* PIR2693 */

        return result;
    } else if (result == XtGeometryYes) {
       /* Some brain damaged geometry managers return XtGeometryYes and 
          don't change the widget's size. */
       if ((request.request_mode & CWWidth && 
         widget->text.inner_widget->core.width != width) ||
	(request.request_mode & CWHeight && 
         widget->text.inner_widget->core.height != height))
        result = XtGeometryNo;
    }
    return result;
}


static void RedisplayHBar(widget)
XmTextWidget widget;
{
    OutputData data = widget->text.output->data;
    int value, sliderSize, maximum,new_sliderSize;
    Arg args[3];

    static Arg arglist[] = {
        {XmNmaximum, NULL},
        {XmNvalue, NULL},
        {XmNsliderSize, NULL},
    };

    if (!(data->scrollhorizontal &&
          (XtClass(widget->core.parent) == xmScrolledWindowWidgetClass)))
        return;

    ChangeHOffset(widget, data->hoffset, False);/* Makes sure that hoffset is */
                                               /* still reasonable. */

    new_sliderSize = widget->text.inner_widget->core.width
             - (data->leftmargin + data->rightmargin);

    if (new_sliderSize < 1) new_sliderSize = 1;
    if (new_sliderSize > data->scrollwidth) new_sliderSize = data->scrollwidth;

    XtSetArg(args[0], XmNmaximum, &maximum);
    XtSetArg(args[1], XmNvalue, &value);
    XtSetArg(args[2], XmNsliderSize, &sliderSize);
    XtGetValues(data->hbar, args, XtNumber(args));

    if ((maximum != data->scrollwidth || 
     value != data->hoffset || 
     sliderSize != new_sliderSize) &&
         !(sliderSize == maximum && new_sliderSize == data->scrollwidth)) {
       arglist[0].value = (XtArgVal) data->scrollwidth;
       arglist[1].value = (XtArgVal) data->hoffset;
       arglist[2].value = (XtArgVal) new_sliderSize;

       data->ignorehbar = TRUE;
       XtSetValues(data->hbar, arglist, XtNumber(arglist));
       data->ignorehbar = FALSE;
    }
}

static int CountLines(widget, start, end)
XmTextWidget widget;
XmTextPosition start, end;
{
  OutputData data = widget->text.output->data;
  XmTextPosition position = start;
  XmTextPosition nextpos = start;
  LineTableExtra extra;
  int numlines = 0;

  if (ShouldWordWrap(data, widget)) {
     while(position < end) {
        nextpos = FindLineEnd(widget, position, &extra);
	if (nextpos == PASTENDPOS) break;
        if (nextpos == position)
                nextpos = (*widget->text.source->Scan)(widget->text.source,
                                                   position, XmSELECT_POSITION,
                                                   XmsdRight, 1, TRUE);
        position = nextpos;
        numlines++;
     }
  } else {
     numlines = (*widget->text.source->CountLines)(widget->text.source,
						   start, end - start);
  }
  return (numlines);
}


static ChangeVSB(widget)
XmTextWidget widget;
{
    OutputData data = widget->text.output->data;
    static Arg args[3];
    int local_total;
    int new_size;
    int old_top_line = widget->text.top_line;
    int old_total_lines = widget->text.total_lines;

    if (!widget->text.top_character)
      widget->text.top_line = 0;
    else
      if (widget->text.last_top_char < widget->text.top_character)
         widget->text.top_line += CountLines(widget, widget->text.last_top_char,
					     widget->text.top_character);
      else
         widget->text.top_line -= CountLines(widget, widget->text.top_character,
					     widget->text.last_top_char);

    if (ShouldWordWrap(data, widget))
        widget->text.total_lines = CountLines(widget,
					      widget->text.top_character,
				              widget->text.last_position) +
				              widget->text.top_line + 1;

    if (widget->text.total_lines <= 0)
       widget->text.total_lines = 1;

    if (widget->text.top_line + widget->text.number_lines >
					 widget->text.total_lines)
	local_total = widget->text.top_line + widget->text.number_lines;
   else
	local_total = widget->text.total_lines;
    if (data->vbar){
      XtSetArg(args[0], XmNmaximum, local_total);
      XtSetArg(args[1], XmNvalue, widget->text.top_line);
      
      if (local_total >= widget->text.number_lines)
	new_size = widget->text.number_lines;
      else
	new_size = local_total;

      if (new_size + widget->text.top_line > local_total)
	new_size = local_total - widget->text.top_line;

      XtSetArg(args[2], XmNsliderSize, new_size);

      data->ignorevbar = TRUE;
      XtSetValues(data->vbar, args, XtNumber(args));
      data->ignorevbar = FALSE;
      if (!XtIsManaged(data->vbar))
	XtManageChild(data->vbar);
    }
}

static void CheckForNewSize(widget, position)
XmTextWidget widget;
XmTextPosition position;
{
    OutputData data = widget->text.output->data;
    XmTextPosition first_position, start;
    register int i;
    LineNum l, numlines;
    Dimension newwidth, newheight;
    LineTableExtra extra;

    if (data->scrollvertical &&
        widget->text.edit_mode != XmSINGLE_LINE_EDIT &&
        XtClass(widget->core.parent) == xmScrolledWindowWidgetClass &&
	!widget->text.vsbar_scrolling)
        ChangeVSB(widget);

    if (data->resizewidth || (data->scrollhorizontal &&
        XtClass(widget->core.parent) == xmScrolledWindowWidgetClass))
    {
        newwidth = 0;
        for (l=0 ; l < data->number_lines ; l++) {
            _XmTextLineInfo(widget, l, &start, &extra);
            if (extra && newwidth < extra->width) newwidth = extra->width;
        }
        if (data->scrollhorizontal &&
            XtClass(widget->core.parent) == xmScrolledWindowWidgetClass)
        {
            newwidth -= (data->rightmargin + data->leftmargin);
            if (newwidth != data->scrollwidth && newwidth > 0) {
                data->scrollwidth = newwidth;
                RedisplayHBar(widget);
            }
            newwidth = widget->text.inner_widget->core.width;
        } else if (newwidth < data->minwidth) newwidth = data->minwidth;
    } else newwidth = widget->text.inner_widget->core.width;

    newheight = widget->text.inner_widget->core.height;
    if (data->resizeheight) {
        newheight = data->number_lines * data->lineheight +
            data->topmargin + data->bottommargin;
        _XmTextLineInfo(widget, (LineNum) 0, &start, &extra);
        if (start > 0) {
            first_position = (*widget->text.source->Scan)
                                        (widget->text.source, start,
                                         XmSELECT_ALL, XmsdLeft, 1, TRUE);
            if (start > first_position) {
                XmTextSetTopCharacter((Widget)widget, start);
                return;
            }
        }
        if (position != PASTENDPOS) {
            newheight += data->lineheight;
        } else {
            numlines = _XmTextNumLines(widget);
            _XmTextLineInfo(widget, numlines - 1, &start, &extra);

            for (i = 2; start == PASTENDPOS &&
                               newheight > data->minheight &&
		               i < numlines; i++) {
                newheight -= data->lineheight;
                _XmTextLineInfo(widget, numlines - i, &start, &extra);
            }
        }
        if (newheight < data->minheight) newheight = data->minheight;
    }
    if ((newwidth != widget->text.inner_widget->core.width ||
         newheight != widget->text.inner_widget->core.height) &&
         !widget->text.in_resize) {
        if (TryResize(widget, newwidth, newheight) == XtGeometryYes)
            NotifyResized(widget, FALSE);
        else
	    widget->text.needs_refigure_lines = False;
    }
}

static Boolean MeasureLine(widget, line, position, nextpos, extra)
XmTextWidget widget;
LineNum line;
XmTextPosition position;
XmTextPosition *nextpos;        /* RETURN */
LineTableExtra *extra;          /* RETURN */
{
    OutputData data = widget->text.output->data;
    XmTextPosition temp, last_position;
    XmTextBlockRec block;
    Dimension width;

    posToXYCachedWidget = NULL;
    if (extra) *extra = NULL;
    if (line >= data->number_lines) {
        if (data->resizewidth || data->resizeheight ||
           ((data->scrollvertical || data->scrollhorizontal) &&
            XtClass(widget->core.parent) == xmScrolledWindowWidgetClass)) {
             CheckForNewSize(widget, position);
        }
        return(False);
    }
    if (nextpos) {
        if (position == PASTENDPOS) {
            *nextpos = last_position = PASTENDPOS;
        } else {
            if (ShouldWordWrap(data, widget)) {
                *nextpos = FindLineEnd(widget, position, extra);
            } else {
                last_position = (*widget->text.source->Scan)
                                                 (widget->text.source,
                                                  position, XmSELECT_LINE,
                                                  XmsdRight, 1, FALSE);
                *nextpos = (*widget->text.source->Scan)(widget->text.source,
                                                  last_position, XmSELECT_LINE,
                                                  XmsdRight, 1, TRUE);
                if (*nextpos == last_position)
                    *nextpos = PASTENDPOS;
                if (extra && (data->resizewidth || (data->scrollhorizontal &&
                    XtClass(widget->core.parent) ==
				xmScrolledWindowWidgetClass))) {
                    (*extra) = (LineTableExtra)
                        XtMalloc((unsigned) sizeof(LineTableExtraRec));
                    (*extra)->wrappedbychar = FALSE;
                    width = data->leftmargin;
                    temp = position;
                    while (temp < last_position) {
                        temp = (*widget->text.source->ReadSource)
			     (widget->text.source, temp, last_position, &block);
                        width += FindWidth(data, (Position) width, &block,
                                                0, block.length);
                    }
                    (*extra)->width = width + data->rightmargin;
                }
            }
            if (*nextpos == position)
                *nextpos = (*widget->text.source->Scan)(widget->text.source,
                                                   position, XmSELECT_POSITION,
                                                   XmsdRight, 1, TRUE);
        }
    }
    return (True);
}


static void Draw(widget, line, start, end, highlight)
XmTextWidget widget;
LineNum line;
XmTextPosition start, end;
XmHighlightMode highlight;
{
    OutputData data = widget->text.output->data;
    XFontStruct *font = data->font;
    XmTextPosition linestart, nextlinestart;
    LineTableExtra extra;
    XmTextBlockRec block;
    int x, y, length, newx, i;
    int text_border;
    int rightedge = ((int)widget->text.inner_widget->core.width)+ data->hoffset;

    int width, height;
    Boolean cleartoend, cleartobottom;

    if (!XtIsRealized((Widget) widget)) return;
    _XmTextLineInfo(widget, line+1, &nextlinestart, &extra);
    _XmTextLineInfo(widget, line, &linestart, &extra);

    if (!data->has_rect) _XmTextAdjustGC(widget);

    if (linestart == PASTENDPOS) {
        start = end = nextlinestart = PASTENDPOS;
        cleartoend = cleartobottom = TRUE;
    } else if (nextlinestart == PASTENDPOS) {
        nextlinestart = (*widget->text.source->Scan)(widget->text.source, 0,
                                        XmSELECT_ALL, XmsdRight, 1, FALSE);
        cleartoend = cleartobottom = (end >= nextlinestart);
        if (start >= nextlinestart) highlight = XmHIGHLIGHT_NORMAL;
    } else {
        cleartobottom = FALSE;
        cleartoend = (end >= nextlinestart);
        if (cleartoend && (!extra || !extra->wrappedbychar))
            end = (*widget->text.source->Scan)(widget->text.source,
                         nextlinestart, XmSELECT_POSITION, XmsdLeft, 1, TRUE);
    }
    y = data->topmargin + line * data->lineheight + font->ascent;
    x = data->leftmargin;
    while (linestart < start && x <= rightedge) {
        linestart = (*widget->text.source->ReadSource)(widget->text.source,
                                                      linestart, start, &block);
        x += FindWidth(data, x, &block, 0, block.length);
    }

    while (start < end && x <= rightedge) {
        start = (*widget->text.source->ReadSource)(widget->text.source, start,
                                                                 end, &block);
        while (block.length > 0) {
            if (block.format == FMT16BIT) length = block.length / 2;
            else {
                while (block.ptr[0] == '\t') {
                    newx = x;
                    while (block.length > 0 &&
			   newx - data->hoffset < data->leftmargin) {
                        width = FindWidth(data, newx, &block, 0, 1);
                        newx += width;
    
                        if (newx - data->hoffset < data->leftmargin) {
                           block.length--;
                           block.ptr++;
                           x = newx;
                        }
                    }
                    if (block.length <= 0 || block.ptr[0] != '\t') break;

                    width = FindWidth(data, x, &block, 0, 1);
   
                    if (highlight == XmHIGHLIGHT_SELECTED)
		       XmSetNormGC(widget, data->gc);
                    else
                       XmSetInvGC(widget, data->gc);
                    XmSetFullGC(widget, data->gc);
                    XFillRectangle(XtDisplay(widget),
                                   XtWindow(widget->text.inner_widget),
                                   data->gc, x - data->hoffset,
				   y - font->ascent,
                                   width, data->lineheight);
                    XmSetMarginGC(widget, data->gc);
                    if (highlight == XmHIGHLIGHT_SECONDARY_SELECTED) {
                        if (highlight == XmHIGHLIGHT_SELECTED)
			    XmSetInvGC(widget, data->gc);
                        else
                            XmSetNormGC(widget, data->gc);
                        XDrawLine(XtDisplay(widget),
                                  XtWindow(widget->text.inner_widget),
                                  data->gc, x - data->hoffset, y,
				  ((x - data->hoffset) + width) - 1, y);
                    }
                    x += width;
                    block.ptr++;
                    if (--block.length <= 0) break;
                }
                for ( length = 0; length < block.length; length++ ) {
                    if (block.ptr[length] == '\t') break;
                }
            }
            if (length <= 0) break;
            if (block.format == FMT16BIT) {
                if (highlight == XmHIGHLIGHT_SELECTED)
		   XmSetInvGC(widget, data->gc);
                else
		   XmSetNormGC(widget, data->gc);
                XDrawImageString16(XtDisplay(widget),
                                  XtWindow(widget->text.inner_widget), data->gc,                                  x, y, (XChar2b *) block.ptr, length);
                x += FindWidth(data, x, &block, 0, length);
                block.length -= length * 2;
                block.ptr += length * 2;
            } else {
                newx = x;
                while (length > 0 && newx - data->hoffset < data->leftmargin) {
                    newx += FindWidth(data, newx, &block, 0, 1);

                    if (newx - data->hoffset < data->leftmargin) {
                       length--;
                       block.length--;
                       block.ptr++;
                       x = newx;
                    }
                }
                if (length == 0) continue;
                    newx = x + FindWidth(data, x, &block, 0, length);
                if (newx > rightedge) {
                    int lastx;
                    newx = x;
                    for (i=0 ; i<length && newx <= rightedge ; i++) {
                        lastx = newx;
                        newx += FindWidth(data, newx, &block, i, i+1);
                    }
                    length = i;
                    newx = lastx;
                    start = end; /* Force a break out of the outer loop. */
                    block.length = length; /* ... and out of the inner loop. */
                }
                if (highlight == XmHIGHLIGHT_SELECTED)
		   XmSetInvGC(widget, data->gc);
                else
		   XmSetNormGC(widget, data->gc);
                XDrawImageString(XtDisplay(widget),
                                 XtWindow(widget->text.inner_widget), data->gc,
                                 x - data->hoffset, y, block.ptr, length);
                if (highlight == XmHIGHLIGHT_SECONDARY_SELECTED)
                    XDrawLine(XtDisplay(widget),
                              XtWindow(widget->text.inner_widget), data->gc,
                              x - data->hoffset, y,
			      (newx - data->hoffset) - 1, y);
                x = newx;
                block.length -= length;
                block.ptr += length;
            }
        }
    }

    /* clear left margin */
    text_border = widget->primitive.shadow_thickness +
                  widget->primitive.highlight_thickness;
    if (data->leftmargin - text_border > 0 && y + font->descent > 0)
           XClearArea(XtDisplay(widget), XtWindow(widget->text.inner_widget),
                      text_border, text_border, data->leftmargin - text_border,
                      y + font->descent - text_border, FALSE);

    if (cleartoend) {
        x -= data->hoffset;
        if (x > ((int)widget->text.inner_widget->core.width)- data->rightmargin)
            x = ((int)widget->text.inner_widget->core.width)- data->rightmargin;
        if (x < data->leftmargin)
            x = data->leftmargin;
        width = ((int)widget->text.inner_widget->core.width) - x -
                (widget->primitive.shadow_thickness +
                 widget->primitive.highlight_thickness);
        if (width > 0 && data->lineheight > 0) {
          if (highlight == XmHIGHLIGHT_SELECTED) XmSetNormGC(widget, data->gc);
          else XmSetInvGC(widget, data->gc);
          XmSetFullGC(widget, data->gc);
          XFillRectangle(XtDisplay(widget), XtWindow(widget->text.inner_widget),
                         data->gc, x, y - font->ascent, width,
                         data->lineheight);
          XmSetMarginGC(widget, data->gc);
        }
    }
    if (cleartobottom) {
        x =  widget->primitive.shadow_thickness +
             widget->primitive.highlight_thickness;
        width = widget->text.inner_widget->core.width -
                2 * (widget->primitive.shadow_thickness +
                widget->primitive.highlight_thickness);
        height = widget->text.inner_widget->core.height -
                 ((y + font->descent) +
                 widget->primitive.shadow_thickness +
                 widget->primitive.highlight_thickness);
        if (width > 0 && height > 0)
            XClearArea(XtDisplay(widget), XtWindow(widget->text.inner_widget),
                       x, y + font->descent, width, height, FALSE);
    }
}



static OnOrOff CurrentCursorState(widget)
XmTextWidget widget;
{
    OutputData data = widget->text.output->data;
    if (data->onoroff == off) return off;
    if (!data->hasfocus || data->blinkstate == on || !XtSensitive(widget))
        return on;
    return off;
}



/*
 * Draws the destination cursor.
 */

#ifdef _NO_PROTO
void _XmTextDrawDestination(widget)
XmTextWidget widget;
#else /* _NO_PROTO */
void _XmTextDrawDestination(XmTextWidget widget)
#endif /* _NO_PROTO */
{
    OutputData data = widget->text.output->data;
    Position x, y;
    XFontStruct *font = data->font;
    Position insertx, inserty;
    Boolean old_needs_refigure_lines = False;

    if (!XtIsRealized((Widget) widget) || !XtSensitive((Widget) widget) ||
        data->dest_on || !data->dest_visible || !data->cursor_position_visible)
       return;

    if (!data->has_rect) _XmTextAdjustGC(widget);

    if (widget->text.needs_refigure_lines) {
        old_needs_refigure_lines = True;
        widget->text.needs_refigure_lines = False;
    }

    if (PosToXY(widget, widget->text.dest_position, &insertx, &inserty)) {

      /* offset half of the caret width minus 1 */
       x = insertx - ((CARET_WIDTH >> 1) - 1);

      /*
       * offset the font descent minus the cursor height plus 2 i.e.
       * the top of the cursor is 2 pixels below the base line
       */
       y = inserty + ((font->descent - CARET_HEIGHT) + 2);

       if (y <= data->topmargin || x < 0 || y < 0) return;

       XCopyArea(XtDisplay(widget), data->dest_cursor,
                 XtWindow(widget->text.inner_widget), data->imagegc,
                 0, 0, CARET_WIDTH, CARET_HEIGHT, x, y);

    }

    if (old_needs_refigure_lines) {
        widget->text.needs_refigure_lines = True;
    }

    data->dest_on = True;
}

/*
 * Clear the rectangle containing the cursor.
 */

#ifdef _NO_PROTO
void _XmTextClearDestination(widget, ignore_sens)
XmTextWidget widget;
Boolean ignore_sens;
#else /* _NO_PROTO */
void _XmTextClearDestination(XmTextWidget widget, Boolean ignore_sens)
#endif /* _NO_PROTO */
{
    OutputData data = widget->text.output->data;
    Position x, y;
    XFontStruct *font = data->font;
    Position insertx, inserty;
    Boolean old_needs_refigure_lines = False;

    if (!XtIsRealized((Widget) widget) ||
	(!ignore_sens && !XtSensitive((Widget) widget)) ||
        !data->dest_on || !data->dest_visible || !data->cursor_position_visible)
       return;

    if (!data->has_rect) _XmTextAdjustGC(widget);

    if (widget->text.needs_refigure_lines) {
        old_needs_refigure_lines = True;
        widget->text.needs_refigure_lines = False;
    }

    if (PosToXY(widget, widget->text.dest_position, &insertx, &inserty)) {

      /* offset half of the caret width minus 1 */
       x = insertx - ((CARET_WIDTH >> 1) - 1);

      /*
       * offset the font descent minus the cursor height plus 2 i.e.
       * the top of the cursor is 2 pixels below the base line
       */
       y = inserty + ((font->descent - CARET_HEIGHT) + 2);

       if (y <= data->topmargin) return;

       if (x < 0 || y < 0) return;

       XCopyArea(XtDisplay(widget), data->dest_cursor,
                 XtWindow(widget->text.inner_widget), data->imagegc,
                 0, 0, CARET_WIDTH, CARET_HEIGHT, x, y);

    }

    if (old_needs_refigure_lines) {
        widget->text.needs_refigure_lines = True;
    }

    data->dest_on = False;
}


#ifdef _NO_PROTO
void _XmTextDestinationVisible(w, turn_on)
Widget w;
Boolean turn_on;
#else /* _NO_PROTO */
void _XmTextDestinationVisible(Widget w, Boolean turn_on)
#endif /* _NO_PROTO */
{
   XmTextWidget tw = (XmTextWidget) w;
   OutputData data = tw->text.output->data;

   if (turn_on) {
      if (data->dest_visible) return;
      data->dest_visible = True;
      if (tw->text.input->data->has_destination &&
            (tw->text.dest_position != tw->text.cursor_position ||
	        !tw->text.output->data->hasfocus))
         _XmTextDrawDestination(tw);
   } else {
      if (!data->dest_visible) return;
      if (tw->text.input->data->has_destination &&
            (tw->text.dest_position != tw->text.cursor_position ||
	        !tw->text.output->data->hasfocus))
         _XmTextClearDestination(tw, False);
      data->dest_visible = False;
   }
}


/*
 * All the info about the cursor has been figured; draw or erase it.
 */

static void PaintCursor(widget)
XmTextWidget widget;
{
    OutputData data = widget->text.output->data;
    Position x, y;
    Pixmap pixmap, cursor;
    XFontStruct *font = data->font;

    if (!data->cursor_position_visible) return;

    if (!data->has_rect) _XmTextAdjustGC(widget);

    x = data->insertx - (data->cursorwidth >> 1) - 1;
    y = data->inserty + font->descent - data->cursorheight;

    if (widget->text.add_mode)
       cursor = data->add_mode_cursor;
    else
       cursor = data->cursor;

    if (!data->hasfocus || !XtSensitive(widget)) {
        pixmap = data->putback;
    } else {
        pixmap = (data->onoroff == on && data->blinkstate == on) ?
                 cursor : data->putback;
    }
    if (data->putback != NULL && pixmap != data->putback)
        XCopyArea(XtDisplay(widget), data->putback,
                  XtWindow(widget->text.inner_widget), data->imagegc,
                  0, 0, data->cursorwidth, data->cursorheight, x, y);
    if (pixmap != NULL)
        XCopyArea(XtDisplay(widget), pixmap,
                  XtWindow(widget->text.inner_widget), data->imagegc,
                  0, 0, data->cursorwidth, data->cursorheight, x, y);
    if (data->putback == pixmap)
        data->putback = NULL;
    else
    data->putback = pixmap;
}



/*
 * Clear the rectangle containing the cursor.
 */

static void ClearCursor(widget)
XmTextWidget widget;
{
    OutputData data = widget->text.output->data;
    Position x, y;
    XFontStruct *font = data->font;

    if (!XtIsRealized((Widget) widget)) return;

    if (!data->has_rect) _XmTextAdjustGC(widget);

    x = data->insertx - (data->cursorwidth >> 1) - 1;
    y = data->inserty + font->descent - data->cursorheight;

    XClearArea(XtDisplay(widget), XtWindow(widget->text.inner_widget),
               x, y, data->cursorwidth, data->cursorheight, FALSE);
    RedrawRegion(widget, x, y, data->cursorwidth, data->cursorheight);
}



static void ChangeHOffset(widget, newhoffset, redisplay_hbar)
XmTextWidget widget;
int newhoffset;
Boolean redisplay_hbar;
{
    OutputData data = widget->text.output->data;
    int delta;
    int width = widget->text.inner_widget->core.width;
    int height = widget->text.inner_widget->core.height;
    int innerwidth = width - (data->leftmargin + data->rightmargin);
    int innerheight = height - (data->topmargin + data->bottommargin);
    if (ShouldWordWrap(data, widget)) return;
    if ((data->scrollhorizontal &&
            XtClass(widget->core.parent) == xmScrolledWindowWidgetClass) &&
	data->scrollwidth - innerwidth < newhoffset)
        newhoffset = data->scrollwidth - innerwidth;
    if (newhoffset < 0) newhoffset = 0;
    if (newhoffset == data->hoffset) return;
    if (!data->has_rect) _XmTextAdjustGC(widget);
    delta = newhoffset - data->hoffset;
    data->hoffset = newhoffset;
    posToXYCachedWidget = NULL;
    XmSetNormGC(widget, data->gc);
    if (delta < 0) {
      if (XtIsRealized(widget)) {
        if (width > 0 && innerheight > 0) {
            XCopyArea(XtDisplay(widget), XtWindow(widget->text.inner_widget),
                      XtWindow(widget->text.inner_widget), data->gc,
                      data->leftmargin, data->topmargin, width, innerheight,
                      data->leftmargin - delta, data->topmargin);
            /* clear left margin + delta change */
            if ((data->leftmargin - (widget->primitive.shadow_thickness +
                widget->primitive.highlight_thickness) - delta) < innerwidth)
               XClearArea(XtDisplay(widget), XtWindow(widget),
                       widget->primitive.shadow_thickness +
                       widget->primitive.highlight_thickness, data->topmargin,
                       data->leftmargin - (widget->primitive.shadow_thickness +
                       widget->primitive.highlight_thickness) - delta,
                       innerheight, FALSE);
            /* clear right margin */
            XClearArea(XtDisplay(widget), XtWindow(widget),
                      data->leftmargin + innerwidth, data->topmargin,
                      data->rightmargin - (widget->primitive.shadow_thickness +
                      widget->primitive.highlight_thickness),
                      innerheight,
                      FALSE);
            data->exposehscroll++;
        }
        RedrawRegion(widget, data->leftmargin, 0, -delta, height);
      }
    } else {
      if (XtIsRealized(widget)) {
        if (innerwidth - delta > 0 && innerheight > 0) {
            XCopyArea(XtDisplay(widget), XtWindow(widget->text.inner_widget),
                      XtWindow(widget->text.inner_widget), data->gc,
                      data->leftmargin + delta, data->topmargin,
                      innerwidth - delta, innerheight,
                      data->leftmargin, data->topmargin);
            /* clear right margin + delta change */
            XClearArea(XtDisplay(widget), XtWindow(widget),
                      data->leftmargin + innerwidth - delta, data->topmargin,
                      delta + data->rightmargin -
                      (widget->primitive.shadow_thickness +
                      widget->primitive.highlight_thickness),
                      innerheight, FALSE);
            /* clear left margin */
            XClearArea(XtDisplay(widget), XtWindow(widget),
                      widget->primitive.shadow_thickness +
                      widget->primitive.highlight_thickness,
                      data->topmargin, data->leftmargin -
                      (widget->primitive.shadow_thickness +
                      widget->primitive.highlight_thickness),
                      innerheight, FALSE);
            data->exposehscroll++;
        } else {
         /* clear all text */
          XClearArea(XtDisplay(widget), XtWindow(widget),
                    widget->primitive.shadow_thickness +
                    widget->primitive.highlight_thickness,
                    data->topmargin,
                    width - 2 *(widget->primitive.shadow_thickness +
                    widget->primitive.highlight_thickness),
                    innerheight,
                    FALSE);
          data->exposehscroll++;
        }
        RedrawRegion(widget, width - data->rightmargin - delta, 0,
                     delta, height);
      }
    }
    if (redisplay_hbar) RedisplayHBar(widget);
}



static void DrawInsertionPoint(widget, position, onoroff)
XmTextWidget widget;
XmTextPosition position;
OnOrOff onoroff;
{
    OutputData data = widget->text.output->data;

    if (data->onoroff == onoroff || !XtIsRealized((Widget) widget))
        return;
    data->onoroff = onoroff;

    if (data->onoroff != on && data->blinkrate != 0) data->blinkstate = off;

    if (PosToXY(widget, position, &data->insertx, &data->inserty)) {
        PaintCursor(widget);
    } else
        data->onoroff = off;
}



static void MakePositionVisible(widget, position)
XmTextWidget widget;
XmTextPosition position;
{
    OutputData data = widget->text.output->data;
    Position x, y;

    if (!ShouldWordWrap(data, widget) && PosToXY(widget, position, &x, &y)) {
        x += data->hoffset;
        if (x - data->hoffset < data->leftmargin) {
            ChangeHOffset(widget, x - data->leftmargin, True);
        } else if (x - data->hoffset >
                   ((Position) (widget->text.inner_widget->core.width -
                                data->rightmargin))) {
            ChangeHOffset(widget, (int) (x) -
                                 (int) (widget->text.inner_widget->core.width) +
                                 (int) (data->rightmargin), True);
        }
    }
}

static void BlinkInsertionPoint(widget)
XmTextWidget widget;
{
    OutputData data = widget->text.output->data;
    if (data->onoroff == on && data->blinkstate == CurrentCursorState(widget)){
        if (data->blinkstate == on) data->blinkstate = off;
        else data->blinkstate = on;
        (void) PosToXY(widget, widget->text.cursor_position,
                       &data->insertx, &data->inserty);
        PaintCursor(widget);
    }
}



static Boolean MoveLines(widget, fromline, toline, destline)
XmTextWidget widget;
LineNum fromline, toline, destline;
{
    OutputData data = widget->text.output->data;
    if (!XtIsRealized((Widget) widget)) return FALSE;
    if (!data->has_rect) _XmTextAdjustGC(widget);
    XmSetNormGC(widget, data->gc);
    XmSetFullGC(widget, data->gc);
    XCopyArea(XtDisplay(widget), XtWindow(widget->text.inner_widget),
              XtWindow(widget->text.inner_widget), data->gc,
              widget->primitive.shadow_thickness +
	      widget->primitive.highlight_thickness,
              (Position) data->lineheight * fromline + data->topmargin,
              widget->text.inner_widget->core.width -
              2 * (widget->primitive.shadow_thickness +
	      widget->primitive.highlight_thickness),
              data->lineheight * (toline - fromline + 1),
              widget->primitive.shadow_thickness +
	      widget->primitive.highlight_thickness,
              (Position) data->lineheight * destline + data->topmargin);
    XmSetMarginGC(widget, data->gc);
    data->exposevscroll++;
    return TRUE;
}

/* ARGSUSED */
static void OutputInvalidate(widget, position, topos, delta)
XmTextWidget widget;
XmTextPosition position, topos;
int delta;
{
    OutputData data = widget->text.output->data;
    if (data->blinkrate > 0)
        widget->text.output->data->blinkstate = off;
    posToXYCachedWidget = NULL;
}

static void RefigureDependentInfo(widget)
XmTextWidget widget;
{
    OutputData data = widget->text.output->data;

    data->rows = data->number_lines;
    data->columns = (widget->core.width -
                    (data->leftmargin + data->rightmargin))
        / (data->averagecharwidth);
}


static void SizeFromRowsCols(widget, width, height)
XmTextWidget widget;
Dimension *width, *height;
{
    OutputData data = widget->text.output->data;
    *width = data->columns * data->averagecharwidth +
        data->leftmargin + data->rightmargin;
    *height = data->rows * data->lineheight +
        data->topmargin + data->bottommargin;
}


static void LoadFontMetrics(widget)
XmTextWidget widget;
{
    OutputData data = widget->text.output->data;
    XmFontContext context;
    XmStringCharSet charset;
    XFontStruct *font;
    unsigned long w;

    if (!XmFontListInitFontContext(&context, data->fontlist))
       _XmWarning (widget, "XmFontListInitFontContext Failed.");

    if (!XmFontListGetNextFont(context, &charset, &font))
       _XmWarning (widget, "XmFontListGetNextFont Failed.");

    data->font = font;
    XtFree(charset);
    XmFontListFreeFontContext(context);

    data->lineheight = font->descent + font->ascent;
    w = 0;
    if ((!XGetFontProperty(font, XA_QUAD_WIDTH, &w)) || w == 0) {
        if (font->per_char && font->min_char_or_byte2 <= '0' &&
                              font->max_char_or_byte2 >= '0')
            w = font->per_char['0' - font->min_char_or_byte2].width;
        else
        w = font->max_bounds.width;
    }
    if (w <= 0) w = 1;
    data->averagecharwidth = w;
    data->tabwidth = 8 * w;
}


static void LoadGCs(widget, background, foreground)
XmTextWidget widget;
Pixel background, foreground;
{
   OutputData data = widget->text.output->data;
   unsigned long valuemask = (GCFont | GCGraphicsExposures | GCFunction |
                              GCForeground | GCBackground | GCClipMask);
   XGCValues values;
   Display *display = XtDisplay((Widget)widget);
   static XContext context = NULL;
   Pixmap tw_cache_pixmap;

   if (context == NULL) context = XUniqueContext();

   if (XFindContext(display, (Window)0, context, 
		    (caddr_t*)&tw_cache_pixmap)) {

      /* Get the Pixmap identifier that the X Toolkit uses to cache our */
      /* GC's.  We never actually use this Pixmap; just so long as it's */
      /* a unique identifier. */
      tw_cache_pixmap = XCreatePixmap(display, DefaultRootWindow(display),
                                       1, 1, 1);
      XSaveContext(display, (Window)0, context, (void*)tw_cache_pixmap);
   }

   values.font = data->font->fid;
   values.graphics_exposures = (Bool) TRUE;
   values.function = GXcopy;
   values.foreground = foreground ^ background;
   values.background = 0;
   values.clip_mask = tw_cache_pixmap;
   if (data->gc != NULL)
       XtReleaseGC(widget, data->gc);
   data->gc = XtGetGC((Widget) widget, valuemask, &values);
   valuemask |= GCTile | GCFillStyle;
   values.tile = data->stipple_tile;
   values.fill_style = FillSolid;
   if (data->imagegc != NULL)
       XtReleaseGC(widget, data->imagegc);
   data->imagegc = XtGetGC((Widget) widget, valuemask, &values);
}

static void MakeIBeamCursor(widget, pixmap_name, cursor, line_width)
XmTextWidget widget;
char *pixmap_name;
Pixmap *cursor;
int line_width;
{
   OutputData data = widget->text.output->data;
   Screen *screen = XtScreen(widget);
   Display *dpy = XtDisplay(widget);
   XSegment segments[3];
   Pixel fg = widget->primitive.foreground ^ widget->core.background_pixel;
   GC fillGC;
   XRectangle ClipRect;

  /* Create a pixmap to store the I-Beam cursor */
   *cursor =  XCreatePixmap(dpy, XtWindow(widget->text.inner_widget),
                                 data->cursorwidth, data->cursorheight,
                                 screen->root_depth);

   /* Create a deafult GC for drawing the the segmentd */
    fillGC = XCreateGC(dpy, *cursor, 0, (XGCValues *)NULL);

   /* Fill the cursor with 0's */
    XFillRectangle(dpy, *cursor, fillGC, 0, 0,
                   data->cursorwidth, data->cursorheight);

   /* Draw the segments of the I-Beam */
   /* 1st segment is the top horizontal line of the 'I' */
    segments[0].x1 = 0;
    segments[0].y1 = line_width - 1;
    segments[0].x2 = data->cursorwidth;
    segments[0].y2 = line_width - 1;

   /* 2nd segment is the vertical line of the 'I' */
    segments[1].x1 = 0;
    segments[1].y1 = data->cursorheight - 1;
    segments[1].x2 = data->cursorwidth;
    segments[1].y2 = data->cursorheight - 1;

   /* 3rd segment is the bottom horizontal line of the 'I' */
    segments[2].x1 = data->cursorwidth/2;
    segments[2].y1 = line_width;
    segments[2].x2 = data->cursorwidth/2;
    segments[2].y2 = data->cursorheight - 1;

   /* Set the clipping rectangle of the image GC from drawing */
    ClipRect.width = data->cursorwidth;
    ClipRect.height = data->cursorheight;
    ClipRect.x = 0;
    ClipRect.y = 0;

    XSetClipRectangles(XtDisplay(widget), data->imagegc, 0, 0,
                       &ClipRect, 1, Unsorted);

   /* Draw the segments onto the pixmap */
    XDrawSegments(dpy, *cursor, data->imagegc, segments, 3);

   /* Install the pixmap for pixmap caching */
    _XmInstallPixmap(*cursor, screen, pixmap_name, fg, 0);

   /* Free the fill GC */
    XFreeGC(XtDisplay(widget), fillGC);
}

static void MakeAddModeCursor(widget, pixmap_name, cursor)
XmTextWidget widget;
char *pixmap_name;
Pixmap *cursor;
{
   OutputData data = widget->text.output->data;
   XtGCMask  valueMask;
   XGCValues values;
   int line_width = 1;

   if (data->cursorheight > 19)
      line_width = 2;

   valueMask = GCFillStyle | GCLineWidth;
   values.fill_style = FillTiled;
   values.line_width = line_width;

   XChangeGC(XtDisplay(widget), data->imagegc, valueMask, &values);

   MakeIBeamCursor(widget, pixmap_name, cursor, line_width);

   valueMask = GCFillStyle;
   values.fill_style = FillSolid;

   XChangeGC(XtDisplay(widget), data->imagegc, valueMask, &values);
}

static void MakeDestCursor(widget)
XmTextWidget widget;
{
   OutputData data = widget->text.output->data;
   Display *dpy = XtDisplay((Widget)widget);
   Screen *screen = XtScreen((Widget)widget);
   unsigned long fg = widget->primitive.foreground ^ widget->core.background_pixel;

   /* Create the filled caret pixmap */
    data->dest_cursor = XCreatePixmapFromBitmapData(dpy,
					            RootWindowOfScreen(screen),
					            (char*)caretBits, 
						    CARET_WIDTH,
					            CARET_HEIGHT, 
						    fg, 0, 
                                                    screen->root_depth);
   /* Install the pixmap for pixmap caching */
    _XmInstallPixmap(data->dest_cursor, screen, "_XmText_Caret", fg, 0);
}

static void MakeCursors(widget)
XmTextWidget widget;
{
   OutputData data = widget->text.output->data;
   XFontStruct *font = data->font;
   Screen *screen = XtScreen(widget);
   Pixel fg = widget->primitive.foreground ^ widget->core.background_pixel;
   char pixmap_name[25];
   int line_width = 1;

   if (!XtIsRealized((Widget) widget)) return;

   data->cursorwidth = 5;
   data->cursorheight = font->ascent + font->descent;

  /* Remove old destination cursor */
   if (data->dest_cursor != XmUNSPECIFIED_PIXMAP)
       (void) XmDestroyPixmap(screen, data->dest_cursor);

  /* Create a new destination cursor */
   data->dest_cursor = XmGetPixmap(screen, "_XmText_Caret", fg, 0);
   if (data->dest_cursor == XmUNSPECIFIED_PIXMAP)
      MakeDestCursor(widget);

  /* Remove old add mode cursor */
   if (data->add_mode_cursor != XmUNSPECIFIED_PIXMAP)
       (void) XmDestroyPixmap(screen, data->add_mode_cursor);

   if (data->cursorheight > 19) {
      data->cursorwidth++;
      line_width = 2;
   }

  /* Create a new add_mode cursor */
   sprintf(pixmap_name, "_XmText_AddMode_%d_%d", data->cursorheight,line_width);
   data->add_mode_cursor = XmGetPixmap(screen, pixmap_name, fg, 0);
   if (data->add_mode_cursor == XmUNSPECIFIED_PIXMAP)
      MakeAddModeCursor(widget, pixmap_name, &(data->add_mode_cursor));

  /* Remove old insert cursor */
   if (data->cursor != XmUNSPECIFIED_PIXMAP)
       (void) XmDestroyPixmap(screen, data->cursor);

  /* Create a new i-beam cursor */
   sprintf(pixmap_name, "_XmText_%d_%d", data->cursorheight, line_width);
   data->cursor = XmGetPixmap(screen, pixmap_name, fg, 0);
   if (data->cursor == XmUNSPECIFIED_PIXMAP)
      MakeIBeamCursor(widget, pixmap_name, &(data->cursor), line_width);

   XmSetXorGC(widget, data->imagegc);
}

static void OutputGetValues(widget, args, num_args)
XmTextWidget widget;
ArgList args;
Cardinal num_args;
{
    RefigureDependentInfo(widget);
    XtGetSubvalues((caddr_t) widget->text.output->data,
                   output_resources, XtNumber(output_resources), args, num_args);
}

static Boolean CKMAR(args, num_args)
ArgList args;
Cardinal num_args;
{
    register ArgList arg;
    for (arg = args ; num_args != 0; num_args--, arg++) {
       if (strcmp(arg->name, XmNmarginWidth) == 0) return(TRUE);
       if (strcmp(arg->name, XmNmarginHeight) == 0) return(TRUE);
    }
    return(FALSE);
}

static Boolean CKHIGH(args, num_args)
ArgList args;
Cardinal num_args;
{
    register ArgList arg;
    for (arg = args ; num_args != 0; num_args--, arg++) {
       if (strcmp(arg->name, XmNhighlightThickness) == 0) return(TRUE);
    }
    return(FALSE);
}


static Boolean CKSHAD(args, num_args)
ArgList args;
Cardinal num_args;
{
    register ArgList arg;
    for (arg = args ; num_args != 0; num_args--, arg++) {
       if (strcmp(arg->name, XmNshadowThickness) == 0) return(TRUE);
    }
    return(FALSE);
}

static Boolean CKEMODE(args, num_args, edit_mode)
ArgList args;
Cardinal num_args;
int *edit_mode;
{
    register ArgList arg;
    for (arg = args ; num_args != 0; num_args--, arg++) {
       if (strcmp(arg->name, XmNeditMode) == 0) {
          *edit_mode = (int) arg->value;
          return(TRUE);
        }
    }
    return(FALSE);
}

static Boolean CKFG(args, num_args, foreground)
ArgList args;
Cardinal num_args;
Pixel *foreground;
{
    register ArgList arg;
    for (arg = args ; num_args != 0; num_args--, arg++) {
       if (strcmp(arg->name, XmNforeground) == 0) {
          *foreground = (Pixel)arg->value;
          return(TRUE);
        }
    }
    return(FALSE);
}

static Boolean CKBG(args, num_args, background)
ArgList args;
Cardinal num_args;
Pixel *background;
{
    register ArgList arg;
    for (arg = args ; num_args != 0; num_args--, arg++) {
       if (strcmp(arg->name, XmNbackground) == 0) {
          *background = (Pixel)arg->value;
          return(TRUE);
        }
    }
    return(FALSE);
}

static Boolean CKSensitive(args, num_args, widget)
ArgList args;
Cardinal num_args;
XmTextWidget widget;
{
    register ArgList arg;
    for (arg = args ; num_args != 0; num_args--, arg++) {
       if (strcmp(arg->name, XmNsensitive) == 0)
          return (Boolean) arg->value;
    }
    return XtSensitive(widget);
}  


static Boolean OutputSetValues(widget, args, num_args)
XmTextWidget widget;
ArgList args;
Cardinal num_args;
{
    OutputDataRec      newdatarec;
    OutputData         newdata  = &newdatarec;
    OutputData         data     = widget->text.output->data;

    Boolean            needgcs, newsize, newfont, newval;
    Boolean            newmargins, newhighlight, newshadow, newthickness;
    Boolean            newmode, newforeground, newbackground;
    Boolean            o_redisplay = False;

    int                edit_mode   = widget->text.edit_mode;

    Pixel              foreground  = widget->primitive.foreground;
    Pixel              background  = widget->core.background_pixel;


    *newdata = *data;
    XtSetSubvalues((caddr_t) newdata, output_resources,
		   XtNumber(output_resources), args, num_args);

#define CK(fld) (newdata->fld != data->fld)
#define CP(fld) (data->fld = newdata->fld)

    newfont = CK(fontlist);

    newforeground = CKFG(args, num_args, &foreground);
    newbackground = CKBG(args, num_args, &background);

    needgcs = newfont || newforeground || newbackground;

    if (newfont) {
       XmFontListFree(( XmFontList) data->fontlist);
       if (newdata->fontlist == NULL)
          newdata->fontlist = _XmGetDefaultFontList(widget, XmTEXT_FONTLIST);
       newdata->fontlist = (XmFontList)XmFontListCopy(newdata->fontlist);
       CP(fontlist);
       LoadFontMetrics(widget);

       o_redisplay = True;
    }

    newmode = CKEMODE(args, num_args, &edit_mode);

  /* Don't word wrap, have multiple row or have vertical scrollbars
     if editMode is single_line */
    if (newmode)
      if (edit_mode == XmSINGLE_LINE_EDIT) {
          data->rows = 1;
          o_redisplay = True;
      }
      else if ((data->scrollvertical) && (data->vbar))
	  if( !XtIsManaged(data->vbar) )
	    XtManageChild(data->vbar);

/*  what is called margin, in this code, is composed of margin, shadow, and
    highlight.   Previously, only margin was accomodated.   This addition
    may not be very clever, but it blends in with the rest of the way this
    code works.    
*/
    newmargins   = CKMAR  (args, num_args);
    newshadow    = CKSHAD (args, num_args);
    newhighlight = CKHIGH (args, num_args);

    newthickness = newmargins | newshadow | newhighlight;

    if (newthickness) {
       data->leftmargin = data->rightmargin = widget->text.margin_width +
					  widget->primitive.shadow_thickness +
					  widget->primitive.highlight_thickness;
       data->topmargin = data->bottommargin = widget->text.margin_height +
					  widget->primitive.shadow_thickness +
					  widget->primitive.highlight_thickness;
       o_redisplay = True;
    }

    newsize = newfont || newthickness;

    if (newsize) RefigureDependentInfo(widget);

    if (CK(wordwrap)) o_redisplay = True;

    CP(wordwrap);

    if (data->hasfocus && XtSensitive(widget) && CK(blinkrate)) {
        if (newdata->blinkrate == 0) {
            data->blinkstate = on;
            if (data->timerid) {
                XtRemoveTimeOut(data->timerid);
                data->timerid = NULL;
            }
        } else if (data->timerid == NULL) {
            data->timerid =
                XtAppAddTimeOut(XtWidgetToApplicationContext(widget),
                                (unsigned long) newdata->blinkrate,
                                (XtTimerCallbackProc) HandleTimer,
                                (caddr_t) widget);
        }
    }
    CP(blinkrate);

    CP(resizewidth);
    CP(resizeheight);
    CP(cursor_position_visible);

    if (needgcs) {
        LoadGCs(widget, background, foreground);
        MakeCursors(widget);
        _XmTextAdjustGC(widget);
        o_redisplay = True;
    }

    if (newsize && XtIsRealized((Widget)widget))
        XClearArea(XtDisplay(widget), XtWindow(widget), 0, 0, 0, 0, TRUE);

    if (newdata->rows <= 0) {
        _XmWarning(widget, "Invalid rows, must be > 0");
        newdata->rows = data->rows;
    }

    if (newdata->columns <= 0) {
        _XmWarning(widget, "Invalid columns, must be > 0");
        newdata->columns = data->columns;
    }

    if (CK(columns) || CK(rows) || newsize ) {
        Dimension width, height;
        CP(columns);
        CP(rows);
        SizeFromRowsCols(widget, &width, &height);
        widget->core.width = width;
        widget->core.height = height;
        o_redisplay = True;
    }

    if (data->hasfocus) {
        newval = CKSensitive(args, num_args, widget);
        if (newval != XtSensitive(widget)) {
            widget->core.sensitive = newval;
            _XmTextChangeBlinkBehavior((XmTextWidget)widget->core.self, newval);
        }
    }

    return (o_redisplay);
}


static void NotifyResized(widget, o_create)
XmTextWidget widget;
Boolean o_create;
{
    OutputData data = widget->text.output->data;
    Boolean resizewidth = data->resizewidth;
    Boolean resizeheight = data->resizeheight;
    XmTextPosition linestart = 0;
    XmTextPosition position;
    int text_width = 0;
    int new_width;
    static XmTextBlockRec block;
    Arg args[1];

    _XmTextAdjustGC(widget);

    widget->text.in_resize = TRUE;
    data->resizewidth = data->resizeheight = FALSE;
    data->number_lines = widget->text.inner_widget->core.height -
                     data->topmargin - data->bottommargin;
    if (data->number_lines < (int) data->lineheight) data->number_lines = 1;
    else data->number_lines /= data->lineheight;

    if (data->vbar)
    {
        if (data->number_lines > 1)
           XtSetArg(args[0], XmNpageIncrement, data->number_lines - 1);
        else
           XtSetArg(args[0], XmNpageIncrement, 1);
        XtSetValues(data->vbar, args, 1);
    }

    if (XtIsRealized((Widget) widget)) {
        XClearWindow(XtDisplay(widget), XtWindow(widget->text.inner_widget));
        data->onoroff = off;
    }


    if (!o_create)              /* FALSE only if called from OutputCreate */
        _XmTextInvalidate(widget, (XmTextPosition) 0, (XmTextPosition) 0,
                          NODELTA);

    /* move the text is the new size grew enough to include new text */
    new_width = widget->core.width - (data->leftmargin + data->rightmargin);

    if (widget->text.edit_mode == XmSINGLE_LINE_EDIT) {
       position = XmTextGetLastPosition((Widget)widget);
       while (linestart < position) {
          linestart = (*widget->text.source->ReadSource) (widget->text.source,
			      		          linestart, position, &block);
          text_width += FindWidth(data, 0, &block, 0, block.length);
       }
       if (widget->text.input->data->has_destination &&
          widget->text.dest_position != widget->text.cursor_position)
          _XmTextClearDestination(widget, False);
       if (text_width - new_width < data->hoffset)
          if (text_width - new_width >= 0)
            ChangeHOffset(widget, text_width - new_width, True);
          else
	    ChangeHOffset(widget, 0, True);

       if (widget->text.auto_show_cursor_position &&
           text_width - new_width > data->hoffset)
          ChangeHOffset(widget, text_width - new_width, True);
       if (widget->text.input->data->has_destination &&
           widget->text.dest_position != widget->text.cursor_position)
          _XmTextDrawDestination(widget);
    } else RedisplayHBar(widget);

    data->resizewidth = resizewidth;
    data->resizeheight = resizeheight;
    _XmTextDrawShadow(widget);
    widget->text.in_resize = FALSE;

}


/* ARGSUSED */
static XtTimerCallbackProc HandleTimer(closure, id)
caddr_t closure;
XtIntervalId id;
{
    XmTextWidget widget = (XmTextWidget) closure;
    OutputData data = widget->text.output->data;
    if (data->blinkrate != 0)
        data->timerid = XtAppAddTimeOut(XtWidgetToApplicationContext(widget),
                                        (unsigned long)data->blinkrate,
                                        (XtTimerCallbackProc) HandleTimer,
                                        (caddr_t) closure);
    if (data->hasfocus && XtSensitive(widget)) BlinkInsertionPoint(widget);
}



#ifdef _NO_PROTO
void _XmTextChangeBlinkBehavior(widget, newvalue)
XmTextWidget widget;
Boolean newvalue;
#else /* _NO_PROTO */
void _XmTextChangeBlinkBehavior(XmTextWidget widget, Boolean newvalue)
#endif /* _NO_PROTO */
{
    OutputData data = widget->text.output->data;
    if (newvalue) {
        if (data->blinkrate != 0 && data->timerid == NULL)
            data->timerid =
                XtAppAddTimeOut(XtWidgetToApplicationContext(widget),
                                        (unsigned long)data->blinkrate,
                                        (XtTimerCallbackProc) HandleTimer,
                                        (caddr_t) widget);
        data->blinkstate = on;
    } else {
        if (data->timerid)
            XtRemoveTimeOut(data->timerid);
        data->timerid = NULL;
    }

   if (data->onoroff == on)
      PaintCursor(widget);
}

/* ARGSUSED */
static void HandleFocusEvents(w, closure, event)
Widget w;
Opaque closure;
XEvent *event;
{
    XmTextWidget widget = (XmTextWidget) w;
    OutputData data = widget->text.output->data;
    Boolean newhasfocus = data->hasfocus;
    XmAnyCallbackStruct cb;
    switch (event->type) {
      case FocusIn:
        if (event->xfocus.send_event && !(newhasfocus)) {
            cb.reason = XmCR_FOCUS;
            cb.event = event;
            XtCallCallbackList ((Widget) widget, widget->text.focus_callback, 
				(Opaque) &cb);
            newhasfocus = TRUE;
        }
        break;
      case FocusOut:
        if (event->xfocus.send_event && newhasfocus) {
            newhasfocus = FALSE;
            XmTextSetAddMode(w, False);
        }
        break;
      case EnterNotify:
        if ((_XmGetFocusPolicy(w) != XmEXPLICIT) && !(newhasfocus) &&
	    event->xcrossing.focus) {
            cb.reason = XmCR_FOCUS;
            cb.event = event;
            XtCallCallbackList ((Widget) widget, widget->text.focus_callback, (Opaque) &cb);
            newhasfocus = TRUE;
        }
        break;
      case LeaveNotify:
        if ((_XmGetFocusPolicy(w) != XmEXPLICIT) && newhasfocus) {
            newhasfocus = FALSE;
            XmTextSetAddMode(w, False);
        }
        break;
    }
    if (newhasfocus != data->hasfocus) {
       data->hasfocus = newhasfocus;
       if (newhasfocus && widget->text.input->data->has_destination &&
	   widget->text.dest_position == widget->text.cursor_position)
          _XmTextClearDestination(widget, False);
       if (XtSensitive(widget))
          _XmTextChangeBlinkBehavior(widget, newhasfocus);
       if (!newhasfocus && widget->text.input->data->has_destination)
          _XmTextDrawDestination(widget);
    }
}



/* ARGSUSED */
static void HandleGraphicsExposure(w, closure, event)
Widget w;
Opaque closure;
XEvent *event;
{
    XmTextWidget widget = (XmTextWidget) w;
    OutputData data = widget->text.output->data;
    if (event->xany.type == GraphicsExpose) {
        XGraphicsExposeEvent *xe = (XGraphicsExposeEvent *) event;
        if (data->exposehscroll != 0) {
            xe->x = 0;
            xe->width = widget->core.width;
        }
        if (data->exposevscroll != 0) {
            xe->y = 0;
            xe->height = widget->core.height;
        }
        RedrawRegion(widget, xe->x, xe->y, xe->width, xe->height);
        if (xe->count == 0) {
            if (data->exposehscroll) data->exposehscroll--;
            if (data->exposevscroll) data->exposevscroll--;
        }
    }
    if (event->xany.type == NoExpose) {
        if (data->exposehscroll) data->exposehscroll--;
        if (data->exposevscroll) data->exposevscroll--;
    }
}


static void OutputRealize(widget, valueMask, attributes)
XmTextWidget widget;
Mask *valueMask;
XSetWindowAttributes *attributes;
{
    XtCreateWindow((Widget) widget, (unsigned int) InputOutput,
                   (Visual *) CopyFromParent, *valueMask, attributes);
    MakeCursors(widget);
    _XmTextAdjustGC(widget);
}


static void OutputDestroy(widget)
XmTextWidget widget;
{
    OutputData data = widget->text.output->data;
    Cardinal depth;

    if (data->timerid)
        XtRemoveTimeOut(data->timerid);

    XtRemoveEventHandler((Widget) widget->text.inner_widget,
                     (EventMask)FocusChangeMask|EnterWindowMask|LeaveWindowMask,
                      FALSE, HandleFocusEvents, (Opaque)NULL);

    XtRemoveEventHandler((Widget) widget->text.inner_widget,
                      (EventMask) 0, TRUE, HandleGraphicsExposure,
                      (Opaque)NULL);

    XmDestroyPixmap(XtScreen(widget), data->stipple_tile);

    if (data->has_rect) {
      TextGCData gc_data = GetTextGCData((Widget)widget);
      gc_data->tw = NULL;
    }

    depth = widget->core.depth;
    widget->core.depth = 1;
    XtReleaseGC(widget, data->imagegc);
    widget->core.depth = depth;

    XtReleaseGC(widget, data->gc);

    XmFontListFree((XmFontList)data->fontlist);

    if (data->dest_cursor != XmUNSPECIFIED_PIXMAP)
       (void) XmDestroyPixmap(XtScreen(widget), data->dest_cursor);

    if (data->add_mode_cursor != XmUNSPECIFIED_PIXMAP)
       (void) XmDestroyPixmap(XtScreen(widget), data->add_mode_cursor);

    if (data->cursor != XmUNSPECIFIED_PIXMAP)
       (void) XmDestroyPixmap(XtScreen(widget), data->cursor);

    XtFree((char *)data);
    XtFree((char *)widget->text.output);
    posToXYCachedWidget = NULL;
}


static void RedrawRegion(widget, x, y, width, height)
XmTextWidget widget;
int x, y, width, height;
{
    OutputData data = widget->text.output->data;
    int i;
    XmTextPosition first, last;
    for (i = y ; i < y + height + data->lineheight ; i += data->lineheight) {
       first = XYToPos(widget, x, i);
       last = XYToPos(widget, x + width, i);
       first = (*widget->text.source->Scan)(widget->text.source, first, XmSELECT_POSITION,
                             XmsdLeft, 1, TRUE);
       last = (*widget->text.source->Scan)(widget->text.source, last, XmSELECT_POSITION,
                              XmsdRight, 1, TRUE);
       _XmTextMarkRedraw(widget, first, last);
    }
} 


static void OutputExpose(widget, event)
XmTextWidget widget;
XEvent *event;
{
    XExposeEvent *xe = (XExposeEvent *) event;
    OutputData data = widget->text.output->data;
    Boolean erased_cursor = False;
    int old_number_lines = data->number_lines;
    Boolean changed_visible = False;
    Arg args[1];

    if (event->xany.type != Expose)
        return;
    
    if (data->dest_visible) {
       _XmTextDestinationVisible((Widget) widget, False);
       changed_visible = True;
    }

    if (XtSensitive(widget) && data->hasfocus)
          _XmTextChangeBlinkBehavior(widget, False);

    if (widget->text.input->data->has_destination) {
       XClearArea(XtDisplay(widget), XtWindow(widget->text.inner_widget),
                  xe->x - CARET_WIDTH, xe->y - CARET_HEIGHT,
                  xe->width + (2 * CARET_WIDTH),
                  xe->height + (2 * CARET_HEIGHT), FALSE);
       data->dest_on = False;
       erased_cursor = True;
    }

    data->number_lines = widget->text.inner_widget->core.height -
                     data->topmargin - data->bottommargin;
    if (data->number_lines < (int) data->lineheight) data->number_lines = 1;
    else data->number_lines /= data->lineheight;

    if (data->vbar && old_number_lines != data->number_lines)
    {
        if (data->number_lines > 1)
           XtSetArg(args[0], XmNpageIncrement, data->number_lines - 1);
        else
           XtSetArg(args[0], XmNpageIncrement, 1);
        XtSetValues(data->vbar, args, 1);
    }

    if (!data->handlingexposures) {
        _XmTextDisableRedisplay(widget, FALSE);
        data->handlingexposures = TRUE;
        ClearCursor(widget);    /* %%% needs reviewing */
        data->putback = NULL;
    }
    if (data->exposehscroll != 0) {
        xe->x = 0;
        xe->width = widget->core.width;
    }
    if (data->exposevscroll != 0) {
        xe->y = 0;
        xe->height = widget->core.height;
    }
    if (xe->x == 0 && xe->y == 0 && xe->width == widget->core.width &&
          xe->height == widget->core.height)
        _XmTextMarkRedraw(widget, (XmTextPosition)0, 9999999);
    else {
        if (!erased_cursor)
           RedrawRegion(widget, xe->x, xe->y, xe->width, xe->height);
        else
           RedrawRegion(widget, xe->x - CARET_WIDTH, xe->y - CARET_HEIGHT,
			xe->width + (2 * CARET_WIDTH),
			xe->height + (2 * CARET_HEIGHT));
    }

    _XmTextInvalidate(widget, (XmTextPosition) widget->text.top_character,
		      (XmTextPosition) widget->text.cursor_position, NODELTA);

    _XmTextEnableRedisplay(widget);

    if (widget->text.input->data->has_destination)
       if (erased_cursor && (!data->hasfocus ||
           widget->text.dest_position != widget->text.cursor_position))
          _XmTextDrawDestination(widget);

    data->handlingexposures = FALSE;

    _XmTextDrawShadow(widget);

    if (changed_visible)
       _XmTextDestinationVisible((Widget) widget, True);
    if (XtSensitive(widget) && data->hasfocus)
       _XmTextChangeBlinkBehavior(widget, True);
}


static void GetPreferredSize(widget, width, height)
Widget widget;
Dimension *width, *height;
{
    SizeFromRowsCols((XmTextWidget) widget, width, height);
}


/* ARGSUSED */
static void HandleVBar(w, param, cback)
Widget w;
Opaque param, cback;
{
    XmScrollBarCallbackStruct *info = (XmScrollBarCallbackStruct *) cback;
    Widget parent = XtParent(w);
    Widget widget;
    XmTextWidget tw;
    Arg args[1];
    OutputData data;
    int lines = 0;

    XtSetArg(args[0], XmNworkWindow, &widget);
    XtGetValues(parent, args, 1);
    tw = (XmTextWidget) widget;
    data = tw->text.output->data;
    if (data->ignorevbar) return;
    tw->text.vsbar_scrolling = True;
    lines = info->value - tw->text.top_line;
    tw->text.top_line = info->value;
    XmTextScroll((Widget)tw, lines);
    tw->text.vsbar_scrolling = False;
}


/* ARGSUSED */
static void HandleHBar(w, param, cback)
Widget w;
Opaque param, cback;
{
    XmScrollBarCallbackStruct *info = (XmScrollBarCallbackStruct *) cback;
    Widget parent = XtParent(w);
    Widget widget;
    XmTextWidget tw;
    Arg args[1];
    OutputData data;
    int newhoffset;

    XtSetArg(args[0], XmNworkWindow, &widget);
    XtGetValues(parent, args, 1);
    tw = (XmTextWidget) widget;
    data = tw->text.output->data;
    newhoffset = data->hoffset;
    if (data->ignorehbar) return;
    switch (info->reason) {
      case XmCR_INCREMENT:
        newhoffset += data->averagecharwidth;
        break;
      case XmCR_DECREMENT:
        newhoffset -= data->averagecharwidth;
        break;
      case XmCR_PAGE_INCREMENT:
        newhoffset += ((int) tw->text.inner_widget->core.width);
        break;
      case XmCR_PAGE_DECREMENT:
        newhoffset -= ((int) tw->text.inner_widget->core.width);
        break;
      case XmCR_DRAG:
      case XmCR_VALUE_CHANGED:
        newhoffset = info->value;
        break;
      case XmCR_TO_TOP:
        newhoffset += info->pixel;
        break;
      case XmCR_TO_BOTTOM:
        newhoffset -= info->pixel;
        break;
    }
    if (tw->text.input->data->has_destination &&
        tw->text.dest_position != tw->text.cursor_position)
       _XmTextClearDestination(tw, False);

    ChangeHOffset(tw, newhoffset, False);

    if (tw->text.input->data->has_destination &&
        tw->text.dest_position != tw->text.cursor_position)
       _XmTextDrawDestination(tw);
}


/* Public routines. */

#ifdef _NO_PROTO
void _XmTextOutputCreate(widget, args, num_args)
XmTextWidget widget;
ArgList args;
Cardinal num_args;
#else /* _NO_PROTO */
void _XmTextOutputCreate(XmTextWidget widget, ArgList args, Cardinal num_args)
#endif /* _NO_PROTO */
{
    Output output;
    OutputData data;
    Dimension width, height;

    widget->text.output = output = (Output)
        XtMalloc((unsigned) sizeof(OutputRec));
    output->data = data = (OutputData)
        XtMalloc((unsigned) sizeof(OutputDataRec));

    XtGetSubresources((Widget) widget->core.parent, (caddr_t)data,
                      widget->core.name, "XmText", output_resources,
                      XtNumber(output_resources), args, num_args);
    data->widget = widget;

    output->XYToPos = (XYToPosProc)XYToPos;
    output->PosToXY = PosToXY;
    output->MeasureLine = MeasureLine;
    output->Draw = Draw;
    output->DrawInsertionPoint = DrawInsertionPoint;
    output->MakePositionVisible = MakePositionVisible;
    output->MoveLines = MoveLines;
    output->Invalidate = OutputInvalidate;
    output->GetPreferredSize = GetPreferredSize;
    output->GetValues = OutputGetValues;
    output->SetValues = OutputSetValues;
    output->realize = OutputRealize;
    output->destroy = OutputDestroy;
    output->resize = (XmTextResizeProc)NotifyResized;
    output->expose = OutputExpose;

    data->insertx = data->inserty = -99;
    data->hoffset = 0;
    data->scrollwidth = 1;
    data->exposehscroll = data->exposevscroll = FALSE;
    data->putback = NULL;
    data->stipple_tile = XmGetPixmap(XtScreen(widget), "50_foreground",
				     widget->primitive.foreground ^
				     widget->core.background_pixel, 0);
    data->dest_cursor = XmUNSPECIFIED_PIXMAP;
    data->add_mode_cursor = XmUNSPECIFIED_PIXMAP;
    data->cursor = XmUNSPECIFIED_PIXMAP;
    data->dest_on = False;
    data->dest_visible = True;
    data->timerid = NULL;
    data->font = NULL;
    /* copy over the font list */
    if (data->fontlist == NULL)
        data->fontlist = _XmGetDefaultFontList(widget, XmTEXT_FONTLIST);
    data->fontlist = (XmFontList)XmFontListCopy(data->fontlist);
    LoadFontMetrics(widget);
    widget->text.inner_widget = (Widget) widget;
    data->leftmargin = data->rightmargin = widget->text.margin_width +
					  widget->primitive.shadow_thickness +
					  widget->primitive.highlight_thickness;
    data->topmargin = data->bottommargin = widget->text.margin_height +
					  widget->primitive.shadow_thickness +
					  widget->primitive.highlight_thickness;

  /* Don't word wrap, have multiple row or have vertical scrollbars
     if editMode is single_line */
     if (widget->text.edit_mode == XmSINGLE_LINE_EDIT) 
        data->rows = 1;

  /* Don't grow in width if word wrap is on */
    if (widget->text.edit_mode != XmSINGLE_LINE_EDIT &&
        data->wordwrap) 
       data->resizewidth = FALSE;

    if (data->rows <= 0) {
        _XmWarning(widget, "Invalid rows, must be > 0");
        data->rows = 1;
    }

    if (data->columns <= 0) {
        _XmWarning(widget, "Invalid columns, must be > 0");
        data->columns = 20;
    }

    SizeFromRowsCols(widget, &width, &height);
    if (widget->core.width == 0)
        widget->core.width = width;
    if (widget->core.height == 0)
        widget->core.height = height;
    data->prevW = widget->core.width;
    data->prevH = widget->core.height;
    data->minwidth = widget->core.width;
    data->minheight = widget->core.height;

    data->imagegc = NULL;
    data->gc = NULL;

    LoadGCs(widget, widget->core.background_pixel,
                         widget->primitive.foreground);

    data->number_lines = widget->text.inner_widget->core.height -
                     data->topmargin - data->bottommargin;
    if (data->number_lines < (int) data->lineheight) data->number_lines = 1;
    else data->number_lines /= data->lineheight;

    RefigureDependentInfo(widget);

  /* Only create the scrollbars if text is a child of scrolled window */

   if ((XtClass(widget->core.parent) == xmScrolledWindowWidgetClass) &&
      (((XmScrolledWindowWidget)widget->core.parent)->swindow.VisualPolicy ==
				XmVARIABLE))
      
   {
    int n;
        static XtCallbackRec hcallback[2] = {HandleHBar, NULL};
        static XtCallbackRec vcallback[2] = {HandleVBar, NULL};
        static Arg arglist[30];
        static Arg swarglist[] = {
            {XmNhorizontalScrollBar, (XtArgVal) NULL},
            {XmNverticalScrollBar, (XtArgVal) NULL},
            {XmNworkWindow, (XtArgVal) NULL},
            {XmNscrollBarPlacement, (XtArgVal) NULL},
      };

      if (data->scrollhorizontal) {
          data->resizewidth = FALSE;
          data->ignorehbar = FALSE;

          n = 0;
          XtSetArg(arglist[n], XmNorientation, (XtArgVal) XmHORIZONTAL); n++;
          XtSetArg(arglist[n], XmNvalueChangedCallback,
                        (XtArgVal) hcallback); n++;
          XtSetArg(arglist[n], XmNincrementCallback, (XtArgVal) hcallback); n++;
          XtSetArg(arglist[n], XmNdecrementCallback, (XtArgVal) hcallback); n++;
          XtSetArg(arglist[n], XmNpageIncrementCallback,
                        (XtArgVal) hcallback); n++;
          XtSetArg(arglist[n], XmNpageDecrementCallback,
                        (XtArgVal) hcallback); n++;
          XtSetArg(arglist[n], XmNtoTopCallback, (XtArgVal) hcallback); n++;
          XtSetArg(arglist[n], XmNtoBottomCallback, (XtArgVal) hcallback); n++;
          XtSetArg(arglist[n], XmNdragCallback, (XtArgVal) hcallback); n++;
          XtSetArg(arglist[n], XmNshadowThickness,
                        (XtArgVal) widget->primitive.shadow_thickness); n++;
          XtSetArg(arglist[n], XmNforeground,
                        (XtArgVal) widget->primitive.foreground); n++;
          XtSetArg(arglist[n], XmNbackground,
                        (XtArgVal) widget->core.background_pixel); n++;
          XtSetArg(arglist[n], XmNbackgroundPixmap,
                        (XtArgVal) widget->core.background_pixmap); n++;
          XtSetArg(arglist[n], XmNtopShadowColor,
                        (XtArgVal) widget->primitive.top_shadow_color); n++;
          XtSetArg(arglist[n], XmNtopShadowPixmap,
                        (XtArgVal) widget->primitive.top_shadow_pixmap); n++;
          XtSetArg(arglist[n], XmNbottomShadowColor,
                        (XtArgVal) widget->primitive.bottom_shadow_color); n++;
          XtSetArg(arglist[n], XmNbottomShadowPixmap,
                        (XtArgVal) widget->primitive.bottom_shadow_pixmap); n++;
          XtSetArg(arglist[n], XmNminimum, 0); n++;
          XtSetArg(arglist[n], XmNmaximum, 1); n++;
          XtSetArg(arglist[n], XmNvalue, 0); n++;
          XtSetArg(arglist[n], XmNsliderSize, 1); n++;
          XtSetArg(arglist[n], XmNpageIncrement, 
                   widget->text.inner_widget->core.width); n++;
          XtSetArg(arglist[n], XmNincrement, data->averagecharwidth); n++;
          XtSetArg(arglist[n], XmNtraversalOn, False); n++;
/*
 *   Highlight thickness shouldn't be passed onto scrollbars during create.     PIR2328
 *   This also caused a bug in which the addtional space required for the       PIR2328
 *   highlight was factored into the text widget's core.height .   This is      PIR2328
 *   a no no especially if the text widget is given an initial size in either   PIR2328
 *   row or column units.   The extra space was folded into this setting        PIR2328
 *   which made it larger than necessary.   The addtional space always turned   PIR2328
 *   out to be highlight_thickness * 4.                                         PIR2328
 *                                                                              PIR2328
 *   This may not be the right way to fix the bug, but this is such God         PIR2328
 *   awful code that it is impossible to determine where anything gets done     PIR2328
 *                                                                              PIR2328
 *        XtSetArg(arglist[n], XmNhighlightThickness,                           PIR2328
 *                      (XtArgVal) widget->primitive.highlight_thickness); n++; PIR2328
 */
          XtSetArg(arglist[n], XmNhighlightThickness, 0); n++;               /* PIR2328 */

          data->hbar = (Widget) XmCreateScrollBar(XtParent(widget), "hbar",
                                              arglist, n);
          XtManageChild(data->hbar);
      } else data->hbar = NULL;

      if (data->scrollvertical) {
          data->resizeheight = FALSE;
          data->ignorevbar = FALSE;

          n = 0;
          XtSetArg(arglist[n], XmNorientation, (XtArgVal) XmVERTICAL); n++;
          XtSetArg(arglist[n], XmNvalueChangedCallback,
                        (XtArgVal) vcallback); n++;
          XtSetArg(arglist[n], XmNincrementCallback, (XtArgVal) vcallback); n++;
          XtSetArg(arglist[n], XmNdecrementCallback, (XtArgVal) vcallback); n++;
          XtSetArg(arglist[n], XmNpageIncrementCallback,
                        (XtArgVal) vcallback); n++;
          XtSetArg(arglist[n], XmNpageDecrementCallback,
                        (XtArgVal) vcallback); n++;
          XtSetArg(arglist[n], XmNtoTopCallback, (XtArgVal) vcallback); n++;
          XtSetArg(arglist[n], XmNtoBottomCallback, (XtArgVal) vcallback); n++;
          XtSetArg(arglist[n], XmNdragCallback, (XtArgVal) vcallback); n++;
          XtSetArg(arglist[n], XmNshadowThickness,
                        (XtArgVal) widget->primitive.shadow_thickness); n++;
          XtSetArg(arglist[n], XmNforeground,
                        (XtArgVal) widget->primitive.foreground); n++;
          XtSetArg(arglist[n], XmNbackground,
                        (XtArgVal) widget->core.background_pixel); n++;
          XtSetArg(arglist[n], XmNbackgroundPixmap,
                        (XtArgVal) widget->core.background_pixmap); n++;
          XtSetArg(arglist[n], XmNtopShadowColor,
                        (XtArgVal) widget->primitive.top_shadow_color); n++;
          XtSetArg(arglist[n], XmNtopShadowPixmap,
                        (XtArgVal) widget->primitive.top_shadow_pixmap); n++;
          XtSetArg(arglist[n], XmNbottomShadowColor,
                        (XtArgVal) widget->primitive.bottom_shadow_color); n++;
          XtSetArg(arglist[n], XmNbottomShadowPixmap,
                        (XtArgVal) widget->primitive.bottom_shadow_pixmap); n++;
          XtSetArg(arglist[n], XmNminimum, 0); n++;
          XtSetArg(arglist[n], XmNmaximum, 5); n++;
          XtSetArg(arglist[n], XmNvalue, 0); n++;
          XtSetArg(arglist[n], XmNsliderSize, 5); n++;
          XtSetArg(arglist[n], XmNtraversalOn, False); n++;
/*
 *   Highlight thickness shouldn't be passed onto scrollbars during create.     PIR2328
 *   This also caused a bug in which the addtional space required for the       PIR2328
 *   highlight was factored into the text widget's core.height .   This is      PIR2328
 *   a no no especially if the text widget is given an initial size in either   PIR2328
 *   row or column units.   The extra space was folded into this setting        PIR2328
 *   which made it larger than necessary.   The addtional space always turned   PIR2328
 *   out to be highlight_thickness * 4.                                         PIR2328
 *                                                                              PIR2328
 *   This may not be the right way to fix the bug, but this is such God         PIR2328
 *   awful code that it is impossible to determine where anything gets done     PIR2328
 *                                                                              PIR2328
 *         XtSetArg(arglist[n], XmNhighlightThickness,                          PIR2328
 *                     (XtArgVal) widget->primitive.highlight_thickness); n++;  PIR2328
 */
          XtSetArg(arglist[n], XmNhighlightThickness, 0); n++;               /* PIR2328 */

          if (data->number_lines > 1) {
            XtSetArg(arglist[n], XmNpageIncrement, data->number_lines - 1); n++;
          } else {
            XtSetArg(arglist[n], XmNpageIncrement, 1); n++;
          }
          data->vbar = (Widget) XmCreateScrollBar(XtParent(widget), "vbar",
                                              arglist, n);
	  if (widget->text.edit_mode != XmSINGLE_LINE_EDIT) 
	    XtManageChild(data->vbar);
      } else data->vbar = NULL;

      swarglist[0].value = (XtArgVal)data->hbar;
      swarglist[1].value = (XtArgVal)data->vbar;
      swarglist[2].value = (XtArgVal)widget;

    /* Tell scrolled window parent where to put the scrollbars */

        if (data->scrollleftside) {
           if (data->scrolltopside)
              swarglist[3].value = (XtArgVal) XmTOP_LEFT;
           else
              swarglist[3].value = (XtArgVal) XmBOTTOM_LEFT;
        } else {
           if (data->scrolltopside)
              swarglist[3].value = (XtArgVal) XmTOP_RIGHT;
           else
              swarglist[3].value = (XtArgVal) XmBOTTOM_RIGHT;
        }
        XtSetValues(widget->core.parent, swarglist, XtNumber(swarglist));

    } else {
        data->vbar = NULL;
        data->hbar = NULL;
        if ((XtClass(widget->core.parent) == xmScrolledWindowWidgetClass) &&
           (((XmScrolledWindowWidget)widget->core.parent)->swindow.VisualPolicy
					 == XmCONSTANT)) {
           data->scrollhorizontal = FALSE;
           data->scrollvertical = FALSE;
           data->resizewidth = TRUE;
           data->resizeheight = TRUE;
        }
    }

    data->hasfocus = FALSE;
    data->has_rect = FALSE;
    data->onoroff = off;
    data->handlingexposures = FALSE;
    XtAddEventHandler((Widget) widget->text.inner_widget,
                     (EventMask)FocusChangeMask|EnterWindowMask|LeaveWindowMask,
                      FALSE, HandleFocusEvents, (Opaque)NULL);
    XtAddEventHandler((Widget) widget->text.inner_widget,
                      (EventMask) 0, TRUE, HandleGraphicsExposure,
                      (Opaque)NULL);
}

#ifdef _NO_PROTO
int _XmTextGetBaseLine(widget)
XmTextWidget widget;
#else /* _NO_PROTO */
int _XmTextGetBaseLine(XmTextWidget widget)
#endif /* _NO_PROTO */
{
   OutputData data = widget->text.output->data;

   return(data->topmargin + data->font->ascent);
}

#ifdef _NO_PROTO
void _XmTextChangeHOffset(widget, length)
XmTextWidget widget;
int length;
#else /* _NO_PROTO */
void _XmTextChangeHOffset(XmTextWidget widget, int length)
#endif /* _NO_PROTO */
{
   OutputData data = widget->text.output->data;
   Dimension margin_width = widget->text.margin_width +
                           widget->primitive.shadow_thickness +
                           widget->primitive.highlight_thickness;
   int new_offset = data->hoffset;
   XmTextPosition nextpos;
   XmTextPosition last_position;
   XmTextPosition temp;
   int inner_width, width, i;
   int text_width = 0;
   int new_text_width = 0;
   XmTextBlockRec block;

   if (widget->text.input->data->has_destination &&
       widget->text.dest_position != widget->text.cursor_position)
      _XmTextClearDestination(widget, False);
   new_offset += length;

   for (i = 0 ; i < widget->text.number_lines ; i++) {
      last_position = (*widget->text.source->Scan) (widget->text.source,
                                                    widget->text.line[i].start,
                                                    XmSELECT_LINE,
                                                    XmsdRight, 1, FALSE);
      nextpos = (*widget->text.source->Scan)(widget->text.source,
                                              last_position, XmSELECT_LINE,
                                              XmsdRight, 1, TRUE);
      if (nextpos == last_position)
          nextpos = PASTENDPOS;
      width = 0;
      temp = widget->text.line[i].start;
      while (temp < last_position) {
            temp = (*widget->text.source->ReadSource)
		        (widget->text.source, temp, last_position, &block);
            width += FindWidth(data, (Position) width, &block,
                                0, block.length);
      }
      new_text_width = width;
      if (new_text_width > text_width) text_width = new_text_width;
   }

   inner_width = widget->core.width - (2 * margin_width);
   if (new_offset >= text_width - inner_width)
       new_offset = text_width - inner_width;

   ChangeHOffset(widget, new_offset, True);
   if (widget->text.input->data->has_destination &&
       widget->text.dest_position != widget->text.cursor_position)
      _XmTextDrawDestination(widget);
}
