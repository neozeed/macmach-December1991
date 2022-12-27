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
 * $Log:	icmp.c,v $
 * Revision 2.3  91/03/19  11:38:09  mrt
 * 	Changed to new copyright
 * 
 * Revision 2.2  90/09/12  16:29:32  rpd
 * 	First check-in.
 * 	[90/09/12  16:25:11  rpd]
 * 
 */

#include <stdio.h>
#include <errno.h>
#include <sys/time.h>
#include <sys/resource.h>

#include <sys/param.h>
#include <sys/socket.h>
#include <sys/file.h>

#include <netinet/in_systm.h>
#include <netinet/in.h>
#include <netinet/ip.h>
#include <netinet/ip_icmp.h>
#include <netdb.h>

#ifndef MAXHOSTNAMELEN
#define MAXHOSTNAMELEN	64
#endif

#define streql(a, b)	(strcmp((a), (b)) == 0)

usage()
{
	fprintf(stderr, "usage:  icmp [-i iterations] [-s size] host\n");
	fprintf(stderr, "\t-i: number of RPCs measured\n");
	fprintf(stderr, "\t-s: size (bytes, in decimal) of packets\n");
	exit(1);
}

double
perrpc(before, after, iterations)
	struct timeval *before, *after;
	int iterations;
{
	/* usecs/RPC */

	return (((after->tv_sec - before->tv_sec) * 1000000 +
		 (after->tv_usec - before->tv_usec)) /
		(double) iterations);
}

double
throughput(before, after, iterations, size)
	struct timeval *before, *after;
	int iterations;
	unsigned int size;
{
	/* bytes/sec */

	return (((double)iterations * size) /
		((after->tv_sec - before->tv_sec) +
		 (after->tv_usec - before->tv_usec) / 1000000.0));
}

main(argc, argv)
char *argv[];
{
	struct rusage rbefore, rafter;
	struct timeval tbefore, tafter;

	int iterations = 1000;
	unsigned int size = 64;
	char *data;

	struct sockaddr whereto;
	struct sockaddr_in *to = (struct sockaddr_in *) &whereto;
	struct protoent *proto;
	int s;
	unsigned short sequence;
	unsigned short ident;

	int i;

	for (i = 1; i < argc; i++)
		if (streql(argv[i], "-i") && (i < argc-1))
			iterations = atoi(argv[++i]);
		else if (streql(argv[i], "-s") && (i < argc-1))
			size = atoi(argv[++i]);
		else
			break;

	if (i != argc-1)
		usage();

	bzero((char *) &whereto, sizeof(struct sockaddr));
	to->sin_family = AF_INET;
	to->sin_addr.s_addr = inet_addr(argv[argc-1]);
	if (to->sin_addr.s_addr == -1) {
		struct hostent *hp;

		hp = gethostbyname(argv[argc-1]);
		if (hp == NULL)
			quit(1, "icmp: unknown host %s\n", argv[argc-1]);

		to->sin_family = hp->h_addrtype;
		bcopy(hp->h_addr, (caddr_t)&to->sin_addr, hp->h_length);
	}

	proto = getprotobyname("icmp");
	if (proto == NULL)
		quit(1, "icmp: unknown protocol\n");

	s = socket(AF_INET, SOCK_RAW, proto->p_proto);
	if (s < 0)
		quit(1, "icmp: socket\n");

	if (size < ICMP_MINLEN)
		size = ICMP_MINLEN;

	data = (char *) malloc(size);
	if (data == NULL)
		quit(1, "icmp: malloc\n");

	ident = getpid();
	sequence = 0;

	printf("%d RPCs, %d bytes data:\n", iterations, size);

	(void) gettimeofday(&tbefore, (struct timezone *) NULL);
	(void) getrusage(RUSAGE_SELF, &rbefore);

	for (i = 0; i < iterations; i++) {
		struct icmp *icmp;
		int cc;

		icmp = (struct icmp *) data;
		icmp->icmp_type = ICMP_ECHO;
		icmp->icmp_code = 0;
		icmp->icmp_cksum = 0;
		icmp->icmp_seq = sequence++;
		icmp->icmp_id = ident;

		icmp->icmp_cksum = in_cksum((u_short *) icmp, size);

		cc = sendto(s, icmp, size, 0,
			    &whereto, sizeof(struct sockaddr));
		if (cc != size)
			quit(1, "icmp: sendto\n");

		do {
			struct ip *ip;
			struct sockaddr_in from;
			int fromlen = sizeof (from);
			int hlen;

			cc = recvfrom(s, data, size, 0, &from, &fromlen);
			if (cc <= 0)
				continue;

			ip = (struct ip *) data;
			hlen = ip->ip_hl << 2;
			if (cc < hlen + ICMP_MINLEN)
				continue;

			icmp = (struct icmp *) (data + hlen);
		} while ((icmp->icmp_type != ICMP_ECHOREPLY) ||
			 (icmp->icmp_id != ident));

		if (icmp->icmp_seq+1 != sequence)
			printf("warning: bad sequence number (%d)\n",
			       icmp->icmp_seq);
	}

	(void) getrusage(RUSAGE_SELF, &rafter);
	(void) gettimeofday(&tafter, (struct timezone *) NULL);

	printf("Elapsed usecs/RPC: %7.3f\n",
	       perrpc(&tbefore, &tafter, iterations));
	printf("User    usecs/RPC: %7.3f\n",
	       perrpc(&rbefore.ru_utime, &rafter.ru_utime, iterations));
	printf("System  usecs/RPC: %7.3f\n\n",
	       perrpc(&rbefore.ru_stime, &rafter.ru_stime, iterations));
	printf("Throughput bytes/sec: %d (%dK)\n",
	       (int) throughput(&tbefore, &tafter, iterations, size),
	       (int) (throughput(&tbefore, &tafter, iterations, size)/1024.0));
	exit(0);
}

in_cksum(addr, len)
u_short *addr;
int len;
{
	register int nleft = len;
	register u_short *w = addr;
	register u_short answer;
	register int sum = 0;

	/*
	 *  Our algorithm is simple, using a 32 bit accumulator (sum),
	 *  we add sequential 16 bit words to it, and at the end, fold
	 *  back all the carry bits from the top 16 bits into the lower
	 *  16 bits.
	 */
	while( nleft > 1 )  {
		sum += *w++;
		nleft -= 2;
	}

	/* mop up an odd byte, if necessary */
	if( nleft == 1 )
		sum += *(u_char *)w;

	/*
	 * add back carry outs from top 16 bits to low 16 bits
	 */
	sum = (sum >> 16) + (sum & 0xffff);	/* add hi 16 to low 16 */
	sum += (sum >> 16);			/* add carry */
	answer = ~sum;				/* truncate to 16 bits */
	return (answer);
}
