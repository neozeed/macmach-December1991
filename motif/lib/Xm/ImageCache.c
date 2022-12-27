#ifdef REV_INFO
#ifndef lint
static char SCCSID[] = "OSF/Motif: @(#)ImageCache.c	3.18 91/01/10";
#endif /* lint */
#endif /* REV_INFO */
/******************************************************************************
*******************************************************************************
*
*  (c) Copyright 1989, 1990, 1991 OPEN SOFTWARE FOUNDATION, INC.
*  (c) Copyright 1987, 1988, 1989, 1990, HEWLETT-PACKARD COMPANY
*  ALL RIGHTS RESERVED
*  
*  	THIS SOFTWARE IS FURNISHED UNDER A LICENSE AND MAY BE USED
*  AND COPIED ONLY IN ACCORDANCE WITH THE TERMS OF SUCH LICENSE AND
*  WITH THE INCLUSION OF THE ABOVE COPYRIGHT NOTICE.  THIS SOFTWARE OR
*  ANY OTHER COPIES THEREOF MAY NOT BE PROVIDED OR OTHERWISE MADE
*  AVAILABLE TO ANY OTHER PERSON.  NO TITLE TO AND OWNERSHIP OF THE
*  SOFTWARE IS HEREBY TRANSFERRED.
*  
*  	THE INFORMATION IN THIS SOFTWARE IS SUBJECT TO CHANGE WITHOUT
*  NOTICE AND SHOULD NOT BE CONSTRUED AS A COMMITMENT BY OPEN SOFTWARE
*  FOUNDATION, INC. OR ITS THIRD PARTY SUPPLIERS  
*  
*  	OPEN SOFTWARE FOUNDATION, INC. AND ITS THIRD PARTY SUPPLIERS,
*  ASSUME NO RESPONSIBILITY FOR THE USE OR INABILITY TO USE ANY OF ITS
*  SOFTWARE .   OSF SOFTWARE IS PROVIDED "AS IS" WITHOUT WARRANTY OF ANY
*  KIND, AND OSF EXPRESSLY DISCLAIMS ALL IMPLIED WARRANTIES, INCLUDING
*  BUT NOT LIMITED TO THE IMPLIED WARRANTIES OF MERCHANTABILITY AND
*  FITNESS FOR A PARTICULAR PURPOSE.
*  
*  Notice:  Notwithstanding any other lease or license that may pertain to,
*  or accompany the delivery of, this computer software, the rights of the
*  Government regarding its use, reproduction and disclosure are as set
*  forth in Section 52.227-19 of the FARS Computer Software-Restricted
*  Rights clause.
*  
*  (c) Copyright 1989, 1990, 1991 Open Software Foundation, Inc.  Unpublished - all
*  rights reserved under the Copyright laws of the United States.
*  
*  RESTRICTED RIGHTS NOTICE:  Use, duplication, or disclosure by the
*  Government is subject to the restrictions as set forth in subparagraph
*  (c)(1)(ii) of the Rights in Technical Data and Computer Software clause
*  at DFARS 52.227-7013.
*  
*  Open Software Foundation, Inc.
*  11 Cambridge Center
*  Cambridge, MA   02142
*  (617)621-8700
*  
*  RESTRICTED RIGHTS LEGEND:  This computer software is submitted with
*  "restricted rights."  Use, duplication or disclosure is subject to the
*  restrictions as set forth in NASA FAR SUP 18-52.227-79 (April 1985)
*  "Commercial Computer Software- Restricted Rights (April 1985)."  Open
*  Software Foundation, Inc., 11 Cambridge Center, Cambridge, MA  02142.  If
*  the contract contains the Clause at 18-52.227-74 "Rights in Data General"
*  then the "Alternate III" clause applies.
*  
*  (c) Copyright 1989, 1990, 1991 Open Software Foundation, Inc.
*  ALL RIGHTS RESERVED 
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
*******************************************************************************
******************************************************************************/

