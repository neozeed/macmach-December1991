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
 * $Log:	scsi_aha15_hdw.c,v $
 * Revision 2.2  91/08/24  12:27:24  af
 * 	Created, from the Adaptec spec:
 * 	"AHA-1540B/1542B Technical Reference Manual"
 * 	Adaptec Inc. Literature Department,
 * 	600001-031 Rev. C, 7/90, Milpitas, CA.
 * 	[91/06/14            af]
 * 
 */
/*
 *	File: scsi_aha15_hdw.c
 * 	Author: Alessandro Forin, Carnegie Mellon University
 *	Date:	6/91
 *
 *	Bottom layer of the SCSI driver: chip-dependent functions
 *
 *	This file contains the code that is specific to the Adaptec
 *	AHA-15xx family of Intelligent SCSI Host Adapter boards:
 *	probing, start operation, and interrupt routine.
 */

/*
 * Since the board is "Intelligent" we do not need scripts like
 * other simpler HBAs.  Maybe.
 */

#include <aha.h>
#if	NAHA > 0

#include <platforms.h>		/* for now this is only a joke */

#include <mach/std_types.h>
#include <sys/types.h>
#include <chips/busses.h>
#include <scsi/compat_30.h>

#include <sys/syslog.h>

#include <scsi/scsi.h>
#include <scsi/scsi2.h>
#include <scsi/scsi_defs.h>

#include <scsi/adapters/scsi_aha15.h>

#ifdef	AT386
#define	MACHINE_PGBYTES		I386_PGBYTES
#define	MAPPABLE			0
#define	gimmeabreak()		asm("int3")
#include <i386/pio.h>		/* inlining of outb and inb */
#endif	/*AT386*/

#ifndef	MACHINE_PGBYTES		/* cross compile check */
#define	MACHINE_PGBYTES		0x1000
#define	MAPPABLE			1
#define	gimmeabreak()		Debugger("gimmeabreak");
#endif

/*
 * Data structures: ring, ccbs, a per target buffer
 */

#define	AHA_NMBOXES	2		/* no need for more, I think */
struct aha_mb_ctl {
	aha_mbox_t	omb[AHA_NMBOXES];
	aha_mbox_t	imb[AHA_NMBOXES];
	char		iidx, oidx;	/* roving ptrs into */
};
#define	next_mbx_idx(i)	((((i)+1)==AHA_NMBOXES)?0:((i)+1))

#define AHA_NCCB	8	/* for now */
struct aha_ccb_raw {
	target_info_t	*active_target;
	aha_ccb_t	ccb;
	char		buffer[256];	/* separate out this ? */
};
#define	rccb_to_cmdptr(rccb)	((char*)&((rccb)->ccb.ccb_scsi_cmd))

/* forward decls */
int	aha_reset_scsibus();
boolean_t aha_probe_target();

/*
 * State descriptor for this layer.  There is one such structure
 * per (enabled) board
 */
struct aha_softc {
	watchdog_t	wd;
	unsigned int	port;		/* I/O port */

	int		ntargets;	/* how many alive on this scsibus */

	scsi_softc_t	*sc;		/* HBA-indep info */

	struct aha_mb_ctl	mb;	/* mailbox structures */

	/* This chicanery is for mapping back the phys address
	   of a CCB (which we get in an MBI) to its virtual */
	/* [we could use phystokv(), but it isn't standard] */
	vm_offset_t		I_hold_my_phys_address;
	struct aha_ccb_raw	aha_ccbs[AHA_NCCB];

} aha_softc_data[NAHA];

typedef struct aha_softc *aha_softc_t;

aha_softc_t	aha_softc[NAHA];

struct aha_ccb_raw *
mb_to_rccb(aha, mbi)
	aha_softc_t	aha;
	aha_mbox_t	mbi;
{
	vm_offset_t	addr;

	AHA_MB_GET_PTR(&mbi,addr); /* phys address of ccb */

	/* make virtual */
	addr = ((vm_offset_t)&aha->I_hold_my_phys_address) +
		(addr - aha->I_hold_my_phys_address);

	/* adjust by proper offset to get base */
	addr -= (vm_offset_t)&(((struct aha_ccb_raw *)0)->ccb);

	return (struct aha_ccb_raw *)addr;
}

target_info_t *
aha_tgt_alloc(aha, id, sns_len, tgt)
	aha_softc_t	aha;
	target_info_t	*tgt;
{
	struct aha_ccb_raw	*rccb;

	aha->ntargets++;

	if (tgt == 0)
		tgt = scsi_slave_alloc(aha - aha_softc_data, id, aha);

	rccb = &(aha->aha_ccbs[id]);
	rccb->ccb.ccb_reqsns_len = sns_len;
	tgt->cmd_ptr = rccb_to_cmdptr(rccb);
	tgt->dma_ptr = 0;
	return tgt;
}

/*
 * Synch xfer timing conversions
 */
#define aha_to_scsi_period(a)	((200 + ((a) * 50)) >> 2)
#define scsi_period_to_aha(p)	((((p) << 2) - 200) / 50)

