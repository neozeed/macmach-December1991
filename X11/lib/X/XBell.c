#include "copyright.h"

/* $XConsortium: XBell.c,v 11.7 88/09/06 16:04:12 jim Exp $ */
/* Copyright    Massachusetts Institute of Technology    1986	*/

#include "Xlibint.h"

XBell(dpy, percent)
    register Display *dpy;
    int percent;
{
    register xBellReq *req;

    LockDisplay(dpy);
    GetReq(Bell,req);
    req->percent = percent;
    UnlockDisplay(dpy);
    SyncHandle();
}

