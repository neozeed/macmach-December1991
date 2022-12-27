#ifndef lint
static char Xrcsid[] = "$XConsortium: TMstate.c,v 1.98 90/04/10 15:58:55 swick Exp $";
/* $oHeader: TMstate.c,v 1.5 88/09/01 17:17:29 asente Exp $ */
#endif /* lint */
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

/* TMstate.c -- maintains the state table of actions for the translation 
 *              manager.
 */
#include <X11/Xlib.h>
#define XK_MISCELLANY
#define XK_LATIN1
#include <X11/keysymdef.h>
#include "StringDefs.h"
#include <stdio.h>
#include "IntrinsicI.h"

static String XtNtranslationError = "translationError";

/* usual number of expected keycodes in XtKeysymToKeycodeList */
#define KEYCODE_ARRAY_SIZE 10

static String PrintState();

typedef struct _GrabActionRec {
    struct _GrabActionRec* next;
    XtActionProc action_proc;
    Boolean owner_events;
    unsigned int event_mask;
    int pointer_mode, keyboard_mode;
} GrabActionRec;

static GrabActionRec *grabActionList = NULL;

#define StringToAction(string)	((XtAction) StringToQuark(string))

#define STR_THRESHOLD 25
#define STR_INCAMOUNT 100
#define CHECK_STR_OVERFLOW \
    if (str - *buf > *len - STR_THRESHOLD) {			  \
	String old = *buf;					  \
	*buf = XtRealloc(old, (Cardinal)(*len += STR_INCAMOUNT)); \
	str = str - old + *buf;					  \
    }

#define ExpandForChars( nchars ) \
    if (str - *buf > *len - STR_THRESHOLD - nchars) { 		\
	String old = *buf;					\
	*buf = XtRealloc(old,					\
	    (Cardinal)(*len += STR_INCAMOUNT + nchars));	\
	str = str - old + *buf;					\
    }

#define ExpandToFit(more) \
    {								\
	int l = strlen(more);					\
	ExpandForChars(l);					\
    }

#define _InitializeKeysymTables(dpy, pd) \
    if (pd->keysyms == NULL) \
        _XtBuildKeysymTables(dpy, pd)
   

static void FreeActions(action)
  register ActionPtr action;
{
    while (action != NULL) {
	register ActionPtr next = action->next;

	if (action->params != NULL) {
	    register Cardinal i;

	    for (i=0; i<action->num_params; i++) XtFree(action->params[i]);
	    XtFree((char *)action->params);
	}

	XtFree((char *)action);
	action = next;
    }
}

static String PrintModifiers(buf, len, str, mask, mod)
    String *buf;
    int *len;
    String str;
    unsigned long mask, mod;
{
    Boolean notfirst = False;
    CHECK_STR_OVERFLOW;

    if (mask == ~0L && mod == 0) {
	*str++ = '!';
	*str = '\0';
	return str;
    }

#define PRINTMOD(modmask,modstring) \
    if (mask & modmask) {		 \
	if (! (mod & modmask)) {	 \
	    *str++ = '~';		 \
	    notfirst = True;		 \
	}				 \
	else if (notfirst)		 \
	    *str++ = ' ';		 \
	else notfirst = True;		 \
	strcpy(str, modstring);		 \
	str += strlen(str);		 \
    }

    PRINTMOD(ShiftMask, "Shift");
    PRINTMOD(ControlMask, "Ctrl");	/* name is not CtrlMask... */
    PRINTMOD(LockMask, "Lock");
    PRINTMOD(Mod1Mask, "Mod1");
    PRINTMOD(Mod2Mask, "Mod2");
    PRINTMOD(Mod3Mask, "Mod3");
    PRINTMOD(Mod4Mask, "Mod4");
    PRINTMOD(Mod5Mask, "Mod5");
    PRINTMOD(Button1Mask, "Button1");
    PRINTMOD(Button2Mask, "Button2");
    PRINTMOD(Button3Mask, "Button3");
    PRINTMOD(Button4Mask, "Button4");
    PRINTMOD(Button5Mask, "Button5");

#undef PRINTMOD

    return str;
}

static String PrintEventType(buf, len, str, event)
    String *buf;
    int *len;
    register String str;
    unsigned long event;
{
    CHECK_STR_OVERFLOW;
    switch (event) {
#define PRINTEVENT(event) case event: (void) strcpy(str, "<event>"); break;
	PRINTEVENT(KeyPress)
	PRINTEVENT(KeyRelease)
	PRINTEVENT(ButtonPress)
	PRINTEVENT(ButtonRelease)
	PRINTEVENT(MotionNotify)
	PRINTEVENT(EnterNotify)
	PRINTEVENT(LeaveNotify)
	PRINTEVENT(FocusIn)
	PRINTEVENT(FocusOut)
	PRINTEVENT(KeymapNotify)
	PRINTEVENT(Expose)
	PRINTEVENT(GraphicsExpose)
	PRINTEVENT(NoExpose)
	PRINTEVENT(VisibilityNotify)
	PRINTEVENT(CreateNotify)
	PRINTEVENT(DestroyNotify)
	PRINTEVENT(UnmapNotify)
	PRINTEVENT(MapNotify)
	PRINTEVENT(MapRequest)
	PRINTEVENT(ReparentNotify)
	PRINTEVENT(ConfigureNotify)
	PRINTEVENT(ConfigureRequest)
	PRINTEVENT(GravityNotify)
	PRINTEVENT(ResizeRequest)
	PRINTEVENT(CirculateNotify)
	PRINTEVENT(CirculateRequest)
	PRINTEVENT(PropertyNotify)
	PRINTEVENT(SelectionClear)
	PRINTEVENT(SelectionRequest)
	PRINTEVENT(SelectionNotify)
	PRINTEVENT(ColormapNotify)
	PRINTEVENT(ClientMessage)
	case _XtEventTimerEventType: (void) strcpy(str,"<EventTimer>"); break;
	default: (void) sprintf(str, "<0x%x>", (int) event);
#undef PRINTEVENT
    }
    str += strlen(str);
    return str;
}

static String PrintCode(buf, len, str, mask, code)
    String *buf;
    int *len;
    register String str;
    unsigned long mask, code;
{
    CHECK_STR_OVERFLOW;
    if (mask != 0) {
	if (mask != (unsigned long)~0L)
	    (void) sprintf(str, "0x%lx:0x%lx", mask, code);
	else (void) sprintf(str, /*"0x%lx"*/ "%d", code);
	str += strlen(str);
    }
    return str;
}

static String PrintKeysym(buf, len, str, keysym)
    String *buf;
    int *len;
    register String str;
    KeySym keysym;
{
    String keysymName;

    if (keysym == 0) return str;

    CHECK_STR_OVERFLOW;
    keysymName = XKeysymToString(keysym);
    if (keysymName == NULL)
	return PrintCode(buf,len,str,(unsigned long)~0L,(unsigned long)keysym);

    strcpy(str, keysymName);
    return str + strlen(str);
}

static String PrintAtom(buf, len, str, dpy, atom)
    String *buf;
    int *len;
    register String str;
    Display *dpy;
    Atom atom;
{
    String atomName;

    if (atom == 0) return str;

    if (dpy == NULL)
	atomName = NULL;
    else
	atomName = XGetAtomName(dpy, atom);

    if (atomName == NULL)
	return PrintCode(buf, len, str,(unsigned long)~0L,(unsigned long)atom);

    ExpandToFit( atomName );
    strcpy(str, atomName);
    return str + strlen(str);
}

static String PrintLateModifiers(buf, len, str, lateModifiers)
    String *buf;
    int *len;
    register String str;
    LateBindingsPtr lateModifiers;
{
    for (; lateModifiers->keysym != NULL; lateModifiers++) {
	CHECK_STR_OVERFLOW;
	if (lateModifiers->knot) {
	    *str++ = '~';
	} else {
	    *str++ = ' ';
	}
	strcpy(str, XKeysymToString(lateModifiers->keysym));
	str += strlen(str);
	if (lateModifiers->pair) {
	    *(str -= 2) = '\0';	/* strip "_L" */
	    lateModifiers++;	/* skip _R keysym */
	}
    }
    return str;
}

static String PrintEvent(buf, len, str, event, dpy)
    String *buf;
    int *len;
    register String str;
    register Event *event;
    Display *dpy;
{
    if (event->standard) *str++ = ':';

    str = PrintModifiers(buf, len, str, event->modifierMask, event->modifiers);
    if (event->lateModifiers != NULL)
	str = PrintLateModifiers(buf, len, str, event->lateModifiers);
    str = PrintEventType(buf, len, str, event->eventType);
    switch (event->eventType) {
      case KeyPress:
      case KeyRelease:
	str = PrintKeysym(buf, len, str, (KeySym)event->eventCode);
	break;

      case PropertyNotify:
      case SelectionClear:
      case SelectionRequest:
      case SelectionNotify:
      case ClientMessage:
	str = PrintAtom(buf, len, str, dpy, (Atom)event->eventCode);
	break;

      default:
	str = PrintCode(buf, len, str, event->eventCodeMask, event->eventCode);
    }
    return str;
}

static String PrintEventSeq(buf, len, str, eventSeq, dpy)
    String *buf;
    int *len;
    register String str;
    register EventSeqPtr eventSeq;
    Display *dpy;
{
    while (eventSeq != NULL) {
	str = PrintEvent(buf, len, str, &eventSeq->event, dpy);
	if ((eventSeq = eventSeq->next) != NULL)
	    *str++ = ',';
    }
    return str;
}

static String PrintParams(buf, len, str, params, num_params)
    String *buf;
    int *len;
    register String str, *params;
    Cardinal num_params;
{
    register Cardinal i;
    for (i = 0; i<num_params; i++) {
	ExpandToFit( params[i] );
	if (i != 0) {
	    *str++ = ',';
	    *str++ = ' ';
	}
	*str++ = '"';
	strcpy(str, params[i]);
	str += strlen(str);
	*str++ = '"';
    }
    *str = '\0';
    return str;
}

static String PrintActions(buf, len, str, actions, stateTable)
    String *buf;
    int *len;
    register String str;
    register ActionPtr actions;
    XtTranslations stateTable;
{
    while (actions != NULL) {
	String proc;
	*str++ = ' ';
	if (actions->index >= 0) {
	    /* normal translation */
	    proc = XrmQuarkToString( stateTable->quarkTable[actions->index] );
	} else {
	    /* accelerator */
	    int temp = -(actions->index+1);
	    if (stateTable->accProcTbl != NULL) {
		Widget w = stateTable->accProcTbl[temp].widget;
		if (w != NULL) {
		    String name = XtName(w);
		    int nameLen = strlen(name);
		    ExpandForChars( nameLen );
		    bcopy( name, str, nameLen );
		    str += nameLen;
		    *str++ = '`';
		}
	    }
	    proc = XrmQuarkToString( stateTable->accQuarkTable[temp] );
	}
	ExpandToFit( proc );
	strcpy(str, proc);
	str += strlen(proc);
	*str++ = '(';
	str = PrintParams(buf, len, str, actions->params, actions->num_params);
	*str++ = ')';
	actions = actions->next;
    }
    *str = '\0';
    return str;
}

