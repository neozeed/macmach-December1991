#include "copyright.h"

/* $XConsortium: XSetSSaver.c,v 11.6 88/09/06 16:10:46 jim Exp $ */
/* Copyright    Massachusetts Institute of Technology    1986	*/

#include "Xlibint.h"

XSetScreenSaver(dpy, timeout, interval, prefer_blank, allow_exp)
    register Display *dpy;
    int timeout, interval, prefer_blank, allow_exp;

{
    register xSetScreenSaverReq *req;

    LockDisplay(dpy);
    GetReq(SetScreenSaver, req);
    req->timeout = timeout;
    req->interval = interval;
    req->preferBlank = prefer_blank;
    req->allowExpose = allow_exp;
    UnlockDisplay(dpy);
    SyncHandle();
}

