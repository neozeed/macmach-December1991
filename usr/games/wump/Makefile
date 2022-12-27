# Makefile for wump.

DESTDIR=

CFLAGS=	-O -MD

SRCS=	wump.c

OBJS=	wump.o

all:	wump .depend

wump: ${OBJS}
	cc -o $@ ${OBJS}

install: FRC
	install -c -s -o games -g bin -m 500 wump ${DESTDIR}/usr/games/hide
	cd ${DESTDIR}/usr/games; rm -f wump; ln -s dm wump

install.man: FRC
	install -c -o bin -g bin -m 444 wump.man ${DESTDIR}/usr/man/man6/wump.6

clean:	FRC
	rm -f .depend ${OBJS} wump

.depend: ${SRCS}
	md -d -f -u .depend *.d 2>/dev/null

-include .depend

FRC:
