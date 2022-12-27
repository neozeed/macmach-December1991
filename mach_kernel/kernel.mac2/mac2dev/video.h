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
 * $Log:	video.h,v $
 * Revision 2.2  91/09/12  16:48:40  bohman
 * 	Created Mach 3.0 version from 2.5 version.
 * 	[91/09/11  16:09:38  bohman]
 * 
 * Revision 2.2  90/08/30  11:07:17  bohman
 * 	Created.
 * 	[90/08/29  12:55:22  bohman]
 * 
 */

/*
 *	Apple Macintosh II Mach (macmach)
 *
 *	File: mac2dev/video.h
 *	Author: David E. Bohman II (CMU macmach)
 */

#ifndef	_MAC2DEV_VIDEO_H_
#define	_MAC2DEV_VIDEO_H_	1

#ifdef KERNEL
struct video {
    short		video_RefNum;
    unsigned short	video_slot;
    unsigned short	video_id;
    vm_offset_t		video_devbase;	/* device base address */
    vm_size_t		video_devsize;	/* device size */
    unsigned long	video_flags;
#define VIDEO_F_INITED		0x00000001
#define VIDEO_F_OPEN		0x00000002
#define VIDEO_F_STATUS		0x00000004
    union {
	VDPageInfo	vpi;
	VDEntryRecord	ver;
    } video_stat;
};
#endif KERNEL

/*
 * Video Ioctl calls.
 * See "Designing Cards and
 * Drivers for Macintosh II
 * and Macintosh SE"
 * for info.
 */

/*
 * MAC driver control calls
 */
#define VIDEO_CTRL_Init		_IOR('V', 0, VDPageInfo)
#define VIDEO_CTRL_KillIO	_IO('V', 1)
#define VIDEO_CTRL_SetMode	_IOWR('V', 2, VDPageInfo)
#define VIDEO_CTRL_SetEntries	_IOW('V', 3, VDEntryRecord)
#define VIDEO_CTRL_SetGamma	_IOW('V', 4, Ptr)
#define VIDEO_CTRL_GrayScreen	_IOW('V', 5, VDPageInfo)
#define VIDEO_CTRL_SetGray	_IOW('V', 6, VDPageInfo)

/*
 * MAC driver status calls
 */
#define VIDEO_STAT_GetMode	_IOR('V', 7, VDPageInfo)
#define VIDEO_STAT_GetEntries	_IOWR('V', 8, VDEntryRecord)
#define VIDEO_STAT_GetPages	_IOWR('V', 9, VDPageInfo)
#define VIDEO_STAT_GetBaseAddr	_IOWR('V', 10, VDPageInfo)
#define VIDEO_STAT_GetGray	_IOR('V', 11, VDPageInfo)

/*
 * Return size of a video device.
 */
#define VIDEO_SIZE		_IOR('V', 20, vm_size_t)

/*
 * Return video parameters based on
 * current device mode.
 */
#define VIDEO_PARAMS		_IOR('V', 22, VPBlock)

#ifdef KERNEL
extern struct video video[];
int nvideo;

#define	NVIDEO	    6
#endif /* KERNEL */

#endif	_MAC2DEV_VIDEO_H_
