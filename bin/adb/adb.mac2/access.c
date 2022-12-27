/* adb - access data in file/process address space */

/* The routines in this file access referenced data using
 * the maps to access files, ptrace to access subprocesses,
 * or the system page tables when debugging the kernel,
 * to translate virtual to physical addresses.
 */

#ifdef mach3
char *mach3_no_ptrace = "Mach 3.0 does not support ptrace.";
#endif

#include "defs.h"

/* access modes */
#define RD 0
#define WT 1

/* is address in range: lbd <= addr < ubd */
#define within(addr, lbd, ubd) ((addr >= lbd) && (addr < ubd))

/*
 * Read/write according to mode at address addr in i/d space.
 * Value is quantity to be written, if write.
 *
 * This routine decides whether to get the data from the subprocess
 * address space with ptrace, or to get it from the files being
 * debugged.  
 */
static int access(const int mode, const off_t addr, const int space,
  u_int w_value)
{
  static char errmsg[80];
  MAPPTR amap;   /* txtmap or datmap */
  u_int r_value; /* read value */
  off_t p_addr;  /* physical address */
  int pmode;     /* mode for ptrace() */
  extern STRING errflg;
  extern MAP txtmap, datmap;
  extern int errno;
  extern INT wtflag;
  extern L_INT pid;

  if (mode == WT && !wtflag) error("not in write mode");
  if (space == NSP) return 0;
  if (pid) {
#ifdef mach3
  errflg = mach3_no_ptrace;
  return 0;
#else
    pmode = (space & DSP) ? ((mode == RD) ? RDUSER : WDUSER) :
      ((mode == RD) ? RIUSER : WIUSER);
    r_value = ptrace(pmode, pid, addr, w_value);
    if (errno) {
      sprintf(errmsg, "ptrace %s %s address 0x%08X failed",
        (mode == RD) ? "read" : "write",
        (space & DSP) ? "/" : "?", addr);
      errflg = errmsg;
      return 0;
    }
    return r_value;
#endif
  }
  amap = (space & DSP) ? &datmap : &txtmap;
#ifdef mac2
  /* non-symbolic address entered, treat it as physical address */
  if (kernel && !s_addr) p_addr = addr;
  else
#endif
  if ((space & STAR) || !within(addr, amap->b1, amap->e1)) {
    if (!within(addr, amap->b2, amap->e2)) {
      sprintf(errmsg, "invalid %s address 0x%08X",
        (space & DSP) ? "/" : "?", addr);
      errflg = errmsg;
      return(0);
    }
    p_addr = addr + (amap->f2)-(amap->b2);
  }
  else p_addr = addr + (amap->f1) - (amap->b1);
  if (lseek(amap->ufd, p_addr, 0) != p_addr) {
    sprintf(errmsg, "seek to physical %s address 0x%08X failed",
      (space & DSP) ? "/" : "?", p_addr);
    errflg = errmsg;
    return(0);
   }
  if (mode == RD) {
    if (read(amap->ufd, (char *)&r_value, sizeof(r_value)) == sizeof(r_value))
      return r_value;
  }
  else {
    if (write(amap->ufd, (char *)&w_value, sizeof(w_value)) != sizeof(w_value))
     return 0;
  }
  sprintf(errmsg, "%s at physical %s address 0x%08X failed",
    (mode == RD) ? "read" : "write", (space & DSP) ? "/" : "?", p_addr);
  errflg = errmsg;
  return 0;
}

/*
 * Primitives: put a value in a space, get a value from a space
 * and get a word or byte not returning if an error occurred.
 */
void put(const off_t addr, const int space, u_int value) 
{
  (void)access(WT, addr, space, value);
}

u_int get(const off_t addr, const int space)
{
  return access(RD, addr, space, 0);
}

u_int chkget(const off_t addr, const int space)
{
   u_int w_value;

   w_value = get(addr, space);
   chkerr();
   return w_value;
}

u_int bchkget(off_t addr, int space) 
{
  return (u_int)byte(chkget(addr, space));
}

