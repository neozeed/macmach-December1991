#ifndef lint
static char Xrcsid[] = "$XConsortium: Core.c,v 1.34 89/12/12 19:30:44 swick Exp $";
/* $oHeader: Core.c,v 1.2 88/08/18 15:37:59 asente Exp $ */
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

#define _XT_CORE_C
#include "IntrinsicP.h"
#include "EventI.h"
#include "TranslateI.h"

#include "RectObj.h"
#include "RectObjP.h"
#include "StringDefs.h"

/******************************************************************
 *
 * CoreWidget Resources
 *
 ******************************************************************/

externaldef(xtinherittranslations) int _XtInheritTranslations = NULL;

static XtResource resources[] = {
    {XtNscreen, XtCScreen, XtRScreen, sizeof(int),
      XtOffset(CoreWidget,core.screen), XtRCallProc, (XtPointer)XtCopyScreen},
/*XtCopyFromParent does not work for screen because the Display
parameter is not passed through to the XtRCallProc routines */
    {XtNdepth, XtCDepth, XtRInt,sizeof(int),
         XtOffset(CoreWidget,core.depth), XtRCallProc, (XtPointer)XtCopyFromParent},
    {XtNcolormap, XtCColormap, XtRColormap, sizeof(Colormap),
      XtOffset(CoreWidget,core.colormap), XtRCallProc,(XtPointer)XtCopyFromParent},
    {XtNbackground, XtCBackground, XtRPixel,sizeof(Pixel),
         XtOffset(CoreWidget,core.background_pixel),
	 XtRString, (XtPointer)"XtDefaultBackground"},
    {XtNbackgroundPixmap, XtCPixmap, XtRPixmap, sizeof(Pixmap),
         XtOffset(CoreWidget,core.background_pixmap),
	 XtRImmediate, (XtPointer)XtUnspecifiedPixmap},
    {XtNborderColor, XtCBorderColor, XtRPixel,sizeof(Pixel),
         XtOffset(CoreWidget,core.border_pixel),
         XtRString,(XtPointer)"XtDefaultForeground"},
    {XtNborderPixmap, XtCPixmap, XtRPixmap, sizeof(Pixmap),
         XtOffset(CoreWidget,core.border_pixmap),
	 XtRImmediate, (XtPointer)XtUnspecifiedPixmap},
    {XtNmappedWhenManaged, XtCMappedWhenManaged, XtRBoolean, sizeof(Boolean),
         XtOffset(CoreWidget,core.mapped_when_managed),
	 XtRImmediate, (XtPointer)True},
    {XtNtranslations, XtCTranslations, XtRTranslationTable,
        sizeof(XtTranslations), XtOffset(CoreWidget,core.tm.translations),
        XtRTranslationTable, (XtPointer)NULL},
    {XtNaccelerators, XtCAccelerators, XtRAcceleratorTable,
        sizeof(XtTranslations), XtOffset(CoreWidget,core.accelerators),
        XtRTranslationTable, (XtPointer)NULL}
    };

static void CoreInitialize();
static void CoreClassPartInitialize();
static void CoreDestroy();
static void CoreRealize();
static Boolean CoreSetValues();
static void CoreSetValuesAlmost();

static RectObjClassRec unNamedObjClassRec = {
  {
    /* superclass	  */	(WidgetClass)&rectObjClassRec,
    /* class_name	  */	"UnNamedObj",
    /* widget_size	  */	0,
    /* class_initialize   */    NULL,
    /* class_part_initialize*/	NULL,
    /* class_inited       */	FALSE,
    /* initialize	  */	NULL,
    /* initialize_hook    */	NULL,		
    /* realize		  */	XtInheritRealize,
    /* actions		  */	NULL,
    /* num_actions	  */	0,
    /* resources	  */	NULL,
    /* num_resources	  */	0,
    /* xrm_class	  */	NULLQUARK,
    /* compress_motion	  */	FALSE,
    /* compress_exposure  */	FALSE,
    /* compress_enterleave*/ 	FALSE,
    /* visible_interest	  */	FALSE,
    /* destroy		  */	NULL,
    /* resize		  */	NULL,
    /* expose		  */	NULL,
    /* set_values	  */	NULL,
    /* set_values_hook    */	NULL,			
    /* set_values_almost  */	XtInheritSetValuesAlmost,  
    /* get_values_hook    */	NULL,			
    /* accept_focus	  */	NULL,
    /* version		  */	XtVersion,
    /* callback_offsets   */    NULL,
    /* tm_table           */    NULL,
    /* query_geometry	    */  NULL,
    /* display_accelerator  */	NULL,
    /* extension	    */  NULL
  }
};


externaldef(widgetclassrec) WidgetClassRec widgetClassRec = {
{
    /* superclass         */    (WidgetClass)&unNamedObjClassRec,
    /* class_name         */    "Core",
    /* widget_size        */    sizeof(WidgetRec),
    /* class_initialize   */    NULL,
    /* class_part_initialize*/  CoreClassPartInitialize,
    /* class_inited       */    FALSE,
    /* initialize         */    CoreInitialize,
    /* initialize_hook    */    NULL,
    /* realize            */    CoreRealize,
    /* actions            */    NULL,
    /* num_actions        */    0,
    /* resources          */    resources,
    /* num_resources      */    XtNumber(resources),
    /* xrm_class          */    NULLQUARK,
    /* compress_motion    */    FALSE,
    /* compress_exposure  */    TRUE,
    /* compress_enterleave*/    FALSE,
    /* visible_interest   */    FALSE,
    /* destroy            */    CoreDestroy,
    /* resize             */    NULL,
    /* expose             */    NULL,
    /* set_values         */    CoreSetValues,
    /* set_values_hook    */    NULL,
    /* set_values_almost  */    CoreSetValuesAlmost,
    /* get_values_hook    */    NULL,
    /* accept_focus       */    NULL,
    /* version            */    XtVersion,
    /* callback_offsets   */    NULL,
    /* tm_table           */    NULL,
    /* query_geometry       */  NULL,
    /* display_accelerator  */  NULL,
    /* extension            */  NULL
  }
};
externaldef (WidgetClass) WidgetClass widgetClass = &widgetClassRec;

externaldef (WidgetClass) WidgetClass coreWidgetClass = &widgetClassRec;

/*
 * Start of Core methods
 */


static void CoreClassPartInitialize(wc)
    register WidgetClass wc;
{
    extern Opaque _CompileActionTable();

    /* We don't need to check for null super since we'll get to object
       eventually, and it had better define them!  */

    register WidgetClass super = wc->core_class.superclass;

    if (wc->core_class.realize == XtInheritRealize) {
	wc->core_class.realize = super->core_class.realize;
    }

    if (wc->core_class.accept_focus == XtInheritAcceptFocus) {
	wc->core_class.accept_focus = super->core_class.accept_focus;
    }

    if (wc->core_class.display_accelerator == XtInheritDisplayAccelerator) {
	wc->core_class.display_accelerator = 
		super->core_class.display_accelerator;
    }
    if (wc->core_class.tm_table == (char *) XtInheritTranslations) {
	wc->core_class.tm_table =
		wc->core_class.superclass->core_class.tm_table;
    } else if (wc->core_class.tm_table != NULL) {
	wc->core_class.tm_table =
	      (String)XtParseTranslationTable(wc->core_class.tm_table);
    }
    if (wc->core_class.actions != NULL) {
	/* Compile the action table into a more efficient form */
        wc->core_class.actions = (XtActionList) _CompileActionTable(
	    wc->core_class.actions, wc->core_class.num_actions);
    }
}
/* ARGSUSED */
static void CoreInitialize(requested_widget, new_widget)
    Widget   requested_widget;
    register Widget new_widget;
{
    XtTranslations save;
    new_widget->core.window = (Window) NULL;
    new_widget->core.visible = TRUE;
    new_widget->core.event_table = NULL;
    new_widget->core.popup_list = NULL;
    new_widget->core.num_popups = 0;
    new_widget->core.tm.proc_table = NULL;
    new_widget->core.tm.current_state = NULL;
    new_widget->core.tm.lastEventTime = 0;
    save = new_widget->core.tm.translations;
    new_widget->core.tm.translations =
	(XtTranslations)new_widget->core.widget_class->core_class.tm_table;
    if (save!= NULL) {
        switch ((int)(save->operation)) {
               case XtTableReplace:
                  new_widget->core.tm.translations = save;
                  break;
               case XtTableAugment:
                  XtAugmentTranslations(new_widget,save);
                  break;
               case XtTableOverride:
                  XtOverrideTranslations(new_widget,save);
                  break;
        }
     }

}

static void CoreRealize(widget, value_mask, attributes)
    Widget		 widget;
    Mask		 *value_mask;
    XSetWindowAttributes *attributes;
{
    XtCreateWindow(widget, (unsigned int) InputOutput,
	(Visual *) CopyFromParent, *value_mask, attributes);
} /* CoreRealize */

static void CoreDestroy (widget)
     Widget    widget;
{
    XtFree((char *) (widget->core.name));
    _XtFreeEventTable(&widget->core.event_table);
    XtFree((char *) widget->core.tm.proc_table);
    _XtUnregisterWindow(widget->core.window, widget);

    if (widget->core.popup_list != NULL)
        XtFree((char *)widget->core.popup_list);

} /* CoreDestroy */

/* ARGSUSED */
static Boolean CoreSetValues(old, reference, new)
    Widget old, reference, new;
{
    Boolean redisplay;
    Mask    window_mask;
    XSetWindowAttributes attributes;
    XtTranslations save;

    redisplay = FALSE;
    if  (old->core.tm.translations != new->core.tm.translations) {
        switch (new->core.tm.translations->operation) {
            case XtTableAugment:
                save = new->core.tm.translations;
                new->core.tm.translations = old->core.tm.translations;
                XtAugmentTranslations(new,save);
                break;
            case XtTableOverride:
                save = new->core.tm.translations;
                new->core.tm.translations = old->core.tm.translations;
                XtOverrideTranslations(new,save);
                break;
        }
    }       

    /* Check everything that depends upon window being realized */
    if (XtIsRealized(old)) {
	window_mask = 0;
	/* Check window attributes */
	if (old->core.background_pixel != new->core.background_pixel
	    && new->core.background_pixmap == XtUnspecifiedPixmap) {
	   attributes.background_pixel  = new->core.background_pixel;
	   window_mask |= CWBackPixel;
	   redisplay = TRUE;
	}	
	if (old->core.background_pixmap != new->core.background_pixmap) {
	   if (new->core.background_pixmap == XtUnspecifiedPixmap) {
	       window_mask |= CWBackPixel;
	       attributes.background_pixel  = new->core.background_pixel;
	   }
	   else {
	       attributes.background_pixmap = new->core.background_pixmap;
	       window_mask &= ~CWBackPixel;
	       window_mask |= CWBackPixmap;
	   }
	   redisplay = TRUE;
	}	
	if (old->core.border_pixel != new->core.border_pixel
	    && new->core.border_pixmap == XtUnspecifiedPixmap) {
	   attributes.border_pixel  = new->core.border_pixel;
	   window_mask |= CWBorderPixel;
       }
	if (old->core.border_pixmap != new->core.border_pixmap) {
	   if (new->core.border_pixmap == XtUnspecifiedPixmap) {
	       window_mask |= CWBorderPixel;
	       attributes.border_pixel  = new->core.border_pixel;
	   }
	   else {
	       attributes.border_pixmap = new->core.border_pixmap;
	       window_mask &= ~CWBorderPixel;
	       window_mask |= CWBorderPixmap;
	   }
       }
	if (old->core.depth != new->core.depth) {
	   XtAppWarningMsg(XtWidgetToApplicationContext(old),
		    "invalidDepth","setValues","XtToolkitError",
               "Can't change widget depth", (String *)NULL, (Cardinal *)NULL);
	   new->core.depth = old->core.depth;
	}
	if (old->core.colormap != new->core.colormap) {
	    window_mask |= CWColormap;
	    attributes.colormap = new->core.colormap;
	}
	if (window_mask != 0) {
	    /* Actually change X window attributes */
	    XChangeWindowAttributes(
		XtDisplay(new), XtWindow(new), window_mask, &attributes);
	}

	if (old->core.mapped_when_managed != new->core.mapped_when_managed) {
	    Boolean mapped_when_managed = new->core.mapped_when_managed;
	    new->core.mapped_when_managed = !mapped_when_managed;
	    XtSetMappedWhenManaged(new, mapped_when_managed);
	} 

	/* Translation table and state */
	if (old->core.tm.translations != new->core.tm.translations) {
	    XtUninstallTranslations((Widget)old);
	    new->core.tm.proc_table = NULL;
	    _XtBindActions(new, &new->core.tm);
	    _XtInstallTranslations((Widget) new, new->core.tm.translations);
	}
    } /* if realized */

    return redisplay;
} /* CoreSetValues */

/*ARGSUSED*/
static void CoreSetValuesAlmost(old, new, request, reply)
    Widget		old;
    Widget		new;
    XtWidgetGeometry    *request;
    XtWidgetGeometry    *reply;
{
    *request = *reply;
}
