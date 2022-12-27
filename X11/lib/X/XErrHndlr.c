#include "copyright.h"

/* $XConsortium: XErrHndlr.c,v 11.12 89/07/18 11:06:19 jim Exp $ */
/* Copyright    Massachusetts Institute of Technology    1986	*/

#include "Xlibint.h"

extern int _XDefaultError();
extern int _XDefaultIOError();
/* 
 * XErrorHandler - This procedure sets the X non-fatal error handler
 * (_XErrorFunction) to be the specified routine.  If NULL is passed in
 * the original error handler is restored.
 */
 
int (*XSetErrorHandler(handler))()
    register int (*handler)();
{
    int (*oldhandler)() = _XErrorFunction;

    if (handler != NULL) {
	_XErrorFunction = handler;
    }
    else {
	_XErrorFunction = _XDefaultError;
    }

    return oldhandler;
}

/* 
 * XIOErrorHandler - This procedure sets the X fatal I/O error handler
 * (_XIOErrorFunction) to be the specified routine.  If NULL is passed in 
 * the original error handler is restored.
 */
 
extern int _XIOError();
int (*XSetIOErrorHandler(handler))()
    register int (*handler)();
{
    int (*oldhandler)() = _XIOErrorFunction;

    if (handler != NULL) {
	_XIOErrorFunction = handler;
    }
    else {
	_XIOErrorFunction = _XDefaultIOError;
    }

    return oldhandler;
}
