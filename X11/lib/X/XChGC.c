#include "copyright.h"

/* $XConsortium: XChGC.c,v 11.9 88/09/06 16:04:28 jim Exp $ */
/* Copyright    Massachusetts Institute of Technology    1986	*/

#include "Xlibint.h"

XChangeGC (dpy, gc, valuemask, values)
    register Display *dpy;
    GC gc;
    unsigned long valuemask;
    XGCValues *values;
{
    LockDisplay(dpy);

    valuemask &= (1L << (GCLastBit + 1)) - 1;
    if (valuemask) _XUpdateGCCache (gc, valuemask, values);

    /* if any Resource ID changed, must flush */
    if (gc->dirty & (GCFont | GCTile | GCStipple))
	_XFlushGCCache(dpy, gc);
    UnlockDisplay(dpy);
    SyncHandle();
}

