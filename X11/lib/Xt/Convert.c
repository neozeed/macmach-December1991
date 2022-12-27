#ifndef lint
static char Xrcsid[] = "$XConsortium: Convert.c,v 1.38 89/12/20 16:30:54 swick Exp $";
/* $oHeader: Convert.c,v 1.4 88/09/01 11:10:44 asente Exp $ */
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

#include	"StringDefs.h"
#include	"IntrinsicI.h"
#include	"Quarks.h"

#ifdef DEBUG
#include	<stdio.h>
#endif

/* Conversion procedure hash 	table */

typedef struct _ConverterRec *ConverterPtr;

typedef struct _ConverterRec {
    ConverterPtr	next;
    XrmRepresentation	from, to;
    XtTypeConverter	converter;
    XtDestructor	destructor;
    XtConvertArgList    convert_args;
    Cardinal		num_args;
    Boolean		new_style;
    XtCacheType		cache_type;
} ConverterRec;

#define CONVERTHASHSIZE	((unsigned)512)
#define CONVERTHASHMASK	511
#define ProcHash(from_type, to_type) (2 * (from_type) + to_type)

/* used for old-style type converter cache only */
static Heap globalHeap = {NULL, NULL, 0};

void _XtSetDefaultConverterTable(table)
	ConverterTable *table;
{
    register ConverterTable globalConverterTable =
	_XtGetProcessContext()->globalConverterTable;

    *table = (ConverterTable)
	XtCalloc(CONVERTHASHSIZE, (unsigned)sizeof(ConverterPtr));
    _XtAddDefaultConverters(*table);

    if (globalConverterTable != (ConverterTable)NULL) {
	ConverterPtr rec;
	int i;
	for (i = CONVERTHASHSIZE; i; i--, globalConverterTable++) {
	    for (rec = *globalConverterTable; rec != NULL; rec = rec->next)
	       _XtTableAddConverter(*table, rec->from, rec->to, rec->converter,
				    rec->convert_args, rec->num_args,
				    rec->new_style, rec->cache_type,
				    rec->destructor);
  	}
    }
}

void _XtFreeConverterTable(table)
	ConverterTable table;
{
	register int i;
	register ConverterPtr p;

	for (i = 0; i < CONVERTHASHSIZE; i++) {
	    for (p = table[i]; p != NULL;) {
		register ConverterPtr next = p->next;
		XtFree((char*)p);
		p = next;
	    }
	}
	XtFree((char*)table);
}	

/* Data cache hash table */

typedef struct _CacheRec *CachePtr;

typedef struct _CacheRec {
    CachePtr	next;
    XtPointer	tag;
    int		hash;
    XtTypeConverter converter;
    XrmValue	*args;
    short	num_args;
    short	flags;
#define		CONVERSION_SUCCEEDED	0x01
#define		HAS_DESTRUCTOR		0x02
#define		IS_REFCOUNTED		0x04
#define		MUST_BE_FREED		0x08
    XrmValue	from;
    XrmValue	to;
    struct _CacheRecExt {
	CachePtr	prev;
	XtDestructor	destructor;
	XtPointer	closure;
	long		ref_count;
    } more;
} CacheRec;

#define CACHEHASHSIZE	256
#define CACHEHASHMASK	255
typedef CachePtr CacheHashTable[CACHEHASHSIZE];

static CacheHashTable	cacheHashTable;

void _XtTableAddConverter(table, from_type, to_type, converter, convert_args, num_args, new_style, cache_type, destructor)
    ConverterTable	table;
    XrmRepresentation   from_type, to_type;
    XtTypeConverter	converter;
    XtConvertArgList    convert_args;
    Cardinal		num_args;
    Boolean		new_style;
    XtCacheType		cache_type;
    XtDestructor	destructor;
{
    register ConverterPtr	*pHashEntry;
    register ConverterPtr	p;
    XtDestructor nullProc = NULL;

    pHashEntry= &table[ProcHash(from_type, to_type) & CONVERTHASHMASK];
    for (p = *pHashEntry; p != NULL; p = p->next) {
	if (p->from == from_type && p->to == to_type) break;
    }

    if (p == NULL) {
	p = (ConverterPtr) XtMalloc((unsigned)sizeof(ConverterRec));
	p->next	    = *pHashEntry;
	*pHashEntry     = p;
	p->from	    = from_type;
	p->to	    = to_type;
    }
    p->converter    = converter;
    p->destructor   = destructor;
    p->num_args     = num_args;	
    if (num_args) {
	XtConvertArgList args =
	    p->convert_args = (XtConvertArgList)
		XtMalloc( num_args*sizeof(XtConvertArgRec) );
	while (num_args--) {
	    *args++ = *convert_args++;
	}
    }
    else {
	p->convert_args = NULL;
    }
    p->new_style    = new_style;
    if (destructor != nullProc || ((cache_type & 0xff) != XtCacheNone))
	p->cache_type = cache_type;
    else
	p->cache_type = XtCacheNone;
}

