#ifndef lint
static char Xrcsid[] =
    "$XConsortium: GetValues.c,v 1.1 89/09/29 14:02:04 swick Exp $";
#endif /*lint*/
/*LINTLIBRARY*/

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

extern void _XtCopyToArg();
extern XrmResourceList* _XtCreateIndirectionTable();

static void GetValues(base, res, num_resources, args, num_args)
  char*			base;		/* Base address to fetch values from */
  XrmResourceList*      res;		/* The current resource values.      */
  register Cardinal	num_resources;	/* number of items in resources      */
  ArgList 		args;		/* The resource values requested     */
  Cardinal		num_args;	/* number of items in arg list       */
{
    register ArgList		arg;
    register int 		i;
    register XrmName		argName;
    register XrmResourceList*   xrmres;
    register XrmQuark		QCallback = XrmStringToQuark(XtRCallback);
    extern XtCallbackList	_XtGetCallbackList();

    /* Resource lists should be in compiled form already  */

    for (arg = args ; num_args != 0; num_args--, arg++) {
	argName = StringToName(arg->name);
	for (xrmres = res, i = 0; i < num_resources; i++, xrmres++) {
	    if (argName == (*xrmres)->xrm_name) {
		if ((*xrmres)->xrm_type == QCallback) {
		    /* hack; do this here instead of a get_values_hook
		     * because get_values_hook looses info as to
		     * whether arg->value == NULL for _XtCopyToArg.
		     * It helps performance, too...
		     */
		    XtCallbackList callback = _XtGetCallbackList(
			      base - (*xrmres)->xrm_offset - 1);
		    _XtCopyToArg(
			      (char*)&callback, &arg->value,
			      (*xrmres)->xrm_size);
		}
		else {
		    _XtCopyToArg(
			      base - (*xrmres)->xrm_offset - 1,
			      &arg->value,
			      (*xrmres)->xrm_size);
		}
		break;
	    }
	}
    }
} /* GetValues */

static void CallGetValuesHook(widget_class, w, args, num_args)
    WidgetClass	  widget_class;
    Widget	  w;
    ArgList	  args;
    Cardinal	  num_args;
{
    if (widget_class->core_class.superclass != NULL) {
	CallGetValuesHook
	    (widget_class->core_class.superclass, w, args, num_args);
    }
    if (widget_class->core_class.get_values_hook != NULL) {
	(*(widget_class->core_class.get_values_hook)) (w, args, &num_args);
    }
}



static void CallConstraintGetValuesHook(widget_class, w, args, num_args)
    WidgetClass	  widget_class;
    Widget	  w;
    ArgList	  args;
    Cardinal	  num_args;
{
    ConstraintClassExtension ext;

    if (widget_class->core_class.superclass
	->core_class.class_inited & ConstraintClassFlag) {
	CallConstraintGetValuesHook
	    (widget_class->core_class.superclass, w, args, num_args);
    }

    for (ext = (ConstraintClassExtension)((ConstraintWidgetClass)widget_class)
		 ->constraint_class.extension;
	 ext != NULL && ext->record_type != NULLQUARK;
	 ext = (ConstraintClassExtension)ext->next_extension);

    if (ext != NULL) {
	if (  ext->version == XtConstraintExtensionVersion
	      && ext->record_size == sizeof(ConstraintClassExtensionRec)) {
	    if (ext->get_values_hook != NULL)
		(*(ext->get_values_hook)) (w, args, &num_args);
	} else {
	    String params[1];
	    Cardinal num_params = 1;
	    params[0] = widget_class->core_class.class_name;
	    XtAppWarningMsg(XtWidgetToApplicationContext(w),
		 "invalidExtension", "xtCreateWidget", "XtToolkitError",
		 "widget class %s has invalid ConstraintClassExtension record",
		 params, &num_params);
	}
    }
}


void XtGetValues(w, args, num_args)
    register Widget   w;
    register ArgList  args;
    register Cardinal num_args;
{
    WidgetClass wc = XtClass(w);

    if (num_args == 0) return;
    if ((args == NULL) && (num_args != 0)) {
	XtAppErrorMsg(XtWidgetToApplicationContext(w),
		"invalidArgCount","xtGetValues","XtToolkitError",
            "Argument count > 0 on NULL argument list in XtGetValues",
              (String *)NULL, (Cardinal *)NULL);
    }
    /* Get widget values */
    GetValues((char*)w, (XrmResourceList *) wc->core_class.resources,
	wc->core_class.num_resources, args, num_args);

    /* Get constraint values if necessary */
    /* if (!XtIsShell(w) && XtIsConstraint(w->core.parent)) */
    if (w->core.constraints != NULL) {
	ConstraintWidgetClass cwc
	    = (ConstraintWidgetClass) XtClass(w->core.parent);
	GetValues((char*)w->core.constraints, 
	    (XrmResourceList *)(cwc->constraint_class.resources),
	    cwc->constraint_class.num_resources, args, num_args);
    }
    /* Notify any class procedures that we have performed get_values */
    CallGetValuesHook(wc, w, args, num_args);

    /* Notify constraint get_values if necessary */
    /* if (!XtIsShell(w) && XtIsConstraint(w->core.parent)) */
    if (w->core.constraints != NULL)
	CallConstraintGetValuesHook(XtClass(w->core.parent), w, args,num_args);
} /* XtGetValues */

void XtGetSubvalues(base, resources, num_resources, args, num_args)
  XtPointer	    base;           /* Base address to fetch values from */
  XtResourceList    resources;      /* The current resource values.      */
  Cardinal	    num_resources;  /* number of items in resources      */
  ArgList	    args;           /* The resource values requested     */
  Cardinal	    num_args;       /* number of items in arg list       */
{
    XrmResourceList* xrmres;
    xrmres = _XtCreateIndirectionTable(resources, num_resources);
    GetValues((char*)base, xrmres, num_resources, args, num_args);
    XtFree((char *)xrmres);
}
