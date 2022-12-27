#include "copyright.h"

/* $XConsortium: XFreeCols.c,v 11.9 88/09/06 16:07:28 jim Exp $ */
/* Copyright    Massachusetts Institute of Technology    1986	*/

#include "Xlibint.h"

XFreeColors(dpy, cmap, pixels, npixels, planes)
register Display *dpy;
Colormap cmap;
unsigned long *pixels; /* LISTofCARD32 */
int npixels;
unsigned long planes; /* CARD32 */
{
    register xFreeColorsReq *req;
    register long nbytes;

    LockDisplay(dpy);
    GetReq(FreeColors, req);
    req->cmap = cmap;
    req->planeMask = planes;

    /* on the VAX, each pixel is a 32-bit (unsigned) integer */
    req->length += npixels;

    nbytes = npixels << 2;		/* watch out for macros... */
    Data32 (dpy, (long *) pixels, nbytes);
    UnlockDisplay(dpy);
    SyncHandle();
}

