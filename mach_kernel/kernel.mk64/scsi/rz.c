/* 
 * Mach Operating System
 * Copyright (c) 1991,1990 Carnegie Mellon University
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
 * $Log:	rz.c,v $
 * Revision 2.10  91/08/24  12:27:43  af
 * 	Pass along the ior in open calls.
 * 	Removed luna88k specialization, as the device headers have been
 * 	 rationalized. 
 * 	[91/08/02  03:51:40  af]
 * 
 * Revision 2.9  91/07/09  23:22:23  danner
 * 	Added gross ifdef luna88k to use <sd.h> instead of <scsi.h>. Will
 * 	 be fixed as soon as I figure out the configuration tools.
 * 	[91/07/09            danner]
 * 
 * 	<<<log message for ./kernel/scsi/rz.c>>>
 * 	[91/07/09  11:05:28  danner]
 * 
 * Revision 2.8  91/06/19  11:56:54  rvb
 * 	File moved here from mips/PMAX since it is now "MI" code, also
 * 	used by Vax3100 and soon -- the omron luna88k.
 * 	[91/06/04            rvb]
 * 
 * Revision 2.7  91/05/14  17:26:11  mrt
 * 	Correcting copyright
 * 
 * Revision 2.6  91/05/13  06:04:14  af
 * 	Use a longer timeout on opens (exabytes are so slooooow to
 * 	get ready).  If we are still rewinding the tape make open
 * 	fail.  Start watchdog on first open, for those adapters
 * 	that need it.  Ask for sense data when unit does not
 * 	come online quickly enough. On close, invoke anything
 * 	target-specific that needs to.
 * 	[91/05/12  16:04:29  af]
 * 
 * Revision 2.5.1.1  91/03/29  17:21:00  af
 * 	Use a longer timeout on opens (exabytes are so slooooow to
 * 	get ready).  If we are still rewinding the tape make open
 * 	fail.  Start watchdog on first open, for those adapters
 * 	that need it.  Ask for sense data when unit does not
 * 	come online quickly enough. On close, invoke anything
 * 	target-specific that needs to.
 * 
 * Revision 2.5  91/02/05  17:43:37  mrt
 * 	Added author notices
 * 	[91/02/04  11:16:27  mrt]
 * 
 * 	Changed to use new Mach copyright
 * 	[91/02/02  12:15:22  mrt]
 * 
 * Revision 2.4  90/12/20  16:59:59  jeffreyh
 * 	Do not use minphys(), we do not need to trim requests.
 * 	[90/12/11            af]
 * 
 * Revision 2.3  90/12/05  23:33:46  af
 * 	Cut&retry for requests that exceed the HBA's dma capacity.
 * 	[90/12/03  23:32:38  af]
 * 
 * Revision 2.1.1.1  90/11/01  03:43:31  af
 * 	Created.
 * 	[90/10/21            af]
 */
/*
 *	File: rz.c
 * 	Author: Alessandro Forin, Carnegie Mellon University
 *	Date:	10/90
 *
 *	Top layer of the SCSI driver: interface with the MI side.
 */

/*
 * This file contains the code that is common to all scsi devices,
 * operations and/or behaviours specific to certain devices live
 * in the corresponding rz_mumble files.
 */

#include <scsi.h>

#if	(NSCSI>0)

#include <mach/std_types.h>
#include <scsi/compat_30.h>

#ifdef	MACH_KERNEL
#include <kern/time_out.h>
#else	/*MACH_KERNEL*/
#include <sys/kernel.h>		/* for hz */

static io_req_t	getbp();
#endif	/*MACH_KERNEL*/

#include <scsi/scsi_defs.h>
#include <scsi/rz.h>

/*static*/ boolean_t
rz_check(dev, p_sc, p_tgt)
	scsi_softc_t	**p_sc;
	target_info_t	**p_tgt;
{
	if (rzcontroller(dev) >= NSCSI ||
	    (*p_sc = scsi_softc[rzcontroller(dev)]) == 0)
		return FALSE;

	*p_tgt = (*p_sc)->target[rzslave(dev)];

	if (!*p_tgt ||
	    !((*p_tgt)->flags&TGT_ALIVE))
		return FALSE;
	return TRUE;
}

/*
 * Open routine
 *
 * On tapes and other devices might have to wait a bit for
 * the unit to come alive. The following patchable variable
 * takes this into account
 */
int rz_open_timeout = 60;/* seconds */

