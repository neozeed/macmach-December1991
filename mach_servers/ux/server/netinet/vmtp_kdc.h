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
 * $Log:	vmtp_kdc.h,v $
 * Revision 2.1  89/08/04  14:27:46  rwd
 * Created.
 * 
 *  1-Jul-87  Daniel Julin (dpj) at Carnegie-Mellon University
 *	Created from Stanford sources (as of June 87).
 *
 **********************************************************************
 */
 
/*
 * $Header: vmtp_kdc.h,v 2.1 89/08/04 14:27:46 rwd Exp $
 */

/*
 * 	vmtp_kdc.h			5/21/87
 *	Written by Erik Nordmark
 *
 * Defines the protocol being used between the secure library and the KDC.
 */


/*
 * The KDC group
 */
#define VMTP_KDC_FLTM	0x48000010	
#define VMTP_KDC_HOST	VMTP_HOSTGROUP	


/* 
 * Form of MCB used for KDC requests.
 */
struct kdcreq {
	int		krq_cksum;
	int	 	krq_tmstamp;
	struct princid	krq_serverp;	/* principal id; 2 words */
	struct princid	krq_clientp;	/* principal id; 2 words */
	int		krq_res;	/* reserved -  not used */
};

/* 
 * Form of MCB used for KDC responses.
 */
struct kdcresp {
	int		krp_cksum;
	int	 	krp_tmstamp;
	int		krp_timelimit;
	int		krp_encryptqual;
	union vmtpkey	krp_key;	/* 2 words */
	int		krp_segsize;	/* vmtp segment size */
};

/*
 * Request codes
 */
#define KDC_GETKEY		(VU_WRA | 0x010000)


/* 
 * Response codes
 */
#define KDC_OK			0x0		/* same as VMTP_OK */
#define KDC_UNKNOWNREQ		0x010001
#define KDC_UNKNOWNCLIENT	0x010002
#define KDC_UNKNOWNSERVER	0x010003



