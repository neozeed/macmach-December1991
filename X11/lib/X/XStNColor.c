#include "copyright.h"

/* $XConsortium: XStNColor.c,v 11.11 89/12/11 19:10:29 rws Exp $ */
/* Copyright    Massachusetts Institute of Technology    1986	*/

#include "Xlibint.h"

#if NeedFunctionPrototypes
XStoreNamedColor(
register Display *dpy,
Colormap cmap,
const char *name, /* STRING8 */
unsigned long pixel, /* CARD32 */
int flags)  /* DoRed, DoGreen, DoBlue */
#else
XStoreNamedColor(dpy, cmap, name, pixel, flags)
register Display *dpy;
Colormap cmap;
char *name; /* STRING8 */
unsigned long pixel; /* CARD32 */
int flags;  /* DoRed, DoGreen, DoBlue */
#endif
{
    unsigned int nbytes;
    register xStoreNamedColorReq *req;

    LockDisplay(dpy);
    GetReq(StoreNamedColor, req);

    req->cmap = cmap;
    req->flags = flags;
    req->pixel = pixel;
    req->nbytes = nbytes = name ? strlen(name) : 0;
    req->length += (nbytes + 3) >> 2; /* round up to multiple of 4 */
    Data(dpy, name, (long)nbytes);
    UnlockDisplay(dpy);
    SyncHandle();
}


