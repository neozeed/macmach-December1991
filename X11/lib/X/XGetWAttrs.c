#include "copyright.h"

/* $XConsortium: XGetWAttrs.c,v 11.22 89/06/14 14:21:58 jim Exp $ */
/* Copyright    Massachusetts Institute of Technology    1986	*/

#define NEED_REPLIES
#include "Xlibint.h"

Status XGetWindowAttributes(dpy, w, attr)
     register Display *dpy;
     Window w;
     XWindowAttributes *attr;

{       
    xGetWindowAttributesReply rep;
    xGetGeometryReply rep2;
    register xResourceReq *req1;
    register xResourceReq *req2;
    register int i;
    register Screen *sp;
 
    LockDisplay(dpy);
    GetResReq(GetWindowAttributes, w, req1);
    if (!_XReply (dpy, (xReply *)&rep,
       (SIZEOF(xGetWindowAttributesReply) - SIZEOF(xReply)) >> 2, xTrue)) {
		UnlockDisplay(dpy);
		SyncHandle();
      		return (0);
	}
    attr->class = rep.class;
    attr->bit_gravity = rep.bitGravity;
    attr->win_gravity = rep.winGravity;
    attr->backing_store = rep.backingStore;
    attr->backing_planes = rep.backingBitPlanes;
    attr->backing_pixel = rep.backingPixel;
    attr->save_under = rep.saveUnder;
    attr->colormap = rep.colormap;
    attr->map_installed = rep.mapInstalled;
    attr->map_state = rep.mapState;
    attr->all_event_masks = rep.allEventMasks;
    attr->your_event_mask = rep.yourEventMask;
    attr->do_not_propagate_mask = rep.doNotPropagateMask;
    attr->override_redirect = rep.override;
    attr->visual = _XVIDtoVisual (dpy, rep.visualID);
    
    GetResReq(GetGeometry, w, req2);

    if (!_XReply (dpy, (xReply *)&rep2, 0, xTrue)) {
	UnlockDisplay(dpy);
	SyncHandle();
	return (0);
	}
    attr->x = cvtINT16toInt (rep2.x);
    attr->y = cvtINT16toInt (rep2.y);
    attr->width = rep2.width;
    attr->height = rep2.height;
    attr->border_width = rep2.borderWidth;
    attr->depth = rep2.depth;
    attr->root = rep2.root;
    /* find correct screen so that applications find it easier.... */
    for (i = 0; i < dpy->nscreens; i++) {
	sp = &dpy->screens[i];
	if (sp->root == attr->root) {
	    attr->screen = sp;
	    break;
	}
    }
    UnlockDisplay(dpy);
    SyncHandle();
    return(1);
}

