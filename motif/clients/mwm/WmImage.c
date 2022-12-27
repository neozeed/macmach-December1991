#ifdef REV_INFO
#ifndef lint
static char SCCSID[] = "OSF/Motif: @(#)WmImage.c	3.13.1.2 91/03/20";
#endif /* lint */
#endif /* REV_INFO */
 /*****************************************************************************
 ******************************************************************************
 *
 * (c) Copyright 1989, 1990, 1991 OPEN SOFTWARE FOUNDATION, INC.
 * (c) Copyright 1987, 1988, 1989, 1990 HEWLETT-PACKARD COMPANY
 * ALL RIGHTS RESERVED
 *
 * 	THIS SOFTWARE IS FURNISHED UNDER A LICENSE AND MAY BE USED
 * AND COPIED ONLY IN ACCORDANCE WITH THE TERMS OF SUCH LICENSE AND
 * WITH THE INCLUSION OF THE ABOVE COPYRIGHT NOTICE.  THIS SOFTWARE OR
 * ANY OTHER COPIES THEREOF MAY NOT BE PROVIDED OR OTHERWISE MADE
 * AVAILABLE TO ANY OTHER PERSON.  NO TITLE TO AND OWNERSHIP OF THE
 * SOFTWARE IS HEREBY TRANSFERRED.
 *
 * 	THE INFORMATION IN THIS SOFTWARE IS SUBJECT TO CHANGE WITHOUT
 * NOTICE AND SHOULD NOT BE CONSTRUED AS A COMMITMENT BY OPEN SOFTWARE
 * FOUNDATION, INC. OR ITS THIRD PARTY SUPPLIERS
 *
 * 	OPEN SOFTWARE FOUNDATION, INC. AND ITS THIRD PARTY SUPPLIERS,
 * ASSUME NO RESPONSIBILITY FOR THE USE OR INABILITY TO USE ANY OF ITS
 * SOFTWARE .   OSF SOFTWARE IS PROVIDED "AS IS" WITHOUT WARRANTY OF ANY
 * KIND, AND OSF EXPRESSLY DISCLAIMS ALL IMPLIED WARRANTIES, INCLUDING
 * BUT NOT LIMITED TO THE IMPLIED WARRANTIES OF MERCHANTABILITY AND
 * FITNESS FOR A PARTICULAR PURPOSE.
 *
 * Notice:  Notwithstanding any other lease or license that may pertain to,
 * or accompany the delivery of, this computer software, the rights of the
 * Government regarding its use, reproduction and disclosure are as set
 * forth in Section 52.227-19 of the FARS Computer Software-Restricted
 * Rights clause.
 *
 * (c) Copyright 1989,1990, 1991 Open Software Foundation, Inc.  Unpublished - all
 * rights reserved under the Copyright laws of the United States.
 *
 * RESTRICTED RIGHTS NOTICE:  Use, duplication, or disclosure by the
 * Government is subject to the restrictions as set forth in subparagraph
 * (c)(1)(ii) of the Rights in Technical Data and Computer Software clause
 * at DFARS 52.227-7013.
 *
 * Open Software Foundation, Inc.
 * 11 Cambridge Center
 * Cambridge, MA   02142
 * (617)621-8700
 *
 * RESTRICTED RIGHTS LEGEND:  This computer software is submitted with
 * "restricted rights."  Use, duplication or disclosure is subject to the
 * restrictions as set forth in NASA FAR SUP 18-52.227-79 (April 1985)
 * "Commercial Computer Software- Restricted Rights (April 1985)."  Open
 * Software Foundation, Inc., 11 Cambridge Center, Cambridge, MA  02142.  If
 * the contract contains the Clause at 18-52.227-74 "Rights in Data General"
 * then the "Alternate III" clause applies.
 *
 * (c) Copyright 1989,1990, 1991 Open Software Foundation, Inc.
 * ALL RIGHTS RESERVED
 *
 *
 * Open Software Foundation is a trademark of The Open Software Foundation, Inc.
 * OSF is a trademark of Open Software Foundation, Inc.
 * OSF/Motif is a trademark of Open Software Foundation, Inc.
 * Motif is a trademark of Open Software Foundation, Inc.
 * DEC is a registered trademark of Digital Equipment Corporation
 * DIGITAL is a registered trademark of Digital Equipment Corporation
 * X Window System is a trademark of the Massachusetts Institute of Technology
 *
 ******************************************************************************
 *****************************************************************************/



