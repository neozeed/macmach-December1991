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
 * mac2Init.c --
 *	Initialization functions for screen/keyboard/mouse, etc.
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
#include    <servermd.h>
#include    "dixstruct.h"
#include    "dix.h"
#include    "opaque.h"
#include    "mipointer.h"

#include <sys/ioctl.h>
#include <sys/types.h>

extern int mac2MouseProc();
extern void mac2KbdProc();
extern int mac2KbdSetUp();
extern Bool mac2MonoProbe();
extern Bool mac2ColorProbe();
extern void ProcessInputEvents();

extern void SetInputCheck();
extern char *strncpy();

int mac2SigIO = 0;	 /* For use with SetInputCheck */
static int autoRepeatHandlersInstalled; /* FALSE each time InitOutput called */

int arrowKeys = 0; /* If zero, map arrow keys to mouse buttons. */

static int mac2OpenAllScreens = TRUE;
static int mac2ScreensWanted = 0;


/*-
 *-----------------------------------------------------------------------
 * SigIOHandler --
 *	Signal handler for SIGIO - input is available.
 *
 * Results:
 *	mac2SigIO is set - ProcessInputEvents() will be called soon.
 *
 * Side Effects:
 *	None
 *
 *-----------------------------------------------------------------------
 */
/*ARGSUSED*/
static int SigIOHandler(int sig, int code, struct sigcontext *scp)
{
  mac2SigIO = 1;
  return 1;
}

mac2FbDataRec mac2FbData[] = {
#ifdef MM_KLUDGE
    mac2ColorProbe,	"/dev/scn0",  FALSE,  1,  neverProbed,
    mac2ColorProbe,	"/dev/scn1",  FALSE,  1,  neverProbed,
    mac2ColorProbe,	"/dev/scn2",  FALSE,  1,  neverProbed,
    mac2ColorProbe,	"/dev/scn3",  FALSE,  1,  neverProbed,
    mac2ColorProbe,	"/dev/scn4",  FALSE,  1,  neverProbed,
#else MM_KLUDGE
    mac2ColorProbe,	"/dev/scn0",  FALSE,  DEFAULTDEPTH,  neverProbed,
    mac2ColorProbe,	"/dev/scn1",  FALSE,  DEFAULTDEPTH,  neverProbed,
    mac2ColorProbe,	"/dev/scn2",  FALSE,  DEFAULTDEPTH,  neverProbed,
    mac2ColorProbe,	"/dev/scn3",  FALSE,  DEFAULTDEPTH,  neverProbed,
    mac2ColorProbe,	"/dev/scn4",  FALSE,  DEFAULTDEPTH,  neverProbed,
#endif MM_KLUDGE
};

/*
 * NUMSCREENS is the number of supported frame buffers (i.e. the number of
 * structures in mac2FbData which have an actual probeProc).
 */
#define NUMSCREENS (sizeof(mac2FbData)/sizeof(mac2FbData[0]))
#define NUMDEVICES 2

fbFd	mac2Fbs[NUMSCREENS];  /* Space for descriptors of open frame buffers */

static PixmapFormatRec	formats[] = {
  1, 1, BITMAP_SCANLINE_PAD,	/* 1-bit deep */
  8, 8, BITMAP_SCANLINE_PAD,	/* 8-bit deep */
};
#define NUMFORMATS	(sizeof formats)/(sizeof formats[0])

/*-
 *-----------------------------------------------------------------------
 * InitOutput --
 *	Initialize screenInfo for all actually accessible framebuffers.
 *
 * Results:
 *	screenInfo init proc field set
 *
 * Side Effects:
 *	None
 *
 *-----------------------------------------------------------------------
 */

InitOutput(ScreenInfo *pScreenInfo, int argc, char **argv)
{
  int i, index, ac = argc;
  char **av = argv;

  pScreenInfo->imageByteOrder = IMAGE_BYTE_ORDER;
  pScreenInfo->bitmapScanlineUnit = BITMAP_SCANLINE_UNIT;
  pScreenInfo->bitmapScanlinePad = BITMAP_SCANLINE_PAD;
  pScreenInfo->bitmapBitOrder = BITMAP_BIT_ORDER;
  pScreenInfo->numPixmapFormats = NUMFORMATS;
  for (i=0; i< NUMFORMATS; i++) pScreenInfo->formats[i] = formats[i];
  autoRepeatHandlersInstalled = FALSE;
  for (i = 0, index = 0; i < NUMSCREENS; i++) {
    if (mac2OpenAllScreens || mac2FbData[i].Wanted) {
      if ((* mac2FbData[i].probeProc) (pScreenInfo, index, i, argc, argv)) {
        /* This display exists OK */
        index++;
      }
      else {
        /* This display can't be opened */
        if (mac2FbData[i].Wanted)
          FatalError("Could not open display %s\n", mac2FbData[i].devName);
      }
    }
  }
  if (index == 0) FatalError("Can't find any displays\n");
  pScreenInfo->numScreens = index;
}

/*-
 *-----------------------------------------------------------------------
 * InitInput --
 *	Initialize all supported input devices...what else is there
 *	besides pointer and keyboard?
 *
 * Results:
 *	None.
 *
 * Side Effects:
 *	Two DeviceRec's are allocated and registered as the system pointer
 *	and keyboard devices.
 *
 *-----------------------------------------------------------------------
 */
/*ARGSUSED*/
InitInput(int argc, char **argv)
{
  DevicePtr p, k;
  static int  zero = 0;

  arrowKeys = getenv("ARROWKEYS") != 0;
  p = AddInputDevice(mac2MouseProc, TRUE);
  k = AddInputDevice(mac2KbdProc, TRUE);
  if (!p || !k) FatalError("failed to create input devices in InitInput");
  RegisterPointerDevice(p);
  RegisterKeyboardDevice(k);
  miRegisterPointerDevice(screenInfo.screens[0], p);
  signal(SIGIO, SigIOHandler);
  SetInputCheck (&zero, &mac2SigIO);
}


/*-
 *-----------------------------------------------------------------------
 * mac2ScreenInit --
 *	Things which must be done for all types of frame buffers...
 *	Should be called last of all.
 *
 * Results:
 *	None.
 *
 * Side Effects:
 *	The graphics context for the screen is created. The CreateGC,
 *	CreateWindow and ChangeWindowAttributes vectors are changed in
 *	the screen structure.
 *
 *	Both a BlockHandler and a WakeupHandler are installed for the
 *	first screen.  Together, these handlers implement autorepeat
 *	keystrokes.
 *
 *-----------------------------------------------------------------------
 */
Bool mac2ScreenInit(ScreenPtr pScreen)
{
  extern void mac2BlockHandler();
  extern void mac2WakeupHandler();
  static ScreenPtr autoRepeatScreen;
  extern miPointerCursorFuncRec mac2PointerCursorFuncs;

  /* Block/Unblock handlers */
  if (autoRepeatHandlersInstalled == FALSE) {
    autoRepeatScreen = pScreen;
    autoRepeatHandlersInstalled = TRUE;
  }
  if (pScreen == autoRepeatScreen) {
    pScreen->BlockHandler = mac2BlockHandler;
    pScreen->WakeupHandler = mac2WakeupHandler;
  }
  miDCInitialize(pScreen, &mac2PointerCursorFuncs);
  return TRUE;
}

/*-
 *-----------------------------------------------------------------------
 * mac2OpenFrameBuffer --
 *	Open a frame buffer through the /dev/scn* interface.
 *
 * Results:
 *	The fd of the framebuffer.
 *
 * Side Effects:
 *
 *-----------------------------------------------------------------------
 */
int mac2OpenFrameBuffer(index, fbNum, argc, argv)
int fbNum;    	/* Index into the mac2FbData array */
int index;    	/* Screen index */
int argc;	/* Command-line arguments... */
char **argv;   	/* ... */
{
  int fd = -1;
  VPBlockPtr v;
  int mode;

  if ((fd = open(mac2FbData[fbNum].devName, O_RDWR, 0)) < 0) return -1;
  mac2Fbs[index].fd = fd;
  (void)mac2VideoSetInit(index);
  /* copy the driver data, base address and page fields onto all possible modes */
  for (mode = 1; mode < VIDEO_MAXMODES; mode++) {
    mac2Fbs[index].vpinfo[mode].csData = mac2Fbs[index].vpinfo[0].csData;
    mac2Fbs[index].vpinfo[mode].csPage = mac2Fbs[index].vpinfo[0].csPage;
    mac2Fbs[index].vpinfo[mode].csBaseAddr = mac2Fbs[index].vpinfo[0].csBaseAddr;
  }
  mode = 0; mac2Fbs[index].default_depth = 1;
  v = &(mac2Fbs[index].info[mode]);
  if (ioctl(fd, VIDEO_PARAMS, v) < 0 ) {
    FatalError("could not obtain characteristics (ioctl VIDEO_PARAMS failed!)\n");
    (void)close(fd);
    return(!Success);
  }
  else mac2Fbs[index].default_depth = v->vpPixelSize;
  return (fd);
}

