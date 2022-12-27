/* $XConsortium: TranslateI.h,v 1.16 89/10/06 19:16:55 swick Exp $ */
/* $oHeader: TranslateI.h,v 1.2 88/08/18 15:56:37 asente Exp $ */
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

/* 
 * TranslateI.h - Header file private to translation management
 * 
 * Author:	Charles Haynes
 * 		Digital Equipment Corporation
 * 		Western Research Laboratory
 * Date:	Sat Aug 29 1987
 */

/*#define REFCNT_TRANSLATIONS*/
#define CACHE_TRANSLATIONS

typedef Boolean (*MatchProc)();
  /* Event parsed;
     XEvent incoming;
  */

typedef struct _ModToKeysymTable {
    Modifiers mask;
    int count;
    int index;
} ModToKeysymTable;

typedef struct _LateBindings {
    Boolean knot;
    Boolean pair;
    KeySym keysym;
} LateBindings, *LateBindingsPtr;

typedef short ModifierMask;
typedef struct _EventObjRec *EventObjPtr;
typedef struct _EventRec {
    unsigned long modifiers;
    unsigned long modifierMask;
    LateBindingsPtr lateModifiers;
    unsigned long eventType;
    unsigned long eventCode;
    unsigned long eventCodeMask;
    MatchProc matchEvent;
    Boolean standard;
} Event;

typedef enum _TMkind {override,augment} TMkind;

typedef struct _TMConvertRec {
   XtTranslations old; /* table to merge into */
   XtTranslations new; /* table to merge from */
   TMkind  operation; /* merge or augment     */
} TMConvertRec;

typedef struct _EventObjRec {
    Event event;	/* X event description */
    StatePtr state;	/* pointer to linked lists of state info */
} EventObjRec;

typedef struct _ActionsRec *ActionPtr;
typedef struct _ActionsRec {
    int index;			/* index into quarkTable to find proc */
    String *params;		/* pointer to array of params */
    Cardinal num_params;	/* number of params */
    ActionPtr next;		/* next action to perform */
} ActionRec;

typedef struct _StateRec {
    int index;		/* index of event into EventObj table */
    ActionPtr actions;	/* rhs list of actions to perform */
    StatePtr nextLevel;	/* the next level points to the next event
			   in one event sequence */
    StatePtr next;	/* points to other event state at same level */
    StatePtr forw;	/* points to next state in list of all states */
    Boolean cycle;	/* true iff nextLevel is a loop */
}  StateRec;
typedef enum {XtTableReplace,XtTableAugment,XtTableOverride} _XtTranslateOp;

typedef struct _TranslationData {
    _XtTranslateOp	operation; /*replace,augment,override*/
    unsigned int	numEvents;
    unsigned int	eventTblSize;
    EventObjPtr		eventObjTbl;
    unsigned long	lastEventTime;
    unsigned int	numQuarks;   /* # of entries in quarkTable */
    unsigned int	quarkTblSize; /*total size of quarkTable */
    XrmQuark*		quarkTable;  /* table of quarkified rhs*/
    unsigned int	accNumQuarks;
    unsigned int	accQuarkTblSize;
    XrmQuark*		accQuarkTable;
    struct _XtBoundAccActionRec* accProcTbl;
    StatePtr		head;	/* head of list of all states */
    
} TranslationData;

#define _XtEventTimerEventType ((unsigned long)-1L)
#define KeysymModMask		(1<<27) /* private to TM */
#define AnyButtonMask		(1<<28)	/* private to TM */

typedef struct _EventSeqRec *EventSeqPtr;
typedef struct _EventSeqRec {
    Event event;	/* X event description */
    StatePtr state;	/* private to state table builder */
    EventSeqPtr next;	/* next event on line */
    ActionPtr actions;	/* r.h.s.   list of actions to perform */
} EventSeqRec;

typedef EventSeqRec EventRec;
typedef EventSeqPtr EventPtr;
typedef struct _TMEventRec {
    XEvent *xev;
    Event event;
}TMEventRec,*TMEventPtr;

typedef struct _XtBoundAccActionRec {
    Widget widget;    /*widgetID to pass to action Proc*/
    XtActionProc proc; /*action procedure */
} XtBoundAccActionRec;


extern void _XtAddEventSeqToStateTable();
extern void _XtInitializeStateTable(); /* stateTable */
    /* _XtTranslations *stateTable; */

typedef XrmQuark XtAction;

typedef unsigned int	Value;
typedef struct {
    char	*name;
    XrmQuark	signature;
    Value	value;
} NameValueRec, *NameValueTable;

typedef struct _ActionHookRec {
    struct _ActionHookRec* next; /* must remain first */
    XtAppContext app;
    XtActionHookProc proc;
    XtPointer closure;
} ActionHookRec, *ActionHook;

typedef struct _TMContext {
    XEvent *event;
    unsigned long serial;
    KeySym keysym;
    Modifiers modifiers;
} TMContextRec, *TMContext;

extern Boolean _XtMatchUsingStandardMods();
extern Boolean _XtMatchUsingDontCareMods();
extern Boolean _XtRegularMatch();
extern Boolean _XtMatchAtom();
extern void _XtConvertCase();
extern void _XtBuildKeysymTables();

extern void _XtPopupInitialize();

extern void _XtInstallTranslations(); /* widget, stateTable */
    /* Widget widget; */
    /* XtTranslations stateTable; */

extern void _XtBindActions(); /* widget, stateTable */
    /* Widget widget; */
    /* XtTranslations stateTable; */

extern void _XtTranslateInitialize();

extern XtTranslations _XtParseTranslationTable(); /* source */
    /* String source; */

extern void _XtRegisterGrabs(); /* widget */
    /* Widget widget; */

extern void _XtPopup(); /* widget, grab_kind, spring_loaded */
    /* Widget      widget; */
    /* XtGrabKind  grab_kind; */
    /* Boolean     spring_loaded; */
