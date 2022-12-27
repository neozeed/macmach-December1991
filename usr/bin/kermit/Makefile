# Makefile for kermit
#
# CKUKER.MAK, Sun Nov  3 23:54:51 1991
#
CKVER= "5A(174) ALPHA"
#
# -- Makefile to build C-Kermit for UNIX and UNIX-like systems --
#
# Author: Frank da Cruz, Columbia University Center for Computing Activities
# 612 West 115th Street, New York, NY 10025, USA.  Phone (212) 854-5126.
# e-mail: fdc@watsun.cc.columbia.edu, fdc@columbia.edu, or FDCCU@CUVMA.BITNET.
#
#   C-Kermit can also be built for many other systems not supported by this
#   makefile, including the Apple Macintosh, the Commodore Amiga, OS-9, 
#   IBM OS/2, DEC VAX/VMS.  See the ckaaaa.hlp file for information.  Support 
#   was also added to C-Kermit for some other systems like Data General AOS/VS
#   and Apollo Aegis, but the system-dependent modules for these versions need
#   a lot of work to bring them up to 5A level.
#
# WARNING: This is a pre-Beta test release.  Use at your own risk.
#
# Many of the implementations listed below are untested for v5A:
# + Those that have been tested successfully are marked with "+".
# - Those that are known not to work are marked with "-".
# ? Those as yet untested marked with "?".
#
# Those marked with + are not guaranteed to work.  The "+" means that C-Kermit
# 5A was built successfully and tested to some degree without showing obvious
# problems, but not necessarily in the current edit.  That is, something that
# has been done to the program in recent edits might have broken a previously
# working version.
#
# Please report successes or failures (preferably with fixes) to the author.
#
# INSTALLATION NOTES:
#
# Before proceeding, read the instructions below, and also read the files
# ckuins.doc (detailed installation instructions) and ckuker.bwr (the "beware
# file"), and then rename this file to "makefile" or "Makefile" if necessary,
# and then give the appropriate "make" command.
#
# MAKE COMMANDS FOR DIFFERENT UNIX VERSIONS:
#
# Those marked with ? are untested in version 5A.  Please report results
# to the author.  Those marked with + are tested and believed to work.  Those
# marked with - believed not to work.  Those marked with x can be built and 
# run but some features (typically DIAL or HANGUP) might not work.
#
# + for Alliant FX/8 with Concentrix 4.1 or later, "make bsdlck"
# + for Altos 486, 586, 986 with Xenix 3.0, "make altos"
# ? for Amdahl UTS 2.4 on IBM 370 series & compatible mainframes, "make uts24"
# + for Amdahl UTSV IBM 370 series & compatible mainframes, "make utsv"
# + for Amdahl mainframes with with UNIX System V R 5.2.6b 580, "make sys3"
# ? for Apollo DOMAIN/IX, "make bsd" or "make sys3", for desired environment
# ? for Apollo DOMAIN/IX, if the above fails, try "make apollobsd"
# + for Apollo with SR10.0 or later, BSD environment, "make sr10-bsd"
# + for Apollo with SR10.0 or later, System V environment, "make sr10-s5r3"
# ? for Apollo with straight Aegis using native Aegis i/o,
#     give "cc" commands for each module, then "bind" to link them together.
# + for Apple Macintosh II with A/UX, "make aux" or "make auxufs"
# ? for Apple Macintosh with Minix 1.5.10, "make minix68k" or "make minixc68"
# + for AT&T 6300 PLUS, "make att6300" or (with no debugging) "make att6300nd"
# + for AT&T 6386 WGS Unix PC, "make sys5r3"
# + for AT&T 3B2, 3B20 systems, "make att3bx"
# + for AT&T 3B1, 7300 Unix PC, "make sys3upc", "sys3upcg" (gcc), "sys3upcold"
# + for AT&T System III/System V R2 or earlier, "make sys3" or "make sys3nid"
# + for AT&T System III/System V with Honey DanBer UUCP, "make sys3hdb"
# + for AT&T System V on DEC VAX, "make sys3" or "make sys5r3"
# + for AT&T System V R3, use "make sys5r3".  This is different from the above.
# + for AT&T System V R4, "make sys5r4", "make sys5r4sx", or "make sys5r4nx",
#     or if the ANSI C function prototyping makes trouble, add -DNOANSI,
#     as in "sys5r4sxna" entry (used with AT&T System V/386 R 4.0 V 2.1.
# + for AT&T System V R4 with Wollongong TCP/IP, "make sys5r4net", ...
# ? for Atari ST with Minix ST 1.5.10.3, "make minix68k" or "make minixc68"
# ? for BBN C/70 with IOS 2.0, "make c70"
# + for Bell Unix Version 7 (aka 7th Edition), "make v7" (but see below)
# + for Berkeley Unix 2.4, "make v7" (but see below)
# ? for Berkeley Unix 2.9 (DEC PDP-11 or Pro-3xx), "make bsd29"
# ? for Berkeley Unix 2.10, "make bsd210" (requires overlays)
# ? for Berkeley Unix 2.11, same as 2.10 BSD
# + for Berkeley Unix 4.1, "make bsd41"
# + for Berkeley Unix 4.2, "make bsd" (tested with 4.2 and 4.3)
# + for Berkeley Unix 4.2 or 4.3 with HoneyDanBer UUCP, "make bsdhdb"
# + for Berkeley Unix 4.3, "make bsd43" (uses acucntrl program)
# + for Berkeley Unix 4.3 without acucntrl program, "make bsdlck" or "make bsd"
# + for Berkeley Unix 4.3-Tahoe, same as 4.3 BSD.
# ? for Berkeley Unix 4.3-Reno, same as 4.3 BSD.
# ? for Berkeley Unix 4.4, maybe "make posix"?
# ? for Cadmus, "make sys3"
# ? for Callan, "make sys3"
# ? for CDC VX/VE 5.2.1 Sys V emulation, "make vxve"
# ? for Charles River Data Systems 680x0 systems with Unos, "make sys3nid"
# ? for CIE Systems 680/20 with Regulus, "make cie"
# + for Commodore Amiga 3000UX Sys V R4, "make sys5r4sx"
# + for Commodore Amiga 3000UX Sys V R4 and TCP/IP, "make svr4amiganet"
# ? for Commodore Amiga with Minix 1.5.10, "make minix68k" of "make minixc68"
# + for Convergent with CTIX Sys V R2, "make sys5"
# + for Convex C1, "make convex"
# + for Convex C210 with Convex/OS 8, "make convex8"
# + for Convex C2 with Convex/OS 9.1, "make convex9"
# + for Cray X/MP or Y/MP with Unicos System V R3, "make cray"
# + for Cyber 910 (Silicon-Graphics Iris) with Irix 3.3, "irix33"
# + for Data General Aviion, "make sys5r3" (maybe compile ckwart separately)
# ? for Data General MVxxxxx with DG/UX, ???
# + for DEC VAX (or DECstation?) with Ultrix 1.x "make bsd"
# + for DEC VAX (or DECstation?) with Ultrix 2.x "make du2"
# + for DEC VAX (or DECstation?) with Ultrix 3.0, 3.1, "make du3"
# + for DECstation (or VAX) with Ultrix 4.0 or 4.1 or 4.2, "make du4"
# + for DECstation (or VAX) with Ultrix 4.x and Kanji support, "make du4kanji"
# ? for DEC Pro-350 with Pro/Venix V1.x, "make provx1"
# ? for DEC Pro-350 with Pro/Venix V2.0 (Sys V), "make sys3nid" 
# ? for DEC Pro-380 with Pro/Venix V2.0 (Sys V), "make sys3" or "make sys3nid"
# ? for DEC Pro-380 with 2.9, 2.10, or 2.11 BSD, "make bsd29" or "make bsd210"
# + for DIAB DS90 with DNIX (any version) create an empty <sys/file.h>
#     if this file does not already exist (or add -DNOFILEH to the make entry).
# + for DIAB DS90 or LUXOR ABC-9000 with pre-5.2 DNIX, add "getcwd" to libc.a
#     (search below for "getcwd"), then "make dnixold".
# + for DIAB DS90 with DNIX 5.2 (Sys V.2) or earlier, "make dnix" or "dnixnd".
# + for DIAB DS90 with DNIX 5.3 (Sys V.3), "make dnix5r3"
# + for Encore Multimax 310, 510 with UMAX 4.2, "make umax42"
# + for Encore Multimax 310, 510 with UMAX 4.3, "make umax43"
# + for Encore Multimax 310, 510 with UMAX V 2.2, use Berkeley cc, "make bsd"
# + for Encore 88K with UMAX V 5.2, "make encore88k"
# + for ESIX System V R4.0.3 with TCP/IP support, "make esixr4"
# ? for Fortune 32:16, For:Pro 1.8, "make ft18"
# + for Fortune 32:16, For:Pro 2.1, "make ft21"
# + for FPS 500 with FPX 4.1, "made bsd"
# ? for Heurikon, "make sys3"
# + for HP-9000 Series, HP-UX < 6.5, without long filenames, no job control,
#   "make hpuxpre65"
# + for HP-9000 Series, HP-UX pre-7.0, without long filenames, "make hpux"
# ? for HP-9000 Series, HP-UX 7.0 or later, no long filenames, "make hpux7sf"
# + for HP-9000 Series with HP-UX Sys V R2, BSD long names, "make hpuxlf"
# + for HP-9000 Series with HP-UX Sys V R2, dirent long names, "make hpuxde"
# + for HP-9000 Series with HP-UX Sys V R3, "make hpuxs5r3"
# ? for IBM 370 Series with IX/370, "make ix370"
# + for IBM 370 Series with AIX/370 3.0, "make aix370"
# + for IBM PC/AT & compatibles with Mark Williams Coherent OS, "make coherent"
# ? for IBM PC/AT & compatibles with original MINIX, "make minix".
# + for IBM PC/AT & compatibles with MINIX, new compiler, "make minixnew"
# + for IBM PC family, 386-based, with MINIX/386, "make minix386"
# + for IBM PS/2 with PS/2 AIX 1.0, 1.1, or 1.2, "make ps2aix"
# + for IBM PS/2 with PS/2 AIX 3.0, "make ps2aix3"
# + for IBM RISC System/6000 with AIX 3.0, "make rs6000"
# + for IBM RISC System/6000 with AIX 3.1, "make rs6000"
# ? for IBM RT PC with AIX 2.1, "make sys3"
# + for IBM RT PC with AIX 2.2.1, "make rtaix" (special lockfile handling)
# ? for IBM RT PC with ACIS 4.2, "make bsd"
# + for IBM RT PC with ACIS 4.3, "make rtacis"
# + for Intel 302 with Bell Tech Sys V/386 R3.2, "make sys5r3"
# ? for Intel Xenix/286, "make sco286"
# ? for Interactive System III (PC/IX) on PC/XT, "make pcix"
# ? for Interactive System III on other systems, "make is3"
# + for Interactive 386/ix 2.x, "make is5r3"
# + for Interactive 386/ix 2.x with Ethernet, "make is5r3net" or "is5r3netg"
# + for Luxor ABC-9000 (DIAB DS-90) with pre-5.2 DNIX, add "getcwd" to libc.a
#     (search below for "getcwd"), then "make dnixold".
# ? for Masscomp RTU AT&T System III, "make rtu"
# + for Masscomp/Concurrent with RTU 4.0 or later, BSD environment, "make 
#     rtubsd", "make rtubsd2", "make rtubsd3" (depending on where ndir.h 
#     is stored, see entries below).
# ? for Masscomp/Concurrent with RTU 4.0 or later, System V environment,
#     "make rtus5" (System V R2) or "make rtus5r3" (System V R3 or later).
# + for Microport SV/AT (System V R2), "make mpsysv"
# + for Microport SVR4 2.2, "make sys5r4"
# ? for Microsoft,IBM Xenix (/286, PC/AT, etc), "make xenix" or "make sco286"
# + for MIPS machine with AT&T System V R3.0, "make mips"
# + for Modcomp 9730, Real/IX, "make sys5r3" (or modify to use gcc = GLS cc)
# ? for Motorola Four Phase, "make sys3" or "make sys3nid"
# ? for NCR Tower 1632, OS 1.02, "make tower1"
# + for NCR Tower 1632 with System V, "make sys3"
# + for NCR Tower 32, OS Release 1.xx.xx, "make tower32-1"
# + for NCR Tower 32, OS Release 2.xx.xx, "make tower32-2"
# + for NCR Tower 32, OS Releases based on Sys V R3, "make tower32"
# + for NCR Tower 32, OS Releases based on Sys V R3 with gcc "make tower32g"
# + for NeXT, "make next"
# + for Olivetti X/OS R.2.3, "make xos23"
# + for PFU Compact A Series SX/A TISP V10/E50 (Japan), "make sxae50"
# ? for Plexus, "make sys3"
# + for Pyramid, "make bsd"
# + for POSIX on anything, "make posix" (but adjustments might be necessary).
# + for POSIX on SUNOS 4.1 or later, "make sunposix"
# + for POSIX on SUNOS 4.1 or later with GNU cc, "make gccposix"
# ? for Ridge 32 (ROS3.2), "make ridge32"
# ? for Samsung MagicStation, "make sys5r4"
# ? for SCO Xenix 2.2.1 with development system 2.2 on 8086/8 "make sco86"
# + for SCO Xenix/286 2.2.1 with development system 2.2 on 80286, "make sco286"
#     NOTE: reportedly this makefile is too long for SCO Xenix/286 make, but it
#     works with "makeL", or if some of the other make entries are edited out.
# + for SCO Xenix/386 2.2.2, "make sco386"
# + for SCO Xenix/386 2.3.x, "make sco3r2" or "make sco3r2x"
# + for SCO UNIX/386 3.2.0 or 3.2.1, "make sco3r2" or "make sco3r2x"
# + for SCO UNIX/386 3.2.2, "make sco3r22"
# + for SCO UNIX/386 3.2.2 with SCO TCP/IP, "make sco3r22net"
# + for SCO Xenix/386 or UNIX/386 with Excelan TCP/IP, "make sco3r2net"
# + for SCO Xenix 2.3.x with Racal InterLan TCP/IP, "make sco3r2netri"
# + for other Unix varieties with Racal Interlan TCP/IP, read sco3r2netri entry
# + for Sequent Balance 8000 or B8 with DYNIX 3.0.xx, "make bsdlck"
# + for Sequent Symmetry S81 with DYNIX 3.0.xx, "make bsdlck"
# + for Sequent DYNIX 3.0.xx, if the above two don't work, "make dynix3"
# + for Sequent with DYNIX/PTX 1.2.1, "make ptx"
# + for Silicon Graphics Iris System V Irix 3.2 or earlier, "make iris"
# + for Silicon Graphics Sys V R3 with Irix 3.3 or later, "make sys5r3"
# + for Solbourne 4/500 with OS/MP 4 "make sunos4"
# + for Solbourne 4/500 with OS/MP 4.1 "make sunos41"
# + for SONY NEWS with NEWS-OS 4.01C, "make sonynews"
# + for Sperry (Unisys) 5000-80, "make sys5r3"
# + for SUN with pre-4.0 SUNOS versions, "make bsd" (or appropriate variant)
# + for SUN with SUNOS 4.0, BSD environment, "make sunos4"
# + for SUN with SUNOS 4.0, BSD, with SunLink X.25, "make sunos4x25"
# + for SUN with SUNOS 4.0, AT&T Sys V R3 environment, "make sunos4s5"
# + for SUN with SUNOS 4.1 or 4.1.1, BSD environment, "make sunos41" 
# + for SUN with SUNOS 4.1, BSD, with SunLink X.25, "make sunos41x25"
# + for SUN with SUNOS 4.1, 4.1.1, AT&T Sys V R3 environment, "make sunos41s5" 
# + for SUN with SUNOS 4.1 or later, POSIX environment, "make sunposix"
# + for Tandy 16/6000 with Xenix 3.0, "make trs16"
# + for Tektronix 6130/4132/43xx (e.g.4301) with UTek OS, "make utek"
# + for Tri Star Flash Cache with Esix SVR3.2, "make sys5r3"
# ? for Unistar, "make sys3"
# ? for Valid Scaldstar, "make valid"
# ? for Whitechapel MG01 Genix 1.3, "make white"
# ? for Zilog ZEUS 3.21, "make zilog"
#
# The result should be a runnable program called "wermit" in the current 
# directory.  After satisfactory testing, you can rename wermit to "kermit" 
# and put it where users can find it.
#
# To remove intermediate and object files, "make clean".
# To run lint on the source files, "make lintsun", "make lintbsd",
# "make lints5", as appropriate.
#
##############################################################################
# Adding getcwd() function to your C library:
#
# Copy the following code into a file getcwd.c, compile using the -o option,
# and use ar to put it in your copy of /lib/libc.a.  You might also have to
# make sure that function zxcmd() in ckufio.c includes a declaration like
# the one below for popen() (if your stdio.h doesn't include one):
#
# #include <stdio.h>
# char *
# getcwd(buf,size) char *buf; int size; {
#     FILE *pfp, *popen();
#     if (!(pfp = popen("pwd"","r"))) return(".");
#     fgets(buf,size,pfp);
#     pclose(pfp);
#     buf[strlen(buf)-1] = '\0';
#     return(buf);
# }
#
##############################################################################
#
# NOTES FOR V7 AND 2.X BSD (BASED ON VERSION 4E OF C-KERMIT):
#
# For Unix Version 7, several variables must be defined to the values
# associated with your system.  BOOTNAME=/edition7 is the kernel image on
# okstate's Perkin-Elmer 3230.  Others will probably be /unix.  PROCNAME=proc
# is the name of the structure assigned to each process on okstate's system.
# This may be "_proc" or some other variation.  See <sys/proc.h> for more
# info on your systems name conventions.  NPROCNAME=nproc is the name of a
# kernel variable that tells how many "proc" structures there are.  Again
# this may be different on your system, but nproc will probably be somewhere.
# The variable NPTYPE is the type of the nproc variable -- int, short, etc.
# which can probably be gleaned from <sys/param.h>.  The definition of DIRECT
# is a little more complicated.  If nlist() returns, for "proc" only, the
# address of the array, then you should define DIRECT as it is below.  If
# however, nlist() returns the address of a pointer to the array, then you
# should give DIRECT a null definition (DIRECT= ).  The extern declaration in
# <sys/proc.h> should clarify this for you.  If it is "extern struct proc
# *proc", then you should NOT define DIRECT.  If it is "extern struct proc
# proc[]", then you should probably define DIRECT as it is below.  See
# ckuv7.hlp for further information.
#
# For 2.9 BSD, the makefile may use pcc rather than cc for compiles; that's
# what the CC and CC2 definitions are for (the current version of the
# makefile uses cc for both; this was tested in version 4E of C-Kermit and
# worked OK on the DEC Pro 380, but all bets are off for version 5A).  2.9
# support basically follows the 4.1 path.  Some 2.9 systems use "dir.h" for
# the directory header file, others will need to change this to "ndir.h".
# There are also newer entries for 2.10 BSD, which need testing and probably
# refinement, particularly a new entry that builds the program with overlays
# for DEC PDP-11s.
#
# The v7 and 2.9bsd versions assume I&D space on a PDP-11.  When building
# C-Kermit for v7 on a PDP-11, you should probably add the -i option to the
# link flags.  Without I&D space, overlays will be necessary (if available),
# or code segment mapping (a`la Pro/Venix if that's available).
#
##############################################################################
#
# V7-specific variables.
# These are set up for Perkin-Elmer 3230 V7 Unix:
# 
PROC=proc
DIRECT=
NPROC=nproc
NPTYPE=int
BOOTFILE=/edition7
#
# ( For old Tandy TRS-80 Model 16A or 6000 V7-based Xenix, use PROC=_proc,
#   DIRECT=-DDIRECT, NPROC=_Nproc, NPTYPE=short, BOOTFILE=/xenix )
#
###########################################################################
#
#  Compile and Link variables:
#
#  EXT is the extension (file type) for object files, normally o.
#  See MINIX entry for what to do if another filetype must be used.
#
EXT=o
LNKFLAGS=
SHAREDLIB=
CC= cc
CC2= cc
SHELL=/bin/sh
#
###########################################################################
#
# Easy installation. Modify this to suit your own computer's file organization
# and permissions.  If you don't have write access to the destination
# directories, "make install" will fail.

WERMIT = bsd
DESTDIR =
BINDIR = ${DESTDIR}/usr/bin
MANDIR = ${DESTDIR}/usr/man/man1
MANEXT = 1
ALL = $(WERMIT)

all: $(ALL)

install:
	install -c -s -o bin -g bin -m 555 wermit $(BINDIR)/kermit

install.man:
	install -c -o bin -g bin -m 444 ckuker.nr ${MANDIR}/kermit.${MANEXT}

FRC:

makewhat:
	@echo 'make what?  You must tell which system to make C-Kermit for.'
	@echo Examples:  make bsd, make sys3, make sunos4, etc.

###########################################################################
#
# Dependencies Section:

wermit:	ckcmai.$(EXT) ckucmd.$(EXT) ckuusr.$(EXT) ckuus2.$(EXT) ckuus3.$(EXT) \
		ckuus4.$(EXT) ckuus5.$(EXT) ckuus6.$(EXT) ckuus7.$(EXT) \
		ckuusx.$(EXT) ckuusy.$(EXT) ckcpro.$(EXT) ckcfns.$(EXT) \
		ckcfn2.$(EXT) ckcfn3.$(EXT) ckuxla.$(EXT) ckucon.$(EXT) \
		ckutio.$(EXT) ckufio.$(EXT) ckudia.$(EXT) ckuscr.$(EXT) \
		ckcnet.$(EXT)
	$(CC2) $(LNKFLAGS) -o wermit ckcmai.$(EXT) ckutio.$(EXT) \
		ckufio.$(EXT) ckcfns.$(EXT) ckcfn2.$(EXT) ckcfn3.$(EXT) \
		ckuxla.$(EXT) ckcpro.$(EXT) ckucmd.$(EXT) ckuus2.$(EXT) \
		ckuus3.$(EXT) ckuus4.$(EXT) ckuus5.$(EXT) ckuus6.$(EXT) \
		ckuus7.$(EXT) ckuusx.$(EXT) ckuusy.$(EXT) ckuusr.$(EXT) \
		ckucon.$(EXT) ckudia.$(EXT) ckuscr.$(EXT) ckcnet.$(EXT) $(LIBS)

#Malloc Debugging version

mermit:	ckcmdb.$(EXT) ckcmai.$(EXT) ckucmd.$(EXT) ckuusr.$(EXT) ckuus2.$(EXT) \
		ckuus3.$(EXT) ckuus4.$(EXT) ckuus5.$(EXT) ckuus6.$(EXT) \
		ckuus7.$(EXT) ckuusx.$(EXT) ckuusy.$(EXT) ckcpro.$(EXT) \
		ckcfns.$(EXT) ckcfn2.$(EXT) ckcfn3.$(EXT) ckuxla.$(EXT) \
		ckucon.$(EXT) ckutio.$(EXT) ckufio.$(EXT) ckudia.$(EXT) \
		ckuscr.$(EXT) ckcnet.$(EXT)
	$(CC2) $(LNKFLAGS) -o mermit ckcmdb.$(EXT) ckcmai.$(EXT) \
		ckutio.$(EXT) ckufio.$(EXT) ckcfns.$(EXT) ckcfn2.$(EXT) \
		ckcfn3.$(EXT) ckuxla.$(EXT) ckcpro.$(EXT) ckucmd.$(EXT) \
		ckuus2.$(EXT) ckuus3.$(EXT) ckuus4.$(EXT) ckuus5.$(EXT) \
		ckuus6.$(EXT) ckuus7.$(EXT) ckuusx.$(EXT) ckuusy.$(EXT) \
		ckuusr.$(EXT) ckucon.$(EXT) ckudia.$(EXT) ckuscr.$(EXT) \
		ckcnet.$(EXT) $(LIBS)

# Here is an example of building Kermit with overlays for a small machine,
# Like a PDP-11 without separate I&D space. This example comes from a 
# submission by Steven M Schultz <sms@wlv.imsd.contel.com>.

ovwermit: ckcmai.$(EXT) ckucmd.$(EXT) ckuusr.$(EXT) ckuus2.$(EXT) \
	ckuus3.$(EXT) ckuus4.$(EXT) ckuus5.$(EXT) ckcpro.$(EXT) \
	ckcfns.$(EXT) ckcfn2.$(EXT) ckcfn3.$(EXT) ckuxla.$(EXT) \
	ckucon.$(EXT) ckutio.$(EXT) ckufio.$(EXT) ckudia.$(EXT) \
	ckuscr.$(EXT) ckcnet.$(EXT) ckuus6.$(EXT) ckuus7.$(EXT) ckuusx.$(EXT) \
	ckuusy.$(EXT) ckustr.o
	$(CC2) $(LNKFLAGS) -o wermit /lib/crt0.$(EXT) ckcmai.$(EXT) \
		ckutio.$(EXT) ckufio.$(EXT) ckcfns.$(EXT) ckcfn2.$(EXT) \
		ckcfn3.$(EXT) \
		 -Z ckuxla.$(EXT) ckcpro.$(EXT) ckucmd.$(EXT) ckuus2.$(EXT) \
		 -Z ckuus4.$(EXT) ckuus5.$(EXT) ckuusr.$(EXT) ckuus6.$(EXT) \
		 -Z ckcfn3.$(EXT) ckucon.$(EXT) ckudia.$(EXT) ckuscr.$(EXT) \
		    ckcnet.$(EXT) ckuusy.$(EXT) \
		 -Z ckuus3.$(EXT) ckuus7.$(EXT) ckuusx.$(EXT) \
		 -Y ckustr.o -lc $(LIBS)

# Dependencies for each module...
#
# The following almost makes this work with gcc on the Sun-4.
# .SUFFIXES: .c .$(EXT)
# .$(EXT).c: ;	$(CC) $(CFLAGS) -c $<

ckcmai.$(EXT): ckcmai.c ckcker.h ckcdeb.h ckcsym.h ckcasc.h ckcnet.h

ckcpro.$(EXT): ckcpro.c ckcker.h ckcdeb.h ckcasc.h

ckcpro.c: ckcpro.w wart ckcdeb.h ckcasc.h ckcker.h
	./wart ckcpro.w ckcpro.c

ckcfns.$(EXT): ckcfns.c ckcker.h ckcdeb.h ckcsym.h ckcasc.h ckcxla.h \
		ckuxla.h

ckcfn2.$(EXT): ckcfn2.c ckcker.h ckcdeb.h ckcsym.h ckcasc.h ckcxla.h ckuxla.h

ckcfn3.$(EXT): ckcfn3.c ckcker.h ckcdeb.h ckcsym.h ckcasc.h ckcxla.h \
		ckuxla.h

ckuxla.$(EXT): ckuxla.c ckcker.h ckcdeb.h ckcxla.h ckuxla.h

ckuusr.$(EXT): ckuusr.c ckucmd.h ckcker.h ckuusr.h ckcdeb.h ckcxla.h ckuxla.h \
		ckcasc.h ckcnet.h

ckuus2.$(EXT): ckuus2.c ckucmd.h ckcker.h ckuusr.h ckcdeb.h ckcxla.h ckuxla.h \
		ckcasc.h

ckuus3.$(EXT): ckuus3.c ckucmd.h ckcker.h ckuusr.h ckcdeb.h ckcxla.h ckuxla.h \
		ckcasc.h ckcnet.h

ckuus4.$(EXT): ckuus4.c ckucmd.h ckcker.h ckuusr.h ckcdeb.h ckcxla.h ckuxla.h \
		ckcasc.h ckcnet.h

ckuus5.$(EXT): ckuus5.c ckucmd.h ckcker.h ckuusr.h ckcdeb.h ckcasc.h

ckuus6.$(EXT): ckuus6.c ckucmd.h ckcker.h ckuusr.h ckcdeb.h ckcasc.h

ckuus7.$(EXT): ckuus7.c ckucmd.h ckcker.h ckuusr.h ckcdeb.h ckcxla.h ckuxla.h \
		ckcasc.h ckcnet.h

ckuusx.$(EXT): ckuusx.c  ckcker.h ckuusr.h ckcdeb.h ckcasc.h

ckuusy.$(EXT): ckuusy.c  ckcker.h ckcdeb.h ckcasc.h

ckucmd.$(EXT): ckucmd.c ckcasc.h ckucmd.h ckcdeb.h

ckufio.$(EXT): ckufio.c ckcdeb.h ckuver.h

ckutio.$(EXT): ckutio.c ckcdeb.h ckcnet.h ckuver.h

ckucon.$(EXT): ckucon.c ckcker.h ckcdeb.h ckcasc.h ckcnet.h

ckcnet.$(EXT): ckcnet.c ckcdeb.h ckcker.h ckcnet.h

wart: ckwart.$(EXT)
	$(CC) $(LNKFLAGS) -o wart ckwart.$(EXT) $(LIBS)

ckcmdb.$(EXT): ckcmdb.c ckcdeb.h

ckwart.$(EXT): ckwart.c

ckudia.$(EXT): ckudia.c ckcker.h ckcdeb.h ckucmd.h ckcasc.h

ckuscr.$(EXT): ckuscr.c ckcker.h ckcdeb.h ckcasc.h

###########################################################################
#
# Make commands for specific systems:
#
#
#Apple Mac II, A/UX
#Warning, if "send *" doesn't work, try the auxufs makefile entry below.
aux:
	@echo Making C-Kermit $(CKVER) for Macintosh A/UX...
	make wermit "CFLAGS = -DAUX -DDYNAMIC -DTCPSOCKET -i -O" \
		"LNKFLAGS = -i"

#Apple Mac II, A/UX, but with ufs file volumes, uses <dirent.h>.
auxufs:
	@echo Making C-Kermit $(CKVER) for Macintosh A/UX...
	make wermit "CFLAGS = -DAUX -DDYNAMIC -DTCPSOCKET -DDIRENT -i -O" \
		"LNKFLAGS = -i"

#Berkeley Unix 4.1
bsd41:
	@echo Making C-Kermit $(CKVER) for 4.1BSD...
	make wermit "CFLAGS= -DBSD41" "LIBS = -ljobs"

#Berkeley 4.2, 4.3, also Ultrix-32 1.x, 2.x, 3.x, many others
# Add -O, -DDYNAMIC, -s, etc, if they work.
bsd:
	@echo Making C-Kermit $(CKVER) for 4.2BSD...
	make wermit "CFLAGS= -DBSD4 -DTCPSOCKET"

#Berkeley 4.2, 4.3, minimum size
bsdm:
	@echo Making C-Kermit $(CKVER) for 4.2BSD...
	make wermit "CFLAGS= -O -DBSD4 -DDYNAMIC \
	-DNODIAL -DNOHELP -DNODEBUG -DNOTLOG -DNOSCRIPT -DNOCSETS -DNOICP" \
	"LNKFLAGS = -s"

#Berkeley Unix with HoneyDanBer UUCP
bsdhdb:
	@echo Making C-Kermit $(CKVER) for 4.2BSD with HDB UUCP...
	make wermit "CFLAGS= -DHDBUUCP -DBSD4 -DTCPSOCKET"

#Berkeley Unix 4.3 with acucntrl program
bsd43:
	@echo Making C-Kermit $(CKVER) for 4.3BSD with acucntrl...
	make wermit "CFLAGS= -DBSD43 -DACUCNTRL -DTCPSOCKET -O"

#Berkeley Unix 4.2 or 4.3 with lock directory /usr/spool/uucp/LCK/LCK..ttyxx,
#but without acucntrl program
bsdlck:
	@echo Making C-Kermit $(CKVER) for 4.2BSD, /usr/spool/uucp/LCK/...
	make wermit "CFLAGS= -DLCKDIR -DBSD4 -DTCPSOCKET"

#Tektronix 6130, 4132, 43xx with UTek OS, /usr/spool/uucp/LCK./...
utek:
	@echo Making C-Kermit $(CKVER) for 4.2BSD/UTek
	make wermit "CFLAGS= -O -DLCKDIR -DBSD4 -DTCPSOCKET -DDYNAMIC \
	-DUTEK -DLOCK_DIR=\\\"/usr/spool/uucp/LCK.\\\""

#Mark Williams Coherent on IBM PC/AT family.
#There is a 64K limit on program size, so most features are compiled out.
#Result should be an interactive version with minimal features - no scripts,
#no character sets, no help, no dial, no debug/transaction logging, no
#transmit, msend, mail, type, etc, commands.
coherent:
	make "CFLAGS = -O -DCOHERENT -DNOANSI -DDYNAMIC -DNOICP \
	-DNOCSETS -DNOHELP -DNODIAL -DNOSCRIPT -DNODEBUG -DNOTLOG \
	-DNOSYSIOCTLH -DNOXMIT -DNOMSEND -DNOFRILLS -VSUVAR" wermit

#DEC Ultrix 2.x
# Add -O, -DDYNAMIC, -s, etc, if they work.
du2:
	@echo Making C-Kermit $(CKVER) for Ultrix 2.x ...
	make wermit "CFLAGS= -DBSD4 -DTCPSOCKET -DDU2"

#DEC Ultrix 3.1 (and probably also 3.0)
du3:
	@echo Making C-Kermit $(CKVER) for Ultrix 3.x...
	make wermit "CFLAGS= -DBSD4 -DTCPSOCKET -DDIRENT -DSIGTYP=void \
	-DDYNAMIC -O" "LNKFLAGS = -s"

#Name changed from ds4 to du4, should work on VAXes as well as DECstations.
ds4:
	make du4

#DEC Ultrix 4.0 on DECstation, VAXstation, VAX, etc.
du4:
	@echo Making C-Kermit $(CKVER) for Ultrix 4.x...
	make wermit "CFLAGS= -DBSD4 -DTCPSOCKET -DSIGTYP=void -DDYNAMIC \
	-Olimit 1619" "LNKFLAGS = -s"

#DEC Ultrix 4.0 on DECstation, VAXstation, VAX, etc, with Kanji support.
du4kanji:
	@echo Making C-Kermit $(CKVER) for Ultrix 4.x...
	make wermit "CFLAGS= -DBSD4 -DTCPSOCKET -DSIGTYP=void -DDYNAMIC \
	-DKANJI -Olimit 1619" "LNKFLAGS = -s"

#Sequent Dynix 3.x
dynix3:
	@echo Making C-Kermit $(CKVER) for Sequent Dynix 3.x...
	make wermit "CFLAGS= -DBSD43 -DACUCNTRL -DTCPSOCKET -O \
	-DUID_T=int -DGID_T=int"

#Encore, UMAX 4.3 (BSD) but without acucntrl program.
encore:
	make umax43

#Encore, UMAX 4.3 (BSD) but without acucntrl program.
umax43:
	@echo Making C-Kermit $(CKVER) for Encore UMAX 4.3...
	make PARALLEL=4 wermit \
	"CFLAGS= -DBSD43 -DENCORE -DDYNAMIC -DTCPSOCKET -O"

#Encore, UMAX 4.2 (BSD)
umax42:
	@echo Making C-Kermit $(CKVER) for Encore UMAX 4.3...
	make PARALLEL=4 wermit \
	"CFLAGS= -DBSD4 -DENCORE -DDYNAMIC -DTCPSOCKET -O"


#Encore 88K UMAX 5.3 with TCP/IP support
encore88K:
	@echo 'Making C-Kermit $(CKVER) for Encore 88K UMAX V, TCP/IP...'
	make wermit "CFLAGS = -q ext=pcc -DSVR3 -DTCPSOCKET -DDIRENT \
	-DHDBUUCP -DDYNAMIC -O" "LNKFLAGS ="

#Berkeley Unix 2.8, 2.9 for PDP-11s with I&D space, maybe also Ultrix-11???
#C-Kermit(5A) is simply too large (even turning off almost every feature
#available) to run without both I&D space plus overlays.  The old comment
#suggested running 'pcc' but that won't help.  Changing 'cc' to 'ckustr.sed'
#will cause a string extraction to be done, saving D space by moving strings
#to a file.
bsd29:
	@echo Making C-Kermit $(CKVER) for 2.8 or 2.9BSD.
	@echo Read the makefile if you have trouble with this...
	make ovwermit "CFLAGS= -DBSD29 -DNODEBUG -DNOTLOG -DNOCSETS -DNOHELP \
	-DNOSCRIPT -DNOSPL -DNOXMIT -DNODIAL" \
		"LNKFLAGS= -i -lndir" "CC= cc " "CC2= cc"

#Berkely UNIX 2.10 and 2.11, using overlays, see ovwermit above.
#2.10BSD and 2.11BSD (the latter to a larger extent) are the same as
#4.3BSD but without a large address space.  The BSD29 define should *not*
#be used.
#NOTE: a string extraction method is used to put approx. 9kb of strings
#into a file.  The module ckustr.c should be edited to change the pathname
#of the string file to where this file will reside.
bsd210:
	@echo Making C-Kermit $(CKVER) for 2.10BSD with overlays
	make ovwermit "CFLAGS= -O -DBSD43 -DLCKDIR -DNODEBUG -DNOTLOG \
	-DNOCSETS -DNOHELP -DNOSCRIPT -DNOSPL -DNOXMIT" \
		"LNKFLAGS= -i " "CC= ./ckustr.sed " "CC2= cc"

#Convex C1 with Berkeley Unix
convex:
	@echo Making C-Kermit $(CKVER) for Convex C1 / BSD...
	make wermit "CFLAGS= -DBSD4 -Dmsleep=mnap"

#Convex C210 with Convex/OS 8
convex8:
	@echo Making C-Kermit $(CKVER) for Convex C210 with OS 8
	make wermit \
	"CFLAGS= -DBSD4 -DTCPSOCKET -DNODEBUG -DDIRENT -DNOFILEH \
	-DDYNAMIC -DSIGTYP=void -Dmsleep=mnap"

#Convex C2 with Convex OS 9.1 (should also work with 8.1 or later)
#with ANSI C compiler, uses BSD 4.3 uucp lockfile convention.
convex9:
	@echo Making C-Kermit $(CKVER) for Convex C210 with OS 9.1
	make wermit \
	"CFLAGS= -DPOSIX -DDIRENT -DNOFILEH -O \
	-DDYNAMIC -D__STDC__ -DLCKDIR -Dmsleep=mnap -ext -tm c1"

#Cray X-MP or Y-MP UNICOS System V R3
#Maybe the -i link option should be removed.
cray:
	@echo 'Making C-Kermit $(CKVER) for Cray X/Y-MP UNICOS System V R3...'
	make wermit NPROC=1 "CFLAGS= -DSVR3 -DDIRENT -DHDBUUCP -i -O" \
	"LNKFLAGS = -i"

#NeXT
#Uses shared library to make executable program about 80K smaller.
#Remove "LIBS = -lsys_s" if this causes trouble.
next:
	@echo Making C-Kermit $(CKVER) for NeXT...
	make wermit \
	"CFLAGS= -DNEXT -DTCPSOCKET -DDYNAMIC -DLCKDIR -O -w" \
	"LIBS = -lsys_s"

#As above, but with Kanji support.
nextk:
	@echo Making C-Kermit $(CKVER) for NeXT...
	make wermit \
	"CFLAGS= -DNEXT -DTCPSOCKET -DDYNAMIC -DKANJI -DLCKDIR -O -w" \
	"LIBS = -lsys_s"

#NeXT version built with C-Kermit malloc debugger.
nextmd:
	@echo Making C-Kermit $(CKVER) for NeXT with malloc debug...
	make mermit \
	"CFLAGS= -DNEXT -DTCPSOCKET -DDYNAMIC -DLCKDIR -O -w \
	-Dmalloc=dmalloc -Dfree=dfree -DMDEBUG" "LIBS = -lsys_s"

#nextg - Gets useful error messages out of GNU cc.
# can't use -ansi here for some reason it clobbers definition of SIGQUIT (?)
nextg:
	@echo Making C-Kermit $(CKVER) for NeXT...
	make wermit "CFLAGS= -DNEXT -DTCPSOCKET -DDYNAMIC -DLCKDIR -Wall -O" \
	"LIBS = -lsys_s"

#POSIX
posix:
	@echo 'Making C-Kermit $(CKVER) for POSIX, no UUCP lockfile support...'
	make wermit "CFLAGS= -DPOSIX -DNOUUCP -O"

#POSIX
hdbposix:
	@echo Making C-Kermit $(CKVER) for POSIX with HDB UUCP...
	make wermit "CFLAGS= -DPOSIX -DHDBUUCP -O"

#SONY NEWS, NEWS-OS 4.01C
sonynews:
	@echo Making C-Kermit $(CKVER) for SONY NEWS-OS 4.01C...
	make wermit "CFLAGS= -DBSD43 -DKANJI -DACUCNTRL \
	-DTCPSOCKET -O"

#SUNPOSIX
sunposix:
	@echo Making C-Kermit $(CKVER) for POSIX...
	make wermit "CC= /usr/5bin/cc " \
	"CFLAGS= -DPOSIX -DHDBUUCP -DVOID=void -O"

#GCCPOSIX
# Doesn't work very well on SUN because of -target...
gccsunposix:
	@echo Making C-Kermit $(CKVER) for POSIX with gcc...
	make wermit "CC= gcc " \
	"CFLAGS= -Dsun -DPOSIX -DHDBUUCP -ansi -Wall -O"

#IBM's AIX 3.0 on IBM 370 mainframe, tested on AIX F44 thru F50.
aix370:
	@echo Making C-Kermit $(CKVER) for IBM System/370 AIX 3.0...
	make wermit "CFLAGS= -DAIX370 -DTCPSOCKET -DLCKDIR -DDIRENT -DYNAMIC" \
	"LIBS = -lbsd"

#IBM's AIX 3.0 on IBM PS/2, tested on AIX F44 thru F50.
#This is exactly the same as AIX370 except for the version herald.
#(Note, apparently AIX 3.0 for PS/2's was never formally released).
ps2aix3:
	@echo Making C-Kermit $(CKVER) for IBM PS/2 AIX 3.0...
	make wermit "CFLAGS= -DAIXPS2 -DTCPSOCKET -DLCKDIR -DDIRENT -DYNAMIC" \
	"LIBS = -lbsd"

#IBM AIX 3.0 for RISC System/6000.
rs6000:
	@echo Making C-Kermit $(CKVER) for IBM AIX 3.0 or 3.1, RS/6000...
	make wermit \
	"CFLAGS= -DAIXRS -DTCPSOCKET -DSVR3 -DDIRENT -DDYNAMIC -DCK_ANSIC" \
	"LNKFLAGS = -s"

#IBM AIX 3.0 for RISC System/6000.
rs6na:
	@echo Making C-Kermit $(CKVER) for IBM AIX 3.0 or 3.1, RS/6000...
	make wermit \
	"CFLAGS= -DAIXRS -DTCPSOCKET -DSVR3 -DDIRENT -DDYNAMIC -DNOANSI" \
	"LNKFLAGS = -s"

#SUN OS version 4.0, BSD environment, has saved original euid feature.
sunos4:
	@echo Making C-Kermit $(CKVER) for SUNOS 4.0, BSD environment...
	make wermit \
	"CFLAGS= -O -DSUNOS4 -DDIRENT -DTCPSOCKET -DSAVEDUID -DDYNAMIC"

#SUN OS version 4.0, BSD environment, has saved original euid feature.
# Like sunos4, but with SunLink X.25 support.
sunos4x25:
	@echo Making C-Kermit $(CKVER) for SUNOS 4.0 with SunLink X.25...
	make wermit \
	"CFLAGS= -O -DSUNOS4 -DDIRENT -DTCPSOCKET -DSAVEDUID \
	-DDYNAMIC -DSUNX25"

#SUN OS version 4.1, BSD environment, has saved original euid feature.
#Uses Honey DanBer UUCP.  Requires presence of /usr/spool/locks directory.
# /var/spool/ should be a symbolic link to  /usr/spool/.
sunos41:
	@echo Making C-Kermit $(CKVER) for SUNOS 4.1/BSD...
	make wermit \
	"CFLAGS= -O -DSUNOS41 -DHDBUUCP -DDIRENT -DTCPSOCKET \
	-DSAVEDUID -DDYNAMIC"

#As above, but with Kanji support.
sunos41k:
	@echo Making C-Kermit $(CKVER) for SUNOS 4.1/BSD, Cyrillic+Kanji...
	make wermit \
	"CFLAGS= -O -DSUNOS41 -DHDBUUCP -DDIRENT -DTCPSOCKET \
	-DSAVEDUID -DDYNAMIC -DKANJI"

#As above, but without Cyrillic support.
sunos41knc:
	@echo Making C-Kermit $(CKVER) for SUNOS 4.1/BSD, Kanji, no Cyrillic
	make wermit \
	"CFLAGS= -O -DSUNOS41 -DHDBUUCP -DDIRENT -DTCPSOCKET \
	-DSAVEDUID -DDYNAMIC -DKANJI -DNOCYRIL"

#SUN OS version 4.1, BSD environment, has saved original euid feature.
# Like sunos41, but with SunLink X.25 support.
sunos41x25:
	@echo Making C-Kermit $(CKVER) for SUNOS 4.1 with SunLink X.25...
	make wermit \
	"CFLAGS= -O -DSUNOS41 -DHDBUUCP -DDIRENT -DTCPSOCKET -DSAVEDUID \
	-DDYNAMIC -DSUNX25"

#SUN OS version 4.1, BSD environment, no character set translation.
# Otherwise just like sunos41.
sunos41ncs:
	@echo Making C-Kermit $(CKVER) for SUNOS 4.1, no character sets...
	make wermit \
	"CFLAGS= -O -DSUNOS41 -DHDBUUCP -DDIRENT -DTCPSOCKET -DSAVEDUID \
	-DDYNAMIC -DNOCSETS"

#SUN OS version 4.1, BSD environment, no DIAL command.
# Otherwise just like sunos41.
sunos41xd:
	@echo Making C-Kermit $(CKVER) for SUNOS 4.1, no DIAL...
	make wermit \
	"CFLAGS= -O -DSUNOS41 -DHDBUUCP -DDIRENT -DTCPSOCKET -DSAVEDUID \
	-DDYNAMIC -DNODIAL"

#SUN OS version 4.0, BSD environment, minimum size...
sunos4m:
	@echo Making C-Kermit $(CKVER) for SUNOS 4.0, minimum size...
	make wermit \
	"CFLAGS= -O -DSUNOS4 -DHDBUUCP -DDIRENT -DSAVEDUID -DDYNAMIC \
	-DNODIAL -DNOHELP -DNODEBUG -DNOTLOG -DNOSCRIPT -DNOCSETS -DNOICP \
	-DNOMSEND" "LNKFLAGS = -s"

#SUN OS version 4.1, BSD environment, minimum size...
sunos41m:
	@echo Making C-Kermit $(CKVER) for SUNOS 4.1, minimum size...
	make wermit \
	"CFLAGS= -O -DSUNOS41 -DHDBUUCP -DDIRENT -DSAVEDUID -DDYNAMIC \
	-DNODIAL -DNOHELP -DNODEBUG -DNOTLOG -DNOSCRIPT -DNOCSETS -DNOICP \
	-DNOMSEND" "LNKFLAGS = -s"

#SUN OS version 4.0 or later, BSD environment, minimum size w/command parser.
sunos4mi:
	@echo Making C-Kermit $(CKVER) for SUNOS 4.0, minimum interactive...
	make wermit \
	"CFLAGS= -O -DSUNOS4 -DHDBUUCP -DDIRENT -DSAVEDUID -DDYNAMIC \
	-DNOSPL -DNOXMIT -DNOMSEND -DNOFRILLS -DNODIAL -DNOHELP -DNODEBUG \
	-DNOTLOG -DNOSCRIPT -DNOCSETS" \
	"LNKFLAGS = -s"

#SUN OS version 4.1 or later, BSD environment, minimum size w/command parser.
sunos41mi:
	@echo Making C-Kermit $(CKVER) for SUNOS 4.1, minimum interactive...
	make wermit \
	"CFLAGS= -O -DSUNOS41 -DHDBUUCP -DDIRENT -DSAVEDUID -DDYNAMIC \
	-DNOSPL -DNOXMIT -DNOMSEND -DNOFRILLS -DNODIAL -DNOHELP -DNODEBUG \
	-DNOTLOG -DNOSCRIPT -DNOCSETS" \
	"LNKFLAGS = -s"

#SUN OS version 4.1, BSD environment, no network support.
sunos41nn:
	@echo Making C-Kermit $(CKVER) for SUNOS 4.1, BSD, no networks...
	make wermit \
	"CFLAGS= -O -DSUNOS41 -DHDBUUCP -DDIRENT -DSAVEDUID -DDYNAMIC"

#SUN OS version 4.1, BSD, no debugging, no transaction log.
sunos41ndt:
	@echo Making C-Kermit $(CKVER) for SUNOS 4.1, no debug or trans logs...
	make wermit \
	"CFLAGS= -O -DSUNOS41 -DSAVEDUID -DTCPSOCKET -DDYNAMIC -DDIRENT \
	-DNODEBUG -DNOTLOG"

#SUN OS version 4.1 or later, BSD, no debugging log.
sunos41nd:
	@echo Making C-Kermit $(CKVER) for SUNOS 4.1, no debug log...
	make wermit \
	"CFLAGS= -O -DSUNOS41 -DSAVEDUID -DTCPSOCKET -DDYNAMIC -DDIRENT \
	-DNODEBUG"

#SUN OS version 4.1 or later, BSD, no debugging log.
sunos41nt:
	@echo Making C-Kermit $(CKVER) for SUNOS 4.1, no transaction log...
	make wermit \
	"CFLAGS= -O -DSUNOS4 -DSAVEDUID -DTCPSOCKET -DDYNAMIC -DDIRENT \
	-DNOTLOG"

#SUN OS version 4.1 or later, BSD, no frills.
sunos41nf:
	@echo Making C-Kermit $(CKVER) for SUNOS 4.x, no frills...
	make wermit \
	"CFLAGS= -O -DSUNOS4 -DSAVEDUID -DTCPSOCKET -DDYNAMIC -DDIRENT \
	-DNOFRILLS"

#SUN OS version 4.1 or later, BSD, no command-line options.
sunos41nl:
	@echo Making C-Kermit $(CKVER) for SUNOS 4.x, no command-line opts...
	make wermit \
	"CFLAGS= -O -DSUNOS4 -DSAVEDUID -DTCPSOCKET -DDYNAMIC -DDIRENT \
	-DNOCMDL"

#SUN OS version 4.1 or later, BSD, no job control.
sunos41nj:
	@echo Making C-Kermit $(CKVER) for SUNOS 4.x, no job control...
	make wermit \
	"CFLAGS= -O -DSUNOS4 -DSAVEDUID -DTCPSOCKET -DDYNAMIC -DDIRENT \
	-DNOJC"

#SUN OS version 4.1 or later, BSD, dynamic command-bug allocation.
sunos41dc:
	@echo Making C-Kermit $(CKVER) for SUNOS 4.x, no frills...
	make wermit \
	"CFLAGS= -O -DSUNOS4 -DSAVEDUID -DTCPSOCKET -DDYNAMIC -DDIRENT \
	-DDCMDBUF"

#SUNOS 4.1 with malloc debugger
sunos41md:
	@echo Making C-Kermit $(CKVER) for SUNOS 4.1 malloc debug...
	make mermit \
	"CFLAGS= -O -DSUNOS41 -DHDBUUCP -DDIRENT -DTCPSOCKET \
	-DSAVEDUID -DDYNAMIC -Dmalloc=dmalloc -Dfree=dfree -DMDEBUG"

#SUN OS version 4.0, System V R3 environment (-i option omitted).
sunos4s5:
	@echo Making C-Kermit $(CKVER) for SUNOS 4.0, System V R3...
	@echo Ignore harmless complaints about redefinition of symbols...
	make wermit "CC= /usr/5bin/cc " \
	"CFLAGS = -DDIRENT -DSUN4S5 -DYNAMIC -O"

#SUN OS version 4.1 or later, System V R3 environment (-i option omitted).
#Like sunos4s5, but SUNOS 4.1 has switched to HDB UUCP lockfile conventions.
sunos41s5:
	@echo Making C-Kermit $(CKVER) for SUNOS 4.1 System V R3...
	@echo Ignore harmless complaints about redefinition of symbols...
	make wermit "CC= /usr/5bin/cc " \
	"CFLAGS = -DSUN4S5 -DDIRENT -DHDBUUCP -DDYNAMIC -O"

#SUN OS version 4.1 or later, gcc, profiling with gprof, no debugging.
#To get profile, "make sunos4p" (on SUN), then "./wermit".  After running
#wermit, "gprof ./wermit | lpr" or whatever to get execution profile.
sunos41p:
	@echo Making C-Kermit $(CKVER) for SUNOS 4.x with profiling...
	make wermit "CC= gcc " \
	"CFLAGS= -DSUNOS41 -DNODEBUG -DSAVEDUID -DDIRENT -DTCPSOCKET \
	-DDYNAMIC -pg" "LNKFLAGS = -pg"

#Apollo with Domain SR10.0 or later, BSD environment
#Reportedly, it might also help to add '-A,systype=bsd4.3' to CFLAGS.
#Reportedly, there is also a problem with getc & putc macros that can
#be handled by using '#ifdef apollo' somewhere to redefine them???
#On the other hand, other reports indicate that it works fine as-is.
sr10-bsd:
	@echo Making C-Kermit $(CKVER) for Apollo SR10.0 / BSD ...
	make wermit "CFLAGS= -DNOFILEH -DBSD4 -Uaegis -U__STDC__"

#Apollo with Domain SR10.0 or later, System V R3 environment.
#Don't use the optimizer (-O), it causes problems at runtime.
sr10-s5r3:
	@echo Making C-Kermit $(CKVER) for Apollo SR10.0 / Sys V R3 ...
	make wermit "CFLAGS= -DNOFILEH -DSVR3 -Uaegis -U__STDC__"

#Apollo Domain/IX (untested, try this if sr10-bsd doesn't work)
# (Can we add -DTCPSOCKET here?)
apollobsd:
	@echo Making C-Kermit $(CKVER) for Apollo Domain/IX...
	make wermit "CC= /bin/cc " "CC2= /bin/cc " \
	"CFLAGS= -DNOFILEH -DBSD4 -Uaegis"

#Version 7 Unix (see comments above)
v7:
	@echo Making C-Kermit $(CKVER) for UNIX Version 7.
	@echo Read the makefile if you have trouble with this...
	make wermit "CFLAGS=-DV7 -DPROCNAME=\\\"$(PROC)\\\" \
	-DBOOTNAME=\\\"$(BOOTFILE)\\\" -DNPROCNAME=\\\"$(NPROC)\\\" \
	-DNPTYPE=$(NPTYPE) $(DIRECT) -DO_RDWR=2 -DO_NDELAY=0 -DO_SCCS_ID"

#AT&T UNIX System V R3, signal() is void rather than int.
#Uses dirent.h and Honey DanBer uucp.
#Add the -i link option if necessary.
sys5r3:
	@echo 'Making C-Kermit $(CKVER) for AT&T UNIX System V R3...'
	make wermit "CFLAGS = -DSVR3 -DDIRENT -DHDBUUCP -DDYNAMIC -O" \
	"LNKFLAGS ="

#AT&T UNIX System V R3, for 3B computers with Wollongong TCP/IP.
sys5r3net3b:
	@echo 'Making C-Kermit $(CKVER) for AT&T UNIX SVR3/3B/Wollongong...'
	make wermit \
	"CFLAGS = -DSVR3 -DDIRENT -DHDBUUCP -DDYNAMIC -DWOLLONGONG -O" \
	"LIBS= -lnet -lnsl_s" "LNKFLAGS ="

#AT&T UNIX System V R3, signal() is void rather than int.
#Uses dirent.h and Honey DanBer uucp, has <termiox.h>.
#Has <termiox.h> for RTS/CTS flow control.
sys5r3tx:
	@echo 'Making C-Kermit $(CKVER) for AT&T UNIX System V R3...'
	make wermit "CFLAGS = -DSVR3 -DDIRENT -DHDBUUCP -DDYNAMIC \
	-DTERMIOX -i -O" "LNKFLAGS ="

#AT&T UNIX System V R3, signal() is void rather than int.
#Uses dirent.h and Honey DanBer uucp, has <termiox.h>.
#Has <sys/termiox.h> for RTS/CTS flow control.
sys5r3sx:
	@echo 'Making C-Kermit $(CKVER) for AT&T UNIX System V R3...'
	make wermit "CFLAGS = -DSVR3 -DDIRENT -DHDBUUCP -DDYNAMIC \
	-DSTERMIOX -i -O" "LNKFLAGS ="

#AT&T UNIX System V R4.
#Has <termiox.h>.
sys5r4:
	@echo 'Making C-Kermit $(CKVER) for AT&T UNIX System V R4...'
	make wermit "CFLAGS = -O -DSVR4 -DDIRENT -DHDBUUCP -DDYNAMIC \
	-DTERMIOX" "LNKFLAGS = -s"

#AT&T UNIX System V R4 with Wollongong TCP/IP.
#Has <termiox.h>.
sys5r4net:
	@echo 'Making C-Kermit $(CKVER) for AT&T UNIX System V R4...'
	make wermit "CFLAGS = -O -DSVR4 -DDIRENT -DHDBUUCP -DDYNAMIC \
	-DTERMIOX -DWOLLONGONG" "LNKFLAGS = -s"

#AT&T UNIX System V R4.
#Has <sys/termiox.h>.
sys5r4sx:
	@echo 'Making C-Kermit $(CKVER) for AT&T UNIX System V R4...'
	make wermit "CFLAGS = -O -DSVR4 -DDIRENT -DHDBUUCP -DDYNAMIC \
	-DSTERMIOX" "LNKFLAGS = -s"

#AT&T UNIX System V R4.
#Has <sys/termiox.h>, regular Berkeley sockets library, i.e. in.h and inet.h
#are not misplaced in sys (rather than netinet and arpa, respectively).
#Uses ANSI C constructs, <sys/termiox.h>, etc etc. 
sys5r4sxtcp:
	@echo 'Making C-Kermit $(CKVER) for AT&T UNIX System V R4...'
	make wermit "CFLAGS = -O -DSVR4 -DDIRENT -DHDBUUCP -DDYNAMIC \
	-DSTERMIOX -DTCPSOCKET" "LIBS= -lsocket -lnsl" "LNKFLAGS = -s"

#AT&T UNIX System V R4, has <sys/termiox.h>
#ANSI C function prototyping disabled.
sys5r4sxna:
	@echo 'Making C-Kermit $(CKVER) for AT&T UNIX System V R4...'
	make wermit "CFLAGS = -O -DSVR4 -DDIRENT -DHDBUUCP -DDYNAMIC \
	-DSTERMIOX -DNOANSI" "LNKFLAGS = -s"

#Commodore Amiga with AT&T UNIX System V R4 and TCP/IP support.
#Has <sys/termiox.h>.
svr4amiganet:
	@echo 'Making C-Kermit $(CKVER) for Amiga SVR4 + TCP/IP...'
	make wermit "CC=gcc" "CC2=gcc" \
	"CFLAGS = -O -DSVR4 -DDIRENT -DHDBUUCP -DDYNAMIC \
	-DSTERMIOX -DTCPSOCKET" "LNKFLAGS = -s" "LIBS = -lsocket -lnsl"

#ESIX SVR4.0.3 with TCP/IP support.
#Has <sys/termiox.h>, ANSI C function prototyping disabled.
esixr4:
	@echo 'Making C-Kermit $(CKVER) for ESIX SVR4 + TCP/IP...'
	make wermit "CFLAGS = -O -DSVR4 -DDIRENT -DHDBUUCP -DDYNAMIC -DNOANSI \
	-DSTERMIOX -DTCPSOCKET" "LNKFLAGS = -s" "LIBS = -lsocket -lnsl"

#AT&T UNIX System V R4.
#Has <sys/termiox.h>, Wollongong TCP/IP.
sys5r4sxnet:
	@echo 'Making C-Kermit $(CKVER) for AT&T UNIX System V R4...'
	make wermit "CFLAGS = -O -DSVR4 -DDIRENT -DHDBUUCP -DDYNAMIC \
	-DSTERMIOX -DWOLLONGONG" "LNKFLAGS = -s"

#AT&T UNIX System V R4, no <termio.x> or <sys/termio.x>.
sys5r4nx:
	@echo 'Making C-Kermit $(CKVER) for AT&T UNIX System V R4...'
	make wermit "CFLAGS = -O -DSVR4 -DDIRENT -DHDBUUCP -DDYNAMIC" \
	"LNKFLAGS = -s"

#AT&T UNIX System V R4, no <termio.x> or <sys/termio.x>.
sys5r4nxnet:
	@echo 'Making C-Kermit $(CKVER) for AT&T UNIX System V R4...'
	make wermit "CFLAGS = -O -DSVR4 -DDIRENT -DHDBUUCP -DDYNAMIC \
	-DWOLLONGONG" "LNKFLAGS = -s"

#Silicon Graphics System V R3 with BSD file system (IRIS)
iris:
	@echo Making C-Kermit $(CKVER) for Silicon Graphics IRIS...
	make wermit "CFLAGS = -O -DSVR3 -DLONGFN -I/usr/include/bsd" \
	"LIBS = -lbsd"

#Silicon Graphics IRIS System V R3
irix33:
	@echo 'Making C-Kermit $(CKVER) for AT&T UNIX System V R3...'
	make wermit "CFLAGS = -DSVR3 -DDIRENT -DHDBUUCP -DDYNAMIC -O" \
	"LNKFLAGS ="

#In case they type "make sys5"...
sys5:
	make sys3

#Generic ATT System III or System V (with I&D space)
sys3:
	@echo 'Making C-Kermit $(CKVER) for AT&T UNIX System III'
	@echo 'or System V R2 or earlier...'
	make wermit "CFLAGS = -DATTSV -i -O" "LNKFLAGS = -i"

#Generic ATT System III or System V R2 or earlier, "no void":
#special entry to remove "Illegal pointer combination" warnings.
sys3nv:
	@echo 'Making C-Kermit $(CKVER) for AT&T UNIX System III'
	@echo 'or System V R2 or earlier...'
	make wermit "CFLAGS = -DATTSV -Dvoid=int -i -O" "LNKFLAGS = -i"

#Generic ATT System III or System V (no I&D space)
sys3nid:
	@echo 'Making C-Kermit $(CKVER) for AT&T UNIX System III'
	@echo 'or System V R2 or earlier, no I&D space...'
	make wermit "CFLAGS = -DATTSV -O" "LNKFLAGS ="

#AT&T 7300/Unix PC systems, sys3 but special handling for internal modem.
#Link with the shared library -- the conflict with openi in shared library
#is solved with -Dopeni=xopeni
sys3upc:
	@echo 'Making C-Kermit $(CKVER) for AT&T 7300 UNIX PC, shared lib...'
	@echo 'If shared lib causes trouble, use make sys3upcold.'
	make wermit "CFLAGS = -O -DATT7300 -DDYNAMIC -Dopeni=xopeni" \
	"CC2 = ld /lib/crt0s.o /lib/shlib.ifile" "LNKFLAGS = -s"

#AT&T 7300/Unix PC systems, as above, but use gcc.
sys3upcg:
	@echo 'Making C-Kermit $(CKVER) for AT&T 7300 UNIX PC...'
	make wermit "CFLAGS = -O -DATT7300 -DDYNAMIC -Dopeni=xopeni" \
	"CC = gcc" "CC2 = gcc" "LNKFLAGS = -s -shlib"

#AT&T 7300/Unix PC systems, sys3 but special handling for internal modem.
sys3upcold:
	@echo 'Making C-Kermit $(CKVER) for AT&T 7300 UNIX PC...'
	make wermit "CFLAGS = -DATT7300 -O" "LNKFLAGS = -i"

#AT&T 6300 PLUS (warning, -O makes it run out of space).
#NOTE: Remove -DHDBUUCP if not using Honey DanBer UUCP.
att6300:
	@echo 'Making C-Kermit $(CKVER) for AT&T 6300 PLUS...'
	make wermit "CFLAGS = -DATT6300 -DHDBUUCP -Ml -i" \
	"LNKFLAGS = -i -Ml"

#AT&T 6300 PLUS with no debugging (about 34K smaller.  Can't use -O here.)
att6300nd:
	@echo 'Making C-Kermit $(CKVER) for AT&T 6300 PLUS, no debugging...'
	make wermit "CFLAGS = -DATT6300 -DHDBUUCP -DNODEBUG -i -Ml" \
		"LNKFLAGS = -i -Ml"

#AT&T 3B2, 3B20-series computers running AT&T UNIX System V.
#This is just generic System V with Honey DanBer UUCP, so refer to sys3hdb.
att3bx:
	make sys3hdb

#Any System V R2 or earlier with Honey DanBer UUCP (same as above)
sys3hdb:
	@echo 'Making C-Kermit $(CKVER) for AT&T UNIX System III'
	@echo 'or System V R2 or earlier with Honey DanBer UUCP...'
	make wermit "CFLAGS = -DATTSV -DHDBUUCP -i -O" \
	"LNKFLAGS = -i"

#In case they say "make sys5hdb" instead of "make sys3hdb"...
sys5hdb:
	make sys3hdb

#IBM PS/2 with AIX 1.0 (currently in field test as F10A)
#  Reports indicate that -O switch must be omitted.
#  It is also possible that "made bsd" will work (reports welcome).
#  One report recommended "make LIBS=-lbsd bsd" did the trick.
ps2aix:
	@echo 'Making C-Kermit $(CKVER) for IBM AIX 1.0 PS/2...'
	make wermit "CFLAGS = -DATTSV -i" "LNKFLAGS = -i"

#IBM RT PC with AIX 2.2.1
#This one has unique and strange lockfiles.
rtaix:
	@echo 'Making C-Kermit $(CKVER) for IBM RT PC, AIX 2.2.1...'
	make wermit "CFLAGS = -DATTSV -DRTAIX -DHDBUUCP -DTCPSOCKET -O -w" \
	"LNKFLAGS = -i"

#IBM RT PC with AIX 2.2.1 (BSD 4.3)
# Add -O, -DDYNAMIC, -s, etc, if they work.
rtacis:
	@echo Making C-Kermit $(CKVER) for RT PC with ACIS 2.2.1 = BSD 4.3...
	make wermit "CFLAGS= -DBSD4 -DTCPSOCKET -U__STDC__"

#HP 9000 series 300, 500, 800, no long filenames and no job control.
#This is certainly only good for HPUX versions earlier than 6.5.
hpuxpre65:
	@echo 'Making C-Kermit $(CKVER) for HP-9000 HP-UX, no long filenames.'
	make wermit "CFLAGS = -DHPUX -DHPUXPRE65 -O" "LNKFLAGS ="

#HP 9000 series 300, 500, 800, no long filenames.
#This is probably only good for HPUX versions earlier than 6.2.
hpux:
	@echo 'Making C-Kermit $(CKVER) for HP-9000 HP-UX, no long filenames.'
	make wermit "CFLAGS = -DHPUX -O" "LNKFLAGS ="

#HP-UX 7.0, no long filenames, no network support.
hpux7sf:
	@echo 'Making C-Kermit $(CKVER) for HP-9000 HP-UX, no long filenames.'
	make wermit "CFLAGS = -DHPUX -DSIGTYP=void -O" "LNKFLAGS ="

#HP 9000 series 300, 500, 800, long filenames (using BSD file system)
# (This one is probably necessary for the Series 300)
hpuxlf:
	@echo 'Making C-Kermit $(CKVER) for HP-9000 HP-UX, long filenames...'
	make wermit "CFLAGS = -DHPUX -DNDIR -DLONGFN -DYNAMIC -O" \
	"LNKFLAGS ="

#HP 9000 series 300, 500, 800, long filenames (using <dirent.h>)
hpuxde:
	@echo 'Making C-Kermit $(CKVER) for HP-9000 HP-UX, long filenames...'
	make wermit "CFLAGS = -DHPUX -DDIRENT -DDYNAMIC -O" \
	"LNKFLAGS ="

#HP 9000 series 300, 500, 800, long filenames, System V R3 or later
# (Does anybody know what is the earliest release of HP-UX based on SVR3?)
hpuxs5r3:
	@echo 'Making C-Kermit $(CKVER) for HP-9000 HP-UX, long filenames...'
	make wermit \
	"CFLAGS = -DHPUX -DSVR3 -DDIRENT -DTCPSOCKET -O" \
	"LNKFLAGS =" "LIBS=-lBSD"

#HP 9000 series 800 HP-UX 7.0, long filenames, network support, HDB uucp
#there must be <arpa/telnet.h> & <arpa/inet.h> present to support this
#configuration.  To use this, you must have bought the ARPA Services
#Product from HP, and you must get the files "telnet.h" and "inet.h"
#from the Berkeley Standard Distribution because (reportedly) they are not
#part of the HP product.
hpux70lfn:
	@echo 'Making C-Kermit $(CKVER) for HP9000/8xx HP-UX V. 7.0'
	@echo 'supporting: long filenames, networking, HDB uucp...'
	make wermit "CFLAGS = -DHPUXDEBUG -DHPUX -DSVR3 \
	-DDIRENT -DLONGFN -DHDBUUCP -DLOCK_DIR=\\\"/usr/spool/uucp\\\" \
	-DTCPSOCKET -O" "LNKFLAGS = -s" "LIBS = -lBSD"

#Regulus on CIE Systems 680/20
cie:
	@echo 'Making C-Kermit $(CKVER) for CIE Systems 680/20 Regulus...'
	make wermit "CFLAGS = -DATTSV -DNOFILEH -DCIE -O" \
	"LNKFLAGS ="

#Microport SV/AT for IBM PC/AT 286 and clones, System V R2.
#The -O flag may fail on some modules (like ckuus2.c), in which case you 
#should compile them by hand, omitting the -O.  If you get "hash table
#overflow", try adding -DNODEBUG.
#Also, reportedly this compiles better with gcc than with cc.
mpsysv:
	@echo 'Making C-Kermit $(CKVER) for Microport SV/AT 286...'
	make wermit "CFLAGS= -DATTSV -O -Ml" "LNKFLAGS = -Ml"

#Microsoft "Xenix/286" e.g. for IBM PC/AT
xenix:
	@echo 'Making C-Kermit $(CKVER) for Xenix/286'
	make wermit "CFLAGS= -DXENIX -DNOFILEH -Dunix -F 3000 -i" \
	"LNKFLAGS = -F 3000 -i"

#SCO Xenix/286 2.2.1, e.g. for IBM PC/AT, PS/2 Model 50, etc.
#Reportedly, this "make" can fail simply because of the size of this
#makefile.  If that happens, use "makeL", or edit out some of the
#other entries.
sco286:
	@echo 'Making C-Kermit $(CKVER) for SCO Xenix/286...'
	@echo 'If make fails, try using makeL.'
	make wermit \
	"CFLAGS= -s -O -LARGE -DXENIX -DNOFILEH -Dunix -DRDCHK -DNAP -F 3000 \
	-i -M2let32" \
	"LIBS = -lx" "LNKFLAGS = -s -O -LARGE -F 3000 -i -M2let32"

#SCO Xenix/286 2.2.1, e.g. for IBM PC/AT, PS/2 Model 50, etc.
#As above, but with HDBUUCP
sco286hdb:
	@echo 'Making C-Kermit $(CKVER) for SCO Xenix/286 with HDB UUCP...'
	@echo 'If make fails, try using makeL.'
	make wermit \
	"CFLAGS= -s -O -LARGE -DXENIX -DNOFILEH -Dunix -DRDCHK -DNAP \
	-DHDBUUCP -F 3000 -i -M2let32" \
	"LIBS = -lx" "LNKFLAGS = -s -O -LARGE -F 3000 -i -M2let32"

#SCO Xenix 2.2.1 for IBM PC, XT, PS2/30, or other 8088 or 8086 machine
#If this doesn't work, try some of the tricks from sco286.
sco86:
	@echo 'Making C-Kermit $(CKVER) for SCO Xenix/86...'
	make wermit \
	"CFLAGS= -DXENIX -DNOFILEH -Dunix -F 3000 -i -M0me" \
	"LNKFLAGS = -F 3000 -i -M0me" "LIBS = -lx"

#SCO Xenix/386 2.2.2
sco386:
	@echo 'Making C-Kermit $(CKVER) for SCO Xenix/386...'
	make wermit \
	"CFLAGS= -DXENIX -DNOFILEH -Dunix -DRDCHK -DNAP -Otcl -i -M3e" \
	"LNKFLAGS = -i" "LIBS = -lx"

#SCO UNIX/386 3.2.0, 3.2.1, and SCO Xenix 2.3.x
#Maybe the -i link option should be removed?
sco3r2:
	@echo 'Making C-Kermit $(CKVER) for SCO UNIX/386...'
	@echo 'Warning: If make blows up, edit the makefile to join'
	@echo 'the following three continued lines into one line.'
	make wermit \
	"CFLAGS= -DXENIX -DSVR3 -DNOFILEH -DHDBUUCP -DRDCHK -DNAP \
	-DNOJC -Otcl -i -M3e" "LNKFLAGS = -i" "LIBS = -lc -lx"

# Exactly the same as above, but enables some special SCO-specific code
# that allegedly clears up some problems with HANGUP and with uugetty.
# For satisfactory operation on bidrectional lines that are handled by
# uugetty, you must install the kermit program with owner=group=uucp
# and permission 06755.
sco3r2x:
	@echo 'Making C-Kermit $(CKVER) for SCO UNIX/386...'
	@echo 'Warning: If make blows up, edit the makefile to join'
	@echo 'the following three continued lines into one line.'
	make wermit \
	"CFLAGS= -DXENIX -DSVR3 -DNOFILEH -DHDBUUCP -DRDCHK -DNAP \
	-DNOJC -DSCO32 -Otcl -i -M3e" "LNKFLAGS = -i" "LIBS = -lc -lx"

#SCO UNIX/386 3.2.0 and SCO Xenix 2.3.x with Excelan TCP/IP support
#Maybe the -i link option should be removed?
sco3r2net:
	@echo 'Making C-Kermit $(CKVER) for SCO UNIX/386 / Excelan...'
	@echo 'Warning: If make blows up, edit the makefile to join'
	@echo 'the following three continued lines into one line.'
	make wermit \
	"CFLAGS= -I/usr/include/exos -DXENIX -DNOFILEH -DHDBUUCP -DRDCHK \
	-DSVR3 -DNAP -DTCPSOCKET -DEXCELAN -DNOJC -Otcl -i -M3e" \
	"LNKFLAGS = -i" "LIBS = -lc -lx -lsocket"

#SCO Xenix 2.3.x with Racal InterLan TCP/IP support
# Extra compile flags for other version of Racal InterLan TCP/IP:
# Xenix286/NP621-286, use -Ml -DPARAMH -DINTERLAN -Di286 -DSYSV
# Xenix386/NP621-386, use -DPARAMH -DINTERLAN -Di386 -DSYSV
# ISC386ix/NP622I, use -DSYSV -Di386
# SCO Unix3.2/NP622S, use -DSYSV -Di386 -DSCO_UNIX
# AT&T SVR3.2/NP622A, use -DSYSV -Di386 -DATT
#Maybe the -i link option should be removed?
sco3r2netri:
	@echo 'Making C-Kermit $(CKVER) for SCO UNIX/386 / Racal InterLan...'
	@echo 'Warning: If make blows up, edit the makefile to join'
	@echo 'the following four continued lines into one line.'
	make wermit \
	"CFLAGS= -I/usr/include/interlan -DXENIX -DNOFILEH -DHDBUUCP \
	-DSVR3 -DRDCHK -DNAP -DTCPSOCKET -DPARAMH -DINTERLAN -Di386 -DSYSV \
	-DNOJC -Otcl -i -M3e" "LNKFLAGS = -i" "LIBS = -lc -lx -ltcp"

#SCO UNIX/386 3.2.2 or later (supports POSIX job control)
#Maybe the -i link option should be removed?
sco3r22:
	@echo 'Making C-Kermit $(CKVER) for SCO UNIX/386 3.2.2...'
	@echo 'Warning: If make blows up, edit the makefile to join'
	@echo 'the following three continued lines into one line.'
	make wermit \
	"CFLAGS= -DXENIX -DSVR3 -DNOFILEH -DHDBUUCP -DRDCHK -DNAP \
	-DPWID_T=int -Otcl -i -M3e" "LNKFLAGS = -i" "LIBS = -lx"

#SCO UNIX/386 3.2.2 or later with gcc 1.40 or later, POSIX job control
sco3r22gcc:
	@echo 'Making C-Kermit $(CKVER) for SCO UNIX/386 3.2.2, gcc'
	@echo 'Warning: If make blows up, edit the makefile to join'
	@echo 'the following three continued lines into one line.'
	make wermit "CC = gcc" \
	"CFLAGS= -O -DPOSIX -DXENIX -DSVR3 -DNOFILEH -DHDBUUCP -DRDCHK -DNAP \
	-traditional -fpcc-struct-return -fstrength-reduce \
	-DPWID_T=int " "LNKFLAGS = " "LIBS = -lx"

#SCO UNIX/386 3.2.2 or later (supports POSIX job control) with SCO TCP/IP
#Maybe the -i link option should be removed?
sco3r22net:
	@echo 'Making C-Kermit $(CKVER) for SCO UNIX/386 3.2.2 + TCP/IP...'
	@echo 'Warning: If make blows up, edit the makefile to join'
	@echo 'the following three continued lines into one line.'
	make wermit \
	"CFLAGS= -DXENIX -DSVR3 -DNOFILEH -DHDBUUCP -DRDCHK -DNAP -DTCPSOCKET \
	-DPWID_T=int -Otcl -i -M3e" "LNKFLAGS = -i" "LIBS = -lx -lsocket -lc_s"

#PC/IX, Interactive Corp System III for IBM PC/XT
pcix:
	@echo 'Making C-Kermit $(CKVER) for PC/IX...'
	make wermit \
	"CFLAGS= -DPCIX -DISIII -Dsdata=sdatax -O -i" "LNKFLAGS = -i"

#Interactive Corp System III port in general --
is3:
	@echo 'Making C-Kermit $(CKVER) for Interactive System III...'
	make wermit \
	"CFLAGS= -DISIII -Ddata=datax -O -i" "LNKFLAGS= -i"

#Interactive UNIX System V R3, signal() is void rather than int.
#Uses dirent.h and Honey DanBer uucp.
is5r3:
	@echo 'Making C-Kermit $(CKVER) for Interactive 386/ix or later...'
	make wermit "CFLAGS = -DSVR3 -DDIRENT -DHDBUUCP -g -DNOCSETS \
	-DDYNAMIC -DI386IX" "LNKFLAGS = -g"

#Interactive UNIX System V R3, Posix variant.  Untested.
#Uses dirent.h and Honey DanBer uucp.
is5r3p:
	@echo 'Making C-Kermit $(CKVER) for Interactive 386/ix or later...'
	make wermit "CFLAGS= -DSVR3 -DDIRENT -DHDBUUCP -g -DNOCSETS \
	-DDYNAMIC -DI386IX -DPOSIX" "LNKFLAGS=" "LIBS=-lcposix"

#Interactive UNIX System V R3, signal() is void rather than int.
#Uses dirent.h and Honey DanBer uucp. Needs -linet for net functions.
is5r3net:
	@echo 'Making C-Kermit $(CKVER) for Interactive 386/ix or later...'
	make wermit "CFLAGS = -DSVR3 -DDIRENT -DHDBUUCP -DDYNAMIC -DTCPSOCKET \
	-DI386IX -O" "LIBS = -linet"

#Interactive UNIX System V R3.
#Like is5r3net, but with -g (whatever that is, some like it, some don't).
is5r3netg:
	@echo 'Making C-Kermit $(CKVER) for Interactive 386/ix or later...'
	make wermit "CFLAGS = -DSVR3 -DDIRENT -DHDBUUCP -DDYNAMIC -DTCPSOCKET \
	-DI386IX -g" "LNKFLAGS = -g" "LIBS = -linet"

#Masscomp System III
rtu:
	@echo 'Making C-Kermit $(CKVER) for Masscomp RTU System III...'
	make wermit "CFLAGS= -UFIONREAD -DATTSV -O" "LNKFLAGS =" "LIBS= -ljobs"

#Masscomp/Concurrent RTU 4.0 or later, Berkeley environment.
#Includes <ndir.h> = /usr/include/ndir.h
#Note "LIBS = lndir" might not be necessary because of "ucb make".
rtubsd:
	@echo 'Making C-Kermit $(CKVER) for Masscomp RTU 4.1A...'
	ucb make wermit "CFLAGS= -DBSD4 -DRTU -DNDIR -DHDBUUCP -DTCPSOCKET" \
	"LIBS = -lndir"
	
#Masscomp/Concurrent RTU 4.0 or later, same as above,
#Includes "usr/lib/ndir.h"
#Note "LIBS = -lndir" might not be necessary because of "ucb make".
rtubsd2:
	@echo 'Making C-Kermit $(CKVER) for Masscomp RTU 4.1A...'
	ucb make wermit "CFLAGS= -DBSD4 -DRTU -DXNDIR -DHDBUUCP" \
	"LIBS = -lndir"

#Masscomp/Concurrent RTU 4.0 or later, same as above,
#Includes <sys/ndir.h>
#Note "LIBS = lndir" might not be necessary because of "ucb make".
rtubsd3:
	@echo 'Making C-Kermit $(CKVER) for Masscomp RTU 4.x BSD...'
	ucb make wermit "CFLAGS= -DBSD4 -DRTU -DHDBUUCP" "LIBS = -lndir"

#Masscomp/Concurrent RTU 4.0 or later, System V R2, using <dirent.h>.
#If this gives problems, add back the -DRTU switch.
rtus5:
	@echo 'Making C-Kermit $(CKVER) for Masscomp RTU 4.x BSD...'
	make wermit "CFLAGS= -DATTSV -DHDBUUCP -DDIRENT"

#Masscomp/Concurrent RTU 4.x, System V R3, using <dirent.h>.
#Use this one if rtus5 gives warnings about pointer type mismatches.
#If this gives problems, add back the -DRTU switch.
rtus5r3:
	@echo 'Making C-Kermit $(CKVER) for Masscomp RTU Sys V R3...'
	make wermit "CFLAGS= -DSVR3 -DHDBUUCP -DDIRENT"

#DEC Pro-3xx with Pro/Venix V1.0 or V1.1
# Requires code-mapping on non-I&D-space 11/23 processor, plus some
# fiddling to get interrupt targets into resident code section.
# This almost certainly doesn't work any more, but there are almost certainly
# no more of these systems in existence.
provx1:
	@echo 'Making C-Kermit $(CKVER) for DEC Pro-3xx, Pro/Venix 1.x...'
	make wart "CFLAGS= -DPROVX1" "LNKFLAGS= "
	make wermit "CFLAGS = -DPROVX1 -DNOFILEH -md780" \
		"LNKFLAGS= -u _sleep -lc -md780"

#NCR Tower 1632, OS 1.02
tower1:
	@echo 'Making C-Kermit $(CKVER) for NCR Tower 1632, OS 1.02...'
	make wermit "CFLAGS= -DTOWER1"

#NCR Tower 32, OS Release 1.xx.xx
tower32-1:
	@echo 'Making C-Kermit $(CKVER) for NCR Tower 32 Rel 1 System V R2...'
	make wermit "CFLAGS = -DATTSV -DDYNAMIC -O" "LNKFLAGS = -n"

#NCR Tower 32, OS Release 2.xx.xx
tower32-2:
	@echo 'Making C-Kermit $(CKVER) for NCR Tower 32 Rel 2 System V R2...'
	make wermit "CFLAGS = -DATTSV -DHDBUUCP -DDYNAMIC -O2" "LNKFLAGS = -n"

#NCR Tower 32, OS Releases based on System V R3
#Don't add -DNAP (doesn't work right) or -DRDCHK (not available in libc).
tower32:
	@echo 'Making C-Kermit $(CKVER) for NCR Tower 32 System V R3...'
	make wermit \
	"CFLAGS = -DSVR3 -DDIRENT -DHDBUUCP -DDYNAMIC -DNOSYSIOCTLH
	-DUID_T=ushort -DGIDT=ushort -O1"

#NCR Tower 32, OS Releases based on System V R3
tower32g:
	@echo 'Making C-Kermit $(CKVER) for NCR Tower 32 System V R3, gcc...'
	make wermit "CC = gcc" \
	"CFLAGS = -DSVR3 -DDIRENT -DHDBUUCP -DDYNAMIC -DNOSYSIOCTLH \
	DUID_T=ushort -DGIDT=ushort -O -fstrength-reduce -fomit-frame-pointer"

#Fortune 32:16, For:Pro 1.8 (mostly like 4.1bsd)
ft18:
	@echo 'Making C-Kermit $(CKVER) for Fortune 32:16 For:Pro 1.8...'
	make wermit "CFLAGS= -DNODEBUG -DBSD4 -DFT18 -DNOFILEH \
	-DPID_T=short"

#Fortune 32:16, For:Pro 2.1 (mostly like 4.1bsd)
ft21:
	@echo 'Making C-Kermit $(CKVER) for Fortune 32:16 For:Pro 2.1...'
	make wermit "CFLAGS= -O -DNODEBUG -DBSD4 -DFT21 -DNOFILEH -SYM 800 \
	-DDYNAMIC -DPID_T=short" "LNKFLAGS= -n -s" "LIBS= -lv -lnet"

#Valid Scaldstar
#Berkeleyish, but need to change some variable names.
valid:
	@echo 'Making C-Kermit $(CKVER) for Valid Scaldstar...'
	make wermit "CFLAGS= -DBSD4 -DNODEBUG -DNOTLOG -Dcc=ccx -DFREAD=1"

#IBM IX/370 on IBM 370 Series mainframes
#Mostly like sys3, but should buffer packets.
ix370:
	@echo 'Making C-Kermit $(CKVER) for IBM IX/370...'
	make wermit "CFLAGS = -DIX370 -DATTSV -i -O" \
	"LNKFLAGS = -i"

#Amdahl UTS 2.4 on IBM 370 series compatible mainframes.
#Mostly like V7, but can't do initrawq() buffer peeking.
uts24:
	@echo 'Making C-Kermit $(CKVER) for Amdahl UTS 2.4...'
	make wermit "CFLAGS=-DV7 -DPROCNAME=\\\"$(PROC)\\\" \
	-DUTS24 -DBOOTNAME=\\\"$(BOOTFILE)\\\" -DNPROCNAME=\\\"$(NPROC)\\\" \
	-DNPTYPE=$(NPTYPE) $(DIRECT)"

#Amdahl UTSV UNIX System V = System V R2 or earlier.
utsv:
	@echo 'Making C-Kermit $(CKVER) for Amdahl UTSV...'
	make wermit "CFLAGS = -DUTSV -i -O" "LNKFLAGS = -i"

#BBN C/70 with IOS 2.0
#Mostly Berkeley-like, but with some ATTisms
c70:
	@echo 'Making C-Kermit $(CKVER) for BBN C/70 IOS 2.0...'
	make wermit "CFLAGS= -DBSD4 -DC70"

#Zilog ZEUS 3.21
zilog:
	@echo 'Making C-Kermit $(CKVER) for Zilog Zeus 3.21...'
	make wermit "CFLAGS = -DATTSV -DZILOG -DNODEBUG -i -O" \
	"LNKFLAGS = -i -lpw"

#Whitechapel MG-1 Genix 1.3
white:
	@echo 'Making C-Kermit $(CKVER) for Whitechapel MG-1 Genix 1.3...'
	@touch ckcpro.c
	make wermit "CFLAGS= -DBSD4 -Dzkself()=0"

#Pixel 1000
pixel:
	@echo 'Making C-Kermit $(CKVER) for Pixel 1000...'
	make wermit "CFLAGS= -DBSD4 -Dzkself()=0"

#Sequent DYNIX/PTX 1.2.1
ptx:
	@echo Making C-Kermit $(CKVER) for Sequent DYNIX/PTX 1.2.1...
	make wermit "CFLAGS= -DSVR3 -DDIRENT -DHDBUUCP -DDYNAMIC -DPTX \
	-DPID_T=pid_t -DUID_T=uid_t -DGID_T=gid_t -i -O" "LNKFLAGS = -i"

#CDC VX/VE 5.2.1
vxve:
	@echo 'Making C-Kermit $(CKVER) for CDC VX/VE 5.2.1...'
	make wermit "CFLAGS = -DATTSV -DVXVE -DNODEBUG -DNOTLOG -i -O" \
	"LNKFLAGS = -i"

#Tandy 16/6000 with Xenix 3.0
#Add -DNOxxx options to remove features if program won't load.
trs16:
	@echo 'Making C-Kermit $(CKVER) for Tandy 16/16000, Xenix 3.0...'
	make wermit "CFLAGS = -DATTSV -DTRS16 -DDCLPOPEN -O" \
	"LNKFLAGS = -n -s"

#DIAB DS90 or LUXOR ABC-9000 with pre-5.2 DNIX.  Sys V with nap() and rdchk().
# nd = no opendir(), readdir(), closedir(), etc.
# Some of the modules fail to compile with -O.
dnixnd:
	@echo 'Making C-Kermit $(CKVER) for DIAB DS90 with very old DNIX 5.2.'
	make wermit "CFLAGS = -DATTSV -DNAP -DRDCHK -DDYNAMIC -DDCLPOPEN \
	-U__STDC__"

#DIAB DS90 with DNIX 5.2.  Sys V with nap() and rdchk().
# This one has opendir(), readdir(), closedir(), etc.
# Some of the modules fail to compile with -O.
dnix:
	@echo 'Making C-Kermit $(CKVER) for DIAB DS90 with old DNIX 5.2...'
	make wermit "CFLAGS = -DATTSV -DNAP -DRDCHK -DDIRENT -DDYNAMIC  \
	-U__STDC__"

#DIAB DS90 with DNIX 5.3 or later, with HDB UUCP, nap() and rdchk().
dnix5r3:
	@echo 'Making C-Kermit $(CKVER) for DIAB DS90 with DNIX 5.3...'
	@echo 'with Honey DanBer UUCP'
	make wermit \
	"CFLAGS = -DSVR3 -DHDBUUCP -DNAP -DRDCHK -DDIRENT -DDYNAMIC -O"

#Ridge 32 with ROS 3.2
ridge32:
	@echo 'Making C-Kermit $(CKVER) Ridge 32 ROS 3.2'
	make wermit \
	"CFLAGS = -DATTSV -DNOFILEH -DNODEBUG -DNOTLOG -i -O" "LNKFLAGS = -i"

#Altos 486, 586, or 986 with Xenix 3.0
altos:
	@echo 'Making C-Kermit $(CKVER) for Altos x86 with Xenix 3.0...'
	make wermit \
	"CFLAGS= -DATTSV -DA986 -DNODEBUG -DNOTLOG -i -O" \
	"LNKFLAGS= -i"

#Altos 986 with Xenix 3.0, as above, but command-line only, minimal size.
#For systems with small memories.  It might also be necessary to chop certain
#modules up into smaller pieces, e.g. ckuus3-6, because of symbol table
#overflow.   If this makefile is too big or complex for the Altos, compile
#and link by hand or write shell scripts.
altosc:
	@echo 'Making C-Kermit $(CKVER) for Altos x86 Xenix 3.0, remote...'
	make wermit \
	"CFLAGS= -DATTSV -DA986 -DNODEBUG -DNOTLOG -DNOSCRIPT -DNODIAL
	-DNOCSETS -DNOANSI -DNOMSEND -DNOSPL -DNOICP -Mm -O" \
	"LNKFLAGS= -Mm -s"

#Altos 986 with Xenix 3.0, as above, but interactive only, minimal size.
altosi:
	@echo 'Making C-Kermit $(CKVER) for Altos x86 Xenix 3.0, local...'
	make wermit \
	"CFLAGS= -DATTSV -DA986 -DNODEBUG -DNOTLOG -DNOSCRIPT -DNODIAL \
	-DNOCSETS -DNOANSI -DNOMSEND -DNOSPL -DNOCMDL -DNOFRILLS -DNOHELP \
	-Mm -O" "LNKFLAGS= -Mm -s"

#MINIX - PC version with 64K+64K limit.
# Reportedly, the linker (asld) can run out of space while linking.  The only
# way around this is to make a copy of libc.a from which all modules that are
# not used by Kermit are removed.  If you have trouble compiling or running
# wart, "touch wart".  If that doesn't help, "touch ckcpro.c".
# The version configured below has no interactive command parser.
# If you can build this version successfully, maybe there will be room for
# a minimal interactive command parser too; try replacing -DNOICP with
# -DNOSPL.
minix:
	@echo 'Making C-Kermit $(CKVER) for MINIX, no command parser...
	@echo 'TOTALLY UNTESTED!'
	make wermit EXT=s \
	"CFLAGS= -DV7 -DMINIX -i -D_MINIX -D_POSIX_SOURCE -DDYNAMIC \
	-DPID_T=pid_t -DUID_T=uid_t -DGID_T=gid_t -DSIGTYP=void \
	-DNOXMIT -DNOMSEND -DNOFRILLS \
	-DNODIAL -DNOHELP -DNODEBUG -DNOTLOG -DNOSCRIPT -DNOCSETS -DNOICP" \
	"LNKFLAGS= -i -T"

#MINIX - PC version with 64K+64K limit, new (as yet unreleased) ACK 2.0 beta C 
#compiler, which outputs .o object files, rather than .s.  But 'make' still
#expects .s files, so must be patched to use .o.  Tested on Minix 1.5.10.
minixnew:
	@echo 'Making C-Kermit $(CKVER) for MINIX (new ACK 2.0 compiler),'
	@echo 'no command parser...  TOTALLY UNTESTED!'
	make wermit \
	"CFLAGS= -DV7 -DMINIX -i -D_MINIX -D_POSIX_SOURCE -DDYNAMIC \
	-DPID_T=pid_t -DUID_T=uid_t -DGID_T=gid_t -DSIGTYP=void \
	-DNODIAL -DNOHELP -DNODEBUG -DNOTLOG -DNOSCRIPT -DNOCSETS -DNOICP" \
	"LNKFLAGS= -i -T"

#MINIX/386 (PC Minix modifed by Bruce Evans in Australia to use 386 addressing)
minix386:
	@echo 'Making C-Kermit $(CKVER) for MINIX/386...
	@echo 'TOTALLY UNTESTED!'
	make wermit EXT=s \
	"CFLAGS= -DV7 -DMINIX -D_POSIX_SOURCE -DDYNAMIC"

#MINIX - 68k version with ACK compiler.
# If you have trouble compiling or running wart, "touch wart".
# If it still doesn't work, "touch ckcpro.c".
# The version configured below has many features removed, including
# the TRANSMIT, MSEND, HELP, and SCRIPT commands, international
# character set support, and the entire script programming language.
# Make sure make(1) has (at least) 100000 chmemory!
# But it does have an interactive command parser.
minix68k:
	@echo 'Making C-Kermit $(CKVER) for MINIX 68k with ACK...
	make wermit \
	"CFLAGS= -DV7 -DMINIX -D_MINIX -D_POSIX_SOURCE -DDYNAMIC \
	-DNODIAL -DNOHELP -DNODEBUG -DNOTLOG \
	-DNOSCRIPT -DNOCSETS -DNOSPL \
	-DPID_T=pid_t -DUID_T=uid_t -DGID_T=gid_t -DSIGTYP=void"

#MINIX - 68k version with c68 compiler.
# If you have trouble compiling or running wart, "touch wart" or
# "touch ckcpro.c". Compiling ckudia.c (no -DNODIAL!) might fail. :-(
# Give c68 250000 bytes of stack+heap; make sure make(1) has at least
# 100000 chmemory.  On a 1Mb Atari ST this means that the recursive
# call of make fails due to memory shortage.  Try "make -n minixc68 >makeit",
# followed by ". makeit".  Otherwise, as above.
minixc68:
	@echo 'Making C-Kermit $(CKVER) for MINIX 68k with c68...
	make wermit "CC= cc -c68" \
	"CFLAGS= -DV7 -DMINIX -D_MINIX -D_POSIX_SOURCE -DDYNAMIC \
	-DNODIAL -DNOHELP -DNODEBUG -DNOTLOG \
	-DNOSCRIPT -DNOCSETS -DNOSPL \
	-DPID_T=pid_t -DUID_T=uid_t -DGID_T=gid_t -DSIGTYP=void"

#MINSUN
# For testing MINIX compilation on SUN-4
# (it compiles, but it doesn't load at all...)
minsun:
	@echo 'Making C-Kermit $(CKVER) for MINIX, no command parser,'
	@echo 'Compilation test on SUN-4...'
	touch ckcpro.c	
	touch wart
	make wermit \
	"CFLAGS= -DV7 -DMINIX -i -D_MINIX -D_POSIX_SOURCE -DDYNAMIC \
	-DNODIAL -DNOHELP -DNODEBUG -DNOTLOG -DNOSCRIPT -DNOCSETS -DNOICP \
	-DSIGTYP=void -Du_long=long -Du_short=short -DS_IFMT=0170000 \
	-DS_IFDIR=0040000 -DS_IFREG=0100000" \
	"LNKFLAGS = -i -T"

#MIPS machine with AT&T UNIX System V R3.0
mips:
	@echo 'Making C-Kermit $(CKVER) for MIPS AT&T System V R3.0...'
	make wermit "CFLAGS = -DMIPS -DDIRENT -DPID_T=int \
	-DNOJC -DGID_T=gid_t -DUID_T=uid_t -i -O1500"

#PFU Compact A Series UNIX System V R3, SX/A TISP V10/L50 (Japan)
#Maybe the -i link option should be removed?
sxae50:
	@echo 'Making C-Kermit $(CKVER) for PFU SX/A V10/L50...'
	make wermit "CFLAGS= -DSVR3 -DDIRENT -DsxaE50 -DTCPSOCKET -i -O" \
	"LNKFLAGS= "

#Olivetti X/OS R2.3
xos23:
	@echo 'Making C-Kermit $(CKVER) for Olivetti X/OS R2.3...'
	$(MAKE) wermit \
		'CFLAGS=-ucb -DBSD4 -DTCPSOCKET -DHDBUUCP -DDYNAMIC -DOXOS' \
		"LNKFLAGS=-ucb -s"		

#Clean up intermediate and object files
clean:
	@echo 'Removing object files...'
	-rm -f ckcmai.$(EXT) ckucmd.$(EXT) ckuusr.$(EXT) ckuus2.$(EXT) \
ckuus3.$(EXT) ckuus4.$(EXT) ckuus5.$(EXT) ckcpro.$(EXT) ckcfns.$(EXT) \
ckcfn2.$(EXT) ckcfn3.$(EXT) ckuxla.$(EXT) ckucon.$(EXT) ckutio.$(EXT) \
ckufio.$(EXT) ckudia.$(EXT) ckuscr.$(EXT) ckwart.$(EXT) ckuusx.$(EXT) \
ckuusy.$(EXT) ckcnet.$(EXT) ckuus6.$(EXT) ckuus7.$(EXT) ckcmdb.$(EXT) \
ckcpro.c wart wermit

#Run Lint on this mess for the SUN/BSD version.
lintsun:
	@echo 'Running Lint on C-Kermit $(CKVER) sources for SUNOS version...'
	lint -x -DSUNOS4 -DDIRENT -DTCPSOCKET -DSAVEDUID -DDYNAMIC \
	ck[cu]*.c > ckuker.lint.sun

lintbsd:
	@echo 'Running Lint on C-Kermit $(CKVER) sources for BSD 4.2 version..'
	lint -x -DBSD4 -DTCPSOCKET ck[cu]*.c > ckuker.lint.bsd42

lints5:
	@echo 'Running Lint on C-Kermit $(CKVER) sources for Sys V version...'
	lint -x -DATTSV ck[cu]*.c > ckuker.lint.s5

lintmips:
	@echo 'Running lint on C-Kermit $(CKVER) sources for MIPS version...'
	lint -DMIPS -DDIRENT -DPID_T=int -DGID_T=gid_t \
	-DUID_T=uid_t -DNOJC ck[cu]*.c > ckuker.lint.mips
