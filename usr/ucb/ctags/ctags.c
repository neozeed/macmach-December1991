/*
 * Copyright (c) 1987 Regents of the University of California.
 * All rights reserved.  The Berkeley software License Agreement
 * specifies the terms and conditions for redistribution.
 */

#ifndef lint
char copyright[] =
"@(#) Copyright (c) 1987 Regents of the University of California.\n\
 All rights reserved.\n";
#endif not lint

#ifndef lint
static char sccsid[] = "@(#)ctags.c	1.2 (Berkeley) 3/16/87";
#endif not lint

#include "ctags.h"
#include <strings.h>

/*
 * ctags: create a tags file
 */

NODE	*head;			/* head of the sorted binary tree */

				/* boolean "func" (see init()) */
bool	_wht[0177],_etk[0177],_itk[0177],_btk[0177],_gd[0177];

FILE	*inf,			/* ioptr for current input file */
	*outf;			/* ioptr for tags file */

long	lineftell;		/* ftell after getc( inf ) == '\n' */

int	lineno,			/* line number of current line */
	dflag,			/* -d: non-macro defines */
	tflag,			/* -t: create tags for typedefs */
	wflag,			/* -w: suppress warnings */
	vflag,			/* -v: vgrind style index output */
	xflag;			/* -x: cxref style output */

char	*curfile,		/* current input file name */
	searchar = '/',		/* use /.../ searches by default */
	lbuf[BUFSIZ];

main(argc,argv)
	int	argc;
	char	**argv;
{
	extern char	*optarg;		/* getopt arguments */
	extern int	optind;
	static char	*outfile = "tags";	/* output file */
	int	aflag,				/* -a: append to tags */
		uflag,				/* -u: update tags */
		exit_val,			/* exit value */
		step,				/* step through args */
		ch;				/* getopts char */
	char	cmd[100];			/* too ugly to explain */

	aflag = uflag = NO;
	while ((ch = getopt(argc,argv,"BFadf:tuwvx")) != EOF)
		switch((char)ch) {
			case 'B':
				searchar = '?';
				break;
			case 'F':
				searchar = '/';
				break;
			case 'a':
				aflag++;
				break;
			case 'd':
				dflag++;
				break;
			case 'f':
				outfile = optarg;
				break;
			case 't':
				tflag++;
				break;
			case 'u':
				uflag++;
				break;
			case 'w':
				wflag++;
				break;
			case 'v':
				vflag++;
			case 'x':
				xflag++;
				break;
			case '?':
			default:
				goto usage;
		}
	argv += optind;
	argc -= optind;
	if (!argc) {
usage:		puts("Usage: ctags [-BFadtuwvx] [-f tagsfile] file ...");
		exit(1);
	}

	init();

	for (exit_val = step = 0;step < argc;++step)
		if (!(inf = fopen(argv[step],"r"))) {
			perror(argv[step]);
			exit_val = 1;
		}
		else {
			curfile = argv[step];
			find_entries(argv[step]);
			(void)fclose(inf);
		}

	if (head)
		if (xflag)
			put_entries(head);
		else {
			if (uflag) {
				for (step = 0;step < argc;step++) {
					(void)sprintf(cmd,"mv %s OTAGS;fgrep -v '\t%s\t' OTAGS >%s;rm OTAGS",outfile,argv[step],outfile);
					system(cmd);
				}
				++aflag;
			}
			if (!(outf = fopen(outfile, aflag ? "a" : "w"))) {
				perror(outfile);
				exit(exit_val);
			}
			put_entries(head);
			(void)fclose(outf);
			if (uflag) {
				(void)sprintf(cmd,"sort %s -o %s",outfile,outfile);
				system(cmd);
			}
		}
	exit(exit_val);
}

/*
 * init --
 *	this routine sets up the boolean psuedo-functions which work by
 *	setting boolean flags dependent upon the corresponding character.
 *	Every char which is NOT in that string is false with respect to
 *	the pseudo-function.  Therefore, all of the array "_wht" is NO
 *	by default and then the elements subscripted by the chars in
 *	CWHITE are set to YES.  Thus, "_wht" of a char is YES if it is in
 *	the string CWHITE, else NO.
 */
init()
{
	register int	i;
	register char	*sp;

	for (i = 0; i < 0177; i++) {
		_wht[i] = _etk[i] = _itk[i] = _btk[i] = NO;
		_gd[i] = YES;
	}
#define	CWHITE	" \f\t\n"
	for (sp = CWHITE; *sp; sp++)	/* white space chars */
		_wht[*sp] = YES;
#define	CTOKEN	" \t\n\"'#()[]{}=-+%*/&|^~!<>;,.:?"
	for (sp = CTOKEN; *sp; sp++)	/* token ending chars */
		_etk[*sp] = YES;
#define	CINTOK	"ABCDEFGHIJKLMNOPQRSTUVWXYZ_abcdefghijklmnopqrstuvwxyz0123456789"
	for (sp = CINTOK; *sp; sp++)	/* valid in-token chars */
		_itk[*sp] = YES;
#define	CBEGIN	"ABCDEFGHIJKLMNOPQRSTUVWXYZ_abcdefghijklmnopqrstuvwxyz"
	for (sp = CBEGIN; *sp; sp++)	/* token starting chars */
		_btk[*sp] = YES;
#define	CNOTGD	",;"
	for (sp = CNOTGD; *sp; sp++)	/* invalid after-function chars */
		_gd[*sp] = NO;
}

/*
 * find_entries --
 *	this routine opens the specified file and calls the function
 *	which searches the file.
 */
find_entries(file)
	char	*file;
{
	register char	*cp;

	lineno = 0;				/* should be 1 ?? KB */
	if (cp = rindex(file, '.')) {
		if (cp[1] == 'l' && !cp[2]) {
			register int	c;

			for (;;) {
				if (GETC(==,EOF))
					return;
				if (!iswhite(c)) {
					rewind(inf);
					break;
				}
			}
#define	LISPCHR	";(["
/* lisp */		if (index(LISPCHR,(char)c)) {
				l_entries();
				return;
			}
/* lex */		else {
				/*
				 * we search all 3 parts of a lex file
				 * for C references.  This may be wrong.
				 */
				toss_yysec();
				(void)strcpy(lbuf,"%%$");
				pfnote("yylex",lineno);
				rewind(inf);
			}
		}
/* yacc */	else if (cp[1] == 'y' && !cp[2]) {
			/*
			 * we search only the 3rd part of a yacc file
			 * for C references.  This may be wrong.
			 */
			toss_yysec();
			(void)strcpy(lbuf,"%%$");
			pfnote("yyparse",lineno);
			y_entries();
		}
/* fortran */	else if ((cp[1] != 'c' && cp[1] != 'h') && !cp[2]) {
			if (PF_funcs())
				return;
			rewind(inf);
		}
	}
/* C */	c_entries();
}
