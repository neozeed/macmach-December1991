#ifndef lint
static char Xrcsid[] =
    "$XConsortium: Resources.c,v 1.82 89/12/15 09:59:55 swick Exp $";
/* $oHeader: Resources.c,v 1.6 88/09/01 13:39:14 asente Exp $ */
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

#include <stdio.h>
#include "IntrinsicI.h"
#include "VarargsI.h"
#include "Shell.h"
#include "ShellP.h"
#include "StringDefs.h"

static XrmClass	QBoolean, QString, QCallProc, QImmediate;
static XrmName QinitialResourcesPersistent, QInitialResourcesPersistent;

#ifdef CRAY
void Cjump();
char *Cjumpp = (char *) Cjump;
void Cjump() {}
#endif

void XtCopyFromParent(widget, offset, value)
    Widget      widget;
    int		offset;
    XrmValue    *value;
{
    if (widget->core.parent == NULL) {
	XtAppWarningMsg(XtWidgetToApplicationContext(widget),
		"invalidParent","xtCopyFromParent","XtToolkitError",
                  "CopyFromParent must have non-NULL parent",
		  (String *)NULL, (Cardinal *)NULL);
        value->addr = NULL;
        return;
    }
    value->addr = ((XtPointer)widget->core.parent) + offset;
} /* XtCopyFromParent */

/*ARGSUSED*/
void XtCopyScreen(widget, offset, value)
    Widget      widget;
    int		offset;
    XrmValue    *value;
{
    value->addr = (XtPointer)(&widget->core.screen);
} /* XtCopyScreen */

/*ARGSUSED*/
void XtCopyDefaultColormap(widget, offset, value)
    Widget      widget;
    int		offset;
    XrmValue    *value;
{
    value->addr = (XtPointer)(&DefaultColormapOfScreen(XtScreenOfObject(widget)));
} /* XtCopyDefaultColormap */


/*ARGSUSED*/
void XtCopyAncestorSensitive(widget, offset, value)
    Widget      widget;
    int		offset;
    XrmValue    *value;
{
    static Boolean  sensitive;
	   Widget   parent = widget->core.parent;

    sensitive = (parent->core.ancestor_sensitive & parent->core.sensitive);
    value->addr = (XtPointer)(&sensitive);
} /* XtCopyAncestorSensitive */

/*ARGSUSED*/
void XtCopyDefaultDepth(widget, offset, value)
    Widget      widget;
    int		offset;
    XrmValue    *value;
{
    value->addr = (XtPointer)(&DefaultDepthOfScreen(XtScreenOfObject(widget)));
} /* XtCopyDefaultDepth */

/* If the alignment characteristics of your machine are right, these may be
   faster */

#ifdef UNALIGNED

void _XtCopyFromArg(src, dst, size)
    XtArgVal src;
    char* dst;
    register unsigned int size;
{
    if	    (size == sizeof(long))	*(long *)dst = (long)src;
    else if (size == sizeof(short))	*(short *)dst = (short)src;
    else if (size == sizeof(char))	*(char *)dst = (char)src;
    else if (size == sizeof(XtPointer))	*(XtPointer *)dst = (XtPointer)src;
    else if (size == sizeof(char*))	*(char **)dst = (char*)src;
    else if (size == sizeof(XtArgVal))	*(XtArgVal *)dst = src;
    else if (size > sizeof(XtArgVal))
	bcopy((char *)  src, (char *) dst, (int) size);
    else
	bcopy((char *) &src, (char *) dst, (int) size);
} /* _XtCopyFromArg */

void _XtCopyToArg(src, dst, size)
    char* src;
    XtArgVal *dst;
    register unsigned int size;
{
    if (*dst == NULL) {
	/* old GetValues semantics (storing directly into arglists) are bad,
	 * but preserve for compatibility as long as arglist contains NULL.
	 */
        if	(size == sizeof(long))	   *dst = (XtArgVal)*(long*)src;
	else if (size == sizeof(short))    *dst = (XtArgVal)*(short*)src;
	else if (size == sizeof(char))	   *dst = (XtArgVal)*(char*)src;
	else if (size == sizeof(XtPointer)) *dst = (XtArgVal)*(XtPointer*)src;
	else if (size == sizeof(char*))    *dst = (XtArgVal)*(char**)src;
	else if (size == sizeof(XtArgVal)) *dst = *(XtArgVal*)src;
	else bcopy((char*)src, (char*)dst, (int)size);
    }
    else {
	/* proper GetValues semantics: argval is pointer to destination */
	if	(size == sizeof(long))	   *((long*)*dst) = *(long*)src;
	else if (size == sizeof(short))    *((short*)*dst) = *(short*)src;
	else if (size == sizeof(char))	   *((char*)*dst) = *(char*)src;
	else if (size == sizeof(XtPointer)) *((XtPointer*)*dst) = *(XtPointer*)src;
	else if (size == sizeof(char*))    *((char**)*dst) = *(char**)src;
	else if (size == sizeof(XtArgVal)) *((XtArgVal*)*dst)= *(XtArgVal*)src;
	else bcopy((char*)src, (char*)*dst, (int)size);
    }
} /* _XtCopyToArg */

