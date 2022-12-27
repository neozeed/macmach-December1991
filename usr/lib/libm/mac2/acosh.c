#ifndef lint
static char sccsid[] = "@(#)acosh.c	MacMach 4/26/90";
#endif /* not lint */

#include <math.h>

double acosh(const double x)
{
  volatile double temp;

  if (x != x) temp = x; /* x is NaN */
  else if(x>1.0E20) {
    temp = log1p(x) +  1.9082149292705877000E-10; /* 2^-33 * 1.A39EF35793C76 */
    return temp + 6.9314718036912381649E-1; /* 2^-1 * 1.62E42FEE00000 */
  }
  else {
    temp = sqrt(x - 1.0);
    return log1p(temp * (temp + sqrt(x + 1.0)));
  }
}
