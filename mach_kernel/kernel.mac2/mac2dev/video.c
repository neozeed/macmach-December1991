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
 * $Log:	video.c,v $
 * Revision 2.2  91/09/12  16:48:29  bohman
 * 	Created Mach 3.0 version from 2.5 version.
 * 	[91/09/11  16:08:10  bohman]
 * 
 * Revision 2.2  90/08/30  11:07:05  bohman
 * 	Created.
 * 	[90/08/29  12:54:57  bohman]
 * 
 */

/*
 *	Apple Macintosh II Mach (macmach)
 *
 *	File: mac2dev/video.c
 *	Author: David E. Bohman II (CMU macmach)
 */
#include <mach/mach_types.h>

#include <device/errno.h>

#include <vm/vm_kern.h>

#include <sys/types.h>
#include <sys/ioctl.h>

#undef NULL
#include <mac2os/Types.h>
#include <mac2os/Errors.h>
#include <mac2os/Slots.h>
#include <mac2os/Video.h>
#include <mac2os/ROMDefs.h>
#include <mac2os/Files.h>

#include <mac2/slots.h>

#include <mac2dev/video.h>

/*
 * Macintosh Video driver
 * for MACH 3.0
 */

/*
 * global screen structures
 * (indexed by minor dev)
 */
struct video	video[NVIDEO];
int nvideo;

vm_offset_t	rbvideo_kern;

video_open(dev)
dev_t				dev;
{
    register struct video	*vp;
    int				unit = minor(dev);

    if (unit >= NVIDEO)
	return (ENXIO);

    vp = &video[unit];
    if ((vp->video_flags&VIDEO_F_INITED) == 0)
	return (ENXIO);

    if ((vp->video_flags&VIDEO_F_OPEN) == 0)
	vp->video_flags |= VIDEO_F_OPEN;
    
    return (0);
}

video_close(dev)
dev_t				dev;
{
    register struct video	*vp;
    int				unit = minor(dev);

    vp = &video[unit];

    if (unit == 0) console_reset();
    else video_setup(vp);
    
    vp->video_flags &= ~(VIDEO_F_OPEN|VIDEO_F_STATUS);

    return (0);
}

video_mmap(dev, offset, prot)
dev_t				dev;
register vm_offset_t		offset;
{
    register struct video	*vp = &video[minor(dev)];
    
    if (slot_ptr_to_offset(vp->video_devbase+offset) >= (16*1024*1024))
	return (-1);
    
    return (mac2_btop(pmap_extract(kernel_pmap, vp->video_devbase + offset)));
}

#define VOID	0
#define IN	1
#define OUT	2	
#define Ctrl_Call(code, len, dir)	\
MACRO_BEGIN								\
    Ptr		p;							\
\
    if ((len) > 0) {							\
	p = (Ptr)NewPtr(len);						\
	if (!p) {							\
	    error = ENOMEM;				     		\
	    break;							\
	}								\
	*(Ptr *)params->csParam = p;					\
	if ((dir) & IN)							\
	    bcopy(data, p, (len));					\
    }									\
\
    params->ioCRefNum = vp->video_RefNum;				\
    params->csCode = (code);						\
\
    if (PBControl(params, FALSE) != noErr)				\
	error = EIO;							\
    else								\
    if ((dir) & OUT)							\
	bcopy(p, data, (len));						\
\
    if ((len) > 0)							\
	DisposPtr(p);							\
MACRO_END

#define Stat_Call(code, len, dir)	\
MACRO_BEGIN								\
    Ptr		p;							\
\
    if ((len) > 0) {							\
	p = (Ptr)NewPtr(len);						\
	if (!p) {							\
	    error = ENOMEM;						\
	    break;							\
	}								\
	*(Ptr *)params->csParam = p;					\
	if ((dir) & IN)							\
	    bcopy(data, p, (len));					\
    }									\
\
    params->ioCRefNum = vp->video_RefNum;				\
    params->csCode = (code);						\
\
    if (PBStatus(params, FALSE) != noErr)				\
	error = EIO;							\
    else								\
    if ((dir) & OUT)							\
	bcopy(p, data, (len));						\
\
    if ((len) > 0)							\
	DisposPtr(p);							\
MACRO_END
#define size_to_count(size) (((size) + 3) >> 2)

