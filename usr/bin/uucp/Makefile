# /usr/src/usr.bin/uucp/Makefile for MacMach

# This version expects the 4.2BSD directory reading routines to
# located in libc.a.  If they aren't, augment LIBS.  Also, LIBS
# can be set to reference other object modules and libraries needed
# by uucico, such as -lunet (needed when UNET is defined).
#
# Common object files are assembled into a library {uulib.a}
#
STD=	uucico uucp uux uuxqt uulog uuclean uuname uupoll uusnap \
	uuencode uudecode uusend uucpd uuq acucntrl
# Common object files placed in uulib.a
COMMON=	anlwrk.o anyread.o cfgets.o chkpth.o \
	cpmv.o expfile.o gename.o getargs.o getprm.o \
	getpwinfo.o gnxseq.o lastpart.o logent.o mailst.o \
	prefix.o setline.o subdir.o sysacct.o systat.o \
	ulockf.o uucpname.o versys.o xqt.o uucpdefs.o
SRCS=	${USRCS} ${OSRCS} ${LSRCS}
USRCS=	cico.c cntrl.c conn.c condevs.c gnsys.c pk0.c chksum.c pk1.c tio.c \
	fio.c gio.c imsg.c 
OSRCS=	uuclean.c uucp.c uulog.c uuname.c uupoll.c \
	uusend.c uusnap.c uux.c uuxqt.c uuq.c acucntrl.c
LSRCS=	anlwrk.c anyread.c cfgets.c chkpth.c \
	cpmv.c expfile.c gename.c getargs.c getprm.c getpwinfo.c \
	gnxseq.c lastpart.c logent.c mailst.c prefix.c \
	setline.c subdir.c sysacct.c systat.c ulockf.c \
	uucpdefs.c uucpname.c versys.c xqt.c

#These might be useful on non 4.3bsd systems.
MISC= htonl.c ioctl.c index.c mkdir.c strpbrk.c 

DESTDIR=

# Define P=& if your make supports parallelism (e.g. Sequent)
P=

OWNER=	uucp
GROUP=	daemon
CFLAGS=	-O -MD
LDFLAGS=
LINTFLAGS= -chbap
LIBS= 
LIBDIR=	${DESTDIR}/usr/lib/uucp
BIN=	${DESTDIR}/usr/bin
ETC=	${DESTDIR}/etc
USRETC=	${DESTDIR}/usr/etc
PUBDIR=	${DESTDIR}/usr/spool/uucppublic
SPOOL=	${DESTDIR}/usr/spool/uucp
XQTDIR=	${SPOOL}/XTMP
CORRUPT= ${SPOOL}/CORRUPT
AUDIT=	${SPOOL}/AUDIT
LCK= ${SPOOL}/LCK
LOG= ${SPOOL}/LOG
STST=	${SPOOL}/STST
# The six subdirs {not counting XTMP}.
HOSTNAME=`uuname -l | sed 's/\(.......\).*/\1/'`
SUBDIRS=C. D.${HOSTNAME}X D.${HOSTNAME} D. X. TM.

all:	$(P) ${STD}  .depend

print:	$(SRCS)
	vgrind $?
	vgrind -x index
	touch print

uucp:	$(P) uucp.o uulib.a
	cc -o uucp ${LDFLAGS} uucp.o uulib.a ${LIBS}

uux:	$(P) uux.o uulib.a
	cc -o uux ${LDFLAGS} uux.o uulib.a ${LIBS}

uuxqt:	$(P) uuxqt.o uulib.a
	cc -o uuxqt ${LDFLAGS} uuxqt.o uulib.a ${LIBS}

uucico:	$(P) cico.o cntrl.o conn.o condevs.o gnsys.o pk0.o chksum.o pk1.o \
	tio.o fio.o gio.o imsg.o uulib.a $(P) aculib/aculib.a
	cc -o uucico ${LDFLAGS} cico.o cntrl.o conn.o condevs.o gnsys.o \
	pk0.o chksum.o pk1.o tio.o fio.o gio.o imsg.o \
	uulib.a aculib/aculib.a ${LIBS}

uulog:	$(P) uulog.o uulib.a
	cc -o uulog ${LDFLAGS} uulog.o uulib.a ${LIBS}

uuclean: $(P) uuclean.o uulib.a
	cc -o uuclean ${LDFLAGS} uuclean.o uulib.a ${LIBS}

uuname:	$(P) uuname.o uulib.a
	cc -o uuname ${LDFLAGS} uuname.o uulib.a ${LIBS}

