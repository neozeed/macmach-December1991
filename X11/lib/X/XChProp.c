#include "copyright.h"

/* $XConsortium: XChProp.c,v 11.19 89/12/11 19:08:47 rws Exp $ */
/* Copyright    Massachusetts Institute of Technology    1986	*/

#include "Xlibint.h"

#if NeedFunctionPrototypes
XChangeProperty (
    register Display *dpy,
    Window w,
    Atom property,
    Atom type,
    int format,  /* 8, 16, or 32 */
    int mode,  /* PropModeReplace, PropModePrepend, PropModeAppend */
    const unsigned char *data,
    int nelements)
#else
XChangeProperty (dpy, w, property, type, format, mode, data, nelements)
    register Display *dpy;
    Window w;
    Atom property, type;
    int format;  /* 8, 16, or 32 */
    int mode;  /* PropModeReplace, PropModePrepend, PropModeAppend */
    unsigned char *data;
    int nelements;
#endif
    {
    register xChangePropertyReq *req;
    register long len;

    LockDisplay(dpy);
    GetReq (ChangeProperty, req);
    req->window = w;
    req->property = property;
    req->type = type;
    req->format = format;
    req->mode = mode;
    req->nUnits = nelements;
    
    switch (format) {
      case 8:
	len = req->length + (((long)nelements + 3)>>2);
	if (len <= 65535) {
	    req->length = len;
	    Data (dpy, (char *)data, nelements);
	} /* else force BadLength */
        break;
 
      case 16:
	len = req->length + (((long)nelements + 1)>>1);
	if (len <= 65535) {
	    req->length = len;
	    len = (long)nelements << 1;
	    Data16 (dpy, (short *) data, len);
	} /* else force BadLength */
	break;

      case 32:
	len = req->length + (long)nelements;
	if (len <= 65535) {
	    req->length = len;
	    len = (long)nelements << 2;
	    Data32 (dpy, (long *) data, len);
	} /* else force BadLength */
	break;

      default:
        /* BadValue will be generated */ ;
      }

    UnlockDisplay(dpy);
    SyncHandle();
    }





