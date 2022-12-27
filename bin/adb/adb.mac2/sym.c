#ifndef lint
static	char sccsid[] = "@(#)sym.c	1.2 (Berkeley) 4/27/87";
#endif

/*
 * adb - symbol table routines
 */
#include "defs.h"
#include <stab.h>

/*
 * Lookup a symbol by name.
 */
struct nlist *lookup(char *symstr)
{
  register struct nlist *sp;

  cursym = 0;
  if (symtab) for (sp = symtab; sp < esymtab; sp++)
    /* SHOULD DO SOME OF EQSYM INLINE TO SAVE TIME */
    if ((sp->n_type&N_STAB)==0 && eqsym(sp->n_un.n_name, symstr, '_'))
      return (cursym = sp);
  return 0;
}

/*
 * Find the closest symbol to val, and return
 * the difference between val and the symbol found.
 * Leave a pointer to the symbol found as cursym.
 */
int findsym(int val, int type)
{
  int diff;
  struct nlist *sp;

  cursym = 0;
  diff = MAXINT;
  if (type == NSYM || symtab == 0) return diff;
  for (sp = symtab; sp < esymtab; sp++) {
    if (sp->n_type & N_STAB || !(sp->n_type & N_EXT)
        || sp->n_type == (N_FN|N_EXT))
      continue;
    if (val - sp->n_value < diff && val >= sp->n_value) {
      diff = val - sp->n_value;
      cursym = sp;
      if (diff == 0) break;
    }
  }
  return diff;
}

/*
 * Advance cursym to the next local variable.
 * Leave its value in localval as a side effect.
 * Return 0 at end of file.
 */
localsym(ADDR cframe)
{
  register int type;
  register struct nlist *sp;

  if (cursym) for (sp = cursym; ++sp < esymtab; ) {
    type = sp->n_type;
    if (sp->n_un.n_name[0] =='_' || type == N_FN) return 0;
    switch (type) {
      case N_TEXT:
      case N_TEXT|N_EXT:
      case N_DATA:
      case N_DATA|N_EXT:
      case N_BSS:
      case N_BSS|N_EXT:
	localval = sp->n_value;
	cursym = sp;
	return (1);
      case N_LSYM:
	localval = cframe - sp->n_value;
	cursym = sp;
	return (1);
      case N_PSYM:
      case N_ABS:
	localval = cframe + sp->n_value;
	cursym = sp;
	return (1);
    }
  }
  cursym = 0;
  return 0;
}

/*
 * Print value v and then the string s.
 * If v is not zero and the originalvalue was specified
 * with a symbol, then we look for a nearby symbol and
 * print name+offset if we find a symbol for which the
 * offset is small enough (less than maxoff).
 */
psymoff(int v, int type, char *s)
{
  register w;

  if (kernel && !s_addr) printf(LPRMODE, v);
  else if ((v == 0) || ((w = findsym(v, type)) >= maxoff)) printf(LPRMODE, v);
  else {
    printf("%s", cursym->n_un.n_name);
    if (w) printf(OFFMODE, w);
  }
  printf(s);
}

/*
 * Print value v symbolically if it has a reasonable
 * interpretation as name+offset.  If not, print nothing.
 * Used in printing out registers $r.
 */
void valpr(int v, int idsp)
{
  register off_t d;

#ifdef mac2
  /* kernel symbols are relocatable values */
  if (kernel) return;
#endif
  if ((d = findsym(v, idsp)) >= maxoff) return;
  printf("%s", cursym->n_un.n_name);
  if (d) printf(OFFMODE, d);
}
