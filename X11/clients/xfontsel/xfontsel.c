#ifndef lint
static char Xrcsid[] = "$XConsortium: xfontsel.c,v 1.16 89/12/12 14:10:48 rws Exp $";
#endif

/*

Copyright 1985, 1986, 1987, 1988, 1989 by the
Massachusetts Institute of Technology

Permission to use, copy, modify, and distribute this
software and its documentation for any purpose and without
fee is hereby granted, provided that the above copyright
notice appear in all copies and that both that copyright
notice and this permission notice appear in supporting
documentation, and that the name of M.I.T. not be used in
advertising or publicity pertaining to distribution of the
software without specific, written prior permission.
M.I.T. makes no representations about the suitability of
this software for any purpose.  It is provided "as is"
without express or implied warranty.

Author:	Ralph R. Swick, DEC/MIT Project Athena
	one weekend in November, 1989
*/

#include <stdio.h>
#include <X11/Intrinsic.h>
#include <X11/StringDefs.h>
#include <X11/Xatom.h>
#include <X11/Xaw/AsciiText.h>
#include <X11/Xaw/Box.h>
#include <X11/Xaw/Cardinals.h>
#include <X11/Xaw/Command.h>
#include <X11/Xaw/Form.h>
#include <X11/Xaw/MenuButton.h>
#include <X11/Xaw/Paned.h>
#include <X11/Xaw/SimpleMenu.h>
#include <X11/Xaw/SmeBSB.h>
#include <X11/Xaw/Toggle.h>
#include <X11/Xaw/Viewport.h>
#include <X11/Xmu/Atoms.h>

#define MIN_APP_DEFAULTS_VERSION 1
#define FIELD_COUNT 14
#define DELIM '-'

/* number of font names to parse in each background iteration */
#ifndef PARSE_QUANTUM
#define PARSE_QUANTUM 25
#endif

#define NZ NULL,ZERO
#define BACKGROUND 10

void GetFontNames();
Boolean Matches();
Boolean DoWorkPiece();
void Quit();
void OwnSelection();
void SelectField();
void ParseFontNames();
void MakeFieldMenu();
void SelectValue();
void AnyValue();
void EnableOtherValues();
void EnableMenu();
void SetCurrentFont();
Boolean IsXLFDFontName();

typedef void (*XtProc)();

static struct _appRes {
    int app_defaults_version;
    Cursor cursor;
    String pattern;
    Boolean print_on_quit;
} AppRes;

char defaultPattern[] = "-*-*-*-*-*-*-*-*-*-*-*-*-*-*";

static XtResource resources[] = {
    { "cursor", "Cursor", XtRCursor, sizeof(Cursor),
		XtOffsetOf( struct _appRes, cursor ),
		XtRImmediate, NULL },
    { "pattern", "Pattern", XtRString, sizeof(String),
		XtOffsetOf( struct _appRes, pattern ),
		XtRString, (XtPointer)defaultPattern },
    { "printOnQuit", "PrintOnQuit", XtRBoolean, sizeof(Boolean),
	  	XtOffsetOf( struct _appRes, print_on_quit ),
      		XtRImmediate, (XtPointer)False },
    { "appDefaultsVersion", "AppDefaultsVersion", XtRInt, sizeof(int),
		XtOffsetOf( struct _appRes, app_defaults_version ),
		XtRImmediate, (XtPointer)0 },
};

static XrmOptionDescRec options[] = {
{"-pattern",	"pattern",	XrmoptionSepArg,	NULL},
{"-print",	"printOnQuit",	XrmoptionNoArg,		"True"},
{"-sample",	"*sampleText.label", XrmoptionSepArg,	NULL},
};

Syntax(call)
    char *call;
{
    fprintf( stderr,
	     "Usage: %s [-toolkitOption] [-pattern <fontspec>] [-print] [-sample <text>]\n",
	     call );
}


typedef struct FieldValue FieldValue;
struct FieldValue {
    int field;
    String string;
    Widget menu_item;
    int count;			/* of fonts */
    int allocated;
    int *font;
    Boolean enable;
};


typedef struct FieldValueList FieldValueList;
struct FieldValueList {
    int count;			/* of values */
    int allocated;
    Boolean show_unselectable;
    FieldValue value[1];	/* really [allocated] */
};


typedef struct FontValues FontValues;
struct FontValues {
    int value_index[FIELD_COUNT];
};


typedef struct FieldMenuRec FieldMenuRec;
struct FieldMenuRec {
    int field;
    Widget button;
};


typedef struct Choice Choice;
struct Choice {
    Choice *prev;
    FieldValue *value;
};


static XtResource menuResources[] = {
    { "showUnselectable", "ShowUnselectable", XtRBoolean, sizeof(Boolean),
		XtOffsetOf( FieldValueList, show_unselectable ),
		XtRImmediate, (XtPointer)True },
};


typedef enum {ValidateCurrentField, SkipCurrentField} ValidateAction;

XtAppContext appCtx;
int numFonts;
int numBadFonts;
FontValues *fonts;
FieldValueList *fieldValues[FIELD_COUNT];
FontValues currentFont;
int matchingFontCount;
static Boolean anyDisabled = False;
Widget ownButton;
Widget fieldBox;
Widget countLabel;
Widget currentFontName;
String currentFontNameString;
int currentFontNameSize;
Widget sampleText;
static XFontStruct *sampleFont = NULL;
Boolean *fontInSet;
static Choice *choiceList = NULL;
int enabledMenuIndex;
static Boolean patternFieldSpecified[FIELD_COUNT]; /* = 0 */


void main(argc, argv)
    unsigned int argc;
    char **argv;
{
    Widget topLevel, pane;

    topLevel = XtInitialize( NULL, "XFontSel", options, XtNumber(options),
			     &argc, argv );

    if (argc != 1) Syntax(argv[0]);

    appCtx = XtWidgetToApplicationContext(topLevel);
    XtGetApplicationResources( topLevel, &AppRes,
			       resources, XtNumber(resources), NZ );
    if (AppRes.app_defaults_version < MIN_APP_DEFAULTS_VERSION) {
	XrmDatabase rdb = XtDatabase(XtDisplay(topLevel));
	XtWarning( "app-defaults file not properly installed." );
	XrmPutLineResource( &rdb,
"*sampleText*Label:XFontSel app-defaults file not properly installed;\\n\
see 'xfontsel' manual page."
			  );
    }

    ScheduleWork(GetFontNames, (XtPointer)XtDisplay(topLevel), 0);

    pane = XtCreateManagedWidget("pane",panedWidgetClass,topLevel,NZ);
    {
	Widget commandBox, /* fieldBox, currentFontName,*/ viewPort;

	commandBox = XtCreateManagedWidget("commandBox",formWidgetClass,pane,NZ);
	{
	    Widget quitButton /*, ownButton , countLabel*/;

	    quitButton =
		XtCreateManagedWidget("quitButton",commandWidgetClass,commandBox,NZ);

	    ownButton =
		XtCreateManagedWidget("ownButton",toggleWidgetClass,commandBox,NZ);

	    countLabel =
		XtCreateManagedWidget("countLabel",labelWidgetClass,commandBox,NZ);

	    XtAddCallback(quitButton, XtNcallback, Quit, NULL);
	    XtAddCallback(ownButton,XtNcallback,OwnSelection,(XtPointer)True);
	}

	fieldBox = XtCreateManagedWidget("fieldBox", boxWidgetClass, pane, NZ);
	{
	    Widget /*dash,*/ field /*[FIELD_COUNT]*/;
	    int f;

	    for (f = 0; f < FIELD_COUNT; f++) {
		char name[10];
		FieldMenuRec *makeRec = XtNew(FieldMenuRec);
		sprintf( name, "field%d", f );
		XtCreateManagedWidget("dash",labelWidgetClass,fieldBox,NZ);
		field = XtCreateManagedWidget(name,menuButtonWidgetClass,fieldBox,NZ);
		XtAddCallback(field, XtNcallback, SelectField, (XtPointer)f);
		makeRec->field = f;
		makeRec->button = field;
		ScheduleWork(MakeFieldMenu, (XtPointer)makeRec, 2);
		ScheduleWork(XtFree, (XtPointer)makeRec, 2);
	    }
	}

	/* currentFontName = */
	{
	    Arg args[1];
	    currentFontNameSize = strlen(AppRes.pattern);
	    if (currentFontNameSize < 128) currentFontNameSize = 128;
	    currentFontNameString = (String)XtMalloc(currentFontNameSize);
	    strcpy(currentFontNameString, AppRes.pattern);
	    XtSetArg(args[0], XtNlabel, currentFontNameString);
	    currentFontName =
		XtCreateManagedWidget("fontName",labelWidgetClass,pane,args,ONE);
	}

	viewPort =
	    XtCreateManagedWidget("viewPort",viewportWidgetClass,pane,NZ);
	{
#ifdef USE_TEXT_WIDGET
	    Widget text =
		XtCreateManagedWidget("sampleText",asciiTextWidgetClass,viewPort,NZ);
	    Arg args[1];
	    XtSetArg( args[0], XtNtextSink, &sampleText );
	    XtGetValues( text, args, ONE );
#else
	    sampleText =
		XtCreateManagedWidget("sampleText",labelWidgetClass,viewPort,NZ);
#endif
	}
    }
    
    XtRealizeWidget(topLevel);
    XDefineCursor( XtDisplay(topLevel), XtWindow(topLevel), AppRes.cursor );
    {
	int f;
	for (f = 0; f < FIELD_COUNT; f++) currentFont.value_index[f] = -1;
    }
    XtAppMainLoop(appCtx);
}


typedef struct WorkPiece WorkPieceRec, *WorkPiece;
struct WorkPiece {
    WorkPiece next;
    int priority;
    XtProc proc;
    XtPointer closure;
};
static WorkPiece workQueue = NULL;


/*
 * ScheduleWork( XtProc proc, XtPointer closure, int priority )
 *
 * Adds a WorkPiece to the workQueue in FIFO order by priority.
 * Lower numbered priority work is completed before higher numbered
 * priorities.
 *
 * If the workQueue was previously empty, then makes sure that
 * Xt knows we have (background) work to do.
 */


ScheduleWork( proc, closure, priority )
    XtProc proc;
    XtPointer closure;
    int priority;
{
    WorkPiece piece = XtNew(WorkPieceRec);

    piece->priority = priority;
    piece->proc = proc;
    piece->closure = closure;
    if (workQueue == NULL) {
	piece->next = NULL;
	workQueue = piece;
	XtAppAddWorkProc(appCtx, DoWorkPiece, NULL);
    } else {
	if (workQueue->priority > priority) {
	    piece->next = workQueue;
	    workQueue = piece;
	}
	else {
	    WorkPiece n;
	    for (n = workQueue; n->next && n->next->priority <= priority;)
		n = n->next;
	    piece->next = n->next;
	    n->next = piece;
	}
    }
}

/* ARGSUSED */
Boolean DoWorkPiece(closure)
    XtPointer closure;		/* unused */
{
    WorkPiece piece = workQueue;

    if (piece) {
	(*piece->proc)(piece->closure);
	workQueue = piece->next;
	XtFree((XtPointer)piece);
	if (workQueue != NULL)
	    return False;
    }
    return True;
}


/*
 * FinishWork()
 *
 * Drains foreground tasks from the workQueue.
 * Foreground == (priority < BACKGROUND)
 */

void FinishWork()
{
    while (workQueue && workQueue->priority < BACKGROUND)
	DoWorkPiece(NULL);
}


typedef struct ParseRec ParseRec;
struct ParseRec {
    char **fontNames;
    int num_fonts;
    int start, end;
    FontValues *fonts;
    FieldValueList **fieldValues;
};


