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
 * mac2Mono.c --
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
#include    "resource.h"

#include <sys/ioctl.h>
#include <sys/mman.h>
#include <sys/types.h>

/*-
 *-----------------------------------------------------------------------
 * mac2MonoSaveScreen --
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
mac2MonoSaveScreen (pScreen, on)
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
 * mac2MonoCloseScreen --
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
mac2MonoCloseScreen(i, pScreen)
    int		i;
    ScreenPtr	pScreen;
{
/* #ifdef never */
    extern mac2WhiteScreen();

    mac2WhiteScreen(pScreen->myNum);
    return (pScreen->SaveScreen(pScreen, SCREEN_SAVER_OFF));
/* #endif never */
}

/*-
 *-----------------------------------------------------------------------
 * mac2MonoResolveColor --
 *	Resolve an RGB value into some sort of thing we can handle.
 *	Just looks to see if the intensity of the color is greater than
 *	1/2 and sets it to 'white' (all ones) if so and 'black' (all zeroes)
 *	if not.
 *
 * Results:
 *	*pred, *pgreen and *pblue are overwritten with the resolved color.
 *
 * Side Effects:
 *	see above.
 *
 *-----------------------------------------------------------------------
 */
/*ARGSUSED*/
static void
mac2MonoResolveColor(pred, pgreen, pblue, pVisual)
    unsigned short	*pred;
    unsigned short	*pgreen;
    unsigned short	*pblue;
    VisualPtr		pVisual;
{
    /* 
     * Gets intensity from RGB.  If intensity is >= half, pick white, else
     * pick black.  This may well be more trouble than it's worth.
     */

    *pred = *pgreen = *pblue = 
        (((39L * (long)*pred +
           50L * (long)*pgreen +
           11L * (long)*pblue) >> 8) >= (((1<<8)-1)*50)) ? ~0 : 0;
    
}

/*-
 *-----------------------------------------------------------------------
 * mac2MonoCreateColormap --
 *	create a bw colormap
 *
 * Results:
 *	None
 *
 * Side Effects:
 *	allocate two pixels
 *
 *-----------------------------------------------------------------------
 */
Bool
mac2MonoCreateColormap(pmap)
    ColormapPtr	pmap;
{
    int	red, green, blue, pix;

    /* this is a monochrome colormap, it only has two entries, just fill
     * them in by hand.  If it were a more complex static map, it would be
     * worth writing a for loop or three to initialize it */

    /* this will be pixel 0 */
    pix = 0;
    red = green = blue = ~0;
    AllocColor(pmap, &red, &green, &blue, &pix, 0);

    /* this will be pixel 1 */
    red = green = blue = 0;
    AllocColor(pmap, &red, &green, &blue, &pix, 0);

    return TRUE;
}

/*-
 *-----------------------------------------------------------------------
 * mac2MonoDestroyColormap --
 *	destroy a bw colormap
 *
 * Results:
 *	None
 *
 * Side Effects:
 *	None
 *
 *-----------------------------------------------------------------------
 */
/*ARGSUSED*/
void
mac2MonoDestroyColormap(pmap)
    ColormapPtr	pmap;
{
}

/*-
 *-----------------------------------------------------------------------
 * mac2MonoInit --
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
mac2MonoInit (index, pScreen, argc, argv)
    int	    	  index;    	/* The index of pScreen in the ScreenInfo */
    ScreenPtr	  pScreen;  	/* The Screen to initialize */
    int	    	  argc;	    	/* The number of the Server's arguments. */
    char    	  **argv;   	/* The arguments themselves. Don't change! */
{
    ColormapPtr pColormap;
    PixmapPtr   pPixmap;

    if (!mfbScreenInit(pScreen,
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
     * area, therefore the PixmapBytePad macro in mfbScreenInit gave the 
     * wrong value to the devKind field of the Pixmap it made for the screen.
     * So we fix it here. */

    pPixmap = (PixmapPtr)(pScreen->devPrivate);
    pPixmap->devKind =  mac2Fbs[index].info[mac2Fbs[index].mode].vpRowBytes;

    pScreen->SaveScreen = mac2MonoSaveScreen;
    pScreen->CloseScreen = mac2MonoCloseScreen;
    /*
      pScreen->ResolveColor = mac2MonoResolveColor;
      pScreen->CreateColormap = mac2MonoCreateColormap;
      pScreen->DestroyColormap = mac2MonoDestroyColormap;
      */
    pScreen->whitePixel = 0;
    pScreen->blackPixel = 1;

    /*
    if (CreateColormap(pScreen->defColormap, pScreen,
		       pScreen->rootVisual,
		       &pColormap, AllocNone, 0) != Success
	|| pColormap == NULL)
	    FatalError("Can't create colormap in mac2MonoInit()\n");
    mfbInstallColormap(pColormap);
    */

    /*
     * Enable video output...? 
     */
    (void) mac2MonoSaveScreen(pScreen, SCREEN_SAVER_FORCER);

    return( mac2ScreenInit(pScreen) && mfbCreateDefColormap(pScreen) );
}

