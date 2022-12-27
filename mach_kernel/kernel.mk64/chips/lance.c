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
 * $Log:	lance.c,v $
 * Revision 2.18  91/09/04  11:41:18  jsb
 * 	Don't let ethernet interrupts blow us away.
 * 	[91/09/04  11:35:00  jsb]
 * 
 * Revision 2.17  91/08/24  11:52:03  af
 * 	Spls for 3min, which has yet another bogus DMA setup: now the
 * 	alignment for the rings can be different from the one for data.
 * 	What will be next ?
 * 	[91/08/02  02:24:15  af]
 * 
 * Revision 2.16  91/06/25  20:53:52  rpd
 * 	Tweaks to make gcc happy.
 * 	[91/06/25            rpd]
 * 
 * Revision 2.15  91/06/19  15:19:34  rvb
 * 	mips->DECSTATION; vax->VAXSTATION
 * 	[91/06/12  14:01:39  rvb]
 * 
 * 	File moved here from mips/PMAX since it tries to be generic;
 * 	it is used on the PMAX and the Vax3100.
 * 	[91/06/04            rvb]
 * 
 * Revision 2.14  91/05/14  17:21:57  mrt
 * 	Correcting copyright
 * 
 * Revision 2.13  91/03/16  14:53:46  rpd
 * 	Changed net_filter to net_packet.
 * 	[91/01/15            rpd]
 * 
 * Revision 2.12  91/02/14  14:34:05  mrt
 * 	In interrupt routine, drop priority as now required.
 * 	Added (optional) loopback of packets sent to ourselves,
 * 	this way can run multiple POEs and have them talk to each other.
 * 	Same flag lets you run with external hardware loopback terminator
 * 	ignoring no-carrier errors.
 * 	Fixed bug with multiple ether boards.
 * 	Fixed bug with loopback path shifting data incorrectly.
 * 	[91/02/12  12:53:29  af]
 * 
 * Revision 2.11  91/02/05  17:41:05  mrt
 * 	Added author notices
 * 	[91/02/04  11:13:39  mrt]
 * 
 * 	Changed to use new Mach copyright
 * 	[91/02/02  12:11:37  mrt]
 * 
 * Revision 2.10  91/01/08  15:47:51  rpd
 * 	Changed NET_KMSG_GET, NET_KMSG_FREE to net_kmsg_get, net_kmsg_put.
 * 	[91/01/05            rpd]
 * 
 * Revision 2.9  90/12/05  23:31:17  af
 * 	Cleanups.
 * 	Consistency renames, get defs for 3max from new pmad_aa.h file.
 * 	seinfo[] was improperly sized.
 * 	[90/12/03  23:18:52  af]
 * 
 * Revision 2.7  90/09/09  23:20:23  rpd
 * 	Ram size for lance depends on machine type: added to switch.
 * 	[90/09/09  18:58:52  af]
 * 
 * 	Co-existency hacks for the mapped interface.
 * 	[90/08/30  17:29:04  af]
 * 
 * Revision 2.6  90/08/27  22:05:36  dbg
 * 	Some revisions of the Lance chip might lose the CSR1 info,
 * 	made sure loading of the init block resets them.
 * 	Localized local localities :-)
 * 	[90/08/17  12:28:49  af]
 * 
 * Revision 2.5  90/08/07  22:24:40  rpd
 * 	Made silent by default.
 * 	[90/08/07  15:32:00  af]
 * 
 * Revision 2.3.4.2  90/07/27  11:27:43  af
 * 	3max support.
 * 
 * 
 * Revision 2.3.4.1  90/06/02  12:24:39  af
 * 	Works now.  The problem was an amusing one.  Turns out the chip
 * 	needs a little time to settle the csr1 and csr2 info, more than
 * 	the r3000 allowed for.  Since we allocate the init block
 * 	differently from the prom's way the chip was getting it from the wrong
 * 	place.
 * 	[90/06/01            af]
 * 
 * Revision 2.3.3.1  90/05/30  16:13:06  af
 * 	Support for 3max, new autocof.  What a nightmare, and still does
 * 	not work.
 * 
 * Revision 2.3.2.1  90/05/02  23:51:02  rpd
 * 	Converted to new IPC.
 * 	[90/03/26  22:45:00  rpd]
 *
 * Revision 2.3  89/12/08  19:46:33  rwd
 * 	Do not lose buffers in case of errors.
 * 	Make it silent by default.
 * 	[89/12/05  02:24:10  af]
 * 
 * Revision 2.2  89/11/29  14:10:33  af
 * 	Heavily modified for pure Mach kernel.
 * 	BTW, since the device is known to users as "se", sounded
 * 	like pure braindamage to use different names here.
 * 	[89/11/21            af]
 * 
 * Revision 2.4  89/09/25  22:20:33  mja
 * 	Support Ultrix SIOCRPHYSADDR call.
 * 	[89/09/22  18:02:05  mja]
 * 
 * Revision 2.3  89/08/08  21:47:40  jsb
 * 	Fixed the argument definition for lnint(), since it
 * 	is called directly from intr().
 * 	[89/07/31            af]
 * 
 * Revision 2.2  89/05/31  14:27:10  rvb
 * 	For now turn off ln_verbose.  I know I get misses.
 * 	[89/05/31            rvb]
 * 
 * Revision 2.1  89/05/30  12:49:45  rvb
 * Created.
 * 
 * 18-May-89  Robert Baron (rvb) at Carnegie-Mellon University
 *	Created.
 */
/*
 *	File:	if_se.c
 *	Author: Robert V. Baron & Alessandro Forin
 *	Date:	5/90
 *
 *	Driver for the DEC LANCE Ethernet Controller.
 */

#include <ln.h>
#if     NLN > 0
#include <platforms.h>

#define IF_CNTRS 0

/*
 * AMD Am7990 LANCE (Ethernet Interface)
 */
#include <sys/ioctl.h>
#include <vm/vm_kern.h>

#include <machine/machparam.h>		/* spl definitions */
#include <sys/types.h>
#include <kern/time_out.h>
#include <sys/syslog.h>
#include <ipc/ipc_port.h>
#include <ipc/ipc_kmsg.h>

#include <device/device_types.h>
#include <device/errno.h>
#include <device/io_req.h>
#include <device/if_hdr.h>
#include <device/if_ether.h>
#include <device/net_status.h>
#include <device/net_io.h>

