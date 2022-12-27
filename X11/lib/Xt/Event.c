#ifndef lint
static char Xrcsid[] = "$XConsortium: Event.c,v 1.112 89/12/15 23:51:06 swick Exp $";
/* $oHeader: Event.c,v 1.9 88/09/01 11:33:51 asente Exp $ */
#endif /* lint */

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

#include "IntrinsicI.h"
#include "Shell.h"
#include "StringDefs.h"

/*
 * These are definitions to make the code that handles exposure compresssion
 * easier to read.
 *
 * COMP_EXPOSE      - The compression exposure field of "widget"
 * COMP_EXPOSE_TYPE - The type of compression (lower 4 bits of COMP_EXPOSE.
 * GRAPHICS_EXPOSE  - TRUE if the widget wants graphics expose events
 *                    dispatched.
 * NO_EXPOSE        - TRUE if the widget wants No expose events dispatched.
 */

#define COMP_EXPOSE   (widget->core.widget_class->core_class.compress_exposure)
#define COMP_EXPOSE_TYPE (COMP_EXPOSE & 0x0f)
#define GRAPHICS_EXPOSE  ((XtExposeGraphicsExpose & COMP_EXPOSE) || \
			  (XtExposeGraphicsExposeMerged & COMP_EXPOSE))
#define NO_EXPOSE        (XtExposeNoExpose & COMP_EXPOSE)



extern void 			bzero();
typedef struct XtPerDisplayInputRec *XtPerDisplayInput;
extern XtPerDisplayInput 	_XtGetPerDisplayInput();
extern XtGrabList *		_XtGetGrabList();



CallbackList *_XtDestroyList;

EventMask XtBuildEventMask(widget)
    Widget widget;
{
    XtEventTable ev;
    EventMask	mask = 0L;

    for (ev = widget->core.event_table; ev != NULL; ev = ev->next)
	if (ev->select) mask |= ev->mask;
    if (widget->core.widget_class->core_class.expose != NULL)
	mask |= ExposureMask;
    if (widget->core.widget_class->core_class.visible_interest) 
	mask |= VisibilityChangeMask;

    return mask;
}

static void
RemoveEventHandler(widget, eventMask, other, proc, closure, raw, check_closure)
Widget	        widget;
EventMask       eventMask;
Boolean	        other;
XtEventHandler  proc;
XtPointer	closure;
Boolean	        raw;
Boolean	        check_closure;
{
    XtEventRec *p, **pp;
    EventMask oldMask = XtBuildEventMask(widget);

    pp = &widget->core.event_table;
    p = *pp;

    if (p == NULL) return;	                    /* No Event Handlers. */

    /* find it */
    while (p->proc != proc || (check_closure && p->closure != closure)) {
        pp = &p->next;
        p = *pp;
	if (p == NULL) return;	                     /* Didn't find it */
    }

    if (raw)
	p->raw = FALSE; 
    else 
	p->select = FALSE;

    if (p->raw || p->select) return;

    /* un-register it */
    p->mask &= ~eventMask;
    if (other) p->non_filter = FALSE;

    if (p->mask == 0 && !p->non_filter) {        /* delete it entirely */
        *pp = p->next;
	XtFree((char *)p);	
    }

    /* Reset select mask if realized and not raw. */
    if ( !raw && XtIsRealized(widget) && !widget->core.being_destroyed) {
	EventMask mask = XtBuildEventMask(widget);

	if (oldMask != mask) 
	    XSelectInput(XtDisplay(widget), XtWindow(widget), mask);
    }
}

/*	Function Name: AddEventHandler
 *	Description: An Internal routine that does the actual work of
 *                   adding the event handlers.
 *	Arguments: widget - widget to register an event handler for.
 *                 eventMask - events to mask for.
 *                 other - pass non maskable events to this proceedure.
 *                 proc - proceedure to register.
 *                 closure - data to pass to the event hander.
 *                 position - where to add this event handler.
 *                 force_new_position - If the element is already in the
 *                                      list, this will force it to the 
 *                                      beginning or end depending on position.
 *                 raw - If FALSE call XSelectInput for events in mask.
 *                 check_closure - check to see if closures match as
 *                                 as well as proceedure.
 *	Returns: none
 */

static void 
AddEventHandler(widget, eventMask, other, proc, 
		closure, position, force_new_position, raw, check_closure)
Widget	        widget;
EventMask       eventMask;
Boolean         other, force_new_position, raw, check_closure;
XtEventHandler  proc;
XtPointer	closure;
XtListPosition  position;
{
    register XtEventRec *p, *prev;
    EventMask oldMask;
    
    if (eventMask == 0 && other == FALSE) return;
    
    if (XtIsRealized(widget) && !raw) oldMask = XtBuildEventMask(widget);
    
    p = widget->core.event_table;    
    prev = NULL;

    while ((p != NULL) &&
	   (p->proc != proc || (check_closure && (p->closure != closure)))) {
	prev = p;
	p = p->next;
    }
    
    if (p == NULL) {		                /* New proc to add to list */
	p = XtNew(XtEventRec);
	p->proc = proc;
	p->closure = closure;
	p->mask = eventMask;
	p->non_filter = other;
	p->select = ! raw;
	p->raw = raw;
	
	if ( (position == XtListHead) || (prev == NULL) ) {
	    p->next = widget->core.event_table;
	    widget->core.event_table = p;
	}
	else {			/* position == XtListTail && prev != NULL */
	    prev->next = p;
	    p->next = NULL;
	}
    } 
    else {
	if (force_new_position) {
	    if (prev != NULL) 
		prev->next = p->next; /* remove p from its current location. */
	    else
		prev = widget->core.event_table = p->next;

	    if (position == XtListHead) {
		p->next = widget->core.event_table;
		widget->core.event_table = p;
	    }
	    else {		                 /* position == XtListTail */
	       	/*
		 * Find the last element in the list.
		 */
		for ( ; prev->next != NULL ; prev = prev->next );
		prev->next = p;
		p->next = NULL;
	    }
	}

	/* update existing proc */
	p->mask |= eventMask;
	p->non_filter = p->non_filter || other;
	p->select |= ! raw;
	p->raw |= raw;
	if (!check_closure) p->closure = closure;
    }

    if (XtIsRealized(widget) && !raw) {
	EventMask mask = XtBuildEventMask(widget);

	if (oldMask != mask) 
	    XSelectInput(XtDisplay(widget), XtWindow(widget), mask);
    }

}

void XtRemoveEventHandler(widget, eventMask, other, proc, closure)
    Widget	    widget;
    EventMask       eventMask;
    Boolean	    other;
    XtEventHandler  proc;
    XtPointer	    closure;
{
    RemoveEventHandler(widget, eventMask, other, proc, closure, FALSE, TRUE);
}

void XtAddEventHandler(widget, eventMask, other, proc, closure)
    Widget	    widget;
    EventMask       eventMask;
    Boolean         other;
    XtEventHandler  proc;
    XtPointer	    closure;
{
    AddEventHandler(widget, eventMask, other, 
		    proc, closure, XtListTail, FALSE, FALSE, TRUE);
}

void XtInsertEventHandler(widget, eventMask, other, proc, closure, position)
    Widget	    widget;
    EventMask       eventMask;
    Boolean         other;
    XtEventHandler  proc;
    XtPointer	    closure;
    XtListPosition  position;
{
    AddEventHandler(widget, eventMask, other, 
		    proc, closure, position, TRUE, FALSE, TRUE);
}

void XtRemoveRawEventHandler(widget, eventMask, other, proc, closure)
    Widget	    widget;
    EventMask       eventMask;
    Boolean	    other;
    XtEventHandler  proc;
    XtPointer	    closure;
{
    RemoveEventHandler(widget, eventMask, other, proc, closure, TRUE, TRUE);
}

void XtInsertRawEventHandler(widget, eventMask, other, proc, closure, position)
    Widget	    widget;
    EventMask       eventMask;
    Boolean	    other;
    XtEventHandler  proc;
    XtPointer	    closure;
    XtListPosition  position;
{
    AddEventHandler(widget, eventMask, other, 
		    proc, closure, position, TRUE, TRUE, TRUE);
}

void XtAddRawEventHandler(widget, eventMask, other, proc, closure)
    Widget	    widget;
    EventMask       eventMask;
    Boolean         other;
    XtEventHandler  proc;
    XtPointer	    closure;
{
    AddEventHandler(widget, eventMask, other, 
		    proc, closure, XtListTail, FALSE, TRUE, TRUE);
}

typedef struct _HashRec *HashPtr;

typedef struct _HashRec {
    Display	*display;
    Window	window;
    Widget	widget;
    HashPtr	next;
} HashRec;

typedef struct {
    unsigned int	size;
    unsigned int	count;
    HashPtr		entries[1];
} HashTableRec, *HashTable;

static HashTable table = NULL;

static void ExpandTable();

void _XtRegisterWindow(window, widget)
    Window window;
    Widget widget;
{
    register HashPtr hp, *hpp;

    if ((table->count + (table->count / 4)) >= table->size) ExpandTable();

    hpp = &table->entries[(unsigned int)window & (table->size-1)];
    hp = *hpp;

    while (hp != NULL) {
        if (hp->window == window && hp->display == XtDisplay(widget)) {
	    if (hp->widget != widget)
		XtAppWarningMsg(XtWidgetToApplicationContext(widget),
			"registerWindowError","xtRegisterWindow",
                         "XtToolkitError",
                        "Attempt to change already registered window.",
                          (String *)NULL, (Cardinal *)NULL);
	    return;
	}
        hpp = &hp->next;
	hp = *hpp;
    }

    hp = *hpp = XtNew(HashRec);
    hp->display = XtDisplay(widget);
    hp->window = window;
    hp->widget = widget;
    hp->next = NULL;
    table->count++;
}


void _XtUnregisterWindow(window, widget)
    Window window;
    Widget widget;
{
    HashPtr hp, *hpp;

    hpp = &table->entries[(unsigned int)window  & (table->size-1)];
    hp = *hpp;

    while (hp != NULL) {
        if (hp->window == window && hp->display == XtDisplay(widget)) {
	    if (hp->widget != widget) {
                XtAppWarningMsg(XtWidgetToApplicationContext(widget),
			"registerWindowError","xtUnregisterWindow",
                         "XtToolkitError",
                        "Attempt to unregister invalid window.",
                          (String *)NULL, (Cardinal *)NULL);

                return;
                }
             else /* found entry to delete */
                  (*hpp) = hp->next;
                  XtFree((char*)hp);
                  table->count--;
                  return;
	}
        hpp = &hp->next;
	hp = *hpp;
    }
    
}

static void ExpandTable()
{
    HashTable	oldTable = table;
    unsigned int i;

    i = oldTable->size * 2;
    table = (HashTable) XtCalloc((Cardinal)1,
	    (unsigned) sizeof(HashTableRec)+i*sizeof(HashPtr));

    table->size = i;
    table->count = 0;
    for (i = 0; i<oldTable->size; i++) {
	HashPtr hp;
	hp = oldTable->entries[i];
	while (hp != NULL) {
	    HashPtr temp = hp;
	    _XtRegisterWindow(hp->window, hp->widget);
	    hp = hp->next;
	    XtFree((char *) temp);
	}
    }
    XtFree((char *)oldTable);
}


/*ARGSUSED*/
Widget XtWindowToWidget(display, window)
    Display *display;
    Window window;
{
    register HashPtr hp;

    for (hp = table->entries[(unsigned int)window & (table->size-1)];
	    hp != NULL; hp = hp->next) {
	if (hp->window == window && hp->display == display) return hp->widget;
    }

    return NULL;
}

static void InitializeHash()
{
    int size = sizeof(HashTableRec)+1024*sizeof(HashPtr);

    table = (HashTable) XtMalloc((unsigned) size);
    bzero((char *) table, size);

    table->size = 1024;
    table->count = 0;
}

static Region nullRegion;

static Boolean DispatchEvent(event, widget, mask, pd)
    register XEvent    *event;
    Widget    widget;
    EventMask mask;
    XtPerDisplay pd;
{
    XtEventRec *p;   
    XtEventHandler proc[100];
    XtPointer closure[100];
    int numprocs, i;
    XEvent nextEvent;
    Boolean was_dispatched = False;

    if ( (mask == ExposureMask) ||
	 ((event->type == NoExpose) && NO_EXPOSE) ||
	 ((event->type == GraphicsExpose) && GRAPHICS_EXPOSE) ) {

	if (widget->core.widget_class->core_class.expose != NULL ) {

	    /* We need to mask off the bits that could contain the information
	     * about whether or not we desire Graphics and NoExpose events.  */

	    if ( (COMP_EXPOSE_TYPE == XtExposeNoCompress) || 
		 (event->type == NoExpose) )

		(*widget->core.widget_class->core_class.expose)
		    (widget, event, (Region)NULL);
	    else {
		static void CompressExposures();
		CompressExposures(event, widget, pd);
	    }
	    was_dispatched = True;
	}
    }

    if (mask == EnterWindowMask &&
	    widget->core.widget_class->core_class.compress_enterleave) {
	if (XPending(event->xcrossing.display)) {
	    XPeekEvent(event->xcrossing.display, &nextEvent);
	    if (nextEvent.type == LeaveNotify &&
		  event->xcrossing.window == nextEvent.xcrossing.window &&
		  event->xcrossing.subwindow == nextEvent.xcrossing.subwindow){
		/* skip the enter/leave pair */
		XNextEvent(event->xcrossing.display, &nextEvent);
		return False;
	    }
	}
    }

    if (event->type == MotionNotify &&
	    widget->core.widget_class->core_class.compress_motion) {
	while (XPending(event->xmotion.display)) {
	    XPeekEvent(event->xmotion.display, &nextEvent);
	    if (nextEvent.type == MotionNotify &&
		    event->xmotion.window == nextEvent.xmotion.window &&
		    event->xmotion.subwindow == nextEvent.xmotion.subwindow) {
		/* replace the current event with the next one */
		XNextEvent(event->xmotion.display, event);
	    } else break;
	}
    }

    if ((mask == VisibilityChangeMask) &&
        XtClass(widget)->core_class.visible_interest) {
	    was_dispatched = True;
	    /* our visibility just changed... */
	    switch (((XVisibilityEvent *)event)->state) {
		case VisibilityUnobscured:
		    widget->core.visible = TRUE;
		    break;

		case VisibilityPartiallyObscured:
		    /* what do we want to say here? */
		    /* well... some of us is visible */
		    widget->core.visible = TRUE;
		    break;

		case VisibilityFullyObscured:
		    widget->core.visible = FALSE;
		    /* do we want to mark our children obscured? */
		    break;
	    }
	}

    /* Have to copy the procs into an array, because calling one of them */
    /* might call XtRemoveEventHandler, which would break our linked list.*/

    numprocs = 0;

    for (p=widget->core.event_table; p != NULL; p = p->next) {
	if ((mask & p->mask) != 0 || (mask == 0 && p->non_filter)) {
	    proc[numprocs] = p->proc;
	    closure[numprocs++] = p->closure;
	}
    }

    {
	Boolean continue_to_dispatch = True;
	for (i=0 ; i < numprocs && continue_to_dispatch; i++)
	    (*(proc[i]))(widget, closure[i], event, &continue_to_dispatch);
    }
    return (numprocs > 0 || was_dispatched);
}

/*
 * This structure is passed into the check exposure proc.
 */

typedef struct _CheckExposeInfo {
    int type1, type2;		/* Types of events to check for. */
    Boolean maximal;		/* Ignore non-exposure events? */
    Boolean non_matching;	/* Was there an event that did not 
				   match eighter type? */
    Window window;		/* Window to match. */
} CheckExposeInfo;

#define GetCount(ev) ( ((ev)->type == GraphicsExpose) ? \
		       (ev)->xgraphicsexpose.count : (ev)->xexpose.count)

/*	Function Name: CompressExposures
 *	Description: Handles all exposure compression
 *	Arguments: event - the xevent that is to be dispatched
 *                 widget - the widget that this event occured in.
 *	Returns: none.
 *
 *      NOTE: Event must be of type Expose or GraphicsExpose.
 */

static void
CompressExposures(event, widget, pd)
Widget widget;
XEvent * event;
XtPerDisplay pd;
{
    CheckExposeInfo info;
    static void SendExposureEvent();
    int count;

    XtAddExposureToRegion(event, pd->region);

    if ( GetCount(event) != 0 )
	return;

    if ( (COMP_EXPOSE_TYPE == XtExposeCompressSeries) ||
	 (XEventsQueued(XtDisplay(widget), QueuedAfterReading) == 0) ) {
	SendExposureEvent(event, widget, pd);
	return;
    }

    if (COMP_EXPOSE & XtExposeGraphicsExposeMerged) {
	info.type1 = Expose;
	info.type2 = GraphicsExpose;
    }
    else {
	info.type1 = event->type;
	info.type2 = 0;
    }
    info.maximal =(COMP_EXPOSE_TYPE == XtExposeCompressMaximal) ? True : False;
    info.non_matching = FALSE;
    info.window = XtWindow(widget);

/*
 * We have to be very careful here not to hose down the processor
 * when blocking until count gets to zero.
 *
 * First, check to see if there are any events in the queue for this
 * widget, and of the correct type.
 *
 * Once we cannot find any more events, check to see that count is zero. 
 * If it is not then block until we get another exposure event.
 *
 * If we find no more events, and count on the last one we saw was zero we
 * we can be sure that all events have been processed.
 *
 * Unfortunately, we wind up having to look at the entire queue
 * event if we're not doing Maximal compression, due to the
 * semantics of XCheckIfEvent (we can't abort without re-ordering
 * the event queue as a side-effect).
 */

    count = 0;
    while (TRUE) {
	XEvent event_return;
	static Bool CheckExposureEvent();

	if (XCheckIfEvent(XtDisplay(widget), &event_return, 
			  CheckExposureEvent, (char *) &info)) {

	    count = GetCount(&event_return);
	    XtAddExposureToRegion(&event_return, pd->region);
	}
	else if (count != 0) {
	    XIfEvent(XtDisplay(widget), &event_return,
		     CheckExposureEvent, (char *) &info);
	    count = GetCount(&event_return);
	    XtAddExposureToRegion(&event_return, pd->region);
	}
	else /* count == 0 && XCheckIfEvent Failed. */
	    break;
    }

    SendExposureEvent(event, widget, pd);
}

/*	Function Name: SendExposureEvent
 *	Description: Sets the x, y, width, and height of the event
 *                   to be the clip box of Expose Region.
 *	Arguments: event  - the X Event to mangle; Expose or GraphicsExpose.
 *                 widget - the widget that this event occured in.
 *                 pd     - the per display information for this widget.
 *	Returns: none.
 */

static void
SendExposureEvent(event, widget, pd)
XEvent * event;
Widget widget;
XtPerDisplay pd;
{
    XtExposeProc expose = widget->core.widget_class->core_class.expose;
    XRectangle rect;

    XClipBox(pd->region, &rect);
    if (event->type == Expose) {
	event->xexpose.x = rect.x;
	event->xexpose.y = rect.y;
	event->xexpose.width = rect.width;
	event->xexpose.height = rect.height;
    }
    else {			/* Graphics Expose Event. */
	event->xgraphicsexpose.x = rect.x;
	event->xgraphicsexpose.y = rect.y;
	event->xgraphicsexpose.width = rect.width;
	event->xgraphicsexpose.height = rect.height;
    }
    (*expose)(widget, event, pd->region);
    (void) XIntersectRegion(nullRegion, pd->region, pd->region);
}

/*	Function Name: CheckExposureEvent
 *	Description: Checks the event cue for an expose event
 *	Arguments: display - the display connection.
 *                 event - the event to check.
 *                 arg - a pointer to the exposure info structure.
 *	Returns: TRUE if we found an event of the correct type
 *               with the right window.
 *
 * NOTE: The only valid types (info.type1 and info.type2) are Expose
 *       and GraphicsExpose.
 */

/* ARGSUSED */
static Bool
CheckExposureEvent(disp, event, arg)
Display * disp;
XEvent * event;
char * arg;
{
    CheckExposeInfo * info = ((CheckExposeInfo *) arg);

    if ( (info->type1 == event->type) || (info->type2 == event->type)) {
	if (!info->maximal && info->non_matching) return FALSE;
	if (event->type == GraphicsExpose)
	    return(event->xgraphicsexpose.drawable == info->window);
	return(event->xexpose.window == info->window);
    }
    info->non_matching = TRUE;
    return(FALSE);
}

typedef enum _GrabType {pass, ignore, remap} GrabType;

static void ConvertTypeToMask (eventType, mask, grabType)
    int		eventType;
    EventMask   *mask;
    GrabType    *grabType;
{

static struct {
    EventMask   mask;
    GrabType    grabType;
} masks[] = {
    {0,				pass},      /* shouldn't see 0  */
    {0,				pass},      /* shouldn't see 1  */
    {KeyPressMask,		remap},     /* KeyPress		*/
    {KeyReleaseMask,		remap},     /* KeyRelease       */
    {ButtonPressMask,		remap},     /* ButtonPress      */
    {ButtonReleaseMask,		remap},     /* ButtonRelease    */
    {PointerMotionMask
     | Button1MotionMask
     | Button2MotionMask
     | Button3MotionMask
     | Button4MotionMask
     | Button5MotionMask
     | ButtonMotionMask,	ignore},    /* MotionNotify	*/
    {EnterWindowMask,		ignore},    /* EnterNotify	*/
    {LeaveWindowMask,		pass},      /* LeaveNotify	*/
    {FocusChangeMask,		pass},      /* FocusIn		*/
    {FocusChangeMask,		pass},      /* FocusOut		*/
    {KeymapStateMask,		pass},      /* KeymapNotify	*/
    {ExposureMask,		pass},      /* Expose		*/
    {0,				pass},      /* GraphicsExpose   */
    {0,				pass},      /* NoExpose		*/
    {VisibilityChangeMask,      pass},      /* VisibilityNotify */
    {SubstructureNotifyMask,    pass},      /* CreateNotify	*/
    {StructureNotifyMask
     | SubstructureNotifyMask,  pass},      /* DestroyNotify	*/
    {StructureNotifyMask
     | SubstructureNotifyMask,  pass},      /* UnmapNotify	*/
    {StructureNotifyMask
     | SubstructureNotifyMask,  pass},      /* MapNotify	*/
    {SubstructureRedirectMask,  pass},      /* MapRequest	*/
    {StructureNotifyMask
     | SubstructureNotifyMask,  pass},      /* ReparentNotify   */
    {StructureNotifyMask
     | SubstructureNotifyMask,  pass},      /* ConfigureNotify  */
    {SubstructureRedirectMask,  pass},      /* ConfigureRequest */
    {StructureNotifyMask
     | SubstructureNotifyMask,  pass},      /* GravityNotify	*/
    {ResizeRedirectMask,	pass},      /* ResizeRequest	*/
    {StructureNotifyMask
     | SubstructureNotifyMask,  pass},      /* CirculateNotify	*/
    {SubstructureRedirectMask,  pass},      /* CirculateRequest */
    {PropertyChangeMask,	pass},      /* PropertyNotify   */
    {0,				pass},      /* SelectionClear   */
    {0,				pass},      /* SelectionRequest */
    {0,				pass},      /* SelectionNotify  */
    {ColormapChangeMask,	pass},      /* ColormapNotify   */
    {0,				pass},      /* ClientMessage	*/
    {0,				pass},      /* MappingNotify	*/
  };

    eventType &= 0x7f;	/* Events sent with XSendEvent have high bit set. */
    (*mask)      = masks[eventType].mask;
    (*grabType)  = masks[eventType].grabType;
}

Boolean _XtOnGrabList(widget, grabList)
    register Widget widget;
    XtGrabRec  *grabList;
{
    register XtGrabRec* gl;
    for (; widget != NULL; widget = (Widget)widget->core.parent) {
	for (gl = grabList; gl != NULL; gl = gl->next) {
	    if (gl->widget == widget) return TRUE;
	    if (gl->exclusive) break;
	}
    }
    return FALSE;
}
static Widget LookupSpringLoaded(grabList)
    XtGrabList	grabList;
{
    XtGrabList	gl;

    for (gl = grabList; gl != NULL; gl = gl->next) {
	if (gl->spring_loaded)
	  if (XtIsSensitive(gl->widget))
	    return gl->widget;
	  else
	    return NULL;
	if (gl->exclusive) break;
    }
    return NULL;
}

static Boolean DecideToDispatch(event)
    XEvent  *event;
{
    register    Widget widget;
    EventMask   mask;
    GrabType    grabType;
    Widget	dspWidget;
    Time	time = 0;
    XtPerDisplay pd;
    XtPerDisplayInput pdi;
    XtGrabList  grabList;

    extern XtPerDisplayInput _XtGetPerDisplayInput();
    
    widget = XtWindowToWidget (event->xany.display, event->xany.window);
    pd = _XtGetPerDisplay(event->xany.display);
    pdi = _XtGetPerDisplayInput(event->xany.display);
    grabList = *(XtGrabList *)_XtGetGrabList(pdi);
    
    
    /* Lint complains about &grabType not matching the declaration.
       Don't bother trying to fix it, it won't work */
    ConvertTypeToMask(event->xany.type, &mask, &grabType);

    switch (event->xany.type) {

      case KeyPress:
      case KeyRelease:		time = event->xkey.time; break;

      case ButtonPress:
      case ButtonRelease:	time = event->xbutton.time; break;

      case MotionNotify:	time = event->xmotion.time; break;

      case EnterNotify:
      case LeaveNotify:		time = event->xcrossing.time; break;

      case PropertyNotify:	time = event->xproperty.time; break;

      case SelectionClear:	time = event->xselectionclear.time; break;
    }

    if (time) pd->last_timestamp = time;

    if (widget == NULL) {
	if (grabType != remap) return False;
	/* event occurred in a non-widget window, but we've promised also
	   to dispatch it to the nearest accessible spring_loaded widget */
	else if ((widget = LookupSpringLoaded(grabList)) != NULL)
	    return DispatchEvent(event, widget, mask, pd);
	return False;
    }

    switch(grabType) {
	case pass:
	    return DispatchEvent(event, widget, mask, pd);

	case ignore:
	    if ((grabList == NULL || _XtOnGrabList(widget,grabList))
		&& XtIsSensitive(widget)) {
		return DispatchEvent(event, widget, mask, pd);
	    }
	    return False;

	case remap:
	    
	    {
		Boolean was_dispatched = False;
		extern Widget _XtFindRemapWidget();
		extern void _XtUngrabBadGrabs();

		dspWidget = _XtFindRemapWidget(event, widget, mask, pdi);
		
		if ((grabList == NULL || 
		     _XtOnGrabList(dspWidget, grabList)) &&
		    XtIsSensitive(dspWidget)) {
		    was_dispatched = DispatchEvent(event, dspWidget,
						   mask, pd);
		}
		else _XtUngrabBadGrabs(event, widget, mask, pdi);
		
		/* Also dispatch to nearest accessible spring_loaded. */
		/* Fetch this afterward to reflect modal list changes */
		grabList = *(XtGrabList *)_XtGetGrabList(pdi);
		widget = LookupSpringLoaded(grabList);
		if (widget != NULL && widget != dspWidget) {
		    was_dispatched |= DispatchEvent(event, widget,
						    mask, pd);
		}
		
		return was_dispatched;
	    }
    }
    /* should never reach here */
    return False;
}

Boolean XtDispatchEvent (event)
    XEvent  *event;
{
    CallbackList *oldDestroyList, destroyList;
    Boolean was_dispatched;

    /*
     * To make recursive XtDispatchEvent work, we need to do phase 2 destroys
     * only on those widgets destroyed by this particular dispatch.
     * The "right" way to do this is by passing a local variable through to
     * each recursive instance, and passing the list to XtDestroy, but that
     * causes unwieldy proliferation of arguments. We could do all this stuff
     * with signals (if we had them), but instead we have a global pointer
     * to the "current" destroy list, and XtDispatchEvent and XtDestroy
     * conspire to keep it up to date, and use the right one.
     *
     * This is pretty gross.
     */

    oldDestroyList = _XtDestroyList;
    _XtDestroyList = &destroyList;
    destroyList = NULL;

    was_dispatched = DecideToDispatch(event);

    /* To accomodate widgets destroying other widgets in their destroy
     * callbacks, we have to make this a loop */

    while (destroyList != NULL) {
	CallbackList newList = NULL;
	_XtDestroyList = &newList;
	_XtCallCallbacks (&destroyList, (XtPointer) NULL);
	_XtRemoveAllCallbacks (&destroyList);
	destroyList = newList;
    }

    _XtDestroyList = oldDestroyList;

    if (_XtSafeToDestroy) {
	if (_XtAppDestroyCount != 0) _XtDestroyAppContexts();
	if (_XtDpyDestroyCount != 0) _XtCloseDisplays();
    }
    return was_dispatched;
}

static Boolean RemoveGrab();

/* ARGSUSED */
static void GrabDestroyCallback(widget, closure, call_data)
    Widget  widget;
    XtPointer closure;
    XtPointer call_data;
{
    /* Remove widget from grab list if it destroyed */
    (void)RemoveGrab(widget, False);
}

static XtGrabRec *NewGrabRec(widget, exclusive, spring_loaded)
    Widget  widget;
    Boolean exclusive;
    Boolean spring_loaded;
{
    register XtGrabList    gl;

    gl		      = XtNew(XtGrabRec);
    gl->next	      = NULL;
    gl->widget        = widget;
    gl->exclusive     = exclusive;
    gl->spring_loaded = spring_loaded;

    return gl;
}