video_getstat(dev, flavor, data, count)
dev_t		dev;
int		flavor;
int *		data;
unsigned int *	count;
{
    register struct video	*vp = &video[minor(dev)];
    register			error = 0;
    CntrlParam			*params;

    params = (CntrlParam *)NewPtr(sizeof (CntrlParam));
    if (!params)
	return (ENOMEM);

    switch (flavor) {
      case VIDEO_CTRL_Init:
	{
	    register VDPgInfoPtr v = (VDPgInfoPtr)data;

	    /*
	     * Doing a cscReset seems
	     * to be a bad thing to do.
	     * With the ci builtin video
	     * it seems to do totally the
	     * wrong thing.
	     * This code simulates it
	     * according to the spec in
	     * the book
	     * "Designing Cards and Drivers
	     * for the Macintosh II and
	     * Macintosh SE".
	     */
	    video_setup(vp);

	    Stat_Call(cscGetMode, sizeof (*v), OUT);
	    *count = size_to_count(sizeof (*v));
	    v->csBaseAddr = (Ptr)slot_ptr_to_offset(v->csBaseAddr);
	}
	break;

      case VIDEO_STAT_GetMode:
      case VIDEO_CTRL_SetMode:
	{
	    register VDPgInfoPtr v = (VDPgInfoPtr)data;

	    Stat_Call(cscGetMode, sizeof (*v), OUT);
	    *count = size_to_count(sizeof (*v));
	    v->csBaseAddr = (Ptr)slot_ptr_to_offset(v->csBaseAddr);
	}
	break;

      case VIDEO_STAT_GetEntries:
	{
	    register VDEntRecPtr v = (VDEntRecPtr)data;
	    register Ptr p;
	    register size;

	    if ((vp->video_flags&VIDEO_F_STATUS) == 0) {
		error = EINVAL;
		break;
	    }
	    vp->video_flags &= ~VIDEO_F_STATUS;

	    bcopy(&vp->video_stat.ver, data, sizeof (VDEntryRecord));

	    size = (v->csCount + 1) * sizeof (ColorSpec);
	    p = (Ptr)NewPtr(size);
	    if (!p) {
		error = ENOMEM;
		break;
	    }

	    v->csTable = (ColorSpec *)p;
	    Stat_Call(cscGetEntries, sizeof (*v), IN);
	    if (!error) {
		bcopy(p, data, size);
		*count = size_to_count(size);
	    }

	    DisposPtr(p);
	}
	break;

      case VIDEO_STAT_GetPages:
	if ((vp->video_flags&VIDEO_F_STATUS) == 0) {
	    error = EINVAL;
	    break;
	}
	vp->video_flags &= ~VIDEO_F_STATUS;

	bcopy(&vp->video_stat.vpi, data, sizeof (VDPageInfo));

	*count = size_to_count(sizeof (VDPageInfo));
	Stat_Call(cscGetPages, sizeof (VDPageInfo), IN | OUT);
	break;

      case VIDEO_STAT_GetBaseAddr:
	{
	    register VDPgInfoPtr v = (VDPgInfoPtr)data;

	    if ((vp->video_flags&VIDEO_F_STATUS) == 0) {
		error = EINVAL;
		break;
	    }
	    vp->video_flags &= ~VIDEO_F_STATUS;

	    bcopy(&vp->video_stat.vpi, data, sizeof (VDPageInfo));

	    *count = size_to_count(sizeof (*v));
	    Stat_Call(cscGetBaseAddr, sizeof (*v), IN | OUT);
	    v->csBaseAddr = (Ptr)slot_ptr_to_offset(v->csBaseAddr);
	}
	break;

      case VIDEO_STAT_GetGray:
	*count = size_to_count(sizeof (VDPageInfo));
	Stat_Call(cscGetGray, sizeof (VDPageInfo), OUT);
	break;

	/*
	 * Return the size of the video
	 * device.  This is probably not
	 * useful because the value stored
	 * on most cards is incorrect.
	 */
      case VIDEO_SIZE:
	bcopy(&vp->video_devsize, data, sizeof (vm_size_t));
	*count = size_to_count(sizeof (vm_size_t));
	break;

	/*
	 * Return the video parameter block
	 * for the mode that the card is
	 * currently in.
	 */
      case VIDEO_PARAMS:
	{
	    VDPgInfoPtr	vpi = (VDPgInfoPtr)NewPtr(sizeof (VDPageInfo));

	    if (vpi) {
		params->ioCRefNum = vp->video_RefNum;
		params->csCode = cscGetMode;
		*(Ptr *)params->csParam = (Ptr)vpi;
		
		if (PBStatus(params, FALSE) != noErr)
		    error = EIO;
		else if (video_get_params(vp->video_slot,
					  vp->video_id,
					  vpi->csMode,
					  data) == FALSE)
		    error = EIO;
		else
		    *count = size_to_count(sizeof(VPBlock));
	    }
	    else
		error = ENOMEM;

	    if (vpi)
		DisposPtr(vpi);
	}
	break;

      default:
	error = EINVAL;
	break;
    }

    DisposPtr(params);

    return (error);
}

