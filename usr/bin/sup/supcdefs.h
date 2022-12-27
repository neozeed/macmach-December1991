/*
 *	supcdefs.h -- Declarations shared by the collection of files
 *			that build the sup client.
 *
 **********************************************************************
 * HISTORY
 * $Log:	supcdefs.h,v $
 * Revision 1.3  89/11/29  17:40:46  bww
 * 	Changed CMUCS conditional to CMU.
 * 	[89/11/29  17:32:35  bww]
 * 
 * 10-Feb-88  Glenn Marcy (gm0w) at Carnegie-Mellon University
 *	Added timeout for backoff.
 *
 * 28-Jun-87  Glenn Marcy (gm0w) at Carnegie-Mellon University
 *	Added Crelease for "release" support.
 *
 * 25-May-87  Doug Philips (dwp) at Carnegie-Mellon University
 *	Created.
 *
 **********************************************************************
 */

#include <libc.h>
#include <netdb.h>
#include <signal.h>
#include <setjmp.h>
#include <pwd.h>
#include <grp.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/time.h>
#include <sys/file.h>
#include <sys/errno.h>
#if	CMU			/* used by resource pausing code only */
#include <sys/ioctl.h>
#include <sys/resource.h>
#endif
#include <c.h>
#include "sup.h"
#include "supmsg.h"

extern int errno;
extern uid_t getuid();
extern gid_t getgid();
extern long time();

extern int PGMVERSION;

/*******************************************
 ***    D A T A   S T R U C T U R E S    ***
 *******************************************/

struct collstruct {			/* one per collection to be upgraded */
	char *Cname;			/* collection name */
	TREE *Chost;			/* attempted host for collection */
	TREE *Chtree;			/* possible hosts for collection */
	char *Cbase;			/* local base directory */
	char *Chbase;			/* remote base directory */
	char *Cprefix;			/* local collection pathname prefix */
	char *Crelease;			/* release name */
	char *Cnotify;			/* user to notify of status */
	char *Clogin;			/* remote login name */
	char *Cpswd;			/* remote password */
	char *Ccrypt;			/* data encryption key */
	int Ctimeout;			/* timeout for backoff */
	int Cflags;			/* collection flags */
	int Cnogood;			/* upgrade no good, "when" unchanged */
	int Clockfd;			/* >= 0 if collection is locked */
	struct collstruct *Cnext;	/* next collection */
};
typedef struct collstruct COLLECTION;

#define CFALL		00001
#define CFBACKUP	00002
#define CFDELETE	00004
#define CFEXECUTE	00010
#define CFLIST		00020
#define CFLOCAL		00040
#define CFMAIL		00100
#define CFOLD		00200
#define CFVERBOSE	00400

/*************************
 ***	M A C R O S    ***
 *************************/

#define vnotify	if (thisC->Cflags&CFVERBOSE)  notify
