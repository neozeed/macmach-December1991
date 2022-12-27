/* Allow this file to be included multiple times
   with different settings of NDEBUG.  */
#undef assert
#undef _assert
#undef __assert

#ifdef NDEBUG
#define assert(ignore)  ((void)0)
#define _assert(ignore)  ((void)0)
#else

void __eprintf ();		/* Defined in gnulib */

#ifdef __STDC__

#define _assert(expression)  \
  ((void) ((expression) ? 0 : __assert (#expression, __FILE__, __LINE__)))

#define __assert(expression, file, lineno)  \
  (__eprintf ("Failed assertion `%s' at line %d of `%s'.\n",	\
	      expression, lineno, file), 0)

#else /* no __STDC__; i.e. -traditional.  */

#define _assert(expression)  \
  ((void) ((expression) ? 0 : __assert (expression, __FILE__, __LINE__)))

#define __assert(expression, file, lineno)  \
  (__eprintf ("Failed assertion `%s' at line %d of `%s'.\n",	\
	      "expression", lineno, file), 0)

#endif /* no __STDC__; i.e. -traditional.  */

#define assert(expression) _assert(expression)

#endif
