#ifdef REV_INFO
#ifndef lint
static char SCCSID[] = "OSF/Motif: @(#)MapEvents.c	3.7 91/01/10";
#endif /* lint */
#endif /* REV_INFO */
/******************************************************************************
*******************************************************************************
*
*  (c) Copyright 1989, 1990, 1991 OPEN SOFTWARE FOUNDATION, INC.
*  (c) Copyright 1987, 1988, 1989, 1990 HEWLETT-PACKARD COMPANY
*  (c) Copyright 1988 MASSACHUSETTS INSTITUTE OF TECHNOLOGY 
*  ALL RIGHTS RESERVED
*  
*  	THIS SOFTWARE IS FURNISHED UNDER A LICENSE AND MAY BE USED
*  AND COPIED ONLY IN ACCORDANCE WITH THE TERMS OF SUCH LICENSE AND
*  WITH THE INCLUSION OF THE ABOVE COPYRIGHT NOTICE.  THIS SOFTWARE OR
*  ANY OTHER COPIES THEREOF MAY NOT BE PROVIDED OR OTHERWISE MADE
*  AVAILABLE TO ANY OTHER PERSON.  NO TITLE TO AND OWNERSHIP OF THE
*  SOFTWARE IS HEREBY TRANSFERRED.
*  
*  	THE INFORMATION IN THIS SOFTWARE IS SUBJECT TO CHANGE WITHOUT
*  NOTICE AND SHOULD NOT BE CONSTRUED AS A COMMITMENT BY OPEN SOFTWARE
*  FOUNDATION, INC. OR ITS THIRD PARTY SUPPLIERS  
*  
*  	OPEN SOFTWARE FOUNDATION, INC. AND ITS THIRD PARTY SUPPLIERS,
*  ASSUME NO RESPONSIBILITY FOR THE USE OR INABILITY TO USE ANY OF ITS
*  SOFTWARE .   OSF SOFTWARE IS PROVIDED "AS IS" WITHOUT WARRANTY OF ANY
*  KIND, AND OSF EXPRESSLY DISCLAIMS ALL IMPLIED WARRANTIES, INCLUDING
*  BUT NOT LIMITED TO THE IMPLIED WARRANTIES OF MERCHANTABILITY AND
*  FITNESS FOR A PARTICULAR PURPOSE.
*  
*  Notice:  Notwithstanding any other lease or license that may pertain to,
*  or accompany the delivery of, this computer software, the rights of the
*  Government regarding its use, reproduction and disclosure are as set
*  forth in Section 52.227-19 of the FARS Computer Software-Restricted
*  Rights clause.
*  
*  (c) Copyright 1989, 1990, 1991 Open Software Foundation, Inc.  Unpublished - all
*  rights reserved under the Copyright laws of the United States.
*  
*  RESTRICTED RIGHTS NOTICE:  Use, duplication, or disclosure by the
*  Government is subject to the restrictions as set forth in subparagraph
*  (c)(1)(ii) of the Rights in Technical Data and Computer Software clause
*  at DFARS 52.227-7013.
*  
*  Open Software Foundation, Inc.
*  11 Cambridge Center
*  Cambridge, MA   02142
*  (617)621-8700
*  
*  RESTRICTED RIGHTS LEGEND:  This computer software is submitted with
*  "restricted rights."  Use, duplication or disclosure is subject to the
*  restrictions as set forth in NASA FAR SUP 18-52.227-79 (April 1985)
*  "Commercial Computer Software- Restricted Rights (April 1985)."  Open
*  Software Foundation, Inc., 11 Cambridge Center, Cambridge, MA  02142.  If
*  the contract contains the Clause at 18-52.227-74 "Rights in Data General"
*  then the "Alternate III" clause applies.
*  
*  (c) Copyright 1989, 1990, 1991 Open Software Foundation, Inc.
*  ALL RIGHTS RESERVED 
*  
*  
* Open Software Foundation is a trademark of The Open Software Foundation, Inc.
* OSF is a trademark of Open Software Foundation, Inc.
* OSF/Motif is a trademark of Open Software Foundation, Inc.
* Motif is a trademark of Open Software Foundation, Inc.
* DEC is a registered trademark of Digital Equipment Corporation
* DIGITAL is a registered trademark of Digital Equipment Corporation
* X Window System is a trademark of the Massachusetts Institute of Technology
*
*******************************************************************************
******************************************************************************/
#include <X11/Intrinsic.h>
#include <X11/IntrinsicP.h>
#include <X11/Xutil.h>
#include <X11/Xatom.h>
#include <Xm/XmP.h>


