/***********************************************************
Copyright 1988 by Digital Equipment Corporation, Maynard, Massachusetts,
and the Massachusetts Institute of Technology, Cambridge, Massachusetts.

                        All Rights Reserved

Permission to use, copy, modify, and distribute this software and its 
documentation for any purpose and without fee is hereby granted, 
provided that the above copyright notice appear in all copies and that
both that copyright notice and this permission notice appear in 
supporting documentation, and that the names of Digital or MIT not be
used in advertising or publicity pertaining to distribution of the
software without specific, written prior permission.  

DIGITAL DISCLAIMS ALL WARRANTIES WITH REGARD TO THIS SOFTWARE, INCLUDING
ALL IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS, IN NO EVENT SHALL
DIGITAL BE LIABLE FOR ANY SPECIAL, INDIRECT OR CONSEQUENTIAL DAMAGES OR
ANY DAMAGES WHATSOEVER RESULTING FROM LOSS OF USE, DATA OR PROFITS,
WHETHER IN AN ACTION OF CONTRACT, NEGLIGENCE OR OTHER TORTIOUS ACTION,
ARISING OUT OF OR IN CONNECTION WITH THE USE OR PERFORMANCE OF THIS
SOFTWARE.

******************************************************************/

/* $XConsortium: fontdir.c,v 1.12 89/08/11 16:52:10 rws Exp $ */

#include <stdio.h>
#include <X11/Xos.h>
#include <sys/param.h>

#include "fontdir.h"

#define  XK_LATIN1
#include <X11/keysymdef.h>

extern void Xfree();
extern unsigned long *Xalloc(), *Xrealloc();

static char *MakeCopy(orig)
    char *orig;
{
    char *copy = (char *) Xalloc((unsigned long)(strlen(orig) + 1));
    if (copy)
	strcpy(copy, orig);
    return copy;
}

/* 
 * It is the caller's responsibility to avoid calling this if file
 * already is in table.
 */
int AddFileEntry(table, name, isAlias)
    FontTable table;
    char *name;
    Boolean isAlias;
{
    register int idx = table->file.used;

    name = MakeCopy(name);
    if (!name)
	return -1;
    if (table->file.size == idx) {
	FontFile ff;

	ff = (FontFile)Xrealloc((unsigned char *)table->file.ff,
				(unsigned long)(sizeof(FontFileRec)*
						(table->file.size)*2));
	if (!ff)
	    return -1;
	table->file.size *= 2;
	table->file.ff = ff;
    }
    table->file.used++;
    table->file.ff[idx].name = name;
	/* Warning: file names cannot be case smashed */
    table->file.ff[idx].private = NULL;
    table->file.ff[idx].alias = isAlias;
    return idx;
}

/*
 * The value returned is either the entry that matched, or in the case
 * that 'found' is false, where in the table the entry should be inserted.
 */

static int FindNormalNameInFontTable(table, name, found)
    FontTable table;
    char *name;
    Boolean *found;
{
    register int left, right, center, result;
    *found = False;

/*
 * binary search with invariant:
 *	legal search space is in [left .. right - 1];
 */

    left = 0;
    right = table->name.used;
    while (left < right) {
	center = (left + right) / 2;
	result = strcmp(name, table->name.fn[center].name);
	if (result == 0) {
	    *found = True;
	    return center;
	}
	if (result < 0)
	    right = center;
	else
	    left = center + 1;
    }
    return left;
}

int SetupWildMatch(table, pat, firstWild, phead, ptail, plen)
    FontTable table;
    register char *pat, *firstWild;
    int *phead, *ptail, *plen;
{
    char stub[MAXPATHLEN];
    int i;
    register int head, tail;
    Boolean ignore;
    register char c;

    *plen = tail = strlen(pat);
    if (!firstWild)
	for (firstWild = pat;
	     ((c = *firstWild) &&
	      !((c == XK_asterisk) || (c == XK_question)));
	     firstWild++)
	    ;
    head = firstWild - pat;
    i = 0;
    if (head)
    {
	strncpy(stub, pat, head);
	stub[head] = '\0';
	i = FindNormalNameInFontTable(table, stub, &ignore);
    }
    while ((c = pat[head]) && (c != XK_asterisk))
	head++;
    if (head < tail)
    {
	while (pat[tail-1] != XK_asterisk)
	    tail--;
    }	
    *phead = head;
    *ptail = tail;
    return i;
}

static int FindWildNameInFontTable(table, pat, firstWild, found)
    FontTable table;
    char *pat, *firstWild;
    Boolean *found;
{
    int head, tail, len;
    int i, res;

    i = SetupWildMatch(table, pat, firstWild, &head, &tail, &len);
    while (i < table->name.used)
    {
	res = Match(table->name.fn[i].name, pat, head, tail, len);
	if (res)
	{
	    *found = (res > 0);
	    return i;
	}
	i++;
    }
    *found = False;
    return 0;
}

int FindNameInFontTable(table, name, found)
    FontTable table;
    char *name;
    Boolean *found;
{
    register char *wildChar;

    for (wildChar = name; *wildChar; wildChar++)
    {
	if ((*wildChar == XK_asterisk) || (*wildChar == XK_question))
	    return FindWildNameInFontTable(table, name, wildChar, found);
    }
    return FindNormalNameInFontTable(table, name, found);
}

/*
 * This will overwrite a previous entry for the same name. This means that if
 * multiple files have the same font name contained within them, then the last
 * will win.
 */

int AddNameEntry(table, name, index)
    FontTable table;
    char *name;
    int index;
{
    int     i;
    Boolean found;
    register char *lower;

    for (lower = name; *lower; lower++)
    {
	if ((*lower == XK_asterisk) || (*lower == XK_question))
	    return 0;
    }
    i = FindNormalNameInFontTable (table, name, &found);
    if (!found) {				/* else just overwrite entry */
	name = MakeCopy (name);
	if (!name)
	    return -1;
	if (table->name.size == table->name.used) {
	    FontName fn;

	    fn = (FontName)Xrealloc ((unsigned char *)table->name.fn,
				     (unsigned long)(sizeof (FontNameRec) *
						     (table->name.size) * 2));
	    if (!fn)
		return -1;
	    table->name.size *= 2;
	    table->name.fn = fn;
	}
	if (i < table->name.used) {
	    register int j;

	    for (j = table->name.used; j > i; j--) {
		table->name.fn[j] = table->name.fn[j-1];     /* struct copy */
	    }
	}
	table->name.used++;
	table->name.fn[i].name = name;
    }
    table->name.fn[i].u.index = index;
    return 1;
}

FontTable
MakeFontTable(directory, size)
    char *directory;
    int size;
{
    FontTable table;

    table = (FontTable)Xalloc((unsigned long)sizeof(FontTableRec));
    if (table)
    {
	table->directory = MakeCopy(directory);
	table->file.ff = (FontFile)Xalloc((unsigned long)(sizeof(FontFileRec)*
							  size));
	table->name.fn = (FontName)Xalloc((unsigned long)(sizeof(FontNameRec)*
							  size));
	if (!table->directory || !table->file.ff || !table->name.fn)
	    return (FontTable)NULL;
	table->file.size = table->name.size = size;
	table->file.used = table->name.used = 0;
    }
    return table;
}

void FreeFontTable(table)
    FontTable table;
{
    int i;

    Xfree((unsigned char *)table->directory);
    for (i = 0; i < table->file.used; i++) {
	Xfree ((unsigned char *)table->file.ff[i].name);
    }
    for (i = 0; i < table->name.used; i++) {
	Xfree((unsigned char *)table->name.fn[i].name);
    }
    Xfree((unsigned char *)table->file.ff);
    Xfree((unsigned char *)table->name.fn);
    Xfree((unsigned char *)table);
}

int Match(string, pat, head, tail, plen)
    register char	*string;
    register char	*pat;
    int			head, tail, plen;
{
    register int i, l;
    int j, m, res;
    register char cp, cs;

    res = -1;
    for (i = 0; i < head; i++)
    {
	cp = pat[i];
	if (cp == XK_question)
	{
	    if (!string[i])
		return res;
	    res = 0;
	}
	else if (cp != string[i])
	    return res;
    }
    if (head == plen)
	return (string[head] ? res : 1);
    l = head;
    while (++i < tail)
    {
	/* we just skipped an asterisk */
	j = i;
	m = l;
	while ((cp = pat[i]) != XK_asterisk)
	{
	    if (!(cs = string[l]))
		return 0;
	    if ((cp != cs) && (cp != XK_question))
	    {
		m++;
		cp = pat[j];
		if (cp == XK_asterisk)
		{
		    if (!string[m])
			return 0;
		}
		else
		{
		    while ((cs = string[m]) != cp)
		    {
			if (!cs)
			    return 0;
			m++;
		    }
		}
		l = m;
		i = j;
	    }
	    l++;
	    i++;
	}
    }
    m = strlen(&string[l]);
    j = plen - tail;
    if (m < j)
	return 0;
    l = (l + m) - j;
    while (cp = pat[i])
    {
	if ((cp != string[l]) && (cp != XK_question))
	    return 0;
	l++;
	i++;
    }
    return 1;
}
