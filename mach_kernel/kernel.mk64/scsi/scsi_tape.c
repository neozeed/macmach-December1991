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
 * $Log:	scsi_tape.c,v $
 * Revision 2.10  91/08/24  12:29:16  af
 * 	Support for fixed-size-tapes: careful about speed selections,
 * 	round sizes properly, use fixed bit.
 * 	[91/08/02  03:58:18  af]
 * 
 * Revision 2.9  91/06/19  11:58:13  rvb
 * 	File moved here from mips/PMAX since it is now "MI" code, also
 * 	used by Vax3100 and soon -- the omron luna88k.
 * 	[91/06/04            rvb]
 * 
 * Revision 2.8  91/05/14  17:31:26  mrt
 * 	Correcting copyright
 * 
 * Revision 2.7  91/05/13  06:35:55  af
 * 	Cleaned up mode_select, vendor unique data can be passed
 * 	in if necessary.  Density and speed are taken from target 
 * 	structure.  Added rewind command and all th eothers that
 * 	were missing (but what we do not use is disabled).
 * 
 * Revision 2.6.2.1  91/05/12  16:25:01  af
 * 	Cleaned up mode_select, vendor unique data can be passed
 * 	in if necessary.  Density and speed are taken from target 
 * 	structure.  Added rewind command and all th eothers that
 * 	were missing (but what we do not use is disabled).
 * 
 * Revision 2.6.1.1  91/03/29  17:26:59  af
 * 	Cleaned up mode_select, vendor unique data can be passed
 * 	in if necessary.  Density and speed are taken from target 
 * 	structure.  Added rewind command and all th eothers that
 * 	were missing (but what we do not use is disabled).
 * 
 * 
 * Revision 2.6  91/02/05  17:46:24  mrt
 * 	Added author notices
 * 	[91/02/04  11:20:18  mrt]
 * 
 * 	Changed to use new Mach copyright
 * 	[91/02/02  12:19:07  mrt]
 * 
 * Revision 2.5  90/12/05  23:35:49  af
 * 	Mild attempt to get it working.  Actually wrote and read
 * 	back a tape.
 * 	[90/12/03  23:49:49  af]
 * 
 * Revision 2.3.1.1  90/11/01  03:40:30  af
 * 	Created, from the SCSI specs:
 * 	"Small Computer Systems Interface (SCSI)", ANSI Draft
 * 	X3T9.2/82-2 - Rev 17B December 1985
 * 	"Small Computer System Interface - 2 (SCSI-II)", ANSI Draft
 * 	X3T9.2/86-109 -  Rev 10C March 1990
 * 	[90/10/11            af]
 */
/*
 *	File: scsi_tape.c
 * 	Author: Alessandro Forin, Carnegie Mellon University
 *	Date:	10/90
 *
 *	Middle layer of the SCSI driver: SCSI protocol implementation
 *
 * This file contains code for SCSI commands for SEQUENTIAL ACCESS devices.
 */

#include <mach/std_types.h>
#include <scsi/compat_30.h>

#include <scsi/scsi.h>
#include <scsi/scsi_defs.h>

char *sctape_name(internal)
	boolean_t	internal;
{
	return internal ? "tz" : "tape";
}

sctape_optimize(sc, tgt)
	scsi_softc_t		*sc;
	target_info_t		*tgt;
{
	register int 	i;
	char		result[6];

	/* Some (DEC) tapes want to send you the self-test results */
	for (i = 0; i < 10; i++) {
		if (scsi_receive_diag( sc, tgt, result, sizeof(result), 0)
		    == SCSI_RET_SUCCESS)
			break;
	}
	if (scsi_debug)
		printf("[tape_rcvdiag: after %d, x%x x%x x%x x%x x%x x%x]\n", i+1,
		result[0], result[1], result[2], result[3], result[4], result[5]);
}

/*
 * SCSI commands specific to sequential access devices
 */
sctape_mode_select( sc, tgt, vuque_data, vuque_data_len, newspeed, ior)
	register scsi_softc_t	*sc;
	register target_info_t	*tgt;
	unsigned char		*vuque_data;
	int			vuque_data_len;
	io_req_t		ior;
{
	scsi_cmd_mode_select_t	*cmd;
	scsi_mode_select_param_t	*parm;
	register int			offs;

	bzero(tgt->cmd_ptr, sizeof(*cmd) + 2 * sizeof(*parm));
	cmd = (scsi_cmd_mode_select_t*) (tgt->cmd_ptr);
	cmd->scsi_cmd_code = SCSI_CMD_MODE_SELECT;
	cmd->scsi_cmd_lun_and_lba1 = 0;
	cmd->scsi_cmd_xfer_len = sizeof(scsi_mode_select_param_t) + vuque_data_len;

	parm = (scsi_mode_select_param_t*) (cmd + 1);
	if (newspeed) {
		parm->speed = tgt->dev_info.tape.speed;
		parm->descs[0].density_code = tgt->dev_info.tape.density;
	} else {
		parm->speed = 0;
		parm->descs[0].density_code = 0;
	}
	parm->buffer_mode = 1;
	parm->desc_len = 8;

	if (vuque_data_len)
		bcopy(vuque_data, (char*)(parm+1), vuque_data_len);

	tgt->cur_cmd = SCSI_CMD_MODE_SELECT;

	scsi_go_and_wait(sc, tgt, sizeof(*cmd) + sizeof(*parm) + vuque_data_len, 0, ior);

	return tgt->done;
}

sctape_read( sc, tgt, ior)
	register scsi_softc_t	*sc;
	register target_info_t	*tgt;
	io_req_t		ior;
{
	scsi_cmd_read_t		*cmd;
	register unsigned	len, max;
	boolean_t		fixed = FALSE;

	len = ior->io_count;
	max = sc->max_dma_data;
	if (tgt->dev_info.tape.fixed_size) {
		fixed = TRUE;
		ior->io_residual = len % tgt->block_size;
		len = len / tgt->block_size;
	} else {
		if (max > tgt->dev_info.tape.maxreclen)
			max = tgt->dev_info.tape.maxreclen;
		if (len > max) {
			ior->io_residual = len - max;
			len = max;
		}
		if (len < tgt->block_size)
			len = tgt->block_size;
	}

	cmd = (scsi_cmd_read_t*) (tgt->cmd_ptr);
	cmd->scsi_cmd_code = SCSI_CMD_READ;
	cmd->scsi_cmd_lun_and_lba1 = fixed ? SCSI_CMD_TP_FIXED : 0;
	cmd->scsi_cmd_lba2 	   = len >> 16;
	cmd->scsi_cmd_lba3 	   = len >>  8;
	cmd->scsi_cmd_xfer_len     = len;
	cmd->scsi_cmd_ctrl_byte = 0;	/* not linked */
	
	tgt->cur_cmd = SCSI_CMD_READ;

	(*sc->go)(sc, tgt, sizeof(*cmd), len, FALSE);
}

sctape_write( sc, tgt, ior)
	register scsi_softc_t	*sc;
	register target_info_t	*tgt;
	io_req_t		ior;
{
	scsi_cmd_write_t	*cmd;
	register unsigned	len, max;
	boolean_t		fixed = FALSE;

	len = ior->io_count;
	max = sc->max_dma_data;
	if (tgt->dev_info.tape.fixed_size) {
		fixed = TRUE;
		ior->io_residual = len % tgt->block_size;
		len = len / tgt->block_size;
	} else {
		if (max > tgt->dev_info.tape.maxreclen)
			max = tgt->dev_info.tape.maxreclen;
		if (len > max) {
			ior->io_residual = len - max;
			len = max;
		}
		if (len < tgt->block_size)
			len = tgt->block_size;
	}

	cmd = (scsi_cmd_write_t*) (tgt->cmd_ptr);
	cmd->scsi_cmd_code = SCSI_CMD_WRITE;
	cmd->scsi_cmd_lun_and_lba1 = fixed ? SCSI_CMD_TP_FIXED : 0;
	cmd->scsi_cmd_lba2 	   = len >> 16;
	cmd->scsi_cmd_lba3 	   = len >>  8;
	cmd->scsi_cmd_xfer_len     = len;
	cmd->scsi_cmd_ctrl_byte = 0;	/* not linked */
	
	tgt->cur_cmd = SCSI_CMD_WRITE;

	(*sc->go)(sc, tgt, sizeof(*cmd), 0, FALSE);
}

