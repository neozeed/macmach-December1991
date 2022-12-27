/*
 * These routines are from "a pre-release of a bunch of berkelix
 * regex(3)/ed(1) compatible regular-expression routines" written by Ozan
 * S. Yigit, Dept. of Computer Science, York University.  Parts of the
 * code that are not needed by Prospero have been removed, but most of
 * the accompanying information has been left intact.  This file is to be
 * included on those operating systems that do not support re_comp and
 * re_exec.
 */

/*

These routines are completely public domain. You can do whatever you
like with them, and hopefully you are professional enough not to strip
out the authorship information, acknowledgements and references.

The reason for this being a *pre-release* is that I received a lot
of useful suggestions about packaging, about additional routines etc.
from a few people. I do not have too much time to do those changes
right now, so I am putting this package out for those who needed
it yesterday. Next release will include other routines, and better
packaging.

These routines are *not* tested under SysV, but they are tested
under PRO/Venix (2.0) and BSD 4.2.

In general, these routines run just as fast, or faster than regex library
routines under BSD 4.2. In some cases, they are slightly slower. I did not
try too hard to optimize the re_exec routine.

Coding style is a la K&R, with lotsa short identifiers. I like it
that way. All flames should be fed to yetti!dragon.

Acknowledgements: Henry Spencer, Hugh Redelmeier and Drew Sullivan made
		  a lot of important suggestions, some of which will be
		  incorporated into the next version.

 */

/*
 * regex - Regular expression pattern matching
 *         and replacement
 *
 *
 * By:  Ozan S. Yigit (oz)
 *      Dept. of Computer Science
 *      York University
 *         [...!utzoo!yetti!oz || oz@yusol.BITNET || oz@yuyetti.BITNET]
 *
 * These routines are the PUBLIC DOMAIN equivalents 
 * of regex routines as found in 4.nBSD UN*X, with minor
 * extensions.
 *
 * These routines are derived from various implementations
 * found in software tools books, and Conroy's grep. They
 * are NOT derived from licensed/restricted software.
 * For more interesting/academic/complicated implementations,
 * see Henry Spencer's regexp routines, or GNU Emacs pattern
 * matching module.
 *
 * Routines:
 *      re_comp:        compile a regular expression into
 *                      a DFA.
 *
 *			char *re_comp(s)
 *			char *s;
 *
 *      re_exec:        execute the DFA to match a pattern.
 *
 *			int re_exec(s)
 *			char *s;
 *
 * Regular Expressions:
 *
 *      [1]     char    matches itself, unless it is a special
 *                      character (metachar): . \ [ ] * + ^ $
 *
 *      [2]     .       matches any character.
 *
 *      [3]     \       matches the character following it, except
 *			when followed by a left or right round bracket,
 *			a digit 1 to 9 or a left or right angle bracket. 
 *			(see [7], [8] and [9])
 *			It is used as an escape character for all 
 *			other meta-characters, and itself. When used
 *			in a set ([4]), it is treated as an ordinary
 *			character.
 *
 *      [4]     [set]   matches one of the characters in the set.
 *                      If the first character in the set is "^",
 *                      it matches a character NOT in the set. A
 *                      shorthand S-E is used to specify a set of
 *                      characters S upto E, inclusive. The special
 *                      characters "]" and "-" have no special
 *                      meaning if they appear as the first chars
 *                      in the set.
 *                      examples:        match:
 *
 *                              [a-z]    any lowercase alpha
 *
 *                              [^]-]    any char except ] and -
 *
 *                              [^A-Z]   any char except uppercase
 *                                       alpha
 *
 *                              [a-zA-Z] any alpha
 *
 *      [5]     *       any regular expression form [1] to [4], followed by
 *                      closure char (*) matches zero or more matches of
 *                      that form.
 *
 *      [6]     +       same as [5], except it matches one or more.
 *
 *      [7]             a regular expression in the form [1] to [10], enclosed
 *                      as \(form\) matches what form matches. The enclosure
 *                      creates a set of tags, used for [8] and for
 *                      pattern substution. The tagged forms are numbered
 *			starting from 1.
 *
 *      [8]             a \ followed by a digit 1 to 9 matches whatever a
 *                      previously tagged regular expression ([7]) matched.
 *
 *	[9]	\<	a regular expression starting with a \< construct
 *		\>	and/or ending with a \> construct, restricts the
 *			pattern matching to the beginning of a word, and/or
 *			the end of a word. A word is defined to be a character
 *			string beginning and/or ending with the characters
 *			A-Z a-z 0-9 and _. It must also be preceded and/or
 *			followed by any character outside those mentioned.
 *
 *      [10]            a composite regular expression xy where x and y
 *                      are in the form [1] to [10] matches the longest
 *                      match of x followed by a match for y.
 *
 *      [11]	^	a regular expression starting with a ^ character
 *		$	and/or ending with a $ character, restricts the
 *                      pattern matching to the beginning of the line,
 *                      or the end of line. [anchors] Elsewhere in the
 *			pattern, ^ and $ are treated as ordinary characters.
 *
 *
 * Acknowledgements:
 *
 *	HCR's Hugh Redelmeier has been most helpful in various
 *	stages of development. He convinced me to include BOW
 *	and EOW constructs, originally invented by Rob Pike at
 *	the University of Toronto.
 *
 * References:
 *              Software tools			Kernighan & Plauger
 *              Software tools in Pascal        Kernighan & Plauger
 *              Grep [rsx-11 C dist]            David Conroy
 *		ed - text editor		Un*x Programmer's Manual
 *		Advanced editing on Un*x	B. W. Kernighan
 *		RegExp routines			Henry Spencer
 *
 * Notes:
 *
 *	This implementation uses a bit-set representation for character
 *	classes for speed and compactness. Each character is represented 
 *	by one bit in a 128-bit block. Thus, CCL or NCL always takes a 
 *	constant 16 bytes in the internal dfa, and re_exec does a single
 *	bit comparison to locate the character in the set.
 *
 * Examples:
 *
 *	pattern:	foo*.*
 *	compile:	CHR f CHR o CLO CHR o END CLO ANY END END
 *	matches:	fo foo fooo foobar fobar foxx ...
 *
 *	pattern:	fo[ob]a[rz]	
 *	compile:	CHR f CHR o CCL 2 o b CHR a CCL bitset END
 *	matches:	fobar fooar fobaz fooaz
 *
 *	pattern:	foo\\+
 *	compile:	CHR f CHR o CHR o CHR \ CLO CHR \ END END
 *	matches:	foo\ foo\\ foo\\\  ...
 *
 *	pattern:	\(foo\)[1-3]\1	(same as foo[1-3]foo)
 *	compile:	BOT 1 CHR f CHR o CHR o EOT 1 CCL bitset REF 1 END
 *	matches:	foo1foo foo2foo foo3foo
 *
 *	pattern:	\(fo.*\)-\1
 *	compile:	BOT 1 CHR f CHR o CLO ANY END EOT 1 CHR - REF 1 END
 *	matches:	foo-foo fo-fo fob-fob foobar-foobar ...
 * 
 */

