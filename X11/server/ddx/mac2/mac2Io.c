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
 * mac2Io.c --
 *	Functions to handle input from the keyboard and mouse.
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

#include    "mac2.h"
#include    "opaque.h"

unsigned long  	lastEventTime = 0;
extern int	mac2SigIO;
extern int      screenIsSaved;
extern void	SaveScreens();
extern int	arrowKeys;
int		KbdFd = -1;
int		MouseFd = -1;
#define	INPBUFSIZE	1024

/* return time of day in milliseconds */
/* always at least one more than lastEventTime */
/* it may warp its way into the future every then and now... */
unsigned long WarpedGetTimeInMillis()
{
  unsigned long now;

  now = GetTimeInMillis();
  return (now > lastEventTime) ? now : lastEventTime + 1;
}

/*-
 *-----------------------------------------------------------------------
 * SetTimeSinceLastInputEvent --
 *	Set the lastEventTime to now.
 *
 * Results:
 *	None.
 *
 * Side Effects:
 *	lastEventTime is altered.
 *
 *-----------------------------------------------------------------------
 */
void SetTimeSinceLastInputEvent()
{
  lastEventTime = WarpedGetTimeInMillis();
}

/*-
 *-----------------------------------------------------------------------
 * TimeSinceLastInputEvent --
 *	Function used for screensaver purposes by the os module.
 *
 * Results:
 *	The time in milliseconds since there last was any
 *	input.
 *
 * Side Effects:
 *	None.
 *
 *-----------------------------------------------------------------------
 */
unsigned long TimeSinceLastInputEvent()
{
  return (lastEventTime == 0) ? 0 : WarpedGetTimeInMillis() - lastEventTime;
}

/*-
 *-----------------------------------------------------------------------
 * ProcessInputEvents --
 *	Retrieve all waiting input events and pass them to DIX in their
 *	correct chronological order. Only reads from the system pointer
 *	and keyboard.
 *
 * Results:
 *	None.
 *
 * Side Effects:
 *	Events are passed to the DIX layer.
 *
 *-----------------------------------------------------------------------
 */

