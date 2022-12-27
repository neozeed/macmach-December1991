#ifndef lint
static	char sccsid[] = "@(#)runpcs.c	1.1 (Berkeley) 2/25/86";
#endif
/*
 *
 *	UNIX debugger
 *
 */

#include "defs.h"

extern	MAP	txtmap;

MSG		NOFORK;
MSG		ENDPCS;
MSG		BADWAIT;

CHAR		*lp;
SIG		sigint;
SIG		sigqit;

/* breakpoints */
BKPTR		bkpthead;

REGLIST		reglist[];

CHAR		lastc;

INT		fcor;
INT		fsym;
STRING		errflg;
int		errno;
INT		signo;
INT		sigcode;

L_INT		dot;
STRING		symfil;
INT		wtflag;
L_INT		pid;
L_INT		expv;
INT		adrflg;
L_INT		loopcnt;





/* service routines for sub process control */

getsig(sig)
{	return(expr(0) ? expv : sig);
}

ADDR userpc = 1;

#ifdef mac2
#ifdef SUNCOMPAT
int adjust_pc_flag;
#endif SUNCOMPAT
#endif mac2

runpcs(runmode,execsig)
{
#ifdef mach3
	printf("%s\n", mach3_no_ptrace);
	return 0;
#else
	REG		rc;
	REG BKPTR	bkpt;
	IF adrflg THEN userpc=dot; FI
	printf("%s: running\n", symfil);

	WHILE --loopcnt>=0
	DO
#ifdef DEBUG
		printf("\ncontinue %X %D loopcnt=%D\n",userpc,execsig, loopcnt);
#endif
		IF runmode==SINGLE
		THEN delbp(); /* hardware handles single-stepping */
		ELSE /* continuing from a breakpoint is hard */
			IF bkpt=scanbkpt(userpc)
			THEN execbkpt(bkpt,execsig); execsig=0;
			FI
			setbp();
		FI
		ptrace(runmode,pid,userpc,execsig);
#ifdef mac2
#ifdef SUNCOMPAT
		if (runmode != SINGLE)
		    adjust_pc_flag++;
#endif SUNCOMPAT
#endif mac2
		bpwait(); chkerr(); execsig=0; delbp(); readregs();

		IF (signo==0) ANDF (bkpt=scanbkpt(userpc))
		THEN /* stopped by BPT instruction */
#ifdef DEBUG
			printf("\n BPT code; '%s'%o'%d",
				bkpt->comm,bkpt->comm[0],EOR,bkpt->flag);
#endif
			dot=bkpt->loc;
			IF bkpt->flag==BKPTEXEC
			ORF ((bkpt->flag=BKPTEXEC)
				ANDF bkpt->comm[0]!=EOR
				ANDF command(bkpt->comm,':')
				ANDF --bkpt->count)
			THEN execbkpt(bkpt,execsig); execsig=0; loopcnt++;
			ELSE bkpt->count=bkpt->initcnt; rc=1;
			FI
		ELSE execsig=signo; rc=0;
		FI
	OD
	return(rc);
#endif
}

#define BPOUT 0
#define BPIN 1
INT bpstate = BPOUT;

endpcs()
{
#ifdef mach3
	printf("%s\n", mach3_no_ptrace);
#else
	REG BKPTR	bkptr;
	IF pid
	THEN ptrace(EXIT,pid); pid=0; userpc=1;
	     FOR bkptr=bkpthead; bkptr; bkptr=bkptr->nxtbkpt
	     DO IF bkptr->flag
		THEN bkptr->flag=BKPTSET;
		FI
	     OD
	FI
#ifdef mac2
#ifdef SUNCOMPAT
        adjust_pc_flag = 0;
#endif SUNCOMPAT
#endif mac2
	bpstate=BPOUT;
#endif
}

#ifdef VFORK
nullsig()
{

}
#endif

setup()
{
#ifdef mach3
	printf("%s\n", mach3_no_ptrace);
#else
	close(fsym); fsym = -1;
#ifndef VFORK
	IF (pid = fork()) == 0
#else
	IF (pid = vfork()) == 0
#endif
	THEN ptrace(SETTRC);
#ifdef VFORK
	     signal(SIGTRAP,nullsig);
#endif
	     signal(SIGINT,sigint); signal(SIGQUIT,sigqit);
	     doexec(); exit(0);
	ELIF pid == -1
	THEN error(NOFORK);
	ELSE bpwait(); readregs(); lp[0]=EOR; lp[1]=0;
	     fsym=open(symfil,wtflag);
	     IF errflg
	     THEN printf("%s: cannot execute\n",symfil);
		  endpcs(); error(0);
	     FI
	FI
	bpstate=BPOUT;
#endif
}

execbkpt(bkptr,execsig)
BKPTR	bkptr;
{
#ifdef mach3
	printf("%s\n", mach3_no_ptrace);
#else
#ifdef DEBUG
	printf("exbkpt: %d\n",bkptr->count);
#endif
	delbp();
	ptrace(SINGLE,pid,bkptr->loc,execsig);
	bkptr->flag=BKPTSET;
	bpwait(); chkerr(); readregs();
#endif
}


doexec()
{
	STRING		argl[MAXARG];
	CHAR		args[LINSIZ];
	REG	STRING	p, *ap, filnam;
	extern STRING environ;

	ap=argl; p=args;
	*ap++=symfil;
	REP	IF rdc()==EOR THEN break; FI
		*ap = p;
		WHILE lastc!=EOR ANDF lastc!=SP ANDF lastc!=TB DO *p++=lastc; readchar(); OD
		*p++=0; filnam = *ap+1;
		IF **ap=='<'
		THEN	close(0);
			IF open(filnam,0)<0
			THEN	printf("%s: cannot open\n",filnam); _exit(0);
			FI
		ELIF **ap=='>'
		THEN	close(1);
			IF creat(filnam,0666)<0
			THEN	printf("%s: cannot create\n",filnam); _exit(0);
			FI
		ELSE	ap++;
		FI
	PER lastc!=EOR DONE
	*ap++=0;
	execve(symfil, argl, environ);
	perror(symfil);
}

BKPTR	scanbkpt(adr)
ADDR adr;
{
	REG BKPTR	bkptr;
	FOR bkptr=bkpthead; bkptr; bkptr=bkptr->nxtbkpt
	DO IF bkptr->flag ANDF bkptr->loc==adr
	   THEN break;
	   FI
	OD
	return(bkptr);
}

delbp()
{
#ifdef mach3
	printf("%s\n", mach3_no_ptrace);
#else
	REG ADDR	a;
	REG BKPTR	bkptr;
	IF bpstate!=BPOUT
	THEN
		FOR bkptr=bkpthead; bkptr; bkptr=bkptr->nxtbkpt
		DO	IF bkptr->flag
			THEN a=bkptr->loc;
				IF a < txtmap.e1 THEN
				ptrace(WIUSER,pid,a,bkptr->ins);
				ELSE
				ptrace(WDUSER,pid,a,bkptr->ins);
				FI
			FI
		OD
		bpstate=BPOUT;
	FI
#endif
}

#if defined(vax)
#define	SETBP(ins)	(BPT | ((ins) &~ 0xff))
#else vax
#if defined(mac2)
#define	SETBP(ins)	((BPT<<16) | ((ins) & 0xffff))
#else mac2
#define	SETBP(ins)	((BPT<<24) | ((ins) & 0xffffff))
#endif mac2
#endif vax

setbp()
{
#ifdef mach3
	printf("%s\n", mach3_no_ptrace);
#else
	REG ADDR	a;
	REG BKPTR	bkptr;

	IF bpstate!=BPIN
	THEN
		FOR bkptr=bkpthead; bkptr; bkptr=bkptr->nxtbkpt
		DO IF bkptr->flag
		   THEN a = bkptr->loc;
			IF a < txtmap.e1 THEN
				bkptr->ins = ptrace(RIUSER, pid, a);
				ptrace(WIUSER, pid, a, SETBP(bkptr->ins));
			ELSE
				bkptr->ins = ptrace(RDUSER, pid, a);
				ptrace(WDUSER, pid, a, SETBP(bkptr->ins));
			FI
			IF errno
			THEN printf("cannot set breakpoint: ");
			     psymoff(bkptr->loc,ISYM,"\n");
			FI
		   FI
		OD
		bpstate=BPIN;
	FI
#endif
}

bpwait()
{
	REG w;
	ADDR stat;
#ifdef mach3
	printf("%s\n", mach3_no_ptrace);
#else
	signal(SIGINT, SIG_IGN);
	WHILE (w = wait((union wait *)&stat))!=pid ANDF w != -1 DONE
	signal(SIGINT,sigint);
	IF w == -1
	THEN pid=0;
	     errflg=BADWAIT;
	ELIF (stat & 0177) != 0177
	THEN sigcode = 0;
	     IF signo = stat&0177
	     THEN sigprint();
	     FI
	     IF stat&0200
	     THEN printf(" - core dumped");
		  close(fcor);
		  setcor();
	     FI
#ifdef mac2
	     endpcs();
#endif mac2
	     pid=0;
	     errflg=ENDPCS;
	ELSE signo = stat>>8;
	     sigcode = ptrace(RUREGS, pid, &((struct user *)0)->u_code, 0);
	     IF signo!=SIGTRAP
	     THEN sigprint();
	     ELSE signo=0;
	     FI
	     flushbuf();
	FI
	MAXSTACK = USRSTACK;
	MINSTACK = MAXSTACK - 
	    ctob(ptrace(RUREGS, pid, &((struct user *)0)->u_ssize,   0));
#endif
}

readregs()
{
#ifdef mach3
	printf("%s\n", mach3_no_ptrace);
#else
#ifdef mac2
        ptrace(GETUREGS, pid, adbinfo.userregs, 0);
	ptrace(GETFREGS, pid, &adbinfo.fpregs, 0);
#ifndef SUNCOMPAT
	ptrace(GETSREGS, pid, &adbinfo.userregs[PS], 0);
	userpc = adbinfo.userregs[PC];
#else   SUNCOMPAT

	IF adjust_pc_flag THEN
	   userpc = adbinfo.userregs[PC] -= 2;
	   ptrace(SETUREGS, pid, adbinfo.userregs, 0);
           adjust_pc_flag = 0;
	ELSE
	   userpc = adbinfo.userregs[PC];
        FI

#endif SUNCOMPAT
#else  mac2
	/*get REG values from pcs*/
	REG REGPTR	p;

	FOR p=reglist; p->rname; p++
	DO *(ADDR *)(((ADDR)&u)+p->roffs) = 
		    ptrace(RUREGS, pid, p->roffs, 0);
	OD
 	userpc= *(ADDR *)(((ADDR)&u)+PC);
#endif mac2
#endif
}

#ifdef mac2
writeregs()
{
#ifdef mach3
	printf("%s\n", mach3_no_ptrace);
#else
        ptrace(SETUREGS, pid, adbinfo.userregs, 0);
	ptrace(SETFREGS, pid, &adbinfo.fpregs, 0);
#ifndef SUNCOMPAT
	ptrace(SETSREGS, pid, &adbinfo.userregs[PS], 0);
#endif  SUNCOMPAT
#endif
}
#endif mac2
