/*
 * Copyright (c) 1983 Regents of the University of California.
 * All rights reserved.  The Berkeley software License Agreement
 * specifies the terms and conditions for redistribution.
 */
/* 20-Dec-89  Zon Williamson (zon) at Carnegie Mellon University
 *	Added passwd.key compatability for mac2.
 */

#if defined(LIBC_SCCS) && !defined(lint)
static char sccsid[] = "@(#)getpwnamuid.c	5.3 (Berkeley) 12/21/87";
#endif LIBC_SCCS and not lint

#include <stdio.h>
#include <pwd.h>
#include <ndbm.h>

#include <sys/file.h>

#ifdef mac2
#include <sys/types.h>
#include <sys/stat.h>
#endif

static char line[BUFSIZ+1];
static struct passwd passwd;

/*
 * The following are shared with getpwent.c
 */
extern	char *_pw_file;
DBM	*_pw_db;
int	_pw_stayopen;
#ifdef mac2
extern FILE *_pwf;
extern FILE *_pwkey;
#endif

static struct passwd *
fetchpw(key)
	datum key;
{
        register char *cp, *tp;

        if (key.dptr == 0)
                return ((struct passwd *)NULL);
	key = dbm_fetch(_pw_db, key);
	if (key.dptr == 0)
                return ((struct passwd *)NULL);
        cp = key.dptr;
	tp = line;

#define	EXPAND(e)	passwd.e = tp; while (*tp++ = *cp++);
	EXPAND(pw_name);
	EXPAND(pw_passwd);
	bcopy(cp, (char *)&passwd.pw_uid, sizeof (int));
	cp += sizeof (int);
	bcopy(cp, (char *)&passwd.pw_gid, sizeof (int));
	cp += sizeof (int);
	bcopy(cp, (char *)&passwd.pw_quota, sizeof (int));
	cp += sizeof (int);
	EXPAND(pw_comment);
	EXPAND(pw_gecos);
	EXPAND(pw_dir);
	EXPAND(pw_shell);
        return (&passwd);
}

#ifdef mac2

static char EMPTY[] = "";

static struct passwd *passwdstruct(p)
char *p;
{
  extern char *_pwskip();

  if (p==NULL) return(0);
  passwd.pw_name = p;
  p = _pwskip(p);
  passwd.pw_passwd = p;
  p = _pwskip(p);
  passwd.pw_uid = atoi(p);
  p = _pwskip(p);
  passwd.pw_gid = atoi(p);
  passwd.pw_quota = 0;
  passwd.pw_comment = EMPTY;
  p = _pwskip(p);
  passwd.pw_gecos = p;
  p = _pwskip(p);
  passwd.pw_dir = p;
  p = _pwskip(p);
  passwd.pw_shell = p;
  while(*p && *p != '\n') p++;
  *p = '\0';
  return(&passwd);
}
	
static int openkey()
{
  if (_pw_db != (DBM *)0) return 0;
  setpwent();
  return _pwkey != NULL;
}

#endif

struct passwd *
getpwnam(nam)
	char *nam;
{
        datum key;
	register struct passwd *pw;

#ifdef mac2
  /* if not dbm version and key file exists, do binary search for name*/
  /* see /etc/mkpasswd for key generation algorithm */
  if (openkey()) {
    register char *p;
    char c;
    int to, from, mid, interval, test, keylen, adjust;
    struct stat statbuf;

    keylen = strlen(nam);
    if (fstat(fileno(_pwf), &statbuf)) goto badkey;
    to = statbuf.st_size;
    from = adjust = 0;
    interval = to - from;
    while (interval > adjust) {
	mid = (to + from) / 2;
	interval = mid - from;
	if (fseek(_pwf, mid, 0) == -1) goto badkey;
	if ((p = fgets(line, BUFSIZ, _pwf))==NULL) break;
	mid += (adjust = strlen(p));
	if (adjust>=interval) break;
	if((p = fgets(line, BUFSIZ, _pwf))==NULL)  break;
	if (!(test = strncmp(nam, p, keylen))) {
	  if ((c = *(p + keylen)) == ':')  {
            if (!_pw_stayopen) endpwent();
	    return (passwdstruct(p));
          }
	  test = -1;
	  if (c < ':')  test = 1;
	}
	if (test < 0 ) to = mid;
	else from = mid;
    }
    if (fseek(_pwf,from,0) == -1) goto badkey;
    while(from < to) {
      if((p = fgets(line, BUFSIZ, _pwf))==NULL)  goto badkey;
      if (!strncmp(nam,p,keylen) && *(p+keylen) == ':')  {
        if (!_pw_stayopen) endpwent();
      	return (passwdstruct(p));
      }
      from += strlen(p);
    }
badkey:
    endpwent();
  }
#endif
        if (_pw_db == (DBM *)0 &&
	    (_pw_db = dbm_open(_pw_file, O_RDONLY)) == (DBM *)0) {
	oldcode:
		setpwent();
		while ((pw = getpwent()) && strcmp(nam, pw->pw_name))
			;
		if (!_pw_stayopen)
			endpwent();
		return (pw);
	}
	if (flock(dbm_dirfno(_pw_db), LOCK_SH) < 0) {
		dbm_close(_pw_db);
		_pw_db = (DBM *)0;
		goto oldcode;
	}
        key.dptr = nam;
        key.dsize = strlen(nam);
	pw = fetchpw(key);
	(void) flock(dbm_dirfno(_pw_db), LOCK_UN);
	if (!_pw_stayopen) {
		dbm_close(_pw_db);
		_pw_db = (DBM *)0;
	}
        return (pw);
}

struct passwd *
getpwuid(uid)
	int uid;
{
        datum key;
	register struct passwd *pw;

#ifdef mac2
  if (openkey()) {
    register char *p;
    struct passwd *pp;
    long location,indx;

    indx = 7 * uid;
    if (fseek(_pwkey, indx, 0) == -1) goto badkey;
    if (fscanf(_pwkey,"%7ld",&location) == EOF) goto badkey;
    if (location < 0) {
      if (!_pw_stayopen) endpwent();
      return 0;
    }
    if (location > 0) {
      if (fseek(_pwf, location - 1, 0) == -1 || fgetc(_pwf) != '\n') goto badkey;
    }
    else {
      if (fseek(_pwf, location, 0) == -1) goto badkey;
    }
    if ((p = fgets(line, BUFSIZ, _pwf)) == NULL) goto badkey;
    pp = passwdstruct(p);
    if (pp->pw_uid != uid) goto badkey;
    if (!_pw_stayopen) endpwent();
    return pp;
badkey:
    endpwent();
  }
#endif
        if (_pw_db == (DBM *)0 &&
	    (_pw_db = dbm_open(_pw_file, O_RDONLY)) == (DBM *)0) {
	oldcode:
		setpwent();
		while ((pw = getpwent()) && pw->pw_uid != uid)
			;
		if (!_pw_stayopen)
			endpwent();
		return (pw);
	}
	if (flock(dbm_dirfno(_pw_db), LOCK_SH) < 0) {
		dbm_close(_pw_db);
		_pw_db = (DBM *)0;
		goto oldcode;
	}
        key.dptr = (char *) &uid;
        key.dsize = sizeof uid;
	pw = fetchpw(key);
	(void) flock(dbm_dirfno(_pw_db), LOCK_UN);
	if (!_pw_stayopen) {
		dbm_close(_pw_db);
		_pw_db = (DBM *)0;
	}
        return (pw);
}
