
# line 2 "gram.y"
/*
 * Copyright (c) 1983 Regents of the University of California.
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms are permitted
 * provided that the above copyright notice and this paragraph are
 * duplicated in all such forms and that any documentation,
 * advertising materials, and other materials related to such
 * distribution and use acknowledge that the software was developed
 * by the University of California, Berkeley.  The name of the
 * University may not be used to endorse or promote products derived
 * from this software without specific prior written permission.
 * THIS SOFTWARE IS PROVIDED ``AS IS'' AND WITHOUT ANY EXPRESS OR
 * IMPLIED WARRANTIES, INCLUDING, WITHOUT LIMITATION, THE IMPLIED
 * WARRANTIES OF MERCHANTIBILITY AND FITNESS FOR A PARTICULAR PURPOSE.
 */

#ifndef lint
static char sccsid[] = "@(#)gram.y	5.4 (Berkeley) 6/29/88";
#endif /* not lint */

#include "defs.h"

struct	cmd *cmds = NULL;
struct	cmd *last_cmd;
struct	namelist *last_n;
struct	subcmd *last_sc;


# line 48 "gram.y"
typedef union  {
	int intval;
	char *string;
	struct subcmd *subcmd;
	struct namelist *namel;
} YYSTYPE;
# define EQUAL 1
# define LP 2
# define RP 3
# define SM 4
# define ARROW 5
# define COLON 6
# define DCOLON 7
# define NAME 8
# define STRING 9
# define INSTALL 10
# define NOTIFY 11
# define EXCEPT 12
# define PATTERN 13
# define SPECIAL 14
# define OPTION 15
#define yyclearin yychar = -1
#define yyerrok yyerrflag = 0
extern int yychar;
extern short yyerrflag;
#ifndef YYMAXDEPTH
#define YYMAXDEPTH 150
#endif
YYSTYPE yylval, yyval;
# define YYERRCODE 256

# line 177 "gram.y"


int	yylineno = 1;
extern	FILE *fin;

yylex()
{
	static char yytext[INMAX];
	register int c;
	register char *cp1, *cp2;
	static char quotechars[] = "[]{}*?$";
	
again:
	switch (c = getc(fin)) {
	case EOF:  /* end of file */
		return(0);

	case '#':  /* start of comment */
		while ((c = getc(fin)) != EOF && c != '\n')
			;
		if (c == EOF)
			return(0);
	case '\n':
		yylineno++;
	case ' ':
	case '\t':  /* skip blanks */
		goto again;

	case '=':  /* EQUAL */
		return(EQUAL);

	case '(':  /* LP */
		return(LP);

	case ')':  /* RP */
		return(RP);

	case ';':  /* SM */
		return(SM);

	case '-':  /* -> */
		if ((c = getc(fin)) == '>')
			return(ARROW);
		ungetc(c, fin);
		c = '-';
		break;

	case '"':  /* STRING */
		cp1 = yytext;
		cp2 = &yytext[INMAX - 1];
		for (;;) {
			if (cp1 >= cp2) {
				yyerror("command string too long\n");
				break;
			}
			c = getc(fin);
			if (c == EOF || c == '"')
				break;
			if (c == '\\') {
				if ((c = getc(fin)) == EOF) {
					*cp1++ = '\\';
					break;
				}
			}
			if (c == '\n') {
				yylineno++;
				c = ' '; /* can't send '\n' */
			}
			*cp1++ = c;
		}
		if (c != '"')
			yyerror("missing closing '\"'\n");
		*cp1 = '\0';
		yylval.string = makestr(yytext);
		return(STRING);

	case ':':  /* : or :: */
		if ((c = getc(fin)) == ':')
			return(DCOLON);
		ungetc(c, fin);
		return(COLON);
	}
	cp1 = yytext;
	cp2 = &yytext[INMAX - 1];
	for (;;) {
		if (cp1 >= cp2) {
			yyerror("input line too long\n");
			break;
		}
		if (c == '\\') {
			if ((c = getc(fin)) != EOF) {
				if (any(c, quotechars))
					c |= QUOTE;
			} else {
				*cp1++ = '\\';
				break;
			}
		}
		*cp1++ = c;
		c = getc(fin);
		if (c == EOF || any(c, " \"'\t()=;:\n")) {
			ungetc(c, fin);
			break;
		}
	}
	*cp1 = '\0';
	if (yytext[0] == '-' && yytext[2] == '\0') {
		switch (yytext[1]) {
		case 'b':
			yylval.intval = COMPARE;
			return(OPTION);

		case 'R':
			yylval.intval = REMOVE;
			return(OPTION);

		case 'v':
			yylval.intval = VERIFY;
			return(OPTION);

		case 'w':
			yylval.intval = WHOLE;
			return(OPTION);

		case 'y':
			yylval.intval = YOUNGER;
			return(OPTION);

		case 'h':
			yylval.intval = FOLLOW;
			return(OPTION);

		case 'i':
			yylval.intval = IGNLNKS;
			return(OPTION);
		}
	}
	if (!strcmp(yytext, "install"))
		c = INSTALL;
	else if (!strcmp(yytext, "notify"))
		c = NOTIFY;
	else if (!strcmp(yytext, "except"))
		c = EXCEPT;
	else if (!strcmp(yytext, "except_pat"))
		c = PATTERN;
	else if (!strcmp(yytext, "special"))
		c = SPECIAL;
	else {
		yylval.string = makestr(yytext);
		return(NAME);
	}
	yylval.subcmd = makesubcmd(c);
	return(c);
}

any(c, str)
	register int c;
	register char *str;
{
	while (*str)
		if (c == *str++)
			return(1);
	return(0);
}

/*
 * Insert or append ARROW command to list of hosts to be updated.
 */
insert(label, files, hosts, subcmds)
	char *label;
	struct namelist *files, *hosts;
	struct subcmd *subcmds;
{
	register struct cmd *c, *prev, *nc;
	register struct namelist *h;

	files = expand(files, E_VARS|E_SHELL);
	hosts = expand(hosts, E_ALL);
	for (h = hosts; h != NULL; free(h), h = h->n_next) {
		/*
		 * Search command list for an update to the same host.
		 */
		for (prev = NULL, c = cmds; c!=NULL; prev = c, c = c->c_next) {
			if (strcmp(c->c_name, h->n_name) == 0) {
				do {
					prev = c;
					c = c->c_next;
				} while (c != NULL &&
					strcmp(c->c_name, h->n_name) == 0);
				break;
			}
		}
		/*
		 * Insert new command to update host.
		 */
		nc = ALLOC(cmd);
		if (nc == NULL)
			fatal("ran out of memory\n");
		nc->c_type = ARROW;
		nc->c_name = h->n_name;
		nc->c_label = label;
		nc->c_files = files;
		nc->c_cmds = subcmds;
		nc->c_next = c;
		if (prev == NULL)
			cmds = nc;
		else
			prev->c_next = nc;
		/* update last_cmd if appending nc to cmds */
		if (c == NULL)
			last_cmd = nc;
	}
}

/*
 * Append DCOLON command to the end of the command list since these are always
 * executed in the order they appear in the distfile.
 */
append(label, files, stamp, subcmds)
	char *label;
	struct namelist *files;
	char *stamp;
	struct subcmd *subcmds;
{
	register struct cmd *c;

	c = ALLOC(cmd);
	if (c == NULL)
		fatal("ran out of memory\n");
	c->c_type = DCOLON;
	c->c_name = stamp;
	c->c_label = label;
	c->c_files = expand(files, E_ALL);
	c->c_cmds = subcmds;
	c->c_next = NULL;
	if (cmds == NULL)
		cmds = last_cmd = c;
	else {
		last_cmd->c_next = c;
		last_cmd = c;
	}
}

/*
 * Error printing routine in parser.
 */
yyerror(s)
	char *s;
{
	extern int yychar;

	nerrs++;
	fflush(stdout);
	fprintf(stderr, "rdist: line %d: %s\n", yylineno, s);
}

/*
 * Return a copy of the string.
 */
char *
makestr(str)
	char *str;
{
	register char *cp, *s;

	str = cp = malloc(strlen(s = str) + 1);
	if (cp == NULL)
		fatal("ran out of memory\n");
	while (*cp++ = *s++)
		;
	return(str);
}

/*
 * Allocate a namelist structure.
 */
struct namelist *
makenl(name)
	char *name;
{
	register struct namelist *nl;

	nl = ALLOC(namelist);
	if (nl == NULL)
		fatal("ran out of memory\n");
	nl->n_name = name;
	nl->n_next = NULL;
	return(nl);
}

/*
 * Make a sub command for lists of variables, commands, etc.
 */
struct subcmd *
makesubcmd(type, name)
	int type;
	register char *name;
{
	register char *cp;
	register struct subcmd *sc;

	sc = ALLOC(subcmd);
	if (sc == NULL)
		fatal("ran out of memory\n");
	sc->sc_type = type;
	sc->sc_args = NULL;
	sc->sc_next = NULL;
	sc->sc_name = NULL;
	return(sc);
}
short yyexca[] ={
-1, 1,
	0, -1,
	-2, 0,
	};
# define YYNPROD 24
# define YYLAST 255
short yyact[]={

   6,  38,  37,   4,  44,  24,   3,  16,  21,  12,
  14,  15,  26,  27,  28,  29,  30,   6,  19,  17,
  20,  23,   6,  13,  18,  22,  46,  45,  13,  34,
  35,  36,  31,  32,  43,  40,  39,   9,   7,  10,
  42,   2,  41,   8,   1,  11,  25,  33,   0,   0,
   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,
   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,
   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,
   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,
   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,
   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,
   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,
   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,
   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,
   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,
   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,
   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,
   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,
   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,
   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,
   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,
   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,
   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,
   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,
   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,
   0,   0,   0,   0,   5 };
short yypact[]={

-1000,  -2,-1000,  37,  32,-1000,-1000,  15,  15,  15,
  -1,  16,-1000,-1000,  13,-1000,-1000,-1000,-1000,  15,
  -3,   2,   2,-1000,-1000,-1000,-1000,  15,  15,  15,
  15,   2,   2,  20,  38,  36,  30,  -5,-1000,  23,
-1000,-1000,-1000,-1000,  22,-1000,-1000 };
short yypgo[]={

   0,  47,   8,  46,   1,  45,   2,  44,  41 };
short yyr1[]={

   0,   7,   7,   8,   8,   8,   8,   8,   8,   4,
   4,   5,   5,   2,   2,   3,   3,   3,   3,   3,
   1,   1,   6,   6 };
short yyr2[]={

   0,   0,   2,   3,   4,   6,   4,   6,   1,   1,
   3,   0,   2,   0,   2,   4,   3,   3,   3,   4,
   0,   2,   0,   1 };
short yychk[]={

-1000,  -7,  -8,   8,  -4, 256,   2,   1,   6,   5,
   7,  -5,  -4,   8,  -4,  -4,   8,   3,   8,   5,
   7,  -2,  -2,  -4,   8,  -3,  10,  11,  12,  13,
  14,  -2,  -2,  -1,  -4,  -4,  -4,  -6,  -4,  -6,
  15,   4,   4,   4,   9,   4,   4 };
short yydef[]={

   1,  -2,   2,   9,   0,   8,  11,   0,   0,   0,
   0,   0,   3,   9,   0,  13,  13,  10,  12,   0,
   0,   4,   6,  13,  13,  14,  20,   0,   0,   0,
  22,   5,   7,  22,   0,   0,   0,   0,  23,   0,
  21,  16,  17,  18,   0,  15,  19 };
#ifndef lint
static char yaccpar_sccsid[] = "@(#)yaccpar	4.1	(Berkeley)	2/11/83";
#endif not lint

#
# define YYFLAG -1000
# define YYERROR goto yyerrlab
# define YYACCEPT return(0)
# define YYABORT return(1)

/*	parser for yacc output	*/

#ifdef YYDEBUG
int yydebug = 0; /* 1 for debugging */
#endif
YYSTYPE yyv[YYMAXDEPTH]; /* where the values are stored */
int yychar = -1; /* current input token number */
int yynerrs = 0;  /* number of errors */
short yyerrflag = 0;  /* error recovery flag */

yyparse() {

	short yys[YYMAXDEPTH];
	short yyj, yym;
	register YYSTYPE *yypvt;
	register short yystate, *yyps, yyn;
	register YYSTYPE *yypv;
	register short *yyxi;

	yystate = 0;
	yychar = -1;
	yynerrs = 0;
	yyerrflag = 0;
	yyps= &yys[-1];
	yypv= &yyv[-1];

 yystack:    /* put a state and value onto the stack */

#ifdef YYDEBUG
	if( yydebug  ) printf( "state %d, char 0%o\n", yystate, yychar );
#endif
		if( ++yyps> &yys[YYMAXDEPTH] ) { yyerror( "yacc stack overflow" ); return(1); }
		*yyps = yystate;
		++yypv;
		*yypv = yyval;

 yynewstate:

	yyn = yypact[yystate];

	if( yyn<= YYFLAG ) goto yydefault; /* simple state */

	if( yychar<0 ) if( (yychar=yylex())<0 ) yychar=0;
	if( (yyn += yychar)<0 || yyn >= YYLAST ) goto yydefault;

	if( yychk[ yyn=yyact[ yyn ] ] == yychar ){ /* valid shift */
		yychar = -1;
		yyval = yylval;
		yystate = yyn;
		if( yyerrflag > 0 ) --yyerrflag;
		goto yystack;
		}

 yydefault:
	/* default state action */

	if( (yyn=yydef[yystate]) == -2 ) {
		if( yychar<0 ) if( (yychar=yylex())<0 ) yychar = 0;
		/* look through exception table */

		for( yyxi=yyexca; (*yyxi!= (-1)) || (yyxi[1]!=yystate) ; yyxi += 2 ) ; /* VOID */

		while( *(yyxi+=2) >= 0 ){
			if( *yyxi == yychar ) break;
			}
		if( (yyn = yyxi[1]) < 0 ) return(0);   /* accept */
		}

	if( yyn == 0 ){ /* error */
		/* error ... attempt to resume parsing */

		switch( yyerrflag ){

		case 0:   /* brand new error */

			yyerror( "syntax error" );
		yyerrlab:
			++yynerrs;

		case 1:
		case 2: /* incompletely recovered error ... try again */

			yyerrflag = 3;

			/* find a state where "error" is a legal shift action */

			while ( yyps >= yys ) {
			   yyn = yypact[*yyps] + YYERRCODE;
			   if( yyn>= 0 && yyn < YYLAST && yychk[yyact[yyn]] == YYERRCODE ){
			      yystate = yyact[yyn];  /* simulate a shift of "error" */
			      goto yystack;
			      }
			   yyn = yypact[*yyps];

			   /* the current yyps has no shift onn "error", pop stack */

#ifdef YYDEBUG
			   if( yydebug ) printf( "error recovery pops state %d, uncovers %d\n", *yyps, yyps[-1] );
#endif
			   --yyps;
			   --yypv;
			   }

			/* there is no state on the stack with an error shift ... abort */

	yyabort:
			return(1);


		case 3:  /* no shift yet; clobber input char */

#ifdef YYDEBUG
			if( yydebug ) printf( "error recovery discards char %d\n", yychar );
#endif

			if( yychar == 0 ) goto yyabort; /* don't discard EOF, quit */
			yychar = -1;
			goto yynewstate;   /* try again in the same state */

			}

		}

	/* reduction by production yyn */

#ifdef YYDEBUG
		if( yydebug ) printf("reduce %d\n",yyn);
#endif
		yyps -= yyr2[yyn];
		yypvt = yypv;
		yypv -= yyr2[yyn];
		yyval = yypv[1];
		yym=yyn;
			/* consult goto table to find next state */
		yyn = yyr1[yyn];
		yyj = yypgo[yyn] + *yyps + 1;
		if( yyj>=YYLAST || yychk[ yystate = yyact[yyj] ] != -yyn ) yystate = yyact[yypgo[yyn]];
		switch(yym){
			
case 3:
# line 66 "gram.y"
 {
			(void) lookup(yypvt[-2].string, INSERT, yypvt[-0].namel);
		} break;
case 4:
# line 69 "gram.y"
 {
			insert(NULL, yypvt[-3].namel, yypvt[-1].namel, yypvt[-0].subcmd);
		} break;
case 5:
# line 72 "gram.y"
 {
			insert(yypvt[-5].string, yypvt[-3].namel, yypvt[-1].namel, yypvt[-0].subcmd);
		} break;
case 6:
# line 75 "gram.y"
 {
			append(NULL, yypvt[-3].namel, yypvt[-1].string, yypvt[-0].subcmd);
		} break;
case 7:
# line 78 "gram.y"
 {
			append(yypvt[-5].string, yypvt[-3].namel, yypvt[-1].string, yypvt[-0].subcmd);
		} break;
case 9:
# line 84 "gram.y"
 {
			yyval.namel = makenl(yypvt[-0].string);
		} break;
case 10:
# line 87 "gram.y"
 {
			yyval.namel = yypvt[-1].namel;
		} break;
case 11:
# line 92 "gram.y"
{
			yyval.namel = last_n = NULL;
		} break;
case 12:
# line 95 "gram.y"
 {
			if (last_n == NULL)
				yyval.namel = last_n = makenl(yypvt[-0].string);
			else {
				last_n->n_next = makenl(yypvt[-0].string);
				last_n = last_n->n_next;
				yyval.namel = yypvt[-1].namel;
			}
		} break;
case 13:
# line 106 "gram.y"
{
			yyval.subcmd = last_sc = NULL;
		} break;
case 14:
# line 109 "gram.y"
 {
			if (last_sc == NULL)
				yyval.subcmd = last_sc = yypvt[-0].subcmd;
			else {
				last_sc->sc_next = yypvt[-0].subcmd;
				last_sc = yypvt[-0].subcmd;
				yyval.subcmd = yypvt[-1].subcmd;
			}
		} break;
case 15:
# line 120 "gram.y"
 {
			register struct namelist *nl;

			yypvt[-3].subcmd->sc_options = yypvt[-2].intval | options;
			if (yypvt[-1].namel != NULL) {
				nl = expand(yypvt[-1].namel, E_VARS);
				if (nl->n_next != NULL)
					yyerror("only one name allowed\n");
				yypvt[-3].subcmd->sc_name = nl->n_name;
				free(nl);
			}
			yyval.subcmd = yypvt[-3].subcmd;
		} break;
case 16:
# line 133 "gram.y"
 {
			if (yypvt[-1].namel != NULL)
				yypvt[-2].subcmd->sc_args = expand(yypvt[-1].namel, E_VARS);
			yyval.subcmd = yypvt[-2].subcmd;
		} break;
case 17:
# line 138 "gram.y"
 {
			if (yypvt[-1].namel != NULL)
				yypvt[-2].subcmd->sc_args = expand(yypvt[-1].namel, E_ALL);
			yyval.subcmd = yypvt[-2].subcmd;
		} break;
case 18:
# line 143 "gram.y"
 {
			struct namelist *nl;
			char *cp, *re_comp();

			for (nl = yypvt[-1].namel; nl != NULL; nl = nl->n_next)
				if ((cp = re_comp(nl->n_name)) != NULL)
					yyerror(cp);
			yypvt[-2].subcmd->sc_args = expand(yypvt[-1].namel, E_VARS);
			yyval.subcmd = yypvt[-2].subcmd;
		} break;
case 19:
# line 153 "gram.y"
 {
			if (yypvt[-2].namel != NULL)
				yypvt[-3].subcmd->sc_args = expand(yypvt[-2].namel, E_ALL);
			yypvt[-3].subcmd->sc_name = yypvt[-1].string;
			yyval.subcmd = yypvt[-3].subcmd;
		} break;
case 20:
# line 161 "gram.y"
 {
			yyval.intval = 0;
		} break;
case 21:
# line 164 "gram.y"
 {
			yyval.intval |= yypvt[-0].intval;
		} break;
case 22:
# line 169 "gram.y"
 {
			yyval.namel = NULL;
		} break;
case 23:
# line 172 "gram.y"
 {
			yyval.namel = yypvt[-0].namel;
		} break;
		}
		goto yystack;  /* stack new state and value */

	}
