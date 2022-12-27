#ifndef lint
static char sccsid[] = "@(#)floor.c	MacMach 4/26/90";
#endif /* not lint */

/* floor(), ceil() and rint() are inline functions in math.h */

#define __LIBM__

#include <math.h>

double floor(x)
double x;
{
  return floor_68881(x);
}

double ceil(const double x)
{
  return ceil_68881(x);
}

double rint(const double x)
{
  return rint_68881(x);
}
