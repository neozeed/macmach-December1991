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
 * $Log:	scsi2.h,v $
 * Revision 2.7  91/06/19  11:57:28  rvb
 * 	File moved here from mips/PMAX since it is now "MI" code, also
 * 	used by Vax3100 and soon -- the omron luna88k.
 * 	[91/06/04            rvb]
 * 
 * Revision 2.6  91/05/14  17:28:31  mrt
 * 	Correcting copyright
 * 
 * Revision 2.5  91/05/13  06:04:53  af
 * 	Added read_defect_data.
 * 	[91/05/12  16:02:41  af]
 * 
 * Revision 2.4  91/02/05  17:44:53  mrt
 * 	Added author notices
 * 	[91/02/04  11:18:21  mrt]
 * 
 * 	Changed to use new Mach copyright
 * 	[91/02/02  12:17:02  mrt]
 * 
 * Revision 2.3  90/12/05  23:34:44  af
 * 	Status byte decl was wrong.
 * 	[90/12/03  23:39:07  af]
 * 
 * Revision 2.1.1.1  90/11/01  03:38:39  af
 * 	Created, from the SCSI specs:
 * 	"Small Computer System Interface - 2 (SCSI-II)", ANSI Draft
 * 	X3T9.2/86-109 -  Rev 10C March 1990
 * 	[90/09/03            af]
 * 
 */
/*
 *	File: scsi2.h
 * 	Author: Alessandro Forin, Carnegie Mellon University
 *	Date:	9/90
 *
 *	Additions and changes of the SCSI-II standard viz SCSI-I
 */

#ifndef	_SCSI_SCSI2_H_
#define	_SCSI_SCSI2_H_

#include <scsi/scsi_endian.h>

/*
 * Single byte messages
 *
 * originator:	I-nitiator T-arget
 * T-support:	M-andatory O-ptional
 */

#define SCSI_ABORT_TAG			0x0d	/* O I 2 */
#define SCSI_CLEAR_QUEUE		0x0e	/* O I 2 */
#define SCSI_INITIATE_RECOVERY		0x0f	/* O IT2 */
#define SCSI_RELEASE_RECOVERY		0x10	/* O I 2 */
#define SCSI_TERMINATE_IO_PROCESS	0x11	/* O I 2 */

/*
 * Two byte messages
 */
#define SCSI_SIMPLE_QUEUE_TAG		0x20	/* O IT2 */
#define SCSI_HEADOF_QUEUE_TAG		0x21	/* O I 2 */
#define SCSI_ORDERED_QUEUE_TAG		0x22	/* O I 2 */
#define SCSI_IGNORE_WIDE_RESIDUE	0x23	/* O  T2 */
					/* 0x24..0x2f reserved */

/*
 * Extended messages, codes and formats
 */

#define SCSI_WIDE_XFER_REQUEST		0x03	/* IT 2 */
typedef struct {
	unsigned char	xtn_msg_tag;		/* const 0x01 */
	unsigned char	xtn_msg_len;		/* const 0x02 */
	unsigned char	xtn_msg_code;		/* const 0x03 */
	unsigned char	xtn_msg_xfer_width;
} scsi_wide_xfer_t;

/*
 * NOTE: some command-specific mods and extensions
 * are actually defined in the scsi.h file for
 * readability reasons
 */

				/* GROUP 1 */

#define	SCSI_CMD_READ_DEFECT_DATA	0x37	/* O2 disk opti */
typedef scsi_command_group_1	scsi_cmd_read_defect_t;
#	define SCSI_CMD_RDD_LIST_TYPE		0x07
#	define SCSI_CMD_RDD_GLIST		0x08
#	define SCSI_CMD_RDD_PLIST		0x10

#define SCSI_CMD_WRITE_BUFFER		0x3b	/* O2 all */
typedef scsi_command_group_1	scsi_cmd_write_buffer_t;
#	define SCSI_CMD_BUF_MODE		0x07
#	define scsi_cmd_buf_id			scs_cmd_lba1
#	define scsi_cmd_buf_offset1		scs_cmd_lba2
#	define scsi_cmd_buf_offset2		scs_cmd_lba3
#	define scsi_cmd_buf_offset3		scs_cmd_lba4
#	define scsi_cmd_buf_alloc1		scs_cmd_xxx
#	define scsi_cmd_buf_alloc2		scs_cmd_xfer_len_1
#	define scsi_cmd_buf_alloc3		scs_cmd_xfer_len_2

#define SCSI_CMD_READ_BUFFER		0x3c	/* O2 all */
#define	scsi_cmd_read_buffer_t scsi_command_group_1

				/* GROUP 2 */
