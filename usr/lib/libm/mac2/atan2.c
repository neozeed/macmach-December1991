#ifndef lint
static char sccsid[] = "@(#)atan2.c	MacMach 4/26/90";
#endif /* not lint */

#include <math.h>

double atan2(const double y, const double x)
{
  double pi, pi_over_2;

  __asm("fmovecr%.x %#0,%0" : "=f" (pi) : /* no inputs */ );
  __asm("fscale%.b %#-1,%0" : "=f" (pi_over_2) : "0" (pi));
  if (x > 0) {
    if (y > 0) {
      if (x > y) return(atan(y / x));
      else return(pi_over_2 - atan(x / y));
    }
    else {
      if (x > -y) return(atan(y / x));
      else return(-pi_over_2 - atan(x / y));
    }
  }
  else {
    if (y > 0) {
      if (-x > y) return(pi + atan(y / x));
      else return(pi_over_2 - atan(x / y));
    }
    else {
      if (-x > -y) return(-pi + atan(y / x));
      else if (y < 0) return(-pi_over_2 - atan(x / y));
      else {
        double value;
        errno = EDOM;
        __asm("fmove%.d %#0rnan,%0" : "=f" (value) : /* no inputs */);
        return(value);
      }
    }
  }
}
