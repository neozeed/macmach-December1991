#include "copyright.h"

/* $XConsortium: XAllowEv.c,v 1.12 88/09/06 16:04:05 jim Exp $ */
/* Copyright    Massachusetts Institute of Technology    1986	*/

#include "Xlibint.h"

XAllowEvents(dpy, mode, time)
    register Display *dpy;
    int mode;
    Time time;

{
    register xAllowEventsReq *req;

    LockDisplay(dpy);
    GetReq(AllowEvents,req);
    req->mode = mode;
    req->time = time;
    UnlockDisplay(dpy);
    SyncHandle();
}



