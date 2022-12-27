/* 
 * Mach Operating System
 * Copyright (c) 1990 Carnegie-Mellon University
 * Copyright (c) 1989 Carnegie-Mellon University
 * Copyright (c) 1988 Carnegie-Mellon University
 * All rights reserved.  The CMU software License Agreement specifies
 * the terms and conditions for use and redistribution.
 */
/*
 * HISTORY
 * $Log:	frame.h,v $
 * Revision 2.2  91/09/12  16:52:40  bohman
 * 	Created Mach 3.0 version from 2.5 version.
 * 	[91/09/11  17:12:52  bohman]
 * 
 * Revision 2.2.1.1  90/09/07  00:55:15  bohman
 * 	Final cleanup.
 * 	[90/09/07            bohman]
 * 
 * Revision 2.2  90/08/30  17:50:12  bohman
 * 	Created.
 * 
 */

/*
 *	Apple Macintosh II Mach (macmach)
 *
 *	File: mach/mac2/frame.h
 *	Author: David E. Bohman II (CMU macmach)
 */

#ifndef _MACH_MAC2_FRAME_H_
#define _MACH_MAC2_FRAME_H_

/*
 * MC68020/30 exception frames
 */

typedef struct {
    unsigned short f_sr;
    unsigned long f_pc;
    unsigned short f_fmt:4,
		f_vector:12;
} normal_exception_frame_t;
#define STKFMT_NORMAL 0x0
#define NORMAL_EXCEPTION_FRAME_SIZE	sizeof (normal_exception_frame_t)

typedef struct {
    unsigned short f_sr;
    unsigned long f_pc;
    unsigned short f_fmt:4,
		f_vector:12;
    unsigned long f_instpc;
} special_exception_frame_t;
#define STKFMT_SPECIAL 0x2
#define SPECIAL_EXCEPTION_FRAME_SIZE	sizeof (special_exception_frame_t)

typedef struct {
    unsigned short f_sr;
    unsigned long f_pc;
    unsigned short f_fmt:4,
		f_vector:12;
    unsigned long f_instpc;
    unsigned long f_internal;
    unsigned long f_eea;
} coproc_exception_frame_t;
#define STKFMT_COPROC 0x9
#define COPROC_EXCEPTION_FRAME_SIZE	sizeof (coproc_exception_frame_t)

#define BUSERR_READ  1
#define BUSERR_WRITE 0

#define BUSERR_SIZE_LONG 0
#define BUSERR_SIZE_BYTE 1
#define BUSERR_SIZE_WORD 2

typedef struct {
    unsigned short f_sr;
    unsigned long f_pc;
    unsigned short f_fmt:4,
		f_vector:12;
    unsigned short f_internal;
    struct {
	unsigned short ssw_faultc:1,
		       ssw_faultb:1,
		       ssw_rerunc:1,
		       ssw_rerunb:1,
				 :3,
		       ssw_dfault:1,
			  ssw_rmw:1,
			   ssw_rw:1,
			 ssw_size:2,
				 :1,
			ssw_fcode:3;
    } f_ssw;
#define f_faultc f_ssw.ssw_faultc
#define f_faultb f_ssw.ssw_faultb
#define f_rerunc f_ssw.ssw_rerunc
#define f_rerunb f_ssw.ssw_rerunb
#define f_dfault f_ssw.ssw_dfault
#define f_rmw    f_ssw.ssw_rmw
#define f_rw     f_ssw.ssw_rw
#define f_size   f_ssw.ssw_size
#define f_fcode  f_ssw.ssw_fcode
    unsigned short f_ipsc;
    unsigned short f_ipsb;
    unsigned long f_fault;
    unsigned long f_internal2;
    unsigned long f_dob;
    unsigned long f_internal3;
} short_buserr_exception_frame_t;
#define STKFMT_SHORT_BUSERR 0xA
#define SHORT_BUSERR_EXCEPTION_FRAME_SIZE \
	sizeof (short_buserr_exception_frame_t)

typedef struct {
    unsigned short f_sr;
    unsigned long f_pc;
    unsigned short f_fmt:4,
		f_vector:12;
    unsigned short f_internal;
    struct {
	unsigned short ssw_faultc:1,
		       ssw_faultb:1,
		       ssw_rerunc:1,
		       ssw_rerunb:1,
				 :3,
		       ssw_dfault:1,
			  ssw_rmw:1,
			   ssw_rw:1,
			 ssw_size:2,
				 :1,
			ssw_fcode:3;
    } f_ssw;
    unsigned short f_ipsc;
    unsigned short f_ipsb;
    unsigned long f_fault;
    unsigned long f_internal2;
    unsigned long f_dob;
    unsigned long f_internal3;
    unsigned long f_internal4;
    unsigned long f_stageb;
    unsigned long f_internal5;
    unsigned long f_dib;
    unsigned long f_internal6[11];
} long_buserr_exception_frame_t;
#define STKFMT_LONG_BUSERR 0xB
#define LONG_BUSERR_EXCEPTION_FRAME_SIZE \
	sizeof (long_buserr_exception_frame_t)

/*
 * Generic exception frame
 */
typedef union {
    normal_exception_frame_t	f_normal;
    special_exception_frame_t	f_special;
    coproc_exception_frame_t	f_coproc;
    short_buserr_exception_frame_t
				f_short_buserr;
    long_buserr_exception_frame_t
				f_long_buserr;
} generic_exception_frame_t;

#endif	_MACH_MAC2_FRAME_H_
