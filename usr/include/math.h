/*
 * Copyright (c) 1985 Regents of the University of California.
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms are permitted
 * provided that the above copyright notice and this paragraph are
 * duplicated in all such forms and that any documentation,
 * advertising materials, and other materials related to such
 * distribution and use acknowledge that the software was developed
 * by the University of California, Berkeley.  The name of the
 * University may not be used to endorse or promote products derived
 * from this software without specific prior written permission.
 * THIS SOFTWARE IS PROVIDED ``AS IS'' AND WITHOUT ANY EXPRESS OR
 * IMPLIED WARRANTIES, INCLUDING, WITHOUT LIMITATION, THE IMPLIED
 * WARRANTIES OF MERCHANTIBILITY AND FITNESS FOR A PARTICULAR PURPOSE.
 *
 * All recipients should regard themselves as participants in an ongoing
 * research project and hence should feel obligated to report their
 * experiences (good or bad) with these elementary function codes, using
 * the sendbug(8) program, to the authors.
 *
 *	@(#)math.h	5.3 (Berkeley) 6/30/88
 */

#ifdef mac2

/*
 * This version of math-68881.h started out being the one written
 * "Copyright (C) 1989 by Matthew Self, last modified 18 May 1989"
 * as distributed with version 1.37.1 of GCC.  I have since tweaked it
 * into its present form.  Zon Williamson, 4/26/90
 */

/******************************************************************\
*								   *
*  <math-68881.h>		last modified: 18 May 1989.	   *
*								   *
*  Copyright (C) 1989 by Matthew Self.				   *
*  You may freely distribute verbatim copies of this software	   *
*  provided that this copyright notice is retained in all copies.  *
*  You may distribute modifications to this software under the     *
*  conditions above if you also clearly note such modifications    *
*  with their author and date.			   	     	   *
*								   *
*  Note:  errno is not set to EDOM when domain errors occur for    *
*  most of these functions.  Rather, it is assumed that the	   *
*  68881s OPERR exception will be enabled and handled		   *
*  appropriately by the	operating system.  Similarly, overflow	   *
*  and underflow do not set errno to ERANGE.			   *
*								   *
*  Send bugs to Matthew Self (self@bayes.arc.nasa.gov).		   *
*								   *
\******************************************************************/

#ifndef	_MATH_H_
#define	_MATH_H_

#include <errno.h>

/* use __inline and __asm in case of -traditional */

/* define __LIBM__ when building libm.a to append "_68881" to function names */
#ifdef __LIBM__
#define _68881(X) X##_68881
#else
#define _68881(X) X
#endif

typedef struct { double r, i; } complex;

/* non-zero if not a number */
#define NaN(X)	(((union { double d; struct { unsigned :1, e:11; } s; } \
			*)&X)->s.e == 0x7ff)

/* an IEEE std. infinity occurs when exp=max=0x7ff and fractional part == 0 */
#define INF(X) \
(((union{double d; struct {unsigned :1,e:11,f1:20,f2:32;} s;} *)&X)->s.e \
  ==0x7ff) &&  \
(((union{double d; struct {unsigned :1,e:11,f1:20,f2:32;} s;} *)&X)->s.f1 \
  ==0) && \
(((union{double d; struct {unsigned :1,e:11,f1:20,f2:32;} s;} *)&X)->s.f2 \
  ==0)

__inline static const double _68881(sin)(const double x)
{
  double value;
  __asm("fsin%.x %1,%0" : "=f" (value) : "f" (x));
  return value;
}

__inline static const double _68881(cos)(const double x)
{
  double value;
  __asm("fcos%.x %1,%0" : "=f" (value) : "f" (x));
  return value;
}

__inline static const double _68881(tan)(double x)
{
  double value;
  __asm("ftan%.x %1,%0" : "=f" (value) : "f" (x));
  return value;
}

__inline static const double _68881(asin)(const double x)
{
  double value;
  __asm("fasin%.x %1,%0" : "=f" (value) : "f" (x));
  return value;
}

__inline static const double _68881(acos)(const double x)
{
  double value;
  __asm("facos%.x %1,%0" : "=f" (value) : "f" (x));
  return value;
}

__inline static const double _68881(atan)(double x)
{
  double value;
  __asm("fatan%.x %1,%0" : "=f" (value) : "f" (x));
  return value;
}

__inline static const double _68881(sinh)(const double x)
{
  double value;
  __asm("fsinh%.x %1,%0" : "=f" (value) : "f" (x));
  return value;
}

__inline static const double _68881(cosh)(const double x)
{
  double value;
  __asm("fcosh%.x %1,%0" : "=f" (value) : "f" (x));
  return value;
}

