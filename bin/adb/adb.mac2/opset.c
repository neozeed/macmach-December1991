/****************************************************************************

	DEBUGGER - 68020 instruction printout

****************************************************************************/
#include "defs.h"

INT	dotinc;
POS	space;

char *badop = "\t???";
char *IMDF;				/* immediate data format */

char *bname[16] = { "ra", "sr", "hi", "ls", "cc", "cs", "ne",
		    "eq", "vc", "vs", "pl", "mi", "ge", "lt", "gt", "le" };

char *sccname[16] = { "t", "f", "hi", "ls", "cc", "cs", "ne",
		    "eq", "vc", "vs", "pl", "mi", "ge", "lt", "gt", "le" };

char *dbname[16] = { "t ", "ra", "hi", "ls", "cc", "cs", "ne",
		    "eq", "vc", "vs", "pl", "mi", "ge", "lt", "gt", "le" };

char *fbname[32] = { "f", "eq", "ogt", "oge", "olt", "ole", "ogl", "or",
		    "un", "ueq", "ugt", "uge", "ult", "ule", "neq", "t",
		    "sf", "seq", "gt", "ge", "lt", "le", "gl", "gle",
		    "ngle", "ngl", "nle", "nlt", "nge", "ngt", "sneq", "st" };

char *pbname[17] = { "bs", "bc", "ls", "lc", "ss", "sc", "as", "ac", "ws",
		    "wc", "is", "ic", "gs", "gc", "cs", "cc", "???" };

char *shro[4] = { "as", "ls", "rox", "ro" };

char *bit[4] = { "btst", "bchg", "bclr", "bset" };

char *creg[] = { "sfc", "dfc", "cacr" };
char *creg2[] = { "usp", "vbr", "caar", "msp", "isp" };
char *preg[] = {"tc", "drp", "srp", "crp", "cal", "val", "scc", "ac" };
int spreg[] = {4, 8, 8, 8, 1, 1, 1, 2};

int omove(),obranch(),oimmed(),oprint(),oneop(),soneop(),oreg(),ochk();
int olink(),omovem(),oquick(),omoveq(),otrap(),odbcc(),oscc(),opmode(),shroi();
int extend(),biti(),omovec(),omoves(),ortd(),ochk2(),obf(),obkpt(),ocas();
int ocallm(),omuldiv(),oaoccsr(),omvccsr();
int fop(), fmost(), fmov(), fmovmcr(), fmovm();
int cpcc(), cpsres(), cpbcc(), cpgen();
int pop(), pvalid(), pvalid2(), pload(), pflush(), pmove(), pmove2();
int ptest(), pflushr();

struct opdesc
{			
	int mask, match;
	int (*opfun)();
	char *farg;
} opdecode[] =
{					/* order is important below */
  0xF000, 0x1000, omove, "b",		/* move instructions */
  0xF000, 0x2000, omove, "l",
  0xF000, 0x3000, omove, "w",
  0xF000, 0x6000, obranch, 0,		/* branches */
  0xFFFF, 0x003C, oaoccsr, "orb",	/* ori to ccr */
  0xFFFF, 0x007C, oaoccsr, "orw",	/* ori to sr */
  0xFFFF, 0x023C, oaoccsr, "andb",	/* andi to ccr */
  0xFFFF, 0x027C, oaoccsr, "andw",	/* andi to sr */
  0xFFC0, 0x06C0, ocallm, 0,
  0xF9C0, 0x00C0, ochk2, 0,
  0xFFC0, 0x0AC0, ocas, "b",		/* Need three of these */
  0xFFC0, 0x0CC0, ocas, "w",		/* to distinguish them */
  0xFFC0, 0x0EC0, ocas, "l",		/* from static bit instructions*/
  0xFF00, 0x0000, oimmed, "or",		/* op class 0  */
  0xFF00, 0x0200, oimmed, "and",
  0xFF00, 0x0400, oimmed, "sub",
  0xFF00, 0x0600, oimmed, "add",
  0xFF00, 0x0A00, oimmed, "eor",
  0xFF00, 0x0C00, oimmed, "cmp",
  0xFF00, 0x0E00, omoves, 0,
  0xF100, 0x0100, biti, 0,
  0xF800, 0x0800, biti, 0,
  0xFFC0, 0x40C0, omvccsr, "sr",	/* move from sr */
  0xFF00, 0x4000, soneop, "negx",
  0xFFC0, 0x42C0, omvccsr, "cc",	/* move from ccr */
  0xFF00, 0x4200, soneop, "clr",
  0xFFC0, 0x44C0, omvccsr, "cc",	/* move to ccr */
  0xFF00, 0x4400, soneop, "neg",
  0xFFC0, 0x46C0, omvccsr, "sr",	/* move to sr */
  0xFF00, 0x4600, soneop, "not",
  0xFFF8, 0x4808, olink, "l",
  0xFFC0, 0x4800, oneop, "nbcd",
  0xFFF8, 0x4840, oreg, "\tswap\td%D",
  0xFFF8, 0x4848, obkpt, 0,
  0xFFC0, 0x4840, oneop, "pea",
  0xFFF8, 0x4880, oreg, "\text.w\td%D",
  0xFFF8, 0x48C0, oreg, "\textl\td%D",
  0xFB80, 0x4880, omovem, 0,
  0xFFF8, 0x49C0, oreg, "\textbl\td%D",
  0xFFC0, 0x4AC0, oneop, "tas",
  0xFF00, 0x4A00, soneop, "tst",
  0xFFC0, 0x4C00, omuldiv, "mul",
  0xFFC0, 0x4C40, omuldiv, "div",
  0xFFF0, 0x4E40, otrap, 0,
  0xFFF8, 0x4E50, olink, "w",
  0xFFF8, 0x4E58, oreg, "\tunlk\ta%D",
  0xFFF8, 0x4E60, oreg, "\tmovl\ta%D,usp",
  0xFFF8, 0x4E68, oreg, "\tmovl\tusp,a%D",
  0xFFFF, 0x4E70, oprint, "reset",
  0xFFFF, 0x4E71, oprint, "nop",
  0xFFFF, 0x4E72, oprint, "stop",
  0xFFFF, 0x4E73, oprint, "rte",
  0xFFFF, 0x4E74, ortd, 0,
  0xFFFF, 0x4E75, oprint, "rts",
  0xFFFF, 0x4E76, oprint, "trapv",
  0xFFFF, 0x4E77, oprint, "rtr",
  0xFFFE, 0x4E7A, omovec, 0,
  0xFFC0, 0x4E80, oneop, "jsr",
  0xFFC0, 0x4EC0, oneop, "jmp",
  0xF1C0, 0x4100, ochk, "chkw",
  0xF1C0, 0x4180, ochk, "chkl",
  0xF1C0, 0x41C0, ochk, "lea",
  0xF0F8, 0x50C8, odbcc, 0,
  0xF0C0, 0x50C0, oscc, 0,
  0xF100, 0x5000, oquick, "addq",
  0xF100, 0x5100, oquick, "subq",
  0xF000, 0x7000, omoveq, 0,
  0xF1C0, 0x80C0, ochk, "divu",
  0xF1C0, 0x81C0, ochk, "divs",
  0xF1F0, 0x8100, extend, "sbcd",
  0xF1F0, 0x8140, extend, "pack",
  0xF1F0, 0x8180, extend, "unpk",
  0xF000, 0x8000, opmode, "or",
  0xF1C0, 0x91C0, opmode, "sub",
  0xF130, 0x9100, extend, "subx",
  0xF000, 0x9000, opmode, "sub",
  0xF1C0, 0xB1C0, opmode, "cmp",
  0xF138, 0xB108, extend, "cmpm",
  0xF100, 0xB000, opmode, "cmp",
  0xF100, 0xB100, opmode, "eor",
  0xF1C0, 0xC0C0, ochk, "mulu",
  0xF1C0, 0xC1C0, ochk, "muls",
  0xF1F8, 0xC188, extend, "exg",
  0xF1F8, 0xC148, extend, "exg",
  0xF1F8, 0xC140, extend, "exg",
  0xF1F0, 0xC100, extend, "abcd",
  0xF000, 0xC000, opmode, "and",
  0xF1C0, 0xD1C0, opmode, "add",
  0xF130, 0xD100, extend, "addx",
  0xF000, 0xD000, opmode, "add",
  0xF8C0, 0xE8C0, obf, 0,
  0xF100, 0xE000, shroi, "r",
  0xF100, 0xE100, shroi, "l",
  0xF1C0, 0xF000, cpgen, 0,
  0xF1C0, 0xF040, cpcc, 0,
  0xF180, 0xF080, cpbcc, 0,
  0xF180, 0xF100, cpsres, 0,
  0, 0, 0, 0
};

mkioptab()
{
/* dummy routine called from main */
}

printins(idsp, inst)
register int inst;
{
	register struct opdesc *p;

	space = idsp; dotinc = 2;
	IMDF = "#%Z";
	inst >>= 16;
	for (p = opdecode; p->mask; p++)
		if ((inst & p->mask) == p->match) break;
	if (p->mask != 0) (*p->opfun)(inst, p->farg);
	else printf(badop);
}

long
instfetch(size)
int size;
{
	long l1;

	l1 = (long)(chkget(inkdot(dotinc), space));
	if (size==2)
	    l1 = (l1 >> 16) & 0xFFFF;
	dotinc += size;
	return(l1);
}


