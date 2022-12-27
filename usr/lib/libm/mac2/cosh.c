#ifndef lint
static char sccsid[] = "@(#)cosh.c	MacMach 4/26/90";
#endif /* not lint */

/* cosh() is an inline function in math.h */

#define __LIBM__

#include <math.h>

double cosh(const double x)
{
  return cosh_68881(x);
}