void XtSetTypeConverter(from_type, to_type, converter, convert_args, num_args, cache_type, destructor)
    register String	from_type, to_type;
    XtTypeConverter	converter;
    XtConvertArgList    convert_args;
    Cardinal		num_args;
    XtCacheType		cache_type;
    XtDestructor	destructor;
{
    ProcessContext process = _XtGetProcessContext();
    XtAppContext app = process->appContextList;
    XrmRepresentation from = XrmStringToRepresentation(from_type);
    XrmRepresentation to = XrmStringToRepresentation(to_type);

    if (process->globalConverterTable == (ConverterTable)NULL) {
	process->globalConverterTable = (ConverterTable)
	    XtCalloc(CONVERTHASHSIZE, (unsigned)sizeof(ConverterPtr));
    }
    _XtTableAddConverter(process->globalConverterTable, from, to,
			 converter, convert_args,
			 num_args, True, cache_type, destructor);
    while (app != (XtAppContext)NULL) {
	_XtTableAddConverter(app->converterTable, from, to,
			     converter, convert_args,
			     num_args, True, cache_type, destructor);
	app = app->next;
    }
}

void XtAppSetTypeConverter(app, from_type, to_type, converter, convert_args, num_args, cache_type, destructor)
    XtAppContext	app;
    register String	from_type, to_type;
    XtTypeConverter	converter;
    XtConvertArgList    convert_args;
    Cardinal		num_args;
    XtCacheType		cache_type;
    XtDestructor	destructor;
{
    _XtTableAddConverter(app->converterTable,
	XrmStringToRepresentation(from_type),
        XrmStringToRepresentation(to_type),
	converter, convert_args, num_args,
	True, cache_type, destructor);
}

/* old interface */
void XtAddConverter(from_type, to_type, converter, convert_args, num_args)
    register String	from_type, to_type;
    XtConverter		converter;
    XtConvertArgList    convert_args;
    Cardinal		num_args;
{
    ProcessContext process = _XtGetProcessContext();
    XtAppContext app = process->appContextList;
    XrmRepresentation from = XrmStringToRepresentation(from_type);
    XrmRepresentation to = XrmStringToRepresentation(to_type);

    if (process->globalConverterTable == (ConverterTable)NULL) {
	process->globalConverterTable = (ConverterTable)
	    XtCalloc(CONVERTHASHSIZE, (unsigned)sizeof(ConverterPtr));
    }
    _XtTableAddConverter(process->globalConverterTable, from, to,
			 (XtTypeConverter)converter, convert_args, num_args,
			 False, XtCacheAll, (XtDestructor)NULL);
    while (app != (XtAppContext)NULL) {
	_XtTableAddConverter(app->converterTable, from, to,
			     (XtTypeConverter)converter, convert_args,
			     num_args, False, XtCacheAll, (XtDestructor)NULL);
	app = app->next;
    }
}

/* old interface */
void XtAppAddConverter(app, from_type, to_type, converter, convert_args, num_args)
    XtAppContext	app;
    register String	from_type, to_type;
    XtConverter		converter;
    XtConvertArgList    convert_args;
    Cardinal		num_args;
{
    _XtTableAddConverter(app->converterTable,
	XrmStringToRepresentation(from_type),
        XrmStringToRepresentation(to_type),
	(XtTypeConverter)converter, convert_args, num_args,
	False, XtCacheAll, (XtDestructor)NULL);
}

