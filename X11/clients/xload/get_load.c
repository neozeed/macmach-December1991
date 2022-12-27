/*
 * Copyright 1989 Massachusetts Institute of Technology
 *
 * Permission to use, copy, modify, distribute, and sell this software and its
 * documentation for any purpose is hereby granted without fee, provided that
 * the above copyright notice appear in all copies and that both that
 * copyright notice and this permission notice appear in supporting
 * documentation, and that the name of M.I.T. not be used in advertising or
 * publicity pertaining to distribution of the software without specific,
 * written prior permission.  M.I.T. makes no representations about the
 * suitability of this software for any purpose.  It is provided "as is"
 * without express or implied warranty.
 *
 * M.I.T. DISCLAIMS ALL WARRANTIES WITH REGARD TO THIS SOFTWARE, INCLUDING ALL
 * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS, IN NO EVENT SHALL M.I.T.
 * BE LIABLE FOR ANY SPECIAL, INDIRECT OR CONSEQUENTIAL DAMAGES OR ANY DAMAGES
 * WHATSOEVER RESULTING FROM LOSS OF USE, DATA OR PROFITS, WHETHER IN AN ACTION
 * OF CONTRACT, NEGLIGENCE OR OTHER TORTIOUS ACTION, ARISING OUT OF OR IN
 * CONNECTION WITH THE USE OR PERFORMANCE OF THIS SOFTWARE.
 *
 * Authors:  Many and varied...
 */

#include <stdio.h>
#include <X11/Xos.h>
#include <X11/Intrinsic.h>

#ifdef att
#define LOADSTUB
#endif

#ifdef apollo
#include <apollo/base.h>
#include <apollo/time.h>
typedef struct {
	short		state;		/* ready, waiting, etc. */
	pinteger	usr;		/* user sr */
	linteger	upc;		/* user pc */
	linteger	usp;		/* user stack pointer */
	linteger	usb;		/* user sb ptr (A6) */
	time_$clock_t	cpu_total;	/* cumulative cpu used by process */
	unsigned short	priority;	/* process priority */
    } proc1_$info_t;

void proc1_$get_cput(
	time_$clock_t	*cput
);

void proc1_$get_info(
	short		&pid,
	proc1_$info_t	*info,
	status_$t	*sts
);
#endif /* apollo */

#ifndef macII
#ifndef apollo
#ifndef LOADSTUB
#include <nlist.h>
#endif /* LOADSTUB */
#endif /* apollo */
#endif /* macII */

#ifdef sun
#    include <sys/param.h>
#    ifdef i386
#        include <kvm.h>
#        define	KVM_ROUTINES
#    endif /* i386 */
#endif

#ifdef mips
#include <sys/fixpoint.h>
#endif

#ifdef	CRAY
#include <sys/param.h>
#include <sys/sysinfo.h>
#undef n_type
#define n_type n_value
#endif	/* CRAY */

#ifdef sequent
#include <sys/vm.h>
#endif /* sequent */

#ifdef macII
#include <a.out.h>
#include <sys/var.h>
#define X_AVENRUN 0
#define fxtod(i) (vec[i].high+(vec[i].low/65536.0))
struct lavnum {
    unsigned short high;
    unsigned short low;
};
#endif /* macII */

#if defined(UTEK) || defined(alliant)
#define FSCALE	100.0
#endif

#ifdef sequent
#define FSCALE	1000.0
#endif

extern long lseek();
extern void exit();

#ifdef mac2
#include <sys/table.h>
/* ARGSUSED */
void
GetLoadPoint( w, closure, call_data )
Widget  w;              /* unused */
XtPointer closure;      /* unused */
XtPointer call_data;    /* pointer to (double) return value */
{
  double avg;
  struct {
    union {
      long   tl_lavenrun[3];
      double tl_davenrun[3];
    } tl_un;
    int tl_lscale;
  } tbl_loadavg;

  if (table(TBL_LOADAVG, 0, &tbl_loadavg, 1, sizeof(tbl_loadavg)) != 1)
      avg = 0.0;
  else
  {
    if (tbl_loadavg.tl_lscale)
      avg = ( (double)(tbl_loadavg.tl_un.tl_lavenrun[0]) ) / ( (double)(tbl_loadavg.tl_lscale) );
    else
      avg = tbl_loadavg.tl_un.tl_davenrun[0];
  }
  *(double *)call_data =  avg;
}

#else /* mac2 */
#if apollo
/* ARGSUSED */
void GetLoadPoint( w, closure, call_data )
     Widget	w;		/* unused */
     caddr_t	closure;	/* unused */
     caddr_t	call_data;	/* pointer to (double) return value */
{
     static Bool    firstTime = TRUE;
     static int     lastNullCpu;
     static int     lastClock;
     time_$clock_t  timeNow;
     double         temp;
     proc1_$info_t  info;
     status_$t      st;

     proc1_$get_info( (short) 2, &info, &st );
     time_$clock( &timeNow );

     if (firstTime)
     {
     	*(double *)call_data = 1.0;
         firstTime = FALSE;
     }
     else {
         temp = info.cpu_total.low32 - lastNullCpu;
     	*(double *)call_data = 1.0 - temp / (timeNow.low32 - lastClock);
     }

     lastClock = timeNow.low32;
     lastNullCpu = info.cpu_total.low32;
}
#else
#ifdef KVM_ROUTINES
/*
 *	Sun 386i Code - abstracted to see the wood for the trees
 */

/* ARGSUSED */
void 
GetLoadPoint( w, closure, call_data )
Widget	w;		/* unused */
XtPointer closure;	/* unused */
XtPointer call_data;	/* pointer to (double) return value */
{
    double *loadavg = (double *)call_data;
    long	temp;
    static int init = 0;
    static struct nlist nl[2];
    static kvm_t *kd;
    
    if (!init) {
	kd = kvm_open("/vmunix", NULL, NULL, O_RDONLY, "Load Widget");
	if (kd == (kvm_t *)0) {
	    xload_error("cannot get access to kernel address space");
	}
	
	nl[0].n_name = "avenrun";
	nl[1].n_name = NULL;
	
	if (kvm_nlist(kd, nl) != 0) {
	    xload_error("cannot get name list");
	}
	init = 1;
    }
    
    if (nl[0].n_value == 0) {
	xload_error("Cannot find address for avenrun in the kernel\n");
    }
    if (kvm_read(kd, nl[0].n_value, (char *)&temp, sizeof (temp)) != 
	sizeof (temp)) {
	xload_error("Kernel read error");
    }
    *loadavg = (double)temp/FSCALE;
}
#else /* KVM_ROUTINES */
#ifdef LOADSTUB

/* ARGSUSED */
void GetLoadPoint( w, closure, call_data )
     Widget	w;		/* unused */
     caddr_t	closure;	/* unused */
     caddr_t	call_data;	/* pointer to (double) return value */
{
	*(double *)call_data = 1.0;
}

#else /* LOADSTUB */

#ifndef KMEM_FILE
#define KMEM_FILE "/dev/kmem"
#endif

#ifndef KERNEL_FILE

#ifdef alliant
#define KERNEL_FILE "/unix"
#endif /* alliant */

#ifdef CRAY
#define KERNEL_FILE "/unicos"
#endif /* CRAY */

#ifdef hpux
#define KERNEL_FILE "/hp-ux"
#endif /* hpux */

#ifdef macII
#define KERNEL_FILE "/unix"
#endif /* macII */

#ifdef mips
# ifdef SYSTYPE_SYSV
# define KERNEL_FILE "/unix"
# else
# define KERNEL_FILE "/vmunix"
# endif /* SYSTYPE_SYSV */
#endif /* mips */

#ifdef sequent
#define KERNEL_FILE "/dynix"
#endif /* sequent */

/*
 * provide default for everyone else
 */
#ifndef KERNEL_FILE
#ifdef SYSV
#define KERNEL_FILE "/unix"
#else
#define KERNEL_FILE "/vmunix"
#endif /* SYSV */
#endif /* KERNEL_FILE */
#endif /* KERNEL_FILE */


#ifndef KERNEL_LOAD_VARIABLE
#    ifdef alliant
#        define KERNEL_LOAD_VARIABLE "_Loadave"
#    endif /* alliant */

#    ifdef CRAY
#        if defined(CRAY2) && OSMAJORVERSION == 4
#            define KERNEL_LOAD_VARIABLE "avenrun"
#        else
#            define KERNEL_LOAD_VARIABLE "sysinfo"
#            define SYSINFO
#        endif /* defined(CRAY2) && OSMAJORVERSION == 4 */
#    endif /* CRAY */

#    ifdef hpux
#        ifdef hp9000s800
#            define KERNEL_LOAD_VARIABLE "avenrun"
#        endif /* hp9000s800 */
#    endif /* hpux */

#    ifdef mips
#        ifdef SYSTYPE_SYSV
#            define KERNEL_LOAD_VARIABLE "avenrun"
#        else
#            define KERNEL_LOAD_VARIABLE "_avenrun"
#        endif /* SYSTYPE_SYSV */
#    endif /* mips */

#    ifdef sequent
#        define KERNEL_FILE "/dynix"
#    endif /* sequent */

/*
 * provide default for everyone else
 */

#    ifdef USG
#        define KERNEL_LOAD_VARIABLE "sysinfo"
#        define SYSINFO
#    else
#        define KERNEL_LOAD_VARIABLE "_avenrun"
#    endif
#endif /* KERNEL_LOAD_VARIABLE */

#ifdef macII
struct var v;
static int pad[2];	/* This padding is needed if xload compiled on */
			/* a/ux 1.1 is executed on a/ux 1.0, because */
			/* the var structure had too much padding in 1.0, */
			/* so the 1.0 kernel writes past the end of the 1.1 */
			/* var structure in the uvar() call. */
static struct nlist nl[2];
static struct lavnum vec[3];
#else /* not macII */
static struct nlist namelist[] = {	    /* namelist for vmunix grubbing */
#define LOADAV 0
    {KERNEL_LOAD_VARIABLE},
    {0}
};
#endif /* macII */


/* ARGSUSED */
void GetLoadPoint( w, closure, call_data )
     Widget	w;		/* unused */
     caddr_t	closure;	/* unused */
     caddr_t	call_data;	/* pointer to (double) return value */
{
  	double *loadavg = (double *)call_data;
	static int init = 0;
	static kmem;
	static long loadavg_seek;
	static xload_error();
#ifdef macII
        extern nlist();

        if(!init)   {
            int i;

            strcpy(nl[0].n_name, "avenrun");
            nl[1].n_name[0] = '\0';

            kmem = open(KMEM_FILE, O_RDONLY);
            if (kmem < 0) {
	        xload_error("cannot open", KMEM_FILE);
            }

            uvar(&v);

            if (nlist( KERNEL_FILE, nl) != 0) {
		xload_error("cannot get name list from", KERNEL_FILE);
            }
            for (i = 0; i < 2; i++) {
                nl[i].n_value = (int)nl[i].n_value - v.v_kvoffset;
            }
            init = 1;
        }
#else /* not macII */
	extern void nlist();
	
	if(!init)   {
	    nlist( KERNEL_FILE, namelist);
/*
 * Some systems appear to set only one of these to Zero if the entry could
 * not be found, I hope no_one returns Zero as a good value, or bad things
 * will happen to you.  (I have a hard time believing the value will
 * ever really be zero anyway).   CDP 5/17/89.
 */
	    if (namelist[LOADAV].n_type == 0 ||
		namelist[LOADAV].n_value == 0) {
		xload_error("cannot get name list from", KERNEL_FILE);
		exit(-1);
	    }
	    loadavg_seek = namelist[LOADAV].n_value;
#if defined(mips) && defined(SYSTYPE_SYSV)
	    loadavg_seek &= 0x7fffffff;
#endif /* mips && SYSTYPE_SYSV */
#if (defined(CRAY) && defined(SYSINFO))
 	    loadavg_seek += ((char *) (((struct sysinfo *)NULL)->avenrun)) -
	                    ((char *) NULL);
#endif /* SYSINFO */
  
	    kmem = open(KMEM_FILE, O_RDONLY);
	    if (kmem < 0) xload_error("cannot open", KMEM_FILE);
	    init = 1;
	}
	

	(void) lseek(kmem, loadavg_seek, 0);
#endif /* macII */
#if defined(sun) || defined (UTEK) || defined(sequent) || defined(alliant)
	{
		long temp;
		(void) read(kmem, (char *)&temp, sizeof(long));
		*loadavg = (double)temp/FSCALE;
	}
#else /* else not sun */
# ifdef macII
        {
                lseek(kmem, (long)nl[X_AVENRUN].n_value, 0);
                read(kmem, vec, 3*sizeof(struct lavnum));
                *loadavg = fxtod(0);
        }
# else /* else not macII */
#  ifdef mips
	{
		fix temp;
		(void) read(kmem, (char *)&temp, sizeof(fix));
		*loadavg = FIX_TO_DBL(temp);
	}
#  else /* not mips */
	(void) read(kmem, (char *)loadavg, sizeof(double));
#  endif /* mips */
# endif /* macII */
#endif /* sun */
	return;
}
#endif /* LOADSTUB */
#endif /* KVM_ROUTINES */
#endif /* apollo */
#endif /* mac2 */

static xload_error(str1, str2)
char *str1, *str2;
{
    (void) fprintf(stderr,"xload: %s %s\n", str1, str2);
    exit(-1);
}

