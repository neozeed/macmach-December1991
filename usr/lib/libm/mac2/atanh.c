#ifndef lint
static char sccsid[] = "@(#)atanh.c	MacMach 4/26/90";
#endif /* not lint */

/* atanh() is an inline function in math.h */

#define __LIBM__

#include <math.h>

double atanh(const double x)
{
  return atanh_68881(x);
}
