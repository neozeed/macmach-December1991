/* 
 * Mach Operating System
 * Copyright (c) 1989 Carnegie-Mellon University
 * Copyright (c) 1988 Carnegie-Mellon University
 * Copyright (c) 1987 Carnegie-Mellon University
 * All rights reserved.  The CMU software License Agreement specifies
 * the terms and conditions for use and redistribution.
 */
/*
 * HISTORY
 * $Log:	po_utils.c,v $
 * Revision 2.1  90/10/27  20:45:42  dpj
 * Moving under MACH3 source control
 * 
 * Revision 1.10.2.1  90/08/02  20:23:41  dpj
 * 	Use explicit port set instead of port_enable/port_disable.
 * 	[90/06/03  17:40:59  dpj]
 * 
 * Revision 1.10  89/05/02  11:15:37  dpj
 * 	Fixed all files to conform to standard copyright/log format
 * 
 * Revision 1.9  89/04/24  20:41:38  dpj
 * 	Changes from NeXT as of 88/09/30
 * 	[89/04/19  17:56:35  dpj]
 * 
 *  8-Jun-87  Robert Sansom (rds) at Carnegie Mellon University
 *	Encrypt/decrypt using RO Keys according to the param.crypt_algorithm.
 *
 * 28-Apr-87  Robert Sansom (rds) at Carnegie Mellon University
 *	Added po_utils_init - allocates a port for po_port_deallocate.
 *	Replaced fprintf by ERROR and DEBUG/LOG macros.
 *
 *  2-Feb-87  Robert Sansom (rds) at Carnegie Mellon University
 *	Added po_check_ro_key, po_create_ro_key.
 *	Split into po_utils. and po_handler.c.  Added po_port_deallocate.
 *
 * 27-Jan-87  Robert Sansom (rds) at Carnegie Mellon University
 *	Moved po_request_token to here from portops.c
 *
 * 12-Dec-86  Robert Sansom (rds) at Carnegie Mellon University
 *	Started.
 *
 */
/*
 * po_utils.c
 *
 *
 */

#ifndef	lint
char po_utils_rcsid[] = "$Header: /afs/cs.cmu.edu/project/mach-2/rcs/pkg/netmsg/server/po_utils.c,v 2.1 90/10/27 20:45:42 dpj Exp $";
#endif not lint

/*
 * Utility functions for the port operations module.
 */


#include <mach.h>
#include <mach/boolean.h>
#include <mach/kern_return.h>
#include <mach/message.h>

#include "crypt.h"
#include "debug.h"
#include "ls_defs.h"
#include "multperm.h"
#include "netmsg.h"
#include "nm_defs.h"
#include "po_defs.h"
#include "uid.h"

static port_t deallocate_port;



/*
 * po_utils_init
 *	allocates a port for use by po_port_deallocate.
 *
 * Returns:
 *	TRUE or FALSE.
 *
 */
PUBLIC boolean_t po_utils_init()
BEGIN("po_utils_init")
    kern_return_t	kr;

    if ((kr = port_allocate(task_self(), &deallocate_port)) != KERN_SUCCESS) {
	ERROR((msg, "po_utils_init.port_allocate fails, kr = %d.", kr));
	RETURN(FALSE);
    }

    RETURN(TRUE);

END



#if	USE_KEYMAN
/*
 * po_check_ro_key
 *	Checks an RO key received against the token stored in a port record.
 *
 * Parameters:
 *	port_rec_ptr	: pointer to record for port for which key is to be checked.
 *	secure_info_ptr	: pointer to RO key to be checked.
 *
 * Design:
 *	Tries to decrypt the stored token using the inverse of the RO key.
 *	Checks the decrypted value against what we already know.
 *
 * Returns:
 *	TRUE or FALSE.
 *
 */
PUBLIC boolean_t po_check_ro_key(port_rec_ptr, secure_info_ptr)
port_rec_ptr_t		port_rec_ptr;
secure_info_ptr_t	secure_info_ptr;
BEGIN("po_check_ro_key")
    key_t	dkey;
    key_t	token;

    dkey = secure_info_ptr->si_key;
#if	USE_MULTPERM
    if (param.crypt_algorithm == CRYPT_MULTPERM) invert_key(&dkey);
#endif	USE_MULTPERM
    token = port_rec_ptr->portrec_secure_info.si_token;

    if (CHECK_DECRYPT_ALGORITHM(param.crypt_algorithm)) {
	(void)crypt_functions[param.crypt_algorithm].decrypt(dkey, (pointer_t)&token.key_longs[0], 16); 
    }
    else {
	ERROR((msg, "po_check_ro_key: illegal decryption algorithm.", param.crypt_algorithm));
	RETURN(FALSE);
    }

    if (token.key_longs[0] != token.key_longs[3]) RETURN(FALSE);
    if (token.key_longs[0] != port_rec_ptr->portrec_random) RETURN(FALSE);
    if (token.key_longs[1] != port_rec_ptr->portrec_network_port.np_sid.np_uid_high) RETURN(FALSE);
    if (token.key_longs[2] != port_rec_ptr->portrec_network_port.np_sid.np_uid_low) RETURN(FALSE);

    RETURN(TRUE);

END



/*
 * po_create_ro_key
 *	Creates a key representing receive/ownership rights for a network port.
 *
 * Parameters:
 *	port_rec_ptr	: pointer to record for port for which key is to be created.
 *
 * Design:
 *	Creates a multperm key from random numbers.
 *
 */
PUBLIC void po_create_ro_key(port_rec_ptr)
port_rec_ptr_t		port_rec_ptr;
BEGIN("po_create_ro_key")
    unsigned long	key_temp;
    int			i;

    for (i = 0; i < 4; i++) {
	do {
	    key_temp = uid_get_new_uid();
	    key_temp = minverse((unsigned int)key_temp);
	} while (key_temp == 0);
	port_rec_ptr->portrec_secure_info.si_key.key_longs[i] = key_temp;
    }

    RET;
END

#endif	USE_KEYMAN



/*
 * po_create_token
 *	Creates a new token of authenticity of a receiver or an owner.
 *
 * Parameters:
 *	port_rec_ptr	: pointer to the record of the port for which the token is to be created
 *	token_ptr	: pointer to the token to be created
 *
 * Returns:
 *	the random number that was used to construct the new token.
 *
 * Design:
 *	Encrypt the SID of the network port plus a 32bit random number with the RO key.
 *
 * Note:
 *	The token is [X,SID,X] so that we can encrypt in multiples of 8 bytes.
 *
 */
EXPORT long po_create_token(port_rec_ptr, token_ptr)
port_rec_ptr_t		port_rec_ptr;
secure_info_ptr_t	token_ptr;
BEGIN("po_create_token")
    long	x;
    key_t	ekey;

    x = uid_get_new_uid();
    /*
     * Fill in the token.
     */
    token_ptr->si_token.key_longs[0] = x;
    token_ptr->si_token.key_longs[1] = port_rec_ptr->portrec_network_port.np_sid.np_uid_high;
    token_ptr->si_token.key_longs[2] = port_rec_ptr->portrec_network_port.np_sid.np_uid_low;
    token_ptr->si_token.key_longs[3] = x;

    /*
     * Encrypt the token.
     */
    ekey = port_rec_ptr->portrec_secure_info.si_key;
    if (CHECK_ENCRYPT_ALGORITHM(param.crypt_algorithm)) {
	(void)crypt_functions[param.crypt_algorithm].encrypt
				(ekey, (pointer_t)&token_ptr->si_token.key_longs[0], 16); 
    }
    else {
	ERROR((msg, "po_create_token: illegal decryption algorithm.", param.crypt_algorithm));
	RETURN(0);
    }

    RETURN(x);

END



/*
 * po_port_deallocate
 *	deallocate a port locally but retain send rights to it.
 *
 * Parameters:
 *	lport	: the local port to be deallocated.
 *
 * Design:
 *	check that we have either receive or ownership rights to this port but not both.
 *	If so, send a message to ourselves to retain send rights to this port;
 *	otherwise do not send a message to ourselves.
 *
 * Note:
 *	we rely on the checkups module to later determine that this port is invalid.
 *
 */
EXPORT void po_port_deallocate(lport)
port_t	lport;
BEGIN("po_port_deallocate")
    msg_header_t	send_rights_message;
    kern_return_t	kr;
    boolean_t		enabled, ownership, receive_rights;
    int			num_msgs, backlog;

    if ((kr = port_status(task_self(), lport, &enabled, &num_msgs, &backlog,
			&ownership, &receive_rights)) != KERN_SUCCESS)
    {
	ERROR((msg, "po_port_deallocate.port_status fails, kr = %d.", kr));
	RET;
    }
#ifdef lint
    enabled; num_msgs; backlog;
#endif lint

    if (ownership && receive_rights) {
	LOG1(TRUE, 5, 1130, lport);
	/*
 	 * Just deallocate the port and exit.
	 */
	if ((kr = port_deallocate(task_self(), lport)) != KERN_SUCCESS) {
	    ERROR((msg, "po_port_deallocate.port_deallocate fails, kr = %d.", kr));
	}
	RET;
    }

    send_rights_message.msg_simple = TRUE;
    send_rights_message.msg_size = sizeof(msg_header_t);
    send_rights_message.msg_type = MSG_TYPE_NORMAL;
    send_rights_message.msg_local_port = lport;
    send_rights_message.msg_remote_port = deallocate_port;
    send_rights_message.msg_id = 0;
    if ((kr = msg_send(&send_rights_message, MSG_OPTION_NONE, 0)) != KERN_SUCCESS) {
	ERROR((msg, "po_port_deallocate.msg_send fails, kr = %d.", kr));
	RET;
    }

    if ((kr = port_deallocate(task_self(), lport)) != KERN_SUCCESS) {
	ERROR((msg, "po_port_deallocate.port_deallocate fails, kr = %d.", kr));
    }

    send_rights_message.msg_local_port = deallocate_port;
    send_rights_message.msg_size = sizeof(msg_header_t);
    if ((kr = msg_receive(&send_rights_message, MSG_OPTION_NONE, 0)) != KERN_SUCCESS) {
	ERROR((msg, "po_port_deallocate.msg_receive fails, kr = %d.", kr));
	RET;
    }

    if (lport != send_rights_message.msg_remote_port) {
	LOG2(TRUE, 5, 1131, lport, send_rights_message.msg_remote_port);
    }

    RET;

END