/*
 * Definition of the controller for the auto-configuration program.
 */

/* DOCUMENTATION */
/* base ports can be:
	0x334, 0x330 (default), 0x234, 0x230, 0x134, 0x130
   possible interrupt channels are:
	9, 10, 11 (default), 12, 14, 15
   DMA channels can be:
	7, 6, 5 (default), 0
/* DOCUMENTATION */

int	aha_probe(), scsi_slave(), scsi_attach(), aha_go(), aha_intr();

caddr_t	aha_std[NAHA] = { 0 };
struct	bus_device *aha_dinfo[NAHA*8];
struct	bus_ctlr *aha_minfo[NAHA];
struct	bus_driver aha_driver = 
        { aha_probe, scsi_slave, scsi_attach, aha_go, aha_std, "rz", aha_dinfo,
	  "ahac", aha_minfo, BUS_INTR_B4_PROBE};

#define DEBUG
#ifdef	DEBUG

#define	PRINT(x)	if (scsi_debug) printf x

aha_state(port)
{
	register unsigned char st, intr;

	if (port == 0)
		port = 0x330;
	st = inb(AHA_STATUS_PORT(port));
	intr = inb(AHA_INTR_PORT(port));

	db_printf("status %x intr %x\n", st, intr);
	return 0;
}

aha_target_state(tgt)
	target_info_t	*tgt;
{
	if (tgt == 0)
		tgt = aha_softc[0]->sc->target[0];
	if (tgt == 0)
		return 0;
	db_printf("fl %x dma %X+%x cmd %x@%X id %x per %x off %x ior %X ret %X\n",
		tgt->flags, tgt->dma_ptr, tgt->transient_state.dma_offset, tgt->cur_cmd,
		tgt->cmd_ptr, tgt->target_id, tgt->sync_period, tgt->sync_offset,
		tgt->ior, tgt->done);

	return 0;
}

aha_all_targets(unit)
{
	int i;
	target_info_t	*tgt;
	for (i = 0; i < 8; i++) {
		tgt = aha_softc[unit]->sc->target[i];
		if (tgt)
			aha_target_state(tgt);
	}
}

#define TRMAX 200
int tr[TRMAX+3];
int trpt, trpthi;
#define	TR(x)	tr[trpt++] = x
#define TRWRAP	trpthi = trpt; trpt = 0;
#define TRCHECK	if (trpt > TRMAX) {TRWRAP}

#define TRACE

#ifdef TRACE

#define LOGSIZE 256
int aha_logpt;
char aha_log[LOGSIZE];

#define MAXLOG_VALUE	0x1e
struct {
	char *name;
	unsigned int count;
} logtbl[MAXLOG_VALUE];

static LOG(e,f)
	char *f;
{
	aha_log[aha_logpt++] = (e);
	if (aha_logpt == LOGSIZE) aha_logpt = 0;
	if ((e) < MAXLOG_VALUE) {
		logtbl[(e)].name = (f);
		logtbl[(e)].count++;
	}
}

aha_print_log(skip)
	int skip;
{
	register int i, j;
	register unsigned char c;

	for (i = 0, j = aha_logpt; i < LOGSIZE; i++) {
		c = aha_log[j];
		if (++j == LOGSIZE) j = 0;
		if (skip-- > 0)
			continue;
		if (c < MAXLOG_VALUE)
			db_printf(" %s", logtbl[c].name);
		else
			db_printf("-%x", c & 0x7f);
	}
	return 0;
}

aha_print_stat()
{
	register int i;
	register char *p;
	for (i = 0; i < MAXLOG_VALUE; i++) {
		if (p = logtbl[i].name)
			printf("%d %s\n", logtbl[i].count, p);
	}
}

#else	/*TRACE*/
#define	LOG(e,f)
#define LOGSIZE
#endif	/*TRACE*/

#else	/*DEBUG*/
#define PRINT(x)
#define	LOG(e,f)
#define LOGSIZE
#define TRCHECK
#define TR(a)

#endif	/*DEBUG*/

/* Utility functions at end */


/*
 *	Probe/Slave/Attach functions
 */

/*
 * Probe routine:
 *	Should find out (a) if the controller is
 *	present and (b) which/where slaves are present.
 *
 * Implementation:
 *	Just ask the board to do it
 */
aha_probe(port, ui)
	register	port;
	struct bus_ctlr	*ui;
{
	int             unit = ui->unit;
	aha_softc_t     aha = &aha_softc_data[unit];
	int		target_id;
	scsi_softc_t	*sc;
	int		s;
	boolean_t	did_banner = FALSE;
	struct aha_devs	installed;
	struct aha_conf conf;

	/* No interrupts yet */
	s = splbio();

