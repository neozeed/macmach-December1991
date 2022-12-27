#include "copyright.h"

/* $XConsortium: XChAccCon.c,v 11.8 88/09/06 16:04:19 jim Exp $ */
/* Copyright    Massachusetts Institute of Technology    1986	*/

#include "Xlibint.h"

XEnableAccessControl(dpy) 
    register Display *dpy;

{
    XSetAccessControl(dpy, EnableAccess);
}

XDisableAccessControl(dpy) 
    register Display *dpy;

{
    XSetAccessControl(dpy, DisableAccess);
}

XSetAccessControl(dpy, mode)
    register Display *dpy; 
    int mode;

{
    register xSetAccessControlReq *req;

    LockDisplay(dpy);
    GetReq(SetAccessControl, req);
    req->mode = mode;
    UnlockDisplay(dpy);
    SyncHandle();
}

