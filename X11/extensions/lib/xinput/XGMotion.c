/* $XConsortium: XGMotion.c,v 1.4 89/12/06 20:38:16 rws Exp $ */

/************************************************************
Copyright (c) 1989 by Hewlett-Packard Company, Palo Alto, California, and the 
Massachusetts Institute of Technology, Cambridge, Massachusetts.

			All Rights Reserved

Permission to use, copy, modify, and distribute this software and its
documentation for any purpose and without fee is hereby granted,
provided that the above copyright notice appear in all copies and that
both that copyright notice and this permission notice appear in
supporting documentation, and that the names of Hewlett-Packard or MIT not be
used in advertising or publicity pertaining to distribution of the
software without specific, written prior permission.

HEWLETT-PACKARD DISCLAIMS ALL WARRANTIES WITH REGARD TO THIS SOFTWARE, INCLUDING
ALL IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS, IN NO EVENT SHALL
HEWLETT-PACKARD BE LIABLE FOR ANY SPECIAL, INDIRECT OR CONSEQUENTIAL DAMAGES OR
ANY DAMAGES WHATSOEVER RESULTING FROM LOSS OF USE, DATA OR PROFITS,
WHETHER IN AN ACTION OF CONTRACT, NEGLIGENCE OR OTHER TORTIOUS ACTION,
ARISING OUT OF OR IN CONNECTION WITH THE USE OR PERFORMANCE OF THIS
SOFTWARE.

********************************************************/

/***********************************************************************
 *
 * XGetDeviceMotionEvents - Get the motion history of an input device.
 *
 */

#include "XI.h"
#include "XIproto.h"
#include "Xlibint.h"
#include "XInput.h"
#include "extutil.h"

XDeviceTimeCoord
*XGetDeviceMotionEvents (dpy, dev, start, stop, nEvents, mode, axis_count)
    register 	Display	*dpy;
    XDevice		*dev;
    Time 		start;
    Time 		stop;
    int 		*nEvents;
    int 		*mode;
    int 		*axis_count;
    {       
    char **tmp;
    xGetDeviceMotionEventsReq 	*req;
    xGetDeviceMotionEventsReply 	rep;
    char *tc, *tc2;
    char *bufp, *buf2p, *tmpp;
    long size, size2;
    int	 i, j;
    XExtDisplayInfo *info = (XExtDisplayInfo *) XInput_find_display (dpy);

    LockDisplay (dpy);
    if (CheckExtInit(dpy, XInput_Initial_Release) == -1)
	return ((XDeviceTimeCoord *) NoSuchExtension);

    GetReq(GetDeviceMotionEvents,req);		
    req->reqType = info->codes->major_opcode;
    req->ReqType = X_GetDeviceMotionEvents;
    req->start = start;
    req->stop = stop;
    req->deviceid = dev->device_id;

    if (!_XReply (dpy, (xReply *)&rep, 0, xFalse)) {
	UnlockDisplay(dpy);
        SyncHandle();
	*nEvents = 0;
	return (NULL);
	}

    *mode = rep.mode;
    *axis_count = rep.axes;
    *nEvents = rep.nEvents;
    size = rep.length << 2;
    size2 = (rep.nEvents * sizeof (short *)) + size;
    buf2p = (char *) Xmalloc (size);
    bufp = (char *) Xmalloc (size2);
    if (!bufp) {
	/* XXX this is wrong!!  we need to read and throw away the data
           somehow.  Probably we should try to malloc less space and repeatedly
           read the events into the smaller space.... */
	*nEvents = 0;
	UnlockDisplay(dpy);
        SyncHandle();
	return (NULL);
	}
    _XRead (dpy, (char *) buf2p, size);

    tc = bufp;
    tc2 = bufp + (rep.nEvents * sizeof (XDeviceTimeCoord));
    tmpp = buf2p;
    for (i=0; i<*nEvents; i++)
	{
	*((Time *) tc) = *((Time *) tmpp);
	tc += sizeof (Time);
	tmpp += sizeof (Time);

	tmp = (char **) tc;
	*tmp = tc2;
	tc += sizeof (unsigned short *);

	for (j=0; j<*axis_count; j++)
	    {
	    *((short *) tc2) = *((short *) tmpp);
	    tc2 += sizeof (short);
	    tmpp += sizeof (short);
	    }
	}
    XFree (buf2p);
    UnlockDisplay(dpy);
    SyncHandle();
    return ((XDeviceTimeCoord *) bufp);
    }

