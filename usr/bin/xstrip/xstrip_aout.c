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
 * $Log:	xstrip_aout.c,v $
 * Revision 2.2  91/08/29  16:47:37  jsb
 * 	First checkin.
 * 
 */
/*
 *	File:	xstrip_aout.c
 *	Author:	Joseph S. Barrera III
 *	Date:	1991
 */

#include <sys/types.h>
#include <stdio.h>
#include <ctype.h>
#include <a.out.h>
#include <stab.h>
#include <sys/stat.h>
#include <sys/resource.h>

/*
 * Convert
 *	../../../../../src/latest/kernel/vm/vm_map.c
 * to
 *	vm/vm_map.c
 * by going to end, backing up two slashes, and skipping one.
 */
trim_filename(sp)
	char **sp;
{
	char *s = *sp;
	char *t;
	int scount = 0;

	if (! *s) {
		return;
	}
	for (t = s; *t; t++) {
		continue;
	}
	for (; t > s; t--) {
		if (*t == '/') {
			if (++scount == 2) {
				*sp = t + 1;
				return;
			}
		}
	}
}

/*
 * Unrestrict data size limit, since we malloc large things.
 */
unlimit_data()
{
	struct rlimit rl;

	if (getrlimit(RLIMIT_DATA, &rl) != -1) {
		rl.rlim_cur = rl.rlim_max;
		setrlimit(RLIMIT_DATA, &rl);
	}
}

f_current_position(fd)
	int fd;
{
	int rv;

	rv = lseek(fd, 0, 1);
	if (rv == -1) {
		perror("f_current_position: lseek");
		exit(1);
	}
	return rv;
}

f_size(fd)
	int fd;
{
	struct stat st;
	int rv;

	rv = fstat(fd, &st);
	if (rv == -1) {
		perror("f_size: fstat");
		exit(1);
	}
	return st.st_size;
}

read_header(fd, exec, symtab_offset, symtab_size, strtab_offset, strtab_size)
	int fd;
	struct exec *exec;
	off_t *symtab_offset;
	int *symtab_size;
	off_t *strtab_offset;
	int *strtab_size;
{
	read(fd, (char *)exec, sizeof(*exec));
	if (N_BADMAG(*exec)) {
		fprintf(stderr, "bad magic\n");
		exit(1);
	}
	*symtab_offset = N_SYMOFF(*exec);
	*symtab_size = exec->a_syms;
	if (symtab_size == 0) {
		fprintf(stderr, "no name list\n");
		exit(1);
	}
	if (N_STROFF(*exec) + sizeof(off_t) > f_size(fd)) {
		fprintf(stderr, "no string table\n");
		exit(1);
	}
	*strtab_offset = *symtab_offset + *symtab_size;
	lseek(fd, *strtab_offset, 0);
	read(fd, strtab_size, sizeof(*strtab_size));
}

char *
read_table(fd, offset, size)
	int fd;
	off_t offset;
	int size;
{
	char *table;
	int rv;

	table = (char *) malloc(size);
	if (table == 0) {
		fprintf(stderr, "read_table: out of memory\n");
		exit(1);
	}
	lseek(fd, offset, 0);
	rv = read(fd, table, size);
	if (rv < 0) {
		perror("read_table: read");
		exit(1);
	}
	if (rv != size) {
		fprintf(stderr, "read_table: short read\n");
		exit(1);
	}
	return table;
}

symtab_strip(symtab, symtab_size)
	struct nlist **symtab;
	int *symtab_size;
{
	register struct nlist *sp, *ep, *np;
	struct nlist *new_symtab;

	new_symtab = (struct nlist *) malloc(*symtab_size);
	if (new_symtab == 0) {
		fprintf(stderr, "symtab_strip: out of memory\n");
		exit(1);
	}
	ep = &(*symtab)[*symtab_size / sizeof(struct nlist)];
	np = new_symtab;
	for (sp = *symtab; sp < ep; sp++) {
		if (sp->n_type & N_STAB) {
			if ((sp->n_type & 0xff) == N_SLINE ||
			    (sp->n_type & 0xff) == N_SO) {
				*np++ = *sp;
				continue;
			}
		}
		if (sp->n_type & N_EXT) {
			*np++ = *sp;
			continue;
		}
	}
	printf("stripped %d bytes from symbol table\n",
	       *symtab_size - (np - new_symtab) * sizeof(*np));
	free(*symtab);
	*symtab = new_symtab;
	*symtab_size = (np - new_symtab) * sizeof(*np);
}

strtab_strip(strtab, strtab_size, symtab, symtab_size)
	char **strtab;
	int *strtab_size;
	struct nlist *symtab;
	int symtab_size;
{
	register struct nlist *sp, *ep;
	char *new_strtab, *s;
	int new_strtab_size, len;

	new_strtab = (char *) malloc(*strtab_size);
	if (new_strtab == 0) {
		fprintf(stderr, "strtab_strip: out of memory\n");
		exit(1);
	}
	new_strtab_size = sizeof(new_strtab_size);

	ep = &symtab[symtab_size / sizeof(*ep)];
	for (sp = symtab; sp < ep; sp++) {
		if (sp->n_un.n_strx == 0) {
			continue;
		}
		s = sp->n_un.n_strx + *strtab;
		if ((sp->n_type & N_STAB) && (sp->n_type & 0xff) == N_SO) {
			trim_filename(&s);
		}
		len = strlen(s) + 1;
		bcopy(s, new_strtab + new_strtab_size, len);
		sp->n_un.n_strx = new_strtab_size;
		new_strtab_size += len;
	}
	printf("stripped %d bytes from string table\n",
	       *strtab_size - new_strtab_size);
	* (int *) new_strtab = new_strtab_size;
	free(*strtab);
	*strtab = new_strtab;
	*strtab_size = new_strtab_size;
}

write_header(fd, exec, symtab_size)
	int fd;
	struct exec *exec;
	int symtab_size;
{
	int rv;

	exec->a_syms = symtab_size;
	lseek(fd, (char *) &exec->a_syms - (char *) exec, 0);
	rv = write(fd, (char *) &exec->a_syms, sizeof(exec->a_syms));
	if (rv < 0) {
		perror("write_header: write");
		exit(1);
	}
	if (rv != sizeof(exec->a_syms)) {
		fprintf(stderr, "write_header: short write");
		exit(1);
	}
}

write_table(fd, table, offset, size)
	int fd;
	char *table;
	off_t offset;
	int size;
{
	int rv;

	lseek(fd, offset, 0);
	rv = write(fd, table, size);
	if (rv < 0) {
		perror("write_table: write");
		exit(1);
	}
	if (rv != size) {
		fprintf(stderr, "write_table: short write\n");
		exit(1);
	}
	free(table);
}

main(argc, argv)
	int argc;
	char **argv;
{
	struct exec exec;
	int fd;
	char *strtab;
	struct nlist *symtab;
	off_t symtab_offset, strtab_offset;
	int symtab_size, strtab_size;

	unlimit_data();

	/*
	 * Open file
	 */
	if (argc != 2) {
		fprintf(stderr, "usage: %s filename\n", argv[0]);
		exit(1);
	}
	fd = open(argv[1], 2);
	if (fd < 0) {
		perror(argv[1]);
		exit(1);
	}

	/*
	 * Read header.
	 */
	read_header(fd, &exec, &symtab_offset, &symtab_size,
		    &strtab_offset, &strtab_size);

	/*
	 * Read and process symbol table.
	 */
	symtab = (struct nlist *) read_table(fd, symtab_offset, symtab_size);
	symtab_strip(&symtab, &symtab_size);

	/*
	 * Read and process string table.
	 */
	strtab = read_table(fd, strtab_offset, strtab_size);
	strtab_strip(&strtab, &strtab_size, symtab, symtab_size);

	/*
	 * Write header, symbol table, and string table.
	 */
	write_header(fd, &exec, symtab_size);
	write_table(fd, symtab, symtab_offset, symtab_size);
	strtab_offset = f_current_position(fd);
	write_table(fd, strtab, strtab_offset, strtab_size);

	/*
	 * Truncate file at current file pointer position, close, and exit.
	 */
	printf("%s: truncating from %d to %d (stripped = %d)\n",
	       argv[1],
	       f_size(fd),
	       f_current_position(fd),
	       f_size(fd) - f_current_position(fd));
	ftruncate(fd, f_current_position(fd));
	close(fd);
	exit(0);
}
