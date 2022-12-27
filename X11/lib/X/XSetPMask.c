#include "copyright.h"

/* $XConsortium: XSetPMask.c,v 11.8 88/09/06 16:10:40 jim Exp $ */
/* Copyright    Massachusetts Institute of Technology    1986	*/

#include "Xlibint.h"

XSetPlaneMask (dpy, gc, planemask)
register Display *dpy;
GC gc;
unsigned long planemask; /* CARD32 */
{
    LockDisplay(dpy);
    if (gc->values.plane_mask != planemask) {
	gc->values.plane_mask = planemask;
	gc->dirty |= GCPlaneMask;
    }
    UnlockDisplay(dpy);	
    SyncHandle();
}
