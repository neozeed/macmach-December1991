#ifndef lint
static char Xrcsid[] = "$XConsortium: TMparse.c,v 1.88 89/12/19 16:06:15 swick Exp $";
/* $oHeader: TMparse.c,v 1.4 88/09/01 17:30:39 asente Exp $ */
#endif /*lint*/

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

#include <X11/Xos.h>
#include <X11/Xlib.h>
#include "StringDefs.h"
#include <stdio.h>
#include "IntrinsicI.h"
#include <ctype.h>
#ifndef NOTASCII
#define XK_LATIN1
#endif
#define XK_MISCELLANY
#include <X11/keysymdef.h>

#ifdef CACHE_TRANSLATIONS
# ifdef REFCNT_TRANSLATIONS
#  define CACHED XtCacheAll | XtCacheRefCount
# else
#  define CACHED XtCacheAll
# endif
#else
# define CACHED XtCacheNone
#endif

/* Private definitions. */
#define LF 0x0a
#define BSLASH '\\'

typedef int		EventType;
typedef unsigned int	XtEventType;
typedef unsigned int	EventCode;

typedef String (*ParseProc)(); /* str, closure, event ,error */
    /* String str; */
    /* Opaque closure; */
    /* EventPtr event; */
    /* Boolean* error */

typedef void (*ModifierProc)(); 

typedef struct _ModifierRec {
    char*      name;
    XrmQuark   signature;
    ModifierProc modifierParseProc;
    Value      value;
} ModifierRec,*ModifierKeys;

typedef struct _EventKey {
    char    	*event;
    XrmQuark	signature;
    EventType	eventType;
    ParseProc	parseDetail;
    Opaque	closure;
}EventKey, *EventKeys;
static void ParseModImmed();
static void ParseModSym();
static String PanicModeRecovery();
static String CheckForPoundSign();
static String ScanFor();
static KeySym StringToKeySym();
static ModifierRec modifiers[] = {
    {"None",    0,      ParseModImmed,None},
    {"Shift",	0,	ParseModImmed,ShiftMask},
    {"Lock",	0,	ParseModImmed,LockMask},
    {"Ctrl",	0,	ParseModImmed,ControlMask},
    {"Mod1",	0,	ParseModImmed,Mod1Mask},
    {"Mod2",	0,	ParseModImmed,Mod2Mask},
    {"Mod3",	0,	ParseModImmed,Mod3Mask},
    {"Mod4",	0,	ParseModImmed,Mod4Mask},
    {"Mod5",	0,	ParseModImmed,Mod5Mask},
    {"Meta",	0,	ParseModSym,  XK_Meta_L},
    {"m",       0,      ParseModSym,  XK_Meta_L},
    {"h",       0,      ParseModSym,  XK_Hyper_L},
    {"su",      0,      ParseModSym,  XK_Super_L},
    {"a",       0,      ParseModSym,  XK_Alt_L},
    {"Hyper",   0,      ParseModSym,  XK_Hyper_L},
    {"Super",   0,      ParseModSym,  XK_Super_L},
    {"Alt",     0,      ParseModSym,  XK_Alt_L},
    {"Button1",	0,	ParseModImmed,Button1Mask},
    {"Button2",	0,	ParseModImmed,Button2Mask},
    {"Button3",	0,	ParseModImmed,Button3Mask},
    {"Button4",	0,	ParseModImmed,Button4Mask},
    {"Button5",	0,	ParseModImmed,Button5Mask},

    {"Any",	0,	ParseModImmed,AnyModifier},

    {"c",	0,	ParseModImmed,ControlMask},
    {"s",	0,	ParseModImmed,ShiftMask},
    {"l",	0,	ParseModImmed,LockMask},

    {NULL, NULL, NULL},
};

static NameValueRec buttonNames[] = {
    {"Button1",	0,	Button1},
    {"Button2", 0,	Button2},
    {"Button3", 0,	Button3},
    {"Button4", 0,	Button4},
    {"Button5", 0,	Button5},
    {NULL, NULL, NULL},
};

static NameValueRec motionDetails[] = {
    {"Normal",		0,	NotifyNormal},
    {"Hint",		0,	NotifyHint},
    {NULL, NULL, NULL},
};

static NameValueRec notifyModes[] = {
    {"Normal",		0,	NotifyNormal},
    {"Grab",		0,	NotifyGrab},
    {"Ungrab",		0,	NotifyUngrab},
    {"WhileGrabbed",    0,	NotifyWhileGrabbed},
    {NULL, NULL, NULL},
};

static NameValueRec notifyDetail[] = {
    {"Ancestor",	    0,	NotifyAncestor},
    {"Virtual",		    0,	NotifyVirtual},
    {"Inferior",	    0,	NotifyInferior},
    {"Nonlinear",	    0,	NotifyNonlinear},
    {"NonlinearVirtual",    0,	NotifyNonlinearVirtual},
    {"Pointer",		    0,	NotifyPointer},
    {"PointerRoot",	    0,	NotifyPointerRoot},
    {"DetailNone",	    0,	NotifyDetailNone},
    {NULL, NULL, NULL},
};

static NameValueRec visibilityNotify[] = {
    {"Unobscured",	    0,	VisibilityUnobscured},
    {"PartiallyObscured",   0,	VisibilityPartiallyObscured},
    {"FullyObscured",       0,	VisibilityFullyObscured},
    {NULL, NULL, NULL},
};

static NameValueRec circulation[] = {
    {"OnTop",       0,	PlaceOnTop},
    {"OnBottom",    0,	PlaceOnBottom},
    {NULL, NULL, NULL},
};

static NameValueRec propertyChanged[] = {
    {"NewValue",    0,	PropertyNewValue},
    {"Delete",      0,	PropertyDelete},
    {NULL, NULL, NULL},
};

static NameValueRec mappingNotify[] = {
    {"Modifier",	0,	MappingModifier},
    {"Keyboard",	0,	MappingKeyboard},
    {"Pointer",	0,	MappingPointer},
    {NULL, NULL, NULL},
};

static String ParseKeySym();
static String ParseKeyAndModifiers();
static String ParseTable();
static String ParseImmed();
static String ParseAddModifier();
static String ParseNone();
static String ParseAtom();

