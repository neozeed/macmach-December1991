/* $XConsortium: XExtInt.c,v 1.10 89/12/13 20:30:17 rws Exp $ */

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
 * Input Extension library internal functions.
 *
 */

#define NEED_EVENTS
#define NEED_REPLIES
#include <stdio.h>
#include "XI.h"
#include "XIproto.h"
#include "Xlibint.h"
#include "XInput.h"
#include "extutil.h"

#define ENQUEUE_EVENT	True
#define DONT_ENQUEUE	False

static	XExtensionInfo *xinput_info;
static	/* const */ char *xinput_extension_name = INAME;
static	int XInputClose();
static  char *XInputError();
static Bool XInputWireToEvent();
Status	XInputEventToWire();
static	/* const */ XEvent	emptyevent;

#define XInputCheckExtension(dpy,i,val) \
  XextCheckExtension (dpy, i, xinput_extension_name, val)

static /* const */ XExtensionHooks xinput_extension_hooks = {
    NULL,				/* create_gc */
    NULL,				/* copy_gc */
    NULL,				/* flush_gc */
    NULL,				/* free_gc */
    NULL,				/* create_font */
    NULL,				/* free_font */
    XInputClose,			/* close_display */
    XInputWireToEvent,			/* wire_to_event */
    XInputEventToWire,			/* event_to_wire */
    NULL,				/* error */
    XInputError,			/* error_string */
};

static char *XInputErrorList[] = {
	"BadDevice, invalid or uninitialized input device", /* BadDevice */
	"BadEvent, invalid event type",			    /* BadEvent	*/	
	"BadMode, invalid mode parameter",		    /* BadMode	*/
	"BadClass, invalid event class",		    /* BadClass	*/	
};

XEXT_GENERATE_FIND_DISPLAY (XInput_find_display, xinput_info, 
	xinput_extension_name, &xinput_extension_hooks, IEVENTS, NULL)

static XEXT_GENERATE_ERROR_STRING (XInputError, xinput_extension_name,
				   IERRORS, XInputErrorList)
/***********************************************************************
 *
 * Check to see if the input extension is installed in the server.
 * Also check to see if the version is >= the requested version.
 *
 */

CheckExtInit(dpy, major_version)
    register	Display *dpy;
    register	int	major_version;
    {
    XExtensionVersion 	*ext;
    XExtDisplayInfo 	*info = XInput_find_display (dpy);

    XInputCheckExtension (dpy, info, -1);

    if (info->data == NULL)
	{
	info->data = (caddr_t) Xmalloc (sizeof (XEvent));
	if (!info->data)
	    return (-1);
	}

    if (major_version > Dont_Check)
	{
	ext = XGetExtensionVersion (dpy, "XInputExtension");
	if (ext->major_version < major_version)
	    {
	    XFree ((char *)ext);
    	    UnlockDisplay(dpy);
	    return (-1);
	    }
	XFree ((char *)ext);
	}
    return (0);
    }

/***********************************************************************
 *
 * Close display routine.
 *
 */

static int
XInputClose (dpy, codes)
    Display *dpy;
    XExtCodes *codes;
    {
    XExtDisplayInfo 	*info = XInput_find_display (dpy);

    XFree(info->data);
    return XextRemoveDisplay (xinput_info, dpy);
    }


int
Ones(mask)  
    Mask mask;
{
    register Mask y;

    y = (mask >> 1) &033333333333;
    y = mask - y - ((y >>1) & 033333333333);
    return (((y + (y >> 3)) & 030707070707) % 077);
}

/***********************************************************************
 *
 * Handle Input extension events.
 * Reformat a wire event into an XEvent structure of the right type.
 *
 */

