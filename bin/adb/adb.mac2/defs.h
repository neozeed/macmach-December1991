/*	defs.h	1.3	86/11/20	*/

/*
 * adb - vax string table version; common definitions
 */

#ifdef mach3
char *mach3_no_ptrace;
#endif

#include <sys/param.h>
#include <sys/dir.h>
#include <machine/psl.h>
#include <machine/reg.h>
#include <sys/user.h>
#include <ctype.h>
#include <a.out.h>

#include "mac.h"
#include "mode.h"
#include "head.h"

#define NSP	0
#define	ISP	1
#define	DSP	2
#define STAR	4

/*
 * Symbol types, used internally in calls to findsym routine.
 * One the VAX this all degenerates since I & D symbols are indistinct.
 * Basically we get NSYM==0 for `=' command, ISYM==DSYM otherwise.
 */
#define NSYM	0
#define DSYM	1		/* Data space symbol */
#define ISYM	DSYM		/* Instruction space symbol == DSYM on VAX */

#define BKPTSET	1
#define BKPTEXEC 2

#ifdef mac2
#define BPT	0x4e4f
#else   mac2
#define BPT	0x30
#define KCALL	0xcf
#define CASEL	0xfc
#define TBIT	0x10
#endif  mac2

#define	SETTRC	0
#define	RIUSER	1
#define	RDUSER	2
#define	RUREGS	3
#define WIUSER	4
#define	WDUSER	5
#define	WUREGS	6
#define	CONTIN	7
#define	EXIT	8
#define SINGLE	9
#define GETSREGS 10
#define SETSREGS 11
#define GETUREGS 12
#define SETUREGS 13
#define GETFREGS 14
#define SETFREGS 15

#define MAXOFF	1024
#define MAXPOS	80
#define MAXLIN	256
#define QUOTE	0200
#define ALIGN	-4

#ifndef vax
#define leng(a)		((long)((unsigned)(a)))
#define shorten(a)	(((a) >> 16) & 0xffff)
#define	itol(a,b)	(((a) << 16) | ((b) & 0xffff))
#define	byte(a)		(((a) >> 24) & 0xff)
#define	btol(a)		((a) << 24)
#else
#define leng(a)		itol(0,a)
#define	shorten(a)	((a) & 0xffff)
#define	itol(a,b)	(((a) & 0xffff) | ((b) << 16))
#define	byte(a)		((a) & 0xff)
#define	btol(a)		(a)
#endif

/* result type declarations */
L_INT		inkdot();
POS		get();
POS		chkget();
STRING		exform();
L_INT		round();
BKPTR		scanbkpt();
VOID		fault();

int s_addr; /* if non-zero, address is symbolic otherwise physical */

#ifndef mach3
struct	pcb	pcb;
#endif
int	kernel;
int	kcore;
struct	pte *sbr;
int	slr;
int	masterpcbb;
