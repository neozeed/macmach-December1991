# Makefile for lpr.

# Ownerships {see note at the top of lpd.c}
# ROOT		a user that can access any and all files
# DAEMON	someone special
# SPGRP		the group id of the spooling programs
#
CFLAGS=	-O -MD
LIBDIR=	/usr/lib
BINDIR=	/usr/ucb
SPLDIR=	/usr/spool/lpd
ROOT=	root
DAEMON=	daemon
SPGRP=	daemon
L1SRCS=	lpd.c printjob.c recvjob.c displayq.c rmjob.c startdaemon.c \
	lpdchar.c common.c printcap.c
L1OBJS=	lpd.o printjob.o recvjob.o displayq.o rmjob.o startdaemon.o \
	lpdchar.o common.o printcap.o
L2SRCS=	lpr.c startdaemon.c printcap.c
L2OBJS=	lpr.o startdaemon.o printcap.o
L3SRCS=	lpq.c displayq.c common.c printcap.c
L3OBJS=	lpq.o displayq.o common.o printcap.o
L4SRCS=	lprm.c rmjob.c startdaemon.c common.c printcap.c
L4OBJS=	lprm.o rmjob.o startdaemon.o common.o printcap.o
L5SRCS=	lpc.c cmds.c cmdtab.c startdaemon.c common.c printcap.c
L5OBJS=	lpc.o cmds.o cmdtab.o startdaemon.o common.o printcap.o
L6SRCS=	lptest.c
L6OBJS=	lptest.o
L7SRCS=	pac.c printcap.c
L7OBJS=	pac.o printcap.o
SRCS=	lpd.c lpr.c lpq.c lprm.c pac.c lpd.c cmds.c cmdtab.c printjob.c \
	recvjob.c displayq.c rmjob.c startdaemon.c common.c printcap.c \
	lpdchar.c
OBJS=	${L1OBJS} ${L2OBJS} ${L3OBJS} ${L4OBJS} ${L5OBJS} ${L6OBJS} ${L7OBJS}
STD=	lpd lpr lpq lprm lpc lptest pac

all:	${STD} FILTERS VFILTERS .depend

lpd:	${L1OBJS}
	cc -o $@ ${L1OBJS}

lpr:	${L2OBJS}
	cc -o $@ ${L2OBJS}

lpq:	${L3OBJS}
	cc -o $@ ${L3OBJS}

lprm:	${L4OBJS}
	cc -o $@ ${L4OBJS}

lpc:	${L5OBJS}
	cc -o $@ ${L5OBJS}

lptest: ${L6OBJS}
	cc ${CFLAGS} -o $@ ${L6OBJS}

pac:	${L7OBJS}
	cc -o $@ ${L7OBJS}

FILTERS: FRC
	cd filters; make ${MFLAGS}

VFILTERS: FRC
	cd vfilters; make ${MFLAGS}

install: FRC
	install -c -s -o ${ROOT} -g ${SPGRP} -m 6755 lpd ${DESTDIR}/${LIBDIR}/lpd
	install -c -s -o ${ROOT} -g ${SPGRP} -m 6755 lpr ${DESTDIR}/${BINDIR}/lpr
	install -c -s -o ${ROOT} -g ${SPGRP} -m 6755 lpq ${DESTDIR}/${BINDIR}/lpq
	install -c -s -o ${ROOT} -g ${SPGRP} -m 6755 lprm ${DESTDIR}/${BINDIR}/lprm
	install -c -s -o bin -g ${SPGRP} -m 2755 lpc ${DESTDIR}/etc/lpc
	install -c -s -o bin -g bin -m 755 lptest ${DESTDIR}/${BINDIR}/lptest
	install -c -s -o bin -g bin -m 755 pac ${DESTDIR}/usr/etc/pac
	rm -f ${DESTDIR}/etc/pac
	ln -s ../usr/etc/pac ${DESTDIR}/etc/pac
	-[ -d ${DESTDIR}/${SPLDIR} ] || mkdir ${DESTDIR}/${SPLDIR}
	chown ${DAEMON} ${DESTDIR}/${SPLDIR}
	chgrp ${SPGRP} ${DESTDIR}/${SPLDIR}
	chmod 775 ${DESTDIR}/${SPLDIR}
	cd filters; make ${MFLAGS} DESTDIR=${DESTDIR} install
	cd vfilters; make ${MFLAGS} DESTDIR=${DESTDIR} install

install.man: FRC

clean:	FRC
	rm -f .depend ${OBJS} core ${STD}
	cd filters; make ${MFLAGS} clean
	cd vfilters; make ${MFLAGS} clean

tags:	FRC
	ctags ${L1SRCS}
	ctags -a ${L2SRCS}
	ctags -a ${L3SRCS}
	ctags -a ${L4SRCS}
	ctags -a ${L5SRCS}
	ctags -a ${L6SRCS}
	ctags -a ${L7SRCS}
	sort -o tags tags

.depend: ${SRCS}
	md -d -f -u .depend *.d 2>/dev/null

-include .depend

FRC:
