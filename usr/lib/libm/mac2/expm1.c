#ifndef lint
static char sccsid[] = "@(#)expm1.c	MacMach 4/26/90";
#endif /* not lint */

/* expm1() is an inline function in math.h */

#define __LIBM__

#include <math.h>

double expm1(const double x)
{
  return expm1_68881(x);
}
