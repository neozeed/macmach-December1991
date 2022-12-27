#ifndef	_km
#define	_km

/* Module km */

#include <mach/kern_return.h>
#if	(defined(__STDC__) || defined(c_plusplus)) || defined(LINTLIBRARY)
#include <mach/port.h>
#include <mach/message.h>
#endif

#ifndef	mig_external
#define mig_external extern
#endif

mig_external void init_km
#if	(defined(__STDC__) || defined(c_plusplus))
    (port_t rep_port);
#else
    ();
#endif
#include "key_defs.h"
#include "nm_defs.h"

/* Routine km_dummy */
mig_external kern_return_t km_dummy
#if	defined(LINTLIBRARY)
    (server_port)
	port_t server_port;
{ return km_dummy(server_port); }
#else
#if	(defined(__STDC__) || defined(c_plusplus))
(
	port_t server_port
);
#else
    ();
#endif
#endif

#endif	_km
