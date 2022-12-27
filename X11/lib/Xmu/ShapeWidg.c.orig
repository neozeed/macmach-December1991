static char rcsid[] =
	"$XConsortium: ShapeWidg.c,v 1.2 89/10/08 13:42:38 rws Exp $";

/* 
 * Copyright 1988 by the Massachusetts Institute of Technology
 *
 * Permission to use, copy, modify, and distribute this software and its
 * documentation for any purpose and without fee is hereby granted, provided 
 * that the above copyright notice appear in all copies and that both that 
 * copyright notice and this permission notice appear in supporting 
 * documentation, and that the name of M.I.T. not be used in advertising
 * or publicity pertaining to distribution of the software without specific, 
 * written prior permission. M.I.T. makes no representations about the 
 * suitability of this software for any purpose.  It is provided "as is"
 * without express or implied warranty.
 *
 */

#include	<X11/IntrinsicP.h>
#include	<X11/extensions/shape.h>
#include	"Xmu.h"

static ShapeError();
static  ShapeRectangle(), ShapeOval(), ShapeEllipseOrRoundedRectangle();

Boolean XmuReshapeWidget(w, shape_style, corner_width, corner_height)
    Widget w;
    int shape_style;
    int corner_width, corner_height;
{
    switch (shape_style) {

      case XmuShapeRectangle:
	ShapeRectangle(w);
	break;

      case XmuShapeOval:
	ShapeOval(w);
	break;

      case XmuShapeEllipse:
      case XmuShapeRoundedRectangle:
	ShapeEllipseOrRoundedRectangle
	    (w,
	     ((shape_style == XmuShapeEllipse) ? True : False),
	     corner_width,
	     corner_height);
	break;

      default:
	ShapeError(w);
	return False;
    }
    return True;
}

static ShapeError(w)
    Widget w;
{
    String params[1];
    Cardinal num_params = 1;
    params[0] = XtName(w);
    XtAppWarningMsg( XtWidgetToApplicationContext(w),
		     "shapeUnknown", "xmuReshapeWidget", "XmuLibrary",
		     "Unsupported shape style for Command widget \"%s\"",
		     params, &num_params
		   );
}


static ShapeRectangle(w)
    Widget w;
{
    XShapeCombineMask( XtDisplay(w), XtWindow(w),
		       ShapeBounding, 0, 0, None, ShapeSet );
    XShapeCombineMask( XtDisplay(w), XtWindow(w),
		       ShapeClip, 0, 0, None, ShapeSet );
}


static ShapeOval(w)
    Widget w;
{
    Display *dpy = XtDisplay(w);
    unsigned width = w->core.width + (w->core.border_width<<1);
    unsigned height = w->core.height + (w->core.border_width<<1);
    Pixmap p = XCreatePixmap( dpy, XtWindow(w), width, height, 1 );
    XGCValues values;
    GC gc;
    unsigned int diam, x2, y2;

    values.foreground = 0;
    values.background = 1;
    values.cap_style = CapRound;
    if (width < height) {
	diam = width;
	x2 = diam>>1;
	y2 = height - x2 - 1;	/* can't explain the off-by-one */
    } else {
	diam = height;
	y2 = diam>>1;
	x2 = width - y2 - 1;
    }
    values.line_width = diam;
    gc = XCreateGC (dpy, p,
		    GCForeground | GCBackground | GCLineWidth | GCCapStyle,
		    &values);
    XFillRectangle( dpy, p, gc, 0, 0, width, height );
    XSetForeground( dpy, gc, 1 );
    XDrawLine( dpy, p, gc, diam>>1, diam>>1, x2, y2 );
    XShapeCombineMask( dpy, XtWindow(w), ShapeBounding, 
		       -(w->core.border_width), -(w->core.border_width),
		       p, ShapeSet );
    XSetForeground( dpy, gc, 0 );
    XFillRectangle( dpy, p, gc, 0, 0, width, height );
    if (w->core.width < w->core.height) {
	diam = w->core.width;
	x2 = diam>>1;
	y2 = w->core.height - x2 - 1;
    } else {
	diam = w->core.height;
	y2 = diam>>1;
	x2 = w->core.width - y2 - 1;
    }
    values.line_width = diam;
    values.foreground = 1;
    XChangeGC (dpy, gc, GCLineWidth|GCForeground, &values);
    XDrawLine( dpy, p, gc, diam>>1, diam>>1, x2, y2 );
    XShapeCombineMask( dpy, XtWindow(w), ShapeClip, 0, 0, p, ShapeSet );
    XFreePixmap( dpy, p );
    XFreeGC (dpy, gc );
}


static ShapeEllipseOrRoundedRectangle(w, ellipse, ew, eh)
    Widget w;
    Boolean ellipse;
    int ew, eh;
{
    Display *dpy = XtDisplay(w);
    unsigned width = w->core.width + (w->core.border_width<<1);
    unsigned height = w->core.height + (w->core.border_width<<1);
    Pixmap p = XCreatePixmap( dpy, XtWindow(w), width, height, 1 );
    XGCValues values;
    GC gc;

    values.foreground = 0;
    gc = XCreateGC (dpy, p, GCForeground, &values );
    XFillRectangle( dpy, p, gc, 0, 0, width, height );
    XSetForeground (dpy, gc, 1);
    if (!ellipse)
	XmuFillRoundedRectangle( dpy, p, gc, 0, 0, width, height, ew, eh );
    else
	XFillArc( dpy, p, gc, 0, 0, width, height, 0, 360*64 );
    XShapeCombineMask( dpy, XtWindow(w), ShapeBounding, 
		       -(w->core.border_width), -(w->core.border_width),
		       p, ShapeSet );
    XSetForeground (dpy, gc, 0);
    XFillRectangle( dpy, p, gc, 0, 0, width, height );
    XSetForeground (dpy, gc, 1);
    if (!ellipse)
	XmuFillRoundedRectangle( dpy, p, gc, 0, 0,
				 w->core.width, w->core.height,
				 ew, eh );
    else
	XFillArc( dpy, p, gc, 0, 0, w->core.width, w->core.height, 0, 360*64 );
    XShapeCombineMask( dpy, XtWindow(w), ShapeClip, 0, 0, p, ShapeSet );
    XFreePixmap( dpy, p );
    XFreeGC (dpy, gc);
}
