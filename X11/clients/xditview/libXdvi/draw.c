/*
 * draw.c
 *
 * accept dvi function calls and translate to X
 */

#include <X11/Xos.h>
#include <X11/IntrinsicP.h>
#include <X11/StringDefs.h>
#include <stdio.h>
#include <ctype.h>
#include "DviP.h"

HorizontalMove(dw, delta)
	DviWidget	dw;
	int		delta;
{
	dw->dvi.state->x += delta;
}

HorizontalGoto(dw, NewPosition)
	DviWidget	dw;
	int		NewPosition;
{
	dw->dvi.state->x = NewPosition;
}

VerticalMove(dw, delta)
	DviWidget	dw;
	int		delta;
{
	dw->dvi.state->y += delta;
}

VerticalGoto(dw, NewPosition)
	DviWidget	dw;
	int		NewPosition;
{
	dw->dvi.state->y = NewPosition;
}

FlushCharCache (dw)
	DviWidget	dw;
{
	if (dw->dvi.cache.char_index != 0)
	    XDrawText (XtDisplay (dw), XtWindow (dw), dw->dvi.normal_GC,
			dw->dvi.cache.start_x, dw->dvi.cache.start_y,
 			dw->dvi.cache.cache, dw->dvi.cache.index + 1);
	dw->dvi.cache.index = 0;
	dw->dvi.cache.max = DVI_TEXT_CACHE_SIZE;
	if (dw->dvi.noPolyText)
	    dw->dvi.cache.max = 1;
	dw->dvi.cache.char_index = 0;
	dw->dvi.cache.cache[0].nchars = 0;
	dw->dvi.cache.start_x = dw->dvi.cache.x = dw->dvi.state->x;
	dw->dvi.cache.start_y = dw->dvi.cache.y = dw->dvi.state->y;
}

ClearPage (dw)
	DviWidget	dw;
{
	XClearWindow (XtDisplay (dw), XtWindow (dw));
}

static void setGC (dw)
	DviWidget	dw;
{
	if (dw->dvi.state->line_style != dw->dvi.line_style ||
	    dw->dvi.state->line_width != dw->dvi.line_width)
	{
		XSetLineAttributes (XtDisplay (dw), dw->dvi.normal_GC,
				    dw->dvi.state->line_width,
				    LineSolid,
				    CapButt,
				    JoinMiter
				    );
		dw->dvi.line_style = dw->dvi.state->line_style;
		dw->dvi.line_width = dw->dvi.state->line_width;
	}
}

DrawLine (dw, x, y)
	DviWidget	dw;
	int		x, y;
{
	XDrawLine (XtDisplay (dw), XtWindow (dw), dw->dvi.normal_GC,
		dw->dvi.state->x, dw->dvi.state->y, x, y);
	dw->dvi.state->x = x;
	dw->dvi.state->y = y;
}

DrawCircle (dw, radius)
	DviWidget	dw;
	int		radius;
{
}

DrawEllipse (dw, a, b)
	DviWidget	dw;
	int		a, b;
{
}

DrawArc (dw, x0, y0, x1, y1)
	DviWidget	dw;
	int		x0, y0, x1, y1;
{
}

DrawSpline (dw, s, len)
	DviWidget	dw;
	char		*s;
	int		len;
{
}
