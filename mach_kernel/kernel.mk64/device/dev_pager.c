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
 * $Log:	dev_pager.c,v $
 * Revision 2.21  91/09/12  16:37:07  bohman
 * 	For device_map objects, don't setup the actual resident page mappings
 * 	at initialization time.  Instead, do it in response to actual requests
 * 	for the data.  I left in the old code for NORMA_VM.
 * 	[91/09/11  17:05:20  bohman]
 * 
 * Revision 2.20  91/08/28  11:26:19  jsb
 * 	For now, use older dev_pager code for NORMA_VM.
 * 	I'll fix this soon.
 * 	[91/08/28  11:24:45  jsb]
 * 
 * Revision 2.19  91/08/03  18:17:26  jsb
 * 	Corrected declaration of xmm_vm_object_lookup.
 * 	[91/07/17  13:54:47  jsb]
 * 
 * Revision 2.18  91/07/31  17:33:24  dbg
 * 	Use vm_object_page_map to allocate pages for a device-map object.
 * 	Its vm_page structures are allocated and freed by the VM system.
 * 	[91/07/30  16:46:43  dbg]
 * 
 * Revision 2.17  91/06/25  11:06:33  rpd
 * 	Fixed includes to avoid norma files unless they are really needed.
 * 	[91/06/25            rpd]
 * 
 * Revision 2.16  91/06/18  20:49:55  jsb
 * 	Removed extra include of norma_vm.h.
 * 	[91/06/18  18:47:29  jsb]
 * 
 * Revision 2.15  91/06/17  15:43:50  jsb
 * 	NORMA_VM: use xmm_vm_object_lookup, since we really need a vm_object_t;
 * 	use xmm_add_exception to mark device pagers as non-interposable.
 * 	[91/06/17  13:22:58  jsb]
 * 
 * Revision 2.14  91/05/18  14:29:38  rpd
 * 	Added proper locking for vm_page_insert.
 * 	[91/04/21            rpd]
 * 	Changed vm_page_init.
 * 	[91/03/24            rpd]
 * 
 * Revision 2.13  91/05/14  15:41:35  mrt
 * 	Correcting copyright
 * 
 * Revision 2.12  91/02/05  17:08:40  mrt
 * 	Changed to new Mach copyright
 * 	[91/01/31  17:27:40  mrt]
 * 
 * Revision 2.11  90/11/05  14:26:46  rpd
 * 	Fixed memory_object_terminate to return KERN_SUCCESS.
 * 	[90/10/29            rpd]
 * 
 * Revision 2.10  90/09/09  14:31:20  rpd
 * 	Use decl_simple_lock_data.
 * 	[90/08/30            rpd]
 * 
 * Revision 2.9  90/06/02  14:47:19  rpd
 * 	Converted to new IPC.
 * 	Renamed functions to mainline conventions.
 * 	Fixed private/fictitious bug in memory_object_init.
 * 	Fixed port leak in memory_object_terminate.
 * 	[90/03/26  21:49:57  rpd]
 * 
 * Revision 2.8  90/05/29  18:36:37  rwd
 * 	From rpd: set private to specify that the page structure is
 * 	ours, not fictitious.
 * 	[90/05/14            rwd]
 * 	From rpd: set private to specify that the page structure is
 * 	ours, not fictitious.
 * 	[90/05/14            rwd]
 * 
 * Revision 2.7  90/05/21  13:26:21  dbg
 * 	 	From rpd: set private to specify that the page structure is
 * 	 	ours, not fictitious.
 * 	 	[90/05/14            rwd]
 * 	 
 * 
 * Revision 2.6  90/02/22  20:02:05  dbg
 * 	Change PAGE_WAKEUP to PAGE_WAKEUP_DONE to reflect the fact that
 * 	it clears the busy flag.
 * 	[90/01/29            dbg]
 * 
 * Revision 2.5  90/01/11  11:41:53  dbg
 * 	De-lint.
 * 	[89/12/06            dbg]
 * 
 * Revision 2.4  89/09/08  11:23:24  dbg
 * 	Rewrite to run in kernel task (off user thread or
 * 	vm_pageout!)
 * 	[89/08/24            dbg]
 * 
 * Revision 2.3  89/08/09  14:33:03  rwd
 * 	Call round_page on incoming size to get to mach page.
 * 	[89/08/09            rwd]
 * 
 * Revision 2.2  89/08/05  16:04:51  rwd
 * 	Added char_pager code for frame buffer.
 * 	[89/07/26            rwd]
 * 
 * 26-May-89  Randall Dean (rwd) at Carnegie-Mellon University
 *	If no error, zero pad residual and set to 0
 *
 *  3-Mar-89  David Golub (dbg) at Carnegie-Mellon University
 *	Created.
 *
 */
