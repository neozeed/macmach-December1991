#ifndef lint
static char Xrcsid[] = "$XConsortium: Create.c,v 1.71 90/03/27 12:56:32 swick Exp $";
/* $oHeader: Create.c,v 1.5 88/09/01 11:26:22 asente Exp $ */
#endif /*lint*/

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
#include "VarargsI.h"
#include "StringDefs.h"
#include "Shell.h"
#include "ShellP.h"
#include <stdio.h>

extern void bcopy();

static void CallClassPartInit(ancestor, wc)
     WidgetClass ancestor, wc;
{
    if (ancestor->core_class.superclass != NULL) {
	CallClassPartInit(ancestor->core_class.superclass, wc);
    }
    if (ancestor->core_class.class_part_initialize != NULL) {
	(*(ancestor->core_class.class_part_initialize)) (wc);
    }
}

void XtInitializeWidgetClass(wc)
    WidgetClass wc;
{
    XtEnum inited;
    if (wc->core_class.class_inited) return;
    inited = 0x01;
    {
	WidgetClass pc;
#define LeaveIfClass(c, d) if (pc == c) { inited = d; break; }
	for (pc = wc; pc; pc = pc->core_class.superclass) {
	    LeaveIfClass(rectObjClass, 0x01 |
			 RectObjClassFlag);
	    LeaveIfClass(coreWidgetClass, 0x01 |
			 RectObjClassFlag |
			 WidgetClassFlag);
	    LeaveIfClass(compositeWidgetClass, 0x01 |
			 RectObjClassFlag |
			 WidgetClassFlag |
			 CompositeClassFlag);
	    LeaveIfClass(constraintWidgetClass, 0x01 |
			 RectObjClassFlag |
			 WidgetClassFlag |
			 CompositeClassFlag |
			 ConstraintClassFlag);
	    LeaveIfClass(shellWidgetClass, 0x01 |
			 RectObjClassFlag |
			 WidgetClassFlag |
			 CompositeClassFlag |
			 ShellClassFlag);
	    LeaveIfClass(wmShellWidgetClass, 0x01 |
			 RectObjClassFlag |
			 WidgetClassFlag |
			 CompositeClassFlag |
			 ShellClassFlag |
			 WMShellClassFlag);
	    LeaveIfClass(topLevelShellWidgetClass, 0x01 |
			 RectObjClassFlag |
			 WidgetClassFlag |
			 CompositeClassFlag |
			 ShellClassFlag |
			 WMShellClassFlag |
			 TopLevelClassFlag);
	}
#undef LeaveIfClass
    }
    if (wc->core_class.version != XtVersion &&
	wc->core_class.version != XtVersionDontCheck) {
	String param[3];
        param[0] = wc->core_class.class_name;
	if (wc->core_class.version == (11 * 1000 + 3)) { /* MIT X11R3 */
	    if (inited & ShellClassFlag) {
		Cardinal num_params=1;
		XtWarningMsg("r3versionMismatch","widget","XtToolkitError",
			     "Shell Widget class %s binary compiled for R3",
			     param,&num_params);
	    }
	}
	else {
	    Cardinal num_params=3;
	    param[1] = (String)wc->core_class.version;
	    param[2] = (String)XtVersion;
	    XtWarningMsg("versionMismatch","widget","XtToolkitError",
			 "Widget class %s version mismatch (recompilation needed):\n  widget %d vs. intrinsics %d.",
			 param,&num_params);
	    if (wc->core_class.version == (2 * 1000 + 2)) /* MIT X11R2 */ {
		Cardinal num_params=1;
		XtErrorMsg("r2versionMismatch","widget","XtToolkitError",
			   "Widget class %s must be re-compiled.",
			   param, &num_params);
	    }
	}
    }

    if ((wc->core_class.superclass != NULL) 
	    && (!(wc->core_class.superclass->core_class.class_inited)))
 	XtInitializeWidgetClass(wc->core_class.superclass);
 
    if (wc->core_class.class_initialize != NULL)
	(*(wc->core_class.class_initialize))();
    CallClassPartInit(wc, wc);
    wc->core_class.class_inited = inited;
}

static void CallInitialize (class, req_widget, new_widget, args, num_args)
    WidgetClass class;
    Widget      req_widget;
    Widget      new_widget;
    ArgList     args;
    Cardinal    num_args;
{
    if (class->core_class.superclass)
        CallInitialize (class->core_class.superclass,
	    req_widget, new_widget, args, num_args);
    if (class->core_class.initialize != NULL)
	(*class->core_class.initialize)
	    (req_widget, new_widget, args, &num_args);
    if (class->core_class.initialize_hook != NULL)
	(*class->core_class.initialize_hook) (new_widget, args, &num_args);
}

static void CallConstraintInitialize (class, req_widget, new_widget, args, num_args)
    ConstraintWidgetClass class;
    Widget	req_widget, new_widget;
    ArgList	args;
    Cardinal	num_args;
{
    if (class->core_class.superclass != constraintWidgetClass)
	CallConstraintInitialize(
	    (ConstraintWidgetClass) class->core_class.superclass,
	    req_widget, new_widget, args, num_args);
    if (class->constraint_class.initialize != NULL)
        (*class->constraint_class.initialize)
	    (req_widget, new_widget, args, &num_args);
}

static Widget _XtCreate(
	name, class, widget_class, parent, default_screen,
	args, num_args, typed_args, num_typed_args, parent_constraint_class)
    char        *name, *class;
    WidgetClass widget_class;
    Widget      parent;
    Screen*     default_screen;
    ArgList     args;		/* must be NULL if typed_args is non-NULL */
    Cardinal    num_args;
    XtTypedArgList typed_args;	/* must be NULL if args is non-NULL */
    Cardinal	num_typed_args;
    ConstraintWidgetClass parent_constraint_class;
        /* NULL if not a subclass of Constraint or if child is popup shell */
{
    register _XtOffsetList  offsetList;
    XtCallbackList          *pCallbacks;
    /* need to use strictest alignment rules possible in next two decls. */
    double                  widget_cache[100];
    double                  constraint_cache[20];
    Widget                  req_widget;
    XtPointer               req_constraints;
    Cardinal                size;
    register Widget	    widget;
    XtCacheRef		    *cache_refs;
    register int	    i;

    if (! (widget_class->core_class.class_inited))
	XtInitializeWidgetClass(widget_class);
    widget = (Widget) XtMalloc((unsigned)widget_class->core_class.widget_size);
    widget->core.self = widget;
    widget->core.parent = parent;
    widget->core.widget_class = widget_class;
    widget->core.xrm_name = StringToName((name != NULL) ? name : "");
    widget->core.being_destroyed =
	(parent != NULL ? parent->core.being_destroyed : FALSE);
    widget->core.constraints = NULL;
    if (parent_constraint_class != NULL) 
       	widget->core.constraints = 
	    (XtPointer) XtMalloc((unsigned)parent_constraint_class->
                       constraint_class.constraint_size);
    if (XtIsWidget(widget)) {
	widget->core.name = XtNewString((name != NULL) ? name : "");
        widget->core.screen = default_screen;
        widget->core.tm.translations = NULL;
    };
    if (XtIsApplicationShell(widget)) {
	ApplicationShellWidget a = (ApplicationShellWidget) widget;
	if (class != NULL) a->application.class = XtNewString(class);
	else a->application.class = widget_class->core_class.class_name;
	a->application.xrm_class = StringToClass(a->application.class);
    }

    /* fetch resources */
    cache_refs = _XtGetResources(widget, args, num_args, 
			          typed_args, &num_typed_args);

    /* Convert typed arg list to arg list */
    if (typed_args != NULL && num_typed_args > 0) {
	args = (ArgList)ALLOCATE_LOCAL(sizeof(Arg) * num_typed_args);
	if (args == NULL) _XtAllocError(NULL);
	for (i = 0; i < num_typed_args; i++) {
	    args[i].name = typed_args[i].name;
	    args[i].value = typed_args[i].value;
	}
	num_args = num_typed_args;
    }

    /* Compile any callback lists into internal form */
    for (offsetList = (_XtOffsetList)widget->core.widget_class->
				core_class.callback_private;
	 offsetList != NULL;
	 offsetList = offsetList->next) {
	 pCallbacks = (XtCallbackList *)
	     ((XtPointer)widget - offsetList->offset - 1);
	if (*pCallbacks != NULL) {
	    extern CallbackStruct* _XtCompileCallbackList();
	    *pCallbacks =
		(XtCallbackList) _XtCompileCallbackList(widget, *pCallbacks);
	}
    }
    if (cache_refs != NULL) {
	extern void _XtCallbackReleaseCacheRefs();
	XtAddCallback( widget, XtNdestroyCallback,
		       XtCallbackReleaseCacheRefList, (XtPointer)cache_refs );
    }

    size = XtClass(widget)->core_class.widget_size;
    req_widget = (Widget) XtStackAlloc(size, widget_cache);
    bcopy ((char *) widget, (char *) req_widget, (int) size);
    CallInitialize (XtClass(widget), req_widget, widget, args, num_args);

    if (typed_args != NULL) {
	while (num_typed_args-- > 0) {
	
	    /* In GetResources we may have dynamically alloc'd store to hold */
	    /* a copy of a resource which was larger then sizeof(XtArgVal). */
	    /* We must free this store now in order to prevent a memory leak */
	    /* A typed arg that has a converted value in dynamic store has a */
	    /* negated size field. */

	    if (typed_args->type != NULL && typed_args->size < 0) {
		XtFree((char*)typed_args->value);
		typed_args->size = -(typed_args->size);
		}
	    typed_args++;
	}

	DEALLOCATE_LOCAL((char*)args);
    }

    if (parent_constraint_class != NULL) {
	size = parent_constraint_class->constraint_class.constraint_size;
	req_constraints = XtStackAlloc(size, constraint_cache);
	bcopy(widget->core.constraints, (char*)req_constraints, (int) size);
	req_widget->core.constraints = req_constraints;
	CallConstraintInitialize(parent_constraint_class, req_widget, widget,
				 args, num_args);
	XtStackFree(req_constraints, constraint_cache);
    }
    XtStackFree((XtPointer)req_widget, widget_cache);
    return (widget);
}


Widget _XtCreateWidget(name, widget_class, parent, args, num_args,
		       typed_args, num_typed_args)
    String      name;
    WidgetClass widget_class;
    Widget      parent;
    ArgList     args;
    Cardinal    num_args;
    XtTypedArgList typed_args;
    Cardinal	num_typed_args;
{
    register Widget	    widget;
    ConstraintWidgetClass   cwc;
    XtWidgetProc	    insert_child;
    Screen*                 default_screen;

    if (parent == NULL) {
	XtErrorMsg("invalidParent","xtCreateWidget","XtToolkitError",
                "XtCreateWidget requires non-NULL parent",
                  (String *)NULL, (Cardinal *)NULL);
    } else if (widget_class == NULL) {
	XtAppErrorMsg(XtWidgetToApplicationContext(parent),
		"invalidClass","xtCreateWidget","XtToolkitError",
                "XtCreateWidget requires non-NULL widget class",
                  (String *)NULL, (Cardinal *)NULL);
    }
    if (!widget_class->core_class.class_inited)
	XtInitializeWidgetClass(widget_class);
    if ((widget_class->core_class.class_inited & WidgetClassFlag) == 0) {
	/* not a widget */
	if (XtIsComposite(parent)) {
	    CompositeClassExtension ext;
	    for (ext = (CompositeClassExtension)
		       ((CompositeWidgetClass)XtClass(parent))
			 ->composite_class.extension;
		 ext != NULL && ext->record_type != NULLQUARK;
		 ext = (CompositeClassExtension)ext->next_extension);
	    if (ext != NULL &&
		(ext->version != XtCompositeExtensionVersion
		 || ext->record_size != sizeof(CompositeClassExtensionRec))) {
		String params[1];
		Cardinal num_params = 1;
		params[0] = XtClass(parent)->core_class.class_name;
		XtAppWarningMsg(XtWidgetToApplicationContext(parent),
		  "invalidExtension", "xtCreateWidget", "XtToolkitError",
		  "widget class %s has invalid CompositeClassExtension record",
		  params, &num_params);
		ext = NULL;
	    }
	    if (ext == NULL || !ext->accepts_objects) {
		String params[2];
		Cardinal num_params = 2;
		params[0] = name;
		params[1] = XtName(parent);
		XtAppErrorMsg(XtWidgetToApplicationContext(parent),
			      "nonWidget","xtCreateWidget","XtToolkitError",
			      "attempt to add non-widget child \"%s\" to parent \"%s\" which supports only widgets",
			      params, &num_params);
	    }
	}
    }

    if (XtIsConstraint(parent)) {
	cwc = (ConstraintWidgetClass) parent->core.widget_class;
    } else {
	cwc = NULL;
    }
    default_screen = parent->core.screen;
    widget = _XtCreate(name, (char *)NULL, widget_class, parent,
		       default_screen, args, num_args,
		       typed_args, num_typed_args, cwc);
    if (XtIsComposite(parent)) {
        insert_child = ((CompositeWidgetClass) parent->core.widget_class)->
	    composite_class.insert_child;
    } else {
	return(widget);
    }
    if (insert_child == NULL) {
	XtAppErrorMsg(XtWidgetToApplicationContext(parent),
		"nullProc","insertChild","XtToolkitError",
                "NULL insert_child procedure",
                  (String *)NULL, (Cardinal *)NULL);
    } else {
	(*insert_child) (widget);
    }
    return (widget);
}

