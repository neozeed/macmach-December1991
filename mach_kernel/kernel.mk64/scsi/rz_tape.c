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
 * $Log:	rz_tape.c,v $
 * Revision 2.9  91/08/24  12:28:15  af
 * 	Flag tapes as exclusive open, Spl defs, now we
 * 	understand fixed-size tapes (QIC-11), multiP locks.
 * 	[91/08/02  03:56:41  af]
 * 
 *	Cast args for bcopy.
 * 
 * Revision 2.8  91/06/25  20:56:33  rpd
 * 	Tweaks to make gcc happy.
 * 
 * Revision 2.7  91/06/19  11:57:12  rvb
 * 	File moved here from mips/PMAX since it is now "MI" code, also
 * 	used by Vax3100 and soon -- the omron luna88k.
 * 	[91/06/04            rvb]
 * 
 * Revision 2.6  91/05/14  17:27:00  mrt
 * 	Correcting copyright
 * 
 * Revision 2.5  91/05/13  06:34:29  af
 * 	Do not say we failed to bspfile when the target really was
 * 	only busy.
 * 	Retrieve from mode_sense speed, density and writeprotect info.
 * 	Deal with targets that are busy, note when we get confused due
 * 	to a scsi bus reset.  Set speed and density if user asks to.
 * 	Generally, made it work properly [it passes Rudy's tests].
 * 	Tapes really work now.
 * 
 * Revision 2.4.2.1  91/05/12  16:05:10  af
 * 	Do not say we failed to bspfile when the target really was
 * 	only busy.
 * 
 * 	Retrieve from mode_sense speed, density and writeprotect info.
 * 	Deal with targets that are busy, note when we get confused due
 * 	to a scsi bus reset.  Set speed and density if user asks to.
 * 	Generally, made it work properly [it passes Rudy's tests].
 * 
 * 	Tapes really work now.
 * 
 * Revision 2.4.1.3  91/04/05  13:10:07  af
 * 	Do not say we failed to bspfile when the target really was
 * 	only busy.
 * 
 * Revision 2.4.1.2  91/03/29  17:10:38  af
 * 	Retrieve from mode_sense speed, density and writeprotect info.
 * 	Deal with targets that are busy, note when we get confused due
 * 	to a scsi bus reset.  Set speed and density if user asks to.
 * 	Generally, made it work properly [it passes Rudy's tests].
 * 
 * Revision 2.4.1.1  91/02/21  19:00:22  af
 * 	Tapes really work now.
 * 
 * 
 * Revision 2.4  91/02/05  17:44:01  mrt
 * 	Added author notices
 * 	[91/02/04  11:17:11  mrt]
 * 
 * 	Changed to use new Mach copyright
 * 	[91/02/02  12:15:49  mrt]
 * 
 * Revision 2.3  90/12/05  23:34:04  af
 * 	Mild attempt to get it working.  Needs lots of work still.
 * 	[90/12/03  23:34:27  af]
 * 
 * Revision 2.1.1.1  90/11/01  03:44:11  af
 * 	Created.
 * 	[90/10/21            af]
 */
/*
 *	File: rz_tape.c
 * 	Author: Alessandro Forin, Carnegie Mellon University
 *	Date:	10/90
 *
 *	Top layer of the SCSI driver: interface with the MI.
 *	This file contains operations specific to TAPE-like devices.
 */

#include <mach/std_types.h>
#include <machine/machparam.h>		/* spl definitions */
#include <scsi/compat_30.h>
#include <machine/machparam.h>

#include <sys/ioctl.h>
#ifdef	MACH_KERNEL
#include <device/tape_status.h>
#else	/*MACH_KERNEL*/
#include <mips/PMAX/tape_status.h>
#endif	/*MACH_KERNEL*/

#include <scsi/scsi.h>
#include <scsi/scsi_defs.h>
#include <scsi/rz.h>

int scsi_tape_timeout = 5*60;	/* secs, tk50 is slow when positioning far apart */

sctape_open(sc, tgt, req)
	scsi_softc_t	*sc;
	target_info_t	*tgt;
	io_req_t	req;
{
	io_return_t	ret;
	io_req_t	ior;
	int		i;
	scsi_mode_sense_data_t *mod;

	req->io_device->flag |= D_EXCL_OPEN;

	/* Preferably allow tapes to disconnect */
	if (BGET(scsi_might_disconnect,sc->masterno,tgt->target_id))
		BSET(scsi_should_disconnect,sc->masterno,tgt->target_id);

	/*
	 * Dummy ior for proper sync purposes
	 */
	io_req_alloc(ior,0);
	ior->io_count = 0;

	/* Exabyte wants a mode sense first */
	do {
		ior->io_op = IO_INTERNAL;
		ior->io_next = 0;
		ret = scsi_mode_sense(sc, tgt, 0, 32, ior);
	} while (ret == SCSI_RET_RETRY);

	mod = (scsi_mode_sense_data_t *)tgt->cmd_ptr;
	if (scsi_debug) {
		int	p[5];
		bcopy((char*)mod, (char*)p, sizeof(p));
		printf("[modsns(%x): x%x x%x x%x x%x x%x]", ret,
			p[0], p[1], p[2], p[3], p[4]);
	}
	if (ret == SCSI_RET_DEVICE_DOWN)
		goto out;
	if (ret == SCSI_RET_SUCCESS) {
		tgt->dev_info.tape.read_only = mod->wp;
		tgt->dev_info.tape.speed = mod->speed;
		tgt->dev_info.tape.density = mod->bdesc[0].density_code;
	}

	/* Some tapes have limits on record-length */
again:
	ior->io_op = IO_INTERNAL;
	ior->io_next = 0;
	ior->io_error = 0;
	ret = scsi_read_block_limits( sc, tgt, ior);
	if (ret == SCSI_RET_RETRY) goto again;
	if (!ior->io_error && (ret == SCSI_RET_SUCCESS)) {
		scsi_blimits_data_t	*lim;
		int			maxl;

		lim = (scsi_blimits_data_t *) tgt->cmd_ptr;

		tgt->block_size = (lim->minlen_msb << 8) |
				   lim->minlen_lsb;

		maxl =	(lim->maxlen_msb << 16) |
			(lim->maxlen_sb  <<  8) |
			 lim->maxlen_lsb;
		if (maxl == 0)
			maxl = (unsigned)-1;
		tgt->dev_info.tape.maxreclen = maxl;
		tgt->dev_info.tape.fixed_size = (maxl == tgt->block_size);
	} else {
		/* let the user worry about it */
		tgt->dev_info.tape.maxreclen = (unsigned)-1;
		tgt->dev_info.tape.fixed_size = FALSE;
	}

	/* Try hard to do a mode select */
	for (i = 0; i < 5; i++) {
		ior->io_op = IO_INTERNAL;
		ret = sctape_mode_select(sc, tgt, 0, 0, FALSE, ior);
		if (ret == SCSI_RET_SUCCESS)
			break;
	}
	if (scsi_watchdog_period < scsi_tape_timeout)
		scsi_watchdog_period += scsi_tape_timeout;

#if 0	/* this might imply rewind, which we do not want, although yes, .. */
	/* we want the tape loaded */
	ior->io_op = IO_INTERNAL;
	ior->io_next = 0;
	ret = scsi_start_unit(sc, tgt, SCSI_CMD_SS_START, ior);
#endif

out:
	io_req_free(ior);
	return ret;
}


sctape_close(sc, tgt)
	scsi_softc_t	*sc;
	target_info_t	*tgt;
{
	io_return_t	ret = SCSI_RET_SUCCESS;
	io_req_t	ior;

	/*
	 * Dummy ior for proper sync purposes
	 */
	io_req_alloc(ior,0);
	ior->io_op = IO_INTERNAL;
	ior->io_next = 0;
	ior->io_count = 0;

	if (tgt->ior) printf("TAPE: Close with pending requests ?? \n");

	/* write a filemark if we xtnded/truncated the tape */
	if (tgt->flags & TGT_WRITTEN_TO) {
		tgt->ior = ior;
		ret = scsi_write_filemarks(sc, tgt, 2, ior);
		if (ret != SCSI_RET_SUCCESS)
			 printf("%s%d: wfmark failed x%x\n",
			 (*tgt->dev_ops->driver_name)(TRUE), tgt->target_id, ret);
		/*
		 * Don't bother repositioning if we'll rewind it
		 */
		if (tgt->flags & TGT_REWIND_ON_CLOSE)
			goto rew;
retry:
		tgt->ior = ior;
		ior->io_op = IO_INTERNAL;
		ior->io_next = 0;
		ret = scsi_space(sc, tgt, SCSI_CMD_SP_FIL, -1, ior);
		if (ret != SCSI_RET_SUCCESS) {
			if (ret == SCSI_RET_RETRY) {
				timeout(wakeup, tgt, hz);
				await(tgt);
				goto retry;
			}
			printf("%s%d: bspfile failed x%x\n",
			 (*tgt->dev_ops->driver_name)(TRUE), tgt->target_id, ret);
		}
	}
rew:
	if (tgt->flags & TGT_REWIND_ON_CLOSE) {
		/* Rewind tape */
		ior->io_error = 0;
		ior->io_op = IO_INTERNAL;
		tgt->ior = ior;
		(void) scsi_rewind(sc, tgt, ior, FALSE);
		iowait(ior);
		if (tgt->done == SCSI_RET_RETRY) {
			timeout(wakeup, tgt, 5*hz);
			await(tgt);
			goto rew;
		}
	}
		io_req_free(ior);

	tgt->flags &= ~(TGT_ONLINE|TGT_WRITTEN_TO|TGT_REWIND_ON_CLOSE);
	return ret;
}

sctape_strategy(ior)
	register io_req_t	ior;
{
	target_info_t  *tgt;
	register scsi_softc_t	*sc;
	scsi_ret_t      ret;
	register int    i = ior->io_unit;
	io_req_t	head, tail;

	sc = scsi_softc[rzcontroller(i)];
	tgt = sc->target[rzslave(i)];

	if (((ior->io_op & IO_READ) == 0) &&
	    tgt->dev_info.tape.read_only) {
		ior->io_error = D_INVALID_OPERATION;
		ior->io_op |= IO_ERROR;
		ior->io_residual = ior->io_count;
		iodone(ior);
		return ior->io_error;
	}

	ior->io_next = 0;
	ior->io_prev = 0;

	i = splbio();
	simple_lock(&tgt->target_lock);
	if (head = tgt->ior) {
		/* Queue it up at the end of the list */
		if (tail = head->io_prev)
			tail->io_next = ior;
		else
			head->io_next = ior;
		head->io_prev = ior;	/* tail pointer */
		simple_unlock(&tgt->target_lock);
	} else {
		/* Was empty, start operation */
		tgt->ior = ior;
		simple_unlock(&tgt->target_lock);
		sctape_start( sc, tgt, FALSE);
	}
	splx(i);

	return D_SUCCESS;
}

sctape_start( sc, tgt, done)
	scsi_softc_t	*sc;
	target_info_t	*tgt;
	boolean_t	done;
{
	io_req_t		head, ior = tgt->ior;
	scsi_ret_t		ret;

	if (ior == 0)
		return;

	if (done) {

		/* see if we must retry */
		if ((tgt->done == SCSI_RET_RETRY) &&
		    ((ior->io_op & IO_INTERNAL) == 0)) {
			delay(1000000);/*XXX*/
			goto start;
		} else
		/* got a bus reset ? ouch, that hurts */
		if (tgt->done == (SCSI_RET_ABORTED|SCSI_RET_RETRY)) {
			/*
			 * we really cannot retry because the tape position
			 * is lost.
			 */
			printf("Lost tape position\n");
			ior->io_error = D_IO_ERROR;
			ior->io_op |= IO_ERROR;
		} else

		/* check completion status */

		if (tgt->cur_cmd == SCSI_CMD_REQUEST_SENSE) {
			scsi_sense_data_t *sns;

			if (scsi_debug) {
				unsigned int p[7];
				bcopy(tgt->cmd_ptr,(char*)p,sizeof(p));
				printf("rz_tape sense data: %x %x %x %x %x %x %x, ",
					p[0],p[1],p[2],p[3],p[4],p[5],p[6]);
			}

			ior->io_op = ior->io_temporary;
			ior->io_error = D_IO_ERROR;
			ior->io_op |= IO_ERROR;

			sns = (scsi_sense_data_t *)tgt->cmd_ptr;
			if (scsi_check_sense_data(tgt, sns)) {
			    if (sns->u.xtended.eom) {
				ior->io_error = 0;
				ior->io_op &= ~IO_ERROR;
				ior->io_residual = ior->io_count;
				if (scsi_debug)
					printf("End of Physical Tape!\n");
			    } else if (sns->u.xtended.fm) {
				ior->io_error = 0;
				ior->io_op &= ~IO_ERROR;
				ior->io_residual = ior->io_count;
				if (scsi_debug)
					printf("File Mark\n");
			    } else if (sns->u.xtended.ili) {
				if (ior->io_op & IO_READ) {
				    int residue;

				    residue =	sns->u.xtended.info0 << 24 |
						sns->u.xtended.info1 << 16 |
						sns->u.xtended.info2 <<  8 |
						sns->u.xtended.info3;
				    if (scsi_debug)
					printf("Tape Short Read (%d)\n", residue);
				    /*
				     * NOTE: residue == requested - actual
				     * We only care if > 0
				     */
				    if (residue < 0) residue = 0;/* sanity */
				    ior->io_residual += residue;
				    ior->io_error = 0;
				    ior->io_op &= ~IO_ERROR;
				    /* goto ok */
				}
			    }
			}
		}

		else if (tgt->done != SCSI_RET_SUCCESS) {

		    if (tgt->done == SCSI_RET_NEED_SENSE) {

			ior->io_temporary = ior->io_op;
			ior->io_op = IO_INTERNAL;
			if (scsi_debug)
				printf("[NeedSns x%x x%x]", ior->io_residual, ior->io_count);
			scsi_request_sense(sc, tgt, ior, 0);
			return;

		    } else if (tgt->done == SCSI_RET_RETRY) {
			/* only retry here READs and WRITEs */
			if ((ior->io_op & IO_INTERNAL) == 0) {
				ior->io_residual = 0;
				goto start;
			} else{
				ior->io_error = D_WOULD_BLOCK;
				ior->io_op |= IO_ERROR;
			}
		    } else {
			ior->io_error = D_IO_ERROR;
			ior->io_op |= IO_ERROR;
		    }
		}

		if (scsi_debug)
			printf("[Resid x%x]", ior->io_residual);

		/* dequeue next one */
		head = ior;

		simple_lock(&tgt->target_lock);
		ior = head->io_next;
		tgt->ior = ior;
		if (ior)
			ior->io_prev = head->io_prev;
		simple_unlock(&tgt->target_lock);

		iodone(head);

		if (ior == 0)
			return;
	}
	ior->io_residual = 0;
start:
	if (ior->io_op & IO_READ) {
		tgt->flags &= ~TGT_WRITTEN_TO;
		ret = sctape_read( sc, tgt, ior );
	} else if ((ior->io_op & IO_INTERNAL) == 0) {
		tgt->flags |= TGT_WRITTEN_TO;
		ret = sctape_write( sc, tgt, ior );
	}
}

io_return_t
sctape_get_status(sc, tgt, flavor, status, status_count)
	scsi_softc_t	*sc;
	target_info_t	*tgt;
	int		flavor;
	dev_status_t	status;
	unsigned int	*status_count;
{
	switch (flavor) {
	case TAPE_STATUS: {
		struct tape_status *ts = (struct tape_status *) status;

		ts->mt_type		= MT_ISSCSI;
		ts->speed		= tgt->dev_info.tape.speed;
		ts->density		= tgt->dev_info.tape.density;
		ts->flags		= (tgt->flags & TGT_REWIND_ON_CLOSE) ?
						TAPE_FLG_REWIND : 0;
		if (tgt->dev_info.tape.read_only)
			ts->flags |= TAPE_FLG_WP;
#ifdef	MACH_KERNEL
		*status_count = TAPE_STATUS_COUNT;
#endif	MACH_KERNEL

		break;
	    }
	/* U*x compat */
	case MTIOCGET: {
		struct mtget *g = (struct mtget *) status;

		bzero(g, sizeof(struct mtget));
		g->mt_type = 0x7;	/* Ultrix compat */
#ifdef	MACH_KERNEL
		*status_count = sizeof(struct mtget)/sizeof(int);
#endif	MACH_KERNEL
		break;
	    }
	default:
		return D_INVALID_OPERATION;
	}
	return D_SUCCESS;
}

io_return_t
sctape_set_status(sc, tgt, flavor, status, status_count)
	scsi_softc_t	*sc;
	target_info_t	*tgt;
	int		flavor;
	dev_status_t	status;
	unsigned int	status_count;
{
	scsi_ret_t		ret;

	switch (flavor) {
	case TAPE_STATUS: {
		struct tape_status *ts = (struct tape_status *) status;
		if (ts->flags & TAPE_FLG_REWIND)
			tgt->flags |= TGT_REWIND_ON_CLOSE;
		else
			tgt->flags &= ~TGT_REWIND_ON_CLOSE;

		if (ts->speed || ts->density) {
			unsigned int ospeed, odensity;
			io_req_t	ior;

			io_req_alloc(ior,0);
			ior->io_op = IO_INTERNAL;
			ior->io_next = 0;
			ior->io_count = 0;

			ospeed = tgt->dev_info.tape.speed;
			odensity = tgt->dev_info.tape.density;
			tgt->dev_info.tape.speed = ts->speed;
			tgt->dev_info.tape.density = ts->density;

			ret = sctape_mode_select(sc, tgt, 0, 0, (ospeed == ts->speed), ior);
			if (ret != SCSI_RET_SUCCESS) {
				tgt->dev_info.tape.speed = ospeed;
				tgt->dev_info.tape.density = odensity;
			}

			io_req_free(ior);
		}

		break;
	    }
	/* U*x compat */
	case MTIOCTOP: {
		struct tape_params *mt = (struct tape_params *) status;
		io_req_t	ior;

		if (scsi_debug)
			printf("[sctape_sstatus: %x %x %x]\n",
				flavor, mt->mt_operation, mt->mt_repeat_count);

		io_req_alloc(ior,0);
retry:
		ior->io_count = 0;
		ior->io_op = IO_INTERNAL;
		ior->io_next = 0;
		tgt->ior = ior;

		/* compat: in U*x it is a short */
		switch ((short)(mt->mt_operation)) {
		case MTWEOF:	/* write an end-of-file record */
			ret = scsi_write_filemarks(sc, tgt, mt->mt_repeat_count, ior);
			break;
		case MTFSF:	/* forward space file */
			ret = scsi_space(sc, tgt, SCSI_CMD_SP_FIL, mt->mt_repeat_count, ior);
			break;
		case MTBSF:	/* backward space file */
			ret = scsi_space(sc, tgt, SCSI_CMD_SP_FIL, -mt->mt_repeat_count,ior);
			break;
		case MTFSR:	/* forward space record */
			ret = scsi_space(sc, tgt, SCSI_CMD_SP_BLOCKS, mt->mt_repeat_count, ior);
			break;
		case MTBSR:	/* backward space record */
			ret = scsi_space(sc, tgt, SCSI_CMD_SP_BLOCKS, -mt->mt_repeat_count, ior);
			break;
		case MTREW:	/* rewind */
		case MTOFFL:	/* rewind and put the drive offline */
			ret = scsi_rewind(sc, tgt, ior, TRUE);
			iowait(ior);
			if ((short)(mt->mt_operation) == MTREW) break;
			ior->io_op = 0;
			ior->io_next = 0;
			(void) scsi_start_unit(sc, tgt, 0, ior);
			break;
		case MTNOP:	/* no operation, sets status only */
		case MTCACHE:	/* enable controller cache */
		case MTNOCACHE:	/* disable controller cache */
			ret = SCSI_RET_SUCCESS;
			break;
		default:
			tgt->ior = 0;
			io_req_free(ior);
			return D_INVALID_OPERATION;
		}

		if (ret == SCSI_RET_RETRY) {
			timeout(wakeup, ior, 5*hz);
			await(ior);
			goto retry;
		}

		io_req_free(ior);
		if (ret != SCSI_RET_SUCCESS)
			return D_IO_ERROR;
		break;
	}
	case MTIOCIEOT:
	case MTIOCEEOT:
	default:
		return D_INVALID_OPERATION;
	}
	return D_SUCCESS;
}

