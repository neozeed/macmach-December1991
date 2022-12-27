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
 * $Log:	tcpip.c,v $
 * Revision 2.3  91/03/19  11:38:23  mrt
 * 	Changed to new copyright
 * 
 * Revision 2.2  90/09/12  16:29:44  rpd
 * 	First check-in.
 * 	[90/09/12  16:24:59  rpd]
 * 
 */

#include <stdio.h>
#include <strings.h>
#include <netdb.h>
#include <errno.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <sys/uio.h>
#include <sys/time.h>
#include <sys/resource.h>
#include <sys/signal.h>
#include <sys/wait.h>
#include <netinet/in.h>
#include <arpa/inet.h>

#define streql(a, b)	(strcmp((a), (b)) == 0)

typedef int boolean_t;
#define TRUE	1
#define	FALSE	0

extern int errno;

extern char *malloc();


int requestsize;
char *requestdata;
int replysize;
char *replydata;

static void client(), server();

static void
usage()
{
	fprintf(stderr, "usage: tcpip -s [-p port] [-req size] [-rep size]\n");
	fprintf(stderr, "usage: tcpip -c host [-p port] [-i iterations] [-req size] [-rep size]\n");
	fprintf(stderr, "\t-p: TCP/IP port number\n");
	fprintf(stderr, "\t-req: size (bytes, in hex) of request\n");
	fprintf(stderr, "\t-rep: size (bytes, in hex) of reply\n");
	fprintf(stderr, "\t-i: number of RPCs measured\n");
	fprintf(stderr, "\t-c: host/address of server\n");
	exit(1);
}

main(argc, argv)
	int argc;
	char *argv[];
{
	boolean_t AmClient = FALSE, AmServer = FALSE;
	int iterations = 1000;
	unsigned short port = 27348;
	char *servername;

	struct protoent *p;
	int s;

	int i;

	for (i = 1; i < argc; i++)
		if (streql(argv[i], "-c") && !AmServer && (i < argc-1)) {
			AmClient = TRUE;
			servername = argv[++i];
		} else if (streql(argv[i], "-s") && !AmClient)
			AmServer = TRUE;
		else if (streql(argv[i], "-i") && (i < argc-1) && AmClient)
			iterations = atoi(argv[++i]);
		else if (streql(argv[i], "-p") && (i < argc-1))
			port = atoi(argv[++i]);
		else if (streql(argv[i], "-req") && (i < argc-1))
			requestsize = atoh(argv[++i]);
		else if (streql(argv[i], "-rep") && (i < argc-1))
			replysize = atoh(argv[++i]);
		else
			usage();

	if (!AmClient && !AmServer)
		usage();

	if (requestsize <= 0)
		requestsize = 64;

	requestdata = malloc(requestsize);
	if (requestdata == NULL)
		quit(1, "tcpip: malloc failed\n");

	if (replysize <= 0)
		replysize = 64;

	replydata = malloc(replysize);
	if (replydata == NULL)
		quit(1, "tcpip: malloc failed\n");

	p = getprotobyname("tcp");
	if (p == NULL)
		quit(1, "tcpip: can't get protocol\n");

	s = socket(PF_INET, SOCK_STREAM, p->p_proto);
	if (s < 0)
		quit(1, "tcpip: can't create socket\n");


	if (AmClient)
		client(s, servername, port, iterations);
	else
		server(s, port);

	exit(0);
}

static int
recvdata(s, data, datasize)
	int s;
	char *data;
	int datasize;
{
	char *buffer = data;
	int left = datasize;
	int sofar = 0;
	int cc;

	while (left > 0) {
		cc = recv(s, buffer, left, 0);
		if (cc < 0) {
			if (errno == EINTR)
				continue;
			quit(1, "tcpip: recv failed\n");
		} else if (cc == 0)
			break;

		sofar += cc;
		buffer += cc;
		left -= cc;
	}

	return sofar;
}

static void
senddata(s, data, datasize)
	int s;
	char *data;
	int datasize;
{
	struct iovec iovec;
	int cc;

	iovec.iov_base = data;
	iovec.iov_len = datasize;

	if (writev(s, &iovec, 1) != datasize)
		quit(1, "tcpip: writev failed\n");
}

double
perrpc(before, after, iterations)
	struct timeval *before, *after;
	int iterations;
{
	return (((after->tv_sec - before->tv_sec) * 1000000 +
		 (after->tv_usec - before->tv_usec)) /
		(double) iterations);
}

double
throughput(before, after, iterations, size)
	struct timeval *before, *after;
	int iterations;
	vm_size_t size;
{
	/* bytes/sec */

	return (((double)iterations * size) /
		((after->tv_sec - before->tv_sec) +
		 (after->tv_usec - before->tv_usec) / 1000000.0));
}

static void
clientloop(s, iterations)
	int s;
	int iterations;
{
	struct rusage rbefore, rafter;
	struct timeval tbefore, tafter;
	double tput;
	int i;

	printf("%d RPCs, 0x%x requests, 0x%x replies:\n",
	       iterations, requestsize, replysize);

	(void) gettimeofday(&tbefore, (struct timezone *) NULL);
	(void) getrusage(RUSAGE_SELF, &rbefore);

	for (i = 0; i < iterations; i++) {
		senddata(s, requestdata, requestsize);
		if (recvdata(s, replydata, replysize) != replysize)
			quit(1, "tcpip: rpc incomplete\n");
	}

	(void) getrusage(RUSAGE_SELF, &rafter);
	(void) gettimeofday(&tafter, (struct timezone *) NULL);

	printf("Elapsed usecs/RPC: %7.3f\n",
	       perrpc(&tbefore, &tafter, iterations));
	printf("User    usecs/RPC: %7.3f\n",
	       perrpc(&rbefore.ru_utime, &rafter.ru_utime, iterations));
	printf("System  usecs/RPC: %7.3f\n",
	       perrpc(&rbefore.ru_stime, &rafter.ru_stime, iterations));

	tput = throughput(&tbefore, &tafter, iterations, requestsize);

	printf("\n");
	printf("Throughput bytes/sec: %d (%dK)\n",
	       (int) tput, (int) (tput / 1024.0));
}

static void
client(s, servername, port, iterations)
	int s;
	char *servername;
	unsigned short port;
	int iterations;
{
	struct sockaddr sockaddr;
	struct sockaddr_in *to = (struct sockaddr_in *) &sockaddr;

	bzero((char *) &sockaddr, sizeof(struct sockaddr));
	to->sin_family = AF_INET;
	to->sin_port = htons(port);
	to->sin_addr.s_addr = inet_addr(servername);
	if (to->sin_addr.s_addr == -1) {
		struct hostent *hp;

		hp = gethostbyname(servername);
		if (hp == NULL)
			quit(1, "tcpip: unknown host %s\n", servername);

		to->sin_family = hp->h_addrtype;
		bcopy(hp->h_addr, (caddr_t)&to->sin_addr, hp->h_length);
	}

	if (connect(s, (struct sockaddr *) to, sizeof *to) < 0)
		quit(1, "tcpip: connect failed\n");

	clientloop(s, iterations);
	if (close(s) < 0)
		quit(1, "tcpip: close failed\n");
}

static void
serverloop(s)
	int s;
{
	while (recvdata(s, requestdata, requestsize) == requestsize)
		senddata(s, replydata, replysize);
}

static int
reaper()
{
	union wait status;

	while (wait3(&status, WNOHANG, (struct rusage *) NULL) > 0)
		continue;
}

static void
server(s, port)
	int s;
	unsigned short port;
{
	struct sockaddr sockaddr;
	struct sockaddr_in *from = (struct sockaddr_in *) &sockaddr;
	int sockaddrlen;
	int ns;

	(void) signal(SIGCHLD, reaper);

	from->sin_family = AF_INET;
	from->sin_port = htons(port);
	from->sin_addr.s_addr = INADDR_ANY;

	if (bind(s, (struct sockaddr *) from, sizeof *from) < 0)
		quit(1, "tcpip: bind failed\n");

	if (listen(s, 5) < 0)
		quit(1, "tcpip: listen failed\n");

	for (;;) {
		int pid;

		sockaddrlen = sizeof sockaddr;
		ns = accept(s, &sockaddr, &sockaddrlen);
		if (ns < 0) {
			if (errno == EINTR)
				continue;
			quit(1, "tcpip: accept failed\n");
		}

		pid = fork();
		if (pid < 0)
			quit(1, "tcpip: fork failed\n");

		if (pid == 0) {
			serverloop(ns);
			if (close(ns) < 0)
				quit(1, "tcpip: close failed\n");
			exit(0);
		}

		if (close(ns) < 0)
			quit(1, "tcpip: close failed\n");
	}
}
