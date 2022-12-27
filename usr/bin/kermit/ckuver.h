/* ckuver.h -- C-Kermit UNIX Version heralds */

#ifndef CKUVER_H
#define CKUVER_H

/* Arranged alphabetically by compiler symbol */
/* Must be included AFTER ckcdeb.h. */

#ifdef aegis
#ifdef BSD4
#define HERALD " Apollo DOMAIN/IX 4.2 BSD"
#else
#ifdef ATTSV
#define HERALD " Apollo DOMAIN/IX System V"
#else
#define HERALD " Apollo Aegis"
#endif /* BSD4  */
#endif /* ATTSV */
#endif /* aegis */

#ifdef AIXRS
#define HERALD " IBM RS/6000 (AIX 3.x)"
#endif /* AIXRS */

#ifdef AIXPS2
#define HERALD " IBM PS/2 (AIX 3.x)"
#endif /* AIXPS2 */

#ifdef AIX370
#ifndef HERALD
#define HERALD " IBM System/370 (AIX 3.x)"
#endif
#endif /* AIX370 */

#ifdef ATT6300
#define HERALD " AT&T 6300"
#endif /* ATT6300 */

#ifdef ATT7300
#define HERALD " AT&T 7300/UNIX PC"
#endif /* ATT7300 */

#ifdef AUX
#define HERALD " Apple Macintosh AUX"
#endif /* AUX */

#ifdef ENCORE
#ifdef BSD43
#define HERALD " Encore Multimax UMAX 4.3"
#else
#define HERALD " Encore Multimax UMAX 4.2"
#endif
#endif /* ENCORE */

#ifdef BSD29
#define HERALD " 2.9 BSD"
#endif /* BSD29 */

#ifdef BSD41
#define HERALD " 4.1 BSD"
#endif /* BSD41 */

#ifdef C70
#define HERALD " BBN C/70"
#endif /* c70 */

#ifdef CIE
#define HERALD " CIE Systems 680/20 Regulus"
#endif /* CIE */

#ifdef COHERENT
#define HERALD " PC/AT MWC Coherent 3.0"
#endif /* COHERENT */

#ifdef FT18
#ifdef FT21
#define HERALD " Fortune For:Pro 2.1"
#else
#define HERALD " Fortune For:Pro 1.8"
#endif /* FT21 */
#endif /* FT18 */

#ifdef ISIII
#define HERALD " Interactive Systems Corp System III"
#endif /* ISIII */

#ifdef IX370
#define HERALD " IBM IX/370"
#endif /* IX370 */

#ifdef HPUX
#define HERALD " HP 9000 Series HP-UX"
#endif /* HPUX */

#ifdef MINIX
#define HERALD " Minix"
#endif /* MINIX */

#ifdef MIPS
#define HERALD " MIPS RISC/OS (System V R3)"
#endif

#ifdef NEXT
#define HERALD " NeXT"
#endif

#ifdef PCIX
#define HERALD " PC/IX"
#endif /* PCIX */

#ifdef sxaE50
#define HERALD " PFU SX/A V10/L50"
#endif /* sxaE50 */

#ifdef PROVX1
#define HERALD " DEC Professional 300 (Venix 1.0)"
#endif /* PROVX1 */

#ifdef RTAIX
#define HERALD " IBM RT PC (AIX 2.2)"
#endif /* RTAIX */

#ifdef RTU
#define HERALD " Masscomp/Concurrent RTU"
#endif /* RTU */

#ifdef sony_news
#define HERALD " SONY NEWS"
#endif /* sony_news */

#ifdef SUNOS4
#ifdef BSD4
#ifdef SUNOS41
#define HERALD " SUNOS 4.1 (BSD)"
#else
#define HERALD " SUNOS 4.0 (BSD)"
#endif /* SUNOS41 */
#endif /* BSD4 */
#endif /* SUNOS4 */

#ifdef SUN4S5
#ifdef HDBUUCP
#define HERALD " SUNOS 4.1 (SVR3)"
#else
#define HERALD " SUNOS 4.0 (SVR3)"
#endif /* HDBUUCP */
#endif /* SUN4S5 */

#ifdef TOWER1
#define HERALD " NCR Tower 1632 (OS 1.02)"
#endif /* TOWER1 */

#ifdef TRS16
#define HERALD " Tandy 16/6000 (Xenix 3.0)"
#endif /* TRS16 */

#ifdef u3b2
#ifndef HERALD
#ifdef SVR3
#define HERALD " AT&T 3B2 (System V R3)"
#else
#define HERALD " AT&T 3B2 (System V)"
#endif /* SVR3 */
#endif /* HERALD */
#endif /* u3b2 */

#ifdef ultrix
#ifdef vax
#define HERALD " VAX/Ultrix"
#else
#ifdef mips
#define HERALD " DECstation/Ultrix"
#else
#define HERALD " Ultrix"
#endif /* mips */
#endif /* vax */
#endif /* ultrix */

#ifdef OXOS
#define HERALD " Olivetti X/OS"
#endif /* OXOS */

#ifdef POSIX
#ifdef HERALD
#undef HERALD
#endif /* HERALD */
#define HERALD " POSIX"
#endif /* POSIX */

#ifdef UTS24
#define HERALD " Amdahl UTS 2.4"
#endif /* UTS24 */

#ifdef UTSV
#define HERALD " Amdahl UTS V"
#endif /* UTSV */

#ifdef VXVE
#define HERALD " CDC VX/VE 5.2.1 System V"
#endif /* VXVE */

#ifdef XENIX
#ifdef M_UNIX 
#define HERALD " SCO UNIX/386"
#else
#ifdef M_I386
#define HERALD " Xenix/386"
#else
#ifdef M_I286
#define HERALD " Xenix/286"
#else
#define HERALD " Xenix"
#endif /* M_I286 */
#endif /* M_I386 */
#endif /* M_UNIX */
#endif /* XENIX  */

#ifdef ZILOG
#define HERALD " Zilog S8000 Zeus 3.21+"
#endif /* ZILOG */

#ifdef UTEK
#define HERALD " UTek"
#endif /* UTEK */

/* Catch-alls for anything not defined explicitly above */

#ifndef HERALD
#ifdef SVR4
#define HERALD " AT&T System V R4"
#else
#ifdef SVR3
#define HERALD " AT&T System V R3"
#else
#ifdef ATTSV
#define HERALD " AT&T System III / System V"
#else
#ifdef BSD43
#define HERALD " 4.3 BSD"
#else
#ifdef BSD4
#define HERALD " 4.2 BSD"
#else
#ifdef V7
#define HERALD " UNIX Version 7"
#endif /* V7 */
#endif /* BSD4 */
#endif /* BSD43 */
#endif /* ATTSV */
#endif /* SVR3 */
#endif /* SVR4 */
#endif /* HERALD */

#ifndef HERALD
#define HERALD " Unknown Version"
#endif /* HERALD */

#endif /* CKUVER_H */

/* End of ckuver.h */