static EventKey events[] = {

/* Event Name,	  Quark, Event Type,	Detail Parser, Closure */

{"KeyPress",	    NULL, KeyPress,	ParseKeySym,	NULL},
{"Key", 	    NULL, KeyPress,	ParseKeySym,	NULL},
{"KeyDown",	    NULL, KeyPress,	ParseKeySym,	NULL},
{"Ctrl",            NULL, KeyPress,  ParseKeyAndModifiers,(Opaque)ControlMask},
{"Shift",           NULL, KeyPress,    ParseKeyAndModifiers,(Opaque)ShiftMask},
{"Meta",            NULL, KeyPress,     ParseKeyAndModifiers,(Opaque)NULL},
{"KeyUp",	    NULL, KeyRelease,	ParseKeySym,	NULL},
{"KeyRelease",	    NULL, KeyRelease,	ParseKeySym,	NULL},

{"ButtonPress",     NULL, ButtonPress,  ParseTable,(Opaque)buttonNames},
{"BtnDown",	    NULL, ButtonPress,  ParseTable,(Opaque)buttonNames},
{"Btn1Down",	    NULL, ButtonPress,	ParseImmed,(Opaque)Button1},
{"Btn2Down", 	    NULL, ButtonPress,	ParseImmed,(Opaque)Button2},
{"Btn3Down", 	    NULL, ButtonPress,	ParseImmed,(Opaque)Button3},
{"Btn4Down", 	    NULL, ButtonPress,	ParseImmed,(Opaque)Button4},
{"Btn5Down", 	    NULL, ButtonPress,	ParseImmed,(Opaque)Button5},

/* Event Name,	  Quark, Event Type,	Detail Parser, Closure */

{"ButtonRelease",   NULL, ButtonRelease,    ParseTable,(Opaque)buttonNames},
{"BtnUp", 	    NULL, ButtonRelease,    ParseTable,(Opaque)buttonNames},
{"Btn1Up", 	    NULL, ButtonRelease,    ParseImmed,(Opaque)Button1},
{"Btn2Up", 	    NULL, ButtonRelease,    ParseImmed,(Opaque)Button2},
{"Btn3Up", 	    NULL, ButtonRelease,    ParseImmed,(Opaque)Button3},
{"Btn4Up", 	    NULL, ButtonRelease,    ParseImmed,(Opaque)Button4},
{"Btn5Up", 	    NULL, ButtonRelease,    ParseImmed,(Opaque)Button5},

{"MotionNotify",    NULL, MotionNotify,	ParseTable,	(Opaque)motionDetails},
{"PtrMoved", 	    NULL, MotionNotify,	ParseTable,	(Opaque)motionDetails},
{"Motion", 	    NULL, MotionNotify,	ParseTable,	(Opaque)motionDetails},
{"MouseMoved", 	    NULL, MotionNotify,	ParseTable,	(Opaque)motionDetails},
{"BtnMotion",       NULL, MotionNotify,ParseAddModifier,(Opaque)AnyButtonMask},
{"Btn1Motion",      NULL, MotionNotify, ParseAddModifier, (Opaque)Button1Mask},
{"Btn2Motion",      NULL, MotionNotify, ParseAddModifier, (Opaque)Button2Mask},
{"Btn3Motion",      NULL, MotionNotify, ParseAddModifier, (Opaque)Button3Mask},
{"Btn4Motion",      NULL, MotionNotify, ParseAddModifier, (Opaque)Button4Mask},
{"Btn5Motion",      NULL, MotionNotify, ParseAddModifier, (Opaque)Button5Mask},

{"EnterNotify",     NULL, EnterNotify,    ParseTable,(Opaque)notifyModes},
{"Enter",	    NULL, EnterNotify,    ParseTable,(Opaque)notifyModes},
{"EnterWindow",     NULL, EnterNotify,    ParseTable,(Opaque)notifyModes},

{"LeaveNotify",     NULL, LeaveNotify,    ParseTable,(Opaque)notifyModes},
{"LeaveWindow",     NULL, LeaveNotify,    ParseTable,(Opaque)notifyModes},
{"Leave",	    NULL, LeaveNotify,    ParseTable,(Opaque)notifyModes},

/* Event Name,	  Quark, Event Type,	Detail Parser, Closure */

{"FocusIn",	    NULL, FocusIn,	  ParseTable,(Opaque)notifyModes},

{"FocusOut",	    NULL, FocusOut,       ParseTable,(Opaque)notifyModes},

{"KeymapNotify",    NULL, KeymapNotify,	ParseNone,	NULL},
{"Keymap",	    NULL, KeymapNotify,	ParseNone,	NULL},

{"Expose", 	    NULL, Expose,		ParseNone,	NULL},

{"GraphicsExpose",  NULL, GraphicsExpose,	ParseNone,	NULL},
{"GrExp",	    NULL, GraphicsExpose,	ParseNone,	NULL},

{"NoExpose",	    NULL, NoExpose,	ParseNone,	NULL},
{"NoExp",	    NULL, NoExpose,	ParseNone,	NULL},

{"VisibilityNotify",NULL, VisibilityNotify,ParseNone,	NULL},
{"Visible",	    NULL, VisibilityNotify,ParseNone,	NULL},

{"CreateNotify",    NULL, CreateNotify,	ParseNone,	NULL},
{"Create",	    NULL, CreateNotify,	ParseNone,	NULL},

/* Event Name,	  Quark, Event Type,	Detail Parser, Closure */

{"DestroyNotify",   NULL, DestroyNotify,	ParseNone,	NULL},
{"Destroy",	    NULL, DestroyNotify,	ParseNone,	NULL},

{"UnmapNotify",     NULL, UnmapNotify,	ParseNone,	NULL},
{"Unmap",	    NULL, UnmapNotify,	ParseNone,	NULL},

{"MapNotify",	    NULL, MapNotify,	ParseNone,	NULL},
{"Map",		    NULL, MapNotify,	ParseNone,	NULL},

{"MapRequest",	    NULL, MapRequest,	ParseNone,	NULL},
{"MapReq",	    NULL, MapRequest,	ParseNone,	NULL},

{"ReparentNotify",  NULL, ReparentNotify,	ParseNone,	NULL},
{"Reparent",	    NULL, ReparentNotify,	ParseNone,	NULL},

{"ConfigureNotify", NULL, ConfigureNotify,	ParseNone,	NULL},
{"Configure",	    NULL, ConfigureNotify,	ParseNone,	NULL},

{"ConfigureRequest",NULL, ConfigureRequest,ParseNone,	NULL},
{"ConfigureReq",    NULL, ConfigureRequest,ParseNone,	NULL},

/* Event Name,	  Quark, Event Type,	Detail Parser, Closure */

{"GravityNotify",   NULL, GravityNotify,	ParseNone,	NULL},
{"Grav",	    NULL, GravityNotify,	ParseNone,	NULL},

{"ResizeRequest",   NULL, ResizeRequest,	ParseNone,	NULL},
{"ResReq",	    NULL, ResizeRequest,	ParseNone,	NULL},

{"CirculateNotify", NULL, CirculateNotify,	ParseNone,	NULL},
{"Circ",	    NULL, CirculateNotify,	ParseNone,	NULL},

{"CirculateRequest",NULL, CirculateRequest,ParseNone,	NULL},
{"CircReq",	    NULL, CirculateRequest,ParseNone,	NULL},

{"PropertyNotify",  NULL, PropertyNotify,	ParseAtom,	NULL},
{"Prop",	    NULL, PropertyNotify,	ParseAtom,	NULL},

{"SelectionClear",  NULL, SelectionClear,	ParseAtom,	NULL},
{"SelClr",	    NULL, SelectionClear,	ParseAtom,	NULL},

{"SelectionRequest",NULL, SelectionRequest,	ParseAtom,	NULL},
{"SelReq",	    NULL, SelectionRequest,	ParseAtom,	NULL},

/* Event Name,	  Quark, Event Type,	Detail Parser, Closure */

{"SelectionNotify", NULL, SelectionNotify,	ParseAtom,	NULL},
{"Select",	    NULL, SelectionNotify,	ParseAtom,	NULL},

{"ColormapNotify",  NULL, ColormapNotify,	ParseNone,	NULL},
{"Clrmap",	    NULL, ColormapNotify,	ParseNone,	NULL},

{"ClientMessage",   NULL, ClientMessage,	ParseAtom,	NULL},
{"Message",	    NULL, ClientMessage,	ParseAtom,	NULL},

{"MappingNotify",   NULL,MappingNotify, ParseTable,	(Opaque)mappingNotify},
{"Mapping",	    NULL,MappingNotify,	ParseTable,	(Opaque)mappingNotify},

#ifdef notdef
{"Timer",	    NULL, _XtTimerEventType,ParseNone,	NULL},

{"EventTimer",	    NULL, _XtEventTimerEventType,ParseNone,NULL},
#endif

/* Event Name,	  Quark, Event Type,	Detail Parser, Closure */

{ NULL, NULL, NULL, NULL, NULL}};

static Boolean initialized = FALSE;

static void FreeEventSeq(eventSeq)
    EventSeqPtr eventSeq;
{
    register EventSeqPtr evs = eventSeq;

    while (evs != NULL) {
	evs->state = (StatePtr) evs;
	if (evs->next != NULL
	    && evs->next->state == (StatePtr) evs->next)
	    evs->next = NULL;
	evs = evs->next;
    }

    evs = eventSeq;
    while (evs != NULL) {
	register EventPtr event = evs;
	evs = evs->next;
	if (evs == event) evs = NULL;
	XtFree((char *)event);
    }
}

static void CompileNameValueTable(table)
    NameValueTable table;
{
    register int i;

    for (i=0; table[i].name; i++)
        table[i].signature = StringToQuark(table[i].name);
}

static void Compile_XtEventTable(table)
    EventKeys	table;
{
    register int i;

    for (i=0; table[i].event; i++)
        table[i].signature = StringToQuark(table[i].event);
}
static void Compile_XtModifierTable(table)
    ModifierKeys table;

{
    register int i;

    for (i=0; table[i].name; i++)
        table[i].signature = StringToQuark(table[i].name);
}

static String PanicModeRecovery(str)
    String str;
{
     str = ScanFor(str,'\n');
     if (*str == '\n') str++;
     return str;

}


static Syntax(str,str1)
    String str,str1;
{
    char message[1000];
    Cardinal numChars;
    Cardinal num_params = 1;
    String params[1];
    (void)strcpy(message,str);
    numChars = strlen(message);
    (void) strcpy(&message[numChars], str1);
    numChars += strlen(str1);
    message[numChars] = '\0';
    params[0] = message;
  XtWarningMsg("translationParseError","parseError","XtToolkitError",
            "translation table syntax error: %s",params,&num_params);
}



static Cardinal LookupTMEventType(eventStr,error)
  String eventStr;
  Boolean *error;
{
    register Cardinal   i;
    register XrmQuark	signature;

    signature = StringToQuark(eventStr);
    for (i = 0; events[i].signature != NULL; i++)
        if (events[i].signature == signature) return i;

    Syntax("Unknown event type :  ",eventStr);
    *error = TRUE;
    return i;
}

/***********************************************************************
 * _XtLookupTableSym
 * Given a table and string, it fills in the value if found and returns
 * status
 ***********************************************************************/

static Boolean _XtLookupTableSym(table, name, valueP)
    NameValueTable	table;
    String name;
    Value *valueP;
{
/* ||| should implement via hash or something else faster than linear search */

    register int i;
    register XrmQuark signature = StringToQuark(name);

    for (i=0; table[i].name != NULL; i++)
	if (table[i].signature == signature) {
	    *valueP = table[i].value;
	    return TRUE;
	}

    return FALSE;
}




static void StoreLateBindings(keysymL,notL,keysymR,notR,lateBindings)

    KeySym  keysymL;
    Boolean notL;
    KeySym keysymR;
    Boolean notR;
    LateBindingsPtr* lateBindings;
{
    LateBindingsPtr temp;
    Boolean pair = FALSE;
    unsigned long count,number;
    if (lateBindings != NULL){
        temp = *lateBindings;
        if (temp != NULL) {
            for (count = 0; temp[count].keysym != NULL; count++){}
        }
        else count = 0;
        if (keysymR == NULL){
             number = 1;pair = FALSE;
        } else{
             number = 2;pair = TRUE;
        }
          
        temp = (LateBindingsPtr)XtRealloc((XtPointer)temp,
            (unsigned)((count+number+1) * sizeof(LateBindings)) );
        *lateBindings = temp;
        temp[count].knot = notL;
        temp[count].pair = pair;
        temp[count++].keysym = keysymL;
        if (keysymR != NULL){
            temp[count].knot = notR;
            temp[count].pair = FALSE;
            temp[count++].keysym = keysymR;
        }
        temp[count].knot = NULL;
        temp[count].keysym = NULL;
    }
    
} 
static _XtParseKeysymMod(name,lateBindings,notFlag,valueP,error)
    String name;
    LateBindingsPtr* lateBindings;
    Boolean notFlag;
    Value *valueP;
    Boolean *error;
{
    KeySym keySym;
    keySym = StringToKeySym(name, error);
    *valueP = 0;
    if (keySym != NoSymbol) {
        StoreLateBindings(keySym,notFlag,(KeySym) NULL,FALSE,lateBindings);
    }
}

static Boolean _XtLookupModifier(name,lateBindings,notFlag,valueP,check)
    String name;
    LateBindingsPtr* lateBindings;
    Boolean notFlag;
    Value *valueP;
    Bool check;
{
   register int i;
   register XrmQuark signature = StringToQuark(name);
   for (i=0; modifiers[i].name != NULL; i++)
      if (modifiers[i].signature == signature) {
          if (check == TRUE)  *valueP = modifiers[i].value;
          if ((modifiers[i].modifierParseProc != NULL) && (check == FALSE))
            (*modifiers[i].modifierParseProc)(name,
                modifiers[i].value,lateBindings,notFlag,valueP);
      return TRUE;
      }
   return FALSE;
}


static String ScanFor(str, ch)
    register String str;
    register char ch;
{
    while ((*str != ch) &&( *str != '\0') &&(*str != '\n') ) str++;
    return str;
}

static String ScanNumeric(str)
    register String str;
{
    while ('0' <= *str && *str <= '9') str++;
    return str;
}

static String ScanAlphanumeric(str)
    register String str;
{
    while (
        ('A' <= *str && *str <= 'Z') || ('a' <= *str && *str <= 'z')
	|| ('0' <= *str && *str <= '9')) str++;
    return str;
}

static String ScanIdent(str)
    register String str;
{
    str = ScanAlphanumeric(str);
    while (
	   ('A' <= *str && *str <= 'Z')
	|| ('a' <= *str && *str <= 'z')
	|| ('0' <= *str && *str <= '9')
	|| (*str == '-')
	|| (*str == '_')
	|| (*str == '$')
	) str++;
    return str;
}

static String ScanWhitespace(str)
    register String str;
{
    while (*str == ' ' || *str == '\t') str++;
    return str;
}
static String FetchModifierToken(str,modStr)
    String str,modStr;
{
    String start = str;
    String metaString = "Alt";
    String ctrlString = "Ctrl";
    if (*str == '$') {
        strcpy(modStr,metaString);
        str++;
        return str;
    }
    if (*str == '^') {
        strcpy(modStr,ctrlString);
        str++;
        return str;
    }
    str = ScanIdent(str);
    if (start != str) {
         (void) strncpy(modStr, start, str-start);
          modStr[str-start] = '\0';
          return str;
    }
    return str;
}        
    
static String ParseModifiers(str, event,error)
    register String str;
    EventPtr event;
    Boolean* error;
{
    register String start;
    char modStr[100];
    Boolean notFlag, exclusive, keysymAsMod;
    Value maskBit;
 
    str = ScanWhitespace(str);
    start = str;
    str = FetchModifierToken(str,modStr);
    exclusive = FALSE;
    if (start != str) {
          if (_XtLookupModifier(modStr,(LateBindingsPtr *) NULL,
		  FALSE,&maskBit,TRUE))
	    if (maskBit== None) {
                event->event.modifierMask = ~0;
		event->event.modifiers = 0;
                str = ScanWhitespace(str);
	        return str;
            }
            if (maskBit == AnyModifier) {/*backward compatability*/
                event->event.modifierMask = 0;
                event->event.modifiers = 0;
                str = ScanWhitespace(str);
                return str;
            }
         str = start; /*if plain modifier, reset to beginning */
    }
    else while (*str == '!' || *str == ':') {
        if (*str == '!') {
             exclusive = TRUE;
             str++;
             str = ScanWhitespace(str);
        }
        if (*str == ':') {
             event->event.standard = TRUE;
             str++;
             str = ScanWhitespace(str);
        }
    }
   
    while (*str != '<') {
        if (*str == '~') {
             notFlag = TRUE;
             str++;
          } else 
              notFlag = FALSE;
        if (*str == '@') {
            keysymAsMod = TRUE;
            str++;
        }
        else keysymAsMod = FALSE;
	start = str;
        str = FetchModifierToken(str,modStr);
        if (start == str) {
            Syntax("Modifier or '<' expected","");
            *error = TRUE;
            return PanicModeRecovery(str);
        }
         if (keysymAsMod) {
             _XtParseKeysymMod(modStr,&event->event.lateModifiers,
			       notFlag,&maskBit, error);
	     if (*error)
                 return PanicModeRecovery(str);

         } else
  	     if (!_XtLookupModifier( modStr,
	   &event->event.lateModifiers, notFlag, &maskBit,FALSE)) {
	         Syntax("Unknown modifier name:  ",modStr);
                 *error = TRUE;
                 return PanicModeRecovery(str);
             }
        event->event.modifierMask |= maskBit;
	if (notFlag) event->event.modifiers &= ~maskBit;
	else event->event.modifiers |= maskBit;
        str = ScanWhitespace(str);
    }
    if (exclusive) event->event.modifierMask = ~0;
    return str;
}

static String ParseXtEventType(str, event, tmEventP,error)
    register String str;
    EventPtr event;
    Cardinal *tmEventP;
    Boolean* error;
{
    String start = str;
    char eventTypeStr[100];

    str = ScanAlphanumeric(str);
    (void) strncpy(eventTypeStr, start, str-start);
    eventTypeStr[str-start] = '\0';
    *tmEventP = LookupTMEventType(eventTypeStr,error);
    if (*error) 
        return PanicModeRecovery(str);
    event->event.eventType = events[*tmEventP].eventType;
    return str;
}

static unsigned long StrToHex(str)
    String str;
{
    register char   c;
    register unsigned long    val = 0;

    while (c = *str) {
	if ('0' <= c && c <= '9') val = val*16+c-'0';
	else if ('a' <= c && c <= 'z') val = val*16+c-'a'+10;
	else if ('A' <= c && c <= 'Z') val = val*16+c-'A'+10;
	else return 0;
	str++;
    }

    return val;
}

static unsigned long StrToOct(str)
    String str;
{
    register char c;
    register unsigned long  val = 0;

    while (c = *str) {
        if ('0' <= c && c <= '7') val = val*8+c-'0'; else return 0;
	str++;
    }

    return val;
}

static unsigned long StrToNum(str)
    String str;
{
    register char c;
    register unsigned long val = 0;

    if (*str == '0') {
	str++;
	if (*str == 'x' || *str == 'X') return StrToHex(++str);
	else return StrToOct(str);
    }

    while (c = *str) {
	if ('0' <= c && c <= '9') val = val*10+c-'0';
	else return 0;
	str++;
    }

    return val;
}

static KeySym StringToKeySym(str, error)
    String str;
    Boolean *error;
{
    KeySym k;

    if (str == NULL || *str == '\0') return (KeySym) 0;

#ifndef NOTASCII
    /* special case single character ASCII, for speed */
    if (*(str+1) == '\0') {
	if (' ' <= *str && *str <= '~') return XK_space + (*str - ' ');
    }
#endif

    k = XStringToKeysym(str);
    if (k != NoSymbol) return k;

    if ('0' <= *str && *str <= '9') return (KeySym) StrToNum(str);

#ifdef NOTASCII
    /* fall-back case to preserve backwards compatibility; no-one
     * should be relying upon this!
     */
    if (*(str+1) == '\0') return (KeySym) *str;
#endif

    Syntax("Unknown keysym name: ", str);
    *error = True;
    return NoSymbol;
}
/* ARGSUSED */
static void ParseModImmed(name,value,lateBindings,notFlag,valueP)
    String name;
    Value value;
    LateBindingsPtr* lateBindings;
    Boolean notFlag;
    Value* valueP;
{
    *valueP = value;
}
/* ARGSUSED */
static void ParseModSym (name,value,lateBindings,notFlag,valueP)
 /* is only valid with keysyms that have an _L and _R in their name;
  * and ignores keysym lookup errors (i.e. assumes only valid keysyms)
  */
    String name;
    Value value;
    LateBindingsPtr* lateBindings;
    Boolean notFlag;
    Value* valueP;
{
    register KeySym keysymL = (KeySym)value;
    register KeySym keysymR = keysymL + 1; /* valid for supported keysyms */
    StoreLateBindings(keysymL,notFlag,keysymR,notFlag,lateBindings);
    *valueP = 0;
}

#ifdef sparc
/*
 * The stupid optimizer in SunOS 4.0.3 and below generates bogus code that
 * causes the value of the most recently used variable to be returned instead
 * of the value passed in.
 */
static String stupid_optimizer_kludge;
#define BROKEN_OPTIMIZER_HACK(val) stupid_optimizer_kludge = (val)
#else
#define BROKEN_OPTIMIZER_HACK(val) val
#endif

/* ARGSUSED */
static String ParseImmed(str, closure, event,error)
    register String str;
    register Opaque closure;
    register EventPtr event;
    Boolean* error;
{
    event->event.eventCode = (unsigned long)closure;
    event->event.eventCodeMask = (unsigned long)~0L;

    return BROKEN_OPTIMIZER_HACK(str);
}

/* ARGSUSED */
static String ParseAddModifier(str, closure, event, error)
    register String str;
    register Opaque closure;
    register EventPtr event;
    Boolean* error;
{
    register unsigned long modval = (unsigned long)closure;
    event->event.modifiers |= modval;
    if (modval != AnyButtonMask) /* AnyButtonMask is don't-care mask */
	event->event.modifierMask |= modval;

    return BROKEN_OPTIMIZER_HACK(str);
}


static String ParseKeyAndModifiers(str, closure, event,error)
    String str;
    Opaque closure;
    EventPtr event;
    Boolean* error;
{
    str = ParseKeySym(str, closure, event,error);

    event->event.modifiers |= (unsigned long)closure;
    event->event.modifierMask |= (unsigned long)closure;

    return str;
}

/*ARGSUSED*/
static String ParseKeySym(str, closure, event,error)
    register String str;
    Opaque closure;
    EventPtr event;
    Boolean* error;
{
    char keySymName[100], *start;

    str = ScanWhitespace(str);

    if (*str == '\\') {
	str++;
	keySymName[0] = *str;
	if (*str != '\0' && *str != '\n') str++;
	keySymName[1] = '\0';
	event->event.eventCode = StringToKeySym(keySymName, error);
	event->event.eventCodeMask = ~0L;
    } else if (*str == ',' || *str == ':') {
	/* no detail */
	event->event.eventCode = 0L;
        event->event.eventCodeMask = 0L;
    } else {
	start = str;
	while (
		*str != ','
		&& *str != ':'
		&& *str != ' '
		&& *str != '\t'
                && *str != '\n'
		&& *str != '\0') str++;
	(void) strncpy(keySymName, start, str-start);
	keySymName[str-start] = '\0';
	event->event.eventCode = StringToKeySym(keySymName, error);
	event->event.eventCodeMask = ~0L;
    }
    if (*error) {
	if (keySymName[0] == '<') {
	    /* special case for common error */
	    XtWarningMsg("translationParseError", "missingComma",
			 "XtToolkitError",
		     "... possibly due to missing ',' in event sequence.",
		     (String*)NULL, (Cardinal*)NULL);
	}
	return PanicModeRecovery(str);
    }
    if (event->event.standard) event->event.matchEvent = 
        _XtMatchUsingStandardMods;
    else event->event.matchEvent = _XtMatchUsingDontCareMods;
    return str;
}

static String ParseTable(str, closure, event,error)
    register String str;
    Opaque closure;
    EventPtr event;
    Boolean* error;
{
    register String start = str;
    char tableSymName[100];

    event->event.eventCode = 0L;
    str = ScanAlphanumeric(str);
    if (str == start) {event->event.eventCodeMask = 0L; return str; }
    if (str-start >= 99) {
	Syntax("Invalid Detail Type (string is too long).", "");
	*error = TRUE;
	return str;
    }
    (void) strncpy(tableSymName, start, str-start);
    tableSymName[str-start] = '\0';
    if (! _XtLookupTableSym((NameValueTable)closure, tableSymName, 
            (Value *)&event->event.eventCode)) {
	Syntax("Unknown Detail Type:  ",tableSymName);
        *error = TRUE;
        return PanicModeRecovery(str);
    }
    event->event.eventCodeMask = ~0L;

    return str;
}

/*ARGSUSED*/
static String ParseNone(str, closure, event,error)
    String str;
    Opaque closure;
    EventPtr event;
    Boolean* error;
{
    event->event.eventCode = 0;
    event->event.eventCodeMask = 0;

    return BROKEN_OPTIMIZER_HACK(str);
}

/*ARGSUSED*/
static String ParseAtom(str, closure, event,error)
    String str;
    Opaque closure;
    EventPtr event;
    Boolean* error;
{
    char atomName[1000], *start;

    str = ScanWhitespace(str);

    if (*str == ',' || *str == ':') {
	/* no detail */
	event->event.eventCode = 0L;
        event->event.eventCodeMask = 0L;
    } else {
	start = str;
	while (
		*str != ','
		&& *str != ':'
		&& *str != ' '
		&& *str != '\t'
                && *str != '\n'
		&& *str != '\0') str++;
	if (str-start >= 999) {
	    Syntax( "Atom name must be less than 1000 characters long.", "" );
	    *error = TRUE;
	    return str;
	}
	(void) strncpy(atomName, start, str-start);
	atomName[str-start] = '\0';
	event->event.eventCode = XrmStringToQuark(atomName);
	event->event.eventCodeMask = ~0L;
	event->event.matchEvent = _XtMatchAtom;
    }
    return str;
}

static ModifierMask buttonModifierMasks[] = {
    0, Button1Mask, Button2Mask, Button3Mask, Button4Mask, Button5Mask
};

static String ParseEvent(str, event,error)
    register String str;
    EventPtr	event;
    Boolean* error;
{
    Cardinal	tmEvent;

    str = ParseModifiers(str, event,error);
    if (*error) return str;
    if (*str != '<') {
         Syntax("Missing '<' while parsing event type.",""); 
         *error = TRUE;
         return PanicModeRecovery(str);
    }
    else str++;
    str = ParseXtEventType(str, event, &tmEvent,error);
    if (*error) return str;
    if (*str != '>'){
         Syntax("Missing '>' while parsing event type","");
         *error = TRUE;
         return PanicModeRecovery(str);
    }
    else str++;
    str = (*(events[tmEvent].parseDetail))(
        str, events[tmEvent].closure, event,error);
    if (*error) return str;

/* gross hack! ||| this kludge is related to the X11 protocol deficiency w.r.t.
 * modifiers in grabs.
 */
    if ((event->event.eventType == ButtonRelease)
	&& (event->event.modifiers | event->event.modifierMask) /* any */
        && (event->event.modifiers != AnyModifier))
    {
	event->event.modifiers
	    |= buttonModifierMasks[event->event.eventCode];
	/* the button that is going up will always be in the modifiers... */
    }

    return str;
}

static String ParseQuotedStringEvent(str, event,error)
    register String str;
    register EventPtr event;
    Boolean *error;
{
    register int j;

    Value ctrlMask;
    Value metaMask;
    Value shiftMask;
    register char	c;
    char	s[2];
    Cardinal	tmEvent;
    (void) _XtLookupModifier("Ctrl",(LateBindingsPtr*)NULL,
                 FALSE,(Value *) &ctrlMask,TRUE);
    (void) _XtLookupModifier("Alt",(LateBindingsPtr*)NULL,
                 FALSE,(Value *) &metaMask,TRUE);
    (void) _XtLookupModifier("Shift",(LateBindingsPtr*)NULL,
                 FALSE,(Value *) &shiftMask,TRUE);
    for (j=0; j < 2; j++)
	if (*str=='^' && !(event->event.modifiers | ctrlMask)) {
	    str++;
	    event->event.modifiers |= ctrlMask;
	} else if (*str == '$' && !(event->event.modifiers | metaMask)) {
	    str++;
	    event->event.modifiers |= metaMask;
	} else if (*str == '\\') {
	    str++;
	    c = *str;
	    if (*str != '\0' && *str != '\n') str++;
	    break;
	} else {
	    c = *str;
	    if (*str != '\0' && *str != '\n') str++;
	    break;
	}
    tmEvent = (EventType) LookupTMEventType("Key",error);
    if (*error)
        return PanicModeRecovery(str);

    event->event.eventType = events[tmEvent].eventType;
    if ('A' <= c && c <= 'Z') {
	event->event.modifiers |=  shiftMask;
	c += 'a' - 'A';
    }
    s[0] = c;
    s[1] = '\0';
    event->event.eventCode = StringToKeySym(s, error);

    return str;
}

static void RepeatDown(eventP, reps, actionsP)
    EventPtr *eventP;
    int reps;
    ActionPtr **actionsP;
{
    EventRec upEventRec;
    register EventPtr event, downEvent;
    EventPtr upEvent = &upEventRec;
    register int i;

    static EventSeqRec timerEventRec = {
	{0, 0,NULL, _XtEventTimerEventType, 0L, 0L,NULL},
	/* (StatePtr) -1 */ NULL,
	NULL,
	NULL
    };

    downEvent = event = *eventP;
    *upEvent = *downEvent;
    upEvent->event.eventType = ((event->event.eventType == ButtonPress) ?
	ButtonRelease : KeyRelease);
    if ((upEvent->event.eventType == ButtonRelease)
	&& (upEvent->event.modifiers != AnyModifier)
        && (upEvent->event.modifiers | upEvent->event.modifierMask))
	upEvent->event.modifiers
	    |= buttonModifierMasks[event->event.eventCode];

    for (i=1; i<reps; i++) {

	/* up */
	event->next = XtNew(EventSeqRec);
	event = event->next;
	*event = *upEvent;

	/* timer */
	event->next = XtNew(EventSeqRec);
	event = event->next;
	*event = timerEventRec;

	/* down */
	event->next = XtNew(EventSeqRec);
	event = event->next;
	*event = *downEvent;

    }

    event->next = NULL;
    *eventP = event;
    *actionsP = &event->actions;
}

static void RepeatDownPlus(eventP, reps, actionsP)
    EventPtr *eventP;
    int reps;
    ActionPtr **actionsP;
{
    EventRec upEventRec;
    register EventPtr event, downEvent, lastDownEvent;
    EventPtr upEvent = &upEventRec;
    register int i;

    static EventSeqRec timerEventRec = {
	{0, 0,NULL, _XtEventTimerEventType, 0L, 0L,NULL},
	/* (StatePtr) -1 */ NULL,
	NULL,
	NULL
    };

    downEvent = event = *eventP;
    *upEvent = *downEvent;
    upEvent->event.eventType = ((event->event.eventType == ButtonPress) ?
	ButtonRelease : KeyRelease);
    if ((upEvent->event.eventType == ButtonRelease)
	&& (upEvent->event.modifiers != AnyModifier)
        && (upEvent->event.modifiers | upEvent->event.modifierMask))
	upEvent->event.modifiers
	    |= buttonModifierMasks[event->event.eventCode];

    for (i=0; i<reps; i++) {

	if (i > 0) {
	/* down */
	event->next = XtNew(EventSeqRec);
	event = event->next;
	*event = *downEvent;
	}
	lastDownEvent = event;

	/* up */
	event->next = XtNew(EventSeqRec);
	event = event->next;
	*event = *upEvent;

	/* timer */
	event->next = XtNew(EventSeqRec);
	event = event->next;
	*event = timerEventRec;

    }

    event->next = lastDownEvent;
    *eventP = event;
    *actionsP = &lastDownEvent->actions;
}

static void RepeatUp(eventP, reps, actionsP)
    EventPtr *eventP;
    int reps;
    ActionPtr **actionsP;
{
    EventRec upEventRec;
    register EventPtr event, downEvent;
    EventPtr upEvent = &upEventRec;
    register int i;

    static EventSeqRec timerEventRec = {
	{0, 0,NULL, _XtEventTimerEventType, 0L, 0L,NULL},
	/* (StatePtr) -1 */ NULL,
	NULL,
	NULL
    };

    /* the event currently sitting in *eventP is an "up" event */
    /* we want to make it a "down" event followed by an "up" event, */
    /* so that sequence matching on the "state" side works correctly. */

    downEvent = event = *eventP;
    *upEvent = *downEvent;
    downEvent->event.eventType = ((event->event.eventType == ButtonRelease) ?
	ButtonPress : KeyPress);
    if ((downEvent->event.eventType == ButtonPress)
	&& (downEvent->event.modifiers != AnyModifier)
        && (downEvent->event.modifiers | downEvent->event.modifierMask))
	downEvent->event.modifiers
	    &= ~buttonModifierMasks[event->event.eventCode];

    /* up */
    event->next = XtNew(EventSeqRec);
    event = event->next;
    *event = *upEvent;

    for (i=1; i<reps; i++) {

	/* timer */
	event->next = XtNew(EventSeqRec);
	event = event->next;
	*event = timerEventRec;

	/* down */
	event->next = XtNew(EventSeqRec);
	event = event->next;
	*event = *downEvent;

	/* up */
	event->next = XtNew(EventSeqRec);
	event = event->next;
	*event = *upEvent;

	}

    event->next = NULL;
    *eventP = event;
    *actionsP = &event->actions;
}

static void RepeatUpPlus(eventP, reps, actionsP)
    EventPtr *eventP;
    int reps;
    ActionPtr **actionsP;
{
    EventRec upEventRec;
    register EventPtr event, downEvent, lastUpEvent;
    EventPtr upEvent = &upEventRec;
    register int i;

    static EventSeqRec timerEventRec = {
	{0, 0,NULL, _XtEventTimerEventType, 0L, 0L,NULL},
	/* (StatePtr) -1 */ NULL,
	NULL,
	NULL
    };

    /* the event currently sitting in *eventP is an "up" event */
    /* we want to make it a "down" event followed by an "up" event, */
    /* so that sequence matching on the "state" side works correctly. */

    downEvent = event = *eventP;
    *upEvent = *downEvent;
    downEvent->event.eventType = ((event->event.eventType == ButtonRelease) ?
	ButtonPress : KeyPress);
    if ((downEvent->event.eventType == ButtonPress)
	&& (downEvent->event.modifiers != AnyModifier)
        && (downEvent->event.modifiers | downEvent->event.modifierMask))
	downEvent->event.modifiers
	    &= ~buttonModifierMasks[event->event.eventCode];

    for (i=0; i<reps; i++) {

	/* up */
	event->next = XtNew(EventSeqRec);
	lastUpEvent = event = event->next;
	*event = *upEvent;

	/* timer */
	event->next = XtNew(EventSeqRec);
	event = event->next;
	*event = timerEventRec;

	/* down */
	event->next = XtNew(EventSeqRec);
        event = event->next;
	*event = *downEvent;

	}

    event->next = lastUpEvent;
    *eventP = event;
    *actionsP = &lastUpEvent->actions;
}

static void RepeatOther(eventP, reps, actionsP)
    EventPtr *eventP;
    int reps;
    ActionPtr **actionsP;
{
    register EventPtr event, tempEvent;
    register int i;

    tempEvent = event = *eventP;

    for (i=1; i<reps; i++) {
	event->next = XtNew(EventSeqRec);
	event = event->next;
	*event = *tempEvent;
    }

    *eventP = event;
    *actionsP = &event->actions;
}

static void RepeatOtherPlus(eventP, reps, actionsP)
    EventPtr *eventP;
    int reps;
    ActionPtr **actionsP;
{
    register EventPtr event, tempEvent;
    register int i;

    tempEvent = event = *eventP;

    for (i=1; i<reps; i++) {
	event->next = XtNew(EventSeqRec);
	event = event->next;
	*event = *tempEvent;
    }

    event->next = event;
    *eventP = event;
    *actionsP = &event->actions;
}

static void RepeatEvent(eventP, reps, plus, actionsP)
    EventPtr *eventP;
    int reps;
    Boolean plus;
    ActionPtr **actionsP;
{
    switch ((*eventP)->event.eventType) {

	case ButtonPress:
	case KeyPress:
	    if (plus) RepeatDownPlus(eventP, reps, actionsP);
	    else RepeatDown(eventP, reps, actionsP);
	    break;

	case ButtonRelease:
	case KeyRelease:
	    if (plus) RepeatUpPlus(eventP, reps, actionsP);
	    else RepeatUp(eventP, reps, actionsP);
	    break;

	default:
	    if (plus) RepeatOtherPlus(eventP, reps, actionsP);
	    else RepeatOther(eventP, reps, actionsP);
    }
}

static String ParseRepeat(str, eventP, actionsP)
    register String str;
    EventPtr *eventP;
    ActionPtr **actionsP;
{
    int reps;
    Boolean plus = FALSE;
    String right_paren;

    /*** Parse the repetitions, for double click etc... ***/
    if (*str != '(') return str;
    str++;
    right_paren = ScanFor (str, ')');
    if (isascii(*str) && isdigit(*str)) {
	String start = str;
	char repStr[100];
	int len;

	str = ScanNumeric(str);
	len = (str - start);
	if (len < sizeof repStr) {
	    (void) strncpy (repStr, start, len);
	    repStr[len] = '\0';
	    reps = StrToNum(repStr);
	} else {
	    Syntax ("Repeat count too large; ignored.", "");
	    return right_paren;
	}
    } else {
	Syntax("Missing repeat count.","");
	return right_paren;
    }

    if (*str == '+') {
	plus = TRUE;
	str++;
    }
    if (*str == ')')
	str++;
    else {
	Syntax("Missing ')'.","");
	return right_paren;
    }

    if (reps > 1 || plus) RepeatEvent(eventP, reps, plus, actionsP);

    return str;
}

/***********************************************************************
 * ParseEventSeq
 * Parses the left hand side of a translation table production
 * up to, and consuming the ":".
 * Takes a pointer to a char* (where to start parsing) and returns an
 * event seq (in a passed in variable), having updated the String 
 **********************************************************************/

static String ParseEventSeq(str, eventSeqP, actionsP,error)
    register String str;
    EventSeqPtr *eventSeqP;
    ActionPtr	**actionsP;
    Boolean *error;
{
    EventSeqPtr *nextEvent = eventSeqP;

    *eventSeqP = NULL;

    while ( *str != '\0' && *str != '\n') {
	static Event	nullEvent =
             {0, 0,0L, 0, 0L, 0L,_XtRegularMatch,FALSE};
	EventPtr	event;

	str = ScanWhitespace(str);

	if (*str == '"') {
	    str++;
	    while (*str != '"' && *str != '\0' && *str != '\n') {
                event = XtNew(EventRec);
                event->event = nullEvent;
                event->state = /* (StatePtr) -1 */ NULL;
                event->next = NULL;
                event->actions = NULL;
		str = ParseQuotedStringEvent(str, event,error);
		if (*error) {
		    XtWarningMsg("translationParseError", "nonLatin1",
			"XtToolkitError",
			"... probably due to non-Latin1 character in quoted string",
			(String*)NULL, (Cardinal*)NULL);
		    return PanicModeRecovery(str);
		}
		*nextEvent = event;
		*actionsP = &event->actions;
		nextEvent = &event->next;
	    }
	    if (*str != '"') {
                Syntax("Missing '\"'.","");
                *error = TRUE;
                return PanicModeRecovery(str);
             }
             else str++;
	} else {
            event = XtNew(EventRec);
            event->event = nullEvent;
            event->state = /* (StatePtr) -1 */ NULL;
            event->next = NULL;
            event->actions = NULL;

	    str = ParseEvent(str, event,error);
            if (*error) return str;
	    *nextEvent = event;
	    *actionsP = &event->actions;
	    str = ParseRepeat(str, &event, actionsP);
	    nextEvent = &event->next;
	}
	str = ScanWhitespace(str);
        if (*str == ':') break;
        else {
            if (*str != ',') {
                Syntax("',' or ':' expected while parsing event sequence.","");
                *error = TRUE;
                return PanicModeRecovery(str);
	    } else str++;
        }
    }

    if (*str != ':') {
        Syntax("Missing ':'after event sequence.",""); 
        *error = TRUE;
        return PanicModeRecovery(str);
    } else str++;

    return str;
}


static String ParseActionProc(str, actionProcNameP, error)
    register String str;
    XrmQuark *actionProcNameP;
    Boolean *error;
{
    register String start = str;
    char procName[200];

    str = ScanIdent(str);
    if (str-start >= 199) {
	Syntax("Action procedure name is longer than 199 chars","");
	*error = TRUE;
	return str;
    }
    (void) strncpy(procName, start, str-start);
    procName[str-start] = '\0';
    *actionProcNameP = XrmStringToQuark( procName );
    return str;
}


static String ParseString(str, strP)
    register String str;
    String *strP;
{
    register String start;

    if (*str == '"') {
	str++;
	start = str;
	while (*str != '"' && *str != '\0' && *str != '\n') str++;
	*strP = strncpy(XtMalloc((unsigned)(str-start+1)), start, str-start);
	(*strP)[str-start] = '\0';
	if (*str == '"') str++; else
            XtWarningMsg("translationParseError","parseString",
                      "XtToolkitError","Missing '\"'.",
		      (String *)NULL, (Cardinal *)NULL);
    } else {
	/* scan non-quoted string, stop on whitespace, ',' or ')' */
	start = str;
	while (*str != ' '
		&& *str != '\t'
		&& *str != ','
		&& *str != ')'
                && *str != '\n'
		&& *str != '\0') str++;
	*strP = strncpy(XtMalloc((unsigned)(str-start+1)), start, str-start);
	(*strP)[str-start] = '\0';
    }
    return str;
}


static String ParseParamSeq(str, paramSeqP, paramNumP)
    register String str;
    String **paramSeqP;
    Cardinal *paramNumP;
{
    typedef struct _ParamRec *ParamPtr;
    typedef struct _ParamRec {
	ParamPtr next;
	String	param;
    } ParamRec;

    ParamPtr params = NULL;
    register Cardinal num_params = 0;
    register Cardinal i;

    str = ScanWhitespace(str);
    while (*str != ')' && *str != '\0' && *str != '\n') {
	String newStr;
	str = ParseString(str, &newStr);
	if (newStr != NULL) {
	    ParamPtr temp = (ParamRec*)
		ALLOCATE_LOCAL( (unsigned)sizeof(ParamRec) );

	    num_params++;
	    temp->next = params;
	    params = temp;
	    temp->param = newStr;
	    str = ScanWhitespace(str);
	    if (*str == ',') str = ScanWhitespace(++str);
	}
    }

    if (num_params != 0) {
	String *paramP =
	    *paramSeqP = (String *)
		XtMalloc( (unsigned)(num_params+1) * sizeof(String) );
	*paramNumP = num_params;
	paramP += num_params; /* list is LIFO right now */
	*paramP-- = NULL;
	for (i=0; i < num_params; i++) {
	    ParamPtr next = params->next;
	    *paramP-- = params->param;
	    DEALLOCATE_LOCAL( (XtPointer)params );
	    params = next;
	}
    } else {
	*paramSeqP = NULL;
	*paramNumP = 0;
    }

    return str;
}

static String ParseAction(str, actionP, quarkP, error)
    String str;
    ActionPtr actionP;
    XrmQuark* quarkP;
    Boolean* error;
{
    str = ParseActionProc(str, quarkP, error);
    if (*error) return str;

    if (*str == '(') {
	str++;
	str = ParseParamSeq(str, &actionP->params, &actionP->num_params);
    } else {
        Syntax("Missing '(' while parsing action sequence",""); 
        *error = TRUE;
        return str;
    }
    if (*str == ')') str++;
    else{
        Syntax("Missing ')' while parsing action sequence","");
        *error = TRUE;
        return str;
    }
    return str;
}


static String ParseActionSeq(stateTable,str, actionsP,acc,error)
    XtTranslations stateTable;
    String str;
    ActionPtr *actionsP;
    Bool acc;
    Boolean* error;
{
    ActionPtr *nextActionP = actionsP;
    int index;
    Boolean found;
    *actionsP = NULL;

    while (*str != '\0' && *str != '\n') {
	register ActionPtr	action;
	XrmQuark quark;

	action = XtNew(ActionRec);
        action->params = NULL;
        action->num_params = 0;
        action->next = NULL;

	str = ParseAction(str, action, &quark, error);
	if (*error) return PanicModeRecovery(str);

        if (!acc) { /*regular table */
            found = FALSE;
            for (index=0;index<stateTable->numQuarks;index++)
               if ((stateTable->quarkTable)[index]==quark){
                   found = TRUE;
                   break;
               }
            if (found==FALSE) {
                index = stateTable->numQuarks++;
                if (index==stateTable->quarkTblSize) {
                    stateTable->quarkTblSize += 10;
                    stateTable->quarkTable=(XrmQuark*) XtRealloc(
                        (XtPointer)stateTable->quarkTable,
                        stateTable->quarkTblSize*sizeof(XrmQuark));
                }

                (stateTable->quarkTable)[index] = quark;
             }
            action->index=index;
        }
        else { /*accelerator table */
            index = stateTable->accNumQuarks++;
            if (index == stateTable->accQuarkTblSize) {
                stateTable->accQuarkTblSize += 10;
                stateTable->accQuarkTable = (XrmQuark*) XtRealloc(
                   (XtPointer)stateTable->accQuarkTable,
                   stateTable->accQuarkTblSize*sizeof(XrmQuark));
            }
            stateTable->accQuarkTable[index] = quark;
            action->index= -(index+1);
        }

	str = ScanWhitespace(str);
	*nextActionP = action;
	nextActionP = &action->next;
    }
    if (*str == '\n') str++;
    str = ScanWhitespace(str);
    return str;
}


static void ShowProduction(currentProduction)
  String currentProduction;
{
    Cardinal num_params = 1;
    char production[500], *eol;
    String params[1];
	
    strncpy( production, currentProduction, 500 );
    if ((eol = index(production, '\n')) != 0) *eol = '\0';
    else production[499] = '\0'; /* just in case */

    params[0] = production;
    XtWarningMsg("translationParseError", "showLine", "XtToolkitError",
		 "... found while parsing '%s'", params, &num_params);
}

/***********************************************************************
 * ParseTranslationTableProduction
 * Parses one line of event bindings.
 ***********************************************************************/

static String ParseTranslationTableProduction(stateTable, str,acc)
  XtTranslations stateTable;
  register String str;
  Boolean acc;
{
    EventSeqPtr	eventSeq = NULL;
    ActionPtr	*actionsP;
    Boolean error = FALSE;
    String	production = str;

    str = ParseEventSeq(str, &eventSeq, &actionsP,&error);
    if (error == TRUE) {
	ShowProduction(production);
        FreeEventSeq(eventSeq);
        return (str);
    }
    str = ScanWhitespace(str);
    str = ParseActionSeq(stateTable, str, actionsP,acc,&error);
    if (error) {
	ShowProduction(production);
        FreeEventSeq(eventSeq);
        return (str);
    }

    _XtAddEventSeqToStateTable(eventSeq, stateTable);

    FreeEventSeq(eventSeq);
    return (str);
}

/*ARGSUSED*/
static Boolean CvtStringToAccelerators(dpy, args, num_args, from, to, closure)
    Display*	dpy;
    XrmValuePtr args;
    Cardinal    *num_args;
    XrmValuePtr from,to;
    XtPointer	*closure;
{
    String str;

    if (*num_args != 0)
        XtAppWarningMsg(XtDisplayToApplicationContext(dpy),
	  "invalidParameters","compileAccelerators","XtToolkitError",
          "String to TranslationTable needs no extra arguments",
	  (String *)NULL, (Cardinal *)NULL);
     str = (String)(from->addr);
     if (str == NULL)
         return False;

    if (to->addr != NULL) {
	if (to->size < sizeof(XtAccelerators)) {
	    to->size = sizeof(XtAccelerators);
	    return False;
	}
	*(XtAccelerators*)to->addr = XtParseAcceleratorTable(str);
    }
    else {
	static XtAccelerators staticStateTable;
	staticStateTable = XtParseAcceleratorTable(str);
	to->addr= (XtPointer)&staticStateTable;
	to->size = sizeof(XtAccelerators);
    }
    return True;
}


static String CheckForPoundSign(stateTable,str)
    XtTranslations stateTable;
    String str;
{
    String start;
    char operation[20];
    if (*str == '#') {
       str++;
       start = str;
       str = ScanIdent(str);
       (void) strncpy(operation, start, MIN(20, str-start));
       operation[str-start] = '\0';
       if (!strcmp(operation,"replace"))
            stateTable->operation = XtTableReplace;
       else if (!strcmp(operation,"augment"))
            stateTable->operation = XtTableAugment;
       else if (!strcmp(operation,"override"))
            stateTable->operation =  XtTableOverride;
       else  stateTable->operation = XtTableReplace;
       str = ScanWhitespace(str);
       if (*str == '\n') {
   	    str++;
	    str = ScanWhitespace(str);
       }
    }
    else stateTable->operation = XtTableReplace;
   return str;
}

/*ARGSUSED*/
static Boolean
CvtStringToTranslations(dpy, args, num_args, from, to, closure_ret)
    Display	*dpy;
    XrmValuePtr args;
    Cardinal    *num_args;
    XrmValuePtr from,to;
    XtPointer	*closure_ret;
{
    String str;

    if (*num_args != 0)
	XtAppWarningMsg(XtDisplayToApplicationContext(dpy),
	  "invalidParameters","compileTranslations","XtToolkitError",
          "String to TranslationTable conversion needs no extra arguments",
	  (String *)NULL, (Cardinal *)NULL);
     str = (String)(from->addr);
     if (str == NULL)
         return False;

    if (to->addr != NULL) {
	if (to->size < sizeof(XtTranslations)) {
	    to->size = sizeof(XtTranslations);
	    return False;
	}
	*(XtTranslations*)to->addr = XtParseTranslationTable(str);
    }
    else {
	static XtTranslations staticStateTable;
	staticStateTable = XtParseTranslationTable(str);
	to->addr= (XtPointer)&staticStateTable;
	to->size = sizeof(XtTranslations);
    }
    return True;
}

/*** public procedures ***/

/*
 * Parses a user's or applications translation table
 */
XtTranslations XtParseTranslationTable(source)
    String source;
{
    XtTranslations stateTable;

    if (source == NULL)
         return (XtTranslations)NULL;

    _XtInitializeStateTable(&stateTable);

    source = CheckForPoundSign(stateTable,source);
    while (source != NULL && *source != '\0') {
       source =  ParseTranslationTableProduction(stateTable,source,FALSE);
    }
    return stateTable;
}

XtAccelerators XtParseAcceleratorTable (source)
    String   source;
{
    XtTranslations stateTable;

    if (source == NULL)
	return NULL;

    _XtInitializeStateTable(&stateTable);

    source = CheckForPoundSign(stateTable,source);
    while (source != NULL && *source != '\0') {
       source =  ParseTranslationTableProduction(stateTable,source,TRUE);
    }
    return stateTable;
}


void _XtTranslateInitialize()
{
    if (initialized) {
	XtWarningMsg("translationError","xtTranslateInitialize",
                  "XtToolkitError","Intializing Translation manager twice.",
                    (String *)NULL, (Cardinal *)NULL);
	return;
    }

    initialized = TRUE;

    Compile_XtEventTable( events );
    Compile_XtModifierTable( modifiers );
    CompileNameValueTable( buttonNames );
    CompileNameValueTable( notifyModes );
    CompileNameValueTable( motionDetails );
    CompileNameValueTable( notifyDetail );
    CompileNameValueTable( visibilityNotify );
    CompileNameValueTable( circulation );
    CompileNameValueTable( propertyChanged );
    CompileNameValueTable( mappingNotify );
}

_XtAddTMConverters(table)
    ConverterTable table;
{
    XrmQuark q;
    extern void _XtFreeTranslations();
    extern Boolean _XtCvtMergeTranslations();
     _XtTableAddConverter(table,
	     q = XrmStringToRepresentation(XtRString), 
	     XrmStringToRepresentation(XtRTranslationTable), 
 	     CvtStringToTranslations, (XtConvertArgList) NULL,
	     (Cardinal)0, True, CACHED, _XtFreeTranslations);
     _XtTableAddConverter(table, q,
	     XrmStringToRepresentation(XtRAcceleratorTable),
 	     CvtStringToAccelerators, (XtConvertArgList) NULL,
	     (Cardinal)0, True, XtCacheNone,(XtDestructor)_XtFreeTranslations);
     _XtTableAddConverter(table,
	     XrmStringToRepresentation( "_XtTranslationTablePair" ),
	     XrmStringToRepresentation(XtRTranslationTable), 
 	     _XtCvtMergeTranslations, (XtConvertArgList) NULL,
	     (Cardinal)0, True, CACHED, _XtFreeTranslations);
}
