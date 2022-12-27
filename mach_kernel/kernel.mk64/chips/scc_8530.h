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
 * $Log:	scc_8530.h,v $
 * Revision 2.2  91/08/24  11:52:49  af
 * 	Created, from the Zilog specs:
 * 	"Z8530 SCC Serial Communications Controller, Product Specification"
 * 	in the "1983/84 Components Data Book" pp 409-429, September 1983
 * 	Zilog, Campbell, CA 95008
 * 	[91/06/21            af]
 * 
 */
/*
 *	File: scc_8530.h
 * 	Author: Alessandro Forin, Carnegie Mellon University
 *	Date:	6/91
 *
 *	Definitions for the Zilog Z8530 SCC serial line chip
 */

#ifndef	_SCC_8530_H_
#define	_SCC_8530_H_

/*
 * 3min's padding
 */
typedef struct {
	char			pad0;
	volatile unsigned char	datum;
	char			pad1[2];
} scc_padded1_register_t;

#include <platforms.h>

#ifdef	KMIN
#define	scc_register_t	scc_padded1_register_t
#else	/*KMIN*/
typedef struct {
	volatile unsigned char	datum;
} scc_register_t;
#endif	/*KMIN*/

/*
 * Register map
 */

typedef struct {
    /* Channel B is first, then A */
    struct {
	scc_register_t	scc_command;	/* reg select */
	scc_register_t	scc_data;	/* Rx/Tx buffer */
    } scc_channel[2];
} scc_regmap_t;

#define	SCC_CHANNEL_A	1
#define	SCC_CHANNEL_B	0

#define	scc_init_reg(scc,chan)		{ \
		char tmp; \
		tmp = (scc)->scc_channel[(chan)].scc_command.datum; \
		tmp = (scc)->scc_channel[(chan)].scc_command.datum; \
	}

#define	scc_read_reg(scc,chan,reg,val)	{ \
		(scc)->scc_channel[(chan)].scc_command.datum = (reg); \
		(val) = (scc)->scc_channel[(chan)].scc_command.datum; \
	}

#define	scc_read_reg_zero(scc,chan,val)	{ \
		(val) = (scc)->scc_channel[(chan)].scc_command.datum; \
	}

#define	scc_write_reg(scc,chan,reg,val)	{ \
		(scc)->scc_channel[(chan)].scc_command.datum = (reg); \
		(scc)->scc_channel[(chan)].scc_command.datum = (val); \
	}

#define	scc_read_data(scc,chan,val)	{ \
		(val) = (scc)->scc_channel[(chan)].scc_data.datum; \
	}

#define	scc_write_data(scc,chan,val) { \
		(scc)->scc_channel[(chan)].scc_data.datum = (val); \
	}

/*
 * Addressable registers
 */

#define	SCC_RR0		0
#define	SCC_RR1		1
#define	SCC_RR2		2
#define	SCC_RR3		3
#define	SCC_RR8		8
#define	SCC_RR10	10
#define	SCC_RR12	12
#define	SCC_RR13	13
#define	SCC_RR15	15

#define	SCC_WR0		0
#define	SCC_WR1		1
#define	SCC_WR2		2
#define	SCC_WR3		3
#define	SCC_WR4		4
#define	SCC_WR5		5
#define	SCC_WR6		6
#define	SCC_WR7		7
#define	SCC_WR8		8
#define	SCC_WR9		9
#define	SCC_WR10	10
#define	SCC_WR11	11
#define	SCC_WR12	12
#define	SCC_WR13	13
#define	SCC_WR14	14
#define	SCC_WR15	15

/*
 * Read registers defines
 */

#define	SCC_RR0_BREAK		0x80
#define	SCC_RR0_TX_UNDERRUN	0x40
#define	SCC_RR0_CTS		0x20
#define	SCC_RR0_SYNCH		0x10
#define	SCC_RR0_DCD		0x08
#define	SCC_RR0_TX_EMPTY	0x04
#define	SCC_RR0_ZERO_COUNT	0x02
#define	SCC_RR0_RX_AVAIL	0x01

#define	SCC_RR1_EOF		0x80
#define	SCC_RR1_CRC_ERR		0x40
#define	SCC_RR1_FRAME_ERR	0x40
#define	SCC_RR1_RX_OVERRUN	0x20
#define	SCC_RR1_PARITY_ERR	0x10
#define	SCC_RR1_RESIDUE0	0x08
#define	SCC_RR1_RESIDUE1	0x04
#define	SCC_RR1_RESIDUE2	0x02
#define	SCC_RR1_ALL_SENT	0x01

/* RR2 contains the interrupt vector unmodified (channel A) or
   modified as follows (channel B) */
/* XXX missing from specs */

/* Interrupts pending, to be read from channel A only (B raz) */
#define	SCC_RR3_zero		0xc0
#define	SCC_RR3_RX_IP_A		0x20
#define	SCC_RR3_TX_IP_A		0x10
#define	SCC_RR3_EXT_IP_A	0x08
#define	SCC_RR3_RX_IP_B		0x04
#define	SCC_RR3_TX_IP_B		0x02
#define	SCC_RR3_EXT_IP_B	0x01

/* RR8 is the receive data buffer, a 3 deep FIFO */
#define	SCC_RECV_BUFFER		SCC_RR8
#define	SCC_RECV_FIFO_DEEP	3

#define	SCC_RR10_1CLKS		0x80
#define	SCC_RR10_2CLKS		0x40
#define	SCC_RR10_zero		0x2d
#define	SCC_RR10_LOOP_SND	0x10
#define	SCC_RR10_ON_LOOP	0x02

/* RR12/RR13 hold the timing base, upper byte in RR13 */

#define	scc_get_timing_base(scc,chan,val)	{ \
		register char	tmp;	\
		scc_read_reg(scc,chan,SCC_RR12,val);\
		scc_read_reg(scc,chan,SCC_RR13,tmp);\
		(val) = ((val)<<8)|(tmp&0xff);\
	}

#define	SCC_RR15_BREAK_IE	0x80
#define	SCC_RR15_TX_UNDERRUN_IE	0x40
#define	SCC_RR15_CTS_IE		0x20
#define	SCC_RR15_SYNCH_IE	0x10
#define	SCC_RR15_DCD_IE		0x08
#define	SCC_RR15_zero		0x05
#define	SCC_RR15_ZERO_COUNT_IE	0x02


/*
 * Write registers defines
 */

/* WR0 is used for commands too */
#define	SCC_RESET_TXURUN_LATCH	0xc0
#define	SCC_RESET_TX_CRC	0x80
#define	SCC_RESET_RX_CRC	0x40
#define	SCC_RESET_HIGHEST_IUS	0x38
#define	SCC_RESET_ERROR		0x30
#define	SCC_RESET_TX_IP		0x28
#define	SCC_IE_NEXT_CHAR	0x20
#define	SCC_SEND_SDLC_ABORT	0x18
#define	SCC_RESET_EXT_IP	0x10

#define	SCC_WR1_DMA_ENABLE	0x80	/* dma control */
#define	SCC_WR1_DMA_ACTLOW	0x40
#define	SCC_WR1_DMA_ON_RX_TX	0x20
#define	SCC_WR1_RXI_SPECIAL_O	0x18	/* interrupt enable/conditions */
#define	SCC_WR1_RXI_ALL_CHAR	0x10
#define	SCC_WR1_RXI_FIRST_CHAR	0x08
#define	SCC_WR1_RXI_DISABLE	0x00
#define	SCC_WR1_PARITY_IE	0x04
#define	SCC_WR1_TX_IE		0x02
#define	SCC_WR1_EXT_IE		0x01

/* WR2 is common and contains the interrupt vector */

#define	SCC_WR3_RX_8_BITS	0xc0
#define	SCC_WR3_RX_6_BITS	0x80
#define	SCC_WR3_RX_7_BITS	0x40
#define	SCC_WR3_RX_5_BITS	0x00
#define	SCC_WR3_AUTO_ENABLE	0x20
#define	SCC_WR3_HUNT_MODE	0x10
#define	SCC_WR3_RX_CRC_ENABLE	0x08
#define	SCC_WR3_SDLC_SRCH	0x04
#define	SCC_WR3_INHIBIT_SYNCH	0x02
#define	SCC_WR3_RX_ENABLE	0x01

#define	SCC_WR4_CLK_x64		0xc0
#define	SCC_WR4_CLK_x32		0x80
#define	SCC_WR4_CLK_x16		0x40
#define	SCC_WR4_CLK_x1		0x00
#define	SCC_WR4_EXT_SYNCH_MODE	0x30
#define	SCC_WR4_SDLC_MODE	0x20
#define	SCC_WR4_16BIT_SYNCH	0x10
#define	SCC_WR4_8BIT_SYNCH	0x00
#define	SCC_WR4_2_STOP		0x0c
#define	SCC_WR4_1_5_STOP	0x08
#define	SCC_WR4_1_STOP		0x04
#define	SCC_WR4_SYNCH_MODE	0x00
#define	SCC_WR4_EVEN_PARITY	0x02
#define	SCC_WR4_PARITY_ENABLE	0x01

#define	SCC_WR5_DTR		0x80
#define	SCC_WR5_TX_8_BITS	0x60
#define	SCC_WR5_TX_6_BITS	0x40
#define	SCC_WR5_TX_7_BITS	0x20
#define	SCC_WR5_TX_5_BITS	0x00
#define	SCC_WR5_SEND_BREAK	0x10
#define	SCC_WR5_TX_ENABLE	0x08
#define	SCC_WR5_CRC_16		0x04	/* if zero.. */
#define	SCC_WR5_SDLC		0x00	/* ... */
#define	SCC_WR5_RTS		0x02
#define	SCC_WR5_TX_CRC_ENABLE	0x01

/* Registers WR6 and WR7 are for synch modes data, with among other things: */

#define	SCC_WR6_BISYNCH_12	0x0f
#define	SCC_WR6_SDLC_RANGE_MASK	0x0f
#define	SCC_WR7_SDLC		0x7e

/* WR8 is the transmit data buffer (no FIFO) */
#define	SCC_XMT_BUFFER		SCC_WR8

#define	SCC_WR9_HW_RESET	0xc0
#define	SCC_WR9_RESET_CHA_A	0x80
#define	SCC_WR9_RESET_CHA_B	0x40
#define	SCC_WR9_zero		0x20
#define	SCC_WR9_STATUS_HIGH	0x10
#define	SCC_WR9_MASTER_IE	0x08
#define	SCC_WR9_DLC		0x04
#define	SCC_WR9_NV		0x02
#define	SCC_WR9_VIS		0x01

#define	SCC_WR10_CRC_PRESET	0x80
#define	SCC_WR10_FM0		0x60
#define	SCC_WR10_FM1		0x40
#define	SCC_WR10_NRZI		0x20
#define	SCC_WR10_NRZ		0x00
#define	SCC_WR10_ACTIVE_ON_POLL	0x10
#define	SCC_WR10_MARK_IDLE	0x08
#define	SCC_WR10_ABORT_ON_URUN	0x04
#define	SCC_WR10_LOOP_MODE	0x02
#define	SCC_WR10_6BIT_SYNCH	0x01
#define	SCC_WR10_8BIT_SYNCH	0x00

#define	SCC_WR11_EXT_RTx_CLOCK	0x80
#define	SCC_WR11_RCLK_DPLL	0x60
#define	SCC_WR11_RCLK_BAUDR	0x40
#define	SCC_WR11_RCLK_TRc_PIN	0x20
#define	SCC_WR11_RCLK_RTc_PIN	0x00
#define	SCC_WR11_XTLK_DPLL	0x18
#define	SCC_WR11_XTLK_BAUDR	0x10
#define	SCC_WR11_XTLK_TRc_PIN	0x08
#define	SCC_WR11_XTLK_RTc_PIN	0x00
#define	SCC_WR11_TRc_OUT	0x04
#define	SCC_WR11_TRcOUT_DPLL	0x03
#define	SCC_WR11_TRcOUT_BAUDR	0x02
#define	SCC_WR11_TRcOUT_XMTCLK	0x01
#define	SCC_WR11_TRcOUT_XTAL	0x00

/* WR12/WR13 are for timing base preset */
#define	scc_set_timing_base(scc,chan,val)	{ \
		scc_write_reg(scc,chan,SCC_RR12,val);\
		scc_write_reg(scc,chan,SCC_RR13,(val)>>8);\
	}

/* More commands in this register */
#define	SCC_WR14_NRZI_MODE	0xe0
#define	SCC_WR14_FM_MODE	0xc0
#define	SCC_WR14_RTc_SOURCE	0xa0
#define	SCC_WR14_BAUDR_SOURCE	0x80
#define	SCC_WR14_DISABLE_DPLL	0x60
#define	SCC_WR14_RESET_CLKMISS	0x40
#define	SCC_WR14_SEARCH_MODE	0x20
/* ..and more bitsy */
#define	SCC_WR14_LOCAL_LOOPB	0x10
#define	SCC_WR14_AUTO_ECHO	0x08
#define	SCC_WR14_DTR_REQUEST	0x04
#define	SCC_WR14_BAUDR_SRC	0x02
#define	SCC_WR14_BAUDR_ENABLE	0x01

#define	SCC_WR15_BREAK_IE	0x80
#define	SCC_WR15_TX_UNDERRUN_IE	0x40
#define	SCC_WR15_CTS_IE		0x20
#define	SCC_WR15_SYNCHUNT_IE	0x10
#define	SCC_WR15_DCD_IE		0x08
#define	SCC_WR15_zero		0x05
#define	SCC_WR15_ZERO_COUNT_IE	0x02


/* XXX Move elsewhere XXX */
#define NSCC_LINE	2
/* mouse on 0,a
   keybd on 1,a
   (r)console on ?wesay?
 */

#endif	/*_SCC_8530_H_*/
