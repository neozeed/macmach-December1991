/* adb - routines to read a.out and core files at startup */

#include <stdio.h>
#include <mach/machine/frame.h>
#include <ctype.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/file.h>
#ifndef mach3
#include <machine/pcb.h>
#endif
#include "defs.h"

off_t	txtbas;
off_t	datbas;		/* offset of the base of the data segment */
off_t	stksiz;		/* stack size in the core image */
INT	sigcode;	/* belongs in head.h */

char	*symfil	= "a.out";
char	*corfil	= "core";

static int getfile(char *filnam, int cnt)
{
  register int fsym;

  if (eqstr(filnam, "-")) return -1;
  fsym = open(filnam, wtflag);
  if (fsym < 0 && xargc > cnt) {
    if (wtflag) {
      if ((fsym = creat(filnam, 0644)) < 0) return -1;
      close(fsym);
      fsym = open(filnam, wtflag);
    }
    if (fsym < 0) printf("cannot open `%s'\n", filnam);
  }
  return fsym;
}

void setsym()
{
  off_t loc;
  struct exec hdr;
  register struct nlist *sp;
  int ssiz;
  char *strtab, *malloc();

  fsym = getfile(symfil, 1);
  txtmap.ufd = fsym;
  if (read(fsym, (char *)&hdr, sizeof hdr) != sizeof hdr || N_BADMAG(hdr)) {
    txtmap.e1 = MAXFILE;
    return;
  }
  filhdr = hdr;
  loc = filhdr.a_text+filhdr.a_data;
  txtbas = (filhdr.a_magic == ZMAGIC) ? N_TXTADDR(filhdr) : filhdr.a_entry;
  txtmap.f1 = txtmap.f2 = N_TXTOFF(filhdr);
  txtmap.b1 = 0;
  switch (filhdr.a_magic) {
    case OMAGIC:
      txtmap.b1 = txtmap.e1 = 0;
      txtmap.b2 = datbas = txtbas;
      txtmap.e2 = loc+txtbas;
      break;
    case ZMAGIC:
    case NMAGIC:
      txtmap.b1 = txtbas;
      txtmap.e1 = filhdr.a_text+txtbas;
      txtmap.b2 = datbas = round(filhdr.a_text, SEGSIZ);
      txtmap.e2 = datbas + filhdr.a_data;
      txtmap.f2 += filhdr.a_text;
  }
  loc = N_SYMOFF(filhdr);
  symtab = (struct nlist *) malloc(filhdr.a_syms);
  esymtab = &symtab[filhdr.a_syms / sizeof (struct nlist)];
  if (symtab == NULL) goto nospac;
  lseek(fsym, loc, L_SET);
  if (filhdr.a_syms == 0) goto nosymt;
  /* SHOULD SQUISH OUT STABS HERE!!! */
  if (read(fsym, symtab, filhdr.a_syms) != filhdr.a_syms) goto readerr;
  if (read(fsym, &ssiz, sizeof (ssiz)) != sizeof (ssiz)) goto oldfmt;
  strtab = malloc(ssiz);
  if (strtab == 0) goto nospac;
  *(int *)strtab = ssiz;
  ssiz -= sizeof (ssiz);
  if (read(fsym, strtab + sizeof (ssiz), ssiz) != ssiz) goto readerr;
  for (sp = symtab; sp < esymtab; sp++)
    if (sp->n_un.n_strx)
      /* SHOULD PERFORM RANGE CHECK HERE */
      sp->n_un.n_name = strtab + sp->n_un.n_strx;
nosymt:
  return;
readerr:
  printf("Error reading symbol|string table\n");
  exit(1);
nospac:
  printf("Not enough space for symbol|string table\n");
  exit(1);
oldfmt:
  printf("Old format a.out - no string table\n");
  exit(1);
}

void setcor()
{
#ifdef mac2
  struct pcb *pcb;
  int frame;
  unsigned long base;
#endif

  bzero(&datmap, sizeof(datmap));

  fcor = datmap.ufd = getfile(corfil, 2);

  if (kernel) {

#ifdef mac2

    /* first word of /dev/kmem is kernel relocation base */
    if (read(fcor, &base, sizeof(base)) != sizeof(base)) {
      printf("Can not read kernel base\n");
      exit(1);
    }
    /* add base to symbol value to get actual address */
    datmap.f1 = base;
    /* valid symbol values are 0 - (0xFFFFFFFF - base) */
    datmap.b1 = 0;
    datmap.e1 = 0xFFFFFFFF - base;
    /* there is no secondary mapping */
    datmap.f2 = datmap.f1;
    datmap.b2 = datmap.b1;
    datmap.e2 = datmap.e1;
    return;

#endif

  } /* if (kernel) */

  /* read u structure at beginning of core file */
  if (read(fcor, (char *)&u, ctob(UPAGES)) != ctob(UPAGES)) {
    datmap.e1 = MAXFILE;
    return;
  }
  signo = u.u_arg[0];
  sigcode = u.u_code;
  filhdr.a_text = ctob(u.u_tsize);
  filhdr.a_data = ctob(u.u_dsize);
  stksiz = ctob(u.u_ssize);
  switch (filhdr.a_magic) {
    case OMAGIC:
      datmap.b1 = txtbas;
      datmap.e1 = datmap.b1 + filhdr.a_text+filhdr.a_data;
      datmap.f2 = ctob(UPAGES) + filhdr.a_text+filhdr.a_data;
      break;
    case NMAGIC:
    case ZMAGIC:
      datmap.b1 = round(filhdr.a_text, SEGSIZ);
      datmap.e1 = datmap.b1 + filhdr.a_data;
      datmap.f2 = ctob(UPAGES) + filhdr.a_data;
      break;
  }
  datmap.f1 = ctob(UPAGES);
  datmap.b2 = MAXSTOR - stksiz;
  datmap.e2 = MAXSTOR;
#ifdef mac2
#ifndef mach3
  /* registers are stored in pcb structure */
  pcb = (struct pcb *)&udot.UU[NBPG];
  bcopy(&pcb->pcb_user, adbinfo.userregs, sizeof pcb->pcb_user);
  bcopy(&pcb->pcb_fp_state, adbinfo.fpregs, sizeof adbinfo.fpregs);
  /* stack pointer and pc are in frame structure */
  frame = ((int)pcb->pcb_frame & (NBPG-1)) + NBPG;
  adbinfo.userregs[PS] = *((u_short *)&udot.UU[frame]);
  adbinfo.userregs[PC] = *((int *)&udot.UU[frame+2]);
#endif
#endif mac2

} /* setcor() */

void setvar()
{
  var[varchk('b')] = datbas;
  var[varchk('d')] = filhdr.a_data;
  var[varchk('e')] = filhdr.a_entry;
  var[varchk('m')] = filhdr.a_magic;
  var[varchk('s')] = stksiz;
  var[varchk('t')] = filhdr.a_text;
}
