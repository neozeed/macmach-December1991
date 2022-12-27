/* $XConsortium: xchgfctl.c,v 1.5 89/12/02 15:20:28 rws Exp $ */

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
 *  Change feedback control attributes for an extension device.
 *
 */

#define	 NEED_EVENTS			/* for inputstr.h    */
#define	 NEED_REPLIES
#include "X.h"				/* for inputstr.h    */
#include "Xproto.h"			/* Request macro     */
#include "inputstr.h"			/* DeviceIntPtr	     */
#include "XI.h"
#include "XIproto.h"			/* control constants */

#define DO_ALL    (-1)

extern	int 	IReqCode;
extern	int	BadDevice;
DeviceIntPtr	LookupDeviceIntRec();

/***********************************************************************
 *
 * This procedure changes the control attributes for an extension device,
 * for clients on machines with a different byte ordering than the server.
 *
 */

int
SProcXChangeFeedbackControl(client)
    register ClientPtr client;
    {
    register char n;

    REQUEST(xChangeFeedbackControlReq);
    swaps(&stuff->length, n);
    swapl(&stuff->mask, n);
    return(ProcXChangeFeedbackControl(client));
    }

/***********************************************************************
 *
 * Change the control attributes.
 *
 */

ProcXChangeFeedbackControl(client)
    ClientPtr client;
    {
    DeviceIntPtr dev;

    REQUEST(xChangeFeedbackControlReq);
    REQUEST_AT_LEAST_SIZE(xChangeFeedbackControlReq);

    dev = LookupDeviceIntRec (stuff->deviceid);
    if (dev == NULL)
	{
	SendErrorToClient (client, IReqCode, X_ChangeFeedbackControl, 0, 
		BadDevice);
	return Success;
	}

    switch (stuff->feedbackid)
	{
	case KbdFeedbackClass:
	    ChangeKbdFeedback (client, dev, stuff->mask, &stuff[1]);
	    break;
	case PtrFeedbackClass:
	    ChangePtrFeedback (client, dev, stuff->mask, &stuff[1]);
	    break;
	case StringFeedbackClass:
	    ChangeStringFeedback (client, dev, stuff->mask, &stuff[1]);
	    break;
	case IntegerFeedbackClass:
	    ChangeIntegerFeedback (client, dev, stuff->mask, &stuff[1]);
	    break;
	case LedFeedbackClass:
	    ChangeLedFeedback (client, dev, stuff->mask, &stuff[1]);
	    break;
	case BellFeedbackClass:
	    ChangeBellFeedback (client, dev, stuff->mask, &stuff[1]);
	    break;
	default:
	    SendErrorToClient(client, IReqCode, X_ChangeFeedbackControl, 0, 
		BadMatch);
	    break;
	}

    return Success;
    } 

/******************************************************************************
 *
 * This procedure changes KbdFeedbackClass data.
 *
 */

ChangeKbdFeedback (client, dev, mask, f)
    ClientPtr client;
    DeviceIntPtr dev;
    unsigned long 	mask;
    xKbdFeedbackCtl 	*f;
    {
    register char n;
    KbdFeedbackPtr	k = dev->kbdfeed;
    KeybdCtrl kctrl;
    int t;
    int led = DO_ALL;
    int key = DO_ALL;

    if (client->swapped)
	{
	swaps(&f->pitch,n);
	swaps(&f->duration,n);
	swapl(&f->led_mask,n);
	swapl(&f->led_values,n);
	}

    kctrl = k->ctrl;
    if (mask & DvKeyClickPercent)
	{
	t = f->click;
	if (t == -1)
	    t = defaultKeyboardControl.click;
	else if (t < 0 || t > 100)
	    {
	    client->errorValue = t;
	    SendErrorToClient (client, IReqCode, X_ChangeFeedbackControl, 0, 
		BadValue);
	    return Success;
	    }
	kctrl.click = t;
        }

    if (mask & DvPercent)
	{
	t = f->percent;
	if (t == -1)
	    t = defaultKeyboardControl.bell;
	else if (t < 0 || t > 100)
	    {
	    client->errorValue = t;
	    SendErrorToClient (client, IReqCode, X_ChangeFeedbackControl, 0, 
		BadValue);
	    return Success;
	    }
	kctrl.bell = t;
	}

    if (mask & DvPitch)
	{
	t = f->pitch;
	if (t == -1)
	    t = defaultKeyboardControl.bell_pitch;
	else if (t < 0)
	    {
	    client->errorValue = t;
	    SendErrorToClient (client, IReqCode, X_ChangeFeedbackControl, 0, 
		BadValue);
	    return Success;
	    }
	kctrl.bell_pitch = t;
	}

    if (mask & DvDuration)
	{
	t = f->duration;
	if (t == -1)
	    t = defaultKeyboardControl.bell_duration;
	else if (t < 0)
	    {
	    client->errorValue = t;
	    SendErrorToClient (client, IReqCode, X_ChangeFeedbackControl, 0, 
		BadValue);
	    return Success;
	    }
	kctrl.bell_duration = t;
        }

    if (mask & DvLed)
        {
	kctrl.leds &= ~(f->led_mask);
	kctrl.leds |= (f->led_mask & f->led_values);
        }

    if (mask & DvKey)
	{
	key = (KeyCode) f->key;
	if (key < 8 || key > 255)
	    {
	    client->errorValue = key;
	    SendErrorToClient (client, IReqCode, X_ChangeFeedbackControl, 0, 
		BadValue);
	    return Success;
	    }
	if (!(mask & DvAutoRepeatMode))
	    {
	    SendErrorToClient (client, IReqCode, X_ChangeFeedbackControl, 0, 
		BadMatch);
	    return Success;
	    }
	}

    if (mask & DvAutoRepeatMode)
	{
	int index = (key >> 3);
	int kmask = (1 << (key & 7));
	t = (CARD8) f->auto_repeat_mode;
	if (t == AutoRepeatModeOff)
	    {
	    if (key == DO_ALL)
		kctrl.autoRepeat = FALSE;
	    else
		kctrl.autoRepeats[index] &= ~kmask;
	    }
	else if (t == AutoRepeatModeOn)
	    {
	    if (key == DO_ALL)
		kctrl.autoRepeat = TRUE;
	    else
		kctrl.autoRepeats[index] |= kmask;
	    }
	else if (t == AutoRepeatModeDefault)
	    {
	    if (key == DO_ALL)
		kctrl.autoRepeat = defaultKeyboardControl.autoRepeat;
	    else
		kctrl.autoRepeats[index] &= ~kmask;
		kctrl.autoRepeats[index] =
			(kctrl.autoRepeats[index] & ~kmask) |
			(defaultKeyboardControl.autoRepeats[index] & kmask);
	    }
	else
	    {
	    client->errorValue = t;
	    SendErrorToClient (client, IReqCode, X_ChangeFeedbackControl, 0, 
		BadValue);
	    return Success;
	    }
        }

    k->ctrl = kctrl;
    (*k->CtrlProc)(dev, &k->ctrl);
    return Success;
    }

/******************************************************************************
 *
 * This procedure changes PtrFeedbackClass data.
 *
 */

ChangePtrFeedback (client, dev, mask, f)
    ClientPtr 		client;
    DeviceIntPtr 	dev;
    unsigned long 	mask;
    xPtrFeedbackCtl 	*f;
    {
    register char n;
    PtrFeedbackPtr p = dev->ptrfeed;
    PtrCtrl pctrl;		/* might get BadValue part way through */

    if (client->swapped)
	{
	swaps(&f->num,n);
	swaps(&f->denom,n);
	swaps(&f->thresh,n);
	}

    pctrl = p->ctrl;
    if (mask & DvAccelNum)
	{
	int	accelNum;

	accelNum = f->num;
	if (accelNum == -1)
	    pctrl.num = defaultPointerControl.num;
	else if (accelNum < 0)
	    {
	    client->errorValue = accelNum;
	    SendErrorToClient (client, IReqCode, X_ChangeFeedbackControl, 0, 
		BadValue);
	    return Success;
	    }
	else pctrl.num = accelNum;
	}

    if (mask & DvAccelDenom)
	{
	int	accelDenom;

	accelDenom = f->denom;
	if (accelDenom == -1)
	    pctrl.den = defaultPointerControl.den;
	else if (accelDenom <= 0)
	    {
	    client->errorValue = accelDenom;
	    SendErrorToClient (client, IReqCode, X_ChangeFeedbackControl, 0, 
		BadValue);
	    return Success;
	    }
	else pctrl.den = accelDenom;
        }

    if (mask & DvThreshold)
	{
	int	threshold;

	threshold = f->thresh;
	if (threshold == -1)
	    pctrl.threshold = defaultPointerControl.threshold;
	else if (threshold < 0)
	    {
	    client->errorValue = threshold;
	    SendErrorToClient (client, IReqCode, X_ChangeFeedbackControl, 0, 
		BadValue);
	    return Success;
	    }
	else pctrl.threshold = threshold;
        }

    p->ctrl = pctrl;
    (*p->CtrlProc)(dev, &p->ctrl);
    return Success;
    }

