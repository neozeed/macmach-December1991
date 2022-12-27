/*	head.h	1.1	86/02/25	*/
/*
 * 21-Sep-89  Zon Williamson (zon) at Carnegie-Mellon University
 *	Changed var from long to L_INT.
 */

ADDR	maxoff;
ADDR	localval;

struct	nlist *symtab, *esymtab;
struct	nlist *cursym;
struct	nlist *lookup();

struct	exec filhdr;

L_INT	var[36];

int	xargc;

MAP	txtmap;
MAP	datmap;
INT	wtflag;
INT	fcor;
INT	fsym;
L_INT	maxfile;
L_INT	maxstor;
INT	signo;

#ifdef mac2

#define NUMREGS 18
#define NUMFPREGS 8
#define NUMFSREGS 3

struct adbinfo {
    int userregs[NUMREGS];
    fp_state_t fpregs;
} adbinfo;

#endif mac2

union {
	struct	user U;
	char	UU[ctob(UPAGES)];
} udot;
#define	u	udot.U

char	*corfil, *symfil;