uupoll:	$(P) uupoll.o uulib.a
	cc -o uupoll ${LDFLAGS} uupoll.o uulib.a ${LIBS}

uusnap:	uusnap.o
	cc -o uusnap ${LDFLAGS} uusnap.o ${LIBS}

uuencode: uuencode.o
	cc -o uuencode ${LDFLAGS} uuencode.o ${LIBS}

uudecode: uudecode.o
	cc -o uudecode ${LDFLAGS} uudecode.o ${LIBS}

uusend: uusend.o
	cc -o uusend ${LDFLAGS} uusend.o uulib.a ${LIBS}

uucpd: uucpd.o uucp.h
	cc -o uucpd ${LDFLAGS} uucpd.o ${LIBS}

uuq: $(P) uuq.o uulib.a
	cc -o uuq ${LDFLAGS} uuq.o uulib.a ${LIBS}

acucntrl: acucntrl.o
	cc -o acucntrl ${LDFLAGS} acucntrl.o ${LIBS}

# Note: mode 700 on ${LIBDIR} would be safer.
# Also,  L.sys etc should be 600.

mkdirs:	FRC
	-for i in ${LIBDIR} ${SPOOL} ${PUBDIR} ${XQTDIR} ${CORRUPT} \
		${AUDIT} ${LCK} ${LOG} ${STST} ; do \
		rm -f $$i ; mkdir $$i ; \
		chmod 755 $$i; /etc/chown ${OWNER} $$i; chgrp ${GROUP} $$i ; \
	done
	chmod 777 ${PUBDIR} ${LCK}
	-cd ${LOG}; for i in uucico uux uuxqt uucp xferstats; do\
		rm -f $$i ; mkdir $$i ; \
		chmod 755 $$i; /etc/chown ${OWNER} $$i; chgrp ${GROUP} $$i ; \
	done
	-cd ${SPOOL}; for i in ${SUBDIRS} ; do \
		mkdir $$i; \
		/etc/chown ${OWNER} $$i; chgrp ${GROUP} $$i; chmod 755 $$i; \
	done

# The lorder...tsort below is for systems w/o ranlib
# But it doesnt hurt to use it. Ignore the spurious "cycle" messages
uulib.a: $(P) ${COMMON}
	rm -f $@
	ar cr $@ `lorder ${COMMON} | tsort`
	ranlib $@

aculib/aculib.a:
	cd aculib; make ${MFLAGS} "CFLAGS=${CFLAGS}" "P=${P}"

install: mkdirs
	for i in uucico uuclean; do \
	  install -c -s -m 6555 -o ${OWNER} -g ${GROUP} $$i ${LIBDIR} ;\
	done
	for i in uucp uux uulog uuname uusnap uupoll uuq; do \
	  install -c -s -m 6555 -o ${OWNER} -g ${GROUP} $$i ${BIN} ;\
	done
	install -c -s -m 6554 -o ${OWNER} -g ${GROUP} uuxqt ${LIBDIR}
	install -c -s -m 6554 -o root -g ${GROUP} acucntrl ${LIBDIR}
	install -c -s -m 544 -o uucp uucpd ${USRETC}
	rm -f ${ETC}/uucpd
	ln -s ../usr/etc/uucpd ${ETC}/uucpd
	@echo Dont forget to fork off /etc/uucpd in /etc/rc.local
	@echo if you are not running /etc/inetd
	install -c -s -m 755 -o bin -g bin uusend ${BIN}
	install -c -s -m 755 -o bin -g bin uuencode ${BIN}
	install -c -s -m 755 -o bin -g bin uudecode ${BIN}
	rm -f ${BIN}/ruusend
	ln -s uusend ${BIN}/ruusend

install.man: FRC
	install -c -o bin -g bin -m 444 uucico.man ${DESTDIR}/usr/man/man8/uucico.8
	rm -f ${DESTDIR}/usr/man/man8/uucpd.8
	ln -s uucico.8 ${DESTDIR}/usr/man/man8/uucpd.8
	rm -f ${DESTDIR}/usr/man/man1/uudecode.1
	ln -s uuencode.1 ${DESTDIR}/usr/man/man1/uudecode.1

clean:	FRC
	rm -f .depend *.o ${STD} uulib.a
	cd aculib; make ${MFLAGS} clean

.depend:
	md -d -f -u .depend *.d 2>/dev/null

-include .depend

FRC:
