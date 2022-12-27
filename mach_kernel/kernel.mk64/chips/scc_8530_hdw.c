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
 * $Log:	scc_8530_hdw.c,v $
 * Revision 2.3  91/08/28  11:09:53  jsb
 * 	Fixed scc_scan to actually check the tp->state each time,
 * 	we are not notified when the MI code brutally zeroes it
 * 	on close and so we cannot update our software CARrier.
 * 	[91/08/27  16:18:05  af]
 * 
 * Revision 2.2  91/08/24  11:52:54  af
 * 	Created, from the Zilog specs:
 * 	"Z8530 SCC Serial Communications Controller, Product Specification"
 * 	in the "1983/84 Components Data Book" pp 409-429, September 1983
 * 	Zilog, Campbell, CA 95008
 * 	[91/06/28            af]
 * 
 */
/*
 *	File: scc_8530_hdw.c
 * 	Author: Alessandro Forin, Carnegie Mellon University
 *	Date:	6/91
 *
 *	Hardware-level operations for the SCC Serial Line Driver
 */

#include <scc.h>
#if	NSCC > 0
#include <bm.h>
#include <platforms.h>

#include <mach_kdb.h>

#include <machine/machparam.h>		/* spl definitions */
#include <sys/types.h>
#include <mach/std_types.h>
#include <device/io_req.h>
#include <device/tty.h>

#include <chips/busses.h>
#include <chips/serial_defs.h>
#include <chips/screen_defs.h>

#include <chips/scc_8530.h>

/* debug */
#define	SCC_READ_REG(a,b,c,d)	{scc_read_reg(a,b,c,d);delay(10);}
#define	SCC_WRITE_REG(a,b,c,d)	{scc_write_reg(a,b,c,d);delay(10);}


/*
 * On the 3min keyboard and mouse come in on channels A
 * of the two units.  The MI code expects them at 'lines'
 * 0 and 1, respectively.  So we map here back and forth.
 */

#define	SCC_KBDUNIT	1
#define	SCC_PTRUNIT	0

mi_to_scc(unitp, linep)
	int	*unitp, *linep;
{
	/* only play games on MI 'unit' 0 */
	if (*unitp)
		return;

	/* always get unit=0 (console) and line = 0|1 */
	if (*linep == SCREEN_LINE_KEYBOARD) {
		*unitp = SCC_KBDUNIT;
		*linep = SCC_CHANNEL_A;
	} else if (*linep == SCREEN_LINE_POINTER) {
		*unitp = SCC_PTRUNIT;
		*linep = SCC_CHANNEL_A;
	} else {
		gimmeabreak();/* cannot handle yet */
		*unitp = (*linep & 1);
		*linep = SCC_CHANNEL_B;
	}
/* line 0 is channel B, line 1 is channel A */
}

/* only care for mapping to ttyno */
scc_to_mi(sccunit, sccline)
{
	if (sccunit > 1)
		return (sccunit * NSCC_LINE + sccline);
	/* only for console (first pair of SCCs): */
	if (sccline == SCC_CHANNEL_A)
		return ((!sccunit) & 1);
	return 2+sccunit;
}


/*
 * Driver status
 */
struct scc_softc {
	scc_regmap_t	*regs;
	unsigned short	breaks;
	unsigned short	fake;	/* missing rs232 bits, channel A */
	char		polling_mode;
	char		softCAR, osoftCAR;
	char		probed_once;

	/* software copy of some write regs, for reg |= */
	struct softreg {
		unsigned char	wr1;
		unsigned char	wr4;
		unsigned char	wr5;
		unsigned char	wr14;
		unsigned char	wr15;
	} softr[2];	/* per channel */
} scc_softc_data[NSCC];

typedef struct scc_softc *scc_softc_t;

scc_softc_t	scc_softc[NSCC];

scc_softCAR(unit, line, on)
{
	mi_to_scc(&unit, &line);
	if (on)
		scc_softc[unit]->softCAR |= 1<<line;
	else
		scc_softc[unit]->softCAR &= ~(1 << line);
}


/* Speed selections with Pclk=7.3728Mhz, clock x16 */
static
short	scc_speeds[] =
	/* 0   50    75    110  134.5  150  200   300  600 1200 1800 2400 */
	{  0, 4606, 3070, 2093, 1711, 1534, 1150, 766, 382, 190, 126, 94,

	/* 4800 9600 19.2k 38.4k */
	  46,   22,  10,    4};

/*
 * Definition of the driver for the auto-configuration program.
 */

int	scc_probe(), scc_attach(), scc_intr();

caddr_t	scc_std[NSCC] = { 0 };
struct	bus_device *scc_info[NSCC];
struct	bus_driver scc_driver = 
        { scc_probe, 0, scc_attach, 0, scc_std, "scc", scc_info,};

/*
 * Adapt/Probe/Attach functions
 */
static boolean_t 	scc_full_modem = TRUE;
boolean_t		scc_uses_modem_control = FALSE;/* patch this with adb */

int SCC = 1;
#define SCCDEBUG SCC

set_scc_address( sccunit, regs, has_modem)
	caddr_t		regs;
	boolean_t	has_modem;
{
	extern int	scc_probe(), scc_param(), scc_start(),
			scc_putc(), scc_getc(),
			scc_pollc(), scc_mctl(), scc_softCAR();

	scc_std[sccunit] = regs;
	scc_full_modem = has_modem & scc_uses_modem_control;

/* for now, no comm line support */
	rcline = 0;

	/* Do this here */
	console_probe		= scc_probe;
	console_param		= scc_param;
	console_start		= scc_start;
	console_putc		= scc_putc;
	console_getc		= scc_getc;
	console_pollc		= scc_pollc;
	console_mctl		= scc_mctl;
	console_softCAR		= scc_softCAR;

}

scc_probe( xxx, ui)
	struct bus_device *ui;
{
	int             sccunit = ui->unit;
	scc_softc_t     scc;
	register int	val;
	register scc_regmap_t	*regs;

	regs = (scc_regmap_t *)scc_std[sccunit];
	if (regs == 0)
		return 0;

	/*
	 * See if we are here
	 */
	if (check_memory(regs, 0)) {
		/* no rides today */
		return 0;
	}

	scc = &scc_softc_data[sccunit];

	if (scc->probed_once++)
		return 1;

	/*
	 * Chip once-only initialization
	 *
	 * NOTE: The wiring we assume is the one on the 3min:
	 *
	 *	out	A-TxD	-->	TxD	keybd or mouse
	 *	in	A-RxD	-->	RxD	keybd or mouse
	 *	out	A-DTR~	-->	DTR	comm
	 *	out	A-RTS~	-->	RTS	comm
	 *	in	A-CTS~	-->	SI	comm
	 *	in	A-DCD~	-->	RI	comm
	 *	in	A-SYNCH~-->	DSR	comm
	 *	out	B-TxD	-->	TxD	comm
	 *	in	B-RxD	-->	RxD	comm
	 *	in	B-RxC	-->	TRxCB	comm		(ignored?)
	 *	in	B-TxC	-->	RTxCB	comm		(ignored?)
	 *	o	B-RTS~	-->	SS	coutmm
	 *	in	B-CTS~	-->	CTS	comm
	 *	in	B-DCD~	-->	CD	comm
	 */

	scc_softc[sccunit] = scc;
	scc->regs = regs;

	scc->fake = 1<<SCC_CHANNEL_A;

	{ 
		register int i;
		/* We need this in scc_start only, hence the funny
		   value: we need it non-zero and we want to avoid
		   too much overhead in getting to (scc,regs,line) */
		for (i = 0; i < NSCC_LINE; i++)
			console_tty[i+(sccunit*NSCC_LINE)]->t_addr =
				(char*)(0x80000000 + (sccunit<<1) + i);
	}

	/* make sure reg pointer is in known state */
	scc_init_reg(regs, SCC_CHANNEL_A);
	scc_init_reg(regs, SCC_CHANNEL_B);

	/* reset chip, fully */
	SCC_WRITE_REG(regs, SCC_CHANNEL_A, SCC_WR9, SCC_WR9_HW_RESET);
	delay(50000);/*enough ? */
	SCC_WRITE_REG(regs, SCC_CHANNEL_A, SCC_WR9, 0);

	/* program the interrupt vector */
	SCC_WRITE_REG(regs, SCC_CHANNEL_A, SCC_WR2, 0xf0);

	/* most of the init is in scc_param() */

	/* timing base defaults */
	scc->softr[SCC_CHANNEL_A].wr4 = SCC_WR4_CLK_x16;
	scc->softr[SCC_CHANNEL_B].wr4 = SCC_WR4_CLK_x16;

	/* enable DTR, RTS and SS */
	scc->softr[SCC_CHANNEL_B].wr5 = SCC_WR5_RTS;
	scc->softr[SCC_CHANNEL_A].wr5 = SCC_WR5_RTS | SCC_WR5_DTR;

	scc->softr[SCC_CHANNEL_B].wr14 = SCC_WR14_BAUDR_ENABLE | SCC_WR14_DTR_REQUEST;
	scc->softr[SCC_CHANNEL_A].wr14 = SCC_WR14_BAUDR_ENABLE;

	/* interrupt conditions */
	val =	SCC_WR15_BREAK_IE | SCC_WR15_CTS_IE | 
		/*SCC_WR15_SYNCH_IE | */ SCC_WR15_DCD_IE;
	scc->softr[SCC_CHANNEL_A].wr15 = val;
	scc->softr[SCC_CHANNEL_B].wr15 = val;

	val =	SCC_WR1_EXT_IE | /* SCC_WR1_TX_IE | SCC_WR1_PARITY_IE | */
		SCC_WR1_RXI_ALL_CHAR;
	scc->softr[SCC_CHANNEL_A].wr1 = val;
	val = 	SCC_WR1_DMA_ACTLOW | SCC_WR1_DMA_ON_RX_TX | 
		SCC_WR1_EXT_IE | SCC_WR1_PARITY_IE;
	scc->softr[SCC_CHANNEL_B].wr1 = val;

	/*
	 * After probing, any line that should be active
	 * (keybd,mouse,rcline) is activated via scc_param().
	 */

	scc_set_modem_control(scc, scc_full_modem);

#ifdef	KMIN
	/*
	 * Crock: MI code knows of unit 0 as console, we need
	 * unit 1 as well since the keyboard is there
	 */
	if (sccunit == 0) {
		struct bus_device d;
		d = *ui;
		d.unit = 1;
		scc_probe( xxx, &d);
	}
#endif

	return 1;
}

boolean_t scc_timer_started = FALSE;

scc_attach(ui)
	register struct bus_device *ui;
{
	int sccunit = ui->unit;
	extern scc_scan();
	extern int tty_inq_size;
	struct tty	*tp;
	int i;

	/* We only have 4 ttys, but always at 9600
	 * Give em a lot of room (plus dma..)
	 */
	tty_inq_size = 4096;
	if (!scc_timer_started) {
		/* do all of them, before we call scc_scan() */
		for (i = 0; i < NSCC*NSCC_LINE; i++)
			ttychars(console_tty[i]);

		scc_timer_started = TRUE;
		scc_scan();
	}

	tp = console_tty[scc_to_mi(sccunit,SCC_CHANNEL_B)];
	asic_enable_scc_dma(sccunit, &tp->t_inq.c_start, &tp->t_outq.c_start);

#if	NBM > 0
	if (SCREEN_ISA_CONSOLE()) {
		printf("\n sl0: not yet working");
		if (sccunit && rcline == 3) printf("( rconsole )");

		if (sccunit == SCC_KBDUNIT) {
			printf("\n sl1: "); lk201_attach(0, sccunit >> 1);
		} else if (sccunit == SCC_PTRUNIT) {
			printf("\n sl1: "); mouse_attach(0, sccunit >> 1);
		}
	} else {
#endif	/*NBM > 0*/
		printf("%s", (sccunit == 1) ?
			"\n sl0: ( alternate console )\n sl1:" :
			"\n sl0:\n sl1:");
#if	NBM > 0
	}
#endif
}

/*
 * Would you like to make a phone call ?
 */
scc_set_modem_control(scc, on)
	scc_softc_t      scc;
	boolean_t	on;
{
	if (on)
		/* your problem if the hardware then is broke */
		scc->fake = 0;
	else
		scc->fake = 3;
}

/*
 * Polled I/O (debugger)
 */
scc_pollc(unit, on)
	boolean_t		on;
{
	scc_softc_t		scc;
	int			line = SCREEN_LINE_KEYBOARD,
				sccunit = unit;

	mi_to_scc(&sccunit, &line);

	scc = scc_softc[sccunit];
	if (on) {
		scc->polling_mode++;
#if	NBM > 0
		screen_on_off(unit, TRUE);
#endif	NBM > 0
	} else
		scc->polling_mode--;
}

/*
 * Interrupt routine
 */
int scc_intr_count;

scc_intr(unit,spllevel)
{
	scc_softc_t		scc = scc_softc[unit];
	register scc_regmap_t	*regs = scc->regs;
	register int		rr3, rr1;

scc_intr_count++;
	SCC_READ_REG(regs, SCC_CHANNEL_A, SCC_RR3, rr3);

	if (rr3 & SCC_RR3_TX_IP_A) {
		register int	c;

		SCC_WRITE_REG(regs, SCC_CHANNEL_A, SCC_RR0, SCC_RESET_TX_IP);
		c = cons_simple_tint(scc_to_mi(unit,SCC_CHANNEL_A));

		if (c == -1) {
			/* no more data for this line */

			c = scc->softr[SCC_CHANNEL_A].wr15 & ~SCC_WR15_TX_UNDERRUN_IE;
			SCC_WRITE_REG(regs, SCC_CHANNEL_A, SCC_WR15, c);
			scc->softr[SCC_CHANNEL_A].wr15 = c;

			c = scc->softr[SCC_CHANNEL_A].wr1 & ~SCC_WR1_TX_IE;
			SCC_WRITE_REG(regs, SCC_CHANNEL_A, SCC_WR1, c);
			scc->softr[SCC_CHANNEL_A].wr1 = c;
		} else {
			scc_write_data(regs, SCC_CHANNEL_A, c);
			/* and leave it enabled */
		}
	}

	if (rr3 & SCC_RR3_TX_IP_B)
		gimmeabreak();

	if (rr3 & SCC_RR3_EXT_IP_A) {
		gimmeabreak();
		SCC_WRITE_REG(regs, SCC_CHANNEL_A, SCC_RR0, SCC_RESET_EXT_IP);
	}

	if (rr3 & SCC_RR3_EXT_IP_B) {
		gimmeabreak();
		SCC_WRITE_REG(regs, SCC_CHANNEL_B, SCC_RR0, SCC_RESET_EXT_IP);
	}

	if (scc->polling_mode) {
		return;
	}

	if (rr3 & SCC_RR3_RX_IP_A) {
		int c, s, err = 0;

		SCC_READ_REG(regs, SCC_CHANNEL_A, SCC_RR1, rr1);
		scc_read_data(regs, SCC_CHANNEL_A, c);
		s = spltty();/* lowering */
		if (rr1 & (SCC_RR1_PARITY_ERR | SCC_RR1_RX_OVERRUN | SCC_RR1_FRAME_ERR)) {
			/* map to CONS_ERR_xxx MI error codes */
			err =	((rr1 & SCC_RR1_PARITY_ERR)<<8) |
				((rr1 & SCC_RR1_RX_OVERRUN)<<9) |
				((rr1 & SCC_RR1_FRAME_ERR)<<7);
			SCC_WRITE_REG(regs, SCC_CHANNEL_A,SCC_RR0, SCC_RESET_ERROR);
		}
		rr1 = SCREEN_LINE_OTHER;
		if (unit == SCC_KBDUNIT) rr1 = SCREEN_LINE_KEYBOARD;
		if (unit == SCC_PTRUNIT) rr1 = SCREEN_LINE_POINTER;
		cons_simple_rint(scc_to_mi(unit,SCC_CHANNEL_A), rr1, c, err);
		splx(s);
	}

	if (rr3 & SCC_RR3_RX_IP_B)
		gimmeabreak();

}

boolean_t
scc_start(tp)
	struct tty *tp;
{
	register scc_regmap_t	*regs;
	register int		line, temp;
	register struct softreg	*sr;

	temp = (int)tp->t_addr;
	line = temp & 1;	/* channel */
	temp = (temp >> 1)&0xff;/* sccunit */
	regs = scc_softc[temp]->regs;
	sr   = &scc_softc[temp]->softr[line];

	temp = sr->wr15 | SCC_WR15_TX_UNDERRUN_IE;
	SCC_WRITE_REG(regs, line, SCC_WR15, temp);
	sr->wr15 = temp;

	temp = sr->wr1 | SCC_WR1_TX_IE;
	SCC_WRITE_REG(regs, line, SCC_WR1, temp);
	sr->wr1 = temp;

	/* yes, we need a char out first */
	return TRUE;
}

/*
 * Get a char from a specific SCC line
 * [this is only used for console&screen purposes]
 */
scc_getc( unit, line, wait, raw )
	boolean_t	wait;
	boolean_t	raw;
{
	scc_softc_t     scc;
/*	int             s = spltty();*/
	register scc_regmap_t *regs;
	unsigned char   c;
	int             value;

	value = line;
	mi_to_scc(&unit, &value);

/* NOTE: when handled, rcline should be polled too in KEYBD case */

	scc = scc_softc[unit];
	regs = scc->regs;

	/*
	 * wait till something available
	 */
again:
	while (1) {
		scc_read_reg_zero(regs, SCC_CHANNEL_A, value);
		if (((value & SCC_RR0_RX_AVAIL) == 0) && wait)
			delay(10);
		else
			break;
	}

	/*
	 * if nothing found return -1 
	 */
	if (value & SCC_RR0_RX_AVAIL) {
		scc_read_reg(regs, SCC_CHANNEL_A, SCC_RR1, value);
		scc_read_data(regs, SCC_CHANNEL_A, c);
	} else {
/*		splx(s);*/
		return -1;
	}

	/*
	 * bad chars not ok 
	 */
	if (value&(SCC_RR1_PARITY_ERR | SCC_RR1_RX_OVERRUN | SCC_RR1_FRAME_ERR)) {
scc_state(unit);
		scc_write_reg(regs, SCC_CHANNEL_A, SCC_RR0, SCC_RESET_ERROR);
		if (wait)
			goto again;
	}

/*	splx(s);*/


#if	NBM > 0
	if ((line == SCREEN_LINE_KEYBOARD) && !raw && SCREEN_ISA_CONSOLE())
		return lk201_rint(SCREEN_CONS_UNIT(), c, wait, scc->polling_mode);
	else
#endif	NBM > 0
		return c;
}

/*
 * Put a char on a specific SCC line
 */
scc_putc( unit, line, c )
{
	scc_softc_t      scc;
	register scc_regmap_t *regs;
	int             s = spltty();
	register int	value;

	mi_to_scc(&unit, &line);

	scc = scc_softc[unit];
	regs = scc->regs;

	do {
		scc_read_reg(regs, SCC_CHANNEL_A, SCC_RR0, value);
		if (value & SCC_RR0_TX_EMPTY)
			break;
		delay(100);
	} while (1);

/*	SCC_WRITE_REG(regs, SCC_CHANNEL_A, SCC_WR8, c);*/
	scc_write_data(regs, SCC_CHANNEL_A, c);
/* wait for it to swallow the char ? */

	splx(s);
}

scc_param(tp, line)
	struct tty	*tp;
{
	scc_regmap_t	*regs;
	int		value, sccline, unit;
	struct softreg	*sr;
	scc_softc_t	scc;
 
	unit = line >> 2;	/* XXXXXXXXXXXXXXX */
	sccline = line;
	mi_to_scc(&unit, &sccline);

	scc = scc_softc[unit];
	regs = scc->regs;

	sr = &scc->softr[sccline];

	/*
	 * Do not let user fool around with kbd&mouse
	 */
#if	NBM > 0
	if (screen_captures(line)) {
		tp->t_ispeed = tp->t_ospeed = B4800;
		tp->t_flags |= TF_LITOUT;
	}
#endif	NBM > 0

	if (tp->t_ispeed == 0) {
		(void) scc_mctl(tp->t_dev, TM_HUP, DMSET);	/* hang up line */
		return;
	}

	/* reset line */
	value = (sccline == SCC_CHANNEL_A) ? SCC_WR9_RESET_CHA_A : SCC_WR9_RESET_CHA_B;
	SCC_WRITE_REG(regs, sccline, SCC_WR9, value);
	delay(25);

	/* stop bits, normally 1 */
	value = sr->wr4 & 0xf0;
	value |= (tp->t_ispeed == B110) ? SCC_WR4_2_STOP : SCC_WR4_1_STOP;
	/* .. and parity */
#if 0
	if ((tp->t_flags & (TF_ODDP | TF_EVENP)) == TF_EVENP)
		value |= SCC_WR4_EVEN_PARITY;
	value |= SCC_WR4_PARITY_ENABLE;
#else
	if ((tp->t_flags & (TF_ODDP | TF_EVENP)) == TF_ODDP)
		value |= SCC_WR4_PARITY_ENABLE;
#endif

	/* set it now */
	sr->wr4 = value;
	SCC_WRITE_REG(regs, sccline, SCC_WR4, value);

	value = sr->wr1;
	SCC_WRITE_REG(regs, sccline, SCC_WR1, value);

	SCC_WRITE_REG(regs, sccline, SCC_WR2, 0xf0);

	/* we only do 8 bits per char */
	value = SCC_WR3_RX_8_BITS;
	SCC_WRITE_REG(regs, sccline, SCC_WR3, value);

#if 0
	value = sr->wr5 & (~0x6);/*bit-per-char only*/
	value |= SCC_WR5_TX_8_BITS | SCC_WR5_TX_ENABLE;
#else
	value = SCC_WR5_TX_8_BITS;
#endif
	sr->wr5 = value;
	SCC_WRITE_REG(regs, sccline, SCC_WR5, value);

	SCC_WRITE_REG(regs, sccline, SCC_WR9, 0);

	SCC_WRITE_REG(regs, sccline, SCC_WR10, SCC_WR10_NRZ);

	/* speed selection */
	value = SCC_WR11_TRcOUT_XMTCLK | SCC_WR11_TRc_OUT |
		SCC_WR11_RCLK_BAUDR | SCC_WR11_XTLK_BAUDR;
	SCC_WRITE_REG(regs, sccline, SCC_WR11, value);

	value = scc_speeds[tp->t_ispeed];
	scc_set_timing_base(regs,sccline,value);

#if 0
	value = sr->wr14;
#else
	value = SCC_WR14_BAUDR_SRC;
#endif
	SCC_WRITE_REG(regs, sccline, SCC_WR14, value);

	value = sr->wr15;
	SCC_WRITE_REG(regs, sccline, SCC_WR15, value);

	value = SCC_WR14_BAUDR_SRC | SCC_WR14_BAUDR_ENABLE;
	SCC_WRITE_REG(regs, sccline, SCC_WR14, value);

	value = SCC_WR3_RX_8_BITS | SCC_WR3_RX_ENABLE;
	SCC_WRITE_REG(regs, sccline, SCC_WR3, value);

	value = SCC_WR5_TX_8_BITS | SCC_WR5_TX_ENABLE;
	sr->wr5 = value;
	SCC_WRITE_REG(regs, sccline, SCC_WR5, value);

	/* master inter enable */
	SCC_WRITE_REG(regs,sccline,SCC_WR9,SCC_WR9_MASTER_IE);

}
 
/*
 * Modem control functions
 */
scc_mctl(dev, bits, how)
	dev_t dev;
	int bits, how;
{
	return 0;
#if 0
	register scc_regmap_t *regs;
	register int unit;
	register int tcr, msr, brk, n_tcr, n_brk;
	int b, s;
	scc_softc_t      sc;

	unit = minor(dev);

	/* no modem support on lines 0 & 1 */
/* XXX break on 0&1 */
	if ((unit & 2) == 0)
		return TM_LE | TM_DTR | TM_CTS | TM_CAR | TM_DSR;

	b = 1 ^ (unit & 1);	/* line 2 ? */
	
	scc = scc_softc[unit>>2];
	regs = scc->regs;
	s = spltty();

	tcr = ((regs->scc_tcr | (scc->fake>>4)) & 0xf00) >> (8 + b*2);
	brk = (scc->breaks >> (8 + (unit&3))) & 1;	/* THE break bit */

	n_tcr = (bits & (TM_RTS | TM_DTR)) >> 1;
	n_brk = (bits & TM_BRK) >> 9;

	/* break transitions, must 'send' a char out */
	bits = (brk ^ n_brk) & 1;

	switch (how) {
	case DMSET:
		tcr = n_tcr;
		brk = n_brk;
		break;

	case DMBIS:
		tcr |= n_tcr;
		brk |= n_brk;
		break;

	case DMBIC:
		tcr &= ~n_tcr;
		brk = 0;
		break;

	case DMGET:
		msr = ((regs->scc_msr | scc->fake) & 0xf0f) >> (b*8);
		(void) splx(s);
		return  (tcr<<1)|/* DTR, RTS */
			((msr&1)<<5)|/* CTS */
			((msr&2)<<7)|/* DSR */
			((msr&0xc)<<4)|/* CD, RNG */
			(brk << 9)|/* BRK */
			TM_LE;
	}
	n_tcr =	(regs->scc_tcr & ~(3 << (8 + b*2))) |
		(tcr << (8 + b*2));

	regs->scc_tcr = n_tcr;
	scc->fake = (scc->fake & 0xf0f) | (n_tcr<<4&0xf000);

	scc->breaks = (scc->breaks & ~(1 << (8 + (unit&3)))) |
		    (brk << (8 + (unit&3)));
	if(bits) scc_putc( unit>>2, unit&3, 0);/* force break, now */
	(void) splx(s);
	return 0;/* useless to compute it */
#endif
}

/*
 * Periodically look at the CD signals:
 * they do generate interrupts but we
 * must fake them on channel A.  We might
 * also fake them on channel B.
 */
scc_scan()
{
	register i, s = spltty();

	for (i = 0; i < NSCC; i++) {
		register scc_softc_t	scc;
		register int		car;
		register struct tty	**tpp;

		scc = scc_softc[i];
		if (scc == 0)
			continue;
		car = scc->softCAR | scc->fake;

		tpp = &console_tty[i * NSCC_LINE];

		while (car) {
			if (car & 1)
				check_car(*tpp, 1);
			tpp++;
			car  = car>>1;
		}

	}
	splx(s);
	timeout(scc_scan, (caddr_t)0, 5*hz);
}

static check_car(tp, car)
	register struct tty *tp;
{
	if (car) {
#if notyet
		/* cancel modem timeout if need to */
		if (car & (SCC_MSR_CD2 | SCC_MSR_CD3))
			untimeout(scc_hup, (caddr_t)tp);
#endif

		/* I think this belongs in the MI code */
		if (tp->t_state & TS_WOPEN)
			tp->t_state |= TS_ISOPEN;
		/* carrier present */
		if ((tp->t_state & TS_CARR_ON) == 0)
			(void)ttymodem(tp, 1);
	} else if ((tp->t_state&TS_CARR_ON) && ttymodem(tp, 0) == 0)
		scc_mctl( tp->t_dev, TM_DTR, DMBIC);
}

#if 1 /* debug */
scc_rreg(unit,n)
{
	int val;
	SCC_READ_REG(scc_softc[unit]->regs, 1, n, val);
	return val;
}

scc_wreg(unit,n,val)
{
	SCC_WRITE_REG(scc_softc[unit]->regs, 1, n, val);
}

scc_state(unit)
{
	printf("{%d intr, R0 %x R1 %x R2 %x R3 %x R10 %x baudr %x R15 %x\n",
		scc_intr_count,
		scc_rreg(unit, SCC_RR0),
		scc_rreg(unit, SCC_RR1),
		scc_rreg(unit, SCC_RR2),
		scc_rreg(unit, SCC_RR3),
		scc_rreg(unit, SCC_RR10),
		(scc_rreg(unit, SCC_RR13) << 8) | scc_rreg(unit, SCC_RR12),
		scc_rreg(unit, SCC_RR15));
}

#endif

#endif	NSCC > 0
