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
 * mac2Color.c --
 *	Functions for handling the mac2 video board with 1 bit/pixel.
 *
 * Copyright (c) 1987 by the Regents of the University of California
 * Copyright (c) 1987 by Adam de Boor, UC Berkeley
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
#include    "colormap.h"
#include    "colormapst.h"
#include    "resource.h"

#include <sys/ioctl.h>
#include <sys/mman.h>
#include <sys/types.h>

extern Bool mac2MonoInit();
extern Bool mac2MonoCloseScreen();


static void
mac2ColorUpdateColormap(pScreen, index, count, pdefs)
     ScreenPtr	pScreen;
     int	index, count; /* update #count entries from index onwards */
     register xColorItem *pdefs;
{
  /* $$NOTE: fix error recovery mechanism in mac2VideoCLUT -- Sohan */
  (void) mac2VideoSetCLUT(pScreen->myNum, index, count, pdefs);
}

mac2ColorUpdateClass(pScreen, class)
     ScreenPtr	pScreen;
     short class;
{
    mac2VideoSetGray(pScreen->myNum, (class == GrayScale));
}

/*-
 *-----------------------------------------------------------------------
 * mac2ColorSaveScreen --
 *	Disable the video on the frame buffer to save the screen.
 *
 * Results:
 *	None.
 *
 * Side Effects:
 *	Video enable state changes.
 *
 *-----------------------------------------------------------------------
 */
static Bool
mac2ColorSaveScreen (pScreen, on)
    ScreenPtr	  pScreen;
    Bool    	  on;
{
    int         state = on;
    if (on != SCREEN_SAVER_ON) {
      SetTimeSinceLastInputEvent();
	state = 1;
    } else {
	state = 0;
    }
    return TRUE;
}


/*-
 *-----------------------------------------------------------------------
 * mac2ColorCloseScreen --
 *	called to ensure video is enabled when server exits.
 *
 * Results:
 *	Screen is unsaved.
 *
 * Side Effects:
 *	None
 *
 *-----------------------------------------------------------------------
 */
/*ARGSUSED*/
Bool
mac2ColorCloseScreen(i, pScreen)
    int		i;
    ScreenPtr	pScreen;
{
    extern mac2WhiteScreen();

    mac2WhiteScreen(pScreen->myNum);
    /* $$NOTE: reset colormap and screen to 1-bit monochrome mode here --- Sohan */
    mac2Fbs[pScreen->myNum].installedMap = NULL;
    return (pScreen->SaveScreen(pScreen, SCREEN_SAVER_OFF));

}



/*-
 *-----------------------------------------------------------------------
 * mac2ColorInstallColormap --
 *	Install given colormap.
 *
 * Results:
 *	None
 *
 * Side Effects:
 *	Existing map is uninstalled.
 *	All clients requesting ColormapNotify are notified
 *
 *-----------------------------------------------------------------------
 */
static void
mac2ColorInstallColormap(cmap)
    ColormapPtr	cmap;
{
    register int i;
    register Entry *pent;
    xColorItem *newMap;
    register VisualPtr pVisual = cmap->pVisual;
    register xColorItem *pc;
    ColormapPtr mac2ColorInstalledMap;

    mac2ColorInstalledMap = mac2Fbs[cmap->pScreen->myNum].installedMap;
    if (cmap == mac2ColorInstalledMap)
      return;
    if (mac2ColorInstalledMap)
      WalkTree(mac2ColorInstalledMap->pScreen, TellLostMap,
	       (char *) &(mac2ColorInstalledMap->mid));
    newMap = (xColorItem *)calloc(pVisual->ColormapEntries, sizeof(xColorItem));
    if ( (pVisual->class | DynamicClass) == DirectColor){
	for (i = 0, pc = newMap; i < pVisual->ColormapEntries; i++, pc++) {
	    pc->pixel = i;
	    pent = &cmap->red[(i & pVisual->redMask) >> pVisual->offsetRed];
	    if (pent->fShared)
		pc->red = pent->co.shco.red->color;
	    else
		pc->red = pent->co.local.red;
	    pent = &cmap->green[(i & pVisual->greenMask) >> pVisual->offsetGreen];
	    if (pent->fShared)
		pc->green = pent->co.shco.green->color;
	    else
		pc->green = pent->co.local.green;
	    pent = &cmap->blue[(i & pVisual->blueMask) >> pVisual->offsetBlue];
	    if (pent->fShared)
		pc->blue = pent->co.shco.blue->color;
	    else
		pc->blue = pent->co.local.blue;
	}
    } else {
	for (i = 0, pc = newMap, pent = cmap->red; i < pVisual->ColormapEntries; i++, pc++) {
	    pc->pixel = i;
	    if (pent->fShared) {
		pc->red = pent->co.shco.red->color;
		pc->green = pent->co.shco.green->color;
		pc->blue  = pent->co.shco.blue->color;
	    } else {
		pc->red = pent->co.local.red;
		pc->green = pent->co.local.green;
		pc->blue = pent->co.local.blue;
	    }
	    pent++;
	}
    }
    mac2Fbs[cmap->pScreen->myNum].installedMap = cmap;
    mac2ColorUpdateClass(cmap->pScreen, pVisual->class);
    mac2ColorUpdateColormap(cmap->pScreen, 0, pVisual->ColormapEntries, newMap);
    WalkTree(cmap->pScreen, TellGainedMap, (char *) &(cmap->mid));
    free((char*)newMap);
}

/*-
 *-----------------------------------------------------------------------
 * mac2ColorUninstallColormap --
 *	Uninstall given colormap.
 *
 * Results:
 *	None
 * Side Effects:
 *	default map is installed
 *	All clients requesting ColormapNotify are notified
 *
 *-----------------------------------------------------------------------
 */
static void
mac2ColorUninstallColormap(cmap)
    ColormapPtr	cmap;
{
  ColormapPtr mac2ColorInstalledMap;

  mac2ColorInstalledMap = mac2Fbs[cmap->pScreen->myNum].installedMap;
  if (cmap == mac2ColorInstalledMap) {
    Colormap defMapID = cmap->pScreen->defColormap;

    if (cmap->mid != defMapID) {
      ColormapPtr defMap = (ColormapPtr) LookupIDByType(defMapID, RT_COLORMAP);

      if (defMap)
	(*cmap->pScreen->InstallColormap)(defMap);
      else
	ErrorF("mac2Color: Can't find default colormap\n");
    }
  }
}


/*-
 *-----------------------------------------------------------------------
 * mac2ColorListInstalledColormaps --
 *	Fills in the list with the IDs of the installed maps
 *
 * Results:
 *	Returns the number of IDs in the list
 * * Side Effects:
 *	None
 *
 *-----------------------------------------------------------------------
 */
/*ARGSUSED*/
static int
mac2ColorListInstalledColormaps(pScreen, pCmapList)
    ScreenPtr	pScreen;
    Colormap	*pCmapList;
{
    *pCmapList = mac2Fbs[pScreen->myNum].installedMap->mid;
    return (1);
}


/*-
 *-----------------------------------------------------------------------
 * mac2ColorStoreColors --
 *	Sets the pixels in pdefs into the specified map.
 *
 * Results:
 *	None
 * * Side Effects:
 *	None
 *
 *-----------------------------------------------------------------------
 */
static void
mac2ColorStoreColors(pmap, ndef, pdefs)
     ColormapPtr	pmap;
     int		ndef;
     xColorItem	*pdefs;
{
  ColormapPtr mac2ColorInstalledMap;

  mac2ColorInstalledMap = mac2Fbs[pmap->pScreen->myNum].installedMap;
  switch (pmap->class) {
  case PseudoColor:
    case GrayScale:
    if (pmap == mac2ColorInstalledMap) {
      /* We only have a single colormap */
      /*
       * $$NOTE: Zach gave me a story about having to zap a whole colormap
       * into the CLUT at one shot, but the code doesn't seem to indicate
       * so. It seems as if, even though colors get "stacked" from a given,
       * address, by zapping in only one color at a time and controlling
       * the address of zap as in mac2VideoSetCLUT, we have a Random-Write-Memory!!
       * Hope this is right. This can be verified only after we have an
       * otherwise working server and try to change colormap entries.
       *				-- Sohan
       */
      mac2ColorUpdateClass(pmap->pScreen, pmap->class);
      mac2ColorUpdateColormap(pmap->pScreen, 0, ndef, pdefs);
    }
    break;
  case DirectColor:
  default:
    ErrorF("mac2ColorStoreColors: bad class %d\n", pmap->class);
    break;
  }
}



/*-
 *-----------------------------------------------------------------------
 * mac2ColorInit --
 *	Initialize the mac2 framebuffer
 *
 * Results:
 *	None
 *
 * Side Effects:
 *	Most of the elements of the ScreenRec are filled in.  The
 *	video is enabled for the frame buffer...
 *
 *-----------------------------------------------------------------------
 */
/*ARGSUSED*/
Bool
mac2ColorInit (index, pScreen, argc, argv)
    int	    	  index;    	/* The index of pScreen in the ScreenInfo */
    ScreenPtr	  pScreen;  	/* The Screen to initialize */
    int	    	  argc;	    	/* The number of the Server's arguments. */
    char    	  **argv;   	/* The arguments themselves. Don't change! */
{
    ColormapPtr pColormap;
    PixmapPtr   pPixmap;

    if (!cfbScreenInit(pScreen,
			mac2Fbs[index].fb,
			mac2Fbs[index].info[mac2Fbs[index].mode].vpBounds.right -
			mac2Fbs[index].info[mac2Fbs[index].mode].vpBounds.left,
			mac2Fbs[index].info[mac2Fbs[index].mode].vpBounds.bottom -
			mac2Fbs[index].info[mac2Fbs[index].mode].vpBounds.top, 
			mac2Fbs[index].info[mac2Fbs[index].mode].vpHRes >> 16,
			mac2Fbs[index].info[mac2Fbs[index].mode].vpVRes >> 16,
			mac2Fbs[index].info[mac2Fbs[index].mode].vpBounds.right -
			mac2Fbs[index].info[mac2Fbs[index].mode].vpBounds.left))
	return (FALSE);

    /* mac2 screens may have extra video memory to the right of the visible
     * area, therefore the PixmapBytePad macro in cfbScreenInit gave the 
     * wrong value to the devKind field of the Pixmap it made for the screen.
     * So we fix it here. */

    pPixmap = (PixmapPtr)(pScreen->devPrivate);
    pPixmap->devKind =  mac2Fbs[index].info[mac2Fbs[index].mode].vpRowBytes;

    pScreen->SaveScreen = mac2ColorSaveScreen;
    pScreen->CloseScreen = mac2ColorCloseScreen;
#ifndef STATIC_COLOR
    pScreen->InstallColormap = mac2ColorInstallColormap;
    pScreen->UninstallColormap = mac2ColorUninstallColormap;
    pScreen->ListInstalledColormaps = mac2ColorListInstalledColormaps;
    pScreen->StoreColors = mac2ColorStoreColors;
#endif
    (void) mac2ColorSaveScreen(pScreen, SCREEN_SAVER_FORCER);

    return ( mac2ScreenInit(pScreen) && cfbCreateDefColormap(pScreen) );
}

/*-
 *-----------------------------------------------------------------------
 * mac2ColorProbe --
 *	Attempt to find and initialize a mac2 framebuffer
 *
 * Results:
 *	None
 *
 * Side Effects:
 *	Memory is allocated for the frame buffer and the buffer is mapped. 
 *
 *-----------------------------------------------------------------------
 */

Bool
mac2ColorProbe(pScreenInfo, index, fbNum, argc, argv)
    ScreenInfo	  *pScreenInfo;	/* The screenInfo struct */
    int	    	  index;    	/* The index of pScreen in the ScreenInfo */
    int	    	  fbNum;    	/* Index into the mac2FbData array */
    int	    	  argc;	    	/* The number of the Server's arguments. */
    char    	  **argv;   	/* The arguments themselves. Don't change! */
{
    int i, mode, depth, depthfound, oldNumScreens;
    int fd = -1;

    if (mac2FbData[fbNum].probeStatus == probedAndFailed) {
	return FALSE;
    }
    if (mac2FbData[fbNum].probeStatus == neverProbed) {
	if ((fd = mac2OpenFrameBuffer(index, fbNum,
				     argc, argv)) < 0) {
	    mac2FbData[fbNum].probeStatus = probedAndFailed;
	    return FALSE;
	}
	mac2FbData[fbNum].probeStatus = probedAndSucceeded;
    }

    /*
     * If we've ever successfully probed this device, do the following.
     */
    oldNumScreens = pScreenInfo->numScreens;

    /* figure out the depth to use */
    depth = ( mac2FbData[fbNum].depthRequested == DEFAULTDEPTH ) ? mac2Fbs[index].default_depth : mac2FbData[fbNum].depthRequested;
    depthfound = FALSE;
    if ( depth == 1 )
    {
	/* our initialization routine has set the right pixel size etc.; */
	/* therefore, do nothing */
	depthfound = TRUE;
    }
    else
    {
	if (mac2VideoSetPixelSize(index, depth) == Success )
	    depthfound = TRUE;
    }
    if ( !depthfound )
    {
	FatalError("A screen depth of %d not currently supported by this device\n");
    }
    else
    {
	/* map the frame buffer in */
	int chunksize;
	caddr_t fb_virtaddr;
	int fbsize;

	/* allocate a chunk of page-aligned memory to map the frame buffer onto */
	chunksize = getpagesize();
	/*
	 if ( ioctl(fd, VIDEO_SIZE, &fbsize) < 0 )
	 {
	     FatalError("could not obtain size of video device( ioctl VIDEO_SIZE failed!)\n");
	     (void) close (fd); return (!Success);
	 }
	 */
	/* Since we canot determine the fbsize via some system call, we use the following heuristic */
	fbsize = (mac2Fbs[index].info[mac2Fbs[index].mode].vpRowBytes *(mac2Fbs[index].info[mac2Fbs[index].mode].vpBounds.bottom-mac2Fbs[index].info[mac2Fbs[index].mode].vpBounds.top) + mac2Fbs[index].info[mac2Fbs[index].mode].vpBaseOffset);
	if ( (fb_virtaddr = (caddr_t)malloc((unsigned)fbsize + (unsigned)chunksize)) == (caddr_t)NULL )
	{
	    FatalError ("Cannot allocate enough memory for frame buffer");
	    (void) close (fd);
	    return FALSE;
	}
	fb_virtaddr = (caddr_t)(((int)fb_virtaddr/chunksize+1)*chunksize);
	/* map frame buffer */
	if (mmap(fb_virtaddr, fbsize, PROT_READ|PROT_WRITE, MAP_SHARED, fd, (off_t)0) < 0)
	{
	    FatalError ("MMAP failed");
	    (void) close (fd);
	    return FALSE;
	}
	mac2Fbs[index].fb = (pointer)((unsigned)fb_virtaddr + mac2Fbs[index].info[mac2Fbs[index].mode].vpBaseOffset);
    }

    if ( mac2Fbs[index].info[mac2Fbs[index].mode].vpPixelSize == 1 )
    {
	i = AddScreen(mac2MonoInit, argc, argv);
	pScreenInfo->screens[index]->CloseScreen = mac2MonoCloseScreen;
    }
    else if ( mac2Fbs[index].info[mac2Fbs[index].mode].vpPixelSize == 8 )
      {
	i = AddScreen(mac2ColorInit, argc, argv);
	pScreenInfo->screens[index]->CloseScreen = mac2ColorCloseScreen;
      }
    else
      {
	FatalError("Screen depth %d not currently supported by this server\n",
	  mac2Fbs[index].info[mac2Fbs[index].mode].vpPixelSize );
      }
    return (i >= 0);
}

