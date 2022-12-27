/* m- file for Apple Macintosh running Mach (for either 68020 or 68030 systems).

   Copyright (C) 1985, 1986 Free Software Foundation, Inc.

This file is part of GNU Emacs.

GNU Emacs is free software; you can redistribute it and/or modify
it under the terms of the GNU General Public License as published by
the Free Software Foundation; either version 1, or (at your option)
any later version.

GNU Emacs is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with GNU Emacs; see the file COPYING.  If not, write to
the Free Software Foundation, 675 Mass Ave, Cambridge, MA 02139, USA.  */


/* The following three symbols give information on
 the size of various data types.  */

#define SHORTBITS 16		/* Number of bits in a short */

#define INTBITS 32		/* Number of bits in an int */

#define LONGBITS 32		/* Number of bits in a long */

/* 68000 has lowest-numbered byte as most significant */

#ifndef BIG_ENDIAN
#define BIG_ENDIAN
#endif

/* Define how to take a char and sign-extend into an int.
   On machines where char is signed, this is a no-op.  */

#define SIGN_EXTEND_CHAR(c) (c)

/* Say this machine is a 68000 */

#ifndef m68000
#define m68000
#endif

/* Use type int rather than a union, to represent Lisp_Object */

#define NO_UNION_TYPE

/* XINT must explicitly sign-extend */

#define EXPLICIT_SIGN_EXTEND

/* Data type of load average. */

#define LOAD_AVE_TYPE double

/* Convert that into an integer that is 100 for a load average of 1.0  */

#define LOAD_AVE_CVT(x) ((int)x)

/* Must use the system's termcap.  It does special things.  */

#define LIBS_TERMCAP -ltermcap

/* Mask for address bits within a memory segment */

#define SEGMENT_MASK (SEGSIZ - 1)

/* Say that the text segment of a.out includes the header;
   the header actually occupies the first few bytes of the text segment
   and is counted in hdr.a_text.  */

#define A_TEXT_OFFSET(HDR) sizeof (HDR)
