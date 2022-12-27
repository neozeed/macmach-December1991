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
 * $Log:	mapped_scsi.c,v $
 * Revision 2.11  91/08/24  12:27:36  af
 * 	Spl defs.
 * 	[91/08/02  03:45:16  af]
 * 
 * Revision 2.10  91/06/25  20:56:05  rpd
 * 	Tweaks to make gcc happy.
 * 
 * Revision 2.9  91/06/19  11:56:42  rvb
 * 	minor improvements
 * 	[91/06/18  21:34:58  rvb]
 * 
 * 	mips->DECSTATION; vax->VAXSTATION
 * 	[91/06/12  14:02:17  rvb]
 * 
 * 	File moved here from mips/PMAX since it is now "MI" code, also
 * 	used by Vax3100 and soon -- the omron luna88k.
 * 	[91/06/04            rvb]
 * 
 * Revision 2.8  91/05/14  17:24:01  mrt
 * 	Correcting copyright
 * 
 * Revision 2.7  91/05/13  06:35:07  af
 * 	Check for spurious interrupts before disabling them.
 * 
 * Revision 2.6.1.1  91/05/12  16:20:17  af
 * 	Check for spurious interrupts before disabling them.
 * 
 * Revision 2.6  91/03/16  14:54:20  rpd
 * 	Updated for new kmem_alloc interface.
 * 	[91/03/03            rpd]
 * 	Removed thread_swappable.
 * 	[91/01/18            rpd]
 * 
 * Revision 2.5  91/02/14  14:34:32  mrt
 * 	In interrupt routines, drop priority as now required.
 * 	Also, sanity check against spurious interrupts.
 * 	[91/02/12  13:17:21  af]
 * 
 * Revision 2.4  91/02/05  17:42:25  mrt
 * 	Added author notices
 * 	[91/02/04  11:14:48  mrt]
 * 
 * 	Changed to use new Mach copyright
 * 	[91/02/02  12:13:25  mrt]
 * 
 * Revision 2.3  90/12/05  23:32:20  af
 * 
 * 
 * Revision 2.1.1.2  90/11/13  15:10:18  af
 * 	Added pmax support.
 * 
 * Revision 2.1.1.1  90/11/01  02:44:02  af
 * 	Created.
 * 	[90/09/12            af]
 * 
 */
/*
 *	File: mapped_scsi.c
 * 	Author: Alessandro Forin, Carnegie Mellon University
 *	Date:	9/90
 *
 *	In-kernel side of the user-mapped SCSI driver.
 */

#include <asc.h>
#include <sii.h>
#define NRZ	(NASC+NSII)
#if	NRZ > 0
#include <platforms.h>

#include <sys/types.h>
#include <chips/busses.h>
#include <machine/machparam.h>		/* spl definitions */

#include <device/device_types.h>
#include <device/io_req.h>

#include <vm/vm_kern.h>

#include <scsi/mapped_scsi.h>

#include <machine/machparam.h>

#ifdef	DECSTATION

#define	machine_btop	mips_btop

#define	kvctophys(v)	K0SEG_TO_PHYS((v))	/* kernel virtual cached */
#define	phystokvc(p)	PHYS_TO_K0SEG((p))	/* and back */
#define	kvutophys(v)	K1SEG_TO_PHYS((v))	/* kernel virtual uncached */
#define	phystokvu(p)	PHYS_TO_K1SEG((p))	/* and back */

#include <mips/mips_cpu.h>
#include <mips/PMAX/kn01.h>
#include <mips/PMAX/pmaz_aa.h>

#define SII_REG_PHYS(self)	kvutophys(self->registers.any)
#define	SII_RAM_PHYS(self)	(SII_REG_PHYS((self))+(KN01_SYS_SII_B_START-KN01_SYS_SII))
#define	SII_RAM_SIZE		(KN01_SYS_SII_B_END-KN01_SYS_SII_B_START)

