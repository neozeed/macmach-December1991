/* 
 * Mach Operating System
 * Copyright (c) 1989 Carnegie-Mellon University
 * All rights reserved.  The CMU software License Agreement specifies
 * the terms and conditions for use and redistribution.
 */
/*
 * HISTORY
 * $Log:	utils.c,v $
 * Revision 1.1  90/02/19  01:16:08  bww
 * 	Mach Release 2.5
 * 	[90/02/19  01:14:23  bww]
 * 
 * Revision 1.11  89/08/14  17:37:15  mrt
 * 	Changed _doprnt to vfprintf
 * 	[89/08/14            mrt]
 * 
 * 	Remove extraneous "," from static message type declaration
 * 	outputs.  Most compilers seem to accept this notation, but "lint"
 * 	does not.
 * 	[89/07/30            mwyoung]
 * 
 * Revision 1.10  89/07/30  20:29:36  mrt
 * 	Changed msg_type_padding to msg_type_unused to correspond
 * 	to the kernel version of message.h.
 * 	[89/07/28            mrt]
 * 
 * Revision 1.9  89/07/18  17:54:26  mrt
 * 	Fixed WriteStatic{Short/Long}Decl and WriteCheckDecl and
 * 	WritePackMsgType[Short/Long] to set the padding bit in
 * 	Msg_Type to 0. Otherwise the integer compares sometimes
 * 	fail.
 * 	[89/07/18            mrt]
 * 
 * Revision 1.8  89/05/20  22:18:21  mrt
 * 	Extensive revamping.  Added polymorphic arguments.
 * 	Allow multiple variable-sized inline arguments in messages.
 * 	[89/04/07            rpd]
 * 
 * 21-Aug-87  Mary Thompson (mrt) at Carnegie-Mellon University
 *	Added deallocflag to the WritePackMsg routines.
 *
 * 29-Jul-87  Mary Thompson (mrt) at Carnegie-Mellon University
 *	Changed WriteVarDecl to not automatically write
 *	semi-colons between items, so that it can be
 *	used to write C++ argument lists.
 *
 * 27-May-87  Richard Draves (rpd) at Carnegie-Mellon University
 *	Created.
 */
/*
 * ABSTRACT:
 *   Code generation utility routines.
 *   Called by header.c, user.c and server.c
 *   Exports routines:
 *	WriteImport, WriteRCSDecl, WriteBogusDefines,
 *	WriteList, WriteNameDecl, WriteVarDecl, WriteTypeDecl,
 *	ReturnTypeStr, WriteStructDecl, WriteStaticDecl,
 *	WriteCopyType, WritePackMsgType.
 */

#include <mach/message.h>
#include <varargs.h>
#include "write.h"
#include "utils.h"

void
WriteImport(file, filename)
    FILE *file;
    string_t filename;
{
    fprintf(file, "#include %s\n", filename);
}

void
WriteRCSDecl(file, name, rcs)
    FILE *file;
    identifier_t name;
    string_t rcs;
{
    fprintf(file, "#ifndef\tlint\n");
    fprintf(file, "#if\tUseExternRCSId\n");
    fprintf(file, "char %s_rcsid[] = %s;\n", name, rcs);
    fprintf(file, "#else\tUseExternRCSId\n");
    fprintf(file, "static char rcsid[] = %s;\n", rcs);
    fprintf(file, "#endif\tUseExternRCSId\n");
    fprintf(file, "#endif\tlint\n");
    fprintf(file, "\n");
}

void
WriteBogusDefines(file)
    FILE *file;
{
    fprintf(file, "#ifndef\tmig_internal\n");
    fprintf(file, "#define\tmig_internal\tstatic\n");
    fprintf(file, "#endif\n");
    fprintf(file, "\n");

    fprintf(file, "#ifndef\tTypeCheck\n");
    fprintf(file, "#define\tTypeCheck 1\n");
    fprintf(file, "#endif\n");
    fprintf(file, "\n");

    fprintf(file, "#ifndef\tUseExternRCSId\n");
    fprintf(file, "#ifdef\thc\n");
    fprintf(file, "#define\tUseExternRCSId\t\t1\n");
    fprintf(file, "#endif\n");
    fprintf(file, "#endif\n");
    fprintf(file, "\n");

    /* hc 1.4 can't handle the static msg-type declarations (??) */

    fprintf(file, "#ifndef\tUseStaticMsgType\n");
    fprintf(file, "#if\t!defined(hc) || defined(__STDC__)\n");
    fprintf(file, "#define\tUseStaticMsgType\t1\n");
    fprintf(file, "#endif\n");
    fprintf(file, "#endif\n");
    fprintf(file, "\n");
}

