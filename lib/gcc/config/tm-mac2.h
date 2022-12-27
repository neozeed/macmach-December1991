/* Definitions of target machine for GNU compiler.  Mac2 68020/68030 version.
   Copyright (C) 1987, 1988 Free Software Foundation, Inc.

This file is part of GNU CC.

GNU CC is free software; you can redistribute it and/or modify
it under the terms of the GNU General Public License as published by
the Free Software Foundation; either version 1, or (at your option)
any later version.

GNU CC is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with GNU CC; see the file COPYING.  If not, write to
the Free Software Foundation, 675 Mass Ave, Cambridge, MA 02139, USA.  */

#include "tm-m68k.h"

/* use bzero(), bcopy(), etc. from libc */
#define BSTRING

/* See tm-m68k.h.  7 means 68020 with 68881.  */
#ifndef TARGET_DEFAULT
#define TARGET_DEFAULT 7
#endif

/* Define __HAVE_FPA__ or __HAVE_68881__ in preprocessor,
   according to the -m flags.
   This will control the use of inline 68881 insns in certain macros.
   Also inform the program which CPU this is for.  */

#if TARGET_DEFAULT & 02

/* -m68881 is the default */
#define CPP_SPEC \
"%{!msoft-float:-D__HAVE_68881__ }\
%{!ansi:%{m68000:-Dmc68010}%{mc68000:-Dmc68010}%{!mc68000:%{!m68000:-Dmc68020}}}"

#else

/* -msoft-float is the default */
#define CPP_SPEC \
"%{m68881:-D__HAVE_68881__ }\
%{!ansi:%{m68000:-Dmc68010}%{mc68000:-Dmc68010}%{!mc68000:%{!m68000:-Dmc68020}}}"

#endif

/* -m68000 requires special flags to the assembler.  */
#define ASM_SPEC \
 "%{m68000:-mc68010}%{mc68000:-mc68010}%{!mc68000:%{!m68000:-mc68020}}"

/* Names to predefine in the preprocessor for this target machine.  */
#define CPP_PREDEFINES "-Dmc68000 -Dmac2 -Dunix -Dis68k -DMACH -DCMU -DCMUCS -DMM_XXX -DBYTE_MSF"

/* Make cc1 behave in pseudo-traditional fashion */
#define CC1_SPEC "-fwritable-strings"

/* Use -n on all links to save disk space. */
/*#define LINK_SPEC "-n"*/

/* Every structure or union's size must be a multiple of 2 bytes.  */
#define STRUCTURE_SIZE_BOUNDARY 16

/* This is BSD, so it wants DBX format.  */
#define DBX_DEBUGGING_INFO

/* do not want -pipe to be the default */
#undef DEFAULT_PIPE
