#ifdef REV_INFO
#ifndef lint
static char SCCSID[] = "OSF/Motif: @(#)VaSimple.c	3.21 91/01/10";
#endif /* lint */
#endif /* REV_INFO */
/******************************************************************************
*******************************************************************************
*
*  (c) Copyright 1989, 1990, 1991 OPEN SOFTWARE FOUNDATION, INC.
*  (c) Copyright 1987, 1988, 1989, 1990, HEWLETT-PACKARD COMPANY
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

#include <X11/IntrinsicP.h>
#include <X11/StringDefs.h>
#include <X11/Shell.h>
#include <Xm/VaSimple.h>
#include <Xm/Xm.h>

#define XMBUTTONS_ARGS_PER_LIST		5
#define XMCASCADE_ARGS_PER_LIST		3
#define XMTITLE_ARGS_PER_LIST		1
#define XMSEPARATOR_ARGS_PER_LIST	0

#define _XmINVALID_BUTTON_TYPE		0xff

static XmButtonType _XmVaBType_to_XmBType(symbol)
String symbol;
{
    if (strcmp(symbol, XmVaPUSHBUTTON) == 0)
      	return (XmPUSHBUTTON);
    else if (strcmp(symbol, XmVaTOGGLEBUTTON) == 0)
      	return (XmTOGGLEBUTTON);
    else if (strcmp(symbol, XmVaCHECKBUTTON) == 0)
      	return (XmCHECKBUTTON);
    else if (strcmp(symbol, XmVaRADIOBUTTON) == 0)
      	return (XmRADIOBUTTON);
    else if (strcmp(symbol, XmVaCASCADEBUTTON) == 0)
      	return (XmCASCADEBUTTON);
    else if (strcmp(symbol, XmVaSEPARATOR) == 0)
      	return (XmSEPARATOR);
    else if (strcmp(symbol, XmVaDOUBLE_SEPARATOR) == 0)
      	return (XmDOUBLE_SEPARATOR);
    else if (strcmp(symbol, XmVaTITLE) == 0)
      	return (XmTITLE);
    else
      	return (_XmINVALID_BUTTON_TYPE);
}

/*
 *    Given a nested list, _XmCountNestedList() returns counts of the
 *    total number of attribute-value pairs and the count of those
 *    attributes that are typed. The list is counted recursively.
 */
static  void
_XmCountNestedList(avlist, total_count, typed_count)
    XtTypedArgList  avlist;
    int             *total_count;
    int             *typed_count;
{
    for (; avlist->name != NULL; avlist++) {
        if (strcmp(avlist->name, XtVaNestedList) == 0) {
            _XmCountNestedList((XtTypedArgList)avlist->value, total_count,
                	       typed_count);
        } else {
            if (avlist->type != NULL) {
                ++(*typed_count);
            }
            ++(*total_count);
        }
    }    
}


/*
 *    Given a variable length attribute-value list, _XmCountVaList()
 *    returns counts of the total number of attribute-value pairs,
 *    and the count of the number of those attributes that are typed.
 *    The list is counted recursively.
 */
#ifndef _NO_PROTO
_XmCountVaList(va_list var, int* button_count, int* args_count, int* typed_count, int* total_count)
#else
void
_XmCountVaList(var, button_count, args_count, typed_count, total_count)
    va_list     var;
    int         *button_count;
    int         *args_count;
    int         *typed_count;
    int         *total_count;
#endif
{
    String          attr;
    int		    i;

    *button_count = 0;
    *args_count = 0;
    *typed_count = 0;
    *total_count = 0;
 
    for(attr = va_arg(var, String) ; attr != NULL;
                        attr = va_arg(var, String)) {
/* Count typed Args */
        if (strcmp(attr, XtVaTypedArg) == 0) {
            va_arg(var, String);
            va_arg(var, String);
            va_arg(var, XtArgVal);
            va_arg(var, int);
            ++(*total_count);
            ++(*typed_count);
        } else if (strcmp(attr, XtVaNestedList) == 0) {
            _XmCountNestedList(va_arg(var, XtTypedArgList),
			       total_count, typed_count);
        } else {
/* Count valid VaBUTTONS (done here because of the variable arg length) */
	    if (strcmp(attr, XmVaCASCADEBUTTON) == 0) {
		for (i = 1; i < XMCASCADE_ARGS_PER_LIST; i++)
		  va_arg(var, XtArgVal);		
		++(*total_count);
		++(*button_count);
	    } else if ((strcmp(attr, XmVaSEPARATOR) == 0) ||
		(strcmp(attr, XmVaDOUBLE_SEPARATOR) == 0)) {
		++(*total_count);
		++(*button_count);
	    } else if (strcmp(attr, XmVaTITLE) == 0){
		  va_arg(var, XtArgVal);
		++(*total_count);
		++(*button_count);
/* Check matches other known VaBUTTONS */
	    } else if (_XmVaBType_to_XmBType(attr) != _XmINVALID_BUTTON_TYPE) {
		for (i = 1; i < XMBUTTONS_ARGS_PER_LIST; i++)
		  va_arg(var, XtArgVal);
		++(*total_count);
		++(*button_count);
/* Else it's a simple Arg */
	    } else {
		va_arg(var, XtArgVal);
		++(*args_count);
		++(*total_count);
	    }
	}
    }
}


/*
 *    _XmTypedArgToArg() invokes a resource converter to convert the
 *    passed typed arg into a name/value pair and stores the name/value
 *    pair in the passed Arg structure. It returns 1 if the conversion
 *    succeeded and 0 if the conversion failed.
 */
static int
_XmTypedArgToArg(widget, typed_arg, arg_return, resources, num_resources)
    Widget              widget;
    XtTypedArgList      typed_arg;
    ArgList             arg_return;
    XtResourceList      resources;
    Cardinal            num_resources;
{     
    String              to_type = NULL;
    XrmValue            from_val, to_val;
    register int        i;
      

    if (widget == NULL) {
        XtAppWarningMsg(XtWidgetToApplicationContext(widget),
            "nullWidget", "xtConvertVarTArgList", "XtToolkitError",
	    "XtVaTypedArg conversion needs non-NULL widget handle",
            (String *)NULL, (Cardinal *)NULL);
        return(0);
    }
       
    /* again we assume that the XtResourceList is un-compiled */

    for (i = 0; i < num_resources; i++) {
        if (StringToName(typed_arg->name) ==
            StringToName(resources[i].resource_name)) {
            to_type = resources[i].resource_type;
            break;
        }
    }

    if (to_type == NULL) {
        XtAppWarningMsg(XtWidgetToApplicationContext(widget),
            "unknownType", "xtConvertVarTArgList", "XtToolkitError",
            "Unable to find type of resource for conversion",
            (String *)NULL, (Cardinal *)NULL);
        return(0);
    }
       
    to_val.addr = NULL;
    from_val.size = typed_arg->size;
    if ((strcmp(typed_arg->type, XtRString) == 0) ||
            (typed_arg->size > sizeof(XtArgVal))) {
        from_val.addr = (caddr_t)typed_arg->value;
    } else {
            from_val.addr = (caddr_t)&typed_arg->value;
    }
       
    XtConvert(widget, typed_arg->type, &from_val, to_type, &to_val);
 
    if (to_val.addr == NULL) {
        XtAppWarningMsg(XtWidgetToApplicationContext(widget),
            "conversionFailed", "xtConvertVarToArgList", "XtToolkitError",
            "Type conversion failed", (String *)NULL, (Cardinal *)NULL);
        return(0);
    }

    arg_return->name = typed_arg->name;

    if (strcmp(to_type, XtRString) == 0) {
	arg_return->value = (XtArgVal) to_val.addr;
    }
    else {
	if (to_val.size == sizeof(long))
	    arg_return->value = (XtArgVal) *(long *)to_val.addr;
	else if (to_val.size == sizeof(short))
	    arg_return->value = (XtArgVal) *(short *)to_val.addr;
	else if (to_val.size == sizeof(char))
	    arg_return->value = (XtArgVal) *(char *)to_val.addr;
	else if (to_val.size == sizeof(XtArgVal))
	    arg_return->value = *(XtArgVal *)to_val.addr;
    }
       
    return(1);
}

/*
 *    _XmNestedArgtoArg() converts the passed nested list into
 *    an ArgList/count.
 */
static int
_XmNestedArgtoArg(widget, avlist, args, resources, num_resources)
    Widget              widget;
    XtTypedArgList      avlist;
    ArgList             args;
    XtResourceList      resources;
    Cardinal            num_resources;
{
    int         count = 0;
 
    for (; avlist->name != NULL; avlist++) {
        if (avlist->type != NULL) {
            /* If widget is NULL, the typed arg is ignored */
            if (widget != NULL) {
                /* this is a typed arg */
                count += _XmTypedArgToArg(widget, avlist, (args+count),
                             resources, num_resources);
            }
        } else if (strcmp(avlist->name, XtVaNestedList) == 0) {
            count += _XmNestedArgtoArg(widget, (XtTypedArgList)avlist->value,
                        (args+count), resources, num_resources);
        } else {
            (args+count)->name = avlist->name;
            (args+count)->value = avlist->value;
            ++count;
        }
    }

    return(count);
}

static int _XmNestedArgtoTypedArg(args, avlist) 
    XtTypedArgList      args;
    XtTypedArgList      avlist;
{    
    int         count = 0;
     
    for (; avlist->name != NULL; avlist++) { 
        if (avlist->type != NULL) { 
            (args+count)->name = avlist->name; 
            (args+count)->type = avlist->type; 
            (args+count)->size = avlist->size;
            (args+count)->value = avlist->value;
            ++count; 
        } else if(strcmp(avlist->name, XtVaNestedList) == 0) {             
            count += _XmNestedArgtoTypedArg((args+count),  
                            (XtTypedArgList)avlist->value); 
        } else {                             
            (args+count)->name = avlist->name; 
	    (args+count)->type = NULL;
            (args+count)->value = avlist->value; 
            ++count;
        }                                     
    }         
    return(count);
}


/*
 *    Given a variable argument list, _XmVaToTypedArgList() returns 
 *    the equivalent TypedArgList. _XmVaToTypedArgList() handles nested
 *    lists.
 *    Note: _XmVaToTypedArgList() does not do type conversions.
 */
#ifndef _NO_PROTO
void
_XmVaToTypedArgList(
    va_list             var,
    int			max_count,
    XtTypedArgList   	*args_return,
    Cardinal            *num_args_return)
#else
void
_XmVaToTypedArgList(var, max_count, args_return, num_args_return)
    va_list             var;
    int			max_count;
    XtTypedArgList   	*args_return;
    Cardinal            *num_args_return;
#endif
{
    XtTypedArgList	args = NULL;
    String              attr;
    int			count;

    args = (XtTypedArgList)
	XtMalloc((unsigned)(max_count * sizeof(XtTypedArg))); 

    for(attr = va_arg(var, String), count = 0 ; attr != NULL;
		    attr = va_arg(var, String)) {
        if (strcmp(attr, XtVaTypedArg) == 0) {
	    args[count].name = va_arg(var, String);
	    args[count].type = va_arg(var, String);
	    args[count].value = va_arg(var, XtArgVal);
	    args[count].size = va_arg(var, int);
	    ++count;
	} else if (strcmp(attr, XtVaNestedList) == 0) {
   	    count += _XmNestedArgtoTypedArg(&args[count], 
			va_arg(var, XtTypedArgList));
	} else {
	    args[count].name = attr;
	    args[count].type = NULL;
	    args[count].value = va_arg(var, XtArgVal);
	    ++count;
	}
    }

    *args_return = args;
    *num_args_return = count;
}


static void
_XmVaProcessEverything(widget, var, buttonTypes, buttonStrings, buttonMnemonics,
			buttonAccelerators, buttonAcceleratorText, button_count,
			args, num_args)

    Widget widget;
    va_list     var;
    XmButtonTypeTable	*buttonTypes;
    XmStringTable	*buttonStrings;
    XmKeySymTable	*buttonMnemonics;
    String *		*buttonAccelerators;
    XmStringTable	*buttonAcceleratorText;
    int			button_count;
    ArgList		*args;
    int			num_args;
{

    XtTypedArg		typed_args;
    String              attr;
    int			count, bcount;



    *args = (ArgList) XtMalloc(num_args * sizeof(Arg));

/* Process the routine specific args */
    *buttonTypes = (XmButtonTypeTable)
	XtMalloc((unsigned)(button_count * sizeof(XmButtonType *)));

    *buttonStrings = (XmStringTable)
	XtMalloc((unsigned)(button_count * sizeof(XmString *)));

    *buttonMnemonics = (XmKeySymTable)
	XtMalloc((unsigned)(button_count * sizeof(KeySym *)));

    *buttonAccelerators = (String *)
	XtMalloc((unsigned)(button_count * sizeof(String *)));

    *buttonAcceleratorText = (XmStringTable)
	XtMalloc((unsigned)(button_count * sizeof(XmString *)));

    bcount = 0;
    for(attr = va_arg(var, String), count = 0 ; attr != NULL;
		    attr = va_arg(var, String)) {
        if (strcmp(attr, XtVaTypedArg) == 0) {
	    typed_args.name = va_arg(var, String);
	    typed_args.type = va_arg(var, String);
	    typed_args.value = va_arg(var, XtArgVal);
	    typed_args.size = va_arg(var, int);
	    count += _XmTypedArgToArg(widget, &typed_args, &args[count],
			     NULL, NULL);
	} else if (strcmp(attr, XtVaNestedList) == 0) {
	    count += _XmNestedArgtoArg(widget, va_arg(var, XtTypedArgList),
			&args[count], NULL, NULL);
	} else if ((strcmp(attr, XmVaSEPARATOR) == 0) ||
	    (strcmp(attr, XmVaDOUBLE_SEPARATOR) == 0)) {
		(*buttonTypes)[bcount] = _XmVaBType_to_XmBType(attr);
		(*buttonStrings)[bcount] = (XmString) NULL;
		(*buttonMnemonics)[bcount] = (KeySym) NULL;
		(*buttonAccelerators)[bcount] = (String) NULL;
		(*buttonAcceleratorText)[bcount] = (XmString) NULL;
	    ++bcount;
	} else if (strcmp(attr, XmVaTITLE) == 0){
		(*buttonTypes)[bcount] = _XmVaBType_to_XmBType(attr);
		(*buttonStrings)[bcount] = va_arg(var, XmString);
		(*buttonMnemonics)[bcount] = (KeySym) NULL;
		(*buttonAccelerators)[bcount] = (String) NULL;
		(*buttonAcceleratorText)[bcount] = (XmString) NULL;
	    ++bcount;
	} else if (strcmp(attr, XmVaCASCADEBUTTON) == 0) {
		(*buttonTypes)[bcount] = _XmVaBType_to_XmBType(attr);
		(*buttonStrings)[bcount] = va_arg(var, XmString);
		(*buttonMnemonics)[bcount] = va_arg(var, KeySym);
		(*buttonAccelerators)[bcount] = (String) NULL;
		(*buttonAcceleratorText)[bcount] = (XmString) NULL;
	    ++bcount;
	} else if (_XmVaBType_to_XmBType(attr) != _XmINVALID_BUTTON_TYPE) {
		(*buttonTypes)[bcount] = _XmVaBType_to_XmBType(attr);
		(*buttonStrings)[bcount] = va_arg(var, XmString);
		(*buttonMnemonics)[bcount] = va_arg(var, KeySym);
		(*buttonAccelerators)[bcount] = va_arg(var, String);
		(*buttonAcceleratorText)[bcount] = va_arg(var, XmString);
	    ++bcount;
	} else {
	    (*args)[count].name = attr;
	    (*args)[count].value = va_arg(var, XtArgVal);
	    ++count;
	}

    }
}


#ifndef _NO_PROTO
Widget
XmVaCreateSimpleMenuBar(Widget parent, String name, ...)
#else
/*VARARGS3*/
Widget XmVaCreateSimpleMenuBar(parent, name, va_alist)
    Widget parent;
    String name;
    va_dcl
#endif
{
#define MB_EXTRA_ARGS 1

    va_list		var;
    register Widget	widget;
    ArgList		args;
    int			button_count, args_count, typed_count, total_count;
    int			n, num_args;
    XmButtonTypeTable	menuBarButtonTypes;
    XmStringTable	menuBarStrings;
    XmKeySymTable	menuBarMnemonics;
    String *		menuBarAccelerators;
    XmStringTable	menuBarAcceleratorText;

/* Count number of arg (arg in this case means number of arg groups) */

    Va_start(var,parent);
    _XmCountVaList(var, &button_count, &args_count, &typed_count, &total_count);
    va_end(var);

/* Convert into a normal ArgList */

    Va_start(var,parent);

    num_args = args_count + (XMCASCADE_ARGS_PER_LIST + MB_EXTRA_ARGS);

    _XmVaProcessEverything(parent, var, &menuBarButtonTypes, &menuBarStrings,
		&menuBarMnemonics, &menuBarAccelerators, &menuBarAcceleratorText,
		button_count, &args, num_args);

    n = args_count;
    XtSetArg (args[n], XmNbuttonCount, button_count); n++;
    XtSetArg (args[n], XmNbuttonType, menuBarButtonTypes); n++;
    XtSetArg (args[n], XmNbuttons, menuBarStrings); n++;
    XtSetArg (args[n], XmNbuttonMnemonics, menuBarMnemonics); n++;

    widget = XmCreateSimpleMenuBar(parent, name, args, n);

    if (args != NULL) {
	XtFree((XtPointer)args);
    }
    if (menuBarButtonTypes != NULL) {
	XtFree((XtPointer)menuBarButtonTypes);
    }
    if (menuBarStrings != NULL) {
	XtFree((XtPointer)menuBarStrings);
    }
    if (menuBarMnemonics != NULL) {
	XtFree((XtPointer)menuBarMnemonics);
    }
    if (menuBarAccelerators != NULL) {
	XtFree((XtPointer)menuBarAccelerators);
    }
    if (menuBarAcceleratorText != NULL) {
	XtFree((XtPointer)menuBarAcceleratorText);
    }

    va_end(var);

    return (widget);
}


#ifndef _NO_PROTO
Widget
XmVaCreateSimplePulldownMenu(Widget parent, String name, int post_from_button, XtCallbackProc callback, ...)
#else
/*VARARGS3*/
Widget XmVaCreateSimplePulldownMenu(parent, name, post_from_button, callback, va_alist)
    Widget parent;
    String name;
    int post_from_button;
    XtCallbackProc callback;
    va_dcl
#endif
{
#define PD_EXTRA_ARGS 3

    va_list		var;
    register Widget	widget;
    Arg			*args;
    int			button_count, args_count, typed_count, total_count;
    int			n, num_args;
    XmButtonTypeTable	pulldownMenuButtonTypes;
    XmStringTable	pulldownMenuStrings;
    XmKeySymTable	pulldownMenuMnemonics;
    String *		pulldownMenuAccelerators;
    XmStringTable	pulldownMenuAcceleratorText;

/* Count number of arg (arg in this case means number of arg groups) */
    Va_start(var,parent);
    _XmCountVaList(var, &button_count, &args_count, &typed_count, &total_count);
    va_end(var);

/* Convert into a normal ArgList */
    Va_start(var,parent);

    num_args = args_count + (XMBUTTONS_ARGS_PER_LIST + PD_EXTRA_ARGS);

    _XmVaProcessEverything(parent, var, &pulldownMenuButtonTypes,
		&pulldownMenuStrings, &pulldownMenuMnemonics, 
		&pulldownMenuAccelerators, &pulldownMenuAcceleratorText,
		button_count, &args, num_args);

    n = args_count;
    XtSetArg (args[n], XmNsimpleCallback, callback); n++;
    XtSetArg (args[n], XmNpostFromButton, post_from_button); n++;
    XtSetArg (args[n], XmNbuttonCount, button_count); n++;
    XtSetArg (args[n], XmNbuttonType, pulldownMenuButtonTypes); n++;
    XtSetArg (args[n], XmNbuttons, pulldownMenuStrings); n++;
    XtSetArg (args[n], XmNbuttonMnemonics, pulldownMenuMnemonics); n++;
    XtSetArg (args[n], XmNbuttonAccelerators,
	      pulldownMenuAccelerators); n++;
    XtSetArg (args[n], XmNbuttonAcceleratorText,
	      pulldownMenuAcceleratorText); n++;

    widget = XmCreateSimplePulldownMenu(parent, name, args, n);

    if (args != NULL) {
	XtFree((XtPointer)args);
    }
    if (pulldownMenuButtonTypes != NULL) {
	XtFree((XtPointer)pulldownMenuButtonTypes);
    }
    if (pulldownMenuStrings != NULL) {
	XtFree((XtPointer)pulldownMenuStrings);
    }
    if (pulldownMenuMnemonics != NULL) {
	XtFree((XtPointer)pulldownMenuMnemonics);
    }
    if (pulldownMenuAccelerators != NULL) {
	XtFree((XtPointer)pulldownMenuAccelerators);
    }
    if (pulldownMenuAcceleratorText != NULL) {
	XtFree((XtPointer)pulldownMenuAcceleratorText);
    }

    va_end(var);

    return (widget);
}


#ifndef _NO_PROTO
Widget
XmVaCreateSimplePopupMenu(Widget parent, String name, XtCallbackProc callback, ...)
#else
/*VARARGS3*/
Widget XmVaCreateSimplePopupMenu(parent, name, callback, va_alist)
    Widget parent;
    String name;
    XtCallbackProc callback;
    va_dcl
#endif
{
#define PU_EXTRA_ARGS 2


    va_list		var;
    register Widget	widget;
    Arg			*args;
    int			button_count, args_count, typed_count, total_count;
    int			n, num_args;
    XmButtonTypeTable	popupMenuButtonTypes;
    XmStringTable	popupMenuStrings;
    XmKeySymTable	popupMenuMnemonics;
    String *		popupMenuAccelerators;
    XmStringTable	popupMenuAcceleratorText;

/* Count number of arg (arg in this case means number of arg groups) */
    Va_start(var,parent);
    _XmCountVaList(var, &button_count, &args_count, &typed_count, &total_count);
    va_end(var);

/* Convert into a normal ArgList */
    Va_start(var,parent);

    num_args = args_count + (XMBUTTONS_ARGS_PER_LIST + PU_EXTRA_ARGS);

    _XmVaProcessEverything(parent, var, &popupMenuButtonTypes,
		&popupMenuStrings, &popupMenuMnemonics, 
		&popupMenuAccelerators, &popupMenuAcceleratorText,
		button_count, &args, num_args);

    n = args_count;
    XtSetArg (args[n], XmNsimpleCallback, callback); n++;
    XtSetArg (args[n], XmNbuttonCount, button_count); n++;
    XtSetArg (args[n], XmNbuttonType, popupMenuButtonTypes); n++;
    XtSetArg (args[n], XmNbuttons, popupMenuStrings); n++;
    XtSetArg (args[n], XmNbuttonMnemonics, popupMenuMnemonics); n++;
    XtSetArg (args[n], XmNbuttonAccelerators,
	      popupMenuAccelerators); n++;
    XtSetArg (args[n], XmNbuttonAcceleratorText,
	      popupMenuAcceleratorText); n++;

    widget = XmCreateSimplePopupMenu(parent, name, args, n);

    if (args != NULL) {
	XtFree((XtPointer)args);
    }
    if (popupMenuButtonTypes != NULL) {
	XtFree((XtPointer)popupMenuButtonTypes);
    }
    if (popupMenuStrings != NULL) {
	XtFree((XtPointer)popupMenuStrings);
    }
    if (popupMenuMnemonics != NULL) {
	XtFree((XtPointer)popupMenuMnemonics);
    }
    if (popupMenuAccelerators != NULL) {
	XtFree((XtPointer)popupMenuAccelerators);
    }
    if (popupMenuAcceleratorText != NULL) {
	XtFree((XtPointer)popupMenuAcceleratorText);
    }

    va_end(var);

    return (widget);
}


#ifndef _NO_PROTO
Widget
XmVaCreateSimpleOptionMenu(Widget parent, String name, XmString option_label, KeySym option_mnemonic, int button_set, XtCallbackProc callback, ...)
#else
/*VARARGS3*/
Widget XmVaCreateSimpleOptionMenu(parent, name, option_label, option_mnemonic, button_set, callback, va_alist)
    Widget parent;
    String name;
    XmString option_label;
    KeySym option_mnemonic;
    int button_set;
    XtCallbackProc callback;
    va_dcl
#endif
{
#define OM_EXTRA_ARGS 5


    va_list		var;
    register Widget	widget;
    Arg			*args;
    int			button_count, args_count, typed_count, total_count;
    int			n, num_args;
    XmButtonTypeTable	optionMenuButtonTypes;
    XmStringTable	optionMenuStrings;
    XmKeySymTable	optionMenuMnemonics;
    String *		optionMenuAccelerators;
    XmStringTable	optionMenuAcceleratorText;

/* Count number of arg (arg in this case means number of arg groups) */
    Va_start(var,parent);
    _XmCountVaList(var, &button_count, &args_count, &typed_count, &total_count);
    va_end(var);

/* Convert into a normal ArgList */
    Va_start(var,parent);

    num_args = args_count + (XMBUTTONS_ARGS_PER_LIST + OM_EXTRA_ARGS);

    _XmVaProcessEverything(parent, var, &optionMenuButtonTypes,
		&optionMenuStrings, &optionMenuMnemonics, 
		&optionMenuAccelerators, &optionMenuAcceleratorText,
		button_count, &args, num_args);

    n = args_count;
    XtSetArg (args[n], XmNsimpleCallback, callback); n++;
    XtSetArg (args[n], XmNoptionLabel, option_label); n++;
    XtSetArg (args[n], XmNoptionMnemonic, option_mnemonic); n++;
    XtSetArg (args[n], XmNbuttonSet, button_set); n++;
    XtSetArg (args[n], XmNbuttonCount, button_count); n++;
    XtSetArg (args[n], XmNbuttonType, optionMenuButtonTypes); n++;
    XtSetArg (args[n], XmNbuttons, optionMenuStrings); n++;
    XtSetArg (args[n], XmNbuttonMnemonics, optionMenuMnemonics); n++;
    XtSetArg (args[n], XmNbuttonAccelerators,
	      optionMenuAccelerators); n++;
    XtSetArg (args[n], XmNbuttonAcceleratorText,
	      optionMenuAcceleratorText); n++;

    widget = XmCreateSimpleOptionMenu(parent, name, args, n);

    if (args != NULL) {
	XtFree((XtPointer)args);
    }
    if (optionMenuButtonTypes != NULL) {
	XtFree((XtPointer)optionMenuButtonTypes);
    }
    if (optionMenuStrings != NULL) {
	XtFree((XtPointer)optionMenuStrings);
    }
    if (optionMenuMnemonics != NULL) {
	XtFree((XtPointer)optionMenuMnemonics);
    }
    if (optionMenuAccelerators != NULL) {
	XtFree((XtPointer)optionMenuAccelerators);
    }
    if (optionMenuAcceleratorText != NULL) {
	XtFree((XtPointer)optionMenuAcceleratorText);
    }

    va_end(var);

    return (widget);
}


#ifndef _NO_PROTO
Widget
XmVaCreateSimpleRadioBox(Widget parent, String name, int button_set, XtCallbackProc callback, ...)
#else
/*VARARGS3*/
Widget XmVaCreateSimpleRadioBox(parent, name, button_set, callback, va_alist)
    Widget parent;
    String name;
    int button_set;
    XtCallbackProc callback;
    va_dcl
#endif
{
#define RB_EXTRA_ARGS 3

    va_list		var;
    register Widget	widget;
    Arg			*args;
    int			button_count, args_count, typed_count, total_count;
    int			n, num_args;
    XmButtonTypeTable	radioBoxButtonTypes;
    XmStringTable	radioBoxStrings;
    XmKeySymTable	radioBoxMnemonics;
    String *		radioBoxAccelerators;
    XmStringTable	radioBoxAcceleratorText;

/* Count number of arg (arg in this case means number of arg groups) */
    Va_start(var,parent);
    _XmCountVaList(var, &button_count, &args_count, &typed_count, &total_count);
    va_end(var);

/* Convert into a normal ArgList */
    Va_start(var,parent);

    num_args = args_count + (XMBUTTONS_ARGS_PER_LIST + RB_EXTRA_ARGS);

    _XmVaProcessEverything(parent, var, &radioBoxButtonTypes,
		&radioBoxStrings, &radioBoxMnemonics, 
		&radioBoxAccelerators, &radioBoxAcceleratorText,
		button_count, &args, num_args);

    n = args_count;
    XtSetArg (args[n], XmNsimpleCallback, callback); n++;
    XtSetArg (args[n], XmNbuttonSet, button_set); n++;
    XtSetArg (args[n], XmNbuttonCount, button_count); n++;
    XtSetArg (args[n], XmNbuttonType, radioBoxButtonTypes); n++;
    XtSetArg (args[n], XmNbuttons, radioBoxStrings); n++;
    XtSetArg (args[n], XmNbuttonMnemonics, radioBoxMnemonics); n++;
    XtSetArg (args[n], XmNbuttonAccelerators,
	      radioBoxAccelerators); n++;
    XtSetArg (args[n], XmNbuttonAcceleratorText,
	      radioBoxAcceleratorText); n++;

    widget = XmCreateSimpleRadioBox(parent, name, args, n);

    if (args != NULL) {
	XtFree((XtPointer)args);
    }
    if (radioBoxButtonTypes != NULL) {
	XtFree((XtPointer)radioBoxButtonTypes);
    }
    if (radioBoxStrings != NULL) {
	XtFree((XtPointer)radioBoxStrings);
    }
    if (radioBoxMnemonics != NULL) {
	XtFree((XtPointer)radioBoxMnemonics);
    }
    if (radioBoxAccelerators != NULL) {
	XtFree((XtPointer)radioBoxAccelerators);
    }
    if (radioBoxAcceleratorText != NULL) {
	XtFree((XtPointer)radioBoxAcceleratorText);
    }

    va_end(var);

    return (widget);
}


#ifndef _NO_PROTO
Widget
XmVaCreateSimpleCheckBox(Widget parent, String name, XtCallbackProc callback, ...)
#else
/*VARARGS3*/
Widget XmVaCreateSimpleCheckBox(parent, name, callback, va_alist)
    Widget parent;
    String name;
    XtCallbackProc callback;
    va_dcl
#endif
{
#define CB_EXTRA_ARGS 2

    va_list		var;
    register Widget	widget;
    Arg			*args;
    int			button_count, args_count, typed_count, total_count;
    int			n, num_args;
    XmButtonTypeTable	checkBoxButtonTypes;
    XmStringTable	checkBoxStrings;
    XmKeySymTable	checkBoxMnemonics;
    String *		checkBoxAccelerators;
    XmStringTable	checkBoxAcceleratorText;

/* Count number of arg (arg in this case means number of arg groups) */
    Va_start(var,parent);
    _XmCountVaList(var, &button_count, &args_count, &typed_count, &total_count);
    va_end(var);

/* Convert into a normal ArgList */
    Va_start(var,parent);

    num_args = args_count + (XMBUTTONS_ARGS_PER_LIST + CB_EXTRA_ARGS);

    _XmVaProcessEverything(parent, var, &checkBoxButtonTypes,
		&checkBoxStrings, &checkBoxMnemonics, 
		&checkBoxAccelerators, &checkBoxAcceleratorText,
		button_count, &args, num_args);

    n = args_count;
    XtSetArg (args[n], XmNsimpleCallback, callback); n++;
    XtSetArg (args[n], XmNbuttonCount, button_count); n++;
    XtSetArg (args[n], XmNbuttonType, checkBoxButtonTypes); n++;
    XtSetArg (args[n], XmNbuttons, checkBoxStrings); n++;
    XtSetArg (args[n], XmNbuttonMnemonics, checkBoxMnemonics); n++;
    XtSetArg (args[n], XmNbuttonAccelerators,
	      checkBoxAccelerators); n++;
    XtSetArg (args[n], XmNbuttonAcceleratorText,
	      checkBoxAcceleratorText); n++;

    widget = XmCreateSimpleCheckBox(parent, name, args, n);

    if (args != NULL) {
	XtFree((XtPointer)args);
    }
    if (checkBoxButtonTypes != NULL) {
	XtFree((XtPointer)checkBoxButtonTypes);
    }
    if (checkBoxStrings != NULL) {
	XtFree((XtPointer)checkBoxStrings);
    }
    if (checkBoxMnemonics != NULL) {
	XtFree((XtPointer)checkBoxMnemonics);
    }
    if (checkBoxAccelerators != NULL) {
	XtFree((XtPointer)checkBoxAccelerators);
    }
    if (checkBoxAcceleratorText != NULL) {
	XtFree((XtPointer)checkBoxAcceleratorText);
    }

    va_end(var);

    return (widget);
}
