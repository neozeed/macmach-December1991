#ifndef lint
static char sccsid[] = "@(#)tanh.c	MacMach 4/26/90";
#endif /* not lint */

/* tanh() is an inline function in math.h */

#define __LIBM__

#include <math.h>

double tanh(const double x)
{
  return tanh_68881(x);
}
