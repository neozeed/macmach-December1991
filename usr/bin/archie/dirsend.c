/*
 * Copyright (c) 1989, 1990, 1991 by the University of Washington
 *
 * For copying and distribution information, please see the file
 * <uw-copyright.h>.
 *
 * Xarchie status calls added by George Ferguson, 13 Aug 1991.
 */

#include <uw-copyright.h>
#include <stdio.h>
#include <errno.h>
#include <sys/time.h>
#include <netdb.h>
#include <strings.h>

#include <sys/types.h>
#include <sys/socket.h>
#ifdef AIX
#include <sys/select.h>
#endif

#include <pfs.h>
#include <pprot.h>
#include <pcompat.h>
#include <perrno.h>
#include <pmachine.h>


static int notprived = 0;
extern int errno;
extern int perrno;
#ifdef DEBUG
extern int pfs_debug;
#endif
extern int pfs_disable_flag;

int	rdgram_priority = 0;

char	*nlsindex();

#define max(X, Y)  ((X) > (Y) ? (X) : (Y))

static int		dir_udp_port = 0;	/* Remote UDP port number */

static unsigned short	next_conn_id = 0;

#ifdef XARCHIE
int client_dirsrv_timeout = CLIENT_DIRSRV_TIMEOUT;
int client_dirsrv_retry = CLIENT_DIRSRV_RETRY; 
#else /* !XARCHIE */
static int client_dirsrv_timeout = CLIENT_DIRSRV_TIMEOUT;
static int client_dirsrv_retry = CLIENT_DIRSRV_RETRY; 
#endif /* XARCHIE */

/*
 * dirsend - send packet and receive response
 *
 *   DIRSEND takes a pointer to a structure of type PTEXT, a hostname,
 *   and a pointer to a host address.  It then sends the supplied
 *   packet off to the directory server on the specified host.  If
 *   hostaddr points to a valid address, that address is used.  Otherwise,
 *   the hostname is looked up to obtain the address.  If hostaddr is a
 *   non-null pointer to a 0 address, then the address will be replaced
 *   with that found in the hostname lookup.
 *
 *   DIRSEND will wait for a response and retry an appropriate
 *   number of times as defined by timeout and retries (both static
 *   variables).  It will collect however many packets form the reply, and
 *   return them in a structure (or structures) of type PTEXT.
 *
 *   DIRSEND will free the packet that it is presented as an argument.
 *   The packet is freed even if dirsend fails.
 */
PTEXT
dirsend(pkt,hostname,hostaddr)
    PTEXT pkt;
    char *hostname;
    struct sockaddr_in	*hostaddr;
{
    PTEXT		first = NULL;	/* First returned packet	 */
    PTEXT		next;		/* The one we are waiting for 	 */
    PTEXT		vtmp;           /* For reorganizing linked list  */
    PTEXT		comp_thru;	/* We have all packets though    */
    int			lp = -1;	/* Opened UDP port	         */
    int			hdr_len;	/* Header Length                 */
    int			nd_pkts;	/* Number of packets we want     */
    int			no_pkts;	/* Number of packets we have     */
    int			pkt_cid;        /* Packet connection identifier  */
    unsigned short	this_conn_id;	/* Connection ID we are using    */
    unsigned short	recvd_thru;	/* Received through              */
    short		priority;	/* Priority for request          */
    static short	one = 0;	/* Pointer to value 1            */
    static short	zero = 0;	/* Pointer to value 0		 */
    char		*seqtxt;	/* Pointer to text w/ sequence # */
    struct sockaddr_in  us;		/* Our address                   */
    struct sockaddr_in	to;		/* Address to send query	 */
    struct sockaddr_in	from;		/* Reply received from		 */
    int			from_sz;	/* Size of from structure	 */
    struct hostent	*host;		/* Host info from gethostbyname  */
    long		newhostaddr;    /* New host address from *host   */
    int			req_udp_port=0; /* Requested port (optional)     */
    char		*openparen;	/* Delimits port in name         */
    char		hostnoport[500];/* Host name without port        */
    int			ns;		/* Number of bytes actually sent */
    int			nr;		/* Number of bytes received      */
    fd_set		readfds;	/* Used for select		 */
    int			tmp;
    char		*ctlptr;	/* Pointer to control field      */
    short		stmp;		/* Temp short for conversions    */
    int			backoff;	/* Server requested backoff      */
    unsigned char	rdflag11;	/* First byte of flags (bit vect)*/
    unsigned char	rdflag12;	/* Second byte of flags (int)    */
    int			scpflag = 0;	/* Set if any sequencd cont pkts */

