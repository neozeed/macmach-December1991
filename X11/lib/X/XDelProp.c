#include "copyright.h"

/* $XConsortium: XDelProp.c,v 11.5 88/09/06 16:06:18 jim Exp $ */
/* Copyright    Massachusetts Institute of Technology    1986	*/

#include "Xlibint.h"

XDeleteProperty(dpy, window, property)
register Display *dpy;
Window window;
Atom property;
{
    register xDeletePropertyReq *req;

    LockDisplay(dpy);
    GetReq(DeleteProperty, req);
    req->window = window;
    req->property = property;
    UnlockDisplay(dpy);
    SyncHandle();
}
