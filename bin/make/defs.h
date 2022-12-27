#ifndef lint
static char *RCSdefs = "$Id: defs.h,v 1.7 90/05/07 17:20:21 bww Exp $";
#endif

/*
 * Copyright (c) 1989 Carnegie-Mellon University
 * All rights reserved.  The CMU software License Agreement specifies
 * the terms and conditions for use and redistribution.
 ************************************************************************
 * HISTORY
 * $Log:	defs.h,v $
 * Revision 5.1  90/08/06  18:18:52  mrt
 * 	Picked up bww's changes from Mt Xinu
 * 	[90/08/06  17:23:38  mrt]
 * 
 * Revision 1.7  90/05/07  17:20:21  bww
 * 	Added declaration of botchflag.
 * 	[90/05/07  17:06:44  bww]
 * 
 * Revision 1.6  90/05/07  12:40:39  bww
 * 	Added isarch field to nameblock.
 * 	[90/05/07  12:22:05  bww]
 * 
 * Revision 1.5  90/02/01  20:47:01  bww
 * 	Added lockp and exok fields.  Modified *_cmd.
 * 	[90/02/01  20:31:33  bww]
 * 
 * Revision 1.4  89/12/19  16:05:04  bww
 * 	Defined return type for isdependent().
 * 	[89/12/19  16:00:47  bww]
 * 
 * Revision 1.3  89/11/20  14:17:37  bww
 * 	Reduced buffer size so that it compiles under pcc on an RT.
 * 	[89/11/20  14:12:54  bww]
 * 
 * Revision 1.2  89/05/26  10:06:17  bww
 * 	CMU CS as of 89/05/15
 * 	[89/05/26  09:46:50  bww]
 * 
 * Revision 5.0  89/03/01  01:36:56  bww
 * 	Version 5.
 * 	[89/03/01  01:27:54  bww]
 * 
 */

#ifndef lint
static	char *sccsdefs = "@(#)defs 4.10 86/03/22";
#endif

#include <sys/param.h>
#include <sys/stat.h>
#include <sys/dir.h>
#include <strings.h>
#include <ctype.h>
#include <stdio.h>

#ifndef dirfd
#define dirfd(dirp)	((dirp)->dd_fd)
#endif

#define SHELLCOM	"/bin/sh"

#define RCS		"RCS"	/* name of RCS dir */
#define RCS_SUF		",v"	/* suffix of RCS files */

/*
 * to install metering, add a statement like
 * #define METERFILE "/usr/sif/make/Meter"
 * to turn metering on, set external variable meteron to 1.
 */

/*
 * define FSTATIC to be static on systems with C compilers
 * supporting file-static; otherwise define it to be null
 */
#define FSTATIC		static

#define NO		0
#define YES		1

#define unequal		strcmp
#define HASHSIZE	8191
#define INMAX		15000
#define OUTMAX		15000
#define QBUFMAX		15000

#define ALLDEPS		1
#define SOMEDEPS	2

#define META		01
#define TERMINAL	02
extern char funny[128];

#ifdef lint
#define ALLOC(x)	((struct x *) 0)
#else
#define ALLOC(x)	((struct x *) ckalloc(sizeof(struct x)))
#endif

/*
 * RCS-specific declarations
 */
extern int	coflag;			/* auto-checkout flag */
extern int	rmflag;			/* auto-remove flag */
extern int	nocmds;			/* don't make, just check-out */
extern int	rcsdep;			/* do "foo: foo,v" dependencies */
extern struct lineblock *co_cmd;	/* auto-checkout shell command */
extern struct lineblock *rcstime_cmd;	/* rcs modtime shell command */
extern char	*RCSdir;		/* name of RCS dir */
extern char	*RCSsuf; 		/* suffix of RCS files */

extern int dbgflag;
extern int prtrflag;
extern int silflag;
extern int noexflag;
extern int keepgoing;
extern int descflag;
extern int noruleflag;
extern int noconfig;
extern int touchflag;
extern int questflag;
extern int machdep;
extern int ndocoms;
extern int ignerr;
extern int botchflag;
extern int okdel;
extern int inarglist;
extern int ininternal;
extern char *prompt;
extern char *curfname;

struct nameblock {
	struct nameblock *nxtnameblock;
	char *namep;
	char *alias;
	struct lineblock *linep;
	struct tmacblock *tmacp;
	unsigned int done:2;
	unsigned int septype:2;
	unsigned int rundep:1;
	unsigned int objarch:1;
	unsigned int dollar:1;
	unsigned int percent:1;
	unsigned int bquotes:1;
	unsigned int isarch:1;
	struct nameblock *archp;
	char *RCSnamep;
	int hashval;
	time_t modtime;
};
extern struct nameblock *mainname;
extern struct nameblock *firstname;
extern struct nameblock *inobjdir;

struct lineblock {
	struct lineblock *nxtlineblock;
	struct nameblock *lockp;
	struct depblock *depp;
	struct shblock *shp;
};
extern struct lineblock *sufflist;

struct depblock {
	struct depblock *nxtdepblock;
	struct nameblock *depname;
};

struct shblock {
	struct shblock *nxtshblock;
	char *shbp;
	int exok;
};

struct tmacblock {
	struct tmacblock *nxttmacblock;
	char *tmacbp;
};

struct varblock {
	struct varblock *lftvarblock;
	struct varblock *rgtvarblock;
	char *varname;
	char *varval;
	short varlen;
	unsigned int noreset:1;
	unsigned int used:1;
	unsigned int builtin:1;
};
extern struct varblock *firstvar;

struct pattern {
	struct pattern *lftpattern;
	struct pattern *rgtpattern;
	char *patval;
	short patlen;
};

struct dirhdr {
	struct dirhdr *nxtopendir;
	char **ntable;
	int nents;
	int nsize;
	DIR *dirfc;
	char *dirn;
	time_t mtime;
};
extern struct dirhdr *firstod;

struct chain {
	struct chain *nextp;
	char *datap;
};
extern struct chain *rmchain;
extern struct chain *deschain;

extern char *copys(), *concat(), *subst(), *ncat(), *ckalloc();
extern struct nameblock *srchname(), *makename(), *rcsco();
extern struct varblock *varptr(), *srchvar();
extern struct lineblock *isdependent();
extern time_t exists();
