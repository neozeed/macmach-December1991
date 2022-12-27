#include "copyright.h"

/* $XConsortium: XFreeCmap.c,v 11.6 88/09/06 16:07:26 jim Exp $ */
/* Copyright    Massachusetts Institute of Technology    1986	*/

#include "Xlibint.h"

XFreeColormap(dpy, cmap)
register Display *dpy;
Colormap cmap;
{
    register xResourceReq *req;

    LockDisplay(dpy);
    GetResReq(FreeColormap, cmap, req);
    UnlockDisplay(dpy);
    SyncHandle();
}
