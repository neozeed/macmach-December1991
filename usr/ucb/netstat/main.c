/*
 **********************************************************************
 * HISTORY
 * $Log:	main.c,v $
 * Revision 2.4  90/02/02  23:40:42  bww
 * 	Excluded the SUN from the "kflag" code.
 * 	[90/02/02  23:40:07  bww]
 * 
 * Revision 2.3  89/10/24  19:17:33  bww
 * 	Leave the default address family unspecified.  Add 'H'
 * 	to getopt() argument.
 * 	[89/10/24  19:17:10  bww]
 * 
 * Revision 2.2  89/10/24  19:02:21  bww
 * 	Incorporated CMU changes, including: Fix klseek for sun,
 * 	rt, ..., added setnetent(1) call, added call to vmnlist()
 * 	routine before calling nlist(), and added -H option to
 * 	display ARP cache.
 * 	[89/10/24            bww]
 * 
 */
/*
 * Copyright (c) 1983, 1988 Regents of the University of California.
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
 */

#ifndef lint
char copyright[] =
"@(#) Copyright (c) 1983, 1988 Regents of the University of California.\n\
 All rights reserved.\n";
#endif /* not lint */

#ifndef lint
static char sccsid[] = "@(#)main.c	5.14 (Berkeley) 6/29/88";
#endif /* not lint */

#include <sys/param.h>
#ifndef mac2
#include <sys/vmmac.h>
#endif
#include <sys/socket.h>
#include <sys/file.h>
#include <ctype.h>
#include <errno.h>
#include <netdb.h>
#include <nlist.h>
#include <stdio.h>

#ifdef mac2
vm_offset_t	kload;
#endif

struct nlist nl[] = {
#define	N_MBSTAT	0
	{ "_mbstat" },
#define	N_IPSTAT	1
	{ "_ipstat" },
#define	N_TCB		2
	{ "_tcb" },
#define	N_TCPSTAT	3
	{ "_tcpstat" },
#define	N_UDB		4
	{ "_udb" },
#define	N_UDPSTAT	5
	{ "_udpstat" },
#define	N_RAWCB		6
	{ "_rawcb" },
#if	defined(vax)
#define	N_SYSMAP	7
	{ "_Sysmap" },
#define	N_SYSSIZE	8
	{ "_Syssize" },
#elif	defined(ibmrt)
#define	N_HATIPT	7
	{ "_MMU_HATIPT" },
#define	N_HASHMASK	8
	{ "_MMU_HASHMASK" },
#else
	{ "_mpid" },
	{ "_mpid" },
#endif
#define	N_IFNET		9
	{ "_ifnet" },
#define	N_IMP		10
	{ "_imp_softc" },
#define	N_RTHOST	11
	{ "_rthost" },
#define	N_RTNET		12
	{ "_rtnet" },
#define	N_ICMPSTAT	13
	{ "_icmpstat" },
#define	N_RTSTAT	14
	{ "_rtstat" },
#define	N_NFILE		15
	{ "_nfile" },
#define	N_FILE		16
	{ "_file" },
#define	N_UNIXSW	17
	{ "_unixsw" },
#define N_RTHASHSIZE	18
	{ "_rthashsize" },
#define N_IDP		19
	{ "_nspcb"},
#define N_IDPSTAT	20
	{ "_idpstat"},
#define N_SPPSTAT	21
	{ "_spp_istat"},
#define N_NSERR		22
	{ "_ns_errstat"},
#define N_NIMP		23
	{ "_nimp"},
#if	CMU
#define	N_ARPTAB	24
	{ "_arptab" },
#define	N_ARPTAB_BSIZ	25
	{ "_arptab_bsiz" },
#define	N_ARPTAB_NB	26
	{ "_arptab_nb" },
#endif	/* CMU */
	"",
};

/* internet protocols */
extern	int protopr();
extern	int tcp_stats(), udp_stats(), ip_stats(), icmp_stats();
/* ns protocols */
extern	int nsprotopr();
extern	int spp_stats(), idp_stats(), nserr_stats();

#define NULLPROTOX	((struct protox *) 0)
struct protox {
	u_char	pr_index;		/* index into nlist of cb head */
	u_char	pr_sindex;		/* index into nlist of stat block */
	u_char	pr_wanted;		/* 1 if wanted, 0 otherwise */
	int	(*pr_cblocks)();	/* control blocks printing routine */
	int	(*pr_stats)();		/* statistics printing routine */
	char	*pr_name;		/* well-known name */
} protox[] = {
	{ N_TCB,	N_TCPSTAT,	1,	protopr,
	  tcp_stats,	"tcp" },
	{ N_UDB,	N_UDPSTAT,	1,	protopr,
	  udp_stats,	"udp" },
	{ -1,		N_IPSTAT,	1,	0,
	  ip_stats,	"ip" },
	{ -1,		N_ICMPSTAT,	1,	0,
	  icmp_stats,	"icmp" },
	{ -1,		-1,		0,	0,
	  0,		0 }
};

