/* 
 * Mach Operating System
 * Copyright (c) 1988 Carnegie-Mellon University
 * Copyright (c) 1987 Carnegie-Mellon University
 * All rights reserved.  The CMU software License Agreement specifies
 * the terms and conditions for use and redistribution.
 */

/* 
 *  Compatibility support for common vendor system calls.
 *
 *  WHERE DID THIS ORIGINALLY CODE COME FROM?
 *
 * HISTORY
 * $Log:	xxx_syscalls.c,v $
 * Revision 2.1  89/08/04  14:16:50  rwd
 * Created.
 * 
 * Revision 2.3  88/12/19  02:39:54  mwyoung
 * 	Fix include file references.
 * 	[88/12/18  23:53:36  mwyoung]
 * 	
 * 	Removed lint.
 * 	[88/12/09            mwyoung]
 * 
 * 30-Mar-87  Robert Baron (rvb) at Carnegie-Mellon University
 *	Extend getdirentries to RFS for now, but not to vice.
 *	use rwuio vs rwip -- really simple.
 *
 *  8-Jan-87  Robert Beck (beck) at Sequent Computer Systems, Inc.
 *	Sequent specific system calls.
 *
 */ 

#include <cmucs_rfs.h>
#include <vice.h>


#include <sys/param.h>
#include <sys/systm.h>
#include <sys/dir.h>
#include <sys/user.h>
#include <sys/kernel.h>
#include <sys/file.h>
#include <sys/inode.h>
#include <sys/fs.h>
#include <sys/buf.h>
#include <sys/proc.h>
#include <sys/uio.h>


/*
 * getdirentries()
 *	Read directory entries in a file system independent format.
 *
 * Adapted from VFS code of the same function.
 */

getdirentries()
{
	register struct a {
		int	fd;
		char	*buf;
		unsigned count;
		long	*basep;
	} *uap = (struct a *) u.u_ap;
	register struct file *fp;
	struct uio auio;
	struct iovec aiov;

	GETF(fp, uap->fd)
	if (fp->f_type != DTYPE_INODE
#if	CMUCS_RFS
					&&
	    fp->f_type != DTYPE_RFSINO
#endif	CMUCS_RFS
#if	VICE
					&&
	    fp->f_type != DTYPE_VICEINO
#endif	VICE
					)
	{
		u.u_error = EBADF;
		return;
	}

	if ((fp->f_flag & FREAD) == 0) {
		u.u_error = EBADF;
		return;
	}
	/*
	 * Must read at least one directory block and read from
	 * block boundary.
	 */

	if (uap->count < DIRBLKSIZ || (fp->f_offset & (DIRBLKSIZ-1))) {
		u.u_error = EINVAL;
		return;
	}

	/*
	 * Only read an integral number of directory blocks.
	 */

	aiov.iov_base = uap->buf;
	aiov.iov_len = uap->count & ~(DIRBLKSIZ-1);

	auio.uio_iov = &aiov;
	auio.uio_iovcnt = 1;
	u.u_error = copyout((caddr_t)&fp->f_offset, (caddr_t)uap->basep,
								sizeof(long));
	if (u.u_error)
		return;

	if (fp->f_type == DTYPE_INODE)
		ILOCK((struct inode *)fp->f_data);

	rwuio(&auio, UIO_READ);		/* NB.  rwuio expects 1st arg*/
					/* in u.u_ap to be a file descriptor*/

	if (fp->f_type == DTYPE_INODE)
		IUNLOCK((struct inode *)fp->f_data);

}

char	domainname[32];
int	domainnamelen = 0;

getdomainname()
{
	register struct a {
		char	*domainname;
		int	len;
	} *uap = (struct a *)u.u_ap;
	register u_int len;

	len = uap->len;
	if (len > domainnamelen + 1)
		len = domainnamelen + 1;
	u.u_error = copyout((caddr_t)domainname,(caddr_t)uap->domainname,len);
}

setdomainname()
{
	register struct a {
		char	*domainname;
		u_int	len;
	} *uap = (struct a *)u.u_ap;

	if (!suser())
		return;
	if (uap->len > sizeof (domainname) - 1) {
		u.u_error = EINVAL;
		return;
	}
	domainnamelen = uap->len;
	u.u_error = copyin((caddr_t)uap->domainname, domainname, uap->len);
	domainname[domainnamelen] = 0;
}
