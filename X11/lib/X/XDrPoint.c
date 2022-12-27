#include "copyright.h"

/* $XConsortium: XDrPoint.c,v 11.11 88/09/06 16:06:51 jim Exp $ */
/* Copyright    Massachusetts Institute of Technology    1986	*/

#include "Xlibint.h"

/* precompute the maximum size of batching request allowed */

static int size = SIZEOF(xPolyPointReq) + EPERBATCH * SIZEOF(xPoint);

XDrawPoint(dpy, d, gc, x, y)
    register Display *dpy;
    Drawable d;
    GC gc;
    int x, y; /* INT16 */
{
    xPoint *point;
#ifdef MUSTCOPY
    xPoint pointdata;
    long len = SIZEOF(xPoint);

    point = &pointdata;
#endif /* MUSTCOPY */

    LockDisplay(dpy);
    FlushGC(dpy, gc);

    {
    register xPolyPointReq *req = (xPolyPointReq *) dpy->last_req;


    /* if same as previous request, with same drawable, batch requests */
    if (
          (req->reqType == X_PolyPoint)
       && (req->drawable == d)
       && (req->gc == gc->gid)
       && (req->coordMode == CoordModeOrigin)
       && ((dpy->bufptr + SIZEOF(xPoint)) <= dpy->bufmax)
       && (((char *)dpy->bufptr - (char *)req) < size) ) {
	 req->length += SIZEOF(xPoint) >> 2;
#ifndef MUSTCOPY
         point = (xPoint *) dpy->bufptr;
	 dpy->bufptr += SIZEOF(xPoint);
#endif /* not MUSTCOPY */
	 }

    else {
	GetReqExtra(PolyPoint, 4, req); /* 1 point = 4 bytes */
	req->drawable = d;
	req->gc = gc->gid;
	req->coordMode = CoordModeOrigin;
#ifdef MUSTCOPY
	dpy->bufptr -= SIZEOF(xPoint);
#else
	point = (xPoint *) NEXTPTR(req,xPolyPointReq);
#endif /* MUSTCOPY */
	}

    point->x = x;
    point->y = y;

#ifdef MUSTCOPY
    Data (dpy, (char *) point, len);
#endif /* MUSTCOPY */
    }
    UnlockDisplay(dpy);
    SyncHandle();
}

