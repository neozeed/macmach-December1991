/*
 **********************************************************************
 * HISTORY
 * 09-Apr-87  Glenn Marcy (gm0w) at Carnegie-Mellon University
 *	Changed to save length and use bcopy instead of strcpy.
 *
 * 02-Nov-85  Glenn Marcy (gm0w) at Carnegie-Mellon University
 *	Created from routine by same name in Steve Shafer's sup program.
 *
 **********************************************************************
 */
char *malloc();

char *salloc(p)
char *p;
{
	register char *q;
	register int l;

	q = malloc(l = strlen(p) + 1);
	if (q != 0)
		bcopy(p, q, l);
	return(q);
}