#include <stdio.h>
#include <ctype.h>
#include <errno.h>            /* defines ENAMETOOLONG */
#include <memory.h>
#include <pwd.h>
#include <sys/param.h>        /* defines MAXPATHLEN */
#include <Xm/XmP.h>
#include <Xm/bitmaps.h>

#ifndef MAXPATHLEN
#define MAXPATHLEN 1024
#endif
#ifndef ENAMETOOLONG
#define ENAMETOOLONG 248
#endif

#define PATHSBLOCKSIZE	4


/* external declarations */

extern int errno;
extern char *getenv();
extern int getuid();

extern XImage * _XmGetImageFromFile ();

/* forward declarations */

#ifdef _NO_PROTO
static String InitBmPath();
#else /* _NO_PROTO */
static String InitBmPath(String image_name);
#endif /* _NO_PROTO */
static String GetRootDirName();

/*  Image array handling defines, structure, and global statics  */

#define MAX_BUILTIN_IMAGES	11
#define INCREMENT_SIZE		10

typedef struct _ImageSet
{
   XImage  * image;
   char    * image_name;
} ImageSet;

static ImageSet * image_set = NULL;
static int image_set_size = 0;


/*  Pixmap caching structure and global statics  */

typedef struct _PixmapCache
{
   Screen * screen;
   char   * image_name;
   Pixel    foreground;
   Pixel    background;
   Pixmap   pixmap;
   int      reference_count;
   struct _PixmapCache * next;
} PixmapCache;

static PixmapCache * pixmap_cache = NULL;




/************************************************************************
 *
 *  InitializeImageSet
 *	Initialize the image set if needed.
 *
 ************************************************************************/

static void InitializeImageSet()

{
   register int i;

   image_set_size = MAX_BUILTIN_IMAGES + INCREMENT_SIZE;
   image_set = (ImageSet *) XtMalloc (sizeof (ImageSet) * image_set_size);

   for (i = 0; i < MAX_BUILTIN_IMAGES + INCREMENT_SIZE; i++)
   {
      image_set[i].image = NULL;

      if (i < MAX_BUILTIN_IMAGES)
         image_set[i].image_name = bitmap_name_set[i];
      else
         image_set[i].image_name = NULL;
   }
}





/************************************************************************
 *
 *  XmInstallImage
 *	Add the provided image for the image set and return an
 *	tile id to be used for further referencing.  Keep the
 *	allocation of the image_set array straight.
 *
 ************************************************************************/

#ifdef _NO_PROTO
Boolean XmInstallImage (image, image_name)
XImage * image;
char   * image_name;

#else /* _NO_PROTO */
Boolean XmInstallImage (XImage *image, char *image_name)
#endif /* _NO_PROTO */
{
   register int i;
   register int image_loc;


   /*  Error checking  */

   if (image == NULL || image_name == NULL) return (False);


   /*  Check for the initial allocation of the image set array  */

   if (image_set_size == 0)
      InitializeImageSet();


   /*  Verify that the image_name is not already in the image set.  */

   for (i = 0; i < image_set_size; i++)
   {
      if (image_set[i].image_name != NULL)
      {
         if (strcmp (image_name, image_set[i].image_name) == 0)
            return (False);
      }
   }


   /*  Check for an open location for the new image  */

   image_loc = -1;

   for (i = MAX_BUILTIN_IMAGES; i < image_set_size; i++)
   {
      if (image_set[i].image == NULL)
      {
         image_loc = i;
         break;
      }
   }


   /*  If the image loc still is -1 then more image set space  */
   /*  needs to be allocated.                                  */

   if (image_loc == -1)
   {
      image_loc = image_set_size;
      image_set_size += INCREMENT_SIZE;
      image_set = 
         (ImageSet *) XtRealloc(image_set, sizeof(ImageSet) * image_set_size);

      for (i = image_set_size - INCREMENT_SIZE; i < image_set_size; i++)
      {
         image_set[i].image = NULL;
         image_set[i].image_name = NULL;
      }
   }


   /*  Initialize the image element for the new image and return True.  */

   image_set[image_loc].image = image;
   image_set[image_loc].image_name = 
      (char *) strcpy (XtMalloc (XmStrlen (image_name) + 1), image_name);

   return (True);
}