void ProcessInputEvents()
{
  static int optionKeyUp = 1;
  DevicePtr pPointer;
  DevicePtr pKeyboard;
  PtrPrivPtr ptrPriv;
  KbPrivPtr kbdPriv;
  enum { NoneYet, Ptr, Kbd } lastType = NoneYet; /* Type of last event */
  unsigned char MouseEvents[INPBUFSIZE];
  unsigned char KeyboardEvents[INPBUFSIZE];
  adb_cmd_t *adb_cmd;
  unsigned char *adb_data;
  int i, j, n;
  char debug_buffer[4096];
  static int debug = 0;

  /* clear debug buffer */
  *debug_buffer = 0;

  /* clear SigIO flag */
  mac2SigIO = 0;

  /* get private pointer and keyboard data */
  pPointer = LookupPointerDevice();
  ptrPriv = (PtrPrivPtr)pPointer->devicePrivate;
  pKeyboard = LookupKeyboardDevice();
  kbdPriv = (KbPrivPtr)pKeyboard->devicePrivate;

  /* check for mouse input */
  n = read(MouseFd, MouseEvents, sizeof(MouseEvents));
  if ((n < 0) && (errno != EWOULDBLOCK))
    ErrorF("WARNING: ADB mouse: read errno = %d\n", errno);

  /* process adb packets from mouse */
  for (i = 0; i < n;) {

    /* get next adb packet from mouse */
    adb_cmd = (adb_cmd_t *)&MouseEvents[i];
    adb_data = &MouseEvents[i + sizeof(adb_cmd_t)];
    i += adb_cmd->cmd.len + 2;
    if (adb_cmd->reg.cmd != ADB_talk) {
      ErrorF("WARNING: ADB mouse: rec.cmd = 0x%X\n", adb_cmd->reg.cmd);
      continue;
    }
    if (adb_cmd->reg.reg) {
      ErrorF("WARNING: ADB mouse: reg.reg = 0x%X\n", adb_cmd->reg.reg);
      continue;
    }

    /* turn of screen saver if it's on */
    if (screenIsSaved == SCREEN_SAVER_ON)
      SaveScreens(SCREEN_SAVER_OFF, ScreenSaverReset);

    /* note when this event is being processed */
    SetTimeSinceLastInputEvent();

    /* zero keyboard buttons, steps on adb_cmd->cmd.len */
    *(adb_data - 1) = 0;

    /* process mouse event */
    (*ptrPriv->ProcessEvent)(pPointer, adb_data - 1);

    /* note that last event was a mouse event */
    lastType = Ptr;

  } /* for (i = 0; i < n;) */

  /* check for keyboard input */
  n = read(KbdFd, KeyboardEvents, sizeof(KeyboardEvents));
  if ((n < 0) && (errno != EWOULDBLOCK))
    ErrorF("ADB keyboard: read errno = %d\n", errno);

  if ((debug > 1) && (n > 0)) {
    sprintf(index(debug_buffer, 0), "adb keyboard %d bytes: ", n);
    for (i = 0; i < n; i++)
      sprintf(index(debug_buffer, 0), " 0x%X", KeyboardEvents[i]);
      sprintf(index(debug_buffer, 0), "\n");
  }

  /* fake a keyboard event if autorepeating */
  if (autoRepeatKeyDown && autoRepeatReady &&
    (kbdPriv->ctrl->autoRepeat == AutoRepeatModeOn) && (n <= 0)) {
    adb_cmd = (adb_cmd_t *)KeyboardEvents;
    adb_cmd->cmd.len = 1;
    adb_cmd->reg.cmd = ADB_talk;
    adb_cmd->reg.reg = 0;
    adb_data = &KeyboardEvents[sizeof(adb_cmd_t)];
    *adb_data = AUTOREPEAT_EVENTID;
    n = 3;
  }

  /* process adb packets from keyboard */
  for (i = 0; i < n;) {

    /* get next adb packet from keyboard */
    adb_cmd = (adb_cmd_t *)&KeyboardEvents[i];
    adb_data = &KeyboardEvents[i + sizeof(adb_cmd_t)];
    i += adb_cmd->cmd.len + 2;
    if (adb_cmd->reg.cmd != ADB_talk) {
      ErrorF("WARNING: ADB keyboard: rec.cmd = 0x%X\n", adb_cmd->reg.cmd);
      continue;
    }
    if (adb_cmd->reg.reg) {
      ErrorF("WARNING: ADB keyboard: reg.reg = 0x%X\n", adb_cmd->reg.reg);
      continue;
    }

    /* reset key toggles debug flag */
    if (adb_data[0] == 0x7F) {
      ++debug;
      if (debug > 2) debug = 0;
    }

    for (j = 0; j < adb_cmd->cmd.len; j++) {

      /* ignore all FF bytes */
      if (adb_data[j] == 0xFF) continue;

      /* ignore reset key, so that auto repeat will work */
      if (adb_data[j] == 0x7F) continue;

      /* usefull debugging message */
      if (debug) sprintf(index(debug_buffer, 0), "adb key 0x%X %s\n",
        KEY_DETAIL(adb_data[j]), KEY_UP(adb_data[j]) ? "up" : "down");

      /* turn of screen saver if it's on */
      if (screenIsSaved == SCREEN_SAVER_ON)
        SaveScreens(SCREEN_SAVER_OFF, ScreenSaverReset);

      /* note when this event is being processed */
      SetTimeSinceLastInputEvent();

      /* note state of option key */
      if (IS_OPTION_KEY(adb_data[j])) optionKeyUp = KEY_UP(adb_data[j]);

      /* option/arrow generates middle and right mouse button events */
      if ((!optionKeyUp != !arrowKeys) && IS_MOUSE_KEY(adb_data[j])) {
        /* switch from keyboard to mouse events */
        if (lastType == Kbd) (*kbdPriv->DoneEvents)(pKeyboard, FALSE);
        /* process mouse event */
        (*ptrPriv->ProcessEvent)(pPointer, &adb_data[j]);
        /* note that last event was a mouse event */
        lastType = Ptr;
      }

      /* process keyboard event */
      else {
        /* switch from mouse to keyboard events */
        if (lastType == Ptr) (*ptrPriv->DoneEvents)(pPointer, FALSE);
        /* process keyboard event */
        (*kbdPriv->ProcessEvent)(pKeyboard, &adb_data[j]);
        /* note that last event was a keyboard event */
        lastType = Kbd;
      }

    } /* for (j = 0; j < adb_CMD->cmd.len; j++) */

  } /* for (i = 0; i < n;) */

  /* all done */
  (*kbdPriv->DoneEvents)(pKeyboard, TRUE);
  (*ptrPriv->DoneEvents)(pPointer, TRUE);

  /* display debugging information */
  if (debug) ErrorF("%s", debug_buffer);

} /* ProcessInputEvents() */