scsi_rewind( sc, tgt, ior, wait)
	register scsi_softc_t	*sc;
	register target_info_t	*tgt;
	io_req_t		ior;
	boolean_t		wait;
{
	scsi_cmd_rewind_t	*cmd;


	cmd = (scsi_cmd_rewind_t*) (tgt->cmd_ptr);
	cmd->scsi_cmd_code = SCSI_CMD_REWIND;
	cmd->scsi_cmd_lun_and_lba1 = wait ? 0 : SCSI_CMD_REW_IMMED;
	cmd->scsi_cmd_lba2 	   = 0;
	cmd->scsi_cmd_lba3 	   = 0;
	cmd->scsi_cmd_xfer_len     = 0;
	cmd->scsi_cmd_ctrl_byte = 0;	/* not linked */
	
	tgt->cur_cmd = SCSI_CMD_REWIND;

	(*sc->go)(sc, tgt, sizeof(*cmd), 0, FALSE);
	return SCSI_RET_SUCCESS;
}

scsi_write_filemarks( sc, tgt, count, ior)
	register scsi_softc_t	*sc;
	register target_info_t	*tgt;
	register unsigned int	count;
	io_req_t		ior;
{
	scsi_cmd_write_fil_t	*cmd;

	cmd = (scsi_cmd_write_fil_t*) (tgt->cmd_ptr);
	cmd->scsi_cmd_code = SCSI_CMD_WRITE_FILEMARKS;
	cmd->scsi_cmd_lun_and_lba1 = 0;
	cmd->scsi_cmd_lba2 	   = count >> 16;
	cmd->scsi_cmd_lba3 	   = count >>  8;
	cmd->scsi_cmd_xfer_len     = count;
	cmd->scsi_cmd_ctrl_byte = 0;	/* not linked */


	tgt->cur_cmd = SCSI_CMD_WRITE_FILEMARKS;

	scsi_go_and_wait(sc, tgt, sizeof(*cmd), 0, ior);

	return tgt->done;
}

scsi_space( sc, tgt, mode, count, ior)
	register scsi_softc_t	*sc;
	register target_info_t	*tgt;
	register int		count;
	io_req_t		ior;
{
	scsi_cmd_space_t	*cmd;

	cmd = (scsi_cmd_space_t*) (tgt->cmd_ptr);
	cmd->scsi_cmd_code = SCSI_CMD_SPACE;
	cmd->scsi_cmd_lun_and_lba1 = mode & 0x3;
	cmd->scsi_cmd_lba2 	   = count >> 16;
	cmd->scsi_cmd_lba3 	   = count >>  8;
	cmd->scsi_cmd_xfer_len     = count;
	cmd->scsi_cmd_ctrl_byte = 0;	/* not linked */


	tgt->cur_cmd = SCSI_CMD_SPACE;

	scsi_go_and_wait(sc, tgt, sizeof(*cmd), 0, ior);

	return tgt->done;
}


scsi_read_block_limits( sc, tgt, ior)
	register scsi_softc_t	*sc;
	register target_info_t	*tgt;
	io_req_t		ior;
{
	scsi_cmd_block_limits_t	*cmd;

	cmd = (scsi_cmd_block_limits_t*) (tgt->cmd_ptr);
	cmd->scsi_cmd_code = SCSI_CMD_READ_BLOCK_LIMITS;
	cmd->scsi_cmd_lun_and_lba1 = 0;
	cmd->scsi_cmd_lba2 	   = 0;
	cmd->scsi_cmd_lba3 	   = 0;
	cmd->scsi_cmd_xfer_len     = 0;
	cmd->scsi_cmd_ctrl_byte = 0;	/* not linked */


	tgt->cur_cmd = SCSI_CMD_READ_BLOCK_LIMITS;

	scsi_go_and_wait(sc, tgt, sizeof(*cmd), sizeof(scsi_blimits_data_t), ior);
	return tgt->done;
}

#if 0 /* unused */

scsi_track_select( sc, tgt, trackno, ior)
	register scsi_softc_t	*sc;
	register target_info_t	*tgt;
	register unsigned char	trackno;
	io_req_t		ior;
{
	scsi_cmd_seek_t	*cmd;

	cmd = (scsi_cmd_seek_t*) (tgt->cmd_ptr);
	cmd->scsi_cmd_code = SCSI_CMD_TRACK_SELECT;
	cmd->scsi_cmd_lun_and_lba1 = 0;
	cmd->scsi_cmd_lba2 	   = 0;
	cmd->scsi_cmd_lba3 	   = 0;
	cmd->scsi_cmd_tp_trackno   = trackno;
	cmd->scsi_cmd_ctrl_byte = 0;	/* not linked */


	tgt->cur_cmd = SCSI_CMD_TRACK_SELECT;

	scsi_go_and_wait(sc, tgt, sizeof(*cmd), 0, ior);

}

scsi_read_reverse( sc, tgt, ior)
	register scsi_softc_t	*sc;
	register target_info_t	*tgt;
	io_req_t		ior;
{
	scsi_cmd_rev_read_t	*cmd;
	register unsigned	len;

	len = ior->io_count;
	if (len > sc->max_dma_data)
		len = sc->max_dma_data;

	cmd = (scsi_cmd_rev_read_t*) (tgt->cmd_ptr);
	cmd->scsi_cmd_code = SCSI_CMD_READ_REVERSE;
	cmd->scsi_cmd_lun_and_lba1 = 0;
	cmd->scsi_cmd_lba2 	   = len >> 16;
	cmd->scsi_cmd_lba3 	   = len >>  8;
	cmd->scsi_cmd_xfer_len     = len;
	cmd->scsi_cmd_ctrl_byte = 0;	/* not linked */
	
	tgt->cur_cmd = SCSI_CMD_READ_REVERSE;

	(*sc->go)(sc, tgt, sizeof(*cmd), len, FALSE);
}

sctape_verify( sc, tgt, len, ior)
	register scsi_softc_t	*sc;
	register target_info_t	*tgt;
	register unsigned int	len;
	io_req_t		ior;
{
	scsi_cmd_verify_t	*cmd;

	cmd = (scsi_cmd_verify_t*) (tgt->cmd_ptr);
	cmd->scsi_cmd_code = SCSI_CMD_VERIFY_0;
	cmd->scsi_cmd_lun_and_lba1 = 0;/* XXX */
	cmd->scsi_cmd_lba2 	   = len >> 16;
	cmd->scsi_cmd_lba3 	   = len >>  8;
	cmd->scsi_cmd_xfer_len     = len;
	cmd->scsi_cmd_ctrl_byte = 0;	/* not linked */


	tgt->cur_cmd = SCSI_CMD_VERIFY_0;

	scsi_go_and_wait(sc, tgt, sizeof(*cmd), 0, ior);

}


scsi_recover_buffered_data( sc, tgt, ior)
	register scsi_softc_t	*sc;
	register target_info_t	*tgt;
	io_req_t		ior;
{
	scsi_cmd_recover_buffer_t	*cmd;
	register unsigned		len;

	len = ior->io_count;
	if (len > sc->max_dma_data)
		len = sc->max_dma_data;

	cmd = (scsi_cmd_recover_buffer_t*) (tgt->cmd_ptr);
	cmd->scsi_cmd_code = SCSI_CMD_RECOVER_BUFFERED_DATA;
	cmd->scsi_cmd_lun_and_lba1 = 0;
	cmd->scsi_cmd_lba2 	   = len >> 16;
	cmd->scsi_cmd_lba3 	   = len >>  8;
	cmd->scsi_cmd_xfer_len     = len;
	cmd->scsi_cmd_ctrl_byte = 0;	/* not linked */
	
	tgt->cur_cmd = SCSI_CMD_RECOVER_BUFFERED_DATA;

	(*sc->go)(sc, tgt, sizeof(*cmd), len, FALSE);
}

scsi_erase( sc, tgt, mode, ior)
	register scsi_softc_t	*sc;
	register target_info_t	*tgt;
	io_req_t		ior;
{
	scsi_cmd_erase_t	*cmd;

	cmd = (scsi_cmd_erase_t*) (tgt->cmd_ptr);
	cmd->scsi_cmd_code = SCSI_CMD_ERASE;
	cmd->scsi_cmd_lun_and_lba1 = mode & SCSI_CMD_ER_LONG;
	cmd->scsi_cmd_lba2 	   = 0;
	cmd->scsi_cmd_lba3 	   = 0;
	cmd->scsi_cmd_xfer_len     = 0;
	cmd->scsi_cmd_ctrl_byte = 0;	/* not linked */


	tgt->cur_cmd = SCSI_CMD_ERASE;

	scsi_go_and_wait(sc, tgt, sizeof(*cmd), 0, ior);

}

#endif

#ifdef	SCSI2
scsi_locate
scsi_read_position
#endif	SCSI2