	/*
	 * We should be called with a sensible port, but you never know.
	 * Send an echo command and see that we get it back properly
	 */
	{
		register char    st;

		st = inb(AHA_STATUS_PORT(port));
		if (st & AHA_CSR_DATAO_FULL) goto fail;

		outb(AHA_COMMAND_PORT(port), AHA_CMD_ECHO);
		delay(1000);/*?*/
		st = inb(AHA_STATUS_PORT(port));
		if (st & (AHA_CSR_CMD_ERR|AHA_CSR_DATAO_FULL))
			goto fail;

                outb(AHA_COMMAND_PORT(port), 0x5e);
		delay(1000);

		st = inb(AHA_STATUS_PORT(port));
		if ((st & AHA_CSR_CMD_ERR) ||
		    ((st & AHA_CSR_DATAI_FULL) == 0))
			goto fail;

		st = inb(AHA_DATA_PORT(port));
		if (st != 0x5e) {
fail:			splx(s);
			return 0;
		}
		outb(AHA_CONTROL_PORT(port), AHA_CTL_INTR_CLR);
	}

#if	MAPPABLE
	/* Mappable version side */
	AHA_probe(port, ui);
#endif	/*MAPPABLE*/

	/*
	 * Who are we ?
	 */
	{
		struct aha_inq	inq;
		char		*id;

		aha_command(port, AHA_CMD_INQUIRY, 0, 0, &inq, sizeof(inq), TRUE);
		switch (inq.board_id) {
		case AHA_BID_1540_B16:
		case AHA_BID_1540_B64:
			id = "1540";break;
		case AHA_BID_1540B:
			id = "1540B/1542B";break;
		case AHA_BID_1640:
			id = "1640";break;
		default:
			id = 0; break;
		}

		printf("Adaptec %s [id %x], rev %c%c, options x%x\n",
			id ? id : "Board",
			inq.board_id, inq.frl_1, inq.frl_2, inq.options);

		/* readin conf data as well */
		aha_command(port, AHA_CMD_GET_CONFIG, 0, 0, &conf, sizeof(conf), TRUE);
	}
	/*
	 * Initialize hw descriptor, cache some pointers
	 */
	aha_softc[unit] = aha;
	aha->port = port;

	sc = scsi_master_alloc(unit, aha);
	aha->sc = sc;

	sc->go = aha_go;
	sc->watchdog = scsi_watchdog;
	sc->probe = aha_probe_target;
	aha->wd.reset = aha_reset_scsibus;

	/* Stupid limitation, no way around it */
	sc->max_dma_data = (AHA_MAX_SEGLIST-1) * MACHINE_PGBYTES;

	/*
	 * Reset board.
	 */
	aha_reset(port, TRUE);

	/*
	 * Init mailbox control stuff
	 * Also, set up the DMA channel we'll be using.
	 */
	{
		register int		d;
		static struct {
		    char port;
		    char init_data;
		} aha_dma_init[8][2] = {
			{{0x0b,0x0c}, {0x0a,0x00}},	/* channel 0 */
			{{0,0},{0,0}},
			{{0,0},{0,0}},
			{{0,0},{0,0}},
			{{0,0},{0,0}},
			{{0xd6,0xc1}, {0xd4,0x01}},	/* channel 5 (def) */
			{{0xd6,0xc2}, {0xd4,0x02}},	/* channel 6 */
			{{0xd6,0xc3}, {0xd4,0x03}}	/* channel 7 */
		};


		for (d = 0; d < 8; d++)
			if ((1 << d) & conf.dma_arbitration) break;

		aha_init_1(aha);

		/* Initialize the DMA controller viz the channel we'll use */
		outb(aha_dma_init[d][0].port, aha_dma_init[d][0].init_data);
		outb(aha_dma_init[d][1].port, aha_dma_init[d][1].init_data);

		/* make mapping phys->virt possible for CCBs */
		aha->I_hold_my_phys_address =
			(vm_offset_t)kvtophys(&aha->I_hold_my_phys_address);

	}

	/*
	 * Our SCSI id on the bus.
	 * The user can only set this via the board's jumpers.
	 */
	{
		register int	d, i;

		sc->initiator_id = conf.my_scsi_id;

		for (i = 0; i < 8; i++)
			if ((1 << i) & conf.intr_ch) break;
		i += 9;

		for (d = 0; d < 8; d++)
			if ((1 << d) & conf.dma_arbitration) break;

		printf("%s%d: [dma ch %d intr ch %d] my SCSI id is %d",
			ui->name, unit,	d, i, sc->initiator_id);

		/* Interrupt vector setup */
		ui->sysdep1 = i;
		take_ctlr_irq(ui);
	}

	/*
	 * More initializations
	 */
	{
		register target_info_t	*tgt;

		aha_init_2(port);

		/* allocate a desc for tgt mode role */
		tgt = aha_tgt_alloc(aha, sc->initiator_id, 1, 0);
		sccpu_new_initiator(aha->sc, tgt, tgt); /* self */

	}

	/* Now we could take interrupts, BUT we do not want to
	   be selected as targets by some other host just yet */

