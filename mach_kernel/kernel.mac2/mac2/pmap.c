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
 * $Log:	pmap.c,v $
 * Revision 2.2  91/09/12  16:42:25  bohman
 * 	Created Mach 3.0 version from 2.5 version.
 * 	[91/09/11  14:56:49  bohman]
 * 
 * Revision 2.2  90/09/04  17:28:14  bohman
 * 	Created.
 * 
 */

/*
 *	Apple Macintosh II Mach (macmach)
 *
 *	File: mac2/pmap.c
 *	Author: David E. Bohman II (CMU macmach)
 */

/*
 * Manages physical address maps.
 *
 * Since the information managed by this module is
 * also stored by the logical address mapping module,
 * this module may throw away valid virtual-to-physical
 * mappings at almost any time.  However, invalidations
 * of virtual-to-physical mappings must be done as
 * requested.
 *
 * In order to cope with hardware architectures which
 * make virtual-to-physical map invalidates expensive,
 * this module may delay invalidate or reduced protection
 * operations until such time as they are actually
 * necessary.
 */

#include <cpus.h>

#include <mach/mach_types.h>

#include <kern/zalloc.h>
#include <kern/lock.h>

#include <mach/vm_param.h>
#include <mach/vm_prot.h>

#include <vm/pmap.h>
#include <vm/vm_map.h>
#include <vm/vm_kern.h>
#include <vm/vm_page.h>
#include <vm/vm_user.h>

#include <mac2/pmap_kmap.h>

#include <mac2os/Globals.h>

extern vm_offset_t	avail_start, avail_end;
extern vm_offset_t	virtual_avail, virtual_end;

static vm_offset_t	avail_next;
static unsigned		avail_npages;

/*
 * Private data structures.
 */

/*
 * For each managed physical page, there is a list of all currently
 * valid virtual mappings of that page.
 */
typedef struct pv_entry {
    struct pv_entry *	next;		/* next pv_entry */
    pmap_t		pmap;		/* pmap where mapping lies */
    vm_offset_t		va;		/* virtual address for mapping */
} pv_head_t, *pv_entry_t;

#define	PV_ENTRY_NULL	((pv_entry_t) 0)

zone_t		pv_entry_zone;		/* zone of pv_entry structures */

/*
 * First and last physical addresses that we maintain any information
 * for.  Initialized to zero so that pmap operations done before
 * pmap_init won't touch any non-existent structures.
 */
vm_offset_t	vm_first_phys = (vm_offset_t) 0;
vm_offset_t	vm_last_phys  = (vm_offset_t) 0;

/*
 * Page descriptor -
 * Maintained for each managed page.
 */
typedef struct {
    unsigned long	pfn;		/* physical pfn of page */
    vm_offset_t		kern;		/* offset in kernel pmap */
    pv_head_t		pv_list;	/* head of pv list for page */
    unsigned char	page_attrib;	/* page attributes: */
#define	PG_MOD	    0x01		/* page is modified */
#define	PG_REF	    0x02		/* page has been referenced */
} pg_desc_t, *pg_desc_ptr_t;

#define PG_DESC_NULL	((pg_desc_ptr_t)0)

pg_desc_t	*pg_desc_tbl;

#define pg_desc(pa) \
    (((phys_offset_t)(pa) >= BANKB_PHYS)?			\
     &bankB.pg_desc_tbl[phys_to_pfn((phys_offset_t)(pa) -	\
				    bankB.pg_first_phys)]:	\
     &bankA.pg_desc_tbl[phys_to_pfn((phys_offset_t)(pa) -	\
				    bankA.pg_first_phys)])

#define pg_desc_pvh(pd)		(&(pd)->pv_list)

vm_offset_t	pg_first_kern, pg_last_kern;

/*
 * Page table descriptor -
 * Maintained for each managed page table.
 */
typedef struct {
    queue_chain_t	link;		/* link in active or free queues */
    unsigned long	pfn;		/* physical pfn of page */
    vm_offset_t		kern;		/* offset in kernel pmap */
    pmap_t		pmap;		/* pmap this page is allocated to */
    vm_offset_t		va;		/* offset into pmap mapped */
    int			use_count;	/* number of mappings in use */
    int			wired_count;	/* number of mappings wired in */
} pt_desc_t, *pt_desc_ptr_t;

#define	PT_DESC_NULL	((pt_desc_ptr_t)0)

pt_desc_t	*pt_desc_tbl;

queue_head_t	pt_page_queue_active;	/* list of all active pt pages */
queue_head_t	pt_page_queue_free;	/* list of all free pt pages */

simple_lock_data_t
		pt_page_queue_lock;	/* both queues are locked either
					   by this lock, or by a write-
					   lock on the pmap system lock. */

vm_offset_t	pt_first_kern, pt_last_kern;

/*
 * Count of pt pages allocated at
 * boot.  Can be patched.
 */
int		pt_page_alloc_count = 30;

/*
 * pt_desc manipulation macros.
 */

/*
 * Given a ptr (virtual) into a page table,
 * return a ptr to the corresponding
 * pt_desc.
 */
#define pt_kern_to_pt_desc(p) \
    (&pt_desc_tbl[vm_offset_to_index((vm_offset_t)(p) - pt_first_kern)])

/*
 * Given the pfn of a page table,
 * return a ptr to the corresponding
 * pt_desc.
 */
#define pfn_to_pt_desc(pfn) \
    (((unsigned long)(pfn) >= BANKB_PFN)?				\
     &bankB.pt_desc_tbl[(unsigned long)(pfn) - bankB.pt_first_pfn]:	\
     &bankA.pt_desc_tbl[(unsigned long)(pfn) - bankA.pt_first_pfn])

/*
 * Return the size of memory
 * mapped by one page-pointer table.
 */
#define	pt_page_map_size	(mac2_ptob(MAC2_PGBYTES/sizeof(pp_t)))

/*
 * Kernel Page table descriptor -
 */
typedef struct {
    vm_offset_t		kern;		/* offset in kernel pmap */
} kern_pt_desc_t, *kern_pt_desc_ptr_t;

#define KERN_PT_DESC_NULL	((kern_pt_desc_ptr_t)0)

kern_pt_desc_t		kern_pt_desc_tbl[256];

vm_offset_t	kern_pt_first_kern;

#define pfn_to_kern_pt_desc(pfn) \
    (((unsigned long)(pfn) >= BANKB_PFN)?		\
     &bankB.kern_pt_desc_tbl[(unsigned long)(pfn) -	\
			     bankB.kern_pt_first_pfn]:	\
     &bankA.kern_pt_desc_tbl[(unsigned long)(pfn) -	\
			     bankA.kern_pt_first_pfn])

/*
 * Bank descriptor -
 */
typedef struct {
    pg_desc_t		*pg_desc_tbl;
    phys_offset_t	pg_first_phys, pg_last_phys;
    pt_desc_t		*pt_desc_tbl;
    unsigned long	pt_first_pfn;
    kern_pt_desc_t	*kern_pt_desc_tbl;
    unsigned long	kern_pt_first_pfn;
} bank_t, *bank_ptr_t;

bank_t		bankA, bankB;
#define	BANKB_PHYS	0x04000000
#define BANKB_PFN	0x2000

/*
 * Page table pointer tables are allocated
 * in non-pageable kernel virtual memory.
 * Each ptptbl is 1024 bytes, so 8 of them fit
 * in a page.
 */
typedef struct pt_root_page {
    queue_chain_t	link;		/* link in free queue */
    vm_offset_t		page;		/* ptr to base of page */
    int			free_count;	/* number of free ptptbls in page */
    unsigned char	free_map;	/* bitmap of free ptptbls in page */
} pt_root_desc_t, *pt_root_desc_ptr_t;

#define PT_ROOT_DESC_NULL	((pt_root_desc_ptr_t)0)

zone_t	pt_root_desc_zone;

/*
 * list of above pages that contain
 * at least one free ptptbl
 */
queue_head_t		pt_root_page_queue_free;

simple_lock_data_t	pt_root_page_lock;	/* lock for above */

/*
 * Inline expansions for special
 * PMMU functions
 */
#include <machine/pmmu_inline.c>

/*
 * Character bitmap allocation routines (inline expansions).
 *
 * The bit offsets are numbered as follows:
 *
 *	7 6 5 4 3 2 1 0
 *	M	      L
 *	S	      S
 *	B	      B
 */
#include <machine/bitmap_inline.c>

/*
 * Address translation tree.
 *
 * An address translation tree contains 2 levels.
 * The top level is called the page-table-pointer table (ptptbl).
 * It consists of short format table descriptors, and is indexed
 * by the top 8 bits of the virtual address (256 entries).
 * Each of these page-table-pointers (ptp) can point to a
 * page-pointer table (pptbl).  Each pptbl consists of short format
 * page descriptors and is indexed by the next lower 11 bits of
 * of the virtual address (2048 entries).  Each pptbl consists
 * of 8192 bytes (a machine dependent memory page), and is
 * allocated as a pt_page by the pmap module.  Each of these
 * page-pointers (pp) can point to a physical memory page, which
 * is indexed by the lower 13 bits of the virtual address.
 *
 * The kernel has it's own address translation tree which is
 * defined by the kernel_pmap, and pointed to by the PMMU
 * supervisor root pointer (SRP).  Thus the address space
 * for the kernel is 2^32 bytes.
 * 
 * Each task has it's own address translation tree which is
 * defined by it's pmap, and pointed to by the PMMU
 * cpu root pointer (CRP) while a thread owned by the task is executing.
 * The address space for each task is also 2^32 bytes.
 */

/*
 * Address Translation tree walking
 * primatives (inline expansions).
 *
 * NB: these primatives DO NOT check
 * for zero pointers or invalid ptp's or pp's.
 */
#include <machine/pmap_inline.c>

/*
 *	Statistics
 */
int	pt_page_active_count	= 0;
int	pt_page_free_count	= 0;
int	pt_page_wired_count	= 0;
int	pt_page_max_active	= 0;
int	pt_page_max_wired	= 0;
int	pt_root_page_alloc_count= 0;
int	pt_root_page_free_count	= 0;

/*
 *	Locking Protocols:
 *
 *	There are two structures in the pmap module that need locking:
 *	the pmaps themselves, and the per-page pv_lists (which are locked
 *	by locking the pv_lock_table entry that corresponds to the pv_head
 *	for the list in question.)  Most routines want to lock a pmap and
 *	then do operations in it that require pv_list locking -- however
 *	pmap_remove_all and pmap_copy_on_write operate on a physical page
 *	basis and want to do the locking in the reverse order, i.e. lock
 *	a pv_list and then go through all the pmaps referenced by that list.
 *	To protect against deadlock between these two cases, the pmap_lock
 *	is used.  There are three different locking protocols as a result:
 *
 *  1.  pmap operations only (pmap_extract, pmap_access, ...)  Lock only
 *		the pmap.
 *
 *  2.  pmap-based operations (pmap_enter, pmap_remove, ...)  Get a read
 *		lock on the pmap_lock (shared read), then lock the pmap
 *		and finally the pv_lists as needed [i.e. pmap lock before
 *		pv_list lock.]
 *
 *  3.  pv_list-based operations (pmap_remove_all, pmap_copy_on_write, ...)
 *		Get a write lock on the pmap_lock (exclusive write); this
 *		also guaranteees exclusive access to the pv_lists.  Lock the
 *		pmaps as needed.
 *
 *	At no time may any routine hold more than one pmap lock or more than
 *	one pv_list lock.  Because interrupt level routines can allocate
 *	mbufs and cause pmap_enter's, the pmap_lock and the lock on the
 *	kernel_pmap can only be held at splvm.
 */

#define	SPLVM(spl)	{ spl = splvm(); }
#define	SPLX(spl)	{ splx(spl); }

#define	PMAP_READ_LOCK(pmap, spl)	SPLVM(spl)
#define	PMAP_WRITE_LOCK(spl)		SPLVM(spl)
#define	PMAP_READ_UNLOCK(pmap, spl)	SPLX(spl)
#define	PMAP_WRITE_UNLOCK(spl)		SPLX(spl)
#define	PMAP_WRITE_TO_READ_LOCK(pmap)

#define	LOCK_PVH(index)
#define	UNLOCK_PVH(index)

/*
 * CPU Cache managment.
 */
#define PMAP_FLUSH_CPU_CACHES(pmap)			\
{							\
    if ((pmap) == kernel_pmap || (pmap)->cpus_using)	\
	flush_cpu_caches();				\
}

/*
 * Address Translation Cache (ATC) managment.
 */

extern is68030;

/*
 * Flush ATC entries corresponding to the
 * given pmap and address range (start inclusive, end exclusive).
 */
#ifdef MODE24
#define	PMAP_UPDATE_ATC(pmap, s, e)			\
{							\
    if ((pmap) == kernel_pmap) {			\
        ATC_INVALIDATE_SUPR_RANGE((s), (e));		\
    }							\
    else if ((pmap)->cpus_using) {			\
	if ((pmap)->alt_root == 0) {			\
	    ATC_INVALIDATE_USER_RANGE((s), (e));	\
	}						\
	else {						\
	    if ((pmap)->flags&PMAP_F_24BIT) {		\
		ATC_INVALIDATE_USER_ALL();		\
		ATC_INVALIDATE_MAP((pmap)->root);	\
	    }						\
	    else {					\
		ATC_INVALIDATE_USER_RANGE((s), (e));	\
		ATC_INVALIDATE_MAP((pmap)->alt_root);	\
	    }						\
	}						\
    }							\
    else {						\
        ATC_INVALIDATE_MAP((pmap)->root);		\
	if ((pmap)->alt_root)				\
	    ATC_INVALIDATE_MAP((pmap)->alt_root);	\
    }							\
}
#else /* MODE24 */
#define	PMAP_UPDATE_ATC(pmap, s, e)			\
{							\
    if ((pmap) == kernel_pmap) {			\
        ATC_INVALIDATE_SUPR_RANGE((s), (e));		\
    }							\
    else if ((pmap)->cpus_using) {			\
	ATC_INVALIDATE_USER_RANGE((s), (e));		\
    }							\
    else {						\
        ATC_INVALIDATE_MAP((pmap)->root);		\
    }							\
}
#endif /* MODE24 */

