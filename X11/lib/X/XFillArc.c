#include "copyright.h"

/* $XConsortium: XFillArc.c,v 11.12 88/09/06 16:07:06 jim Exp $ */
/* Copyright    Massachusetts Institute of Technology    1986	*/

#include "Xlibint.h"

/* precompute the maximum size of batching request allowed */

static int size = SIZEOF(xPolyFillArcReq) + EPERBATCH * SIZEOF(xArc);

XFillArc(dpy, d, gc, x, y, width, height, angle1, angle2)
    register Display *dpy;
    Drawable d;
    GC gc;
    int x, y; /* INT16 */
    unsigned int width, height; /* CARD16 */
    int angle1, angle2; /* INT16 */
{
    xArc *arc;
#ifdef MUSTCOPY
    xArc arcdata;
    long len = SIZEOF(xArc);

    arc = &arcdata;
#endif /* MUSTCOPY */

    LockDisplay(dpy);
    FlushGC(dpy, gc);

    {
    register xPolyFillArcReq *req = (xPolyFillArcReq *) dpy->last_req;

    /* if same as previous request, with same drawable, batch requests */
    if (
          (req->reqType == X_PolyFillArc)
       && (req->drawable == d)
       && (req->gc == gc->gid)
       && ((dpy->bufptr + SIZEOF(xArc)) <= dpy->bufmax)
       && (((char *)dpy->bufptr - (char *)req) < size) ) {
	 req->length += SIZEOF(xArc) >> 2;
#ifndef MUSTCOPY
         arc = (xArc *) dpy->bufptr;
	 dpy->bufptr += SIZEOF(xArc);
#endif /* not MUSTCOPY */
	 }

    else {
	GetReqExtra(PolyFillArc, SIZEOF(xArc), req);

	req->drawable = d;
	req->gc = gc->gid;
#ifdef MUSTCOPY
	dpy->bufptr -= SIZEOF(xArc);
#else
	arc = (xArc *) NEXTPTR(req,xPolyFillArcReq);
#endif /* MUSTCOPY */
	}
    arc->x = x;
    arc->y = y;
    arc->width = width;
    arc->height = height;
    arc->angle1 = angle1;
    arc->angle2 = angle2;

#ifdef MUSTCOPY
    Data (dpy, (char *) arc, len);
#endif /* MUSTCOPY */

    }
    UnlockDisplay(dpy);
    SyncHandle();
}
