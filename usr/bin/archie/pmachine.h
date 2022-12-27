/*
 * Miscellaneous system dependencies.
 *
 * I kept the name pmachine.h because it was already in all of the files...this
 * barely resembles the pmachine.h that comes with the real Prospero, tho.
 */

#if defined(NOREGEX)
#define REGEX_FILE "regex.c" /* originally "../../misc/regex.c in wcmatch.c */
#endif

#ifdef FUNCS
#define index		strchr
#define rindex		strrchr
#ifndef AUX
#define bcopy(a,b,n)	memcpy(b,a,n)
#define bzero(a,n)	memset(a,0,n)
#endif
#endif

/*
 * FD_SET: lib/pfs/dirsend.c, user/vget/ftp.c
 */
#ifndef FD_SET
#define	NFDBITS		32
#define	FD_SETSIZE	32
#define	FD_SET(n, p)	((p)->fds_bits[(n)/NFDBITS] |= (1 << ((n) % NFDBITS)))
#define	FD_CLR(n, p)	((p)->fds_bits[(n)/NFDBITS] &= ~(1 << ((n) % NFDBITS)))
#define	FD_ISSET(n, p)	((p)->fds_bits[(n)/NFDBITS] & (1 << ((n) % NFDBITS)))
#define FD_ZERO(p)	bzero((char *)(p), sizeof(*(p)))
#endif
