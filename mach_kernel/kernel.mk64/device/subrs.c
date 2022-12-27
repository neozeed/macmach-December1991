/* 
 * Mach Operating System
 * Copyright (c) 1991,1990,1989,1988 Carnegie Mellon University
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
 * $Log:	subrs.c,v $
 * Revision 2.11  91/09/12  16:37:39  bohman
 * 	Added strlen() (BSD version).
 * 	[91/09/11  17:07:25  bohman]
 * 
 * Revision 2.10  91/08/24  11:56:04  af
 * 	Use BSD string functions now, which of course come with a
 * 	copyright.  Also, undef them in case some smarty..
 * 	[91/08/22            af]
 * 
 * Revision 2.9  91/07/31  17:34:27  dbg
 * 	Add strcpy.
 * 	[91/07/30  16:47:37  dbg]
 * 
 * Revision 2.8  91/05/14  16:01:30  mrt
 * 	Correcting copyright
 * 
 * Revision 2.7  91/03/16  14:43:34  rpd
 * 	Updated for new kmem_alloc interface.
 * 	[91/03/03            rpd]
 * 
 * Revision 2.6  91/02/05  17:10:17  mrt
 * 	Changed to new Mach copyright
 * 	[91/01/31  17:30:39  mrt]
 * 
 * Revision 2.5  91/01/08  15:10:02  rpd
 * 	Added continuation argument to thread_block.
 * 	[90/12/08            rpd]
 * 
 * Revision 2.4  90/05/03  15:19:27  dbg
 * 	Add compatibility routines for BSD-compatible device drivers:
 * 	sleep, wakeup, geteblk, brelse.
 * 	[90/03/14            dbg]
 * 
 * Revision 2.3  90/01/11  11:42:31  dbg
 * 	De-linted.
 * 	[89/12/15            dbg]
 * 
 * 	Add lock initialization in if_init_queues.
 * 	[89/11/30            dbg]
 * 
 * Revision 2.2  89/11/29  14:08:58  af
 * 	RCS-ed, added strncmp (needed for scsi label comparisons).
 * 	[89/10/28            af]
 * 
 */
/*
 * Copyright (c) 1988 Regents of the University of California.
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions
 * are met:
 * 1. Redistributions of source code must retain the above copyright
 *    notice, this list of conditions and the following disclaimer.
 * 2. Redistributions in binary form must reproduce the above copyright
 *    notice, this list of conditions and the following disclaimer in the
 *    documentation and/or other materials provided with the distribution.
 * 3. All advertising materials mentioning features or use of this software
 *    must display the following acknowledgement:
 *	This product includes software developed by the University of
 *	California, Berkeley and its contributors.
 * 4. Neither the name of the University nor the names of its contributors
 *    may be used to endorse or promote products derived from this software
 *    without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE REGENTS AND CONTRIBUTORS ``AS IS'' AND
 * ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
 * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
 * ARE DISCLAIMED.  IN NO EVENT SHALL THE REGENTS OR CONTRIBUTORS BE LIABLE
 * FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
 * DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS
 * OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION)
 * HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT
 * LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY
 * OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF
 * SUCH DAMAGE.
 */
/*
 * Random device subroutines and stubs.
 */

#include <vm/vm_kern.h>
#include <device/buf.h>
#include <device/if_hdr.h>
#include <device/if_ether.h>

/*
 * Print out disk name and block number for hard disk errors.
 */
harderr(bp, cp)
	struct buf *bp;
	char *	cp;
{
	printf("%s%d%c: hard error sn%d ",
	       cp,
	       minor(bp->b_dev) >> 3,
	       'a' + (minor(bp->b_dev) & 0x7),
	       bp->b_blkno);
}

/* String routines, from BSD */
#ifdef	strcpy
#undef strcmp
#undef strncmp
#undef strcpy
#undef strncpy
#endif

int
strcmp(s1, s2)
	register char *s1, *s2;
{
	while (*s1 == *s2++)
		if (*s1++ == 0)
			return (0);
	return (*(unsigned char *)s1 - *(unsigned char *)--s2);
}

int
strncmp(s1, s2, n)
	register char *s1, *s2;
	register unsigned int n;
{

	if (n == 0)
		return (0);
	do {
		if (*s1 != *s2++)
			return (*(unsigned char *)s1 - *(unsigned char *)--s2);
		if (*s1++ == 0)
			break;
	} while (--n != 0);
	return (0);
}

char *
strcpy(to, from)
	register char *to;
	register char *from;
{
	char *save = to;

	for (; *to = *from; ++from, ++to);
	return(save);
}

char *
strncpy(dst, src, n)
	char *dst;
	char *src;
	register unsigned int n;
{
	if (n != 0) {
		register char *d = dst;
		register char *s = src;

		do {
			if ((*d++ = *s++) == 0) {
				/* NUL pad the remaining n-1 bytes */
				while (--n != 0)
					*d++ = 0;
				break;
			}
		} while (--n != 0);
	}
	return (dst);
}

/*
 * Returns the number of
 * non-NULL bytes in string argument.
 */

strlen(s)
register char *s;
{
	register n;

	n = 0;
	while (*s++)
		n++;
	return(n);
}

/*
 * Ethernet support routines.
 */
u_char	etherbroadcastaddr[6] = { 0xff, 0xff, 0xff, 0xff, 0xff, 0xff };

/*
 * Convert Ethernet address to printable (loggable) representation.
 */
char *
ether_sprintf(ap)
	register u_char *ap;
{
	register i;
	static char etherbuf[18];
	register char *cp = etherbuf;
	static char digits[] = "0123456789abcdef";

	for (i = 0; i < 6; i++) {
		*cp++ = digits[*ap >> 4];
		*cp++ = digits[*ap++ & 0xf];
		*cp++ = ':';
	}
	*--cp = 0;
	return (etherbuf);
}

/*
 * Initialize send and receive queues on an interface.
 */
if_init_queues(ifp)
	register struct ifnet *ifp;
{
	IFQ_INIT(&ifp->if_snd);
	queue_init(&ifp->if_rcv_port_list);
	simple_lock_init(&ifp->if_rcv_port_list_lock);
}


/*
 * Compatibility with BSD device drivers.
 */
sleep(channel, priority)
	char *	channel;
	int	priority;
{
	assert_wait((int)channel, FALSE);	/* not interruptible XXX */
	thread_block((void (*)()) 0);
}

wakeup(channel)
	char *	channel;
{
	thread_wakeup((int)channel);
}

struct buf *
geteblk(size)
	int	size;
{
	register io_req_t	ior;

	io_req_alloc(ior, 0);
	ior->io_device = (device_t)0;
	ior->io_unit = 0;
	ior->io_op = 0;
	ior->io_mode = 0;
	ior->io_recnum = 0;
	ior->io_count = size;
	ior->io_residual = 0;
	ior->io_error = 0;

	size = round_page(size);
	ior->io_alloc_size = size;
	if (kmem_alloc(kernel_map, &ior->io_data, size) != KERN_SUCCESS)
	    panic("geteblk");

	return (ior);
}

brelse(bp)
	struct buf *bp;
{
	register io_req_t	ior = bp;

	(void) vm_deallocate(kernel_map,
			(vm_offset_t) ior->io_data,
			(vm_size_t) ior->io_alloc_size);
	io_req_free(ior);
}
