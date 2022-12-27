#ifndef lint
static char sccsid[] = "@(#)asincos.c	MacMach 4/26/90";
#endif /* not lint */

/* asin() and acos() are inline functions in math.h */

#define __LIBM__

#include <math.h>

double asin(const double x)
{
  return asin_68881(x);
}

double acos(const double x)
{
  return acos_68881(x);
}