	/*
	 * For all possible targets, see if there is one and allocate
	 * a descriptor for it if it is there.
	 * This includes ourselves, when acting as target
	 */
	aha_command( port, AHA_CMD_FIND_DEVICES, 0, 0, &installed, sizeof(installed), TRUE);
	for (target_id = 0; target_id < 8; target_id++) {

		if (target_id == sc->initiator_id)	/* done already */
			continue;

		if (installed.tgt_luns[target_id] == 0)
			continue;

		printf(",%s%d", did_banner++ ? " " : " target(s) at ",
				target_id);

		/* Normally, only LUN 0 */
		if (installed.tgt_luns[target_id] != 1)
			printf("(%x)", installed.tgt_luns[target_id]);
		/*
		 * Found a target
		 */
		(void) aha_tgt_alloc(aha, target_id, 1/*no REQSNS*/, 0);

	}
	printf(".\n");
	splx(s);

	return 1;
}

boolean_t
aha_probe_target(sc, tgt, ior)
	scsi_softc_t		*sc;
	target_info_t		*tgt;
	io_req_t		ior;
{
	aha_softc_t     aha = aha_softc[sc->masterno];
	boolean_t	newlywed;

	newlywed = (tgt->cmd_ptr == 0);
	if (newlywed) {
		/* desc was allocated afresh */
		(void) aha_tgt_alloc(aha,tgt->target_id, 1/*no REQSNS*/, tgt);
	}

	if (scsi_inquiry(sc, tgt, SCSI_INQ_STD_DATA) == SCSI_RET_DEVICE_DOWN)
		return FALSE;

	tgt->flags = TGT_ALIVE;
	return TRUE;
}

aha_reset(port, quick)
{
	register char	st;

	/*
	 * Reset board and wait till done
	 */
	outb(AHA_CONTROL_PORT(port), AHA_CTL_SOFT_RESET);
	do {
		delay(25);
		st = inb(AHA_STATUS_PORT(port));
	} while ((st & (AHA_CSR_IDLE|AHA_CSR_INIT_REQ)) == 0);

	if (quick) return;

	/*
	 * reset the scsi bus.  Does NOT generate an interrupt (bozos)
	 */
	outb(AHA_CONTROL_PORT(port), AHA_CTL_SCSI_RST);
}

aha_init_1(aha)
	aha_softc_t     aha;
{
	struct aha_init		a;
	vm_offset_t		phys;

	bzero(&aha->mb, sizeof(aha->mb)); /* also means all free */
	a.mb_count = AHA_NMBOXES;
	phys = kvtophys(&aha->mb);
	AHA_ADDRESS_SET(a.mb_ptr, phys);
	aha_command(aha->port, AHA_CMD_INIT, &a, sizeof(a), 0, 0, TRUE);
}

aha_init_2(port)
{
	char		disable = AHA_MBO_DISABLE;
	struct aha_tgt	role;

	/* Disable MBO available interrupt */
	aha_command(port, AHA_CMD_MBO_IE, &disable, 1, 0,0, FALSE);

	/* Enable target mode role */
	role.enable = 1;
	role.luns = 1; /* only LUN 0 */
	aha_command(port, AHA_CMD_ENB_TGT_MODE, &role, sizeof(role), 0, 0, TRUE);
}

aha_init(aha)
	aha_softc_t     aha;
{
	aha_init_1(aha);
	aha_init_2(aha->port);
}

/*
 *	Operational functions
 */

/*
 * Start a SCSI command on a target
 */
aha_go(sc, tgt, cmd_count, in_count, cmd_only)
	scsi_softc_t		*sc;
	target_info_t		*tgt;
	boolean_t		cmd_only;
{
	aha_softc_t		aha;
	int			s;
	struct aha_ccb_raw	*rccb;
	int			len;
	vm_offset_t		virt, phys;

	LOG(1,"go");

	aha = (aha_softc_t)tgt->hw_state;

/* XXX delay the handling of the ccb till later */
	rccb = &(aha->aha_ccbs[tgt->target_id]);
	rccb->active_target = tgt;

	/*
	 * We can do real DMA.
	 */
/*	tgt->transient_state.copy_count = 0;	unused */
/*	tgt->transient_state.dma_offset = 0;	unused */

	tgt->transient_state.cmd_count = cmd_count;

	if ((tgt->cur_cmd == SCSI_CMD_WRITE) ||
	    (tgt->cur_cmd == SCSI_CMD_LONG_WRITE)){
		io_req_t	ior = tgt->ior;
		register int	len = ior->io_count;

		tgt->transient_state.out_count = len;

		/* How do we avoid leaks here ?  Trust the board
		   will do zero-padding, for now.  XXX CHECKME */
#if 0
		if (len < tgt->block_size) {
			bzero(to + len, tgt->block_size - len);
			len = tgt->block_size;
			tgt->transient_state.out_count = len;
		}
#endif
	} else {
		tgt->transient_state.out_count = 0;
	}

	/* See above for in_count < block_size */
	tgt->transient_state.in_count = in_count;

	/*
	 * Setup CCB state
	 */
	tgt->done = SCSI_RET_IN_PROGRESS;

	switch (tgt->cur_cmd) {
	    case SCSI_CMD_READ:
	    case SCSI_CMD_LONG_READ:
		LOG(9,"readop");
		virt = (vm_offset_t)tgt->ior->io_data;
		len = tgt->transient_state.in_count;
		rccb->ccb.ccb_in = 1; rccb->ccb.ccb_out = 0;
		break;
	    case SCSI_CMD_WRITE:
	    case SCSI_CMD_LONG_WRITE:
		LOG(0x1a,"writeop");
		virt = (vm_offset_t)tgt->ior->io_data;
		len = tgt->transient_state.out_count;
		rccb->ccb.ccb_in = 0; rccb->ccb.ccb_out = 1;
		break;
	    case SCSI_CMD_INQUIRY:
	    case SCSI_CMD_REQUEST_SENSE:
	    case SCSI_CMD_MODE_SENSE:
	    case SCSI_CMD_RECEIVE_DIAG_RESULTS:
	    case SCSI_CMD_READ_CAPACITY:
	    case SCSI_CMD_READ_BLOCK_LIMITS:
		LOG(0x1c,"cmdop");
		LOG(0x80+tgt->cur_cmd,0);
		virt = (vm_offset_t)tgt->cmd_ptr;
		len = tgt->transient_state.in_count;
		rccb->ccb.ccb_in = 1; rccb->ccb.ccb_out = 0;
		break;
	    case SCSI_CMD_MODE_SELECT:
	    case SCSI_CMD_REASSIGN_BLOCKS:
	    case SCSI_CMD_FORMAT_UNIT:
		tgt->transient_state.cmd_count = sizeof(scsi_command_group_0);
		len =
		tgt->transient_state.out_count = cmd_count - sizeof(scsi_command_group_0);
		virt = (vm_offset_t)tgt->cmd_ptr+sizeof(scsi_command_group_0);
		rccb->ccb.ccb_in = 0; rccb->ccb.ccb_out = 1;
		LOG(0x1c,"cmdop");
		LOG(0x80+tgt->cur_cmd,0);
		break;
	    default:
		LOG(0x1c,"cmdop");
		LOG(0x80+tgt->cur_cmd,0);
		virt = 0;
		len = 0;
		rccb->ccb.ccb_in = 0; rccb->ccb.ccb_out = 0;
	}

	aha_prepare_rccb(tgt, rccb, virt, len);	

	rccb->ccb.ccb_lun = tgt->lun;
	rccb->ccb.ccb_scsi_id = tgt->target_id;

/*	AHA_LENGTH_SET(rccb->ccb.ccb_linkptr, 0);	unused */
/*	rccb->ccb.ccb_linkid = 0;			unused */

	/*
	 * XXX here and everywhere, locks!
	 */
	s = splbio();

	if (aha->wd.nactive++ == 0)
		aha->wd.watchdog_state = SCSI_WD_ACTIVE;

	LOG(3,"enqueue");

	aha_start_scsi(aha, &rccb->ccb);

	splx(s);
}

aha_prepare_rccb(tgt, rccb, virt, len)
	target_info_t		*tgt;
	struct aha_ccb_raw	*rccb;
	vm_offset_t		virt;
	vm_size_t		len;
{
	vm_offset_t		phys;

	rccb->ccb.ccb_cmd_len = tgt->transient_state.cmd_count;

	/* this opcode is refused, grrrr. */
/*	rccb->ccb.ccb_code = AHA_CCB_I_CMD_R;	/* default common case */
	rccb->ccb.ccb_code = AHA_CCB_I_CMD;	/* default common case */
	AHA_LENGTH_SET(rccb->ccb.ccb_datalen, len);/* default common case */

	if (virt == 0) {
		/* no xfers */
		AHA_ADDRESS_SET(rccb->ccb.ccb_dataptr, 0);
	} else if (len <= MACHINE_PGBYTES) {
/* INCORRECT: what if across two pages :INCORRECT */
		/* simple xfer */
		phys = kvtophys(virt);
		AHA_ADDRESS_SET(rccb->ccb.ccb_dataptr, phys);
	} else {
		/* messy xfer */
		aha_seglist_t		*seglist;
		vm_offset_t		ph1, off;
		vm_size_t		l1;

		/* this opcode does not work, grrrrr */
/*		rccb->ccb.ccb_code = AHA_CCB_I_CMD_SG_R;*/
		rccb->ccb.ccb_code = AHA_CCB_I_CMD_SG;

		if (tgt->dma_ptr == 0)
			aha_alloc_segment_list(tgt);
		seglist = (aha_seglist_t *) tgt->dma_ptr;
		phys = kvtophys(seglist);
		AHA_ADDRESS_SET(rccb->ccb.ccb_dataptr, phys);

		ph1 = /*i386_trunc_page*/ virt & ~(MACHINE_PGBYTES - 1);
		off = virt & (MACHINE_PGBYTES - 1);
		ph1 = kvtophys(ph1) + off;
		l1 = MACHINE_PGBYTES - off;

		off = 1;/* now #pages */
		while (1) {
			AHA_ADDRESS_SET(seglist->ptr, ph1);
			AHA_LENGTH_SET(seglist->len, l1);
			seglist++;

			if ((len -= l1) <= 0)
				break;
			virt += l1; off++;

			ph1 = kvtophys(virt);
			l1 = (len > MACHINE_PGBYTES) ? MACHINE_PGBYTES : len;
		}
		l1 = off * sizeof(*seglist);
		AHA_LENGTH_SET(rccb->ccb.ccb_datalen, l1);
	}
}

aha_start_scsi(aha, ccb)
	aha_softc_t	aha;
	aha_ccb_t	*ccb;
{
	register aha_mbox_t	*mb;
	register		idx;
	vm_offset_t		phys;
	aha_mbox_t		mbo;		

	LOG(4,"start");
	LOG(0x80+ccb->ccb_scsi_id,0);

	/*
	 * Get an MBO, spin if necessary (takes little time)
	 */
	phys = kvtophys(ccb);
	/* might cross pages, but should be ok (kernel is contig) */
	AHA_MB_SET_PTR(&mbo,phys);
	mbo.mb.mb_cmd = AHA_MBO_START;
/* XXX needs MP locking */
		idx = aha->mb.oidx;
		aha->mb.oidx = next_mbx_idx(idx);
		mb = &aha->mb.omb[idx];
		while (mb->mb.mb_status != AHA_MBO_FREE)
			delay(1);
		mb->bits = mbo.bits;
/* XXX needs MP locking */

	/*
	 * Start the board going
	 */
	aha_command(aha->port, AHA_CMD_START, 0, 0, 0, 0, FALSE);
}

/*
 * Interrupt routine
 *	Take interrupts from the board
 *
 * Implementation:
 *	TBD
 */
aha_intr(unit)
{
	register aha_softc_t	aha;
	register		port;
	register		csr, intr;
#if	MAPPABLE
	extern boolean_t	rz_use_mapped_interface;

	if (rz_use_mapped_interface)
		return AHA_intr(unit);
#endif	/*MAPPABLE*/

	aha = aha_softc[unit];
	port = aha->port;

	LOG(5,"\n\tintr");
gotintr:
	/* collect ephemeral information */
	csr  = inb(AHA_STATUS_PORT(port));
	intr = inb(AHA_INTR_PORT(port));

	/*
	 * Check for errors
	 */
	if (csr & (AHA_CSR_DIAG_FAIL|AHA_CSR_CMD_ERR)) {
/* XXX */	gimmeabreak();
	}

	/* drop spurious interrupts */
	if ((intr & AHA_INTR_PENDING) == 0) {
		LOG(2,"SPURIOUS");
		return;
	}
	outb(AHA_CONTROL_PORT(port), AHA_CTL_INTR_CLR);

TR(csr);TR(intr);TRCHECK

	if (intr & AHA_INTR_RST)
		return aha_bus_reset(aha);

	/* we got an interrupt allright */
	if (aha->wd.nactive)
		aha->wd.watchdog_state = SCSI_WD_ACTIVE;

	if (intr == AHA_INTR_DONE) {
		/* csr & AHA_CSR_CMD_ERR --> with error */
		LOG(6,"done");
		return;
	}

/*	if (intr & AHA_INTR_MBO_AVAIL) will not happen */

	/* Some real work today ? */
	if (intr & AHA_INTR_MBI_FULL) {
		register int		idx;
		register aha_mbox_t	*mb;
		int			nscan = 0;
		aha_mbox_t		mbi;
rescan:
/* XXX needs MP locking */
		idx = aha->mb.iidx;
		aha->mb.iidx = next_mbx_idx(idx);
		mb = &aha->mb.imb[idx];
		mbi.bits = mb->bits;
		mb->mb.mb_status = AHA_MBI_FREE;
/* XXX needs MP locking */

		nscan++;

		switch (mbi.mb.mb_status) {

		case AHA_MBI_FREE:
			if (nscan >= AHA_NMBOXES)
				return;
			goto rescan;
			break;

		case AHA_MBI_SUCCESS:
		case AHA_MBI_ERROR:
			aha_initiator_intr(aha, mbi);
			break;

		case AHA_MBI_NEED_CCB:
			aha_target_intr(aha, mbi);
			break;

/*		case AHA_MBI_ABORTED:	/* this we wont see */
/*		case AHA_MBI_NOT_FOUND:	/* this we wont see */
		default:
			log(	LOG_KERN,
				"aha%d: Bogus status (x%x) in MBI\n",
				unit, mbi.mb.mb_status);
			break;
		}
		
		/* peek ahead */
		if (aha->mb.imb[aha->mb.iidx].mb.mb_status != AHA_MBI_FREE)
			goto rescan;
	}

	/* See if more work ready */
	if (inb(AHA_INTR_PORT(port)) & AHA_INTR_PENDING) {
		LOG(7,"\n\tre-intr");
		goto gotintr;
	}
}

/*
 * The interrupt routine turns to one of these two
 * functions, depending on the incoming mbi's role
 */
