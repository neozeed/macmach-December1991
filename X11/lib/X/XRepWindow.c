#include "copyright.h"

/* $XConsortium: XRepWindow.c,v 11.6 88/09/06 16:10:19 jim Exp $ */
/* Copyright    Massachusetts Institute of Technology    1986	*/

#include "Xlibint.h"

XReparentWindow(dpy, w, p, x, y)
    register Display *dpy;
    Window w, p;
    int x, y;
{
    register xReparentWindowReq *req;

    LockDisplay(dpy);
    GetReq(ReparentWindow, req);
    req->window = w;
    req->parent = p;
    req->x = x;
    req->y = y;
    UnlockDisplay(dpy);
    SyncHandle();
}

