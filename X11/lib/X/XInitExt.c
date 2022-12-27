#include "copyright.h"

/* $XConsortium: XInitExt.c,v 11.23 89/12/11 19:09:33 rws Exp $ */
/* Copyright  Massachusetts Institute of Technology 1987 */

#include "Xlibint.h"
#include "Xos.h"
#include <stdio.h>

extern Bool _XUnknownWireEvent();
extern Status _XUnknownNativeEvent();

/*
 * This routine is used to link a extension in so it will be called
 * at appropriate times.
 */

#if NeedFunctionPrototypes
XExtCodes *XInitExtension (
	Display *dpy,
	const char *name)
#else
XExtCodes *XInitExtension (dpy, name)
	Display *dpy;
	char *name;
#endif
{
	XExtCodes codes;	/* temp. place for extension information. */
	register _XExtension *ext;/* need a place to build it all */
	if (!XQueryExtension(dpy, name, 
		&codes.major_opcode, &codes.first_event,
		&codes.first_error)) return (NULL);

	LockDisplay (dpy);
	if (! (ext = (_XExtension *) Xcalloc (1, sizeof (_XExtension))) ||
	    ! (ext->name = Xmalloc((unsigned) strlen(name) + 1))) {
	    if (ext) Xfree((char *) ext);
	    UnlockDisplay(dpy);
	    return (XExtCodes *) NULL;
	}
	codes.extension = dpy->ext_number++;
	ext->codes = codes;
	(void) strcpy(ext->name, name);

	/* chain it onto the display list */	
	ext->next = dpy->ext_procs;
	dpy->ext_procs = ext;
	UnlockDisplay (dpy);

	return (&ext->codes);		/* tell him which extension */
}

XExtCodes *XAddExtension (dpy)
    Display *dpy;
{
    register _XExtension *ext;

    LockDisplay (dpy);
    if (! (ext = (_XExtension *) Xcalloc (1, sizeof (_XExtension)))) {
	UnlockDisplay(dpy);
	return (XExtCodes *) NULL;
    }
    ext->codes.extension = dpy->ext_number++;

    /* chain it onto the display list */
    ext->next = dpy->ext_procs;
    dpy->ext_procs = ext;
    UnlockDisplay (dpy);

    return (&ext->codes);		/* tell him which extension */
}

static _XExtension *XLookupExtension (dpy, extension)
	register Display *dpy;	/* display */
	register int extension;	/* extension number */
{
	register _XExtension *ext = dpy->ext_procs;
	while (ext != NULL) {
		if (ext->codes.extension == extension) return (ext);
		ext = ext->next;
	}
	return (NULL);
}

XExtData **XEHeadOfExtensionList(object)
    XEDataObject object;
{
    return *(XExtData ***)&object;
}

XAddToExtensionList(structure, ext_data)
    XExtData **structure;
    XExtData *ext_data;
{
    ext_data->next = *structure;
    *structure = ext_data;
}

XExtData *XFindOnExtensionList(structure, number)
    XExtData **structure;
    int number;
{
    XExtData *ext;

    ext = *structure;
    while (ext && (ext->number != number))
	ext = ext->next;
    return ext;
}

/*
 * Routines to hang procs on the extension structure.
 */
int (*XESetCreateGC(dpy, extension, proc))()
	Display *dpy;		/* display */
	int extension;		/* extension number */
	int (*proc)();		/* routine to call when GC created */
{
	register _XExtension *e;	/* for lookup of extension */
	register int (*oldproc)();
	if ((e = XLookupExtension (dpy, extension)) == NULL) return (NULL);
	LockDisplay(dpy);
	oldproc = e->create_GC;
	e->create_GC = proc;
	UnlockDisplay(dpy);
	return (oldproc);
}
int (*XESetCopyGC(dpy, extension, proc))()
	Display *dpy;		/* display */
	int extension;		/* extension number */
	int (*proc)();		/* routine to call when GC copied */
{
	register _XExtension *e;	/* for lookup of extension */
	register int (*oldproc)();
	if ((e = XLookupExtension (dpy, extension)) == NULL) return (NULL);
	LockDisplay(dpy);
	oldproc = e->copy_GC;
	e->copy_GC = proc;
	UnlockDisplay(dpy);
	return (oldproc);
}
int (*XESetFlushGC(dpy, extension, proc))()
	Display *dpy;		/* display */
	int extension;		/* extension number */
	int (*proc)();		/* routine to call when GC copied */
{
	register _XExtension *e;	/* for lookup of extension */
	register int (*oldproc)();
	if ((e = XLookupExtension (dpy, extension)) == NULL) return (NULL);
	LockDisplay(dpy);
	oldproc = e->flush_GC;
	e->flush_GC = proc;
	UnlockDisplay(dpy);
	return (oldproc);
}

int (*XESetFreeGC(dpy, extension, proc))()
	Display *dpy;		/* display */
	int extension;		/* extension number */
	int (*proc)();		/* routine to call when GC freed */
{
	register _XExtension *e;	/* for lookup of extension */
	register int (*oldproc)();
	if ((e = XLookupExtension (dpy, extension)) == NULL) return (NULL);
	LockDisplay(dpy);
	oldproc = e->free_GC;
	e->free_GC = proc;
	UnlockDisplay(dpy);
	return (oldproc);
}

int (*XESetCreateFont(dpy, extension, proc))()
	Display *dpy;		/* display */
	int extension;		/* extension number */
	int (*proc)();		/* routine to call when font created */
{
	register _XExtension *e;	/* for lookup of extension */
	register int (*oldproc)();
	if ((e = XLookupExtension (dpy, extension)) == NULL) return (NULL);
	LockDisplay(dpy);
	oldproc = e->create_Font;
	e->create_Font = proc;
	UnlockDisplay(dpy);
	return (oldproc);
}

int (*XESetFreeFont(dpy, extension, proc))()
	Display *dpy;		/* display */
	int extension;		/* extension number */
	int (*proc)();		/* routine to call when font freed */
{
	register _XExtension *e;	/* for lookup of extension */
	register int (*oldproc)();
	if ((e = XLookupExtension (dpy, extension)) == NULL) return (NULL);
	LockDisplay(dpy);
	oldproc = e->free_Font;
	e->free_Font = proc;
	UnlockDisplay(dpy);
	return (oldproc);
}

int (*XESetCloseDisplay(dpy, extension, proc))()
	Display *dpy;		/* display */
	int extension;		/* extension number */
	int (*proc)();		/* routine to call when display closed */
{
	register _XExtension *e;	/* for lookup of extension */
	register int (*oldproc)();
	if ((e = XLookupExtension (dpy, extension)) == NULL) return (NULL);
	LockDisplay(dpy);
	oldproc = e->close_display;
	e->close_display = proc;
	UnlockDisplay(dpy);
	return (oldproc);
}
Bool (*XESetWireToEvent(dpy, event_number, proc))()
	Display *dpy;		/* display */
	Bool (*proc)();		/* routine to call when converting event */
	int event_number;	/* event routine to replace */
{
	register Bool (*oldproc)();
	if (proc == NULL) proc = _XUnknownWireEvent;
	LockDisplay (dpy);
	oldproc = dpy->event_vec[event_number];
	dpy->event_vec[event_number] = proc;
	UnlockDisplay (dpy);
	return (oldproc);
}
Status (*XESetEventToWire(dpy, event_number, proc))()
	Display *dpy;		/* display */
	Status (*proc)();	/* routine to call when converting event */
	int event_number;	/* event routine to replace */
{
	register Status (*oldproc)();
	if (proc == NULL) proc = _XUnknownNativeEvent;
	LockDisplay (dpy);
	oldproc = dpy->wire_vec[event_number];
	dpy->wire_vec[event_number] = proc;
	UnlockDisplay(dpy);
	return (oldproc);
}
int (*XESetError(dpy, extension, proc))()
	Display *dpy;		/* display */
	int extension;		/* extension number */
	int (*proc)();		/* routine to call when X error happens */
{
	register _XExtension *e;	/* for lookup of extension */
	register int (*oldproc)();
	if ((e = XLookupExtension (dpy, extension)) == NULL) return (NULL);
	LockDisplay(dpy);
	oldproc = e->error;
	e->error = proc;
	UnlockDisplay(dpy);
	return (oldproc);
}
char *(*XESetErrorString(dpy, extension, proc))()
	Display *dpy;		/* display */
	int extension;		/* extension number */
	char *(*proc)();	/* routine to call when I/O error happens */
{
	register _XExtension *e;	/* for lookup of extension */
	register char *(*oldproc)();
	if ((e = XLookupExtension (dpy, extension)) == NULL) return (NULL);
	LockDisplay(dpy);
	oldproc = e->error_string;
	e->error_string = proc;
	UnlockDisplay(dpy);
	return (oldproc);
}
