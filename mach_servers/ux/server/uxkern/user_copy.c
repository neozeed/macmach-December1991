/* 
 **********************************************************************
 * Mach Operating System
 * Copyright (c) 1987 Carnegie-Mellon University
 * All rights reserved.  The CMU software License Agreement specifies
 * the terms and conditions for use and redistribution.
 **********************************************************************
 *
 * HISTORY
 * $Log:	user_copy.c,v $
 * Revision 2.7  90/11/05  16:59:29  rpd
 * 	Removed old code from useracc.
 * 	[90/11/02            rpd]
 * 
 * Revision 2.6  90/06/02  15:28:44  rpd
 * 	Revamped for new extend_reply_msg/extend_current_output.
 * 	[90/04/27            rpd]
 * 
 * 	In uiomove, must drop master lock when potentially touching
 * 	memory received out-of-line from a user.
 * 	[90/04/04            rpd]
 * 	Converted to new IPC.
 * 	Backed out vm_write optimization in copyout.
 * 	[90/03/26  20:24:45  rpd]
 * 
 * Revision 2.5  90/05/29  20:25:26  rwd
 * 	Copyout doesn't play with master lock in NBC case since the NBC
 * 	code deals with it itself.
 * 	[90/03/16            rwd]
 * 
 * Revision 2.4  90/03/14  21:31:04  rwd
 * 	Release master lock on copyout!
 * 	[90/02/14            rwd]
 * 
 * Revision 2.3  90/01/19  14:38:16  rwd
 * 	Get changes from rfr (use vm_write).
 * 	[90/01/05            rwd]
 * 
 * Revision 2.2  89/12/08  20:16:53  rwd
 * 	Need to include sys/parallel.h
 * 	[89/10/30            rwd]
 * 
 * Revision 2.1  89/08/04  14:50:48  rwd
 * Created.
 * 
 * 13-Apr-89  David Golub (dbg) at Carnegie-Mellon University
 *	Treat user space as system space if u.u_user_is_kernel is set.
 *
 * Out-of-kernel version - moved to uxkern/user_copy.c.
 * Includes all user-to-kernel and kernel-to-user copying routines.
 */ 
 
/*
 * Copyright (c) 1982, 1986 Regents of the University of California.
 * All rights reserved.  The Berkeley software License Agreement
 * specifies the terms and conditions for redistribution.
 *
 *	@(#)kern_subr.c	7.1 (Berkeley) 6/5/86
 */

#include <mach_nbc.h>

#include <sys/param.h>
#include <sys/systm.h>
#include <sys/dir.h>
#include <sys/user.h>
#include <sys/proc.h>
#include <sys/uio.h>
#include <sys/parallel.h>

#include <uxkern/import_mach.h>

extern char *extend_reply_msg();
extern void extend_current_output();

uiomove(cp, n, rw, uio)
	register caddr_t cp;
	register int n;
	enum uio_rw rw;
	register struct uio *uio;
{
	register struct iovec *iov;
	u_int cnt;
	int error = 0;

	while (n > 0 && uio->uio_resid) {
		iov = uio->uio_iov;
		cnt = iov->iov_len;
		if (cnt == 0) {
			uio->uio_iov++;
			uio->uio_iovcnt--;
			continue;
		}
		if (cnt > n)
			cnt = n;
		if (uio->uio_segflg == UIO_SYSSPACE || u.u_reply_msg == 0) {
		    /*UIO_SYSSPACE*/
			if (rw == UIO_READ)
				bcopy(cp, iov->iov_base, cnt);
			else {
				/*
				 * Unlock master lock to touch user data.
				 */
				if (u.u_master_lock)
				    master_unlock();

				bcopy(iov->iov_base, cp, cnt);

				/*
				 * Re-lock master lock.
				 */
				if (u.u_master_lock)
				    master_lock();
			}
		}
		else {
		    /*UIO_USERSPACE*/
			if (rw == UIO_READ) {
				register caddr_t user_addr;

				user_addr = extend_reply_msg(iov->iov_base,
							     iov->iov_len);
				bcopy(cp, user_addr, cnt);
				extend_current_output(cnt);
			}
			else {
				error = copyin(iov->iov_base, cp, cnt);
				if (error)
					return (error);
			}
		}
		iov->iov_base += cnt;
		iov->iov_len -= cnt;
		uio->uio_resid -= cnt;
		uio->uio_offset += cnt;
		cp += cnt;
		n -= cnt;
	}
	return (error);
}

/*
 * Give next character to user as result of read.
 */
ureadc(c, uio)
	register int c;
	register struct uio *uio;
{
	register struct iovec *iov;

again:
	if (uio->uio_iovcnt == 0)
		panic("ureadc");
	iov = uio->uio_iov;
	if (iov->iov_len <= 0 || uio->uio_resid <= 0) {
		uio->uio_iovcnt--;
		uio->uio_iov++;
		goto again;
	}
	if (uio->uio_segflg == UIO_SYSSPACE || u.u_reply_msg == 0) {
	    /*UIO_SYSSPACE*/
		*iov->iov_base = c;
	}
	else {
	    /*UIO_USERSPACE*/
		register char *		user_addr;

		user_addr = extend_reply_msg(iov->iov_base, iov->iov_len);
		*user_addr = c;
		extend_current_output(1);
	}
	iov->iov_base++;
	iov->iov_len--;
	uio->uio_resid--;
	uio->uio_offset++;
	return (0);
}

copyout(from, to, len)
	caddr_t	from, to;
	unsigned int	len;
{
	if (u.u_reply_msg == 0) {
	    /*UIO_SYSSPACE*/
		bcopy(from, to, len);
	}
	else {
	    /*UIO_USERSPACE*/
		register caddr_t	user_addr;

		user_addr = extend_reply_msg(to, len);
		bcopy(from, user_addr, len);
		extend_current_output(len);
	}
	return (0);
}

/*----------------*/

/*
 * Get next character written in by user from uio.
 */
uwritec(uio)
	struct uio *uio;
{
	register struct iovec *iov;
	register int c;

	if (uio->uio_resid <= 0)
		return (-1);
again:
	if (uio->uio_iovcnt <= 0)
		panic("uwritec");
	iov = uio->uio_iov;
	if (iov->iov_len == 0) {
		uio->uio_iov++;
		if (--uio->uio_iovcnt == 0)
			return (-1);
		goto again;
	}
	if (uio->uio_segflg == UIO_SYSSPACE || u.u_reply_msg == 0) {
	    /*UIO_SYSSPACE*/
		c = *iov->iov_base & 0377;
	}
	else {
	    /*UIO_USERSPACE*/
		c = fubyte(iov->iov_base);
	}
	if (c < 0)
		return (-1);
	iov->iov_base++;
	iov->iov_len--;
	uio->uio_resid--;
	uio->uio_offset++;
	return (c & 0377);
}

int copyin(from, to, len)
	char *	from;
	char *	to;
	unsigned int	len;
{
	vm_offset_t	start, end;
	char		*data;
	vm_size_t	data_len;
	kern_return_t	result;

	if (u.u_reply_msg == 0) {
	    /*UIO_SYSSPACE*/
		bcopy(from, to, len);
		return (0);
	}
	    /*UIO_USERSPACE*/

	start = trunc_page((vm_offset_t)from);
	end   = round_page((vm_offset_t)from + len);

	/*
	 * Unlock master lock to touch user data.
	 */
	if (u.u_master_lock)
	    master_unlock();

	result = vm_read(u.u_procp->p_task,
			 start,
			 (vm_size_t)(end - start),
			 &data,
			 &data_len);
	if (result == KERN_SUCCESS) {
	    bcopy(data + ((vm_offset_t)from - start),
		  to,
		  len);
	    (void) vm_deallocate(mach_task_self(),
				(vm_offset_t)data,
				data_len);
	}
	else {
	    result = EFAULT;
	}

	/*
	 * Re-lock master lock.
	 */
	if (u.u_master_lock)
	    master_lock();

	return (result);
}

int fuword(addr)
	caddr_t	addr;
{
	int	word;

	if (copyin(addr, &word, sizeof(word)))
	    return (-1);
	return (word);
}

int fubyte(addr)
	caddr_t	addr;
{
	char	byte;

	if (copyin(addr, &byte, sizeof(byte)))
	    return (-1);
	return ((int)byte & 0xff);
}

copyinstr(from, to, max_len, len_copied)
	caddr_t	from;
	register caddr_t to;
	int	max_len;
	int	*len_copied;
{
	vm_offset_t	start, end;
	char		* data;
	vm_size_t	data_len;
	kern_return_t	result;

	register int	count, cur_max;
	register char	* cp;

	if (u.u_reply_msg == 0) {
	    /*UIO_SYSSPACE*/
		return (copystr(from, to, max_len, len_copied));
	}
	    /*UIO_USERSPACE*/

	/*
	 * Unlock master lock to touch user data.
	 */
	if (u.u_master_lock)
	    master_unlock();

	count = 0;

	while (count < max_len) {
	    start = trunc_page((vm_offset_t)from);
	    end   = start + vm_page_size;

	    result = vm_read(u.u_procp->p_task,
			     start,
			     vm_page_size,
			     &data,
			     &data_len);
	    if (result != KERN_SUCCESS) {
		/*
		 * Re-lock master lock.
		 */
		if (u.u_master_lock)
		    master_lock();

		return (EFAULT);
	    }

	    cur_max = end - (vm_offset_t)from;
	    if (cur_max > max_len)
		cur_max = max_len;

	    cp = data + ((vm_offset_t)from - start);
	    while (count < cur_max) {
		count++;
		if ((*to++ = *cp++) == 0) {
		    goto done;
		}
	    }
	    (void) vm_deallocate(mach_task_self(),
				 (vm_offset_t)data,
				 data_len);
	    from = (char *)end;
	}

	/*
	 * Re-lock master lock.
	 */
	if (u.u_master_lock)
	    master_lock();

	return (ENOENT);

    done:
	if (len_copied)
	    *len_copied = count;

	(void) vm_deallocate(mach_task_self(),
			     (vm_offset_t)data,
			     data_len);
	/*
	 * Re-lock master lock.
	 */
	if (u.u_master_lock)
	    master_lock();

	return (0);
}

copystr(from, to, max_len, len_copied)
	register caddr_t from, to;
	int		max_len;
	int		*len_copied;
{
	register int	count;

	count = 0;
	while (count < max_len) {
	    count++;
	    if ((*to++ = *from++) == 0) {
		goto done;
	    }
	}
	return (ENOENT);
    done:
	if (len_copied)
	    *len_copied = count;
	return (0);
}

/*
 * Check address.
 * Given virtual address, byte count, and flag (nonzero for READ).
 * Returns 0 on no access.
 */
useracc(user_addr, len, direction)
	caddr_t	user_addr;
	u_int	len;
	int	direction;
{
	/*
	 * We take the 'hit' in the emulator (SIGSEGV) instead of the
	 * UX server (EFAULT).  If we fault, the system call will have
	 * been completed: this is a change to the existing semantics,
	 * but only brain-damaged programs should be able to tell
	 * the difference.
	 */
	return (1);	/* always */
}
