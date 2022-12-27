/* 
 * Mach Operating System
 * Copyright (c) 1990 Carnegie-Mellon University
 * All rights reserved.  The CMU software License Agreement specifies
 * the terms and conditions for use and redistribution.
 */
/*
 * File: $Source: /afs/cs.cmu.edu/project/mach3-rcs/us/rcs/lib/mach3/mach3/mach3_flsbuf.c,v $
 *
 * Purpose: Output functions for libc stdio, suitable for a
 *	standalone environment.
 *
 * This file is a replacement for flsbuf.c, but it is
 * currently reusing some of the original UNIX code:
 *
 * Copyright (c) 1980 Regents of the University of California.
 * All rights reserved.  The Berkeley software License Agreement
 * specifies the terms and conditions for redistribution.
 *
 * HISTORY
 * $Log:	mach3_flsbuf.c,v $
 * Revision 2.5  90/12/21  13:52:57  jms
 * 	Merge forward.
 * 	[90/12/15  14:19:36  roy]
 * 	merge for roy/neves @osf
 * 	[90/12/20  13:17:10  jms]
 * 
 * Revision 2.4.1.1  90/12/14  18:38:55  roy
 * 	Remove printf's when changing output device.
 * 
 * 
 * Revision 2.4  90/10/29  17:27:49  dpj
 * 	No changes ? (part of the great war around setlinebuf ...)
 * 	[90/10/21  21:20:11  dpj]
 * 
 * 	Merged-up to U25
 * 	[90/09/02  20:00:52  dpj]
 * 
 * 	No changes?
 * 	[90/08/15  14:23:30  dpj]
 * 
 * 	Added a couple of setlinebuf() calls.
 * 	Removed the initial message in mach3_init_diag().
 * 	[90/08/13  10:19:31  dpj]
 * 
 * Revision 2.3  90/08/22  18:10:21  roy
 * 	Added mach3_init_diag().
 * 	[90/08/14  12:31:36  roy]
 * 
 * Revision 2.2  90/07/26  12:37:27  dpj
 * 	First version
 * 	[90/07/24  14:28:30  dpj]
 * 
 */

#include	<stdio.h>

#include	<sys/types.h>
#if	(! defined(MACH3)) || MACH3_UNIX || MACH3_VUS
#include	<sys/stat.h>
#endif	(! defined(MACH3)) || MACH3_UNIX || MACH3_VUS
#if	_FMAP
#include	<mach.h>
#define L_SET 0
#endif

#if	MACH3

#include	<mach.h>
#include	<mach/error.h>

/*
 * Sanity checks.
 */
#if	_FMAP
	! _FMAP cannot be defined with MACH3 !
#endif
#if	PARALLEL
	! PARALLEL cannot be defined with MACH3 !
#endif


/*
 * Global variable defining where output messages should go.
 */
#define	MACH3_OUT_UNIX		1
#define	MACH3_OUT_DIAG		2
#define	MACH3_OUT_CONSOLE	3
#if	MACH3_UNIX || MACH3_VUS
int			mach3_output_channel = MACH3_OUT_UNIX;
#endif	MACH3_UNIX || MACH3_VUS
#if	MACH3_US
int			mach3_output_channel = MACH3_OUT_DIAG;
#endif	MACH3_US
#if	MACH3_SA
int 			mach3_output_channel = MACH3_OUT_CONSOLE;
#endif	MACH3_SA
#if	MACH3_UNIX || MACH3_VUS || MACH3_US
#define	DIAG_NAME	"mach-diag"
#define	DIAG_LEVEL	-2
port_t			mach3_diag_port = PORT_NULL;
int			mach3_diag_level = DIAG_LEVEL;
#endif	MACH3_UNIX || MACH3_VUS || MACH3_US


/*
 * Functions to switch output channel.
 */
