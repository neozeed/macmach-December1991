/*
 * supmsg.h - global definitions/variables used in msg routines.
 *
 **********************************************************************
 * HISTORY
 * $Log:	supmsg.h,v $
 * Revision 1.3  90/01/10  17:17:23  bww
 * 	Changed MSGF to MSG constants.
 * 	From "[89/08/23            gm0w]" at CMU.
 * 	[90/01/10            bww]
 * 
 * 27-Dec-87  Glenn Marcy (gm0w) at Carnegie-Mellon University
 *	Added crosspatch support.  Removed nameserver support.
 *
 * 29-Jun-87  Glenn Marcy (gm0w) at Carnegie-Mellon University
 *	Added "release" support.
 *
 * 27-May-87  Doug Philips (dwp) at Carnegie-Mellon University
 *	Added MSGFDONE and subvalues, added doneack and donereason.
 *
 * 20-May-87  Glenn Marcy (gm0w) at Carnegie-Mellon University
 *	Added changes to make lint happy.
 *
 * 04-Jan-86  Glenn Marcy (gm0w) at Carnegie-Mellon University
 *	Changed hostname to hostT to support multiple repositories per
 *	collection.  Added FSETUPBUSY to tell clients that server is
 *	currently busy.
 *
 * 19-Dec-85  Glenn Marcy (gm0w) at Carnegie-Mellon University
 *	Created.
 *
 **********************************************************************
 */

/* Special messages reserved for SCM */
#define MSGGOAWAY	(-1)		/* see scm.c */

/* Message types -- see supmsg.c */
#define MSGSIGNON	(101)
#define MSGSIGNONACK	(102)
#define MSGSETUP	(103)
#define MSGSETUPACK	(104)
#define MSGLOGIN	(105)
#define MSGLOGACK	(106)
#define MSGCRYPT	(107)
#define MSGCRYPTOK	(108)
#define MSGREFUSE	(109)
#define MSGLIST		(110)
#define MSGNEED		(111)
#define MSGDENY		(112)
#define MSGSEND		(113)
#define MSGRECV		(114)
#define MSGDONE		(115)
#define MSGXPATCH	(117)

/* MSGSETUPACK data codes - setupack */
#define FSETUPOK	(999)
#define FSETUPHOST	(998)
#define FSETUPSAME	(997)
#define FSETUPOLD	(996)
#define FSETUPBUSY	(995)
#define FSETUPRELEASE	(994)

/* MSGLOGACK data codes - loginack */
#define FLOGOK		(989)
#define FLOGNG		(988)

/* MSGDONE data codes - doneack */
#define FDONESUCCESS	(979)
#define FDONEDONTLOG	(978)
#define FDONESRVERROR	(977)
#define FDONEUSRERROR	(976)
#define FDONEGOAWAY	(975)

#ifdef	MSGSUBR

/* used in all msg routines */
extern int	server;			/* true if we are the server */
extern int	protver;		/* protocol version of partner */

#else	MSGSUBR

#ifdef	MSGFILE
#define	EXTERN
#else	MSGFILE
#define	EXTERN	extern
#endif	MSGFILE

/* used in all msg routines */
EXTERN	int	server;			/* true if we are the server */

/* msggoaway */
EXTERN	char	*goawayreason;		/* reason for goaway */

/* msgsignon */
EXTERN	int	pgmversion;		/* version of this program */
EXTERN	int	protver;		/* protocol version of partner */
EXTERN	int	pgmver;			/* program version of partner */
EXTERN	char	*scmver;		/* scm version of partner */
EXTERN	int	fspid;			/* process id of fileserver */

/* msgsetup */
EXTERN	int	xpatch;			/* setup crosspatch to a new client */
EXTERN	char	*xuser;			/* user for crosspatch */
EXTERN	char	*collname;		/* collection name */
EXTERN	char	*basedir;		/* base directory */
EXTERN	int	basedev;		/* base directory device */
EXTERN	int	baseino;		/* base directory inode */
EXTERN	long	lasttime;		/* time of last upgrade */
EXTERN	int	listonly;		/* only listing files, no data xfer */
EXTERN	int	newonly;		/* only send new files */
EXTERN	char	*release;		/* release name */
EXTERN	int	setupack;		/* ack return value for setup */

/* msgcrypt */
EXTERN	char	*crypttest;		/* encryption test string */

/* msglogin */
EXTERN	char	*logcrypt;		/* login encryption test */
EXTERN	char	*loguser;		/* login username */
EXTERN	char	*logpswd;		/* password for login */
EXTERN	int	logack;			/* login ack status */
EXTERN	char	*logerror;		/* error string from oklogin */

/* msgxpatch */
EXTERN	int	xargc;			/* arg count for crosspatch */
EXTERN	char	**xargv;		/* arg array for crosspatch */

/* msgrefuse */
EXTERN	TREE	*refuseT;		/* tree of files to refuse */

/* msglist */
EXTERN	TREE	*listT;			/* tree of files to list */
EXTERN	long	scantime;		/* time that collection was scanned */

/* msgneed */
EXTERN	TREE	*needT;			/* tree of files to need */

/* msgdeny */
EXTERN	TREE	*denyT;			/* tree of files to deny */

/* msgrecv */
/* msgsend */
EXTERN	TREE	*upgradeT;		/* pointer to file being upgraded */

/* msgdone */
EXTERN	int	doneack;		/* done ack status */
EXTERN	char 	*donereason;		/* set if indicated by doneack */

#undef	EXTERN

#endif	MSGSUBR
