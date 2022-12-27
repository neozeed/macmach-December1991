#include "copyright.h"

/* $XConsortium: XFillArcs.c,v 11.13 89/05/26 18:27:59 rws Exp $ */
/* Copyright    Massachusetts Institute of Technology    1986	*/

#include "Xlibint.h"

#define arc_scale (SIZEOF(xArc) / 4)

XFillArcs(dpy, d, gc, arcs, n_arcs)
register Display *dpy;
Drawable d;
GC gc;
XArc *arcs;
int n_arcs;
{
    register xPolyFillArcReq *req;
    long len;
    int n;

    LockDisplay(dpy);
    FlushGC(dpy, gc);
    GetReq(PolyFillArc, req);
    while (n_arcs) {
	req->drawable = d;
	req->gc = gc->gid;
	n = n_arcs;
	len = ((long)n) * arc_scale;
	if (len > (dpy->max_request_size - req->length)) {
	    n = (dpy->max_request_size - req->length) / arc_scale;
	    len = ((long)n) * arc_scale;
	}
	req->length += len;
	len <<= 2; /* watch out for macros... */
	Data16 (dpy, (short *) arcs, len);
	n_arcs -= n;
	arcs += n;
    }
    UnlockDisplay(dpy);
    SyncHandle();
}
