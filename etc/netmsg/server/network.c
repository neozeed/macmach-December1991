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
 * $Log:	network.c,v $
 * Revision 2.1  90/10/27  20:45:16  dpj
 * Moving under MACH3 source control
 * 
 * Revision 1.12.1.3  90/10/21  21:54:04  dpj
 * 	Removed use of 128.2.0.0 as default broadcast address (from MtXinu).
 * 	Fixed include files for MACH3.
 * 
 * Revision 1.12.1.2  90/10/07  13:49:55  dpj
 * 	Upgraded to work with !MACH3, MACH3_VUS and MACH3_SA.
 * 
 * Revision 1.12.1.1  90/07/30  14:02:15  dpj
 * 	Fixed generation of IP addresses for self, localhost and broadcast
 * 	under MACH3.
 * 
 * Revision 1.12  89/05/02  11:13:50  dpj
 * 	Fixed all files to conform to standard copyright/log format
 * 
 * 23-Jun-88  Daniel Julin (dpj) at Carnegie-Mellon University
 *	Obtain the broadcast address from a network interface when possible.
 *
 * 25-Oct-87  Daniel Julin (dpj) at Carnegie-Mellon University
 *	Redone network_init() to check for the presence of a network
 *	and obtain the broadcast address from /etc/nmbroadcast if needed.
 *
 * 29-Apr-87  Robert Sansom (rds) at Carnegie Mellon University
 *	Replaced fprintf by ERROR macro.
 *
 * 22-Dec-86  Robert Sansom (rds) at Carnegie Mellon University
 *	Added udp_checksum and last_ip_id.
 *
 *  3-Nov-86  Robert Sansom (rds) at Carnegie Mellon University
 *	Started.
 *
 */
/*
 * network.c
 *
 *
 */
#ifndef	lint
char network_rcsid[] = "$Header: /afs/cs.cmu.edu/project/mach-2/rcs/pkg/netmsg/server/network.c,v 2.1 90/10/27 20:45:16 dpj Exp $";
#endif not lint
/*
 * Random things for the network level.
 */


#include "netmsg.h"

#ifndef	KERNEL_FEATURES
#define	KERNEL_FEATURES	1
#endif	KERNEL_FEATURES

#include <mach.h>
#include <stdio.h>
#include <errno.h>

#if	USE_NETIPC_MACH3
#else	USE_NETIPC_MACH3
#if	MACH3_UNIX || !defined(MACH3)
#include <sys/ioctl.h>
#else	MACH3_UNIX || !defined(MACH3)
#include <unix_include/sys/ioctl.h>
#endif	MACH3_UNIX || !defined(MACH3)
#include <netdb.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <net/if.h>
#include <sys/file.h>
#endif	USE_NETIPC_MACH3

#include "network.h"
#include "nm_defs.h"
#include "nm_extra.h"
#include "uid.h"

char		my_host_name[HOST_NAME_SIZE];
netaddr_t	my_host_id;
netaddr_t	broadcast_address;
netaddr_t	localhost_id;
short		last_ip_id;

extern int	errno;

#if	USE_NETIPC_MACH3
extern netaddr_t	mach3_ip_addr;
#endif	USE_NETIPC_MACH3


/*
 * init_network
 *	Initialises some things for the network-level interface.
 *
 * Returns:
 *	TRUE or FALSE.
 *
 * Side effects:
 *	Sets up my_host_name, my_host_id, last_ip_id and broadcast_address.
 *
 */
PUBLIC boolean_t network_init()
BEGIN("network_init")
#if	USE_NETIPC_MACH3
	localhost_id = ntohl(0x7f000001);
	last_ip_id = (short)uid_get_new_uid();
	strcpy(my_host_name,"(Standalone netmsgserver)");
	if (mach3_ip_addr != 0) {
		my_host_id = mach3_ip_addr;
		if ((ntohl(my_host_id) & 0x80000000) == 0) {
			/* class A */
			broadcast_address = my_host_id | ntohl(0xffffff);
		} else
		if ((ntohl(my_host_id) & 0xc0000000) == 0x80000000) {
			/* class B */
			broadcast_address = my_host_id | ntohl(0xffff);
		} else
		if ((ntohl(my_host_id) & 0xe0000000) == 0xc0000000) {
			/* class C */
			broadcast_address = my_host_id | ntohl(0xff);
		} else {
			/* class D */
			broadcast_address = my_host_id | ntohl(0xfffffff);
			ERROR((msg,"Warning: class D network ???"));
		}
		broadcast_address = my_host_id | ntohl(0xffff);
		ERROR((msg,"Broadcast address: 0x%x",broadcast_address));
		DEBUG1(TRUE,0,2702,broadcast_address);
		RETURN(TRUE);
	} else {
		my_host_id = localhost_id;
		broadcast_address = my_host_id + 1;
		ERROR((msg,
			"No IP address specified -- disabling the network"));
		RETURN(FALSE);
	}
#else	USE_NETIPC_MACH3
	struct hostent		*hp;
	ip_addr_t		temp_address;
	int			s;
	char			buf[1000];
	struct ifconf		ifc;
        struct ifreq		*ifr;
	netaddr_t		cur_if_addr;
	struct sockaddr_in	*cur_if_sockaddr;
	int			n;
	boolean_t		found_bcast = FALSE;
	char			outstr[100];
	FILE			*nm_fp;

	/*
	 * Make sure all important variables get a sensible value no
	 * matter what happens.
	 */
	localhost_id = ntohl(0x7f000001);
	my_host_id = 1;
	broadcast_address = 2;
	last_ip_id = (short)uid_get_new_uid();

	/*
	 * Find the local IP address.
	 */
	if ((gethostname(my_host_name, HOST_NAME_SIZE)) != 0) {
		panic("network_init.gethostname");
	}

	hp = gethostbyname(my_host_name);

	if (hp == 0) {
		ERROR((msg, "network_init.gethostbyname fails, errno=%d",errno));
		RETURN(FALSE);
	} else {
		my_host_id = *(netaddr_t *)(hp->h_addr);
	}

	/*
	 * Check that the network is up.
	 */
	s = socket(AF_INET,SOCK_DGRAM,0);
	if (s <= 0) {
		ERROR((msg,"network_init.socket fails, errno=%d",errno));
		RETURN(FALSE);
	}
	ifc.ifc_len = sizeof(buf);
	ifc.ifc_buf = buf;
	if (ioctl(s,SIOCGIFCONF,(char *)&ifc) < 0 ) {
		ERROR((msg,
			"network_init.ioctl(SIOCGIFCONF) fails, errno=%d",errno));
		close(s);
		RETURN(FALSE);
	}
	DEBUG1(TRUE,0,2700,(ifc.ifc_len / sizeof(struct ifreq)));
	ifr = ifc.ifc_req;
	for (n = ifc.ifc_len / sizeof(struct ifreq); n > 0; n--) {
		cur_if_sockaddr = (struct sockaddr_in *)&(ifr->ifr_addr);
		cur_if_addr = cur_if_sockaddr->sin_addr.s_addr;
		if (ioctl(s,SIOCGIFFLAGS,ifr) < 0) {
			ERROR((msg,
				"network_init.ioctl(SIOCGIFFLAGS) fails, errno=%d",
									errno));
			RETURN(FALSE);
		}
		sprintf(outstr,"   interface \"%.50s\" flags=0x%x, addr=0x%x",
					ifr->ifr_name,ifr->ifr_flags,cur_if_addr);
		DEBUG_STRING(TRUE,0,2701,outstr);
		if ((ifr->ifr_flags & IFF_UP) && (cur_if_addr == my_host_id)) {
			n = -1;
			break;
		} else {
			ifr++;
		}
	}
	if (n != -1) {
		ERROR((msg,
	"Could not find a working network interface - disabling the network"));
		close(s);
		RETURN(FALSE);
	}

#ifdef SIOCGIFBRDADDR
	/*
	 * Find the socket broadcast address.
	 */
	if (ioctl( s, SIOCGIFBRDADDR, (char *)ifr) < 0) {
		ERROR((msg,"Cannot get the socket broadcast address: errno=%d",errno));
	} else {
		found_bcast = TRUE;
		cur_if_sockaddr = (struct sockaddr_in *)&(ifr->ifr_broadaddr);
		broadcast_address = cur_if_sockaddr->sin_addr.s_addr;
	}
#endif SIOCGIFBRDADDR

	close(s);

	/*
	 * Decide on the broadcast address.
	 */
	nm_fp = fopen("nmbroadcast","r");
	if (nm_fp == NULL) {
		nm_fp = fopen("/etc/nmbroadcast","r");
	}
	if (nm_fp != NULL) {
		int	a,b,c,d;
		int	ret;

		ret = fscanf(nm_fp,"%d.%d.%d.%d\n",&a,&b,&c,&d);
		temp_address.ia_bytes.ia_net_owner = a;
		temp_address.ia_bytes.ia_net_node_type = b;
		temp_address.ia_bytes.ia_host_high = c;
		temp_address.ia_bytes.ia_host_low = d;
		fclose(nm_fp);
		if (ret != 4) {
			ERROR((msg,"** Invalid format for nmbroadcast file."));
			RETURN(FALSE);
		}
		broadcast_address = temp_address.ia_netaddr;
		found_bcast = TRUE;
	}

	if (found_bcast == FALSE) {
		ERROR((msg,"** Could not find a useful broadcast address."));
		RETURN(FALSE);
	}

	ERROR((msg,"Broadcast address: 0x%x",broadcast_address));
	DEBUG1(TRUE,0,2702,broadcast_address);

	RETURN(TRUE);
#endif	USE_NETIPC_MACH3
END




/*
 * udp_checksum
 *	Calculate a UDP checksum.
 *
 * Parameters:
 *	base	: pointer to data to be checksummed.
 *	length	: number of bytes to be checksummed.
 *
 * Returns:
 *	The checksum.
 *
 */

#ifdef vax
/*
 * Checksum routine for Internet Protocol family headers (VAX Version).
 *
 * This routine is very heavily used in the network
 * code and should be modified for each CPU to be as fast as possible.
 */

udp_checksum(base, length)
	register unsigned short *base;	/* r11 */
	register int length;		/* r10 */
{
	register int sum = 0;		/* r9 */

	/*
	 * Force to long boundary so we do longword aligned
	 * memory operations.  It is too hard to do byte
	 * adjustment, do only word adjustment.
	 */
	if (((int)base&0x2) && length >= 2) {
		sum += *base++;
		length -= 2;
	}
	/*
	 * Do as much of the checksum as possible 32 bits at at time.
	 * In fact, this loop is unrolled to make overhead from
	 * branches &c small.
	 *
	 * We can do a 16 bit ones complement sum 32 bits at a time
	 * because the 32 bit register is acting as two 16 bit
	 * registers for adding, with carries from the low added
	 * into the high (by normal carry-chaining) and carries
	 * from the high carried into the low on the next word
	 * by use of the adwc instruction.  This lets us run
	 * this loop at almost memory speed.
	 *
	 * Here there is the danger of high order carry out, and
	 * we carefully use adwc.
	 */
	while ((length -= 32) >= 0) {
#define ADD	asm("adwc (r11)+,r9;")
		ADD; ADD; ADD; ADD; ADD; ADD; ADD; ADD;
		asm("adwc $0,r9");
	}
	length += 32;
	while ((length -= 8) >= 0) {
		ADD; ADD;
		asm("adwc $0,r9");
	}
	length += 8;
	/*
	 * Now eliminate the possibility of carry-outs by
	 * folding back to a 16 bit number (adding high and
	 * low parts together.)  Then mop up trailing words
	 * and maybe an odd byte.
	 */
	{ asm("ashl $-16,r9,r0; addw2 r0,r9");
	  asm("adwc $0,r9; movzwl r9,r9"); }
	while ((length -= 2) >= 0) {
		asm("movzwl (r11)+,r0; addl2 r0,r9");
	}
	if (length == -1) {
		sum += *(unsigned char *)base;
	}
	/*
	 * Add together high and low parts of sum
	 * and carry to get cksum.
	 * Have to be careful to not drop the last
	 * carry here.
	 */
	{ asm("ashl $-16,r9,r0; addw2 r0,r9; adwc $0,r9");
	  asm("mcoml r9,r9; movzwl r9,r9"); }
	return sum;
}

#else

#ifdef romp
udp_checksum(base, length)
	register char	*base;
	register int	length;
{
	return (0xffff ^ scksum(base, length));
}

#else

/*
 * Checksum routine for Internet Protocol family headers (NON-VAX Version).
 *
 * This routine is very heavily used in the network
 * code and should be modified for each CPU to be as fast as possible.
 * This particular version is a quick hack which needs to be rewritten.
 */

udp_checksum(base, length)
	register unsigned short *base;
	register int length;
{
	register int sum = 0;

	while (length >= 2) {
		sum += *base++;
		if (sum > 0xffff)
			sum -= 0xffff;
		length -= 2;
	}
	if (length == 1) {
		sum += *(unsigned char *)base;
		if (sum > 0xffff)
			sum -= 0xffff;
	}
	return sum ^ 0xffff;
}
#endif !(defined romp)
#endif !(defined vax)
