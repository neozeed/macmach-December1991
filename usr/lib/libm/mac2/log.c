#ifndef lint
static char sccsid[] = "@(#)log.c	MacMach 4/26/90";
#endif /* not lint */

/* log() is an inline function in math.h */

#define __LIBM__

#include <math.h>

double log(const double x)
{
  return log_68881(x);
}
