#include <mach/mach.h>
#include <sys/types.h>
#include <sys/param.h>
#include <sys/errno.h>
#include <sys/signal.h>
#include <sys/syscall.h>
#include <uxkern/bsd_msg.h>	/* error code definitions */

#include <mac2/psl.h>
#include <mac2/vmparam.h>

#include <syscall_table.h>

#include <emul_stack.h>

#include <setjmp.h>

#ifdef	MAP_UAREA
#include <sys/ushared.h>

extern int shared_enabled;
extern struct ushared_ro *shared_base_ro;
extern struct ushared_rw *shared_base_rw;
#endif	MAP_UAREA

extern mach_port_t	our_bsd_server_port;

#define	E_JUSTRETURN	255	/* return without changing registers */

/*
 * Stored on user stack
 */
typedef struct {
    unsigned		d0;
    unsigned		d1;
    unsigned		a0;
    unsigned		a1;
    int			syscode;
    unsigned short	ccr;
    unsigned		pc;
} emul_frame_t;

/*
 * Stored on emulator stack
 */
typedef struct {
    unsigned		d2;
    unsigned		d3;
    unsigned		d4;
    unsigned		d5;
    unsigned		d6;
    unsigned		d7;
    unsigned		a2;
    unsigned		a3;
    unsigned		a4;
    unsigned		a5;
    unsigned		fp;
    emul_frame_t	*sp;
} emul_regs_t;

void	emul_restart(), take_signals();

int	emul_low_entry = -9;
int	emul_high_entry = 181;

extern emul_common();

void
emul_setup(task)
task_t	task;
{
    register int i;
    register kern_return_t	rc;
    
    for (i = emul_low_entry;
	 i <= emul_high_entry;
	 i++) {
	rc = task_set_emulation(task,
				emul_common,
				i);
    }
    rc = task_set_emulation(task,
			    emul_common,
			    -33);
    rc = task_set_emulation(task,
			    emul_common,
			    -34);
    rc = task_set_emulation(task,
			    emul_common,
			    -41);
    rc = task_set_emulation(task,
			    emul_common,
			    -52);
}

inline
int *
emul_stack_fault()
{
    int				x;
    register vm_offset_t	stack_addr = (vm_offset_t)&x;
    register emul_stack_t	stack;
    extern vm_offset_t		emul_stack_mask, emul_stack_size;

    stack_addr &= emul_stack_mask;
    stack_addr += emul_stack_size;  /* get top of stack */
    stack = ((emul_stack_t)stack_addr) - 1;
                                        /* point to structure at stack top */

    return (stack->fault);
}    

/*
 * System calls enter here.
 */
void
emul_syscall(regs)
register emul_regs_t	*regs;
{
    register int		syscode;
    register int		error;
    register struct sysent	*callp;
    int				rval[2];
    boolean_t			interrupt = FALSE;
    register emul_frame_t	*frame;
    register unsigned		*args;

    frame = regs->sp;
    args = &frame->pc + 2;

    syscode = frame->syscode;

#ifdef	MAP_UAREA
    if (shared_enabled) {
	if (shared_base_ro->us_cursig) {
	    error = ERESTART;
	    goto signal;
	}
    }
#endif	MAP_UAREA
    
    if (syscode == 0) {
	/*
	 * Indirect system call.
	 */
	syscode = *args++;
    }
    
    /*
     * Find system call table entry for the system call.
     */
    if (syscode >= nsysent)
	callp = &sysent[63];	/* nosysent */
    else if (syscode >= 0)
	callp = &sysent[syscode];
    else {
	/*
	 * Negative system call numbers are CMU extensions.
	 */
	if (syscode == -33)
	    callp = &sysent_task_by_pid;
	else if (syscode == -34)
	    callp = &sysent_pid_by_task;
	else if (syscode == -41)
	    callp = &sysent_init_process;
	else if (syscode == -59)
	    callp = &sysent_htg_ux_syscall;
	else if (syscode < -ncmusysent)
	    callp = &sysent[63];	/* nosysent */
	else
	    callp = &cmusysent[-syscode];
    }
    
    /*
     * Set up the initial return values.
     */
    rval[0] = 0;
    rval[1] = frame->d1;

    if (_setjmp(emul_stack_fault())) {
	frame->ccr |= SR_CC;
	frame->d0 = EFAULT;
	return;
    }
    
    /*
     * Call the routine, passing arguments according to the table
     * entry.
     */
    switch (callp->nargs) {
      case 0:
	error = (*callp->routine)(our_bsd_server_port,
				  &interrupt,
				  rval);
	break;
      case 1:
	error = (*callp->routine)(our_bsd_server_port,
				  &interrupt,
				  args[0],
				  rval);
	break;
      case 2:
	error = (*callp->routine)(our_bsd_server_port,
				  &interrupt,
				  args[0], args[1],
				  rval);
	break;
      case 3:
	error = (*callp->routine)(our_bsd_server_port,
				  &interrupt,
				  args[0], args[1], args[2],
				  rval);
	break;
      case 4:
	error = (*callp->routine)(our_bsd_server_port,
				  &interrupt,
				  args[0], args[1], args[2], args[3],
				  rval);
	break;
      case 5:
	error = (*callp->routine)(our_bsd_server_port,
				  &interrupt,
				  args[0], args[1], args[2], args[3], args[4],
				  rval);
	break;
      case 6:
	error = (*callp->routine)(our_bsd_server_port,
				  &interrupt,
				  args[0], args[1], args[2],
				  args[3], args[4], args[5],
				  rval);
	break;
	
      case -1:	/* generic */
	error = (*callp->routine)(our_bsd_server_port,
				  &interrupt,
				  syscode,
				  args,
				  rval);
	break;
	
      case -2:	/* pass registers to modify */
	error = (*callp->routine)(our_bsd_server_port,
				  &interrupt,
				  args,
				  rval,
				  regs);
	frame = regs->sp;	/* if changed */
	break;
    }

    /*
     * Set up return values.
     */

#ifdef	MAP_UAREA
signal:
#endif	MAP_UAREA

    switch (error) {
      case E_JUSTRETURN:
	/* Do not alter registers */
	break;

      case 0:
	/* Success */
	frame->ccr &= ~SR_C;
	frame->d0 = rval[0];
	frame->d1 = rval[1];
	break;

      case ERESTART:
	/* restart call */
	emul_restart(regs);
	break;

      default:
	/* error */
	frame->ccr |= SR_C;
	frame->d0 = error;
	break;
    }

    /*
     * Handle interrupt request
     */
    if (error == ERESTART || error == EINTR || interrupt)
	take_signals(regs);
}

void
emul_restart(regs)
register emul_regs_t	*regs;
{
    emul_frame_t	saved_frame;
    register unsigned	sp;

    saved_frame = *regs->sp;

    sp = (unsigned)(regs->sp + 1);

    sp -= sizeof (int);
    *(int *)sp = saved_frame.syscode;

    sp -= sizeof (saved_frame);
    (unsigned)regs->sp = sp;

    saved_frame.pc -= 2;
    *regs->sp = saved_frame;
}

void
emul_fault()
{
    /* assume that there is at most one fault per system call */
    /* the unwind after this longjmp must be clean */
    _longjmp(emul_stack_fault(), 1);
}

/*
 * Take a signal.
 */
void
take_signals(regs)
register emul_regs_t	*regs;
{
    register emul_frame_t	*frame;
    emul_frame_t		saved_frame;
    register struct sigcontext	*sc;
    register struct sigframe {
	int		sig;
	int		code;
	unsigned	scp;
    } *sf;
    int				old_mask, old_onstack;
    int				sig, code;
    unsigned			handler, new_sp;
    boolean_t	interrupt;

    /*
     * Get anything valuable off user stack first.
     */
    saved_frame = *regs->sp;

    /*
     * Get the signal to take from the server.  It also
     * switches the signal mask and the stack, so we must
     * be off the old user stack before calling it.
     */
    (void) bsd_take_signal(our_bsd_server_port,
			   &interrupt,
			   &old_mask,
			   &old_onstack,
			   &sig,
			   &code,
			   &handler,
			   &new_sp);

    /*
     * If there really were no signals to take, return.
     */
    if (sig == 0)
	return;

    /*
     * Put the signal context and signal frame on the signal stack.
     */
    if (new_sp == 0) {
	/*
	 * Build signal frame and context on user's stack.
	 */
	new_sp = (unsigned)regs->sp;
    }

    sc = ((struct sigcontext *)new_sp) - 1;
    sf  = ((struct sigframe *)sc) - 1;

    /*
     * Build the argument list for the signal handler.
     */
    sf->sig = sig;
    sf->code = code;
    sf->scp = (unsigned)sc;

    /*
     * Build the signal context to be used by sigreturn.
     */
    sc->sc_onstack = old_onstack;
    sc->sc_mask = old_mask;
    sc->sc_sp = (int)(regs->sp+1);	/* user sp after return */
    sc->sc_pc = saved_frame.pc;
    sc->sc_ps = saved_frame.ccr;
    
    /*
     * Set up the new stack and handler addresses.
     */
    frame = ((emul_frame_t *)sf) - 1;
    *frame = saved_frame;
    regs->sp = frame;
    frame->pc = handler;
}

int
e_sigreturn(serv_port, interrupt, argp, rval, regs)
mach_port_t		serv_port;
boolean_t		*interrupt;
struct {
    struct sigcontext	*scp;
}			*argp;
int			*rval;
emul_regs_t		*regs;
{
    register emul_frame_t	*frame;
    emul_frame_t		saved_frame;
    register struct sigcontext	sc;
    register int		rc;

    saved_frame = *regs->sp;
    sc = *argp->scp;

    /*
     * Change signal stack and mask.  If new signals are pending,
     * do not take them until we switch user stack.
     */
#ifdef	MAP_UAREA
    if (shared_enabled) {
	rc = e_shared_sigreturn(serv_port, interrupt, 
				sc.sc_onstack & 01, sc.sc_mask);
    } else {
#endif	MAP_UAREA
	rc = bsd_sigreturn(serv_port, interrupt,
			   sc.sc_onstack & 01, sc.sc_mask);
#ifdef	MAP_UAREA
    }
#endif	MAP_UAREA

    /*
     * Move registers to old stack.
     */
    frame = ((emul_frame_t *)sc.sc_sp) - 1;

    *frame = saved_frame;
    frame->pc = sc.sc_pc;
    frame->ccr = sc.sc_ps;
    
    regs->sp = frame;

    return (E_JUSTRETURN);
}

int
e_osigcleanup(serv_port, interrupt, argp, rval, regs)
mach_port_t		serv_port;
boolean_t		*interrupt;
int			*argp;
int			*rval;
emul_regs_t		*regs;
{
    register emul_frame_t	*frame;
    emul_frame_t		saved_frame;
    register struct sigcontext	sc;
    register int		rc;

    saved_frame = *regs->sp;
    sc = **((struct sigcontext **)(regs->sp + 1));

    /*
     * Change signal stack and mask.  If new signals are pending,
     * do not take them until we switch user stack.
     */
#ifdef	MAP_UAREA
    if (shared_enabled) {
	rc = e_shared_sigreturn(serv_port, interrupt, 
				sc.sc_onstack & 01, sc.sc_mask);
    } else {
#endif	MAP_UAREA
	rc = bsd_sigreturn(serv_port, interrupt,
			   sc.sc_onstack & 01, sc.sc_mask);
#ifdef	MAP_UAREA
    }
#endif	MAP_UAREA

    /*
     * Move registers to old stack.
     */
    frame = ((emul_frame_t *)sc.sc_sp) - 1;

    *frame = saved_frame;
    frame->pc = sc.sc_pc;
    frame->ccr = sc.sc_ps;
    
    regs->sp = frame;

    return (E_JUSTRETURN);
}

/*
 * Wait has a weird parameter passing mechanism.
 */
int
e_wait(serv_port, interrupt, argp, rval, regs)
mach_port_t		serv_port;
boolean_t		*interrupt;
int			*argp;
int			*rval;
emul_regs_t		*regs;
{
    register emul_frame_t	*frame = regs->sp;
    int				new_args[2];

    if ((frame->ccr & PSL_ALLCC) == PSL_ALLCC) {
	new_args[0] = frame->d0;	/* options */
	new_args[1] = frame->d1;	/* rusage_p */
    }
    else {
	new_args[0] = 0;
	new_args[1] = 0;
    }

    return (emul_generic(serv_port, interrupt,
			 SYS_wait, &new_args[0], rval));
}

int
e_fork(serv_port, interrupt, argp, rval, regs)
mach_port_t		serv_port;
boolean_t		*interrupt;
int			*argp;
int			*rval;
register emul_regs_t	*regs;
{
    register emul_frame_t	*frame = regs->sp;
    register int error;
    struct {
	regs_t				regs;
	normal_exception_frame_t	frame;
    } child;
    register unsigned			count;
    extern int				child_fork();

    /*
     * Set up registers for child.  It resumes on its own stack.
     */
    child.regs.r_dreg[0] = frame->d0;
    child.regs.r_dreg[1] = frame->d1;
    child.regs.r_areg[0] = frame->a0;
    child.regs.r_areg[1] = frame->a1;
    child.regs.r_dreg[2] = regs->d2;
    child.regs.r_dreg[3] = regs->d3;
    child.regs.r_dreg[4] = regs->d4;
    child.regs.r_dreg[5] = regs->d5;
    child.regs.r_dreg[6] = regs->d6;
    child.regs.r_dreg[7] = regs->d7;
    child.regs.r_areg[2] = regs->a2;
    child.regs.r_areg[3] = regs->a3;
    child.regs.r_areg[4] = regs->a4;
    child.regs.r_areg[5] = regs->a5;
    child.regs.r_areg[6] = regs->fp;
    child.regs.r_areg[7] = (unsigned)regs->sp;
    child.frame.f_pc	 = (int)child_fork;	/* not a procedure */
    child.frame.f_sr	 = (frame->ccr & SR_CC);
    child.frame.f_fmt	 = STKFMT_NORMAL;

    frame->ccr &= ~SR_C;

    count = sizeof (regs_t) + NORMAL_EXCEPTION_FRAME_SIZE;
    count = (count + sizeof (int) - 1) / sizeof (int);
	
    /*
     * Create the child.
     */
    error = bsd_fork(serv_port, interrupt,
		     (int *)&child,
		     count,
		     &rval[0]);

    if (error == 0)
	rval[1] = 0;

    return (error);
}

/*
 * Exec starts here to save registers.
 */
struct execa {
    char	*fname;
    char	**argp;
    char	**envp;
};

int
e_execv(serv_port, interrupt, argp, rval, regs)
mach_port_t		serv_port;
boolean_t		*interrupt;
register struct execa	*argp;
int			*rval;
emul_regs_t		*regs;
{
    struct execa		execa;

    execa.fname = argp->fname;
    execa.argp  = argp->argp;
    execa.envp  = (char **)0;

    return (e_execve(serv_port, interrupt, &execa, rval, regs));
}

int
e_execve(serv_port, interrupt, argp, rval, regs)
mach_port_t		serv_port;
boolean_t		*interrupt;
register struct execa	*argp;
int			*rval;
register emul_regs_t	*regs;
{
    emul_frame_t		saved_frame;
    register emul_frame_t	*frame;
    int				entry;
    unsigned int		entry_count;
    vm_offset_t			arg_addr;
    register int		error;

    /*
     * Save anything from old stack.
     */
    saved_frame = *regs->sp;

    /*
     * Call exec.  If error, return without changing registers.
     */
    entry_count = 1;
    error = e_exec_call(serv_port,
			interrupt,
			argp->fname,
			argp->argp,
			argp->envp,
			&arg_addr,
			&entry,
			&entry_count);

    if (error)
	return (error);

    /*
     * Put new user stack just below arguments.
     */
    frame = (emul_frame_t *)arg_addr;

    /*
     * Push old registers on user stack.
     */
    *--frame = saved_frame;

    /*
     * Set new pc, and clear frame pointer for traceback.
     */
    regs->sp = frame;
    frame->pc = entry;
    regs->fp = 0;

    /*
     * Return to new stack.
     */
    return (E_JUSTRETURN);
}

vm_offset_t
set_arg_addr(arg_size)
vm_size_t	arg_size;
{
    /*
     * Round argument size to fullwords
     */
    arg_size = (arg_size + NBPW - 1) & ~(NBPW - 1);

    /*
     * Put argument list at top of stack.
     */
    return (USRSTACK - arg_size);
}
