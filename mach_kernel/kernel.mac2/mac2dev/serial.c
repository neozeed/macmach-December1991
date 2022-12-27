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

/* MacMach driver for modem and printer serial ports
 *
 * Create by Zonnie L. Williamson at CMU, 1991
 *
 * This driver combines the MACH generic tty interface with the Macintosh
 * serial drivers.  The Macintosh provides 1/1.5/2 stop bits, even/odd/no
 * parity and 300/600/1200/1800/2400/3600/4800/7200/9600/19200/57600 baud.
 * The supported control/status lines are CTS input and DTR output.  The
 * Macintosh serial driver can handle CTS or XON/XOFF handshake.  The driver
 * can detect software overrun, parity, hardware overrun and framing errors.
 * Chapter 9 in Inside Macintosh, II-243 describes the serial drivers.
 *
 * The MACH generic tty interface provides everything that a Unix server
 * wants to have in a tty device.  It maintains input and output queues and
 * contains the glue that makes a well behaved MACH device.  It assumes that
 * interrupt handlers are active to produce the input and consume the output.
 * Macintosh asynchronous reads and writes are used to simulate interrupt
 * behavior.
 *
 * It is assumed that the Macintosh RAM versions of the serial drivers have
 * already been loaded and that generous input buffers have been established.
 * These things must be done BEFORE the MACH kernel boots.
 *
 * This driver exports the following functions:
 *   serial_open       -- open serial port
 *   serial_close      -- close serial port
 *   serial_read       -- read from serial port
 *   serial_write      -- write to serial port
 *   serial_getstat    -- get serial port status
 *   serial_setstat    -- set serial port status
 *   serial_portdeath  -- serial device portdeath
 *
 * serial_open        -- open serial port
 *   1) check for valid port number (this is the only place it is checked)
 *   2) if TS_ISOPEN and TS_WOPEN are not set, then setup serial port
 *      a) set completion routines and reference number, when the port is
 *         closed, they are zeroed to help detect "impossible" errors
 *      b) allocate input and output buffers (this is only done once)
 *      c) allocate input, output and control parameter blocks (done once)
 *      d) set default baud rate, char size, etc.
 *      e) disable Xon/Xoff and CTS handshaking
 *      f) flush input and output buffers
 *      g) turn on DTR
 *      h) setup for MACH generic tty layer
 *   3) if TS_ISOPEN bit not set yet, check for carrier
 *      ??? There doesn't seem to be any way to read CTS status to detect
 *      carrier.  Perhaps the CTS handshake can be temporarily turned on and
 *      the handshake status inspected.
 *   4) finish up by calling the MACH generic tty open function
 *
 * serial_close       -- close serial port
 *   1) flush output and input buffers
 *   2) turn of DTR if TS_HUPCLS is set
 *   3) zero completion routines and reference numbers
 *   4) finish by calling the MACH generic tty close function
 *   5) always return D_SUCCESS
 *
 * serial_read        -- read from serial port
 *   1) find out how many chars are ready in the internal input buffer
 *   2) normalize: 1 <= count <= SERIAL_BUFFER_SIZE
 *   3) set up asynchronous read into input buffer
 *   4) finish by calling the MACH generic tty read function which will
 *      enter a queue to wait for completion of the asynchronous read
 *
 * serial_write       -- write to serial port
 *   1) call the MACH generic tty write function which will eventually
 *      force the serial_start function to be called
 *
 * serial_getstat     -- get serial port status
 *   1) if TTY_MODEM, check CTS bit, there doesn't seem to be any way
 *      to do this
 *   2) otherwise do MACH generic tty_get_status
 *
 * serial_setstat     -- set serial port status
 *   1) if TTY_SET_BREAK or TTY_CLEAR_BREAK, do it
 *   2) if TTY_MODEM, turn DTR on or off
 *   3) otherwise, do MACH generic tty_set_status
 *   4) if TTY_STATUS, set baud and bits
 *
 * serial_portdeath   -- serial device portdeath
 *   1) do MACH generic tty_portdeath
 *
 * modem_read_done    -- low level completion routine for port 0 read
 *   if not MODE24, call serial_read_done for modem port
 *   1) enter 32-bit addressing mode
 *   2) call serial_read_done with argument 0
 *   3) return to 24-bit addressing mode
 *
 * printer_read_done  -- low level completion routine for port 1 read
 *   if not MODE24, call serial_read_done for printer port
 *   1) enter 32-bit addressing mode
 *   2) call serial_read_done with argument 1
 *   3) return to 24-bit addressing mode
 *
 * modem_write_done   -- low level completion routine for port 0 write
 *   if not MODE24, call serial_write_done for modem port
 *   1) enter 32-bit addressing mode
 *   2) call serial_write_done with argument 0
 *   3) return to 24-bit addressing mode
 *
 * printer_write_done -- low level completion routine for port 1 write
 *   if not MODE24, call serial_write_done for printer port
 *   1) enter 32-bit addressing mode
 *   2) call serial_write_done with argument 1
 *   3) return to 24-bit addressing mode
 *
 * serial_read_done   -- completion routine for asynchronous read
 *   1) print warning if error was encountered
 *   2) signal MACH generic tty layer if BREAK received
 *   3) if TS_ISOPEN not set, send signal to MACH generic tty layer
 *   4) otherwise, copy data from input buffer to MACH generic tty input queue
 *   5) signal MACH generic tty layer that the read has completed
 *
 * serial_write_done  -- completion routine for asynchronous write
 *   1) print warning if error was encountered
 *   2) clear TS_BUSY bit
 *   3) signal MACH generic tty layer that the write has completed
 *   4) call serial_start to try and send some more
 *
 * serial_start       -- used by MACH generic tty interface to start output
 *   1) if timed out, stopped or busy, do nothing
 *   2) if output queue is getting empty, signal the MACH generic tty layer
 *      that more output is needed
 *   3) if the output queue is empty, do nothing
 *   4) otherwise, copy as much data as will fit from the MACH generic tty
 *      output queue into the output buffer
 *   5) set the TS_BUSY bit
 *   6) start asynchronous write from output buffer
 *
 * serial_stop        -- used by MACH generic tty interface to stop output
 *   1) if TS_BUSY is set, flush the output buffer
 *   2) if D_READ is specified, flush the input buffer
 */

#include <mach/mach_types.h>

#include <device/device_types.h>
#include <device/io_req.h>
#include <device/tty.h>

#include <mac2os/Types.h>
#include <mac2os/Errors.h>
#include <mac2os/Files.h>

/* serial driver reference numbers */
#define AIn_RefNum  -6
#define AOut_RefNum -7
#define BIn_RefNum  -8
#define BOut_RefNum -9

/* bits for SerReset */
#define TWO_STOP_BITS  0xC000
#define SEVEN_BIT_DATA 0x0400
#define EIGHT_BIT_DATA 0x0C00
#define EVEN_PARITY    0x3000
#define ODD_PARITY     0x1000
#define NO_PARITY      0x0000
#define BAUD_300       0x017C
#define BAUD_600       0x00BD
#define BAUD_1200      0x005E
#define BAUD_1800      0x003E
#define BAUD_2400      0x002E
#define BAUD_4800      0x0016
#define BAUD_9600      0x000A

/* PBControl and PBStatus function codes */
#define SerGetBuf   2
#define SerReset    8
#define SerHShake  10
#define SerClrBrk  11
#define SerSetBrk  12
#define SerDTRon   17
#define SerDTRoff  18

/* parameters from SerGetBuf */
typedef struct {
  long count;
} *SerGetBuf_t;

/* parameters to SerHShake */
typedef struct {
  unsigned char fXon;
  unsigned char fCTS;
  char xOn;
  char xOff;
  unsigned char errs;
  unsigned char evts;
  unsigned char fInX;
  unsigned char null;
} *SerHShake_t;

/* parameters to SerReset */
typedef struct {
  unsigned short SerConfig;
} *SerReset_t;

/* each port has input and output buffers allocated with NewPtr() */
#define SERIAL_BUFFER_SIZE 512

static struct serial_port {
  short inRefNum, outRefNum;   /* input and output reference numbers */
  Ptr iBuf, oBuf;              /* input and output buffers */
  ProcPtr readDone, writeDone; /* completion routines */
  IOParam *iPb, *oPb;          /* input and output parameter blocks */
  CntrlParam *cPb;             /* control parameter block */
  int modem_bits;              /* TM_CTS and TM_DTR bits */
  struct tty tty;              /* MACH generic tty information */
} serial_port[2];              /* port 0 is modem, port 1 is printer */

/* convert OSErr to io_return_t */
#define OSErr_io_return_t(err) ((err == noErr) ? D_SUCCESS : D_IO_ERROR)

/* initial baud rate and flags */
#define DEFAULT_BITS (BAUD_2400 | TWO_STOP_BITS | EIGHT_BIT_DATA | NO_PARITY)
#define ISPEED	B2400
/* ??? are the IFLAGS correct ??? */
#define IFLAGS	(EVENP|ODDP|ECHO|CRMOD)

/* the MACH generic tty layer uses this to start output */
/* NOTE: this is sometimes an extension of the interrupt handler */
int serial_start(struct tty *tp)
{
  struct serial_port *p = &serial_port[tp->t_dev];

  /* if timed out, stopped or busy, do nothing */
  if (tp->t_state & (TS_TIMEOUT|TS_TTSTOP|TS_BUSY)) return 0;

  /* if queue getting empty, signal that more output is needed */
  if (tp->t_outq.c_cc <= TTLOWAT(tp)) tt_write_wakeup(tp);

  /* if queue is empty, do nothing */
  if (!tp->t_outq.c_cc) return 0;

  /* get as many chars as possible from MACH generic tty output queue */
  p->oPb->ioReqCount = q_to_b(&tp->t_outq, p->oBuf, SERIAL_BUFFER_SIZE);

  /* set the MACH generic tty "output busy" bit */
  tp->t_state |= TS_BUSY;

  /* let the Macintosh serial output driver finish the job */
  p->oPb->ioCompletion = p->writeDone;
  p->oPb->ioActCount = p->oPb->ioReqCount;
  p->oPb->ioRefNum = p->outRefNum;
  p->oPb->ioBuffer = p->oBuf;
  (void)PBWrite(p->oPb, TRUE);

  return 0;

} /* serial_start() */

/* the MACH generic tty layer uses this to stop output */
/* if flags include D_READ, then the input buffer is also flushed */
int serial_stop(struct tty *tp, int flags)
{
  struct serial_port *p = &serial_port[tp->t_dev];

  /* if buffer is being written, *flush* it */
  if (tp->t_state & TS_BUSY) {

    /* flush Macintosh serial driver output buffer */
    p->cPb->ioCRefNum = p->outRefNum;
    if (PBKillIO(p->cPb, FALSE) != noErr)
      xprintf("serial_stop: port %d output KillIo ioResult is %d",
        tp->t_dev, p->cPb->ioResult);

  }

  /* flush input buffer too, if D_READ specified */
  if (flags & D_READ) {

    /* flush Macintosh serial driver input buffer */
    p->cPb->ioCRefNum = p->inRefNum;
    if (PBKillIO(p->cPb, FALSE) != noErr)
      xprintf("serial_stop: port %d input KillIO ioResult is %d",
        tp->t_dev, p->cPb->ioResult);

  }

  return 0;

} /* serial_stop() */

/* a call to PBRead() has completed */
/* NOTE: this is an extension of the interrupt handler */
#ifdef MODE24
void serial_read_done(register int number)
{
  register struct serial_port *p = &serial_port[number];
#else /* MODE24 */
void serial_read_done(register struct serial_port *p)
{
#endif /* MODE24 */
  register struct tty *tp = &p->tty;
  register int n;

  switch (p->iPb->ioResult) {
    case noErr:
    case abortErr:
      break;
    case breakRecd:
#ifdef notdef
      /* signal the MACH generic tty layer that BREAK has been received */
      (void)ttymodem(tp, 0);
      tty_queue_completion(&tp->t_delayed_read);
      return;
#else
      break;
#endif
    default:
      xprintf("serial_read_done: port %d ioResult is %d",
        p->inRefNum == AIn_RefNum ? 0 : 1, p->iPb->ioResult);
  }

  /* if not open, signal the MACH generic tty layer that it should be */
  if (!(tp->t_state & TS_ISOPEN)) {
    tt_open_wakeup(tp);
    return;
  }

  /* insert chars into MACH generic tty input queue */
  /* no-op if ioActCount is zero, drop chars if input queue is full */
  /* the PBRead() should not be called with ioReqCount too large!! */
  if (p->iPb->ioResult == noErr)
    if (n = b_to_q(p->iBuf, p->iPb->ioActCount, &tp->t_inq))
      xprintf("serial_read_done: port %d dropped %d chars",
         p->inRefNum == AIn_RefNum ? 0 : 1, n);

  /* signal the MACH generic tty layer that something has been read */
  tty_queue_completion(&tp->t_delayed_read);

} /* serial_read_done() */

/* a call to PBWrite() has completed */
/* NOTE: this is an extension of the interrupt handler */
#ifdef MODE24
void serial_write_done(register int number)
{
  register struct serial_port *p = &serial_port[number];
#else /* MODE24 */
void serial_write_done(register struct serial_port *p)
{
#endif /* MODE24 */
  register struct tty *tp = &p->tty;

  /* check the ioResult */
  switch (p->iPb->ioResult) {
    case noErr:
    case abortErr:
    case 1: /* ??? what is ioResult == 1, it sure happens a lot ??? */
      break;
    default:
      xprintf("serial_write_done: port %d ioResult is %d",
        p->inRefNum == AOut_RefNum ? 0 : 1, p->iPb->ioResult);
  }

  /* clear the MACH generic tty "output busy" bit */
  tp->t_state &= ~TS_BUSY;

  /* signal the MACH generic tty layer that something has been written */
  tt_write_wakeup(tp);

  /* try and send some more */
  serial_start(tp);

} /* serial_write_done() */

#ifdef MODE24
/* These routines are in "serial_done.s".  They make sure that
 * serial_read_done() and serial_write_done() are called
 * in 32-bit addressing mode even if the interrupt handler was not.
 */
extern void modem_read_done(); /* calls serial_read_done(0) */
extern void printer_read_done(); /* calls serial_read_done(1) */
extern void modem_write_done(); /* calls serial_write_read_done(0) */
extern void printer_write_done(); /* calls serial_write_done(1) */
#else

static void modem_read_done()
{
  serial_read_done(&serial_port[0]);
}

static void printer_read_done()
{
  serial_read_done(&serial_port[1]);
}

static void modem_write_done()
{
  serial_write_done(&serial_port[0]);
}

static void printer_write_done()
{
  serial_write_done(&serial_port[1]);
}

#endif

/* MACH device getstat for serial port */
/* assume that the number is valid and that the serial port is open */
io_return_t serial_getstat(int number, int flavor, int *data, int *count)
{
  struct serial_port *p = &serial_port[number];
  struct tty *tp = &p->tty;

  /* if TTY_MODEM, get the status bits */
  if (flavor == TTY_MODEM) {
    /* there doesn't seem to be any way to read the CTS bit ??? */
    p->modem_bits |= TM_CTS;
    data = &p->modem_bits;
    *count = TTY_MODEM_COUNT;
    return D_SUCCESS;
  }

  /* otherwise do MACH generic tty_get_status() */
  return tty_get_status(&serial_port->tty, flavor, data, count);

} /* serial_getstat() */

/* MACH device setstat for serial port */
/* assume that the number is valid and that the serial port is open */
io_return_t serial_setstat(int number, int flavor, int *data, int count)
{
  struct serial_port *p = &serial_port[number];
  struct tty *tp = &p->tty;
  io_return_t result;
  unsigned short bits;

  switch (flavor) {

    case TTY_SET_BREAK:
      p->cPb->ioCRefNum = p->outRefNum;
      p->cPb->csCode = SerSetBrk;
      return OSErr_io_return_t(PBControl(p->cPb, FALSE));

    case TTY_CLEAR_BREAK:
      p->cPb->ioCRefNum = p->outRefNum;
      p->cPb->csCode = SerClrBrk;
      return OSErr_io_return_t(PBControl(p->cPb, FALSE));

    case TTY_MODEM:
      if (count != TTY_MODEM_COUNT) return D_IO_ERROR;
      if ((p->modem_bits & TM_DTR) != (*data & TM_DTR)) {
        if (*data & TM_DTR) {
          p->modem_bits |= TM_DTR;
          p->cPb->ioCRefNum = p->outRefNum;
          p->cPb->csCode = SerDTRon;
          return OSErr_io_return_t(PBControl(p->cPb, FALSE));
        }
        else {
          p->modem_bits &= ~TM_DTR;
          p->cPb->ioCRefNum = p->outRefNum;
          p->cPb->csCode = SerDTRoff;
          return OSErr_io_return_t(PBControl(p->cPb, FALSE));
        }
      }
      else return D_SUCCESS;

  }

  /* otherwise, do MACH generic tty_set_status() */
  result = tty_set_status(tp, flavor, data, count);
  if (result != D_SUCCESS || flavor != TTY_STATUS) return result;

  /* if it was a TTY_STATUS call, set baud and bits */
  if (tp->t_ispeed != tp->t_ospeed) return D_IO_ERROR;
  switch (tp->t_ispeed) {
    case B300:  bits = BAUD_300; break;
    case B600:  bits = BAUD_600; break;
    case B1200: bits = BAUD_1200; break;
    case B1800: bits = BAUD_1800; break;
    case B2400: bits = BAUD_2400; break;
    case B4800: bits = BAUD_4800; break;
    case B9600: bits = BAUD_9600; break;
    default:    return D_IO_ERROR;
  }
  if (tp->t_flags & TF_LITOUT) bits |= EIGHT_BIT_DATA;
  else bits |= SEVEN_BIT_DATA;
  if (tp->t_flags & TF_EVENP) bits |= EVEN_PARITY;
  else if (tp->t_flags & TF_ODDP) bits |= ODD_PARITY;
  bits |= TWO_STOP_BITS;

  ((SerReset_t)p->cPb->csParam)->SerConfig = bits;
  p->cPb->ioCRefNum = p->outRefNum;
  p->cPb->csCode = SerReset;
  return OSErr_io_return_t(PBControl(p->cPb, FALSE));

} /* serial_setstat() */

/* MACH device close for serial port */
/* assume that the number is valid and that the serial port is open */
io_return_t serial_close(int number)
{
  struct serial_port *p = &serial_port[number];
  struct tty *tp = &serial_port->tty;

  /* flush output buffers */
  if (p->cPb && p->outRefNum) {
    p->cPb->ioCRefNum = p->outRefNum;
    if (PBKillIO(p->cPb, FALSE) != noErr)
      xprintf("serial_close: port %d output KillIO ioResult is %d",
        number, p->cPb->ioResult);
  }

  /* flush input buffers */
  if (p->cPb && p->inRefNum) {
    p->cPb->ioCRefNum = p->inRefNum;
    if (PBKillIO(p->cPb, FALSE) != noErr)
      xprintf("serial_close: port %d input KillIO ioResult is %d",
        number, p->cPb->ioResult);
  }

  /* hangup the modem if TS_HUPCLS bit set */
  if (p->cPb && p->outRefNum && (tp->t_state & TS_HUPCLS)) {
    p->modem_bits &= ~TM_DTR;
    p->cPb->ioCRefNum = p->outRefNum;
    p->cPb->csCode = SerDTRoff;
    if (PBControl(p->cPb, FALSE) != noErr)
      xprintf("serial_close: port %d output SerDTRoff ioResult is %d",
        number, p->cPb->ioResult);
  }

  /* clear completion routines and reference numbers to catch any errors */
  p->readDone = 0;
  p->writeDone = 0;
  p->outRefNum = 0;
  p->inRefNum = 0;

  /* flush the MACH generic tty queues, clear the TS_ISOPEN bit */
  if (tp->t_state & (TS_ISOPEN | TS_WOPEN)) ttyclose(tp);

  /* all done, ignore any errors */
  return D_SUCCESS;

} /* serial_close() */

/* MACH device open for serial port */
io_return_t serial_open(int number, dev_mode_t mode, io_req_t ior)
{
  struct serial_port *p;
  struct tty *tp;
  io_return_t result;
  OSErr err;

  /* valid port numbers are 0 (modem) and 1 (printer) */
  if (number & ~1) return D_NO_SUCH_DEVICE;

  /* get a pointer to the structure for this port */
  p = &serial_port[number];
  tp = &p->tty;

  /* if not open, open it */
  if ((tp->t_state & (TS_ISOPEN | TS_WOPEN)) == 0) {

    /* note completion routines and reference numbers */
    p->readDone = number ? printer_read_done : modem_read_done;
    p->writeDone = number ? printer_write_done : modem_write_done;
    p->outRefNum = number ? BOut_RefNum : AOut_RefNum;
    p->inRefNum = number ? BIn_RefNum : AIn_RefNum;

    /* allocate input buffer */
    if (!p->iBuf && !(p->iBuf = (Ptr)NewPtr(SERIAL_BUFFER_SIZE))) {
      xprintf("serial_open: port %d iBuf NewPtr(%d) failed",
        number, SERIAL_BUFFER_SIZE);
      result = D_NO_MEMORY;
      goto error;
    }

    /* allocate output buffer */
    if (!p->oBuf && !(p->oBuf = (Ptr)NewPtr(SERIAL_BUFFER_SIZE))) {
      xprintf("serial_open: port %d oBuf NewPtr(%d) failed",
        number, SERIAL_BUFFER_SIZE);
      result = D_NO_MEMORY;
      goto error;
    }

    /* allocate control parameter block */
    if (!p->cPb && !(p->cPb = (CntrlParam *)NewPtr(sizeof(CntrlParam)))) {
      xprintf("serial_open: port %d cPb NewPtr(%d) failed",
        number, sizeof(CntrlParam));
      result = D_NO_MEMORY;
      goto error;
    }

    /* allocate input parameter block */
    if (!p->iPb && !(p->iPb = (IOParam *)NewPtr(sizeof(IOParam)))) {
      xprintf("serial_open: port %d iPb NewPtr(%d) failed",
        number, sizeof(IOParam));
      result = D_NO_MEMORY;
      goto error;
    }

    /* allocate output parameter block */
    if (!p->oPb && !(p->oPb = (IOParam *)NewPtr(sizeof(IOParam)))) {
      xprintf("serial_open: port %d oPb NewPtr(%d) failed",
        number, sizeof(IOParam));
      result = D_NO_MEMORY;
      goto error;
    }

    /* set default input parameters */
    bzero(p->cPb, sizeof(CntrlParam));
    ((SerReset_t)p->cPb->csParam)->SerConfig = DEFAULT_BITS;
    p->cPb->ioCRefNum = p->inRefNum;
    p->cPb->csCode = SerReset;
    if ((result = OSErr_io_return_t(PBControl(p->cPb, FALSE))) != D_SUCCESS) {
      xprintf("serial_open: port %d input SerReset ioResult is %d",
        number, p->cPb->ioResult);
      goto error;
    }

    /* don't do any handshaking on input */
    ((SerHShake_t)p->cPb->csParam)->fXon = 0;
    ((SerHShake_t)p->cPb->csParam)->fCTS = 0;
    ((SerHShake_t)p->cPb->csParam)->xOn = 0;
    ((SerHShake_t)p->cPb->csParam)->xOff = 0;
    ((SerHShake_t)p->cPb->csParam)->errs = 0;
    ((SerHShake_t)p->cPb->csParam)->evts = 0;
    ((SerHShake_t)p->cPb->csParam)->fInX = 0;
    ((SerHShake_t)p->cPb->csParam)->null = 0;
    p->cPb->ioCRefNum = p->inRefNum;
    p->cPb->csCode = SerHShake;
    if ((result = OSErr_io_return_t(PBControl(p->cPb, FALSE))) != D_SUCCESS) {
      xprintf("serial_open: port %d input SerHShake ioResult is %d",
        number, p->cPb->ioResult);
      goto error;
    }

    /* set default output parameters */
    ((SerReset_t)p->cPb->csParam)->SerConfig = DEFAULT_BITS;
    p->cPb->ioCRefNum = p->outRefNum;
    p->cPb->csCode = SerReset;
    if ((result = OSErr_io_return_t(PBControl(p->cPb, FALSE))) != D_SUCCESS) {
      xprintf("serial_open: port %d output SerReset ioResult is %d",
        number, p->cPb->ioResult);
      goto error;
    }

    /* don't do any handshaking on output */
    ((SerHShake_t)p->cPb->csParam)->fXon = 0;
    ((SerHShake_t)p->cPb->csParam)->fCTS = 0;
    ((SerHShake_t)p->cPb->csParam)->xOn = 0;
    ((SerHShake_t)p->cPb->csParam)->xOff = 0;
    ((SerHShake_t)p->cPb->csParam)->errs = 0;
    ((SerHShake_t)p->cPb->csParam)->evts = 0;
    ((SerHShake_t)p->cPb->csParam)->fInX = 0;
    ((SerHShake_t)p->cPb->csParam)->null = 0;
    p->cPb->ioCRefNum = p->outRefNum;
    p->cPb->csCode = SerHShake;
    if ((result = OSErr_io_return_t(PBControl(p->cPb, FALSE))) != D_SUCCESS) {
      xprintf("serial_open: port %d output SerHShake ioResult is %d",
        number, p->cPb->ioResult);
      goto error;
    }

    /* clean out input buffer */
    p->cPb->ioCRefNum = p->inRefNum;
    if ((result = OSErr_io_return_t(PBKillIO(p->cPb, FALSE))) != D_SUCCESS) {
      xprintf("serial_open: port %d input KillIO ioResult is %d",
        number, p->cPb->ioResult);
      goto error;
    }

    /* clean out output queue */
    p->cPb->ioCRefNum = p->outRefNum;
    if ((result = OSErr_io_return_t(PBKillIO(p->cPb, FALSE))) != D_SUCCESS) {
      xprintf("serial_open: port %d output KillIO ioResult is %d",
        number, p->cPb->ioResult);
      goto error;
    }

    /* turn on DTR */
    p->modem_bits |= TM_DTR;
    p->cPb->ioCRefNum = p->outRefNum;
    p->cPb->csCode = SerDTRon;
    if ((result = OSErr_io_return_t(PBControl(p->cPb, FALSE))) != D_SUCCESS) {
      xprintf("serial_open: port %d output SerDTRon ioResult is %d",
       number, p->cPb->ioResult);
      goto error;
    }

    /* set up things for MACH generic tty layer */
    ttychars(tp);
    tp->t_dev = number;
    tp->t_oproc = serial_start;
    tp->t_stop = serial_stop;
    tp->t_getstat = serial_getstat;
    tp->t_setstat = serial_setstat;
    tp->t_state |= TS_HUPCLS;
    tp->t_ispeed = ISPEED;
    tp->t_ospeed = ISPEED;
    tp->t_flags = IFLAGS;
    tp->t_state &= ~TS_BUSY;

  }

  /* if not fully open, check for carrier */
  /* there doesn't seem to be any way to read the CTS bit ??? */
  if (!(tp->t_state & TS_ISOPEN)) tp->t_state |= TS_CARR_ON;

  /* finish up with the MACH generic tty layer */
  return char_open(number, tp, mode, ior);

error: /* clean up after any error */
  (void)serial_close(number);
  return result;

} /* serial_open() */

/* MACH device portdeath for serial port */
/* assume that the number is valid and that the serial port is open */
io_return_t serial_portdeath(int number, mach_port_t port)
{

  /* this is handled entirely by the MACH generic tty layer */
  return (tty_portdeath(&serial_port[number].tty, port));

} /* serial_portdeath() */

/* MACH device read for serial port */
/* assume that the number is valid and that the serial port is open */
io_return_t serial_read(int number, io_req_t ior)
{
  struct serial_port *p = &serial_port[number];
  struct tty *tp = &p->tty;

  /* check how many chars are currently in the internal input buffer */
  p->cPb->ioCRefNum = p->inRefNum;
  p->cPb->csCode = SerGetBuf;
  if (PBStatus(p->cPb, FALSE) != noErr) {
    xprintf("serial_read: port %d SerGetBuf ioResult is %d",
      number, p->cPb->ioResult);
    p->iPb->ioReqCount = 0;
  }
  else p->iPb->ioReqCount = ((SerGetBuf_t)p->cPb->csParam)->count;

  /* normalize ioReqCount */
  if (!p->iPb->ioReqCount) p->iPb->ioReqCount = 1;
  else if (p->iPb->ioReqCount > SERIAL_BUFFER_SIZE)
    p->iPb->ioReqCount = SERIAL_BUFFER_SIZE;

  /* let the Macintosh serial input driver finish the job */
  p->iPb->ioCompletion = p->readDone;
  p->iPb->ioActCount = p->iPb->ioReqCount;
  p->iPb->ioRefNum = p->inRefNum;
  p->iPb->ioBuffer = p->iBuf;
  (void)PBRead(p->iPb, TRUE);

  /* queue MACH generic tty input request */
  /* serial_read_done() will finish the job */
  return char_read(tp, ior);

} /* serial_read() */

/* MACH device write for serial port */
/* assume that the number is valid and that the serial port is open */
io_return_t serial_write(int number, io_req_t ior)
{
  struct tty *tp = &serial_port[number].tty;

  /* queue MACH generic tty input request */
  /* serial_start() starts the job */
  /* serial_write_done() will finish the job */
  return char_write(tp, ior);

} /* serial_write() */