/*
 * This constant defines the maximum number
 * of entries that will be flushed without
 * flushing all of the supervisor or current user
 * entries from the ATC.
 */
#define	ATC_PAGE_INVAL_MAX 8

/*
 * Invalidate the given range of (global) kernel virtual
 * addresses from the ATC.
 */
#define	ATC_INVALIDATE_SUPR_RANGE(s, e)				\
{								\
    if ((s) < (e) &&						\
	(e) - (s) < ATC_PAGE_INVAL_MAX*MAC2_PGBYTES) {		\
	register vm_offset_t _va;				\
\
	if (is68030) {						\
	    for (_va = (s); _va < (e); _va += MAC2_PGBYTES)     \
	        pmmu_flush_supr(_va);				\
	}							\
	else {							\
	    for (_va = (s); _va < (e); _va += MAC2_PGBYTES)     \
	        pmmu_flush_supr_shared(_va);			\
	}							\
    }								\
    else {						        \
	if (is68030)						\
	    pmmu_flush_supr_all();				\
	else							\
	    pmmu_flush_supr_shared_all();			\
    }								\
}

/*
 * Invalidate the given range of (current) user virtual
 * addresses from the ATC.  Used when the range
 * is in the address translation tree currently
 * pointed at by the CRP.
 */
#define	ATC_INVALIDATE_USER_RANGE(s, e)				\
{								\
    if ((s) < (e) &&						\
	(e) - (s) < ATC_PAGE_INVAL_MAX*MAC2_PGBYTES) {		\
	register vm_offset_t _va;				\
\
        for (_va = (s); _va < (e); _va += MAC2_PGBYTES)		\
	    pmmu_flush_user(_va);				\
    }								\
    else {							\
	pmmu_flush_user_all();					\
    }								\
}

/*
 * Unconditionally invalidate all (current) user virtual
 * addresses from the ATC.
 */
#define ATC_INVALIDATE_USER_ALL()	\
    pmmu_flush_user_all();

/*
 * Invalidate all of the ATC entries for
 * the tree which is rooted at physical
 * address r.  Used when the address
 * translation tree is not the active
 * one pointed at by the CRP.
 */
#define	ATC_INVALIDATE_MAP(r)	    \
if (!is68030) {			    \
    PMMU_RP_reg _rp;		    \
				    \
    _rp.limit = PMMU_RP_LIMIT;	    \
    _rp.valid = PMMU_VALID_RP;	    \
    _rp.phys = (r);		    \
    pmmu_flush_map(&_rp);	    \
}

/*
 * Translate a MACH vm_prot_t into
 * PMMU write-protect value.
 */
#define	pmmu_prot(prot)		(((prot)&VM_PROT_WRITE)?PMMU_RW:PMMU_WP)

/*
 * The kernel pmap is statically
 * allocated.
 */
static struct pmap	kernel_pmap_store;
pmap_t			kernel_pmap;
pt_root_desc_t		kernel_pt_root_desc;

#ifdef MODE24
/*
 * A special translation tree
 * is setup for 24 bit kernel
 * compatability.
 */
typedef struct compat_pmap {
    phys_offset_t	root;
    struct pt_root_page *
			ptrt;
    pp_t		*
			pptbl;
} *compat_pmap_t;

static struct compat_pmap	kernel_compat_pmap_store;
compat_pmap_t			kernel_compat_pmap;
#endif /* MODE24 */

/*
 * Zone used to allocate
 * pmap structures.
 */
zone_t		pmap_zone;

/*
 * Page-table-pointer tables
 * exist 1-to-1 with pmaps and
 * are created at pmap creation time.
 */
#define		pmap_ptptbl_size	(256*sizeof(ptp_t))

/*
 * Number of page-pointers required to
 * map one machine indepedent page.
 * Calculated at initialization.
 */
int		pps_per_vm_page;

/*
 * Has pmap_init completed?
 */
boolean_t	pmap_initialized = FALSE;

/*
 * Amount of space reserved in the kernel pmap
 * to be used for allocating kernel virtual memory
 * once the system is up.
 */
vm_size_t   kernel_alloc_space = (64*1024*1024);    /* 64 Meg */

static inline
ptp_t *
root_to_ptptbl(page, root)
vm_offset_t	page;
struct {
    phys_offset_t	:19,
		   index:3,
			:10;
} root;
{
    union {
	vm_offset_t		full_offset;
	struct {
	    vm_offset_t		:19,
	  		   index:3,
		       		:10;
	} f;
    } v;

    v.full_offset = page;
    v.f.index = root.index;

    return ((ptp_t *)v.full_offset);
}

/*
 * Given a ptr (virtual) to a page-table-pointer, return
 * a ptr (virtual) to the corresponding page-pointer table.
 */
static inline
pp_t	*
ptp_to_pptbl(p, pmap)
ptp_t			*p;
pmap_t			pmap;
{
    if (pmap == kernel_pmap)
	return ((pp_t *)pfn_to_kern_pt_desc(p->pfn)->kern);
    else
	return ((pp_t *)pfn_to_pt_desc(p->pfn)->kern);
}

/*
 * Given a ptr (virtual) to a page-table-pointer and
 * an offset, return a ptr (virtual) to the corresponding
 * page-pointer.
 */
static inline
pp_t *
ptp_to_pp(p, pmap, v)
ptp_t			*p;
pmap_t			pmap;
vm_offset_fields_t	v;
{
    return (pptbl_to_pp(ptp_to_pptbl(p, pmap), v));
}

/*
 * Given an offset and a pmap, compute the address of the
 * pp.  If the address is invalid with respect to the pmap
 * then PP_NULL is returned.
 */
inline
pp_t *
pmap_pp(pmap, va)
register pmap_t	    	pmap;
register vm_offset_t    va;
{
    register ptp_t	    *ptp;

    ptp = ptptbl_to_ptp(root_to_ptptbl(pmap->ptrt->page,
				       pmap->root),
			va);
    if (ptp->valid != PMMU_VALID_PTP)
	return (PP_NULL);

    return (ptp_to_pp(ptp, pmap, va));
}

/*
 * Given an offset and a pmap, compute the address of the
 * ptp.
 *
 * NB: This routine may return the address of a pp if
 * an early termination page descriptor exists at this
 * offset in the ptptbl.
 */
inline
ptp_t *
/* or perhaps pp_t * */
pmap_ptp(pmap, va)
register pmap_t		pmap;
register vm_offset_t	va;
{
    return (ptptbl_to_ptp(root_to_ptptbl(pmap->ptrt->page,
					 pmap->root),
			  va));
}

static inline
phys_offset_t
pmap_phys(pmap, va)
pmap_t				pmap;
register vm_offset_fields_t	va;
{
    register pp_t		*pp;
    union {
	phys_offset_t		full_offset;
	struct {
	    phys_offset_t	pfn:19,
			     offset:13;
	} f;
    } pa;

    if ((pp = pmap_pp(pmap, va.full_offset)) == PP_NULL) {
	pp = (pp_t *)pmap_ptp(pmap, va.full_offset);
	if (pp->valid == PMMU_VALID_PP) {
	    pa.full_offset = va.ptptbl_offset;
	    pa.f.pfn += pp->pfn;
	}
	else
	    pa.full_offset = (phys_offset_t)0;
    }
    else {
	pa.f.pfn = pp->pfn;
	pa.f.offset = va.pg_offset;
    }

    return (pa.full_offset);
}

/*
 * External form of the PMAP_UPDATE_ATC() macro.
 */
void
pmap_update_atc(pmap, start, end)
register pmap_t pmap;
register vm_offset_t start, end;
{
    PMAP_UPDATE_ATC(pmap, start, end);
}

/*
 * Define and initialize data structure
 * used to decode page access based on
 * MACH vm_prot_t (really permission).
 */
static vm_prot_t pmmu_prot_map[8];

static
pmmu_prot_map_init()
{
    register comb;

    for (comb = 0; comb < 8; comb++)
	switch (comb) {
	  case VM_PROT_NONE:
	  case VM_PROT_EXECUTE:
	  case VM_PROT_WRITE:
	  case VM_PROT_WRITE | VM_PROT_EXECUTE:
	    pmmu_prot_map[comb] = VM_PROT_NONE;
	    break;

	  case VM_PROT_READ:
	  case VM_PROT_READ | VM_PROT_EXECUTE:
	    pmmu_prot_map[comb] = VM_PROT_READ;
	    break;

	  case VM_PROT_READ | VM_PROT_WRITE:
	  case VM_PROT_READ | VM_PROT_WRITE | VM_PROT_EXECUTE:
	    pmmu_prot_map[comb] = VM_PROT_READ | VM_PROT_WRITE;
	    break;
	}
}

#define roundup(x, y)	((((x) + ((y) - 1)) / (y)) * (y))

/*
 * Map memory in kernel pmap at initialization.
 * The physical addresses being mapped are not managed
 * and are never unmapped.  Page tables must already have
 * been allocated in the kernel_pmap.
 */
static
vm_offset_t
pmap_map(va, phys, size, prot)
register vm_offset_t	va;
register vm_offset_t	phys;
register vm_size_t	size;
register int		prot;
{
#define each		MAC2_PGBYTES

    size = roundup(size, each);

    size /= each;
    while (size-- > 0) {
	pmap_enter(kernel_pmap, va, phys, prot, FALSE);
	pmap_resident_count(kernel_pmap)--; /* XXX */
	va += each;
	phys += each;
    }

    return (va);
#undef each
}

static phys_offset_t	Log2Phys();

/*
 * Allocate page tables in the kernel_pmap from
 * physical memory starting at *aphys_free, according to
 * the indicated offset and size.
 */
static
void
pmap_alloc_kernel_pt(aphys_free, va, size)
vm_offset_t		*aphys_free;
vm_offset_t		va;
register vm_size_t	size;
{
    ptp_t		*ptp;
    vm_offset_t		a;
    phys_offset_t	phys;
    kern_pt_desc_ptr_t	kp;
#define each		pt_page_map_size

    ptp = pmap_ptp(kernel_pmap, va);

    size = roundup(size, each);

    size /= each; size += 1;
    while (size-- > 0) {
	if (ptp->valid == PMMU_INVALID) {
	    a = *aphys_free;
	    *aphys_free += MAC2_PGBYTES;
	    if (kern_pt_first_kern == 0)
		kern_pt_first_kern = a;
	    bzero(a, MAC2_PGBYTES);
	    kp = &kern_pt_desc_tbl[vm_offset_to_index(a - kern_pt_first_kern)];
	    phys = Log2Phys(a);
	    if (phys >= BANKB_PHYS) {
		if (bankB.kern_pt_desc_tbl == KERN_PT_DESC_NULL) {
		    bankB.kern_pt_desc_tbl = kp;
		    bankB.kern_pt_first_pfn = phys_to_pfn(phys);
		}
	    } else {
		if (bankA.kern_pt_desc_tbl == KERN_PT_DESC_NULL) {
		    bankA.kern_pt_desc_tbl = kp;
		    bankA.kern_pt_first_pfn = phys_to_pfn(phys);
		}
	    }
	    kp->kern = a;
	    ptp->pfn = phys_to_pfn(phys); 
	    ptp->valid = PMMU_VALID_PTP;
	}
	ptp++;
    }
#undef each
}

/*
 * Map memory in kernel pmap using early
 * termination pp entries in the ptptbl.
 * Each mapping is worth 16MB.
 */
static
void
pmap_map_trunc(va, phys, size, prot)
register vm_offset_t	va;
register vm_offset_t	phys;
register vm_size_t	size;
register int		prot;
{
    register pp_t	*pp;
    pp_t		template;
    int			mprot;
#define each		pt_page_map_size

    prot = pmmu_prot_map[prot];
    mprot = pmmu_prot(prot);

    pp = (pp_t *)pmap_ptp(kernel_pmap, va);

    *((unsigned long *)&template) = 0;
    template.prot = mprot;
    template.valid = PMMU_VALID_PP;

    size = roundup(size, each);

    size /= each;
    while (size-- > 0) {
	template.pfn = phys_to_pfn(phys);
	template.ci = (phys >= PMAP_IO_ADDR);
	*pp++ = template;
	phys += each;
    }
#undef each
}

#ifdef MODE24
/*
 * Setup mappings in the kernel
 * 24 bit compatability translation tree.
 */
static
void
pmap_map_compat(va, phys, size, prot)
register vm_offset_t	va;
register vm_offset_t	phys;
register vm_size_t	size;
register int		prot;
{
    register pp_t	*pp;
    pp_t		template;
    int			mprot;
#define each		MAC2_PGBYTES

    prot = pmmu_prot_map[prot];
    mprot = pmmu_prot(prot);

    pp = pptbl_to_pp(kernel_compat_pmap->pptbl, va);

    *((unsigned long *)&template) = 0;
    template.prot = mprot;
    template.valid = PMMU_VALID_PP;

    size = roundup(size, each);
    
    size /= each;
    while (size-- > 0) {
	template.pfn = phys_to_pfn(phys);
	template.ci = (phys >= PMAP_IO_ADDR);
	*pp++ = template;
	phys += each;
    }
#undef each
}
#endif /* MODE24 */

#ifdef MODE24
/*
 * Allocate the one page table
 * used for the kernel compatability
 * translation tree.
 */
static
void
pmap_alloc_kernel_compat_pt(aphys_free)
vm_offset_t	*aphys_free;
{
    register ptp_t	*ptp, *eptp;
    ptp_t		template;
    vm_offset_t		a;
    phys_offset_t	phys;

    ptp = root_to_ptptbl(kernel_compat_pmap->ptrt->page,
			 kernel_compat_pmap->root);
    if (ptp->valid == PMMU_VALID_PTP)
	return;
    a = *aphys_free;
    *aphys_free += MAC2_PGBYTES;
    bzero(a, MAC2_PGBYTES);
    kernel_compat_pmap->pptbl = (pp_t *)a;
    phys = Log2Phys(a);
    ptp->pfn = phys_to_pfn(phys);
    ptp->valid = PMMU_VALID_PTP;

    template = *ptp;
    eptp = ptp+(pmap_ptptbl_size/sizeof (ptp_t));
    for (; ptp < eptp;)
	*ptp++ = template;
}
#endif /* MODE24 */

#ifdef MODE24
/*
 * Change mapping protections in the
 * kernel 24 bit compatability tree.
 */
static
void
pmap_protect_compat(start, end, prot)
register vm_offset_t	start;
register vm_offset_t	end;
register int		prot;
{
    register pp_t	*pp;
    register vm_size_t	size;
    register		mprot;
#define each		MAC2_PGBYTES

    prot = pmmu_prot_map[prot];
    mprot = pmmu_prot(prot);

    pp = pptbl_to_pp(kernel_compat_pmap->pptbl, start);

    size = end - start;

    size /= each;
    while (size-- > 0) {
	pp->prot = mprot;
	pp++;
    }
#undef each
}
#endif /* MODE24 */

typedef struct {
    unsigned char	TypeMMU;
    unsigned char	MMFlags;
    long		Phys2Log;
    unsigned long	TotalRAM;
    unsigned long	PhysicalTableOff;
    unsigned long	AllocOff;
    unsigned long	TopOff;
    unsigned long	WarmStart;
    struct {
	unsigned long	base;
	unsigned long	length;
    } RamTable[0];
} BootGlobs_t;

#include <machine/pmmu_log2phys.c>

/*
 * Setup the kernel pmap.
 */
static
boolean_t
pmap_setup(aphys_free, aphys_last, avirt_free, avirt_last)
vm_offset_t	*aphys_free;
vm_offset_t	*aphys_last;
vm_offset_t	*avirt_free;
vm_offset_t	*avirt_last;
{
    BootGlobs_t *bg;
    struct PhysicalSpace {
	unsigned short	numEntries;
	unsigned short	entrySize;
	struct PhysicalSpaceBlock {
	    unsigned short	usage;
#define kUsageRAM		1
#define kUsageROM		2
#define kUsageIO		3
#define kUsageMinorSlot		4
#define kUsageMajorSlot		5
#define kUsageLowMemory		6
#define kUsageBootGlobals	7
#define kUsageOnBoardVideo	8
#define kUsageOnBoardVideoWrap	9
#define kUsageOther		0
	    unsigned short	attributes;
#define kCacheableBit		1
	    struct AddressRange {
		unsigned long	address;
		unsigned long	rangeSize;
	    } physical, logical, logical24;
	} firstEntry[0];
    } *phys_tbl;
    register struct PhysicalSpaceBlock	*bp;
    register 				i;
    vm_offset_t				a, t, askip;
#ifdef MODE24
    vm_size_t				s, compat_mem_size;
#else /* MODE24 */
    vm_size_t				s;
#endif /* MODE24 */
    extern int				start, etext;
    extern vm_offset_t			rbvideo_kern;
#ifdef MODE24
    extern boolean_t			mac32bit;
#endif /* MODE24 */

    if (BootGlobPtr == 0xffffffff)
	return (FALSE);

    bg = (BootGlobs_t *)(BootGlobPtr - sizeof (BootGlobs_t));
    phys_tbl = (struct PhysicalSpace *)(BootGlobPtr + bg->PhysicalTableOff);

    kernel_pmap->root = Log2Phys(kernel_pmap->ptrt->page);
#ifdef MODE24
    kernel_pmap->alt_root = kernel_pmap->root + pmap_ptptbl_size;
#endif /* MODE24 */

#ifdef MODE24
    kernel_compat_pmap->root = kernel_pmap->root + (2 * pmap_ptptbl_size);
#endif /* MODE24 */

#ifdef MODE24
    pmap_alloc_kernel_compat_pt(aphys_free);
#endif /* MODE24 */

#define	pmap_map_kernel(va, pa, size, prot)	\
    {								\
	pmap_alloc_kernel_pt(aphys_free, (va), (size));		\
	(va) = pmap_map((va), (pa), (size), (prot));		\
    }

#define pmap_map_kernel_special(va, pa, size, prot)	\
    {								\
	if ((size) >= pt_page_map_size				\
	    && ((va) % pt_page_map_size) == 0			\
	    && ((size) % pt_page_map_size) == 0) {		\
	    pmap_map_trunc((va), (pa), (size), (prot));		\
	}							\
	else {							\
	    pmap_alloc_kernel_pt(aphys_free, (va), (size));	\
	    (void) pmap_map((va), (pa), (size), (prot));	\
	}							\
    }

#ifdef MODE24
#define pmap_map_compat_kernel(va, pa, size, prot)	\
    {								\
	pmap_map_compat((va), (pa), (size), (prot));		\
    }
#endif /* MODE24 */

    a = VM_MIN_KERNEL_ADDRESS;

#ifdef MODE24
    if (!mac32bit) {
	/*
	 * Running with 24 bit mac memory
	 * manager.
	 */
	if (mem_size > 0x800000)
	    compat_mem_size = 0x800000;
	else
	    compat_mem_size = mem_size;
    }
#endif /* MODE24 */

    for (i = 0; i < phys_tbl->numEntries; i++) {
	bp = &phys_tbl->firstEntry[i];
	switch (bp->usage) {
	  case kUsageLowMemory:
	  case kUsageRAM:
#ifdef MODE24
	    if (!mac32bit && a < compat_mem_size) {
		s = round_page(bp->physical.rangeSize);
		if ((a + s) > compat_mem_size)
		    s -= (a + s - compat_mem_size);
		pmap_map_compat_kernel(a,
				       bp->physical.address,
				       s,
				       VM_PROT_READ|VM_PROT_WRITE);
	    }
#endif /* MODE24 */
	    pmap_map_kernel(a,
			    bp->physical.address,
			    round_page(bp->physical.rangeSize),
			    VM_PROT_READ|VM_PROT_WRITE);
	    break;

	  case kUsageBootGlobals:
	    if (bp->physical.rangeSize >= MAC2_PGBYTES) {
#ifdef MODE24
		if (!mac32bit && a < compat_mem_size) {
		    s = round_page(bp->physical.rangeSize);
		    if ((a + s) > compat_mem_size)
			s -= (a + s - compat_mem_size);
		    pmap_map_compat_kernel(a,
					   trunc_page(bp->physical.address),
					   s,
					   VM_PROT_READ|VM_PROT_WRITE);
		}
#endif /* MODE24 */
		pmap_map_kernel(a,
				trunc_page(bp->physical.address),
				round_page(bp->physical.rangeSize),
				VM_PROT_READ|VM_PROT_WRITE);
	    }
	    break;

	  case kUsageOnBoardVideo:
	    rbvideo_kern = round_page(bp->logical.address);
	  case kUsageOnBoardVideoWrap:
	    t = round_page(bp->logical.address);
	    pmap_map_kernel(t,
			    bp->physical.address,
			    round_page(bp->logical.rangeSize),
			    VM_PROT_READ|VM_PROT_WRITE);

#ifdef MODE24
	    if (!mac32bit) {
		t = round_page(bp->logical24.address);
		pmap_map_compat_kernel(t,
				       bp->physical.address,
				       round_page(bp->logical24.rangeSize),
				       VM_PROT_READ|VM_PROT_WRITE);
	    }
#endif /* MODE24 */
	    break;

	  case kUsageROM:
	    t = bp->logical.address;
	    pmap_map_kernel_special(t,
				    bp->physical.address,
				    bp->logical.rangeSize,
				    VM_PROT_READ);

#ifdef MODE24
	    if (!mac32bit) {
		t = bp->logical24.address;
		pmap_map_compat_kernel(t,
				       bp->physical.address,
				       bp->logical24.rangeSize,
				       VM_PROT_READ);
	    }
#endif /* MODE24 */
	    break;

	  case kUsageIO:
	  case kUsageMinorSlot:
	    t = bp->logical.address;
	    pmap_map_kernel_special(t,
				    bp->physical.address,
				    bp->logical.rangeSize,
				    VM_PROT_READ|VM_PROT_WRITE);

#ifdef MODE24
	    if (!mac32bit) {
		t = bp->logical24.address;
		pmap_map_compat_kernel(t,
				       bp->physical.address,
				       bp->logical24.rangeSize,
				       VM_PROT_READ|VM_PROT_WRITE);
	    }
#endif /* MODE24 */
	    break;
	}
    }

#undef pmap_map_kernel
#undef pmap_map_kernel_special
#ifdef MODE24
#undef pmap_map_compat_kernel
#endif /* MODE24 */

    /*
     * Write protect the kernel
     * text.
     */
    pmap_protect(kernel_pmap,
		 &start,
		 round_page(&etext),
		 VM_PROT_READ);

#ifdef MODE24
    if (!mac32bit)
	pmap_protect_compat(&start,
			    round_page(&etext),
			    VM_PROT_READ);
#endif /* MODE24 */

    /*
     * Allocate enough extra kernel_pmap
     * space to map an extra kernel_alloc_space bytes.
     */
    pmap_alloc_kernel_pt(aphys_free, a, kernel_alloc_space);

    *avirt_free = a;
    *avirt_last = a + kernel_alloc_space;

#ifdef MODE24
#undef	pmap_map_kernel_compat
#endif /* MODE24 */

    /*
     *	Allocate some amount of physical memory to use for
     *	user page tables.  The amount allocated is set at
     *  compile time in the kernel data segment, and can
     *  be patched in the kernel a.out file.
     */
    pt_first_kern = *aphys_free;
    *aphys_free += round_page(mac2_ptob(pt_page_alloc_count));
    pt_last_kern = *aphys_free;

    bzero(pt_first_kern, pt_last_kern - pt_first_kern);

    /*
     * Allocate the pt descriptor table, and the
     * pt page queues.
     */
    (vm_offset_t) pt_desc_tbl = *aphys_free;
    *aphys_free += round_page(sizeof (pt_desc_t) * pt_page_alloc_count);

    bzero(pt_desc_tbl, *aphys_free - (vm_offset_t)pt_desc_tbl);

    simple_lock_init(&pt_page_queue_lock);
    queue_init(&pt_page_queue_free);
    queue_init(&pt_page_queue_active);
    {
	register pt_desc_ptr_t 	ptpg;
	vm_offset_t		a = pt_first_kern;
	phys_offset_t		phys;

	for (ptpg = pt_desc_tbl; a < pt_last_kern; ptpg++, a += MAC2_PGBYTES) {
	    enqueue_tail(&pt_page_queue_free, (queue_entry_t) ptpg);
	    ptpg->kern = a;
	    phys = Log2Phys(a);
	    ptpg->pfn = phys_to_pfn(phys);
	    if (phys >= BANKB_PHYS) {
		if (bankB.pt_desc_tbl == PT_DESC_NULL) {
		    bankB.pt_desc_tbl = ptpg;
		    bankB.pt_first_pfn = phys_to_pfn(phys);
		}
	    } else {
		if (bankA.pt_desc_tbl == PT_DESC_NULL) {
		    bankA.pt_desc_tbl = ptpg;
		    bankA.pt_first_pfn = phys_to_pfn(phys);
		}
	    }
	}
    }
    pt_page_free_count = pt_page_alloc_count;

    pg_first_kern = *aphys_free;
    pg_last_kern = *aphys_last;

#ifdef MODE24
    if (!mac32bit && (mem_size > compat_mem_size)) {
	askip = pg_last_kern;
	*aphys_last = pg_last_kern = mem_size;
    }
#endif /* MODE24 */

    /*
     * Allocate the page descriptor table.
     */
    (vm_offset_t) pg_desc_tbl = *aphys_free;
    *aphys_free += round_page(sizeof (pg_desc_t) *
			      atop(pg_last_kern - pg_first_kern));

    bzero(pg_desc_tbl, *aphys_free - (vm_offset_t)pg_desc_tbl);

    pg_first_kern = *aphys_free;

    {
	register pg_desc_ptr_t	pd;
	vm_offset_t		a = pg_first_kern;
	phys_offset_t		phys, last_phys;
	boolean_t		A_done = FALSE, B_done = FALSE;

	for (pd = pg_desc_tbl; a < pg_last_kern; a += MAC2_PGBYTES, pd++) {
#ifdef MODE24
	    if (mac32bit || (mem_size == compat_mem_size))
		pd->kern = a;
	    else {
		if (a < compat_mem_size) {
		    if (a >= askip)
			;
		    else
			pd->kern = a;
		}
		else
		    pd->kern = a;
	    }
#else /* MODE24 */
	    pd->kern = a;
#endif /* MODE24 */
	    phys = Log2Phys(a);
	    pd->pfn = phys_to_pfn(phys);
	    if (phys >= BANKB_PHYS) {
		if (bankB.pg_desc_tbl == PG_DESC_NULL) {
		    if (bankA.pg_desc_tbl != PG_DESC_NULL) {
			bankA.pg_last_phys = last_phys;
			A_done = TRUE;
		    }
		    bankB.pg_desc_tbl = pd;
		    bankB.pg_first_phys = phys;
		}
		else if (B_done == TRUE)
		    break;
	    } else {
		if (bankA.pg_desc_tbl == PG_DESC_NULL) {
		    if (bankB.pg_desc_tbl != PG_DESC_NULL) {
			bankB.pg_last_phys = last_phys;
			B_done = TRUE;
		    }
		    bankA.pg_desc_tbl = pd;
		    bankA.pg_first_phys = phys;
		}
		else if (A_done == TRUE)
		    break;
	    }
	    last_phys = phys + MAC2_PGBYTES;
	}
	if (bankA.pg_desc_tbl != PG_DESC_NULL && A_done == FALSE)
	    bankA.pg_last_phys = last_phys;
	else if (bankB.pg_desc_tbl != PG_DESC_NULL && B_done == FALSE)
	    bankB.pg_last_phys = last_phys;
    }

    if (bankA.pg_desc_tbl != PG_DESC_NULL) {
	*aphys_free = bankA.pg_first_phys;
	if (bankB.pg_desc_tbl == PG_DESC_NULL)
	    *aphys_last = bankA.pg_last_phys;
    }

    if (bankB.pg_desc_tbl != PG_DESC_NULL) {
	*aphys_last = bankB.pg_last_phys;
	if (bankA.pg_desc_tbl == PG_DESC_NULL)
	    *aphys_free = bankB.pg_first_phys;
    }

    return (TRUE);
}

/*
 * Setup the kernel pmap on older (pre-ci)
 * machines.
 */
static
void
pmap_setup_direct(aphys_free, aphys_last, avirt_free, avirt_last)
vm_offset_t	*aphys_free;
vm_offset_t	*aphys_last;
vm_offset_t	*avirt_free;
vm_offset_t	*avirt_last;
{
    register 		slot;
    vm_offset_t		a, askip;
#ifdef MODE24
    vm_size_t		compat_mem_size;
#endif /* MODE24 */
    extern int		start, etext;

    kernel_pmap->root = kernel_pmap->ptrt->page;
#ifdef MODE24
    kernel_pmap->alt_root = kernel_pmap->root + pmap_ptptbl_size;
#endif /* MODE24 */

#ifdef MODE24
    kernel_compat_pmap->root = kernel_pmap->root + (2 * pmap_ptptbl_size);
#endif /* MODE24 */

#ifdef MODE24
    pmap_alloc_kernel_compat_pt(aphys_free);
#endif /* MODE24 */

    /*
     * Map the system RAM into the
     * kernel pmap.
     */
    a = VM_MIN_KERNEL_ADDRESS;

#ifdef MODE24
    /*
     * Running with 24 bit mac memory
     * manager.
     */
    if (mem_size > 0x800000)
	compat_mem_size = 0x800000;
    else
	compat_mem_size = mem_size;
#endif /* MODE24 */

#ifdef MODE24
#define	pmap_map_kernel(va, pa, size, prot) \
    {								\
	if ((va) + (size) <= compat_mem_size)			\
	    pmap_map_compat((va), (pa), (size), (prot));	\
	pmap_alloc_kernel_pt(aphys_free, (va), (size));		\
	(va) = pmap_map((va), (pa), (size), (prot));		\
    }
#else /* MODE24 */
#define	pmap_map_kernel(va, pa, size, prot) \
    {								\
	pmap_alloc_kernel_pt(aphys_free, (va), (size));		\
	(va) = pmap_map((va), (pa), (size), (prot));		\
    }
#endif /* MODE24 */

#ifdef MODE24
    pmap_map_kernel(a, 0, compat_mem_size, VM_PROT_READ|VM_PROT_WRITE);
#else /* MODE24 */
    pmap_map_kernel(a, 0, mem_size, VM_PROT_READ|VM_PROT_WRITE);
#endif

#ifdef MODE24    
    if (mem_size > compat_mem_size)
	pmap_map_kernel(a,
			compat_mem_size,
			mem_size - compat_mem_size,
			VM_PROT_READ|VM_PROT_WRITE);
#endif /* MODE24 */

#undef pmap_map_kernel

    /*
     * Write protect the kernel
     * text.
     */
    pmap_protect(kernel_pmap,
		 &start,
		 round_page(&etext),
		 VM_PROT_READ);

#ifdef MODE24
    pmap_protect_compat(&start,
			round_page(&etext),
			VM_PROT_READ);
#endif /* MODE24 */

    /*
     * Allocate enough extra kernel_pmap
     * space to map an extra kernel_alloc_space bytes.
     */
    pmap_alloc_kernel_pt(aphys_free, a, kernel_alloc_space);

    /*
     * Set virtual_avail and virtual_end
     * to describe the virtual memory space
     * that the kernel may allocate from.
     */
    *avirt_free = a;
    *avirt_last = a + kernel_alloc_space;

    /*
     * Enter special virt == phys
     * mappings into the kernel pmap.
     */

#define	pmap_map_kernel_direct(addr, size, prot) \
    {								\
	pmap_map_trunc((addr), (addr), (size), (prot));		\
    }

#ifdef MODE24
#define pmap_map_kernel_compat(addr, size, prot, phys) \
    {									\
	pmap_map_compat((addr), (phys), (size), (prot));		\
    }
#endif /* MODE24 */

#ifdef	PMAP_ROM_ADDR
    pmap_map_kernel_direct(PMAP_ROM_ADDR,
			   PMAP_ROM_SIZE,
			   PMAP_ROM_PROT);
#endif

#ifdef MODE24
#ifdef	PMAP_ROM_COMPAT_ADDR
    pmap_map_kernel_compat(PMAP_ROM_COMPAT_ADDR,
			   PMAP_ROM_COMPAT_SIZE,
			   PMAP_ROM_PROT,
			   PMAP_ROM_ADDR);
#endif
#endif /* MODE24 */

#ifdef	PMAP_IO_ADDR
    pmap_map_kernel_direct(PMAP_IO_ADDR,
			   PMAP_IO_SIZE,
			   PMAP_IO_PROT);
#endif

#ifdef MODE24
#ifdef	PMAP_IO_COMPAT_ADDR
    pmap_map_kernel_compat(PMAP_IO_COMPAT_ADDR,
			   PMAP_IO_COMPAT_SIZE,
			   PMAP_IO_PROT,
			   PMAP_IO_ADDR);
#endif
#endif /* MODE24 */

    pmap_map_kernel_direct(PMAP_SLOTS_ADDR,
			   PMAP_SLOTS_SIZE,
			   PMAP_SLOTS_PROT);

    for (slot = 0x9; slot <= 0xe; slot++) {

#ifdef MODE24
#ifdef	PMAP_SLOT_COMPAT_ADDR
    pmap_map_kernel_compat(PMAP_SLOT_COMPAT_ADDR(slot),
			   PMAP_SLOT_COMPAT_SIZE,
			   PMAP_SLOT_PROT,
			   PMAP_SLOT_ADDR(slot));
#endif
#endif /* MODE24 */

    }

#undef	pmap_map_kernel_direct
#ifdef MODE24
#undef	pmap_map_kernel_compat
#endif /* MODE24 */

    /*
     *	Allocate some amount of physical memory to use for
     *	user page tables.  The amount allocated is set at
     *  compile time in the kernel data segment, and can
     *  be patched in the kernel a.out file.
     */
    pt_first_kern = *aphys_free;
    *aphys_free += round_page(mac2_ptob(pt_page_alloc_count));
    pt_last_kern = *aphys_free;

    bzero(pt_first_kern, pt_last_kern - pt_first_kern);

    /*
     * Allocate the pt descriptor table, and the
     * pt page queues.
     */
    (vm_offset_t) pt_desc_tbl = *aphys_free;
    *aphys_free += round_page(sizeof (pt_desc_t) * pt_page_alloc_count);

    bzero(pt_desc_tbl, *aphys_free - (vm_offset_t)pt_desc_tbl);

    simple_lock_init(&pt_page_queue_lock);
    queue_init(&pt_page_queue_free);
    queue_init(&pt_page_queue_active);
    {
	register pt_desc_ptr_t 	ptpg;
	vm_offset_t		a = pt_first_kern;
	phys_offset_t		phys;

	for (ptpg = pt_desc_tbl; a < pt_last_kern; ptpg++, a += MAC2_PGBYTES) {
	    enqueue_tail(&pt_page_queue_free, (queue_entry_t) ptpg);
	    ptpg->kern = a;
	    phys = (phys_offset_t)a;
	    ptpg->pfn = phys_to_pfn(phys);
	    if (phys >= BANKB_PHYS) {
		if (bankB.pt_desc_tbl == PT_DESC_NULL) {
		    bankB.pt_desc_tbl = ptpg;
		    bankB.pt_first_pfn = phys_to_pfn(phys);
		}
	    } else {
		if (bankA.pt_desc_tbl == PT_DESC_NULL) {
		    bankA.pt_desc_tbl = ptpg;
		    bankA.pt_first_pfn = phys_to_pfn(phys);
		}
	    }
	}
    }
    pt_page_free_count = pt_page_alloc_count;

    pg_first_kern = *aphys_free;
    pg_last_kern = *aphys_last;

#ifdef MODE24
    if (mem_size > compat_mem_size) {
	askip = pg_last_kern;
	*aphys_last = pg_last_kern = mem_size;
    }
#endif /* MODE24 */

    /*
     * Allocate the page descriptor table.
     */
    (vm_offset_t) pg_desc_tbl = *aphys_free;
    *aphys_free += round_page(sizeof (pg_desc_t) *
			      atop(pg_last_kern - pg_first_kern));

    bzero(pg_desc_tbl, *aphys_free - (vm_offset_t)pg_desc_tbl);

    pg_first_kern = *aphys_free;

    {
	register pg_desc_ptr_t	pd;
	vm_offset_t		a = pg_first_kern;
	phys_offset_t		phys, last_phys;

	for (pd = pg_desc_tbl; a < pg_last_kern; a += MAC2_PGBYTES, pd++) {
#ifdef MODE24
	    if (mem_size == compat_mem_size)
		pd->kern = a;
	    else {
		if (a < compat_mem_size) {
		    if (a >= askip)
			;
		    else
			pd->kern = a;
		}
		else
		    pd->kern = a;
	    }
#else /* MODE24 */
	    pd->kern = a;
#endif /* MODE24 */
	    phys = (phys_offset_t)a;
	    pd->pfn = phys_to_pfn(phys);
	    if (phys >= BANKB_PHYS) {
		if (bankB.pg_desc_tbl == PG_DESC_NULL) {
		    if (bankA.pg_desc_tbl != PG_DESC_NULL)
			bankA.pg_last_phys = last_phys;
		    bankB.pg_desc_tbl = pd;
		    bankB.pg_first_phys = phys;
		}
	    } else {
		if (bankA.pg_desc_tbl == PG_DESC_NULL) {
		    if (bankB.pg_desc_tbl != PG_DESC_NULL)
			bankB.pg_last_phys = last_phys;
		    bankA.pg_desc_tbl = pd;
		    bankA.pg_first_phys = phys;
		}
	    }
	    last_phys = phys + MAC2_PGBYTES;
	}
	if (phys >= BANKB_PHYS)
	    bankB.pg_last_phys = last_phys;
	else
	    bankA.pg_last_phys = last_phys;
    }
}

/*
 * Bootstrap the system to run with MACH virtual memory.  Here
 * the kernel pmap (PMMU tables) is created and enabled.  We must
 * deal with two very different physical architectures.  Pre-ci
 * machines (ROMS?) are setup with RAM, ROM, IO and NuBus mapped
 * virtual == physical, whereas later machines provide configuration
 * tables which are used to set things up.  The resulting kernel
 * address space is identical to the MacOS 32 bit addressing mode
 * for the machine.
 *
 * This routine is called with the machine in 32 bit addressing mode;
 * for most models this means that the PMMU is enabled.  However, for
 * the 020/851 mac II (with original ROMS) the PMMU is disabled.
 *
 * The IN parameters aphys_free and aphys_last are not necessarily
 * physical addresses, they are actually virtual addresses corresponding
 * to the active 32 bit mode mappings.
 *
 * The OUT parameters avirt_free and avirt_last point to an area in
 * the kernel address space which contains no valid mappings, but
 * does have page tables already allocated.  The kernel uses this
 * space to allocate virtual memory for its own use.
 *
 * For machines booted using the 24 bit MacOS memory manager, a special
 * kernel translation table is setup which corresponds to the MacOS
 * 24 bit mode mappings for the machine.
 *
 *	Parameters:
 *	aphys_free	ptr to pa of first available physical page
 *	aphys_last	ptr to pa of last available physical page
 *	avirt_free	ptr to va of first available virtual page address
 *	avirt_last	ptr to va of last available virtual page address
 */
