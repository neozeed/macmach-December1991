| 
| Mach Operating System
| Copyright (c) 1989 Carnegie-Mellon University
| All rights reserved.  The CMU software License Agreement specifies
| the terms and conditions for use and redistribution.
|

| HISTORY
| $Log:	lock.s,v $
| Revision 2.3  90/10/29  17:27:07  dpj
| 	Renamed all spin_lock operations to mach3_spin_lock, to avoid
| 	conflicts with the cthreads version.
| 	[90/10/27  17:48:26  dpj]
| 
| 	Renamed all spin_lock operations to mach3_spin_lock, to avoid
| 	conflicts with the cthreads version.
| 	[90/10/21  21:19:29  dpj]
| 
| Revision 2.2  90/07/26  12:36:44  dpj
| 	First version.
| 	[90/07/24  14:58:05  dpj]
| 
| Revision 1.1  90/02/22  18:38:21  bww
| 	Mach Release 2.5
| 	[90/02/22  18:38:07  bww]
| 
| Revision 1.3  89/05/05  19:00:44  mrt
| 	Cleanup for Mach 2.5
| 

| sun/lock.s
|
| Mutex implementation for Sun.

|	int
|	mach3_spin_try_lock(m)
|		int	*m;

	.globl	_mach3_spin_try_lock
_mach3_spin_try_lock:
	movl	sp@(4),a0
	tas	a0@
	bne	1f
	moveq	#1,d0		| yes
	rts
1:
	clrl	d0		| no
	rts

|	void
|	mach3_spin_unlock(m)
|		int	*m;

	.globl	_mach3_spin_unlock
_mach3_spin_unlock:
	movl	sp@(4),a0
	clrl	a0@
	rts
