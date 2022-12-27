#ifndef lint
static char sccsid[] = "@(#)sinh.c	MacMach 4/26/90";
#endif /* not lint */

/* sinh() is an inline function in math.h */

#define __LIBM__

#include <math.h>

double sinh(const double x)
{
  return sinh_68881(x);
}
