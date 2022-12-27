/* 
 * Mach Operating System
 * Copyright (c) 1989 Carnegie-Mellon University
 * All rights reserved.  The CMU software License Agreement specifies
 * the terms and conditions for use and redistribution.
 */
/*
 * HISTORY
 * $Log:	user_reply_msg.c,v $
 * Revision 2.3  90/06/02  15:28:49  rpd
 * 	Updated moveout additions for new IPC.
 * 	[90/05/03            rpd]
 * 	Major revisions, principally to let extend_reply_msg
 * 	reuse an existing data descriptor.
 * 	[90/04/27            rpd]
 * 	Converted to new IPC.
 * 	Fixed unsigned bug in extend_reply_msg.
 * 	[90/03/26  20:26:21  rpd]
 * 
 * Revision 2.2  90/03/14  21:31:15  rwd
 * 	Change moveout to see if moving to user or server address space.
 * 	[90/02/06            rwd]
 * 
 */
/*
 * Routines to extend generic reply message to handle data
 * intended for user.
 */
#include <map_uarea.h>

#include <uxkern/import_mach.h>

#include <sys/param.h>
#include <sys/types.h>
#include <sys/user.h>
#include <sys/proc.h>

#include <uxkern/bsd_msg.h>

struct data_desc {
	mach_msg_type_t addr_type;
	vm_offset_t addr;
	mach_msg_type_long_t data_type;
	vm_offset_t data;
};

mach_msg_type_long_t	data_type_ooline = {
	{
	    0,		/* name - unused */
	    0,		/* size - unused */
	    0,		/* number - unused */
	    FALSE,	/* not inline */
	    TRUE,	/* longform */
	    TRUE,	/* deallocate */
	    0		/* unused */
	},
	MACH_MSG_TYPE_CHAR,	/* name */
	8,			/* size */
	0			/* number - fill in */
};

mach_msg_type_long_t	data_type_inline = {
	{
	    0,		/* name - unused */
	    0,		/* size - unused */
	    0,		/* number - unused */
	    TRUE,	/* inline */
	    TRUE,	/* longform */
	    FALSE,	/* NOT deallocate */
	    0		/* unused */
	},
	MACH_MSG_TYPE_CHAR,	/* name */
	8,			/* size */
	0			/* number - fill in */
};

mach_msg_type_t		addr_type = {
	MACH_MSG_TYPE_INTEGER_32,
	32,
	1,
	TRUE,		/* inline */
	FALSE,
	FALSE,
	0
};

#define	BSD_REP_DATA_SIZE	(sizeof(struct data_desc))

#define	BSD_REP_DATA_MAX	(2*16) /* XXX - BSD allows at most 16 iovs */

/* The limit on inline data we can allow, lest we run out of space. */

#define	BSD_REP_SIZE_MAX	(8192 - \
				 sizeof(struct bsd_reply) - \
				 BSD_REP_DATA_MAX * BSD_REP_DATA_SIZE)

/*
 *	extend_reply_msg reserves space in the reply message for
 *	"size" bytes of data, eventually to be copied to "user_addr"
 *	in the client.
 *
 *	Not all of the reserved space must be used immediately.
 *	extend_current_output should be used to mark space as used.
 *
 *	finish_reply_msg should be used to finalize the msg,
 *	discarding unused memory and setting the msg's size correctly.
 */

void
finish_data_desc(msg, dd)
	register mach_msg_header_t *msg;
	register struct data_desc *dd;
{
	register struct uthread *uth = &u;
	vm_size_t number = dd->data_type.msgtl_number;

	/*
	 *	If the descriptor is out-of-line,
	 *	free any unused memory.  In any case,
	 *	increment the msg size if the descriptor
	 *	actually contains data.
	 */

	if (dd->data_type.msgtl_header.msgt_inline) {
		if (number > 0)
			msg->msgh_size += (number+3 &~ 3) +
				(sizeof *dd - sizeof dd->data);
	} else {
		vm_size_t used = round_page(number);

		if (used < uth->uu_current_size)
			(void) vm_deallocate(mach_task_self(),
					     dd->data + used,
					     uth->uu_current_size - used);

		if (number > 0) {
			msg->msgh_size += sizeof *dd;

			/* the reply message is no longer simple */
			msg->msgh_bits |= MACH_MSGH_BITS_COMPLEX;
		}
	}

	uth->uu_current_size = 0;
}

vm_offset_t
extend_reply_msg(user_addr, size)
	vm_offset_t	user_addr;
	vm_size_t	size;
{
	register struct uthread *uth = &u;
	register mach_msg_header_t *msg = uth->uu_reply_msg;
	register struct data_desc *dd;
	vm_offset_t addr;

	if (size == 0)
		return 0;

	/*
	 *	Is there a preexisting data descriptor that we can use?
	 */

	if (uth->uu_current_size != 0) {
		vm_size_t used;

		dd = (struct data_desc *) ((char *) msg + msg->msgh_size);
		used = dd->data_type.msgtl_number;

		if ((user_addr == dd->addr + used) &&
		    (used + size <= uth->uu_current_size)) {
			/* yes, we can use this data descriptor */

			if (dd->data_type.msgtl_header.msgt_inline)
				addr = (vm_offset_t) &dd->data;
			else
				addr = dd->data;

			return addr + used;
		}

		finish_data_desc(msg, dd);
	}

	/*
	 *	We have to allocate a new data descriptor.
	 *	If possible, we try to make it inline.
	 */

	if (msg->msgh_size + sizeof(struct data_desc) > 8192)
		panic("extend_reply_msg");

	dd = (struct data_desc *) ((char *) msg + msg->msgh_size);
	dd->addr_type = addr_type;
	dd->addr = user_addr;

	/*
	 *	We don't let the amount of inline data in the message
	 *	exceed BSD_REP_SIZE_MAX.  This ensures that there
	 *	will be enough space in the message for data descriptors.
	 *	We use msg->msgh_size as a conservative estimate
	 *	of the current amount of inline data.
	 */

	if (msg->msgh_size + size > BSD_REP_SIZE_MAX) {
		/* must use out-of-line memory */

		size = round_page(size);
		(void) vm_allocate(mach_task_self(), &addr, size, TRUE);

		dd->data_type = data_type_ooline;
		dd->data = addr;
	} else {
		dd->data_type = data_type_inline;
		addr = (vm_offset_t) &dd->data;
	}

	uth->uu_current_size = size;
	return addr;
}

void
extend_current_output(size)
	vm_size_t	size;
{
	if (size != 0) {
		register mach_msg_header_t *msg = u.u_reply_msg;
		register struct data_desc *dd =
			(struct data_desc *) ((char *) msg + msg->msgh_size);

		dd->data_type.msgtl_number += size;
	}
}

void
finish_reply_msg()
{
	register struct uthread *uth = &u;

	/*
	 *	If we were working on a data descriptor,
	 *	it must be finalized.
	 */

	if (uth->uu_current_size != 0) {
		register mach_msg_header_t *msg = uth->uu_reply_msg;
		register struct data_desc *dd =
			(struct data_desc *) ((char *) msg + msg->msgh_size);

		finish_data_desc(msg, dd);
	}
}

int
moveout(data, user_addr, count)
	vm_offset_t data;
	vm_offset_t user_addr;
	vm_size_t count;
{
	register struct uthread *uth = &u;
	register mach_msg_header_t *msg = uth->uu_reply_msg;
	register struct data_desc *dd;

#if	MAP_UAREA
	if (msg == 0) {
		bcopy(data, user_addr, count);
		(void) vm_deallocate(mach_task_self(), data, count);
		return 0;
	}
#endif	MAP_UAREA

	finish_reply_msg();

	/*
	 *	We have to add a new out-of-line data descriptor.
	 *	This is like a combined extend_reply_msg,
	 *	extend_current_output, finish_reply_msg operation.
	 */

	if (msg->msgh_size + sizeof *dd > 8192)
		panic("moveout");

	dd = (struct data_desc *) ((char *) msg + msg->msgh_size);
	dd->addr_type = addr_type;
	dd->addr = user_addr;
	dd->data_type = data_type_ooline;
	dd->data_type.msgtl_number = count;
	dd->data = data;

	msg->msgh_size += sizeof *dd;
	/* the reply message is no longer simple */
	msg->msgh_bits |= MACH_MSGH_BITS_COMPLEX;

	return 0;
}
