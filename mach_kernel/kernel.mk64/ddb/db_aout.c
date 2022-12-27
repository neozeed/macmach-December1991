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
 * CARNEGIE MELLON ALLOWS FREE USE OF THIS SOFTWARE IN ITS "AS IS"
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
 * any improvements or extensions that they make and grant Carnegie Mellon
 * the rights to redistribute these changes.
 */
/*
 * HISTORY
 * $Log:	db_aout.c,v $
 * Revision 2.8  91/08/28  11:10:58  jsb
 * 	Added line number support, via X_db_line_at_pc.
 * 	[91/08/13  18:12:37  jsb]
 * 
 * Revision 2.7  91/07/31  17:29:43  dbg
 * 	Removed read_symtab_from_file.
 * 	Added task argument to X_db_sym_init.
 * 	[91/07/30  16:42:51  dbg]
 * 
 * Revision 2.6  91/07/09  23:15:35  danner
 * 	On luna, kdb_init needs to be called ddb_init. Add ifndef
 * 	 DB_SYMBOLS_PRELOADED for machines that use a.out format but
 * 	 whose prom loaders load generously load the symbol table.
 * 	[91/07/08            danner]
 * 
 * Revision 2.5  91/05/14  15:32:00  mrt
 * 	Correcting copyright
 * 
 * Revision 2.4  91/03/16  14:42:23  rpd
 * 	Updated for new kmem_alloc interface.
 * 	[91/03/03            rpd]
 * 
 * Revision 2.3  91/02/05  17:05:55  mrt
 * 	Changed to new Mach copyright
 * 	[91/01/31  16:16:44  mrt]
 * 
 * Revision 2.2  90/08/27  21:48:35  dbg
 * 	Created.
 * 	[90/08/17            dbg]
 * 
 */
/*
 *	Author: David B. Golub, Carnegie Mellon University
 *	Date:	7/90
 */
/*
 * Symbol table routines for a.out format files.
 */

#include <mach/boolean.h>
#include <machine/db_machdep.h>		/* data types */
#include <ddb/db_sym.h>

#ifndef	DB_NO_AOUT

#include <ddb/nlist.h>			/* a.out symbol table */

/*
 * An a.out symbol table as loaded into the kernel debugger:
 *
 * symtab	-> size of symbol entries, in bytes
 * sp		-> first symbol entry
 *		   ...
 * ep		-> last symbol entry + 1
 * strtab	== start of string table
 *		   size of string table in bytes,
 *		   including this word
 *		-> strings
 */

/*
 * Find pointers to the start and end of the symbol entries,
 * given a pointer to the start of the symbol table.
 */
#define	db_get_aout_symtab(symtab, sp, ep) \
	(sp = (struct nlist *)((symtab) + 1), \
	 ep = (struct nlist *)((char *)sp + *(symtab)))

boolean_t
X_db_sym_init(symtab, esymtab, name, task_addr)
	char *	symtab;		/* pointer to start of symbol table */
	char *	esymtab;	/* pointer to end of string table,
				   for checking - may be rounded up to
				   integer boundary */
	char *	name;
	char *	task_addr;	/* use for this task only */
{
	register struct nlist	*sym_start, *sym_end;
	register struct nlist	*sp;
	register char *	strtab;
	register int	strlen;
	char *		estrtab;

	db_get_aout_symtab((int *)symtab, sym_start, sym_end);

	strtab = (char *)sym_end;
	strlen = *(int *)strtab;
	estrtab = strtab + strlen;

#define	round_to_int(x)	(((int)(x) + sizeof(int) - 1) & ~(sizeof(int) - 1))

	if (round_to_int(estrtab) != round_to_int(esymtab)) {
	    db_printf("[ %s symbol table not valid ]\n", name);
	    return (FALSE);
	}

#undef	round_to_int

	db_printf("[ preserving %d bytes of %s symbol table ]\n",
		esymtab - (char *)symtab, name);

	for (sp = sym_start; sp < sym_end; sp++) {
	    register int strx;
	    strx = sp->n_un.n_strx;
	    if (strx != 0) {
		if (strx > strlen) {
		    db_printf("Bad string table index (%#x)\n", strx);
		    sp->n_un.n_name = 0;
		    continue;
		}
		sp->n_un.n_name = strtab + strx;
	    }
	}

	return db_add_symbol_table(sym_start,
				sym_end,
				name,
				symtab,
				task_addr);
}

db_sym_t
X_db_lookup(stab, symstr)
	db_symtab_t	*stab;
	char *		symstr;
{
	register struct nlist *sp, *ep;

	sp = (struct nlist *)stab->start;
	ep = (struct nlist *)stab->end;

	for (; sp < ep; sp++) {
	    if (sp->n_un.n_name == 0)
		continue;
	    if ((sp->n_type & N_STAB) == 0 &&
		sp->n_un.n_name != 0 &&
		db_eqname(sp->n_un.n_name, symstr, '_'))
	    {
		return ((db_sym_t)sp);
	    }
	}
	return ((db_sym_t)0);
}

db_sym_t
X_db_search_symbol(symtab, off, strategy, diffp)
	db_symtab_t *	symtab;
	register
	db_addr_t	off;
	db_strategy_t	strategy;
	db_expr_t	*diffp;		/* in/out */
{
	register unsigned int	diff = *diffp;
	register struct nlist	*symp = 0;
	register struct nlist	*sp, *ep;

	sp = (struct nlist *)symtab->start;
	ep = (struct nlist *)symtab->end;

	for (; sp < ep; sp++) {
	    if (sp->n_un.n_name == 0)
		continue;
	    if ((sp->n_type & N_STAB) != 0)
		continue;
	    if (off >= sp->n_value) {
		if (off - sp->n_value < diff) {
		    diff = off - sp->n_value;
		    symp = sp;
		    if (diff == 0)
			break;
		}
		else if (off - sp->n_value == diff) {
		    if (symp == 0)
			symp = sp;
		    else if ((symp->n_type & N_EXT) == 0 &&
				(sp->n_type & N_EXT) != 0)
			symp = sp;	/* pick the external symbol */
		}
	    }
	}
	if (symp == 0) {
	    *diffp = off;
	}
	else {
	    *diffp = diff;
	}
	return ((db_sym_t)symp);
}

/*
 * Return the name and value for a symbol.
 */
void
X_db_symbol_values(sym, namep, valuep)
	db_sym_t	sym;
	char		**namep;
	db_expr_t	*valuep;
{
	register struct nlist *sp;

	sp = (struct nlist *)sym;
	if (namep)
	    *namep = sp->n_un.n_name;
	if (valuep)
	    *valuep = sp->n_value;
}

/*
 * Find filename and lineno within, given the current pc.
 */

#define	N_SLINE	0x44	/* source line number */
#define	N_SO	0x64	/* source file name */

boolean_t
X_db_line_at_pc(symtab, sym, filenamep, linep, pc)
	db_symtab_t	*symtab;
	db_sym_t	sym;		/* unused for a.out */
	char		**filenamep;
	int		*linep;
	db_expr_t	pc;
{
	register struct nlist *sp, *ep, *best_lsp, *best_fsp;
	unsigned long diff, best_ldiff, best_fdiff;

	sp = (struct nlist *)symtab->start;
	ep = (struct nlist *)symtab->end;
	best_lsp = (struct nlist *)0;
	best_fsp = (struct nlist *)0;
	best_ldiff = (unsigned long)-1;
	best_fdiff = (unsigned long)-1;

	for (; sp < ep; sp++) {
		if ((sp->n_type & N_STAB) == 0) {
			continue;
		}
		if ((sp->n_type & 0xff) == N_SLINE) {
			diff = pc - sp->n_value;
			if (diff < best_ldiff) {
				best_ldiff = diff;
				best_lsp = sp;
			}
		} else if ((sp->n_type & 0xff) == N_SO) {
			diff = pc - sp->n_value;
			if (diff < best_fdiff) {
				best_fdiff = diff;
				best_fsp = sp;
			}
		}
	}
	if (best_fsp) {
		*filenamep = best_fsp->n_un.n_name;
	} else {
		*filenamep = "?";
	}
	if (best_lsp) {
		*linep = best_lsp->n_desc & 0xffff;
	} else {
		*linep = 0;
	}
	return ((best_fsp != 0) || (best_lsp != 0));
}

/*
 * Initialization routine for a.out files.
 */
#ifdef luna88k
ddb_init()
#else
kdb_init()
#endif
{
	extern char	*esym;
	extern int	end;

	if (esym > (char *)&end) {
	    X_db_sym_init((char *)&end, esym, "mach", (char *)0);
	}
}

#endif	/* DB_NO_AOUT */