/*
 *	Author: David B. Golub, Carnegie Mellon University
 *	Date: 	3/89
 *
 * 	Device pager.
 */
#include <norma_vm.h>
#if	NORMA_VM
#include <norma/xmm_server_rename.h>
#endif	NORMA_VM

#include <mach/boolean.h>
#include <mach/port.h>
#include <mach/message.h>
#include <mach/std_types.h>
#include <mach/mach_types.h>

#include <ipc/ipc_port.h>
#include <ipc/ipc_space.h>

#include <kern/queue.h>
#include <kern/zalloc.h>
#include <kern/kalloc.h>

#include <vm/vm_page.h>
#include <vm/vm_kern.h>

#include <device/device_types.h>
#include <device/ds_routines.h>
#include <device/dev_hdr.h>
#include <device/io_req.h>
#include <device/param.h>		/* DEV_BSIZE */

#if	NORMA_VM
extern	vm_object_t		xmm_vm_object_lookup();
#define	vm_object_lookup	xmm_vm_object_lookup
#endif	NORMA_VM

extern int	block_io_mmap();	/* dummy routine to allow
					   mmap for block devices */

/*
 *	The device pager routines are called directly from the message
 *	system (via mach_msg), and thus run in the kernel-internal
 *	environment.  All ports are in internal form (ipc_port_t),
 *	and must be correctly reference-counted in order to be saved
 *	in other data structures.  Kernel routines may be called
 *	directly.  Kernel types are used for data objects (tasks,
 *	memory objects, ports).  The only IPC routines that may be
 *	called are ones that masquerade as the kernel task (via
 *	msg_send_from_kernel).
 *
 *	Port rights and references are maintained as follows:
 *		Memory object port:
 *			The device_pager task has all rights.
 *		Memory object control port:
 *			The device_pager task has only send rights.
 *		Memory object name port:
 *			The device_pager task has only send rights.
 *			The name port is not even recorded.
 *	Regardless how the object is created, the control and name
 *	ports are created by the kernel and passed through the memory
 *	management interface.
 *
 *	The device_pager assumes that access to its memory objects
 *	will not be propagated to more that one host, and therefore
 *	provides no consistency guarantees beyond those made by the
 *	kernel.
 *
 *	In the event that more than one host attempts to use a device
 *	memory object, the device_pager will only record the last set
 *	of port names.  [This can happen with only one host if a new
 *	mapping is being established while termination of all previous
 *	mappings is taking place.]  Currently, the device_pager assumes
 *	that its clients adhere to the initialization and termination
 *	protocols in the memory management interface; otherwise, port
 *	rights or out-of-line memory from erroneous messages may be
 *	allowed to accumulate.
 *
 *	[The phrase "currently" has been used above to denote aspects of
 *	the implementation that could be altered without changing the rest
 *	of the basic documentation.]
 */

/*
 * Basic device pager structure.
 */
struct dev_pager {
	decl_simple_lock_data(, lock)	/* lock for reference count */
	int		ref_count;	/* reference count */
	int		client_count;	/* How many memory_object_create
					 * calls have we received */
	ipc_port_t	pager;		/* pager port */
	ipc_port_t	pager_request;	/* Known request port */
	ipc_port_t	pager_name;	/* Known name port */
	device_t	device;		/* Device handle */
	int		type;		/* to distinguish */
#define DEV_PAGER_TYPE	0
#define CHAR_PAGER_TYPE	1
	/* char pager specifics */
	int		prot;
	vm_offset_t	offset;
	vm_size_t	size;
#if	NORMA_VM
	struct vm_page *pages;		/* Page structures allocated */
	vm_size_t	pages_size;	/* ... amount thereof */
#endif	NORMA_VM
};
typedef struct dev_pager *dev_pager_t;
#define	DEV_PAGER_NULL	((dev_pager_t)0)

zone_t		dev_pager_zone;

void dev_pager_reference(ds)
	register dev_pager_t	ds;
{
	simple_lock(&ds->lock);
	ds->ref_count++;
	simple_unlock(&ds->lock);
}

void dev_pager_deallocate(ds)
	register dev_pager_t	ds;
{
	simple_lock(&ds->lock);
	if (--ds->ref_count > 0) {
	    simple_unlock(&ds->lock);
	    return;
	}

	simple_unlock(&ds->lock);
	zfree(dev_pager_zone, (vm_offset_t)ds);
}

/*
 * A hash table of ports for device_pager backed objects.
 */

#define	DEV_PAGER_HASH_COUNT		127

struct dev_pager_entry {
	queue_chain_t	links;
	ipc_port_t	name;
	dev_pager_t	pager_rec;
};
typedef struct dev_pager_entry *dev_pager_entry_t;

queue_head_t	dev_pager_hashtable[DEV_PAGER_HASH_COUNT];
zone_t		dev_pager_hash_zone;
decl_simple_lock_data(,
		dev_pager_hash_lock)

#define	dev_pager_hash(name_port) \
		(((int)(name_port) & 0xffffff) % DEV_PAGER_HASH_COUNT)

void dev_pager_hash_init()
{
	register int	i;
	register vm_size_t	size;

	size = sizeof(struct dev_pager_entry);
	dev_pager_hash_zone = zinit(
				size,
				size * 1000,
				PAGE_SIZE,
				FALSE,
				"dev_pager port hash");
	for (i = 0; i < DEV_PAGER_HASH_COUNT; i++)
	    queue_init(&dev_pager_hashtable[i]);
	simple_lock_init(&dev_pager_hash_lock);
}

void dev_pager_hash_insert(name_port, rec)
	ipc_port_t	name_port;
	dev_pager_t	rec;
{
	register dev_pager_entry_t new_entry;

	new_entry = (dev_pager_entry_t) zalloc(dev_pager_hash_zone);
	new_entry->name = name_port;
	new_entry->pager_rec = rec;

	simple_lock(&dev_pager_hash_lock);
	queue_enter(&dev_pager_hashtable[dev_pager_hash(name_port)],
			new_entry, dev_pager_entry_t, links);
	simple_unlock(&dev_pager_hash_lock);
}

void dev_pager_hash_delete(name_port)
	ipc_port_t	name_port;
{
	register queue_t	bucket;
	register dev_pager_entry_t	entry;

	bucket = &dev_pager_hashtable[dev_pager_hash(name_port)];

	simple_lock(&dev_pager_hash_lock);
	for (entry = (dev_pager_entry_t)queue_first(bucket);
	     !queue_end(bucket, &entry->links);
	     entry = (dev_pager_entry_t)queue_next(&entry->links)) {
	    if (entry->name == name_port) {
		queue_remove(bucket, entry, dev_pager_entry_t, links);
		break;
	    }
	}
	simple_unlock(&dev_pager_hash_lock);
	if (entry)
	    zfree(dev_pager_hash_zone, (vm_offset_t)entry);
}

dev_pager_t dev_pager_hash_lookup(name_port)
	ipc_port_t	name_port;
{
	register queue_t	bucket;
	register dev_pager_entry_t	entry;
	register dev_pager_t	pager;

	bucket = &dev_pager_hashtable[dev_pager_hash(name_port)];

	simple_lock(&dev_pager_hash_lock);
	for (entry = (dev_pager_entry_t)queue_first(bucket);
	     !queue_end(bucket, &entry->links);
	     entry = (dev_pager_entry_t)queue_next(&entry->links)) {
	    if (entry->name == name_port) {
		pager = entry->pager_rec;
		dev_pager_reference(pager);
		simple_unlock(&dev_pager_hash_lock);
		return (pager);
	    }
	}
	simple_unlock(&dev_pager_hash_lock);
	return (DEV_PAGER_NULL);
}

