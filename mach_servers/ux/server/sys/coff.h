/* 
 * Mach Operating System
 * Copyright (c) 1989 Carnegie-Mellon University
 * All rights reserved.  The CMU software License Agreement specifies
 * the terms and conditions for use and redistribution.
 */
/*
 * HISTORY
 * $Log:	coff.h,v $
 * Revision 2.2  89/11/29  15:29:28  af
 * 	Revision 2.1.1.1  89/10/29  15:13:56  af
 * 		Created, for pure Mach kernel.
 * 		[89/10/09            af]
 * 	<<<log message for ./server/sys/coff.h>>>
 * 	[89/11/16  17:06:33  af]
 * 
 */
struct filehdr {
	unsigned short	f_magic;	/* magic number */
	unsigned short	f_nscns;	/* number of sections */
	long		f_timdat;	/* time & date stamp */
	long		f_symptr;	/* file pointer to symtab */
	long		f_nsyms;	/* number of symtab entries */
	unsigned short	f_opthdr;	/* sizeof(optional hdr) */
	unsigned short	f_flags;	/* flags */
};

#define  F_EXEC		0000002

#if	BYTE_MSF
#define MIPSMAGIC	0540
#else
#define MIPSMAGIC	0542
#endif


struct scnhdr {
	char		s_name[8];	/* section name */
	long		s_paddr;	/* physical address */
	long		s_vaddr;	/* virtual address */
	long		s_size;		/* section size */
	long		s_scnptr;	/* file ptr to raw data for section */
	long		s_relptr;	/* file ptr to relocation */
	long		s_lnnoptr;	/* file ptr to line numbers */
	unsigned short	s_nreloc;	/* number of relocation entries */
	unsigned short	s_nlnno;	/* number of line number entries */
	long		s_flags;	/* flags */
};



struct aouthdr {
	short	magic;		/* see magic.h				*/
	short	vstamp;		/* version stamp			*/
	long	tsize;		/* text size in bytes, padded to FW
				   bdry					*/
	long	dsize;		/* initialized data "  "		*/
	long	bsize;		/* uninitialized data "   "		*/
	long	entry;		/* entry point, value of "start"	*/
	long	text_start;	/* base of text used for this file	*/
	long	data_start;	/* base of data used for this file	*/
	long	bss_start;	/* base of bss used for this file	*/
	long	gprmask;	/* general purpose register mask	*/
	long	cprmask[4];	/* co-processor register masks		*/
	long	gp_value;	/* the gp value used for this object    */
};


#define SCNROUND ((long)16)
#define OSCNRND  ((long)8)

#define N_TXTOFF(f, a) \
 ((a).magic == ZMAGIC ? 0 : \
  ((a).vstamp < 23 ? \
   ((sizeof(struct filehdr) + sizeof(struct aouthdr) + \
     (f).f_nscns * sizeof(struct scnhdr) + 7) & ~7) : \
   ((sizeof(struct filehdr) + sizeof(struct aouthdr) + \
     (f).f_nscns * sizeof(struct scnhdr) + 15) & ~15)))
