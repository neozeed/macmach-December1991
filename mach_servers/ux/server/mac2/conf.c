/* 
 * Mach Operating System
 * Copyright (c) 1990 Carnegie-Mellon University
 * Copyright (c) 1989 Carnegie-Mellon University
 * Copyright (c) 1988 Carnegie-Mellon University
 * All rights reserved.  The CMU software License Agreement specifies
 * the terms and conditions for use and redistribution.
 */
/*
 * HISTORY
 * $Log:	conf.c,v $
 * Revision 2.2  90/09/04  17:33:57  bohman
 * 	Created.
 * 
 */

/*
 *	Apple Macintosh II Mach (macmach)
 *
 *	File: mac2/conf.c
 */

/*
 * Copyright (c) 1982, 1986 Regents of the University of California.
 * All rights reserved.  The Berkeley software License Agreement
 * specifies the terms and conditions for redistribution.
 *
 *	@(#)conf.c	7.1 (Berkeley) 6/5/86
 */

#include <vice.h>
#include <pty.h>

#include <sys/param.h>
#include <sys/systm.h>
#include <sys/buf.h>
#include <sys/conf.h>
#include <sys/errno.h>

#include <sys/ioctl.h>
#include <sys/tty.h>

extern int	nulldev();
extern int	nodev();
extern int	seltrue();
extern struct tty *nulltty();

/*
 * Block devices all use the same open/close/strategy routines.
 */
extern int	bdev_open(), bdev_close(), bio_strategy();
#define	bdev_ops	bdev_open, bdev_close, bio_strategy

extern int	sdisk_bopen(), sdisk_bclose(), sdisk_strategy();
#define sdisk_b_ops	sdisk_bopen, sdisk_bclose, sdisk_strategy

struct bdevsw	bdevsw[] =
{
    { "hd",	C_BLOCK(32),	sdisk_b_ops },		/*0*/
    { "",	0,		bdev_ops },		/*1*/
    { "",	0,		bdev_ops },		/*2*/
    { "",	0,		bdev_ops },		/*3*/
    { "",	0,		bdev_ops },		/*4*/
    { "",	0,		bdev_ops },		/*5*/
    { "",	0,		bdev_ops },		/*6*/
    { "",	0,		bdev_ops },		/*7*/
    { "rd",	0,		bdev_ops },		/*8*/
};
int	nblkdev = sizeof (bdevsw) / sizeof (bdevsw[0]);

extern int	char_open(), char_close(), char_read(), char_write();
extern int	char_ioctl(), char_select(), char_port();
#define	char_ops \
	char_open, char_close, char_read, char_write, char_ioctl, \
	char_select, nulldev, nulltty

extern int	sdisk_copen(), sdisk_cclose(), sdisk_read(), sdisk_write();
extern int	sdisk_ioctl();
#define	sdisk_c_ops \
	sdisk_copen, sdisk_cclose, sdisk_read, sdisk_write, sdisk_ioctl, \
	seltrue, nulldev, nulltty

extern int	apple_open(), apple_close(), apple_read(), apple_write();
#define apple_ops \
	apple_open, apple_close, apple_read, apple_write, nodev, \
	seltrue, nulldev, nulltty
	
extern int	tty_open(), tty_close(), tty_read(), tty_write();
extern int	tty_ioctl(), ttselect(), tty_stop();
extern struct tty *tty_find_tty();
#define	tty_ops	\
	tty_open, tty_close, tty_read, tty_write, tty_ioctl, ttselect, \
	tty_stop, tty_find_tty

extern int	cons_open(), cons_write(), cons_ioctl();
#define	console_ops	\
	cons_open, tty_close, tty_read, cons_write, cons_ioctl, ttselect, \
	tty_stop, tty_find_tty

extern int	syopen(), syread(), sywrite(), syioctl(), syselect();
#define	sy_ops \
	syopen,   nulldev,   syread,   sywrite,   syioctl,   syselect, \
	nulldev, nulltty

extern int	logopen(), logclose(), logread(), logioctl(), logselect();
#define log_ops \
	logopen,  logclose,  logread,  nulldev,  logioctl,  logselect, \
	nulldev, nulltty

extern int	mmopen(), mmread(), mmwrite();
#define	mm_ops \
	mmopen,   nulldev,   mmread,   mmwrite,   nodev,     seltrue,  \
	nulldev, nulltty

#if	NPTY > 0
extern int	ptsopen(), ptsclose(), ptsread(), ptswrite();
extern int	ptyioctl(), ptsstop();
extern struct tty *pty_find_tty();
#define	pts_ops \
	ptsopen,  ptsclose,  ptsread,  ptswrite,  ptyioctl,  ttselect, \
	ptsstop, pty_find_tty

extern int	ptcopen(), ptcclose(), ptcread(), ptcwrite();
extern int	ptcselect();
#define	ptc_ops \
	ptcopen,  ptcclose,  ptcread,  ptcwrite,  ptyioctl,  ptcselect, \
	nulldev, nulltty
#endif	NPTY > 0

#if	VICE
extern int	rmtopen(), rmtclose(), rmtread(), rmtwrite(), rmtselect();
#define	rmt_ops \
	rmtopen, rmtclose, rmtread, rmtwrite, nodev, rmtselect, \
	nulldev, nulltty
#endif	VICE

int	video_ioctl();
#define video_ops \
    char_open, char_close, nodev, nodev, video_ioctl, \
    nodev, nodev, nulltty, char_port

int	disk_open(), disk_close(), disk_read(), disk_write(), disk_ioctl();
#define disk_ops \
    disk_open, disk_close, disk_read, disk_write, disk_ioctl, \
    seltrue, nulldev, nulltty

int	sony_ioctl();
#define	sony_ops \
    disk_open, disk_close, disk_read, disk_write, sony_ioctl, \
    seltrue, nulldev, nulltty

#define adb_ops	\
    char_open, char_close, char_read, nodev, char_ioctl, \
    char_select, nulldev, nulltty

#define serial_ops tty_ops

#define	no_ops \
	nodev, nodev, nodev, nodev, nodev, nodev, nulldev, nulltty

struct cdevsw	cdevsw[] =
{
    { "console",	0,		console_ops	},	/*0*/
    { "",		0,		no_ops		},	/*1*/
    { "",		0,		sy_ops		},	/*2*/
    { "",		0,		mm_ops		},	/*3*/
    { "",		0,		no_ops		},	/*4*/
    { "video",		0,		video_ops	},	/*5*/
    { "hd",		C_BLOCK(32),	sdisk_c_ops	},	/*6*/
    { "ahd",		C_BLOCK(32),	apple_ops	},	/*7*/
    { "rd",		0,		disk_ops	},	/*8*/
    { "sony",		0,		sony_ops	},	/*9*/
    { "adb",		0,		adb_ops		},	/*10*/
    { "serial",		0,		serial_ops	},	/*11*/
    { "",		0,		no_ops		},	/*12*/
    { "",		0,		no_ops		},	/*13*/
    { "",		0,		no_ops		},	/*14*/
    { "",		0,		no_ops		},	/*15*/
    { "",		0,		no_ops		},	/*16*/
    { "",		0,		no_ops		},	/*17*/
    { "",		0,		no_ops		},	/*18*/
    { "",		0,		no_ops		},	/*19*/
#if	NPTY > 0
    { "",		0,		pts_ops		},	/*20*/
    { "",		0,		ptc_ops		},	/*21*/
#else	NPTY > 0
    { "",		0,		no_ops		},	/*20*/
    { "",		0,		no_ops		},	/*21*/
#endif	NPTY > 0
#if	VICE
    { "",		0,		rmt_ops		},	/*22*/
#else	VICE
    { "",		0,		no_ops		},	/*22*/
#endif	VICE
    { "",		0,		log_ops,	},	/*23*/
};
int	nchrdev = sizeof (cdevsw) / sizeof (cdevsw[0]);

dev_t	sydev = makedev(2, 0);	/* device number for indirect tty */

/*
 * Conjure up a name string for funny devices (not all minors have
 * the same name).
 */
int
check_dev(dev, str)
dev_t	dev;
char	str[];	/* REF OUT */
{
    int	major_num = major(dev);
    int	minor_num = minor(dev);
    return (ENXIO);
}
