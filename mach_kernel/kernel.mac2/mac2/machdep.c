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
 * $Log:	machdep.c,v $
 * Revision 2.2  91/09/12  16:41:29  bohman
 * 	Created Mach 3.0 version from 2.5 version.
 * 	[91/09/11  14:50:52  bohman]
 * 
 */

/*
 *	Apple Macintosh II Mach (macmach)
 *
 *	File: mac2/machdep.c
 *	Author: David E. Bohman II (CMU macmach)
 */

#include <mach/mach_types.h>
#include <mach/vm_param.h>

#include <device/device_types.h>
#include <device/io_req.h>

#include <vm/pmap.h>

#include <mac2/cpu.h>
#include <mac2/machparam.h>

#include <mac2os/Globals.h>
#include <mac2os/Types.h>
#include <mac2os/Errors.h>
#include <mac2os/Files.h>
#include <mac2os/Timer.h>

#include <mac2/slots.h>

extern char	version[];
extern int	cold;

extern void	setup_main();
extern void	inittodr();

/*
 * Machine dependent initialization
 * routines.
 */
void
machine_startup()
{
    extern int		hz;
    extern boolean_t	is68030;
#ifdef MODE24
    extern boolean_t	mac32bit;
#endif /* MODE24 */

    master_cpu = cpu_number();

    printf(version);
#ifdef MODE24
    printf("MacMach %s cpu, 24-bit compatible in %s mode\n",
      is68030 ? "68030" : "68020", mac32bit ? "32-bit" : "24-bit");
#else /* MODE24 */
    printf("MacMach %s cpu, 32-bit clean\n", is68030 ? "68030" : "68020");
#endif /* MODE24 */
    printf("kernel base is 0x%x\n", *(unsigned *)0);

    (void) spl0();

    /*
     * Configure the CPU
     */
    machine_slot[0].is_cpu = TRUE;
    machine_slot[0].running = TRUE;
    if (is68030) {
	machine_slot[0].cpu_type = CPU_TYPE_MC68030;
	machine_slot[0].cpu_subtype = CPU_SUBTYPE_MAC2_030;
    }
    else {
	machine_slot[0].cpu_type = CPU_TYPE_MC68020;
	machine_slot[0].cpu_subtype = CPU_SUBTYPE_MAC2_020;
    }
    machine_slot[0].clock_freq = hz;

    /*
     * Configure all slot
     * devices.
     */
    config_slots();

    /*
     * Now that configure is done, write protect
     * the interrrupt vector page in the kernel pmap.
     */
    pmap_protect(kernel_pmap,
		 (vm_offset_t)ivect_tbl,
		 (vm_offset_t)ivect_tbl+MAC2_PGBYTES,
		 VM_PROT_READ);

    setup_main();
}

void
machine_init()
{
#ifdef notdef
    static VolumeParam	params;

    /*
     * Take the default volume off-line.
     */
    params.ioNamePtr = 0;
    params.ioVRefNum = ((VCB *)(DefVCBPtr))->vcbDrvNum;
    (void) PBOffLine(&params);
#endif

    scsi_initialize();

    sdisk_initialize();

    get_root_device();

    inittodr();
}

/*
 * Halt/Reboot cpu's
 */
void
halt_cpu()
{
    spl5();
    printf("cpu halted\n");
    for (;;)
	;
}

void
halt_all_cpus(reboot)
boolean_t	reboot;
{
    if (!reboot)
	halt_cpu();
    else
	boot_inst();
}

/*
 * Device routines that
 * probably belong somewhere
 * else.
 */

/*
 * The 'time' device provides
 * two different functions.  First,
 * via device_map() it allows a
 * read-only shared page to be mapped
 * into a task that contains the continually
 * updated system time of day.  This 
 * functionality is assumed by the UX single
 * server, as well as probably other systems.
 * Secondly, the read function provides access
 * to the MacOS Time Manager as required by
 * the macintosh emulation.  Please note that
 * this is not in a finished state, and should
 * be redone as a separate machine dependent
 * IPC kernel interface.
 */
static queue_head_t	time_queue;
static boolean_t	time_queue_initialized;

io_return_t
timeread(dev, ior)
register io_req_t	ior;
{
    register io_req_t		r;
    register queue_t		q = &time_queue;
    register queue_entry_t	qe;
    register TMTaskPtr		tm;
    register			s;
    boolean_t			timeread_done();
    extern void			timeread_task();

    if (!time_queue_initialized) {
	queue_init(q);
	time_queue_initialized = TRUE;
    }

    if (!(ior->io_op & IO_INBAND))
	return (D_INVALID_OPERATION);

    s = splclock();
    for (qe = queue_first(q); !queue_end(q, qe); qe = queue_next(qe)) {
	r = (io_req_t)qe;
	if (r->io_reply_port == ior->io_reply_port)
	    break;
    }

    if (!queue_end(q, qe)) {
	remqueue(q, r);
	RmvTime(r->io_dev_ptr);
	ior->io_dev_ptr = r->io_dev_ptr;
	r->io_dev_ptr = 0;
	iodone(r);
	splx(s);
    }
    else {
	splx(s);
	ior->io_dev_ptr = 0;
    }

    ior->io_count = 0;

    if (ior->io_recnum == 0) {
	if (ior->io_dev_ptr)
	    DisposPtr(ior->io_dev_ptr);

	return (D_SUCCESS);
    }

    if (ior->io_dev_ptr == 0
	&& ((Ptr)ior->io_dev_ptr = (Ptr)NewPtr(sizeof (TMTask))) == 0)
	return (D_NO_MEMORY);

    ior->io_done = timeread_done;

    enqueue_tail(q, ior);

    tm = (TMTaskPtr)ior->io_dev_ptr;
    tm->qLink = 0;
    tm->qType = 0;
    tm->tmAddr = timeread_task;
    tm->tmCount = 0;
    InsTime(tm);
    PrimeTime(tm, ior->io_recnum);

    return (D_IO_QUEUED);
}

void
timeread_intr(tm)
register TMTaskPtr	tm;
{
    register io_req_t		ior;
    register queue_t		q = &time_queue;
    register queue_entry_t	qe;

    for (qe = queue_first(q); !queue_end(q, qe); qe = queue_next(qe)) {
	ior = (io_req_t)qe;
	if ((TMTaskPtr)ior->io_dev_ptr == tm) {
	    remqueue(q, ior);
	    RmvTime(ior->io_dev_ptr);
	    iodone(ior);
	    break;
	}
    }
}

boolean_t
timeread_done(ior)
register io_req_t	ior;
{
    if (ior->io_dev_ptr) {
	DisposPtr(ior->io_dev_ptr);

	if (ior->io_op & IO_INBAND)
	    (void) ds_device_read_reply_inband(ior->io_reply_port,
					       ior->io_reply_port_type,
					       D_SUCCESS,
					       0, 0);
	else
	    (void) ds_device_read_reply(ior->io_reply_port,
					ior->io_reply_port_type,
					D_SUCCESS,
					0, 0);
    }

    device_deallocate(ior->io_device);

    return (TRUE);
}

timemmap(dev, offset, prot)
vm_prot_t	prot;
{
    extern time_value_t *mtime;

    if (prot & VM_PROT_WRITE)
	return (-1);

    return (mac2_btop(pmap_extract(pmap_kernel(), (vm_offset_t) mtime)));
}

/*
 * The 'kern' device provides two
 * functions.  First, it allows the
 * Macintosh ROM and NuBus slots
 * to be mapped into a task.  Secondly,
 * getstat & setstat allow the macintosh
 * parameter ram to be read/written.
 */
static struct kern_valid_region {
    vm_offset_t	    offset;
    vm_size_t	    size;
    vm_prot_t	    perm;
} kern_valid_region[] = {
    { 0x40800000, (512*1024),
	  VM_PROT_EXECUTE|VM_PROT_READ },	/* ROM */
    { 0xf9000000, (6*16*1024*1024),
	  VM_PROT_READ|VM_PROT_WRITE },		/* NUBUS */
#define KERN_VALID_REGION_SLOT	1
    { 0 }
};

static
boolean_t
kern_valid_region_check(offset, size, perm)
register vm_offset_t	offset;
register vm_size_t	size;
register vm_prot_t	perm;
{
    register struct kern_valid_region	*p;

    for (p = kern_valid_region; ; p++) {
	if (p->offset == 0)
	    return (FALSE);

	if (offset < p->offset)
	    return (FALSE);

	if (offset < (p->offset + p->size))
	    break;
    }

    if (!(perm & p->perm))
	return (FALSE);

    if (p == &kern_valid_region[KERN_VALID_REGION_SLOT]) {
	register 	slot;
	int		slot_lo, slot_hi;
#define slot_ptr_to_slot(p)	(((vm_offset_t)(p) & 0x0f000000) >> 24)

	slot_lo = slot_ptr_to_slot(offset);
	slot_hi = slot_ptr_to_slot(offset+size-1);

	for (slot = slot_lo; slot <= slot_hi; slot++) 
	    if ((slot_to_slotdata_ptr(slot)->SFlags&SLOT_MAPPABLE) == 0)
		return (FALSE);
    }

    return (TRUE);
}

kernmmap(dev, offset, prot)
vm_offset_t	offset;
{
    if (!kern_valid_region_check(offset, MAC2_PGBYTES, prot))
	return (-1);

    return (mac2_btop(pmap_extract(pmap_kernel(), offset)));
}

io_return_t
kerngetstat(dev, flavor, status, count)
int		flavor;
dev_status_t	status;
unsigned	*count;
{
    register io_return_t	result;

    if (flavor & 0x80000000) {
	struct {
	    unsigned short	len;
	    unsigned short	addr;
	} *arg = (typeof (arg))&flavor;

	flavor &= ~0x80000000;

	if (arg->len <= (DEV_STATUS_MAX * sizeof (int))
	    && ReadXPRam(status, flavor) == noErr) {
	    *count = (arg->len + sizeof (int) - 1) / sizeof (int);
	    result = D_SUCCESS;
	}
	else
	    result = D_INVALID_OPERATION;
    }
    else
	result = D_INVALID_OPERATION;

    return (result);
}

io_return_t
kernsetstat(dev, flavor, status, count)
int		flavor;
dev_status_t	status;
unsigned	count;
{
    register io_return_t	result;

    if (flavor & 0x80000000) {
	struct {
	    unsigned short	len;
	    unsigned short	addr;
	} *arg = (typeof (arg))&flavor;

	flavor &= ~0x80000000;

	if (arg->len <= (count * sizeof (int))
	    && WriteXPRam(status, flavor) == noErr)
	    result = D_SUCCESS;
	else
	    result = D_INVALID_OPERATION;
    }
    else
	result = D_INVALID_OPERATION;

    return (result);
}
