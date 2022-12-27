#undef DEBUG

/* disk I/O for mac2 using MACH sdisk interface.               zw 7/17/91
 *
 * There are three flavors of access to Apple disks via sdisk on a mac2:
 *   1) char access to the entire Apple disk
 *   2) char access to Unix partitions withing the "Mach" Apple partition
 *   3) block access to Unix partitions withing the "Mach" Apple partition
 *
 * The sdisk interface encodes Apple partition references in the
 * device name as "sd#$" where 'sd' signifies sdisk, '#' is the SCSI
 * address of the disk and '$' is a letter representing an Apple partition.
 * Partition 'a' is the entire Apple disk.  Partition 'b' is the first
 * actual Apple partition.
 *
 * apple_open() initiates access to the entire Apple disk.  It simply
 * composes a device name and does a device_open().  The apple_device_port[]
 * array holds device ports for all Apple disks currently open.  apple_close()
 * does a device_close followed by a mach_port_deallocate().  The apple_read()
 * and apple_write() routines allow full access to the apple disk, including
 * the ability to create, edit, trample and destroy the Apple partition map.
 *
 * sdisk_open() and sdisk_close() initiate and terminate access to Unix
 * partitions within "Mach" Apple partitions.  An Apple disk should have only
 * one "Mach" Apple partition, if it has more, only the first will be used.
 * At block LABELSECTOR in the "Mach" Apple partition is the standard Unix
 * disklabel.  The sdisk[] array device ports and disklabels for all disks
 * currently open.  A reference count is used to keep track of multiple calls
 * to sdisk_open() as more than one Unix partition can exist in the same
 * Apple partition.
 *
 * The first thing sdisk_open() does is call sdisk_device_name() to compose
 * the device name.  sdisk_device_name() searches all Apple partitions on the
 * disk for the "Mach" Apple partition.  Note that sdisk_open() is used for
 * both char and block access, but it extracts the disk number from the device
 * minor using the flags from bdevsw[].
 *
 * sdisk_read() and sdisk_write() provide char I/O to Unix partitions.  The
 * sdisk_strategy() routine does the block I/O.  Note that the disklabel is
 * usually read and written via sdisk_ioctl().  Other write access to the
 * disklabel must be explicitly enabled by a call to sdisk_ioctl().
 *
 */

#define WRONG

#ifdef WRONG
 /* silently ignore all non-ioctl requests to write LABELSECTOR */
#endif

#include <sys/param.h>
#include <sys/types.h>
#include <sys/errno.h>
#include <sys/ioctl.h>
#include <sys/file.h>
#include <sys/buf.h>
#include <sys/uio.h>
#include <sys/conf.h>
#include <sys/disklabel.h>
#include <mac2dev/sdisk.h>
#include <uxkern/import_mach.h>
#include <uxkern/device_reply_hdlr.h>
#include <uxkern/device_utils.h>
#include <device/device.h>

/* this is the "Mach" Apple partition type */
#define MACH_PARTITION_TYPE "Mach_UNIX_BSD4.3"

/* this is the number of sdisk disks supported */
/* these translate directly into SCSI addresses, and address 7 is the cpu */
#define NSD 7

/* device ports for direct access to Apple disks (partition 'a') */
static mach_port_t apple_device_port[NSD];

/* sdisk information per disk */
static struct sdisk {
  mach_port_t      device_port;	    /* single device port for disk I/O */
  int              reference_count; /* if non-zero, some partition is open */
  int              wlabel;          /* if non-zero, allow LABELSECTOR write */
  struct disklabel disklabel;       /* the disklabel */
} sdisk[NSD];

/* return the first disklabel magic number */
#define disklabel_magic(disk) (sdisk[disk].disklabel.d_magic)

/* return the second disklabel magic number */
#define disklabel_magic2(disk) (sdisk[disk].disklabel.d_magic2)

/* return the sector size for a disk */
#define disk_secsize(disk) (sdisk[disk].disklabel.d_secsize)

/* return the size of a partition on a disk */
#define partition_size(disk, partition) \
  (sdisk[disk].disklabel.d_partitions[partition].p_size)

/* return the block offset for a partition on a disk */
#define partition_offset(disk, partition) \
  (sdisk[disk].disklabel.d_partitions[partition].p_offset)

/* extract disk number from device number */
#define dev_disk(dev, d_flags) (minor(dev) / C_BLOCK_GET(d_flags))

/* extract disk number from block device number */
#define bdev_disk(dev) (dev_disk(dev, bdevsw[major(dev)].d_flags))

/* extract disk number from char device number */
#define cdev_disk(dev) (dev_disk(dev, cdevsw[major(dev)].d_flags))

/* extract partition number from device number */
#define dev_partition(dev, d_flags) (minor(dev) % C_BLOCK_GET(d_flags))

/* extract partition number from block device number */
#define bdev_partition(dev) (dev_partition(dev, bdevsw[major(dev)].d_flags))

/* extract partition number from char device number */
#define cdev_partition(dev) (dev_partition(dev, cdevsw[major(dev)].d_flags))

/* generate sdisk device name from disk number */
/* side effect: sets sdisk[disk].device_port */
/* return non-zero error code if error */
static inline int sdisk_device_name(int disk, char *name)
{
  char *partition, max_partition;
  mach_port_t device_port;
  mach_port_t previous_port;
  sd_part_info_t status;
  mach_msg_type_number_t status_count;
  kern_return_t rc;

  /* note that the "partitions" referenced here are Apple partitions */

#ifdef DEBUG
  printf("sdisk_device_name: disk = %d\n", disk);
#endif

  /* disk number (SCSI address) must be one of 0, 1, 2, 3, 4, 5, 6 */
  /* SCSI address 7 is the cpu */
  if ((disk < 0) || (disk > 6)) return ENXIO;

  /* compose name */
  name[0] = 's';
  name[1] = 'd';
  name[2] = '0' + disk;
  partition = &name[3];
  name[4] = 0;

  /* open Apple partition 'b', get status to find number of partitions */
  *partition = 'b';
  rc = device_open(device_server_port,
                   D_READ,
                   name,
                   &device_port);
  if (rc != D_SUCCESS) goto error;
  status_count = (sizeof(status) + sizeof(int) - 1) / sizeof(int);
  rc = device_get_status(device_port,
                         SD_PART_INFO,
                         (dev_status_t)&status,
                         &status_count);
  if (rc != D_SUCCESS) goto error;

  max_partition = 'a' + status.parts;

#ifdef DEBUG
  printf("sdisk_device_name: max_partition = '%c'\n", max_partition);
#endif

  /* set up previous port, always want to keep at least one port open */
  previous_port = device_port;
  device_port = MACH_PORT_NULL;

  /* search for "Mach" Apple partition */
  for (*partition = 'b'; *partition <= max_partition; (*partition)++) {
#ifdef DEBUG
    printf("sdisk_device_name: trying \"%s\"\n", name);
#endif
    rc = device_open(device_server_port,
                     D_READ | D_WRITE,
                     name,
                     &device_port);
    if (rc != D_SUCCESS) goto error;
    rc = device_close(previous_port);
    (void)mach_port_deallocate(mach_task_self(), previous_port);
    previous_port = MACH_PORT_NULL;
    if (rc != D_SUCCESS) goto error;
    status_count = (sizeof(status) + sizeof(int) - 1) / sizeof(int);
    rc = device_get_status(device_port,
                           SD_PART_INFO,
                           (dev_status_t)&status,
                           &status_count);
    if (rc != D_SUCCESS) goto error;
#ifdef DEBUG
    printf("sdisk_device_name: type = \"%s\"\n", status.type);
#endif
    if (!strcmp(status.type, MACH_PARTITION_TYPE)) break;
    previous_port = device_port;
    device_port = MACH_PORT_NULL;
  }

  /* all done with the previous port */
  if (previous_port != MACH_PORT_NULL) {
    (void)device_close(previous_port);
    (void)mach_port_deallocate(mach_task_self(), previous_port);
  }

  /* error if no "Mach" Apple partition */
  if (device_port == MACH_PORT_NULL) return ENXIO;

  /* "Mach" Apple partition found, keep port open */
  sdisk[disk].device_port = device_port;

#ifdef DEBUG
  printf("sdisk_device_name: name = \"%s\"\n", name);
#endif

  /* all done, name string contains disk name, indicate success */
  return 0;

  /* if any device error, close and deallocate the ports */
error:
  if (previous_port != MACH_PORT_NULL) {
    (void)device_close(previous_port);
    (void)mach_port_deallocate(mach_task_self(), previous_port);
  }
  if (device_port != MACH_PORT_NULL) {
    (void)device_close(device_port);
    (void)mach_port_deallocate(mach_task_self(), device_port);
  }
  return dev_error_to_errno(rc);

} /* sdisk_device_name() */

/* close block or char sdisk device, return non-zero error code if error */
static int sdisk_close(dev_t dev, int flag, int d_flags, int in_sdisk_open)
{
  int disk;
  mach_port_t port;
  int error;

  /* extract disk number from device number */
  disk = dev_disk(dev, d_flags);

#ifdef DEBUG
  printf("sdisk_close: disk = %d, reference count = %d\n",
    disk, sdisk[disk].reference_count);
#endif

  /* panic if not open */
  if (!sdisk[disk].reference_count) {
#ifdef DEBUG
    printf("sdisk_close: not open\n");
    return ENXIO;
#else
    panic("sdisk_close: not open");
#endif
  }

  /* decrement reference count, do nothing if still referenced */
  if (--sdisk[disk].reference_count) return 0;

  /* panic if no port (ok if in sdisk_open) */
  if (sdisk[disk].device_port == MACH_PORT_NULL) {
    if (in_sdisk_open) return 0;
#ifdef DEBUG
    printf("sdisk_close: null port\n");
    return ENXIO;
#else
    panic("sdisk_close: null port");
#endif
  }

#ifdef DEBUG
  printf("sdisk_close: device_close()\n");
#endif

  /* close device and deallocate port */
  error = dev_error_to_errno(device_close(sdisk[disk].device_port));
  (void)mach_port_deallocate(mach_task_self(), sdisk[disk].device_port);
  sdisk[disk].device_port = MACH_PORT_NULL;

  /* return error code from device_close() */
  return error;

} /* sdisk_close() */

/* close block sdisk device, return non-zero error code if error */
int sdisk_bclose(dev_t dev, int flag)
{
  return sdisk_close(dev, flag, bdevsw[major(dev)].d_flags, 0);
}

/* close char sdisk device, return non-zero error code if error */
int sdisk_cclose(dev_t dev, int flag)
{
  return sdisk_close(dev, flag, cdevsw[major(dev)].d_flags, 0);
}

/* open block or char sdisk device, return non-zero error code if error */
static int sdisk_open(dev_t dev, int flag, int d_flags)
{
  int disk;
  char name[32];
  kern_return_t rc;
  int error;
  io_buf_ptr_t data;
  mach_msg_type_number_t count;

  /* note that the flag value is ignored */

  /* extract disk number from device number */
  /* this uses the block version for char access too */
  disk = dev_disk(dev, d_flags);

#ifdef DEBUG
  printf("sdisk_open: disk = %d, reference count = %d\n",
    disk, sdisk[disk].reference_count);
#endif

  /* increment reference count, do nothing if previously referenced */
  if (sdisk[disk].reference_count++) return 0;

  /* open sdisk device on first reference */

  /* compose sdisk device name */
  error = sdisk_device_name(disk, name);

  /* open sdisk device */
  if (!error && (sdisk[disk].device_port == MACH_PORT_NULL)) {
    rc = device_open(device_server_port,
                     D_READ | D_WRITE,
                     name,
                     &sdisk[disk].device_port);
    if (rc != D_SUCCESS) error = dev_error_to_errno(rc);
  }

  /* read disklabel from LABELSECTOR */
  if (!error) {
#ifdef DEBUG
    printf("sdisk_open: reading disklabel\n");
#endif
    rc = device_read(sdisk[disk].device_port,
                     0, /* ignored by sdisk driver */
                     LABELSECTOR,
                     SD_REC_SIZE,
                     &data,
                     &count);
    if (rc != D_SUCCESS) error = dev_error_to_errno(rc);
  }

  /* copy disklabel into sdisk[] array, deallocate data from device_read */
  if (!error) {
    if (count < sizeof(struct disklabel)) error = ENXIO;
    else bcopy(&data[LABELOFFSET],
               &sdisk[disk].disklabel,
               sizeof(struct disklabel));
    (void)vm_deallocate(mach_task_self(), data, count);
  }

  /* if error, do sdisk_close to clean up loose ends */
  if (error) {
    (void)sdisk_close(dev, flag, d_flags, 1);
    return error;
  }

#ifdef DEBUG
  printf("sdisk_open: disklabel magic = 0x%X, magic2 = 0x%X\n",
    disklabel_magic(disk), disklabel_magic2(disk));
#endif

  /* Access to sdisk device has been established.
   * Note that we do not look at the contents of the disklabel at this time.
   * This allows the /etc/disklabel utility to open a new disk in order to
   * write the first disklabel with the DIOCWDINFO ioctl.
   */
  return 0;
  
} /* sdisk_open() */

/* open block sdisk device, return non-zero error code if error */
int sdisk_bopen(dev_t dev, int flag)
{
  return sdisk_open(dev, flag, bdevsw[major(dev)].d_flags);
}

/* open char sdisk device, return non-zero error code if error */
int sdisk_copen(dev_t dev, int flag)
{
  return sdisk_open(dev, flag, cdevsw[major(dev)].d_flags);
}

/* sdisk block I/O strategy */
/* Any errors generated here are noted in the buff structure. */
void sdisk_strategy(struct buf *bp)
{
  int disk, partition;
  recnum_t recnum;
  mach_msg_type_number_t count;
  mach_port_t device_port;
  kern_return_t	error;

  /* extract disk number from device number */
  disk = bdev_disk(bp->b_dev);

#ifdef DEBUG
  printf("sdisk_strategy: disk = %d\n", disk);
#endif

  /* check that device port exists */
  if (sdisk[disk].device_port == MACH_PORT_NULL) {
#ifdef DEBUG
    printf("sdisk_strategy null port");
    bp->b_flags |= B_ERROR;
    bp->b_error = ENXIO;
    biodone(bp);
    return;
#else
    panic("sdisk_strategy: null port");
#endif
  }

  /* check magic numbers in disklabel */
  if (disklabel_magic(disk) != DISKMAGIC) {
#ifdef DEBUG
    printf("sdisk_strategy: magic 0x%X != 0x%X\n",
      disklabel_magic(disk), DISKMAGIC);
#endif
    bp->b_flags |= B_ERROR;
    bp->b_error = ENXIO;
    biodone(bp);
    return;
  }
  if (disklabel_magic2(disk) != DISKMAGIC) {
#ifdef DEBUG
    printf("sdisk_strategy: magic2 0x%X != 0x%X\n",
      disklabel_magic(disk), DISKMAGIC);
#endif
    bp->b_flags |= B_ERROR;
    bp->b_error = ENXIO;
    biodone(bp);
    return;
  }

  /* extract partition number from device number */
  partition = bdev_partition(bp->b_dev);

#ifdef DEBUG
  printf("sdisk_strategy: partition = %c\n", partition + 'a');
#endif

  /* check for valid partition access, if error send message to reply port */
  if (((bp->b_blkno * disk_secsize(disk)) + bp->b_bcount) >
      (partition_size(disk, partition) * disk_secsize(disk))) {
#ifdef DEBUG
    printf("sdisk_strategy: offset %d + count %d = %d, > size %d\n",
      bp->b_blkno * disk_secsize(disk), bp->b_bcount,
      partition_size(disk, partition) * disk_secsize(disk));
#endif
    bp->b_flags |= B_ERROR;
    bp->b_error = EINVAL;
    biodone(bp);
    return;
  }

  /* use partiton offset to calculate record number */
  recnum = bp->b_blkno + partition_offset(disk, partition);
  count = bp->b_bcount;

  /* special case for writing LABELSECTOR */
  if (!(bp->b_flags & B_READ) && (recnum <= LABELSECTOR) &&
#if LABELSECTOR != 0
      ((recnum + count / disk_secsize(disk)) > LABELSECTOR) &&
#endif
      sdisk[disk].wlabel == 0) {
#ifdef DEBUG
  printf("sdisk_strategy: attempt to write LABELSECTOR\n");
#endif
#ifdef WRONG
    /* silently ignore attempts to write disklabel */
    recnum++;
    if ((count -= SD_REC_SIZE) < 0) count = 0;
#else
    /* allow disklabel writes only if previously enabled by ioctl */
    bp->b_flags |= B_ERROR;
    bp->b_error = EROFS;
    biodone(bp);
    return;
#endif
  }

  /* Make read or write requests.  The device reply handler thread will
   * read messages on the reply port to complete the block I/O.
   */

  /* make read request, data will be returned in message to reply port */
  if (bp->b_flags & B_READ) {
#ifdef DEBUG
    printf("sdisk_strategy: read %d bytes at block %d\n", count, recnum);
#endif
    error = device_read_request(sdisk[disk].device_port,
                                bp->b_reply_port,
                                0,
                                recnum,
                                count);
    if (error != KERN_SUCCESS) {
#ifdef DEBUG
      printf("sdisk_strategy: read request %d\n", error);
      bp->b_flags |= B_ERROR;
      bp->b_error = ENXIO;
      biodone(bp);
      return;
#else
      panic("sdisk_strategy: read request", error);
#endif
    }
  }

  /* make write request, result will be returned in message to reply port */
  else {
#ifdef DEBUG
    printf("sdisk_strategy: write %d bytes at block %d\n", count, recnum);
#endif
    error = device_write_request(sdisk[disk].device_port,
                                 bp->b_reply_port,
                                 0,
                                 recnum,
                                 bp->b_un.b_addr,
                                 count);
    if (error != KERN_SUCCESS) {
#ifdef DEBUG
      printf("sdisk_strategy: write request %d\n", error);
      bp->b_flags |= B_ERROR;
      bp->b_error = ENXIO;
      biodone(bp);
      return;
#else
      panic("sdisk_strategy: write request", error);
#endif
    }
  }

} /* sdisk_strategy() */

/* char sdisk read, return non-zero error code if error */
int sdisk_read(dev_t dev, struct uio *uio)
{
  struct iovec *iov;
  kern_return_t rc;
  int disk, partition;
  io_buf_ptr_t data;
  unsigned int count;
  recnum_t recnum;

  /* extract disk number from device number */
  disk = cdev_disk(dev);

#ifdef DEBUG
  printf("sdisk_read: disk = %d\n", disk);
#endif

  /* make sure device port exists */
  if (sdisk[disk].device_port == MACH_PORT_NULL) {
#ifdef DEBUG
    printf("sdisk_read: null port\n");
    return ENXIO;
#else
    panic("sdisk_read: null port");
#endif
  }

  /* check magic numbers in disklabel */
  if (disklabel_magic(disk) != DISKMAGIC) {
#ifdef DEBUG
    printf("sdisk_read: magic 0x%X != 0x%X\n",
      disklabel_magic(disk), DISKMAGIC);
#endif
    return ENXIO;
  }
  if (disklabel_magic2(disk) != DISKMAGIC) {
#ifdef DEBUG
    printf("sdisk_read: magic2 0x%X != 0x%X\n",
      disklabel_magic(disk), DISKMAGIC);
#endif
    return ENXIO;
  }

  /* extract partition number from device number */
  partition = cdev_partition(dev);

#ifdef DEBUG
  printf("sdisk_read: partition = %c\n", partition + 'a');
#endif

  while (uio->uio_iovcnt > 0) {

    iov = uio->uio_iov;
    if (useracc(iov->iov_base, (u_int)iov->iov_len, 0) == 0) return EFAULT;

    /* check for valid partition access */
    if ((uio->uio_offset + iov->iov_len) >
        (partition_size(disk, partition) * disk_secsize(disk))) {
#ifdef DEBUG
      printf("sdisk_read: offset %d + count %d = %d, > size %d\n",
        uio->uio_offset, iov->iov_len,
        partition_size(disk, partition) * disk_secsize(disk));
#endif
      return EINVAL;
    }

    /* use partition offset to calculate record number */
    recnum = (uio->uio_offset / disk_secsize(disk)) +
             partition_offset(disk, partition);

#ifdef DEBUG
    printf("sdisk_read: reading %d bytes at block %d\n", iov->iov_len, recnum);
#endif

    rc = device_read(sdisk[disk].device_port,
		     0, /* wait until data is available */
		     recnum,
		     iov->iov_len,
		     &data,
		     &count);
    if (rc != 0) return (dev_error_to_errno(rc));

    (void)moveout(data, iov->iov_base, count); /* deallocates data */

    iov->iov_base += count;
    iov->iov_len -= count;
    uio->uio_resid -= count;
    uio->uio_offset += count;

    uio->uio_iov++;
    uio->uio_iovcnt--;
  }

  return 0;

} /* sdisk_read() */

/* char sdisk write, return non-zero error code if error */
int sdisk_write(dev_t dev, struct uio *uio)
{
  struct iovec *iov;
  int c;
  kern_return_t rc;
  int disk, partition;
  vm_offset_t kern_addr;
  vm_size_t kern_size;
  vm_offset_t off;
  vm_size_t count;
  recnum_t recnum;

  /* extract disk number from device number */
  disk = cdev_disk(dev);

#ifdef DEBUG
  printf("sdisk_write: disk = %d\n", disk);
#endif

  /* make sure device port exists */
  if (sdisk[disk].device_port == MACH_PORT_NULL) {
#ifdef DEBUG
    printf("sdisk_write: null port\n");
    return ENXIO;
#else
    panic("sdisk_write: null port");
#endif
  }

  /* check magic numbers in disklabel */
  if (disklabel_magic(disk) != DISKMAGIC) {
#ifdef DEBUG
    printf("sdisk_write: magic 0x%X != 0x%X\n",
      disklabel_magic(disk), DISKMAGIC);
#endif
    return ENXIO;
  }
  if (disklabel_magic2(disk) != DISKMAGIC) {
#ifdef DEBUG
    printf("sdisk_write: magic2 0x%X != 0x%X\n",
      disklabel_magic(disk), DISKMAGIC);
#endif
    return ENXIO;
  }

  /* extract partition number from device number */
  partition = cdev_partition(dev);

#ifdef DEBUG
  printf("sdisk_write: partition = %c\n", partition + 'a');
#endif

  while (uio->uio_iovcnt > 0) {
    iov = uio->uio_iov;

    /* check for valid partition access */
    if ((uio->uio_offset + iov->iov_len) >
        (partition_size(disk, partition) * disk_secsize(disk))) {
#ifdef DEBUG
      printf("sdisk_write: offset %d + count %d = %d, > size %d\n",
        uio->uio_offset, iov->iov_len,
        partition_size(disk, partition) * disk_secsize(disk));
#endif
      return EINVAL;
    }

    /* use partition offset to calculate record number */
    recnum = (uio->uio_offset / disk_secsize(disk)) +
             partition_offset(disk, partition);

    /* special case for writing LABELSECTOR */
    /* allow disklabel writes only if previously enabled by ioctl */
    if ((recnum <= LABELSECTOR) &&
#if LABELSECTOR != 0
        (((recnum + iov->iov_len / disk_secsize(disk)) > LABELSECTOR)) &&
#endif
      sdisk[disk].wlabel == 0) {
#ifdef DEBUG
      printf("sdisk_write: attempt to write LABELSECTOR\n");
#endif
#ifdef WRONG
      /* silently ignore attempts to write disklabel */
      recnum++;
      if ((iov->iov_len -= SD_REC_SIZE) < 0) iov->iov_len = 0;
#else
      return EROFS;
#endif
    }

    /* copy data in from user's address space */
    kern_size = iov->iov_len;
    (void)vm_allocate(mach_task_self(), &kern_addr, kern_size, TRUE);
    if (copyin(iov->iov_base, kern_addr, (u_int)iov->iov_len)) {
      (void)vm_deallocate(mach_task_self(), kern_addr, kern_size);
      return EFAULT;
    }

#ifdef DEBUG
    printf("sdisk_write: writing %d bytes at block %d\n",
      iov->iov_len, recnum);
#endif

    rc = device_write(sdisk[disk].device_port,
		      0, /* wait until data written */
		      recnum,
		      kern_addr,
		      iov->iov_len,
		      &count);

    (void)vm_deallocate(mach_task_self(), kern_addr, kern_size);

    if (rc != D_SUCCESS) return (dev_error_to_errno(rc));

    iov->iov_base += count;
    iov->iov_len -= count;
    uio->uio_resid -= count;
    uio->uio_offset += count;

    uio->uio_iov++;
    uio->uio_iovcnt--;
  }

  return 0;

} /* sdisk_write() */

/* check disklabel, return non-zero error code if error */
static int check_disklabel(struct disklabel *lp)
{
  u_short *start, *end, sum;

  /* verify magic numbers */
  if (lp->d_magic != DISKMAGIC) return EINVAL;
  if (lp->d_magic2 != DISKMAGIC) return EINVAL;

  /* verify checksum */
/*
  sum = 0;
  start = (u_short *)lp;
  end = (u_short *)&lp->d_partitions[lp->d_npartitions];
  while (start < end) sum ^= *start++;
  if (sum != 0) return EINVAL;
*/

  return 0;

} /* check_disklabel() */

/* char sdisk ioctl */
int sdisk_ioctl(dev_t dev, int cmd, caddr_t data, int flag)
{
  struct disklabel *lp;
  int error;
  int disk;
  kern_return_t rc;
  int count;
  io_buf_ptr_t tmp_data;
  mach_msg_type_number_t tmp_count;

  /* extract disk number from device number */
  disk = cdev_disk(dev);

#ifdef DEBUG
  printf("sdisk_ioctl: disk = %d\n", disk);
#endif

  /* make sure device port exists */
  if (sdisk[disk].device_port == MACH_PORT_NULL) {
#ifdef DEBUG
    printf("sdisk_ioctl: null port\n");
    return ENXIO;
#else
    panic("sdisk_ioctl: null port");
#endif
  }

  lp = &sdisk[disk].disklabel;

  error = 0;

  switch (cmd) {

    case DIOCGDINFO:
#ifdef DEBUG
      printf("sdisk_ioctl: DIOCGDINFO\n");
#endif
      bcopy(lp, data, sizeof(struct disklabel));
      break;

    case DIOCSDINFO:
#ifdef DEBUG
      printf("sdisk_ioctl: DIOCSDINFO\n");
#endif
      if ((flag & FWRITE) == 0) error = EBADF;
      else if (!(error = check_disklabel((struct disklabel *)data)))
        bcopy(data, lp, sizeof(*lp));
      break;

    case DIOCWLABEL:
#ifdef DEBUG
      printf("sdisk_ioctl: DIOCWLABEL\n");
#endif
      if ((flag & FWRITE) == 0) error = EBADF;
      else sdisk[disk].wlabel = *(int *)data;
      break;

    case DIOCWDINFO:
#ifdef DEBUG
      printf("sdisk_ioctl: DIOCWDINFO\n");
#endif
      if ((flag & FWRITE) == 0) error = EBADF;
      else if (!(error = check_disklabel((struct disklabel *)data))) {
	rc = device_read(sdisk[disk].device_port,
                         0, /* wait until data written */
                         LABELSECTOR,
                         SD_REC_SIZE,
                         &tmp_data,
                         &tmp_count);
        bcopy(data, &tmp_data[LABELOFFSET], sizeof(struct disklabel));
        rc = device_write(sdisk[disk].device_port,
                          0, /* wait until data written */
                          LABELSECTOR,
                          tmp_data,
                          SD_REC_SIZE,
			  &count);
        (void)vm_deallocate(mach_task_self(), tmp_data, tmp_count);
        if (rc != D_SUCCESS) error = dev_error_to_errno(rc);
        else if (count < sizeof(struct disklabel)) error = ENXIO;
        if (!error) bcopy(data, lp, sizeof(*lp));
      }
      break;

    default:
      error = ENOTTY;
      break;

  } /* switch(cmd) */

  return error;

} /* sdisk_ioctl() */

/* close Apple disk, return non-zero error code if error */
int apple_close(dev_t dev, int flag)
{
  int disk;
  mach_port_t port;
  int error;

  /* extract disk number from device number */
  disk = cdev_disk(dev);

#ifdef DEBUG
  printf("apple_close: disk = %d\n", disk);
#endif

  /* remove device port */
  port = apple_device_port[disk];
  apple_device_port[disk] = MACH_PORT_NULL;

  /* error if already closed */
  if (port == MACH_PORT_NULL) return ENXIO;

#ifdef DEBUG
  printf("apple_close: device_close()\n");
#endif

  /* close device and deallocate port */
  error = dev_error_to_errno(device_close(port));
  (void)mach_port_deallocate(mach_task_self(), port);

  /* return error code from device_close() */
  return error;

} /* apple_close() */

/* open Apple disk, return non-zero error code if error */
int apple_open(dev_t dev, int flag)
{
  int disk;
  char name[32];
  kern_return_t rc;
  int error;
  io_buf_ptr_t data;
  mach_msg_type_number_t count;

  /* note that the flag value is ignored */

  /* extract disk number from device number */
  disk = cdev_disk(dev);

#ifdef DEBUG
  printf("apple_open: disk = %d\n", disk);
#endif

  /* do nothing if already open */
  if (apple_device_port[disk] != MACH_PORT_NULL) return 0;

  /* compose Apple sdisk name */
  name[0] = 's';
  name[1] = 'd';
  name[2] = '0' + disk;
  name[3] = 'a';
  name[4] = 0;

#ifdef DEBUG
  printf("apple_open: device_open()\n");
#endif

  /* open Apple sdisk device */
  rc = device_open(device_server_port,
                   D_READ | D_WRITE,
                   name,
                   &apple_device_port[disk]);
  if (rc != D_SUCCESS) return dev_error_to_errno(rc);

  return 0;

} /* apple_open() */

/* char Apple disk read, return non-zero error code if error */
int apple_read(dev_t dev, struct uio *uio)
{
  struct iovec *iov;
  kern_return_t rc;
  int disk;
  io_buf_ptr_t data;
  unsigned int count;

  /* extract disk number from device number */
  disk = cdev_disk(dev);

#ifdef DEBUG
  printf("apple_read: disk = %d\n", disk);
#endif

  /* make sure device port exists */
  if (apple_device_port[disk] == MACH_PORT_NULL) {
#ifdef DEBUG
    printf("apple_read: null port\n");
    return ENXIO;
#else
    panic("apple_read: null port");
#endif
  }

  while (uio->uio_iovcnt > 0) {

    iov = uio->uio_iov;
    if (useracc(iov->iov_base, (u_int)iov->iov_len, 0) == 0) return EFAULT;

#ifdef DEBUG
    printf("apple_read: reading %d bytes at block %d\n",
      iov->iov_len, uio->uio_offset / SD_REC_SIZE);
#endif

    rc = device_read(apple_device_port[disk],
		     0, /* wait until data is available */
		     uio->uio_offset / SD_REC_SIZE,
		     iov->iov_len,
		     &data,
		     &count);
    if (rc != 0) return (dev_error_to_errno(rc));

    (void)moveout(data, iov->iov_base, count); /* deallocates data */

    iov->iov_base += count;
    iov->iov_len -= count;
    uio->uio_resid -= count;
    uio->uio_offset += count;

    uio->uio_iov++;
    uio->uio_iovcnt--;
  }

  return 0;

} /* apple_read() */

/* char Apple disk write, return non-zero error code if error */
int apple_write(dev_t dev, struct uio *uio)
{
  struct iovec *iov;
  kern_return_t rc;
  int disk;
  vm_offset_t kern_addr;
  vm_size_t kern_size;
  vm_offset_t off;
  vm_size_t count;

  /* extract disk number from device number */
  disk = cdev_disk(dev);

#ifdef DEBUG
  printf("apple_write: disk = %d\n", disk);
#endif

  /* make sure device port exists */
  if (apple_device_port[disk] == MACH_PORT_NULL) {
#ifdef DEBUG
    printf("apple_write: null port\n");
    return ENXIO;
#else
    panic("apple_write: null port");
#endif
  }

  while (uio->uio_iovcnt > 0) {
    iov = uio->uio_iov;

    /* copy data in from user's address space */
    kern_size = iov->iov_len;
    (void)vm_allocate(mach_task_self(), &kern_addr, kern_size, TRUE);
    if (copyin(iov->iov_base, kern_addr, (u_int)iov->iov_len)) {
      (void)vm_deallocate(mach_task_self(), kern_addr, kern_size);
      return EFAULT;
    }

#ifdef DEBUG
    printf("apple_write: writing %d bytes at block %d\n",
      iov->iov_len, uio->uio_offset / SD_REC_SIZE);
#endif

    rc = device_write(apple_device_port[disk],
		      0, /* wait until data written */
		      uio->uio_offset / SD_REC_SIZE,
		      kern_addr,
		      iov->iov_len,
		      &count);

    (void)vm_deallocate(mach_task_self(), kern_addr, kern_size);

    if (rc != D_SUCCESS) return (dev_error_to_errno(rc));

    iov->iov_base += count;
    iov->iov_len -= count;
    uio->uio_resid -= count;
    uio->uio_offset += count;

    uio->uio_iov++;
    uio->uio_iovcnt--;
  }

  return 0;

} /* apple_write() */
