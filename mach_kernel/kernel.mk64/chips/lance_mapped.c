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
 * $Log:	lance_mapped.c,v $
 * Revision 2.12  91/08/24  11:52:17  af
 * 	Bugfix: typos, spl for 3min.
 * 	[91/08/02  01:52:26  af]
 * 
 * Revision 2.11  91/06/25  20:54:03  rpd
 * 	Tweaks to make gcc happy.
 * 
 * Revision 2.10  91/06/19  15:19:40  rvb
 * 	mips->DECSTATION; vax->VAXSTATION
 * 	[91/06/12  14:01:45  rvb]
 * 
 * 	File moved here from mips/PMAX since it tries to be generic;
 * 	it is used on the PMAX and could be used on the Vax3100.
 * 	[91/06/04            rvb]
 * 
 * Revision 2.9  91/05/14  17:22:24  mrt
 * 	Correcting copyright
 * 
 * Revision 2.8  91/03/16  14:54:00  rpd
 * 	Picked up a further spl fix from Sandro.
 * 	[91/03/09            rpd]
 * 	Updated for new kmem_alloc interface.
 * 	[91/03/03            rpd]
 * 	Removed thread_swappable.
 * 	[91/01/18            rpd]
 * 
 * Revision 2.7  91/02/14  14:34:16  mrt
 * 	In interrupt routine, drop priority as now required.
 * 	Also sanity check for spurious interrupts anyways.
 * 	[91/02/12  12:41:46  af]
 * 
 * Revision 2.6  91/02/05  17:41:22  mrt
 * 	Added author notices
 * 	[91/02/04  11:13:58  mrt]
 * 
 * 	Changed to use new Mach copyright
 * 	[91/02/02  12:12:01  mrt]
 * 
 * Revision 2.5  90/12/05  23:31:33  af
 * 	Cleanups.
 * 	[90/12/03  23:20:13  af]
 * 
 * Revision 2.3.1.1  90/10/03  12:06:02  af
 * 	Made it work with multiple ether boards (e.g. thick ether
 * 	on 3maxen).  This is the first time a mapped device does
 * 	something before the un-mapped one..
 * 
 * Revision 2.3  90/09/09  23:52:11  rpd
 * 	Changed thread_resume_from_kernel to thread_resume.
 * 
 * Revision 2.2  90/09/09  23:20:35  rpd
 * 	Correctly return the ram size for the givem machine: 3maxen
 * 	have twice as much ram as pmaxen.
 * 	[90/09/09  18:59:41  af]
 * 
 * 	Created.
 * 	[90/08/22            af]
 */
/*
 *	File: if_se_mapped.c
 * 	Author: Alessandro Forin, Carnegie Mellon University
 *	Date:	8/90
 *
 *	In-kernel side of the user-mapped ethernet driver.
 */

#include <ln.h>
#if     NLN > 0
#include <platforms.h>

#include <machine/machparam.h>		/* spl definitions */
#include <sys/types.h>
#include <chips/lance.h>
#include <chips/busses.h>

#include <device/device_types.h>
#include <device/errno.h>
#include <device/io_req.h>
#include <device/net_status.h>
#include <device/net_io.h>
#include <device/if_hdr.h>
#include <device/if_ether.h>

#include <vm/vm_kern.h>

#include <machine/machparam.h>

#ifdef	DECSTATION

#define	machine_btop	mips_btop

#define	kvctophys(v)	K0SEG_TO_PHYS((v))	/* kernel virtual cached */
#define	phystokvc(p)	PHYS_TO_K0SEG((p))	/* and back */
#define	kvutophys(v)	K1SEG_TO_PHYS((v))	/* kernel virtual uncached */
#define	phystokvu(p)	PHYS_TO_K1SEG((p))	/* and back */

#include <mips/mips_cpu.h>
/*
 * Wired addresses and sizes
 */
#define SE0_REG_EMRG	(se_reg_t)(0xb8000000)

#define	REGBASE(unit)	(((u_long)SE_statii[unit].registers) - se_sw->regspace)

#define SE_REG_PHYS(unit)	kvutophys(REGBASE(unit)+se_sw->regspace)
#define SE_REG_SIZE		PAGE_SIZE

