/******************************************************************************
 * SALLOC.H -- SALLOC version 1.0 Copyright (c) 1990 FWR Group
 ******************************************************************************/

/* salloc.h edit log:
 * 11/27/90 created version 1.0 zw
 */

/* stack memory allocator, instead of alloca() */

/* reserve stack space for salloc(), touch before and after */
#define SALLOC_SPACE(limit) \
  int salloc_last = 0; \
  int salloc_next = 0; \
  char salloc_space[limit]; \
  int salloc_limit = limit

/* allocate n bytes of memory from the stack */
/* does limit check, returns 0 if error */
#define salloc(n) \
  (char *)( \
    salloc_next = (salloc_last = salloc_next) + (n), \
    salloc_next > salloc_limit ? (salloc_next = salloc_last, 0) : \
      (int)&salloc_space[salloc_last] \
  )

