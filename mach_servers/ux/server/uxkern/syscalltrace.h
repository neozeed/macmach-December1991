/*
 * System call tracing
 */
#define	SYSCALLTRACE	1

#ifdef	SYSCALLTRACE
extern int	syscalltrace;

#define	TRACE(arglist)	{ \
	if (syscalltrace == -1 || syscalltrace == u.u_procp->p_pid) { \
	    printf arglist ; \
	} \
}
#endif