#define MAXDFA  1024
#define MAXTAG  10

#define OKP     1
#define NOP     0

#define CHR     1
#define ANY     2
#define CCL     3
#define NCL     4
#define BOL     5
#define EOL     6
#define BOT     7
#define EOT     8
#define BOW	9
#define EOW	10
#define REF     11
#define CLO     12

#define END     0

/*
 * The following defines are not meant
 * to be changeable. They are for readibility
 * only.
 *
 */
#define MAXCHR	128
#define CHRBIT	8
#define BITBLK	MAXCHR/CHRBIT
#define BLKIND	0170
#define BITIND	07

#define ASCIIB	0177

typedef /*unsigned*/ char CHAR;

static int  tagstk[MAXTAG];             /* subpat tag stack..*/
static CHAR dfa[MAXDFA];		/* automaton..       */
static int  sta = NOP;               	/* status of lastpat */

static CHAR bittab[BITBLK];		/* bit table for CCL */

static int  internal_error;

static void
chset(c) register CHAR c; { bittab[((c)&BLKIND)>>3] |= 1<<((c)&BITIND); }

#define badpat(x)	return(*dfa = END, x)
#define store(x)	*mp++ = x
 
static char *     
re_comp(pat)
char *pat;
{
	register char *p;               /* pattern pointer   */
	register CHAR *mp=dfa;          /* dfa pointer       */
	register CHAR *lp;              /* saved pointer..   */
	register CHAR *sp=dfa;          /* another one..     */

	register int tagi = 0;          /* tag stack index   */
	register int tagc = 1;          /* actual tag count  */

	register int n;
	int c1, c2;
		
	internal_error = 0;

	if (!pat || !*pat)
		if (sta)
			return(0);
		else
			badpat("No previous regular expression");
	sta = NOP;

	for (p = pat; *p; p++) {
		lp = mp;
		switch(*p) {

		case '.':               /* match any char..  */
			store(ANY);
			break;

		case '^':               /* match beginning.. */
			if (p == pat)
				store(BOL);
			else {
				store(CHR);
				store(*p);
			}
			break;

		case '$':               /* match endofline.. */
			if (!*(p+1))
				store(EOL);
			else {
				store(CHR);
				store(*p);
			}
			break;

		case '[':               /* match char class..*/

			if (*++p == '^') {
				store(NCL);
				p++;
			}
			else
				store(CCL);

			if (*p == '-')		/* real dash */
				chset(*p++);
			if (*p == ']')		/* real brac */
				chset(*p++);
			while (*p && *p != ']') {
				if (*p == '-' && *(p+1) && *(p+1) != ']') {
					p++;
					c1 = *(p-2) + 1;
					c2 = *p++;
					while (c1 <= c2)
						chset(c1++);
				}
#ifdef EXTEND
				else if (*p == '\\' && *(p+1)) {
					p++;
					chset(*p++);
				}
#endif
				else
					chset(*p++);
			}
			if (!*p)
				badpat("Missing ]");

			for (n = 0; n < BITBLK; bittab[n++] = (char) 0)
				store(bittab[n]);
	
			break;

		case '*':               /* match 0 or more.. */
		case '+':               /* match 1 or more.. */
			if (p == pat)
				badpat("Empty closure");
			lp = sp;                /* previous opcode */
			if (*lp == CLO)         /* equivalence..   */
				break;
			switch(*lp) {

			case BOL:
			case BOT:
			case EOT:
			case BOW:
			case EOW:
			case REF:
				badpat("Illegal closure");
			default:
				break;
			}

			if (*p == '+')
				for (sp = mp; lp < sp; lp++)
					store(*lp);

			store(END);
			store(END);
			sp = mp;
			while (--mp > lp)
				*mp = mp[-1];
			store(CLO);
			mp = sp;
			break;

		case '\\':              /* tags, backrefs .. */
			switch(*++p) {

			case '(':
				if (tagc < MAXTAG) {
					tagstk[++tagi] = tagc;
					store(BOT);
					store(tagc++);
				}
				else
					badpat("Too many \\(\\) pairs");
				break;
			case ')':
				if (*sp == BOT)
					badpat("Null pattern inside \\(\\)");
				if (tagi > 0) {
					store(EOT);
					store(tagstk[tagi--]);
				}
				else
					badpat("Unmatched \\)");
				break;
			case '<':
				store(BOW);
				break;
			case '>':
				if (*sp == BOW)
					badpat("Null pattern inside \\<\\>");
				store(EOW);
				break;
			case '1':
			case '2':
			case '3':
			case '4':
			case '5':
			case '6':
			case '7':
			case '8':
			case '9':
				n = *p-'0';
				if (tagi > 0 && tagstk[tagi] == n)
					badpat("Cyclical reference");
				if (tagc > n) {
					store(REF);
					store(n);
				}
				else
					badpat("Undetermined reference");
				break;
#ifdef EXTEND
			case 'b':
				store(CHR);
				store('\b');
				break;
			case 'n':
				store(CHR);
				store('\n');
				break;
			case 'f':
				store(CHR);
				store('\f');
				break;
			case 'r':
				store(CHR);
				store('\r');
				break;
			case 't':
				store(CHR);
				store('\t');
				break;
#endif
			default:
				store(CHR);
				store(*p);
			}
			break;

		default :               /* an ordinary char  */
			store(CHR);
			store(*p);
			break;
		}
		sp = lp;
	}
	if (tagi > 0)
		badpat("Unmatched \\(");
	store(END);
	sta = OKP;
	return(0);
}


static char *bol;
static char *bopat[MAXTAG];
static char *eopat[MAXTAG];

char *pmatch();

/*
 * re_exec:
 * 	execute dfa to find a match.
 *
 *	special cases: (dfa[0])	
 *		BOL
 *			Match only once, starting from the
 *			beginning.
 *		CHR
 *			First locate the character without
 *			calling pmatch, and if found, call
 *			pmatch for the remaining string.
 *		END
 *			re_comp failed, poor luser did not
 *			check for it. Fail fast.
 *
 *	If a match is found, bopat[0] and eopat[0] are set
 *	to the beginning and the end of the matched fragment,
 *	respectively.
 *
 */

static int
re_exec(lp)
register char *lp;
{
	register char c;
	register char *ep = 0;
	register CHAR *ap = dfa;

	bol = lp;

	bopat[0] = 0;
	bopat[1] = 0;
	bopat[2] = 0;
	bopat[3] = 0;
	bopat[4] = 0;
	bopat[5] = 0;
	bopat[6] = 0;
	bopat[7] = 0;
	bopat[8] = 0;
	bopat[9] = 0;

	switch(*ap) {

	case BOL:			/* anchored: match from BOL only */
		ep = pmatch(lp,ap);
		break;
	case CHR:			/* ordinary char: locate it fast */
		c = *(ap+1);
		while (*lp && *lp != c)
			lp++;
		if (!*lp)		/* if EOS, fail, else fall thru. */
			return(0);
	default:			/* regular matching all the way. */
		while (*lp) {
			if ((ep = pmatch(lp,ap)))
				break;
			lp++;
		}
		break;
	case END:			/* munged automaton. fail always */
		return(0);
	}
	if (!ep)
		return(0);

	if (internal_error) 
	    return(-1);

	bopat[0] = lp;
	eopat[0] = ep;
	return(1);
}

/* 
 * pmatch: 
 *	internal routine for the hard part
 *
 * 	This code is mostly snarfed from an early
 * 	grep written by David Conroy. The backref and
 * 	tag stuff, and various other mods are by oZ.
 *
 *	special cases: (dfa[n], dfa[n+1])
 *		CLO ANY
 *			We KNOW ".*" will match ANYTHING
 *			upto the end of line. Thus, go to
 *			the end of line straight, without
 *			calling pmatch recursively. As in
 *			the other closure cases, the remaining
 *			pattern must be matched by moving
 *			backwards on the string recursively,
 *			to find a match for xy (x is ".*" and 
 *			y is the remaining pattern) where
 *			the match satisfies the LONGEST match
 *			for x followed by a match for y.
 *		CLO CHR
 *			We can again scan the string forward
 *			for the single char without recursion, 
 *			and at the point of failure, we execute 
 *			the remaining dfa recursively, as
 *			described above.
 *
 *	At the end of a successful match, bopat[n] and eopat[n]
 *	are set to the beginning and end of subpatterns matched
 *	by tagged expressions (n = 1 to 9).	
 *
 */


/*
 * character classification table for word boundary
 * operators BOW and EOW. the reason for not using 
 * ctype macros is that we can let the user add into 
 * our own table. see re_modw. This table is not in
 * the bitset form, since we may wish to extend it
 * in the future for other character classifications. 
 *
 *	TRUE for 0-9 A-Z a-z _
 */
static char chrtyp[MAXCHR] = {
	0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 
	0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 
	0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 
	0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 
	0, 0, 0, 0, 0, 0, 0, 0, 1, 1, 
	1, 1, 1, 1, 1, 1, 1, 1, 0, 0, 
	0, 0, 0, 0, 0, 1, 1, 1, 1, 1, 
	1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 
	1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 
	1, 0, 0, 0, 0, 1, 0, 1, 1, 1, 
	1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 
	1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 
	1, 1, 1, 0, 0, 0, 0, 0
	};

#define inascii(x)	(0177&(x))
#define iswordc(x) 	chrtyp[inascii(x)]
#define isinset(x,y) 	((x)[((y)&BLKIND)>>3] & (1<<((y)&BITIND)))

/*
 * skip values for CLO XXX to skip past the closure
 *
 */

#define ANYSKIP	2 	/* CLO ANY END ...	   */
#define CHRSKIP	3	/* CLO CHR chr END ...	   */
#define CCLSKIP 18	/* CLO CCL 16bytes END ... */

static char *
pmatch(lp, ap)
register char *lp;
register CHAR *ap;
{
	register char *e;		/* extra pointer for CLO */
	register char *bp;		/* beginning of subpat.. */
	register char *ep;		/* ending of subpat..	 */
	register int op, c, n;
	char *are;			/* to save the line ptr. */

	while ((op = *ap++) != END)
		switch(op) {

		case CHR:
			if (*lp++ != *ap++)
				return(0);
			break;
		case ANY:
			if (!*lp++)
				return(0);
			break;
		case CCL:
			c = *lp++;
			if (!isinset(ap,c))
				return(0);
			ap += BITBLK;
			break;
		case NCL:
			c = *lp++;
			if (isinset(ap,c))
				return(0);
			ap += BITBLK;
			break;
		case BOL:
			if (lp != bol)
				return(0);
			break;
		case EOL:
			if (*lp)
				return(0);
			break;
		case BOT:
			bopat[*ap++] = lp;
			break;
		case EOT:
			eopat[*ap++] = lp;
			break;
 		case BOW:
			if (!(lp!=bol && iswordc(lp[-1])) && iswordc(*lp))
				break;
			return(0);
		case EOW:
			if ((lp!=bol && iswordc(lp[-1])) && !iswordc(*lp))
				break;
			return(0);
		case REF:
			n = *ap++;
			bp = bopat[n];
			ep = eopat[n];
			while (bp < ep)
				if (*bp++ != *lp++)
					return(0);
			break;
		case CLO:
			are = lp;
			switch(*ap) {

			case ANY:
				while (*lp)
					lp++;
				n = ANYSKIP;
				break;
			case CHR:
				c = *(ap+1);
				while (*lp && c == *lp)
					lp++;
				n = CHRSKIP;
				break;
			case CCL:
			case NCL:
				while (*lp && (e = pmatch(lp, ap)))
					lp = e;
				n = CCLSKIP;
				break;
			default:
				internal_error++;
				return(0);
			}

			ap += n;

			while (lp >= are) {
				if (e = pmatch(lp, ap))
					return(e);
				--lp;
			}
			return(0);
		default:
			internal_error++;
			return(0);
		}
	return(lp);
}


