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
 * $Log:	machid_debug_procs.c,v $
 * Revision 2.7  91/08/30  14:51:58  rpd
 * 	Moved machid include files into the standard directory.
 * 
 * Revision 2.6  91/03/27  17:26:06  mrt
 * 	Changed mach.h include
 * 
 * Revision 2.5  91/03/19  12:30:26  mrt
 * 	Changed to new copyright
 * 
 * Revision 2.4  91/03/10  13:41:15  rpd
 * 	Added host_zone_info.
 * 	[91/01/14            rpd]
 * 
 * Revision 2.3  90/11/05  23:33:58  rpd
 * 	Added host_stack_usage, processor_set_stack_usage.
 * 	[90/10/29            rpd]
 * 
 * Revision 2.2  90/09/12  16:31:39  rpd
 * 	Created.
 * 	[90/06/18            rpd]
 * 
 */

#include <stdio.h>
#include <mach.h>
#include <mach_debug/mach_debug.h>
#include <servers/machid_types.h>
#include "machid_internal.h"

kern_return_t
do_port_get_srights(server, auth, task, name, srightsp)
    mach_port_t server;
    mach_port_t auth;
    mtask_t task;
    mach_port_t name;
    mach_port_rights_t *srightsp;
{
    mach_port_t port;
    kern_return_t kr;

    kr = port_lookup(task, auth, mo_Info, &port);
    if (kr != KERN_SUCCESS) {
	if (kr == KERN_INVALID_NAME)
	    kr = KERN_INVALID_TASK;
	return kr;
    }

    kr = mach_port_get_srights(port, name, srightsp);
    port_consume(port);
    if (kr != KERN_SUCCESS)
	return kr;

    auth_consume(auth);
    return KERN_SUCCESS;
}

kern_return_t
do_port_space_info(server, auth, task, infop,
		   table_infop, table_infoCntp,
		   tree_infop, tree_infoCntp)
    mach_port_t server;
    mach_port_t auth;
    mtask_t task;
    ipc_info_space_t *infop;
    ipc_info_name_array_t *table_infop;
    unsigned int *table_infoCntp;
    ipc_info_tree_name_array_t *tree_infop;
    unsigned int *tree_infoCntp;
{
    mach_port_t port;
    kern_return_t kr;

    kr = port_lookup(task, auth, mo_Info, &port);
    if (kr != KERN_SUCCESS) {
	if (kr == KERN_INVALID_NAME)
	    kr = KERN_INVALID_TASK;
	return kr;
    }

    kr = mach_port_space_info(port, infop,
			      table_infop, table_infoCntp,
			      tree_infop, tree_infoCntp);
    port_consume(port);
    if (kr != KERN_SUCCESS)
	return kr;

    auth_consume(auth);
    return KERN_SUCCESS;
}

kern_return_t
do_port_dnrequest_info(server, auth, task, name, totalp, usedp)
    mach_port_t server;
    mach_port_t auth;
    mtask_t task;
    mach_port_t name;
    unsigned int *totalp, *usedp;
{
    mach_port_t port;
    kern_return_t kr;

    kr = port_lookup(task, auth, mo_Info, &port);
    if (kr != KERN_SUCCESS) {
	if (kr == KERN_INVALID_NAME)
	    kr = KERN_INVALID_TASK;
	return kr;
    }

    kr = mach_port_dnrequest_info(port, name, totalp, usedp);
    port_consume(port);
    if (kr != KERN_SUCCESS)
	return kr;

    auth_consume(auth);
    return KERN_SUCCESS;
}

kern_return_t
do_vm_region_info(server, auth, task, addr, regionp, objectsp, objectsCntp)
    mach_port_t server;
    mach_port_t auth;
    mtask_t task;
    vm_offset_t addr;
    vm_info_region_t *regionp;
    vm_info_object_array_t *objectsp;
    unsigned int *objectsCntp;
{
    mach_port_t port;
    kern_return_t kr;

    kr = port_lookup(task, auth, mo_Info, &port);
    if (kr != KERN_SUCCESS) {
	if (kr == KERN_INVALID_NAME)
	    kr = KERN_INVALID_TASK;
	return kr;
    }

    kr = mach_vm_region_info(port, addr, regionp, objectsp, objectsCntp);
    port_consume(port);
    if (kr != KERN_SUCCESS)
	return kr;

    auth_consume(auth);
    return KERN_SUCCESS;
}

kern_return_t
do_host_stack_usage(server, auth, host,
		    reserved, total, space, resident, maxusage, maxstack)
    mach_port_t server;
    mach_port_t auth;
    mhost_t host;
    vm_size_t *reserved;
    unsigned int *total;
    vm_size_t *space;
    vm_size_t *resident;
    vm_size_t *maxusage;
    vm_offset_t *maxstack;
{
    mach_port_t port;
    kern_return_t kr;

    kr = port_lookup(host, auth, mo_Info, &port);
    if (kr != KERN_SUCCESS) {
	if (kr == KERN_INVALID_NAME)
	    kr = KERN_INVALID_HOST;
	return kr;
    }

    kr = host_stack_usage(port, reserved, total, space, resident,
			  maxusage, maxstack);
    port_consume(port);
    if (kr != KERN_SUCCESS)
	return kr;

    auth_consume(auth);
    return KERN_SUCCESS;
}

kern_return_t
do_processor_set_stack_usage(server, auth, pset,
			     total, space, resident, maxusage, maxstack)
    mach_port_t server;
    mach_port_t auth;
    mprocessor_set_t pset;
    unsigned int *total;
    vm_size_t *space;
    vm_size_t *resident;
    vm_size_t *maxusage;
    vm_offset_t *maxstack;
{
    mach_port_t port;
    kern_return_t kr;

    kr = port_lookup(pset, auth, mo_Info, &port);
    if (kr != KERN_SUCCESS) {
	if (kr == KERN_INVALID_NAME)
	    kr = KERN_INVALID_PROCESSOR_SET_NAME;
	return kr;
    }

    kr = processor_set_stack_usage(port, total, space, resident,
				   maxusage, maxstack);
    port_consume(port);
    if (kr != KERN_SUCCESS)
	return kr;

    auth_consume(auth);
    return KERN_SUCCESS;
}

kern_return_t
do_host_zone_info(server, auth, host,
		  names, namesCnt, info, infoCnt)
    mach_port_t server;
    mach_port_t auth;
    mhost_t host;
    zone_name_array_t *names;
    unsigned int *namesCnt;
    zone_info_array_t *info;
    unsigned int *infoCnt;
{
    mach_port_t port;
    kern_return_t kr;

    kr = port_lookup(host, auth, mo_Info, &port);
    if (kr != KERN_SUCCESS) {
	if (kr == KERN_INVALID_NAME)
	    kr = KERN_INVALID_HOST;
	return kr;
    }

    kr = host_zone_info(port, names, namesCnt, info, infoCnt);
    port_consume(port);
    if (kr != KERN_SUCCESS)
	return kr;

    auth_consume(auth);
    return KERN_SUCCESS;
}
