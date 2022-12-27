/*
 * $XConsortium: sharedlib.c,v 1.3 89/10/09 14:28:56 jim Exp $
 *
 * Copyright 1989 Massachusetts Institute of Technology
 *
 * This file is used to force shared libraries to get the right routines.
 */

#if defined(SUNSHLIB) && !defined(SHAREDCODE)
#include <X11/IntrinsicP.h>

/*
 * The following hack is used by XmuConvertStandardSelection to get the
 * following class records.  Without these, a runtime undefined symbol error 
 * occurs.
 */
extern WidgetClass applicationShellWidgetClass,wmShellWidgetClass;

WidgetClass get_applicationShellWidgetClass()
{
    return applicationShellWidgetClass;
}

WidgetClass get_wmShellWidgetClass()
{
    return wmShellWidgetClass;
}

#else
static int dummy;                       /* avoid warning from ranlib */
#endif /* SHAREDCODE && SUNSHLIB */

