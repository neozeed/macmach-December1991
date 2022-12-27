/*
 **********************************************************************
 * HISTORY
 * $Log:	tar.c,v $
 * Revision 2.4  90/03/13  19:52:26  bww
 * 	Updated usage string.
 * 	[90/03/13  19:52:00  bww]
 * 
 * Revision 2.3  89/12/15  22:12:41  bww
 * 	Ensure synchronization of "fprintf(stderr, ...); perror();"
 * 	when writing to stdout by not line buffering stderr.  Also
 * 	changed some "stderr"s to "vfile" under vflag.
 * 	[89/12/15  22:12:12  bww]
 * 
 * Revision 2.2  89/11/02  19:37:40  bww
 * 	Moved CMU fixes and stripping options into /bin version.
 * 	[89/11/02  19:37:11  bww]
 * 
 * Revision 1.5  89/05/19  12:54:09  gm0w
 * 	Changed "list except_list" to "struct list except_list".
 * 	[89/05/19            gm0w]
 * 
 * Revision 1.4  89/05/13  15:14:51  gm0w
 * 	Updated to 4.3 tahoe.  Changed CMU strip prefix function 's' to 'S'
 * 	since tahoe has an 's' function that just strips of the leading '/'
 * 	and therefore doesn't take an argument like the CMU version.  Sigh.
 * 	Added code to read entire directory into memory instead of using
 * 	a readdir/telldir/seekdir loop.
 * 	[89/05/13            gm0w]
 * 
 * 05-Jun-87  Glenn Marcy (gm0w) at Carnegie-Mellon University
 *	Fixed bug in dodirtimes() that caused a race through two empty
 *	buffers (and some of the address space beyond) because the only
 *	terminating condition was the pointers being non-equal.
 *
 * 21-Dec-86  Rudy Nedved (ern) at Carnegie-Mellon University
 *	If tar does not read the same amount as it writes then it
 *	will cause a broken pipe signal if to tars are connected
 *	via pipe. If it reads more then it writes it may jump a
 *	save set. Therefore, for standard input only, try to read
 *	as much as we expect to get....namely eat the second
 *	zero filled block.
 *
 * 02-Nov-85  Glenn Marcy (gm0w) at Carnegie-Mellon University
 *	Fixed previous edit.  The code to give directories the correct
 *	permissions was merged into the wrong routine.  Also added code
 *	to tar special device files.
 *
 * 10-Jun-85  Steven Shafer (sas) at Carnegie-Mellon University
 *	Modified for CS.  Installed changes from 4.1 version due to
 *	Glenn Marcy, Mike Accetta, Eric Patterson, Steve Shafer:
 *	creates intermediate directories with appropriate search permission
 *	write permission, added -s and -k options to strip prefixes from
 *	files, added -e option to specify files/directories to be omitted
 *	("excepted").
 *
 **********************************************************************
 */
/*
 * Copyright (c) 1980 Regents of the University of California.
 * All rights reserved.  The Berkeley software License Agreement
 * specifies the terms and conditions for redistribution.
 */

#ifndef lint
char copyright[] =
"@(#) Copyright (c) 1980 Regents of the University of California.\n\
 All rights reserved.\n";
#endif /* not lint */

#ifndef lint
static char sccsid[] = "@(#)tar.c	5.12 (Berkeley) 5/23/88";
#endif /* not lint */

/*
 * Tape Archival Program
 */
#include <stdio.h>
#include <sys/param.h>
#include <sys/stat.h>
#include <sys/dir.h>
#include <sys/ioctl.h>
#include <sys/mtio.h>
#include <sys/time.h>
#include <signal.h>
#include <errno.h>
#include <fcntl.h>

#define TBLOCK	512
#define NBLOCK	20
#define NAMSIZ	100

#define	writetape(b)	writetbuf(b, 1)
#define	min(a,b)  ((a) < (b) ? (a) : (b))
#define	max(a,b)  ((a) > (b) ? (a) : (b))

union hblock {
	char dummy[TBLOCK];
	struct header {
		char name[NAMSIZ];
		char mode[8];
		char uid[8];
		char gid[8];
		char size[12];
		char mtime[12];
		char chksum[8];
		char linkflag;
		char linkname[NAMSIZ];
	} dbuf;
};

struct linkbuf {
	ino_t	inum;
	dev_t	devnum;
	int	count;
	char	pathname[NAMSIZ];
	struct	linkbuf *nextp;
};

#if	CMU
struct elem {
	struct elem *next;
	char *data;
};

struct list {
	struct elem *first;
	struct elem *last;
};

struct list except_list;
int	Sflag;
int	kflag;
int	writingdir;
char	*stripstring;
#endif	/* CMU */

union	hblock dblock;
union	hblock *tbuf;
struct	linkbuf *ihead;
struct	stat stbuf;

int	rflag;
int	sflag;
int	xflag;
int	vflag;
int	tflag;
int	cflag;
int	mflag;
int	fflag;
int	iflag;
int	oflag;
int	pflag;
int	wflag;
int	hflag;
int	Bflag;
int	Fflag;

int	mt;
int	term;
int	chksum;
int	recno;
int	first;
int	prtlinkerr;
int	freemem = 1;
int	nblock = 0;
int	onintr();
int	onquit();
int	onhup();
#ifdef notdef
int	onterm();
#endif

daddr_t	low;
daddr_t	high;
daddr_t	bsrch();

FILE	*vfile = stdout;
FILE	*tfile;
char	tname[] = "/tmp/tarXXXXXX";
char	*usefile;
char	magtape[] = "/dev/rmt8";
char	*malloc();
long	time();
off_t	lseek();
char	*mktemp();
char	*strcat();
char	*strcpy();
char	*rindex();
char	*getcwd();
char	*getwd();
char	*getmem();

