#include "copyright.h"

/* $XConsortium: XConfWind.c,v 11.9 88/09/06 16:04:57 jim Exp $ */
/* Copyright    Massachusetts Institute of Technology    1986	*/

#include "Xlibint.h"

XMoveResizeWindow(dpy, w, x, y, width, height)
register Display *dpy;
Window w;
int x, y;
unsigned int width, height;
{
    register xConfigureWindowReq *req;

    LockDisplay(dpy);
    GetReqExtra(ConfigureWindow, 16, req);
    req->window = w;
    req->mask = CWX | CWY | CWWidth | CWHeight;
#ifdef MUSTCOPY
    {
	long lx = x, ly = y;
	unsigned long lwidth = width, lheight = height;

	dpy->bufptr -= 16;
	Data32 (dpy, (long *) &lx, 4);	/* order must match values of */
	Data32 (dpy, (long *) &ly, 4);	/* CWX, CWY, CWWidth, and CWHeight */
	Data32 (dpy, (long *) &lwidth, 4);
	Data32 (dpy, (long *) &lheight, 4);
    }
#else
    {
	register unsigned long *valuePtr =
	  (unsigned long *) NEXTPTR(req,xConfigureWindowReq);
	*valuePtr++ = x;
	*valuePtr++ = y;
	*valuePtr++ = width;
	*valuePtr   = height;
    }
#endif /* MUSTCOPY */
    UnlockDisplay(dpy);
    SyncHandle();
}
