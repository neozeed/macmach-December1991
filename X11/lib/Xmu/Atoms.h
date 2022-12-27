/*
 * $XConsortium: Atoms.h,v 1.3 89/12/08 12:03:56 rws Exp $
 *
 * Copyright 1988 by the Massachusetts Institute of Technology
 *
 * Permission to use, copy, modify, and distribute this software and its
 * documentation for any purpose and without fee is hereby granted, provided 
 * that the above copyright notice appear in all copies and that both that 
 * copyright notice and this permission notice appear in supporting 
 * documentation, and that the name of M.I.T. not be used in advertising
 * or publicity pertaining to distribution of the software without specific, 
 * written prior permission. M.I.T. makes no representations about the 
 * suitability of this software for any purpose.  It is provided "as is"
 * without express or implied warranty.
 *
 * The X Window System is a Trademark of MIT.
 *
 * The interfaces described by this header file are for miscellaneous utilities
 * and are not part of the Xlib standard.
 */


#ifndef _XMU_ATOMS_H_
#define _XMU_ATOMS_H_

typedef struct _AtomRec *AtomPtr;

extern AtomPtr
    _XA_ATOM_PAIR,
    _XA_CHARACTER_POSITION,
    _XA_CLASS,
    _XA_CLIENT_WINDOW,
    _XA_CLIPBOARD,
    _XA_COMPOUND_TEXT,
    _XA_DECNET_ADDRESS,
    _XA_DELETE,
    _XA_FILENAME,
    _XA_HOSTNAME,
    _XA_IP_ADDRESS,
    _XA_LENGTH,
    _XA_LIST_LENGTH,
    _XA_NAME,
    _XA_NET_ADDRESS,
    _XA_NULL,
    _XA_OWNER_OS,
    _XA_SPAN,
    _XA_TARGETS,
    _XA_TEXT,
    _XA_TIMESTAMP,
    _XA_USER;

#define XA_ATOM_PAIR(d)		XmuInternAtom(d, _XA_ATOM_PAIR)
#define XA_CHARACTER_POSITION(d) XmuInternAtom(d, _XA_CHARACTER_POSITION)
#define XA_CLASS(d)		XmuInternAtom(d, _XA_CLASS)
#define XA_CLIENT_WINDOW(d)	XmuInternAtom(d, _XA_CLIENT_WINDOW)
#define XA_CLIPBOARD(d)		XmuInternAtom(d, _XA_CLIPBOARD)
#define XA_COMPOUND_TEXT(d)	XmuInternAtom(d, _XA_COMPOUND_TEXT)
#define XA_DECNET_ADDRESS(d)	XmuInternAtom(d, _XA_DECNET_ADDRESS)
#define XA_DELETE(d)		XmuInternAtom(d, _XA_DELETE)
#define XA_FILENAME(d)		XmuInternAtom(d, _XA_FILENAME)
#define XA_HOSTNAME(d)		XmuInternAtom(d, _XA_HOSTNAME)
#define XA_IP_ADDRESS(d)	XmuInternAtom(d, _XA_IP_ADDRESS)
#define XA_LENGTH(d)		XmuInternAtom(d, _XA_LENGTH)
#define XA_LIST_LENGTH(d)	XmuInternAtom(d, _XA_LIST_LENGTH)
#define XA_NAME(d)		XmuInternAtom(d, _XA_NAME)
#define XA_NET_ADDRESS(d)	XmuInternAtom(d, _XA_NET_ADDRESS)
#define XA_NULL(d)		XmuInternAtom(d, _XA_NULL)
#define XA_OWNER_OS(d)		XmuInternAtom(d, _XA_OWNER_OS)
#define XA_SPAN(d)		XmuInternAtom(d, _XA_SPAN)
#define XA_TARGETS(d)		XmuInternAtom(d, _XA_TARGETS)
#define XA_TEXT(d)		XmuInternAtom(d, _XA_TEXT)
#define XA_TIMESTAMP(d)		XmuInternAtom(d, _XA_TIMESTAMP)
#define XA_USER(d)		XmuInternAtom(d, _XA_USER)

extern AtomPtr XmuMakeAtom( /* char* */ );
extern Atom XmuInternAtom( /* Display*, AtomPtr */ );
extern void XmuInternStrings( /* Display*, String*, Cardinal, Atom* */);
extern char *XmuGetAtomName( /* Display*, Atom */ );
extern char *XmuNameOfAtom( /* AtomPtr */ );

#endif /* _XMU_ATOMS_H_ */
