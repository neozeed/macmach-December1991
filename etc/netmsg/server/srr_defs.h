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
 * $Log:	srr_defs.h,v $
 * Revision 2.1  90/10/27  20:46:30  dpj
 * Moving under MACH3 source control
 * 
 * Revision 1.11.2.1  90/08/02  20:24:13  dpj
 * 	Reorganized for new netipc interface and packet format.
 * 	[90/06/03  17:43:14  dpj]
 * 
 * Revision 1.11  89/05/02  11:17:47  dpj
 * 	Fixed all files to conform to standard copyright/log format
 * 
 * 26-Mar-88  Daniel Julin (dpj) at Carnegie-Mellon University
 *	Converted to use the new memory management module.
 *
 *  2-Jun-87  Robert Sansom (rds) at Carnegie Mellon University
 *	Increased the size of the srr_pkt_data by 8 bytes to allow for crypt padding.
 *
 * 30-May-87  Robert Sansom (rds) at Carnegie Mellon University
 *	Merged in external definitions from srr_utils and srr_handler.
 *
 * 18-May-87  Robert Sansom (rds) at Carnegie Mellon University
 *	Made srr_pkt_type field in srr_packet_t an unsigned quantity.
 *	Made fields in srr_uid_t unsigned quantities.
 *
 * 22-Apr-87  Robert Sansom (rds) at Carnegie Mellon University
 *	Removed the shi_sbuf_ptr from the srr_host_info record.
 *	Made the mutex and timer inline in srr_host_info.
 *
 *  1-Jan-87  Robert Sansom (rds) at Carnegie Mellon University
 *	Added SRR_BCAST_REQUEST and SRR_BCAST_RESPONSE.
 *	Added information to srr_host_info_t for handling broadcast requests.
 *
 * 22-Dec-86  Robert Sansom (rds) at Carnegie Mellon University
 *	Dont use the ip_id in the srr template.
 *
 * 25-Nov-86  Robert Sansom (rds) at Carnegie Mellon University
 *	Added sbuf field to host_info record.
 *
 *  6-Nov-86  Robert Sansom (rds) at Carnegie-Mellon University
 *	Started.
 *
 */
/*
 * srr_defs.h
 *
 *
 * $Header: /afs/cs.cmu.edu/project/mach-2/rcs/pkg/netmsg/server/srr_defs.h,v 2.1 90/10/27 20:46:30 dpj Exp $
 *
 */

/*
 * Internal definitions for the simple request-response transport protocol.
 */


#ifndef	_SRR_DEFS_
#define	_SRR_DEFS_

/*
 * Definitions for packets sent out over the network.
 */
#include "mem.h"
#include "netipc.h"
#include "crypt.h"

typedef struct srr_uid_ {
    unsigned long		srr_uid_incarnation;
    unsigned long		srr_uid_sequence_no;
} srr_uid_t;

#define HTON_SRR_UID(uid) {						\
    (uid).srr_uid_incarnation = htonl((uid).srr_uid_incarnation);	\
    (uid).srr_uid_sequence_no = htonl((uid).srr_uid_sequence_no);	\
}

#define NTOH_SRR_UID(uid) {						\
    (uid).srr_uid_incarnation = ntohl((uid).srr_uid_incarnation);	\
    (uid).srr_uid_sequence_no = ntohl((uid).srr_uid_sequence_no);	\
}


#define SRR_HEADER_SIZE		(sizeof(long) + sizeof(srr_uid_t))
#define SRR_MAX_DATA_SIZE	(NETIPC_MAX_DATA_SIZE - SRR_HEADER_SIZE - CRYPT_HEADER_SIZE - 8)

typedef struct srr_packet_ {
    netipc_header_t	srr_pkt_header;
    crypt_header_t	crypt_header;
    unsigned long	srr_pkt_type;
    srr_uid_t		srr_pkt_uid;
    char		srr_pkt_data[SRR_MAX_DATA_SIZE+8];
} srr_packet_t, *srr_packet_ptr_t;

#define SRR_NULL_PACKET	((srr_packet_ptr_t)0)

/*
 * SRR Packet Types
 */
#define SRR_REQUEST		0
#define SRR_RESPONSE		1
#define SRR_BCAST_REQUEST	2
#define SRR_BCAST_RESPONSE	3
#define SRR_CRYPT_FAILURE	4

/*
 * Channel for srr communications.
 */
extern netipc_channel_ptr_t		srr_channel;

/*
 * Macro for filling in an srr packet header.
 */

#define SRR_SET_PKT_HEADER(pkt_ptr,size,dest,type,crypt_level) {	\
    netipc_prepare_pkt(srr_channel,pkt_ptr,dest,			\
			size + SRR_HEADER_SIZE + CRYPT_HEADER_SIZE);	\
    (pkt_ptr)->srr_pkt_type = (long)htonl((unsigned long)(type));	\
    (pkt_ptr)->crypt_header.ch_crypt_level =				\
				htonl((unsigned long)crypt_level);	\
    (pkt_ptr)->crypt_header.ch_data_size =				\
			htons((unsigned short)(size) + SRR_HEADER_SIZE);\
}




/*
 * Definitions for information maintained about hosts.
 */
#include <cthreads.h>
#include "sbuf.h"
#include "timer.h"

/*
 * The queue of requests awaiting transmission.
 */
typedef struct srr_request_q_ {
    struct srr_request_q_	*srq_next;
    srr_packet_ptr_t		srq_request_packet;
    netaddr_t			srq_destination;
    int				srq_client_id;
    int				(*srq_cleanup)();
} srr_request_q_t, *srr_request_q_ptr_t;

#define SRR_NULL_Q	((srr_request_q_ptr_t)0)

/*
 * Information about one host.
 */
typedef struct srr_host_info_ {
    netaddr_t		shi_host_id;
    struct mutex	shi_lock;
    /* Information about outstanding requests. */
    int			shi_request_status;
    int			shi_request_tries;
    srr_uid_t		shi_request_uid;
    srr_request_q_ptr_t	shi_request_q_head;
    srr_request_q_ptr_t	shi_request_q_tail;
    struct timer	shi_timer;
    /* Information for a response. */
    srr_uid_t		shi_response_uid;
    srr_packet_ptr_t	shi_response_packet;
    /* Information for a response to a broadcast request. */
    srr_uid_t		shi_bcast_response_uid;
    srr_packet_ptr_t	shi_bcast_response_packet;
} srr_host_info_t, *srr_host_info_ptr_t;

#define SRR_NULL_HOST_INFO	((srr_host_info_ptr_t)0)

/*
 * Status codes for host records.
 */
#define SRR_INACTIVE			0
#define SRR_AWAITING_RESPONSE		1
#define SRR_HAVE_RESPONSE		2
#define SRR_LOCAL_CRYPT_FAILURE		3
#define SRR_REMOTE_CRYPT_FAILURE	4
#define SRR_LOCAL_ABORT			5
#define SRR_REMOTE_ABORT		5

/*
 * The retry function.
 */
extern int srr_retry();


/*
 * External definitions of funtions implemented by srr_handler.c
 */

extern srr_packet_ptr_t srr_handle_crypt_failure();
/*
srr_packet_ptr_t in_packet_ptr;
int crypt_level;
*/

extern srr_packet_ptr_t srr_handle_request();
/*
srr_packet_ptr_t	in_packet_ptr;
int			data_size;
int			crypt_level;
boolean_t		broadcast;
*/

extern srr_packet_ptr_t srr_handle_response();
/*
srr_packet_ptr_t	in_packet_ptr;
int			data_size;
int			crypt_level;
boolean_t		broadcast;
*/

extern void srr_process_queued_request();
/*
srr_host_info_ptr_t	host_info;
*/

extern void srr_send_crypt_failure();
/*
srr_packet_ptr_t in_packet_ptr;
int crypt_level;
*/


/*
 * External definitions of funtions implemented by srr_utils.c
 */

void srr_utils_init();

srr_host_info_ptr_t srr_hash_lookup();
/*
netaddr_t host_id;
*/

srr_host_info_ptr_t srr_hash_enter();
/*
netaddr_t host_id;
*/

void srr_enqueue();
/*
srr_request_q_ptr_t	request_ptr;
srr_host_info_ptr_t	host_record;
*/

srr_request_q_ptr_t srr_dequeue();
/*
srr_host_info_ptr_t	host_record;
*/

/*
 * Memory management definitions.
 */
extern mem_objrec_t		MEM_SRRREQ;


#endif	_SRR_DEFS_