void GetFontNames( closure )
    XtPointer closure;
{
    Display *dpy = (Display*)closure;
    ParseRec *parseRec = XtNew(ParseRec);
    int f, field, count;
    String *fontNames;
    Boolean *b;
    int work_priority = 0;

    fontNames = parseRec->fontNames =
	XListFonts(dpy, AppRes.pattern, 32767, &numFonts);

    fonts = (FontValues*)XtMalloc( numFonts*sizeof(FontValues) );
    fontInSet = (Boolean*)XtMalloc( numFonts*sizeof(Boolean) );
    for (f = numFonts, b = fontInSet; f; f--, b++) *b = True;
    for (field = 0; field < FIELD_COUNT; field++) {
	fieldValues[field] = (FieldValueList*)XtMalloc(sizeof(FieldValueList));
	fieldValues[field]->allocated = 1;
	fieldValues[field]->count = 0;
    }
    if (numFonts == 0) {
	SetNoFonts();
	return;
    }
    numBadFonts = 0;
    parseRec->fonts = fonts;
    parseRec->num_fonts = count = matchingFontCount = numFonts;
    parseRec->fieldValues = fieldValues;
    parseRec->start = 0;
    /* this is bogus; the task should be responsible for quantizing...*/
    while (count > PARSE_QUANTUM) {
	ParseRec *prevRec = parseRec;
	parseRec->end = parseRec->start + PARSE_QUANTUM;
	ScheduleWork(ParseFontNames, (XtPointer)parseRec, work_priority);
	ScheduleWork(XtFree, (XtPointer)parseRec, work_priority);
	parseRec = XtNew(ParseRec);
	*parseRec = *prevRec;
	parseRec->start += PARSE_QUANTUM;
	parseRec->fonts += PARSE_QUANTUM;
	parseRec->fontNames += PARSE_QUANTUM;
	count -= PARSE_QUANTUM;
	work_priority = 1;
    }
    parseRec->end = numFonts;
    ScheduleWork(ParseFontNames,(XtPointer)parseRec,work_priority);
    ScheduleWork((XtProc)XFreeFontNames,(XtPointer)fontNames,work_priority);
    ScheduleWork(XtFree, (XtPointer)parseRec, work_priority);
    SetParsingFontCount(matchingFontCount);
    if (AppRes.pattern != defaultPattern) {
	int maxField, f;
	for (f = 0; f < numFonts && !IsXLFDFontName(fontNames[f]); f++);
	if (f != numFonts) {
	    if (Matches(AppRes.pattern, fontNames[f],
			 patternFieldSpecified, &maxField)) {
		for (f = 0; f <= maxField; f++) {
		    if (patternFieldSpecified[f])
			currentFont.value_index[f] = 0;
		}
	    }
	    else
		XtAppWarning( "internal error; pattern didn't match first font" );
	}
	else {
	    SetNoFonts();
	    return;
	}
    }
    ScheduleWork(SetCurrentFont, NULL, 1);
}


void ParseFontNames( closure )
    XtPointer closure;
{
    ParseRec *parseRec = (ParseRec*)closure;
    char **fontNames = parseRec->fontNames;
    int num_fonts = parseRec->end;
    FieldValueList **fieldValues = parseRec->fieldValues;
    FontValues *fontValues = parseRec->fonts - numBadFonts;
    int i, font;

    for (font = parseRec->start; font < num_fonts; font++) {
        char *p;
        int f, len;
        FieldValue *v;

	if (!IsXLFDFontName(*fontNames)) {
	    numFonts--;
	    numBadFonts++;
	    continue;
	}

	for (f = 0, p = *fontNames++; f < FIELD_COUNT; f++) {
	    char *fieldP;

	    if (*p) fieldP = ++p;
	    if (*p == DELIM || *p == '\0') {
		fieldP = "";
		len = 0;
	    } else {
		while (*p && *++p != DELIM);
		len = p - fieldP;
	    }
	    for (i=fieldValues[f]->count,v=fieldValues[f]->value; i;i--,v++) {
		if (len == 0) {
		    if (v->string == NULL) break;
		}
		else
		    if (v->string &&
			strncmp( v->string, fieldP, len ) == 0 &&
			(v->string)[len] == '\0')
			break;
	    }
	    if (i == 0) {
		int count = fieldValues[f]->count++;
		if (count == fieldValues[f]->allocated) {
		    int allocated = (fieldValues[f]->allocated += 10);
		    fieldValues[f] = (FieldValueList*)
			XtRealloc( fieldValues[f],
				   sizeof(FieldValueList) +
					(allocated-1) * sizeof(FieldValue) );
		}
		v = &fieldValues[f]->value[count];
		v->field = f;
		if (len == 0)
		    v->string = NULL;
		else {
		    v->string = (String)XtMalloc( len+1 );
		    strncpy( v->string, fieldP, len );
		    v->string[len] = '\0';
		}
		v->font = (int*)XtMalloc( 10*sizeof(int) );
		v->allocated = 10;
		v->count = 0;
		v->enable = True;
		i = 1;
	    }
	    fontValues->value_index[f] = fieldValues[f]->count - i;
	    if ((i = v->count++) == v->allocated) {
		int allocated = (v->allocated += 10);
		v->font = (int*)XtRealloc( v->font, allocated * sizeof(int) );
	    }
	    v->font[i] = font - numBadFonts;
	}
	fontValues++;
    }
    SetParsingFontCount(numFonts - num_fonts);
}


