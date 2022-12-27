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
 * $Log:	scsi_alldevs.c,v $
 * Revision 2.8  91/08/24  12:28:29  af
 * 	Revised scsi_inquiry to try selecting with ATN too,
 * 	a certain target insults us ("dumb initiator") if not.
 * 	[91/08/02  03:48:38  af]
 * 
 * Revision 2.7  91/06/19  11:57:31  rvb
 * 	File moved here from mips/PMAX since it is now "MI" code, also
 * 	used by Vax3100 and soon -- the omron luna88k.
 * 	[91/06/04            rvb]
 * 
 * Revision 2.6  91/05/14  17:29:48  mrt
 * 	Correcting copyright
 * 
 * Revision 2.5  91/05/13  06:05:30  af
 * 	Redone the request_sense command, split out function
 * 	that checks the sense data.
 * 	Rid of some embarassing crap use of identify messages.
 * 	Redone the scsi_error() routine.
 * 	[91/05/12  16:23:25  af]
 * 
 * Revision 2.4.1.1  91/03/29  17:16:16  af
 * 	Redone the request_sense command, split out function
 * 	that checks the sense data.
 * 	Rid of some embarassing crap use of identify messages.
 * 	Redone the scsi_error() routine.
 * 
 * Revision 2.4  91/02/05  17:45:27  mrt
 * 	Added author notices
 * 	[91/02/04  11:19:11  mrt]
 * 
 * 	Changed to use new Mach copyright
 * 	[91/02/02  12:17:51  mrt]
 * 
 * Revision 2.3  90/12/05  23:34:58  af
 * 	Turned debugging off.
 * 	[90/12/03  23:46:29  af]
 * 
 * Revision 2.1.1.1  90/11/01  03:39:35  af
 * 	Created, from the SCSI specs:
 * 	"Small Computer Systems Interface (SCSI)", ANSI Draft
 * 	X3T9.2/82-2 - Rev 17B December 1985
 * 	"Small Computer System Interface - 2 (SCSI-II)", ANSI Draft
 * 	X3T9.2/86-109 -  Rev 10C March 1990
 * 	[90/10/11            af]
 */
/*
 *	File: scsi_alldevs.c
 * 	Author: Alessandro Forin, Carnegie Mellon University
 *	Date:	10/90
 *
 *	Middle layer of the SCSI driver: SCSI protocol implementation
 *	This file contains code for SCSI commands defined for all device types.
 */

#include <mach/std_types.h>
#include <sys/types.h>
#include <scsi/compat_30.h>

#include <scsi/scsi.h>
#include <scsi/scsi2.h>
#include <scsi/scsi_defs.h>

/*
 * Utilities
 */
void scsi_go_and_wait(sc, tgt, insize, outsize, ior)
	scsi_softc_t	*sc;
	target_info_t	*tgt;
	io_req_t		ior;
{
	tgt->ior = ior;

	(*sc->go)(sc, tgt, insize, outsize, ior==0);

	if (ior)
		iowait(ior);
	else
		while (tgt->done == SCSI_RET_IN_PROGRESS);
}

/*
 * INQUIRY (Almost mandatory)
 */
scsi_inquiry( sc, tgt, pagecode)
	register scsi_softc_t	*sc;
	register target_info_t	*tgt;
	int			pagecode;
{
	scsi_cmd_inquiry_t	*cmd;
	boolean_t		no_ify = TRUE;

retry:
	cmd = (scsi_cmd_inquiry_t*) (tgt->cmd_ptr);
	cmd->scsi_cmd_code = SCSI_CMD_INQUIRY;
	cmd->scsi_cmd_lun_and_lba1 = 0;
	cmd->scsi_cmd_lba3 = 0;
	cmd->scsi_cmd_xfer_len = 0xff;	/* max len always */
	cmd->scsi_cmd_ctrl_byte = 0;	/* not linked */
/*#ifdef	SCSI2*/
	if (pagecode != SCSI_INQ_STD_DATA) {
		cmd->scsi_cmd_lun_and_lba1 |= SCSI_CMD_INQ_EVPD;
		cmd->scsi_cmd_page_code = pagecode;
	} else
/*#endif	SCSI2*/
		cmd->scsi_cmd_page_code = 0;

	tgt->cur_cmd = SCSI_CMD_INQUIRY;

	/*
	 * Note: this is sent when we do not know much about the
	 * target, so we might not put an identify message upfront
	 */
	(*sc->go)(sc, tgt, sizeof(*cmd), 0xff, no_ify);

	/*
	 * This spin loop is because we are called at autoconf
	 * time where we cannot thread_block(). Sigh.
	 */
	while (tgt->done == SCSI_RET_IN_PROGRESS) ;
	if (tgt->done == SCSI_RET_RETRY)	/* sync negotiation ? */
		goto retry;
	if ((tgt->done != SCSI_RET_SUCCESS) && no_ify) {
		no_ify = FALSE;
		goto retry;
	}
	return tgt->done;
}

scsi_print_inquiry( inq, pagecode, result)
	scsi2_inquiry_data_t	*inq;
	char			*result;
{
	static char *periph_names[10] = {
		"disk", "tape", "printer", "processor", "WORM-disk",
		"CD-ROM", "scanner", "memory", "jukebox", "communication"
	};
	static char *periph_state[4] = {
		"online", "offline", "?", "absent"
	};

	char dev[SCSI_TARGET_NAME_LEN], *devname;
	register int i, j = 0;

	if (pagecode != SCSI_INQ_STD_DATA)
		return;

	devname = result ? result : dev;

	if (!result) {
		printf("\n\t%s%s %s (%s %x)",
			(inq->rmb) ? "" : "non-", "removable SCSI",
			(inq->periph_type > 10) ?
				"?device?" : periph_names[inq->periph_type],
			periph_state[inq->periph_qual & 0x3],
			inq->device_type);
		printf("\n\t%s%s%s",
			inq->iso ? "ISO-compliant, " : "",
			inq->ecma ? "ECMA-compliant, " : "",
			inq->ansi ? "ANSI-compliant, " : "");
		if (inq->ansi)
			printf("%s%d, ", "SCSI-", inq->ansi);
		if (inq->response_fmt == 2)
			printf("%s%s%s%s%s%s%s%s%s%s%s", "Supports: ",
			inq->aenc ? "AENC, " : "",
			inq->trmIOP ? "TrmIOP, " : "",
			inq->RelAdr ? "RelAdr, " : "",
			inq->Wbus32 ? "32 bit xfers, " : "",
			inq->Wbus16 ? "16 bis xfers, " : "",
			inq->Sync ? "Sync xfers, " : "",
			inq->Linked ? "Linked cmds, " : "",
			inq->CmdQue ? "Tagged cmd queues, " : "",
			inq->SftRe ? "Soft" : "Hard", " RESET, ");
	}

	for (i = 0; i < 8; i++)
		if (inq->vendor_id[i] != ' ')
			devname[j++] = inq->vendor_id[i];
	devname[j++] = ' ';
	for (i = 0; i < 16; i++)
		if (inq->product_id[i] != ' ')
			devname[j++] = inq->product_id[i];
	devname[j++] = ' ';
	for (i = 0; i < 4; i++)
		if (inq->product_rev[i] != ' ')
			devname[j++] = inq->product_rev[i];
#if unsafe
	devname[j++] = ' ';
	for (i = 0; i < 8; i++)
		if (inq->vendor_uqe[i] != ' ')
			devname[j++] = inq->vendor_uqe[i];
#endif
	devname[j] = 0;

	if (!result)
		printf("(%s, %s%s)\n", devname, "SCSI ",
			(inq->periph_type > 10) ?
				"?device?" : periph_names[inq->periph_type]);
}

/*
 * REQUESTE SENSE (Mandatory, All)
 */

scsi_request_sense( sc, tgt, ior, data)
	register scsi_softc_t	*sc;
	register target_info_t	*tgt;
	io_req_t		ior;
	char			**data;
{
	scsi_cmd_request_sense_t *cmd;

	cmd = (scsi_cmd_request_sense_t *) (tgt->cmd_ptr);
	cmd->scsi_cmd_code = SCSI_CMD_REQUEST_SENSE;
	cmd->scsi_cmd_lun_and_lba1 = 0;
	cmd->scsi_cmd_lba2 = 0;
	cmd->scsi_cmd_lba3 = 0;
	cmd->scsi_cmd_allocation_length = 0xff;	/* max len always */
	cmd->scsi_cmd_ctrl_byte = 0;	/* not linked */

	tgt->cur_cmd = SCSI_CMD_REQUEST_SENSE;

	if (ior==0)
		scsi_go_and_wait (sc, tgt, sizeof(*cmd), 0xff, ior);
	else {
		(*sc->go) (sc, tgt, sizeof(*cmd), 0xff, FALSE);
		return tgt->done;
	}

	if (data)
		*data = tgt->cmd_ptr;

	(void) scsi_check_sense_data(tgt, tgt->cmd_ptr);

	return tgt->done;
}

boolean_t
scsi_check_sense_data(tgt, sns)
	register target_info_t	*tgt;
	scsi_sense_data_t	*sns;
{
	unsigned char   code;

	if (sns->error_class != SCSI_SNS_XTENDED_SENSE_DATA) {
		printf("Bad sense data, vuqe class x%x code x%x\n",
			sns->error_class, sns->error_code);
		return FALSE;	/* and good luck */
	} else {
		code = sns->u.xtended.sense_key;

		switch (code) {
		case SCSI_SNS_NOSENSE:
		case SCSI_SNS_EQUAL:
			return TRUE;
			break;
		case SCSI_SNS_RECOVERED:
			scsi_error(tgt, SCSI_ERR_BAD | SCSI_ERR_SENSE,
				   code, sns->u.xtended.add_bytes);
			return TRUE;
			break;
		case SCSI_SNS_UNIT_ATN:
			scsi_error(tgt, SCSI_ERR_SENSE,
				   code, sns->u.xtended.add_bytes);
			return TRUE;
			break;
		case SCSI_SNS_NOTREADY:
			tgt->done = SCSI_RET_RETRY;
			/* fall through */
		case SCSI_SNS_ILLEGAL_REQ:
			if (tgt->flags & TGT_OPTIONAL_CMD)
				return TRUE;
			/* fall through */
		default:
/* e.g.
		case SCSI_SNS_MEDIUM_ERR:
		case SCSI_SNS_HW_ERR:
		case SCSI_SNS_PROTECT:
		case SCSI_SNS_BLANK_CHK:
		case SCSI_SNS_VUQE:
		case SCSI_SNS_COPY_ABRT:
		case SCSI_SNS_ABORTED:
		case SCSI_SNS_VOLUME_OVFL:
		case SCSI_SNS_MISCOMPARE:
		case SCSI_SNS_RESERVED:
*/
			scsi_error(tgt, SCSI_ERR_GRAVE|SCSI_ERR_SENSE,
				   code, sns->u.xtended.add_bytes);
			return FALSE;
			break;
		}
	}
}

/*
 * START STOP UNIT (Optional, disk prin work rom tape[load/unload])
 */
scsi_start_unit( sc, tgt, ss, ior)
	register scsi_softc_t	*sc;
	register target_info_t	*tgt;
	io_req_t		ior;
{
	scsi_cmd_start_t	*cmd;

	cmd = (scsi_cmd_start_t*) (tgt->cmd_ptr);
	cmd->scsi_cmd_code = SCSI_CMD_START_STOP_UNIT;
	cmd->scsi_cmd_lun_and_lba1 = SCSI_CMD_SS_IMMED;/* 0 won't work ? */
	cmd->scsi_cmd_lba2 = 0;
	cmd->scsi_cmd_lba3 = 0;
	cmd->scsi_cmd_ss_flags = ss;
	cmd->scsi_cmd_ctrl_byte = 0;	/* not linked */
	
	tgt->cur_cmd = SCSI_CMD_START_STOP_UNIT;

	scsi_go_and_wait(sc, tgt, sizeof(*cmd), 0, ior);
	return tgt->done;
}

/*
 * TEST UNIT READY (Optional, All)
 * Note: this is where we do the synch negotiation at autoconf
 */
scsi_test_unit_ready( sc, tgt, ior)
	register scsi_softc_t	*sc;
	register target_info_t	*tgt;
	io_req_t		ior;
{
	scsi_cmd_test_unit_ready_t	*cmd;

	cmd = (scsi_cmd_test_unit_ready_t*) (tgt->cmd_ptr);

	cmd->scsi_cmd_code = SCSI_CMD_TEST_UNIT_READY;
	cmd->scsi_cmd_lun_and_lba1 = 0;
	cmd->scsi_cmd_lba2 = 0;
	cmd->scsi_cmd_lba3 = 0;
	cmd->scsi_cmd_ss_flags = 0;
	cmd->scsi_cmd_ctrl_byte = 0;	/* not linked */
	
	tgt->cur_cmd = SCSI_CMD_TEST_UNIT_READY;

	scsi_go_and_wait(sc, tgt, sizeof(*cmd), 0, ior);

	return tgt->done;
}

/*
 * RECEIVE DIAGNOSTIC RESULTS (Optional, All)
 */
scsi_receive_diag( sc, tgt, result, result_len, ior)
	register scsi_softc_t	*sc;
	register target_info_t	*tgt;
	char			*result;
	io_req_t		ior;
{
	scsi_cmd_receive_diag_t	*cmd;

	cmd = (scsi_cmd_receive_diag_t*) (tgt->cmd_ptr);
	cmd->scsi_cmd_code = SCSI_CMD_RECEIVE_DIAG_RESULTS;
	cmd->scsi_cmd_lun_and_lba1 = 0;
	cmd->scsi_cmd_lba2 = 0;
	cmd->scsi_cmd_lba3 = result_len >> 8 & 0xff;
	cmd->scsi_cmd_xfer_len = result_len & 0xff;
	cmd->scsi_cmd_ctrl_byte = 0;	/* not linked */
	
	tgt->cur_cmd = SCSI_CMD_RECEIVE_DIAG_RESULTS;

	scsi_go_and_wait(sc, tgt, sizeof(*cmd), result_len, ior);

	bcopy(tgt->cmd_ptr, (char*)result, result_len);

	return tgt->done;
}


scsi_mode_sense( sc, tgt, pagecode, len, ior)
	register scsi_softc_t	*sc;
	register target_info_t	*tgt;
	int			len;
	io_req_t		ior;
{
	scsi_cmd_mode_sense_t	*cmd;

	cmd = (scsi_cmd_mode_sense_t*) (tgt->cmd_ptr);
	cmd->scsi_cmd_code = SCSI_CMD_MODE_SENSE;
	cmd->scsi_cmd_lun_and_lba1 = 0;
	cmd->scsi_cmd_ms_pagecode = pagecode;
	cmd->scsi_cmd_lba3 = 0;
	cmd->scsi_cmd_xfer_len = len;
	cmd->scsi_cmd_ctrl_byte = 0;	/* not linked */
	
	tgt->cur_cmd = SCSI_CMD_MODE_SENSE;

	scsi_go_and_wait(sc, tgt, sizeof(*cmd), len, ior);

	return tgt->done;
}

#if	0 /* unused */

/*
 * COPY (Optional, All)
 */
scsi_copy( sc, tgt, params, params_len, ior)
	register scsi_softc_t	*sc;
	register target_info_t	*tgt;
	char			*params;
	io_req_t		ior;
{
	scsi_cmd_copy_t	*cmd;

	cmd = (scsi_cmd_copy_t*) (tgt->cmd_ptr;
	cmd->scsi_cmd_code = SCSI_CMD_COPY;
	cmd->scsi_cmd_lun_and_lba1 = 0;
	cmd->scsi_cmd_lba2 = params_len>>16 & 0xff;
	cmd->scsi_cmd_lba3 = params_len >> 8 & 0xff;
	cmd->scsi_cmd_xfer_len = params_len & 0xff;
	cmd->scsi_cmd_ctrl_byte = 0;	/* not linked */
	
	bcopy(params, cmd + 1, params_len);

	tgt->cur_cmd = SCSI_CMD_COPY;

	scsi_go_and_wait(sc, tgt, sizeof(*cmd) + params_len, 0, ior);
}

/*
 * SEND DIAGNOSTIC (Optional, All)
 */
scsi_send_diag( sc, tgt, flags, params, params_len, ior)
	register scsi_softc_t	*sc;
	register target_info_t	*tgt;
	char			*params;
	io_req_t		ior;
{
	scsi_cmd_send_diag_t	*cmd;

	cmd = (scsi_cmd_send_diag_t*) (tgt->cmd_ptr);
	cmd->scsi_cmd_code = SCSI_CMD_SEND_DIAGNOSTICS;
	cmd->scsi_cmd_lun_and_lba1 = flags & 0x7;
	cmd->scsi_cmd_lba2 = 0;
	cmd->scsi_cmd_lba3 = params_len >> 8 & 0xff;
	cmd->scsi_cmd_xfer_len = params_len & 0xff;
	cmd->scsi_cmd_ctrl_byte = 0;	/* not linked */
	
	bcopy(params, cmd + 1, params_len);

	tgt->cur_cmd = SCSI_CMD_SEND_DIAGNOSTICS;

	scsi_go_and_wait(sc, tgt, sizeof(*cmd), 0, ior);
}

/*
 * COMPARE (Optional, All)
 */
scsi_compare( sc, tgt, params, params_len, ior)
	register scsi_softc_t	*sc;
	register target_info_t	*tgt;
	char			*params;
	io_req_t		ior;
{
	scsi_cmd_compare_t	*cmd;

	cmd = (scsi_cmd_compare_t*) (tgt->cmd_ptr);
	cmd->scsi_cmd_code = SCSI_CMD_COMPARE;
	cmd->scsi_cmd_lun_and_relbit = 0;
	cmd->scsi_cmd_lba1 = 0;
	cmd->scsi_cmd_1_paraml1 = params_len >> 16 & 0xff;
	cmd->scsi_cmd_1_paraml2 = params_len >> 8 & 0xff;
	cmd->scsi_cmd_1_paraml3 = params_len & 0xff;
	cmd->scsi_cmd_xxx = 0;
	cmd->scsi_cmd_xfer_len_1 = 0;
	cmd->scsi_cmd_xfer_len_2 = 0;
	cmd->scsi_cmd_ctrl_byte = 0;	/* not linked */
	
	bcopy(params, cmd + 1, params_len);

	tgt->cur_cmd = SCSI_CMD_COMPARE;

	scsi_go_and_wait(sc, tgt, sizeof(*cmd), 0, ior);
}

/*
 * COPY AND VERIFY (Optional, All)
 */
scsi_copy_and_verify( sc, tgt, params, params_len, bytchk, ior)
	register scsi_softc_t	*sc;
	register target_info_t	*tgt;
	char			*params;
	io_req_t		ior;
{
	scsi_cmd_compare_t	*cmd;

	cmd = (scsi_cmd_compare_t*) (tgt->cmd_ptr);
	cmd->scsi_cmd_code = SCSI_CMD_COMPARE;
	cmd->scsi_cmd_lun_and_relbit = bytchk ? SCSI_CMD_CPY_BYTCHK : 0;
	cmd->scsi_cmd_lba1 = 0;
	cmd->scsi_cmd_1_paraml1 = params_len >> 16 & 0xff;
	cmd->scsi_cmd_1_paraml2 = params_len >> 8 & 0xff;
	cmd->scsi_cmd_1_paraml3 = params_len & 0xff;
	cmd->scsi_cmd_xxx = 0;
	cmd->scsi_cmd_xfer_len_1 = 0;
	cmd->scsi_cmd_xfer_len_2 = 0;
	cmd->scsi_cmd_ctrl_byte = 0;	/* not linked */
	
	bcopy(params, cmd + 1, params_len);

	tgt->cur_cmd = SCSI_CMD_COMPARE;

	scsi_go_and_wait(sc, tgt, sizeof(*cmd), 0, ior);
}

#endif

#ifdef	SCSI2
scsi_change_definition
scsi_log_select
scsi_log_sense
scsi_long_mode_select
scsi_read_buffer
scsi_write_buffer
#endif	SCSI2

/*
 * Warn user of some device error
 */
int	scsi_debug = 0;

void
scsi_error( tgt, code, info, addtl)
	target_info_t	*tgt;
	unsigned	code;
	unsigned	info;
	char		*addtl;
{
	char		unit;
	char		*msg, *cmd;
	scsi2_status_byte_t	status;
	static char *sns_msg[SCSI_SNS_RESERVED+1] = {
		"",/* shouldn't happen */
		"Recovered",
		"Unit not ready",
		"Medium",
		"Hardware failure",
		"Illegal request",
		"Unit Attention Condition",
		"Protection",
		"Blank Check",
		"Vendor Unique",
		"Copy Operation Aborted",
		"Aborted Command",
		"Equal Comparison",
		"Volume Overflow",
		"Miscompare",
		"Reserved"
	};

	if (scsi_debug)
		code |= SCSI_ERR_GRAVE;

	if (tgt)
		unit = tgt->unit_no + '0';
	else
		unit = '?';


	switch (SCSI_ERR_CLASS(code)) {
	case SCSI_ERR_STATUS:
		cmd = "Bad status return";
		status.bits = info;
		switch (status.st.scsi_status_code) {
		case SCSI_ST_GOOD:
		case SCSI_ST_CONDITION_MET:
		case SCSI_ST_INT_GOOD:
		case SCSI_ST_INT_MET:
			return;	/* all is fine */
		case SCSI_ST_CHECK_CONDITION:
			msg = "Check condition"; break;
		case SCSI_ST_RES_CONFLICT:
			msg = "Reservation conflict"; break;
		case SCSI_ST_BUSY:
			msg = "Target busy"; break;
		case SCSI_ST2_QUEUE_FULL:
			msg = "Queue full"; break;
		case SCSI_ST2_CMD_TERMINATED:
			msg = "Command terminated"; break;
		default:
			msg = "Strange"; break;
		}
		break;
	case SCSI_ERR_SENSE:
		cmd = "Sensed a";
		msg = sns_msg[info & 0xf];
		break;
	case SCSI_ERR_MSEL:
		cmd = "Mode select broken"; msg = ""; break;
	default:
		cmd = "Generic"; msg = "";
	}
	if (SCSI_ERR_GRAVITY(code)) {
		printf("\n%s%c: %s %s %sx%x", "target ", unit, cmd, msg,
			" error, code ", info);
		if (addtl) {
			unsigned int	add[3];
			bcopy(addtl, (char*)add, 3*sizeof(int));
			printf("%s x%x x%x x%x", ", additional info ",
				add[0], add[1], add[2]);
		}
		printf("\n");
	}
}
