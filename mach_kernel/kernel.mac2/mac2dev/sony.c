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
 * $Log:	sony.c,v $
 * Revision 2.2  91/09/12  16:47:49  bohman
 * 	Created.
 * 	[91/09/11  16:04:36  bohman]
 * 
 */

/*
 *	Apple Macintosh II Mach (macmach)
 *
 *	File: mac2dev/sony.c
 *	Author: David E. Bohman II (CMU macmach)
 */

#include <mach/mach_types.h>

#include <device/device_types.h>
#include <device/io_req.h>

#include <kern/queue.h>

#include <mac2os/Types.h>
#include <mac2os/Errors.h>
#include <mac2os/Files.h>
#include <mac2os/Disks.h>

#include <mac2dev/sony.h>

/*
 * Macintosh floppy driver
 * for MACH 3.0
 */

#define SONY_DRIVE_LOW	1
#define SONY_DRIVE_HIGH	3

#define SONY_RefNum	-5

typedef struct sony {
    queue_head_t	pending;
    IOParam		*pb;
    Ptr			buffer;
    int			ref_count;
} *sony_t;

static struct sony	sony_info;

typedef struct {
    unsigned	     :30,
      		drive:2;
} *sony_id_t;

void	sony_begin();
OSErr	sonyeject(), sonyformat(), sonystatus();

io_return_t
sony_open(number, mode, ior)
int		number;
dev_mode_t	mode;
io_req_t	ior;
{
    register sony_id_t		dev = (sony_id_t)&number;
    register sony_t		sony;
    DrvSts			stats;

    if (dev->drive < SONY_DRIVE_LOW || dev->drive > SONY_DRIVE_HIGH)
	return (D_NO_SUCH_DEVICE);

    if (sonystatus(dev->drive, &stats) != noErr)
	return (D_NO_SUCH_DEVICE);

    if (stats.installed != 1 && stats.installed != 0)
	return (D_NO_SUCH_DEVICE);

    sony = &sony_info;

    if (sony->ref_count == 0) {
	sony->pb = (IOParam *)NewPtr(sizeof (IOParam));
	if (!sony->pb)
	    return (D_NO_MEMORY);

	sony->buffer = (Ptr)NewPtr(32*1024);
	if (!sony->buffer) {
	    DisposPtr(sony->pb);
	    return (D_NO_MEMORY);
	}

	queue_init(&sony->pending);

	sony->ref_count++;
    }

    return (D_SUCCESS);
}

io_return_t
sony_close(number)
int		number;
{
    register sony_t		sony = &sony_info;

    if (--sony->ref_count == 0) {
	DisposPtr(sony->pb);
	DisposPtr(sony->buffer);
    }

    return (D_SUCCESS);
}

io_return_t
sony_rw(number, ior)
int			number;
register io_req_t	ior;
{
    register sony_t		sony = &sony_info;
    register io_return_t	result;
    register			s;
    boolean_t			wait = FALSE,
    				sony_read_done(), sony_write_done();

    if (ior->io_count % 512)
	return (D_INVALID_SIZE);

    if (ior->io_count > (32*1024))
	ior->io_count = (32*1024);

    if (ior->io_op & IO_READ) {
	result = device_read_alloc(ior, ior->io_count);
	if (result != KERN_SUCCESS)
	    return (result);

	ior->io_done = sony_read_done;
    }
    else {
	result = device_write_get(ior, &wait);
	if (result != KERN_SUCCESS)
	    return (result);

	ior->io_done = sony_write_done;
    }

    s = spl1();

    if (!queue_empty(&sony->pending)) {
	enqueue_tail(&sony->pending, (queue_entry_t)ior);
	(void) splx(s);
	if (!wait)
	    return (D_IO_QUEUED);
	else {
	    iowait(ior);
	    return (D_SUCCESS);
	}
    }

    enqueue_head(&sony->pending, (queue_entry_t)ior);

    (void) splx(s);

    (void) sony_begin(ior);

    if (!wait)
	return (D_IO_QUEUED);
    else {
	iowait(ior);
	return (D_SUCCESS);
    }
}

boolean_t
sony_read_done(ior)
register io_req_t	ior;
{
    register sony_t	sony = &sony_info;
    register IOParam	*pb = sony->pb;
    register		s;

    if (pb->ioResult == noErr)
	bcopy(sony->buffer, ior->io_data, pb->ioActCount);

    ior->io_error = sonyerror(pb->ioResult);
    ior->io_residual = ior->io_count - pb->ioActCount;

    (void) ds_read_done(ior);

    s = spl1();

    if (!queue_empty(&sony->pending))
	ior = (io_req_t)queue_first(&sony->pending);
    else
	ior = (io_req_t) 0;

    (void) splx(s);

    if (ior)
	(void) sony_begin(ior);

    return (TRUE);
}

boolean_t
sony_write_done(ior)
register io_req_t	ior;
{
    register sony_t	sony = &sony_info;
    register IOParam	*pb = sony->pb;
    register		s;

    ior->io_error = sonyerror(pb->ioResult);
    ior->io_residual = ior->io_count - pb->ioActCount;

    (void) ds_write_done(ior);

    s = spl1();

    if (!queue_empty(&sony->pending))
	ior = (io_req_t)queue_first(&sony->pending);
    else
	ior = (io_req_t) 0;

    (void) splx(s);

    if (ior)
	(void) sony_begin(ior);

    return (TRUE);
}

#ifndef MODE24
static
#endif /* MODE24 */
void
sony_complete()
{
    register sony_t	sony = &sony_info;
    register io_req_t	ior;

    ior = (io_req_t)dequeue_head(&sony->pending);

    if (ior)
	(void) iodone(ior);
}

void
sony_begin(ior)
register io_req_t	ior;
{
    register sony_id_t	dev = (sony_id_t)&ior->io_unit;
    register sony_t	sony = &sony_info;
    register IOParam	*pb;
#ifdef MODE24
    extern void		sony_done();
#endif /* MODE24 */

    pb = sony->pb;

#ifdef MODE24
    pb->ioCompletion = sony_done;
#else /* MODE24 */
    pb->ioCompletion = sony_complete;
#endif /* MODE24 */

    pb->ioVRefNum = dev->drive;
    pb->ioRefNum = SONY_RefNum;
    pb->ioBuffer = sony->buffer;
    pb->ioReqCount = ior->io_count;
    pb->ioActCount = 0;
    pb->ioPosMode = fsFromStart;
    pb->ioPosOffset = (ior->io_recnum << 9);

    if (ior->io_op & IO_READ)
	(void) PBRead(pb, TRUE);
    else {
	bcopy(ior->io_data, sony->buffer, ior->io_count);
	(void) PBWrite(pb, TRUE);
    }
}

io_return_t
sony_getstat(number, flavor, data, count)
int		number;
register	flavor;
dev_status_t	data;
unsigned	*count;
{
    register sony_id_t		dev = (sony_id_t)&number;
    register sony_t		sony = &sony_info;
    register OSErr		err;

    switch (flavor) {
      case SONY_STATUS:
	{
	    DrvSts	stats;

	    err = sonystatus(dev->drive, &stats);
	    if (err != noErr)
		return (sonyerror(err));

	    data[0] = *(int *)&stats.writeProt;
	    *count = 1;
	}
	break;
	
      default:
	return (D_INVALID_OPERATION);
    }

    return (D_SUCCESS);
}

io_return_t
sony_setstat(number, flavor, data, count)
int		number;
register	flavor;
dev_status_t	data;
unsigned	count;
{
    register sony_id_t		dev = (sony_id_t)&number;
    register sony_t		sony = &sony_info;
    register OSErr		err;

    switch (flavor) {
      case SONY_EJECT:
	err = sonyeject(dev->drive);
	if (err != noErr)
	    return (sonyerror(err));
	break;

      case SONY_FORMAT:
	{
	    register	mode;

	    if (count == 1)
		mode = *data;
	    else
		mode = 0;

	    err = sonyformat(dev->drive, mode);
	    if (err != noErr)
		return (sonyerror(err));
	}
	break;

      default:
	return (D_INVALID_OPERATION);
    }

    return (D_SUCCESS);
}

sonyerror(err)
register OSErr	err;
{
    switch (err) {
      case noErr:
	return (D_SUCCESS);

      case wPrErr:
	return (D_INVALID_OPERATION);

      case offLinErr:
	return (D_DEVICE_DOWN);

      case eofErr:
	return (D_INVALID_RECNUM);

      case memFullErr:
	return (D_NO_MEMORY);

      default:
	return (D_IO_ERROR);
    }
}

OSErr
sonyeject(drive)
short		drive;
{
    register CntrlParam	*pb;
    register OSErr	err;

    pb = (CntrlParam *)NewPtr(sizeof (CntrlParam));
    if (!pb)
	return (memFullErr);

    pb->ioVRefNum = drive;
    pb->ioCRefNum = SONY_RefNum;
    pb->csCode = 7;

    err = thread_io(PBControl, pb);

    DisposPtr(pb);

    return (err);
}

OSErr
sonyformat(drive, mode)
short		drive;
{
    register CntrlParam	*pb;
    register OSErr	err;

    pb = (CntrlParam *)NewPtr(sizeof (CntrlParam));
    if (!pb)
	return (memFullErr);

    pb->ioVRefNum = drive;
    pb->ioCRefNum = SONY_RefNum;
    pb->csCode = 6;
    pb->csParam[0] = 0;

    err = thread_io(PBControl, pb);

    DisposPtr(pb);

    return (err);
}

OSErr
sonystatus(drive, data)
short		drive;
{
    register CntrlParam	*pb;
    register OSErr	err;

    pb = (CntrlParam *)NewPtr(sizeof (CntrlParam));
    if (!pb)
	return (memFullErr);

    pb->ioVRefNum = drive;
    pb->ioCRefNum = SONY_RefNum;
    pb->csCode = 8;
    
    err = thread_io(PBStatus, pb);
    if (err == noErr)
	bcopy(&pb->csParam[0], data, sizeof (DrvSts));

    DisposPtr(pb);

    return (err);
}
