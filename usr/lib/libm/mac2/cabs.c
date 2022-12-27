#ifndef lint
static char sccsid[] = "@(#)cabs.c	MacMach 4/26/90";
#endif /* not lint */

/* hypot() is an inline functions in math.h */

#define __LIBM__

#include <math.h>

double hypot(const double x, const double y)
{
  return hypot_68881(x, y);
}

double cabs(const complex z)
{
  return hypot_68881(z.r, z.i);
}

double z_abs(const complex *z)
{
  return hypot_68881(z->r, z->i);
}
