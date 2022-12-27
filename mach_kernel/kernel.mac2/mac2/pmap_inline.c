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
 * $Log:	pmap_inline.c,v $
 * Revision 2.2  91/09/12  16:42:50  bohman
 * 	Created Mach 3.0 version from 2.5 version.
 * 	[91/09/11  14:58:28  bohman]
 * 
 * Revision 2.2  90/08/30  11:02:37  bohman
 * 	Created.
 * 	[90/08/29  11:43:33  bohman]
 * 
 */

/*
 *	Apple Macintosh II Mach (macmach)
 *
 *	File: mac2/pmap_inline.c
 *	Author: David E. Bohman II (CMU macmach)
 */

#ifndef	_MACHINE_PMAP_INLINE_C_
#define	_MACHINE_PMAP_INLINE_C_

static inline
unsigned long
phys_to_pfn(phys)
struct {
    phys_offset_t     pfn:19,
			 :13;
} phys;
{
    return (phys.pfn);
}

static inline
phys_offset_t
pfn_to_phys(pfn)
unsigned long	pfn;
{
    union {
	phys_offset_t		full_offset;
	struct {
	    phys_offset_t	pfn:19,
				   :13;
	} f;
    } phys;

    phys.full_offset = 0;
    phys.f.pfn = pfn;

    return (phys.full_offset);
}    

static inline
unsigned long
vm_offset_to_index(v)
vm_offset_fields_t	v;
{
    return (v.pg_index);
}

static inline
vm_offset_t
index_to_vm_offset(i)
unsigned long 		i;
{
    vm_offset_fields_t	v;

    v.full_offset = 0;
    v.pg_index = i;

    return (v.full_offset);
}

/*
 * Address Translation tree walking
 * primatives (inline expansions).
 *
 * NB: these primatives DO NOT check
 * for zero pointers or invalid ptp's or pp's.
 */

/*
 * Given a ptr (virtual) to a page-table-pointer table and
 * an offset, return a ptr (virtual) to the corresponding
 * page-table-pointer.
 */
static inline
ptp_t	*
ptptbl_to_ptp(p, v)
ptp_t			p[];
vm_offset_fields_t	v;
{
    return (&p[v.ptptbl_index]);
}

/*
 * Given a ptr (virtual) to a page-pointer table and
 * an offset, return a ptr (virtual) to the corresponding
 * page-pointer.
 */
static inline
pp_t	*
pptbl_to_pp(p, v)
pp_t			p[];
vm_offset_fields_t	v;
{
    return (&p[v.pptbl_index]);
}

/*
 * Given a ptr (virtual) to a ptp, return the corresponding
 * virtual offset.
 */
static inline
vm_offset_t
ptp_to_va(p)
ptp_t			*p;
{
    union {
	vm_offset_t	full_offset;
	struct {
	    vm_offset_t	     :22,
			index:8,
			     :2;
	} t;
    } t;
    vm_offset_fields_t	v;

    t.full_offset = (vm_offset_t)p;

    v.full_offset = 0;
    v.ptptbl_index = t.t.index;

    return (v.full_offset);
}

#endif	_MACHINE_PMAP_INLINE_C_
