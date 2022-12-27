/*
 * HISTORY
 * $Log:	emulator_base.c,v $
 * Revision 2.4  91/09/03  11:11:31  jsb
 * 	Conditionalize on TARGET_MACHINE to allow cross-compilation.
 * 	Add -d for I860.
 * 	[91/09/03  09:51:38  jsb]
 * 
 * 	From Intel SSD: Added i860 cases.
 * 	From me: improved conditional structure.
 * 	[91/09/02  15:05:31  jsb]
 * 
 * Revision 2.3  90/05/21  13:45:57  dbg
 * 	Return 0 for success.
 * 	[90/03/14            dbg]
 * 
 * Revision 2.2  89/11/29  15:26:49  af
 * 	Added mips, RCS-ed.
 * 	[89/11/16            af]
 * 
 */
#include <machine/vmparam.h>
#include <sys/exec.h>

/*
 * Compiled with -D$(TARGET_MACHINE) so that we do the right things
 * for the target machine, not ourselves (if different).
 */
main()
{
#define	DEFAULT	1

#if	MAC2
#undef	DEFAULT
	printf("%x\n", EMULATOR_BASE + sizeof (struct exec));
#endif

#if	SUN3
#undef	DEFAULT
	printf("%x\n", EMULATOR_BASE + sizeof (struct exec));
#endif

#if	PMAX
#undef	DEFAULT
	printf("%x -D %x\n", EMULATOR_BASE, EMULATOR_BASE + (1024*1024));
#endif

#if	I860
#undef	DEFAULT
	printf("0x%x -d 0x%x\n", EMULATOR_BASE, EMULATOR_BASE + (128*1024));
#endif

#ifdef	DEFAULT
	printf("%x\n", EMULATOR_BASE);
#endif

	return (0);
}
