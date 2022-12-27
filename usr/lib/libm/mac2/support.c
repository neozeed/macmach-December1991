#ifndef lint
static char sccsid[] = "@(#)support.c	MacMach 4/26/90";
#endif /* not lint */

/* scalb(), copysign(), logb(), finite(), drem() and sqrt() are
 * inline functions in math.h
 */

#define __LIBM__

#include <math.h>

double scalb(const double x, const int N)
{
  return scalb_68881(x, N);
}

double copysign(const double x, const double y)
{
  return copysign_68881(x, y);
}

double logb(const double x)
{
  return logb_68881(x);
}

double finite(const double x)
{
  return finite_68881(x);
}

double drem(const double x, const double p)
{
  return drem_68881(x, p);
}

double sqrt(const x)
{
  return sqrt_68881(x);
}