#include <chips/lance.h>
#include <chips/busses.h>

#include <machine/machparam.h>

#ifdef	DECSTATION
#include <mips/mips_cpu.h>
#include <mips/PMAX/pmad_aa.h>

#define	MAPPED 1

/*
 * The LANCE buffer memory as seen from the Pmax cpu is funny.
 * It is viewed as short words (16bits), spaced at word (32bits)
 * intervals.  The same applies to the registers.  From the LANCE
 * point of view memory is instead contiguous.
 * The ROM that contains the station address is in the space belonging
 * to the clock/battery backup memory.  This space is again 16 bits
 * in a 32bit envelope.  And the ether address is stored in the "high"
 * byte of 6 consecutive quantities.
 *
 * But Pmaxen and 3maxen (and..) map lance space differently.
 * This requires dynamic adaptation of the driver, which
 * is done via the following switches.
 * For convenience, the switch holds information about
 * the location of the lance control registers as well.
 * This could be either absolute (pmax) or relative to
 * some register base (3max, turbochannel)
 */
int copyin_gap16(), copyout_gap16(), bzero_gap16();
extern int bcopy(), bzero();
int copyin_gap32(), copyout_gap32();
static unsigned se_Hmem_gap16(), se_Hmem_nogap();

static struct se_switch se_switch[] = {
/* pmax */
	{ 0x00000000, 0x01000000, 0x0, 0x05000000, 8, 16, 64*1024, 
	  copyin_gap16, copyout_gap16, copyin_gap16, copyout_gap16,
	  bzero_gap16, se_Hmem_gap16, se_Hmem_gap16},
/* 3max */
	{ PMAD_OFFSET_LANCE, PMAD_OFFSET_RAM, PMAD_OFFSET_RAM, PMAD_OFFSET_ROM,
	  16, 0, PMAD_RAM_SIZE,
	  bcopy, bcopy, bcopy, bcopy, bzero, se_Hmem_nogap, se_Hmem_nogap},
/* 3min */
/* XXX re-use other 64k */
	{ 0/*later*/, 0/*later*/, 0x0, 0/*later*/, 0, 128, 64*1024,
	  copyin_gap16, copyout_gap16, copyin_gap32, copyout_gap32,
	  bzero_gap16, se_Hmem_gap16, se_Hmem_nogap},
};

/*
 * "lna" is what se_malloc hands back.  They are offsets using
 * the sizing that the Lance would use. The Lance space is
 * mapped somewhere in the I/O space, as indicated by the softc.
 * Hence we have these two macros:
 */
/* H & L are not hi and lo but
   H = HOST  == addresses for host to reference board memory
   L = LOCAL == addresses on board
 */
#define Hmem(lna)	(volatile char*)((se_sw->mapaddr)(lna) + sc->lnbuf)
#define Lmem(lna)	(volatile char*)((unsigned)lna + sc->lnoffset)
#endif	/*DECSTATION*/

#ifdef	VAXSTATION
#include <vax/ka3100.h>
#define wbflush()

static unsigned se_Hmem_nogap();

xzero(x, l) char *x; int l; { blkclr(x, l); }
xcopy(f, t, l) char *f, *t; int l; { bcopy(f, t, l); }

static struct se_switch se_switch[] = {
	/* pvax sees contiguous bits in lower 16Meg of memory */
	{ 0, 0, 0, 0, 0, 0, 64*1024,
	  xcopy, xcopy, xcopy, xcopy, xzero, se_Hmem_nogap, se_Hmem_nogap},
};

/*
 * "lna" is what se_malloc hands back.  They are offsets using
 * the sizing that the Lance would use. The Lance space is
 * mapped somewhere in the I/O space, as indicated by the softc.
 * Hence we have these two macros:
 */
/* H & L are not hi and lo but
   H = HOST  == addresses for host to reference board memory
   L = LOCAL == addresses on board
 */
	/*
	 * This does not deal with > 16 Meg physical memory, where
	 * Hmem != Lmem
	 */
#define Hmem(lna)	(volatile char*)((lna) + sc->lnbuf)
#define Lmem(lna)	(volatile char*)((lna) + sc->lnoffset)

#endif	/*VAXSTATION*/


static unsigned se_Hmem_gap16(lna) unsigned lna; { return lna << 1;}
static unsigned se_Hmem_nogap(lna) unsigned lna; { return lna;}

/*
 * Memory addresses for LANCE are 24 bits wide.
 */
#define Addr_lo(y)	((u_short)((unsigned)(y) & 0xffff))
#define	Addr_hi(y)	((u_short)(((unsigned)(y)>>16) & 0xff))

#define	LN_MEMORY_SIZE	(se_sw->ramsize)

/* XXX to accomodate heterogeneity this should be made per-drive */
/* XXX and then some more */

struct se_switch *se_sw = se_switch;
set_se_switch(n)
{
	se_sw = &se_switch[n];
}
setse_switch(n, r, b, l, o)
{
	se_switch[n].regspace = r;
	se_switch[n].bufspace = b;
	se_switch[n].ln_bufspace = l;
	se_switch[n].romspace = o;

	/* make sure longword aligned */
	if (se_switch[n].bufspace & 0x7) {
		se_switch[n].bufspace = (se_switch[n].bufspace+0x7) & ~0x7;
	}

	set_se_switch(n);
}

/*
 * Autoconf info
 */

static caddr_t se_std[NLN] = { 0 };
static struct bus_device *se_info[NLN];
static int se_probe(), se_attach();

struct bus_driver se_driver =
       { se_probe, 0, se_attach, 0, se_std, "se", se_info, };

/*
 * Externally visible functions
 */
char	*se_unprobed_addr = 0;
int	se_intr();				/* kernel */

int	se_open(), se_output(), se_get_status(),	/* user */
	se_set_status(), se_setinput(), se_restart();

/*
 *
 * Internal functions & definitions
 *
 */

static	int se_probe(), se_attach(), se_init();
static	init_lance_space(), se_desc_set_status();
static  volatile long *se_desc_alloc();
int	se_start();
static	void copy_from_lance();
static	int copy_to_lance();
static	int se_malloc();

int se_verbose = 0;	/* debug flag */

#define RLOG	4		/* 2**4 = 16  receive descriptors */
#define TLOG	4		/* 2**4 = 16  transmit descriptors */
#define NRCV	(1<<RLOG) 	/* Receive descriptors */
#define NXMT	(1<<TLOG) 	/* Transmit descriptors	*/

#define	LN_BUFFER_SIZE	(0x800-0x80)

/*
 * Ethernet software status per interface.
 *
 * Each interface is referenced by a network interface structure,
 * is_if, which contains the output queue for the interface, its address, ...
 */
int se_loopback_hack = 1;

struct	se_softc {
	struct	ifnet	is_if;		/* generic interface header	*/
	u_char	is_addr[6];		/* ethernet hardware address	*/
	u_short	pad;
	se_reg_t		lnregs;		/* Lance registers	*/
	vm_offset_t		lnbuf;		/* Lance memory, Host offset */
	vm_offset_t		lnoffset;	/* Lance memory, Lance offset */
	vm_offset_t		lnrom;
	long			lnsbrk;		/* Lance memory allocator */
	unsigned		lninit_block;	/* Init block address	*/
	se_desc_t		lnrring[NRCV];	/* Receive  ring desc. */
	volatile long 		*lnrbuf[NRCV];	/* Receive  buffers */
	se_desc_t		lntring[NXMT];	/* Transmit ring desc. */
	volatile long		*lntbuf[NXMT];	/* Transmit buffers */

	int	rcv_last;		/* Rcv buffer last read		*/

	io_req_t tpkt[NXMT+1];		/* Xmt pkt queue		*/
	int	xmt_count;		/* Xmt queue size		*/
	int	xmt_last;		/* Xmt queue head (insert)	*/
	int	xmt_complete;		/* Xmt queue tail (remove)	*/

	int	se_flags;		/* Flags for SIOCSIFFLAGS	*/
	int	counters[4];		/* error counters */
#define bablcnt  counters[0]
#define misscnt  counters[1]
#define merrcnt  counters[2]
#define rstrtcnt counters[3]
} se_softc_data[NLN];

typedef struct se_softc *se_softc_t;

se_softc_t	se_softc[NLN];		/* quick access */


#ifdef	IF_CNTRS
/*
 * Simple statistics
 */
int se_ein[32], se_eout[32]; 
int se_lin[128/8], se_lout[128/8]; 
int se_dump = 0;

static
log_2(i)
{
	/*
	 * Specialized log2, takes at most 2**EXPMAX
	 * (invoke with log_2(-1) to find EXPMAX)
	 */
#	define EXPMAX 11
	register int bit = EXPMAX;

	i <<= 31-EXPMAX;
	while (i > 0)
		bit--, i <<= 1;
	return i ? bit : 0;
#	undef	EXPMAX
}
#endif	IF_CNTRS

/*
 * Probe the Lance to see if it's there
 */
static int se_open_state = 0;

static se_probe(reg, ui)
	u_long reg;
	register struct bus_device *ui;
{
	register se_softc_t sc;
	se_reg_t        rdp, rap;
	int             unit = ui->unit;
	int             i;

	/*
	 * See if the interface is there by reading the lance CSR.  On pmaxen
	 * and 3maxen this is superfluous, but.. 
	 */
	rdp = (se_reg_t) (reg + se_sw->regspace);
#ifdef	DECSTATION
	if (check_memory(rdp, 0))
		return 0;
#endif	/*DECSTATION*/
#ifdef	MAPPED
	SE_probe(reg,ui);
#endif	/*MAPPED*/
	rap = rdp + 2;		/* XXX might not be true in the future XXX */
				/* rdp and rap are "shorts" on consecutive
				   "long" word boundaries */

	/*
	 * Bind this interface to the softc. 
	 */
	sc = &se_softc_data[unit];
	se_softc[unit] = sc;
	sc->lnregs	= (se_reg_t) (reg + se_sw->regspace);
	sc->lnbuf	= (vm_offset_t) (reg + se_sw->bufspace);
	sc->lnoffset	= (vm_offset_t) (se_sw->ln_bufspace);
	sc->lnrom	= (vm_offset_t) (reg + se_sw->romspace);

	/*
	 * Reset the interface, and make sure we really do it! (the 3max
	 * seems quite stubborn about these registers) 
	 */
	se_write_reg(rap, CSR0_SELECT, CSR0_SELECT, "RAP");
	se_write_reg(rdp, LN_CSR0_STOP, LN_CSR0_STOP, "csr0");

	/*
	 * Allocate lance RAM buffer memory 
	 */
	init_lance_space(sc);

	/*
	 * Initialize the chip
	 *
	 * NOTE: From now on we will only touch csr0
	 */
	if (se_ship_init_block(sc, unit))
		return 0;

	/*
	 * Tell the world we are alive and well 
	 */
	se_open_state++;
	return 1;
}

se_ship_init_block(sc, unit)
	register se_softc_t sc;
	int		unit;
{
	se_reg_t	rdp = sc->lnregs;
	se_reg_t	rap;
	register int    i = 0;

	rap = rdp + 2;		/* XXX might not be true in the future XXX */

	/*
	 * Load LANCE control block. 
	 */
	se_write_reg(rap, CSR1_SELECT, CSR1_SELECT, "RAP");
	se_write_reg(rdp, Addr_lo(Lmem(sc->lninit_block)),
		     Addr_lo(Lmem(sc->lninit_block)), "csr1");

	se_write_reg(rap, CSR2_SELECT, CSR2_SELECT, "RAP");
	se_write_reg(rdp, Addr_hi(Lmem(sc->lninit_block)),
		     Addr_hi(Lmem(sc->lninit_block)), "csr2");

	/*
	 * Start the INIT sequence now
	 */
	se_write_reg(rap, CSR0_SELECT, CSR0_SELECT, "RAP");
	*rdp = (LN_CSR0_IDON | LN_CSR0_INIT);
	wbflush();

	/* give it plenty of time to settle */
	while (i++ < 10000) {
		delay(100);
		if ((*rdp & LN_CSR0_IDON) != 0)
			break;
	}
	/* make sure got out okay */
	if ((*rdp & LN_CSR0_IDON) == 0) {
		printf("se%d: cannot initialize\n", unit);
		if (*rdp & LN_CSR0_ERR)
			printf("se%d: initialization error, csr = %04x\n",
			       unit, (*rdp & 0xffff));
		return 1;
	}
	/*
	 * Do not enable interrupts just yet. 
	 */
	se_write_reg(rdp, LN_CSR0_STOP, LN_CSR0_STOP, "csr0");

	return 0;
}
 
