#include "copyright.h"

/* $XConsortium: XPmapBgnd.c,v 11.7 88/09/06 16:09:38 jim Exp $ */
/* Copyright    Massachusetts Institute of Technology    1986	*/

#include "Xlibint.h"

XSetWindowBackgroundPixmap(dpy, w, pixmap)
    register Display *dpy;
    Window w;
    Pixmap pixmap;
{
    register xChangeWindowAttributesReq *req;
    LockDisplay (dpy);
    GetReqExtra (ChangeWindowAttributes, 4, req);
    req->window = w;
    req->valueMask = CWBackPixmap;
    OneDataCard32 (dpy, NEXTPTR(req,xChangeWindowAttributesReq), pixmap);
    UnlockDisplay (dpy);
    SyncHandle();
}

