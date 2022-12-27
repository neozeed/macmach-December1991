#include "copyright.h"

/* $XConsortium: XModMap.c,v 11.10 89/12/10 10:15:22 rws Exp $ */
/* Copyright    Massachusetts Institute of Technology    1986	*/

#define NEED_REPLIES
#include "Xlibint.h"

XModifierKeymap *
XGetModifierMapping(dpy)
     register Display *dpy;
{       
    xGetModifierMappingReply rep;
    register xReq *req;
    unsigned long nbytes;
    XModifierKeymap *res;

    LockDisplay(dpy);
    GetEmptyReq(GetModifierMapping, req);
    (void) _XReply (dpy, (xReply *)&rep, 0, xFalse);

    nbytes = (unsigned long)rep.length << 2;
    res = (XModifierKeymap *) Xmalloc(sizeof (XModifierKeymap));
    if (res) res->modifiermap = (KeyCode *) Xmalloc ((unsigned) nbytes);
    if ((! res) || (! res->modifiermap)) {
	if (res) Xfree((char *) res);
	res = (XModifierKeymap *) NULL;
	_XEatData(dpy, nbytes);
    } else {
	_XReadPad(dpy, (char *) res->modifiermap, (long) nbytes);
	res->max_keypermod = rep.numKeyPerModifier;
    }

    UnlockDisplay(dpy);
    SyncHandle();
    return (res);
}

/*
 *	Returns:
 *	0	Success
 *	1	Busy - one or more old or new modifiers are down
 *	2	Failed - one or more new modifiers unacceptable
 */
int
XSetModifierMapping(dpy, modifier_map)
    register Display *dpy;
    register XModifierKeymap *modifier_map;
{
    register xSetModifierMappingReq *req;
    xSetModifierMappingReply rep;
    int         mapSize = modifier_map->max_keypermod << 3;	/* 8 modifiers */

    LockDisplay(dpy);
    GetReqExtra(SetModifierMapping, mapSize, req);

    req->numKeyPerModifier = modifier_map->max_keypermod;

    bcopy ((char *) modifier_map->modifiermap, 
    	   (char *) NEXTPTR(req,xSetModifierMappingReq), mapSize);

    (void) _XReply(dpy, (xReply *) & rep,
	(SIZEOF(xSetModifierMappingReply) - SIZEOF(xReply)) >> 2, xTrue);
    UnlockDisplay(dpy);
    SyncHandle();
    return (rep.success);
}

XModifierKeymap *
XNewModifiermap(keyspermodifier)
    int keyspermodifier;
{
    XModifierKeymap *res = (XModifierKeymap *) Xmalloc((sizeof (XModifierKeymap)));
    if (res) {
	res->max_keypermod = keyspermodifier;
	res->modifiermap = (keyspermodifier > 0 ?
			    (KeyCode *) Xmalloc((unsigned) (8 * keyspermodifier))
			    : (KeyCode *) NULL);
	if (keyspermodifier && (res->modifiermap == NULL)) {
	    Xfree((char *) res);
	    return (XModifierKeymap *) NULL;
	}
    }
    return (res);
}


XFreeModifiermap(map)
    XModifierKeymap *map;
{
    if (map) {
	if (map->modifiermap)
	    Xfree((char *) map->modifiermap);
	Xfree((char *) map);
    }
}

#if NeedFunctionPrototypes
XModifierKeymap *
XInsertModifiermapEntry(XModifierKeymap *map,
#if NeedWidePrototypes
			unsigned int keycode,
#else
			KeyCode keycode,
#endif
			int modifier)
#else
XModifierKeymap *
XInsertModifiermapEntry(map, keycode, modifier)
    XModifierKeymap *map;
    KeyCode keycode;
    int modifier;
#endif
{
    XModifierKeymap *newmap;
    int i,
	row = modifier * map->max_keypermod,
	newrow,
	lastrow;

    for (i=0; i<map->max_keypermod; i++) {
        if (map->modifiermap[ row+i ] == keycode)
	    return(map); /* already in the map */
        if (map->modifiermap[ row+i ] == 0) {
            map->modifiermap[ row+i ] = keycode;
	    return(map); /* we added it without stretching the map */
	}
    }   

    /* stretch the map */
    if ((newmap = XNewModifiermap(map->max_keypermod+1)) == NULL)
	return (XModifierKeymap *) NULL;
    newrow = row = 0;
    lastrow = map->max_keypermod * 8;
    while (row < lastrow) {
	for (i=0; i<map->max_keypermod; i++)
	    newmap->modifiermap[ newrow+i ] = map->modifiermap[ row+i ];
	newmap->modifiermap[ newrow+i ] = 0;
	row += map->max_keypermod;
	newrow += newmap->max_keypermod;
    }
    XFreeModifiermap(map);
    newrow = newmap->max_keypermod * modifier + newmap->max_keypermod - 1;
    newmap->modifiermap[ newrow ] = keycode;
    return(newmap);
}

#if NeedFunctionPrototypes
XModifierKeymap *
XDeleteModifiermapEntry(XModifierKeymap *map,
#if NeedWidePrototypes
			unsigned int keycode,
#else
			KeyCode keycode,
#endif
			int modifier)
#else
XModifierKeymap *
XDeleteModifiermapEntry(map, keycode, modifier)
    XModifierKeymap *map;
    KeyCode keycode;
    int modifier;
#endif
{
    int i,
	row = modifier * map->max_keypermod;

    for (i=0; i<map->max_keypermod; i++) {
        if (map->modifiermap[ row+i ] == keycode)
            map->modifiermap[ row+i ] = 0;
    }
    /* should we shrink the map?? */
    return (map);
}
