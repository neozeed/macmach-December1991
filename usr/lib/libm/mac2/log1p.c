#ifndef lint
static char sccsid[] = "@(#)log1p.c	MacMach 4/26/90";
#endif /* not lint */

/* log1p() is an inline function in math.h */

#define __LIBM__

#include <math.h>

double log1p(const double x)
{
  return log1p_68881(x);
}
