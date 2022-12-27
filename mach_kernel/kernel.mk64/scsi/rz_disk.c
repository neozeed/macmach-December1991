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
 * $Log:	rz_disk.c,v $
 * Revision 2.12  91/08/24  12:27:52  af
 * 	Spl defs.  Search for label. MultiP locks.
 * 	[91/08/02  03:53:45  af]
 * 
 *	Bcopy casts.
 * 
 * Revision 2.11  91/07/09  23:22:28  danner
 * 	Upgraded to support new label technology.
 * 	[91/07/09  11:15:50  danner]
 * 
 * Revision 2.10  91/06/25  20:56:16  rpd
 * 	Tweaks to make gcc happy.
 * 
 * Revision 2.9  91/06/19  11:57:01  rvb
 * 	File moved here from mips/PMAX since it is now "MI" code, also
 * 	used by Vax3100 and soon -- the omron luna88k.
 * 	[91/06/04            rvb]
 * 
 * Revision 2.8  91/05/14  17:26:29  mrt
 * 	Correcting copyright
 * 
 * Revision 2.7  91/05/13  06:04:25  af
 * 	Revision 2.6.1.2  91/04/05  13:08:43  af
 * 	Fixed a type declaration, now an HBA with unlimited DMA
 * 	capability sez so by setting the max_dma_size to -1.
 * 
 * 	Revision 2.6.1.1  91/03/29  17:03:03  af
 * 	Use long forms of read/write commands on disks bigger than 1Gb,
 * 	which we automatically recognize from the read_capacity.
 * 	Allocate io_req_t records properly.
 * 	Check for transactions that aborted due to scsi bus resets.
 * 	Issue a request_sense on bad transactions, log soft errors
 * 	and only make fail bad errors.
 * 	[91/05/12  16:03:45  af]
 * 
 * Revision 2.6.1.2  91/04/05  13:08:43  af
 * 	Fixed a type declaration, now an HBA with unlimited DMA
 * 	capability sez so by setting the max_dma_size to -1.
 * 
 * Revision 2.6.1.1  91/03/29  17:03:03  af
 * 	Use long forms of read/write commands on disks bigger than 1Gb,
 * 	which we automatically recognize from the read_capacity.
 * 	Allocate io_req_t records properly.
 * 	Check for transactions that aborted due to scsi bus resets.
 * 	Issue a request_sense on bad transactions, log soft errors
 * 	and only make fail bad errors.
 * 
 * Revision 2.6  91/02/14  14:35:22  mrt
 * 	Added (optional and disabled) code for checksumming.
 * 	[91/02/12  12:58:27  af]
 * 
 * 	Bug in strategy routine: for xfers larger than
 * 	max_dma was not setting the io_count right at
 * 	the end of the transfer.  Now new fsck works.
 * 	[90/12/10  17:26:47  af]
 * 
 * Revision 2.5  91/02/05  17:43:47  mrt
 * 	Added author notices
 * 	[91/02/04  11:16:50  mrt]
 * 
 * 	Changed to use new Mach copyright
 * 	[91/02/02  12:15:37  mrt]
 * 
 * Revision 2.4  90/12/20  17:01:09  jeffreyh
 * 	Bug in strategy routine: for xfers larger than
 * 	max_dma was not setting the io_count right at
 * 	the end of the transfer.  Now new fsck works.
 * 	[90/12/10  17:26:47  af]
 * 
 * Revision 2.3  90/12/05  23:33:57  af
 * 	Use and prefer BSD/OSF labels.
 * 	[90/12/03  23:33:43  af]
 * 
 * Revision 2.1.1.1  90/11/01  03:43:47  af
 * 	Created.
 * 	[90/10/21            af]
 */
/*
 *	File: rz_disk.c
 * 	Author: Alessandro Forin, Carnegie Mellon University
 *	Date:	10/90
 *
 *	Top layer of the SCSI driver: interface with the MI.
 *	This file contains operations specific to disk-like devices.
 */

#include <machine/machparam.h>		/* spl definitions */
#include <mach/std_types.h>
#include <scsi/compat_30.h>
#include <machine/machparam.h>

#ifdef	MACH_KERNEL
#else	/*MACH_KERNEL*/
#include <sys/kernel.h>		/* for hz */
#endif	/*MACH_KERNEL*/

#include <scsi/scsi.h>
#include <scsi/scsi_defs.h>
#include <scsi/rz.h>
#include <scsi/rz_labels.h>


extern scsi_ret_t	scdisk_read(), scdisk_write(),
			scsi_long_read(), scsi_long_write();

/*
 * Label used if the disk does not have one
 */
struct disklabel scsi_default_label =
{
	DISKMAGIC, DTYPE_SCSI, 0,
	"SCSI", "",
	DEV_BSIZE, 1, 1, 1, 1, 1, 0, 0, 0,
	3600, 1, 1, 1, 0, 0, 0,
	{0,}, {0,},
	DISKMAGIC, 0,
	8, 8192, 8192,
	{{ -1, 0, 1024, FS_BSDFFS, 8, 3 },
	 { -1, 0, 1024, FS_BSDFFS, 8, 3 },
	 { -1, 0, 1024, FS_BSDFFS, 8, 3 },
	 { -1, 0, 1024, FS_BSDFFS, 8, 3 },
	 { -1, 0, 1024, FS_BSDFFS, 8, 3 },
	 { -1, 0, 1024, FS_BSDFFS, 8, 3 },
	 { -1, 0, 1024, FS_BSDFFS, 8, 3 },
	 { -1, 0, 1024, FS_BSDFFS, 8, 3 }}
};

/*
 * Specialized side of the open routine for disks
 */
scdisk_open(sc, tgt, req)
	scsi_softc_t		*sc;
	target_info_t		*tgt;
{
	register int 	i;
	register scsi_ret_t	ret = SCSI_RET_SUCCESS;
	unsigned int		disk_size, secs_per_cyl, sector_size;
	scsi_rcap_data_t	*cap;
	struct disklabel	*label;
	io_req_t		ior;
	scsi_ret_t		(*readfun)() = scdisk_read;
	char			buff[DEV_BSIZE], *data;

	if (tgt->flags & TGT_ONLINE)
		return SCSI_RET_SUCCESS;

	/*
	 * Dummy ior for proper sync purposes
	 */
	io_req_alloc(ior,0);
	ior->io_next = 0;
	ior->io_count = 0;

#ifdef	MACH_KERNEL
	data = buff;
#else	/*MACH_KERNEL*/
	data = (char *)ior->io_data;
#endif	/*MACH_KERNEL*/

	/*
	 * Set the LBN to DEV_BSIZE with a MODE SELECT.
	 * xxx do a MODE SENSE instead ?
	 */
	for (i = 0; i < 5; i++) {
		ior->io_op = IO_INTERNAL;
		ret = scdisk_mode_select(sc, tgt, DEV_BSIZE, ior, 0);
		if (ret == SCSI_RET_SUCCESS)
			break;
		if (ret == SCSI_RET_RETRY) {
			timeout(wakeup, tgt, 2*hz);
			await(tgt);
		}
		if (ret == SCSI_RET_DEVICE_DOWN)
			goto done;
	}
	if (ret != SCSI_RET_SUCCESS) {
		scsi_error( tgt, SCSI_ERR_MSEL, ret, 0);
		ret = D_INVALID_SIZE;
		goto done;
	}
	tgt->block_size = DEV_BSIZE;

	/*
	 * Do a READ CAPACITY to get max size. Check LBN too.
	 */
	for (i = 0; i < 5; i++) {
		ior->io_op = IO_INTERNAL;
		ret = scsi_read_capacity(sc, tgt, 0, ior);
		if (ret == SCSI_RET_SUCCESS)
			break;
	}
	if (ret == SCSI_RET_SUCCESS) {
		int			val;

		cap = (scsi_rcap_data_t*) tgt->cmd_ptr;
		disk_size = (cap->lba1<<24) |
			    (cap->lba2<<16) |
			    (cap->lba3<< 8) |
			     cap->lba4;
		if (scsi_debug)
			printf("rz%d holds %d blocks\n", tgt->unit_no, disk_size);
		val = (cap->blen1<<24) |
		      (cap->blen2<<16) |
		      (cap->blen3<<8 ) |
		       cap->blen4;
		if (val != DEV_BSIZE) panic("read capacity bad");

		if (disk_size > SCSI_CMD_READ_MAX_LBA)
			tgt->flags |= TGT_BIG;

	} else {
		printf("Unknown disk capacity??\n");
		disk_size = -1;
	}
	/*
	 * Mandatory long-form commands ?
	 */
	if (BGET(scsi_use_long_form,sc->masterno,tgt->target_id))
		tgt->flags |= TGT_BIG;
	if (tgt->flags & TGT_BIG)
		readfun = scsi_long_read;

	/*
	 * Find out about the phys disk geometry
	 */

	ior->io_op = IO_INTERNAL;
	ret = scsi_read_capacity( sc, tgt, 1, ior);
	/* scsi_error(...) */
	cap = (scsi_rcap_data_t*) tgt->cmd_ptr;
	secs_per_cyl =	(cap->lba1<<24) | (cap->lba2<<16) |
			(cap->lba3<< 8) |  cap->lba4;
	secs_per_cyl += 1;
	sector_size =	(cap->blen1<<24) | (cap->blen2<<16) |
			(cap->blen3<<8 ) |  cap->blen4;
	if (scsi_debug)
	printf("rz%d: %d sect/cyl %d bytes/sec\n", tgt->unit_no,
		secs_per_cyl, sector_size);

#ifdef	SCSI2
	/*
	 * ... and a bunch of other things for scsi2
	 */
#endif	SCSI2

	/*
	 * Get partition table off pack
	 */

	/* first look for a BSD label */	
	ior->io_data = data;
	ior->io_count = DEV_BSIZE;
	ior->io_op = IO_READ;
	tgt->ior = ior;
	(*readfun)( sc, tgt, LABELOFFSET/DEV_BSIZE, ior);
	iowait(ior);

	{
		/* Search for BSD label, might be a bit further along */
		register int	i;
		boolean_t	found;

		for (i = LABELOFFSET % DEV_BSIZE, found = FALSE;
		     i < (DEV_BSIZE-sizeof(struct disklabel));
		     i += sizeof(int)) {
			label = (struct disklabel *) &data[i];
			if (label->d_magic  == DISKMAGIC &&
			    label->d_magic2 == DISKMAGIC) {
				found = TRUE;
				break;
			}
		}
		if (!found) label = 0;
	}

	if (label) {
		if (scsi_debug)
			printf("{Using BSD label}");
		tgt->dev_info.disk.l = *label;
	} else {
		/* then look for a vendor's label, but first
		   fill in defaults and what we found */

		label = &tgt->dev_info.disk.l;
		*label = scsi_default_label;
		label->d_secsize = sector_size;
		label->d_secpercyl = secs_per_cyl;
		label->d_secperunit = disk_size;

		ior->io_data = data;
		if (!rz_vendor_label ||
		    !(*rz_vendor_label)(sc, tgt, label, ior)) {

			printf("rz%d: %s\n", tgt->unit_no,
				"No valid disk label, using defaults");
			/* Validate partitions a and c for initialization */
			tgt->dev_info.disk.l.d_partitions[0].p_size = disk_size;
			tgt->dev_info.disk.l.d_partitions[2].p_size = disk_size;
		}
		label->d_checksum = 0;
		label->d_checksum = dkcksum(label);;
	}

	ret = SCSI_RET_SUCCESS;
done:
	io_req_free(ior);
	return ret;
}

/*
 * Disk strategy routine
 */
scdisk_strategy(ior)
	register io_req_t	ior;
{
	target_info_t  *tgt;
	register scsi_softc_t	*sc;
	scsi_ret_t      ret;
	register int    i = ior->io_unit, part;
	register unsigned rec, max;

	sc = scsi_softc[rzcontroller(i)];
	tgt = sc->target[rzslave(i)];
	part = rzpartition(i);

	/*
	 * Validate request 
	 */

	/* readonly ? */
	if ((tgt->flags & TGT_READONLY) &&
	    (ior->io_op & (IO_READ|IO_INTERNAL) == 0)) {
		ior->io_error = D_READ_ONLY;
		ior->io_op |= IO_ERROR;
		ior->io_residual = ior->io_count;
		iodone(ior);
		return;
	}

	rec = ior->io_recnum;
	max = tgt->dev_info.disk.l.d_partitions[part].p_size;
	if (max == -1)
		max = tgt->dev_info.disk.l.d_secperunit -
			tgt->dev_info.disk.l.d_partitions[part].p_offset;
	i = btodb(ior->io_count + DEV_BSIZE - 1);
	if (((rec + i) > max) ||
#if later
	    ((rec <= LABELSECTOR) && ((tgt->flags & TGT_WRITE_LABEL) == 0))
#else
	    FALSE
#endif
	    ) {
		ior->io_error = D_INVALID_SIZE;
		ior->io_op |= IO_ERROR;
		ior->io_residual = ior->io_count;
		iodone(ior);
		return;
	}
	/*
	 * Find location on disk: secno and cyl (for disksort) 
	 */
	rec += tgt->dev_info.disk.l.d_partitions[part].p_offset;
	ior->io_residual = rec / tgt->dev_info.disk.l.d_secpercyl;

	/*
	 * Enqueue operation 
	 */
	i = splbio();
	simple_lock(&tgt->target_lock);
	if (tgt->ior) {
		disksort(tgt->ior, ior);
		simple_unlock(&tgt->target_lock);
	} else {
		ior->io_next = 0;
		tgt->ior = ior;
		simple_unlock(&tgt->target_lock);
		scdisk_start(sc,tgt,FALSE);
	}
	splx(i);
}

/*#define CHECKSUM*/
#ifdef	CHECKSUM
int max_checksum_size = 0x2000;
#endif	CHECKSUM

/*
 * Start/completion routine for disks
 */
