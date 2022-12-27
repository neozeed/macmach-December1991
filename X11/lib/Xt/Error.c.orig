#ifndef lint
static char Xrcsid[] = "$XConsortium: Error.c,v 1.23 89/12/15 17:33:30 swick Exp $";
/* $oHeader: Error.c,v 1.6 88/08/31 17:46:14 asente Exp $ */
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

#include <X11/Xlib.h>
#include <stdio.h>
#include "IntrinsicI.h"

/* The error handlers in the application context aren't used since we can't
   come up with a uniform way of using them.  If you can, define
   GLOBALERRORS to be FALSE (or 0). */

#ifndef GLOBALERRORS
#define GLOBALERRORS 1
#endif

#if GLOBALERRORS
static XrmDatabase errorDB = NULL;
static Boolean error_inited = FALSE;
static void _XtDefaultErrorMsg(), _XtDefaultWarningMsg(), 
	_XtDefaultError(), _XtDefaultWarning();
static XtErrorMsgHandler errorMsgHandler = _XtDefaultErrorMsg;
static XtErrorMsgHandler warningMsgHandler = _XtDefaultWarningMsg;
static XtErrorHandler errorHandler = _XtDefaultError;
static XtErrorMsgHandler warningHandler = _XtDefaultWarning;
#endif /* GLOBALERRORS */

XrmDatabase *XtGetErrorDatabase()
{
#if GLOBALERRORS
    return &errorDB;
#else
    return XtAppGetErrorDatabase(_XtDefaultAppContext());
#endif /* GLOBALERRORS */
}

XrmDatabase *XtAppGetErrorDatabase(app)
	XtAppContext app;
{
#if GLOBALERRORS
	return &errorDB;
#else
	return &app->errorDB;
#endif /* GLOBALERRORS */
}

void XtGetErrorDatabaseText(name,type,class,defaultp, buffer, nbytes)
    register String name, type, class;
    String defaultp;
    String buffer;
    int nbytes;
{
#if GLOBALERRORS
    XtAppGetErrorDatabaseText(NULL,
	    name,type,class,defaultp, buffer, nbytes, NULL);
#else
    XtAppGetErrorDatabaseText(_XtDefaultAppContext(),
	    name,type,class,defaultp, buffer, nbytes, NULL);
#endif /* GLOBALERRORS */
}

void XtAppGetErrorDatabaseText(app, name,type,class,defaultp,
	buffer, nbytes, db)
    XtAppContext app;
    register String name, type, class;
    String defaultp;
    String buffer;
    int nbytes;
    XrmDatabase db;
{
    String type_str;
    XrmValue result;
    char temp[BUFSIZ];

#if GLOBALERRORS
    if (error_inited == FALSE) {
        _XtInitErrorHandling (&errorDB);
        error_inited = TRUE;
    }
#else
    if (app->error_inited == FALSE) {
        _XtInitErrorHandling (&app->errorDB);
        app->error_inited = TRUE;
    }
#endif /* GLOBALERRORS */
    (void) sprintf(temp, "%s.%s", name, type);
    if (db == NULL) {
#if GLOBALERRORS
	(void) XrmGetResource(errorDB, temp, class, &type_str, &result);
#else
	(void) XrmGetResource(app->errorDB, temp, class, &type_str, &result);
#endif /* GLOBALERRORS */
    } else (void) XrmGetResource(db, temp, class, &type_str, &result);
    if (result.addr) {
        (void) strncpy (buffer, result.addr, nbytes);
        if (result.size < nbytes) buffer[result.size] = 0;
    } else (void) strncpy(buffer, defaultp, nbytes);
}

_XtInitErrorHandling (db)
    XrmDatabase *db;
{
    XrmDatabase errordb;

    errordb = XrmGetFileDatabase(ERRORDB);
    XrmMergeDatabases(errordb, db);
}

