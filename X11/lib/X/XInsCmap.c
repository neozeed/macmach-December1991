#include "copyright.h"

/* $XConsortium: XInsCmap.c,v 11.6 88/09/06 16:08:46 jim Exp $ */
/* Copyright    Massachusetts Institute of Technology    1986	*/

#include "Xlibint.h"

XInstallColormap(dpy, cmap)
register Display *dpy;
Colormap cmap;
{
    register xResourceReq *req;
    LockDisplay(dpy);
    GetResReq(InstallColormap, cmap, req);
    UnlockDisplay(dpy);
    SyncHandle();
}

