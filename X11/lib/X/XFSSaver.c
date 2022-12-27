#include "copyright.h"

/* $XConsortium: XFSSaver.c,v 1.4 88/09/06 16:07:02 jim Exp $ */
/* Copyright    Massachusetts Institute of Technology    1987	*/

#include "Xlibint.h"

XActivateScreenSaver(dpy) 
    register Display *dpy;

{
    XForceScreenSaver (dpy, ScreenSaverActive);
}

XResetScreenSaver(dpy) 
    register Display *dpy;

{
    XForceScreenSaver (dpy, ScreenSaverReset);
}

XForceScreenSaver(dpy, mode)
    register Display *dpy; 
    int mode;

{
    register xForceScreenSaverReq *req;

    LockDisplay(dpy);
    GetReq(ForceScreenSaver, req);
    req->mode = mode;
    UnlockDisplay(dpy);
    SyncHandle();
}

