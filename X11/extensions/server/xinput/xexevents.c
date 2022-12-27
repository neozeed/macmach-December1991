/* $XConsortium: xexevents.c,v 1.8 89/12/02 15:20:43 rws Exp $ */
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
 *  Routines to register and initialize extension input devices.
 *  This also contains ProcessOtherEvent, the routine called from DDX
 *  to route extension events.
 *
 */

#define	 NEED_EVENTS
#include "X.h"
#include "Xproto.h"
#include "XI.h"
#include "XIproto.h"
#include "inputstr.h"
#include "windowstr.h"
#include "miscstruct.h"
#include "region.h"

#define WID(w) ((w) ? ((w)->drawable.id) : 0)
#define AllModifiersMask ( \
	ShiftMask | LockMask | ControlMask | Mod1Mask | Mod2Mask | \
	Mod3Mask | Mod4Mask | Mod5Mask )
#define AllButtonsMask ( \
	Button1Mask | Button2Mask | Button3Mask | Button4Mask | Button5Mask )
#define Motion_Filter(class) (DevicePointerMotionMask | \
			      (class)->state | (class)->motionMask)

void 			ActivateKeyboardGrab();
void 			DeactivateKeyboardGrab();
void 			ProcessOtherEvent();
void 			RecalculateDeviceDeliverableEvents();
extern int		DeviceKeyPress;
extern int		DeviceButtonPress;
extern int		DeviceValuator;
extern Mask 		DevicePointerMotionMask;
extern Mask 		DeviceMappingNotifyMask;
extern Mask 		DeviceButton1Mask;
extern Mask 		DeviceButtonMotionMask;
extern Mask 		DeviceButtonGrabMask;
extern Mask 		DeviceOwnerGrabButtonMask;
extern Mask		PropagateMask[];
extern WindowPtr 	GetSpriteWindow();
extern InputInfo	inputInfo;

/**************************************************************************
 *
 * Procedures for extension device event routing.
 *
 */

void
RegisterOtherDevice (device)
    DevicePtr device;
    {
    device->processInputProc = ProcessOtherEvent;
    device->realInputProc = ProcessOtherEvent;
    ((DeviceIntPtr)device)->ActivateGrab = ActivateKeyboardGrab;
    ((DeviceIntPtr)device)->DeactivateGrab = DeactivateKeyboardGrab;
    }

/*ARGSUSED*/
void
ProcessOtherEvent (xE, other, count)
    deviceKeyButtonPointer *xE;
    register DeviceIntPtr other;
    int count;
    {
    extern	int	DeviceKeyRelease;
    extern	int	DeviceButtonRelease;
    extern	int	DeviceMotionNotify;
    extern	int	ProximityIn;
    extern	int	ProximityOut;
    register BYTE   	*kptr;
    register int    	i;
    register CARD16 	modifiers;
    register CARD16 	mask;
    GrabPtr         	grab = other->grab;
    Bool            	deactivateDeviceGrab = FALSE;
    int             	key, bit;
    ButtonClassPtr	b = other->button;
    KeyClassPtr		k = other->key;
    void		NoticeEventTime();
    deviceValuator	*xV = (deviceValuator *) xE;

    key = xE->detail;
    NoticeEventTime(xE);
    xE->state = inputInfo.keyboard->key->state | 
		inputInfo.pointer->button->state;
    for (i=1; i<count; i++)
	if ((++xV)->type == DeviceValuator)
	    {
	    xV->device_state = 0;
	    if (k)
		xV->device_state |= k->state;
	    if (b)
	        xV->device_state |= b->state;
	    }
    bit = 1 << (key & 7);
    modifiers = other->key->modifierMap[key];
    
    if (xE->type == DeviceKeyPress)
	{
        kptr = &k->down[key >> 3];
	if (*kptr & bit) /* allow ddx to generate multiple downs */
	    {   
	    if (!modifiers)
		{
		xE->type = DeviceKeyRelease;
		ProcessOtherEvent(xE, other, count);
		xE->type = DeviceKeyPress;
		/* release can have side effects, don't fall through */
		ProcessOtherEvent(xE, other, count);
		}
	    return;
	    }
	if (other->valuator)
	    other->valuator->motionHintWindow = NullWindow;
	*kptr |= bit;
	for (i = 0, mask = 1; modifiers; i++, mask <<= 1)
	    {
	    if (mask & modifiers) 
	        {
		/* This key affects modifier "i" */
		k->modifierKeyCount[i]++;
		k->state |= mask;
		modifiers &= ~mask;
		}
	    }
	if (!grab && CheckDeviceGrabs(other, xE, 0, count))
	    return;
	}
    else if (xE->type == DeviceKeyRelease)
	{
        kptr = &k->down[key >> 3];
	if (!(*kptr & bit)) /* guard against duplicates */
	    return;
	if (other->valuator)
	    other->valuator->motionHintWindow = NullWindow;
	*kptr &= ~bit;
	for (i = 0, mask = 1; modifiers; i++, mask <<= 1)
	    {
	    if (mask & modifiers) 
	        {
		/* This key affects modifier "i" */
		if (--k->modifierKeyCount[i] <= 0) 
		    {
		    k->modifierKeyCount[i] = 0;
		    k->state &= ~mask;
		    }
		modifiers &= ~mask;
		}
	    }

	/* the dix code uses a variable keyThatActivatedPassiveGrab
	   for comparison.  But if it activated the passive grab, that
	   key should be the one in the grabrec.		
	   However, AnyKey is a special case			*/

	if ((other->fromPassiveGrab) &&
	     (key == other->grab->detail.exact ||
	     other->grab->detail.exact == AnyKey))
	    deactivateDeviceGrab = TRUE;
	}
    else if (xE->type == DeviceButtonPress)
	{
        kptr = &b->down[key >> 3];
	*kptr |= bit;
	if (other->valuator)
	    other->valuator->motionHintWindow = NullWindow;
	b->buttonsDown++;
	b->motionMask = DeviceButtonMotionMask;
	xE->detail = b->map[key];
	if (xE->detail == 0)
	     return;
	if (xE->detail <= 5)
	    b->state |= (DeviceButton1Mask >> 1) << xE->detail;
	SetMaskForEvent(Motion_Filter(b),DeviceMotionNotify);
	if (!grab)
	    if (CheckDeviceGrabs(other, xE, 0, count))
		return;

	}
    else if (xE->type == DeviceButtonRelease)
	{
        kptr = &b->down[key >> 3];
	*kptr &= ~bit;
	if (other->valuator)
	    other->valuator->motionHintWindow = NullWindow;
	if (!--b->buttonsDown)
		b->motionMask = 0;
	xE->detail = b->map[key];
	if (xE->detail == 0)
	    return;
	if (xE->detail <= 5)
	    b->state &= ~((DeviceButton1Mask >> 1) << xE->detail);
	SetMaskForEvent(Motion_Filter(b),DeviceMotionNotify);
	if (!b->state && other->fromPassiveGrab)
	    deactivateDeviceGrab = TRUE;
	}

    if (grab)
	DeliverGrabbedEvent(xE, other, deactivateDeviceGrab, count);
    else
	DeliverFocusedEvent(other, xE, GetSpriteWindow(), count);

    if (deactivateDeviceGrab == TRUE)
	{
        (*other->DeactivateGrab)(other);
        deactivateDeviceGrab = FALSE;
	}
    }

InitProximityClassDeviceStruct(dev)
    DeviceIntPtr dev;
    {
    register ProximityClassPtr proxc;

    proxc = (ProximityClassPtr)xalloc(sizeof(ProximityClassRec));
    if (!proxc)
	return FALSE;
    dev->proximity = proxc;
    return TRUE;
    }

InitValuatorAxisStruct(dev, axnum, minval, maxval, resolution)
    DeviceIntPtr dev;
    int axnum;
    int minval;
    int maxval;
    int resolution;
    {
    register XAxisInfoPtr ax = dev->valuator->axes + axnum;

    ax->min_value = minval;
    ax->max_value = maxval;
    ax->resolution = resolution;
    }

DeviceFocusEvent(dev, type, mode, detail, pWin)
    DeviceIntPtr dev;
    int type, mode, detail;
    register WindowPtr pWin;
    {
    extern      int     DeviceFocusIn;
    extern      int     DeviceFocusOut;
    extern      int     DeviceStateNotify;
    extern      int     DeviceKeyStateNotify;
    extern      int     DeviceButtonStateNotify;
    extern      int     DeviceValuatorStateNotify;
    extern      Mask    DeviceStateNotifyMask;
    extern      Mask    DeviceFocusChangeMask;
    deviceFocus	event;

    if (type == FocusIn)
	type = DeviceFocusIn;
    else
	type = DeviceFocusOut;

    event.deviceid = dev->id;
    event.mode = mode;
    event.type = type;
    event.detail = detail;
    event.window = pWin->drawable.id;
    event.time = currentTime.milliseconds;

    (void)
    DeliverEventsToWindow(pWin, &event, 1, DeviceFocusChangeMask, NullGrab, 
	dev->id);

    if ((type == DeviceFocusIn) && 
	(wOtherInputMasks(pWin)) &&
	(wOtherInputMasks(pWin)->inputEvents[dev->id] & DeviceStateNotifyMask))
        {
	int			i,j;
	int 			evcount = 1;
	Mask			classmask = 0;
	deviceStateNotify 	*ev, *sev;
	deviceKeyStateNotify 	*kev;
	deviceButtonStateNotify *bev;
	deviceValuator 		*vev;

	KeyClassPtr k;
	ButtonClassPtr b;
	ValuatorClassPtr v;

	if ((k=dev->key) != NULL)
	    {
	    classmask |= (1 << KeyClass);
	    if (k->curKeySyms.maxKeyCode > 32)
		evcount++;
	    }
	if ((b=dev->button) != NULL)
	    {
	    classmask |= (1 << ButtonClass);
	    if (b->numButtons > 32)
		evcount++;
	    }
	if ((v=dev->valuator) != NULL)
	    {
	    classmask |= (1 << ValuatorClass);
	    if (v->numAxes > 4)
		evcount += (v->numAxes - 4) / 7;
	    }

	ev = (deviceStateNotify *) xalloc(evcount * sizeof(xEvent));

	ev->type = DeviceStateNotify;
	ev->deviceid = dev->id;
        ev->time = currentTime.milliseconds;
	ev->classes_reported = classmask;
	sev = ev;

	if (k != NULL)
	    {
	    bcopy((char *) k->down, (char *) &sev->keys[0], 4);
	    if (k->curKeySyms.maxKeyCode > 32)
		{
		ev->deviceid |= MORE_EVENTS;
		kev = (deviceKeyStateNotify *) ++ev; 
		kev->type = DeviceKeyStateNotify;
		kev->deviceid = dev->id;
		bcopy((char *) &k->down[4], (char *) &kev->keys[0], 28);
		}
	    }
	if (b != NULL)
	    {
	    bcopy((char *) b->down, (char *) &sev->buttons[0], 4);
	    if (b->numButtons > 32)
		{
		ev->deviceid |= MORE_EVENTS;
		bev = (deviceButtonStateNotify *) ++ev; 
		bev->type = DeviceButtonStateNotify;
		bev->deviceid = dev->id;
		bcopy((char *) &b->down[4], (char *) &bev->buttons[0], 28);
		}
	    }
	if (v != NULL)
	    {
	    for (i=0; i<3 && i<v->numAxes; i++)
		sev->valuators[i] = v->axisVal[i]; 
	    if (v->numAxes > 3)
		{
		for (i=4; i<=v->numAxes; i+=7)
		    {
		    ev->deviceid |= MORE_EVENTS;
		    vev = (deviceValuator *) ++ev; 
		    vev->type = DeviceValuator;
		    vev->deviceid = dev->id;
		    vev->num_valuators = v->numAxes;
		    vev->first_valuator = v->numAxes;
		    for (j=0; j<7; j++)
		        vev->valuators[j] = v->axisVal[i*7+j+3]; 
		    }
		}
	    }

	(void) DeliverEventsToWindow(pWin, sev, evcount, DeviceStateNotifyMask, 
	    NullGrab, dev->id);
        }
    }

int
GrabButton(client, dev, this_device_mode, other_devices_mode, modifiers,
	modifier_device, button, grabWindow, ownerEvents, rcursor, rconfineTo,
	eventMask)
    ClientPtr client;
    DeviceIntPtr dev;
    BYTE this_device_mode;
    BYTE other_devices_mode;
    CARD16 modifiers;
    DeviceIntPtr modifier_device;
    CARD8 button;
    Window grabWindow;
    BOOL ownerEvents;
    Cursor rcursor;
    Window rconfineTo;
    Mask eventMask[];
   
{
    WindowPtr pWin, confineTo;
    CursorPtr cursor;
    GrabPtr CreateGrab();
    GrabPtr grab;

    if ((this_device_mode != GrabModeSync) &&
	(this_device_mode != GrabModeAsync))
    {
	client->errorValue = this_device_mode;
        return BadValue;
    }
    if ((other_devices_mode != GrabModeSync) &&
	(other_devices_mode != GrabModeAsync))
    {
	client->errorValue = other_devices_mode;
        return BadValue;
    }
    if ((modifiers != AnyModifier) &&
	(modifiers & ~AllModifiersMask))
    {
	client->errorValue = modifiers;
	return BadValue;
    }
    if ((ownerEvents != xFalse) && (ownerEvents != xTrue))
    {
	client->errorValue = ownerEvents;
	return BadValue;
    }
    pWin = LookupWindow(grabWindow, client);
    if (!pWin)
	return BadWindow;
    if (rconfineTo == None)
	confineTo = NullWindow;
    else
    {
	confineTo = LookupWindow(rconfineTo, client);
	if (!confineTo)
	    return BadWindow;
    }
    if (rcursor == None)
	cursor = NullCursor;
    else
    {
	cursor = (CursorPtr)LookupIDByType(rcursor, RT_CURSOR);
	if (!cursor)
	{
	    client->errorValue = rcursor;
	    return BadCursor;
	}
    }

    grab = CreateGrab(client->index, dev, pWin, eventMask,
	(Bool)ownerEvents, (Bool) other_devices_mode, (Bool)this_device_mode,
	modifier_device, modifiers, DeviceButtonPress, button, confineTo, 
	cursor);
    if (!grab)
	return BadAlloc;
    return AddPassiveGrabToList(grab);
    }

int
GrabKey(client, dev, this_device_mode, other_devices_mode, modifiers,
    modifier_device, key, grabWindow, ownerEvents, mask)
    ClientPtr client;
    DeviceIntPtr dev;
    BYTE this_device_mode;
    BYTE other_devices_mode;
    CARD16 modifiers;
    DeviceIntPtr modifier_device;
    CARD8 key;
    Window grabWindow;
    BOOL ownerEvents;
    Mask mask[];
   
{
    WindowPtr pWin;
    GrabPtr CreateGrab();
    GrabPtr grab;
    KeyClassPtr k = dev->key;

    if (k==NULL)
	return BadMatch;
    if ((other_devices_mode != GrabModeSync) &&
	(other_devices_mode != GrabModeAsync))
    {
	client->errorValue = other_devices_mode;
        return BadValue;
    }
    if ((this_device_mode != GrabModeSync) &&
	(this_device_mode != GrabModeAsync))
    {
	client->errorValue = this_device_mode;
        return BadValue;
    }
    if (((key > k->curKeySyms.maxKeyCode) || 
	 (key < k->curKeySyms.minKeyCode))
	&& (key != AnyKey))
    {
	client->errorValue = key;
        return BadValue;
    }
    if ((modifiers != AnyModifier) &&
	(modifiers & ~AllModifiersMask))
    {
	client->errorValue = modifiers;
	return BadValue;
    }
    pWin = LookupWindow(grabWindow, client);
    if (!pWin)
	return BadWindow;

    grab = CreateGrab(client->index, dev, pWin, 
	mask, ownerEvents, this_device_mode, other_devices_mode, 
	modifier_device, modifiers, DeviceKeyPress, key, NullWindow, 
	NullCursor);
    if (!grab)
	return BadAlloc;
    return AddPassiveGrabToList(grab);
    }

int
SelectForWindow(dev, pWin, client, mask, exclusivemasks, validmasks)
	DeviceIntPtr dev;
	WindowPtr pWin;
	ClientPtr client;
	Mask mask;
	Mask exclusivemasks;
	Mask validmasks;
{
    int mskidx = dev->id;
    int i;
    Mask check;
    InputClientsPtr others;
    extern Mask DevicePointerMotionHintMask;
    extern int RT_INPUTCLIENT;

    if (mask & ~validmasks)
    {
	client->errorValue = mask;
	return BadValue;
    }
    check = (mask & exclusivemasks);
    if (wOtherInputMasks(pWin))
	{
	if (check & wOtherInputMasks(pWin)->inputEvents[mskidx])
	    {			       /* It is illegal for two different
				          clients to select on any of the
				          events for maskcheck. However,
				          it is OK, for some client to
				          continue selecting on one of those
				          events.  */
	    for (others = wOtherInputMasks(pWin)->inputClients; others; 
		others = others->next)
	        {
	        if (!SameClient(others, client) && (check & 
		    others->mask[mskidx]))
		    return BadAccess;
	        }
            }
	for (others = wOtherInputMasks(pWin)->inputClients; others; 
		others = others->next)
	    {
	    if (SameClient(others, client))
	        {
		check = others->mask[mskidx];
		if (mask == 0)
		    {
		    for (i=0; i<EMASKSIZE; i++)
			if (i != mskidx && others->mask[i] != 0)
			    break;
		    if (i == EMASKSIZE)
			{
		        FreeResource(others->resource, RT_NONE);
		        return Success;
			}
		    }
		else
		    others->mask[mskidx] = mask;
		goto maskSet;
	        }
	    }
	}
    check = 0;
    if (!pWin->optional && !MakeWindowOptional (pWin))
	return BadAlloc;
    others = (InputClients *) xalloc(sizeof(InputClients));
    if (!others)
	return BadAlloc;
    if (!pWin->optional->inputMasks && !MakeInputMasks (pWin))
	return BadAlloc;
    bzero((char *) &others->mask[0], sizeof(Mask)*EMASKSIZE);
    others->mask[mskidx] = mask;
    others->resource = FakeClientID(client->index);
    others->next = pWin->optional->inputMasks->inputClients;
    pWin->optional->inputMasks->inputClients = others;
    if (!AddResource(others->resource, RT_INPUTCLIENT, (pointer)pWin))
	return BadAlloc;
maskSet: 
    if ((dev->valuator->motionHintWindow == pWin) &&
	(mask & DevicePointerMotionHintMask) &&
	!(check & DevicePointerMotionHintMask) &&
	!dev->grab)
	dev->valuator->motionHintWindow = NullWindow;
    RecalculateDeviceDeliverableEvents(pWin);
    return Success;
}

Bool
MakeInputMasks (pWin)
    WindowPtr	pWin;
    {
    struct _OtherInputMasks *imasks;

    if (pWin->optional->inputMasks)
	return TRUE;
    imasks = (struct _OtherInputMasks *) 
	xalloc (sizeof (struct _OtherInputMasks));
    if (!imasks)
	return FALSE;
    bzero((char *) imasks, sizeof (struct _OtherInputMasks));
    pWin->optional->inputMasks = imasks;
    return TRUE;
    }

void
RecalculateDeviceDeliverableEvents(pWin)
    WindowPtr pWin;
    {
    register InputClientsPtr others;
    struct _OtherInputMasks *inputMasks;   /* default: NULL */
    register WindowPtr pChild;
    int i;

    pChild = pWin;
    while (1)
	{
	if (wOtherInputMasks(pChild))
	    {
	    for (others = wOtherInputMasks(pChild)->inputClients; others; 
		others = others->next)
		{
		for (i=0; i<EMASKSIZE; i++)
		    if (others->mask[i])
			wOtherInputMasks(pChild)->inputEvents[i] |= 
				others->mask[i];
		}
	    for (i=0; i<EMASKSIZE; i++)
		wOtherInputMasks(pChild)->deliverableEvents[i] =
		    wOtherInputMasks(pChild)->inputEvents[i];
	    if (pChild->parent)
		for (i=0; i<EMASKSIZE; i++)
		    wOtherInputMasks(pChild)->deliverableEvents[i] |=
			(wOtherInputMasks(pChild->parent)->deliverableEvents[i] &
			~wOtherInputMasks(pChild)->dontPropagateMask[i] &
			PropagateMask[i]);
	    }
	if (pChild->firstChild)
	    {
	    pChild = pChild->firstChild;
	    continue;
	    }
	while (!pChild->nextSib && (pChild != pWin))
	    pChild = pChild->parent;
	if (pChild == pWin)
	    break;
	pChild = pChild->nextSib;
	}
    }

int
InputClientGone(pWin, id)
    register WindowPtr pWin;
    XID   id;
    {
    register InputClientsPtr other, prev;

    if (!wOtherInputMasks(pWin))
	return(Success);
    prev = 0;
    for (other = wOtherInputMasks(pWin)->inputClients; other; 
	other = other->next)
	{
	if (other->resource == id)
	    {
	    if (prev)
		prev->next = other->next;
	    else
	        {
		if (!(wOtherInputMasks(pWin)->inputClients = other->next))
		    {
		    xfree(wOtherInputMasks(pWin));
		    pWin->optional->inputMasks = (OtherInputMasks *) NULL;
		    CheckWindowOptionalNeed (pWin);
		    }
	        }
	    xfree(other);
	    RecalculateDeviceDeliverableEvents(pWin);
	    return(Success);
	    }
	prev = other;
        }
    FatalError("client not on device event list");
    /*NOTREACHED*/
    }

int
SendEvent (client, d, dest, propagate, ev, mask, count)
    ClientPtr		client;
    DeviceIntPtr	d;
    Window		dest;
    Bool		propagate;
    xEvent		*ev;
    Mask		mask;
    {
    WindowPtr pWin;
    WindowPtr effectiveFocus = NullWindow; /* only set if dest==InputFocus */
    WindowPtr GetCurrentRootWindow();
    WindowPtr spriteWin=GetSpriteWindow();

    if (dest == PointerWindow)
	pWin = spriteWin;
    else if (dest == InputFocus)
    {
	WindowPtr inputFocus = d->focus->win;

	if (inputFocus == NoneWin)
	    return Success;

	/* If the input focus is PointerRootWin, send the event to where
	the pointer is if possible, then perhaps propogate up to root. */
   	if (inputFocus == PointerRootWin)
	    inputFocus = GetCurrentRootWindow();

	if (IsParent(inputFocus, spriteWin))
	{
	    effectiveFocus = inputFocus;
	    pWin = spriteWin;
	}
	else
	    effectiveFocus = pWin = inputFocus;
    }
    else
	pWin = LookupWindow(dest, client);
    if (!pWin)
	return BadWindow;
    if ((propagate != xFalse) && (propagate != xTrue))
    {
	client->errorValue = propagate;
	return BadValue;
    }
    ev->u.u.type |= 0x80;
    if (propagate && wOtherInputMasks(pWin))
    {
	for (;pWin; pWin = pWin->parent)
	{
	    if (DeliverEventsToWindow( pWin, ev, count, mask, NullGrab, d->id))
		return Success;
	    if (pWin == effectiveFocus)
		return Success;
	    mask &= ~wOtherInputMasks(pWin)->dontPropagateMask[d->id];
	}
    }
    else
	(void)(DeliverEventsToWindow( pWin, ev, count, mask, NullGrab, d->id));
    return Success;
    }

int
SetButtonMapping (client, dev, nElts, map)
    ClientPtr client;
    DeviceIntPtr dev;
    int nElts;
    BYTE *map;
    {
    register int i;
    ButtonClassPtr b = dev->button;

    if (b == NULL)
	return BadMatch;

    if (nElts != b->numButtons)
    {
	client->errorValue = nElts;
	return BadValue;
    }
    if (BadDeviceMap(&map[0], nElts, 1, 255, &client->errorValue))
	return BadValue;
    for (i=0; i < nElts; i++)
	if ((b->map[i + 1] != map[i]) &&
		BitIsOn(b->down, i + 1))
    	    return MappingBusy;
    for (i = 0; i < nElts; i++)
	b->map[i + 1] = map[i];
    return Success;
    }

int 
SetModifierMapping(client, dev, len, rlen, numKeyPerModifier, inputMap, k)
    ClientPtr client;
    DeviceIntPtr dev;
    int len;
    int rlen;
    int numKeyPerModifier;
    KeyCode *inputMap;
    KeyClassPtr *k;
{
    KeyCode *map;
    int inputMapLen;
    register int i;
    
    *k = dev->key;
    if (*k == NULL)
	return BadMatch;
    if (len != ((numKeyPerModifier<<1) + rlen))
	return BadLength;

    inputMapLen = 8*numKeyPerModifier;

    /*
     *	Now enforce the restriction that "all of the non-zero keycodes must be
     *	in the range specified by min-keycode and max-keycode in the
     *	connection setup (else a Value error)"
     */
    i = inputMapLen;
    while (i--) {
	if (inputMap[i]
	    && (inputMap[i] < (*k)->curKeySyms.minKeyCode
		|| inputMap[i] > (*k)->curKeySyms.maxKeyCode)) {
		client->errorValue = inputMap[i];
		return BadValue;
		}
    }

    /*
     *	Now enforce the restriction that none of the old or new
     *	modifier keys may be down while we change the mapping,  and
     *	that the DDX layer likes the choice.
     */
    if (!AllModifierKeysAreUp (dev, (*k)->modifierKeyMap, 
	(int)(*k)->maxKeysPerModifier, inputMap, (int)numKeyPerModifier)
	    ||
	!AllModifierKeysAreUp(dev, inputMap, (int)numKeyPerModifier,
	      (*k)->modifierKeyMap, (int)(*k)->maxKeysPerModifier)) {
	return MappingBusy;
    } else {
	for (i = 0; i < inputMapLen; i++) {
	    if (inputMap[i] && !LegalModifier(inputMap[i])) {
		return MappingFailed;
	    }
	}
    }

    /*
     *	Now build the keyboard's modifier bitmap from the
     *	list of keycodes.
     */
    map = (KeyCode *)xalloc(inputMapLen);
    if (!map)
        return BadAlloc;
    if ((*k)->modifierKeyMap)
        xfree((*k)->modifierKeyMap);
    (*k)->modifierKeyMap = map;
    bcopy((char *)inputMap, (char *)(*k)->modifierKeyMap, inputMapLen);
    (*k)->maxKeysPerModifier = numKeyPerModifier;
    for (i = 0; i < MAP_LENGTH; i++)
        (*k)->modifierMap[i] = 0;
    for (i = 0; i < inputMapLen; i++) if (inputMap[i]) {
        (*k)->modifierMap[inputMap[i]]
          |= (1<<(i/ (*k)->maxKeysPerModifier));
    }

    return(MappingSuccess);
    }

int
SendDeviceMappingNotify(request, firstKeyCode, count, dev)
    CARD8 request, count;
    KeyCode firstKeyCode;
    DeviceIntPtr dev;
    {
    int i;
    xEvent event;
    deviceMappingNotify         *ev = (deviceMappingNotify *) &event;
    extern              	int     DeviceMappingNotify;

    ev->type = DeviceMappingNotify;
    ev->request = request;
    ev->deviceid = dev->id;
    ev->time = currentTime.milliseconds;
    if (request == MappingKeyboard)
	{
	ev->firstKeyCode = firstKeyCode;
	ev->count = count;
	}

    SendEventToAllWindows (dev, DeviceMappingNotifyMask, ev, 1);
    }

int
ChangeKeyMapping(client, dev, len, type, firstKeyCode, keyCodes, 
	keySymsPerKeyCode, map)
    ClientPtr 	client;
    DeviceIntPtr dev;
    unsigned 	len;
    int 	type;
    KeyCode 	firstKeyCode;
    CARD8 	keyCodes;
    CARD8 	keySymsPerKeyCode;
    KeySym	*map;
{
    KeySymsRec keysyms;
    KeyClassPtr k = dev->key;

    if (k == NULL)
	return (BadMatch);

    if (len != (keyCodes * keySymsPerKeyCode))
            return BadLength;

    if ((firstKeyCode < k->curKeySyms.minKeyCode) ||
	(firstKeyCode + keyCodes - 1 > k->curKeySyms.maxKeyCode))
    {
	    client->errorValue = firstKeyCode;
	    return BadValue;
    }
    if (keySymsPerKeyCode == 0)
    {
	    client->errorValue = 0;
            return BadValue;
    }
    keysyms.minKeyCode = firstKeyCode;
    keysyms.maxKeyCode = firstKeyCode + keyCodes - 1;
    keysyms.mapWidth = keySymsPerKeyCode;
    keysyms.map = map;
    if (!SetKeySymsMap(&k->curKeySyms, &keysyms))
	return BadAlloc;
    SendDeviceMappingNotify(MappingKeyboard, firstKeyCode, keyCodes,
	dev);
    return client->noClientException;
    }

void
DeleteWindowFromAnyExtEvents(pWin, freeResources)
    WindowPtr		pWin;
    Bool		freeResources;
    {
    WindowPtr		parent;
    DeviceIntPtr	dev;
    InputClientsPtr	ic;


    for (dev=inputInfo.devices; dev; dev=dev->next)
	{
	if (dev == inputInfo.pointer ||
	    dev == inputInfo.keyboard)
	    continue;

	/* Deactivate any grabs performed on this window, before making
	   any input focus changes.
           Deactivating a device grab should cause focus events. */

	if (dev->grab && (dev->grab->window == pWin))
	    (*dev->DeactivateGrab)(dev);

	/* If the focus window is a root window (ie. has no parent) 
	   then don't delete the focus from it. */
    
	if ((pWin == dev->focus->win) && (pWin->parent != NullWindow))
	    {
	    int focusEventMode = NotifyNormal;

 	    /* If a grab is in progress, then alter the mode of focus events. */

	    if (dev->grab)
		focusEventMode = NotifyWhileGrabbed;

	    switch (dev->focus->revert)
		{
		case RevertToNone:
		    DoFocusEvents(dev, pWin, NoneWin, focusEventMode);
		    dev->focus->win = NoneWin;
		    dev->focus->traceGood = 0;
		    break;
		case RevertToParent:
		    parent = pWin;
		    do
			{
			parent = parent->parent;
			dev->focus->traceGood--;
			} while (!parent->realized);
		    DoFocusEvents(dev, pWin, parent, focusEventMode);
		    dev->focus->win = parent;
		    dev->focus->revert = RevertToNone;
		    break;
		case RevertToPointerRoot:
		    DoFocusEvents(dev, pWin, PointerRootWin, focusEventMode);
		    dev->focus->win = PointerRootWin;
		    dev->focus->traceGood = 0;
		    break;
		}
	    }

	if (dev->valuator)
	    if (dev->valuator->motionHintWindow == pWin)
		dev->valuator->motionHintWindow = NullWindow;
	}

    if (freeResources)
	while (wOtherInputMasks(pWin))
	    {
	    ic = wOtherInputMasks(pWin)->inputClients;
	    FreeResource(ic->resource, RT_NONE);
	    }
    }

