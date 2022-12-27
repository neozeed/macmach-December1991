#include "defs.h"
/*
 *
 *	UNIX debugger
 *
 */

#define ADDQ	0x5000		/* addq  #x,	*/
#define ADDL	0xD1FC		/* addl #X,	*/
#define ADDW	0xDEFC		/* addw #X,	*/
#define Scc	0x50C0
#define BSR	0x6100
#define BSRL	0x61FF
#define BSRS	0x6100
#define Bcc	0x6000
#define BccL	0x60FF
#define BccS	0x6000
#define JSRW	0x4EB8
#define JSRL	0x4EB9
#define JSRPC	0x4EBA
#define JSR	0x4E90
#define LEA	0x4FEF

#define TR_ADDQ  1
#define TR_ADDL  2
#define TR_ADDW  3
#define TR_BSRL  4
#define TR_BSR   5
#define TR_BSRS  6
#define TR_BccL  7
#define TR_Bcc   8
#define TR_BccS  9
#define TR_JSRL  10
#define TR_JSRW  11
#define TR_JSRPC 12
#define TR_JSR   13
#define TR_LEA   14

struct backtrop {
    u_int mask;
    u_int inst;
    int index;
    int off;
} backtrops[]  =
{
    0xF1C0, Scc,  0,       0,
    0xF100, ADDQ, TR_ADDQ, 0,
    0xF1FC, ADDL, TR_ADDL, 0,
    0xFFFF, ADDW, TR_ADDW, 0,
    0xFFFF, LEA,  TR_LEA,  0,
    0xFFFF, BSRL, TR_BSRL, 6,
    0xFFFF, BSR,  TR_BSR,  4,
    0xFF00, BSRS, TR_BSRS, 2,
    0xF0FF, BccL, TR_BccL, 0,
    0xF0FF, BccS, TR_Bcc,  0,
    0xF000, Bcc,  TR_BccS, 0,
    0xFFFF, JSRL, TR_JSRL, 6,
    0xFFFF, JSRW, TR_JSRW, 4,
    0xFFFF, JSRPC,TR_JSRPC,4,
    0xFFF0, JSR,  TR_JSR,  2,
    0, 0, 0,
};

findlongjsr(addr, index)
{
    return 0;
}

findop(inst)
int inst;
{
    register struct backtrop *p;

    for (p = backtrops; p->mask; p++) {
	if ((p->mask & inst) == p->inst) {
	    return p->index;
	}
    }
    return 0;
}

struct backtrop *findcallop(addr)
{
    register struct backtrop *p, *curr = 0;
    int off = 0; 
    int inst;

    for (p = backtrops; p->mask; p++) {
	if (p->off > off) {
	    inst = fetch(addr - p->off, ISP, 2);
	    if ((inst & p->mask) == p->inst) {
		curr = p;
		off = p->off;
	    }
	}
    }
    return curr;
}
	    
	    


findcall(loc, entry, calladr, pind)
register long loc;
register long *entry, *calladr, *pind;
{
    struct backtrop *p;

    if ((p = findcallop(loc)) == 0)
	return 0;

    switch (p->index) {
      case TR_JSRL:
	*entry = fetch(loc - 4, ISP, 4);
	break;
      case TR_BSRL:
	*entry = loc - 4 + fetch(loc - 4, ISP, 4);
	break;
      case TR_JSRPC:
	break;
      case TR_BSR:
	*entry = loc - 2 + fetch(loc - 2, ISP, 2);
	break;
      case TR_JSRW:
	*entry = fetch(loc - 2, ISP, 2);
	break;
      case TR_BSRS:
	*entry = loc + (fetch(loc - 2, ISP, 2) & 0xFF);
	break;
      case TR_JSR:
	*pind = (fetch(loc - 2, ISP, 2) & 0x7);
	break;
      default:
	return 0;
    }
    *calladr = loc - p->off;
    return 1;
}

findstackadj(rtn)
{
    register int argn;
    int inst;

    switch(findop(inst = fetch(rtn, ISP, 2))) {

      case TR_ADDQ:
	argn = (inst>>9) & 07;
	if (argn == 0) 
	    argn = 8;
	break;
      case TR_ADDL:
	argn = fetch(rtn + 2, ISP, 4);
	break;
      case TR_ADDW:
      case TR_LEA:
	argn = fetch(rtn + 2, ISP, 2);
	break;
      case TR_Bcc:
	rtn = (short)fetch(rtn + 2, ISP, 2) + rtn + 2;
	argn = findstackadj(rtn);
	break;
      case TR_JSRPC:
	if ((fetch(rtn - 6, ISP, 2) & 0xFFC0) == 0x2E80)
	    argn = 4;
	else
	    argn = 0;
	break;
      default:
	argn = 0;
    }
    return argn;
}

backtrace(link, stack, callpc, cnt)
{
	register long	rtn, p, inst;
	register int	n = 1, i, argn;
	long		calladr, entadr;
	int		tinst, taddr, indir;

	while(cnt--)
	{
		calladr = -1; entadr = -1; indir = -1;
		rtn = stack ? fetch(stack, DSP, 4) : 0;

		/* If no link instruction, return is on top of stack */
		if (rtn && findcall(rtn, &entadr, &calladr, &indir)) {
			p = stack;
			stack += 4;
		} else {
		/* Didn't find call; there must be link instruction */
			p = link;
			link = fetch(p, DSP, 4);
			rtn = fetch(p += 4, DSP, 4);
			if (!findcall(rtn, &entadr, &calladr, &indir))
				break;
		}

		argn = findstackadj(rtn);

		if (argn && (argn % 4)) 
		    argn = (argn/4) + 1;
		else
		    argn /= 4;

		if (calladr == -1)
		    break;

		if (entadr == -1)
		    printc ('?');
		else {
		    findsym(entadr, ISYM);
		    if (cursym)
			printf(cursym->n_un.n_name);
		    else
			printf("%X", entadr);
		}

		printc('(');
		if (argn > 0) {
		    printf("%X", fetch(p += 4, DSP, 4)); 
		    for(i = 1; i < argn; i++)
			printf(", %X", fetch(p += 4, DSP, 4));
		}
		printf (") at ");
	        psymoff(calladr, ISYM, "\n");

		callpc = calladr;
		findsym(calladr, ISYM);
		if ((cursym && !strcmp(cursym->n_un.n_name, "start")) ||
		    (++n > 15)) {
		    break;
		    }

	}
}

fetch(adr, space, size)
{
	register long data;

	data = get(adr, space);
	if (size == 2)
	    data = (data >> 16);
	return(data);
}