#else
void _XtCopyFromArg(src, dst, size)
    XtArgVal src;
    char* dst;
    register unsigned int size;
{
    if (size > sizeof(XtArgVal))
	bcopy((char *)  src, (char *) dst, (int) size);
    else {
	union {
	    long	longval;
	    short	shortval;
	    char	charval;
	    char*	charptr;
	    XtPointer	ptr;
	} u;
	char *p = (char*)&u;
	if	(size == sizeof(long))	    u.longval = (long)src;
	else if (size == sizeof(short))	    u.shortval = (short)src;
	else if (size == sizeof(char))	    u.charval = (char)src;
	else if (size == sizeof(XtPointer)) u.ptr = (XtPointer)src;
	else if (size == sizeof(char*))	    u.charptr = (char*)src;
	else				    p = (char*)&src;

	bcopy(p, (char *) dst, (int) size);
    }
} /* _XtCopyFromArg */

void _XtCopyToArg(src, dst, size)
    char* src;
    XtArgVal *dst;
    register unsigned int size;
{
    if (*dst == NULL) {
	/* old GetValues semantics (storing directly into arglists) are bad,
	 * but preserve for compatibility as long as arglist contains NULL.
	 */
	union {
	    long	longval;
	    short	shortval;
	    char	charval;
	    char*	charptr;
	    XtPointer	ptr;
	} u;
	if (size <= sizeof(XtArgVal)) {
	    bcopy( (char*)src, (char*)&u, (int)size );
	    if	    (size == sizeof(long)) 	*dst = (XtArgVal)u.longval;
	    else if (size == sizeof(short))	*dst = (XtArgVal)u.shortval;
	    else if (size == sizeof(char))	*dst = (XtArgVal)u.charval;
	    else if (size == sizeof(char*))	*dst = (XtArgVal)u.charptr;
	    else if (size == sizeof(XtPointer))	*dst = (XtArgVal)u.ptr;
	    else bcopy( (char*)src, (char*)dst, (int)size );
	}
	else
	    bcopy( (char*)src, (char*)dst, (int)size );
    }
    else {
	/* proper GetValues semantics: argval is pointer to destination */
	bcopy( (char*)src, (char*)*dst, (int)size );
    }
} /* _XtCopyToArg */

#endif

static Cardinal GetNamesAndClasses(w, names, classes)
    register Widget	  w;
    register XrmNameList  names;
    register XrmClassList classes;
{
    register Cardinal length, j;
    register XrmQuark t;
    WidgetClass class;

    /* Return null-terminated quark arrays, with length the number of
       quarks (not including NULL) */

    for (length = 0; w != NULL; w = (Widget) w->core.parent) {
	names[length] = w->core.xrm_name;
	class = XtClass(w);
	/* KLUDGE KLUDGE KLUDGE KLUDGE */
	if (w->core.parent == NULL && XtIsApplicationShell(w)) {
	    classes[length] =
		((ApplicationShellWidget) w)->application.xrm_class;
	} else classes[length] = class->core_class.xrm_class;
	length++;
     }
    /* They're in backwards order, flop them around */
    for (j = 0; j < length/2; j++) {
	t = names[j];
	names[j] = names[length-j-1];
	names[length-j-1] = t;
        t = classes[j];
	classes[j] = classes[length-j-1];
	classes[length-j-1] = t;
    }
    names[length] = NULLQUARK;
    classes[length] = NULLQUARK;
    return length;
} /* GetNamesAndClasses */


/* Spiffy fast compiled form of resource list.				*/
/* XtResourceLists are compiled in-place into XrmResourceLists		*/
/* All atoms are replaced by quarks, and offsets are -offset-1 to	*/
/* indicate that this list has been compiled already			*/

void  XrmCompileResourceList(resources, num_resources)
    register XtResourceList resources;
    	     Cardinal       num_resources;
{
    register Cardinal count;

#define xrmres  ((XrmResourceList) resources)

    for (count = 0; count < num_resources; resources++, count++) {
    	xrmres->xrm_name	 = StringToName(resources->resource_name);
    	xrmres->xrm_class	 = StringToClass(resources->resource_class);
    	xrmres->xrm_type	 = StringToQuark(resources->resource_type);
/*	xrmres->xrm_size	 = resources->resource_size; */
#ifdef CRAY1
	xrmres->xrm_offset = -(resources->resource_offset * sizeof(long) + 1);
#else
        xrmres->xrm_offset	 = -resources->resource_offset - 1;
#endif
    	xrmres->xrm_default_type = StringToQuark(resources->default_type);
/*	xrmres->xrm_default_addr = resources->default_addr; */
    }
#undef xrmres
} /* XrmCompileResourceList */

static void BadSize(size, name)
    Cardinal size;
    XrmQuark name;
{
    String params[2];
    Cardinal num_params = 2;

    params[0] = (String) size;
    params[1] = XrmQuarkToString(name);
    XtWarningMsg("invalidSizeOverride", "xtDependencies", "XtToolkitError",
	"Representation size %d must match superclass's to override %s",
	params, &num_params);
} /* BadType */

/*
 * Create a new resource list, with the class resources following the
 * superclass's resources.  If a resource in the class list overrides
 * a superclass resource, then just replace the superclass entry in place.
 *
 * At the same time, add a level of indirection to the XtResourceList to
 * create and XrmResourceList.
 */
void _XtDependencies(class_resp, class_num_resp, super_res, super_num_res,
		     super_widget_size)
    XtResourceList  *class_resp;	/* VAR */
    Cardinal	    *class_num_resp;    /* VAR */
    XrmResourceList *super_res;
    Cardinal	    super_num_res;
    Cardinal	    super_widget_size;
{
    register XrmResourceList *new_res;
	     Cardinal	     new_num_res;
	     XrmResourceList class_res = (XrmResourceList) *class_resp;
	     Cardinal        class_num_res = *class_num_resp;
    register Cardinal	     i, j;
	     Cardinal        new_next;

    if (class_num_res == 0) {
	/* Just point to superclass resource list */
	*class_resp = (XtResourceList) super_res;
	*class_num_resp = super_num_res;
	return;
    }

    /* Allocate and initialize new_res with superclass resource pointers */
    new_num_res = super_num_res + class_num_res;
    new_res = (XrmResourceList *) XtMalloc(new_num_res*sizeof(XrmResourceList));
    XtBCopy(super_res, new_res, super_num_res * sizeof(XrmResourceList));
    
    /* Put pointers to class resource entries into new_res */
    new_next = super_num_res;
    for (i = 0; i < class_num_res; i++) {
	if (-class_res[i].xrm_offset-1 < super_widget_size) {
	    /* Probably an override of superclass resources--look for overlap */
	    for (j = 0; j < super_num_res; j++) {
		if (class_res[i].xrm_offset == new_res[j]->xrm_offset) {
		    /* Spec is silent on what fields subclass can override.
		     * The only two of real concern are type & size.
		     * Although allowing type to be over-ridden introduces
		     * the possibility of errors, it's at present the only
		     * reasonable way to allow a subclass to force a private
		     * converter to be invoked for a subset of fields.
		     */
		    /* We do insist that size be identical to superclass */
		    if (class_res[i].xrm_size != new_res[j]->xrm_size) {
			BadSize(class_res[i].xrm_size, class_res[i].xrm_name);
			class_res[i].xrm_size = new_res[j]->xrm_size;
		    }
		    new_res[j] = &(class_res[i]);
		    new_num_res--;
		    goto NextResource;
		}
	    } /* for j */
	}
	/* Not an overlap, add an entry to new_res */
	new_res[new_next++] = &(class_res[i]);
NextResource:;
    } /* for i */

    /* Okay, stuff new resources back into class record */
    *class_resp = (XtResourceList) new_res;
    *class_num_resp = new_num_res;
} /* _XtDependencies */


void _XtResourceDependencies(wc)
    WidgetClass wc;
{
    WidgetClass sc;

    sc = wc->core_class.superclass;
    if (sc == NULL) {
	_XtDependencies(&(wc->core_class.resources),
			&(wc->core_class.num_resources),
			(XrmResourceList *) NULL, (unsigned)0, (unsigned)0);
    } else {
	_XtDependencies(&(wc->core_class.resources),
			&(wc->core_class.num_resources),
			(XrmResourceList *) sc->core_class.resources,
			sc->core_class.num_resources,
			sc->core_class.widget_size);
    }
} /* _XtResourceDependencies */

void _XtConstraintResDependencies(wc)
    ConstraintWidgetClass wc;
{
    ConstraintWidgetClass sc;

    if (wc == (ConstraintWidgetClass) constraintWidgetClass) {
	_XtDependencies(&(wc->constraint_class.resources),
			&(wc->constraint_class.num_resources),
			(XrmResourceList *)NULL, (unsigned)0, (unsigned)0);
    } else {
	sc = (ConstraintWidgetClass) wc->core_class.superclass;
	_XtDependencies(&(wc->constraint_class.resources),
			&(wc->constraint_class.num_resources),
			(XrmResourceList *) sc->constraint_class.resources,
			sc->constraint_class.num_resources,
			sc->constraint_class.constraint_size);
    }
} /* _XtConstraintResDependencies */



    
XrmResourceList* _XtCreateIndirectionTable (resources, num_resources)
    XtResourceList  resources;
    Cardinal	    num_resources;
{
    register int index;
    XrmResourceList* table;

    table = (XrmResourceList*)XtMalloc(num_resources * sizeof(XrmResourceList));
    for (index = 0; index < num_resources; index++)
        table[index] = (XrmResourceList)(&(resources[index]));
    return table;
}

static XtCacheRef *GetResources(widget, base, names, classes,
	table, num_resources, quark_args, args, num_args,
	typed_args, pNumTypedArgs)
    Widget	    widget;	    /* Widget resources are associated with */
    char*	    base;	    /* Base address of memory to write to   */
    XrmNameList     names;	    /* Full inheritance name of widget      */
    XrmClassList    classes;	    /* Full inheritance class of widget     */
    XrmResourceList*  table;	    /* The list of resources required.      */
    Cardinal	    num_resources;  /* number of items in resource list     */
    XrmQuarkList    quark_args;     /* Arg names quarkified		    */
    ArgList	    args;	    /* ArgList to override resources	    */
    Cardinal	    num_args;       /* number of items in arg list	    */
    XtTypedArgList  typed_args;	    /* Typed arg list to override resources */
    Cardinal*	    pNumTypedArgs;  /* number of items in typed arg list    */
{
/*
 * assert: *pNumTypedArgs == 0 if num_args > 0
 * assert: num_args == 0 if *pNumTypedArgs > 0
 */
#define SEARCHLISTLEN 100

    XrmValue	    value;
    XrmQuark	    rawType;
    XrmValue	    rawValue;
    XrmHashTable    stackSearchList[SEARCHLISTLEN];
    XrmHashTable    *searchList = stackSearchList;
    unsigned int    searchListSize = SEARCHLISTLEN;
    Bool            status;
    Boolean	    found[400];
    int		    typed[400];
    XtCacheRef	    cache_ref[400];
    int		    cache_ref_size = 0;
    Display	    *dpy;
    Boolean	    persistent_resources = True;
    Boolean	    found_persistence = False;
    int		    num_typed_args = *pNumTypedArgs;

    if ((args == NULL) && (num_args != 0)) {
    	XtAppWarningMsg(XtWidgetToApplicationContext(widget),
		"invalidArgCount","getResources","XtToolkitError",
                 "argument count > 0 on NULL argument list",
                   (String *)NULL, (Cardinal *)NULL);
	num_args = 0;
    }
    if (num_resources == 0) {
	return NULL;
    } else if (table == NULL) {
    	XtAppWarningMsg(XtWidgetToApplicationContext(widget),
		"invalidResourceCount","getResources","XtToolkitError",
              "resource count > 0 on NULL resource list",
	      (String *)NULL, (Cardinal *)NULL);
	return NULL;
    }

    /* Mark each resource as not found on arg list */
    bzero((char *) found, (int) (num_resources * sizeof(Boolean)));
    bzero((char *) typed, (int) (num_resources * sizeof(int)));

    dpy = XtDisplayOfObject(widget);
    
    /* Copy the args into the resources, mark each as found */
    {
	register ArgList	    arg;
	register XtTypedArgList	    typed_arg;
	register XrmName	    argName;
	register int	    j;
	register int	    i;
	register XrmResourceList rx;
	register XrmResourceList *res;
	for (arg = args, i = 0; i < num_args; i++, arg++) {
	    argName = quark_args[i];
	    if (argName == QinitialResourcesPersistent) {
		persistent_resources = (Boolean)arg->value;
		found_persistence = True;
		break;
	    }
	    for (j = 0, res = table; j < num_resources; j++, res++) {
		rx = *res;
		if (argName == rx->xrm_name) {
		    _XtCopyFromArg(
			arg->value,
			base - rx->xrm_offset - 1,
			rx->xrm_size);
		    found[j] = TRUE;
		    break;
		}
	    }
	}
	for (typed_arg = typed_args, i = 0; i < num_typed_args;
	     i++, typed_arg++) {
	    argName = quark_args[i];
	    if (argName == QinitialResourcesPersistent) {
		persistent_resources = (Boolean)typed_arg->value;
		found_persistence = True;   
		break;
	    }
	    for (j = 0, res = table; j < num_resources; j++, res++) {
		rx = *res;
		if (argName == rx->xrm_name) {
		    if (typed_arg->type != NULL) {
			typed[j] = i + 1;
		    } else {
			_XtCopyFromArg(
				       typed_arg->value,
				       base - rx->xrm_offset - 1,
				       rx->xrm_size);
		    }
		    found[j] = TRUE;
		    break;
		}   
	    }
	}
    }

    /* Ask resource manager for a list of database levels that we can
       do a single-level search on each resource */

    status = XrmQGetSearchList(dpy->db, names, classes,
			       searchList, searchListSize);

    if (!status) {
	searchList = NULL;
	do {
	    searchList = (XrmHashTable*)
		XtRealloc((char*)searchList,
			  sizeof(XrmHashTable) * (searchListSize *= 2));
	    status = XrmQGetSearchList(dpy->db, names, classes,
				       searchList, searchListSize);
	} while (!status);
    }


    
    /* go to the resource manager for those resources not found yet */
    /* if it's not in the resource database use the default value   */

    {
	register XrmResourceList  rx;
	register XrmResourceList  *res;
	register int		  j;
	register XrmValue	  *pv = &value;
	register XrmRepresentation xrm_type;
	register XrmRepresentation xrm_default_type;
	char	char_val;
	short	short_val;
	int	int_val;
	long	long_val;
	char*	char_ptr;

	if (!found_persistence) {
	    if (XrmQGetSearchResource(searchList, QinitialResourcesPersistent,
			QInitialResourcesPersistent, &rawType, &value)) {
		if (rawType != QBoolean) {
		    rawValue = value;
		    value.size = sizeof(Boolean);
		    value.addr = (XtPointer)&persistent_resources;
		    if (!_XtConvert(widget, rawType, &rawValue, QBoolean,
				    &value, NULL))
			persistent_resources = *(Boolean*)value.addr;
		}
		else
		    persistent_resources = *(Boolean*)value.addr;
	    }
	}

	for (res = table, j = 0; j < num_resources; j++, res++) {
	    rx = *res;
	    xrm_type = rx->xrm_type;
	    if (typed[j]) {
		register XtTypedArg* arg = typed_args + typed[j] - 1;

		/*
                 * This resource value has been specified as a typed arg and 
		 * has to be converted. Typed arg conversions are done here 
		 * to correctly interpose them with normal resource conversions.
                 */
		XrmQuark	    from_type;
		XrmValue            from_val, to_val;
		Boolean		    converted;
                 
		from_type = StringToQuark(arg->type);
    		from_val.size = arg->size;
		if ((from_type == QString) || (arg->size > sizeof(XtArgVal)))
        	    from_val.addr = (caddr_t)arg->value;
	        else
            	    from_val.addr = (caddr_t)&arg->value;
		to_val.size = rx->xrm_size;
		to_val.addr = base - rx->xrm_offset - 1;
		converted = _XtConvert(widget, from_type, &from_val,
					xrm_type, &to_val, 
					persistent_resources ?
					NULL : &cache_ref[cache_ref_size]);
		if (converted) {
		    char *vp;

		    /* Copy the converted value back into the typed argument.
		     * normally the data should be <= sizeof(XtArgVal) and
		     * is stored directly into the 'value' field .... BUT
		     * if the resource size is greater than sizeof(XtArgVal)
		     * then we dynamically alloc a block of store to hold the
		     * data and zap a copy in there !!! .... freeing it later
		     * the size field in the typed arg is negated to indicate
		     * that the store pointed to by the value field is
		     * dynamic .......
		     * "freeing" happens in the case of _XtCreate after the
		     * CallInitialize ..... other clients of GetResources
		     * using typed args should be aware of the need to free
		     * this store .....
		     */

		    if(rx->xrm_size > sizeof(XtArgVal)) {
			arg->value = (XtArgVal)(vp = XtMalloc(rx->xrm_size));
			arg->size = -(arg->size);
		    } else { /* will fit - copy directly into value field */
			vp = (char *)&arg->value;
		    }

		    XtBCopy((char *)(base - rx->xrm_offset - 1), vp, rx->xrm_size);

		} else {
		   /* Conversion failed. Get default value. */
		   found[j] = False;
		}

		if ((persistent_resources == False)
			&& cache_ref[cache_ref_size] != NULL) {
		    cache_ref_size++;
		}
	    }

	    if (!found[j]) {
		Boolean	already_copied = False;
		Boolean have_value = False;

		if (XrmQGetSearchResource(searchList,
			rx->xrm_name, rx->xrm_class, &rawType, &value)) {
		    if (rawType != xrm_type) {
			rawValue = *pv;
			value.size = rx->xrm_size;
			value.addr = (XtPointer)(base - rx->xrm_offset - 1);
			already_copied = have_value =
			    _XtConvert(widget, rawType, &rawValue,
				       xrm_type, &value,
				       persistent_resources ?
				          NULL : &cache_ref[cache_ref_size]);
			if ((persistent_resources == False)
			  && cache_ref[cache_ref_size] != NULL)
			    cache_ref_size++;
		    } else have_value = True;
		}
		if (!have_value
		    && ((rx->xrm_default_type == QImmediate)
			|| (rx->xrm_default_type == xrm_type)
			|| (rx->xrm_default_addr != NULL))) {
		    /* Convert default value to proper type */
		    xrm_default_type = rx->xrm_default_type;
		    if (xrm_default_type == QCallProc) {
#ifdef CRAY
 			if ( (int) Cjumpp != (int) Cjump)
 			    (*(XtProc)(((int)(rx->xrm_default_addr))<<2))(
 				 widget,-(rx->xrm_offset+1), pv);
			else
#endif
			(*(XtProc)(rx->xrm_default_addr))(
			      widget,-(rx->xrm_offset+1), pv);

		    } else if (xrm_default_type == QImmediate) {
			/* XtRImmediate == XtRString for type XtRString */
			if (xrm_type == QString) {
			    pv->addr = rx->xrm_default_addr;
			} else if (rx->xrm_size == sizeof(int)) {
			    int_val = (int)rx->xrm_default_addr;
			    pv->addr = (XtPointer) &int_val;
			} else if (rx->xrm_size == sizeof(short)) {
			    short_val = (short)rx->xrm_default_addr;
			    pv->addr = (XtPointer) &short_val;
			} else if (rx->xrm_size == sizeof(char)) {
			    char_val = (char)rx->xrm_default_addr;
			    pv->addr = (XtPointer) &char_val;
			} else if (rx->xrm_size == sizeof(long)) {
			    long_val = (long)rx->xrm_default_addr;
			    pv->addr = (XtPointer) &long_val;
			} else if (rx->xrm_size == sizeof(char*)) {
			    char_ptr = (char*)rx->xrm_default_addr;
			    pv->addr = (XtPointer) &char_ptr;
			} else {
			    pv->addr = (XtPointer) &(rx->xrm_default_addr);
			}
		    } else if (xrm_default_type == xrm_type) {
			pv->addr = rx->xrm_default_addr;
		    } else {
			rawValue.addr = rx->xrm_default_addr;
			if (xrm_default_type == QString) {
			    rawValue.size = strlen((char *)rawValue.addr) + 1;
			} else {
			    rawValue.size = sizeof(XtPointer);
			}
			value.size = rx->xrm_size;
			value.addr = (XtPointer)(base - rx->xrm_offset - 1);
			already_copied =
			    _XtConvert(widget, xrm_default_type,
				       &rawValue, xrm_type, &value,
				       &cache_ref[cache_ref_size]);
			if (cache_ref[cache_ref_size] != NULL)
			    cache_ref_size++;
		    }
		}
		if (!already_copied) {
		    if (xrm_type == QString) {
			*((String*)(base - rx->xrm_offset - 1)) = pv->addr;
		    } else {
			if (pv->addr != NULL) {
			    XtBCopy(pv->addr, base - rx->xrm_offset - 1,
				    rx->xrm_size);
			} else {
			    /* didn't get value, initialize to NULL... */
			    XtBZero(base - rx->xrm_offset - 1, rx->xrm_size);
			}
		    }
		}

		if (typed[j]) {
		    /*
		     * This resource value was specified as a typed arg.
		     * However, the default value is being used here since
		     * type type conversion failed, so we compress the list.
		     */
		    register XtTypedArg* arg = typed_args + typed[j] - 1;
		    register int i;

		    for (i = num_typed_args - typed[j]; i; i--, arg++) {
			*arg = *(arg+1);
		    }
		    num_typed_args--;
		}
	    } 
	}
    }
    if (num_typed_args != *pNumTypedArgs) *pNumTypedArgs = num_typed_args;
    if (searchList != stackSearchList) XtFree((char*)searchList);
    if (cache_ref_size > 0) {
	XtCacheRef *refs = (XtCacheRef*)
	    XtMalloc((unsigned)sizeof(XtCacheRef)*(cache_ref_size + 1));
	bcopy( cache_ref, refs, sizeof(XtCacheRef)*cache_ref_size );
	refs[cache_ref_size] = NULL;
	return refs;
    }
    else return (XtCacheRef*)NULL;
}