int
MaybeSendDeviceMotionNotifyHint (pEvents, mask)
    deviceKeyButtonPointer *pEvents;
    Mask mask;
    {
    DeviceIntPtr dev;
    DeviceIntPtr LookupDeviceIntRec ();

    dev = LookupDeviceIntRec (pEvents->deviceid & DEVICE_BITS);
    if (pEvents->type == DeviceMotionNotify)
	{
	if (mask & DevicePointerMotionHintMask)
	    {
	    if (WID(dev->valuator->motionHintWindow) == pEvents->event)
		{
		return 1; /* don't send, but pretend we did */
		}
	    pEvents->detail = NotifyHint;
	    }
	 else
	    {
	    pEvents->detail = NotifyNormal;
	    }
	}
    return (0);
    }

int
CheckDeviceGrabAndHintWindow (pWin, type, xE, grab, client, deliveryMask)
    WindowPtr pWin;
    int type;
    deviceKeyButtonPointer *xE;
    GrabPtr grab;
    ClientPtr client;
    Mask deliveryMask;
    {
    DeviceIntPtr dev;
    DeviceIntPtr LookupDeviceIntRec ();

    dev = LookupDeviceIntRec (xE->deviceid & DEVICE_BITS);
    if (type == DeviceMotionNotify)
	dev->valuator->motionHintWindow = pWin;
    else if ((type == DeviceButtonPress) && (!grab) && 
	(deliveryMask & DeviceButtonGrabMask))
        {
	GrabRec tempGrab;

	tempGrab.device = dev;
	tempGrab.resource = client->clientAsMask;
	tempGrab.window = pWin;
	tempGrab.ownerEvents = (deliveryMask & DeviceOwnerGrabButtonMask) ? TRUE : FALSE;
	tempGrab.eventMask = deliveryMask;
	tempGrab.keyboardMode = GrabModeAsync;
	tempGrab.pointerMode = GrabModeAsync;
	tempGrab.confineTo = NullWindow;
	tempGrab.cursor = NullCursor;
	(*dev->ActivateGrab)(dev, &tempGrab, currentTime, TRUE);
        }
    }

Mask
DeviceEventMaskForClient(dev, pWin, client)
    DeviceIntPtr	dev;
    WindowPtr		pWin;
    ClientPtr		client;
    {
    register InputClientsPtr other, prev;

    if (!wOtherInputMasks(pWin))
	return 0;
    for (other = wOtherInputMasks(pWin)->inputClients; other; 
	other = other->next)
	{
	if (SameClient(other, client))
	    return other->mask[dev->id];
	}
    return 0;
    }

void
MaybeStopDeviceHint(dev, client)
    register DeviceIntPtr dev;
    ClientPtr client;
{
    WindowPtr pWin;
    GrabPtr grab = dev->grab;
    pWin = dev->valuator->motionHintWindow;

    if ((grab && SameClient(grab, client) &&
	 ((grab->eventMask & DevicePointerMotionHintMask) ||
	  (grab->ownerEvents &&
	   (DeviceEventMaskForClient(dev, pWin, client) &
	    DevicePointerMotionHintMask)))) ||
	(!grab &&
	 (DeviceEventMaskForClient(dev, pWin, client) &
	  DevicePointerMotionHintMask)))
	dev->valuator->motionHintWindow = NullWindow;
}

int
DeviceEventSuppressForWindow(pWin, client, mask, maskndx)
	WindowPtr pWin;
	ClientPtr client;
	Mask mask;
	int maskndx;
    {
    if (mask & ~PropagateMask[maskndx])
	{
	client->errorValue = mask;
	return BadValue;
	}
    if (mask == 0) 
	{
	if (wOtherInputMasks(pWin))
	    {
	    wOtherInputMasks(pWin)->dontPropagateMask[maskndx] = mask;
	    CheckWindowOptionalNeed (pWin);
	    }
	} 
    else 
	{
	if (!pWin->optional && !MakeWindowOptional (pWin))
	    return BadAlloc;
	wOtherInputMasks(pWin)->dontPropagateMask[maskndx] = mask;
	}
    RecalculateDeviceDeliverableEvents(pWin);
    }
