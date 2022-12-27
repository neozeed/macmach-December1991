#ifndef	_service_user_
#define	_service_user_

/* Module service */

#include <mach/kern_return.h>
#if	(defined(__STDC__) || defined(c_plusplus)) || defined(LINTLIBRARY)
#include <mach/port.h>
#include <mach/message.h>
#endif

#include <mach/std_types.h>

/* Routine service_checkin */
#ifdef	mig_external
mig_external
#else
extern
#endif
kern_return_t service_checkin
#if	defined(LINTLIBRARY)
    (service_request, service_desired, service_granted)
	mach_port_t service_request;
	mach_port_t service_desired;
	mach_port_t *service_granted;
{ return service_checkin(service_request, service_desired, service_granted); }
#else
#if	(defined(__STDC__) || defined(c_plusplus))
(
	mach_port_t service_request,
	mach_port_t service_desired,
	mach_port_t *service_granted
);
#else
    ();
#endif
#endif

/* Routine service_waitfor */
#ifdef	mig_external
mig_external
#else
extern
#endif
kern_return_t service_waitfor
#if	defined(LINTLIBRARY)
    (service_request, service_desired)
	mach_port_t service_request;
	mach_port_t service_desired;
{ return service_waitfor(service_request, service_desired); }
#else
#if	(defined(__STDC__) || defined(c_plusplus))
(
	mach_port_t service_request,
	mach_port_t service_desired
);
#else
    ();
#endif
#endif

#endif	_service_user_
