#include "copyright.h"

/* $XConsortium: XSetClOrig.c,v 11.12 88/09/06 16:10:27 jim Exp $ */
/* Copyright    Massachusetts Institute of Technology    1986	*/

#include "Xlibint.h"

XSetClipOrigin (dpy, gc, xorig, yorig)
register Display *dpy;
GC gc;
int xorig, yorig;
{
    XGCValues *gv = &gc->values;

    LockDisplay(dpy);
    if (xorig != gv->clip_x_origin) {
        gv->clip_x_origin = xorig;
	gc->dirty |= GCClipXOrigin;
    }
    if (yorig != gv->clip_y_origin) {
        gv->clip_y_origin = yorig;
	gc->dirty |= GCClipYOrigin;
    }
    UnlockDisplay(dpy);
    SyncHandle();
}