rz_open(dev, mode, ior)
	int		dev;
	dev_mode_t	mode;
	io_req_t	ior;
{
	scsi_softc_t	*sc = 0;
	target_info_t	*tgt;
	scsi_ret_t	ret;
	register int	i;

	if (!rz_check(dev, &sc, &tgt)) {
		/*
		 * Probe it again: might have installed a new device
		 */
		if (!sc || !scsi_probe(sc, &tgt, rzslave(dev), ior))
			return D_NO_SUCH_DEVICE;
	}

	/* tapes do not wait for rewind to complete on close */
	if (tgt->ior && !(tgt->flags & TGT_ONLINE))
		return D_WOULD_BLOCK;

	if (scsi_debug)
		printf("opening %s%d..", (*tgt->dev_ops->driver_name)(TRUE), dev&0xff);

	if (sc->watchdog) {
		(*sc->watchdog)(tgt->hw_state);
		sc->watchdog = 0;
	}

	/*
	 * Bring the unit online, retrying if necessary.
	 * If the target is spinning up we wait for it.
	 */
	if ( ! (tgt->flags & TGT_ONLINE)) {
		io_req_t	ior;

		io_req_alloc(ior,0);		
		ior->io_next = 0;
		ior->io_count = 0;

		for (i = 0; i < rz_open_timeout; i++) {

			ior->io_op = IO_INTERNAL;
			ret = scsi_test_unit_ready(sc, tgt, ior);

			if (ret == SCSI_RET_SUCCESS)
				break;

			if (ret == SCSI_RET_DEVICE_DOWN) {
				i = rz_open_timeout;
				break;
			}

			if (ret == SCSI_RET_NEED_SENSE) {

				ior->io_op = IO_INTERNAL;
				ior->io_count = 0;
				ior->io_residual = 0;
				tgt->ior = ior;
				scsi_request_sense(sc, tgt, ior, 0);
				iowait(ior);

			}

			if (i == 5) printf("%s%d: %s\n", 
					   (*tgt->dev_ops->driver_name)(TRUE),
					   tgt->target_id,
					   "Waiting to come online..");
			timeout(wakeup, tgt, hz);
			await(tgt);
		}

		/* lock on removable media */
		if ((i != rz_open_timeout) && (tgt->flags & TGT_REMOVABLE_MEDIA)) {
			ior->io_op = IO_INTERNAL;
			/* too many dont support it. Sigh */
			tgt->flags |= TGT_OPTIONAL_CMD;
			(void) scsi_medium_removal( sc, tgt, FALSE, ior);
			tgt->flags &= ~TGT_OPTIONAL_CMD;
		}

		io_req_free(ior);
		if (i == rz_open_timeout)
			return D_DEVICE_DOWN;
	}
	/*
	 * Perform anything open-time special on the device
	 */
	if (tgt->dev_ops->open) {
		ret = (*tgt->dev_ops->open)(sc, tgt, ior);
		if (ret != SCSI_RET_SUCCESS) {
			 printf("%s%d: open failed x%x\n",
			 (*tgt->dev_ops->driver_name)(TRUE), dev&0xff, ret);
			return ret;
		}
	}
	tgt->flags |= TGT_ONLINE;
	return 0;	/* XXX */
}

rz_close(dev)
	int		dev;
{
	scsi_softc_t	*sc;
	target_info_t	*tgt;
	scsi_ret_t	ret;

	if (!rz_check(dev, &sc, &tgt))
		return D_NO_SUCH_DEVICE;

	if (scsi_debug)
		printf("closing %s%d..", (*tgt->dev_ops->driver_name)(TRUE), dev&0xff);

	if (tgt->flags & TGT_REMOVABLE_MEDIA) {
		io_req_t	ior;

		io_req_alloc(ior,0);		
		ior->io_next = 0;
		ior->io_count = 0;
		ior->io_op = IO_INTERNAL;
		/* too many dont support it. Sigh */
		tgt->flags |= TGT_OPTIONAL_CMD;
		(void) scsi_medium_removal( sc, tgt, TRUE, ior);
		tgt->flags &= ~TGT_OPTIONAL_CMD;
		io_req_free(ior);
	}

	/*
	 * Perform anything close-time special on the device
	 */
	if (tgt->dev_ops->close) {
		ret = (*tgt->dev_ops->close)(sc, tgt);
		if (ret != SCSI_RET_SUCCESS) {
			 printf("%s%d: close failed x%x\n",
			 (*tgt->dev_ops->driver_name)(TRUE), dev&0xff, ret);
		}
	}
	if (tgt->flags & TGT_REMOVABLE_MEDIA)
		tgt->flags &= ~TGT_ONLINE;

	return 0;	/* XXX */
}

/* our own minphys */
rz_minphys(ior)
	io_req_t	ior;
{
#ifdef	MACH_KERNEL
#else	/*MACH_KERNEL*/
	if (ior->io_count > scsi_per_target_virtual)
		ior->io_count = scsi_per_target_virtual;
#endif	/*MACH_KERNEL*/
}

rz_read(dev, ior)
	int		dev;
	io_req_t	ior;
{
	target_info_t	*tgt;

	tgt = scsi_softc[rzcontroller(dev)]->target[rzslave(dev)];

#ifdef	MACH_KERNEL
	return block_io(tgt->dev_ops->strategy, rz_minphys, ior);
#else	/*MACH_KERNEL*/
	return physio(tgt->dev_ops->strategy, getbp(dev), dev, IO_READ, rz_minphys, ior);
#endif	/*MACH_KERNEL*/
}

rz_write(dev, ior)
	int		dev;
	io_req_t	ior;
{
	target_info_t	*tgt;

	tgt = scsi_softc[rzcontroller(dev)]->target[rzslave(dev)];

	if (tgt->flags & TGT_READONLY)
		return D_INVALID_OPERATION;

#ifdef	MACH_KERNEL
	return block_io(tgt->dev_ops->strategy, rz_minphys, ior);
#else	/*MACH_KERNEL*/
	return physio(tgt->dev_ops->strategy, getbp(dev), dev, IO_WRITE, rz_minphys, ior);
#endif	/*MACH_KERNEL*/
}

rz_get_status(dev, flavor, status, status_count)
	int		dev;
	int		flavor;
	dev_status_t	status;
	unsigned int	*status_count;
{
	scsi_softc_t	*sc;
	target_info_t	*tgt;

	sc  = scsi_softc[rzcontroller(dev)];
	tgt = sc->target[rzslave(dev)];

	if (scsi_debug)
		printf("rz_get_status: x%x x%x x%x x%x\n",
			dev, flavor, status, *status_count);
	return (*tgt->dev_ops->get_status)(sc, tgt, flavor, status, status_count);
}

rz_set_status(dev, flavor, status, status_count)
	int		dev;
	int		flavor;
	dev_status_t	status;
	unsigned int	status_count;
{
	scsi_softc_t	*sc;
	target_info_t	*tgt;

	sc  = scsi_softc[rzcontroller(dev)];
	tgt = sc->target[rzslave(dev)];

	if (scsi_debug)
		printf("rz_set_status: x%x x%x x%x x%x\n",
			dev, flavor, status, status_count);
	return (*tgt->dev_ops->set_status)(sc, tgt, flavor, status, status_count);
}

#if	0
rz_size(dev)
	int	dev;
{	
	return -1;
}

rz_reset(adaptor)
	int	adaptor;
{}
#endif

#ifdef	MACH_KERNEL
#else	/*MACH_KERNEL*/

rz_strategy(ior)
	io_req_t	ior;
{
	target_info_t	*tgt;
	register int	dev = ior->io_unit;

	tgt = scsi_softc[rzcontroller(dev)]->target[rzslave(dev)];

	return (*tgt->dev_ops->strategy)(ior);
}

#include <sys/ioctl.h>
#define	IOCPARM_SIZE(c)		(((c)>>16)&IOCPARM_MASK)
#define	IOC_WDSIZE(s)		((IOCPARM_SIZE(s))>>2)

rz_ioctl(dev, cmd, data, flag)
{
	io_return_t	error;
	unsigned int	count;

	count = IOC_WDSIZE(cmd);
        if (cmd & (IOC_VOID|IOC_IN)) {
            error = rz_set_status(dev, cmd, (dev_status_t)data, count);
            if (error)
                return (error);
        }
        if (cmd & IOC_OUT) {
            error = rz_get_status(dev, cmd, (dev_status_t *)data, &count);
            if (error)
                return (error);
        }
        return (0);
}

/* This is a very simple-minded config,
 * assumes we have << 8 disks per bus */
#define NBUF	(NSCSI*8)
struct	io_req	rz_buffers[NBUF];

static io_req_t
getbp(dev)
{
	io_req_t	ior;
	int		hash = minor(dev) >> 3;

	ior = &rz_buffers[hash];
#if 1
	if (ior->io_op & IO_BUSY) {
		register io_req_t ior;
		for (ior = rz_buffers; ior < &rz_buffers[NBUF]; ior++)
			if ((ior->io_op & IO_BUSY) == 0)
				return ior;
		
	}
#endif
	return ior;
}

/*
 * This ugliness is only needed because of the
 * way the minor is encoded for tapes.
 */
tz_open(dev, mode, ior)
	int		dev;
	dev_mode_t	mode;
	io_req_t	ior;
{
	io_return_t	error;

	error = rz_open(TAPE_UNIT(dev), mode, ior);
	if(error)
		return error;
	if (TAPE_REWINDS(dev)) {
		scsi_softc_t	*sc;
		target_info_t	*tgt;

		rz_check(TAPE_UNIT(dev), &sc, &tgt);
		tgt->flags |= TGT_REWIND_ON_CLOSE;
	}
	return 0;
}

tz_close(dev) { return rz_close(TAPE_UNIT(dev));}
tz_read(dev, ior) { return rz_read(TAPE_UNIT(dev), ior);}
tz_write(dev, ior) { return rz_write(TAPE_UNIT(dev), ior);}
tz_ioctl(dev, cmd, data, flag) { return rz_ioctl(TAPE_UNIT(dev), cmd, data, flag);}

#endif	/*MACH_KERNEL*/

#endif	(NSCSI>0)
