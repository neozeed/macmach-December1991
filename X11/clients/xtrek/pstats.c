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

#include <sys/resource.h>
#include "defs.h"
#include "data.h"

GLOBAL void     savestats(p)
  register struct player *p;
{
  char            buf[80];
  datum           key, sinfo;
  long            now;
  struct rusage   rbuf;
  struct stats    old;
  register DBM   *db = dbm_open(SCOREFILE, O_RDWR, 0700);

  if (!db)
    return;

  if (ISROBOT(p))
    sprintf(buf, "%s", p->p_name);
  else
    sprintf(buf, "%s:%s", p->p_login, p->p_monitor);
  key.dptr = buf;
  key.dsize = strlen(buf) + 1;
  sinfo = dbm_fetch(db, key);
  if (sinfo.dptr)
    bzero(&old, sizeof (struct stats));
  else {
    bcopy(sinfo.dptr, &old, sizeof (struct stats));
    key.dptr = buf;
    key.dsize = strlen(buf) + 1;
    dbm_delete(db, key);
  }

  p->p_stats.st_kills += old.st_kills;
  p->p_stats.st_losses += old.st_losses;
  p->p_stats.st_entries += old.st_entries;
  p->p_stats.st_conqs += old.st_conqs;
  p->p_stats.st_coups += old.st_coups;
  p->p_stats.st_torps += old.st_torps;
  p->p_stats.st_phasers += old.st_phasers;
  p->p_stats.st_armsbomb += old.st_armsbomb;
  p->p_stats.st_armsship += old.st_armsship;
  p->p_stats.st_planets += old.st_planets;
  p->p_stats.st_genocides += old.st_genocides;

  time(&now);
  p->p_stats.st_time = old.st_time + (now - p->start_time);

  getrusage(0, &rbuf);
  p->p_stats.st_cpu = old.st_cpu + rbuf.ru_utime.tv_sec + rbuf.ru_stime.tv_sec;

  if (p->p_stats.st_maxkills < old.st_maxkills)
    p->p_stats.st_maxkills = old.st_maxkills;

  if (!sinfo.dptr)
    sinfo.dptr = (char *) &p->p_stats;
  else {
    sinfo.dptr = (char *) &old;
    bcopy(&p->p_stats, sinfo.dptr, sizeof (struct stats));
  }
  sinfo.dsize = sizeof (struct stats);
  key.dptr = buf;
  key.dsize = strlen(buf) + 1;
  dbm_store(db, key, sinfo, DBM_INSERT);
  (void) dbm_close(db);
}
