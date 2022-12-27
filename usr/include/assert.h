/*	assert.h	4.2	85/01/21	*/

/* NOTE -- see /lib/gcc-include/assert.h for another definition */

# ifndef NDEBUG
# define _assert(ex)	{if (!(ex)){fprintf(stderr,"Assertion failed: file \"%s\", line %d\n", __FILE__, __LINE__);exit(2);}}
# define assert(ex)	_assert(ex)
# else
# define _assert(ex)
# define assert(ex)
# endif