void
pmap_bootstrap()
{
#define aphys_free	(&avail_start)
#define aphys_last	(&avail_end)
#define avirt_free	(&virtual_avail)
#define avirt_last	(&virtual_end)
    PMMU_RP_reg		SRP;
    PMMU_TC_reg		TC;

    /*
     * Setup protection mapping table.
     */
    pmmu_prot_map_init();

    /*
     *	Set pps_per_vm_page for general use.
     */
    pps_per_vm_page = PAGE_SIZE / MAC2_PGBYTES;

    /*
     *	The kernel's pmap is statically allocated so we don't
     *	have to use pmap_create, which is unlikely to work
     *	correctly at this part of the boot sequence.
     */
    kernel_pmap = &kernel_pmap_store;
    kernel_pmap->ref_count = 1;
    simple_lock_init(&kernel_pmap->lock);

#ifdef MODE24
    kernel_compat_pmap = &kernel_compat_pmap_store;
#endif /* MODE24 */

    /*
     * Allocate a page tbl ptr table
     * for the kernel pmap, with a fake
     * pt_root_desc structure.
     */
    kernel_pmap->ptrt = &kernel_pt_root_desc;
    kernel_pmap->ptrt->page = *aphys_free;
    *aphys_free += MAC2_PGBYTES;

#ifdef MODE24
    kernel_pmap->alt_ptrt = kernel_pmap->ptrt;
#endif /* MODE24 */

#ifdef MODE24
    kernel_compat_pmap->ptrt = kernel_pmap->ptrt;
#endif /* MODE24 */

    /*
     * zero it.
     */
    bzero(kernel_pmap->ptrt->page, MAC2_PGBYTES);

    /*
     * Setup kernel pmap.
     */
    if (pmap_setup(aphys_free, aphys_last,
		   avirt_free, avirt_last) == FALSE)
	pmap_setup_direct(aphys_free, aphys_last,
			  avirt_free, avirt_last);

    avail_next = *aphys_free;
    avail_npages = atop(pg_last_kern - pg_first_kern);

    /*
     * Load the SRP with the root of the kernel
     * translation tree.
     */
    SRP.limit = PMMU_RP_LIMIT;
    SRP.valid = (PMMU_RP_SG|PMMU_VALID_RP);
    SRP.phys = kernel_pmap->root;
    pmmu_set_srp(&SRP);

    /*
     * Load the TC register to
     * set the mapping parameters
     * and enable mapping.
     */
    *((unsigned long *)&TC) = 0;
    TC.enable = TC.sre = 1;
    TC.pagesize = PMMU_PAGESIZE;
    TC.addrsize = PMMU_IS_32b;
    TC.tia = PMMU_TIA;
    TC.tib = PMMU_TIB;
    pmmu_set_tc(&TC);
#undef aphys_free
#undef aphys_last
#undef avirt_free
#undef avirt_last
}

/*
 * Initialize the pmap module.
 * Called by vm_init, to initialize any structures that the pmap
 * system needs to map virtual memory.
 */
void
pmap_init()
{
#define phys_start	avail_start
#define phys_end	avail_end
    /*
     *	Create the zone of pmaps, pt root pages and
     *  the physical-to-virtual entries.
     */
    pmap_zone = zinit(sizeof (struct pmap),
		      400 * sizeof (struct pmap),
		      MAC2_PGBYTES, FALSE, "pmap");		/* XXX */

    pt_root_desc_zone = zinit(sizeof (struct pt_root_page),
			      50 * sizeof (struct pt_root_page),
			      MAC2_PGBYTES, FALSE, "pt_root");	/* XXX */

    pv_entry_zone = zinit(sizeof (struct pv_entry),
			  10000 * sizeof (struct pv_entry),
			  MAC2_PGBYTES, FALSE, "pv_list");	/* XXX */

    /*
     * Initialize the pt root page free queue.
     */
    simple_lock_init(&pt_root_page_lock);
    queue_init(&pt_root_page_queue_free);

    /*
     *	Only now, when all of the data structures are allocated,
     *	can we set vm_first_phys and vm_last_phys.  If we set them
     *	too soon, the kmem_alloc above will try to use these
     *	data structures and blow up.
     */
    vm_first_phys = phys_start;
    vm_last_phys = phys_end;

    pmap_initialized = TRUE;
#undef phys_start
#undef phys_end
}

unsigned
pmap_free_pages()
{
    return (avail_npages);
}

static
inline
boolean_t
pmap_valid_page(pa)
register vm_offset_t	pa;
{
    register bank_ptr_t	bp;

    if ((phys_offset_t)pa >= BANKB_PHYS)
	bp = &bankB;
    else
	bp = &bankA;

    if (bp->pg_desc_tbl != PG_DESC_NULL &&
	(phys_offset_t)pa >= bp->pg_first_phys &&
	(phys_offset_t)pa < bp->pg_last_phys) {

	if (pg_desc(pa)->kern != 0)
	    return (TRUE);
    }

    return (FALSE);
}

boolean_t
pmap_next_page(addrp)
register vm_offset_t	*addrp;
{
    while (!pmap_valid_page(avail_next)) {
	if (avail_next == avail_end)
	    return (FALSE);

	avail_next += MAC2_PGBYTES;
    }

    *addrp = avail_next;
    avail_next += MAC2_PGBYTES;
    avail_npages--;

    return (TRUE);
}

void
pmap_virtual_space(startp, endp)
vm_offset_t	*startp, *endp;
{
    *startp = virtual_avail;
    *endp = virtual_end;
}

/*
 * Allocate a free pt page.
 * The affected pmap must be locked.
 * Returns NULL if there are no free pt pages.
 */
static
pt_desc_ptr_t
pt_free_page_alloc(pmap, va)
    register pmap_t	pmap;
    vm_offset_t		va;
{
    register pt_desc_ptr_t	ptpg;

    simple_lock(&pt_page_queue_lock);

    if (queue_empty(&pt_page_queue_free)) {
	simple_unlock(&pt_page_queue_lock);
	return (PT_DESC_NULL);
    }

    ptpg = (pt_desc_ptr_t) dequeue_head(&pt_page_queue_free);
    pt_page_free_count--;

    if (ptpg->pmap != PMAP_NULL || ptpg->va != (vm_offset_t) 0)
	panic("ptpg in use!");

    va &= ~(pt_page_map_size - 1);

    ptpg->pmap = pmap;
    ptpg->va = va;

    enqueue_tail(&pt_page_queue_active, (queue_entry_t) ptpg);
    pt_page_active_count++;

    if (pt_page_active_count > pt_page_max_active)
	pt_page_max_active = pt_page_active_count;

    simple_unlock(&pt_page_queue_lock);

    return (ptpg);
}

/*
 * Allocate a pt page.  May grab one from another pmap if
 * there are no free pages.
 * Must be called with the pmap system locked for write.
 */
static
pt_desc_ptr_t
pt_page_alloc(pmap, va)
    pmap_t		pmap;
    vm_offset_t		va;
{
    register pt_desc_ptr_t	ptpg;
    static void			pmap_remove_range();

    if (queue_empty(&pt_page_queue_free)) {
	pmap_t		old_pmap;
	vm_offset_t	old_va;
	pp_t		*spp, *epp;

	/*
	 * no free pt pages.  Recycle one.
	 */
	ptpg = (pt_desc_ptr_t) queue_first(&pt_page_queue_active);
	if (ptpg == PT_DESC_NULL)
	    panic("not enough pt pages!");

	/*
	 * free the pt page
	 */
	old_pmap = ptpg->pmap;
	old_va = ptpg->va;
	spp = pmap_pp(old_pmap, old_va);
	epp = spp + MAC2_PGBYTES/sizeof(pp_t);
	simple_lock(&old_pmap->lock);
	PMAP_UPDATE_ATC(old_pmap, old_va, old_va + pt_page_map_size);
	pmap_remove_range(old_pmap, old_va, spp, epp, TRUE);
	/*
	 * calls pt_page_free, which clears the pt page
	 * and puts it at the head of the free queue.
	 */
	simple_unlock(&old_pmap->lock);
    }
    return (pt_free_page_alloc(pmap, va));
}
	
/*
 * Free a pt page.
 * The mappings in the pt page must have already been freed.
 * The pmap system needs only to be locked for read.
 */
static
void
pt_page_free(ptpg)
    register pt_desc_ptr_t	ptpg;
{
    /*
     *	Clear the pt group fields.
     */
    ptpg->pmap = PMAP_NULL;
    ptpg->va = (vm_offset_t) 0;
    ptpg->use_count = 0;
    ptpg->wired_count = 0;

    /*
     *	Put it on the free list.
     */
    simple_lock(&pt_page_queue_lock);

    remqueue(&pt_page_queue_active, (queue_entry_t) ptpg);
    pt_page_active_count--;

    enqueue_head(&pt_page_queue_free, (queue_entry_t) ptpg);
    pt_page_free_count++;

    simple_unlock(&pt_page_queue_lock);
}

/*
 * Wire down a pt page so it cannot be reused.
 */
static
void
pt_page_wire(ptpg)
    pt_desc_ptr_t	ptpg;
{
    simple_lock(&pt_page_queue_lock);

    remqueue(&pt_page_queue_active, (queue_entry_t) ptpg);

    pt_page_active_count--;
    pt_page_wired_count++;

    if (pt_page_wired_count > pt_page_max_wired)
	pt_page_max_wired = pt_page_wired_count;

    simple_unlock(&pt_page_queue_lock);
}

/*
 * Unwire a pt page.
 */
static
void
pt_page_unwire(ptpg)
    pt_desc_ptr_t	ptpg;
{
    simple_lock(&pt_page_queue_lock);

    enqueue_tail(&pt_page_queue_active, (queue_entry_t) ptpg);

    pt_page_wired_count--;
    pt_page_active_count++;

    simple_unlock(&pt_page_queue_lock);
}

/*
 * Allocate a pt root and
 * return the pt root page
 * structure pointer and
 * the pt root physical address.
 */
void
pt_root_alloc(aptrt, aroot)
register pt_root_desc_ptr_t *aptrt;
register phys_offset_t *aroot;
{
    register pt_root_desc_ptr_t ptrt;
    register ndx;

    simple_lock(&pt_root_page_lock);

    /*
     * Find a pt root page with a free
     * pt root.  If there are none on the
     * free queue, have to allocate another.
     */
    if (queue_empty(&pt_root_page_queue_free)) {
	simple_unlock(&pt_root_page_lock);

	ptrt = (pt_root_desc_ptr_t)zalloc(pt_root_desc_zone);
	if (ptrt == PT_ROOT_DESC_NULL)
	    panic("pt_root_alloc: zalloc failed");

	if (kmem_alloc_wired(kernel_map,
			     &ptrt->page,
			     MAC2_PGBYTES) != KERN_SUCCESS)
	    panic("pt_root_alloc: kmem_alloc failed");
/* XXX */
	ptrt->free_count = 8;
/* XXX */
	init_bitmap_byte(&ptrt->free_map);

	simple_lock(&pt_root_page_lock);

	pt_root_page_alloc_count++;

	enqueue_head(&pt_root_page_queue_free, (queue_entry_t)ptrt);
	pt_root_page_free_count++;
    }
    else
	ptrt = (pt_root_desc_ptr_t)queue_first(&pt_root_page_queue_free);

    /*
     * Allocate a pt root from this
     * pt root page.  If we are taking
     * the last one, remove from free
     * queue.
     */
    if (--ptrt->free_count == 0) {
	remqueue(&pt_root_page_queue_free, (queue_entry_t)ptrt);
	pt_root_page_free_count--;
    }
    ndx = alloc_bitmap_byte(&ptrt->free_map);
    if (ndx < 0)
	panic("pmap_ptptbl_alloc: bitmap full");

    simple_unlock(&pt_root_page_lock);

#define index_to_pt_root_phys(page, n) \
    pmap_phys(kernel_pmap, ((page)|((n)<<10)))
    /*
     * Return a pointer to the
     * pt root page structure and the
     * physical pt root address.
     */
    *aptrt = ptrt;
    *aroot = index_to_pt_root_phys(ptrt->page, ndx);
#undef index_to_pt_root_phys

    /*
     * Clear the pt root.
     */
    bzero(root_to_ptptbl(ptrt->page, *aroot), pmap_ptptbl_size);
}    

/*
 * Free a pt root.
 */
static
void
pt_root_free(ptrt, root)
register pt_root_desc_ptr_t ptrt;
register phys_offset_t root;
{
    register ndx;

#define pt_root_to_index(p)	(((p)&(MAC2_PGBYTES-1))>>10)
    /*
     * Calculate index from
     * pt root address.
     */
    ndx = pt_root_to_index(root);
#undef pt_root_to_index

    simple_lock(&pt_root_page_lock);

    /*
     * Free pt root.  If we are deallocating
     * the last one in the pt root page and
     * there are other pt root pages with free
     * pt roots, free this one.
     */
    if (++ptrt->free_count == 8 && !queue_empty(&pt_root_page_queue_free)) {
	remqueue(&pt_root_page_queue_free, (queue_entry_t)ptrt);
	pt_root_page_free_count--;

	kmem_free(kernel_map, ptrt->page, MAC2_PGBYTES);
	zfree(pt_root_desc_zone, ptrt);
	pt_root_page_alloc_count--;
    }
    else {
	if (ptrt->free_count == 1) {
	    enqueue_head(&pt_root_page_queue_free, (queue_entry_t)ptrt);
	    pt_root_page_free_count++;
	}
	if (free_bitmap_byte(&ptrt->free_map, ndx) < 0)
	    panic("pmap_ptptbl_free: bitmap already free");
    }

    simple_unlock(&pt_root_page_lock);
}

#ifdef MODE24
/*
 * Switches the kernel pmap to use
 * the alternate root page.  This routine
 * can only be called if the current kernel
 * stack is addressable from 24 bit mode.
 * Called at ipl high.
 */
void
pmap_set_kern_addr_mode(to32bit)
boolean_t	to32bit;
{
    PMMU_RP_reg			rp;

    rp.limit = PMMU_RP_LIMIT;
    rp.valid = (PMMU_RP_SG|PMMU_VALID_RP);
    
    if (to32bit == TRUE)
	rp.phys = kernel_pmap->root;
    else
	rp.phys = kernel_compat_pmap->root;

    pmmu_set_srp(&rp);

    flush_cpu_caches();
}
#endif /* MODE24 */

#ifdef MODE24
/*
 * Setup an alternate root page
 * that simulates 24 bit addressing.
 */
boolean_t
pmap_do_24bit_map(pmap)
register pmap_t	pmap;
{
    register pt_desc_ptr_t	ptpg;
    register ptp_t		*ptp;
    register			ipl;

    PMAP_READ_LOCK(pmap, ipl);

    ptp = root_to_ptptbl(pmap->ptrt->page, pmap->root);

    if (pmap != kernel_pmap) {
	/*
	 * Allocate a page table if
	 * none exists.
	 */
	if (ptp->valid == PMMU_INVALID) {
	    ptpg = pt_free_page_alloc(pmap, (vm_offset_t)0);
	    if (ptpg == PT_DESC_NULL) {
		PMAP_READ_UNLOCK(pmap, ipl);
		PMAP_WRITE_LOCK(ipl);
		ptpg = pt_page_alloc(pmap, (vm_offset_t)0);
		PMAP_WRITE_TO_READ_LOCK(pmap);
	    }
	    ptp->pfn = ptpg->pfn;
	    ptp->valid = PMMU_VALID_PTP;
	}
	else
	    ptpg = pfn_to_pt_desc(ptp->pfn);
	
	ptpg->use_count++;
	
	/*
	 * Wire down the page table
	 * since it will be referenced
	 * from two root pages, one
	 * of which is not known about
	 * by the rest of the code.
	 */
	if (ptpg->wired_count++ == 0)
	    pt_page_wire(ptpg);
    }

    /*
     * Setup the mappings.
     */
    {
	register ptp_t	*eptp;
	ptp_t	template;

	template = *ptp;
	ptp = root_to_ptptbl(pmap->alt_ptrt->page, pmap->alt_root);
	eptp = ptp+(pmap_ptptbl_size/sizeof (ptp_t));
	for (; ptp < eptp;)
	    *ptp++ = template;
    }

    PMAP_READ_UNLOCK(pmap, ipl);

    return (TRUE);
}
#endif /* MODE24 */

#ifdef MODE24
/*
 * Remove references on behalf
 * of alternate root.
 */
void
pmap_undo_24bit_map(pmap)
register pmap_t	pmap;
{
    register pt_desc_ptr_t	ptpg;
    register ptp_t		*ptp;
    register			ipl;

    PMAP_READ_LOCK(pmap, ipl);

    if (pmap != kernel_pmap) {
	ptp = root_to_ptptbl(pmap->alt_ptrt->page, pmap->alt_root);
	ptpg = pfn_to_pt_desc(ptp->pfn);
	
	if (--ptpg->wired_count == 0)
	    pt_page_unwire(ptpg);
	
	if (--ptpg->use_count == 0)
	    pt_page_free(ptpg);
	
    }

    PMAP_READ_UNLOCK(pmap, ipl);    
}
#endif /* MODE24 */

/*
 * The next two functions support the kernel
 * call vm_machine_attribute().  Currently
 * only allow cache flushing.
 */
kern_return_t
pmap_attribute(pmap, addr, size, attr, value)
register pmap_t				pmap;
vm_offset_t				addr;
vm_size_t				size;
vm_machine_attribute_t			attr;
register vm_machine_attribute_val_t	*value;
{
    register kern_return_t		result;

    if (attr)
	result = KERN_INVALID_ARGUMENT;
    else
    switch (*value) {
      case MATTR_VAL_CACHE_FLUSH:
	if (pmap->cpus_using)
	    flush_cpu_cache_opt(CPU_ICACHE_CLEAR | CPU_DCACHE_CLEAR);
	result = KERN_SUCCESS;
	break;

      case MATTR_VAL_ICACHE_FLUSH:
	if (pmap->cpus_using)
	    flush_cpu_cache_opt(CPU_ICACHE_CLEAR);
	result = KERN_SUCCESS;
	break;

      case MATTR_VAL_DCACHE_FLUSH:
	if (pmap->cpus_using)
	    flush_cpu_cache_opt(CPU_DCACHE_CLEAR);
	result = KERN_SUCCESS;
	break;

      default:
	result = KERN_INVALID_ARGUMENT;
	break;
    }

    return (result);
}

void
pmap_remove_attributes(pmap, start, end)
{
}

/*
 * Create and return a physical map.
 *
 * If the size specified for the map
 * is zero, the map is an actual physical
 * map, and may be referenced by the
 * hardware.
 *
 * If the size specified is non-zero,
 * the map will be used in software only, and
 * is bounded by that size.
 */
pmap_t
pmap_create(size)
    vm_size_t		size;
{
    register pmap_t	pmap;

    /*
     *	A software use-only map doesn't even need a map.
     */
    if (size != 0)
	return(PMAP_NULL);

    pmap = (pmap_t) zalloc(pmap_zone);
    if (pmap == PMAP_NULL)
	panic("pmap_create: cannot allocate a pmap");

    /*
     * Initialize some variables
     */
    simple_lock_init(&pmap->lock);
    pmap->ref_count = 1;
#ifdef MODE24
    pmap->flags = 0;
#endif /* MODE24 */
    pmap->cpus_using = 0;
#ifdef MODE24
    pmap->alt_root = 0;
#endif /* MODE24 */

    /*
     *	Initialize statistics.
     */
    pmap->stats.resident_count = 0;
    pmap->stats.wired_count = 0;

    /*
     * Allocate a pt root (ptptbl)
     * for the pmap.
     */
    pt_root_alloc(&pmap->ptrt, &pmap->root);

    return(pmap);
}

/*
 * Retire the given physical map from service.
 * Should only be called if the map contains
 * no valid mappings.
 */
void
pmap_destroy(pmap)
    register pmap_t	pmap;
{
    register int	c, ipl;

    if (pmap == PMAP_NULL)
	return;

    SPLVM(ipl);

    simple_lock(&pmap->lock);
    c = --pmap->ref_count;
    simple_unlock(&pmap->lock);

    SPLX(ipl);

    if (c != 0)
	return;	/* pmap still in use */

    if (pmap_resident_count(pmap))
	printf("pmap_destroy: pmap %x %d mappings still exist\n", pmap, pmap_resident_count(pmap));

#ifdef MODE24
    if (pmap->alt_root) {
	ATC_INVALIDATE_MAP(pmap->alt_root);
	pmap_undo_24bit_map(pmap);
	pt_root_free(pmap->alt_ptrt, pmap->alt_root);
    }
#endif /* MODE24 */

    ATC_INVALIDATE_MAP(pmap->root);
    pt_root_free(pmap->ptrt, pmap->root);

    zfree(pmap_zone, (vm_offset_t)pmap);
}

/*
 * Add a reference to the specified pmap.
 */
void
pmap_reference(pmap)
    register pmap_t	pmap;
{
    register		ipl;

    if (pmap != PMAP_NULL) {
	SPLVM(ipl);

	simple_lock(&pmap->lock);
	pmap->ref_count++;
	simple_unlock(&pmap->lock);

	SPLX(ipl);
    }
}

/*
 * Remove a range of page-pointers.
 * The entries given are the first (inclusive)
 * and last (exclusive) pointers for the VM pages.
 * start is the va for the first page.
 *
 * The pmap must be locked.
 * The range must lie entirely within one pt page.
 * This is NOT checked.
 * Assumes that the pt page exists.
 *
 */
static
void
pmap_remove_range(pmap, start, spp, epp, free_if_empty)
    pmap_t			pmap;
    vm_offset_t			start;
    pp_t			*spp;
    pp_t			*epp;
    boolean_t			free_if_empty;
{
    register pp_t		*pp;
    register pg_desc_ptr_t	pd;
    int				num_removed, num_unwired;
    register vm_offset_t	va;
    vm_offset_t			pa;

    num_removed = 0;
    num_unwired = 0;

    va = start;
    for (pp = spp; pp < epp; pp += pps_per_vm_page, va += PAGE_SIZE) {

	if (pp->valid == PMMU_INVALID)
	    continue;

	num_removed++;
	if (pp->wired)
	    num_unwired++;

	pa = pfn_to_phys(pp->pfn);
	if (pa < vm_first_phys || pa >= vm_last_phys) {
	    /*
	     *	Outside range of managed physical memory.
	     */
	    continue;
	}

	pd = pg_desc(pa);

	LOCK_PVH(pai);

	/*
	 * Get the referenced & modified bits.
	 */
	{
	    register int	i;
	    register pp_t	*p;

	    p = pp;
	    for (i = pps_per_vm_page; i-- > 0; p++) {
		if (p->mod)
		    pd->page_attrib |= PG_MOD;
		if (p->ref)
		    pd->page_attrib |= PG_REF;
	    }
	}

	/*
	 * Remove the mapping from the pvlist for
	 * this physical page.
	 */
	{
	    register pv_entry_t	pv_h, prev, cur;

	    pv_h = pg_desc_pvh(pd);
	    if (pv_h->pmap == PMAP_NULL)
		panic("pmap_remove: null pv_list!");

	    if (pv_h->va == va && pv_h->pmap == pmap) {
		/*
		 * Header is the pv_entry.  Copy the next one
		 * to header and free the next one (we cannot
		 * free the header)
		 */
		cur = pv_h->next;
		if (cur != PV_ENTRY_NULL) {
		    *pv_h = *cur;
		    zfree(pv_entry_zone, (vm_offset_t) cur);
		} else {
		    pv_h->pmap = PMAP_NULL;
		}
	    } else {
		cur = pv_h;
		do {
		    prev = cur;
		    if ((cur = prev->next) == PV_ENTRY_NULL)
			panic("pmap-remove: mapping not in pv_list!");
		} while (cur->va != va || cur->pmap != pmap);
		prev->next = cur->next;
		zfree(pv_entry_zone, (vm_offset_t) cur);
	    }
	}
	UNLOCK_PVH(pai);
    }

    /*
     *	Zero the PPs to remove the mappings.
     */
    bzero(spp, (epp-spp)*sizeof(pp_t));

    /*
     *	Update the counts
     */
    pmap->stats.resident_count -= num_removed;
    pmap->stats.wired_count -= num_unwired;

    if (pmap != kernel_pmap) {
	register pt_desc_ptr_t	ptpg;
	ptp_t *			ptp;

	/*
	 * See whether this pt page can be freed.
	 */
	ptpg = pt_kern_to_pt_desc(spp);
	ptpg->use_count -= num_removed;
	ptpg->wired_count -= num_unwired;
	if (num_unwired > 0 && ptpg->wired_count == 0)
	    pt_page_unwire(ptpg);
	if (ptpg->use_count == 0 && free_if_empty) {
	    ptp = pmap_ptp(pmap, start);
	    ptp->valid = PMMU_INVALID;
	    pt_page_free(ptpg);
	}
    }
}

/*
 * Remove the given range of addresses
 * from the specified pmap.
 *
 * It is assumed that the start and end are properly
 * rounded to the page size.
 */
void
pmap_remove(pmap, start, end)
    pmap_t		pmap;
    vm_offset_t		start, end;
{
    register ptp_t	*sptp, *eptp;
    register pp_t	*spp, *epp;
    int			ipl;

    if (pmap == PMAP_NULL)
	return;

    PMAP_READ_LOCK(pmap, ipl);

    /*
     *	Invalidate the translation buffer first
     */
    PMAP_UPDATE_ATC(pmap, start, end);

    /*
     * Flush the CPU caches if warranted
     */
    PMAP_FLUSH_CPU_CACHES(pmap);

    /*
     * calculate the start and end
     * page-table-pointers
     */
    sptp = pmap_ptp(pmap, start);
    eptp = pmap_ptp(pmap, end);
    if (sptp == eptp) {
	/*
	 * range is in one
	 * page-pointer table
	 */
	if (sptp->valid == PMMU_VALID_PTP) {
	    spp = ptp_to_pp(sptp, pmap, start);
	    epp = ptp_to_pp(sptp, pmap, end);
	    if (spp < epp)
		pmap_remove_range(pmap, start, spp, epp, TRUE);
	}
    } else {
	/*
	 * range crosses page-pointer table
	 * boundary
	 */
	if (sptp->valid == PMMU_VALID_PTP) {
	    /*
	     * do first (maybe partial) pptbl
	     */
	    spp = ptp_to_pp(sptp, pmap, start);
	    epp = (pp_t *)mac2_round_page(spp+1);
	    pmap_remove_range(pmap, start, spp, epp, TRUE);
	}
	for (sptp++; sptp < eptp; sptp++) {
	    /*
	     * do all complete pptbls
	     */
	    if (sptp->valid == PMMU_INVALID)
		continue;
	    start = ptp_to_va(sptp);
	    spp = ptp_to_pptbl(sptp, pmap);
	    epp = (pp_t *)mac2_round_page(spp+1);
	    pmap_remove_range(pmap, start, spp, epp, TRUE);
	}
	if (sptp->valid == PMMU_VALID_PTP) {
	    /*
	    * do last (maybe partial) pptbl
	    */
	    start = ptp_to_va(sptp);
	    spp = ptp_to_pptbl(sptp, pmap);
	    epp = ptp_to_pp(sptp, pmap, end);
	    if (spp < epp)
		pmap_remove_range(pmap, start, spp, epp, TRUE);
	}
    }

    PMAP_READ_UNLOCK(pmap, ipl);
}

/*
 * Removes this physical page from all pmaps
 * in which it resides.  Reflects back modify
 * bits to the pager.
 */
static
void
pmap_remove_all(pa)
    vm_offset_t			pa;
{
    register pv_entry_t		pv_h;
    register pp_t		*pp;
    register pg_desc_ptr_t	pd;
    register vm_offset_t	va;
    register pmap_t		pmap;
    int				ipl;

    if (pa < vm_first_phys || pa >= vm_last_phys) {
	/*
	 *	Not a managed page.
	 */
	return;
    }

    /*
     *	Lock the pmap system first, since we will be changing
     *	several pmaps.
     */
    PMAP_WRITE_LOCK(ipl);

    /*
     *	Walk down PV list, removing all mappings.
     *	We don't have to lock the pv_head, since
     *  we have the entire pmap system.
     */
    pd = pg_desc(pa);
    pv_h = pg_desc_pvh(pd);

    while ((pmap = pv_h->pmap) != PMAP_NULL) {
	va = pv_h->va;

	/*
	 *	Lock the pmap to block pmap_extract and similar routines.
	 */
	simple_lock(&pmap->lock);

	pp = pmap_pp(pmap, va);

	if (pp == PP_NULL)
	    panic("pmap_remove_all: no page-ptr tbl");

	if (pfn_to_phys(pp->pfn) != pa)
	    panic("pmap_remove_all: page_ptr doesn't point to page!");

	pmap->stats.resident_count--;
	if (pp->wired)
	    panic("pmap_remove_all: removing a wired page");

	/*
	 *	Tell CPU using pmap to invalidate its TLB
	 */
	PMAP_UPDATE_ATC(pmap, va, va + PAGE_SIZE);

	/*
	 * Flush cpu caches if warranted
	 */
	PMAP_FLUSH_CPU_CACHES(pmap);

	/*
	 *	Remove the pv_entry from the pv_list.
	 */
	{
	    register pv_entry_t	cur;

	    if ((cur = pv_h->next) != PV_ENTRY_NULL) {
		*pv_h = *cur;
		zfree(pv_entry_zone, (vm_offset_t) cur);
	    } else
		pv_h->pmap = PMAP_NULL;
	}

	/*
	 *	Remove the mapping, collecting any referenced & modified bits.
	 */
	{
	    register pp_t   *p;
	    register int    i;

	    p = pp;
	    for (i = pps_per_vm_page; i-- > 0; p++) {
		if (p->mod)
		    pd->page_attrib |= PG_MOD;
		if (p->ref)
		    pd->page_attrib |= PG_REF;
		*(unsigned long *)p = 0;
	    }
	}

	/*
	 *	Free the pt page if it is no longer needed.
	 */
	if (pmap != kernel_pmap) {
	    register pt_desc_ptr_t	ptpg;
	    register ptp_t *		ptp;

	    ptpg = pt_kern_to_pt_desc(pp);
	    if (--ptpg->use_count == 0) {
		pt_page_free(ptpg);
		/*
		 * Invalidate the ptp.
		 */
		ptp = pmap_ptp(pmap, va);
		ptp->valid = PMMU_INVALID;
	    }
		    
	}
	simple_unlock(&pmap->lock);
    }

    PMAP_WRITE_UNLOCK(ipl);
}

boolean_t
pmap_verify_free(pa)
    vm_offset_t	pa;
{
    int			ipl;
    boolean_t		result;

    if (!pmap_initialized)
	return(TRUE);

    if (pa < vm_first_phys || pa >= vm_last_phys)
	return(FALSE);

    PMAP_WRITE_LOCK(ipl);

    result = (pg_desc_pvh(pg_desc(pa))->pmap == PMAP_NULL);

    PMAP_WRITE_UNLOCK(ipl);
    
    return (result);
}

/*
 * Remove write privileges from all
 * pmaps for this physical page.
 */
static
void
pmap_copy_on_write(pa)
    vm_offset_t		pa;
{
    register pv_entry_t	pv_e;
    register pp_t	*pp;
    register int	i;
    int			ipl;

    if (pa < vm_first_phys || pa >= vm_last_phys) {
	/*
	 *	Not a managed page.
	 */
	return;
    }

    /*
     *	Lock the entire pmap system, since we may be changing
     *	several maps.
     */
    PMAP_WRITE_LOCK(ipl);

    pv_e = pg_desc_pvh(pg_desc(pa));

    if (pv_e->pmap == PMAP_NULL) {
	PMAP_WRITE_UNLOCK(ipl);
	return;		/* no mappings */
    }
    /*
     *	Run down the list of mappings to this physical page,
     *	disabling write privileges on each one.
     */
    while (pv_e != PV_ENTRY_NULL) {
	pmap_t		pmap;
	vm_offset_t	va;

	pmap = pv_e->pmap;
	va = pv_e->va;

	simple_lock(&pmap->lock);

	pp = pmap_pp(pmap, va);

	if (pp == PP_NULL)
	    panic("pmap_copy_on_write: no page-ptr tbl");

	/*
	 * Ask cpus using pmap to invalidate their ATC
	 */
	PMAP_UPDATE_ATC(pmap, va, va + PAGE_SIZE);

	for (i = pps_per_vm_page; i-- > 0; pp++)
	    pp->prot = PMMU_WP;

	simple_unlock(&pmap->lock);

	pv_e = pv_e->next;
    }

    PMAP_WRITE_UNLOCK(ipl);
}

/*
 * Set the physical protection on the
 * specified range of this pmap as requested.
 */
void
pmap_protect(pmap, start, end, prot)
    pmap_t		pmap;
    vm_offset_t		start, end;
    vm_prot_t		prot;
{
    register ptp_t	*sptp, *eptp;
    register pp_t	*spp, *epp;
    register int	mprot;
    int		ipl;

    if (pmap == PMAP_NULL)
	return;

    /*
     * Convert the requested MACH
     * protection value into one
     * that we can implement.
     */
    prot = pmmu_prot_map[prot];

    /*
     * VM_PROT_NONE means remove
     * mappings.
     */
    if (prot == VM_PROT_NONE) {
	pmap_remove(pmap, start, end);
	return;
    }

    mprot = pmmu_prot(prot);

    /*
     * Due to the XP changes,
     * this routine can now only
     * reduce permissions.
     */
    if (mprot == PMMU_RW)
	return;

    SPLVM(ipl);
    simple_lock(&pmap->lock);

    /*
     *	Invalidate the translation buffer first
     */
    PMAP_UPDATE_ATC(pmap, start, end);

    /*
     * calculate the start and end
     * page-table-pointers
     */
    sptp = pmap_ptp(pmap, start);
    eptp = pmap_ptp(pmap, end);
    if (sptp == eptp) {
	/*
	 * range is in one
	 * page-pointer table
	 */
	if (sptp->valid == PMMU_VALID_PTP) {
	    spp = ptp_to_pp(sptp, pmap, start);
	    epp = ptp_to_pp(sptp, pmap, end);
	    for (; spp < epp; spp++)
		spp->prot = mprot;
	}
    } else {
	/*
	 * range crosses page-pointer table
	 * boundary
	 */
	if (sptp->valid == PMMU_VALID_PTP) {
	    /*
	     * do first (maybe partial) pptbl
	     */
	    spp = ptp_to_pp(sptp, pmap, start);
	    epp = (pp_t *)mac2_round_page(spp+1);
	    for (; spp < epp; spp++)
		spp->prot = mprot;
	}
	for (sptp++; sptp < eptp; sptp++) {
	    /*
	     * do all complete pptbls
	     */
	    if (sptp->valid == PMMU_INVALID)
		continue;
	    spp = ptp_to_pptbl(sptp, pmap);
	    epp = (pp_t *)mac2_round_page(spp+1);
	    for (; spp < epp; spp++)
		spp->prot = mprot;
	}
	if (sptp->valid == PMMU_VALID_PTP) {
	    /*
	     * do last (maybe partial) pptbl
	     */
	    spp = ptp_to_pptbl(sptp, pmap);
	    epp = ptp_to_pp(sptp, pmap, end);
	    for (; spp < epp; spp++)
		spp->prot = mprot;
	}
    }

    simple_unlock(&pmap->lock);
    SPLX(ipl);
}

/*
 * Insert the given physical page pa at
 * the specified virtual address va in the
 * target pmap with the protection requested.
 *
 * If specified, the page will be wired down, meaning
 * that the related page_ptr can not be reclaimed.
 *
 * NB:  This is the only routine which MAY NOT lazy-evaluate
 * or lose information.  That is, this routine must actually
 * insert this page into the given pmap NOW.
 */
void
pmap_enter(pmap, va, pa, prot, wired)
    register pmap_t		pmap;
    vm_offset_t			va;
    register vm_offset_t	pa;
    vm_prot_t			prot;
    boolean_t			wired;
{
    register pp_t		*pp;
    register ptp_t		*ptp;
    pv_entry_t			pv_e;
    int				ipl, mprot;
    pt_desc_ptr_t		ptpg;
    vm_offset_t			old_pa;

    if (pmap == PMAP_NULL)
	return;

    /*
     * Convert the requested MACH
     * protection value into one
     * that we can implement.
     */
    prot = pmmu_prot_map[prot];

    /*
     * VM_PROT_NONE means remove
     * the mapping.
     */
    if (prot == VM_PROT_NONE) {
	pmap_remove(pmap, va, va + PAGE_SIZE);
	return;
    }

    mprot = pmmu_prot(prot);

    /*
     *	Must allocate a new pvlist entry while we're unlocked;
     *	zalloc may cause pageout (which will lock the pmap system).
     *	If we determine we need a pvlist entry, we will unlock
     *	and allocate one.  Then we will retry, throwing away
     *	the allocated entry later (if we no longer need it).
     */
    pv_e = PV_ENTRY_NULL;

Retry:
    PMAP_READ_LOCK(pmap, ipl);

    /*
     * check to see if the
     * page-pointer table exists.
     * if not, have to get one.
     */
    pp = pmap_pp(pmap, va);
    if (pp == PP_NULL) {
	if (pmap == kernel_pmap)
	    panic("pmap_enter: kernel pmap too small");

	ptpg = pt_free_page_alloc(pmap, va);
	if (ptpg == PT_DESC_NULL) {
	    /*
	     *	No free pt pages.  Must get the write lock
	     *	on the pmap system to grab one from another pmap.
	     */
	    PMAP_READ_UNLOCK(pmap, ipl);
	    PMAP_WRITE_LOCK(ipl);
	    ptpg = pt_page_alloc(pmap, va);
	    PMAP_WRITE_TO_READ_LOCK(pmap);
	}
	ptp = pmap_ptp(pmap, va);
	ptp->pfn = ptpg->pfn;
	ptp->valid = PMMU_VALID_PTP;
	pp = ptp_to_pp(ptp, pmap, va);
    } else if (pmap == kernel_pmap)
	ptpg = PT_DESC_NULL;
    else
	ptpg = pt_kern_to_pt_desc(pp);

    /*
     *	Special case if the physical page is already mapped
     *	at this address.
     */
    old_pa = pfn_to_phys(pp->pfn);
    if ((pp->valid == PMMU_VALID_PP) && (old_pa == pa)) {
	/*
	 *	May be changing its wired attribute or protection
	 */
	if (wired && !pp->wired) {
	    pmap->stats.wired_count++;
	    if (ptpg && ptpg->wired_count++ == 0)
		pt_page_wire(ptpg);
	} else if (!wired && pp->wired) {
	    pmap->stats.wired_count--;
	    if (ptpg && --ptpg->wired_count == 0)
		pt_page_unwire(ptpg);
	}

	/*
	 * Set protection and wire as indicated.
	 */
	{
	    register int i;
	    register pp_t *p = pp;
	    pp_t template;

	    PMAP_UPDATE_ATC(pmap, va, va + PAGE_SIZE);
	    for (i = pps_per_vm_page; i > 0; i--) {
		template = *p;
		template.prot = mprot;
		if (wired)
		    template.wired = 1;
		else
		    template.wired = 0;
		*p++ = template;
	    }
	}
    } else {
	/*
	 *	Remove old mapping from the PV list if necessary.
	 */
	if (pp->valid == PMMU_VALID_PP) {
	    /*
	     *	Invalidate the translation buffer,
	     *	then remove the mapping.
	     */
	    PMAP_UPDATE_ATC(pmap, va, va + PAGE_SIZE);

	    /*
	     * Flush the CPU caches if warranted
	     */
	    PMAP_FLUSH_CPU_CACHES(pmap);

	    /*
	     *	Don't free the pt page if removing last
	     *	mapping - we will immediately replace it.
	     */
	    pmap_remove_range(pmap, va, pp, pp + pps_per_vm_page, FALSE);
	}

	if (pa >= vm_first_phys && pa < vm_last_phys) {
	    /*
	     *	Enter the mapping in the PV list for this
	     *	physical page.
	     */
	    register pv_entry_t		pv_h;

	    LOCK_PVH(pai);
	    pv_h = pg_desc_pvh(pg_desc(pa));

	    if (pv_h->pmap == PMAP_NULL) {
		/*
		 * No mappings yet
		 */
		pv_h->va = va;
		pv_h->pmap = pmap;
		pv_h->next = PV_ENTRY_NULL;
	    } else {
		/*
		 *	Add new pv_entry after header.
		 */
		if (pv_e == PV_ENTRY_NULL) {
		    UNLOCK_PVH(pai);
		    PMAP_READ_UNLOCK(pmap, ipl);
		    /*
		     * This isn't correct either: if zalloc blocks in
		     * kmem_alloc, we return with our CPU in the active
		     * set but ipl raised.  We then can deadlock
		     * grabbing the zone lock.
		     */
		    SPLVM(ipl);
		    pv_e = (pv_entry_t) zalloc(pv_entry_zone);
		    SPLX(ipl);
		    goto Retry;
		}
		pv_e->va = va;
		pv_e->pmap = pmap;
		pv_e->next = pv_h->next;
		pv_h->next = pv_e;
		/*
		 *	Remember that we used the pvlist entry.
		 */
		pv_e = PV_ENTRY_NULL;
	    }
	    UNLOCK_PVH(pai);
	}

	/*
	 * Count the mapping.
	 */
	pmap->stats.resident_count++;
	if (ptpg)
	    ptpg->use_count++;
	if (wired) {
	    pmap->stats.wired_count++;
	    if (ptpg && ptpg->wired_count++ == 0)
		pt_page_wire(ptpg);
	}

	/*
	 * Create the entry.
	 */
	{
	    register int i;
	    register pp_t *p = pp;
	    pp_t template;

	    *((unsigned long *)&template) = 0;
	    template.pfn = phys_to_pfn(pa);
	    if (wired)
		template.wired = 1;
	    if (pa >= PMAP_IO_ADDR)
		template.ci = 1;
	    template.prot = mprot;
	    template.valid = PMMU_VALID_PP;
	    for (i = pps_per_vm_page; i > 0; i--) {
		*p++ = template;
		template.pfn++;
	    }
	    PMAP_UPDATE_ATC(pmap, va, va + PAGE_SIZE);
	}
    }

    PMAP_READ_UNLOCK(pmap, ipl);

    if (pv_e != PV_ENTRY_NULL) {
	SPLVM(ipl);
	zfree(pv_entry_zone, (vm_offset_t) pv_e);
	SPLX(ipl);
    }
}

/*
 * Change the wiring attribute for a
 * pmap/virtual-address	pair.
 *
 * The mapping must already exist in the pmap.
 */
void
pmap_change_wiring(pmap, va, wired)
    register pmap_t	pmap;
    vm_offset_t		va;
    boolean_t		wired;
{
    register pp_t	*pp;
    register int	i;
    int			ipl;
    pt_desc_ptr_t	ptpg;

    /*
     *	We must grab the pmap system lock because we may
     *	change a pt_page queue.
     */
    PMAP_READ_LOCK(pmap, ipl);

    if ((pp = pmap_pp(pmap, va)) == PP_NULL)
	panic("pmap_change_wiring: no page-ptr tbl");

    if (pmap == kernel_pmap)
	ptpg = PT_DESC_NULL;
    else
	ptpg = pt_kern_to_pt_desc(pp);

    if (wired && !pp->wired) {
	/*
	 * wiring down mapping
	 */
	pmap->stats.wired_count++;
	if (ptpg && ptpg->wired_count++ == 0)
	    pt_page_wire(ptpg);
    }
    else
    if (!wired && pp->wired) {
	/*
	 * unwiring mapping
	 */
	pmap->stats.wired_count--;
	if (ptpg && --ptpg->wired_count == 0)
	    pt_page_unwire(ptpg);
    }

    for (i = pps_per_vm_page; i > 0; i--)
	(pp++)->wired = wired;

    PMAP_READ_UNLOCK(pmap, ipl);
}

/*
 * 	Extract the physical page address associated
 * 	with the given map/virtual_address pair.
 */
vm_offset_t
pmap_extract(pmap, va)
    register pmap_t	pmap;
    vm_offset_t		va;
{
    int			ipl;
    phys_offset_t	pa;

    SPLVM(ipl);

    simple_lock(&pmap->lock);

    pa = pmap_phys(pmap, va);

    simple_unlock(&pmap->lock);

    SPLX(ipl);

    return ((vm_offset_t) pa);
}

/*
 * Copy the range specified by src_addr/len
 * from the source map to the range dst_addr/len
 * in the destination map.
 *
 * This routine is only advisory and need not do anything.
 */
void
pmap_copy(dst_pmap, src_pmap, dst_addr, size, src_addr)
    pmap_t		dst_pmap;
    pmap_t		src_pmap;
    vm_offset_t		dst_addr;
    vm_size_t		size;
    vm_offset_t		src_addr;
{
    /* OPTIONAL */
}

/*
 * Garbage collects the physical map system for
 * pages which are no longer used.
 * Success need not be guaranteed -- that is, there
 * may well be pages which are not referenced, but
 * others may be collected.
 *
 * Called by the pageout daemon when pages are scarce.
 */
void
pmap_collect(pmap)
    pmap_t		pmap;
{
    /* OPTIONAL */
}

/*
 * Bind the given pmap to the given
 * processor.
 */
void
pmap_activate(pmap, thread, cpu)
register pmap_t	pmap;
thread_t	thread;
int		cpu;
{
    PMAP_ACTIVATE(pmap, thread, cpu);
}


/*
 * Indicates that the given physical map is no longer
 * in use on the specified processor.
 */
void
pmap_deactivate(pmap, thread, cpu)
register pmap_t	pmap;
thread_t	thread;
int		cpu;
{
    PMAP_DEACTIVATE(pmap, thread, cpu);
}

/*
 * Return the pmap handle for the kernel.
 */
pmap_t
pmap_kernel()
{
    return (kernel_pmap);
}

/*
 * pmap_zero_page zeros the specified
 * page.
 */
void
pmap_zero_page(pa)
register phys_offset_t	pa;
{
    if (pa >= vm_first_phys && pa < vm_last_phys)
	bzero(pg_desc(pa)->kern, MAC2_PGBYTES);			/* XXX */
}

/*
 * pmap_copy_page copies the specified (machine independent)
 * pages.
 */
void
pmap_copy_page(src, dst)
register phys_offset_t	src, dst;
{
    if (src >= vm_first_phys && src < vm_last_phys)
	(vm_offset_t)src = pg_desc(src)->kern;

    if (dst >= vm_first_phys && dst < vm_last_phys)
	(vm_offset_t)dst = pg_desc(dst)->kern;

    bcopy((vm_offset_t)src, (vm_offset_t)dst, MAC2_PGBYTES);	/* XXX */
}

void
copy_to_phys(ksrc, dst, length)
register vm_offset_t	ksrc;
register phys_offset_t	dst;
register vm_size_t	length;
{
    register vm_offset_t	kdst;

    for (; length > 0;
	 ksrc += MAC2_PGBYTES,
	 dst += MAC2_PGBYTES,
	 length -= MAC2_PGBYTES) {

	if (dst >= vm_first_phys && dst < vm_last_phys)
	    kdst = pg_desc(dst)->kern;
	else
	    kdst = dst;		/* XXX */

	bcopy(ksrc, kdst, MAC2_PGBYTES);
    }
}

/*
 * Make the specified pages (by pmap, offset)
 * pageable (or not) as requested.
 *
 * A page which is not pageable may not take
 * a fault; therefore, its page table entry
 * must remain valid for the duration.
 *
 * This routine is merely advisory; pmap_enter
 * will specify that these pages are to be wired
 * down (or not) as appropriate.
 */
void
pmap_pageable(pmap, start, end, pageable)
    pmap_t	pmap;
    vm_offset_t	start;
    vm_offset_t	end;
    boolean_t	pageable;
{
    /* OPTIONAL */
}

static void		pmap_clear_page_attrib();
static boolean_t	pmap_check_page_attrib();

/*
 * Clear the modify bits on the specified physical page.
 */
void
pmap_clear_modify(pa)
    register vm_offset_t	pa;
{
    if (pa >= vm_first_phys && pa < vm_last_phys)
	pmap_clear_page_attrib(pa, PG_MOD);
}

/*
 * Return whether or not the specified physical page is modified
 * by any pmaps.
 */
boolean_t
pmap_is_modified(pa)
    register vm_offset_t	pa;
{
    if (pa >= vm_first_phys && pa < vm_last_phys)
	return (pmap_check_page_attrib(pa, PG_MOD));
    else
	return (FALSE);
}

/*
 * Clear the reference bit on the specified physical page.
 */
void
pmap_clear_reference(pa)
    vm_offset_t			pa;
{
    if (pa >= vm_first_phys && pa < vm_last_phys)
	pmap_clear_page_attrib(pa, PG_REF);
}

/*
 * Return whether or not the specified physical page is referenced
 * by any physical maps.
 */
boolean_t
pmap_is_referenced(pa)
    vm_offset_t			pa;
{
    if (pa >= vm_first_phys && pa < vm_last_phys)
	return (pmap_check_page_attrib(pa, PG_REF));
    else
	return (FALSE);
}

/*
 * Clear the specified page attributes both in the
 * pmap_page_attributes table and the address translation
 * tables.  Note that we DO have to flush the entries from
 * the ATC since the PMMU uses the bits in the ATC to
 * determine whether it has to write the bits out to memory.
 */
static
void
pmap_clear_page_attrib(pa, attrib)
    register vm_offset_t	pa;
    register int		attrib;
{
    pv_entry_t			pv_h;
    pmap_t			pmap;
    vm_offset_t			va;
    pg_desc_ptr_t		pd;
    register int		i;
    register pp_t		*pp;
    int				ipl;

    pd = pg_desc(pa);

    pd->page_attrib &= ~attrib;

    pv_h = pg_desc_pvh(pd);
    PMAP_WRITE_LOCK(ipl);
    while ((pmap = pv_h->pmap) != PMAP_NULL) {
	va = pv_h->va;

	simple_lock(&pmap->lock);

	PMAP_UPDATE_ATC(pmap, va, va+PAGE_SIZE);

	pp = pmap_pp(pmap, va);
	if (pp != PP_NULL) {
	    for (i = pps_per_vm_page; i-- > 0; pp++) {
		if (attrib & PG_MOD)
		    pp->mod = 0;
		if (attrib & PG_REF)
		    pp->ref = 0;
	    }
	}

	simple_unlock(&pmap->lock);
	if ((pv_h = pv_h->next) == PV_ENTRY_NULL)
	    break;
    }
    PMAP_WRITE_UNLOCK(ipl);
}

/*
 * Check for the specified attributes for the
 * physical page.  if all bits are true in
 * the pmap_page_attributes table, we can trust
 * it.  otherwise, we must check the address
 * translation tables ourselves.  Note that we
 * DO NOT have to flush the entry from the ATC
 * before looking at the address translation
 * table since the ATC is write-through for the bits.
 */
static
boolean_t
pmap_check_page_attrib(pa, attrib)
    register vm_offset_t	pa;
    register int		attrib;
{
    pv_entry_t			pv_h;
    pmap_t			pmap;
    vm_offset_t			va;
    register pg_desc_ptr_t	pd;
    register int		i;
    register pp_t		*pp;
    int				ipl;

    pd = pg_desc(pa);

    if ((pd->page_attrib & attrib) == attrib)
	return (TRUE);

    pv_h = pg_desc_pvh(pd);
    PMAP_WRITE_LOCK(ipl);
    while ((pmap = pv_h->pmap) != PMAP_NULL) {
	va = pv_h->va;

	simple_lock(&pmap->lock);

	pp = pmap_pp(pmap, va);
	if (pp != PP_NULL) {
	    for (i = pps_per_vm_page; i-- > 0; pp++) {
		if (pp->mod)
		    pd->page_attrib |= PG_MOD;
		if (pp->ref)
		    pd->page_attrib |= PG_REF;
	    }
	    if ((pd->page_attrib & attrib) == attrib) {
		simple_unlock(&pmap->lock);
		PMAP_WRITE_UNLOCK(ipl);
		return (TRUE);
	    }
	}

	simple_unlock(&pmap->lock);

	if ((pv_h = pv_h->next) == PV_ENTRY_NULL)
	    break;
    }
    PMAP_WRITE_UNLOCK(ipl);

    return (FALSE);
}

/*
 * Dummy routine to satisfy external reference.
 */
void
pmap_update_interrupt()
{
    /* should never be called. */
    panic("pmap_update_interrupt");
}

/*
 * Lower the permission for all mappings to a given page.
 */
void
pmap_page_protect(pa, prot)
    vm_offset_t	pa;
    vm_prot_t	prot;
{
    switch (prot) {
	case VM_PROT_READ:
	case VM_PROT_READ|VM_PROT_EXECUTE:
	    pmap_copy_on_write(pa);
	    break;

	case VM_PROT_ALL:
	    break;

	default:
	    pmap_remove_all(pa);
	    break;
    }
}