static Boolean ParseImmed();
static Boolean ParseKeySym();



typedef struct {
   char       * event;
   XrmQuark     signature;
   unsigned int eventType;
   Boolean      (*parseProc)();
   unsigned int closure;
} EventKey;

static EventKey modifierStrings[] = {

  /* Modifier,  Quark,  Mask */

    {"None",    0,      NULL,    NULL,  None},
    {"Shift",	0,      NULL,    NULL,	ShiftMask},
    {"Lock",	0,      NULL,    NULL,  LockMask},
    {"Ctrl",	0,      NULL,    NULL,	ControlMask},
    {"Meta",	0,      NULL,    NULL,	Mod1Mask},
    {"Alt",	0,      NULL,    NULL,	Mod1Mask},
    {"Mod1",	0,      NULL,    NULL,	Mod1Mask},
    {"Mod2",	0,      NULL,    NULL,	Mod2Mask},
    {"Mod3",	0,      NULL,    NULL,	Mod3Mask},
    {"Mod4",	0,      NULL,    NULL,	Mod4Mask},
    {"Mod5",	0,      NULL,    NULL,	Mod5Mask},
    {NULL, NULL, NULL, NULL, NULL},
};

static EventKey buttonEvents[] = {

/* Event Name,    Quark, Event Type,    DetailProc      Closure */

{"Btn1Down",        NULL, ButtonPress,  ParseImmed,     Button1},
{"Button1",         NULL, ButtonPress,  ParseImmed,     Button1},
{"Btn1",            NULL, ButtonPress,  ParseImmed,     Button1},
{"Btn2Down",        NULL, ButtonPress,  ParseImmed,     Button2},
{"Button2",         NULL, ButtonPress,  ParseImmed,     Button2},
{"Btn2",            NULL, ButtonPress,  ParseImmed,     Button2},
{"Btn3Down",        NULL, ButtonPress,  ParseImmed,     Button3},
{"Button3",         NULL, ButtonPress,  ParseImmed,     Button3},
{"Btn3",            NULL, ButtonPress,  ParseImmed,     Button3},
{"Btn4Down",        NULL, ButtonPress,  ParseImmed,     Button4},
{"Button4",         NULL, ButtonPress,  ParseImmed,     Button4},
{"Btn4",            NULL, ButtonPress,  ParseImmed,     Button4},
{"Btn5Down",        NULL, ButtonPress,  ParseImmed,     Button5},
{"Button5",         NULL, ButtonPress,  ParseImmed,     Button5},
{"Btn5",            NULL, ButtonPress,  ParseImmed,     Button5},
#ifdef FUTURES
{"Btn1Up",          NULL, ButtonRelease,    ParseImmed, Button1},
{"Btn2Up",          NULL, ButtonRelease,    ParseImmed, Button2},
{"Btn3Up",          NULL, ButtonRelease,    ParseImmed, Button3},
{"Btn4Up",          NULL, ButtonRelease,    ParseImmed, Button4},
{"Btn5Up",          NULL, ButtonRelease,    ParseImmed, Button5},
#endif
{ NULL, NULL, NULL, NULL}};


static EventKey keyEvents[] = {

/* Event Name,	  Quark, Event Type,	DetailProc	Closure */

{"KeyPress",	    NULL, KeyPress,	ParseKeySym,	NULL},
{"Key", 	    NULL, KeyPress,	ParseKeySym,	NULL},
{"KeyDown", 	    NULL, KeyPress,	ParseKeySym,	NULL},
{"KeyUp", 	    NULL, KeyRelease,   ParseKeySym,	NULL},
{"KeyRelease", 	    NULL, KeyRelease,   ParseKeySym,	NULL},
{ NULL, NULL, NULL, NULL}};

static unsigned int buttonModifierMasks[] = {
    0, Button1Mask, Button2Mask, Button3Mask, Button4Mask, Button5Mask
};

static initialized = FALSE;



/*************************************<->*************************************
 *
 *  Numeric convertion routines
 *
 *   Description:
 *   -----------
 *     xxxxxxxxxxxxxxxxxxxxxxx
 *
 *
 *   Inputs:
 *   ------
 *     xxxxxxxxxxxx = xxxxxxxxxxxxx
 * 
 *   Outputs:
 *   -------
 *     xxxxxxxxxxxx = xxxxxxxxxxxxx
 *
 *   Procedures Called
 *   -----------------
 *
 *************************************<->***********************************/

static unsigned int StrToHex(str)
    String str;
{
    register char   c;
    register int    val = 0;

    while (c = *str) {
	if ('0' <= c && c <= '9') val = val*16+c-'0';
	else if ('a' <= c && c <= 'z') val = val*16+c-'a'+10;
	else if ('A' <= c && c <= 'Z') val = val*16+c-'A'+10;
	else return -1;
	str++;
    }

    return val;
}

static unsigned int StrToOct(str)
    String str;
{
    register char c;
    register int  val = 0;

    while (c = *str) {
        if ('0' <= c && c <= '7') val = val*8+c-'0'; else return -1;
	str++;
    }

    return val;
}

static unsigned int StrToNum(str)
    String str;
{
    register char c;
    register int val = 0;

    if (*str == '0') {
	str++;
	if (*str == 'x' || *str == 'X') return StrToHex(++str);
	else return StrToOct(str);
    }

    while (c = *str) {
	if ('0' <= c && c <= '9') val = val*10+c-'0';
	else return -1;
	str++;
    }

    return val;
}


/*************************************<->*************************************
 *
 *  FillInQuarks (parameters)
 *
 *   Description:
 *   -----------
 *     Converts each string entry in the modifier/event tables to a
 *     quark, thus facilitating faster comparisons.
 *
 *
 *   Inputs:
 *   ------
 *     xxxxxxxxxxxx = xxxxxxxxxxxxx
 * 
 *   Outputs:
 *   -------
 *     xxxxxxxxxxxx = xxxxxxxxxxxxx
 *
 *   Procedures Called
 *   -----------------
 *
 *************************************<->***********************************/

static void FillInQuarks (table)

    EventKey * table;

{
    register int i;

    for (i=0; table[i].event; i++)
        table[i].signature = XrmStringToQuark(table[i].event);
}


/*************************************<->*************************************
 *
 *  LookupModifier (parameters)
 *
 *   Description:
 *   -----------
 *     Compare the passed in string to the list of valid modifiers.
 *
 *
 *   Inputs:
 *   ------
 *     xxxxxxxxxxxx = xxxxxxxxxxxxx
 * 
 *   Outputs:
 *   -------
 *     xxxxxxxxxxxx = xxxxxxxxxxxxx
 *
 *   Procedures Called
 *   -----------------
 *
 *************************************<->***********************************/

static Boolean LookupModifier (name, valueP)

    String name;
    unsigned int *valueP;

{
    register int i;
    register XrmQuark signature = XrmStringToQuark(name);

    for (i=0; modifierStrings[i].event != NULL; i++)
	if (modifierStrings[i].signature == signature) {
	    *valueP = modifierStrings[i].closure;
	    return TRUE;
	}

    return FALSE;
}


/*************************************<->*************************************
 *
 *  ScanAlphanumeric (parameters)
 *
 *   Description:
 *   -----------
 *     Scan string until a non-alphanumeric character is encountered.
 *
 *
 *   Inputs:
 *   ------
 *     xxxxxxxxxxxx = xxxxxxxxxxxxx
 * 
 *   Outputs:
 *   -------
 *     xxxxxxxxxxxx = xxxxxxxxxxxxx
 *
 *   Procedures Called
 *   -----------------
 *
 *************************************<->***********************************/

static String ScanAlphanumeric (str)

    register String str;

{
    while (
        ('A' <= *str && *str <= 'Z') || ('a' <= *str && *str <= 'z')
	|| ('0' <= *str && *str <= '9')) str++;
    return str;
}


/*************************************<->*************************************
 *
 *  ScanWhitespace (parameters)
 *
 *   Description:
 *   -----------
 *     Scan the string, skipping over all white space characters.
 *
 *
 *   Inputs:
 *   ------
 *     xxxxxxxxxxxx = xxxxxxxxxxxxx
 * 
 *   Outputs:
 *   -------
 *     xxxxxxxxxxxx = xxxxxxxxxxxxx
 *
 *   Procedures Called
 *   -----------------
 *
 *************************************<->***********************************/

static String ScanWhitespace(str)
    register String str;
{
    while (*str == ' ' || *str == '\t') str++;
    return str;
}


/*************************************<->*************************************
 *
 *  ParseImmed (parameters)
 *
 *   Description:
 *   -----------
 *     xxxxxxxxxxxxxxxxxxxxxxx
 *
 *
 *   Inputs:
 *   ------
 *     xxxxxxxxxxxx = xxxxxxxxxxxxx
 *
 *   Outputs:
 *   -------
 *     xxxxxxxxxxxx = xxxxxxxxxxxxx
 *
 *   Procedures Called
 *   -----------------
 *
 *************************************<->***********************************/
/* ARGSUSED */
static Boolean ParseImmed (str, closure, detail)
   String str;
   unsigned int closure;
   unsigned int * detail;

{
   *detail = closure;
   return (TRUE);
}


/*************************************<->*************************************
 *
 *  ParseKeySym (parameters)
 *
 *   Description:
 *   -----------
 *     xxxxxxxxxxxxxxxxxxxxxxx
 *
 *
 *   Inputs:
 *   ------
 *     xxxxxxxxxxxx = xxxxxxxxxxxxx
 * 
 *   Outputs:
 *   -------
 *     xxxxxxxxxxxx = xxxxxxxxxxxxx
 *
 *   Procedures Called
 *   -----------------
 *
 *************************************<->***********************************/
/* ARGSUSED */
static Boolean ParseKeySym (str, closure, detail)

   String str;
   unsigned int closure;
   unsigned int * detail;

{
    char keySymName[100], *start;

    str = ScanWhitespace(str);

    if (*str == '\\') {
	str++;
	keySymName[0] = *str;
	if (*str != '\0' && *str != '\n') str++;
	keySymName[1] = '\0';
	*detail = XStringToKeysym(keySymName);
    } else if (*str == ',' || *str == ':') {
       /* No detail; return a failure */
       *detail = NoSymbol;
       return (FALSE);
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
	*detail = XStringToKeysym(keySymName);
    }

    if (*detail == NoSymbol)
    {
       if (( '0' <= keySymName[0]) && (keySymName[0] <= '9'))
       {
          *detail = StrToNum(keySymName);
          return (TRUE);
       }
       return (FALSE);
    }
    else
       return (TRUE);
}


/*************************************<->*************************************
 *
 *  ParseModifiers (parameters)
 *
 *   Description:
 *   -----------
 *     Parse the string, extracting all modifier specifications.
 *
 *
 *   Inputs:
 *   ------
 *     xxxxxxxxxxxx = xxxxxxxxxxxxx
 * 
 *   Outputs:
 *   -------
 *     xxxxxxxxxxxx = xxxxxxxxxxxxx
 *
 *   Procedures Called
 *   -----------------
 *
 *************************************<->***********************************/

static String ParseModifiers(str, modifiers, status)

    register String str;
    unsigned int  * modifiers;
    Boolean       * status;

{
    register String start;
    char modStr[100];
    Boolean notFlag;
    unsigned int maskBit;

    /* Initially assume all is going to go well */
    *status = TRUE;
    *modifiers = 0;
 
    /* Attempt to parse the first button modifier */
    str = ScanWhitespace(str);
    start = str;
    str = ScanAlphanumeric(str);
    if (start != str) {
         (void) strncpy(modStr, start, str-start);
          modStr[str-start] = '\0';
          if (LookupModifier(modStr, &maskBit))
          {
	    if (maskBit== None) {
		*modifiers = 0;
                str = ScanWhitespace(str);
	        return str;
            }
         }
         str = start;
    }

   
    /* Keep parsing modifiers, until the event specifier is encountered */
    while ((*str != '<') && (*str != '\0')) {
        if (*str == '~') {
             notFlag = TRUE;
             str++;
          } else 
              notFlag = FALSE;

	start = str;
        str = ScanAlphanumeric(str);
        if (start == str) {
           /* ERROR: Modifier or '<' missing */
           *status = FALSE;
           return str;
        }
        (void) strncpy(modStr, start, str-start);
        modStr[str-start] = '\0';

        if (!LookupModifier(modStr, &maskBit))
        {
           /* Unknown modifier name */
           *status = FALSE;
           return str;
        }

	if (notFlag) 
           *modifiers &= ~maskBit;
	else 
           *modifiers |= maskBit;
        str = ScanWhitespace(str);
    }

    return str;
}


/*************************************<->*************************************
 *
 *  ParseEventType (parameters)
 *
 *   Description:
 *   -----------
 *     xxxxxxxxxxxxxxxxxxxxxxx
 *
 *
 *   Inputs:
 *   ------
 *     xxxxxxxxxxxx = xxxxxxxxxxxxx
 * 
 *   Outputs:
 *   -------
 *     xxxxxxxxxxxx = xxxxxxxxxxxxx
 *
 *   Procedures Called
 *   -----------------
 *
 *************************************<->***********************************/

static String ParseEventType(str, table, eventType, _index, status)

    register String str;
    EventKey * table;
    unsigned int * eventType;
    Cardinal     * _index;
    Boolean      * status;

{
    String start = str;
    char eventTypeStr[100];
    register Cardinal   i;
    register XrmQuark	signature;

    /* Parse out the event string */
    str = ScanAlphanumeric(str);
    (void) strncpy(eventTypeStr, start, str-start);
    eventTypeStr[str-start] = '\0';

    /* Attempt to match the parsed event against our supported event set */
    signature = XrmStringToQuark(eventTypeStr);
    for (i = 0; table[i].signature != NULL; i++)
        if (table[i].signature == signature)
        {
           *_index = i;
           *eventType = table[*_index].eventType;

           *status = TRUE;
           return str; 
        }

    /* Unknown event specified */
    *status = FALSE;
    return (str);
}


/*************************************<->*************************************
 *
 *  _MapEvent (parameters)
 *
 *   Description:
 *   -----------
 *     xxxxxxxxxxxxxxxxxxxxxxx
 *
 *
 *   Inputs:
 *   ------
 *     xxxxxxxxxxxx = xxxxxxxxxxxxx
 * 
 *   Outputs:
 *   -------
 *     xxxxxxxxxxxx = xxxxxxxxxxxxx
 *
 *   Procedures Called
 *   -----------------
 *
 *************************************<->***********************************/

static Boolean 
_MapEvent (str, table, eventType, detail, modifiers)

    register String str;
    EventKey     * table;
    unsigned int * eventType;
    unsigned int * detail;
    unsigned int * modifiers;

{
    Cardinal _index;
    Boolean  status;
 
    /* Initialize, if first time called */
    if (!initialized)
    {
       initialized = TRUE;
       FillInQuarks (buttonEvents);
       FillInQuarks (modifierStrings);
       FillInQuarks (keyEvents);
    }

    /* Parse the modifiers */
    str = ParseModifiers(str, modifiers, &status);
    if ( status == FALSE || *str != '<') 
       return (FALSE);
    else 
       str++;

    /* Parse the event type and detail */
    str = ParseEventType(str, table, eventType, &_index, &status);
    if (status == FALSE || *str != '>') 
       return (FALSE);
    else 
       str++;

    /* Save the detail */
    return ((*(table[_index].parseProc))(str, table[_index].closure, detail));
}

/*************************************<->*************************************
 *
 *  _MapBtnEvent (parameters)
 *
 *   Description:
 *   -----------
 *     xxxxxxxxxxxxxxxxxxxxxxx
 *
 *
 *   Inputs:
 *   ------
 *     xxxxxxxxxxxx = xxxxxxxxxxxxx
 *
 *   Outputs:
 *   -------
 *     xxxxxxxxxxxx = xxxxxxxxxxxxx
 *
 *   Procedures Called
 *   -----------------
 *
 *************************************<->***********************************/
#ifdef _NO_PROTO
Boolean _XmMapBtnEvent (str, eventType, button, modifiers)

    register String str;
    unsigned int * eventType;
    unsigned int * button;
    unsigned int * modifiers;

#else /* _NO_PROTO */
Boolean _XmMapBtnEvent (String str, unsigned int *eventType, unsigned int *button, unsigned int *modifiers)
#endif /* _NO_PROTO */
{
    if (_MapEvent (str, buttonEvents, eventType, button, modifiers) == FALSE)
       return (FALSE);

    /*
     * The following is a fix for an X11 deficiency in regards to
     * modifiers in grabs.
     */
    if (*eventType == ButtonRelease)
    {
        /* the button that is going up will always be in the modifiers... */
        *modifiers |= buttonModifierMasks[*button];
    }

    return (TRUE);
}



/*************************************<->*************************************
 *
 *  _XmMapKeyEvent (parameters)
 *
 *   Description:
 *   -----------
 *     xxxxxxxxxxxxxxxxxxxxxxx
 *
 *
 *   Inputs:
 *   ------
 *     xxxxxxxxxxxx = xxxxxxxxxxxxx
 * 
 *   Outputs:
 *   -------
 *     xxxxxxxxxxxx = xxxxxxxxxxxxx
 *
 *   Procedures Called
 *   -----------------
 *
 *************************************<->***********************************/

#ifdef _NO_PROTO
Boolean _XmMapKeyEvent (str, eventType, key, modifiers)

    register String str;
    unsigned int * eventType;
    unsigned int * key;
    unsigned int * modifiers;

#else /* _NO_PROTO */
Boolean _XmMapKeyEvent (String str, unsigned int *eventType, unsigned int *key, unsigned int *modifiers)
#endif /* _NO_PROTO */
{
    return (_MapEvent (str, keyEvents, eventType, key, modifiers));
}


/*************************************<->*************************************
 *
 *  _XmMatchBtnEvent (parameters)
 *
 *   Description:
 *   -----------
 *     Compare the passed in event to the event described by the parameter
 *     list.
 *
 *
 *   Inputs:
 *   ------
 *     xxxxxxxxxxxx = xxxxxxxxxxxxx
 *
 *   Outputs:
 *   -------
 *     xxxxxxxxxxxx = xxxxxxxxxxxxx
 *
 *   Procedures Called
 *   -----------------
 *
 *************************************<->***********************************/

#ifdef _NO_PROTO
Boolean _XmMatchBtnEvent (event, eventType, button, modifiers)

   XButtonEvent * event;
   unsigned int   eventType;
   unsigned int   button;
   unsigned int   modifiers;

#else /* _NO_PROTO */
Boolean _XmMatchBtnEvent (XButtonEvent *event, unsigned int eventType, unsigned int button, unsigned int modifiers)
#endif /* _NO_PROTO */
{
   register unsigned int state = event->state & (ShiftMask | LockMask | 
						ControlMask | Mod1Mask | 
						Mod2Mask | Mod3Mask | 
						Mod4Mask | Mod5Mask);
   if (((eventType == XmIGNORE_EVENTTYPE)||(event->type == eventType)) &&
       (event->button == button) &&
       ((modifiers == AnyModifier)||(state == modifiers)) )
      return (TRUE);
   else
      return (FALSE);
}



/*************************************<->*************************************
 *
 *  _XmMatchKeyEvent (parameters)
 *
 *   Description:
 *   -----------
 *     Compare the passed in event to the event described by the parameter
 *     list.
 *
 *
 *   Inputs:
 *   ------
 *     xxxxxxxxxxxx = xxxxxxxxxxxxx
 * 
 *   Outputs:
 *   -------
 *     xxxxxxxxxxxx = xxxxxxxxxxxxx
 *
 *   Procedures Called
 *   -----------------
 *
 *************************************<->***********************************/

#ifdef _NO_PROTO
Boolean _XmMatchKeyEvent (event, eventType, key, modifiers)

   XKeyEvent    * event;
   unsigned int   eventType;
   unsigned int   key;
   unsigned int   modifiers;

#else /* _NO_PROTO */
Boolean _XmMatchKeyEvent (XKeyEvent *event, unsigned int eventType, unsigned int key, unsigned int modifiers)
#endif /* _NO_PROTO */
{
   if ((event->type == eventType) &&
       (event->keycode == key) &&
       (event->state == modifiers))
      return (TRUE);
   else
      return (FALSE);
}