main(argc, argv)
int	argc;
char	*argv[];
{
	char *cp;

	if (argc < 2)
		usage();

#if	CMU
	list_empty(&except_list);
#endif	/* CMU */
	tfile = NULL;
	usefile =  magtape;
	argv[argc] = 0;
	argv++;
	for (cp = *argv++; *cp; cp++) 
		switch(*cp) {

		case 'f':
			if (*argv == 0) {
				fprintf(stderr,
			"tar: tapefile must be specified with 'f' option\n");
				usage();
			}
			usefile = *argv++;
			fflag++;
			break;

		case 'c':
			cflag++;
			rflag++;
			break;

		case 'o':
			oflag++;
			break;

		case 'p':
			pflag++;
			break;
		
		case 'u':
			mktemp(tname);
			if ((tfile = fopen(tname, "w")) == NULL) {
				fprintf(stderr,
				 "tar: cannot create temporary file (%s)\n",
				 tname);
				done(1);
			}
			fprintf(tfile, "!!!!!/!/!/!/!/!/!/! 000\n");
			/*FALL THRU*/

		case 'r':
			rflag++;
			break;

		case 's':
			sflag++;
			break;

		case 'v':
			vflag++;
			break;

		case 'w':
			wflag++;
			break;

		case 'x':
			xflag++;
			break;

		case 't':
			tflag++;
			break;

		case 'm':
			mflag++;
			break;

		case '-':
			break;

		case '0':
		case '1':
		case '4':
		case '5':
		case '7':
		case '8':
			magtape[8] = *cp;
			usefile = magtape;
			break;

		case 'b':
			if (*argv == 0) {
				fprintf(stderr,
			"tar: blocksize must be specified with 'b' option\n");
				usage();
			}
			nblock = atoi(*argv);
			if (nblock <= 0) {
				fprintf(stderr,
				    "tar: invalid blocksize \"%s\"\n", *argv);
				done(1);
			}
			argv++;
			break;

		case 'l':
			prtlinkerr++;
			break;

		case 'h':
			hflag++;
			break;

		case 'i':
			iflag++;
			break;

		case 'B':
			Bflag++;
			break;

		case 'F':
			Fflag++;
			break;

#if	CMU
		case 'S':
			if (*argv == 0) {
				fprintf(stderr,
			"tar: prefix must be specified with 'S' option\n");
				usage();
			}
			stripstring = *argv++;
			Sflag++;
			break;

		case 'k':
			Sflag++;
			kflag++;
			break;
#endif	/* CMU */
		default:
			fprintf(stderr, "tar: %c: unknown option\n", *cp);
			usage();
		}

	if (!rflag && !xflag && !tflag)
		usage();
	if (rflag) {
		if (cflag && tfile != NULL)
			usage();
		if (signal(SIGINT, SIG_IGN) != SIG_IGN)
			(void) signal(SIGINT, onintr);
		if (signal(SIGHUP, SIG_IGN) != SIG_IGN)
			(void) signal(SIGHUP, onhup);
		if (signal(SIGQUIT, SIG_IGN) != SIG_IGN)
			(void) signal(SIGQUIT, onquit);
#ifdef notdef
		if (signal(SIGTERM, SIG_IGN) != SIG_IGN)
			(void) signal(SIGTERM, onterm);
#endif
		mt = openmt(usefile, 1);
		dorep(argv);
		done(0);
	}
	mt = openmt(usefile, 0);
	if (xflag)
		doxtract(argv);
	else
		dotable(argv);
#if	CMU
	/* read second null block if standard input */
	if (strcmp(usefile,"-") == 0)
		readtape((char *)&dblock);
#endif	/* CMU */
	done(0);
}

usage()
{
	fprintf(stderr,
#if	CMU
"tar: usage: tar -{txru}[cvfFblmhopwsBiSk] [tapefile] [blocksize] [prefix] file1 file2...\n");
#else	/* CMU */
"tar: usage: tar -{txru}[cvfFblmhopwsBi] [tapefile] [blocksize] file1 file2...\n");
#endif	/* CMU */
	done(1);
}

int
openmt(tape, writing)
	char *tape;
	int writing;
{

	if (strcmp(tape, "-") == 0) {
		/*
		 * Read from standard input or write to standard output.
		 */
		if (writing) {
			if (cflag == 0) {
				fprintf(stderr,
			 "tar: can only create standard output archives\n");
				done(1);
			}
			vfile = fdopen(dup(fileno(stderr)), "w");
			if (vfile == NULL)
				vfile = stderr;
			else
				setlinebuf(vfile);
			mt = dup(1);
		} else {
			mt = dup(0);
			Bflag++;
		}
	} else {
		/*
		 * Use file or tape on local machine.
		 */
		if (writing) {
			if (cflag)
				mt = open(tape, O_RDWR|O_CREAT|O_TRUNC, 0666);
			else
				mt = open(tape, O_RDWR);
		} else
			mt = open(tape, O_RDONLY);
		if (mt < 0) {
			fprintf(stderr, "tar: ");
			perror(tape);
			done(1);
		}
	}
	return(mt);
}

dorep(argv)
	char *argv[];
{
	register char *cp, *cp2;
	char wdir[MAXPATHLEN], tempdir[MAXPATHLEN], *parent;

	if (!cflag) {
		getdir();
		do {
			passtape();
			if (term)
				done(0);
			getdir();
		} while (!endtape());
		backtape();
		if (tfile != NULL) {
			char buf[200];

			(void)sprintf(buf,
"sort +0 -1 +1nr %s -o %s; awk '$1 != prev {print; prev=$1}' %s >%sX; mv %sX %s",
				tname, tname, tname, tname, tname, tname);
			fflush(tfile);
			system(buf);
			freopen(tname, "r", tfile);
			fstat(fileno(tfile), &stbuf);
			high = stbuf.st_size;
		}
	}

	(void) getcwd(wdir);
	while (*argv && ! term) {
		cp2 = *argv;
#if	CMU
		if (strcmp(cp2,"-e") == 0) {
			argv++;
			if (*argv) {
				list_append(&except_list, *argv);
				argv++;
			}
			continue;
		}
#endif	/* CMU */
		if (!strcmp(cp2, "-C") && argv[1]) {
			argv++;
			if (chdir(*argv) < 0) {
				fprintf(stderr, "tar: can't change directories to ");
				perror(*argv);
			} else
				(void) getcwd(wdir);
			argv++;
			continue;
		}
		parent = wdir;
		for (cp = *argv; *cp; cp++)
			if (*cp == '/')
				cp2 = cp;
		if (cp2 != *argv) {
			*cp2 = '\0';
			if (chdir(*argv) < 0) {
				fprintf(stderr, "tar: can't change directories to ");
				perror(*argv);
				continue;
			}
			parent = getcwd(tempdir);
			*cp2 = '/';
			cp2++;
		}
		putfile(*argv++, cp2, parent);
		if (chdir(wdir) < 0) {
			fprintf(stderr, "tar: cannot change back?: ");
			perror(wdir);
		}
	}
	putempty();
	putempty();
	flushtape();
	if (prtlinkerr == 0)
		return;
	for (; ihead != NULL; ihead = ihead->nextp) {
		if (ihead->count == 0)
			continue;
		fprintf(stderr, "tar: missing links to %s\n", ihead->pathname);
	}
}

endtape()
{
	return (dblock.dbuf.name[0] == '\0');
}

getdir()
{
	register struct stat *sp;
	int i;

top:
	readtape((char *)&dblock);
	if (dblock.dbuf.name[0] == '\0')
		return;
	sp = &stbuf;
	sscanf(dblock.dbuf.mode, "%o", &i);
	sp->st_mode = i;
	sscanf(dblock.dbuf.uid, "%o", &i);
	sp->st_uid = i;
	sscanf(dblock.dbuf.gid, "%o", &i);
	sp->st_gid = i;
	sscanf(dblock.dbuf.size, "%lo", &sp->st_size);
	sscanf(dblock.dbuf.mtime, "%lo", &sp->st_mtime);
	sscanf(dblock.dbuf.chksum, "%o", &chksum);
	if (chksum != (i = checksum())) {
		fprintf(stderr, "tar: directory checksum error (%d != %d)\n",
		    chksum, i);
		if (iflag)
			goto top;
		done(2);
	}
	/* strip off leading "/" if present */
	if (sflag && dblock.dbuf.name[0] == '/') {
		register char *cp1, *cp2;
		for (cp1 = cp2 = dblock.dbuf.name; *cp2 && *cp2 == '/'; ++cp2);
		if (!*cp2)
			goto top;
		while (*cp1++ = *cp2++);
	}
	if (tfile != NULL)
		fprintf(tfile, "%s %s\n", dblock.dbuf.name, dblock.dbuf.mtime);
}

passtape()
{
	long blocks;
	char *bufp;

	if (dblock.dbuf.linkflag == '1')
		return;
	blocks = stbuf.st_size;
	blocks += TBLOCK-1;
	blocks /= TBLOCK;

	while (blocks-- > 0)
		(void) readtbuf(&bufp, TBLOCK);
}

#if	CMU
list_empty(listp)
struct list *listp;
{
	listp->first = listp->last = NULL;
}

list_append(listp, data)
struct list *listp;
char *data;
{
	struct elem *ep;
	int len;

	ep = (struct elem *) calloc(1, sizeof(struct elem));
	if (ep == NULL) {
		fprintf(stderr, "tar: out of memory\n");
		done(5);
	}
	ep->data = malloc(len = (strlen(data) + 1));
	bcopy(data, ep->data, len);
	if (listp->first == NULL)
		listp->first = ep;
	else
		listp->last->next = ep;
	listp->last = ep;
}

char *
list_first(listp)
struct list *listp;
{
	struct elem *ep;
	char *data;

	if ((ep = listp->first) == NULL)
		return(NULL);
	listp->first = ep->next;
	data = ep->data;
	free((char *)ep);
	return(data);
}

int
excepted(data)
char *data;
{
	struct elem *ep;

	for (ep = except_list.first; ep != NULL; ep = ep->next)
		if (strcmp(ep->data, data) == 0)
			break;
	return(ep != NULL);
}

char *
stripprefix(filename)
register char *filename;
{
	register char *ptr;
	register int len;
	
	if (kflag)  {
		if ((ptr = rindex(filename, '/')) == NULL)
			return(filename);
		else
			return(++ptr);
	}
	len = strlen(stripstring);
	if (strncmp(filename, stripstring, len))
		return(filename);
	else
		return(filename+len);
}
#endif	/* CMU */

putfile(longname, shortname, parent)
	char *longname;
	char *shortname;
	char *parent;
{
	int infile = 0;
	long blocks;
	char buf[TBLOCK];
	char *bigbuf;
	register char *cp;
	struct direct *dp;
	DIR *dirp;
	register int i;
	long l;
	char newparent[NAMSIZ+64];
	int	maxread;
	int	hint;		/* amount to write to get "in sync" */
#if	CMU
	struct list list;
	char *elem;
#endif	/* CMU */

#if	CMU
	if (excepted(longname)) {
		if (vflag)
			fprintf(vfile,"except %s\n",longname);
		return;
	}
	if ((Sflag) && (!writingdir))
		longname = stripprefix(longname);
#endif	/* CMU */
	if (!hflag)
		i = lstat(shortname, &stbuf);
	else
		i = stat(shortname, &stbuf);
	if (i < 0) {
		fprintf(stderr, "tar: ");
		perror(longname);
		return;
	}
	if (tfile != NULL && checkupdate(longname) == 0)
		return;
	if (checkw('r', longname) == 0)
		return;
	if (Fflag && checkf(shortname, stbuf.st_mode, Fflag) == 0)
		return;

	switch (stbuf.st_mode & S_IFMT) {
	case S_IFDIR:
		for (i = 0, cp = buf; *cp++ = longname[i++];)
			;
		*--cp = '/';
		*++cp = 0  ;
		if (!oflag) {
			if ((cp - buf) >= NAMSIZ) {
				fprintf(stderr, "tar: %s: file name too long\n",
				    longname);
				return;
			}
			stbuf.st_size = 0;
			tomodes(&stbuf);
			strcpy(dblock.dbuf.name,buf);
			(void)sprintf(dblock.dbuf.chksum, "%6o", checksum());
			(void) writetape((char *)&dblock);
		}
		(void)sprintf(newparent, "%s/%s", parent, shortname);
		if (chdir(shortname) < 0) {
			perror(shortname);
			return;
		}
		if ((dirp = opendir(".")) == NULL) {
			fprintf(stderr, "tar: %s: directory read error\n",
			    longname);
			if (chdir(parent) < 0) {
				fprintf(stderr, "tar: cannot change back?: ");
				perror(parent);
			}
			return;
		}
#if	CMU
		list_empty(&list);
#endif	/* CMU */
		while ((dp = readdir(dirp)) != NULL && !term) {
			if (!strcmp(".", dp->d_name) ||
			    !strcmp("..", dp->d_name))
				continue;
#if	CMU
			list_append(&list, dp->d_name);
#else	/* CMU */
			strcpy(cp, dp->d_name);
			l = telldir(dirp);
			closedir(dirp);
			putfile(buf, cp, newparent);
			dirp = opendir(".");
			seekdir(dirp, l);
#endif	/* CMU */
		}
		closedir(dirp);
#if	CMU
		while ((elem = list_first(&list)) != NULL && !term) {
			strcpy(cp, elem);
			free(elem);
			writingdir++;
			putfile(buf, cp, newparent);
		}
#endif	/* CMU */
		if (chdir(parent) < 0) {
			fprintf(stderr, "tar: cannot change back?: ");
			perror(parent);
		}
#if	CMU
		writingdir = 0;
#endif	/* CMU */
		break;

	case S_IFLNK:
		tomodes(&stbuf);
		if (strlen(longname) >= NAMSIZ) {
			fprintf(stderr, "tar: %s: file name too long\n",
			    longname);
			return;
		}
		strcpy(dblock.dbuf.name, longname);
		if (stbuf.st_size + 1 >= NAMSIZ) {
			fprintf(stderr, "tar: %s: symbolic link too long\n",
			    longname);
			return;
		}
		i = readlink(shortname, dblock.dbuf.linkname, NAMSIZ - 1);
		if (i < 0) {
			fprintf(stderr, "tar: can't read symbolic link ");
			perror(longname);
			return;
		}
		dblock.dbuf.linkname[i] = '\0';
		dblock.dbuf.linkflag = '2';
		if (vflag)
			fprintf(vfile, "a %s symbolic link to %s\n",
			    longname, dblock.dbuf.linkname);
		(void)sprintf(dblock.dbuf.size, "%11lo", 0L);
		(void)sprintf(dblock.dbuf.chksum, "%6o", checksum());
		(void) writetape((char *)&dblock);
		break;

#if	CMU
	case S_IFBLK:
	case S_IFCHR:
		tomodes(&stbuf);
		if (strlen(longname) >= NAMSIZ) {
			fprintf(stderr, "tar: %s: file name too long\n",
			    longname);
			return;
		}
		strcpy(dblock.dbuf.name, longname);
		*((unsigned *)dblock.dbuf.linkname) = stbuf.st_rdev;
		if ((stbuf.st_mode & S_IFMT) == S_IFBLK)
			dblock.dbuf.linkflag = '3';
		else
			dblock.dbuf.linkflag = '4';
		if (vflag) {
			fprintf(vfile, "a %s ", longname);
			fprintf(vfile, "%s device %d, %d\n",
				dblock.dbuf.linkflag == '3' ?
					"block" : "character",
				major(*((unsigned *)dblock.dbuf.linkname)),
				minor(*((unsigned *)dblock.dbuf.linkname)));
		}
		sprintf(dblock.dbuf.size, "%11lo", 0);
		sprintf(dblock.dbuf.chksum, "%6o", checksum());
		writetape((char *)&dblock);
		break;
#endif	/* CMU */

	case S_IFREG:
		if ((infile = open(shortname, 0)) < 0) {
			fprintf(stderr, "tar: ");
			perror(longname);
			return;
		}
		tomodes(&stbuf);
		if (strlen(longname) >= NAMSIZ) {
			fprintf(stderr, "tar: %s: file name too long\n",
			    longname);
			close(infile);
			return;
		}
		strcpy(dblock.dbuf.name, longname);
		if (stbuf.st_nlink > 1) {
			struct linkbuf *lp;
			int found = 0;

			for (lp = ihead; lp != NULL; lp = lp->nextp)
				if (lp->inum == stbuf.st_ino &&
				    lp->devnum == stbuf.st_dev) {
					found++;
					break;
				}
			if (found) {
				strcpy(dblock.dbuf.linkname, lp->pathname);
				dblock.dbuf.linkflag = '1';
				(void)sprintf(dblock.dbuf.chksum, "%6o", checksum());
				(void) writetape( (char *) &dblock);
				if (vflag)
					fprintf(vfile, "a %s link to %s\n",
					    longname, lp->pathname);
				lp->count--;
				close(infile);
				return;
			}
			lp = (struct linkbuf *) getmem(sizeof(*lp));
			if (lp != NULL) {
				lp->nextp = ihead;
				ihead = lp;
				lp->inum = stbuf.st_ino;
				lp->devnum = stbuf.st_dev;
				lp->count = stbuf.st_nlink - 1;
				strcpy(lp->pathname, longname);
			}
		}
		blocks = (stbuf.st_size + (TBLOCK-1)) / TBLOCK;
		if (vflag)
			fprintf(vfile, "a %s %ld blocks\n", longname, blocks);
		(void)sprintf(dblock.dbuf.chksum, "%6o", checksum());
		hint = writetape((char *)&dblock);
		maxread = max(stbuf.st_blksize, (nblock * TBLOCK));
		if ((bigbuf = malloc((unsigned)maxread)) == 0) {
			maxread = TBLOCK;
			bigbuf = buf;
		}

		while ((i = read(infile, bigbuf, min((hint*TBLOCK), maxread))) > 0
		  && blocks > 0) {
		  	register int nblks;

			nblks = ((i-1)/TBLOCK)+1;
		  	if (nblks > blocks)
		  		nblks = blocks;
			hint = writetbuf(bigbuf, nblks);
			blocks -= nblks;
		}
		close(infile);
		if (bigbuf != buf)
			free(bigbuf);
		if (i < 0) {
			fprintf(stderr, "tar: Read error on ");
			perror(longname);
		} else if (blocks != 0 || i != 0)
			fprintf(stderr, "tar: %s: file changed size\n",
			    longname);
		while (--blocks >=  0)
			putempty();
		break;

	default:
		fprintf(stderr, "tar: %s is not a file. Not dumped\n",
		    longname);
		break;
	}
}

doxtract(argv)
	char *argv[];
{
	extern int errno;
	long blocks, bytes;
	int ofile, i;

	for (;;) {
		if ((i = wantit(argv)) == 0)
			continue;
		if (i == -1)
			break;		/* end of tape */
#if	CMU
		if (Sflag)
			strcpy(dblock.dbuf.name,stripprefix(dblock.dbuf.name));
#endif	/* CMU */
		if (checkw('x', dblock.dbuf.name) == 0) {
			passtape();
			continue;
		}
		if (Fflag) {
			char *s;

			if ((s = rindex(dblock.dbuf.name, '/')) == 0)
				s = dblock.dbuf.name;
			else
				s++;
			if (checkf(s, stbuf.st_mode, Fflag) == 0) {
				passtape();
				continue;
			}
		}
		if (checkdir(dblock.dbuf.name)) {	/* have a directory */
			if (mflag == 0)
				dodirtimes(&dblock);
			continue;
		}
#if	CMU
		if (dblock.dbuf.linkflag == '3' ||
		    dblock.dbuf.linkflag == '4') {
			unlink(dblock.dbuf.name);
			if (mknod(dblock.dbuf.name,
				(stbuf.st_mode & 0777) |
				(dblock.dbuf.linkflag == '3'?S_IFBLK:S_IFCHR),
				*((unsigned *)dblock.dbuf.linkname)) < 0) {
				fprintf(stderr, "tar: %s: mknod failed\n",
				    dblock.dbuf.name);
				continue;
			}
			if (vflag)
				fprintf(vfile,
					"x %s %s device %d, %d\n",
					dblock.dbuf.name,
					dblock.dbuf.linkflag == '3' ?
					"block" : "character",
				major(*((unsigned *)dblock.dbuf.linkname)),
				minor(*((unsigned *)dblock.dbuf.linkname)));
			chown(dblock.dbuf.name, stbuf.st_uid, stbuf.st_gid);
			if (mflag == 0)
				setimes(dblock.dbuf.name, stbuf.st_mtime);
			if (pflag)
				chmod(dblock.dbuf.name, stbuf.st_mode & 07777);
			continue;
		}
#endif	/* CMU */
		if (dblock.dbuf.linkflag == '2') {	/* symlink */
			/*
			 * only unlink non directories or empty
			 * directories
			 */
			if (rmdir(dblock.dbuf.name) < 0) {
				if (errno == ENOTDIR)
					unlink(dblock.dbuf.name);
			}
			if (symlink(dblock.dbuf.linkname, dblock.dbuf.name)<0) {
				fprintf(stderr, "tar: %s: symbolic link failed: ",
				    dblock.dbuf.name);
				perror("");
				continue;
			}
			if (vflag)
				fprintf(vfile, "x %s symbolic link to %s\n",
				    dblock.dbuf.name, dblock.dbuf.linkname);
#ifdef notdef
			/* ignore alien orders */
			chown(dblock.dbuf.name, stbuf.st_uid, stbuf.st_gid);
			if (mflag == 0)
				setimes(dblock.dbuf.name, stbuf.st_mtime);
			if (pflag)
				chmod(dblock.dbuf.name, stbuf.st_mode & 07777);
#endif
			continue;
		}
		if (dblock.dbuf.linkflag == '1') {	/* regular link */
			/*
			 * only unlink non directories or empty
			 * directories
			 */
			if (rmdir(dblock.dbuf.name) < 0) {
				if (errno == ENOTDIR)
					unlink(dblock.dbuf.name);
			}
			if (link(dblock.dbuf.linkname, dblock.dbuf.name) < 0) {
				fprintf(stderr, "tar: can't link %s to %s: ",
				    dblock.dbuf.name, dblock.dbuf.linkname);
				perror("");
				continue;
			}
			if (vflag)
				fprintf(vfile, "%s linked to %s\n",
				    dblock.dbuf.name, dblock.dbuf.linkname);
			continue;
		}
		if ((ofile = creat(dblock.dbuf.name,stbuf.st_mode&0xfff)) < 0) {
			fprintf(stderr, "tar: can't create %s: ",
			    dblock.dbuf.name);
			perror("");
			passtape();
			continue;
		}
		chown(dblock.dbuf.name, stbuf.st_uid, stbuf.st_gid);
		blocks = ((bytes = stbuf.st_size) + TBLOCK-1)/TBLOCK;
		if (vflag)
			fprintf(vfile, "x %s, %ld bytes, %ld tape blocks\n",
			    dblock.dbuf.name, bytes, blocks);
		for (; blocks > 0;) {
			register int nread;
			char	*bufp;
			register int nwant;
			
			nwant = NBLOCK*TBLOCK;
			if (nwant > (blocks*TBLOCK))
				nwant = (blocks*TBLOCK);
			nread = readtbuf(&bufp, nwant);
			if (write(ofile, bufp, (int)min(nread, bytes)) < 0) {
				fprintf(stderr,
				    "tar: %s: HELP - extract write error: ",
				    dblock.dbuf.name);
				perror("");
				done(2);
			}
			bytes -= nread;
			blocks -= (((nread-1)/TBLOCK)+1);
		}
		close(ofile);
		if (mflag == 0)
			setimes(dblock.dbuf.name, stbuf.st_mtime);
		if (pflag)
			chmod(dblock.dbuf.name, stbuf.st_mode & 07777);
	}
	if (mflag == 0) {
		dblock.dbuf.name[0] = '\0';	/* process the whole stack */
		dodirtimes(&dblock);
	}
}

dotable(argv)
	char *argv[];
{
	register int i;

	for (;;) {
		if ((i = wantit(argv)) == 0)
			continue;
		if (i == -1)
			break;		/* end of tape */
		if (vflag)
			longt(&stbuf);
		printf("%s", dblock.dbuf.name);
		if (dblock.dbuf.linkflag == '1')
			printf(" linked to %s", dblock.dbuf.linkname);
		if (dblock.dbuf.linkflag == '2')
			printf(" symbolic link to %s", dblock.dbuf.linkname);
#if	CMU
		if (dblock.dbuf.linkflag == '3')
			printf(" block device %d, %d",
				major(*((unsigned *)dblock.dbuf.linkname)),
				minor(*((unsigned *)dblock.dbuf.linkname)));
		if (dblock.dbuf.linkflag == '4')
			printf(" character device %d, %d",
				major(*((unsigned *)dblock.dbuf.linkname)),
				minor(*((unsigned *)dblock.dbuf.linkname)));
#endif	/* CMU */
		printf("\n");
		passtape();
	}
}

putempty()
{
	char buf[TBLOCK];

	bzero(buf, sizeof (buf));
	(void) writetape(buf);
}

longt(st)
	register struct stat *st;
{
	register char *cp;
	char *ctime();

	pmode(st);
	printf("%3d/%1d", st->st_uid, st->st_gid);
	printf("%7ld", st->st_size);
	cp = ctime(&st->st_mtime);
	printf(" %-12.12s %-4.4s ", cp+4, cp+20);
}

#define	SUID	04000
#define	SGID	02000
#define	ROWN	0400
#define	WOWN	0200
#define	XOWN	0100
#define	RGRP	040
#define	WGRP	020
#define	XGRP	010
#define	ROTH	04
#define	WOTH	02
#define	XOTH	01
#define	STXT	01000
int	m1[] = { 1, ROWN, 'r', '-' };
int	m2[] = { 1, WOWN, 'w', '-' };
int	m3[] = { 2, SUID, 's', XOWN, 'x', '-' };
int	m4[] = { 1, RGRP, 'r', '-' };
int	m5[] = { 1, WGRP, 'w', '-' };
int	m6[] = { 2, SGID, 's', XGRP, 'x', '-' };
int	m7[] = { 1, ROTH, 'r', '-' };
int	m8[] = { 1, WOTH, 'w', '-' };
int	m9[] = { 2, STXT, 't', XOTH, 'x', '-' };

int	*m[] = { m1, m2, m3, m4, m5, m6, m7, m8, m9};

pmode(st)
	register struct stat *st;
{
	register int **mp;

	for (mp = &m[0]; mp < &m[9];)
		selectbits(*mp++, st);
}

selectbits(pairp, st)
	int *pairp;
	struct stat *st;
{
	register int n, *ap;

	ap = pairp;
	n = *ap++;
	while (--n>=0 && (st->st_mode&*ap++)==0)
		ap++;
	putchar(*ap);
}

/*
 * Make all directories needed by `name'.  If `name' is itself
 * a directory on the tar tape (indicated by a trailing '/'),
 * return 1; else 0.
 */
checkdir(name)
	register char *name;
{
	register char *cp;

	/*
	 * Quick check for existence of directory.
	 */
	if ((cp = rindex(name, '/')) == 0)
		return (0);
	*cp = '\0';
	if (access(name, 0) == 0) {	/* already exists */
		*cp = '/';
		return (cp[1] == '\0');	/* return (lastchar == '/') */
	}
	*cp = '/';

	/*
	 * No luck, try to make all directories in path.
	 */
	for (cp = name; *cp; cp++) {
		if (*cp != '/')
			continue;
		*cp = '\0';
		if (access(name, 0) < 0) {
			if (mkdir(name, 0777) < 0) {
				perror(name);
				*cp = '/';
				return (0);
			}
			chown(name, stbuf.st_uid, stbuf.st_gid);
#if	CMU
			if (pflag) {
				int mask = (S_IWRITE | S_IREAD);
				int mode = (stbuf.st_mode & 07777) | S_IWRITE;
				int exec = S_IEXEC;
				/* Make sure the created directory has
				 * search permission for each access level
				 * that has any permission in the file
				 * for which the directory is being created.
				 * Since we created this directory in order
				 * to create something within it, make sure
				 * that it at least has owner write permission
				 * so that the create will not fail.
				 */
				while (mask) {
					if (mode & mask) mode |= exec;
					mask >>= 3;
					exec >>= 3;
				}
				chmod (dblock.dbuf.name,mode);
			}
#else	/* CMU */
			if (pflag && cp[1] == '\0')	/* dir on the tape */
				chmod(name, stbuf.st_mode & 07777);
#endif	/* CMU */
		}
		*cp = '/';
	}
	return (cp[-1]=='/');
}

onintr()
{
	(void) signal(SIGINT, SIG_IGN);
	term++;
}

onquit()
{
	(void) signal(SIGQUIT, SIG_IGN);
	term++;
}

onhup()
{
	(void) signal(SIGHUP, SIG_IGN);
	term++;
}

#ifdef notdef
onterm()
{
	(void) signal(SIGTERM, SIG_IGN);
	term++;
}
#endif

tomodes(sp)
register struct stat *sp;
{
	register char *cp;

	for (cp = dblock.dummy; cp < &dblock.dummy[TBLOCK]; cp++)
		*cp = '\0';
	(void)sprintf(dblock.dbuf.mode, "%6o ", sp->st_mode & 07777);
	(void)sprintf(dblock.dbuf.uid, "%6o ", sp->st_uid);
	(void)sprintf(dblock.dbuf.gid, "%6o ", sp->st_gid);
	(void)sprintf(dblock.dbuf.size, "%11lo ", sp->st_size);
	(void)sprintf(dblock.dbuf.mtime, "%11lo ", sp->st_mtime);
}

checksum()
{
	register i;
	register char *cp;

	for (cp = dblock.dbuf.chksum;
	     cp < &dblock.dbuf.chksum[sizeof(dblock.dbuf.chksum)]; cp++)
		*cp = ' ';
	i = 0;
	for (cp = dblock.dummy; cp < &dblock.dummy[TBLOCK]; cp++)
		i += *cp;
	return (i);
}

checkw(c, name)
	char *name;
{
	if (!wflag)
		return (1);
	printf("%c ", c);
	if (vflag)
		longt(&stbuf);
	printf("%s: ", name);
	return (response() == 'y');
}

response()
{
	char c;

	c = getchar();
	if (c != '\n')
		while (getchar() != '\n')
			;
	else
		c = 'n';
	return (c);
}

checkf(name, mode, howmuch)
	char *name;
	int mode, howmuch;
{
	int l;

	if ((mode & S_IFMT) == S_IFDIR){
		if ((strcmp(name, "SCCS")==0) || (strcmp(name, "RCS")==0)) 
			return(0); 
		return(1);
	}
	if ((l = strlen(name)) < 3)
		return (1);
	if (howmuch > 1 && name[l-2] == '.' && name[l-1] == 'o')
		return (0);
	if (strcmp(name, "core") == 0 ||
	    strcmp(name, "errs") == 0 ||
	    (howmuch > 1 && strcmp(name, "a.out") == 0))
		return (0);
	/* SHOULD CHECK IF IT IS EXECUTABLE */
	return (1);
}

/* Is the current file a new file, or the newest one of the same name? */
checkupdate(arg)
	char *arg;
{
	char name[100];
	long mtime;
	daddr_t seekp;
	daddr_t	lookup();

	rewind(tfile);
	for (;;) {
		if ((seekp = lookup(arg)) < 0)
			return (1);
		fseek(tfile, seekp, 0);
		fscanf(tfile, "%s %lo", name, &mtime);
		return (stbuf.st_mtime > mtime);
	}
}

done(n)
{
	unlink(tname);
	exit(n);
}

/* 
 * Do we want the next entry on the tape, i.e. is it selected?  If
 * not, skip over the entire entry.  Return -1 if reached end of tape.
 */
wantit(argv)
	char *argv[];
{
	register char **cp;

	getdir();
	if (endtape())
		return (-1);
	if (*argv == 0)
		return (1);
	for (cp = argv; *cp; cp++)
		if (prefix(*cp, dblock.dbuf.name))
			return (1);
	passtape();
	return (0);
}

/*
 * Does s2 begin with the string s1, on a directory boundary?
 */
prefix(s1, s2)
	register char *s1, *s2;
{
	while (*s1)
		if (*s1++ != *s2++)
			return (0);
	if (*s2)
		return (*s2 == '/');
	return (1);
}

#define	N	200
int	njab;

daddr_t
lookup(s)
	char *s;
{
	register i;
	daddr_t a;

	for(i=0; s[i]; i++)
		if (s[i] == ' ')
			break;
	a = bsrch(s, i, low, high);
	return (a);
}

daddr_t
bsrch(s, n, l, h)
	daddr_t l, h;
	char *s;
{
	register i, j;
	char b[N];
	daddr_t m, m1;

	njab = 0;

loop:
	if (l >= h)
		return ((daddr_t) -1);
	m = l + (h-l)/2 - N/2;
	if (m < l)
		m = l;
	fseek(tfile, m, 0);
	fread(b, 1, N, tfile);
	njab++;
	for(i=0; i<N; i++) {
		if (b[i] == '\n')
			break;
		m++;
	}
	if (m >= h)
		return ((daddr_t) -1);
	m1 = m;
	j = i;
	for(i++; i<N; i++) {
		m1++;
		if (b[i] == '\n')
			break;
	}
	i = cmp(b+j, s, n);
	if (i < 0) {
		h = m;
		goto loop;
	}
	if (i > 0) {
		l = m1;
		goto loop;
	}
	return (m);
}

cmp(b, s, n)
	char *b, *s;
{
	register i;

	if (b[0] != '\n')
		exit(2);
	for(i=0; i<n; i++) {
		if (b[i+1] > s[i])
			return (-1);
		if (b[i+1] < s[i])
			return (1);
	}
	return (b[i+1] == ' '? 0 : -1);
}

readtape(buffer)
	char *buffer;
{
	char *bufp;

	if (first == 0)
		getbuf();
	(void) readtbuf(&bufp, TBLOCK);
	bcopy(bufp, buffer, TBLOCK);
	return(TBLOCK);
}

readtbuf(bufpp, size)
	char **bufpp;
	int size;
{
	register int i;

	if (recno >= nblock || first == 0) {
		if ((i = bread(mt, (char *)tbuf, TBLOCK*nblock)) < 0)
			mterr("read", i, 3);
		if (first == 0) {
			if ((i % TBLOCK) != 0) {
				fprintf(stderr, "tar: tape blocksize error\n");
				done(3);
			}
			i /= TBLOCK;
			if (i != nblock) {
				fprintf(stderr, "tar: blocksize = %d\n", i);
				nblock = i;
			}
			first = 1;
		}
		recno = 0;
	}
	if (size > ((nblock-recno)*TBLOCK))
		size = (nblock-recno)*TBLOCK;
	*bufpp = (char *)&tbuf[recno];
	recno += (size/TBLOCK);
	return (size);
}

writetbuf(buffer, n)
	register char *buffer;
	register int n;
{
	int i;

	if (first == 0) {
		getbuf();
		first = 1;
	}
	if (recno >= nblock) {
		i = write(mt, (char *)tbuf, TBLOCK*nblock);
		if (i != TBLOCK*nblock)
			mterr("write", i, 2);
		recno = 0;
	}

	/*
	 *  Special case:  We have an empty tape buffer, and the
	 *  users data size is >= the tape block size:  Avoid
	 *  the bcopy and dma direct to tape.  BIG WIN.  Add the
	 *  residual to the tape buffer.
	 */
	while (recno == 0 && n >= nblock) {
		i = write(mt, buffer, TBLOCK*nblock);
		if (i != TBLOCK*nblock)
			mterr("write", i, 2);
		n -= nblock;
		buffer += (nblock * TBLOCK);
	}
		
	while (n-- > 0) {
		bcopy(buffer, (char *)&tbuf[recno++], TBLOCK);
		buffer += TBLOCK;
		if (recno >= nblock) {
			i = write(mt, (char *)tbuf, TBLOCK*nblock);
			if (i != TBLOCK*nblock)
				mterr("write", i, 2);
			recno = 0;
		}
	}

	/* Tell the user how much to write to get in sync */
	return (nblock - recno);
}

backtape()
{
	static int mtdev = 1;
	static struct mtop mtop = {MTBSR, 1};
	struct mtget mtget;
	
	if (mtdev == 1)
		mtdev = ioctl(mt, MTIOCGET, (char *)&mtget);
	if (mtdev == 0) {
		if (ioctl(mt, MTIOCTOP, (char *)&mtop) < 0) {
			fprintf(stderr, "tar: tape backspace error: ");
			perror("");
			done(4);
		}
	} else
		lseek(mt, (daddr_t) -TBLOCK*nblock, 1);
	recno--;
}

flushtape()
{
	int i;

	i = write(mt, (char *)tbuf, TBLOCK*nblock);
	if (i != TBLOCK*nblock)
		mterr("write", i, 2);
}

mterr(operation, i, exitcode)
	char *operation;
	int i;
{
	fprintf(stderr, "tar: tape %s error: ", operation);
	if (i < 0)
		perror("");
	else
		fprintf(stderr, "unexpected EOF\n");
	done(exitcode);
}

bread(fd, buf, size)
	int fd;
	char *buf;
	int size;
{
	int count;
	static int lastread = 0;

	if (!Bflag)
		return (read(fd, buf, size)); 

	for (count = 0; count < size; count += lastread) {
		lastread = read(fd, buf, size - count);
		if (lastread <= 0) {
			if (count > 0)
				return (count);
			return (lastread);
		}
		buf += lastread;
	}
	return (count);
}

char *
getcwd(buf)
	char *buf;
{
	if (getwd(buf) == NULL) {
		fprintf(stderr, "tar: %s\n", buf);
		exit(1);
	}
	return (buf);
}

getbuf()
{
	
	if (nblock == 0) {
		fstat(mt, &stbuf);
		if ((stbuf.st_mode & S_IFMT) == S_IFCHR)
			nblock = NBLOCK;
		else {
			nblock = stbuf.st_blksize / TBLOCK;
			if (nblock == 0)
				nblock = NBLOCK;
		}
	}
	tbuf = (union hblock *)malloc((unsigned)nblock*TBLOCK);
	if (tbuf == NULL) {
		fprintf(stderr, "tar: blocksize %d too big, can't get memory\n",
		    nblock);
		done(1);
	}
}

/*
 * Save this directory and its mtime on the stack, popping and setting
 * the mtimes of any stacked dirs which aren't parents of this one.
 * A null directory causes the entire stack to be unwound and set.
 *
 * Since all the elements of the directory "stack" share a common
 * prefix, we can make do with one string.  We keep only the current
 * directory path, with an associated array of mtime's, one for each
 * '/' in the path.  A negative mtime means no mtime.  The mtime's are
 * offset by one (first index 1, not 0) because calling this with a null
 * directory causes mtime[0] to be set.
 * 
 * This stack algorithm is not guaranteed to work for tapes created
 * with the 'r' option, but the vast majority of tapes with
 * directories are not.  This avoids saving every directory record on
 * the tape and setting all the times at the end.
 */
char dirstack[NAMSIZ];
#define NTIM (NAMSIZ/2+1)		/* a/b/c/d/... */
time_t mtime[NTIM];

dodirtimes(hp)
	union hblock *hp;
{
	register char *p = dirstack;
	register char *q = hp->dbuf.name;
	register int ndir = 0;
	char *savp;
	int savndir;

	/* Find common prefix */
	while (*p == *q && *p) {
		if (*p++ == '/')
			++ndir;
		q++;
	}

	savp = p;
	savndir = ndir;
	while (*p) {
		/*
		 * Not a child: unwind the stack, setting the times.
		 * The order we do this doesn't matter, so we go "forward."
		 */
		if (*p++ == '/')
			if (mtime[++ndir] >= 0) {
				*--p = '\0';	/* zap the slash */
				setimes(dirstack, mtime[ndir]);
				*p++ = '/';
			}
	}
	p = savp;
	ndir = savndir;

	/* Push this one on the "stack" */
	while (*p = *q++)	/* append the rest of the new dir */
		if (*p++ == '/')
			mtime[++ndir] = -1;
	mtime[ndir] = stbuf.st_mtime;	/* overwrite the last one */
}

setimes(path, mt)
	char *path;
	time_t mt;
{
	struct timeval tv[2];

	tv[0].tv_sec = time((time_t *) 0);
	tv[1].tv_sec = mt;
	tv[0].tv_usec = tv[1].tv_usec = 0;
	if (utimes(path, tv) < 0) {
		fprintf(stderr, "tar: can't set time on %s: ", path);
		perror("");
	}
}

char *
getmem(size)
{
	char *p = malloc((unsigned) size);

	if (p == NULL && freemem) {
		fprintf(stderr,
		    "tar: out of memory, link and directory modtime info lost\n");
		freemem = 0;
	}
	return (p);
}