scdisk_start( sc, tgt, done)
	scsi_softc_t	*sc;
	target_info_t	*tgt;
	boolean_t	done;
{
	register io_req_t	ior = tgt->ior;
	register unsigned	part, secno;
	scsi_ret_t		ret;

	if (ior == 0)
		return;

	if (tgt->flags & TGT_BBR_ACTIVE)
		return scdisk_bbr_start( sc, tgt, done);

	if (done) {
		register unsigned int	xferred;

		/* see if we must retry */
		if ((tgt->done == SCSI_RET_RETRY) &&
		    ((ior->io_op & IO_INTERNAL) == 0)) {
			delay(1000000);/*XXX*/
			goto start;
		} else
		/* got a bus reset ? pifff.. */
		if (tgt->done == (SCSI_RET_ABORTED|SCSI_RET_RETRY)) {
			if (xferred = ior->io_residual) {
				ior->io_data -= xferred;
				ior->io_count += xferred;
				ior->io_recnum -= xferred / DEV_BSIZE;
				ior->io_residual = 0;
			}
			goto start;
		} else
		/*
		 * Quickly check for errors: if anything goes wrong
		 * we do a request sense, see if that is what we did.
		 */
		if (tgt->cur_cmd == SCSI_CMD_REQUEST_SENSE) {
			scsi_sense_data_t	*sns;
			unsigned int		blockno;
			char			*outcome;

			ior->io_op = ior->io_temporary;

			sns = (scsi_sense_data_t *)tgt->cmd_ptr;
			if (sns->addr_valid)
				blockno = sns->u.xtended.info0 << 24 |
					  sns->u.xtended.info1 << 16 |
					  sns->u.xtended.info2 <<  8 |
					  sns->u.xtended.info3;
			else {
				part     = rzpartition(ior->io_unit);
				blockno  = tgt->dev_info.disk.l.d_partitions[part].p_offset;
				blockno += ior->io_recnum;
			}

			if (scsi_check_sense_data(tgt, sns)) {
				ior->io_error = 0;
				outcome = "Recovered";
			} else {
				outcome = "Unrecoverable";
				ior->io_error = D_IO_ERROR;
				ior->io_op |= IO_ERROR;
			}
			if ((tgt->flags & TGT_OPTIONAL_CMD) == 0) {
			    printf("%s Error, rz%d: %s%s%d\n", outcome,
				tgt->target_id + (sc->masterno * 8),
				(ior->io_op & IO_READ) ? "Read" :
				 ((ior->io_op & IO_INTERNAL) ? "(command)" : "Write"),
				" disk error, phys block no. ", blockno);
{unsigned int p[7];
bcopy(tgt->cmd_ptr, (char*)p, sizeof(p));
printf("rz_disk sense data: %x %x %x %x %x %x %x, ",
p[0],p[1],p[2],p[3],p[4],p[5],p[6]);
}
			    /*
			     * On fatal read/write errors try replacing the bad block
			     * The bbr routine will return TRUE iff it took control
			     * over the target for all subsequent operations. In this
			     * event, the queue of requests is effectively frozen.
			     */
			    if (ior->io_error && 
				((sns->error_class == SCSI_SNS_XTENDED_SENSE_DATA) &&
				 ((sns->u.xtended.sense_key == SCSI_SNS_HW_ERR) ||
				  (sns->u.xtended.sense_key == SCSI_SNS_MEDIUM_ERR))) &&
			    	scdisk_bad_block_repl(sc, tgt, blockno))
				    return;
			}
		}

		/*
	 	 * See if we had errors
		 */
		else if (tgt->done != SCSI_RET_SUCCESS) {

			if (tgt->done == SCSI_RET_NEED_SENSE) {

				ior->io_temporary = ior->io_op;
				ior->io_op = IO_INTERNAL;
				scsi_request_sense(sc, tgt, ior, 0);
				return;

			} else if (tgt->done == SCSI_RET_DEVICE_DOWN) {
				ior->io_error = D_DEVICE_DOWN;
				ior->io_op |= IO_ERROR;
			} else {
				printf("%s%x\n", "?rz_disk Disk error, ret=x", tgt->done);
				ior->io_error = D_IO_ERROR;
				ior->io_op |= IO_ERROR;
			}
		}
		/*
		 * No errors.
		 * See if we requested more than the max
		 * (We use io_residual in a flip-side way here)
		 */
		else if (ior->io_count > (xferred = sc->max_dma_data)) {
			ior->io_residual += xferred;
			ior->io_count -= xferred;
			ior->io_data += xferred;
			ior->io_recnum += xferred / DEV_BSIZE;
			goto start;
		}
		else if (xferred = ior->io_residual) {
			ior->io_data -= xferred;
			ior->io_count += xferred;
			ior->io_recnum -= xferred / DEV_BSIZE;
			ior->io_residual = 0;
		} /* that's it */

#ifdef	CHECKSUM
		if ((ior->io_op & IO_READ) && (ior->io_count < max_checksum_size)) {
			part = rzpartition(ior->io_unit);
			secno = ior->io_recnum + tgt->dev_info.disk.l.d_partitions[part].p_offset;
			scdisk_bcheck(secno, ior->io_data, ior->io_count);
		}
#endif	CHECKSUM

		/* dequeue next one */
		{
			io_req_t	next;

			simple_lock(&tgt->target_lock);
			next = ior->io_next;
			tgt->ior = next;
			simple_unlock(&tgt->target_lock);

			iodone(ior);
			if (next == 0)
				return;

			ior = next;
		}

#ifdef	CHECKSUM
		if (((ior->io_op & IO_READ) == 0) && (ior->io_count < max_checksum_size)) {
			part = rzpartition(ior->io_unit);
			secno = ior->io_recnum + tgt->dev_info.disk.l.d_partitions[part].p_offset;
			scdisk_checksum(secno, ior->io_data, ior->io_count);
		}
#endif	CHECKSUM
	}
	ior->io_residual = 0;
start:
	return scdisk_start_rw( sc, tgt, ior);
}

scdisk_start_rw( sc, tgt, ior)
	scsi_softc_t	*sc;
	target_info_t	*tgt;
	register io_req_t	ior;
{
	unsigned int	part, secno;
	register boolean_t	long_form;
	scsi_ret_t		ret;

	part = rzpartition(ior->io_unit);
	secno = ior->io_recnum + tgt->dev_info.disk.l.d_partitions[part].p_offset;

	/* Use long form if either big block addresses or
	   the size is more than we can fit in one byte */
	long_form = (tgt->flags & TGT_BIG) ||
		    (ior->io_count > (256 * tgt->block_size));
	if (ior->io_op & IO_READ)
		ret = (long_form ? scsi_long_read : scdisk_read)
					(sc, tgt, secno, ior);
	else if ((ior->io_op & IO_INTERNAL) == 0)
		ret = (long_form ? scsi_long_write : scdisk_write)
					(sc, tgt, secno, ior);
	return ret;
}

#include <sys/ioctl.h>
#ifdef	ULTRIX_COMPAT
#include <mips/PMAX/rzdisk.h>
#endif	/*ULTRIX_COMPAT*/

io_return_t
scdisk_get_status(sc, tgt, flavor, status, status_count)
	scsi_softc_t	*sc;
	target_info_t	*tgt;
	int		flavor;
	dev_status_t	status;
	unsigned int	*status_count;

{
	struct disklabel *lp;

	lp = &tgt->dev_info.disk.l;

	switch (flavor) {
	case DIOCGDINFO:
		*(struct disklabel *)status = *lp;
#ifdef	MACH_KERNEL
		*status_count = sizeof(struct disklabel)/sizeof(int);
#endif	MACH_KERNEL
		break;

#ifdef	MACH_KERNEL
#else	/*MACH_KERNEL*/
#if	ULTRIX_COMPAT
	case SCSI_MODE_SENSE:		/*_IOWR(p, 9, struct mode_sel_sns_params) */
		break;
	case DIOCGETPT:			/*_IOR(p, 1, struct pt) */
	case SCSI_GET_SENSE:		/*_IOR(p, 10, struct extended_sense) */
		return ul_disk_ioctl(sc, tgt, flavor, status, status_count);
#endif	/*ULTRIX_COMPAT*/
#endif	/*!MACH_KERNEL*/

#if 0
	case DIOCRFORMAT:
		break;
#endif
	default:
		return D_INVALID_OPERATION;
	}
	return D_SUCCESS;
}

io_return_t
scdisk_set_status(sc, tgt, flavor, status, status_count)
	scsi_softc_t	*sc;
	target_info_t	*tgt;
	int		flavor;
	dev_status_t	status;
	unsigned int	status_count;

{
	io_return_t error = D_SUCCESS;
	struct disklabel *lp;

	lp = &tgt->dev_info.disk.l;


	switch (flavor) {
	case DIOCSRETRIES:
#ifdef	MACH_KERNEL
		if (status_count != sizeof(int))
			return D_INVALID_SIZE;
#endif	/* MACH_KERNEL */
		scsi_bbr_retries = *(int *)status;
		break;

	case DIOCWLABEL:
		if (*(int*)status)
			tgt->flags |= TGT_WRITE_LABEL;
		else
			tgt->flags &= ~TGT_WRITE_LABEL;
		break;
	case DIOCSDINFO:
	case DIOCWDINFO:
#ifdef	MACH_KERNEL
		if (status_count != sizeof(struct disklabel) / sizeof(int))
			return D_INVALID_SIZE;
#endif	/* MACH_KERNEL */
		error = setdisklabel(lp, (struct disklabel*) status, 0);
		if (error || (flavor == DIOCSDINFO))
			return error;
		error = scdisk_writelabel(sc, tgt);
		break;

#ifdef	MACH_KERNEL
#else	/*MACH_KERNEL*/
#if	ULTRIX_COMPAT
	case SCSI_FORMAT_UNIT:		/*_IOW(p, 4, struct format_params) */
	case SCSI_REASSIGN_BLOCK:	/*_IOW(p, 5, struct reassign_params) */
	case SCSI_READ_DEFECT_DATA:	/*_IOW(p, 6, struct read_defect_params) */
	case SCSI_VERIFY_DATA:		/*_IOW(p, 7, struct verify_params) */
	case SCSI_MODE_SELECT:		/*_IOW(p, 8, struct mode_sel_sns_params) */
	case SCSI_MODE_SENSE:		/*_IOW(p, 9, struct mode_sel_sns_params) */
	case SCSI_GET_INQUIRY_DATA:	/*_IOW(p, 11, struct inquiry_info) */
		return ul_disk_ioctl(sc, tgt, flavor, status, status_count);
#endif	/*ULTRIX_COMPAT*/
#endif	/*!MACH_KERNEL*/

#if notyet
	case DIOCWFORMAT:
	case DIOCSBAD:	/* ?? how ? */
#endif
	default:
		error = D_INVALID_OPERATION;
	}
	return error;
}

#ifdef	MACH_KERNEL

/* Perform some checks and then copy a disk label */
setdisklabel(lp, nlp, xxx)
	struct disklabel *lp, *nlp;
{
	if (nlp->d_magic != DISKMAGIC || nlp->d_magic2 != DISKMAGIC ||
	    dkcksum(nlp != 0))
		return D_INVALID_OPERATION;
	*lp = *nlp;
	return D_SUCCESS;
}

/* Checksum a disk label */
dkcksum(lp)
	struct disklabel *lp;
{
	register unsigned short *start, *end, sum = 0;

	start = (unsigned short *)lp;
	end = (unsigned short*)&lp->d_partitions[lp->d_npartitions];
	while (start < end) sum ^= *start++;
	return sum;
}

#endif	/* MACH_KERNEL */

/* Write back a label to the disk */
scdisk_writelabel(sc, tgt)
	scsi_softc_t	*sc;
	target_info_t	*tgt;
{
	io_req_t	ior;
	char		buf[DEV_BSIZE], *data;
	struct disklabel *label;
	io_return_t	error;

	/* XXX too mild a check */
	if (tgt->ior != 0)
		return D_ALREADY_OPEN;

	io_req_alloc(ior,0);
#ifdef	MACH_KERNEL
	data = buf;
#else	/*MACH_KERNEL*/
	data = (char *)ior->io_data;
#endif	/*MACH_KERNEL*/
	ior->io_next = 0;
	ior->io_count = 0;
	ior->io_data = data;
	ior->io_count = DEV_BSIZE;
	ior->io_op = IO_READ;
	tgt->ior = ior;
	scdisk_read( sc, tgt, LABELSECTOR, ior);
	iowait(ior);
	if (error = ior->io_error)
		goto ret;

	label = (struct disklabel *) &data[LABELOFFSET % DEV_BSIZE];
	*label = tgt->dev_info.disk.l;

	ior->io_next = 0;
	ior->io_count = 0;
	ior->io_count = DEV_BSIZE;
	ior->io_op = IO_WRITE;
	tgt->ior = ior;
	scdisk_write(sc, tgt, LABELSECTOR, ior);
	iowait(ior);

	error = ior->io_error;
ret:
	io_req_free(ior);
	return error;
}

#ifdef	MACH_KERNEL
#else	/*MACH_KERNEL*/
#if	ULTRIX_COMPAT

ul_disk_ioctl(sc, tgt, flavor, status, status_count)
	scsi_softc_t	*sc;
	target_info_t	*tgt;
	int		flavor;
	dev_status_t	status;
	unsigned int	status_count;
{
	io_return_t	ret;
	scsi_ret_t	err = SCSI_RET_ABORTED;/*xxx*/
	io_req_t	ior;

	if (!suser())
		return EACCES;

	ior = geteblk(sizeof(struct defect_descriptors));
	ior->io_next = 0;
	ior->io_count = 0;
	ior->io_op = IO_INTERNAL;
	ior->io_error = 0;
	ior->io_recnum = 0;
	ior->io_residual = 0;

	switch (flavor) {

	case DIOCGETPT: {			/*_IOR(p, 1, struct pt) */
		scsi_dec_label_t	*p;
		struct disklabel	*lp;
		int			i;

		lp = &tgt->dev_info.disk.l;
		p = (scsi_dec_label_t *)status;

		p->magic = DEC_PARTITION_MAGIC;
		p->in_use = 1;
		for (i = 0; i < 8; i++) {
			p->partitions[i].n_sectors = lp->d_partitions[i].p_size;
			p->partitions[i].offset = lp->d_partitions[i].p_offset;
		}
		err = SCSI_RET_SUCCESS;
	    }
	    break;

	case SCSI_GET_SENSE: {		/*_IOR(p, 10, struct extended_sense) */
		scsi_sense_data_t	*s;

		s = (scsi_sense_data_t*)tgt->cmd_ptr;
		bcopy(s, status, sizeof(*s) + s->u.xtended.add_len - 1);
		err = SCSI_RET_SUCCESS;
		/* only once */
		bzero(tgt->cmd_ptr, sizeof(scsi_sense_data_t));
	    }
	    break;

	case SCSI_GET_INQUIRY_DATA: {	/*_IOR(p, 11, struct inquiry_info) */
		struct mode_sel_sns_params *ms;

		ms = (struct mode_sel_sns_params*)status;
		err = scsi_inquiry( sc, tgt, SCSI_INQ_STD_DATA);
		if (copyout(tgt->cmd_ptr, ms->msp_addr, sizeof(struct inquiry_info))){
			ret = EFAULT;
			goto out;
		}
	    }
	    break;

	case SCSI_FORMAT_UNIT: {	/*_IOW(p, 4, struct format_params) */
		struct format_params *fp;
		struct defect_descriptors *df;
		unsigned char	mode;
		unsigned int	old_timeout;

		fp = (struct format_params *)status;
		df = (struct defect_descriptors*)ior->io_data;
		if (fp->fp_length != 0) {
			if (copyin(fp->fp_addr, df, sizeof(*df))) {
				ret = EFAULT;
				goto out;
			}
			ior->io_count = sizeof(*df);
		} else
			ior->io_count = 0;
		mode = fp->fp_format & SCSI_CMD_FMT_LIST_TYPE;
		switch (fp->fp_defects) {
		case VENDOR_DEFECTS:
			mode |= SCSI_CMD_FMT_FMTDATA|SCSI_CMD_FMT_CMPLIST;
			break;
		case KNOWN_DEFECTS:
			mode |= SCSI_CMD_FMT_FMTDATA;
			break;
		case NO_DEFECTS:
		default:
			break;
		}
		old_timeout = scsi_watchdog_period;
		scsi_watchdog_period = 60*60;	/* 1 hour should be enough, I hope */
		err = scsi_format_unit( sc, tgt, mode, fp->fp_pattern,
					fp->fp_interleave, ior);
		scsi_watchdog_period = old_timeout;
		/* Make sure we re-read all info afresh */
		tgt->flags = TGT_ALIVE |
			     (tgt->flags & (TGT_REMOVABLE_MEDIA|TGT_FULLY_PROBED));
	    }
	    break;

	case SCSI_REASSIGN_BLOCK: {	/*_IOW(p, 5, struct reassign_params) */
		struct reassign_params	*r;
		int			ndef;

		r = (struct reassign_params*) status;
		ndef = r->rp_header.defect_len0 | (r->rp_header.defect_len1 >> 8);
		ndef >>= 2;
		tgt->ior = ior;
		(void) scsi_reassign_blocks( sc, tgt, &r->rp_lbn3, ndef, ior);
		iowait(ior);
		err = tgt->done;
	    }
	    break;

	case SCSI_READ_DEFECT_DATA: {	/*_IOW(p, 6, struct read_defect_params) */
		struct read_defect_params *dp;

		dp = (struct read_defect_params *)status;
		ior->io_count = ior->io_alloc_size;
		if (dp->rdp_alclen > ior->io_count)
			dp->rdp_alclen = ior->io_count;
		else
			ior->io_count = dp->rdp_alclen;
		ior->io_op |= IO_READ;
		tgt->ior = ior;
		err = scsi_read_defect(sc, tgt, dp->rdp_format|0x18, ior);
		if (copyout(ior->io_data, dp->rdp_addr, dp->rdp_alclen)) {
			ret = EFAULT;
			goto out;
		}
	    }
	    break;

	case SCSI_VERIFY_DATA: {	/*_IOW(p, 7, struct verify_params) */
		struct verify_params	*v;
		unsigned int	old_timeout;

		old_timeout = scsi_watchdog_period;
		scsi_watchdog_period = 5*60;	/* 5 mins enough, I hope */
		v = (struct verify_params *)status;
		ior->io_count = 0;
		err = scdisk_verify( sc, tgt, v->vp_lbn, v->vp_length, ior);
		scsi_watchdog_period = old_timeout;
	    }
	    break;

	case SCSI_MODE_SELECT: {	/*_IOW(p, 8, struct mode_sel_sns_params) */
		struct mode_sel_sns_params *ms;

		ms = (struct mode_sel_sns_params*)status;
		if(copyin(ms->msp_addr, ior->io_data, ms->msp_length)) {
			ret = EFAULT;
			goto out;
		}
		err = scdisk_mode_select( sc, tgt, DEV_BSIZE, ior, ior->io_data,
					  ms->msp_length, ms->msp_setps);
	    }
	    break;

	case SCSI_MODE_SENSE: {		/*_IOWR(p, 9, struct mode_sel_sns_params) */
		struct mode_sel_sns_params *ms;
		unsigned char		pagecode;

		ms = (struct mode_sel_sns_params*)status;
		pagecode = (ms->msp_pgcode & 0x3f) | (ms->msp_pgctrl << 6);
		err = scsi_mode_sense( sc, tgt, pagecode, ms->msp_length, ior);
		if (copyout(tgt->cmd_ptr, ms->msp_addr, ms->msp_length)){
			ret = EFAULT;
			goto out;
		}
	    }
	    break;
	}

	ret = (err == SCSI_RET_SUCCESS) ? D_SUCCESS : D_IO_ERROR;
	if (ior->io_op & IO_ERROR)
		ret = D_IO_ERROR;
out:
	brelse(ior);
	return ret;
}
#endif	/*ULTRIX_COMPAT*/
#endif	/*!MACH_KERNEL*/

#ifdef	CHECKSUM

#define SUMSIZE 0x10000
#define SUMHASH(b)	(((b)>>1) & (SUMSIZE - 1))
struct {
	long blockno;
	long sum;
} scdisk_checksums[SUMSIZE];

scdisk_checksum(bno, addr, size)
	long bno;
	register unsigned int *addr;
{
	register int i = size/sizeof(int);
	register unsigned int sum = -1;

	while (i-- > 0)
		sum ^= *addr++;
	scdisk_checksums[SUMHASH(bno)].blockno = bno;
	scdisk_checksums[SUMHASH(bno)].sum = sum;
}

scdisk_bcheck(bno, addr, size)
	long bno;
	register unsigned int *addr;
{
	register int i = size/sizeof(int);
	register unsigned int sum = -1;
	unsigned int *start = addr;

	if (scdisk_checksums[SUMHASH(bno)].blockno != bno) {
if (scsi_debug) printf("No checksum for block x%x\n", bno);
		return;
	}

	while (i-- > 0)
		sum ^= *addr++;

	if (scdisk_checksums[SUMHASH(bno)].sum != sum) {
		printf("Bad checksum (x%x != x%x), bno x%x size x%x at x%x\n",
			sum,
			scdisk_checksums[bno & (SUMSIZE - 1)].sum,
			bno, size, start);
		gimmeabreak();
		scdisk_checksums[SUMHASH(bno)].sum = sum;
	}
}


#endif CHECKSUM

#define PERF
#ifdef	PERF
int test_read_size = 512;
int test_read_skew = 12;
int test_read_skew_min = 0;
int test_read_nreads = 1000;
int test_read_bdev = 0;

#include <sys/time.h>

test_read(max)
{
	int i, ssk, usecs;
	struct timeval start, stop;

	if (max == 0)
		max = test_read_skew + 1;
	ssk = test_read_skew;
	for (i = test_read_skew_min; i < max; i++){
		test_read_skew = i;

		start = time;
		read_test();
		stop = time;

		usecs = stop.tv_usec - start.tv_usec;
		if (usecs < 0) {
			stop.tv_sec -= 1;
			usecs += 1000000;
		}
		printf("Skew %3d size %d count %d time %3d sec %d us\n",
			i, test_read_size, test_read_nreads,
			stop.tv_sec - start.tv_sec, usecs);
	}
	test_read_skew = ssk;
}

read_test()
{
	static char	buffer[8192*2];
	struct io_req	io;
	register int 	i, rec;

	bzero(&io, sizeof(io));
	io.io_unit = test_read_bdev;
	io.io_op = IO_READ;
	io.io_count = test_read_size;
	io.io_data = buffer;

	for (rec = 0, i = 0; i < test_read_nreads; i++) {
		io.io_op = IO_READ;
		io.io_recnum = rec;
		scdisk_strategy(&io);
		rec += test_read_skew;
		iowait(&io);
	}
}

tur_test()
{
	struct io_req	io;
	register int 	i;
	char		*a, *b;
	struct timeval start, stop;

	bzero(&io, sizeof(io));
	io.io_unit = test_read_bdev;
	io.io_data = (char*)&io;/*unused but kernel space*/

	start = time;
	for (i = 0; i < test_read_nreads; i++) {
		io.io_op = IO_INTERNAL;
		rz_check(io.io_unit, &a, &b);
		scsi_test_unit_ready(a,b,&io);
	}
	stop = time;
	i = stop.tv_usec - start.tv_usec;
	if (i < 0) {
		stop.tv_sec -= 1;
		i += 1000000;
	}
	printf("%d test-unit-ready took %3d sec %d us\n",
			test_read_nreads,
			stop.tv_sec - start.tv_sec, i);
}

#endif	PERF

/*#define WDEBUG*/
#ifdef	WDEBUG

int buggo_write_size = 8192;
int buggo_dev = 1; /* rz0b */
int	buggo_out_buffer[8192/2];
int	buggo_in_buffer[8192/2];
buggotest(n, pattern, verbose)
{
	struct io_req	io;
	register int 	i, rec;

	if (n <= 0)
		n = 1;

	if(pattern)
		for (i = 0; i < buggo_write_size/4; i++)
			buggo_out_buffer[i] = i + pattern;

	for (i = 0; i < n; i++) {
		register int j;

		buggo_out_buffer[0] = i + pattern;
		buggo_out_buffer[(buggo_write_size/4)-1] = i + pattern;
		bzero(&io, sizeof(io));
		io.io_unit = buggo_dev;
		io.io_data = (char*)buggo_out_buffer;
		io.io_op = IO_WRITE;
		io.io_count = buggo_write_size;
		io.io_recnum = i % 1024;
		scdisk_strategy(&io);

		bzero(buggo_in_buffer, sizeof(buggo_in_buffer));
		iowait(&io);

		if (verbose)
			printf("Done write with %x", io.io_error);

		bzero(&io, sizeof(io));
		io.io_unit = buggo_dev;
		io.io_data = (char*)buggo_in_buffer;
		io.io_op = IO_READ;
		io.io_count = buggo_write_size;
		io.io_recnum = i % 1024;
		scdisk_strategy(&io);
		iowait(&io);

		if (verbose)
			printf("Done read with %x", io.io_error);

		for  (j = 0; j < buggo_write_size/4; j++)
			if (buggo_out_buffer[j] != buggo_in_buffer[j]){
				printf("Difference at %d-th word: %x %x\n",
					buggo_out_buffer[j], buggo_in_buffer[j]);
				return i;
			}
	}
	printf("Test ok\n");
	return n;
}


#endif	WDEBUG