__inline static const double _68881(tanh)(const double x)
{
  double value;
  __asm("ftanh%.x %1,%0" : "=f" (value) : "f" (x));
  return value;
}

__inline static const double _68881(atanh)(const double x)
{
  double value;
  __asm("fatanh%.x %1,%0" : "=f" (value) : "f" (x));
  return value;
}

__inline static const double _68881(exp)(const double x)
{
  double value;
  __asm("fetox%.x %1,%0" : "=f" (value) : "f" (x));
  return value;
}

__inline static const double _68881(expm1)(const double x)
{
  double value;
  __asm("fetoxm1%.x %1,%0" : "=f" (value) : "f" (x));
  return value;
}

__inline static const double _68881(log)(const double x)
{
  double value;
  __asm("flogn%.x %1,%0" : "=f" (value) : "f" (x));
  return value;
}

__inline static const double _68881(log1p)(const double x)
{
  double value;
  __asm("flognp1%.x %1,%0" : "=f" (value) : "f" (x));
  return value;
}

__inline static const double _68881(log10)(const double x)
{
  double value;
  __asm("flog10%.x %1,%0" : "=f" (value) : "f" (x));
  return value;
}

__inline static const double _68881(sqrt)(double x)
{
  double value;
  __asm("fsqrt%.x %1,%0" : "=f" (value) : "f" (x));
  return value;
}

__inline static const double _68881(pow)(const double x, const double y)
{
  if (x > 0) return _68881(exp)(y * _68881(log)(x));
  else if (x == 0) {
    if (y > 0) return 0.0;
    else {
      double value;
      errno = EDOM;
      __asm("fmove%.d %#0rnan,%0" : "=f" (value) : /* no inputs */);
      return value;
    }
  }
  else {
    double temp;
    __asm("fintrz%.x %1,%0" : "=f" (temp) : "f" (y));
    if (y == temp) {
      int i = (int) y;
      if ((i & 1) == 0) return _68881(exp)(y * _68881(log)(x));	/* even */
      else return - _68881(exp)(y * _68881(log)(x));
    }
    else {
      double value;
      errno = EDOM;
      __asm("fmove%.d %#0rnan,%0" : "=f" (value) : /* no inputs */);
      return value;
    }
  }
}

__inline static const double _68881(fabs)(double x)
{
  double value;
  __asm("fabs%.x %1,%0" : "=f" (value) : "f" (x));
  return value;
}

__inline static const double _68881(ceil)(const double x)
{
  int rounding_mode, round_up;
  double value;

  __asm volatile ("fmove%.l fpcr,%0" : "=dm" (rounding_mode) : /* no inputs */ );
  round_up = rounding_mode | 0x30;
  __asm volatile ("fmove%.l %0,fpcr" : /* no outputs */ : "dmi" (round_up));
  __asm volatile ("fint%.x %1,%0" : "=f" (value) : "f" (x));
  __asm volatile ("fmove%.l %0,fpcr" : /* no outputs */ : "dmi" (rounding_mode));
  return value;
}

__inline static const double _68881(floor)(const double x)
{
  int rounding_mode, round_down;
  double value;

  __asm volatile ("fmove%.l fpcr,%0" : "=dm" (rounding_mode) : /* no inputs */ );
  round_down = (rounding_mode & ~0x10) | 0x20;
  __asm volatile ("fmove%.l %0,fpcr" : /* no outputs */ : "dmi" (round_down));
  __asm volatile ("fint%.x %1,%0" : "=f" (value) : "f" (x));
  __asm volatile ("fmove%.l %0,fpcr" : /* no outputs */ : "dmi" (rounding_mode));
  return value;
}

__inline static const double _68881(rint)(const double x)
{
  int rounding_mode, round_nearest;
  double value;

  __asm volatile ("fmove%.l fpcr,%0" : "=dm" (rounding_mode) : /* no inputs */ );
  round_nearest = rounding_mode & ~0x30;
  __asm volatile ("fmove%.l %0,fpcr" : /* no outputs */ : "dmi" (round_nearest));
  __asm volatile ("fint%.x %1,%0" : "=f" (value) : "f" (x));
  __asm volatile ("fmove%.l %0,fpcr" : /* no outputs */ : "dmi" (rounding_mode));
  return value;
}

__inline static const double _68881(fmod)(double x, double y)
{
  double value;
  __asm("fmod%.x %2,%0" : "=f" (value) : "0" (x), "f" (y));
  return value;
}

__inline static const double _68881(drem)(double x, double y)
{
  double value;
  __asm("frem%.x %2,%0" : "=f" (value) : "0" (x), "f" (y));
  return value;
}

__inline static const double _68881(scalb)(double x, int n)
{
  double value;
  __asm("fscale%.l %2,%0" : "=f" (value) : "0" (x), "dmi" (n));
  return value;
}

__inline static double _68881(logb)(double x)
{
  double exponent;
  __asm("fgetexp%.x %1,%0" : "=f" (exponent) : "f" (x));
  return exponent;
}

__inline static const double _68881(ldexp)(double x, int n)
{
  double value;
  __asm("fscale%.l %2,%0" : "=f" (value) : "0" (x), "dmi" (n));
  return value;
}

__inline static double _68881(frexp)(double x, int *exp)
{
  double float_exponent;
  int int_exponent;
  double mantissa;

  __asm("fgetexp%.x %1,%0" : "=f" (float_exponent) : "f" (x));
  int_exponent = (int) float_exponent;
  __asm("fgetman%.x %1,%0" : "=f" (mantissa) : "f" (x));
  /* 1.0 <= mantissa < 2.0 */
  if (mantissa != 0) {
    __asm("fscale%.b %#-1,%0" : "=f" (mantissa) : "0" (mantissa));
    int_exponent += 1;
  }
  *exp = int_exponent;
  return mantissa;
}

__inline static double _68881(modf)(double x, double *ip)
{
  double temp;
  __asm("fintrz%.x %1,%0" : "=f" (temp) : "f" (x));
  *ip = temp;
  return x - temp;
}

__inline static double _68881(copysign)(double x, double y)
{
  unsigned short  *px, *py;
  px = (unsigned short *) &x;
  py = (unsigned short *) &y;
  *px = (*px & 0x7FFF) | (*py & ~0x7FFF);
  return x;
}

__inline static int _68881(finite)(double x)
{
  unsigned short *px;
  px = (unsigned short *) &x;
  return (*px & 0x7FF0) != 0x7FF0;
}

__inline static double _68881(hypot)(const double x, const double y)
{
  return _68881(sqrt)(x * x + y * y);
}

/* mac2 functions in libm.a */
extern double acosh(const double);
extern double asinh(const double);
extern double atan2(const double, const double);
extern double cbrt(const double);
extern double cabs(const complex);
extern double z_abs(const complex*);

/* standard math functions in libm.a */
extern double erf(double), erfc(double);
extern double lgamma(double);
extern double j0(double), j1(double), jn(int, double);
extern double y0(double), y1(double), yn(int, double);

/* some useful constants */
#define M_E	2.7182818284590452354
#define M_LOG2E	1.4426950408889634074
#define M_LOG10E	0.43429448190325182765
#define M_LN2	0.69314718055994530942
#define M_LN10	2.30258509299404568402
#define M_PI	3.14159265358979323846
#define M_PI_2	1.57079632679489661923
#define M_PI_4	0.78539816339744830962
#define M_1_PI	0.31830988618379067154
#define M_2_PI	0.63661977236758134308
#define M_2_SQRTPI	1.12837916709551257390
#define M_SQRT2	1.41421356237309504880
#define M_SQRT1_2	0.70710678118654752440

/* some useful constants */
#define HUGE \
({								\
  double huge;							\
  __asm("fmove%.d #0x7ff0000000000000,%0" : "=f" (huge) :);	\
  huge;								\
})
#define M_E		2.7182818284590452354
#define M_LOG2E		1.4426950408889634074
#define M_LOG10E	0.43429448190325182765
#define M_LN2		0.69314718055994530942
#define M_LN10		2.30258509299404568402
#define M_PI		3.14159265358979323846
#define M_PI_2		1.57079632679489661923
#define M_PI_4		0.78539816339744830962
#define M_1_PI		0.31830988618379067154
#define M_2_PI		0.63661977236758134308
#define M_2_SQRTPI	1.12837916709551257390
#define M_SQRT2		1.41421356237309504880
#define M_SQRT1_2	0.70710678118654752440

#endif	/* _MATH_H_ */

#else

extern double asinh(), acosh(), atanh();
extern double erf(), erfc();
extern double exp(), expm1(), log(), log10(), log1p(), pow();
extern double fabs(), floor(), ceil(), rint();
extern double lgamma();
extern double hypot(), cabs();
extern double copysign(), drem(), logb(), scalb();
extern int finite();
#if defined(vax)||defined(tahoe)
extern double infnan();
#endif	/* defined(vax)||defined(tahoe) */
extern double j0(), j1(), jn(), y0(), y1(), yn();
extern double sin(), cos(), tan(), asin(), acos(), atan(), atan2();
extern double sinh(), cosh(), tanh();
extern double cbrt(), sqrt();
extern double modf(), ldexp(), frexp(), atof();

#define HUGE	1.701411733192644270e38

#endif
