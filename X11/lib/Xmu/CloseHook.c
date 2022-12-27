/*
 * $XConsortium: CloseHook.c,v 1.4 89/07/16 14:25:48 jim Exp $
 *
 * CloseDisplayHook package - provide callback on XCloseDisplay
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
 * 
 * 
 *			      Public Entry Points
 * 
 * CloseHook XmuAddCloseDisplayHook (dpy, func, arg)
 *     Display *dpy;
 *     int (*func)();
 *     caddr_t arg;
 * 
 * Bool XmuRemoveCloseDisplayHook (dpy, hook, func, arg)
 *     Display *dpy;
 *     CloseHook hook;
 *     int (*func)();
 *     caddr_t arg;
 * 
 * Bool XmuLookupCloseDisplayHook (dpy, hook, func, arg)
 *     Display *dpy;
 *     CloseHook hook;
 *     int (*func)();
 *     caddr_t arg;
 * 
 */

#include <stdio.h>					/* for NULL */
#include <X11/Xos.h>
#include <X11/Xlib.h>
#include <X11/Xmu/CloseHook.h>

extern char *malloc();					/* should be void * */

/*
 *				 Private data
 *
 * This is a list of display entries, each of which contains a list of callback
 * records.
 */

typedef struct _CallbackRec {
    struct _CallbackRec *next;		/* next link in chain */
    int (*func)();			/* function to call */
    caddr_t arg;			/* argument to pass with function */
} CallbackRec;


typedef struct _DisplayEntry {
    struct _DisplayEntry *next;		/* next link in chain */
    Display *dpy;			/* the display this represents */
    int extension;			/* from XAddExtension */
    struct _CallbackRec *start, *end;	/* linked list of callbacks */
} DisplayEntry;


static DisplayEntry *elist = NULL;
static Bool _MakeExtension();
static DisplayEntry *_FindDisplayEntry();


/*
 *****************************************************************************
 *			      Public Entry Points                            *
 *****************************************************************************
 */

/*
 * Add - add a callback for the given display.  When the display is closed,
 * the given function will be called as:
 *
 *         (*func) (dpy, arg)
 *
 * The function is declared to return an int even though the value is ignored
 * because some compilers have problems with functions returning void.
 * 
 * This routine returns NULL if it was unable to add the callback, otherwise
 * it returns an untyped pointer that can be used with Remove or Lookup, but
 * not dereferenced.
 */
CloseHook XmuAddCloseDisplayHook (dpy, func, arg)
    Display *dpy;
    int (*func)();			/* function to call on close display */
    caddr_t arg;			/* arg to pass */
{
    DisplayEntry *de;
    CallbackRec *cb;

    /* allocate ahead of time so that we can fail atomically */
    cb = (CallbackRec *) malloc (sizeof (CallbackRec));
    if (!cb) return ((caddr_t) NULL);

    de = _FindDisplayEntry (dpy, NULL);
    if (!de) {
	if ((de = (DisplayEntry *) malloc (sizeof (DisplayEntry))) == NULL ||
	    !_MakeExtension (dpy, &de->extension)) {
	    free ((char *) cb);
	    if (de) free ((char *) de);
	    return ((CloseHook) NULL);
	}
	de->dpy = dpy;
	de->start = de->end = NULL;
	de->next = elist;
	elist = de;
    }

    /* add to end of list of callback recordss */
    cb->func = func;
    cb->arg = arg;
    cb->next = NULL;
    if (de->end) {
	de->end->next = cb;
    } else {
	de->start = cb;
    }
    de->end = cb;

    return ((CloseHook) cb);
}


/*
 * Remove - get rid of a callback.  If handle is non-null, use that to compare
 * entries.  Otherwise, remove first instance of the function/argument pair.
 */
Bool XmuRemoveCloseDisplayHook (dpy, handle, func, arg)
    Display *dpy;
    CloseHook handle;			/* value from XmuAddCloseDisplayHook */
    int (*func)();			/* function to call on close display */
    caddr_t arg;			/* arg to pass */
{
    DisplayEntry *de = _FindDisplayEntry (dpy, NULL);
    register CallbackRec *h, *prev;

    if (!de) return False;

    /* look for handle or function/argument pair */
    for (h = de->start, prev = NULL; h; h = h->next) {
	if (handle) {
	    if (h == (CallbackRec *) handle) break;
	} else {
	    if (h->func == func && h->arg == arg) break;
	}
	prev = h;
    }
    if (!h) return False;


    /* remove from list, watch head and tail */
    if (de->start == h) {
	de->start = h->next;
    } else {
	prev->next = h->next;
    }
    if (de->end == h) de->end = prev;
    free ((char *) h);
    return True;
}


/*
 * Lookup - see whether or not a handle has been installed.  If handle is 
 * non-NULL, look for an entry that matches it; otherwise look for an entry 
 * with the same function/argument pair.
 */
Bool XmuLookupCloseDisplayHook (dpy, handle, func, arg)
    Display *dpy;
    CloseHook handle;			/* value from XmuAddCloseDisplayHook */
    int (*func)();			/* function to call on close display */
    caddr_t arg;			/* arg to pass */
{
    DisplayEntry *de = _FindDisplayEntry (dpy, NULL);
    register CallbackRec *h;

    if (!de) return False;

    for (h = de->start; h; h = h->next) {
	if (handle) {
	    if (h == (CallbackRec *) handle) break;
	} else {
	    if (h->func == func && h->arg == arg) break;
	}
    }
    return (h ? True : False);
}


/*
 *****************************************************************************
 *			       internal routines                             *
 *****************************************************************************
 */


/*
 * Find the specified display on the linked list of displays.  Also return
 * the preceeding link so that the display can be unlinked without having
 * back pointers.
 */
static DisplayEntry *_FindDisplayEntry (dpy, prevp)
    register Display *dpy;
    DisplayEntry **prevp;
{
    register DisplayEntry *d, *prev;

    for (d = elist, prev = NULL; d; d = d->next) {
	if (d->dpy == dpy) {
	    if (prevp) *prevp = prev;
	    return d;
	}
	prev = d;
    }
    return NULL;
}



/*
 * _DoCallbacks - process all of the callbacks for this display and free
 * the associated callback data (callback records and display entries).
 */
static int _DoCallbacks (dpy, codes)
    Display *dpy;
    XExtCodes *codes;
{
    register CallbackRec *h;
    DisplayEntry *prev;
    DisplayEntry *de = _FindDisplayEntry (dpy, &prev);

    if (!de) return 0;

    /* walk the list doing the callbacks and freeing callback record */
    for (h = de->start; h;) {
	register CallbackRec *oldh = h;
	(*(h->func)) (dpy, h->arg);
	free ((char *) h);
	h = oldh->next;
    }

    /* unlink this display from chain */
    if (elist == de) {
	elist = de->next;
    } else {
	prev->next = de->next;
    }
    free ((char *) de);
    return 1;
}


/*
 * _MakeExtension - create an extension for this display; done once per display
 */
static Bool _MakeExtension (dpy, extensionp)
    Display *dpy;
    int *extensionp;
{
    XExtCodes *codes;

    codes = XAddExtension (dpy);
    if (!codes) return False;

    (void) XESetCloseDisplay (dpy, codes->extension, _DoCallbacks);

    *extensionp = codes->extension;
    return True;
}