se_write_reg(regptr, val, result, regname)
	register volatile unsigned short *regptr;
	register		 val, result;
	char			*regname;
{
	register int    i = 0;

	while (*regptr != result) {
		*regptr = val;
		wbflush();
		if (++i > 10000) {
			printf("se: %s did not settle (to x%x): x%x\n",
			       regname, result, *regptr);
			return;
		}
		delay(100);
	}
}

static
init_lance_space(sc)
	register se_softc_t sc;
{
	register int   	lptr;			/* Generic lance pointer */
	se_desc_t	ringaddr;
	long           *rom_eaddress = (long *) sc->lnrom;
	int             i;
	struct se_init_block	init_block;

	/*
	 * Allocate local RAM buffer memory for the init block,
	 * fill in our local copy then copyout.
	 */

	sc->lninit_block = se_malloc(sc, sizeof (struct se_init_block));

	/*
	 * Set values on stack, then copyout en-masse
	 */
	bzero(&init_block, sizeof(init_block));
	init_block.mode = 0;
	init_block.phys_addr_low = ((rom_eaddress[0]>>se_sw->romstride)&0xff) |
			      (((rom_eaddress[1]>>se_sw->romstride)&0xff) << 8);
	init_block.phys_addr_med = ((rom_eaddress[2]>>se_sw->romstride)&0xff) |
			      (((rom_eaddress[3]>>se_sw->romstride)&0xff) << 8);
	init_block.phys_addr_high = ((rom_eaddress[4]>>se_sw->romstride)&0xff) |
			      (((rom_eaddress[5]>>se_sw->romstride)&0xff) << 8);

	/*
	 * Allocate both descriptor rings at once.
	 * Note that the quadword alignment requirement is
	 * inherent in the way we perform allocation,
	 * but it does depend on the size of the init block.
	 */
	lptr = se_malloc(sc, sizeof (struct se_desc) * (NXMT + NRCV));

	/*
	 * Initialize the buffer descriptors
	 */
	init_block.recv_ring_pointer_lo = Addr_lo(Lmem(lptr));
	init_block.recv_ring_pointer_hi = Addr_hi(Lmem(lptr));
	init_block.recv_ring_len = RLOG;

	for ( i = 0; i < NRCV ; i++, lptr += sizeof(struct se_desc)) {
		ringaddr = (se_desc_t)Hmem(lptr);
		sc->lnrring[i] = ringaddr;
		sc->lnrbuf[i] = se_desc_alloc (sc, ringaddr);
	}

	init_block.xmit_ring_pointer_lo = Addr_lo(Lmem(lptr));
	init_block.xmit_ring_pointer_hi = Addr_hi(Lmem(lptr));
	init_block.xmit_ring_len = TLOG;

	for ( i = 0 ; i < NXMT ; i++, lptr += sizeof(struct se_desc)) {
		ringaddr = (se_desc_t)Hmem(lptr);
		sc->lntring[i] = ringaddr;
		sc->lntbuf[i] = se_desc_alloc (sc, ringaddr);
	}

	/*
	 * No logical address filtering
	 */
	init_block.logical_addr_filter0 = 0;
	init_block.logical_addr_filter1 = 0;
	init_block.logical_addr_filter2 = 0;
	init_block.logical_addr_filter3 = 0;

	/*
	 * Move init block into lance space
	 */
	(se_sw->desc_copyout)(&init_block, Hmem(sc->lninit_block), sizeof(init_block));
	wbflush();
}

/*
 * Interface exists: make available by filling in network interface
 * record.  System will initialize the interface when it is ready
 * to accept packets.
 */
static
se_attach(ui)
	register struct bus_device *ui;
{
	u_char         *enaddr;
	struct ifnet   *ifp;
	long           *rom_eaddress;
	int             unit = ui->unit;
	se_softc_t	sc = se_softc[unit];

	rom_eaddress = (long *) sc->lnrom;

	/*
	 * Read the address from the prom and save it. 
	 */
	enaddr = sc->is_addr;
	enaddr[0] = (u_char) ((rom_eaddress[0] >> se_sw->romstride) & 0xff);
	enaddr[1] = (u_char) ((rom_eaddress[1] >> se_sw->romstride) & 0xff);
	enaddr[2] = (u_char) ((rom_eaddress[2] >> se_sw->romstride) & 0xff);
	enaddr[3] = (u_char) ((rom_eaddress[3] >> se_sw->romstride) & 0xff);
	enaddr[4] = (u_char) ((rom_eaddress[4] >> se_sw->romstride) & 0xff);
	enaddr[5] = (u_char) ((rom_eaddress[5] >> se_sw->romstride) & 0xff);

	printf(": %x-%x-%x-%x-%x-%x",
	       (rom_eaddress[0] >> se_sw->romstride) & 0xff,
	       (rom_eaddress[1] >> se_sw->romstride) & 0xff,
	       (rom_eaddress[2] >> se_sw->romstride) & 0xff,
	       (rom_eaddress[3] >> se_sw->romstride) & 0xff,
	       (rom_eaddress[4] >> se_sw->romstride) & 0xff,
	       (rom_eaddress[5] >> se_sw->romstride) & 0xff);

	/*
	 * Initialize the standard interface descriptor 
	 */
	ifp = &sc->is_if;
	ifp->if_unit = unit;
	ifp->if_header_size = sizeof(struct ether_header);
	ifp->if_header_format = HDR_ETHERNET;
	ifp->if_address_size = 6;
	ifp->if_mtu = ETHERMTU;
	ifp->if_flags |= IFF_BROADCAST;

	ifp->if_address = (char *) enaddr;

	if_init_queues(ifp);
#ifdef	MAPPED
	SE_attach(ui);
#endif	/*MAPPED*/
}

/*
 * Use a different hardware address for interface
 */
