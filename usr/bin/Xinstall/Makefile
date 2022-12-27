# Makefile for install.

DESTDIR=

MANEXT=	1

BINDIR=	${DESTDIR}/usr/bin

MANDIR=	${DESTDIR}/usr/man/man${MANEXT}

CFLAGS=	-O -MD

SRCS=	install.c

OBJS=	install.o

all:	Xinstall .depend

Xinstall: ${OBJS}
	cc -o $@ ${OBJS}

install: FRC
	install -c -s -o bin -g bin -m 555 Xinstall ${BINDIR}/install

install.man: FRC
	install -c -o bin -g bin -m 444 install.man ${MANDIR}/install.${MANEXT}

clean:	FRC
	rm -f .depend ${OBJS} Xinstall

.depend: ${SRCS}
	md -d -f -u .depend *.d 2>/dev/null

-include .depend

FRC:
