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
 * $Log:	scsi.c,v $
 * Revision 2.12  91/08/24  12:28:21  af
 * 	Spls defs, corrected scsi_master_alloc() to allocate
 * 	as expected, support for processor devices, 
 * 	made it possible to avoid sync neg at all (for rb's sake),
 * 	no start unit on processor devices, understand our
 * 	own descriptor.
 * 	[91/08/02  04:01:50  af]
 * 
 * Revision 2.11  91/07/09  23:22:46  danner
 * 	Added gross luna88k ifdef to use <sd.h> instead of <scsi.h>. Will be
 * 	fixed when I understand how to use the configuration tools.
 * 	[91/07/09  11:08:15  danner]
 * 
 * Revision 2.10  91/06/25  20:56:48  rpd
 * 	Tweaks to make gcc happy.
 * 
 * Revision 2.9  91/06/19  11:57:17  rvb
 * 	mips->DECSTATION; vax->VAXSTATION
 * 	[91/06/12  14:02:21  rvb]
 * 
 * 	File moved here from mips/PMAX since it is now "MI" code, also
 * 	used by Vax3100 and soon -- the omron luna88k.
 * 	[91/06/04            rvb]
 * 
 * 	Printing of 'unsupported..' in scsi_slave was screwed cuz
 * 	ui->mi and ui->unit could be bogus at that point in time.
 * 	[91/05/30            af]
 * 
 * Revision 2.8  91/05/14  17:28:00  mrt
 * 	Correcting copyright
 * 
 * Revision 2.7  91/05/13  06:04:45  af
 * 	Added flags to control use of disconnect-reconnect mode.
 * 	Added device-specific close routine (for tapes).
 * 	Wait for start_unit to complete properly and tell the user
 * 	what we are waiting for.
 * 	[91/05/12  16:21:16  af]
 * 
 * Revision 2.6.1.1  91/03/29  16:59:15  af
 * 	Added flags to control use of disconnect-reconnect mode.
 * 	Added device-specific close routine (for tapes).
 * 	Wait for start_unit to complete properly and tell the user
 * 	what we are waiting for.
 * 
 * Revision 2.6  91/02/05  17:44:35  mrt
 * 	Added author notices
 * 	[91/02/04  11:18:01  mrt]
 * 
 * 	Changed to use new Mach copyright
 * 	[91/02/02  12:16:40  mrt]
 * 
 * Revision 2.5  90/12/05  23:34:36  af
 * 	Cleanups, still awaits SCSI-2 fixes.
 * 	[90/12/03  23:38:14  af]
 * 
 * Revision 2.3.1.1  90/11/01  03:38:17  af
 * 	Created.
 * 	[90/09/03            af]
 */
/*
 *	File: scsi.c
 * 	Author: Alessandro Forin, Carnegie Mellon University
 *	Date:	9/90
 *
 *	Middle layer of the SCSI driver: chip independent functions
 *	This file contains Controller and Device-independent functions
 */

#include <scsi.h>

#if	NSCSI > 0
#include <platforms.h>

#include <machine/machparam.h>		/* spl definitions */

#include <mach/std_types.h>
#include <sys/types.h>
#include <scsi/compat_30.h>

#include <chips/busses.h>
#include <scsi/scsi.h>
#include <scsi/scsi2.h>
#include <scsi/scsi_defs.h>
#include <machine/machparam.h>

#ifdef	VAXSTATION
/* We run some of this code on the interrupt stack */
#undef	spl0
#define	spl0()	spl1()
#endif	/*VAXSTATION*/

/*
 *	Overall driver state
 */

target_info_t	scsi_target_data[NSCSI*8];	/* per target state */
scsi_softc_t	scsi_softc_data[NSCSI];		/* per HBA state */
scsi_softc_t	*scsi_softc[NSCSI];		/* quick access&checking */

/*
 * If a specific target should NOT be asked to go synchronous
 * then its bit in this bitmap should be set. Each SCSI controller
 * (Host Bus Adapter) can hold at most 8 targets --> use one
 * byte per controller.  A bit set to one means NO synchronous.
 * Patch with adb if necessary.
 */
unsigned char	scsi_no_synchronous_xfer[NSCSI];

/*
 * For certain targets it is wise to use the long form of the
 * read/write commands even if their capacity would not necessitate
 * it.  Same as above for usage.
 */
unsigned char	scsi_use_long_form[NSCSI];


/*
 * Control over disconnect-reconnect mode.
 */
unsigned char	scsi_might_disconnect[NSCSI] = 	/* do it if deemed appropriate */
		{ 0xff, 0xff};			/* Fix by hand viz NSCSI */
unsigned char	scsi_should_disconnect[NSCSI] =	/* just do it */
		{ 0,};
unsigned char	scsi_initiator_id[NSCSI] =	/* our id on the bus(ses) */
		{ 7, 7};

/*
 * Miscellaneus config
 */
boolean_t	scsi_exabyte_filemarks = FALSE; /* use short filemarks */
int		scsi_watchdog_period = 10;	/* but exabyte needs >=30 for bspace */
int		scsi_delay_after_reset = 5000000;/* microseconds */
boolean_t	scsi_no_automatic_bbr = FALSE;	/* revector bad blocks automatically */

#ifdef	MACH_KERNEL
#else
/* This covers Exabyte's max record size */
unsigned int	scsi_per_target_virtual = 256*1024;
#endif	MACH_KERNEL


/*
 * Device-specific operations are switched off this table
 */

extern char
		*scdisk_name(), *sctape_name(), *scprt_name(),
		*sccpu_name(), *scworm_name(), *sccdrom_name(),
		*scscn_name(), *scmem_name(), *scjb_name(), *sccomm_name();
extern scsi_ret_t
		sctape_optimize(), scprt_optimize(),
		scscn_optimize(), scmem_optimize(), scjb_optimize(),
		sccomm_optimize();
extern scsi_ret_t
		scdisk_open(), sctape_open(), sccpu_open(),
		sctape_close(), sccpu_close();
extern int
		scdisk_strategy(), sctape_strategy(), sccpu_strategy();
extern int
		scdisk_start(), sctape_start(), sccpu_start();

extern io_return_t
		scdisk_set_status(), scdisk_get_status(),
		sctape_set_status(), sctape_get_status();

scsi_devsw_t	scsi_devsw[] = {
/* SCSI_DISK */		{ scdisk_name, 0,
			  scdisk_open, 0, scdisk_strategy, scdisk_start,
			  scdisk_get_status, scdisk_set_status },
/* SCSI_TAPE */		{ sctape_name, sctape_optimize,
			  sctape_open, sctape_close,
			  sctape_strategy, sctape_start,
			  sctape_get_status, sctape_set_status },
/* SCSI_PRINTER */	{ scprt_name, scprt_optimize, /*XXX*/},
/* SCSI_CPU */		{ sccpu_name, 0,
			  sccpu_open, sccpu_close,
			  sccpu_strategy, sccpu_start,},
/* SCSI_WORM */		{ scworm_name, 0,
			  scdisk_open, 0, scdisk_strategy, scdisk_start,
			  scdisk_get_status, scdisk_set_status },
/* SCSI_CDROM */	{ sccdrom_name, 0,
			  scdisk_open, 0, scdisk_strategy, scdisk_start,
			  scdisk_get_status, scdisk_set_status },
#ifdef	SCSI2
/* SCSI_SCANNER */	{ scscn_name, scscn_optimize, /*XXX*/ },
/* SCSI_MEMORY */	{ scmem_name, scmem_optimize, /*XXX*/ },
/* SCSI_J_BOX */	{ scjb_name, scjb_optimize, /*XXX*/ },
/* SCSI_COMM */		{ sccomm_name, sccomm_optimize, /*XXX*/ },
#endif	SCSI2
			0
};

/*
 * Allocation routines for state structures
 */
scsi_softc_t *
scsi_master_alloc(unit, hw)
	char	*hw;
{
	scsi_softc_t	*sc;

	if (unit < NSCSI) {
		sc = &scsi_softc_data[unit];
		scsi_softc[unit] = sc;
		sc->masterno = unit;
		sc->hw_state = hw;
		return sc;
	}
	return 0;
}

target_info_t *
scsi_slave_alloc(unit, slave, hw)
	char	*hw;
{
	target_info_t	*tgt;

	tgt = &scsi_target_data[(unit<<3) + slave];
	tgt->hw_state = hw;
	tgt->dev_ops = 0;	/* later */
	tgt->target_id = slave;
	tgt->block_size = 1;	/* default */
	tgt->flags = TGT_ALIVE;
	tgt->sync_period = 0;
	tgt->sync_offset = 0;
	simple_lock_init(&tgt->target_lock);

	scsi_softc[unit]->target[slave] = tgt;
	return tgt;
}

/*
 * Slave routine:
 *	See if the slave description (controller, unit, ..)
 *	matches one of the slaves found during probe
 *
 * Implementation:
 *	Send out an INQUIRY command to see what sort of device
 *	the slave is.
 * Notes:
 *	At this time the driver is fully functional and works
 *	off interrupts.
 * TODO:
 *	The SCSI2 spec says what exactly must happen: see F.2.3
 */
scsi_slave( ui, reg)
	struct bus_device	*ui;
	unsigned		reg;
{
	scsi_softc_t		*sc = scsi_softc[ui->ctlr];
	target_info_t		*tgt = sc->target[ui->slave];
	scsi_inquiry_data_t	*inq;
	int			ptype, scsi_std, s;

	if (!tgt || !(tgt->flags & TGT_ALIVE))
		return 0;

	/* Might have scanned already */
	if (tgt->dev_ops)
		goto out;

#ifdef	SCSI2
	This is what should happen:
	- for all LUNs 
		INQUIRY
		scsi_verify_state (see)
		scsi_initialize (see)
#endif	SCSI2

	tgt->unit_no = ui->slave;	/* incorrect, but needed early */

	s = spl0();	/* we need interrupts */

	if (BGET(scsi_no_synchronous_xfer,sc->masterno,tgt->target_id))
		tgt->flags |= TGT_DID_SYNCH;

	/*
	 * Ok, it is time to see what type of device this is,
	 * send an INQUIRY cmd and wait till done.
	 * Possibly do the synch negotiation here.
	 */
	scsi_inquiry(sc, tgt, SCSI_INQ_STD_DATA);

	inq = (scsi_inquiry_data_t*)tgt->cmd_ptr;
	ptype = (inq->periph_type) & 0x1f; /* scsi2 safe */

	switch (ptype) {
	case SCSI_DISK :
	case SCSI_TAPE :
	case SCSI_PRINTER :
	case SCSI_CPU :
	case SCSI_WORM :
	case SCSI_CDROM :
#ifdef	SCSI2
	case SCSI_SCANNER :
	case SCSI_MEMORY :
	case SCSI_J_BOX :
	case SCSI_COMM :
/*	case SCSI_PREPRESS1 : reserved, really
	case SCSI_PREPRESS2 :	*/
#endif	SCSI2
		tgt->dev_ops = &scsi_devsw[ptype];
		break;
	default:
		printf("scsi%d: %s %d (x%x). ", ui->ctlr,
		       "Unsupported device type at SCSI id", ui->slave,
			inq->periph_type);
		scsi_print_inquiry(inq, SCSI_INQ_STD_DATA, 0);
		tgt->flags = 0;
		splx(s);
		return 0;
	}

	if (inq->rmb)
		tgt->flags |= TGT_REMOVABLE_MEDIA;

	/*
	 * Tell the user we know this target, then see if we
	 * can be a bit smart about it.
	 */
	scsi_print_inquiry(inq, SCSI_INQ_STD_DATA, tgt->tgt_name);
	if (scsi_debug)
		scsi_print_inquiry(inq, SCSI_INQ_STD_DATA, 0);

#ifdef	SCSI2
	/*
	 * The above says if it currently behaves as a scsi2,
	 * however scsi1 might just be the default setting.
	 * The spec say that even if in scsi1 mode the target
	 * should answer to the full scsi2 inquiry spec.
	 */
	scsi_std = (inq->ansi == 2 || inq->response_fmt == 2) ? 2 : 1;
#if nosey
	if (scsi_std == 2) {
		unsigned char	supp_pages[256], i;
		scsi2_impl_opdef_page_t	*impl;

		scsi_inquiry(sc, tgt, SCSI_INQ_SUPP_PAGES);
		impl = (scsi2_impl_opdef_page_t	*)inq;
		npages = impl->page_len - 2;
		bcopy(impl->supp_opdef, supp_pages, npages);

		for (i = 0; i < npages; i++) {
			scsi_inquiry(sc, tgt, supp_pages[i]);
			scsi_print_inquiry(inq, supp_pages[i], 0);
		}
	}
#endif	nosey

	if (scsi_std == 2) {
		scsi2_impl_opdef_page_t	*impl;
		int i;

		scsi_inquiry(sc, tgt, SCSI_INQ_IMPL_OPDEF);
		impl = (scsi2_impl_opdef_page_t	*)inq;
		for (i = 0; i < impl->page_len - 2; i++)
			if (impl->supp_opdef[i] == SCSI2_OPDEF) {
				scsi_change_definition(sc, tgt, SCSI2_OPDEF);
				/* if success .. */
					tgt->flags |= TGT_SCSI_2_MODE;
				break;
			}
	}
#endif	SCSI2

	splx(s);
out:
	return (strcmp(ui->name, (*tgt->dev_ops->driver_name)(TRUE)) == 0);
}

#ifdef	SCSI2
scsi_verify_state(...)
{
verify_state: send test_unit_ready up to 3 times, each time it fails
(with check condition) send a requeste_sense. It is ok to get UNIT ATTENTION 
the first time only, NOT READY the second, only GOOD the last time.
If you get BUSY or RESERVATION CONFLICT retry.
}

scsi_initialize(...)
{

initialize: send start_unit with immed=0 (->disconnect), if fails
with check condition send requeste_sense and if "illegal request"
proceed anyways. Retry on BUSY.
Do a verify_state, then
disks:
	- mode_sense (current) if ANSI2 or needed by vendor (!!!!)
	  and if check-condition&illegal-request goto capacity
	- mode_sense (changeable)
	- if needed do a mode_select (yes, 512)
	- read_capacity
tapes:

}
#endif	SCSI2

/*
 * Attach routine:
 *	Fill in all the relevant per-slave data and make
 *	the slave operational.
 *
 * Implementation:
 *	Get target's status, start the unit and then
 *	switch off to device-specific functions to gather
 *	as much info as possible about the slave.
 */
scsi_attach(ui)
	register struct bus_device *ui;
{
	scsi_softc_t		*sc = scsi_softc[ui->mi->unit];
	target_info_t		*tgt = sc->target[ui->slave];
	int			s, i;

	printf(" (%s %s)", (*tgt->dev_ops->driver_name)(FALSE),tgt->tgt_name);

	s = spl0();

	/* sense return from inquiry */
	scsi_request_sense(sc, tgt, 0, 0);

	/*
	 * Do this twice, certain targets need it
	 */
	if (tgt->dev_ops != &scsi_devsw[SCSI_CPU]) {
		(void) scsi_start_unit(sc, tgt, SCSI_CMD_SS_START, 0);
		i = 0;
		while (scsi_start_unit(sc, tgt, SCSI_CMD_SS_START, 0) == SCSI_RET_RETRY) {
			if (i++ == 5)
				printf(".. not yet online ..");
			delay(1000000);
			if (i == 60) {
				printf(" seems hopeless.");
				break;
			}
		}
	}

	/*
	 * See if it is up and about
	 */
	scsi_test_unit_ready(sc, tgt, 0);

	if (tgt->dev_ops->optimize)
		(*tgt->dev_ops->optimize)(sc, tgt);

	tgt->flags |= TGT_FULLY_PROBED;

	splx(s);

}

/*
 * Probe routine:
 *	See if a device answers.  Used AFTER autoconf.
 *
 * Implementation:
 *	First ask the HBA to see if anyone is there at all, then
 *	call the scsi_slave and scsi_attach routines with a fake ui.
 */
boolean_t
scsi_probe( sc, tgt_ptr, target_id, ior)
	scsi_softc_t		*sc;
	target_info_t		**tgt_ptr;
	io_req_t		ior;
{
	struct bus_device	ui;
	target_info_t		*tgt;

	if (!sc->probe || target_id > 7 || target_id == sc->initiator_id)
		return FALSE;	/* sanity */

	if (sc->target[target_id] == 0)
		scsi_slave_alloc( sc->masterno, target_id, sc->hw_state);
	tgt = sc->target[target_id];
	tgt->flags = 0;/* we donno yet */

	/* mildly enquire */
	if (!(sc->probe)(sc, tgt, ior))
		goto fail;

	/* There is something there, see what it is */
	bzero(&ui, sizeof(ui));
	ui.ctlr = sc->masterno;
	ui.unit =
	ui.slave = target_id;
	ui.name = "";

	/* this fails on the name for sure */
	(void) scsi_slave( &ui, 0 /* brrrr */);
	if ((tgt->flags & TGT_ALIVE) == 0)
		goto fail;

	{
		struct bus_ctlr	mi;

		mi.unit = sc->masterno;
		ui.mi = &mi;
		printf("%s at slave %d ",
			(*tgt->dev_ops->driver_name)(TRUE), target_id);
		scsi_attach(&ui);
	}

	*tgt_ptr = tgt;
	return TRUE;
fail:
	tgt->flags = 0;
	return FALSE;
}


/*
 * Watchdog routine:
 *	Issue a SCSI bus reset if a target holds up the
 *	bus for too long.
 *
 * Implementation:
 *	Each HBA that wants to use this should have a
 *	watchdog_t structure at the head of its hardware
 *	descriptor.  This variable is set by this periodic
 *	routine and reset on bus activity. If it is not reset on
 *	time (say some ten seconds or so) we reset the
 *	SCSI bus.
 * NOTE:
 *	An HBA must be ready to accept bus reset interrupts
 *	properly in order to use this.
 */
scsi_watchdog(hw)
	watchdog_t	*hw;
{
	unsigned int	s = splbio();

	switch (hw->watchdog_state) {
	case SCSI_WD_EXPIRED:

		/* double check first */
		if (hw->nactive == 0) {
			hw->watchdog_state = SCSI_WD_INACTIVE;
			break;
		}
		if (scsi_debug)
			printf("SCSI Watchdog expired\n");
		hw->watchdog_state = SCSI_WD_INACTIVE;
		(*hw->reset)(hw);
		break;

	case SCSI_WD_ACTIVE:

		hw->watchdog_state = SCSI_WD_EXPIRED;
		break;

	case SCSI_WD_INACTIVE:

		break;
	}

	/* do this here, fends against powered down devices */
	if (scsi_watchdog_period != 0)
		timeout(scsi_watchdog, hw, scsi_watchdog_period * hz);

	splx(s);
}


/*
 * BusReset Notification:
 *	Called when the HBA sees a BusReset interrupt
 *
 * Implementation:
 *	Go through the list of targets, redo the synch
 *	negotiation, and restart whatever operation was
 *	in progress for that target.
 */
scsi_bus_was_reset(sc)
	scsi_softc_t	*sc;
{
	register target_info_t	*tgt;
	int			i;
	/*
	 * Redo the synch negotiation
	 */
	for (i = 0; i < 8; i++) {
		io_req_t	ior;
		int		s;

		tgt = sc->target[i];
		if (!tgt || !(tgt->flags & TGT_ALIVE) ||
		    (tgt->flags & TGT_US))
			continue;

		tgt->flags &= ~(TGT_DID_SYNCH|TGT_DISCONNECTED);

		/*
		 * retry the synch negotiation
		 */
		ior = tgt->ior;
		tgt->ior = 0;
		printf(".. tgt %d ", tgt->target_id);
		if (BGET(scsi_no_synchronous_xfer,sc->masterno,tgt->target_id))
			tgt->flags |= TGT_DID_SYNCH;
		else {
			s = spl0();
			scsi_test_unit_ready(sc, tgt, 0);
			splx(s);
		}
		tgt->ior = ior;
	}

	/*
	 * Notify each target of the accident
	 */
	for (i = 0; i < 8; i++) {
		tgt = sc->target[i];
		if (!tgt || !(tgt->flags & TGT_ALIVE))
			continue;
		tgt->done = SCSI_RET_ABORTED|SCSI_RET_RETRY;
		if (tgt->ior)
			(*tgt->dev_ops->restart)( sc, tgt, TRUE);
	}

	printf("%s", " reset complete\n");
}

#endif	NSCSI > 0
