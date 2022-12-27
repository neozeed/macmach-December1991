/* $XConsortium: CallbackI.h,v 1.8 89/09/12 16:48:17 swick Exp $ */
/* $oHeader: CallbackI.h,v 1.2 88/08/18 15:53:53 asente Exp $ */
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

/****************************************************************
 *
 * Callbacks
 *
 ****************************************************************/

typedef struct _XtOffsetRec {
     struct _XtOffsetRec *next;
     XrmQuark       name;
     int            offset;
} XtOffsetRec, *_XtOffsetList;

extern void _XtRemoveAllCallbacks (); /* callbackList */
    /* CallbackList     *callbackList;  */

extern void _XtCallCallbacks (); /* callbacks, call_data */
    /* CallbackList     *callbacks;     */
    /* XtPointer        callData;       */


extern void _XtAddCallback(); /* widget, callbacks, callback, closure */
    /* Widget           widget; */
    /* CallbackList     *callbacks; */
    /* XtCallbackProc   callback; */
    /* XtPointer        closure; */

typedef struct _CallbackRec *CallbackList;

typedef struct _CallbackStruct CallbackStruct;

extern CallbackList *_XtDestroyList;

#define _XtSafeToDestroy (_XtDestroyList == NULL)
