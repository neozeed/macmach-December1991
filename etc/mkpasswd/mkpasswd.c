/*
 * Copyright (c) 1980, 1983 Regents of the University of California.
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms are permitted
 * provided that the above copyright notice and this paragraph are
 * duplicated in all such forms and that any documentation,
 * advertising materials, and other materials related to such
 * distribution and use acknowledge that the software was developed
 * by the University of California, Berkeley.  The name of the
 * University may not be used to endorse or promote products derived
 * from this software without specific prior written permission.
 * THIS SOFTWARE IS PROVIDED ``AS IS'' AND WITHOUT ANY EXPRESS OR
 * IMPLIED WARRANTIES, INCLUDING, WITHOUT LIMITATION, THE IMPLIED
 * WARRANTIES OF MERCHANTIBILITY AND FITNESS FOR A PARTICULAR PURPOSE.
 */
/* 22-Feb-89  Zon Williamson (zon) at Carnegie Mellon University
 *	Added passwd.key compatability for mac2.
 */

#ifndef lint
char copyright[] =
"@(#) Copyright (c) 1980, 1983 Regents of the University of California.\n\
 All rights reserved.\n";
#endif /* not lint */

#ifndef lint
static char sccsid[] = "@(#)mkpasswd.c	5.3 (Berkeley) 6/18/88";
#endif /* not lint */

#include <sys/file.h>
#include <stdio.h>
#include <pwd.h>
#include <ndbm.h>

#ifdef mac2

#include <sys/types.h>
#include <sys/stat.h>

static char EMPTY[] = "";
static char PASSWD[200]	= "/etc/passwd";
static char PASSWDKEY[200]	= "/etc/passwd.key";
static FILE *pwf = NULL;
static FILE *pwkey = NULL;
static char line[BUFSIZ+1];
static struct passwd passwd;
static int NumberOfUids =0;
static int NumberOfEntries =0;
static int NumberOfValidUids =0;
static char  *pwskip();

struct passwd *nextpwent()
{
  register char *p;

  if (pwf == NULL) {
    if( (pwf = fopen( PASSWD, "r" )) == NULL )return(0);
  }
  p = fgets(line, BUFSIZ, pwf);
  if (p==NULL) return(0);
  passwd.pw_name = p;
  p = pwskip(p);
  passwd.pw_passwd = p;
  p = pwskip(p);
  passwd.pw_uid = atoi(p);
  p = pwskip(p);
  passwd.pw_gid = atoi(p);
  passwd.pw_quota = 0;
  passwd.pw_comment = EMPTY;
  p = pwskip(p);
  passwd.pw_gecos = p;
  p = pwskip(p);
  passwd.pw_dir = p;
  p = pwskip(p);
  passwd.pw_shell = p;
  while(*p && *p != '\n') p++;
  *p = '\0';
  return(&passwd);
}

static char *pwskip(p)
register char *p;
{
  while( *p && *p != ':' ) ++p;
  if( *p ) *p++ = 0;
  return(p);
}

GetMaxuid()
{
  register struct passwd *p;
  struct passwd *getpwent();
  int max=0;

  while (p = nextpwent()){ 
    if(max < p->pw_uid)  max = p->pw_uid; 
    NumberOfEntries++;
  }
  rewind(pwf);
  return (max);
}

pwkeygen(argc,argv)
int argc;
char *argv[];
{
  int i;
  register char *p;
  long *keys;
  long offset=0;

  if (argc == 3){
    strcpy(PASSWD, argv[1]);
    strcpy(PASSWDKEY, argv[2]);
  }
  if ((argc ==2) || (argc > 3))usage();
  if (pwf == NULL) {
    if( (pwf = fopen( PASSWD, "r" )) == NULL ){
      fprintf(stderr,"can\'t open %s\n",PASSWD);
      exit(1);
    }
  }
  NumberOfUids = GetMaxuid() + 1;
  if ((keys = (long *)malloc(sizeof(int) * NumberOfUids)) == 0){
    fprintf(stderr,"out of core for internal table\n");
    exit(1);
  }
  for (i=0;i<NumberOfUids;i++) keys[i]= -1;
  while((p = fgets(line, BUFSIZ, pwf))!=NULL) {
    sscanf(index(index(p,':')+1,':')+1,"%d",&i);
    keys[i] = offset;
    offset += strlen(p);
    NumberOfValidUids++;
  }
  if ((keys[0] == -1) || (NumberOfValidUids != NumberOfEntries)){
    printf("Generated %d keys for %d passwd entries. Offset for root = %d\n",NumberOfValidUids, NumberOfEntries,keys[0]);
    fprintf(stderr,"Aborted generation of a bad passwd.key. Please try again!\n");
    exit(1);
  }
  if( (pwkey = fopen( PASSWDKEY, "w" )) == NULL ) {
    printf("can't create %s\n", PASSWDKEY);
    exit(1);
  }
  for (i=0;i<NumberOfUids;i++) fprintf(pwkey,"%6ld\n",keys[i]);
  fclose(pwkey);
  exit(0);
}

usage()
{
  fprintf(stderr,"Usage: mkpasswd -k [passwd file] [passwd key]\n	with no arg, makes /etc/passwd.key from /etc/passwd\n");
  exit(1);
}

#endif

main(argc, argv)
	int argc;
	char *argv[];
{
	DBM *dp;
	datum key, content;
	register char *cp, *tp;
	register struct passwd *pwd;
	int verbose = 0, entries = 0, maxlen = 0;
	char buf[BUFSIZ];

	if (argc > 1 && strcmp(argv[1], "-v") == 0) {
		verbose++;
		argv++, argc--;
	}
#ifdef mac2
	if (argc > 1 && strcmp(argv[1], "-k") == 0) {
	  argv++, argc--;
	  pwkeygen(argc, argv);
	  exit(0);
	}
#endif
	if (argc != 2) {
		fprintf(stderr, "usage: mkpasswd [ -v ] file\n");
		exit(1);
	}
	if (access(argv[1], R_OK) < 0) {
		fprintf(stderr, "mkpasswd: ");
		perror(argv[1]);
		exit(1);
	}
	(void)umask(0);
	dp = dbm_open(argv[1], O_WRONLY|O_CREAT|O_EXCL, 0644);
	if (dp == NULL) {
		fprintf(stderr, "mkpasswd: ");
		perror(argv[1]);
		exit(1);
	}
	setpwfile(argv[1]);
	while (pwd = getpwent()) {
		cp = buf;
#define	COMPACT(e)	tp = pwd->e; while (*cp++ = *tp++);
		COMPACT(pw_name);
		COMPACT(pw_passwd);
		bcopy((char *)&pwd->pw_uid, cp, sizeof (int));
		cp += sizeof (int);
		bcopy((char *)&pwd->pw_gid, cp, sizeof (int));
		cp += sizeof (int);
		bcopy((char *)&pwd->pw_quota, cp, sizeof (int));
		cp += sizeof (int);
		COMPACT(pw_comment);
		COMPACT(pw_gecos);
		COMPACT(pw_dir);
		COMPACT(pw_shell);
		content.dptr = buf;
		content.dsize = cp - buf;
		if (verbose)
			printf("store %s, uid %d\n", pwd->pw_name, pwd->pw_uid);
		key.dptr = pwd->pw_name;
		key.dsize = strlen(pwd->pw_name);
		if (dbm_store(dp, key, content, DBM_INSERT) < 0) {
			fprintf(stderr, "mkpasswd: ");
			perror("dbm_store failed");
			exit(1);
		}
		key.dptr = (char *)&pwd->pw_uid;
		key.dsize = sizeof (int);
		if (dbm_store(dp, key, content, DBM_INSERT) < 0) {
			fprintf(stderr, "mkpasswd: ");
			perror("dbm_store failed");
			exit(1);
		}
		entries++;
		if (cp - buf > maxlen)
			maxlen = cp - buf;
	}
	dbm_close(dp);
	printf("%d password entries, maximum length %d\n", entries, maxlen);
	exit(0);
}
