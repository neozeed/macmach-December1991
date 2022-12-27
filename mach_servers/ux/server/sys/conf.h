/* 
 * Mach Operating System
 * Copyright (c) 1990 Carnegie-Mellon University
 * Copyright (c) 1989 Carnegie-Mellon University
 * Copyright (c) 1988 Carnegie-Mellon University
 * Copyright (c) 1987 Carnegie-Mellon University
 * All rights reserved.  The CMU software License Agreement specifies
 * the terms and conditions for use and redistribution.
 */
/*
 * HISTORY
 * $Log:	conf.h,v $
 * Revision 2.5  90/06/02  15:25:05  rpd
 * 	Cleaned up conditionals; removed MACH, CMU, CMUCS, MACH_NO_KERNEL.
 * 	[90/04/28            rpd]
 * 	Converted to new IPC.
 * 	[90/03/26  20:03:21  rpd]
 * 
 * Revision 2.4  90/05/21  13:58:36  dbg
 * 	Get it right!
 * 	[90/05/08            dbg]
 * 
 * 	Redefine C_BLOCK(n) to indicate the number of partitions per
 * 	minor device.
 * 	[90/04/23            dbg]
 * 
 * Revision 2.3  89/11/29  15:29:31  af
 * 	Made external variables declared as such.
 * 	[89/11/16  17:05:22  af]
 * 
 * Revision 2.2  89/08/09  14:45:48  rwd
 * 	Added d_port to cdevsw.  Needed by atleast mmap.
 * 	[89/07/20            rwd]
 * 
 * 	Out-of-kernel version.  Rearrange.
 * 	[89/01/16            dbg]
 * 
 * 	Always include l_select in linesw.
 * 	[89/01/11            dbg]
 * 
 * Revision 2.3  88/08/24  02:25:01  mwyoung
 * 	Adjusted include file references.
 * 	[88/08/17  02:10:54  mwyoung]
 * 
 *
 * 24-Dec-86  Bill Bolosky (bolosky) at Carnegie-Mellon University
 *	romp: Added l_select field to linesw for X on the RT.
 *
 * 17-Jul-86  Bill Bolosky (bolosky) at Carnegie-Mellon University
 *	MACH: removed definition of swdevt.
 *
 */
/*
 * Copyright (c) 1982, 1986 Regents of the University of California.
 * All rights reserved.  The Berkeley software License Agreement
 * specifies the terms and conditions for redistribution.
 *
 *	@(#)conf.h	7.1 (Berkeley) 6/4/86
 */
 
#ifndef	_SYS_CONF_H_
#define	_SYS_CONF_H_

#define	C_MINOR		(0x100)		/* not all minors have same name */
#define	C_BLOCK(n)	(0x200 | ((n)<<16))
					/* fold 'n' minor device numbers
					   into partitions on same device */
#define	C_BLOCK_GET(f)	(((f)>>16) & 0xFF)

/*
 * Declaration of block device
 * switch. Each entry (row) is
 * the only link between the
 * main unix code and the driver.
 * The initialization of the
 * device switches is in the
 * file conf.c.
 */
struct bdevsw
{
	char	*d_name;
	int	d_flags;
	int	(*d_open)();
	int	(*d_close)();
	int	(*d_strategy)();
};
#ifdef	KERNEL
extern struct	bdevsw bdevsw[];
#endif	KERNEL

/*
 * Character device switch.
 */
struct cdevsw
{
	char	*d_name;
	int	d_flags;
	int	(*d_open)();
	int	(*d_close)();
	int	(*d_read)();
	int	(*d_write)();
	int	(*d_ioctl)();
	int	(*d_select)();
	int	(*d_stop)();
	struct tty *(*d_tty)();
	mach_port_t (*d_port)();
};
#ifdef	KERNEL
extern struct	cdevsw cdevsw[];
#endif	KERNEL

/*
 * tty line control switch.
 */
struct linesw
{
	int	(*l_open)();
	int	(*l_close)();
	int	(*l_read)();
	int	(*l_write)();
	int	(*l_ioctl)();
	int	(*l_rint)();
	int	(*l_rend)();
	int	(*l_meta)();
	int	(*l_start)();
	int	(*l_modem)();
	int	(*l_select)();
};
#ifdef	KERNEL
extern struct	linesw linesw[];
#endif	KERNEL

#endif	_SYS_CONF_H_
