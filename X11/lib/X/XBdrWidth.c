#include "copyright.h"

/* $XConsortium: XBdrWidth.c,v 11.7 88/09/06 16:04:10 jim Exp $ */
/* Copyright    Massachusetts Institute of Technology    1987 */

#include "Xlibint.h"

XSetWindowBorderWidth(dpy, w, width)
Display *dpy;
Window w;
unsigned int width;
{
    unsigned long lwidth = width;	/* must be CARD32 */

    register xConfigureWindowReq *req;
    LockDisplay(dpy);
    GetReqExtra(ConfigureWindow, 4, req);
    req->window = w;
    req->mask = CWBorderWidth;
    OneDataCard32 (dpy, NEXTPTR(req,xConfigureWindowReq), lwidth);
    UnlockDisplay(dpy);
    SyncHandle();
}

