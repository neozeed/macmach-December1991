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
 * $Log:	rz_cpu.c,v $
 * Revision 2.2  91/08/24  12:27:47  af
 * 	Created.
 * 	[91/08/02  03:52:11  af]
 * 
 */
/*
 *	File: rz_cpu.c
 * 	Author: Alessandro Forin, Carnegie Mellon University
 *	Date:	7/91
 *
 *	Top layer of the SCSI driver: interface with the MI.
 *	This file contains operations specific to CPU-like devices.
 *
 * We handle here the case of simple devices which do not use any
 * sophisticated host-to-host communication protocol, they look
 * very much like degenerative cases of TAPE devices.
 *
 * For documentation and debugging, we also provide code to act like one.
 */

#include <mach/std_types.h>
#include <machine/machparam.h>		/* spl definitions */
#include <scsi/compat_30.h>

#include <scsi/scsi.h>
#include <scsi/scsi_defs.h>
#include <scsi/rz.h>

/*
 * This function decides which 'protocol' we well speak
 * to a cpu target. For now the decision is left to a
 * global var. XXXXXXX
 */
extern scsi_devsw_t scsi_host;
scsi_devsw_t	*scsi_cpu_protocol = /* later &scsi_host*/
			&scsi_devsw[SCSI_CPU];

sccpu_new_initiator(sc, self, initiator)
	scsi_softc_t	*sc;
	target_info_t	*self, *initiator;
{
	initiator->dev_ops = scsi_cpu_protocol;
	if (initiator == self) {
		self->flags = TGT_DID_SYNCH|TGT_FULLY_PROBED|TGT_ONLINE|
			     TGT_ALIVE|TGT_US;
		self->dev_info.cpu.req_pending = FALSE;
	} else {
		initiator->flags = TGT_DID_SYNCH|TGT_FULLY_PROBED|TGT_ONLINE|
			     TGT_ALIVE;
		initiator->dev_info.cpu.req_pending = TRUE;
	}
}

scsi_ret_t
sccpu_open(sc, tgt, req)
	scsi_softc_t	*sc;
	target_info_t	*tgt;
	io_req_t	req;
{
	return SCSI_RET_SUCCESS;	/* XXX if this is it, drop it */
}

scsi_ret_t
sccpu_close(sc, tgt)
	scsi_softc_t	*sc;
	target_info_t	*tgt;
{
	return SCSI_RET_SUCCESS;	/* XXX if this is it, drop it */
}

sccpu_strategy(ior)
	register io_req_t	ior;
{
	target_info_t  *tgt;
	register scsi_softc_t	*sc;
	scsi_ret_t      ret;
	register int    i = ior->io_unit;
	io_req_t	head, tail;

	sc = scsi_softc[rzcontroller(i)];
	tgt = sc->target[rzslave(i)];

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
		sccpu_start( sc, tgt, FALSE);
	}
	splx(i);

	return D_SUCCESS;
}


sccpu_start( sc, tgt, done)
	scsi_softc_t	*sc;
	target_info_t	*tgt;
	boolean_t	done;
{
	io_req_t		head, ior = tgt->ior;
	scsi_ret_t		ret;

	/* this is to the doc & debug code mentioned in the beginning */
	if (!done && tgt->dev_info.cpu.req_pending)
		return sccpu_act_as_target( sc, tgt);

	if (ior == 0)
		return;

	if (done) {

		/* see if we must retry */
		if ((tgt->done == SCSI_RET_RETRY) &&
		    ((ior->io_op & IO_INTERNAL) == 0)) {
			delay(1000000);/*XXX*/
			goto start;
		} else
		/* got a bus reset ? shouldn't matter */
		if (tgt->done == (SCSI_RET_ABORTED|SCSI_RET_RETRY)) {
			goto start;
		} else

		/* check completion status */

		if (tgt->cur_cmd == SCSI_CMD_REQUEST_SENSE) {
			scsi_sense_data_t *sns;

			if (scsi_debug) {
				unsigned int p[7];
				bcopy(tgt->cmd_ptr, (char*)p, sizeof(p));
				printf("rz_cpu sense data: %x %x %x %x %x %x %x, ",
					p[0],p[1],p[2],p[3],p[4],p[5],p[6]);
			}

			ior->io_op = ior->io_temporary;
			ior->io_error = D_IO_ERROR;
			ior->io_op |= IO_ERROR;

			sns = (scsi_sense_data_t *)tgt->cmd_ptr;
			if (scsi_check_sense_data(tgt, sns)) {
			    if (sns->u.xtended.ili) {
				if (ior->io_op & IO_READ) {
				    int residue;

				    residue =	sns->u.xtended.info0 << 24 |
						sns->u.xtended.info1 << 16 |
						sns->u.xtended.info2 <<  8 |
						sns->u.xtended.info3;
				    if (scsi_debug)
					printf("Cpu Short Read (%d)\n", residue);
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
		ret = scsi_receive( sc, tgt, ior );
	} else if ((ior->io_op & IO_INTERNAL) == 0) {
		tgt->flags |= TGT_WRITTEN_TO;
		ret = scsi_send( sc, tgt, ior );
	}
}

/*
 * This is a simple code to make us act as a dumb
 * processor type.  Use for debugging only.
 */
static struct io_req	sccpu_ior;

sccpu_act_as_target( sc, self)
	scsi_softc_t	*sc;
	target_info_t	*self;
{
	target_info_t	*tgt;
	static char	buffer[512] = "For debugging only";

	self->dev_info.cpu.req_pending = FALSE;
/*	tgt = sc->target[self->dev_info.cpu.req_id];*/
	sccpu_ior.io_next = 0;
	sccpu_ior.io_count = (512 < self->dev_info.cpu.req_len) ?
		512 : self->dev_info.cpu.req_len;
	sccpu_ior.io_data = buffer;
	if ((self->cur_cmd = self->dev_info.cpu.req_cmd) == SCSI_CMD_RECEIVE) {
		sccpu_ior.io_op = IO_READ;
	} else {
		sccpu_ior.io_op = IO_WRITE;
	}
	self->ior = &sccpu_ior;
}
