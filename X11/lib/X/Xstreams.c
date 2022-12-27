/*
 * $XConsortium: Xstreams.c,v 1.12 89/11/08 17:08:29 converse Exp $
 */

#ifdef STREAMSCONN
/*
 * Copyright 1988, 1989 AT&T, Inc.
 *
 * Permission to use, copy, modify, and distribute this software and its
 * documentation for any purpose and without fee is hereby granted, provided
 * that the above copyright notice appear in all copies and that both that
 * copyright notice and this permission notice appear in supporting
 * documentation, and that the name of AT&T not be used in advertising
 * or publicity pertaining to distribution of the software without specific,
 * written prior permission.  AT&T makes no representations about the
 * suitability of this software for any purpose.  It is provided "as is"
 * without express or implied warranty.
 *
 * AT&T DISCLAIMS ALL WARRANTIES WITH REGARD TO THIS SOFTWARE, INCLUDING ALL
 * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS, IN NO EVENT SHALL AT&T
 * BE LIABLE FOR ANY SPECIAL, INDIRECT OR CONSEQUENTIAL DAMAGES OR ANY DAMAGES
 * WHATSOEVER RESULTING FROM LOSS OF USE, DATA OR PROFITS, WHETHER IN AN ACTION
 * OF CONTRACT, NEGLIGENCE OR OTHER TORTIOUS ACTION, ARISING OUT OF OR IN 
 * CONNECTION WITH THE USE OR PERFORMANCE OF THIS SOFTWARE.
 *
*/

/*
 Xstreams.c (C source file)
	Acc: 575561834 Tue Apr 26 09:17:14 1988
	Mod: 575561843 Tue Apr 26 09:17:23 1988
	Sta: 575561843 Tue Apr 26 09:17:23 1988
	Owner: 2011
	Group: 1985
	Permissions: 664
*/
/*
	START USER STAMP AREA
*/
/*
  	END USER STAMP AREA
*/

#define _USHORT_H	/* prevent conflicts between BSD sys/types.h and
                           interlan/il_types.h */

#define NEED_REPLIES
#include <stdio.h>
#include <X11/Xos.h>
#include "Xlibint.h"
#include <X11/Xauth.h>
#include <X11/Xproto.h>

#include <tiuser.h>
#include <sys/param.h>
#include <sys/utsname.h>
#include <sys/signal.h>

#ifdef SHARELIB
#undef t_bind
#endif

#include "Xstreams.h"


#ifdef DEBUG
#define PRMSG(x,a,b)	fprintf(stderr, x,a,b); fflush(stderr)
#else
#define PRMSG(x,a,b)
#endif

#define	LISTEN_QUE_SIZE	8	/* maximum # of connections for gen. listen */
#define	CLEAR		1

/*
 * Ridiculously high value for maximum number of connects per stream.
 * Transport Provider will determine actual maximum to be used.
 */

#define	MAXCONNECTIONS	100	/* maximum # of connections for gen. listen */


#define MAXLEN	80
#define BUFFERSIZE 2048
#define NOBUFFERNEEDED 512
#define X_TCP_PORT	6000

char * _XsInputBuffer[NOFILES_MAX] = {NULL};
int _XsInputBuffersize[NOFILES_MAX] = {0};
int _XsInputBufferptr[NOFILES_MAX] = {0};

static char	*ptmx = "/dev/ptmx";
static char	*dispno = "0";

static  char _dispno[20];

extern int t_errno;

char	** addheader();
char	** addtliheader();

static	struct	t_bind bind_ret, bind_req;
static	struct	t_call call;
static  char	ret_buf[MAXLEN], req_buf[MAXLEN], call_buf[MAXLEN];

void CloseTcpStream () {}
void WriteTcpStream () {}
void ReadTcpStream () {}
void CallTcpServer () {}
void ConnectTcpClient () {}
void SetupTcpStream () {}

#ifdef SHARELIB
#define t_bind (*_libX_t_bind)
extern int t_bind();
#undef t_bind
#endif

extern int t_errno;
typedef void (*PFV)();
PFV signal();

#define SUCCESS		"1"

extern char _XsTypeOfStream[];
extern Xstream _XsStream[];

static networkInfo Network;
static int NameServer = -1;

/* Routines everybody shares */

ErrorCall()
{
	fprintf(stderr, "ErrorCall: invalid or unsupported subroutine call\n");
	return(-1);
}

/*
 * Following are some general queueing routines.  The call list head contains
 * a pointer to the head of the queue and to the tail of the queue.  Normally,
 * calls are added to the tail and removed from the head to ensure they are
 * processed in the order received, however, because of the possible interruption
 * of an acceptance with the resulting requeueing, it is necessary to have a
 * way to do a "priority queueing" which inserts at the head of the queue for
 * immediate processing
 */

/*
 * Que:
 *
 * add calls to tail of queue
 */


static	void
Que(head, lc, flag)
register struct listenQue *head;
register struct listenCall *lc;
char	 flag;
{
	if(flag == CLEAR)
		ClearCall(lc->CurrentCall);
	
	if (head->QueTail == (struct listenCall *) NULL) {
		lc->NextCall = (struct listenCall *) NULL;
		head->QueHead = head->QueTail = lc;
	}
	else {
		lc->NextCall = head->QueTail->NextCall;
		head->QueTail->NextCall = lc;
		head->QueTail = lc;
	}
}


/*
 * pQue:
 *
 * priority queuer, add calls to head of queue
 */

static void
pQue(head, lc)
register struct listenQue *head;
register struct listenCall *lc;
{
	if (head->QueHead == (struct listenCall *) NULL) {
		lc->NextCall = (struct listenCall *) NULL;
		head->QueHead = head->QueTail = lc;
	}
	else {
		lc->NextCall = head->QueHead;
		head->QueHead = lc;
	}
}


/*
 * dequeue:
 *
 * remove a call from the head of queue
 */


static struct listenCall *
deQue(head)
register struct listenQue *head;
{
	register struct listenCall *ret;

	if (head->QueHead == (struct listenCall *) NULL){
		PRMSG("Fatal error. Queue is empty (shouldn't happen)\n",0,0);
		exit(1);	
		}
	ret = head->QueHead;
	head->QueHead = ret->NextCall;
	if (head->QueHead == (struct listenCall *) NULL)
		head->QueTail = (struct listenCall *) NULL;
	return(ret);
}


/* Routines for handling local streams (streams-pipes) */

SetupLocalStream(display, stype)
char *	display;
char	*stype;
{
	int 	munix, sunix;
	char *	slave;
	char	buf[64];
	int	type = X_LOCAL_STREAM;
	int	nameserver();

	PRMSG("Calling SetupLocalStream()\n",0,0);

	SetupNetworkInfo();
	dispno = display;

	NetworkInfo->_nnets = NetworkInfo->_npeers = 0;
	NetworkInfo->_peer = NULL;
	NetworkInfo->_peerlen = NULL;

	NetworkInfo->_net[0] = "local";
	NetworkInfo->_nnets++;


	munix = atoi(display);
	if(munix != 0){
		fprintf(stderr, "Only display # 0 can be used on this server\n");
		return(-1);
		}
			
	sprintf(buf, "%s.%d", LOCAL_LISTENER, munix);
	if(open(buf, O_RDWR) >= 0){
		fprintf(stderr, "Server is already running\n");
		return(-1);
		}
	if( (munix = open(ptmx, O_RDWR)) < 0 ){
		fprintf(stderr,"Cannot open %s", ptmx);
		perror(" ");
		return(-1);
	}
	grantpt(munix);
	unlockpt(munix);

	if(unlink(buf) < 0 && errno != ENOENT){
		fprintf(stderr, "Cannot unlink %s", buf);
		perror(" ");
		return(-1);
		}

	slave = (char *) ptsname(munix);
	if( link(slave, buf) <0 ){
		fprintf(stderr, "Cannot link %s to %s", slave, buf);
		perror(" ");
		return(-1);
		}
	if( chmod(buf, 0666) < 0){
		fprintf(stderr, "Cannot chmod %s", buf);
		perror(" ");
		return(-1);
		}

	sunix = open(buf, O_RDWR);
	if(sunix < 0){
		fprintf(stderr, "Cannot open %s", buf);
		perror(" ");
		close(munix);
		return(-1);
		}

	_XsTypeOfStream[munix] = type;
	_XsTypeOfStream[sunix] = CLOSED_STREAM;

	return(munix);
}

ConnectLocalClient(ufd, MoreConnections)
int	ufd;
char	* MoreConnections;
{
	
	int fd;
	int read_in;
	char length;
	char buf[64];

	PRMSG("Calling ConnectLocalClient(%d)\n", ufd,0);

/* MoreConnections is set to zero because if any more connections are underway
 * select() will return immediately. It is nicer if we can process all connections
 * that exist the way we handle TLI connections by setting MoreConnections.
 * May be I will end up doing it later.
 */
	*MoreConnections = 0;

	if( (read_in = read(ufd, &length, 1)) <= 0 ){
		if( !read_in )  /* client closed fd */
			perror("0 bytes read");
		else	perror("Error in reading the local connection msg length");
		return(-1);
		}


	if( (read_in = read(ufd, buf, length)) <= 0 ){
		if( !read_in )  /* client closed fd */
			perror("0 bytes read");
		else	perror("Error in reading the local connection slave name");
		return(-1);
		}

	buf[ length ] = '\0';

	if( (fd = open(buf,O_RDWR)) < 0 ){
		strcat(buf," open fail, clientfd");
		perror(buf);
		return(-1);
		}

	write(fd,SUCCESS,1);

	_XsTypeOfStream[fd] = _XsTypeOfStream[ufd];
        if (_XsInputBuffer[fd] == NULL)
            if ((_XsInputBuffer[fd] = (char *) Xmalloc(BUFFERSIZE)) == NULL)
               {
	       errno = ENOMEM;
               perror("Server can't connect local client");
               return (-1);
               }
        _XsInputBuffersize[fd] = 0;
        _XsInputBufferptr[fd] = 0;
	PRMSG("ConnectLocalClient(%d) return success\n", ufd,0);
	return(fd);
}

static void dummy (sig)
int sig;
{
}

CallLocalServer(host, idisplay, nettype)
char	*host;
int	idisplay;
char	*nettype;
{
	char	buf[64];
	int	type = X_LOCAL_STREAM;
	int	fd;


	PRMSG("Calling CallLocalServer(%s)\n", host,0);

        sprintf(_dispno, "%d", idisplay);
	dispno = _dispno;

	/*
	 * Open channel to server
	 */
	
	sprintf(buf, "%s.%d", LOCAL_LISTENER, idisplay);

	if((fd = OpenLocalServer(buf)) < 0)
	{
		PRMSG("Cannot open %s\n", buf,0);
#ifdef DEBUG
		perror("XIO");	/* Sorry, but I don't have the dpy handy */
#endif
		return(-1);
	}

	_XsTypeOfStream[fd] = type;
        if (_XsInputBuffer[fd] == NULL)
		if ((_XsInputBuffer[fd] = (char *) Xmalloc(BUFFERSIZE)) == NULL)
        	{
	               errno = ENOMEM;
                       perror("Client can't connect to local server");
                       return (-1);
        	}
        _XsInputBuffersize[fd] = 0;
        _XsInputBufferptr[fd] = 0;
	PRMSG("Calling CallLocalServer(%s) return success\n", host,0);

	return(fd);
}

static int
OpenLocalServer(node)
char	*node;
{
	int 	server, fd, c;
	char	buf[64], *slave;
	int	(*savef)();

	if ((server = open (node, O_RDWR)) < 0) 
	{
#ifdef DEBUG
		fprintf(stderr, "open(%s) failed\n", node);
		perror(" ");
#endif
		return(-1);
	}

	/*
	 * Open streams based pipe and get slave name
	 */


	if ((fd = open (ptmx, O_RDWR)) < 0) {
		close (server);
		PRMSG("Cannot open %s\n", ptmx, 0);
		return(-1);
	}

	grantpt (fd);

	unlockpt (fd);

	slave = (char *) ptsname (fd);

	if (chmod(slave, 0666) < 0) 
	{
		close(fd);
		close(server);
		PRMSG("Cannot chmod %s\n", buf,0);
		return(-1);
	}

	c = strlen (slave);
	
	buf[0] = c;
	sprintf(&buf[1], slave);
	
	/*
	 * write slave name to server
	 */

	write(server, buf, c+1);
	close (server);
	/*
	 * wait for server to respond
	 */
	savef = (int (*)()) signal (SIGALRM, dummy);
	alarm (30);

	if (read (fd, &c, 1) != 1) 
	{
		fprintf(stderr, "No reply from the server.\n");
		close(fd);
		fd = -1;
	}	
	alarm (0);
	signal (SIGALRM, savef);

	return(fd);
}

ReadLocalStream(fd, buf, count, do_buffering)
int	fd;
char	*buf;
int	count;
int	do_buffering;
{

	if (do_buffering == NO_BUFFERING)
		return (read(fd, buf, count));

        if (_XsInputBuffersize[fd] <= _XsInputBufferptr[fd])
           {
           errno = 0;
           _XsInputBuffersize[fd] = read(fd, _XsInputBuffer[fd], BUFFERSIZE);
           _XsInputBufferptr[fd] = 0;
           }

        if (_XsInputBuffersize[fd] > 0)
           {
           register i = _XsInputBuffersize[fd] - _XsInputBufferptr[fd];

	   i = i > count ? count : i;
	   memcpy(buf, &_XsInputBuffer[fd][_XsInputBufferptr[fd]], i);
	   _XsInputBufferptr[fd] += i;
	   return i;
           }
       else
          return (_XsInputBuffersize[fd]);
       
}

WriteLocalStream(fd, buf, count)
int	fd;
char	*buf;
int	count;
{
	return (write(fd, buf, count));
}

CloseLocalStream(fd)
int	fd;
{
	return (close(fd));
} 

int	timeout = 0;
SetTimeout()
{
	timeout = 1;
}

/* Routines for handling TLI streams */

SetupTliStream(display, stype)
char *	display;
char	*stype;
{
	int	i, n;
	int	fd, type;
	struct	utsname  machine;
	struct listenCall *tmp;
	int	nameserver();

        if(NameServer < 0 &&
		 (NameServer = OpenLocalServer(NAME_SERVER_NODE)) < 0)
	{
                        return(-1);
	}

	dispno = display;

	if(uname(&machine) < 0){
		t_error("Cannot get nodename");
		return(-2);
		}
		
	bind_req.addr.buf = req_buf;
	n = strlen(stype) +1;
	type = Network._nnets;
	if((Network._net[type] = Xmalloc(n)) == NULL){
             		PRMSG( "Xmalloc failed\n",0,0);
			return(-2);
			}

	bcopy(stype, Network._net[type], n);

	PRMSG("Calling SetupTliStream()\n",0,0);

	bind_ret.addr.buf = ret_buf;
	call.addr.buf	  = call_buf;
	bind_req.addr.maxlen = MAXLEN;
	bind_ret.addr.maxlen = MAXLEN;
	call.addr.maxlen     = MAXLEN;

fd = OpenAndBind(machine.nodename, atoi(display), MAXCONNECTIONS, Network._net[type], type);
	
	if( fd < 0){
		PRMSG("Cannot OpenAndBind %s", machine.nodename,0);
		Xfree(Network._net[type]);
		return(-2);
		}
	_XsTypeOfStream[fd] = type;

/*
 * set up call save list for general network listen service
 */
	for (i = 0; i < LISTEN_QUE_SIZE; ++i) 
	{
	    if((tmp = (struct listenCall *) Xmalloc(sizeof(struct listenCall))) == NULL)
	    {
			PRMSG( "Xmalloc failed\n",0,0);
			exit(1);
	    }
	    if((tmp->CurrentCall = (struct t_call *) t_alloc(fd,T_CALL,T_ALL)) == NULL)
            {
			PRMSG( "t_alloc failed\n",0,0);
			exit(1);
	    }
	    Que(&Network.FreeList[type], tmp, CLEAR);
	}

	if(Network._npeers > 0 && Network._peer == NULL)
	{
		register	i;
		register	char	*ptr;
		int		n;

		n =  (Network._npeers + 1) * 
			(sizeof(int) + sizeof(char *) + (1 + UNAME_LENGTH));

		PRMSG("Allocating %d chars for %d peeers names", 
							n, Network._npeers);

		if((ptr = Xmalloc(n)) == NULL){
			fprintf(stderr,"Cannot Xmalloc space for peers names\n");
			exit(1);
		}

		ptr = (char *) ((((unsigned) (ptr + 3)) >> 2) << 2);
		Network._peerlen = (int *) ptr;
		ptr += Network._npeers * sizeof(int);
		Network._peer = (char **) ptr; 
		ptr += Network._npeers * sizeof(char *);
		for(i= 0; i< Network._npeers; i++)
		{
			Network._peerlen[i] = 0;
			Network._peer[i]    = ptr;
#ifdef DEBUG
			fprintf(stderr, "peer[%d] is %u; peerlen[%d] is %u\n",
				i, Network._peer[i], i, &Network._peerlen[i]);
#endif
			ptr += (1 + UNAME_LENGTH);
		}
	}
	PRMSG("SetupTliStream () (success) fd = %d\n", fd,0);
	return(fd);
}

ConnectTliClient(sfd,MoreConnections)
int    sfd;
char  * MoreConnections;
{
	register	char	type = _XsTypeOfStream[sfd];
	register	struct  listenQue *freeq, *pendq;

	freeq = &Network.FreeList[type];
	pendq = &Network.PendingQue[type];

	PRMSG("Calling ConnectTliClient(%d)\n", sfd,0);
	LookForEvents(freeq, pendq, sfd);
	return (CheckListenQue(freeq, pendq, sfd, MoreConnections));
}

