#ifndef lint
static char sccsid[] = "@(#)log10.c	MacMach 4/26/90";
#endif /* not lint */

/* log10() is an inline function in math.h */

#define __LIBM__

#include <math.h>

double log10(const double x)
{
  return log10_68881(x);
}
