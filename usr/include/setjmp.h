/*	setjmp.h	4.1	83/05/03	*/
/*  8-Jul-91  Zon Williamson (zon) at Carnegie Mellon University
 *	Added mc68000 version for mac2.
 *	See /usr/src/lib/libc/gen/gen.mac2/setjmp.S for more info.
 */

#ifndef _SETJMP_H_
#define _SETJMP_H_

#ifdef mc68000
typedef int jmp_buf[14]; /* ps, sigmask, d2-d7, a2-a7 */
#else
typedef int jmp_buf[10];
#endif

#endif /* _SETJMP_H_ */
