/* 
 **********************************************************************
 * Mach Operating System
 * Copyright (c) 1987 Carnegie-Mellon University
 * All rights reserved.  The CMU software License Agreement specifies
 * the terms and conditions for use and redistribution.
 */
/*
 **********************************************************************
 * HISTORY
 * $Log:	esp_sendprp.c,v $
 * Revision 2.1  89/08/04  14:20:30  rwd
 * Created.
 * 
 *  1-Jul-87  Daniel Julin (dpj) at Carnegie-Mellon University
 *	Created from Stanford sources (as of June 87).
 *
 **********************************************************************
 */
 
#include "mach_vmtp.h"

/*
 * $Header: esp_sendprp.c,v 2.1 89/08/04 14:20:30 rwd Exp $
 */

/*
 * 	esp_sendprp.c		2/26/87
 *	Written by Erik Nordmark
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

#include "../net/if.h"
#include "../net/route.h"

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
#include "vmtp_group.h"
#include "vmtp_buf.h"
#include "esp.h"
#include "esp_ip.h"
#include "esp_var.h"
#include "esp_cache.h"

/* 
 * Send a probe response. ei is passed to ip_output which will free it.
 * len is the length of the data portion of the packet i.e. without
 * the esp(ip) header.
 */
esp_sendprp(csr, ei, len, encryptqual)
	register struct vmtpcsr *csr;
	register struct espiphdr *ei;
	int len;
	u_long encryptqual;
{
	struct mbuf *m;
	int error;


#ifdef ESP_DEBUG
	printf("esp_sendprp(): csr = 0x%x, ei = 0x%x, len = %d\n", 
		csr, ei, len);
#endif

	setei_fnctcode(ei, VMTP_PROBERESP);
	setei_inpktgap(ei, VMTP_INPKTGAP);
	setei_priority(ei, VPR_IMPORTANT);
	ei->ei_addr = ei->ei_src;	/* noop */
	if (vmtp_localaddr(ei->ei_addr)) {
		vmtp_sendlocalpacket(csr, ei, sizeof(struct espiphdr)+len,
			encryptqual);
		return 0;
	}
	error = vmtp_sendpacket(csr, ei, sizeof(struct espiphdr)+len, 
					encryptqual);
	if (error) {
#ifdef VMTPDEBUG
		printf("esp_sendprp(): failed to send (%d)\n", error);
#endif
	}
	return(error);


} /* esp_sendprp */

#endif	MACH_VMTP
