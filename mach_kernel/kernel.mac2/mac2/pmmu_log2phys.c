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
 * $Log:	pmmu_log2phys.c,v $
 * Revision 2.2  91/09/12  16:43:22  bohman
 * 	Created.
 * 	[91/09/11  15:01:14  bohman]
 * 
 */

/*
 *	Apple Macintosh II Mach (macmach)
 *
 *	File: mac2/pmmu_log2phys.c
 *	Author: David E. Bohman II (CMU macmach)
 */

/*
 * Log2Phys() converts a virtual address
 * into a physical one by walking an arbitrary
 * PMMU translation tree.  It is only used at
 * boot time to setup the kernel pmap.
 */

static inline
unsigned long
extract_field(v, o, w)
unsigned long	v, o, w;
{
    unsigned long	field;

    asm("bfextu %1{%2:%3},%0" : "=d" (field) : "d" (v), "d" (o), "d" (w));

    return (field);
}

typedef union {
    unsigned long	d_4_byte;
    unsigned long	d_8_byte[2];
    struct short_tbl {
	unsigned long
	  tbl_adr:28,
	    :2,
	  dt:2;
    } short_tbl;
    struct long_tbl {
	unsigned short
	  l_u:1,
	  limit:15;
	unsigned short
	    :14,
	  dt:2;
	unsigned long
	  tbl_adr:28,
	    :4;
    } long_tbl;
#define TBL_SHIFT	4
    struct short_pg {
	unsigned long
	  pg_adr:24,
	    :6,
	  dt:2;
    } short_pg;
    struct long_pg {
	unsigned short
	    :16;
	unsigned short
	    :14,
	  dt:2;
	unsigned long
	  pg_adr:24,
	    :8;
    } long_pg;
#define PG_SHIFT	8
#define PG_MASK		(pagesize - 1)
} desc_t, *desc_ptr_t;
#define DT_INVALID	0
#define DT_PAGE		1
#define DT_TBL_4	2
#define DT_TBL_8	3

typedef struct {
    struct long_tbl RP;
    struct {
	unsigned long
	  enable:1,
	    :5,
	  sre:1,
	  fcl:1,
	  ps:4,
	  is:4,
	  tia:4,
	  tib:4,
	  tic:4,
	  tid:4;
    } TC;
} *mmu_info_t;

int	Log2Physdebug = 0;

static
phys_offset_t
do_level(level,
	 dp, desc_is_long,
	 va, bit_offset, bit_widths,
	 pagesize, phys2log)
int		level;
desc_ptr_t	dp;
boolean_t	desc_is_long;
vm_offset_t	va;
unsigned char	bit_offset;
unsigned char	*bit_widths;
unsigned long	pagesize;
long		phys2log;
{
    desc_t		desc;
    phys_offset_t	phys;
    boolean_t		tbl_is_long;
#define FETCH_DESC(t, i) \
    {							\
	unsigned long	p;				\
\
	if (tbl_is_long) {				\
	    p = ((t) + phys2log) + ((i) * 8);		\
	    desc.d_8_byte[0] = *((unsigned long *)p)++;	\
	    desc.d_8_byte[1] = *((unsigned long *)p);	\
	}						\
	else {						\
	    p = ((t) + phys2log) + ((i) * 4);		\
	    desc.d_4_byte = *((unsigned long *)p);	\
	}						\
    }

indirect:

    if (Log2Physdebug)
	printf("do_level(%d, %c, %x, %d, [ %d, %d, %d, %d ], %x, %x)\n",
	       level,
	       (desc_is_long == TRUE)? 't': 'f',
	       va,
	       bit_offset,
	       bit_widths[0], bit_widths[1], bit_widths[2], bit_widths[3],
	       pagesize, phys2log);

    if (desc_is_long) {
	switch (dp->long_tbl.dt) {
	  case DT_INVALID:
	    phys = 0;
	    return (phys);
	    
	  case DT_PAGE:
	    phys = (dp->long_pg.pg_adr << PG_SHIFT) & ~PG_MASK;
	    phys += extract_field(va, bit_offset, 32 - bit_offset);
	    return (phys);
	    
	  case DT_TBL_4:
	    tbl_is_long = FALSE;
	    break;
	    
	  case DT_TBL_8:
	    tbl_is_long = TRUE;
	    break;
	}

	FETCH_DESC(dp->long_tbl.tbl_adr << TBL_SHIFT,
		   extract_field(va, bit_offset, bit_widths[level]));
    }
    else {
	switch (dp->short_tbl.dt) {
	  default:
	  case DT_INVALID:
	    phys = 0;
	    return (phys);

	  case DT_PAGE:
	    phys = (dp->short_pg.pg_adr << PG_SHIFT) & ~PG_MASK;
	    phys += extract_field(va, bit_offset, 32 - bit_offset);
	    return (phys);
	    
	  case DT_TBL_4:
	    tbl_is_long = FALSE;
	    break;
	    
	  case DT_TBL_8:
	    tbl_is_long = TRUE;
	    break;
	}

	FETCH_DESC(dp->short_tbl.tbl_adr << TBL_SHIFT,
		   extract_field(va, bit_offset, bit_widths[level]));
    }

    if (bit_widths[level])
	phys = do_level(level + 1,
			&desc, tbl_is_long,
			va,
			bit_offset + bit_widths[level],
			bit_widths,
			pagesize, phys2log);
    else {
	dp = &desc;
	desc_is_long = tbl_is_long;
	goto indirect;
    }

    return (phys);
}

static
phys_offset_t
Log2Phys(va)
vm_offset_t	va;
{

    if (BootGlobPtr == 0xffffffff)
	return ((phys_offset_t)va);

    {
	BootGlobs_t *bg = (BootGlobs_t *)(BootGlobPtr - sizeof (BootGlobs_t));
	mmu_info_t mi = (mmu_info_t)MMUInfoPtr;
	unsigned char	widths[4] =
	    { mi->TC.tia, mi->TC.tib, mi->TC.tic, mi->TC.tid };
	
	return ((phys_offset_t)do_level(0,
					&mi->RP, TRUE,
					va, 0, widths,
					(1 << mi->TC.ps), bg->Phys2Log));
    }
}
