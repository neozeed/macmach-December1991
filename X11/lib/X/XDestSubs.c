#include "copyright.h"

/* $XConsortium: XDestSubs.c,v 11.5 88/09/06 16:06:23 jim Exp $ */
/* Copyright    Massachusetts Institute of Technology    1986	*/

#include "Xlibint.h"

XDestroySubwindows(dpy, win)
register Display *dpy;
Window win;
{
    register xResourceReq *req;

    LockDisplay(dpy);
    GetResReq (DestroySubwindows,win, req);
    UnlockDisplay(dpy);
    SyncHandle();
}

