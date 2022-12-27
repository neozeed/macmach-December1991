/* $XConsortium: InitialI.h,v 1.30 89/12/12 14:32:48 swick Exp $ */
/* $oHeader: InitializeI.h,v 1.8 88/09/01 11:25:04 asente Exp $ */
/***********************************************************
Copyright 1987, 1988 by Digital Equipment Corporation, Maynard, Massachusetts,
and the Massachusetts Institute of Technology, Cambridge, Massachusetts.

                        All Rights Reserved

Permission to use, copy, modify, and distribute this software and its 
documentation for any purpose and without fee is hereby granted, 
provided that the above copyright notice appear in all copies and that
both that copyright notice and this permission notice appear in 
supporting documentation, and that the names of Digital or MIT not be
used in advertising or publicity pertaining to distribution of the
software without specific, written prior permission.  

DIGITAL DISCLAIMS ALL WARRANTIES WITH REGARD TO THIS SOFTWARE, INCLUDING
ALL IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS, IN NO EVENT SHALL
DIGITAL BE LIABLE FOR ANY SPECIAL, INDIRECT OR CONSEQUENTIAL DAMAGES OR
ANY DAMAGES WHATSOEVER RESULTING FROM LOSS OF USE, DATA OR PROFITS,
WHETHER IN AN ACTION OF CONTRACT, NEGLIGENCE OR OTHER TORTIOUS ACTION,
ARISING OUT OF OR IN CONNECTION WITH THE USE OR PERFORMANCE OF THIS
SOFTWARE.

******************************************************************/

/****************************************************************
 *
 * Displays
 *
 ****************************************************************/

#include <sys/param.h>				/* to get MAXPATHLEN */
#ifndef MAXPATHLEN
#define MAXPATHLEN 256
#endif

#ifndef MAX
#define MAX(a,b) (((a) > (b)) ? (a) : (b))
#endif

#ifndef MIN
#define MIN(a,b) (((a) < (b)) ? (a) : (b))
#endif


#include "fd.h"

typedef struct _TimerEventRec {
        struct timeval        te_timer_value;
	struct _TimerEventRec *te_next;
	Display		      *te_dpy;
	XtTimerCallbackProc   te_proc;
	XtAppContext	      app;
	XtPointer	      te_closure;
} TimerEventRec;

typedef struct _InputEvent {
	XtInputCallbackProc   ie_proc;
	XtPointer	      ie_closure;
	struct	_InputEvent   *ie_next;
	struct  _InputEvent   *ie_oq;
	XtAppContext	      app;
	int		      ie_source;
} InputEvent;

typedef struct _WorkProcRec {
	XtWorkProc proc;
	XtPointer closure;
	struct _WorkProcRec *next;
	XtAppContext app;
} WorkProcRec;


typedef struct 
{
  	Fd_set rmask;
	Fd_set wmask;
	Fd_set emask;
	int	nfds;
	int	count;
} FdStruct;

typedef struct _ProcessContextRec {
    XtAppContext	defaultAppContext;
    XtAppContext	appContextList;
    ConverterTable	globalConverterTable;
} ProcessContextRec, *ProcessContext;

typedef struct {
    char*	start;
    char*	current;
    int		bytes_remaining;
} Heap;

typedef struct _XtAppStruct {
    XtAppContext next;		/* link to next app in process context */
    ProcessContext process;	/* back pointer to our process context */
    CallbackList destroy_callbacks;
    Display **list;
    TimerEventRec *timerQueue;
    WorkProcRec *workQueue;
    InputEvent **selectRqueue;
    InputEvent **selectWqueue;
    InputEvent **selectEqueue;
    InputEvent *outstandingQueue;
    XrmDatabase errorDB;
    XtErrorMsgHandler errorMsgHandler, warningMsgHandler;
    XtErrorHandler errorHandler, warningHandler;
    struct _ActionListRec *action_table;
    ConverterTable converterTable;
    unsigned long selectionTimeout;
    FdStruct fds;
    short count;			/* num of assigned entries in list */
    short max;				/* allocate size of list */
    short last;
    Boolean sync, being_destroyed, error_inited, in_phase2_destroy;
    Heap heap;
    String * fallback_resources;	/* Set by XtAppSetFallbackResources. */
    struct _ActionHookRec* action_hook_list;
#ifndef NO_IDENTIFY_WINDOWS
    Boolean identify_windows;		/* debugging hack */
#endif
} XtAppStruct;

