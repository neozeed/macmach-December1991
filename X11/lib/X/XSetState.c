#include "copyright.h"

/* $XConsortium: XSetState.c,v 11.10 88/09/06 16:10:50 jim Exp $ */
/* Copyright    Massachusetts Institute of Technology    1986	*/

#include "Xlibint.h"

XSetState(dpy, gc, foreground, background, function, planemask)
register Display *dpy;
GC gc;
int function;
unsigned long planemask;
unsigned long foreground, background;
{
    XGCValues *gv = &gc->values;

    LockDisplay(dpy);

    if (function != gv->function) {
	gv->function = function;
	gc->dirty |= GCFunction;
    }
    if (planemask != gv->plane_mask) {
	gv->plane_mask = planemask;
	gc->dirty |= GCPlaneMask;
    }
    if (foreground != gv->foreground) {
	gv->foreground = foreground;
	gc->dirty |= GCForeground;
    }
    if (background != gv->background) {
	gv->background = background;
	gc->dirty |= GCBackground;
    }
    UnlockDisplay(dpy);
    SyncHandle();
}
