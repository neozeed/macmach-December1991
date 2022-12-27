#include "copyright.h"

/* $XConsortium: XQuCurShp.c,v 11.10 88/09/06 16:09:55 jim Exp $ */
/* Copyright    Massachusetts Institute of Technology    1986	*/

#define NEED_REPLIES
#include "Xlibint.h"

Status XQueryBestCursor(dpy, drawable, width, height, ret_width, ret_height)
    register Display *dpy;
    Drawable drawable;
    unsigned int width, height;
    unsigned int *ret_width, *ret_height;
{       
    xQueryBestSizeReply rep;
    register xQueryBestSizeReq *req;

    LockDisplay(dpy);
    GetReq(QueryBestSize, req);
    req->class = CursorShape;
    req->drawable = drawable;
    req->width = width;
    req->height = height;
    if (_XReply (dpy, (xReply *)&rep, 0, xTrue) == 0) {
	UnlockDisplay(dpy);
	SyncHandle();
	return 0;
	}
    *ret_width = rep.width;
    *ret_height = rep.height;
    UnlockDisplay(dpy);
    SyncHandle();
    return 1;
}

