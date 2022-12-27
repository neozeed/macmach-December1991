#include "copyright.h"

/* $XConsortium: XUngrabSvr.c,v 11.6 88/09/06 16:11:13 jim Exp $ */
/* Copyright    Massachusetts Institute of Technology    1986	*/

#include "Xlibint.h"
XUngrabServer (dpy)
register Display *dpy;
{
        register xReq *req;

        LockDisplay(dpy);
        GetEmptyReq(UngrabServer, req);
        UnlockDisplay(dpy);
	SyncHandle();
}

