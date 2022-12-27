/* $XConsortium: XStrKeysym.c,v 11.4 89/12/11 19:10:34 rws Exp $ */
/* Copyright 1985, 1987, Massachusetts Institute of Technology */

#include "Xlibint.h"
#include <X11/Xresource.h>

static struct ks_info {
    char	*ks_name;
    KeySym	ks_val;
} keySymInfo[] = {
#include	"ks_names.h"
};

#ifndef KEYSYMDB
#define KEYSYMDB "/usr/lib/X11/XKeysymDB"
#endif

static Bool initialized = False;
static XrmDatabase keysymdb;

_XInitKeysymDB()
{
    XrmInitialize();
    keysymdb = XrmGetFileDatabase(KEYSYMDB);
    initialized = True;
}

#if NeedFunctionPrototypes
KeySym XStringToKeysym(s)
    const char *s;
#else
KeySym XStringToKeysym(s)
    char *s;
#endif
{
    int i;

    /*
     *	Yes,  yes,  yes.  I know this is a linear search,  and we should
     *	do better,  but I'm in a hurry right now.
     */

    for (i = 0; i < ((sizeof keySymInfo)/(sizeof keySymInfo[0])); i++) {
	if (strcmp(s, keySymInfo[i].ks_name) == 0)
	    return (keySymInfo[i].ks_val);
    }
    if (!initialized)
	_XInitKeysymDB();
    if (keysymdb)
    {
	XrmString type_str;
	XrmValue result;
	char c;
	KeySym val;

	XrmGetResource(keysymdb, s, "Keysym", &type_str, &result);
	if (result.addr && (result.size > 1))
	{
	    val = 0;
	    for (i = 0; i < result.size - 1; i++)
	    {
		c = ((char *)result.addr)[i];
		if ('0' <= c && c <= '9') val = val*16+c-'0';
		else if ('a' <= c && c <= 'z') val = val*16+c-'a'+10;
		else if ('A' <= c && c <= 'Z') val = val*16+c-'A'+10;
		else return NoSymbol;
	    }
	    return val;
	}
    }
    return (NoSymbol);
}

extern char *_XrmGetResourceName();

char *XKeysymToString(ks)
    KeySym ks;
{
    int i;

    /*
     *	Yes,  yes,  yes.  I know this is a linear search,  and we should
     *	do better,  but I'm in a hurry right now.
     */

    for (i = 0; i < ((sizeof keySymInfo)/(sizeof keySymInfo[0])); i++) {
	if (ks == keySymInfo[i].ks_val)
	    return (keySymInfo[i].ks_name);
    }
    if (!initialized)
	_XInitKeysymDB();
    if (keysymdb)
    {
	char buf[8];
	XrmValue resval;

	sprintf(buf, "%lX", ks);
	resval.addr = (caddr_t)buf;
	resval.size = strlen(buf) + 1;
	return _XrmGetResourceName(keysymdb, "String", &resval);
    }
    return ((char *) NULL);
}