printea(mode,reg,size)
long mode, reg;
int size;
{
	long index;
	extern int dot;

	switch ((int)(mode)) {
	  case 0:
		printf("d%d",reg);
		break;

	  case 1:
		printf("a%d",reg);
		break;

	  case 2:
		printf("a%d@",reg);
		break;

	  case 3:
		printf("a%d@+",reg);
		break;

	  case 4:
		printf("a%d@-",reg);
		break;

	  case 5:
		printf("a%d@(%z)",reg,instfetch(2));
		break;

	  case 6:
		hardea(reg);
		break;

	  case 7:
		switch ((int)(reg)) {
		  case 0:
			index = instfetch(2);
			printf("%z:w",index);
			break;

		  case 1:
			index = instfetch(4);
			psymoff(index, ISYM, "");
			break;

		  case 2:
			index = instfetch(2) + inkdot(2);
			psymoff(index, ISYM, "");
			break;

		  case 3:	/* complicated pc-relative stuff */
			hardea(8);
			break;

		  case 4:
/*
**			The formats here are slightly eccentric, geared to the
**			built-in printf, which I have modified. (WAT)
*/
			switch(size) {
			  case 1:
				printf(IMDF, (char)instfetch(2));
				break;
			  case 2:
				printf(IMDF, instfetch(size));
				break;
			  case 4:
				printf(IMDF, instfetch(size));
				break;
			  case 8:
				printf("#0x%08.8X", instfetch(4));
				printf("%08.8X", instfetch(4));
				break;
			  case 10:
				printf("#0x%08.8X", instfetch(4));
				printf("%08.8X", instfetch(4));
				printf("%04.4X", instfetch(2));
				break;
			  case 12:
				printf("#0x%08.8X", instfetch(4));
				printf("%08.8X", instfetch(4));
				printf("%08.8X", instfetch(4));
				break;
			  default:
				printf("???");
				break;
			}
			break;

		  default:
			printf("???");
			break;
			}
			break;

	  default:	printf("???");
	}
}

#define IIS(x) (x & 0x0007)
#define BDSIZE(x) (((x & 0x0030) >> 3)-2)
#define BS(x) (x & 0x0080)
#define IS(x) (x & 0x0040)
#define DA(x) (x & 0x8000)
#define IREG(x) ((x & 0x7000) >> 12)
#define WL(x) (x & 0x0800)
#define SCALE(x) ((x & 0x0600) >> 9)

hardea(reg)
long reg;
{
	long index,disp,outer,needcomma;

	index = instfetch(2);
	if (index & 0x0100) {	/* Full format extension */
	    if (!BS(index)) {
		if (reg == 8) printf("pc");
		else printf("a%d",reg);
	    }
	    printf("@(");
	    needcomma = 0;
	    if (IIS(index))
		printf("[");
	    if ((disp = BDSIZE(index)) > 0) {
		printf("%z:w", instfetch(disp));
		needcomma++;
	    } else if (disp < 0) {
		printf("???");
		needcomma++;
	    }
	    if (IIS(index)==5 || IIS(index)==6 || IIS(index)==7) {
		printf("]");
		needcomma++;
	    }
	    if (!IS(index)) {
		if (needcomma++) printf(",");
		printf("%c%d", DA(index)?'a':'d',IREG(index));
		printf(":%c", WL(index)?'l':'w');
		if (SCALE(index))
			printf(":%d", 1<<SCALE(index));
	    }
	    if (IIS(index)==1 || IIS(index)==2 || IIS(index)==3) {
		printf("]");
		needcomma++;
	    }
	    if (IIS(index) == 2 || IIS(index) == 6) outer = 2;
	    else if (IIS(index) == 3 || IIS(index) == 7) outer = 4;
	    else outer = 0;
	    if (outer) {
		if (needcomma) printf(",");
		printf("%X", instfetch(outer));
	    }
	    printf(")");
	} else {		/* Brief format extension */
	    if (reg == 8) printf("pc,");
	    else printf("a%d", reg);
	    disp = (char)(index&0xFF);
	    printf("@(%Z,", disp);
	    printf("%c%d", DA(index)?'a':'d',IREG(index));
	    printf(":%c", WL(index)?'l':'w');
	    if (SCALE(index))
		printf(":%d", 1<<SCALE(index));
	    printf(")");
	}
}

printEA(ea,size)
long ea;
int size;
{
	printea((ea>>3)&07,ea&07,size);
}

mapsize(inst)
register long inst;
{
	inst >>= 6;
	inst &= 03;
	return((inst==0) ? 1 : (inst==1) ? 2 : (inst==2) ? 4 : -1);
}

char suffix(size)
register int size;
{
	return((size==1) ? 'b' : (size==2) ? 'w' : (size==4) ? 'l' : '?');
}

omove(inst, s)
register long inst;
char *s;
{
	register int size;

	printf("\tmov%c\t",*s);
	size = ((*s == 'b') ? 1 : (*s == 'w') ? 2 : 4);
	printea((inst>>3)&07,inst&07,size);
	printc(',');
	printea((inst>>6)&07,(inst>>9)&07,size);
}

obranch(inst,dummy)
long inst;
{
	long disp = inst & 0xFF;
	char *s = "s"; 

	if (disp == 0xFF) { s = "l"; disp = instfetch(4); }
	else if (disp == 0) { s = ""; disp = instfetch(2); }
	else if (disp > 0x7F) disp |= ~0xFF;
	printf("\tb%s%s\t",bname[(int)((inst>>8)&017)],s);
	psymoff(disp+inkdot(2), ISYM, "");
}

odbcc(inst,dummy)
long inst;
{
	long disp;

	printf("\tdb%s\t",dbname[(int)((inst>>8)&017)]);
	printea(0,inst&07,1);
        printc(',');
	disp = instfetch(2);
	psymoff(disp+inkdot(2),ISYM,"");
}

oscc(inst,dummy)
register long inst;
{
	register long opmode = inst & 7;
	register char *cond = sccname[(int)((inst>>8)&017)];

	switch(opmode) {
	  case 2:
		printf("\ttp%sw\t", cond);
		printf(IMDF, instfetch(2));
		break;
	  case 3:
		printf("\ttp%sl\t", cond);
		printf(IMDF, instfetch(4));
		break;
	  case 4:
		printf("\tt%s", cond);
		break;
	  default:
		printf("\ts%s\t",cond);;
		printea((inst>>3)&07,inst&07,1);
		break;
	}
}

biti(inst, dummy)
register long inst;
{
	printf("\t%s\t", bit[(int)((inst>>6)&03)]);
	if (inst&0x0100) printf("d%D,", inst>>9);
	else { printf(IMDF, instfetch(2)); printc(','); }
	printEA(inst);
}

opmode(inst,opcode)
register long inst;
{
	register int opmode = (int)((inst>>6) & 07);
	register int reg = (int)((inst>>9) & 07);
	register int size;

	size = (opmode==0 || opmode==4) ?
		1 : (opmode==1 || opmode==3 || opmode==5) ? 2 : 4;
	printf("\t%s%c\t", opcode, suffix(size));
	if ((inst & 0xF000) == 0xB000)
	{
		printea((inst>>3)&07,inst&07, size);
		if (opmode == 3 || opmode == 7) 
		    printf(",a%d",reg);
		else 
		    printf(",d%d",reg);
	}
	else if (opmode>=4 && opmode<=6)
	{
		printf("d%d,",reg);
		printea((inst>>3)&07,inst&07, size);
	}
	else
	{
		printea((inst>>3)&07,inst&07, size);
		printf(",%c%d",(opmode<=2)?'d':'a',reg);
	}
}

char *bfnames[] = {"bftst", "bfextu", "bfchg", "bfexts", "bfclr", "bfffo",
			"bfset", "bfins" };

obf(inst, dummy)
register long inst;
{
	register int type, ext;

	type = (inst & 0x0700) >> 8;
	ext = instfetch(2);
	printf("\t%s\t", bfnames[type]);
	if (type == 7) 				/* bfins */
		printf("d%d,", (ext & 0x7000) >> 12);
	printEA(inst);
	printf("{");
	if (ext & 0x0800) {		/* offset is data register */
		if (ext & 0x0600) printf("???");	/* These must be zero */
		else printf("d%d", (ext & 0x01C0) >> 6);
	} else printf("#%D", (ext & 0x07C0) >> 6); /* offset is immediate */
	printf(":");
	if (ext & 0x0020) {			/* width is data register */
		if (ext & 0x0018) printf("???");	/* These must be zero */
		else printf("d%d", ext & 0x0007);
	} else printf("#%D", ext & 0x001F);	/* width is immediate */
	printf("}");
	if (type == 1 || type == 3 || type == 5)  /* bfexts, bfextu, bfffo */
		printf(",d%d", (ext & 0x7000) >> 12);
}

shroi(inst,ds)
register long inst;
char *ds;
{
	int rx, ry;
	register char *opcode;
	if ((inst & 0xC0) == 0xC0)
	{
		opcode = shro[(int)((inst>>9)&03)];
		printf("\t%s%s\t", opcode, ds);
		printEA(inst);
	}
	else
	{
		opcode = shro[(int)((inst>>3)&03)];
		printf("\t%s%s%c\t", opcode, ds, suffix(mapsize(inst)));
		rx = (int)((inst>>9)&07); ry = (int)(inst&07);
		if ((inst>>5)&01) printf("d%d,d%d", rx, ry);
		else
		{
			printf(IMDF, (rx ? rx : 8));
			printf(",d%d", ry);
		}
	}
}		

oimmed(inst,opcode) 
long inst;
register char *opcode;
{
	register int size = mapsize(inst);
	long value;

	if (size > 0)
	{
		value = instfetch(size==4?4:2);
		printf("\t%s%c\t", opcode, suffix(size));
		if ((inst & 0xFF00) == 0x0C00) {	/* cmpi */
			printEA(inst,size); printc(',');
			printf(IMDF, value);
		} else {				/* other immediate */
			printf(IMDF, value); printc(',');
			printEA(inst,size);
		}
	}
	else printf(badop);
}

oreg(inst,opcode)
long inst;
char *opcode;
{
	printf(opcode, (inst & 07));
}

extend(inst, opcode)
register long	inst;
char	*opcode;
{
	register int size = mapsize(inst);
	int ry = (inst&07), rx = ((inst>>9)&07);
	char c;

	printf("\t%s", opcode);
	if (inst & 0x1000) printf("%c", suffix(size));
	printf("\t");
	if (*opcode == 'e')
	{
		if (inst & 0x0080) printf("d%D,a%D", rx, ry);
		else if (inst & 0x0008) printf("a%D,a%D", rx, ry);
		else printf("d%D,d%D", rx, ry);
	}
	else if ((inst & 0xF000) == 0xB000) printf("a%d@+,a%d@+", ry, rx);
	else if (inst & 0x8) printf("a%d@-,a%d@-", ry, rx);
	else printf("d%D,d%D", ry, rx);
	if (((inst & 0xF1F0) == 0x8180) || ((inst & 0xF1F0) == 0x8140))	{
		printf(","); printf(IMDF, instfetch(2));
	}
}

olink(inst, s)
register long inst;
char *s;
{
	printf("\tlink%c\ta%D,", *s, inst&07);
	printf(IMDF, instfetch((*s == 'w') ? 2 : 4));
}

omuldiv(inst,opcode)
register long inst;
register char *opcode;
{
	register long dq, dr;
	register long ext = instfetch(2);

	dr = ext & 7;
	dq = (ext & 0x7000) >> 12;
	printf("\t");
	if ((*opcode == 'd') && ((ext & 0x0400)==0) && (dq != dr)) printf("t");
	printf("%s%cl\t", opcode, (ext & 0x0800) ? 's' : 'u');
	printEA(inst,4);
	printf(",d%d", dr);
	if (dq != dr) printf(":d%d", dq);
	if ((*opcode == 'm') && (ext & 0x0400)==0x0400 && dq == dr)
		printf(":d%d\n\t\t\t(WARNING: results are undefined)", dq);
}

otrap(inst,dummy)
long inst;
{
	printf("\ttrap\t");
	printf(IMDF, inst&017);
}

obkpt(inst,dummy)
long inst;
{
	printf("\tbkpt\t");
	printf(IMDF, inst&7);
}

oneop(inst,opcode)
long inst;
register char *opcode;
{
	printf("\t%s\t",opcode);
	printEA(inst);
}

oaoccsr(inst,opcode)
long inst;
register char *opcode;
{
	printf("\t%s\t",opcode);
	printf(IMDF,(unsigned short)instfetch(2));
	printf(",%s", (inst&0x0040) ? "sr":"cc");
}

omvccsr(inst,reg)
long inst;
register char *reg;
{
	printf("\tmovw\t");
	if (inst&0x0400) {
		printEA(inst,2);
		printf(",%s", reg);
	} else {
		printf("%s,", reg);
		printEA(inst,2);
	}
}

pregmask(mask)
register int mask;
{
	register int i;
	register int flag = 0;

	for (i=0; i<16; i++)
	{
		if (mask&1)
		{
			if (flag) printc('/'); else flag++;
			printf("%c%d",(i<8)?'d':'a',i&07);
		}
		mask >>= 1;
	}
}

omovem(inst,dummy)
long inst;
{
	register int i, list = 0, mask = 0100000;
	register int reglist = (int)(instfetch(2));

	if ((inst & 070) == 040)	/* predecrement */
	{
		for(i = 15; i > 0; i -= 2)
		{ list |= ((mask & reglist) >> i); mask >>= 1; }
		for(i = 1; i < 16; i += 2)
		{ list |= ((mask & reglist) << i); mask >>= 1; }
		reglist = list;
	}
	printf("\tmovem%c\t",(inst&0x40)?'l':'w');
	if (inst&02000)
	{
		printEA(inst);
		printc(',');
		pregmask(reglist);
	}
	else
	{
		pregmask(reglist);
		printc(',');
		printEA(inst);
	}
}

ochk(inst,opcode)
long inst;
register char *opcode;
{
	printf("\t%s\t",opcode);
	printEA(inst);
	printf(",%c%D",(*opcode=='l')?'a':'d',(inst>>9)&07);
}

ochk2(inst,dummy)
long inst;
{
	register long ext;
	register int size;

	ext = instfetch(2);
	if (ext & 0x0800) {	/* chk2 */
		printf("\tchk2.");
		size = (inst & 0x0600) >> 9;
		printf("%c", (size==0)?'b':(size==1)?'w':(size==2)?'l':'?');
		printEA(inst);
		printf(",\t%c%d,", (ext & 0x8000)?'a':'d',(ext & 0x7000)>>12);
	} else {		/* cmp2 */
		printf("\tcmp2.");
		size = (inst & 0x0600) >> 9;
		printf("%c", (size==0)?'b':(size==1)?'w':(size==2)?'l':'?');
		printf("\t%c%d,", (ext & 0x8000)?'a':'d',(ext & 0x7000)>>12);
		printEA(inst);
	}
}

soneop(inst,opcode)
long inst;
register char *opcode;
{
	register int size = mapsize(inst);

	if (size > 0)
	{
		printf("\t%s%c\t",opcode,suffix(size));
		printEA(inst);
	}
	else printf(badop);
}

oquick(inst,opcode)
long inst;
register char *opcode;
{
	register int size = mapsize(inst);
	register int data = (int)((inst>>9) & 07);

	if (data == 0) data = 8;
	if (size > 0)
	{
		printf("\t%s%c\t", opcode, suffix(size));
		printf(IMDF, data); printc(',');
		printEA(inst);
	}
	else printf(badop);
}

omoveq(inst,dummy)
long inst;
{
	register int data = (int)(inst & 0377);

	if (data > 127) data |= ~0377;
	printf("\tmovl\t"); printf(IMDF, data);
	printf(",d%D", (inst>>9)&07);
}

oprint(inst,opcode)
long inst;
register char *opcode;
{
	printf("\t%s",opcode);
}

omovec(inst,dummy)
long inst;
{
	register long ext, cntrl;
	register char *regname;

	ext = instfetch(2);
	cntrl = ext & 0xFFF;
	if (cntrl > 0x804 || (cntrl < 0x800 && cntrl > 2))
		printf(badop);
	else {
		printf("\tmovl\t");
		regname = (cntrl&0x800) ? creg2[cntrl&0x7ff] : creg[cntrl];
		if (inst & 1) {
			printf("%c%D,%s",  (ext & 0x8000)? 'a': 'd',
			    (ext >> 12) & 07, regname);
		} else {
			printf("%s,%c%D", regname,
			    (ext & 0x8000)? 'a': 'd', (ext >> 12) & 07);
		}
	}
}

omoves(inst, dummy)
long inst;
{
	register long ext;
	register int size = mapsize(inst);

	ext = instfetch(2);
	if (ext & 0x7FF)
		printf(badop);
	else {
		printf("\tmovs%c\t", suffix(size));
		if (ext & 0x800) {
			printf("%c%D,",  (ext & 0x8000)? 'a': 'd',
			    (ext >> 12) & 07);
			printea((inst>>3)&07,inst&07, size);
		} else {
			printea((inst>>3)&07,inst&07, size);
			printf(",%c%D",  (ext & 0x8000)? 'a': 'd',
			    (ext >> 12) & 07);
		}
	}
}

ortd(inst,dummy)
long inst;
{
	printf("\trtd\t");
	printf(IMDF, instfetch(2));
}

ocas(inst,size)
long inst;
char *size;
{ 
	register long ext1, ext2;
	register int du1, dc1;

	ext1 = instfetch(2);
	dc1 = ext1 & 7;
	du1 = (ext1 & 0x01C0) >> 6;
	if ((inst & 0x3F) == 0x3C) {	/* cas2 */
		ext2 = instfetch(2);
		printf("\tcas2.%s\td%d:d%d,", size, dc1, ext2&7);
		printf("d%d:d%d,", du1, (ext2 & 0x01C0) >> 6);
		printf("%c%d@:", (ext1&0x8000)?'a':'d',(ext1&0x7000)>>12);
		printf("%c%d@", (ext2&0x8000)?'a':'d',(ext2&0x7000)>>12);
	} else	{			/* cas */
		printf("\tcas.%s\td%d,d%d,", size, dc1, du1);
		printEA(inst);
	}
}

ocallm(inst,dummy)
long inst;
{ 
	register long ext;

	if ((inst & 0xFFF0) == 0x06C0) {	/* rtm */
		printf("\trtm\t%c%d", (inst&8)?'a':'d', inst&7);
	} else {				/* callm */
		ext = instfetch(2);
		printf("\tcallm\t");
		if (ext & 0xFF00) printf("&???");
		else printf(IMDF, ext);
		printf(","); printEA(inst);
	}
}

char buf[10], buf1[10];

cpsres(inst,dummy)
long inst;
{
	register char *cpname;

	switch ((inst & 0x0e00) >> 9) {
	  case 0:				/* 68851 */
		cpname = "p";
		break;
	  case 1:				/* 68881 */
		cpname = "f";
		break;
	  default:
		sprintf(buf, "CP%x", (inst & 0x0e00) >> 9);
		cpname = buf;
		break;
	}
	printf("\t%s%s\t", cpname, (inst & 0x40) ? "restore" : "save");
	printEA(inst);
}

cpcc(inst, dummy)
long inst;
{
	register int com, reg, pred, mode;
	register char *lbname, *cpname;

	com = instfetch(2);
	if (com & 0xFFC0) {
		printf("???");
		return;
	}
	pred = com & 0x003F;
	switch ((inst & 0x0e00) >> 9) {
	  case 0:				/* 68851 */
		cpname = "p";
		if (pred > 0xf) pred = 0x10;
		lbname = pbname[pred];
		break;
	  case 1:				/* 68881 */
		cpname = "f";
		lbname = fbname[pred];
		break;
	  default:
		sprintf(buf, "CP%x", (inst & 0x0e00) >> 9);
		cpname = buf;
		sprintf(buf1, "%x", pred);
		lbname = buf1;
		break;
	}
	switch (inst & 0x0038) {
	  case 8:				/* CPDBcc */
		printf("\t%sdb%s\td%d,", cpname, lbname, reg );
		psymoff(instfetch(2)+inkdot(4), ISYM, "");
		break;
	  case 0x38:				/* CPTRAPcc */
		mode = inst & 7;
		if (mode == 2) {
			printf("\t%stp%sw\t&%X", cpname, lbname, instfetch(2));
			return;
		} else if (mode == 3) {
			printf("\t%stp%sl\t&%X", cpname, lbname, instfetch(4));
			return;
		} else if (mode == 4) {
			printf("\t%st%s", cpname, lbname);
			return;
		} /*	NOTE -- other cases fall through to CPScc */

	  default:				/* CPScc */
		printf("\t%ss%s\t", cpname, lbname);
		printEA(inst);
	}
}

fop(inst,com)
register long inst, com;
{
	if ((com & 0xFC00) == 0x5C00) fmovecr(inst, com);
	else if ((com & 0xA000) == 0x0000) fmost(inst, com);
	else if ((com & 0xE000) == 0x6000) fmov(inst, com);
	else if ((com & 0xC3FF) == 0x8000) fmovmcr(inst, com);
	else if ((com & 0xC700) == 0xC000) fmovm(inst, com);
	else printf("\tfgen\t<%X,%X>", (unsigned short)inst,
	  (unsigned short)com);
}

char *from1[] = {"log10(2)", "e", "log2(e)", "log10(e)", "0.0"};
char *from2[] = { "ln(2)", "ln(10)", "10**0", "10**1", "10**2", "10**4",
		"10**8", "10**16", "10**32", "10**64", "10**128", "10**256",
		"10**512", "10**1024", "10**2048", "10**4096"};

fmovecr(inst,com)
long inst, com;
{
	register long index = com & 0x7F;

	printf("\tfmovcr.x\t<");
	if (index == 0) printf("pi");
	else if (0xB <= index && index <= 0xF)
		printf("%s", from1[(com&0x7F) - 0xB]);
	else if (0x30 <= index && index <= 0x3F)
		printf("%s", from2[(com&0x7F) - 0x30]);
	else printf("%X (Motorola reserved)", index);
	printf(">,fp%d", (com & 0x3800) >> 11);
}

char *fnames[] =
{
	"fmov",
	"fint",
	"fsinh",
	"fintrz",
	"fsqrt",
	0,
	"flognp1",
	0,
	"fetoxm1",
	"ftanh",
	"fatan",
	0,
	"fasin",
	"fatanh",
	"fsin",
	"ftan",
	"fetox",
	"ftwotox",
	"ftentox",
	0,
	"flogn",
	"flog10",
	"flog2",
	0,
	"fabs",
	"fcosh",
	"fneg",
	0,
	"facos",
	"fcos",
	"fgetexp",
	"fgetman",
	"fdiv",
	"fmod",
	"fadd",
	"fmul",
	"fsgldiv",
	"frem",
	"fscale",
	"fsglmul",
	"fsub",
	0,
	0,
	0,
	0,
	0,
	0,
	0,
	"fsincos",
	"fsincos",
	"fsincos",
	"fsincos",
	"fsincos",
	"fsincos",
	"fsincos",
	"fsincos",
	"fcmp",
	0,
	"ftst",
};
	
char fsdf[] = "lsxpwdbp";
int fsize[] = {4, 4, 10, 12, 2, 8, 1, 12};

fmost(inst, com)
long inst, com;
{
	register int ssp, des;

	if (((com & 0x40) == 0) && fnames[com&0x3f]) {
		printf("\t%s", fnames[com&0x3f]);
		ssp = (com & 0x1C00) >> 10;
		des = (com & 0x0380) >> 7;
		if (com & 0x4000) {
			printf("%c\t", fsdf[ssp]);
			printEA(inst, fsize[ssp]);
		} else {
			printf(".x\t");
			printf("fp%d", ssp);
		}
		printf(",");
		if ((com & 0x38) == 0x30)	/* fsincos */
			printf("fp%d:", com & 7);
		printf("fp%d", des);
	} else
		printf("FP: %X %X", (unsigned short)inst, (unsigned short)com);
}

fmov(inst, com)
long inst, com;
{
	register int dff, src;

	dff = (com & 0x1C00) >> 10;
	src = (com & 0x0380) >> 7;
	printf("\tfmov%c\tfp%d,", fsdf[dff], src);
	printEA(inst);
	if (dff == 3) {	/* packed decimal, static k-factor */
		printf("{");
		printf(IMDF, com & 0x7F);
		printf("}");
	} else if (dff == 7) { /* packed decimal, dynamic k-factor */
		printf("{d%d}", (com & 0x7F) >> 4);
	}
}

pfcr(regmask)
int regmask;
{
	register int needcomma = 0;
	if (regmask & 0x1000) {
		printf("control");
		needcomma++;
	}
	if (regmask & 0x0800) {
		if (needcomma++) printf(",");
		printf("status");
	}
	if (regmask & 0x0400) {
		if (needcomma++) printf(",");
		printf("iaddr");
	}
	if (needcomma == 0)
		printf("<null>");
}

fmovmcr(inst, com)
long inst, com;
{

	printf("\tfmoveml\t");
	if (com & 0x2000) {	/* move from 68881 to memory */
		pfcr(com & 0x1C00);
		printf(",");
		printEA(inst);
	} else {		/* move from memory to 68881 */
		printEA(inst, 4);
		printf(",");
		pfcr(com & 0x1C00);
	}
}

fpregmsk(mask)
register int mask;
{
	register int i;
	register int flag = 0;

	for (i=0; i<8; i++)
	{
		if (mask&1)
		{
			if (flag) printc('/'); else flag++;
			printf("fp%d",i);
		}
		mask >>= 1;
	}
}

fmovm(inst,com)
long inst, com;
{
	register int i, list = 0;
	register int reglist;

	reglist = com & 0x00FF;
	if ((com & 0x1800) == 0x1000) { /* static postincrement or control */
		for (i=0; i<8; i++) {
			list |= ((reglist & 1) <<  (7 - i));
			reglist >>= 1;
		}
		reglist = list;
	}
	printf("\tfmovem\t");
	if (com&0x2000) {
		if ((com & 0x0800) == 0x0800) printf("d%d", (com & 0x70)>>4);
		else fpregmsk(reglist);
		printc(',');
		printEA(inst);
	} else {
		printEA(inst);
		printc(',');
		if ((com & 0x0800) == 0x0800) printf("d%d", (com & 0x70)>>4);
		else fpregmsk(reglist);
	}
}


cpbcc(inst,dummy)
long inst;
{
	register long cpid, size, disp, pred;
	register char *cpname, *lbname;

	cpid = (inst & 0x0e00) >> 9;
	disp = size ? instfetch(4) : instfetch(2);
	pred = inst & 0x003F;
	switch (cpid) {
	  case 0:				/* 68851 */
		cpname = "p";
		if (pred > 0xf) pred = 0x10;
		lbname = pbname[pred];
		break;
	  case 1:				/* 68881 */
		cpname = "f";
		lbname = fbname[pred];
		if (!pred && !disp) { printf("\tfnop"); return; }
		break;
	  default:
		sprintf(buf, "CP%x", cpid);
		cpname = buf;
		sprintf(buf1, "%x", pred);
		lbname = buf1;
		break;
	}
	printf("\t%sb%s%c\t", cpname, lbname, (size ? 'l' : 'w'));
	psymoff(disp+inkdot(2), ISYM, "");
}

cpgen(inst,dummy)
long inst;
{
	register long com, cpid;

	cpid = (inst & 0x0e00) >> 9;
	com = instfetch(2);
	switch (cpid) {
	  case 0:		/* 68851 */
		pop(inst, com);
		break;
	  case 1:		/* 68881 */
		fop(inst, com);
		break;
	  default:
		printf("\tCP%xgen\t<%X,%X>", cpid, (unsigned short)inst,
		  (unsigned short)com);
		break;
	}
}

pop(inst,com)
register long inst, com;
{
	if (com == 0x2800) pvalid(inst, com);
	else if ((com & 0xFFF8) == 0x2C00) pvalid2(inst, com);
	else if ((com & 0xFDE0) == 0x2000) pload(inst, com);
	else if ((com & 0xE200) == 0x2000) pflush(inst, com);
	else if ((com & 0xE1FF) == 0x4000) pmove(inst, com);
	else if ((com & 0xE1E3) == 0x6000) pmove2(inst, com);
	else if ((com & 0xE000) == 0x8000) ptest(inst, com);
	else if ((com & 0xFFFF) == 0xA000) pflushr(inst, com);
	else printf("\tpgen\t<%X,%X>", (unsigned short)inst,
	  (unsigned short)com);
}

pvalid(inst,com)
register long inst, com;
{
	printf("\tpvalid\tval,");
	printEA(inst);
}

pvalid2(inst,com)
register long inst, com;
{
	printf("\tpvalid\ta%d,", com & 7);
	printEA(inst);
}

pfc(fc)
register long fc;
{
	if (fc & 0x0010) printf(IMDF, fc & 0x000F);
	else if (fc & 0x0008) printf("d%d", fc & 0x0007);
	else if (fc == 0) printf("sfc");
	else if (fc == 1) printf("dfc");
	else printf("???");
}

pload(inst,com)
register long inst, com;
{
	printf("\tpload%c\t", (com & 0x0200) ? 'r' : 'w');
	pfc(com & 0x1F);
	printf(",");
	printEA(inst);
}

pmove(inst,com)
register long inst, com;
{
	register int pno = (com & 0x1C00) >> 10;

	printf("\tpmov\t");
	if (com & 0x0200) {
		printf("%s,", preg[pno]);
		printEA(inst, spreg[pno]);
	} else {
		printEA(inst, spreg[pno]);
		printf(",%s", preg[pno]);
	}
}

pstuff(pno, num)
register int pno, num;
{
	switch (pno) {
	  case 0:
		printf("psr");
		break;
	  case 1:
		printf("pcsr");
		break;
	  case 4:
		printf("bad%d", num);
		break;
	  case 5:
		printf("bac%d", num);
		break;
	  default:
		printf("???");
		break;
	}
}

pmove2(inst,com)
register long inst, com;
{ 
	register int pno = (com & 0x1C00) >> 10;
	register int num = (com & 0x001C) >> 2;
	register int size;

	printf("\tpmov\t");
	if (com & 0x0200) {
		pstuff(pno, num);
		printf(",");
		printEA(inst, 2);
	} else {
		printEA(inst, 2);
		printf(",");
		pstuff(pno, num);
	}
}

ptest(inst,com)
register long inst, com;
{ 
	printf("\tptest%c\t", (com & 0x0200) ? 'r' : 'w');
	pfc(com & 0x1F);
	printf(",");
	printEA(inst);
	printf(",");
	printf(IMDF, (com & 0x1C00) >> 10);
	if (com & 0x0100) printf(",a%d", (com & 0x00E0) >> 5);
}

pflush(inst,com)
register long inst, com;
{ 
	register int pmode = (com & 0x1C00) >> 10;

	printf("\tpflush");
	if (pmode == 1) { printf("a"); return; }
	else if (pmode & 1) printf("s\t");
	else printf("\t");
	pfc(com & 0x1F);
	printf(",");
	printf(IMDF, (com & 0x01E0) >> 5);
	if (pmode & 2) { printf(","); printEA(inst); }
}

pflushr(inst,com)
register long inst, com;
{ 
	printf("\tpflushr\t");
	printEA(inst, 8);
}
