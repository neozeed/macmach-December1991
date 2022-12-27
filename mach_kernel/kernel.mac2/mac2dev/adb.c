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
 * $Log:	adb.c,v $
 * Revision 2.2  91/09/12  16:45:38  bohman
 * 	Created.
 * 	[91/09/11  15:23:32  bohman]
 * 
 */

/*
 *	Apple Macintosh II Mach (macmach)
 *
 *	File: mac2dev/adb.c
 *	Author: David E. Bohman II (CMU macmach)
 */

#include <mach/mach_types.h>

#include <device/device_types.h>
#include <device/io_req.h>
#include <device/cirbuf.h>

#include <kern/queue.h>

#include <mac2os/Types.h>
#include <mac2os/DeskBus.h>

#include <mac2dev/adb.h>

/*
 * Macintosh ADB driver
 * for MACH 3.0
 */

typedef struct adb {
    unsigned char	oaddr;		/* original ADB address */
    unsigned char	type;		/* ADB device type */
    queue_head_t	pending;	/* queue of pending io requests */
    struct cirbuf	cbuf;		/* input character buffer */
#define ADB_CBUF_SIZE	1024
    int			flags;
#define	ADB_EXISTS	0x01
#define ADB_OPEN	0x02
#define ADB_XOPEN	0x04
    void		(*Xinput)();	/* backwards compatability for console
					 * driver */
} *adb_t;

static struct adb	adb_info[16];

typedef struct {
    unsigned		:28,
		    addr:4;
} *adb_id_t;

void	adb_dequeue();

io_return_t
adb_open(number, mode, ior)
int		number;
dev_mode_t	mode;
io_req_t	ior;
{
    register adb_id_t	dev = (adb_id_t)&number;
    register adb_t	a;

    if (dev->addr == 0)
	return (D_NO_SUCH_DEVICE);

    a = &adb_info[dev->addr];
    if (!(a->flags & ADB_EXISTS))
	return (D_NO_SUCH_DEVICE);

    cb_alloc(&a->cbuf, ADB_CBUF_SIZE);
    a->flags |= ADB_OPEN;

    return (D_SUCCESS);
}

io_return_t
adb_close(number)
int	number;
{
    register adb_id_t	dev = (adb_id_t)&number;
    register adb_t	a;

    a = &adb_info[dev->addr];

    if (a->flags & ADB_OPEN) {
      a->flags &= ~ADB_OPEN;
      cb_free(&a->cbuf);
    }

    return (D_SUCCESS);
}

io_return_t
adb_read(number, ior)
int			number;
register io_req_t	ior;
{
    register adb_id_t		dev = (adb_id_t)&number;
    register kern_return_t	result;
    register adb_t		a;
    register			s;
    boolean_t			adb_read_done();

    a = &adb_info[dev->addr];

    if (ior->io_mode & D_NOWAIT) {
	s = spl1();

	if (a->cbuf.c_cc <= 0) {
	    (void) splx(s);
	    return (D_WOULD_BLOCK);
	}

	(void) splx(s);
    }

    result = device_read_alloc(ior, ior->io_count);
    if (result != KERN_SUCCESS)
	return (result);

    s = spl1();

    if (ior->io_mode & D_NOWAIT) {
	if (a->cbuf.c_cc <= 0) {
	    (void) splx(s);
	    return (D_WOULD_BLOCK);
	}
    }

    if (!queue_empty(&a->pending) || a->cbuf.c_cc <= 0) {
	ior->io_done = adb_read_done;
	enqueue_tail(&a->pending, (queue_entry_t)ior);
	(void) splx(s);
	return (D_IO_QUEUED);
    }

    if (ior->io_count > 0)
	adb_dequeue(a, ior);

    (void) splx(s);

    return (D_SUCCESS);
}

boolean_t
adb_read_done(ior)
register io_req_t	ior;
{
    register adb_id_t	dev = (adb_id_t)&ior->io_unit;
    register adb_t	a;
    int			s;

    a = &adb_info[dev->addr];

    s = spl1();

    if (a->cbuf.c_cc <= 0) {
	enqueue_tail(&a->pending, (queue_entry_t)ior);
	(void) splx(s);
	return (FALSE);
    }

    if (ior->io_count > 0)
	adb_dequeue(a, ior);

    (void) splx(s);

    return (ds_read_done(ior));
}

/*
 * Special 'quick' versions of
 * getc() and putc().
 */
#define qgetc(c, q) \
MACRO_BEGIN				\
    (c) = *(q)->c_cf; (q)->c_cc--;	\
    if (++(q)->c_cf == (q)->c_end)	\
	(q)->c_cf = (q)->c_start;	\
MACRO_END

#define qputc(c, q) \
MACRO_BEGIN				\
    *(q)->c_cl = (c); (q)->c_cc++;	\
    if (++(q)->c_cl == (q)->c_end)	\
	(q)->c_cl = (q)->c_start;	\
MACRO_END

void
adb_dequeue(a, ior)
register adb_t		a;
register io_req_t	ior;
{
    register struct cirbuf	*cb = &a->cbuf;
    register			i, n;

    if (ior->io_count > cb->c_cc)
	n = cb->c_cc;
    else
	n = ior->io_count;

    for (i = 0; i < n; i++)
	qgetc(*(ior->io_data + i), cb);

    ior->io_residual = ior->io_count - n;
}

#define cb_size(cb)	((cb)->c_end - (cb)->c_start)

void
adb_enqueue(cmd, data)
adb_cmd_t	cmd;
unsigned char	data[];
{
    adb_t			a = &adb_info[cmd.reg.addr];
    register struct cirbuf	*cb = &a->cbuf;
    register io_req_t		ior;

    if ((sizeof (cmd.cmd) + cmd.cmd.len)
	<= (cb_size(cb) - cb->c_cc)) {
	register		i;
	
	qputc(cmd.cmd.cmd, cb);
	qputc(cmd.cmd.len, cb);
	for (i = 0; i < cmd.cmd.len; i++)
	    qputc(*(data + i), cb);
    }

    while (ior = (io_req_t)dequeue_head(&a->pending))
	iodone(ior);
}

io_return_t
adb_getstat(number, flavor, data, count)
int		number;
register	flavor;
dev_status_t	data;
unsigned	*count;
{
    register adb_id_t	dev = (adb_id_t)&number;
    register adb_t	a;

    a = &adb_info[dev->addr];

    switch (flavor) {
      case ADB_INFO:
	{
	    register adb_info_t		*info = (adb_info_t *)data;

	    info->addr = a->oaddr;
	    info->type = a->type;

	    *count = ADB_INFO_COUNT;
	}
	break;

      default:
	return (D_INVALID_OPERATION);
    }

    return (D_SUCCESS);
}

io_return_t
adb_setstat(number, flavor, data, count)
int		number;
register	flavor;
dev_status_t	data;
unsigned	count;
{
    register adb_id_t	dev = (adb_id_t)&number;
    register adb_t	a;

    a = &adb_info[dev->addr];

    switch (flavor) {
      case ADB_FLUSH:
	{
	    register	s = spl1();

	    ndflush(&a->cbuf, a->cbuf.c_cc);

	    (void) splx(s);
	}
	break;

      default:
	return (D_INVALID_OPERATION);
    }

    return (D_SUCCESS);
}

adb_setup()
{
    register short	i, n = CountADBs();
    register		addr;
    ADBDataBlock	*db;
    ADBSetInfoBlock	*sib;
    extern void		adb_service();

    db = (ADBDataBlock *)NewPtr(sizeof (ADBDataBlock));
    sib = (ADBSetInfoBlock *)NewPtr(sizeof (ADBSetInfoBlock));

    if (db && sib) for (i = 1; i <= n; i++) {
	addr = GetIndADB(db, i);
	if (addr >= 0) {
	    adb_t	a = &adb_info[addr];
	    
	    a->oaddr = db->origADBAddr;
	    a->type = db->devType;
	    queue_init(&a->pending);

	    sib->siServiceRtPtr = adb_service;
	    sib->siDataAreaAddr = 0;
	    SetADBInfo(sib, addr);

	    a->flags = ADB_EXISTS;
	}
    }

    if (db)
	DisposPtr(db);

    if (sib)
	DisposPtr(sib);
}

void
adb_input(cmd, data)
adb_cmd_t	cmd;
unsigned char	data[];
{
    register adb_t		a;
#ifdef MODE24
    register unsigned char	mode;
#endif

    if (cmd.gen.cmd == ADB_reset || cmd.gen.cmd == ADB_flush)
	return;

    a = &adb_info[cmd.reg.addr];
    if (a->flags & ADB_OPEN) {
#ifdef MODE24
	mode = SwapMMUMode(TRUE);
#endif /* MODE24 */
	(void) adb_enqueue(cmd, data);
#ifdef MODE24
	(void) SwapMMUMode(mode);
#endif /* MODE24 */
    }
    else if (a->flags & ADB_XOPEN) {
#ifdef MODE24
	mode = SwapMMUMode(TRUE);
#endif /* MODE24 */
	(*a->Xinput)(cmd, data);
#ifdef MODE24
	(void) SwapMMUMode(mode);
#endif /* MODE24 */
    }
}

/*
 * Backwards compatability for
 * console driver.
 */
Xadb_query(oaddr, type, n)
register	oaddr, type, n;
{
    register adb_t	a;
    register		addr;

    for (addr = 0; addr < 16; addr++) {
	a = &adb_info[addr];
	if (a->oaddr == oaddr
	    && (type == -1 || a->type == type)
	    && (a->flags & ADB_EXISTS)
	    && n-- == 0)
	    return (addr);
    }

    return (-1);
}

Xadb_type(addr)
register	addr;
{
    register adb_t	a;

    if (addr >= 0 && addr < 16) {
	a = &adb_info[addr];
	if (a->flags & ADB_EXISTS)
	    return (a->type);
    }

    return (-1);
}

boolean_t
Xadb_open(addr, input)
register	addr;
void		(*input)();
{
    register adb_t	a;

    if (addr >= 0 && addr < 16) {
	a = &adb_info[addr];
	if ((a->flags & (ADB_EXISTS | ADB_XOPEN)) == ADB_EXISTS) {
	    a->Xinput = input;
	    a->flags |= ADB_XOPEN;

	    return (TRUE);
	}
    }

    return (FALSE);
}

void
Xadb_close(addr)
register	addr;
{
    register adb_t	a;

    if (addr >= 0 && addr < 16) {
	a = &adb_info[addr];
	a->flags &= ~ADB_XOPEN;
    }
}
