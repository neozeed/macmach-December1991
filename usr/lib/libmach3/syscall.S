| 
| Mach Operating System
| Copyright (c) 1990 Carnegie-Mellon University
| All rights reserved.  The CMU software License Agreement specifies
| the terms and conditions for use and redistribution.
| 

| 
| HISTORY
| $Log:	syscall.s,v $
| Revision 2.2  90/07/26  12:36:47  dpj
| 	First version.
| 	[90/07/24  14:59:22  dpj]
| 

| 
|  Invoke an indirect UNIX trap, to access any potential UNIX emulator.
| 

	.globl _mach3_syscall;
_mach3_syscall:
	movl	sp@(4),d0
	movl	sp@,sp@(4)
	movl	d0,sp@
	trap	#0
	bcc	noerror
	movl	sp@,a0
	movl	a0,sp@-
	moveq	#-1,d0
	rts
noerror:
	movl	sp@,a0
	jra	a0@