#define SCSI_CMD_CHANGE_DEFINITION	0x40	/* O2 all */
#define	scsi_cmd_change_def_t	scsi_command_group_2
#	define scsi_cmd_chg_save		scsi_cmd_lba1
#	define scsi_cmd_chg_definition		scsi_cmd_lba2
#	define SCSI_CMD_CHG_CURRENT		0x00
#	define SCSI_CMD_CHG_SCSI_1		0x01
#	define SCSI_CMD_CHG_CCS			0x02
#	define SCSI_CMD_CHG_SCSI_2		0x03

					/* 0x41..0x4b reserved */

#define SCSI_CMD_LOG_SELECT		0x4c	/* O2 all */
#define	scsi_cmd_logsel_t	scsi_command_group_2
#	define SCSI_CMD_LOG_SP			0x01
#	define SCSI_CMD_LOG_PCR			0x02
#	define scsi_cmd_log_page_control	scsi_cmd_lba1

#define SCSI_CMD_LOG_SENSE		0x4d	/* O2 all */
#define	scsi_cmd_logsense_t	scsi_command_group_2
#	define SCSI_CMD_LOG_PPC			0x02
#	define scsi_cmd_log_page_code		scsi_cmd_lba1
#	define scsi_cmd_log_param_ptr1		scsi_cmd_lba4
#	define scsi_cmd_log_param_ptr2		scsi_cmd_xxx


					/* 0x4e..0x54 reserved */

#define SCSI_CMD_MODE_SELECT_2		0x55	/* Z2 */
#define	scsi_cmd_mode_select_long_t	scsi_command_group_2
#	define SCSI_CMD_MSL2_PF		0x10
#	define SCSI_CMD_MSL2_SP		0x01

					/* 0x56..0x59 reserved */

#define SCSI_CMD_MODE_SENSE_2		0x5a	/* Z2 */
#define	scsi_cmd_mode_sense_long_t	scsi_command_group_2
#	define SCSI_CMD_MSS2_DBD	0x08

					/* 0x5b..0x5f reserved */

/*
 * Command specific defines
 */
typedef struct {
	BITFIELD_2(unsigned char,
			periph_type : 5,
			periph_qual : 3);
#define	SCSI_SCANNER		0x06	/* periph_type values */
#define	SCSI_MEMORY		0x07
#define	SCSI_J_BOX		0x08
#define	SCSI_COMM		0x09
#define	SCSI_PREPRESS1		0x0a
#define	SCSI_PREPRESS2		0x0b

#define	SCSI_PERIPH_CONNECTED	0x00	/* periph_qual values */
#define	SCSI_PERIPH_DISCONN	0x20
#define	SCSI_PERIPH_NOTHERE	0x30

	BITFIELD_2(unsigned char,
			device_type : 7,
			rmb : 1);

	BITFIELD_3( unsigned char,
			ansi : 3,
			ecma : 3,
			iso : 2);

	BITFIELD_4( unsigned char,
			response_fmt : 4,
			res1 : 2,
			trmIOP : 1,
			aenc : 1);
	unsigned char	length;
	unsigned char	res2;
	unsigned char	res3;

	BITFIELD_8(unsigned char,
			SftRe : 1,
			CmdQue : 1,
			res4 : 1,
			Linked : 1,
			Sync : 1,
			Wbus16 : 1,
			Wbus32 : 1,
			RelAdr : 1);

	unsigned char	vendor_id[8];
	unsigned char	product_id[16];
	unsigned char	product_rev[4];
	unsigned char	vendor_uqe[20];
	unsigned char	reserved[40];
	unsigned char	vendor_uqe1[1];	/* varsize */
} scsi2_inquiry_data_t;
#define	SCSI_INQ_SUPP_PAGES	0x00
#define	SCSI_INQ_A_INFO		0x01	/* 0x01..0x1f, really */
#define	SCSI_INQ_SERIALNO	0x80
#define	SCSI_INQ_IMPL_OPDEF	0x81
#define	SCSI_INQ_A_IMPL_OPDEF	0x82


/*
 * Status byte (a-la scsi2)
 */

typedef union {
    struct {
	BITFIELD_3( unsigned char,
			scsi_status_reserved1:1,
			scsi_status_code:5,
			scsi_status_reserved2:2);
							/* more scsi_status_code values */
					/* 00..0c as in SCSI-I */
#	define SCSI_ST2_CMD_TERMINATED	0x11	/* 2 */
#	define SCSI_ST2_QUEUE_FULL	0x14	/* 2 */
					/* anything else is reserved */
    } st;
    unsigned char bits;
} scsi2_status_byte_t;

#endif	_SCSI_SCSI2_H_
