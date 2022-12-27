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
 * $Log:	device_utils.c,v $
 * Revision 2.4  91/03/20  15:05:06  dbg
 * 	Distinguish block and character devices.
 * 	[91/02/21            dbg]
 * 
 * Revision 2.3  90/11/05  16:57:51  rpd
 * 	Added per-bucket locks and counts.
 * 	[90/10/31            rpd]
 * 
 * Revision 2.2  90/10/25  15:07:19  rwd
 * 	Change hash function.
 * 	[90/10/15            rwd]
 * 
 */
/*
 * Support routines for device interface in out-of-kernel kernel.
 */

#include <uxkern/device_utils.h>
#include <sys/errno.h>

#include <sys/queue.h>
#include <sys/zalloc.h>
#include <cthreads.h>

/*
 * device_number to pointer hash table.
 */
#define	NDEVHASH	8
#define	DEV_NUMBER_HASH(dev)	\
		((major(dev) ^ minor(dev)) & (NDEVHASH-1))

struct dev_number_hash {
	struct mutex lock;
	queue_head_t queue;
	int count;
} dev_number_hash_table[NDEVHASH];

struct dev_entry {
	queue_chain_t	chain;
	char *		object;	/* anything */
	xdev_t		dev;
};

zone_t	dev_entry_zone;

void dev_utils_init()
{
	register int	i;

	for (i = 0; i < NDEVHASH; i++) {
	    mutex_init(&dev_number_hash_table[i].lock);
	    queue_init(&dev_number_hash_table[i].queue);
	    dev_number_hash_table[i].count = 0;
	}

	dev_entry_zone =
		zinit((vm_size_t)sizeof(struct dev_entry),
		      (vm_size_t)sizeof(struct dev_entry)
			* 4096,
		      vm_page_size,
		      FALSE,	/* must be wired because inode_pager
				   uses these structures */
		      "device to device_request port");
}

/*
 * Enter a device in the devnumber hash table.
 */
void dev_number_hash_enter(dev, object)
	xdev_t	dev;
	char *	object;
{
	register struct dev_entry *de;
	register struct dev_number_hash *b;

	de = (struct dev_entry *)zalloc(dev_entry_zone);
	de->dev = dev;
	de->object = object;

	b = &dev_number_hash_table[DEV_NUMBER_HASH(dev)];
	mutex_lock(&b->lock);
	enqueue_tail(&b->queue, (queue_entry_t)de);
	b->count++;
	mutex_unlock(&b->lock);
}

/*
 * Remove a device from the devnumber hash table.
 */
void dev_number_hash_remove(dev)
	xdev_t	dev;
{
	register struct dev_entry *de;
	register struct dev_number_hash *b;

	b = &dev_number_hash_table[DEV_NUMBER_HASH(dev)];

	mutex_lock(&b->lock);

	for (de = (struct dev_entry *)queue_first(&b->queue);
	     !queue_end(&b->queue, (queue_entry_t)de);
	     de = (struct dev_entry *)queue_next(&de->chain)) {
	    if (de->dev == dev) {
		queue_remove(&b->queue, de, struct dev_entry *, chain);
		b->count--;
		zfree(dev_entry_zone, (vm_offset_t)de);
		break;
	    }
	}

	mutex_unlock(&b->lock);
}

/*
 * Map a device to an object.
 */
char *
dev_number_hash_lookup(dev)
	xdev_t	dev;
{
	register struct dev_entry *de;
	register struct dev_number_hash *b;
	char *	object = 0;

	b = &dev_number_hash_table[DEV_NUMBER_HASH(dev)];
	mutex_lock(&b->lock);

	for (de = (struct dev_entry *)queue_first(&b->queue);
	     !queue_end(&b->queue, (queue_entry_t)de);
	     de = (struct dev_entry *)queue_next(&de->chain)) {
	    if (de->dev == dev) {
		object = de->object;
		break;
	    }
	}

	mutex_unlock(&b->lock);
	return (object);
}

/*
 * Map kernel device error codes to BSD error numbers.
 */
int
dev_error_to_errno(err)
	int	err;
{
	switch (err) {
	    case D_SUCCESS:
		return (0);

	    case D_IO_ERROR:
		return (EIO);

	    case D_WOULD_BLOCK:
		return (EWOULDBLOCK);

	    case D_NO_SUCH_DEVICE:
		return (ENXIO);

	    case D_ALREADY_OPEN:
		return (EBUSY);

	    case D_DEVICE_DOWN:
		return (ENETDOWN);

	    case D_INVALID_OPERATION:
		return (ENOTTY);    /* weird, but ioctl() callers expect it */

	    case D_INVALID_RECNUM:
	    case D_INVALID_SIZE:
		return (EINVAL);

	    default:
		return (EIO);
	}
	/*NOTREACHED*/
}
