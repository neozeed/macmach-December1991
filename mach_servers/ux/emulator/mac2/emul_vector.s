#include <sys/syscall.h>

	jmp	_emul_fault

/*
 * Emulator vector entry - allocate new stack
 */
	.data
	.globl	_emul_stack_lock
_emul_stack_lock:
	.long	0
	.text

/*
 * Generic call
 */
	.globl	_emul_common
_emul_common:
	moveml	a0-a1/d0-d1,sp@-
0:	tas	_emul_stack_lock
	bne	0b
	movl	_emul_stack_list,d0
	bne	1f
	jsr	_emul_stack_alloc
	movl	d0,a0
	bra	2f
1:	movl	d0,a0
	movl	a0@,_emul_stack_list
2:	clrb	_emul_stack_lock
	movl	sp,a0@
	moveml	a2-a6/d2-d7,a0@-
	movl	a0,sp
	pea	sp@
	jsr	_emul_syscall
	addqw	#4,sp
/*
 * Return
 */
	moveml	sp@+,a2-a6/d2-d7
	movl	sp,a0
	movl	sp@,sp
0:	tas	_emul_stack_lock
	bne	0b
	movl	_emul_stack_list,a0@
	movl	a0,_emul_stack_list
	clrb	_emul_stack_lock
	moveml	sp@+,a0-a1/d0-d1
	addqw	#4,sp
	rtr

	.globl	_child_fork
_child_fork:
	moveml	d0-d1,sp@
	jsr	_child_init
	moveml	sp@+,a0-a1/d0-d1
	addqw	#4,sp
	rtr

	.globl	_emul_execve
_emul_execve:
	pea	SYS_execve
	trap	#0
	/* we only get here on failure */
	moveq	#-1,d0
	rts
