#
# Mach Operating System
# Copyright (c) 1991,1990,1989 Carnegie Mellon University
# All Rights Reserved.
# 
# Permission to use, copy, modify and distribute this software and its
# documentation is hereby granted, provided that both the copyright
# notice and this permission notice appear in all copies of the
# software, derivative works or modified versions, and any portions
# thereof, and that both notices appear in supporting documentation.
# 
# CARNEGIE MELLON ALLOWS FREE USE OF THIS SOFTWARE IN ITS 
# CONDITION.  CARNEGIE MELLON DISCLAIMS ANY LIABILITY OF ANY KIND FOR
# ANY DAMAGES WHATSOEVER RESULTING FROM THE USE OF THIS SOFTWARE.
# 
# Carnegie Mellon requests users of this software to return to
# 
#  Software Distribution Coordinator  or  Software.Distribution@CS.CMU.EDU
#  School of Computer Science
#  Carnegie Mellon University
#  Pittsburgh PA 15213-3890
# 
# any improvements or extensions that they make and grant Carnegie the
# rights to redistribute these changes.
#
#
# HISTORY
# $Log:	Makefile,v $
# Revision 2.5  91/02/05  17:52:40  mrt
# 	Changed to new Mach copyright
# 	[91/02/01  17:50:46  mrt]
# 
# Revision 2.4  90/08/27  22:09:05  dbg
# 	Merged old CMU changes into Tahoe release.
# 	[90/08/16            dbg]
# 
# Revision 2.3  90/05/03  15:49:57  dbg
# 		Use -lcs for CS library.
# 		Fix install.
# 		[90/01/22            rvb]
# 
# Revision 2.2  89/11/29  14:15:51  af
# 	fopenp() is a CMU thing only found in libcs.a
# 	[89/10/28            af]
# 
# Revision 2.1  89/08/03  18:10:42  rwd
# Created.
# 
# Revision 2.7  89/02/25  19:17:50  gm0w
# 	Changes for cleanup.
# 
# Revision 2.6  89/02/07  22:56:09  mwyoung
# Code cleanup cataclysm.
# 
# Revision 2.5  89/01/23  22:21:19  af
# 	Added def of CMU for those who do not have the light yet.
# 	[89/01/16  17:42:42  af]
# 
# Revision 2.4  88/09/01  13:53:35  mwyoung
# 	Use /usr/cs/etc/install until it's placed correctly.
# 	[88/08/30            mwyoung]
# 
# Revision 2.3  88/07/29  03:20:45  rpd
# Cleaned up some oddities; no real functional changes.
# 
# 24-Mar-88  Mike Accetta (mja) at Carnegie-Mellon University
#	Added DESTDIR, DDIR and SRCS macros and parameterized
#	installation rules so that they can optionally be used to build
#	and install a new version if the sources are newer than the
#	currently installed binary.
#	[ V5.1(XF22) ]
#
# 09-Nov-87  Mike Accetta (mja) at Carnegie-Mellon University
#	Add ${CFILES} to "depend" rule.
#	[ V5.1(XF21) ]
#
# 08-Jan-87  Robert Beck (beck) at Sequent Computer Systems, Inc.
#	Mod config.x rule to take $P (for parallel make).
#
# 09-Dec-86  Mike Accetta (mja) at Carnegie-Mellon University
#	Install in bin/config with the other kernel maintenance
#	programs.
#
# 31-Oct-86  Avadis Tevanian (avie) at Carnegie-Mellon University
#	Build config.x instead of config.  Make install move config.x to
#	config (../../conf/config is a symbolic link to here).
#
# 19-Jul-86  Mike Accetta (mja) at Carnegie-Mellon University
#	Added -q to install and moved destination from /etc to conf.
#

#
# Copyright (c) 1988 Regents of the University of California.
# All rights reserved.
#
# Redistribution and use in source and binary forms are permitted
# provided that the above copyright notice and this paragraph are
# duplicated in all such forms and that any documentation,
# advertising materials, and other materials related to such
# distribution and use acknowledge that the software was developed
# by the University of California, Berkeley.  The name of the
# University may not be used to endorse or promote products derived
# from this software without specific prior written permission.
# THIS SOFTWARE IS PROVIDED ``AS IS'' AND WITHOUT ANY EXPRESS OR
# IMPLIED WARRANTIES, INCLUDING, WITHOUT LIMITATION, THE IMPLIED
# WARRANTIES OF MERCHANTIBILITY AND FITNESS FOR A PARTICULAR PURPOSE.
#
#	@(#)Makefile	5.7 (Berkeley) 6/18/88
#

CFILES_SRC =	main.c mkioconf.c mkmakefile.c \
		mkglue.c mkheaders.c mkswapconf.c
CFILES=	y.tab.c lex.yy.c ${CFILES_SRC}
SRCS=${CFILES_SRC} config.h config.y config.l

OBJS=	y.tab.o main.o lex.yy.o mkioconf.o mkmakefile.o \
	mkglue.o mkheaders.o mkswapconf.o

DDIR=../../bin

CFLAGS=-O -DCMU
#LDFLAGS=-ll -lcs
LDFLAGS=-ll

config.x:$P ${OBJS}
	${CC} ${OBJS} -o config.x ${LDFLAGS}

lex.yy.c: config.l
	lex config.l

y.tab.h y.tab.c: config.y
	yacc -d config.y

clean:
	rm -f ${OBJS} lex.yy.c y.tab.c y.tab.h config.x

${DDIR}/config: ${SRCS}
	@$(MAKE) DDIR=${DDIR} install

install: config.x
#####	install -q -c -s config.x ${DDIR}/config
	cp config.x ${DDIR}/config && strip ${DDIR}/config

lint: ${CFILES}
	lint ${CFILES}


depend: ${CFILES}
	grep '^#include' ${CFILES} | grep -v '<' | \
	sed -e 's/:[^"]*"\([^"]*\)".*/: \1/' \
	    -e 's/\.c/.o/' \
	    -e 's,../[a-zA-Z]*/,,' | \
	awk ' { if ($$1 != prev) { print rec; rec = $$0; prev = $$1; } \
		else { if (length(rec $$2) > 78) { print rec; rec = $$0; } \
		       else rec = rec " " $$2 } } \
	      END { print rec } ' > makedep
	echo '$$r makedep' >>eddep
	echo '/^# DO NOT DELETE THIS LINE/+1,$$d' >eddep
	echo '$$r makedep' >>eddep
	echo 'w' >>eddep
	cp Makefile Makefile.bak
	ed - Makefile < eddep
	rm eddep makedep

# DO NOT DELETE THIS LINE -- make depend uses it

y.tab.o: config.h
lex.yy.o: y.tab.h config.h
main.o: y.tab.h config.h
mkioconf.o: y.tab.h config.h
mkmakefile.o: y.tab.h config.h
mkglue.o: config.h y.tab.h
mkheaders.o: config.h y.tab.h
mkswapconf.o: config.h
