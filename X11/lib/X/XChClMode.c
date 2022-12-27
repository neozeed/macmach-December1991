#include "copyright.h"

/* $XConsortium: XChClMode.c,v 11.5 88/09/06 16:04:23 jim Exp $ */
/* Copyright    Massachusetts Institute of Technology    1986	*/

#include "Xlibint.h"


XSetCloseDownMode(dpy, mode)
    register Display *dpy; 
    int mode;

{
    register xSetCloseDownModeReq *req;

    LockDisplay(dpy);
    GetReq(SetCloseDownMode, req);
    req->mode = mode;
    UnlockDisplay(dpy);
    SyncHandle();
}

