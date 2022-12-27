#include "copyright.h"

/* $XConsortium: XClearArea.c,v 11.8 88/09/06 16:04:52 jim Exp $ */
/* Copyright    Massachusetts Institute of Technology    1986	*/

#include "Xlibint.h"

XClearArea (dpy, w, x, y, width, height, exposures)
    register Display *dpy;
    Window w;
    int x, y;
    unsigned int width, height;
    Bool exposures;
{
    register xClearAreaReq *req;

    LockDisplay(dpy);
    GetReq(ClearArea, req);
    req->window = w;
    req->x = x;
    req->y = y;
    req->width = width;
    req->height = height;
    req->exposures = exposures;
    UnlockDisplay(dpy);
    SyncHandle();
}

