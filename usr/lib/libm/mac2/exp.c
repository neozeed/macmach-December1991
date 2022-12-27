#ifndef lint
static char sccsid[] = "@(#)exp.c	MacMach 4/26/90";
#endif /* not lint */

/* exp() is an inline function in math.h */

#define __LIBM__

#include <math.h>

double exp(const double x)
{
  return exp_68881(x);
}
