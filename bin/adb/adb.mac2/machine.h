/*	machine.h	1.2	87/06/25	*/

#ifndef mach3
#include <sys/vm.h>
#endif

#define itol68(a,b)	((a << 16) | (b & 0xFFFF))


#define DBNAME "adb\n"
#define LPRMODE "%R"
#define OFFMODE "+%R"
#define	TXTRNDSIZ	PAGSIZ

#define	MAXINT	0x7fffffff
#define	MAXSTOR (KERNBASE - ctob(1))
#define	MAXFILE 0xffffffff

#define FP AR6
#define PS (AR7+1)
#define PC (AR7+2)

/*
 * INSTACK tells whether its argument is a stack address.
 * INUDOT tells whether its argument is in the (extended) u. area.
 * These are used for consistency checking and dont have to be exact.
 *
 * INKERNEL tells whether its argument is a kernel space address.
 * KVTOPH trims a kernel virtal address back to its offset
 * in the kernel address space.
 */
#define	INSTACK(x)	( (x) >= MINSTACK && (x) <= MAXSTACK )
#define	INUDOT(x)	(INSTACK(x) && (x) >= MAXSTOR)
int MINSTACK, MAXSTACK;

#define KERNBASE	0
#define	KERNOFF		(KERNBASE)	/* start of kernel's text */
#define	KVTOPH(x)	(x)
