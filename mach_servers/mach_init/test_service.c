/*
 * Mach Operating System
 * Copyright (c) 1991,1990 Carnegie Mellon University
 * All Rights Reserved.
 * 
 * Permission to use, copy, modify and distribute this software and its
 * documentation is hereby granted, provided that both the copyright
 * notice and this permission notice appear in all copies of the
 * software, derivative works or modified versions, and any portions
 * thereof, and that both notices appear in supporting documentation.
 * 
 * CARNEGIE MELLON ALLOWS FREE USE OF THIS SOFTWARE IN ITS 
 * CONDITION.  CARNEGIE MELLON DISCLAIMS ANY LIABILITY OF ANY KIND FOR
 * ANY DAMAGES WHATSOEVER RESULTING FROM THE USE OF THIS SOFTWARE.
 * 
 * Carnegie Mellon requests users of this software to return to
 * 
 *  Software Distribution Coordinator  or  Software.Distribution@CS.CMU.EDU
 *  School of Computer Science
 *  Carnegie Mellon University
 *  Pittsburgh PA 15213-3890
 * 
 * any improvements or extensions that they make and grant Carnegie the
 * rights to redistribute these changes.
 */
/*
 * HISTORY
 * $Log:	test_service.c,v $
 * Revision 2.4  91/03/27  17:36:08  mrt
 * 	Changed mach.h include
 * 
 * Revision 2.3  91/03/19  11:27:10  mrt
 * 	Changed to new copyright
 * 
 * Revision 2.2  90/09/12  16:35:14  rpd
 * 	Created.
 * 	[90/09/11            rpd]
 * 
 */

#include <stdio.h>
#include <mach.h>
#include <mach/message.h>
#include <mach/notify.h>

static void send_bad_message();

static void
usage()
{
    quit(1, "usage: test_service slot-num test-num\n");
}

main(argc, argv)
    int argc;
    char *argv[];
{
    mach_port_t acquired;
    mach_port_t *ports;
    unsigned int portsCnt;
    unsigned int slot;
    kern_return_t kr;

    if (argc != 3)
	usage();
    slot = atoi(argv[1]);

    kr = mach_ports_lookup(mach_task_self(), &ports, &portsCnt);
    if (kr != KERN_SUCCESS)
	quit(1, "test_service: mach_ports_lookup: %s\n",
	     mach_error_string(kr));

    if (slot >= portsCnt)
	quit(1, "test_service: only %d slots\n", portsCnt);

    switch (atoi(argv[2])) {
      case 0:
	kr = service_checkin(service_port, ports[slot], &acquired);
	if (kr != KERN_SUCCESS)
	    quit(1, "test_service: service_checkin: %s\n",
		 mach_error_string(kr));

	printf("Acquired port %x, slot %d.\n", acquired, slot);
	pause();
	break;

      case 1:
	send_bad_message(service_port, ports[slot]);
	break;

      case 2: {
	mach_port_t previous;

	kr = service_checkin(service_port, ports[slot], &acquired);
	if (kr != KERN_SUCCESS)
	    quit(1, "test_service: service_checkin: %s\n",
		 mach_error_string(kr));

	printf("Acquired port %x, slot %d.\n", acquired, slot);

	kr = mach_port_request_notification(mach_task_self(), acquired,
				MACH_NOTIFY_PORT_DESTROYED, 0,
				MACH_PORT_NULL, MACH_MSG_TYPE_MOVE_SEND_ONCE,
				&previous);
	if (kr != KERN_SUCCESS)
	    quit(1, "test_service: mach_port_request_notification: %s\n",
		 mach_error_string(kr));

	break;
      }

      case 3: {
	mach_port_t notify, newport;

	kr = service_checkin(service_port, ports[slot], &acquired);
	if (kr != KERN_SUCCESS)
	    quit(1, "test_service: service_checkin: %s\n",
		 mach_error_string(kr));

	kr = mach_port_allocate(mach_task_self(),
				MACH_PORT_RIGHT_RECEIVE, &newport);
	if (kr != KERN_SUCCESS)
	    quit(1, "test_service: mach_port_allocate: %s\n",
		 mach_error_string(kr));

	kr = mach_port_request_notification(mach_task_self(), acquired,
				MACH_NOTIFY_PORT_DESTROYED, 0,
				MACH_PORT_NULL, MACH_MSG_TYPE_MOVE_SEND_ONCE,
				&notify);
	if (kr != KERN_SUCCESS)
	    quit(1, "test_service: mach_port_request_notification: %s\n",
		 mach_error_string(kr));

	kr = mach_notify_port_destroyed(notify, newport,
					MACH_MSG_TYPE_MOVE_RECEIVE);
	if (kr != KERN_SUCCESS)
	    quit(1, "test_service: mach_notify_port_destroyed: %s\n",
		 mach_error_string(kr));

	break;
      }

      case 4: {
	mach_port_t notify;

	kr = service_checkin(service_port, ports[slot], &acquired);
	if (kr != KERN_SUCCESS)
	    quit(1, "test_service: service_checkin: %s\n",
		 mach_error_string(kr));

	kr = mach_port_request_notification(mach_task_self(), acquired,
				MACH_NOTIFY_PORT_DESTROYED, 0,
				acquired, MACH_MSG_TYPE_MAKE_SEND_ONCE,
				&notify);
	if (kr != KERN_SUCCESS)
	    quit(1, "test_service: mach_port_request_notification: %s\n",
		 mach_error_string(kr));

	kr = mach_notify_port_destroyed(notify, acquired,
					MACH_MSG_TYPE_MOVE_RECEIVE);
	if (kr != KERN_SUCCESS)
	    quit(1, "test_service: mach_notify_port_destroyed: %s\n",
		 mach_error_string(kr));

	break;
      }

      default:
	break;
    }

    exit(0);
}

static void
send_bad_message(port, carried)
    mach_port_t port, carried;
{
    struct {
	mach_msg_header_t h;
	mach_msg_type_t t;
	vm_offset_t d;
    } msg;
    mach_msg_return_t mr;

    bzero((char *) &msg, sizeof msg);

    msg.h.msgh_bits = (MACH_MSGH_BITS(MACH_MSG_TYPE_COPY_SEND, 0) |
		       MACH_MSGH_BITS_COMPLEX);
    msg.h.msgh_remote_port = port;

    msg.t.msgt_name = MACH_MSG_TYPE_COPY_SEND;
    msg.t.msgt_number = 1;
    msg.t.msgt_size = 32;
    msg.t.msgt_longform = FALSE;
    msg.t.msgt_inline = FALSE;
    msg.t.msgt_deallocate = FALSE;

    msg.d = (vm_offset_t) &carried;

    mr = mach_msg(&msg.h, MACH_SEND_MSG, sizeof msg, 0, MACH_PORT_NULL,
		  MACH_MSG_TIMEOUT_NONE, MACH_PORT_NULL);
    if (mr != MACH_MSG_SUCCESS)
	quit(1, "test_service: mach_msg: %s\n", mach_error_string(mr));
}
