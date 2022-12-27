/* $XConsortium: xsetmode.c,v 1.4 89/12/02 15:21:40 rws Exp $ */

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
 * Request to change the mode of an extension input device.
 *
 */

#define	 NEED_EVENTS
#define	 NEED_REPLIES
#include "X.h"				/* for inputstr.h    */
#include "Xproto.h"			/* Request macro     */
#include "XI.h"
#include "XIproto.h"
#include "inputstr.h"			/* DeviceIntPtr	     */
#include "windowstr.h"			/* window structure  */

extern	int 		IReqCode;
extern	int		BadDevice;
extern	int		BadMode;
extern	void		(* ReplySwapVector[256]) ();
extern	InputInfo	inputInfo;

/***********************************************************************
 *
 * Handle a request from a client with a different byte order.
 *
 */

int
SProcXSetDeviceMode(client)
    register ClientPtr client;
    {
    register char n;

    REQUEST(xSetDeviceModeReq);
    swaps(&stuff->length, n);
    return(ProcXSetDeviceMode(client));
    }

/***********************************************************************
 *
 * This procedure sets the mode of a device.
 *
 */

int
ProcXSetDeviceMode(client)
    register ClientPtr client;
    {
    int status;
    xSetDeviceModeReply	rep;

    REQUEST(xSetDeviceModeReq);
    REQUEST_SIZE_MATCH(xSetDeviceModeReq);

    rep.repType = X_Reply;
    rep.RepType = X_SetDeviceMode;
    rep.length = 0;
    rep.sequenceNumber = client->sequence;

    if (stuff->deviceid == inputInfo.pointer->id || 
	stuff->deviceid == inputInfo.keyboard->id)
	{
        rep.status = BadDevice;
        WriteReplyToClient (client, sizeof (xSetDeviceModeReply), &rep);
	SendErrorToClient(client, IReqCode, X_SetDeviceMode, 0, BadDevice);
        return Success;
	}

    status = SetDeviceMode (client, stuff->deviceid, stuff->mode);
    rep.status = status;
    WriteReplyToClient (client, sizeof (xSetDeviceModeReply), &rep);

    if (status != Success)
	SendErrorToClient(client, IReqCode, X_SetDeviceMode, 0, BadDevice);
    return Success;
    }

/***********************************************************************
 *
 * This procedure writes the reply for the XSetDeviceMode function,
 * if the client and server have a different byte ordering.
 *
 */

SRepXSetDeviceMode (client, size, rep)
    ClientPtr	client;
    int		size;
    xSetDeviceModeReply	*rep;
    {
    register char n;

    swaps(&rep->sequenceNumber, n);
    swapl(&rep->length, n);
    WriteToClient(client, size, rep);
    }

/****************************************************************************
 *
 * Change the mode of an extension device.
 *
 */

SetDeviceMode (client, id, mode)
    register	ClientPtr	client;
    int		id;
    int		mode;
    {
    return Success;
    }
