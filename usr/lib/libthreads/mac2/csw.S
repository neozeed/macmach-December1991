| Context switch and cproc startup for mac2 COROUTINE implementation.

	.text

| Suspend the current thread and resume the next one.
|
|	void
|	cproc_switch(cur, next, lock)
|		int *cur;
|		int *next;

	.globl	_cproc_switch
_cproc_switch:
	movl	sp,a1		| keep sp
	moveml	#Lpush,sp@-	| save current registers
	movl	a1@(4),a0	| a0 = cur
	movl	sp,a0@		| save current sp
	movl	a1@(12),a2	| get lock pointer
	movl	a1@(8),a3	| a3 = next
	movl	a3@,sp		| restore next sp
	clrl	a2@		| clear lock
	moveml	sp@+,#Lpop	| restore next registers
	rts			| return to next thread

|	void
|	cproc_start_wait(parent_context, child, stackp, lock)
|		int *parent_context;
|		cproc_t child;
|		int stackp;

	.globl	_cproc_start_wait
_cproc_start_wait:
	movl	sp,a1		| keep sp
	moveml	#Lpush,sp@-	| save parent registers
	movl	a1@(4),a0	| a0 = parent
	movl	sp,a0@		| save parent sp
	movl	a1@(12),a3	| a3 = stackp
	movl	a1@(8),a3@-	| push child onto stackp
	movl	a1@(16),a2	| get lock pointer
	movl	a3,sp		| child sp = stackp
	clrl	a2@		| clear lock
	jbsr	_cproc_waiting

|	void
|	cproc_prepare(child, child_context, stack)
|		int *child_context;
|		int *stack;

	.globl	_cproc_prepare
_cproc_prepare:
	movl	sp@(12),a0	| get stack
	movl	sp@(4),a0@-	| child is arg to cthread_body
	clrl	a0@-		| pretend return address from cthread_body
	movl	#_cthread_body,a0@-
				| push return address
	moveml	#Lpush,a0@-	| push random regs
	movl	sp@(8),a1	| get context
	movl	a0,a1@		| save context
	rts

	| Register save masks for moveml instruction.
	| The pop mask is for post-increment mode.
	| The push mask is for pre-decrement mode.
	Lpop	= 0x7cfc	| A6 .. A2, D7 .. D2
	Lpush	= 0x3f3e	| D2 .. D7, A2 .. A6