se_setaddr(eaddr, unit)
	u_char	eaddr[6];
{
	register se_softc_t sc = se_softc[unit];
	struct se_init_block	init_block;

	/*
	 * Modify initialization block accordingly
	 */
	(se_sw->desc_copyin) (Hmem(sc->lninit_block), &init_block, sizeof(init_block));
	bcopy(eaddr, &init_block.phys_addr_low, sizeof(*eaddr));
	(se_sw->desc_copyout)(&init_block, Hmem(sc->lninit_block), sizeof(init_block));
	/*
	 * Make a note of it
	 */
	bcopy(eaddr, sc->is_addr, sizeof(*eaddr));

	/*
	 * Restart the interface
	 */
	se_restart(&sc->is_if);
	se_init(unit);
}

/*
 * Restart interface
 *
 * We use this internally on those errors that hang the chip,
 * not sure yet what use the MI code will make of it.
 *
 * After stopping the chip and effectively turning off the interface
 * we release all pending buffers and cause the chip to init
 * itself.  We do not enable interrupts here.
 */
se_restart( ifp )
	register struct ifnet *ifp;
{
	register se_softc_t sc = se_softc[ifp->if_unit];
	se_reg_t        rdp;
	register int    i;

	rdp = sc->lnregs;

	/*
	 * stop the chip 
	 */
	se_write_reg(rdp, LN_CSR0_STOP, LN_CSR0_STOP, "csr0");

	/*
	 * stop network activity 
	 */
	if (ifp->if_flags & IFF_RUNNING) {
		ifp->if_flags &= ~(IFF_UP | IFF_RUNNING);
		sc->se_flags &= ~(IFF_UP | IFF_RUNNING);
	}
	sc->rstrtcnt++;

	if (se_verbose)
		printf("se%d: %d restarts\n", ifp->if_unit, sc->rstrtcnt);

	/*
	 * free up any buffers currently in use 
	 */
	for (i = 0; i < NXMT; i++)
		if (sc->tpkt[i]) {
			iodone(sc->tpkt[i]);
			sc->tpkt[i] = (io_req_t) 0;
		}
	/*
	 * INIT the chip again, no need to reload init block address. 
	 */
	se_ship_init_block(sc, ifp->if_unit);

	return (0);
}

/*
 * Initialize the interface.
 */
static
se_init(unit)
	int unit;
{
	register se_softc_t	 sc = se_softc[unit];
	register se_desc_t	*rp;
	register struct ifnet	*ifp = &sc->is_if;
	se_reg_t        rdp;
	short           mode;
	int             s;
	int             i;

	if (ifp->if_flags & IFF_RUNNING)
		return;

	rdp = sc->lnregs;

	/*
	 * Init the buffer descriptors and indexes for each of the rings. 
	 */
	for (i = 0, rp = sc->lnrring; i < NRCV; i++, rp++)
		se_desc_set_status(*rp, LN_RSTATE_OWN);

	for (i = 0, rp = sc->lntring; i < NXMT; i++, rp++)
		se_desc_set_status(*rp, 0);

	sc->xmt_count = sc->xmt_complete = sc->xmt_last = sc->rcv_last = 0;

	/*
	 * Deal with loopback mode operation 
	 */
	s = splimp();

	(se_sw->desc_copyin) (Hmem(sc->lninit_block), &mode, sizeof(mode));

	if (ifp->if_flags & IFF_LOOPBACK
	    && ((mode & LN_MODE_LOOP) == 0)) {
		/* if not already in loopback mode, do external loopback */
		mode &= ~LN_MODE_INTL;
		mode |= LN_MODE_LOOP;
		(se_sw->desc_copyout) (&mode, Hmem(sc->lninit_block), sizeof(mode));
		se_restart(ifp);
		se_init(ifp->if_unit);
		splx(s);
		return;
	}

	ifp->if_flags |= (IFF_UP | IFF_RUNNING);
	sc->se_flags |= (IFF_UP | IFF_RUNNING);

	/*
	 * Start the Lance and enable interrupts 
	 */
	*rdp = (LN_CSR0_STRT | LN_CSR0_INEA);
	wbflush();

	/*
	 * See if anything is already queued 
	 */
	se_start(unit);
	splx(s);
}


/*
 * Shut off the lance
 */
se_stop(unit)
{
	se_reg_t        rdp = se_softc[unit]->lnregs;

	se_write_reg(rdp, LN_CSR0_STOP, LN_CSR0_STOP, "csr0");
}


/*
 * Open the device, declaring the interface up
 * and enabling lance interrupts.
 */
/*ARGSUSED*/
se_open(dev, flag)
	dev_t	dev;
	int	flag;
{
	register int		unit = minor(dev);
	register se_softc_t	sc = se_softc[unit];

	if (unit >= NLN)
		return EINVAL;
	if (!se_open_state)
		return ENXIO;

	sc->is_if.if_flags |= IFF_UP;
	se_open_state++;
	se_init(unit);
	return (0);
}

#ifdef	MAPPED
int se_use_mapped_interface[NLN];
#endif	/*MAPPED*/

se_normal(unit)
{
#ifdef	MAPPED
	se_use_mapped_interface[unit] = 0;
#endif	/*MAPPED*/
	if (se_softc[unit]) {
		se_restart(se_softc[unit]);
		se_init(unit);
	}
}

/*
 * Ethernet interface interrupt routine
 */
se_intr(unit,spllevel)
{
	register se_softc_t	 sc = se_softc[unit];
	se_reg_t 		 rdp;
	register struct ifnet	*ifp = &sc->is_if;
	register unsigned short	 csr;

#ifdef	MAPPED
	if (se_use_mapped_interface[unit])
		return SE_intr(unit,spllevel);
#endif	/*MAPPED*/

	if (se_open_state < 2) { /* Stray, or not open for business */
		rdp = (sc ? sc->lnregs : (se_reg_t)se_unprobed_addr);
		*rdp |= LN_CSR0_STOP;
		wbflush();
		return;
	}
	rdp = sc->lnregs;

	/*
	 * Read the CSR and process any error condition.
	 * Later on, restart the lance by writing back
	 * the CSR (for set-to-clear bits).
	 */
	csr = *rdp;		/* pick up the csr */

	/* drop spurious interrupts */
	if ((csr & LN_CSR0_INTR) == 0)
		return;
#ifdef	DECSTATION
	splx(spllevel);	/* drop priority now */
#endif	/*DECSTATION*/
again:
	/*
	 * Check for errors first
	 */
	if ( csr & LN_CSR0_ERR ) {
		if (csr & LN_CSR0_MISS) {
			/*
			 * Stop the chip to prevent a corrupt packet from
			 * being transmitted.  There is a known problem with
			 * missed packet errors causing corrupted data to
			 * be transmitted to the same host as was just
			 * transmitted, with a valid crc appended to the
			 * packet.  The only solution is to stop the chip,
			 * which will clear the Lance silo, thus preventing
			 * the corrupt data from being sent.
			 */
			se_write_reg(rdp, LN_CSR0_STOP, LN_CSR0_STOP, "csr0");

			sc->misscnt++;
			if (se_verbose) {
				int me = 0, lance = 0, index;
				struct se_desc r;
				for (index = 0; index < NRCV; index++) {
					(se_sw->desc_copyin)(sc->lnrring[index], &r, sizeof(r));
					if (r.status & LN_RSTATE_OWN)
						lance++;
					else
						me++;
				}
				printf("se%d: missed packet (%d) csr = %x, Lance %x, me %x\n",
					unit, sc->misscnt, csr, lance, me);
			}
			se_restart(ifp);
			se_init(unit);
			return;
		}
		if (csr & LN_CSR0_BABL) {
			sc->bablcnt++;
			if (se_verbose)
			    printf("se%d: xmt timeout (%d)\n",
			    	   unit, sc->bablcnt);
		}
		if (csr & LN_CSR0_MERR) {
			sc->merrcnt++;
			printf("se%d: memory error (%d)\n",
				   unit, sc->merrcnt);

			if (((csr & LN_CSR0_RXON) == 0)
			    || ((csr & LN_CSR0_TXON) == 0)) {
				se_restart(ifp);
				se_init(unit);
				return;
			}
		}
	}

	*rdp = LN_CSR0_INEA | (csr & LN_CSR0_WTC);
	wbflush();

	if ( csr & LN_CSR0_RINT )
		se_rint( unit );

	if ( csr & LN_CSR0_TINT )
		se_tint( unit );

	if ((csr = *rdp) & (LN_CSR0_RINT | LN_CSR0_TINT))
		goto again;
}
 
/*
 * Handle a transmitter complete interrupt.
 */
se_tint(unit)
	int unit;
{
	register se_softc_t sc = se_softc[unit];
	register        index;
	register        status;
	io_req_t        request;
	struct se_desc  r;

	/*
	 * Free up descriptors for all packets in queue for which
	 * transmission is complete.  Start from queue tail, stop at first
	 * descriptor we do not OWN, or which is in an inconsistent state
	 * (lance still working). 
	 */

	while ((sc->xmt_complete != sc->xmt_last) && (sc->xmt_count > 0)) {

		index = sc->xmt_complete;
		(se_sw->desc_copyin) (sc->lntring[index], &r, sizeof(r));
		status = r.status;

		/*
		 * Does lance still own it ? 
		 */
		if (status & LN_TSTATE_OWN)
			break;

		/*
		 * Packet sent allright, release queue slot.
		 */
		request = sc->tpkt[index];
		sc->tpkt[index] = (io_req_t) 0;
		sc->xmt_complete = ++index & (NXMT - 1);
		--sc->xmt_count;

		sc->is_if.if_opackets++;
		if (status & (LN_TSTATE_DEF|LN_TSTATE_ONE|LN_TSTATE_MORE))
			sc->is_if.if_collisions++;

		/*
		 * Check for transmission errors. 
		 */
		if (!se_loopback_hack && status & LN_TSTATE_ERR) {
			sc->is_if.if_oerrors++;
			if (se_verbose)
				printf("se%d: xmt error (x%x)\n", unit, r.status2);

			if (r.status2 & (LN_TSTATE2_RTRY|LN_TSTATE2_LCOL))
				sc->is_if.if_collisions++;

			/*
			 * Restart chip on errors that disable the
			 * transmitter. 
			 */
			iodone(request);
			if (r.status2 & LN_TSTATE2_DISABLE) {
				register struct ifnet *ifp = &sc->is_if;
				se_restart(ifp);
				se_init(ifp->if_unit);
				return;
			}
		} else if (request) {
			/*
			 * If this was a broadcast packet loop it back.
			 * Signal successful transmission of the packet. 
			 */
			register struct ether_header *eh;
			register int    i;

			eh = (struct ether_header *) request->io_data;
			/* ether broadcast address is in the spec */
			for (i = 0; (i < 6) && (eh->ether_dhost[i] == 0xff); i++)
				; /* nop */
			/* sending to ourselves makes sense sometimes */
			if (i != 6 && se_loopback_hack)
				for (i = 0;
				     (i < 6) && (eh->ether_dhost[i] == sc->is_addr[i]);
				     i++)
				; /* nop */
			if (i == 6)
				se_read(sc, 0, request->io_count, request);
			iodone(request);
		}
	}
	/*
	 * Dequeue next transmit request, if any. 
	 */
	if (sc->xmt_count <= 0)
		se_start(unit);
}
 
/*
 * Handle a receiver complete interrupt.
 */
se_rint(unit)
	int unit;
{
	register se_softc_t	sc = se_softc[unit];
	register        index, first, len;
	u_char          status, status1;
	int             ring_cnt;
	struct se_desc  r;

	/*
	 * Starting from where we left off, look around the receive ring and
	 * pass on all complete packets. 
	 */

	for (;; sc->rcv_last = ++index & (NRCV - 1)) {

		/*
		 * Read in current descriptor 
		 */
read_descriptor:
		(se_sw->desc_copyin) (sc->lnrring[sc->rcv_last], &r, sizeof(r));
		status = r.status;
		if (status & LN_RSTATE_OWN)
			break;
		first = index = sc->rcv_last;

		/*
		 * If not the start of a packet, error 
		 */
		if (!(status & LN_RSTATE_STP)) {
			if (se_verbose)
				printf("se%d: Rring #%d, status=%x !STP\n",
				       index, status);
			break;
		}
		/*
		 * See if packet is chained (should not) by looking at
		 * the last descriptor (OWN clear and ENP set).
		 * Remember the status info in this last descriptor. 
		 */
		ring_cnt = 1, status1 = status;
		while (((status1 & (LN_RSTATE_ERR | LN_RSTATE_OWN | LN_RSTATE_ENP)) == 0) &&
		       (ring_cnt++ <= NRCV)) {
			struct se_desc  r1;
			index = (index + 1) & (NRCV - 1);
			(se_sw->desc_copyin) (sc->lnrring[index], &r1, sizeof(r1));
			status1 = r1.status;
		}

		/*
		 * Chained packet (--> illegally sized!); re-init the
		 * descriptors involved and ignore this bogus packet.  I
		 * donno how, but it really happens that we get these
		 * monsters. 
		 */
		if (ring_cnt > 1) {
			/*
			 * Return all descriptors to lance 
			 */
			se_desc_set_status(sc->lnrring[first], LN_RSTATE_OWN);
			while (first != index) {
				first = (first + 1) & (NRCV - 1);
				se_desc_set_status(sc->lnrring[first], LN_RSTATE_OWN);
			}
			if ((status1 & LN_RSTATE_ERR) && se_verbose)
				printf("se%d: rcv error %x (chained)\n", unit, status1);
			continue;
		}

		/*
		 * Good packets must be owned by us and have the end of
		 * packet flag.  And nothing else. 
		 */
		if ((status & ~LN_RSTATE_STP) == LN_RSTATE_ENP) {
			sc->is_if.if_ipackets++;

			if ((len = r.message_size) == 0)
				/* race seen on pmaxen: the lance
				 * has not updated the size yet ??
				 */
				goto read_descriptor;
#ifdef	IF_CNTRS
			se_ein[log_2(len)]++;
			if (len < 128)
				se_lin[len >> 3]++;
#endif	IF_CNTRS
			/*
			 * Drop trailing CRC bytes from len and ship packet
			 * up 
			 */
			se_read(sc, sc->lnrbuf[first], len - 4, 0);

			/*
			 * Return descriptor to lance, and move on to next
			 * packet 
			 */
			r.status = LN_RSTATE_OWN;
			(se_sw->desc_copyout) (&r, sc->lnrring[first], sizeof(r));
			continue;
		}
		/*
		 * Not a good packet, see what is wrong 
		 */
		if (status & LN_RSTATE_ERR) {
			sc->is_if.if_ierrors++;

			if (se_verbose)
				printf("se%d: rcv error (x%x)\n", unit, status);

			/*
			 * Return descriptor to lance 
			 */
			se_desc_set_status(sc->lnrring[first], LN_RSTATE_OWN);
		} else {
			/*
			 * Race condition viz lance, Wait for the next
			 * interrupt. 
			 */
			return;
		}
	}
}


/*
 * Output routine.
 * Call common function for wiring memory,
 * come back later (to se_start) to get
 * things going.
 */
io_return_t
se_output(dev, ior)
	dev_t		dev;
	io_req_t	ior;
{
	return (net_write(&se_softc[minor(dev)]->is_if, se_start, ior));
}

 
/*
 * Start output on interface.
 *
 */
se_start(unit)
	int	unit;
{
	register se_softc_t sc = se_softc[unit];
	io_req_t        request;
	struct se_desc  r;
	int             tlen, s;
	register int    index;

	s = splimp();

	for (index = sc->xmt_last;
	     sc->xmt_count < (NXMT - 1);
	     sc->xmt_last = index = (index + 1) & (NXMT - 1)) {
		/*
		 * Dequeue the next transmit request, if any. 
		 */
		IF_DEQUEUE(&sc->is_if.if_snd, request);
		if (request == 0) {
			splx(s);
			return;	/* Nothing on the queue	 */
		}

		/*
		 * Keep request around until transmission complete
		 */
		sc->tpkt[index] = request;
		tlen = copy_to_lance(request, sc->lntbuf[index]);

		/*
		 * Give away buffer.  Must copyin/out, set len,
		 * and set the OWN flag.  We do not do chaining.
		 */
		(se_sw->desc_copyin)(sc->lntring[index], &r, sizeof(r));
		r.buffer_size = -(tlen) | 0xf000;
		r.status = (LN_TSTATE_OWN | LN_TSTATE_STP | LN_TSTATE_ENP);
		(se_sw->desc_copyout)(&r, sc->lntring[index], sizeof(r));
		wbflush();

		sc->xmt_count++;
	}
	/*
	 * Since we actually have queued new packets, tell
	 * the chip to rescan the descriptors _now_.
	 * It is quite unlikely that the ring be filled,
	 * but if it is .. the more reason to do it!
	 */
	*sc->lnregs = LN_CSR0_TDMD | LN_CSR0_INEA;

	splx(s);
}


/*
 * Pull a packet off the interface and
 * hand it up to the higher levels.
 *
 * Simulate broadcast packets in software.
 */
se_read(sc, lnrbuf, len, loop_back)
	register se_softc_t	 sc;
	volatile char		*lnrbuf;
	int			 len;
	io_req_t		 loop_back;
{
	register struct ifnet *ifp = &sc->is_if;
	register ipc_kmsg_t	new_kmsg;
	char			*addr, *hdr, *pkt;

	if (len <= sizeof(struct ether_header))
		return;	/* sanity */

	/*
	 * Get a new kmsg to put data into.
	 */
	new_kmsg = net_kmsg_get();
	if (new_kmsg == IKM_NULL) {
	    /*
	     * No room, drop the packet
	     */
	    ifp->if_rcvdrops++;
	    return;
	}

	hdr = net_kmsg(new_kmsg)->header;
	pkt = net_kmsg(new_kmsg)->packet;

#define OFF0 (sizeof(struct ether_header) - sizeof(struct packet_header))
#define OFF1 (OFF0 & ~3)
	if (loop_back) {
		bcopy(loop_back->io_data, hdr, sizeof(struct ether_header));
		bcopy(loop_back->io_data + OFF0,
			pkt, len - OFF0);
	} else
		copy_from_lance(lnrbuf, len, hdr, pkt);

	/*
	 * Set up the 'fake' header with length.  Type has been left
	 * in the correct place.
	 */
	len = len - OFF0;
	((struct packet_header *)pkt)->length = len;

#ifdef	IF_CNTRS
	if (se_dump) {
		printf("[r %x %x %x]\n",
			new_kmsg, len, loop_back);
		if (se_dump > 1)
			gimmeabreak();
	}
#endif	IF_CNTRS

	/*
	 * Hand the packet to the network module.
	 */
	net_packet(ifp, new_kmsg, len, ethernet_priority(new_kmsg));
}


/*
 * Get a packet out of Lance memory and into main memory.
 */
static void
copy_from_lance(rbuf, nbytes, hdr, pkt)
	register volatile u_char *rbuf;
	register unsigned	  nbytes;
	struct ether_header	 *hdr;
	struct packet_header 	 *pkt;
{
	/*
	 * Read in ethernet header 
	 */
	(se_sw->data_copyin) (rbuf, hdr, sizeof(struct ether_header));

	nbytes -= sizeof(struct ether_header);
	rbuf += (se_sw->mapoffs) (sizeof(struct ether_header));

	pkt->type = (u_short) hdr->ether_type;

#ifdef	IF_CNTRS
	if (se_dump > 1) {
		register u_short *dp = (u_short *) hdr;
		printf("[i %x %x %x %x %x %x %x ...]\n",
		       dp[0], dp[1], dp[2], dp[3], dp[4], dp[5], dp[6]);
	}
#endif	IF_CNTRS

	(se_sw->data_copyin) (rbuf, pkt + 1, nbytes);
}


/*
 * Move a packet into Lance space
 */
static int
copy_to_lance(request, sbuf)
	register io_req_t request;
	volatile long	 *sbuf;
{
	register u_short *dp;
	register int    len;

	dp = (u_short *) request->io_data;
	len = request->io_count;

#ifdef	IF_CNTRS
	se_eout[log_2(len)]++;
	if (len < 128)
		se_lout[len >> 3]++;
	if (se_dump) {
		printf("[o %x %x %x %x %x %x %x - %x %x %x %x %x %x %x %x %x %x..]\n",
		       dp[0], dp[1], dp[2], dp[3], dp[4], dp[5], dp[6],
		dp[7], dp[8], dp[9], dp[10], dp[11], dp[12], dp[13], dp[14],
		       dp[15], dp[16]);

	}
#endif	IF_CNTRS

	if (len > ETHERMTU + sizeof(struct ether_header)) {
		printf("se: truncating HUGE packet\n");
		len = ETHERMTU + sizeof(struct ether_header);
	}

	(se_sw->data_copyout) (dp, sbuf, len);

	if (len < LN_MINBUF_NOCH)
		/*
		 * The lance needs at least this much data in a packet. Who
		 * cares if I send some garbage that was left in the lance
		 * buffer ?  If one can spoof packets then one can spoof
		 * packets!
		 */
		len = LN_MINBUF_NOCH;
	return len;
}

/*
 * Reset a descriptor's flags.
 * Optionally give the descriptor to the lance
 */
static
se_desc_set_status (lndesc, val)
	register se_desc_t	lndesc;
{
	struct se_desc		desc;

	(se_sw->desc_copyin) (lndesc, &desc, sizeof(desc));
	desc.desc4.bits = 0;
	desc.status     = val;
	(se_sw->desc_copyout) (&desc, lndesc, sizeof(desc));
	wbflush();
}



/*
 * Set/Get status functions
 */
se_get_status(dev, flavor, status, status_count)
	dev_t		 dev;
	int		 flavor;
	dev_status_t	 status;	/* pointer to OUT array */
	unsigned int	*status_count;		/* out */
{
	return (net_getstat(&se_softc[minor(dev)]->is_if,
			    flavor, status, status_count));
}

se_set_status(dev, flavor, status, status_count)
	dev_t		dev;
	int		flavor;
	dev_status_t	status;
	unsigned int	status_count;
{
	register se_softc_t	sc;
	int			unit = minor(dev);

	sc = se_softc[unit];

	switch (flavor) {
	case NET_STATUS:
		{
			/* XXX */
			break;
		}

	case NET_ADDRESS:
		{
			register union ether_cvt {
				char            addr[6];
				int             lwd[2];
			}              *ec = (union ether_cvt *) status;

			if (status_count < sizeof(*ec) / sizeof(int))
				return (D_INVALID_SIZE);

			ec->lwd[0] = ntohl(ec->lwd[0]);
			ec->lwd[1] = ntohl(ec->lwd[1]);

			se_setaddr(ec->addr, unit);

			break;
		}
	default:
		return (D_INVALID_OPERATION);
	}
	return (D_SUCCESS);
}


/*
 * Install new filter.
 * Nothing special needs to be done here.
 */
io_return_t
se_setinput(dev, receive_port, priority, filter, filter_count)
	dev_t		dev;
	ipc_port_t	receive_port;
	int		priority;
	filter_t	*filter;
	unsigned int	filter_count;
{
	return (net_set_filter(&se_softc[minor(dev)]->is_if,
			       receive_port, priority,
			       filter, filter_count));
}

/*
 * Allocate and initialize a ring descriptor.
 */
static volatile long
*se_desc_alloc (sc, lndesc)
	register se_softc_t	sc;
	register se_desc_t	lndesc;
{
	register long   dp;	/* data pointer */
	struct se_desc  desc;

	/*
	 * Allocate buffer in lance space 
	 */
	dp = se_malloc(sc, LN_BUFFER_SIZE);

	/*
	 * Build a descriptor pointing to it 
	 */
	desc.addr_low = Addr_lo(Lmem(dp));
	desc.addr_hi  = Addr_hi(Lmem(dp));
	desc.status   = 0;
	desc.buffer_size = -LN_BUFFER_SIZE;
	desc.desc4.bits  = 0;

	/*
	 * Copy the descriptor to lance space 
	 */
	(se_sw->desc_copyout) (&desc, lndesc, sizeof(desc));
	wbflush();

	return (volatile long *) Hmem(dp);
}

/*
 * Allocate a chunk of lance RAM buffer. Since we never
 * give lance RAM buffer memory back, we'll just step up the
 * byte-count on a per-unit basis.
 */
static int
se_malloc(sc, size)
	se_softc_t	sc;
	int		size;
{
	register int    ret;

	/*
	 * On first call, zero lance memory 
	 */
	if (sc->lnsbrk == 0)
		(se_sw->bzero) (Hmem(0), LN_MEMORY_SIZE);

	/*
	 * Start out on the first double longword boundary
	 * (this accomodates some machines, with minimal loss)
	 */
	if (sc->lnsbrk & 0xf)
		sc->lnsbrk = (sc->lnsbrk + 0x10) & ~0xf;

	ret = sc->lnsbrk;
	sc->lnsbrk += size;

	if (sc->lnsbrk > LN_MEMORY_SIZE)
		panic("se_malloc");

	return ret;
}

#endif     NLN > 0
