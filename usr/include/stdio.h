/*
 * Copyright (c) 1980 Regents of the University of California.
 * All rights reserved.  The Berkeley software License Agreement
 * specifies the terms and conditions for redistribution.
 *
 *	@(#)stdio.h	5.4 (Berkeley) 10/22/87
 */
/* 20-Mar-89  Zon Williamson (zon) at Carnegie Mellon University
 *	Changed sprintf to return buffer address for mac2.
 */

# ifndef FILE
#define	BUFSIZ	1024
extern	struct	_iobuf {
	int	_cnt;
	char	*_ptr;		/* should be unsigned char */
	char	*_base;		/* ditto */
	int	_bufsiz;
	short	_flag;
	char	_file;		/* should be short */
} _iob[];

#define	_IOREAD	01
#define	_IOWRT	02
#define	_IONBF	04
#define	_IOMYBUF	010
#define	_IOEOF	020
#define	_IOERR	040
#define	_IOSTRG	0100
#define	_IOLBF	0200
#define	_IORW	0400
#define	NULL	0
#define	FILE	struct _iobuf
#define	EOF	(-1)

#define	stdin	(&_iob[0])
#define	stdout	(&_iob[1])
#define	stderr	(&_iob[2])
#ifndef lint
#define	getc(p)		(--(p)->_cnt>=0? (int)(*(unsigned char *)(p)->_ptr++):_filbuf(p))
#endif /* not lint */
#define	getchar()	getc(stdin)
#ifndef lint
#define putc(x, p)	(--(p)->_cnt >= 0 ?\
	(int)(*(unsigned char *)(p)->_ptr++ = (x)) :\
	(((p)->_flag & _IOLBF) && -(p)->_cnt < (p)->_bufsiz ?\
		((*(p)->_ptr = (x)) != '\n' ?\
			(int)(*(unsigned char *)(p)->_ptr++) :\
			_flsbuf(*(unsigned char *)(p)->_ptr, p)) :\
		_flsbuf((unsigned char)(x), p)))
#endif /* not lint */
#define	putchar(x)	putc(x,stdout)
#define	feof(p)		(((p)->_flag&_IOEOF)!=0)
#define	ferror(p)	(((p)->_flag&_IOERR)!=0)
#define	fileno(p)	((p)->_file)
#define	clearerr(p)	((p)->_flag &= ~(_IOERR|_IOEOF))

FILE	*fopen();
FILE	*fdopen();
FILE	*freopen();
FILE	*popen();
long	ftell();
char	*fgets();
char	*gets();
#ifdef mac2
char	*sprintf();
#else
int	sprintf();	/* here until everyone does it right */
#endif
# endif