void AbortDDX()
{
  extern int KbdFd;
  extern int MouseFd;

  if (KbdFd > 0) {
    mac2AdbSetUp(KbdFd, FALSE); /* Must NOT FatalError() anywhere! */
    close(KbdFd);
    KbdFd = 0;
  }
  if (MouseFd > 0) {
    mac2AdbSetUp(MouseFd, FALSE);
    close(MouseFd);
    MouseFd = 0;
  }
}

/* Called by GiveUp(). */
void
ddxGiveUp()
{
}


int ddxProcessArgument(int argc, char **argv, int i)
{
  int skip = 0;

  if (!strcmp(argv[i], "-dev")) {
    mac2FbDataRec *ppScr = mac2FbData;
    mac2FbDataRec *pScr;
    int found = FALSE;
    int count = 0;
    mac2OpenAllScreens = FALSE;
    while ((pScr = ppScr++) && !found && (count < NUMSCREENS)) {
      if (!strcmp( argv[i+1], pScr->devName)) {
        pScr->Wanted = TRUE;
        mac2ScreensWanted++;
        found = TRUE;
        skip = 2;
        if (!strcmp(argv[i+2], "-mono")) {
          pScr->depthRequested = 1;
          skip++;
        }
        else if (!strcmp(argv[i+2], "-color")) {
          pScr->depthRequested = 8;
          skip++;
        }
        else if (!strcmp(argv[i+2], "-depth")) {
          pScr->depthRequested = atoi(argv[i+3]);
          skip = skip + 2;
        }
        else { /* use default depth supported by hardware */
          pScr->depthRequested = DEFAULTDEPTH;
        }
      }
      count++;
    }
    if (!found) FatalError("%s not supported\n", &argv[i][1]);
  }
  return(skip);
}


void ddxUseMsg()
{
}

/*-
 *-----------------------------------------------------------------------
 * mac2WhiteScreen --
 *	Fill a frame buffer with pixel 0.
 *
 * Results:
 *	None
 *
 * Side Effects:
 *
 *-----------------------------------------------------------------------
 */
int mac2WhiteScreen(int index)
{
  fbFd *pf;
  register unsigned char* fb;
  register int fbinc, line, lw;
  register unsigned int *fbt;

  pf = &mac2Fbs[index];
  fb = pf->fb; /* Assumed longword aligned! */
  switch (pf->info[mac2Fbs[index].mode].vpPixelSize) {
    case 1:
      fbinc = pf->info[mac2Fbs[index].mode].vpRowBytes;
      for (line = pf->info[mac2Fbs[index].mode].vpBounds.top; line < pf->info[mac2Fbs[index].mode].vpBounds.bottom; line++) {
        lw = ((pf->info[mac2Fbs[index].mode].vpBounds.right - pf->info[mac2Fbs[index].mode].vpBounds.left) + 31) >> 5;
        fbt = (unsigned int *)fb;
        do *fbt++ = 0x00000000; while (--lw);
        fb += fbinc;
      }
      break;
    case 8:
      fbinc = pf->info[mac2Fbs[index].mode].vpRowBytes;
      for (line = pf->info[mac2Fbs[index].mode].vpBounds.top; line < pf->info[mac2Fbs[index].mode].vpBounds.bottom; line++) {
        lw = ((pf->info[mac2Fbs[index].mode].vpBounds.right - pf->info[mac2Fbs[index].mode].vpBounds.left) + 31) >> 5;
        fbt = (unsigned int *)fb;
        do *fbt++ = 0x0; while (--lw);
        fb += fbinc;
      }
      break;
    default:
      ErrorF("Bad depth in mac2WhiteScreen.");
      break;
  }
}

int mac2AdbSetUp(int fd, Bool openClose)
{
  int iarg;

  iarg = openClose ? 1 : 0;
  if (ioctl(fd, FIOASYNC, &iarg) < 0) {
    FatalError("mac2AdbSetUp: ioctl FIOASYNC failed!\n");
    return !Success;
  }
  if (ioctl(fd, FIONBIO, &iarg, 0) < 0) {
    FatalError("mac2AdbSetUp: ioctl FIONBIO failed! \r\n");
    return (!Success);
  }
}
