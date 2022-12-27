# Makefile for makedev.

DESTDIR=

MANEXT=	8

MANDIR=	${DESTDIR}/usr/man/man${MANEXT}

CONFDIR= ${DESTDIR}/etc/newconfig

all:

install:
	install -c -o bin -g bin -m 444 MAKEDEV.sh ${CONFDIR}/MAKEDEV

install.man:
	install -c -o bin -g bin -m 444 makedev.man ${MANDIR}/makedev.${MANEXT}

clean:

