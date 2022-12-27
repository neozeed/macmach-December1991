#include "copyright.h"

/* $XConsortium: XFillPoly.c,v 11.11 89/05/30 09:58:49 rws Exp $ */
/* Copyright    Massachusetts Institute of Technology    1986	*/

#include "Xlibint.h"

XFillPolygon(dpy, d, gc, points, n_points, shape, mode)
register Display *dpy;
Drawable d;
GC gc;
XPoint *points;
int n_points;
int shape;
int mode;
{
    register xFillPolyReq *req;
    register long nbytes;

    LockDisplay(dpy);
    FlushGC(dpy, gc);
    GetReq(FillPoly, req);

    req->drawable = d;
    req->gc = gc->gid;
    req->shape = shape;
    req->coordMode = mode;

    if ((req->length + n_points) > 65535)
	n_points = 65535 - req->length; /* force BadLength, if possible */
    req->length += n_points;

    /* shift (mult. by 4) before passing to the (possible) macro */

    nbytes = n_points << 2;
    
    Data16 (dpy, (short *) points, nbytes);
    UnlockDisplay(dpy);
    SyncHandle();
}
