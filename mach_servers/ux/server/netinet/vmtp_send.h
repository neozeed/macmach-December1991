/* 
 * Mach Operating System
 * Copyright (c) 1987 Carnegie-Mellon University
 * All rights reserved.  The CMU software License Agreement specifies
 * the terms and conditions for use and redistribution.
 */
/*
 **********************************************************************
 * HISTORY
 * $Log:	vmtp_send.h,v $
 * Revision 2.1  89/08/04  14:30:35  rwd
 * Created.
 * 
 *  1-Jul-87  Daniel Julin (dpj) at Carnegie-Mellon University
 *	Updated from new VMTP sources from Stanford (June 87).
 *
 * 29-May-87  Daniel Julin (dpj) at Carnegie-Mellon University
 *	Created from Stanford sources.
 *
 **********************************************************************
 */
 

/*
 * $Header: vmtp_send.h,v 2.1 89/08/04 14:30:35 rwd Exp $
 */

/*
 * 	vmtp_send.h
 *	Written by Erik Nordmark
 */


/*
 * Flags for sendpg()
 */
#define VMTP_TRANSMIT	0	/* passed to sendpg */
#define VMTP_RETRANSMIT	1	/* passed to sendpg */
#define vmtp_transmit(f) ((f)==VMTP_TRANSMIT)
#define vmtp_retransmit(f) ((f)==VMTP_RETRANSMIT)

#ifdef KERNEL
struct mbuf *vmtp_copyvihdr(); 		/* placing the header at the end of
					 * the mbuf so that an ethernet header
					 * can fit in front of it. */
#endif




