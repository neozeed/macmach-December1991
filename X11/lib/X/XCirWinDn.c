#include "copyright.h"

/* $XConsortium: XCirWinDn.c,v 11.7 88/09/06 16:04:43 jim Exp $ */
/* Copyright    Massachusetts Institute of Technology    1986	*/

#include "Xlibint.h"

XCirculateSubwindowsDown(dpy,w)
    register Display *dpy;
    Window w;
{
    register xCirculateWindowReq *req;

    LockDisplay(dpy);
    GetReq(CirculateWindow, req);
    req->window = w;
    req->direction = LowerHighest;
    UnlockDisplay(dpy);
    SyncHandle();
}

