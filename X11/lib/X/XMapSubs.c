#include "copyright.h"

/* $XConsortium: XMapSubs.c,v 11.5 88/09/06 16:09:14 jim Exp $ */
/* Copyright    Massachusetts Institute of Technology    1986	*/

#include "Xlibint.h"

XMapSubwindows(dpy, win)
    register Display *dpy;
    Window win;
{
    register xResourceReq *req;
    LockDisplay(dpy);
    GetResReq(MapSubwindows, win, req);
    UnlockDisplay(dpy);
    SyncHandle();
}
