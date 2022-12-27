#include "copyright.h"

/* $XConsortium: XUnmapSubs.c,v 11.7 88/09/06 16:11:17 jim Exp $ */
/* Copyright    Massachusetts Institute of Technology    1986	*/

#include "Xlibint.h"

XUnmapSubwindows(dpy, win)
register Display *dpy;
Window win;
{
    register xResourceReq *req;

    LockDisplay(dpy);
    GetResReq(UnmapSubwindows,win, req);
    UnlockDisplay(dpy);
    SyncHandle();
}
