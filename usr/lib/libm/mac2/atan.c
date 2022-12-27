#ifndef lint
static char sccsid[] = "@(#)atan.c	MacMach 4/26/90";
#endif /* not lint */

/* atan() is an inline function in math.h */

#define __LIBM__

#include <math.h>

double atan(const double x)
{
  return atan_68881(x);
}
