/*
 * Copyright (c) 1989, 1990, 1991 by the University of Washington
 *
 * For copying and distribution information, please see the file
 * <uw-copyright.h>.
 */

#include <uw-copyright.h>
#include <stdio.h>

#include <pfs.h>
#include <pmachine.h> /* for correct definition of ZERO */

static PTEXT	free = NULL;
int 		ptext_count = 0;
int		ptext_max = 0;

/*
 * ptalloc - allocate and initialize ptext structure
 *
 *    PTALLOC returns a pointer to an initialized structure of type
 *    PTEXT.  If it is unable to allocate such a structure, it
 *    returns NULL.
 */
PTEXT
ptalloc()
    {
	PTEXT	vt;
	if(free) {
	    vt = free;
	    free = free->next;
	}
	else {
	    vt = (PTEXT) malloc(sizeof(PTEXT_ST));
	    if (!vt) return(NULL);
	    ptext_max++;
	}
	ptext_count++;

	/* nearly all parts are 0 [or NULL] */
	ZERO(vt);
	/* The offset is to leave room for additional headers */
	vt->start = vt->dat + MAX_PTXT_HDR;

	return(vt);
    }

/*
 * ptfree - free a VTEXT structure
 *
 *    VTFREE takes a pointer to a VTEXT structure and adds it to
 *    the free list for later reuse.
 */
ptfree(vt)
    PTEXT	vt;
    {
	vt->next = free;
	vt->previous = NULL;
	free = vt;
	ptext_count--;
    }

/*
 * ptlfree - free a VTEXT structure
 *
 *    VTLFREE takes a pointer to a VTEXT structure frees it and any linked
 *    VTEXT structures.  It is used to free an entrie list of VTEXT
 *    structures.
 */
ptlfree(vt)
    PTEXT	vt;
    {
	PTEXT	nxt;

	while(vt != NULL) {
	    nxt = vt->next;
	    ptfree(vt);
	    vt = nxt;
	}
    }