static void CacheArgs(args, num_args, typed_args, num_typed_args, quark_cache, 
	num_quarks, pQuarks)
    ArgList	    args;
    Cardinal	    num_args;
    XtTypedArgList  typed_args;
    Cardinal	    num_typed_args;
    XrmQuarkList    quark_cache;
    Cardinal	    num_quarks;
    XrmQuarkList    *pQuarks;       /* RETURN */
{
    register XrmQuarkList   quarks;
    register Cardinal       i;
    register Cardinal       count;

    count = (args != NULL) ? num_args : num_typed_args;

    if (num_quarks < count) {
	quarks = (XrmQuarkList) XtMalloc(count * sizeof(XrmQuark));
    } else {
	quarks = quark_cache;
    }
    *pQuarks = quarks;

    if (args != NULL) {
	for (i = count; i; i--)
	    *quarks++ = StringToQuark((args++)->name);
    }
    else {
	for (i = count; i; i--)
	    *quarks++ = StringToQuark((typed_args++)->name);
    }
}

#define FreeCache(cache, pointer) \
	  if (cache != pointer) XtFree((char *)pointer)


XtCacheRef *_XtGetResources(w, args, num_args, typed_args, num_typed_args)
    register 	Widget	  	w;
    		ArgList	  	args;
    		Cardinal  	num_args;
		XtTypedArgList	typed_args;
		Cardinal*	num_typed_args;
{
    XrmName	    names[100];
    XrmClass	    classes[100];
    XrmQuark	    quark_cache[100];
    XrmQuarkList    quark_args;
    WidgetClass     wc;
    ConstraintWidgetClass   cwc;
    XtCacheRef	    *cache_refs;

    wc = XtClass(w);

    /* Make sure xrm_class is valid */
    /* ||| Class quarkifying should be part of Core.c */
    if (wc->core_class.xrm_class == NULLQUARK) {
        wc->core_class.xrm_class = StringToClass(wc->core_class.class_name);
    }

    /* Get names, classes for widget and ancestors */
    (void) GetNamesAndClasses(w, names, classes);
   
    /* Compile arg list into quarks */
    CacheArgs(args, num_args, typed_args, *num_typed_args, quark_cache,
	      XtNumber(quark_cache), &quark_args);

    /* Get normal resources */
    cache_refs = GetResources(w, (char*)w, names, classes,
	(XrmResourceList *) wc->core_class.resources,
	wc->core_class.num_resources, quark_args, args, num_args,
	typed_args, num_typed_args);

    if (w->core.constraints != NULL) {
	cwc = (ConstraintWidgetClass) XtClass(w->core.parent);
	GetResources(w, (char*)w->core.constraints, names, classes,
	    (XrmResourceList *) cwc->constraint_class.resources,
	    cwc->constraint_class.num_resources,
	    quark_args, args, num_args, typed_args, num_typed_args);
    }
    FreeCache(quark_cache, quark_args);
    return cache_refs;
} /* XtGetResources */