Widget XtCreateWidget(name, widget_class, parent, args, num_args)
    String	name;
    WidgetClass widget_class;
    Widget   	parent;
    ArgList 	args;
    Cardinal    num_args;
{
    return(_XtCreateWidget(name, widget_class, parent, args, num_args,
			   (XtTypedArgList)NULL, (Cardinal)0));
}


Widget XtCreateManagedWidget(name, widget_class, parent, args, num_args)
    String      name;
    WidgetClass widget_class;
    Widget      parent;
    ArgList     args;
    Cardinal    num_args;
{
    register Widget	    widget;

    widget = XtCreateWidget(name, widget_class, parent, args, num_args);
    XtManageChild(widget);
    return widget;
}

/*ARGSUSED*/
static void RemovePopupFromParent(widget,closure,call_data)
    Widget  widget;
    XtPointer closure;
    XtPointer call_data;
{
    int i;
    Boolean found = FALSE;
    register Widget parent;
    parent = widget->core.parent;
    if (parent == NULL || parent->core.num_popups == 0)
        XtAppErrorMsg(XtWidgetToApplicationContext(widget),
		"invalidParameter","removePopupFromParent","XtToolkitError",
                "RemovePopupFromParent requires non-NULL popuplist",
                  (String *)NULL, (Cardinal *)NULL);

    for (i=0; i<parent->core.num_popups; i++)
        if (parent->core.popup_list[i] == widget){
            found = TRUE; break;
        }
    if (found == FALSE) {
        XtAppWarningMsg(XtWidgetToApplicationContext(widget),
		  "invalidWidget","removePopupFromParent","XtToolkitError",
                  "RemovePopupFromParent, widget not on parent list",
                   (String *)NULL, (Cardinal *)NULL);
        return;
    }
    if (parent->core.being_destroyed) {
	return;
	/* don't update parent's popup_list, as we won't then be able to find
	 * this child for Phase2Destroy.  This also allows for the possibility
	 * that a destroy callback higher up in the hierarchy may care to
	 * know that this popup child once existed.
	 */
    }
    parent->core.num_popups--;
    for (/*i=i*/; i<parent->core.num_popups; i++)
        parent->core.popup_list[i]= parent->core.popup_list[i+1];

}

