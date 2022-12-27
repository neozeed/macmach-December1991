/*
 * $XConsortium: sharedlib.c,v 1.8 90/03/05 17:34:01 kit Exp $
 * 
 * Copyright 1989 Massachusetts Institute of Technology
 *
 * This file is used to force shared libraries to get the right routines.  For
 * Sun shared libraries, this only wants to be compiled when we are *not*
 * generating shared code so that this gets copied into the application binary.
 */

#if defined(SUNSHLIB) && !defined(SHAREDCODE)
#include "IntrinsicI.h"
#include "VarargsI.h"

/*
 * _XtInherit needs to be statically linked since it is compared against as
 * well as called.
 */
void _XtInherit()
{
    extern void __XtInherit();
    __XtInherit();
}

/*
 * The following routine will be called by every toolkit
 * application, forcing this file to be statically linked.
 *
 * Note: Both XtInitialize and XtAppInitialize call XtToolkitInitialize.
 */

void XtToolkitInitialize()
{
    extern void _XtToolkitInitialize();
    _XtToolkitInitialize();
}

Widget 
XtInitialize(name, classname, options, num_options, argc, argv)
String name, classname;
XrmOptionDescRec *options;
Cardinal num_options, *argc;
String *argv;
{
    extern Widget _XtInitialize();
    return _XtInitialize (name, classname, options, num_options, argc, argv);
}

Widget
XtAppInitialize(app_context_return, application_class, options, num_options,
		argc_in_out, argv_in_out, fallback_resources, 
		args_in, num_args_in)
XtAppContext * app_context_return;
String application_class;
XrmOptionDescRec *options;
Cardinal num_options, *argc_in_out, num_args_in;
String *argv_in_out, * fallback_resources;     
ArgList args_in;
{
    extern Widget _XtAppInitialize();
    return _XtAppInitialize (app_context_return, application_class, options,
			     num_options, argc_in_out, argv_in_out, 
			     fallback_resources, args_in, num_args_in);
}

#if NeedFunctionPrototypes
Widget
XtVaAppInitialize(XtAppContext *app_context_return, String application_class,
		  XrmOptionDescList options, Cardinal num_options,
		  Cardinal *argc_in_out, String *argv_in_out,
		  String *fallback_resources, ...)
#else
Widget XtVaAppInitialize(app_context_return, application_class, options,
			 num_options, argc_in_out, argv_in_out,
			 fallback_resources, va_alist)
    XtAppContext *app_context_return;
    String application_class;
    XrmOptionDescList options;
    Cardinal num_options;
    Cardinal *argc_in_out;
    String *argv_in_out;
    String *fallback_resources;
    va_dcl
#endif
{
    va_list	var;
    extern Widget _XtVaAppInitialize();

    Va_start(var, fallback_resources);    
    return _XtVaAppInitialize(app_context_return, application_class, options,
			      num_options, argc_in_out, argv_in_out,
			      fallback_resources, var);
}

#else
static int dummy;			/* avoid warning from ranlib */
#endif /* SUNSHLIB */
