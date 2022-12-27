/*
 * $XConsortium: globals.c,v 1.7 89/06/16 16:57:00 jim Exp $
 *
 * Copyright 1989 Massachusetts Institute of Technology
 *
 *
 *                                 Global data
 *
 * This file should contain only those objects which must be predefined.
 */
#include "Xlib.h"
#include <sys/param.h>			/* for definition of NULL */


/*
 * If possible, it is useful to have the global data default to a null value.
 * Some shared library implementations are *much* happier if there isn't any
 * global initialized data.
 */
#ifdef NULL_NOT_ZERO			/* then need to initialize */
#define SetZero(t,var,z) t var = z
#else 
#define SetZero(t,var,z) t var
#endif

#ifdef ATTSHAREDLIB			/* then need extra variables */
/*
 * If we need to define extra variables for each global
 */
#if defined(__STDC__) && !defined(UNIXCPP)  /* then ANSI C concatenation */
#define ZEROINIT(t,var,val) SetZero(t,var,val); \
  SetZero (long, _libX_##var##Flag, 0); \
  SetZero (void *, _libX_##var##Ptr, NULL)
#else /* else pcc concatenation */
#define ZEROINIT(t,var,val) SetZero(t,var,val); \
  SetZero (long, _libX_/**/var/**/Flag, 0); \
  SetZero (void *, _libX_/**/var/**/Ptr, NULL)
#endif /* concat ANSI C vs. pcc */

#else /* else not ATTSHAREDLIB */
/*
 * no extra crud
 */
#define ZEROINIT(t,var,val) SetZero (t, var, val)

#endif /* ATTSHAREDLIB */


/*
 * Error handlers; used to be in XlibInt.c
 */
typedef int (*funcptr)();
ZEROINIT (funcptr, _XErrorFunction, NULL);
ZEROINIT (funcptr, _XIOErrorFunction, NULL);
ZEROINIT (_XQEvent *, _qfree, NULL);


/*
 * Debugging information and display list; used to be in XOpenDis.c
 */
ZEROINIT (int, _Xdebug, 0);
ZEROINIT (Display *, _XHeadOfDisplayList, NULL);




#ifdef STREAMSCONN
/*
 * descriptor block for streams connections
 */
#include "Xstreams.h"

char _XsTypeOfStream[100] = { 0 };

extern int SetupLocalStream();
extern int ConnectLocalClient();
extern int CallLocalServer();
extern int ReadLocalStream();
extern int ErrorCall();
extern int WriteLocalStream();
extern int CloseLocalStream(); 
extern int SetupTliStream();
extern int ConnectTliClient();
extern int CallTliServer(); 
extern int ReadTliStream(); 
extern int WriteTliStream();
extern int CloseTliStream();
extern int ErrorCall(); 

Xstream _XsStream[] = {
    { 
	SetupLocalStream,
	ConnectLocalClient,
	CallLocalServer,
	ReadLocalStream,
	ErrorCall,
	WriteLocalStream,
	CloseLocalStream,
	NULL
    },
    { 
	SetupTliStream,
	ConnectTliClient,
	CallTliServer, 
	ReadTliStream, 
	ErrorCall, 
	WriteTliStream,
	CloseTliStream,
	NULL
    },
    { 
	SetupTliStream,
	ConnectTliClient,
	CallTliServer, 
	ReadTliStream, 
	ErrorCall, 
	WriteTliStream,
	CloseTliStream,
	NULL
    },
    { 
	NULL,
	NULL, 
	NULL, 
	NULL, 
	NULL,
	NULL,
	NULL,
	NULL
    },
    { 
	NULL,
	NULL,
	NULL,
	NULL,
	NULL, 
	NULL, 
	NULL,
	NULL
    },
    { 
	NULL,
	NULL,
	NULL,
	NULL,
	NULL,
	NULL, 
	NULL, 
	NULL
    }
};
#endif /* STREAMSCONN */

#ifdef XTEST1
/*
 * Stuff for input synthesis extension:
 */
/*
 * Holds the two event type codes for this extension.  The event type codes
 * for this extension may vary depending on how many extensions are installed
 * already, so the initial values given below will be added to the base event
 * code that is aquired when this extension is installed.
 *
 * These two variables must be available to programs that use this extension.
 */
int			XTestInputActionType = 0;
int			XTestFakeAckType   = 1;
#endif

/*
 * NOTE: any additional external definition NEED
 * to be inserted BELOW this point!!!
 */

/*
 * NOTE: any additional external definition NEED
 * to be inserted ABOVE this point!!!
 */

