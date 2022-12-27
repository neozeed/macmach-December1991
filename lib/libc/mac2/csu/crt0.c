/*
 * Copyright (c) 1980 Regents of the University of California.
 * All rights reserved.  The Berkeley software License Agreement
 * specifies the terms and conditions for redistribution.
 */

#if defined(VERSION) && !defined(lint)
/* VERSION is a identifying string to exist in all executables */
static char version[] = VERSION;
#endif

#if defined(LIBC_SCCS) && !defined(lint)
static char sccsid[] = "@(#)crt0.c	5.4 (Berkeley) 1/18/88";
#endif LIBC_SCCS and not lint

/*
 *	C start up routine.
 */

/* Define EXIT_ZERO if main() should ignore return value and exit zero */
#define EXIT_ZERO

/*
  start() is jmp'ed to and the stack looks like:
	sp +0  [kargc]
	   +4  [kargv0]
  Since kargc is in the place that the compiler expects a return address,
  it can not be accessed as an argument to start().  The compiler sets up
  a stack frame for start().  The (argc, argv, envp) arguments to main()
  are allocated as local variables to start().  The stack then looks like:
	   -12  [argc]
	   -8   [argv]
	   -4   [envp]
	a6 +0   [old a6]
	   +4   [kargc]
	   +8   [kargv0]
*/
#define MAIN main(argc, argv, envp)

/* MACH function pointers are initialized non-zero by the MACH libraries */
void	(*mach_init_routine)();
void	(*_cthread_init_routine)();
int	(*_cthread_exit_routine)();

char	**environ = (char **)0;

#ifdef MCRT0
extern	unsigned char	etext;
extern	unsigned char	eprol; /* Not really extern, see asm() below. */
#endif

static
start(kargv0)
char		*kargv0;
{
    char	**envp;
    char	**argv;
    int		argc;

    argc = *(int *)(&kargv0 - 1);
    argv = &kargv0;
    environ = envp = &argv[argc + 1];

    if (mach_init_routine)
	(*mach_init_routine)();

#ifdef MCRT0
asm("_eprol:");
	monstartup(&eprol, &etext);
#endif MCRT0

    if (_cthread_init_routine) {
	(*_cthread_init_routine)();
	asm("tstl	d0; beq 0f; movl d0,sp; 0:");
    }

#ifdef EXIT_ZERO
    (void) MAIN;
    if (_cthread_exit_routine)
	_exit((*_cthread_exit_routine)(0));

    exit(0);
#else
    if (_cthread_exit_routine)
	_exit((*_cthread_exit_routine)(MAIN));

    exit(MAIN);
#endif

}

#ifdef MCRT0
/*ARGSUSED*/
exit(code)
	register int code;	/* r11 */
{
	monitor(0);
	_cleanup();
	_exit(code);
}
#endif MCRT0

#ifdef CRT0
/*
 * null mcount and moncontrol,
 * just in case some routine is compiled for profiling
 */
moncontrol(val)
int	val;
{

}
asm("	.text");
asm("	.globl	mcount");
asm("mcount:	rts");
#endif CRT0

/* change _start to start */
asm(".set start, _start; .globl start");
