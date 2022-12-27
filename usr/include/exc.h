#ifndef	_exc_user_
#define	_exc_user_

/* Module exc */

#include <mach/kern_return.h>
#if	(defined(__STDC__) || defined(c_plusplus)) || defined(LINTLIBRARY)
#include <mach/port.h>
#include <mach/message.h>
#endif

#include <mach/std_types.h>

/* Routine exception_raise */
#ifdef	mig_external
mig_external
#else
extern
#endif
kern_return_t exception_raise
#if	defined(LINTLIBRARY)
    (exception_port, thread, task, exception, code, subcode)
	mach_port_t exception_port;
	mach_port_t thread;
	mach_port_t task;
	int exception;
	int code;
	int subcode;
{ return exception_raise(exception_port, thread, task, exception, code, subcode); }
#else
#if	(defined(__STDC__) || defined(c_plusplus))
(
	mach_port_t exception_port,
	mach_port_t thread,
	mach_port_t task,
	int exception,
	int code,
	int subcode
);
#else
    ();
#endif
#endif

#endif	_exc_user_
