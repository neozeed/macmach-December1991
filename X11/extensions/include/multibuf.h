/*
 * $XConsortium: multibuf.h,v 1.12 89/11/01 09:09:56 jim Exp $
 *
 * Copyright 1989 Massachusetts Institute of Technology
 *
 * Permission to use, copy, modify, distribute, and sell this software and its
 * documentation for any purpose is hereby granted without fee, provided that
 * the above copyright notice appear in all copies and that both that
 * copyright notice and this permission notice appear in supporting
 * documentation, and that the name of M.I.T. not be used in advertising or
 * publicity pertaining to distribution of the software without specific,
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
 */

#ifndef _MULTIBUF_H_
#define _MULTIBUF_H_

#define MULTIBUFFER_PROTOCOL_NAME "Multi-Buffering"

#define MULTIBUFFER_MAJOR_VERSION	1	/* current version numbers */
#define MULTIBUFFER_MINOR_VERSION	0

#define X_MbufGetBufferVersion		0
#define X_MbufCreateImageBuffers	1
#define X_MbufDestroyImageBuffers	2
#define X_MbufDisplayImageBuffers	3
#define X_MbufSetMultiBufferAttributes	4
#define X_MbufGetMultiBufferAttributes	5
#define X_MbufSetBufferAttributes	6
#define X_MbufGetBufferAttributes	7
#define X_MbufGetBufferInfo		8
#define X_MbufCreateStereoWindow	9

/*
 * update_action field
 */
#define MultibufferUpdateActionUndefined	0
#define MultibufferUpdateActionBackground	1
#define MultibufferUpdateActionUntouched	2
#define MultibufferUpdateActionCopied		3

/*
 * update_hint field
 */
#define MultibufferUpdateHintFrequent		0
#define MultibufferUpdateHintIntermittent	1
#define MultibufferUpdateHintStatic		2

/*
 * valuemask fields
 */
#define MultibufferWindowUpdateHint	(1L << 0)
#define MultibufferBufferEventMask	(1L << 0)

/*
 * mono vs. stereo and left vs. right
 */
#define MultibufferModeMono		0
#define MultibufferModeStereo		1
#define MultibufferSideMono		0
#define MultibufferSideLeft	  	1
#define MultibufferSideRight		2

/*
 * clobber state
 */
#define MultibufferUnclobbered		0
#define MultibufferPartiallyClobbered	1
#define MultibufferFullyClobbered	2

/*
 * event stuff
 */
#define MultibufferClobberNotifyMask	0x02000000
#define MultibufferUpdateNotifyMask	0x04000000

#define MultibufferClobberNotify	0
#define MultibufferUpdateNotify		1
#define MultibufferNumberEvents		(MultibufferUpdateNotify + 1)

#define MultibufferBadBuffer		0
#define MultibufferNumberErrors		(MultibufferBadBuffer + 1)


#ifndef _MULTIBUF_SERVER_
/*
 * Extra definitions that will only be needed in the client
 */
typedef XID Multibuffer;

typedef struct {
    int	type;		    /* of event */
    unsigned long serial;   /* # of last request processed by server */
    int send_event;	    /* true if this came frome a SendEvent request */
    Display *display;	    /* Display the event was read from */
    Multibuffer buffer;	    /* buffer of event */
    int	state;		    /* see Clobbered constants above */
} XmbufClobberNotifyEvent;

typedef struct {
    int	type;		    /* of event */
    unsigned long serial;   /* # of last request processed by server */
    int send_event;	    /* true if this came frome a SendEvent request */
    Display *display;	    /* Display the event was read from */
    Multibuffer buffer;	    /* buffer of event */
} XmbufUpdateNotifyEvent;


/*
 * per-window attributes that can be got
 */
typedef struct {
    int displayed_index;	/* which buffer is being displayed */
    int update_action;		/* Undefined, Background, Untouched, Copied */
    int update_hint;		/* Frequent, Intermittent, Static */
    int window_mode;		/* Mono, Stereo */
} XmbufWindowAttributes;

/*
 * per-window attributes that can be set
 */
typedef struct {
    int update_hint;		/* Frequent, Intermittent, Static */
} XmbufSetWindowAttributes;


/*
 * per-buffer attributes that can be got
 */
typedef struct {
    Window window;		/* which window this belongs to */
    unsigned long event_mask;	/* events that have been selected */
    int buffer_index;		/* which buffer is this */
    int side;			/* Mono, Left, Right */
} XmbufBufferAttributes;

/*
 * per-buffer attributes that can be set
 */
typedef struct {
    unsigned long event_mask;	/* events that have been selected */
} XmbufSetBufferAttributes;


/*
 * per-screen buffer info (there will be lists of them)
 */
typedef struct {
    VisualID visualid;		/* visual usuable at this depth */
    int max_buffers;		/* most buffers for this visual */
    int depth;			/* depth of buffers to be created */
} XmbufBufferInfo;


