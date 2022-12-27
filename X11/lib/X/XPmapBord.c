#include "copyright.h"

/* $XConsortium: XPmapBord.c,v 11.7 88/09/06 16:09:42 jim Exp $ */
/* Copyright    Massachusetts Institute of Technology    1986	*/

#include "Xlibint.h"

XSetWindowBorderPixmap(dpy, w, pixmap)
    register Display *dpy;
    Window w;
    Pixmap pixmap;
{
    register xChangeWindowAttributesReq *req;
    LockDisplay(dpy);
    GetReqExtra (ChangeWindowAttributes, 4, req);
    req->window = w;
    req->valueMask = CWBorderPixmap;
    OneDataCard32 (dpy, NEXTPTR(req,xChangeWindowAttributesReq), pixmap);
    UnlockDisplay(dpy);
    SyncHandle();
}

