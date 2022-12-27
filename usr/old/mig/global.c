/* 
 * Mach Operating System
 * Copyright (c) 1989 Carnegie-Mellon University
 * All rights reserved.  The CMU software License Agreement specifies
 * the terms and conditions for use and redistribution.
 */
/*
 * HISTORY
 * $Log:	global.c,v $
 * Revision 1.1  90/02/19  01:14:41  bww
 * 	Mach Release 2.5
 * 	[90/02/19  01:14:23  bww]
 * 
 * Revision 1.9  89/05/20  22:14:43  mrt
 * 	Extensive revamping.  Added polymorphic arguments.
 * 	Allow multiple variable-sized inline arguments in messages.
 * 	[89/04/07            rpd]
 * 
 *  8-Feb-89  David Golub (dbg) at Carnegie-Mellon University
 *	Use default values for output file names only if they have not
 *	yet been set.
 *
 * 17-Sep-87  Bennet Yee (bsy) at Carnegie-Mellon University
 *	Added GenSymTab
 *
 *
 * 28-May-87  Richard Draves (rpd) at Carnegie-Mellon University
 *	Created.
 */
/*
 *  ABSTRACT:
 *	exports most of the global variables.
 * 	init_global sets input file name before yyparse
 *		is called.
 *	more_global is called after yyparse and before
 *		any code generation to initialize names
 *		that are derived from the subsystem name.
 *
 */

#include "string.h"
#include "global.h"

boolean_t BeQuiet = FALSE;
boolean_t BeVerbose = FALSE;
boolean_t UseMsgRPC = TRUE;
boolean_t GenSymTab = FALSE;

boolean_t IsKernel = FALSE;
boolean_t IsCamelot = FALSE;

string_t RCSId = strNULL;

string_t SubsystemName = strNULL;
u_int SubsystemBase = 0;

string_t MsgType = strNULL;
string_t WaitTime = strNULL;
string_t ErrorProc = "MsgError";
string_t ServerPrefix = strNULL;
string_t UserPrefix = strNULL;
char CamelotPrefix[4] = "op_";

string_t yyinname;

char NewCDecl[] = "(defined(__STDC__) || defined(c_plusplus))";
char LintLib[] = "defined(LINTLIBRARY)";

void
init_global()
{
    yyinname = strmake("<no name yet>");
}

string_t HeaderFileName = strNULL;
string_t UserFileName   = strNULL;
string_t ServerFileName = strNULL;

identifier_t InitRoutineName;
identifier_t SetMsgTypeName;
identifier_t ReplyPortName;
identifier_t ReplyPortIsOursName;
identifier_t MsgTypeVarName;
identifier_t DeallocPortRoutineName;
identifier_t AllocPortRoutineName;
identifier_t ServerProcName;

void
more_global()
{
    if (SubsystemName == strNULL)
	fatal("no SubSystem declaration");

    if (HeaderFileName == strNULL)
	HeaderFileName = strconcat(SubsystemName, ".h");
    if (UserFileName == strNULL)
	UserFileName = strconcat(SubsystemName, "User.c");
    if (ServerFileName == strNULL)
	ServerFileName = strconcat(SubsystemName, "Server.c");

    InitRoutineName = strconcat("init_", SubsystemName);
    SetMsgTypeName = strconcat("set_msg_type_", SubsystemName);
    MsgTypeVarName = strconcat("msg_type_var_", SubsystemName);
    ReplyPortName = strconcat(SubsystemName, "_reply_port");
    ReplyPortIsOursName = strconcat(ReplyPortName, "_is_ours");
    DeallocPortRoutineName = strconcat(ReplyPortName, "_dealloc");
    AllocPortRoutineName = strconcat(ReplyPortName, "_alloc");
    ServerProcName = strconcat(SubsystemName, "_server");
}
