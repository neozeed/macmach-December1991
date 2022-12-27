/*
 * $XConsortium: CrPixFBit.c,v 1.2 89/12/12 18:43:12 jim Exp $
 *
 * Copyright 1988 Massachusetts Institute of Technology
 *
 * This file contains miscellaneous utility routines and is not part of the
 * Xlib standard.
 *
 * Public entry points:
 *
 *     XmuCreatePixmapFromBitmap	make a pixmap from a bitmap
 */

#include <X11/copyright.h>

#include <X11/Xos.h>
#include <X11/Xlib.h>

Pixmap XmuCreatePixmapFromBitmap (dpy, d, bitmap, width, height, depth,
				  fore, back)
    Display *dpy;			/* connection to X server */
    Drawable d;				/* drawable indicating screen */
    Pixmap bitmap;			/* single plane pixmap */
    unsigned int width, height;		/* dimensions of bitmap and pixmap */
    unsigned int depth;			/* depth of pixmap to create */
    unsigned long fore, back;		/* colors to use */
{
    Pixmap pixmap;

    pixmap = XCreatePixmap (dpy, d, width, height, depth);
    if (pixmap != None) {
	GC gc;
	XGCValues xgcv;

	xgcv.foreground = fore;
	xgcv.background = back;
	xgcv.graphics_exposures = False;

	gc = XCreateGC (dpy, d,
			(GCForeground | GCBackground | GCGraphicsExposures),
			&xgcv);
	if (gc) {
	    XCopyPlane (dpy, bitmap, pixmap, gc, 0, 0, width, height, 0, 0, 1);
	    XFreeGC (dpy, gc);
	} else {
	    XFreePixmap (dpy, pixmap);
	    pixmap = None;
	}
    }
    return pixmap;
}
