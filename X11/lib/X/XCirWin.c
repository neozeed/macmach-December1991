#include "copyright.h"

/* $XConsortium: XCirWin.c,v 11.6 88/09/06 16:04:42 jim Exp $ */
/* Copyright    Massachusetts Institute of Technology    1986	*/

#include "Xlibint.h"

XCirculateSubwindows(dpy, w, direction)
    register Display *dpy;
    Window w;
    int direction;
{
    register xCirculateWindowReq *req;

    LockDisplay(dpy);
    GetReq(CirculateWindow, req);
    req->window = w;
    req->direction = direction;
    UnlockDisplay(dpy);
    SyncHandle();
}

