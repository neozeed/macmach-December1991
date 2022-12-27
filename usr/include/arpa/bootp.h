/*
 * Bootstrap Protocol (BOOTP).  RFC 951.
 *
 * HISTORY
 * $Log:	bootp.h,v $
 * Revision 2.4  89/08/24  16:26:08  moore
 * 	I hate the workon workoff tools.  They screw things up.
 * 	And I have to unscrew them.
 * 	[89/08/24  16:25:07  moore]
 * 
 * Revision 2.3  89/08/24  15:43:10  moore
 * 	Created.
 * 	[89/08/24  15:25:56  moore]
 * 
 * Revision 2.2  89/08/24  15:26:34  moore
 * 	Created.
 * 	[89/08/24  15:25:56  moore]
 * 
 *
 */

#define BOOTREPLY	2
#define BOOTREQUEST	1
struct bootp {
	unsigned char	bp_op;		/* packet opcode type */
	unsigned char	bp_htype;	/* hardware addr type */
	unsigned char	bp_hlen;	/* hardware addr length */
	unsigned char	bp_hops;	/* gateway hops */
	unsigned long	bp_xid;		/* transaction ID */
	unsigned short	bp_secs;	/* seconds since boot began */
	unsigned short	bp_unused;
	struct in_addr	bp_ciaddr;	/* client IP address */
	struct in_addr	bp_yiaddr;	/* 'your' IP address */
	struct in_addr	bp_siaddr;	/* server IP address */
	struct in_addr	bp_giaddr;	/* gateway IP address */
	unsigned char	bp_chaddr[16];	/* client hardware address */
	unsigned char	bp_sname[64];	/* server host name */
	unsigned char	bp_file[128];	/* boot file name */
	unsigned char	bp_vend[64];	/* vendor-specific area */
};

/*
 * UDP port numbers, server and client.
 */
#define	IPPORT_BOOTPS		67
#define	IPPORT_BOOTPC		68
