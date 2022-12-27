/* sony.c */

#include <sys/param.h>
#include <sys/types.h>
#include <sys/fs.h>		/* btodb */
#include <sys/uio.h>
#include <sys/ioctl.h>
#include <sys/errno.h>

#include <uxkern/device_utils.h>

#define	disk_port_lookup(dev) \
		((mach_port_t) dev_number_hash_lookup(XDEV_CHAR(dev)))

sony_ioctl(dev, cmd, data, flag)
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
				      cmd & 0xFF,
				      (int *)data,
				      count);
	    if (error)
		return (dev_error_to_errno(error));
	}
	if (cmd & IOC_OUT) {
	    error = device_get_status(device_port,
				      cmd & 0xFF,
				      (int *)data,
				      &count);
	}
	if (error)
	     return (dev_error_to_errno(error));
	else
	     return (0);
}

