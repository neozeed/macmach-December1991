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
 * $Log:	conf.c,v $
 * Revision 2.2  91/09/12  16:39:29  bohman
 * 	Created.
 * 	[91/09/11  14:26:30  bohman]
 * 
 */

/*
 *	Apple Macintosh II Mach (macmach)
 *
 *	File: mac2/conf.c
 */

#include <device/conf.h>

extern int	block_io_mmap();

extern int	sd_open(), sd_close(), sd_read(), sd_write();
extern int	sd_getstat();
#define sd_name		"sd"

extern int	rd_open(), rd_close(), rd_read(), rd_write();
#define rd_name		"rd"

extern int	adb_open(), adb_close(), adb_read();
extern int	adb_getstat(), adb_setstat();
#define adb_name	"adb"

extern int	console_open(), console_close(), console_read();
extern int	console_write(), console_getstat(), console_setstat();
extern int	console_portdeath();
#define console_name	"console"

extern int	timeopen(), timeclose(), timeread(), timemmap();
#define timename	"time"

extern int	mhdopen(), mhdread(), mhdwrite();
#define mhdname		"hd"

extern int	enopen(), enoutput();
extern int	ensetinput(), engetstat(), ensetstat();
#define enname		"en"

extern int	video_open(), video_close(), video_mmap();
extern int	video_getstat(), video_setstat();
#define video_name	"video"

extern int	sony_open(), sony_close(), sony_rw();
extern int	sony_getstat(), sony_setstat();
#define sony_name	"sony"

extern int	kernmmap(), kerngetstat(), kernsetstat();
#define kernname	"kern"

extern int	serial_open(), serial_close(), serial_read(), serial_write();
extern int	serial_getstat(), serial_setstat(), serial_portdeath();
#define serial_name	"serial"

/*
 * List of devices - console must be at slot 0
 */
struct dev_ops	dev_name_list[] =
{
    /*
        name,		open,		close,		read,
        write,		getstat,	setstat,	mmap,
        async_in,	reset,		port_death,	subdev,
	dev_info,
    */
    {
	console_name,	console_open,	console_close,	console_read,
	console_write,	console_getstat,console_setstat, 0,
	nodev,		nulldev,	console_portdeath, 0,
	nodev,
    },
    {
	adb_name,	adb_open,	adb_close,	adb_read,
	nodev,		adb_getstat,	adb_setstat,	0,
	nodev,		nulldev,	nulldev,	1,
	nodev,
    },
    {
	sd_name,	sd_open,	sd_close,	sd_read,
 	sd_write,	sd_getstat,	nulldev,	block_io_mmap,
	nodev,		nulldev,	nulldev,	0x20000000 /* XXX */,
	nodev,
    },
    {
	rd_name,	rd_open,	rd_close,	rd_read,
	rd_write,	nulldev,	nulldev,	block_io_mmap,
	nodev,		nulldev,	nulldev,	0,
	nodev,
    },
    {
	timename,	timeopen,	timeclose,	timeread,
	nulldev,	nulldev,	nulldev,	timemmap,
	nodev,		nulldev,	nulldev,	0,
	nodev,
    },
    {
	mhdname,	mhdopen,	nulldev,	mhdread,
	mhdwrite,	nulldev,	nulldev,	block_io_mmap,
	nodev,		nulldev,	nulldev,	32,
	nodev,
    },
    {
	enname,		enopen,		nulldev,	nodev,
	enoutput,	engetstat,	ensetstat,	0,
	ensetinput,	nulldev,	nulldev,	0,
	nodev,
    },
    {
	video_name,	video_open,	video_close,	nodev,
	nodev,		video_getstat,	video_setstat,	video_mmap,
	nodev,		nulldev,	nulldev,	1,
	nodev,
    },
    {
	sony_name,	sony_open,	sony_close,	sony_rw,
	sony_rw,	sony_getstat,	sony_setstat,	block_io_mmap,
	nodev,		nulldev,	nulldev,	1,
	nodev,
    },
    {
	kernname,	nulldev,	nulldev,	nodev,
	nodev,		kerngetstat,	kernsetstat,	kernmmap,
	nodev,		nulldev,	nulldev,	0,
	nodev,
    },
    {
	serial_name,	serial_open,	serial_close,	serial_read,
	serial_write,	serial_getstat,	serial_setstat,	nodev,
	nodev,		nulldev,	serial_portdeath,	0,
	nodev,
    },
};

int	dev_name_count =
    sizeof (dev_name_list) / sizeof (dev_name_list[0]);

/*
 * Indirect list.
 */
struct dev_indirect dev_indirect_list[] = {
    /* console */
    {
	"console",	&dev_name_list[0],		0,
    },
};

int	dev_indirect_count =
    sizeof (dev_indirect_list) / sizeof (dev_indirect_list[0]);