video_setstat(dev, flavor, data, count)
dev_t		dev;
int		flavor;
int *		data;
unsigned int	count;
{
    register struct video	*vp = &video[minor(dev)];
    register			error = 0;
    CntrlParam			*params;

    params = (CntrlParam *)NewPtr(sizeof (CntrlParam));
    if (!params)
	return (ENOMEM);

    switch (flavor) {
      case VIDEO_CTRL_KillIO:
	Ctrl_Call(cscKillIO, 0, VOID);
	break;

      case VIDEO_CTRL_SetMode:
	Ctrl_Call(cscSetMode, sizeof (VDPageInfo), IN);
	break;

      case VIDEO_CTRL_SetEntries:
	{
	    register VDEntRecPtr v = (VDEntRecPtr)data;
	    register Ptr p;
	    register size;

	    if (v->csStart == -1) {
		error = EINVAL;
		break;
	    }

	    size = (v->csCount + 1) * sizeof (ColorSpec);
	    if ((sizeof (*v) + size) > (count << 2)) {
		error = EINVAL;
		break;
	    }

	    p = (Ptr)NewPtr(size);
	    if (!p) {
		error = ENOMEM;
		break;
	    }
	    bcopy((Ptr)(v + 1), p, size);

	    v->csTable = (ColorSpec *)p;
	    Ctrl_Call(cscSetEntries, sizeof (*v), IN);

	    DisposPtr(p);
	}
	break;

      case VIDEO_CTRL_GrayScreen:
	Ctrl_Call(cscGrayPage, sizeof (VDPageInfo), IN);
	break;

      case VIDEO_CTRL_SetGray:
	Ctrl_Call(cscSetGray, sizeof (VDPageInfo), IN);
	break;

      case VIDEO_STAT_GetEntries:
	{
	    register VDEntRecPtr v = (VDEntRecPtr)data;
	    register size;

	    if (v->csStart == -1) {
		error = EINVAL;
		break;
	    }

	    size = sizeof (*v) + (v->csCount + 1) * sizeof (ColorSpec);
	    if (size_to_count(size) > DEV_STATUS_MAX) {
		error = EINVAL;
		break;
	    }

	    if ((vp->video_flags&VIDEO_F_STATUS) == 0
		&& (count << 2) >= sizeof (VDEntryRecord)) {
		bcopy(data, &vp->video_stat.ver, sizeof (VDEntryRecord));
		vp->video_flags |= VIDEO_F_STATUS;
	    }
	    else
		error = EINVAL;
	}
	break;

      case VIDEO_STAT_GetPages:
      case VIDEO_STAT_GetBaseAddr:
	if ((vp->video_flags&VIDEO_F_STATUS) == 0
	    && (count << 2) >= sizeof (VDEntryRecord)) {
	    bcopy(data, &vp->video_stat.vpi, sizeof (VDPageInfo));
	    vp->video_flags |= VIDEO_F_STATUS;
	}
	else
	    error = EINVAL;
	break;
	
      default:
	error = EINVAL;
	break;
    }

    DisposPtr(params);

    return (error);
}
#undef size_to_count
#undef Stat_Call
#undef Ctrl_Call
#undef OUT
#undef IN
#undef VOID

/*
 * Retrieve a video parameter
 * block.
 */
boolean_t
video_get_params(slot, id, mode, addr)
vm_offset_t	addr;
{
    boolean_t	result = TRUE;
    SpBlockPtr	sp = (SpBlockPtr)NewPtr(sizeof (SpBlock));

    if (!sp)
	result = FALSE;

    if (result) {
	sp->spSlot = slot;
	sp->spID = id;
	sp->spExtDev = 0;
	if (SRsrcInfo(sp) != noErr)
	    result = FALSE;
    }

    if (result) {
	sp->spID = mode;
	if (SFindStruct(sp) != noErr)
	    result = FALSE;
    }

    if (result) {
	sp->spID = mVidParams;
	if (SGetBlock(sp) != noErr)
	    result = FALSE;
	else {
	    bcopy(sp->spResult,
		  addr,
		  sizeof (VPBlock));

	    DisposPtr(sp->spResult);
	}
    }

    if (sp)
	DisposPtr(sp);

    return (result);
}

/*
 * Retrieve the device base and size.
 */
boolean_t
video_get_devbase_size(slot, id, vp)
register struct video	*vp;
{
    SpBlock		slot_params;

    slot_params.spSlot = slot;
    slot_params.spID = id;
    slot_params.spExtDev = 0;
    if (SFindDevBase(&slot_params) != noErr)
	return (FALSE);

    vp->video_devbase = slot_params.spResult;

    if (SRsrcInfo(&slot_params) != noErr)
	return (FALSE);

    if ((vp->video_devbase&0xf0000000) == 0xf0000000)
	slot_params.spID = minorLength;
    else
	slot_params.spID = majorLength;

    if (SReadLong(&slot_params) != noErr)
	return (FALSE);

    vp->video_devsize = slot_params.spResult;

    return (TRUE);
}

/*
 * Called from autoconfig.  The
 * console device has already been
 * initialized.
 */
boolean_t
video_config(slot, id)
register	slot, id;
{
    register	unit;
    VPBlock	v;

    if (video[0].video_slot == slot && video[0].video_id == id) {
	unit = 0;

	video_get_params(slot, id, oneBitMode, &v);
    }
    else {
	if (nvideo >= NVIDEO)
	    return (FALSE);

	unit = nvideo;

	if (video_get_params(slot, id, oneBitMode, &v) == FALSE)
	    return (FALSE);

	if (video_init(slot, id, &video[unit]) == FALSE)
	    return (FALSE);

	nvideo++;
    }
    printf("video sRsrc: slot %x id %d Bounds %d %d %d %d assigned to unit %d %s\n",
	   slot, id,
	   v.vpBounds.top, v.vpBounds.left,
	   v.vpBounds.bottom, v.vpBounds.right,
	   unit, unit? "": "(console)");

    return (TRUE);
}

/*
 * Initialize a video device.
 */
boolean_t
video_init(slot, id, vp)
register		slot, id;
register struct video	*vp;
{
    SpBlock		slot_params;

    if (video_get_devbase_size(slot, id, vp) == FALSE)
	return (FALSE);

    slot_params.spSlot = slot;
    slot_params.spID = id;
    slot_params.spExtDev = 0;

    if (SRsrcInfo(&slot_params) != noErr)
	return (FALSE);

    vp->video_RefNum = slot_params.spRefNum;

    vp->video_slot = slot;
    vp->video_id = id;
    
    if (video_setup(vp) == FALSE) {
	vp->video_flags = 0;
	return (FALSE);
    }
    vp->video_flags |= VIDEO_F_INITED;

    if (vp->video_slot >= SLOT_NUM_LOW
	&& vp->video_slot <= SLOT_NUM_HIGH)
	slot_to_slotdata_ptr(vp->video_slot)->SFlags |= SLOT_MAPPABLE;

    return (TRUE);
}

/*
 * Pick a video display device to be used as the system console.
 */
boolean_t
video_cons_find(v)
VPBlockPtr	*v;
{
    SpBlock	slot_params;
    struct {
	unsigned char	sdSlot;
	unsigned char	sdSResource;
    } vd;
    struct {
	unsigned short	len;
	unsigned short	addr;
    } arg;

    if (nvideo > 0)
	return (FALSE);

    /*
     * First try to locate the display
     * used for the 'Happy Macintosh'
     * under MacOS.
     */
    arg.addr = 0x80;
    arg.len = sizeof (vd);
    if (ReadXPRam(&vd, arg) == noErr && vd.sdSlot != 0) {
	slot_params.spSlot = vd.sdSlot;
	slot_params.spID = vd.sdSResource;
	slot_params.spExtDev = 0;
	if (SRsrcInfo(&slot_params) == noErr) {
	    if (slot_params.spCategory == catDisplay &&
		slot_params.spCType == typeVideo) {
		if (video_get_params(slot_params.spSlot,
				     slot_params.spID,
				     oneBitMode,
				     v) == TRUE &&
		    video_init(slot_params.spSlot,
			       slot_params.spID,
			       &video[0]) == TRUE) {
		    nvideo++;
		    return (TRUE);
		}
	    }
	}
    }

    /*
     * Otherwise find the display with
     * lowest slot and id number.
     */
    slot_params.spSlot = 0;
    slot_params.spID = slot_params.spExtDev = 0;
    if (SNextsRsrc(&slot_params) == noErr)
	for (;;) {
	    if (slot_params.spCategory == catDisplay &&
		slot_params.spCType == typeVideo) {
		if (video_get_params(slot_params.spSlot,
				     slot_params.spID,
				     oneBitMode,
				     v) == TRUE &&
		    video_init(slot_params.spSlot,
			       slot_params.spID,
			       &video[0]) == TRUE) {
		    nvideo++;
		    return (TRUE);
		}
	    }

	    if (SNextsRsrc(&slot_params) != noErr)
		return (FALSE);
	}
    else
	return (FALSE);
}

/*
 * Do MacOS driver calls necessary to
 * initialize a video device.  Called both
 * when initializing the system, and from
 * video_close().
 */
boolean_t
video_setup(vp)
register struct video	*vp;
{
    boolean_t		result = TRUE;
    CntrlParam		*params = (CntrlParam *)NewPtr(sizeof (CntrlParam));

    if (!params)
	result = FALSE;

    /*
     * Reset the device to
     * one-bit mode.  Also switch
     * to video page 0 and set the
     * screen to 50% gray.
     */
    if (result) {
	VDPgInfoPtr	vpi = (VDPgInfoPtr)NewPtr(sizeof (VDPageInfo));

	if (vpi) {
	    vpi->csMode = oneBitMode;
	    vpi->csPage = 0;
	    params->ioCRefNum = vp->video_RefNum;
	    params->csCode = cscSetMode;
	    *(Ptr *)params->csParam = (Ptr)vpi;

	    if (PBControl(params, FALSE) != noErr)
		result = FALSE;

	    if (result == TRUE) {
		vpi->csPage = 0;
		params->ioCRefNum = vp->video_RefNum;
		params->csCode = cscGrayPage;
		*(Ptr *)params->csParam = (Ptr)vpi;

		if (PBControl(params, FALSE) != noErr)
		    result = FALSE;
	    }
	}
	else
	    result = FALSE;

	if (vpi)
	    DisposPtr(vpi);
    }

    /*
     * Load the CLUT with black & white
     * pixel values.
     * The invocation code is different
     * here; the SetEntries call is
     * allowed to return an error for
     * devices that have a read-only CLUT.
     */
    if (result == TRUE) {
	VDEntRecPtr	ver =
	    (VDEntRecPtr)NewPtr(sizeof (VDEntryRecord));
	ColorSpec	*color =
	    (ColorSpec *)NewPtr(2 * sizeof (ColorSpec));

	if (ver && color) {
	    color[0].value = 0;
	    color[0].rgb.red = color[0].rgb.green = color[0].rgb.blue = 0xffff;

	    color[1].value = 1;
	    color[1].rgb.red = color[1].rgb.green = color[1].rgb.blue = 0x0000;

	    ver->csTable = color;
	    ver->csStart = 0;
	    ver->csCount = (2 - 1);

	    params->ioCRefNum = vp->video_RefNum;
	    params->csCode = cscSetEntries;
	    *(Ptr *)params->csParam = (Ptr)ver;

	    (void) PBControl(params, FALSE);
	}
	else
	    result = FALSE;

	if (ver)
	    DisposPtr(ver);

	if (color)
	    DisposPtr(color);
    }

    if (params)
	DisposPtr(params);

    return (result);
}
