/* $XConsortium: xgetfctl.c,v 1.5 89/12/02 15:20:55 rws Exp $ */

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

/********************************************************************
 *
 *  Get feedback control attributes for an extension device.
 *
 */

#define	 NEED_EVENTS			/* for inputstr.h    */
#define	 NEED_REPLIES
#include "X.h"				/* for inputstr.h    */
#include "Xproto.h"			/* Request macro     */
#include "inputstr.h"			/* DeviceIntPtr	     */
#include "XI.h"
#include "XIproto.h"

extern	int 	IReqCode;
extern	int	BadDevice;
extern	void	(* ReplySwapVector[256]) ();
DeviceIntPtr	LookupDeviceIntRec();
void		CopySwapKbdFeedback();
void		CopySwapPtrFeedback();
void		CopySwapIntegerFeedback();
void		CopySwapStringFeedback();
void		CopySwapLedFeedback();
void		CopySwapBellFeedback();

/***********************************************************************
 *
 * This procedure gets the control attributes for an extension device,
 * for clients on machines with a different byte ordering than the server.
 *
 */

int
SProcXGetFeedbackControl(client)
    register ClientPtr client;
    {
    register char n;

    REQUEST(xGetFeedbackControlReq);
    swaps(&stuff->length, n);
    return(ProcXGetFeedbackControl(client));
    }

/***********************************************************************
 *
 * Get the feedback control state.
 *
 */

ProcXGetFeedbackControl(client)
    ClientPtr client;
    {
    int	total_length = 0;
    int	i;
    char *p, *buf, *savbuf;
    register DeviceIntPtr dev;
    xGetFeedbackControlReply rep;

    REQUEST(xGetFeedbackControlReq);
    REQUEST_SIZE_MATCH(xGetFeedbackControlReq);

    dev = LookupDeviceIntRec (stuff->deviceid);
    if (dev == NULL)
	{
	SendErrorToClient (client, IReqCode, X_GetFeedbackControl, 0, 
		BadDevice);
	return Success;
	}

    rep.repType = X_Reply;
    rep.RepType = X_GetFeedbackControl;
    rep.length = 0;
    rep.sequenceNumber = client->sequence;
    rep.num_feedbacks = 0;

    if (dev->kbdfeed != NULL)
	{
	rep.num_feedbacks++;
	total_length += sizeof(xKbdFeedbackState);
	}
    if (dev->ptrfeed != NULL)
	{
	rep.num_feedbacks++;
	total_length += sizeof(xPtrFeedbackState);
	}
    if (dev->stringfeed != NULL)
	{
	rep.num_feedbacks++;
	total_length += sizeof(xStringFeedbackState);
	}
    if (dev->intfeed != NULL)
	{
	rep.num_feedbacks++;
	total_length += sizeof(xIntegerFeedbackState);
	}
    if (dev->leds != NULL)
	{
	rep.num_feedbacks++;
	total_length += sizeof(xLedFeedbackState);
	}
    if (dev->bell != NULL)
	{
	rep.num_feedbacks++;
	total_length += sizeof(xBellFeedbackState);
	}

    if (total_length == 0)
	{
	SendErrorToClient(client, IReqCode, X_ChangeFeedbackControl, 0, 
		BadMatch);
	return Success;
	}

    buf = (char *) Xalloc (total_length);
    savbuf=buf;

    if (dev->kbdfeed != NULL)
	CopySwapKbdFeedback (client, dev->kbdfeed, &buf);
    if (dev->ptrfeed != NULL)
	CopySwapPtrFeedback (client, dev->ptrfeed, &buf);
    if (dev->stringfeed != NULL)
	CopySwapStringFeedback (client, dev->stringfeed, &buf);
    if (dev->intfeed != NULL)
	CopySwapIntegerFeedback (client, dev->intfeed, &buf);
    if (dev->leds != NULL)
	CopySwapLedFeedback (client, dev->leds, &buf);
    if (dev->bell != NULL)
	CopySwapBellFeedback (client, dev->bell, &buf);

    rep.length = (total_length+3) >> 2;
    WriteReplyToClient(client, sizeof(xGetFeedbackControlReply), &rep);
    WriteToClient(client, total_length, savbuf);
    return Success;
    }

/***********************************************************************
 *
 * This procedure copies KbdFeedbackClass data, swapping if necessary.
 *
 */

void
CopySwapKbdFeedback (client, k, buf)
    ClientPtr 		client;
    KbdFeedbackPtr 	k;
    char 		**buf;
    {
    int	i;
    register char 	n;
    xKbdFeedbackState	*k2;

    k2 = (xKbdFeedbackState *) *buf;
    k2->class = KbdFeedbackClass;
    k2->length = sizeof (xKbdFeedbackState);
    k2->click = k->ctrl.click;
    k2->percent = k->ctrl.bell;
    k2->pitch = k->ctrl.bell_pitch;
    k2->duration = k->ctrl.bell_duration;
    k2->led_mask = k->ctrl.leds;
    k2->global_auto_repeat = k->ctrl.autoRepeat;
    for (i=0; i<32; i++)
	k2->auto_repeats[i] = k->ctrl.autoRepeats[i];
    if (client->swapped)
	{
	swaps(&k2->pitch,n);
	swaps(&k2->duration,n);
	swapl(&k2->led_mask,n);
	swapl(&k2->led_values,n);
	}
    *buf += sizeof (xKbdFeedbackState);
    }

