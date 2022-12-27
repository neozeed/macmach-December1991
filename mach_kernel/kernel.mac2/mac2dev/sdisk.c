/* 
 * Mach Operating System
 * Copyright (c) 1991,1990,1989 Carnegie Mellon University
 * All Rights Reserved.
 * 
 * Permission to use, copy, modify and distribute this software and its
 * documentation is hereby granted, provided that both the copyright
 * notice and this permission notice appear in all copies of the
 * software, derivative works or modified versions, and any portions
 * thereof, and that both notices appear in supporting documentation.
 * 
 * CARNEGIE MELLON ALLOWS FREE USE OF THIS SOFTWARE IN ITS "AS IS"
 * CONDITION.  CARNEGIE MELLON DISCLAIMS ANY LIABILITY OF ANY KIND FOR
 * ANY DAMAGES WHATSOEVER RESULTING FROM THE USE OF THIS SOFTWARE.
 * 
 * Carnegie Mellon requests users of this software to return to
 * 
 *  Software Distribution Coordinator  or  Software.Distribution@CS.CMU.EDU
 *  School of Computer Science
 *  Carnegie Mellon University
 *  Pittsburgh PA 15213-3890
 * 
 * any improvements or extensions that they make and grant Carnegie Mellon
 * the rights to redistribute these changes.
 */


/*
 * HISTORY
 * $Log:	sdisk.c,v $
 * Revision 2.2  91/09/12  16:47:29  bohman
 * 	Created.
 * 	[91/09/11  16:01:41  bohman]
 * 
 */

/*
 *	Apple Macintosh II Mach (macmach)
 *
 *	File: mac2dev/sdisk.c
 *	Author: David E. Bohman II (CMU macmach)
 */

#include <mach/mach_types.h>

#include <device/device_types.h>
#include <device/io_req.h>

#include <kern/queue.h>

#include <mac2os/Types.h>
#include <mac2os/SCSI.h>

#include <mac2dev/sdisk.h>
#include <mac2dev/mscsi.h>

/*
 * Macintosh SCSI disk driver
 * for MACH 3.0
 */

#define HASH_SIZE	8
#define HASH(n)		((n) & (HASH_SIZE - 1))

/*
 * An open disk partition.
 *	part 0 is the entire disk
 *	parts 1 - 31 are apple partition N
 */
typedef struct {
    queue_chain_t	link;
    unsigned short	part;
    unsigned		offset;		/* in blocks */
    unsigned		length;		/* in blocks */
    unsigned char	name[32];
    unsigned char	type[32];
} *disk_part_t;

#define DISK_PART_NOT_FOUND		((disk_part_t) 0)
#define DISK_PART_DOES_NOT_EXIST	((disk_part_t) -1)

/*
 * A disk device.  Assume one 'disk'
 * per SCSI controller.
 */
typedef struct disk {
    boolean_t		valid;		/* drive contains valid Block 0 */
    boolean_t		busy;		/* drive is currently being opened */
    short		nparts;		/* num of partitions from map */
    short		oparts;		/* num of open partitions */
    queue_head_t	parts[HASH_SIZE];
} *disk_t;

static struct disk	disk_info[SCSI_NCTLR];

/*
 * MACH device number encoding.
 */
typedef struct {
    unsigned		disk:3,
			    :24,
			part:5;
} *disk_id_t;

/*
 * Called during machine initialization.
 */
void
sdisk_initialize()
{
    register	i, j;

    for (i = 0; i < SCSI_NCTLR; i++)
	for (j = 0; j < HASH_SIZE; j++)
	    queue_init(&disk_info[i].parts[j]);
}

/*
 * The following routines manipulate
 * the open partition information.
 */
static inline
disk_part_t
disk_part_lookup(dev)
register disk_id_t	dev;
{
    register queue_t	q = &disk_info[dev->disk].parts[HASH(dev->part)];
    register
	queue_entry_t	qe = queue_first(q);
#define qpart		((disk_part_t) qe)

    if (queue_end(q, qe))
	return (DISK_PART_NOT_FOUND);

    if (dev->part == qpart->part)
	return (qpart);

    if (dev->part > disk_info[dev->disk].nparts)
	return (DISK_PART_DOES_NOT_EXIST);

    for (qe = queue_next(qe); !queue_end(q, qe); qe = queue_next(qe))
	if (dev->part == qpart->part)
	    return (qpart);

    return (DISK_PART_NOT_FOUND);
#undef	qpart
}

static inline
void
disk_part_add(dev, part)
register disk_id_t	dev;
register disk_part_t	part;
{
    register disk_t	disk = &disk_info[dev->disk];

    queue_enter(&disk->parts[HASH(dev->part)],
		part, disk_part_t,
		link);

    disk->oparts++;
}

static inline
void
disk_part_remove(dev, part)
register disk_id_t	dev;
register disk_part_t	part;
{
    register disk_t	disk = &disk_info[dev->disk];

    queue_remove(&disk->parts[HASH(dev->part)],
		part, disk_part_t,
		link);

    disk->oparts--;
}

/*
 * The following routines perform
 * SCSI operations and block until
 * the operation is complete.
 */
static inline
io_return_t
disk_test_unit_rdy(ior)
register io_req_t	ior;
{
    io_return_t		result;

    ior->io_op = IO_OPEN | IO_WANTED;
    ior->io_mode = 0;
    ior->io_alloc_size = 0;

    result = scsi_op(ior);
    if (result == D_IO_QUEUED)
	result = scsi_wait(ior);

    return (result);
}

static inline
io_return_t
disk_read(ior, recnum, count, buffer)
register io_req_t	ior;
unsigned		recnum;
unsigned		count;
unsigned char		buffer[];
{
    io_return_t		result;

    ior->io_op = IO_READ | IO_WANTED;
    ior->io_mode = 0;
    ior->io_recnum = recnum;
    ior->io_data = (io_buf_ptr_t)(buffer);
    ior->io_count = count << SCSI_NBBLKLOG2;
    ior->io_alloc_size = 0;

    result = scsi_op(ior);
    if (result == D_IO_QUEUED)
	result = scsi_wait(ior);

    return (result);
}

static inline
io_return_t
disk_write(ior, recnum, count, buffer)
register io_req_t	ior;
unsigned		recnum;
unsigned		count;
unsigned char		buffer[];
{
    io_return_t		result;

    ior->io_op = IO_WRITE | IO_WANTED;
    ior->io_mode = 0;
    ior->io_recnum = recnum;
    ior->io_data = (io_buf_ptr_t)(buffer);
    ior->io_count = count << SCSI_NBBLKLOG2;
    ior->io_alloc_size = 0;

    result = scsi_op(ior);
    if (result == D_IO_QUEUED)
	result = scsi_wait(ior);

    return (result);
}

/*
 * Block the thread if the disk
 * is busy and return whether we
 * blocked or not.
 */
static inline
boolean_t
disk_busy(disk)
register disk_t	disk;
{
    if (!disk->busy)
	return (FALSE);

    assert_wait(&disk->busy, FALSE);
    thread_block((void (*)()) 0);

    return (TRUE);
}

/*
 * MACH device routines.
 */
io_return_t
sd_open(number, mode, ior)
int		number;
dev_mode_t	mode;
io_req_t	ior;
{
    register disk_id_t		dev = (disk_id_t)&number;
    register disk_t		disk = &disk_info[dev->disk];
    register disk_part_t	part;

    if (dev->disk >= SCSI_NCTLR)
	return (D_NO_SUCH_DEVICE);

    /*
     * Only let one thread complete
     * a 'complex' open per disk at
     * a time.
     */
    do {
	part = disk_part_lookup(dev);
	if (part == DISK_PART_DOES_NOT_EXIST)
	    return (D_NO_SUCH_DEVICE);
    } while (part == DISK_PART_NOT_FOUND && disk_busy(disk));

    if (part == DISK_PART_NOT_FOUND) {
	unsigned char		block[SCSI_NBBLK];
	register Block0		*b0 = (Block0 *)block;
	register Partition	*p = (Partition *)block;
	register io_req_t	ior;
#define RETURN(result)				\
	MACRO_BEGIN				\
	    io_req_free(ior);			\
	    thread_wakeup_one(&disk->busy);	\
	    disk->busy = FALSE;			\
	    return (result);			\
	MACRO_END

	disk->busy = TRUE;

	io_req_alloc(ior, 0);
	ior->io_device = 0;
	ior->io_unit = dev->disk;

	if (!disk->valid) {
	    if (disk_test_unit_rdy(ior))
		RETURN (D_NO_SUCH_DEVICE);
	}

	if (!disk->valid || dev->part == 0) {
	    if (disk_read(ior, 0, 1, block))
		RETURN (D_NO_SUCH_DEVICE);
	}

	if (dev->part == 0) {
	    /*
	     * Make a dummy partition
	     * that refers to the entire
	     * disk.
	     */
	    part = (disk_part_t)kalloc(sizeof (*part));
	    if (!part)
		RETURN (D_NO_MEMORY);

	    part->part = 0;
	    part->offset = 0;
	    part->type[0] = part->name[0] = 0;

	    if (b0->sbSig != sbSIGWord)
		part->length = 0xffffffff;
	    else {
		part->length = b0->sbBlkCount;

		disk->valid = TRUE;
		if (disk->nparts == 0
		    	&& !disk_read(ior, 1, 1, block)) {
		    if (p->pmSig == pMapSIG)
			disk->nparts = p->pmMapBlkCnt;
		}
	    }
	}
	else {
	    if (!disk->valid && b0->sbSig != sbSIGWord)
		RETURN (D_NO_SUCH_DEVICE);

	    if (disk_read(ior, 1, 1, block))
		RETURN (D_NO_SUCH_DEVICE);

	    if (p->pmSig != pMapSIG)
		RETURN (D_NO_SUCH_DEVICE);

	    if (dev->part > p->pmMapBlkCnt)
		RETURN (D_NO_SUCH_DEVICE);

	    if (dev->part != 1) {
		if (disk_read(ior, dev->part, 1, block))
		    RETURN (D_NO_SUCH_DEVICE);

		if (p->pmSig != pMapSIG)
		    RETURN (D_NO_SUCH_DEVICE);
	    }

	    part = (disk_part_t)kalloc(sizeof (*part));
	    if (!part)
		RETURN (D_NO_MEMORY);

	    part->part = dev->part;
	    part->offset = p->pmPyPartStart;
	    part->length = p->pmPartBlkCnt;
	    bcopy(p->pmPartName, part->name, sizeof (part->name));
	    bcopy(p->pmPartType, part->type, sizeof (part->type));

	    disk->valid = TRUE;
	    if (disk->nparts == 0)
		disk->nparts = p->pmMapBlkCnt;
	}

	disk_part_add(dev, part);

	RETURN (D_SUCCESS);
#undef RETURN
    }

    return (D_SUCCESS);
}

io_return_t
sd_close(number)
int	number;
{
    register disk_id_t		dev = (disk_id_t)&number;
    register disk_t		disk = &disk_info[dev->disk];
    register disk_part_t	part;

    if (dev->disk >= SCSI_NCTLR)
	return (D_NO_SUCH_DEVICE);

    part = disk_part_lookup(dev);
    if (part == DISK_PART_NOT_FOUND || part == DISK_PART_DOES_NOT_EXIST)
	return (D_NO_SUCH_DEVICE);	/* CAN'T HAPPEN */
    
    disk_part_remove(dev, part);
    
    kfree(part, sizeof (*part));
    
    if (disk->oparts == 0) {
	disk->valid = FALSE;
	disk->nparts = 0;
    }
    else if (disk->oparts < 0)
	panic("sd close");

    return (D_SUCCESS);
}

#define SCSI_MAXBLK	(SCSI_MAXPHYS >> SCSI_NBBLKLOG2)

io_return_t
sd_read(number, ior)
int			number;
register io_req_t	ior;
{
    register disk_id_t		dev = (disk_id_t)&number;
    register disk_part_t	part;
    register kern_return_t	result;
    register unsigned		offset, length;

    part = disk_part_lookup(dev);
    if (part == DISK_PART_NOT_FOUND || part == DISK_PART_DOES_NOT_EXIST)
	return (D_NO_SUCH_DEVICE);	/* CAN'T HAPPEN */

    /*
     * The SCSI layer's notion of
     * 'unit' is different that ours.
     */
    ior->io_unit = dev->disk;

    /*
     * Check for bogus arguments.
     */
    if (ior->io_recnum >= part->length)
	return (D_INVALID_RECNUM);

    if (ior->io_count % SD_REC_SIZE)
	return (D_INVALID_SIZE);

    offset = ior->io_recnum;
    length = (ior->io_count >> SD_REC_SIZE_LOG2);

    /*
     * Adjust the request to fit within
     * the partition, if necessary.
     */
    if ((offset + length) > part->length) {
	length = part->length - offset;
	ior->io_count = (length << SD_REC_SIZE_LOG2);
    }

    /*
     * Allocate space for the request.
     */
    result = device_read_alloc(ior, (vm_size_t)ior->io_count);
    if (result != KERN_SUCCESS)
	return (result);

    /*
     * Change offset into an absolute
     * quantity.
     */
    offset += part->offset;

    /*
     * The SCSI layer uses a fixed-sized
     * intermediate buffer for the data
     * so that the kernel will run on a
     * machine that is not 32 bit clean.
     * We allow arbitrarily large requests
     * to be handled here so that large
     * disk transfers can be handled with
     * a minimum of overhead wrt RPC calls
     * and vm region creation.  Please note
     * that a thread which is blocked at this
     * time will not allow a stack handoff to
     * occur.
     */
    if (length > SCSI_MAXBLK) {
	register io_buf_ptr_t	data = ior->io_data;
	register io_req_t	r;

	io_req_alloc(r, 0);
	r->io_device = 0;
	r->io_unit = ior->io_unit;

	while (length > SCSI_MAXBLK) {
	    if (disk_read(r, offset, SCSI_MAXBLK, data)) {
		ior->io_residual = (length << SD_REC_SIZE_LOG2);
		io_req_free(r);
		return (D_SUCCESS);
	    }

	    length -= SCSI_MAXBLK;
	    offset += SCSI_MAXBLK;
	    data += SCSI_MAXPHYS;
	}

	if (length > 0)
	    if (disk_read(r, offset, length, data)) {
		ior->io_residual = (length << SD_REC_SIZE_LOG2);
		io_req_free(r);
		return (D_SUCCESS);
	    }

	io_req_free(r);
    }
    else {
	/*
	 * Requests which will fit into the
	 * SCSI buffer can fully exploit the
	 * MACH asynchronous device features,
	 * including the stack handoff facility.
	 */
	ior->io_recnum = offset;
	return (scsi_op(ior));
    }

    return (D_SUCCESS);
}

io_return_t
sd_write(number, ior)
int			number;
register io_req_t	ior;
{
    register disk_id_t		dev = (disk_id_t)&number;
    register disk_part_t	part;
    register kern_return_t	result;
    register unsigned		offset, length;
    boolean_t			wait;

    part = disk_part_lookup(dev);
    if (part == DISK_PART_NOT_FOUND || part == DISK_PART_DOES_NOT_EXIST)
	return (D_NO_SUCH_DEVICE);

    ior->io_unit = dev->disk;

    if (ior->io_recnum >= part->length)
	return (D_INVALID_RECNUM);

    if (ior->io_count % SD_REC_SIZE)
	return (D_INVALID_SIZE);

    offset = ior->io_recnum;
    length = (ior->io_count >> SD_REC_SIZE_LOG2);

    if ((offset + length) > part->length) {
	length = part->length - offset;
	ior->io_count = (length << SD_REC_SIZE_LOG2);
    }

    if ((ior->io_op & IO_INBAND) == 0) {
	result = device_write_get(ior, &wait);
	if (result != KERN_SUCCESS)
	    return (result);
    }

    offset += part->offset;

    if (length > SCSI_MAXBLK) {
	register io_buf_ptr_t	data = ior->io_data;
	register io_req_t	r;

	io_req_alloc(r, 0);
	r->io_device = 0;
	r->io_unit = ior->io_unit;

	while (length > SCSI_MAXBLK) {
	    if (disk_write(r, offset, SCSI_MAXBLK, data)) {
		ior->io_residual = (length << SD_REC_SIZE_LOG2);
		io_req_free(r);
		return (D_SUCCESS);
	    }

	    length -= SCSI_MAXBLK;
	    offset += SCSI_MAXBLK;
	    data += SCSI_MAXPHYS;
	}

	if (length > 0)
	    if (disk_write(r, offset, length, data)) {
		ior->io_residual = (length << SD_REC_SIZE_LOG2);
		io_req_free(r);
		return (D_SUCCESS);
	    }

	io_req_free(r);
    }
    else {
	ior->io_recnum = offset;
	if (!wait)
	    return (scsi_op(ior));
	else {
	    result = scsi_op(ior);
	    if (result == D_IO_QUEUED)
		scsi_wait(ior);

	    return (D_SUCCESS);
	}
    }

    return (D_SUCCESS);
}

io_return_t
sd_getstat(number, flavor, status, status_count)
int		number, flavor;
dev_status_t	status;
unsigned	*status_count;
{
    register disk_id_t		dev = (disk_id_t)&number;
    register disk_part_t	part;

    part = disk_part_lookup(dev);
    if (part == DISK_PART_NOT_FOUND || part == DISK_PART_DOES_NOT_EXIST)
	return (D_NO_SUCH_DEVICE);

    switch (flavor) {
      case SD_PART_INFO:
	{
	    register sd_part_info_t	*info = (sd_part_info_t *)status;

	    info->parts = disk_info[dev->disk].nparts;
	    info->length = part->length;
	    bcopy(part->name, info->name, sizeof (info->name));
	    bcopy(part->type, info->type, sizeof (info->type));
	    *status_count = SD_PART_INFO_COUNT;
	}
	break;

      default:
	return (D_INVALID_OPERATION);
    }

    return (D_SUCCESS);
}
