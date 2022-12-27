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

#include "defs.h"
#include "data.h"
#include "polar.h"

#define SKILL_DIV 8

GLOBAL void     repair_damage(p)
  register aPlayer *p;
{
  register int    repair_work, work_chunk, nunused;
  int             i, maxshield = p->ship.maxshields, oldrepair;

  if (p->p_damage || p->p_shield < maxshield)
    goto isdamage;			/* Yuck (I'm guilty, Mike) */

  for (i = 0; i < NSUBSYSTEMS; i++)
    if (p->p_sysdamage[i])
      goto isdamage;

  CLRREPAIR(p);				/* if no damage then punt out of here */
  return;

isdamage:
  repair_work = p->ship.repair;
  {
    int             skill_bonus = (repair_work * p->p_stats.st_kills) / (p->p_stats.st_losses + 1);

    repair_work += skill_bonus / SKILL_DIV;
  }


  if (ISREPAIR(p) && p->p_speed == 0)
    repair_work *= 2;

  if ((ISORBIT(p)) && !hostilePlanet(&planets[p->p_planet], p)) {
    if (!(planets[p->p_planet].pl_flags & PLREPAIR))
      repair_work *= 2;
    else
      repair_work *= 4;
  }
  p->p_damage -= repair_work;		/* accumulate repair */
  if (p->p_damage <= 0)
    p->p_damage = 0;


  oldrepair = repair_work;		/* store for later use with shields */
  repair_work *= DAMSCALE(1);

  work_chunk = repair_work / 2;		/* A nibble of work to use as needed */
  nunused = 30;				/* loop N times that work is not used */

  for (;;) {
    int             i;
    int             variance = RAND(100);

    if (work_chunk > 0) {
      if (nunused-- <= 0)
	break;
    }
    else if (repair_work > 0) {
      work_chunk = repair_work;
      repair_work = 0;
    }
    else
      break;

    for (i = 0; i < NSUBSYSTEMS; i++) {	/* find a subsystem and fix it */
      int             dam = p->p_sysdamage[i];

      if (dam <= 0 || variance >= sysfix[i])
	continue;
      dam -= work_chunk;
      if (dam < 0) {
	repair_work -= dam;
	dam = 0;
      }
      else
	repair_work -= work_chunk;
      work_chunk = 0;
      p->p_sysdamage[i] = dam;
      break;
    }
  }


  if (FAILED(p, Shield))
    return;
  p->p_shield += ISSHIELD(p) ? oldrepair / 2 : oldrepair;
  if (p->p_shield >= maxshield)
    p->p_shield = maxshield;

}

GLOBAL void     damage_ship(p, attacker, damage, how)
  register struct player *p;
  register int    attacker, damage;
  DeathReason     how;
{
  struct player  *other = &players[attacker];

  if (how != KPlanet && other->p_hostile & p->p_mask)
    other->p_swar |= p->p_mask;
  /* Note that if a player is at peace with the victim, then the damage was
     caused either accidently, or because the victim was at war with, or
     hostile to, the player. In either case, we don't consider the damage to
     be an act of war. */

  if (ISSHIELD(p)) {
    int             shield = p->p_shield - damage;

    if (shield < 0) {
      damage = -shield;
      p->p_shield = 0;
    }
    else {
      p->p_shield = shield;
      return;
    }
  }
  p->p_damage += damage;
  if (p->p_damage < p->ship.maxdamage) {
    p->p_newdamage += damage + 1;
    return;
  }
  p->p_status = PExplode;
  p->p_explode = PEXPTIME;
  if (how != KPlanet) {
    if (attacker != p->p_no) {
      float           killvalue = 1.0 + (p->p_armies + p->p_kills) * 0.1;

      other->p_kills += killvalue;
      other->p_stats.st_kills += killvalue;
      empires[other->p_empire].stats.kills += killvalue;
      if (other->p_stats.st_maxkills < other->p_kills)
	other->p_stats.st_maxkills = other->p_kills;
    }
    killmess(p, other);
  }
  empires[p->p_empire].stats.losses++;
  p->p_stats.st_losses++;
  p->p_whydead = how;
  p->p_whodead = attacker;
}

GLOBAL void     break_ship(p)
  register struct player *p;
{
  register int    newdamage = p->p_newdamage;
  register int    loop = random() % ((newdamage / 16 + p->p_damage / 64) + 1);
  register int    pound = newdamage * 6 + 2;

  p->p_newdamage = 0;

  while (loop--) {
    int             i;

    for (i = 0; i < NSUBSYSTEMS; i++)
      if ((random() % pound) >= sysfail[i])
	p->p_sysdamage[i] += (random() % newdamage) + sysfail[i];
  }

  if (FAILED(p, Cloak))
    CLRCLOAK(p);
  if (FAILED(p, Shield))
    CLRSHIELD(p);
  if (ISENG(p))
    CLRTURBO(p);
  if (FAILED(p, Lock)) {
    CLRPLOCK(p);
    CLRPLLOCK(p);
  }
  if (FAILED(p, Trans)) {
    CLRBEAMDOWN(p);
    CLRBEAMUP(p);
  }
}