aha_target_intr(aha, mbi)
	aha_softc_t	aha;
	aha_mbox_t	mbi;
{
	target_info_t		*initiator;	/* this is the caller */
	target_info_t		*self;		/* this is us */
	int			len;

	if (mbi.mbt.mb_cmd != AHA_MBI_NEED_CCB)
		gimmeabreak();

	/* If we got here this is not zero .. */
	self = aha->sc->target[aha->sc->initiator_id];

	initiator = aha->sc->target[mbi.mbt.mb_initiator_id];
	/* ..but initiators are not required to answer to our inquiry */
	if (initiator == 0) {
		/* allocate */
		initiator = aha_tgt_alloc(aha, mbi.mbt.mb_initiator_id,
				sizeof(scsi_sense_data_t) + 5, 0);

		/* We do not know here wether the host was down when
		   we inquired, or it refused the connection.  Leave
		   the decision on how we will talk to it to higher
		   level code */
		LOG(0xC, "new_initiator");
		sccpu_new_initiator(aha->sc, self, initiator);
	}

	/* The right thing to do would be build an ior
	   and call the self->dev_ops->strategy routine,
	   but we cannot allocate it at interrupt level.
	   Also note that we are now disconnected from the
	   initiator, no way to do anything else with it
	   but reconnect and do what it wants us to do */

	/* obviously, this needs both spl and MP protection */
	self->dev_info.cpu.req_pending = TRUE;
	self->dev_info.cpu.req_id = mbi.mbt.mb_initiator_id;
	self->dev_info.cpu.req_lun = mbi.mbt.mb_lun;
	self->dev_info.cpu.req_cmd =
		mbi.mbt.mb_isa_send ? SCSI_CMD_SEND: SCSI_CMD_RECEIVE;
	len =	(mbi.mbt.mb_data_len_msb << 16) |
		(mbi.mbt.mb_data_len_mid << 8 );
	len += 0x100;/* truncation problem */
	self->dev_info.cpu.req_len = len;

	LOG(0xB,"tgt-mode-restart");
	(*self->dev_ops->restart)( aha->sc, self, FALSE);

	/* The call above has either prepared the data,
	   placing an ior on self, or it handled it some
	   other way */
	if (self->ior == 0)
		return;	/* I guess we'll do it later */

	{
		struct aha_ccb_raw	*rccb;

		rccb = &(aha->aha_ccbs[initiator->target_id]);
		rccb->active_target = initiator;
		if (self->cur_cmd == SCSI_CMD_SEND) {
			rccb->ccb.ccb_in = 1;
			rccb->ccb.ccb_out = 0;
		} else {
			rccb->ccb.ccb_in = 0;
			rccb->ccb.ccb_out = 1;
		}

		aha_prepare_rccb(initiator, rccb,
			(vm_offset_t)self->ior->io_data, self->ior->io_count);
		rccb->ccb.ccb_code = AHA_CCB_T_CMD;
		rccb->ccb.ccb_lun = initiator->lun;
		rccb->ccb.ccb_scsi_id = initiator->target_id;

		if (aha->wd.nactive++ == 0)
			aha->wd.watchdog_state = SCSI_WD_ACTIVE;

		aha_start_scsi(aha, &rccb->ccb);
	}
}

aha_initiator_intr(aha, mbi)
	aha_softc_t	aha;
	aha_mbox_t	mbi;
{
	struct aha_ccb_raw	*rccb;
	scsi2_status_byte_t	status;
	target_info_t		*tgt;

	rccb = mb_to_rccb(aha,mbi);
	tgt = rccb->active_target;
	rccb->active_target = 0;

	/* shortcut (sic!) */
	if (mbi.mb.mb_status == AHA_MBI_SUCCESS)
		goto allok;

	switch (rccb->ccb.ccb_hstatus) {
	case AHA_HST_SUCCESS:
allok:
		status = rccb->ccb.ccb_status;
		if (status.st.scsi_status_code != SCSI_ST_GOOD) {
			scsi_error(tgt, SCSI_ERR_STATUS, status.bits, 0);
			tgt->done = (status.st.scsi_status_code == SCSI_ST_BUSY) ?
				SCSI_RET_RETRY : SCSI_RET_NEED_SENSE;
		} else
			tgt->done = SCSI_RET_SUCCESS;
		break;
	case AHA_HST_SEL_TIMEO:
		if (tgt->flags & TGT_FULLY_PROBED)
			tgt->flags = 0; /* went offline */
		tgt->done = SCSI_RET_DEVICE_DOWN;
		break;
	case AHA_HST_DATA_OVRUN:
printf("aha: OVRUN\n");gimmeabreak();
		goto allok;
	case AHA_HST_BAD_DISCONN:
printf("aha: DISC\n");gimmeabreak();
		tgt->done = SCSI_RET_ABORTED;
		break;
	case AHA_HST_BAD_PHASE_SEQ:
		/* we'll get an interrupt soon */
		tgt->done = SCSI_RET_ABORTED;
printf("aha: PHASE\n");
		break;
	case AHA_HST_BAD_OPCODE:	/* fall through */
	case AHA_HST_BAD_PARAM:
printf("aha: BADCCB\n");gimmeabreak();
		tgt->done = SCSI_RET_RETRY;
		break;
	case AHA_HST_BAD_LINK_LUN:	/* these should not happen */
	case AHA_HST_INVALID_TDIR:
	case AHA_HST_DUPLICATED_CCB:
		printf("aha: bad hstatus (x%x)\n", rccb->ccb.ccb_hstatus);
		tgt->done = SCSI_RET_ABORTED;
		break;
	}

	LOG(8,"end");

	if (aha->wd.nactive-- == 1)
		aha->wd.watchdog_state = SCSI_WD_INACTIVE;

	if (tgt->ior) {
		LOG(0xA,"ops->restart");
		(*tgt->dev_ops->restart)( aha->sc, tgt, TRUE);
	}

	return FALSE;
}

/*
 * The bus was reset
 */
aha_bus_reset(aha)
	register aha_softc_t	aha;
{
	register 		port = aha->port;

	LOG(0x1d,"bus_reset");

	/*
	 * Clear bus descriptor
	 */
	aha->wd.nactive = 0;
	aha_reset(port, TRUE);
	aha_init(aha);

	printf("aha: (%d) bus reset ", ++aha->wd.reset_count);
	delay(scsi_delay_after_reset); /* some targets take long to reset */

	if (aha->sc == 0)	/* sanity */
		return;

	scsi_bus_was_reset(aha->sc);
}

/*
 * Watchdog
 *
 * We know that some (name withdrawn) disks get
 * stuck in the middle of dma phases...
 */
aha_reset_scsibus(aha)
	register aha_softc_t	aha;
{
	register target_info_t	*tgt;
	register 		port = aha->port;
	register int		i;

	for (i = 0; i < AHA_NCCB; i++) {
		tgt = aha->aha_ccbs[i].active_target;
		if (/*scsi_debug &&*/ tgt)
			printf("Target %d was active, cmd x%x in x%x out x%x\n", 
				tgt->target_id, tgt->cur_cmd,
				tgt->transient_state.in_count,
				tgt->transient_state.out_count);
	}
	aha_reset(port, FALSE);
	delay(35);
	/* no interrupt will come */
	aha_bus_reset(aha);
}

/*
 * Utilities
 */

/*
 * Send a command to the board along with some
 * optional parameters, optionally receive the
 * results at command completion, returns how
 * many bytes we did NOT get back.
 */
aha_command(port, cmd, outp, outc, inp, inc, clear_interrupt)
	unsigned char	*outp, *inp;
{
	register char   st;
	boolean_t	failed = TRUE;

	do {
		st = inb(AHA_STATUS_PORT(port));
	} while (st & AHA_CSR_DATAO_FULL);

	/* Output command and any data */
	outb(AHA_COMMAND_PORT(port), cmd);
	while (outc--) {
		do {
			st = inb(AHA_STATUS_PORT(port));
			if (st & AHA_CSR_CMD_ERR) goto out;
		} while (st & AHA_CSR_DATAO_FULL);

                outb(AHA_COMMAND_PORT(port), *outp++);
	}

	/* get any data */
	while (inc--) {
		do {
			st = inb(AHA_STATUS_PORT(port));
			if (st & AHA_CSR_CMD_ERR) goto out;
		} while ((st & AHA_CSR_DATAI_FULL) == 0);

		*inp++ = inb(AHA_DATA_PORT(port));
	}
	++inc;
	failed = FALSE;

	/* wait command complete */
	if (clear_interrupt) do {
		delay(1);
		st = inb(AHA_INTR_PORT(port));
	} while ((st & AHA_INTR_DONE) == 0);

out:
	if (clear_interrupt)
		outb(AHA_CONTROL_PORT(port), AHA_CTL_INTR_CLR);
	if (failed)
		printf("aha_command: error on (%x %x %x %x %x %x), status %x\n",
			port, cmd, outp, outc, inp, inc, st);
	return inc;
}

#include <vm/vm_kern.h>

/*
 * Allocate dynamically segment lists to
 * targets (for scatter/gather)
 * Its a max of 17*6=102 bytes per target.
 */
vm_offset_t	aha_seglist_next, aha_seglist_end;

aha_alloc_segment_list(tgt)
	target_info_t	*tgt;
{
#define	ALLOC_SIZE	(AHA_MAX_SEGLIST * sizeof(aha_seglist_t))

/* XXX locking */
	if ((aha_seglist_next + ALLOC_SIZE) > aha_seglist_end) {
		(void) kmem_alloc_wired(kernel_map, &aha_seglist_next, PAGE_SIZE);
		aha_seglist_end = aha_seglist_next + PAGE_SIZE;
	}
	tgt->dma_ptr = (char *)aha_seglist_next;
	aha_seglist_next += ALLOC_SIZE;
/* XXX locking */
}

#endif	NAHA > 0

