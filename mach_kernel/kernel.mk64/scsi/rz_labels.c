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
 * $Log:	rz_labels.c,v $
 * Revision 2.3  91/08/24  12:28:06  af
 * 	Created, splitting out DEC-specific code from rz_disk.c and
 * 	adding some more.
 * 	[91/06/26            af]
 * 
 */
/*
 *	File: rz_labels.c
 * 	Author: Alessandro Forin, Carnegie Mellon University
 *	Date:	6/91
 *
 *	Routines for various vendor's disk label formats.
 */

#include <platforms.h>

#include <mach/std_types.h>
#include <scsi/compat_30.h>
#include <scsi/scsi_defs.h>
#include <scsi/rz_labels.h>

/*
 * Find and convert from a DEC label to BSD
 */
boolean_t
rz_dec_label(sc, tgt, label, ior)
	scsi_softc_t		*sc;
	target_info_t		*tgt;
	struct disklabel	*label;
	io_req_t		ior;	
{
	/* here look for a DEC label */
	register scsi_dec_label_t	*part;
	char				*data;
	int				i;

	ior->io_count = DEV_BSIZE;
	ior->io_op = IO_READ;
	tgt->ior = ior;
	scdisk_read( sc, tgt, DEC_LABEL_BYTE_OFFSET/DEV_BSIZE, ior);
	iowait(ior);
	data = (char *)ior->io_data;
	part = (scsi_dec_label_t*)&data[DEC_LABEL_BYTE_OFFSET%DEV_BSIZE];
	if (part->magic == DEC_LABEL_MAGIC) {
		if (scsi_debug)
		printf("{Using DEC label}");
		for (i = 0; i < 8; i++) {
			label->d_partitions[i].p_size = part->partitions[i].n_sectors;
			label->d_partitions[i].p_offset = part->partitions[i].offset;
		}
		return TRUE;
	}
	return FALSE;
}

/*
 * Find and convert from a Omron label to BSD
 */
boolean_t
rz_omron_label(sc, tgt, label, ior)
	scsi_softc_t		*sc;
	target_info_t		*tgt;
	struct disklabel	*label;
	io_req_t		ior;	
{
	/* here look for an Omron label */
	register scsi_omron_label_t	*part;
	char				*data;
	int				i;

	ior->io_count = DEV_BSIZE;
	ior->io_op = IO_READ;
	tgt->ior = ior;
	scdisk_read( sc, tgt, OMRON_LABEL_BYTE_OFFSET/DEV_BSIZE, ior);
	iowait(ior);
	data = (char *)ior->io_data;
	part = (scsi_omron_label_t*)&data[OMRON_LABEL_BYTE_OFFSET%DEV_BSIZE];
	if (part->magic == OMRON_LABEL_MAGIC) {
		if (scsi_debug)
		printf("{Using OMRON label}");
		for (i = 0; i < 8; i++) {
			label->d_partitions[i].p_size = part->partitions[i].n_sectors;
			label->d_partitions[i].p_offset = part->partitions[i].offset;
		}
		bcopy(part->packname, label->d_packname, 16);
		label->d_ncylinders = part->ncyl;
		label->d_acylinders = part->acyl;
		label->d_ntracks = part->nhead;
		label->d_nsectors = part->nsect;
		/* Many disks have this wrong, therefore.. */
#if 0
		label->d_secperunit = part->maxblk;
#else
		label->d_secperunit = label->d_ncylinders * label->d_ntracks *
					label->d_nsectors;
#endif
		return TRUE;
	}
	return FALSE;
}

/*
 * Find and convert from a Intel BIOS label to BSD
 */
boolean_t
rz_bios_label(sc, tgt, label, ior)
	scsi_softc_t		*sc;
	target_info_t		*tgt;
	struct disklabel	*label;
	io_req_t		ior;	
{
	/* here look for a BIOS label */
	register scsi_bios_label_t	*part;
	char				*data;
	int				i;

	ior->io_count = DEV_BSIZE;
	ior->io_op = IO_READ;
	tgt->ior = ior;
	scdisk_read( sc, tgt, BIOS_LABEL_BYTE_OFFSET/DEV_BSIZE, ior);
	iowait(ior);
	data = (char *)ior->io_data;
	part = (scsi_bios_label_t*)&data[BIOS_LABEL_BYTE_OFFSET%DEV_BSIZE];
	if (part->magic == BIOS_LABEL_MAGIC) {
		struct bios_partition_info *bpart;
		if (scsi_debug)
		printf("{Using BIOS label}");
		bpart = (struct bios_partition_info *)part->partitions;
		for (i = 0; i < 4; i++) {
			label->d_partitions[i].p_size = bpart[i].n_sectors;
			label->d_partitions[i].p_offset = bpart[i].offset;
		}
#ifdef	AT386	/* this goes away real fast */
		grab_bob_label(sc, tgt, label, ior, bpart);
#else
		label->d_npartitions = 4;
#endif
		label->d_bbsize = BIOS_BOOT0_SIZE;
		return TRUE;
	}
	return FALSE;
}

/*
 * Initialize the following based on the chosen configuration,
 * patch dynamically if necessary.
 */
#if	defined(DECSTATION) || defined(VAXSTATION)
#define	VENDOR_LABEL_FUNCTION	rz_dec_label
#endif	/* DECSTATION || VAXSTATION */

#ifdef	LUNA88K
#define	VENDOR_LABEL_FUNCTION	rz_omron_label;
#endif	/*LUNA88K*/

#ifdef	AT386
#define	VENDOR_LABEL_FUNCTION	rz_bios_label;
#endif	AT386

#ifndef	VENDOR_LABEL_FUNCTION
#define	VENDOR_LABEL_FUNCTION	0
#endif

boolean_t (*rz_vendor_label)() =  VENDOR_LABEL_FUNCTION;
