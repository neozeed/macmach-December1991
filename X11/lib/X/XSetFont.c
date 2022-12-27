#include "copyright.h"

/* $XConsortium: XSetFont.c,v 11.8 88/09/06 16:10:31 jim Exp $ */
/* Copyright    Massachusetts Institute of Technology    1986	*/

#include "Xlibint.h"

XSetFont (dpy, gc, font)
register Display *dpy;
GC gc;
Font font;
{
    LockDisplay(dpy);
    if (gc->values.font != font) {
        gc->values.font = font;
	gc->dirty |= GCFont;
	_XFlushGCCache(dpy, gc);
    }
    UnlockDisplay(dpy);
    SyncHandle();
}