/*
 * The application programming library contains the interfaces described below.
 * With the exception of XmbufQueryExtension, if any of these routines are
 * called with a display that does not support the extension, the
 * ExtensionErrorHandler (which can be set with XSetExtensionErrorHandler and
 * functions the same way as XSetErrorHandler) will be called.
 *

Bool XmbufQueryExtension (dpy, event_base_return, error_base_return)
    Display *dpy;
    int *event_base_return, *error_base_return;

	Returns True if the multibuffering/stereo extension is available on the
	given display.  If the extension exists, the value of the first event
	code (which should be added to the event type constants
	MultibufferClobberNotify and MultibufferUpdateNotify to get the actual
	values) is stored into event_base_return and the value of the first
	error code (which should be added to the error type constant
	MultibufferBadBuffer to get the actual value) is stored into
	error_base_return.


Status XmbufGetVersion (dpy, major_version_return, minor_version_return)
    Display *dpy;
    int *major_version_return, *minor_version_return;

	Gets the major and minor version numbers of the extension.  The return
	value is zero if an error occurs or non-zero if no error happens.


int XmbufCreateBuffers (dpy, window, count, update_action, update_hint,
			buffers_update)
    Display *dpy;
    Window window;
    int count;
    int update_action, update_hint;
    Multibuffer *buffers_update;

	Requests that "count" buffers be created with the given update_action
	and update_hint and be associated with the indicated window.  The
	number of buffers created is returned (zero if an error occurred)
	and buffers_update is filled in with that many Multibuffer identifiers.


void XmbufDestroyBuffers (dpy, window)
    Display *dpy;
    Window window;

	Destroys the buffers associated with the given window.


void XmbufDisplayBuffers (dpy, count, buffers, min_delay, max_delay)
    Display *dpy;
    int count;
    Multibuffer *buffers;
    int min_delay, max_delay;

	Displays the indicated buffers their appropriate windows within
	max_delay milliseconds after min_delay milliseconds have passed.
	No two buffers may be associated with the same window or else a Matc
	error is generated.


Status XmbufGetWindowAttributes (dpy, window, attributes)
    Display *dpy;
    Window window;
    XmbufWindowAttributes *attributes;

	Gets the multibuffering attributes that apply to all buffers associated
	with the given window.  Returns non-zero on success and zero if an
	error occurs.


void XmbufChangeWindowAttributes (dpy, window, valuemask, attributes)
    Display *dpy;
    Window window;
    unsigned long valuemask;
    XmbufSetWindowAttributes *attributes;

	Sets the multibuffering attributes that apply to all buffers associated
	with the given window.  This is currently limited to the update_hint.


Status XmbufGetBufferAttributes (dpy, buffer, attributes)
    Display *dpy;
    Buffer buffer;
    XmbufBufferAttributes *attributes;

	Gets the attributes for the indicated buffer.  Returns non-zero on
	success and zero if an error occurs.


void XmbufChangeBufferAttributes (dpy, buffer, valuemask, attributes)
    Display *dpy;
    Multibuffer buffer;
    unsigned long valuemask;
    XmbufSetBufferAttributes *attributes;

	Sets the attributes for the indicated buffer.  This is currently
	limited to the event_mask.


Status XmbufGetScreenInfo (dpy, drawable, nmono_return, mono_info_return,
			   nstereo, stereo_info_return)
    Display *dpy;
    Drawable drawable;
    int *nmono_return;
    XmbufBufferInfo **mono_info_return;
    int *nstereo_return;
    XmbufBufferInfo **stereo_info_return;

	Gets the parameters controlling how mono and stereo windows may be
	created on the screen of the given drawable.  The numbers of sets of
	visual and depths are returned in nmono_return and nstereo_return.  If
	nmono_return is greater than zero, then mono_info_return is set to the
	address of an array of XmbufBufferInfo structures describing the
	various visuals and depths that may be used.  Otherwise,
	mono_info_return is set to NULL.  Similarly, stereo_info_return is set
	according to nstereo_return.  The storage returned in mono_info_return
	and stereo_info_return may be released by XFree.  If no errors are
	encounted, non-zero will be returned.


Window XmbufCreateStereoWindow (dpy, parent, x, y, width, height, border_width,
				depth, class, visual, valuemask, attributes,
				left_return, right_return)
    Display *dpy;
    Window parent;
    int x, y;
    unsigned int width, height, border_width;
    int depth;
    unsigned int class;
    Visual *visual;
    unsigned long valuemask;
    XSetWindowAttributes *attributes;
    Multibuffer *left_return, *right_return;

	Creates a stereo window in the same way that XCreateWindow creates
	a mono window.  The buffer ids for the left and right buffers are 
	returned in left_return and right_return, respectively.  If an
	extension error handler that returns is installed, None will be
	returned if the extension is not available on this display.

 * 
 */

extern Bool XmbufQueryExtension ();	/* is extension on server */
extern Status XmbufGetVersion ();		/* what is extension rev */
extern int XmbufCreateBuffers ();		/* create buffers for window */
extern void XmbufDestroyBuffers ();	/* destroy buffers for window */
extern void XmbufDisplayBuffers ();	/* make buffers current */
extern Status XmbufGetWindowAttributes ();   /* get extension window attrs */
extern void XmbufChangeWindowAttributes ();  /* set extension window attrs */
extern Status XmbufGetBufferAttributes ();   /* get buffer attrs */
extern void XmbufChangeBufferAttributes ();  /* set buffer attrs */
extern Status XmbufGetScreenInfo ();	/* get mono and stereo parameters */
extern Window XmbufCreateStereoWindow ();/* stereo version of XCreateWindow */

#endif /* _MULTIBUF_SERVER_ */
#endif /* _MULTIBUF_H_ */