void
WriteList(file, args, func, mask, between, after)
    FILE *file;
    argument_t *args;
    void (*func)();
    u_int mask;
    char *between, *after;
{
    register argument_t *arg;
    register boolean_t sawone = FALSE;

    for (arg = args; arg != argNULL; arg = arg->argNext)
	if (akCheckAll(arg->argKind, mask))
	{
	    if (sawone)
		fprintf(file, "%s", between);
	    sawone = TRUE;

	    (*func)(file, arg);
	}

    if (sawone)
	fprintf(file, "%s", after);
}

static boolean_t
WriteReverseListPrim(file, arg, func, mask, between)
    FILE *file;
    register argument_t *arg;
    void (*func)();
    u_int mask;
    char *between;
{
    boolean_t sawone = FALSE;

    if (arg != argNULL)
    {
	sawone = WriteReverseListPrim(file, arg->argNext, func, mask, between);

	if (akCheckAll(arg->argKind, mask))
	{
	    if (sawone)
		fprintf(file, "%s", between);
	    sawone = TRUE;

	    (*func)(file, arg);
	}
    }

    return sawone;
}

void
WriteReverseList(file, args, func, mask, between, after)
    FILE *file;
    argument_t *args;
    void (*func)();
    u_int mask;
    char *between, *after;
{
    boolean_t sawone;

    sawone = WriteReverseListPrim(file, args, func, mask, between);

    if (sawone)
	fprintf(file, "%s", after);
}

void
WriteNameDecl(file, arg)
    FILE *file;
    argument_t *arg;
{
    fprintf(file, "%s", arg->argVarName);
}

void
WriteVarDecl(file, arg)
    FILE *file;
    argument_t *arg;
{
    char *ref = argByReferenceUser(arg) ? "*" : "";

    fprintf(file, "\t%s %s%s", arg->argType->itUserType, ref, arg->argVarName);
}

void
WriteTypeDecl(file, arg)
    FILE *file;
    register argument_t *arg;
{
    WriteStaticDecl(file, arg->argType,
		    arg->argDeallocate, arg->argLongForm,
		    arg->argTTName);
}

void
WriteCheckDecl(file, arg)
    FILE *file;
    register argument_t *arg;
{
    register ipc_type_t *it = arg->argType;

    /* We'll only be called for short-form types.
       Note we use itOutNameStr instead of itInNameStr, because
       this declaration will be used to check received types. */

    fprintf(file, "#if\tUseStaticMsgType\n");
    fprintf(file, "\tstatic msg_type_t %sCheck = {\n", arg->argVarName);
    fprintf(file, "\t\t/* msg_type_name = */\t\t%s,\n", it->itOutNameStr);
    fprintf(file, "\t\t/* msg_type_size = */\t\t%d,\n", it->itSize);
    fprintf(file, "\t\t/* msg_type_number = */\t\t%d,\n", it->itNumber);
    fprintf(file, "\t\t/* msg_type_inline = */\t\t%s,\n",
	    strbool(it->itInLine));
    fprintf(file, "\t\t/* msg_type_longform = */\tFALSE,\n");
    fprintf(file, "\t\t/* msg_type_deallocate = */\t%s,\n",
	    strbool(arg->argDeallocate));
    fprintf(file, "\t\t/* msg_type_unused = */\t0\n");
    fprintf(file, "\t};\n");
    fprintf(file, "#endif\tUseStaticMsgType\n");
}

char *
ReturnTypeStr(rt)
    routine_t *rt;
{
    if (rt->rtProcedure)
	return "void";
    else
	return rt->rtReturn->argType->itUserType;
}

char *
FetchUserType(it)
    ipc_type_t *it;
{
    return it->itUserType;
}

char *
FetchServerType(it)
    ipc_type_t *it;
{
    return it->itServerType;
}

void
WriteFieldDeclPrim(file, arg, tfunc)
    FILE *file;
    argument_t *arg;
    char *(*tfunc)();
{
    register ipc_type_t *it = arg->argType;

    fprintf(file, "\t\tmsg_type_%st %s;\n",
	    arg->argLongForm ? "long_" : "", arg->argTTName);

    if (it->itInLine && it->itVarArray)
    {
	register ipc_type_t *btype = it->itElement;

	/*
	 *	Build our own declaration for a varying array:
	 *	use the element type and maximum size specified.
	 *	Note arg->argCount->argMultiplier == btype->itNumber.
	 */
	fprintf(file, "\t\t%s %s[%d];",
			(*tfunc)(btype),
			arg->argMsgField,
			it->itNumber/btype->itNumber);
    }
    else
	fprintf(file, "\t\t%s %s;", (*tfunc)(it), arg->argMsgField);

    if (it->itPadSize != 0)
	fprintf(file, "\n\t\tchar %s[%d];", arg->argPadName, it->itPadSize);
}

void
WriteStructDecl(file, args, func, mask, name)
    FILE *file;
    argument_t *args;
    void (*func)();
    u_int mask;
    char *name;
{
    fprintf(file, "\ttypedef struct {\n");
    fprintf(file, "\t\tmsg_header_t Head;\n");
    WriteList(file, args, func, mask, "\n", "\n");
    fprintf(file, "\t} %s;\n", name);
    fprintf(file, "\n");
}

static void
WriteStaticLongDecl(file, it, dealloc, longform, name)
    FILE *file;
    register ipc_type_t *it;
    boolean_t dealloc;
    boolean_t longform;
    identifier_t name;
{
    fprintf(file, "\tstatic msg_type_long_t %s = {\n", name);
    fprintf(file, "\t{\n");
    fprintf(file, "\t\t/* msg_type_name = */\t\t0,\n");
    fprintf(file, "\t\t/* msg_type_size = */\t\t0,\n");
    fprintf(file, "\t\t/* msg_type_number = */\t\t0,\n");
    fprintf(file, "\t\t/* msg_type_inline = */\t\t%s,\n",
	    strbool(it->itInLine));
    fprintf(file, "\t\t/* msg_type_longform = */\t%s,\n",
	    strbool(longform));
    fprintf(file, "\t\t/* msg_type_deallocate = */\t%s,\n",
	    strbool(dealloc));
    fprintf(file, "\t\t/* msg_type_unused = */\t0\n");
    fprintf(file, "\t},\n");
    fprintf(file, "\t\t/* msg_type_long_name = */\t%s,\n", it->itInNameStr);
    fprintf(file, "\t\t/* msg_type_long_size = */\t%d,\n", it->itSize);
    fprintf(file, "\t\t/* msg_type_long_number = */\t%d\n", it->itNumber);
    fprintf(file, "\t};\n");
}

static void
WriteStaticShortDecl(file, it, dealloc, longform, name)
    FILE *file;
    register ipc_type_t *it;
    boolean_t dealloc;
    boolean_t longform;
    identifier_t name;
{
    fprintf(file, "\tstatic msg_type_t %s = {\n", name);
    fprintf(file, "\t\t/* msg_type_name = */\t\t%s,\n", it->itInNameStr);
    fprintf(file, "\t\t/* msg_type_size = */\t\t%d,\n", it->itSize);
    fprintf(file, "\t\t/* msg_type_number = */\t\t%d,\n", it->itNumber);
    fprintf(file, "\t\t/* msg_type_inline = */\t\t%s,\n",
	    strbool(it->itInLine));
    fprintf(file, "\t\t/* msg_type_longform = */\t%s,\n",
	    strbool(longform));
    fprintf(file, "\t\t/* msg_type_deallocate = */\t%s,\n",
	    strbool(dealloc));
    fprintf(file, "\t\t/* msg_type_unused = */\t0\n");
    fprintf(file, "\t};\n");
}

void
WriteStaticDecl(file, it, dealloc, longform, name)
    FILE *file;
    ipc_type_t *it;
    boolean_t dealloc;
    boolean_t longform;
    identifier_t name;
{
    fprintf(file, "#if\tUseStaticMsgType\n");
    if (longform)
	WriteStaticLongDecl(file, it, dealloc, longform, name);
    else
	WriteStaticShortDecl(file, it, dealloc, longform, name);
    fprintf(file, "#endif\tUseStaticMsgType\n");
}

/*ARGSUSED*/
/*VARARGS4*/
void
WriteCopyType(file, it, left, right, va_alist)
    FILE *file;
    ipc_type_t *it;
    char *left, *right;
    va_dcl
{
    va_list pvar;
    va_start(pvar);

    if (it->itStruct)
    {
	fprintf(file, "\t");
	vfprintf(file, left, pvar);
	fprintf(file, " = ");
	vfprintf(file, right, pvar);
	fprintf(file, ";\n");
    }
    else if (it->itString)
    {
	fprintf(file, "\t(void) mig_strncpy(");
	vfprintf(file, left, pvar);
	fprintf(file, ", ");
	vfprintf(file, right, pvar);
	fprintf(file, ", %d);\n\t", it->itTypeSize);
	vfprintf(file, left, pvar);
	fprintf(file, "[%d] = '\\0';\n", it->itTypeSize - 1);
    }
    else
    {
	fprintf(file, "\t{ typedef struct { char data[%d]; } *sp; * (sp) ",
		it->itTypeSize);
	vfprintf(file, left, pvar);
	fprintf(file, " = * (sp) ");
	vfprintf(file, right, pvar);
	fprintf(file, "; }\n");
    }
    va_end(pvar);
}

static void
WritePackMsgTypeLong(file, it, dealloc, longform, left, pvar)
    FILE *file;
    ipc_type_t *it;
    boolean_t dealloc;
    boolean_t longform;
    char *left;
    va_list pvar;
{
    if (it->itInName != MSG_TYPE_POLYMORPHIC)
    {
	/* if polymorphic-in, this field will get filled later */
	fprintf(file, "\t"); vfprintf(file, left, pvar);
	fprintf(file, ".msg_type_long_name = %s;\n", it->itInNameStr);
    }
    fprintf(file, "\t"); vfprintf(file, left, pvar);
    fprintf(file, ".msg_type_long_size = %d;\n", it->itSize);
    if (!it->itVarArray)
    {
	/* if VarArray, this field will get filled later */
	fprintf(file, "\t"); vfprintf(file, left, pvar);
	fprintf(file, ".msg_type_long_number = %d;\n", it->itNumber);
    }
    fprintf(file, "\t"); vfprintf(file, left, pvar);
    fprintf(file, ".msg_type_header.msg_type_inline = %s;\n", strbool(it->itInLine));
    fprintf(file, "\t"); vfprintf(file, left, pvar);
    fprintf(file, ".msg_type_header.msg_type_longform = %s;\n", strbool(longform));
    fprintf(file, "\t"); vfprintf(file, left, pvar);
    fprintf(file, ".msg_type_header.msg_type_deallocate = %s;\n", strbool(dealloc));
    fprintf(file, "\t"); vfprintf(file, left, pvar);
    fprintf(file, ".msg_type_header.msg_type_unused = 0;\n");
}

static void
WritePackMsgTypeShort(file, it, dealloc, longform, left, pvar)
    FILE *file;
    ipc_type_t *it;
    boolean_t dealloc;
    boolean_t longform;
    char *left;
    va_list pvar;
{
    if (it->itInName != MSG_TYPE_POLYMORPHIC)
    {
	/* if polymorphic-in, this field will get filled later */
	fprintf(file, "\t"); vfprintf(file, left, pvar);
	fprintf(file, ".msg_type_name = %s;\n", it->itInNameStr);
    }
    fprintf(file, "\t"); vfprintf(file, left, pvar);
    fprintf(file, ".msg_type_size = %d;\n", it->itSize);
    if (!it->itVarArray)
    {
	/* if VarArray, this field will get filled later */
	fprintf(file, "\t"); vfprintf(file, left, pvar);
	fprintf(file, ".msg_type_number = %d;\n", it->itNumber);
    }
    fprintf(file, "\t"); vfprintf(file, left, pvar);
    fprintf(file, ".msg_type_inline = %s;\n", strbool(it->itInLine));
    fprintf(file, "\t"); vfprintf(file, left, pvar);
    fprintf(file, ".msg_type_longform = %s;\n", strbool(longform));
    fprintf(file, "\t"); vfprintf(file, left, pvar);
    fprintf(file, ".msg_type_deallocate = %s;\n", strbool(dealloc));
    fprintf(file, "\t"); vfprintf(file, left, pvar);
    fprintf(file, ".msg_type_unused = 0;\n");
}

/*ARGSUSED*/
/*VARARGS4*/
void
WritePackMsgType(file, it, dealloc, longform, left, right, va_alist)
    FILE *file;
    ipc_type_t *it;
    boolean_t dealloc;
    boolean_t longform;
    char *left, *right;
    va_dcl
{
    va_list pvar;
    va_start(pvar);

    fprintf(file, "#if\tUseStaticMsgType\n");
    fprintf(file, "\t");
    vfprintf(file, left, pvar);
    fprintf(file, " = ");
    vfprintf(file, right, pvar);
    fprintf(file, ";\n");
    fprintf(file, "#else\tUseStaticMsgType\n");
    if (longform)
	WritePackMsgTypeLong(file, it, dealloc, longform, left, pvar);
    else
	WritePackMsgTypeShort(file, it, dealloc, longform, left, pvar);
    fprintf(file, "#endif\tUseStaticMsgType\n");

    va_end(pvar);
}
