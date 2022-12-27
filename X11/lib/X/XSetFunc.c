#include "copyright.h"

/* $XConsortium: XSetFunc.c,v 11.8 88/09/06 16:10:34 jim Exp $ */
/* Copyright    Massachusetts Institute of Technology    1986	*/

#include "Xlibint.h"

XSetFunction (dpy, gc, function)
register Display *dpy;
GC gc;
int function;
{
    LockDisplay(dpy);
    if (gc->values.function != function) {
	gc->values.function = function;
	gc->dirty |= GCFunction;
    }
    UnlockDisplay(dpy);
    SyncHandle();
}