/***********************************************************************
 *
 * This procedure copies PtrFeedbackClass data, swapping if necessary.
 *
 */

void
CopySwapPtrFeedback (client, p, buf)
    ClientPtr 		client;
    PtrFeedbackPtr 	p;
    char 		**buf;
    {
    register char 	n;
    xPtrFeedbackState	*p2;

    p2 = (xPtrFeedbackState *) *buf;
    p2->class = PtrFeedbackClass;
    p2->length = sizeof (xPtrFeedbackState);
    p2->accelNum = p->ctrl.num;
    p2->accelDenom = p->ctrl.den;
    p2->threshold = p->ctrl.threshold;
    if (client->swapped)
	{
	swaps(&p2->accelNum,n);
	swaps(&p2->accelDenom,n);
	swaps(&p2->threshold,n);
	}
    *buf += sizeof (xPtrFeedbackState);
    }

/***********************************************************************
 *
 * This procedure copies IntegerFeedbackClass data, swapping if necessary.
 *
 */

void
CopySwapIntegerFeedback (client, i, buf)
    ClientPtr 		client;
    IntegerFeedbackPtr 	i;
    char 		**buf;
    {
    register char 		n;
    xIntegerFeedbackState	*i2;

    i2 = (xIntegerFeedbackState *) *buf;
    i2->class = IntegerFeedbackClass;
    i2->length = sizeof (xIntegerFeedbackState);
    i2->resolution = i->ctrl.resolution;
    i2->min_value = i->ctrl.min_value;
    i2->max_value = i->ctrl.max_value;
    if (client->swapped)
	{
	swapl(&i2->resolution,n);
	swapl(&i2->min_value,n);
	swapl(&i2->max_value,n);
	}
    *buf += sizeof (xIntegerFeedbackState);
    }

/***********************************************************************
 *
 * This procedure copies StringFeedbackClass data, swapping if necessary.
 *
 */

void
CopySwapStringFeedback (client, s, buf)
    ClientPtr 		client;
    StringFeedbackPtr 	s;
    char 		**buf;
    {
    register char 		n;
    xStringFeedbackState	*s2;

    s2 = (xStringFeedbackState *) *buf;
    s2->class = StringFeedbackClass;
    s2->length = sizeof (xStringFeedbackState);
    s2->max_symbols = s->ctrl.max_symbols;
    s2->num_syms_supported = s->ctrl.num_symbols_supported;
    if (client->swapped)
	{
	swaps(&s2->max_symbols,n);
	swaps(&s2->num_syms_supported,n);
	}
    *buf += sizeof (xStringFeedbackState);
    }

/***********************************************************************
 *
 * This procedure copies LedFeedbackClass data, swapping if necessary.
 *
 */

void
CopySwapLedFeedback (client, l, buf)
    ClientPtr 		client;
    LedFeedbackPtr 	l;
    char 		**buf;
    {
    register char 	n;
    xLedFeedbackState	*l2;

    l2 = (xLedFeedbackState *) *buf;
    l2->class = LedFeedbackClass;
    l2->length = sizeof (xLedFeedbackState);
    l2->led_values = l->ctrl.led_values;
    if (client->swapped)
	{
	swapl(&l2->led_values,n);
	swapl(&l2->led_mask,n);
	}
    *buf += sizeof (xLedFeedbackState);
    }

/***********************************************************************
 *
 * This procedure copies BellFeedbackClass data, swapping if necessary.
 *
 */

void
CopySwapBellFeedback (client, b, buf)
    ClientPtr 		client;
    BellFeedbackPtr 	b;
    char 		**buf;
    {
    register char 	n;
    xBellFeedbackState	*b2;

    b2 = (xBellFeedbackState *) *buf;
    b2->class = BellFeedbackClass;
    b2->length = sizeof (xBellFeedbackState);
    b2->percent = b->ctrl.percent;
    b2->pitch = b->ctrl.pitch;
    b2->duration = b->ctrl.duration;
    if (client->swapped)
	{
	swaps(&b2->pitch,n);
	swaps(&b2->duration,n);
	}
    *buf += sizeof (xBellFeedbackState);
    }

/***********************************************************************
 *
 * This procedure writes the reply for the xGetFeedbackControl function,
 * if the client and server have a different byte ordering.
 *
 */

SRepXGetFeedbackControl (client, size, rep)
    ClientPtr	client;
    int		size;
    xGetFeedbackControlReply	*rep;
    {
    register char n;

    swaps(&rep->sequenceNumber, n);
    swapl(&rep->length, n);
    swaps(&rep->num_feedbacks, n);
    WriteToClient(client, size, rep);
    }
