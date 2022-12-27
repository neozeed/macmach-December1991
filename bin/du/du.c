/*
 **********************************************************************
 * HISTORY
 * $Log:	du.c,v $
 * Revision 2.2  89/11/03  12:47:37  scherrer
 * 	Added code to read entire directory contents into memory instead of
 * 	using readdir/telldir/seekdir loop.
 * 	[89/11/03  12:46:55  scherrer]
 * 
 * Revision 1.2  89/05/13  15:55:02  gm0w
 * 	Added code to read entire directory contents into memory instead of
 * 	using readdir/telldir/seekdir loop.
 * 	[89/05/13            gm0w]
 * 
 **********************************************************************
 */
#ifndef lint
static char *sccsid = "@(#)du.c       4.12 (Berkeley) 10/22/87";
#endif

#include <stdio.h>
#include <sys/param.h>
#include <sys/stat.h>
#include <sys/dir.h>

char	path[BUFSIZ], name[BUFSIZ];
int	aflg;
int	sflg;
char	*dot = ".";

#define ML	1000
struct {
	int	dev;
	ino_t	ino;
} ml[ML];
int	mlx;

long	descend();
char	*index(), *rindex(), *strcpy();

#define	kb(n)	(howmany(dbtob(n), 1024))

main(argc, argv)
	int argc;
	char **argv;
{
	long blocks = 0;
	register char *np;
	int pid;

	argc--, argv++;
again:
	if (argc && !strcmp(*argv, "-s")) {
		sflg++;
		argc--, argv++;
		goto again;
	}
	if (argc && !strcmp(*argv, "-a")) {
		aflg++;
		argc--, argv++;
		goto again;
	}
	if (argc == 0) {
		argv = &dot;
		argc = 1;
	}
	do {
		if (argc > 1) {
			pid = fork();
			if (pid == -1) {
				fprintf(stderr, "No more processes.\n");
				exit(1);
			}
			if (pid != 0)
				wait((int *)0);
		}
		if (argc == 1 || pid == 0) {
			(void) strcpy(path, *argv);
			(void) strcpy(name, *argv);
			if (np = rindex(name, '/')) {
				*np++ = '\0';
				if (chdir(*name ? name : "/") < 0) {
					perror(*name ? name : "/");
					exit(1);
				}
			} else
				np = path;
			blocks = descend(path, *np ? np : ".");
			if (sflg)
				printf("%ld\t%s\n", kb(blocks), path);
			if (argc > 1)
				exit(1);
		}
		argc--, argv++;
	} while (argc > 0);
	exit(0);
}

DIR	*dirp = NULL;

#if	CMU
struct elem {
	struct elem *next;
	char *data;
};

struct list {
	struct elem *first;
	struct elem *last;
};

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
	char *malloc();

	ep = (struct elem *) calloc(1, sizeof(struct elem));
	if (ep == NULL) {
		fprintf(stderr, "Out of memory\n");
		exit(1);
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
#endif	/* CMU */

long
descend(base, name)
	char *base, *name;
{
	char *ebase0, *ebase;
	struct stat stb;
	int i;
	long blocks = 0;
	long curoff = NULL;
	register struct direct *dp;
#if	CMU
	struct list list;
	char *elem;
#endif	/* CMU */

	ebase0 = ebase = index(base, 0);
	if (ebase > base && ebase[-1] == '/')
		ebase--;
	if (lstat(name, &stb) < 0) {
		perror(base);
		*ebase0 = 0;
		return (0);
	}
	if (stb.st_nlink > 1 && (stb.st_mode&S_IFMT) != S_IFDIR) {
		for (i = 0; i <= mlx; i++)
			if (ml[i].ino == stb.st_ino && ml[i].dev == stb.st_dev)
				return (0);
		if (mlx < ML) {
			ml[mlx].dev = stb.st_dev;
			ml[mlx].ino = stb.st_ino;
			mlx++;
		}
	}
	blocks = stb.st_blocks;
	if ((stb.st_mode&S_IFMT) != S_IFDIR) {
		if (aflg)
			printf("%ld\t%s\n", kb(blocks), base);
		return (blocks);
	}
#if	CMU
	/* we always read the whole directory */
#else	/* CMU */
	if (dirp != NULL)
		closedir(dirp);
#endif	/* CMU */
	dirp = opendir(name);
	if (dirp == NULL) {
		perror(base);
		*ebase0 = 0;
		return (0);
	}
	if (chdir(name) < 0) {
		perror(base);
		*ebase0 = 0;
		closedir(dirp);
		dirp = NULL;
		return (0);
	}
#if	CMU
	list_empty(&list);
#endif	/* CMU */
	while (dp = readdir(dirp)) {
		if (!strcmp(dp->d_name, ".") || !strcmp(dp->d_name, ".."))
			continue;
#if	CMU
		list_append(&list, dp->d_name);
#else	/* CMU */
		(void) sprintf(ebase, "/%s", dp->d_name);
		curoff = telldir(dirp);
		blocks += descend(base, ebase+1);
		*ebase = 0;
		if (dirp == NULL) {
			dirp = opendir(".");
			if (dirp == NULL) {
				perror(".");
				return (0);
			}
			seekdir(dirp, curoff);
		}
#endif	/* CMU */
	}
	closedir(dirp);
	dirp = NULL;
#if	CMU
	while ((elem = list_first(&list)) != NULL) {
		*ebase = '/';
		(void) strcpy(ebase+1, elem);
		free(elem);
		blocks += descend(base, ebase+1);
		*ebase = 0;
	}
#endif	/* CMU */
	if (sflg == 0)
		printf("%ld\t%s\n", kb(blocks), base);
	if (chdir("..") < 0) {
		(void) sprintf(index(base, 0), "/..");
		perror(base);
		exit(1);
	}
	*ebase0 = 0;
	return (blocks);
}
