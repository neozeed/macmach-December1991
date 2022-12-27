#include "copyright.h"

/* $XConsortium: XChKeyCon.c,v 11.10 88/09/06 16:04:31 jim Exp $ */
/* Copyright    Massachusetts Institute of Technology    1986	*/

#include "Xlibint.h"

XChangeKeyboardControl(dpy, mask, value_list)
    register Display *dpy;
    unsigned long mask;
    XKeyboardControl *value_list;
{
    unsigned long values[7];
    register unsigned long *value = values;
    long nvalues;
    register xChangeKeyboardControlReq *req;

    LockDisplay(dpy);
    GetReq(ChangeKeyboardControl, req);
    req->mask = mask;

    /* Warning!  This code assumes that "unsigned long" is 32-bits wide */

    if (mask & KBKeyClickPercent)
	*value++ = value_list->key_click_percent;
	
    if (mask & KBBellPercent)
    	*value++ = value_list->bell_percent;

    if (mask & KBBellPitch)
    	*value++ = value_list->bell_pitch;

    if (mask & KBBellDuration)
    	*value++ = value_list->bell_duration;

    if (mask & KBLed)
    	*value++ = value_list->led;

    if (mask & KBLedMode)
	*value++ = value_list->led_mode;

    if (mask & KBKey)
        *value++ = value_list->key;

    if (mask & KBAutoRepeatMode)
        *value++ = value_list->auto_repeat_mode;


    req->length += (nvalues = value - values);

    /* note: Data is a macro that uses its arguments multiple
       times, so "nvalues" is changed in a separate assignment
       statement */

    nvalues <<= 2;
    Data32 (dpy, (long *) values, nvalues);
    UnlockDisplay(dpy);
    SyncHandle();

    }
