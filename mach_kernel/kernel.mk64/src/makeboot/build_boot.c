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
 * $Log:	build_boot.c,v $
 * Revision 2.2  91/05/08  13:09:36  dbg
 * 	Created.
 * 	[91/02/26            dbg]
 * 
 * Revision 2.1.1.1  91/02/26  11:17:55  dbg
 * 	Created.
 * 	[91/02/26            dbg]
 * 
 */

/*
 * Build a boot file for the Sequent Symmetry.
 * Will eventually tack on the boot file, also...
 * but not quite yet.
 */
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/file.h>
#include <mach.h>

#include <a.out.h>

#include "loader_info.h"

char *	kernel_name;
char *	boot_file_name = "mach.boot";

int	kern_file;
int	out_file;

int	kern_symbols = 1;
int	is_Sequent = 0;

#define	SQT_K_MAGIC	0x42eb

usage()
{
	printf("usage: build_sqt_boot [ -sequent] [ -o boot_file ] kernel\n");
	exit(1);
}

main(argc, argv)
	int	argc;
	char	**argv;
{
	argc--, argv++;	/* skip program name */

	if (argc == 0)
	    usage();

	/*
	 * Parse switches.
	 */
	while (argc > 0 && **argv == '-') {
	    char *s;

	    s = *argv;
	    if (s[1] == 'o') {
		/* output file name */
		argc--, argv++;
		if (argc == 0)
		    usage();
		boot_file_name = *argv;
		argc--, argv++;
	    }
	    else if (!strcmp(&s[1], "sequent")) {
		is_Sequent = 1;
		argc--, argv++;
	    }
	    else {
		printf("unknown switch: %s\n", s);
	    }
	}

	if (argc != 1)
	    usage();

	kernel_name = argv[0];

	kern_file = check_and_open(kernel_name);

	out_file = creat(boot_file_name, 0777);	/* XXX mode */
	if (out_file < 0) {
	    perror(boot_file_name);
	    exit(2);
	}

	build_boot();

	close(out_file);
	close(kern_file);

	exit(0);
}

int
check_and_open(fname)
	char *fname;
{
	int f;
	struct stat statb;

	if (stat(fname, &statb) < 0) {
	    perror(fname);
	    exit(2);
	}
	if ((statb.st_mode & S_IFMT) != S_IFREG ||
	    (statb.st_mode & (S_IEXEC|(S_IEXEC>>3)|(S_IEXEC>>6))) == 0) {
		printf("build_boot: %s: not an executable file\n",
			fname);
		exit(2);
	}

	f = open(fname, O_RDONLY, 0);
	if (f < 0) {
	    perror(fname);
	    exit(2);
	}
	return (f);
}

/*
 * Create the boot file.
 */
build_boot()
{
	struct exec	out_header;

	struct loader_info	kern_header;

	off_t	off;

	/*
	 * Read in the kernel header.
	 */
	if (!ex_get_header(kern_file, &kern_header)) {
	    printf("%s: not an executable file\n", kernel_name);
	    exit(4);
	}

	/*
	 * Copy its text and data to the text section of the output file.
	 */
	lseek(out_file, (off_t) sizeof(struct exec), L_SET);

	lseek(kern_file, kern_header.text_offset, L_SET);
	file_copy(out_file, kern_file, kern_header.text_size);

	lseek(kern_file, kern_header.data_offset, L_SET);
	file_copy(out_file, kern_file, kern_header.data_size);

	/*
	 * Seek to the kernel symbol table and add that.
	 * If no symbol table, set its size to zero.
	 */
	if (kern_symbols && kern_header.sym_size != 0) {
	    write(out_file, (char *)&kern_header.sym_size, sizeof(int));
	    /* NList and string table */
	    lseek(kern_file, kern_header.sym_offset, L_SET);
	    file_copy(out_file, kern_file,
			kern_header.sym_size + kern_header.str_size);
	}
	else {
	    /* Zero size for symbol table. */
	    kern_header.sym_size = 0;
	    write(out_file, (char *)&kern_header.sym_size, sizeof(int));
	}

	/*
	 * Round to an integer boundary in the file.
	 */
	off = lseek(out_file, (off_t) 0, L_INCR);
	if (off % sizeof(int)) {
	    off = (off + sizeof(int) - 1) & ~(sizeof(int)-1);
	    lseek(out_file, off, L_SET);
	}

	/*
	 * Build the a.out header and write it out.
	 * If a Sequent, patch up the header for the
	 * prom loader.
	 */

	if (is_Sequent)
	    out_header.a_magic = SQT_K_MAGIC;
	else
	    out_header.a_magic = OMAGIC;
	out_header.a_text = (int) off - sizeof(struct exec);
	out_header.a_data = 0;
	out_header.a_syms = 0;
	out_header.a_bss  = 0;
	out_header.a_trsize = 0;
	out_header.a_drsize = 0;
	if (is_Sequent)
	    out_header.a_entry = kern_header.entry_1 & 0x0fffffff;
	else
	    out_header.a_entry = kern_header.entry_1;

	lseek(out_file, (off_t) 0, L_SET);
	write(out_file, (char *)&out_header, sizeof(out_header));
}

int
ex_get_header(in_file, lp)
	int	in_file;
	struct loader_info *lp;
{
	struct exec	x;
	int		str_size;

	lseek(in_file, (long)0, L_SET);
	read(in_file, (char *)&x, sizeof(x));

#ifdef	i386
	switch ((int)x.a_magic) {
	    case 0407:
		lp->text_start  = 0;
		lp->text_size   = 0;
		lp->text_offset = 0;
		lp->data_start  = 0x10000;
		lp->data_size	= x.a_text + x.a_data;
		lp->data_offset = sizeof(struct exec);
		lp->bss_size	= x.a_bss;
		break;

	    case 0410:
		if (x.a_text == 0) {
		    return (0);
		}
		lp->text_start	= 0x10000;
		lp->text_size	= x.a_text;
		lp->text_offset	= sizeof(struct exec);
		lp->data_start	= lp->text_start + lp->text_size;
		lp->data_size	= x.a_data;
		lp->data_offset	= lp->text_offset + lp->text_size;
		lp->bss_size	= x.a_bss;
		break;

	    case 0413:
		if (x.a_text == 0) {
		    return (0);
		}
		lp->text_start	= 0x10000;
		lp->text_size	= sizeof(struct exec) + x.a_text;
		lp->text_offset	= 0;
		lp->data_start	= lp->text_start + lp->text_size;
		lp->data_size	= x.a_data;
		lp->data_offset	= lp->text_offset + lp->text_size;
		lp->bss_size	= x.a_bss;
		/*
		 * If loading a Sequent boot file, treat as 0410.
		 */
		if (is_Sequent) {
		    lp->text_size -= sizeof(struct exec);
		    lp->text_offset = sizeof(struct exec);
		}
		break;

	    default:
		return (0);
	}
	lp->entry_1 = x.a_entry;
	lp->entry_2 = 0;
#endif
	/*
	 * Find symbol table for a.out format.
	 */
	lp->sym_offset = lp->data_offset
			+ lp->data_size;
	lp->sym_size = x.a_syms;

	/*
	 * And string table size.
	 */
	lseek(in_file, (off_t) (lp->sym_offset+lp->sym_size), L_SET);
	read(in_file, (char *)&str_size, sizeof(str_size));

	lp->str_size = str_size;

	return (1);
}

check_read(f, addr, size)
	int	f;
	char *	addr;
	int	size;
{
	if (read(f, addr, size) != size) {
	    perror("read");
	    exit(6);
	}
}

/*
 * Copy N bytes from in_file to out_file
 */
file_copy(out_f, in_f, size)
	int	out_f;
	int	in_f;
	int	size;
{
	char	buf[4096];

	while (size >= sizeof(buf)) {
	    check_read(in_f, buf, sizeof(buf));
	    write(out_f, buf, sizeof(buf));
	    size -= sizeof(buf);
	}
	if (size > 0) {
	    check_read(in_f, buf, size);
	    write(out_f, buf, size);
	}
}

