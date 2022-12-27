#ifndef lint
static char Xrcsid[] = "$XConsortium: Destroy.c,v 1.24 90/03/27 11:07:16 swick Exp $";
/* $oHeader: Destroy.c,v 1.3 88/09/01 11:27:27 asente Exp $ */
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

static void Recursive(widget, proc)
    Widget       widget;
    XtWidgetProc proc;
{
    register int    i;
    CompositePart   *cwp;

    /* Recurse down normal children */
    if (XtIsComposite(widget)) {
	cwp = &(((CompositeWidget) widget)->composite);
	for (i = 0; i < cwp->num_children; i++) {
	    Recursive(cwp->children[i], proc);
	}
    }

    /* Recurse down popup children */
    if (XtIsWidget(widget)) {
	for (i = 0; i < widget->core.num_popups; i++) {
	    Recursive(widget->core.popup_list[i], proc);
	}
    }

    /* Finally, apply procedure to this widget */
    (*proc) (widget);  
} /* Recursive */

static void Phase1Destroy (widget)
    Widget    widget;
{
    widget->core.being_destroyed = TRUE;
} /* Phase1Destroy */

static void Phase2Callbacks(widget)
    Widget    widget;
{
    extern CallbackList* _XtCallbackList();
    if (widget->core.destroy_callbacks != NULL) {
	_XtCallCallbacks(
	      _XtCallbackList((CallbackStruct*)widget->core.destroy_callbacks),
	      (XtPointer) NULL);
    }
} /* Phase2Callbacks */

static void Phase2Destroy(widget)
    register Widget widget;
{
    register WidgetClass	    class;
    register ConstraintWidgetClass  cwClass;

    /* Call constraint destroy procedures */
    if (widget->core.parent != NULL && widget->core.constraints != NULL) {
	cwClass = (ConstraintWidgetClass)widget->core.parent->core.widget_class;
	for (;;) {
	    if (cwClass->constraint_class.destroy != NULL)
		(*(cwClass->constraint_class.destroy)) (widget);
            if (cwClass == (ConstraintWidgetClass)constraintWidgetClass) break;
            cwClass = (ConstraintWidgetClass) cwClass->core_class.superclass;
	}
    }

    /* Call widget destroy procedures */
    for (class = widget->core.widget_class;
	 class != NULL; 
	 class = class->core_class.superclass) {
	if ((class->core_class.destroy) != NULL)
	    (*(class->core_class.destroy))(widget);
    }
} /* Phase2Destroy */

/*ARGSUSED*/
static void XtPhase2Destroy (widget, closure, call_data)
    register Widget widget;
    XtPointer	    closure;
    XtPointer	    call_data;
{
    Display	    *display;
    Window	    window;
    Widget          parent;
    CallbackList    *oldDestroyList = _XtDestroyList;
    CallbackList    newDestroyList = NULL;
    XtAppContext    app = XtWidgetToApplicationContext(widget);
    Boolean	    outerInPhase2Destroy = app->in_phase2_destroy;

    _XtDestroyList = &newDestroyList;

    parent = widget->core.parent;

    if (parent != NULL && XtIsComposite(parent)) {
	XtWidgetProc delete_child =
	    ((CompositeWidgetClass) parent->core.widget_class)->
		composite_class.delete_child;
        if (XtIsRectObj(widget)) {
       	    XtUnmanageChild(widget);
        }
	if (delete_child == NULL) {
	    String param = parent->core.widget_class->core_class.class_name;
	    Cardinal num_params = 1;
	    XtAppWarningMsg(XtWidgetToApplicationContext(widget),
		"invalidProcedure","deleteChild","XtToolkitError",
		"null delete_child procedure for class %s in XtDestroy",
		&param, &num_params);
	} else {
	    (*delete_child) (widget);
	}
    }

    /* widget is freed in Phase2Destroy, so retrieve window now.
     * Shells destroy their own windows, to prevent window leaks in
     * popups; this test is practical only when XtIsShell() is cheap.
     */
    if (XtIsShell(widget) || !XtIsWidget(widget)) {
	window = 0;
#ifdef lint
	display = 0;
#endif
    }
    else {
	display = XtDisplay(widget);
	window = widget->core.window;
    }

    Recursive(widget, Phase2Callbacks);
    while (newDestroyList != NULL) {
	CallbackList newerList = NULL;
	_XtDestroyList = &newerList;
	_XtCallCallbacks(&newDestroyList, (XtPointer)NULL);
	_XtRemoveAllCallbacks(&newDestroyList);
	newDestroyList = newerList;
    }

    _XtDestroyList = oldDestroyList;

    app->in_phase2_destroy = TRUE;
    Recursive(widget, Phase2Destroy);
    app->in_phase2_destroy = outerInPhase2Destroy;

    if (window)
	XDestroyWindow(display, window);
} /* XtPhase2Destroy */


void XtDestroyWidget (widget)
    Widget    widget;
{
    CallbackList tempDestroyList = NULL;
    XtAppContext app = XtWidgetToApplicationContext(widget);

    if (widget->core.being_destroyed) return;

    if (_XtSafeToDestroy || app->in_phase2_destroy)
	_XtDestroyList = &tempDestroyList;

    Recursive(widget, Phase1Destroy);
    _XtAddCallback(widget, _XtDestroyList, XtPhase2Destroy, (XtPointer)NULL);

    if (_XtDestroyList == &tempDestroyList) {
	while (tempDestroyList != NULL) {
	    CallbackList newList = NULL;
	    _XtDestroyList = &newList;
	    _XtCallCallbacks(&tempDestroyList, (XtPointer)NULL);
	    _XtRemoveAllCallbacks(&tempDestroyList);
	    tempDestroyList = newList;
	}
	_XtDestroyList = NULL;
    }
	
} /* XtDestroyWidget */
