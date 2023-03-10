#if	CMU
/* 
 * Mach Operating System
 * Copyright (c) 1988 Carnegie-Mellon University
 * All rights reserved.  The CMU software License Agreement specifies
 * the terms and conditions for use and redistribution.
 */
/*
 * HISTORY
 * $Log:	jioctl.h,v $
 * Revision 2.1  89/08/04  14:46:01  rwd
 * Created.
 * 
 * Revision 2.2  88/08/24  02:29:53  mwyoung
 * 	Adjusted include file references.
 * 	[88/08/17  02:13:58  mwyoung]
 * 
 */
#endif	CMU
/*
**	Unix to Blit I/O control codes
*/

#ifndef _JIOCTL_
#define	_JIOCTL_
#include <sys/ioctl.h>

/*
#define	JBOOT		_IO(j, 1)
#define	JTERM		_IO(j, 2)
#define	JMPX		_IO(j, 3)
#define	JTIMO		_IO(j, 4)
#define	JTIMOM		_IO(j, 6)
#define	JZOMBOOT	_IO(j, 7)
#define JSMPX		_IOW(j, 9, int)
*/
#define JSMPX		TIOCUCNTL
#define JMPX		_IO(u,0)
#define	JBOOT		_IO(u, 1)
#define	JTERM		_IO(u, 2)
#define	JTIMO		_IO(u, 4)	/* Timeouts in seconds */
#define	JTIMOM		_IO(u, 6)	/* Timeouts in millisecs */
#define	JZOMBOOT	_IO(u, 7)
#define JWINSIZE	TIOCGWINSZ
#define JSWINSIZE	TIOCSWINSZ

/**	Channel 0 control message format **/

struct jerqmesg
{
	char	cmd;		/* A control code above */
	char	chan;		/* Channel it refers to */
};

/*
**	Character-driven state machine information for Blit to Unix communication.
*/

#define	C_SENDCHAR	1	/* Send character to layer process */
#define	C_NEW		2	/* Create new layer process group */
#define	C_UNBLK		3	/* Unblock layer process */
#define	C_DELETE	4	/* Delete layer process group */
#define	C_EXIT		5	/* Exit */
#define	C_BRAINDEATH	6	/* Send terminate signal to proc. group */
#define	C_SENDNCHARS	7	/* Send several characters to layer proc. */
#define	C_RESHAPE	8	/* Layer has been reshaped */
#define C_JAGENT	9	/* Jagent return (What do they mean? */

/*
 * Map to new window structure
 */
#define bitsx	ws_xpixel
#define bitsy	ws_ypixel
#define	bytesx	ws_col
#define	bytesy	ws_row
#define jwinsize winsize

/*
**	Usual format is: [command][data]
*/
#endif _JIOCTL_
