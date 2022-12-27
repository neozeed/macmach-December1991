#ifndef lint
static char *RCSid = "$Id: dirs.c,v 1.5 90/05/07 15:57:59 bww Exp $";
#endif

/*
 * Copyright (c) 1989 Carnegie-Mellon University
 * All rights reserved.  The CMU software License Agreement specifies
 * the terms and conditions for use and redistribution.
 ************************************************************************
 * HISTORY
 * $Log:	dirs.c,v $
 * Revision 5.1  90/08/06  18:19:35  mrt
 * 	Picked up bww's changes from Mt Xinu
 * 	[90/08/06  17:21:16  mrt]
 * 
 * Revision 1.5  90/05/07  15:57:59  bww
 * 	fatal() now accepts printf-style arguments.
 * 	[90/05/07  15:49:35  bww]
 * 
 * Revision 1.4  90/04/30  15:20:01  bww
 * 	Added forward declarations of static functions.
 * 	[90/04/30  15:06:40  bww]
 * 
 * Revision 1.3  90/02/01  20:47:25  bww
 * 	Updated RCS header.
 * 	[90/02/01  20:31:59  bww]
 * 
 * Revision 1.2  89/05/26  10:05:30  bww
 * 	CMU CS as of 89/05/15
 * 	[89/05/26  09:46:50  bww]
 * 
 * Revision 5.0  89/03/01  01:37:11  bww
 * 	Version 5.
 * 	[89/03/01  01:28:19  bww]
 * 
 */

/*
 * directory access/caching
 */

#include "defs.h"
#include <errno.h>
#include <fcntl.h>

#define DIRTABSIZE	64
#define DIRHASHSIZE	509

extern int errno;
extern char *strerror();
extern char *malloc(), *realloc();

extern char vpath[];
extern char machine[];

FSTATIC DIR *cdirf = 0;
FSTATIC struct dirhdr cdhdr;
FSTATIC struct dirhdr *dirhashtab[DIRHASHSIZE];
FSTATIC dhashed = 0;
FSTATIC nopdir = 0;
FSTATIC maxdir = 0;
FSTATIC struct dirhdr *mscandir();


FSTATIC int
dhashloc(nam)
	register char *nam;
{
	register char *p;
	register unsigned int i;
	
	i = 0;
	for (p = nam; *p; p++)
		i += (*p << 8) | *(p + 1);
	i %= DIRHASHSIZE;

	while (dirhashtab[i] && unequal(nam, dirhashtab[i]->dirn))
		i = (i + 1) % DIRHASHSIZE;

	return i;
}


FSTATIC struct dirhdr *
finddir(nam)
	register char *nam;
{
	register struct dirhdr *dh;
	register int loc;
	struct stat sbuf;

	if (*nam == 0 || (*nam == '.' && (*(nam+1) == 0
	|| (*(nam+1) == '/' && *(nam+2) == 0)))) {
		if (cdirf)
			rewinddir(cdirf);
		else {
			if ((cdirf = opendir(nam)) == 0)
				return 0;
			(void) fcntl(dirfd(cdirf), F_SETFD, 1);
		}
		return &cdhdr;
	}

	if (dh = dirhashtab[loc = dhashloc(nam)]) {
		if (stat(nam, &sbuf) == -1)
			return 0;
		if (sbuf.st_mtime != dh->mtime)
			dirhashtab[loc] = mscandir(dh, nam, &sbuf);
		return dh;
	}

	if ((dh = mscandir((struct dirhdr *)0, nam, &sbuf)) == 0)
		return 0;

	if (dhashed++ > DIRHASHSIZE-8)
		fatal("Directory hash table overflow");
	dirhashtab[loc] = dh;
	return dh;
}


FSTATIC int
ptrcmp(a, b)
	register char **a, **b;
{
	register int c;

	if (c = **a - **b)
		return c;
	return strcmp(*a, *b);
}


FSTATIC struct dirhdr *
mscandir(dh, nam, s)
	register struct dirhdr *dh;
	char *nam;
	struct stat *s;
{
	register DIR *dirf;
	register struct direct *dptr;

