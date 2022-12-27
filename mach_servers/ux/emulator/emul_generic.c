/* 
 * Mach Operating System
 * Copyright (c) 1989 Carnegie-Mellon University
 * All rights reserved.  The CMU software License Agreement specifies
 * the terms and conditions for use and redistribution.
 */
/*
 * HISTORY
 * $Log:	emul_generic.c,v $
 * Revision 2.5  90/06/02  15:20:25  rpd
 * 	Converted to new IPC.
 * 	[90/03/26  19:26:22  rpd]
 * 
 * Revision 2.4  89/11/29  15:26:09  af
 * 	Added rval2, because some syscalls do not modify it and it
 * 	must be preserved.
 * 	[89/11/20            af]
 * 
 * Revision 2.3  89/10/17  11:24:01  rwd
 * 	Add interrupt return parameter.  Remove special msg_rpc
 * 	redefinition.
 * 	[89/09/21            dbg]
 * 
 * Revision 2.2  89/08/09  14:35:19  rwd
 * 	Added initialization of unused field in msg_hdr.
 * 	[89/08/08            rwd]
 * 
 *
 */
#include <mach/mach.h>
#include <mach/message.h>
#include <mach/msg_type.h>
#include <uxkern/bsd_msg.h>

/*
 * Generic emulated UX call.
 */
struct bsd_request bsd_req_template = {
    {
	MACH_MSGH_BITS(MACH_MSG_TYPE_COPY_SEND, MACH_MSG_TYPE_MAKE_SEND_ONCE),
					/* msgh_bits */
	0,				/* msgh_size */
	MACH_PORT_NULL,			/* msgh_remote_port */
	MACH_PORT_NULL,			/* msgh_local_port */
	MACH_MSGH_KIND_NORMAL,		/* msgh_kind */
	BSD_REQ_MSG_ID			/* msgh_id */
    },
    {
	MACH_MSG_TYPE_INTEGER_32,	/* msgt_name */
	32,				/* msgt_size */
	8,				/* msgt_number */
	TRUE,				/* msgt_inline */
	FALSE,				/* msgt_longform */
	FALSE,				/* msgt_deallocate */
	0				/* msgt_unused */
    },
    0,0,
    { 0, 0, 0, 0, 0, 0 },
};

int
emul_generic(serv_port, interrupt, syscode, argp, rvalp)
	mach_port_t	serv_port;
	boolean_t	*interrupt;
	int	syscode;
	int	* argp;
	int	* rvalp;
{
	register kern_return_t	error;
	register mach_port_t	reply_port;
	union bsd_msg	bsd_msg;

    Restart:
	bsd_msg.req = bsd_req_template;

	reply_port = mig_get_reply_port();
	bsd_msg.req.hdr.msgh_remote_port = serv_port;
	bsd_msg.req.hdr.msgh_local_port  = reply_port;

	bsd_msg.req.syscode	= syscode;
	bsd_msg.req.rval2	= rvalp[1];
	bsd_msg.req.arg[0]	= argp[0];
	bsd_msg.req.arg[1]	= argp[1];
	bsd_msg.req.arg[2]	= argp[2];
	bsd_msg.req.arg[3]	= argp[3];
	bsd_msg.req.arg[4]	= argp[4];
	bsd_msg.req.arg[5]	= argp[5];

	error = mach_msg(&bsd_msg.req.hdr, MACH_SEND_MSG|MACH_RCV_MSG,
			 sizeof bsd_msg.req, sizeof bsd_msg, reply_port,
			 MACH_MSG_TIMEOUT_NONE, MACH_PORT_NULL);

	if (error != MACH_MSG_SUCCESS) {
	    return (error);
	}

	error = bsd_msg.rep.retcode;
	*interrupt = bsd_msg.rep.interrupt;

	if (error == 0) {

	    register char *		start;
	    char *			end;
	    register mach_msg_type_long_t *	tp;
	    register vm_size_t		size;

	    vm_address_t	user_addr, msg_addr;

	    /*
	     * Pass return values back to caller.
	     */
	    rvalp[0] = bsd_msg.rep.rval[0];
	    rvalp[1] = bsd_msg.rep.rval[1];

	    /*
	     * Scan reply message for data to copy
	     */
	    start = (char *)&bsd_msg + sizeof(struct bsd_reply);
	    end   = (char *)&bsd_msg + bsd_msg.rep.hdr.msgh_size;
	    while (end > start) {

		/*
		 * Descriptor for address
		 */
		start += sizeof(mach_msg_type_t);

		/*
		 * Address
		 */
		user_addr = *(vm_address_t *)start;
		start += sizeof(vm_address_t);

		/*
		 * Data - size is in bytes
		 */
		tp = (mach_msg_type_long_t *)start;
		if (tp->msgtl_header.msgt_longform) {
		    size = tp->msgtl_number;
		    start += sizeof(mach_msg_type_long_t);
		}
		else {
		    size = tp->msgtl_header.msgt_number;
		    start += sizeof(mach_msg_type_t);
		}

		if (tp->msgtl_header.msgt_inline) {
		    bcopy(start, (char *)user_addr, size);
		    start += size;
		    start = (char *)
				( ((int)start + sizeof(int) - 1)
				  & ~(sizeof(int) - 1) );
		}
		else {
		    msg_addr = *(vm_address_t *)start;
		    start += sizeof(vm_address_t);
		    bcopy((char *)msg_addr, (char *)user_addr, size);
		    (void) vm_deallocate(mach_task_self(), msg_addr, size);
		}
	    }
	}

	return (error);
}