extern char* _XtHeapAlloc( /* Heap*, Cardinal */ );

extern void _XtSetDefaultErrorHandlers();
extern void _XtSetDefaultSelectionTimeout();
extern void _XtSetDefaultConverterTable();
extern void _XtFreeConverterTable();

extern XtAppContext _XtDefaultAppContext();
extern ProcessContext _XtGetProcessContext();
extern void _XtDestroyAppContexts();
extern void _XtCloseDisplays();
extern int _XtAppDestroyCount;
extern int _XtDpyDestroyCount;

extern int _XtwaitForSomething(); /* ignoreTimers, ignoreInputs, ignoreEvents,
				     block, howlong, appContext */
    /* Boolean ignoreTimers; */
    /* Boolean ignoreInputs; */
    /* Boolean ignoreEvents; */
    /* Boolean block; */
    /* unsigned long *howlong; */
    /* XtAppContext app */

typedef struct _XtPerDisplayStruct {
    CallbackList destroy_callbacks;
    Region region;
    XtCaseProc defaultCaseConverter;
    XtKeyProc defaultKeycodeTranslator;
    XtAppContext appContext;
    KeySym *keysyms;                   /* keycode to keysym table */
    int keysyms_per_keycode;           /* number of keysyms for each keycode */
    int min_keycode, max_keycode;      /* range of keycodes */
    KeySym *modKeysyms;                /* keysym values for modToKeysysm */
    ModToKeysymTable *modsToKeysyms;   /* modifiers to Keysysms index table*/
    unsigned char isModifier[32];      /* key-is-modifier-p bit table */
    KeySym lock_meaning;	       /* Lock modifier meaning */
    Modifiers mode_switch;	       /* keyboard group modifiers */
    Boolean being_destroyed;
    Boolean rv;			       /* reverse_video resource */
    XrmName name;		       /* resolved app name */
    XrmClass class;		       /* application class */
    Heap heap;
    struct _GCrec *GClist;	       /* for XtGetGC */
    Drawable* drawables;	       /* support for XtGetGC */
    Cardinal drawable_count;	       /* num entries in above list */
    String language;		       /* XPG language string */
    Atom xa_wm_colormap_windows;       /* the WM_COLORMAP_WINDOWS atom.
					  this is currently only used in 
					  XtSetColormapWindows. */
    Time last_timestamp;	       /* from last event dispatched */
    int multi_click_time;	       /* for XtSetMultiClickTime */
    struct _TMContext* tm_context;     /* for XtGetActionKeysym */
    CallbackList mapping_callbacks;    /* special case for TM */
} XtPerDisplayStruct, *XtPerDisplay;

extern void _XtPerDisplayInitialize();

typedef struct _PerDisplayTable {
	Display *dpy;
	XtPerDisplayStruct perDpy;
	struct _PerDisplayTable *next;
} PerDisplayTable, *PerDisplayTablePtr;

extern PerDisplayTablePtr _XtperDisplayList;

extern XtPerDisplay _XtSortPerDisplayList();
    /* Display *dpy */

#ifdef DEBUG
#define _XtGetPerDisplay(display) \
    ((_XtperDisplayList != NULL && (_XtperDisplayList->dpy == (display))) \
     ? &_XtperDisplayList->perDpy \
     : _XtSortPerDisplayList(display))
#else
#define _XtGetPerDisplay(display) \
    ((_XtperDisplayList->dpy == (display)) \
     ? &_XtperDisplayList->perDpy \
     : _XtSortPerDisplayList(display))
#endif /*DEBUG*/

extern void _XtDisplayInitialize();
    /* 	Display *dpy; */
    /* 	String name, classname; */
    /* 	XrmOptionDescRec *urlist; */
    /* 	Cardinal num_urs; */
    /* 	Cardinal *argc; */
    /* 	char *argv[];  */

extern void _XtCacheFlushTag();
    /*  XtAppContext app; */
    /*	XtPointer tag;	  */
