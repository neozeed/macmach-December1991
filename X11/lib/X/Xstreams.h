/*
 * $XConsortium: Xstreams.h,v 1.3 89/06/15 15:59:09 jim Exp $
 */

#ifndef _XSTREAMS_H_
#define _XSTREAMS_H_

#ifndef	NOIDENT
#ident	"@(#)Xstreams.h	1.11"
#endif

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
 Xstreams.h (C header file)
	Acc: 575304412 Tue Apr 26 09:46:52 1988
	Mod: 574017273 Tue Apr 26 12:14:33 1988
	Sta: 574017273 Tue Apr 26 12:14:33 1988
	Owner: 2011
	Group: 1985
	Permissions: 644
*/
/*
	START USER STAMP AREA
*/
/*
	END USER STAMP AREA
*/


#define	MAX_NETS	8

typedef struct _host {
	char	host_name[32];
	int	host_len;
	struct _host *next;
} HOST;

/*
 * Structure for handling multiple connection requests on the same stream.
 */

struct listenCall {
	struct t_call *CurrentCall;
	struct listenCall *NextCall;
};

struct listenQue {
	struct listenCall *QueHead;
	struct listenCall *QueTail;
};

#define EMPTY(p)	(p->QueHead == (struct listenCall *) NULL)


typedef struct {
	int	flags;
	char	type;
	int	display;
	char	*inputbuf;
	int	buflen;
	int	bufptr;
	int	msglen;
	} IOBUFFER;

typedef struct {
	int	_nnets;
	char	*_net[MAX_NETS];
        struct listenQue FreeList[MAX_NETS];
	struct listenQue PendingQue[MAX_NETS];
	int	_npeers;
	char	**_peer;
	int	*_peerlen;
	HOST	*_validhosts;
	} networkInfo;

typedef struct _Xstream {
	int	(*SetupTheListener)();
	int	(*ConnectNewClient)();
	int	(*CallTheListener)();
	int	(*ReadFromStream)();
	int	(*BytesCanBeRead)();
	int	(*WriteToStream)();
	int	(*CloseStream)();
	int	(*CreateAddress)();
	union ext {
		int	(*NameServer)();
		networkInfo *NetInfo;
		} u;	
	} Xstream;

extern Xstream _XsStream[];


#define NO_BUFFERING	0
#define BUFFERING	1

/* Network services */

#define OpenDaemonConnection	0
#define	PEER_NAME		1
#define	PEER_ALLOC		2
#define	PEER_FREE		3
#define	ConvertNetAddrToName	4
#define	ConvertNameToNetAddr	5
#define	ConvertNameToTliCall	6
#define	ConvertTliCallToName	7
#define	ConvertNameToTliBind	8

#define	UNAME_LENGTH	14

#define X_LOCAL_STREAM	0
#define X_TLI_STREAM	1
#define CLOSED_STREAM	-1

/*
	The following are defined in X.h. Any changes to FamilyUname
	should take X.h into consideration.
*/
		/* protocol families */

		/*

		#define FamilyInternet		0
		#define FamilyDECnet		1
		#define FamilyChaos		2

		*/

#define FamilyUname	3

#define X_TCP_PORT		6000

#define LOCAL_LISTENER "/dev/X/server"
#define	NAME_SERVER_NODE "/dev/X/nameserver"
#define XNETSPECDIR	"/usr/X/lib/net"
#define XROOTDIR	"/usr/X"

#define	MAX_SIMUL_TLI_CALLS	20

#define SetupNetworkInfo()	_XsStream[X_LOCAL_STREAM].u.NetInfo = &Network; \
	_XsStream[X_TLI_STREAM].u.NameServer = nameserver

#define NetworkInfo (_XsStream[X_LOCAL_STREAM].u.NetInfo)
#define GetNetworkInfo (*_XsStream[X_TLI_STREAM].u.NameServer)
#define validhosts ((HOST *) _XsStream[X_LOCAL_STREAM].u.NetInfo->_validhosts)

/*
 *	header of messages sent by X to the nameserver 
 *      1st int: the size of the entire message.
 *	2nd int: the size of the header itself.
 *  	3rd int: the service number.
 *      4th int: the display number.
 * 	5th int: the length of the network name.
 */
 
#define HEADERSIZE	(5*sizeof(int))

#endif /* _XSTREAMS_H_ */
