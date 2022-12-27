# Makefile for eqn.

DESTDIR=

SUBDIR=	eqn neqn

all:	FRC
	for i in ${SUBDIR}; do (cd $$i; make); done

install: FRC
	for i in ${SUBDIR}; do \
	  (cd $$i; make ${MFLAGS} DESTDIR=${DESTDIR} install); \
	done

install.man: FRC
	install -c -o bin -g bin -m 444 eqn.man ${DESTDIR}/usr/man/man1/eqn.1
	rm -f ${DESTDIR}/usr/man/man1/checkeq.1
	ln -s eqn.1 ${DESTDIR}/usr/man/man1/checkeq.1
	rm -f ${DESTDIR}/usr/man/man1/neqn.1
	ln -s eqn.1 ${DESTDIR}/usr/man/man1/neqn.1

clean:	FRC
	for i in ${SUBDIR}; do (cd $$i; make ${MFLAGS} clean); done

FRC:
