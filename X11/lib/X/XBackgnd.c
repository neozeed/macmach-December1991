#include "copyright.h"

/* $XConsortium: XBackgnd.c,v 11.7 88/09/06 16:04:08 jim Exp $ */
/* Copyright    Massachusetts Institute of Technology    1986	*/

#include "Xlibint.h"

XSetWindowBackground(dpy, w, pixel)
    register Display *dpy;
    Window w;
    unsigned long pixel;
{
    register xChangeWindowAttributesReq *req;

    LockDisplay(dpy);
    GetReqExtra (ChangeWindowAttributes, 4, req);
    req->window = w;
    req->valueMask = CWBackPixel;
    OneDataCard32 (dpy, NEXTPTR(req,xChangeWindowAttributesReq), pixel);
    UnlockDisplay(dpy);
    SyncHandle();
}

