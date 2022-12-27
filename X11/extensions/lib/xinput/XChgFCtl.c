/* $XConsortium: XChgFCtl.c,v 1.4 89/12/06 20:31:25 rws Exp $ */

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
 * XChangeFeedbackControl - Change the control attributes of feedbacks on
 * an extension device.
 *
 */

#include "XI.h"
#include "XIproto.h"
#include "Xlibint.h"
#include "XInput.h"
#include "extutil.h"

int
XChangeFeedbackControl (dpy, dev, mask, f)
    register	Display 	*dpy;
    XDevice			*dev;
    unsigned	long		mask;
    XFeedbackControl		*f;
    {
    char			*t;
    int				length;
    xChangeFeedbackControlReq	*req;
    XExtDisplayInfo *info = (XExtDisplayInfo *) XInput_find_display (dpy);

    LockDisplay (dpy);
    if (CheckExtInit(dpy, XInput_Initial_Release) == -1)
	return (NoSuchExtension);

    GetReq(ChangeFeedbackControl,req);
    req->reqType = info->codes->major_opcode;
    req->ReqType = X_ChangeFeedbackControl;
    req->deviceid = dev->device_id;
    req->mask = mask;
    req->feedbackid = f->class;

    if (f->class == KbdFeedbackClass)
	{
	XKbdFeedbackControl	*K;
	xKbdFeedbackCtl		k;

	K = (XKbdFeedbackControl *) f;
	k.class = KbdFeedbackClass;
	k.length = sizeof (xKbdFeedbackCtl);
	k.click = K->click;
	k.percent = K->percent;
	k.pitch = K->pitch;
	k.duration = K->duration;
	k.led_mask = K->led_mask;
	k.led_values = K->led_value;
	k.key = K->key;
	k.auto_repeat_mode = K->auto_repeat_mode;
	length = ((k.length + 3) >> 2);
	t = (char *) &k;
	}
    else if (f->class == PtrFeedbackClass)
	{
	XPtrFeedbackControl	*P;
	xPtrFeedbackCtl		p;

	P = (XPtrFeedbackControl *) f;
	p.class = PtrFeedbackClass;
	p.length = sizeof (xPtrFeedbackCtl);
	p.num = P->accelNum;
	p.denom = P->accelDenom;
	p.thresh = P->threshold;
	length = ((p.length + 3) >> 2);
	t = (char *) &p;
	}
    else if (f->class == IntegerFeedbackClass)
	{
	XIntegerFeedbackControl	*I;
	xIntegerFeedbackCtl	i;

	I = (XIntegerFeedbackControl *) f;
	i.class = IntegerFeedbackClass;
	i.length = sizeof (xIntegerFeedbackCtl);
	i.int_to_display = I->int_to_display;
	length = ((i.length + 3) >> 2);
	t = (char *) &i;
	}
    else if (f->class == StringFeedbackClass)
	{
	XStringFeedbackControl	*S;
	xStringFeedbackCtl	s;

	S = (XStringFeedbackControl *) f;
	s.class = StringFeedbackClass;
	s.length = sizeof (xStringFeedbackCtl);
	s.num_keysyms = S->num_keysyms;
	length = ((s.length + 3) >> 2);
	t = (char *) &s;
	}
    else if (f->class == BellFeedbackClass)
	{
	XBellFeedbackControl	*B;
	xBellFeedbackCtl	b;

	B = (XBellFeedbackControl *) f;
	b.class = BellFeedbackClass;
	b.length = sizeof (xBellFeedbackCtl);
	b.percent = B->percent;
	b.pitch = B->pitch;
	b.duration = B->duration;
	length = ((b.length + 3) >> 2);
	t = (char *) &b;
	}
    else if (f->class == LedFeedbackClass)
	{
	XLedFeedbackControl	*L;
	xLedFeedbackCtl		l;

	L = (XLedFeedbackControl *) f;
	l.class = LedFeedbackClass;
	l.length = sizeof (xLedFeedbackCtl);
	l.led_mask = L->led_mask;
	l.led_values = L->led_values;
	length = ((l.length + 3) >> 2);
	t = (char *) &l;
	}
    else
	{
	xFeedbackCtl		u;

	u.class = f->class;
	u.length = f->length - sizeof (int);
	length = ((u.length + 3) >> 2);
	t = (char *) &u;
	}

    req->length += length;
    Data (dpy, (char *) t, length<<2);
    UnlockDisplay(dpy);
    SyncHandle();
    return (Success);
    }

