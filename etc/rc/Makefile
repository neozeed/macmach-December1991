# Makefile for init.

PROG=	rc

CFGS=	rc rc.afs rc.local rc.man rc.network

DESTDIR=

MANEXT=	8

CFGDIR=	${DESTDIR}/etc/newconfig

MANDIR= ${DESTDIR}/usr/man/man${MANEXT}

all:

install:
	install -c -o bin -g bin -m 444 ${CFGS} ${CFGDIR}

install.man:
	install -c -o bin -g bin -m 444 ${PROG}.man ${MANDIR}/${PROG}.${MANEXT}

clean:
