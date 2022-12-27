/*
 * The routines in this file
 * deal with the region, that magic space
 * between "." and mark. Some functions are
 * commands. Some functions are just for
 * internal use.
 */
#include        <stdio.h>
#include	"estruct.h"
#include	"etype.h"
#include        "edef.h"
#include	"elang.h"

/*	reglines:	how many lines in the current region
			used by the trim/entab/detab-region commands
*/

int PASCAL NEAR reglines()

{
        register LINE *linep;	/* position while scanning */
	register int n;		/* number of lines in this current region */
        REGION region;

	/* check for a valid region first */
        if (getregion(&region) != TRUE)
                return(0);

	/* start at the top of the region.... */
        linep = region.r_linep;
	region.r_size += region.r_offset;
        n = 0;

        /* scan the region... counting lines */
        while (region.r_size > 0L) {
		region.r_size -= llength(linep) + 1;
		linep = lforw(linep);
		n++;
	}

	/* place us at the beginning of the region */
        curwp->w_dotp = region.r_linep;
        curwp->w_doto = region.r_offset;

        return(n);
}

/*
 * Kill the region. Ask "getregion"
 * to figure out the bounds of the region.
 * Move "." to the start, and kill the characters.
 * Bound to "C-W".
 */
PASCAL NEAR killregion(f, n)

{
        register int    s;
        REGION          region;

	if (curbp->b_mode&MDVIEW)	/* don't allow this command if	*/
		return(rdonly());	/* we are in read only mode	*/
        if ((s=getregion(&region)) != TRUE)
                return(s);
        if ((lastflag&CFKILL) == 0)             /* This is a kill type  */
                kdelete();                      /* command, so do magic */
        thisflag |= CFKILL;                     /* kill buffer stuff.   */
        curwp->w_dotp = region.r_linep;
        curwp->w_doto = region.r_offset;
        return(ldelete(region.r_size, TRUE));
}

/*
 * Copy all of the characters in the
 * region to the kill buffer. Don't move dot
 * at all. This is a bit like a kill region followed
 * by a yank. Bound to "M-W".
 */
PASCAL NEAR copyregion(f, n)
{
        register LINE   *linep;
        register int    loffs;
        register int    s;
        REGION          region;

        if ((s=getregion(&region)) != TRUE)
                return(s);
        if ((lastflag&CFKILL) == 0)             /* Kill type command.   */
                kdelete();
        thisflag |= CFKILL;
        linep = region.r_linep;                 /* Current line.        */
        loffs = region.r_offset;                /* Current offset.      */
        while (region.r_size--) {
                if (loffs == llength(linep)) {  /* End of line.         */
                        if ((s=kinsert('\r')) != TRUE)
                                return(s);
                        linep = lforw(linep);
                        loffs = 0;
                } else {                        /* Middle of line.      */
                        if ((s=kinsert(lgetc(linep, loffs))) != TRUE)
                                return(s);
                        ++loffs;
                }
        }
	mlwrite(TEXT70);
/*              "[region copied]" */
        return(TRUE);
}

/*
 * Lower case region. Zap all of the upper
 * case characters in the region to lower case. Use
 * the region code to set the limits. Scan the buffer,
 * doing the changes. Call "lchange" to ensure that
 * redisplay is done in all buffers. Bound to
 * "C-X C-L".
 */
PASCAL NEAR lowerregion(f, n)
{
        register LINE   *linep;
        register int    loffs;
        register int    s;
        int c;
        REGION          region;

	if (curbp->b_mode&MDVIEW)	/* don't allow this command if	*/
		return(rdonly());	/* we are in read only mode	*/
        if ((s=getregion(&region)) != TRUE)
                return(s);
        lchange(WFHARD);
        linep = region.r_linep;
        loffs = region.r_offset;
        while (region.r_size--) {
                if (loffs == llength(linep)) {
                        linep = lforw(linep);
                        loffs = 0;
                } else {
                        c = lgetc(linep, loffs);
			c = lowerc(c);
                        lputc(linep, loffs, c);
                        ++loffs;
                }
        }
        return(TRUE);
}

/*
 * Upper case region. Zap all of the lower
 * case characters in the region to upper case. Use
 * the region code to set the limits. Scan the buffer,
 * doing the changes. Call "lchange" to ensure that
 * redisplay is done in all buffers. Bound to
 * "C-X C-L".
 */
PASCAL NEAR upperregion(f, n)
{
        register LINE   *linep;
        register int    loffs;
        register int    s;
        int c;
        REGION          region;

	if (curbp->b_mode&MDVIEW)	/* don't allow this command if	*/
		return(rdonly());	/* we are in read only mode	*/
        if ((s=getregion(&region)) != TRUE)
                return(s);
        lchange(WFHARD);
        linep = region.r_linep;
        loffs = region.r_offset;
        while (region.r_size--) {
                if (loffs == llength(linep)) {
                        linep = lforw(linep);
                        loffs = 0;
                } else {
                        c = lgetc(linep, loffs);
			c = upperc(c);
                        lputc(linep, loffs, c);
                        ++loffs;
                }
        }
        return(TRUE);
}

/*	Narrow-to-region (^X-<) makes all but the current region in
	the current buffer invisable and unchangable
*/

PASCAL NEAR narrow(f, n)

{
        register int status;	/* return status */
	BUFFER *bp;		/* buffer being narrowed */
	WINDOW *wp;		/* windows to fix up pointers in as well */
	REGION creg;		/* region boundry structure */
	int cmark;		/* current mark */

	/* find the proper buffer and make sure we aren't already narrow */
	bp = curwp->w_bufp;		/* find the right buffer */
	if (bp->b_flag&BFNAROW) {
		mlwrite(TEXT71);
/*                      "%%This buffer is already narrowed" */
		return(FALSE);
	}

	/* find the boundries of the current region */
        if ((status=getregion(&creg)) != TRUE)
                return(status);
        curwp->w_dotp = creg.r_linep;	/* only by full lines please! */
        curwp->w_doto = 0;
	creg.r_size += (long)creg.r_offset;
	if (creg.r_size <= (long)curwp->w_dotp->l_used) {
		mlwrite(TEXT72);
/*                      "%%Must narrow at least 1 full line" */
		return(FALSE);
	}

	/* archive the top fragment */
	if (bp->b_linep->l_fp != creg.r_linep) {
		bp->b_topline = bp->b_linep->l_fp;
		creg.r_linep->l_bp->l_fp = (LINE *)NULL;
		bp->b_linep->l_fp = creg.r_linep;
		creg.r_linep->l_bp = bp->b_linep;
	}

	/* move forward to the end of this region
	   (a long number of bytes perhaps) */
	while (creg.r_size > (long)32000) {
		forwchar(TRUE, 32000);
		creg.r_size -= (long)32000;
	}
	forwchar(TRUE, (int)creg.r_size);
	curwp->w_doto = 0;		/* only full lines! */

	/* archive the bottom fragment */
	if (bp->b_linep != curwp->w_dotp) {
		bp->b_botline = curwp->w_dotp;
		bp->b_botline->l_bp->l_fp = bp->b_linep;
		bp->b_linep->l_bp->l_fp = (LINE *)NULL;
		bp->b_linep->l_bp = bp->b_botline->l_bp;
	}

	/* let all the proper windows be updated */
	wp = wheadp;
	while (wp) {
		if (wp->w_bufp == bp) {
			wp->w_linep = creg.r_linep;
			wp->w_dotp = creg.r_linep;
			wp->w_doto = 0;
			for (cmark = 0; cmark < NMARKS; cmark++) {
				wp->w_markp[cmark] = creg.r_linep;
				wp->w_marko[cmark] = 0;
			}
			wp->w_flag |= (WFHARD|WFMODE);
		}
		wp = wp->w_wndp;
	}

	/* and now remember we are narrowed */
	bp->b_flag |= BFNAROW;
	mlwrite(TEXT73);
/*              "[Buffer is narrowed]" */
        return(TRUE);
}

/*	widen-from-region (^X->) restores a narrowed region	*/

PASCAL NEAR widen(f, n)

{
	LINE *lp;	/* temp line pointer */
	BUFFER *bp;	/* buffer being narrowed */
	WINDOW *wp;	/* windows to fix up pointers in as well */

	/* find the proper buffer and make sure we are narrow */
	bp = curwp->w_bufp;		/* find the right buffer */
	if ((bp->b_flag&BFNAROW) == 0) {
		mlwrite(TEXT74);
/*                      "%%This buffer is not narrowed" */
		return(FALSE);
	}

	/* recover the top fragment */
	if (bp->b_topline != (LINE *)NULL) {
		lp = bp->b_topline;
		while (lp->l_fp != (LINE *)NULL)
			lp = lp->l_fp;
		lp->l_fp = bp->b_linep->l_fp;
		lp->l_fp->l_bp = lp;
		bp->b_linep->l_fp = bp->b_topline;
		bp->b_topline->l_bp = bp->b_linep;
		bp->b_topline = (LINE *)NULL;
	}

	/* recover the bottom fragment */
	if (bp->b_botline != (LINE *)NULL) {
		lp = bp->b_botline;
		while (lp->l_fp != (LINE *)NULL)
			lp = lp->l_fp;
		lp->l_fp = bp->b_linep;
		bp->b_linep->l_bp->l_fp = bp->b_botline;
		bp->b_botline->l_bp = bp->b_linep->l_bp;
		bp->b_linep->l_bp = lp;
		bp->b_botline = (LINE *)NULL;
	}

	/* let all the proper windows be updated */
	wp = wheadp;
	while (wp) {
		if (wp->w_bufp == bp)
			wp->w_flag |= (WFHARD|WFMODE);
		wp = wp->w_wndp;
	}
	/* and now remember we are not narrowed */
	bp->b_flag &= (~BFNAROW);
	mlwrite(TEXT75);
/*              "[Buffer is widened]" */
        return(TRUE);
}

/*
 * This routine figures out the bounds of the region in the current
 * window, and fills in the fields of the "REGION" structure pointed to by
 * "rp". Because the dot and mark are usually very close together, we scan
 * outward from dot looking for mark. This should save time. Return a
 * standard code. Callers of this routine should be prepared to get an
 * "ABORT" status; we might make this have the confirm thing later.
 */

PASCAL NEAR getregion(rp)

register REGION *rp;

{
        register LINE   *flp;
        register LINE   *blp;
        long fsize;
        long bsize;

        if (curwp->w_markp[0] == (LINE *)NULL) {
                mlwrite(TEXT76);
/*                      "No mark set in this window" */
                return(FALSE);
        }
        if (curwp->w_dotp == curwp->w_markp[0]) {
                rp->r_linep = curwp->w_dotp;
                if (curwp->w_doto < curwp->w_marko[0]) {
                        rp->r_offset = curwp->w_doto;
                        rp->r_size = (long)(curwp->w_marko[0]-curwp->w_doto);
                } else {
                        rp->r_offset = curwp->w_marko[0];
                        rp->r_size = (long)(curwp->w_doto-curwp->w_marko[0]);
                }
                return(TRUE);
        }
        blp = curwp->w_dotp;
        bsize = (long)curwp->w_doto;
        flp = curwp->w_dotp;
        fsize = (long)(llength(flp)-curwp->w_doto+1);
        while (flp!=curbp->b_linep || lback(blp)!=curbp->b_linep) {
                if (flp != curbp->b_linep) {
                        flp = lforw(flp);
                        if (flp == curwp->w_markp[0]) {
                                rp->r_linep = curwp->w_dotp;
                                rp->r_offset = curwp->w_doto;
                                rp->r_size = fsize+curwp->w_marko[0];
                                return(TRUE);
                        }
                        fsize += llength(flp)+1;
                }
                if (lback(blp) != curbp->b_linep) {
                        blp = lback(blp);
                        bsize += llength(blp)+1;
                        if (blp == curwp->w_markp[0]) {
                                rp->r_linep = blp;
                                rp->r_offset = curwp->w_marko[0];
                                rp->r_size = bsize - curwp->w_marko[0];
                                return(TRUE);
                        }
                }
        }
        mlwrite(TEXT77);
/*              "Bug: lost mark" */
        return(FALSE);
}

char *PASCAL NEAR getreg()	/* return some of the contents of the current region */

{
	register char * sp;	/* ptr into temporary buffer */
	register LINE *linep;
	register int loffs;
	register int s;
	REGION region;
	char value[NSTRING];	/* temp buffer for value */

	/* get the region limits */
	if ((s=getregion(&region)) != TRUE)
		return(errorm);
	linep = region.r_linep; 		/* Current line.	*/
	loffs = region.r_offset;		/* Current offset.	*/

	/* don't let the region be larger than a string can hold */
	if (region.r_size >= NSTRING)
		region.r_size = NSTRING - 1;

	/* scan the region, record the characters */
	sp = value;
	while (region.r_size--) {
		if (loffs == llength(linep)) {	/* End of line. 	*/
			*sp++ = '\r';
			linep = lforw(linep);
			loffs = 0;
		} else {			/* Middle of line.	*/
			*sp++ = lgetc(linep, loffs);
			++loffs;
		}
	}

	/* and return the constructed value */
	*sp = 0;
	return(value);
}

