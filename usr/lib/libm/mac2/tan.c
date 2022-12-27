#ifndef lint
static char sccsid[] = "@(#)tan.c	MacMach 4/26/90";
#endif /* not lint */

/* tan() is an inline function in math.h */

#define __LIBM__

#include <math.h>

double tan(const double x)
{
  return tan_68881(x);
}