void XtGetSubresources
	(w, base, name, class, resources, num_resources, args, num_args)
    Widget	  w;		  /* Widget "parent" of subobject   */
    XtPointer	  base;		  /* Base address to write to       */
    String	  name;		  /* name of subobject		    */
    String	  class;	  /* class of subobject		    */
    XtResourceList resources;	  /* resource list for subobject    */
    Cardinal	  num_resources;
    ArgList	  args;		  /* arg list to override resources */
    Cardinal	  num_args;
{
    XrmName	  names[100];
    XrmClass	  classes[100];
    register Cardinal	  length;
    XrmQuark	  quark_cache[100];
    XrmQuarkList  quark_args;
    XrmResourceList* table;
    Cardinal	  null_typed_args = 0;

    if (num_resources == 0) return;

    /* Get full name, class of subobject */
    length = GetNamesAndClasses(w, names, classes);
    names[length] = StringToName(name);
    classes[length] = StringToClass(class);
    length++;
    names[length] = NULLQUARK;
    classes[length] = NULLQUARK;

    /* Compile arg list into quarks */
    CacheArgs(args, num_args, (XtTypedArgList)NULL, (Cardinal)0,
	      quark_cache, XtNumber(quark_cache), &quark_args);

    /* Compile resource list if needed */
    if (((int) resources->resource_offset) >= 0) {
	XrmCompileResourceList(resources, num_resources);
    }
    table = _XtCreateIndirectionTable(resources, num_resources); 
    (void) GetResources(w, (char*)base, names, classes, table, num_resources,
			quark_args, args, num_args,
			(XtTypedArgList)NULL, &null_typed_args);
    FreeCache(quark_cache, quark_args);
    XtFree((char *)table);
}


void XtGetApplicationResources
	(w, base, resources, num_resources, args, num_args)
    Widget	    w;		  /* Application shell widget       */
    XtPointer	    base;	  /* Base address to write to       */
    XtResourceList  resources;	  /* resource list for subobject    */
    Cardinal	    num_resources;
    ArgList	    args;	  /* arg list to override resources */
    Cardinal	    num_args;
{
    XrmName	    names[100];
    XrmClass	    classes[100];
    XrmQuark	    quark_cache[100];
    XrmQuarkList    quark_args;
    XrmResourceList* table;
    Cardinal	  null_typed_args = 0;

    if (num_resources == 0) return;

    /* Get full name, class of application */
    if (w == NULL) {
	/* hack for R2 compatibility */
	XtPerDisplay pd = _XtGetPerDisplay(_XtDefaultAppContext()->list[0]);
	names[0] = pd->name;
	names[1] = NULLQUARK;
	classes[0] = pd->class;
	classes[1] = NULLQUARK;
    }
    else {
	(void) GetNamesAndClasses(w, names, classes);
    }

    /* Compile arg list into quarks */
    CacheArgs(args, num_args, (XtTypedArgList)NULL, (Cardinal)0,  quark_cache, 
	XtNumber(quark_cache), &quark_args);
    /* Compile resource list if needed */
    if (((int) resources->resource_offset) >= 0) {
#ifdef	CRAY2
 	if (base == 0) {	/* this client is non-portable, but... */
 	    int count;
	    XtResourceList  res = resources;
	    for (count = 0; count < num_resources; res++, count++) {
 		res->resource_offset *= sizeof(long);
 	    }
 	}
#endif	/* CRAY2 */
	XrmCompileResourceList(resources, num_resources);
    }
    table = _XtCreateIndirectionTable(resources,num_resources);

    (void) GetResources(w, (char*)base, names, classes, table, num_resources,
			quark_args, args, num_args,
			(XtTypedArgList)NULL, &null_typed_args);
    FreeCache(quark_cache, quark_args);
    XtFree((char *)table);
}

static Boolean initialized = FALSE;

void _XtResourceListInitialize()
{
    if (initialized) {
	XtWarningMsg("initializationError","xtInitialize","XtToolkitError",
                  "Initializing Resource Lists twice",
		  (String *)NULL, (Cardinal *)NULL);
    	return;
    }
    initialized = TRUE;

    QBoolean = StringToClass(XtCBoolean);
    QString = StringToClass(XtCString);
    QCallProc = XrmStringToRepresentation(XtRCallProc);
    QImmediate = XrmStringToRepresentation(XtRImmediate);
    QinitialResourcesPersistent = StringToName(XtNinitialResourcesPersistent);
    QInitialResourcesPersistent = StringToName(XtCInitialResourcesPersistent);
}
