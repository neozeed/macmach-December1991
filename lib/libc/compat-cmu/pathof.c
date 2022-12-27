/*
 * pathof - find path of a component needed by a program
 *
 * char *pathof(component)
 * char *component;
 *
 * Where component is of the form "<prog>.<subprog>" or "<prog>.<subdir>"
 *
 * Examples of component are:
 *      "sup.libpath"
 *      "csh.profiledir"
 *      "login.mkplan"
 *
 * If component is not found, pathof() returns NULL.  If component
 * is found, a copy of the path for that component is salloc()ed and
 * a pointer to the copy is returned.
 *
 **********************************************************************
 * HISTORY
 * 30-Apr-88  Glenn Marcy (gm0w) at Carnegie-Mellon University
 *	Created.
 *
 **********************************************************************
 */
#include <stdio.h>

char *pathof(component)
char *component;
{
    FILE *f;
    char *p, *q, buf[BUFSIZ];
    char *salloc();
    int len;

    if ((f = fopen("/usr/lib/paths", "r")) == NULL)
	return(NULL);
    len = strlen(component);
    while ((p = fgets(buf, BUFSIZ, f)) != NULL) {
	if (index("#;:\n", buf[0]) != NULL)
	    continue;
	if (buf[len] != ' ' && buf[len] != '\t')
	    continue;
	if (strncasecmp(buf, component, len) != 0)
	    continue;
	p = buf + len + 1;
	while (*p == ' ' || *p == '\t')
	    p++;
	q = p;
	while (*q != ' ' && *q != '\t' && *q != '\n' && *q != '\0')
	    q++;
	*q = '\0';
	break;
    }
    (void) fclose(f);
    return(p ? salloc(p) : NULL);
}
