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
 * $Log:	db_sym.c,v $
 * Revision 2.9  91/07/31  17:31:14  dbg
 * 	Add task pointer and space for string storage to symbol table
 * 	descriptor.
 * 	[91/07/31            dbg]
 * 
 * Revision 2.8  91/07/09  23:16:08  danner
 * 	Changed a printf.
 * 	[91/07/08            danner]
 * 
 * Revision 2.7  91/05/14  15:35:54  mrt
 * 	Correcting copyright
 * 
 * Revision 2.6  91/03/16  14:42:40  rpd
 * 	Changed the default db_maxoff to 4K.
 * 	[91/03/10            rpd]
 * 
 * Revision 2.5  91/02/05  17:07:07  mrt
 * 	Changed to new Mach copyright
 * 	[91/01/31  16:19:17  mrt]
 * 
 * Revision 2.4  90/10/25  14:44:05  rwd
 * 	Changed db_printsym to print unsigned.
 * 	[90/10/19            rpd]
 * 
 * Revision 2.3  90/09/09  23:19:56  rpd
 * 	Avoid totally incorrect guesses of symbol names for small values.
 * 	[90/08/30  17:39:48  af]
 * 
 * Revision 2.2  90/08/27  21:52:18  dbg
 * 	Removed nlist.h.  Fixed some type declarations.
 * 	Qualifier character is ':'.
 * 	[90/08/20            dbg]
 * 	Modularized symtab info into a new db_symtab_t type.
 * 	Modified db_add_symbol_table  and others accordingly.
 * 	Defined db_sym_t, a new (opaque) type used to represent
 * 	symbols.  This should support all sort of future symtable
 * 	formats. Functions like db_qualify take a db_sym_t now.
 * 	New db_symbol_values() function to explode the content
 * 	of a db_sym_t.
 * 	db_search_symbol() replaces db_find_sym_and_offset(), which is
 * 	now a macro defined in our (new) header file.  This new
 * 	function accepts more restrictive searches, which are
 * 	entirely delegated to the symtab-specific code.
 * 	Accordingly, db_printsym() accepts a strategy parameter.
 * 	New db_line_at_pc() function.
 * 	Renamed misleading db_eqsym into db_eqname.
 * 	[90/08/20  10:47:06  af]
 * 
 * 	Created.
 * 	[90/07/25            dbg]
 * 
 * Revision 2.1  90/07/26  16:43:52  dbg
 * Created.
 * 
 */
/*
 * 	Author: David B. Golub, Carnegie Mellon University
 *	Date:	7/90
 */

#include <machine/db_machdep.h>
#include <ddb/db_sym.h>

#include <vm/vm_map.h>	/* vm_map_t */

/*
 * We import from the symbol-table dependent routines:
 */
extern db_sym_t	X_db_lookup();
extern db_sym_t	X_db_search_symbol();
extern boolean_t X_db_line_at_pc();
extern void	X_db_symbol_values();

/*
 * Multiple symbol tables
 */
#define	MAXNOSYMTABS	3	/* mach, ux, emulator */

db_symtab_t	db_symtabs[MAXNOSYMTABS] = {{0,},};
int db_nsymtab = 0;

db_symtab_t	*db_last_symtab;

db_sym_t	db_lookup();	/* forward */

/*
 * Add symbol table, with given name, to list of symbol tables.
 */
boolean_t
db_add_symbol_table(start, end, name, ref, map_pointer)
	char *start;
	char *end;
	char *name;
	char *ref;
	char *map_pointer;
{
	register db_symtab_t *st;

	if (db_nsymtab >= MAXNOSYMTABS)
	    return (FALSE);

	st = &db_symtabs[db_nsymtab];
	st->start = start;
	st->end = end;
	st->private = ref;
	st->map_pointer = map_pointer;
	strcpy(st->name, name);

	db_nsymtab++;

	return (TRUE);
}

/*
 *  db_qualify("vm_map", "ux") returns "unix:vm_map".
 *
 *  Note: return value points to static data whose content is
 *  overwritten by each call... but in practice this seems okay.
 */
static char *
db_qualify(sym, symtabname)
	db_sym_t	sym;
	register char	*symtabname;
{
	char		*symname;
	static char     tmp[256];
	register char	*s;

	db_symbol_values(sym, &symname, 0);
	s = tmp;
	while (*s++ = *symtabname++) {
	}
	s[-1] = ':';
	while (*s++ = *symname++) {
	}
	return tmp;
}


boolean_t
db_eqname(src, dst, c)
	char *src;
	char *dst;
	char c;
{
	if (!strcmp(src, dst))
	    return (TRUE);
	if (src[0] == c)
	    return (!strcmp(src+1,dst));
	return (FALSE);
}

boolean_t
db_value_of_name(name, valuep)
	char		*name;
	db_expr_t	*valuep;
{
	db_sym_t	sym;

	sym = db_lookup(name);
	if (sym == DB_SYM_NULL)
	    return (FALSE);
	db_symbol_values(sym, &name, valuep);
	return (TRUE);
}


/*
 * Lookup a symbol.
 * If the symbol has a qualifier (e.g., ux:vm_map),
 * then only the specified symbol table will be searched;
 * otherwise, all symbol tables will be searched.
 */
db_sym_t
db_lookup(symstr)
	char *symstr;
{
	db_sym_t sp;
	register int i;
	int symtab_start = 0;
	int symtab_end = db_nsymtab;
	register char *cp;

	/*
	 * Look for, remove, and remember any symbol table specifier.
	 */
	for (cp = symstr; *cp; cp++) {
		if (*cp == ':') {
			*cp = '\0';
			for (i = 0; i < db_nsymtab; i++) {
				if (! strcmp(symstr, db_symtabs[i].name)) {
					symtab_start = i;
					symtab_end = i + 1;
					break;
				}
			}
			*cp = ':';
			if (i == db_nsymtab) {
				db_error("invalid symbol table name");
			}
			symstr = cp+1;
		}
	}

	/*
	 * Look in the specified set of symbol tables.
	 * Return on first match.
	 */
	for (i = symtab_start; i < symtab_end; i++) {
		if (sp = X_db_lookup(&db_symtabs[i], symstr)) {
			db_last_symtab = &db_symtabs[i];
			return sp;
		}
	}
	return 0;
}

/*
 * Does this symbol name appear in more than one symbol table?
 * Used by db_symbol_values to decide whether to qualify a symbol.
 */
boolean_t db_qualify_ambiguous_names = FALSE;

boolean_t
db_symbol_is_ambiguous(sym)
	db_sym_t	sym;
{
	char		*sym_name;
	register int	i;
	register
	boolean_t	found_once = FALSE;

	if (!db_qualify_ambiguous_names)
		return FALSE;

	db_symbol_values(sym, &sym_name, 0);
	for (i = 0; i < db_nsymtab; i++) {
		if (X_db_lookup(&db_symtabs[i], sym_name)) {
			if (found_once)
				return TRUE;
			found_once = TRUE;
		}
	}
	return FALSE;
}

/*
 * Find the closest symbol to val, and return its name
 * and the difference between val and the symbol found.
 * If a symbol table has a map pointer, only use that
 * symbol table if the map pointer matches val.
 */
extern vm_map_t		db_map_addr();
extern boolean_t	db_map_equal();

db_sym_t
db_search_symbol( val, strategy, offp)
	register db_addr_t	val;
	db_strategy_t		strategy;
	db_expr_t		*offp;
{
	register
	unsigned int	diff;
	unsigned int	newdiff;
	register int	i;
	db_symtab_t	*sp;
	db_sym_t	ret = DB_SYM_NULL, sym;
	vm_map_t	map_for_val = db_map_addr(val);

	newdiff = diff = ~0;
	db_last_symtab = 0;
	for (sp = &db_symtabs[0], i = 0;
	     i < db_nsymtab;
	     sp++, i++) {
	    if ((vm_map_t)sp->map_pointer == VM_MAP_NULL ||
		db_map_equal((vm_map_t)sp->map_pointer, map_for_val))
	    {
		sym = X_db_search_symbol(sp, val, strategy, &newdiff);
		if (newdiff < diff) {
		    db_last_symtab = sp;
		    diff = newdiff;
		    ret = sym;
		}
	    }
	}
	*offp = diff;
	return ret;
}

/*
 * Return name and value of a symbol
 */
void
db_symbol_values(sym, namep, valuep)
	db_sym_t	sym;
	char		**namep;
	db_expr_t	*valuep;
{
	db_expr_t	value;

	if (sym == DB_SYM_NULL) {
		*namep = 0;
		return;
	}

	X_db_symbol_values(sym, namep, &value);

	if (db_symbol_is_ambiguous(sym))
		*namep = db_qualify(sym, db_last_symtab->name);
	if (valuep)
		*valuep = value;
}


/*
 * Print a the closest symbol to value
 *
 * After matching the symbol according to the given strategy
 * we print it in the name+offset format, provided the symbol's
 * value is close enough (eg smaller than db_maxoff).
 * We also attempt to print [filename:linenum] when applicable
 * (eg for procedure names).
 *
 * If we could not find a reasonable name+offset representation,
 * then we just print the value in hex.  Small values might get
 * bogus symbol associations, e.g. 3 might get some absolute
 * value like _INCLUDE_VERSION or something, therefore we do
 * not accept symbols whose value is zero (and use plain hex).
 */

unsigned int	db_maxoff = 0x1000;

void
db_printsym(off, strategy)
	db_expr_t	off;
	db_strategy_t	strategy;
{
	db_expr_t	d;
	char 		*filename;
	char		*name;
	db_expr_t	value;
	int 		linenum;
	db_sym_t	cursym;

	cursym = db_search_symbol(off, strategy, &d);
	db_symbol_values(cursym, &name, &value);
	if (name == 0 || d >= db_maxoff || value == 0) {
		db_printf("%#n", off);
		return;
	}
	db_printf("%s", name);
	if (d)
		db_printf("+0x%x", d);
	if (strategy == DB_STGY_PROC) {
		if (db_line_at_pc(cursym, &filename, &linenum, off))
			db_printf(" [%s:%d]", filename, linenum);
	}
}


boolean_t
db_line_at_pc( sym, filename, linenum, pc)
{
	return X_db_line_at_pc( db_last_symtab, sym, filename, linenum, pc);
}
