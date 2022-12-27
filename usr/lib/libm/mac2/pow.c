#ifndef lint
static char sccsid[] = "@(#)pow.c	MacMach 4/26/90";
#endif /* not lint */

/* pow() is an inline function in math.h */

#define __LIBM__

#include <math.h>

double pow(const double x, const double y)
{
  return pow_68881(x, y);
}
