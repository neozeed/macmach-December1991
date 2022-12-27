#include "copyright.h"

/* $XConsortium: XSetClMask.c,v 11.8 88/09/06 16:11:33 jim Exp $ */
/* Copyright    Massachusetts Institute of Technology    1986	*/

#include "Xlibint.h"

XSetClipMask (dpy, gc, mask)
register Display *dpy;
GC gc;
Pixmap mask;
{
    LockDisplay(dpy);
    /* always update, since client may have changed pixmap contents */
    gc->values.clip_mask = mask;
    gc->dirty |= GCClipMask;
    gc->rects = 0;
    _XFlushGCCache(dpy, gc);
    UnlockDisplay(dpy);
    SyncHandle();
}
