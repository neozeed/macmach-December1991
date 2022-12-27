#include "copyright.h"

/* $XConsortium: XSetIFocus.c,v 11.8 88/09/06 16:10:37 jim Exp $ */
/* Copyright    Massachusetts Institute of Technology    1986	*/

#include "Xlibint.h"

XSetInputFocus(dpy, focus, revert_to, time)
    register Display *dpy;
    Window focus;
    int revert_to;
    Time time;
{       
    register xSetInputFocusReq *req;

    LockDisplay(dpy);
    GetReq(SetInputFocus, req);
    req->focus = focus;
    req->revertTo = revert_to;
    req->time = time;
    UnlockDisplay(dpy);
    SyncHandle();
}