    int			ackpend = 0;    /* Acknowledgement pending      */
    int			gaps = 0;	/* Gaps present in recvd pkts   */
    struct timeval	timeout;	/* Time to wait for response    */
    struct timeval	ackwait;	/* Time to wait before acking   */
    struct timeval	gapwait;	/* Time to wait b4 filling gaps */
    struct timeval	*selwait;	/* Time to wait for select      */
    int			retries = client_dirsrv_retry;

#ifdef XARCHIE
    status0("Initializing");
#endif

    if(one == 0) one = htons((short) 1);

    priority = htons(rdgram_priority);

    timeout.tv_sec = client_dirsrv_timeout;
    timeout.tv_usec = 0;

    ackwait.tv_sec = 0;
    ackwait.tv_usec = 500000;

    gapwait.tv_sec = (client_dirsrv_timeout < 5 ? client_dirsrv_timeout : 5);
    gapwait.tv_usec = 0;

    comp_thru = NULL;
    perrno = 0;
    nd_pkts = 0;
    no_pkts = 0;
    pkt_cid = 0;

    /* Find first connection ID */
    if(next_conn_id == 0) {
	srand(getpid()+time(0));
	next_conn_id = rand();
    }


    /* If necessary, find out what udp port to send to */
    if (dir_udp_port == 0) {
        register struct servent *sp;
	tmp = pfs_enable; pfs_enable = PMAP_DISABLE;
#ifdef USE_ASSIGNED_PORT
        if ((sp = getservbyname("prospero","udp")) == 0) {
#ifdef DEBUG
	    if (pfs_debug)
		fprintf(stderr, "dirsrv: udp/prospero unknown service - using %d\n", 
			PROSPERO_PORT);
#endif
	    dir_udp_port = htons((u_short) PROSPERO_PORT);
        }
#else
        if ((sp = getservbyname("dirsrv","udp")) == 0) {
#ifdef DEBUG
	    if (pfs_debug)
		fprintf(stderr, "dirsrv: udp/dirsrv unknown service - using %d\n", 
			DIRSRV_PORT);
#endif
	    dir_udp_port = htons((u_short) DIRSRV_PORT);
        }
#endif
	else dir_udp_port = sp->s_port;
	pfs_enable = tmp;
#ifdef DEBUG
        if (pfs_debug > 3)
            fprintf(stderr,"dir_udp_port is %d\n", ntohs(dir_udp_port));
#endif
    }

    /* If we were given the host address, then use it.  Otherwise  */
    /* lookup the hostname.  If we were passed a host address of   */
    /* 0, we must lookup the host name, then replace the old value */
    if(!hostaddr || (hostaddr->sin_addr.s_addr == 0)) {
	/* I we have a null host name, return an error */
	if((hostname == NULL) || (*hostname == '\0')) {
#ifdef DEBUG
            if (pfs_debug)
                fprintf(stderr, "dirsrv: Null hostname specified\n");
#endif
	    perrno = DIRSEND_BAD_HOSTNAME;
	    ptlfree(pkt);
            return(NULL);
	}
	/* If a port is included, save it away */
	if(openparen = index(hostname,'(')) {
	    sscanf(openparen+1,"%d",&req_udp_port);
	    strncpy(hostnoport,hostname,400);
	    if((openparen - hostname) < 400) {
		*(hostnoport + (openparen - hostname)) = '\0';
		hostname = hostnoport;
	    }
	}
	tmp = pfs_enable; pfs_enable = PMAP_DISABLE;
        if((host = gethostbyname(hostname)) == NULL) {
	    pfs_enable = tmp;
	    /* Check if a numeric address */
	    newhostaddr = inet_addr(hostname);
	    if(newhostaddr == -1) {
#ifdef DEBUG
		if (pfs_debug)
		  fprintf(stderr, "dirsrv: Can't resolve host %s\n",hostname);
#endif
		perrno = DIRSEND_BAD_HOSTNAME;
		ptlfree(pkt);
		return(NULL);
	    }
	    bzero((char *)&to, S_AD_SZ);
	    to.sin_family = AF_INET;
	    bcopy(&newhostaddr, (char *)&to.sin_addr, 4);
	    if(hostaddr) bcopy(&to,hostaddr, S_AD_SZ);
	}
	else {
	    pfs_enable = tmp;
	    bzero((char *)&to, S_AD_SZ);
	    to.sin_family = host->h_addrtype;
	    bcopy(host->h_addr, (char *)&to.sin_addr, host->h_length);
	    if(hostaddr) bcopy(&to,hostaddr, S_AD_SZ);
	}
    }
    else bcopy(hostaddr,&to, S_AD_SZ);

    if(req_udp_port) to.sin_port = htons(req_udp_port);
    else to.sin_port = dir_udp_port;

    /* If a port was specified in hostaddr, use it, otherwise fill it in */
    if(hostaddr) {
	if(hostaddr->sin_port) to.sin_port = hostaddr->sin_port;
	else hostaddr->sin_port = to.sin_port;
    }

    /* Must open a new port each time. we do not want to see old */
    /* responses to messages we are done with                    */
    if ((lp = socket(AF_INET, SOCK_DGRAM, 0)) < 0) {
#ifdef DEBUG
        if (pfs_debug)
            fprintf(stderr,"dirsrv: Can't open socket\n");
#endif
	perrno = DIRSEND_UDP_CANT;
	ptlfree(pkt);
        return(NULL);
    }

    /* Try to bind it to a privileged port - loop through candidate */
    /* ports trying to bind.  If failed, that's OK, we will let the */
    /* system assign a non-privileged port later                    */
    if(!notprived) {
	for(tmp = PROS_FIRST_PRIVP; tmp < PROS_FIRST_PRIVP+PROS_NUM_PRIVP; 
	    tmp++) {
	    bzero((char *)&us, sizeof(us));
	    us.sin_family = AF_INET;
	    us.sin_port = htons((u_short) tmp);
	    if (bind(lp, (struct sockaddr *)&us, sizeof(us))) {
		if(errno != EADDRINUSE) {
		    notprived++;
		    break;
		}
	    }
	    else break;
	}
    }

#ifndef USE_V3_PROT
    /* Add header */
    if(rdgram_priority) {
	pkt->start -= 15;
	pkt->length += 15;
	*(pkt->start) = (char) 15;
	bzero(pkt->start+9,4);
	*(pkt->start+11) = 0x02;
	bcopy(&priority,pkt->start+13,2);
    }
    else {
	pkt->start -= 9;
	pkt->length += 9;
	*(pkt->start) = (char) 9;
    }
    this_conn_id = htons(next_conn_id++);
    if(next_conn_id == 0) next_conn_id++;
    bcopy(&this_conn_id,pkt->start+1,2);
    bcopy(&one,pkt->start+3,2);
    bcopy(&one,pkt->start+5,2);
    bzero(pkt->start+7,2);
#endif

#ifdef DEBUG
    if (pfs_debug > 2) {
#ifndef USE_V3_PROT
        if (to.sin_family == AF_INET) {
	    if(req_udp_port) 
		fprintf(stderr,"Sending message to %s+%d(%d)...",
			inet_ntoa(*(struct in_addr *)&to.sin_addr), req_udp_port, ntohs(this_conn_id));
	    else fprintf(stderr,"Sending message to %s(%d)...",
			 inet_ntoa(*(struct in_addr *)&to.sin_addr), ntohs(this_conn_id));
	}
#else
        if (to.sin_family == AF_INET) 
	    fprintf(stderr,"Sending message to %s...", inet_ntoa(*(struct in_addr *)&to.sin_addr));
#endif
        else
            fprintf(stderr,"Sending message...");
        (void) fflush(stderr);
    }
#endif

    first = ptalloc();
    next = first;

#ifdef XARCHIE
    status1("Connecting to %s",inet_ntoa(*(struct in_addr *)&to.sin_addr));
#endif

 retry:

    gaps = ackpend = 0;

    ns = sendto(lp,(char *)(pkt->start), pkt->length, 0, (struct sockaddr *)&to, S_AD_SZ);
    if(ns != pkt->length) {
#ifdef DEBUG
	if (pfs_debug) {
	    fprintf(stderr,"\nsent only %d/%d: ",ns, pkt->length);
	    perror("");
	}
#endif
	close(lp);
	perrno = DIRSEND_NOT_ALL_SENT;
	ptlfree(first);
	ptlfree(pkt);
#ifdef XARCHIE
        status0("Send Error");
#endif
        return(NULL);
    }
#ifdef DEBUG
    if (pfs_debug > 2) fprintf(stderr,"Sent.\n");
#endif
    /* We come back to this point (by a goto) if the packet */
    /* received is only part of the response, or if the     */
    /* response came from the wrong host		    */

 keep_waiting:	
#ifdef DEBUG
    if (pfs_debug > 2) fprintf(stderr,"Waiting for reply...");
#endif
    FD_ZERO(&readfds);
    FD_SET(lp, &readfds);

    if(ackpend) selwait = &ackwait;
    else if(gaps) selwait = &gapwait;
    else selwait = &timeout;

    /* select - either recv is ready, or timeout */
    /* see if timeout or error or wrong descriptor */
    tmp = select(lp + 1, &readfds, (fd_set *)0, (fd_set *)0, selwait);

    if((tmp == 0) && (gaps || ackpend)) { /* Send acknowledgment */
	/* Acks are piggybacked on retries - If we have received */
	/* an ack from the server, then the packet sent is only  */
	/* an ack and the rest of the message will be empty      */
#ifdef DEBUG
	if (pfs_debug > 2) {
            fprintf(stderr,"Acknowledging (%s).\n",
		    (ackpend ? "requested" : "gaps"));
	}	
#endif
	goto retry;
    }

    if((tmp == 0) && (retries-- > 0)) {
	timeout.tv_sec = CLIENT_DIRSRV_BACKOFF(timeout.tv_sec);
#ifdef DEBUG
	if (pfs_debug > 2) {
            fprintf(stderr,"Timed out.  Setting timeout to %d seconds.\n",
		    timeout.tv_sec);
	}
#endif
#ifdef XARCHIE
        status1("Timed out -- retrying (%d seconds)",timeout.tv_sec);
#endif
	goto retry;
    }

    if((tmp < 1) || !FD_ISSET(lp, &readfds)) {
#ifdef DEBUG
	if (pfs_debug) {
	    fprintf(stderr, "select failed: readfds=%x ",
                    readfds);
            perror("");
        }
#endif
	close(lp);
	perrno = DIRSEND_SELECT_FAILED;
	ptlfree(first);
	ptlfree(pkt);
#ifdef XARCHIE
        status1("Couldn't connect to %s",
		inet_ntoa(*(struct in_addr *)&to.sin_addr));
#endif
        return(NULL);
    }


    from_sz = sizeof(from);
    next->start = next->dat;
    if ((nr = recvfrom(lp, next->start, sizeof(next->dat), 0, (struct sockaddr *)&from, &from_sz)) < 0) {
#ifdef DEBUG
        if (pfs_debug) perror("recvfrom");
#endif
	close(lp);
	perrno = DIRSEND_BAD_RECV;
	ptlfree(first);
	ptlfree(pkt);
#ifdef XARCHIE
        status0("Recv Error");
#endif
        return(NULL);
    }

    next->length = nr;
    
    *(next->start + next->length) = NULL;

#ifdef DEBUG
    if (pfs_debug > 2) 
        fprintf(stderr,"Received packet from %s\n", inet_ntoa(*(struct in_addr *)&from.sin_addr));
#endif
    if (to.sin_addr.s_addr != from.sin_addr.s_addr) {
#ifdef DEBUG
	if (pfs_debug) {
	    fprintf(stderr, "dirsend: received packet from wrong host! (%x)\n",
		    from.sin_addr.s_addr);
	    (void) fflush(stdout);
	}
#endif
	goto keep_waiting;
    }

#ifdef XARCHIE
    {
        static int toggle;
        status0(toggle ? "Receiving +" : "Receiving *");
        toggle = !toggle;
    }
#endif

    /* For the current format, if the first byte is less than             */
    /* 20, then the first two bits are a version number and the next six  */
    /* are the header length (including the first byte).                  */
    if((hdr_len = (unsigned char) *(next->start)) < 20) {
	ctlptr = next->start + 1;
	next->seq = 0;
	if(hdr_len >= 3) { 	/* Connection ID */
	    bcopy(ctlptr,&stmp,2);
	    if(stmp) pkt_cid = ntohs(stmp);
	    ctlptr += 2;
	}
	if(hdr_len >= 5) {	/* Packet number */
	    bcopy(ctlptr,&stmp,2);
	    next->seq = ntohs(stmp);
	    ctlptr += 2;
	}
	else { /* No packet number specified, so this is the only one */
	    next->seq = 1;
	    nd_pkts = 1;
	}
	if(hdr_len >= 7) {	    /* Total number of packets */
	    bcopy(ctlptr,&stmp,2);  /* 0 means don't know      */
	    if(stmp) nd_pkts = ntohs(stmp);
	    ctlptr += 2;
	}
	if(hdr_len >= 9) {	/* Receievd through */
	    bcopy(ctlptr,&stmp,2);  /* 1 means received request */
#ifndef USE_V3_PROT
	    if((stmp) && (ntohs(stmp) == 1)) {
		/* Future retries will be acks only */
		pkt->length = 9;
		bcopy(&zero,pkt->start+3,2);
#ifdef DEBUG
		if(pfs_debug > 2) 
		    fprintf(stderr,"Server acked request - retries will be acks only\n");
#endif
	    }
#endif
	    ctlptr += 2;
	}
	if(hdr_len >= 11) {	/* Backoff */
	    bcopy(ctlptr,&stmp,2);
	    if(stmp) {
		backoff = ntohs(stmp);
#ifdef DEBUG
		if(pfs_debug > 2) 
		    fprintf(stderr,"Backing off to %d seconds\n", backoff);
#endif
		timeout.tv_sec = backoff;
	    }
	    ctlptr += 2;
	}
	if(hdr_len >= 12) {	/* Flags (1st byte) */
	    bcopy(ctlptr,&rdflag11,1);
	    if(rdflag11 & 0x80) {
#ifdef DEBUG
		if(pfs_debug > 2) 
		    fprintf(stderr,"Ack requested\n");
#endif
		ackpend++;
	    }
	    if(rdflag11 & 0x40) {
#ifdef DEBUG
		if(pfs_debug > 2) 
		    fprintf(stderr,"Sequenced control packet\n");
#endif
		next->length = -1;
		scpflag++;
	    }
	    ctlptr += 1;
	}
	if(hdr_len >= 13) {	/* Flags (2nd byte) */
	    /* Reserved for future use */
	    bcopy(ctlptr,&rdflag12,1);
	    ctlptr += 1;
	}
	if(next->seq == 0) goto keep_waiting;
	if(next->length >= 0) next->length -= hdr_len;
	next->start += hdr_len;
	goto done_old;
    }

    pkt_cid = 0;

    /* if intermediate format (between old and new), then process */
    /* and go to done_old                                         */
    ctlptr = next->start + max(0,next->length-20);
    while(*ctlptr) ctlptr++;
    /* Control fields start after the terminating null */
    ctlptr++;
    /* Until old version are gone, must be 4 extra bytes minimum */
    /* When no version 3 servers, can remove the -4              */
    if(ctlptr < (next->start + next->length - 4)) {
	/* Connection ID */
	bcopy(ctlptr,&stmp,2);
	if(stmp) pkt_cid = ntohs(stmp);
	ctlptr += 2;
	/* Packet number */
	if(ctlptr < (next->start + next->length)) {
	    bcopy(ctlptr,&stmp,2);
	    next->seq = ntohs(stmp);
	    ctlptr += 2;
	}
	/* Total number of packets */
	if(ctlptr < (next->start + next->length)) {
	    bcopy(ctlptr,&stmp,2);
	    if(stmp) nd_pkts = ntohs(stmp);
	    ctlptr += 2;
	}
	/* Receievd through */
	if(ctlptr < (next->start + next->length)) {
	    /* Not supported by clients */
	    ctlptr += 2;
	}
	/* Backoff */
	if(ctlptr < (next->start + next->length)) {
	    bcopy(ctlptr,&stmp,2);
	    backoff = ntohs(stmp);
#ifdef DEBUG
	    if(pfs_debug > 2) 
		fprintf(stderr,"Backing off to %d seconds\n", backoff);
#endif
	    if(backoff) timeout.tv_sec = backoff;
	    ctlptr += 2;
	}
	if(next->seq == 0) goto keep_waiting;
	goto done_old;

    }

    /* Notes that we have to start searching 11 bytes before the    */
    /* expected start of the MULTI-PACKET line because the message  */
    /* might include up to 10 bytes of data after the trailing null */
    /* The order of those bytes is two bytes each for Connection ID */
    /* Packet-no, of, Received-through, Backoff                     */
    seqtxt = nlsindex(next->start + max(0,next->length - 40),"MULTI-PACKET"); 
    if(seqtxt) seqtxt+= 13;

