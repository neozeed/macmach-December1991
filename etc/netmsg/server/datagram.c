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
 * $Log:	datagram.c,v $
 * Revision 2.1  90/10/27  20:43:52  dpj
 * Moving under MACH3 source control
 * 
 * Revision 1.17.1.1.1.2  90/08/02  20:21:15  dpj
 * 	Reorganized for new netipc interface and packet format.
 * 	[90/06/03  17:29:24  dpj]
 * 
 * Revision 1.17.1.1.1.1  90/07/30  13:52:00  dpj
 * 	Added filter priority argument in netipc_bind().
 * 
 * Revision 1.17  89/05/02  11:07:33  dpj
 * 	Fixed all files to conform to standard copyright/log format
 * 
 * Revision 1.16  89/04/24  20:38:06  dpj
 * 	Changes from NeXT as of 88/09/30
 * 	[89/04/19  17:51:23  dpj]
 * 
 * 05-Sep-88  Avadis Tevanian (avie) at NeXT
 *	Added USE_DATAGRAM
 *
 * 23-Jun-88  Daniel Julin (dpj) at Carnegie-Mellon University
 *	Added a LOGCHECK.
 *
 * 26-Mar-88  Daniel Julin (dpj) at Carnegie-Mellon University
 *	Converted to use the new memory management module.
 *
 *  2-Jun-87  Robert Sansom (rds) at Carnegie Mellon University
 *	ip_id in packet header is set in netipc_send.
 *
 * 19-May-87  Robert Sansom (rds) at Carnegie Mellon University
 *	Increased the backlog on the datagram_listen_port.
 *	Added some statistics gathering.
 *
 * 23-Apr-87  Robert Sansom (rds) at Carnegie Mellon University
 *	Conditionally use thread_lock - ensures only one thread is executing.
 *	Added call to cthread_set_name.
 *	Replaced fprintf by ERROR and DEBUG/LOG macros.
 *
 *  6-Feb-87  Robert Sansom (rds) at Carnegie Mellon University
 *	Replaced SBUF_INIT by SBUF_SEG_INIT.
 *
 * 17-Dec-86  Robert Sansom (rds) at Carnegie Mellon University
 *	Added datagram_max_data_size; initialised from DATAGRAM_MAX_DATA_SIZE.
 *	Removed datagram_abort.
 *
 *  3-Nov-86  Robert Sansom (rds) at Carnegie Mellon University
 *	Started.
 *
 */
/*
 * datagram.c
 *
 *
 */
#ifndef	lint
char datagram_rcsid[] = "$Header: /afs/cs.cmu.edu/project/mach-2/rcs/pkg/netmsg/server/datagram.c,v 2.1 90/10/27 20:43:52 dpj Exp $";
#endif not lint

/*
 * Datagram interface to the network.
 */


#include "netmsg.h"
#include "nm_defs.h"

#if	USE_DATAGRAM

#include <mach.h>
#include <mach/message.h>
#include <cthreads.h>

#include "crypt.h"
#include "debug.h"
#include "datagram.h"
#include "disp_hdr.h"
#include "mem.h"
#include "netipc.h"
#include "network.h"
#include "nm_extra.h"
#include "sbuf.h"
#include "transport.h"
#include "uid.h"

typedef	struct datagram_packet {
	netipc_header_t		netipc_header;
	crypt_header_t		crypt_header;
	char			data[1];
} *datagram_packet_ptr_t;

static netipc_channel_ptr_t	datagram_channel;
static cthread_t		datagram_listen_thread;

PRIVATE void datagram_main();

int	datagram_max_data_size = NETIPC_MAX_DATA_SIZE - CRYPT_HEADER_SIZE - 8;


/*
 * datagram_init
 *	Initialises the datagram transport protocol.
 *
 * Results:
 *	FALSE : we failed to initialise the datagram transport protocol.
 *	TRUE  : we were successful.
 *
 * Side effects:
 *	Initialises the datagram protocol entry point in the switch array.
 *	Initialises the template for sending datagrams.
 *	Allocates the listener port and creates a thread to listen to the network.
 *
 */
EXPORT boolean_t datagram_init()
BEGIN("datagram_init")
    kern_return_t	kr;

    datagram_channel = netipc_bind(DATAGRAM_UDP_PORT,CRYPT_HEADER_SIZE,
							NETIPC_PRI_LOW);
    if (datagram_channel == 0) {
	    ERROR((msg, "datagram_init.netipc_bind fails"));
	    return(FALSE);
    }
    transport_switch[TR_DATAGRAM_ENTRY].send = datagram_send;

    /*
     * Now fork a thread to execute the receive loop of the 
     * datagram transport protocol.
     */
    datagram_listen_thread = cthread_fork((cthread_fn_t)datagram_main,
								(any_t)0);
    cthread_set_name(datagram_listen_thread, "datagram_main");
    cthread_detach(datagram_listen_thread);

    RETURN(TRUE);

END


/*
 * datagram_main
 *	Main loop of datagram transport protocol
 *	Waits for incoming packets contained in IPC messages
 *	and calls the dispatcher to handle them.
 *
 * Note:
 *	It is assumed that the data in the incoming packet is no longer
 *	needed by the higher-level routine to which it was given by the
 *	disp_inmsg_simple function after disp_inmsg_simple returns.
 *	In other words we can reuse the buffer for the next datagram.
 */
PRIVATE void datagram_main()
BEGIN("datagram_main")
    datagram_packet_ptr_t	in_pkt_ptr;
    kern_return_t	kr;
    sbuf_t		in_sbuf;
    sbuf_seg_t		in_sbuf_seg;

#if	LOCK_THREADS
    mutex_lock(thread_lock);
#endif	LOCK_THREADS

    MEM_ALLOCOBJ(in_pkt_ptr,datagram_packet_ptr_t,MEM_TRBUFF);

    SBUF_SEG_INIT(in_sbuf, &in_sbuf_seg);

    while (TRUE) {
	kr = netipc_receive(datagram_channel,in_pkt_ptr);
	if (kr != RCV_SUCCESS) {
	    ERROR((msg, "datagram_main.netipc_receive fails, kr = %d.", kr));
	}
	else {
	    int		data_size;
	    netaddr_t	source_host;
	    boolean_t	broadcast;
	    int		crypt_level;

	    INCSTAT(datagram_pkts_rcvd);
	    source_host = in_pkt_ptr->netipc_header.control.from;
	    broadcast = FALSE; /* XXX */
	    crypt_level = ntohl(in_pkt_ptr->crypt_header.ch_crypt_level);

	    if (crypt_level != CRYPT_DONT_ENCRYPT) {
		kr = crypt_decrypt_packet(in_pkt_ptr, crypt_level);
	    }
	    else kr = CRYPT_SUCCESS;

	    if (kr == CRYPT_SUCCESS) {
		data_size = ntohs(in_pkt_ptr->crypt_header.ch_data_size);
		SBUF_REINIT(in_sbuf);
		SBUF_APPEND(in_sbuf, in_pkt_ptr->data, data_size);
		kr = disp_indata_simple(0, (sbuf_ptr_t)&(in_sbuf), source_host, broadcast, crypt_level);
		if (kr != DISP_SUCCESS) {
		    LOG1(TRUE, 5, 1101, kr);
		}
	    }
	}
	LOGCHECK;
    }
END


/*
 * datagram_send
 *	Sends a datagram over the network.
 *
 * Parameters:
 *	to		: the host to which the datagram is to be sent.
 *	crypt_level	: whether this packet should be encrypted.
 *
 * Returns:
 *	TR_SUCCESS or a specific failure code.
 *
 * Note:
 *	cleanup is never called as this routine is guaranteed to have done
 *	with the input sbuf after it returns.
 *
 */
/* ARGSUSED */
EXPORT int datagram_send(client_id, data, to, service, crypt_level, cleanup)
int		client_id;
sbuf_ptr_t	data;
netaddr_t	to;
int		service;
int		crypt_level;
int		(*cleanup)();
BEGIN("datagram_send")
    datagram_packet_ptr_t	pkt_ptr;
    kern_return_t	kr;
    unsigned short	size;

    /*
     * Sanity check.
     */
    SBUF_GET_SIZE(*data,size);
    if (size > datagram_max_data_size) {
	LOG1(TRUE, 5, 1100, size);
	MEM_DEALLOCOBJ(pkt_ptr,MEM_TRBUFF);
	RETURN(DATAGRAM_TOO_LARGE);
    }

    /*
     * Copy the input sbuf into our local buffer.
     */
    MEM_ALLOCOBJ(pkt_ptr,datagram_packet_ptr_t,MEM_TRBUFF);
    SBUF_FLATTEN(data, pkt_ptr->data, size);

    /*
     * Fill in the netipc header.
     */
    netipc_prepare_pkt(datagram_channel,pkt_ptr,to,size + CRYPT_HEADER_SIZE)
    pkt_ptr->crypt_header.ch_crypt_level = htonl((unsigned long)crypt_level);
    pkt_ptr->crypt_header.ch_data_size = htons(size);

    /*
     * Check to see whether we should encrypt this datagram.
     */
    if (crypt_level != CRYPT_DONT_ENCRYPT) {
	kr = crypt_encrypt_packet((netipc_ptr_t)pkt_ptr, crypt_level);
    }
    else kr = CRYPT_SUCCESS;

    if (kr == CRYPT_SUCCESS) {
	/*
	 * Send the message to the kernel.
	 */
	if ((kr = netipc_send(datagram_channel,(netipc_ptr_t)pkt_ptr,CRYPT_NEED_CKSUM(pkt_ptr))) != SEND_SUCCESS) {
	    ERROR((msg, "datagram_send.netipc_send fails, kr = %1d.", kr));
	    MEM_DEALLOCOBJ(pkt_ptr,MEM_TRBUFF);
	    RETURN(TR_SEND_FAILURE);
	}
	else INCSTAT(datagram_pkts_sent);
    }
    else {
	MEM_DEALLOCOBJ(pkt_ptr,MEM_TRBUFF);
	RETURN(TR_CRYPT_FAILURE);
    }

    MEM_DEALLOCOBJ(pkt_ptr,MEM_TRBUFF);
    RETURN(TR_SUCCESS);
END

#else	USE_DATAGRAM
	/*
	 * Just a dummy to keep the loader happy.
	 */
static int	dummy;
#endif	USE_DATAGRAM
