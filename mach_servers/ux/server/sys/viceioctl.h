/* 
 * Mach Operating System
 * Copyright (c) 1987 Carnegie-Mellon University
 * All rights reserved.  The CMU software License Agreement specifies
 * the terms and conditions for use and redistribution.
 */

/*
 * ITC Remote file system - vice ioctl interface module
 *
 **********************************************************************
 * HISTORY
 * $Log:	viceioctl.h,v $
 * Revision 2.2  91/03/20  15:04:36  dbg
 * 	Fixed IOCTLs for ANSI C.
 * 	[91/02/21            dbg]
 * 
 * Revision 2.1  89/08/04  14:49:59  rwd
 * Created.
 * 
 *  7-Feb-87  Avadis Tevanian (avie) at Carnegie-Mellon University
 *	No need for VICE conditional.
 *
 * 22-Oct-86  Jay Kistler (jjk) at Carnegie-Mellon University
 *	Created from Andrew's vice.h and viceioctl.h.
 *
 **********************************************************************
 */

/*
 *  TODO:  Find /usr/local/include/viceioctl.h.
 */

struct ViceIoctl {
	caddr_t in, out;	/* Data to be transferred in, or out */
	short in_size;		/* Size of input buffer <= 2K */
	short out_size;		/* Maximum size of output buffer, <= 2K */
};

/* The 2K limits above are a consequence of the size of the kernel buffer
   used to buffer requests from the user to venus--2*MAXPATHLEN.
   The buffer pointers may be null, or the counts may be 0 if there
   are no input or output parameters
 */

#define _VICEIOCTL(id)  ((unsigned int ) _IOW('V', id, struct ViceIoctl))
/* Use this macro to define up to 256 vice ioctl's.  These ioctl's
   all potentially have in/out parameters--this depends upon the
   values in the ViceIoctl structure.  This structure is itself passed
   into the kernel by the normal ioctl parameter passing mechanism.
 */

#define _VALIDVICEIOCTL(com) (com >= _VICEIOCTL(0) && com <= _VICEIOCTL(255))
