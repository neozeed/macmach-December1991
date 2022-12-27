#include "copyright.h"

/* $XConsortium: XChWindow.c,v 11.9 88/09/06 16:04:37 jim Exp $ */
/* Copyright    Massachusetts Institute of Technology    1986	*/

#include "Xlibint.h"

XResizeWindow(dpy, w, width, height)
register Display *dpy;
Window w;
unsigned int width, height;
{
    register xConfigureWindowReq *req;

    LockDisplay(dpy);
    GetReqExtra(ConfigureWindow, 8, req); /* 2 4-byte quantities */

    /* XXX assuming that sizeof(unsigned long) is 32 bits */

    req->window = w;
    req->mask = CWWidth | CWHeight;
#ifdef MUSTCOPY
    {
	unsigned long lwidth = width, lheight = height;
    dpy->bufptr -= 8;
    Data32 (dpy, (long *) &lwidth, 4);	/* order dictated by values of */
    Data32 (dpy, (long *) &lheight, 4);	/* CWWidth and CWHeight */
    }
#else
    {
	unsigned long * valuePtr =
	  (unsigned long *) NEXTPTR(req,xConfigureWindowReq);
	*valuePtr++ = width;
	*valuePtr++ = height;
    }
#endif /* MUSTCOPY */
    UnlockDisplay(dpy);
    SyncHandle();
}
