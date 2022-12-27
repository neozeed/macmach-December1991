/************************************************************ 
Copyright 1988 by Apple Computer, Inc, Cupertino, California
			All Rights Reserved

Permission to use, copy, modify, and distribute this software
for any purpose and without fee is hereby granted, provided
that the above copyright notice appear in all copies.

APPLE MAKES NO WARRANTY OR REPRESENTATION, EITHER EXPRESS,
OR IMPLIED, WITH RESPECT TO THIS SOFTWARE, ITS QUALITY,
PERFORMANCE, MERCHANABILITY, OR FITNESS FOR A PARTICULAR
PURPOSE. AS A RESULT, THIS SOFTWARE IS PROVIDED "AS IS,"
AND YOU THE USER ARE ASSUMING THE ENTIRE RISK AS TO ITS
QUALITY AND PERFORMANCE. IN NO EVENT WILL APPLE BE LIABLE 
FOR DIRECT, INDIRECT, SPECIAL, INCIDENTAL, OR CONSEQUENTIAL
DAMAGES RESULTING FROM ANY DEFECT IN THE SOFTWARE.

THE WARRANTY AND REMEDIES SET FORTH ABOVE ARE EXCLUSIVE
AND IN LIEU OF ALL OTHERS, ORAL OR WRITTEN, EXPRESS OR
IMPLIED.

************************************************************/
/*-
 * mac2Mouse.c --
 *	Functions for playing cat and mouse... sorry.
 *
 * Copyright (c) 1987 by the Regents of the University of California
 *
 * Permission to use, copy, modify, and distribute this
 * software and its documentation for any purpose and without
 * fee is hereby granted, provided that the above copyright
 * notice appear in all copies.  The University of California
 * makes no representations about the suitability of this
 * software for any purpose.  It is provided "as is" without
 * express or implied warranty.
 *
 *
 */

/************************************************************
Copyright 1987 by Sun Microsystems, Inc. Mountain View, CA.

                    All Rights Reserved

Permission  to  use,  copy,  modify,  and  distribute   this
software  and  its documentation for any purpose and without
fee is hereby granted, provided that the above copyright no-
tice  appear  in all copies and that both that copyright no-
tice and this permission notice appear in  supporting  docu-
mentation,  and  that the names of Sun or MIT not be used in
advertising or publicity pertaining to distribution  of  the
software  without specific prior written permission. Sun and
M.I.T. make no representations about the suitability of this
software for any purpose. It is provided "as is" without any
express or implied warranty.

SUN DISCLAIMS ALL WARRANTIES WITH REGARD TO  THIS  SOFTWARE,
INCLUDING ALL IMPLIED WARRANTIES OF MERCHANTABILITY AND FIT-
NESS FOR A PARTICULAR PURPOSE. IN NO EVENT SHALL SUN BE  LI-
ABLE  FOR  ANY SPECIAL, INDIRECT OR CONSEQUENTIAL DAMAGES OR
ANY DAMAGES WHATSOEVER RESULTING FROM LOSS OF USE,  DATA  OR
PROFITS,  WHETHER  IN  AN  ACTION OF CONTRACT, NEGLIGENCE OR
OTHER TORTIOUS ACTION, ARISING OUT OF OR IN CONNECTION  WITH
THE USE OR PERFORMANCE OF THIS SOFTWARE.

********************************************************/

#undef MACII_ALL_MOTION
#define NEED_EVENTS

#include    "mac2.h"
#include    "mipointer.h"
#include    "misprite.h"

Bool ActiveZaphod = TRUE;

static long mac2EventTime();
static Bool mac2CursorOffScreen();
static void mac2CrossScreen();
extern void miPointerQueueEvent();

miPointerCursorFuncRec mac2PointerCursorFuncs = {
    mac2EventTime,
    mac2CursorOffScreen,
    mac2CrossScreen,
    miPointerQueueEvent,
};

typedef struct {
    Bool    mouseMoved;	    /* Mouse has moved */
} mac2MsPrivRec, *mac2MsPrivPtr;

static void 	  	mac2MouseCtrl();
static int 	  	mac2MouseGetMotionEvents();
void 	  		mac2MouseProcessEvent();
static void 	  	mac2MouseDoneEvents();

extern int mac2AdbSetUp(int fd, Bool openClose);
extern int MouseFd;

static mac2MsPrivRec	mac2MousePriv;
static PtrPrivRec 	sysMousePriv = {
    mac2MouseProcessEvent,	/* Function to process an event */
    mac2MouseDoneEvents,		/* When all the events have been */
				/* handled, this function will be */
				/* called. */
    0,				/* Current X coordinate of pointer */
    0,				/* Current Y coordinate */
    NULL,			/* Screen pointer is on */
    (pointer)&mac2MousePriv,	/* Field private to device */
};

/*-
 *-----------------------------------------------------------------------
 * mac2MouseProc --
 *	Handle the initialization, etc. of a mouse
 *
 * Results:
 *	none.
 *
 * Side Effects:
 *
 * Note:
 *      Mouse input comes from adb device 3, /dev/mouse.
 *-----------------------------------------------------------------------
 */
int mac2MouseProc(DevicePtr pMouse, int what)
{
  BYTE map[4];

  switch (what) {
    case DEVICE_INIT:
      if (pMouse != LookupPointerDevice()) {
        ErrorF("Cannot open non-system mouse\n");	
        return(!Success);
      }
      if ((MouseFd < 0) && ((MouseFd = open("/dev/mouse", O_RDONLY)) < 0)) {
        FatalError("Could not open /dev/mouse.\n");
        return(!Success);
      }
      sysMousePriv.pScreen = screenInfo.screens[0];
      sysMousePriv.x = 0;
      sysMousePriv.y = 0;
      mac2MousePriv.mouseMoved = FALSE;
      pMouse->devicePrivate = (pointer) &sysMousePriv;
      pMouse->on = FALSE;
      map[1] = 1;
      map[2] = 2;
      map[3] = 3;
      InitPointerDeviceStruct(
        pMouse,
        map,
        3,
        mac2MouseGetMotionEvents,
        mac2MouseCtrl,
        0);
      break;
    case DEVICE_ON:
      mac2AdbSetUp(MouseFd, TRUE);
      AddEnabledDevice(MouseFd);
      pMouse->on = TRUE;
      break;
    case DEVICE_CLOSE:
      mac2AdbSetUp(MouseFd, TRUE);
      RemoveEnabledDevice(MouseFd);
      pMouse->on = FALSE;
      close(MouseFd);
      MouseFd = -1;
      break;
    case DEVICE_OFF:
      mac2AdbSetUp(MouseFd, FALSE);
      RemoveEnabledDevice(MouseFd);
      pMouse->on = FALSE;
      break;
  }
  return (Success);
} /* mac2MouseProc() */
	    
/*-
 *-----------------------------------------------------------------------
 * mac2MouseCtrl --
 *	Alter the control parameters for the mouse. Since acceleration
 *	etc. is done from the PtrCtrl record in the mouse's device record,
 *	there's nothing to do here.
 *
 * Results:
 *	None.
 *
 * Side Effects:
 *	None.
 *
 *-----------------------------------------------------------------------
 */
static void mac2MouseCtrl(DevicePtr pMouse)
{
}

/*-
 *-----------------------------------------------------------------------
 * mac2MouseGetMotionEvents --
 *	Return the (number of) motion events in the "motion history
 *	buffer" (snicker) between the given times.
 *
 * Results:
 *	The number of events stuffed.
 *
 * Side Effects:
 *	The relevant xTimecoord's are stuffed in the passed memory.
 *
 *-----------------------------------------------------------------------
 */
static int mac2MouseGetMotionEvents(xTimecoord *buff, CARD32 start, CARD32 stop)
{
  return 0;
}


/*-
 *-----------------------------------------------------------------------
 * MouseAccelerate --
 *	Given a delta and a mouse, return the acceleration of the delta.
 *
 * Results:
 *	The corrected delta
 *
 * Side Effects:
 *	None.
 *
 *-----------------------------------------------------------------------
 */
static short MouseAccelerate (DevicePtr pMouse, int delta)
{
  register int sgn = sign(delta);
  register PtrCtrl *pCtrl;

  delta = abs(delta);
  pCtrl = &((DeviceIntPtr) pMouse)->ptrfeed->ctrl;
  if (delta > pCtrl->threshold) {
    return ((short)(sgn * (pCtrl->threshold +
      ((delta - pCtrl->threshold) * pCtrl->num) / pCtrl->den)));
  } else return ((short)(sgn * delta));
}

/*-
 *-----------------------------------------------------------------------
 * mac2MouseDoneEvents --
 *	Finish off any mouse motions we haven't done yet. (At the moment
 *	this code is unused since we never save mouse motions as I'm
 *	unsure of the effect of getting a keystroke at a given [x,y] w/o
 *	having gotten a motion event to that [x,y])
 *
 * Results:
 *	None.
 *
 * Side Effects:
 *	A MotionNotify event may be generated.
 *
 *-----------------------------------------------------------------------
 */
/*ARGSUSED*/
static void mac2MouseDoneEvents (DevicePtr pMouse, Bool final)
{
  PtrPrivPtr pPriv;
  mac2MsPrivPtr pmac2Priv;
  int x , y;

  pPriv = (PtrPrivPtr) pMouse->devicePrivate;
  pmac2Priv = (mac2MsPrivPtr) pPriv->devPrivate;
  if (pmac2Priv->mouseMoved) {
    x = pPriv->x; y = pPriv->y;
    pPriv->x = 0; pPriv->y = 0;
    pmac2Priv->mouseMoved = FALSE;
    miPointerDeltaCursor (screenInfo.screens[0], x, y, TRUE);
  }
}

void mac2MouseProcessEvent(DeviceRec *pMouse, MouseEvent_t *Event)
{   
  xEvent		xE;
  register PtrPrivPtr	pPriv;	/* Private data for pointer */
  register mac2MsPrivPtr pmac2Priv; /* Private data for mouse */
  static int last_button = -1;

  pPriv = (PtrPrivPtr)pMouse->devicePrivate;
  pmac2Priv = (mac2MsPrivPtr)pPriv->devPrivate;
  xE.u.keyButtonPointer.time = lastEventTime;
  if (IS_MIDDLE_KEY(Event->keyboard_button)) {
    xE.u.u.detail = MS_MIDDLE - MS_LEFT + 1;
    xE.u.u.type = KEY_UP(Event->keyboard_button) ? ButtonRelease : ButtonPress;
    /* update client and DIX before processing button event */
    if (pmac2Priv->mouseMoved) (*pPriv->DoneEvents)(pMouse, FALSE);
    miPointerPosition(pPriv->pScreen,
                      &xE.u.keyButtonPointer.rootX,
                      &xE.u.keyButtonPointer.rootY);
    (*pMouse->processInputProc)(&xE, pMouse, 1);
    return;
  }
  if (IS_RIGHT_KEY(Event->keyboard_button)) {
    xE.u.u.detail = MS_RIGHT - MS_LEFT + 1;
    xE.u.u.type = KEY_UP(Event->keyboard_button) ? ButtonRelease : ButtonPress;
    /* update client and DIX before processing button event */
    if (pmac2Priv->mouseMoved) (*pPriv->DoneEvents)(pMouse, FALSE);
    miPointerPosition(pPriv->pScreen,
                      &xE.u.keyButtonPointer.rootX,
                      &xE.u.keyButtonPointer.rootY);
    (*pMouse->processInputProc)(&xE, pMouse, 1);
    return;
  }
  /*
   * When we detect a change in the mouse coordinates, we call
   * the cursor module to move the cursor. It has the option of
   * simply removing the cursor or just shifting it a bit.
   * If it is removed, DIX will restore it before we goes to sleep...
   *
   * What should be done if it goes off the screen? Move to another
   * screen? For now, we just force the pointer to stay on the
   * screen...
   */
  pPriv->x += MouseAccelerate(pMouse, Event->xdelta);
  pPriv->y += MouseAccelerate(pMouse, Event->ydelta);
#ifdef MACII_ALL_MOTION
  miPointerDeltaCursor(pPriv->pScreen, pPriv->x, pPriv->y, TRUE);
  pPriv->x = 0; pPriv->y = 0;
#else
  pmac2Priv->mouseMoved = TRUE;
#endif MACII_ALL_MOTION
  if (Event->mouse_button != last_button) {
    xE.u.u.detail = (MS_LEFT - MS_LEFT) + 1;
    xE.u.u.type = Event->mouse_button ? ButtonRelease : ButtonPress;
    last_button = Event->mouse_button;
    /* update client and DIX before processing button event */
    if (pmac2Priv->mouseMoved) (*pPriv->DoneEvents)(pMouse, FALSE);
    miPointerPosition(pPriv->pScreen,
                      &xE.u.keyButtonPointer.rootX,
                      &xE.u.keyButtonPointer.rootY);
    (*pMouse->processInputProc)(&xE, pMouse, 1);
  }
}
   
/*ARGSUSED*/
static Bool mac2CursorOffScreen(ScreenPtr *pScreen, int *x, int *y)
{
  int index;
  /*
   * Active Zaphod implementation:
   *    increment or decrement the current screen
   *    if the x is to the right or the left of
   *    the current screen.
   */
  if (screenInfo.numScreens > 1 && (*x >= (*pScreen)->width || *x < 0)) {
    index = (*pScreen)->myNum;
    if (*x < 0) {
      index = (index ? index : screenInfo.numScreens) - 1;
      *pScreen = screenInfo.screens[index];
      *x += (*pScreen)->width;
    }
    else {
      *x -= (*pScreen)->width;
      index = (index + 1) % screenInfo.numScreens;
      *pScreen = screenInfo.screens[index];
    }
    return TRUE;
  }
  else return FALSE;
}

/*ARGSUSED*/
static long mac2EventTime(ScreenPtr pScreen)
{
  return lastEventTime;
}


static void mac2CrossScreen(ScreenPtr pScreen, Bool entering)
{
}
