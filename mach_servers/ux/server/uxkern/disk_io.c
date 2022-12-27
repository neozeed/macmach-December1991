/* 
 * Mach Operating System
 * Copyright (c) 1991 Carnegie-Mellon University
 * Copyright (c) 1990 Carnegie-Mellon University
 * Copyright (c) 1989 Carnegie-Mellon University
 * All rights reserved.  The CMU software License Agreement specifies
 * the terms and conditions for use and redistribution.
 */
/*
 * HISTORY
 * $Log:	disk_io.c,v $
 * Revision 2.8  91/03/20  14:35:56  dbg
 * 	Fixed handling of IOC_IN vz IOC_OUT as per char_ioctl().
 * 	[91/02/21  11:54:37  af]
 * 
 * Revision 2.7  91/03/12  21:59:23  dbg
 * 	Use new macros to distinguish block and character device numbers.
 * 	Add disk_port.  Move i386 IOCTL support to i386/conf.c.
 * 	[91/02/21            dbg]
 * 
 * Revision 2.6  91/01/08  17:44:29  rpd
 * 	Fix V_VERIFY.
 * 	[91/01/08  16:43:28  rvb]
 * 
 * Revision 2.5  90/09/09  14:13:41  rpd
 * 	Deallocate the device port after closing it.
 * 	[90/08/23            rpd]
 * 
 * Revision 2.4  90/06/02  15:27:36  rpd
 * 	Converted to new IPC.
 * 	[90/03/26  20:14:43  rpd]
 * 
 * Revision 2.3  89/11/15  13:27:40  dbg
 * 	Add open and close routines (from old character IO).
 * 	[89/10/27            dbg]
 * 
 * Revision 2.2  89/09/15  15:29:35  rwd
 * 	Change includes
 * 	[89/09/11            rwd]
 * 
 */
/*
 * Routines for char IO to block devices.
 */
#include <sys/param.h>
#include <sys/types.h>
#include <sys/fs.h>		/* btodb */
#include <sys/uio.h>
#include <sys/ioctl.h>
#include <sys/errno.h>

#include <uxkern/device_utils.h>

#define	disk_port_enter(dev, port) \
		dev_number_hash_enter(XDEV_CHAR(dev), (char *)(port))
#define	disk_port_remove(dev) \
		dev_number_hash_remove(XDEV_CHAR(dev))
#define	disk_port_lookup(dev) \
		((mach_port_t) dev_number_hash_lookup(XDEV_CHAR(dev)))

int
disk_open(dev, flag)
	dev_t	dev;
	int	flag;
{
	char		name[32];
	kern_return_t	rc;
	mach_port_t	device_port;
	int		mode;

	rc = cdev_name_string(dev, name);
	if (rc != 0)
	    return (rc);	/* bad name */

	/* fix modes */
	mode = 0;	/* XXX */
	rc = device_open(device_server_port,
			 mode,
			 name,
			 &device_port);
	if (rc != D_SUCCESS)
	    return (dev_error_to_errno(rc));

	disk_port_enter(dev, device_port);
	return (0);
}

int
disk_close(dev, flag)
	dev_t	dev;
	int	flag;
{
	mach_port_t	device_port;
	int		error;

	device_port = disk_port_lookup(dev);
	if (device_port == MACH_PORT_NULL)
	    return (0);		/* should not happen */

	disk_port_remove(dev);
	error = dev_error_to_errno(device_close(device_port));
	(void) mach_port_deallocate(mach_task_self(), device_port);
	return (error);
}

int
disk_read(dev, uio)
	dev_t		dev;
	struct uio	*uio;
{
	register struct iovec *iov;
	register int	c;
	register kern_return_t	rc;
	io_buf_ptr_t	data;
	unsigned int	count;

	while (uio->uio_iovcnt > 0) {

	    iov = uio->uio_iov;
	    if (useracc(iov->iov_base, (u_int)iov->iov_len, 0) == 0)
		return (EFAULT);

	    /*
	     * Can read entire block here - device handler
	     * breaks into smaller pieces.
	     */

	    c = iov->iov_len;

	    rc = device_read(disk_port_lookup(dev),
			     0,	/* mode */
			     btodb(uio->uio_offset),
			     iov->iov_len,
			     &data,
			     &count);
	    if (rc != 0)
		return (dev_error_to_errno(rc));

	    (void) moveout(data, iov->iov_base, count);
			/* deallocates data (eventually) */

	    iov->iov_base += count;
	    iov->iov_len -= count;
	    uio->uio_resid -= count;
	    uio->uio_offset += count;

	    /* temp kludge for tape drives */
	    if (count < c)
		break;

	    uio->uio_iov++;
	    uio->uio_iovcnt--;
	}
	return (0);
}

int
disk_write(dev, uio)
	dev_t		dev;
	struct uio	*uio;
{
	register struct iovec *iov;
	register int	c;
	register kern_return_t	rc;
	vm_offset_t	kern_addr;
	vm_size_t	kern_size;
	vm_offset_t	off;
	vm_size_t	count;

	while (uio->uio_iovcnt > 0) {
	    iov = uio->uio_iov;

	    kern_size = iov->iov_len;
	    (void) vm_allocate(mach_task_self(), &kern_addr, kern_size, TRUE);
	    if (copyin(iov->iov_base, kern_addr, (u_int)iov->iov_len)) {
		(void) vm_deallocate(mach_task_self(), kern_addr, kern_size);
		return (EFAULT);
	    }

	    /*
	     * Can write entire block here - device handler
	     * breaks into smaller pieces.
	     */

	    c = iov->iov_len;

	    rc = device_write(disk_port_lookup(dev),
			      0,	/* mode */
			      btodb(uio->uio_offset),
			      kern_addr,
			      iov->iov_len,
			      &count);

	    (void) vm_deallocate(mach_task_self(), kern_addr, kern_size);

	    if (rc != 0)
		return (dev_error_to_errno(rc));

	    iov->iov_base += count;
	    iov->iov_len -= count;
	    uio->uio_resid -= count;
	    uio->uio_offset += count;

	    /* temp kludge for tape drives */
	    if (count < c)
		break;

	    uio->uio_iov++;
	    uio->uio_iovcnt--;
	}
	return (0);
}

disk_ioctl(dev, cmd, data, flag)
	dev_t	dev;
	int	cmd;
	caddr_t	data;
	int	flag;
{
	mach_port_t	device_port = disk_port_lookup(dev);
	unsigned int	count;
	register int	error;

	count = (cmd & ~(IOC_INOUT|IOC_VOID)) >> 16; /* bytes */
	count = (count + 3) >> 2;		     /* ints */
	if (count == 0)
	    count = 1;

	if (cmd & (IOC_VOID|IOC_IN)) {
	    error = device_set_status(device_port,
				      cmd,
				      (int *)data,
				      count);
	    if (error)
		return (dev_error_to_errno(error));
	}
	if (cmd & IOC_OUT) {
	    error = device_get_status(device_port,
				      cmd,
				      (int *)data,
				      &count);
	}
	if (error)
	     return (dev_error_to_errno(error));
	else
	     return (0);
}

mach_port_t
disk_port(dev)
	dev_t	dev;
{
	return (disk_port_lookup(dev));
}
