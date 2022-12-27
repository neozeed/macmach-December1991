/* 
 * Mach Operating System
 * Copyright (c) 1990 Carnegie-Mellon University
 * Copyright (c) 1989 Carnegie-Mellon University
 * All rights reserved.  The CMU software License Agreement specifies
 * the terms and conditions for use and redistribution.
 */
/*
 * HISTORY
 * $Log:	block_io.c,v $
 * Revision 2.5  91/03/20  15:04:41  dbg
 * 	Move bdev_open and bdev_close here from device_misc.c.  Use new
 * 	macros to distinguish block and character device numbers.
 * 
 * 	Remove vm_pageable calls from buffers.
 * 	[90/11/07            dbg]
 * 
 * Revision 2.4  90/09/09  14:13:31  rpd
 * 	Increase debugging info.
 * 	[90/09/05            rwd]
 * 
 * Revision 2.3  90/06/19  23:14:16  rpd
 * 	Fixed bio_strategy to not call bio_read_reply/bio_write_reply.
 * 	Fixed bio_read_reply/bio_write_reply to protect ALL buffer
 * 	munging with SPLBIO.
 * 	[90/06/11            rpd]
 * 
 * Revision 2.2  90/06/02  15:26:39  rpd
 * 	Converted to new IPC.
 * 	[90/03/26  20:06:28  rpd]
 * 
 */

/*
 * Block IO using MACH KERNEL interface.
 */

#include <sys/param.h>
#include <sys/buf.h>
#include <sys/errno.h>
#include <sys/synch.h>

#include <uxkern/import_mach.h>
#include <uxkern/device_reply_hdlr.h>
#include <uxkern/device_utils.h>

#include <device/device.h>

/*
 * We store the block-io port in the hash table
 */
#define	bio_port_enter(dev, port) \
		dev_number_hash_enter(XDEV_BLOCK(dev), (char *)(port))
#define	bio_port_remove(dev)	\
		dev_number_hash_remove(XDEV_BLOCK(dev))
#define	bio_port_lookup(dev)	\
		((mach_port_t)dev_number_hash_lookup(XDEV_BLOCK(dev)))

kern_return_t	bio_read_reply();
kern_return_t	bio_write_reply();

struct mutex	bio_lock = MUTEX_INITIALIZER;

/*
 * Initialize the buffer system.  The buffer headers have already
 * been allocated.
 */
void
bio_init()
{
	register int	i;
	register struct bufhd *bhp;
	register struct buf *bp;
	vm_offset_t	buf_addr;
	vm_size_t	buf_size;

	for (bhp = bufhash, i = 0; i < BUFHSZ; i++, bhp++) {
	    bhp->b_forw = (struct buf *)bhp;
	    bhp->b_back = (struct buf *)bhp;
	}

	for (bp = bfreelist; bp < &bfreelist[BQUEUES]; bp++) {
	    bp->b_forw = bp;
	    bp->b_back = bp;
	    bp->av_forw = bp;
	    bp->av_back = bp;
	    bp->b_flags = B_HEAD;
	}

	/*
	 * Existing code wants buffers to have some memory.
	 */
	buf_size = round_page(MAXBSIZE);
	(void) vm_allocate(mach_task_self(),
			   &buf_addr,
			   nbuf*buf_size,
			   TRUE);

	for (bp = buf, i = 0; i < nbuf; bp++, i++) {
	    bp->b_dev = NODEV;
	    bp->b_bcount = 0;
	    bp->b_un.b_addr = (char *)buf_addr + i * buf_size;
	    bp->b_bufsize = buf_size;

	    /* allocate one reply port per buffer (the Accent way...) */
	    bp->b_reply_port = mach_reply_port();
	    reply_hash_enter(bp->b_reply_port,
			     (char *)bp,
			     bio_read_reply,
			     bio_write_reply);

	    binshash(bp, &bfreelist[BQ_AGE]);
	    bp->b_flags = B_BUSY | B_INVAL;
	    brelse(bp);
	}
}

bio_strategy(bp)
	struct buf *	bp;
{
	dev_t	dev;
	mach_port_t	device_port;
	kern_return_t	error;

	/*
	 * Find the request port for the device.
	 */
	device_port = bio_port_lookup(bp->b_dev);
	if (device_port == MACH_PORT_NULL)
		panic("bio_strategy null port");

	/*
	 * Start the IO.  XXX What should we do in case of error???
	 * Calling bio_read_reply/bio_write_reply isn't correct,
	 * because they use interrupt_enter/interrupt_exit.
	 */

	if (bp->b_flags & B_READ) {
	    error = device_read_request(device_port,
					bp->b_reply_port,
					0,
					bp->b_blkno,
					(unsigned)bp->b_bcount);
	    if (error != KERN_SUCCESS)
		panic("bio_strategy read request", error);
	} else {
	    error = device_write_request(device_port,
					 bp->b_reply_port,
					 0,
					 bp->b_blkno,
					 bp->b_un.b_addr,
					 bp->b_bcount);
	    if (error != KERN_SUCCESS)
		panic("bio_strategy write request",error);
	}
}

kern_return_t
bio_read_reply(bp_ptr, return_code, data, data_count)
	char *		bp_ptr;
	kern_return_t	return_code;
	char		*data;
	unsigned int	data_count;
{
	register struct buf *bp = (struct buf *)bp_ptr;
	vm_offset_t dealloc_addr;
	vm_size_t dealloc_size = 0;

	interrupt_enter(SPLBIO);
	if (return_code != D_SUCCESS) {
	    bp->b_flags |= B_ERROR;
	    bp->b_error = EIO;
	} else {
	    /*
	     * Deallocate old memory.  Actually do it later,
	     * after we have lowered IPL.
	     */
	    if (bp->b_bufsize > 0) {
		dealloc_addr = (vm_offset_t) bp->b_un.b_addr;
		dealloc_size = (vm_size_t) bp->b_bufsize;
		bp->b_bufsize = 0;
	    }

	    if (data_count < bp->b_bcount) {
		bp->b_flags |= B_ERROR;
		bp->b_resid = bp->b_bcount - data_count;
	    }
	    bp->b_un.b_addr = data;
	    bp->b_bufsize = round_page(data_count);
	}
	biodone(bp);
	interrupt_exit(SPLBIO);

	if (dealloc_size != 0)
	    (void) vm_deallocate(mach_task_self(), dealloc_addr, dealloc_size);
}

kern_return_t
bio_write_reply(bp_ptr, return_code, bytes_written)
	char *		bp_ptr;
	kern_return_t	return_code;
	int		bytes_written;
{
	register struct buf *bp = (struct buf *)bp_ptr;

	interrupt_enter(SPLBIO);
	if (return_code != D_SUCCESS) {
	    bp->b_flags |= B_ERROR;
	    bp->b_error = EIO;
	} else if (bytes_written < bp->b_bcount) {
	    bp->b_flags |= B_ERROR;
	    bp->b_resid = bp->b_bcount - bytes_written;
	}
	biodone(bp);
	interrupt_exit(SPLBIO);
}

/*
 * Release space associated with a buffer.
 */
bfree(bp)
	struct buf *bp;
{
	bp->b_bcount = 0;
}

/*
 * Expand or contract the actual memory allocated to a buffer.
 * If no memory is available, release buffer and take error exit
 */
allocbuf(bp, size)
	register struct buf *bp;
	int size;
{
	vm_size_t	current_size, desired_size;
	vm_offset_t	new_start;

	current_size = bp->b_bufsize;
	desired_size = round_page(size);

	if (current_size < desired_size) {
	    /*
	     * Buffer is growing.
	     * If buffer already has data, allocate new area and copy
	     * old data to it.
	     */
	    (void) vm_allocate(mach_task_self(),
			       &new_start,
			       desired_size,
			       TRUE);
	    bcopy(bp->b_un.b_addr,
		  (caddr_t) new_start,
		  bp->b_bufsize);
	    (void) vm_deallocate(mach_task_self(),
				 (vm_offset_t)bp->b_un.b_addr,
				 current_size);
	    bp->b_un.b_addr = (char *)new_start;
	    bp->b_bufsize = desired_size;
	}
	bp->b_bcount = size;
	return (1);
}

/*
 * Open block device.
 */
int
bdev_open(dev, flag)
	dev_t	dev;
	int	flag;
{
	char		name[32];
	kern_return_t	rc;
	mach_port_t	device_port;
	int		mode;

	/*
	 * See whether we have opened the device already.
	 */
	if (bio_port_lookup(dev)) {
	    return (0);
	}

	rc = bdev_name_string(dev, name);
	if (rc != 0)
	    return (rc);

	/* fix modes */
	mode = 0;	/* XXX */
	rc = device_open(device_server_port,
			 mode,
			 name,
			 &device_port);
	if (rc != D_SUCCESS)
	    return (dev_error_to_errno(rc));

	bio_port_enter(dev, device_port);
	return (0);
}

int
bdev_close(dev, flag)
	dev_t	dev;
	int	flag;
{
	mach_port_t	device_port;
	int		error;

	device_port = bio_port_lookup(dev);
	if (device_port == MACH_PORT_NULL)
	    return (ENXIO);	/* shouldn't happen */

	bio_port_remove(dev);
	error = dev_error_to_errno(device_close(device_port));
	(void) mach_port_deallocate(mach_task_self(), device_port);
	return (error);
}

