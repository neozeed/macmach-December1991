/* 
 * Mach Operating System
 * Copyright (c) 1991,1990,1989,1988,1987 Carnegie Mellon University
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
 * $Log:	config.h,v $
 * Revision 2.8  91/07/08  16:59:14  danner
 * 	Luna88k support.
 * 	[91/06/26            danner]
 * 
 * Revision 2.7  91/06/19  11:58:22  rvb
 * 	cputypes.h->platforms.h
 * 	[91/06/12  13:45:45  rvb]
 * 
 * Revision 2.6  91/02/05  17:52:45  mrt
 * 	Changed to new Mach copyright
 * 	[91/02/01  17:51:01  mrt]
 * 
 * Revision 2.5  90/11/25  17:48:31  jsb
 * 	Added i860 support.
 * 	[90/11/25  16:52:39  jsb]
 * 
 * Revision 2.4  90/08/27  22:09:12  dbg
 * 	Merged CMU changes into Tahoe release.
 * 	[90/07/18            dbg]
 * 
 * Revision 2.3  90/06/02  15:03:48  rpd
 * 	Removed timezone, hadtz, dst, maxusers, maxdsiz, CONFIGDEP.
 * 	[90/03/26  22:56:10  rpd]
 * 
 * Revision 2.2  90/05/03  15:50:04  dbg
 * 		Do not declare sprintf() anymore.  It should be treated in code
 * 		as void.
 * 		[90/01/22            rvb]
 * 
 * Revision 2.1  89/08/03  16:54:27  rwd
 * Created.
 * 
 * Revision 2.7  89/02/25  19:17:57  gm0w
 * 	Changes for cleanup.
 * 
 * Revision 2.6  89/02/19  18:39:45  rpd
 * 	Added fopenp, get_VPATH, VPATH declarations.
 * 
 * Revision 2.5  89/02/07  22:56:06  mwyoung
 * Code cleanup cataclysm.
 * 
 * Revision 2.4  89/01/23  22:21:35  af
 * 	Changes for MIPS and I386: CONFTYPE_<name> and
 * 		THE REAL MIPS MACHINE uses 4 bit MINORS
 * 	Purge FORCE_VOLATILE/VOLATILE and replace it with f_extra.
 * 	[89/01/09            rvb]
 * 
 * 31-Mar-88  Michael Young (mwyoung) at Carnegie-Mellon University
 *	Add source, object, and configuration directory variables.
 *
 * 03-Mar-88  Jonathan J. Chew (jjc) at Carnegie-Mellon University
 *	Defined CONFTYPE_SUN4.
 *
 * 08-Jan-87  Robert Beck (beck) at Sequent Computer Systems, Inc.
 *	Add CONFTYPE_SQT and SEDIT, FASTOBJ options flags.
 *	Add NFIELDS, and d_fields,d_bin to struct device.
 *
 * 27-Oct-86  Jonathan J. Chew (jjc) at Carnegie-Mellon University
 *	Merged in David Black's changes for the Multimax.
 *
 * 21-Oct-86  Jonathan J. Chew (jjc) at Carnegie-Mellon University
 *	Merged in changes for Sun 2 and 3.
 *
 *  4-Oct-86  Avadis Tevanian (avie) at Carnegie-Mellon University
 *	Changed all references of CMUCS to CMU.
 *
 * 25-Jan-86  Avadis Tevanian (avie) at Carnegie-Mellon University
 *	Upgraded to 4.3.
 *
 * 17-Oct-85  Operating System (sys) at Carnegie-Mellon University
 *	Added OPTIONSDEF definition.
 *
 */

/*
 * Copyright (c) 1980 Regents of the University of California.
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms are permitted
 * provided that the above copyright notice and this paragraph are
 * duplicated in all such forms and that any documentation,
 * advertising materials, and other materials related to such
 * distribution and use acknowledge that the software was developed
 * by the University of California, Berkeley.  The name of the
 * University may not be used to endorse or promote products derived
 * from this software without specific prior written permission.
 * THIS SOFTWARE IS PROVIDED ``AS IS'' AND WITHOUT ANY EXPRESS OR
 * IMPLIED WARRANTIES, INCLUDING, WITHOUT LIMITATION, THE IMPLIED
 * WARRANTIES OF MERCHANTIBILITY AND FITNESS FOR A PARTICULAR PURPOSE.
 *
 *	@(#)config.h	5.8 (Berkeley) 6/18/88
 */

/*
 * Config.
 */

#include <stdio.h>
#include <sys/types.h>

#define	NODEV	((dev_t)-1)

struct file_list {
	struct	file_list *f_next;	
	char	*f_fn;			/* the name */
	u_char	f_type;			/* see below */
	u_char	f_flags;		/* see below */
	short	f_special;		/* requires special make rule */
	char	*f_needs;
	char	*f_extra;		/* stuff to add to make line */
	/*
	 * Random values:
	 *	swap space parameters for swap areas
	 *	root device, etc. for system specifications
	 */
	union {
		struct {		/* when swap specification */
			dev_t	fuw_swapdev;
			int	fuw_swapsize;
		} fuw;
		struct {		/* when system specification */
			dev_t	fus_rootdev;
			dev_t	fus_argdev;
			dev_t	fus_dumpdev;
		} fus;
	} fun;
#define	f_swapdev	fun.fuw.fuw_swapdev
#define	f_swapsize	fun.fuw.fuw_swapsize
#define	f_rootdev	fun.fus.fus_rootdev
#define	f_argdev	fun.fus.fus_argdev
#define	f_dumpdev	fun.fus.fus_dumpdev
};

/*
 * Types.
 */
#define DRIVER		1
#define NORMAL		2
#define	INVISIBLE	3
#define	PROFILING	4
#define	SYSTEMSPEC	5
#define	SWAPSPEC	6

/*
 * Attributes (flags).
 */
#define	CONFIGDEP	0x01	/* obsolete? */
#define	OPTIONSDEF	0x02	/* options definition entry */
#define ORDERED		0x04	/* don't list in OBJ's, keep "files" order */
#define SEDIT		0x08	/* run sed filter (SQT) */

/*
 * Maximum number of fields for variable device fields (SQT).
 */
#define	NFIELDS		10

struct	idlst {
	char	*id;
	struct	idlst *id_next;
	int	id_vec;		/* Sun interrupt vector number */
#ifdef	luna88k
	short	id_span;		/* ISI: distance to next vector */
#endif	luna88k
};

struct device {
	int	d_type;			/* CONTROLLER, DEVICE, bus adaptor */
	struct	device *d_conn;		/* what it is connected to */
	char	*d_name;		/* name of device (e.g. rk11) */
	struct	idlst *d_vec;		/* interrupt vectors */
	int	d_pri;			/* interrupt priority */
	int	d_addr;			/* address of csr */
	int	d_unit;			/* unit number */
	int	d_drive;		/* drive number */
	int	d_slave;		/* slave number */
#define QUES	-1	/* -1 means '?' */
#define	UNKNOWN -2	/* -2 means not set yet */
	int	d_dk;			/* if init 1 set to number for iostat */
	int	d_flags;		/* nlags for device init */
	struct	device *d_next;		/* Next one in list */
        u_short d_mach;                 /* Sun - machine type (0 = all)*/
        u_short d_bus;                  /* Sun - bus type (0 = unknown) */
	u_long	d_fields[NFIELDS];	/* fields values (SQT) */
	int	d_bin;			/* interrupt bin (SQT) */
	int	d_addrmod;		/* address modifier (MIPS) */
};
#define TO_NEXUS	(struct device *)-1
#define TO_SLOT		(struct device *)-1

struct config {
	char	*c_dev;
	char	*s_sysname;
};

/*
 * Config has a global notion of which machine type is
 * being used.  It uses the name of the machine in choosing
 * files and directories.  Thus if the name of the machine is ``vax'',
 * it will build from ``Makefile.vax'' and use ``../vax/inline''
 * in the makerules, etc.
 */
int	conftype;
char	*conftypename;
#define	CONFTYPE_VAX	1
#define	CONFTYPE_SUN	2
#define	CONFTYPE_ROMP	3
#define	CONFTYPE_SUN2	4
#define	CONFTYPE_SUN3	5
#define	CONFTYPE_MMAX	6
#define	CONFTYPE_SQT	7
#define CONFTYPE_SUN4	8
#define	CONFTYPE_I386	9
#define	CONFTYPE_IX	10
#define CONFTYPE_MIPSY	11
#define	CONFTYPE_MIPS	12
#define	CONFTYPE_I860	13
#define	CONFTYPE_MAC2	14
#define	CONFTYPE_SUN4C	15
#define	CONFTYPE_LUNA88K	16

/*
 * For each machine, a set of CPU's may be specified as supported.
 * These and the options (below) are put in the C flags in the makefile.
 */
struct platform {
	char		 *name;
	struct	platform *next;
} *platform;

/*
 * In order to configure and build outside the kernel source tree,
 * we may wish to specify where the source tree lives.
 */
char *source_directory;
char *config_directory;
char *object_directory;

FILE *fopenp();
char *get_VPATH();
#define VPATH	get_VPATH()

/*
 * A set of options may also be specified which are like CPU types,
 * but which may also specify values for the options.
 * A separate set of options may be defined for make-style options.
 */
struct opt {
	char	*op_name;
	char	*op_value;
	struct	opt *op_next;
}  *opt,
   *mkopt,
   *opt_tail,
   *mkopt_tail;

char	*ident;
char	*ns();
char	*tc();
char	*qu();
char	*get_word();
char	*path();
char	*raise();

int	do_trace;

char	*index();
char	*rindex();
char	*malloc();
char	*strcpy();
char	*strcat();

#if	CONFTYPE_VAX
int	seen_mba, seen_uba;
#endif

int	seen_vme, seen_mbii;

struct	device *connect();
struct	device *dtab;
dev_t	nametodev();
char	*devtoname();

char	errbuf[80];
int	yyline;

struct	file_list *ftab, *conf_list, **confp;
char	*PREFIX;

int	profiling;

#define eq(a,b)	(!strcmp(a,b))

#ifdef	mips
#define DEV_MASK 0xf
#define	DEV_SHIFT  4
#else	mips
#define DEV_MASK 0x7
#define	DEV_SHIFT  3
#endif	mips