static Bool
XInputWireToEvent (dpy, re, event)
    Display	*dpy;
    XEvent	*re;
    xEvent	*event;
    {
    xEvent	nevent;
    unsigned	int	type, reltype;
    unsigned	int	ret = 1;
    unsigned	int	i;
    XExtDisplayInfo 	*info = XInput_find_display (dpy);
    XEvent		*save = (XEvent *) info->data;

    type = event->u.u.type & 0x7f;
    reltype = (type - info->codes->first_event);

    if (reltype != XI_DeviceValuator && 
	reltype != XI_DeviceKeystateNotify &&
	reltype != XI_DeviceButtonstateNotify)
	{
	*save = emptyevent;
        save->type = type;
        ((XAnyEvent *)save)->serial = _XSetLastRequestRead(dpy,
	    (xGenericReply *)event);
        ((XAnyEvent *)save)->send_event = ((event->u.u.type & 0x80) != 0);
        ((XAnyEvent *)save)->display = dpy;
	}
	
    switch (reltype)
	{
	case XI_DeviceMotionNotify:
	    {
	    register XDeviceMotionEvent *ev = (XDeviceMotionEvent*) save;
	    deviceKeyButtonPointer *ev2 = (deviceKeyButtonPointer *) event;

	    ev->root 		= ev2->root;
	    ev->window 		= ev2->event;
	    ev->subwindow 	= ev2->child;
	    ev->time 		= ev2->time;
	    ev->x_root 		= ev2->root_x;
	    ev->y_root 		= ev2->root_y;
	    ev->x 		= ev2->event_x;
	    ev->y 		= ev2->event_y;
	    ev->state		= ev2->state;
	    ev->same_screen	= ev2->same_screen;
	    ev->is_hint 	= ev2->detail;
	    ev->deviceid	= ev2->deviceid & DEVICE_BITS;
    	    return (DONT_ENQUEUE);
	    break;
	    }
	case XI_DeviceKeyPress:
	case XI_DeviceKeyRelease:
	    {
	    register XDeviceKeyEvent *ev = (XDeviceKeyEvent*) save;
	    deviceKeyButtonPointer *ev2 = (deviceKeyButtonPointer *) event;

	    ev->root 		= ev2->root;
	    ev->window 		= ev2->event;
	    ev->subwindow 	= ev2->child;
	    ev->time 		= ev2->time;
	    ev->x_root 		= ev2->root_x;
	    ev->y_root 		= ev2->root_y;
	    ev->x 		= ev2->event_x;
	    ev->y 		= ev2->event_y;
	    ev->state		= ev2->state;
	    ev->same_screen	= ev2->same_screen;
	    ev->keycode 	= ev2->detail;
	    ev->deviceid	= ev2->deviceid & DEVICE_BITS;
	    if (ev2->deviceid & MORE_EVENTS)
		return (DONT_ENQUEUE);
	    else
		{
		*re = *save;
		return (ENQUEUE_EVENT);
		}
	    break;
	    }
	case XI_DeviceButtonPress:
	case XI_DeviceButtonRelease:
	    {
	    register XDeviceButtonEvent *ev = (XDeviceButtonEvent*) save;
	    deviceKeyButtonPointer *ev2 = (deviceKeyButtonPointer *) event;

	    ev->root 		= ev2->root;
	    ev->window 		= ev2->event;
	    ev->subwindow 	= ev2->child;
	    ev->time 		= ev2->time;
	    ev->x_root 		= ev2->root_x;
	    ev->y_root 		= ev2->root_y;
	    ev->x 		= ev2->event_x;
	    ev->y 		= ev2->event_y;
	    ev->state		= ev2->state;
	    ev->same_screen	= ev2->same_screen;
	    ev->button  	= ev2->detail;
	    ev->deviceid	= ev2->deviceid & DEVICE_BITS;
	    if (ev2->deviceid & MORE_EVENTS)
		return (DONT_ENQUEUE);
	    else
		{
		*re = *save;
		return (ENQUEUE_EVENT);
		}
	    break;
	    }
	case XI_ProximityIn:
	case XI_ProximityOut:
	    {
	    register XProximityNotifyEvent *ev = 
		(XProximityNotifyEvent *) save;
	    deviceKeyButtonPointer *ev2 = (deviceKeyButtonPointer *) event;

	    ev->root 		= ev2->root;
	    ev->window 		= ev2->event;
	    ev->subwindow 	= ev2->child;
	    ev->time 		= ev2->time;
	    ev->x_root 		= ev2->root_x;
	    ev->y_root 		= ev2->root_y;
	    ev->x 		= ev2->event_x;
	    ev->y 		= ev2->event_y;
	    ev->state		= ev2->state;
	    ev->same_screen	= ev2->same_screen;
	    ev->deviceid	= ev2->deviceid & DEVICE_BITS;
	    if (ev2->deviceid & MORE_EVENTS)
		return (DONT_ENQUEUE);
	    else
		{
		*re = *save;
		return (ENQUEUE_EVENT);
		}
	    break;
	    }
	case XI_DeviceValuator:
	    {
	    deviceValuator *xev = (deviceValuator *) event;
	    int save_type = save->type - info->codes->first_event;

	    if (save_type == XI_DeviceKeyPress ||
	        save_type == XI_DeviceKeyRelease)
		{
	        XDeviceKeyEvent *kev = (XDeviceKeyEvent*) save;
		kev->device_state = xev->device_state;
		if (kev->axes_count == 0)
		    {
		    kev->axes_count = xev->num_valuators;
		    kev->first_axis = xev->first_valuator;
		    }
		else if (kev->axes_count + xev->num_valuators <= 8)
		    kev->axes_count += xev->num_valuators;
		for (i=0; i<xev->num_valuators; i++)
		    kev->axis_data[xev->first_valuator+i] = xev->valuators[i];
		}
	    else if (save_type == XI_DeviceButtonPress ||
	        save_type == XI_DeviceButtonRelease)
		{
	        XDeviceButtonEvent *bev = (XDeviceButtonEvent*) save;
		bev->device_state = xev->device_state;
		if (bev->axes_count == 0)
		    {
		    bev->axes_count = xev->num_valuators;
		    bev->first_axis = xev->first_valuator;
		    }
		else if (bev->axes_count + xev->num_valuators <= 8)
		    bev->axes_count += xev->num_valuators;
		for (i=0; i<xev->num_valuators; i++)
		    bev->axis_data[xev->first_valuator+i] = xev->valuators[i];
		for (i=0; i<xev->num_valuators; i++)
		    bev->axis_data[xev->first_valuator+i] = xev->valuators[i];
		}
	    else if (save_type == XI_DeviceMotionNotify) 
		{
	        XDeviceMotionEvent *mev = (XDeviceMotionEvent*) save;
		mev->device_state = xev->device_state;
		if (mev->axes_count == 0)
		    {
		    mev->axes_count = xev->num_valuators;
		    mev->first_axis = xev->first_valuator;
		    }
		else if (mev->axes_count + xev->num_valuators <= 8)
		    mev->axes_count += xev->num_valuators;
		for (i=0; i<xev->num_valuators; i++)
		    mev->axis_data[xev->first_valuator+i] = xev->valuators[i];
		for (i=0; i<xev->num_valuators; i++)
		    mev->axis_data[xev->first_valuator+i] = xev->valuators[i];
		}
	    else if (save_type == XI_ProximityIn ||
	        save_type == XI_ProximityOut)
		{
	        XProximityNotifyEvent *pev = 
			(XProximityNotifyEvent*) save;
		pev->device_state = xev->device_state;
		if (pev->axes_count == 0)
		    {
		    pev->axes_count = xev->num_valuators;
		    pev->first_axis = xev->first_valuator;
		    }
		else if (pev->axes_count + xev->num_valuators <= 8)
		    pev->axes_count += xev->num_valuators;
		for (i=0; i<xev->num_valuators; i++)
		    pev->axis_data[xev->first_valuator+i] = xev->valuators[i];
		}
	    else if (save_type == XI_DeviceStateNotify)
		{
	        XDeviceStateNotifyEvent *sev = 
			(XDeviceStateNotifyEvent*) save;
		XInputClass *any = (XInputClass *) &sev->data[0];
		XValuatorStatus *v;

		for (i=0; i<sev->num_classes; i++)
		    if (any->class != ValuatorClass)
			any = (XInputClass *) ((char *) any + any->length);
		v = (XValuatorStatus *) any;
		if (v->num_valuators + xev->num_valuators <= 6)
		    v->num_valuators += xev->num_valuators;
		for (i=0; i<xev->num_valuators; i++)
		    v->valuators[xev->first_valuator+i] = xev->valuators[i];

		}
	    if (xev->deviceid & MORE_EVENTS)
		return (DONT_ENQUEUE);
	    else
		{
		*re = *save;
		return (ENQUEUE_EVENT);
		}
	    break;
	    }
	case XI_DeviceFocusIn:
	case XI_DeviceFocusOut:
	    {
	    register XDeviceFocusChangeEvent *ev = 
		(XDeviceFocusChangeEvent *) re;
	    deviceFocus *fev = (deviceFocus *) event;

	    *ev			= *((XDeviceFocusChangeEvent *) save);
	    ev->window 		= fev->window;
	    ev->time   		= fev->time;
	    ev->mode		= fev->mode;
	    ev->detail		= fev->detail;
	    ev->deviceid 		= fev->deviceid & DEVICE_BITS;
    	    return (ENQUEUE_EVENT);
	    break;
	    }
	case XI_DeviceStateNotify:
	    {
	    XDeviceStateNotifyEvent *stev = 
		(XDeviceStateNotifyEvent *) save;
	    deviceStateNotify *sev = (deviceStateNotify *) event;
	    XKeyStatus *kstev;
	    XButtonStatus *bev;
	    XValuatorStatus *vev;
	    char *data;

	    stev->window 		= dpy->current;
	    stev->deviceid 		= sev->deviceid & DEVICE_BITS;
	    stev->time     		= sev->time;
	    stev->num_classes	 	= Ones (sev->classes_reported);
 	    data = (char *) &stev->data[0];
	    if (sev->classes_reported & (1 << KeyClass))
	        {
	        register XKeyStatus *kstev = (XKeyStatus *) data;
	        kstev->class = KeyClass;
	        kstev->length = sizeof (XKeyStatus);
	        kstev->num_keys = 32;
	        bcopy ((char *) &sev->keys[0], (char *) &kstev->keys[0], 4);
	        data += sizeof (XKeyStatus);
	        }
	    if (sev->classes_reported & (1 << ButtonClass))
	        {
	        register XButtonStatus *bev = (XButtonStatus *) data;
	        bev->class = ButtonClass;
	        bev->length = sizeof (XButtonStatus);
	        bev->num_buttons = 32;
	        bcopy ((char *) sev->buttons, (char *) bev->buttons, 4);
	        data += sizeof (XButtonStatus);
	        }
	    if (sev->classes_reported & (1 << ValuatorClass))
	        {
	        register XValuatorStatus *vev = (XValuatorStatus *) data;
	        vev->class = ValuatorClass;
	        vev->length = sizeof (XValuatorStatus);
	        vev->num_valuators = 3;
	        for (i=0; i<3; i++)
		    vev->valuators[i] = sev->valuators[i];
	        data += sizeof (XValuatorStatus);
	        }
    	    if (sev->deviceid & MORE_EVENTS)
	        return (DONT_ENQUEUE);
	    else
	        {
	        *re = *save;
	        stev = (XDeviceStateNotifyEvent *) re;
	        return (ENQUEUE_EVENT);
	        }
	    break;
	    }
	case XI_DeviceKeystateNotify:
	    {
	    int i;
	    XInputClass *anyclass;
	    register XKeyStatus *kv;
	    deviceKeyStateNotify *ksev = (deviceKeyStateNotify *) event;
	    XDeviceStateNotifyEvent *kstev = 
		(XDeviceStateNotifyEvent *) save;

	    anyclass = (XInputClass *) &kstev->data[0];
	    for (i=0; i<kstev->num_classes; i++)
	        if (anyclass->class == KeyClass)
		    break;
	        else 
		    anyclass = (XInputClass *) ((char *) anyclass + 
			anyclass->length);
	
	    kv = (XKeyStatus *) anyclass;
	    kv->num_keys = 256;
	    bcopy ((char *) ksev->keys, (char *) &kv->keys[4], 28);
    	    if (ksev->deviceid & MORE_EVENTS)
	        return (DONT_ENQUEUE);
	    else
	        {
	        *re = *save;
	        kstev = (XDeviceStateNotifyEvent *) re;
	        return (ENQUEUE_EVENT);
	        }
	    break;
	    }
	case XI_DeviceButtonstateNotify:
	    {
	    int i;
	    XInputClass *anyclass;
	    register XButtonStatus *bv;
	    deviceButtonStateNotify *bsev = (deviceButtonStateNotify *) event;
	    XDeviceStateNotifyEvent *bstev = 
		(XDeviceStateNotifyEvent *) save;


	    anyclass = (XInputClass *) &bstev->data[0];
	    for (i=0; i<bstev->num_classes; i++)
	        if (anyclass->class == ButtonClass)
		    break;
	        else 
		    anyclass = (XInputClass *) ((char *) anyclass + 
			anyclass->length);
	
	    bv = (XButtonStatus *) anyclass;
	    bv->num_buttons = 256;
	    bcopy ((char *) bsev->buttons, (char *) &bv->buttons[4], 28);
    	    if (bsev->deviceid & MORE_EVENTS)
	        return (DONT_ENQUEUE);
	    else
	        {
	        *re = *save;
	        bstev = (XDeviceStateNotifyEvent *) re;
	        return (ENQUEUE_EVENT);
	        }
	    break;
	    }
	case XI_DeviceMappingNotify:
	    {
	    register XDeviceMappingEvent *ev = (XDeviceMappingEvent *) re;
	    deviceMappingNotify *ev2 = (deviceMappingNotify *) event;

	    *ev			= *((XDeviceMappingEvent *) save);
	    ev->first_keycode 	= ev2->firstKeyCode;
	    ev->request 		= ev2->request;
	    ev->count 		= ev2->count;
	    ev->time  		= ev2->time;
	    ev->deviceid 		= ev2->deviceid & DEVICE_BITS;
    	    return (ENQUEUE_EVENT);
	    }
	case XI_ChangeDeviceNotify:
	    {
	    register XChangeDeviceNotifyEvent *ev = 
		(XChangeDeviceNotifyEvent *) re;
	    changeDeviceNotify *ev2 = (changeDeviceNotify *) event;

	    *ev			= *((XChangeDeviceNotifyEvent *) save);
	    ev->request 		= ev2->request;
	    ev->time  		= ev2->time;
	    ev->deviceid 		= ev2->deviceid & DEVICE_BITS;
    	    return (ENQUEUE_EVENT);
	    }
	default:
	    printf ("XInputWireToEvent: UNKNOWN WIRE EVENT! type=%d\n",type);
	    break;
	}

    return (DONT_ENQUEUE);
    }
