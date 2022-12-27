#include "copyright.h"

/* $XConsortium: XRotProp.c,v 11.13 89/12/09 19:14:38 rws Exp $ */
/* Copyright    Massachusetts Institute of Technology    1986	*/

#include "Xlibint.h"

XRotateWindowProperties(dpy, w, properties, nprops, npositions)
    register Display *dpy;
    Window w;
    Atom *properties;
    register int nprops;
    int npositions;
    {
    register long nbytes;
    register xRotatePropertiesReq *req;

    LockDisplay(dpy);
    GetReq (RotateProperties, req);
    req->window = w;
    req->nAtoms = nprops;
    req->nPositions = npositions;
    
    req->length += nprops;
    nbytes = nprops << 2;
/* XXX Cray needs packing here.... */
    Data32 (dpy, (long *) properties, nbytes);


    UnlockDisplay(dpy);
    SyncHandle();
    }





