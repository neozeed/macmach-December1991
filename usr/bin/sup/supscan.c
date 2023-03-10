/*
 * supscan -- SUP Scan File Builder
 *
 * Usage: supscan [ -v ] collection [ basedir ]
 *	  supscan [ -v ] -f dirfile
 *	  supscan [ -v ] -s
 *	-f	"file" -- use dirfile instead of system coll.dir
 *	-s	"system" -- perform scan for system supfile
 *	-v	"verbose" -- print messages as you go
 *	collection	-- name of the desired collection if not -s
 *	basedir		-- name of the base directory, if not
 *			   the default or recorded in coll.dir
 *	dirfile		-- name of replacement for system coll.dir.
 *
 **********************************************************************
 * HISTORY
 * $Log:	supscan.c,v $
 * Revision 1.5  90/04/10  15:14:51  bww
 * 	Changed localhost to retry getting the local host name 4
 * 	times with 30 second sleep intervals before aborting; after
 * 	4 tries, things are probably too messed up for the supscan
 * 	to do anything useful.
 * 	From "[90/04/04            dlc]" at CMU.
 * 	[90/04/10  15:14:22  bww]
 * 
 * Revision 1.4  89/11/29  17:42:00  bww
 * 	Updated variable argument list usage.
 * 	[89/11/29  17:37:09  bww]
 * 
 * Revision 1.3  89/08/15  15:31:49  bww
 * 	Updated to use v*printf() in place of _doprnt().
 * 	From "[89/04/19            mja]" at CMU.
 * 	Fixed up some notify messages of errors to use "SUP:" prefix.
 * 	From "[89/06/18            gm0w]" at CMU.
 * 	[89/08/15            bww]
 * 
 * 13-May-88  Glenn Marcy (gm0w) at Carnegie-Mellon University
 *	Changed goaway to longjmp back to top-level to scan next
 *	collection. [V7.6]
 *
 * 19-Feb-88  Glenn Marcy (gm0w) at Carnegie-Mellon University
 *	Added -f <filename> switch to scan all (or part) of the
 *	collections in a file of collection/base-directory pairs.
 *	[V7.5]
 *
 * 27-Dec-87  Glenn Marcy (gm0w) at Carnegie-Mellon University
 *	Removed nameserver support (which means to use a new
 *	datafile).
 *
 * 09-Sep-87  Glenn Marcy (gm0w) at Carnegie-Mellon University
 *	Use case-insensitive hostname comparison.
 *
 * 28-Jun-87  Glenn Marcy (gm0w) at Carnegie-Mellon University
 *	Added code for "release" support. [V6.4]
 *
 * 05-Jan-86  Glenn Marcy (gm0w) at Carnegie-Mellon University
 *	Changed collection setup errors to be non-fatal. [V5.3]
 *
 * 29-Dec-85  Glenn Marcy (gm0w) at Carnegie-Mellon University
 *	Moved most of the scanning code to scan.c. [V4.2]
 *
 * 02-Nov-85  Glenn Marcy (gm0w) at Carnegie-Mellon University
 *	Added "-s" option.
 *
 * 22-Sep-85  Glenn Marcy (gm0w) at Carnegie-Mellon University
 *	Merged 4.1 and 4.2 versions together.
 *
 * 04-Jun-85  Steven Shafer (sas) at Carnegie-Mellon University
 *	Created for 4.2 BSD.
 *
 **********************************************************************
 */

#include <libc.h>
#include <c.h>
#include <netdb.h>
#include <setjmp.h>
#if __STDC__
#include <stdarg.h>
#else
#include <varargs.h>
#endif
#include <sys/time.h>
#include <sys/types.h>
#include "sup.h"

#define PGMVERSION 6

/*******************************************
 ***    D A T A   S T R U C T U R E S    ***
 *******************************************/

struct collstruct {			/* one per collection to be upgraded */
	char *Cname;			/* collection name */
	char *Cbase;			/* local base directory */
	char *Cprefix;			/* local collection pathname prefix */
	struct collstruct *Cnext;	/* next collection */
};
typedef struct collstruct COLLECTION;

/*********************************************
 ***    G L O B A L   V A R I A B L E S    ***
 *********************************************/

int trace;				/* -v flag */

COLLECTION *firstC;			/* collection list pointer */
char *collname;				/* collection name */
char *basedir;				/* base directory name */
char *prefix;				/* collection pathname prefix */
long lasttime = 0;			/* time of last upgrade */
long scantime;				/* time of this scan */
int newonly = FALSE;			/* new files only */
jmp_buf sjbuf;				/* jump location for errors */

TREELIST *listTL;	/* list of all files specified by <coll>.list */
TREE *listT;		/* final list of files in collection */
TREE *refuseT = NULL;	/* list of all files specified by <coll>.list */

long time ();

/*************************************
 ***    M A I N   R O U T I N E    ***
 *************************************/

main (argc,argv)
int argc;
char **argv;
{
	register COLLECTION *c;

	init (argc,argv);		/* process arguments */
	for (c = firstC; c; c = c->Cnext) {
		collname = c->Cname;
		basedir = c->Cbase;
		prefix = c->Cprefix;
		(void) chdir (basedir);
		scantime = time ((long *)NULL);
		printf ("SUP Scan for %s starting at %s",collname,
			ctime (&scantime));
		(void) fflush (stdout);
		if (!setjmp (sjbuf)) {
			makescanlists (); /* record names in scan files */
			scantime = time ((long *)NULL);
			printf ("SUP Scan for %s completed at %s",collname,
				ctime (&scantime));
		} else
			printf ("SUP: Scan for %s aborted at %s",collname,
				ctime (&scantime));
		(void) fflush (stdout);
	}
	while (c = firstC) {
		firstC = firstC->Cnext;
		free (c->Cname);
		free (c->Cbase);
		if (c->Cprefix)  free (c->Cprefix);
		free ((char *)c);
	}
	exit (0);
}

/*****************************************
 ***    I N I T I A L I Z A T I O N    ***
 *****************************************/

usage ()
{
	fprintf (stderr,"Usage: supscan [ -v ] collection [ basedir ]\n");
	fprintf (stderr,"       supscan [ -v ] -f dirfile\n");
	fprintf (stderr,"       supscan [ -v ] -s\n");
	exit (1);
}

init (argc,argv)
int argc;
char **argv;
{
	char buf[STRINGLENGTH],fbuf[STRINGLENGTH],*p,*q;
	FILE *f;
	COLLECTION **c, *getcoll();
	int fflag,sflag;
	char *filename;

	trace = FALSE;
	fflag = FALSE;
	sflag = FALSE;
	while (argc > 1 && argv[1][0] == '-') {
		switch (argv[1][1]) {
		case 'f':
			fflag = TRUE;
			if (argc == 2)
				usage ();
			--argc;
			argv++;
			filename = argv[1];
			break;
		case 'v':
			trace = TRUE;
			break;
		case 's':
			sflag = TRUE;
			break;
		default:
			fprintf (stderr,"supscan: Invalid flag %s ignored\n",argv[1]);
			(void) fflush (stderr);
		}
		--argc;
		argv++;
	}
	if (!fflag) {
		(void) sprintf (fbuf,FILEDIRS,DEFDIR);
		filename = fbuf;
	}
	if (sflag) {
		if (argc != 1)
			usage ();
		firstC = NULL;
		c = &firstC;
		(void) sprintf (buf,FILEHOSTS,DEFDIR);
		if ((f = fopen (buf,"r")) == NULL)
			quit (1,"supscan: Unable to open %s\n",buf);
		while ((p = fgets (buf,STRINGLENGTH,f)) != NULL) {
			q = index (p,'\n');
			if (q)  *q = 0;
			if (index ("#;:",*p))  continue;
			collname = nxtarg (&p," \t=");
			p = skipover (p," \t=");
			if (!localhost (p))  continue;
			*c = getcoll(filename,salloc (collname),
					(char *)NULL);
			if (*c)  c = &((*c)->Cnext);
		}
		(void) fclose (f);
		return;
	}
	if (argc < 2 && fflag) {
		firstC = NULL;
		c = &firstC;
		if ((f = fopen (filename,"r")) == NULL)
			quit (1,"supscan: Unable to open %s\n",filename);
		while (p = fgets (buf,STRINGLENGTH,f)) {
			q = index (p,'\n');
			if (q)  *q = 0;
			if (index ("#;:",*p))  continue;
			q = nxtarg (&p," \t=");
			p = skipover (p," \t=");
			*c = getcoll(filename,salloc (q),salloc (p));
			if (*c)  c = &((*c)->Cnext);
		}
		(void) fclose (f);
		return;
	}
	if (argc < 2 || argc > 3)
		usage ();
	firstC = getcoll(filename,salloc (argv[1]),
			argc > 2 ? salloc (argv[2]) : (char *)NULL);
}

COLLECTION *
getcoll(filename, collname, basedir)
register char *filename,*collname,*basedir;
{
	char buf[STRINGLENGTH],*p,*q;
	FILE *f;
	COLLECTION *c;

	if (basedir == NULL) {
		if (f = fopen (filename,"r")) {
			while (p = fgets (buf,STRINGLENGTH,f)) {
				q = index (p,'\n');
				if (q)  *q = 0;
				if (index ("#;:",*p))  continue;
				q = nxtarg (&p," \t=");
				if (strcmp (q,collname) == 0) {
					p = skipover (p," \t=");
					basedir = salloc (p);
					break;
				}
			}
			(void) fclose (f);
		}
		if (basedir == NULL) {
			(void) sprintf (buf,FILEBASEDEFAULT,collname);
			basedir = salloc (buf);
		}
	}
	if (chdir(basedir) < 0) {
		fprintf (stderr,"supscan:  Can't chdir to base directory %s for %s\n",
			basedir,collname);
		return (NULL);
	}
	prefix = NULL;
	(void) sprintf (buf,FILEPREFIX,collname);
	if (f = fopen (buf,"r")) {
		while (p = fgets (buf,STRINGLENGTH,f)) {
			q = index (p,'\n');
			if (q) *q = 0;
			if (index ("#;:",*p))  continue;
			prefix = salloc (p);
			if (chdir(prefix) < 0) {
				fprintf (stderr,"supscan: can't chdir to %s from base directory %s for %s\n",
					prefix,basedir,collname);
				return (NULL);
			}
			break;
		}
		(void) fclose (f);
	}
	if ((c = (COLLECTION *) malloc (sizeof(COLLECTION))) == NULL)
		quit (1,"supscan: can't malloc collection structure\n");
	c->Cname = collname;
	c->Cbase = basedir;
	c->Cprefix = prefix;
	c->Cnext = NULL;
	return (c);
}

#if __STDC__
goaway (char *fmt,...)
#else
/* VARARGS */
goaway (va_alist)
va_dcl
#endif
{
#if !__STDC__
	char *fmt;
#endif
	va_list ap;

#if __STDC__
	va_start(ap,fmt);
#else
	va_start(ap);
	fmt = va_arg(ap,char *);
#endif
	vfprintf(stderr, fmt, ap);
	va_end(ap);
	(void) putc ('\n',stderr);
	(void) fflush (stderr);
	longjmp (sjbuf,TRUE);
}

int localhost (host)
register char *host;
{
	register struct hostent *h;
	static char myhost[STRINGLENGTH];
	int i;
	if (*myhost == '\0') {
		(void) gethostname (myhost,sizeof (myhost));
		for (i = 0; i < 5 && (h = gethostbyname (myhost)) == NULL; i++)
		    (void) sleep(30);
		/* Should we just use kernel name? -- DLC */
		if (h == NULL) quit (1,"supscan: can't find my host entry\n");
		(void) strcpy (myhost,h->h_name);
	}
	h = gethostbyname (host);
	/* We will not retry since the worst that will happen is that this
	 * collection will not be scanned this run -- DLC.
	 */
	if (h == NULL) return (0);
	return (strcasecmp (myhost,h->h_name) == 0);
}
