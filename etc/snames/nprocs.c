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
 * $Log:	nprocs.c,v $
 * Revision 2.4  91/03/27  17:29:40  mrt
 * 	Changed mach.h include
 * 
 * Revision 2.3  91/03/19  12:40:00  mrt
 * 	Changed to new copyright
 * 
 * Revision 2.2  90/09/12  16:33:51  rpd
 * 	Initial check-in.
 * 	[90/09/12  15:53:44  rpd]
 * 
 */

#include <stdio.h>
#include <strings.h>
#include <mach.h>
#include <mach/message.h>
#include <mach/notify.h>
#include <servers/netname_defs.h>

extern char *malloc();
extern void free();

extern boolean_t Debug;
extern char *program;

extern mach_port_t notify;
extern mach_port_t service;
extern mach_port_t child;

typedef struct port_record {
    struct port_record *next;
    mach_port_t port;
    mach_port_urefs_t urefs;
} *port_record_t;

static port_record_t ports;

#define	MAX_UREFS	1000	/* max urefs we will hold for a port */
#define MORE_UREFS	1000	/* when we need urefs, how many to make */

static void
add_reference(port)
    mach_port_t port;
{
    port_record_t this, *last;
    mach_port_t previous;
    kern_return_t kr;

    if (!MACH_PORT_VALID(port))
	return;

    for (last = &ports, this = ports;
	 this != NULL;
	 last = &this->next, this = *last)
	if (this->port == port) {
	    /* we already know about this port */

	    if (++this->urefs > MAX_UREFS) {
		kr = mach_port_mod_refs(mach_task_self(), port,
					MACH_PORT_RIGHT_SEND, 1 - this->urefs);
		if (kr == KERN_SUCCESS)
		    this->urefs = 1;
		else if (kr == KERN_INVALID_RIGHT)
		    ; /* port is a dead name now */
		else
		    quit(1, "%s: add_reference: mach_port_mod_refs: %s\n",
			 program, mach_error_string(kr));
	    }

	    /* remove the record and move it to the front */

	    *last = this->next;
	    goto insert;
	}

    /* we haven't seen this port before */

    this = (port_record_t) malloc(sizeof *this);
    this->port = port;
    this->urefs = 1;

    kr = mach_port_request_notification(mach_task_self(), port,
					MACH_NOTIFY_DEAD_NAME, TRUE,
					notify, MACH_MSG_TYPE_MAKE_SEND_ONCE,
					&previous);
    if ((kr != KERN_SUCCESS) || (previous != MACH_PORT_NULL))
	quit(1, "%s: mach_port_request_notification: %s\n",
	     program, mach_error_string(kr));

  insert:
    this->next = ports;
    ports = this;
}

static void
sub_reference(port)
    mach_port_t port;
{
    port_record_t this, *last;
    kern_return_t kr;

    if (!MACH_PORT_VALID(port))
	return;

    /* we must know about this port */

    for (last = &ports, this = ports;
	 this != NULL;
	 last = &this->next, this = *last)
	if (this->port == port)
	    break;

    /* move it to the front */

    *last = this->next;
    this->next = ports;
    ports = this;

    /* make more user-references if we don't have enough */

    if (this->urefs == 1) {
	kr = mach_port_mod_refs(mach_task_self(), port,
				MACH_PORT_RIGHT_SEND, MORE_UREFS);
	if (kr == KERN_INVALID_RIGHT)
	    kr = mach_port_mod_refs(mach_task_self(), port,
				    MACH_PORT_RIGHT_DEAD_NAME, MORE_UREFS);
	if (kr != KERN_SUCCESS)
	    quit(1, "%s: mach_port_mod_refs: %s\n",
		 program, mach_error_string(kr));

	this->urefs += MORE_UREFS;
    }

    this->urefs--;
}

static void
remove_port(port)
    mach_port_t port;
{
    port_record_t this, *last;
    kern_return_t kr;

    /* we must know about this port */

    for (last = &ports, this = ports;
	 this != NULL;
	 last = &this->next, this = *last)
	if (this->port == port)
	    break;

    /* deallocate the dead name */

    kr = mach_port_mod_refs(mach_task_self(), port,
			    MACH_PORT_RIGHT_DEAD_NAME, - this->urefs - 1);
    if (kr != KERN_SUCCESS)
	quit(1, "%s: mach_port_mod_refs: %s\n",
	     program, mach_error_string(kr));

    /* remove the record */

    *last = this->next;
    free((char *) this);
}

/*
 *  We use a simple self-organizing list to keep track of
 *  registered ports.
 */

typedef struct name_record {
    struct name_record *next;
    netname_name_t key;
    mach_port_t name, sig;
} *name_record_t;

static name_record_t list;

static boolean_t
netname_find(key, thisp, prevp)
    netname_name_t key;
    name_record_t *thisp, **prevp;
{
    register name_record_t *prev, this;

    for (this = *(prev = &list);
	 this != NULL;
	 this = *(prev = &this->next))
	if (strncasecmp(this->key, key, 80) == 0) {
	    *thisp = this;
	    *prevp = prev;
	    return TRUE;
	}

    return FALSE;
}

void
netname_init()
{
    list = NULL;
    ports = NULL;
}

void
netname_remove(name)
    mach_port_t name;
{
    register name_record_t *prev, this;

    if (Debug)
	printf("%s: netname_remove(%x)\n",
	       program, name);

    prev = &list;
    this = list;

    while (this != NULL)
	if ((this->name == name) ||
	    (this->sig == name)) {
	    register name_record_t last = this;

	    this = *prev = this->next;
	    free(last);
	} else
	    this = *(prev = &this->next);
}

kern_return_t
do_netname_check_in(server, key, sig, name)
    mach_port_t server;
    netname_name_t key;
    mach_port_t sig, name;
{
    name_record_t *prev, this;

    if (Debug)
	printf("%s: netname_check_in(%.80s, name=%x, sig=%x)\n",
	       program, key, name, sig);

    if (netname_find(key, &this, &prev)) {
	if (this->sig == sig) {
	    this->name = name;
	    *prev = this->next;
	} else
	    return NETNAME_NOT_YOURS;
    } else {
	this = (name_record_t) malloc(sizeof *this);
	if (this == NULL)
	    return KERN_RESOURCE_SHORTAGE;

	this->name = name;
	this->sig = sig;
	(void) strncpy(this->key, key, 80);
    }

    this->next = list;
    list = this;

    /* we take responsibility for the refs in the request */

    add_reference(sig);
    add_reference(name);
    return NETNAME_SUCCESS;
}

kern_return_t
do_netname_look_up(server, host, key, namep)
    mach_port_t server;
    netname_name_t host, key;
    mach_port_t *namep;
{
    name_record_t *prev, this;

    if (netname_find(key, &this, &prev)) {
	mach_port_t name = this->name;

	*prev = this->next;
	this->next = list;
	list = this;

	if (Debug)
	    printf("%s: netname_look_up(%.80s) => %x\n",
		   program, key, name);

	/* return a ref for the port */

	sub_reference(*namep = name);
	return NETNAME_SUCCESS;
    } else {
	if (Debug)
	    printf("%s: netname_look_up(%.80s) failed\n",
		   program, key);

	return NETNAME_NOT_CHECKED_IN;
    }
}

kern_return_t
do_netname_check_out(server, key, sig)
    mach_port_t server;
    netname_name_t key;
    mach_port_t sig;
{
    name_record_t *prev, this;

    if (Debug)
	printf("%s: netname_check_out(%.80s, sig=%x)\n",
	       program, key, sig);

    if (netname_find(key, &this, &prev)) {
	if (this->sig == sig) {
	    *prev = this->next;
	    free((char *) this);

	    /* we take responsibility for the ref in the request */

	    add_reference(sig);
	    return NETNAME_SUCCESS;
	} else
	    return NETNAME_NOT_YOURS;
    } else
	return NETNAME_NOT_CHECKED_IN;
}

kern_return_t
do_netname_version(server, version)
    mach_port_t server;
    netname_name_t version;
{
    if (Debug)
	printf("%s: netname_version()\n", program);

    (void) strcpy(version, "Simple Name Service 3.0");
    return KERN_SUCCESS;
}

kern_return_t
do_mach_notify_port_deleted(notify, name)
    mach_port_t notify;
    mach_port_t name;
{
    quit(1, "%s: do_mach_notify_port_deleted\n", program);
    return KERN_FAILURE;
}

kern_return_t
do_mach_notify_msg_accepted(notify, name)
    mach_port_t notify;
    mach_port_t name;
{
    quit(1, "%s: do_mach_notify_msg_accepted\n", program);
    return KERN_FAILURE;
}

kern_return_t
do_mach_notify_port_destroyed(notify, port)
    mach_port_t notify;
    mach_port_t port;
{
    quit(1, "%s: do_mach_notify_port_destroyed\n", program);
    return KERN_FAILURE;
}

kern_return_t
do_mach_notify_no_senders(notify, mscount)
    mach_port_t notify;
    mach_port_mscount_t mscount;
{
    quit(1, "%s: do_mach_notify_no_senders\n", program);
    return KERN_FAILURE;
}

kern_return_t
do_mach_notify_send_once(notify)
    mach_port_t notify;
{
    quit(1, "%s: do_mach_notify_send_once\n", program);
    return KERN_FAILURE;
}

kern_return_t
do_mach_notify_dead_name(notify, name)
    mach_port_t notify;
    mach_port_t name;
{
    if (Debug)
	printf("%s: do_mach_notify_dead_name: %x\n", program, name);

    if (name == child)
	exit(0);

    netname_remove(name);
    remove_port(name);
}