#define ASC_REG_PHYS(self)	kvutophys(self->registers.any)
#define ASC_DMAR_PHYS(self)	(ASC_REG_PHYS((self))+ ASC_OFFSET_DMAR)
#define ASC_RAM_PHYS(self)	(ASC_REG_PHYS((self))+ ASC_OFFSET_RAM)

#endif	/*DECSTATION*/

#ifdef	VAXSTATION
#define	machine_btop	vax_btop
#endif	/*VAXSTATION*/


/*
 * Phys defines for the various supported HBAs
 */

/* DEC7061	*/
#include <scsi/adapters/scsi_7061.h>

/* NCR 53C94	*/
#include <scsi/adapters/scsi_53C94.h>


/*
 * Co-existency with in-kernel drivers
 */
boolean_t	rz_use_mapped_interface = FALSE;

/*
 * Status information for all HBAs
 */
static struct RZ_status {
	union {
		unsigned long		any;
		asc_regmap_t		*asc;
		sii_regmap_t		*sii;
	} registers;
	int				(*stop)();
	vm_offset_t			(*mmap)();
	thread_t			 user_thread;
	mapped_scsi_info_t		 info;
} RZ_statii[NRZ];

typedef struct RZ_status	*RZ_status_t;


/*
 * Probe routine for all HBAs
 */
RZ_probe(regbase, ui, hba)
	unsigned long			regbase;
	register struct bus_device 	*ui;
{
	int unit = ui->unit;
	vm_offset_t		addr;
	mapped_scsi_info_t	info;

	printf("[mappable] ");

	RZ_statii[unit].registers.any = regbase;

	/*
	 * Grab a page to be mapped later to users 
	 */
	(void) kmem_alloc_wired(kernel_map, &addr, PAGE_SIZE);	/* kseg2 */
	bzero(addr, PAGE_SIZE);
	addr = pmap_extract(pmap_kernel(), addr);	/* phys */
	info = (mapped_scsi_info_t) (phystokvc(addr));
	RZ_statii[unit].info = info;

	/*
	 * Set permanent info
	 */
	info->interrupt_count	=	0;
	info->wait_event	=	(unsigned)info;
/*XXX*/	info->ram_size		=	ASC_RAM_SIZE;
	info->hba_type		=	hba;

	return 1;
}

/*
 * Device open procedure
 */
RZ_open(dev, flag, ior)
	io_req_t ior;
{
	int             	unit = minor(dev);
	thread_t		thread = current_thread();
	register RZ_status_t	self = &RZ_statii[unit];


	if (unit >= NRZ)
		return D_NO_SUCH_DEVICE;

	if (self->user_thread) {
		printf("rz%d: %s open, threads %x %x\n", unit,
			(self->user_thread == thread) ?
				"repeated" : "multiple",
			self->user_thread, thread);
	}

	/*
	 * Silence interface, just in case 
	 */
	(*self->stop)(unit);

	rz_use_mapped_interface = TRUE;

	/*
	 * Switch to that thread on interrupt 
	 */
	self->user_thread = thread;

	/*
	 * Do not turn interrupts on.  The user can do it when ready
	 * to take them. 
	 */

	return 0;
}

/*
 * Device close procedure
 */
RZ_close(dev, flag)
{
	int             	unit = minor(dev);
	thread_t		thread;
	register RZ_status_t	self = &RZ_statii[unit];

	if (unit >= NRZ)
		return D_NO_SUCH_DEVICE;

	/*
	 * Silence interface, in case user forgot
	 */
	(*self->stop)(unit);

	rz_use_mapped_interface = FALSE;

	/* XXX	rz_kernel_mode(); XXX */

	self->user_thread = 0;

	return 0;
}


/*
 * Get status procedure.
 * We need to tell that we are mappable.
 */