kern_return_t	device_pager_setup(device, prot, offset, size, pager)
	device_t	device;
	int		prot;
	vm_offset_t	offset;
	vm_size_t	size;
	mach_port_t	*pager;
{
	kern_return_t	result;
	register dev_pager_t	d;

	/*
	 *	Allocate a structure to hold the arguments
	 *	and port to represent this object.
	 */

	d = dev_pager_hash_lookup((ipc_port_t)device);	/* HACK */
	if (d != DEV_PAGER_NULL) {
		*pager = (mach_port_t) ipc_port_make_send(d->pager);
		dev_pager_deallocate(d);
		return (D_SUCCESS);
	}

	d = (dev_pager_t) zalloc(dev_pager_zone);
	if (d == DEV_PAGER_NULL)
		return (KERN_RESOURCE_SHORTAGE);

	simple_lock_init(&d->lock);
	d->ref_count = 1;

	/*
	 * Allocate the pager port.
	 */
	d->pager = ipc_port_alloc_kernel();
	if (d->pager == IP_NULL) {
		dev_pager_deallocate(d);
		return (KERN_RESOURCE_SHORTAGE);
	}

	d->client_count = 0;
	d->pager_request = IP_NULL;
	d->pager_name = IP_NULL;
	d->device = device;
	device_reference(device);
	d->prot = prot;
	d->offset = offset;
	d->size = round_page(size);
	if (device->dev_ops->d_mmap == block_io_mmap) {
		d->type = DEV_PAGER_TYPE;
	} else {
		d->type = CHAR_PAGER_TYPE;
	}

	dev_pager_hash_insert(d->pager, d);
	dev_pager_hash_insert((ipc_port_t)device, d);	/* HACK */

	*pager = (mach_port_t) ipc_port_make_send(d->pager);
#if	NORMA_VM
	xmm_add_exception(d->pager);
#endif	NORMA_VM
	return (KERN_SUCCESS);
}

/*
 *	Routine:	device_pager_release
 *	Purpose:
 *		Relinquish any references or rights that were
 *		associated with the result of a call to
 *		device_pager_setup.
 */
void	device_pager_release(object)
	memory_object_t	object;
{
	if (MACH_PORT_VALID(object))
		ipc_port_release_send((ipc_port_t) object);
}

/*
 * Rename all of the functions in the pager interface, to avoid
 * confusing them with the kernel interface.
 */

#define	memory_object_init		device_pager_init_pager
#define	memory_object_terminate		device_pager_terminate
#define	memory_object_data_request	device_pager_data_request
#define	memory_object_data_unlock	device_pager_data_unlock
#define	memory_object_data_write	device_pager_data_write
#define	memory_object_lock_completed	device_pager_lock_completed

boolean_t	device_pager_debug = FALSE;

boolean_t	device_pager_data_request_done();	/* forward */
boolean_t	device_pager_data_write_done();		/* forward */


