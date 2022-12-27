#ifndef lint
static	char sccsid[] = "@(#)print.c	1.2 (Berkeley) 7/25/86";
#endif
/*
 *
 *	UNIX debugger
 *
 */
#include "defs.h"

MSG		LONGFIL;
MSG		NOTOPEN;
MSG		BADMOD;

MAP		txtmap;
MAP		datmap;

ADDR		lastframe;
ADDR		callpc;

INT		infile;
INT		outfile;
INT		fcor;
CHAR		*lp;
INT		maxpos;
ADDR		maxoff;
INT		radix;

/* symbol management */
ADDR		localval;

/* breakpoints */
BKPTR		bkpthead;

REGLIST reglist[] =
{
		"d0",  0, 0,
		"d1",  1, 0,
		"d2",  2, 0,
		"d3",  3, 0,
		"d4",  4, 0,
		"d5",  5, 0,
		"d6",  6, 0,
		"d7",  7, 0,
		"a0",  8, 0,
		"a1",  9, 0,
		"a2", 10, 0,
		"a3", 11, 0,
		"a4", 12, 0,
		"a5", 13, 0,
		"a6", 14, 0,
		"sp", 15, 0,
		"ps", 16, 0,
		"pc", 17, 0,
		0,
};

# ifdef M68881
REGLIST fpreglist[] =
{
		"fp0", 0, 0,
		"fp1", 1, 0,
		"fp2", 2, 0,
		"fp3", 3, 0,
		"fp4", 4, 0,
		"fp5", 5, 0,
		"fp6", 6, 0,
		"fp7", 7, 0,
		0,
};

REGLIST fsreglist[] =
{
		"fpcr", 0, 0,
		"fpsr", 1, 0,
		"fpiar",2, 0,
		0,
};
# endif M68881

CHAR		lastc;

INT		fcor;
STRING		errflg;
INT		signo;
INT		sigcode;

L_INT		dot;
L_INT		var[];
STRING		symfil;
STRING		corfil;
L_INT		pid;
L_INT		adrval;
INT		adrflg;
L_INT		cntval;
INT		cntflg;

STRING		signals[] = {
		"",
		"hangup",
		"interrupt",
		"quit",
		"illegal instruction",
		"trace/BPT",
		"IOT",
		"EMT",
		"floating exception",
		"killed",
		"bus error",
		"memory fault",
		"bad system call",
		"broken pipe",
		"alarm call",
		"terminated",
		"signal 16",
		"stop (signal)",
		"stop (tty)",
		"continue (signal)",
		"child termination",
		"stop (tty input)",
		"stop (tty output)",
		"input available (signal)",
		"cpu timelimit",
		"file sizelimit",
		"signal 26",
		"signal 27",
		"signal 28",
		"signal 29",
		"signal 30",
		"signal 31",
};

printtrace(modif)
{
	REG		narg, i;
	REG BKPTR	bkptr;
	REG	STRING	comptr;
	ADDR		frame, callsp;
	register struct nlist *sp;
	INT		stack;


	IF cntflg==0 THEN cntval = -1; FI

	switch (modif) {

	    case '<':
		IF cntval == 0
		THEN	WHILE readchar() != EOR
			DO OD
			lp--;
			break;
		FI
		IF rdc() == '<'
		THEN	stack = 1;
		ELSE	stack = 0; lp--;
		FI
							/* fall through */
	    case '>':
		{CHAR		file[64];
		CHAR		Ifile[128];
		extern CHAR	*Ipath;
		INT		index;

		index=0;
		IF modif=='<'
		THEN	iclose(stack, 0);
		ELSE	oclose();
		FI
		IF rdc()!=EOR
		THEN	REP file[index++]=lastc;
			    IF index>=63 THEN error(LONGFIL); FI
			PER readchar()!=EOR DONE
			file[index]=0;
			IF modif=='<'
			THEN	IF Ipath THEN
					strcpy(Ifile, Ipath);
					strcat(Ifile, "/");
					strcat(Ifile, file);
				FI
				infile=open(file,0);
				IF infile<0 && (infile=open(Ifile,0))<0
				THEN	infile=0; error(NOTOPEN);
				ELSE	IF cntflg
					THEN	var[9] = cntval;
					ELSE	var[9] = 1;
					FI
				FI
			ELSE	outfile=open(file,1);
				IF outfile<0
				THEN	outfile=creat(file,0644);
#ifndef EDDT
				ELSE	lseek(outfile,0L,2);
#endif
				FI
			FI

		ELSE	IF modif == '<'
			THEN	iclose(-1, 0);
			FI
		FI
		lp--;
		}
		break;

	    case 'd':
		if (adrflg) {
			if (!(adrval>=2 && adrval<=16 || adrval<=-2 && adrval>=-16)) {
				printf("illegal radix %d base ten",radix);
				break;
			}
			radix=adrval;
		}
		printf("radix=%d base ten",radix);
		break;

	    case 'q': case 'Q': case '%':
		done();

	    case 'w': case 'W':
		maxpos=(adrflg?adrval:MAXPOS);
		break;

	    case 's': case 'S':
		maxoff=(adrflg?adrval:MAXOFF);
		break;

	    case 'v': case 'V':
		printf("variables\n");
		FOR i=0;i<=35;i++
		DO IF var[i]
		   THEN printc((i<=9 ? '0' : 'a'-10) + i);
			printf(" = %R\n",var[i]);
		   FI
		OD
		break;

	    case 'm': case 'M':
		printmap("? map",&txtmap);
		printmap("/ map",&datmap);
		break;

	    case 0: case '?':
		IF pid
		THEN printf("pcs id = %d\n",pid);
		ELSE printf("no process\n");
		FI
		sigprint(); flushbuf();

	    case 'r': case 'R':
		if (pid || (fcor != -1)) 
		    printregs();
		else printf("no process or core image\n");
		return;

# ifdef M68881
	    case 'f': case 'F':
		if (pid || (fcor != -1)) 
		    printfregs();
		else printf("no process or core image\n");
		return;
# endif M68881

	    case 'c': case 'C':

		if (!pid && (fcor  == -1)) {
		    printf ("no process or core image\n");
		    return;
		}
#ifdef mac2
		IF adrflg
		THEN    frame=adrval;
			callpc=get(frame-4, DSP);
			callsp=0;
		ELSE
		        frame = adbinfo.userregs[FP];
			callpc = adbinfo.userregs[PC];
			callsp = adbinfo.userregs[AR7];
		FI

	        printpc();
		backtrace(frame, callsp, callpc, cntval);
#endif mac2
		break;

	    /*print externals*/
	    case 'e': case 'E':
		for (sp = symtab; sp < esymtab; sp++) {
		   if (sp->n_type==(N_DATA|N_EXT) ORF sp->n_type==(N_BSS|N_EXT))
		   	printf("%s:%12t%R\n", sp->n_un.n_name, get(sp->n_value,DSP));
		}
		break;

	    /*print breakpoints*/
	    case 'b': case 'B':
		printf("breakpoints\ncount%8tbkpt%24tcommand\n");
		for (bkptr=bkpthead; bkptr; bkptr=bkptr->nxtbkpt)
			if (bkptr->flag) {
		   		printf("%-8.8d",bkptr->count);
				psymoff(bkptr->loc,ISYM,"%24t");
				comptr=bkptr->comm;
				WHILE *comptr DO printc(*comptr++); OD
			}
		break;

	    default: error(BADMOD);
	}

}

printmap(s,amap)
STRING	s; REG	MAP *amap;
{
	int file;
	file=amap->ufd;
	printf("%s%12t`%s'\n",s,(file<0 ? "-" : (file==fcor ? corfil : symfil)));
	printf("b1 = %-16R",amap->b1);
	printf("e1 = %-16R",amap->e1);
	printf("f1 = %-16R",amap->f1);
	printf("\nb2 = %-16R",amap->b2);
	printf("e2 = %-16R",amap->e2);
	printf("f2 = %-16R",amap->f2);
	printc(EOR);
}

printregs()
{
	REG REGPTR	p;
	ADDR		v;

	FOR p=reglist; p->rname; p++
	DO
#ifdef mac2
	        v = kcore ? *p->rkern  : adbinfo.userregs[p->roffs];
#else  mac2
		v = kcore ? *p->rkern : *(ADDR *)(((ADDR)&u)+p->roffs);
#endif mac2
		printf("%s%6t%R %16t", p->rname, v);
		valpr(v,(p->roffs==PC?ISYM:DSYM));
		printc(EOR);
	OD
	printpc();
}

getreg(regnam)
{
	REG REGPTR	p;
	REG STRING	regptr;
	CHAR	*olp;

	olp=lp;
	FOR p=reglist; p->rname; p++
	DO	regptr=p->rname;
		IF (regnam == *regptr++)
		THEN
			WHILE *regptr
			DO IF readchar() != *regptr++
				THEN --regptr; break;
				FI
			OD
			IF *regptr
			THEN lp=olp;
			ELSE return(kcore ? (int)p->rkern : (int)p->roffs);
			FI
		FI
	OD
	lp=olp;
	return(-1);
}

# ifdef M68881
/*
 *	This routine decodes the extended floating-point format.
 *	Parts of it probably should be redone in assembly language.
 */
fdecode(f)
fp_reg_t *f;
{
	union {
		struct {
			unsigned sign:1;
			unsigned exp:11;
			unsigned man1:20;
			unsigned man2:32;
		} ds;
		double d;
		unsigned un[2];
	} du;
	short bigexp;

	du.ds.sign = (f->fp[0] & 0x80000000) >> 32;
	bigexp = (f->fp[0] & 0x7fffffff) >> 16;
	if (bigexp == 0x7fff) {
		if (f->fp[1] || f->fp[2]) {
			printf("\t(NaN)");
			return;
		} else {
			if (du.ds.sign) printf("\t(-Inf)");
			else printf("\t(Inf)");
			return;
		}
	}
	bigexp -= (0x3fff-0x3ff);
	if (bigexp > 0x7fe) {
		printf("\t(???)");
		return;
	}
	if (bigexp < 0) {
		printf("\t(0.)");
		return;
	}
	du.ds.exp = bigexp;
	du.ds.man1 = (f->fp[1] & 0x7fffffff) >> 11;
	du.ds.man2 = (f->fp[2] & 0x000007ff) << 21;
	du.ds.man2 |= ((f->fp[2] & 0xfffff800) >> 11) & 0x001fffff;
	if ((f->fp[2]  & 0x400) && (f->fp[2] & 0xB00))
		if (++du.ds.man2 == 0)
			if (++du.ds.man1== 0)
				++du.ds.exp;
/*	printf("\t%X %X\t", du.un[0], du.un[1]);	/* DEBUG */
	printf("\t(%.18F)", du.d);
}

printfregs()
{
        REG REGPTR	p;
	REG INT		i;

	FOR p=fpreglist; p->rname; p++
	DO
		fp_reg_t *f;

	        f = kcore ? (fp_reg_t *)p->rkern : &adbinfo.fpregs.fps_regs[p->roffs];
		printf("%s\t0x", p->rname);
	        printf("%08.8X%08.8X%08.8X", f->fp[0], f->fp[1], f->fp[2]);
		fdecode(f);
		printf("\n");
	OD

	printf("cntl\t%X\n",   adbinfo.fpregs.fps_control);
	printf("status\t%X\n", adbinfo.fpregs.fps_status);
	printf("iaddr\t%X\n",  adbinfo.fpregs.fps_iaddr);
	printf("\n");
}
# endif M68881

printpc()
{
#ifdef mac2
	dot= adbinfo.userregs[PC];
#else  mac2
	dot= *(ADDR *)(((ADDR)(&u))+PC);
#endif mac2
	psymoff(dot, ISYM, ":");
	printins(ISP, chkget(dot, ISP));
	printc(EOR);
}

char	*illinames[] = {
	"reserved addressing fault",
	"priviliged instruction fault",
	"reserved operand fault"
};
char	*fpenames[] = {
	0,
	"ftrap instruction trap",
	"f-line emulation trap",
	"coprocessor protocol violation",
	"floating point branch or set on unordered condition",
	"floating point inexact result",
	"floating point divide by zero",
	"floating point underflow",
	"floating point operand error",
	"floating point overflow",
	"floating point not a number",
};

sigprint()
{
	IF (signo>=0) ANDF (signo<sizeof signals/sizeof signals[0])
	THEN printf(signals[signo]); FI
	switch (signo) {

#ifdef M68881
	case SIGFPE:
	        IF  (sigcode == 0x1C) || (sigcode == 0x2C) || (sigcode == 0x34) THEN
		    sigcode >>= 4;
		ELIF (sigcode >= 0xC0) && (sigcode <= 0xD8) THEN
		    sigcode = (sigcode/4) - 44;
		ELSE sigcode = 0;
		FI

		IF (sigcode > 0 &&
		    sigcode < sizeof fpenames / sizeof fpenames[0]) THEN
			printf(" ("); printf(fpenames[sigcode]); printc(')');
		FI
		break;
#endif M68881

	case SIGILL:
		IF (sigcode >= 0 &&
		    sigcode < sizeof illinames / sizeof illinames[0]) THEN
			printf(" ("); printf(illinames[sigcode]); printc(')');
		FI
		break;
	}
}
