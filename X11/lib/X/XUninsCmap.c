#include "copyright.h"

/* $XConsortium: XUninsCmap.c,v 11.6 88/09/06 16:11:14 jim Exp $ */
/* Copyright    Massachusetts Institute of Technology    1986	*/

#include "Xlibint.h"

XUninstallColormap(dpy, cmap)
register Display *dpy;
Colormap cmap;
{
    register xResourceReq *req;

    LockDisplay(dpy);
    GetResReq(UninstallColormap, cmap, req);
    UnlockDisplay(dpy);
    SyncHandle();
}
