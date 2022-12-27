#ifndef lint
static char sccsid[] = "@(#)sincos.c	MacMach 4/26/90";
#endif /* not lint */

/* sin() and cos() are inline functions in math.h */

#define __LIBM__

#include <math.h>

double sin(const double x)
{
  return sin_68881(x);
}

double cos(const double x)
{
  return cos_68881(x);
}
