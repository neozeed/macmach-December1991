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


/* SCSI tape device
 *
 * Created by Zonnie L. Williamson at CMU, 1991
 */

#include <mach/mach_types.h>

#include <device/device_types.h>
#include <device/io_req.h>

#include <kern/queue.h>

#include <mac2os/Types.h>
#include <mac2os/SCSI.h>

#include <mac2dev/stape.h>
#include <mac2dev/mscsi.h>

#define HASH_SIZE	8
#define HASH(n)		((n) & (HASH_SIZE - 1))

/* Assume one tape per SCSI controller. */
typedef struct tape {
} *tape_t;

static struct tape tape_info[SCSI_NCTLR];

/*
 * Called during machine initialization.
 */
void
stape_initialize()
{
    register	i, j;

    for (i = 0; i < SCSI_NCTLR; i++)
	for (j = 0; j < HASH_SIZE; j++)
	    queue_init(&tape_info[i].parts[j]);
}

/*
 * The following routines perform SCSI operations.
 * They will block until the operation is complete.
 */

static inline io_return_t tape_test_unit_rdy(ior)
register io_req_t ior;
{
  io_return_t result;

  ior->io_op = IO_OPEN | IO_WANTED;
  ior->io_mode = 0;
  ior->io_alloc_size = 0;

  if ((result = scsi_op(ior)) == D_IO_QUEUED)
    result = scsi_wait(ior);
  return result;
}

static inline io_return_t tape_read(ior, recnum, count, buffer)
register io_req_t ior;
unsigned recnum;
unsigned count;
unsigned char buffer[];
{
  io_return_t result;

  ior->io_op = IO_READ | IO_WANTED;
  ior->io_mode = 0;
  ior->io_recnum = recnum;
  ior->io_data = (io_buf_ptr_t)(buffer);
  ior->io_count = count << SCSI_NBBLKLOG2;
  ior->io_alloc_size = 0;

  if ((result = scsi_op(ior)) == D_IO_QUEUED)
    result = scsi_wait(ior);
  return result;

}

static inline io_return_t tape_write(ior, recnum, count, buffer)
register io_req_t ior;
unsigned recnum;
unsigned count;
unsigned char buffer[];
{
  io_return_t result;

  ior->io_op = IO_WRITE | IO_WANTED;
  ior->io_mode = 0;
  ior->io_recnum = recnum;
  ior->io_data = (io_buf_ptr_t)(buffer);
  ior->io_count = count << SCSI_NBBLKLOG2;
  ior->io_alloc_size = 0;

  if ((result = scsi_op(ior)) == D_IO_QUEUED)
    result = scsi_wait(ior);
  return result;

}

/*
 * Block the thread if the tape
 * is busy and return whether we
 * blocked or not.
 */
static inline
boolean_t
tape_busy(tape)
register tape_t	tape;
{
    if (!tape->busy)
	return (FALSE);

    assert_wait(&tape->busy, FALSE);
    thread_block((void (*)()) 0);

    return (TRUE);
}

/*
 * MACH device routines.
 */
io_return_t stape_open(int number, dev_mode_t mode, io_req_t ior)
{
  register tape_t tape;

  if ((number < 0) || (number >= SCSI_NCTLR) return (D_NO_SUCH_DEVICE);

  tape = &tape_info[number];

  io_req_alloc(ior, 0);
  ior->io_device = 0;
  ior->io_unit = number;

  if (tape_test_unit_rdy(ior)) {
    io_req_free(ior);
    return D_NO_SUCH_DEVICE;
  }
  io_req_free(ior);
  return D_SUCCESS;

}

io_return_t stape_close(number)
int number;
{
  register tape_t		tape = &tape_info[number];

  return D_SUCCESS
}

#define SCSI_MAXBLK	(SCSI_MAXPHYS >> SCSI_NBBLKLOG2)

io_return_t
sd_read(number, ior)
int			number;
register io_req_t	ior;
{
    register tape_id_t		dev = (tape_id_t)&number;
    register tape_part_t	part;
    register kern_return_t	result;
    register unsigned		offset, length;

    part = tape_part_lookup(dev);
    if (part == tape_PART_NOT_FOUND || part == tape_PART_DOES_NOT_EXIST)
	return (D_NO_SUCH_DEVICE);	/* CAN'T HAPPEN */

    /*
     * The SCSI layer's notion of
     * 'unit' is different that ours.
     */
    ior->io_unit = dev->tape;

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
     * tape transfers can be handled with
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
	    if (tape_read(r, offset, SCSI_MAXBLK, data)) {
		ior->io_residual = (length << SD_REC_SIZE_LOG2);
		io_req_free(r);
		return (D_SUCCESS);
	    }

	    length -= SCSI_MAXBLK;
	    offset += SCSI_MAXBLK;
	    data += SCSI_MAXPHYS;
	}

	if (length > 0)
	    if (tape_read(r, offset, length, data)) {
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
    register tape_id_t		dev = (tape_id_t)&number;
    register tape_part_t	part;
    register kern_return_t	result;
    register unsigned		offset, length;
    boolean_t			wait;

    part = tape_part_lookup(dev);
    if (part == tape_PART_NOT_FOUND || part == tape_PART_DOES_NOT_EXIST)
	return (D_NO_SUCH_DEVICE);

    ior->io_unit = dev->tape;

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
	result = device_write_get(ior, wait);
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
	    if (tape_write(r, offset, SCSI_MAXBLK, data)) {
		ior->io_residual = (length << SD_REC_SIZE_LOG2);
		io_req_free(r);
		return (D_SUCCESS);
	    }

	    length -= SCSI_MAXBLK;
	    offset += SCSI_MAXBLK;
	    data += SCSI_MAXPHYS;
	}

	if (length > 0)
	    if (tape_write(r, offset, length, data)) {
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
    register tape_id_t		dev = (tape_id_t)&number;
    register tape_part_t	part;

    part = tape_part_lookup(dev);
    if (part == tape_PART_NOT_FOUND || part == tape_PART_DOES_NOT_EXIST)
	return (D_NO_SUCH_DEVICE);

    switch (flavor) {
      case SD_PART_INFO:
	{
	    register sd_part_info_t	*info = (sd_part_info_t *)status;

	    info->parts = tape_info[dev->tape].nparts;
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