void XtAddGrab(widget, exclusive, spring_loaded)
    Widget  widget;
    Boolean exclusive;
    Boolean spring_loaded;
{
    register    XtGrabList gl;
    XtGrabList	*grabListPtr;

    grabListPtr = 
      _XtGetGrabList(_XtGetPerDisplayInput(XtDisplay(widget)));
      
					 

    if (spring_loaded && !exclusive) {
	XtAppWarningMsg(XtWidgetToApplicationContext(widget),
		"grabError", "grabDestroyCallback", "XtToolkitError",
		"XtAddGrab requires exclusive grab if spring_loaded is TRUE",
		(String *) NULL, (Cardinal *) NULL);
	exclusive = TRUE;
    }

    gl = NewGrabRec(widget, exclusive, spring_loaded);
    gl->next = *grabListPtr;
    *grabListPtr = gl;

    XtAddCallback (widget, XtNdestroyCallback, 
	    GrabDestroyCallback, (XtPointer) NULL);
}

static Boolean RemoveGrab(widget)
    Widget  widget;
    /* returns False if no grab entry was found, True otherwise */
{
    register XtGrabList gl;
    register Boolean done;
    XtGrabList	*grabListPtr;

    grabListPtr = 
      _XtGetGrabList(_XtGetPerDisplayInput(XtDisplay(widget)));
      
    for (gl = *grabListPtr; gl != NULL; gl = gl->next) {
	if (gl->widget == widget) break;
    }

    if (gl == NULL) {
	    XtAppWarningMsg(XtWidgetToApplicationContext(widget),
		       "grabError","xtRemoveGrab","XtToolkitError",
		       "XtRemoveGrab asked to remove a widget not on the list",
		       (String *)NULL, (Cardinal *)NULL);
	    return False;
	}	

    do {
	gl = *grabListPtr;
	done = (gl->widget == widget);
	*grabListPtr = gl->next;
	XtRemoveCallback(gl->widget, XtNdestroyCallback,
		GrabDestroyCallback, (XtPointer)NULL);
	XtFree((char *)gl);
    } while (! done);
    return True;
}

void XtRemoveGrab(widget)
    Widget  widget;
{
    (void)RemoveGrab(widget);
}

void XtMainLoop()
{
	XtAppMainLoop(_XtDefaultAppContext());
}

void XtAppMainLoop(app)
	XtAppContext app;
{
    XEvent event;

    for (;;) {
    	XtAppNextEvent(app, &event);
	XtDispatchEvent(&event);
    }
}


void _XtEventInitialize()
{
    static Boolean initialized = FALSE;
    if (initialized) return;
    initialized = TRUE;

    _XtDestroyList = NULL;
    nullRegion = XCreateRegion();
    InitializeHash();
}

void XtAddExposureToRegion(event, region)
    XEvent   *event;
    Region   region;
{
    XRectangle rect;

    switch (event->type) {
	case Expose:
		rect.x = event->xexpose.x;
		rect.y = event->xexpose.y;
		rect.width = event->xexpose.width;
		rect.height = event->xexpose.height;
		break;
	case GraphicsExpose:
		rect.x = event->xgraphicsexpose.x;
		rect.y = event->xgraphicsexpose.y;
		rect.width = event->xgraphicsexpose.width;
		rect.height = event->xgraphicsexpose.height;
		break;
	default:
		return;
    }

    XUnionRectWithRegion(&rect, region, region);
}


void _XtFreeEventTable(event_table)
    XtEventTable *event_table;
{
    register XtEventTable event;

    event = *event_table;
    while (event != NULL) {
	register XtEventTable next = event->next;
	XtFree((char *) event);
	event = next;
    }
}

Time XtLastTimestampProcessed(dpy)
    Display *dpy;
{
    return _XtGetPerDisplay(dpy)->last_timestamp;
}
      

void _XtSendFocusEvent(child, type)
    Widget child;
    int type;
{

    child = XtIsWidget(child) ? child : _XtWindowedAncestor(child);
    if (XtIsSensitive(child) && !child->core.being_destroyed
	&& XtIsRealized(child)
	&& (XtBuildEventMask(child) & FocusChangeMask))
    {
	EventMask mask;
	GrabType grabType;
	XFocusChangeEvent event;

	event.type = type;
	event.serial = LastKnownRequestProcessed(XtDisplay(child));
	event.send_event = True;
	event.display = XtDisplay(child);
	event.window = XtWindow(child);
	event.mode = NotifyNormal;
	event.detail = NotifyAncestor;
	ConvertTypeToMask(type, &mask, &grabType);
	DispatchEvent((XEvent*)&event, child, mask,
		      _XtGetPerDisplay(XtDisplay(child)));
    }
}
