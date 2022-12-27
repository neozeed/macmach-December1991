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
 * $Log:	exception.h,v $
 * Revision 2.2  91/09/12  16:52:29  bohman
 * 	Created Mach 3.0 version from 2.5 version.
 * 	[91/09/11  17:12:17  bohman]
 * 
 * Revision 2.2  90/08/30  17:49:35  bohman
 * 	Created.
 * 
 */

/*
 *	Apple Macintosh II Mach (macmach)
 *
 *	File: mach/mac2/exeception.h
 */

#ifndef _MACH_MAC2_EXCEPTION_H_
#define _MACH_MAC2_EXCEPTION_H_

/*
 * Machine dependent exception definitions for the Macintosh II.
 */

/*
 * EXC_BAD_INSTRUCTION
 */
#define	EXC_MAC2_ILLEGAL_INSTRUCTION	0x10
#define EXC_MAC2_PRIVILEGE_VIOLATION	0x20
#define EXC_MAC2_COPROCESSOR		0x34
#define EXC_MAC2_TRAP0			0x80
#define EXC_MAC2_TRAP1			0x84
#define EXC_MAC2_TRAP2			0x88
#define EXC_MAC2_TRAP3			0x8c
#define EXC_MAC2_TRAP4			0x90
#define EXC_MAC2_TRAP5			0x94
#define EXC_MAC2_TRAP6			0x98
#define EXC_MAC2_TRAP7			0x9c
#define EXC_MAC2_TRAP8			0xa0
#define EXC_MAC2_TRAP9			0xa4
#define EXC_MAC2_TRAP10			0xa8
#define EXC_MAC2_TRAP11			0xac
#define EXC_MAC2_TRAP12			0xb0
#define EXC_MAC2_TRAP13			0xb4
#define EXC_MAC2_TRAP14			0xb8
#define EXC_MAC2_TRAP15			0xbc
#define EXC_MAC2_FLT_BSUN		0xc0
#define EXC_MAC2_FLT_OPERAND_ERROR	0xd0

/*
 * EXC_ARITHMETIC
 */
#define EXC_MAC2_ZERO_DIVIDE		0x14
#define EXC_MAC2_FLT_INEXACT		0xc4
#define EXC_MAC2_FLT_ZERO_DIVIDE	0xc8
#define EXC_MAC2_FLT_UNDERFLOW		0xcc
#define EXC_MAC2_FLT_OVERFLOW		0xd4
#define EXC_MAC2_FLT_NOT_A_NUMBER	0xd8

/*
 * EXC_EMULATION
 */
#define EXC_MAC2_LINE_1010		0x28
#define EXC_MAC2_LINE_1111		0x2c

/*
 * EXC_SOFTWARE
 */
#define	EXC_MAC2_CHK			0x18
#define EXC_MAC2_TRAPV			0x1c

/*
 * EXC_BREAKPOINT
 */
#define EXC_MAC2_TRACE			0x24
#define EXC_MAC2_BREAKPOINT		0xbc

#endif	_MACH_MAC2_EXCEPTION_H_
