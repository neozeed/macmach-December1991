/* $XConsortium: xchgkbd.c,v 1.7 89/12/02 15:20:31 rws Exp $ */

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
 * Extension function to change the keyboard device.
 *
 */

#define	 NEED_EVENTS
#define	 NEED_REPLIES
#include "X.h"				/* for inputstr.h    */
#include "Xproto.h"			/* Request macro     */
#include "inputstr.h"			/* DeviceIntPtr	     */
#include "XI.h"
#include "XIproto.h"

extern	int 		IReqCode;
extern	int 		BadDevice;
extern	int 		ChangeDeviceNotify;
extern	Mask		ChangeDeviceNotifyMask;
extern	InputInfo	inputInfo;
extern	void		(* ReplySwapVector[256]) ();
DeviceIntPtr		LookupDeviceIntRec();

/***********************************************************************
 *
 * This procedure changes the keyboard device.
 *
 */

int
SProcXChangeKeyboardDevice(client)
    register ClientPtr client;
    {
    register char n;

    REQUEST(xChangeKeyboardDeviceReq);
    swaps(&stuff->length, n);
    return(ProcXChangeKeyboardDevice(client));
    }

/***********************************************************************
 *
 * This procedure is invoked to swap the request bytes if the server and
 * client have a different byte order.
 *
 */

ProcXChangeKeyboardDevice (client)
    register ClientPtr client;
    {
    extern			ChangeKeyboardDevice();
    int				i;
    DeviceIntPtr 		dev;
    GrabPtr 			grab;
    KeyClassPtr 		k;
    xChangeKeyboardDeviceReply	rep;
    changeDeviceNotify		ev;

    REQUEST(xChangeKeyboardDeviceReq);
    REQUEST_SIZE_MATCH(xChangeKeyboardDeviceReq);

    rep.repType = X_Reply;
    rep.RepType = X_ChangeKeyboardDevice;
    rep.length = 0;
    rep.sequenceNumber = client->sequence;

    dev = LookupDeviceIntRec (stuff->deviceid);
    if (dev == NULL)
	{
	rep.status = -1;
	SendErrorToClient(client, IReqCode, X_ChangeKeyboardDevice, 0, 
		BadDevice);
	return Success;
	}

    k = dev->key;
    if (k == NULL)
	{
	rep.status = -1;
	SendErrorToClient(client, IReqCode, X_ChangeKeyboardDevice, 0, 
		BadMatch);
	return Success;
	}

    grab =  dev->grab;

    if ((grab) && !SameClient(grab, client))
	rep.status = AlreadyGrabbed;
    else if (dev->sync.frozen &&
	     ((dev->sync.other &&
	       !SameClient(dev->sync.other, client)) ||
	     ((dev->sync.state >= FROZEN) &&
	      !SameClient(dev->grab, client))))
	rep.status = GrabFrozen;

    if (ChangeKeyboardDevice (inputInfo.keyboard, dev) != Success)
	{
	SendErrorToClient(client, IReqCode, X_ChangeKeyboardDevice, 0, 
		BadDevice);
	return Success;
	}
    dev->focus->win = inputInfo.keyboard->focus->win;
    inputInfo.keyboard = dev;

    ev.type = ChangeDeviceNotify;
    ev.deviceid = stuff->deviceid;
    ev.time = currentTime.milliseconds;
    ev.request = NewKeyboard;

    SendEventToAllWindows (dev, ChangeDeviceNotifyMask, &ev, 1);
    SendMappingNotify (MappingKeyboard, k->curKeySyms.minKeyCode, 
	k->curKeySyms.maxKeyCode - k->curKeySyms.minKeyCode + 1);

    rep.status = 0;
    WriteReplyToClient (client, sizeof (xChangeKeyboardDeviceReply), 
	&rep);

    return Success;
    }

/***********************************************************************
 *
 * This procedure writes the reply for the XChangeKeyboardDevice 
 * function, if the client and server have a different byte ordering.
 *
 */

SRepXChangeKeyboardDevice (client, size, rep)
    ClientPtr	client;
    int		size;
    xChangeKeyboardDeviceReply	*rep;
    {
    register char n;

    swaps(&rep->sequenceNumber, n);
    swapl(&rep->length, n);
    WriteToClient(client, size, rep);
    }

