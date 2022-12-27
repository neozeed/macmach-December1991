#ifndef lint
static char Xrcsid[] =
    "$XConsortium: Selection.c,v 1.45 90/02/16 11:45:54 kit Exp $";
#endif

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
#include "StringDefs.h"
#include "SelectionI.h"
#include <X11/Xatom.h>

extern void bcopy();

void _XtSetDefaultSelectionTimeout(timeout)
	unsigned long *timeout;
{
	*timeout = 5000; /* default to 5 seconds */
}

void XtSetSelectionTimeout(timeout)
	unsigned long timeout;
{
	XtAppSetSelectionTimeout(_XtDefaultAppContext(), timeout);
}

void XtAppSetSelectionTimeout(app, timeout)
	XtAppContext app;
	unsigned long timeout;
{
	app->selectionTimeout = timeout;
}

unsigned int XtGetSelectionTimeout()
{
	return XtAppGetSelectionTimeout(_XtDefaultAppContext());
}

unsigned int XtAppGetSelectionTimeout(app)
	XtAppContext app;
{
	return app->selectionTimeout;
}


/* General utilities */

static XContext selectPropertyContext = 0;

/*ARGSUSED*/
static void FreePropList(w, closure, callData)
 Widget w;			/* unused */
 XtPointer closure;
 XtPointer callData;		/* unused */
{
    PropList sarray = (PropList)closure;
    XDeleteContext(sarray->dpy, DefaultRootWindow(sarray->dpy),
		   selectPropertyContext);
    XtFree((XtPointer)sarray->list);
    XtFree(closure);
}


static PropList GetPropList(dpy)
    Display *dpy;
{
    PropList sarray;
    if (selectPropertyContext == 0)
	selectPropertyContext = XUniqueContext();
    if (XFindContext(dpy, DefaultRootWindow(dpy), selectPropertyContext,
		     (caddr_t *)&sarray)) {
	XtPerDisplay pd = _XtGetPerDisplay(dpy);
	sarray = (PropList) XtMalloc((unsigned) sizeof(PropListRec));
	sarray->dpy = dpy;
	sarray->incremental_atom = XInternAtom(dpy, "INCR", FALSE);
	sarray->indirect_atom = XInternAtom(dpy, "MULTIPLE", FALSE);
	sarray->timestamp_atom = XInternAtom(dpy, "TIMESTAMP", FALSE);
	sarray->propCount = 1;
	sarray->list = (SelectionProp)XtMalloc((unsigned) sizeof(SelectionPropRec));
	sarray->list[0].prop = XInternAtom(dpy, "_XT_SELECTION_0", FALSE);
	sarray->list[0].avail = TRUE;
	(void) XSaveContext(dpy, DefaultRootWindow(dpy), selectPropertyContext, 
			    (caddr_t) sarray);
	_XtAddCallback( (Widget)NULL, &pd->destroy_callbacks,
			FreePropList, (XtPointer)sarray );
    }
    return sarray;
}


static Atom GetSelectionProperty(dpy)
Display *dpy;
{
 SelectionProp p;
 int propCount;
 char propname[80];
 PropList sarray = GetPropList(dpy);

 for (p = sarray->list, propCount=sarray->propCount;
	propCount;
	p++, propCount--) {
   if (p->avail) {
      p->avail = FALSE;
      return(p->prop);
   }
 }
 propCount = sarray->propCount++;
 sarray->list = (SelectionProp) XtRealloc((XtPointer)sarray->list, 
  		(unsigned)(sarray->propCount*sizeof(SelectionPropRec)));
 (void) sprintf(propname, "%s%d", "_XT_SELECTION_", propCount);
 sarray->list[propCount].prop = XInternAtom(dpy, propname, FALSE);
 sarray->list[propCount].avail = FALSE;
 return(sarray->list[propCount].prop);
}

static FreeSelectionProperty(dpy, prop)
Display *dpy;
Atom prop;
{
 SelectionProp p;
 PropList sarray;
 if (prop == None) return;
 if (XFindContext(dpy, DefaultRootWindow(dpy), selectPropertyContext,
    (caddr_t *)&sarray)) 
    XtAppErrorMsg(XtDisplayToApplicationContext(dpy),
	    "noSelectionProperties", "freeSelectionProperty", "XtToolkitError",
		"internal error: no selection property context for display",
		 (String *)NULL,  (Cardinal *)NULL );
 for (p = sarray->list; p; p++) 
   if (p->prop == prop) {
      p->avail = TRUE;
      return;
      }
}

static CallBackInfo MakeInfo(ctx, callback, closures, count, widget,
			     time, incremental)
Select ctx;
XtSelectionCallbackProc callback;
XtPointer *closures;
int count;
Widget widget;
Time time;
Boolean incremental;
{
    	static void HandleSelectionReplies();
    	CallBackInfo info = XtNew(CallBackInfoRec);

	info->ctx = ctx;
	info->callback = callback;
	info->req_closure =
	    (XtPointer*)XtMalloc((unsigned) (count * sizeof(XtPointer)));
	bcopy((char*)closures, (char*)info->req_closure, count * sizeof(XtPointer));
        info->property = GetSelectionProperty(XtDisplay(widget));
	info->proc = HandleSelectionReplies;
	info->widget = widget;
	info->time = time;
	info->incremental = incremental;
	return (info);
}

static RequestSelectionValue(info, selection, target)
CallBackInfo info;
Atom selection;
Atom target;
{
    static void HandleSelectionReplies();
    static void ReqTimedOut();
#ifndef DEBUG_WO_TIMERS
    XtAppContext app = XtWidgetToApplicationContext(info->widget);
	info->timeout = XtAppAddTimeOut(app,
			 app->selectionTimeout, ReqTimedOut, (XtPointer)info);
#endif 
	XtAddEventHandler(info->widget, (EventMask) NULL, TRUE,
			  HandleSelectionReplies, (XtPointer)info);
	XConvertSelection(info->ctx->dpy, selection, target, 
			  info->property, XtWindow(info->widget), info->time);
}



static Select FindCtx(dpy, selection)
Display *dpy;
Atom selection;
{
    Select ctx;
    static XContext selectContext = 0;

    if (selectContext == 0)
	selectContext = XUniqueContext();
    if (XFindContext(dpy, (Window)selection, selectContext, (caddr_t *)&ctx)) {
	PropList sarray = GetPropList(dpy);
	ctx = XtNew(SelectRec);
	ctx->dpy = dpy;
	ctx->selection = selection;
	ctx->widget = NULL;
	ctx->prop_list = sarray;
	(void)XSaveContext(dpy, (Window)selection, selectContext, (caddr_t)ctx);
    }
    return ctx;
}

/*ARGSUSED*/
static void WidgetDestroyed(widget, closure, data)
Widget widget;
XtPointer closure, data;
{
    Select ctx = (Select) closure;
    if (ctx->widget == widget)
	ctx->widget = NULL;
}

/* Selection Owner code */

static Boolean LoseSelection(ctx, widget, selection, time)
Select ctx;
Widget widget;
Atom selection;
Time time;
{
    static void HandleSelectionEvents();

    if ((ctx->widget == widget) &&
	(ctx->selection == selection) && /* paranoia */
	((time == CurrentTime) || (time >= ctx->time)))
    {
	XtRemoveEventHandler(widget, (EventMask) NULL, TRUE,
			     HandleSelectionEvents, (XtPointer)ctx); 
	XtRemoveCallback(widget, XtNdestroyCallback, 
			 WidgetDestroyed, (XtPointer)ctx); 
	ctx->widget = NULL; /* widget officially loses ownership */
	/* now inform widget */
	if (ctx->loses) { 
	    if (ctx->incremental)  
	       (*ctx->loses)(widget, &ctx->selection, ctx->owner_closure);
	    else  (*ctx->loses)(widget, &ctx->selection);
	}
	return(TRUE);
    }
    else return(FALSE);
}

static XContext selectWindowContext = 0;

static void AddHandler(dpy, window, widget, mask, proc, closure)
Display *dpy;
Window window;
Widget widget;
EventMask mask;
XtEventHandler proc;
XtPointer closure;
{
    if (XtWindow(widget) == window)
	XtAddEventHandler(widget, mask, TRUE, proc, closure);
    else {
	Widget w = XtWindowToWidget(dpy, window);
	RequestWindowRec* requestWindow;
	if (w != NULL && w != widget) widget = w;
	if (selectWindowContext == 0)
	    selectWindowContext = XUniqueContext();
	if (XFindContext(dpy, window, selectWindowContext,
			 (caddr_t *)&requestWindow)) {
	    requestWindow = XtNew(RequestWindowRec);
	    requestWindow->active_transfer_count = 0;
	    (void)XSaveContext(dpy, window, selectWindowContext,
			       (caddr_t)requestWindow);
	}
	if (requestWindow->active_transfer_count++ == 0) {
	    _XtRegisterWindow(window, widget);
	    XSelectInput(dpy, window, mask);
	}
	XtAddRawEventHandler(widget, mask, FALSE, proc, closure);
    }
}

static void RemoveHandler(dpy, window, widget, mask, proc, closure)
Display *dpy;
Window window;
Widget widget;
EventMask mask;
XtEventHandler proc;
XtPointer closure;
{
    if ((XtWindowToWidget(dpy, window) == widget) && 
        (XtWindow(widget) != window)) {
	/* we had to hang this window onto our widget; take it off */
	RequestWindowRec* requestWindow;
	XtRemoveRawEventHandler(widget, mask, TRUE, proc, closure);
	(void)XFindContext(dpy, window, selectWindowContext,
			   (caddr_t *)&requestWindow);
	if (--requestWindow->active_transfer_count == 0) {
	    _XtUnregisterWindow(window, widget);
	    XSelectInput(dpy, window, 0L);
	    (void)XDeleteContext(dpy, window, selectWindowContext);
	    XtFree((XtPointer)requestWindow);
	}
    } else {
        XtRemoveEventHandler(widget, mask, TRUE,  proc, closure); 
    }
}

/* ARGSUSED */
static void OwnerTimedOut(closure, id)
XtPointer closure;
XtIntervalId   *id;
{
    Request req = (Request)closure;
    Select ctx = req->ctx;
    static void HandlePropertyGone();

    if (ctx->incremental && (ctx->owner_cancel != NULL)) {
	(*ctx->owner_cancel)(ctx->widget, &ctx->selection, 
			     &req->target, (XtRequestId*)&req,
			     ctx->owner_closure);
    } else {
	if (ctx->notify == NULL)
	    XtFree((XtPointer)req->value);
	else {
	    /* the requestor hasn't deleted the property, but
	     * the owner needs to free the value.
	     */
	    if (ctx->incremental)
		(*ctx->notify)(ctx->widget, &ctx->selection, &req->target, 
			       (XtRequestId*)&req, ctx->owner_closure);
	    else
		(*ctx->notify)(ctx->widget, &ctx->selection, &req->target);
	}
    }

    RemoveHandler(ctx->dpy, req->requestor, req->widget,
	  	(EventMask) PropertyChangeMask, HandlePropertyGone, closure); 
    XtFree((XtPointer)req);
}

static void SendIncrement(incr)
    Request incr;
{
    Display *dpy = incr->ctx->dpy;

    int incrSize = MAX_SELECTION_INCR(dpy);
    if (incrSize >  incr->bytelength - incr->offset)
        incrSize = incr->bytelength - incr->offset;
    XChangeProperty(dpy, incr->requestor, incr->property, 
    	    incr->type, incr->format, PropModeReplace, 
	    (unsigned char *)&incr->value[incr->offset], 
	    NUMELEM(incrSize, incr->format));
    incr->offset += incrSize;
}

static AllSent(req)
Request req;
{
    Select ctx = req->ctx;
    XChangeProperty(ctx->dpy, req->requestor, 
		    req->property, req->type,  req->format, 
		    PropModeReplace, (unsigned char *) NULL, 0);
    req->allSent = TRUE;

    if (ctx->notify == NULL) XtFree((XtPointer)req->value);
}

static void HandlePropertyGone(widget, closure, ev)
Widget widget;
XtPointer closure;
XEvent *ev;
{


    XPropertyEvent *event = (XPropertyEvent *) ev;
    Request req = (Request)closure;
    Select ctx = req->ctx;

    if ((event->type != PropertyNotify) ||
        (event->state != PropertyDelete) ||
	(event->atom != req->property) ||
	(event->window != req->requestor))
      return;
#ifndef DEBUG_WO_TIMERS
    XtRemoveTimeOut(req->timeout);
#endif 
    if (req->allSent) { 
	if (ctx->notify)  
	    if (ctx->incremental) {
		(*ctx->notify)(ctx->widget, &ctx->selection, &req->target,
			       (XtRequestId*)&req, ctx->owner_closure);
	    }
	    else (*ctx->notify)(ctx->widget, &ctx->selection, &req->target);
	RemoveHandler(event->display, event->window, widget,
	  	(EventMask) PropertyChangeMask, HandlePropertyGone, closure); 
	XtFree((XtPointer)req);
    } else  { /* is this part of an incremental transfer? */ 
	if (ctx->incremental) {
	     if (req->bytelength == 0)
		AllSent(req);
	     else {
		unsigned long size = MAX_SELECTION_INCR(ctx->dpy);
    		SendIncrement(req);
		(*ctx->convert)(ctx->widget, &ctx->selection, &req->target, 
			    &req->type, &req->value, 
			    &req->bytelength, &req->format,
			    &size, ctx->owner_closure, (XtRequestId*)&req);
		if (req->bytelength)
		    req->bytelength = BYTELENGTH(req->bytelength, req->format);
		req->offset = 0;
	    }
	} else {
	    if (req->offset < req->bytelength) 
		SendIncrement(req);
	    else AllSent(req);
	}
#ifndef DEBUG_WO_TIMERS
	{
	  XtAppContext app = XtWidgetToApplicationContext(req->widget);
	  req->timeout = XtAppAddTimeOut(app,
			 app->selectionTimeout, OwnerTimedOut, (XtPointer)req);
	}
#endif 
    }
}

static PrepareIncremental(req, widget, window, property, target, 
	 targetType, value, length, format)
Request req;
Widget widget;
Window window;
Atom target;
Atom property;
Atom targetType;
XtPointer value;
unsigned long length;
int format;
{
    unsigned long size;

	req->requestor = window;
	req->type = targetType;
	req->property = property;
	req->value = value;
	req->bytelength = BYTELENGTH(length,format);
	req->format = format;
	req->offset = 0;
	req->target = target;
	req->widget = widget;
	req->allSent = FALSE;
#ifndef DEBUG_WO_TIMERS
	{
	XtAppContext app = XtWidgetToApplicationContext(widget);
	req->timeout = XtAppAddTimeOut(app,
			 app->selectionTimeout, OwnerTimedOut, (XtPointer)req);
	}
#endif 
	AddHandler(req->ctx->dpy, window, widget, 
			(EventMask) PropertyChangeMask, 
	       		HandlePropertyGone, (XtPointer)req);
/* now send client INCREMENT property */
	size = BYTELENGTH(length,format);
	value = ((char*)&size) + sizeof(long) - 4;
	XChangeProperty(req->ctx->dpy, window, req->property,
			req->ctx->prop_list->incremental_atom,
			32, PropModeReplace, (unsigned char *)value, 1);
}

static Boolean GetConversion(ctx, event, target, property, widget, incremental)
Select ctx;			/* logical owner */
XSelectionRequestEvent* event;
Atom target;
Atom property;			/* requestor's property */
Widget widget;			/* physical owner (receives events) */
Boolean *incremental;
{
    XtPointer value = NULL;
    unsigned long length;
    int format;
    Atom targetType;
    Request req = XtNew(RequestRec);
    Boolean timestamp_target = (target == ctx->prop_list->timestamp_atom);

    req->ctx = ctx;
    req->event = *event;

    if (timestamp_target) {
	value = XtMalloc(4);
	if (sizeof(long) == 4)
	    *(long*)value = ctx->time;
	else {
	    /* NOTREACHED */ /* sizeof(long)!=4 */
	    long temp = ctx->time;
	    bcopy( ((char*)&temp)+sizeof(long)-4, value, 4);
	}
	targetType = XA_INTEGER;
	length = 1;
	format = 32;
    }
    else {
	if (ctx->incremental == TRUE) {
	     unsigned long size = MAX_SELECTION_INCR(ctx->dpy);
	     if ((*ctx->convert)(ctx->widget, &event->selection, &target,
				&targetType, &value, &length, &format,
				&size, ctx->owner_closure, (XtRequestId*)&req)
		     == FALSE) {
		 XtFree((XtPointer)req);
		 return(FALSE);
	     }
	     PrepareIncremental(req, widget, event->requestor, property,
				target, targetType, value, length, format);
	     *incremental = True;
	     return(TRUE);
	}
	ctx->req = req;
	if ((*ctx->convert)(ctx->widget, &event->selection, &target,
			    &targetType, &value, &length, &format) == FALSE) {
	    XtFree((XtPointer)req);
	    ctx->req = NULL;
	    return(FALSE);
	}
	ctx->req = NULL;
    }
    if (BYTELENGTH(length,format) <= MAX_SELECTION_INCR(ctx->dpy)) {
	if (! timestamp_target && ctx->notify != NULL) {
		  req->target = target;
		  req->property = property;
		  req->widget = widget;
		  req->requestor = event->requestor;
		  req->allSent = TRUE;
#ifndef DEBUG_WO_TIMERS
		  {
		  XtAppContext app = XtWidgetToApplicationContext(req->widget);
		  req->timeout = XtAppAddTimeOut(app,
			 app->selectionTimeout, OwnerTimedOut, (XtPointer)req);
		  }
#endif 
	          AddHandler(ctx->dpy, event->requestor,
			     widget, (EventMask) PropertyChangeMask, 
			     HandlePropertyGone, (XtPointer)req);
        }
	XChangeProperty(ctx->dpy, event->requestor, property, 
			targetType, format, PropModeReplace,
			(unsigned char *)value, (int)length);
	/* free storage for client if no notify proc */
	if (timestamp_target || ctx->notify == NULL) {
	    XtFree((XtPointer)value);
	    XtFree((XtPointer)req);
	}
	*incremental = FALSE;
    } else {
	 PrepareIncremental(req, widget, event->requestor, property,
			    target, targetType, value, length, format);
	 *incremental = True;
    }
    return(TRUE);
}

/*ARGSUSED*/
static void HandleSelectionEvents(widget, closure, event)
Widget widget;
XtPointer closure;
XEvent *event;
{
    Select eventCtx, ctx;
    XSelectionEvent ev;
    Boolean incremental;
    Atom target;
    int count;
    Boolean writeback = FALSE;

    ctx = (Select) closure;
    switch (event->type) {
      case SelectionClear:
	eventCtx = FindCtx(event->xselectionclear.display,
			   event->xselectionclear.selection);
	/* if this event is not for the selection we registered for,
	 * don't do anything */
	if (eventCtx != ctx)
	    break;
	(void) LoseSelection(ctx, widget, event->xselectionclear.selection,
			event->xselectionclear.time);
	break;
      case SelectionRequest:
	eventCtx = FindCtx(event->xselectionrequest.display,
			   event->xselectionrequest.selection);
	/* if this event is not for the selection we registered for,
	 * don't do anything */
	if (eventCtx != ctx)
	    break;
	ev.type = SelectionNotify;
	ev.display = event->xselectionrequest.display;
	ev.requestor = event->xselectionrequest.requestor;
	ev.selection = event->xselectionrequest.selection;
	ev.time = event->xselectionrequest.time;
	ev.target = event->xselectionrequest.target;
	if (event->xselectionrequest.property == None) /* obsolete requestor */
	   event->xselectionrequest.property = event->xselectionrequest.target;
	if (!(ctx->widget)
	   || ((event->xselectionrequest.time != CurrentTime)
	        && (event->xselectionrequest.time < ctx->time)))
	    ev.property = None;
         else {
	   if (ev.target == ctx->prop_list->indirect_atom) {
	      IndirectPair *p;
	      int format;
	      unsigned long bytesafter, length;
	      unsigned char *value;
	      ev.property = event->xselectionrequest.property;
	      (void) XGetWindowProperty(ev.display, ev.requestor,
			event->xselectionrequest.property, 0L, 1000000,
			False,(Atom)AnyPropertyType, &target, &format, &length,
			&bytesafter, &value);
	      count = BYTELENGTH(length, format) / sizeof(IndirectPair);
	      for (p = (IndirectPair *)value; count; p++, count--) {
		  if (!GetConversion(ctx, (XSelectionRequestEvent*)event,
				     p->target, p->property, widget,
				     &incremental)) {

			p->property = None;
			writeback = TRUE;
		  }
	      }
	      if (writeback) 
		XChangeProperty(ev.display, ev.requestor, 
			event->xselectionrequest.property, target,
			format, PropModeReplace, value, (int)length);
	      XFree((char *)value);
	  } else /* not multiple */ {
	       if (GetConversion(ctx, (XSelectionRequestEvent*)event,
				 event->xselectionrequest.target,
				 event->xselectionrequest.property,
				 widget, &incremental))
		   ev.property = event->xselectionrequest.property;
	       else
		   ev.property = None;
	   }
      }
      (void) XSendEvent(ctx->dpy, ev.requestor, False, (unsigned long)NULL,
		   (XEvent *) &ev);
      break;
    }
}

static Boolean OwnSelection(widget, selection, time, convert, lose, notify,
			    cancel, closure, incremental)
Widget widget;
Atom selection;
Time time;
XtConvertSelectionProc convert;
XtLoseSelectionProc lose;
XtSelectionDoneProc notify;
XtCancelConvertSelectionProc cancel;
XtPointer closure;
Boolean incremental;
{
    Select ctx;
    SelectRec oldctx;
    Window window;
    Boolean old_context = FALSE;

    ctx = FindCtx(XtDisplay(widget), selection);
    if (ctx->widget != widget || ctx->time != time)
    {
	window = XtWindow(widget);
        XSetSelectionOwner(ctx->dpy, selection, window, time);
        if (XGetSelectionOwner(ctx->dpy, selection) != window)
	    return FALSE;
    	if (ctx->widget != widget)
 	{
	    XtAddEventHandler(widget, (EventMask)NULL, TRUE,
			      HandleSelectionEvents, (XtPointer)ctx);
	    XtAddCallback(widget, XtNdestroyCallback,
			  WidgetDestroyed, (XtPointer)ctx);

	    if (ctx->widget) {
		oldctx = *ctx;
		old_context = TRUE;
	    }
	}
	ctx->widget = widget;	/* Selection offically changes hands. */
	ctx->time = time;
    }
    ctx->convert = convert;
    ctx->loses = lose;
    ctx->notify = notify;
    ctx->owner_cancel = cancel;
    ctx->incremental = incremental;
    ctx->owner_closure = closure;

    if (old_context)
	(void) LoseSelection(&oldctx, oldctx.widget, selection, oldctx.time);

    return TRUE;
}


Boolean XtOwnSelection(widget, selection, time, convert, lose, notify)
Widget widget;
Atom selection;
Time time;
XtConvertSelectionProc convert;
XtLoseSelectionProc lose;
XtSelectionDoneProc notify;
{
    return OwnSelection(widget, selection, time, convert, lose, notify,
			(XtCancelConvertSelectionProc)NULL,
			(XtPointer)NULL, FALSE);
}


Boolean XtOwnSelectionIncremental(widget, selection, time, convert, 
				  lose, notify, cancel, closure)
Widget widget;
Atom selection;
Time time;
XtConvertSelectionIncrProc convert;
XtLoseSelectionIncrProc lose;
XtSelectionDoneIncrProc notify;
XtCancelConvertSelectionProc cancel;
XtPointer closure;
{
    return OwnSelection(widget, selection, time, 
			(XtConvertSelectionProc)convert, 
			(XtLoseSelectionProc)lose,
			(XtSelectionDoneProc)notify,
			cancel, closure, TRUE);
}


