#include "copyright.h"

/* $XConsortium: XFlush.c,v 11.7 89/06/23 13:32:48 jim Exp $ */
/* Copyright    Massachusetts Institute of Technology    1986	*/

#include "Xlibint.h"

/* Flush all buffered output requests. */
/* NOTE: NOT necessary when calling any of the Xlib routines. */

XFlush (dpy)
    register Display *dpy;
    {
    LockDisplay(dpy);
    _XFlush (dpy);
    UnlockDisplay(dpy);
    }