#define SE_BUF_PHYS(unit)	kvutophys(REGBASE(unit)+se_sw->bufspace)
#define SE_BUF_SIZE		(128*1024)

#define SE_ADR_PHYS(unit)	kvutophys(REGBASE(unit)+se_sw->romspace)
#define SE_ADR_SIZE		PAGE_SIZE
#endif	/*DECSTATION*/

#ifdef	VAXSTATION
#define	machine_btop	vax_btop
#endif	/*VAXSTATION*/

/*
 * Autoconf info
 */

static	caddr_t SEstd[NLN] = { 0 };
static	struct bus_device *SEinfo[NLN];
	int SE_attach(), SE_probe();

struct bus_driver SEdriver =
       { SE_probe, 0, SE_attach, 0, SEstd, "SE", SEinfo, };

/*
 * Externally visible functions
 */
int		SE_probe(), SE_intr();		/* Kernel */

						/* User */
int		SE_open(), SE_close(), SE_portdeath();
vm_offset_t	SE_mmap();


/*
 * Status information for all interfaces
 */
static struct SE_status {
	se_reg_t			 registers;
	thread_t			 user_thread;
	mapped_ether_info_t		 info;
} SE_statii[NLN];


/*
 * Probe the Lance to see (if) that it's there
 */
SE_probe(regbase, ui)
	u_long regbase;
	register struct bus_device *ui;
{
	int unit = ui->unit;
	se_reg_t	regs;
	vm_offset_t     addr;
	mapped_ether_info_t		 info;

	if (unit >= NLN)
		return 0;

	printf("[mappable] ");

	regs = (se_reg_t) (regbase + se_sw->regspace);
	SE_statii[unit].registers = regs;

	/*
	 * Reset the interface 
	 */
	SE_stop(unit);

	/*
	 * Grab a page to be mapped later to users 
	 */
	(void) kmem_alloc_wired(kernel_map, &addr, PAGE_SIZE);	/* kseg2 */
	bzero(addr, PAGE_SIZE);
	addr = pmap_extract(pmap_kernel(), addr);	/* phys */
	info = (mapped_ether_info_t) (phystokvc(addr));
	SE_statii[unit].info = info;

	/*
	 * Set permanent info
	 */
	info->rom_stride = se_sw->romstride;
	info->ram_stride = se_sw->ramstride;
	info->buffer_size = se_sw->ramsize;
	info->buffer_physaddr = se_sw->ln_bufspace;
	info->wait_event = (unsigned) info;

	return 1;
}

SE_attach(ui)
	register struct bus_device *ui;
{
	return 1;
}
 

/*
 * Shut off the lance
 */
static SE_stop(unit)
{
	register se_reg_t	regs = SE_statii[unit].registers;

	if (regs == 0)
	    	/* Stray interrupt */
 		regs = SE0_REG_EMRG;

	regs[2] = CSR0_SELECT;	/* XXXX rap XXXX */
	wbflush();
	regs[0] = LN_CSR0_STOP;
	wbflush();
}


/*
 * Ethernet interface interrupt routine
 */
SE_intr(unit,spllevel)
{
	register struct SE_status *self = &SE_statii[unit];
	register se_reg_t	regs = self->registers;
	register thread_t thread = self->user_thread;
	register        csr;

	if (regs == 0) {	/* stray */
		SE_stop(unit);
		return;
	}

	/* Acknowledge interrupt request, drop spurious intr */
	csr = regs[0];
	if ((csr & LN_CSR0_INTR) == 0)
		return;
	regs[0] = csr & LN_CSR0_WTC;		/* silence it */

	splx(spllevel);	/* drop priority now */

	/* Pass csr state up to user thread */
	if (self->info) {
		self->info->interrupt_count++;	/* total interrupts */
		self->info->saved_csr0 = csr;
	}

	/* Awake user thread */
	if (thread){
		/* unblock if stopped */
		(void) thread_resume_from_kernel(thread);
	}
}
 

extern boolean_t se_use_mapped_interface[NLN];

/*
 * Device open procedure
 */
SE_open(dev, flag, ior)
	io_req_t ior;
{
	int             unit = minor(dev);
	register struct SE_status *self = &SE_statii[unit];
	thread_t	thread = current_thread();

	if (unit >= NLN)
		return EINVAL;

	if (self->user_thread) {
		printf("se%d: %s open, threads %x %x\n", unit,
			(self->user_thread == thread) ?
				"repeated" : "multiple",
			self->user_thread, thread);
	}

	/*
	 * Silence interface, just in case 
	 */
	SE_stop(unit);

	se_use_mapped_interface[unit] = 1;

	/*
	 * Switch to that thread on interrupt 
	 */
	self->user_thread = thread;

	/*
	 * Do not turn Ether interrupts on.  The user can do it when ready
	 * to take them. 
	 */

	return 0;
}

/*
 * Device close procedure
 */
SE_close(dev, flag)
{
	int             unit = minor(dev);
	register struct SE_status *self = &SE_statii[unit];

	if (unit >= NLN)
		return EINVAL;

	/*
	 * Silence interface, in case user forgot
	 */
	SE_stop(unit);

	se_normal(unit);

	self->user_thread = 0;

	return 0;
}


/*
 * Get status procedure.
 * We need to tell that we are mappable.
 */
io_return_t
SE_get_status(ifp, flavor, status, status_count)
/*	struct ifnet	*ifp; not really..*/
	int		flavor;
	dev_status_t	status;		/* pointer to OUT array */
	unsigned int	*status_count;		/* OUT */
{
	switch (flavor) {
	    case NET_STATUS:
	    {
		register struct net_status *ns = (struct net_status *)status;

		ns->min_packet_size = sizeof(struct ether_header);
		ns->max_packet_size = sizeof(struct ether_header) + ETHERMTU;
		ns->header_format   = HDR_ETHERNET;
		ns->header_size	    = sizeof(struct ether_header);
		ns->address_size    = 6;
		ns->flags	    = IFF_BROADCAST;
		ns->mapped_size	    = (128*1024) + (3 * PAGE_SIZE);

		*status_count = NET_STATUS_COUNT;
		break;
	    }
/*	    case NET_ADDRESS:	find it yourself */
	    default:
		return (D_INVALID_OPERATION);
	}
	return (D_SUCCESS);
}

/*
 * Should not refuse this either
 */
SE_set_status(dev, flavor, status, status_count)
	dev_t		dev;
	int		flavor;
	dev_status_t	status;
	unsigned int	status_count;
{
	return (D_SUCCESS);
}

/*
 * Port death notification routine
 */
SE_portdeath(dev, dead_port)
{
	int unit = minor(dev);

	printf("SE_portdeath(%x %x): %d %x\n", dev, dead_port, unit,
		SE_statii[unit].user_thread);
}


/*
 * Virtual->physical mapping routine.
 */
vm_offset_t
SE_mmap(dev, off, prot)
	dev_t		dev;
	vm_offset_t	off;
	vm_prot_t	prot;
{
	vm_offset_t     page;
	vm_offset_t     addr;
	int             unit = minor(dev);

	/*
	 * The offset (into the VM object) defines the following layout
	 *
	 *	off	size	what
	 *	0	1pg	mapping information (csr & #interrupts)
	 *	1pg	1pg	lance registers
	 *	2pg	1pg	lance station address (ROM)
	 *	3pg	128k	lance buffers 
	 */
#define S0	PAGE_SIZE
#define S1	(S0+SE_REG_SIZE)
#define S2	(S1+SE_ADR_SIZE)
#define S3	(S2+SE_BUF_SIZE)

	if (off < S0) {
		addr = kvctophys (SE_statii[unit].info);
	} else if (off < S1) {
		addr = (vm_offset_t) SE_REG_PHYS(unit);
		off -= S0;
	} else if (off < S2) {
		addr = (vm_offset_t) SE_ADR_PHYS(unit);
		off -= S1;
	} else if (off < S3) {
		addr = (vm_offset_t) SE_BUF_PHYS(unit);
		off -= S2;
	} else
		return (EINVAL);

	page = mips_btop(addr + off);

	return (page);
}

#endif	NLN > 0
