# Makefile for mklost+found.

PROG= mklost+found

DESTDIR=

MANEXT=8

BINDIR=${DESTDIR}/etc

MANDIR=${DESTDIR}/usr/man/man${MANEXT}

IFLAGS=	-o bin -g bin -m 544

all: ${PROG}.sh

install:
	install -c ${IFLAGS} ${PROG}.sh ${BINDIR}/${PROG}

install.man:
	install -c -o bin -g bin -m 444 ${PROG}.man ${MANDIR}/${PROG}.${MANEXT}

clean:
