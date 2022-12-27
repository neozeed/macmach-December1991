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
 * $Log:	mscsi.c,v $
 * Revision 2.2  91/09/12  16:47:09  bohman
 * 	Created.
 * 	[91/09/11  15:35:57  bohman]
 * 
 */

/*
 *	Apple Macintosh II Mach (macmach)
 *
 *	File: mac2dev/mscsi.c
 *	Author: David E. Bohman II (CMU macmach)
 */

#include <mach/mach_types.h>

#include <device/device_types.h>
#include <device/io_req.h>

#include <mac2/act.h>

#include <mac2dev/mscsi.h>

#include <mac2os/Types.h>
#include <mac2os/Errors.h>
#include <mac2os/SCSI.h>

/*
 * NOTE: This module will only
 * work in a 32 bit clean environment.
 */

/*
 * Keep a queue of pending requests,
 * as well as the current one.
 */
static queue_head_t	mscsi_queue;
static io_req_t		mscsi_cur;

/*
 * The macII hardware does support
 * interrupt driven disk I/O, however
 * the current (7.0) MacOS code does not
 * support the use of the hardware in this
 * manner.  We fake it by asynchronously
 * polling the REQ line to begin data
 * transfers.  The 'softint' activity
 * facility is used for this purpose.
 * Activities in this queue are executed
 * whenever a normal return to user mode
 * is taken (via rte_user) or on a return
 * from an interrupt (via rte_intr) when
 * the previous context is at IPL0.
 */
static struct act	*mscsi_act;
#define MSCSI_ACT_LIST	0

static int		mscsi_mode;

static			scsi_cmd(), scsi_transfer();

/*
 * Called during machine initialization.
 */
void
scsi_initialize()
{
    extern struct actlist	actsoft;
    static void			scsi_intr();

    mscsi_act = makeact(scsi_intr, SR_IPL0, 1);
    if (!mscsi_act)
	panic("scsi_initialize");

    addact(MSCSI_ACT_LIST, mscsi_act, &actsoft);

    queue_init(&mscsi_queue);
}

/*
 * Meant to be called by a client layer
 * when a thread needs to block inside
 * the kernel until a request is completed.
 */
io_return_t
scsi_wait(ior)
register io_req_t	ior;
{
    int			s;

    s = splbio();
    while ((ior->io_op & IO_DONE) == 0) {
	assert_wait((int)ior, FALSE);
	thread_block((void (*)()) 0);
    }
    splx(s);

    return (ior->io_error);
}

/*
 * Called by a client layer to perform
 * a SCSI operation.  Currently, this
 * module only supports disk operations,
 * however it would be relatively straight-
 * forward to generalize it for other types
 * of SCSI devices.
 */
io_return_t
scsi_op(ior)
register io_req_t	ior;
{
    register		s = splbio();

    if (mscsi_cur != 0 || !queue_empty(&mscsi_queue)) {
	enqueue_tail(&mscsi_queue, (queue_entry_t)ior);
	splx(s);
	return (D_IO_QUEUED);
    }

    mscsi_cur = ior;

    splx(s);

    return (scsi_cmd(ior));
}

#define IODONE(result) \
MACRO_BEGIN			\
    register	s = splbio();	\
    mscsi_cur = 0;		\
    splx(s);			\
    return (result);		\
MACRO_END

#define scsiCmdSetup(x) \
MACRO_BEGIN			\
    cmd = (SCSICmdBlk) { 0 };	\
    cmd.op = (x);		\
MACRO_END

/*
 * Send a SCSI command to a target.  This
 * routine may return D_IO_QUEUED if there
 * is a data phase for the command (scsi_transfer()
 * gets called) and the target does not assert
 * REQ right after the command is sent.
 */
static
io_return_t
scsi_cmd(ior)
register io_req_t	ior;
{
    static unsigned short	stat, msg;
    static SCSICmdBlk		cmd;

    switch (ior->io_op & (IO_READ | IO_WRITE | IO_OPEN)) {
      case IO_OPEN:
	scsiCmdSetup(SCSI_TSTUNITRDY);
	mscsi_mode = 0;
	break;

      case IO_READ:
	scsiCmdSetup(SCSI_READ);
	mscsi_mode = SCSI_X_READ | SCSI_X_BLIND;
	cmd.rw.msblk = scsiMSBlk(ior->io_recnum);
	cmd.rw.blk = scsiBlk(ior->io_recnum);
	cmd.rw.lsblk = scsiLSBlk(ior->io_recnum);

	cmd.rw.nblk = scsiNBlk(ior->io_count >> SCSI_NBBLKLOG2);
	break;

      case IO_WRITE:
	scsiCmdSetup(SCSI_WRITE);
	mscsi_mode = SCSI_X_WRITE | SCSI_X_BLIND;
	cmd.rw.msblk = scsiMSBlk(ior->io_recnum);
	cmd.rw.blk = scsiBlk(ior->io_recnum);
	cmd.rw.lsblk = scsiLSBlk(ior->io_recnum);

	cmd.rw.nblk = scsiNBlk(ior->io_count >> SCSI_NBBLKLOG2);
	break;

      default:
	panic("scsi_cmd");
    }

    if (SCSIGet()) {
	(void) SCSIReset();
	IODONE(D_IO_ERROR);
    }

    if (SCSISelect(ior->io_unit))
	IODONE(D_NO_SUCH_DEVICE);

    if (SCSICmd(&cmd, SCSI_CMDLEN)) {
	(void) SCSIReset();
	IODONE (D_IO_ERROR);
    }

    if (mscsi_mode)
	return (scsi_transfer(ior));
    else
    if (SCSIComplete(&stat, &msg, 120)) {
	(void) SCSIReset();
	IODONE(D_IO_ERROR);
    }

    IODONE(D_SUCCESS);
}
#undef scsiCmdSetup

/*
 * This can be patched to FALSE to
 * make all SCSI operations be performed
 * synchronously.
 */
boolean_t	async_scsi = TRUE;

/*
 * Perform a SCSI data transfer phase
 * for a SCSI command.
 */
static
io_return_t
scsi_transfer(ior)
register io_req_t	ior;
{
    OSErr			result;
    static SCSITransferInstr	prog[3];
    static unsigned short	stat, msg;
#ifdef MODE24
    static unsigned char	buffer[SCSI_MAXPHYS];
    extern boolean_t mac32bit;
#endif /* MODE24 */

    if (async_scsi)
	if ((SCSIStat() & SCSI_STAT_REQ) == 0) {
	    runact(MSCSI_ACT_LIST, mscsi_act, 0, 0);
	    return (D_IO_QUEUED);
	}

    if (ior->io_count > SCSI_MAXPHYS)
	panic("scsi_transfer");

    if (mscsi_mode & SCSI_X_BLIND) {
	prog[0] =
	    (SCSITransferInstr)
#ifdef MODE24
		{ scInc,
                  mac32bit ? (unsigned)ior->io_data : (unsigned)buffer,
                  SCSI_NBBLK
                };
#else /* MODE24 */
		{ scInc, (unsigned)ior->io_data, SCSI_NBBLK };
#endif /* MODE24 */
	prog[1] =
	    (SCSITransferInstr)
		{ scLoop, scsiBranch(-1), (ior->io_count
					   >> SCSI_NBBLKLOG2) };
	prog[2] =
	    (SCSITransferInstr)
		{ scStop };
    }
    else {
	prog[0] =
	    (SCSITransferInstr)
#ifdef MODE24
		{ scNoInc,
                  mac32bit ? (unsigned)ior->io_data : (unsigned)buffer,
                  ior->io_count
                };
#else /* MODE24 */
		{ scNoInc, (unsigned)ior->io_data, ior->io_count };
#endif /* MODE24 */
	prog[1] =
	    (SCSITransferInstr)
		{ scStop };
    }
    
    if (mscsi_mode & SCSI_X_WRITE) {
#ifdef MODE24
        if (!mac32bit) bcopy(ior->io_data, buffer, ior->io_count);
#endif /* MODE24 */
	if (mscsi_mode & SCSI_X_BLIND)
	    result = SCSIWBlind(prog);
	else
	    result = SCSIWrite(prog);
    }
    else {
	if (mscsi_mode & SCSI_X_BLIND)
	    result = SCSIRBlind(prog);
	else
	    result = SCSIRead(prog);
#ifdef MODE24
        if (!mac32bit) bcopy(buffer, ior->io_data, ior->io_count);
#endif /* MODE24 */
    }
    
    if (result) {
	(void) SCSIReset();
	IODONE(D_IO_ERROR);
    }

    if (SCSIComplete(&stat, &msg, 120)) {
	(void) SCSIReset();
	IODONE(D_IO_ERROR);
    }

    IODONE(D_SUCCESS);
}
#undef IODONE

/*
 * The 'interrupt' routine.  First it
 * attempts to finish up the currently
 * active command, and then performs the
 * next one.
 */
static
void
scsi_intr()
{
    register io_req_t		ior;
    register io_return_t	result;

    ior = mscsi_cur;
    if (!ior)
	panic("mscsi_intr");

    result = scsi_transfer(ior);
    if (result == D_IO_QUEUED)
	return;

    if (ior->io_error = result)
	ior->io_op |= IO_ERROR;

    iodone(ior);

    while (mscsi_cur = (io_req_t)dequeue_head(&mscsi_queue)) {
	ior = mscsi_cur;
	result = scsi_cmd(ior);
	if (result == D_IO_QUEUED)
	    break;

	if (ior->io_error = result)
	    ior->io_op |= IO_ERROR;

	iodone(ior);
    }
}
