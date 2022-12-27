/* Macro definitions for a sun 3 running os 4.
   Copyright (C) 1989, Free Software Foundation, Inc.

This file is part of GDB.

This program is free software; you can redistribute it and/or modify
it under the terms of the GNU General Public License as published by
the Free Software Foundation; either version 2 of the License, or
(at your option) any later version.

This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with this program; if not, write to the Free Software
Foundation, Inc., 675 Mass Ave, Cambridge, MA 02139, USA.  */

#include "xm-sun3.h"
#define FPU

/* There is a bug which can cause alloca to fail to allocate large
   areas of memory one time in every 4096 (we think).  */
/* chase@orc.olivetti.com says that 4 megabyte alloca's consistently fail,
   even though the stack limit (SET_STACK_LIMIT_HUGE) has been set
   to 250 megabytes.  */
#define BROKEN_LARGE_ALLOCA
