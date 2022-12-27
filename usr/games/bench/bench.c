#define VERSION 1.0

#define UNIX

#undef MACH25

#undef	NOENUM		/* Define if compiler has no enum's */
#undef	NOSTRUCTASSIGN	/* Define if compiler can't assign structures */

#define COUNT_BENCHMARK
#define FIB_BENCHMARK
#define FLOAT_BENCHMARK
#define INTEGER_BENCHMARK
#define SIEVE_BENCHMARK
#define QUICK_BENCHMARK
#define SAVAGE_BENCHMARK
#define WHETSTONE_BENCHMARK
#define DHRYSTONE_BENCHMARK

#ifdef UNIX
#define UNIX_BENCHMARK
#define TAK_BENCHMARK
#define PT_BENCHMARK
#define IOCALL_BENCHMARK
#endif

#ifdef MACH
#define MACH_BENCHMARK
#endif

#ifdef MACH25
#define mach_thread_self thread_self
#endif

#ifdef UNIX

#include <stdio.h>
#include <math.h>

long gettime()
{
  long now;
  (void)time(&now);
  return now;
}

void start_display(int display, char *name)
{
  if (display) {
    printf("%s: ", name);
    fflush(stdout);
  }
}

void finish_display(int display, long count, long seconds)
{
  if (display)
    printf("%.1f per second\n", ((float)count) / (float)seconds);
}

#endif /* BSD */

#ifdef COUNT_BENCHMARK

long count_benchmark(long count, int display)
{
  volatile long counter, start, seconds;

  start_display(display, "count");
  start = gettime();
  for (counter = count; counter; counter--);
  seconds = gettime() - start;
  finish_display(display, count, seconds);
  return seconds;
}

#endif /* COUNT_BENCHMARK */

#ifdef FIB_BENCHMARK

int fib(int x)
{
  return (x <= 2) ? 1 : (fib(x - 1) + fib(x - 2));
}

long fib_benchmark(long count, int display)
{
  volatile long counter, start, seconds;

  start_display(display, "fib");
  start = gettime();
  for (counter = count; counter; counter--) (void)fib(20);
  seconds = gettime() - start;
  finish_display(display, count, seconds);
  return seconds;
}

#endif FIB_BENCHMARK

#ifdef FLOAT_BENCHMARK

double float_crunch(double x, double y)
{
  volatile double a, b;

  a = x;
  b = y;
  a = a * b; a = a * b; a = a * b; a = a * b;
  a = a * b; a = a * b; a = a * b;
  a = a / b; a = a / b; a = a / b; a = a / b;
  a = a / b; a = a / b; a = a / b;
  return a;
}

long float_benchmark(long count, int display)
{
  volatile long counter, start, seconds;

  start_display(display, "float");
  start = gettime();
  for (counter = count; counter; counter--)
    (void)float_crunch(3.141597E0, 1.7839032E4);
  seconds = gettime() - start;
  finish_display(display, count, seconds);
  return seconds;
}

#endif /* FLOAT_BENCHMARK */

#ifdef INTEGER_BENCHMARK

long integer_crunch(int x, int y)
{
  volatile long a, b;

  a = x;
  b = y;
  a = a * b; a = a * b; a = a * b; a = a * b;
  a = a * b; a = a * b; a = a * b;
  a = a / b; a = a / b; a = a / b; a = a / b;
  a = a / b; a = a / b; a = a / b;
  return a;
}

long integer_benchmark(long count, int display)
{
  volatile long counter, start, seconds;

  start_display(display, "integer");
  start = gettime();
  for (counter = count; counter; counter--)
    (void)integer_crunch(31415970, 178390324);
  seconds = gettime() - start;
  finish_display(display, count, seconds);
  return seconds;
}

#endif /* INTEGER_BENCHMARK */

#ifdef SIEVE_BENCHMARK

int sieve(int size)
{
  int i, prime, k, count, iter;
  char *flags;
  extern char *alloca(unsigned count);

  flags = alloca(size + 1);
  count = 0;
  for (i = 0; i <= size; i++) flags[i] = 1;
  for (i = 0; i <= size; i++) {
    if (flags[i]) {
      prime = i + i + 3;
      for (k = i + prime; k <= size; k += prime) flags[k] = 0;
      count++;
    }
  }
  return count;
}

long sieve_benchmark(long count, int display)
{
  volatile long counter, start, seconds;

  start_display(display, "sieve");
  start = gettime();
  for (counter = count; counter; counter--) (void)sieve(8190);
  seconds = gettime() - start;
  finish_display(display, count, seconds);
  return seconds;
}

#endif /* SIEVE_BENCHMARK */

#ifdef QUICK_BENCHMARK

long seed;

long random(long size)
{
  seed = seed * 25173L + 13849L;
  return (seed % size);
}

quick(int lo, int hi, long *base)
{
  int i, j;
  long pivot, temp;

  if (lo<hi) {
    for (i=lo, j=hi-1, pivot=base[hi]; i<j;) {
      while (i<hi && base[i] <= pivot) ++i;
      while (j>lo && base[j] >= pivot) --j;
      if (i<j) {
        temp = base[i];
        base[i] = base[j];
        base[j] = temp;
      }
    }
    temp = base[i];
    base[i] = base[hi];
    base[hi] = temp;
    quick(lo, i-1, base);
    quick(i+1, hi, base);
  }
}

long quick_benchmark(long count, int display)
{
  volatile long counter, start, seconds;
  long buffer[1000];
  int i;

  start_display(display, "quick");
  start = gettime();
  for (counter = count; counter; counter--) {
    seed = 7L;
    for (i = 0; i < (sizeof(buffer) / sizeof(*buffer)); ++i) {
      buffer[i] = random(131072);
      if (buffer[i] < 0L) buffer[i] = -buffer[i];
    }
    quick(0, (sizeof(buffer) / sizeof(*buffer)) - 1, buffer);
  }
  seconds = gettime() - start;
  finish_display(display, count, seconds);
  return seconds;
}

#endif /* QUICK_BENCHMARK */

#ifdef SAVAGE_BENCHMARK

double savage(double a)
{
  return tan(atan(exp(log(sqrt(a * a))))) + 1.0;
}

long savage_benchmark(long count, int display)
{
  volatile long counter, start, seconds;

  start_display(display, "savage");
  start = gettime();
  for (counter = count; counter; counter--) (void)savage(1.0);
  seconds = gettime() - start;
  finish_display(display, count, seconds);
  return seconds;
}

#endif /* SAVAGE_BENCHMARK */

#ifdef WHETSTONE_BENCHMARK

/* Whetstone benchmark in C.  This program is a translation of the
 * original Algol version in "A Synthetic Benchmark" by H.J. Curnow
 * and B.A. Wichman in Computer Journal, Vol  19 #1, February 1976.
 * Used to test compiler optimization and floating point performance.
 * Compile by:		cc -O -s -o whet whet.c
 * or:			cc -O -DPOUT -s -o whet whet.c
 * if output is desired.
 */

double		x1, x2, x3, x4, x, y, z, t, t1, t2;
double 		e1[4];
int		i, j, k, l, n1, n2, n3, n4, n6, n7, n8, n9, n10, n11;

pa(e)
double e[4];
{
  register int j;

  j = 0;
lab:
  e[0] = (  e[0] + e[1] + e[2] - e[3] ) * t;
  e[1] = (  e[0] + e[1] - e[2] + e[3] ) * t;
  e[2] = (  e[0] - e[1] + e[2] + e[3] ) * t;
  e[3] = ( -e[0] + e[1] + e[2] + e[3] ) / t2;
  j += 1;
  if (j < 6) goto lab;
}

p3(x, y, z)
double x, y, *z;
{
  x  = t * (x + y);
  y  = t * (x + y);
  *z = (x + y) /t2;
}

p0()
{
  e1[j] = e1[k];
  e1[k] = e1[l];
  e1[l] = e1[j];
}

#ifdef POUT
pout(n, j, k, x1, x2, x3, x4)
int n, j, k;
double x1, x2, x3, x4;
{
  printf("%6d%6d%6d  %5 e  %5 e  %5 e  %5 e\n",	n, j, k, x1, x2, x3, x4);
}
#endif

void whetstone_loop(long count)
{
  volatile long counter;

  for (counter = count; counter; counter--) {
    /* initialize constants */
    t   =   0.499975;
    t1  =   0.50025;
    t2  =   2.0;
    /* set values of module weights */
    n1  =   0; n2  =  12;
    n3  =  14; n4  = 345;
    n6  = 210; n7  =  32;
    n8  = 899; n9  = 616;
    n10 =   0; n11 =  93;
    /* MODULE 1:  simple identifiers */
    x1 =  1.0;
    x2 = x3 = x4 = -1.0;
    for(i = 1; i <= n1; i += 1) {
      x1 = ( x1 + x2 + x3 - x4) * t;
      x2 = ( x1 + x2 - x3 - x4) * t;
      x3 = ( x1 - x2 + x3 + x4) * t;
      x4 = (-x1 + x2 + x3 + x4) * t;
    }
#ifdef POUT
    pout(n1, n1, n1, x1, x2, x3, x4);
#endif
    /* MODULE 2:  array elements */
    e1[0] =  1.0;
    e1[1] = e1[2] = e1[3] = -1.0;
    for (i = 1; i <= n2; i +=1) {
      e1[0] = ( e1[0] + e1[1] + e1[2] - e1[3]) * t;
      e1[1] = ( e1[0] + e1[1] - e1[2] + e1[3]) * t;
      e1[2] = ( e1[0] - e1[1] + e1[2] + e1[3]) * t;
      e1[3] = (-e1[0] + e1[1] + e1[2] + e1[3]) * t;
    }
#ifdef POUT
    pout(n2, n3, n2, e1[0], e1[1], e1[2], e1[3]);
#endif
    /* MODULE 3:  array as parameter */
    for (i = 1; i <= n3; i += 1) pa(e1);
#ifdef POUT
    pout(n3, n2, n2, e1[0], e1[1], e1[2], e1[3]);
#endif
    /* MODULE 4:  conditional jumps */
    j = 1;
    for (i = 1; i <= n4; i += 1) {
      if (j == 1) j = 2; else j = 3;
      if (j > 2) j = 0; else j = 1;
      if (j < 1) j = 1; else j = 0;
    }
#ifdef POUT
    pout(n4, j, j, x1, x2, x3, x4);
#endif
    /* MODULE 5:  omitted */
    /* MODULE 6:  integer arithmetic */
    j = 1;
    k = 2;
    l = 3;
    for (i = 1; i <= n6; i += 1) {
      j = j * (k - j) * (l -k);
      k = l * k - (l - j) * k;
      l = (l - k) * (k + j);
      e1[l - 2] = j + k + l;		/* C arrays are zero based */
      e1[k - 2] = j * k * l;
    }
#ifdef POUT
    pout(n6, j, k, e1[0], e1[1], e1[2], e1[3]);
#endif
    /* MODULE 7:  trig. functions */
    x = y = 0.5;
    for(i = 1; i <= n7; i +=1) {
      x = t * atan(t2*sin(x)*cos(x)/(cos(x+y)+cos(x-y)-1.0));
      y = t * atan(t2*sin(y)*cos(y)/(cos(x+y)+cos(x-y)-1.0));
    }
#ifdef POUT
    pout(n7, j, k, x, x, y, y);
#endif
    /* MODULE 8:  procedure calls */
    x = y = z = 1.0;
    for (i = 1; i <= n8; i +=1) p3(x, y, &z);
#ifdef POUT
    pout(n8, j, k, x, y, z, z);
#endif
    /* MODULE9:  array references */
    j = 1;
    k = 2;
    l = 3;
    e1[0] = 1.0;
    e1[1] = 2.0;
    e1[2] = 3.0;
    for(i = 1; i <= n9; i += 1) p0();
#ifdef POUT
    pout(n9, j, k, e1[0], e1[1], e1[2], e1[3]);
#endif
    /* MODULE10:  integer arithmetic */
    j = 2;
    k = 3;
    for(i = 1; i <= n10; i +=1) {
      j = j + k; k = j + k;
      j = k - j; k = k - j - j;
    }
#ifdef POUT
    pout(n10, j, k, x1, x2, x3, x4);
#endif
    /* MODULE11:  standard functions */
    x = 0.75;
    for(i = 1; i <= n11; i += 1) x = sqrt(exp(log(x) / t1));
#ifdef POUT
    pout(n11, j, k, x, x, x, x);
#endif
  }
}

long whetstone_benchmark(long count, int display)
{
  long start, seconds;
  start_display(display, "whetstone");
  start = gettime();
  whetstone_loop(count);
  seconds = gettime() - start;
  finish_display(display, count, seconds);
  return seconds;
}

#endif /* WHETSTONE_BENCHMARK */

#ifdef DHRYSTONE_BENCHMARK

/*
 *
 *	"DHRYSTONE" Benchmark Program
 *
 *	Version:	C/1.1, 12/01/84
 *
 *	Date:		PROGRAM updated 01/06/86, COMMENTS changed 01/31/87
 *
 *	Author:		Reinhold P. Weicker,  CACM Vol 27, No 10, 10/84 pg. 1013
 *			Translated from ADA by Rick Richardson
 *			Every method to preserve ADA-likeness has been used,
 *			at the expense of C-ness.
 *
 *	Compile:	cc -O dry.c -o drynr			: No registers
 *			cc -O -DREG=register dry.c -o dryr	: Registers
 *
 *	Defines:	Defines are provided for old C compiler's
 *			which don't have enums, and can't assign structures.
 *			The time(2) function is library dependant; Most
 *			return the time in seconds, but beware of some, like
 *			Aztec C, which return other units.
 *			The LOOPS define is initially set for 50000 loops.
 *			If you have a machine with large integers and is
 *			very fast, please change this number to 500000 to
 *			get better accuracy.  Please select the way to
 *			measure the execution time using the TIME define.
 *			For single user machines, time(2) is adequate. For
 *			multi-user machines where you cannot get single-user
 *			access, use the times(2) function.  Be careful to
 *			adjust the HZ parameter below for the units which
 *			are returned by your times(2) function.  You can
 *			sometimes find this in <sys/param.h>.  If you have
 *			neither time(2) nor times(2), use a stopwatch in
 *			the dead of the night.
 *			Use a "printf" at the point marked "start timer"
 *			to begin your timings. DO NOT use the UNIX "time(1)"
 *			command, as this will measure the total time to
 *			run this program, which will (erroneously) include
 *			the time to malloc(3) storage and to compute the
 *			time it takes to do nothing.
 *
 *	Run:		drynr; dryr
 *
 *	Results:	If you get any new machine/OS results, please send to:
 *
 *				ihnp4!castor!pcrat!rick
 *
 *			and thanks to all that do.
 *
 *	Note:		I order the list in increasing performance of the
 *			"with registers" benchmark.  If the compiler doesn't
 *			provide register variables, then the benchmark
 *			is the same for both REG and NOREG.
 *
 *	PLEASE:		Send complete information about the machine type,
 *			clock speed, OS and C manufacturer/version.  If
 *			the machine is modified, tell me what was done.
 *			On UNIX, execute uname -a and cc -V to get this info.
 *
 *	80x8x NOTE:	80x8x benchers: please try to do all memory models
 *			for a particular compiler.
 *
 *
 *	The following program contains statements of a high-level programming
 *	language (C) in a distribution considered representative:
 *
 *	assignments			53%
 *	control statements		32%
 *	procedure, function calls	15%
 *
 *	100 statements are dynamically executed.  The program is balanced with
 *	respect to the three aspects:
 *		- statement type
 *		- operand type (for simple data types)
 *		- operand access
 *			operand global, local, parameter, or constant.
 *
 *	The combination of these three aspects is balanced only approximately.
 *
 *	The program does not compute anything meaningfull, but it is
 *	syntactically and semantically correct.
 *
 */

/* for compatibility with goofed up version */
/*#undef GOOF			/* Define if you want the goofed up version */

#ifdef GOOF
char Version[] = "1.0";
#else
char Version[] = "1.1";
#endif

#ifdef NOSTRUCTASSIGN
#define	structassign(d, s) memcpy(&(d), &(s), sizeof(d))
#else
#define	structassign(d, s) d = s
#endif

#ifdef NOENUM
#define	Ident1 1
#define	Ident2 2
#define	Ident3 3
#define	Ident4 4
#define	Ident5 5
typedef int Enumeration;
#else
typedef enum {Ident1, Ident2, Ident3, Ident4, Ident5} Enumeration;
#endif

typedef int OneToThirty;
typedef int OneToFifty;
typedef char CapitalLetter;
typedef char String30[31];
typedef int Array1Dim[51];
typedef int Array2Dim[51][51];

struct	Record
{
  struct Record *PtrComp;
  Enumeration	Discr;
  Enumeration	EnumComp;
  OneToFifty	IntComp;
  String30	StringComp;
};

typedef struct Record 	RecordType;
typedef RecordType *	RecordPtr;
typedef int		boolean;

#ifndef REG
#define	REG
#endif

extern Enumeration	Func1();
extern boolean		Func2();

/*
 * Package 1
 */
int		IntGlob;
boolean		BoolGlob;
char		Char1Glob;
char		Char2Glob;
Array1Dim	Array1Glob;
Array2Dim	Array2Glob;
RecordPtr	PtrGlb;
RecordPtr	PtrGlbNext;

Proc1(PtrParIn)
REG RecordPtr	PtrParIn;
{
#define	NextRecord	(*(PtrParIn->PtrComp))

  structassign(NextRecord, *PtrGlb);
  PtrParIn->IntComp = 5;
  NextRecord.IntComp = PtrParIn->IntComp;
  NextRecord.PtrComp = PtrParIn->PtrComp;
  Proc3(NextRecord.PtrComp);
  if (NextRecord.Discr == Ident1) {
    NextRecord.IntComp = 6;
    Proc6(PtrParIn->EnumComp, &NextRecord.EnumComp);
    NextRecord.PtrComp = PtrGlb->PtrComp;
    Proc7(NextRecord.IntComp, 10, &NextRecord.IntComp);
  }
  else structassign(*PtrParIn, NextRecord);

#undef	NextRecord
}

Proc2(IntParIO)
OneToFifty *IntParIO;
{
  REG OneToFifty		IntLoc;
  REG Enumeration		EnumLoc;

  IntLoc = *IntParIO + 10;
  for(;;) {
    if (Char1Glob == 'A') {
      --IntLoc;
      *IntParIO = IntLoc - IntGlob;
      EnumLoc = Ident1;
    }
    if (EnumLoc == Ident1) break;
  }
}

Proc3(PtrParOut)
RecordPtr *PtrParOut;
{
  if (PtrGlb != 0) *PtrParOut = PtrGlb->PtrComp;
  else IntGlob = 100;
  Proc7(10, IntGlob, &PtrGlb->IntComp);
}

Proc4()
{
  REG boolean	BoolLoc;

  BoolLoc = Char1Glob == 'A';
  BoolLoc |= BoolGlob;
  Char2Glob = 'B';
}

Proc5()
{
  Char1Glob = 'A';
  BoolGlob = 0;
}

extern boolean Func3();

Proc6(EnumParIn, EnumParOut)
REG Enumeration	EnumParIn;
REG Enumeration	*EnumParOut;
{
  *EnumParOut = EnumParIn;
  if (! Func3(EnumParIn) ) *EnumParOut = Ident4;
  switch (EnumParIn)  {
  case Ident1:	*EnumParOut = Ident1; break;
  case Ident2:	if (IntGlob > 100) *EnumParOut = Ident1;
  		else *EnumParOut = Ident4;
  		break;
  case Ident3:	*EnumParOut = Ident2; break;
  case Ident4:	break;
  case Ident5:	*EnumParOut = Ident3;
  }
}

Proc7(IntParI1, IntParI2, IntParOut)
OneToFifty	IntParI1;
OneToFifty	IntParI2;
OneToFifty	*IntParOut;
{
  REG OneToFifty	IntLoc;

  IntLoc = IntParI1 + 2;
  *IntParOut = IntParI2 + IntLoc;
}

Proc8(Array1Par, Array2Par, IntParI1, IntParI2)
Array1Dim	Array1Par;
Array2Dim	Array2Par;
OneToFifty	IntParI1;
OneToFifty	IntParI2;
{
  REG OneToFifty	IntLoc;
  REG OneToFifty	IntIndex;

  IntLoc = IntParI1 + 5;
  Array1Par[IntLoc] = IntParI2;
  Array1Par[IntLoc+1] = Array1Par[IntLoc];
  Array1Par[IntLoc+30] = IntLoc;
  for (IntIndex = IntLoc; IntIndex <= (IntLoc+1); ++IntIndex)
    Array2Par[IntLoc][IntIndex] = IntLoc;
  ++Array2Par[IntLoc][IntLoc-1];
  Array2Par[IntLoc+20][IntLoc] = Array1Par[IntLoc];
  IntGlob = 5;
}

Enumeration Func1(CharPar1, CharPar2)
CapitalLetter	CharPar1;
CapitalLetter	CharPar2;
{
  REG CapitalLetter	CharLoc1;
  REG CapitalLetter	CharLoc2;

  CharLoc1 = CharPar1;
  CharLoc2 = CharLoc1;
  if (CharLoc2 != CharPar2) return (Ident1);
  else return (Ident2);
}

boolean Func2(StrParI1, StrParI2)
String30	StrParI1;
String30	StrParI2;
{
  REG OneToThirty		IntLoc;
  REG CapitalLetter	CharLoc;

  IntLoc = 1;
  while (IntLoc <= 1)
    if (Func1(StrParI1[IntLoc], StrParI2[IntLoc+1]) == Ident1) {
      CharLoc = 'A';
      ++IntLoc;
    }
  if (CharLoc >= 'W' && CharLoc <= 'Z') IntLoc = 7;
  if (CharLoc == 'X') return(1);
  else {
    if (strcmp(StrParI1, StrParI2) > 0) {
      IntLoc += 7;
      return (1);
    }
    else return (0);
  }
}

boolean Func3(EnumParIn)
REG Enumeration	EnumParIn;
{
  REG Enumeration	EnumLoc;

  EnumLoc = EnumParIn;
  if (EnumLoc == Ident3) return (1);
  return (0);
}

#ifdef	NOSTRUCTASSIGN
memcpy(d, s, l)
register char	*d;
register char	*s;
register int	l;
{
  while (l--) *d++ = *s++;
}
#endif

void dhrystone_loop(long count)
{
  volatile long counter;
  OneToFifty		IntLoc1;
  REG OneToFifty	IntLoc2;
  OneToFifty		IntLoc3;
  REG char		CharLoc;
  REG char		CharIndex;
  Enumeration	 	EnumLoc;
  String30		String1Loc;
  String30		String2Loc;
  extern char		*malloc(unsigned count);

  PtrGlbNext = (RecordPtr) malloc(sizeof(RecordType));
  PtrGlb = (RecordPtr) malloc(sizeof(RecordType));
  PtrGlb->PtrComp = PtrGlbNext;
  PtrGlb->Discr = Ident1;
  PtrGlb->EnumComp = Ident3;
  PtrGlb->IntComp = 40;
  strcpy(PtrGlb->StringComp, "DHRYSTONE PROGRAM, SOME STRING");
#ifndef	GOOF
  strcpy(String1Loc, "DHRYSTONE PROGRAM, 1'ST STRING");	/*GOOF*/
#endif
  Array2Glob[8][7] = 10;	/* Was missing in published program */
  for (counter = count; counter; counter--) {
    Proc5();
    Proc4();
    IntLoc1 = 2;
    IntLoc2 = 3;
    strcpy(String2Loc, "DHRYSTONE PROGRAM, 2'ND STRING");
    EnumLoc = Ident2;
    BoolGlob = ! Func2(String1Loc, String2Loc);
    while (IntLoc1 < IntLoc2) {
      IntLoc3 = 5 * IntLoc1 - IntLoc2;
      Proc7(IntLoc1, IntLoc2, &IntLoc3);
      ++IntLoc1;
    }
    Proc8(Array1Glob, Array2Glob, IntLoc1, IntLoc3);
    Proc1(PtrGlb);
    for (CharIndex = 'A'; CharIndex <= Char2Glob; ++CharIndex)
      if (EnumLoc == Func1(CharIndex, 'C'))
        Proc6(Ident1, &EnumLoc);
    IntLoc3 = IntLoc2 * IntLoc1;
    IntLoc2 = IntLoc3 / IntLoc1;
    IntLoc2 = 7 * (IntLoc3 - IntLoc2) - IntLoc1;
    Proc2(&IntLoc1);
  }
}

long dhrystone_benchmark(long count, int display)
{
  long start, seconds;

  start_display(display, "dhrystone");
  start = gettime();
  dhrystone_loop(count);
  seconds = gettime() - start;
  finish_display(display, count, seconds);
  return seconds;
}

#endif /* DHRYSTONE_BENCHMARK */

#ifdef UNIX_BENCHMARK

long unix_benchmark(long count, int display)
{
  volatile long counter, start, seconds;

  start_display(display, "unix");
  start = gettime();
  for (counter = count; counter; counter--) (void)getpid();
  seconds = gettime() - start;
  finish_display(display, count, seconds);
  return seconds;
}

#endif /* UNIX_BENCHMARK */

#ifdef MACH_BENCHMARK

long mach_benchmark(long count, int display)
{
  volatile long counter, start, seconds;

  start_display(display, "mach");
  start = gettime();
  for (counter = count; counter; counter--) (void)mach_thread_self();
  seconds = gettime() - start;
  finish_display(display, count, seconds);
  return seconds;
}

#endif /* MACH_BENCHMARK */

#ifdef TAK_BENCHMARK

/* This is primarily a test of subprogram linkage efficiency,
 * using recursion and a touch of super-simple integer arithmetic.
 * Paul Raveling USC-ISI
 */

int tak (int x, int y, int z)
{
  if (x <= y) return z;
  return tak(tak(x - 1, y, z), tak(y - 1, z, x), tak(z - 1, x, y));
}

long tak_benchmark(long count, int display)
{
  volatile long counter, start, seconds;

  start_display(display, "tak");
  start = gettime();
  for (counter = count; counter; counter--) (void)tak(18, 12, 6);
  seconds = gettime() - start;
  finish_display(display, count, seconds);
  return seconds;
}

#endif /* TAK_BENCHMARK */

#ifdef PT_BENCHMARK

/* Test kernel speed for context switches induced by interprocess
 * communication of short messages.  This particular test uses
 * pipes for the IPC, and is believed to be sufficiently "plain
 * vanilla" to run on any UNIX system. If the machine is suitably idle,
 * the reported system time (billed to only one of the 3 processes)
 * should be very close to real time / 3, and the CPU utilization
 * percentage should be very close to 33%.
 * Paul Raveling USC-ISI
 */

rpipe(pipefd, process, buffer, size)
int pipefd;
int process;
char *buffer;
int size;
{
  int i;

  i = read(pipefd, buffer, size);
  if (i == -1) {
    printf("Error reading pipe in process %d\n\n", process);
    return;
  }
  if (i < size) {
    printf("ir = %d\n", i);
    rpipe(pipefd, process, &buffer[i], size - i);
  }
}

wpipe(pipefd, process, buffer, size)
int pipefd;
int process;
char *buffer;
int size;
{
  int	i;

  i = write(pipefd, buffer, size);
  if (i == -1) {
    printf("Error writing pipe in process %d\n", process);
    return;
  }
  if (i < size) {
    printf("iw = %d\n", i);
    wpipe(pipefd, process, &buffer[i], size - i);
  }
}

void pt_loop(long count, int size)
{
  volatile long counter;
  int pipe1[2], pipe2[2], pipe3[2];
  int self, child1, child2;
  int i;
  char *msg;
  extern char *malloc(unsigned count);

  if (pipe(pipe1)) { perror("pipe 1\n"); exit(1); }
  if (pipe(pipe2)) { perror("pipe 2\n"); exit(1); }
  if (pipe(pipe3)) { perror("pipe 3\n"); exit(1); }
  if (i = fork()) {
    child1 = i;
    if (i = fork()) {
      child2 = i;
      self = 0;
    }
    else self = 2;
  }
  else self = 1;
  if ((msg = malloc(size)) == 0) {
    printf("Can't allocate buffer for process %d\n\n", self);
    exit(1);
  }
  switch (self) {
    case 0:
      for (i=0; i<size; ++i) msg[i] = i + '0';
      wpipe(pipe1[1], self, msg, size);
      rpipe(pipe3[0], self, msg, size);
      for (counter = count; counter; counter--) {
        wpipe(pipe1[1], self, msg, size);
        rpipe(pipe3[0], self, msg, size);
      }
      kill(child1, 9);
      kill(child2, 9);
      close(pipe1[0]);
      close(pipe2[0]);
      close(pipe3[0]);
      close(pipe1[1]);
      close(pipe2[1]);
      close(pipe3[1]);
      break;
    case 1:
      while (1) {
        rpipe(pipe1[0], self, msg, size);
        wpipe(pipe2[1], self, msg, size);
      }
    case 2:
      while (1)	{
        rpipe(pipe2[0], self, msg, size);
        wpipe(pipe3[1], self, msg, size);
      }
  }
  free(msg);
}

long pt_benchmark(long count, int display)
{
  long start, seconds;

  start_display(display, "pt");
  start = gettime();
  pt_loop(count, 10);
  seconds = gettime() - start;
  finish_display(display, count, seconds);
  return seconds;
}

#endif /* PT_BENCHMARK */

#ifdef IOCALL_BENCHMARK

/* This benchmark tests speed of Unix system call interface
 * and speed of cpu doing common Unix io system calls.
 * Paul Raveling USC-ISI
 */

iocall_loop(long count)
{
  volatile long counter;
  char buf[512];
  int fd, n, i;

  fd = creat("/tmp/testfile", 0777);
  close(fd);
  fd = open("/tmp/testfile", 2);
  unlink("/tmp/testfile");
  for (counter = count; counter; counter--) {
    lseek(fd, 0L, 0);
    n = write(fd, buf, 500);
    lseek(fd, 0L, 0);
    for (i = 0; i <= 3; i++) n = read(fd, buf, 100);
  }
}

long iocall_benchmark(long count, int display)
{
  long start, seconds;

  start_display(display, "iocall");
  start = gettime();
  iocall_loop(count);
  seconds = gettime() - start;
  finish_display(display, count, seconds);
  return seconds;
}

#endif /* IOCALL_BENCHMARK */

/* run a benchmark function for specified number of seconds */
void run(long (*function)(long count, int display), int seconds)
{
  long count;

  /* determine count that runs for at least a second */
  for (count = 1; (*function)(count, 0) < 4; count *= 2);
  count /= 4;

  /* run benchmark for specified number of seconds */
  (void)(*function)(count * seconds, 1);

} /* run() */

main(argc, argv)
int argc;
char **argv;
{
  int seconds;

  printf("bench version %s\n", VERSION);

  /* skip program name */
  argc--;
  argv++;

  if ((argc < 1) || (argc > 2)) {
    printf("usage: bench <seconds> [ <option> ]\n");
    exit(1);
  }

  seconds = atoi(*argv);
  argc--;
  argv++;

#ifdef COUNT_BENCHMARK
  if (!argc || !strcmp(*argv, "count")) run(count_benchmark, seconds);
#endif

#ifdef FIB_BENCHMARK
  if (!argc || !strcmp(*argv, "fibonacci")) run(fib_benchmark, seconds);
#endif

#ifdef FLOAT_BENCHMARK
  if (!argc || !strcmp(*argv, "float")) run(float_benchmark, seconds);
#endif

#ifdef INTEGER_BENCHMARK
  if (!argc || !strcmp(*argv, "integer")) run(integer_benchmark, seconds);
#endif

#ifdef SIEVE_BENCHMARK
  if (!argc || !strcmp(*argv, "sieve")) run(sieve_benchmark, seconds);
#endif

#ifdef QUICK_BENCHMARK
  if (!argc || !strcmp(*argv, "quick")) run(quick_benchmark, seconds);
#endif

#ifdef SAVAGE_BENCHMARK
  if (!argc || !strcmp(*argv, "savage")) run(savage_benchmark, seconds);
#endif

#ifdef WHETSTONE_BENCHMARK
  if (!argc || !strcmp(*argv, "whetstone")) run(whetstone_benchmark, seconds);
#endif

#ifdef DHRYSTONE_BENCHMARK
  if (!argc || !strcmp(*argv, "dhrystone")) run(dhrystone_benchmark, seconds);
#endif

#ifdef UNIX_BENCHMARK
  if (!argc || !strcmp(*argv, "unix")) run(unix_benchmark, seconds);
#endif

#ifdef MACH_BENCHMARK
  if (!argc || !strcmp(*argv, "mach")) run(mach_benchmark, seconds);
#endif

#ifdef TAK_BENCHMARK
  if (!argc || !strcmp(*argv, "tak")) run(tak_benchmark, seconds);
#endif

#ifdef PT_BENCHMARK
  if (!argc || !strcmp(*argv, "pt")) run(pt_benchmark, seconds);
#endif

#ifdef IOCALL_BENCHMARK
  if (!argc || !strcmp(*argv, "iocall")) run(iocall_benchmark, seconds);
#endif

} /* main() */
