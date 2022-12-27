#include "copyright.h"

/* $XConsortium: XSetFore.c,v 11.8 88/09/06 16:10:33 jim Exp $ */
/* Copyright    Massachusetts Institute of Technology    1986	*/

#include "Xlibint.h"

XSetForeground (dpy, gc, foreground)
register Display *dpy;
GC gc;
unsigned long foreground; /* CARD32 */
{
    LockDisplay(dpy);
    if (gc->values.foreground != foreground) {
	gc->values.foreground = foreground;
	gc->dirty |= GCForeground;
    }
    UnlockDisplay(dpy);
    SyncHandle();
}
