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
 * $Log:	slots.c,v $
 * Revision 2.2  91/09/12  16:43:52  bohman
 * 	Created.
 * 	[91/09/11  15:03:36  bohman]
 * 
 */

/*
 *	Apple Macintosh II Mach (macmach)
 *
 *	File: mac2/slots.c
 *	Author: David E. Bohman II (CMU macmach)
 */

#include <mach/mach_types.h>

#include <mac2os/Types.h>
#include <mac2os/Errors.h>
#include <mac2os/ROMDefs.h>
#include <mac2os/Retrace.h>
#include <mac2os/Slots.h>

#include <mac2/slots.h>

struct slotdata		slots[SLOT_NUM_HIGH-SLOT_NUM_LOW+1];

static void	config_ignored_sRsrc();

/*
 * Configure all slot devices.
 */
void
config_slots()
{
    register slot, id;
    register unsigned short category, ctype;
    register SInfoRecPtr si;
    SpBlock slot_params;

    slot_params.spSlot = 0;
    slot_params.spID = slot_params.spExtDev = 0;

    if (SNextsRsrc(&slot_params) != noErr)
	return;

    for (;;) {
	slot = slot_params.spSlot;
	id = slot_params.spID;
	category = slot_params.spCategory;
	ctype = slot_params.spCType;

	if (category == catBoard || category == catTest)
	    ;
	else
	if (category == catDisplay && ctype == typeVideo) {
	    if (video_config(slot, id) == FALSE)
		config_ignored_sRsrc(&slot_params);
	}
	else
	if (category == catNetwork && ctype == typeEtherNet) {
	    if (enattach(slot, id) == FALSE)
		config_ignored_sRsrc(&slot_params);
	}

	if (SNextsRsrc(&slot_params) != noErr)
	    break;
    }
}

static
void
config_ignored_sRsrc(p)
SpBlockPtr p;
{
    printf("ignored sRsrc: slot %x id %x category %x ctype %x\n",
	   p->spSlot, p->spID, p->spCategory, p->spCType);
}

void
macslot_intr(slot)
{
    register slot_ptr_t	p = slot_to_slotdata_ptr(slot);

    if (CHECKACTLIST(p->SActList))
	doactlist(&p->SActList);

    p->STaskPtr->vblCount = 1;
}

void
macslot_install(slot, rout)
void	(*rout)();
{
    register slot_ptr_t	p = slot_to_slotdata_ptr(slot);
    VBLTaskPtr	v;

    v = (VBLTaskPtr)NewPtr(sizeof (VBLTask));
    if (v) {
	p->STaskPtr = v;
	v->qLink = 0;
	v->qType = 1;
	v->vblAddr = rout;
	v->vblCount = 1;
	v->vblPhase = 0;
	SlotVInstall(v, slot);
    }
}

void
slot_tasks_install()
{
#define MACSLOT_INSTALL(x) \
    {						\
	extern void	macslot##x##_task();	\
\
	macslot_install(0x##x, macslot##x##_task);	\
    }

    MACSLOT_INSTALL(9);
    MACSLOT_INSTALL(A);
    MACSLOT_INSTALL(B);
    MACSLOT_INSTALL(C);
    MACSLOT_INSTALL(D);
    MACSLOT_INSTALL(E);
}
