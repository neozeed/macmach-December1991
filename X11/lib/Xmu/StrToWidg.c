#ifndef lint
static char Xrcsid[] = "$XConsortium: StrToWidg.c,v 1.3 89/09/22 12:11:59 keith Exp $";
#endif /* lint */

/* Copyright 1988 Massachusetts Institute of Technology, Cambridge, Massachusetts.
 *
 * XmuCvtStringToWidget
 *
 *   static XtConvertArgRec parentCvtArgs[] = {
 *	{XtBaseOffset, (caddr_t)XtOffset(Widget, core.parent), sizeof(Widget)},
 *   };
 *
 * matches the string against the name of the immediate children (normal
 * or popup) of the parent.  If none match, compares string to classname
 * & returns first match.  Case is significant.
 */

#include <X11/copyright.h>

#include <X11/IntrinsicP.h>
#include <X11/ObjectP.h>
#include <X11/Object.h>

#define	done(address, type) \
	{ toVal->size = sizeof(type); \
	  toVal->addr = (caddr_t) address; \
	  return; \
	}

/* ARGSUSED */
void XmuCvtStringToWidget(args, num_args, fromVal, toVal)
    XrmValuePtr args;		/* parent */
    Cardinal    *num_args;      /* 1 */
    XrmValuePtr fromVal;
    XrmValuePtr toVal;
{
    static Widget widget, *widgetP, parent;
    XrmName name = XrmStringToName(fromVal->addr);
    int i;

    if (*num_args != 1)
	XtErrorMsg("wrongParameters", "cvtStringToWidget", "xtToolkitError",
		   "StringToWidget conversion needs parent arg", NULL, 0);

    parent = *(Widget*)args[0].addr;
    /* try to match names of normal children */
    if (XtIsComposite(parent)) {
	i = ((CompositeWidget)parent)->composite.num_children;
	for (widgetP = ((CompositeWidget)parent)->composite.children;
	     i; i--, widgetP++) {
	    if ((*widgetP)->core.xrm_name == name) {
		widget = *widgetP;
		done(&widget, Widget);
	    }
	}
    }
    /* try to match names of popup children */
    i = parent->core.num_popups;
    for (widgetP = parent->core.popup_list; i; i--, widgetP++) {
	if ((*widgetP)->core.xrm_name == name) {
	    widget = *widgetP;
	    done(&widget, Widget);
	}
    }
    /* try to match classes of normal children */
    if (XtIsComposite(parent)) {
	i = ((CompositeWidget)parent)->composite.num_children;
	for (widgetP = ((CompositeWidget)parent)->composite.children;
	     i; i--, widgetP++) {
	    if ((*widgetP)->core.widget_class->core_class.xrm_class == name) {
		widget = *widgetP;
		done(&widget, Widget);
	    }
	}
    }
    /* try to match classes of popup children */
    i = parent->core.num_popups;
    for (widgetP = parent->core.popup_list; i; i--, widgetP++) {
	if ((*widgetP)->core.widget_class->core_class.xrm_class == name) {
	    widget = *widgetP;
	    done(&widget, Widget);
	}
    }
    XtStringConversionWarning(fromVal->addr, "Widget");
    toVal->addr = NULL;
    toVal->size = 0;
}
