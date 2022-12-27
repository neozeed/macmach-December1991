/* 
 * Mach Operating System
 * Copyright (c) 1989 Carnegie-Mellon University
 * All rights reserved.  The CMU software License Agreement specifies
 * the terms and conditions for use and redistribution.
 */
/*
 * HISTORY
 * $Log:	header.c,v $
 * Revision 1.1  90/02/19  01:14:49  bww
 * 	Mach Release 2.5
 * 	[90/02/19  01:14:23  bww]
 * 
 * Revision 1.7  89/05/20  22:14:51  mrt
 * 	Extensive revamping.  Added polymorphic arguments.
 * 	Allow multiple variable-sized inline arguments in messages.
 * 	[89/04/07            rpd]
 * 
 *  8-Jul-88  Mary Thompson (mrt) at Carnegie-Mellon University
 *	Conditionally defined mig_external to be extern and then defined
 *	all functions  with the storage class mig_external.
 *	Mig_external can be changed
 *	when the generated code is compiled.
 *
 * 18-Jan-88  David Detlefs (dld) at Carnegie-Mellon University
 *	Modified to produce C++ compatible code via #ifdefs.
 *	All changes have to do with argument declarations.
 *
 *  3-Aug-87  Mary Thompson (mrt) at Carnegie-Mellon University
 *	Revision to make multi-threaded use work. Removed definitions for
 * 	alloc_reply_port and init_msg_type as these routines are 
 * 	no longer generated.
 *
 * 30-Jul-87  Mary Thompson (mrt) at Carnegie-Mellon University
 * 	Made changes to generate conditional code for C++ parameter lists
 *
 * 29-Jul-87  Mary Thompson (mrt) at Carnegie-Mellon University
 *	Changed WriteRoutine to produce conditional argument
 *	lists for C++
 *
 *  8-Jun-87  Mary Thompson (mrt) at Carnegie-Mellon University
 *	Changed the KERNEL include from ../h to sys/
 *	Removed extern from WriteHeader to make hi-c happy
 *
 * 28-May-87  Richard Draves (rpd) at Carnegie-Mellon University
 *	Created.
 */
/*
 *  Abstract:
 *	routines to write pieces of the Header module.
 *	exports WriteHeader which directs the writing of
 *	the Header module
 */

#include "write.h"
#include "utils.h"
#include "global.h"

static void
WriteIncludes(file)
    FILE *file;
{
    fprintf(file, "#include <mach/kern_return.h>\n");
    fprintf(file, "#if\t%s || %s\n", NewCDecl, LintLib);
    fprintf(file, "#include <mach/port.h>\n");
    fprintf(file, "#include <mach/message.h>\n");
    fprintf(file, "#endif\n");
    fprintf(file, "\n");
}

static void
WriteExternalDecls(file)
    FILE *file;
{
    fprintf(file, "mig_external void %s\n", InitRoutineName);
    fprintf(file, "#if\t%s\n", NewCDecl);
    fprintf(file, "    (port_t rep_port);\n");
    fprintf(file, "#else\n");
    fprintf(file, "    ();\n");
    fprintf(file, "#endif\n");
}

static void
WriteProlog(file)
    FILE *file;
{
    fprintf(file, "#ifndef\t_%s\n", SubsystemName);
    fprintf(file, "#define\t_%s\n", SubsystemName);
    fprintf(file, "\n");
    fprintf(file, "/* Module %s */\n", SubsystemName);
    fprintf(file, "\n");

    WriteIncludes(file);

    fprintf(file, "#ifndef\tmig_external\n");
    fprintf(file, "#define mig_external extern\n");
    fprintf(file, "#endif\n");
    fprintf(file, "\n");

    WriteExternalDecls(file);
}

static void
WriteEpilog(file)
    FILE *file;
{
    fprintf(file, "\n");
    fprintf(file, "#endif\t_%s\n", SubsystemName);
}

static void
WriteRoutine(file, rt)
    FILE *file;
    routine_t *rt;
{
    fprintf(file, "\n");
    fprintf(file, "/* %s %s */\n", rtRoutineKindToStr(rt->rtKind), rt->rtName);
    fprintf(file, "mig_external %s %s\n", ReturnTypeStr(rt), rt->rtUserName);
    fprintf(file, "#if\t%s\n", LintLib);
    fprintf(file, "    (");
    WriteList(file, rt->rtArgs, WriteNameDecl, akbUserArg, ", " , "");
    fprintf(file, ")\n");
    WriteList(file, rt->rtArgs, WriteVarDecl, akbUserArg, ";\n", ";\n");
    fprintf(file, "{ ");
    if (!rt->rtProcedure)
	fprintf(file, "return ");
    fprintf(file, "%s(", rt->rtUserName);
    WriteList(file, rt->rtArgs, WriteNameDecl, akbUserArg, ", ", "");
    fprintf(file, "); }\n");
    fprintf(file, "#else\n");
    fprintf(file, "#if\t%s\n", NewCDecl);
    fprintf(file, "(\n");
    WriteList(file, rt->rtArgs, WriteVarDecl, akbUserArg, ",\n", "\n");
    fprintf(file, ");\n");
    fprintf(file, "#else\n");
    fprintf(file, "    ();\n");
    fprintf(file, "#endif\n");
    fprintf(file, "#endif\n");
}

void
WriteHeader(file, stats)
    FILE *file;
    statement_t *stats;
{
    register statement_t *stat;

    WriteProlog(file);
    for (stat = stats; stat != stNULL; stat = stat->stNext)
	switch (stat->stKind)
	{
	  case skRoutine:
	    WriteRoutine(file, stat->stRoutine);
	    break;
	  case skImport:
	  case skUImport:
	    WriteImport(file, stat->stFileName);
	    break;
	  case skSImport:
	    break;
	  default:
	    fatal("WriteUser(): bad statement_kind_t (%d)",
		  (int) stat->stKind);
	}
    WriteEpilog(file);
}