	if (dh == 0) {
		if (stat(nam, s) == -1)
			return 0;
		dh = ALLOC(dirhdr);
		dh->nxtopendir = 0;
		dh->ntable = 0;
		dh->dirfc = 0;
		dh->dirn = copys(nam);
	}
	dh->nents = 0;
	dh->mtime = s->st_mtime;

	if (dh->dirfc == 0) {
		errno = 0;
		if ((dirf = opendir(nam)) == 0) {
			if (errno == ENOENT)
				return dh;
			fatal("%s: %s", nam, strerror(errno));
		}
		if (maxdir == 0) {
			maxdir = getdtablesize() - 8;
			if (maxdir < 12)
				fatal("NOFILE < 20");
		}
		if (nopdir < maxdir) {
			nopdir++;
			dh->nxtopendir = firstod;
			firstod = dh;
			dh->dirfc = dirf;
			(void) fcntl(dirfd(dirf), F_SETFD, 1);
		}
	} else {
		dirf = dh->dirfc;
		rewinddir(dirf);
	}

	while (dptr = readdir(dirf)) {
		if (dh->nents == dh->nsize) {
			if (dh->ntable == 0) {
				dh->nsize = DIRTABSIZE;
				dh->ntable = (char **) malloc(
					(unsigned) dh->nsize * sizeof(char *));
			} else {
				dh->nsize <<= 1;
				dh->ntable = (char **) realloc(
					(char *) dh->ntable,
					(unsigned) dh->nsize * sizeof(char *));
			}
			if (dh->ntable == 0)
				fatal("out of memory");
		}
		dh->ntable[dh->nents++] = copys(dptr->d_name);
	}

	qsort((char *) dh->ntable, dh->nents, sizeof(char *), ptrcmp);

	if (dh->dirfc == 0)
		closedir(dirf);
	return dh;
}


FSTATIC int
prevpat(pat)
	register char *pat;
{
	register struct pattern *patp, *lpatp;
	register int len, n;
	static struct pattern *firstpat = 0;

	len = strlen(pat);
	lpatp = 0;
	patp = firstpat;
	while (patp) {
		if ((n = (len - patp->patlen)) == 0
		&& (n = (*pat - *patp->patval)) == 0
		&& (n = strcmp(pat, patp->patval)) == 0)
			return 1;
		lpatp = patp;
		patp = (n < 0) ? patp->lftpattern : patp->rgtpattern;
	}

	patp = ALLOC(pattern);
	patp->lftpattern = 0;
	patp->rgtpattern = 0;
	patp->patval = copys(pat);
	patp->patlen = len;

	if (lpatp == 0)
		firstpat = patp;
	else if (n < 0)
		lpatp->lftpattern = patp;
	else
		lpatp->rgtpattern = patp;

	return 0;
}


FSTATIC struct depblock *
addpref(dirname, p, rcspref, mkchain, nextdbl)
	char *dirname, *p, *rcspref;
	int mkchain;
	struct depblock *nextdbl;
{
	struct nameblock *q;
	struct depblock *thisdbl;
	char fullname[MAXPATHLEN+1];
	char nbuf[MAXNAMLEN+1];

	q = makename(concat(dirname, p, fullname));
	if (mkchain) {
		thisdbl = ALLOC(depblock);
		thisdbl->nxtdepblock = nextdbl;
		thisdbl->depname = q;
	} else
		thisdbl = 0;
	/*
	 * Strip any VPATH prefix and any "RCS/" suffix
	 * from the directory name, and any ",v" suffix
	 * from the file name so that implicit rules
	 * can find the corresponding files.
	 */
	(void) makename(concat(rcspref, suffix(p, RCSsuf, nbuf)
					? nbuf : p, fullname));
	return thisdbl;
}


FSTATIC struct depblock *
srchpref(vpref, dpref, rcspref, fpat, mkchain, nextdbl)
	char *vpref, *dpref, *rcspref, *fpat;
	int mkchain;
	struct depblock *nextdbl;
{
	register struct direct *dptr;
	register int i, j, k;
	register char *p, **pp;
	struct dirhdr *d;
	char *dirname, *q;
	struct depblock *thisdbl;
	char temp[MAXPATHLEN+1];
	char pbuf[MAXNAMLEN+1];
	int plen, prefix;

	if ((d = finddir(dirname = concat(vpref, dpref, temp))) == 0)
		return 0;

	thisdbl = 0;
	for (p = pbuf, q = fpat; *p = *q; p++, q++)
		if (index("*?[\\", *p)) {
			*p = 0;
			break;
		}
	prefix = (*q == '*' && *(q+1) == 0);
	plen = p - pbuf;

	if (d == &cdhdr) {
		while (dptr = readdir(cdirf)) {
			p = dptr->d_name;
			if (plen && bcmp(pbuf, p, plen))
				continue;
			if (!prefix && !amatch(p, fpat))
				continue;
			nextdbl = thisdbl = addpref(dirname, p, rcspref,
							mkchain, nextdbl);
		}
		return thisdbl;
	}

	i = -1;  j = d->nents;  q = pbuf;
	while (i + 1 != j) {
		k = (i + j) / 2;
		if (ptrcmp(&q, d->ntable + k) >= 0)
			i = k;
		else
			j = k;
	}
	if (i == -1 || ptrcmp(&q, d->ntable + i))
		i++;

	for (pp = d->ntable + i, i = d->nents - i; i; pp++, i--) {
		p = *pp;
		if (plen && bcmp(pbuf, p, plen))
			break;
		if (!prefix && !amatch(p, fpat))
			continue;
		nextdbl = thisdbl = addpref(dirname, p, rcspref,
						mkchain, nextdbl);
	}
	return thisdbl;
}


struct depblock *
srchdir(pat, mkchain, nextdbl)
	char *pat;		/* pattern to be matched in directory */
	int mkchain;		/* nonzero if results to be remembered */
	struct depblock *nextdbl; /* final value for chain */
{
	register char *endir, *dirpref, *filepat, *RCSpref;
	register char *p;
	register struct depblock *db;
	struct depblock *thisdbl;
	char vpref[MAXPATHLEN+1], pth[MAXPATHLEN+1];
	char temp[MAXPATHLEN+1], temp2[MAXPATHLEN+1];
	extern char *execat();

	if (mkchain == NO && prevpat(pat))
		return 0;

	/*
	 * dirpref == directory part of "pat" with trailing '/'
	 * RCSpref == dirpref sans any trailing RCSdir or $(MACHINE)
	 * filepat == file part of "pat"
	 */
	if ((endir = rindex(pat, '/')) == 0) {
		dirpref = RCSpref = "";
		filepat = pat;
	} else {
		*endir = 0;
		dirpref = RCSpref = concat(pat, "/", pth);
		if (coflag && mkchain == NO) {
			if (!unequal(pat, RCSdir))
				RCSpref = "";
			else if (suffix(pat, concat("/", RCSdir, temp), temp))
				(void) strcat(RCSpref = temp, "/");
		}
		if (machdep && mkchain == NO
		&& (p = rindex(RCSpref, '/'))) {
			if (*(p+1))
				fatal("Cannot happen in srchdir");
			*p = 0;
			if (!unequal(RCSpref, machine))
				RCSpref = "";
			else if (suffix(RCSpref, concat("/", machine, temp2), temp2))
				(void) strcat(RCSpref = temp2, "/");
			*p = '/';
		}
		filepat = endir + 1;
	}

	if (thisdbl = srchpref("", dirpref, RCSpref, filepat, mkchain, nextdbl))
		nextdbl = thisdbl;

	if (*pat == '/'
	|| (*pat == '.' && *(pat+1) == '/')
	|| *(p = vpath) == 0)
		goto out;

	/*
	 * vpref == VPATH component + '/' if non-null
	 */
	do {
		p = execat(p, "", vpref);
		if (*vpref == 0)
			continue;
		if (db = srchpref(vpref, dirpref, RCSpref, filepat, mkchain, nextdbl))
			nextdbl = thisdbl = db;
	} while(p);

out:
	if (endir)
		*endir = '/';

	return thisdbl;
}
