#include "copyright.h"

/* $XConsortium: XGetGeom.c,v 11.15 88/09/06 16:07:49 jim Exp $ */
/* Copyright    Massachusetts Institute of Technology    1986	*/

#define NEED_REPLIES
#include "Xlibint.h"

Status XGetGeometry (dpy, d, root, x, y, width, height, borderWidth, depth)
    register Display *dpy;
    Drawable d;
    Window *root; /* RETURN */
    int *x, *y;  /* RETURN */
    unsigned int *width, *height, *borderWidth, *depth;  /* RETURN */
{
    xGetGeometryReply rep;
    register xResourceReq *req;
    LockDisplay(dpy);
    GetResReq(GetGeometry, d, req);
    if (!_XReply (dpy, (xReply *)&rep, 0, xTrue)) {
	UnlockDisplay(dpy);
	SyncHandle();
	return (0);
	}
    *root = rep.root;
    *x = cvtINT16toInt (rep.x);
    *y = cvtINT16toInt (rep.y);
    *width = rep.width;
    *height = rep.height;
    *borderWidth = rep.borderWidth;
    *depth = rep.depth;
    UnlockDisplay(dpy);
    SyncHandle();
    return (1);
}

