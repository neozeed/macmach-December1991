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
 * $Log:	if_en.c,v $
 * Revision 2.2  91/09/12  16:49:20  bohman
 * 	Created.
 * 	[91/09/11  16:17:20  bohman]
 * 
 * Revision 2.2  90/08/30  11:07:24  bohman
 * 	Created.
 * 	[90/08/29  12:30:35  bohman]
 * 
 */

/*
 *	Apple Macintosh II Mach (macmach)
 *
 *	File: mac2if/if_en.c
 *	Author: David E. Bohman II (CMU macmach)
 */

#include <kern/time_out.h>
#include <device/device_types.h>
#include <device/errno.h>
#include <device/io_req.h>
#include <device/if_hdr.h>
#include <device/if_ether.h>
#include <device/net_status.h>
#include <device/net_io.h>

#include <mac2if/if_en.h>

#include <mac2os/Types.h>
#include <mac2os/Errors.h>
#include <mac2os/Slots.h>
#include <mac2os/Files.h>
#include <mac2os/Ethernet.h>

/*
 * Macintosh ethernet driver
 * for MACH 3.0.  This driver
 * uses the MacOS .ENET driver
 * for its interface with the
 * hardware so it should work with
 * third party cards.
 */

#define	NEN		6

struct en_softc {
	struct ifnet	en_if;
	ether_address_t	en_enaddr;
	short		en_RefNum;
	int		en_flags;
#define ENF_OACTIVE	0x01
	io_req_t	en_ior;
	Eiopb		*en_pb;
	wdsEntry	en_wds[2];
	Ptr		en_packet;
} en_softc[NEN];

int	nen;

boolean_t	endebug = 0;

static
boolean_t
en_setup(slot, id, is)
register			slot, id;
register struct en_softc	*is;
{
    register short	RefNum;
    SpBlock		slot_params;
    extern boolean_t	mac32bit;

    slot_params.spSlot = slot;
    slot_params.spID = id;
    slot_params.spExtDev = 0;
    if (SRsrcInfo(&slot_params) != noErr)
	return (FALSE);

    slot_params.spID = 128; /* XXX */
    if (SFindStruct(&slot_params) != noErr)
	return (FALSE);

    slot_params.spSize = sizeof (ether_address_t);
    slot_params.spResult = (vm_offset_t)is->en_enaddr;
    if (SReadStruct(&slot_params) != noErr)
	return (FALSE);

    if (slot_params.spRefNum == 0) {
	SlotDevParam	open_params;

	open_params.ioNamePtr = (StringPtr)"X.ENET";
	open_params.ioNamePtr->length = 5;
	open_params.ioPermssn = 0;

	open_params.ioFlags = 0;
	open_params.ioSlot = slot;
	open_params.ioID = id;

	if (OpenSlot(&open_params, FALSE) != noErr)
	    return (FALSE);

	RefNum = open_params.ioRefNum;
    }
    else
	RefNum = slot_params.spRefNum;

    is->en_pb = (Eiopb *)NewPtr(sizeof (Eiopb));
    if (!is->en_pb)
	return (FALSE);

    is->en_packet = (Ptr)NewPtr(EMaxPacketSz);
    if (!is->en_packet)
	return (FALSE);

    is->en_pb->ioRefNum = RefNum;
    is->en_pb->csCode = ESetGeneral;
    if (PBControl(is->en_pb, FALSE) != noErr)
	return (FALSE);

    is->en_RefNum = RefNum;

    return (TRUE);
}

/*
 * this routine is for configuring the network IF
 * and is called from the autoconfig system.
 *
 */
boolean_t
enattach(slot, id)
register	slot, id;
{
    register struct en_softc	*is;
    register struct ifnet	*ifp;
    register			unit;

    if (nen >= NEN)
	return (FALSE);

    unit = nen++;
    is = &en_softc[unit];

    /*
     * Check for driver
     */
    if (!en_setup(slot, id, is))
	return (FALSE);

    /*
     * initialize the if
     */
    ifp = &is->en_if;

    ifp->if_unit = unit;
    ifp->if_header_size = sizeof (ether_header_t);
    ifp->if_header_format = HDR_ETHERNET;
    ifp->if_address_size = sizeof (ether_address_t);
    ifp->if_mtu = ETHERMTU;
    ifp->if_flags = IFF_BROADCAST;
    ifp->if_address = (char *)is->en_enaddr;
    if_init_queues(ifp);
    printf("ether sRsrc: slot %x id %d assigned to unit %d\n",
	   slot, id, unit);

    return (TRUE);
}

