# Makefile for false.
PROG=	false

DESTDIR=

MANEXT=1

BINDIR=	${DESTDIR}/bin

MANDIR=${DESTDIR}/usr/man/man${MANEXT}

IFLAGS=	-o bin -g bin -m 555

all:	${PROG}.sh

install: FRC
	install -c ${IFLAGS} ${PROG}.sh ${BINDIR}/${PROG}

install.man: FRC
	install -c -o bin -g bin -m 444 ${PROG}.man ${MANDIR}/${PROG}.${MANEXT}

clean:	${PROG}.sh

FRC:
