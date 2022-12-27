/*
 * Copyright 1989 Jon Bennett, Mike Bolotski, David Gagne, Dan Lovinger
 * Copyright 1986 Chris Gutherie
 *
 * Permission to use, copy, modify, distribute, and sell this software and its
 * documentation for any purpose is hereby granted without fee, provided that
 * the above copyright notice appear in all copies and that both that
 * copyright notice and this permission notice appear in supporting
 * documentation, and that the name of the copyright holders not be used in
 * advertising or publicity pertaining to distribution of the software
 * without specific, written prior permission.  The copyright holders make no
 * representations about the suitability of this software for any purpose.
 * It is provided "as is" without express or implied warranty.
 *
 * THE COPYRIGHT HOLDERS DISCLAIM ALL WARRANTIES WITH REGARD TO THIS SOFTWARE,
 * INCLUDING ALL IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS, IN NO
 * EVENT SHALL THE COPYRIGHT HOLDERS BE LIABLE FOR ANY SPECIAL, INDIRECT OR
 * CONSEQUENTIAL DAMAGES OR ANY DAMAGES WHATSOEVER RESULTING FROM LOSS OF
 * USE, DATA OR PROFITS, WHETHER IN AN ACTION OF CONTRACT, NEGLIGENCE OR
 * OTHER TORTIOUS ACTION, ARISING OUT OF OR IN CONNECTION WITH THE USE OR
 * PERFORMANCE OF THIS SOFTWARE.
 */

#include <X11/Xlib.h>
#include <X11/Xos.h>

#include <stdio.h>
#include <ndbm.h>

#include "defs.h"

struct stats    old;

main()
{
  DBM            *sdb;
  datum           key, sinfo;

  if ((sdb = dbm_open(SCOREFILE, O_RDONLY, 0700)) < 0) {
    perror(SCOREFILE);
    return 1;
  }
  /* 0123456789012345678901234567890123456789012345678901234567890 */
  fprintf(stdout,
	  "Name                     rtime     kills  maxkills  losses\n");
  fprintf(stdout,
  "entries conqs coups   torps phasers  abomb aship planets gens ratio\n\n");

  key = dbm_firstkey(sdb);
  while (key.dptr) {
    sinfo = dbm_fetch(sdb, key);
    bcopy(sinfo.dptr, &old, sizeof (struct stats));
    if (old.st_entries > 0) {
      fprintf(stdout, "%-24s %8d %6.2f  %6.2f  %5d\n",
	      key.dptr,
	      old.st_time,
	      old.st_kills,
	      old.st_maxkills,
	      old.st_losses);
      fprintf(stdout, "%7d %5d %5d %7d %7d %6d %5d %7d %4d %5.3f\n",
	      old.st_entries,
	      old.st_conqs,
	      old.st_coups,
	      old.st_torps,
	      old.st_phasers,
	      old.st_armsbomb,
	      old.st_armsship,
	      old.st_planets,
	      old.st_genocides,
	      (old.st_losses ? old.st_kills / old.st_losses : 0.0));
      fprintf(stdout, "\n");
    }
    key = dbm_nextkey(sdb);
  }
}
