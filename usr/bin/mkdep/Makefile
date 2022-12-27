# Makefile for mkdep.

PROG=	mkdep

DESTDIR=

MANEXT=	1

BINDIR=	${DESTDIR}/usr/bin

MANDIR=	${DESTDIR}/usr/man/man${MANEXT}

IFLAGS=	-o bin -g bin -m 555

all:	FRC

install: FRC
	install -c ${IFLAGS} ${PROG}.sh ${BINDIR}/${PROG}

install.man: FRC
	install -c -o bin -g bin -m 444 ${PROG}.man ${MANDIR}/${PROG}.${MANEXT}

clean:	FRC

FRC:
