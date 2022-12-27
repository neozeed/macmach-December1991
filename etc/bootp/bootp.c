/*
 * CLIENT bootstrap protocol client program
 * See [SRI-NIC]<RFC>RFC951.TXT for a description of the protocol.
 */

/*
 * history
 * 07/11/86	dk1z (Kovar) created from bootp.c
 */


#include <sys/types.h>
#include <sys/stat.h>
#include <sys/socket.h>
#include <sys/ioctl.h>
#include <sys/file.h>
#include <sys/time.h>

#include <net/if.h>
#include <netinet/in_systm.h>
#include <netinet/in.h>
#include <netinet/if_ether.h>
#include <netinet/ip.h>
#include <netinet/udp.h>

#ifdef ibm032
#include <caif/if_landma.h>
#include <caif/if_lanreg.h>
#include <caif/if_lanvar.h>
#endif

#define	iaddr_t struct in_addr
#include <arpa/bootp.h>

#include <signal.h>
#include <stdio.h>
#include <strings.h>
#include <errno.h>
#include <ctype.h>
#include <netdb.h>
#include <setjmp.h>
#include <nlist.h>

#ifdef mac2
#define BOOTP_DLI
#endif

extern	int errno;
struct	sockaddr_in sin = { AF_INET };
#ifdef BOOTP_DLI
struct  sockaddr    to  = { AF_DLI };
char etherbroadcastaddr[] = {0xff,0xff,0xff,0xff,0xff,0xff};
#else  BOOTP_DLI
struct  sockaddr_in to  = { AF_INET };
#endif BOOTP_DLI
int	insock;		/* socket fd */
int	outsock;
struct	sockaddr_in from;
int	fromlen;
u_char	buf[1024];		/* receive packet buffer */
long	time(), tloc;		/* time of day */
struct	ifreq ifreq[10];	/* holds interface configuration */
struct	ifconf ifconf;		/* int. config. ioctl block (points to ifreq) */
struct	arpreq arpreq;		/* arp request ioctl block */
char *outp;
int  outsize;
struct  bootp *bp, *rbp;	/* Boot packet pointers */
char tmpmsg[80];		/* For logging messages */
unsigned int backoff = 1;	/* Backoff interval */
long random();
long starttime;		        /* Start of session time */
int nback;			/* "no backoff" flag */
int retries;			/* Number of retries */
int debug;			/* Debugging flag */

#ifndef BROADCAST
#define BROADCAST (inet_addr("255.255.255.255"))
#endif  BROADCAST

#ifndef MAXREQ
#define MAXREQ  6
#endif  MAXREQ

#ifndef BACKINT
#define BACKINT 1
#endif  BACKINT

/* convert hardware (ethernet) address to string */
char *haddrtoa(h)
unsigned char *h;
{
  static char haddrbuf[18];
  sprintf(haddrbuf, "%02x:%02x:%02x:%02x:%02x:%02x",
    h[0], h[1], h[2], h[3], h[4], h[5]);
  return (haddrbuf);
}

cksum(d, len)
register caddr_t d;
register len;
{
    register u_short *sp;
    register sum = 0, high;
    int s;

    if (len&1) {
    	d[len] = 0;
	len++;
    }
    for (sp = (u_short *)d; len > 0; len-=2)
    	sum += *sp++;
    s = sum;
    sp = (u_short *)&s;
    high = sp[0];
    sp[0] = 0;
    s += high;
    high = sp[0];
    sp[0] = 0;
    s += high;
    return ((~s)&0xffff);
}    

main(argc, argv)
	int argc;
	char *argv[];
{
	register int n;
	register char *ap;
	u_char haddr[16];	/* Hardware address */
	u_long inet_addr();
	int tid;		/* Temp ID */
	struct vend *vendp;
#ifdef BOOTP_DLI
	struct  ether_header *eh;
	struct  ip *ipp;
	struct ifreq ifr;
#endif
#if defined(BOOTP_UDP) || defined(BOOTP_DLI)
	struct	udphdr *udp;
#endif
	
	argc--, argv++;
	if (argc > 0) {
	    ap = argv[0];
	    while (*ap) {
	        while (*ap == ' ' || *ap == '-') {
		    *ap++;
		}
		switch (*ap++) {
		    case 'h':		 /* hardware address */
		        ghaddr(haddr);
		        puts(haddrtoa(haddr));
		        exit(0);
		    case 'd':		 /* Debugging */
		        debug++;
		        break;
		    case 'n':		 /* No backoff */
		        nback++;
		        break;
		    default:		 
		        if (isdigit(ap[-1])) {  /* Retries ... */
			    retries = atoi(--ap);
			    *ap++;
			    break;
			}
			else {
			    printf("One of dn[1-9]\n");
			    exit(1);
			}
		}
	    }
	}
	

	if (retries == 0) {
	    retries = MAXREQ;
	}
	
	/* Open datagram socket for dialog */
	while ((insock = socket(AF_INET, SOCK_DGRAM, 0)) < 0) {
		fprintf(stderr,"socket (DGRAM) failed\n");
		sleep(5);
	}
	
	sin.sin_port = htons(IPPORT_BOOTPC);
	sin.sin_addr.s_addr = INADDR_ANY;
	
	while (bind(insock, (struct sockaddr *)&sin, sizeof (sin)) < 0) {
		fprintf(stderr,"bind call failed\n");
		sleep(5);
	}
	
#ifdef BOOTP_DLI
	strcpy (ifr.ifr_name, "en0");
	ifr.ifr_addr.sa_family = AF_DLI;

	while ((outsock = socket(AF_DLI, SOCK_RAW, 0)) < 0) {
		fprintf(stderr,"socket (DLI)  failed\n");
		sleep(5);
	}

	if (ioctl(outsock, SIOCGIFADDR, (caddr_t)&ifr) < 0) {
	    fprintf (stderr, "cannot get ethernet address\n");
	    exit (1);
	}

	if (bind(outsock, &ifr.ifr_addr, sizeof ifr.ifr_addr) < 0) {
	    perror ("connect out");
	    exit(1);
	}
	outsize = sizeof *eh + sizeof *ipp + sizeof *udp + sizeof *bp;
	outp = (char *)malloc(outsize);
	eh  = (struct ether_header *) outp;
	ipp = (struct ip *)(eh + 1);
	udp = (struct udphdr *)(ipp + 1);
	bp  = (struct bootp *) (udp + 1);
#else
#ifdef BOOTP_UDP
	while ((outsock = socket(AF_INET, SOCK_RAW, IPPROTO_UDP)) < 0) {
		fprintf(stderr,"socket (RAW)  failed\n");
		sleep(5);
	}
	outsize = sizeof *udp + sizeof *bp;
	outp = (char *)malloc(outsize);
	udp = (struct udphdr *) outp;
	bp  = (struct bootp *) (udp + 1);
	outp = (char *)udp;
#else  BOOTP
        to.sin_port = htons(IPPORT_BOOTPS);
	to.sin_addr.s_addr = BROADCAST;
	outsock = insock;
	outsize = sizeof *bp;
	outp = (char *)malloc(outsize);
	bp = (struct bootp *) outp;
#endif BOOTP
#endif BOOTP	

	/* Remember what time we started */
	time(&starttime);
	srandom(starttime);
	
	bzero(outp, outsize);
	
	/* Set up boot packet */

#ifdef  BOOTP_DLI
	eh  -> ether_type = ntohs(ETHERTYPE_IP);
	ipp -> ip_v = IPVERSION;
	ipp -> ip_hl = (sizeof *ipp) >> 2;
	ipp -> ip_tos = 0;
	ipp -> ip_len = htons(sizeof *ipp + sizeof *udp + sizeof *bp);
	ipp -> ip_id = 1;
	ipp -> ip_off = IP_DF;
	ipp -> ip_ttl = 127;
	ipp -> ip_p = IPPROTO_UDP;
	ipp -> ip_src.s_addr = 0;
	ipp -> ip_dst.s_addr = BROADCAST;
	ipp -> ip_sum = 0;
	ipp -> ip_sum = cksum((caddr_t)ipp, sizeof *ipp);
	bcopy (etherbroadcastaddr, to.sa_data, 6);
#endif  BOOTP_DLI

#if defined(BOOTP_DLI) || defined(BOOTP_UDP)
	udp -> uh_sport = htons(IPPORT_BOOTPC);
	udp -> uh_dport = htons(IPPORT_BOOTPS);
	udp -> uh_ulen = sizeof(struct bootp) + sizeof(struct udphdr);
	udp -> uh_sum = 0;
#endif BOOTP_DLI || BOOTP_UDP

	/* Set up boot packet */
	bp -> bp_op = BOOTREQUEST;    /* Request boot */
	bp -> bp_htype = 1;	      /* Not really used */
	bp->bp_hlen = 6;	      /* Ditto */
	bp->bp_hops = 0;	      /* Ditto */
	tid = random();
	bp->bp_xid = tid;	      /* Transaction ID */
	bp->bp_secs = 0;	      /* How long we've been waiting */
	bp->bp_ciaddr.s_addr = 0;     /* All the IP addresses */
	bp->bp_yiaddr.s_addr = 0;
	bp->bp_siaddr.s_addr = 0;
	bp->bp_giaddr.s_addr = 0;
	ghaddr(haddr);
	bcopy(haddr, bp->bp_chaddr, sizeof(bp->bp_chaddr));
	bp->bp_sname[0] = NULL;	      /* Again, not used */
	bp->bp_file[0] = NULL;
	bp->bp_vend[0] = NULL;


	init_timers();		      /* Start up timers */
	send_req();		      /* Send first request */
	
	/* Run around in a look waiting for packets. We block on the
	   recvfrom and let the timer wake us up to send out new requests.
	*/
	for (;;) {
	    fromlen = sizeof(from);
	    n = recvfrom(insock, (char *)buf, sizeof buf, 0, (struct sockaddr *)&from, &fromlen);

	    if (n <= 0)		       /* Empty packet */
		continue;

	    rbp = (struct bootp *) buf;
	    
	    if (n < sizeof *rbp)       /* Not the right size packet */
	    continue;

	    switch (rbp->bp_op) {
		case BOOTREQUEST: 
	        break;

		case BOOTREPLY:	       /* Make sure it is for us ... */
	        if ((bcmp(haddr, rbp -> bp_chaddr, 6) == 0) &&
		    (tid == rbp -> bp_xid)) {
				
			vendp = (struct vend *) (rbp -> bp_vend);
			{
			    register int i;
			    u_char *ipaddr = (u_char *)&rbp -> bp_yiaddr.s_addr;

			    for (i = 0; i < 3; i++)
				printf ("%d.", (u_char)ipaddr[i]);
			    printf ("%d\n", (u_char)ipaddr[i]);
			}
#ifdef CMU_VEND
			printf("SubMask: %x\n", vendp -> v_smask);
			printf("Gateway: %x\n", vendp -> v_dgate);
#endif CMU_VEND
			exit(0);
		    }
		break;
	}
    }
}


/* Get hardware address of this machine */
ghaddr(haddr)
char *haddr;
{
    struct ifreq ifr;
    int s;

    strcpy (ifr.ifr_name, "en0");
    ifr.ifr_addr.sa_family = AF_DLI;
    
    if ((s = socket(AF_DLI, SOCK_RAW, 0)) < 0) {
	perror("bootp: socket(AF_DLI, SOCK_RAW, 0)");
	exit(1);
    }

    if (ioctl(s, SIOCGIFADDR, (caddr_t)&ifr) < 0) {
	perror("bootp: ioctl(s, SIOCGIFADDR, (caddr_t)&ifr)");
	exit (1);
    }

    if (debug) fprintf(stderr, "haddr: %s\n", haddrtoa(ifr.ifr_addr.sa_data));
    bcopy((caddr_t)ifr.ifr_addr.sa_data, haddr, 6);
    return;
}

/* Send out a BOOTP request */
send_req()
{
    int     ret;
    static int nreq;
    struct itimerval timer;
    long curtime;
    
    time(&curtime);
    
    /* Figure out how long we've been waiting */
    bp -> bp_secs = curtime - starttime;
        
    ret = sendto(outsock, (char *)outp, outsize, 0, (struct sockaddr *)&to, sizeof to);

    if (ret < 0)
	perror("sendto failed");
    else {
        }

    nreq++;
    
    /* If we need to back off, figure out how long to back off */
    if (nback == 0) {
        if (backoff < 60) {
	    backoff = (backoff << 1) + (random() % 3);
	    if (backoff > 60) backoff = 60;
        }
    }
    else {
	backoff = BACKINT;
    }

    timer.it_interval.tv_sec = 0;
    timer.it_interval.tv_usec = 0;
    timer.it_value.tv_sec = backoff;
    timer.it_value.tv_usec = 0;

    if (nreq > retries) {
	fprintf(stderr, "Server not responding, giving up.\n");
	exit(1);
    }

    setitimer(ITIMER_REAL, &timer, 0);

    return(ret);
}

init_timers()
{
    struct sigvec   new_vec;
    int     catch_alarm ();	/* Routine to catch timer alarms */


 /* Set up to catch timer alarms */
    new_vec.sv_handler = catch_alarm;
    new_vec.sv_mask = 0;
    new_vec.sv_onstack = 0;
    if (sigvec (SIGALRM, &new_vec, 0) != 0) {
	fprintf(stderr, "sigalrm sigvec failed");
	exit(-1);
    }
}

/*  catch_alarm -
	Subroutine to catch the timer alarms. Send bootp request again.
 */

catch_alarm( sig, code, scp)
int sig, code;
struct sigcontext *scp;
{

send_req();

}
