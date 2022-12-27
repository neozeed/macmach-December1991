/* 
 * Mach Operating System
 * Copyright (c) 1991,1990 Carnegie Mellon University
 * All Rights Reserved.
 * 
 * Permission to use, copy, modify and distribute this software and its
 * documentation is hereby granted, provided that both the copyright
 * notice and this permission notice appear in all copies of the
 * software, derivative works or modified versions, and any portions
 * thereof, and that both notices appear in supporting documentation.
 * 
 * CARNEGIE MELLON ALLOWS FREE USE OF THIS SOFTWARE IN ITS 
 * CONDITION.  CARNEGIE MELLON DISCLAIMS ANY LIABILITY OF ANY KIND FOR
 * ANY DAMAGES WHATSOEVER RESULTING FROM THE USE OF THIS SOFTWARE.
 * 
 * Carnegie Mellon requests users of this software to return to
 * 
 *  Software Distribution Coordinator  or  Software.Distribution@CS.CMU.EDU
 *  School of Computer Science
 *  Carnegie Mellon University
 *  Pittsburgh PA 15213-3890
 * 
 * any improvements or extensions that they make and grant Carnegie the
 * rights to redistribute these changes.
 */
/*
 * HISTORY
 * $Log:	xstrip_mips.c,v $
 * Revision 2.2  91/08/29  16:47:53  jsb
 * 	First checkin.
 * 
 * Revision 2.3  91/03/19  13:02:45  mrt
 * 	Changed to new copyright
 * 
 * Revision 2.2  90/09/12  16:34:30  rpd
 * 	Added dummy non-mips code.
 * 	[90/06/21            rpd]
 * 
 * 	Keep scText/stNil symbols too.
 * 	[90/06/07            rpd]
 * 
 */
/*
 *	File:	xstrip_mips.c
 *	Author:	Joseph S. Barrera III
 *	Date:	1991
 */

#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <mips/coff.h>
#include <mips/syms.h>

typedef int boolean_t;
#define TRUE	1
#define FALSE	0

extern char *malloc();

#define	PRINT_SYMS	0
#define	STRIP_SYMS	1

char *file = 0;
char *outptr;
HDRR *symtab, *newsymtab = 0;
unsigned st_filptr;

#define	ZAPPED_SYM	((long)0xffffdead)

SYMR *old_symr, *new_symr;
EXTR *old_extr, *new_extr;
FDR *new_fdr;
PDR *new_pdr;

char *sTypeNames[] = {
	"stNil",
	"stGlobal",
	"stStatic",
	"stParam",
	"stLocal",
	"stLabel",
	"stProc",
	"stBlock",
	"stEnd",
	"stMember",
	"stTypedef",
	"stFile",
	"stRegReloc",
	"stForward",
	"stStaticProc",
	"stConstant",
};
#define	Type_count	(sizeof(sTypeNames)/sizeof(*sTypeNames))

char *sClassNames[] = {
	"scNil",
	"scText",
	"scData",
	"scBss",
	"scRegister",
	"scAbs",
	"scUndefined",
	"scCdbLocal",
	"scBits",
	"scCdbSystem",
	"scRegImage",
	"scInfo",
	"scUserStruct",
	"scSData",
	"scSBss",
	"scRData",
	"scVar",
	"scCommon",
	"scSCommon",
	"scVarRegister",
	"scVariant",
	"scSUndefined",
	"scInit",
};
#define	Class_count	(sizeof(sClassNames)/sizeof(*sClassNames))

#define	do_strip(off, max, count, bool) \
	Do_strip(symtab->off, symtab->max, &newsymtab->off, \
	&newsymtab->max, count, bool)

Do_strip(oldoff, oldmax, newoffp, newmaxp, count, should)
	long oldoff;
	long oldmax;
	long *newoffp;
	long *newmaxp;
	int count;
	boolean_t should;
{
	int size;

	if (should || oldoff == 0) {
		*newoffp = 0;
		*newmaxp = 0;
	} else {
		if (count < 0) {
			size = -count;
		} else {
			size = count * oldmax;
		}
		*newoffp = (outptr - (char *) newsymtab) + st_filptr;
		*newmaxp = oldmax;
		bcopy(&file[oldoff], outptr, size);
		outptr += size;
	}
}

char *
get_class_name(class)
	int class;
{
	if (class < 0 || class >= Class_count) {
		return "?class?";
	} else {
		return sClassNames[class];
	}
}

char *
get_type_name(type)
	int type;
{
	if (type < 0 || type >= Type_count) {
		return "?type?";
	} else {
		return sTypeNames[type];
	}
}

print_syms()
{
	int i;
	SYMR *symr;

	symr = (SYMR *) &file[symtab->cbSymOffset];
	for (i = 0; i < symtab->isymMax; i++) {
		printf("%5d = %s.%s\n",
			i,
			get_class_name(symr[i].sc),
			get_type_name(symr[i].st));
	}
}

boolean_t
keep_sym(class, type)
	int class;
	int type;
{
	if (class == scText) {
		if (type == stNil) return TRUE;
		if (type == stFile) return TRUE;
		if (type == stProc) return TRUE;
		if (type == stLabel) return TRUE;
		if (type == stStaticProc) return TRUE;
		return FALSE;
	}
	if (class == scAbs) {
		if (type == stParam) return TRUE;
		return FALSE;
	}
	return FALSE;
}

strip_syms()
{
	int i, outi = 0, nsyms;

	old_symr = (SYMR *) &file[symtab->cbSymOffset];
	new_symr = (SYMR *) outptr;
	for (i = outi = 0; i < symtab->isymMax; i++) {
		if (keep_sym(old_symr[i].sc, old_symr[i].st)) {
			new_symr[outi] = old_symr[i];
			old_symr[i].value = outi++;
		} else {
			old_symr[i].value = ZAPPED_SYM;
		}
	}
	printf("stripped %d local syms out of %d\n",
		symtab->isymMax - outi, symtab->isymMax);
	newsymtab->cbSymOffset = (outptr - (char *) newsymtab) + st_filptr;
	newsymtab->isymMax = outi;
	outptr = (char *) &new_symr[outi];
}

boolean_t
keep_ext(class, type)
	int class;
	int type;
{
	return TRUE;
}

strip_exts()
{
	int i, outi = 0, nexts;

	old_extr = (EXTR *) &file[symtab->cbExtOffset];
	new_extr = (EXTR *) outptr;
	for (i = outi = 0; i < symtab->iextMax; i++) {
		if (keep_ext(old_extr[i].asym.sc, old_extr[i].asym.st)) {
			new_extr[outi] = old_extr[i];
			old_extr[i].asym.value = outi++;
			switch (new_extr[i].asym.st) {
				case stNil:
				case stFile:
				case stLabel:
				case stBlock:
				case stEnd:
					break;
				default:
					new_extr[i].asym.index = indexNil;
			}
		} else {
			old_extr[i].asym.value = ZAPPED_SYM;
			old_extr[i].asym.index = indexNil;
		}
	}
	printf("stripped %d global syms out of %d\n",
		symtab->iextMax - outi, symtab->iextMax);
	newsymtab->cbExtOffset = (outptr - (char *) newsymtab) + st_filptr;
	newsymtab->iextMax = outi;
	outptr = (char *) &new_extr[outi];
}

/*
 *  The only things we need to fix up -- I hope --
 *  are PDRs and FDRs. AUXUs also have isyms but we
 *  always strip them!
 */
fixup_sym_refs()
{
	int i, j, new_csym, old_base, new_base;
	PDR *pdr;

	for (i = 0; i < newsymtab->ifdMax; i++) {
		old_base = new_fdr[i].isymBase;
		new_base = old_symr[old_base].value;
		if (new_base == ZAPPED_SYM) {
			fprintf(stderr, "fatal: fdr sym zapped!\n");
			exit(1);
		}
		for (j = 0; j < new_fdr[i].cpd; j++) {
			pdr = &new_pdr[new_fdr[i].ipdFirst + j];
			pdr->isym += old_base;
			if (old_symr[pdr->isym].value == ZAPPED_SYM) {
				fprintf(stderr, "fatal: pdr sym zapped!\n");
				fprintf(stderr, "class=%d, type=%d\n",
					old_symr[pdr->isym].sc,
					old_symr[pdr->isym].st);
				fprintf(stderr, "i=%d, isym=%d, adr=0x%x\n",
					i, pdr->isym, pdr->adr);
				exit(1);
			}
			pdr->isym = old_symr[pdr->isym].value - new_base;
		}
		new_csym = 0;
		for (j = 0; j < new_fdr[i].csym; j++) {
			if (old_symr[old_base + j].value != ZAPPED_SYM) {
				new_csym++;
			}
		}
		new_fdr[i].csym = new_csym;
		new_fdr[i].isymBase = new_base;
		new_fdr[i].copt = 0;
		new_fdr[i].ioptBase = indexNil;
		new_fdr[i].caux = 0;
		new_fdr[i].iauxBase = indexNil;
		new_fdr[i].crfd = 0;
		new_fdr[i].rfdBase = indexNil;
	}
}

main(argc, argv)
	int argc;
	char **argv;
{
	int stsize;
	struct filehdr *filehdr;
	int st_hdrsize;
	int fd, rv;
	struct stat st;

	/*
	 *  Read in file header
	 */
	if (argc != 2) {
		fprintf(stderr, "usage: %s filename\n", argv[0]);
		exit(1);
	}
	fd = open(argv[1], 2);
	if (fd < 0) perror(argv[1]), exit(1);
	rv = fstat(fd, &st);
	if (rv < 0) perror("stat"), exit(1);

	file = malloc(st.st_size);
	if (file == 0) perror("malloc"), exit(1);
	rv = read(fd, file, st.st_size);
	if (rv < 0) perror("read"), exit(1);

	/*
	 *  Initialize basic variables
	 */
	filehdr = (struct filehdr *) file;
	st_hdrsize = filehdr->f_nsyms;
	st_filptr  = filehdr->f_symptr;
	if (st_filptr == 0) {
		fprintf(stderr, "%s has no symbol table\n", argv[1]);
		exit(1);
	}
	symtab = (HDRR *) &file[st_filptr];
	if (symtab->magic != magicSym) {
		fprintf("bad magic number in %s symbol header\n", argv[1]);
		exit(1);
	}
	stsize = st.st_size - st_filptr;
	/*
	 *  Allocate memory for semi-stripped symbol table
	 */
	newsymtab = (HDRR *) malloc(stsize);
	if (newsymtab == 0) perror("malloc"), exit(1);
	outptr = (char *) &newsymtab[1];
	newsymtab->magic = symtab->magic;
	newsymtab->vstamp = symtab->vstamp;

#if PRINT_SYMS
	print_syms();
#endif

	do_strip(cbLineOffset, ilineMax, -symtab->cbLine, FALSE);
	do_strip(cbDnOffset,idnMax,cbDNR, TRUE);
	new_pdr = (PDR *) outptr;
	do_strip(cbPdOffset, ipdMax, cbPDR, FALSE);

#if STRIP_SYMS
	strip_syms();
#else
	do_strip(cbSymOffset, isymMax, cbSYMR, FALSE);
#endif
	do_strip(cbOptOffset, ioptMax, cbOPTR, TRUE);
	do_strip(cbAuxOffset, iauxMax, cbAUXU, TRUE);
	do_strip(cbSsOffset, issMax, sizeof(char), FALSE);
	do_strip(cbSsExtOffset, issExtMax, sizeof(char), FALSE);
	new_fdr = (FDR *) outptr;
	do_strip(cbFdOffset, ifdMax, cbFDR, FALSE);
	do_strip(cbRfdOffset, crfd, cbRFDT, TRUE);
#if 1
	strip_exts();
#else
	do_strip(cbExtOffset, iextMax, cbEXTR, FALSE);
#endif

	if (newsymtab->ilineMax) {
		newsymtab->cbLine = symtab->cbLine;
	} else {
		newsymtab->cbLine = 0;
	}

#if STRIP_SYMS
	fixup_sym_refs();
#endif

	rv = lseek(fd, st_filptr, 0);
	if (rv < 0) perror("lseek"), exit(1);
	rv = write(fd, (char *)newsymtab, outptr - (char *)newsymtab);
	if (rv < 0) perror("write"), exit(1);
	if (rv != (outptr - (char *)newsymtab)) {
		fprintf(stderr, "short write (%d vs. %d)\n",
			rv, (outptr - (char *)newsymtab));
		exit(1);
	}
	printf("%s: truncating from %d to %d (stripped = %d)\n",
		argv[1],
		st.st_size,
		st_filptr + (outptr - (char *) newsymtab),
		st_filptr);
	rv = ftruncate(fd, (outptr - (char *) newsymtab) + st_filptr);
	if (rv < 0) perror("ftruncate"), exit(1);
	exit(0);
}
