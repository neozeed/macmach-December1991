#include "copyright.h"

/* $XConsortium: XGetHColor.c,v 11.10 88/09/06 16:07:50 jim Exp $ */
/* Copyright    Massachusetts Institute of Technology    1986	*/

#define NEED_REPLIES
#include "Xlibint.h"

Status XAllocColor(dpy, cmap, def)
register Display *dpy;
Colormap cmap;
XColor *def;
{
    Status status;
    xAllocColorReply rep;
    register xAllocColorReq *req;
    LockDisplay(dpy);
    GetReq(AllocColor, req);

    req->cmap = cmap;
    req->red = def->red;
    req->green = def->green;
    req->blue = def->blue;

    status = _XReply(dpy, (xReply *) &rep, 0, xTrue);
    if (status) {
      def->pixel = rep.pixel;
      def->red = rep.red;
      def->green = rep.green;
      def->blue = rep.blue;
      }
    UnlockDisplay(dpy);
    SyncHandle();
    return(status);
}
