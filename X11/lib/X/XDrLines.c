#include "copyright.h"

/* $XConsortium: XDrLines.c,v 11.13 89/05/30 09:56:14 rws Exp $ */
/* Copyright    Massachusetts Institute of Technology    1986	*/

#include "Xlibint.h"

XDrawLines (dpy, d, gc, points, npoints, mode)
    register Display *dpy;
    Drawable d;
    GC gc;
    XPoint *points;
    int npoints;
    int mode;
{
    register xPolyLineReq *req;
    register long length;
    LockDisplay(dpy);
    FlushGC(dpy, gc);
    GetReq (PolyLine, req);
    req->drawable = d;
    req->gc = gc->gid;
    req->coordMode = mode;
    if ((req->length + npoints) > 65535)
	npoints = 65535 - req->length; /* force BadLength, if possible */
    req->length += npoints;
       /* each point is 2 16-bit integers */
    length = npoints << 2;		/* watch out for macros... */
    Data16 (dpy, (short *) points, length);
    UnlockDisplay(dpy);
    SyncHandle();
}