/************************************************************************
 *
 *  XmUninstallImage
 *	Remove an image from the image set.
 *	Return a boolean (True) if the uninstall succeeded.  Return
 *	a boolean (False) if an error condition occurs.
 *
 ************************************************************************/

#ifdef _NO_PROTO
Boolean XmUninstallImage (image)
XImage * image;

#else /* _NO_PROTO */
Boolean XmUninstallImage (XImage *image)
#endif /* _NO_PROTO */
{
   register int i;


   /*  Check for invalid conditions  */

   if (image == NULL) return (False);


   /*  Loop through the image set until the image is found.   */
   /*  Free the image, pack down the array, shrink the array  */
   /*  if needed, and return true.                            */

   for (i = 0; i < image_set_size; i++)
   {
      if (image == image_set[i].image)
      {
         image_set[i].image = NULL;
         XtFree (image_set[i].image_name);
         image_set[i].image_name = NULL;

         /*  Pack the images down one slot  */

         for (i = i + 1; i < image_set_size; i++)
         {
            image_set[i - 1].image = image_set[i].image;
            image_set[i - 1].image_name = image_set[i].image_name;
         }


         /*  Shrink the image set array if necessary  */

         for (i = image_set_size - 1; 
              i > image_set_size - INCREMENT_SIZE - 1 && 
              image_set[i].image == NULL; i--)

         if (i == image_set_size - INCREMENT_SIZE - 1)
         {
            image_set_size -= INCREMENT_SIZE;
            image_set = 
               (ImageSet *) XtRealloc (image_set, 
                                       sizeof(ImageSet) * image_set_size);
         }         

         return (True);
      }
   }

   return (False);
}




/************************************************************************
 *
 *  XmGetPixmap
 *	Create a pixmap of screen depth, using the image referenced
 *	by the name and the foreground and background colors 
 *      specified.  Ensure that multiple pixmaps of the same attributes
 *	are not created by maintaining a cache of the pixmaps.
 *
 ************************************************************************/

#ifdef _NO_PROTO
Pixmap XmGetPixmap (screen, image_name, foreground, background)
Screen * screen;
char   * image_name;
Pixel    foreground;
Pixel    background;

#else /* _NO_PROTO */
Pixmap XmGetPixmap (Screen *screen, char *image_name, Pixel foreground, Pixel background)
#endif /* _NO_PROTO */
{
    register PixmapCache * cache_ptr;
    register Display * display = DisplayOfScreen(screen);
    register int image_loc;
    register int i;
    XImage * image;
    int path_count;
    Pixmap   pixmap;
    static Boolean initialized = False;
    static XImage  built_in_image;
    static GC        gc = NULL;
    static Screen  * gcScreen = NULL;
    static XGCValues gcValues;
    
    
    /*  Error checking  */
    
    if (image_name == NULL) return (XmUNSPECIFIED_PIXMAP);
    
    
    /*  Check for a matching pixmap  */
    
    for (cache_ptr = pixmap_cache; cache_ptr; cache_ptr = cache_ptr->next)
      {
	  if (strcmp (cache_ptr->image_name, image_name) == 0  &&
	      cache_ptr->foreground == foreground              &&
	      cache_ptr->background == background              &&
	      cache_ptr->screen == screen)
	    {
		cache_ptr->reference_count++;
		return (cache_ptr->pixmap);
	    }
      }
    
    
    /*  Check for the initial allocation of the image set array  */
    
    if (image_set_size == 0)
      InitializeImageSet();
    
    
    image_loc = -1;
    for (i = 0; i < image_set_size; i++)
      {
	  if (image_set[i].image_name != NULL &&
	      strcmp (image_set[i].image_name, image_name) == 0)
	    {
		image_loc = i;
		break;
	    }
      }
    
    
    /*  If no image was found, set up and go try to get an image from a file  */
    
    if (image_loc == -1)
      {
	  char *bmPath;
	  char *file_name;
	  SubstitutionRec	subs[1] ;

          subs[0].match = 'B';
	  subs[0].substitution = image_name;

	  bmPath = InitBmPath(image_name);

	  if (file_name = XtResolvePathname(display, 
					    "bitmaps",
					    NULL,
					    NULL,
					    bmPath,
					    subs,
					    XtNumber(subs),
					    NULL))
	    {
		if ((image = (XImage *)_XmGetImageFromFile (file_name)) != NULL)
		  {
		      XmInstallImage (image, image_name);
		      
		      for (image_loc = MAX_BUILTIN_IMAGES - 1; 
			   image != image_set[image_loc].image; image_loc++);
		  }
		XtFree(file_name);
	    }

	  XtFree (bmPath);
      }

    if (image_loc == -1)
      return (XmUNSPECIFIED_PIXMAP);
    
    
    /*  Is the image format correct for this screen? */
    
    if (image_loc >= MAX_BUILTIN_IMAGES)
      {
	  if ((image_set[image_loc].image->depth == 1 &&
	       image_set[image_loc].image->format == XYBitmap) ||
	      image_set[image_loc].image->depth == DefaultDepthOfScreen(screen))
	    ;
	  else
	    return (XmUNSPECIFIED_PIXMAP);
      }
    
    
    /*  If the image is a built in image then get it.  Otherwise  */
    /*  just use the image out of the image set array.            */
    
    if (image_loc < MAX_BUILTIN_IMAGES)
      {
	  if (!initialized)
	    {
		initialized = True;
		built_in_image.width = 16;
		built_in_image.height = 16;
		built_in_image.xoffset = 0;
		built_in_image.format = XYBitmap;
		built_in_image.byte_order = MSBFirst;
		built_in_image.bitmap_pad = 8;
		built_in_image.bitmap_bit_order = LSBFirst;
		built_in_image.bitmap_unit = 8;
		built_in_image.depth = 1;
		built_in_image.bytes_per_line = 2;
		built_in_image.obdata = NULL;
	    }
	  
	  built_in_image.data = (char *) bitmaps [image_loc];
	  image = &built_in_image;
      }
    else
      image = image_set[image_loc].image;
    
    
    /*  Create a pixmap to hold the image, allocate a new pixmap        */
    /*  cache entry, put the cache entry a the head of the queue,       */
    /*  initialize the cache fields and increment the image reference.  */
    
    pixmap = XCreatePixmap (display, RootWindowOfScreen(screen), 
			    image->width, image->height,
			    DefaultDepthOfScreen (screen));
    
    cache_ptr = XtNew (PixmapCache);
    cache_ptr->next = pixmap_cache;
    pixmap_cache = cache_ptr;
    
    cache_ptr->screen = screen;
    cache_ptr->foreground = foreground;
    cache_ptr->background = background;
    cache_ptr->image_name = 
      (char *) strcpy (XtMalloc (XmStrlen (image_name) + 1), image_name);
    cache_ptr->reference_count = 1;
    cache_ptr->pixmap = pixmap;
    
    
    /*  Set up a gc for the image to pixmap copy, store the image  */ 
    /*  into the pixmap and return the pixmap.                     */
    
    if (screen != gcScreen)
      {
	  if (gc) XFreeGC (gcScreen->display, gc);
	  
	  gcValues.foreground = foreground;
	  gcValues.background = background;
	  gcScreen = screen;
	  gc = XCreateGC (display, RootWindowOfScreen(screen),
			  GCForeground | GCBackground, &gcValues);
      }
    else if ((gcValues.foreground != foreground) ||
	     (gcValues.background != background))
      {
	  gcValues.foreground = foreground;
	  gcValues.background = background;
	  XChangeGC (display, gc, GCForeground | GCBackground, &gcValues);
      }
    
    XPutImage (display, pixmap, gc, image, 
	       0, 0, 0, 0, image->width, image->height);
    
    return (pixmap);
}




