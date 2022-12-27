#include "copyright.h"

/* $XConsortium: XLiICmaps.c,v 11.16 89/11/08 17:07:16 converse Exp $ */
/* Copyright    Massachusetts Institute of Technology    1986	*/

#define NEED_REPLIES
#include "Xlibint.h"

Colormap *XListInstalledColormaps(dpy, win, n)
register Display *dpy;
Window win;
int *n;  /* RETURN */
{
    long nbytes;
    Colormap *cmaps;
    xListInstalledColormapsReply rep;
    register xResourceReq *req;

    LockDisplay(dpy);
    GetResReq(ListInstalledColormaps, win, req);

    if(_XReply(dpy, (xReply *) &rep, 0, xFalse) == 0) {
	    UnlockDisplay(dpy);
	    SyncHandle();
	    *n = 0;
	    return((Colormap *) NULL);
	}

    if (rep.nColormaps) {
	nbytes = rep.nColormaps * sizeof(Colormap);
	cmaps = (Colormap *) Xmalloc((unsigned) nbytes);
	nbytes = rep.nColormaps << 2;
	if (! cmaps) {
	    _XEatData(dpy, (unsigned long) nbytes);
	    UnlockDisplay(dpy);
	    SyncHandle();
	    return((Colormap *) NULL);
	}
	_XRead32 (dpy, (char *) cmaps, nbytes);
    }
    else cmaps = (Colormap *) NULL;
    
    *n = rep.nColormaps;
    UnlockDisplay(dpy);
    SyncHandle();
    return(cmaps);
}

