#ifndef lint
static char sccsid[] = "@(#)asinh.c	MacMach 4/26/90";
#endif /* not lint */

#include <math.h>

double asinh(const double x)
{
  volatile double temp1;
  double temp2;

  if (x != x) temp1 = x; /* x is NaN */
  else if((temp1 = copysign(x, 1.0)) > 1.0E-10) {
    if(temp1 < 1.0E20) {
      temp2 = 1.0 / temp1;
      return copysign(log1p(temp1 +
        temp1 / (temp2 + sqrt(1.0 + temp2 * temp2))), x);
     }
    else { /* if |x| > big */
      temp1 = log1p(temp1) + 1.9082149292705877000E-10; /* 2^-33 * 1.A39EF35793C76 */
      return copysign(temp1 + 6.9314718036912381649E-1, x); /* 2^-1 * 1.62E42FEE00000 */
    }
  }
  else return x; /* if |x| < small */
}