struct protox nsprotox[] = {
	{ N_IDP,	N_IDPSTAT,	1,	nsprotopr,
	  idp_stats,	"idp" },
	{ N_IDP,	N_SPPSTAT,	1,	nsprotopr,
	  spp_stats,	"spp" },
	{ -1,		N_NSERR,	1,	0,
	  nserr_stats,	"ns_err" },
	{ -1,		-1,		0,	0,
	  0,		0 }
};

char	*system = "/vmunix";
char	*kmemf = "/dev/kmem";
int	kmem;
int	kflag;
int	Aflag;
int	aflag;
#if	CMU
int	Hflag;
#endif
int	hflag;
int	iflag;
int	mflag;
int	nflag;
int	pflag;
int	rflag;
int	sflag;
int	tflag;
int	dflag;
int	interval;
char	*interface;
int	unit;

int	af = AF_UNSPEC;

extern	char *malloc();
extern	off_t lseek();

main(argc, argv)
	int argc;
	char *argv[];
{
	extern char *optarg;
	extern int optind;
	register struct protoent *p;
	register struct protox *tp;	/* for printing cblocks & stats */
	struct protox *name2protox();	/* for -p */
	int ch;

	while ((ch = getopt(argc, argv, "AI:af:Hhimnp:drstu")) != EOF)
		switch((char)ch) {
		case 'A':
			Aflag++;
			break;
		case 'I': {
			char *cp;

			iflag++;
			for (cp = interface = optarg; isalpha(*cp); cp++);
			unit = atoi(cp);
#if	CMU
			{
				int l = (cp-interface);
				char *ip = (char *)malloc(l+1);

				strncpy(ip, interface, l);
				cp = &ip[l];
				interface = ip;
			}
#endif
			*cp = '\0';
			break;
		}
		case 'a':
			aflag++;
			break;
		case 'd':
			dflag++;
			break;
		case 'f':
			if (strcmp(optarg, "ns") == 0)
				af = AF_NS;
			else if (strcmp(optarg, "inet") == 0)
				af = AF_INET;
			else if (strcmp(optarg, "unix") == 0)
				af = AF_UNIX;
			else {
				fprintf(stderr, "%s: unknown address family\n", optarg);
				exit(10);
			}
			break;
#if	CMU
		case 'H':
			Hflag++;
			break;

#endif
		case 'h':
			hflag++;
			break;
		case 'i':
			iflag++;
			break;
		case 'm':
			mflag++;
			break;
		case 'n':
			nflag++;
			break;
		case 'p':
			if ((tp = name2protox(optarg)) == NULLPROTOX) {
				fprintf(stderr, "%s: unknown or uninstrumented protocol\n", optarg);
				exit(10);
			}
			pflag++;
			break;
		case 'r':
			rflag++;
			break;
		case 's':
			sflag++;
			break;
		case 't':
			tflag++;
			break;
		case 'u':
			af = AF_UNIX;
			break;
		case '?':
		default:
			usage();
		}
	argv += optind;
	argc -= optind;

	if (argc > 0) {
		if (isdigit(argv[0][0])) {
			interval = atoi(argv[0]);
			if (interval <= 0)
				usage();
			argv++, argc--;
			iflag++;
		}
		if (argc > 0) {
			system = *argv;
			argv++, argc--;
			if (argc > 0) {
				kmemf = *argv;
				kflag++;
			}
		}
	}
#if	CMU
	if (vmnlist(system, nl))
#endif
	if (nlist(system, nl) < 0 || nl[0].n_type == 0) {
		fprintf(stderr, "%s: no namelist\n", system);
		exit(1);
	}
	kmem = open(kmemf, O_RDONLY);
	if (kmem < 0) {
		perror(kmemf);
		exit(1);
	}
#ifdef	mac2
	read(kmem, &kload, sizeof (kload));
	offset_syms();
#endif
	if (kflag) 
		init_sysmap();
	if (mflag) {
		mbpr((off_t)nl[N_MBSTAT].n_value);
		exit(0);
	}
	if (pflag) {
		if (tp->pr_stats)
			(*tp->pr_stats)(nl[tp->pr_sindex].n_value,
				tp->pr_name);
		else
			printf("%s: no stats routine\n", tp->pr_name);
		exit(0);
	}
	if (hflag) {
		hostpr(nl[N_IMP].n_value, nl[N_NIMP].n_value);
		exit(0);
	}
	/*
	 * Keep file descriptors open to avoid overhead
	 * of open/close on each call to get* routines.
	 */
	sethostent(1);
	setnetent(1);
	if (iflag) {
		intpr(interval, nl[N_IFNET].n_value);
		exit(0);
	}
#if	CMU
	if (Hflag) {
		arppr(nl[N_ARPTAB].n_value, nl[N_ARPTAB_BSIZ].n_value,
			nl[N_ARPTAB_NB].n_value);
		exit(0);
	}
#endif
	if (rflag) {
		if (sflag)
			rt_stats((off_t)nl[N_RTSTAT].n_value);
		else
			routepr((off_t)nl[N_RTHOST].n_value, 
				(off_t)nl[N_RTNET].n_value,
				(off_t)nl[N_RTHASHSIZE].n_value);
		exit(0);
	}
    if (af == AF_INET || af == AF_UNSPEC) {
	setprotoent(1);
	setservent(1);
	while (p = getprotoent()) {

		for (tp = protox; tp->pr_name; tp++)
			if (strcmp(tp->pr_name, p->p_name) == 0)
				break;
		if (tp->pr_name == 0 || tp->pr_wanted == 0)
			continue;
		if (sflag) {
			if (tp->pr_stats)
				(*tp->pr_stats)(nl[tp->pr_sindex].n_value,
					p->p_name);
		} else
			if (tp->pr_cblocks)
				(*tp->pr_cblocks)(nl[tp->pr_index].n_value,
					p->p_name);
	}
	endprotoent();
    }
    if (af == AF_NS || af == AF_UNSPEC) {
	for (tp = nsprotox; tp->pr_name; tp++) {
		if (sflag) {
			if (tp->pr_stats)
				(*tp->pr_stats)(nl[tp->pr_sindex].n_value,
					tp->pr_name);
		} else
			if (tp->pr_cblocks)
				(*tp->pr_cblocks)(nl[tp->pr_index].n_value,
					tp->pr_name);
	}
    }
    if ((af == AF_UNIX || af == AF_UNSPEC) && !sflag)
	    unixpr((off_t)nl[N_NFILE].n_value, (off_t)nl[N_FILE].n_value,
		(struct protosw *)nl[N_UNIXSW].n_value);
    if (af == AF_UNSPEC && sflag)
	impstats(nl[N_IMP].n_value, nl[N_NIMP].n_value);
    exit(0);
}

