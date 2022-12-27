%n 5000
%a 20000
%e 10000
%p 25000

Ident		([A-Za-z_][A-Za-z_0-9]*)
Number		([0-9]+)
String		([-/._$A-Za-z0-9]+)
QString		(\"[^"\n]*\")
AString		(\<[^>\n]*\>)
FileName	({QString}|{AString})

%{
/*
/* 
 * Mach Operating System
 * Copyright (c) 1989 Carnegie-Mellon University
 * All rights reserved.  The CMU software License Agreement specifies
 * the terms and conditions for use and redistribution.
 */
/*
 * HISTORY
 * $Log:	lexxer.l,v $
 *
 * 6/15/90 fixed name/number bug, this version only works with STDC. Zon.
 *
NONERevision 1.1  90/02/19  01:14:58  bww
NONE	Mach Release 2.5
NONE	[90/02/19  01:14:23  bww]
NONE
 * Revision 1.5  89/05/20  22:15:00  mrt
 * 	Extensive revamping.  Added polymorphic arguments.
 * 	Allow multiple variable-sized inline arguments in messages.
 * 	[89/04/07            rpd]
 * 
 * 27-May-87  Richard Draves (rpd) at Carnegie-Mellon University
 *	Created.
 */

#include <mach/message.h>
#include "string.h"
#include "type.h"
#include "statement.h"
#include "global.h"
#include "parser.h"
#include "lexxer.h"

#ifdef	LDEBUG
#define RETURN(sym)							\
{									\
    printf("yylex: returning %s (%d)\n", #sym, (sym));			\
    return (sym);							\
}
#else	LDEBUG
#define RETURN(sym)	return (sym)
#endif	LDEBUG
#define	TPRETURN(intypenum, intype, outtypenum, outtype, tsize)		\
{									\
    yylval.symtype.innumber = (intypenum);				\
    yylval.symtype.instr = intype;					\
    yylval.symtype.outnumber = (outtypenum);				\
    yylval.symtype.outstr = outtype;					\
    yylval.symtype.size = (tsize);					\
    RETURN(sySymbolicType);						\
}

#define	TRETURN(typenum, type, tsize)	TPRETURN(typenum,type,typenum,type,tsize)

#define	FRETURN(val)							\
{									\
    yylval.flag = (val);						\
    RETURN(syIPCFlag);							\
}

static struct yysvf *oldYYBegin;

static void doSharp(); /* process body of # directives */
%}

%Start	Normal String FileName QString SkipToEOL

%%

<Normal>[Rr][Oo][Uu][Tt][Ii][Nn][Ee]		RETURN(syRoutine);
<Normal>[Ff][Uu][Nn][Cc][Tt][Ii][Oo][Nn]	RETURN(syFunction);
<Normal>[Pp][Rr][Oo][Cc][Ee][Dd][Uu][Rr][Ee]	RETURN(syProcedure);
<Normal>[Ss][Ii][Mm][Pp][Ll][Ee][Pp][Rr][Oo][Cc][Ee][Dd][Uu][Rr][Ee] RETURN(sySimpleProcedure);
<Normal>[Ss][Ii][Mm][Pp][Ll][Ee][Rr][Oo][Uu][Tt][Ii][Nn][Ee] RETURN(sySimpleRoutine);
<Normal>[Cc][Aa][Mm][Ee][Ll][Oo][Tt][Rr][Oo][Uu][Tt][Ii][Nn][Ee] RETURN(syCamelotRoutine);
<Normal>[Ss][Uu][Bb][Ss][Yy][Ss][Tt][Ee][Mm]	RETURN(sySubsystem);
<Normal>[Mm][Ss][Gg][Tt][Yy][Pp][Ee]		RETURN(syMsgType);
<Normal>[Ww][Aa][Ii][Tt][Tt][Ii][Mm][Ee]	RETURN(syWaitTime);
<Normal>[Nn][Oo][Ww][Aa][Ii][Tt][Tt][Ii][Mm][Ee]	RETURN(syNoWaitTime);
<Normal>[Ii][Nn]				RETURN(syIn);
<Normal>[Oo][Uu][Tt]				RETURN(syOut);
<Normal>[Ii][Nn][Oo][Uu][Tt]			RETURN(syInOut);
<Normal>[Rr][Ee][Qq][Uu][Ee][Ss][Tt][Pp][Oo][Rr][Tt]	RETURN(syRequestPort);
<Normal>[Rr][Ee][Pp][Ll][Yy][Pp][Oo][Rr][Tt]	RETURN(syReplyPort);
<Normal>[Aa][Rr][Rr][Aa][Yy]			RETURN(syArray);
<Normal>[Oo][Ff]				RETURN(syOf);
<Normal>[Ee][Rr][Rr][Oo][Rr]			RETURN(syErrorProc);
<Normal>[Ss][Ee][Rr][Vv][Ee][Rr][Pp][Rr][Ee][Ff][Ii][Xx] RETURN(syServerPrefix);
<Normal>[Uu][Ss][Ee][Rr][Pp][Rr][Ee][Ff][Ii][Xx]	RETURN(syUserPrefix);
<Normal>[Rr][Cc][Ss][Ii][Dd]			RETURN(syRCSId);
<Normal>[Ii][Mm][Pp][Oo][Rr][Tt]		RETURN(syImport);
<Normal>[Uu][Ii][Mm][Pp][Oo][Rr][Tt]		RETURN(syUImport);
<Normal>[Ss][Ii][Mm][Pp][Oo][Rr][Tt]		RETURN(sySImport);
<Normal>[Tt][Yy][Pp][Ee]			RETURN(syType);
<Normal>[Kk][Ee][Rr][Nn][Ee][Ll]		RETURN(syKernel);
<Normal>[Cc][Aa][Mm][Ee][Ll][Oo][Tt]		RETURN(syCamelot);
<Normal>[Ss][Kk][Ii][Pp]			RETURN(sySkip);
<Normal>[Ss][Tt][Rr][Uu][Cc][Tt]		RETURN(syStruct);
<Normal>[Ii][Nn][Tt][Rr][Aa][Nn]		RETURN(syInTran);
<Normal>[Oo][Uu][Tt][Tt][Rr][Aa][Nn]		RETURN(syOutTran);
<Normal>[Dd][Ee][Ss][Tt][Rr][Uu][Cc][Tt][Oo][Rr]	RETURN(syDestructor);
<Normal>[Cc][Tt][Yy][Pp][Ee]				RETURN(syCType);
<Normal>[Cc][Uu][Ss][Ee][Rr][Tt][Yy][Pp][Ee]		RETURN(syCUserType);
<Normal>[Cc][Ss][Ee][Rr][Vv][Ee][Rr][Tt][Yy][Pp][Ee]	RETURN(syCServerType);
<Normal>[Ii][Ss][Ll][Oo][Nn][Gg]		FRETURN(flLong);
<Normal>[Ii][Ss][Nn][Oo][Tt][Ll][Oo][Nn][Gg]	FRETURN(flNotLong);
<Normal>[Dd][Ee][Aa][Ll][Ll][Oo][Cc]		FRETURN(flDealloc);
<Normal>[Nn][Oo][Tt][Dd][Ee][Aa][Ll][Ll][Oo][Cc] FRETURN(flNotDealloc);

<Normal>[Pp][Oo][Ll][Yy][Mm][Oo][Rr][Pp][Hh][Ii][Cc]	TRETURN(MSG_TYPE_POLYMORPHIC,"MSG_TYPE_POLYMORPHIC",32);

<Normal>"MSG_TYPE_UNSTRUCTURED"		TRETURN(MSG_TYPE_UNSTRUCTURED,"MSG_TYPE_UNSTRUCTURED",0);
<Normal>"MSG_TYPE_BIT"			TRETURN(MSG_TYPE_BIT,"MSG_TYPE_BIT",1);
<Normal>"MSG_TYPE_BOOLEAN"		TRETURN(MSG_TYPE_BOOLEAN,"MSG_TYPE_BOOLEAN",32);
<Normal>"MSG_TYPE_INTEGER_16"		TRETURN(MSG_TYPE_INTEGER_16,"MSG_TYPE_INTEGER_16",16);
<Normal>"MSG_TYPE_INTEGER_32"		TRETURN(MSG_TYPE_INTEGER_32,"MSG_TYPE_INTEGER_32",32);
<Normal>"MSG_TYPE_PORT_OWNERSHIP"	TRETURN(MSG_TYPE_PORT_OWNERSHIP,"MSG_TYPE_PORT_OWNERSHIP",32);
<Normal>"MSG_TYPE_PORT_RECEIVE"		TRETURN(MSG_TYPE_PORT_RECEIVE,"MSG_TYPE_PORT_RECEIVE",32);
<Normal>"MSG_TYPE_PORT_ALL"		TRETURN(MSG_TYPE_PORT_ALL,"MSG_TYPE_PORT_ALL",32);
<Normal>"MSG_TYPE_PORT"			TRETURN(MSG_TYPE_PORT,"MSG_TYPE_PORT",32);
<Normal>"MSG_TYPE_CHAR"			TRETURN(MSG_TYPE_CHAR,"MSG_TYPE_CHAR",8);
<Normal>"MSG_TYPE_BYTE"			TRETURN(MSG_TYPE_BYTE,"MSG_TYPE_BYTE",8);
<Normal>"MSG_TYPE_INTEGER_8"		TRETURN(MSG_TYPE_INTEGER_8,"MSG_TYPE_INTEGER_8",8);
<Normal>"MSG_TYPE_REAL"			TRETURN(MSG_TYPE_REAL,"MSG_TYPE_REAL",0);
<Normal>"MSG_TYPE_STRING"		TRETURN(MSG_TYPE_STRING,"MSG_TYPE_STRING",0);
<Normal>"MSG_TYPE_STRING_C"		TRETURN(MSG_TYPE_STRING_C,"MSG_TYPE_STRING_C",0);
<Normal>"MSG_TYPE_PORT_NAME"		TRETURN(MSG_TYPE_PORT_NAME,"MSG_TYPE_PORT_NAME",0);
<Normal>"MSG_TYPE_INTERNAL_MEMORY"	TRETURN(MSG_TYPE_INTERNAL_MEMORY,"MSG_TYPE_INTERNAL_MEMORY",8);
<Normal>"MSG_TYPE_POLYMORPHIC"		TRETURN(MSG_TYPE_POLYMORPHIC,"MSG_TYPE_POLYMORPHIC",0);

<Normal>":"		RETURN(syColon);
<Normal>";"		RETURN(sySemi);
<Normal>","		RETURN(syComma);
<Normal>"+"		RETURN(syPlus);
<Normal>"-"		RETURN(syMinus);
<Normal>"*"		RETURN(syStar);
<Normal>"/"		RETURN(syDiv);
<Normal>"("		RETURN(syLParen);
<Normal>")"		RETURN(syRParen);
<Normal>"="		RETURN(syEqual);
<Normal>"^"		RETURN(syCaret);
<Normal>"~"		RETURN(syTilde);
<Normal>"<"		RETURN(syLAngle);
<Normal>">"		RETURN(syRAngle);
<Normal>"["		RETURN(syLBrack);
<Normal>"]"		RETURN(syRBrack);
<Normal>"|"		RETURN(syBar);

<Normal>{Ident}		{ yylval.identifier = strmake(yytext);
			  RETURN(syIdentifier); }
<Normal>{Number}	{ yylval.number = atoi(yytext); RETURN(syNumber); }

<String>{String}	{ yylval.string = strmake(yytext);
			  BEGIN Normal; RETURN(syString); }
<FileName>{FileName}	{ yylval.string = strmake(yytext);
			  BEGIN Normal; RETURN(syFileName); }
<QString>{QString}	{ yylval.string = strmake(yytext);
			  BEGIN Normal; RETURN(syQString); }

^\#[ \t]*{Number}[ \t]*\"[^"]*\"	{ doSharp(yytext+1);
					  oldYYBegin = yybgin;
					  BEGIN SkipToEOL; }
^\#\ *{Number}				{ doSharp(yytext+1);
					  oldYYBegin = yybgin;
					  BEGIN SkipToEOL; }
^\#					{ yyerror("illegal # directive");
					  oldYYBegin = yybgin;
					  BEGIN SkipToEOL; }

<SkipToEOL>\n		yybgin = oldYYBegin;
<SkipToEOL>.		;

[ \t\n]			;
.			{ BEGIN Normal; RETURN(syError); }

%%

extern void
LookNormal()
{
    BEGIN Normal;
}

extern void
LookString()
{
    BEGIN String;
}

extern void
LookQString()
{
    BEGIN QString;
}

extern void
LookFileName()
{
    BEGIN FileName;
}

static void
doSharp(body)
    char *body;
{
    register char *startName;

    yylineno = atoi(body);
    startName = index(body, '"');
    if (startName != NULL)
    {
	*rindex(body, '"') = '\0';
	strfree(yyinname);
	yyinname = strmake(startName+1);
    }
}