io_return_t
RZ_get_status(dev, flavor, status, status_count)
	dev_t		dev;
	int		flavor;
	dev_status_t	status;
	unsigned int	status_count;
{
	printf("RZ_getstat? %x %x %x %x\n", dev, flavor, status, status_count);
	return (D_SUCCESS);
}

/*
 * Should not refuse this either
 */
RZ_set_status(dev, flavor, status, status_count)
	dev_t		dev;
	int		flavor;
	dev_status_t	status;
	unsigned int	status_count;
{
	printf("RZ_setstat? %x %x %x %x\n", dev, flavor, status, status_count);
	return (D_SUCCESS);
}

/*
 * Port death notification routine
 */
RZ_portdeath(dev, dead_port)
{
	int unit = minor(dev);

	printf("RZ_portdeath(%x %x): %d %x\n", dev, dead_port, unit,
		RZ_statii[unit].user_thread);
}

/*
 * Page mapping, switch off to HBA-specific for regs&ram
 */
vm_offset_t
RZ_mmap(dev, off, prot)
	dev_t		dev;
{
	int             	unit = minor(dev);
	register RZ_status_t	self = &RZ_statii[unit];
	vm_offset_t     	page;
	vm_offset_t     	addr;
	io_return_t		ret;

	if (off < SCSI_INFO_SIZE) {
		addr = kvctophys (self->info) + off;
		ret = D_SUCCESS;
	} else
		ret = (*self->mmap)(self, off, prot, &addr);

	if (ret != D_SUCCESS)
		return ret;

	page = machine_btop(addr);

	return (page);	
}


/*
 *---------------------------------------------------------------
 * 	The rest of the file contains HBA-specific routines
 *---------------------------------------------------------------
 */

#if	NASC > 0
/*
 * Routines for the NCR 53C94
 */
static
ASC_stop(unit)
{
	register RZ_status_t	   self = &RZ_statii[unit];
	register asc_regmap_t	  *regs = self->registers.asc;
	int			   ack;

	ack = regs->asc_intr;	/* Just acknowledge pending interrupts */
}

ASC_probe(reg, ui)
	unsigned long			reg;
	register struct bus_device 	*ui;
{
	register RZ_status_t	self = &RZ_statii[ui->unit];
	static vm_offset_t	ASC_mmap();

	self->stop = ASC_stop;
	self->mmap = ASC_mmap;
	return RZ_probe(reg, ui, HBA_NCR_53c94);
}


ASC_intr(unit,spllevel)
{
	register RZ_status_t 	   self = &RZ_statii[unit];
	register asc_regmap_t	  *regs = self->registers.asc;
	register thread_t	   thread = self->user_thread;
	register        	   csr, intr, seq_step;

	/*
	 * Acknowledge interrupt request
	 *
	 * This clobbers some two other registers, therefore
	 * we read them beforehand.  It also clears the intr
	 * request bit, silencing the interface for now.
	 */
	csr = regs->asc_csr;

	/* drop spurious interrupts */
	if ((csr & ASC_CSR_INT) == 0)
		return;
	seq_step = regs->asc_ss;

	intr = regs->asc_intr;	/* ack */

	splx(spllevel);	/* drop priority */

	if (self->info) {
		self->info->interrupt_count++;	/* total interrupts */
		self->info->saved_regs.asc.csr = csr;
		self->info->saved_regs.asc.isr = intr;
		self->info->saved_regs.asc.seq = seq_step;
		self->info->saved_regs.asc.flg = regs->asc_flags;
	}

	/* Awake user thread */
#if	notyet
	thread_wakeup(self->wait_event);
#else
	if (thread){
		/* unblock if stopped */
		(void) thread_resume_from_kernel(thread);
	}
#endif
}

/*
 * Virtual->physical mapping routine for PMAZ-AA
 */
static vm_offset_t
ASC_mmap(self, off, prot, addr)
	RZ_status_t	self;
	vm_offset_t	off;
	vm_prot_t	prot;
	vm_offset_t	*addr;
{
	/*
	 * The offset (into the VM object) defines the following layout
	 *
	 *	off	size	what
	 *	0	1pg	mapping information (csr & #interrupts)
	 *	1pg	1pg	ASC registers
	 *	2pg	1pg	ASC dma
	 *	3pg	128k	ASC ram buffers
	 */

#define	ASC_END	(ASC_RAM_BASE+ASC_RAM_SIZE)

	if (off < ASC_REGS_BASE)
		*addr = (vm_offset_t) ASC_REG_PHYS(self) + (off - SCSI_INFO_SIZE);
	else if (off < ASC_RAM_BASE)
		*addr = (vm_offset_t) ASC_DMAR_PHYS(self) + (off - ASC_REGS_BASE);
	else if (off < ASC_END)
		*addr = (vm_offset_t) ASC_RAM_PHYS(self) + (off - ASC_RAM_BASE);
	else
		return D_INVALID_SIZE;

	return D_SUCCESS;
}
#endif	NASC > 0

#if	NSII > 0
SII_stop(unit)
{
	register RZ_status_t	   self = &RZ_statii[unit];
	register sii_regmap_t	  *regs = self->registers.sii;

	regs->sii_csr &= ~SII_CSR_IE;	/* disable interrupts */
					/* clear all wtc bits */
	regs->sii_conn_csr = regs->sii_conn_csr;
	regs->sii_data_csr = regs->sii_data_csr;
}

SII_probe(reg, ui)
	unsigned long			reg;
	register struct bus_device 	*ui;
{
	register RZ_status_t	self = &RZ_statii[ui->unit];
	static vm_offset_t	SII_mmap();

	self->stop = SII_stop;
	self->mmap = SII_mmap;
	return RZ_probe(reg, ui, HBA_DEC_7061);
}

SII_intr(unit,spllevel)
{
	register RZ_status_t 	   self = &RZ_statii[unit];
	register sii_regmap_t	  *regs = self->registers.sii;
	register thread_t	   thread = self->user_thread;
	register unsigned short	   conn, data;

	/*
	 * Disable interrupts, saving cause(s) first.
	 */
	conn = regs->sii_conn_csr;
	data = regs->sii_data_csr;

	/* drop spurious calls */
	if (((conn|data) & (SII_DTR_DI|SII_DTR_CI)) == 0)
		return;

	regs->sii_csr &= ~SII_CSR_IE;

	regs->sii_conn_csr = conn;
	regs->sii_data_csr = data;

	splx(spllevel);

	if (self->info) {
		self->info->interrupt_count++;	/* total interrupts */
		self->info->saved_regs.sii.sii_conn_csr = conn;
		self->info->saved_regs.sii.sii_data_csr = data;
	}

	/* Awake user thread */
#if	notyet
	thread_wakeup(self->wait_event);
#else
	if (thread){
		/* unblock if stopped */
		(void) thread_resume_from_kernel(thread);
	}
#endif
}

static vm_offset_t
SII_mmap(self, off, prot, addr)
	RZ_status_t	self;
	vm_offset_t	off;
	vm_prot_t	prot;
	vm_offset_t	*addr;
{
	/*
	 * The offset (into the VM object) defines the following layout
	 *
	 *	off	size	what
	 *	0	1pg	mapping information (csr & #interrupts)
	 *	1pg	1pg	SII registers
	 *	2pg	128k	SII ram buffer
	 */

#define	SII_END	(SII_RAM_BASE+SII_RAM_SIZE)

	if (off < SII_REGS_BASE)
		*addr = (vm_offset_t) SII_REG_PHYS(self) + (off - SCSI_INFO_SIZE);
	else if (off < SII_END)
		*addr = (vm_offset_t) SII_RAM_PHYS(self) + (off - SII_RAM_BASE);
	else
		return D_INVALID_SIZE;

	return D_SUCCESS;
}
#endif	NSII > 0

#endif	NRZ > 0