#ifdef mac2
offset_syms()
{
    register struct nlist	*n;

    for (n = nl; n->n_name != 0 && *(n->n_name) != 0; n++)
	if (n->n_type != N_UNDF)
	    n->n_value += kload;
}
#endif

/*
 * Seek into the kernel for a value.
 */
off_t
klseek(fd, base, off)
	int fd, off;
	off_t base;
{
	if (kflag) {
		base = vtop(base);
		if (base == -1)
			panic("bad vtop");
	}
	return (lseek(fd, base, off));
}

char *
plural(n)
	int n;
{

	return (n != 1 ? "s" : "");
}

#if	defined(vax)
#include <machine/pte.h>

struct	pte *Sysmap;

vtop(base)
int base;
{
	/* get kernel pte */
	base &= ~KERNBASE;
	return(ctob(Sysmap[btop(base)].pg_pfnum) + (base & PGOFSET));
}

init_sysmap()
{
	off_t off;

	Sysmap = (struct pte *)
	   malloc((u_int)(nl[N_SYSSIZE].n_value * sizeof(struct pte)));
	if (!Sysmap) {
		fputs("netstat: can't get memory for Sysmap.\n", stderr);
		exit(1);
	}
	off = nl[N_SYSMAP].n_value & ~KERNBASE;
	(void)lseek(kmem, off, L_SET);
	(void)read(kmem, (char *)Sysmap,
	    (int)(nl[N_SYSSIZE].n_value * sizeof(struct pte)));
}
#elif	defined(ibmrt)
#include <machine/mmu.h>

#define SEG_MASK	0xf0000000	/* mask for segment number */
#define	MMU_SID_SYSTEM	RTA_SID_SYSTEM
#define	MMU_VPAGE_BITS	RTA_VPAGE_BITS
#define	MMU_HASH	RTA_HASH
#define	MMU_ENDCHAIN	RTA_ENDCHAIN
#define	RTA_HASHMASK	MMU_HASHMASK
struct hatipt_entry *MMU_HATIPT;
int MMU_HASHMASK;

/*
 * read in hat/ipt so that we can do virtual to real translations
 */
init_sysmap()
{

	register int ptr = wfetch(nl[N_HATIPT].n_value & ~SEG_MASK);
	register int hash = wfetch(nl[N_HASHMASK].n_value & ~SEG_MASK);
	register int size = (hash+1) * sizeof (struct hatipt_entry);

	MMU_HASHMASK = hash;
	if ((MMU_HATIPT = (struct hatipt_entry *) calloc(size, 1)) == 0)
		panic("Sysmap");
	readinfo((char *) MMU_HATIPT, ptr & ~SEG_MASK, size, "hatipt");
}


/*
 * do virtual to real translation using the hat/ipt and the 
 * contents of the segment registers
 */
vtop(where)
	register unsigned int where;
{
	register struct hatipt_entry *ipte;
	register unsigned int rpage, addrtag, vpage, sid;
	register int probes;
	unsigned int page_off;
#define MAX_PROBES 100
	sid = MMU_SID_SYSTEM;
	vpage = btop(where & 0x0FFFFFFF);
	page_off = where & PGOFSET;
	ipte = &MMU_HATIPT[MMU_HASH(sid, vpage)];
	/* make sure there is an ipt chain */
	if (MMU_ENDCHAIN(ipte->hat_ptr)) { /* hat pointer empty...not found */
		printf("bad virtual address\n");
		return (-1);
	}
	/* chase down the hat ptr */
	ipte = &MMU_HATIPT[get_hatptr(ipte) & MMU_HASHMASK];
	addrtag = (sid << MMU_VPAGE_BITS) | vpage;
	/* check addrtag of first element of ipt chain */
	if ((ipte->key_addrtag & 0x1fffffff) == addrtag) {
		/* same, return vpage + page offset from where 
		*/
		return (ctob(ipte - MMU_HATIPT) + page_off);
	}
	/* for second element ff. */
	probes = 0;
	while (!MMU_ENDCHAIN(get_iptptr(ipte))) { /* while there is a next element */
		/* point to the next element */
		ipte = &MMU_HATIPT[get_iptptr(ipte) & MMU_HASHMASK];
		/* check addrtag for this element against desired one */
		if ((ipte->key_addrtag & 0x1fffffff) == addrtag) {
			/* same, return vpage + page offset from where 
			*/
			return (ctob(ipte - MMU_HATIPT) + page_off);
		}
		if (probes++ >= MAX_PROBES) {
			return (-1);
		}
	}
	printf("bad virtual address\n");
	return (-1);
}

wfetch(addr)
{
	int value;

	readinfo((char *) &value, addr, sizeof value, "?");
	return(value);
}

readinfo(value,addr,length,name)
char *value;
char *name;
{
	if (read(kmem, value, length) != length)
		panic("read");
}
#else
vtop(base)
int base;
{
	fprintf(stderr, "netstat: cannot do address translation\n");
	exit(1);
	/*NOTREACHED*/
}

init_sysmap()
{
	fprintf(stderr, "netstat: cannot do address translation\n");
	exit(1);
}
#endif

panic(str)
char *str;
{
	perror(str);
	exit(1);
}

/*
 * Find the protox for the given "well-known" name.
 */
struct protox *
knownname(name)
	char *name;
{
	struct protox *tp;

	for (tp = protox; tp->pr_name; tp++)
		if (strcmp(tp->pr_name, name) == 0)
			return(tp);
	for (tp = nsprotox; tp->pr_name; tp++)
		if (strcmp(tp->pr_name, name) == 0)
			return(tp);
	return(NULLPROTOX);
}

/*
 * Find the protox corresponding to name.
 */
struct protox *
name2protox(name)
	char *name;
{
	struct protox *tp;
	char **alias;			/* alias from p->aliases */
	struct protoent *p;

	/*
	 * Try to find the name in the list of "well-known" names. If that
	 * fails, check if name is an alias for an Internet protocol.
	 */
	if (tp = knownname(name))
		return(tp);

	setprotoent(1);			/* make protocol lookup cheaper */
	while (p = getprotoent()) {
		/* assert: name not same as p->name */
		for (alias = p->p_aliases; *alias; alias++)
			if (strcmp(name, *alias) == 0) {
				endprotoent();
				return(knownname(p->p_name));
			}
	}
	endprotoent();
	return(NULLPROTOX);
}

usage()
{
	fputs("usage: netstat [-Aan] [-f address_family] [system] [core]\n               [-himnrs] [-f address_family] [system] [core]\n               [-n] [-I interface] interval [system] [core]\n", stderr);
	exit(1);
}