#if	MACH3_UNIX || MACH3_VUS
mach_error_t mach3_output_unix()
{
	if (write(2,"\n\r*** ",6) != 6) {
		return(KERN_FAILURE);
	}

#if	MACH3_UNIX || MACH3_VUS || MACH3_US
	if (mach3_diag_port != PORT_NULL) {
		(void) port_deallocate(task_self(),mach3_diag_port);
	}
#endif	MACH3_UNIX || MACH3_VUS || MACH3_US

	mach3_output_channel = MACH3_OUT_UNIX;

	/* fprintf(stderr,"MACH3 output now handled through UNIX traps.\n"); */
	return(ERR_SUCCESS);
}
#endif	MACH3_UNIX || MACH3_VUS

#if	MACH3_UNIX || MACH3_VUS || MACH3_US
mach_error_t mach3_init_diag(host,diag_name)
	char		*host;
	char		*diag_name;
{
	mach_error_t	ret;
	port_t		server_port;
	port_t		old_diag_port = mach3_diag_port;

	ret = netname_look_up(name_server_port,host,DIAG_NAME,
							&server_port);
	if (ret != ERR_SUCCESS) {
		return(ret);
	}
	ret = diag_checkin(server_port,diag_name,&mach3_diag_port);
	(void) port_deallocate(task_self(),server_port);
	if (ret != ERR_SUCCESS) {
		return(ret);
	}
	if (old_diag_port != PORT_NULL) {
		(void) port_deallocate(task_self(),old_diag_port);
	}

	mach3_output_channel = MACH3_OUT_DIAG;

	return(ERR_SUCCESS);
}

mach_error_t mach3_output_diag(host,diag_name)
	char		*host;
	char		*diag_name;
{
	(void) mach3_init_diag(host,diag_name);

	setlinebuf(stdout);
	setlinebuf(stderr);

	/* fprintf(stderr,
	"\n*** MACH3 output now handled through the Diag server at \"%s\" (%s).\n",
							host,diag_name); 
	 */
	return(ERR_SUCCESS);
}

mach_error_t mach3_set_diag_name(name) {
	mach_error_t		ret;
	port_t			newport;

	if (mach3_diag_port == PORT_NULL) {
		return(KERN_FAILURE);
	}

	ret = diag_checkin(mach3_diag_port,name,&newport);
	if (ret != ERR_SUCCESS) {
		return(ret);
	}

	(void) port_deallocate(task_self(),mach3_diag_port);
	mach3_diag_port = newport;

	return(ERR_SUCCESS);
}


mach_error_t mach3_diag_clone_init(child)
{
	mach_error_t		ret;

	if (mach3_diag_port != PORT_NULL) {
		/*
		 * Give the remote port to the child.
		 */
		ret = port_insert_send(child,mach3_diag_port,mach3_diag_port);
		if (ret == KERN_NAME_EXISTS) {
			/* XXX - temp until no more senders */
			ret = ERR_SUCCESS;
		}
		if (ret != ERR_SUCCESS) {
			return(ret);
		}
	}

	return(ERR_SUCCESS);
}

#endif	MACH3_UNIX || MACH3_VUS || MACH3_US

mach_error_t mach3_output_console()
{
#if	MACH3_SA || MACH3_VUS || MACH3_US
	mach_error_t		ret;

	setlinebuf(stdout);
	setlinebuf(stderr);

	ret = console_write("\n\r*** ",6);
	if (ret != ERR_SUCCESS) {
		return(ret);
	}

#if	MACH3_UNIX || MACH3_VUS || MACH3_US
	if (mach3_diag_port != PORT_NULL) {
		(void) port_deallocate(task_self(),mach3_diag_port);
	}
#endif	MACH3_UNIX || MACH3_VUS || MACH3_US

	mach3_output_channel = MACH3_OUT_CONSOLE;

	/* fprintf(stderr,
	   "MACH3 output now handled through the raw console.\n\r"); */
	return(ERR_SUCCESS);
#else	MACH3_SA || MACH3_VUS || MACH3_US
	return(ERR_SUCCESS);
#endif	MACH3_SA || MACH3_VUS || MACH3_US
}


int		errno;


/*
 * Generic output routine.
 */
int mach3_write(fd,buf,len)
	int		fd;
	char		*buf;
	int		len;
{
	mach_error_t		ret;

	if ((mach3_output_channel != MACH3_OUT_UNIX) &&
		(fd != 1) && (fd != 2)) {
#if	MACH3_UNIX || MACH3_VUS
		fprintf(stderr,
			"Warning: writing on file descriptor %d\n",fd);
		return(write(fd,buf,len));
#else	MACH3_UNIX || MACH3_VUS
		fprintf(stderr,
			"Error: attempting to write on file descriptor %d\n",
									fd);
		errno = KERN_FAILURE;
		return(-1);
#endif	MACH3_UNIX || MACH3_VUS
	}

	switch(mach3_output_channel) {
#if	MACH3_UNIX || MACH3_VUS
		case MACH3_OUT_UNIX:
			return(write(fd,buf,len));
#endif	MACH3_UNIX || MACH3_VUS

#if	MACH3_UNIX || MACH3_VUS || MACH3_US
		case MACH3_OUT_DIAG:
			if (mach3_diag_port == PORT_NULL) {
				ret = mach3_init_diag("","printf");
				if (ret != ERR_SUCCESS) {
					errno = ret;
					return(-1);
				}
			}
			{
				char		str[256];
				len = (len > 255) ? 255 : len;
				bcopy(buf,str,len);
				str[len] = '\0';
				(void) diag_mesg(mach3_diag_port,mach3_diag_level,str);
				return(len);
			}
#endif	MACH3_UNIX || MACH3_VUS || MACH3_US

#if	MACH3_VUS || MACH3_US || MACH3_SA
		case MACH3_OUT_CONSOLE:
			ret = console_write(buf,len);
			if (ret != ERR_SUCCESS) {
				errno = ret;
				return(-1);
			}
			return(len);
#endif	MACH3_VUS || MACH3_US || MACH3_SA

		default:
			errno = KERN_FAILURE;
			return(-1);
	}
}

#endif	MACH3

char	*malloc();

_flsbuf(c, iop)
unsigned char c;
register FILE *iop;
{
	register char *base;
	register n, rn;
	char c1;
	int size;
#if	(! defined(MACH3)) || MACH3_UNIX || MACH3_VUS
	struct stat stbuf;
#endif	(! defined(MACH3)) || MACH3_UNIX || MACH3_VUS

	if (iop->_flag & _IORW) {
		iop->_flag |= _IOWRT;
		iop->_flag &= ~(_IOEOF|_IOREAD);
	}

	if ((iop->_flag&_IOWRT)==0)
		return(EOF);
#if	_FMAP
	if (iop->_flag&_IOMAP)
		return(EOF);
#endif
tryagain:
	if (iop->_flag&_IOSTRG) {
		rn = n = 0;
		if (--iop->_cnt >= 0)
			*iop->_ptr++ = c;
		else {
			n = -1;
			iop->_cnt = 0;
		}
	} else if (iop->_flag&_IOLBF) {
		base = iop->_base;
		*iop->_ptr++ = c;
		if (iop->_ptr >= base+iop->_bufsiz || c == '\n') {
#if	MACH3
			n = mach3_write(fileno(iop), base, rn = iop->_ptr - base);
#else	MACH3
			n = write(fileno(iop), base, rn = iop->_ptr - base);
#endif	MACH3
			iop->_ptr = base;
			iop->_cnt = 0;
		} else
			rn = n = 0;
	} else if (iop->_flag&_IONBF) {
		c1 = c;
		rn = 1;
#if	MACH3
		n = mach3_write(fileno(iop), &c1, rn);
#else	MACH3
		n = write(fileno(iop), &c1, rn);
#endif	MACH3
		iop->_cnt = 0;
	} else {
		if ((base=iop->_base)==NULL) {
#if	(! defined(MACH3)) || MACH3_UNIX || MACH3_VUS
			if (fstat(fileno(iop), &stbuf) < 0 ||
			    stbuf.st_blksize <= NULL)
				size = BUFSIZ;
			else
				size = stbuf.st_blksize;
#else	(! defined(MACH3)) || MACH3_UNIX || MACH3_VUS
				size = BUFSIZ;
#endif	(! defined(MACH3)) || MACH3_UNIX || MACH3_VUS
			if ((iop->_base=base=malloc(size)) == NULL) {
				iop->_flag |= _IONBF;
				goto tryagain;
			}
			iop->_flag |= _IOMYBUF;
			iop->_bufsiz = size;
#if	(! defined(MACH3)) || MACH3_UNIX || MACH3_VUS
			if (iop==stdout && isatty(fileno(stdout))) {
				iop->_flag |= _IOLBF;
				iop->_ptr = base;
				goto tryagain;
			}
#endif	(! defined(MACH3)) || MACH3_UNIX || MACH3_VUS
			rn = n = 0;
		} else if ((rn = n = iop->_ptr - base) > 0) {
			iop->_ptr = base;
#if	MACH3
			n = mach3_write(fileno(iop), base, n);
#else	MACH3
			n = write(fileno(iop), base, n);
#endif	MACH3
		}
		iop->_cnt = iop->_bufsiz-1;
		*base++ = c;
		iop->_ptr = base;
	}
	if (rn != n) {
		iop->_flag |= _IOERR;
		return(EOF);
	}
	return(c);
}

fflush(iop)
register FILE *iop;
{
	register char *base;
	register n;
#if	_FMAP
	register char	c, *mptr;
#endif
#if	PARALLEL
	register f_buflock buflock;
#endif

#if	PARALLEL
	buflock = f_lockbuf(iop);
#endif
#if	_FMAP
	if ((base=iop->_base)!=NULL && ((iop->_flag&(_IONBF|_IOWRT))==_IOWRT
	    || (iop->_flag&(_IORW|_IOMAP))==(_IORW|_IOMAP)))
	{   if (iop->_flag&_IOMAP){
		for(mptr=iop->_base; mptr<= iop->_base+iop->_bufsiz; 
		    mptr += vm_page_size) {
			c = *mptr;
#ifdef	lint
			c = c;
#endif
		}
		n = iop->_bufsiz;
	    }
	    else if ((n=iop->_ptr-base)>0) 
	    {
#else	/* _FMAP */
	if ((iop->_flag&(_IONBF|_IOWRT))==_IOWRT
	 && (base=iop->_base)!=NULL && (n=iop->_ptr-base)>0) {
#endif	/* _FMAP */
		iop->_ptr = base;
		iop->_cnt = (iop->_flag&(_IOLBF|_IONBF)) ? 0 : iop->_bufsiz;
#if	_FMAP
	    }
#endif
#if	MACH3
		if (mach3_write(fileno(iop), base, n)!=n) {
#else	MACH3
		if (write(fileno(iop), base, n)!=n) {
#endif	MACH3
			iop->_flag |= _IOERR;
#if	PARALLEL
			f_unlockbuf(buflock);
#endif
			return(EOF);
		}
#if	_FMAP
	    if (iop->_flag&_IOMAP)
		lseek(fileno(iop), (off_t) 0, L_SET);
#endif
	}
#if	PARALLEL
	f_unlockbuf(buflock);
#endif
	return(0);
}

#if	(! defined(MACH3)) || MACH3_UNIX || MACH3_VUS
fclose(iop)
	register FILE *iop;
{
	register int r;
#if	PARALLEL
	register f_buflock buflock;
#endif

#if	PARALLEL
	buflock = f_lockbuf(iop);
#endif
	r = EOF;
	if (iop->_flag&(_IOREAD|_IOWRT|_IORW) && (iop->_flag&_IOSTRG)==0) {
		r = fflush(iop);
		if (close(fileno(iop)) < 0)
			r = EOF;
		if (iop->_flag&_IOMYBUF)
			free(iop->_base);
#if	_FMAP
		else if (iop->_flag&_IOMAP)
			(void)vm_deallocate(task_self_,
				(vm_address_t)iop->_base, iop->_bufsiz);
#endif
	}
	iop->_cnt = 0;
	iop->_base = (char *)NULL;
	iop->_ptr = (char *)NULL;
	iop->_bufsiz = 0;
	iop->_flag = 0;
	iop->_file = 0;
#if	PARALLEL
	f_unlockbuf(buflock);
#endif
	return(r);
}
#endif	(! defined(MACH3)) || MACH3_UNIX || MACH3_VUS
