/*
 * $XConsortium: xcutsel.c,v 1.11 89/12/08 13:25:23 swick Exp $
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
 */

#ifndef lint
static char rcsid[] = "$XConsortium: xcutsel.c,v 1.11 89/12/08 13:25:23 swick Exp $";
#endif /* lint */

#include <stdio.h>
#include <X11/Intrinsic.h>
#include <X11/StringDefs.h>
#include <X11/Xatom.h>

#include <X11/Xmu/Atoms.h>
#include <X11/Xmu/StdSel.h>

#include <X11/Xaw/Command.h>
#include <X11/Xaw/Box.h>
#include <X11/Xaw/Cardinals.h>

static XrmOptionDescRec options[] = {
    {"-selection", "selection",	XrmoptionSepArg, NULL},
    {"-select",    "selection",	XrmoptionSepArg, NULL},
    {"-sel",	   "selection",	XrmoptionSepArg, NULL},
    {"-s",	   "selection",	XrmoptionSepArg, NULL},
    {"-cutbuffer", "cutBuffer",	XrmoptionSepArg, NULL},
};


struct _app_resources {
    String  selection_name;
    int	    buffer;
    Atom    selection;
    char*   value;
    int     length;
} app_resources;

static XtResource resources[] = {
    {"selection", "Selection", XtRString, sizeof(String),
       XtOffset(struct _app_resources*, selection_name), XtRString, "PRIMARY"},
    {"cutBuffer", "CutBuffer", XtRInt, sizeof(int),
       XtOffset(struct _app_resources*, buffer), XtRImmediate, (caddr_t)0},
};

typedef struct {
    Widget button;
    Boolean is_on;
} ButtonState;

static ButtonState state;

Syntax(call)
	char *call;
{
    fprintf (stderr, "usage:  %s [-selection name] [-cutbuffer number]\n", 
	     call);
    exit (1);
}


static void StoreBuffer(w, client_data, selection, type, value, length, format)
    Widget w;
    caddr_t client_data;
    Atom *selection, *type;
    caddr_t value;
    unsigned long *length;
    int *format;
{

    if (*type == 0 || *type == XT_CONVERT_FAIL || *length == 0) {
	XBell( XtDisplay(w), 0 );
	return;
    }

    XStoreBuffer( XtDisplay(w), (char*)value, (int)(*length),
		  app_resources.buffer );
   
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
	*value = XtMalloc(sizeof(Atom)*(std_length + 4));
	targetP = *(Atom**)value;
	*length = std_length + 4;
	*targetP++ = XA_STRING;
	*targetP++ = XA_TEXT(d);
	*targetP++ = XA_LENGTH(d);
	*targetP++ = XA_LIST_LENGTH(d);
/*
	*targetP++ = XA_CHARACTER_POSITION(d);
*/
	bcopy((char*)std_targets, (char*)targetP, sizeof(Atom)*std_length);
	XtFree((char*)std_targets);
	*type = XA_ATOM;
	*format = 32;
	return True;
    }
    if (*target == XA_STRING || *target == XA_TEXT(d)) {
	*type = XA_STRING;
	*value = app_resources.value;
	*length = app_resources.length;
	*format = 8;
	return True;
    }
    if (*target == XA_LIST_LENGTH(d)) {
	long *temp = (long *) XtMalloc (sizeof(long));
	*temp = 1L;
	*value = (caddr_t) temp;
	*type = XA_INTEGER;
	*length = 1;
	*format = 32;
	return True;
    }
    if (*target == XA_LENGTH(d)) {
	long *temp = (long *) XtMalloc (sizeof(long));
	*temp = app_resources.length;
	*value = (caddr_t) temp;
	*type = XA_INTEGER;
	*length = 1;
	*format = 32;
	return True;
    }
#ifdef notdef
    if (*target == XA_CHARACTER_POSITION(d)) {
	long *temp = (long *) XtMalloc (2 * sizeof(long));
	temp[0] = ctx->text.s.left + 1;
	temp[1] = ctx->text.s.right;
	*value = (caddr_t) temp;
	*type = XA_SPAN(d);
	*length = 2;
	*format = 32;
	return True;
    }
#endif /* notdef */
    if (XmuConvertStandardSelection(w, req->time, selection, target, type,
				    value, length, format))
	return True;

    /* else */
    return False;
}


static void SetButton(state, on)
    ButtonState *state;
    Boolean on;
{
    if (state->is_on != on) {
	Arg args[2];
	Pixel fg, bg;
	XtSetArg( args[0], XtNforeground, &fg );
	XtSetArg( args[1], XtNbackground, &bg );
	XtGetValues( state->button, args, TWO );
	args[0].value = (XtArgVal)bg;
	args[1].value = (XtArgVal)fg;
	XtSetValues( state->button, args, TWO );
	state->is_on = on;
    }
}


static void LoseSelection(w, selection)
    Widget w;
    Atom *selection;
{
    XtFree( app_resources.value );
    app_resources.value = NULL;
    SetButton(&state, False);
}


/* ARGSUSED */
static void Quit(w, closure, callData)
    Widget w;
    caddr_t closure;		/* unused */
    caddr_t callData;		/* unused */
{
    XtCloseDisplay( XtDisplay(w) );
    exit(0);
}


/* ARGSUSED */
static void GetSelection(w, closure, callData)
    Widget w;
    caddr_t closure;		/* unused */
    caddr_t callData;		/* unused */
{
    XtGetSelectionValue(w, app_resources.selection, XA_STRING,
			StoreBuffer, NULL,
			XtLastTimestampProcessed(XtDisplay(w)));
}


/* ARGSUSED */
static void GetBuffer(w, closure, callData)
    Widget w;
    caddr_t closure;
    caddr_t callData;		/* unused */
{
    XtFree( app_resources.value );
    app_resources.value =
	XFetchBuffer(XtDisplay(w), &app_resources.length, app_resources.buffer);
    if (app_resources.value != NULL) {
	if (XtOwnSelection(w, app_resources.selection,
			   XtLastTimestampProcessed(XtDisplay(w)),
			   ConvertSelection, LoseSelection, NULL))
	    SetButton((ButtonState*)closure, True);
    }
}


void main(argc, argv)
    unsigned int argc;
    char **argv;
{
    char label[100];
    Widget box, button;
    Widget shell =
	XtInitialize( "xcutsel", "XCutsel", options, XtNumber(options),
		      &argc, argv );
    XrmDatabase rdb = XtDatabase(XtDisplay(shell));

    if (argc != 1) Syntax(argv[0]);

    XtGetApplicationResources( shell, (caddr_t)&app_resources,
			       resources, XtNumber(resources),
			       NULL, ZERO );

    app_resources.value = NULL;
    XmuInternStrings( XtDisplay(shell), &app_resources.selection_name, ONE,
		      &app_resources.selection );

    box = XtCreateManagedWidget("box", boxWidgetClass, shell, NULL, ZERO);

    button =
	XtCreateManagedWidget("quit", commandWidgetClass, box, NULL, ZERO);
	XtAddCallback( button, XtNcallback, Quit, NULL );

    /* %%% hack alert... */
    sprintf(label, "*label:copy %s to %d",
	    app_resources.selection_name,
	    app_resources.buffer);
    XrmPutLineResource( &rdb, label );

    button =
	XtCreateManagedWidget("sel-cut", commandWidgetClass, box, NULL, ZERO);
	XtAddCallback( button, XtNcallback, GetSelection, NULL );

    sprintf(label, "*label:copy %d to %s",
	    app_resources.buffer,
	    app_resources.selection_name);
    XrmPutLineResource( &rdb, label );

    button =
	XtCreateManagedWidget("cut-sel", commandWidgetClass, box, NULL, ZERO);
	XtAddCallback( button, XtNcallback, GetBuffer, (caddr_t)&state );
 	state.button = button;
	state.is_on = False;
   
    XtRealizeWidget(shell);
    XtMainLoop();
}
