#ifndef	_notify_user_
#define	_notify_user_

/* Module notify */

#include <mach/kern_return.h>
#if	(defined(__STDC__) || defined(c_plusplus)) || defined(LINTLIBRARY)
#include <mach/port.h>
#include <mach/message.h>
#endif

#include <mach/std_types.h>

/* SimpleRoutine mach_notify_port_deleted */
#ifdef	mig_external
mig_external
#else
extern
#endif
kern_return_t mach_notify_port_deleted
#if	defined(LINTLIBRARY)
    (notify, name)
	mach_port_t notify;
	mach_port_t name;
{ return mach_notify_port_deleted(notify, name); }
#else
#if	(defined(__STDC__) || defined(c_plusplus))
(
	mach_port_t notify,
	mach_port_t name
);
#else
    ();
#endif
#endif

/* SimpleRoutine mach_notify_msg_accepted */
#ifdef	mig_external
mig_external
#else
extern
#endif
kern_return_t mach_notify_msg_accepted
#if	defined(LINTLIBRARY)
    (notify, name)
	mach_port_t notify;
	mach_port_t name;
{ return mach_notify_msg_accepted(notify, name); }
#else
#if	(defined(__STDC__) || defined(c_plusplus))
(
	mach_port_t notify,
	mach_port_t name
);
#else
    ();
#endif
#endif

/* SimpleRoutine mach_notify_port_destroyed */
#ifdef	mig_external
mig_external
#else
extern
#endif
kern_return_t mach_notify_port_destroyed
#if	defined(LINTLIBRARY)
    (notify, rights, rightsPoly)
	mach_port_t notify;
	mach_port_t rights;
	mach_msg_type_name_t rightsPoly;
{ return mach_notify_port_destroyed(notify, rights, rightsPoly); }
#else
#if	(defined(__STDC__) || defined(c_plusplus))
(
	mach_port_t notify,
	mach_port_t rights,
	mach_msg_type_name_t rightsPoly
);
#else
    ();
#endif
#endif

/* SimpleRoutine mach_notify_no_senders */
#ifdef	mig_external
mig_external
#else
extern
#endif
kern_return_t mach_notify_no_senders
#if	defined(LINTLIBRARY)
    (notify, mscount)
	mach_port_t notify;
	mach_port_mscount_t mscount;
{ return mach_notify_no_senders(notify, mscount); }
#else
#if	(defined(__STDC__) || defined(c_plusplus))
(
	mach_port_t notify,
	mach_port_mscount_t mscount
);
#else
    ();
#endif
#endif

/* SimpleRoutine mach_notify_send_once */
#ifdef	mig_external
mig_external
#else
extern
#endif
kern_return_t mach_notify_send_once
#if	defined(LINTLIBRARY)
    (notify)
	mach_port_t notify;
{ return mach_notify_send_once(notify); }
#else
#if	(defined(__STDC__) || defined(c_plusplus))
(
	mach_port_t notify
);
#else
    ();
#endif
#endif

/* SimpleRoutine mach_notify_dead_name */
#ifdef	mig_external
mig_external
#else
extern
#endif
kern_return_t mach_notify_dead_name
#if	defined(LINTLIBRARY)
    (notify, name)
	mach_port_t notify;
	mach_port_t name;
{ return mach_notify_dead_name(notify, name); }
#else
#if	(defined(__STDC__) || defined(c_plusplus))
(
	mach_port_t notify,
	mach_port_t name
);
#else
    ();
#endif
#endif

#endif	_notify_user_