static Boolean ComputeLateBindings(event,eventSeq,computed,computedMask)
    Event *event;
    TMEventPtr eventSeq;
    Modifiers *computed,*computedMask;
{
    int i,j,ref;
    ModToKeysymTable* temp;
    XtPerDisplay perDisplay;
    Display *dpy;
    Boolean found;
    KeySym tempKeysym = NoSymbol;
    dpy = eventSeq->xev->xany.display;
    perDisplay = _XtGetPerDisplay(dpy);
    if (perDisplay == NULL) {
        XtAppWarningMsg(XtDisplayToApplicationContext(dpy),
		"displayError","invalidDisplay",XtCXtToolkitError,
            "Can't find display structure",
            (String *)NULL, (Cardinal *)NULL);
         return FALSE;
    }
    _InitializeKeysymTables(dpy, perDisplay);
    for (ref=0;event->lateModifiers[ref].keysym != NULL;ref++) {
        found = FALSE;
        for (i=0;i<8;i++) {
            temp = &(perDisplay->modsToKeysyms[i]);
            for (j=0;j<temp->count;j++){
                if (perDisplay->modKeysyms[temp->index+j] ==
                      event->lateModifiers[ref].keysym) {
                    *computedMask = *computedMask | temp->mask;
                    if (!event->lateModifiers[ref].knot)
                        *computed |= temp->mask;
                    tempKeysym = event->lateModifiers[ref].keysym;
                    found = TRUE; break;
                }
            }
            if (found) break;
        }
        if (!found  && !event->lateModifiers[ref].knot)
            if (!event->lateModifiers[ref].pair && (tempKeysym == NoSymbol))
                return FALSE;
        /* if you didn't find the modifier and the modifier must be
           asserted then return FALSE. If you didn't find the modifier
           and the modifier must be off, then it is OK . Don't
           return FALSE if this is the first member of a pair or if
           it is the second member of a pair when the first member
           was bound to a modifier */
    if (!event->lateModifiers[ref].pair) tempKeysym = NoSymbol;
    }
    return TRUE;
}

Boolean _XtRegularMatch(event,eventSeq)
    Event *event;
    TMEventPtr eventSeq;
{
    Modifiers computed =0;
    Modifiers computedMask =0;
    Boolean resolved = TRUE;
    if (event->eventCode != (eventSeq->event.eventCode &
               event->eventCodeMask)) return FALSE;
    if (event->lateModifiers != NULL)
        resolved = ComputeLateBindings(event,eventSeq,&computed,&computedMask);
    if (!resolved) return FALSE;
    computed |= event->modifiers;
    computedMask |= event->modifierMask;

    return ( (computed & computedMask) ==
          (eventSeq->event.modifiers & computedMask));

}


static TMContext AllocTMContext(dpy)
    Display *dpy;
{
    extern char* _XtHeapAlloc();
    XtPerDisplay pd = _XtGetPerDisplay(dpy);
    TMContext ctx =
	(TMContext)_XtHeapAlloc( &pd->heap, (unsigned)sizeof(TMContextRec) );
    pd->tm_context = ctx;
    return ctx;
}


Boolean _XtMatchUsingDontCareMods(event,eventSeq)
    Event *event;
    TMEventPtr eventSeq;
{
    Modifiers modifiers_return;
    KeySym keysym_return;
    Modifiers useful_mods;
    int i;
    Modifiers computed = 0;
    Modifiers computedMask = 0;
    Boolean resolved = TRUE;
    Display *dpy = eventSeq->xev->xany.display;

    if (event->lateModifiers != NULL)
        resolved = ComputeLateBindings(event,eventSeq,&computed,&computedMask);
    if (!resolved) return FALSE;
    computed |= event->modifiers;
    computedMask |= event->modifierMask; /* gives do-care mask */

    if ( (computed & computedMask) ==
        (eventSeq->event.modifiers & computedMask) ) {
	Modifiers least_mod;
        XtTranslateKeycode(dpy, (KeyCode)eventSeq->event.eventCode,
			   (unsigned)0, &modifiers_return, &keysym_return);
        if ((keysym_return & event->eventCodeMask)  == event->eventCode ) {
	    TMContext tm_context = _XtGetPerDisplay(dpy)->tm_context;
	    if (tm_context == NULL) tm_context = AllocTMContext(dpy);
	    tm_context->event = eventSeq->xev;
	    tm_context->serial = eventSeq->xev->xany.serial;
	    tm_context->keysym = keysym_return;
	    tm_context->modifiers = 0;
	    return TRUE;
	}
        useful_mods = ~computedMask & modifiers_return;
        if (useful_mods == 0) return FALSE;
	for (least_mod = 1; (least_mod & useful_mods)==0; least_mod <<= 1);
        for (i = modifiers_return; i >= least_mod; i--)
	    /* all useful combinations of 8 modifier bits */
            if (useful_mods & i) {
		 XtTranslateKeycode(dpy, (KeyCode)eventSeq->event.eventCode,
			      (Modifiers)i, &modifiers_return,&keysym_return);
                 if (keysym_return  ==
                     (event->eventCode & event->eventCodeMask)) {
		     TMContext tm_context = _XtGetPerDisplay(dpy)->tm_context;
		     if (tm_context == NULL) tm_context = AllocTMContext(dpy);
		     tm_context->event = eventSeq->xev;
		     tm_context->serial = eventSeq->xev->xany.serial;
		     tm_context->keysym = keysym_return;
		     tm_context->modifiers = (Modifiers)i;
		     return TRUE;
		 }
            }
     }
    return FALSE;

}
void XtConvertCase(dpy,keysym,lower_return,upper_return)
    Display *dpy;
    KeySym keysym;
    KeySym* lower_return,*upper_return;
{
    (*_XtGetPerDisplay(dpy)->defaultCaseConverter)
	(dpy, keysym, lower_return, upper_return);
}

Boolean _XtMatchUsingStandardMods (event,eventSeq)
    Event *event;
    TMEventPtr eventSeq;
{
    Modifiers modifiers_return;
    KeySym keysym_return;
    Modifiers computed= 0;
    Modifiers computedMask = 0;
    Boolean resolved = TRUE;
    Display *dpy = eventSeq->xev->xany.display;
    XtPerDisplay pd = _XtGetPerDisplay(dpy);

    XtTranslateKeycode( dpy, (KeyCode)eventSeq->event.eventCode,
		        (Modifiers)(eventSeq->event.modifiers &
				    ((ShiftMask|LockMask) | pd->mode_switch)),
		        &modifiers_return, &keysym_return);

    if ((event->eventCode & event->eventCodeMask) ==
             (keysym_return & event->eventCodeMask)) {
        if (event->lateModifiers != NULL) 
            resolved = ComputeLateBindings(event,
                eventSeq,&computed,&computedMask);
        if (!resolved) return FALSE;
        computed |= event->modifiers;
        computedMask |= event->modifierMask;

        if ((computed & computedMask) ==
	    (eventSeq->event.modifiers & ~modifiers_return & computedMask)) {
	    TMContext tm_context = pd->tm_context;
	    if (tm_context == NULL) tm_context = AllocTMContext(dpy);
	    tm_context->event = eventSeq->xev;
	    tm_context->serial = eventSeq->xev->xany.serial;
	    tm_context->keysym = keysym_return;
	    tm_context->modifiers = (Modifiers)
		(eventSeq->event.modifiers &
		 ((ShiftMask|LockMask) | pd->mode_switch));
	    return TRUE;
	}
    }
    return FALSE;
}


Boolean _XtMatchAtom(event, eventSeq)
    Event *event;
    TMEventPtr eventSeq;
{
    if (event->eventCodeMask) {	/* first time? */
	event->eventCode = XInternAtom( eventSeq->xev->xany.display,
				        XrmQuarkToString(event->eventCode),
				        False
				      );
	event->eventCodeMask = 0L;
    }
    return (event->eventCode == eventSeq->event.eventCode);
}

static int MatchEvent(translations, eventSeq) 
  XtTranslations translations;
  register TMEventPtr eventSeq;
{
    register EventObjPtr eventTbl = translations->eventObjTbl;
    register int i;

/*
 * The use of "Any" as a modifier can cause obscure bugs since an incoming
 * event may match the "Any" alternative even though a more specific (and
 * correct) event is in the table. It's hard to know which event in the table
 * to match since either could be correct, depending on the circumstances.
 * It's unfortunate that there isn't a unique identifier for a given event...
 * The "any" should be used only when all else fails, but this complicates
 * the algorithms quite a bit. Relying on the order of the productions in the
 * translation table helps, but is not sufficient, both because the earlier
 * specific event may not apply to the current state, and because we can
 * merge translations, resulting in events in the table that are "out of
 * order"
 */
    for (i=0; i < translations->numEvents; i++) {
        if (eventTbl[i].event.eventType ==
                (eventSeq->event.eventType /* & 0x7f */)
            && (eventTbl[i].event.matchEvent != NULL) 
            && ((*eventTbl[i].event.matchEvent)(
                       &eventTbl[i].event,eventSeq)))
                    return i;
            
    }    
    return (-1);
}

#define IsOn(vec,idx) ((vec)[(idx)>>3] & (1 << ((idx) & 7)))

/*
 * there are certain cases where you want to ignore the event and stay
 * in the same state.
 */
static Boolean Ignore(event)
    register TMEventPtr event;
{
    register Display *dpy;
    register XtPerDisplay pd;

    if (event->event.eventType == MotionNotify)
	return TRUE;
    if (!(event->event.eventType == KeyPress ||
	  event->event.eventType == KeyRelease))
	return FALSE;
    dpy = event->xev->xany.display;
    pd = _XtGetPerDisplay(dpy);
    _InitializeKeysymTables(dpy, pd);
    return IsOn(pd->isModifier, event->event.eventCode) ? TRUE : FALSE;
}


static void XEventToTMEvent(event, tmEvent)
    register XEvent *event;
    register TMEventPtr tmEvent;
{
    tmEvent->xev = event;
    tmEvent->event.eventCodeMask = 0;
    tmEvent->event.modifierMask = 0;
    tmEvent->event.eventType = event->type;
    tmEvent->event.lateModifiers = NULL;
    tmEvent->event.matchEvent = NULL;
    tmEvent->event.standard = FALSE;

    switch (event->type) {

	case KeyPress:
	case KeyRelease:
            tmEvent->event.eventCode = event->xkey.keycode;
	    tmEvent->event.modifiers = event->xkey.state;
	    break;

	case ButtonPress:
	case ButtonRelease:
	    tmEvent->event.eventCode = event->xbutton.button;
	    tmEvent->event.modifiers = event->xbutton.state;
	    break;

	case MotionNotify:
	    tmEvent->event.eventCode = event->xmotion.is_hint;
	    tmEvent->event.modifiers = event->xmotion.state;
	    break;

	case EnterNotify:
	case LeaveNotify:
	    tmEvent->event.eventCode = event->xcrossing.mode;
	    tmEvent->event.modifiers = event->xcrossing.state;
	    break;

	case PropertyNotify:
	    tmEvent->event.eventCode = event->xproperty.atom;
	    tmEvent->event.modifiers = 0;
	    break;

	case SelectionClear:
	    tmEvent->event.eventCode = event->xselectionclear.selection;
	    tmEvent->event.modifiers = 0;
	    break;

	case SelectionRequest:
	    tmEvent->event.eventCode = event->xselectionrequest.selection;
	    tmEvent->event.modifiers = 0;
	    break;

	case SelectionNotify:
	    tmEvent->event.eventCode = event->xselection.selection;
	    tmEvent->event.modifiers = 0;
	    break;

	case ClientMessage:
	    tmEvent->event.eventCode = event->xclient.message_type;
	    tmEvent->event.modifiers = 0;
	    break;

	case MappingNotify:
	    tmEvent->event.eventCode = event->xmapping.request;
	    tmEvent->event.modifiers = 0;
	    break;

	case FocusIn:
	case FocusOut:
	    tmEvent->event.eventCode = event->xfocus.mode;
	    tmEvent->event.modifiers = 0;
	    break;

	default:
	    tmEvent->event.eventCode = 0;
	    tmEvent->event.modifiers = 0;
	    break;
    }
}


static unsigned long GetTime(tm, event)
    XtTM tm;
    register XEvent *event;
{
    switch (event->type) {

        case KeyPress:
	case KeyRelease:
	    return event->xkey.time;

        case ButtonPress:
	case ButtonRelease:
	    return event->xbutton.time;

	default:
	    return tm->lastEventTime;

    }

}


/* ARGSUSED */
void _XtTranslateEvent (w, closure, event, continue_to_dispatch)
    Widget w;
    XtPointer closure;		/* XtTM */
    register    XEvent * event;
    Boolean *continue_to_dispatch; /* unused */
{
    register XtTranslations stateTable = ((XtTM)closure)->translations;
    StatePtr oldState;
    TMEventRec curEvent;
    StatePtr current_state = ((XtTM)closure)->current_state;
    int     index;
    register ActionPtr actions;
    XtBoundActions proc_table = ((XtTM)closure)->proc_table;
    XtBoundAccActions accProcTbl = stateTable->accProcTbl;
    XtTM tm = (XtTM)closure;
    ActionHook actionHookList
	= XtWidgetToApplicationContext(w)->action_hook_list;

#ifdef notdef
/* gross disgusting special case ||| */
    if ((event->type == EnterNotify || event->type == LeaveNotify)
        &&( event->xcrossing.detail == NotifyInferior
        ||  event->xcrossing.mode != NotifyNormal) )
	return;
#endif

    XEventToTMEvent (event, &curEvent);

    if (stateTable == NULL) {
        XtAppWarningMsg(XtWidgetToApplicationContext(w),
		XtNtranslationError,"nullTable",XtCXtToolkitError,
            "Can't translate event through NULL table",
            (String *)NULL, (Cardinal *)NULL);
       return ;
    }
    index = MatchEvent (stateTable, &curEvent);
    if (index == -1)
	/* some event came in that we don't have any states for */
	/* ignore it. */
	return;

    /* are we currently in some state other than ground? */
    if (current_state != NULL) {

	oldState = current_state;

	/* find this event in the current level */
	while (current_state != NULL) {
	    Event *ev;
	    /* does this state's index match? --> done */
	    if (current_state->index == index) break;

	    /* is this an event timer? */
	    ev = &stateTable->eventObjTbl[
		current_state->index].event;
	    if (ev->eventType == _XtEventTimerEventType) {

		/* does the succeeding state match? */
		StatePtr nextState = current_state->nextLevel;

		/* is it within the timeout? */
		if (nextState != NULL && nextState->index == index) {
		    unsigned long time = GetTime(tm, event);
		    unsigned long delta =
#ifdef notdef
			ev->eventCode;
		    if (delta == 0)
		      delta =
#endif
		       _XtGetPerDisplay(event->xany.display)->multi_click_time;
		    if ((tm->lastEventTime + delta) >= time) {
			current_state = nextState;
			break;
		    }
		}
	    }

	    /* go to next state */
	    current_state = current_state->next;
	}

	if (current_state == NULL)
	    /* couldn't find it... */
	    if (Ignore(&curEvent)) {
		/* ignore it. */
	        current_state = oldState;
		return;
	    } /* do ground state */
    }

    if (current_state == NULL) {
	/* check ground level */
	current_state = stateTable->eventObjTbl[index].state;
	if (current_state == NULL) return;
    }

    tm->lastEventTime = GetTime (tm, event);

    /* perform any actions */
    actions = current_state->actions;
    while (actions != NULL) {
	/* perform any actions */
        if (actions->index >= 0) {
	   /* non-accelerator context */
           if (proc_table[actions->index] != NULL) {
	       ActionHook hook;
	       String procName =
		   XrmQuarkToString( stateTable->quarkTable[actions->index] );

	       for (hook = actionHookList; hook != NULL; hook = hook->next) {
		   (*hook->proc)( w,
				  hook->closure,
				  procName,
				  event,
				  actions->params,
				  &actions->num_params
				);
	       }
	       (*(proc_table[actions->index]))
		   ( w, event, actions->params, &actions->num_params );
	  }
        }
        else /* actions->index < 0 */ {
	    /* accelerator context */
            int temp = -(actions->index+1);
	    Widget widget;
            if (accProcTbl[temp].proc != NULL
		&& (widget = accProcTbl[temp].widget) != NULL
		&& XtIsSensitive(widget)) {

		ActionHook hook;
		String procName =
		    XrmQuarkToString( stateTable->accQuarkTable[temp] );

		for (hook = actionHookList; hook != NULL; hook = hook->next) {
		    (*hook->proc)( widget,
				   hook->closure,
				   procName,
				   event,
				   actions->params,
				   &actions->num_params
				 );
		}
		(*(accProcTbl[temp].proc))(
					   accProcTbl[temp].widget,
					   event,
					   actions->params,
					   &actions->num_params
					  );
	    }
        }

	actions = actions->next;
    }

    /* move into successor state */
    ((XtTM)tm)->current_state = current_state->nextLevel;
}

static Boolean EqualEvents(event1, event2)
    Event *event1, *event2;
{
    if (   event1->eventType     == event2->eventType
	&& event1->eventCode     == event2->eventCode
	&& event1->eventCodeMask == event2->eventCodeMask
	&& event1->modifiers     == event2->modifiers
	&& event1->modifierMask  == event2->modifierMask) {
        if (event1->lateModifiers != NULL || event2->lateModifiers != NULL) {
	    int i = 0;
	    int j = 0;
	    LateBindingsPtr late1P, late2P;
            if (event1->lateModifiers != NULL)
                for (late1P = event1->lateModifiers;
		     late1P->keysym != NoSymbol; i++) late1P++;
            if (event2->lateModifiers != NULL)
                for (late2P = event2->lateModifiers;
		     late2P->keysym != NoSymbol; j++) late2P++;
            if (i != j) return FALSE;
	    late1P--;
	    while (late1P >= event1->lateModifiers) {
		Boolean last = True;
		for (late2P = event2->lateModifiers + i;
		     late2P >= event2->lateModifiers;
		     late2P--) {

		    if (late1P->keysym == late2P->keysym
			&& late1P->knot == late2P->knot) {
			j--;
			if (last) i--;
			break;
		    }
		    last = False;
		}
		late1P--;
	    }
	    if (j != 0) return FALSE;
        }
        return TRUE;
    }
    return FALSE;
}

static int GetEventIndex(stateTable, event)
    XtTranslations stateTable;
    register EventPtr event;
{
    register int	index;
    register EventObjPtr new;
    register EventObjPtr eventTbl = stateTable->eventObjTbl;

    for (index=0; index < stateTable->numEvents; index++)
        if (EqualEvents(&eventTbl[index].event, &event->event)) return(index);

    if (stateTable->numEvents == stateTable->eventTblSize) {
        stateTable->eventTblSize += 10;
	stateTable->eventObjTbl = (EventObjPtr) XtRealloc(
	    (char *)stateTable->eventObjTbl, 
	    stateTable->eventTblSize*sizeof(EventObjRec));
    }

    new = &stateTable->eventObjTbl[stateTable->numEvents];

    new->event = event->event;
    new->state = NULL;

    return stateTable->numEvents++;
}

static StatePtr NewState(index, stateTable)
    int index;
    XtTranslations stateTable;
{
    register StatePtr state = XtNew(StateRec);

    state->index = index;
    state->nextLevel = NULL;
    state->next = NULL;
    state->actions = NULL;
    state->forw = stateTable->head;
    state->cycle = FALSE;
    stateTable->head = state;
/*
    state->back = NULL;
    if (state->forw != NULL) state->forw->back = state;
*/

    return state;
}

typedef NameValueRec CompiledAction;
typedef NameValueTable CompiledActionTable;

#ifdef lint
Opaque _CompileActionTable(actions, count)
#else
CompiledActionTable _CompileActionTable(actions, count)
#endif
    register struct _XtActionsRec *actions;
    register Cardinal count;
{
    register int i;
    register CompiledActionTable compiledActionTable;

    compiledActionTable = (CompiledActionTable) XtCalloc(
	count+1, (unsigned) sizeof(CompiledAction));

    for (i=0; i<count; i++) {
	compiledActionTable[i].name = actions[i].string;
	compiledActionTable[i].signature = StringToAction(actions[i].string);
	compiledActionTable[i].value = (Value) actions[i].proc;
    }

    compiledActionTable[count].name = NULL;
    compiledActionTable[count].signature = NULL;
    compiledActionTable[count].value = NULL;

#ifdef lint
    return (Opaque) compiledActionTable;
#else
    return compiledActionTable;
#endif
}

static EventMask EventToMask(event)
    EventObjPtr	event;
{
static EventMask masks[] = {
        0,			    /* Error, should never see  */
        0,			    /* Reply, should never see  */
        KeyPressMask,		    /* KeyPress			*/
        KeyReleaseMask,		    /* KeyRelease		*/
        ButtonPressMask,	    /* ButtonPress		*/
        ButtonReleaseMask,	    /* ButtonRelease		*/
        PointerMotionMask	    /* MotionNotify		*/
		| Button1MotionMask
		| Button2MotionMask
		| Button3MotionMask
		| Button4MotionMask
		| Button5MotionMask
		| ButtonMotionMask,
        EnterWindowMask,	    /* EnterNotify		*/
        LeaveWindowMask,	    /* LeaveNotify		*/
        FocusChangeMask,	    /* FocusIn			*/
        FocusChangeMask,	    /* FocusOut			*/
        KeymapStateMask,	    /* KeymapNotify		*/
        ExposureMask,		    /* Expose			*/
        0,			    /* GraphicsExpose, in GC    */
        0,			    /* NoExpose, in GC		*/
        VisibilityChangeMask,       /* VisibilityNotify		*/
        SubstructureNotifyMask,     /* CreateNotify		*/
        StructureNotifyMask,	    /* DestroyNotify		*/
/*		| SubstructureNotifyMask, */
        StructureNotifyMask,	    /* UnmapNotify		*/
/*		| SubstructureNotifyMask, */
        StructureNotifyMask,	    /* MapNotify		*/
/*		| SubstructureNotifyMask, */
        SubstructureRedirectMask,   /* MapRequest		*/
        StructureNotifyMask,	    /* ReparentNotify		*/
/*		| SubstructureNotifyMask, */
        StructureNotifyMask,	    /* ConfigureNotify		*/
/*		| SubstructureNotifyMask, */
        SubstructureRedirectMask,   /* ConfigureRequest		*/
        StructureNotifyMask,	    /* GravityNotify		*/
/*		| SubstructureNotifyMask, */
        ResizeRedirectMask,	    /* ResizeRequest		*/
        StructureNotifyMask,	    /* CirculateNotify		*/
/*		| SubstructureNotifyMask, */
        SubstructureRedirectMask,   /* CirculateRequest		*/
        PropertyChangeMask,	    /* PropertyNotify		*/
        0,			    /* SelectionClear		*/
        0,			    /* SelectionRequest		*/
        0,			    /* SelectionNotify		*/
        ColormapChangeMask,	    /* ColormapNotify		*/
        0,			    /* ClientMessage		*/
        0 ,			    /* MappingNotify		*/
    };

    /* Events sent with XSendEvent in R1 will have high bit set. */
    unsigned long eventType = event->event.eventType /* & 0x7f */;
    if (eventType == MotionNotify) {
        Modifiers modifierMask = event->event.modifierMask;
        EventMask returnMask = 0;
        Modifiers tempMask;

        if (modifierMask == 0) {
	    if (event->event.modifiers == AnyButtonMask)
		return ButtonMotionMask;
	    else
		return PointerMotionMask;
	}
        tempMask = modifierMask &
	    (Button1Mask | Button2Mask | Button3Mask
	     | Button4Mask | Button5Mask);
        if (tempMask == 0)
	    return PointerMotionMask;
        if (tempMask & Button1Mask)
            returnMask |= Button1MotionMask;
        if (tempMask & Button2Mask)
            returnMask |= Button2MotionMask;
        if (tempMask & Button3Mask)
            returnMask |= Button3MotionMask;
        if (tempMask & Button4Mask)
            returnMask |= Button4MotionMask;
        if (tempMask & Button5Mask)
            returnMask |= Button5MotionMask;
        return returnMask;
    }
    return ((eventType >= XtNumber(masks)) ?  0 : masks[eventType]);
}

static void DispatchMappingNotify(widget, closure, call_data)
    Widget widget;
    XtPointer closure;		/* XtTM */
    XtPointer call_data;	/* XEvent* */
{
    Boolean bool;
    _XtTranslateEvent( widget, closure, (XEvent*)call_data, &bool );
}


/*** Public procedures ***/

void _XtInstallTranslations(widget, stateTable)
    Widget widget;
    XtTranslations stateTable;
{
    register EventMask	eventMask = 0;
    register Boolean	nonMaskable = FALSE;
    register Cardinal	i;

/*    widget->core.translations = stateTable; */
    if (stateTable == NULL) return;

    for (i = 0; i < stateTable->numEvents; i++) {
	register EventMask mask = EventToMask(&stateTable->eventObjTbl[i]);

	if (mask != 0)
	    eventMask |= mask;
	else {
	    nonMaskable = True;
	    if (stateTable->eventObjTbl[i].event.eventType == MappingNotify) {
		_XtAddCallback( widget,
			        &_XtGetPerDisplay(XtDisplay(widget))
			            ->mapping_callbacks,
			        DispatchMappingNotify,
			        (XtPointer)&widget->core.tm
			      );
	    }
	}
    }

    /* double click needs to make sure that you have selected on both
	button down and up. */

    if (eventMask & ButtonPressMask) eventMask |= ButtonReleaseMask;
    if (eventMask & ButtonReleaseMask) eventMask |= ButtonPressMask;

    XtAddEventHandler(
        widget, eventMask, nonMaskable,
             _XtTranslateEvent, (XtPointer)&widget->core.tm);

}

void XtUninstallTranslations(widget)
    Widget widget;
{
    XtRemoveEventHandler(widget, XtAllEvents, TRUE, _XtTranslateEvent,
                     (XtPointer)&widget->core.tm);
    widget->core.tm.translations = NULL;
    if (widget->core.tm.proc_table != NULL)
        XtFree((char *)widget->core.tm.proc_table);
    widget->core.tm.proc_table = NULL;
    widget->core.tm.current_state = NULL;
}


typedef struct _ActionListRec *ActionList;
typedef struct _ActionListRec {
    ActionList		next;
    CompiledActionTable table;
} ActionListRec;

static void ReportUnboundActions(tm, stateTable)
    XtTM tm;
    XtTranslations stateTable;
{
    Cardinal num_unbound;
    char     message[10000];
    register Cardinal num_chars;
    register Cardinal i;

    num_unbound = 0;
    (void) strcpy(message, "Actions not found: ");
    num_chars = strlen(message);

    for (i=0; i < stateTable->numQuarks; i++) {
	if (tm->proc_table[i] == NULL) {
	    String s = XrmQuarkToString(stateTable->quarkTable[i]);
	    if (num_unbound != 0) {
		(void) strcpy(&message[num_chars], ", ");
		num_chars = num_chars + 2;
	    }
	    (void) strcpy(&message[num_chars], s);
	    num_chars += strlen(s);
	    num_unbound++;
	}
    }
    message[num_chars] = '\0';
    if (num_unbound != 0)
        XtWarningMsg(XtNtranslationError,"unboundActions",XtCXtToolkitError,
                  message, (String *)NULL, (Cardinal *)NULL);
}


static int BindActions(tm, compiledActionTable, indexP)
    XtTM tm;
    CompiledActionTable compiledActionTable;
    Cardinal *indexP;
{
    XtTranslations stateTable=tm->translations;
    int unbound = stateTable->numQuarks;
    CompiledAction* action;
    Cardinal index;
    Boolean savedIndex = False;

    for (index = *indexP; index < stateTable->numQuarks; index++) {
       if (tm->proc_table[index] == NULL) {
           /* attempt to bind it */
           register XrmQuark q = stateTable->quarkTable[index];
	   Boolean found = False;
           for (action = compiledActionTable; action->name != NULL; action++) {
               if (action->signature == q) {
		   tm->proc_table[index] = (XtActionProc)action->value;
                   unbound--;
		   found = True;
                   break;
               }
           }
	   if (!found && !savedIndex) {
	       *indexP= index;
	       savedIndex = True;
	   }
       } else {
           /* already bound, leave it alone */
           unbound--;
       }
    }
    return unbound;
}



static int BindAccActions(widget, stateTable, compiledActionTable,
			  accBindings, indexP)
    Widget widget;
    XtTranslations stateTable;
    CompiledActionTable compiledActionTable;
    XtBoundAccActions accBindings;
    Cardinal *indexP;
{
    int unbound = stateTable->accNumQuarks;
    int i;
    Cardinal index;
    Boolean savedIndex = False;

    for (index = *indexP; index < stateTable->accNumQuarks; index++) {
       if (accBindings[index].proc == NULL) {
           /* attempt to bind it */
           register XrmQuark q = stateTable->accQuarkTable[index];
	   Boolean found = False;
           for (i = 0; compiledActionTable[i].name != NULL; i++) {
               if (compiledActionTable[i].signature == q) {
                   accBindings[index].widget =widget;
		   accBindings[index].proc=
                     (XtActionProc) compiledActionTable[i].value;
                   unbound--;
		   found = True;
                   break;
               }
           }
	   if (!found && !savedIndex) {
	       *indexP= index;
	       savedIndex = True;
	   }
       } else {
           /* already bound, leave it alone */
           unbound--;
       }
    }
    return unbound;
}


void _XtBindActions(widget, tm)
    Widget widget;
    XtTM tm;
{
    XtTranslations  stateTable=tm->translations;
    register Widget	    w;
    register WidgetClass    class;
    register ActionList     actionList;
    int unbound = -1; /* initialize to non-zero */
    XtAppContext app;
    Cardinal index = 0;

    if (stateTable == NULL) return;
    tm->proc_table= (XtBoundActions) XtCalloc(
                      stateTable->numQuarks,(Cardinal)sizeof(XtBoundActions));
    w = widget;
    do {
        class = w->core.widget_class;
        do {
            if (class->core_class.actions != NULL)
             unbound =
		 BindActions( tm,
			      (CompiledActionTable)class->core_class.actions,
			      &index
			    );
	    class = class->core_class.superclass;
        } while (unbound != 0 && class != NULL);
    w = w->core.parent;
    } while (unbound != 0 && w != NULL);

    app = XtWidgetToApplicationContext(widget);
    for (actionList = app->action_table;
	 unbound != 0 && actionList != NULL;
	 actionList = actionList->next) {
	unbound = BindActions(tm, actionList->table, &index);
    }
    if (unbound != 0) ReportUnboundActions(tm, stateTable);
}

static
void _XtBindAccActions(widget, stateTable)
    Widget	    widget;
    XtTranslations  stateTable;
{
    register Widget	    w;
    register WidgetClass    class;
    register ActionList     actionList;
    int unbound = -1; /* initialize to non-zero */
    XtBoundAccActions accTemp;
    XtAppContext app;
    Cardinal index = 0;

    w = widget;
    if (stateTable == NULL) return;
    accTemp = (XtBoundAccActions) XtCalloc(
                      stateTable->accNumQuarks,
		      (Cardinal)sizeof(XtBoundAccActionRec));
    do {
	class = w->core.widget_class;
	do {
	    if (class->core_class.actions != NULL)
	     unbound = BindAccActions(
				widget,
				stateTable,
				(CompiledActionTable)class->core_class.actions,
				accTemp,
				&index);
	    class = class->core_class.superclass;
	} while (unbound != 0 && class != NULL);
	w = w->core.parent;
    } while (unbound != 0 && w != NULL);

    app = XtWidgetToApplicationContext(widget);
    for (actionList = app->action_table;
	 unbound != 0 && actionList != NULL;
	 actionList = actionList->next) {
	unbound = BindAccActions( widget,
				  stateTable,
				  actionList->table,
				  accTemp,
				  &index);
    }
/*    if (unbound != 0) ReportUnboundActions(tm, stateTable);*/
    stateTable->accProcTbl = accTemp;
}

void XtAddActions(actions, num_actions)
    XtActionList actions;
    Cardinal num_actions;
{
    XtAppAddActions(_XtDefaultAppContext(), actions, num_actions);
}

void XtAppAddActions(app, actions, num_actions)
    XtAppContext app;
    XtActionList actions;
    Cardinal num_actions;
{
    register ActionList rec;

    rec = XtNew(ActionListRec);
    rec->next = app->action_table;
    app->action_table = rec;
    rec->table = (CompiledActionTable) _CompileActionTable(actions, num_actions);
}

void _XtInitializeStateTable(pStateTable)
    XtTranslations *pStateTable;
{
    register XtTranslations  stateTable;

    (*pStateTable) = stateTable = XtNew(TranslationData);
    stateTable->operation = XtTableReplace;
    stateTable->numEvents = 0;
    stateTable->eventTblSize = 0;
    stateTable->eventObjTbl = NULL;
    stateTable->head = NULL;
    stateTable->quarkTable =
        (XrmQuark *)XtCalloc((Cardinal)20,(Cardinal)sizeof(XrmQuark));
    stateTable->quarkTblSize = 20;
    stateTable->numQuarks = 0;
    stateTable->accNumQuarks = 0;
    stateTable->accQuarkTable = NULL;
    stateTable->accProcTbl= NULL;
    stateTable->accQuarkTblSize = 0;
}

void _XtAddEventSeqToStateTable(eventSeq, stateTable)
    register EventSeqPtr eventSeq;
    XtTranslations stateTable;
{
    register int     index;
    register StatePtr *state;
    EventSeqPtr initialEvent = eventSeq;

    if (eventSeq == NULL) return;

    /* initialize event index and state ptr */
    /* note that all states in the event seq passed in start out null */
    /* we fill them in with the matching state as we traverse the list */

    index = GetEventIndex (stateTable, eventSeq);
    state = &stateTable->eventObjTbl[index].state;

    for (;;) {
    /* index is eventIndex for event */
    /* *state is head of state chain for current state */

	while (*state != NULL && (*state)->index != index)
	    state = &(*state)->next;
	if (*state == NULL) *state = NewState (index, stateTable);

	/* *state now points at state record matching event */
	eventSeq->state = *state;

	if (eventSeq->actions != NULL) {
	    if ((*state)->actions != NULL) {
		String str, buf = (String)XtMalloc((Cardinal)100);
		int len = 100;
		String params[1];
		Cardinal num_params = 1;
		XtWarningMsg (XtNtranslationError,"ambiguousActions", 
                           XtCXtToolkitError,
                           "Overriding earlier translation manager actions.",
                            (String *)NULL, (Cardinal *)NULL);
		str = PrintEventSeq( &buf, &len, buf, initialEvent, NULL );
		/* CHECK_STR_OVERFLOW */
		if (str - buf > len - STR_THRESHOLD) {
		    String old = buf;
		    buf = XtRealloc(old, (Cardinal)(len += STR_INCAMOUNT));
		    str = str - old + buf;
		}
		*str++ = ':';
		(void)PrintActions( &buf, &len, str, (*state)->actions, stateTable );
		params[0] = buf;
		XtWarningMsg (XtNtranslationError,"oldActions",XtCXtToolkitError,
			      "Previous entry was: %s", params, &num_params);
		(void)PrintActions( &buf, &len, buf, eventSeq->actions, stateTable );
		params[0] = buf;
		XtWarningMsg (XtNtranslationError,"newActions",XtCXtToolkitError,
			      "New actions are:%s", params, &num_params);
		XtFree((XtPointer)buf);
		FreeActions((*state)->actions);
	    }
	    (*state)->actions = eventSeq->actions;
	}

    /* are we done? */
	eventSeq = eventSeq->next;
	if (eventSeq == NULL) break;
	if (eventSeq->state != NULL) {
	    /* we've been here before... must be a cycle in the event seq. */
	    (*state)->nextLevel = eventSeq->state;
	    (*state)->cycle = TRUE;
	    break;
	}

	if ((*state)->cycle) {

	    /* unroll the loop one state */
	    /* this code hurts my head... ||| think about multiple */
	    /* states pointing at same "next" state record */

	    StatePtr oldNextLevel = (*state)->nextLevel;
	    register StatePtr newNextLevel =
		NewState(oldNextLevel->index, stateTable);

	    newNextLevel->actions = oldNextLevel->actions;
	    newNextLevel->nextLevel = oldNextLevel->nextLevel;
	    newNextLevel->next = oldNextLevel->next;
	    newNextLevel->cycle = TRUE;
	    (*state)->cycle = FALSE;
	    (*state)->nextLevel = newNextLevel;
	}
	state = &(*state)->nextLevel;
	index = GetEventIndex (stateTable, eventSeq);
    }
}


typedef struct _StateMapRec *StateMap;
typedef struct _StateMapRec {
    StatePtr	old, new;
    StateMap	next;
} StateMapRec;

static void MergeStates(old, new, override, indexMap,
                           quarkIndexMap, accQuarkIndexMap,oldTable, stateMap)
    register StatePtr *old, new;
    Boolean override;
    Cardinal *indexMap, *quarkIndexMap,*accQuarkIndexMap;
    XtTranslations oldTable;
    StateMap stateMap;
{
    register StatePtr state;
    StateMap oldStateMap = stateMap;
    ActionRec *a,**aa,*b;

    while (new != NULL) {
	register int index = indexMap[new->index];

	/* make sure old and new match */
	for (state = *old; ; state=state->next) {
	    if (state == NULL) {
		/* corresponding arc doesn't exist, add it */
		state = NewState(index, oldTable);
		state->next = *old;
		*old = state;
		break;
	    }

	    if (state->index == index) /* found it */ break;
	}
    
	/* state and new are pointing at corresponding state records */
	{
	    StateMap temp = XtNew(StateMapRec);
	    temp->next = stateMap;
	    stateMap = temp;
	    temp->old = state;
	    temp->new = new;
	}
    
	/* merge the actions */
	while (state->actions != NULL && override) {
	   a = state->actions;
	   state->actions=a->next;
	   XtFree((char *)a);
	}
      if (state->actions == NULL) {
        aa = &(state->actions);
        b = new->actions;
        while (b != NULL) {
           a = XtNew(ActionRec); 
           if (b->index >= 0)
               a->index = quarkIndexMap[b->index];
           else
               a->index = -(accQuarkIndexMap[-(b->index+1)]+1);
           a->params = b->params;
           a->num_params=b->num_params;
           a->next = NULL;
           *aa = a;
           aa = &a->next;
           b=b->next;
        }
      }


                     
	if (new->cycle) {
	    /* we're in a cycle, search state map for corresponding state */
	    register StateMap temp;
	    for (
		temp=stateMap;
		temp->new != new->nextLevel;
		temp=temp->next)
	        if (temp == NULL)
                     XtErrorMsg(XtNtranslationError,"mergingTablesWithCycles",
                             XtCXtToolkitError,
"Trying to merge translation tables with cycles, and can't resolve this cycle."
			     , (String *)NULL, (Cardinal *)NULL);
	    (*old)->nextLevel = temp->old;
	    (*old)->cycle = True;
	} else if (! (*old)->cycle || override) {
	    if ((*old)->cycle) {
		(*old)->nextLevel = NULL;
		(*old)->cycle = False;
	    }
	    MergeStates(
	        &(*old)->nextLevel,
		new->nextLevel,
		override,
		indexMap,quarkIndexMap,accQuarkIndexMap,
		oldTable,
		stateMap);
	}
    
	new = new->next;
    }
    while (stateMap != oldStateMap) {
	StateMap temp = stateMap;
	stateMap = stateMap->next;
	XtFree((char *)temp);
    }
}


static void MergeTables(old, new, override,accProcTbl)
    register XtTranslations old, new;
    Boolean override;
    XtBoundAccActions accProcTbl;
{
    register Cardinal i,j,k;
    Cardinal *indexMap,*quarkIndexMap,*accQuarkIndexMap;

    if (new == NULL) return;
    if (old == NULL) {
	XtWarningMsg(XtNtranslationError,"mergingNullTable",XtCXtToolkitError,
            "Old translation table was null, cannot modify.",
	    (String *)NULL, (Cardinal *)NULL);
	return;
    }

    indexMap = (Cardinal *)XtCalloc(new->eventTblSize, (Cardinal)sizeof(Cardinal));

    for (i=0; i < new->numEvents; i++) {
	register Cardinal j;
	EventObjPtr newEvent = &new->eventObjTbl[i];

	for (j=0; j < old->numEvents; j++)
	    if (EqualEvents(
	        &newEvent->event, &old->eventObjTbl[j].event)) break;

	if (j==old->numEvents) {
	    if (j == old->eventTblSize) {
		old->eventTblSize += 10;
		old->eventObjTbl = (EventObjPtr) XtRealloc(
		    (char *)old->eventObjTbl, 
		    old->eventTblSize*sizeof(EventObjRec));
	    }
	    old->eventObjTbl[j] = *newEvent;
#ifdef REFCNT_TRANSLATIONS
	    if (newEvent->event.lateModifiers != NULL) {
		int count = 0;
		LateBindingsPtr b = newEvent->event.lateModifiers;
		while (b->keysym != 0) {b++; count++};
		old->eventObjTbl[j].event.lateModifiers =
		    b = (LateBindingsPtr)
			XtMalloc( (unsigned)count*sizeof(LateBindings) );
		bcopy( (char*)newEvent->event.lateModifiers, (char*)b,
		       (unsigned)count*sizeof(LateBindings) );
	    }
#endif
	    old->eventObjTbl[j].state = NULL;
	    old->numEvents++;
	}
	indexMap[i] = j;
    }
/* merge quark tables */
  quarkIndexMap = (Cardinal *)XtCalloc(new->quarkTblSize, (Cardinal)sizeof(Cardinal));


    for (i=0; i < new->numQuarks; i++) {
        register Cardinal j;

       for (j=0; j < old->numQuarks; j++)
            if (old->quarkTable[j] == new->quarkTable[i]) break;
                

       if (j==old->numQuarks) {
            if (j == old->quarkTblSize) {
                old->quarkTblSize += 20;
                old->quarkTable = (XrmQuark*) XtRealloc(
                    (char *)old->quarkTable,
                    old->quarkTblSize*sizeof(int));
                  }
            old->quarkTable[j]=new->quarkTable[i];
            old->numQuarks++;
        }
        quarkIndexMap[i] = j;
    }
/* merge accelerator quark tables */
  accQuarkIndexMap = (Cardinal *)XtCalloc(
      new->accQuarkTblSize, (Cardinal)sizeof(Cardinal));
    k = old->accNumQuarks;

    for (i=0,j=old->accNumQuarks; i < new->accNumQuarks; ) {
        if (j == old->accQuarkTblSize) {
            old->accQuarkTblSize += 20;
            old->accQuarkTable = (XrmQuark*) XtRealloc(
                (char *)old->accQuarkTable,
                old->accQuarkTblSize*sizeof(int));
         }
         old->accQuarkTable[j]=new->accQuarkTable[i];
         old->accNumQuarks++;
         accQuarkIndexMap[i++] = j++;
    }

/* merge accelerator action bindings */

    if (old->accProcTbl == NULL) {
        old->accProcTbl = (XtBoundAccActionRec*)XtCalloc(
            old->accQuarkTblSize,(Cardinal)sizeof(XtBoundAccActionRec) );
    }
    else old->accProcTbl = (XtBoundAccActionRec*)XtRealloc(
        (char *)old->accProcTbl,
	old->accQuarkTblSize*sizeof(XtBoundAccActionRec) );
    for (i=0/*,k=k*/;i<new->accNumQuarks;){
        old->accProcTbl[k].widget = accProcTbl[i].widget;
        old->accProcTbl[k++].proc = accProcTbl[i++].proc;
    }

    for (i=0; i < new->numEvents; i++)
	MergeStates(
	    &old->eventObjTbl[indexMap[i]].state,
	    new->eventObjTbl[i].state,
	    override,
	    indexMap,quarkIndexMap,accQuarkIndexMap,
	    old,
	    (StateMap) NULL);
   XtFree((char *)indexMap);
   XtFree((char *)quarkIndexMap);
   XtFree((char *)accQuarkIndexMap);
}


/*ARGSUSED*/
Boolean _XtCvtMergeTranslations(dpy, args, num_args, from, to, closure_ret)
    Display	*dpy;
    XrmValuePtr args;
    Cardinal    *num_args;
    XrmValuePtr from,to;
    XtPointer	*closure_ret;
{
    XtTranslations old, new, merged;
    TMkind operation;

    if (*num_args != 0)
	XtWarningMsg("invalidParameters","mergeTranslations",XtCXtToolkitError,
             "MergeTM to TranslationTable needs no extra arguments",
               (String *)NULL, (Cardinal *)NULL);

    if (to->addr != NULL && to->size < sizeof(XtTranslations)) {
	to->size = sizeof(XtTranslations);
	return False;
    }
	
    old = ((TMConvertRec*)from->addr)->old;
    new = ((TMConvertRec*)from->addr)->new;
    operation = ((TMConvertRec*)from->addr)->operation;

    if (old == NULL)
#ifdef REFCNT_TRANSLATIONS
    {
	_XtInitializeStateTable(&merged);
	MergeTables(merged, new, FALSE, new->accProcTbl);
    }
#else
	merged = new;
#endif
    else {
	_XtInitializeStateTable(&merged);
	if (operation == override) {
	    MergeTables(merged, new, FALSE, new->accProcTbl);
	    MergeTables(merged, old, FALSE, old->accProcTbl);
	}
	else if (operation == augment) {
	    MergeTables(merged, old, FALSE, old->accProcTbl);
	    MergeTables(merged, new, FALSE, new->accProcTbl);
	}
    }

    if (to->addr != NULL) {
	*(XtTranslations*)to->addr = merged;
    }
    else {
	static XtTranslations staticStateTable;
	staticStateTable = merged;
	to->addr= (XtPointer)&staticStateTable;
	to->size = sizeof(XtTranslations);
    }
    return True;
}

void XtOverrideTranslations(widget, new)
    Widget widget;
    XtTranslations new;
{
/*
    MergeTables(widget->core.translations, new, TRUE);
*/
    XrmValue from,to;
    TMConvertRec foo;
    XtTranslations newTable;
    XtCacheRef cache_ref;
    from.addr = (XtPointer)&foo;
    from.size = sizeof(TMConvertRec);
    foo.old = widget->core.tm.translations;
    foo.new = new;
    foo.operation = override;
    to.addr = (XtPointer)&newTable;
    to.size = sizeof(XtTranslations);
    if ( ! XtCallConverter( XtDisplay(widget), _XtCvtMergeTranslations,
			    (XrmValuePtr)NULL, (Cardinal)0, &from, &to,
			    &cache_ref ))
	return;

    if (XtIsRealized(widget)) {
            XtUninstallTranslations((Widget)widget);
           widget->core.tm.translations = newTable;
           _XtBindActions(widget, &widget->core.tm);
           _XtInstallTranslations(widget,newTable);
    }
    else widget->core.tm.translations = newTable;

    if (cache_ref != NULL) {
	XtAddCallback( widget, XtNdestroyCallback,
		       XtCallbackReleaseCacheRef, (XtPointer)cache_ref );
    }
}

/* ARGSUSED */
void _XtFreeTranslations(app, toVal, closure, args, num_args)
    XtAppContext app;
    XrmValuePtr	toVal;
    XtPointer	closure;
    XrmValuePtr	args;
    Cardinal	*num_args;
{
    XtTranslations stateTable;
    register StatePtr state;
    register EventObjPtr eventObj;
    register int i;
    register ActionPtr action;

    if (*num_args != 0)
	XtAppWarningMsg(app,
	  "invalidParameters","freeTranslations",XtCXtToolkitError,
          "Freeing XtTranslations requires no extra arguments",
	  (String *)NULL, (Cardinal *)NULL);

    stateTable = *(XtTranslations*)toVal->addr;
    for (i = stateTable->numEvents, eventObj = stateTable->eventObjTbl; i;) {
	XtFree( (char*)eventObj->event.lateModifiers );
	i--; eventObj++;
    }
    XtFree( (char*)stateTable->eventObjTbl );
    XtFree( (char*)stateTable->quarkTable );
    XtFree( (char*)stateTable->accQuarkTable );
    XtFree( (char*)stateTable->accProcTbl );
    for (state = stateTable->head; state;) {
	register StatePtr nextState = state->forw;
	for (action = state->actions; action;) {
	    ActionPtr nextAction = action->next;
	    for (i = action->num_params; i;) {
		XtFree( action->params[--i] );
	    }
	    XtFree( (char*)action->params );
	    action = nextAction;
	}
	XtFree( (char*)state );
	state = nextState;
    }
}

/* ARGSUSED */
static void RemoveAccelerators(widget,closure,data)
    Widget widget;
    XtPointer closure, data;
{
    int i;
    XtTranslations table = (XtTranslations)closure;
    if (table == NULL) {
        XtAppWarningMsg(XtWidgetToApplicationContext(widget),
		XtNtranslationError,"nullTable",XtCXtToolkitError,
            "Can't remove accelerators from NULL table",
            (String *)NULL, (Cardinal *)NULL);
        return;
    }
    if (table->accProcTbl == NULL) {
        XtAppWarningMsg(XtWidgetToApplicationContext(widget),
		XtNtranslationError,"nullTable",XtCXtToolkitError,
            "Tried to remove non-existant accelerators",
            (String *)NULL, (Cardinal *)NULL);
        return;
    }
    for (i=0;i<table->accNumQuarks;i++) {
        if (table->accProcTbl[i].widget == widget)
            table->accProcTbl[i].widget = 0;
    }

}
        

void XtInstallAccelerators(destination, source)
    Widget destination, source;
{
    if ((!XtIsWidget(source)) || source->core.accelerators == NULL) return;
/*    if (source->core.accelerators->accProcTbl == NULL)
 *  %%%
 *  The spec is not clear on when actions specified in accelerators are bound;
 *  The most useful (and easiest) thing seems to be to bind them at this time
 *  (rather than at Realize).  Under the current code the preceeding test
 *  seems always to be True, thus guaranteeing accBindings is always set
 *  before being used below.
 */
        _XtBindAccActions( source, source->core.accelerators );
    if (destination->core.tm.translations == NULL)
	destination->core.tm.translations = source->core.accelerators;
    else {
	XrmValue from, to;
	TMConvertRec cvt;
	XtCacheRef cache_ref;
	XtTranslations temp;
	from.addr = (XtPointer)&cvt;
	from.size = sizeof(TMConvertRec);
	cvt.old = destination->core.tm.translations;
	cvt.new = source->core.accelerators;
	to.addr = (XtPointer)&temp;
	to.size = sizeof(XtTranslations);
	if (source->core.accelerators->operation == XtTableOverride)
	    cvt.operation = override;
	else
	    cvt.operation = augment;
	if ( ! XtCallConverter( XtDisplay(destination),
			        _XtCvtMergeTranslations,
			        (XrmValue*)NULL, (Cardinal)0, &from, &to,
			        &cache_ref )) {
	    return;
	}
	destination->core.tm.translations = temp;
	if (cache_ref != NULL) {
	    XtAddCallback( destination, XtNdestroyCallback,
			   XtCallbackReleaseCacheRef, (XtPointer)cache_ref );
	}
    }
    if (XtIsRealized(destination))
        _XtInstallTranslations(destination,
             destination->core.tm.translations);
    XtAddCallback(source, XtNdestroyCallback,
        RemoveAccelerators,(XtPointer)destination->core.tm.translations);
    if (XtClass(source)->core_class.display_accelerator != NULL){
	 char *buf = XtMalloc((Cardinal)100);
	 int len = 100;
	 String str = buf;
	 int i;
         *str = '\0';
	 for (i = 0; i < source->core.accelerators->numEvents; i++) {
	     StatePtr state = source->core.accelerators->eventObjTbl[i].state;
	     if (state != NULL) {
		 if (str != buf) *str++ = '\n';
		 str = PrintState( &buf, &len, str, str-buf, state,
				   source->core.accelerators,
				   XtDisplay(destination)
				 );
	     }
	 }
         (*(XtClass(source)->core_class.display_accelerator))(source,buf);
	 XtFree(buf);
    }
}
  
void XtInstallAllAccelerators(destination,source)
    Widget destination,source;
{
    register int i;
    CompositeWidget cw;

    /* Recurse down normal children */
    if (XtIsComposite(source)) {
        cw = (CompositeWidget) source;
        for (i = 0; i < cw->composite.num_children; i++) {
            XtInstallAllAccelerators(destination,cw->composite.children[i]);
        }
    }

    /* Recurse down popup children */
    if (XtIsWidget(source)) {
        for (i = 0; i < source->core.num_popups; i++) {
            XtInstallAllAccelerators(destination,source->core.popup_list[i]);
        }
    }
    /* Finally, apply procedure to this widget */
    XtInstallAccelerators(destination,source);
}

void XtAugmentTranslations(widget, new)
    Widget widget;
    XtTranslations new;
{
    XrmValue from,to;
    TMConvertRec foo;
    XtTranslations newTable;
    XtCacheRef cache_ref;
    from.addr = (XtPointer)&foo;
    from.size = sizeof(TMConvertRec);
    foo.old = widget->core.tm.translations;
    foo.new = new;
    foo.operation = augment;
    to.addr = (XtPointer)&newTable;
    to.size = sizeof(XtTranslations);
    if ( ! XtCallConverter( XtDisplay(widget), _XtCvtMergeTranslations,
			    (XrmValue*)NULL, (Cardinal)0, &from, &to,
			    &cache_ref ))
	return;

    if (XtIsRealized(widget)) {
        XtUninstallTranslations((Widget)widget);
        widget->core.tm.translations = newTable;
        _XtBindActions(widget,&widget->core.tm);
        _XtInstallTranslations(widget,newTable);
    }
    else widget->core.tm.translations = newTable;

    if (cache_ref != NULL) {
	XtAddCallback( widget, XtNdestroyCallback,
		       XtCallbackReleaseCacheRef, (XtPointer)cache_ref );
    }
}

static Boolean LookAheadForCycleOrMulticlick(state, eot, countP, nextLevelP)
    register StatePtr state;
    EventObjPtr eot;
    int *countP;
    StatePtr *nextLevelP;
{
    int index = state->index;
    int repeatCount = 0;

    *nextLevelP = NULL;
    for (state = state->nextLevel; state != NULL; state = state->nextLevel) {
	if (state->cycle) {
	    *countP = repeatCount;
	    return True;
	}
	if (state->index == index) {
	    repeatCount++;
	    *nextLevelP = state;
	}
	else if (eot[state->index].event.eventType == _XtEventTimerEventType)
	    continue;
	else /* not same event as starting event and not timer */ {
	    Event *start_evP = &eot[index].event;
	    Event *evP = &eot[state->index].event;
	    unsigned int type = eot[index].event.eventType;
	    unsigned int t = eot[state->index].event.eventType;
	    if (   (type == ButtonPress	  && t != ButtonRelease)
		|| (type == ButtonRelease && t != ButtonPress)
		|| (type == KeyPress	  && t != KeyRelease)
		|| (type == KeyRelease	  && t != KeyPress)
		|| evP->eventCode != start_evP->eventCode
		|| evP->modifiers != start_evP->modifiers
		|| evP->modifierMask != start_evP->modifierMask
		|| evP->lateModifiers != start_evP->lateModifiers
		|| evP->eventCodeMask != start_evP->eventCodeMask
		|| evP->matchEvent != start_evP->matchEvent
		|| evP->standard != start_evP->standard)
		/* not inverse of starting event, either */
		break;
	}
    }
    *countP = repeatCount;
    return False;
}

static String PrintState(buf, len, str, start, state, stateTable, dpy)
    String *buf;
    int *len;
    register String str;
    int start;			/* offset of current LHS; -1 =>print *buf */
    StatePtr state;
    XtTranslations stateTable;
    Display *dpy;
{
    int oldOffset = str - *buf;
    int clickCount;
    Boolean cycle;
    StatePtr nextLevel;
    StatePtr sameLevel;

    /* print the current state */
    if (state == NULL) return str;

    sameLevel = state->next;
    str = PrintEvent( buf, len, str,
		      &stateTable->eventObjTbl[state->index].event,
		      dpy );

    cycle = LookAheadForCycleOrMulticlick( state, stateTable->eventObjTbl,
					   &clickCount, &nextLevel )
	|| state->cycle;

    if (cycle || clickCount > 0) {
	if (clickCount > 0)
	    sprintf( str, "(%d%s)", clickCount+1, cycle ? "+" : "" );
	else
	    bcopy("(+)", str, 4);
	str += strlen(str);
	if (state->actions == NULL && nextLevel != NULL)
	    state = nextLevel;
    }

    if (state->actions != NULL) {
	if (start == -1) {
	    int offset = str - *buf;
	    CHECK_STR_OVERFLOW;
	    *str++ = ':';
	    (void) PrintActions(buf, len, str, state->actions, stateTable);
	    (void) printf("%s\n", *buf);
	    str = *buf + offset;
	}
    } else {
	*str++ = ',';
    }
    *str = '\0';

    /* print succeeding states */
    if (state->nextLevel != NULL && !state->cycle) {
	if (state->actions && start != -1) {
	    *str++ = '\n';
	    ExpandForChars( oldOffset - start );
	    bcopy( *buf+start, str, oldOffset - start );
	    str += oldOffset - start;
    	}
	str = PrintState( buf, len, str, start, state->nextLevel,
			  stateTable, dpy );
    }

    if (sameLevel != NULL) {	/* print sibling states */
	if (start == -1)
	    str = *buf + oldOffset;
	else {
	    *str++ = '\n';
	    ExpandForChars( oldOffset - start );
	    bcopy( *buf+start, str, oldOffset - start );
	    str += oldOffset - start;
	}
	str = PrintState(buf, len, str, start, sameLevel, stateTable, dpy);
    }

    if (start == -1) str = *buf + oldOffset;
    *str = '\0';
    return str;
}

#ifndef NO_MIT_HACKS
void _XtTranslateTablePrint(translations)
    XtTranslations translations;
{
    register Cardinal i;
    int len = 1000;
    char *buf;

    if (translations == NULL) return;
    buf = XtMalloc((Cardinal)1000);
    for (i = 0; i < translations->numEvents; i++) {
	buf[0] = '\0';
	(void) PrintState(
			   &buf,
			   &len,
			   buf,
			   -1,
			   translations->eventObjTbl[i].state,
			   translations,
			   NULL
			 );
    }
    XtFree(buf);
}

/*ARGSUSED*/
static void _XtDisplayTranslations(widget, event, params, num_params)
    Widget widget;
    XEvent *event;
    String *params;
    Cardinal *num_params;
{
    _XtTranslateTablePrint(widget->core.tm.translations);
}

/*ARGSUSED*/
static void _XtDisplayAccelerators(widget, event, params, num_params)
    Widget widget;
    XEvent *event;
    String *params;
    Cardinal *num_params;
{
    _XtTranslateTablePrint(widget->core.accelerators);
}

/*ARGSUSED*/
static void _XtDisplayInstalledAccelerators(widget, event, params, num_params)
    Widget widget;
    XEvent *event;
    String *params;
    Cardinal *num_params;
{
    Widget eventWidget
	= XtWindowToWidget(event->xany.display, event->xany.window);
    XtTranslations translations;
    register Cardinal i;
    int len = 1000;
    char *buf;

    if (eventWidget == NULL) return;

    if ((translations = eventWidget->core.tm.translations) == NULL) return;
    buf = XtMalloc((Cardinal)1000);
    for (i = 0; i < translations->numEvents; i++) {
	register ActionPtr actions
	    = translations->eventObjTbl[i].state->actions;
	/* look for states with accelerator actions only */
	while (actions != NULL && actions->index >= 0) actions = actions->next;
	if (actions != NULL) {
	    buf[0] = '\0';
	    (void) PrintState(
			       &buf,
			       &len,
			       buf,
			       -1,
			       translations->eventObjTbl[i].state,
			       translations,
			       XtDisplay(eventWidget)
			     );
	}
    }
    XtFree(buf);
}
#endif /*NO_MIT_HACKS*/

/***********************************************************************
 *
 * Pop-up and Grab stuff
 *
 ***********************************************************************/

static Widget _XtFindPopup(widget, name)
    Widget widget;
    String name;
{
    register Cardinal i;
    register XrmQuark q;
    register Widget w;

    q = XrmStringToQuark(name);

    for (w=widget; w != NULL; w=w->core.parent)
	for (i=0; i<w->core.num_popups; i++)
	    if (w->core.popup_list[i]->core.xrm_name == q)
		return w->core.popup_list[i];

    return NULL;
}

void XtMenuPopupAction(widget, event, params, num_params)
    Widget widget;
    XEvent *event;
    String *params;
    Cardinal *num_params;
{
    Boolean spring_loaded;
    register Widget popup_shell;

    if (*num_params != 1) {
	XtAppWarningMsg(XtWidgetToApplicationContext(widget),
		      "invalidParameters","xtMenuPopupAction",XtCXtToolkitError,
			"MenuPopup wants exactly one argument",
			(String *)NULL, (Cardinal *)NULL);
	return;
    }

    if (event->type == ButtonPress)
	spring_loaded = True;
    else if (event->type == KeyPress || event->type == EnterNotify)
	spring_loaded = False;
    else {
	XtAppWarningMsg(XtWidgetToApplicationContext(widget),
		"invalidPopup","unsupportedOperation",XtCXtToolkitError,
"Pop-up menu creation is only supported on ButtonPress, KeyPress or EnterNotify events.",
                  (String *)NULL, (Cardinal *)NULL);
	spring_loaded = False;
    }

    popup_shell = _XtFindPopup(widget, params[0]);
    if (popup_shell == NULL) {
	XtAppWarningMsg(XtWidgetToApplicationContext(widget),
			"invalidPopup","xtMenuPopup",XtCXtToolkitError,
			"Can't find popup widget \"%s\" in XtMenuPopup",
			params, num_params);
	return;
    }

    if (spring_loaded) _XtPopup(popup_shell, XtGrabExclusive, TRUE);
    else _XtPopup(popup_shell, XtGrabNonexclusive, FALSE);
}


/*ARGSUSED*/
static void _XtMenuPopdownAction(widget, event, params, num_params)
    Widget widget;
    XEvent *event;
    String *params;
    Cardinal *num_params;
{
    Widget popup_shell;

    if (*num_params == 0) {
	XtPopdown(widget);
    } else if (*num_params == 1) {
	popup_shell = _XtFindPopup(widget, params[0]);
	if (popup_shell == NULL) {
            XtAppWarningMsg(XtWidgetToApplicationContext(widget),
			    "invalidPopup","xtMenuPopup",XtCXtToolkitError,
			    "Can't find popup widget \"%s\" in XtMenuPopdown",
			    params, num_params);
	    return;
	}
	XtPopdown(popup_shell);
    } else {
	XtAppWarningMsg(XtWidgetToApplicationContext(widget),
			"invalidParameters","xtmenuPopdown",XtCXtToolkitError,
			"XtMenuPopdown called with num_params != 0 or 1",
			(String *)NULL, (Cardinal *)NULL);
    }
}

static void GrabAllCorrectKeys(widget, event, grabP)
    Widget widget;
    Event *event;
    GrabActionRec* grabP;
{
    Display *dpy = XtDisplay(widget);
    KeyCode *keycodes, *keycodeP;
    Cardinal keycount;
    XtKeysymToKeycodeList(
	    dpy,
	    (KeySym)event->eventCode,
	    &keycodes,
	    &keycount
			 );
    if (keycount == 0) return;
    for (keycodeP = keycodes; keycount--; keycodeP++) {
	if (event->standard) {
	    /* find standard modifiers that produce this keysym */
	    KeySym keysym;
	    int std_mods, least_mod = 1;
	    Modifiers modifiers_return;
	    XtTranslateKeycode( dpy, *keycodeP, (Modifiers)0,
			        &modifiers_return, &keysym );
	    if (keysym == event->eventCode) {
		XtGrabKey(widget, *keycodeP,
			  (unsigned)event->modifiers,
			  grabP->owner_events,
			  grabP->pointer_mode,
			  grabP->keyboard_mode
			);
		/* continue; */		/* grab all modifier combinations */
	    }
	    while ((least_mod & modifiers_return)==0) least_mod <<= 1;	    
	    for (std_mods = modifiers_return;
		 std_mods >= least_mod; std_mods--) {
		 /* check all useful combinations of modifier bits */
		if (modifiers_return & std_mods) {
		    XtTranslateKeycode( dpy, *keycodeP,
					(Modifiers)std_mods,
					&modifiers_return, &keysym );
		    if (keysym == event->eventCode) {
			XtGrabKey(widget, *keycodeP,
				  (unsigned)event->modifiers | std_mods,
				  grabP->owner_events,
				  grabP->pointer_mode,
				  grabP->keyboard_mode
				);
			/* break; */	/* grab all modifier combinations */
		    }
		}
	    }
	} else /* !event->standard */ {
	    XtGrabKey(widget, *keycodeP,
		      (unsigned)event->modifiers,
		      grabP->owner_events,
		      grabP->pointer_mode,
		      grabP->keyboard_mode
		    );
	}
    }
    XtFree((XtPointer)keycodes);
}

void _XtRegisterGrabs(widget,tm)
    Widget widget;
    XtTM  tm;
{
    XtTranslations stateTable=tm->translations;
    unsigned int count;

    if (! XtIsRealized(widget)) return;

    /* walk the widget instance action bindings table looking for */
    /* actions registered as grab actions. */
    /* when you find one, do a grab on the triggering event */

    if (stateTable == NULL) return;
    for (count=0; count < stateTable->numQuarks; count++) {
      GrabActionRec* grabP;
      for (grabP = grabActionList; grabP != NULL; grabP = grabP->next) {
        if (grabP->action_proc == tm->proc_table[count]) {
	    register StatePtr state;
	    /* we've found a "grabber" in the action table. Find the */
	    /* states that call this action. */
	    /* note that if there is more than one "grabber" in the action */
	    /* table, we end up searching all of the states multiple times. */
	    for (state=stateTable->head; state != NULL; state=state->forw) {
		register ActionPtr action;
	        for (
		    action = state->actions;
		    action != NULL;
		    action=action->next) {
		    if (action->index == count) {
			/* this action is a "grabber" */
			register Event *event;
			event = &stateTable->eventObjTbl[state->index].event;
			switch (event->eventType) {
			    case ButtonPress:
			    case ButtonRelease:
				XtGrabButton(
				    widget,
				    (unsigned) event->eventCode,
				    (unsigned) event->modifiers,
				    grabP->owner_events,
				    grabP->event_mask,
				    grabP->pointer_mode,
				    grabP->keyboard_mode,
				    None,
				    None
				);
				break;
	    
			    case KeyPress:
			    case KeyRelease:
				GrabAllCorrectKeys(widget, event, grabP);
				break;
	    
			    case EnterNotify:
				break;

			    default:
              XtAppWarningMsg(XtWidgetToApplicationContext(widget),
		    "invalidPopup","unsupportedOperation",XtCXtToolkitError,
"Pop-up menu creation is only supported on Button, Key or EnterNotify events.",
                  (String *)NULL, (Cardinal *)NULL);
			    break;
			}
		    }
		}
	    }
	}
      }
    }
}

static XtActionsRec tmActions[] = {
    {"XtMenuPopup", XtMenuPopupAction},
    {"XtMenuPopdown", _XtMenuPopdownAction},
    {"MenuPopup", XtMenuPopupAction}, /* old & obsolete */
    {"MenuPopdown", _XtMenuPopdownAction}, /* ditto */
#ifndef NO_MIT_HACKS
    {"XtDisplayTranslations", _XtDisplayTranslations},
    {"XtDisplayAccelerators", _XtDisplayAccelerators},
    {"XtDisplayInstalledAccelerators", _XtDisplayInstalledAccelerators},
#endif
};


void _XtPopupInitialize(app)
    XtAppContext app;
{
    XtAppAddActions(app, tmActions, XtNumber(tmActions));
    if (grabActionList == NULL)
	XtRegisterGrabAction( XtMenuPopupAction, True,
			      (unsigned)(ButtonPressMask | ButtonReleaseMask),
			      GrabModeAsync,
			      GrabModeAsync
			    );
}

void _XtBuildKeysymTables(dpy,pd)
    Display *dpy;
    register XtPerDisplay pd;
{
    ModToKeysymTable *table;
    int maxCount,i,j,k,tempCount,index;
    KeySym keysym,tempKeysym;
    XModifierKeymap* modKeymap;
    KeyCode keycode;
#define KeysymTableSize 16

    if (pd->keysyms)
	XtFree( (XtPointer)pd->keysyms );
    XDisplayKeycodes(dpy, &pd->min_keycode, &pd->max_keycode);
    pd->keysyms = XGetKeyboardMapping(dpy, pd->min_keycode,
				      pd->max_keycode-pd->min_keycode+1,
				      &pd->keysyms_per_keycode);
    if (pd->modKeysyms)
	XtFree((XtPointer)pd->modKeysyms);
    if (pd->modsToKeysyms)
	XtFree((XtPointer)pd->modsToKeysyms);
    pd->modKeysyms = (KeySym*)XtMalloc((Cardinal)KeysymTableSize*sizeof(KeySym));
    maxCount = KeysymTableSize;
    tempCount = 0;

    table = (ModToKeysymTable*)XtMalloc((Cardinal)8*sizeof(ModToKeysymTable));
    pd->modsToKeysyms = table;

    table[0].mask = ShiftMask;
    table[1].mask = LockMask;
    table[2].mask = ControlMask;
    table[3].mask = Mod1Mask;
    table[4].mask = Mod2Mask;
    table[5].mask = Mod3Mask;
    table[6].mask = Mod4Mask;
    table[7].mask = Mod5Mask;
    tempKeysym = 0;

    modKeymap = XGetModifierMapping(dpy);
    for (i=0;i<32;i++)
	pd->isModifier[i] = 0;
    pd->mode_switch = 0;
    for (i=0;i<8;i++) {
        table[i].index = tempCount;
        table[i].count = 0;
        for (j=0;j<modKeymap->max_keypermod;j++) {
            keycode = modKeymap->modifiermap[i*modKeymap->max_keypermod+j];
            if (keycode != 0) {
		pd->isModifier[keycode>>3] |= 1 << (keycode & 7);
                for (k=0; k<pd->keysyms_per_keycode;k++) {
                    index = ((keycode-pd->min_keycode)*
                             pd->keysyms_per_keycode)+k;
                    keysym = pd->keysyms[index];
		    if ((keysym == XK_Mode_switch) && (i > 2))
			pd->mode_switch |= 1 << i;
                    if (keysym != 0 && keysym != tempKeysym ){
                        if (tempCount==maxCount) {
                            maxCount += KeysymTableSize;
                            pd->modKeysyms = (KeySym*)XtRealloc(
                                (char*)pd->modKeysyms,
                                (unsigned) (maxCount*sizeof(KeySym)) );
                        }
                        pd->modKeysyms[tempCount++] = keysym;
                        table[i].count++;
                        tempKeysym = keysym;
                    }
                }
            }
        }
    }
    pd->lock_meaning = NoSymbol;
    for (i = 0; i < table[1].count; i++) {
	keysym = pd->modKeysyms[table[1].index + i];
	if (keysym == XK_Caps_Lock) {
	    pd->lock_meaning = XK_Caps_Lock;
	    break;
	} else if (keysym == XK_Shift_Lock) {
	    pd->lock_meaning = XK_Shift_Lock;
	}
    }
    XFree((char *)modKeymap);
}

void XtTranslateKeycode (dpy, keycode, modifiers,
                            modifiers_return, keysym_return)

    Display *dpy;
    KeyCode keycode;
    Modifiers modifiers;
    Modifiers *modifiers_return;
    KeySym *keysym_return;

{
    XtPerDisplay pd = _XtGetPerDisplay(dpy);
    _InitializeKeysymTables(dpy, pd);
    (*pd->defaultKeycodeTranslator)(
            dpy,keycode,modifiers,modifiers_return,keysym_return);
}

/* This code should match XTranslateKey (internal, sigh) in Xlib */
void XtTranslateKey(dpy, keycode, modifiers,
                            modifiers_return, keysym_return)
    register Display *dpy;
    KeyCode keycode;
    Modifiers modifiers;
    Modifiers *modifiers_return;
    KeySym *keysym_return;
{
    register XtPerDisplay pd = _XtGetPerDisplay(dpy);
    int per;
    register KeySym *syms;
    KeySym sym, lsym, usym;

    *modifiers_return = (ShiftMask|LockMask) | pd->mode_switch;
    if ((keycode < pd->min_keycode) || (keycode > pd->max_keycode))  {
	*keysym_return = NoSymbol;
	return;
    }
    per = pd->keysyms_per_keycode;
    syms = &pd->keysyms[(keycode - pd->min_keycode) * per];
    while ((per > 2) && (syms[per - 1] == NoSymbol))
	per--;
    if ((per > 2) && (modifiers & pd->mode_switch)) {
	syms += 2;
	per -= 2;
    }
    if (!(modifiers & ShiftMask) &&
	(!(modifiers & LockMask) || (pd->lock_meaning == NoSymbol))) {
	if ((per == 1) || (syms[1] == NoSymbol))
	    (*pd->defaultCaseConverter)(dpy, syms[0], keysym_return, &usym);
	else
	    *keysym_return = syms[0];
    } else if (!(modifiers & LockMask) ||
	       (dpy->lock_meaning != XK_Caps_Lock)) {
	if ((per == 1) || ((usym = syms[1]) == NoSymbol))
	    (*pd->defaultCaseConverter)(dpy, syms[0], &lsym, &usym);
	*keysym_return = usym;
    } else {
	if ((per == 1) || ((sym = syms[1]) == NoSymbol))
	    sym = syms[0];
	(*pd->defaultCaseConverter)(dpy, sym, &lsym, &usym);
	if (!(modifiers & ShiftMask) && (sym != syms[0]) &&
	    ((sym != usym) || (lsym == usym)))
	    (*pd->defaultCaseConverter)(dpy, syms[0], &lsym, &usym);
	*keysym_return = usym;
    }
    if (*keysym_return == XK_VoidSymbol)
	*keysym_return = NoSymbol;
}

void XtSetKeyTranslator(dpy, translator)

    Display *dpy;
    XtKeyProc translator;

{
    _XtGetPerDisplay(dpy)->defaultKeycodeTranslator = translator;
    /* XXX should now redo grabs */
}

/* ARGSUSED */
void XtRegisterCaseConverter(dpy, proc, start, stop)

    Display *dpy;
    XtCaseProc proc;
    KeySym start;
    KeySym stop;

{
    _XtGetPerDisplay(dpy)->defaultCaseConverter = proc;
    /* XXX should now redo grabs */
}

/* This code should match XConvertCase (internal, sigh) in Xlib */
/* ARGSUSED */
void _XtConvertCase(dpy, sym, lower, upper)
    Display *dpy;
    KeySym sym;
    KeySym *lower;
    KeySym *upper;
{
    *lower = sym;
    *upper = sym;
    switch(sym >> 8) {
    case 0:
	if ((sym >= XK_A) && (sym <= XK_Z))
	    *lower += (XK_a - XK_A);
	else if ((sym >= XK_a) && (sym <= XK_z))
	    *upper -= (XK_a - XK_A);
	else if ((sym >= XK_Agrave) && (sym <= XK_Odiaeresis))
	    *lower += (XK_agrave - XK_Agrave);
	else if ((sym >= XK_agrave) && (sym <= XK_odiaeresis))
	    *upper -= (XK_agrave - XK_Agrave);
	else if ((sym >= XK_Ooblique) && (sym <= XK_Thorn))
	    *lower += (XK_oslash - XK_Ooblique);
	else if ((sym >= XK_oslash) && (sym <= XK_thorn))
	    *upper -= (XK_oslash - XK_Ooblique);
	break;
    default:
	/* XXX do all other sets */
	break;
    }
}


void XtRegisterGrabAction(action_proc, owner_events, event_mask,
			  pointer_mode, keyboard_mode)
    XtActionProc action_proc;
    Boolean owner_events;
    unsigned int event_mask;
    int pointer_mode, keyboard_mode;
{
    GrabActionRec* actionP;

    for (actionP = grabActionList; actionP != NULL; actionP = actionP->next) {
	if (actionP->action_proc == action_proc) break;
    }
    if (actionP == NULL) {
	actionP = XtNew(GrabActionRec);
	actionP->action_proc = action_proc;
	actionP->next = grabActionList;
	grabActionList = actionP;
    }
#ifdef DEBUG
    else
	if (   actionP->owner_events != owner_events
	    || actionP->event_mask != event_mask
	    || actionP->pointer_mode != pointer_mode
	    || actionP->keyboard_mode != keyboard_mode) {
	    XtWarningMsg(
		"argsReplaced", "xtRegisterGrabAction", XtCXtToolkitError,
		"XtRegisterGrabAction called on same proc with different args"
			);
	}
#endif /*DEBUG*/

    actionP->owner_events = owner_events;
    actionP->event_mask = event_mask;
    actionP->pointer_mode = pointer_mode;
    actionP->keyboard_mode = keyboard_mode;
}

KeySym *XtGetKeysymTable(dpy, min_keycode_return, keysyms_per_keycode_return)
    Display *dpy;
    KeyCode *min_keycode_return;
    int *keysyms_per_keycode_return;
{
    XtPerDisplay pd = _XtGetPerDisplay(dpy);
    _InitializeKeysymTables(dpy, pd);
    *min_keycode_return = pd->min_keycode; /* %%% */
    *keysyms_per_keycode_return = pd->keysyms_per_keycode;
    return pd->keysyms;
}

void XtKeysymToKeycodeList(dpy, keysym, keycodes_return, keycount_return)
    Display *dpy;
    KeySym keysym;
    KeyCode **keycodes_return;
    Cardinal *keycount_return;
{
    register XtPerDisplay pd = _XtGetPerDisplay(dpy);
    KeyCode keycode;
    int per, match;
    register KeySym *syms;
    register int i, j;
    KeySym lsym, usym;
    unsigned maxcodes = 0;
    unsigned ncodes = 0;
    KeyCode *keycodes, *codeP;

    _InitializeKeysymTables(dpy, pd);
    keycodes = NULL;
    per = pd->keysyms_per_keycode;
    for (syms = pd->keysyms, keycode = pd->min_keycode;
	 keycode <= pd->max_keycode;
	 syms += per, keycode++) {
	match = 0;
	for (j = 0; j < per; j++) {
	    if (syms[j] == keysym) {
		match = 1;
		break;
	    }		
	}
	if (!match)
	    for (i = 1; i < 5; i += 2) {
		if ((per == i) || ((per > i) && (syms[i] == NoSymbol))) {
		    (*pd->defaultCaseConverter)(dpy, syms[i-1], &lsym, &usym);
		    if ((lsym == keysym) || (usym == keysym)) {
			match = 1;
			break;
		    }
		}
	    }
	if (match) {
	    if (ncodes == maxcodes) {
		KeyCode *old = keycodes;
		maxcodes += KEYCODE_ARRAY_SIZE;
		keycodes = (KeyCode*)XtMalloc(maxcodes*sizeof(KeyCode));
		if (ncodes) {
		    bcopy( old, keycodes, ncodes*sizeof(KeyCode) );
		    XtFree((XtPointer)old);
		}
		codeP = &keycodes[ncodes];
	    }
	    *codeP++ = keycode;
	    ncodes++;
	}
    }
    *keycodes_return = keycodes;
    *keycount_return = ncodes;
}


void XtCallActionProc(widget, action, event, params, num_params)
    Widget widget;
    String action;
    XEvent *event;
    String *params;
    Cardinal num_params;
{
    CompiledAction* actionP;
    XrmQuark q = XrmStringToQuark(action);
    Widget w = widget;
    XtAppContext app = XtWidgetToApplicationContext(widget);
    ActionList actionList;

    XtCheckSubclass(widget, coreWidgetClass,
	    "XtCallActionProc first argument is not a subclass of Core");

    do {
	WidgetClass class = XtClass(w);
	do {
	    if (actionP = (CompiledActionTable)class->core_class.actions)
	      for (; actionP->name != NULL; actionP++) {

		if (actionP->signature == q) {
		    ActionHook hook = app->action_hook_list;
		    while (hook != NULL) {
			(*hook->proc)( widget,
				       hook->closure,
				       action,
				       event,
				       params,
				       &num_params
				     );
			hook= hook->next;
		    }
		    (*(XtActionProc)(actionP->value))
			(widget, event, params, &num_params);
		    return;
		}
	    }
	    class = class->core_class.superclass;
	} while (class != NULL);
	w = XtParent(w);
    } while (w != NULL);

    for (actionList = app->action_table;
	 actionList != NULL;
	 actionList = actionList->next) {

	for (actionP = actionList->table; actionP->name != NULL; actionP++) {
	    if (actionP->signature == q) {
		ActionHook hook = app->action_hook_list;
		while (hook != NULL) {
		    (*hook->proc)( widget,
				   hook->closure,
				   action,
				   event,
				   params,
				   &num_params
				 );
		    hook= hook->next;
		}
		(*(XtActionProc)(actionP->value))
		    (widget, event, params, &num_params);
		return;
	    }
	}

    }

    {
	String params[2];
	Cardinal num_params = 2;
	params[0] = action;
	params[1] = XtName(widget);
	XtAppWarningMsg(app,
	    "noActionProc", "xtCallActionProc", XtCXtToolkitError,
	    "No action proc named \"%s\" is registered for widget \"%s\"",
	    params, &num_params
			);
    }
}


