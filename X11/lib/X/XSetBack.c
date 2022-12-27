#include "copyright.h"

/* $XConsortium: XSetBack.c,v 11.8 88/09/06 16:10:25 jim Exp $ */
/* Copyright    Massachusetts Institute of Technology    1986	*/

#include "Xlibint.h"

XSetBackground (dpy, gc, background)
register Display *dpy;
GC gc;
unsigned long background; /* CARD32 */
{
    LockDisplay(dpy);
    if (gc->values.background != background) {
	gc->values.background = background;
	gc->dirty |= GCBackground;
    }
    UnlockDisplay(dpy);
    SyncHandle();
}