static void _XtDefaultErrorMsg (name,type,class,defaultp,params,num_params)
    String name,type,class,defaultp;
    String* params;
    Cardinal* num_params;
{
    char buffer[1000], message[1000];
    XtGetErrorDatabaseText(name,type,class,defaultp, buffer, 1000);
/*need better solution here, perhaps use lower level printf primitives? */
    if (params == NULL || num_params == NULL || *num_params == 0)
	XtError(buffer);
    else {
	int i = *num_params;
	String par[10];
	if (i > 10) i = 10;
	bcopy( (char*)params, (char*)par, i * sizeof(String) );
	bzero( &par[i], (10-i) * sizeof(String) );
        (void) sprintf(message, buffer, par[0], par[1], par[2], par[3],
		       par[4], par[5], par[6], par[7], par[8], par[9]);
	XtError(message);
	if (i != *num_params)
	    XtWarning( "some arguments in previous message were lost" );
    }
}

static void _XtDefaultWarningMsg (name,type,class,defaultp,params,num_params)
    String name,type,class,defaultp;
    String* params;
    Cardinal* num_params;
{

    char buffer[1000], message[1000];
    XtGetErrorDatabaseText(name,type,class,defaultp, buffer, 1000);
/*need better solution here*/
    if (params == NULL || num_params == NULL || *num_params == 0)
	XtWarning(buffer);
    else {
	int i = *num_params;
	String par[10];
	if (i > 10) i = 10;
	bcopy( (char*)params, (char*)par, i * sizeof(String) );
	bzero( &par[i], (10-i) * sizeof(String) );
        (void) sprintf(message, buffer, par[0], par[1], par[2], par[3],
		       par[4], par[5], par[6], par[7], par[8], par[9]);
	XtWarning(message); 
	if (i != *num_params)
	    XtWarning( "some arguments in previous message were lost" );
   }
}

void XtErrorMsg(name,type,class,defaultp,params,num_params)
    String name,type,class,defaultp;
    String* params;
    Cardinal* num_params;
{
#if GLOBALERRORS
    (*errorMsgHandler)(name,type,class,defaultp,params,num_params);
#else
    XtAppErrorMsg(_XtDefaultAppContext(),name,type,class,
	    defaultp,params,num_params);
#endif /* GLOBALERRORS */
}

void XtAppErrorMsg(app, name,type,class,defaultp,params,num_params)
    XtAppContext app;
    String name,type,class,defaultp;
    String* params;
    Cardinal* num_params;
{
#if GLOBALERRORS
    (*errorMsgHandler)(name,type,class,defaultp,params,num_params);
#else
    (*app->errorMsgHandler)(name,type,class,defaultp,params,num_params);
#endif /* GLOBALERRORS */
}

void XtWarningMsg(name,type,class,defaultp,params,num_params)
    String name,type,class,defaultp;
    String* params;
    Cardinal* num_params;
{
#if GLOBALERRORS
    (*warningMsgHandler)(name,type,class,defaultp,params,num_params);
#else
    XtAppWarningMsg(_XtDefaultAppContext(),name,type,class,
	    defaultp,params,num_params);
#endif /* GLOBALERRORS */
}

void XtAppWarningMsg(app,name,type,class,defaultp,params,num_params)
    XtAppContext app;
    String name,type,class,defaultp;
    String* params;
    Cardinal* num_params;
{
#if GLOBALERRORS
    (*warningMsgHandler)(name,type,class,defaultp,params,num_params);
#else
    (*app->warningMsgHandler)(name,type,class,defaultp,params,num_params);
#endif /* GLOBALERRORS */
}

void XtSetErrorMsgHandler(handler)
    XtErrorMsgHandler handler;
{
#if GLOBALERRORS
    if (handler != NULL) errorMsgHandler = handler;
    else errorMsgHandler  = _XtDefaultErrorMsg;
#else
    XtAppSetErrorMsgHandler(_XtDefaultAppContext(), handler);
#endif /* GLOBALERRORS */
}

XtErrorMsgHandler XtAppSetErrorMsgHandler(app,handler)
    XtAppContext app;
    XtErrorMsgHandler handler;
{
    XtErrorMsgHandler old;
#if GLOBALERRORS
    old = errorMsgHandler;
    if (handler != NULL) errorMsgHandler = handler;
    else errorMsgHandler  = _XtDefaultErrorMsg;
#else
    old = app->errorMsgHandler;
    if (handler != NULL) app->errorMsgHandler = handler;
    else app->errorMsgHandler  = _XtDefaultErrorMsg;
#endif /* GLOBALERRORS */
    return old;
}

void XtSetWarningMsgHandler(handler)
    XtErrorMsgHandler handler;
{
#if GLOBALERRORS
    if (handler != NULL) warningMsgHandler  = handler;
    else warningMsgHandler = _XtDefaultWarningMsg;
#else
    XtAppSetWarningMsgHandler(_XtDefaultAppContext(),handler);
#endif /* GLOBALERRORS */
}

XtErrorMsgHandler XtAppSetWarningMsgHandler(app,handler)
    XtAppContext app;
    XtErrorMsgHandler handler;
{
    XtErrorMsgHandler old;
#if GLOBALERRORS
    old = warningMsgHandler;
    if (handler != NULL) warningMsgHandler  = handler;
    else warningMsgHandler = _XtDefaultWarningMsg;
#else
    old = app->warningMsgHandler;
    if (handler != NULL) app->warningMsgHandler  = handler;
    else app->warningMsgHandler = _XtDefaultWarningMsg;
#endif /* GLOBALERRORS */
    return old;
}

static void _XtDefaultError(message)
    String message;
{
    extern void exit();

    (void)fprintf(stderr, "%sError: %s\n", XTERROR_PREFIX, message);
    exit(1);
}

static void _XtDefaultWarning(message)
    String message;
{
       (void)fprintf(stderr, "%sWarning: %s\n", XTWARNING_PREFIX, message); 
    return;
}

void XtError(message)
    String message;
{
#if GLOBALERRORS
    (*errorHandler)(message);
#else
    XtAppError(_XtDefaultAppContext(),message);
#endif /* GLOBALERRORS */
}

void XtAppError(app,message)
    XtAppContext app;
    String message;
{
#if GLOBALERRORS
    (*errorHandler)(message);
#else
    (*app->errorHandler)(message);
#endif /* GLOBALERRORS */
}

void XtWarning(message)
    String message;
{
#if GLOBALERRORS
    (*warningHandler)(message);
#else
    XtAppWarning(_XtDefaultAppContext(),message);
#endif /* GLOBALERRORS */
}

void XtAppWarning(app,message)
    XtAppContext app;
    String message;
{
#if GLOBALERRORS
    (*warningHandler)(message);
#else
    (*app->warningHandler)(message);
#endif /* GLOBALERRORS */
}

void XtSetErrorHandler(handler)
    XtErrorHandler handler;
{
#if GLOBALERRORS
    if (handler != NULL) errorHandler = handler;
    else errorHandler  = _XtDefaultError;
#else
    XtAppSetErrorHandler(_XtDefaultAppContext(),handler);
#endif /* GLOBALERRORS */
}

XtErrorHandler XtAppSetErrorHandler(app,handler)
    XtAppContext app;
    XtErrorHandler handler;
{
    XtErrorHandler old;
#if GLOBALERRORS
    old = errorHandler;
    if (handler != NULL) errorHandler = handler;
    else errorHandler  = _XtDefaultError;
#else
    old = app->errorHandler;
    if (handler != NULL) app->errorHandler = handler;
    else app->errorHandler  = _XtDefaultError;
#endif /* GLOBALERRORS */
    return old;
}

void XtSetWarningHandler(handler)
    XtErrorHandler handler;
{
#if GLOBALERRORS
    if (handler != NULL) warningHandler = handler;
    else warningHandler = _XtDefaultWarning;
#else
    XtAppSetWarningHandler(_XtDefaultAppContext(),handler);
#endif /* GLOBALERRORS */
}

XtErrorHandler XtAppSetWarningHandler(app,handler)
    XtAppContext app;
    XtErrorHandler handler;
{
    XtErrorHandler old;
#if GLOBALERRORS
    old = warningHandler;
    if (handler != NULL) warningHandler  = handler;
    else warningHandler = _XtDefaultWarning;
#else
    old = app->warningHandler;
    if (handler != NULL) app->warningHandler  = handler;
    else app->warningHandler = _XtDefaultWarning;
#endif /* GLOBALERRORS */
    return old;
}

void _XtSetDefaultErrorHandlers(errMsg, warnMsg, err, warn)
	XtErrorMsgHandler *errMsg, *warnMsg;
	XtErrorHandler *err, *warn;
{
#ifndef GLOBALERRORS
	*errMsg = _XtDefaultErrorMsg;
	*warnMsg = _XtDefaultWarningMsg;
	*err = _XtDefaultError;
	*warn = _XtDefaultWarning;
#endif /* GLOBALERRORS */
}
