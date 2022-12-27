/*
 * $XConsortium: LocBitmap.c,v 1.4 89/12/10 10:35:21 rws Exp $
 *
 * Copyright 1989 Massachusetts Institute of Technology
 *
 * Permission to use, copy, modify, and distribute this software and its
 * documentation for any purpose and without fee is hereby granted, provided
 * that the above copyright notice appear in all copies and that both that
 * copyright notice and this permission notice appear in supporting
 * documentation, and that the name of M.I.T. not be used in advertising
 * or publicity pertaining to distribution of the software without specific,
 * written prior permission.  M.I.T. makes no representations about the
 * suitability of this software for any purpose.  It is provided "as is"
 * without express or implied warranty.
 *
 * M.I.T. DISCLAIMS ALL WARRANTIES WITH REGARD TO THIS SOFTWARE, INCLUDING ALL
 * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS, IN NO EVENT SHALL M.I.T.
 * BE LIABLE FOR ANY SPECIAL, INDIRECT OR CONSEQUENTIAL DAMAGES OR ANY DAMAGES
 * WHATSOEVER RESULTING FROM LOSS OF USE, DATA OR PROFITS, WHETHER IN AN ACTION
 * OF CONTRACT, NEGLIGENCE OR OTHER TORTIOUS ACTION, ARISING OUT OF OR IN 
 * CONNECTION WITH THE USE OR PERFORMANCE OF THIS SOFTWARE.
 *
 * Author:  Jim Fulton, MIT X Consortium
 */

#include <X11/Xlib.h>
#include <X11/Xresource.h>
#include <X11/Xutil.h>
#include <X11/StringDefs.h>
#include <sys/param.h>			/* get MAXPATHLEN if possible */
#ifndef MAXPATHLEN
#define MAXPATHLEN 256
#endif
#include <X11/Xmu/CvtCache.h>
#include <X11/Xmu/Drawing.h>


/*
 * XmuLocateBitmapFile - read a bitmap file using the normal defaults
 */

Pixmap XmuLocateBitmapFile (screen, name, srcname, srcnamelen,
			    widthp, heightp, xhotp, yhotp)
    Screen *screen;
    char *name;
    char *srcname;			/* RETURN */
    int srcnamelen;
    int *widthp, *heightp, *xhotp, *yhotp;  /* RETURN */
{
    Display *dpy = DisplayOfScreen (screen);
    Window root = RootWindowOfScreen (screen);
    Bool try_plain_name = True;
    XmuCvtCache *cache = _XmuCCLookupDisplay (dpy);
    char **file_paths;
    char filename[MAXPATHLEN];
    unsigned int width, height;
    int xhot, yhot;
    int i;
    static char **split_path_string();


    /*
     * look in cache for bitmap path
     */
    if (cache) {
	if (!cache->string_to_bitmap.bitmapFilePath) {
	    XrmName xrm_name[2];
	    XrmClass xrm_class[2];
	    XrmRepresentation rep_type;
	    XrmValue value;

	    xrm_name[0] = XrmStringToName ("bitmapFilePath");
	    xrm_name[1] = NULL;
	    xrm_class[0] = XrmStringToClass ("BitmapFilePath");
	    xrm_class[1] = NULL;
	    /*
	     * XXX - warning, derefing Display * until XDisplayDatabase
	     */
	    if (!dpy->db) {
		/* what a hack; need to initialize dpy->db */
		(void) XGetDefault (dpy, "", "");
	    }
	    if (XrmQGetResource (dpy->db, xrm_name, xrm_class, 
				 &rep_type, &value) &&
		rep_type == XrmStringToQuark(XtRString)) {
		cache->string_to_bitmap.bitmapFilePath = 
		  split_path_string (value.addr);
	    }
	}
	file_paths = cache->string_to_bitmap.bitmapFilePath;
    }


    /*
     * Search order:
     *    1.  name if it begins with / or ./
     *    2.  "each prefix in file_paths"/name
     *    3.  BITMAPDIR/name
     *    4.  name if didn't begin with / or .
     */

#ifndef BITMAPDIR
#define BITMAPDIR "/usr/include/X11/bitmaps"
#endif

    for (i = 1; i <= 4; i++) {
	char *fn = filename;
	Pixmap pixmap;

	switch (i) {
	  case 1:
	    if (!(name[0] == '/' || (name[0] == '.') && name[1] == '/')) 
	      continue;
	    fn = name;
	    try_plain_name = False;
	    break;
	  case 2:
	    if (file_paths && *file_paths) {
		sprintf (filename, "%s/%s", *file_paths, name);
		file_paths++;
		i--;
		break;
	    }
	    continue;
	  case 3:
	    sprintf (filename, "%s/%s", BITMAPDIR, name);
	    break;
	  case 4:
	    if (!try_plain_name) continue;
	    fn = name;
	    break;
	}

	if (XReadBitmapFile (dpy, root, fn, &width, &height, 
			     &pixmap, &xhot, &yhot) == BitmapSuccess) {
	    if (widthp) *widthp = (int)width;
	    if (heightp) *heightp = (int)height;
	    if (xhotp) *xhotp = xhot;
	    if (yhotp) *yhotp = yhot;
	    if (srcname && srcnamelen > 0) {
		strncpy (srcname, fn, srcnamelen - 1);
		srcname[srcnamelen - 1] = '\0';
	    }
	    return pixmap;
	}
    }

    return None;
}


/*
 * split_path_string - split a colon-separated list into its constituent
 * parts; to release, free list[0] and list.
 */
static char **split_path_string (src)
    register char *src;
{
    int nelems = 1;
    register char *dst;
    char **elemlist, **elem;

    /* count the number of elements */
    for (dst = src; *dst; dst++) if (*dst == ':') nelems++;

    /* get memory for everything */
    dst = (char *) malloc (dst - src + 1);
    if (!dst) return NULL;
    elemlist = (char **) calloc ((nelems + 1), sizeof (char *));
    if (!elemlist) {
	free (dst);
	return NULL;
    }

    /* copy to new list and walk up nulling colons and setting list pointers */
    strcpy (dst, src);
    for (elem = elemlist, src = dst; *src; src++) {
	if (*src == ':') {
	    *elem++ = dst;
	    *src = '\0';
	    dst = src + 1;
	}
    }
    *elem = dst;

    return elemlist;
}


void _XmuStringToBitmapInitCache (c)
    register XmuCvtCache *c;
{
    c->string_to_bitmap.bitmapFilePath = NULL;
}

void _XmuStringToBitmapFreeCache (c)
    register XmuCvtCache *c;
{
    if (c->string_to_bitmap.bitmapFilePath) {
	if (c->string_to_bitmap.bitmapFilePath[0]) 
	  free (c->string_to_bitmap.bitmapFilePath[0]);
	free ((char *) (c->string_to_bitmap.bitmapFilePath));
    }
}