static
boolean_t
enblproto(is, type)
register struct en_softc	*is;
unsigned short			type;
{
    register			unit = (is - en_softc);
    register Eiopb		*pb;
    register boolean_t		result;
    extern unsigned long	enproto[];

    pb = (Eiopb *)NewPtr(sizeof (Eiopb));
    if (!pb)
	return (FALSE);

    pb->ioRefNum = is->en_RefNum;
    pb->csCode = EDetachPH;
    pb->p.proto.EProtType = type;
    (void) thread_io(PBControl, pb);

    pb->ioRefNum = is->en_RefNum;
    pb->csCode = EAttachPH;
    pb->p.proto.EProtType = type;
    pb->p.proto.EHandler = (ProcPtr)&enproto[unit];
    (void) thread_io(PBControl, pb);

    result = (pb->ioResult == noErr);

    DisposPtr(pb);

    return (result);
}

static
boolean_t
addmulti(is, address)
register struct en_softc	*is;
ether_address_t			address;
{
    register Eiopb		*pb;
    register boolean_t		result;

    pb = (Eiopb *)NewPtr(sizeof (Eiopb));
    if (!pb)
	return (FALSE);

    pb->ioRefNum = is->en_RefNum;
    pb->csCode = EAddMulti;
    bcopy(address, pb->p.multi.EMultiAddr, sizeof (ether_address_t));
    (void) thread_io(PBControl, pb);

    result = (pb->ioResult == noErr);

    DisposPtr(pb);

    return (result);
}

static
boolean_t
delmulti(is, address)
register struct en_softc	*is;
ether_address_t			address;
{
    register Eiopb		*pb;
    register boolean_t		result;

    pb = (Eiopb *)NewPtr(sizeof (Eiopb));
    if (!pb)
	return (FALSE);

    pb->ioRefNum = is->en_RefNum;
    pb->csCode = EDelMulti;
    bcopy(address, pb->p.multi.EMultiAddr, sizeof (ether_address_t));
    (void) thread_io(PBControl, pb);

    result = (pb->ioResult == noErr);

    DisposPtr(pb);

    return (result);
}

#define ETHERTYPE_ARP	0x806
#define ETHERTYPE_IP	0x800

static
void
eninit(unit)
int	unit;
{
    register struct en_softc	*is = &en_softc[unit];

    if (is->en_if.if_flags & IFF_RUNNING)
	return;

    (void) enblproto(is, ETHERTYPE_ARP);

    (void) enblproto(is, ETHERTYPE_IP);

    is->en_if.if_flags |= IFF_RUNNING;
}

io_return_t
enopen(dev, flag)
dev_t	dev;
int	flag;
{
    register			unit = minor(dev);
    register struct en_softc	*is = &en_softc[unit];

    if (unit < 0 || unit >= nen || is->en_RefNum == 0)
	return (ENXIO);

    is->en_if.if_flags |= IFF_UP;
    eninit(unit);

    return (0);
}

static
void
enstart(unit)
int	unit;
{
    register struct en_softc	*is = &en_softc[unit];
    register io_req_t		ior;
    register Eiopb		*pb = is->en_pb;
    extern void			endone();
    
    if (is->en_flags & ENF_OACTIVE)
	return;

    IF_DEQUEUE(&is->en_if.if_snd, ior);
    if (ior == 0)
	return;

    is->en_flags |= ENF_OACTIVE;

    is->en_wds[0].length = ior->io_count;

    bcopy(ior->io_data, is->en_packet, ior->io_count);
    is->en_wds[0].ptr = is->en_packet;
    is->en_ior = ior;

    is->en_wds[1].length = 0;
    pb->ioRefNum = is->en_RefNum;
    pb->ioVRefNum = unit;
    pb->ioCompletion = endone;
    pb->csCode = EWrite;
    pb->p.write.EWdsPointer = is->en_wds;
    (void) PBControl(pb, TRUE);
}

void
encomplete(pb)
register Eiopb	*pb;
{
    register			unit = pb->ioVRefNum;
    register struct en_softc	*is = &en_softc[unit];
    register io_req_t		ior;
    register			s;

    if ((is->en_flags & ENF_OACTIVE) == 0)
	return;

    is->en_flags &= ~ENF_OACTIVE;

    ior = is->en_ior;

    if (pb->ioResult == noErr)
	is->en_if.if_opackets++;
    else {
	ior->io_error = D_IO_ERROR;
	is->en_if.if_oerrors++;
    }

    iodone(ior);

    s = splimp();
    if (is->en_if.if_snd.ifq_len > 0)
	enstart(unit);

    splx(s);
}

io_return_t
enoutput(dev, ior)
dev_t		dev;
io_req_t	ior;
{
    register			unit = minor(dev);
    register struct en_softc	*is = &en_softc[unit];

    if (unit < 0 || unit >= nen || is->en_RefNum == 0)
	return (ENXIO);

    return (net_write(&is->en_if, enstart, ior));
}

io_return_t
ensetinput(dev, receive_port, priority, filter, filter_count)
dev_t		dev;
mach_port_t	receive_port;
int		priority;
filter_t	filter[];
unsigned int	filter_count;
{
    register			unit = minor(dev);
    register struct en_softc	*is = &en_softc[unit];

    if (unit < 0 || unit >= nen || is->en_RefNum == 0)
	return (ENXIO);

    return (net_set_filter(&is->en_if,
			   receive_port, priority,
			   filter, filter_count));
}

io_return_t
engetstat(dev, flavor, status, count)
dev_t		dev;
int		flavor;
dev_status_t	status;		/* pointer to OUT array */
unsigned int	*count;		/* out */
{
    register			unit = minor(dev);
    register struct en_softc	*is = &en_softc[unit];

    if (unit < 0 || unit >= nen || is->en_RefNum == 0)
	return (ENXIO);

    return (net_getstat(&is->en_if,
			flavor,
			status,
			count));
}

io_return_t
ensetstat(dev, flavor, status, count)
dev_t		dev;
int		flavor;
dev_status_t	status;
unsigned int	count;
{
    register			unit = minor(dev);
    register struct en_softc	*is = &en_softc[unit];

    if (unit < 0 || unit >= nen || is->en_RefNum == 0)
	return (ENXIO);

    switch (flavor) {
      case EN_ENBL_PROTO:
	{
	    register en_enbl_proto_t	*list = (en_enbl_proto_t *)status;

	    while (count-- > 0) {
		if (!enblproto(is, (unsigned short)*list++))
		    return (D_NO_MEMORY);
	    }
	}
	break;

      case EN_ADD_MULTI:
	if (count < EN_ADDRESS_STATUS_COUNT
	    || !addmulti(is, status))
	    return (D_INVALID_OPERATION);
	break;

      case EN_DEL_MULTI:
	if (count < EN_ADDRESS_STATUS_COUNT
	    || !delmulti(is, status))
	    return (D_INVALID_OPERATION);
	break;

      default:
	return (D_INVALID_OPERATION);
    }

    return (D_SUCCESS);
}

void
eninput(unit, eh, length, rprr, regs)
int			unit;
ether_header_t		*eh;
register unsigned	length;
unsigned short		rprr[];
unsigned		regs;
{
    register ipc_kmsg_t			kmsg;
    register struct packet_header	*pkt;
#define ifp	(&en_softc[unit].en_if)
#define readpacket(buffer, length) \
    asm volatile("moveml %0@,a0-a1;  movl %1,a3; movl %2,d3; jsr %3"	      \
		 :							      \
		 : "a" (&regs), "g" (buffer), "g" (length), "m" (rprr[1])   \
		 : "d0", "d1", "d3", "a0", "a1", "a3" )

    if (endebug) printf("eninput eh %x length %d\n", eh, length);

    ifp->if_ipackets++;

    kmsg = net_kmsg_get();
    if (kmsg == IKM_NULL) {
	readpacket(0, 0);
	ifp->if_rcvdrops++;
    }
    else {
	bcopy(eh, &net_kmsg(kmsg)->header[0], sizeof (ether_header_t));

	pkt = (struct packet_header *)(net_kmsg(kmsg)->packet);

	pkt->type = eh->type;
	pkt->length = length + sizeof (struct packet_header);

	readpacket((pkt + 1), length);

	{
	    register	s = splimp();

	    net_packet(ifp, kmsg, pkt->length, ethernet_priority(kmsg));

	    splx(s);
	}
    }

    if (endebug) printf("eninput exit\n");
#undef readpacket
#undef ifp
}