Boolean IsXLFDFontName(fontName)
    String fontName;
{
    int f;
    for (f = 0; *fontName;) if (*fontName++ == DELIM) f++;
    return (f == FIELD_COUNT);
}


void MakeFieldMenu(closure)
    XtPointer closure;
{
    FieldMenuRec *makeRec = (FieldMenuRec*)closure;
    Widget menu;
    FieldValueList *values = fieldValues[makeRec->field];
    FieldValue *val = values->value;
    int i;
    Arg args[1];
    register Widget item;

    if (numFonts)
	menu =
	  XtCreatePopupShell("menu",simpleMenuWidgetClass,makeRec->button,NZ);
    else {
	SetNoFonts();
	return;
    }
    XtGetSubresources(menu, values, "options", "Options",
		      menuResources, XtNumber(menuResources), NZ);
    XtAddCallback(menu, XtNpopupCallback, EnableOtherValues,
		  (XtPointer)makeRec->field );

    if (!patternFieldSpecified[val->field]) {
	XtSetArg( args[0], XtNlabel, "*" );
	item = XtCreateManagedWidget("any",smeBSBObjectClass,menu,args,ONE);
	XtAddCallback(item, XtNcallback, AnyValue, (XtPointer)val->field);
    }

    for (i = values->count; i; i--, val++) {
	XtSetArg( args[0], XtNlabel, val->string ? val->string : "(nil)" );
	item =
	    XtCreateManagedWidget(val->string ? val->string : "nil",
				  smeBSBObjectClass, menu, args, ONE);
	XtAddCallback(item, XtNcallback, SelectValue, (XtPointer)val);
	val->menu_item = item;
    }
}


SetNoFonts()
{
    matchingFontCount = 0;
    SetCurrentFontCount();
    XtSetSensitive(fieldBox, False);
    XtSetSensitive(ownButton, False);
    if (AppRes.app_defaults_version >= MIN_APP_DEFAULTS_VERSION) {
#ifdef USE_TEXT_WIDGET
	XtUnmapWidget(XtParent(sampleText));
#else
	XtUnmapWidget(sampleText);
#endif
    }
}


Boolean Matches(pattern, fontName, fields, maxField)
    register String pattern, fontName;
    Boolean fields[/*FIELD_COUNT*/];
    int *maxField;
{
    register int field = (*fontName == DELIM) ? -1 : 0;
    register Boolean marked_this_field = False;

    while (*pattern) {
	if (*pattern == *fontName || *pattern == '?') {
	    pattern++;
	    if (*fontName++ == DELIM) {
		field++;
		marked_this_field = False;
	    }
	    else if (!marked_this_field)
		fields[field] = marked_this_field = True; 
	    continue;
	}
	if (*pattern == '*') {
	    if (*++pattern == '\0') {
		*maxField = field;
		return True;
	    }
	    while (*fontName) {
		Boolean field_bits[FIELD_COUNT];
		int max_field;
		if (*fontName == DELIM) field++;
		bzero( field_bits, sizeof(field_bits) );
		if (Matches(pattern, fontName++, field_bits, &max_field)) {
		    int f;
		    *maxField = field + max_field;
		    for (f = 0; f <= max_field; field++, f++)
			fields[field] = field_bits[f];
		    return True;
		}
	    }
	    return False;
	}
	else /* (*pattern != '*') */
	    return False;
    }
    if (*fontName)
	return False;

    *maxField = field;
    return True;
}


/* ARGSUSED */
void SelectValue(w, closure, callData)
    Widget w;
    XtPointer closure, callData;
{
    FieldValue *val = (FieldValue*)closure;
#ifdef LOG_CHOICES
    Choice *choice = XtNew(Choice);
#else
    static Choice pChoice;
    Choice *choice = &pChoice;
#endif

#ifdef notdef
    Widget button = XtParent(XtParent(w));
    Arg args[1];

    XtSetArg(args[0], XtNlabel, val->string);
    XtSetValues( button, args, ONE );
#endif

    currentFont.value_index[val->field] = val - fieldValues[val->field]->value;

    choice->prev = choiceList;
    choice->value = val;
    choiceList = choice;
	
    SetCurrentFont(NULL);
    EnableRemainingItems(SkipCurrentField);
}


/* ARGSUSED */
void AnyValue(w, closure, callData)
    Widget w;
    XtPointer closure, callData;
{
    int field = (int)closure;
    currentFont.value_index[field] = -1;
    SetCurrentFont(NULL);
    EnableAllItems(field);
    EnableRemainingItems(ValidateCurrentField);
}


SetCurrentFontCount()
{
    char label[80];
    Arg args[1];
    if (matchingFontCount == 1)
	strcpy( label, "1 font matches" );
    else if (matchingFontCount)
	sprintf( label, "%d fonts match", matchingFontCount );
    else
	strcpy( label, "no fonts match" );
    XtSetArg( args[0], XtNlabel, label );
    XtSetValues( countLabel, args, ONE );
}


SetParsingFontCount(count)
{
    char label[80];
    Arg args[1];
    if (count == 1)
	strcpy( label, "1 name to parse" );
    else
	sprintf( label, "%d names to parse", count );
    XtSetArg( args[0], XtNlabel, label );
    XtSetValues( countLabel, args, ONE );
    FlushXqueue(XtDisplay(countLabel));
}


/* ARGSUSED */
void SetCurrentFont(closure)
    XtPointer closure;		/* unused */
{
    int f;
    Boolean *b;

    if (numFonts == 0) {
	SetNoFonts();
	return;
    }
    for (f = numFonts, b = fontInSet; f; f--, b++) *b = True;

    {
	int bytesLeft = currentFontNameSize;
	int pos = 0;

	for (f = 0; f < FIELD_COUNT; f++) {
	    int len, i;
	    String str;

	    currentFontNameString[pos++] = DELIM;
	    if ((i = currentFont.value_index[f]) != -1) {
		FieldValue *val = &fieldValues[f]->value[i];
		if (str = val->string)
		    len = strlen(str);
		else {
		    str = "";
		    len = 0;
		}
		MarkInvalidFonts(fontInSet, val);
	    } else {
		str = "*";
		len = 1;
	    }
	    if (len+1 > --bytesLeft) {
		currentFontNameString = (String)
		    XtRealloc(currentFontNameString, currentFontNameSize+=128);
		bytesLeft += 128;
	    }
	    strcpy( &currentFontNameString[pos], str );
	    pos += len;
	    bytesLeft -= len;
	}
    }
    {
	Arg args[1];
	XtSetArg( args[0], XtNlabel, currentFontNameString );
	XtSetValues( currentFontName, args, ONE );
    }
    matchingFontCount = 0;
    for (f = numFonts, b = fontInSet; f; f--, b++) {
	if (*b) matchingFontCount++;
    }

    SetCurrentFontCount();

    {
#ifdef USE_TEXT_WIDGET
	Widget mapWidget = XtParent(sampleText);
#else
	Widget mapWidget = sampleText;
#endif
	Display *dpy = XtDisplay(mapWidget);
	XFontStruct *font = XLoadQueryFont(dpy, currentFontNameString);
	if (font == NULL)
	    XtUnmapWidget(mapWidget);
	else {
	    Arg args[1];
	    XtSetArg( args[0], XtNfont, font );
	    XtSetValues( sampleText, args, ONE );
	    XtMapWidget(mapWidget);
	    if (sampleFont) XFreeFont( dpy, sampleFont );
	    sampleFont = font;
	    OwnSelection( sampleText, (XtPointer)False, (XtPointer)True );
	}
	FlushXqueue(dpy);
    }
}


MarkInvalidFonts( set, val )
    Boolean *set;
    FieldValue *val;
{
    int fi = 0, vi;
    int *fp = val->font;
    for (vi = val->count; vi; vi--, fp++) {
	while (fi < *fp) {
	    if (set[fi]) set[fi] = False;
	    fi++;
	}
	fi++;
    }
    while (fi < numFonts) {
	if (set[fi]) set[fi] = False;
	fi++;
    }
}


EnableRemainingItems(current_field_action)
    ValidateAction current_field_action;
{
    if (matchingFontCount == 0 || matchingFontCount == numFonts) {
	if (anyDisabled) {
	    int field;
	    for (field = 0; field < FIELD_COUNT; field++) {
		EnableAllItems(field);
	    }
	    anyDisabled = False;
	}
    }
    else {
	int field;
	for (field = 0; field < FIELD_COUNT; field++) {
	    FieldValue *value = fieldValues[field]->value;
	    int count;
	    if (current_field_action == SkipCurrentField &&
		field == choiceList->value->field)
		continue;
	    for (count = fieldValues[field]->count; count; count--, value++) {
		int *fp = value->font;
		int fontCount;
		for (fontCount = value->count; fontCount; fontCount--, fp++) {
		    if (fontInSet[*fp]) {
			value->enable = True;
			goto NextValue;
		    }
		}
		value->enable = False;
	      NextValue:;
	    }
	}
	anyDisabled = True;
    }
    enabledMenuIndex = -1;
    {
	int f;
	for (f = 0; f < FIELD_COUNT; f++)
	    ScheduleWork(EnableMenu, (XtPointer)f, BACKGROUND);
    }
}


EnableAllItems(field)
{
    FieldValue *value = fieldValues[field]->value;
    int count;
    for (count = fieldValues[field]->count; count; count--, value++) {
	value->enable = True;
    }
}


/* ARGSUSED */
void SelectField(w, closure, callData)
    Widget w;
    XtPointer closure, callData;
{
    int field = (int)closure;
    FieldValue *values = fieldValues[field]->value;
    int count = fieldValues[field]->count;
    printf( "field %d:\n", field );
    while (count--) {
	printf( " %s: %d fonts\n", values->string, values->count );
	values++;
    }
    printf( "\n" );
}


/* ARGSUSED */
void EnableOtherValues(w, closure, callData)
    Widget w;
    XtPointer closure, callData;
{
    int field = (int)closure;
    Boolean *font_in_set = (Boolean*)XtMalloc(numFonts*sizeof(Boolean));
    Boolean *b;
    int f, count;

    FinishWork();
    for (f = numFonts, b = font_in_set; f; f--, b++) *b = True;
    for (f = 0; f < FIELD_COUNT; f++) {
	int i;
	if (f != field && (i = currentFont.value_index[f]) != -1) {
	    MarkInvalidFonts( font_in_set, &fieldValues[f]->value[i] );
	}
    }
    count = 0;
    for (f = numFonts, b = font_in_set; f; f--, b++) {
	if (*b) count++;
    }
    if (count != matchingFontCount) {
	Boolean *sp = fontInSet;
	FieldValueList *fieldValue = fieldValues[field];
	for (b = font_in_set, f = 0; f < numFonts; f++, b++, sp++) {
	    if (*b != *sp) {
		int i = fonts[f].value_index[field];
		FieldValue *val = &fieldValue->value[i];
		val->enable = True;
		XtSetSensitive(val->menu_item, True);
		if (++count == matchingFontCount) break;
	    }
	}
    }
    XtFree(font_in_set);
    if (enabledMenuIndex < field)
	EnableMenu((XtPointer)field);
}


void EnableMenu(closure)
    XtPointer closure;
{
    int field = (int)closure;
    FieldValue *val = fieldValues[field]->value;
    int f;
    Widget *managed = NULL, *pManaged = NULL;
    Widget *unmanaged = NULL, *pUnmanaged = NULL;
    Boolean showUnselectable = fieldValues[field]->show_unselectable;

    for (f = fieldValues[field]->count; f; f--, val++) {
	if (showUnselectable) {
	    if (val->enable != XtIsSensitive(val->menu_item))
		XtSetSensitive(val->menu_item, val->enable);
	}
	else {
	    if (val->enable != XtIsManaged(val->menu_item)) {
		if (val->enable) {
		    if (managed == NULL) {
			managed = (Widget*)
			    XtMalloc(fieldValues[field]->count*sizeof(Widget));
			pManaged = managed;
		    }
		    *pManaged++ = val->menu_item;
		}
		else {
		    if (unmanaged == NULL) {
			unmanaged = (Widget*)
			    XtMalloc(fieldValues[field]->count*sizeof(Widget));
			pUnmanaged = unmanaged;
		    }
		    *pUnmanaged++ = val->menu_item;
		}
	    }
	}
    }
    if (pManaged != managed) {
	XtManageChildren(managed, pManaged - managed);
	XtFree(managed);
    }
    if (pUnmanaged != unmanaged) {
	XtUnmanageChildren(unmanaged, pUnmanaged - unmanaged);
	XtFree(unmanaged);
    }
    enabledMenuIndex = field;
}


FlushXqueue(dpy)
    Display *dpy;
{
    XSync(dpy, False);
    while (XtAppPending(appCtx)) XtAppProcessEvent(appCtx, XtIMAll);
}


/* ARGSUSED */
void Quit(w, closure, callData)
    Widget w;
    XtPointer closure, callData;
{
    XtCloseDisplay(XtDisplay(w));
    if (AppRes.print_on_quit) printf( "%s", currentFontNameString );
    exit(0);
}


Boolean ConvertSelection(w, selection, target, type, value, length, format)
    Widget w;
    Atom *selection, *target, *type;
    XtPointer *value;
    unsigned long *length;
    int *format;
{
    if (XmuConvertStandardSelection(w,selection,target,type,value,length,format))
	return True;

    if (*target == XA_STRING) {
	*type = XA_STRING;
	*value = currentFontNameString;
	*length = strlen(*value);
	*format = 8;
	return True;
    }
    else {
	return False;
    }
}

static AtomPtr _XA_PRIMARY_FONT = NULL;
#define XA_PRIMARY_FONT XmuInternAtom(XtDisplay(w),_XA_PRIMARY_FONT)

/* ARGSUSED */
void LoseSelection(w, selection)
    Widget w;
    Atom *selection;
{
    Arg args[1];
    XtSetArg( args[0], XtNstate, False );
    XtSetValues( w, args, ONE );
    if (*selection == XA_PRIMARY_FONT) {
	XtSetSensitive(currentFontName, False);
    }
}


/* ARGSUSED */
void DoneSelection(w, selection, target)
    Widget w;
    Atom *selection, *target;
{
    /* do nothing */
}


/* ARGSUSED */
void OwnSelection(w, closure, callData)
    Widget w;
    XtPointer closure, callData;
{
    Time time = XtLastTimestampProcessed(XtDisplay(w));
    Boolean primary = (Boolean)closure;
    Boolean own = (Boolean)callData;

    if (_XA_PRIMARY_FONT == NULL)
	_XA_PRIMARY_FONT = XmuMakeAtom("PRIMARY_FONT");

    if (own) {
	XtOwnSelection( w, XA_PRIMARY_FONT, time,
			ConvertSelection, LoseSelection, DoneSelection );
	if (primary)
	    XtOwnSelection( w, XA_PRIMARY, time,
			   ConvertSelection, LoseSelection, DoneSelection );
	if (!XtIsSensitive(currentFontName)) {
	    XtSetSensitive(currentFontName, True);
	}
    }
    else {
	XtDisownSelection(w, XA_PRIMARY_FONT, time);
	if (primary)
	    XtDisownSelection(w, XA_PRIMARY, time);
	XtSetSensitive(currentFontName, False);
    }
}