kern_return_t	memory_object_data_request(pager, pager_request, offset,
				   length, protection_required)
	memory_object_t	pager;
	ipc_port_t	pager_request;
	vm_offset_t	offset;
	vm_size_t	length;
	vm_prot_t	protection_required;
{
	register dev_pager_t	ds;

#ifdef lint
	protection_required++;
#endif lint

	if (device_pager_debug)
		printf("(device_pager)data_request: pager=%d, offset=0x%x, length=0x%x\n",
			pager, offset, length);

	ds = dev_pager_hash_lookup(pager);
	if (ds == DEV_PAGER_NULL)
		panic("(device_pager)data_request: lookup failed");

	if (ds->pager_request != pager_request)
		panic("(device_pager)data_request: bad pager_request");

	if (ds->type == CHAR_PAGER_TYPE) {
	    register vm_object_t	object;
	    vm_offset_t			device_map_page();

	    object = vm_object_lookup(pager_request);
	    assert(object != VM_OBJECT_NULL);

	    vm_object_page_map(object,
			       ds->offset + offset, length,
			       device_map_page, (char *)ds);

	    vm_object_deallocate(object);
	}
	else {
	    register io_req_t	ior;
	    register device_t	device;
	    io_return_t		result;

	    panic("(device_pager)data_request: dev pager");
	    
	    device = ds->device;
	    device_reference(device);
	    dev_pager_deallocate(ds);
	    
	    /*
	     * Package the read for the device driver.
	     */
	    io_req_alloc(ior, 0);
	    
	    ior->io_device		= device;
	    ior->io_unit		= device->dev_number;
	    ior->io_op		= IO_READ | IO_CALL;
	    ior->io_mode		= 0;
	    ior->io_recnum		= offset / DEV_BSIZE;	/* XXX */
	    ior->io_data		= 0;		/* driver must allocate */
	    ior->io_count		= length;
	    ior->io_alloc_size	= 0;		/* no data allocated yet */
	    ior->io_residual	= 0;
	    ior->io_error		= 0;
	    ior->io_done		= device_pager_data_request_done;
	    ior->io_reply_port	= pager_request;
	    ior->io_reply_port_type	= MACH_MSG_TYPE_PORT_SEND;
	    
	    result = (*device->dev_ops->d_read)(device->dev_number, ior);
	    if (result == D_IO_QUEUED)
		return (KERN_SUCCESS);
	    
	    /*
	     * Return by queuing IOR for io_done thread, to reply in
	     * correct environment (kernel).
	     */
	    ior->io_error = result;
	    iodone(ior);
	}

	dev_pager_deallocate(ds);

	return (KERN_SUCCESS);
}

/*
 * Always called by io_done thread.
 */
boolean_t device_pager_data_request_done(ior)
	register io_req_t	ior;
{
	vm_offset_t	start_alloc, end_alloc;
	vm_size_t	size_read;

	if (ior->io_error == D_SUCCESS) {
	    size_read = ior->io_count;
	    if (ior->io_residual) {
		if (device_pager_debug)
		    printf("(device_pager)data_request_done: r: 0x%x\n",ior->io_residual);
		bzero( (char *) (&ior->io_data[ior->io_count - 
					       ior->io_residual]),
		      (unsigned) ior->io_residual);
	    }
	} else {
	    size_read = ior->io_count - ior->io_residual;
	}

	start_alloc = trunc_page((vm_offset_t)ior->io_data);
	end_alloc   = start_alloc + round_page(ior->io_alloc_size);

	if (ior->io_error == D_SUCCESS) {
	    (void) memory_object_data_provided(
					vm_object_lookup(ior->io_reply_port),
					ior->io_recnum * DEV_BSIZE,
					(vm_offset_t)ior->io_data,
					size_read,
					VM_PROT_NONE);
	}
	else {
	    (void) memory_object_data_error(
					vm_object_lookup(ior->io_reply_port),
					ior->io_recnum * DEV_BSIZE,
					(vm_size_t)ior->io_count,
					ior->io_error);
	}

	(void)vm_deallocate(kernel_map,
			    start_alloc,
			    end_alloc - start_alloc);
	device_deallocate(ior->io_device);
	return (TRUE);
}

kern_return_t memory_object_data_write(pager, pager_request,
			offset, addr, data_count)
	memory_object_t		pager;
	ipc_port_t		pager_request;
	register vm_offset_t	offset;
	register pointer_t	addr;
	vm_size_t		data_count;
{
	register dev_pager_t	ds;
	register device_t	device;
	register io_req_t	ior;
	kern_return_t		result;

	panic("(device_pager)data_write: called");

	ds = dev_pager_hash_lookup(pager);
	if (ds == DEV_PAGER_NULL)
		panic("(device_pager)data_write: lookup failed");

	if (ds->pager_request != pager_request)
		panic("(device_pager)data_write: bad pager_request");

	if (ds->type == CHAR_PAGER_TYPE)
		panic("(device_pager)data_write: char pager");

	device = ds->device;
	device_reference(device);
	dev_pager_deallocate(ds);

	/*
	 * Package the write request for the device driver.
	 */
	io_req_alloc(ior, data_count);

	ior->io_device		= device;
	ior->io_unit		= device->dev_number;
	ior->io_op		= IO_WRITE | IO_CALL;
	ior->io_mode		= 0;
	ior->io_recnum		= offset / DEV_BSIZE;	/* XXX */
	ior->io_data		= (io_buf_ptr_t)addr;
	ior->io_count		= data_count;
	ior->io_alloc_size	= data_count;	/* amount to deallocate */
	ior->io_residual	= 0;
	ior->io_error		= 0;
	ior->io_done		= device_pager_data_write_done;
	ior->io_reply_port	= IP_NULL;

	result = (*device->dev_ops->d_write)(device->dev_number, ior);

	if (result != D_IO_QUEUED) {
	    device_write_dealloc(ior);
	    io_req_free(ior);
	    device_deallocate(device);
	}

	return (KERN_SUCCESS);
}

