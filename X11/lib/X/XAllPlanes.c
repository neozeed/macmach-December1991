#include "copyright.h"

/* $XConsortium: XAllPlanes.c,v 11.16 89/12/09 19:14:39 rws Exp $ */
/* Copyright    Massachusetts Institute of Technology    1986	*/
#define NEED_REPLIES
#include "Xlibint.h"

Status XAllocColorPlanes(dpy, cmap, contig, pixels, ncolors, nreds, ngreens, 
                         nblues, rmask, gmask, bmask)
register Display *dpy;
Colormap cmap;
Bool contig;
unsigned long *pixels; /* LISTofCARD32 */ /* RETURN */
int ncolors;
int nreds, ngreens, nblues;
unsigned long *rmask, *gmask, *bmask; /* CARD32 */ /* RETURN */
{
    xAllocColorPlanesReply rep;
    Status status;
    register xAllocColorPlanesReq *req;

    LockDisplay(dpy);
    GetReq(AllocColorPlanes,req);

    req->cmap = cmap;
    req->colors = ncolors;
    req->red = nreds;
    req->green = ngreens;
    req->blue = nblues;
    req->contiguous = contig;

    status = _XReply(dpy, (xReply *)&rep, 0, xFalse);


    if (status) {
	*rmask = rep.redMask;
	*gmask = rep.greenMask;
	*bmask = rep.blueMask;

	/* sizeof(CARD32) = 4 */
	_XRead32 (dpy, (char *) pixels, (long)(ncolors * 4));
    }

    UnlockDisplay(dpy);
    SyncHandle();
    return(status);
}    