static CachePtr
CacheEnter(heap, converter, args, num_args, from, to, succeeded, hash,
	   flags, destructor, closure)
    Heap*		    heap;
    register XtTypeConverter converter;
    register XrmValuePtr    args;
    Cardinal		    num_args;
    XrmValuePtr		    from;
    XrmValuePtr		    to;
    Boolean		    succeeded;
    register int	    hash;
    int			    flags;
    XtDestructor	    destructor;
    XtPointer		    closure;
{
    register	CachePtr *pHashEntry;
    register	CachePtr p;
    register    Cardinal i;
    extern char* _XtHeapAlloc();
    XtDestructor nullProc = NULL; /* some compilers broken */
    Boolean	has_destructor = (destructor != nullProc);

    pHashEntry = &cacheHashTable[hash & CACHEHASHMASK];

    if (has_destructor || (flags & IS_REFCOUNTED)) {
	p = (CachePtr) _XtHeapAlloc(heap, (Cardinal)sizeof(CacheRec));
	p->more.prev = NULL;
	p->more.ref_count = 1;
    }
    else {
	p = (CachePtr)_XtHeapAlloc(heap,
		  (Cardinal)(sizeof(CacheRec) - sizeof(struct _CacheRecExt)));
    }
    if (has_destructor) {
	p->flags = HAS_DESTRUCTOR | CONVERSION_SUCCEEDED | flags;
	p->more.destructor = destructor;
	p->more.closure = closure;
    }
    else
	p->flags = CONVERSION_SUCCEEDED | flags; 

    p->next	    = *pHashEntry;
    if (p->next != NULL && (p->next->flags & HAS_DESTRUCTOR))
	p->next->more.prev = p;

    *pHashEntry     = p;
    p->tag	    = (XtPointer)heap;
    p->hash	    = hash;
    p->converter    = converter;
    p->from.size    = from->size;
    p->from.addr = (XtPointer) _XtHeapAlloc(heap, from->size);
    XtBCopy(from->addr, p->from.addr, from->size);
    p->num_args = num_args;
    if (num_args == 0) {
	p->args = NULL;
    } else {
	p->args = (XrmValuePtr) _XtHeapAlloc(heap, num_args * sizeof(XrmValue));
	for (i = 0; i < num_args; i++) {
	    p->args[i].size = args[i].size;
	    p->args[i].addr = (XtPointer) _XtHeapAlloc(heap, args[i].size);
	    XtBCopy(args[i].addr, p->args[i].addr, args[i].size);
	}
    }
    p->to.size  = to->size;
    if (succeeded && to->addr != NULL) {
	p->to.addr  = (XtPointer) _XtHeapAlloc(heap, to->size);
	XtBCopy(to->addr, p->to.addr, to->size);
    }
    else {
	p->to.addr = NULL;
	p->flags &= ~CONVERSION_SUCCEEDED;
    }
    return p;
}


void _XtCacheFlushTag(app, tag)
    XtAppContext app;
    XtPointer	tag;
{
    int i;
    extern void _XtFreeCacheRec();
    for (i = CACHEHASHSIZE; i;) {
	register CachePtr* prev = &cacheHashTable[--i];
	register CachePtr rec = *prev;
	while (rec != NULL) {
	    while (rec != NULL && rec->tag == tag) {
		CachePtr next = rec->next;
		_XtFreeCacheRec( app, (XtCacheRef)rec );
		rec = next;
	    }
	    *prev = rec;
	    if (rec == NULL) break;
	    do {
		prev = &rec->next;
		rec = *prev;
	    } while (rec != NULL && rec->tag != tag);
	}
    }
}

#ifdef DEBUG
void CacheStats()
{
    register Cardinal i;
    register CachePtr p;
    register Cardinal entries;

    for (i = 0; i < CACHEHASHSIZE; i++) {
	p = cacheHashTable[i];
	if (p != NULL) {
	    for (entries = 0; p != NULL; p = p->next) {
		entries++;
	    }
	    (void) fprintf(stdout, "Index: %4d  Entries: %d\n", i, entries);
	    for (p = cacheHashTable[i]; p != NULL; p = p->next) {
		(void) fprintf(stdout, "    Size: %3d  Refs: %3d  '",
			       p->from.size, p->more.ref_count);
		(void) fwrite(
		    p->from.addr, 1, MIN( (int)p->from.size, 48), stdout);
		(void) fprintf(stdout, "'\n");
	    }
	    (void) fprintf(stdout, "\n");
	}
    }
}
#endif /*DEBUG*/

static Boolean ResourceQuarkToOffset(widget_class, name, offset)
    WidgetClass widget_class;
    XrmName     name;
    Cardinal    *offset;
{
    register WidgetClass     wc;
    register Cardinal        i;
    register XrmResourceList res, *resources;

    for (wc = widget_class;
	wc != NULL;
	wc = wc->core_class.superclass) {
	resources = (XrmResourceList*) wc->core_class.resources;
	for (i = 0; i < wc->core_class.num_resources; i++, resources++) {
	    res = *resources;
	    if (res->xrm_name == name) {
		*offset = -res->xrm_offset - 1;
		return True;
	    }
	} /* for i in resources */
    } /* for wc in widget classes */
    (*offset) = 0;
    return False;
}


static void ComputeArgs(widget, convert_args, num_args, args)
    Widget		widget;
    XtConvertArgList    convert_args;
    Cardinal		num_args;
    XrmValuePtr		args;
{
    register Cardinal   i;
    Cardinal		offset;
    String              params[1];
    Cardinal		num_params = 1;
    Widget		ancestor = NULL;

    for (i = 0; i < num_args; i++) {
	args[i].size = convert_args[i].size;
	switch (convert_args[i].address_mode) {
	case XtAddress: 
	    args[i].addr = convert_args[i].address_id;
	    break;

	case XtBaseOffset:
#ifdef CRAY1
	    args[i].addr =
		(XtPointer)((int)widget + (int)convert_args[i].address_id);
#else
	    args[i].addr = (XtPointer)widget + (int)convert_args[i].address_id;
#endif
	    break;

	case XtWidgetBaseOffset:
	    if (ancestor == NULL) {
		if (XtIsWidget(widget))
		    ancestor = widget;
		else
		    ancestor = _XtWindowedAncestor(widget);
	    }

#ifdef CRAY1
	    args[i].addr =
		(XtPointer)((int)ancestor + (int)convert_args[i].address_id);
#else
	    args[i].addr =
		(XtPointer)ancestor + (int)convert_args[i].address_id;
#endif
	    break;

	case XtImmediate:
	    args[i].addr = (XtPointer) &(convert_args[i].address_id);
	    break;

	case XtProcedureArg:
	    (*(XtConvertArgProc)convert_args[i].address_id)
		(widget, &convert_args[i].size, &args[i]);
	    break;

	case XtResourceString:
	    /* Convert in place for next usage */
	    convert_args[i].address_mode = XtResourceQuark;
	    convert_args[i].address_id =
	       (XtPointer)XrmStringToQuark((String)convert_args[i].address_id);
	    /* Fall through */

	case XtResourceQuark:
	    if (! ResourceQuarkToOffset(widget->core.widget_class,
		    (XrmQuark) convert_args[i].address_id, &offset)) {
		params[0]=
                  XrmQuarkToString((XrmQuark) convert_args[i].address_id);
               XtAppWarningMsg(XtWidgetToApplicationContext(widget),
		    "invalidResourceName","computeArgs","XtToolkitError",
		    "Cannot find resource name %s as argument to conversion",
                     params,&num_params);
		offset = 0;
	    }
#ifdef CRAY1
	    args[i].addr = (XtPointer)((int)widget + offset);
#else
	    args[i].addr = (XtPointer)widget + offset;
#endif
	    break;
	default:
	    params[0] = XtName(widget);
	    XtAppWarningMsg(XtWidgetToApplicationContext(widget),
		"invalidAddressMode", "computeArgs", "XtToolkitError",
		"Conversion arguments for widget '%s' contain an unsupported address mode",
			params,&num_params);
	    args[i].addr = NULL;
	    args[i].size = 0;
	} /* switch */
    } /* for */
} /* ComputeArgs */

void XtDirectConvert(converter, args, num_args, from, to)
    XtConverter     converter;
    XrmValuePtr     args;
    Cardinal	    num_args;
    register XrmValuePtr from;
    XrmValuePtr     to;
{
    register CachePtr   p;
    register int	hash;
    register Cardinal   i;

    /* Try to find cache entry for conversion */
    hash = ((int)(converter) >> 2) + from->size + *((char *) from->addr);
    if (from->size > 1) hash += ((char *) from->addr)[1];
    
    for (p = cacheHashTable[hash & CACHEHASHMASK]; p != NULL; p = p->next) {
	if ((p->hash == hash)
	 && (p->converter == (XtTypeConverter)converter)
	 && (p->from.size == from->size)
	 && (! XtBCmp(p->from.addr, from->addr, from->size))
         && (p->num_args == num_args)) {
	    /* Are all args the same data ? */
	    for (i = 0; i < num_args; i++) {
		if (   p->args[i].size != args[i].size
		    || XtBCmp(p->args[i].addr, args[i].addr, args[i].size)) {
		    break;
		}
	    }
	    if (i == num_args) {	 
		/* Perfect match */
		(*to) = p->to;
		return;
	    }
	}
    }

    /* Didn't find it, call converter procedure and entry result in cache */
    (*to).size = 0;
    (*to).addr = NULL;
    (*converter)(args, &num_args, from, to);
    /* This memory can never be freed since we don't know the Display
     * or app context from which to compute the persistance */
    {
	CacheEnter( &globalHeap, (XtTypeConverter)converter, args, num_args,
		    from, to, (to->addr != NULL), hash, 0,
		    (XtDestructor)NULL, NULL
		   );
    }
}


static ConverterPtr GetConverterEntry( app, converter )
    XtAppContext app;
    XtTypeConverter converter;
{
    int entry;
    register ConverterPtr cP;
    ConverterTable converterTable = app->converterTable;
    cP = NULL;
    for (entry = 0; entry < CONVERTHASHSIZE && cP == NULL; entry++) {
	cP = converterTable[entry];
	while (cP != NULL && cP->converter != converter) cP = cP->next;
    }
    return cP;
}


Boolean
XtCallConverter(dpy, converter, args, num_args, from, to, cache_ref_return)
    Display*	    dpy;
    XtTypeConverter converter;
    XrmValuePtr     args;
    Cardinal	    num_args;
    register XrmValuePtr from;
    XrmValuePtr     to;
    XtCacheRef	    *cache_ref_return;
{
    register CachePtr   p;
    register int	hash;
    register Cardinal   i;
    register ConverterPtr cP;
    XtDestructor nullProc = NULL; /* some compilers broken */

    cP = GetConverterEntry( XtDisplayToApplicationContext(dpy), converter );
    if (cP == NULL
     || ((cP->cache_type == XtCacheNone) && (cP->destructor == nullProc))) {
	char* closure;
	if (cache_ref_return != NULL) *cache_ref_return = NULL;
	return (*(XtTypeConverter)converter)
	    (dpy, args, &num_args, from, to, &closure);
    }

    /* Try to find cache entry for conversion */
    hash = ((int)(converter) >> 2) + from->size + *((char *) from->addr);
    if (from->size > 1) hash += ((char *) from->addr)[1];
    
    if ((cP->cache_type & 0xff) != XtCacheNone) {
	for (p = cacheHashTable[hash & CACHEHASHMASK]; p != NULL; p = p->next){
	    if ((p->hash == hash)
	     && (p->converter == converter)
	     && (p->from.size == from->size)
	     && (! XtBCmp(p->from.addr, from->addr, from->size))
	     && (p->num_args == num_args)) {
		/* Are all args the same data ? */
		for (i = 0; i < num_args; i++) {
		    if (p->args[i].size != args[i].size
		     || XtBCmp(p->args[i].addr, args[i].addr, args[i].size)) {
			break;
		    }
		}
		if (i == num_args) {	 
		    /* Perfect match */
		    if (p->flags & CONVERSION_SUCCEEDED) {
			if (to->addr != NULL) {	/* new-style call */
			    XtBCopy(p->to.addr, to->addr, to->size);
			}
			else	/* old-style call */
			    *to = p->to;
		    }
		    if (p->flags & IS_REFCOUNTED) {
			p->more.ref_count++;
			if (cache_ref_return != NULL)
			    *cache_ref_return = (XtCacheRef)p;
			else
			    p->flags &= ~IS_REFCOUNTED;
		    }
		    else {
			if (cache_ref_return != NULL)
			    *cache_ref_return = NULL;
		    }
		    return (p->flags & CONVERSION_SUCCEEDED);
		}
	    }
	}
    }

    /* No cache entry, call converter procedure and enter result in cache */
    {
	Heap *heap;
	char* closure;
	XtCacheType cache_type = cP->cache_type & 0xff;
	int ref_flags =
	    ((cP->cache_type & XtCacheRefCount) && (cache_ref_return != NULL))
		? IS_REFCOUNTED : 0;
	Boolean retval =
	    (*(XtTypeConverter)converter)(dpy, args, &num_args, from, to, &closure);
	if ((cache_type == XtCacheNone) || (ref_flags & IS_REFCOUNTED)) {
	    heap = NULL;
	    ref_flags |= MUST_BE_FREED;
	}
	else if (cache_type == XtCacheByDisplay)
	    heap = &_XtGetPerDisplay(dpy)->heap;
	else
	    heap = &XtDisplayToApplicationContext(dpy)->heap;

	p = CacheEnter(heap, converter, args, num_args, from, to, retval,
		       hash, ref_flags, cP->destructor, closure);
	if (ref_flags & IS_REFCOUNTED)
	    *cache_ref_return = (XtCacheRef)p;
	else if (cache_ref_return != NULL)
	    *cache_ref_return = NULL;

	return retval;
    }
}

Boolean _XtConvert(widget, from_type, from, to_type, to, cache_ref_return)
             Widget		widget;
    register XrmRepresentation	from_type;
	     XrmValuePtr	from;
    register XrmRepresentation	to_type;
    register XrmValuePtr	to;
    XtCacheRef			*cache_ref_return;
{
    XtAppContext	app = XtWidgetToApplicationContext(widget);
    register ConverterPtr	p;
    Cardinal		num_args;
    XrmValue		*args;

    /* Look for type converter */
    p = app->converterTable[ProcHash(from_type, to_type) & CONVERTHASHMASK];
    for (; p != NULL; p = p->next) {
	if (from_type == p->from && to_type == p->to) {
	    Boolean retval;
	    /* Compute actual arguments from widget and arg descriptor */
	    num_args = p->num_args;
	    if (num_args != 0) {
		args = (XrmValue*)
		    ALLOCATE_LOCAL( num_args * sizeof (XrmValue) );
		if (args == NULL) _XtAllocError("alloca");
		ComputeArgs(widget, p->convert_args, num_args, args);
	    } else args = NULL;
	    if (p->new_style) {
		retval =
		    XtCallConverter(XtDisplayOfObject(widget),
				    p->converter, args, num_args,
				    from, to, cache_ref_return);
	    }
	    else { /* is old-style (non-app) converter */
		XrmValue tempTo;
		XtDirectConvert((XtConverter)p->converter, args, num_args,
				from, &tempTo);
		if (cache_ref_return != NULL)
		    *cache_ref_return = NULL;
		if (tempTo.addr != NULL) {
		    if (to->addr != NULL) { /* new-style call */
			static XrmRepresentation QString = NULLQUARK;
			if (QString == NULLQUARK)
			    QString = XrmStringToRepresentation(XtRString);
			if (to_type == QString)
			    *(String*)(to->addr) = tempTo.addr;
			else
			    XtBCopy(tempTo.addr, to->addr, to->size);
		    } else {	/* old-style call */
			*to = tempTo;
		    }
		    retval = True;
		} else retval = False;
	    }
	    if (args != NULL) DEALLOCATE_LOCAL( (XtPointer)args );
	    return retval;
	}
    }

    {
	String params[2];
	Cardinal num_params = 2;
	params[0] = XrmRepresentationToString(from_type);
	params[1] = XrmRepresentationToString(to_type);
	XtAppWarningMsg(app, "typeConversionError", "noConverter", "XtToolkitError",
	     "No type converter registered for '%s' to '%s' conversion.",
             params, &num_params);
    }
    return False;
}

void XtConvert(widget, from_type_str, from, to_type_str, to)
    Widget	widget;
    String	from_type_str;
    XrmValuePtr	from;
    String	to_type_str;
    XrmValuePtr	to;
{
    XrmQuark    from_type, to_type;

    from_type = XrmStringToRepresentation(from_type_str);
    to_type = XrmStringToRepresentation(to_type_str);
    if (from_type != to_type) {
	/*  It's not safe to ref count these resources, 'cause we
	    don't know what older clients may have assumed about
	    the resource lifetimes.
	XtCacheRef ref;
	*/
	to->addr = NULL;
	to->size = 0;
	_XtConvert(widget, from_type, from, to_type, to, /*&ref*/ NULL);
	/*
	if (ref != NULL) {
	    XtAddCallback( widget, XtNdestroyCallback,
			   XtCallbackReleaseCacheRef, (XtPointer)ref );
	}
	*/
    }
    else
	(*to) = *from;
}

Boolean XtConvertAndStore(object, from_type_str, from, to_type_str, to)
    Widget	object;
    String	from_type_str;
    XrmValuePtr	from;
    String	to_type_str;
    XrmValuePtr	to;
{
    XrmQuark    from_type, to_type;

    from_type = XrmStringToRepresentation(from_type_str);
    to_type = XrmStringToRepresentation(to_type_str);
    if (from_type != to_type) {
	static XtPointer local_valueP = NULL;
	static Cardinal local_valueS = 128;
	XtCacheRef ref;
	Boolean local = False;
	do {
	    if (to->addr == NULL) {
		if (local_valueP == NULL)
		    local_valueP = _XtHeapAlloc(&globalHeap, local_valueS);
		to->addr = local_valueP;
		to->size = local_valueS;
		local = True;
	    }
	    if (!_XtConvert(object, from_type, from, to_type, to, &ref)) {
		if (local && (to->size > local_valueS)) {
		    to->addr =
			local_valueP = _XtHeapAlloc(&globalHeap, to->size);
		    local_valueS = to->size;
		    continue;
		} else {
		    if (local) {
			to->addr = NULL;
			to->size = 0;
		    }
		    return False;
		}
	    }
	    if (ref != NULL) {
		XtAddCallback( object, XtNdestroyCallback,
			       XtCallbackReleaseCacheRef, (XtPointer)ref );
	    }
	    return True;
	} while (local /* && local_valueS < to->size */);
    }
    if (to->addr != NULL) {
	if (to->size < from->size) {
	    to->size = from->size;
	    return False;
	}
	bcopy( from->addr, to->addr, from->size );
    } else			/* from_type == to_type */
	*to = *from;

    return True;
}

void _XtFreeCacheRec(app, p)
    XtAppContext app;
    CachePtr p;
{
    if (p->flags & HAS_DESTRUCTOR) {
	Cardinal num_args = p->num_args;
	(*p->more.destructor) (app, &p->to, p->more.closure, p->args,
			       &num_args);
	if (p->more.prev)
	    p->more.prev->next = p->next;
	else
	    cacheHashTable[p->hash & CACHEHASHMASK] = p->next;
    }
    if ((p->next != NULL) && (p->next->flags & HAS_DESTRUCTOR))
	p->next->more.prev = p->more.prev;

    if (p->flags & MUST_BE_FREED) {
	register int i;
	XtFree(p->from.addr);
	for (i = p->num_args; i;) {
	    XtFree(p->args[--i].addr);
	}
	XtFree((char*)p->args);
	XtFree(p->to.addr);
	XtFree((char*)p);
    }
    /* else on private heap; will free entire heap later */
}

void XtAppReleaseCacheRefs(app, refs)
    XtAppContext app;
    XtCacheRef *refs;
{
    register CachePtr *r;
    for (r = (CachePtr*)refs; *r != NULL; r++) {
	register CachePtr p = *r;
	if ((p->flags & IS_REFCOUNTED) && --(p->more.ref_count) == 0) {
	    _XtFreeCacheRec( app, p );
	}
    }
}


/* ARGSUSED */
void XtCallbackReleaseCacheRefList(widget, closure, call_data)
    Widget widget;		/* unused */
    XtPointer closure;
    XtPointer call_data;	/* unused */
{
    XtAppReleaseCacheRefs( XtWidgetToApplicationContext(widget),
			   (XtCacheRef*)closure );
    XtFree(closure);
}


/* ARGSUSED */
void XtCallbackReleaseCacheRef(widget, closure, call_data)
    Widget widget;		/* unused */
    XtPointer closure;
    XtPointer call_data;	/* unused */
{
    XtCacheRef cache_refs[2];
    cache_refs[0] = (XtCacheRef)closure;
    cache_refs[1] = NULL;
    XtAppReleaseCacheRefs( XtWidgetToApplicationContext(widget), cache_refs );
}