boolean_t device_pager_data_write_done(ior)
	register io_req_t	ior;
{
	device_write_dealloc(ior);
	device_deallocate(ior->io_device);

	return (TRUE);
}

/*
 *	The mapping function takes a byte offset, but returns
 *	a machine-dependent page frame number.  We convert
 *	that into something that the pmap module will
 *	accept later.
 */
vm_offset_t device_map_page(dsp, offset)
	vm_offset_t	offset;
	char		*dsp;
{
	register dev_pager_t	ds = (dev_pager_t) dsp;

	return pmap_phys_address(
		   (*(ds->device->dev_ops->d_mmap))
			(ds->device->dev_number, offset, ds->prot));
}

kern_return_t memory_object_init(pager, pager_request, pager_name,
				 pager_page_size)
	ipc_port_t	pager;
	ipc_port_t	pager_request;
	ipc_port_t	pager_name;
	vm_size_t	pager_page_size;
{
	register dev_pager_t	ds;

	if (device_pager_debug)
		printf("(device_pager)init: pager=%d, request=%d, name=%d\n",
		       pager, pager_request, pager_name);

	assert(pager_page_size == PAGE_SIZE);
	assert(IP_VALID(pager_request));
	assert(IP_VALID(pager_name));

	ds = dev_pager_hash_lookup(pager);
	assert(ds != DEV_PAGER_NULL);

	assert(ds->client_count == 0);
	assert(ds->pager_request == IP_NULL);
	assert(ds->pager_name == IP_NULL);

	ds->client_count = 1;

	/*
	 * We save the send rights for the request and name ports.
	 */

	ds->pager_request = pager_request;
	ds->pager_name = pager_name;

	if (ds->type == CHAR_PAGER_TYPE) {
#if	NORMA_VM
	    vm_object_t	object;
	    vm_offset_t	offset;
	    int		num_pages;
	    vm_page_t	pg_cur;
	    int		i;

	    object = vm_object_lookup(pager_request);
	    assert(object != VM_OBJECT_NULL);

	    /*
	     *	Grab enough memory for vm_page structures to
	     *	represent all pages in the object.
	     */
	    num_pages = atop(ds->size);
	    ds->pages_size = num_pages * sizeof(struct vm_page);
	    ds->pages = (vm_page_t) kalloc(ds->pages_size);

	    /*
	     *	Fill in each page with the proper physical address,
	     *	and attach it to the object.  Mark it as wired-down
	     *	to keep the pageout daemon's hands off it.
	     */

	    for (i = 0, pg_cur = ds->pages, offset = ds->offset; i < num_pages;
			i++, pg_cur++, offset += PAGE_SIZE) {
		vm_offset_t	addr;
		vm_page_t	old_page;

		/*
		 *	The mapping function takes a byte offset, but returns
		 *	a machine-dependent page frame number.  We convert
		 *	that into something that the pmap module will
		 *	accept later.
		 */

		addr = pmap_phys_address(
		   (*(ds->device->dev_ops->d_mmap))
			(ds->device->dev_number, offset, ds->prot));

		/*
		 *	Insert a page with the appropriate physical
		 *	address.  [If a thread using this data
		 *	gets to it before we initialize, it will have
		 *	entered a page and waited on it.  We throw it away,
		 *	causing that thread to be awakened.  We'll later
		 *	ignore the pagein request.]
		 */

		vm_object_lock(object);
		if ((old_page = vm_page_lookup(object, offset)) != 
		    VM_PAGE_NULL) {
			vm_page_lock_queues();
			vm_page_free(old_page);
			vm_page_unlock_queues();
		}

		vm_page_init(pg_cur, addr);
  		pg_cur->wire_count = 1;
		pg_cur->private = TRUE;	/* Page structure is ours */
		vm_page_lock_queues();
		vm_page_insert(pg_cur, object, offset);
		vm_page_unlock_queues();
		PAGE_WAKEUP_DONE(pg_cur);
		vm_object_unlock(object);
	    }

	    vm_object_deallocate(object);
#else	NORMA_VM
#endif	NORMA_VM

	    /*
	     * Reply that the object is ready
	     */
	    (void) memory_object_set_attributes(vm_object_lookup(pager_request),
						TRUE,	/* ready */
						FALSE,	/* do not cache */
						MEMORY_OBJECT_COPY_NONE);
	} else {
	    (void) memory_object_set_attributes(vm_object_lookup(pager_request),
						TRUE,	/* ready */
						TRUE,	/* cache */
						MEMORY_OBJECT_COPY_DELAY);

	}

	dev_pager_deallocate(ds);
	return (KERN_SUCCESS);
}

kern_return_t memory_object_terminate(pager, pager_request, pager_name)
	ipc_port_t	pager;
	ipc_port_t	pager_request;
	ipc_port_t	pager_name;
{
	register dev_pager_t	ds;

	assert(IP_VALID(pager_request));
	assert(IP_VALID(pager_name));

	ds = dev_pager_hash_lookup(pager);
	assert(ds != DEV_PAGER_NULL);

	assert(ds->client_count == 1);
	assert(ds->pager_request == pager_request);
	assert(ds->pager_name == pager_name);

	dev_pager_hash_delete(ds->pager);
	dev_pager_hash_delete((ipc_port_t)ds->device);	/* HACK */
	device_deallocate(ds->device);

#if	NORMA_VM
	if (ds->type == CHAR_PAGER_TYPE)
	    kfree((char *) ds->pages, ds->pages_size);
#endif	NORMA_VM

	/* release the send rights we have saved from the init call */

	ipc_port_release_send(pager_request);
	ipc_port_release_send(pager_name);

	/* release the naked receive rights we just acquired */

	ipc_port_release_receive(pager_request);
	ipc_port_release_receive(pager_name);

	/* release the kernel's receive right for the pager port */

	ipc_port_dealloc_kernel(pager);

	/* once for ref from lookup, once to make it go away */
	dev_pager_deallocate(ds);
	dev_pager_deallocate(ds);

	return (KERN_SUCCESS);
}

kern_return_t memory_object_data_unlock(memory_object,memory_control_port,offset,length,desired_access)
	memory_object_t memory_object;
	ipc_port_t memory_control_port;
	vm_offset_t offset;
	vm_size_t length;
	vm_prot_t desired_access;
{
#ifdef	lint
	memory_object++; memory_control_port++; offset++; length++; desired_access++;
#endif	lint

	panic("(device_pager)data_unlock: called");
	return (KERN_FAILURE);
}

kern_return_t memory_object_lock_completed(memory_object, pager_request_port,
					offset, length)
	memory_object_t	memory_object;
	ipc_port_t	pager_request_port;
	vm_offset_t	offset;
	vm_size_t	length;
{
#ifdef	lint
	memory_object++; pager_request_port++; offset++; length++;
#endif	lint

	panic("(device_pager)lock_completed: called");
	return (KERN_FAILURE);
}

/*
 * Include memory_object_server in this file to avoid name
 * conflicts with other possible pagers.
 */
#define	memory_object_server		device_pager_server
#include <device/device_pager_server.c>

void device_pager_init()
{
	register vm_size_t	size;

	/*
	 * Initialize zone of paging structures.
	 */
	size = sizeof(struct dev_pager);
	dev_pager_zone = zinit(size,
				(vm_size_t) size * 1000,
				PAGE_SIZE,
				FALSE,
				"device pager structures");

	/*
	 *	Initialize the name port hashing stuff.
	 */
	dev_pager_hash_init();
}
