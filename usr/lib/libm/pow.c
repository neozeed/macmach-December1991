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
 */

#ifdef mac2
#include "mac2/pow.c"
#else

#ifndef lint
static char sccsid[] = "@(#)pow.c	5.3 (Berkeley) 6/30/88";
#endif /* not lint */

/* POW(X,Y)  
 * RETURN X**Y 
 * DOUBLE PRECISION (VAX D format 56 bits, IEEE DOUBLE 53 BITS)
 * CODED IN C BY K.C. NG, 1/8/85; 
 * REVISED BY K.C. NG on 7/10/85.
 *
 * Required system supported functions:
 *      scalb(x,n)      
 *      logb(x)         
 *	copysign(x,y)	
 *	finite(x)	
 *	drem(x,y)
 *
 * Required kernel functions:
 *	exp__E(a,c)	...return  exp(a+c) - 1 - a*a/2
 *	log__L(x)	...return  (log(1+x) - 2s)/s, s=x/(2+x) 
 *	pow_p(x,y)	...return  +(anything)**(finite non zero)
 *
 * Method
 *	1. Compute and return log(x) in three pieces:
 *		log(x) = n*ln2 + hi + lo,
 *	   where n is an integer.
 *	2. Perform y*log(x) by simulating muti-precision arithmetic and 
 *	   return the answer in three pieces:
 *		y*log(x) = m*ln2 + hi + lo,
 *	   where m is an integer.
 *	3. Return x**y = exp(y*log(x))
 *		= 2^m * ( exp(hi+lo) ).
 *
 * Special cases:
 *	(anything) ** 0  is 1 ;
 *	(anything) ** 1  is itself;
 *	(anything) ** NaN is NaN;
 *	NaN ** (anything except 0) is NaN;
 *	+-(anything > 1) ** +INF is +INF;
 *	+-(anything > 1) ** -INF is +0;
 *	+-(anything < 1) ** +INF is +0;
 *	+-(anything < 1) ** -INF is +INF;
 *	+-1 ** +-INF is NaN and signal INVALID;
 *	+0 ** +(anything except 0, NaN)  is +0;
 *	-0 ** +(anything except 0, NaN, odd integer)  is +0;
 *	+0 ** -(anything except 0, NaN)  is +INF and signal DIV-BY-ZERO;
 *	-0 ** -(anything except 0, NaN, odd integer)  is +INF with signal;
 *	-0 ** (odd integer) = -( +0 ** (odd integer) );
 *	+INF ** +(anything except 0,NaN) is +INF;
 *	+INF ** -(anything except 0,NaN) is +0;
 *	-INF ** (odd integer) = -( +INF ** (odd integer) );
 *	-INF ** (even integer) = ( +INF ** (even integer) );
 *	-INF ** -(anything except integer,NaN) is NaN with signal;
 *	-(x=anything) ** (k=integer) is (-1)**k * (x ** k);
 *	-(anything except 0) ** (non-integer) is NaN with signal;
 *
 * Accuracy:
 *	pow(x,y) returns x**y nearly rounded. In particular, on a SUN, a VAX,
 *	and a Zilog Z8000,
 *			pow(integer,integer)
 *	always returns the correct integer provided it is representable.
 *	In a test run with 100,000 random arguments with 0 < x, y < 20.0
 *	on a VAX, the maximum observed error was 1.79 ulps (units in the 
 *	last place).
 *
 * Constants :
 * The hexadecimal values are the intended ones for the following constants.
 * The decimal values may be used, provided that the compiler will convert
 * from decimal to binary accurately enough to produce the hexadecimal values
 * shown.
 */

#if defined(vax)||defined(tahoe)	/* VAX D format */
#include <errno.h>
extern double infnan();
#ifdef vax
#define _0x(A,B)	0x/**/A/**/B
#else	/* vax */
#define _0x(A,B)	0x/**/B/**/A
#endif	/* vax */
/* static double */
/* ln2hi  =  6.9314718055829871446E-1    , Hex  2^  0   *  .B17217F7D00000 */
/* ln2lo  =  1.6465949582897081279E-12   , Hex  2^-39   *  .E7BCD5E4F1D9CC */
/* invln2 =  1.4426950408889634148E0     , Hex  2^  1   *  .B8AA3B295C17F1 */
/* sqrt2  =  1.4142135623730950622E0     ; Hex  2^  1   *  .B504F333F9DE65 */
static long     ln2hix[] = { _0x(7217,4031), _0x(0000,f7d0)};
static long     ln2lox[] = { _0x(bcd5,2ce7), _0x(d9cc,e4f1)};
static long    invln2x[] = { _0x(aa3b,40b8), _0x(17f1,295c)};
static long     sqrt2x[] = { _0x(04f3,40b5), _0x(de65,33f9)};
#define    ln2hi    (*(double*)ln2hix)
#define    ln2lo    (*(double*)ln2lox)
#define   invln2    (*(double*)invln2x)
#define    sqrt2    (*(double*)sqrt2x)
#else	/* defined(vax)||defined(tahoe)	*/
static double
ln2hi  =  6.9314718036912381649E-1    , /*Hex  2^ -1   *  1.62E42FEE00000 */
ln2lo  =  1.9082149292705877000E-10   , /*Hex  2^-33   *  1.A39EF35793C76 */
invln2 =  1.4426950408889633870E0     , /*Hex  2^  0   *  1.71547652B82FE */
sqrt2  =  1.4142135623730951455E0     ; /*Hex  2^  0   *  1.6A09E667F3BCD */
#endif	/* defined(vax)||defined(tahoe)	*/

static double pow_p();
static double zero=0.0, half=1.0/2.0, one=1.0, two=2.0, negone= -1.0;

double pow(x,y)  	
double x,y;
{
	double drem(),pow_p(),copysign(),t;
	int finite();

	if     (y==zero)      return(one);
	else if(y==one
#if !defined(vax)&&!defined(tahoe)
		||x!=x
#endif	/* !defined(vax)&&!defined(tahoe) */
		) return( x );      /* if x is NaN or y=1 */
#if !defined(vax)&&!defined(tahoe)
	else if(y!=y)         return( y );      /* if y is NaN */
#endif	/* !defined(vax)&&!defined(tahoe) */
	else if(!finite(y))                     /* if y is INF */
	     if((t=copysign(x,one))==one) return(zero/zero);
	     else if(t>one) return((y>zero)?y:zero);
	     else return((y<zero)?-y:zero);
	else if(y==two)       return(x*x);
	else if(y==negone)    return(one/x);

    /* sign(x) = 1 */
	else if(copysign(one,x)==one) return(pow_p(x,y));

    /* sign(x)= -1 */
	/* if y is an even integer */
	else if ( (t=drem(y,two)) == zero)	return( pow_p(-x,y) );

	/* if y is an odd integer */
	else if (copysign(t,one) == one) return( -pow_p(-x,y) );

	/* Henceforth y is not an integer */
	else if(x==zero)	/* x is -0 */
	    return((y>zero)?-x:one/(-x));
	else {			/* return NaN */
#if defined(vax)||defined(tahoe)
	    return (infnan(EDOM));	/* NaN */
#else	/* defined(vax)||defined(tahoe) */
	    return(zero/zero);
#endif	/* defined(vax)||defined(tahoe) */
	}
}

/* pow_p(x,y) return x**y for x with sign=1 and finite y */
static double pow_p(x,y)       
double x,y;
{
        double logb(),scalb(),copysign(),log__L(),exp__E();
        double c,s,t,z,tx,ty;
#ifdef tahoe
	double tahoe_tmp;
#endif	/* tahoe */
        float sx,sy;
	long k=0;
        int n,m;

	if(x==zero||!finite(x)) {           /* if x is +INF or +0 */
#if defined(vax)||defined(tahoe)
	     return((y>zero)?x:infnan(ERANGE));	/* if y<zero, return +INF */
#else	/* defined(vax)||defined(tahoe) */
	     return((y>zero)?x:one/x);
#endif	/* defined(vax)||defined(tahoe) */
	}
	if(x==1.0) return(x);	/* if x=1.0, return 1 since y is finite */

    /* reduce x to z in [sqrt(1/2)-1, sqrt(2)-1] */
        z=scalb(x,-(n=logb(x)));  
#if !defined(vax)&&!defined(tahoe)	/* IEEE double; subnormal number */
        if(n <= -1022) {n += (m=logb(z)); z=scalb(z,-m);} 
#endif	/* !defined(vax)&&!defined(tahoe) */
        if(z >= sqrt2 ) {n += 1; z *= half;}  z -= one ;

    /* log(x) = nlog2+log(1+z) ~ nlog2 + t + tx */
	s=z/(two+z); c=z*z*half; tx=s*(c+log__L(s*s)); 
	t= z-(c-tx); tx += (z-t)-c;

   /* if y*log(x) is neither too big nor too small */
	if((s=logb(y)+logb(n+t)) < 12.0) 
	    if(s>-60.0) {

	/* compute y*log(x) ~ mlog2 + t + c */
        	s=y*(n+invln2*t);
                m=s+copysign(half,s);   /* m := nint(y*log(x)) */ 
		k=y; 
		if((double)k==y) {	/* if y is an integer */
		    k = m-k*n;
		    sx=t; tx+=(t-sx); }
		else	{		/* if y is not an integer */    
		    k =m;
	 	    tx+=n*ln2lo;
#ifdef mac2
		    c=n*ln2hi;
		    sx=(c)+t;
		    tx+=(c-sx)+t; }
#else
		    sx=(c=n*ln2hi)+t; tx+=(c-sx)+t; }
#endif
	   /* end of checking whether k==y */

                sy=y; ty=y-sy;          /* y ~ sy + ty */
#ifdef tahoe
		s = (tahoe_tmp = sx)*sy-k*ln2hi;
#else	/* tahoe */
		s=(double)sx*sy-k*ln2hi;        /* (sy+ty)*(sx+tx)-kln2 */
#endif	/* tahoe */
		z=(tx*ty-k*ln2lo);
		tx=tx*sy; ty=sx*ty;
		t=ty+z; t+=tx; t+=s;
		c= -((((t-s)-tx)-ty)-z);

	    /* return exp(y*log(x)) */
		t += exp__E(t,c); return(scalb(one+t,m));
	     }
	/* end of if log(y*log(x)) > -60.0 */
	    
	    else
		/* exp(+- tiny) = 1 with inexact flag */
			{ln2hi+ln2lo; return(one);}
	    else if(copysign(one,y)*(n+invln2*t) <zero)
		/* exp(-(big#)) underflows to zero */
	        	return(scalb(one,-5000)); 
	    else
	        /* exp(+(big#)) overflows to INF */
	    		return(scalb(one, 5000)); 

}

#endif
