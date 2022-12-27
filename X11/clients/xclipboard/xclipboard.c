/*
 * $XConsortium: xclipboard.c,v 1.15 89/12/15 20:11:09 keith Exp $
 *
 * Copyright 1989 Massachusetts Institute of Technology
 *
 * Permission to use, copy, modify, and distribute this software and its
 * documentation for any purpose and without fee is hereby granted, provided
 * that the above copyright notice appear in all copies and that both that
 * copyright notice and this permission notice appear in supporting
 * documentation, and that the name of M.I.T. not be used in advertising
 * or publicity pertaining to distribution of the software without specific,
 * written prior permission.  M.I.T. makes no representations about the
 * suitability of this software for any purpose.  It is provided "as is"
 * without express or implied warranty.
 *
 * M.I.T. DISCLAIMS ALL WARRANTIES WITH REGARD TO THIS SOFTWARE, INCLUDING ALL
 * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS, IN NO EVENT SHALL M.I.T.
 * BE LIABLE FOR ANY SPECIAL, INDIRECT OR CONSEQUENTIAL DAMAGES OR ANY DAMAGES
 * WHATSOEVER RESULTING FROM LOSS OF USE, DATA OR PROFITS, WHETHER IN AN ACTION
 * OF CONTRACT, NEGLIGENCE OR OTHER TORTIOUS ACTION, ARISING OUT OF OR IN 
 * CONNECTION WITH THE USE OR PERFORMANCE OF THIS SOFTWARE.
 *
 * Author:  Ralph Swick, DEC/Project Athena
 * Updated for R4:  Chris D. Peterson,  MIT X Consortium.
 * Reauthored by: Keith Packard, MIT X Consortium.
 */

/* $XConsortium: xclipboard.c,v 1.15 89/12/15 20:11:09 keith Exp $ */

#include <stdio.h>
#include <X11/Intrinsic.h>
#include <X11/StringDefs.h>
#include <X11/Xatom.h>

#include <X11/Xmu/Atoms.h>
#include <X11/Xmu/StdSel.h>

#include <X11/Xaw/Form.h>
#include <X11/Xaw/Command.h>
#include <X11/Xaw/AsciiText.h>
#include <X11/Xaw/Cardinals.h>

#define Command commandWidgetClass
#define Text    asciiTextWidgetClass

#define INFINITY 10000000	/* pretty big, huh? */

typedef struct _Clip {
    struct _Clip    *next, *prev;
    char	    *clip;
    int		    avail;
} ClipRec, *ClipPtr;

extern char *malloc ();

static long TextLength (w)
    Widget  w;
{
    return XawTextSourceScan (XawTextGetSource (w),
			      (XawTextPosition) 0,
 			      XawstAll, XawsdRight, 1, TRUE);
}

SaveClip (w, clip)
    Widget  w;
    ClipPtr clip;
{
    Arg	    args[1];
    char    *data;
    int	    len;
    Widget  source;

    source = XawTextGetSource (w);
    XtSetArg (args[0], XtNstring, &data);
    XtGetValues (source, args, 1);
    len = strlen (data);
    if (len >= clip->avail)
    {
	if (clip->clip)
	    free (clip->clip);
	clip->clip = malloc (len + 1);
	if (!clip->clip)
	    clip->avail = 0;
	else
	    clip->avail = len + 1;
    }
    if (clip->avail)
    {
	strcpy (clip->clip, data);
    }
}

RestoreClip (w, clip)
    Widget  w;
    ClipPtr clip;
{
    Arg	    args[1];
    Widget  source;

    source = XawTextGetSource (w);
    XtSetArg (args[0], XtNstring, clip->clip);
    XtSetValues (source, args, 1);
}

ClipPtr
NewClip (w, old)
    Widget  w;
    ClipPtr old;
{
    ClipPtr newClip;

    newClip = (ClipPtr) malloc (sizeof (ClipRec));
    if (!newClip)
	return newClip;
    newClip->clip = 0;
    newClip->avail = 0;
    newClip->prev = old;
    newClip->next = NULL;
    if (old)
    {
	newClip->next = old->next;
	old->next = newClip;
    }
    return newClip;
}

DeleteClip (w, clip)
    Widget  w;
    ClipPtr clip;
{
    if (clip->prev)
	clip->prev->next = clip->next;
    if (clip->next)
	clip->next->prev = clip->prev;
    if (clip->clip)
	free (clip->clip);
    free ((char *) clip);
}

static ClipPtr	currentClip;
static Widget	text, nextButton, prevButton;

static void set_button_state ()
{
    Boolean prevvalid, nextvalid;
    Arg arg;

    prevvalid = False;
    nextvalid = False;
    if (currentClip)
    {
	prevvalid = currentClip->prev != NULL;
	nextvalid = currentClip->next != NULL;
    }
    arg.name = XtNsensitive;
    arg.value = (XtArgVal) prevvalid;
    XtSetValues (prevButton, &arg, ONE);
    arg.value = (XtArgVal) nextvalid;
    XtSetValues (nextButton, &arg, ONE);
}

static void
NextCurrentClip ()
{
    if (currentClip && currentClip->next)
    {
	SaveClip (text, currentClip);
	currentClip = currentClip->next;
	RestoreClip (text, currentClip);
	set_button_state ();
    }
}

static void
PrevCurrentClip ()
{
    if (currentClip && currentClip->prev)
    {
	SaveClip (text, currentClip);
	currentClip = currentClip->prev;
	RestoreClip (text, currentClip);
	set_button_state ();
    }
}

static void
DeleteCurrentClip ()
{
    ClipPtr newCurrent;
    if (currentClip)
    {
	if (currentClip->prev)
	{
	    newCurrent = currentClip->prev;
	}
	else
	{
	    newCurrent = currentClip->next;
	}
	DeleteClip (text, currentClip);
	currentClip = newCurrent;
	if (currentClip)
	    RestoreClip (text, currentClip);
	else
	{
	    EraseTextWidget ();
	}
	set_button_state ();
    }
}

static void
Quit ()
{
    XtCloseDisplay  (XtDisplay (text));
    exit (0);
}

static void
NewCurrentClip ()
{
    NewCurrentClipContents ("", 0);
}

NewCurrentClipContents (data, len)
    char    *data;
    int	    len;
{
    XawTextBlock textBlock;
    ClipPtr newCurrent;

    if (!currentClip && TextLength (text))
	currentClip = NewClip (text, (ClipPtr) 0);
    if (currentClip)
	SaveClip (text, currentClip);
    /* append new clips at the end */
    while (currentClip && currentClip->next)
	currentClip = currentClip->next;
    newCurrent = NewClip (text, currentClip);
    
    currentClip = newCurrent;

    textBlock.ptr = data;
    textBlock.firstPos = 0;
    textBlock.length = len;
    textBlock.format = FMT8BIT;
    if (XawTextReplace(text, 0, TextLength (text), &textBlock))
	XBell( XtDisplay(text), 0);
    set_button_state ();
}

EraseTextWidget ()
{
    XawTextBlock block;

    block.ptr = "";
    block.length = 0;
    block.firstPos = 0;
    block.format = FMT8BIT;

    XawTextReplace(text, 0, INFINITY, &block);
    /* If this fails, too bad. */
}


XtActionsRec xclipboard_actions[] = {
    "NewClip", NewCurrentClip,
    "NextClip",	NextCurrentClip,
    "PrevClip", PrevCurrentClip,
    "DeleteClip", DeleteCurrentClip,
    "Quit", Quit,
};

static XrmOptionDescRec table[] = {
    {"-w",	    "*text*wrap",		XrmoptionNoArg,  "Word"},
    {"-nw",	    "*text*wrap",		XrmoptionNoArg,  "Never"},
};

static void	LoseSelection ();
static void	InsertClipboard ();
static Boolean	ConvertSelection();

static void 
InsertClipboard(w, client_data, selection, type, value, length, format)
Widget w;
caddr_t client_data;
Atom *selection, *type;
caddr_t value;
unsigned long *length;
int *format;
{
    if (*type == 0 /*XT_CONVERT_FAIL*/ || *length == 0) {
	XBell( XtDisplay(w), 0 );
	return;
    }
    
    NewCurrentClipContents ((char *) value, *length);

    XtOwnSelection(w, XA_CLIPBOARD(XtDisplay(w)), CurrentTime,
		   ConvertSelection, LoseSelection, NULL);

    XtFree(value);
}

static Boolean ConvertSelection(w, selection, target,
				type, value, length, format)
    Widget w;
    Atom *selection, *target, *type;
    caddr_t *value;
    unsigned long *length;
    int *format;
{
    Display* d = XtDisplay(w);
    XSelectionRequestEvent* req =
	XtGetSelectionRequest(w, *selection, (XtRequestId)NULL);

    if (*target == XA_TARGETS(d)) {
	Atom* targetP;
	Atom* std_targets;
	unsigned long std_length;
	XmuConvertStandardSelection(w, req->time, selection, target, type,
				  (caddr_t*)&std_targets, &std_length, format);
	*value = XtMalloc(sizeof(Atom)*(std_length + 5));
	targetP = *(Atom**)value;
	*targetP++ = XA_STRING;
	*targetP++ = XA_TEXT(d);
	*targetP++ = XA_LENGTH(d);
	*targetP++ = XA_LIST_LENGTH(d);
	*targetP++ = XA_CHARACTER_POSITION(d);
	*length = std_length + (targetP - (*(Atom **) value));
	bcopy((char*)std_targets, (char*)targetP, sizeof(Atom)*std_length);
	XtFree((char*)std_targets);
	*type = XA_ATOM;
	*format = 32;
	return True;
    }

    if (*target == XA_LIST_LENGTH(d) ||
	*target == XA_LENGTH(d))
    {
    	long * temp;
    	
    	temp = (long *) XtMalloc(sizeof(long));
    	if (*target == XA_LIST_LENGTH(d))
      	  *temp = 1L;
    	else			/* *target == XA_LENGTH(d) */
      	  *temp = (long) TextLength (w);
    	
    	*value = (caddr_t) temp;
    	*type = XA_INTEGER;
    	*length = 1L;
    	*format = 32;
    	return True;
    }
    
    if (*target == XA_CHARACTER_POSITION(d))
    {
    	long * temp;
    	
    	temp = (long *) XtMalloc(2 * sizeof(long));
    	temp[0] = (long) 0;
    	temp[1] = TextLength (w);
    	*value = (caddr_t) temp;
    	*type = XA_SPAN(d);
    	*length = 2L;
    	*format = 32;
    	return True;
    }
    
    if (*target == XA_STRING ||
      *target == XA_TEXT(d) ||
      *target == XA_COMPOUND_TEXT(d))
    {
	extern char *_XawTextGetSTRING();
    	if (*target == XA_COMPOUND_TEXT(d))
	    *type = *target;
    	else
	    *type = XA_STRING;
	*length = TextLength (w);
    	*value = _XawTextGetSTRING((TextWidget) w, 0, *length);
    	*format = 8;
    	return True;
    }
    
    if (XmuConvertStandardSelection(w, req->time, selection, target, type,
				    value, length, format))
	return True;

    return False;
}

static void LoseSelection(w, selection)
    Widget w;
    Atom *selection;
{
    XtGetSelectionValue(w, *selection, XA_STRING, InsertClipboard,
			NULL, CurrentTime);
}

/*ARGSUSED*/
static Boolean RefuseSelection(w, selection, target,
			       type, value, length, format)
    Widget w;
    Atom *selection, *target, *type;
    caddr_t *value;
    unsigned long *length;
    int *format;
{
    return False;
}

/*ARGSUSED*/
static void LoseManager(w, selection)
    Widget w;
    Atom *selection;
{
    XtError("another clipboard has taken over control\n");
}

void
main(argc, argv)
int argc;
char **argv;
{
    Arg args[2];
    Widget top, parent, quit, delete, new;
    Atom manager;

    top = XtInitialize( "xclipboard", "XClipboard", table, XtNumber(table),
			  &argc, argv);

    XtAddActions (xclipboard_actions, XtNumber (xclipboard_actions));
    /* CLIPBOARD_MANAGER is a non-standard mechanism */
    manager = XInternAtom(XtDisplay(top), "CLIPBOARD_MANAGER", False);
    if (XGetSelectionOwner(XtDisplay(top), manager))
	XtError("another clipboard is already running\n");

    parent = XtCreateManagedWidget("form", formWidgetClass, top, NULL, ZERO);
    quit = XtCreateManagedWidget("quit", Command, parent, NULL, ZERO);
    delete = XtCreateManagedWidget("delete", Command, parent, NULL, ZERO);
    new = XtCreateManagedWidget("new", Command, parent, NULL, ZERO);
    nextButton = XtCreateManagedWidget("next", Command, parent, NULL, ZERO);
    prevButton = XtCreateManagedWidget("prev", Command, parent, NULL, ZERO);

    XtSetArg(args[0], XtNtype, XawAsciiString);
    XtSetArg(args[1], XtNeditType, XawtextEdit);
    text = XtCreateManagedWidget( "text", Text, parent, args, TWO);

    set_button_state ();

    XtRealizeWidget(top);

    XtOwnSelection(text, manager, CurrentTime,
		   RefuseSelection, LoseManager, NULL);
    XtOwnSelection(text, XA_CLIPBOARD(XtDisplay(text)), CurrentTime,
		   ConvertSelection, LoseSelection, NULL);

    XtMainLoop();
}
