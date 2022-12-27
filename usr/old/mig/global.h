/* 
 * Mach Operating System
 * Copyright (c) 1989 Carnegie-Mellon University
 * All rights reserved.  The CMU software License Agreement specifies
 * the terms and conditions for use and redistribution.
 */
/*
 * HISTORY
 * $Log:	global.h,v $
 * Revision 1.1  90/02/19  01:14:45  bww
 * 	Mach Release 2.5
 * 	[90/02/19  01:14:23  bww]
 * 
 * Revision 1.8  89/05/20  22:14:47  mrt
 * 	Extensive revamping.  Added polymorphic arguments.
 * 	Allow multiple variable-sized inline arguments in messages.
 * 	[89/04/07            rpd]
 * 
 * 17-Sep-87  Bennet Yee (bsy) at Carnegie-Mellon University
 *	Added GenSymTab
 *
 * 16-Aug-87  Mary Thompson (mrt) at Carnegie-Mellon University
 *	Added CamelotPrefix
 *
 * 28-May-87  Richard Draves (rpd) at Carnegie-Mellon University
 *	Created.
 */

#ifndef	_GLOBAL_H_
#define	_GLOBAL_H_

#define	EXPORT_BOOLEAN
#include <mach/boolean.h>
#include <sys/types.h>
#include "string.h"

extern boolean_t BeQuiet;	/* no warning messages */
extern boolean_t BeVerbose;	/* summarize types, routines */
extern boolean_t UseMsgRPC;
extern boolean_t GenSymTab;

extern boolean_t IsKernel;
extern boolean_t IsCamelot;

extern string_t RCSId;

extern string_t SubsystemName;
extern u_int SubsystemBase;

extern string_t MsgType;
extern string_t WaitTime;
extern string_t ErrorProc;
extern string_t ServerPrefix;
extern string_t UserPrefix;
extern char CamelotPrefix[4];

extern int yylineno;
extern string_t yyinname;

extern void init_global();

extern string_t HeaderFileName;
extern string_t UserFileName;
extern string_t ServerFileName;

extern identifier_t InitRoutineName;
extern identifier_t SetMsgTypeName;
extern identifier_t ReplyPortName;
extern identifier_t ReplyPortIsOursName;
extern identifier_t MsgTypeVarName;
extern identifier_t DeallocPortRoutineName;
extern identifier_t AllocPortRoutineName;
extern identifier_t ServerProcName;

extern void more_global();

extern char NewCDecl[];
extern char LintLib[];

#endif	_GLOBAL_H_
