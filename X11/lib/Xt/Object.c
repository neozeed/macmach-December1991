/* LINTLIBRARY */
#ifndef lint
static char Xrcsid[] = "$XConsortium: Object.c,v 1.10 89/10/05 12:54:13 swick Exp $";
/* $oHeader: Object.c,v 1.2 88/08/18 15:51:09 asente Exp $ */
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

#define OBJECT
#include "IntrinsicI.h"
#include "StringDefs.h"

static XtResource resources[] = {
        {XtNdestroyCallback, XtCCallback, XtRCallback,sizeof(XtPointer),
         XtOffset(Object,object.destroy_callbacks), XtRCallback, (XtPointer)NULL}
    };

static void ObjectClassPartInitialize();
static Boolean ObjectSetValues();
static void ObjectDestroy();

externaldef(objectclassrec) ObjectClassRec objectClassRec = {
  {
    /* superclass	  */	NULL,
    /* class_name	  */	"Object",
    /* widget_size	  */	sizeof(ObjectRec),
    /* class_initialize   */    NULL,
    /* class_part_initialize*/	ObjectClassPartInitialize,
    /* class_inited       */	FALSE,
    /* initialize	  */	NULL,
    /* initialize_hook    */	NULL,		
    /* pad                */    NULL,
    /* pad		  */	NULL,
    /* pad       	  */	0,
    /* resources	  */	resources,
    /* num_resources	  */    XtNumber(resources),
    /* xrm_class	  */	NULLQUARK,
    /* pad                */    FALSE,
    /* pad                */    FALSE,
    /* pad                */    FALSE,
    /* pad                */    FALSE,
    /* destroy		  */	ObjectDestroy,
    /* pad		  */	NULL,
    /* pad		  */	NULL,
    /* set_values	  */	ObjectSetValues,
    /* set_values_hook    */	NULL,			
    /* pad                */    NULL,
    /* get_values_hook    */	NULL,			
    /* pad                */    NULL,
    /* version		  */	XtVersion,
    /* callback_offsets   */    NULL,
    /* pad                */    NULL,
    /* pad                */    NULL,
    /* pad                */    NULL,
    /* extension	    */  NULL
}
};

externaldef(objectClass) WidgetClass objectClass
                          = (WidgetClass)&objectClassRec;

/*
 * Start of object routines.
 */


static void ConstructCallbackOffsets(widgetClass)
    WidgetClass widgetClass;
{
    register Cardinal i;
    register XrmResourceList resourceList;
    register _XtOffsetList newItem;
    register XrmQuark QCallback;
    ObjectClass objectClass = (ObjectClass)widgetClass;

    /* ||| Should initialize QCallback exactly once, actually */
    QCallback = XrmStringToQuark(XtRCallback);

    if (objectClass->object_class.superclass != NULL)
	objectClass->object_class.callback_private = 
	   ((ObjectClass) objectClass->object_class.superclass)->
                      object_class.callback_private;
    for (i = objectClass->object_class.num_resources,
	 resourceList = (XrmResourceList) objectClass->object_class.resources;
         i != 0; i--)
	if (resourceList[i-1].xrm_type == QCallback) {
	    newItem = XtNew(XtOffsetRec);
	    newItem->next   = (_XtOffsetList)objectClass->object_class.callback_private;
	    objectClass->object_class.callback_private = (XtPointer)newItem;
	    newItem->offset = resourceList[i-1].xrm_offset;
	    newItem->name   = resourceList[i-1].xrm_name;
     }
}

static void ObjectClassPartInitialize(wc)
    register WidgetClass wc;
{
   ObjectClass oc = (ObjectClass)wc;
   extern void  XrmCompileResourceList();

    if (oc->object_class.resources != NULL) {
	XrmCompileResourceList(oc->object_class.resources,
		oc->object_class.num_resources);
    }
    ConstructCallbackOffsets(wc);
    _XtResourceDependencies(wc);
}


/*ARGSUSED*/
static Boolean ObjectSetValues(old, request, widget)
    Widget	old, request, widget;
{
    register _XtOffsetList offset;
    register CallbackStruct *oldCallbacks, **newCallbacksP;
    extern CallbackStruct* _XtCompileCallbackList();
    extern void _XtFreeCallbackList();

    /* Compile any callback lists into internal form */
    offset = (_XtOffsetList)XtClass(widget)->core_class.callback_private;
    while (offset != NULL) {
	oldCallbacks = *(CallbackStruct**)((char*)old - offset->offset - 1);
	newCallbacksP = (CallbackStruct**)((char*)widget - offset->offset - 1);

	if (*newCallbacksP != oldCallbacks) {
	    if (oldCallbacks != NULL)
		_XtFreeCallbackList(oldCallbacks);

	    if (*newCallbacksP != NULL)
		*newCallbacksP =
		    _XtCompileCallbackList(widget,
					   (XtCallbackList)*newCallbacksP);
	}
	offset = offset->next;
    }
    return False;
}


static void ObjectDestroy (widget)
    register Widget    widget;
{
    register _XtOffsetList offset;
    extern CallbackList* _XtCallbackList();

    if (((Object)widget)->object.constraints != NULL)
	    XtFree((char *) ((Object)widget)->object.constraints);

    /* Remove all callbacks associated with widget */
    offset = (_XtOffsetList)
	widget->core.widget_class->core_class.callback_private;

    while (offset != NULL) {
	register CallbackStruct *callbacks =
	    *(CallbackStruct**)((char *)widget - offset->offset - 1);
	if (callbacks != NULL) {
	    _XtFreeCallbackList(callbacks);
	}
	offset = offset->next;
    }

    XtFree((char *) widget);
} /* ObjectDestroy */


