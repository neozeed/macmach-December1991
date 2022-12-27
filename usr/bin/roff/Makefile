# Makefile for roff.

SUBDIR=	nroff troff nroff_term troff_font

all:	FRC
	for i in ${SUBDIR}; do (cd $$i; make); done

install: FRC
	-[ -d ${DESTDIR}/usr/lib/term ] || mkdir ${DESTDIR}/usr/lib/term
	-chmod 555 ${DESTDIR}/usr/lib/term
	-chown root.bin ${DESTDIR}/usr/lib/term
	for i in ${SUBDIR}; do \
	  (cd $$i; make ${MFLAGS} DESTDIR=${DESTDIR} install); \
	done
	-[ -d ${DESTDIR}/usr/lib/tmac ] || mkdir ${DESTDIR}/usr/lib/tmac
	-chmod 555 ${DESTDIR}/usr/lib/tmac
	-chown root.bin ${DESTDIR}/usr/lib/tmac
	install -c -m 444 -o root -g bin tmac/* ${DESTDIR}/usr/lib/tmac
	-[ -d ${DESTDIR}/usr/lib/ms ] || mkdir ${DESTDIR}/usr/lib/ms
	-chmod 755 ${DESTDIR}/usr/lib/ms
	-chown root.bin ${DESTDIR}/usr/lib/ms
	install -c -o bin -g bin -m 444 ms/* ${DESTDIR}/usr/lib/ms

install.man: FRC
	install -c -o bin -g bin -m 444 troff.man ${DESTDIR}/usr/man/man1/troff.1
	install -c -o bin -g bin -m 444 nroff.man ${DESTDIR}/usr/man/man1/nroff.1

clean:	FRC
	for i in ${SUBDIR}; do (cd $$i; make ${MFLAGS} clean); done

FRC:
