/*
 * $XConsortium: XConnDis.c,v 11.65 89/12/09 19:10:20 rws Exp $
 *
 * Copyright 1989 Massachusetts Institute of Technology
 *
 * Permission to use, copy, modify, and distribute this software and its
 * documentation for any purpose and without fee is hereby granted, provided
 * that the above copyright notice appear in all copies and that both that
 * copyright notice and this permission notice appear in supporting
 * documentation, and that the name of M.I.T. not be used in advertising
 * or publicity pertaining to distribution of the software without specific,
 * written prior permission.  M.I.T. makes no representations about the
 * suitability of this software for any purpose.  It is provided "as is"
 * without express or implied warranty.
 *
 * M.I.T. DISCLAIMS ALL WARRANTIES WITH REGARD TO THIS SOFTWARE, INCLUDING ALL
 * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS, IN NO EVENT SHALL M.I.T.
 * BE LIABLE FOR ANY SPECIAL, INDIRECT OR CONSEQUENTIAL DAMAGES OR ANY DAMAGES
 * WHATSOEVER RESULTING FROM LOSS OF USE, DATA OR PROFITS, WHETHER IN AN ACTION
 * OF CONTRACT, NEGLIGENCE OR OTHER TORTIOUS ACTION, ARISING OUT OF OR IN 
 * CONNECTION WITH THE USE OR PERFORMANCE OF THIS SOFTWARE.
 *
 * 
 * This file contains operating system dependencies.
 */

#include "copyright.h"
#define NEED_EVENTS

#include <stdio.h>
#include <X11/Xos.h>
#include "Xlibint.h"
#include <X11/Xauth.h>
#include <ctype.h>
#ifdef DNETCONN
#include <netdnet/dn.h>
#include <netdnet/dnetdb.h>
#endif

#ifndef X_CONNECTION_RETRIES		/* number retries on ECONNREFUSED */
#define X_CONNECTION_RETRIES 5
#endif

#ifdef DNETCONN
static int MakeDECnetConnection();
#endif
#ifdef UNIXCONN
static int MakeUNIXSocketConnection();
#endif
#ifdef TCPCONN
static int MakeTCPConnection();
#endif
#ifdef STREAMSCONN
extern int _XMakeStreamsConnection();
#endif


static char *copystring (src, len)
    char *src;
    int len;
{
    char *dst = Xmalloc (len + 1);

    if (dst) {
	strncpy (dst, src, len);
	dst[len] = '\0';
    }

    return dst;
}


/* 
 * Attempts to connect to server, given display name. Returns file descriptor
 * (network socket) or -1 if connection fails.  Display names may be of the
 * following format:
 *
 *     [hostname] : [:] displaynumber [.screennumber]
 *
 * The second colon indicates a DECnet style name.  No hostname is interpretted
 * as the most efficient local connection to a server on the same machine.  
 * This is usually:
 *
 *     o  shared memory
 *     o  local stream
 *     o  UNIX domain socket
 *     o  TCP to local host
 */
int _XConnectDisplay (display_name, fullnamep, dpynump, screenp,
		      familyp, saddrlenp, saddrp)
    char *display_name;
    char **fullnamep;			/* RETURN */
    int *dpynump;			/* RETURN */
    int *screenp;			/* RETURN */
    int *familyp;			/* RETURN */
    int *saddrlenp;			/* RETURN */
    char **saddrp;			/* RETURN, freed by caller */
{
    char *lastp, *p;			/* char pointers */
    char *phostname = NULL;		/* start of host of display */
    char *pdpynum = NULL;		/* start of dpynum of display */
    char *pscrnum = NULL;		/* start of screen of display */
    Bool dnet = False;			/* if true, then DECnet format */
    int idisplay;			/* required display number */
    int iscreen = 0;			/* optional screen number */
    int (*connfunc)();			/* method to create connection */
    int fd = -1;			/* file descriptor to return */
    int len;				/* length tmp variable */

    p = display_name;

    *saddrlenp = 0;			/* set so that we can clear later */
    *saddrp = NULL;

    /*
     * Step 1, find the hostname.  This is delimited by the required 
     * first colon.
     */
    for (lastp = p; *p && *p != ':'; p++) ;
    if (!*p) return -1;		/* must have a colon */

    if (p != lastp) {		/* no hostname given */
	phostname = copystring (lastp, p - lastp);
	if (!phostname) goto bad;	/* no memory */
    }


    /*
     * Step 2, see if this is a DECnet address by looking for the optional
     * second colon.
     */
    if (p[1] == ':') {			/* then DECnet format */
	dnet = True;
	p++;
    }

    /*
     * see if we're allowed to have a DECnet address
     */
#ifndef DNETCONN
    if (dnet) goto bad;
#endif

    
    /*
     * Step 3, find the display number.  This field is required and is 
     * delimited either by a nul or a period, depending on whether or not
     * a screen number is present.
     */

    for (lastp = ++p; *p && isascii(*p) && isdigit(*p); p++) ;
    if ((p == lastp) ||			/* required field */
	(*p != '\0' && *p != '.') ||	/* invalid non-digit terminator */
	!(pdpynum = copystring (lastp, p - lastp)))  /* no memory */
      goto bad;
    idisplay = atoi (pdpynum);


    /*
     * Step 4, find the screen number.  This field is optional.  It is 
     * present only if the display number was followed by a period (which
     * we've already verified is the only non-nul character).
     */

    if (*p) {
	for (lastp = ++p; *p && isascii(*p) && isdigit (*p); p++) ;
	if (*p ||			/* non-digits */
	    !(pscrnum = copystring (lastp, p - lastp)))	 /* no memory */
	  goto bad;
	iscreen = atoi (lastp);
    }



    /*
     * At this point, we know the following information:
     *
     *     phostname                hostname string or NULL
     *     idisplay                 display number
     *     iscreen                  screen number
     *     dnet                     DECnet boolean
     * 
     * We can now decide which transport to use based on the ConnectionFlags
     * build parameter the hostname string.  If phostname is NULL or equals
     * the string "local", then choose the best transport.  If phostname
     * is "unix", then choose BSD UNIX domain sockets (if configured).
     *
     * First, choose default transports:  DECnet else (TCP or STREAMS)
     */


#ifdef DNETCONN
    if (dnet)
      connfunc = MakeDECnetConnection;
    else
#endif
#ifdef TCPCONN
      connfunc = MakeTCPConnection;
#else
#ifdef STREAMSCONN
      connfunc = _XMakeStreamsConnection;
#else
      connfunc = NULL;
#endif
#endif

#ifdef UNIXCONN
    /*
     * Now that the defaults have been established, see if we have any 
     * special names that we have to override:
     *
     *     :N         =>     if UNIXCONN then unix-domain-socket
     *     ::N        =>     if UNIXCONN then unix-domain-socket
     *     unix:N     =>     if UNIXCONN then unix-domain-socket
     *
     * Note that if UNIXCONN isn't defined, then we can use the default
     * transport connection function set above.
     */
    if (!phostname) {
	connfunc = MakeUNIXSocketConnection;
    }
    else if (strcmp (phostname, "unix") == 0) {
	connfunc = MakeUNIXSocketConnection;
    }
#endif
    if (!connfunc)
	goto bad;


#ifdef UNIXCONN
#define LOCALCONNECTION (!phostname || connfunc == MakeUNIXSocketConnection)
#else
#define LOCALCONNECTION (!phostname)
#endif

    if (LOCALCONNECTION) {
	/*
	 * Get the auth info for local hosts so that it doesn't have to be
	 * repeated everywhere; the particular values in these fields are
	 * not part of the protocol.
	 */
	char hostnamebuf[256];
	int len = _XGetHostname (hostnamebuf, sizeof hostnamebuf);

	*familyp = FamilyLocal;
	if (len > 0) {
	    *saddrp = Xmalloc (len + 1);
	    if (*saddrp) {
		strcpy (*saddrp, hostnamebuf);
		*saddrlenp = len;
	    } else {
		*saddrlenp = 0;
	    }
	}
    }
#undef LOCALCONNECTION


    /*
     * Make the connection, also need to get the auth address info for
     * non-local connections.  Do retries in case server host has hit its
     * backlog (which, unfortunately, isn't distinguishable from there not
     * being a server listening at all, which is why we have to not retry
     * too many times).
     */
    if ((fd = (*connfunc) (phostname, idisplay, X_CONNECTION_RETRIES,
			   familyp, saddrlenp, saddrp)) < 0)
      goto bad;


    /*
     * Set the connection non-blocking since we use select() to block; also
     * set close-on-exec so that programs that fork() doesn't get confused.
     */
#ifdef FIOSNBIO
    {
	int arg = 1;
	ioctl (fd, FIOSNBIO, &arg);
    }
#else
    (void) fcntl (fd, F_SETFL, FNDELAY);
#endif /* FIOSNBIO */

    (void) fcntl (fd, F_SETFD, 1);


    /*
     * Build the expanded display name:
     *
     *     [host] : [:] dpy . scr \0
     */
    len = ((phostname ? strlen(phostname) : 0) + 1 + (dnet ? 1 : 0) +
	   strlen(pdpynum) + 1 + (pscrnum ? strlen(pscrnum) : 1) + 1);
    *fullnamep = (char *) Xmalloc (len);
    if (!*fullnamep) goto bad;

    sprintf (*fullnamep, "%s%s%d.%d",
	     (phostname ? phostname : ""), (dnet ? "::" : ":"),
	     idisplay, iscreen);

    *dpynump = idisplay;
    *screenp = iscreen;
    if (phostname) Xfree (phostname);
    if (pdpynum) Xfree (pdpynum);
    if (pscrnum) Xfree (pscrnum);
    return fd;


    /*
     * error return; make sure everything is cleaned up.
     */
  bad:
    if (fd >= 0) (void) close (fd);
    if (*saddrp) {
	Xfree (*saddrp);
	*saddrp = NULL;
    }
    *saddrlenp = 0;
    if (phostname) Xfree (phostname);
    if (pdpynum) Xfree (pdpynum);
    if (pscrnum) Xfree (pscrnum);
    return -1;

}


/*****************************************************************************
 *                                                                           *
 *			   Make Connection Routines                          *
 *                                                                           *
 *****************************************************************************/

#ifdef DNETCONN				/* stupid makedepend */
#define NEED_BSDISH
#endif
#ifdef UNIXCONN
#define NEED_BSDISH
#endif
#ifdef TCPCONN
#define NEED_BSDISH
#endif

#ifdef NEED_BSDISH			/* makedepend can't handle #if */
/*
 * 4.2bsd-based systems
 */
#include <sys/socket.h>

#ifndef hpux
#ifdef apollo			/* nest if(n)defs because makedepend is broken */
#ifndef NO_TCP_H
#include <netinet/tcp.h>
#endif /* NO_TCP_H */
#else  /* apollo */
#include <netinet/tcp.h>
#endif /* apollo */
#endif
void bcopy();
#endif /* NEED_BSDISH */


#ifdef DNETCONN
static int MakeDECnetConnection (phostname, idisplay, retries,
				 familyp, saddrlenp, saddrp)
    char *phostname;
    int idisplay;
    int retries;
    int *familyp;			/* RETURN */
    int *saddrlenp;			/* RETURN */
    char **saddrp;			/* RETURN */
{
    int fd;
    char objname[20];
    extern int dnet_conn();
    struct dn_naddr *dnaddrp, dnaddr;
    struct nodeent *np;

    if (!phostname) phostname = "0";

    /*
     * build the target object name.
     */
    sprintf (objname, "X$X%d", idisplay);

    /*
     * Attempt to open the DECnet connection, return -1 if fails; ought to
     * do some retries here....
     */
    if ((fd = dnet_conn (phostname, objname, SOCK_STREAM, 0, 0, 0, 0)) < 0) {
	return -1;
    }

    *familyp = FamilyDECnet;
    if (dnaddrp = dnet_addr (phostname)) {  /* stolen from xhost */
	dnaddr = *dnaddrp;
    } else {
	if ((np = getnodebyname (phostname)) == NULL) {
	    (void) close (fd);
	    return -1;
	}
	dnaddr.a_len = np->n_length;
	bcopy (np->n_addr, dnaddr.a_addr, np->n_length);
    }

    *saddrlenp = sizeof (struct dn_naddr);
    *saddrp = Xmalloc (*saddrlenp);
    if (!*saddrp) {
	(void) close (fd);
	return -1;
    }
    bcopy ((char *)&dnaddr, *saddrp, *saddrlenp);
    return fd;
}
#endif /* DNETCONN */


#ifdef UNIXCONN
#include <sys/un.h>

#ifndef X_UNIX_PATH
#ifdef hpux
#define X_UNIX_PATH "/usr/spool/sockets/X11/"
#define OLD_UNIX_PATH "/tmp/.X11-unix/X"
#else
#define X_UNIX_PATH "/tmp/.X11-unix/X"
#endif
#endif /* X_UNIX_PATH */

static int MakeUNIXSocketConnection (phostname, idisplay, retries,
				     familyp, saddrlenp, saddrp)
    char *phostname;
    int idisplay;
    int retries;
    int *familyp;			/* RETURN */
    int *saddrlenp;			/* RETURN */
    char **saddrp;			/* RETURN */
{
    struct sockaddr_un unaddr;		/* UNIX socket data block */
    struct sockaddr *addr;		/* generic socket pointer */
    int addrlen;			/* length of addr */
    int fd;				/* socket file descriptor */
#ifdef hpux /* this is disgusting */
    struct sockaddr_un ounaddr;		/* UNIX socket data block */
    struct sockaddr *oaddr;		/* generic socket pointer */
    int oaddrlen;			/* length of addr */
#endif

    unaddr.sun_family = AF_UNIX;
    sprintf (unaddr.sun_path, "%s%d", X_UNIX_PATH, idisplay);

    addr = (struct sockaddr *) &unaddr;
    addrlen = strlen(unaddr.sun_path) + sizeof(unaddr.sun_family);

#ifdef hpux /* this is disgusting */
    ounaddr.sun_family = AF_UNIX;
    sprintf (ounaddr.sun_path, "%s%d", OLD_UNIX_PATH, idisplay);
    oaddr = (struct sockaddr *) &ounaddr;
    oaddrlen = strlen(ounaddr.sun_path) + sizeof(ounaddr.sun_family);
#endif

    /*
     * Open the network connection.
     */
    do {
	if ((fd = socket ((int) addr->sa_family, SOCK_STREAM, 0)) < 0) {
	    return -1;
	}

	if (connect (fd, addr, addrlen) < 0) {
	    int olderrno = errno;
	    (void) close (fd);
#ifdef hpux /* this is disgusting */
	    if (olderrno == ENOENT) {
		fd = socket ((int) oaddr->sa_family, SOCK_STREAM, 0);
		if (fd >= 0) {
		    if (connect (fd, oaddr, oaddrlen) >= 0)
			break;
		    olderrno = errno;
		    (void) close (fd);
		}
	    }
#endif
	    if (olderrno != ENOENT || retries <= 0) {
		errno = olderrno;
		return -1;
	    }
	    sleep (1);
	} else {
	    break;
	}
    } while (retries-- > 0);

    /*
     * Don't need to get auth info since we're local
     */
    return fd;
}
#endif /* UNIXCONN */


#ifdef TCPCONN
static int MakeTCPConnection (phostname, idisplay, retries,
			      familyp, saddrlenp, saddrp)
    char *phostname;
    int idisplay;
    int retries;
    int *familyp;			/* RETURN */
    int *saddrlenp;			/* RETURN */
    char **saddrp;			/* RETURN */
{
    char hostnamebuf[256];		/* tmp space */
    unsigned long hostinetaddr;		/* result of inet_addr of arpa addr */
    struct sockaddr_in inaddr;		/* IP socket */
    struct sockaddr *addr;		/* generic socket pointer */
    int addrlen;			/* length of addr */
    struct hostent *hp;			/* entry in hosts table */
    char *cp;				/* character pointer iterator */
    int fd;				/* file descriptor to return */
    int len;				/* length tmp variable */

#define INVALID_INETADDR ((unsigned long) -1)

    if (!phostname) {
	hostnamebuf[0] = '\0';
	(void) _XGetHostname (hostnamebuf, sizeof hostnamebuf);
	phostname = hostnamebuf;
    }

    /*
     * if numeric host name then try to parse it as such; do the number
     * first because some systems return garbage instead of INVALID_INETADDR
     */
    if (isascii(phostname[0]) && isdigit(phostname[0])) {
	hostinetaddr = inet_addr (phostname);
    } else {
	hostinetaddr = INVALID_INETADDR;
    }

    /*
     * try numeric
     */
    if (hostinetaddr == INVALID_INETADDR) {
	if ((hp = gethostbyname(phostname)) == NULL) {
	    /* No such host! */
	    return -1;
	}
	if (hp->h_addrtype != AF_INET) {  /* is IP host? */
	    /* Not an Internet host! */
	    return -1;
	}
 
	/* Set up the socket data. */
	inaddr.sin_family = hp->h_addrtype;
#if defined(CRAY) && defined(OLDTCP)
	/* Only Cray UNICOS3 and UNICOS4 will define this */
	{
	    long t;
	    bcopy ((char *)hp->h_addr, (char *)&t, sizeof(t));
	    inaddr.sin_addr = t;
	}
#else
	bcopy ((char *)hp->h_addr, (char *)&inaddr.sin_addr, 
	       sizeof(inaddr.sin_addr));
#endif /* CRAY and OLDTCP */
    } else {
#if defined(CRAY) && defined(OLDTCP)
	/* Only Cray UNICOS3 and UNICOS4 will define this */
	inaddr.sin_addr = hostinetaddr;
#else
	inaddr.sin_addr.s_addr = hostinetaddr;
#endif /* CRAY and OLDTCP */
	inaddr.sin_family = AF_INET;
    }

    addr = (struct sockaddr *) &inaddr;
    addrlen = sizeof (struct sockaddr_in);
    inaddr.sin_port = X_TCP_PORT + idisplay;
    inaddr.sin_port = htons (inaddr.sin_port);	/* may be funky macro */


    /*
     * Open the network connection.
     */
    do {
	if ((fd = socket ((int) addr->sa_family, SOCK_STREAM, 0)) < 0) {
	    return -1;
	}

	/*
	 * turn off TCP coalescence
	 */
#ifdef TCP_NODELAY
	{
	    int tmp = 1;
	    setsockopt (fd, IPPROTO_TCP, TCP_NODELAY, &tmp, sizeof (int));
	}
#endif

	/*
	 * connect to the socket; if there is no X server or if the backlog has
	 * been reached, then ECONNREFUSED will be returned.
	 */
	if (connect (fd, addr, addrlen) < 0) {
	    int olderrno = errno;
	    (void) close (fd);
	    if (olderrno != ECONNREFUSED || retries <= 0) {
		errno = olderrno;
		return -1;
	    }
	    sleep (1);
	} else {
	    break;
	}
    } while (retries-- > 0);


    /*
     * Success!  So, save the auth information
     */
#ifdef CRAY
#ifdef OLDTCP
    len = sizeof(inaddr.sin_addr);
#else
    len = SIZEOF_in_addr;
#endif /* OLDTCP */
    cp = (char *) &inaddr.sin_addr;
#else /* else not CRAY */
    len = sizeof(inaddr.sin_addr.s_addr);
    cp = (char *) &inaddr.sin_addr.s_addr;
#endif /* CRAY */

    /*
     * We are special casing the BSD hack localhost address
     * 127.0.0.1, since this address shouldn't be copied to
     * other machines.  So, we simply omit generating the auth info
     * since we set it to the local machine before calling this routine!
     */
    if (!((len == 4) && (cp[0] == 127) && (cp[1] == 0) &&
	  (cp[2] == 0) && (cp[3] == 1))) {
	*saddrp = Xmalloc (len);
	if (*saddrp) {
	    *saddrlenp = len;
	    bcopy (cp, *saddrp, len);
	    *familyp = FamilyInternet;
	} else {
	    *saddrlenp = 0;
	}
    }

    return fd;
}
#undef INVALID_INETADDR
#endif /* TCPCONN */



/*****************************************************************************
 *                                                                           *
 *			  Connection Utility Routines                        *
 *                                                                           *
 *****************************************************************************/

/* 
 * Disconnect from server.
 */

int _XDisconnectDisplay (server)

    int server;

{
    (void) close(server);
    return 0;
}



#undef NULL
#define NULL ((char *) 0)
/*
 * This is an OS dependent routine which:
 * 1) returns as soon as the connection can be written on....
 * 2) if the connection can be read, must enqueue events and handle errors,
 * until the connection is writable.
 */
_XWaitForWritable(dpy)
    Display *dpy;
{
    unsigned long r_mask[MSKCNT];
    unsigned long w_mask[MSKCNT];
    int nfound;

    CLEARBITS(r_mask);
    CLEARBITS(w_mask);

    while (1) {
	BITSET(r_mask, dpy->fd);
        BITSET(w_mask, dpy->fd);

	do {
	    nfound = select (dpy->fd + 1, r_mask, w_mask, NULL, NULL);
	    if (nfound < 0 && errno != EINTR)
		_XIOError(dpy);
	} while (nfound <= 0);

	if (ANYSET(r_mask)) {
	    char buf[BUFSIZE];
	    long pend_not_register;
	    register long pend;
	    register xEvent *ev;

	    /* find out how much data can be read */
	    if (BytesReadable(dpy->fd, (char *) &pend_not_register) < 0)
		_XIOError(dpy);
	    pend = pend_not_register;

	    /* must read at least one xEvent; if none is pending, then
	       we'll just block waiting for it */
	    if (pend < SIZEOF(xEvent)) pend = SIZEOF(xEvent);
		
	    /* but we won't read more than the max buffer size */
	    if (pend > BUFSIZE) pend = BUFSIZE;

	    /* round down to an integral number of XReps */
	    pend = (pend / SIZEOF(xEvent)) * SIZEOF(xEvent);

	    _XRead (dpy, buf, pend);

	    /* no space between comma and type or else macro will die */
	    STARTITERATE (ev,xEvent, buf, (pend > 0),
			  (pend -= SIZEOF(xEvent))) {
		if (ev->u.u.type == X_Error)
		    _XError (dpy, (xError *) ev);
		else		/* it's an event packet; enqueue it */
		    _XEnq (dpy, ev);
	    }
	    ENDITERATE
	}
	if (ANYSET(w_mask))
	    return;
    }
}


_XWaitForReadable(dpy)
  Display *dpy;
{
    unsigned long r_mask[MSKCNT];
    int result;
	
    CLEARBITS(r_mask);
    do {
	BITSET(r_mask, dpy->fd);
	result = select(dpy->fd + 1, r_mask, NULL, NULL, NULL);
	if (result == -1 && errno != EINTR) _XIOError(dpy);
    } while (result <= 0);
}


static int padlength[4] = {0, 3, 2, 1};	 /* make sure auth is multiple of 4 */

_XSendClientPrefix (dpy, client, auth_proto, auth_string)
     Display *dpy;
     xConnClientPrefix *client;		/* contains count for auth_* */
     char *auth_proto, *auth_string;	/* NOT null-terminated */
{
    int auth_length = client->nbytesAuthProto;
    int auth_strlen = client->nbytesAuthString;
    char padbuf[3];			/* for padding to 4x bytes */
    int pad;
    struct iovec iovarray[5], *iov = iovarray;
    int niov = 0;

#define add_to_iov(b,l) \
	  { iov->iov_base = (b); iov->iov_len = (l); iov++, niov++; }

    add_to_iov ((caddr_t) client, SIZEOF(xConnClientPrefix));

    /*
     * write authorization protocol name and data
     */
    if (auth_length > 0) {
	add_to_iov (auth_proto, auth_length);
	pad = padlength [auth_length & 3];
	if (pad) add_to_iov (padbuf, pad);
    }
    if (auth_strlen > 0) {
	add_to_iov (auth_string, auth_strlen);
	pad = padlength [auth_strlen & 3];
	if (pad) add_to_iov (padbuf, pad);
    }

#undef add_to_iov

    (void) WritevToServer (dpy->fd, iovarray, niov);
    return;
}
