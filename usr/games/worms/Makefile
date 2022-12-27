# Makefile for worms.

DESTDIR=

CFLAGS=	-O -MD

SRCS=	worms.c

OBJS=	worms.o

LIBS=	-lcurses -ltermcap

all:	worms .depend

worms: ${OBJS}
	cc -o $@ ${OBJS} ${LIBS}

install: FRC
	install -c -s -o games -g bin -m 500 worms ${DESTDIR}/usr/games/hide
	cd ${DESTDIR}/usr/games; rm -f worms; ln -s dm worms

install.man: FRC
	install -c -o bin -g bin -m 444 worms.man ${DESTDIR}/usr/man/man6/worms.6

clean:	FRC
	rm -f .depend ${OBJS} worms

.depend: ${SRCS}
	md -d -f -u .depend *.d 2>/dev/null

-include .depend

FRC:
