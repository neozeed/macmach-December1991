#ifndef lint
static char Xrcsid[] = "$XConsortium: Callback.c,v 1.22 89/12/12 20:15:27 swick Exp $";
/* $oHeader: Callback.c,v 1.4 88/09/01 11:08:37 asente Exp $ */
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

/* exported internal procedures */

extern CallbackStruct* _XtCompileCallbackList();
extern CallbackList* _XtCallbackList();
extern void _XtFreeCallbackList();
extern XtCallbackList _XtGetCallbackList();


typedef struct _CallbackRec {
    CallbackList  next;
    Widget	    widget;
    XtCallbackProc  callback;
    XtPointer	    closure;
} CallbackRec;

struct _CallbackStruct {
    XtCallbackList	external_form;
    int			array_size;
    CallbackList	internal_form;
};

static CallbackStruct **FetchCallbackStruct(widget, name)
    Widget widget;
    String name;
{
    register _XtOffsetList  offsets;
    register XrmQuark       quark;

    quark = StringToQuark(name);
    for (offsets = (_XtOffsetList)widget->core.widget_class->
				 core_class.callback_private;
	 offsets != NULL;
	 offsets = offsets->next) {
	if (quark == offsets->name) {
	    return (CallbackStruct**)((char *) widget - offsets->offset - 1);
	}
    }
    return NULL;
}

static CallbackList *FetchCallbackList (widget, name, create)
    Widget  widget;
    String  name;
    Boolean create;		/* if False, may return a empty list */
{
    register CallbackStruct **callbacks = FetchCallbackStruct(widget, name);
    if (callbacks == NULL) return NULL;
    if (*callbacks == NULL) {
	if (!create) {
	    static CallbackList emptyList = NULL;
	    return &emptyList;
	}
	*callbacks = XtNew(CallbackStruct);
	(*callbacks)->external_form = NULL;
	(*callbacks)->array_size = 0;
	(*callbacks)->internal_form = NULL;
    }
    return _XtCallbackList(*callbacks);
} /* FetchCallbackList */


void _XtAddCallback(widget, callbacks, callback, closure)
    Widget		    widget;
    register CallbackList   *callbacks;
    XtCallbackProc	    callback;
    XtPointer		    closure;
{
    register CallbackRec *new;

    new = XtNew(CallbackRec);
    new->next = NULL;
    new->widget = widget;
    new->closure = closure;
    new->callback = callback; 

    for ( ; *callbacks != NULL; callbacks = &(*callbacks)->next) {};
    *callbacks = new;
} /* _XtAddCallback */

void XtAddCallback(widget, name, callback, closure)
    Widget	    widget;
    String	    name;
    XtCallbackProc  callback;
    XtPointer	    closure;
{
    CallbackList *callbacks;

    callbacks = FetchCallbackList(widget, name, True);
    if (callbacks == NULL) {
       XtAppWarningMsg(XtWidgetToApplicationContext(widget),
	       "invalidCallbackList","xtAddCallback","XtToolkitError",
              "Cannot find callback list in XtAddCallbacks",
	      (String *)NULL, (Cardinal *)NULL);
       return;
    }
    _XtAddCallback(widget, callbacks, callback, closure);
} /* XtAddCallbacks */

/* ARGSUSED */
static void AddCallbacks(widget, callbacks, newcallbacks)
    Widget		    widget;
    register CallbackList   *callbacks;
    CallbackList	    newcallbacks;
{
    for ( ; *callbacks != NULL; callbacks = &(*callbacks)->next) {};
    *callbacks = newcallbacks;
} /* AddCallback */

void XtAddCallbacks(widget, name, xtcallbacks)
    Widget	    widget;
    String	    name;
    XtCallbackList     xtcallbacks;
{
    CallbackList *callbacks;
    CallbackStruct *add_callbacks;

    callbacks = FetchCallbackList(widget, name, True);
    if (callbacks == NULL) {
       XtAppWarningMsg(XtWidgetToApplicationContext(widget),
	       "invalidCallbackList","xtAddCallback","XtToolkitError",
              "Cannot find callback list in XtAddCallbacks",
	      (String *)NULL, (Cardinal *)NULL);
       return;
    }
    add_callbacks = _XtCompileCallbackList(widget, xtcallbacks);
    AddCallbacks(widget, callbacks, add_callbacks->internal_form);
    XtFree((char*)add_callbacks);
} /* XtAddCallbacks */

void RemoveCallback (widget, callbacks, callback, closure)
    Widget		    widget;
    register CallbackList   *callbacks;
    XtCallbackProc	    callback;
    XtPointer		    closure;

{
    register CallbackList cl;

    for (cl = *callbacks; cl != NULL; (cl = *(callbacks = &cl->next))) {
	if ((cl->widget == widget) && (cl->closure == closure)
                            && (cl->callback == callback)) {
	    *callbacks = cl->next;
	    XtFree ((char *)cl);
	    return;
	}
    }
} /* RemoveCallback */

void XtRemoveCallback (widget, name, callback, closure)
    Widget	    widget;
    String	    name;
    XtCallbackProc  callback;
    XtPointer	    closure;
{

    CallbackList *callbacks;

    callbacks = FetchCallbackList(widget, name, False);
    if (callbacks == NULL) {
       XtAppWarningMsg(XtWidgetToApplicationContext(widget),
	       "invalidCallbackList","xtRemoveCallback","XtToolkitError",
              "Cannot find callback list in XtRemoveCallbacks",
	      (String *)NULL, (Cardinal *)NULL);
	return;
    }
    RemoveCallback(widget, callbacks, callback, closure);
} /* XtRemoveCallback */


void XtRemoveCallbacks (widget, name, xtcallbacks)
    Widget	    widget;
    String	    name;
    register XtCallbackList  xtcallbacks;
{

    CallbackList *callbacks;

    callbacks = FetchCallbackList(widget, name, False);
    if (callbacks == NULL) {
       XtAppWarningMsg(XtWidgetToApplicationContext(widget),
	       "invalidCallbackList","xtRemoveCallback","XtToolkitError",
              "Cannot find callback list in XtRemoveCallbacks",
	      (String *)NULL, (Cardinal *)NULL);
	return;
    }

    for (; xtcallbacks->callback != NULL; xtcallbacks++) {
	RemoveCallback(
	    widget, callbacks, xtcallbacks->callback,
	    xtcallbacks->closure);
    }
} /* XtRemoveCallbacks */


void _XtRemoveAllCallbacks (callbacks)
    CallbackList *callbacks;
{
    register CallbackList cl, next;
    
    cl = *callbacks;
    while (cl != NULL) {
	next = cl->next;
	XtFree((char *)cl);
	cl = next;
    }
    (*callbacks) = NULL;
} /* _XtRemoveAllCallbacks */

void _XtFreeCallbackList(callbacks)
    CallbackStruct *callbacks;	/* may not be NULL */
{
    if (callbacks->internal_form != NULL)
	_XtRemoveAllCallbacks(&callbacks->internal_form);
    XtFree((char*)callbacks->external_form);
    XtFree((char*)callbacks);
} /* XtFreeCallbackList */

void XtRemoveAllCallbacks(widget, name)
    Widget widget;
    String name;
{
    CallbackStruct **callbacks;

    callbacks = FetchCallbackStruct(widget, name);
    if (callbacks == NULL) {
       XtAppWarningMsg(XtWidgetToApplicationContext(widget),
	       "invalidCallbackList","xtRemoveAllCallback","XtToolkitError",
              "Cannot find callback list in XtRemoveAllCallbacks",
	      (String *)NULL, (Cardinal *)NULL);

	return;
    }
    if (*callbacks != NULL) {
	_XtFreeCallbackList(*callbacks);
	*callbacks = (CallbackStruct*)NULL;
    }
} /* XtRemoveAllCallbacks */



#define CALLBACK_CACHE_SIZE	30

void _XtCallCallbacks (callbacks, call_data)
    CallbackList *callbacks;
    XtPointer     call_data;
{
    register CallbackRec *cl;
    CallbackRec		 stack_cache [CALLBACK_CACHE_SIZE];
    CallbackList	 head;
    register Cardinal    i;

    if ((*callbacks) == NULL ) return;

    /* Get length of callback list */
    for (i = 0, cl = *callbacks; cl != NULL; i++, cl = cl->next) {};

    /* If lots of entries allocate an array, otherwise use stack_cache */
    if (i > CALLBACK_CACHE_SIZE) {
	head = (CallbackList) XtMalloc((unsigned) (i * sizeof(CallbackRec)));
    } else {
	head = stack_cache;
    }

    /* Copy callback list into array to insulate us from callbacks that change
       the callback list */
    for (i = 0, cl = *callbacks; cl != NULL; i++, cl = cl->next) {
	head[i] = *cl;
    }

    /* Execute each callback in the array */
    for (cl = head; i != 0; cl++, i--) {
	(*(cl->callback)) (cl->widget, cl->closure, call_data);
    }

    /* If temporary array allocated, free it */
    if (head != stack_cache) {
	XtFree((char *)head);
    }
} /* _XtCallCallbacks */


CallbackStruct* _XtCompileCallbackList(widget, xtcallbacks)
    Widget		    widget;
    register XtCallbackList xtcallbacks;
{
    /* Turn a public XtCallbackList into a private CallbackList */

    register CallbackList   new, *pLast;
    CallbackStruct*	    callbacks;

    if (xtcallbacks->callback == NULL) return NULL;

    callbacks = XtNew(CallbackStruct);
    callbacks->external_form = NULL;
    callbacks->array_size = 0;
    pLast = &callbacks->internal_form;
    for (; xtcallbacks->callback != NULL; xtcallbacks++) {
	new		= XtNew(CallbackRec);
	*pLast		= new;
	pLast		= &(new->next);
	new->widget     = widget;
	new->callback   = xtcallbacks->callback;
	new->closure    = xtcallbacks->closure;
    };
    *pLast = NULL;

    return callbacks;
} /* _XtCompileCallbackList */


CallbackList* _XtCallbackList(callbacks)
    CallbackStruct*	callbacks; /* must not be NULL */
{
    return &callbacks->internal_form;
}

XtCallbackList _XtGetCallbackList(list)
    char*	list;		/* is CallbackStruct** */
{
    CallbackStruct *callbackstruct = *(CallbackStruct**)list;
    int	callback_count = 0;
    register CallbackRec* callback;
    register XtCallbackRec* rec;

    if (callbackstruct == NULL || callbackstruct->internal_form == NULL) {
	static XtCallbackRec emptyList[1] = { {NULL, NULL} };
	return (XtCallbackList)emptyList;
    }

    for (callback = callbackstruct->internal_form; callback != NULL;
	 callback = callback->next, callback_count++);

    callback_count++;		/* for list terminator */
    if (callback_count > callbackstruct->array_size) {
	XtFree((char*)callbackstruct->external_form);
	callbackstruct->external_form =
	    (XtCallbackList)XtMalloc((Cardinal)callback_count * sizeof(XtCallbackRec));
	callbackstruct->array_size = callback_count;
    }
    for (callback = callbackstruct->internal_form,
	 rec = callbackstruct->external_form; callback != NULL;
	 callback = callback->next, rec++) {
	rec->callback = callback->callback;
	rec->closure = callback->closure;
    }
    rec->callback = (XtCallbackProc)NULL;
    rec->closure = NULL;
    return callbackstruct->external_form;
    /* %%% Garbage collection needed; this won't get freed
     *     except by _XtFreeCallbackList in XtRemoveAllCallbacks
     */
}


void XtCallCallbacks(widget, name, call_data)
    Widget   widget;
    String   name;
    XtPointer call_data;
{
    CallbackList *callbacks;

    callbacks = FetchCallbackList(widget, name, False);
    if (callbacks == NULL) {
       XtAppWarningMsg(XtWidgetToApplicationContext(widget),
	       "invalidCallbackList","xtCallCallback","XtToolkitError",
              "Cannot find callback list in XtCallCallbacks",
	      (String *)NULL, (Cardinal *)NULL);
	return;
    }
    _XtCallCallbacks(callbacks, call_data);
} /* XtCallCallbacks */


XtCallbackStatus XtHasCallbacks(widget, callback_name)
     Widget		widget;
     String		callback_name;
{
    CallbackList *callbacks;
    callbacks = FetchCallbackList(widget, callback_name, False);
    if (callbacks == NULL) {
	return XtCallbackNoList;
    }    
    if (*callbacks == NULL) return XtCallbackHasNone;
    return XtCallbackHasSome;
} /* XtHasCallbacks */


/*ARGSUSED*/
void XtCallCallbackList(widget, callbacks, call_data)
    Widget widget;
    XtCallbackList callbacks;
    XtPointer call_data;
{
    if (callbacks != NULL) {
	_XtCallCallbacks( &((CallbackStruct*)callbacks)->internal_form,
			  call_data
			);
    }
}
