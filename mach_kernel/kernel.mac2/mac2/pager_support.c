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
 * $Log:	pager_support.c,v $
 * Revision 2.2  91/09/12  16:41:59  bohman
 * 	Created.
 * 	[91/09/11  14:54:27  bohman]
 * 
 */

/*
 *	Apple Macintosh II Mach (macmach)
 *
 *	File: mac2/pager_support.c
 *	Author: David E. Bohman II (CMU macmach)
 */

#include <mach/mach_types.h>
#include <mach/vm_param.h>

#include <device/device_types.h>

#include <mac2dev/sdisk.h>

#include <mach/mach_user_internal.h>
#include <mach/mach_port_internal.h>

/*
 * This module replaces the
 * default_pager_setup() located in
 * kernel/boot_ufs/def_pager_setup.c
 * It allows the default_pager to
 * use a spare apple partition
 * for paging space.  It chooses
 * the first 'Apple_Scratch' partition
 * it encounters starting with SCSI 0.
 * The default pager currently supports
 * only a single paging space.
 */
void
default_pager_setup(master_host_port, master_device_port)
mach_port_t			master_host_port, master_device_port;
{
    register kern_return_t	result;
    register			nparts;
    register unsigned		paging_size;
    device_t			device;
    sd_part_info_t		status_info;
    unsigned			status_count;
    static unsigned char	device_name[5] = { 's', 'd' };
#define disk	(device_name[2])
#define part	(device_name[3])
    static io_return_t		pager_read_page(), pager_write_page();

    for (disk = '0'; disk < '7'; disk++) {
	part = 'a' + 1;
	if (device_open(master_device_port,
			D_READ|D_WRITE,
			device_name,
			&device) != D_SUCCESS)
	    continue;
	status_count = SD_PART_INFO_COUNT;
	if (device_get_status(device,
			      SD_PART_INFO,
			      &status_info, &status_count) != D_SUCCESS) {
	    (void) device_close(device);
	    (void) mach_port_deallocate(mach_task_self(), device);
	    device = MACH_PORT_NULL;
	    continue;
	}
/*
	printf("%s: type %s length %d\n",
	       device_name, status_info.type, status_info.length);
*/
	if (!strncmp("Apple_Scratch",
		     status_info.type,
		     sizeof (status_info.type)))
	    break;

	nparts = status_info.parts;

	(void) device_close(device);
	(void) mach_port_deallocate(mach_task_self(), device);
	device = MACH_PORT_NULL;

	for (part = 'a' + 2; part <= 'a' + nparts; part++) {
	    if (device_open(master_device_port,
			    D_READ|D_WRITE,
			    device_name,
			    &device) != D_SUCCESS)
		continue;
	    status_count = SD_PART_INFO_COUNT;
	    if (device_get_status(device,
				  SD_PART_INFO,
				  &status_info, &status_count) != D_SUCCESS) {
		(void) device_close(device);
		(void) mach_port_deallocate(mach_task_self(), device);
		device = MACH_PORT_NULL;
		continue;
	    }
/*
	    printf("%s: type %s length %d\n",
		   device_name, status_info.type, status_info.length);
*/
	    if (!strncmp("Apple_Scratch",
			 status_info.type,
			 sizeof (status_info.type)))
		break;

	    (void) device_close(device);
	    (void) mach_port_deallocate(mach_task_self(), device);
	    device = MACH_PORT_NULL;
	}

	if (device != MACH_PORT_NULL)
	    break;
    }

    if (device != MACH_PORT_NULL) {
	printf("paging partition: %d blocks \"Apple_Scratch\" on \"%s\"\n",
	    status_info.length, device_name);
	paging_size = (status_info.length << SD_REC_SIZE_LOG2);
    }
    else {
	printf("paging partition: \"Apple_Scratch\" not found\n");
	paging_size = 0;
    }

    create_default_partition(paging_size,
			     pager_read_page,
			     pager_write_page,
			     device);
}

/*
 * The read/write routines are
 * trivial.
 */
static
io_return_t
pager_read_page(device, offset, size, addr, size_read)
device_t	device;
vm_offset_t	offset;
vm_size_t	size;
vm_offset_t	*addr;
vm_size_t	*size_read;
{
    return (device_read(device,
			0,
			(recnum_t) (offset >> SD_REC_SIZE_LOG2),
			size,
			addr,
			size_read));
}    

static
io_return_t
pager_write_page(device, offset, addr, size, size_written)
device_t	device;
vm_offset_t	offset;
vm_offset_t	addr;
vm_size_t	size;
vm_size_t	*size_written;
{
    return (device_write(device,
			 0,
			 (recnum_t) (offset >> SD_REC_SIZE_LOG2),
			 addr,
			 size,
			 size_written));
}
