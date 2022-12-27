/* 
 * Mach Operating System
 * Copyright (c) 1989 Carnegie-Mellon University
 * All rights reserved.  The CMU software License Agreement specifies
 * the terms and conditions for use and redistribution.
 */
/*
 * HISTORY
 * $Log:	device_reply_hdlr.c,v $
 * Revision 2.7  90/11/05  16:57:19  rpd
 * 	Increased NREPLY_HASH to 64, changed REPLY_HASH to something
 * 	reasonable.  Added per-bucket locks and counts.
 * 	[90/10/31            rpd]
 * 
 * Revision 2.6  90/06/02  15:27:24  rpd
 * 	Updated set_thread_priority call for the new priority range.
 * 	[90/04/23            rpd]
 * 	Converted to new IPC.
 * 	[90/03/26  20:12:33  rpd]
 * 
 * Revision 2.5  89/12/08  20:16:38  rwd
 * 	Call cthread_wire before setting thread priority.
 * 	[89/11/01            rwd]
 * 
 * Revision 2.4  89/09/15  15:29:26  rwd
 * 	Change includes
 * 	[89/09/11            rwd]
 * 
 * Revision 2.3  89/08/31  16:29:18  rwd
 * 	Added device_read_reply_inband
 * 	[89/08/15            rwd]
 * 
 * Revision 2.2  89/08/09  14:46:05  rwd
 * 	Added copyright to file by dbg.  Added device_write_reply_inband.
 * 	[89/07/17            rwd]
 * 
 */
/*
 * Handler for device read and write replies.  Simulates an
 * interrupt handler.
 */
#include <sys/queue.h>
#include <sys/zalloc.h>

#include <uxkern/import_mach.h>
#include <uxkern/device.h>

#include <uxkern/device_reply_hdlr.h>

#define	NREPLY_HASH	64

/*
 *	We add the low 8 bits to the high 24 bits, because
 *	this interacts well with the way the IPC implementation
 *	allocates port names.
 */

#define	REPLY_HASH(port)	\
	((((port) & 0xff) + ((port) >> 8)) & (NREPLY_HASH-1))

struct reply_hash {
	struct mutex lock;
	queue_head_t queue;
	int count;
} reply_hash[NREPLY_HASH];

struct reply_entry {
	queue_chain_t	chain;
	mach_port_t	port;
	char *		object;
	kr_fn_t		read_reply;
	kr_fn_t		write_reply;
};
typedef struct reply_entry *reply_entry_t;

zone_t	reply_entry_zone;

mach_port_t	reply_port_set;

any_t	device_reply_loop();	/* forward */

extern void	ux_create_thread();

enum dev_reply_select {
	DR_READ,
	DR_WRITE
};
typedef enum dev_reply_select	dr_select_t;

void
reply_hash_enter(port, object, read_reply, write_reply)
	mach_port_t	port;
	char		*object;
	kr_fn_t		read_reply, write_reply;
{
	register reply_entry_t	re;
	register struct reply_hash *b;

	re = (reply_entry_t) zalloc(reply_entry_zone);
	re->port = port;
	re->object = object;
	re->read_reply = read_reply;
	re->write_reply = write_reply;

	b = &reply_hash[REPLY_HASH(port)];
	mutex_lock(&b->lock);
	queue_enter(&b->queue, re, reply_entry_t, chain);
	b->count++;
	mutex_unlock(&b->lock);

	(void) mach_port_move_member(mach_task_self(), port, reply_port_set);
}

boolean_t
reply_hash_lookup(port, which, object, func)
	mach_port_t	port;
	dr_select_t	which;
	char		**object;	/* OUT */
	kr_fn_t		*func;		/* OUT */
{
	register reply_entry_t	re;
	register struct reply_hash *b;

	b = &reply_hash[REPLY_HASH(port)];
	mutex_lock(&b->lock);
	for (re = (reply_entry_t)queue_first(&b->queue);
	     !queue_end(&b->queue, (queue_entry_t)re);
	     re = (reply_entry_t)queue_next(&re->chain)) {
	    if (re->port == port) {
		*object = re->object;
		*func   = (which == DR_WRITE) ? re->write_reply
					      : re->read_reply;
		mutex_unlock(&b->lock);
		return (TRUE);
	    }
	}
	mutex_unlock(&b->lock);
	return (FALSE);
}

void
reply_hash_remove(port)
	mach_port_t	port;
{
	register reply_entry_t	re;
	register struct reply_hash *b;

	(void) mach_port_move_member(mach_task_self(), port, MACH_PORT_NULL);

	b = &reply_hash[REPLY_HASH(port)];
	mutex_lock(&b->lock);
	for (re = (reply_entry_t)queue_first(&b->queue);
	     !queue_end(&b->queue, (queue_entry_t)re);
	     re = (reply_entry_t)queue_next(&re->chain)) {
	    if (re->port == port) {
		queue_remove(&b->queue, re, reply_entry_t, chain);
		b->count--;
		mutex_unlock(&b->lock);
		zfree(reply_entry_zone, (vm_offset_t)re);
		return;
	    }
	}
	mutex_unlock(&b->lock);
}

void device_reply_hdlr()
{
	register int	i;

	for (i = 0; i < NREPLY_HASH; i++) {
	    mutex_init(&reply_hash[i].lock);
	    queue_init(&reply_hash[i].queue);
	    reply_hash[i].count = 0;
	}

	reply_entry_zone =
		zinit((vm_size_t)sizeof(struct reply_entry),
		      (vm_size_t)sizeof(struct reply_entry)
			* 4096,
		      vm_page_size,
		      FALSE,	/* must be wired because inode_pager
				   uses these structures */
		      "device_reply_port to device");
	(void) mach_port_allocate(mach_task_self(), MACH_PORT_RIGHT_PORT_SET,
				  &reply_port_set);

	ux_create_thread(device_reply_loop);
}

any_t
device_reply_loop(arg)
	any_t	arg;
{
	kern_return_t		rc;

	union reply_msg {
	    mach_msg_header_t	hdr;
	    char		space[8192];
	} reply_msg;

	/*
	 * We KNOW that none of these messages have replies...
	 */
	mig_reply_header_t	rep_rep_msg;

	cthread_set_name(cthread_self(), "device reply handler");

	/*
	 * Wire this cthread to a kernel thread so we can
	 * up its priority
	 */
	cthread_wire();

	/*
	 * Make this thread high priority.
	 */
	set_thread_priority(mach_thread_self(), 2);

	for (;;) {
	    rc = mach_msg(&reply_msg.hdr, MACH_RCV_MSG,
			  0, sizeof reply_msg, reply_port_set,
			  MACH_MSG_TIMEOUT_NONE, MACH_PORT_NULL);
	    if (rc == MACH_MSG_SUCCESS) {
		if (device_reply_server(&reply_msg.hdr,
					&rep_rep_msg.Head))
		{
		    /*
		     * None of these messages need replies
		     */
		}
	    }
	}
}

kern_return_t
device_open_reply(reply_port, return_code, device_port)
	mach_port_t	reply_port;
	kern_return_t	return_code;
	mach_port_t	device_port;
{
	/* NOT USED */
}

kern_return_t
device_write_reply(reply_port, return_code, bytes_written)
	mach_port_t	reply_port;
	kern_return_t	return_code;
	int		bytes_written;
{
	char		*object;
	kr_fn_t		func;

	if (!reply_hash_lookup(reply_port, DR_WRITE, &object, &func))
	    return(0);

	return ((*func)(object, return_code, bytes_written));
}

kern_return_t
device_write_reply_inband(reply_port, return_code, bytes_written)
	mach_port_t	reply_port;
	kern_return_t	return_code;
	int		bytes_written;
{
	char		*object;
	kr_fn_t		func;

	if (!reply_hash_lookup(reply_port, DR_WRITE, &object, &func))
	    return(0);

	return ((*func)(object, return_code, bytes_written));
}

kern_return_t
device_read_reply(reply_port, return_code, data, data_count)
	mach_port_t	reply_port;
	kern_return_t	return_code;
	io_buf_ptr_t	data;
	unsigned int	data_count;
{
	char		*object;
	kr_fn_t		func;

	if (!reply_hash_lookup(reply_port, DR_READ, &object, &func))
	    return(0);

	return ((*func)(object, return_code, data, data_count));
}

kern_return_t
device_read_reply_inband(reply_port, return_code, data, data_count)
	mach_port_t	reply_port;
	kern_return_t	return_code;
	io_buf_ptr_t	data;
	unsigned int	data_count;
{
	char		*object;
	kr_fn_t		func;

	if (!reply_hash_lookup(reply_port, DR_READ, &object, &func))
	    return(0);

	return ((*func)(object, return_code, data, data_count));
}
