#include "copyright.h"

/* $XConsortium: XFontNames.c,v 11.21 89/12/11 19:09:13 rws Exp $ */
/* Copyright    Massachusetts Institute of Technology    1986	*/
#define NEED_REPLIES
#include "Xlibint.h"

#if NeedFunctionPrototypes
char **XListFonts(
register Display *dpy,
const char *pattern,  /* null-terminated */
int maxNames,
int *actualCount)	/* RETURN */
#else
char **XListFonts(dpy, pattern, maxNames, actualCount)
register Display *dpy;
char *pattern;  /* null-terminated */
int maxNames;
int *actualCount;	/* RETURN */
#endif
{       
    register long nbytes;
    register int i;
    register int length;
    char **flist;
    char *ch;
    xListFontsReply rep;
    register xListFontsReq *req;
    register long rlen;

    LockDisplay(dpy);
    GetReq(ListFonts, req);
    req->maxNames = maxNames;
    nbytes = req->nbytes = pattern ? strlen (pattern) : 0;
    req->length += (nbytes + 3) >> 2;
    _XSend (dpy, pattern, nbytes);
    /* use _XSend instead of Data, since following _XReply will flush buffer */

    (void) _XReply (dpy, (xReply *)&rep, 0, xFalse);

    if (rep.nFonts) {
	flist = (char **)Xmalloc ((unsigned)rep.nFonts * sizeof(char *));
	rlen = rep.length << 2;
	ch = (char *) Xmalloc((unsigned) (rlen + 1));
	    /* +1 to leave room for last null-terminator */

	if ((! flist) || (! ch)) {
	    if (flist) Xfree((char *) flist);
	    if (ch) Xfree(ch);
	    _XEatData(dpy, (unsigned long) rlen);
	    UnlockDisplay(dpy);
	    SyncHandle();
	    return (char **) NULL;
	}

	_XReadPad (dpy, ch, rlen);
	/*
	 * unpack into null terminated strings.
	 */
	length = *ch;
	for (i = 0; i < rep.nFonts; i++) {
	    flist[i] = ch + 1;  /* skip over length */
	    ch += length + 1;  /* find next length ... */
	    length = *ch;
	    *ch = '\0';  /* and replace with null-termination */
	}
    }
    else flist = (char **) NULL;
    *actualCount = rep.nFonts;
    UnlockDisplay(dpy);
    SyncHandle();
    return (flist);
}

XFreeFontNames(list)
char **list;
{       
	if (list != NULL) {
		Xfree (list[0]-1);
		Xfree ((char *)list);
	}
}
