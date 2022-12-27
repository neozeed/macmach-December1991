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

/* MacMach driver for ramdisk
 *
 * Create by Zonnie L. Williamson at CMU, 1991
 *
 */

#include <mach/mach_types.h>

#include <device/device_types.h>
#include <device/io_req.h>

#define RD_REC_SIZE 512

/* these are set in locore.s from the kernel arguments */
extern char *ramdisk_image;
extern int ramdisk_size;

static int ramdisk_mode;

io_return_t rd_open(int number, dev_mode_t mode, io_req_t ior)
{

  if (number) return D_NO_SUCH_DEVICE;

  if (ramdisk_size <= 0) return D_NO_SUCH_DEVICE;

  ramdisk_mode = mode;

} /* rd_open() */

io_return_t rd_close(int number)
{

  ramdisk_mode = 0;
  return D_SUCCESS;

} /* rd_close() */

io_return_t rd_read(int number, io_req_t ior)
{
  kern_return_t result;
  int offset;

  if (!(ramdisk_mode & D_READ)) return D_IO_ERROR;

  if (ior->io_count % RD_REC_SIZE) return D_INVALID_SIZE;

  offset = ior->io_recnum * RD_REC_SIZE;
  if ((offset + ior->io_count) >= ramdisk_size)
    return D_INVALID_RECNUM;

  result = device_read_alloc(ior, (vm_size_t)ior->io_count);
  if (result != KERN_SUCCESS) return result;

  bcopy(ramdisk_image[offset], ior->io_data, ior->io_count);

  return D_SUCCESS;

} /* rd_read() */

io_return_t rd_write(int number, io_req_t ior)
{
  kern_return_t result;
  int offset;
  boolean_t wait;

  if (!(ramdisk_mode & D_WRITE)) return D_READ_ONLY;

  if (ior->io_count % RD_REC_SIZE) return D_INVALID_SIZE;

  offset = ior->io_recnum * RD_REC_SIZE;
  if ((offset + ior->io_count) >= ramdisk_size)
    return D_INVALID_RECNUM;

  if ((ior->io_op & IO_INBAND) == 0) {
    result = device_write_get(ior, &wait);
    if (result != KERN_SUCCESS)
      return result;
  }

  bcopy(ior->io_data, ramdisk_image[offset], ior->io_count);

  return D_SUCCESS;

} /* rd_write() */
