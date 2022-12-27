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
 * $Log:	scsi_defs.h,v $
 * Revision 2.9  91/08/24  12:28:38  af
 * 	Added processor_type infos, definition of an opaque type,
 * 	multiP locking.
 * 	[91/08/02  03:55:05  af]
 * 
 * Revision 2.8  91/07/09  23:22:53  danner
 * 	Added include of <scsi/rz_labels.h>
 * 	[91/07/09  11:16:30  danner]
 * 
 * Revision 2.7  91/06/19  11:57:43  rvb
 * 	File moved here from mips/PMAX since it is now "MI" code, also
 * 	used by Vax3100 and soon -- the omron luna88k.
 * 	[91/06/04            rvb]
 * 
 * Revision 2.6  91/05/14  17:30:18  mrt
 * 	Correcting copyright
 * 
 * Revision 2.5  91/05/13  06:05:34  af
 * 	Made counters unsigned, added copy_count for use by HBAs
 * 	that do unlimited DMA via double buffering.  Made explicit
 * 	two padding bytes, and let them be HBA-specific (e.g. used
 * 	for odd-byte-boundary conditions on some).
 * 	Made max_dma_data unsigned, a value of -1 means unlimited.
 * 	Removed unsed residue field.
 * 
 * 	Defined tape-specific information fields to target structure.
 * 	Added tape-specific flags and flag for targets that require
 * 	the long form of various scsi commands.
 * 	Added disconnected-state information to target structure.
 * 	Added watchdog field to adapter structure.
 * 	[91/05/12  16:24:10  af]
 * 
 * Revision 2.4.1.2  91/04/05  13:13:29  af
 * 	Made counters unsigned, added copy_count for use by HBAs
 * 	that do unlimited DMA via double buffering.  Made explicit
 * 	two padding bytes, and let them be HBA-specific (e.g. used
 * 	for odd-byte-boundary conditions on some).
 * 	Made max_dma_data unsigned, a value of -1 means unlimited.
 * 	Removed unsed residue field.
 * 
 * Revision 2.4.1.1  91/03/29  17:06:09  af
 * 	Defined tape-specific information fields to target structure.
 * 	Added tape-specific flags and flag for targets that require
 * 	the long form of various scsi commands.
 * 	Added disconnected-state information to target structure.
 * 	Added watchdog field to adapter structure.
 * 
 * Revision 2.4  91/02/05  17:45:43  mrt
 * 	Added author notices
 * 	[91/02/04  11:19:29  mrt]
 * 
 * 	Changed to use new Mach copyright
 * 	[91/02/02  12:18:11  mrt]
 * 
 * Revision 2.3  90/12/05  23:35:12  af
 * 	Cleanups, use BSD labels internally.
 * 	[90/12/03  23:47:29  af]
 * 
 * Revision 2.1.1.1  90/11/01  03:39:55  af
 * 	Created.
 * 	[90/09/03            af]
 */
/*
 *	File: scsi_defs.h
 * 	Author: Alessandro Forin, Carnegie Mellon University
 *	Date:	9/90
 *
 *	Controller-independent definitions for the SCSI driver
 */

#ifndef	_SCSI_SCSI_DEFS_H_
#define	_SCSI_SCSI_DEFS_H_

#include <kern/queue.h>
#include <kern/lock.h>

#define	await(event)	sleep(event,0)
extern	wakeup();

typedef	vm_offset_t	opaque_t;	/* should be elsewhere */

/*
 * Internal error codes, and return values
 * XXX use the mach/error.h scheme XXX
 */
typedef unsigned int		scsi_ret_t;

#define	SCSI_ERR_GRAVITY(x)	((unsigned)(x)&0xf0000000)
#define	SCSI_ERR_GRAVE		0x80000000
#define SCSI_ERR_BAD		0x40000000

#define	SCSI_ERR_CLASS(x)	((unsigned)(x)&0x0fffffff)
#define	SCSI_ERR_STATUS		0x00000001
#define	SCSI_ERR_SENSE		0x00000002
#define SCSI_ERR_MSEL		0x00000004

extern	void	scsi_error(/* target_info_t *, unsigned, unsigned */);

#define	SCSI_RET_IN_PROGRESS	0x00
#define	SCSI_RET_SUCCESS	0x01
#define	SCSI_RET_RETRY		0x02
#define SCSI_RET_NEED_SENSE	0x04
#define SCSI_RET_ABORTED	0x08
#define	SCSI_RET_DEVICE_DOWN	0x10

/*
 * Device-specific information kept by driver
 */
typedef struct {
	struct disklabel	l;
	struct {
	    unsigned int	badblockno;
	    unsigned int	save_rec;
	    char		*save_addr;
	    int			save_count;
	    int			save_resid;
	    int			retry_count;
	} b;
} scsi_disk_info_t;

typedef struct {
	boolean_t	read_only;
	unsigned int	speed;
	unsigned int	density;
	unsigned int	maxreclen;
	boolean_t	fixed_size;
} scsi_tape_info_t;

typedef struct {
	char		req_pending;
	char		req_id;
	char		req_lun;
	char		req_cmd;
	unsigned int	req_len;
	/* more later */
} scsi_processor_info_t;

/*
 * Device-specific operations
 */
typedef struct {
	char		*(*driver_name)();	/* my driver's name */
	scsi_ret_t	(*optimize)();		/* tune up internal params */
	scsi_ret_t	(*open)();		/* open time ops */
	scsi_ret_t	(*close)();		/* close time ops */
	int		(*strategy)();		/* sort/start routine */
	int		(*restart)();		/* completion routine */
	io_return_t	(*get_status)();	/* specialization */
	io_return_t	(*set_status)();	/* specialization */
} scsi_devsw_t;

extern scsi_devsw_t	scsi_devsw[];


/*
 * Device descriptor
 */

#define	SCSI_TARGET_NAME_LEN	8+16+4+8	/* our way to keep it */

typedef struct {
	queue_chain_t	links;			/* to queue for bus */
	io_req_t	ior;			/* what we are doing */

	unsigned int	flags;
#define	TGT_DID_SYNCH		0x00000001	/* finished the synch neg */
#define	TGT_TRY_SYNCH		0x00000002	/* do the synch negotiation */
#define	TGT_FULLY_PROBED	0x00000004	/* can sleep to wait */
#define	TGT_ONLINE		0x00000008	/* did the once-only stuff */
#define	TGT_ALIVE		0x00000010
#define	TGT_BBR_ACTIVE		0x00000020	/* bad block replc in progress */
#define	TGT_DISCONNECTED	0x00000040	/* waiting for reconnect */
#define	TGT_WRITTEN_TO		0x00000080	/* tapes: needs a filemark on close */
#define	TGT_REWIND_ON_CLOSE	0x00000100	/* tapes: rewind */
#define	TGT_BIG			0x00000200	/* disks: > 1Gb, use long R/W */
#define	TGT_REMOVABLE_MEDIA	0x00000400	/* e.g. floppy, cd-rom,.. */
#define	TGT_READONLY		0x00000800	/* cd-rom, scanner, .. */
#define	TGT_OPTIONAL_CMD	0x00001000	/* optional cmd, ignore errors */
#define TGT_WRITE_LABEL		0x00002000	/* disks: enable overwriting of label */
#define	TGT_US			0x00004000	/* our desc, when target role */

#define	TGT_HW_SPECIFIC_BITS	0xffff0000	/* see specific HBA */
	char		*hw_state;		/* opaque */
	char		*dma_ptr;
	char		*cmd_ptr;
	scsi_devsw_t	*dev_ops;
	char		target_id;
	char		unit_no;
	unsigned char	sync_period;
	unsigned char	sync_offset;
	decl_simple_lock_data(,target_lock)
#ifdef	MACH_KERNEL
#else	/*MACH_KERNEL*/
	struct fdma	fdma;
#endif	/*MACH_KERNEL*/
	/*
	 * State info kept while waiting to seize bus, either for first
	 * selection or while in disconnected state
	 */
	struct {
	    struct script	*script;
	    int			(*handler)();
	    unsigned int	out_count;
	    unsigned int	in_count;
	    unsigned int	copy_count;	/* optional */
	    unsigned int	dma_offset;
	    unsigned char	identify;
	    unsigned char	cmd_count;
	    unsigned char	hba_dep[2];
	} transient_state;
	unsigned int	block_size;
	volatile char	done;
	char		cur_cmd;
	char		lun;
	char		xxx;
	char		tgt_name[SCSI_TARGET_NAME_LEN];
	union {
	    scsi_disk_info_t	disk;
	    scsi_tape_info_t	tape;
	    scsi_processor_info_t	cpu;
	} dev_info;
} target_info_t;

/*
 * HBA descriptor
 */

typedef struct {
	/* initiator (us) state */
	unsigned char	initiator_id;
	unsigned char	masterno;
	unsigned int	max_dma_data;
	char		*hw_state;		/* opaque */
	int		(*go)();
	int		(*watchdog)();
	boolean_t	(*probe)();
	/* per-target state */
	target_info_t		*target[8];
} scsi_softc_t;

extern scsi_softc_t	*scsi_softc[];
extern scsi_softc_t	*scsi_master_alloc(/* int unit */);
extern target_info_t	*scsi_slave_alloc(/* int unit, int slave, char *hw */);

#define	BGET(d,mid,id)	(d[mid] & (1 << id))		/* bitmap ops */
#define BSET(d,mid,id)	d[mid] |= (1 << id)
#define BCLR(d,mid,id)	d[mid] &= ~(1 << id)

extern unsigned char	scsi_no_synchronous_xfer[];	/* one bitmap per ctlr */
extern unsigned char	scsi_use_long_form[];		/* one bitmap per ctlr */
extern unsigned char	scsi_might_disconnect[];	/* one bitmap per ctlr */
extern unsigned char	scsi_should_disconnect[];	/* one bitmap per ctlr */
extern unsigned char	scsi_initiator_id[];		/* one id per ctlr */

extern boolean_t	scsi_exabyte_filemarks;
extern boolean_t	scsi_no_automatic_bbr;
extern int		scsi_bbr_retries;
extern int		scsi_watchdog_period;
extern int		scsi_delay_after_reset;
extern unsigned int	scsi_per_target_virtual;	/* 2.5 only */

extern int		scsi_debug;

/*
 * HBA-independent Watchdog
 */
typedef struct {

	unsigned short	reset_count;
	char		nactive;

	char		watchdog_state;

#define SCSI_WD_INACTIVE	0
#define	SCSI_WD_ACTIVE		1
#define SCSI_WD_EXPIRED		2

	int		(*reset)();

} watchdog_t;

extern int	scsi_watchdog(/* watchdog_t* */);

#endif	_SCSI_SCSI_DEFS_H_
