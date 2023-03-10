/*
 * jcarith.c
 *
 * Copyright (C) 1991, Thomas G. Lane.
 * This file is part of the Independent JPEG Group's software.
 * For conditions of distribution and use, see the accompanying README file.
 *
 * This file contains arithmetic entropy encoding routines.
 * These routines are invoked via the methods entropy_encode,
 * entropy_encoder_init/term, and entropy_optimize.
 */

#include "jinclude.h"

#ifdef ARITH_CODING_SUPPORTED


/*
 * The arithmetic coding option of the JPEG standard specifies Q-coding,
 * which is covered by patents held by IBM (and possibly AT&T and Mitsubishi).
 * At this time it does not appear to be legal for the Independent JPEG
 * Group to distribute software that implements arithmetic coding.
 * We have therefore removed arithmetic coding support from the
 * distributed source code.
 *
 * We're not happy about it either.
 */


/*
 * The method selection routine for arithmetic entropy encoding.
 */

GLOBAL void
jselcarithmetic (compress_info_ptr cinfo)
{
  if (cinfo->arith_code) {
    ERREXIT(cinfo->emethods, "Sorry, there are legal restrictions on arithmetic coding");
  }
}

#endif /* ARITH_CODING_SUPPORTED */
