/* $XConsortium: Xlibint.h,v 11.79 89/12/12 12:13:42 jim Exp $ */
/* Copyright 1984, 1985, 1987, 1989  Massachusetts Institute of Technology */

/*
 *	XlibInternal.h - Header definition and support file for the internal
 *	support routines (XlibInternal) used by the C subroutine interface
 *	library (Xlib) to the X Window System.
 *
 *	Warning, there be dragons here....
 */

#include <X11/copyright.h>

#ifndef NEED_EVENTS
#define _XEVENT_
#endif

#ifdef USG
#ifndef __TYPES__
#include <sys/types.h>			/* forgot to protect it... */
#define __TYPES__
#endif /* __TYPES__ */
#else
#include <sys/types.h>
#endif /* USG */

/*
 * define the following if you want the Data macro to be a procedure instead
 */
#if defined(CRAY)
#define DataRoutineIsProcedure
#endif /* CRAY */

#include <X11/Xlib.h>		/* get NeedFunctionPrototypes defs */

#if NeedFunctionPrototypes	/* prototypes require event type definitions */
#define NEED_EVENTS
#endif
#include <X11/Xproto.h>
#include <errno.h>
#include <X11/Xlibos.h>

#ifdef __cplusplus			/* do not leave open across includes */
extern "C" {					/* for C++ V2.0 */
#endif

#ifndef NULL
#define NULL 0
#endif
#define LOCKED 1
#define UNLOCKED 0

extern int errno;			/* Internal system error number. */
extern void bcopy();

extern int _XError();			/* prepare to upcall user handler */
extern int _XIOError();			/* prepare to upcall user handler */
extern int (*_XIOErrorFunction)();	/* X system error reporting routine. */
extern int (*_XErrorFunction)();	/* X_Error event reporting routine. */
extern void _XEatData();		/* swallow data from server */
extern char *_XAllocScratch();		/* fast memory allocator */
extern Visual *_XVIDtoVisual();		/* given visual id, find structure */
extern unsigned long _XSetLastRequestRead();	/* update dpy->last_request_read */
extern int _XGetHostname();		/* get name of this machine */
extern Screen *_XScreenOfWindow ();	/* get Screen pointer for window */

#ifndef BUFSIZE
#define BUFSIZE 2048			/* X output buffer size. */
#endif
#ifndef EPERBATCH
#define EPERBATCH 8			/* when batching, how many elements */
#endif
#ifndef CURSORFONT
#define CURSORFONT "cursor"		/* standard cursor fonts */
#endif

/*
 * display flags
 */
#define XlibDisplayIOError	(1L << 0)
#define XlibDisplayClosing	(1L << 1)

/*
 * X Protocol packetizing macros.
 */

/*   Need to start requests on 64 bit word boundries
 *   on a CRAY computer so add a NoOp (127) if needed.
 *   A character pointer on a CRAY computer will be non-zero
 *   after shifting right 61 bits of it is not pointing to
 *   a word boundary.
 */
#ifdef WORD64
#define WORD64ALIGN if ((long)dpy->bufptr >> 61) {\
           dpy->last_req = dpy->bufptr;\
           *(dpy->bufptr)   = X_NoOperation;\
           *(dpy->bufptr+1) =  0;\
           *(dpy->bufptr+2) =  0;\
           *(dpy->bufptr+3) =  1;\
             dpy->request += 1;\
             dpy->bufptr += 4;\
         }
#else /* else does not require alignment on 64-bit boundaries */
#define WORD64ALIGN
#endif /* WORD64 */


/*
 * GetReq - Get the next avilable X request packet in the buffer and
 * return it. 
 *
 * "name" is the name of the request, e.g. CreatePixmap, OpenFont, etc.
 * "req" is the name of the request pointer.
 *
 */

#if defined(__STDC__) && !defined(UNIXCPP)
#define GetReq(name, req) \
        WORD64ALIGN\
	if ((dpy->bufptr + SIZEOF(x##name##Req)) > dpy->bufmax)\
		_XFlush(dpy);\
	req = (x##name##Req *)(dpy->last_req = dpy->bufptr);\
	req->reqType = X_##name;\
	req->length = (SIZEOF(x##name##Req))>>2;\
	dpy->bufptr += SIZEOF(x##name##Req);\
	dpy->request++

#else  /* non-ANSI C uses empty comment instead of "##" for token concatenation */
#define GetReq(name, req) \
        WORD64ALIGN\
	if ((dpy->bufptr + SIZEOF(x/**/name/**/Req)) > dpy->bufmax)\
		_XFlush(dpy);\
	req = (x/**/name/**/Req *)(dpy->last_req = dpy->bufptr);\
	req->reqType = X_/**/name;\
	req->length = (SIZEOF(x/**/name/**/Req))>>2;\
	dpy->bufptr += SIZEOF(x/**/name/**/Req);\
	dpy->request++
#endif

/* GetReqExtra is the same as GetReq, but allocates "n" additional
   bytes after the request. "n" must be a multiple of 4!  */

#if defined(__STDC__) && !defined(UNIXCPP)
#define GetReqExtra(name, n, req) \
        WORD64ALIGN\
	if ((dpy->bufptr + SIZEOF(x##name##Req) + n) > dpy->bufmax)\
		_XFlush(dpy);\
	req = (x##name##Req *)(dpy->last_req = dpy->bufptr);\
	req->reqType = X_##name;\
	req->length = (SIZEOF(x##name##Req) + n)>>2;\
	dpy->bufptr += SIZEOF(x##name##Req) + n;\
	dpy->request++
#else
#define GetReqExtra(name, n, req) \
        WORD64ALIGN\
	if ((dpy->bufptr + SIZEOF(x/**/name/**/Req) + n) > dpy->bufmax)\
		_XFlush(dpy);\
	req = (x/**/name/**/Req *)(dpy->last_req = dpy->bufptr);\
	req->reqType = X_/**/name;\
	req->length = (SIZEOF(x/**/name/**/Req) + n)>>2;\
	dpy->bufptr += SIZEOF(x/**/name/**/Req) + n;\
	dpy->request++
#endif


/*
 * GetResReq is for those requests that have a resource ID 
 * (Window, Pixmap, GContext, etc.) as their single argument.
 * "rid" is the name of the resource. 
 */

#if defined(__STDC__) && !defined(UNIXCPP)
#define GetResReq(name, rid, req) \
        WORD64ALIGN\
	if ((dpy->bufptr + SIZEOF(xResourceReq)) > dpy->bufmax)\
	    _XFlush(dpy);\
	req = (xResourceReq *) (dpy->last_req = dpy->bufptr);\
	req->reqType = X_##name;\
	req->length = 2;\
	req->id = (rid);\
	dpy->bufptr += SIZEOF(xResourceReq);\
	dpy->request++
#else
#define GetResReq(name, rid, req) \
        WORD64ALIGN\
	if ((dpy->bufptr + SIZEOF(xResourceReq)) > dpy->bufmax)\
	    _XFlush(dpy);\
	req = (xResourceReq *) (dpy->last_req = dpy->bufptr);\
	req->reqType = X_/**/name;\
	req->length = 2;\
	req->id = (rid);\
	dpy->bufptr += SIZEOF(xResourceReq);\
	dpy->request++
#endif

/*
 * GetEmptyReq is for those requests that have no arguments
 * at all. 
 */
#if defined(__STDC__) && !defined(UNIXCPP)
#define GetEmptyReq(name, req) \
        WORD64ALIGN\
	if ((dpy->bufptr + SIZEOF(xReq)) > dpy->bufmax)\
	    _XFlush(dpy);\
	req = (xReq *) (dpy->last_req = dpy->bufptr);\
	req->reqType = X_##name;\
	req->length = 1;\
	dpy->bufptr += SIZEOF(xReq);\
	dpy->request++
#else
#define GetEmptyReq(name, req) \
        WORD64ALIGN\
	if ((dpy->bufptr + SIZEOF(xReq)) > dpy->bufmax)\
	    _XFlush(dpy);\
	req = (xReq *) (dpy->last_req = dpy->bufptr);\
	req->reqType = X_/**/name;\
	req->length = 1;\
	dpy->bufptr += SIZEOF(xReq);\
	dpy->request++
#endif


#define SyncHandle() \
	if (dpy->synchandler) (*dpy->synchandler)(dpy)

#define FlushGC(dpy, gc) \
	if ((gc)->dirty) _XFlushGCCache((dpy), (gc))
/*
 * Data - Place data in the buffer and pad the end to provide
 * 32 bit word alignment.  Transmit if the buffer fills.
 *
 * "dpy" is a pointer to a Display.
 * "data" is a pinter to a data buffer.
 * "len" is the length of the data buffer.
 * we can presume buffer less than 2^16 bytes, so bcopy can be used safely.
 */
#ifdef DataRoutineIsProcedure
extern void Data();
#else
#define Data(dpy, data, len) \
	if (dpy->bufptr + (len) <= dpy->bufmax) {\
		bcopy(data, dpy->bufptr, (int)len);\
		dpy->bufptr += ((len) + 3) & ~3;\
	} else\
		_XSend(dpy, data, len)
#endif /* DataRoutineIsProcedure */


/* Allocate bytes from the buffer.  No padding is done, so if
 * the length is not a multiple of 4, the caller must be
 * careful to leave the buffer aligned after sending the
 * current request.
 *
 * "type" is the type of the pointer being assigned to.
 * "ptr" is the pointer being assigned to.
 * "n" is the number of bytes to allocate.
 *
 * Example: 
 *    xTextElt *elt;
 *    BufAlloc (xTextElt *, elt, nbytes)
 */

#define BufAlloc(type, ptr, n) \
    if (dpy->bufptr + (n) > dpy->bufmax) \
        _XFlush (dpy); \
    ptr = (type) dpy->bufptr; \
    dpy->bufptr += (n);

/*
 * provide emulation routines for smaller architectures
 */
#ifndef WORD64
#define Data16(dpy, data, len) Data((dpy), (char *)(data), (len))
#define Data32(dpy, data, len) Data((dpy), (char *)(data), (len))
#define _XRead16Pad(dpy, data, len) _XReadPad((dpy), (char *)(data), (len))
#define _XRead16(dpy, data, len) _XRead((dpy), (char *)(data), (len))
#define _XRead32(dpy, data, len) _XRead((dpy), (char *)(data), (len))
#endif /* not WORD64 */

#define PackData16(dpy,data,len) Data16 (dpy, data, len)
#define PackData32(dpy,data,len) Data32 (dpy, data, len)

/* Xlib manual is bogus */
#define PackData(dpy,data,len) PackData16 (dpy, data, len)

#define min(a,b) (((a) < (b)) ? (a) : (b))
#define max(a,b) (((a) > (b)) ? (a) : (b))

#define CI_NONEXISTCHAR(cs) (((cs)->width == 0) && \
			     ((cs)->rbearing == 0) && \
			     ((cs)->lbearing == 0))

/* 
 * CI_GET_CHAR_INFO_1D - return the charinfo struct for the indicated 8bit
 * character.  If the character is in the column and exists, then return the
 * appropriate metrics (note that fonts with common per-character metrics will
 * return min_bounds).  If none of these hold true, try again with the default
 * char.
 */
#define CI_GET_CHAR_INFO_1D(fs,col,def,cs) \
{ \
    cs = def; \
    if (col >= fs->min_char_or_byte2 && col <= fs->max_char_or_byte2) { \
	if (fs->per_char == NULL) { \
	    cs = &fs->min_bounds; \
	} else { \
	    cs = &fs->per_char[(col - fs->min_char_or_byte2)]; \
	    if (CI_NONEXISTCHAR(cs)) cs = def; \
	} \
    } \
}

#define CI_GET_DEFAULT_INFO_1D(fs,cs) \
  CI_GET_CHAR_INFO_1D (fs, fs->default_char, NULL, cs)



/*
 * CI_GET_CHAR_INFO_2D - return the charinfo struct for the indicated row and 
 * column.  This is used for fonts that have more than row zero.
 */
#define CI_GET_CHAR_INFO_2D(fs,row,col,def,cs) \
{ \
    cs = def; \
    if (row >= fs->min_byte1 && row <= fs->max_byte1 && \
	col >= fs->min_char_or_byte2 && col <= fs->max_char_or_byte2) { \
	if (fs->per_char == NULL) { \
	    cs = &fs->min_bounds; \
	} else { \
	    cs = &fs->per_char[((row - fs->min_byte1) * \
			        (fs->max_char_or_byte2 - \
				 fs->min_char_or_byte2 + 1)) + \
			       (col - fs->min_char_or_byte2)]; \
	    if (CI_NONEXISTCHAR(cs)) cs = def; \
        } \
    } \
}

#define CI_GET_DEFAULT_INFO_2D(fs,cs) \
{ \
    unsigned int r = (fs->default_char >> 8); \
    unsigned int c = (fs->default_char & 0xff); \
    CI_GET_CHAR_INFO_2D (fs, r, c, NULL, cs); \
}





#ifdef MUSTCOPY

/* a little bit of magic */
#define OneDataCard32(dpy,dstaddr,srcvar) \
  { dpy->bufptr -= 4; Data32 (dpy, (char *) &(srcvar), 4); }

#define STARTITERATE(tpvar,type,start,endcond,decr) \
  { register char *cpvar; \
  for (cpvar = (char *) start; endcond; cpvar = NEXTPTR(cpvar,type), decr) { \
    type dummy; bcopy (cpvar, (char *) &dummy, SIZEOF(type)); \
    tpvar = (type *) cpvar;
#define ENDITERATE }}

#else

/* srcvar must be a variable for large architecture version */
#define OneDataCard32(dpy,dstaddr,srcvar) \
  { *(unsigned long *)(dstaddr) = (srcvar); }

#define STARTITERATE(tpvar,type,start,endcond,decr) \
  for (tpvar = (type *) start; endcond; tpvar++, decr) {
#define ENDITERATE }

#endif /* MUSTCOPY - used machines whose C structs don't line up with proto */


/* extension hooks */


extern int (*XESetCreateGC(
#if NeedFunctionPrototypes
    Display*		/* display */,
    int			/* extension */,
    int (*) ( Display*			/* display */,
	      GC			/* gc */,
	      XExtCodes*		/* codes */
	    )		/* proc */
#endif
))(
#if NeedFunctionPrototypes
    Display*, GC, XExtCodes*
#endif
);

extern int (*XESetCopyGC(
#if NeedFunctionPrototypes
    Display*		/* display */,
    int			/* extension */,
    int (*) ( Display*			/* display */,
              GC			/* gc */,
              XExtCodes*		/* codes */
            )		/* proc */	      
#endif
))(
#if NeedFunctionPrototypes
    Display*, GC, XExtCodes*
#endif
);

extern int (*XESetFlushGC(
#if NeedFunctionPrototypes
    Display*		/* display */,
    int			/* extenstion */,
    int (*) ( Display*			/* display */,
              GC			/* gc */,
              XExtCodes*		/* codes */
            )		/* proc */	     
#endif
))(
#if NeedFunctionPrototypes
    Display*, GC, XExtCodes*
#endif
);

extern int (*XESetFreeGC(
#if NeedFunctionPrototypes
    Display*		/* display */,
    int			/* extension */,
    int (*) ( Display*			/* display */,
              GC			/* gc */,
              XExtCodes*		/* codes */
            )		/* proc */	     
#endif
))(
#if NeedFunctionPrototypes
    Display*, GC, XExtCodes*
#endif
);

extern int (*XESetCreateFont(
#if NeedFunctionPrototypes
    Display*		/* display */,
    int			/* extension */,
    int (*) ( Display*			/* display */,
              XFontStruct*		/* fs */,
              XExtCodes*		/* codes */
            )		/* proc */    
#endif
))(
#if NeedFunctionPrototypes
    Display*, XFontStruct*, XExtCodes*
#endif
);

extern int (*XESetFreeFont(
#if NeedFunctionPrototypes
    Display*		/* display */,
    int			/* extension */,
    int (*) ( Display*			/* display */,
              XFontStruct*		/* fs */,
              XExtCodes*		/* codes */
            )		/* proc */    
#endif
))(
#if NeedFunctionPrototypes
    Display*, XFontStruct*, XExtCodes*
#endif
); 

extern int (*XESetCloseDisplay(
#if NeedFunctionPrototypes
    Display*		/* display */,
    int			/* extension */,
    int (*) ( Display*			/* display */,
              XExtCodes*		/* codes */
            )		/* proc */    
#endif
))(
#if NeedFunctionPrototypes
    Display*, XExtCodes*
#endif
);

extern int (*XESetError(
#if NeedFunctionPrototypes
    Display*		/* display */,
    int			/* extension */,
    int (*) ( Display*			/* display */,
              xError*			/* err */,
              XExtCodes*		/* codes */,
              int*			/* ret_code */
            )		/* proc */    
#endif
))(
#if NeedFunctionPrototypes
    Display*, xError*, XExtCodes*, int*
#endif
);

extern char* (*XESetErrorString(
#if NeedFunctionPrototypes
    Display*		/* display */,
    int			/* extension */,
    char* (*) ( Display*		/* display */,
                int			/* code */,
                XExtCodes*		/* codes */,
                char*			/* buffer */,
                int			/* nbytes */
              )		/* proc */	       
#endif
))(
#if NeedFunctionPrototypes
    Display*, int, XExtCodes*, char*, int
#endif
);

extern int (*XESetWireToEvent(
#if NeedFunctionPrototypes
    Display*		/* display */,
    int			/* event_number */,
    Bool (*) ( Display*			/* display */,
               XEvent*			/* re */,
               xEvent*			/* event */
             )		/* proc */    
#endif
))(
#if NeedFunctionPrototypes
    Display*, XEvent*, xEvent*
#endif
);

extern Status (*XESetEventToWire(
#if NeedFunctionPrototypes
    Display*		/* display */,
    int			/* event_number */,
    int (*) ( Display*			/* display */,
              XEvent*			/* re */,
              xEvent*			/* event */
            )		/* proc */   
#endif
))(
#if NeedFunctionPrototypes
    Display*, XEvent*, xEvent*
#endif
);

#ifdef __cplusplus
}						/* for C++ V2.0 */
#endif


