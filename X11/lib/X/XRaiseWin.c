#include "copyright.h"

/* $XConsortium: XRaiseWin.c,v 11.8 88/09/06 16:10:09 jim Exp $ */
/* Copyright    Massachusetts Institute of Technology    1986	*/

#include "Xlibint.h"

XRaiseWindow (dpy, w)
    register Display *dpy;
    Window w;
{
    register xConfigureWindowReq *req;
    unsigned long val = Above;		/* needed for macro below */

    LockDisplay(dpy);
    GetReqExtra(ConfigureWindow, 4, req);
    req->window = w;
    req->mask = CWStackMode;
    OneDataCard32 (dpy, NEXTPTR(req,xConfigureWindowReq), val);
    UnlockDisplay(dpy);
    SyncHandle();
}