void XtDisownSelection(widget, selection, time)
Widget widget;
Atom selection;
Time time;
{
    Select ctx;
    ctx = FindCtx(XtDisplay(widget), selection);
    if (LoseSelection(ctx, widget, selection, time))
	XSetSelectionOwner(XtDisplay(widget), selection, None, time);
}

/* Selection Requestor code */

static Boolean IsINCRtype(info, window, prop)
    CallBackInfo info;
    Window window;
    Atom prop;
{
    unsigned long bytesafter;
    unsigned long length;
    int format;
    Atom type;
    unsigned char *value;

    (void)XGetWindowProperty(XtDisplay(info->widget), window, prop, 0L, 0L,
			     False, info->ctx->prop_list->incremental_atom,
			     &type, &format, &length, &bytesafter, &value);

    return (type == info->ctx->prop_list->incremental_atom);
}

static void ReqCleanup(widget, closure, ev)
Widget widget;
XtPointer closure;
XEvent *ev;
{
    CallBackInfo info = (CallBackInfo)closure;
    unsigned long bytesafter, length;
    char *value;
    int format;
    Atom target;
    static void HandleGetIncrement();

    if (ev->type == SelectionNotify) {
	XSelectionEvent *event = (XSelectionEvent *) ev;
	if (!MATCH_SELECT(event, info)) return; /* not really for us */
         XtRemoveEventHandler(widget, (EventMask) NULL, TRUE,
			   ReqCleanup, (XtPointer) info );
	if (IsINCRtype(info, XtWindow(widget), event->property)
#ifndef NO_DRAFT_ICCCM_COMPATIBILITY
	    || event->target == info->ctx->prop_list->incremental_atom
#endif
	    ) {
	    info->proc = HandleGetIncrement;
	    XtAddEventHandler(info->widget, (EventMask) PropertyChangeMask, 
			      FALSE, ReqCleanup, (XtPointer) info);
	} else {
	   if (event->property != None) 
		XDeleteProperty(event->display, XtWindow(widget),
				event->property);
           FreeSelectionProperty(XtDisplay(widget), info->property);
	   XtFree((XtPointer)info->req_closure);
	   XtFree((XtPointer)info->target);
           XtFree((XtPointer) info);
	}
    } else if ((ev->type == PropertyNotify) &&
		(ev->xproperty.state == PropertyNewValue) &&
	        (ev->xproperty.atom == info->property)) {
	XPropertyEvent *event = (XPropertyEvent *) ev;
        (void) XGetWindowProperty(event->display, XtWindow(widget), 
			   event->atom, 0L, 1000000, True, AnyPropertyType,
			   &target, &format, &length, &bytesafter, 
			   (unsigned char **) &value);
	XFree(value);
	if (length == 0) {
           XtRemoveEventHandler(widget, (EventMask) PropertyChangeMask, FALSE,
			   ReqCleanup, (XtPointer) info );
           FreeSelectionProperty(XtDisplay(widget), info->property);
	   XtFree((XtPointer)info->req_closure);
	   XtFree((XtPointer)info->target);
           XtFree((XtPointer) info);
	}
    }
}

/* ARGSUSED */
static void ReqTimedOut(closure, id)
XtPointer closure;
XtIntervalId   *id;
{
    XtPointer value = NULL;
    int length = 0;
    int format = 8;
    Atom resulttype = XT_CONVERT_FAIL;
    CallBackInfo info = (CallBackInfo)closure;
    unsigned long bytesafter;
    unsigned long proplength;
    Atom type;
    IndirectPair *pairs;
    XtPointer *c;
    static void HandleSelectionReplies();
    if (*info->target == info->ctx->prop_list->indirect_atom) {
        (void) XGetWindowProperty(XtDisplay(info->widget), 
			   XtWindow(info->widget), info->property, 0L,
			   10000000, True, AnyPropertyType, &type, &format,
			   &proplength, &bytesafter, (unsigned char **) &pairs);
       XFree((char*)pairs);
       for (proplength = proplength / IndirectPairWordSize, c = info->req_closure;
	           proplength; proplength--, c++) 
	    (*info->callback)(info->widget, *c, 
	    	&info->ctx->selection, &resulttype, value, &length, &format);
    } else {
	(*info->callback)(info->widget, *info->req_closure, 
	    &info->ctx->selection, &resulttype, value, &length, &format);
    }

    /* change event handlers for straggler events */
    if (info->proc == (XtEventHandler)HandleSelectionReplies) {
        XtRemoveEventHandler(info->widget, (EventMask) NULL, 
			TRUE, info->proc, (XtPointer) info);
	XtAddEventHandler(info->widget, (EventMask) NULL, TRUE,
		ReqCleanup, (XtPointer) info);
    } else {
        XtRemoveEventHandler(info->widget,(EventMask) PropertyChangeMask, 
			FALSE, info->proc, (XtPointer) info);
	XtAddEventHandler(info->widget, (EventMask) PropertyChangeMask, 
		FALSE, ReqCleanup, (XtPointer) info);
    }

}

static void HandleGetIncrement(widget, closure, ev)
Widget widget;
XtPointer closure;
XEvent *ev;
{
    XPropertyEvent *event = (XPropertyEvent *) ev;
    CallBackInfo info = (CallBackInfo) closure;
    Select ctx = info->ctx; 
    char *value;
    unsigned long bytesafter;
    unsigned long length;
    int bad;
    static void HandleSelectionReplies();

    if ((event->state != PropertyNewValue) || (event->atom != info->property))
	 return;

    bad = XGetWindowProperty(event->display, XtWindow(widget),
			     event->atom, 0L,
			     10000000, True, AnyPropertyType, &info->type, 
			     &info->format, &length, &bytesafter, 
			     (unsigned char **) &value);
    if (bad) 
      return;
#ifndef DEBUG_WO_TIMERS
    XtRemoveTimeOut(info->timeout); 
#endif 
    if (length == 0) {
       (*info->callback)(widget, *info->req_closure, &ctx->selection, 
			  &info->type, 
			  (info->offset == 0 ? value : info->value), 
			  &info->offset, &info->format);
       if (info->offset) XFree(value);
       XtRemoveEventHandler(widget, (EventMask) PropertyChangeMask, FALSE, 
		HandleGetIncrement, (XtPointer) info);
       FreeSelectionProperty(event->display, info->property);
       XtFree((XtPointer)info->req_closure);
       XtFree((XtPointer)info->target);
       XtFree((XtPointer) info);
    } else { /* add increment to collection */
      if (info->incremental) {
        (*info->callback)(widget, *info->req_closure, &ctx->selection, 
			  &info->type, value, &length, &info->format);
      } else {
          if ((BYTELENGTH(length,info->format)+info->offset) 
			> info->bytelength) {
  	    info->value = (char *)XtRealloc((XtPointer) info->value, 
					 (unsigned) (info->bytelength *= 2));
          }
          bcopy(value, &info->value[info->offset], 
		(int) BYTELENGTH(length, info->format));
          info->offset += BYTELENGTH(length, info->format);
         XFree(value);
     }
     /* reset timer */
#ifndef DEBUG_WO_TIMERS
     {
     XtAppContext app = XtWidgetToApplicationContext(info->widget);
     info->timeout = XtAppAddTimeOut(app,
			 app->selectionTimeout, ReqTimedOut, (XtPointer) info);
     }
#endif 
   }
}


static HandleNone(widget, callback, closure, selection)
Widget widget;
XtSelectionCallbackProc callback;
XtPointer closure;
Atom selection;
{
    unsigned long length = 0;
    int format = 8;
    Atom type = None;

    (*callback)(widget, closure, &selection, 
		&type, NULL, &length, &format);
}


static long IncrPropSize(widget, value, format, length)
     Widget widget;
     unsigned char* value;
     int format;
     unsigned long length;
{
    unsigned long size;
    if (format == 32
#ifndef NO_DRAFT_ICCCM_COMPATIBILITY
	/* old code was Endian-dependent; don't bother trying to fix it! */
	|| (format == 8 && length == 4)
#endif
	) {
	if (sizeof(long) == 4)
	    size = ((long*)value)[length-1]; /* %%% what order for longs? */
	else {
	    /* NOTREACHED */ /* sizeof(long)!=4 */
	    size = 0;
	    bcopy(value+4*(length-1), ((char*)&size)+sizeof(long)-4, 4);
	}
	return size;
    }
    else {
	XtAppWarningMsg( XtWidgetToApplicationContext(widget),
			"badFormat","xtGetSelectionValue","XtToolkitError",
	"Selection owner returned type INCR property with format != 32",
			(String*)NULL, (Cardinal*)NULL );
	return 0;
    }
}


static
Boolean HandleNormal(dpy, widget, property, info, closure, selection)
Display *dpy;
Widget widget;
Atom property;
CallBackInfo info;
XtPointer closure;
Atom selection;
{
    unsigned long bytesafter;
    unsigned long length;
    int format;
    Atom type;
    unsigned char *value;
    static HandleIncremental();

    (void) XGetWindowProperty(dpy, XtWindow(widget), property, 0L,
			      10000000, False, AnyPropertyType,
			      &type, &format, &length, &bytesafter, &value);

    if (type == info->ctx->prop_list->incremental_atom) {
	unsigned long size = IncrPropSize(widget, value, format, length);
	XFree((char *)value);
	HandleIncremental(dpy, widget, property, info, size);
	return FALSE;
    }

    XDeleteProperty(dpy, XtWindow(widget), property);
    (*info->callback)(widget, closure, &selection, 
			  &type, value, &length, &format);

    if (info->incremental) {
	/* let requestor know the whole thing has been received */
	value = (unsigned char*)XtMalloc((unsigned)1);
	length = 0;
	(*info->callback)(widget, closure, &selection,
			  &type, value, &length, &format);
    }
    return TRUE;
}

static HandleIncremental(dpy, widget, property, info, size)
Display *dpy;
Widget widget;
Atom property;
CallBackInfo info;
unsigned long size;
{
    XtAddEventHandler(widget, (EventMask) PropertyChangeMask, FALSE,
	  	HandleGetIncrement, (XtPointer) info);
	
    /* now start the transfer */
    XDeleteProperty(dpy, XtWindow(widget), property);
    XFlush(dpy);

    info->bytelength = size;
    info->value = (char *) XtMalloc((unsigned) info->bytelength);
    info->offset = 0;

    /* reset the timer */
    info->proc = HandleGetIncrement;
#ifndef DEBUG_WO_TIMERS
    {
    XtAppContext app = XtWidgetToApplicationContext(info->widget);
    info->timeout = XtAppAddTimeOut(app,
			 app->selectionTimeout, ReqTimedOut, (XtPointer) info);
    }
#endif 
}

static unsigned long GetSizeOfIncr(widget, ctx, property)
     Widget widget;
     Select ctx;
     Atom property;
{
    /* assert( prop type is INCR ) */
    unsigned long bytesafter;
    unsigned long length;
    int format;
    Atom type;
    unsigned char *value;
    unsigned long size;

    (void)XGetWindowProperty( XtDisplay(widget), XtWindow(widget), property,
			      0L, 10000000, False, ctx->prop_list->incremental_atom,
			      &type, &format, &length, &bytesafter, &value);

    size = IncrPropSize(widget, value, format, length);
    XFree((char *)value);
    return size;
}


static void HandleSelectionReplies(widget, closure, ev)
Widget widget;
XtPointer closure;
XEvent *ev;
{
    XSelectionEvent *event = (XSelectionEvent *) ev;
    Display *dpy = event->display;
    CallBackInfo info = (CallBackInfo) closure;
    Select ctx = info->ctx;
    IndirectPair *pairs, *p;
    unsigned long bytesafter;
    unsigned long length;
    int format;
    Atom type;
    XtPointer *c;
    Atom *t;

    if (event->type != SelectionNotify) return;
    if (!MATCH_SELECT(event, info)) return; /* not really for us */
#ifndef DEBUG_WO_TIMERS
    XtRemoveTimeOut(info->timeout); 
#endif 
    XtRemoveEventHandler(widget, (EventMask) NULL, TRUE,
		HandleSelectionReplies, (XtPointer) info );
    if (event->target == ctx->prop_list->indirect_atom) {
        (void) XGetWindowProperty(dpy, XtWindow(widget), info->property, 0L,
			   10000000, True, AnyPropertyType, &type, &format,
			   &length, &bytesafter, (unsigned char **) &pairs);
       for (length = length / IndirectPairWordSize, p = pairs, 
		   c = info->req_closure, t = info->target+1;
	           length; length--, p++, c++, t++) {
	    if ((event->property == None) || (format != 32) || 
		 (p->property == None)) {
		HandleNone(widget, info->callback, *c, event->selection);
		if (p->property != None)
                    FreeSelectionProperty(XtDisplay(widget), p->property);
#ifndef NO_DRAFT_ICCCM_COMPATIBILITY
	    } else if (p->target == ctx->prop_list->incremental_atom) {
		CallBackInfo newinfo = XtNew(CallBackInfoRec);
		newinfo->callback = info->callback;
		newinfo->req_closure = (XtPointer *)XtNew(XtPointer);
		*newinfo->req_closure = *c;
		newinfo->property = p->property;
		newinfo->widget = info->widget;
		newinfo->time = info->time;
		newinfo->target = (Atom *)XtNew(Atom);
		*newinfo->target = *t;
		newinfo->ctx = info->ctx;
		newinfo->incremental = info->incremental;
		HandleIncremental(dpy, widget, p->property, newinfo,
				  GetSizeOfIncr(widget, ctx, p->property)
				  );
#endif
	    } else {
		if (HandleNormal(dpy, widget, p->property, info, *c, 
				 event->selection)) {
		    FreeSelectionProperty(XtDisplay(widget), p->property);
		}
	    }
       }
       XFree((char*)pairs);
       FreeSelectionProperty(dpy, info->property);
       XtFree((XtPointer)info->req_closure); 
       XtFree((XtPointer)info->target); 
       XtFree((XtPointer) info);
    } else if (event->property == None) {
	HandleNone(widget, info->callback, *info->req_closure, event->selection);
        FreeSelectionProperty(XtDisplay(widget), info->property);
        XtFree((XtPointer)info->req_closure);
        XtFree((XtPointer)info->target); 
        XtFree((XtPointer) info);
#ifndef NO_DRAFT_ICCCM_COMPATIBILITY
    } else if (event->target == ctx->prop_list->incremental_atom) {
	HandleIncremental(dpy, widget, event->property, info, 0);
#endif
    } else {
	if (HandleNormal(dpy, widget, event->property, info, 
			 *info->req_closure, event->selection)) {
	    FreeSelectionProperty(XtDisplay(widget), info->property);
	    XtFree((XtPointer)info->req_closure);
	    XtFree((XtPointer)info->target); 
	    XtFree((XtPointer) info);
	}
    }
}

static DoLocalTransfer(req, selection, target, widget,
		       callback, closure, incremental)
Request req;
Atom selection;
Atom target;
Widget widget;		/* The widget requesting the value. */
XtSelectionCallbackProc callback;
XtPointer closure;	/* the closure for the callback, not the conversion */
Boolean incremental;
{
    Select ctx = req->ctx;
    XtPointer value = NULL, temp, total = NULL;
    unsigned int length;
    int format;
    Atom resulttype;
    int totallength = 0;

	req->event.target = target;

	if (ctx->incremental) {
	   unsigned long size = MAX_SELECTION_INCR(ctx->dpy);
	   if (!(*ctx->convert)(ctx->widget, &selection, &target,
			    &resulttype, &value, &length, &format,
			    &size, ctx->owner_closure, (XtRequestId*)&req)) {
	       HandleNone(widget, callback, closure, selection);
	   }
	   else {
		if (incremental) {
		  Boolean allSent = FALSE;
	          while (!allSent) {
	    	      if (ctx->notify && (value != NULL)) {
              	        int bytelength = BYTELENGTH(length,format);
	                /* both sides think they own this storage */
	                temp = XtMalloc((unsigned)bytelength);
	                bcopy(value, temp, bytelength);
	                value = temp;
	              }
		      /* use care; older clients were never warned that
		       * they must return a value even if length==0
		       */
		     if (value == NULL) value = XtMalloc((unsigned)1);
		     (*callback)(widget, closure, &selection, 
			&resulttype, value, &length, &format);
		     if (length) {
			 /* should owner be notified on end-of-piece?
			  * Spec is unclear, but non-local transfers don't.
			  */
			 (*ctx->convert)(ctx->widget, &selection, &target,
					 &resulttype, &value, &length, &format,
					 &size, ctx->owner_closure,
					 (XtRequestId*)&req);
		     }
		     else allSent = TRUE;
		  }
	        } else {
	          while (length) {
		    int bytelength = BYTELENGTH(length, format);
		    total = XtRealloc(total, 
			    (unsigned) (totallength += bytelength));
		    bcopy(value, &total[totallength-bytelength], bytelength);
		    (*ctx->convert)(ctx->widget, &selection, &target, 
			    &resulttype, &value, &length, &format,
			    &size, ctx->owner_closure, (XtRequestId*)&req);
		  }
		  if (total == NULL) total = XtMalloc(1);
		  totallength = NUMELEM(totallength, format); 
		  (*callback)(widget, closure, &selection, &resulttype, 
		    total,  &totallength, &format);
	      }
	      if (ctx->notify) 
		  (*ctx->notify)(ctx->widget, &selection, &target, 
				 (XtRequestId*)&req, ctx->owner_closure);
	      else XtFree(value);
	  }
	} else { /* not incremental owner */
	  if (!(*ctx->convert)(ctx->widget, &selection, &target, 
			     &resulttype, &value, &length, &format)) {
	    HandleNone(widget, callback, closure, selection);
	  } else {
	      if (ctx->notify && (value != NULL)) {
                int bytelength = BYTELENGTH(length,format);
	        /* both sides think they own this storage; better copy */
	        temp = XtMalloc((unsigned)bytelength);
	        bcopy(value, temp, bytelength);
	        value = temp;
	      }
	      if (value == NULL) value = XtMalloc((unsigned)1);
	      (*callback)(widget, closure, &selection, &resulttype, 
			  value, &length, &format);
	      if (ctx->notify)
	         (*ctx->notify)(ctx->widget, &selection, &target);
	  }
      }
}

static void GetSelectionValue(widget, selection, target, callback,
			      closure, time, incremental)
Widget widget;
Atom selection;
Atom target;
XtSelectionCallbackProc callback;
XtPointer closure;
Time time;
Boolean incremental;
{
    Select ctx;
    CallBackInfo info;

    ctx = FindCtx(XtDisplay(widget), selection);
    if (ctx->widget) {
	RequestRec req;
	ctx->req = &req;
	req.ctx = ctx;
	req.event.type = 0;
	req.event.requestor = XtWindow(widget);
	req.event.time = time;
	DoLocalTransfer(&req, selection, target, widget,
			callback, closure, incremental);
	ctx->req = NULL;
    }
    else {
	info = MakeInfo(ctx, callback, &closure, 1, widget,
			time, incremental);
	info->target = (Atom *)XtMalloc((unsigned) sizeof(Atom));
	 *(info->target) = target;
	RequestSelectionValue(info, selection, target);
    }
}


void XtGetSelectionValue(widget, selection, target, callback, closure, time)
Widget widget;
Atom selection;
Atom target;
XtSelectionCallbackProc callback;
XtPointer closure;
Time time;
{
    GetSelectionValue(widget, selection, target, callback,
		      closure, time, FALSE);
}


void XtGetSelectionValueIncremental(widget, selection, target, callback, 
				    closure, time)
Widget widget;
Atom selection;
Atom target;
XtSelectionCallbackProc callback;
XtPointer closure;
Time time;
{
    GetSelectionValue(widget, selection, target, callback, 
		      closure, time, TRUE);
}


static void GetSelectionValues(widget, selection, targets, count, callback, 
			       closures, time, incremental)
Widget widget;
Atom selection;
Atom *targets;
int count;
XtSelectionCallbackProc callback;
XtPointer *closures;
Time time;
Boolean incremental;
{
    Select ctx;
    CallBackInfo info;
    IndirectPair *pairs, *p;
    Atom *t;

    if (count == 0) return;
    ctx = FindCtx(XtDisplay(widget), selection);
    if (ctx->widget) {
	RequestRec req;
	ctx->req = &req;
	req.ctx = ctx;
	req.event.type = 0;
	req.event.requestor = XtWindow(widget);
	req.event.time = time;
	for (; count; count--, targets++, closures++ )
	    DoLocalTransfer(&req, selection, *targets, widget,
			    callback, *closures, incremental);
	ctx->req = NULL;
    } else {
	info = MakeInfo(ctx, callback, closures, count, widget,
			time, incremental);
	info->target = (Atom *)XtMalloc((unsigned) ((count+1) * sizeof(Atom)));
        (*info->target) = ctx->prop_list->indirect_atom;
	bcopy((char *) targets, (char *) info->target+sizeof(Atom),
	      count * sizeof(Atom));
	pairs = (IndirectPair*)XtMalloc((unsigned)(count*sizeof(IndirectPair)));
	for (p = &pairs[count-1], t = &targets[count-1];
	     p >= pairs;  p--, t-- ) {
	   p->target = *t;
	   p->property = GetSelectionProperty(XtDisplay(widget));
	}
	XChangeProperty(XtDisplay(widget), XtWindow(widget), 
			info->property, info->property,
			32, PropModeReplace, (unsigned char *) pairs, 
			count * IndirectPairWordSize);
	XtFree((XtPointer)pairs);
	RequestSelectionValue(info, selection, ctx->prop_list->indirect_atom);
    }
}


void XtGetSelectionValues(widget, selection, targets, count, callback, 
	closures, time)
Widget widget;
Atom selection;
Atom *targets;
int count;
XtSelectionCallbackProc callback;
XtPointer *closures;
Time time;
{
    GetSelectionValues(widget, selection, targets, count, callback,
		       closures, time, FALSE);
}


void XtGetSelectionValuesIncremental(widget, selection, targets, count, 
	callback, closures, time)
Widget widget;
Atom selection;
Atom *targets;
int count;
XtSelectionCallbackProc callback;
XtPointer *closures;
Time time;
{
    GetSelectionValues(widget, selection, targets, count, 
		       callback, closures, time, TRUE);
}


XSelectionRequestEvent *XtGetSelectionRequest( widget, selection, id )
    Widget widget;
    Atom selection;
    XtRequestId id;
{ 
    Request req = (Request)id;
    Select ctx;

    if (   (req == NULL
	    && ((ctx = FindCtx( XtDisplay(widget), selection )) == NULL
		|| ctx->req == NULL
		|| ctx->selection != selection
		|| ctx->widget == NULL))
	|| (req != NULL
	    && (req->ctx == NULL
		|| req->ctx->selection != selection
		|| req->ctx->widget != widget)))
    {
	String params = XtName(widget);
	Cardinal num_params = 1;
	XtAppWarningMsg( XtWidgetToApplicationContext(widget),
			 "notInConvertSelection", "xtGetSelectionRequest",
			 "XtToolkitError",
			 "XtGetSelectionRequest called for widget \"%s\" outside of ConvertSelection proc",
			 &params, &num_params
		       );
	return NULL;
    }

    if (req == NULL) {
	/* non-incremental owner; only one request can be
	 * outstanding at a time, so it's safe to keep ptr in ctx */
	req = ctx->req;
    }

    if (req->event.type == 0) {
	/* owner is local; construct the remainder of the event */
	req->event.type = SelectionRequest;
	req->event.serial = LastKnownRequestProcessed(XtDisplay(widget));
	req->event.send_event = True;
	req->event.display = XtDisplay(widget);
	req->event.owner = XtWindow(req->ctx->widget);
    /*  req->event.requestor = XtWindow(requesting_widget); */
	req->event.selection = selection;
    /*  req->event.target = requestors_target; */
	req->event.property = None; /* %%% what to do about side-effects? */
    /*  req->event.time = requestors_time; */
    }
    return &req->event;
}
