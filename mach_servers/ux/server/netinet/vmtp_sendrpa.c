/* 
 * Mach Operating System
 * Copyright (c) 1987 Carnegie-Mellon University
 * All rights reserved.  The CMU software License Agreement specifies
 * the terms and conditions for use and redistribution.
 */
/*
 **********************************************************************
 * HISTORY
 * $Log:	vmtp_sendrpa.c,v $
 * Revision 2.1  89/08/04  14:30:50  rwd
 * Created.
 * 
 * Revision 2.2  88/08/24  02:05:26  mwyoung
 * 	Corrected include file references.
 * 	[88/08/22  22:10:13  mwyoung]
 * 
 *
 *  1-Jul-87  Daniel Julin (dpj) at Carnegie-Mellon University
 *	Updated from new VMTP sources from Stanford (June 87).
 *
 *  3-Jun-87  Daniel Julin (dpj) at Carnegie-Mellon University
 *	Do not zero the dlvrmsk field in the packet if we are
 *	asking for a retry.
 *
 * 29-May-87  Daniel Julin (dpj) at Carnegie-Mellon University
 *	Created from Stanford sources.
 *
 **********************************************************************
 */
 
#include <cmucs.h>
#include "mach_vmtp.h"

/* 
 * $Header: vmtp_sendrpa.c,v 2.1 89/08/04 14:30:50 rwd Exp $
 */

/*
 * 	vmtp_sendrpa.c	0.1	10/19/86
 *	Written by Joyo Wijaya and Erik Nordmark
 */

#if	MACH_VMTP

#include "param.h"
#include "systm.h"
#include "mbuf.h"
#include "protosw.h"
#include "socket.h"
#include "socketvar.h"
#include "errno.h"
#include "time.h"
#include "kernel.h"

#include <net/if.h>
#include <net/route.h>

#include "in.h"
#include "in_pcb.h"
#include "in_systm.h"
#include "ip.h"
#include "ip_var.h"
#include "vmtp_so.h"
#include "vmtp.h"
#include "vmtp_ip.h"
#include "vmtp_var.h"
#include "vmtp_send.h"
#include "esp.h"

/*
 * Send a response acknowledge packet with the given code.
 * If a csr is given, a copy of the packet will be sent;
 * otherwise, the given packet will be passed to ip_output()
 * which will free it. Since, in the non-copy case, there might
 * be garbage at the end of the header portion of the packet, the
 * length has to be adjusted before it can be passed to sendpacket
 * for the checksum to get in the right place.
 */
vmtp_sendrpa(csr, vi, code)
	register struct vmtpcsr *csr;
	register struct vmtpiphdr *vi;
	u_long	code;
{
	struct mbuf *m;
	u_long encryptqual;
	int error;

#ifdef VMTPDEBUG
	printf("vmtp_sendrpa(0x%x, 0x%x, 0x%x)\n", (u_long)csr, 
		(u_long)vi, code);
#endif
	if (csr) {
		/*
		 * Make a copy.
		 */
		m = vmtp_copyvihdr(vi);
		if (m == NULL) {
#ifdef VMTPDEBUG
			printf("vmtp_sendrpa(): failed to send (ENOBUFS)\n");
#endif
			return(ENOBUFS);
		}
		vi = mtod(m, struct vmtpiphdr *);
		encryptqual = csr->vc_encryptqual;
	} else {
		/*
		 * Adjust the length of the first mbuf and free the rest
		 * Since the mbuf chain was just received m_pullup gurantees
		 * that the first mbuf contains at least the header.
		 */
		m = dtom(vi);
		if (m->m_len < sizeof(struct vmtpiphdr))
			panic("vmtp_sendrpa()");
		m->m_len = sizeof(struct vmtpiphdr);
		m_freem(m->m_next);
		m->m_next = NULL;
		encryptqual = ENCRYPT_NONE;
	}

#if	CMUCS
	if (code != VMTP_RETRY) {
		vi->vi_dlvrmsk = 0;
	}
#else	CMUCS
	vi->vi_dlvrmsk = 0;
#endif	CMUCS
	setvi_fnctcode(vi, VMTP_RESPACK);
	setvi_pgcount(vi, 0);
	setvi_control(vi, 0);
	vi->vi_code = code;
	vi->vi_addr = vi->vi_src; 	/* noop */

	error = vmtp_sendpacket(csr, vi, sizeof(struct vmtpiphdr),
					encryptqual);
	if (error) {
#ifdef VMTPDEBUG
		printf("vmtp_sendrpa(): failed to send (%d)\n", error);
#endif
	}
	return(error);
} /* vmtp_sendrpa */

#endif	MACH_VMTP
