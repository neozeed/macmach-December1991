# Makefile for bcd.

DESTDIR=

CFLAGS=	-O -MD

SRCS=	bcd.c

OBJS=	bcd.o

all:	bcd .depend

bcd: ${OBJS}
	cc -o $@ ${OBJS}

install: FRC
	install -c -s -o games -g bin -m 500 bcd ${DESTDIR}/usr/games/hide
	cd ${DESTDIR}/usr/games; rm -f bcd; ln -s dm bcd

install.man: FRC
	install -c -o bin -g bin -m 444 bcd.man ${DESTDIR}/usr/man/man6/bcd.6

clean:	FRC
	rm -f .depend ${OBJS} bcd

.depend: ${SRCS}
	md -d -f -u .depend *.d 2>/dev/null

-include .depend

FRC:
