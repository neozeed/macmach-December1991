#include "copyright.h"

/* $XConsortium: XMoveWin.c,v 11.9 88/09/06 16:10:38 jim Exp $ */
/* Copyright    Massachusetts Institute of Technology    1986	*/

#include "Xlibint.h"

XMoveWindow (dpy, w, x, y)
    register Display *dpy;
    Window w;
    int x, y;
{
    register xConfigureWindowReq *req;

    LockDisplay(dpy);
    GetReqExtra(ConfigureWindow, 8, req);

    req->window = w;
    req->mask = CWX | CWY;

#ifdef MUSTCOPY
    {
	long lx = (long) x, ly = (long) y;
	dpy->bufptr -= 8;
	Data32 (dpy, (long *) &lx, 4);	/* order dictated by CWX and CWY */
	Data32 (dpy, (long *) &ly, 4);
    }
#else
    {
	unsigned long *valuePtr =
	  (unsigned long *) NEXTPTR(req,xConfigureWindowReq);
	*valuePtr++ = x;
	*valuePtr = y;
    }
#endif /* MUSTCOPY */
    UnlockDisplay(dpy);
    SyncHandle();
}

