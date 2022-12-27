#include "copyright.h"

/* $XConsortium: XFillRect.c,v 11.12 88/09/06 16:07:12 jim Exp $ */
/* Copyright    Massachusetts Institute of Technology    1986	*/

#include "Xlibint.h"

/* precompute the maximum size of batching request allowed */

static int size = SIZEOF(xPolyFillRectangleReq) + EPERBATCH * SIZEOF(xRectangle);

XFillRectangle(dpy, d, gc, x, y, width, height)
    register Display *dpy;
    Drawable d;
    GC gc;
    int x, y; /* INT16 */
    unsigned int width, height; /* CARD16 */
{
    xRectangle *rect;
#ifdef MUSTCOPY
    xRectangle rectdata;
    long len = SIZEOF(xRectangle);

    rect = &rectdata;
#endif /* MUSTCOPY */

    LockDisplay(dpy);
    FlushGC(dpy, gc);

    {
    register xPolyFillRectangleReq *req 
		= (xPolyFillRectangleReq *) dpy->last_req;

    /* if same as previous request, with same drawable, batch requests */
    if (
          (req->reqType == X_PolyFillRectangle)
       && (req->drawable == d)
       && (req->gc == gc->gid)
       && ((dpy->bufptr + SIZEOF(xRectangle)) <= dpy->bufmax)
       && (((char *)dpy->bufptr - (char *)req) < size) ) {
	 req->length += SIZEOF(xRectangle) >> 2;
#ifndef MUSTCOPY
         rect = (xRectangle *) dpy->bufptr;
	 dpy->bufptr += SIZEOF(xRectangle);
#endif /* not MUSTCOPY */
	 }

    else {
	GetReqExtra(PolyFillRectangle, SIZEOF(xRectangle), req);
	req->drawable = d;
	req->gc = gc->gid;
#ifdef MUSTCOPY
	dpy->bufptr -= SIZEOF(xRectangle);
#else
	rect = (xRectangle *) NEXTPTR(req,xPolyFillRectangleReq);
#endif /* MUSTCOPY */
	}
    rect->x = x;
    rect->y = y;
    rect->width = width;
    rect->height = height;

#ifdef MUSTCOPY
    Data (dpy, (char *) rect, len);
#endif /* MUSTCOPY */
    }
    UnlockDisplay(dpy);
    SyncHandle();
}
