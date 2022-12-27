/*	brkincr.h	4.1	82/05/07	*/

#ifdef mac2
#define BRKINCR (1024 * 1024)
#define BRKMAX (BRKINCR * 4)
#else
#define BRKINCR 01000
#define BRKMAX 04000
#endif