CallTliServer(host, idisplay, nettype)
char	*host;
int	idisplay;
char	*nettype;
{
	int	fd, i, t, (*savef)();
	char	*retptr, *ptr;
	char	first = 1;
	static	char	netbuffer[80];

	PRMSG("Calling CallTliServer()\n",0,0);

        if(NameServer<0 && (NameServer = OpenLocalServer(NAME_SERVER_NODE)) < 0)
	{
			return(-1);
	}

	i = strlen(nettype);
	SetupNetworkInfo();
	Network._nnets = 2;
	Network._net[1] = netbuffer;
	if(i > 79)
	{
		i = 79;
		nettype[i] = '\0';
	}
	bcopy(nettype, Network._net[1], i +1);


	
	sprintf(_dispno, "%d", idisplay);
	dispno = _dispno;

	savef = (int (*)()) signal (SIGALRM, dummy);

/* 
 * Give up after MAX_TRIES tries or for CONNECT_TIME seconds or an error
 * occurred which comes first.
*/

	retptr = NULL;

#define MAX_TRIES	3
#define CONNECT_TIME    10

	alarm (CONNECT_TIME);
	for(i=0; i < MAX_TRIES;i++)
	{
		t = -1;
	
		if((fd = OpenAndBind(NULL, -1, 0, nettype, 1)) < 0){
				PRMSG("Openandbind failed\n",0,0);
				break;	
			}
	 	if(first)
		{
			first = 0;
	   		if( GetNetworkInfo (fd, nettype, ConvertNameToTliCall,
				 addheader(host, strlen(host)),  &retptr, NULL) <= 0)
			{
				fprintf(stderr, "Cannot create address for system %s \n", host);
				return(-1);
	   		}

			first = 0;
			ptr = retptr;

	   		call.addr.len = ((xHostEntry *) ptr)->length;
	   		call.addr.maxlen = ((xHostEntry *) ptr)->length;
			call.addr.buf = (ptr+sizeof(xHostEntry));
		
			call.addr.buf[call.addr.len] = '\0';
#ifdef DEBUG
			fprintf(stderr, "ADDRESS LENGTH IS %d\n", call.addr.len);
			fprintf(stderr, "Address returned is <%s>\n",call.addr.buf);
#endif
			ptr += (((sizeof(xHostEntry) + call.addr.len+3) >> 2) << 2);

			call.opt.len = ((xHostEntry *) ptr)->length;
			call.opt.maxlen = ((xHostEntry *) ptr)->length;
			if(call.opt.len > 0)
				call.opt.buf = (ptr+sizeof(xHostEntry));
			else	call.opt.buf = NULL;
		
			ptr += (((sizeof(xHostEntry) + call.opt.len+3) >> 2) << 2);

			call.udata.len = ((xHostEntry *) ptr)->length;
			call.udata.maxlen = ((xHostEntry *) ptr)->length;
			if(call.udata.len > 0){
				call.udata.buf = (ptr+sizeof(xHostEntry));
#ifdef DEBUG
			fprintf(stderr, "ADDRESS LENGTH IS %d\n", call.udata.len);
                        fprintf(stderr, "Address returned is <%s>\n",call.udata.buf);
#endif
			}
			else	call.udata.buf = NULL;
#ifdef DEBUG
			fprintf(stderr, "addrlen %d optlen %d udatalen %d\n",
					call.addr.len,
					call.opt.len,
					call.udata.len);
#endif
			close(NameServer);
			NameServer = -1;
		}

		PRMSG("Connecting to %s ... \n", host, 0);
		if((t = t_connect(fd, &call, NULL)) < 0)
		{
			if(t_errno == TLOOK)
			{
				checkNewEvent(fd);
		        	t_close(fd);
				continue;
			}
			else
			{
				t_error("t_connect failed");
				t_close(fd);
				break;
			}
		} else break;
	}

#undef MAX_TRIES
#undef CONNECT_TIME

	alarm (0);
	signal (SIGALRM, savef);
	if(retptr != NULL)
		Xfree(retptr);
	if(t < 0)
	{
		return(-1);
	}

/*
	if (t_rcvconnect (fd, &call) < 0) {
		if(t_errno == TLOOK)
			checkNewEvent(fd);
		t_close(fd);
		t_error ("t_rcvconnect failed!");
		return(-1);
	}
*/

	PRMSG("A Connection has been established to %s ... \n", host,0);
	_XsTypeOfStream[fd] = 1;
        if (_XsInputBuffer[fd] == NULL)
            if ((_XsInputBuffer[fd] = (char *) Xmalloc(BUFFERSIZE)) == NULL)
               {
	       errno = ENOMEM;
               perror("Client can't connect to remote server");
               return (-1);
               }
        _XsInputBuffersize[fd] = 0;
        _XsInputBufferptr[fd] = 0;
	PRMSG("CallTliServer() returns success\n",0,0);

	return(fd);
}

static
checkNewEvent(fd)
int	fd;
{
	int	t;

        t = t_look(fd);
        switch(t)
        {
        case T_DATA	  :
	        fprintf(stderr, "T_DATA received\n");	    
		break;
  	case T_EXDATA	  :
       		fprintf(stderr, "T_EXDATA received\n");    
		break;
  	case T_DISCONNECT :
	        t_rcvdis(fd, NULL);
       		fprintf(stderr, "T_DISCONNECT received\n");
		break;
 	case T_ERROR	  :
        	fprintf(stderr, "T_ERROR received\n");	    
		break;
  	case T_UDERR	  :
        	fprintf(stderr, "T_UDERR received\n");	    
		break;
  	case T_ORDREL	  :
        	fprintf(stderr, "T_ORDREL received\n");    
		break;
  	}
}

ReadTliStream(fd, buf, count, do_buffering)
int	fd;
char	*buf;
int	count;
int	do_buffering;
{
	int	flags, n;

	PRMSG("In ReadTliStream fd (%d) count (%d) \n", fd, count);

	if (do_buffering == NO_BUFFERING){
                n = t_rcv(fd, buf, count, &flags);
		PRMSG("In ReadTliStream t_rcv(%d) returns %d chars\n", fd, n);
                return (n);
	}

        if (_XsInputBuffersize[fd] <= _XsInputBufferptr[fd])
           {
           errno = 0;
           _XsInputBuffersize[fd] = t_rcv(fd, _XsInputBuffer[fd], count, &flags);
           _XsInputBufferptr[fd] = 0;
           }

        if (_XsInputBuffersize[fd] > 0)
           {
           register i = _XsInputBuffersize[fd] - _XsInputBufferptr[fd];

	   i = i > count ? count : i;
	   memcpy(buf, &_XsInputBuffer[fd][_XsInputBufferptr[fd]], i);
	   _XsInputBufferptr[fd] += i;
	   return i;
           }
       else
          return (_XsInputBuffersize[fd]);
}

WriteTliStream(fd, buf, count)
int	fd;
char	*buf;
int	count;
{
	int	flags = 0, n;

	PRMSG("In WriteTliStream fd (%d) count (%d) \n", fd, count);

	if ((n = t_snd(fd, buf, count, &flags)) < 0 ) {
#ifdef DEBUG
		t_error("t_snd failed");
#endif
		return(-1);
	}
	return(n);
}

static void
OnError(sig)
int	sig;
{
}

CloseTliStream(fd)
int	fd;
{
	void	(*f)();

	f = signal(SIGALRM, OnError);
	alarm(2);
	
/*
	if(t_snddis(fd, NULL) < 0)
#ifdef DEBUG
		t_error("t_snddis failed")
#endif
		;

	if (t_unbind(fd) < 0) 
#ifdef DEBUG
		t_error("t_unbind failed")
#endif
		;
*/
	if (t_close(fd) <0)
#ifdef DEBUG
		t_error("t_close failed")
#endif
		;
TheEnd :
	_XsTypeOfStream[fd] = CLOSED_STREAM;	/* mark as invalid */
	alarm(0);
	signal(SIGALRM, f);
}

OpenAndBind(name, port, maxcon, nettype, type)
char *name;
int	port, maxcon, type;
char	*nettype;
{
	char	bind_buf[128];
	int	i, fd;
	char	clonedev[128];
	char	*retptr;
	
	sprintf(clonedev, "/dev/%s", nettype);
	if(name != NULL)
		PRMSG("OpenAndBind name %s, clonedev is %s \n",
				 name, clonedev);

	/* point to the virtual circuit */

	if ((fd = t_open(clonedev,  O_RDWR, NULL)) < 0)
	{
		fprintf(stderr, "Cannot open %s\n", clonedev);
#ifdef DEBUG
		t_error("t_open 1 failed");
#endif
		return(-1);
	}

	_XsTypeOfStream[fd] = type;

	/* fill in the request call structure with necessary infomation */

	if(name != NULL)
	{
	   if(GetNetworkInfo (fd, nettype, ConvertNameToTliBind, 
		addheader(name, strlen(name)), &retptr, NULL)<0)
	   {
		PRMSG("Cannot create address for system %s \n", name, 0);
		return(-1);
	   }
	   bind_req.addr.buf = bind_buf;
	   bind_req.addr.len = ((xHostEntry *) retptr)->length;
	   bcopy (retptr+sizeof(xHostEntry), bind_buf, bind_req.addr.len);
	   Xfree(retptr);
	   bind_buf[bind_req.addr.len] = '\0';
#ifdef DEBUG
	   fprintf(stderr, "ADDRESS LENGTH IS %d\n", bind_req.addr.len);
	   fprintf(stderr, "Address returned is <%s>\n", bind_buf);
#endif
	   bind_req.qlen = maxcon;

	/* bind the name to the transport endpoint.  This operation will */
	/* take some time if the name is not already in the local name  */
	/* table or if the name is not a group name   */
#ifdef SHARELIB
#define t_bind (*_libX_t_bind)
#endif
		i = t_bind (fd, &bind_req, NULL) ;
	}
	else 	i = t_bind (fd, NULL, NULL);
#ifdef SHARELIB
#undef t_bind
#endif

	if(i < 0)
	{
		t_error("t_bind failed");
		close(fd);
		return(-1);
	}
	if(name != NULL)
		PRMSG("OpenAndBind(%s, %d) (success)\n", name, maxcon);
	return(fd);
}


/*
 * LookForEvents:	handle an asynchronous event
 */

static 
LookForEvents(FreeHead, PendHead, fd)
struct listenQue *FreeHead;
struct listenQue *PendHead;
int fd;
{
	int	address;
	short	port, nf;
	struct t_discon disc;
	register struct listenCall *current;
	register struct t_call *call;
	int t;
	char	buf[80];
	int	flag, i;

	if((t = t_look(fd)) < 0) {
		PRMSG("t_look failed. t_errno %d\n", t_errno,0);
		return(-1);
		}
	switch (t) {
	case 0:
		PRMSG("t_look 0\n",0,0);
		break;
		/* no return */
	case T_LISTEN:
		PRMSG("t_look T_LISTEN\n",0,0);
		current = deQue(FreeHead);
		call = current->CurrentCall;

		if (t_listen(fd, call) < 0) {
			PRMSG("t_listen failed\n",0,0);
			return;
		}

/*
	if(strcmp(Network._net[_XsTypeOfStream[fd]], "it") == 0)
	{
	nf = *(short *) call->addr.buf;
	port = *(short *) (call->addr.buf + sizeof(short));
	address = *(int *) (call->addr.buf + sizeof(short) + sizeof(short));
	printf("TCP address %d-%d-%d\n", nf, port, address);
	}
	else if(strcmp(Network._net[_XsTypeOfStream[fd]], "starlan") == 0)
	{
	call->udata.buf[call->udata.len] = '\0';
	printf("STARLAN address %s\n", call->udata.buf);
	}
*/
		Que(PendHead, current, ~CLEAR);
		PRMSG("incoming call seq # %d", call->sequence,0);
		break;
	case T_DISCONNECT:
		PRMSG("t_look T_DISCONNECT\n",0,0);
		if (t_rcvdis(fd, &disc) < 0) {
			PRMSG("Received T_DISCONNECT but t_rcvdis failed\n",0,0);
			exit(1);	
		}
		PRMSG("incoming disconnect seq # %d", disc.sequence,0);
		RemoveCall(FreeHead, PendHead, &disc);
		t_close(fd);
		_XsTypeOfStream[fd] = -1;
		break;
	case T_DATA :
		PRMSG("Reading from  %d\n",fd,0);
		if((i = t_rcv(fd, buf, 79, &flag)) > 0)
			PRMSG("Received on %d:\n%s\n", fd, buf);
		break;
	default:
		PRMSG("t_look default %o %x\n", t, t);
		break;
	}
}


/*
 * CheckListenQue:	try to accept a connection
 */

static 
CheckListenQue(FreeHead, PendHead, fd, MoreConnections)
struct listenQue *FreeHead;
struct listenQue *PendHead;
int fd;
char * MoreConnections;
{
	register struct listenCall *current;
	register struct t_call *call;
	int pid, nfd, n;
	char	*retptr, *ptr;

	int	address;
	short	port, nf;

	PRMSG( "in CheckListenQue",0,0);
	if (!(EMPTY(PendHead))) 
	{
		current = deQue(PendHead);
		call = current->CurrentCall;
		PRMSG( "try to accept #%d", call->sequence,0);
		if ((nfd = OpenAndBind(NULL, -1, 0, 
				Network._net[_XsTypeOfStream[fd]], _XsTypeOfStream[fd])) < 0)
		{
			PRMSG( "OpenAndBind failed\n",0,0);
			Que(FreeHead, current, CLEAR);
			*MoreConnections = !EMPTY(PendHead);
			return(-1);  /* let transport provider generate disconnect */
		}

/*
        fprintf(stderr, "Trying to Accept a call from Network <<%s>>>\n",
                        Network._net[_XsTypeOfStream[fd]]);

        if(strcmp(Network._net[_XsTypeOfStream[fd]], "it") == 0)
	{
        nf = *(short *) call->addr.buf;
	port = *(short *) (call->addr.buf + sizeof(short));
	address = *(int *) (call->addr.buf + sizeof(short) + sizeof(short));
	printf("TCP address %d-%d-%d\n", nf, port, address);
	}
        else if(strcmp(Network._net[_XsTypeOfStream[fd]], "starlan") == 0)
	{
        call->udata.buf[call->udata.len] = '\0';
	printf("STARLAN address %s\n", call->udata.buf);
	}

		
	fprintf(stderr, "calling t_accept\n");
*/
		n = t_accept(fd, nfd, call);
		if (n < 0){

			PRMSG( "t_accept failed\n",0,0);
			if (t_errno == TLOOK) {
				t_close(nfd);
				PRMSG( "t_accept collision",0,0);
				PRMSG( "save call #%d", call->sequence,0);
				pQue(PendHead, current);
				*MoreConnections = !EMPTY(PendHead);
				return(-1);
			}
			else {
				PRMSG( "t_accept failed but not t_look\n",0,0);
				t_close(nfd);
				Que(FreeHead, current, CLEAR);
				*MoreConnections = !EMPTY(PendHead);
				return(-1);
			}
		}
		_XsTypeOfStream[nfd] = _XsTypeOfStream[fd];
		retptr = NULL;

		if( GetNetworkInfo (nfd, NULL, ConvertTliCallToName,
					addtliheader(call),  &retptr, NULL) <= 0)
		{
			retptr = NULL;

		}
		ptr = NULL;
		if(retptr != NULL)
		{
			ptr = retptr;
			retptr += sizeof(xHostEntry);
		}
		GetNetworkInfo (nfd, NULL, PEER_ALLOC, &retptr, NULL);
		if(ptr != NULL)
			Xfree(ptr);
		PRMSG( "Accepted call %d", call->sequence,0);
		PRMSG("Channel %d is opened\n", nfd,0);

		Que(FreeHead, current, CLEAR);
        	if (_XsInputBuffer[nfd] == NULL)
                    if ((_XsInputBuffer[nfd] = (char *)Xmalloc(BUFFERSIZE)) == NULL)
                       {
	               errno = ENOMEM;
                       perror("Server can't connect remote client");
                       return (-1);
                       }
       		_XsInputBuffersize[nfd] = 0;
        	_XsInputBufferptr[nfd] = 0;
		*MoreConnections = !EMPTY(PendHead);
		return(nfd);
	}	

	*MoreConnections = !EMPTY(PendHead);
	return(-1);
}


/*
 * ClearCall:	clear out a call structure
 */

static
ClearCall(call)
struct t_call *call;
{
	call->sequence = 0;
	call->addr.len = 0;
	call->opt.len = 0;
	call->udata.len = 0;
	memset(call->addr.buf, 0, call->addr.maxlen);
	memset(call->opt.buf, 0, call->opt.maxlen);
	memset(call->udata.buf, 0, call->udata.maxlen);
}


/*
 * RemoveCall: remove call from pending list
 */

static
RemoveCall(freeq, pendq, disc)
struct listenQue *freeq;
struct listenQue *pendq;
struct t_discon *disc;
{
	register struct listenCall *p, *oldp;

	PRMSG( "Removing call, sequence # is %d", disc->sequence,0);
	if (EMPTY(pendq)) {
		disc->sequence = -1;
		return;
	}
	p = pendq->QueHead;
	oldp = (struct listenCall *) NULL;
	while (p) {
		if (p->CurrentCall->sequence == disc->sequence) {
			if (oldp == (struct listenCall *) NULL) {
				pendq->QueHead = p->NextCall;
				if (pendq->QueHead == (struct listenCall *) NULL) {
					pendq->QueTail = (struct listenCall *) NULL;
				}
			}
			else if (p == pendq->QueTail) {
				oldp->NextCall = p->NextCall;
				pendq->QueTail = oldp;
			}
			else {
				oldp->NextCall = p->NextCall;
			}
			Que(freeq, p, CLEAR);
			disc->sequence = -1;
			return;
		}
		oldp = p;
		p = p->NextCall;
	}
	disc->sequence = -1;
	return;
}

static	char	*erazeComment(line)
char	*line;
{
 	char	*ptr = line;

        while(*ptr <= ' ' && *ptr != '\0')
                        ptr++;
/*
 *	If you want to check the version do it here
 *if( strncmp(ptr, "#VERSION", 8) == 0)
 *			return(NULL);
 */
   	if(*ptr == '\0' || *ptr == '#'){
                        return(NULL);
                        }
	line = ptr;
	while(*ptr != '\0' && *ptr != '#')
                        ptr++;
	*ptr = '\0';
	return(line);
}


static char	*XNETDB = "/usr/X/lib/Xconnections";

int _XMakeStreamsConnection (name, idisplay, retries,
			     familyp, serveraddrlenp, serveraddrp)
char	*name;
int	idisplay;
int	retries;
int	*familyp;
int	*serveraddrlenp;
char	**serveraddrp;
{
	struct	utsname	 machine;
	register	i;
	FILE	*file;
	char	*line, *ptr, buf[160];
	int	fd = -1, nfound = 0, n;
	char	netype[128], sysname[128], nodname[128];
	char	*procname = "Xlib/_XMakeStreamsConnection";

	PRMSG("GetConnectionType(%s)\n", name, 0);

        if(uname(&machine) < 0){
		t_error("Cannot get nodename");
		return(-1);
		}

	if (!name || !*name ||
	    strcmp(name, "unix") == 0 ||
	    strcmp(name, "local") == 0 ||
	    strcmp(name, machine.nodename) == 0) {
	    /*
	     * auth information for local connection set above
	     */
	    fd = ((*_XsStream[X_LOCAL_STREAM].CallTheListener)
		  ("unix", idisplay, "local"));
	    if (fd >= 0) {
		*familyp = FamilyLocal;
		*serveraddrlenp = strlen (machine.nodename);
		*serveraddrp = (char *) Xmalloc ((*serveraddrlenp) + 1);
		if (!*serveraddrp) {
		    *serveraddrlenp = 0;
		} else {
		    strcpy (*serveraddrp, machine.nodename);
		}
	    }
	    return fd;
	}


	/*
	 * look through the connections database for the node to contact
	 */
	file = fopen(XNETDB, "r");
	if (!file) {
	    fprintf (stderr, "%s:  unable to open connection file %s\n",
		     procname, XNETDB);
	    return -1;
	}
	netype[0] = '\0';
	while((line = fgets(buf, 160, file)) != NULL) {
               	if((n = strlen(line)) > 1)
                              line[n-1] = '\0';
		if((ptr = erazeComment(line)) == NULL)
			continue;

		n = sscanf(ptr, "%s%s%s", sysname, nodname, netype);
		if (n >= 3) {
		    if (strcmp (sysname, name) == 0) {
			/* exact match */
			nfound++;
			fd = ((*_XsStream[X_TLI_STREAM].CallTheListener)
			      (nodname, idisplay, netype));
			if (fd >= 0) break;
		    } else if (strcmp(sysname, "*") == 0 ||
			       strcmp(nodname, "*") == 0) {
			/* wildcard match */
			nfound++;
			/* strcpy (nodname, name);*/
			fd = ((*_XsStream[X_TLI_STREAM].CallTheListener)     
			      (name, idisplay, netype));
			break;
		    }
		}
	}
	fclose(file);

	/*
	 * set the auth information; XXX - what kind?
	 */
	if (fd >= 0) {
	    /*
	     * XXX - until we figure out what kind, just default
	     */
	} else if (nfound == 0) {
	    fprintf (stderr,
		     "%s:  no entry for %s in connections file %s\n",
		     procname, name, XNETDB);
	}
	return fd;
}


#ifdef DEBUG
dump(buf, len)
char	*buf;
int	len;
{
 	int	i;
	if(buf != NULL)
		for(i=0; i< len; i++)
                        fprintf(stderr, "<%o> ", buf[i]);
	fprintf(stderr, "\n");
	fflush(stderr);
}

#endif

int
nameserver(fd, nettype, service, arg1, arg2, arg3)
    int       fd;
    char     *nettype;
    int	      service;
    char     **arg1, **arg2;
    int	     *arg3;
{

	char	*ptr;
	int	n;
	int	type;

	type = _XsTypeOfStream[fd];
PRMSG("in nameserver type %d, fd %d\n", type, fd);
	if(type < 1 || type > Network._nnets)
	{
		if(type == X_LOCAL_STREAM)
			return(0);
PRMSG( "in nameserver type %d unknown\n", type, fd);
		return(-1);
	}

        if(nettype == NULL)
		nettype = Network._net[type];


	switch(service){
	case	OpenDaemonConnection :
		if(NameServer < 0 )
                	NameServer = OpenLocalServer(NAME_SERVER_NODE);
         	return(NameServer);
	case	ConvertTliCallToName :
	case	ConvertNetAddrToName :
	case	ConvertNameToNetAddr :
        case	ConvertNameToTliBind :
	case	ConvertNameToTliCall :
                if((n = CallTheNameServer(service, nettype, arg1, arg2, arg3)) < 0)
                        return(-1);
                return(n);

	case	PEER_NAME  :
                if( fd < Network._npeers )
                {
			*arg2 = Network._peer[fd];
                	return(1);
		}
		return(-1);
	case	PEER_ALLOC :
		if(fd >= Network._npeers)
			return(-1);

		if(*arg1 == NULL){
			n = 0;
			}
		else	n = strlen(*arg1);

		Network._peerlen[fd] = n;

		if(n > 0){
			if(Network._peerlen[fd] > UNAME_LENGTH)
				Network._peerlen[fd] = UNAME_LENGTH;
			bcopy(*arg1, Network._peer[fd], Network._peerlen[fd]);
			Network._peer[fd][Network._peerlen[fd]] = '\0';
		}
		else {	
			Network._peer[fd][0] = '\0';
		}
		return(1);
				
	case	PEER_FREE  :
		if(fd < Network._npeers && Network._peer[fd] != NULL)
		{
			Network._peer[fd][0] = '\0';
			Network._peerlen[fd] = 0;
		}
		return(1);
	}
}

#define	NSECONDS	10

int	_errflag = 0;
OnAlarm()
{
	_errflag = 1;
}

int
CallTheNameServer(service, nettype, arg1, arg2, arg3)
char	*nettype, **arg1, **arg2;
int	service, *arg3;
{
	int	m, n, ret;
	char   *ptr, *p;
	char	buf[128];

PRMSG("In CallTheNameServer, \n", 0, 0);
	
	if(NameServer < 0)
		return(-1);
	if(_errflag)
	{
		int	flags;

		if((flags = fcntl(NameServer, F_GETFL)) == -1)
			flags = O_RDWR;
		fcntl (NameServer, F_SETFL, O_NDELAY);
		while(read (NameServer, buf, 128) > 0);
		if(errno != EAGAIN && errno != EINTR)
		{
			fprintf(stderr, "errno %d while reading the nameserver\n",
					errno);
			close(NameServer);
			NameServer = -1;
			return(-1);
		}
		fcntl (NameServer, F_SETFL, flags);
		_errflag = 0;	
	}
	
	ret = n = (* (int *) (*arg1)) + 2*sizeof(int);

        ptr = buf;
	m = HEADERSIZE + strlen(nettype)+ 1;
	m = (((m +3) >>2)<<2);

	*(int *) ptr = n+m;
	ptr += sizeof(int);
	*(int *) ptr = m;
	ptr += sizeof(int);
	*(int *) ptr = service;
	ptr += sizeof(int);
	*(int *) ptr = atoi(dispno);
	ptr += sizeof(int);
	*(int *) ptr = strlen(nettype);
	ptr += sizeof(int);
	sprintf(ptr, nettype);

  	if (! (p = Xmalloc(m + n))) {
	    close(NameServer);
	    NameServer = -1;
	    return (-1);
	}
	    
	memcpy(p, buf, m);
	memcpy(p+m, *arg1, n);
	
	signal(SIGALRM, OnAlarm);
	alarm(NSECONDS); 

	if(write(NameServer, p, m+n) != m+n){
		fprintf(stderr, "write error\n");
		ret = -1;
		close(NameServer);
		NameServer = -1;
		}
	else if(Read(NameServer, buf, 2*sizeof(int)) == 0)
	{
		ret = -1;
#ifdef DEBUG
		fprintf(stderr, "Server fails to read header from nameserver\n");
#endif
	}
	else {
		ptr = buf;
		ret = *(int *) buf;
		ptr += sizeof(int);
		if(*(int *) ptr <= 0)
		{
#ifdef DEBUG
			fprintf(stderr, "No Of entries returned <= 0\n");
#endif
			ret = -1;
			goto theend;
		}
		if(arg3 != NULL){
			*arg3 = *(int *) ptr;
#ifdef DEBUG
			fprintf(stderr, "No Of Entries returned {%d}\n", *arg3);
#endif
			}
		ptr = *arg2 = (char *) Xmalloc(ret );
#ifdef DEBUG
		if(ptr == NULL)
			fprintf(stderr, "MALLOC returns NULL\n");
#endif
		if(ptr == NULL)
			ret = -1;
		else {
			if(Read(NameServer, ptr, ret) == 0)
			{
				fprintf(stderr, 
					"Server fails to read %d chars\n", ret);
				Xfree(*arg2);
				ret = -1;
			}
		}
	}
theend:
	alarm(0); 
	Xfree(p);
	return(ret);
}

static	int	_hlen = 0;
static	char	*_hptr = NULL;

char	**
addheader(string, len)
char	*string;
int	len;
{

	int	n, m, p;
	char	*ptr;

	n = len;
	m = n + sizeof(xHostEntry);
	p = m + 2 * sizeof(int);
	
	if(p > _hlen){
		if(_hptr == NULL)
			_hptr = Xmalloc(p);
		else	_hptr = Xrealloc(_hptr, p);
		}
	if(_hptr == NULL){
		fprintf(stderr, "addheader(): Xmalloc failed\n");
		exit(1);
		}
	else if(p > _hlen)
		_hlen = p;
		
	ptr = _hptr;
	
	*(int *) ptr = m;
	ptr += sizeof(int);
	*(int *) ptr = 1;
	ptr += sizeof(int);

	((xHostEntry *) ptr)-> length = n;
	ptr += sizeof(xHostEntry);
	memcpy(ptr, string, n);
	
	return(&_hptr);
}

char	**
addtliheader(call)
struct t_call *call;
{

	
	char	*ptr;
	int	a, o, u;
	int	ra, ro, ru, rentlen;


	a = call->addr.len;
	o = call->opt.len;
	u = call->udata.len;

	ra = ((a + sizeof(xHostEntry) +3) >>2) << 2;
	ro = ((o + sizeof(xHostEntry) +3) >>2) << 2;
	ru = ((u + sizeof(xHostEntry) +3) >>2) << 2;

	rentlen = ra + ro + ru + 2 * sizeof(int);

	if(rentlen > _hlen){
		if(_hptr == NULL)
                        _hptr = Xmalloc(rentlen);
		else	_hptr = Xrealloc(_hptr, rentlen);
		}
	if(_hptr == NULL){
		fprintf(stderr, "addheader(): Xmalloc failed\n");
		exit(1);
		}
	else if(rentlen > _hlen)
		_hlen = rentlen;

        ptr = _hptr;
	
        *(int *) ptr = rentlen - 2 * sizeof(int);
	ptr += sizeof(int);
	*(int *) ptr = 1;
	ptr += sizeof(int);

	((xHostEntry *) ptr)-> length = a;
	if(a > 0){
		memcpy(ptr + sizeof(xHostEntry), call->addr.buf, a);
		}

	
	ptr += ra;
	((xHostEntry *) ptr)-> length = o;
	if(o > 0)
		memcpy(ptr + sizeof(xHostEntry), call->opt.buf, o);
	
	ptr += ro;
	((xHostEntry *) ptr)-> length = u;
	if(u > 0){
		memcpy(ptr + sizeof(xHostEntry), call->udata.buf, u);
                }

	return(&_hptr);
}

Read(fd, buf, count)
int	fd, count;
char	*buf;
{
	int	n;
	while((n = read(fd, buf, count)) > 0)
	{
		if(n == count)
		{
			return(1);
		}
		buf += n;
		count -= n;
	}
	return(0);
}

int _XBytesReadable (fd, ptr)
    int fd;
    int *ptr;
{
    int inbuf = _XsInputBuffersize[fd] - _XsInputBufferptr[fd];
    int n;
    int flg;

    /*
     * XXX - ought to have code for other types of connections in here too
     * for systems that want to support streams as well as sockets.
     */

    if (inbuf >= sizeof(xReply)) {
	*ptr = inbuf;
	return (0);
    }

    if (_XsInputBufferptr[fd] > 0) {
	/* move tidbit to front of buffer */
	bcopy (&_XsInputBuffer[fd][_XsInputBufferptr[fd]],
	       &_XsInputBuffer[fd][0], inbuf);

	/* Adjust pointers in buffer to reflect move */
	_XsInputBuffersize[fd] = inbuf;
	_XsInputBufferptr[fd] = 0;
    }

    if (inbuf < 0) {
	inbuf = 0;
	_XsInputBuffersize[fd] = 0;
    }

    /* Read no more than number of bytes left in buffer */
    errno = 0;

    if (_XsTypeOfStream[fd] == X_LOCAL_STREAM)
      n = read(fd, &_XsInputBuffer[fd][inbuf], BUFFERSIZE-inbuf);
    else
      n = t_rcv(fd, &_XsInputBuffer[fd][inbuf], BUFFERSIZE-inbuf, &flg);

    if (n > 0) {
	_XsInputBuffersize[fd] += n;
	*ptr = _XsInputBuffersize[fd];
	return (0);
    } else {
#ifdef EWOULDBLOCK
	if (errno == EWOULDBLOCK) {
	    *ptr = _XsInputBuffersize[fd];
	    return (0);
	} else {
#endif
	    if (n == 0) {
		errno = EPIPE;
		return (-1);
	    } else {
		if (errno != EINTR)
		  return (-1);
		else {
		    *ptr = _XsInputBuffersize[fd];
		    return (0);
		}
	    }
#ifdef EWOULDBLOCK
	}
#endif
    }
}
#else /* not STREAMSCONN */
static int dummy;	/* prevent ranlibs from complaining */
#endif /* STREAMSCONN */