Widget _XtCreatePopupShell(name, widget_class, parent, args, num_args,
			   typed_args, num_typed_args)
    String      name;
    WidgetClass widget_class;
    Widget      parent;
    ArgList     args;
    Cardinal    num_args;
    XtTypedArgList      typed_args;
    Cardinal            num_typed_args;
{
    register Widget widget;
    Screen* default_screen;

    if (parent == NULL) {
	XtErrorMsg("invalidParent","xtCreatePopupShell","XtToolkitError",
                "XtCreatePopupShell requires non-NULL parent",
                  (String *)NULL, (Cardinal *)NULL);
    } else if (widget_class == NULL) {
	XtAppErrorMsg(XtWidgetToApplicationContext(parent),
		"invalidClass","xtCreatePopupShell","XtToolkitError",
                "XtCreatePopupShell requires non-NULL widget class",
                  (String *)NULL, (Cardinal *)NULL);
    }
    default_screen = parent->core.screen;
    widget = _XtCreate(
		       name, (char *)NULL, widget_class, parent,
		       default_screen, args, num_args, typed_args,
		       num_typed_args, (ConstraintWidgetClass)NULL);

    parent->core.popup_list =
	(WidgetList) XtRealloc((char*) parent->core.popup_list,
               (unsigned) (parent->core.num_popups+1) * sizeof(Widget));
    parent->core.popup_list[parent->core.num_popups++] = widget;
    XtAddCallback(
       widget,XtNdestroyCallback,RemovePopupFromParent, (XtPointer)NULL);
    return(widget);
}


Widget XtCreatePopupShell(name, widget_class, parent, args, num_args)
    String      name;
    WidgetClass widget_class;
    Widget      parent;
    ArgList     args;
    Cardinal    num_args;
{
    return _XtCreatePopupShell(name, widget_class, parent, args, num_args,
			       (XtTypedArgList)NULL, (Cardinal)0);
}


Widget _XtAppCreateShell(name, class, widget_class, display, args, num_args,
			 typed_args, num_typed_args)
    String      name, class;
    WidgetClass widget_class;
    Display*    display;
    ArgList     args;
    Cardinal    num_args;
    XtTypedArgList typed_args;
    Cardinal	num_typed_args;
{
    if (widget_class == NULL) {
	XtAppErrorMsg(XtDisplayToApplicationContext(display),
	       "invalidClass","xtAppCreateShell","XtToolkitError",
               "XtAppCreateShell requires non-NULL widget class",
                 (String *)NULL, (Cardinal *)NULL);
    }

    if (name == NULL)
	name = XrmNameToString(_XtGetPerDisplay(display)->name);

    return _XtCreate(name, class, widget_class, (Widget)NULL,
	    (Screen*)DefaultScreenOfDisplay(display),
	    args, num_args, typed_args, num_typed_args,
	    (ConstraintWidgetClass) NULL);
}

Widget XtAppCreateShell(name, class, widget_class, display, args, num_args)
    String              name, class;
    WidgetClass         widget_class;
    Display             *display;
    ArgList             args;
    Cardinal            num_args;
{
    return _XtAppCreateShell(name, class, widget_class, display, args, 
			     num_args, (XtTypedArgList)NULL, (Cardinal)0);
}

/* ARGSUSED */
Widget XtCreateApplicationShell(name, widget_class, args, num_args)
    String      name;		/* unused in R3 and later */
    WidgetClass widget_class;
    ArgList     args;
    Cardinal    num_args;
{
    Display *dpy = _XtDefaultAppContext()->list[0];
    XrmClass class = _XtGetPerDisplay(dpy)->class;

    return _XtAppCreateShell((String)NULL, XrmQuarkToString((XrmQuark)class),
			     widget_class, dpy, args, num_args,
			     (XtTypedArgList)NULL, (Cardinal)0);
}

