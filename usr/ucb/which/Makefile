# Makefile for which.

PROG=	which

DESTDIR=

MANEXT=	1

BINDIR=	${DESTDIR}/usr/ucb

MANDIR=	${DESTDIR}/usr/man/man${MANEXT}

all:	FRC

install: FRC
	install -c -o bin -g bin -m 555 ${PROG}.csh ${BINDIR}/${PROG}

install.man: FRC
	install -c -o bin -g bin -m 444 ${PROG}.man ${MANDIR}/${PROG}.${MANEXT}

clean:	FRC

FRC:
