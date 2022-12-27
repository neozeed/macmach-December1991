/* $XConsortium: ConvertI.h,v 1.8 89/07/21 12:06:41 swick Exp $ */
/* $oHeader: ConvertI.h,v 1.3 88/08/31 16:11:04 asente Exp $ */
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

/**************************************************************
 * Resource conversions
 *
 **************************************************************/

typedef struct _ConverterRec **ConverterTable;

extern void _XtTableAddConverter();
    /* ConverterTable	    table;	    */
    /* XrmRepresentation    from_type;      */
    /* XrmRepresentation    to_type;        */
    /* XtConverter          converter;      */
    /* XtConvertArgList     convert_args;   */
    /* Cardinal             num_args;       */
    /* Boolean              new_style;      */
    /* XtCacheType	    cache_type;	    */
    /* XtDestructor         destructor;     */

extern Boolean _XtConvert();
    /* Widget               widget;         */
    /* XtAppContext         app;            */
    /* XrmRepresentation    from_type       */
    /* XrmValue             *from;          */
    /* XrmRepresentation    to_type;        */
    /* XrmValue             *to;            */  /* RETURN */