/******************************************************************************
 *
 * This procedure changes IntegerFeedbackClass data.
 *
 */

ChangeIntegerFeedback (client, dev, mask, f)
    ClientPtr 			client;
    DeviceIntPtr 		dev;
    unsigned long 		mask;
    xIntegerFeedbackCtl 	*f;
    {
    IntegerFeedbackPtr i = dev->intfeed;
    register char n;

    if (client->swapped)
	{
	swapl(&f->int_to_display,n);
	}

    i->ctrl.integer_displayed = f->int_to_display;
    (*i->CtrlProc)(dev, &i->ctrl);
    return Success;
    }

/******************************************************************************
 *
 * This procedure changes StringFeedbackClass data.
 *
 */

ChangeStringFeedback (client, dev, mask, f)
    ClientPtr 		client;
    DeviceIntPtr 	dev;
    unsigned long 	mask;
    xStringFeedbackCtl 	*f;
    {
    register char n;
    register long *p;
    StringFeedbackPtr s = dev->stringfeed;
    int		i;
    KeySym	*syms;

    if (client->swapped)
	{
	swaps(&f->num_keysyms,n);
	p = (long *) (f+1);
	for (i=0; i<f->num_keysyms; i++)
	    {
	    swapl(p, n);
	    p++;
	    }
	}

    syms = (KeySym *) (f+1);
    s->ctrl.num_symbols_displayed  = f->num_keysyms;
    for (i=0; i<f->num_keysyms; i++)
	s->ctrl.symbols[i] = *(syms+i);
    (*s->CtrlProc)(dev, &s->ctrl);
    return Success;
    }

/******************************************************************************
 *
 * This procedure changes BellFeedbackClass data.
 *
 */

ChangeBellFeedback (client, dev, mask, f)
    ClientPtr 		client;
    DeviceIntPtr 	dev;
    unsigned long 	mask;
    xBellFeedbackCtl 	*f;
    {
    register char n;
    BellFeedbackPtr b = dev->bell;
    int t;
    BellCtrl bctrl;		/* might get BadValue part way through */

    if (client->swapped)
	{
	swaps(&f->pitch,n);
	swaps(&f->duration,n);
	}

    bctrl = b->ctrl;
    if (mask & DvPercent)
	{
	t = f->percent;
	if (t == -1)
	    t = defaultKeyboardControl.bell;
	else if (t < 0 || t > 100)
	    {
	    client->errorValue = t;
	    SendErrorToClient (client, IReqCode, X_ChangeFeedbackControl, 0, 
		BadValue);
	    return Success;
	    }
	bctrl.percent = t;
	}

    if (mask & DvPitch)
	{
	t = f->pitch;
	if (t == -1)
	    t = defaultKeyboardControl.bell_pitch;
	else if (t < 0)
	    {
	    client->errorValue = t;
	    SendErrorToClient (client, IReqCode, X_ChangeFeedbackControl, 0, 
		BadValue);
	    return Success;
	    }
	bctrl.pitch = t;
	}

    if (mask & DvDuration)
	{
	t = f->duration;
	if (t == -1)
	    t = defaultKeyboardControl.bell_duration;
	else if (t < 0)
	    {
	    client->errorValue = t;
	    SendErrorToClient (client, IReqCode, X_ChangeFeedbackControl, 0, 
		BadValue);
	    return Success;
	    }
	bctrl.duration = t;
        }
    b->ctrl = bctrl;
    (*b->CtrlProc)(dev, &b->ctrl);
    return Success;
    }

/******************************************************************************
 *
 * This procedure changes LedFeedbackClass data.
 *
 */

ChangeLedFeedback (client, dev, mask, f)
    ClientPtr 		client;
    DeviceIntPtr 	dev;
    unsigned long 	mask;
    xLedFeedbackCtl 	*f;
    {
    register char n;
    LedFeedbackPtr l = dev->leds;
    LedCtrl lctrl;		/* might get BadValue part way through */

    if (client->swapped)
	{
	swapl(&f->led_values,n);
	swapl(&f->led_mask,n);
	}

    lctrl = l->ctrl;
    if (mask & DvLed)
        {
	lctrl.led_values &= ~(f->led_mask);
	lctrl.led_values |= (f->led_mask & f->led_values);
        }

    l->ctrl = lctrl;
    (*l->CtrlProc)(dev, &l->ctrl);
    return Success;
    }