/*
 * Included Files:
 */

#include "WmGlobal.h"
#include "WmIBitmap.h"
#include <stdio.h>
#include <pwd.h>

#define MAXPATH 1023

/*
 * include extern functions
 */

#include "WmImage.h"
#include "WmGraphics.h"
#include "WmResource.h"
#include "WmResParse.h"

extern char    *getenv ();



/******************************<->*************************************
 *
 *  MakeClientIconPixmap (pCD, iconBitmap)
 *
 *
 *  Description:
 *  -----------
 *  This function takes a client supplied icon image bitmap and makes it
 *  into a colored pixmap suitable for use as an icon image.
 *
 *
 *  Inputs:
 *  ------
 *  pCD = pointer to client data (icon colors and tiles)
 *
 *  iconBitmap = uncolored and formated icon bitmap
 *
 * 
 *  Outputs:
 *  -------
 *  RETURN = icon pixmap (NULL if error)
 *
 ******************************<->***********************************/

#ifdef _NO_PROTO
Pixmap MakeClientIconPixmap (pCD, iconBitmap)

    ClientData *pCD;
    Pixmap iconBitmap;

#else /* _NO_PROTO */
Pixmap MakeClientIconPixmap (ClientData *pCD, Pixmap iconBitmap)
#endif /* _NO_PROTO */
{
    Window root;
    int x;
    int y;
    unsigned int bitmapWidth;
    unsigned int bitmapHeight;
    unsigned int border;
    unsigned int depth;

    
    /*
     * Check out the attributes of the bitmap to insure that it is usable.
     */

    if (!XGetGeometry (DISPLAY, iconBitmap, &root, &x, &y,
	     &bitmapWidth, &bitmapHeight, &border, &depth))
    {
	Warning ("Invalid icon bitmap");
	return (NULL);
    }

    if (ROOT_FOR_CLIENT(pCD) != root)
    {
	/*
	 * The bitmap was not made with usable parameters.
	 */
	Warning ("Invalid root for icon bitmap");
	return (NULL);
    }

#ifdef DISALLOW_DEEP_ICONS
    if (depth != 1)
    {
	Warning ("Warning color icon pixmap not supported");
	return (NULL);
    }
#endif

    /*
     * Color the bitmap, center it in a pixmap ....
     */

    return (MakeIconPixmap (pCD, iconBitmap, NULL, 
	    bitmapWidth, bitmapHeight));


} /* END OF FUNCTION MakeClientIconPixmap */



/*************************************<->*************************************
 *
 *  MakeNamedIconPixmap (pCD, iconName)
 *
 *
 *  Description:
 *  -----------
 *  This function makes an icon pixmap for a particular client given the
 *  name of a bitmap file.
 *
 *
 *  Inputs:
 *  ------
 *  pCD      = (nonNULL) pointer to client data
 *  iconName = pointer to the icon name (bitmap file path name or NULL)
 *
 * 
 *  Outputs:
 *  -------
 *  RETURN = icon pixmap or NULL
 * 
 *************************************<->***********************************/

#ifdef _NO_PROTO
Pixmap MakeNamedIconPixmap (pCD, iconName)

    ClientData *pCD;
    String      iconName;

#else /* _NO_PROTO */
Pixmap MakeNamedIconPixmap (ClientData *pCD, String iconName)
#endif /* _NO_PROTO */
{
    int          bitmapIndex;

    /*
     * Get the bitmap cache entry (will read data from file if necessary).
     * If unable to find the iconName file return NULL.
     */

    if ((bitmapIndex = GetBitmapIndex (PSD_FOR_CLIENT(pCD), iconName)) < 0)
    {
	return (NULL);
    }

    /*
     * Color the bitmap, center it in a pixmap ....
     */

    return (MakeCachedIconPixmap (pCD, bitmapIndex, NULL));

} /* END OF FUNCTION MakeNamedIconPixmap */




/*************************************<->*************************************
 *
 *  Pixmap
 *  MakeCachedIconPixmap (pCD, bitmapIndex, mask)
 *
 *
 *  Description:
 *  -----------
 *  Convert the cached bitmap and mask into an icon pixmap.
 *
 *
 *  Inputs:
 *  ------
 *  pCD		- (nonNULL) pointer to client data (icon colors and tiles)
 *  bitmapIndex	- bitmap cache index of image to be converted
 *  mask	- bitmap mask, 1 for bits of "bitmap" to be kept
 *
 * 
 *  Outputs:
 *  -------
 *  RETURN 	- icon pixmap or NULL
 *
 *
 *  Comments:
 *  --------
 *  o "mask" is not used.
 * 
 *************************************<->***********************************/
#ifdef _NO_PROTO
Pixmap MakeCachedIconPixmap (pCD, bitmapIndex, mask)

    ClientData *pCD;
    int         bitmapIndex;
    Pixmap      mask;

#else /* _NO_PROTO */
Pixmap MakeCachedIconPixmap (ClientData *pCD, int bitmapIndex, Pixmap mask)
#endif /* _NO_PROTO */
{
    BitmapCache  *bitmapc;
    PixmapCache  *pixmapc;
    Pixmap        pixmap = NULL;
    WmScreenData *pSD = PSD_FOR_CLIENT(pCD);

    if (bitmapIndex < 0)
    {
	return (NULL);
    }
    bitmapc = &(pSD->bitmapCache[bitmapIndex]);

    /*
     * Search for an icon pixmap matching the client icon colors.
     */

    pixmapc = bitmapc->pixmapCache;
    while (pixmapc)
    {
        if ((pixmapc->pixmapType == ICON_PIXMAP) &&
	    (pixmapc->foreground == pCD->iconImageForeground) &&
	    (pixmapc->background == pCD->iconImageBackground))
        {
	    pixmap = pixmapc->pixmap;
	    break;
        }
	pixmapc = pixmapc->next;
    }

    /* 
     * If a matching pixmap was not found in the pixmap cache for this bitmap
     *   then create the icon pixmap with the appropriate colors.
     * If have sufficient memory, save the pixmap info in the pixmapCache.
     */

    if (!pixmap &&
        (pixmap = MakeIconPixmap (pCD, bitmapc->bitmap, mask,
                                         bitmapc->width, bitmapc->height)) &&
	(pixmapc = (PixmapCache *) malloc (sizeof (PixmapCache))))
    {

        pixmapc->pixmapType = ICON_PIXMAP;
	pixmapc->foreground = pCD->iconImageForeground;
	pixmapc->background = pCD->iconImageBackground;
	pixmapc->pixmap = pixmap;
	pixmapc->next = bitmapc->pixmapCache;
	bitmapc->pixmapCache = pixmapc;
    }

    return (pixmap);

} /* END OF FUNCTION MakeCachedIconPixmap */



/*************************************<->*************************************
 *
 *  MakeIconPixmap (pCD, bitmap, mask, width, height)
 *
 *
 *  Description:
 *  -----------
 *  Convert the bitmap and mask into an icon pixmap.
 *
 *
 *  Inputs:
 *  ------
 *  pCD		- pointer to client data (icon colors and tiles)
 *  pWS		- pointer to workspace data
 *  bitmap	- bitmap image to be converted
 *  mask	- bitmap mask, 1 for bits of "bitmap" to be kept
 *  width	- pixel width of bitmap
 *  height	- pixel height of bitmap
 *
 * 
 *  Outputs:
 *  -------
 *  RETURN 	- icon pixmap or NULL
 *
 *
 *  Comments:
 *  --------
 *  o "mask" is not used.
 * 
 *************************************<->***********************************/
#ifdef _NO_PROTO
Pixmap MakeIconPixmap (pCD, bitmap, mask, width, height)

    ClientData  *pCD;
    Pixmap       bitmap;
    Pixmap       mask;
    unsigned int width;
    unsigned int height;

#else /* _NO_PROTO */
Pixmap MakeIconPixmap (ClientData *pCD, Pixmap bitmap, Pixmap mask, unsigned int width, unsigned int height)
#endif /* _NO_PROTO */
{
    Pixmap       iconPixmap;
    GC           imageGC, topGC, botGC;
    XGCValues    gcv;
    unsigned int imageWidth;
    unsigned int imageHeight;
    int          dest_x, dest_y;
    Pixel        fg;
    Pixel        bg;
    static RList *top_rects = NULL;
    static RList *bot_rects = NULL;
    WmScreenData	*pSD;

    if ((top_rects == NULL) && 
	(top_rects = AllocateRList 
	((unsigned)2 * ICON_INTERNAL_SHADOW_WIDTH)) == NULL)
    {
	/* Out of memory! */
	Warning ("Insufficient memory to bevel icon image");
	return (NULL);
    }

    if ((bot_rects == NULL) &&
	(bot_rects = AllocateRList 
	((unsigned)2 * ICON_INTERNAL_SHADOW_WIDTH)) == NULL)
    {
	/* Out of memory! */
	Warning ("Insufficient memory to bevel icon image");
	return (NULL);
    }

    if (pCD)
    {
	pSD = pCD->pSD;
    }
    else 
    {
	pSD = wmGD.pActiveSD;
    }

    /* don't make icon pixmap if bitmap is too small */

    if ((width < pSD->iconImageMinimum.width) ||
	(height < pSD->iconImageMinimum.height))
    {
	/* bitmap is too small */
	return (NULL);
    }

    imageWidth = pSD->iconImageMaximum.width + 
		  2 * ICON_INTERNAL_SHADOW_WIDTH;
    imageHeight = pSD->iconImageMaximum.height +
		  2 * ICON_INTERNAL_SHADOW_WIDTH;

    /* create a pixmap (to be returned) */

    iconPixmap = XCreatePixmap (DISPLAY, pSD->rootWindow, 
		     imageWidth, imageHeight,
		     DefaultDepth(DISPLAY, pSD->screen));

    /*
     * If a client is not specified use icon component colors, otherwise
     * use the client-specific icon colors.
     */

    if (pCD)
    {
	bg = pCD->iconImageBackground;
	fg = pCD->iconImageForeground;
    }
    else
    {
	bg = pSD->iconAppearance.background;
	fg = pSD->iconAppearance.foreground;
    }

    /* create a GC to use */
    gcv.foreground = bg;	/* clear it first! */
    gcv.background = bg;
    gcv.graphics_exposures = False;

    imageGC = XCreateGC (DISPLAY, iconPixmap, (GCForeground|GCBackground),
		  &gcv);

    /*
     * Format the image. 
     */

    /* fill in background */

    XFillRectangle(DISPLAY, iconPixmap, imageGC, 0, 0, 
		   imageWidth, imageHeight);

    /* center the image */

    if (width > pSD->iconImageMaximum.width)
    {
	width = pSD->iconImageMaximum.width;
    }
    if (height > pSD->iconImageMaximum.height)
    {
	height = pSD->iconImageMaximum.height;
    }
    /* center the image */

    dest_x = (imageWidth - width) / 2;
    dest_y = (imageHeight - height) / 2;

    /* set the foreground */
    XSetForeground (DISPLAY, imageGC, fg);

    /* copy the bitmap to the pixmap */
    XCopyPlane (DISPLAY, bitmap, iconPixmap, imageGC, 0, 0, width, height, 
		dest_x, dest_y, 1L);

    /* free resources */
    XFreeGC (DISPLAY, imageGC);

    if (pCD)
    {
	/*
	 * Shadowing
	 */

	topGC = GetHighlightGC (pSD, pCD->iconImageTopShadowColor, 
				  pCD->iconImageBackground,
				  pCD->iconImageTopShadowPixmap);

	botGC = GetHighlightGC (pSD, pCD->iconImageBottomShadowColor, 
				  pCD->iconImageBackground,
				  pCD->iconImageBottomShadowPixmap);

	BevelRectangle (top_rects, 
			bot_rects, 
			0, 0,
			imageWidth, imageHeight,
			ICON_INTERNAL_SHADOW_WIDTH,
			ICON_INTERNAL_SHADOW_WIDTH,
			ICON_INTERNAL_SHADOW_WIDTH,
			ICON_INTERNAL_SHADOW_WIDTH);

	XFillRectangles (DISPLAY, iconPixmap, topGC, top_rects->prect, 
							top_rects->used);
	XFillRectangles (DISPLAY, iconPixmap, botGC, bot_rects->prect,
							bot_rects->used);
    }
    return (iconPixmap);

} /* END OF FUNCTION MakeIconPixmap */



/*************************************<->*************************************
 *
 *  Pixmap
 *  MakeCachedLabelPixmap (pSD, menuW, bitmapIndex)
 *
 *
 *  Description:
 *  -----------
 *  Creates and returns a label pixmap.
 *
 *
 *  Inputs:
 *  ------
 *  pSD = pointer to screen data
 *  menuW = menu widget (for foreground and background colors)
 *  bitmapIndex = bitmap cache index
 *
 * 
 *  Outputs:
 *  -------
 *  Return = label pixmap or NULL.
 *
 *
 *  Comments:
 *  --------
 *  Assumes bitmapIndex is valid.
 * 
 *************************************<->***********************************/

#ifdef _NO_PROTO
Pixmap MakeCachedLabelPixmap (pSD, menuW, bitmapIndex)

    WmScreenData  *pSD;
    Widget        menuW;
    int           bitmapIndex;

#else /* _NO_PROTO */
Pixmap MakeCachedLabelPixmap (WmScreenData *pSD, Widget menuW, int bitmapIndex)
#endif /* _NO_PROTO */
{
    BitmapCache  *bitmapc;
    PixmapCache  *pixmapc;
    int           i;
    Arg           args[5];
    Pixel         fg, bg;
    Pixmap        pixmap = NULL;
    GC            gc;
    XGCValues     gcv;

    if (bitmapIndex < 0)
    {
	return (NULL);
    }
    bitmapc = &(pSD->bitmapCache[bitmapIndex]);

    /*
     * Get the foreground and background colors from the menu widget.
     * Search for a label pixmap matching those colors.
     */

    i = 0;
    XtSetArg (args[i], XtNforeground, &fg); i++;
    XtSetArg (args[i], XtNbackground, &bg); i++;
    XtGetValues (menuW, (ArgList)args, i);

    pixmapc = bitmapc->pixmapCache;
    while (pixmapc)
    {
        if ((pixmapc->pixmapType == LABEL_PIXMAP) &&
	    (pixmapc->foreground == fg) &&
	    (pixmapc->background == bg))
        {
	    pixmap = pixmapc->pixmap;
	    break;
        }
	pixmapc = pixmapc->next;
    }

    if (!pixmap)
    /* 
     * A matching pixmap was not found in the pixmap cache for this bitmap.
     * Create and save the label pixmap with appropriate colors.
     */
    {
        /* 
         * Create a pixmap of the appropriate size, root, and depth.
         * Only BadAlloc error possible; BadDrawable and BadValue are avoided.
         */

        pixmap = XCreatePixmap (DISPLAY, pSD->rootWindow, 
				bitmapc->width, bitmapc->height,
                                DefaultDepth (DISPLAY, pSD->screen));

        /*
	 * Create a GC and copy the bitmap to the pixmap.
         * Only BadAlloc and BadDrawable errors are possible; others are avoided
         */

        gcv.foreground = bg;
        gcv.background = bg;
	gcv.graphics_exposures = False;
        gc = XCreateGC(DISPLAY, pixmap, (GCForeground|GCBackground), &gcv);
   
        /*
	 * Fill in the background, set the foreground, copy the bitmap to the 
         * pixmap, and free the gc.
         */

        XFillRectangle (DISPLAY, pixmap, gc, 0, 0,
		        bitmapc->width, bitmapc->height);
        XSetForeground (DISPLAY, gc, fg);
        XCopyPlane (DISPLAY, bitmapc->bitmap, pixmap, gc, 0, 0,
                    bitmapc->width, bitmapc->height, 0, 0, 1L);
        XFreeGC (DISPLAY, gc);

        /*
	 * If have sufficient memory, save the pixmap info in the pixmapCache.
	 */

	if (pixmapc = (PixmapCache *) malloc (sizeof (PixmapCache)))
	{
	    pixmapc->pixmapType = LABEL_PIXMAP;
	    pixmapc->foreground = fg;
	    pixmapc->background = bg;
	    pixmapc->pixmap = pixmap;
	    pixmapc->next = bitmapc->pixmapCache;
	    bitmapc->pixmapCache = pixmapc;
	}
    }

    return (pixmap);

} /* END OF FUNCTION MakeCachedLabelPixmap */



/*************************************<->*************************************
 *
 *  int
 *  GetBitmapIndex (pSD, name)
 *
 *
 *  Description:
 *  -----------
 *  Retrieve bitmap from cache.
 *
 *
 *  Inputs:
 *  ------
 *  pSD = pointer to screen data
 *  name = bitmap file name or NULL pointer
 *  bitmapCache[]
 *  bitmapCacheSize
 *  bitmapCacheCount
 *
 * 
 *  Outputs:
 *  -------
 *  bitmapCache[]
 *  bitmapCacheSize
 *  bitmapCacheCount
 *  Return   = bitmap cache index or -1
 *
 *
 *  Comments:
 *  --------
 *  None
 * 
 *************************************<->***********************************/

#define BITMAP_CACHE_INC 5

#ifdef _NO_PROTO
int GetBitmapIndex (pSD, name)

    WmScreenData *pSD;
    char         *name;
#else /* _NO_PROTO */
int GetBitmapIndex (WmScreenData *pSD, char *name)
#endif /* _NO_PROTO */
{
    char         *path;
    BitmapCache  *bitmapc;
    unsigned int  n;
    int           x, y;

    /*
     * Search a nonempty bitmap cache for a pathname match.
     */
    path = BitmapPathName (name);
    for (n = 0, bitmapc = pSD->bitmapCache;
	 n < pSD->bitmapCacheCount;
	 n++, bitmapc++)
    {
        if ((!path && !bitmapc->path) ||
            (path && bitmapc->path && 
	     !strcmp (path, bitmapc->path)))
        {
	    return (n);
	}
    }

    /*
     * The bitmap path name was not found in bitmapCache.
     * Find the next BitmapCache entry, creating or enlarging bitmapCache if 
     * necessary.
     */
    if (pSD->bitmapCacheSize == 0)
    /* create */
    {
        pSD->bitmapCacheSize = BITMAP_CACHE_INC;
        pSD->bitmapCache =
	    (BitmapCache *) malloc (BITMAP_CACHE_INC * sizeof (BitmapCache));
    }
    else if (pSD->bitmapCacheCount == pSD->bitmapCacheSize)
    /* enlarge */
    {
        pSD->bitmapCacheSize += BITMAP_CACHE_INC;
        pSD->bitmapCache = (BitmapCache *) 
	    realloc (pSD->bitmapCache, 
		     pSD->bitmapCacheSize * sizeof (BitmapCache));
    }

    if (pSD->bitmapCache == NULL)
    {
        MWarning ("Insufficient memory for bitmap %s\n", name);
	pSD->bitmapCacheSize = 0;
	pSD->bitmapCacheCount = 0;
	return (-1);
    }

    bitmapc = &(pSD->bitmapCache[pSD->bitmapCacheCount]);

    /*
     * Fill the entry with the bitmap info.
     * A NULL path indicates the builtin icon bitmap.
     * Indicate that no pixmapCache exists yet.
     */

    if (path)
    {
        if ((bitmapc->path = (String)
                 malloc ((unsigned int)(strlen (path) + 1))) == NULL)
        {
            MWarning ("Insufficient memory for bitmap %s\n", name);
	    return (-1);
        }
        strcpy (bitmapc->path, path);

        if (XReadBitmapFile (DISPLAY, pSD->rootWindow, path, 
			     &bitmapc->width, &bitmapc->height, 
			     &bitmapc->bitmap, &x, &y)
            != BitmapSuccess)
        {
            MWarning ("Unable to read bitmap file %s\n", path);
	    free (bitmapc->path);
	    return (-1);
        }

        if (bitmapc->width == 0 || bitmapc->height == 0)
        {
            MWarning ("Invalid bitmap file %s\n", path);
	    free (bitmapc->path);
	    return (-1);
        }
    }
    else
    /* builtin icon bitmap */
    {
        bitmapc->path   = NULL;
        bitmapc->bitmap = pSD->builtinIconPixmap;
        bitmapc->width  = iImage_width;
        bitmapc->height = iImage_height;
    }

    bitmapc->pixmapCache = NULL;

    return (pSD->bitmapCacheCount++);

} /* END OF FUNCTION GetBitmapIndex */



/*************************************<->*************************************
 *
 *  BitmapPathName (string)
 *
 *
 *  Description:
 *  -----------
 *  Constructs a bitmap file pathname from the bitmap file name and the
 *  bitmapDirectory resource value.
 *
 *
 *  Inputs:
 *  ------
 *  string = bitmap file name or NULL
 *  wmGD.bitmapDirectory = bitmapDirectory resource value
 *  HOME = environment variable for home directory
 *
 * 
 *  Outputs:
 *  -------
 *  Return = string containing the bitmap file pathname or NULL.
 *
 *
 *  Comments:
 *  --------
 *  Does not check for existence of the bitmap file.
 *  Returns NULL path name for a NULL file name.
 * 
 *************************************<->***********************************/

char *BitmapPathName (string)
    char *string;

{
    static char  fileName[MAXPATH+1];
    
    if (!string || !*string)
    {
	return (NULL);
    }

    /*
     * Interpret "~/.." as relative to the user's home directory.
     * Interpret "/.." as an absolute pathname.
     * If the bitmapDirectory resource is nonNULL, interpret path as relative
     *   to it.
     * Else interpret as relative to the current directory.
     */

    if ((string[0] == '~') && (string[1] == '/'))
    /* 
     * Handle "~/.." 
     */
    {
	GetHomeDirName(fileName);
        strncat (fileName, &(string[1]), MAXPATH - strlen (fileName));
        string = fileName;
    }

    else if ((string[0] != '/') &&
             wmGD.bitmapDirectory && 
	     *wmGD.bitmapDirectory)
    /*
     * Relative to nonNULL bitmapDirectory (which may have relative to HOME)
     */
    {
	if ((wmGD.bitmapDirectory[0] == '~') &&
	    (wmGD.bitmapDirectory[1] == '/'))
	{
	    GetHomeDirName(fileName);
            strncat (fileName, &wmGD.bitmapDirectory[1],
		     MAXPATH - strlen (fileName));
	} else {
	    strcpy (fileName, wmGD.bitmapDirectory);
	}
        strncat (fileName, "/", MAXPATH - strlen (fileName));
        strncat (fileName, string, MAXPATH - strlen (fileName));
        string = fileName;
    }

    return(string);

} /* END OF FUNCTION BitmapPathName */

