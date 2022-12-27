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
 * $Log:	machid.c,v $
 * Revision 2.6  91/08/30  14:51:50  rpd
 * 	Moved machid include files into the standard directory.
 * 
 * Revision 2.5  91/03/27  17:25:57  mrt
 * 	Changed mach.h include
 * 
 * Revision 2.4  91/03/19  12:30:07  mrt
 * 	Changed to new copyright
 * 
 * Revision 2.3  91/03/10  13:41:02  rpd
 * 	Changed authorize to allow all operations on informational/name ports.
 * 	[91/01/14            rpd]
 * 
 * Revision 2.2  90/09/12  16:31:22  rpd
 * 	Created.
 * 	[90/06/18            rpd]
 * 
 */

#include <stdio.h>
#include <mach.h>
#include <mach/message.h>
#include <mach/notify.h>
#include <mach_error.h>
#include <servers/machid_types.h>
#include "machid_internal.h"

static mach_port_t service;	/* our own service port */
static mach_port_t notify;	/* our notification port */

static boolean_t machid_demux();

static void
usage()
{
    quit(1, "usage: machid\n");
}

main(argc, argv)
    int argc;
    char *argv[];
{
    mhost_t host;
    mhost_priv_t host_priv;
    mtask_t *mtasks;
    unsigned int mtasksCnt;
    mach_port_t pset;
    kern_return_t kr;

    if (argc != 1)
	usage();

    if (mach_host_priv_self() == MACH_PORT_NULL)
	quit(1, "machid: can't get privileged host port\n");

    kr = mach_port_allocate(mach_task_self(), MACH_PORT_RIGHT_RECEIVE,
			    &service);
    if (kr != KERN_SUCCESS)
	quit(1, "machid: mach_port_allocate: %s\n", mach_error_string(kr));

    kr = netname_check_in(name_server_port, "MachID",
			  mach_task_self(), service);
    if (kr != KERN_SUCCESS)
	quit(1, "machid: netname_check_in: %s\n", mach_error_string(kr));

    kr = mach_port_allocate(mach_task_self(),
			    MACH_PORT_RIGHT_RECEIVE, &notify);
    if (kr != KERN_SUCCESS)
	quit(1, "machid: mach_port_allocate: %s\n", mach_error_string(kr));

    kr = mach_port_allocate(mach_task_self(),
			    MACH_PORT_RIGHT_PORT_SET, &pset);
    if (kr != KERN_SUCCESS)
	quit(1, "machid: mach_port_allocate: %s\n", mach_error_string(kr));

    kr = mach_port_move_member(mach_task_self(), service, pset);
    if (kr != KERN_SUCCESS)
	quit(1, "machid: mach_port_move_member: %s\n", mach_error_string(kr));

    kr = mach_port_move_member(mach_task_self(), notify, pset);
    if (kr != KERN_SUCCESS)
	quit(1, "machid: mach_port_move_member: %s\n", mach_error_string(kr));

    /* call these now to ensure that common things get consistent names */

    kr = do_host_ports(service, mach_host_priv_self(),
		       &host, &host_priv);
    if (kr != KERN_SUCCESS)
	quit(1, "machid: do_host_ports: %s\n", mach_error_string(kr));

    kr = do_host_tasks(service, mach_host_priv_self(),
		       host_priv, &mtasks, &mtasksCnt);
    if (kr != KERN_SUCCESS)
	quit(1, "machid: do_host_tasks: %s\n", mach_error_string(kr));

    kr = vm_deallocate(mach_task_self(), (vm_offset_t) mtasks,
		       (vm_size_t) (mtasksCnt * sizeof *mtasks));
    if (kr != KERN_SUCCESS)
	quit(1, "machid: vm_deallocate: %s\n", mach_error_string(kr));

    kr = mach_msg_server(machid_demux, 8192, pset);
    quit(1, "machid: mach_msg_server: %s\n", mach_error_string(kr));
}

static boolean_t
machid_demux(request, reply)
    mach_msg_header_t *request, *reply;
{
    if (request->msgh_local_port == service)
	return (machid_server(request, reply) ||
		machid_debug_server(request, reply));
    else if (request->msgh_local_port == notify)
	return notify_server(request, reply);
    else
	quit(1, "machid: machid_demux: bad local port %x\n",
	     request->msgh_local_port);
}

static mach_id_t next_machid_name = 1;

typedef struct port_record {
    struct port_record *pr_pnnext;	/* link for port->name hash table */
    struct port_record *pr_npnext;	/* link for name->port hash table */
    mach_port_t pr_port;		/* the send right */
    mach_id_t pr_name;			/* an artificial exported name */
    mach_type_t pr_type;		/* type of this port */
    mach_port_urefs_t pr_urefs;		/* number of urefs we hold */
    struct port_assoc *pr_assoc;	/* list of associations */
} port_record_t;

typedef struct port_assoc {
    struct port_assoc *pa_next;		/* link for list of assocations */
    mach_type_t pa_type;		/* type of the association */
    mach_id_t pa_name;			/* associated name */
} port_assoc_t;

#define	MAX_UREFS	1000	/* max urefs we will hold for a port */
#define MORE_UREFS	100	/* when we need urefs, how many to make */

#define	HASH_TABLE_SIZE	256

#define	PORT_HASH(port)		\
	((((port) & 0xff) + ((port) >> 8)) % HASH_TABLE_SIZE)

#define	NAME_HASH(name)		\
	((name) % HASH_TABLE_SIZE)

static port_record_t *port_to_name[HASH_TABLE_SIZE];
static port_record_t *name_to_port[HASH_TABLE_SIZE];

/* add a user-reference to the port record */

static void
add_reference(pr)
    port_record_t *pr;
{
    kern_return_t kr;

    if (++pr->pr_urefs > MAX_UREFS) {
	/* remove excess user references held by this module */

	kr = mach_port_mod_refs(mach_task_self(), pr->pr_port,
				MACH_PORT_RIGHT_SEND,
				1 - pr->pr_urefs);
	if (kr == KERN_SUCCESS)
	    pr->pr_urefs = 1;
	else if (kr == KERN_INVALID_RIGHT)
	    ; /* port is a dead name now, don't bother */
	else
	    quit(1, "machid: add_reference: mach_port_mod_refs: %s\n",
		 mach_error_string(kr));
    }
}

/* take a user-reference from the port record */

static void
sub_reference(pr)
    port_record_t *pr;
{
    kern_return_t kr;

    /* if we have no spare user-refs, create some */

    if (pr->pr_urefs == 1) {
	kr = mach_port_mod_refs(mach_task_self(), pr->pr_port,
				MACH_PORT_RIGHT_SEND, MORE_UREFS);
	if (kr == KERN_INVALID_RIGHT)
	    kr = mach_port_mod_refs(mach_task_self(), pr->pr_port,
				    MACH_PORT_RIGHT_DEAD_NAME, MORE_UREFS);
	if (kr != KERN_SUCCESS)
	    quit(1, "machid: sub_reference: mach_port_mod_refs: %s\n",
		 mach_error_string(kr));

	pr->pr_urefs += MORE_UREFS;
    }

    pr->pr_urefs--;
}

static port_record_t *
find_port(port)
    mach_port_t port;
{
    port_record_t *this;

    for (this = port_to_name[PORT_HASH(port)];
	 this != NULL;
	 this = this->pr_pnnext)
	if (this->pr_port == port)
	    return this;

    return NULL;
}

static port_record_t *
find_name(name)
    mach_id_t name;
{
    port_record_t *this;

    for (this = name_to_port[NAME_HASH(name)];
	 this != NULL;
	 this = this->pr_npnext)
	if (this->pr_name == name)
	    return this;

    return NULL;
}

static port_assoc_t *
find_assoc(this, atype)
    port_record_t *this;
    mach_type_t atype;
{
    port_assoc_t *thisa;

    for (thisa = this->pr_assoc; thisa != NULL; thisa = thisa->pa_next)
	if (thisa->pa_type == atype)
	    return thisa;

    return NULL;
}

/* convert a send right to our exported name,
   consuming a user-reference for the send right. */

mach_id_t
name_lookup(port, type)
    mach_port_t port;
    mach_type_t type;
{
    port_record_t *this, **bucket;
    mach_port_t previous;
    kern_return_t kr;

    if (!MACH_PORT_VALID(port))
	return 0;

    this = find_port(port);
    if (this != NULL) {
	/* found an existing port record */

	add_reference(this);
	return this->pr_name;
    }

    /* no port record, so create a new one */

    this = (port_record_t *) malloc(sizeof *this);
    if (this == NULL)
	quit(1, "machid: malloc failed\n");

    this->pr_port = port;
    this->pr_name = next_machid_name++;
    this->pr_type = type;
    this->pr_urefs = 1;
    this->pr_assoc = NULL;

    bucket = &port_to_name[PORT_HASH(port)];
    this->pr_pnnext = *bucket;
    *bucket = this;

    /* link record into name->port hash table */

    bucket = &name_to_port[NAME_HASH(this->pr_name)];
    this->pr_npnext = *bucket;
    *bucket = this;

    /* register for a dead-name notification */

    kr = mach_port_request_notification(mach_task_self(), port,
					MACH_NOTIFY_DEAD_NAME, TRUE,
					notify, MACH_MSG_TYPE_MAKE_SEND_ONCE,
					&previous);
    if ((kr != KERN_SUCCESS) || (previous != MACH_PORT_NULL)) {
	printf("port = %x\n", port);
	quit(1, "machid: mach_port_request_notification: %s\n",
	     mach_error_string(kr));
    }

    return this->pr_name;
}

static boolean_t
authorize(this, auth, op)
    port_record_t *this;
    mach_port_t auth;
    mach_op_t op;
{
    port_record_t *authpr;
    port_assoc_t *thisa;

    if (op == mo_Info)
	return TRUE;

    if ((this->pr_type == MACH_TYPE_PROCESSOR_SET_NAME) ||
	(this->pr_type == MACH_TYPE_HOST) ||
	(this->pr_type == MACH_TYPE_OBJECT_NAME))
	return TRUE;

    if (!MACH_PORT_VALID(auth))
	return FALSE;

    authpr = find_port(auth);
    if (authpr == NULL)
	return FALSE;

    if (this->pr_name == authpr->pr_name)
	return TRUE;

    thisa = find_assoc(this, MACH_TYPE_HOST_PRIV);
    if ((thisa != NULL) && (thisa->pa_name == authpr->pr_name))
	return TRUE;

    thisa = find_assoc(this, MACH_TYPE_TASK);
    if ((thisa != NULL) && (thisa->pa_name == authpr->pr_name))
	return TRUE;

    return FALSE;
}

/* convert an exported name to a send right,
   returning a user-reference for the send right.
   does authentication. */

kern_return_t
port_lookup(name, auth, op, portp)
    mach_id_t name;
    mach_port_t auth;
    mach_op_t op;
    mach_port_t *portp;
{
    port_record_t *this;

    this = find_name(name);
    if (this == NULL)
	return KERN_INVALID_NAME;

    if (!authorize(this, auth, op))
	return KERN_PROTECTION_FAILURE;

    sub_reference(this);
    *portp = this->pr_port;
    return KERN_SUCCESS;
}

/* convert an exported name to a type */

mach_type_t
type_lookup(name)
    mach_id_t name;
{
    port_record_t *this;

    this = find_name(name);
    if (this != NULL)
	return this->pr_type;

    return 0;
}

/* consume a user-reference for a port which we already know about */

void
port_consume(port)
    mach_port_t port;
{
    port_record_t *this;

    if (!MACH_PORT_VALID(port))
	quit(1, "machid: port_consume: invalid port\n");

    this = find_port(port);
    if (this != NULL) {
	add_reference(this);
	return;
    }

    quit(1, "machid: port_consume: didn't find port\n");
}

/* consume a user-reference for a port which we might not know about */

void
auth_consume(port)
    mach_port_t port;
{
    port_record_t *this;
    kern_return_t kr;

    if (!MACH_PORT_VALID(port))
	return;

    this = find_port(port);
    if (this != NULL) {
	/* we know about the port, so we can avoid the kernel call */

	add_reference(this);
	return;
    }

    kr = mach_port_deallocate(mach_task_self(), port);
    if (kr != KERN_SUCCESS)
	quit(1, "machid: mach_port_deallocate: %s\n", mach_error_string(kr));
}

/* destroy the port record of a dead port, consuming
   an extra user-reference for the dead-name notification */

void
port_destroy(port)
    mach_port_t port;
{
    port_record_t *this, **prev;
    port_assoc_t *assoc, *next;
    kern_return_t kr;

    /* remove the port record from port->name hash table */

    for (prev = &port_to_name[PORT_HASH(port)], this = *prev;
	 this != NULL;
	 prev = &this->pr_pnnext, this = *prev)
	if (this->pr_port == port)
	    break;

    if (this == NULL)
	quit(1, "machid: port_destroy: didn't find port\n");

    *prev = this->pr_pnnext;

    /* remove the port record from name->port hash table */

    for (prev = &name_to_port[NAME_HASH(this->pr_name)], this = *prev;
	 this != NULL;
	 prev = &this->pr_npnext, this = *prev)
	if (this->pr_port == port)
	    break;

    if (this == NULL)
	quit(1, "machid: port_destroy: didn't find port\n");

    *prev = this->pr_npnext;

    /* deallocate the dead name */

    kr = mach_port_mod_refs(mach_task_self(), port,
			    MACH_PORT_RIGHT_DEAD_NAME,
			    -this->pr_urefs - 1);
    if (kr != KERN_SUCCESS)
	quit(1, "machid: port_destroy: mach_port_mod_refs: %s\n",
	     mach_error_string(kr));

    /* deallocate the associations */

    for (assoc = this->pr_assoc; assoc != NULL; assoc = next) {
	next = assoc->pa_next;
	free((char *) assoc);
    }

    free((char *) this);
}

/* create an assocation name->aname, of the specified type */

void
assoc_create(name, atype, aname)
    mach_id_t name;
    mach_type_t atype;
    mach_id_t aname;
{
    port_record_t *this;

    this = find_name(name);
    if (this != NULL) {
	port_assoc_t *thisa;

	thisa = find_assoc(this, atype);
	if (thisa != NULL) {
	    /* change the existing association */

	    thisa->pa_name = aname;
	    return;
	}

	/* create a new association */

	thisa = (port_assoc_t *) malloc(sizeof *thisa);
	if (thisa == NULL)
	    quit(1, "machid: malloc failed\n");

	thisa->pa_type = atype;
	thisa->pa_name = aname;
	thisa->pa_next = this->pr_assoc;
	this->pr_assoc = thisa;
    }

    /* if we don't find name, do nothing */
}

/* lookup an associated name */

mach_id_t
assoc_lookup(name, atype)
    mach_id_t name;
    mach_type_t atype;
{
    port_record_t *this;

    this = find_name(name);
    if (this != NULL) {
	port_assoc_t *thisa;

	thisa = find_assoc(this, atype);
	if (thisa != NULL)
	    return thisa->pa_name;
    }

    return 0;
}
