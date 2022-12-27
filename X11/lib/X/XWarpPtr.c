#include "copyright.h"

/* $XConsortium: XWarpPtr.c,v 11.6 88/09/06 16:11:23 jim Exp $ */
/* Copyright    Massachusetts Institute of Technology    1986	*/

#include "Xlibint.h"

XWarpPointer(dpy, src_win, dest_win, src_x, src_y, src_width, src_height,
	     dest_x, dest_y)
     register Display *dpy;
     Window src_win, dest_win;
     int src_x, src_y;
     unsigned int src_width, src_height;
     int dest_x, dest_y;
{       
    register xWarpPointerReq *req;

    LockDisplay(dpy);
    GetReq(WarpPointer, req);
    req->srcWid = src_win;
    req->dstWid = dest_win;
    req->srcX = src_x;
    req->srcY = src_y;
    req->srcWidth = src_width;
    req->srcHeight = src_height;
    req->dstX = dest_x;
    req->dstY = dest_y;
    UnlockDisplay(dpy);
    SyncHandle();
}

