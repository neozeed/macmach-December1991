/*
 * Copyright (c) 1987 Regents of the University of California.
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

#ifndef lint
char copyright[] =
"@(#) Copyright (c) 1987 Regents of the University of California.\n\
 All rights reserved.\n";
#endif /* not lint */

#ifndef lint
static char sccsid[] = "@(#)man.c	5.17 (Berkeley) 6/29/88";
#endif /* not lint */

#include <sys/param.h>
#include <sys/file.h>
#include <stdio.h>
#include <ctype.h>

#define	DEF_PAGER	"/usr/ucb/more -s"
#define	DEF_PATH	"/usr/man:/usr/local/man"
#define	LOCAL_PATH	"/usr/local/man"
#define MANROFF		"/usr/man/manroff"

#define	NO	0
#define	YES	1

static char	*command,		/* command buffer */
		*defpath,		/* default search path */
		*locpath,		/* local search path */
		*machine,		/* machine type */
		*manpath,		/* current search path */
		*newpath,		/* new search path */
		*pager,			/* requested pager */
		*manroff,		/* text formatter */
		how;			/* how to display */
static int	command_size,		/* size of command buffer */
		page_size;		/* system memory page size */

#define	ALL	0x1			/* show all man pages */
#define	CAT	0x2			/* copy file to stdout */
#define	WHERE	0x4			/* just tell me where, -w */
#define ROFF	0x8			/* force manroff, -r */
#define Z       0x10                    /* force compression, -z */

typedef struct {
	char	*obj, *objext, *src, *srcext, *msg;
} DIR;
static DIR	list1[] = {		/* section one list */
	"cat1", "0", "man1", "1", "1st",
	"cat8", "0", "man8", "8", "8th",
	"cat6", "0", "man6", "6", "6th",
	"cat.old", "0", "man.old", "old", "old",
	"catn", "0", "mann", "n", "new",
	"catx", "0", "manx", "x", "X Window System",
	NULL, NULL, NULL, NULL, NULL,
},		list2[] = {		/* rest of the list */
	"cat2", "0", "man2", "2", "2nd",
	"cat3", "0", "man3", "3", "3rd",
	"cat4", "0", "man4", "4", "4th",
	"cat5", "0", "man5", "5", "5th",
	"cat7", "0", "man7", "7", "7th",
	"cat3f", "0", "man3f", "3", "3rd (F)",
	"cat3", "0", "man3", "3X11", "X Window System",
	"cat3", "0", "man3", "3Xt", "X Window System",
	"cat3", "0", "man3", "3Xm", "X Window System (motif)",
	NULL, NULL, NULL, NULL, NULL,
},		list3[2];		/* single section */

static man();
static manual();
static add();
static DIR *getsect();
static jump();
static usage();

int debug = 0;

main(argc, argv)
	int argc;
	register char **argv;
{
	extern char *optarg;
	extern int optind;
	int ch;
	char *getenv(), *malloc();

	while ((ch = getopt(argc, argv, "-M:P:adrzfkw")) != EOF)
		switch((char)ch) {
		case '-':
			how |= CAT;
			break;
		case 'M':
		case 'P':		/* backward compatibility */
			defpath = optarg;
			break;
		case 'a':
			how |= ALL;
			break;
		case 'd':
			debug++;
			break;
		case 'r':
			how |= ROFF;
			break;
		case 'z':
			how |= Z;
			break;
		/*
		 * "man -f" and "man -k" are backward contemptible,
		 * undocumented ways of calling whatis(1) and apropos(1).
		 */
		case 'f':
			jump(argv, "-f", "whatis");
			/*NOTREACHED*/
		case 'k':
			jump(argv, "-k", "apropos");
			/*NOTREACHED*/
		/*
		 * Deliberately undocumented; really only useful when
		 * you're moving man pages around.  Not worth adding.
		 */
		case 'w':
			how |= WHERE | ALL;
			break;
		case '?':
		default:
			usage();
		}
	argv += optind;

	if (!*argv)
		usage();

	if (!(how & CAT))
		if (!isatty(1))
			how |= CAT;
		else if (pager = getenv("PAGER")) {
			register char *p;

			/*
			 * if the user uses "more", we make it "more -s"
			 * watch out for PAGER = "mypager /usr/ucb/more"
			 */
			for (p = pager; *p && !isspace(*p); ++p);
			for (; p > pager && *p != '/'; --p);
			if (p != pager)
				++p;
			/* make sure it's "more", not "morex" */
			if (!strncmp(p, "more", 4) && (!p[4] || isspace(p[4]))){
				char *opager = pager;
				/*
				 * allocate space to add the "-s"
				 */
				if (!(pager = malloc((u_int)(strlen(opager) 
				    + sizeof("-s") + 1)))) {
					fputs("man: out of space.\n", stderr);
					exit(1);
				}
				(void)sprintf(pager, "%s %s", opager, "-s");
			}
		}
		else
			pager = DEF_PAGER;
	if (!(machine = getenv("MACHINE")))
		machine = MACHINE;
	if (!defpath && !(defpath = getenv("MANPATH")))
		defpath = DEF_PATH;
	locpath = LOCAL_PATH;
	if (!(manroff = getenv("MANROFF")))
		manroff = MANROFF;
	page_size = getpagesize();
	command_size = page_size;
	command = malloc(command_size);
	if (!command) {
		fputs("man: out of space.\n", stderr);
		exit(1);
	}
	*command = 0;
	man(argv);
	if (debug) {
		puts(command);
		exit(0);
	}
	/* use system(3) in case someone's pager is "pager arg1 arg2" */
	if (command)
		(void)system(command);
	exit(0);
}

static
man(argv)
	char **argv;
{
	register char *p;
	DIR *section, *getsect();
	int res;

	for (; *argv; ++argv) {
		manpath = defpath;
		section = NULL;
		switch(**argv) {
		case 'l':				/* local */
			/* support the "{l,local,n,new}###"  syntax */
			for (p = *argv; isalpha(*p); ++p);
			if (!strncmp(*argv, "l", p - *argv) ||
			    !strncmp(*argv, "local", p - *argv)) {
				++argv;
				manpath = locpath;
				section = getsect(p);
			}
			break;
		/*
		 * old isn't really a separate section of the manual,
		 * and its entries are all in a single directory.
		 */
		case 'o':				/* old */
			for (p = *argv; isalpha(*p); ++p);
			if (!strncmp(*argv, "o", p - *argv) ||
			    !strncmp(*argv, "old", p - *argv)) {
				++argv;
				list3[0] = list1[3];
				section = list3;
			}
			break;
		case '1': case '2': case '3': case '4':
		case '5': case '6': case '7': case '8':
		case 'n': case 'x':
			if (section = getsect(*argv))
				++argv;
		}

		if (*argv) {
			if (section)
				res = manual(section, *argv);
			else {
				res = manual(list1, *argv);
				if (!res || (how & ALL))
					res += manual(list2, *argv);
			}
			if (res || how&WHERE)
				continue;
		}

		fputs("man: ", stderr);
		if (*argv)
			fprintf(stderr, "no entry for %s in the ", *argv);
		else
			fputs("what do you want from the ", stderr);
		if (section)
			fprintf(stderr, "%s section of the ", section->msg);
		if (manpath == locpath)
			fputs("local ", stderr);
		else if (manpath == newpath)
			fputs("new ", stderr);
		if (*argv) {
			fputs("manual.\n", stderr);
			fputs("Try \"info\" from within emacs.\n", stderr);
		}
		else
			fputs("manual?\n", stderr);
		exit(1);
	}
}

/*
 * manual --
 *	given a directory list and a file name find a file that
 *	matches; check ${directory}/${dir}/{file name} and
 *	${directory}/${dir}/${machine}/${file name}.
 */
static manual(section, name)
DIR *section;
char *name;
{
  register char *beg, *end;
  register DIR *dp;
  register int res;
  char objdir[MAXPATHLEN + 1];
  char obj[MAXPATHLEN + 1];
  char zobj[MAXPATHLEN + 1];
  char src[MAXPATHLEN + 1];
  char zsrc[MAXPATHLEN + 1];
  char *found;
  char *index();
  int source_only;

  for (beg = manpath, res = 0;; beg = end + 1) {
    if (end = index(beg, ':')) *end = '\0';
    for (dp = section; dp->msg; ++dp) {
      sprintf(objdir, "%s/%s", beg, dp->obj);
      sprintf(obj, "%s/%s/%s.%s", beg, dp->obj, name, dp->objext);
      sprintf(zobj, "%s/%s/%s.%s.Z", beg, dp->obj, name, dp->objext);
      sprintf(src, "%s/%s/%s.%s", beg, dp->src, name, dp->srcext);
      sprintf(zsrc, "%s/%s/%s.%s.Z", beg, dp->src, name, dp->srcext);
      if (debug) {
        printf("trying: %s\n", obj);
        printf("trying: %s\n", zobj);
        printf("trying: %s\n", src);
        printf("trying: %s\n", zsrc);
      }
      if (access(obj, R_OK) && access(zobj, R_OK) &&
          access(src, R_OK) && access(zsrc, R_OK)) {
        sprintf(objdir, "%s/%s/%s", beg, dp->obj, machine);
        sprintf(obj, "%s/%s/%s/%s.%s", beg, dp->obj, machine, name, dp->objext);
        sprintf(zobj, "%s/%s/%s/%s.%s.Z", beg, dp->obj, machine, name, dp->objext);
        sprintf(src, "%s/%s/%s/%s.%s", beg, dp->src, machine, name, dp->srcext);
        sprintf(zsrc, "%s/%s/%s/%s.%s.Z", beg, dp->src, machine, name, dp->srcext);
        if (debug) {
          printf("trying: %s\n", obj);
          printf("trying: %s\n", zobj);
          printf("trying: %s\n", src);
          printf("trying: %s\n", zsrc);
        }
        if (access(obj, R_OK) && access(zobj, R_OK) &&
            access(src, R_OK) && access(zsrc, R_OK))
          continue;
      }
      if (how & WHERE) {
      	if (!access(obj, R_OK)) printf("man: found in %s.\n", obj);
      	if (!access(zobj, R_OK)) printf("man: found in %s.\n", zobj);
      	if (!access(src, R_OK)) printf("man: found in %s.\n", src);
      	if (!access(zsrc, R_OK)) printf("man: found in %s.\n", zsrc);
      }
      else {
        if ((how & ROFF) || (access(obj, R_OK) && access(zobj, R_OK))) {
          if (access(src, R_OK)) {
            if (access(zsrc, R_OK)) continue;
            else add("zcat %s", zsrc);
            found = zsrc;
          }
          else {
            if ((how & Z) && access(zsrc, F_OK)) {
               add("compress %s; zcat %s", src, zsrc);
               found = src;
            }
            else {
              if (access(zsrc, W_OK)) add("cat %s", src);
              else add("rm %s; compress %s; zcat %s", zsrc, src, zsrc);
            }
            found = src;
          }
          add(" | %s", manroff);
          if (!access(objdir, R_OK | W_OK | X_OK)) {
            if (access(obj, F_OK) || access(obj, W_OK)) {
              add(" > %s; ", obj);
              if ((how & Z) && access(zobj, F_OK))
                add("compress %s; zcat %s", obj, zobj);
              else {
              	if (access(zobj, W_OK)) add("cat %s", obj);
              	else add("rm %s; compress %s; zcat %s", zobj, obj, zobj);
              }
            }
            else {
              /* can't overwrite obj, use manroff output instead */
            }
          }
        }
        else {
          if (access(obj, R_OK)) {
            if (access(zobj, R_OK)) continue;
            else add("zcat %s", zobj);
            found = zobj;
          }
          else {
            if ((how & Z) && access(zobj, F_OK))
              add("compress %s; zcat %s", obj, zobj);
            else {
              if (access(zobj, W_OK)) add("cat %s", obj);
              else add("rm %s; compress %s; zcat %s", zobj, obj, zobj);
            }
            found = obj;
          }
        }
        if (how & CAT) add("; ");
        else add(" | %s; ", pager);
      	if (isatty(fileno(stdout))) {
      	  printf("man: %s\n", found);
      	  fflush(stdout);
      	}
      }
      if (!(how & ALL)) return(1);
      res = 1;
    }
    if (!end) return(res);
    *end = ':';
  }
  /*NOTREACHED*/
}

/*
 * add --
 *	add to the command list
 */
static
add(fmt, args)
	char *fmt;
	int args;
{
	FILE _strbuf;
	char *realloc();

	if ((command_size - strlen(command)) < (page_size / 2)) {
		command_size += page_size;
		command = realloc(command, command_size);
		if (!command) {
			fputs("man: out of space.\n", stderr);
			exit(1);
		}
	}
	_strbuf._flag = _IOWRT+_IOSTRG;
	_strbuf._ptr = &command[strlen(command)];
	_strbuf._cnt = 32767;
	_doprnt(fmt, &args, &_strbuf);
	*_strbuf._ptr = 0;
}

/*
 * getsect --
 *	return a point to the section structure for a particular suffix
 */
static DIR *
getsect(s)
	char *s;
{
	switch(*s++) {
	case '1':
		if (!*s)
			return(list1);
		break;
	case '2':
		if (!*s) {
			list3[0] = list2[0];
			return(list3);
		}
		break;
	/* sect. 3 requests are for either section 3, or section 3[fF]. */
	/* or X11 or Xt or Xm */
	case '3':
		if (!*s) {
			list3[0] = list2[1];
			return(list3);
		}
		else if ((*s == 'f'  || *s == 'F') && !*++s) {
			list3[0] = list2[5];
			return(list3);
		}
		else if (!strcmp(s, "X11")) {
			list3[0] = list2[6];
			return(list3);
		}
		else if (!strcmp(s, "Xt")) {
			list3[0] = list2[7];
			return(list3);
		}
		else if (!strcmp(s, "Xm")) {
			list3[0] = list2[7];
			return(list3);
		}
		break;
	case '4':
		if (!*s) {
			list3[0] = list2[2];
			return(list3);
		}
		break;
	case 'n':
		if (!*s) {
			list3[0] = list1[4];
			return(list3);
		}
		break;
	case 'x':
		if (!*s) {
			list3[0] = list1[5];
			return(list3);
		}
		break;
	case '5':
		if (!*s) {
			list3[0] = list2[3];
			return(list3);
		}
		break;
	case '6':
		if (!*s) {
			list3[0] = list1[2];
			return(list3);
		}
		break;
	case '7':
		if (!*s) {
			list3[0] = list2[4];
			return(list3);
		}
		break;
	case '8':
		if (!*s) {
			list3[0] = list1[1];
			return(list3);
		}
	}
	return((DIR *)NULL);
}

/*
 * jump --
 *	strip out flag argument and jump
 */
static
jump(argv, flag, name)
	char **argv, *name;
	register char *flag;
{
	register char **arg;

	argv[0] = name;
	for (arg = argv + 1; *arg; ++arg)
		if (!strcmp(*arg, flag))
			break;
	for (; *arg; ++arg)
		arg[0] = arg[1];
	execvp(name, argv);
	fprintf(stderr, "%s: Command not found.\n", name);
	exit(1);
}

/*
 * usage --
 *	print usage and die
 */
static
usage()
{
	fputs("usage: man [-] [-a] [-M path] [section] title ...\n", stderr);
	exit(1);
}
