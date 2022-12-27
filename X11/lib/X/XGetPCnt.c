#include "copyright.h"

/* $XConsortium: XGetPCnt.c,v 11.10 88/09/06 16:08:10 jim Exp $ */
/* Copyright    Massachusetts Institute of Technology    1986	*/

#define NEED_REPLIES
#include "Xlibint.h"

XGetPointerControl(dpy, accel_numer, accel_denom, threshold)
     register Display *dpy;
     /* the following are return only vars */
     int *accel_numer, *accel_denom;
     int *threshold;
{       
    xGetPointerControlReply rep;
    xReq *req;
    LockDisplay(dpy);
    GetEmptyReq(GetPointerControl, req);
    (void) _XReply (dpy, (xReply *)&rep, 0, xTrue);
    *accel_numer = rep.accelNumerator;
    *accel_denom = rep.accelDenominator;
    *threshold = rep.threshold;
    UnlockDisplay(dpy);
    SyncHandle();
}