    if((nd_pkts == 0) && (no_pkts == 0) && (seqtxt == NULL)) goto all_done;

    tmp = sscanf(seqtxt,"%d OF %d", &(next->seq), &nd_pkts);
#ifdef DEBUG    
    if (pfs_debug && (tmp == 0)) 
	fprintf(stderr,"Cant read packet sequence number: %s", seqtxt);    
#endif
 done_old:
#ifdef DEBUG
    if(pfs_debug > 2) fprintf(stderr,"Packet %d of %d\n",next->seq,nd_pkts);
#endif
    if ((first == next) && (no_pkts == 0)) {
	if(first->seq == 1) {
	    comp_thru = first;
	    /* If only one packet, then return it */
	    if(nd_pkts == 1) goto all_done;
	}
	else gaps++;
	no_pkts = 1;
	next = ptalloc();
	goto keep_waiting;
    }
	
    if(comp_thru && (next->seq <= comp_thru->seq))
	ptfree(next);
    else if (next->seq < first->seq) {
	vtmp = first;
	first = next;
	first->next = vtmp;
	first->previous = NULL;
	vtmp->previous = first;
	if(first->seq == 1) comp_thru = first;
	no_pkts++;
    }
    else {
	vtmp = (comp_thru ? comp_thru : first);
	while (vtmp->seq < next->seq) {
	    if(vtmp->next == NULL) {
		vtmp->next = next;
		next->previous = vtmp;
		next->next = NULL;
		no_pkts++;
		goto ins_done;
	    }
	    vtmp = vtmp->next;
	}
	if(vtmp->seq == next->seq)
	    ptfree(next);
	else {
	    vtmp->previous->next = next;
	    next->previous = vtmp->previous;
	    next->next = vtmp;
	    vtmp->previous = next;
	    no_pkts++;
	}
    }   

ins_done:
	
    while(comp_thru && comp_thru->next && 
	  (comp_thru->next->seq == (comp_thru->seq + 1))) {
	comp_thru = comp_thru->next;
#ifndef USE_V3_PROT
	recvd_thru = htons(comp_thru->seq);
	bcopy(&recvd_thru,pkt->start+7,2); /* Let server know we got it */
#endif
	/* We've made progress, so reset retry count */
	retries = client_dirsrv_retry;
	/* Also, next retry will be only an acknowledgement */
	/* but for now, we can't fill in the ack field      */
#ifdef DEBUG
	if(pfs_debug > 2) 
	    fprintf(stderr,"Packets now received through %d\n",comp_thru->seq);
#endif
    }

    /* See if there are any gaps */
    if(!comp_thru || comp_thru->next) gaps++;
    else gaps = 0;

    if ((nd_pkts == 0) || (no_pkts < nd_pkts)) {
	next = ptalloc();
	goto keep_waiting;
    }

 all_done:
    if(ackpend) { /* Send acknowledgement if requested */
#ifdef DEBUG
	if (pfs_debug > 2) {
	    if (to.sin_family == AF_INET)
		fprintf(stderr,"Acknowledging final packet to %s(%d)\n",
			inet_ntoa(*(struct in_addr *)&to.sin_addr), ntohs(this_conn_id));
            else
                fprintf(stderr,"Acknowledging final packet\n");
	    (void) fflush(stderr);
	}
#endif
	ns = sendto(lp,(char *)(pkt->start), pkt->length, 0, (struct sockaddr *)&to, S_AD_SZ);
	if(ns != pkt->length) {
#ifdef DEBUG
	    if (pfs_debug) {
		fprintf(stderr,"\nsent only %d/%d: ",ns, pkt->length);
		perror("");
	    }
#endif
	}

    }
    close(lp);
    ptlfree(pkt);

    /* Get rid of any sequenced control packets */
    if(scpflag) {
	while(first && (first->length < 0)) {
	    vtmp = first;
	    first = first->next;
	    if(first) first->previous = NULL;
	    ptfree(vtmp);
	}
	vtmp = first;
	while(vtmp && vtmp->next) {
	    if(vtmp->next->length < 0) {
		if(vtmp->next->next) {
		    vtmp->next = vtmp->next->next;
		    ptfree(vtmp->next->previous);
		    vtmp->next->previous = vtmp;
		}
		else {
		    ptfree(vtmp->next);
		    vtmp->next = NULL;
		}
	    }
	    vtmp = vtmp->next;
	}
    }

    return(first);

}
