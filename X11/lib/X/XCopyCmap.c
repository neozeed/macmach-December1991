#include "copyright.h"

/* $XConsortium: XCopyCmap.c,v 11.6 88/09/06 16:05:22 jim Exp $ */
/* Copyright    Massachusetts Institute of Technology    1986	*/

#include "Xlibint.h"

Colormap XCopyColormapAndFree(dpy, src_cmap)
register Display *dpy;
Colormap src_cmap;
{
    Colormap mid;
    register xCopyColormapAndFreeReq *req;

    LockDisplay(dpy);
    GetReq(CopyColormapAndFree, req);

    mid = req->mid = XAllocID(dpy);
    req->srcCmap = src_cmap;
    UnlockDisplay(dpy);
    SyncHandle();
    return(mid);
}
