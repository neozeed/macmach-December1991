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
 * $Log:	machid_internal.h,v $
 * Revision 2.4  91/08/30  14:51:42  rpd
 * 	Moved machid include files into the standard directory.
 * 
 * Revision 2.3  91/03/19  12:30:30  mrt
 * 	Changed to new copyright
 * 
 * Revision 2.2  90/09/12  16:31:43  rpd
 * 	Created.
 * 	[90/06/18            rpd]
 * 
 */

#ifndef	_MACHID_INTERNAL_H_
#define	_MACHID_INTERNAL_H_

#include <mach/kern_return.h>
#include <mach/port.h>
#include <servers/machid_types.h>

extern char *malloc();
extern void free();

typedef enum mach_op {
    mo_Port,		/* get the object's port - restricted */
    mo_Write,		/* "write" the object - restricted */
    mo_Read,		/* "read" the object - restricted */
    mo_Info		/* get "info" about the object - not restricted */
} mach_op_t;

extern mach_id_t name_lookup(/* port, type */);
extern kern_return_t port_lookup(/* name, auth, op, portp */);
extern mach_type_t type_lookup(/* name */);
extern void port_consume(/* port */);
extern void port_destroy(/* port */);
extern void assoc_create(/* name, atype, aname */);
extern mach_id_t assoc_lookup(/* name, atype */);
extern void auth_consume(/* port */);

#endif	_MACHID_INTERNAL_H_