/************************************************************************
 *
 *  _XmInstallPixmap
 *	Install a pixmap into the pixmap cache.  This is used to add
 *	cached pixmaps which have no image associated with them.
 *
 ************************************************************************/

#ifdef _NO_PROTO
Boolean _XmInstallPixmap (pixmap, screen, image_name, foreground, background)
Pixmap   pixmap;
Screen * screen;
char   * image_name;
Pixel    foreground;
Pixel    background;

#else /* _NO_PROTO */
Boolean _XmInstallPixmap (Pixmap pixmap, Screen *screen, char *image_name, Pixel foreground, Pixel background)
#endif /* _NO_PROTO */
{
   register PixmapCache * cache_ptr;
   register int i;


   /*  Error checking  */

   if (image_name == NULL) return (False);


   /*  See if this data has already been cached. */
 
   for (cache_ptr = pixmap_cache; cache_ptr; cache_ptr = cache_ptr->next)
   {
      if ((strcmp(cache_ptr->image_name, image_name) == 0) &&
		(cache_ptr->screen == screen) &&
		(cache_ptr->foreground == foreground) &&
		(cache_ptr->background == background))
         return (False);
   }


   /*  Allocate the cache structure and put it into the list  */

   cache_ptr = XtNew (PixmapCache);
   cache_ptr->next = pixmap_cache;
   pixmap_cache = cache_ptr;

   cache_ptr->screen = screen;
   cache_ptr->foreground = foreground;
   cache_ptr->background = background;
   cache_ptr->image_name = 
      (char *) strcpy(XtMalloc(XmStrlen(image_name) + 1), image_name);
   cache_ptr->reference_count = 1;
   cache_ptr->pixmap = pixmap;

   return (True);
}



/************************************************************************
 *
 *  XmDestroyPixmap
 *	Locate a pixmap in the cache and decrement its reference count.
 *	When the reference count is at zero, free the pixmap.
 *
 ************************************************************************/

#ifdef _NO_PROTO
Boolean XmDestroyPixmap (screen, pixmap)
Screen * screen;
Pixmap   pixmap;

#else /* _NO_PROTO */
Boolean XmDestroyPixmap (Screen *screen, Pixmap pixmap)
#endif /* _NO_PROTO */
{
   register PixmapCache * cache_ptr;
   register PixmapCache * prev_cache_ptr = NULL;


   /*  Set up a loop that goes through the cache list and  */
   /*  checks for a matching screen and pixmap.            */

   for (cache_ptr = pixmap_cache; cache_ptr; cache_ptr = cache_ptr->next)
   {
      if (cache_ptr->pixmap == pixmap && cache_ptr->screen == screen)
      {
         cache_ptr->reference_count--;

         if (cache_ptr->reference_count == 0)
         {
            if (cache_ptr == pixmap_cache)
               pixmap_cache = cache_ptr->next;
            else
               prev_cache_ptr->next = cache_ptr->next;

            XtFree (cache_ptr->image_name);
            XFreePixmap (DisplayOfScreen(screen), cache_ptr->pixmap);
            XtFree (cache_ptr);
         }

         return (True);
      }

      prev_cache_ptr = cache_ptr;
   }

   return (False);
}


/************************************<+>*************************************
 *
 *   GetRootDirName
 *
 *************************************<+>************************************/
static String GetRootDirName()
{
	int uid;
	struct passwd *pw;
	static char *ptr = NULL;
	char *outptr;

	if (ptr == NULL)
	{
		if((ptr = getenv("HOME")) == NULL) 
		{
			if((ptr = getenv("USER")) != NULL) 
				pw = (struct passwd *)getpwnam(ptr);
			else 
			{
				uid = getuid();
				pw = (struct passwd *)getpwuid(uid);
			}

			if (pw) 
				ptr = pw->pw_dir;
			else 
				ptr = NULL;
		}
	}

	outptr = XtMalloc (strlen(ptr) + 2);
	strcpy (outptr, ptr);
	strcat (outptr, "/");
	return outptr; 
}

/************************************<+>*************************************
 *
 *   InitBmPath
 *
 *************************************<+>************************************/


static char XAPPLRES_DEFAULT[] = "\
%%B\
%%S:\
%s/%%L/%%T/%%N/%%B\
%%S:\
%s/%%l/%%T/%%N/%%B\
%%S:\
%s/%%L/%%T/%%B\
%%S:\
%s/%%l/%%T/%%B\
%%S:\
%s/%%T/%%N/%%B\
%%S:\
%s/%%T/%%B\
%%S:\
%s%%T/%%B\
%%S:\
%s%%B\
%%S:\
/usr/local/lib/X11/%%L/%%T/%%N/%%B\
%%S:\
/usr/local/lib/X11/%%l/%%T/%%N/%%B\
%%S:\
/usr/local/lib/X11/%%T/%%N/%%B\
%%S:\
/usr/local/lib/X11/%%L/%%T/%%B\
%%S:\
/usr/local/lib/X11/%%l/%%T/%%B\
%%S:\
/usr/local/lib/X11/%%T/%%B\
%%S:\
/usr/local/include/X11/%%T/%%B\
%%S";

static char BMPATH_DEFAULT[] = "\
%%B\
%%S:\
%s%%L/%%T/%%N/%%B\
%%S:\
%s%%l/%%T/%%N/%%B\
%%S:\
%s%%T/%%N/%%B\
%%S:\
%s%%L/%%T/%%B\
%%S:\
%s%%l/%%T/%%B\
%%S:\
%s%%T/%%B\
%%S:\
%s%%B\
%%S:\
/usr/local/lib/X11/%%L/%%T/%%N/%%B\
%%S:\
/usr/local/lib/X11/%%l/%%T/%%N/%%B\
%%S:\
/usr/local/lib/X11/%%L/%%T/%%B\
%%S:\
/usr/local/lib/X11/%%l/%%T/%%B\
%%S:\
/usr/local/lib/X11/%%T/%%N/%%B\
%%S:\
/usr/local/lib/X11/%%T/%%B\
%%S:\
/usr/local/include/X11/%%T/%%B\
%%S";

static char ABSOLUTE_PATH[] = "\
%B\
%S";

static String InitBmPath(file_name)
     String	file_name ;
{
  String path;
  String old_path;
  String homedir;
  String bm_path;

  if (file_name[0] == '/')
    {
      bm_path = XtMalloc(strlen(ABSOLUTE_PATH) + 1);
      strcpy (bm_path, ABSOLUTE_PATH);
    }
  else
    {
      path = (char *)getenv ("XBMLANGPATH");
      if (path  == NULL) 
	{
	  homedir = GetRootDirName();
	  old_path = (char *)getenv ("XAPPLRESDIR");
	  if (old_path == NULL) 
	    {
	      bm_path = XtCalloc(1,
				 7*strlen(homedir) + strlen(BMPATH_DEFAULT));
	      sprintf( bm_path, BMPATH_DEFAULT, homedir, homedir, homedir,
		      homedir, homedir, homedir, homedir);
	    } 
	  else 
	    {
	      bm_path = XtCalloc(1, 6*strlen(old_path) 
				 + 2*strlen(homedir) + strlen(XAPPLRES_DEFAULT));
	      sprintf(bm_path, XAPPLRES_DEFAULT, 
		      old_path, old_path, old_path, old_path, old_path, 
		      old_path, homedir, homedir);
	    }
	  XtFree (homedir);
	}
      else
	{
	  bm_path = XtMalloc(strlen(path) + 1);
	  strcpy (bm_path, path);
	}
    }
  return (bm_path);
}

